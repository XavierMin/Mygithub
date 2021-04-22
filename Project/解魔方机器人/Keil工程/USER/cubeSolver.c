#include "cubeSolver.h"
#include "lcd.h"
#include "touch.h"
#include "led.h"
#include "usart.h"

#define shang cubeColorU
#define xia cubeColorD
#define zuo cubeColorL
#define you cubeColorR
#define qian cubeColorF
#define hou cubeColorB


//#define Shangzhuan() TurnCube("U")
//#define Shangfan() TurnCube("U'")
//#define Xiazhuan() TurnCube("D")
//#define Xiafan() TurnCube("D'")
//#define Zouzhuan() TurnCube("L")
//#define Zoufan() TurnCube("L'")
//#define Youzhuan() TurnCube("R")
//#define Youfan() TurnCube("R'")
//#define Qianzhuan() TurnCube("F")
//#define Qianfan() TurnCube("F'")
//#define Houzhuan() TurnCube("B")
//#define Houfan() TurnCube("B'")


unsigned char cubeColorU[8]={blue,blue,blue,blue,blue,blue,blue,blue},
							cubeColorD[8]={green,green,green,green,green,green,green,green},
							cubeColorL[8]={orange,orange,orange,orange,orange,orange,orange,orange},
							cubeColorR[8]={red,red,red,red,red,red,red,red},
							cubeColorF[8]={white,white,white,white,white,white,white,white},
							cubeColorB[8]={yellow,yellow,yellow,yellow,yellow,yellow,yellow,yellow};
			  
const u16 cubeColorDraw[6]={WHITE,RED,BLUE,GREEN,BRRED,YELLOW};

unsigned char cubeColorSX[54];
unsigned char cubeColorSY[54];
unsigned char cubeColorEX[54];
unsigned char cubeColorEY[54];

unsigned char touchPointX,touchPointY;

void DrawCubeColor()
{
	LCD_Fill(50,50,100,100,cubeColorDraw[cubeColorU[0]-1]);						//第一行左边的初始化方块，对应魔方上面(U)
	LCD_Fill(100,50,150,100,cubeColorDraw[cubeColorU[1]-1]);
	LCD_Fill(150,50,200,100,cubeColorDraw[cubeColorU[2]-1]);		
	LCD_Fill(50,100,100,150,cubeColorDraw[cubeColorU[3]-1]);
	LCD_Fill(100,100,150,150,cubeColorDraw[yellow-1]);
	LCD_Fill(150,100,200,150,cubeColorDraw[cubeColorU[4]-1]);	
	LCD_Fill(50,150,100,200,cubeColorDraw[cubeColorU[5]-1]);
	LCD_Fill(100,150,150,200,cubeColorDraw[cubeColorU[6]-1]);
	LCD_Fill(150,150,200,200,cubeColorDraw[cubeColorU[7]-1]);
	LCD_DrawLine(50,50,200,50);
	LCD_DrawLine(50,100,200,100);
	LCD_DrawLine(50,150,200,150);
	LCD_DrawLine(50,200,200,200);
	LCD_DrawLine(50,50,50,200);
	LCD_DrawLine(100,50,100,200);
	LCD_DrawLine(150,50,150,200);
	LCD_DrawLine(200,50,200,200);
	LCD_ShowChar(120,112,'U',24,1);
	
	
	LCD_Fill(250,50,300,100,cubeColorDraw[cubeColorD[5]-1]);						//第一行右边的初始化方块，对应魔方下面(D)
	LCD_Fill(300,50,350,100,cubeColorDraw[cubeColorD[6]-1]);
	LCD_Fill(350,50,400,100,cubeColorDraw[cubeColorD[7]-1]);		
	LCD_Fill(250,100,300,150,cubeColorDraw[cubeColorD[3]-1]);
	LCD_Fill(300,100,350,150,cubeColorDraw[white-1]);
	LCD_Fill(350,100,400,150,cubeColorDraw[cubeColorD[4]-1]);	
	LCD_Fill(250,150,300,200,cubeColorDraw[cubeColorD[0]-1]);
	LCD_Fill(300,150,350,200,cubeColorDraw[cubeColorD[1]-1]);
	LCD_Fill(350,150,400,200,cubeColorDraw[cubeColorD[2]-1]);
	LCD_DrawLine(250,50,400,50);
	LCD_DrawLine(250,100,400,100);
	LCD_DrawLine(250,150,400,150);
	LCD_DrawLine(250,200,400,200);
	LCD_DrawLine(250,50,250,200);
	LCD_DrawLine(300,50,300,200);
	LCD_DrawLine(350,50,350,200);
	LCD_DrawLine(400,50,400,200);
	LCD_ShowChar(320,112,'D',24,1);
	
	
	LCD_Fill(50,250,100,300,cubeColorDraw[cubeColorL[2]-1]);						//第二行左边的初始化方块，对应魔方左面(L)
	LCD_Fill(100,250,150,300,cubeColorDraw[cubeColorL[1]-1]);
	LCD_Fill(150,250,200,300,cubeColorDraw[cubeColorL[0]-1]);		
	LCD_Fill(50,300,100,350,cubeColorDraw[cubeColorL[4]-1]);
	LCD_Fill(100,300,150,350,cubeColorDraw[blue-1]);
	LCD_Fill(150,300,200,350,cubeColorDraw[cubeColorL[3]-1]);	
	LCD_Fill(50,350,100,400,cubeColorDraw[cubeColorL[7]-1]);
	LCD_Fill(100,350,150,400,cubeColorDraw[cubeColorL[6]-1]);
	LCD_Fill(150,350,200,400,cubeColorDraw[cubeColorL[5]-1]);
	LCD_DrawLine(50,250,200,250);
	LCD_DrawLine(50,300,200,300);
	LCD_DrawLine(50,350,200,350);
	LCD_DrawLine(50,400,200,400);
	LCD_DrawLine(50,250,50,400);
	LCD_DrawLine(100,250,100,400);
	LCD_DrawLine(150,250,150,400);
	LCD_DrawLine(200,250,200,400);
	LCD_ShowChar(120,312,'L',24,1);
	

	LCD_Fill(250,250,300,300,cubeColorDraw[cubeColorR[0]-1]);						//第二行右边的初始化方块，对应魔方右面(R)
	LCD_Fill(300,250,350,300,cubeColorDraw[cubeColorR[1]-1]);
	LCD_Fill(350,250,400,300,cubeColorDraw[cubeColorR[2]-1]);		
	LCD_Fill(250,300,300,350,cubeColorDraw[cubeColorR[3]-1]);
	LCD_Fill(300,300,350,350,cubeColorDraw[green-1]);
	LCD_Fill(350,300,400,350,cubeColorDraw[cubeColorR[4]-1]);	
	LCD_Fill(250,350,300,400,cubeColorDraw[cubeColorR[5]-1]);
	LCD_Fill(300,350,350,400,cubeColorDraw[cubeColorR[6]-1]);
	LCD_Fill(350,350,400,400,cubeColorDraw[cubeColorR[7]-1]);
	LCD_DrawLine(250,250,400,250);
	LCD_DrawLine(250,300,400,300);
	LCD_DrawLine(250,350,400,350);
	LCD_DrawLine(250,400,400,400);
	LCD_DrawLine(250,250,250,400);
	LCD_DrawLine(300,250,300,400);
	LCD_DrawLine(350,250,350,400);
	LCD_DrawLine(400,250,400,400);
	LCD_ShowChar(320,312,'R',24,1);
	
	
	LCD_Fill(50,450,100,500,cubeColorDraw[cubeColorF[0]-1]);							//第三行左边的初始化方块，对应魔方前面(F)
	LCD_Fill(100,450,150,500,cubeColorDraw[cubeColorF[1]-1]);
	LCD_Fill(150,450,200,500,cubeColorDraw[cubeColorF[2]-1]);		
	LCD_Fill(50,500,100,550,cubeColorDraw[cubeColorF[3]-1]);
	LCD_Fill(100,500,150,550,cubeColorDraw[red-1]);
	LCD_Fill(150,500,200,550,cubeColorDraw[cubeColorF[4]-1]);	
	LCD_Fill(50,550,100,600,cubeColorDraw[cubeColorF[5]-1]);
	LCD_Fill(100,550,150,600,cubeColorDraw[cubeColorF[6]-1]);
	LCD_Fill(150,550,200,600,cubeColorDraw[cubeColorF[7]-1]);
	LCD_DrawLine(50,450,200,450);
	LCD_DrawLine(50,500,200,500);
	LCD_DrawLine(50,550,200,550);
	LCD_DrawLine(50,600,200,600);
	LCD_DrawLine(50,450,50,600);
	LCD_DrawLine(100,450,100,600);
	LCD_DrawLine(150,450,150,600);
	LCD_DrawLine(200,450,200,600);
	LCD_ShowChar(120,512,'F',24,1);
	
	
	LCD_Fill(250,450,300,500,cubeColorDraw[cubeColorB[2]-1]);							//第三行右边的初始化方块，对应魔方后面(B)
	LCD_Fill(300,450,350,500,cubeColorDraw[cubeColorB[1]-1]);
	LCD_Fill(350,450,400,500,cubeColorDraw[cubeColorB[0]-1]);		
	LCD_Fill(250,500,300,550,cubeColorDraw[cubeColorB[4]-1]);
	LCD_Fill(300,500,350,550,cubeColorDraw[orange-1]);
	LCD_Fill(350,500,400,550,cubeColorDraw[cubeColorB[3]-1]);	
	LCD_Fill(250,550,300,600,cubeColorDraw[cubeColorB[7]-1]);
	LCD_Fill(300,550,350,600,cubeColorDraw[cubeColorB[6]-1]);
	LCD_Fill(350,550,400,600,cubeColorDraw[cubeColorB[5]-1]);
	LCD_DrawLine(250,450,400,450);
	LCD_DrawLine(250,500,400,500);
	LCD_DrawLine(250,550,400,550);
	LCD_DrawLine(250,600,400,600);
	LCD_DrawLine(250,450,250,600);
	LCD_DrawLine(300,450,300,600);
	LCD_DrawLine(350,450,350,600);
	LCD_DrawLine(400,450,400,600);
	LCD_ShowChar(320,512,'B',24,1);
}

