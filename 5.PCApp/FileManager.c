
#include "FileManager.h"

static FILE *ptrFile = NULL;
static uint32_t locationPointData;
static uint32_t lengthData;

void fileManager_OpenFile(const char *fileOpen){
    ptrFile = fopen(fileOpen, "rb");

    if(ptrFile == NULL){
        printf("Invalid file!\n");
        return;
    }

    printf("Open successfully!\n");

    lengthData = fileManager_getSize();
    locationPointData = 0;
}

uint32_t fileManager_getSize(void){
    uint32_t fileSize = 0;
    if(ptrFile == NULL){
        printf("Invalid file!\n");
        return 0;
    }

    fseek(ptrFile, 0, SEEK_SET);
    fseek(ptrFile, 0, SEEK_END);
    fileSize = ftell(ptrFile);
    fseek(ptrFile, 0, SEEK_SET);
    //printf("Size file:%d\n", fileSize);

    return fileSize;
}

void fileManager_gotoStart(void){
    fseek(ptrFile, 0, SEEK_SET);
}

uint32_t fileManager_getData(uint8_t *data, uint16_t maxLength){
    uint32_t lengthRead = fread(data, 1, maxLength, ptrFile);
    locationPointData += lengthRead;
    fseek(ptrFile, locationPointData, SEEK_SET);

    return lengthRead;
}

void fileManager_CloseFile(void){
    fclose(ptrFile);

    ptrFile = NULL;
}