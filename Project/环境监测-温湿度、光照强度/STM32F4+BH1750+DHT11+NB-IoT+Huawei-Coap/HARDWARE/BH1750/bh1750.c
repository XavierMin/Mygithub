#include "bh1750.h"
#include "IIC.h"
#include "delay.h"
#include "stdio.h"

float result_lx=0;

u8 BUF[2]={0};
u16 result=0;

//初始化
void BH1750_Init(void)
{
	  IIC_Init();
}

void BH1750_data_send(u8 cmd)//完全按照BH1750数据手册编写
{
    IIC_Start();                    //IIC起始信号
	
    IIC_Send_Byte(BH1750_Addr+0);   //发送设备地址+写信号
	  while(IIC_Wait_Ack());
			
	
    IIC_Send_Byte(cmd);             //内部寄存器地址
   while(IIC_Wait_Ack());
	
    IIC_Stop();                     //发送停止信号
	  delay_ms(5);
}
void Start_BH1750(void)
{
	  BH1750_data_send(BH1750_ON);	  //power on，发送启动命令
	  BH1750_data_send(BH1750_RSET);	//重置，清除寄存器内容
	  BH1750_data_send(BH1750_ONE);   //设置模式，一次H分辨率模式，至少120ms，之后自动进入断电模式    
}
void Read_BH1750(void)//完全按照BH1750数据手册编写
{   	
    IIC_Start();                          //IIC起始信号
    IIC_Send_Byte(BH1750_Addr+1);         //发送设备地址+读信号
	  while(IIC_Wait_Ack());
	  BUF[0]=IIC_Read_Byte(1);              
	  BUF[1]=IIC_Read_Byte(0);              
 
    IIC_Stop();                           //IIC停止信号
    delay_ms(5);
}
void Convert_BH1750(void)
{
	
	  result=BUF[0];
	  result=(result<<8)+BUF[1];  //合成光照强度
	
	  result_lx=result/1.2;
	 	
}

/*
void  bh_data_send(u8 command)
{
	do
	{
		iic_start();
		iic_byte_write(BHAddwrite);
	}
	while(iic_wait_ack());
	iic_byte_write(command);
	iic_wait_ack();
	iic_stop();

}
u16 bh_data_read(void)
{
	u16 buf;
	iic_start();
	iic_byte_write(BHAddread);
	iic_wait_ack();
	buf=iic_byte_read(1);
	buf=buf<<8;
	buf+=0x00ff&iic_byte_read(0);
	iic_stop();
	return buf;

*/
