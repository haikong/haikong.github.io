#include <miscellaneous.h>
#include <s3c2440_addr.h>
#include <stdio.h>
/*init led*/
void init_led(void)
{
    // LED1-LED4��Ӧ��4��������Ϊ���
    GPBCON = GPB5_out | GPB6_out | GPB7_out | GPB8_out ;
    //LEDȫ��Ϩ��
    GPBDAT = ~0;
}

/*****************************************************************************
 �� �� ��  :key_init
 ��������  :setting key mode,K1-K4 uses GPFCON:0,1,2,4
 �������  :unsigned char key 
 			unsigned char mode
 			 0:input
 			 1:output
 			 2:interrput
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��9��18��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

*****************************************************************************/
void key_init( unsigned char key,unsigned char mode )
{
	key %= 32;
	mode %= 3;
	
	GPFCON &= ~(3 << (key * 2));
	GPFCON |= mode << (key * 2);
}
/*key2 hanlder*/
void KeyINT2_Handle(unsigned int vec_num)
{
	GPBDAT = ~GPBDAT;
	printf("KeyINT2_Handle\n\t");
}

/*key3 hanlder*/
void KeyINT3_Handle(unsigned int vec_num)
{
	printf("KeyINT3_Handle\n\t");
}

/*key4 hanlder*/
void KeyINT4_Handle(unsigned int vec_num)
{
	printf("KeyINT4_Handle\n\t");
}

