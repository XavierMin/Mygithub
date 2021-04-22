#include "M5311.h"
#include "usart2.h"

extern u8  USART2_RX_BUF[USART2_MAX_RECV_LEN]; 		//接收缓冲,最大USART3_MAX_RECV_LEN字节
extern u8  USART2_TX_BUF[USART2_MAX_SEND_LEN]; 		//发送缓冲,最大USART3_MAX_SEND_LEN字节
extern u16 USART2_RX_STA;   						//接收数据状态

char	wbuf[AT_DATA_LEN];

int str_to_hex( const char *bufin, int len, char *bufout )
{
    int i = 0;
    if ( NULL == bufin || len <= 0 || NULL == bufout )
    {
        return (-1);
    }
    for ( i = 0; i < len; i++ )
    {
        sprintf( bufout + i * 2, "%02X", bufin[i] );
    }
    return (0);
}

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

void pre_init_m5311(void)
{
	while(m5311_send_cmd("AT","OK",50))//检测是否接入模块
	{
		printf("未找到M5311模块!\r\n");
		delay_ms(100);
		delay_ms(100);	
	}	
	printf("已找到M5311模块!\r\n正在配置M5311模块...\r\n");
	
	m5311_send_cmd("AT+CIMI","OK",50);
	m5311_send_cmd("AT+CGSN=1","OK",50);
//	m5311_send_cmd("ATE0","OK",50);											//关闭回显
	m5311_send_cmd("AT+CPSMS=0","OK",50);								//关闭PSM模式
	m5311_send_cmd("AT+SM=LOCK_FOREVER","OK",50);				//关闭睡眠模式
	
	while(m5311_send_cmd("AT+MIPLCREATE","+MIPLCREATE:0",1500))//请求添加设备
	{
		printf("配置M5311模块失败，无法请求创建实例！！！\r\n");
		m5311_send_cmd("AT+MIPLDELETE=0","OK",1000);
		delay_ms(500);
	}
		
	m5311_send_cmd("AT+CFUN?","OK",50);
	printf( "[INFO]NB_detect\n" );
	
	m5311_send_cmd("AT+CGATT?","CGATT: 1",100);
	printf( "[INFO]Waiting for join network\r\n" );
	printf( "[INFO]NB_netstat\r\n" );
	
	m5311_send_cmd("AT+CSQ","+CSQ:",100);
	printf( "[INFO]NB_checkCsq\r\n" );
	
	m5311_send_cmd("AT+CEREG?","+CEREG: 0,1",100);				//检查模块是否注网成功
	printf( "[INFO]NB_getCereg \r\n" );
	
	m5311_send_cmd("AT+CGPADDR=1","+CGPADDR",100);
	
	printf("注网完成！\r\n");
}


/*COAP通信方式下,模块初始化 */
void nb_coap_init(void)
{
    pre_init_m5311();

		m5311_send_cmd("AT+MLWNEW=49.4.85.232,5683","OK",50);			//设置COAP服务器地址以及端口号
		m5311_send_cmd("AT+MLWOPEN=1,90","OK",1500);							//连接华为云服务器，之后返回对应的响应码表示成功
		m5311_send_cmd("AT+MLWCFG=1,1","OK",200);								  //设置收发数据的格式类型，1,1 表示收发都是字符串类型，0,0 表示HEX 类型
}


/*COAP通信方式下,模块上报数据 */
int nb_coap_report( const char* buf, int len )
{
    int ret;
    if ( buf == NULL || len <= 0 )
        return (-1);
    ret = NB_M5311_coapSendStr( (char *) buf, len );
    return (ret);
}

/*AT+MLWSEND=12,32342E3135352E3636352E34,9,4 */
int32_t NB_M5311_coapSendStr( char* data, uint16_t sendLen )
{
    char	*cmd		= "AT+MLWSEND";
    char	buf[512]	= { 0 };
    if ( strlen( data ) != sendLen || sendLen > 256 )
    {
        return (-1);
    }

    str_to_hex( (const char *) data, sendLen, buf );
    snprintf( wbuf, AT_DATA_LEN, "%s=%d,%s,9,4",
              cmd,
              (int) sendLen, buf );
		m5311_send_cmd((u8 *) wbuf, "OK",500);

    return 0;
}




