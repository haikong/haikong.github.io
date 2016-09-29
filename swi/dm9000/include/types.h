#ifndef _TPYES_H
#define _TPYES_H

#define _LITTLE_ENDIAN
#ifndef NULL
#define		NULL	((void*)0)
#endif

#ifndef UINT32
#define		UINT32	unsigned int
#endif

#ifndef UINT16
#define		UINT16	unsigned short
#endif

#ifndef UINT8
#define		UINT8	unsigned char
#endif

#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned int size_t;
#endif  /* _SIZE_T */

typedef unsigned long phys_addr_t;
typedef unsigned long phys_size_t;
typedef unsigned long ulong;
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

#endif /* _TPYES_H */

