#include "UART_Receive.h"


#define MAXLENGTHSTRING_QDB (1024)

static uint8_t uartDataBuffer[MAXLENGTHSTRING_QDB];
volatile ringbuffer_t guartRingBuffer;


/**
 * @brief Initialize the RingBuffer that contains data.
 * 
 */
void uartReceive_Init(void){
    ringBuffer_Init(&guartRingBuffer, uartDataBuffer, MAXLENGTHSTRING_QDB);
}


/**
 * @brief The callback function for push data in RingBuffer.
 * 
 * @param data bytes data to be pushed
 */
void uartReceive_PushData(char data){
    ringBuffer_Push(&guartRingBuffer, data);
}


