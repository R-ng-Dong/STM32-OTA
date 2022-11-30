#include "UART_Receive.h"


#define MAXLENGTHSTRING_QDB (1024)

static uint8_t uartDataBuffer[MAXLENGTHSTRING_QDB];
volatile ringbuffer_t guartRingBuffer;


void uartReceive_Init(void){
    ringBuffer_Init(&guartRingBuffer, uartDataBuffer, MAXLENGTHSTRING_QDB);
}


void uartReceive_PushData(char data){
    ringBuffer_Push(&guartRingBuffer, data);
}


