#include <dm9000.h>
#include <s3c2440_addr.h>
#include <stdio.h>
#include <intirq.h>
#include <timer.h>
#include <miscellaneous.h>
#include <net.h>
#include <glob.h>
#include <bsp.h>
#include <platform.h>
#include <string.h>
#include <io.h> 
#include <test.h>

/*dm9000 recieve packet structure*/
typedef struct dm9000_rxPacket{
	unsigned char rxbuf[DM9000_PKT_MAX];
	unsigned int size;
}t_dm9000_rxPacket;

typedef t_dm9000_rxPacket* ElementType ;
#define MAX_RXBUF_SIZE	20
#include <queue.h>

/*glob platform devices*/
extern t_platform_device *gt_platform_device;
/*dm9000 queue variable*/
static t_Queue dm9000_queue;
/*The dm9000 iterrupt happends conter*/
static unsigned int g_dm9000_isr_count;
/*mac address*/
static unsigned char mac_addr[6] __attribute__ ((__aligned__(4)))= {0x1a,0x2b,0x3c,0x4d,0x5e,0x6f};
/*dm9000 recieve buffer queue*/
static struct dm9000_rxPacket* dm9000_rxbuf[MAX_RXBUF_SIZE] = {0};
static struct dm9000_rxPacket dm9000_pakects[MAX_RXBUF_SIZE] = {0};
/*dm9000 board infomation*/
static t_board_info dm9000_info;
static unsigned long dm9000_cmd_base = DM9000_CMD_BASE;
static unsigned long dm9000_data_base = DM9000_DAT_BASE;
/*****************************************************************************
 函 数 名  : void  udelay_us(int us)
 功能描述  : just for udelay some times about xx us,FCK = 200MHZ = 5ns.
 输入参数  : us
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年4月5日
    作    者   : QSWWD
    修改内容   : 新生成函数

*****************************************************************************/
static void udelay_us(unsigned int us)
{
    int i;
	while(us--)
		for(i = 0 ;i < 190;i++);		
}
/*****************************************************************************
 函 数 名  : udelay_ms
 功能描述  : delay some ms
 输入参数  : unsigned int ms  
 输出参数  : 无
 返 回 值  : static
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年9月21日
    作    者   : QSWWD
    修改内容   : 新生成函数

*****************************************************************************/
static void udelay_ms(unsigned int ms)
{
    int i;
	while(ms--)
		for(i = 0 ;i < 200000;i++);		
}

/*****************************************************************************
 函 数 名  : dm9000_outblk_8bit
 功能描述  : dm9000按字节输出
 输入参数  : volatile void *data_ptr
             int count
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年9月17日
    作    者   : haikong
    修改内容   : 新生成函数

*****************************************************************************/
static void dm9000_outblk_8bit( volatile void *data_ptr, int count )
{
	int i;
	for (i = 0; i < count; i++)
		DM9000_outb((((unsigned char*) data_ptr)[i] & 0xff), dm9000_data_base);    
}
/*****************************************************************************
 函 数 名  : dm9000_outblk_16bit
 功能描述  : dm9000按字输出
 输入参数  : volatile void *data_ptr
             int count
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年9月17日
    作    者   : haikong
    修改内容   : 新生成函数

*****************************************************************************/
static void dm9000_outblk_16bit( volatile void *data_ptr, int count )
{
	int i;
	unsigned int tmplen = (count + 1) / 2;
	
	for (i = 0; i < tmplen; i++)
		DM9000_outw((((unsigned short*) data_ptr)[i]), dm9000_data_base);    
}

