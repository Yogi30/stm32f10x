#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"


void Init_Serial(void);
void Init_GPIO(void);

int main(){
	SystemInit();
	SystemCoreClockUpdate();
	Init_GPIO();
	Init_Serial();
	while(1);
}

void Init_GPIO(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitTypeDef GPIO_Struc;
	
	GPIO_Struc.GPIO_Pin=GPIO_Pin_9;
	GPIO_Struc.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Struc.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&GPIO_Struc);
	
	GPIO_Struc.GPIO_Pin=GPIO_Pin_10;
	GPIO_Struc.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_Struc);
	
	GPIO_Struc.GPIO_Pin=GPIO_Pin_13;
	GPIO_Struc.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Struc.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_Struc);
}

void Init_Serial(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	USART_Cmd(USART1,ENABLE);
	
	USART_InitTypeDef USART_Struc;
	USART_Struc.USART_BaudRate=9600;
	USART_Struc.USART_WordLength=USART_WordLength_8b;
	USART_Struc.USART_Parity=USART_Parity_No;
	USART_Struc.USART_StopBits=USART_StopBits_1;
	USART_Struc.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;
	USART_Struc.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	
	USART_Init(USART1,&USART_Struc);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	NVIC_EnableIRQ(USART1_IRQn);
}

void USART1_IRQHandler(void){
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET){
		if((char)USART_ReceiveData(USART1)=='y'){
			GPIOC->ODR^=(1<<13);
			USART_SendData(USART1,'y');
			while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
	}
}
}
