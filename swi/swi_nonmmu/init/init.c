#include <s3c2440_addr.h>
#include <init.h>

/*上电后，WATCH DOG默认是开着的，要把它关掉 */
void diable_watch_dog()
{
	WTCON	= 0;
}

#if 0 
/* 设置控制SDRAM的13个寄存器 */
void memsetup(void)
{
	int 	i = 0;
	unsigned long *p = (unsigned long *)MEM_CTL_BASE;

    /* SDRAM 13个寄存器的值 */
    unsigned long  const    mem_cfg_val[]={ 0x22011110,     //BWSCON
                                            0x00000700,     //BANKCON0
                                            0x00000700,     //BANKCON1
                                            0x00000700,     //BANKCON2
                                            0x00000700,     //BANKCON3  
                                            0x00000700,     //BANKCON4
                                            0x00000700,     //BANKCON5
                                            0x00018005,     //BANKCON6
                                            0x00018005,     //BANKCON7
                                            0x008C04F4,     //REFRESH
                                            0x000000B1,     //BANKSIZE
                                            0x00000030,     //MRSRB6
                                            0x00000030,     //MRSRB7
     };

	for(; i < 13; i++)
		p[i] = mem_cfg_val[i];
}
#endif
/*直接赋值，因为链接地址为0x30000000，否则应把链接地址分为两段0和0x30000000
*不然链接不到，不能取值
*/
void memsetup(void)
{
	unsigned long *p = (unsigned long *)MEM_CTL_BASE;
	p[0]	= 0x22011110;   //BWSCON
	p[1]	= 0x00000700;   //BANKCON0
	p[2]	= 0x00000700;   //BANKCON1
	p[3]	= 0x00000700;   //BANKCON2
	p[4]	= 0x00000700;   //BANKCON3
	p[5]	= 0x00000700;   //BANKCON4
	p[6]	= 0x00000700;   //BANKCON5
	p[7]	= 0x00018005;   //BANKCON6
	p[8]	= 0x00018005;   //BANKCON7
	p[9]	= 0x008C04F4;   //REFRESH
	p[10]	= 0x000000B1;   //BANKSIZE
	p[11]	= 0x00000030;   //MRSRB6
	p[12]	= 0x00000030;   //MRSRB7
}

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
        GPBDAT = 0xffffffff;
}

/*
 * 对于MPLLCON寄存器，[19:12]为MDIV，[9:4]为PDIV，[1:0]为SDIV
 * 有如下计算公式：
 *  S3C2410: MPLL(FCLK) = (m * Fin)/(p * 2^s)
 *  S3C2410: MPLL(FCLK) = (2 * m * Fin)/(p * 2^s)
 *  其中: m = MDIV + 8, p = PDIV + 2, s = SDIV
 * 对于本开发板，Fin = 12MHz
 * 设置CLKDIVN，令分频比为：FCLK:HCLK:PCLK=1:2:4，
 * FCLK=200MHz,HCLK=100MHz,PCLK=50MHz
 */
void clock_init(void)
{
    CLKDIVN  = 0x03;            // FCLK:HCLK:PCLK=1:2:4, HDIVN=1,PDIVN=1

    /* 如果HDIVN非0，CPU的总线模式应该从“fast bus mode”变为“asynchronous bus mode” */
	__asm__ __volatile__(
    "mrc    p15, 0, r1, c1, c0, 0\n"        /* 读出控制寄存器 */ 
    "orr    r1, r1, #0xc0000000\n"          /* 设置为“asynchronous bus mode” */
    "mcr    p15, 0, r1, c1, c0, 0\n"        /* 写入控制寄存器 */
    );

    MPLLCON = S3C2440_MPLL_200MHZ;  /* 现在，FCLK=200MHz,HCLK=100MHz,PCLK=50MHz */        
}
/*copy copy_steppingstone_to_sdram
* copy 4kb code to sdram
*/
void copy_steppingstone_to_sdram(void)
{
    unsigned int *pdwSrc  = (unsigned int *)0;
    unsigned int *pdwDest = (unsigned int *)0x30000000;
    
    while (pdwSrc < (unsigned int *)4096)
    {
        *pdwDest = *pdwSrc;
        pdwDest++;
        pdwSrc++;
    }
}
