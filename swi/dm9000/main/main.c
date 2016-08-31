#include <s3c2440_addr.h>
#include <init.h>
#include <swi.h>
#include <syscall.h>
#include <serial.h>
#include <intirq.h>
#include "printf.h"
#include <dma.h>
#include <lcdlib.h>
#include <adc.h>
#include <dm9000.h>
#include <miscellaneous.h>
#include <test.h>
#include <timer.h>

/*-O2优化时参数需加volatile,否则被优化为0*/
static  inline void wait(volatile unsigned long time)
{
	for(;time > 0;time--);
}

/*JUST FOR TEST*/
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

/*JUST FOR TEST*/
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

/*CLEAR THE BSS SEGTAMENT*/
void clean_bss(void)
{
    extern int _bss_start, _bss_end;
    int *p = &_bss_start;

    for (; p < &_bss_end; p++)
        *p = 0;
}

//
void lcd_test(void)
{
	char c;
	printf("please input a charactor to select function.\n");
	printf("1-Test_Lcd_Tft_16Bit_240320.\n\r");
	printf("2-Test_Lcd_Tft_8Bit_240320.\n\r");
	printf("3-Test_Lcd_Tft_16Bit_640480.\n\r");
	printf("4-Test_Lcd_Tft_8Bit_640480.\n\r");
	printf("5-Test_Lcd_Tft_24Bit_480272.\n\r");
	printf("q-for quit test.\n");
	while((c = getc()) != 'q')
	{
		printf("%c\n\r",c);
		switch(c)
		{
		case '1':{
			Test_Lcd_Tft_16Bit_240320();
			break;
			}	
		case '2':{
			Test_Lcd_Tft_8Bit_240320();
			break;
			}
		case '3':{
			Test_Lcd_Tft_16Bit_640480();
			break;
			}
		case '4':{
			Test_Lcd_Tft_8Bit_640480();
			break;
			}	
		case '5':{
			Test_Lcd_Tft_24Bit_480272();
			break;
			}	
		default:{
			printf("Your input error,try again.\n");
			break;
			}
		}
		printf("Input your select number.\n");
	}
	printf("Quit for lcd test.\n");
}

/*nomal adc test for variable resistance*/
void adc_test(void)
{
	char c;
	printf("Initialization adc......\n");
	adc_init(PRESCVL,SEL_MUX);
	printf("input 'a' to start normal adc\n\rand input 't' to start ts\n\r'q' for quit.\n\r");						
	while((c = getc()) != 'q')
	{
		switch(c){
		case 'a':{
			start_adc();			
			printf("Now read the variable resistance value:%d\n\r",adc_adta());
		break;	
		}
		case 't':{
			test_ts();
			break;
		}
		default:
			break;
		}
	}
	printf("Quit for adc test.\n\r");
}

/*main*/
int main(int argc,char** argv)
{
	int i;
	//swi_test();
    clean_bss();	
    uart0_init();
	init_led();
	init_irq();
	#ifdef _DEBUG
	timer_init();					//invoking timer0 initialize and enable timer0 handle	
	register_interrupt(ISR_TIMER0_OFT,Timer0_Handle);
	#endif
	if(DM9000_Init() < 0)
		return -1;
	for(i = 0;i < 10;i++)
	{
		wait(50000);
		test_dm9000();
	}
	arp_test();
	while(1);
	return 0;
}

