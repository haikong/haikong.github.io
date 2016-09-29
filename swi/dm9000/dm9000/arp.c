#include <arp.h>
#include <dm9000.h>
#include <string.h>
 
/*****************************************************************************
 �� �� ��  : mac_send_pack
 ��������  : MAC PACKETS send
 �������  : unsigned char *datas  
             unsigned int length   
             unsigned short proto  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��5��9��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

*****************************************************************************/
void mac_send_pack(unsigned char *datas,unsigned int length,e_MAC_PROTO proto)
{
	t_ETH_HDR *MACBUF;
	unsigned char packhead[14];
	unsigned char mac_addr[6] = {0xaa,0xbb,0xcc,0xdd,0xee,0xff};
	unsigned char host_mac_addr[6] = {0x1,0x2,0x3,0x4,0x5,0x6};
	unsigned char mac_packet[length + 14];
	
	MACBUF=(t_ETH_HDR *)&packhead;
	memcpy(MACBUF->d_mac, host_mac_addr, 6);
	memcpy(MACBUF->s_mac, mac_addr, 6);
	MACBUF->type = htons(proto);
	memcpy(mac_packet + 14,datas,length); 
	memcpy(mac_packet,packhead,14); //�� 14 �ֽ� MAC ��ͷ���Ƶ� Buffer ǰ��
	DM9000_sendPacket(NULL,(char*)mac_packet,length+14);
}

/*****************************************************************************
 �� �� ��  : ipchecksum
 ��������  : caculate the ip packet checksum
 �������  : unsigned char *dp  
 �������  : ��
 �� �� ֵ  : unsigned
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��5��11��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

*****************************************************************************/
unsigned short ipchecksum(unsigned char *dp)
{
	 unsigned int sum=0;
	 unsigned char di=0;
	 for(di=0;di!=20;di=di+2) sum+=dp[di];
	 sum=sum<<8;
	 for(di=1;di!=21;di=di+2) sum+=dp[di];
	 sum=(sum&0xffff)+(sum>>16);
	 return ~((unsigned short)sum);
}


/*****************************************************************************
 �� �� ��  : IP_pack
 ��������  : IP packets 
 �������  : unsigned char *datas   
             unsigned short length  
             unsigned char type     
 �������  : ��
 �� �� ֵ  : unsigned
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��5��10��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

*****************************************************************************/
unsigned short IP_pack(unsigned char *datas,unsigned short length,unsigned char type)
{
	t_IP *IPBUF;
	unsigned char ip_addr[4] = {192,168,1,6};
	unsigned char host_ip_addr[4] = {192,168,1,128};
	unsigned char packhead[20];
	unsigned char ip_packet[length + 20];
	IPBUF=(t_IP *)&packhead;
	IPBUF->vhl=0x45;
	IPBUF->tos=0;
	IPBUF->len=htons(length+20);
	IPBUF->ipid=htons(0x0);
	IPBUF->ipoffset=0;
	IPBUF->ttl=0x80;
	IPBUF->proto=type;
	IPBUF->ipchksum=0x0; //����ͳ�ʼ��Ϊ 0
	memcpy(IPBUF->srcipaddr, ip_addr, 4);
	memcpy(IPBUF->destipaddr, host_ip_addr, 4);

	IPBUF->ipchksum=htons(ipchecksum(packhead));

	memcpy(ip_packet + 20,datas,length); 
	memcpy(ip_packet,packhead,20); 

	mac_send_pack(ip_packet,length + 20,ETHTYPE_IP);
	return 0x0800; //��������Э�����ͷ��ظ���һ��ʹ��

}

/*****************************************************************************
 �� �� ��  : ARP_pack
 ��������  : arp packet 
 �������  : unsigned char *datas   
             unsigned short length  
             e_MAC_PROTO proto      
 �������  : ��
 �� �� ֵ  : unsigned
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��5��11��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

*****************************************************************************/
unsigned short ARP_pack(unsigned char *datas,unsigned short length,e_MAC_PROTO proto)
{
	t_ARP *ARPBUF;
	unsigned  packhead[28];	
	unsigned char arp_packet[length + 28];
	unsigned char ip_addr[4] = {192,168,1,6};
	unsigned char host_ip_addr[4] = {192,168,1,128};	
	unsigned char mac_addr[6] = {0xaa,0xbb,0xcc,0xdd,0xee,0xff};
	ARPBUF=(t_ARP *)&packhead;
	ARPBUF->hwtype = htons( 1 ); //Ӳ������(1 ��ʾ���������̫�� MAC ��ַ)
	ARPBUF->protocol = htons( 0x0800 ); //Э������(0x0800 ��ʾ������� IP ��ַ)
	ARPBUF->hwlen = 6;
	ARPBUF->protolen = 4;
	ARPBUF->opcode = htons( proto);
	memcpy(ARPBUF->smac, mac_addr, 6);
	memcpy(ARPBUF->sipaddr, ip_addr, 4);
	memcpy(ARPBUF->dipaddr, host_ip_addr, 4);
	memcpy(arp_packet + 28,datas,length); //���ݰ��� Buffer�к��� 28 �ֽ�
	memcpy(arp_packet,packhead,28); //�� 28 �ֽ� IP ��ͷ���Ƶ� Buffer ǰ��
	mac_send_pack(arp_packet,length + 28,ETHTYPE_ARP);
	return 0x0806; //��������Э�����ͷ��ظ���һ��ʹ��

}

