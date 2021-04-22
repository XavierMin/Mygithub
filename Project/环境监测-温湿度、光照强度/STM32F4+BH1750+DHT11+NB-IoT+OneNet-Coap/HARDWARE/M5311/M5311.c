#include "M5311.h"
#include "usart2.h"

extern u8  USART2_RX_BUF[USART2_MAX_RECV_LEN]; 		//接收缓冲,最大USART3_MAX_RECV_LEN字节
extern u8  USART2_TX_BUF[USART2_MAX_SEND_LEN]; 		//发送缓冲,最大USART3_MAX_SEND_LEN字节
extern u16 USART2_RX_STA;   						//接收数据状态

u8* m5311_check_cmd(u8 *str)
{
	
	char *strx=0;
	if(USART2_RX_STA&0X8000)		//接收到一次数据了
	{ 
		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//添加结束符
		printf("\r\nNB-IOT data:%s\r\n",USART2_RX_BUF);
		strx=strstr((const char*)USART2_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}


//向m5311发送命令
//cmd:发送的命令字符串
//ack:期待的应答结果,如果为空,则表示不需要等待应答
//waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果)
//       1,发送失败
u8 m5311_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART2_RX_STA=0;
	uart2_send_buff((u8*)cmd, strlen(cmd));
	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_ms(10);
			if(USART2_RX_STA&0X8000)//接收到期待的应答结果
			{
				if(m5311_check_cmd(ack))
				{
					printf("ack:%s\r\n",(u8*)ack);
					break;//得到有效数据 
				}
				USART2_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
} 

/*
m5311模块初始化
返回数据：
1：模块未接入或者模块未启动
*/

void init_m5311(void)
{
	while(m5311_send_cmd("AT","OK",50))//检测是否接入模块
	{
		printf("未找到M5311模块!\r\n");
		delay_ms(100);
		delay_ms(100);	
	}	
	printf("已找到M5311模块!\r\n正在配置M5311模块...\r\n");
	m5311_send_cmd("AT+CMSYSCTRL=0,2","OK",50);//打开指示灯指示功能
	m5311_send_cmd("AT+SM=LOCK","OK",50);//关闭睡眠模式
	while(m5311_send_cmd("AT+MIPLCREATE","+MIPLCREATE:0",1500))//请求添加设备
	{
		printf("配置M5311模块失败，无法请求创建实例！！！\r\n");
		m5311_send_cmd("AT+MIPLDELETE=0","OK",1000);
		delay_ms(500);
	}
	printf("创建实例成功...\r\n");
	while(m5311_send_cmd("AT+MIPLADDOBJ=0,3200,1,\"1\",1,1","OK",800))//请求添加设备
	{
		printf("配置M5311模块失败，无法请求添加OBJ！！！\r\n");
		m5311_send_cmd("AT+MIPLDELOBJ=0,3200","OK",800);
		delay_ms(500);
	}
	printf("添加OBJ成功...\r\n");
	m5311_send_cmd("AT+MIPLDISCOVERRSP=0,3200,1,4,\"5750\"","OK",200);//指定资源列表
	while(m5311_send_cmd("AT+MIPLOPEN=0,60","+MIPLEVENT:0,6",1500))//想onenet平台注册或查询注册状态
	{
		printf("配置M5311模块失败，无法注册设备！！！\r\n");
	}
	printf("设备注册成功...\r\n");	
	m5311_send_cmd("AT+MIPLUPDATE=0,60,0","+MIPLEVENT:0,11",50);//设定设备在线时间60S

}


