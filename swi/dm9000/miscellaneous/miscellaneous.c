#include <miscellaneous.h>
#include <s3c2440_addr.h>

/*init led*/
void init_led(void)
{
	    // LED1-LED4��Ӧ��4��������Ϊ���
        GPBCON = GPB5_out | GPB6_out | GPB7_out | GPB8_out ;
        // K1-K2��Ӧ��2��������Ϊ����
        GPGCON = GPG11_in & GPG3_in ;
        // K3-K4��Ӧ��2��������Ϊ����
        GPFCON = GPF2_in & GPF0_in ;
        //LEDȫ��Ϩ��
        GPBDAT = ~0;
}

