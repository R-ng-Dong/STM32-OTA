#include "Flash.h"

static uint32_t tmpReadData[FLASH_WORD_PER_BLOCK];

void Flash_InitData (void){
}


uint8_t Flash_ReadBank (uint32_t address, uint32_t *data){
	uint16_t i;
	
	flashDebug("Read:0x%x\n", address);
	if((address % FLASH_BLOCK_SIZE) != 0){
		return 1;
	}
	for (i = 0; i < FLASH_WORD_PER_BLOCK; i++){
		*data = *(volatile uint32_t *)(address);
		data++;
		address += 4;
	}
	return 0;
}

uint8_t Flash_WriteBank (uint32_t address, uint32_t *data){
	uint16_t i;
	
	flashDebug("Write:0x%x\n", address);
	if((address % FLASH_BLOCK_SIZE) != 0){
		return 1;
	}
	
	flashDebug("Unlock\n");
	FLASH_Unlock();
	flashDebug("Clear\n");
	FLASH_ErasePage(address);
	flashDebug("Write\n");
	for (i = 0; i < FLASH_WORD_PER_BLOCK; i++){
		if(FLASH_ProgramWord(address, *data) != FLASH_COMPLETE){
			flashDebug("Write Error\n");
		}
		address += 4;
		data++;
	}
	flashDebug("Lock\n");
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


