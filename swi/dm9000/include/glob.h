#ifndef _GLOB__H__
#define _GLOB__H__

#ifdef	__cplusplus
extern "C"{
#endif
#include <types.h>

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

unsigned int fbit_pend(unsigned int );

#ifdef _LITTLE_ENDIAN
#define __le16_to_cpu(x) ((UINT16)(x))
#endif
#ifdef _BIG_ENDIAN
#define __le16_to_cpu(x) (___swab16(x))
#endif 
#ifdef	__cplusplus
}
#endif

#endif
