#include "MemoryInterface.h"


static uint32_t tmpMemoryData[FLASH_WORD_PER_BLOCK];

void MemInterface_Init(void){
    Flash_InitData();
}

/* Information of Current firmware*/
uint32_t MemInterface_getCurrentVersion(void){
    uint32_t version;

    version = Flash_ReadWord(BOOTLOADER_CURRENT_VER);

    return version;
}

void MemInterface_setCurrentVersion(uint32_t version){
    Flash_WriteWord(BOOTLOADER_CURRENT_VER, version);
}


uint32_t MemInterface_getCurrentFirmLength(void){
    uint32_t length;

    length = Flash_ReadWord(BOOTLOADER_CURRENT_LEN);

    return length;
}

void MemInterface_setCurrentFirmLength(uint32_t length){
    Flash_WriteWord(BOOTLOADER_CURRENT_LEN, length);
}

uint8_t MemInterface_getCurrentCRC(void){
    uint8_t crc;

    crc = Flash_ReadWord(BOOTLOADER_CURRENT_CRC) & 0xFF;

    return crc;
}

void MemInterface_setCurrentCRC(uint8_t crc){
    uint32_t crcTemp = 0;

    crcTemp = crc;
    Flash_WriteWord(BOOTLOADER_CURRENT_CRC, crcTemp);
}



/* Information of Temp firmware*/
uint32_t MemInterface_getTempVersion(void){
    uint32_t version;

    version = Flash_ReadWord(BOOTLOADER_TEMP_VER);

    return version;
}

void MemInterface_setTempVersion(uint32_t version){
    Flash_WriteWord(BOOTLOADER_TEMP_VER, version);
}


uint32_t MemInterface_getTempFirmLength(void){
    uint32_t length;

    length = Flash_ReadWord(BOOTLOADER_TEMP_LEN);

    return length;
}

void MemInterface_setTempFirmLength(uint32_t length){
    Flash_WriteWord(BOOTLOADER_TEMP_LEN, length);
}

uint8_t MemInterface_getTempCRC(void){
    uint8_t crc;

    crc = Flash_ReadWord(BOOTLOADER_TEMP_CRC) & 0xFF;

    return crc;
}

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

void MemInterface_copyProgram(uint32_t source, uint32_t destination, uint32_t length){

}


