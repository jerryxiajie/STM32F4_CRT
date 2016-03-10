#ifndef __PWM_H
#define __PWM_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "pdata.h"

void TIM4_PWM_Init(u32 arr,u32 psc);
void setPWMvalue(u16 pwm_factor);
void stop_pwm(void);

#endif

