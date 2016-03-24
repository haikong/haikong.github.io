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
    INTMOD = 0x0;	     	 			// �����ж϶���ΪIRQģʽ
    INTMSK = BIT_ALLMSK;  				// �����������ж�
    INTSUBMSK = BIT_SUB_ALLMSK;		 	// �����������ж�
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
*ע���ж�ISR
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
	
	/* �����жϷ������ */
    isr_handle_array[oft]();
	//���ⲿ�ж�4-23
	if ((oft == 4) || (oft == 5))
        EINTPEND = 0xfffff0;    //EINT4-7����IRQ4��EINT8-23����IRQ5,ע��EINTPEND[3:0]����δ�ã�����Щλд��1���ܵ���δ֪���
	//����ж�Դ
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
