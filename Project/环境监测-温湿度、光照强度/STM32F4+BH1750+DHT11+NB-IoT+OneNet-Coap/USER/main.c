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
	u16 t=0;	//t用来计时	
	u8 send_dat[200];
	float temperature;  	    
	float humidity;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  		//初始化延时函数
	uart_init(115200);		//初始化串口波特率为115200		
	USART2_Init(115200);	//接M5311模块	
 	LCD_Init();						//LCD初始化   	
	LED_Init();
	BH1750_Init();
	DHT11_Init();
	delay_ms(10);					//延时
	LED0 = 0;
	link=1;          			//标记M5311初始化并连接Onenet成功
	printf("复位M5311模块!\r\n");
	LCD_ShowString(30,150,200,16,16,"Temp:   C");	 
 	LCD_ShowString(30,170,200,16,16,"Humi:   %");
	LED1 = 0;

  while(1)
	{
		LED0 = 1;
		if(USART2_RX_STA&0X8000)//接收到Onenet一次数据了
		{	
			strcpy(nb_iotdat,(char *)USART2_RX_BUF);//字符串复制
			printf("\r\n");
			printf("NB-IOT:%s\r\n",nb_iotdat);
			USART2_RX_STA=0;		   	//启动下一次接收	
			M5311_analysis();//解析M5311模块发送过来的数据
		}
		delay_ms(10);
		t++;
		
		DHT11_Read_Data(&temperature,&humidity);		//读取温湿度值
		LCD_ShowNum(30+40,150,temperature,2,16);		//显示温度	   		   
		LCD_ShowNum(30+40,170,humidity,2,16);			//显示湿度
		Start_BH1750();
		delay_ms(180); //典型测量时间是120ms，我们设置为180ms
		Read_BH1750(); 
		Convert_BH1750();
		sprintf((char*)BH_Buf,"Illumi:%3.1f %s",result_lx,"lx");//.1f 表示精确到小数点一位。%s:表示字符串类型。
		LCD_ShowString(30,190,210,16,16,BH_Buf);
		printf("Temp:%.1f C\r\n",temperature);					//串口助手显示温度、湿度、光强
		printf("Humi:%.1f %%\r\n",humidity);
		printf("illuminance:%.1f lx  \r\n",result_lx);	

		if(t>=10)//10秒钟发送一次数据，修改此值即可定义上传数据时间
		{
			t=0;
			LED0=!LED0;
			if(link)
			{
				sprintf((char *)send_dat,"AT+MIPLNOTIFY=0,0,3200,0,5750,1,17,\"T:%3.1f H:%3.1f I:%3.1f\",0,0", temperature, humidity, result_lx);
				m5311_send_cmd(send_dat,"OK",500);
				printf("up_data:%s\r\n",send_dat);
				m5311_send_cmd("AT+MIPLUPDATE=0,60,0","+MIPLEVENT:0,11",50);//在线命令，告诉Onenet我还活着的意思
			}
		}		
		delay_ms(1000);
	}
}
    

void M5311_analysis(void)//M5311数据解析
{
	if(strstr(nb_iotdat,"READY")!=0)//收到A9G模块启动完毕信息
	{
		link=0;
		printf("检测到M5311模块重启，正在重新初始化...\r\n");
		delay_ms(2000);
		init_m5311();
		printf("初始化M5311模块成功!\r\n");
		link=1;
	}
	else if(strstr(nb_iotdat,"+MIPLEVENT:0,15")!=0)//收到M5311模块离线信息
	{
		link=0;
		printf("检测到M5311模块离线，正在重新初始化...\r\n");
		delay_ms(2000);
		init_m5311();//初始化M5311模块
		printf("初始化M5311模块成功!\r\n");
		link=1;
	}
}

