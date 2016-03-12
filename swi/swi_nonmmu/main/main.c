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

int main(int argc,char** argv)
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
	return 0;	
}
