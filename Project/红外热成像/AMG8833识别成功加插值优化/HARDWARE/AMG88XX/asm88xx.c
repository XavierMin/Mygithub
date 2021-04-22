#include "asm88xx.h"
#include "delay.h"
//#include "iic.h"
#include "MY_IIC.h"


u8 xx;


void amg88xx_init(void)
{
	IIC_Init();
	amg88xx_write(AMG88xx_PCTL, AMG88xx_NORMAL_MODE);
	amg88xx_write(AMG88xx_RST, AMG88xx_INITIAL_RESET);
	amg88xx_write(AMG88xx_FPSC, AMG88xx_FPS_10);
}

u8 amg88xx_write(u8 reg,u8 data)
{
    IIC_Start();
    IIC_SendByte((AMG88xx_ADDRESS<<1)|0); //发送器件地址+写命令
    if(IIC_WaitAck())          //等待应答
    {
        IIC_Stop();
        return 1;
    }
    IIC_SendByte(reg);         //写寄存器地址
    IIC_WaitAck();             //等待应答
    IIC_SendByte(data);        //发送数据
    if(IIC_WaitAck())          //等待ACK
    {
        IIC_Stop();
        return 1;
    }
    IIC_Stop();
    return 0;
}


//AMG8833读函数
//函数作用 ： 读取AMG883寄存器的温度原始值
//形参： reg  要读取温度数据的寄存器的首地址   
// 			 len  数据的长度
//			 buf  存数据的数组					  
u8 amg88xx_read_len(u8 reg,u8 len,u8 *buf)
{ 
    IIC_Start();
    IIC_SendByte((AMG88xx_ADDRESS<<1)|0x00); //发送器件地址+写命令
    if(IIC_WaitAck())          //等待应答
    {
        IIC_Stop();
        return 1;
    }
    IIC_SendByte(reg);         //写寄存器地址
    IIC_WaitAck();             //等待应答
	  IIC_Start();                
    IIC_SendByte((AMG88xx_ADDRESS<<1)|0x01); //发送器件地址+读命令
    IIC_WaitAck();             //等待应答
    while(len)
    {
        if(len==1)
				{
					*buf=IIC_ReadByte();//读数据,发送nACK 
					IIC_NAck();
				}
				else 
				{
					*buf=IIC_ReadByte();		//读数据,发送ACK  
					IIC_Ack();
				}
				len--;
				buf++;  
    }
    IIC_Stop();                 //产生一个停止条件
    return 0;       
}


//根据原始值判断温度是正数还是负数
float AMG88XX_signedMag12ToFloat(u8 val)			//uint16_t
{
	//take first 11 bits as absolute val
	u16 absVal = (val & 0x7FF);
	
	return (val & 0x8000) ? 0 - (float)absVal : (float)absVal ;
}

					  
float amg88xx_readThermistor(void)
{
	u8 raw[2];
	u16 recast;
	
	amg88xx_read_len(AMG88xx_TTHL, 2, raw);
	recast = ((uint16_t)raw[1] << 8) | ((uint16_t)raw[0]);
	return (float)(AMG88XX_signedMag12ToFloat(recast) * AMG88xx_THERMISTOR_CONVERSION);
}

void amg88xx_readPixels(float *buf, u8 size)		
{
	u16 recast;
	int i;
	u8 pos=0; 
	float converted;
	u8 rawArray[128];
	
	amg88xx_read_len(AMG88xx_PIXEL_OFFSET, 128, rawArray);
	
	for(i=0; i<size; i++)
	{
	  pos = i << 1;
		recast = ((u16)rawArray[pos + 1] << 8) | ((u16)rawArray[pos]);	
		
		converted = (float)(AMG88XX_signedMag12ToFloat(recast) * AMG88xx_PIXEL_TEMP_CONVERSION);
		buf[i] = converted;
	}
}







