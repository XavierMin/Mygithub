#ifndef __IIC_H
#define __IIC_H
#include "sys.h"


//IO方向设置
 
#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}
#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;}

//IO操作函数	 
#define IIC_SCL    PBout(6) //SCL
#define IIC_SDA    PBout(7) //SDA	 
#define READ_SDA   PBin(7)  //输入SDA 

//#define I2C_SCL_1()		GPIO_WriteBit(GPIOB, GPIO_Pin_6, Bit_SET)
//#define I2C_SCL_0()		GPIO_WriteBit(GPIOB, GPIO_Pin_6, Bit_RESET)

//#define I2C_SDA_1()		GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_SET)
//#define I2C_SDA_0()		GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_RESET)

//#define I2C_SCL_READ()	GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6)
//#define I2C_SDA_READ()	GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)

#define I2C_WR	0		/* 写控制bit */
#define I2C_RD	1		/* 读控制bit */



//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif
















