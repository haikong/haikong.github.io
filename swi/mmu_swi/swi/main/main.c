#include <s3c2440_addr.h>
#include <init.h>
#include <swi.h>
#include <syscall.h>
#include <serial.h>

static void Delay_ms(unsigned int ) __attribute__ ((__unused__));
static void myled_test(void) __attribute__ ((__unused__));

void Delay_ms(unsigned int num_ms) 
{
	int i = 0;
	while(num_ms--)
		for(i = 0;i < 5000;i++);	
}

void myled_test(void)
{
	int i ,j,tmp = 0;
	for(i = 0;i < 4;i++){
		for(j = 0;j < 10;j++){
			GPBDAT |= 0xff;
			Delay_ms(5000);
			GPBDAT = tmp;
			Delay_ms(5000);
		}
		tmp = 0 << (i + 5);
	}
}

#if 0
void swi_test(void)
{
	unsigned char c;
    uart0_init();   // 波特率115200，8N1(8个数据位，无校验位，1个停止位)
    led_on(0x9);
    while(1)
    {
        // 从串口接收数据后，判断其是否数字或子母，若是则加1后输出
        c = get_c();
        if (isDigit(c) || isLetter(c))
            put_c(c+1);
          
    }	
}
#endif

static inline void wait(unsigned long time)
{
	unsigned int i = 10000000;
	while(time--)
		while(i--);
}

int main(int argc,char** argv)
{
	unsigned long i = 0;
	while(1){
		wait(30000000);
		uGPBDAT = (~(i<<5));	 	// 根据i的值，点亮LED1,2,3,4
		if(++i == 16)
			i = 0;
	}
	return 0;	
}
