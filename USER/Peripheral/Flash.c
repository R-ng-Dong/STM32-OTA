#include "Flash.h"

static uint32_t tmpReadData[256];

void Flash_InitData (void){
}


uint8_t Flash_ReadBank (uint32_t address, uint32_t *data){
	uint16_t i;
	
	if((address % FLASH_BLOCK_SIZE) != 0){
		return 1;
	}
	for (i = 0; i < 256; i++){
		*data = *(volatile uint32_t *)(address);
		data++;
		address += 4;
	}
	return 0;
}

uint8_t Flash_WriteBank (uint32_t address, uint32_t *data){
	uint16_t i;
	
	if((address % FLASH_BLOCK_SIZE) != 0){
		return 1;
	}
	
	
	FLASH_Unlock();
	FLASH_ErasePage(address);
	for (i = 0; i < 256; i++){
		FLASH_ProgramWord(address, *data);
		address += 4;
		data++;
	}
	FLASH_Lock();
	return 0;
}

uint32_t Flash_ReadWord (uint32_t address){
	uint32_t data;
	
	data = *(volatile uint32_t *)(address);
	
	return data;
}

uint8_t Flash_WriteWord (uint32_t address, uint32_t data){
	uint32_t locationPage;
	
	if((address % 4) != 0){
		return 1;
	}
	
	locationPage = ((address + 1)/FLASH_BLOCK_SIZE) * FLASH_BLOCK_SIZE;
	
	Flash_ReadBank(locationPage, tmpReadData);
	tmpReadData[(address % FLASH_BLOCK_SIZE)/4] = data;
	Flash_WriteBank(locationPage, tmpReadData);
	
	return 0;
}


