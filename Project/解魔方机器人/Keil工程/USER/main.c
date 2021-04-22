#include "sys.h"
#include "delay.h"  
#include "usart.h" 
#include "led.h"
#include "lcd.h"
#include "key.h"  
#include "touch.h" 
#include "cubeSolver.h"
#include "cubeSolverNew.h"


extern u8 nrf_flag,tmp_buf[];
extern char val[];

int main(void)
{ 
	u8 key
	 //,mode
	 ;
	//u16 t=0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  			//初始化延时函数
	uart_init(115200);			//初始化串口波特率为115200 		    
	USART2_Configuration();

 	LED_Init();     
	LCD_Init();
	KEY_Init();
	tp_dev.init();
	tp_dev.xfac = 0.0662251636;
	tp_dev.yfac = 0.0861273482;
	tp_dev.xoff = 0xFFF4;
	tp_dev.yoff = 0xFFEA;
	tp_dev.touchtype = 0x00;	
	  
							   
	POINT_COLOR = BLACK;		//定义画笔颜色
	LCD_Clear(WHITE);
	
	LCD_ShowString(15, 10, 240, 24, 24, "Initialise tables...");
	
	LED0 = 0;						//LED亮（红灯）
	
//	USART_SendData(USART2, 0x59);					//Y
	
	delay_ms(500);	
	LED0 = 1;
	
	LCD_Clear(WHITE);
//	LCD_Fill(0, 0, 240, 16, WHITE);
	LCD_ShowString(15, 10, 240, 24, 24, "Ready.");
	DrawCubeColor();
	
//	USART_SendData(USART2, 0x54);				//T
	
	while(1)
	{
		key = KEY_Scan(0);
		if(key == KEY_RIGHT)
		{
			if(CheckInput())
			{	
				LED0 = 0;	
				LCD_ShowString(15, 10, 240, 24, 24, "Calculate...");
				
//				USART_SendData(USART2, 0x58);					//X
//				USART_SendData(USART1, 0x59);					//Y
				Solve();
//				USART_SendData(USART2, 0x57);				//W
				
				LED0 = 1;
				LCD_Fill(0, 0, 400, 40, WHITE);
				LCD_ShowString(15, 10, 240, 24, 24, "OK!");	
				while(1);
			}
		}
		else if(key == KEY_UP)
		{

		}
		if(ScanTPInput())
		{
			DrawCubeColor();
		}

		delay_ms(100);	

	}

}




