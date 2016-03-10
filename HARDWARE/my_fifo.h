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

//定义变量

/*  0:正常     1：溢出 */
 #define FLAGS_OVERRUN 0x0001  
/*  
        buf-  缓冲区地址
        size- 大小
        free-  空余容量
        putP- 下一个数据写入地址
        getP- 下一个数据读取地址
				flags- 溢出标志
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
