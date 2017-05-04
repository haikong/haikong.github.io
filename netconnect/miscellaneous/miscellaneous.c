#include <miscellaneous.h>
#include <s3c2440_addr.h>
#include <stdio.h>
/*init led*/
void init_led(void)
{
    // LED1-LED4对应的4根引脚设为输出
    GPBCON = GPB5_out | GPB6_out | GPB7_out | GPB8_out ;
    //LED全部熄灭
    GPBDAT = ~0;
}

/*****************************************************************************
 函 数 名  :key_init
 功能描述  :setting key mode,K1-K4 uses GPFCON:0,1,2,4
 输入参数  :unsigned char key 
 			unsigned char mode
 			 0:input
 			 1:output
 			 2:interrput
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年9月18日
    作    者   : QSWWD
    修改内容   : 新生成函数

*****************************************************************************/
void key_init( unsigned char key,unsigned char mode )
{
	key %= 32;
	mode %= 3;
	
	GPFCON &= ~(3 << (key * 2));
	GPFCON |= mode << (key * 2);
}
/*key2 hanlder*/
void KeyINT2_Handle(unsigned int vec_num)
{
	GPBDAT = ~GPBDAT;
	printf("KeyINT2_Handle\n\t");
}

/*key3 hanlder*/
void KeyINT3_Handle(unsigned int vec_num)
{
	printf("KeyINT3_Handle\n\t");
}

/*key4 hanlder*/
void KeyINT4_Handle(unsigned int vec_num)
{
	printf("KeyINT4_Handle\n\t");
}

