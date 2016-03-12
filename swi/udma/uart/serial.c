#include <s3c2440_addr.h>
#include "serial.h"
#include <stdio.h>

/*
 * 初始化UART0
 * 115200,8N1,无流控
 */
void uart0_init(void)
{
    GPHCON  |= 0xa0;    // GPH2,GPH3用作TXD0,RXD0
    GPHUP   = 0x0c;     // GPH2,GPH3内部上拉

    ULCON0  = 0x03;     // 8N1(8个数据位，无较验，1个停止位)
    UCON0   = (0x2 << 2) | (0x2 << 0);     // DMA mode，UART时钟源为PCLK
    UFCON0  = 0x00;     // 不使用FIFO
    UMCON0  = 0x00;     // 不使用流控
    UBRDIV0 = UART_BRD; // 波特率为115200
}

#if 0
/*
 * 初始化UART
 *
 */
int  Uart_Init(pt_Uart_Config puart_config)
{
    if(puart_config == NULL)
        return -1;
    return Register_Uart(puart_config);

    GPHCON  |= (0x2 << (console_num* 2 + 4)) | (0x2 << (console_num * 2 + 4)) ;    // GPH2,GPH3用作TXD0,RXD0
    GPHUP   = (0x1 << (console_num + 2)) | (0x1 << (console_num + 3));     // GPH2,GPH3内部上拉

    ULCON0  = 0x03;     // 8N1(8个数据位，无较验，1个停止位)
    UCON0   = 0x05;     // 查询方式，UART时钟源为PCLK
    UFCON0  = 0x00;     // 不使用FIFO
    UMCON0  = 0x00;     // 不使用流控
    UBRDIV0 = baud_rate; // 波特率为115200
}
#endif
/*
 * 初始化UART
 *
 */
int  Uart_Init(unsigned long console_num,unsigned long baud_rate)
{
    GPHCON  |= (0x2 << (console_num * 2 + 4)) | (0x2 << (console_num * 2 + 4)) ;    // GPH2,GPH3用作TXD0,RXD0
    GPHUP   = (0x1 << (console_num + 2)) | (0x1 << (console_num + 3));     // GPH2,GPH3内部上拉

    ULCON0  = 0x03;     // 8N1(8个数据位，无较验，1个停止位)
    UCON0   = (0x1 << 7) |(0x2 << 2) | (0x2 << 0) ; // DMA 超时接受中断,DMA0 style,UART时钟源为PCLK
    UFCON0  = (0x3 << 6) | (0x3 << 4) | (0x1 << 0) ;// tx fifo 48,rx fifo 32,enable fifo
    UMCON0  = 0x00;     // 不使用流控
    UBRDIV0 = baud_rate; // 波特率为115200
    return 0;
}

/*
 * 发送一个字符
 */
void put_c(unsigned char c)
{
    /* 等待，直到发送缓冲区中的数据已经全部发送出去 */
    while (!(UTRSTAT0 & TXD0READY));

    /* 向UTXH0寄存器中写入数据，UART即自动将它发送出去 */
    UTXH0 = c;
}

/*
 * 接收字符
 */
unsigned char getc(void)
{
    /* 等待，直到接收缓冲区中的有数据 */
    while (!(UTRSTAT0 & RXD0READY));

    /* 直接读取URXH0寄存器，即可获得接收到的数据 */
    return URXH0;
}

/*
 * 判断一个字符是否数字
 */
int isDigit(unsigned char c)
{
    if (c >= '0' && c <= '9')
        return 1;
    else
        return 0;
}

/*
 * 判断一个字符是否英文字母
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
    .get_c = getc,
};

int Register_Uart(pt_Uart_Driver ptconfig)
{
    ptconfig = ptconfig;
    return 0;
}


