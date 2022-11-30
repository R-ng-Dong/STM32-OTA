/**
 * @file UART_OTA.h
 * @author your name (you@domain.com)
 * @brief OTA through UART program
 * @version 0.1
 * @date 2022-11-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __UART_OTA__
#define __UART_OTA__

#include "MessageManager.h"
#include "../Bootloader/MemoryInterface.h"

#define UART_OTA_DEBUG 1
#if UART_OTA_DEBUG
#define otaDebug(...) printf("[OTA]"); printf(__VA_ARGS__)
#else
	#define otaDebug(...)
#endif


#define OTA_HEADER_START    (0x01)
#define OTA_HEADER_STOP     (0x02)
#define OTA_HEADER_DATA     (0x03)
#define OTA_HEADER_LENGTH   (0x04)
#define OTA_HEADER_CRC      (0x05)
#define OTA_HEADER_VERSION  (0x06)
#define OTA_HEADER_REBOOT   (0x07)



typedef struct otaDataFrame{
    uint8_t 	Location[4];
	uint8_t 	Length[2];
	uint8_t		Message[MESSAGE_MAXLENGTH];
} otaDataFrame_t;



void UART_OTA_Init(void);
void UART_OTA_Process(void);
void UART_OTA_onLoop(void);


#endif /* B250211A_36DE_47DF_A59B_C70BDA79466C */
