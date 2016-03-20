#include "adc.h"

static pt_adc_reg	adc_reg = NULL;

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
}

/*
*	���ô���������
*/
void ts_init(void)
{

}

void inline start_adc(void)
{
	if(adc_reg == NULL)
		return -1;
	//enable read start transform
	adc_reg->ACCON |= (1 << 1);
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

