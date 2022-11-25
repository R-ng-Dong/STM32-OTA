#ifndef _BOOTFLASH_
#define _BOOTFLASH_

#include "../Common/include.h"


void Flash_InitData (void);
uint8_t Flash_ReadBank (uint32_t address, uint32_t *data);
uint8_t Flash_WriteBank (uint32_t address, uint32_t *data);
uint8_t Flash_ReadWord (uint32_t address, uint32_t *data);
uint8_t Flash_WriteWord (uint32_t address, uint32_t data);


#endif


