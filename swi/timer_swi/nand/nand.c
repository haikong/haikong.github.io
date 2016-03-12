#include <s3c2440_addr.h>
#include <nand.h>

pt_Nand_Chip g_Nand_Chip_op = NULL;

/* 初始化NAND Flash */
void nand_init(void)
{
	if(g_Nand_Chip_op){
		if(g_Nand_Chip_op->nand_reg != NULL){
		/* 设置时序 */
	    g_Nand_Chip_op->nand_reg->NFCONF = (TACLS<<12)|(TWRPH0<<8)|(TWRPH1<<4);
	    /* 使能NAND Flash控制器, 初始化ECC, 禁止片选 */
	    g_Nand_Chip_op->nand_reg->NFCONT = (1<<4)|(1<<1)|(1<<0);
	    /* 复位NAND Flash */
	    g_Nand_Chip_op->nand_reset();
    }
}
}



/* 读函数 */
void nand_read(unsigned char *buf, unsigned long start_addr, int size)
{
    int i, j;

#ifdef LARGER_NAND_PAGE
    if ((start_addr & NAND_BLOCK_MASK_LP) || (size & NAND_BLOCK_MASK_LP)) {
        return ;    /* 地址或长度不对齐 */
    }
#else
    if ((start_addr & NAND_BLOCK_MASK) || (size & NAND_BLOCK_MASK)) {
        return ;    /* 地址或长度不对齐 */
    }
#endif	

    /* 选中芯片 */
    g_Nand_Chip_op->nand_select_chip();

    for(i=start_addr; i < (start_addr + size);) {
      /* 发出READ0命令 */
      g_Nand_Chip_op->write_cmd(0);

      /* Write Address */
      g_Nand_Chip_op->write_addr(i);
#ifdef LARGER_NAND_PAGE
      g_Nand_Chip_op->write_cmd(0x30);		
#endif
      g_Nand_Chip_op->wait_idle();

#ifdef LARGER_NAND_PAGE
      for(j=0; j < NAND_SECTOR_SIZE_LP; j++, i++) {
#else
	  for(j=0; j < NAND_SECTOR_SIZE; j++, i++) {
#endif
          *buf = g_Nand_Chip_op->read_data();
          buf++;
      }
    }
    /* 取消片选信号 */
    g_Nand_Chip_op->nand_deselect_chip();    

}



void RegisterNandOpr(pt_Nand_Chip pt_nand_fun)
{
	if((g_Nand_Chip_op == NULL) && (pt_nand_fun != NULL))
		g_Nand_Chip_op = pt_nand_fun;
}

