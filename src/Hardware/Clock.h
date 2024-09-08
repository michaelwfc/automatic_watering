#include "stm32f10x.h"                  // Device header
#include <timer.h>

#ifndef __CLOCK_H
#define __CLOCK_H

ClockDef init_clock(uint16_t hour, uint16_t minite,uint16_t second);

void show_clock(ClockDef clock_struct,uint8_t row, uint8_t off);
void show_clock_intevally(ClockDef clock_struct,uint8_t row, uint8_t off, uint8_t period);
	
ClockDef set_clock_format(ClockDef clock_struct);

void show_duration_intevally( uint16_t duration, uint8_t row, uint8_t off);

#endif
