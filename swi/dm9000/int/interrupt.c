#include <s3c2440_addr.h>
#include <intirq.h>
#include <stdio.h>
#include <glob.h> 

static void (*isr_handle_array[MAX_IRQ])(unsigned int) ;
static void (*externINT_handle_array[EXTERN_MAX_IRQ])(unsigned int) ;

static void Dummy_isr(unsigned int vector)
{
    printf("Dummy_isr:%d\n\r",vector);
    while(1);
}

static void extern_isr(unsigned int vector)
{
	unsigned int i;
	unsigned int int_pend;
	int_pend = EINTPEND;
	i = fbit_pend(int_pend);
	if(i > 31)
		Dummy_isr(vector);
	else
		externINT_handle_array[i](vector);
	EINTPEND |= (1 << (i % 32)); 
}
//initialize all the ISR
void init_irq(void)
{
    int i;
	//for all interrput
    for (i = 0; i < sizeof(isr_handle_array) / sizeof(isr_handle_array[0]); i++)
    {
    	if((i == ISR_EINT4_7_OFT) || (i == ISR_EINT8_23_OFT))
			isr_handle_array[i] = extern_isr;
		else	
        	isr_handle_array[i] = Dummy_isr;
		
    }
	//for all extern interrput
	for (i = 0; i < sizeof(externINT_handle_array) / sizeof(externINT_handle_array[0]); i++)
    {
        externINT_handle_array[i] = Dummy_isr;
    }
    INTMOD = 0x0;	     	 			// �����ж϶���ΪIRQģʽ
    INTMSK = BIT_ALLMSK;  				// �����������ж�
    INTSUBMSK = BIT_SUB_ALLMSK;		 	// �����������ж�
    EINTMASK = 0xfffff0;				// mask all the extern innterrput 4-23
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
int register_interrupt(unsigned int vector_num,void (*vector_handle)(unsigned int))
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

/*****************************************************************************
 �� �� ��  : register_extern_int
 ��������  : register the extern 4-23 interrput
 �������  : unsigned int
 �������  : ��
 �� �� ֵ  : int
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��4��22��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

*****************************************************************************/
int register_extern_int( unsigned int vector_num,void (*vector_handle)(unsigned int))
{
    
    if((vector_num > (EXTERN_MAX_IRQ - 1)) || (vector_num < 4)){
        printf("Invalid inteerupt vector num.\n\r");
        return -1;
    }
    externINT_handle_array[vector_num] = vector_handle;
	EINTMASK &= ~(1 << vector_num);
	if(vector_num < 8)
		INTMSK &= ~(1 << 4);
	else
		INTMSK &= ~(1 << 5);
	return 0;
}


/*
*	C interrupt code entry
*/
void C_IRQ_Handler(int i,int j)
{
	unsigned long oft = INTOFFSET;
	/* �����жϷ������ */
    isr_handle_array[oft](oft);
	//����ж�Դ
	SUBSRCPND = SUBSRCPND;
	SRCPND |= 1 << oft;
	INTPND = INTPND;
}


