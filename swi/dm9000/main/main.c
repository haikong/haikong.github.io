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

/*-O2�Ż�ʱ�������volatile,�����Ż�Ϊ0*/
static  void wait(volatile unsigned long time)
{
	for(;time > 0;time--);
}

/*JUST FOR TEST*/
void myled_test(void)
{
	int i = 0;
	while(1){
		wait(30000);
		GPBDAT = (~(i<<5));	 	// ����i��ֵ������LED1,2,3,4
		if(++i == 16)
			i = 0;
	}
}

/*JUST FOR TEST*/
void swi_test(void)
{
	unsigned char c;
    uart0_init();   // ������115200��8N1(8������λ����У��λ��1��ֹͣλ)
    led_on(0x9);
    while(1)
    {
        // �Ӵ��ڽ������ݺ��ж����Ƿ����ֻ���ĸ���������1�����
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

/*just test for dma_uart*/
void dma_uart_test(void)
{
	int ret;
	/*register dma0 interrupt*/
	ret = register_interrupt(17,Dma0_Handle);
	if(ret)
	{
		printf("register dma int_handle error.\n");
		return ;
	}
    char temp[255] = "Hello world!This is a dma send buffer.\nHello world!This is a dma send buffer.\n";
    /*use uart0*/
    DMA_UART((unsigned long*)0x50000020,temp,96,1,1);
}

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
	//timer_init();					//invoking timer0 initialize and enable timer0 handle
	//swi_test();
    clean_bss();	
    uart0_init();
	init_led();
	init_irq();
	dm9000_init();
	DM9000_Init();
	while(1);
	return 0;
}

