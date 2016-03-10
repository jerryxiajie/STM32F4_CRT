#include "my_fifo.h"  
#include "pdata.h"

/* ��ʼ�� */
void fifo8_init(struct FIFO8 *fifo,int size, unsigned char *buf)  
{  
		fifo->buf=buf;
		fifo->flags=0;        
		fifo->free=size;
		fifo->size=size;
		fifo->putP=0;                  
		fifo->getP=0;                   
}  

/* ��FIFO��д������ */
int fifo8_put(struct FIFO8 *fifo,unsigned char data)  
{  
		if(fifo->free==0) // ʣ������Ϊ0�������־λ��λ
		{  
			fifo->flags |= FLAGS_OVERRUN;  
			return -1;  
		}
		fifo->buf[fifo->putP] = data;
		fifo->putP++;
		
		 //ѭ�����л�����  
		if(fifo->putP == fifo->size)
		{  
			 fifo->putP = 0;  
		}  
		fifo->free--;

		return 0;  
}

/* ��FIFO��ȡ��һ������ */
int fifo8_get(struct FIFO8 *fifo)  
{  
		int data;  
		if(fifo->free == fifo->size)  // FIFO ��û������
		{  
			return -1;  
		}  
		data = fifo->buf[fifo->getP]; 
		fifo->getP++;  
		
		if(fifo->getP == fifo->size)  //ѭ�� 
		{
			fifo->getP = 0;  
		}  
		fifo->free++;  
			
		return data;  
}

/* ��������ʹ�õ����� */
int fifo8_status(struct FIFO8 *fifo)    
{  
   return fifo->size-fifo->free;  
}  

/* ������ʣ������ */
int fifo8_free(struct FIFO8 *fifo)   
{  
   return fifo->free;  
}  

/* ��ʾ�������е������ַ� */
int fifo8_dispaly(struct FIFO8 *fifo)
{
		u8 i;
		if(fifo->free == fifo->size)  // FIFO ��û������
		{  
			return -1;  
		}
		for(i=0; i<fifo->putP; i++)
		{
			USART_SendData(USART2, fifo->buf[i]);         //�򴮿�2��������
		  while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
		}
		
		return 1;
}