/*****************************************************************************
 �� �� ��  : udpchecksum
 ��������  : ��Ҫ����˽������ݲ�����������ݣ�Ȼ��� TCP �� UDP ��ͷ�ķ�װ�ͷ���
			 ���������� UDP �����
 �������  : unsigned char *head     
             unsigned char *datah    
             unsigned short lengthh  
 �������  : ��
 �� �� ֵ  : unsigned
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��5��11��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

*****************************************************************************/
 unsigned short udpchecksum(unsigned char *head,unsigned char *datah, unsigned short lengthh)
 {
	unsigned int summ=0,suml=0,sumh=0;
	unsigned short pi=0;

	//UDP α�ײ����ײ�У���
	//���α�ײ�ָ��Դ��ַ��Ŀ�ĵ�ַ��UDP ���ݳ��ȡ�Э�����ͣ�0x11����Э�����;�һ���ֽڣ�����Ҫ��һ���ֽڵ� 0x0������ 12 ���ֽڡ�
	//suml=ip_addr[1]+ip_addr[3]+host_ip_addr[1]+host_ip_addr[3]+head[5]+0x11+head[1]+head[3]+head[5]+head[7];
	//sumh=ip_addr[0]+ip_addr[2]+host_ip_addr[0]+host_ip_addr[2]+head[4]+ 0+head[0]+head[2]+head[4]+head[6];

	//���ݲ���У���
	for(pi=1;pi!=lengthh+1;pi+=2)
	suml=suml+datah[pi];
	for(pi=0;pi!=lengthh;pi+=2)
	sumh=sumh+datah[pi];

	//UDP α�ײ�+UDP �ײ�+���� һ�����У��ͣ�У���Ϊ�� 16 λ���߳� 16 λ��������16λ��ӵ�ԭ���� 16 λ��
	summ=(sumh<<8)+suml;
	summ=(summ&0xffff)+(summ>>16);

	//ȡ�����̣�������
	return (~(unsigned short)summ);

}

	
/*****************************************************************************
 �� �� ��  : UDP_pack
 ��������  : udp packet 
 �������  : unsigned char *datas   
             unsigned short length  
 �������  : ��
 �� �� ֵ  : unsigned
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��5��11��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

*****************************************************************************/
unsigned char UDP_pack(unsigned char *datas,unsigned short length)
{
	t_UDP *UDPBUF;
	unsigned short packhead[8];
	UDPBUF =(t_UDP*)&packhead; //�����ò���ȡַ���Ŷ��ǶԵģ��������Ӧ�ò�Ҫȡַ����
	UDPBUF->sport=htons(1024); //Դ�˿ں����� 1024=0x0400,ֱ��д 1024 �Ļ���С��ģʽ�е�һ���ֽ�Ϊ 0���ڶ����ֽ�Ϊ 4��
	//���ǽ��շ�����Ϊ���յ���Ϊ��λ��������� 0x0004,����Ӧ���Ȼ����ߵ�λд��ȥ

	UDPBUF->dport=htons(2844); //Ŀ�Ķ˿ں�����
	UDPBUF->length=htons(length+8); //UDP ������=���ݲ��ֳ���+UDP ��ͷ����
	UDPBUF->udpchksum=0; //UDP У��ͳ�ʼ��Ϊ


	UDPBUF->udpchksum=htons(udpchecksum((unsigned char*)packhead,datas,length)); //UDP �����
	memcpy(datas,packhead,8); //�� 8 �ֽ� UDP ��ͷ���Ƶ� Buffer ǰ��

	return 0x11; //���ش�����Э��Ϊ UDP���Թ���һ��ʹ��
}


