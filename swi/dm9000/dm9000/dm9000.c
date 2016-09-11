#include <dm9000.h>
#include <s3c2440_addr.h>
#include <stdio.h>
#include <intirq.h>
#include <test.h>

static unsigned char mac_addr[6] __attribute__((__aligned__(4)))= {0x1a,0x2b,0x3c,0x4d,0x5e,0x6f};

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
	DM9000_outb(reg,DM9000_CMD_BASE);
	DM9000_outb(data,DM9000_DAT_BASE);
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
    DM9000_outb(reg,DM9000_CMD_BASE);
	return DM9000_inb(DM9000_DAT_BASE);
}

/*****************************************************************************
 函 数 名  : test_dm9000_ID
 功能描述  : just for testing the dm9000 Vender ID and product ID MAC address
 输入参数  : void
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年4月5日
    作    者   : QSWWD
    修改内容   : 新生成函数

*****************************************************************************/
void test_dm9000_ID( void )
{
	unsigned short vid = 0;
	unsigned short pid = 0;
	/*Now the software reset complete over*/
	printf("Now read the dm9000a ID infomation.\n\r");
	vid = DM9000_ior(VIDL);
	vid |= DM9000_ior(VIDH) << 8;
	pid = DM9000_ior(PIDL);
	pid |= DM9000_ior(PIDH) << 8;
	printf("VID = %x,PID = %x.\n\r",vid,pid);
	printf("MAC = %x",DM9000_ior(PAR)&0x00ff);
	printf(" %x",DM9000_ior(PAR+1)&0x00ff);
	printf(" %x",DM9000_ior(PAR+2)&0x00ff);
	printf(" %x",DM9000_ior(PAR+3)&0x00ff);
	printf(" %x",DM9000_ior(PAR+4));
	printf(" %x\r\n",DM9000_ior(PAR+5));

}

/*****************************************************************************
 函 数 名  : dm9000_regs
 功能描述  : display the dm9000 registers
 输入参数  : void
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年4月17日
    作    者   : QSWWD
    修改内容   : 新生成函数

*****************************************************************************/
static void dm9000_regs( void )
{
    printf("NCR = %x\n\r",DM9000_ior(NCR));
    printf("NSR = %x\n\r",DM9000_ior(NSR));
    printf("TCR = %x\n\r",DM9000_ior(TCR));
    printf("TSR1 = %x\n\r",DM9000_ior(TSR1));
    printf("TSR2 = %x\n\r",DM9000_ior(TSR2));
    printf("RCR = %x\n\r",DM9000_ior(RCR));	
    printf("RSR = %x\n\r",DM9000_ior(RSR));
    printf("ROCR = %x\n\r",DM9000_ior(ROCR));
    printf("BPRT = %x\n\r",DM9000_ior(BPTR));
    printf("FCRT = %x\n\r",DM9000_ior(FCTR));
    printf("RTFCR = %x\n\r",DM9000_ior(FCR));
	printf("EPCR = %x\n\r",DM9000_ior(EPCR));
	printf("EPAR = %x\n\r",DM9000_ior(EPAR));
	printf("EPDRL = %x\n\r",DM9000_ior(EPDRL));
	printf("EPDRH = %x\n\r",DM9000_ior(EPDRH));
	printf("WCR = %x\n\r",DM9000_ior(WCR));
    printf("GPCR = %x\n\r",DM9000_ior(GPCR));
    printf("GPR = %x\n\r",DM9000_ior(GPR));
    printf("SMCR = %x\n\r",DM9000_ior(SMCR));
    printf("ISR = %x\n\r",DM9000_ior(ISR));
	printf("IMR = %x\n\r",DM9000_ior(IMR));
	
}

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


