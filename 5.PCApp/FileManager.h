/**
 * @file FileManager.h
 * @author DuanLC (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-11-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __FILE_MANAGER__
#define __FILE_MANAGER__

#include <stdio.h>
#include <stdint.h>
#include <windows.h>

#define MAX_FRAME_SIZE  (256)


void fileManager_OpenFile(const char *fileOpen);
uint32_t fileManager_getSize(void);
void fileManager_gotoStart(void);
uint32_t fileManager_getData(uint8_t *data, uint16_t maxLength);
void fileManager_CloseFile(void);


#endif /* __FILE_MANAGER__ */
