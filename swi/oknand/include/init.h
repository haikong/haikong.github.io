#ifndef INIT_H_
#define INIT_H_

#ifdef	__cplusplus
extern "C"{
#endif

/*
 * LED1-4对应GPB5、GPB6、GPB7、GPB8
 */
#define GPB5_out        (1<<(5*2))
#define GPB6_out        (1<<(6*2))
#define GPB7_out        (1<<(7*2))
#define GPB8_out        (1<<(8*2))

/*
 * K1-K4对应GPG11、GPG3、GPF2、GPF0
 */
#define GPG11_in    ~(3<<(11*2))
#define GPG3_in     ~(3<<(3*2))
#define GPF2_in     ~(3<<(2*2))
#define GPF0_in     ~(3<<(0*2))
/*MPLL 200MZ*/
#define S3C2440_MPLL_200MHZ     ((0x5c<<12)|(0x01<<4)|(0x02))

void disable_watch_dog(void) __attribute__ ((__section__(".init")));
void init_led(void) __attribute__ ((__section__(".init")));
void memsetup(void) __attribute__ ((__section__(".init")));
void copy_steppingstone_to_sdram(void) __attribute__ ((__section__(".init")));
void clock_init(void) __attribute__ ((__section__(".init")));
 
#ifdef	__cplusplus
}
#endif

#endif
