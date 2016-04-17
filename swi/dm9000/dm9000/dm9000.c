#include <dm9000.h>
#include <s3c2440_addr.h>
#include <stdio.h>

static unsigned char mac_addr[6] = {0xa,0xb,0xc,0xd,0xe,0xf};

/*****************************************************************************
 �� �� ��  : DM9000_WRITE_REG
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
static void inline DM9000_WRITE_REG(unsigned char reg,char data )
{
	dm9000_io_outb(DM9000_CMD_BASE,reg);
	dm9000_io_outb(DM9000_DAT_BASE,data);
}

/*****************************************************************************
 �� �� ��  : DM9000_READ_REG
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
static unsigned char inline DM9000_READ_REG(unsigned char reg )
{
    dm9000_io_outb(DM9000_CMD_BASE,reg);
	return dm9000_io_inb(DM9000_DAT_BASE);
}

/*****************************************************************************
 �� �� ��  : test_dm9000_ID
 ��������  : just for testing the dm9000 Vender ID and product ID
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
	vid = DM9000_READ_REG(VIDL);
	vid |= DM9000_READ_REG(VIDH) << 8;
	pid = DM9000_READ_REG(PIDL);
	pid |= DM9000_READ_REG(PIDH) << 8;
	printf("VID = %x,PID = %x.\n\r",vid,pid);

}

/*****************************************************************************
 �� �� ��  : test_dm9000
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
static void test_dm9000( void )
{
    printf("NCR = %x\n\r",DM9000_READ_REG(NCR));
    printf("NSR = %x\n\r",DM9000_READ_REG(NSR));
    printf("TCR = %x\n\r",DM9000_READ_REG(TCR));
    printf("RCR = %x\n\r",DM9000_READ_REG(RCR));
    printf("RSR = %x\n\r",DM9000_READ_REG(RSR));
    printf("ROCR = %x\n\r",DM9000_READ_REG(ROCR));
    printf("BPRT = %x\n\r",DM9000_READ_REG(BPTR));
    printf("FCRT = %x\n\r",DM9000_READ_REG(FCTR));
    printf("RTFCR = %x\n\r",DM9000_READ_REG(FCR));
    printf("GPCR = %x\n\r",DM9000_READ_REG(GPCR));
    printf("GPR = %x\n\r",DM9000_READ_REG(GPR));
    printf("ISR = %x\n\r",DM9000_READ_REG(ISR));
	printf("IMR = %x\n\r",DM9000_READ_REG(IMR));
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
static void DM9000_reset( void )
{
	DM9000_WRITE_REG(GPCR,0x1);
	DM9000_WRITE_REG(GPR,00);
	udelay(20);
	DM9000_WRITE_REG(NCR,0x3);//software reset
	udelay(2);
	DM9000_WRITE_REG(NCR,0x0);//software reset complete
	DM9000_WRITE_REG(NCR,0x3);//sencond software reset 
	udelay(3);
	DM9000_WRITE_REG(NCR,0x0);//software reset complete
    
}


/*****************************************************************************
 �� �� ��  : dm9000_init
 ��������  : dm9000 device initialize
 �������  : void
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��3��30��
    ��    ��   : haikong
    �޸�����   : �����ɺ���

*****************************************************************************/
void dm9000_init( void )
{
	//setting dm9000 extern interrupt
	GPFCON |= 0x2 << 14;
	test_dm9000_ID();
	DM9000_reset();
	test_dm9000_ID();
}

//================================================================
//DM9000_Init()
//1.����صĹܽ����ú�
//2.����صļĴ������ú�

//��ʼ���Ĵ�����Ҫ�������飺
//1.��λ
//2.���ԭ�ȵ�״̬
//3.�Է��䡢���ս��п���
//4.ʹ���ж�
//������ʱ����Щ�ط���Ҫ�ظ����á�
//================================================================
void DM9000_Init(void)
{
	unsigned char i;
	//3.���üĴ���
	//3.1.�����ڲ� PHY
	DM9000_WRITE_REG(GPCR,0x01);//���� GPCR bit[0]=1��ʹ DM9000 Ϊ GPIO0 Ϊ���
	DM9000_WRITE_REG(GPR,0x00);//GPR bit[0]=0��ʹ GPIO0 ����͵�ƽ�Լ����ڲ� PHY
	udelay(5000);

	//3.2 �����λ
	DM9000_WRITE_REG(NCR,0x03); //�����λ��MAC �ڲ�ѭ������
	udelay(3000); //��ʱ 10us ���ϣ��ȴ������λ���
	DM9000_WRITE_REG(NCR,0x00); //��λ��ɣ�������������ģʽ
	DM9000_WRITE_REG(NCR,0x03); //�ڶ��������λ��ȷ�������λ��ȫ�ɹ�
	udelay(3000);
	DM9000_WRITE_REG(NCR,0x00);

	//3.3 ʹ���ж�
	DM9000_WRITE_REG(IMR,0x80);//ʹ��ָ�� SRAM ��ָ����Զ����ع���


	//3.4 ���ԭ������ж�״̬
	DM9000_WRITE_REG(NSR,0x2c); //�������״̬��־λ
	DM9000_WRITE_REG(ISR,0xbf); //��������жϱ�־λ,8-bit

	//3.5 �Է���ͽ��ս����µĿ���
	// ���жϽ����µĿ���
	DM9000_WRITE_REG(RCR,0x39);//���տ���
	DM9000_WRITE_REG(TCR,0x00);//���Ϳ���
	DM9000_WRITE_REG(BPTR,0x3f);//���� RX ����ͷ�ֵ��С�ڽ�����ӵ��
	DM9000_WRITE_REG(FCTR,0x00);//���� FIFO ���� 3K��8K
	DM9000_WRITE_REG(FCR,0xff);//����һЩ���ƹ���
	DM9000_WRITE_REG(SMCR,0x00);//δ��������ģʽ

	//3.6 ���� MAC ��ַ
	for(i=0; i<6; i++)
		DM9000_WRITE_REG(PAR+i, mac_addr[i]);
	//3.7 ���ԭ����״̬���жϱ�־λ
	DM9000_WRITE_REG(NSR,0x2c); //�������״̬��־λ
	DM9000_WRITE_REG(ISR,0x3f); //��������жϱ�־λ,8-bit

	//3.8 ʹ���ж�
	DM9000_WRITE_REG(IMR,0x81);//ʹ��ָ�� SRAM ��ָ�������Զ����ع��ܡ�
	//ʹ�����ݰ������ж�
	/////////////////MAC ��ַ///////////////////////////////
	test_dm9000_ID();
	printf("\r\nMAC = %x",DM9000_READ_REG(PAR)&0x00ff);
	printf(" %x",DM9000_READ_REG(PAR+1)&0x00ff);
	printf(" %x",DM9000_READ_REG(PAR+2)&0x00ff);
	printf(" %x",DM9000_READ_REG(PAR+3)&0x00ff);
	printf(" %x",DM9000_READ_REG(PAR+4)&0x00ff);
	printf(" %x\r\n",DM9000_READ_REG(PAR+5)&0x00ff);
	test_dm9000();
}

