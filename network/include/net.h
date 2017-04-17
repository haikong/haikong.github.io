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

//以太网头部结构
typedef struct _eth_hdr {
	unsigned char 	d_mac[6]; 	//目的地址
	unsigned char 	s_mac[6]; 	//源地址
	unsigned short 	type; 		//协议类型
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

#define PROT_IP		0x0800		/* IP protocol			*/
#define PROT_ARP	0x0806		/* IP ARP protocol		*/
#define PROT_RARP	0x8035		/* IP ARP protocol		*/
#define PROT_VLAN	0x8100		/* IEEE 802.1q protocol		*/

#define IPPROTO_ICMP	 1		/* Internet Control Message Protocol	*/
#define IPPROTO_UDP		17		/* User Datagram Protocol		*/

//IP 首部结构
typedef struct _ip {
	 unsigned char  vhl; //4 			//4位版本号 4 位首部长度(0x45)
	 unsigned char  tos; 			//服务类型(0)
	 unsigned short len; 			//整个 IP 数据报总字节长度
	 unsigned short ipid; 			//IP 标识
	 unsigned short ipoffset; //3 		//3位标识 13 位偏移
	 unsigned char  ttl; 			//生存时间(32 或 64)
	 unsigned char  proto; 			//协议(1 表示 ICMP,2 表示 IGMP,6 表示 TCP,17 表示UDP)
	 unsigned short ipchksum; 		//首部校验和
	 unsigned char  srcipaddr[4]; 	//源 IP
	 unsigned short destipaddr[4]; 	//目的 IP
}t_IP;
//TCP 首部结构
typedef struct _tcp {
	 unsigned short	sport; 			//源端口号
	 unsigned short dport; 			//目的端口号
	 unsigned int 	sequencenum; 	//顺序号
	 unsigned int 	acknowledgenum; //确认号
	 unsigned short tcplength; 		//低四位为 TCP 报头字（32 位）的个数，高四位必须为 0，是保留位
	 unsigned short flags; 			//低两位为保留位，高 6 位为标志位[2:7]依次为
									//URG:紧急指针。用到的时候值为 1，用来处理避免TCP 数据流中断
									//ACK:置 1 时表示确认号为合法，为 0 的时候表示数据段不包含确认信息，确认号被忽略。
									//PSH:置 1 时请求的数据段在接收方得到后就可直接送到应用程序，而不必等到缓冲区满时才传送。
									//RST:用于复位因某种原因引起出现的错误连接，也用来拒绝非法数据和请求。
									//SYN:在连接请求中，SYN=1，ACK=0，连接响应时，SYN=1，ACK=1。
									//FIN:用来释放连接，表明发送方已经没有数据发送了。
	 unsigned short window; 		//指定关于发送端能传输的下一段的大小的指令，表示想收到的每个 TCP 数据段的大小。
	 unsigned short tcpchksum; 		//TCP 校验和
	 unsigned short urgentpoint; 	//紧急指针 16 位，紧急指针指出在本报文段中的紧急数据的位置，在 URG 标志设置了时才有效。
}t_TCP;
 
//UDP 首部结构
typedef struct _udp{
	 unsigned short sport; 		//源端口号 （34 35）
	 unsigned short dport; 		//目的端口号 （36 37）
	 unsigned short length; 	//UDP 数据包报总长度 （38 39）
	 unsigned short udpchksum; 	//UDP 校验和(可选项) （40 41）
}t_UDP;

#define IP_OFFS		0x1fff 		/* ip offset *= 8 */
#define IP_FLAGS	0xe000 		/* first 3 bits */
#define IP_FLAGS_RES	0x8000 	/* reserved */
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

//ARP 首部结构
typedef struct _arp{
	 struct _eth_hdr ethhdr;	//以太网头部
	 unsigned short hwtype; 	//硬件类型(1 表示传输的是以太网 MAC 地址)
	 unsigned short protocol; 	//协议类型(0x0800 表示传输的是 IP 地址)
	 unsigned char  hwlen; 		//硬件地址长度(6)
	 unsigned char  protolen; 	//协议地址长度(4)
	 unsigned short opcode; 	//操作(1 表示 ARP 请求,2 表示 ARP 应答)
	 unsigned char  smac[6]; 	//发送端 MAC 地址
	 unsigned char  sipaddr[4]; //发送端 IP 地址
	 unsigned char  dmac[6]; 	//目的端 MAC 地址
	 unsigned char  dipaddr[4]; //目的端 IP 地址
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

#define VLAN_NONE		4095			/* untagged */
#define VLAN_IDMASK		0x0fff			/* mask of valid vlan id */
#define DEBUG_LL_STATE 	0				/* Link local state machine changes */
#define DEBUG_DEV_PKT 	0				/* Packets or info directed to the device */
#define DEBUG_NET_PKT 	0				/* Packets on info on the network at large */
#define DEBUG_INT_STATE 0				/* Internal network state changes */

/*host to network endian*/
#define htonw(x) ((unsigned short)(\
	(((unsigned short)(x) & (unsigned short)0x00ffU) << 8) |\
	(((unsigned short)(x) & (unsigned short)0xff00U) >> 8)))
	
#define htonl(x) ((unsigned int)(				\
	(((unsigned int)(x) & (unsigned int)0x000000ffUL) << 24) |		\
	(((unsigned int)(x) & (unsigned int)0x0000ff00UL) <<  8) |		\
	(((unsigned int)(x) & (unsigned int)0x00ff0000UL) >>  8) |		\
	(((unsigned int)(x) & (unsigned int)0xff000000UL) >> 24)))

#define ntohl(x) ((unsigned long)___ntohl(x))
#define ntohw(x) ___ntohs(x)

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

