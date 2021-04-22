#ifndef _BH1750_H_
#define _BH1750_H_


#define BH1750_Addr 0x46          //�ӻ���ַ+���д����λ0x46+0;������0x46+1;
#define BH_Powdown  0x00
#define BH1750_ON       0x01          //����
#define BH1750_CON      0x10          //������ȡ
#define BH1750_ONE      0x20          //һ�ζ�ȡ
#define BH1750_RSET     0x07          //����,�������ݼĴ���ֵ��POWERONģʽ����Ч

void BH1750_Init(void);			//IO��ʼ��
void Start_BH1750(void);	  //�ϵ硢���á�������ݼĴ���
void Read_BH1750(void);     //������ȡ�ڲ��Ĵ�������              
void Convert_BH1750(void);  //����ת��






#endif





































