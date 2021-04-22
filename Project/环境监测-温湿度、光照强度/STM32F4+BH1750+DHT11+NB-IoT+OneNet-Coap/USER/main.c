#include "sys.h"
#include "delay.h"
#include <stdbool.h>
#include <stdio.h>
#include "usart2.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "bh1750.h"
#include "dht11.h"
#include "iic.h"
#include "M5311.h"

void M5311_analysis(void);

char nb_iotdat[200];
extern float result_lx;
u8 BH_Buf[200];
u8 link=0;

int main(void)
{
	u16 t=0;	//t������ʱ	
	u8 send_dat[200];
	float temperature;  	    
	float humidity;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  		//��ʼ����ʱ����
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200		
	USART2_Init(115200);	//��M5311ģ��	
 	LCD_Init();						//LCD��ʼ��   	
	LED_Init();
	BH1750_Init();
	DHT11_Init();
	delay_ms(10);					//��ʱ
	LED0 = 0;
	link=1;          			//���M5311��ʼ��������Onenet�ɹ�
	printf("��λM5311ģ��!\r\n");
	LCD_ShowString(30,150,200,16,16,"Temp:   C");	 
 	LCD_ShowString(30,170,200,16,16,"Humi:   %");
	LED1 = 0;

  while(1)
	{
		LED0 = 1;
		if(USART2_RX_STA&0X8000)//���յ�Onenetһ��������
		{	
			strcpy(nb_iotdat,(char *)USART2_RX_BUF);//�ַ�������
			printf("\r\n");
			printf("NB-IOT:%s\r\n",nb_iotdat);
			USART2_RX_STA=0;		   	//������һ�ν���	
			M5311_analysis();//����M5311ģ�鷢�͹���������
		}
		delay_ms(10);
		t++;
		
		DHT11_Read_Data(&temperature,&humidity);		//��ȡ��ʪ��ֵ
		LCD_ShowNum(30+40,150,temperature,2,16);		//��ʾ�¶�	   		   
		LCD_ShowNum(30+40,170,humidity,2,16);			//��ʾʪ��
		Start_BH1750();
		delay_ms(180); //���Ͳ���ʱ����120ms����������Ϊ180ms
		Read_BH1750(); 
		Convert_BH1750();
		sprintf((char*)BH_Buf,"Illumi:%3.1f %s",result_lx,"lx");//.1f ��ʾ��ȷ��С����һλ��%s:��ʾ�ַ������͡�
		LCD_ShowString(30,190,210,16,16,BH_Buf);
		printf("Temp:%.1f C\r\n",temperature);					//����������ʾ�¶ȡ�ʪ�ȡ���ǿ
		printf("Humi:%.1f %%\r\n",humidity);
		printf("illuminance:%.1f lx  \r\n",result_lx);	

		if(t>=10)//10���ӷ���һ�����ݣ��޸Ĵ�ֵ���ɶ����ϴ�����ʱ��
		{
			t=0;
			LED0=!LED0;
			if(link)
			{
				sprintf((char *)send_dat,"AT+MIPLNOTIFY=0,0,3200,0,5750,1,17,\"T:%3.1f H:%3.1f I:%3.1f\",0,0", temperature, humidity, result_lx);
				m5311_send_cmd(send_dat,"OK",500);
				printf("up_data:%s\r\n",send_dat);
				m5311_send_cmd("AT+MIPLUPDATE=0,60,0","+MIPLEVENT:0,11",50);//�����������Onenet�һ����ŵ���˼
			}
		}		
		delay_ms(1000);
	}
}
    

void M5311_analysis(void)//M5311���ݽ���
{
	if(strstr(nb_iotdat,"READY")!=0)//�յ�A9Gģ�����������Ϣ
	{
		link=0;
		printf("��⵽M5311ģ���������������³�ʼ��...\r\n");
		delay_ms(2000);
		init_m5311();
		printf("��ʼ��M5311ģ��ɹ�!\r\n");
		link=1;
	}
	else if(strstr(nb_iotdat,"+MIPLEVENT:0,15")!=0)//�յ�M5311ģ��������Ϣ
	{
		link=0;
		printf("��⵽M5311ģ�����ߣ��������³�ʼ��...\r\n");
		delay_ms(2000);
		init_m5311();//��ʼ��M5311ģ��
		printf("��ʼ��M5311ģ��ɹ�!\r\n");
		link=1;
	}
}

