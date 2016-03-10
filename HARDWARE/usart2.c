#include "usart2.h"

void USART2_ConfigBaud(uint32_t BaudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
		/* USART1 配置 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //使能USART1时钟
	
	//串口2对应引脚复用映射
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);   //  TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);   // RX	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 速度50MHZ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);      //初始化PA2(TX),PA3(RX)

//	NVIC_Configuration();

	USART_InitStructure.USART_BaudRate=BaudRate;   // 波特率设置
	USART_InitStructure.USART_WordLength=USART_WordLength_8b; // 字长为8位的数据格式
	USART_InitStructure.USART_StopBits=USART_StopBits_1;  //1个停止位
	USART_InitStructure.USART_Parity=USART_Parity_No; // 无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;//收发模式
	
	USART_Init(USART2,&USART_InitStructure);

	//USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);  // 中断配置,接收中断
	USART_Cmd(USART2,ENABLE);	// 使能串口
	USART_ClearFlag(USART2,USART_FLAG_TC);  //清标志位
}

/*
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
*/

