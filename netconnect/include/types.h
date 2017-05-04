#ifndef _TPYES_H
#define _TPYES_H

#define _LITTLE_ENDIAN
#ifndef NULL
#define		NULL	((void*)0)
#endif

typedef long long  INT64;
typedef unsigned long long  UINT64;

#ifndef UINT32
#define		UINT32	unsigned int
#endif

#ifndef UINT16
#define		UINT16	unsigned short
#endif

#ifndef UINT8
#define		UINT8	unsigned char
#endif

typedef unsigned long long __u64;
typedef unsigned short umode_t;

typedef __signed__ char __s8;
typedef unsigned char __u8;

typedef __signed__ short __s16;
typedef unsigned short __u16;

typedef __signed__ int __s32;
typedef unsigned int __u32;

#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned int size_t;
#endif  /* _SIZE_T */

typedef INT64  s64 ;
typedef UINT64 u64 ;

/* bsd */
typedef unsigned char		u_char;
typedef unsigned short	u_short;
typedef unsigned int		u_int;
typedef unsigned long		u_long;
typedef unsigned char		uchar;

/* sysv */
typedef unsigned char		unchar;
typedef unsigned short		ushort;
typedef unsigned int		uint;
typedef unsigned long		ulong;

typedef unsigned long phys_addr_t;
typedef unsigned long phys_size_t;
typedef phys_addr_t resource_size_t;
/* Force a compilation error if condition is true, but also produce a
   result (of value 0 and type size_t), so the expression can be used
   e.g. in a structure initializer (or where-ever else comma expressions
   aren't permitted). */
#define BUILD_BUG_ON_ZERO(e) (sizeof(struct { int:-!!(e); }))
#define BUILD_BUG_ON_NULL(e) ((void *)sizeof(struct { int:-!!(e); }))
/* &a[0] degrades to a pointer: a different type from an array */
#define __must_be_array(a) \
  BUILD_BUG_ON_ZERO(__builtin_types_compatible_p(typeof(a), typeof(&a[0])))
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]) + __must_be_array(arr))
/*attibute*/
#ifdef __CHECKER__
#define __user		__attribute__((noderef, address_space(1)))
#define __kernel	/* default address space */
#define __safe		__attribute__((__safe__))
#define __force	__attribute__((__force__))
#define __nocast	__attribute__((__nocast__))
#define __iomem	__attribute__((noderef, address_space(2)))
#else
#define __user
#define __kernel
#define __safe
#define __force
#define __nocast
#define __iomem
#endif
#define __attribute_const__	__attribute__((__const__))
#define __inline__	__attribute__((__always_inline__))
#define __noused__	__attribute__((__unused__)) 

#endif /* _TPYES_H */

