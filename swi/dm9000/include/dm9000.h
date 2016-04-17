#ifndef __DM9000_H__
#define __DM9000_H__

//DM9000 base address for bank4 
#define DM9000_CMD_BASE		(0x20000000)
#define DM9000_DAT_BASE		(0x20000004)

//define the dm9000 contorl and status registers
#define NCR			0x00			//network control register
#define NSR			0x01			//network status register
#define TCR			0x02			//TX control register
#define TSR1		0x03			//TX status register 1
#define TSR2		0x04			//TX status register 2
#define RCR			0x05			//RX control register
#define RSR			0x06			//RX status register
#define ROCR		0x07			//Recive Overflow counter register
#define	BPTR		0x08			//Back pressure threshold register
#define FCTR		0x09			//Flow control threshold register
#define FCR			0x0a			//RX flow control register

#define PAR			0x10			//MAC address 6bytes(10h-15h)
#define GPCR		0x1e			//General Purpose Control Register
#define GPR			0x1f			//General Purpose Register

#define VIDL		0x28			//VENDER ID L
#define VIDH		0x29			//VENDER ID H
#define PIDL		0x2a			//product ID L
#define PIDH		0x2b			//product ID H
#define SMCR		0x2f			//special mode control register

#define MRCMDX		0xf0			//Memery Data Read_address without address increment
#define MRCMD		0xf2			//Memery Data Read_address with address increment
#define MRRL		0xf4			//Memery Data Read_address register low register
#define MRRH		0xf5			//Memery Data Read_address register high register

#define MWCMDX		0xf6			//Memery Data Write_address without address increment
#define MWCMD		0xf8			//Memery Data Write_address with address increment
#define MWRL		0xfa			//Memery Data Write_address register low register
#define MWRH		0xfb			//Memery Data Write_address register high register

#define TXPLL		0xfc			//TX Packet lenth low byte register
#define TXPLH		0xfd			//TX Packet lenth high byte register

#define ISR			0xfe			//Interrupt status register
#define IMR			0xff			//Interrupt mask register

void inline udelay(unsigned int us);
#if 0
//dm9000 write regiser macro
#define DM9000_WRITE_REG(reg,data) \
({\
	udelay(2);\
	DM9000_CMD_BASE = reg;\
	udelay(2);\
	DM9000_DAT_BASE = data;\
	udelay(2);\
})	

//dm9000 read regiser macro
#define DM9000_READ_REG(reg) \
({\
	udelay(2);\
	DM9000_CMD_BASE = reg;\
	udelay(2);\
	DM9000_DAT_BASE;\
})	
#endif
//dm9000 write a byte to a register
#define dm9000_io_outb(reg,value)	(*(volatile unsigned short*)(reg) = (unsigned short)(value))
//dm9000 read a byte from a register
#define dm9000_io_inb(reg)	(*(volatile unsigned short*)(reg))

void dm9000_init( void );
void DM9000_Init(void);

#endif

