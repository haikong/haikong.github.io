#include <s3c2440_addr.h>
#include <init.h>
#include <swi.h>
#include <syscall.h>
#include <serial.h>

/*-O2优化时参数需加volatile,否则被优化为0*/
static  void wait(volatile unsigned long time)
{
	for(;time > 0;time--);
}

void myled_test(void)
{
	int i = 0;
	while(1){
		wait(30000);
		GPBDAT = (~(i<<5));	 	// 根据i的值，点亮LED1,2,3,4
		if(++i == 16)
			i = 0;
	}
}


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

int main(int argc,char** argv)
{

	timer_init();					//invoking timer0 initialize and enable timer0 handle
	swi_test();
	return 0;	
}
