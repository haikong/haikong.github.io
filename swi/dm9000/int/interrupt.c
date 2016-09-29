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
	printf("%s-%s-%dline,i = %d\n\t",__FILE__,__FUNCTION__,__LINE__,i);
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
    INTMOD = 0x0;	     	 			// 所有中断都设为IRQ模式
    INTMSK = BIT_ALLMSK;  				// 先屏蔽所有中断
    INTSUBMSK = BIT_SUB_ALLMSK;		 	// 先屏蔽所有中断
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
*注册中断ISR
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
 函 数 名  : register_extern_int
 功能描述  : register the extern 4-23 interrput
 输入参数  : unsigned int
 输出参数  : 无
 返 回 值  : int
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年4月22日
    作    者   : QSWWD
    修改内容   : 新生成函数

*****************************************************************************/
int register_extern_int( unsigned int vector_num,void (*vector_handle)(unsigned int))
{
    
    if((vector_num > (EXTERN_MAX_IRQ - 1)) || (vector_num < 4)){
        printf("Invalid inteerupt vector num.\n");
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
	/* 调用中断服务程序 */
    isr_handle_array[oft](oft);
#if 0	
	//清外部中断4-23
	if ((oft == 4) || (oft == 5))
        EINTPEND = 0xfffff0;    //EINT4-7合用IRQ4，EINT8-23合用IRQ5,注意EINTPEND[3:0]保留未用，向这些位写入1可能导致未知结果
#endif
	//清除中断源
	SUBSRCPND = SUBSRCPND;
	SRCPND |= 1 << oft;
	INTPND = INTPND;
}


