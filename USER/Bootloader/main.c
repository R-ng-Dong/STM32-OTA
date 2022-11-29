#include <stdio.h>
#include "../Common/include.h"
#include "../Peripheral/UART.h"
#include "../Application/UART_Receive.h"
#include "Bootloader.h"

int main (void){
	SystemInit();
	SystemCoreClockUpdate();
	
	Fn_DELAY_Init(72);
	UARTDebug_Init(115200);
	uartReceive_Init();
	UARTDebug_AddCallBack((void *)(&uartReceive_PushData));
	
	Bootloader_Init();
	Bootloader_Processing();
	/* Test Flash */
	/*
	printf("\n------------\nGo to proram!\n");
	MemInterface_writeProgram(ADDRESS_GO + FLASH_BLOCK_SIZE - 12, tmpData, 24);
	MemInterface_copyProgram(MAIN_PROG_ADDRESS, TEMP_PROG_ADDRESS, 432);
	printf("Done!\n");
	*/
	while(1){
	}
}



