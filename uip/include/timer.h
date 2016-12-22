#ifndef _TIMER_H__
#define _TIMER_H__

#ifdef	__cplusplus
extern "C"{
#endif

void timer_init(void);

void Timer0_Handle(unsigned int vec_num);

#ifdef	__cplusplus
}
#endif

#endif
