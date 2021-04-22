#include "M5311.h"
#include "usart2.h"

extern u8  USART2_RX_BUF[USART2_MAX_RECV_LEN]; 		//���ջ���,���USART3_MAX_RECV_LEN�ֽ�
extern u8  USART2_TX_BUF[USART2_MAX_SEND_LEN]; 		//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
extern u16 USART2_RX_STA;   						//��������״̬

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

void init_m5311(void)
{
	while(m5311_send_cmd("AT","OK",50))//����Ƿ����ģ��
	{
		printf("δ�ҵ�M5311ģ��!\r\n");
		delay_ms(100);
		delay_ms(100);	
	}	
	printf("���ҵ�M5311ģ��!\r\n��������M5311ģ��...\r\n");
	m5311_send_cmd("AT+CMSYSCTRL=0,2","OK",50);//��ָʾ��ָʾ����
	m5311_send_cmd("AT+SM=LOCK","OK",50);//�ر�˯��ģʽ
	while(m5311_send_cmd("AT+MIPLCREATE","+MIPLCREATE:0",1500))//��������豸
	{
		printf("����M5311ģ��ʧ�ܣ��޷����󴴽�ʵ��������\r\n");
		m5311_send_cmd("AT+MIPLDELETE=0","OK",1000);
		delay_ms(500);
	}
	printf("����ʵ���ɹ�...\r\n");
	while(m5311_send_cmd("AT+MIPLADDOBJ=0,3200,1,\"1\",1,1","OK",800))//��������豸
	{
		printf("����M5311ģ��ʧ�ܣ��޷��������OBJ������\r\n");
		m5311_send_cmd("AT+MIPLDELOBJ=0,3200","OK",800);
		delay_ms(500);
	}
	printf("���OBJ�ɹ�...\r\n");
	m5311_send_cmd("AT+MIPLDISCOVERRSP=0,3200,1,4,\"5750\"","OK",200);//ָ����Դ�б�
	while(m5311_send_cmd("AT+MIPLOPEN=0,60","+MIPLEVENT:0,6",1500))//��onenetƽ̨ע����ѯע��״̬
	{
		printf("����M5311ģ��ʧ�ܣ��޷�ע���豸������\r\n");
	}
	printf("�豸ע��ɹ�...\r\n");	
	m5311_send_cmd("AT+MIPLUPDATE=0,60,0","+MIPLEVENT:0,11",50);//�趨�豸����ʱ��60S

}


