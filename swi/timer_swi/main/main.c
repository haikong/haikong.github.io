#include <s3c2440_addr.h>
#include <init.h>
#include <swi.h>
#include <syscall.h>
#include <serial.h>

/*-O2�Ż�ʱ�������volatile,�����Ż�Ϊ0*/
static  void wait(volatile unsigned long time)
{
	for(;time > 0;time--);
}

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


void swi_test(void)
{
	unsigned char c;
    uart0_init();   // ������115200��8N1(8������λ����У��λ��1��ֹͣλ)
    led_on(0x9);
    while(1)
    {
        // �Ӵ��ڽ������ݺ��ж����Ƿ����ֻ���ĸ���������1�����
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
