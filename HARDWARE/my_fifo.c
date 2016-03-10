#include "my_fifo.h"  
#include "pdata.h"

/* 初始化 */
void fifo8_init(struct FIFO8 *fifo,int size, unsigned char *buf)  
{  
		fifo->buf=buf;
		fifo->flags=0;        
		fifo->free=size;
		fifo->size=size;
		fifo->putP=0;                  
		fifo->getP=0;                   
}  

/* 向FIFO中写入数据 */
int fifo8_put(struct FIFO8 *fifo,unsigned char data)  
{  
		if(fifo->free==0) // 剩余容量为0，溢出标志位置位
		{  
			fifo->flags |= FLAGS_OVERRUN;  
			return -1;  
		}
		fifo->buf[fifo->putP] = data;
		fifo->putP++;
		
		 //循环队列缓冲区  
		if(fifo->putP == fifo->size)
		{  
			 fifo->putP = 0;  
		}  
		fifo->free--;

		return 0;  
}

/* 从FIFO中取出一个数据 */
int fifo8_get(struct FIFO8 *fifo)  
{  
		int data;  
		if(fifo->free == fifo->size)  // FIFO 中没有数据
		{  
			return -1;  
		}  
		data = fifo->buf[fifo->getP]; 
		fifo->getP++;  
		
		if(fifo->getP == fifo->size)  //循环 
		{
			fifo->getP = 0;  
		}  
		fifo->free++;  
			
		return data;  
}

/* 缓冲区被使用的容量 */
int fifo8_status(struct FIFO8 *fifo)    
{  
   return fifo->size-fifo->free;  
}  

/* 缓冲区剩余容量 */
int fifo8_free(struct FIFO8 *fifo)   
{  
   return fifo->free;  
}  

/* 显示缓冲区中的所有字符 */
int fifo8_dispaly(struct FIFO8 *fifo)
{
		u8 i;
		if(fifo->free == fifo->size)  // FIFO 中没有数据
		{  
			return -1;  
		}
		for(i=0; i<fifo->putP; i++)
		{
			USART_SendData(USART2, fifo->buf[i]);         //向串口2发送数据
		  while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
		}
		
		return 1;
}




