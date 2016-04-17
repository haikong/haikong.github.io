#include <arp.h>

void MAC_pack(unsigned char *datas,unsigned short length,unsigned short type)
{
	t_ETH_HDR *MACBUF;
	unsigned char packhead[14];
	MACBUF=(t_ETH_HDR *)&packhead;
	memcpy(MACBUF->d_mac, host_mac_addr, 6);
	memcpy(MACBUF->s_mac, mac_addr, 6);
	MACBUF->type = H( type );

	backwards(datas,length,14); //���ݰ��� Buffer�к��� 8 �ֽ�
	memcpy(datas,packhead,14); //�� 14 �ֽ� MAC ��ͷ���Ƶ� Buffer ǰ��

	packet_len=length+14;
}

//����һ�������ĳ���Ϊ���ݰ����� IP ���� ARP ��ͷ

//by hongfangyu 2016/1/9

#include"dm9000.h"

//���������� IP ��ͷ��У���
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


unsigned short IP_pack(unsigned char *datas,unsigned short length,unsigned char type)
{
	t_IP *IPBUF;
	unsigned char packhead[20];
	IPBUF=(IP *)&packhead;
	IPBUF->vhl=0x45;
	IPBUF->tos=0;
	IPBUF->len=H(length+20);
	IPBUF->ipid=H(0x0);
	IPBUF->ipoffset=0;
	IPBUF->ttl=0x80;
	IPBUF->proto=type;
	IPBUF->ipchksum=0x0; //����ͳ�ʼ��Ϊ 0
	memcpy(IPBUF->srcipaddr, ip_addr, 4);
	memcpy(IPBUF->destipaddr, host_ip_addr, 4);

	IPBUF->ipchksum=H(ipchecksum(packhead));

	backwards(datas,length,20); //���ݰ��� Buffer�к��� 20 �ֽ�
	memcpy(datas,packhead,20); //�� 20 �ֽ� IP ��ͷ���Ƶ� Buffer ǰ��

	packet_len=length+20;
	return 0x0800; //��������Э�����ͷ��ظ���һ��ʹ��

}

unsigned short ARP_pack(unsigned char *datas,unsigned short length,unsigned char type)
{
	t_ARP *ARPBUF;
	unsigned  packhead[28];
	ARPBUF=(t_ARP *)&packhead;
	ARPBUF->hwtype = H( 1 ); //Ӳ������(1 ��ʾ���������̫�� MAC ��ַ)
	ARPBUF->protocol = H( 0x0800 ); //Э������(0x0800 ��ʾ������� IP ��ַ)
	ARPBUF->hwlen = 6;
	ARPBUF->protolen = 4;
	ARPBUF->opcode = H( type );
	memcpy(ARPBUF->smac, mac_addr, 6);
	memcpy(ARPBUF->sipaddr, ip_addr, 4);
	memcpy(ARPBUF->dipaddr, host_ip_addr, 4);
	backwards(datas,length,28); //���ݰ��� Buffer�к��� 28 �ֽ�
	memcpy(datas,packhead,28); //�� 28 �ֽ� IP ��ͷ���Ƶ� Buffer ǰ��

	packet_len=length+28;

	return 0x0806; //��������Э�����ͷ��ظ���һ��ʹ��

}

//����һ�������ĳ���
//��Ҫ����˽������ݲ�����������ݣ�Ȼ��� TCP �� UDP ��ͷ�ķ�װ�ͷ���
//���������� UDP �����
 unsigned short udpchecksum(unsigned char *head,unsigned char *datah, unsigned short lengthh)
 {
	unsigned int summ=0,suml=0,sumh=0;
	unsigned short pi=0;

	//UDP α�ײ����ײ�У���
	//���α�ײ�ָ��Դ��ַ��Ŀ�ĵ�ַ��UDP ���ݳ��ȡ�Э�����ͣ�0x11����Э�����;�һ���ֽڣ�����Ҫ��һ���ֽڵ� 0x0������ 12 ���ֽڡ�
	suml=ip_addr[1]+ip_addr[3]+host_ip_addr[1]+host_ip_addr[3]+head[5]+0x11+head[1]+head[3]+head[5]+head[7];
	sumh=ip_addr[0]+ip_addr[2]+host_ip_addr[0]+host_ip_addr[2]+head[4]+ 0+head[0]+head[2]+head[4]+head[6];

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

unsigned char UDP_pack(unsigned char *datas,unsigned short length)
{
	t_UDP *UDPBUF;
	unsigned short packhead[8];
	UDPBUF =(t_UDP*)&packhead; //�����ò���ȡַ���Ŷ��ǶԵģ��������Ӧ�ò�Ҫȡַ����
	UDPBUF->sport=H(1024); //Դ�˿ں����� 1024=0x0400,ֱ��д 1024 �Ļ���С��ģʽ�е�һ���ֽ�Ϊ 0���ڶ����ֽ�Ϊ 4��
	//���ǽ��շ�����Ϊ���յ���Ϊ��λ��������� 0x0004,����Ӧ���Ȼ����ߵ�λд��ȥ

	UDPBUF->dport=H(2844); //Ŀ�Ķ˿ں�����
	UDPBUF->length=H(length+8); //UDP ������=���ݲ��ֳ���+UDP ��ͷ����
	UDPBUF->udpchksum=0; //UDP У��ͳ�ʼ��Ϊ


	UDPBUF->udpchksum=H(udpchecksum(packhead,datas,length)); //UDP �����

	backwards(datas,length,8); //���ݰ��� Buffer�к��� 8 �ֽ�
	memcpy(datas,packhead,8); //�� 8 �ֽ� UDP ��ͷ���Ƶ� Buffer ǰ��

	packet_len=length+8;

	return 0x11; //���ش�����Э��Ϊ UDP���Թ���һ��ʹ��
 }


