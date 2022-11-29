#ifndef _MEMORY_INTERFACE_
#define _MEMORY_INTERFACE_

#include "../Peripheral/Flash.h"
#include "../Common/include.h"

#define BOOTLOADER_ADDRESS		(0x08000000)
#define BOOTLOADER_SHAREDATA	(0x08003F00)

#define BOOTLOADER_CURRENT_VER	(0x08003F20)
#define BOOTLOADER_CURRENT_LEN	(0x08003F24)
#define BOOTLOADER_CURRENT_CRC	(0x08003F28)

#define BOOTLOADER_TEMP_VER		(0x08003F40)
#define BOOTLOADER_TEMP_LEN		(0x08003F44)
#define BOOTLOADER_TEMP_CRC		(0x08003F48)

#define BOOTLOADER_ERROR_CODE	(0x08003F60)

#define MAIN_PROG_ADDRESS		(0x08004000)
#define TEMP_PROG_ADDRESS		(0x08028000)

#define MEMORY_INTERFACE_DEBUG 0
#if MEMORY_INTERFACE_DEBUG
#define memoryDebug(...) printf("[MEMORY_INTERFACE]"); printf(__VA_ARGS__)
#else
	#define memoryDebug(...)
#endif


void MemInterface_Init(void);

uint8_t MemInterface_calculateCRC (uint8_t *data, uint16_t len);

/* Information of Current firmware*/
uint32_t MemInterface_getCurrentVersion(void);
void MemInterface_setCurrentVersion(uint32_t version);
uint32_t MemInterface_getCurrentFirmLength(void);
void MemInterface_setCurrentFirmLength(uint32_t length);
uint8_t MemInterface_getCurrentCRC(void);
void MemInterface_setCurrentCRC(uint8_t crc);

/* Information of Temp firmware*/
uint32_t MemInterface_getTempVersion(void);
void MemInterface_setTempVersion(uint32_t version);
uint32_t MemInterface_getTempFirmLength(void);
void MemInterface_setTempFirmLength(uint32_t length);
uint8_t MemInterface_getTempCRC(void);
void MemInterface_setTempCRC(uint8_t crc);

/* Copy program from source to destination*/
void MemInterface_writeProgram(uint32_t address, uint32_t *data, uint32_t length);
void MemInterface_copyProgram(uint32_t source, uint32_t destination, uint32_t length);


#endif

