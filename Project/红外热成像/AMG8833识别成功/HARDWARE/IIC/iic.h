#ifndef __IIC_H
#define __IIC_H
#include "sys.h"


//IO��������
 
#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}
#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;}

//IO��������	 
#define IIC_SCL    PBout(6) //SCL
#define IIC_SDA    PBout(7) //SDA	 
#define READ_SDA   PBin(7)  //����SDA 

//#define I2C_SCL_1()		GPIO_WriteBit(GPIOB, GPIO_Pin_6, Bit_SET)
//#define I2C_SCL_0()		GPIO_WriteBit(GPIOB, GPIO_Pin_6, Bit_RESET)

//#define I2C_SDA_1()		GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_SET)
//#define I2C_SDA_0()		GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_RESET)

//#define I2C_SCL_READ()	GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6)
//#define I2C_SDA_READ()	GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)

#define I2C_WR	0		/* д����bit */
#define I2C_RD	1		/* ������bit */



//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif
















