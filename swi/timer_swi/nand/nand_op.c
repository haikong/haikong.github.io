#include <s3c2440_addr.h>
#include <nand.h>
/*setting the nand register struct start address */
static pt_S3C2440_NAND  s3c2440nand  = (pt_S3C2440_NAND)0x4e000000;

/* S3C2440的NAND Flash处理函数 */
static void s3c2440_nand_reset(void) ;
static void s3c2440_wait_idle(void) ;
static void s3c2440_nand_select_chip(void) ;
static void s3c2440_nand_deselect_chip(void);
static void s3c2440_write_cmd(int cmd);
static void s3c2440_write_addr(unsigned int addr);
static unsigned char s3c2440_read_data(void) ;
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
/* S3C2440的NAND Flash操作函数 */

/* 复位 */
static void s3c2440_nand_reset(void)
{
    s3c2440_nand_select_chip();
    s3c2440_write_cmd(0xff);  // 复位命令
    s3c2440_wait_idle();
    s3c2440_nand_deselect_chip();
}

/* 等待NAND Flash就绪 */
static void s3c2440_wait_idle(void)
{
    int i;
    volatile unsigned char *p = (volatile unsigned char *)&s3c2440nand->NFSTAT;
    while(!(*p & BUSY))
        for(i=0; i<10; i++);
}

/* 发出片选信号 */
static void s3c2440_nand_select_chip(void)
{
    int i;
    s3c2440nand->NFCONT &= ~(1<<1);
    for(i=0; i<10; i++);    
}

/* 取消片选信号 */
static void s3c2440_nand_deselect_chip(void)
{
    s3c2440nand->NFCONT |= (1<<1);
}

/* 发出命令 */
static void s3c2440_write_cmd(int cmd)
{
    volatile unsigned char *p = (volatile unsigned char *)&s3c2440nand->NFCMMD;
    *p = cmd;
}

/*发出地址*/
static void s3c2440_write_addr(unsigned int addr)
{
	int i;
	volatile unsigned char *p = (volatile unsigned char *)&s3c2440nand->NFADDR;
#ifdef LARGER_NAND_PAGE
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
#else
    *p = addr & 0xff;
    for(i=0; i<10; i++);
    *p = (addr >> 9) & 0xff;
    for(i=0; i<10; i++);
    *p = (addr >> 17) & 0xff;
    for(i=0; i<10; i++);
    *p = (addr >> 25) & 0xff;
    for(i=0; i<10; i++);
#endif	
}


/* 读取数据 */
static unsigned char s3c2440_read_data(void)
{
    volatile unsigned char *p = (volatile unsigned char *)&s3c2440nand->NFDATA;
    return *p;
}

void nand_driver(void)
{
	RegisterNandOpr(&nand_chip);
}
