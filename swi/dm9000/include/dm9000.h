#ifndef __DM9000_H__
#define __DM9000_H__

#include <io.h>
//DM9000 base address for bank4 
#define DM9000_CMD_BASE		(0x20000000)
#define DM9000_DAT_BASE		(0x20000004)

#define DM9000_ID		0x90000A46	//dm9000 ID
#define DM9000_PKT_MAX		1536	/* Received packet max size */
#define DM9000_PKT_RDY		0x01	/* Packet ready to receive */

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
#define EPCR		0x0b			//EEPROM & PHY Control Register
#define EPAR		0x0c			//EEPROM & PHY Adrress Register
#define EPDRL		0x0d			//EEPROM & PHY Low Byte Data Register
#define EPDRH		0x0e			//EEPROM & PHY High Byte Data Register
#define WCR			0x0f			//Wake Up Control Register

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

#define GPCR_GPIO0_OUT		(1<<0)	//GPIO for output setting
#define GPR_PHY_PWROFF		(1<<0)	//GPR PHY POWER OFF
/*NCR bit setting*/
#define NCR_LBK_INT_MAC		(1<<1)
#define NCR_LBK_INT_PHY		(2<<1)
#define NCR_RST				(1<<0)

/*dm9000 register control command*/
#define DM9000_outb(d,r) writeb(d, (volatile UINT8 *)(r))
#define DM9000_outw(d,r) writew(d, (volatile UINT16 *)(r))
#define DM9000_outl(d,r) writel(d, (volatile UINT32 *)(r))
#define DM9000_inb(r) readb((volatile UINT8 *)(r))
#define DM9000_inw(r) readw((volatile UINT16 *)(r))
#define DM9000_inl(r) readl((volatile UINT32 *)(r))
/*dm9000 function declarations*/
void test_dm9000( void );
void DM9000_Init(void);
void DM9000_sendPacket(char* data_src, unsigned int length );
void inline DM9000_reset( void );
int dm9000_revPacket( unsigned char* data_src );
void sdbinit(void);

#endif

