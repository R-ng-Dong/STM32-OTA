#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "Delay.h"

GPIO_InitTypeDef GPIO_InitStructure;

int main (void){
	uint8_t i;
	SystemInit();
	SystemCoreClockUpdate();
	
	Fn_DELAY_Init(72);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOE, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	Fn_DELAY_ms(1000);
	
	while(1){
		for (i = 0; i < 8; i++){
			GPIO_ResetBits(GPIOE, GPIO_Pin_All);
			//GPIO_SetBits(GPIOE, GPIO_Pin_8 << i);
			GPIO_SetBits(GPIOE, GPIO_Pin_15 >> i);
			Fn_DELAY_ms(500);
		}
	}
}


