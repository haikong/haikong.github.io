#include <glob.h>

/*****************************************************************************
 �� �� ��  : fbit_pend
 ��������  : get the first bit 1 from the interger
 �������  : unsigned int num
 �������  : ��
 �� �� ֵ  : unsigned int
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��4��22��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

*****************************************************************************/
unsigned int fbit_pend( unsigned int num )
{
    int i;
	i = 0;
	do{
		if((num & 0x1) == 0x1)
			break;
		num = num >>1;
		i++;
	}while(num);
	return i;
}

