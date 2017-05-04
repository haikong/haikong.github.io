#include <s3c2440_addr.h>
#include <swi.h>

static void Delay_ms(unsigned int num_ms)
{
	int i = 0;
	while(num_ms--)
		for(i = 0;i < 11000;i++);	
}

void C_SWI_Handler(int swi_num,int *regs)
{
	int i = 0;
	regs = regs;
	switch(swi_num){
	//led0 on
	case 0:
		for(i = 0;i < 10;i++){
			GPBDAT |= (1<<5);
			Delay_ms(1000);
			GPBDAT &= ~(1<<5);       // LED1ÁÁ
			Delay_ms(1000);
			
		}
		break;
	case 1:
		GPBDAT &= ~(1<<6);       // LED2ÁÁ
		break;
	case 2:
		GPBDAT &= ~(1<<7);       // LED3ÁÁ
		break;
	case 3:
		GPBDAT &= ~(1<<8);       // LED4ÁÁ
		break;
	default:
		GPBDAT = 0xffffffff;	//LED È«Ï¨Ãð
		break;		
	}
}

void myswi(void)
{
	__asm__ __volatile__("swi 0\n\t");	
}
