#include "OTA.h"



static void ota_sendStartFrame (void){
    uint8_t header[2] = {0xAA, 0x55};

    comPort_Send(header, 2);
}

static void ota_sendHeader (uint8_t header){
    uint8_t headerSend[1];

    headerSend[0] = header;

    comPort_Send(headerSend, 1);
}

static void ota_sendByte (uint8_t dataByte){
    uint8_t headerSend[1];

    headerSend[0] = dataByte;

    comPort_Send(headerSend, 1);
}


static void ota_lengthMessage (uint16_t length){
    uint8_t messageSend[2];

    messageSend[1] = length & 0xFF; length >>= 8;
    messageSend[0] = length & 0xFF;

    comPort_Send(messageSend, 2);
}

uint8_t ota_calculateCRCStep1 (const uint8_t *data, uint32_t len){
    uint8_t crc = 0;

    for (uint32_t i = 0; i < len; i++){
        crc += data[i];
    }

    return crc;
}
uint8_t ota_calculateCRC(const uint8_t *data, uint32_t len){
    uint8_t crc = 0;

    for (uint32_t i = 0; i < len; i++){
        crc += data[i];
    }

    crc = 0xFF - crc;
    return crc;
}
void ota_sendAPackage (const uint8_t *data, uint32_t len, uint32_t address){
    uint8_t dataSend[6];
    uint8_t crc;
    uint32_t lengthTemp;

    if(len % 4 != 0){
        printf("Wrong length of Data!\n");
        return;
    }

    crc = ota_calculateCRC(data, len);
    /*TODO: Send Data + CRC*/
    dataSend[3] = address & 0xFF; address >>= 8;
    dataSend[2] = address & 0xFF; address >>= 8;
    dataSend[1] = address & 0xFF; address >>= 8;
    dataSend[0] = address & 0xFF;

    lengthTemp = len;
    dataSend[5] = lengthTemp & 0xFF; lengthTemp >>= 8;
    dataSend[4] = lengthTemp & 0xFF;

    uint32_t lengthSum = MESSAGE_HEADERLENGTH + len + 3;
    crc = ota_calculateCRC(data, len);

    ota_sendStartFrame();
    ota_lengthMessage(lengthSum);
    ota_sendHeader(HEADER_DATA);
    comPort_Send(dataSend, 6);
    comPort_Send(data, len);
    ota_sendByte(crc);
    ota_sendByte(0xAA);

}

void ota_sendFirmwareVerion (uint8_t major, uint8_t minor){
    uint8_t dataSend[2];

    dataSend[0] = major;
    dataSend[1] = minor;

    uint32_t lengthSum = MESSAGE_OFFSETLENGTH + 2;
    ota_sendStartFrame();
    ota_lengthMessage(lengthSum);
    ota_sendHeader(HEADER_VERSION);
    comPort_Send(dataSend, 2);
    ota_sendByte(0xAA);
}

void ota_sendFirmwareLength (uint32_t length){
    uint8_t dataSend[2];

    dataSend[1] = length & 0xFF;    length >>= 8;
    dataSend[0] = length & 0xFF;

    uint32_t lengthSum = MESSAGE_OFFSETLENGTH + 2;
    ota_sendStartFrame();
    ota_lengthMessage(lengthSum);
    ota_sendHeader(HEADER_LENGTH);
    comPort_Send(dataSend, 2);
    ota_sendByte(0xAA);
}

void ota_sendFirmwareCRC (uint8_t crc){
    uint8_t dataSend[1];

    dataSend[0] = crc;
    uint32_t lengthSum = MESSAGE_OFFSETLENGTH + 1;

    ota_sendStartFrame();
    ota_lengthMessage(lengthSum);
    ota_sendHeader(HEADER_CRC);
    comPort_Send(dataSend, 1);
    ota_sendByte(0xAA);
}

void ota_sendReboot (void){
    uint32_t lengthSum = MESSAGE_OFFSETLENGTH;

    ota_sendStartFrame();
    ota_lengthMessage(lengthSum);
    ota_sendHeader(HEADER_REBOOT);
    ota_sendByte(0xAA);
}

uint8_t     bufferTemp[MAX_FRAME_SIZE];

void ota_sendOTAFile (const char *fileOpen, uint16_t major, uint16_t minor){
    uint32_t    sizeBinFile;

    printf("OTA Start!\n");
    fileManager_OpenFile(fileOpen);
    sizeBinFile = fileManager_getSize();
    printf("Get size:%d\n", sizeBinFile);
    fileManager_gotoStart();

    /* Send all package */
    uint8_t     crcSum = 0x00;
    uint16_t    lengthRead;
    uint32_t    locationData = 0;
    lengthRead = MAX_FRAME_SIZE;
    printf("Send all package!\n");
    while (lengthRead == MAX_FRAME_SIZE){
        lengthRead = fileManager_getData(bufferTemp, MAX_FRAME_SIZE);
        crcSum = crcSum + ota_calculateCRCStep1(bufferTemp, lengthRead);
        printf("-------------\nRead:%d bytes\n", lengthRead);
        ota_sendAPackage(bufferTemp, lengthRead, locationData);
        locationData += lengthRead;
        printf("UART send:%d bytes\n", lengthRead);
        Sleep(100);
    }

    crcSum = 0xFF - crcSum;

    /* Send Length */
    printf("Send length file!\n");
    ota_sendFirmwareLength(sizeBinFile);

    /* Send CRC */
    printf("Checksum:0x%2x\n", crcSum);
    printf("Send CRC file!\n");
    ota_sendFirmwareCRC(crcSum);

    /* Send Version */
    printf("Send version!\n");
    ota_sendFirmwareVerion(major & 0xFF, minor & 0xFF);

    /* Reboot */
    printf("Reboot...\n");
    //ota_sendReboot();
}