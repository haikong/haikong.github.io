#ifndef NAND_H_
#define NAND_H_

#ifdef	__cplusplus
extern "C"{
#endif

#define LARGER_NAND_PAGE 	1		/*select nand:K9XXX08UOA*/
#define TACLS	0					/*CLE & ALE duration setting value (0~3),Duration = HCLK x TACLS--tcls/tals*/
#define TWRPH0	3					/*TWRPH0 duration setting value (0~7),Duration = HCLK x ( TWRPH0 + 1 )--twp*/
#define TWRPH1	0					/*TWRPH1 duration setting value (0~7),Duration = HCLK x ( TWRPH1 + 1 )--tclh/talh*/
#define BUSY    1					/*nand chip busy status*/
#define NAND_SECTOR_SIZE    512		/*page's size 512bytes*/
#define NAND_BLOCK_MASK     (NAND_SECTOR_SIZE - 1)
#define NAND_SECTOR_SIZE_LP 2048							/*large nand page size,nand:K9XXX08UOA*/
#define NAND_BLOCK_MASK_LP  (NAND_SECTOR_SIZE_LP - 1)		/*row address mask*/
typedef unsigned int S3C24X0_REG32;	/*register type*/
/*nand */
typedef struct S3C2440_NAND{
    S3C24X0_REG32   NFCONF;			/*NAND flash configuration register*/
    S3C24X0_REG32   NFCONT;			/*NAND flash control register*/
    S3C24X0_REG32   NFCMMD;			/*NAND flash command set register*/
    S3C24X0_REG32   NFADDR;			/*NAND flash address set register*/
    S3C24X0_REG32   NFDATA;			/*NAND flash data register*/
    S3C24X0_REG32   NFMECCD0;		/*NAND Flash ECC 1stand 2ndregister for main data read*/
    S3C24X0_REG32   NFMECCD1;		/*NAND Flash ECC 3stand 4ndregister for main data read*/
    S3C24X0_REG32   NFSECCD;		/*NAND flash ECC (Error Correction Code)register for spare area data read*/
    S3C24X0_REG32   NFSTAT;			/*NAND flash operation status register*/
    S3C24X0_REG32   NFESTAT0;		/*NAND flash ECC Status register for I/O [7:0]*/
    S3C24X0_REG32   NFESTAT1;		/*NAND flash ECC status register for I/O [15:8] */
    S3C24X0_REG32   NFMECC0;		/*NAND flash ECC register for data[7:0]*/
    S3C24X0_REG32   NFMECC1;		/*NAND flash ECC register for data[15:8]*/
    S3C24X0_REG32   NFSECC;			/*NAND flash ECC register for I/O [15:0]*/
    S3C24X0_REG32   NFSBLK;			/*NAND flash programmable start block address*/
    S3C24X0_REG32   NFEBLK;			/*NAND flash programmable end block address*/
}t_S3C2440_NAND,*pt_S3C2440_NAND;

/*nand chip opreation functions*/
typedef struct {
	pt_S3C2440_NAND nand_reg;				/*nand chip register*/
    void (*nand_reset)(void);				/*nand chip reset*/
    void (*wait_idle)(void);				/*nand chip wait idle*/
    void (*nand_select_chip)(void);			/*nand chip select */
    void (*nand_deselect_chip)(void);		/*nand chip cancel select*/
    void (*write_cmd)(int cmd);				/*nand chip write command */
    void (*write_addr)(unsigned int addr);	/*nand chip write address*/
    unsigned char (*read_data)(void);		/*nand chip read data*/
}t_Nand_Chip,*pt_Nand_Chip;


/* 供外部调用的函数 */
void RegisterNandOpr(pt_Nand_Chip pt_nand_fun) ;
void nand_init(void);						/*initialize nand chip*/
void nand_read(unsigned char *buf, unsigned long start_addr, int size) ;	/*nand chip read data*/
void nand_driver(void) ;

#ifdef	__cplusplus
}
#endif

#endif
