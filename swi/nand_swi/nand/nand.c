#include <s3c2440_addr.h>
#include <nand.h>

pt_Nand_Chip g_Nand_Chip_op = NULL;

/* ��ʼ��NAND Flash */
void nand_init(void)
{
	if(g_Nand_Chip_op){
		if(g_Nand_Chip_op->nand_reg != NULL){
			/* ����ʱ�� */
		    g_Nand_Chip_op->nand_reg->NFCONF = (TACLS<<12)|(TWRPH0<<8)|(TWRPH1<<4);
		    /* ʹ��NAND Flash������, ��ʼ��ECC, ��ֹƬѡ */
		    g_Nand_Chip_op->nand_reg->NFCONT = (1<<4)|(1<<1)|(1<<0);
		    /* ��λNAND Flash */
		    g_Nand_Chip_op->nand_reset();

	    }
    }
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
    g_Nand_Chip_op->nand_select_chip();

    for(i=start_addr; i < (start_addr + size);) {
      /* ����READ0���� */
      g_Nand_Chip_op->write_cmd(0);
      /* Write Address */
      g_Nand_Chip_op->write_addr(i);
	  /*write 2nd cycle*/
      g_Nand_Chip_op->write_cmd(0x30);		
	  /*wait untile ready*/
      g_Nand_Chip_op->wait_idle();
	  /*read a page*/
      for(j=0; j < NAND_SECTOR_SIZE_LP; j++,i++) {
          *buf = g_Nand_Chip_op->read_data();
          buf++;
      }
    }
    /* ȡ��Ƭѡ�ź� */
    g_Nand_Chip_op->nand_deselect_chip();    
    GPBDAT = 0b0101 << 5;
}



void RegisterNandOpr(pt_Nand_Chip pt_nand_fun)
{
	if((g_Nand_Chip_op == NULL) && (pt_nand_fun != NULL))
		g_Nand_Chip_op = pt_nand_fun;
}

