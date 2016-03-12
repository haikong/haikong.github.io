@ File��crt0.s
@ ���ܣ�����SDRAM���������Ƶ�SDRAM��Ȼ������SDRAM����ִ��
@******************************************************************************       
.extern main C_SWI_Handler C_IRQ_Handler sys_call_tbl memsetup copy_steppingstone_to_sdram 
.global SWI_Handler
.equ MAX_CALL,10
@******************
@text code
@******************
.section .init
.global _start
_start:
	b  Reset
Undefined_Handler:
	b 	Undefined_Handler
	b	SWI_Handler
Prefetch_Handler:	
	b	Prefetch_Handler
Abort_Handler:
	b	Abort_Handler
	nop
	b	IRQ_Handler
FIQ_Handler:	
	b	FIQ_Handler

@ϵͳ��λ
Reset:	
	ldr sp,=4096						@user mode sp
	bl  disable_watch_dog
	bl  init_led
    bl  clock_init          			@ ����MPLL���ı�FCLK��HCLK��PCLK
    bl  memsetup            			@ ���ô洢��������ʹ��SDRAM
    bl  nand_init						@ nand initialize
    @bl  create_page_table			    @����ҳ��
    @svc mode stack
	msr cpsr_c,#0xd3					@����svcģʽ
	ldr sp,=4096						@svc mode sp
	@irq mode stack		
	msr cpsr_c,#0xd2					@����irqģʽ
	ldr sp,=3072						@irq mode stack
	@����ϵͳģʽ,���ж�
	msr cpsr_c,#0x1f
	@bl	mmu_init						@����MMU	
    ldr     r0,=0x30000000     			@1. Ŀ���ַ=0x30000000������SDRAM����ʼ��ַ
    mov     r1,#4096           			@2.  Դ��ַ = 4096�����ӵ�ʱ�򣬴��붼����NAND Flash��ַ4096��ʼ��
    mov     r2,#4096           			@3.  ���Ƴ���= 4096(bytes)
    bl      nand_read               	@����C����nand_read
	ldr     lr, =halt_loop          	@���÷��ص�ַ				
    ldr sp,=0x34000000   	  			@ ����ջָ��
    ldr pc,=main         				@ ����main����
halt_loop:
	b	halt_loop
	
@irq�жϷ������	
IRQ_Handler:
	sub lr,lr,#4
	stmfd sp!,{r0-r12,lr}
	ldr lr,=int_return 
	ldr pc,=C_IRQ_Handler
int_return:
	ldmfd sp!,{r0-r12,pc}^		
@swi�������
SWI_Handler:
	stmfd sp!,{r4-r12,lr}
	@bl  printf_word			@r0 -->the syscall num
	@mov r0,r1
	@bl  printf_word			@r1 -->the syscall  arg
	mov	r2,r0
	ldr r0,[lr,#-4]
	@bl  printf_word			@swi #x -->machine code
	bic	r0,r0,#0xff000000
	cmp r0,#0
	ldr lr,=SWI_RETURN
	ldr pc,=C_SWI_Handler
	blne	SysCallEntry
SWI_RETURN:	
	ldmfd sp!,{r4-r12,pc}^
@ϵͳ����
SysCallEntry:
	stmfd sp!,{lr}
	cmp r2,#MAX_CALL
	blgt bad_call
	@mov r0,r2				@the syscall num
	@bl  printf_word
	mov r0,r1				@r0 = arg1
	mov r1,sp				@r1 = sp
	ldr r4,=sys_call_tbl
	@ldr r0,[r4,r2,lsl #2]
	@bl  printf_word
	ldr lr,=bad_call
	ldr pc,[r4,r2,lsl #2]
bad_call:
	ldmfd sp!,{pc}	
										