_Bool ScanTPInput(void)
{
		tp_dev.scan(0); 
		if(tp_dev.sta&TP_PRES_DOWN)	
		{	
			LED1=!LED1;
		 	if(tp_dev.x[0]<lcddev.width&&tp_dev.y[0]<lcddev.height)
			{	
				if(tp_dev.x[0]>(50)&&tp_dev.x[0]<(100)&&tp_dev.y[0]>50&&tp_dev.y[0]<100)				{touchPointX=0;touchPointY=0;}
				else if(tp_dev.x[0]>(100)&&tp_dev.x[0]<(150)&&tp_dev.y[0]>50&&tp_dev.y[0]<100)	{touchPointX=0;touchPointY=1;}
				else if(tp_dev.x[0]>(150)&&tp_dev.x[0]<(200)&&tp_dev.y[0]>50&&tp_dev.y[0]<100)	{touchPointX=0;touchPointY=2;}
				else if(tp_dev.x[0]>(50)&&tp_dev.x[0]<(100)&&tp_dev.y[0]>100&&tp_dev.y[0]<150)	{touchPointX=0;touchPointY=3;}
				else if(tp_dev.x[0]>(150)&&tp_dev.x[0]<(200)&&tp_dev.y[0]>100&&tp_dev.y[0]<150)	{touchPointX=0;touchPointY=4;}
				else if(tp_dev.x[0]>(50)&&tp_dev.x[0]<(100)&&tp_dev.y[0]>150&&tp_dev.y[0]<200)	{touchPointX=0;touchPointY=5;}
				else if(tp_dev.x[0]>(100)&&tp_dev.x[0]<(150)&&tp_dev.y[0]>150&&tp_dev.y[0]<200)	{touchPointX=0;touchPointY=6;}
				else if(tp_dev.x[0]>(150)&&tp_dev.x[0]<(200)&&tp_dev.y[0]>150&&tp_dev.y[0]<200)	{touchPointX=0;touchPointY=7;}
					
				else if(tp_dev.x[0]>(250)&&tp_dev.x[0]<(300)&&tp_dev.y[0]>50&&tp_dev.y[0]<100)	{touchPointX=1;touchPointY=5;}
				else if(tp_dev.x[0]>(300)&&tp_dev.x[0]<(350)&&tp_dev.y[0]>50&&tp_dev.y[0]<100)	{touchPointX=1;touchPointY=6;}
				else if(tp_dev.x[0]>(350)&&tp_dev.x[0]<(400)&&tp_dev.y[0]>50&&tp_dev.y[0]<100)	{touchPointX=1;touchPointY=7;}
				else if(tp_dev.x[0]>(250)&&tp_dev.x[0]<(300)&&tp_dev.y[0]>100&&tp_dev.y[0]<150)	{touchPointX=1;touchPointY=3;}
				else if(tp_dev.x[0]>(350)&&tp_dev.x[0]<(400)&&tp_dev.y[0]>100&&tp_dev.y[0]<150)	{touchPointX=1;touchPointY=4;}
				else if(tp_dev.x[0]>(250)&&tp_dev.x[0]<(300)&&tp_dev.y[0]>150&&tp_dev.y[0]<200)	{touchPointX=1;touchPointY=0;}
				else if(tp_dev.x[0]>(300)&&tp_dev.x[0]<(350)&&tp_dev.y[0]>150&&tp_dev.y[0]<200)	{touchPointX=1;touchPointY=1;}
				else if(tp_dev.x[0]>(350)&&tp_dev.x[0]<(400)&&tp_dev.y[0]>150&&tp_dev.y[0]<200)	{touchPointX=1;touchPointY=2;}
					
				else if(tp_dev.x[0]>(50)&&tp_dev.x[0]<(100)&&tp_dev.y[0]>250&&tp_dev.y[0]<300)	{touchPointX=2;touchPointY=2;}
				else if(tp_dev.x[0]>(100)&&tp_dev.x[0]<(150)&&tp_dev.y[0]>250&&tp_dev.y[0]<300)	{touchPointX=2;touchPointY=1;}
				else if(tp_dev.x[0]>(150)&&tp_dev.x[0]<(200)&&tp_dev.y[0]>250&&tp_dev.y[0]<300)	{touchPointX=2;touchPointY=0;}
				else if(tp_dev.x[0]>(50)&&tp_dev.x[0]<(100)&&tp_dev.y[0]>300&&tp_dev.y[0]<350)	{touchPointX=2;touchPointY=4;}
				else if(tp_dev.x[0]>(150)&&tp_dev.x[0]<(200)&&tp_dev.y[0]>300&&tp_dev.y[0]<350)	{touchPointX=2;touchPointY=3;}
				else if(tp_dev.x[0]>(50)&&tp_dev.x[0]<(100)&&tp_dev.y[0]>350&&tp_dev.y[0]<400)	{touchPointX=2;touchPointY=7;}
				else if(tp_dev.x[0]>(100)&&tp_dev.x[0]<(150)&&tp_dev.y[0]>350&&tp_dev.y[0]<400)	{touchPointX=2;touchPointY=6;}
				else if(tp_dev.x[0]>(150)&&tp_dev.x[0]<(200)&&tp_dev.y[0]>350&&tp_dev.y[0]<400)	{touchPointX=2;touchPointY=5;}
					
				else if(tp_dev.x[0]>(250)&&tp_dev.x[0]<(300)&&tp_dev.y[0]>250&&tp_dev.y[0]<300)	{touchPointX=3;touchPointY=0;}
				else if(tp_dev.x[0]>(300)&&tp_dev.x[0]<(350)&&tp_dev.y[0]>250&&tp_dev.y[0]<300)	{touchPointX=3;touchPointY=1;}
				else if(tp_dev.x[0]>(350)&&tp_dev.x[0]<(400)&&tp_dev.y[0]>250&&tp_dev.y[0]<300)	{touchPointX=3;touchPointY=2;}
				else if(tp_dev.x[0]>(250)&&tp_dev.x[0]<(300)&&tp_dev.y[0]>300&&tp_dev.y[0]<350)	{touchPointX=3;touchPointY=3;}
				else if(tp_dev.x[0]>(350)&&tp_dev.x[0]<(400)&&tp_dev.y[0]>300&&tp_dev.y[0]<350)	{touchPointX=3;touchPointY=4;}
				else if(tp_dev.x[0]>(250)&&tp_dev.x[0]<(300)&&tp_dev.y[0]>350&&tp_dev.y[0]<400)	{touchPointX=3;touchPointY=5;}
				else if(tp_dev.x[0]>(300)&&tp_dev.x[0]<(350)&&tp_dev.y[0]>350&&tp_dev.y[0]<400)	{touchPointX=3;touchPointY=6;}
				else if(tp_dev.x[0]>(350)&&tp_dev.x[0]<(400)&&tp_dev.y[0]>350&&tp_dev.y[0]<400)	{touchPointX=3;touchPointY=7;}
									
				else if(tp_dev.x[0]>(50)&&tp_dev.x[0]<(100)&&tp_dev.y[0]>450&&tp_dev.y[0]<500)	{touchPointX=4;touchPointY=0;}
				else if(tp_dev.x[0]>(100)&&tp_dev.x[0]<(150)&&tp_dev.y[0]>450&&tp_dev.y[0]<500)	{touchPointX=4;touchPointY=1;}
				else if(tp_dev.x[0]>(150)&&tp_dev.x[0]<(200)&&tp_dev.y[0]>450&&tp_dev.y[0]<500)	{touchPointX=4;touchPointY=2;}
				else if(tp_dev.x[0]>(50)&&tp_dev.x[0]<(100)&&tp_dev.y[0]>500&&tp_dev.y[0]<550)	{touchPointX=4;touchPointY=3;}
				else if(tp_dev.x[0]>(150)&&tp_dev.x[0]<(200)&&tp_dev.y[0]>500&&tp_dev.y[0]<550)	{touchPointX=4;touchPointY=4;}
				else if(tp_dev.x[0]>(50)&&tp_dev.x[0]<(100)&&tp_dev.y[0]>550&&tp_dev.y[0]<600)	{touchPointX=4;touchPointY=5;}
				else if(tp_dev.x[0]>(100)&&tp_dev.x[0]<(150)&&tp_dev.y[0]>550&&tp_dev.y[0]<600)	{touchPointX=4;touchPointY=6;}
				else if(tp_dev.x[0]>(150)&&tp_dev.x[0]<(200)&&tp_dev.y[0]>550&&tp_dev.y[0]<600)	{touchPointX=4;touchPointY=7;}
									
				else if(tp_dev.x[0]>(250)&&tp_dev.x[0]<(300)&&tp_dev.y[0]>450&&tp_dev.y[0]<500)	{touchPointX=5;touchPointY=2;}
				else if(tp_dev.x[0]>(300)&&tp_dev.x[0]<(350)&&tp_dev.y[0]>450&&tp_dev.y[0]<500)	{touchPointX=5;touchPointY=1;}
				else if(tp_dev.x[0]>(350)&&tp_dev.x[0]<(400)&&tp_dev.y[0]>450&&tp_dev.y[0]<500)	{touchPointX=5;touchPointY=0;}
				else if(tp_dev.x[0]>(250)&&tp_dev.x[0]<(300)&&tp_dev.y[0]>500&&tp_dev.y[0]<550)	{touchPointX=5;touchPointY=4;}
				else if(tp_dev.x[0]>(350)&&tp_dev.x[0]<(400)&&tp_dev.y[0]>500&&tp_dev.y[0]<550)	{touchPointX=5;touchPointY=3;}
				else if(tp_dev.x[0]>(250)&&tp_dev.x[0]<(300)&&tp_dev.y[0]>550&&tp_dev.y[0]<600)	{touchPointX=5;touchPointY=7;}
				else if(tp_dev.x[0]>(300)&&tp_dev.x[0]<(350)&&tp_dev.y[0]>550&&tp_dev.y[0]<600)	{touchPointX=5;touchPointY=6;}
				else if(tp_dev.x[0]>(350)&&tp_dev.x[0]<(400)&&tp_dev.y[0]>550&&tp_dev.y[0]<600)	{touchPointX=5;touchPointY=5;}
					
				else if(tp_dev.x[0]>(100)&&tp_dev.x[0]<(150)&&tp_dev.y[0]>100&&tp_dev.y[0]<150)				//点中黄色U
				{
					switch (touchPointX)
					{
						case 0:cubeColorU[touchPointY]=yellow;break;
						case 1:cubeColorD[touchPointY]=yellow;break;
						case 2:cubeColorL[touchPointY]=yellow;break;
						case 3:cubeColorR[touchPointY]=yellow;break;
						case 4:cubeColorF[touchPointY]=yellow;break;
						case 5:cubeColorB[touchPointY]=yellow;break;						
					}
				}
				else if(tp_dev.x[0]>(300)&&tp_dev.x[0]<(350)&&tp_dev.y[0]>100&&tp_dev.y[0]<150)				//点中白色D
				{
					switch (touchPointX)
					{
						case 0:cubeColorU[touchPointY]=white;break;
						case 1:cubeColorD[touchPointY]=white;break;
						case 2:cubeColorL[touchPointY]=white;break;
						case 3:cubeColorR[touchPointY]=white;break;
						case 4:cubeColorF[touchPointY]=white;break;
						case 5:cubeColorB[touchPointY]=white;break;						
					}
				}
				else if(tp_dev.x[0]>(100)&&tp_dev.x[0]<(150)&&tp_dev.y[0]>300&&tp_dev.y[0]<350)				//点中蓝色L
				{
					switch (touchPointX)
					{
						case 0:cubeColorU[touchPointY]=blue;break;
						case 1:cubeColorD[touchPointY]=blue;break;
						case 2:cubeColorL[touchPointY]=blue;break;
						case 3:cubeColorR[touchPointY]=blue;break;
						case 4:cubeColorF[touchPointY]=blue;break;
						case 5:cubeColorB[touchPointY]=blue;break;						
					}
				}
				else if(tp_dev.x[0]>(300)&&tp_dev.x[0]<(350)&&tp_dev.y[0]>300&&tp_dev.y[0]<350)				//点中绿色R
				{
					switch (touchPointX)
					{
						case 0:cubeColorU[touchPointY]=green;break;
						case 1:cubeColorD[touchPointY]=green;break;
						case 2:cubeColorL[touchPointY]=green;break;
						case 3:cubeColorR[touchPointY]=green;break;
						case 4:cubeColorF[touchPointY]=green;break;
						case 5:cubeColorB[touchPointY]=green;break;						
					}
				}
				else if(tp_dev.x[0]>(100)&&tp_dev.x[0]<(150)&&tp_dev.y[0]>500&&tp_dev.y[0]<550)				//点中红色F
				{
					switch (touchPointX)
					{
						case 0:cubeColorU[touchPointY]=red;break;
						case 1:cubeColorD[touchPointY]=red;break;
						case 2:cubeColorL[touchPointY]=red;break;
						case 3:cubeColorR[touchPointY]=red;break;
						case 4:cubeColorF[touchPointY]=red;break;
						case 5:cubeColorB[touchPointY]=red;break;						
					}
				}
				else if(tp_dev.x[0]>(300)&&tp_dev.x[0]<(350)&&tp_dev.y[0]>500&&tp_dev.y[0]<550)				//点中橙色B
				{
					switch (touchPointX)
					{
						case 0:cubeColorU[touchPointY]=orange;break;
						case 1:cubeColorD[touchPointY]=orange;break;
						case 2:cubeColorL[touchPointY]=orange;break;
						case 3:cubeColorR[touchPointY]=orange;break;
						case 4:cubeColorF[touchPointY]=orange;break;
						case 5:cubeColorB[touchPointY]=orange;break;						
					}
				}
				return 1;					
			}
			return 0;
		}
		else 
			return 0;
}

