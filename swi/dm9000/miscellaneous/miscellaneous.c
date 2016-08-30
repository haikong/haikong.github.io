#include <miscellaneous.h>
#include <s3c2440_addr.h>

/*init led*/
void init_led(void)
{
	    // LED1-LED4对应的4根引脚设为输出
        GPBCON = GPB5_out | GPB6_out | GPB7_out | GPB8_out ;
        // K1-K2对应的2根引脚设为输入
        GPGCON = GPG11_in & GPG3_in ;
        // K3-K4对应的2根引脚设为输入
        GPFCON = GPF2_in & GPF0_in ;
        //LED全部熄灭
        GPBDAT = ~0;
}