/*****************************************************************************
 函 数 名  : dm9000_outblk_32bit
 功能描述  : dm9000按2字输出
 输入参数  : volatile void *data_ptr
             int count
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年9月17日
    作    者   : haikong
    修改内容   : 新生成函数

*****************************************************************************/
static void dm9000_outblk_32bit( volatile void *data_ptr, int count )
{
	int i;
	unsigned int tmplen = (count + 3) / 4;
	
	for (i = 0; i < tmplen; i++)
		DM9000_outl((((unsigned int*) data_ptr)[i]), dm9000_data_base);    
}
/*****************************************************************************
 函 数 名  : dm9000_inblk_8bit
 功能描述  : dm9000按字节输入
 输入参数  : volatile void *data_ptr
             int count
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年9月17日
    作    者   : haikong
    修改内容   : 新生成函数

*****************************************************************************/
static void dm9000_inblk_8bit(void *data_ptr, int count)
{
	int i;
	for (i = 0; i < count; i++)
		((UINT8*) data_ptr)[i] = DM9000_inb(dm9000_data_base);
}
/*****************************************************************************
 函 数 名  : dm9000_inblk_16bit
 功能描述  : dm9000按半字输入
 输入参数  : volatile void *data_ptr
             int count
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年9月17日
    作    者   : haikong
    修改内容   : 新生成函数

*****************************************************************************/
static void dm9000_inblk_16bit(void *data_ptr, int count)
{
	int i;
	UINT32 tmplen = (count + 1) / 2;

	for (i = 0; i < tmplen; i++)
		((UINT16 *) data_ptr)[i] = DM9000_inw(dm9000_data_base);
}
/*****************************************************************************
 函 数 名  : dm9000_inblk_32bit
 功能描述  : dm9000按字输入
 输入参数  : volatile void *data_ptr
             int count
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年9月17日
    作    者   : haikong
    修改内容   : 新生成函数

*****************************************************************************/
static void dm9000_inblk_32bit(void *data_ptr, int count)
{
	int i;
	UINT32 tmplen = (count + 3) / 4;

	for (i = 0; i < tmplen; i++)
		((UINT32 *) data_ptr)[i] = DM9000_inl(dm9000_data_base);
}

/* dump block from chip to null */
static void dm9000_dumpblk_8bit(int count)
{
	int i;
	int tmp;

	for (i = 0; i < count; i++)
		tmp = DM9000_inb(dm9000_data_base);
}

static void dm9000_dumpblk_16bit(int count)
{
	int i;
	int tmp;

	count = (count + 1) >> 1;

	for (i = 0; i < count; i++)
		tmp = DM9000_inw(dm9000_data_base);
}

static void dm9000_dumpblk_32bit(int count)
{
	int i;
	int tmp;

	count = (count + 3) >> 2;

	for (i = 0; i < count; i++)
		tmp = DM9000_inl(dm9000_data_base);
}

/*****************************************************************************
 函 数 名  : dm9000_rx_status_8bit
 功能描述  : dm9000按字节读取状态字
 输入参数  : UINT16 *RxStatus :状态字指针
             UINT16 *RxLen	  :长度
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年9月17日
    作    者   : haikong
    修改内容   : 新生成函数

*****************************************************************************/
static void dm9000_rx_status_8bit(UINT16 *RxStatus, UINT16 *RxLen)
{
	DM9000_outb(MRCMD, dm9000_cmd_base);

	*RxStatus =
	    __le16_to_cpu(DM9000_inb(dm9000_data_base) +
			  (DM9000_inb(dm9000_data_base) << 8));
	*RxLen =
	    __le16_to_cpu(DM9000_inb(dm9000_data_base) +
			  (DM9000_inb(dm9000_data_base) << 8));
}
/*****************************************************************************
 函 数 名  : dm9000_rx_status_16bit
 功能描述  : dm9000按半字读取状态字
 输入参数  : UINT16 *RxStatus :状态字指针
             UINT16 *RxLen	  :长度
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年9月17日
    作    者   : haikong
    修改内容   : 新生成函数

*****************************************************************************/
static void dm9000_rx_status_16bit(UINT16 *RxStatus, UINT16 *RxLen)
{
	DM9000_outb(MRCMD, dm9000_cmd_base);

	*RxStatus =__le16_to_cpu(DM9000_inw(dm9000_data_base));
	*RxLen =__le16_to_cpu(DM9000_inw(dm9000_data_base));
}
/*****************************************************************************
 函 数 名  : dm9000_rx_status_32bit
 功能描述  : dm9000按半读取状态字
 输入参数  : UINT16 *RxStatus :状态字指针
             UINT16 *RxLen	  :长度
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年9月17日
    作    者   : haikong
    修改内容   : 新生成函数

*****************************************************************************/
static void dm9000_rx_status_32bit(UINT16 *RxStatus, UINT16 *RxLen)
{
	UINT32 tmpdata;

	DM9000_outb(MRCMD, dm9000_cmd_base);
	tmpdata = DM9000_inl(dm9000_data_base);

	*RxStatus =__le16_to_cpu(tmpdata);
	*RxLen =__le16_to_cpu(tmpdata >> 16);
}

