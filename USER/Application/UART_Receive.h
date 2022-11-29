#ifndef __UART_RECEIVE__
#define __UART_RECEIVE__

#include "../Common/include.h"
#include "RingBuffer.h"

extern ringbuffer_t guartRingBuffer;


void uartReceive_Init(void);
void uartReceive_PushData(char data);


#endif /* __UART_RECEIVE__ */


