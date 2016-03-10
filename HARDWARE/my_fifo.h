#ifndef _MY_FIFO_H
#define _MY_FIFO_H

#include "stm32f4xx.h"
#include "misc.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_usart.h"
#include "usart2.h"
#include "key_led.h"
#include "nvic_tim.h"
#include "pwm.h"
#include "i2c.h"
#include "LSM303.h"

//�������

/*  0:����     1����� */
 #define FLAGS_OVERRUN 0x0001  
/*  
        buf-  ��������ַ
        size- ��С
        free-  ��������
        putP- ��һ������д���ַ
        getP- ��һ�����ݶ�ȡ��ַ
				flags- �����־
*/  
typedef struct FIFO8{  
         unsigned char *buf;  
         int putP,getP,size,free,flags;  
}My_FIFO;

void fifo8_init(struct FIFO8 *fifo,int size, unsigned char *buf);  
int fifo8_put(struct FIFO8 *fifo,unsigned char data);  
int fifo8_get(struct FIFO8 *fifo);  
int fifo8_status(struct FIFO8 *fifo);  
int fifo8_free(struct FIFO8 *fifo);  
int fifo8_dispaly(struct FIFO8 *fifo);


#endif
