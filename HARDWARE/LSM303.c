#include "LSM303.h"
#include "i2c.h"
#include "delay.h"

u8 ACC_XYZ_addr[6] = {0x28,0x29,0x2a,0x2b,0x2c,0x2d};
int ACC_Buffer[6];
float ACC_XYZ[3];

//��ʼ��IIC�ӿ�
void LSM303_Init(void)
{
	IIC_Init();//IIC��ʼ��
}

void LSM303_ACC_Init(void)
{
	LSM303_WriteOneByte(0x20,0x47);  // REG1_A ��0x47   DATA_RATE:50HZ; XYZ��ʹ��
	LSM303_WriteOneByte(0x23,0x48);  // REG4_A : 0X48  +-2g; HR(high_resolution �߷ֱ���ʹ��);��˴洢
}

// ��ȡ���ټƵ����ݣ�6��ֵ���� X��Y��Z��ĵͰ�λ�͸߰�λ���ȵ�λ���λ
void LSM_ACC_Read(void)
{
	u8 i;
	for(i=0;i<6;i++)
	{
		ACC_Buffer[i] = LSM303_ReadOneByte(ACC_XYZ_addr[i]);
	}
}

//���ټ����ݴ���
void ACC_Result_XYZ(void)
{
	u8 j;
	int LA_COM;
	
	for(j = 0 ; j < 3; j++)
	{
		LA_COM = (ACC_Buffer[2*j]<<8)|ACC_Buffer[2*j+1];  //��˴洢
		LA_COM = LA_COM >> 4;
		if(LA_COM & 0X0800)
		{
			LA_COM = 0 - (((~LA_COM)+1)&0x0fff);  // ��ֵȡ����һ
		}
		ACC_XYZ[j] = LA_COM*1.0/1023;
	}
}

// ����ַ��ȡ���ټƵ�����
u8 ACC_readaddr(u8 temp)
{
	u8 result;
	LSM303_ACC_Init();
	result = LSM303_ReadOneByte(temp);
	return result;
}

//��ָ����ַ����һ������
//  ���ټ�A �� ����ַ��0x33   д��ַ��  0x32
//  ������M �� ����ַ��0x3c   д��ַ��  0x3d
u8 LSM303_ReadOneByte(u8 ReadAddr)
{				  
	u8 temp=0;		  	    																 
  IIC_Start();  
	IIC_Send_Byte(0X32);	   //����д����
	IIC_Wait_Ack();
	IIC_Send_Byte(ReadAddr);//���Ͷ�ȡ��ַ	    
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(0X33);    //�������ģʽ			   
	IIC_Wait_Ack();	 
  temp=IIC_Read_Byte(0);		   
  IIC_Stop();//����һ��ֹͣ����	    
	return temp;
}

//��ָ����ַд��һ������
void LSM303_WriteOneByte(u8 WriteAddr,u8 DataToWrite)
{				   	  	    																 
	IIC_Start();  
  IIC_Send_Byte(0X32);	    //����д����
	IIC_Wait_Ack();
	IIC_Send_Byte(WriteAddr);//���͵�ַ	   
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(DataToWrite);     //�����ֽ�							   
	IIC_Wait_Ack();  		    	   
  IIC_Stop();//����һ��ֹͣ���� 
	delay_ms(10);
}

//��ָ����ַ��ʼд�볤��ΪLen������
void LSM303_WriteLenByte(u8 WriteAddr,u32 DataToWrite,u8 Len)
{  	
	u8 t;
	for(t=0;t<Len;t++)
	{
		LSM303_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}												    
}

//��ָ����ַ��ʼ��������ΪLen������
u32 LSM303_ReadLenByte(u16 ReadAddr,u8 Len)
{
	u8 t;
	u32 temp=0;
	for(t=0;t<Len;t++)
	{
		temp<<=8;
		temp+=LSM303_ReadOneByte(ReadAddr+Len-t-1); 	 				   
	}
	return temp;												    
}

//��ָ����ַ��ʼ����ָ������������
void LSM303_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=LSM303_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}
}  

//��ָ����ַ��ʼд��ָ������������
void LSM303_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
	while(NumToWrite--)
	{
		LSM303_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}


