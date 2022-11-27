#ifndef _BOOTLOADER_
#define _BOOTLOADER_

#include "../Common/include.h"
#include "MemoryInterface.h"




typedef union {
    uint32_t dataFlash;
    struct {
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


