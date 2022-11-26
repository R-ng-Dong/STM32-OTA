#include "UART.h"

static USART_InitTypeDef 	USART_InitStructure;
static GPIO_InitTypeDef 	GPIO_InitStructure;
static NVIC_InitTypeDef  	NVIC_InitStructure;

static void (*callbackIrqFnc)(char data);

struct __FILE
{
  int handle;
  /* Whatever you require here. If the only file you are using is */
  /* standard output using printf() for debugging, no file handling */
  /* is required. */
};
/* FILE is typedef’d in stdio.h. */
FILE __stdout;
int fputc(int ch, FILE *f)
{
  /* Your implementation of fputc(). */
	UARTDebug_SendChar(ch);
  return ch;
}
int ferror(FILE *f)
{
  /* Your implementation of ferror(). */
  return 0;
}

void UARTDebug_Init (uint32_t baudrate){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;  
    GPIO_Init(GPIOA, &GPIO_InitStructure);     
    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);
	
	USART_ClearFlag(USART2, USART_IT_RXNE);
	USART_ITConfig(USART2, USART_IT_RXNE ,ENABLE);// Cho phep ngat
	USART_Cmd(USART2, ENABLE);
}

void UARTDebug_SendChar (char data){
	USART_SendData(USART2, data);
    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
}

void UARTDebug_SendString (char *string){
	while(*string){
		UARTDebug_SendChar(*string);
		string++;
	}
}

void UARTDebug_AddCallBack (void (*func)(char data)){
	callbackIrqFnc = func;
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_EnableIRQ(USART2_IRQn);
}

void USART2_IRQHandler (void){
	char tmpData;
	if(USART_GetFlagStatus(USART2, USART_IT_RXNE) != RESET){
		tmpData = USART_ReceiveData(USART2);
		callbackIrqFnc(tmpData);
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
	}
}


