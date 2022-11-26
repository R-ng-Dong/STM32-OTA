#include "UART_Receive.h"


#define MAXLENGTHSTRING_QDB (1024)

static char uartDataBuffer[MAXLENGTHSTRING_QDB];
static ringbuffer_t uartRingBuffer;


void uartReceive_Init(void){
    ringBuffer_Init(&uartRingBuffer, MAXLENGTHSTRING_QDB, sizeof(char));
}


void uartReceive_PushData(char data){
    ringBuffer_Push(&uartRingBuffer, data);
}