_Bool CheckInput(void)					//检测输入颜色是否完整，即检测六种颜色是否均为8个（输入的小方块，中心块不算）
{
	u8 yanse1=0,yanse2=0,yanse3=0,yanse4=0,yanse5=0,yanse6=0,i;
		  for(i=0;i<8;i++)
			{
				if(shang[i]==1)yanse1++;
				if(shang[i]==2)yanse2++;
				if(shang[i]==3)yanse3++;
				if(shang[i]==4)yanse4++;
				if(shang[i]==5)yanse5++;
				if(shang[i]==6)yanse6++;
			}
			for(i=0;i<8;i++)
			{
				if(xia[i]==1)yanse1++;
				if(xia[i]==2)yanse2++;
				if(xia[i]==3)yanse3++;
				if(xia[i]==4)yanse4++;
				if(xia[i]==5)yanse5++;
				if(xia[i]==6)yanse6++;
			}
			for(i=0;i<8;i++)
			{
				if(zuo[i]==1)yanse1++;
				if(zuo[i]==2)yanse2++;
				if(zuo[i]==3)yanse3++;
				if(zuo[i]==4)yanse4++;
				if(zuo[i]==5)yanse5++;
				if(zuo[i]==6)yanse6++;
			}
			for(i=0;i<8;i++)
			{
				if(you[i]==1)yanse1++;
				if(you[i]==2)yanse2++;
				if(you[i]==3)yanse3++;
				if(you[i]==4)yanse4++;
				if(you[i]==5)yanse5++;
				if(you[i]==6)yanse6++;
			}
			for(i=0;i<8;i++)
			{
				if(qian[i]==1)yanse1++;
				if(qian[i]==2)yanse2++;
				if(qian[i]==3)yanse3++;
				if(qian[i]==4)yanse4++;
				if(qian[i]==5)yanse5++;
				if(qian[i]==6)yanse6++;
			}			
			for(i=0;i<8;i++)
			{
				if(hou[i]==1)yanse1++;
				if(hou[i]==2)yanse2++;
				if(hou[i]==3)yanse3++;
				if(hou[i]==4)yanse4++;
				if(hou[i]==5)yanse5++;
				if(hou[i]==6)yanse6++;
			}	
			
			if(yanse1==8&&yanse2==8&&yanse3==8&&yanse4==8&&yanse5==8&&yanse6==8)
				return 1;
			else
				return 0;
}


