#include "adc.h"
#include "types.h"
#include <s3c2440_addr.h>
#include <stdio.h>
#include <intirq.h>

//adc register  module
static pt_adc_reg	adc_reg = NULL;
//x\y position  
static unsigned short g_xpos;
static unsigned short g_ypos;
/*
*	����adc��Ƶϵ��������Դ
*	prscvl:A/D ��Ƶϵ��(0-255)
*	sel_mux:000 = AIN 0 001 = AIN 1 010 = AIN 2 011 = AIN 3
*			100 = YM 101 = YP 110 = XM 111 = XP
*/
void adc_init(unsigned char prscvl,unsigned char sel_mux)
{
	//initialze the base registers addr
	adc_reg = (pt_adc_reg)ADC_BASEADDR;
	//adc��Ƶϵ��Ϊ(49+1)������ԴAIN 2
	adc_reg->ACCON = (1 << 14) | (prscvl << 6) | (sel_mux << 3);
	//���ô�����ת��ʱ��
	adc_reg->ADCDLY = 50000;
}

void inline start_adc(void)
{
	if(adc_reg == NULL)
		return ;
	//enable read start transform
	adc_reg->ACCON |= (1 << 0);
}

/*
*�����������жϴ������	
*/
static void ts_interrupt(void)
{
	//�ʼⰴ���ж�
	if((adc_reg->ADCDAT0 & (0x1 << 15)) == 0)
	{
		printf("pen down.\n\r");
		//���ô�����Ϊ�Զ�xy������ת��ģʽ
		adc_reg->ADCTSC = AUTO_XY_POSITION;
		start_adc();
	}
	//�ʼ�̧���ж�
	else
	{
		printf("pen up.\n\r");
		//���ô�����Ϊ�ȴ��ж�ģʽ,�ȴ�����
		adc_reg->ADCTSC = WAIT_INTERRUPT;
	}	
}

/*
*������ת������жϴ������	
*/
static void adc_interrupt(void)
{
	g_xpos = adc_reg->ADCDAT0 & 0x3ff;
	g_ypos = adc_reg->ADCDAT1 & 0x3ff;
	printf("The position : x = %d,y = %d.\n\r",g_xpos,g_ypos);
	//���ô�����Ϊ�ȴ��ж�ģʽ
	adc_reg->ADCTSC = WAIT_INTERRUPT;
	//���ô�ʱ�������ȴ��ɿ�
	adc_reg->ADCTSC |= (1 << 8);

}

//adc and ts interrupt handler
static void adc_ts_inthandle(void)
{
	if(SUBSRCPND & BIT_SUB_TC)
		ts_interrupt();
	else if(SUBSRCPND & BIT_SUB_ADC)
		adc_interrupt();
}

//test touch screen
void test_ts(void)
{
	int ret;
	//register interrupt
	ret = register_interrupt(ISR_ADC_OFT,adc_ts_inthandle);
	//���ô�����ת��ʱ��
	adc_reg->ADCDLY = 50000;
	//���ô�����Ϊ�ȴ��ж�ģʽ���ȴ�����
	adc_reg->ADCTSC = WAIT_INTERRUPT;
}
/*
*	��ͨADCת����ֵ
*/
int inline adc_adta(void)
{
	if(adc_reg == NULL)
		return -1;
	//before tranformed end waiting
	while(adc_reg->ACCON == 0);
	return (adc_reg->ADCDAT0 & 0x3ff);
}

