#ifndef _TIMER_H__
#define _TIMER_H__

#ifdef	__cplusplus
extern "C"{
#endif

/*function declearations*/
void timer0_init(void);
void Timer0_Handle(unsigned int vec_num);
void timer2_counter_init(unsigned short time);
void Timer2_Handle(unsigned int vec_num);
unsigned long get_timer2_counter(void);

#ifdef	__cplusplus
}
#endif

#endif
