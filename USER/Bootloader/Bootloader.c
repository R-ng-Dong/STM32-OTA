#include "Bootloader.h"

//static bootStages_t gBootloaderStatic = Init;

static uint32_t  tmpDataBuffer[FLASH_WORD_PER_BLOCK];

/**
 * @brief Init the Bootloader program.
 * 
 */
void Bootloader_Init (void){
	MemInterface_Init();
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
	
	currentVerion.dataFlash = MemInterface_getCurrentVersion();
	nextVersion.dataFlash = MemInterface_getTempVersion();
	
	
	bootloaderDebug("Current:%d.%d - Next:%d.%d\n", \
					currentVerion.version.major, currentVerion.version.minor, \
					nextVersion.version.major, nextVersion.version.minor);

	if(currentVerion.dataFlash != nextVersion.dataFlash){
		return 1;
	}

	return 0;

}


/**
 * @brief Calculate the checksum of the program y reading whole program and calculated.
 * 
 * @param prog Program need to read
 * @return uint8_t checksum value
 */
static uint8_t Bootloader_CalCheckSum (bootProgram_t prog){
	uint32_t lengthProgram;
	uint32_t startDataAddress;
	uint8_t  nbrOfBlock;
	uint8_t  countBlock;
	uint16_t countData;
	uint16_t lengthEndBlock;
	uint8_t  checkSum = 0x00;
	uint8_t  *pDataCheck;

	if(prog == CurrentProg){
		lengthProgram = Flash_ReadWord(BOOTLOADER_CURRENT_LEN);
		startDataAddress = MAIN_PROG_ADDRESS;
	}
	else{
		lengthProgram = Flash_ReadWord(BOOTLOADER_TEMP_LEN);
		startDataAddress = TEMP_PROG_ADDRESS;
	}
	
	bootloaderDebug("Length OTA file:%d\n", lengthProgram);

	assert_param((startDataAddress % FLASH_BLOCK_SIZE) == 0);

	nbrOfBlock = (lengthProgram + FLASH_BLOCK_SIZE - 1)/FLASH_BLOCK_SIZE;
	for (countBlock = 0; countBlock < nbrOfBlock; countBlock++){
		Flash_ReadBank(startDataAddress + FLASH_BLOCK_SIZE * countBlock, tmpDataBuffer);
		pDataCheck = (uint8_t *)(tmpDataBuffer);
		if(countBlock < nbrOfBlock - 1){
			for (countData = 0; countData < FLASH_BLOCK_SIZE; countData++){
				checkSum += pDataCheck[countData];
			}
		}
		else{
			lengthEndBlock = lengthProgram % FLASH_BLOCK_SIZE;
//			bootloaderDebug("Data Check:");
			for (countData = 0; countData < lengthEndBlock; countData++){
				checkSum += pDataCheck[countData];
//				if(countData % 16 == 0){
//					printf("\n");
//				}
//				printf("%2x ", pDataCheck[countData]);
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
	uint8_t saveCRC;
	uint8_t calCRC;
	uint32_t lengthFirmware;
	uint32_t versionFirmware;
	
	saveCRC = MemInterface_getTempCRC();
	calCRC = Bootloader_CalCheckSum(NextProg);
	
	bootloaderDebug("Before CRC:0x%2x-0x%2x\n", saveCRC, calCRC);
	
	if(saveCRC != calCRC){
		return 1;
	}
	
	lengthFirmware = MemInterface_getTempFirmLength();
	MemInterface_copyProgram(TEMP_PROG_ADDRESS, MAIN_PROG_ADDRESS, lengthFirmware);
	
	saveCRC = MemInterface_getTempCRC();
	calCRC = Bootloader_CalCheckSum(CurrentProg);
	
//	bootloaderDebug("After CRC:0x%2x-0x%2x\n", saveCRC, calCRC);
//	if(saveCRC != calCRC){
//		return 1;
//	}
	
	versionFirmware = MemInterface_getTempVersion();
	MemInterface_setCurrentVersion(versionFirmware);
	
	return 0;
}

/**
 * @brief Run the main program from the bootloader
 * 
 */
void 	Bootloader_RunProgram (void){
	Bootloader_GotoProgram(MAIN_PROG_ADDRESS);
}


/**
 * @brief Main process of OTA process.
 * 1. Check if have different version. If not, run to main program
 * 2. If different, copy program from temp to main program
 * 3. Replace main program information by temp program information
 * 
 */
void 	Bootloader_Processing (void){
	bootloaderDebug("Start OTA process!\n");
	
	if(Bootloader_CheckDiffVersion() != 0){
		bootloaderDebug("Diffirent version, need to be OTA!\n");
		
		if(Bootloader_CopyTemp2Main() != 0){
			bootloaderDebug("Copy fail\n");
		}
		else{
			bootloaderDebug("Copy done!\n");
		}
		//NVIC_SystemReset();
	}
	else{
		bootloaderDebug("Keep current version\n");
		//Bootloader_RunProgram();
	}
}

