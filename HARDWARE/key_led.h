#ifndef __KEY_LED_H
#define __KEY_LED_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"

#define LED0 PFout(9)	// DS0
#define LED1 PFout(10)	// DS1	 

void KEY_Init(void);
u8 KEY_Scan(void);
void LED_Init(void);
void turn_on_led(u8 which_led);
void turn_off_led(u8 which_led);

#endif

