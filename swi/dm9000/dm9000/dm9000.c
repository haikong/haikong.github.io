#include <dm9000.h>
#include <s3c2440_addr.h>
#include <stdio.h>
#include <intirq.h>
#include <test.h>
#include <miscellaneous.h>
#include <net.h>
#include <glob.h>
#include <bsp.h>
/*mac address*/
static unsigned char mac_addr[6] __attribute__ ((__aligned__(4)))= {0x1a,0x2b,0x3c,0x4d,0x5e,0x6f};
/*dm9000 board infomation*/
static t_board_info dm9000_info;
/*****************************************************************************
 �� �� ��  : void  udelay_us(int us)
 ��������  : just for udelay some times about xx us,FCK = 200MHZ = 5ns.
 �������  : us
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��4��5��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

*****************************************************************************/
static void udelay_us(unsigned int us)
{
    int i;
	while(us--)
		for(i = 0 ;i < 190;i++);		
}
/*****************************************************************************
 �� �� ��  : udelay_ms
 ��������  : delay some ms
 �������  : unsigned int ms  
 �������  : ��
 �� �� ֵ  : static
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��9��21��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

*****************************************************************************/
static void udelay_ms(unsigned int ms)
{
    int i;
	while(ms--)
		for(i = 0 ;i < 200000;i++);		
}

/*****************************************************************************
 �� �� ��  : dm9000_outblk_8bit
 ��������  : dm9000���ֽ����
 �������  : volatile void *data_ptr
             int count
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��9��17��
    ��    ��   : haikong
    �޸�����   : �����ɺ���

*****************************************************************************/
static void dm9000_outblk_8bit( volatile void *data_ptr, int count )
{
	int i;
	for (i = 0; i < count; i++)
		DM9000_outb((((unsigned char*) data_ptr)[i] & 0xff), DM9000_DAT_BASE);    
}
/*****************************************************************************
 �� �� ��  : dm9000_outblk_16bit
 ��������  : dm9000�������
 �������  : volatile void *data_ptr
             int count
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��9��17��
    ��    ��   : haikong
    �޸�����   : �����ɺ���

*****************************************************************************/
static void dm9000_outblk_16bit( volatile void *data_ptr, int count )
{
	int i;
	unsigned int tmplen = (count + 1) / 2;
	
	for (i = 0; i < tmplen; i++)
		DM9000_outw((((unsigned short*) data_ptr)[i]), DM9000_DAT_BASE);    
}