/*****************************************************************************
 函 数 名  : DM9000_iow
 功能描述  : setting dm9000 registers
 输入参数  : reg
             data
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年4月11日
    作    者   : QSWWD
    修改内容   : 新生成函数

*****************************************************************************/
static void inline DM9000_iow(unsigned long reg,char data )
{
	DM9000_outb(reg,dm9000_cmd_base);
	DM9000_outb(data,dm9000_data_base);
}

/*****************************************************************************
 函 数 名  : DM9000_ior
 功能描述  : reading dm9000 register content
 输入参数  : reg
 输出参数  : 无
 返 回 值  : int
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年4月11日
    作    者   : QSWWD
    修改内容   : 新生成函数

*****************************************************************************/
static unsigned char inline DM9000_ior(unsigned long reg )
{
    DM9000_outb(reg,dm9000_cmd_base);
	return DM9000_inb(dm9000_data_base);
}

/*****************************************************************************
 函 数 名  : DM9000_iows
 功能描述  : setting dm9000 registers
 输入参数  : reg
             data
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年4月11日
    作    者   : QSWWD
    修改内容   : 新生成函数

*****************************************************************************/
static void inline DM9000_iows(unsigned long reg,short data )
{
	DM9000_outw(reg,dm9000_cmd_base);
	DM9000_outw(data,dm9000_data_base);
}
/*****************************************************************************
 函 数 名  : dm9000_read_srom_word
 功能描述  : Read a word data from SROM
 输入参数  : int offset  
             UINT8*to    
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年9月21日
    作    者   : QSWWD
    修改内容   : 新生成函数

*****************************************************************************/
#if !defined(CONFIG_DM9000_NO_SROM)
void dm9000_read_srom_word(int offset, UINT8*to)
{
	DM9000_iow(EPAR, offset);
	DM9000_iow(EPCR, 0x4);
	udelay_us(8000);
	DM9000_iow(EPCR, 0x0);
	to[0] = DM9000_ior(EPDRL);
	to[1] = DM9000_ior(EPDRH);
}
/*****************************************************************************
 函 数 名  : dm9000_write_srom_word
 功能描述  : write a word data from SROM
 输入参数  : int offset  
             UINT8*to    
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年9月21日
    作    者   : QSWWD
    修改内容   : 新生成函数

*****************************************************************************/
void dm9000_write_srom_word(int offset, UINT16 val)
{
	DM9000_iow(EPAR, offset);
	DM9000_iow(EPDRH, ((val >> 8) & 0xff));
	DM9000_iow(EPDRL, (val & 0xff));
	DM9000_iow(EPCR, 0x12);
	udelay_us(8000);
	DM9000_iow(EPCR, 0);
}
#endif

