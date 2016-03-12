#include <s3c2440_addr.h>
#include <nand.h>

/* S3C2440��NAND Flash������ */
static void s3c2440_nand_reset(void) ;
static void s3c2440_wait_idle(void) ;
static void s3c2440_nand_select_chip(void) ;
static void s3c2440_nand_deselect_chip(void);
static void s3c2440_write_cmd(int cmd);
static void s3c2440_write_addr(unsigned int addr);
static unsigned char s3c2440_read_data(void) ;

/*setting the nand register struct start address */
static pt_S3C2440_NAND  s3c2440nand  = (pt_S3C2440_NAND)0x4e000000;

/*setting nand chip operation functions*/
static  t_Nand_Chip nand_chip = {
	.nand_reg			= (pt_S3C2440_NAND)0x4e000000,
	.nand_reset 		= s3c2440_nand_reset,
	.wait_idle			= s3c2440_wait_idle,
	.nand_select_chip	= s3c2440_nand_select_chip,
	.nand_deselect_chip	= s3c2440_nand_deselect_chip,
	.write_cmd 			= s3c2440_write_cmd,
	.write_addr			= s3c2440_write_addr,
	.read_data			= s3c2440_read_data,
};
/* S3C2440��NAND Flash�������� */

/* ��λ */
static void s3c2440_nand_reset(void)
{
    s3c2440_nand_select_chip();
    s3c2440_write_cmd(0xff);  // ��λ����
    s3c2440_wait_idle();
    s3c2440_nand_deselect_chip();
}

/* �ȴ�NAND Flash���� */
static void s3c2440_wait_idle(void)
{
    int i;
    volatile unsigned char *p = (volatile unsigned char *)&s3c2440nand->NFSTAT;
    while(!(*p & BUSY))
        for(i=0; i<10; i++);
}

/* ����Ƭѡ�ź� */
static void s3c2440_nand_select_chip(void)
{
    int i;
    s3c2440nand->NFCONT &= ~(1<<1);
    for(i=0; i<10; i++);    
}

/* ȡ��Ƭѡ�ź� */
static void s3c2440_nand_deselect_chip(void)
{
    s3c2440nand->NFCONT |= (1<<1);
}

/* �������� */
static void s3c2440_write_cmd(int cmd)
{
    volatile unsigned char *p = (volatile unsigned char *)&s3c2440nand->NFCMMD;
    *p = cmd;
}

/*������ַ*/
static void s3c2440_write_addr(unsigned int addr)
{
	int i;
	volatile unsigned char *p = (volatile unsigned char *)&s3c2440nand->NFADDR;
	int col, page;
	col = addr & NAND_BLOCK_MASK_LP;
	page = addr / NAND_SECTOR_SIZE_LP;
	
	*p = col & 0xff;			/* Column Address A0~A7 */
	for(i=0; i<10; i++);		
	*p = (col >> 8) & 0x0f; 	/* Column Address A8~A11 */
	for(i=0; i<10; i++);
	*p = page & 0xff;			/* Row Address A12~A19 */
	for(i=0; i<10; i++);
	*p = (page >> 8) & 0xff;	/* Row Address A20~A27 */
	for(i=0; i<10; i++);
	*p = (page >> 16) & 0x01;	/* Row Address A28 */
	for(i=0; i<10; i++);
}


/* ��ȡ���� */
static unsigned char s3c2440_read_data(void)
{
    volatile unsigned char *p = (volatile unsigned char *)&s3c2440nand->NFDATA;
    return *p;
}


/* ��ʼ��NAND Flash */
void nand_init(void)
{
	 nand_chip.nand_reg->NFCONF = (TACLS<<12)|(TWRPH0<<8)|(TWRPH1<<4);
    /* ʹ��NAND Flash������, ��ʼ��ECC, ��ֹƬѡ */
    nand_chip.nand_reg->NFCONT = (1<<4)|(1<<1)|(1<<0);
    /* ��λNAND Flash */
    nand_chip.nand_reset();
}


/* ������ */
void nand_read(unsigned char *buf, unsigned long start_addr, int size)
{
    int i, j;
	/* ��ַ�򳤶Ȳ����� */
    if ((start_addr & NAND_BLOCK_MASK_LP) || (size & NAND_BLOCK_MASK_LP)) {
        return ;    
    }

    /* ѡ��оƬ */
    nand_chip.nand_select_chip();

    for(i=start_addr; i < (start_addr + size);) {
      /* ����READ0���� */
      nand_chip.write_cmd(0);
      /* Write Address */
      nand_chip.write_addr(i);
	  /*write 2nd cycle*/
      nand_chip.write_cmd(0x30);		
	  /*wait untile ready*/
      nand_chip.wait_idle();
	  /*read a page*/
      for(j=0; j < NAND_SECTOR_SIZE_LP; j++,i++) {
          *buf = nand_chip.read_data();
          buf++;
      }
    }
    /* ȡ��Ƭѡ�ź� */
    nand_chip.nand_deselect_chip();    
    GPBDAT = 0b0101 << 5;
}

