#include <glob.h>

/*****************************************************************************
 函 数 名  : fbit_pend
 功能描述  : get the first bit 1 from the interger
 输入参数  : unsigned int num
 输出参数  : 无
 返 回 值  : unsigned int
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年4月22日
    作    者   : QSWWD
    修改内容   : 新生成函数

*****************************************************************************/
unsigned int fbit_pend( unsigned int num )
{
    int i;
	i = 0;
	do{
		if((num & 0x1) == 0x1)
			break;
		num = num >>1;
		i++;
	}while(num);
	return i;
}