void TurnCube(char * in)
{
	unsigned int i;
	for(i=0;in[i]!='\0';i++)
	{
		USART_SendData(USART1, in[i]);         
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
		USART_SendData(USART2,in[i]);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
	}
	while(1)
	{
		if(USART2_RX_STA&0x8000)
		{					   
			USART2_RX_STA=0;			
			if(USART2_RX_BUF[0]=='O')
			{
				if(USART2_RX_BUF[1]=='K')
				{
					break;
				}
			}
		}
		else if(USART_RX_STA&0x8000)
		{					   
			USART_RX_STA=0;			
			if(USART_RX_BUF[0]=='O')
			{
				if(USART_RX_BUF[1]=='K')
				{
					break;
				}
			}
		}
	}
}

void Shangzhuan(void)
{
	char t;
	
	t=shang[2];
	shang[2]=shang[0];
	shang[0]=shang[5];
	shang[5]=shang[7];
	shang[7]=t;
	
	t=shang[1];
	shang[1]=shang[3];
	shang[3]=shang[6];
	shang[6]=shang[4];
	shang[4]=t;
	
	t=qian[0];
	qian[0]=you[0];
	you[0]=hou[2];
	hou[2]=zuo[2];
	zuo[2]=t;
	
	t=qian[1];
	qian[1]=you[1];
	you[1]=hou[1];
	hou[1]=zuo[1];
	zuo[1]=t;
	
	t=qian[2];
	qian[2]=you[2];
	you[2]=hou[0];
	hou[0]=zuo[0];
	zuo[0]=t;
	TurnCube("U");
	DrawCubeColor();
}
void Shangfan(void)
{
	char t;
	
	t=shang[7];
	shang[7]=shang[5];
	shang[5]=shang[0];
	shang[0]=shang[2];
	shang[2]=t;
	
	t=shang[4];
	shang[4]=shang[6];
	shang[6]=shang[3];
	shang[3]=shang[1];
	shang[1]=t;
	
	t=zuo[2];
	zuo[2]=hou[2];
	hou[2]=you[0];
	you[0]=qian[0];
	qian[0]=t;
	
	t=zuo[1];
	zuo[1]=hou[1];
	hou[1]=you[1];
	you[1]=qian[1];
	qian[1]=t;
	
	t=zuo[0];
	zuo[0]=hou[0];
	hou[0]=you[2];
	you[2]=qian[2];
	qian[2]=t;
	TurnCube("U'");
	DrawCubeColor();
}
void Zouzhuan(void)
{
	int t;
	
	t=zuo[7];
	zuo[7]=zuo[5];
	zuo[5]=zuo[0];
	zuo[0]=zuo[2];
	zuo[2]=t;

	t=zuo[4];
	zuo[4]=zuo[6];
	zuo[6]=zuo[3];
	zuo[3]=zuo[1];
	zuo[1]=t;

	t=shang[3];
	shang[3]=hou[3];
	hou[3]=xia[3];
	xia[3]=qian[3];
	qian[3]=t;
	
	t=shang[0];
	shang[0]=hou[5];
	hou[5]=xia[5];
	xia[5]=qian[0];
	qian[0]=t;
	
	t=shang[5];
	shang[5]=hou[0];
	hou[0]=xia[0];
	xia[0]=qian[5];
	qian[5]=t;
	TurnCube("L");
	DrawCubeColor();
}
void Zoufan(void)
{
	int t;
	
	t=zuo[2];
	zuo[2]=zuo[0];
	zuo[0]=zuo[5];
	zuo[5]=zuo[7];
	zuo[7]=t;

	t=zuo[1];
	zuo[1]=zuo[3];
	zuo[3]=zuo[6];
	zuo[6]=zuo[4];
	zuo[4]=t;

	t=qian[3];
	qian[3]=xia[3];
	xia[3]=hou[3];
	hou[3]=shang[3];
	shang[3]=t;
	
	t=qian[0];
	qian[0]=xia[5];
	xia[5]=hou[5];
	hou[5]=shang[0];
	shang[0]=t;
	
	t=qian[5];
	qian[5]=xia[0];
	xia[0]=hou[0];
	hou[0]=shang[5];
	shang[5]=t;
	TurnCube("L'");
	DrawCubeColor();
}
void Youzhuan(void)
{
	int t;
	
	t=you[2];
	you[2]=you[0];
	you[0]=you[5];
	you[5]=you[7];
	you[7]=t;
	
	t=you[1];
	you[1]=you[3];
	you[3]=you[6];
	you[6]=you[4];
	you[4]=t;
	
	t=shang[4];
	shang[4]=qian[4];
	qian[4]=xia[4];
	xia[4]=hou[4];
	hou[4]=t;
	
	t=shang[2];
	shang[2]=qian[2];
	qian[2]=xia[7];
	xia[7]=hou[7];
	hou[7]=t;
	
	t=shang[7];
	shang[7]=qian[7];
	qian[7]=xia[2];
	xia[2]=hou[2];
	hou[2]=t;
	TurnCube("R");
	DrawCubeColor();
}
void Youfan(void)
{
	int t;
	
	t=you[7];
	you[7]=you[5];
	you[5]=you[0];
	you[0]=you[2];
	you[2]=t;
	
	t=you[4];
	you[4]=you[6];
	you[6]=you[3];
	you[3]=you[1];
	you[1]=t;
	
	t=hou[4];
	hou[4]=xia[4];
	xia[4]=qian[4];
	qian[4]=shang[4];
	shang[4]=t;
	
	t=hou[7];
	hou[7]=xia[7];
	xia[7]=qian[2];
	qian[2]=shang[2];
	shang[2]=t;
	
	t=hou[2];
	hou[2]=xia[2];
	xia[2]=qian[7];
	qian[7]=shang[7];
	shang[7]=t;
	TurnCube("R'");
	DrawCubeColor();
}
void Qianzhuan(void)
{
	int t;
	
	t=qian[2];
	qian[2]=qian[0];
	qian[0]=qian[5];
	qian[5]=qian[7];
	qian[7]=t;
	
	t=qian[1];
	qian[1]=qian[3];
	qian[3]=qian[6];
	qian[6]=qian[4];
	qian[4]=t;
	
	t=shang[5];
	shang[5]=zuo[5];
	zuo[5]=xia[7];
	xia[7]=you[0];
	you[0]=t;
	
	t=shang[6];
	shang[6]=zuo[3];
	zuo[3]=xia[6];
	xia[6]=you[3];
	you[3]=t;
	
	t=shang[7];
	shang[7]=zuo[0];
	zuo[0]=xia[5];
	xia[5]=you[5];
	you[5]=t;
	TurnCube("F");
	DrawCubeColor();
}
void Qianfan(void)
{
	int t;
	
	t=qian[7];
	qian[7]=qian[5];
	qian[5]=qian[0];
	qian[0]=qian[2];
	qian[2]=t;
	
	t=qian[4];
	qian[4]=qian[6];
	qian[6]=qian[3];
	qian[3]=qian[1];
	qian[1]=t;
	
	t=you[0];
	you[0]=xia[7];
	xia[7]=zuo[5];
	zuo[5]=shang[5];
	shang[5]=t;
	
	t=you[3];
	you[3]=xia[6];
	xia[6]=zuo[3];
	zuo[3]=shang[6];
	shang[6]=t;
	
	t=you[5];
	you[5]=xia[5];
	xia[5]=zuo[0];
	zuo[0]=shang[7];
	shang[7]=t;
	TurnCube("F'");
	DrawCubeColor();
}
void Houzhuan(void)
{
	int t;
	
	t=hou[7];
	hou[7]=hou[5];
	hou[5]=hou[0];
	hou[0]=hou[2];
	hou[2]=t;
	
	t=hou[4];
	hou[4]=hou[6];
	hou[6]=hou[3];
	hou[3]=hou[1];
	hou[1]=t;
	
	t=shang[0];
	shang[0]=you[2];
	you[2]=xia[2];
	xia[2]=zuo[7];
	zuo[7]=t;
	
	t=shang[1];
	shang[1]=you[4];
	you[4]=xia[1];
	xia[1]=zuo[4];
	zuo[4]=t;
	
	t=shang[2];
	shang[2]=you[7];
	you[7]=xia[0];
	xia[0]=zuo[2];
	zuo[2]=t;
	TurnCube("B");
	DrawCubeColor();
}
void Houfan(void)
{
	int t;
	
	t=hou[2];
	hou[2]=hou[0];
	hou[0]=hou[5];
	hou[5]=hou[7];
	hou[7]=t;
	
	t=hou[1];
	hou[1]=hou[3];
	hou[3]=hou[6];
	hou[6]=hou[4];
	hou[4]=t;
	
	t=zuo[7];
	zuo[7]=xia[2];
	xia[2]=you[2];
	you[2]=shang[0];
	shang[0]=t;
	
	t=zuo[4];
	zuo[4]=xia[1];
	xia[1]=you[4];
	you[4]=shang[1];
	shang[1]=t;
	
	t=zuo[2];
	zuo[2]=xia[0];
	xia[0]=you[7];
	you[7]=shang[2];
	shang[2]=t;
	TurnCube("B'");
	DrawCubeColor();
}


