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
    IIC_SendByte((AMG88xx_ADDRESS<<1)|0); //����������ַ+д����
    if(IIC_WaitAck())          //�ȴ�Ӧ��
    {
        IIC_Stop();
        return 1;
    }
    IIC_SendByte(reg);         //д�Ĵ�����ַ
    IIC_WaitAck();             //�ȴ�Ӧ��
    IIC_SendByte(data);        //��������
    if(IIC_WaitAck())          //�ȴ�ACK
    {
        IIC_Stop();
        return 1;
    }
    IIC_Stop();
    return 0;
}


//AMG8833������
//�������� �� ��ȡAMG883�Ĵ������¶�ԭʼֵ
//�βΣ� reg  Ҫ��ȡ�¶����ݵļĴ������׵�ַ   
// 			 len  ���ݵĳ���
//			 buf  �����ݵ�����					  
u8 amg88xx_read_len(u8 reg,u8 len,u8 *buf)
{ 
    IIC_Start();
    IIC_SendByte((AMG88xx_ADDRESS<<1)|0x00); //����������ַ+д����
    if(IIC_WaitAck())          //�ȴ�Ӧ��
    {
        IIC_Stop();
        return 1;
    }
    IIC_SendByte(reg);         //д�Ĵ�����ַ
    IIC_WaitAck();             //�ȴ�Ӧ��
	  IIC_Start();                
    IIC_SendByte((AMG88xx_ADDRESS<<1)|0x01); //����������ַ+������
    IIC_WaitAck();             //�ȴ�Ӧ��
    while(len)
    {
        if(len==1)
				{
					*buf=IIC_ReadByte();//������,����nACK 
					IIC_NAck();
				}
				else 
				{
					*buf=IIC_ReadByte();		//������,����ACK  
					IIC_Ack();
				}
				len--;
				buf++;  
    }
    IIC_Stop();                 //����һ��ֹͣ����
    return 0;       
}


//����ԭʼֵ�ж��¶����������Ǹ���
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







