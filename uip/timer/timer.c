#include <timer.h>
#include <s3c2440_addr.h>

/*
*	timer initialize:
*	��ʱ������ʱ��Ƶ�� = PCLK / {Ԥ��Ƶֵ+1} / {��Ƶֵ};PCLK = 100MHz
*	{Ԥ��Ƶֵ} = 0~255
*	{��Ƶֵ}= 2, 4, 8, 16
*   ��ʱ������ʱ��Ƶ�� = 1MHz
*	Ԥ��Ƶֵ = 24
*	��Ƶֵ = 2
*	��ʱʱ�� = 1.5s��PWM = 1/3
*	use timer0,not dead area,not dma
*/
void timer_init(void)
{
	TCFG0 	= 49;			//Prescaler 0 = 49
	TCFG1 	= 0x00;			//ѡ��PWM��ʱ��0��ѡͨ����,MUX = 1/2
	TCNTB0 	= 50000;		//1s
	TCMPB0 	= 5000;			//0.1s
	TCON  	= (1 << 1);		//�ֶ�����,ֹͣ��ʱ��0
	TCON 	= 0x9;			//�Զ����ؿ���,start timer
}

/*timer0 hanlder*/
void Timer0_Handle(unsigned int vec_num)
{
	static int num = 0;
	//every 1s change the led status
	GPBDAT = (num & 0xf) << 5;
	num++;
}


