#ifndef _ADC_H_
#define _ADC_H_

#ifdef	__cplusplus
extern "C"{
#endif

#define PRESCVL				49				//分频系数
#define SEL_MUX				2				//AIN2
#define WAIT_INTERRUPT		(0xd3)			//等待中断模式
#define SEPARATE_X_POSITION	(0x69)			//分离的x轴坐标转换模式
#define SEPARATE_Y_POSITION	(0x9A)			//分离的y轴坐标转换模式
#define AUTO_XY_POSITION	(0x0c)			//自动xy轴坐标转换模式
#define ADC_BASEADDR		0x58000000		//adc寄存器基地址

/*ADC configuration registers*/
typedef struct _ADC_REG{
	unsigned long ACCON;		//控制寄存器
	unsigned long ADCTSC;		//触摸屏控制寄存器
	unsigned long ADCDLY;		//启动延时寄存器
	unsigned long ADCDAT0;		//转换数据寄存器0
	unsigned long ADCDAT1;		//转换数据寄存器1
	unsigned long ADCUPDN;		//触摸屏起落终端检测寄存器
}t_adc_reg,*pt_adc_reg;
/*
*	设置adc分频系数和输入源
*	prscvl:A/D 分频系数(0-255)
*	sel_mux:000 = AIN 0 001 = AIN 1 010 = AIN 2 011 = AIN 3
*			100 = YM 101 = YP 110 = XM 111 = XP
*/
void adc_init(unsigned char prscvl,unsigned char sel_mux);

/*
*	普通ADC转换数值
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
