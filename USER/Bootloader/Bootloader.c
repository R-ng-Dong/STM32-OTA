#include "Bootloader.h"

static bootStages_t gBootloaderStatic = Init;

/**
 * @brief Init the Bootloader program.
 * 
 */
void Bootloader_Init (void){
	Flash_InitData();
}

/**
 * @brief Check if have different version between current and next programs.
 * 
 * @return uint8_t 0: same version
 * @return uint8_t 1: different version
 */
uint8_t Bootloader_CheckDiffVersion (void){
	bootVersion_t currentVerion;
	bootVersion_t nextVersion;
	
	currentVerion.dataFlash = Flash_ReadWord(BOOTLOADER_CURRENT_VER);
	nextVersion.dataFlash = Flash_ReadWord(BOOTLOADER_PROGNEXT_VER);

	if(currentVerion.dataFlash != nextVersion.dataFlash){
		return 1;
	}

	return 0;

}

/**
 * @brief Get the checksum of the program. Read in the Flash.
 * 
 * @param prog Program need to read
 * @return uint8_t checksum value
 */
uint8_t Bootloader_GetCheckSum (bootProgram_t prog){
	uint8_t checksum = 0;

	if(prog == CurrentProg){
		checksum = Flash_ReadWord(BOOTLOADER_CURRENT_CRC) & 0xFF;
	}
	else{
		checksum = Flash_ReadWord(BOOTLOADER_PROGNEXT_CRC) & 0xFF;
	}

	return checksum;
}

/**
 * @brief Calculate the checksum of the program y reading whole program and calculated.
 * 
 * @param prog Program need to read
 * @return uint8_t checksum value
 */
uint8_t Bootloader_CalCheckSum (bootProgram_t prog){
	uint32_t lengthProgram;
	uint32_t startDataAddress;
	uint8_t  nbrOfBlock;
	uint8_t  countBlock;
	uint16_t countData;
	uint16_t lengthEndBlock;
	uint8_t  checkSum = 0x00;
	uint8_t  tmpDataBuffer[FLASH_BLOCK_SIZE];

	if(prog == CurrentProg){
		lengthProgram = Flash_ReadWord(BOOTLOADER_CURRENT_LEN);
		startDataAddress = MAIN_PROG_ADDRESS;
	}
	else{
		lengthProgram = Flash_ReadWord(BOOTLOADER_PROGNEXT_LEN);
		startDataAddress = TEMP_PROG_ADDRESS;
	}

	nbrOfBlock = (lengthProgram + FLASH_BLOCK_SIZE - 1)/FLASH_BLOCK_SIZE;
	for (countBlock = 0; countBlock < nbrOfBlock; countBlock++){
		Flash_ReadBank(startDataAddress + FLASH_BLOCK_SIZE * countBlock, (uint32_t *)(tmpDataBuffer));
		if(countBlock < nbrOfBlock - 1){
			for (countData = 0; countData < FLASH_BLOCK_SIZE; countData++){
				checkSum += tmpDataBuffer[countData];
			}
		}
		else{
			lengthEndBlock = lengthProgram % FLASH_BLOCK_SIZE;
			for (countData = 0; countData < lengthEndBlock; countData++){
				checkSum += tmpDataBuffer[countData];
			}
		}
	}
	checkSum = 0xFF - checkSum;

	return checkSum;
}

/**
 * @brief Go to the current program of IC.
 * 
 */
void 	Bootloader_GotoProgram (uint32_t address){
	void (*funcGotoProg)(void) = (void*)(*((volatile uint32_t*) (address + 4U)));
	__set_MSP(*((volatile uint32_t*) address));
	funcGotoProg();
}

/**
 * @brief Copy program from temp to current
 * 
 * @return uint8_t copy done or error (0: OK, 1: error)
 */
uint8_t Bootloader_CopyTemp2Main (void){
	
	return 0;
}

