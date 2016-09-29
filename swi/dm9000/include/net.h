#ifndef __NET_H__
#define __NET_H__
#include <types.h>
/**
 * enum sock_type - Socket types
 * @SOCK_STREAM: stream (connection) socket
 * @SOCK_DGRAM: datagram (conn.less) socket
 * @SOCK_RAW: raw socket
 * @SOCK_RDM: reliably-delivered message
 * @SOCK_SEQPACKET: sequential packet socket
 * @SOCK_DCCP: Datagram Congestion Control Protocol socket
 * @SOCK_PACKET: linux specific way of getting packets at the dev level.
 *		  For writing rarp and other similar things on the user level.
 *
 * When adding some new socket type please
 * grep ARCH_HAS_SOCKET_TYPE include/asm-* /socket.h, at least MIPS
 * overrides this enum for binary compat reasons.
 */
/*sock_type*/
typedef enum sock_type {
	SOCK_STREAM	= 1,
	SOCK_DGRAM	= 2,
	SOCK_RAW	= 3,
	SOCK_RDM	= 4,
	SOCK_SEQPACKET	= 5,
	SOCK_DCCP	= 6,
	SOCK_PACKET	= 10,
}E_NET_PROTO;

//ethnetwork
enum eth_state_t {
	ETH_STATE_INIT,
	ETH_STATE_PASSIVE,
	ETH_STATE_ACTIVE
};

//��̫��ͷ���ṹ
typedef struct _eth_hdr {
	unsigned char 	d_mac[6]; 	//Ŀ�ĵ�ַ
	unsigned char 	s_mac[6]; 	//Դ��ַ
	unsigned short 	type; 		//Э������
}t_ETH_HDR;

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
									//URG:����ָ�롣�õ���ʱ��ֵΪ 1�������������TCP �������ж�
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

//ARP �ײ��ṹ
typedef struct _arp{
	 struct _eth_hdr ethhdr;	//��̫��ͷ��
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

//ICMP header
typedef struct icmphdr {
  unsigned char		type;
  unsigned char		code;
  unsigned short	checksum;
  union {
	struct {
		unsigned short	id;
		unsigned short	sequence;
	} echo;
	unsigned int	gateway;
	struct {
		unsigned short	__unused;
		unsigned short	mtu;
	} frag;
  } un;
}t_ICMP_HDR;

//MAC protocole
typedef enum _mac_proto{
	ETHTYPE_ARP = 0,
	ETHTYPE_IP,	
}e_MAC_PROTO;

/*ethwork structure*/
struct eth_device {
	char name[16];
	unsigned char enetaddr[6];
	int iobase;
	int state;

	int  (*init) (struct eth_device *);
	int  (*send) (struct eth_device *, void *packet,unsigned int length);
	int  (*recv) (struct eth_device *netdev, unsigned char* data_src );
	void (*halt) (struct eth_device *);
#ifdef CONFIG_MCAST_TFTP
	int (*mcast) (struct eth_device *, UINT32 ip, UINT8 set);
#endif
	int  (*write_hwaddr) (struct eth_device *);
	struct eth_device *next;
	int index;
	void *priv;
};
/* Structure/enum declaration ------------------------------- */
typedef struct board_info {
	UINT32 	runt_length_counter;	/* counter: RX length < 64byte */
	UINT32 	long_length_counter;	/* counter: RX length > 1514byte */
	UINT32 	reset_counter;			/* counter: RESET */
	UINT32 	reset_tx_timeout;		/* RESET caused by TX Timeout */
	UINT32 	reset_rx_status;		/* RESET caused by RX Statsus wrong */
	UINT16 	tx_pkt_cnt;
	UINT16 	queue_start_addr;
	UINT16 	dbug_cnt;
	UINT8 	phy_addr;
	UINT8 	device_wait_reset;	/* device state */
	unsigned char srom[128];
	void (*outblk)(volatile void *data_ptr, int count);
	void (*inblk)(void *data_ptr, int count);
	void (*rx_status)(UINT16 *RxStatus, UINT16 *RxLen);
	struct eth_device netdev;
}t_board_info,*pt_board_info;

/*host to network endian*/
#define htons(x) ((unsigned short)(\
	(((unsigned short)(x) & (unsigned short)0x00ffU) << 8) |\
	(((unsigned short)(x) & (unsigned short)0xff00U) >> 8)))
	
#define htonl(x) ((unsigned int)(				\
	(((unsigned int)(x) & (unsigned int)0x000000ffUL) << 24) |		\
	(((unsigned int)(x) & (unsigned int)0x0000ff00UL) <<  8) |		\
	(((unsigned int)(x) & (unsigned int)0x00ff0000UL) >>  8) |		\
	(((unsigned int)(x) & (unsigned int)0xff000000UL) >> 24)))

//calculate the check sum
unsigned short cal_chksum(unsigned short *addr,int len);
int eth_register(struct eth_device *dev);
int eth_unregister(struct eth_device *dev);
int  eth_init (struct eth_device * edev);
int  eth_send(struct eth_device *edev, void *packet,unsigned int length);
int  eth_recv(struct eth_device *netdev, unsigned char* data_src );
void eth_halt(struct eth_device *edev);

#endif

