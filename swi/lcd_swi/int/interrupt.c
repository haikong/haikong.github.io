#include <s3c2440_addr.h>
#include <intirq.h>
#include <stdio.h>
 
void (*isr_handle_array[MAX_IRQ])(void) ;

static void Dummy_isr(void)
{
    printf("Dummy_isr\n\r");
    while(1);
}

void init_irq(void)
{
    int i = 0;
    for (i = 0; i < sizeof(isr_handle_array) / sizeof(isr_handle_array[0]); i++)
    {
        isr_handle_array[i] = Dummy_isr;
    }
    INTMOD = 0x0;	      // 所有中断都设为IRQ模式
    INTMSK = BIT_ALLMSK;  // 先屏蔽所有中断
}

int register_interrupt(unsigned int vector_num,void (*vector_handle)(void))
{
    if(vector_num > (MAX_IRQ - 1)){
        printf("Invalid inteerupt vector num.\n");
        return -1;
    }
    isr_handle_array[vector_num] = vector_handle;
    INTMSK &= ~(1 << vector_num);
    return 0;
}

/*
*	C interrupt code entry
*/
void C_IRQ_Handler(int i,int j)
{
	unsigned long oft = INTOFFSET;

	//清中断
	if (oft == 4)
        EINTPEND = 1 << 7;    //EINT4-7合用IRQ4，注意EINTPEND[3:0]保留未用，向这些位写入1可能导致未知结果
	SRCPND = 1 << oft;
	INTPND = INTPND;

    /* 调用中断服务程序 */
    isr_handle_array[oft]();
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
