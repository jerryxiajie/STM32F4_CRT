#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 

#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART2_RX 			1		//使能（1）/禁止（0）串口1接收
#define COMD_LEN          10  // 定义命令字符串的长度

extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u8  COMMAND_BUF[COMD_LEN]; //命令接收缓冲区
extern u16 USART_RX_STA;         		//接收状态标记	
extern u16 SPA_NUM;         		//接收到的数据条数
extern u8 LED14_FLAG;
extern u8 COMD_FLAG;
extern u16 FACTOR[10];   // 字符串转整数数组
extern u8 String_Finish_Flag;  // 字符串输入完毕
extern struct FIFO8 * String_Fifo;
extern unsigned char * String_Buf;
extern u8 FIFO_Rec;
extern u8 STRING_RECVER;
extern u8 Res;

void uart_init(u32 bound);
void send_str(char * Str_Buff);
void string_to_value(void);
void string_to_value_new(void);
void string_para_fun(void); 
void display_buf(u8 Res);
u16 Power_Often(u8 index);

#endif



