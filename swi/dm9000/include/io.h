#ifndef IO_H_
#define IO_H_
/*write command */
#define writeb(b, addr) (void)((*(volatile unsigned char *) (addr)) = (b))
#define writew(b, addr) (void)((*(volatile unsigned short *) (addr)) = (b))
#define writel(b, addr) (void)((*(volatile unsigned int *) (addr)) = (b))
/*read command */
#define readb(addr) (*(volatile unsigned char *) (addr))
#define readw(addr) (*(volatile unsigned short *) (addr))
#define readl(addr) (*(volatile unsigned int *) (addr))

#endif

