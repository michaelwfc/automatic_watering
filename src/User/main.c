#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <OLED.h>
#include <Key.h>
#include "Delay.h"
#include <timer.h>
#include <Clock.h>
#include <Relay.h>



uint16_t clock_value;
// uint16_t relay_duration;
ClockDef clock;


int main(void)
{
	OLED_Init();
	timer1_init(); 
	timer2_clock_init();
	timer3_count_init();
	RELAY_CONTROL_Init();
	Key_interupt_init();
	
	uint16_t mode,pre_mode, period, pre_period, is_runing;
	
	ClockDef watering_time,clock_type;
	clock = init_clock(8,0,0);
	watering_time = init_clock(8,0,30);
	uint16_t duration=120;
	

	while(1)
	{	
		// get clock from timer2
		clock = get_clock();
		// get mode from interupt
		mode= get_mode();
		
		// mode = MODE_Key_GetNum(mode);
		OLED_ShowString(4,1,"Mode:");	
		OLED_ShowNum(4,6,mode,1);
		
		if(mode==0)
		{	
			show_clock(clock,1,0);
			show_clock(watering_time,2,0);
			//OLED_ShowString(3,1,"Duration:");
			OLED_ShowString(2,9,"+");
			OLED_ShowNum(2,10,duration,3);
			OLED_ShowString(2,13,"'");
			
			//OLED_ShowString(3,1,"Status:");	
			int is_start = compareClocks(&clock , &watering_time);	
		
			// get the running state from timer3
			is_runing = is_relay_runing();		
			if(is_start||is_runing)
			{	
				// pass duration to start the relay
				if(is_start){start_relay_duration(duration);}
				
				OLED_ShowString(3,1,"Open  ");		
//				uint16_t count=0;
//				while(count <duration)
//				{
//					set_relay_control_high_volt();
//					OLED_ShowString(3,9,"Open  ");
//					timer1_delay_s(1);
//					count++;
//				}
			}
			else
			{
				set_relay_control_low_volt();
				OLED_ShowString(3,1,"Closed");
			}

		}
		else{ // enter into the setting modes
			
			if(mode==1)
			{
				clock_type= clock;
			}else if(mode==2)
			{
				clock_type= watering_time;
			}
			
			if (mode!=pre_mode)
			{
				// inital the period when trange the mode
				period= 0;}
			else
			{
				// get the period from period key
				period = get_period();
			}
				
			
			//period = PERIOD_Key_GetNum(period);
			if( (mode==1||mode==2)&&((mode!=pre_mode) || (period!=pre_period)))
			{
				//update the clock value accoring to mode, period
				switch(period)
				{
					case 0:
						clock_value =  clock_type.hour;
						break;
					case 1:
						clock_value = clock_type.minute;
						break;
					case 2:
						clock_value = clock_type.second;
						break;
				}
			}
			
			if(mode==3&&(mode!=pre_mode))
			{	
				// intial the clock_value when mode 3
				clock_value  =  duration;
			}
			// get the value from external interput by keys
			clock_value = get_value();
			
	
			switch (mode)
			{
				case 1: //enter into setting clock mode
					if(period==0){
						// get clock_value by interput
						// clock_value= get_value();
						// set the clock_value to period
						clock.hour=clock_value;
					}else if(period==1)
					{

						// clock_value= get_value();
						clock.minute=clock_value;
					}else
					{
						//clock_value= get_value();
						clock.second=clock_value;
					}
//					clock_value = INCREASE_Key_GetNum(clock_value);
//					clock_value = DECREASE_Key_GetNum(clock_value);
					show_clock_intevally(clock,1,1,period);
					break;
				case 2: //enter into setting watering mode
					if(period==0){
						watering_time.hour=clock_value;
					}else if(period==1)
					{
						watering_time.minute=clock_value;
					}else
					{
						watering_time.second=clock_value;
					}
					show_clock_intevally(watering_time,2,1,period);
					break;
				case 3: 
					duration = clock_value;
					show_duration_intevally(duration,2,10);
					break;
			}
			

			
			pre_mode =mode;
			pre_period =period;
		
		}
				
				
	}		
}

