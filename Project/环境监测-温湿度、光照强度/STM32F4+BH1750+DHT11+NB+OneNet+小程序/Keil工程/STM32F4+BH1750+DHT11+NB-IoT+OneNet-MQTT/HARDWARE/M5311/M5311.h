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

/*MQTT通信方式下,模块初始化 */
void nb_mqtt_init(void);

/*MQTT通信方式下,模块上报数据 */
int nb_mqtt_report( const char* buf, int len);

int32_t NB_M5311_mqttSendStr( char* data, uint16_t sendLen );

#endif

