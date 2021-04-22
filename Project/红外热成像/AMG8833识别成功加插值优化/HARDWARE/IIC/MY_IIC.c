#include "MY_IIC.H"
#include "delay.h"

//----------------------------------------------------
//IIC���ų�ʼ��`
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
//���Ϳ�ʼ�ź�
//------------------------------------------
u8 IIC_Start(void)  
{  
  /* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C���������ź� */  
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
//����ֹͣ�ź�
//----------------------------------------------
void IIC_Stop(void)  
{  
  /* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź� */  
  SDA_OUT();
  IIC_SDA = 0;  
  delay_us(4);  
  IIC_SCL = 1;  
  delay_us(4);  
  IIC_SDA = 1;  
} 
//-----------------------------------
//����һ���ֽ�
//-----------------------------------
void IIC_SendByte(u8 _ucByte)  
{  
  u8 i;  
  SDA_OUT();
  /* �ȷ����ֽڵĸ�λbit7 */  
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
      IIC_SDA = 1; // �ͷ�����  
    }  
    _ucByte <<= 1;    /* ����һ��bit */  
    delay_us(4);  
  }  
}  


//-------------------------------------
//��ȡһ���ֽ�
//����ֵ ��value
//-------------------------------------
u8 IIC_ReadByte(void)  
{  
  u8 i;  
  u8 value;  
  IIC_SDA = 1;  
  SDA_IN();
  /* ������1��bitΪ���ݵ�bit7 */  
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
  IIC_SDA = 1;  /* CPU�ͷ�SDA���� */  
  SDA_IN();
  delay_us(4);  
  IIC_SCL = 1;  /* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */  
  delay_us(4);  
  if (READ_SDA)   /* CPU��ȡSDA����״̬ */  
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
  IIC_SDA = 0;  /* CPU����SDA = 0 */  
  delay_us(4);  
  IIC_SCL = 1;  /* CPU����1��ʱ�� */  
  delay_us(4);  
  IIC_SCL = 0;  
  delay_us(4);  
  IIC_SDA = 1;  /* CPU�ͷ�SDA���� */  
}  
void IIC_NAck(void)  
{  
  SDA_OUT();
  IIC_SDA = 1;  /* CPU����SDA = 1 */  
  delay_us(4);  
  IIC_SCL = 1;  /* CPU����1��ʱ�� */  
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











