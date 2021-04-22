#include "M5311.h"
#include "usart2.h"

extern u8  USART2_RX_BUF[USART2_MAX_RECV_LEN]; 		//���ջ���,���USART3_MAX_RECV_LEN�ֽ�
extern u8  USART2_TX_BUF[USART2_MAX_SEND_LEN]; 		//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
extern u16 USART2_RX_STA;   						//��������״̬

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
	if(USART2_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//��ӽ�����
		printf("\r\nNB-IOT data:%s\r\n",USART2_RX_BUF);
		strx=strstr((const char*)USART2_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}


//��m5311��������
//cmd:���͵������ַ���
//ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
//waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)
//       1,����ʧ��
u8 m5311_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART2_RX_STA=0;
	uart2_send_buff((u8*)cmd, strlen(cmd));
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(USART2_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				if(m5311_check_cmd(ack))
				{
					printf("ack:%s\r\n",(u8*)ack);
					break;//�õ���Ч���� 
				}
				USART2_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
} 

/*
m5311ģ���ʼ��
�������ݣ�
1��ģ��δ�������ģ��δ����
*/

void pre_init_m5311(void)
{
	while(m5311_send_cmd("AT","OK",50))//����Ƿ����ģ��
	{
		printf("δ�ҵ�M5311ģ��!\r\n");
		delay_ms(100);
		delay_ms(100);	
	}	
	printf("���ҵ�M5311ģ��!\r\n��������M5311ģ��...\r\n");
	
	m5311_send_cmd("AT+CIMI","OK",50);
	m5311_send_cmd("AT+CGSN=1","OK",50);
	m5311_send_cmd("AT+CPSMS=0","OK",50);								//�ر�PSMģʽ
	m5311_send_cmd("AT+SM=LOCK_FOREVER","OK",50);				//�ر�˯��ģʽ
	
	while(m5311_send_cmd("AT+MIPLCREATE","+MIPLCREATE:0",2000))//��������豸
	{
		printf("����M5311ģ��ʧ�ܣ��޷����󴴽�ʵ��������\r\n");
		m5311_send_cmd("AT+MIPLDELETE=0","OK",1000);
		delay_ms(500);
	}

	m5311_send_cmd("AT+CFUN?","OK",50);
	printf( "[INFO]NB_detect\n" );
	
	m5311_send_cmd("AT+CGATT?","CGATT: 1",100);
	printf( "[INFO]Waiting for join network\r\n" );
	
	m5311_send_cmd("AT+CSQ","+CSQ:",100);
	printf( "[INFO]NB_checkCsq\r\n" );
	
	m5311_send_cmd("AT+CEREG?","+CEREG: 0,1",100);				//���ģ���Ƿ�ע���ɹ�
	printf( "[INFO]NB_getCereg \r\n" );
	
	m5311_send_cmd("AT+CGPADDR=1","+CGPADDR",100);
	
	printf("ע����ɣ�\r\n");
}


/*MQTTͨ�ŷ�ʽ��,ģ���ʼ�� */
void nb_mqtt_init(void)
{
    pre_init_m5311();

		m5311_send_cmd("AT+MQTTCFG=\"183.230.40.39\",6002,\"708607580\",1800,\"417939\",\"zxy\",1","OK",50);			//MQTT client�������ã�����OneNET������
		m5311_send_cmd("AT+MQTTOPEN=1,1,0,0,0,\"\",\"\"","OK",1500);							//�������ӽ�������
}

/*MQTTͨ�ŷ�ʽ��,ģ���ϱ����� */
int nb_mqtt_report( const char* buf, int len )
{
    int ret;
    if ( buf == NULL || len <= 0 )
        return (-1);
    ret = NB_M5311_mqttSendStr( (char *) buf, len );
    return (ret);
}


int32_t NB_M5311_mqttSendStr( char* data, uint16_t sendLen )
{
    char	*cmd		= "AT+MQTTPUB";
    char	buf[512]	= { 0 };
    if ( strlen( data ) != sendLen || sendLen > 256 )
    {
        return (-1);
    }

    str_to_hex( (const char *) data, sendLen, buf );
    snprintf( wbuf, AT_DATA_LEN, "%s=$dp,1,0,0,%d,03001C%s",
              cmd,
              (int) sendLen + 3, buf );
		m5311_send_cmd((u8 *) wbuf, "OK",500);

    return 0;
}


