#ifndef _ADC_H_
#define _ADC_H_

#ifdef	__cplusplus
extern "C"{
#endif

#define PRESCVL				49				//��Ƶϵ��
#define SEL_MUX				2				//AIN2
#define WAIT_INTERRUPT		(0xd3)			//�ȴ��ж�ģʽ
#define SEPARATE_X_POSITION	(0x69)			//�����x������ת��ģʽ
#define SEPARATE_Y_POSITION	(0x9A)			//�����y������ת��ģʽ
#define AUTO_XY_POSITION	(0x0c)			//�Զ�xy������ת��ģʽ
#define ADC_BASEADDR		0x58000000		//adc�Ĵ�������ַ

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
*	��ͨADCת����ֵ
*/
int inline adc_adta(void);

/*start adc transform*/
void inline start_adc(void);
//ts test
void test_ts(void);

#ifdef	__cplusplus
}
#endif

#endif