/*****************************************************************************
 函 数 名  : get_dm9000_resoures
 功能描述  : gain dm9000 devices' resoures
 输入参数  :   
 输出参数  : 无
 返 回 值  : 0:succeed
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年10月23日
    作    者   : QSWWD
    修改内容   : 新生成函数

*****************************************************************************/
static int get_dm9000_resoures(void)
{
	int i,num,ret = 0;
	t_platform_device *pdev;
	struct dm9000_plat_data* resoure;
	pdev = gt_platform_device;
	/*select the devices*/
	while(pdev != NULL)
	{
		if(0 == strcmp(pdev->name,"dm9000"))
			break;
		pdev = pdev->next;
	}
	/*no such the devices*/
	if(NULL == pdev)
	{
		ret =  -1;
		goto tail;
	}
	/*gain dm9000 devices resource's number*/
	num = pdev->num_resources;
	if(num < 1)
	{
		ret =  -1;
		goto tail;
	}
	for(i = 0;i < num;i++)
	{
		if((i == 0) && (pdev->resource[i].flags == IORESOURCE_MEM))
		{
			dm9000_info.netdev.iobase = pdev->resource[i].start;
		}
		else if((pdev->resource[i].flags & IORESOURCE_IRQ) == IORESOURCE_IRQ)
		{
			dm9000_info.netdev.int_base = pdev->resource[i].start;
			dm9000_info.netdev.int_state = pdev->resource[i].flags;
		}
	}
	
	dm9000_info.netdev.priv = pdev->private_data;
	resoure = (struct dm9000_plat_data*)dm9000_info.netdev.priv;
	if(resoure == NULL)
	{
		memcpy(dm9000_info.netdev.enetaddr,mac_addr,6);
		dm9000_info.netdev.flag	= (DM9000_PLATF_16BITONLY | DM9000_PLATF_NO_EEPROM);
	}
	else
	{
		memcpy(dm9000_info.netdev.enetaddr,resoure->dev_addr,6);
		dm9000_info.netdev.flag = resoure->flags;
	}
	snprintf(dm9000_info.netdev.name,16,pdev->name);
	dm9000_cmd_base = dm9000_info.netdev.iobase;
	dm9000_data_base = dm9000_info.netdev.iobase + 4;	
tail:	
	return ret;
}
/*****************************************************************************
 函 数 名  : dm9000_get_enetaddr
 功能描述  : get dm9000 mac addree 
 输入参数  : struct eth_device *dev  
 输出参数  : 无
 返 回 值  : static
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年9月21日
    作    者   : QSWWD
    修改内容   : 新生成函数

*****************************************************************************/
static void dm9000_get_enetaddr(struct eth_device *dev)
{
#if !defined(CONFIG_DM9000_NO_SROM)
	int i;
	for (i = 0; i < 3; i++)
		dm9000_read_srom_word(i, dev->enetaddr + (2 * i));
#endif
}

/*****************************************************************************
 函 数 名  : DM9000_reset
 功能描述  : dm9000 reset function
 输入参数  : void
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年4月11日
    作    者   : QSWWD
    修改内容   : 新生成函数

*****************************************************************************/
void inline DM9000_reset( void )
{

	DM9000_DBG("resetting DM9000...\n\r");

	/* DEBUG: Make all GPIO0 outputs, all others inputs */
	DM9000_iow(GPCR, GPCR_GPIO0_OUT);
	/* Step 1: Power internal PHY by writing 0 to GPIO0 pin */
	DM9000_iow(GPR, 0);
	/* Step 2: Software reset */
	DM9000_iow(NCR, (NCR_LBK_INT_MAC | NCR_RST));

	do {
		//printf("resetting the DM9000, 1st reset...\n\t");
		udelay_us(25); /* Wait at least 20 us */
	} while (DM9000_ior(NCR) & 1);

	DM9000_iow(NCR, 0);
	DM9000_iow(NCR, (NCR_LBK_INT_MAC | NCR_RST)); /* Issue a second reset */

	do {
		//printf("resetting the DM9000, 2nd reset...\n\t");
		udelay_us(25); /* Wait at least 20 us */
	} while (DM9000_ior(NCR) & 1);

	/* Check whether the ethernet controller is present */
	if ((DM9000_ior(PIDL) != 0x0) ||
		(DM9000_ior(PIDH) != 0x90))
		DM9000_DBG("ERROR: resetting DM9000 : not responding!\n\r");
  
}

