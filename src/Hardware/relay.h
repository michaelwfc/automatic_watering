#include "stm32f10x.h" 
#ifndef __RELAY_H
#define __RELAY_H


void RELAY_CONTROL_Init(void);
void set_relay_control_high_volt(void);
void set_relay_control_low_volt(void);
#endif