#include "stm32f10x.h"


/**
  * @brief  微秒级延时
  * @param  xus 延时时长，范围：0~233015
  * @retval 无
  The Delay_us function uses the SysTick timer to create a delay in microseconds. 
  By configuring the reload value based on the system clock (72 MHz), 
  it accurately waits for the specified number of microseconds before continuing execution. 
  This method provides a simple and effective way to implement microsecond delays using 
  the SysTick timer in an STM32F10x microcontroller running at 72 MHz.
  
  */
void Delay_us(uint32_t xus)
{	
 
	/*Setting the Reload Value:
	SysTick->LOAD is the reload value register for the SysTick timer.
	Since the system clock (HCLK) is 72 MHz, 1 microsecond corresponds to 72 clock cycles (because 72 MHz/ 1  MHz = 72 ).
	72 * xus calculates the number of clock cycles needed for the given delay in microseconds.
	*/
	SysTick->LOAD = 72 * xus;				//设置定时器重装值
	/* Clearing the Current Value Register:
	SysTick->VAL is the current value register of the SysTick timer.
	Setting it to 0x00 clears any current value, ensuring the timer starts counting from the reload value.
	*/ 
	SysTick->VAL = 0x00;					//清空当前计数值
	/* Enable SysTick, use processor clock (HCLK)
	SysTick->CTRL is the control and status register of the SysTick timer.
	0x00000005 sets the following bits:
		ENABLE (bit 0) = 1: Enables the counter.
		CLKSOURCE (bit 2) = 1: Selects the processor clock (HCLK) as the source.
	*/
	SysTick->CTRL = 0x00000005;				//设置时钟源为HCLK，启动定时器 
	/* Wait until the count flag is set:
	This line is a busy-wait loop that waits until the COUNTFLAG (bit 16) in the SysTick->CTRL register is set.
	The COUNTFLAG is set when the timer counts down to zero.
	*/
	while(!(SysTick->CTRL & 0x00010000));	//等待计数到0 
	/*Disabling SysTick::
	0x00000004 sets the CLKSOURCE bit to 0 and clears the ENABLE bit, effectively stopping the SysTick timer.
	*/
	SysTick->CTRL = 0x00000004;				//关闭定时器
}

/**
  * @brief  毫秒级延时
  * @param  xms 延时时长，范围：0~4294967295
  * @retval 无
  */
void Delay_ms(uint32_t xms)
{
	while(xms--)
	{
		Delay_us(1000);
	}
}
 
/**
  * @brief  秒级延时
  * @param  xs 延时时长，范围：0~4294967295
  * @retval 无
  */
void Delay_s(uint32_t xs)
{
	while(xs--)
	{
		Delay_ms(1000);
	}
} 