/*****************************************************************************
 �� �� ��  : dm9000_outblk_32bit
 ��������  : dm9000��2�����
 �������  : volatile void *data_ptr
             int count
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��9��17��
    ��    ��   : haikong
    �޸�����   : �����ɺ���

*****************************************************************************/
static void dm9000_outblk_32bit( volatile void *data_ptr, int count )
{
	int i;
	unsigned int tmplen = (count + 3) / 4;
	
	for (i = 0; i < tmplen; i++)
		DM9000_outl((((unsigned int*) data_ptr)[i]), DM9000_DAT_BASE);    
}
/*****************************************************************************
 �� �� ��  : dm9000_inblk_8bit
 ��������  : dm9000���ֽ�����
 �������  : volatile void *data_ptr
             int count
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��9��17��
    ��    ��   : haikong
    �޸�����   : �����ɺ���

*****************************************************************************/
static void dm9000_inblk_8bit(void *data_ptr, int count)
{
	int i;
	for (i = 0; i < count; i++)
		((UINT8*) data_ptr)[i] = DM9000_inb(DM9000_DAT_BASE);
}
/*****************************************************************************
 �� �� ��  : dm9000_inblk_16bit
 ��������  : dm9000����������
 �������  : volatile void *data_ptr
             int count
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��9��17��
    ��    ��   : haikong
    �޸�����   : �����ɺ���

*****************************************************************************/
static void dm9000_inblk_16bit(void *data_ptr, int count)
{
	int i;
	UINT32 tmplen = (count + 1) / 2;

	for (i = 0; i < tmplen; i++)
		((UINT16 *) data_ptr)[i] = DM9000_inw(DM9000_DAT_BASE);
}
/*****************************************************************************
 �� �� ��  : dm9000_inblk_32bit
 ��������  : dm9000��������
 �������  : volatile void *data_ptr
             int count
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��9��17��
    ��    ��   : haikong
    �޸�����   : �����ɺ���

*****************************************************************************/
static void dm9000_inblk_32bit(void *data_ptr, int count)
{
	int i;
	UINT32 tmplen = (count + 3) / 4;

	for (i = 0; i < tmplen; i++)
		((UINT32 *) data_ptr)[i] = DM9000_inl(DM9000_DAT_BASE);
}
/*****************************************************************************
 �� �� ��  : dm9000_rx_status_8bit
 ��������  : dm9000���ֽڶ�ȡ״̬��
 �������  : UINT16 *RxStatus :״̬��ָ��
             UINT16 *RxLen	  :����
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��9��17��
    ��    ��   : haikong
    �޸�����   : �����ɺ���

*****************************************************************************/
static void dm9000_rx_status_8bit(UINT16 *RxStatus, UINT16 *RxLen)
{
	DM9000_outb(MRCMD, DM9000_CMD_BASE);

	*RxStatus =
	    __le16_to_cpu(DM9000_inb(DM9000_DAT_BASE) +
			  (DM9000_inb(DM9000_DAT_BASE) << 8));
	*RxLen =
	    __le16_to_cpu(DM9000_inb(DM9000_DAT_BASE) +
			  (DM9000_inb(DM9000_DAT_BASE) << 8));
}
/*****************************************************************************
 �� �� ��  : dm9000_rx_status_16bit
 ��������  : dm9000�����ֶ�ȡ״̬��
 �������  : UINT16 *RxStatus :״̬��ָ��
             UINT16 *RxLen	  :����
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��9��17��
    ��    ��   : haikong
    �޸�����   : �����ɺ���

*****************************************************************************/
static void dm9000_rx_status_16bit(UINT16 *RxStatus, UINT16 *RxLen)
{
	DM9000_outb(MRCMD, DM9000_CMD_BASE);

	*RxStatus =__le16_to_cpu(DM9000_inw(DM9000_DAT_BASE));
	*RxLen =__le16_to_cpu(DM9000_inw(DM9000_DAT_BASE));
}
/*****************************************************************************
 �� �� ��  : dm9000_rx_status_32bit
 ��������  : dm9000�����ȡ״̬��
 �������  : UINT16 *RxStatus :״̬��ָ��
             UINT16 *RxLen	  :����
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��9��17��
    ��    ��   : haikong
    �޸�����   : �����ɺ���

*****************************************************************************/
static void dm9000_rx_status_32bit(UINT16 *RxStatus, UINT16 *RxLen)
{
	UINT32 tmpdata;

	DM9000_outb(MRCMD, DM9000_CMD_BASE);
	tmpdata = DM9000_inl(DM9000_DAT_BASE);

	*RxStatus =__le16_to_cpu(tmpdata);
	*RxLen =__le16_to_cpu(tmpdata >> 16);
}

/*****************************************************************************
 �� �� ��  : DM9000_iow
 ��������  : setting dm9000 registers
 �������  : reg
             data
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��4��11��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

*****************************************************************************/
static void inline DM9000_iow(unsigned long reg,char data )
{
	DM9000_outb(reg,DM9000_CMD_BASE);
	DM9000_outb(data,DM9000_DAT_BASE);
}

/*****************************************************************************
 �� �� ��  : DM9000_ior
 ��������  : reading dm9000 register content
 �������  : reg
 �������  : ��
 �� �� ֵ  : int
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��4��11��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

*****************************************************************************/
static unsigned char inline DM9000_ior(unsigned long reg )
{
    DM9000_outb(reg,DM9000_CMD_BASE);
	return DM9000_inb(DM9000_DAT_BASE);
}

/*****************************************************************************
 �� �� ��  : DM9000_iows
 ��������  : setting dm9000 registers
 �������  : reg
             data
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��4��11��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

*****************************************************************************/
static void inline DM9000_iows(unsigned long reg,short data )
{
	DM9000_outw(reg,DM9000_CMD_BASE);
	DM9000_outw(data,DM9000_DAT_BASE);
}
/*****************************************************************************
 �� �� ��  : dm9000_read_srom_word
 ��������  : Read a word data from SROM
 �������  : int offset  
             UINT8*to    
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��9��21��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

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
 �� �� ��  : dm9000_write_srom_word
 ��������  : write a word data from SROM
 �������  : int offset  
             UINT8*to    
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��9��21��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

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
 �� �� ��  : dm9000_get_enetaddr
 ��������  : get dm9000 mac addree 
 �������  : struct eth_device *dev  
 �������  : ��
 �� �� ֵ  : static
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��9��21��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

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
 �� �� ��  : test_dm9000_ID
 ��������  : just for testing the dm9000 Vender ID and product ID MAC address
 �������  : void
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��4��5��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

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
 �� �� ��  : dm9000_regs
 ��������  : display the dm9000 registers
 �������  : void
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��4��17��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

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
 �� �� ��  : DM9000_reset
 ��������  : dm9000 reset function
 �������  : void
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��4��11��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

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
 �� �� ��  : DM9000_sendPacket
 ��������  : DM9000 packet ���ͺ���
 �������  : data_src
             unsigned int length
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��4��17��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

*****************************************************************************/
int DM9000_sendPacket(struct eth_device *netdev,void* data_src, unsigned int length )
{
    unsigned int len;
	int i;
	unsigned char c_tmp;
	char* src = data_src;
	DM9000_iow(ISR, IMR_PTM); /* Clear Tx bit in ISR */
	len = length;
	//writing the data to TX_SRAM register , using MWCMD,for network endian
	//DM9000_iows(DM9000_CMD_BASE,MWCMD);
	//dm9000_outblk_16bit(data_src,length);
	#if 1
	for(i = 0;i < len;i +=2)
	{
		udelay_us(2);
		DM9000_iows(DM9000_DAT_BASE,src[i] | (src[i + 1] << 8));	
	}
	#endif
	//write the length to TXPLH and TXPLL registers	
	DM9000_iow(TXPLH,(len >> 8) & 0xff);
	DM9000_iow(TXPLL,len  & 0xff);	
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
	return 0;
}

