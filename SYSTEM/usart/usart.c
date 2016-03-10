#include "sys.h"
#include "usart.h"	
#include "string.h"
#include "pdata.h"
#include "stdlib.h"


//USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)

//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART2->SR&0X40)==0);//ѭ������,ֱ���������   
	USART2->DR = (u8) ch;
	return ch;
}
#endif
 
#if EN_USART2_RX   //���ʹ���˽���
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
char RX_BUF_TEMP[USART_REC_LEN/2];   // ���������ո�֮����ַ���
u8  COMMAND_BUF[COMD_LEN];  // ���������ַ���
u16 FACTOR[10];  // ����ÿ���ַ���ת��Ϊ��ֵ�Ľ��
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	
u16 SPA_NUM=0;   //���յ�����������
u8 LED14_FLAG=0;
u8 FACTOR_NUM = 0;
u8 COMD_FLAG = 0;  // ������Ч��־λ
u8 index_t=0;  // �����ַ����ĳ���
u8 STRING_RECVER = 0;
u8 Res;

u8 String_Finish_Flag = 0;  // �ַ����������
u8 FIFO_Rec= 0;
struct FIFO8 * String_Fifo;
unsigned char * String_Buf;

u8 t1;
u8 len;	

//��ʼ��IO ����2 
void uart_init(u32 bound){
   //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* USART2 ���� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE); //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //ʹ��USART1ʱ��
 
	//����2��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);   //  TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);   // RX	
	
	//USART2�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA2��PA3

   //USART2 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART2, &USART_InitStructure); //��ʼ������2

  USART_Cmd(USART2, ENABLE);  //ʹ�ܴ���2 
	
	USART_ClearFlag(USART2, USART_FLAG_TC);
	
#if EN_USART2_RX	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//��������ж�
	
	//Usart2 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

#endif

}


void USART2_IRQHandler(void)                	//����2�жϷ������
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART2);//(USART2->DR);	//��ȡ���յ�������
		
		if((USART_RX_STA&0x8000)==0)//����δ���
		{
			if(Res == 0x0d)
			{
				USART_RX_STA|=0x8000;	//��������� 
			}
			else
			{
				USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;				
				USART_RX_STA++;
				if(USART_RX_STA>(USART_REC_LEN-1))
				{
					USART_RX_STA=0;//�������ݴ���,���¿�ʼ����
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
	/* 0x0d 0x0a  �س� ����*/
	
	USART_SendData(USART2, Res_temp); 
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	
	if(Res_temp == 0x08)
	{
		turn_on_led(6);
		USART_SendData(USART2, 0x20);    // �滻Ϊ�ո�
	  while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
		
		if(USART_RX_STA > 2)   
		{
			USART_RX_STA = USART_RX_STA-2;  // �����±����2
		}
		else
		{
			USART_RX_STA = 0;
		}
		
		USART_SendData(USART2, Res_temp);   // �س�����ɾ�����ַ���
	  while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	}
}


void send_str(char * Str_Buff)
{
	u8 t;
	len=strlen(Str_Buff);
	for(t=0;t<len;t++)
	{
		USART_SendData(USART2, Str_Buff[t]);         //�򴮿�2��������
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	}
}


void string_to_value_new(void)
{
	u8 len1;
	u8 first_flag  = 0;  // �Ƿ��һ�������ַ����
	u8 start_flag = 0; // ��ʼ�ͽ�����¼���
	u8 para_start_flag = 0;
	u8 i=0;
	u8 j=0; 
	u8 command_cnt = 0;
	int resault_temp = 0;
	index_t = 0;

  len1=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
	USART_RX_BUF[len1] = 0x20;  // ��ĩβ�ӿո��
	COMD_FLAG = 0;
	
	for(t1 = 0; t1 < len1+1; t1++)
	{
		if(USART_RX_BUF[t1] != 0x20)  // �ǿո񱣴�
		{
			if(first_flag == 0) // ��һ�������ǿո񣬽��������ַ�������������
			{
				COMMAND_BUF[command_cnt++] = USART_RX_BUF[t1];
				start_flag = 1;
			}
			else  // ���ǵ�һ�������ǿո񣬴����������
			{
				RX_BUF_TEMP[index_t++] = USART_RX_BUF[t1];
				para_start_flag = 1; // ��ǿ�ʼ�Ʋ���
			}
		}
		else  // �ո���
		{
			if(start_flag == 0)  //����û�п�ʼ��¼����һ�������ǿո�ı��ʼ��Ϊ0
			{
				first_flag = 0;
			}
			else  // start_flag = 1;  �����Ѿ���ʼ��¼����һ�������ǿո�ı����λ
			{
				first_flag = 1;
			}
			
			if(para_start_flag == 1)
			{
				para_start_flag = 0;
				
				printf("\r������ ");
				for(i=0; i<index_t; i++)
				{
					USART_SendData(USART2, RX_BUF_TEMP[i]);         //�򴮿�2��������
					while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
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
						resault_temp += (RX_BUF_TEMP[j]-0x30)*Power_Often(index_t-j-1);  // ���ַ���װ��Ϊ16λ��������
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

 



