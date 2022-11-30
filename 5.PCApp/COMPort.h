/**
 * @file COMPort.h
 * @author DuanLCe (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-11-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __COMPORT__
#define __COMPORT__

#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <stdint.h>


uint8_t comPort_Open(const char *comID);
void comPort_Send (const uint8_t *dataSend, uint32_t length);
void comPort_Close (void);


#endif /* CA361B9C_2479_4C44_8CA9_8B19B3FCB751 */
