#ifndef _BH1750_H_
#define _BH1750_H_


#define BH1750_Addr 0x46          //从机地址+最后写方向位0x46+0;读方向0x46+1;
#define BH_Powdown  0x00
#define BH1750_ON       0x01          //启动
#define BH1750_CON      0x10          //连续读取
#define BH1750_ONE      0x20          //一次读取
#define BH1750_RSET     0x07          //重置,重置数据寄存器值在POWERON模式下有效

void BH1750_Init(void);			//IO初始化
void Start_BH1750(void);	  //上电、设置、清除数据寄存器
void Read_BH1750(void);     //连续读取内部寄存器数据              
void Convert_BH1750(void);  //数据转化






#endif





































