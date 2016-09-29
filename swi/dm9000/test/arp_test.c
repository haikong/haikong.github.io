#include <arp.h>
#include <test.h>
#include <string.h>
#include <dm9000.h>
#include <stdio.h>

//ARPЭ���ʽ������
static unsigned char arpsendbuf[42]={
 
       0xff,0xff,0xff,0xff,0xff,0xff,                     	//��̫��Ŀ���ַ��ȫ1��ʾΪ�㲥��ַ
       0x1a,0x2b,0x3c,0x4d,0x5e,0x6f,        				//��̫��Դ��ַ
       0x08,0x06,                                        	//֡���ͣ�ARP֡
      
       0x00,0x01,                                       	 //Ӳ�����ͣ���̫��
       0x08,0x00,                                       	 //Э�����ͣ�IPЭ��
       0x06,                                                //Ӳ����ַ���ȣ�6�ֽ�
       0x04,                                                //Э���ַ���ȣ�4�ֽ�
       0x00,0x01,                                        	//�����룺ARP����
      
       0xaa,0xbb,0xcc,0xdd,0xee,0xff,        			//���Ͷ���̫��Ӳ����ַ
       192, 168, 1, 6,                                 //���Ͷ�IPЭ���ַ
       0x00,0x00,0x00,0x00,0x00,0x00,        			//���ն���̫��Ӳ����ַ
       192, 168, 1, 128                                 //���ն�IPЭ���ַ
};


/*****************************************************************************
 �� �� ��  : arp_test
 ��������  : arp function test
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��5��11��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

*****************************************************************************/
void arp_test(void)
{
#ifdef FILL_ARP
	unsigned char mac_addr[6] = {0xaa,0xbb,0xcc,0xdd,0,0};
	unsigned char ip_addr[4] = {192,168,1,6};
	unsigned char host_ip_addr[4] = {192,168,1,128};
	unsigned char host_mac_addr[6] = {0xff,0xff,0xff,0xff,0xff,0xff};
	unsigned char buffer[1000] = {0};
	unsigned short packet_len;
	t_ARP* ARPBUF = (t_ARP*)buffer;
	//ethernet mac header
	memcpy(ARPBUF->ethhdr.d_mac,host_mac_addr,6);
	memcpy(ARPBUF->ethhdr.s_mac,mac_addr,6);
	ARPBUF->ethhdr.type = htons(0x0806);
	//arp header
	ARPBUF->hwtype = htons(1);
	ARPBUF->protocol = htons(0x0800);
	ARPBUF->hwlen = 6;
	ARPBUF->protolen = 4;
	ARPBUF->opcode = htons(1);
	memcpy(ARPBUF->smac,mac_addr,6);
	memcpy(ARPBUF->sipaddr,ip_addr,4);
	memcpy(ARPBUF->dipaddr,host_ip_addr,4);
	packet_len = 14 + 28 + 18;
	//send arp request packet
	printf("arp_test:DM9000_sendPacket 46\n\r");	
	DM9000_sendPacket((char*)buffer,packet_len);
	printf("arp test\n\r");
#else
	DM9000_sendPacket(NULL,(char*)arpsendbuf,42);
	printf("arp_test:DM9000_sendPacket 42\n\r");	
#endif
}


/*****************************************************************************
 �� �� ��  : arp_process
 ��������  : arp recieve packet function
 �������  : void  
 �������  : ��
 �� �� ֵ  : int
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��5��11��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

*****************************************************************************/
int arp_process(char* BUFF,unsigned int len)
{
	
	t_ARP* ARPBUF = (t_ARP*)BUFF;
	int i,ret;
	//is not arp packet
	if(len < 28)
		return -1;
	
	switch(htons(ARPBUF->protocol)){
	//arp request
	case 0:
		printf("arp request\n\r");
		ret = 0;
		break;
	case 1:
		printf("arp acknowledge,dest mac:");
		for(i = 0 ; i < 6;i++)
			printf("%x ",ARPBUF->smac[i]);
		printf("\n\r");
		ret = 0;
		break;
	default:
		ret = -1;
		break;
	}
	return ret;
}

