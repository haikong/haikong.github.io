#include <net.h>

/*****************************************************************************
 �� �� ��  : cal_chksum
 ��������  : caculate the checksum
 �������  : unsigned short *addr  
             int len               
 �������  : ��
 �� �� ֵ  : unsigned
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��5��18��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

*****************************************************************************/
unsigned short cal_chksum(unsigned short *addr,int len)
{
	int nleft =len;
	int sum = 0;
	unsigned short *w = addr;
	unsigned short cksum = 0;

	while(nleft > 1){
		sum += *w++;
		nleft -= 2;
	}
	if(nleft == 1){
		sum += *(unsigned char*)w;
	}
	sum = (sum >> 16) + (sum & 0xffff);
	sum += sum >> 16;
	cksum = ~sum;
	return cksum;
}

