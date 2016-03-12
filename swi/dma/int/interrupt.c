#include <s3c2440_addr.h>

static inline void Timer0_Handle(void);
static inline void Dma0_Handle(void);
/*
*	C interrupt code entry
*/
void C_IRQ_Handler(int i,int j)
{
    unsigned long oft = INTOFFSET;
    unsigned long val;

    switch( oft )
    {
        // K4������
        case 0:
        {
            GPBDAT |= (0x0f<<5);   // ����LEDϨ��
            //GPBDAT &= ~(1<<8);      // LED4����
            GPBDAT &= ~(i & 0x80);
            break;
        }

        // K3������
        case 2:
        {
            GPBDAT |= (0x0f<<5);   // ����LEDϨ��
            //GPBDAT &= ~(1<<7);      // LED3����
            GPBDAT &= ~(j & 0x80);
            break;
        }

        // K1��K2������
        case 5:
        {
            GPBDAT |= (0x0f<<5);   // ����LEDϨ��

            // ��Ҫ��һ���ж���K1����K2������K1��K2��ͬʱ����
            val = EINTPEND;
            if (val & (1<<11))
                GPBDAT &= ~(1<<6);      // K2�����£�LED2����
            if (val & (1<<19))
                GPBDAT &= ~(1<<5);      // K1�����£�LED1����

            break;
        }
		case 10:	//timer0
		{
			Timer0_Handle();
		}
		break;
		case 17:	//dma0
		{
			Dma0_Handle();
		}
		break;
        default:
            break;
    }

    //���ж�
    if( oft == 5 )
        EINTPEND |= (1<<11) | (1<<19);   // EINT8_23����IRQ5
    SRCPND |= 1 << oft;
    INTPND |= 1 << oft;
}

/*timer0 hanlder*/
void Timer0_Handle(void)
{
	static int num = 0;
	//every 1s change the led status
	GPBDAT = (num & 0xf) << 5;
	num++;
}
extern int dma_flag;

/*dma0 interrupt handler*/
void Dma0_Handle(void)
{
     dma_flag = 1;
	/*set beep ring*/
	GPBDAT = 0b1001 << 5;
}
