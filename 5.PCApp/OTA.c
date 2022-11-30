#include "OTA.h"


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
void ota_sendAPackage (const uint8_t *data, uint32_t len){
    uint8_t crc = 0;

    crc = ota_calculateCRC(data, len);
    /*TODO: Send Data + CRC*/

}

void ota_sendFirmwareVerion (uint16_t major, uint16_t minor){
    uint32_t version;

    version = (major << 16) + minor;
}

void ota_sendFirmwareLength (uint32_t length){

}

void ota_sendFirmwareCRC (uint8_t crc){
    
}