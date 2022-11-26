#ifndef _UART_H_
#define _UART_H_

#include "../Common/include.h"

void UARTDebug_Init (uint32_t baudrate);
void UARTDebug_SendChar (char data);
void UARTDebug_SendString (char *string);



#endif /* _UART_H_ */
