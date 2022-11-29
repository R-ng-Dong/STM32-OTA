/**
 * @file MessageManager.h
 * @author Duan Luong (duanlc@ptit.edu.vn)
 * @brief 
 * @version 0.1
 * @date 2022-11-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __MESSAGE_MANAGER_
#define __MESSAGE_MANAGER_

#include "../Common/include.h"
#include "RingBuffer.h"
#include "UART_Receive.h"

#define TEMPBUFF_LENGTH		(261)
#define MESSAGE_MAXLENGTH	(256)
#define MESSAGE_HEADLENGTH	(5)
#define MESSAGE_OFFSET      (4)

typedef struct IncomingData{
	uint8_t 	Start[2];
	uint8_t 	Length[2];
	uint8_t		Header;
	uint8_t		Message[MESSAGE_MAXLENGTH];
} messageFrame_t;


void MessageProcess_Init(void);
void MessageProcess_Process(void);
bool MessageProcess_hasMessage(void);
uint16_t MessageProcess_copyMessage (uint8_t *Message, uint8_t *header);


#endif /* __MESSAGE_MANAGER_ */
