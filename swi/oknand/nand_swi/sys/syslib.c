#include <syscall.h>
#include <s3c2440_addr.h>

static int __led_on(int led_num)
{
	GPBDAT = 0xffff;
	GPBDAT = ~((led_num & 0xf)<<5); 
	return 0;	
}

static int __led_off(int led_num)
{
	GPBDAT |= (led_num & 0xf) << 5;
	return 0;	
}

static int __read(int num)
{
	return 0;	
}
static int __write(int num)
{
	return 0;	
}
static int __lock(void)
{
	return 0;	
}
static int __key_on(int key_num)
{
	return 0;	
}
static int __key_off(int key_num)
{
	return 0;	
}
static int __test(void)
{
	return 0;	
}
static int __printf(char* ptr)
{
	return 0;	
}
static int __putchar(char c)
{
	return 0;	
}

void* sys_call_tbl[MAX_CALL] = {
	__led_on,__led_off,__read,__write,__lock,__key_on,__key_off,
	__test,__printf,__putchar	
};

_syscall0(int,lock)
_syscall0(int,test)
_syscall1(int,led_on,int,led_num)
_syscall1(int,led_off,int,led_num)
_syscall1(int,read,int,num)
_syscall1(int,write,int,num)
_syscall1(int,key_off,int,key_num)
_syscall1(int,myprintf,char*,ptr)
_syscall1(int,myputchar,char,c)
