#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "pdata.h"

int main(void)
{
	u8 acc_flag = 0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);		//延时初始化 
	uart_init(115200);	//串口初始化波特率为115200
	LED_Init();		  		//初始化与LED连接的硬件接口  
	IIC_Init();

/*
		LED 控制部分：命令格式： led n(3|4|5|6)  参数n确定点亮哪盏灯;
		PWM 控制部分：命令格式:  pwm n(0 - 100)  参数n确定占空比，具体体现在 ld4 的亮度;
		TIMER 控制部分: 命令格式：  tim n(20 - 10000)  参数n确定延时的ms数
		ACC 控制部分： 命令格式:   acc n(x|y|z)  参数n确定输出哪一轴的加速度数据
		
*/
	
  while(1)
	{
		if(STRING_RECVER == 1)  // 接收到一个字符，在界面显示
		{
			display_buf(Res);
			STRING_RECVER = 0;
		}

		if(USART_RX_STA&0x8000)  // 字符串输入完毕
		{
			string_to_value_new();  // 处理输入的字符串，分离出参数
			
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
						printf("\r命令错误！请重新输入！参数为3|4|5|6");
						printf("\r");
					}
					else
					{
						printf("\rLED控制: %d",FACTOR[0]);
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
						printf("\r命令错误！请重新输入！参数范围在0到100");
						printf("\r");
					}
					else
					{
						printf("\rPWM占空比:%d",FACTOR[0]);
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
						printf("\r命令错误！请重新输入！参数范围在20到10000");
						printf("\r");
					}
					else
					{
					  printf("\rTIMER定时时间：%d ",FACTOR[0]);
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
						printf("\r参数输入有误；默认输出Z轴！");
						printf("\r\r");
					}
				}
				else
				{
					acc_flag = 0;
					turn_off_led(0);
					printf("\r命令错误！请重新输入！");
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
					printf("\rX轴：%f", ACC_XYZ[0]);
					break;
				case 1:
					printf("\rY轴：%f", ACC_XYZ[1]);
					break;
				case 2:
					printf("\rZ轴：%f.3", ACC_XYZ[2]);
					break;
				default:printf("\rZ轴：%f", ACC_XYZ[2]);
			}
			delay_ms(100);
		}
		
	};
}


