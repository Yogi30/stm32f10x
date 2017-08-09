#include <stdio.h>
#include <stdlib.h>
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"


void Delay_ms(unsigned int nCount);		
GPIO_InitTypeDef GPIO_InitStructure;	

int main(void){	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	
	
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_Init(GPIOC, &GPIO_InitStructure);	
while(1){
GPIO_ResetBits(GPIOC, GPIO_Pin_13); 
Delay_ms(50);	
GPIO_SetBits(GPIOC, GPIO_Pin_13);		
Delay_ms(100);	
}
	
}
void Delay_ms(unsigned int nCount){ 
unsigned int i, j;
for(i = 0; i < nCount; i++)
   {  
	  for(j = 0; j < 0x2AFF; j++){;} 
   }
} 
