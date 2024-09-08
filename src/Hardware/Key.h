#include "stm32f10x.h" 
#ifndef __KEY_H
#define __KEY_H


void Keys_Init(void);
uint8_t MODE_Key_GetNum(uint8_t KeyNum);
uint8_t PERIOD_Key_GetNum(uint8_t KeyNum);
uint8_t INCREASE_Key_GetNum(uint8_t KeyNum);
uint8_t DECREASE_Key_GetNum(uint8_t KeyNum);


void Key_interupt_init(void);
uint8_t get_mode(void);
uint8_t get_period(void);
uint8_t get_value(void);


#endif