/*****************************************************************************
 函 数 名  : dm9000_send
 功能描述  :   Hardware start transmission.
 输入参数  : struct eth_device *netdev  
             void *packet               
             int length                 
 输出参数  : 无
 返 回 值  : static
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2017年4月8日
    作    者   : QSWWD
    修改内容   : 新生成函数

*****************************************************************************/
static int dm9000_send(void *packet,unsigned int length)
{
	int tmo;
	struct board_info *db = &dm9000_info;

	DM9000_DMP_PACKET(__func__ , packet, length);

	DM9000_iow(ISR, IMR_PTM); /* Clear Tx bit in ISR */

	/* Move data to DM9000 TX RAM */
	DM9000_outb(MWCMD, dm9000_cmd_base); /* Prepare for TX-data */

	/* push the data to the TX-fifo */
	(db->outblk)(packet, length);

	/* Set TX length to DM9000 */
	DM9000_iow(TXPLL, length & 0xff);
	DM9000_iow(TXPLH, (length >> 8) & 0xff);

	/* Issue TX polling command */
	DM9000_iow(TCR, TCR_TXREQ); /* Cleared after TX complete */

	/* wait for end of transmission:0.5s */
	tmo = get_timer2_counter() + 5 * 1;
	while ( !(DM9000_ior(NSR) & (NSR_TX1END | NSR_TX2END)) ||
		!(DM9000_ior(ISR) & IMR_PTM) ) {
		if (get_timer2_counter() >= tmo) {
			DM9000_DBG("transmission timeout\n");
			break;
		}
	}
	DM9000_iow(ISR, IMR_PTM); /* Clear Tx bit in ISR */

	DM9000_DBG("transmit done\n\n");
	return 0;
}

/*****************************************************************************
 函 数 名  : dm9000_revPacket
 功能描述  : dm9000 recieve packets
 输入参数  : data_src
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年4月17日
    作    者   : QSWWD
    修改内容   : 新生成函数

*****************************************************************************/

static int dm9000_revPacket(unsigned char* data_src )
{
	ElementType rxqueue = NULL;
	unsigned short RX_length=0;//接收数据包的长度
	rxqueue = FrontQueue(&dm9000_queue);
	if(rxqueue != NULL)
	{
		memcpy(data_src,rxqueue->rxbuf,rxqueue->size);
		RX_length = rxqueue->size;
		//DM9000_DMP_PACKET(__FUNCTION__,data_src,RX_length);
	}
	return RX_length ;	
}

/*****************************************************************************
 函 数 名  : dm9000_rx
 功能描述  : the dm9000 recieve pakets function
 输入参数  : void  
 输出参数  : 无
 返 回 值  : -1:error,0:ok
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2017年5月4日
    作    者   : QSWWD
    修改内容   : 新生成函数

*****************************************************************************/
static int dm9000_rx(void)
{
	unsigned char rxbyte;
	unsigned char GoodPacket;
	unsigned short RX_status;
	unsigned short RxLen;
	struct board_info *db = &dm9000_info;
	/* Check packet ready or not */
	do {
		DM9000_inb(MRCMDX);	/* Dummy read */
		/* Get most updated data */
		rxbyte = DM9000_inb(dm9000_data_base) & 0x03;
		DM9000_DBG("rxbyte = %x\n\r",rxbyte);
		/* Status check: this byte must be 0 or 1 */
		if (rxbyte > DM9000_PKT_RDY) 
		{
			DM9000_iow(RCR, 0x00);	/* Stop Device */
			DM9000_DBG("DM9000 error: status check fail: 0x%x\n\r",rxbyte);
			return -1;
		}
		if (rxbyte != DM9000_PKT_RDY)
		{
			//DM9000_iow(ISR, 0x01);	/* Stop INT request */
			//DM9000_iow(IMR, 0x81);
			return -1; /* No packet received, ignore */
		}
		/* A packet ready now  & Get status/length */
		GoodPacket = 1;
		//DM9000_iow(MRCMD, dm9000_cmd_base);

		(db->rx_status)(&RX_status, &RxLen);
		DM9000_DBG("RX_status = %x,RxLen = %d\n\r",RX_status,RxLen);
		/* Packet Status check */
		if ((RX_status & 0xbf00) 
			|| (RxLen < 0x40)
			|| (RxLen > DM9000_PKT_MAX)
			) 
		{
			GoodPacket = 0;
			if (RX_status & 0x100) {
				DM9000_DBG("rx fifo error\n\r");
			}
			if (RX_status & 0x200) {
				DM9000_DBG("rx crc error\n\r");
			}
			if (RX_status & 0x8000) {
				DM9000_DBG("rx length error\n\r");
			}
			if (RxLen > DM9000_PKT_MAX) {
				DM9000_DBG("rx length too big\n\r");
				DM9000_reset();
			}

		}
		/* Move data from DM9000 */
		if (GoodPacket) {
			/* Read received packet from RX SRAM */
			(db->inblk)(&dm9000_pakects[dm9000_queue.Rear].rxbuf, RxLen);
			dm9000_pakects[dm9000_queue.Rear].size = RxLen;
			Enqueue(&dm9000_pakects[dm9000_queue.Rear],&dm9000_queue);
		} else {
			/* need to dump the packet's data */
			(db->dumpblk)(RxLen);
		}
		break;
	} while (rxbyte & DM9000_PKT_RDY);

	return RxLen;
}
/*****************************************************************************
 函 数 名  : dm9000_isr
 功能描述  : dm9000 interrput service program
 输入参数  : void
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年4月19日
    作    者   : haikong
    修改内容   : 新生成函数

*****************************************************************************/

