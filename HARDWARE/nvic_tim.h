#ifndef __NVIC_TIM_H
#define __NVIC_TIM_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "pdata.h"

extern u8 led_flag;

void TIM3_Int_Init(u16 arr,u16 psc);
void set_TIM3_time(u16 arr);
void stop_timer(void);

#endif

