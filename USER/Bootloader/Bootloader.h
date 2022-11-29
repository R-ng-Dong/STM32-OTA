/**
 * @file Bootloader.h
 * @author Duan Luong (duanlc@ptit.edu.vn)
 * @brief 
 * @version 0.1
 * @date 2022-11-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef _BOOTLOADER_
#define _BOOTLOADER_

#include "../Common/include.h"
#include "MemoryInterface.h"

#define BOOTLOADER_DEBUG 1
#if 	BOOTLOADER_DEBUG
#define bootloaderDebug(...) printf("[BOOTLOADER]"); printf(__VA_ARGS__)
#else
	#define bootloaderDebug(...)
#endif

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



void    Bootloader_Init (void);
uint8_t Bootloader_CheckDiffVersion (void);
void 	Bootloader_RunProgram (void);
uint8_t Bootloader_CopyTemp2Main (void);
void 	Bootloader_Processing (void);



#endif