static void dm9000_isr(unsigned int vector)
{
	g_dm9000_isr_count++;
	/*clear the interrput bit*/	
	DM9000_DBG("%s-%d:g_dm9000_isr_count=%d\n\r",__FUNCTION__,__LINE__,g_dm9000_isr_count);
	dm9000_rx();
	/* clear PR status latched in bit 0 */	
	DM9000_iow(ISR, 0x01);
	DM9000_iow(IMR, 0x81);
}

/*****************************************************************************
 函 数 名  : dm9000_gpio_init
 功能描述  : dm9000 some needed gpio initialize
 输入参数  : void
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年4月19日
    作    者   : QSWWD
    修改内容   : 新生成函数

*****************************************************************************/
static int dm9000_gpio_init( void )
{
	int ret;
    //setting dm9000 extern interrupt
	key_init(dm9000_info.netdev.int_base,EXTINT);
	//setting EINT7 is Rising edge triggered
	EXTINT0 &=  ~(dm9000_info.netdev.int_base << 28);
	EXTINT0 |= ((dm9000_info.netdev.int_state & 0xf)<< 28);
	//register EINT7 ISR
	ret = register_extern_int(dm9000_info.netdev.int_base,dm9000_isr);
	return ret;
}

/*****************************************************************************
 函 数 名  : dm9000_phy_read
 功能描述  : Read a word from phyxcer
 输入参数  : int reg
 输出参数  : 无
 返 回 值  : phy read data
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年8月31日
    作    者   : QSWWD
    修改内容   : 新生成函数

*****************************************************************************/
static unsigned short dm9000_phy_read( int reg )
{
	unsigned short val;

	/* Fill the phyxcer register into REG_0C */
	DM9000_iow(EPAR, PHY | reg);
	DM9000_iow(EPCR, 0xc);	/* Issue phyxcer read command */
	udelay_us(100);			/* Wait read complete */
	DM9000_iow(EPCR, 0x0);	/* Clear phyxcer read command */
	val = (DM9000_ior(EPDRH) << 8) | DM9000_ior(EPDRL);

	/* The read data keeps on REG_0D & REG_0E */
	//printf("dm9000_phy_read(0x%x): 0x%x\n\r", reg, val);
	return val; 
}

