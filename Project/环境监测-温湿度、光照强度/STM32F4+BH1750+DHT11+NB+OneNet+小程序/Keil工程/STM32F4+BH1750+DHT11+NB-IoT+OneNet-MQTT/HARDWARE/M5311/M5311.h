#ifndef __M5311_H
#define __M5311_H	 
#include "sys.h"
#include "usart2.h"
#include "string.h"	
#include "delay.h"
#include "usart.h"

#define  AT_DATA_LEN 1024

int str_to_hex(const char *bufin, int len, char *bufout);

u8* m5311_check_cmd(u8 *str);
u8 m5311_send_cmd(u8 *cmd,u8 *ack,u16 waittime);

void pre_init_m5311(void);

/*MQTTͨ�ŷ�ʽ��,ģ���ʼ�� */
void nb_mqtt_init(void);

/*MQTTͨ�ŷ�ʽ��,ģ���ϱ����� */
int nb_mqtt_report( const char* buf, int len);

int32_t NB_M5311_mqttSendStr( char* data, uint16_t sendLen );

#endif

