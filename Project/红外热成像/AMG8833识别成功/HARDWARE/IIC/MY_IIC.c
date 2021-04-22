#include "MY_IIC.H"
#include "delay.h"

//----------------------------------------------------
//IIC引脚初始化`
//---------------------------------------------------
void IIC_Init(void)
{					 
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	

	GPIO_InitStructure.GPIO_Pin 	= GPIO_SCL|GPIO_SDA;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP ;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_Init(IIC_PERIPH, &GPIO_InitStructure);
	GPIO_SetBits(IIC_PERIPH,GPIO_SCL|GPIO_SDA); 	
	
}
//------------------------------------------
//发送开始信号
//------------------------------------------
u8 IIC_Start(void)  
{  
  /* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */  
  SDA_OUT();
  IIC_SDA = 1;  
  delay_us(4); 
  IIC_SCL = 1;  
  delay_us(4); 	
  IIC_SDA = 0;  
  delay_us(4);  
  IIC_SCL = 0;  
  delay_us(4);  
	return 1;
} 
//----------------------------------------------
//发送停止信号
//----------------------------------------------
void IIC_Stop(void)  
{  
  /* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */  
  SDA_OUT();
  IIC_SDA = 0;  
  delay_us(4);  
  IIC_SCL = 1;  
  delay_us(4);  
  IIC_SDA = 1;  
} 
//-----------------------------------
//发送一个字节
//-----------------------------------
void IIC_SendByte(u8 _ucByte)  
{  
  u8 i;  
  SDA_OUT();
  /* 先发送字节的高位bit7 */  
  for (i = 0; i < 8; i++)  
  {       
    if (_ucByte & 0x80)  
    {  
      IIC_SDA = 1;  
    }  
    else  
    {  
      IIC_SDA = 0;  
    }  
    delay_us(4);  
    IIC_SCL = 1;  
    delay_us(4);      
    IIC_SCL = 0;  
    if (i == 7)  
    {  
      IIC_SDA = 1; // 释放总线  
    }  
    _ucByte <<= 1;    /* 左移一个bit */  
    delay_us(4);  
  }  
}  


//-------------------------------------
//读取一个字节
//返回值 ：value
//-------------------------------------
u8 IIC_ReadByte(void)  
{  
  u8 i;  
  u8 value;  
  IIC_SDA = 1;  
  SDA_IN();
  /* 读到第1个bit为数据的bit7 */  
  value = 0;  
  for (i = 0; i < 8; i++)  
  {  
    value <<= 1;  
    IIC_SCL = 1;  
    delay_us(4);  
    if (READ_SDA)  
    {  
      value++;  
    }  
    IIC_SCL = 0;  
    delay_us(4);  
  }  
  return value;  
}  

u8 IIC_WaitAck(void)  
{  
  u8 re;  
  IIC_SDA = 1;  /* CPU释放SDA总线 */  
  SDA_IN();
  delay_us(4);  
  IIC_SCL = 1;  /* CPU驱动SCL = 1, 此时器件会返回ACK应答 */  
  delay_us(4);  
  if (READ_SDA)   /* CPU读取SDA口线状态 */  
  {  
    re = 1;  
  }  
  else  
  {  
    re = 0;  
  }  
  IIC_SCL = 0;  
  delay_us(4);  
  return re;  
}  
void IIC_Ack(void)  
{  
  SDA_OUT();
  IIC_SDA = 0;  /* CPU驱动SDA = 0 */  
  delay_us(4);  
  IIC_SCL = 1;  /* CPU产生1个时钟 */  
  delay_us(4);  
  IIC_SCL = 0;  
  delay_us(4);  
  IIC_SDA = 1;  /* CPU释放SDA总线 */  
}  
void IIC_NAck(void)  
{  
  SDA_OUT();
  IIC_SDA = 1;  /* CPU驱动SDA = 1 */  
  delay_us(4);  
  IIC_SCL = 1;  /* CPU产生1个时钟 */  
  delay_us(4);  
  IIC_SCL = 0;  
  delay_us(4);    
}  

void SDA_OUT()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin 	= GPIO_SDA;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP ;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_Init(IIC_PERIPH, &GPIO_InitStructure);
}

void SDA_IN()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin 	= GPIO_SDA;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN_FLOATING ;
	GPIO_Init(IIC_PERIPH, &GPIO_InitStructure);
}











