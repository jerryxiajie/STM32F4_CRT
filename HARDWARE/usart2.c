#include "usart2.h"

void USART2_ConfigBaud(uint32_t BaudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
		/* USART1 ���� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE); //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //ʹ��USART1ʱ��
	
	//����2��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);   //  TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);   // RX	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // �ٶ�50MHZ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA, &GPIO_InitStructure);      //��ʼ��PA2(TX),PA3(RX)

//	NVIC_Configuration();

	USART_InitStructure.USART_BaudRate=BaudRate;   // ����������
	USART_InitStructure.USART_WordLength=USART_WordLength_8b; // �ֳ�Ϊ8λ�����ݸ�ʽ
	USART_InitStructure.USART_StopBits=USART_StopBits_1;  //1��ֹͣλ
	USART_InitStructure.USART_Parity=USART_Parity_No; // ����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;//�շ�ģʽ
	
	USART_Init(USART2,&USART_InitStructure);

	//USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);  // �ж�����,�����ж�
	USART_Cmd(USART2,ENABLE);	// ʹ�ܴ���
	USART_ClearFlag(USART2,USART_FLAG_TC);  //���־λ
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