void Yiceng(void)
{
	while(!((cubeColorD[6]==1)&&(cubeColorF[6]==2)))  
	{
		if(( (cubeColorU[3]==1) && ( cubeColorL[1]==2 ) )|| ( (cubeColorB[1]==1) && (cubeColorU[1]==2 ) ) )
			Shangfan();
		if(( (cubeColorF[4]==1) && (cubeColorR[3]==2 ) )|| ( (cubeColorL[3]==1) && (cubeColorF[3]==2 ) ) )
			Qianfan();
		if(( (cubeColorL[6]==1) && (cubeColorD[3]==2 ) ))
			Zoufan();
		if( ( (cubeColorR[1]==1) && (cubeColorU[4]==2 ) ))
			Youfan();
		if( ( (cubeColorU[1]==1) && (cubeColorB[1]==2) ) || ( (cubeColorU[4]==1) && ( cubeColorR[1]==2 ) ) || ( (cubeColorF[1]==1) && (cubeColorU[6]==2) ) )
	    Shangzhuan();
		if( ( (cubeColorF[3]==1) && (cubeColorL[3]==2 ) ) || ( (cubeColorF[6]==1) && (cubeColorD[6]==2 ) ) || ( (cubeColorR[3]==1) && (cubeColorF[4]==2 ) ) || ( (cubeColorU[6]==1) && (cubeColorF[1]==2) ) )
			Qianzhuan();
		if( ( (cubeColorB[3]==1) && (cubeColorL[4]==2 ) ) || ( (cubeColorL[1]==1) && (cubeColorU[3]==2 ) ) || ( (cubeColorL[4]==1) && (cubeColorB[3]==2 ) ) || ( (cubeColorD[3]==1)&&(cubeColorL[6]==2) ) )
			Zouzhuan();
		if(( (cubeColorR[6]==1) && (cubeColorD[4]==2 ) ) || ( (cubeColorB[4]==1) && (cubeColorR[4]==2 ) )  || ( (cubeColorR[4]==1) && (cubeColorB[4]==2 ) ) || ( (cubeColorD[4]==1)&&(cubeColorR[6]==2) ) )
			Youzhuan();
		if( ( (cubeColorB[6]==1) && (cubeColorD[1]==2 ) ) || ( (cubeColorD[1]==1)&&(cubeColorB[6]==2) )  )
			Houzhuan();
	}
	while(!((cubeColorD[3]==1)&&(cubeColorL[6]==3)))  
	{
		if( ( (cubeColorB[6]==1) && (cubeColorD[1]==3 ) ) || ( (cubeColorL[4]==1) && (cubeColorB[3]==3 ) )  )
			Houfan();
		if(( (cubeColorL[3]==1) && (cubeColorF[3]==3) )|| ( (cubeColorB[3]==1) && (cubeColorL[4]==3 ) ))
			Zoufan();
		if( ( (cubeColorU[1]==1) && (cubeColorB[1]==3) )|| ( (cubeColorR[1]==1) && (cubeColorU[4]==3 ) ) )
			Shangfan();
		if( ( (cubeColorL[1]==1) && (cubeColorU[3]==3 ) ) || ( (cubeColorU[4]==1) && ( cubeColorR[1]==3 ) ) || ( (cubeColorF[1]==1) && (cubeColorU[6]==3) ) || ( (cubeColorU[6]==1) && (cubeColorF[1]==3) )  )
	    Shangzhuan();
		if( ( (cubeColorF[3]==1) && (cubeColorL[3]==3 ) ) || ( (cubeColorL[6]==1) && (cubeColorD[3]==3 ) ) || ( (cubeColorU[3]==1) && ( cubeColorL[1]==3 ) ) )
			Zouzhuan();
		if( ( (cubeColorF[4]==1) && (cubeColorR[3]==3 ) ) || ( (cubeColorR[6]==1) && (cubeColorD[4]==3 ) ) || ( (cubeColorR[3]==1) && (cubeColorF[4]==3 ) )  || ( (cubeColorD[4]==1)&&(cubeColorR[6]==3) ) )
			Youzhuan();
		if(( (cubeColorR[4]==1) && (cubeColorB[4]==3 ) ) || ( (cubeColorB[4]==1) && (cubeColorR[4]==3 ) ) || ( (cubeColorB[1]==1) && (cubeColorU[1]==3 ) ) || ( (cubeColorD[1]==1)&&(cubeColorB[6]==3) ) )
			Houzhuan();
	}
	while(!((cubeColorD[4]==1)&&(cubeColorR[6]==4))) 
	{
		if(( (cubeColorU[6]==1) && (cubeColorF[1]==4) )  || ( (cubeColorR[1]==1) && (cubeColorU[4]==4 ) ) )
			Shangfan();
		if(( (cubeColorF[4]==1) && (cubeColorR[3]==4 ) ) || ( (cubeColorR[6]==1) && (cubeColorD[4]==4 ) ))
			Youfan();
		if(( (cubeColorL[4]==1) && (cubeColorB[3]==4 ) )  || ( (cubeColorB[1]==1) && (cubeColorU[1]==4 ) ))
			Houfan();
		if( ( (cubeColorL[1]==1) && (cubeColorU[3]==4 ) )|| ( (cubeColorU[1]==1) && (cubeColorB[1]==4) ) || ( (cubeColorU[3]==1) && ( cubeColorL[1]==4 ) ) || ( (cubeColorF[1]==1) && (cubeColorU[6]==4) )  )
	    Shangzhuan();
		if( ( (cubeColorU[4]==1) && ( cubeColorR[1]==4 ) ) || ( (cubeColorB[4]==1) && (cubeColorR[4]==4 ) )  || ( (cubeColorR[3]==1) && (cubeColorF[4]==4 ) ) )
			Youzhuan();
		if( ( (cubeColorB[3]==1) && (cubeColorL[4]==4 ) ) || ( (cubeColorB[6]==1) && (cubeColorD[1]==4 ) ) || ( (cubeColorR[4]==1) && (cubeColorB[4]==4 ) ) || ( (cubeColorD[1]==1)&&(cubeColorB[6]==4) ) )
			Houzhuan();
		if( ((cubeColorF[3]==1) && (cubeColorL[3]==4 )) || ( (cubeColorL[3]==1) && (cubeColorF[3]==4) ) )
		{
			Zoufan();Shangzhuan();Zouzhuan();
		}
	}
	while(!((cubeColorD[1]==1)&&(cubeColorB[6]==5))) 
	{
		if( ( (cubeColorL[1]==1) && (cubeColorU[3]==5 ) ) || ( (cubeColorU[4]==1) && ( cubeColorR[1]==5 ) ) || ( (cubeColorB[1]==1) && (cubeColorU[1]==5 ) ) || ( (cubeColorU[3]==1) && ( cubeColorL[1]==5 ) ) || ( (cubeColorF[1]==1) && (cubeColorU[6]==5) ) || ( (cubeColorU[6]==1) && (cubeColorF[1]==5) )  )
	    Shangzhuan();
		if( ( (cubeColorL[4]==1) && (cubeColorB[3]==5 ) ) || ( (cubeColorR[4]==1) && (cubeColorB[4]==5 ) ) || ( (cubeColorB[4]==1) && (cubeColorR[4]==5 ) ) || ((cubeColorB[3]==1) && (cubeColorL[4]==5 ))||( (cubeColorU[1]==1) && (cubeColorB[1]==5) ) || ( (cubeColorB[6]==1) && (cubeColorD[1]==5 ) ) )
			Houzhuan();
		if(((cubeColorF[4]==1) && (cubeColorR[3]==5 ) ) ||((cubeColorR[3]==1) && (cubeColorF[4]==5 )) )
		{
			Youzhuan();Shangzhuan();Youzhuan();Youzhuan();Youzhuan();		
		}
		if( ( (cubeColorL[3]==1) && (cubeColorF[3]==5) ) || ((cubeColorF[3]==1) && (cubeColorL[3]==5 )))
		{
			Zouzhuan();Zouzhuan();Zouzhuan();Shangzhuan();Zouzhuan();			
		}
		if( (cubeColorR[1]==1) && (cubeColorU[4]==5 ) )
		{
			Youzhuan();Houzhuan();Youzhuan();Youzhuan();Youzhuan();
		}
	}
	while(!((cubeColorD[7]==1)&&(cubeColorF[7]==2)&&(cubeColorR[5]==4)))   
	{
		if( ( (cubeColorU[0]==1)&&(cubeColorL[2]==2)&&(cubeColorB[0]==4) ) || ( (cubeColorL[0]==1)&&(cubeColorU[5]==2)&&(cubeColorF[0]==4) ) || ( (cubeColorL[2]==1)&&(cubeColorB[0]==2)&&(cubeColorU[0]==4) ) || ( (cubeColorF[0]==1)&&(cubeColorL[0]==2)&&(cubeColorU[5]==4) ) )
			Shangfan();
		if( ( (cubeColorB[0]==1)&&(cubeColorU[0]==2)&&(cubeColorL[2]==4) ) || ( (cubeColorB[2]==1)&&(cubeColorR[2]==2)&&(cubeColorU[2]==4) ) || ( (cubeColorR[2]==1)&&(cubeColorU[2]==2)&&(cubeColorB[2]==4)) || ( (cubeColorU[7]==1)&&(cubeColorR[0]==2)&&(cubeColorF[2]==4) ))
      Shangzhuan();
		if(( (cubeColorU[2]==1)&&(cubeColorB[2]==2)&&(cubeColorR[2]==4) ) )
		{
			Houzhuan();Qianfan();Shangzhuan();Shangzhuan();Houfan();Qianzhuan();
		}
		if(( (cubeColorU[5]==1)&&(cubeColorF[0]==2)&&(cubeColorL[0]==4) ) )
		{
			Youzhuan();Zoufan();Shangzhuan();Shangzhuan();Youfan();Zouzhuan();
		}
		if( ( (cubeColorR[0]==1)&&(cubeColorF[2]==2)&&(cubeColorU[7]==4) ) || ( (cubeColorR[5]==1)&&(cubeColorD[7]==2)&&(cubeColorF[7]==4) ) )   //***
		{
			Youzhuan();Shangzhuan();Youfan();
		}
		if(( (cubeColorF[2]==1)&&(cubeColorU[7]==2)&&(cubeColorR[0]==4) ) || ( (cubeColorF[7]==1)&&(cubeColorR[5]==2)&&(cubeColorD[7]==4) ))   //***
		{
			Qianfan();Shangfan();Qianzhuan();
		}
		if( ( (cubeColorF[5]==1)&&(cubeColorD[5]==2)&&(cubeColorL[5]==4) ))
		{
			Zoufan();Shangzhuan();Zouzhuan();
		}
		if(( (cubeColorL[5]==1)&&(cubeColorF[5]==2)&&(cubeColorD[5]==4) )  || ( (cubeColorD[5]==1)&&(cubeColorL[5]==2)&&(cubeColorF[5]==4) ))
		{
			Zoufan();Shangfan();Zouzhuan();
		}
		if(( (cubeColorL[7]==1)&&(cubeColorD[0]==2)&&(cubeColorB[5]==4) ))
		{
			Houfan();Shangzhuan();Houzhuan();
		}
		if(( (cubeColorB[5]==1)&&(cubeColorL[7]==2)&&(cubeColorD[0]==4) )  || ( (cubeColorD[0]==1)&&(cubeColorB[5]==2)&&(cubeColorL[7]==4) ) )
		{
			Houfan();Shangfan();Houzhuan();
		}
		if( ( (cubeColorB[7]==1)&&(cubeColorD[2]==2)&&(cubeColorR[7]==4) ) || ( (cubeColorD[2]==1)&&(cubeColorR[7]==2)&&(cubeColorB[7]==4) ) )
		{
			Houzhuan();Shangzhuan();Houfan();
		}
		if(( (cubeColorR[7]==1)&&(cubeColorB[7]==2)&&(cubeColorD[2]==4) ) )
		{
			Houzhuan();Shangfan();Houfan();
		}
	}
	while(!((cubeColorD[5]==1)&&(cubeColorF[5]==2)&&(cubeColorL[5]==3)))   
	{
		if(( (cubeColorU[7]==1)&&(cubeColorF[2]==2)&&(cubeColorR[0]==3) )  || ( (cubeColorU[2]==1)&&(cubeColorR[2]==2)&&(cubeColorB[2]==3) ) || ( (cubeColorB[0]==1)&&(cubeColorL[2]==2)&&(cubeColorU[0]==3) )|| ( (cubeColorL[2]==1)&&(cubeColorU[0]==2)&&(cubeColorB[0]==3) ) )
			Shangfan();
		if( ( (cubeColorU[5]==1)&&(cubeColorL[0]==2)&&(cubeColorF[0]==3) ) ||( (cubeColorB[2]==1)&&(cubeColorU[2]==2)&&(cubeColorR[2]==3) )  || ( (cubeColorR[2]==1)&&(cubeColorB[2]==2)&&(cubeColorU[2]==3) ) || ( (cubeColorR[0]==1)&&(cubeColorU[7]==2)&&(cubeColorF[2]==3) )|| ( (cubeColorF[2]==1)&&(cubeColorR[0]==2)&&(cubeColorU[7]==3) )  )
      Shangzhuan();
		if(( (cubeColorU[0]==1)&&(cubeColorB[0]==2)&&(cubeColorL[2]==3) ))
		{
			Houfan();Qianzhuan();Shangzhuan();Shangzhuan();Houzhuan();Qianfan();
		}
		if(  ( (cubeColorF[5]==1)&&(cubeColorL[5]==2)&&(cubeColorD[5]==3) ) )
		{
			Zoufan();Shangzhuan();Zouzhuan();
		}
		if( ( (cubeColorF[0]==1)&&(cubeColorU[5]==2)&&(cubeColorL[0]==3) ) )     
		{
			Qianzhuan();Shangzhuan();Qianfan();
		}
		if( ( (cubeColorL[0]==1)&&(cubeColorF[0]==2)&&(cubeColorU[5]==3) ) || ( (cubeColorL[5]==1)&&(cubeColorD[5]==2)&&(cubeColorF[5]==3) ) )  //****
		{
			Zoufan();Shangfan();Zouzhuan();
		}
		if( ( (cubeColorL[7]==1)&&(cubeColorB[5]==2)&&(cubeColorD[0]==3) )  )
		{
			Houfan();Shangzhuan();Houzhuan();
		}
		if(( (cubeColorB[5]==1)&&(cubeColorD[0]==2)&&(cubeColorL[7]==3) )|| ( (cubeColorD[0]==1)&&(cubeColorL[7]==2)&&(cubeColorB[5]==3) ))
		{
			Houfan();Shangfan();Houzhuan();
		}
		if( ( (cubeColorB[7]==1)&&(cubeColorR[7]==2)&&(cubeColorD[2]==3) ) || ( (cubeColorD[2]==1)&&(cubeColorB[7]==2)&&(cubeColorR[7]==3) ) )
		{
			Houzhuan();Shangzhuan();Shangzhuan();Houfan();
		}
		if(( (cubeColorR[7]==1)&&(cubeColorD[2]==2)&&(cubeColorB[7]==3) ) )
		{
			Houzhuan();Shangfan();Houfan();
		}
	}
	while(!((cubeColorD[0]==1)&&(cubeColorB[5]==5)&&(cubeColorL[7]==3)))   
	{
		if( ( (cubeColorU[5]==1)&&(cubeColorF[0]==5)&&(cubeColorL[0]==3) )||( (cubeColorB[2]==1)&&(cubeColorR[2]==5)&&(cubeColorU[2]==3) )  || ( (cubeColorU[7]==1)&&(cubeColorR[0]==5)&&(cubeColorF[2]==3) ) || ( (cubeColorR[2]==1)&&(cubeColorU[2]==5)&&(cubeColorB[2]==3) ) || ( (cubeColorR[0]==1)&&(cubeColorF[2]==5)&&(cubeColorU[7]==3) ))
			Shangfan();
		if( ( (cubeColorU[0]==1)&&(cubeColorL[2]==5)&&(cubeColorB[0]==3) ) || ( (cubeColorL[0]==1)&&(cubeColorU[5]==5)&&(cubeColorF[0]==3) ) || ( (cubeColorF[2]==1)&&(cubeColorU[7]==5)&&(cubeColorR[0]==3) ) || ( (cubeColorF[0]==1)&&(cubeColorL[0]==5)&&(cubeColorU[5]==3) ))
			Shangzhuan();
		if(( (cubeColorU[2]==1)&&(cubeColorB[2]==5)&&(cubeColorR[2]==3) ))
		{
			Zouzhuan();Youfan();Shangzhuan();Shangzhuan();Zoufan();Youzhuan();
		}
		if(( (cubeColorL[2]==1)&&(cubeColorB[0]==5)&&(cubeColorU[0]==3) ) )  
		{
			Zouzhuan();Shangzhuan();Zoufan();
		}
		if(( (cubeColorB[0]==1)&&(cubeColorU[0]==5)&&(cubeColorL[2]==3) ))
		{
			Houfan();Shangfan();Houzhuan();			
		}
		if( ( (cubeColorL[7]==1)&&(cubeColorD[0]==5)&&(cubeColorB[5]==3) ) )
		{
			Houfan();Shangzhuan();Houzhuan();
		}
		if( ( (cubeColorB[5]==1)&&(cubeColorL[7]==5)&&(cubeColorD[0]==3) ) )
		{
			Zouzhuan();Shangfan();Zoufan();
		}
		if( ( (cubeColorB[7]==1)&&(cubeColorD[2]==5)&&(cubeColorR[7]==3) ) )
		{
			Youfan();Shangzhuan();Youzhuan();
		}
		if(((cubeColorR[7]==1)&&(cubeColorB[7]==5)&&(cubeColorD[2]==3) ) || ( (cubeColorD[2]==1)&&(cubeColorR[7]==5)&&(cubeColorB[7]==3) ) )
		{
			Houzhuan();Shangfan();Houfan();	
		}
	}		
	while(!((cubeColorD[2]==1)&&(cubeColorB[7]==5)&&(cubeColorR[7]==4)))   	
	{
		if(( (cubeColorU[7]==1)&&(cubeColorF[2]==5)&&(cubeColorR[0]==4) ) || ( (cubeColorR[0]==1)&&(cubeColorU[7]==5)&&(cubeColorF[2]==4) ) || ( (cubeColorU[5]==1)&&(cubeColorL[0]==5)&&(cubeColorF[0]==4) ) || ( (cubeColorF[2]==1)&&(cubeColorR[0]==5)&&(cubeColorU[7]==4) ) || ( (cubeColorF[0]==1)&&(cubeColorU[5]==5)&&(cubeColorL[0]==4) ) )
			Shangfan();
		if( ( (cubeColorB[0]==1)&&(cubeColorL[2]==5)&&(cubeColorU[0]==4) ) || ( (cubeColorL[0]==1)&&(cubeColorF[0]==5)&&(cubeColorU[5]==4) ) || ( (cubeColorL[2]==1)&&(cubeColorU[0]==5)&&(cubeColorB[0]==4) ) || ( (cubeColorU[0]==1)&&(cubeColorB[0]==5)&&(cubeColorL[2]==4) ) )
			Shangzhuan();
		if(( (cubeColorU[2]==1)&&(cubeColorR[2]==5)&&(cubeColorB[2]==4) ) )
		{
			Houzhuan();Shangzhuan();Shangzhuan();Houfan();
		}
		if(( (cubeColorR[7]==1)&&(cubeColorD[2]==5)&&(cubeColorB[7]==4) ))
		{
			Houzhuan();Shangzhuan();Houfan();
		}
		if(( (cubeColorR[2]==1)&&(cubeColorB[2]==5)&&(cubeColorU[2]==4) ))
		{
			Youfan();Shangfan();Youzhuan();
		}
		if(( (cubeColorB[7]==1)&&(cubeColorR[7]==5)&&(cubeColorD[2]==4) ))
		{
			Youfan();Shangzhuan();Youzhuan();
		}
		if(( (cubeColorB[2]==1)&&(cubeColorU[2]==5)&&(cubeColorR[2]==4) ))
		{
			Houzhuan();Shangzhuan();Houfan();
		}
	}
}

