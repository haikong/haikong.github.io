#include <arp.h>

void MAC_pack(unsigned char *datas,unsigned short length,unsigned short type)
{
	t_ETH_HDR *MACBUF;
	unsigned char packhead[14];
	MACBUF=(t_ETH_HDR *)&packhead;
	memcpy(MACBUF->d_mac, host_mac_addr, 6);
	memcpy(MACBUF->s_mac, mac_addr, 6);
	MACBUF->type = H( type );

	backwards(datas,length,14); //数据包在 Buffer中后移 8 字节
	memcpy(datas,packhead,14); //将 14 字节 MAC 报头复制到 Buffer 前面

	packet_len=length+14;
}

//这是一个网络层的程序，为数据包加上 IP 或者 ARP 报头

//by hongfangyu 2016/1/9

#include"dm9000.h"

//函数：计算 IP 报头的校验和
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
	IPBUF->ipchksum=0x0; //检验和初始化为 0
	memcpy(IPBUF->srcipaddr, ip_addr, 4);
	memcpy(IPBUF->destipaddr, host_ip_addr, 4);

	IPBUF->ipchksum=H(ipchecksum(packhead));

	backwards(datas,length,20); //数据包在 Buffer中后移 20 字节
	memcpy(datas,packhead,20); //将 20 字节 IP 报头复制到 Buffer 前面

	packet_len=length+20;
	return 0x0800; //将网络层的协议类型返回给下一层使用

}

unsigned short ARP_pack(unsigned char *datas,unsigned short length,unsigned char type)
{
	t_ARP *ARPBUF;
	unsigned  packhead[28];
	ARPBUF=(t_ARP *)&packhead;
	ARPBUF->hwtype = H( 1 ); //硬件类型(1 表示传输的是以太网 MAC 地址)
	ARPBUF->protocol = H( 0x0800 ); //协议类型(0x0800 表示传输的是 IP 地址)
	ARPBUF->hwlen = 6;
	ARPBUF->protolen = 4;
	ARPBUF->opcode = H( type );
	memcpy(ARPBUF->smac, mac_addr, 6);
	memcpy(ARPBUF->sipaddr, ip_addr, 4);
	memcpy(ARPBUF->dipaddr, host_ip_addr, 4);
	backwards(datas,length,28); //数据包在 Buffer中后移 28 字节
	memcpy(datas,packhead,28); //将 28 字节 IP 报头复制到 Buffer 前面

	packet_len=length+28;

	return 0x0806; //将网络层的协议类型返回给下一层使用

}

//这是一个传输层的程序
//主要完成了接受数据层和网络层的数据，然后对 TCP 和 UDP 报头的封装和分用
//函数：计算 UDP 检验和
 unsigned short udpchecksum(unsigned char *head,unsigned char *datah, unsigned short lengthh)
 {
	unsigned int summ=0,suml=0,sumh=0;
	unsigned short pi=0;

	//UDP 伪首部和首部校验和
	//这个伪首部指，源地址、目的地址、UDP 数据长度、协议类型（0x11），协议类型就一个字节，但需要补一个字节的 0x0，构成 12 个字节。
	suml=ip_addr[1]+ip_addr[3]+host_ip_addr[1]+host_ip_addr[3]+head[5]+0x11+head[1]+head[3]+head[5]+head[7];
	sumh=ip_addr[0]+ip_addr[2]+host_ip_addr[0]+host_ip_addr[2]+head[4]+ 0+head[0]+head[2]+head[4]+head[6];

	//数据部分校验和
	for(pi=1;pi!=lengthh+1;pi+=2)
	suml=suml+datah[pi];
	for(pi=0;pi!=lengthh;pi+=2)
	sumh=sumh+datah[pi];

	//UDP 伪首部+UDP 首部+数据 一起计算校验和，校验和为低 16 位，高出 16 位部分右移16位后加到原来低 16 位上
	summ=(sumh<<8)+suml;
	summ=(summ&0xffff)+(summ>>16);

	//取反过程，并返回
	return (~(unsigned short)summ);

 }

unsigned char UDP_pack(unsigned char *datas,unsigned short length)
{
	t_UDP *UDPBUF;
	unsigned short packhead[8];
	UDPBUF =(t_UDP*)&packhead; //这里用不用取址符号都是对的，正常理解应该不要取址符号
	UDPBUF->sport=H(1024); //源端口号设置 1024=0x0400,直接写 1024 的话，小端模式中第一个字节为 0，第二个字节为 4，
	//但是接收方会认为先收到的为高位，结果成了 0x0004,所以应该先互换高低位写进去

	UDPBUF->dport=H(2844); //目的端口号设置
	UDPBUF->length=H(length+8); //UDP 包长度=数据部分长度+UDP 报头长度
	UDPBUF->udpchksum=0; //UDP 校验和初始化为


	UDPBUF->udpchksum=H(udpchecksum(packhead,datas,length)); //UDP 检验和

	backwards(datas,length,8); //数据包在 Buffer中后移 8 字节
	memcpy(datas,packhead,8); //将 8 字节 UDP 报头复制到 Buffer 前面

	packet_len=length+8;

	return 0x11; //返回传输层的协议为 UDP，以供下一层使用
 }