/*****************************************************************************
 �� �� ��  : dm9000_revPacket
 ��������  : dm9000 recieve packets
 �������  : data_src
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��4��17��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

*****************************************************************************/
int dm9000_revPacket(struct eth_device *netdev, unsigned char* data_src )
{
	unsigned char RX_First_byte=0;//RX SRAM �ĵ�һ���ֽڵ�ֵ
	unsigned char RX_status=0;//�Ĵ��� RXSR ��ֵ
	unsigned int RX_length=0;//�������ݰ��ĳ���
	unsigned short data_temp=0;
	unsigned char i;//������

	//clear the interrput
	if(DM9000_ior(ISR) & 0x1)
		DM9000_iow(ISR,0x1);
	 //��ȡ RX SRAM �ĵ�һ���ֽ�
	 RX_First_byte = DM9000_ior(MRCMDX);//��ȡ RX SRAM �ĵ�һ���ֽڵ�ֵ
	 printf("RX_First_byte = 0x%x",RX_First_byte);
	//��һ�ζ�ȡ�����õ� 00
	 RX_First_byte = DM9000_ior(MRCMDX);//�ڶ��ζ�ȡһ���ܵõ�����
	 printf("RX_First_byte = 0x%x",RX_First_byte);
	 //��ȡ RX SRAM �ĵڶ��������ĸ��ֽ�
	 if(RX_First_byte == 0x01)
	 {
	 	unsigned char io_mode = DM9000_ior(ISR)>>6;
		DM9000_iow(DM9000_CMD_BASE,MRCMD); //�� Memory Write CMD ���͵� ADD ��
	
	 	if(io_mode == 0)//IO word mode
	 	{
	 		RX_First_byte = DM9000_ior(DM9000_DAT_BASE);//��ȡ����״̬�Ĵ�����ֵ
 			printf("\r\nRX_First_byte = %d\r\nRX_status = %d\r\n",RX_First_byte&0x00ff,(RX_status>>8)&0x00ff);
			RX_length = DM9000_ior(DM9000_DAT_BASE);//��ȡ����״̬�Ĵ�����ֵ
			printf("\r\nRX_length = %d\r\n",RX_length);
		}
		else if(io_mode == 1)//IO dword mode
		{
	
		}
		else if(io_mode == 2)//IO byte mode
		{
	
		}
	
		//��ȡ���յ����ݰ�
		for(i=0;i<RX_length;i=i+2)
		{
			data_temp = DM9000_ior(DM9000_DAT_BASE);//��ȡ���� 16bit ������
			data_src[i] = data_temp&0x00ff;
			printf(" %x",data_src[i]);
			data_src[i+1] = (data_temp >> 8) & 0xff;
			printf(" %x",data_src[i+1]);
		}	
	 	return RX_length;
	
	 }
	 else
	 {
	 	printf("�����ݰ�\n");
		//reset dm9000 again
		DM9000_iow(IMR,0x80);
		DM9000_Init(NULL);
		DM9000_iow(IMR,0x81);
	 	return -1;
	 }

}

/*****************************************************************************
 �� �� ��  : dm9000_isr
 ��������  : dm9000 interrput service program
 �������  : void
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��4��19��
    ��    ��   : haikong
    �޸�����   : �����ɺ���

*****************************************************************************/

static void dm9000_isr(unsigned int vector)
{
	int i;
	unsigned char buf[1000];
	printf("%s-%s-%d\n\t",__FILE__,__FUNCTION__,__LINE__);
	i = dm9000_revPacket(NULL,buf);
	if(i >0)
		arp_process((char*)buf,i);
}

