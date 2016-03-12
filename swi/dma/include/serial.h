#ifndef SERIAL_H_
#define SERIAL_H_

#ifdef	__cplusplus
extern "C"{
#endif

/*UART conctrol register config*/
typedef struct {
    unsigned long ULCON;
    unsigned long UCON;
    unsigned long UFCON;
    unsigned long UMCON;
    unsigned long UTRSTAT;
    unsigned long UERSTAT;
    unsigned long UFSTAT;
    unsigned long UMSTAT;
    unsigned long UTXH;
    unsigned long URXH;
    unsigned long UBRDIV;
}t_Uart_Config,*pt_Uart_Config;

/*UART Driver*/
typedef struct _uart_driver{
    pt_Uart_Config pt_uconfig;
    void (*put_c)(unsigned char c);
    unsigned char (*get_c)(void);
}t_Uart_Driver,*pt_Uart_Driver;

#define TXD0READY   (1<<2)
#define RXD0READY   (1)
#define PCLK            50000000    // init.c中的clock_init函数设置PCLK为50MHz
#define UART_CLK        PCLK        //  UART0的时钟源设为PCLK
#define UART_BAUD_RATE  115200      // 波特率
#define UART_BRD        ((UART_CLK  / (UART_BAUD_RATE * 16)) - 1)

void uart0_init(void);
void put_c(unsigned char c);
unsigned char get_c(void);
int isDigit(unsigned char c);
int isLetter(unsigned char c);
void printf_word(unsigned int w_num);
void printf_string(char* ptr);

#ifdef	__cplusplus
}
#endif

#endif
