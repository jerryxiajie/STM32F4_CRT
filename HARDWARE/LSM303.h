#ifndef __LSM303_H
#define __LSM303_H

#include "sys.h" 

extern int ACC_Buffer[6];
extern float ACC_XYZ[3];

void LSM303_Init(void);
u8 LSM303_ReadOneByte(u8 ReadAddr);
void LSM303_WriteOneByte(u8 WriteAddr,u8 DataToWrite);
void LSM303_WriteLenByte(u8 WriteAddr,u32 DataToWrite,u8 Len);
u32 LSM303_ReadLenByte(u16 ReadAddr,u8 Len);
void LSM303_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead);
void LSM303_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite);
void LSM303_ACC_Init(void);
void LSM_ACC_Read(void);
void ACC_Result_XYZ(void);


#endif

