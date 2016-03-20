#ifndef _ADC_H_
#define _ADC_H_

#ifdef	__cplusplus
extern "C"{
#endif

#define PRESCVL		49				//��Ƶϵ��
#define SEL_MUX		2				//AIN2

#define ADC_BASEADDR	0x58000000	//adc�Ĵ�������ַ
/*ADC configuration registers*/
typedef struct _ADC_REG{
	unsigned long ACCON;		//���ƼĴ���
	unsigned long ADCTSC;		//���������ƼĴ���
	unsigned long ADCDLY;		//������ʱ�Ĵ���
	unsigned long ADCDAT0;		//ת�����ݼĴ���0
	unsigned long ADCDAT1;		//ת�����ݼĴ���1
	unsigned long ADCUPDN;		//�����������ն˼��Ĵ���
}t_adc_reg,*pt_adc_reg;
/*
*	����adc��Ƶϵ��������Դ
*	prscvl:A/D ��Ƶϵ��(0-255)
*	sel_mux:000 = AIN 0 001 = AIN 1 010 = AIN 2 011 = AIN 3
*			100 = YM 101 = YP 110 = XM 111 = XP
*/
void adc_init(unsigned char prscvl,unsigned char sel_mux);

/*
*	���ô���������
*/
void ts_init(void);

/*
*	��ͨADCת����ֵ
*/
int inline adc_adta(void);

/*start adc transform*/
void inline start_adc(void);

#ifdef	__cplusplus
}
#endif

#endif
