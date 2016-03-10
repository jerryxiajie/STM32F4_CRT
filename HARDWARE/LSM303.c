#include "LSM303.h"
#include "i2c.h"
#include "delay.h"

u8 ACC_XYZ_addr[6] = {0x28,0x29,0x2a,0x2b,0x2c,0x2d};
int ACC_Buffer[6];
float ACC_XYZ[3];

//初始化IIC接口
void LSM303_Init(void)
{
	IIC_Init();//IIC初始化
}

void LSM303_ACC_Init(void)
{
	LSM303_WriteOneByte(0x20,0x47);  // REG1_A ：0x47   DATA_RATE:50HZ; XYZ轴使能
	LSM303_WriteOneByte(0x23,0x48);  // REG4_A : 0X48  +-2g; HR(high_resolution 高分辨率使能);大端存储
}

// 读取加速计的数据，6个值包括 X、Y、Z轴的低八位和高八位，先低位后高位
void LSM_ACC_Read(void)
{
	u8 i;
	for(i=0;i<6;i++)
	{
		ACC_Buffer[i] = LSM303_ReadOneByte(ACC_XYZ_addr[i]);
	}
}

//加速计数据处理
void ACC_Result_XYZ(void)
{
	u8 j;
	int LA_COM;
	
	for(j = 0 ; j < 3; j++)
	{
		LA_COM = (ACC_Buffer[2*j]<<8)|ACC_Buffer[2*j+1];  //大端存储
		LA_COM = LA_COM >> 4;
		if(LA_COM & 0X0800)
		{
			LA_COM = 0 - (((~LA_COM)+1)&0x0fff);  // 负值取反加一
		}
		ACC_XYZ[j] = LA_COM*1.0/1023;
	}
}

// 按地址读取加速计的数据
u8 ACC_readaddr(u8 temp)
{
	u8 result;
	LSM303_ACC_Init();
	result = LSM303_ReadOneByte(temp);
	return result;
}

//在指定地址读出一个数据
//  加速计A 的 读地址：0x33   写地址：  0x32
//  磁力计M 的 读地址：0x3c   写地址：  0x3d
u8 LSM303_ReadOneByte(u8 ReadAddr)
{				  
	u8 temp=0;		  	    																 
  IIC_Start();  
	IIC_Send_Byte(0X32);	   //发送写命令
	IIC_Wait_Ack();
	IIC_Send_Byte(ReadAddr);//发送读取地址	    
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(0X33);    //进入接收模式			   
	IIC_Wait_Ack();	 
  temp=IIC_Read_Byte(0);		   
  IIC_Stop();//产生一个停止条件	    
	return temp;
}

//在指定地址写入一个数据
void LSM303_WriteOneByte(u8 WriteAddr,u8 DataToWrite)
{				   	  	    																 
	IIC_Start();  
  IIC_Send_Byte(0X32);	    //发送写命令
	IIC_Wait_Ack();
	IIC_Send_Byte(WriteAddr);//发送地址	   
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(DataToWrite);     //发送字节							   
	IIC_Wait_Ack();  		    	   
  IIC_Stop();//产生一个停止条件 
	delay_ms(10);
}

//在指定地址开始写入长度为Len的数据
void LSM303_WriteLenByte(u8 WriteAddr,u32 DataToWrite,u8 Len)
{  	
	u8 t;
	for(t=0;t<Len;t++)
	{
		LSM303_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}												    
}

//在指定地址开始读出长度为Len的数据
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

//在指定地址开始读出指定个数的数据
void LSM303_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=LSM303_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}
}  

//在指定地址开始写入指定个数的数据
void LSM303_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
	while(NumToWrite--)
	{
		LSM303_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}


