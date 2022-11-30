#include "MemoryInterface.h"


static uint32_t tmpMemoryData[FLASH_WORD_PER_BLOCK];
static uint32_t tmpBackupData[FLASH_WORD_PER_BLOCK];

/**
 * @brief Initialize memory interface
 * 
 */
void MemInterface_Init(void){
    Flash_InitData();
}

/**
 * @brief Calculate the CRC of a frame data
 * 
 * @param data buffer contains data
 * @param len length of the buffer
 * @return uint8_t CRC value
 */
uint8_t MemInterface_calculateCRC (uint8_t *data, uint16_t len){
    uint8_t crcCal = 0;
    uint16_t i;


    for (i = 0; i < len; i++){
        crcCal = crcCal + data[i];
    }

    crcCal = 0xFF - crcCal;

    return crcCal;
}

/* Information of Current firmware*/
/**
 * @brief Get the version of the current firmware
 * 
 * @return uint32_t version as uint32_t (Can convert to Major and Minor version)
 */
uint32_t MemInterface_getCurrentVersion(void){
    uint32_t version;

    version = Flash_ReadWord(BOOTLOADER_CURRENT_VER);

    return version;
}

/**
 * @brief set the version for the current firmware
 * 
 * @param version version of the current firmware (Can convert from Major and Minor version)
 */
void MemInterface_setCurrentVersion(uint32_t version){
    Flash_WriteWord(BOOTLOADER_CURRENT_VER, version);
}

/**
 * @brief get the length (size) of the current firmware
 * 
 * @return uint32_t size of the firmware (word)
 */
uint32_t MemInterface_getCurrentFirmLength(void){
    uint32_t length;

    length = Flash_ReadWord(BOOTLOADER_CURRENT_LEN);

    return length;
}

/**
 * @brief set the length (size) for the current firmware
 * 
 * @param length size of the firmware (word)
 */
void MemInterface_setCurrentFirmLength(uint32_t length){
    Flash_WriteWord(BOOTLOADER_CURRENT_LEN, length);
}


/**
 * @brief get the CRC of the current firmware
 * 
 * @return uint8_t CRC of the current firmware (Save in Flash)
 */
uint8_t MemInterface_getCurrentCRC(void){
    uint8_t crc;

    crc = Flash_ReadWord(BOOTLOADER_CURRENT_CRC) & 0xFF;

    return crc;
}

/**
 * @brief set the CRC of the current firmware (Save in Flash)
 * 
 * @param crc Check Sum
 */
void MemInterface_setCurrentCRC(uint8_t crc){
    uint32_t crcTemp = 0;

    crcTemp = crc;
    Flash_WriteWord(BOOTLOADER_CURRENT_CRC, crcTemp);
}



/* Information of Temp firmware*/
/**
 * @brief Get the version of temp firmware
 * 
 * @return uint32_t version of the temp firmware (Can convert from Major and Minor version)
 */
uint32_t MemInterface_getTempVersion(void){
    uint32_t version;

    version = Flash_ReadWord(BOOTLOADER_TEMP_VER);

    return version;
}

/**
 * @brief set the version of temp firmware
 * 
 * @param version version of the temp firmware (In Flash)
 */
void MemInterface_setTempVersion(uint32_t version){
    Flash_WriteWord(BOOTLOADER_TEMP_VER, version);
}


/**
 * @brief get the length (size) of the temp firmware
 * 
 * @return uint32_t size of the temp firmware
 */
uint32_t MemInterface_getTempFirmLength(void){
    uint32_t length;

    length = Flash_ReadWord(BOOTLOADER_TEMP_LEN);

    return length;
}


/**
 * @brief set the length (size) of the temp firmware
 * 
 * @param length size of the temp firmware
 */
void MemInterface_setTempFirmLength(uint32_t length){
    Flash_WriteWord(BOOTLOADER_TEMP_LEN, length);
}

/**
 * @brief get the CRC for the temp firmware
 * 
 * @return uint8_t CRC in Flash
 */
uint8_t MemInterface_getTempCRC(void){
    uint8_t crc;

    crc = Flash_ReadWord(BOOTLOADER_TEMP_CRC) & 0xFF;

    return crc;
}

/**
 * @brief set the CRC for the temp firmware (In Flash)
 * 
 * @param crc CRC of firmware
 */
void MemInterface_setTempCRC(uint8_t crc){
    uint32_t crcTemp = 0;

    crcTemp = crc;
    Flash_WriteWord(BOOTLOADER_TEMP_CRC, crcTemp);
}

/* Copy program from source to destination*/
typedef struct{
    uint32_t startAddress;
    uint32_t locationFirstBlock;
    uint32_t nbrBlocks;
    uint32_t nbrWords;
} blockData_t;


/**
 * @brief calculate Number of Block need to be access.
 * 
 * @param startAddress First address of data needed to be access
 * @param length Bumber of word be accessed
 * @return blockData_t Information for access to process (Include: Start Address of first block, Location of data in Fist Block,
 * Number of blocks need to be access, Number of words need to be access)
 */