/*****************************************************************************
 �� �� ��  : void  udelay(int us)
 ��������  : just for udelay some times about xx ns.
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
void udelay(unsigned int us)
{
    int i;
	while(us--)
		for(i = 0 ;i < 1000;i++);		
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
void DM9000_sendPacket( unsigned char* data_src, unsigned int length )
{
    unsigned int len;
	int i;
	unsigned char c_tmp;

	//diable the dm9000 interrput
	DM9000_WRITE_REG(IMR,0x80);
	//write the length to TXPLH and TXPLL registers
	len = length;
	DM9000_WRITE_REG(TXPLH,(len >> 8) & 0xff);
	DM9000_WRITE_REG(TXPLL,len  & 0xff);
	//writing the data to TX_SRAM register , using MWCMD,for network endian
	dm9000_io_outb(DM9000_CMD_BASE,MWCMD);
	for(i = 0;i < len;i +=2)
	{
		udelay(20);
		dm9000_io_outb(DM9000_DAT_BASE,data_src[i] | (data_src[i + 1] << 8));	
	}
	//setting the TCR and sending data to the network
	DM9000_WRITE_REG(TCR,0x1);
	//waiting until the data sending complete
	while(1)
	{
		c_tmp = DM9000_READ_REG(TCR);
		if((c_tmp & 0x01) == 0x00)
			break;
	}
	//just for test 
	c_tmp = DM9000_READ_REG(NSR);
	if((c_tmp & 0x04) == 0x04)
	{
		if((DM9000_READ_REG(TSR1) & 0xfc) == 0x0)
			printf("TSR1 succeed\n");
		else
			printf("TSR1 failed\n");;
	}
	else
	{
		if((DM9000_READ_REG(TSR2) & 0xfc) == 0x0)
			printf("TSR2 succeed\n");
		else
			printf("TSR2 failed\n");;		
	}
	//clear the status register ,do not to clear the interrput
	DM9000_WRITE_REG(NSR,0x2c);
	//enable the dm9000 reciving interrput
	DM9000_WRITE_REG(IMR,0x81);
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
int dm9000_revPacket( unsigned char* data_src )
{
	unsigned char RX_First_byte=0;//RX SRAM �ĵ�һ���ֽڵ�ֵ
	unsigned char RX_status=0;//�Ĵ��� RXSR ��ֵ
	unsigned int RX_length=0;//�������ݰ��ĳ���
	unsigned short data_temp=0;
	unsigned char i;//������

	//clear the interrput
	if(DM9000_READ_REG(ISR) & 0x1)
		DM9000_WRITE_REG(ISR,0x1);
	 //��ȡ RX SRAM �ĵ�һ���ֽ�
	 RX_First_byte = DM9000_READ_REG(MRCMDX);//��ȡ RX SRAM �ĵ�һ���ֽڵ�ֵ
	 printf("RX_First_byte = 0x%x",RX_First_byte);
	//��һ�ζ�ȡ�����õ� 00
	 RX_First_byte = DM9000_READ_REG(MRCMDX);//�ڶ��ζ�ȡһ���ܵõ�����
	 printf("RX_First_byte = 0x%x",RX_First_byte);
	 //��ȡ RX SRAM �ĵڶ��������ĸ��ֽ�
	 if(RX_First_byte == 0x01)
	 {
	 	unsigned char io_mode = DM9000_READ_REG(ISR)>>6;
		dm9000_io_outb(DM9000_CMD_BASE,MRCMD); //�� Memory Write CMD ���͵� ADD ��
	
	 	if(io_mode == 0)//IO word mode
	 	{
	 		RX_First_byte = dm9000_io_inb(DM9000_DAT_BASE);//��ȡ����״̬�Ĵ�����ֵ
 			printf("\r\nRX_First_byte = %d\r\nRX_status = %d\r\n",RX_First_byte&0x00ff,(RX_status>>8)&0x00ff);
			RX_length = dm9000_io_inb(DM9000_DAT_BASE);//��ȡ����״̬�Ĵ�����ֵ
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
			data_temp = dm9000_io_inb(DM9000_DAT_BASE);//��ȡ���� 16bit ������
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
		DM9000_WRITE_REG(IMR,0x80);
		DM9000_Init();
		DM9000_WRITE_REG(IMR,0x81);
	 	return -1;
	 }

}

