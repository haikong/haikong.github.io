#ifndef S3C2440_ADDR_H_
#define S3C2440_ADDR_H_

#ifdef	__cplusplus
extern "C"{
#endif
/* WOTCH DOG register */
#define 	WTCON				(*(volatile unsigned long *)0x53000000)
/* SDRAM regisers */
#define 	MEM_CTL_BASE		0x48000000

// I/O PORT 
#define GPACON    (*(volatile unsigned *)0x56000000)	//Port A control
#define GPADAT    (*(volatile unsigned *)0x56000004)	//Port A data

#define GPBCON    (*(volatile unsigned *)0x56000010)	//Port B control
#define GPBDAT    (*(volatile unsigned *)0x56000014)	//Port B data
#define GPBUP     (*(volatile unsigned *)0x56000018)	//Pull-up control B

#define GPCCON    (*(volatile unsigned *)0x56000020)	//Port C control
#define GPCDAT    (*(volatile unsigned *)0x56000024)	//Port C data
#define GPCUP     (*(volatile unsigned *)0x56000028)	//Pull-up control C

#define GPDCON    (*(volatile unsigned *)0x56000030)	//Port D control
#define GPDDAT    (*(volatile unsigned *)0x56000034)	//Port D data
#define GPDUP     (*(volatile unsigned *)0x56000038)	//Pull-up control D

#define GPECON    (*(volatile unsigned *)0x56000040)	//Port E control
#define GPEDAT    (*(volatile unsigned *)0x56000044)	//Port E data
#define GPEUP     (*(volatile unsigned *)0x56000048)	//Pull-up control E

#define GPFCON    (*(volatile unsigned *)0x56000050)	//Port F control
#define GPFDAT    (*(volatile unsigned *)0x56000054)	//Port F data
#define GPFUP     (*(volatile unsigned *)0x56000058)	//Pull-up control F

#define GPGCON    (*(volatile unsigned *)0x56000060)	//Port G control
#define GPGDAT    (*(volatile unsigned *)0x56000064)	//Port G data
#define GPGUP     (*(volatile unsigned *)0x56000068)	//Pull-up control G

#define GPHCON    (*(volatile unsigned *)0x56000070)	//Port H control
#define GPHDAT    (*(volatile unsigned *)0x56000074)	//Port H data
#define GPHUP     (*(volatile unsigned *)0x56000078)	//Pull-up control H

#define GPJCON    (*(volatile unsigned *)0x560000d0)	//Port J control
#define GPJDAT    (*(volatile unsigned *)0x560000d4)	//Port J data
#define GPJUP     (*(volatile unsigned *)0x560000d8)	//Pull-up control J

/*UART registers*/
#define ULCON0              (*(volatile unsigned long *)0x50000000)
#define UCON0               (*(volatile unsigned long *)0x50000004)
#define UFCON0              (*(volatile unsigned long *)0x50000008)
#define UMCON0              (*(volatile unsigned long *)0x5000000c)
#define UTRSTAT0            (*(volatile unsigned long *)0x50000010)
#define UTXH0               (*(volatile unsigned char *)0x50000020)
#define URXH0               (*(volatile unsigned char *)0x50000024)
#define UBRDIV0             (*(volatile unsigned long *)0x50000028)

/*interrupt registes*/
#define SRCPND              (*(volatile unsigned long *)0x4A000000)
#define INTMOD              (*(volatile unsigned long *)0x4A000004)
#define INTMSK              (*(volatile unsigned long *)0x4A000008)
#define PRIORITY            (*(volatile unsigned long *)0x4A00000c)
#define INTPND              (*(volatile unsigned long *)0x4A000010)
#define INTOFFSET           (*(volatile unsigned long *)0x4A000014)
#define SUBSRCPND           (*(volatile unsigned long *)0x4A000018)
#define INTSUBMSK           (*(volatile unsigned long *)0x4A00001c) 
/*external interrupt registers*/
#define EINTMASK            (*(volatile unsigned long *)0x560000a4)
#define EINTPEND            (*(volatile unsigned long *)0x560000a8)
/* NAND Flash registers */
#define NFCONF              (*(volatile unsigned int  *)0x4e000000)
#define NFCMD               (*(volatile unsigned char *)0x4e000004)
#define NFADDR              (*(volatile unsigned char *)0x4e000008)
#define NFDATA              (*(volatile unsigned char *)0x4e00000c)
#define NFSTAT              (*(volatile unsigned char *)0x4e000010)
/*clock registers*/
#define	LOCKTIME		(*(volatile unsigned long *)0x4c000000)
#define	MPLLCON			(*(volatile unsigned long *)0x4c000004)
#define	UPLLCON			(*(volatile unsigned long *)0x4c000008)
#define	CLKCON			(*(volatile unsigned long *)0x4c00000c)
#define	CLKSLOW			(*(volatile unsigned long *)0x4c000010)
#define	CLKDIVN			(*(volatile unsigned long *)0x4c000014)
/*PWM & Timer registers*/
#define	TCFG0		(*(volatile unsigned long *)0x51000000)
#define	TCFG1		(*(volatile unsigned long *)0x51000004)
#define	TCON		(*(volatile unsigned long *)0x51000008)
#define	TCNTB0		(*(volatile unsigned long *)0x5100000c)
#define	TCMPB0		(*(volatile unsigned long *)0x51000010)
#define	TCNTO0		(*(volatile unsigned long *)0x51000014)
/*cpu id register*/
#define GSTATUS1    (*(volatile unsigned long *)0x560000B0)
/*DMA registers*/
//DMA³õÊ¼Ô´¼Ä´æÆ÷
#define DISRC0		(*(volatile unsigned long *)0x4B000000)
#define DISRC1		(*(volatile unsigned long *)0x4B000040)
#define DISRC2		(*(volatile unsigned long *)0x4B000080)
#define DISRC3		(*(volatile unsigned long *)0x4B0000c0)
//DMA³õÊ¼Ô´¿ØÖÆ¼Ä´æÆ÷
#define DISRCC0		(*(volatile unsigned long *)0x4B000004)
#define DISRCC1		(*(volatile unsigned long *)0x4B000044)
#define DISRCC2		(*(volatile unsigned long *)0x4B000084)
#define DISRCC3		(*(volatile unsigned long *)0x4B0000c4)
//DMA³õÊ¼Ä¿±ê¼Ä´æÆ÷
#define DIDST0		(*(volatile unsigned long *)0x4B000008)
#define DIDST1		(*(volatile unsigned long *)0x4B000048)
#define DIDST2		(*(volatile unsigned long *)0x4B000088)
#define DIDST3		(*(volatile unsigned long *)0x4B0000c8)
//DMA³õÊ¼Ä¿±ê¿ØÖÆ¼Ä´æÆ÷
#define DIDSTC0		(*(volatile unsigned long *)0x4B00000c)
#define DIDSTC1		(*(volatile unsigned long *)0x4B00004c)
#define DIDSTC2		(*(volatile unsigned long *)0x4B00008c)
#define DIDSTC3		(*(volatile unsigned long *)0x4B0000cc)
//DMA¿ØÖÆ¼Ä´æÆ÷
#define DCON0		(*(volatile unsigned long *)0x4B000010)
#define DCON1		(*(volatile unsigned long *)0x4B000050)
#define DCON2		(*(volatile unsigned long *)0x4B000090)
#define DCON3		(*(volatile unsigned long *)0x4B0000d0)
//£Ä£Í£Á×´Ì¬¼Ä´æÆ÷
#define DSTAT0		(*(volatile unsigned long *)0x4B000014)
#define DSTAT1		(*(volatile unsigned long *)0x4B000054)
#define DSTAT2		(*(volatile unsigned long *)0x4B000094)
#define DSTAT3		(*(volatile unsigned long *)0x4B0000d4)
//DMAµ±Ç°Ô´¼Ä´æÆ÷
#define DCSRC0		(*(volatile unsigned long *)0x4B000018)
#define DCSRC1		(*(volatile unsigned long *)0x4B000058)
#define DCSRC2		(*(volatile unsigned long *)0x4B000098)
#define DCSRC3		(*(volatile unsigned long *)0x4B0000d8)
//DMAµ±Ç°Ä¿±ê¼Ä´æÆ÷
#define DCDST0		(*(volatile unsigned long *)0x4B00001c)
#define DCDST1		(*(volatile unsigned long *)0x4B00005c)
#define DCDST2		(*(volatile unsigned long *)0x4B00009c)
#define DCDST3		(*(volatile unsigned long *)0x4B0000dc)
//DMA´¥·¢ÆÁ±Î¼Ä´æÆ÷
#define DMASKTRIG0		(*(volatile unsigned long *)0x4B000020)
#define DMASKTRIG1		(*(volatile unsigned long *)0x4B000060)
#define DMASKTRIG2		(*(volatile unsigned long *)0x4B0000A0)
#define DMASKTRIG3		(*(volatile unsigned long *)0x4B0000E0)
#ifdef	__cplusplus
}
#endif

#endif