static void udelay_ms(unsigned int ms)
{
    int i;
	while(ms--)
		for(i = 0 ;i < 200000;i++);		
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

	printf("resetting DM9000\n\t");

	/* DEBUG: Make all GPIO0 outputs, all others inputs */
	DM9000_iow(GPCR, GPCR_GPIO0_OUT);
	/* Step 1: Power internal PHY by writing 0 to GPIO0 pin */
	DM9000_iow(GPR, 0);
	/* Step 2: Software reset */
	DM9000_iow(NCR, (NCR_LBK_INT_MAC | NCR_RST));

	do {
		printf("resetting the DM9000, 1st reset\n\t");
		udelay_us(25); /* Wait at least 20 us */
	} while (DM9000_ior(NCR) & 1);

	DM9000_iow(NCR, 0);
	DM9000_iow(NCR, (NCR_LBK_INT_MAC | NCR_RST)); /* Issue a second reset */

	do {
		printf("resetting the DM9000, 2nd reset\n\t");
		udelay_us(25); /* Wait at least 20 us */
	} while (DM9000_ior(NCR) & 1);

	/* Check whether the ethernet controller is present */
	if ((DM9000_ior(PIDL) != 0x0) ||
		(DM9000_ior(PIDH) != 0x90))
		printf("ERROR: resetting DM9000 -> not responding\nt");
  
}

