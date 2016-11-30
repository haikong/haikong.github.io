#include <timer.h>
#include <s3c2440_addr.h>

/*
*	timer initialize:
*	定时器输入时钟频率 = PCLK / {预分频值+1} / {分频值};PCLK = 100MHz
*	{预分频值} = 0~255
*	{分频值}= 2, 4, 8, 16
*   定时器输入时钟频率 = 1MHz
*	预分频值 = 24
*	分频值 = 2
*	定时时间 = 1.5s，PWM = 1/3
*	use timer0,not dead area,not dma
*/
void timer_init(void)
{
	TCFG0 	= 49;			//Prescaler 0 = 49
	TCFG1 	= 0x00;			//选择PWM定时器0的选通输入,MUX = 1/2
	TCNTB0 	= 50000;		//1s
	TCMPB0 	= 5000;			//0.1s
	TCON  	= (1 << 1);		//手动更新,停止定时器0
	TCON 	= 0x9;			//自动重载开启,start timer
}

/*timer0 hanlder*/
void Timer0_Handle(unsigned int vec_num)
{
	static int num = 0;
	//every 1s change the led status
	GPBDAT = (num & 0xf) << 5;
	num++;
}


