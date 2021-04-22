#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "bh1750.h"
#include "iic.h"
/*�ļ�������ֱ�������£�����ǿ�ȿɴ�6��10��lx��
û��̫��������0.1��1��lx���������ʵ�����100��550lx��ҹ��������Ϊ0.2lx��
���ݷ�Χ�����ǲɼ�������������ȷ��*/

extern float result_lx;
u8 BH_Buf[];

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200				
 	LCD_Init();					//LCD��ʼ��   	
	LED_Init();
	BH1750_Init();//!!!!�տ�ʼΪʲôһֱ��ʾ����������Ϊû��д��һ�仰��û�ж�IIC���г�ʼ����

  while(1)
		{
			LED0=!LED0;
			delay_ms(100);
			Start_BH1750();//����������е���������в���ȥ�ˣ���ѯ����ԭ���� û��дIIC_Init();��Start_BH1750()�У��õ���IIC_Init()����ĳ���
			delay_ms(180); //���Ͳ���ʱ����120ms����������Ϊ180ms
			Read_BH1750(); 
			Convert_BH1750(); 
			sprintf((char*)BH_Buf,"Illumi:%3.1f %s",result_lx,"lx");//.1f ��ʾ��ȷ��С����һλ��%s:��ʾ�ַ������͡�
			//sprintf();��������Ĺ����ǰѵ������ֵ����ݣ����յڶ����ָ�ʽ���ַ��ĸ�ʽ���ѵ������ֵ����ݽ��и�ʽ����Ȼ��Ѹ�ʽ������������ͣ��洢���ַ����Ļ���������ȥ
			//����ʾ���նȵ�ʱ��������նȱ�Ϊʮλ��/��λ�������նȵĵ�λ���Ϊ��lxx����lxxx��,Ŀǰ��û����������������⡣
			
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
    

