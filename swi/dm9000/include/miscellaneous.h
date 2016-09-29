#ifndef _MISCELLANEOUS_H__
#define _MISCELLANEOUS_H__

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
 * K1-K4对应GPF1,GPF4、GPF2、GPF0
 */
#define GPF4_in    ~(3<<(4*2))
#define GPF2_in     ~(3<<(2*2))
#define GPF1_in     ~(3<<(1*2))
#define GPF0_in     ~(3<<(0*2))

void init_led(void);
void key_init( unsigned char key,unsigned char mode );
void KeyINT2_Handle(unsigned int vec_num);
void KeyINT3_Handle(unsigned int vec_num);
void KeyINT4_Handle(unsigned int vec_num);

#ifdef	__cplusplus
}
#endif

#endif
