#ifndef __DM9000_H__
#define __DM9000_H__
#include <types.h>
#include <io.h>
#include <net.h>
#include <bsp.h>
#include <stdio.h>
//DM9000 base address for bank4 
#define DM9000_CMD_BASE		(0x20000000)
#define DM9000_DAT_BASE		(0x20000004)

#define DM9000_ID		0x90000A46	//dm9000 ID
#define DM9000_PKT_MAX		1536	/* Received packet max size */
#define DM9000_PKT_RDY		0x01	/* Packet ready to receive */
/* IO control flags */
#define DM9000_PLATF_8BITONLY	(0x0001)
#define DM9000_PLATF_16BITONLY	(0x0002)
#define DM9000_PLATF_32BITONLY	(0x0004)
#define DM9000_PLATF_EXT_PHY	(0x0008)
#define DM9000_PLATF_NO_EEPROM	(0x0010)
#define DM9000_PLATF_SIMPLE_PHY (0x0020)  /* Use NSR to find LinkStatus */

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

#define PAR			0x10			//Physical Address Register
#define MAR			0x16			//Multicast Address Register
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
/*BPTR setting*/
#define BPTR_BPHW(x)		((x) << 4)
#define BPTR_JPT_200US		(0x07)
#define BPTR_JPT_600US		(0x0f)
/*FCTR setting*/
#define FCTR_HWOT(ot)		(( ot & 0xf ) << 4 )
#define FCTR_LWOT(ot)		( ot & 0xf )
/*NSR setting*/
#define NSR_SPEED		(1<<7)
#define NSR_LINKST		(1<<6)
#define NSR_WAKEST		(1<<5)
#define NSR_TX2END		(1<<3)
#define NSR_TX1END		(1<<2)
#define NSR_RXOV		(1<<1)
/*TCR setting*/
#define TCR_TJDIS		(1<<6)
#define TCR_EXCECM		(1<<5)
#define TCR_PAD_DIS2	(1<<4)
#define TCR_CRC_DIS2	(1<<3)
#define TCR_PAD_DIS1	(1<<2)
#define TCR_CRC_DIS1	(1<<1)
#define TCR_TXREQ		(1<<0)
/*TSR setting*/
#define TSR_TJTO		(1<<7)
#define TSR_LC			(1<<6)
#define TSR_NC			(1<<5)
#define TSR_LCOL		(1<<4)
#define TSR_COL			(1<<3)
#define TSR_EC			(1<<2)
/*ISR setting*/
#define ISR_ROOS		(1<<3)
#define ISR_ROS			(1<<2)
#define ISR_PTS			(1<<1)
#define ISR_PRS			(1<<0)
/*RCR setting*/
#define RCR_WTDIS		(1<<6)
#define RCR_DIS_LONG	(1<<5)
#define RCR_DIS_CRC		(1<<4)
#define RCR_ALL			(1<<3)
#define RCR_RUNT		(1<<2)
#define RCR_PRMSC		(1<<1)
#define RCR_RXEN		(1<<0)
/*IMR setting*/
#define IMR_PAR			(1<<7)
#define IMR_ROOM		(1<<3)
#define IMR_ROM			(1<<2)
#define IMR_PTM			(1<<1)
#define IMR_PRM			(1<<0)
/* PHY address 0x01 */
#define PHY				0x40	
/*dm9000 platform resource*/
struct dm9000_plat_data {
	unsigned int	flags;
	unsigned char	dev_addr[6];

	/* allow replacement IO routines */
	void	(*inblk)(void  *reg, void *data, int len);
	void	(*outblk)(void  *reg, void *data, int len);
	void	(*dumpblk)(void  *reg, int len);
};
/* #define CONFIG_DM9000_DEBUG */

#ifdef CONFIG_DM9000_DEBUG
#define DM9000_DBG(fmt,args...) printf(fmt, ##args)
#define DM9000_DMP_PACKET(func,packet,length)  \
	do { \
		int i; 							\
		printf("%s: length: %d\n", func, length);		\
		for (i = 0; i < length; i++) {				\
			if (i % 8 == 0)					\
				printf("\n%s: %02x: ", func, i);	\
			printf("%02x ", ((unsigned char *) packet)[i]);	\
		} printf("\n");						\
	} while(0)
#else
#define DM9000_DBG(fmt,args...)
#define DM9000_DMP_PACKET(func,packet,length)
#endif

/*dm9000 register control command*/
#define DM9000_outb(d,r) writeb(d, (volatile UINT8 *)(r))
#define DM9000_outw(d,r) writew(d, (volatile UINT16 *)(r))
#define DM9000_outl(d,r) writel(d, (volatile UINT32 *)(r))
#define DM9000_inb(r) readb((volatile UINT8 *)(r))
#define DM9000_inw(r) readw((volatile UINT16 *)(r))
#define DM9000_inl(r) readl((volatile UINT32 *)(r))
/*dm9000 function declarations*/
void test_dm9000( void );
int DM9000_Init(void);
int DM9000_sendPacket(void* data_src, unsigned int length );
void inline DM9000_reset( void );
int dm9000_initialize(bd_t *bis);

#endif

