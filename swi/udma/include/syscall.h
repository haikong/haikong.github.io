#ifndef SYSCALL_H_
#define SYSCALL_H_

#ifdef	__cplusplus
extern "C"{
#endif

#define MAX_CALL 10
/*syscall number list*/
#define __NR_led_on		0
#define __NR_led_off	1
#define __NR_read		2
#define __NR_write		3
#define __NR_lock		4
#define __NR_key_on		5
#define __NR_key_off	6
#define __NR_test		7
#define __NR_myprintf	8
#define __NR_myputchar	9
//下面的内嵌汇编的意思是：
//	movl	__NR_##name	%eax
//	int	$0x80
//	movl	%eax	_res

#define _syscall0(type,name) \
type name(void) \
{ \
	type _res;\
	__asm__ __volatile__("swi 1\n\t"\
		:"=r"(_res)\
		:"r"(__NR_##name)); \
	return _res;\
}

#if 0
#define _syscall1(type,name,typea,arg1) \
type name(typea arg1) \
{ \
	type _res;\
	__asm__ __volatile__("swi 1\n\t"\
		:"=r"(_res)\
		:"r"(__NR_##name),"r"(arg1)); \
	return _res;\
}
#endif

#define _syscall1(type,name,typea,arg1) \
type name(typea arg1) \
{ \
	type _res;\
	__asm__ __volatile__(\
		"mov r0,%[x]\n\t"\
		"mov r1,%[y]\n\t"\
		"swi 1\n\t"\
		:"=r"(_res)\
		:[x]"r"(__NR_##name),[y]"r"(arg1)\
		:"r0","r1"); \
	return _res;\
}
int lock(void);
int led_on(int led_num);
int led_off(int led_num);
int read(int num);
int write(int num);
int key_on(int key_num);
int key_off(int key_num);
int test(void);
int myprintf(char* ptr);
int myputchar(char c);
void myswi(void);
#ifdef	__cplusplus
}
#endif

#endif
