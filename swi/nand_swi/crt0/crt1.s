.text
.global _start
_start:
ldr pc,Reset_Addr
ldr pc,Undefined_Addr	
ldr pc,SWI_Addr
ldr pc,Prefetch_Addr
ldr pc,Abort_Addr
nop
ldr pc,IRQ_Addr
ldr pc,FIQ_Addr

Reset_Addr:
.word Reset
Undefined_Addr:
.long Undefined_Handler
SWI_Addr:
.long	SWI_Handler
Prefetch_Addr:
.long	Prefetch_Handler
Abort_Addr:
.long	Abort_Handler
.long 0
IRQ_Addr:
.long	IRQ_Handler
FIQ_Addr:
.long	FIQ_Handler

.extern main C_SWI_Handler C_IRQ_Handler sys_call_tbl
.global SWI_Handler
.equ MAX_CALL,10

Reset:	
	ldr sp,=4096				@user mode sp
	bl diable_watch_dog
	msr cpsr_c,#0xd2			
	ldr sp,=3072				@irq mode sp
	msr cpsr_c,#0xdf			
	ldr sp,=4096				@system mode sp
	bl init_led
	msr cpsr_c,#0x5f
	ldr lr,=halt_loop
	ldr pc,=main
halt_loop:
	b halt_loop
	
IRQ_Handler:
	sub lr,lr,#4
	stmfd sp!,{r0-r12,lr}
	ldr lr,=int_return 
	ldr pc,=C_IRQ_Handler
int_return:
	ldmfd sp!,{r0-r12,pc}^		

SWI_Handler:
	stmfd sp!,{r4-r12,lr}
	mov	r2,r0
	ldr r0,[lr,#-4]
	bic	r0,r0,#0xff000000
	cmp r0,#0
	bleq	C_SWI_Handler
	blne	SysCallEntry
	ldmfd sp!,{r4-r12,pc}^

Undefined_Handler:
	b 	Undefined_Handler
	
Prefetch_Handler:
	b 	Prefetch_Handler

Abort_Handler:
	b 	Abort_Handler

FIQ_Handler:
	b 	FIQ_Handler			

SysCallEntry:
	stmfd sp!,{lr}
	cmp r2,#MAX_CALL
	blgt bad_call
	mov r3,r2
	mov r0,r1
	mov r2,sp
	ldr r5,=sys_call_tbl
	ldr lr,=bad_call
	ldr pc,[r5,r3,lsl #2]
bad_call:
	ldmfd sp!,{pc}^	
										