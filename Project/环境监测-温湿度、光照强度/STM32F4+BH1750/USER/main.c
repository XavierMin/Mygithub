#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "bh1750.h"
#include "iic.h"
/*夏季在阳光直接照射下，光照强度可达6万～10万lx，
没有太阳的室外0.1万～1万lx，夏天明朗的室内100～550lx，夜间满月下为0.2lx。
根据范围，我们采集到的数据是正确的*/

extern float result_lx;
u8 BH_Buf[];

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);		//初始化串口波特率为115200				
 	LCD_Init();					//LCD初始化   	
	LED_Init();
	BH1750_Init();//!!!!刚开始为什么一直显示不出来，因为没有写这一句话，没有对IIC进行初始化。

  while(1)
		{
			LED0=!LED0;
			delay_ms(100);
			Start_BH1750();//最初程序运行到这里就运行不下去了，查询到的原因是 没有写IIC_Init();在Start_BH1750()中，用到了IIC_Init()里面的程序。
			delay_ms(180); //典型测量时间是120ms，我们设置为180ms
			Read_BH1750(); 
			Convert_BH1750(); 
			sprintf((char*)BH_Buf,"Illumi:%3.1f %s",result_lx,"lx");//.1f 表示精确到小数点一位。%s:表示字符串类型。
			//sprintf();这个函数的功能是把第三部分的数据，按照第二部分格式化字符的格式，把第三部分的数据进行格式化，然后把格式化后的数据类型，存储到字符串的缓冲区间里去
			//在显示光照度的时候，如果光照度变为十位数/个位数，光照度的单位会变为“lxx”“lxxx”,目前还没有是哪里出现了问题。
			
			LCD_ShowString(30,170,210,16,16,BH_Buf);
			printf("illuminance:%.1f lx  \r\n",result_lx);	
			delay_ms(1000);
		}
}

//		 LCD_ShowString(30,200,200,16,16,"Illumi:  ");
//		Illuminance_data[0]=(int)(result_lx)/10000+'0';            //65535.1
//		LCD_ShowChar(90,200,Illuminance_data[0],16,0);
//		
//		Illuminance_data[1]=(int)(result_lx)%10000/1000+'0';       //5535
//		LCD_ShowChar(100,200,Illuminance_data[1],16,0);
//		
//		Illuminance_data[2]=(int)(result_lx)%10000%1000/100+'0';   //535
//		LCD_ShowChar(110,200,Illuminance_data[2],16,0);
//		
//		Illuminance_data[3]=(int)(result_lx)%10000%1000%100/10+'0';   //35
//		LCD_ShowChar(120,200,Illuminance_data[3],16,0);
//		
//		Illuminance_data[4]=(int)(result_lx)%10+'0';
//		LCD_ShowChar(130,200,Illuminance_data[4],16,0);
//		
//		Illuminance_data[5]='.';
//		LCD_ShowChar(140,200,Illuminance_data[5],16,0);
//		
//		Illuminance_data[6]=(int)(result_lx*10)%10+'0';
//		LCD_ShowChar(150,200,Illuminance_data[6],16,0);
//		
//	  Illuminance_data[7]='l';
//		LCD_ShowChar(160,200,Illuminance_data[7],16,0);
//		
//		Illuminance_data[8]='x';
//		LCD_ShowChar(170,200,Illuminance_data[8],16,0);
    