/*****************************************************************************
 函 数 名  : DM9000_sendPacket
 功能描述  : DM9000 packet 发送函数
 输入参数  : data_src
             unsigned int length
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年4月17日
    作    者   : QSWWD
    修改内容   : 新生成函数

*****************************************************************************/
void DM9000_sendPacket(char* data_src, unsigned int length )
{
    unsigned int len;
	int i;
	unsigned char c_tmp;

	//diable the dm9000 interrput
	DM9000_iow(IMR,0x80);
	//write the length to TXPLH and TXPLL registers
	len = length;
	DM9000_iow(TXPLH,(len >> 8) & 0xff);
	DM9000_iow(TXPLL,len  & 0xff);
	//writing the data to TX_SRAM register , using MWCMD,for network endian
	DM9000_iow(DM9000_CMD_BASE,MWCMD);
	for(i = 0;i < len;i +=2)
	{
		udelay_us(2);
		DM9000_iow(DM9000_DAT_BASE,data_src[i] | (data_src[i + 1] << 8));	
	}
	//setting the TCR and sending data to the network
	DM9000_iow(TCR,0x1);
	//waiting until the data sending complete
	while(1)
	{
		c_tmp = DM9000_ior(TCR);
		if((c_tmp & 0x01) == 0x00)
			break;
	}
	//just for test 
	c_tmp = DM9000_ior(NSR);
	if((c_tmp & 0x04) == 0x04)
	{
		if((DM9000_ior(TSR1) & 0xfc) == 0x0)
			printf("TSR1 succeed\n");
		else
			printf("TSR1 failed\n");
	}
	else
	{
		if((DM9000_ior(TSR2) & 0xfc) == 0x0)
			printf("TSR2 succeed\n");
		else
			printf("TSR2 failed\n");;		
	}
	//clear the status register ,do not to clear the interrput
	DM9000_iow(NSR,0x2c);
	//enable the dm9000 reciving interrput
	DM9000_iow(IMR,0x81);
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
int dm9000_revPacket( unsigned char* data_src )
{
	unsigned char RX_First_byte=0;//RX SRAM 的第一个字节的值
	unsigned char RX_status=0;//寄存器 RXSR 的值
	unsigned int RX_length=0;//接收数据包的长度
	unsigned short data_temp=0;
	unsigned char i;//计数用

	//clear the interrput
	if(DM9000_ior(ISR) & 0x1)
		DM9000_iow(ISR,0x1);
	 //读取 RX SRAM 的第一个字节
	 RX_First_byte = DM9000_ior(MRCMDX);//读取 RX SRAM 的第一个字节的值
	 printf("RX_First_byte = 0x%x",RX_First_byte);
	//第一次读取经常得到 00
	 RX_First_byte = DM9000_ior(MRCMDX);//第二次读取一般能得到数据
	 printf("RX_First_byte = 0x%x",RX_First_byte);
	 //读取 RX SRAM 的第二、三、四个字节
	 if(RX_First_byte == 0x01)
	 {
	 	unsigned char io_mode = DM9000_ior(ISR)>>6;
		DM9000_iow(DM9000_CMD_BASE,MRCMD); //将 Memory Write CMD 发送到 ADD 上
	
	 	if(io_mode == 0)//IO word mode
	 	{
	 		RX_First_byte = DM9000_ior(DM9000_DAT_BASE);//读取接收状态寄存器的值
 			printf("\r\nRX_First_byte = %d\r\nRX_status = %d\r\n",RX_First_byte&0x00ff,(RX_status>>8)&0x00ff);
			RX_length = DM9000_ior(DM9000_DAT_BASE);//读取接收状态寄存器的值
			printf("\r\nRX_length = %d\r\n",RX_length);
		}
		else if(io_mode == 1)//IO dword mode
		{
	
		}
		else if(io_mode == 2)//IO byte mode
		{
	
		}
	
		//读取接收的数据包
		for(i=0;i<RX_length;i=i+2)
		{
			data_temp = DM9000_ior(DM9000_DAT_BASE);//读取到的 16bit 的数据
			data_src[i] = data_temp&0x00ff;
			printf(" %x",data_src[i]);
			data_src[i+1] = (data_temp >> 8) & 0xff;
			printf(" %x",data_src[i+1]);
		}	
	 	return RX_length;
	
	 }
	 else
	 {
	 	printf("无数据包\n");
		//reset dm9000 again
		DM9000_iow(IMR,0x80);
		DM9000_Init();
		DM9000_iow(IMR,0x81);
	 	return -1;
	 }

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
	int i;
	unsigned char buf[1000];
	i = dm9000_revPacket(buf);
	if(i >0)
		arp_process((char*)buf,i);
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
static void dm9000_gpio_init( void )
{
    //setting dm9000 extern interrupt
	GPFCON |= 0x2 << 14;
	//setting EINT7 is Rising edge triggered
	EXTINT0 &=  ~(7 << 28);
	EXTINT0 |= (HLEVL << 28);
	//register EINT7 ISR
	register_extern_int(EXTERNIRQ7,dm9000_isr);
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
	printf("dm9000_phy_read(0x%x): 0x%x\n\t", reg, val);
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
	unsigned int id_val;
	id_val = DM9000_ior(VIDL);
	id_val |= DM9000_ior(VIDH) << 8;
	id_val |= DM9000_ior(PIDL) << 16;
	id_val |= DM9000_ior(PIDH) << 24;
	if (id_val == DM9000_ID) {
		printf("dm9000 i/o: 0x%x, id: 0x%x \n\t", DM9000_CMD_BASE,id_val);
		return 0;
	} else {
		printf("dm9000 not found at 0x%08x id: 0x%08x\n\t",DM9000_CMD_BASE, id_val);
		return -1;
	}
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
	//setting dm9000 ISR
	dm9000_gpio_init();
	/* RESET device */
	DM9000_reset();
	if(dm9000_probe() < 0)
		return -1;	

#ifdef _DM9000	
	//3.配置寄存器
	//3.1.激活内部 PHY
	DM9000_iow(GPCR,0x01);//设置 GPCR bit[0]=1，使 DM9000 为 GPIO0 为输出
	DM9000_iow(GPR,0x00);//GPR bit[0]=0，使 GPIO0 输出低电平以激活内部 PHY
	udelay_us(20);

	//3.2 软件复位
	DM9000_iow(NCR,0x03); //软件复位，MAC 内部循环反馈
	udelay_us(20); //延时 10us 以上，等待软件复位完成
	DM9000_iow(NCR,0x00); //复位完成，设置正常工作模式
	DM9000_iow(NCR,0x03); //第二次软件复位。确保软件复位完全成功
	udelay_us(20);
	DM9000_iow(NCR,0x00);

	//3.3 使能中断
	DM9000_iow(IMR,0x80);//使能指向 SRAM 的指针的自动返回功能

	//3.4 清除原网络和中断状态
	DM9000_iow(NSR,0x2c); //清除各种状态标志位
	DM9000_iow(ISR,0xf); //清除所有中断标志位,8-bit

	//3.5 对发射和接收进行新的控制
	// 对中断进行新的控制
	DM9000_iow(RCR,0x39);//接收控制
	DM9000_iow(TCR,0x00);//发送控制
	DM9000_iow(BPTR,0x3f);//设置 RX 的最低阀值，小于将产生拥塞
	DM9000_iow(FCTR,0x38);//接收 FIFO 门限 3K，8K
	DM9000_iow(FCR,0xff);//启动一些控制功能
	DM9000_iow(SMCR,0x00);//未启动特殊模式

	//3.6 设置 MAC 地址
	for(i=0; i<6; i++){
		DM9000_iow(PAR+i, mac_addr[i]);
		udelay_us(1);
	}		
	//3.7 清除原网络状态和中断标志位
	DM9000_iow(NSR,0x2c); //清除各种状态标志位
	DM9000_iow(ISR,0xf); //清除所有中断标志位,8-bit

	//3.8 使能中断
	DM9000_iow(IMR,0x81);//使能指向 SRAM 的指针满后自动返回功能。
	return 0;
#else
	/* Auto-detect 8/16/32 bit mode, ISR Bit 6+7 indicate bus width */
	io_mode = DM9000_ior(ISR) >> 6;
	printf("The dm9000 bus width is %d bit.\n\t",io_mode);
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
		DM9000_iow(oft, mac_addr[i]);
	for (i = 0, oft = MAR; i < 8; i++, oft++)
		DM9000_iow(oft, 0xff);

	/* read back mac, just to be sure */
	for (i = 0, oft = PAR; i < 6; i++, oft++)
		printf("%02x:", DM9000_ior(oft));
	printf("\n\t");

	/* Activate DM9000 */
	/* RX enable */
	DM9000_iow(RCR, RCR_DIS_LONG | RCR_DIS_CRC | RCR_RXEN);
	/* Enable TX/RX interrupt mask */
	DM9000_iow(IMR, IMR_PAR);

	i = 0;
	while (!(dm9000_phy_read(1) & 0x20)) {	/* autonegation complete bit */
		udelay_ms(10);
		i++;
		if (i == 10000) {
			printf("could not establish link\n\t");
			return 0;
		}
	}

	/* see what we've got */
	lnk = dm9000_phy_read(17) >> 12;
	printf("operating at ");
	switch (lnk) {
	case 1:
		printf("10M half duplex ");
		break;
	case 2:
		printf("10M full duplex ");
		break;
	case 4:
		printf("100M half duplex ");
		break;
	case 8:
		printf("100M full duplex ");
		break;
	default:
		printf("unknown: %d ", lnk);
		break;
	}
	printf("mode\n\t");
	return 0;	
#endif	
}

void test_dm9000(void)
{
	int i,oft;
	test_dm9000_ID();
	dm9000_regs();
	
	/* fill device MAC address registers */
	for (i = 0, oft = PAR; i < 6; i++, oft++)
	{
		DM9000_iow(PAR + i, mac_addr[i]);
	}
	for (i = 0, oft = MAR; i < 8; i++, oft++)
		DM9000_iow(oft, 0xff);
	/* read back mac, just to be sure */
	for (i = 0, oft = PAR; i < 6; i++, oft++)
		printf("%02x:", DM9000_ior(oft));
	printf("\n\t");	
}

void sdbinit(void)
{
	DM9000_iow(NCR,0x03); //软件复位，MAC 内部循环反馈
	udelay_us(20); //延时 10us 以上，等待软件复位完成
	DM9000_iow(NCR,0x00); //复位完成，设置正常工作模式
	DM9000_iow(NCR,0x03); //第二次软件复位。确保软件复位完全成功
	udelay_us(20);
	DM9000_iow(NCR,0x00);
	DM9000_iow(IMR,0x80);//使能指向 SRAM 的指针的自动返回功能
	DM9000_iow(NSR,0x2c); //清除各种状态标志位
	DM9000_iow(ISR,0xbf); //清除所有中断标志位,8-bit
	DM9000_iow(RCR,0x39);//接收控制
	DM9000_iow(TCR,0x00);//发送控制
	DM9000_iow(BPTR,0x3f);//设置 RX 的最低阀值，小于将产生拥塞
	DM9000_iow(FCTR,0x00);//接收 FIFO 门限 3K，8K
	DM9000_iow(FCR,0xff);//启动一些控制功能
	DM9000_iow(SMCR,0x00);//未启动特殊模式
}

