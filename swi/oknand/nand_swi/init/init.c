#include <s3c2440_addr.h>
#include <init.h>

/*�ϵ��WATCH DOGĬ���ǿ��ŵģ�Ҫ�����ص� */
void disable_watch_dog(void)
{
	WTCON	= 0;
}

#if 0 
/* ���ÿ���SDRAM��13���Ĵ��� */
void memsetup(void)
{
	int 	i = 0;
	unsigned long *p = (unsigned long *)MEM_CTL_BASE;

    /* SDRAM 13���Ĵ�����ֵ */
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
/*ֱ�Ӹ�ֵ����Ϊ���ӵ�ַΪ0x30000000������Ӧ�����ӵ�ַ��Ϊ����0��0x30000000
*��Ȼ���Ӳ���������ȡֵ
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
	    // LED1-LED4��Ӧ��4��������Ϊ���
        GPBCON = GPB5_out | GPB6_out | GPB7_out | GPB8_out ;
        // K1-K2��Ӧ��2��������Ϊ����
        GPGCON = GPG11_in & GPG3_in ;
        // K3-K4��Ӧ��2��������Ϊ����
        GPFCON = GPF2_in & GPF0_in ;	
        //LEDȫ��Ϩ��
        GPBDAT = ~0;
}

/*
 * ����MPLLCON�Ĵ�����[19:12]ΪMDIV��[9:4]ΪPDIV��[1:0]ΪSDIV
 * �����¼��㹫ʽ��
 *  S3C2410: MPLL(FCLK) = (m * Fin)/(p * 2^s)
 *  S3C2410: MPLL(FCLK) = (2 * m * Fin)/(p * 2^s)
 *  ����: m = MDIV + 8, p = PDIV + 2, s = SDIV
 * ���ڱ������壬Fin = 12MHz
 * ����CLKDIVN�����Ƶ��Ϊ��FCLK:HCLK:PCLK=1:2:4��
 * FCLK=200MHz,HCLK=100MHz,PCLK=50MHz
 */
void clock_init(void)
{
    CLKDIVN  = 0x03;            // FCLK:HCLK:PCLK=1:2:4, HDIVN=1,PDIVN=1

    /* ���HDIVN��0��CPU������ģʽӦ�ôӡ�fast bus mode����Ϊ��asynchronous bus mode�� */
	__asm__ __volatile__(
    "mrc    p15, 0, r1, c1, c0, 0\n"        /* �������ƼĴ��� */ 
    "orr    r1, r1, #0xc0000000\n"          /* ����Ϊ��asynchronous bus mode�� */
    "mcr    p15, 0, r1, c1, c0, 0\n"        /* д����ƼĴ��� */
    );

    MPLLCON = S3C2440_MPLL_200MHZ;  /* ���ڣ�FCLK=200MHz,HCLK=100MHz,PCLK=50MHz */        
}
/*copy copy_steppingstone_to_sdram
* copy 4kb code to sdram
*/
void copy_steppingstone_to_sdram(void)
{
    unsigned int *pdwSrc  = (unsigned int *)2048;
    unsigned int *pdwDest = (unsigned int *)0x30004000;
    
    while (pdwSrc < (unsigned int *)4096)
    {
        *pdwDest = *pdwSrc;
        pdwDest++;
        pdwSrc++;
    }
}