void zuoqian(void)
{
	Qianzhuan();Shangfan();Qianfan();Shangfan();Zoufan();Shangzhuan();Zouzhuan();
}
void zuohou(void)
{
	Houfan();Shangzhuan();Houzhuan();Shangzhuan();Zouzhuan();Shangfan();Zoufan();
}
void qianyou(void)
{
	Youzhuan();Shangfan();Youfan();Shangfan();Qianfan();Shangzhuan();Qianzhuan();
}
void qianzuo(void)
{
	Zoufan();Shangzhuan();Zouzhuan();Shangzhuan();Qianzhuan();Shangfan();Qianfan();
}
void youqian(void)
{
	Qianfan();Shangzhuan();Qianzhuan();Shangzhuan();Youzhuan();Shangfan();Youfan();
}
void youhou(void)
{
	Houzhuan();Shangfan();Houfan();Shangfan();Youfan();Shangzhuan();Youzhuan();
}
void houzuo(void)
{
	Zouzhuan();Shangfan();Zoufan();Shangfan();Houfan();Shangzhuan();Houzhuan();
}
void houyou(void)
{
	Youfan();Shangzhuan();Youzhuan();Shangzhuan();Houzhuan();Shangfan();Houfan();
}

void Erceng(void)
{
	while(!((qian[3]==2)&&(zuo[3]==3)))  
	{
		if(((shang[4]==2)&&(you[1]==3))  || ((qian[1]==2)&&(shang[6]==3)))
			Shangfan();
		if( ((hou[1]==2)&&(shang[1]==3)) || ((zuo[1]==2)&&(shang[3]==3)) || ((shang[3]==2)&&(zuo[1]==3)) || ((shang[6]==2)&&(qian[1]==3)) )
			Shangzhuan();
		if(( (shang[1]==2)&&(hou[1]==3) ) || ((zuo[3]==2)&&(qian[3]==3)) )
			zuoqian();
		if(((qian[4]==2)&&(you[3]==3)) || ((you[3]==2)&&(qian[4]==3)) )
			qianyou();
		if(((zuo[4]==2)&&(hou[3]==3)) || ((hou[3]==2)&&(zuo[4]==3)) )
			zuohou();
		if(((you[1]==2)&&(shang[4]==3)))
			qianzuo();
		if(((you[4]==2)&&(hou[4]==3)) || ((hou[4]==2)&&(you[4]==3)) )
			youhou();
	}
	while(!((qian[4]==2)&&(you[3]==4)))  
	{
		if(  ((hou[1]==2)&&(shang[1]==4)) || ((shang[4]==2)&&(you[1]==4)) )
			Shangfan();
		if(((you[1]==2)&&(shang[4]==4)) || ((qian[1]==2)&&(shang[6]==4)) || ((shang[3]==2)&&(zuo[1]==4))|| ((shang[6]==2)&&(qian[1]==4)) )
			Shangzhuan();
		if(( (shang[1]==2)&&(hou[1]==4) ) || ((you[3]==2)&&(qian[4]==4)))
			youqian();
		if(((zuo[4]==2)&&(hou[3]==4)) || ((hou[3]==2)&&(zuo[4]==4)) )
			zuohou();
		if(((zuo[1]==2)&&(shang[3]==4)))
			qianyou();
		if(((you[4]==2)&&(hou[4]==4)) || ((hou[4]==2)&&(you[4]==4)) )
			youhou();
	}
	while(!((hou[3]==5)&&(zuo[4]==3))) 
	{
		if(((shang[3]==5)&&(zuo[1]==3)) || ((qian[1]==5)&&(shang[6]==3)))
			Shangfan();
		if( ((hou[1]==5)&&(shang[1]==3)) || ((zuo[1]==5)&&(shang[3]==3))  || ((shang[4]==5)&&(you[1]==3)) || ( (shang[1]==5)&&(hou[1]==3) ) )
			Shangzhuan();
		if(((shang[6]==5)&&(qian[1]==3))||((zuo[4]==5)&&(hou[3]==3)))
			zuohou();
		if(((you[1]==5)&&(shang[4]==3)))
			houzuo();
		if(((you[4]==5)&&(hou[4]==3)) || ((hou[4]==5)&&(you[4]==3)) )
			youhou();
	}
	while(!((hou[4]==5)&&(you[4]==4)))  
	{
		if(((hou[1]==5)&&(shang[1]==4)) ||  ((shang[3]==5)&&(zuo[1]==4)) )
			Shangfan();
		if( ((you[1]==5)&&(shang[4]==4)) || ((qian[1]==5)&&(shang[6]==4))|| ((shang[4]==5)&&(you[1]==4)) || ( (shang[1]==5)&&(hou[1]==4) ) )
			Shangzhuan();
		if(((shang[6]==5)&&(qian[1]==4)))
			youhou();
		if(((zuo[1]==5)&&(shang[3]==4))||((you[4]==5)&&(hou[4]==4)) )
			houyou();
	}
}

