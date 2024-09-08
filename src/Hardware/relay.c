#include "stm32f10x.h"                  // Device header

uint16_t RELAY_CONTROL_VOLT = GPIO_Pin_12; //RCC_APB2Periph_GPIOB

void RELAY_CONTROL_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    // GPIO_Mode_Out_PP  推挽输出
	GPIO_InitStructure.GPIO_Pin = RELAY_CONTROL_VOLT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

}


void set_relay_control_high_volt(void)
{
	GPIO_SetBits(GPIOB, RELAY_CONTROL_VOLT);   // gpio_pin 口 高电平
}

void set_relay_control_low_volt(void)
{
	GPIO_ResetBits(GPIOB, RELAY_CONTROL_VOLT);   // gpio_pin 口 高电平
}
