#include "delay.h"
#include "key.h"
#include "sys.h"
#include "LCDlcd.h"
#include "LCDshow.h"
#include "led.h"
#include "usart.h"
#include "asm88xx.h"
#include "interpolation.h"


#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

const uint16_t camColors[] = {0x480F,
0x400F,0x400F,0x400F,0x4010,0x3810,0x3810,0x3810,0x3810,0x3010,0x3010,
0x3010,0x2810,0x2810,0x2810,0x2810,0x2010,0x2010,0x2010,0x1810,0x1810,
0x1811,0x1811,0x1011,0x1011,0x1011,0x0811,0x0811,0x0811,0x0011,0x0011,
0x0011,0x0011,0x0011,0x0031,0x0031,0x0051,0x0072,0x0072,0x0092,0x00B2,
0x00B2,0x00D2,0x00F2,0x00F2,0x0112,0x0132,0x0152,0x0152,0x0172,0x0192,
0x0192,0x01B2,0x01D2,0x01F3,0x01F3,0x0213,0x0233,0x0253,0x0253,0x0273,
0x0293,0x02B3,0x02D3,0x02D3,0x02F3,0x0313,0x0333,0x0333,0x0353,0x0373,
0x0394,0x03B4,0x03D4,0x03D4,0x03F4,0x0414,0x0434,0x0454,0x0474,0x0474,
0x0494,0x04B4,0x04D4,0x04F4,0x0514,0x0534,0x0534,0x0554,0x0554,0x0574,
0x0574,0x0573,0x0573,0x0573,0x0572,0x0572,0x0572,0x0571,0x0591,0x0591,
0x0590,0x0590,0x058F,0x058F,0x058F,0x058E,0x05AE,0x05AE,0x05AD,0x05AD,
0x05AD,0x05AC,0x05AC,0x05AB,0x05CB,0x05CB,0x05CA,0x05CA,0x05CA,0x05C9,
0x05C9,0x05C8,0x05E8,0x05E8,0x05E7,0x05E7,0x05E6,0x05E6,0x05E6,0x05E5,
0x05E5,0x0604,0x0604,0x0604,0x0603,0x0603,0x0602,0x0602,0x0601,0x0621,
0x0621,0x0620,0x0620,0x0620,0x0620,0x0E20,0x0E20,0x0E40,0x1640,0x1640,
0x1E40,0x1E40,0x2640,0x2640,0x2E40,0x2E60,0x3660,0x3660,0x3E60,0x3E60,
0x3E60,0x4660,0x4660,0x4E60,0x4E80,0x5680,0x5680,0x5E80,0x5E80,0x6680,
0x6680,0x6E80,0x6EA0,0x76A0,0x76A0,0x7EA0,0x7EA0,0x86A0,0x86A0,0x8EA0,
0x8EC0,0x96C0,0x96C0,0x9EC0,0x9EC0,0xA6C0,0xAEC0,0xAEC0,0xB6E0,0xB6E0,
0xBEE0,0xBEE0,0xC6E0,0xC6E0,0xCEE0,0xCEE0,0xD6E0,0xD700,0xDF00,0xDEE0,
0xDEC0,0xDEA0,0xDE80,0xDE80,0xE660,0xE640,0xE620,0xE600,0xE5E0,0xE5C0,
0xE5A0,0xE580,0xE560,0xE540,0xE520,0xE500,0xE4E0,0xE4C0,0xE4A0,0xE480,
0xE460,0xEC40,0xEC20,0xEC00,0xEBE0,0xEBC0,0xEBA0,0xEB80,0xEB60,0xEB40,
0xEB20,0xEB00,0xEAE0,0xEAC0,0xEAA0,0xEA80,0xEA60,0xEA40,0xF220,0xF200,
0xF1E0,0xF1C0,0xF1A0,0xF180,0xF160,0xF140,0xF100,0xF0E0,0xF0C0,0xF0A0,
0xF080,0xF060,0xF040,0xF020,0xF800,};


//low range of the sensor (this will be blue on the screen)
#define MINTEMP 14
//high range of the sensor (this will be red on the screen)
#define MAXTEMP 30


#define INTERPOLATED_COLS 32
#define INTERPOLATED_ROWS 32
#define AMG_COLS 8
#define AMG_ROWS 8

//宏定义布尔类型
#define BOOL int
#define TRUE 1
#define FALSE 0

