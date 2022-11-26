#include <stdio.h>
#include "../Common/include.h"
#include "../Peripheral/UART.h"
#include "Bootloader.h"

#define ADDRESS_GO	0x8010000

uint32_t dataRead = 0;

void 	GotoProgram (uint32_t address){
	void (*funcGotoProg)(void) = (void*)(*((volatile uint32_t*) (address + 4U)));
	__set_MSP(*((volatile uint32_t*) address));
	funcGotoProg();
}

int main (void){
	uint32_t tmpData[256];
	
	SystemInit();
	SystemCoreClockUpdate();
	
	Bootloader_Init();
	UARTDebug_Init(115200);
	UARTDebug_SendString("Go to proram!\n");
	printf("Check this function\n");
	printf("Data:0x%x\n",Flash_ReadWord(ADDRESS_GO + 4));
	tmpData[0] = 0x87654321;
	//Flash_ReadBank(0x8000000, tmpData);
	Flash_WriteWord(ADDRESS_GO + 4, 0x12345678);
	//GotoProgram(ADDRESS_GO);
	while(1){
		
	}
}



