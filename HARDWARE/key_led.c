#include "key_led.h"
#include "pdata.h"

#define WK_UP 	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)	//PA0

void KEY_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA,GPIOE时钟
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
} 

void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; 
	/* LED 配置 */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
  GPIO_Init(GPIOD, &GPIO_InitStructure);
}

u8 KEY_Scan(void)
{
	if(WK_UP==1)
	{
		delay_ms(10);
	  if(WK_UP==1)return 1;
	}
 	return 0;
}

//mode = 0; 不连续按； mode = 1； 连续按
u8 KEY_Scan_st(u8 mode)
{	 
	static u8 key_up=1;  // static 变量保存按下的状态
	if(mode)key_up=1;
	if(key_up&&WK_UP==1)
	{
		delay_ms(10);//去抖动
		key_up=0;
		if(WK_UP==1)return 1;
	}else if(WK_UP==0)key_up=1;
 	return 0;
}


void turn_on_led(u8 which_led)   // 打开LED3-LED5，参数为3.4.5.6
{
	switch(which_led)
	{
		case 3: 
			turn_off_led(0);
		  GPIO_SetBits(GPIOD, GPIO_Pin_13);
			break;
		case 4:
			turn_off_led(0);
		  GPIO_SetBits(GPIOD, GPIO_Pin_12);
			break;
		case 5:
			turn_off_led(0);
		  GPIO_SetBits(GPIOD, GPIO_Pin_14);
			break;
		case 6:
			turn_off_led(0);
		  GPIO_SetBits(GPIOD, GPIO_Pin_15);
			break;
		default:
			GPIO_ResetBits(GPIOD, GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
			printf("\n 参数错误，led3-led6");
			break;
	}
}

void turn_off_led(u8 which_led)   // 关闭LED3-LED5，参数为3.4.5.6
{
	switch(which_led)
	{
		case 3: 
		  GPIO_ResetBits(GPIOD, GPIO_Pin_12);
			break;
		case 4:
		  GPIO_ResetBits(GPIOD, GPIO_Pin_13);
			break;
		case 5:
		  GPIO_ResetBits(GPIOD, GPIO_Pin_14);
			break;
		case 6:
		  GPIO_ResetBits(GPIOD, GPIO_Pin_15);
			break;
		default:
			GPIO_ResetBits(GPIOD, GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
			break;
	}
}

