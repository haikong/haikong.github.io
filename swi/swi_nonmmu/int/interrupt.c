#include <s3c2440_addr.h>

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

        default:
            break;
    }

    //���ж�
    if( oft == 5 ) 
        EINTPEND = (1<<11) | (1<<19);   // EINT8_23����IRQ5
    SRCPND = 1<<oft;
    INTPND = 1<<oft;
}
