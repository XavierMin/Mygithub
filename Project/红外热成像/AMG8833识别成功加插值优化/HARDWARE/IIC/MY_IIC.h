#ifndef _H_MY_IIC_H_
#define _H_MY_IIC_H_

#include "sys.h"

#define IIC_SCL       	PBout(6) 	//SCL
#define IIC_SDA        	PBout(7) 	//SDA	  
#define GPIO_SCL        GPIO_Pin_6
#define GPIO_SDA        GPIO_Pin_7
#define IIC_PERIPH    	GPIOB
#define READ_SDA      	PBin(7) 



void IIC_Init(void);
u8 IIC_Start(void);
void IIC_Stop(void);
void IIC_SendByte(u8 txd);
u8 IIC_ReadByte(void);  
u8 IIC_WaitAck(void); 
void IIC_Ack(void); 
void IIC_NAck(void);
void SDA_OUT(void);
void SDA_IN(void);

#endif


