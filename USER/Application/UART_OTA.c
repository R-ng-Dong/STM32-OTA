#include "UART_OTA.h"


static uint8_t dataReceived[256];
static uint8_t headerReceived;
static uint16_t lengthMessage;
static otaDataFrame_t *refDataFrame;


/**
 * @brief Initialize resources for UART OTA process
 * 
 */
void UART_OTA_Init(void){
    otaDebug("Init process!\n");
    MessageProcess_Init();

    refDataFrame = (otaDataFrame_t *)dataReceived;
}


/**
 * @brief Pre-process before write data to the Flash memory
 * 
 * @param lengthInput number of bytes need to be write
 */
static void OTA_ProcessDataMessage (uint16_t lengthInput){
	uint32_t locationData;
    uint16_t lengthData;
    uint16_t sendCRC;
    uint16_t receiveCRC;
	uint16_t i;
	uint32_t *dataSave;
	
	locationData = 	(refDataFrame->Location[0]<<24) | \
					(refDataFrame->Location[1]<<16) | \
					(refDataFrame->Location[2]<<8) | \
					(refDataFrame->Location[3]<<0);
    lengthData = 	refDataFrame->Length[0] << 8 | \
					refDataFrame->Length[1] << 0;
    sendCRC = refDataFrame->Message[lengthData];
    receiveCRC = MemInterface_calculateCRC(refDataFrame->Message, lengthData);
	dataSave = (uint32_t *)refDataFrame->Message;
	
	otaDebug("Location: 0x%8x\n", locationData);
	otaDebug("Length:%d\n", lengthData);
	otaDebug("CRC:0x%2x - 0x%2x\n", receiveCRC, sendCRC);
    
    if(sendCRC != receiveCRC){
		otaDebug("Wrong CRC!\n");
        return;
    }
	if(lengthData %4 != 0){
		otaDebug("Wrong Nbr of Data!\n");
        return;
	}
	
	if(lengthInput != (lengthData + 9)){
		otaDebug("Wrong number of message!\n");
		return;
	}
	
    MemInterface_writeProgram(TEMP_PROG_ADDRESS + locationData, dataSave, lengthData/4);
	otaDebug("Flash done!\n");
}

/**
 * @brief Write the CRC information to the Flash memory
 * 
 */
static void OTA_ProcessCRCMessage (void){
    uint8_t crc;

    crc = dataReceived[0];

    MemInterface_setTempCRC(crc);
}

/**
 * @brief Write the length of program information to the Flash memory
 * 
 */
static void OTA_ProcessLengthMessage (void){
    uint16_t length;

    length = (dataReceived[0] << 8) | dataReceived[1];

    MemInterface_setTempFirmLength(length);
}

/**
 * @brief Write the version of program information to the Flash memory
 * 
 */
static void OTA_ProcessVersionMessage (void){
    uint16_t major, minor;
    uint32_t version;

    major = dataReceived[0];
    minor = dataReceived[1];
    version = (major << 16) | minor;

    MemInterface_setTempVersion(version);
}

/**
 * @brief Main OTA process. Read the command, message and write to the Flash memory
 * 
 */
void UART_OTA_Process(void){
    if(MessageProcess_hasMessage()){
        lengthMessage = MessageProcess_copyMessage(dataReceived, &headerReceived);
        otaDebug("Has new message. Header:0x%0x\n", headerReceived);
        switch (headerReceived)
        {
        case OTA_HEADER_START:
			otaDebug("Start!\n");
            /*TODO: Clear Flash before*/
            break;
        case OTA_HEADER_STOP:
			otaDebug("Stop!\n");
            /*TODO: Check Information of Data*/
            break;
        
        case OTA_HEADER_DATA:
			otaDebug("Update Data to Flash!\n");
            OTA_ProcessDataMessage(lengthMessage);
            break;

        case OTA_HEADER_LENGTH:
			otaDebug("Write Length!\n");
            OTA_ProcessLengthMessage();
            break;

        case OTA_HEADER_CRC:
			otaDebug("Write CRC!\n");
            OTA_ProcessCRCMessage();
            break;

        case OTA_HEADER_VERSION:
			otaDebug("Write Version!\n");
            OTA_ProcessVersionMessage();
            break;

        case OTA_HEADER_REBOOT:
            otaDebug("Reboot!\n");
			Fn_DELAY_ms(100);
            NVIC_SystemReset();
            break;

        default:
            break;
        }
    }
}

/**
 * @brief On loop function for check the Input command
 * 
 */
void UART_OTA_onLoop(void){
    MessageProcess_Process();
}