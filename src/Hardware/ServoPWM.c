#include "stm32f10x.h"                  // Device header


/*
PB5 对应 重定义功能 TIM3_CH2

*/

#define SG90_PIN  GPIO_Pin_5


void TIM3_PWM_Init(uint16_t arr,uint16_t psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //Enable timer 3 clock
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE); //Enable GPIO peripheral and AFIO multiplexing function module clock
	
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3 partial remap TIM3_CH2->PB5    
 
   //Set this pin as a multiplexed output function and output the PWM pulse waveform of TIM3 CH2 GPIOB.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //TIM_CH2
	// 复用 开漏/推挽 输出， 引脚的控制器交给片上的外设，pwn才能输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //Multiplex push-pull output
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//Initialize GPIO
 
	/*
	SG90:  舵机的控制一般需要一个20ms左右的时基脉冲，该脉冲的高电平部分一般为0.5ms~2.5ms范围内的角度控制脉冲部分。
	首先是产生基本的PWM周期信号，本设计是产生20ms的周期信号；其次是脉宽的调整，即单片机模拟PWM信号的输出，并且调整占空比
	T = 20ms, Freq= 1/20ms = 50
	set  ARR+1 =20K,
	then PSC+1 =CN_CNT/(ARR+1)/ Freq =72M/20K/50 =72
	then Reso=1/ (ARR+1) = 0.01%, 
	if output = 0.5 ms 高电平，
	Duty = 0.5ms /20 ms= 2.5%
	CCR = (ARR+1) * 2.5% = 20K * 2.5% = 0.5K
	output = 0.5 ms -2.5 ms , CCR= 500- 2500
	
*/
	
   //Initialize TIM3
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = arr; //Set the value of the automatic reload register period that is loaded in the next update event
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //Set the prescaler value used as the divisor of the TIMx clock frequency 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //Set clock division: TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM up counting mode
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //Initialize the time base unit of TIMx according to the parameters specified in TIM_TimeBaseInitStruct
	
	//Initialize TIM3 Channel2 PWM mode 初始化 OC3 输出比较单元(通道)
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure); // 初始化 TIM_OCInitStruct
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //Select timer mode: TIM pulse width modulation mode 2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //Comparison output enable
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //Output polarity: TIM output comparison polarity is low
	
	TIM_OCInitStructure.TIM_Pulse = 0 ;  // CCR 寄存器值 
	
	// 不同的GPIO口对应的通道是不一样的 PB5 -> TIM3_CH2
	TIM_OC2Init(TIM3, &TIM_OCInitStructure); //Initialize the peripheral TIM3 OC2 according to the parameters specified by T
 
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable); //Enable the preload register of TIM3 on CCR2
 
	TIM_Cmd(TIM3, ENABLE); //Enable TIM3
	
 
}



