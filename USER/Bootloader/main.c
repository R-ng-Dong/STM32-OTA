#include <stdio.h>
#include "../Common/include.h"
#include "../Peripheral/UART.h"
#include "Bootloader.h"

#define ADDRESS_GO	0x8010000

static volatile uint8_t flagIRQ;

void 	GotoProgram (uint32_t address){
	void (*funcGotoProg)(void) = (void*)(*((volatile uint32_t*) (address + 4U)));
	__set_MSP(*((volatile uint32_t*) address));
	funcGotoProg();
}

void addData (char data){
	flagIRQ = 1;
}

int main (void){
	uint32_t tmpData[256];
	
	SystemInit();
	SystemCoreClockUpdate();
	
	Bootloader_Init();
	UARTDebug_Init(115200);
	UARTDebug_AddCallBack((void *)(&addData));
	printf("Go to proram!\n");
	//GotoProgram(ADDRESS_GO);
	while(1){
		if(flagIRQ){
			flagIRQ = 0;
			printf("Run OK!\n");
		}
	}
}



