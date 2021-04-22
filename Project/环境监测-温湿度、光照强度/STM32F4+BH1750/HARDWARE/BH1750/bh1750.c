#include "bh1750.h"
#include "IIC.h"
#include "delay.h"
#include "stdio.h"

float result_lx=0;

u8 BUF[2]={0};
u16 result=0;

//��ʼ��
void BH1750_Init(void)
{
	  IIC_Init();
}

void BH1750_data_send(u8 cmd)//��ȫ����BH1750�����ֲ��д
{
    IIC_Start();                    //IIC��ʼ�ź�
	
    IIC_Send_Byte(BH1750_Addr+0);   //�����豸��ַ+д�ź�
	  while(IIC_Wait_Ack());
			
	
    IIC_Send_Byte(cmd);             //�ڲ��Ĵ�����ַ
   while(IIC_Wait_Ack());
	
    IIC_Stop();                     //����ֹͣ�ź�
	  delay_ms(5);
}
void Start_BH1750(void)
{
	  BH1750_data_send(BH1750_ON);	  //power on��������������
	  BH1750_data_send(BH1750_RSET);	//���ã�����Ĵ�������
	  BH1750_data_send(BH1750_ONE);   //����ģʽ��һ��H�ֱ���ģʽ������120ms��֮���Զ�����ϵ�ģʽ    
}
void Read_BH1750(void)//��ȫ����BH1750�����ֲ��д
{   	
    IIC_Start();                          //IIC��ʼ�ź�
    IIC_Send_Byte(BH1750_Addr+1);         //�����豸��ַ+���ź�
	  while(IIC_Wait_Ack());
	  BUF[0]=IIC_Read_Byte(1);              
	  BUF[1]=IIC_Read_Byte(0);              
 
    IIC_Stop();                           //IICֹͣ�ź�
    delay_ms(5);
}
void Convert_BH1750(void)
{
	
	  result=BUF[0];
	  result=(result<<8)+BUF[1];  //�ϳɹ���ǿ��
	
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
