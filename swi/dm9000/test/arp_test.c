#include <arp.h>
#include <test.h>
#include <string.h>
#include <dm9000.h>
#include <stdio.h>

//ARP协议格式的数组
static unsigned char arpsendbuf[42]={
 
       0xff,0xff,0xff,0xff,0xff,0xff,                     	//以太网目标地址，全1表示为广播地址
       0x1a,0x2b,0x3c,0x4d,0x5e,0x6f,        				//以太网源地址
       0x08,0x06,                                        	//帧类型：ARP帧
      
       0x00,0x01,                                       	 //硬件类型：以太网
       0x08,0x00,                                       	 //协议类型：IP协议
       0x06,                                                //硬件地址长度：6字节
       0x04,                                                //协议地址长度：4字节
       0x00,0x01,                                        	//操作码：ARP请求
      
       0xaa,0xbb,0xcc,0xdd,0xee,0xff,        			//发送端以太网硬件地址
       192, 168, 1, 6,                                 //发送端IP协议地址
       0x00,0x00,0x00,0x00,0x00,0x00,        			//接收端以太网硬件地址
       192, 168, 1, 128                                 //接收端IP协议地址
};


/*****************************************************************************
 函 数 名  : arp_test
 功能描述  : arp function test
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年5月11日
    作    者   : QSWWD
    修改内容   : 新生成函数

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
 函 数 名  : arp_process
 功能描述  : arp recieve packet function
 输入参数  : void  
 输出参数  : 无
 返 回 值  : int
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年5月11日
    作    者   : QSWWD
    修改内容   : 新生成函数

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

