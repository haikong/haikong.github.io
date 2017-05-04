#include <timer.h>
#include <s3c2440_addr.h>

/*timer2 for global counter*/
static unsigned long g_timer2_count;
/*
*	timer initialize:
*	��ʱ������ʱ��Ƶ�� = PCLK / {Ԥ��Ƶֵ+1} / {��Ƶֵ};PCLK = 50MHz
*	{Ԥ��Ƶֵ} = 0~255
*	{��Ƶֵ}= 2, 4, 8, 16
*   ��ʱ������ʱ��Ƶ�� = 1MHz
*	Ԥ��Ƶֵ = 50
*	��Ƶֵ = 2
*	��ʱʱ�� = 1s��PWM = 1/2
*	use timer0,not dead area,not dma
*/
void timer0_init(void)
{
	unsigned int tcfg0;
	unsigned int tcfg1;
	unsigned int tcon;
	tcfg0 = TCFG0;
	tcfg1 = TCFG1;
	tcon  = TCON;

	TCFG0 	= tcfg0 | 49;			//Prescaler 0 = 49
	TCFG1 	= tcfg1 | 0x00;			//ѡ��PWM��ʱ��0��ѡͨ����,MUX = 1/2
	TCNTB0 	= 50000;				//1s
	TCMPB0 	= 5000;					//0.1s
	TCON  	= tcon | (1 << 1);		//�ֶ�����,ֹͣ��ʱ��0
	TCON 	= tcon | (1 << 3) | (1 << 0);	//�Զ����ؿ���,start timer
}

/*timer0 hanlder*/
void Timer0_Handle(unsigned int vec_num)
{
	static int num = 0;
	//every 1s change the led status
	GPBDAT = (num & 0xf) << 5;
	num++;
}

/*****************************************************************************
 �� �� ��  : timer2_counter_init
 ��������  : timer2 usefor number conuter
 	*	��ʱ������ʱ��Ƶ�� = PCLK / {Ԥ��Ƶֵ+1} / {��Ƶֵ};PCLK = 50MHz
	*	{Ԥ��Ƶֵ} = 0~255
	*	{��Ƶֵ}= 2, 4, 8, 16
	*   ��ʱ������ʱ��Ƶ�� = 0.5MHz
	*	Ԥ��Ƶֵ = 49
	*	��Ƶֵ = 2
	*	��ʱʱ�� = 0.1s
	*	use timer2,not dead area,not dma,inverter off
 �������  : unsigned short time:the multiple number for 0.1s  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��1��17��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

*****************************************************************************/
void timer2_counter_init(unsigned short time)
{
	unsigned int tcfg0;
	unsigned int tcfg1;
	unsigned int tcon;
	tcfg0 = TCFG0;
	tcfg1 = TCFG1;
	tcon  = TCON;

	TCFG0 	= tcfg0 | (49 << 8);	//Prescaler 1 = 24
	TCFG1 	= tcfg1 | (0x00 << 8);	//ѡ��PWM��ʱ��2��ѡͨ����,MUX = 1/2
	TCNTB2 	= (time + 1) * 50000;	//Timer 2 count buffer register
	TCMPB2 	= 0;					//Timer 2 compare buffer register
	TCON  	= tcon | (1 << 13);		//�ֶ�����,ֹͣ��ʱ��2
	TCON 	= tcon | (1 << 15) | (1 << 12);	//�Զ����ؿ���,start timer
}

/*****************************************************************************
 �� �� ��  : Timer2_Handle
 ��������  : timer2 interrupt service program,increase the global counter number 
 �������  : unsigned int vec_num : ISR offset numer for timer1 
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��1��17��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

*****************************************************************************/
void Timer2_Handle(unsigned int vec_num)
{
	/*just increase the global number*/
	g_timer2_count++;
}

/*****************************************************************************
 �� �� ��  : get_timer2_counter
 ��������  : get the timer2 global counters
 �������  : void  
 �������  : ��
 �� �� ֵ  : the timer2 global counters
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��1��17��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

*****************************************************************************/
unsigned long get_timer2_counter(void)
{
	return g_timer2_count;
}
