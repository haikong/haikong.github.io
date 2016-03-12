@ File：crt0.s
@ 功能：设置SDRAM，将程序复制到SDRAM，然后跳到SDRAM继续执行
@******************************************************************************       
.extern main C_SWI_Handler C_IRQ_Handler sys_call_tbl memsetup copy_steppingstone_to_sdram 
.global SWI_Handler
.equ MAX_CALL,10
@******************
@text code
@******************
.text
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

@系统复位
Reset:	
	ldr sp,=4096						@user mode sp
	bl  diable_watch_dog
	bl  init_led
    bl  clock_init          			@ 设置MPLL，改变FCLK、HCLK、PCLK
    bl  memsetup            			@ 设置存储控制器以使用SDRAM
    bl  copy_steppingstone_to_sdram     @ 复制代码到SDRAM中	
    @svc mode stack
	msr cpsr_c,#0xd3					@设置svc模式
	ldr sp,=4096						@svc mode sp
	@irq mode stack		
	msr cpsr_c,#0xd2					@设置irq模式
	ldr sp,=3072						@irq mode stack
	@进入系统模式,开中断
	msr cpsr_c,#0x1f		
    ldr pc, =on_sdram                   @ 跳到SDRAM中继续执行
on_sdram:
    ldr sp, =0x34000000   	  			@ 设置栈指针，虚拟地址
    ldr lr, =halt_loop      			@ 设置返回地址
    ldr pc, =main           			@ 调用main函数
halt_loop:
	b	halt_loop
	
@irq中断服务程序	
IRQ_Handler:
	sub lr,lr,#4
	stmfd sp!,{r0-r12,lr}
	ldr lr,=int_return 
	ldr pc,=C_IRQ_Handler
int_return:
	ldmfd sp!,{r0-r12,pc}^		
@swi服务程序
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
	bleq	C_SWI_Handler
	blne	SysCallEntry
	ldmfd sp!,{r4-r12,pc}^
@系统调用
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
										