#include "stm32f10x.h"
#define ADC1_DR_Address    ((uint32_t)0x4001244C)
volatile u16 adc_res[2]={0,0};
void init_usart(void);
void init_gpio(void);
void init_adc(void);
void init_dma(void);
//int status=0;
int main(){
	init_gpio();
	init_usart();
	init_adc();
	init_dma();
	DMA_Cmd(DMA1_Channel1,ENABLE);
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	while(1){
	}
}


void DMA1_Channel1_IRQHandler(void){
	if(DMA_GetITStatus(DMA1_IT_TC1)){
		DMA_ClearITPendingBit(DMA1_IT_GL1);
		USART_SendData(USART1,adc_res[0]);
		USART_SendData(USART1,adc_res[1]);
	}
}


void init_gpio(void){
	//initialize adc and uart1 pins
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	GPIO_InitTypeDef GPIO_Struc;
	
	GPIO_Struc.GPIO_Pin=GPIO_Pin_9;
	GPIO_Struc.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Struc.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&GPIO_Struc);
	
	GPIO_Struc.GPIO_Pin=GPIO_Pin_10;
	GPIO_Struc.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_Struc);
	
	GPIO_Struc.GPIO_Pin=GPIO_Pin_0;
	GPIO_Struc.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_Init(GPIOA,&GPIO_Struc);
	
	GPIO_Struc.GPIO_Pin=GPIO_Pin_1;
	GPIO_Struc.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_Init(GPIOA,&GPIO_Struc);
	
}

void init_usart(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	USART_Cmd(USART1,ENABLE);
	
	USART_InitTypeDef USART_Struc;
	
	USART_Struc.USART_BaudRate=9600;
	USART_Struc.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_Struc.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_Struc.USART_Parity=USART_Parity_No;
	USART_Struc.USART_StopBits=USART_StopBits_1;
	USART_Struc.USART_WordLength=USART_WordLength_8b;
	
	USART_Init(USART1,&USART_Struc);
	USART_SendData(USART1,adc_res[0]);
	USART_SendData(USART1,adc_res[1]);
}


void init_adc(void){
	ADC_InitTypeDef ADC_Struc;
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	ADC_Struc.ADC_Mode=ADC_Mode_Independent;
	ADC_Struc.ADC_DataAlign=ADC_DataAlign_Right;
	ADC_Struc.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	ADC_Struc.ADC_ContinuousConvMode=ENABLE;
	ADC_Struc.ADC_ScanConvMode=ENABLE;
	ADC_Struc.ADC_NbrOfChannel=2;
	
	ADC_Init(ADC1,&ADC_Struc);
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_41Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,2,ADC_SampleTime_41Cycles5);
	
	ADC_Cmd(ADC1,ENABLE);
	
	ADC_DMACmd(ADC1,ENABLE);
	
	ADC_ResetCalibration(ADC1);
	
	ADC_GetResetCalibrationStatus(ADC1);
	
	ADC_StartCalibration(ADC1);
	
	ADC_GetCalibrationStatus(ADC1);
}

void init_dma(void){
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	DMA_InitTypeDef DMA_Struc;
	
	DMA_DeInit(DMA1_Channel1);
	
	DMA_Struc.DMA_M2M=DISABLE;
	DMA_Struc.DMA_Mode=DMA_Mode_Circular;
	DMA_Struc.DMA_Priority=DMA_Priority_High;
	DMA_Struc.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;
	DMA_Struc.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;
	DMA_Struc.DMA_MemoryInc=DMA_MemoryInc_Enable;
	DMA_Struc.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	DMA_Struc.DMA_DIR=DMA_DIR_PeripheralSRC;
	DMA_Struc.DMA_BufferSize=2;
	DMA_Struc.DMA_PeripheralBaseAddr=(u32)ADC1_DR_Address;
	DMA_Struc.DMA_MemoryBaseAddr=(u32)&adc_res;
	
	DMA_Init(DMA1_Channel1,&DMA_Struc);
	
	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);
	DMA_Cmd(DMA1_Channel1,ENABLE);
	
	NVIC_InitTypeDef NVIC_Struc;
	
	NVIC_Struc.NVIC_IRQChannel=DMA1_Channel1_IRQn;
	NVIC_Struc.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_Struc.NVIC_IRQChannelSubPriority=0;
	NVIC_Struc.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_Struc);
}


