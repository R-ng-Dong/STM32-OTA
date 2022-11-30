#ifndef __OTA__
#define __OTA__

#include "FileManager.h"
#include "COMPort.h"


uint8_t ota_calculateCRCStep1 (const uint8_t *data, uint32_t len);
uint8_t ota_calculateCRC(const uint8_t *data, uint32_t len);
void ota_sendAPackage (const uint8_t *data, uint32_t len);
void ota_sendFirmwareVerion (uint16_t major, uint16_t minor);
void ota_sendFirmwareLength (uint32_t length);
void ota_sendFirmwareCRC (uint8_t crc);


#endif /* AEAC1D89_1DFE_4FFD_A2EB_B547E0CDDDD1 */
