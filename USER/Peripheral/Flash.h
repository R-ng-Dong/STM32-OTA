#ifndef _BOOTFLASH_
#define _BOOTFLASH_

#include "../Common/include.h"

#define FLASH_BLOCK_SIZE    	(2048)
#define FLASH_WORD_PER_BLOCK	(FLASH_BLOCK_SIZE/4)

#define FLASH_DEBUG 0
#if FLASH_DEBUG
#define flashDebug(...) printf("[FLASH]"); printf(__VA_ARGS__)
#else
	#define flashDebug(...)
#endif


void Flash_InitData (void);
uint8_t Flash_ReadBank (uint32_t address, uint32_t *data);
uint8_t Flash_WriteBank (uint32_t address, uint32_t *data);
uint32_t Flash_ReadWord (uint32_t address);
uint8_t Flash_WriteWord (uint32_t address, uint32_t data);


#endif


