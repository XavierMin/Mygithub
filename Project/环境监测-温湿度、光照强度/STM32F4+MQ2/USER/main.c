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
	MQ2_Init();//传感器初始化
	delay_init(168);    //初始化延时函数
	uart_init(115200);//串口初始化

	while(1)
	{
		adcx=Get_ADCValue();
		printf("ADC1_CH5_VAL：%d \r\n", adcx);
		printf("Smog_1浓度为：%.2f ppm\r\n",MQ2_GetPPM1());//计算烟雾浓度并通过串口打印
//		printf("Smog_2浓度为：%.2f ppm\r\n",MQ2_GetPPM2());
		delay_ms(2000);
	}
}