/*****************************************************************************
 函 数 名  : static int dm9000_probe(void)
 功能描述  : Search DM9000 board, allocate space and register it
 输入参数  : 
 输出参数  : 无
 返 回 值  : 0:ok;-1:error
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年8月30日
    作    者   : QSWWD
    修改内容   : 新生成函数

*****************************************************************************/
static int dm9000_probe(void)
{
	unsigned int id_val,ret;
	id_val = DM9000_ior(VIDL);
	id_val |= DM9000_ior(VIDH) << 8;
	id_val |= DM9000_ior(PIDL) << 16;
	id_val |= DM9000_ior(PIDH) << 24;
	if (id_val == DM9000_ID) {
		DM9000_DBG("dm9000 i/o: 0x%x, id: 0x%x .\n\r", (unsigned int)dm9000_cmd_base,id_val);
		ret =  0;
	} else {
		DM9000_DBG("dm9000 not found at 0x%08x id: 0x%08x.\n\r",(unsigned int)dm9000_cmd_base, id_val);
		ret =  -1;
	}
	return ret;
}
/*****************************************************************************
 函 数 名  : dm9000_phy_write
 功能描述  : Write a word to phyxcer
 输入参数  : int reg       
             UINT16 value  
 输出参数  : 无
 返 回 值  : static
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年9月21日
    作    者   : QSWWD
    修改内容   : 新生成函数

*****************************************************************************/
static void dm9000_phy_write(int reg, UINT16 value)
{

	/* Fill the phyxcer register into REG_0C */
	DM9000_iow(EPAR, PHY | reg);

	/* Fill the written data into REG_0D & REG_0E */
	DM9000_iow(EPDRL, (value & 0xff));
	DM9000_iow(EPDRH, ((value >> 8) & 0xff));
	DM9000_iow(EPCR, 0xa);	/* Issue phyxcer write command */
	udelay_us(500);			/* Wait write complete */
	DM9000_iow(EPCR, 0x0);	/* Clear phyxcer write command */
	DM9000_DBG("dm9000_phy_write(reg:0x%x, value:0x%x)\n\r", reg, value);
}

/*****************************************************************************
 函 数 名  : dm9000_halt
 功能描述  : Stop the interface.The interface is stopped when it is brought.
 输入参数  : struct eth_device *netdev  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年9月21日
    作    者   : QSWWD
    修改内容   : 新生成函数

*****************************************************************************/
static void dm9000_halt(void)
{
	/* RESET devie */
	dm9000_phy_write(0, 0x8000);	/* PHY RESET */
	DM9000_iow(GPR, 0x01);	/* Power-Down PHY */
	DM9000_iow(IMR, 0x80);	/* Disable all interrupt */
	DM9000_iow(RCR, 0x00);	/* Disable RX */
}

/*****************************************************************************
 函 数 名  : dm9000_init
 功能描述  : dm9000 device initialize
	//1.将相关的管脚配置好
	//2.将相关的寄存器配置好

	//初始化寄存器主要做的事情：
	//1.复位
	//2.清楚原先的状态
	//3.对发射、接收进行控制
	//4.使能中断
	//在设置时，有些地方需要重复设置。
 输入参数  : void
 输出参数  : 无
 返 回 值  : 0:ok;-1:error
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年3月30日
    作    者   : haikong
    修改内容   : 新生成函数

*****************************************************************************/

