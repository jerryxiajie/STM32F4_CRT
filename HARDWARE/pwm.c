#include "pwm.h"

void TIM4_PWM_Init(u32 arr,u32 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 
	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4); // PD12 复用 TIM4_CH1

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;           //GPIOD12
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOD,&GPIO_InitStructure);              //初始化PD12

	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数
	TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 

	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);//初始化定时器4

	//初始化TIM4 Channel1 PWM模式
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM_PWM1模式
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //比较输出极性:TIM输出极性高
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //初始化TIM4OC1

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR1上的预装载寄存器
  TIM_ARRPreloadConfig(TIM4,ENABLE);//ARPE使能(自动重装载预装载使能)
	TIM_Cmd(TIM4, ENABLE);  //使能TIM4
}


void setPWMvalue(u16 pwm_factor)   // 参数范围从1到100
{
	u16 pwmvalue;

	/* 84M 分频系数位84， 计数的频率为1M，重装载值为200，PWM的频率为1M/200 = 5KHz*/
	TIM4_PWM_Init(200-1,84-1);
	pwmvalue = pwm_factor*2;
	TIM_SetCompare1(TIM4,pwmvalue);
}


void stop_pwm()
{
	TIM_Cmd(TIM4,DISABLE);
	LED_Init();
	GPIO_ResetBits(GPIOD, GPIO_Pin_12);
}