void qian_fan(void)
{
	Qianzhuan();Youzhuan();Shangzhuan();Youfan();Shangfan();Qianfan();
}
void you_fan(void)
{
	Youzhuan();Houzhuan();Shangzhuan();Houfan();Shangfan();Youfan();
}
void hou_fan(void)
{
	Houzhuan();Zouzhuan();Shangzhuan();Zoufan();Shangfan();Houfan();
}
void zuo_fan(void )
{
	Zouzhuan();Qianzhuan();Shangzhuan();Qianfan();Shangfan();Zoufan();
}
void ni_qian(void)
{
	Zoufan();Shangfan();Zouzhuan();Shangfan();Zoufan();Shangfan();Shangfan();Zouzhuan();
}
void ni_zuo(void)
{
	Houfan();Shangfan();Houzhuan();Shangfan();Houfan();Shangfan();Shangfan();Houzhuan();
}
void ni_hou(void)
{
	Youfan();Shangfan();Youzhuan();Shangfan();Youfan();Shangfan();Shangfan();Youzhuan();
}
void ni_you(void)
{
	Qianfan();Shangfan();Qianzhuan();Shangfan();Qianfan();Shangfan();Shangfan();Qianzhuan();
}
void shuen_qian(void)
{
	Youzhuan();Shangzhuan();Youfan();Shangzhuan();Youzhuan();Shangzhuan();Shangzhuan();Youfan();
}
void shuen_you(void)
{
	Houzhuan();Shangzhuan();Houfan();Shangzhuan();Houzhuan();Shangzhuan();Shangzhuan();Houfan();
}
void shuen_hou(void)
{
	Zouzhuan();Shangzhuan();Zoufan();Shangzhuan();Zouzhuan();Shangzhuan();Shangzhuan();Zoufan();
}
void shuen_zuo(void)
{
	Qianzhuan();Shangzhuan();Qianfan();Shangzhuan();Qianzhuan();Shangzhuan();Shangzhuan();Qianfan();
}

