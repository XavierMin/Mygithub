#ifndef __ASM88XX_H
#define __ASM88XX_H
#include "sys.h"


#define AMG88xx_ADDRESS   (0x69)

enum
{
	AMG88xx_PCTL = 0x00,
	AMG88xx_RST = 0x01,
	AMG88xx_FPSC = 0x02,
	AMG88xx_INTC = 0x03,
	AMG88xx_STAT = 0x04,
	AMG88xx_SCLR = 0x05,
	//0x06 reserved
	AMG88xx_AVE = 0x07,
	AMG88xx_INTHL = 0x08,
	AMG88xx_INTHH = 0x09,
	AMG88xx_INTLL = 0x0A,
	AMG88xx_INTLH = 0x0B,
	AMG88xx_IHYSL = 0x0C,
	AMG88xx_IHYSH = 0x0D,
	AMG88xx_TTHL = 0x0E,
	AMG88xx_TTHH = 0x0F,
	AMG88xx_INT_OFFSET = 0x010,
	AMG88xx_PIXEL_OFFSET = 0x80
};

//����AMG8833�Ĺ���ģʽ
enum power_modes
{
	AMG88xx_NORMAL_MODE = 0x00,				//����ģʽ
	AMG88xx_SLEEP_MODE = 0x01,				//����ģʽ
	AMG88xx_STAND_BY_60 = 0x20,				//��Ъʱ��Ϊ60s�Ĵ���ģʽ
	AMG88xx_STAND_BY_10 = 0x21				//��Ъʱ��Ϊ10s�Ĵ���ģʽ
};

//������λ
enum sw_resets 			
{
	AMG88xx_FLAG_RESET = 0x30,				//Flag reset
	AMG88xx_INITIAL_RESET = 0x3F			//Initial reset
};

//�趨֡��
enum frame_rates 
{
	AMG88xx_FPS_10 = 0x00,						//10FPS
	AMG88xx_FPS_1 = 0x01							//1FPS
};

//�����жϹ���  bit0��INTEN
enum int_enables
{
	AMG88xx_INT_DISABLED = 0x00,			//INT���Ž�ֹ��������ָ���״̬��
	AMG88xx_INT_ENABLED = 0x01				//�������ʹ��
};

//�����жϹ���  bit1��INTMOD
enum int_modes 
{
	AMG88xx_DIFFERENCE = 0x00,				//Difference Interrupt Mode
	AMG88xx_ABSOLUTE_VALUE = 0x01			//Absolute Value Interrupt Mode
};
	
#define AMG88xx_PIXEL_ARRAY_SIZE 64
#define AMG88xx_PIXEL_TEMP_CONVERSION 0.25				//������·ֱ���0.25��
#define AMG88xx_THERMISTOR_CONVERSION 0.0625			//��������ֱ���0.625��

void amg88xx_init(void);
u8 amg88xx_write(u8 reg,u8 data);
u8 amg88xx_read_len(u8 reg,u8 len,u8 *buf);
//float AMG88XX_signedMag12ToFloat(uint16_t val);
float amg88xx_readThermistor(void);
void amg88xx_readPixels(float *buf, u8 size);
#endif








