#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "stdio.h"
#include "led.h"
#include "lcd.h"
#include "mq2.h"

 
int main()
{
	u16 adcx;
	MQ2_Init();//��������ʼ��
	delay_init(168);    //��ʼ����ʱ����
	uart_init(115200);//���ڳ�ʼ��

	while(1)
	{
		adcx=Get_ADCValue();
		printf("ADC1_CH5_VAL��%d \r\n", adcx);
		printf("Smog_1Ũ��Ϊ��%.2f ppm\r\n",MQ2_GetPPM1());//��������Ũ�Ȳ�ͨ�����ڴ�ӡ
//		printf("Smog_2Ũ��Ϊ��%.2f ppm\r\n",MQ2_GetPPM2());
		delay_ms(2000);
	}
}
