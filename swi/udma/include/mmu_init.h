#ifndef MMU_INIT_H_
#define MMU_INIT_H_

#ifdef	__cplusplus
extern "C"{
#endif

/* 
 * ���ڶ���������һЩ�궨��
 * Bits[1:0]  ���������ͱ�ʶ�� 0b10 ��ʾ����������
 * Bits[3:2]  ���ٻ���ͻ���λ
 * Bits[4]     �ɾ���ʵ�ֶ���
 * Bits[8:5]  ������������ƵĽڵ� 16 ����֮һ
 * Bits[9]     ����û��ʹ�ã�Ӧ��Ϊ��
 * Bits[11:10]  ���ʿ��ƣ����� 3-3
				 �� 3-3 MMU ����Ȩ��
				AP 	  S  R  Privileged permissions User permissions
				0b00  0  0  ���ܷ���    				���ܷ���
				0b00  1  0  ֻ��        				���ܷ���
				0b00  0  1  ֻ��        				ֻ��
				0b00  1  1  ����Ԥ��    				����Ԥ��
				0b01  X  X  ����д      				���ܷ���
				0b10  X  X  ����д      				ֻ��
				0b11  X  X  ����д      				����д  
 * Bits[19:12]  ����û��ʹ�ã�Ӧ��Ϊ��
 * Bits[31:20]  �λ�ַ���γ������ַ�ĸ� 12 λ
*/ 
#define MMU_FULL_ACCESS     (3 << 10)   /* ����Ȩ�� */
#define MMU_DOMAIN          (0 << 5)    /* �����ĸ��� */
#define MMU_SPECIAL         (1 << 4)    /* ������1 */
#define MMU_CACHEABLE       (1 << 3)    /* cacheable */
#define MMU_BUFFERABLE      (1 << 2)    /* bufferable �仯д������*/
#define MMU_SECTION         (2)         /* ��ʾ���Ƕ������� */

#define MMU_SECDESC         (MMU_FULL_ACCESS | MMU_DOMAIN | MMU_SPECIAL | MMU_SECTION)
                             
#define MMU_SECDESC_WB      (MMU_FULL_ACCESS | MMU_DOMAIN | MMU_SPECIAL | MMU_CACHEABLE | MMU_BUFFERABLE | MMU_SECTION)
                             
                             
#define MMU_SECTION_SIZE    0x00100000	/*�δ�С*/
 
void create_page_table(void) __attribute__ ((__section__(".init")));
void mmu_init(void) __attribute__ ((__section__(".init"))); 
#ifdef	__cplusplus
}
#endif

#endif
