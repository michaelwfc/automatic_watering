#include "stm32f10x.h"                  // Device header

#ifndef __INCONTROL_H
#define __INCONTROL_H

void IRremote_Init(void);      // 红外遥控器初始化
uint8_t IRremote_Counttime(void);  // 红外遥控器计数时间

uint8_t get_ir_signal(void);

//定义全局变量
extern uint32_t IR_Receivecode;
extern uint8_t  IR_Receiveflag;

#endif

