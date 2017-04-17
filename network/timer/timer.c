#include <timer.h>
#include <s3c2440_addr.h>

/*timer2 for global counter*/
static unsigned long g_timer2_count;
/*
*	timer initialize:
*	定时器输入时钟频率 = PCLK / {预分频值+1} / {分频值};PCLK = 50MHz
*	{预分频值} = 0~255
*	{分频值}= 2, 4, 8, 16
*   定时器输入时钟频率 = 1MHz
*	预分频值 = 50
*	分频值 = 2
*	定时时间 = 1s，PWM = 1/2
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
	TCFG1 	= tcfg1 | 0x00;			//选择PWM定时器0的选通输入,MUX = 1/2
	TCNTB0 	= 50000;				//1s
	TCMPB0 	= 5000;					//0.1s
	TCON  	= tcon | (1 << 1);		//手动更新,停止定时器0
	TCON 	= tcon | (1 << 3) | (1 << 0);	//自动重载开启,start timer
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
 函 数 名  : timer2_counter_init
 功能描述  : timer2 usefor number conuter
 	*	定时器输入时钟频率 = PCLK / {预分频值+1} / {分频值};PCLK = 50MHz
	*	{预分频值} = 0~255
	*	{分频值}= 2, 4, 8, 16
	*   定时器输入时钟频率 = 0.5MHz
	*	预分频值 = 49
	*	分频值 = 2
	*	定时时间 = 0.1s
	*	use timer2,not dead area,not dma,inverter off
 输入参数  : unsigned short time:the multiple number for 0.1s  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2017年1月17日
    作    者   : QSWWD
    修改内容   : 新生成函数

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
	TCFG1 	= tcfg1 | (0x00 << 8);	//选择PWM定时器2的选通输入,MUX = 1/2
	TCNTB2 	= (time + 1) * 50000;	//Timer 2 count buffer register
	TCMPB2 	= 0;					//Timer 2 compare buffer register
	TCON  	= tcon | (1 << 13);		//手动更新,停止定时器2
	TCON 	= tcon | (1 << 15) | (1 << 12);	//自动重载开启,start timer
}

/*****************************************************************************
 函 数 名  : Timer2_Handle
 功能描述  : timer2 interrupt service program,increase the global counter number 
 输入参数  : unsigned int vec_num : ISR offset numer for timer1 
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2017年1月17日
    作    者   : QSWWD
    修改内容   : 新生成函数

*****************************************************************************/
void Timer2_Handle(unsigned int vec_num)
{
	/*just increase the global number*/
	g_timer2_count++;
}

/*****************************************************************************
 函 数 名  : get_timer2_counter
 功能描述  : get the timer2 global counters
 输入参数  : void  
 输出参数  : 无
 返 回 值  : the timer2 global counters
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2017年1月17日
    作    者   : QSWWD
    修改内容   : 新生成函数

*****************************************************************************/
unsigned long get_timer2_counter(void)
{
	return g_timer2_count;
}