int DM9000_Init(void)
{
	unsigned char i,io_mode,oft,lnk;
	int ret;
	struct board_info *db = &dm9000_info;
	if(db == NULL)
		return -1;
	/* RESET device */
	DM9000_reset();
	if(dm9000_probe() < 0)
		return -1;	
	/* Auto-detect 8/16/32 bit mode, ISR Bit 6+7 indicate bus width */
	io_mode = DM9000_ior(ISR) >> 6;
	switch(io_mode){
		case 0x0:  /* 16-bit mode */
			DM9000_DBG("DM9000: running in 16 bit mode\n\r");
			db->dumpblk	 = dm9000_dumpblk_16bit;
			db->outblk    = dm9000_outblk_16bit;
			db->inblk     = dm9000_inblk_16bit;
			db->rx_status = dm9000_rx_status_16bit;
			break;
		case 0x01:  /* 32-bit mode */
			DM9000_DBG("DM9000: running in 32 bit mode\n\r");
			db->dumpblk	 = dm9000_dumpblk_32bit;
			db->outblk    = dm9000_outblk_32bit;
			db->inblk     = dm9000_inblk_32bit;
			db->rx_status = dm9000_rx_status_32bit;
			break;
		case 0x02: /* 8 bit mode */
			DM9000_DBG("DM9000: running in 8 bit mode\n\r");
			db->dumpblk	 = dm9000_dumpblk_8bit;
			db->outblk    = dm9000_outblk_8bit;
			db->inblk     = dm9000_inblk_8bit;
			db->rx_status = dm9000_rx_status_8bit;
			break;
		default:
			/* Assume 8 bit mode, will probably not work anyway */
			DM9000_DBG("DM9000: Undefined IO-mode:0x%x\n\r", io_mode);
			db->dumpblk	 = dm9000_dumpblk_8bit;
			db->outblk    = dm9000_outblk_8bit;
			db->inblk     = dm9000_inblk_8bit;
			db->rx_status = dm9000_rx_status_8bit;
			break;
	}
	/* Program operating register, only internal phy supported */
	DM9000_iow(NCR, 0x0);
	/* TX Polling clear */
	DM9000_iow(TCR, 0);
	/* Less 3Kb, 200us */
	DM9000_iow(BPTR, BPTR_BPHW(3) | BPTR_JPT_600US);
	/* Flow Control : High/Low Water */
	DM9000_iow(FCTR, FCTR_HWOT(3) | FCTR_LWOT(8));
	/* SH FIXME: This looks strange! Flow Control */
	DM9000_iow(FCR, 0x0);
	/* Special Mode */
	DM9000_iow(SMCR, 0);
	/* clear TX status */
	DM9000_iow(NSR, NSR_WAKEST | NSR_TX2END | NSR_TX1END);
	/* Clear interrupt status */
	DM9000_iow(ISR, ISR_ROOS | ISR_ROS | ISR_PTS | ISR_PRS);

	/* fill device MAC address registers */
	for (i = 0, oft = PAR; i < 6; i++, oft++)
		DM9000_iow(oft, db->netdev.enetaddr[i]);
	
	for (i = 0, oft = MAR; i < 8; i++, oft++)
		DM9000_iow(oft, 0xff);
	/* Activate DM9000 */
	/* RX enable */
	DM9000_iow(RCR, RCR_DIS_LONG | RCR_DIS_CRC | RCR_RXEN);
	/* Enable TX/RX interrupt mask */
	DM9000_iow(IMR, IMR_PAR);
	/* autonegation complete bit */
	i = 0;
	while (!(dm9000_phy_read(1) & 0x20)) {	
		udelay_ms(10);
		i++;
		if (i == 10000) {
			DM9000_DBG("could not establish link\n\r");
			return -1;
		}
	}

	/* see what we've got */
	lnk = dm9000_phy_read(17) >> 12;
	DM9000_DBG("The DM9000 operating at ");
	switch (lnk) {
	case 1:
		DM9000_DBG("10M half duplex ");
		break;
	case 2:
		DM9000_DBG("10M full duplex ");
		break;
	case 4:
		DM9000_DBG("100M half duplex ");
		break;
	case 8:
		DM9000_DBG("100M full duplex ");
		break;
	default:
		DM9000_DBG("unknown: %d ", lnk);
		break;
	}
	DM9000_DBG("mode.\n\r");	
	//setting dm9000 ISR
	ret = dm9000_gpio_init();
	if(ret)
		return -1;
}

/*****************************************************************************
 函 数 名  : dm9000_initialize
 功能描述  : initializs the dm9000 and register it
 输入参数  : bd_t *bis  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年9月21日
    作    者   : QSWWD
    修改内容   : 新生成函数

*****************************************************************************/
int dm9000_initialize(void)
{
	int ret = 0;
	struct eth_device *dev;
	/*setting dm9000*/
	ret = get_dm9000_resoures();	
	ret |= CreateQueue(&dm9000_queue,MAX_RXBUF_SIZE,dm9000_rxbuf);
	if(ret < 0){
		DM9000_DBG("get resoures failed.\n\r");
		goto tail;
	}
	dev = &(dm9000_info.netdev);
	/* Load MAC address from EEPROM */
	dm9000_get_enetaddr(dev);

	dev->init = DM9000_Init;
	dev->halt = dm9000_halt;
	dev->send = dm9000_send;
	dev->recv = dm9000_revPacket;
	ret = eth_register(dev);
tail:
	return ret;
}


