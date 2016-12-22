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
/*
 * provide defaults when no architecture-specific optimization is detected
 */
/*swap inline function*/
static __inline__ __noused__ __attribute__((__const__)) __u16 __fswab16(__u16 x)
{
	return __arch__swab16(x);
}

static __inline__ __noused__  __attribute__((__const__)) __u32 __fswab32(__u32 x)
{
	return __arch__swab32(x);
}

static __inline__ __noused__ __attribute__((__const__)) __u64 __fswab64(__u64 x)
{
#ifdef __SWAB_64_THRU_32__
	__u32 h = x >> 32;
	__u32 l = x & ((1ULL<<32)-1);
	return (((__u64)__swab32(l)) << 32) | ((__u64)(__swab32(h)));
#else
	return __arch__swab64(x);
#endif
}

static __inline__ __noused__ __u16 __swab16p(const __u16 *x)
{
	return __arch__swab16p(x);
}
static __inline__ __noused__  __u32 __swab32p(const __u32 *x)
{
	return __arch__swab32p(x);
}
static __inline__ __noused__ __u64 __swab64p(const __u64 *x)
{
	return __arch__swab64p(x);
}

static __inline__ __noused__ void __swab16s(__u16 *addr)
{
	__arch__swab16s(addr);
}
static __inline__ __noused__ void __swab32s(__u32 *addr)
{
	__arch__swab32s(addr);
}
static __inline__ __noused__ void __swab64s(__u64 *addr)
{
	__arch__swab64s(addr);
}

