#ifndef MMU_INIT_H_
#define MMU_INIT_H_

#ifdef	__cplusplus
extern "C"{
#endif

/* 
 * 用于段描述符的一些宏定义
 * Bits[1:0]  描述符类型标识（ 0b10 表示节描述符）
 * Bits[3:2]  高速缓存和缓冲位
 * Bits[4]     由具体实现定义
 * Bits[8:5]  这个描述符控制的节的 16 种域之一
 * Bits[9]     现在没有使用，应该为零
 * Bits[11:10]  访问控制，见表 3-3
				 表 3-3 MMU 访问权限
				AP 	  S  R  Privileged permissions User permissions
				0b00  0  0  不能访问    				不能访问
				0b00  1  0  只读        				不能访问
				0b00  0  1  只读        				只读
				0b00  1  1  不可预测    				不可预测
				0b01  X  X  读／写      				不能访问
				0b10  X  X  读／写      				只读
				0b11  X  X  读／写      				读／写  
 * Bits[19:12]  现在没有使用，应该为零
 * Bits[31:20]  段基址，形成物理地址的高 12 位
*/ 
#define MMU_FULL_ACCESS     (3 << 10)   /* 访问权限 */
#define MMU_DOMAIN          (0 << 5)    /* 属于哪个域 */
#define MMU_SPECIAL         (1 << 4)    /* 必须是1 */
#define MMU_CACHEABLE       (1 << 3)    /* cacheable */
#define MMU_BUFFERABLE      (1 << 2)    /* bufferable 变化写入主存*/
#define MMU_SECTION         (2)         /* 表示这是段描述符 */

#define MMU_SECDESC         (MMU_FULL_ACCESS | MMU_DOMAIN | MMU_SPECIAL | MMU_SECTION)
                             
#define MMU_SECDESC_WB      (MMU_FULL_ACCESS | MMU_DOMAIN | MMU_SPECIAL | MMU_CACHEABLE | MMU_BUFFERABLE | MMU_SECTION)
                             
                             
#define MMU_SECTION_SIZE    0x00100000	/*段大小*/
 
void create_page_table(void) __attribute__ ((__section__(".init")));
void mmu_init(void) __attribute__ ((__section__(".init"))); 
#ifdef	__cplusplus
}
#endif

#endif