static blockData_t calculatedNbrBlocks(uint32_t startAddress, uint32_t  length){
    blockData_t tmpData;
    uint32_t    endAddress = startAddress + length*4;
    uint32_t    endBlock = ((endAddress + FLASH_BLOCK_SIZE - 1)/FLASH_BLOCK_SIZE) * FLASH_BLOCK_SIZE;

    if((startAddress % 4) != 0){
        return;
    }
    tmpData.startAddress = ((startAddress + 1)/FLASH_BLOCK_SIZE) * FLASH_BLOCK_SIZE;
    tmpData.locationFirstBlock = (startAddress - tmpData.startAddress)/4;
    tmpData.nbrBlocks = (endBlock - tmpData.startAddress)/FLASH_BLOCK_SIZE;
    tmpData.nbrWords = length;
	
	memoryDebug("Start Address:0x%x\n",tmpData.startAddress);
	memoryDebug("End Address:0x%x- 0x%x\n",endAddress, endBlock);

    return tmpData;
}

/**
 * @brief Write some words to the Flash memory
 * 
 * @param address start address are written
 * @param data buffer data be written
 * @param length number of words to write
 */
void MemInterface_writeProgram(uint32_t address, uint32_t *data, uint32_t length){
    blockData_t tmpData;
    uint32_t countBlock;
    uint32_t countWords;
    uint32_t nbrWordsPerBlock;
    uint32_t startLocationOfPage;
    uint32_t nbrByteHasWritten;
    uint32_t exitData = length;
	uint16_t countTimes = 0;
	uint32_t readAddress;

    nbrWordsPerBlock = FLASH_BLOCK_SIZE/4;

    tmpData = calculatedNbrBlocks(address, length);
    memoryDebug("[Input] Address:0x%x - Length:%d\n", address, length);
    memoryDebug("[Output] StartAddress:0x%x - Nbr Blocks:%d - Nbr Words:%d - Location First:%d\n", \
            tmpData.startAddress, tmpData.nbrBlocks, tmpData.nbrWords, tmpData.locationFirstBlock);
    startLocationOfPage = tmpData.locationFirstBlock;
	readAddress = tmpData.startAddress;

    for (countBlock = 0; countBlock < tmpData.nbrBlocks - 1; countBlock++){
        Flash_ReadBank(readAddress, tmpMemoryData);
        for (countWords = startLocationOfPage; countWords < nbrWordsPerBlock; countWords++){
            tmpMemoryData[countWords] = *data;
            data++;
        }
		Flash_WriteBank(readAddress, tmpMemoryData);
        nbrByteHasWritten = nbrWordsPerBlock - startLocationOfPage;
		countTimes++;
		memoryDebug("Time:%d - Address:0x%x - Write done:%d bytes\n", countTimes, readAddress, nbrByteHasWritten);
		
        startLocationOfPage = 0;
		readAddress += FLASH_BLOCK_SIZE;
		exitData = exitData - nbrByteHasWritten;
    }
	
	/* Write lasted block */
	Flash_ReadBank(readAddress, tmpMemoryData);
	if(tmpData.nbrBlocks > 1){
		for (countWords = 0; countWords < exitData; countWords++){
			tmpMemoryData[countWords] = *data;
			data++;
		}
	}
	else{
		for (countWords = startLocationOfPage; countWords < tmpData.nbrWords + startLocationOfPage; countWords++){
            tmpMemoryData[countWords] = *data;
            data++;
        }
	}
	Flash_WriteBank(readAddress, tmpMemoryData);
	countTimes++;
	memoryDebug("Time:%d - Address:0x%x - Write done:%d bytes\n", countTimes, readAddress, exitData);
	memoryDebug("Written done %d times\n", countTimes);
}

/**
 * @brief Copy whole from from a block to another memory block
 * 
 * @param source source address
 * @param destination destination address
 * @param length number of words to copy
 */
void MemInterface_copyProgram(uint32_t source, uint32_t destination, uint32_t length){
	uint32_t nbrBlock;
	uint8_t	 count;
	uint32_t exitData = length;
	
	if( ((source % FLASH_BLOCK_SIZE) != 0) || \
		((destination % FLASH_BLOCK_SIZE) != 0)){
		memoryDebug("Wrong address!\n");
		return;
	}
	
	nbrBlock = (length + FLASH_BLOCK_SIZE - 1)/FLASH_BLOCK_SIZE;
	for(count = 0; count < nbrBlock; count++){
		if(exitData >= FLASH_BLOCK_SIZE){
			memoryDebug("Case 01\n");
			Flash_ReadBank(source, tmpBackupData);
			Flash_WriteBank(destination, tmpBackupData);
			exitData -= FLASH_BLOCK_SIZE;
			source += FLASH_BLOCK_SIZE;
			destination += FLASH_BLOCK_SIZE;
		}
		else{
			memoryDebug("Case 02\n");
			Flash_ReadBank(source, tmpBackupData);
			MemInterface_writeProgram(destination, tmpBackupData, exitData);
			source += exitData * 4;
			destination += exitData * 4;
			exitData = 0;
		}
		
	}
	memoryDebug("Copy done!\n");
}


