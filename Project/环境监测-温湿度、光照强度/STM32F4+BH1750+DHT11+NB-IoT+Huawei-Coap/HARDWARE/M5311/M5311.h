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

//int m5311_send_cmd(char *cmd,char *ack,int waittime);
void pre_init_m5311(void);

/*COAPͨ�ŷ�ʽ��,ģ���ʼ�� */
void nb_coap_init(void);

/*COAPͨ�ŷ�ʽ��,ģ���ϱ����� */
int nb_coap_report( const char* buf, int len);

/*AT+MLWSEND=12,32342E3135352E3636352E34,9,4 */
int32_t NB_M5311_coapSendStr( char* data, uint16_t sendLen );

#endif

