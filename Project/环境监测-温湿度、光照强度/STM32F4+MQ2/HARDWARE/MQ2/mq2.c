#include "mq2.h"
#include "sys.h"
#include "math.h"
 
#define CAL_PPM 20  // 校准环境中PPM值
#define RL			5		// RL阻值
static float R0; 		// 元件在洁净空气中的阻值
 
 
/*传感器初始化*/
void MQ2_Init(void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;
		ADC_CommonInitTypeDef ADC_CommonInitStructure;
		ADC_InitTypeDef   ADC_InitStructure;

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //使能ADC1时钟

		//先初始化ADC1通道5 IO口
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//PA5 通道5
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
		GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化  

		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1复位
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//复位结束	 

		ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
		ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
		ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA失能
		ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
		ADC_CommonInit(&ADC_CommonInitStructure);//初始化

		ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
		ADC_InitStructure.ADC_ScanConvMode = DISABLE;//非扫描模式	
		ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//关闭连续转换
		ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
		ADC_InitStructure.ADC_NbrOfConversion = 1;//1个转换在规则序列中 也就是只转换规则序列1 
		ADC_Init(ADC1, &ADC_InitStructure);//ADC初始化

		ADC_Cmd(ADC1, ENABLE);//开启AD转换器

		ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_3Cycles);
}
 
/*读取 ADC 转换后的值*/
uint16_t Get_ADCValue(void)
{
    ADC_SoftwareStartConv(ADC1);  //开启通道转换
    while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));//等待转换结束
    return ADC_GetConversionValue(ADC1); //返回最近一次转换的结果
}
 
/************************************************************************************/
/*MQ2传感器数据处理*/
float MQ2_GetPPM1(void)
{
    float MQ2_Value = 0.0f;
    float MQ2_PPM = 0.0f;
	
    MQ2_Value = Get_ADCValue() * (3.3 / 4096);
    MQ2_PPM = ((MQ2_Value - 1) /0.1f) * 200;
	
    return MQ2_PPM;
}
/************************************************************************************/
/*MQ2传感器数据处理*/
float MQ2_GetPPM2(void)
{
	  float ppm;
	
		float Vrl = 3.3f * Get_ADCValue() / 4095.f;
    float RS = (3.3f - Vrl) / Vrl * RL; 
	  R0 = RS / pow(CAL_PPM / 613.9f, 1 / -2.074f);
    ppm = 613.9f * pow(RS/R0, -2.074f);
    return  ppm;
}







