#ifndef _BOOTLOADER_
#define _BOOTLOADER_

#include "../Common/include.h"
#include "../Peripheral/Flash.h"


#define BOOTLOADER_ADDRESS		(0x08000000)
#define BOOTLOADER_SHAREDATA	(0x08001F00)

#define BOOTLOADER_CURRENT_VER	(0x08001F20)
#define BOOTLOADER_CURRENT_LEN	(0x08001F24)
#define BOOTLOADER_CURRENT_CRC	(0x08001F28)

#define BOOTLOADER_PROGNEXT_VER	(0x08001F40)
#define BOOTLOADER_PROGNEXT_LEN	(0x08001F44)
#define BOOTLOADER_PROGNEXT_CRC	(0x08001F48)


#define MAIN_PROG_ADDRESS		(0x08002000)
#define TEMP_PROG_ADDRESS		(0x08009000)


typedef union {
    uint32_t dataFlash;
    struct 
    {
        uint16_t major;
        uint16_t minor;
    } version;
} bootVersion_t;

typedef enum{
    Init = 0,
    CheckDiff,
    CheckSum,
    CopyTmp2Src,
    Jmp2Main
} bootStages_t;

typedef enum{
    CurrentProg = 0,
    NextProg,
} bootProgram_t;



void Bootloader_Init (void);
uint8_t Bootloader_CheckDiffVersion (void);
uint8_t Bootloader_GetCheckSum (bootProgram_t prog);
uint8_t Bootloader_CalCheckSum (bootProgram_t prog);
void 	Bootloader_GotoProgram (uint32_t address);
uint8_t Bootloader_CopyTemp2Main (void);



#endif


