#include "sys.h"
#include "usart.h"	
#include "string.h"
#include "pdata.h"
#include "stdlib.h"


//USART_RX_STA,使得串口最大接收字节数为2的14次方
//USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)

//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART2->SR&0X40)==0);//循环发送,直到发送完毕   
	USART2->DR = (u8) ch;
	return ch;
}
#endif
 
#if EN_USART2_RX   //如果使能了接收
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
char RX_BUF_TEMP[USART_REC_LEN/2];   // 保存两个空格之间的字符串
u8  COMMAND_BUF[COMD_LEN];  // 保存命令字符串
u16 FACTOR[10];  // 保存每个字符串转换为数值的结果
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	
u16 SPA_NUM=0;   //接收到的数据条数
u8 LED14_FLAG=0;
u8 FACTOR_NUM = 0;
u8 COMD_FLAG = 0;  // 命令有效标志位
u8 index_t=0;  // 单个字符串的长度
u8 STRING_RECVER = 0;
u8 Res;

u8 String_Finish_Flag = 0;  // 字符串输入完毕
u8 FIFO_Rec= 0;
struct FIFO8 * String_Fifo;
unsigned char * String_Buf;

u8 t1;
u8 len;	

//初始化IO 串口2 
void uart_init(u32 bound){
   //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* USART2 配置 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //使能USART1时钟
 
	//串口2对应引脚复用映射
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);   //  TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);   // RX	
	
	//USART2端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA2，PA3

   //USART2 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART2, &USART_InitStructure); //初始化串口2

  USART_Cmd(USART2, ENABLE);  //使能串口2 
	
	USART_ClearFlag(USART2, USART_FLAG_TC);
	
#if EN_USART2_RX	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启相关中断
	
	//Usart2 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

#endif

}


void USART2_IRQHandler(void)                	//串口2中断服务程序
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(USART2);//(USART2->DR);	//读取接收到的数据
		
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(Res == 0x0d)
			{
				USART_RX_STA|=0x8000;	//接收完成了 
			}
			else
			{
				USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;				
				USART_RX_STA++;
				if(USART_RX_STA>(USART_REC_LEN-1))
				{
					USART_RX_STA=0;//接收数据错误,重新开始接收
				}
				else
				{
					STRING_RECVER = 1;
				}
			}
		}
  }
}

void display_buf(u8 Res_temp)
{
	/* 0x0d 0x0a  回车 换行*/
	
	USART_SendData(USART2, Res_temp); 
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
	
	if(Res_temp == 0x08)
	{
		turn_on_led(6);
		USART_SendData(USART2, 0x20);    // 替换为空格
	  while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
		
		if(USART_RX_STA > 2)   
		{
			USART_RX_STA = USART_RX_STA-2;  // 接收下标回退2
		}
		else
		{
			USART_RX_STA = 0;
		}
		
		USART_SendData(USART2, Res_temp);   // 回车到被删除的字符处
	  while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
	}
}


void send_str(char * Str_Buff)
{
	u8 t;
	len=strlen(Str_Buff);
	for(t=0;t<len;t++)
	{
		USART_SendData(USART2, Str_Buff[t]);         //向串口2发送数据
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
	}
}


void string_to_value_new(void)
{
	u8 len1;
	u8 first_flag  = 0;  // 是否第一次遇到字符标记
	u8 start_flag = 0; // 开始和结束记录标记
	u8 para_start_flag = 0;
	u8 i=0;
	u8 j=0; 
	u8 command_cnt = 0;
	int resault_temp = 0;
	index_t = 0;

  len1=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
	USART_RX_BUF[len1] = 0x20;  // 在末尾加空格符
	COMD_FLAG = 0;
	
	for(t1 = 0; t1 < len1+1; t1++)
	{
		if(USART_RX_BUF[t1] != 0x20)  // 非空格保存
		{
			if(first_flag == 0) // 第一次碰到非空格，接下来的字符存入命令数组
			{
				COMMAND_BUF[command_cnt++] = USART_RX_BUF[t1];
				start_flag = 1;
			}
			else  // 不是第一次碰到非空格，存入参数数组
			{
				RX_BUF_TEMP[index_t++] = USART_RX_BUF[t1];
				para_start_flag = 1; // 标记开始计参数
			}
		}
		else  // 空格处理
		{
			if(start_flag == 0)  //命令没有开始记录，第一次碰到非空格的标记始终为0
			{
				first_flag = 0;
			}
			else  // start_flag = 1;  命令已经开始记录，第一次碰到非空格的标记置位
			{
				first_flag = 1;
			}
			
			if(para_start_flag == 1)
			{
				para_start_flag = 0;
				
				printf("\r参数： ");
				for(i=0; i<index_t; i++)
				{
					USART_SendData(USART2, RX_BUF_TEMP[i]);         //向串口2发送数据
					while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
				}
				for(j=0; j<index_t; j++)
				{
					if(RX_BUF_TEMP[j] - 'x' == 0)
					{
						resault_temp = 0;
					}
					else if(RX_BUF_TEMP[j] - 'y' == 0)
					{
						resault_temp = 1;
					}
					else if (RX_BUF_TEMP[j] - 'z' == 0)
					{
						resault_temp = 2;
					}
					else
					{
						resault_temp += (RX_BUF_TEMP[j]-0x30)*Power_Often(index_t-j-1);  // 将字符串装换为16位整形数据
					}
				}
				index_t = 0;
				FACTOR[FACTOR_NUM++] = resault_temp;
			}
		}
		resault_temp = 0;
	}
	FACTOR_NUM = 0;
	USART_RX_STA = 0;
}


u16 Power_Often(u8 index)
{
	u16 result = 1;
	u8 i;
	if(index == 0)
		return 1;
	for(i=0; i<index; i++)
	{
		result*=10;
	}

	return result;
}




#endif

 



