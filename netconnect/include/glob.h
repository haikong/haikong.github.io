#ifndef _GLOB__H__
#define _GLOB__H__

#ifdef	__cplusplus
extern "C"{
#endif
#include <types.h>
#include <asm.h>

/*  
 *Byte-swapping, independently from CPU endianness
 *	swabXX[ps]?(foo)
 */
#define ___swab16(x) \
	((UINT16)( \
		(((UINT16)(x) & (UINT16)0x00ffU) << 8) | \
		(((UINT16)(x) & (UINT16)0xff00U) >> 8) ))
#define ___swab32(x) \
	((UINT32)( \
		(((UINT32)(x) & (UINT32)0x000000ffUL) << 24) | \
		(((UINT32)(x) & (UINT32)0x0000ff00UL) <<  8) | \
		(((UINT32)(x) & (UINT32)0x00ff0000UL) >>  8) | \
		(((UINT32)(x) & (UINT32)0xff000000UL) >> 24) ))
#define ___swab64(x) \
	((__u64)( \
		(__u64)(((__u64)(x) & (__u64)0x00000000000000ffULL) << 56) | \
		(__u64)(((__u64)(x) & (__u64)0x000000000000ff00ULL) << 40) | \
		(__u64)(((__u64)(x) & (__u64)0x0000000000ff0000ULL) << 24) | \
		(__u64)(((__u64)(x) & (__u64)0x00000000ff000000ULL) <<  8) | \
		(__u64)(((__u64)(x) & (__u64)0x000000ff00000000ULL) >>  8) | \
		(__u64)(((__u64)(x) & (__u64)0x0000ff0000000000ULL) >> 24) | \
		(__u64)(((__u64)(x) & (__u64)0x00ff000000000000ULL) >> 40) | \
		(__u64)(((__u64)(x) & (__u64)0xff00000000000000ULL) >> 56) ))

/*
 * Allow constant folding
 */
/*swap macro defination*/
#ifndef __arch__swab16
#define __arch__swab16(x) ___swab16(x)
#endif
#ifndef __arch__swab32
#define __arch__swab32(x) ___swab32(x)
#endif
#ifndef __arch__swab64
#define __arch__swab64(x) ___swab64(x)
#endif
	
#ifndef __arch__swab16p
#define __arch__swab16p(x) __swab16(*(x))
#endif
#ifndef __arch__swab32p
#define __arch__swab32p(x) __swab32(*(x))
#endif
#ifndef __arch__swab64p
#define __arch__swab64p(x) __swab64(*(x))
#endif
#ifndef __arch__swab16s
#define __arch__swab16s(x) do { *(x) = __swab16p((x)); } while (0)
#endif
#ifndef __arch__swab32s
#define __arch__swab32s(x) do { *(x) = __swab32p((x)); } while (0)
#endif
#ifndef __arch__swab64s
#define __arch__swab64s(x) do { *(x) = __swab64p((x)); } while (0)
#endif	
	
#define __swab16(x) \
	(__builtin_constant_p((__u16)(x)) ? \
	 ___swab16((__u16)(x)) : \
	 __arch__swab16((__u16)(x)))
#define __swab32(x) \
	(__builtin_constant_p((__u32)(x)) ? \
	 ___swab32((__u32)(x)) : \
	 __arch__swab32((__u32)(x)))
#define __swab64(x) \
	(__builtin_constant_p((__u64)(x)) ? \
	 ___swab64((__u64)(x)) : \
	 __arch__swab64((__u64)(x)))
	
#ifdef _LITTLE_ENDIAN
#define __le16_to_cpu(x) ((UINT16)(x))
#define __le32_to_cpu(x) ((UINT16)(x))
#define cpu_to_le32(x)   ((UINT32)(x))
#endif
#ifdef _BIG_ENDIAN
#define __le16_to_cpu(x) (___swab16(x))
#define __le32_to_cpu(x) (___swab32(x))
#define cpu_to_le32(x)   (___swab32(x))

#endif 
/*
 * Output a debug text when condition "cond" is met. The "cond" should be
 * computed by a preprocessor in the best case, allowing for the best
 * optimization.
 */
#define debug_cond(cond, fmt, args...)		\
	do {					\
		if (cond)			\
			printf(fmt, ##args);	\
	} while (0)

#define debug(fmt, args...)			\
	debug_cond(_DEBUG, fmt, ##args)

#define __be32_to_cpu(x) __swab32((__force unsigned int)(unsigned int)(x))
#define __be16_to_cpu(x) __swab16((__force unsigned short)(unsigned short)(x))

#define ___ntohl(x) __be32_to_cpu(x)
#define ___ntohs(x) __be16_to_cpu(x)
/*function*/
unsigned int fbit_pend(unsigned int );

/*host to network endian*/
#define htonw(x) ((unsigned short)(\
	(((unsigned short)(x) & (unsigned short)0x00ffU) << 8) |\
	(((unsigned short)(x) & (unsigned short)0xff00U) >> 8)))
	
#define htonl(x) ((unsigned int)(				\
	(((unsigned int)(x) & (unsigned int)0x000000ffUL) << 24) |		\
	(((unsigned int)(x) & (unsigned int)0x0000ff00UL) <<  8) |		\
	(((unsigned int)(x) & (unsigned int)0x00ff0000UL) >>  8) |		\
	(((unsigned int)(x) & (unsigned int)0xff000000UL) >> 24)))

#define ntohl(x) ((unsigned long)___ntohl(x))
#define ntohw(x) ___ntohs(x)

#ifdef	__cplusplus
}
#endif

#endif