void Shangmian(void)
{
	while(!((shang[1]==6)&&(shang[3]==6)&&(shang[4]==6)&&(shang[6]==6)))
	{
		if(((shang[1]!=6)&&(shang[3]!=6)&&(shang[4]==6)&&(shang[6]==6)) ||((shang[1]!=6)&&(shang[3]!=6)&&(shang[4]!=6)&&(shang[6]!=6)) || ((shang[1]!=6)&&(shang[3]==6)&&(shang[4]==6)&&(shang[6]!=6)))
			qian_fan();
		if(((shang[1]==6)&&(shang[3]==6)&&(shang[4]!=6)&&(shang[6]!=6)))
			hou_fan();
		if(((shang[1]!=6)&&(shang[3]==6)&&(shang[4]!=6)&&(shang[6]==6)))
			zuo_fan();
		if(((shang[1]==6)&&(shang[3]!=6)&&(shang[4]!=6)&&(shang[6]==6)) ||((shang[1]==6)&&(shang[3]!=6)&&(shang[4]==6)&&(shang[6]!=6)))
			you_fan();
	}
	while(!((shang[0]==6)&&(shang[2]==6)&&(shang[5]==6)&&(shang[7]==6)))
	{
		if(((shang[0]==6)&&(shang[2]!=6)&&(shang[5]!=6)&&(shang[7]!=6)))
		{
			if(hou[2]==6)
				ni_hou();
			if(zuo[0]==6)
				shuen_zuo();
		}
		if(((shang[0]!=6)&&(shang[2]==6)&&(shang[5]!=6)&&(shang[7]!=6)))
		{
			if(you[0]==6)
				ni_you();
			if(hou[0]==6)
				shuen_hou();
		}
		if(((shang[0]!=6)&&(shang[2]!=6)&&(shang[5]==6)&&(shang[7]!=6)))
		{
			if(zuo[2]==6)
				ni_zuo();
			if(qian[2]==6)
				shuen_qian();
		}
		if(((shang[0]!=6)&&(shang[2]!=6)&&(shang[5]!=6)&&(shang[7]==6)))
		{
			if(qian[0]==6)
				ni_qian();
			if(you[2]==6)
				shuen_you();
		}
		if(((shang[0]!=6)&&(shang[2]!=6)&&(shang[5]!=6)&&(shang[7]!=6)))
		{
			if((qian[0]!=6)&&(qian[2]!=6))
				shuen_qian();
			if((hou[0]!=6)&&(hou[2]!=6))
				shuen_hou();
			if((zuo[0]!=6)&&(zuo[2]!=6))
				shuen_zuo();
			if((you[0]!=6)&&(you[2]!=6))
				shuen_you();
		}
		if(((shang[0]==6)&&(shang[2]==6)&&(shang[5]!=6)&&(shang[7]!=6)))
		{
			if(qian[0]==6)
		  	shuen_qian();
			else
				shuen_you();
		}
		if(((shang[0]!=6)&&(shang[2]==6)&&(shang[5]!=6)&&(shang[7]==6)))
		{
			if(zuo[0]==6)
			  shuen_zuo();
			else
				shuen_qian();
		}
		if(((shang[0]!=6)&&(shang[2]!=6)&&(shang[5]==6)&&(shang[7]==6)))
		{
			if(hou[0]==6)
			  shuen_hou();
			else
				shuen_zuo();
		}
		if(((shang[0]==6)&&(shang[2]!=6)&&(shang[5]==6)&&(shang[7]!=6)))
		{
			if(you[0]==6)
			  shuen_you();
			else
				shuen_hou();
		}
		if(((shang[0]==6)&&(shang[2]!=6)&&(shang[5]!=6)&&(shang[7]==6)))
		{
			if(qian[0]==6)
				shuen_qian();
			if(zuo[0]==6)
				ni_zuo();
		}
		if(((shang[0]!=6)&&(shang[2]==6)&&(shang[5]==6)&&(shang[7]!=6)))
		{
			if(qian[2]==6)
				ni_qian();
			if(zuo[2]==6)
				shuen_zuo();
		}
	}		
}

void qian_bian(void)
{
	Zoufan();Houzhuan();Zoufan();Qianzhuan();Qianzhuan();Zouzhuan();Houfan();Zoufan();Qianzhuan();Qianzhuan();Zouzhuan();Zouzhuan();
}
void zuo_bian(void)
{
	Houfan();Youzhuan();Houfan();Zouzhuan();Zouzhuan();Houzhuan();Youfan();Houfan();Zouzhuan();Zouzhuan();Houzhuan();Houzhuan();
}
void you_bian(void)
{
	Qianfan();Zouzhuan();Qianfan();Youzhuan();Youzhuan();Qianzhuan();Zoufan();Qianfan();Youzhuan();Youzhuan();Qianzhuan();Qianzhuan();
}
void hou_bian(void)
{
	Youfan();Qianzhuan();Youfan();Houzhuan();Houzhuan();Youzhuan();Qianfan();Youfan();Houzhuan();Houzhuan();Youzhuan();Youzhuan();
}
void qian_zuo(void)
{
	Zoufan();Shangzhuan();Zoufan();Shangfan();Zoufan();Shangfan();Zoufan();Shangzhuan();Zouzhuan();Shangzhuan();Zouzhuan();Zouzhuan();
}
void qian_you(void)
{
	Youzhuan();Shangfan();Youzhuan();Shangzhuan();Youzhuan();Shangzhuan();Youzhuan();Shangfan();Youfan();Shangfan();Youzhuan();Youzhuan();
}
void zuo_qian(void)
{
	Qianzhuan();Shangfan();Qianzhuan();Shangzhuan();Qianzhuan();Shangzhuan();Qianzhuan();Shangfan();Qianfan();Shangfan();Qianzhuan();Qianzhuan();
}
void zuo_hou(void)
{
	Houfan();Shangzhuan();Houfan();Shangfan();Houfan();Shangfan();Houfan();Shangzhuan();Houzhuan();Shangzhuan();Houzhuan();Houzhuan();
}
void you_qian(void)
{
	Qianfan();Shangzhuan();Qianfan();Shangfan();Qianfan();Shangfan();Qianfan();Shangzhuan();Qianzhuan();Shangzhuan();Qianzhuan();Qianzhuan();
}
void you_hou(void)
{
	Houzhuan();Shangfan();Houzhuan();Shangzhuan();Houzhuan();Shangzhuan();Houzhuan();Shangfan();Houfan();Shangfan();Houzhuan();Houzhuan();
}
void hou_zuo(void)
{
	Zouzhuan();Shangfan();Zouzhuan();Shangzhuan();Zouzhuan();Shangzhuan();Zouzhuan();Shangfan();Zoufan();Shangfan();Zouzhuan();Zouzhuan();
}
void hou_you(void)
{
	Youfan();Shangzhuan();Youfan();Shangfan();Youfan();Shangfan();Youfan();Shangzhuan();Youzhuan();Shangzhuan();Youzhuan();Youzhuan();
}

void Wancheng(void)
{
	while(!((qian[0]==qian[2])&&(zuo[0]==zuo[2])&&(you[0]==you[2])&&(hou[0]==hou[2])))
	{
		if((qian[0]!=qian[2])&&(zuo[0]!=zuo[2])&&(you[0]!=you[2])&&(hou[0]!=hou[2]))
			qian_bian();
		if(qian[0]==qian[2])
			qian_bian();
		else if(zuo[0]==zuo[2])
			zuo_bian();
		else if(you[0]==you[2])
			you_bian();
		else if(hou[0]==hou[2])
			hou_bian();
	}
	while(!(qian[0]==2))
		Shangzhuan();
	while(!((qian[1]==2)&&(zuo[1]==3)&&(you[1]==4)&&(hou[1]==5)))
	{
		if((qian[1]!=2)&&(zuo[1]!=3)&&(you[1]!=4)&&(hou[1]!=5))
			qian_zuo();
		if(hou[1]==5)
		{
			if(qian[1]==3)
				qian_zuo();///ok
			else
				qian_you();//no
		}
		else if(zuo[1]==3)
		{
			if(you[1]==2)
				you_qian();//ok
			else
				you_hou();//no
		}
		else if(you[1]==4)
		{
			if(zuo[1]==2)
				zuo_qian();//no
			else
				zuo_hou();//ok
		}
		else if(qian[1]==2)
		{
			if(hou[1]==3)
				hou_zuo();//no
			else
				hou_you();//ok
		}
	}
}
