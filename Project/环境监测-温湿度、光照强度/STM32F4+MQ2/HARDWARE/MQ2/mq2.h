#ifndef _MQ2_H_
#define _MQ2_H_

#include "stm32f4xx.h"
#include "stm32f4xx_adc.h"
	
void   MQ2_Init(void);
uint16_t Get_ADCValue(void);
float MQ2_GetPPM1(void);
float MQ2_GetPPM2(void);

#endif
