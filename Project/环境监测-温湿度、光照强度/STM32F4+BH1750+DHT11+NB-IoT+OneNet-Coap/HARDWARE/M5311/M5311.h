#ifndef __M5311_H
#define __M5311_H	 
#include "sys.h"
#include "usart2.h"
#include "string.h"	
#include "delay.h"
#include "usart.h"

u8* m5311_check_cmd(u8 *str);
u8 m5311_send_cmd(u8 *cmd,u8 *ack,u16 waittime);
void init_m5311(void);

#endif

