#include <s3c2440_addr.h>
#include <serial.h>
#include <hkstdio.h>


/*
 * ��ʼ��UART0
 * 115200,8N1,������
 */
void uart0_init(void)
{
    GPHCON  |= 0xa0;    // GPH2,GPH3����TXD0,RXD0
    GPHUP   = 0x0c;     // GPH2,GPH3�ڲ�����

    ULCON0  = 0x03;     // 8N1(8������λ���޽��飬1��ֹͣλ)
    UCON0   = (0x2 << 2) | (0x2 << 0);     // DMA mode��UARTʱ��ԴΪPCLK
    UFCON0  = 0x00;     // ��ʹ��FIFO
    UMCON0  = 0x00;     // ��ʹ������
    UBRDIV0 = UART_BRD; // ������Ϊ115200
}

#if 0
/*
 * ��ʼ��UART
 *
 */
int  Uart_Init(pt_Uart_Config puart_config)
{
    if(puart_config == NULL)
        return -1;
    return Register_Uart(puart_config);

    GPHCON  |= (0x2 << (console_num* 2 + 4)) | (0x2 << (console_num * 2 + 4)) ;    // GPH2,GPH3����TXD0,RXD0
    GPHUP   = (0x1 << (console_num + 2)) | (0x1 << (console_num + 3));     // GPH2,GPH3�ڲ�����

    ULCON0  = 0x03;     // 8N1(8������λ���޽��飬1��ֹͣλ)
    UCON0   = 0x05;     // ��ѯ��ʽ��UARTʱ��ԴΪPCLK
    UFCON0  = 0x00;     // ��ʹ��FIFO
    UMCON0  = 0x00;     // ��ʹ������
    UBRDIV0 = baud_rate; // ������Ϊ115200
}
#endif
/*
 * ��ʼ��UART
 *
 */
int  Uart_Init(unsigned long console_num,unsigned long baud_rate)
{
    GPHCON  |= (0x2 << (console_num* 2 + 4)) | (0x2 << (console_num * 2 + 4)) ;    // GPH2,GPH3����TXD0,RXD0
    GPHUP   = (0x1 << (console_num + 2)) | (0x1 << (console_num + 3));     // GPH2,GPH3�ڲ�����

    ULCON0  = 0x03;     // 8N1(8������λ���޽��飬1��ֹͣλ)
    UCON0   = (0x1 << 7) |(0x2 << 2) | (0x2 << 0) ; // DMA ��ʱ�����ж�,DMA0 style,UARTʱ��ԴΪPCLK
    UFCON0  = (0x3 << 6) | (0x3 << 4) | (0x1 << 0) ;// tx fifo 48,rx fifo 32,enable fifo
    UMCON0  = 0x00;     // ��ʹ������
    UBRDIV0 = baud_rate; // ������Ϊ115200
    return 0;
}

/*
 * ����һ���ַ�
 */
void put_c(unsigned char c)
{
    /* �ȴ���ֱ�����ͻ������е������Ѿ�ȫ�����ͳ�ȥ */
    while (!(UTRSTAT0 & TXD0READY));

    /* ��UTXH0�Ĵ�����д�����ݣ�UART���Զ��������ͳ�ȥ */
    UTXH0 = c;
}

/*
 * �����ַ�
 */
unsigned char get_c(void)
{
    /* �ȴ���ֱ�����ջ������е������� */
    while (!(UTRSTAT0 & RXD0READY));

    /* ֱ�Ӷ�ȡURXH0�Ĵ��������ɻ�ý��յ������� */
    return URXH0;
}

/*
 * �ж�һ���ַ��Ƿ�����
 */
int isDigit(unsigned char c)
{
    if (c >= '0' && c <= '9')
        return 1;
    else
        return 0;
}

/*
 * �ж�һ���ַ��Ƿ�Ӣ����ĸ
 */
int isLetter(unsigned char c)
{
    if (c >= 'a' && c <= 'z')
        return 1;
    else if (c >= 'A' && c <= 'Z')
        return 1;
    else
        return 0;
}

/*
* printf 32bit
*/
void printf_word(unsigned int w_num)
{
	unsigned char uc = 0;
	do{
		uc = w_num & 0xff;
		w_num >>= 8;
		put_c(uc);
	}while(w_num != 0);
}
/*output a string*/
void printf_string(char* ptr)
{
    if(ptr != NULL){
        while(*ptr != '\0')
            put_c(*ptr++);
    }
}

t_Uart_Driver Uart_Oper = {
    .pt_uconfig = NULL,
    .put_c = put_c,
    .get_c = get_c,
};

int Register_Uart(pt_Uart_Driver ptconfig)
{
    ptconfig = ptconfig;
    return 0;
}


