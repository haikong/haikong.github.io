#include <s3c2440_addr.h>
#include <init.h>
#include <swi.h>
#include <syscall.h>
#include <serial.h>
#include <dma.h>

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
        c = getc();
        if (isDigit(c) || isLetter(c))
            put_c(c+1);
    }
}

void clean_bss(void)
{
    extern int _bss_start, _bss_end;
    int *p = &_bss_start;

    for (; p < &_bss_end; p++)
        *p = 0;
}


int dma_flag;

/*main*/
int main(int argc,char** argv)
{

	//timer_init();					//invoking timer0 initialize and enable timer0 handle
	//swi_test();
    clean_bss();
    uart0_init();

    printf_string("main\n");
    char temp[255] = "Hello world!This is a dma send buffer.\n";
    dma_flag = 0;
    /*use uart0*/
    DMA_UART((unsigned long*)0x50000020,temp,48,1,1);
    while(dma_flag == 0);
    printf_string(temp);
	//DMA_memcpy((unsigned long*)DMA_DST,(unsigned long*)DMA_SRC,DMA_SIZE);
    //printf_string("\n");
    //printf_string("Hello world!\n");
	while(1);
	return 0;
}
