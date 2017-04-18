#include <arp.h>
#include <test.h>
#include <string.h>
#include <stdio.h>
#include <net.h>
/*reading the dm9000 isr counters*/
extern unsigned int g_dm9000_isr_count;
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
      
       0xa,0x1b,0x2c,0x3d,0x4e,0x5f,        			//���Ͷ���̫��Ӳ����ַ
       192, 168, 1, 6,                                 	//���Ͷ�IPЭ���ַ
       0x00,0x00,0x00,0x00,0x00,0x00,        			//���ն���̫��Ӳ����ַ
       192, 168, 1, 8                                 	//���ն�IPЭ���ַ
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
	unsigned int dm9000_isr_counter;
	int len;
	unsigned char buf[1000];
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
	ARPBUF->ethhdr.type = htonw(0x0806);
	//arp header
	ARPBUF->hwtype = htonw(1);
	ARPBUF->protocol = htonw(0x0800);
	ARPBUF->hwlen = 6;
	ARPBUF->protolen = 4;
	ARPBUF->opcode = htonw(1);
	memcpy(ARPBUF->smac,mac_addr,6);
	memcpy(ARPBUF->sipaddr,ip_addr,4);
	memcpy(ARPBUF->dipaddr,host_ip_addr,4);
	packet_len = 14 + 28 + 18;
	//send arp request packet
	printf("arp_test:DM9000_sendPacket 46\n\r");	
	DM9000_sendPacket((char*)buffer,packet_len);
	printf("arp test\n\r");
#else
	eth_send((char*)arpsendbuf,42);
	printf("arp_test:DM9000_sendPacket 42 bytes.\n\r");	
#endif
	len = 0;
	dm9000_isr_counter = 0;
	memset(buf,0,1000);
	len = eth_recv(buf);
	if(len >0)
	{
		arp_process((char*)buf,len);
	}
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
	
	switch(htonw(ARPBUF->protocol)){
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
		printf("other data infomation.\n\r");
		ret = -1;
		break;
	}
	return ret;
}

