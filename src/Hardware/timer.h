#include "stm32f10x.h"                  // Device header

#ifndef __TIMER_H
#define __TIMER_H



// timer3_delay_us  used in interupt servie routine
void timer1_init(void);
void timer1_delay_us(uint32_t xus);
void timer1_delay_ms(uint32_t xms);
void timer1_delay_s(uint32_t xs);

// timer2 as clock
void timer2_clock_init(void);

// timer3 as count for duration
void timer3_count_init(void);
void start_relay_duration(uint16_t duration);
uint16_t is_relay_runing(void);

//struct ClockDef;
typedef struct
{
	uint16_t second;
	uint16_t minute;
	uint16_t hour;
} ClockDef;


ClockDef init_clock(uint16_t hour, uint16_t minute,uint16_t second);
ClockDef set_clock_format(ClockDef clock_struct);
ClockDef get_clock(void);

void show_clock(ClockDef clock_struct,uint8_t row, uint8_t off);
void show_clock_intevally(ClockDef clock_struct,uint8_t row, uint8_t off, uint8_t period);
void show_duration_intevally( uint16_t duration, uint8_t row, uint8_t off);

int compareClocks(ClockDef *c1, ClockDef *c2);
	
#endif
