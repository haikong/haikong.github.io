#include <s3c2440_addr.h>
#include <intirq.h>
#include <stdio.h>
 
static void (*isr_handle_array[MAX_IRQ])(void) ;

static void Dummy_isr(void)
{
    printf("Dummy_isr\n\r");
    while(1);
}

//initialize all the ISR
void init_irq(void)
{
    int i = 0;
    for (i = 0; i < sizeof(isr_handle_array) / sizeof(isr_handle_array[0]); i++)
    {
        isr_handle_array[i] = Dummy_isr;
    }
    INTMOD = 0x0;	     	 			// 所有中断都设为IRQ模式
    INTMSK = BIT_ALLMSK;  				// 先屏蔽所有中断
    INTSUBMSK = BIT_SUB_ALLMSK;		 	// 先屏蔽所有中断
}

static void subint_mask(unsigned int vector_num)
{
	switch(vector_num){
	case ISR_CAM_OFT:
		INTSUBMSK &= ~(0x3 << 11);	
		break;
	case ISR_WDT_OFT:
		INTSUBMSK &= ~(0x3 << 13);	
		break;
	case ISR_UART2_OFT:
		INTSUBMSK &= ~(0x7 << 6);	
		break;
	case ISR_UART1_OFT:
		INTSUBMSK &= ~(0x7 << 3);	
		break;
	case ISR_UART0_OFT:
		INTSUBMSK &= ~(0x7 << 0);	
		break;
	case ISR_ADC_OFT:
		INTSUBMSK &= ~(0x3 << 9);	
		break;
	default:
		break;
	}
}

/*
*注册中断ISR
*/
int register_interrupt(unsigned int vector_num,void (*vector_handle)(void))
{
    if(vector_num > (MAX_IRQ - 1)){
        printf("Invalid inteerupt vector num.\n");
        return -1;
    }
    isr_handle_array[vector_num] = vector_handle;
    INTMSK &= ~(1 << vector_num);
	subint_mask(vector_num);
    return 0;
}

/*
*	C interrupt code entry
*/
void C_IRQ_Handler(int i,int j)
{
	unsigned long oft = INTOFFSET;
	
	/* 调用中断服务程序 */
    isr_handle_array[oft]();
	//清外部中断4-23
	if ((oft == 4) || (oft == 5))
        EINTPEND = 0xfffff0;    //EINT4-7合用IRQ4，EINT8-23合用IRQ5,注意EINTPEND[3:0]保留未用，向这些位写入1可能导致未知结果
	//清除中断源
	SUBSRCPND = SUBSRCPND;
	SRCPND |= 1 << oft;
	INTPND = INTPND;
}

/*timer0 hanlder*/
void Timer0_Handle(void)
{
	static int num = 0;
	//every 1s change the led status
	GPBDAT = (num & 0xf) << 5;
	num++;
}

/*dma0 interrupt handler*/
void Dma0_Handle(void)
{
	printf("finish the transport.\n");
	/*set beep ring*/
	GPBDAT = 0b1001 << 5;
}
