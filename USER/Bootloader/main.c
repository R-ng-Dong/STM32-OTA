#include <stdio.h>
#include "../Common/include.h"
#include "../Peripheral/UART.h"
#include "../Application/UART_Receive.h"
#include "Bootloader.h"

#define ADDRESS_GO	0x8010000
uint32_t tmpData[1024] = {0};

int main (void){
	uint16_t i;
	
	SystemInit();
	SystemCoreClockUpdate();
	Fn_DELAY_Init(72);
	
	for (i = 0; i < 1024; i++){
		tmpData[i] = i | 0xF1000000;
	}
	
	Bootloader_Init();
	UARTDebug_Init(115200);
	uartReceive_Init();
	UARTDebug_AddCallBack((void *)(&uartReceive_PushData));
	printf("\n------------\nGo to proram!\n");
	Bootloader_GotoProgram(ADDRESS_GO);
	//MemInterface_writeProgram(ADDRESS_GO + FLASH_BLOCK_SIZE - 12, tmpData, 24);
	printf("Done!\n");
	while(1){
	}
}



