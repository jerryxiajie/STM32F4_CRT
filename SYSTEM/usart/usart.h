#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 

#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART2_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
#define COMD_LEN          10  // ���������ַ����ĳ���

extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u8  COMMAND_BUF[COMD_LEN]; //������ջ�����
extern u16 USART_RX_STA;         		//����״̬���	
extern u16 SPA_NUM;         		//���յ�����������
extern u8 LED14_FLAG;
extern u8 COMD_FLAG;
extern u16 FACTOR[10];   // �ַ���ת��������
extern u8 String_Finish_Flag;  // �ַ����������
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



