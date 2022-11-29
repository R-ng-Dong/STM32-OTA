#include "UART_OTA.h"


static uint8_t dataReceived[256];
static uint8_t headerReceived;
static uint16_t lengthMessage;
static otaDataFrame_t *refDataFrame;

void UART_OTA_Init(void){
    otaDebug("Init process!\n");
    MessageProcess_Init();

    refDataFrame = (otaDataFrame_t *)dataReceived;
}


static void OTA_ProcessDataMessage (void){
    uint16_t lengthData;
    uint16_t sendCRC;
    uint16_t receiveCRC;

    lengthData = refDataFrame->Length;
    sendCRC = refDataFrame->Message[lengthData];
    receiveCRC = MemInterface_calculateCRC(refDataFrame->Message, lengthData);

    if(sendCRC != receiveCRC){
        return;
    }

    MemInterface_writeProgram(TEMP_PROG_ADDRESS + refDataFrame->Location, refDataFrame->Message, lengthData);
}

static void OTA_ProcessCRCMessage (void){
    uint8_t crc;

    crc = dataReceived[0];

    MemInterface_setTempCRC(crc);
}

static void OTA_ProcessLengthMessage (void){
    uint16_t length;

    length = (dataReceived[0] << 8) | dataReceived[1];

    MemInterface_setTempFirmLength(length);
}

static void OTA_ProcessVersionMessage (void){
    uint16_t major, minor;
    uint32_t version;

    major = dataReceived[0];
    minor = dataReceived[1];
    version = (major << 16) | minor;

    MemInterface_setTempVersion(version);
}


void UART_OTA_Process(void){
    if(MessageProcess_hasMessage()){
        lengthMessage = MessageProcess_copyMessage(dataReceived, &headerReceived);
        otaDebug("Has new message. Header:0x%0x\n", headerReceived);
        switch (headerReceived)
        {
        case OTA_HEADER_START:
            /*TODO: Clear Flash before*/
            break;
        case OTA_HEADER_STOP:
            /*TODO: Check Information of Data*/
            break;
        
        case OTA_HEADER_DATA:
            OTA_ProcessDataMessage();
            break;

        case OTA_HEADER_LENGTH:
            OTA_ProcessLengthMessage();
            break;

        case OTA_HEADER_CRC:
            OTA_ProcessCRCMessage();
            break;

        case OTA_HEADER_VERSION:
            OTA_ProcessVersionMessage();
            break;

        case OTA_HEADER_REBOOT:
            NVIC_SystemReset();
            break;

        default:
            break;
        }
    }
}

void UART_OTA_onLoop(void){
    MessageProcess_Process();
}