#include <stdio.h>
#include "../Common/include.h"
#include "../Peripheral/UART.h"
#include "../Application/UART_Receive.h"
#include "Bootloader.h"

#define ADDRESS_GO	0x8010000

int main (void){
	SystemInit();
	SystemCoreClockUpdate();
	
	Bootloader_Init();
	UARTDebug_Init(115200);
	uartReceive_Init();
	UARTDebug_AddCallBack((void *)(&uartReceive_PushData));
	printf("Go to proram!\n");
	//Bootloader_GotoProgram(ADDRESS_GO);
	while(1){
	}
}



