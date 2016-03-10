#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "pdata.h"

int main(void)
{
	u8 acc_flag = 0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);		//��ʱ��ʼ�� 
	uart_init(115200);	//���ڳ�ʼ��������Ϊ115200
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�  
	IIC_Init();

/*
		LED ���Ʋ��֣������ʽ�� led n(3|4|5|6)  ����nȷ��������յ��;
		PWM ���Ʋ��֣������ʽ:  pwm n(0 - 100)  ����nȷ��ռ�ձȣ����������� ld4 ������;
		TIMER ���Ʋ���: �����ʽ��  tim n(20 - 10000)  ����nȷ����ʱ��ms��
		ACC ���Ʋ��֣� �����ʽ:   acc n(x|y|z)  ����nȷ�������һ��ļ��ٶ�����
		
*/
	
  while(1)
	{
		if(STRING_RECVER == 1)  // ���յ�һ���ַ����ڽ�����ʾ
		{
			display_buf(Res);
			STRING_RECVER = 0;
		}

		if(USART_RX_STA&0x8000)  // �ַ����������
		{
			string_to_value_new();  // ����������ַ��������������
			
			if(COMD_FLAG != 1)
			{
				if(COMMAND_BUF[0]-'l' == 0 && COMMAND_BUF[1]-'e' == 0 && COMMAND_BUF[2]-'d' == 0)
				{
					stop_timer();
					stop_pwm();
					acc_flag = 0;	
					turn_off_led(0);
					if(FACTOR[0] != 3 && FACTOR[0] != 4 && FACTOR[0] != 5 && FACTOR[0] != 6)
					{
						printf("\r����������������룡����Ϊ3|4|5|6");
						printf("\r");
					}
					else
					{
						printf("\rLED����: %d",FACTOR[0]);
					  turn_on_led(FACTOR[0]);
						printf("\r\r");
					}
				}
				else if(COMMAND_BUF[0]-'p' == 0 && COMMAND_BUF[1]-'w' == 0 && COMMAND_BUF[2]-'m' == 0)
				{
					stop_timer();
					acc_flag = 0;			
					turn_off_led(0);
					if(FACTOR[0] - 100 > 0 || FACTOR[0] - 0 < 0)
					{
						printf("\r����������������룡������Χ��0��100");
						printf("\r");
					}
					else
					{
						printf("\rPWMռ�ձ�:%d",FACTOR[0]);
					  setPWMvalue(FACTOR[0]);
						printf("\r\r");
					}
				}
				else if(COMMAND_BUF[0]-'t' == 0 && COMMAND_BUF[1]-'i' == 0 && COMMAND_BUF[2]-'m' == 0)
				{
					stop_pwm();
					acc_flag = 0;
					turn_off_led(0);
					
					if(FACTOR[0] > 10000 || FACTOR[0] < 20)
					{
						printf("\r����������������룡������Χ��20��10000");
						printf("\r");
					}
					else
					{
					  printf("\rTIMER��ʱʱ�䣺%d ",FACTOR[0]);
					  send_str("ms");
					  set_TIM3_time(FACTOR[0]);
						printf("\r\r");
					}
				}
				else if(COMMAND_BUF[0]-'a' == 0 && COMMAND_BUF[1]-'c' == 0 && COMMAND_BUF[2]-'c' == 0)
				{
					acc_flag = 1;
					if(FACTOR[0] != 0 && FACTOR[0] != 1 && FACTOR[0] != 2)
					{
						printf("\r������������Ĭ�����Z�ᣡ");
						printf("\r\r");
					}
				}
				else
				{
					acc_flag = 0;
					turn_off_led(0);
					printf("\r����������������룡");
					printf("\r\r");
				}
			}
		}
		if(acc_flag)
		{
			LSM303_ACC_Init();
			LSM_ACC_Read();
			ACC_Result_XYZ();
			switch(FACTOR[0])
			{
				case 0:
					printf("\rX�᣺%f", ACC_XYZ[0]);
					break;
				case 1:
					printf("\rY�᣺%f", ACC_XYZ[1]);
					break;
				case 2:
					printf("\rZ�᣺%f.3", ACC_XYZ[2]);
					break;
				default:printf("\rZ�᣺%f", ACC_XYZ[2]);
			}
			delay_ms(100);
		}
		
	};
}


