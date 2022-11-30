#ifndef __OTA__
#define __OTA__

#include "FileManager.h"
#include "COMPort.h"

#define HEADER_START    (0x01)
#define HEADER_STOP     (0x02)
#define HEADER_DATA     (0x03)
#define HEADER_LENGTH   (0x04)
#define HEADER_CRC      (0x05)
#define HEADER_VERSION  (0x06)
#define HEADER_REBOOT   (0x07)

#define MESSAGE_HEADERLENGTH (6)
#define MESSAGE_OFFSETLENGTH (2)


uint8_t ota_calculateCRCStep1 (const uint8_t *data, uint32_t len);
uint8_t ota_calculateCRC(const uint8_t *data, uint32_t len);
void ota_sendAPackage (const uint8_t *data, uint32_t len, uint32_t address);
void ota_sendFirmwareVerion (uint8_t major, uint8_t minor);
void ota_sendFirmwareLength (uint32_t length);
void ota_sendFirmwareCRC (uint8_t crc);
void ota_sendReboot (void);
void ota_sendOTAFile (const char *fileOpen);


#endif /* AEAC1D89_1DFE_4FFD_A2EB_B547E0CDDDD1 */
