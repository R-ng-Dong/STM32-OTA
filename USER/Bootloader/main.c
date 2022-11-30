#include <stdio.h>
#include "../Common/include.h"
#include "../Peripheral/UART.h"
#include "../Application/UART_Receive.h"
#include "Bootloader.h"
#include "../Application/UART_OTA.h"

int main (void){
	uint16_t i;
	uint8_t dataTemp;
	
	
	SystemInit();
	SystemCoreClockUpdate();
	
	Fn_DELAY_Init(72);
	UARTDebug_Init(115200);
	uartReceive_Init();
	UARTDebug_AddCallBack((void *)(&uartReceive_PushData));
	printf("\n------------\nGo to proram!\n");
	
	Bootloader_Init();
	Bootloader_Processing();
	
	UART_OTA_Init();
	while(guartRingBuffer.count){
		ringBuffer_Pop(&guartRingBuffer, &dataTemp);
	}
	printf("Wait for Event\n\
			1 - Run\n\
			2 - OTA!\n");
	while(guartRingBuffer.count == 0);
	ringBuffer_Pop(&guartRingBuffer, &dataTemp);
	if(dataTemp == '1'){
		printf("Run to program!\n");
		Bootloader_RunProgram();
	}
	else{
		printf("Run to OTA!\n");
	}
	//Bootloader_GotoProgram(TEMP_PROG_ADDRESS);
	while(1){
		UART_OTA_Process();
		UART_OTA_onLoop();
		Fn_DELAY_ms(1);
	}
}



