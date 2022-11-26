#include "Flash.h"


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
		address = address + 4;
	}
	return 0;
}

uint8_t Flash_WriteBank (uint32_t address, uint32_t *data){
	uint16_t i;
	
	if((address % FLASH_BLOCK_SIZE) != 0){
		return 1;
	}
	FLASH_Unlock();
	FLASH_ErasePage(((address + FLASH_BLOCK_SIZE - 1)/FLASH_BLOCK_SIZE) * FLASH_BLOCK_SIZE);
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
	uint32_t tmpData[256];
	
	Flash_ReadBank(((address + FLASH_BLOCK_SIZE - 1)/FLASH_BLOCK_SIZE) * FLASH_BLOCK_SIZE, tmpData);
	tmpData[(address % FLASH_BLOCK_SIZE)/4] = data;
	//FLASH_Unlock();
	//FLASH_ErasePage(((address + FLASH_BLOCK_SIZE - 1)/FLASH_BLOCK_SIZE) * FLASH_BLOCK_SIZE);
	Flash_WriteBank(((address + FLASH_BLOCK_SIZE - 1)/FLASH_BLOCK_SIZE) * FLASH_BLOCK_SIZE, tmpData);
	//FLASH_Lock();
}