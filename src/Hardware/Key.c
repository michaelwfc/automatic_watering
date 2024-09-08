#include "stm32f10x.h"                  // Device header
#include "Delay.h"

uint16_t MODE_KEY = GPIO_Pin_10; //RCC_APB2Periph_GPIOB
uint16_t TIME_KEY = GPIO_Pin_6; // RCC_APB2Periph_GPIOA
uint16_t INCREASE_KEY = GPIO_Pin_2; // RCC_APB2Periph_GPIOA
uint16_t DECREASE_KEY = GPIO_Pin_14; // RCC_APB2Periph_GPIOC

// Use extern to declare the global variable defined in key.c
extern uint16_t clock_value;  

// Define the global variable
static uint16_t mode=0,period=0;



void Keys_GPIO_Init(void)
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = MODE_KEY;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;    // GPIO_Mode_IPU  上拉输入 高电平的输入方式
	GPIO_InitStructure.GPIO_Pin = TIME_KEY|INCREASE_KEY;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = DECREASE_KEY;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
}



void Key_interupt_init(void)
{	
	// 1. initial GPIO PIN 口
	Keys_GPIO_Init();
	
	// 2. 开启 RCC_APB2Periph_AFIO 时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	//  AFIO 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource10); // for mode key
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource6); // form period key|increase key
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource2); 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource14);
	
	// 4. initial EXTI 边沿检测及控制 外设
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = EXTI_Line10|EXTI_Line6|EXTI_Line2|EXTI_Line14;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; // 下降沿触发
	EXTI_InitStruct.EXTI_LineCmd =ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	// 5. 配置 NVIC 外设 EXTI0 EXTI1
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel= EXTI15_10_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd= ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority= 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel= EXTI9_5_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd= ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority= 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel= EXTI2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd= ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority= 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStruct);
	
//	NVIC_InitStruct.NVIC_IRQChannel= EXTI15_10_IRQn;
//	NVIC_InitStruct.NVIC_IRQChannelCmd= ENABLE;
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority= 1;
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
//	NVIC_Init(&NVIC_InitStruct);
	
}



void  EXTI15_10_IRQHandler(void)
{
	//Checks whether the specified EXTI line is asserted or not
	if(EXTI_GetITStatus(EXTI_Line10) == SET)
	{
		
		if (GPIO_ReadInputDataBit(GPIOB, MODE_KEY) == 0)
		{
			Delay_ms(20);
			while (GPIO_ReadInputDataBit(GPIOB, MODE_KEY) == 0);
			Delay_ms(20);
			mode ++;
		}
		//mode= 0: do noththing, 1: set clock, 2: set watering time,  3: set duration 
		if(mode>3)mode=0;
		EXTI_ClearITPendingBit(EXTI_Line10); // 清除中断状态
	}
	
	//Checks whether the specified EXTI line is asserted or not
	if(EXTI_GetITStatus(EXTI_Line14) == SET)
	{
		if (GPIO_ReadInputDataBit(GPIOC, DECREASE_KEY) == 0)
		{
			Delay_ms(20);
			while (GPIO_ReadInputDataBit(GPIOC, DECREASE_KEY) == 0);
			Delay_ms(20);
			clock_value --;
		}
		if(clock_value<0)clock_value=0;
	
		EXTI_ClearITPendingBit(EXTI_Line14); // 清除中断状态
	}
	
	
}

uint16_t get_mode(void)
{
	return mode;
}


void  EXTI9_5_IRQHandler(void)
{
	//Checks whether the specified EXTI line is asserted or not
	if(EXTI_GetITStatus(EXTI_Line6) == SET)
	{
		if (GPIO_ReadInputDataBit(GPIOA, TIME_KEY) == 0)
		{
			Delay_ms(20);
			while (GPIO_ReadInputDataBit(GPIOA, TIME_KEY) == 0);
			Delay_ms(20);
			period ++;
		}
		//period = 0: HOUR, 1: minute, 2: SECOND,  3: set duration 
		if(period>2)period=0;

		EXTI_ClearITPendingBit(EXTI_Line6); // 清除中断状态
	}
	

	
}

uint16_t get_period(void)
{
	return period;
} 



void  EXTI2_IRQHandler(void)
{
	//Checks whether the specified EXTI line is asserted or not
	if(EXTI_GetITStatus(EXTI_Line2) == SET)
	{
		if (GPIO_ReadInputDataBit(GPIOA, INCREASE_KEY) == 0)
		{
			Delay_ms(20);
			while (GPIO_ReadInputDataBit(GPIOA, INCREASE_KEY) == 0);
			Delay_ms(20);
			clock_value ++;
		}
	
		EXTI_ClearITPendingBit(EXTI_Line2); // 清除中断状态
	}
}


uint16_t get_value(void)
{
		return clock_value;
}

uint8_t MODE_Key_GetNum(uint8_t KeyNum)
{
	if (GPIO_ReadInputDataBit(GPIOB, MODE_KEY) == 0)
	{
		Delay_ms(20);
		while (GPIO_ReadInputDataBit(GPIOB, MODE_KEY) == 0);
		Delay_ms(20);
		KeyNum ++;
	}
	//0: do noththing, 1: set clock, 2: set watering time,  3: set duration 
	if(KeyNum>3)KeyNum=0;
	
	return KeyNum;
}



uint8_t PERIOD_Key_GetNum(uint8_t KeyNum)
{
	if (GPIO_ReadInputDataBit(GPIOA, TIME_KEY) == 0)
	{
		Delay_ms(20);
		while (GPIO_ReadInputDataBit(GPIOA, TIME_KEY) == 0);
		Delay_ms(20);
		KeyNum ++;
	}
	//0: HOUR, 1: minute, 2: SECOND,  3: set duration 
	if(KeyNum>2)KeyNum=0;
	
	return KeyNum;
}


uint8_t INCREASE_Key_GetNum(uint8_t KeyNum)
{
	if (GPIO_ReadInputDataBit(GPIOA, INCREASE_KEY) == 0)
	{
		Delay_ms(20);
		while (GPIO_ReadInputDataBit(GPIOA, INCREASE_KEY) == 0);
		Delay_ms(20);
		KeyNum ++;
	}	
	return KeyNum;
}


uint8_t DECREASE_Key_GetNum(uint8_t KeyNum)
{
	if (GPIO_ReadInputDataBit(GPIOC, DECREASE_KEY) == 0)
	{
		Delay_ms(20);
		while (GPIO_ReadInputDataBit(GPIOC, DECREASE_KEY) == 0);
		Delay_ms(20);
		KeyNum --;
	}
	//0: HOUR, 1: minute, 2: SECOND,  3: set duration 
	if(KeyNum<0)KeyNum=0;
	
	return KeyNum;
}



