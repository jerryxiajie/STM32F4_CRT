#include "pwm.h"

void TIM4_PWM_Init(u32 arr,u32 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 
	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4); // PD12 ���� TIM4_CH1

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;           //GPIOD12
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOD,&GPIO_InitStructure);              //��ʼ��PD12

	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���
	TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 

	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);//��ʼ����ʱ��4

	//��ʼ��TIM4 Channel1 PWMģʽ
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM_PWM1ģʽ
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�Ƚ��������:TIM������Ը�
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //��ʼ��TIM4OC1

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM4��CCR1�ϵ�Ԥװ�ؼĴ���
  TIM_ARRPreloadConfig(TIM4,ENABLE);//ARPEʹ��(�Զ���װ��Ԥװ��ʹ��)
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM4
}


void setPWMvalue(u16 pwm_factor)   // ������Χ��1��100
{
	u16 pwmvalue;

	/* 84M ��Ƶϵ��λ84�� ������Ƶ��Ϊ1M����װ��ֵΪ200��PWM��Ƶ��Ϊ1M/200 = 5KHz*/
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


