#ifndef _ARP_H__
#define _ARP_H__

//��̫��ͷ���ṹ
typedef struct _eth_hdr {
	unsigned char 	d_mac[6]; 	//Ŀ�ĵ�ַ
	unsigned char 	s_mac[6]; 	//Դ��ַ
	unsigned short 	type; 		//Э������
}t_ETH_HDR;

//ARP �ײ��ṹ
typedef struct _arp{
	 unsigned short hwtype; 	//Ӳ������(1 ��ʾ���������̫�� MAC ��ַ)
	 unsigned short protocol; 	//Э������(0x0800 ��ʾ������� IP ��ַ)
	 unsigned char  hwlen; 		//Ӳ����ַ����(6)
	 unsigned char  protolen; 	//Э���ַ����(4)
	 unsigned short opcode; 	//����(1 ��ʾ ARP ����,2 ��ʾ ARP Ӧ��)
	 unsigned char  smac[6]; 	//���Ͷ� MAC ��ַ
	 unsigned char  sipaddr[4]; //���Ͷ� IP ��ַ
	 unsigned char  dmac[6]; 	//Ŀ�Ķ� MAC ��ַ
	 unsigned char  dipaddr[4]; //Ŀ�Ķ� IP ��ַ
}t_ARP;

//IP �ײ��ṹ
typedef struct _ip {
	 unsigned char  vhl; //4 			//4λ�汾�� 4 λ�ײ�����(0x45)
	 unsigned char  tos; 			//��������(0)
	 unsigned short len; 			//���� IP ���ݱ����ֽڳ���
	 unsigned short ipid; 			//IP ��ʶ
	 unsigned short ipoffset; //3 		//3λ��ʶ 13 λƫ��
	 unsigned char  ttl; 			//����ʱ��(32 �� 64)
	 unsigned char  proto; 			//Э��(1 ��ʾ ICMP,2 ��ʾ IGMP,6 ��ʾ TCP,17 ��ʾUDP)
	 unsigned short ipchksum; 		//�ײ�У���
	 unsigned char  srcipaddr[4]; 	//Դ IP
	 unsigned short destipaddr[4]; 	//Ŀ�� IP
}t_IP;
//TCP �ײ��ṹ
typedef struct _tcp {
	 unsigned short	sport; 			//Դ�˿ں�
	 unsigned short dport; 			//Ŀ�Ķ˿ں�
	 unsigned int 	sequencenum; 	//˳���
	 unsigned int 	acknowledgenum; //ȷ�Ϻ�
	 unsigned short tcplength; 		//����λΪ TCP ��ͷ�֣�32 λ���ĸ���������λ����Ϊ 0���Ǳ���λ
	 unsigned short flags; 			//����λΪ����λ���� 6 λΪ��־λ[2:7]����Ϊ
									//URG:����ָ�롣�õ���ʱ��ֵΪ 1��������������TCP �������ж�
									//ACK:�� 1 ʱ��ʾȷ�Ϻ�Ϊ�Ϸ���Ϊ 0 ��ʱ���ʾ���ݶβ�����ȷ����Ϣ��ȷ�Ϻű����ԡ�
									//PSH:�� 1 ʱ��������ݶ��ڽ��շ��õ���Ϳ�ֱ���͵�Ӧ�ó��򣬶����صȵ���������ʱ�Ŵ��͡�
									//RST:���ڸ�λ��ĳ��ԭ��������ֵĴ������ӣ�Ҳ�����ܾ��Ƿ����ݺ�����
									//SYN:�����������У�SYN=1��ACK=0��������Ӧʱ��SYN=1��ACK=1��
									//FIN:�����ͷ����ӣ��������ͷ��Ѿ�û�����ݷ����ˡ�
	 unsigned short window; 		//ָ�����ڷ��Ͷ��ܴ������һ�εĴ�С��ָ���ʾ���յ���ÿ�� TCP ���ݶεĴ�С��
	 unsigned short tcpchksum; 		//TCP У���
	 unsigned short urgentpoint; 	//����ָ�� 16 λ������ָ��ָ���ڱ����Ķ��еĽ������ݵ�λ�ã��� URG ��־������ʱ����Ч��
}t_TCP;
 
//UDP �ײ��ṹ
typedef struct _udp{
	 unsigned short sport; 		//Դ�˿ں� ��34 35��
	 unsigned short dport; 		//Ŀ�Ķ˿ں� ��36 37��
	 unsigned short length; 	//UDP ���ݰ����ܳ��� ��38 39��
	 unsigned short udpchksum; 	//UDP У���(��ѡ��) ��40 41��
}t_UDP;

#define htons(x) ({\
	typeof(x) _x = x;	\
	_x = (((unsigned short)x >> 8) & 0xff) | ((x & 0xff) << 8);\
	x = _x;\
})

#define htonl(x) ({\
	typeof(x) _x = x;	\
	_x = ((unsigned int)x & 0xff) << 24;\
	_x |= (((unsigned int)x >> 8) & 0xff) << 16;\
	_x |= (((unsigned int)x >> 16) & 0xff) << 8;\
	_x |= ((unsigned int)x >> 24) & 0xff;\
	x = _x;\
})
#endif