#include <s3c2440_addr.h>
#include <mmu_init.h>
/*
 * ����ҳ��,ҳ�����ַ0x30000000
 */
void create_page_table(void)
{
    unsigned long virtuladdr, physicaladdr;
    unsigned long *mmu_tlb_base = (unsigned long *)0x30000000;
    
    /*
     * Steppingstone����ʼ�����ַΪ0����һ���ֳ������ʼ���е�ַҲ��0��
     * Ϊ���ڿ���MMU���������е�һ���ֵĳ���
     * ��0��1M�������ַӳ�䵽ͬ���������ַ
     */
    virtuladdr = 0;
    physicaladdr = 0;
    *(mmu_tlb_base + (virtuladdr >> 20)) = (physicaladdr & 0xFFF00000) | \
                                            MMU_SECDESC_WB;

    /*
     * 0x56000000��GPIO�Ĵ�������ʼ�����ַ��
     * GPBCON��GPBDAT�������Ĵ����������ַ0x56000010��0x56000014��
     * Ϊ���ڵڶ����ֳ��������Ե�ַ0xA0000010��0xA0000014������GPBCON��GPBDAT��
     * �Ѵ�0xA0000000��ʼ��1M�����ַ�ռ�ӳ�䵽��0x56000000��ʼ��1M�����ַ�ռ�
     */
    virtuladdr = 0xA0000000;
    physicaladdr = 0x56000000;
    *(mmu_tlb_base + (virtuladdr >> 20)) = (physicaladdr & 0xFFF00000) | \
                                            MMU_SECDESC;

    /*
     * SDRAM�������ַ��Χ��0x30000000��0x33FFFFFF��
     * �������ַ0xB0000000��0xB3FFFFFFӳ�䵽�����ַ0x30000000��0x33FFFFFF�ϣ�
     * �ܹ�64M���漰64����������
     */
    virtuladdr = 0xB0000000;
    physicaladdr = 0x30000000;
    while (virtuladdr < 0xB4000000)
    {
        *(mmu_tlb_base + (virtuladdr >> 20)) = (physicaladdr & 0xFFF00000) | \
                                                MMU_SECDESC_WB;
        virtuladdr += 0x100000;
        physicaladdr += 0x100000;
    }
}