float pixels[AMG_COLS * AMG_ROWS];
uint16_t displayPixelWidth, displayPixelHeight;
uint16_t displayPixelX, displayPixelY;



float map(float val, float I_Min, float I_Max, float O_Min, float O_Max)
{
    return(val/(I_Max-I_Min)*(O_Max-O_Min) + O_Min);
}


/* 画热成像素点 */
void drawpixels(float *p, uint8_t rows, uint8_t cols, uint8_t boxWidth, uint8_t boxHeight, BOOL showVal) 
{
  int colorTemp, x, y;
  float curMax = get_point(p, rows, cols, 0, 0), curMin = get_point(p, rows, cols, 0, 0), curMid = get_point(p, rows, cols, cols/2, rows/2);
  volatile uint16_t colorMax = camColors[0], colorMin = camColors[0], colorMid = camColors[0]; 
	uint8_t colorIndex = map(colorTemp, MINTEMP, MAXTEMP, 0, 255);

  //画热成像
  for (y=0; y<rows; y++) {
    for (x=0; x<cols; x++) {
      float val = get_point(p, rows, cols, x, y);
			
			if(val < 28)
			{
				colorTemp = BLUE;
			}
			else if(val < 29)
			{
				colorTemp = GREEN;
			}
			else if(val < 30)
			{
				colorTemp = YELLOW;
			}
			else if(val < 32.5)
			{
				colorTemp = RED;
			}
			else if(val < 35)
			{
				colorTemp = BRRED;
			}
			
      LCD_Fill(boxWidth * x, boxHeight * y, boxWidth * x + 10, boxHeight * y + 10, colorTemp);
      
      //取最大值
      if(val >= curMax){    
        curMax = val;
        colorMax = camColors[colorIndex];
      }          
      //取最小值 
      if(val <= curMin){
        curMin = val;
        colorMin = camColors[colorIndex];
      }
//      //取中心点
//      if(y == rows/2 && x == cols/2){
//        colorMid = camColors[colorIndex];
//      }
			
//      if (showVal) {
//        LCD_SetCursor(boxWidth * y + boxWidth/2 - 12, 40 + boxHeight * x + boxHeight/2 - 4);
//        tft.setTextSize(1);
//        tft.setTextColor(TFT_WHITE);
//        tft.print(val,1);
//      }
    } 
	}
	
	//显示最高温度
	Show_Str(70, 360, RED, RED, "MAX Temperature:", 32, 1);
	LCD_ShowNum(200, 360, curMax, 4, 16);		//显示数值


//  tft.setTextDatum(TL_DATUM);
//  tft.drawString(F("MAX"), tft.width()-40, 0);    
//  tft.fillRect(tft.width()-45, 10, 40, 30, colorMax);  
//  tft.setTextDatum(CL_DATUM);
//  memset(temp, 0, sizeof(temp));
//  sprintf(temp, "%0.1f", curMax);
//  tft.drawString(String(temp), tft.width()-40, 25);
  //显示最低温度
	Show_Str(70, 400, BLACK, RED, "MIN Temperature:", 32, 1);
	LCD_ShowNum(200, 400, curMin, 4, 16);		//显示数值

//  tft.setTextDatum(TL_DATUM);
//  tft.drawString(F("MIN"), tft.width()-40, 47);    
//  tft.fillRect(tft.width()-45, 57, 40, 30, colorMin); 
//  tft.setTextDatum(CL_DATUM);     
//  memset(temp, 0, sizeof(temp));
//  sprintf(temp, "%0.1f", curMin);
//  tft.drawString(String(temp), tft.width()-40, 72);
} 




 int main(void)
 {	 
// 	u16 x=0,y=0,z=0;
//	u8 colorr=0;
//	u8 lcd_id[12];			//存放LCD ID字符串
//	u16 color[1]={RED};
	float dest_2d[INTERPOLATED_ROWS * INTERPOLATED_COLS];
	 
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	LED_Init();			     //LED端口初始化
	LCD_Init();  
	amg88xx_init();

	 
  while(1) 
	{		
    amg88xx_readPixels(pixels, 64);
		interpolate_image(pixels, AMG_ROWS, AMG_COLS, dest_2d, INTERPOLATED_ROWS, INTERPOLATED_COLS);
		drawpixels(dest_2d, INTERPOLATED_ROWS, INTERPOLATED_COLS, 10, 10, 0);
		LED0 = 0;		
		delay_ms(500);
		LED0 = 1;
		delay_ms(500);
	} 
}









