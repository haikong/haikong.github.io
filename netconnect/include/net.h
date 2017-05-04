#ifndef __NET_H__
#define __NET_H__
#include <types.h>
#include <glob.h>
/* IPv4 addresses are always 32 bits in size */
typedef UINT32		IPaddr_t;
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
/* Ethernet header size */
#define ETHER_HDR_SIZE	(sizeof(t_ETH_HDR))

struct e802_hdr {
	uchar		et_dest[6];	/* Destination node		*/
	uchar		et_src[6];	/* Source node			*/
	ushort		et_protlen;	/* Protocol or length		*/
	uchar		et_dsap;	/* 802 DSAP			*/
	uchar		et_ssap;	/* 802 SSAP			*/
	uchar		et_ctl;		/* 802 control			*/
	uchar		et_snap1;	/* SNAP				*/
	uchar		et_snap2;
	uchar		et_snap3;
	ushort		et_prot;	/* 802 protocol			*/
};

/* 802 + SNAP + ethernet header size */
#define E802_HDR_SIZE	(sizeof(struct e802_hdr))

/*
 *	Virtual LAN Ethernet header
 */
struct vlan_ethernet_hdr {
	uchar		vet_dest[6];		/* Destination node		*/
	uchar		vet_src[6];			/* Source node			*/
	ushort		vet_vlan_type;		/* PROT_VLAN			*/
	ushort		vet_tag;			/* TAG of VLAN			*/
	ushort		vet_type;			/* protocol type		*/
};

/* VLAN Ethernet header size */
#define VLAN_ETHER_HDR_SIZE	(sizeof(struct vlan_ethernet_hdr))

#define PROT_IP	0x0800		/* IP protocol			*/
#define PROT_ARP	0x0806		/* IP ARP protocol		*/
#define PROT_RARP	0x8035		/* IP ARP protocol		*/
#define PROT_VLAN	0x8100		/* IEEE 802.1q protocol		*/

#define IPPROTO_ICMP	 1		/* Internet Control Message Protocol	*/
#define IPPROTO_UDP	17		/* User Datagram Protocol		*/

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

#define IP_OFFS			0x1fff 	/* ip offset *= 8 */
#define IP_FLAGS			0xe000 	/* first 3 bits */
#define IP_FLAGS_RES		0x8000 	/* reserved */
#define IP_FLAGS_DFRAG	0x4000 	/* don't fragments */
#define IP_FLAGS_MFRAG	0x2000 	/* more fragments */

#define IP_HDR_SIZE		(sizeof( t_TCP))

/*
 *	Internet Protocol (IP) + UDP header.
 */
struct ip_udp_hdr {
	uchar		ip_hl_v;	/* header length and version	*/
	uchar		ip_tos;		/* type of service		*/
	ushort		ip_len;		/* total length			*/
	ushort		ip_id;		/* identification		*/
	ushort		ip_off;		/* fragment offset field	*/
	uchar		ip_ttl;		/* time to live			*/
	uchar		ip_p;		/* protocol			*/
	ushort		ip_sum;		/* checksum			*/
	IPaddr_t	ip_src;		/* Source IP address		*/
	IPaddr_t	ip_dst;		/* Destination IP address	*/
	ushort		udp_src;	/* UDP source port		*/
	ushort		udp_dst;	/* UDP destination port		*/
	ushort		udp_len;	/* Length of UDP packet		*/
	ushort		udp_xsum;	/* Checksum			*/
};

#define IP_UDP_HDR_SIZE		(sizeof(struct ip_udp_hdr))
#define UDP_HDR_SIZE		(IP_UDP_HDR_SIZE - IP_HDR_SIZE)

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
	unsigned char enetaddr[6];	  //mac address
	unsigned short flag;		  //work bit mode 
	int iobase;					  //devices io base address
	int int_base;				  //devices' interrupt vector number
	int int_state;				  //devices' interrupt work mode
	int state;					  //devices' work mode
	
	int  (*init) (void);
	int  (*send) (void *packet,unsigned int length);
	int  (*recv) (unsigned char* data_src );
	void (*halt) (void);
#ifdef CONFIG_MCAST_TFTP
	int (*mcast) (UINT32 ip, UINT8 set);
#endif
	int  (*write_hwaddr) (void);
	struct eth_device *next;
	int index;
	void *priv;
};
/* Structure/enum declaration ------------------------------- */
typedef struct board_info {
	UINT32 	runt_length_counter;	/* counter: RX length < 64byte */
	UINT32 	long_length_counter;	/* counter: RX length > 1514byte */
	UINT32 	reset_counter;		/* counter: RESET */
	UINT32 	reset_tx_timeout;		/* RESET caused by TX Timeout */
	UINT32 	reset_rx_status;		/* RESET caused by RX Statsus wrong */
	UINT16 	tx_pkt_cnt;			/* tx packets counter*/
	UINT16 	queue_start_addr;		
	UINT16 	dbug_cnt;
	UINT8 	phy_addr;
	UINT8 	device_wait_reset;	/* device state */
	unsigned char srom[128];
	void (*dumpblk)(int count);
	void (*outblk)(volatile void *data_ptr, int count);
	void (*inblk)(void *data_ptr, int count);
	void (*rx_status)(UINT16 *RxStatus, UINT16 *RxLen);
	struct eth_device netdev;
}t_board_info,*pt_board_info;

#define VLAN_NONE			4095				/* untagged */
#define VLAN_IDMASK		0x0fff			/* mask of valid vlan id */
#define DEBUG_LL_STATE 	0				/* Link local state machine changes */
#define DEBUG_DEV_PKT 	0				/* Packets or info directed to the device */
#define DEBUG_NET_PKT 	0				/* Packets on info on the network at large */
#define DEBUG_INT_STATE 	0				/* Internal network state changes */

//calculate the check sum
unsigned short cal_chksum(unsigned short *addr,int len);
int eth_register(struct eth_device *dev);
int eth_unregister(struct eth_device *dev);
int  eth_init (void);
int  eth_send(void *packet,unsigned int length);
int  eth_recv(unsigned char* data_src );
void eth_halt(void);
void NetReceive(unsigned char *inpkt, int len);
unsigned char* eth_mac_addr(void);

#endif

