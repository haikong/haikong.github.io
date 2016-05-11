#ifndef INIT_H_
#define INIT_H_

#ifdef	__cplusplus
extern "C"{
#endif

/*MPLL 200MZ*/
#define S3C2440_MPLL_200MHZ     ((0x5c<<12)|(0x01<<4)|(0x02))

void disable_watch_dog(void) __attribute__ ((__section__(".init")));
void memsetup(void) __attribute__ ((__section__(".init")));
void copy_steppingstone_to_sdram(void) __attribute__ ((__section__(".init")));
void clock_init(void) __attribute__ ((__section__(".init")));


#ifdef	__cplusplus
}
#endif

#endif