/*****************************************************************************
 �� �� ��  : dm9000_gpio_init
 ��������  : dm9000 some needed gpio initialize
 �������  : void
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��4��19��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

*****************************************************************************/
static void dm9000_gpio_init( void )
{
    //setting dm9000 extern interrupt
	key_init(7,0x2);
	//setting EINT7 is Rising edge triggered
	EXTINT0 &=  ~(7 << 28);
	EXTINT0 |= (RETIG << 28);
	//register EINT7 ISR
	register_extern_int(EXTERNIRQ7,dm9000_isr);
}

/*****************************************************************************
 �� �� ��  : dm9000_phy_read
 ��������  : Read a word from phyxcer
 �������  : int reg
 �������  : ��
 �� �� ֵ  : phy read data
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��8��31��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

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
 �� �� ��  : static int dm9000_probe(void)
 ��������  : Search DM9000 board, allocate space and register it
 �������  : 
 �������  : ��
 �� �� ֵ  : 0:ok;-1:error
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��8��30��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

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
 �� �� ��  : dm9000_phy_write
 ��������  : Write a word to phyxcer
 �������  : int reg       
             UINT16 value  
 �������  : ��
 �� �� ֵ  : static
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��9��21��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

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
	printf("dm9000_phy_write(reg:0x%x, value:0x%x)\n", reg, value);
}

/*****************************************************************************
 �� �� ��  : dm9000_halt
 ��������  : Stop the interface.The interface is stopped when it is brought.
 �������  : struct eth_device *netdev  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��9��21��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

*****************************************************************************/
static void dm9000_halt(struct eth_device *netdev)
{
	/* RESET devie */
	dm9000_phy_write(0, 0x8000);	/* PHY RESET */
	DM9000_iow(GPR, 0x01);	/* Power-Down PHY */
	DM9000_iow(IMR, 0x80);	/* Disable all interrupt */
	DM9000_iow(RCR, 0x00);	/* Disable RX */
}

/*****************************************************************************
 �� �� ��  : dm9000_init
 ��������  : dm9000 device initialize
	//1.����صĹܽ����ú�
	//2.����صļĴ������ú�

	//��ʼ���Ĵ�����Ҫ�������飺
	//1.��λ
	//2.���ԭ�ȵ�״̬
	//3.�Է��䡢���ս��п���
	//4.ʹ���ж�
	//������ʱ����Щ�ط���Ҫ�ظ����á�
 �������  : void
 �������  : ��
 �� �� ֵ  : 0:ok;-1:error
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��3��30��
    ��    ��   : haikong
    �޸�����   : �����ɺ���

*****************************************************************************/

int DM9000_Init(struct eth_device *dev)
{
	unsigned char i,io_mode,oft,lnk;	
	struct board_info *db = &dm9000_info;
	//setting dm9000 ISR
	dm9000_gpio_init();
	/* RESET device */
	DM9000_reset();
	if(dm9000_probe() < 0)
		return -1;	
	/* Auto-detect 8/16/32 bit mode, ISR Bit 6+7 indicate bus width */
	io_mode = DM9000_ior(ISR) >> 6;
	switch(io_mode){
		case 0x0:  /* 16-bit mode */
			printf("DM9000: running in 16 bit mode\n");
			db->outblk    = dm9000_outblk_16bit;
			db->inblk     = dm9000_inblk_16bit;
			db->rx_status = dm9000_rx_status_16bit;
			break;
		case 0x01:  /* 32-bit mode */
			printf("DM9000: running in 32 bit mode\n");
			db->outblk    = dm9000_outblk_32bit;
			db->inblk     = dm9000_inblk_32bit;
			db->rx_status = dm9000_rx_status_32bit;
			break;
		case 0x02: /* 8 bit mode */
			printf("DM9000: running in 8 bit mode\n");
			db->outblk    = dm9000_outblk_8bit;
			db->inblk     = dm9000_inblk_8bit;
			db->rx_status = dm9000_rx_status_8bit;
			break;
		default:
			/* Assume 8 bit mode, will probably not work anyway */
			printf("DM9000: Undefined IO-mode:0x%x\n", io_mode);
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

/*****************************************************************************
 �� �� ��  : dm9000_initialize
 ��������  : initializs the dm9000 and register it
 �������  : bd_t *bis  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��9��21��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

*****************************************************************************/
int dm9000_initialize(bd_t *bis)
{
	struct eth_device *dev = &(dm9000_info.netdev);

	/* Load MAC address from EEPROM */
	dm9000_get_enetaddr(dev);

	dev->init = DM9000_Init;
	dev->halt = dm9000_halt;
	dev->send = DM9000_sendPacket;
	dev->recv = dm9000_revPacket;
	sprintf(dev->name, "dm9000");
	eth_register(dev);

	return 0;
}


