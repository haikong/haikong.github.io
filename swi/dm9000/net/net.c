#include <net.h>
#include <types.h>
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <glob.h>
/*ethnet devices*/
static struct eth_device *eth_devices;
static struct eth_device *current_ethdev;

/* XXX in both little & big endian machines 0xFFFF == ntohs(-1) */
/* default is without VLAN */
ushort		NetOurVLAN = 0xFFFF;
/* ditto */
ushort		NetOurNativeVLAN = 0xFFFF;


/*****************************************************************************
 �� �� ��  : cal_chksum
 ��������  : caculate the checksum
 �������  : unsigned short *addr  
             int len               
 �������  : ��
 �� �� ֵ  : unsigned
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��5��18��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

*****************************************************************************/
unsigned short cal_chksum(unsigned short *addr,int len)
{
	int nleft =len;
	int sum = 0;
	unsigned short *w = addr;
	unsigned short cksum = 0;

	while(nleft > 1){
		sum += *w++;
		nleft -= 2;
	}
	if(nleft == 1){
		sum += *(unsigned char*)w;
	}
	sum = (sum >> 16) + (sum & 0xffff);
	sum += sum >> 16;
	cksum = ~sum;
	return cksum;
}

unsigned NetCksum(uchar *ptr, int len)
{
	ulong	xsum;
	ushort *p = (ushort *)ptr;

	xsum = 0;
	while (len-- > 0)
		xsum += *p++;
	xsum = (xsum & 0xffff) + (xsum >> 16);
	xsum = (xsum & 0xffff) + (xsum >> 16);
	return xsum & 0xffff;
}


int NetCksumOk(uchar *ptr, int len)
{
	return !((NetCksum(ptr, len) + 1) & 0xfffe);
}

/*****************************************************************************
 �� �� ��  : eth_register
 ��������  : eth_register
 �������  : struct eth_device *dev  
 �������  : ��
 �� �� ֵ  : 0:ok;-1:error
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��9��20��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

*****************************************************************************/
int eth_register(struct eth_device *dev)
{
	struct eth_device *d;
	static int index;
	/*assert*/
	if(dev == NULL)
	{
		printf("eth_register error.\n\t");
		return -1;
	}
	/*add to the list*/
	if (!eth_devices) {
		eth_devices = dev;
	} else {
		for (d = eth_devices; d->next != eth_devices; d = d->next)
			;
		d->next = dev;
	}

	dev->state = ETH_STATE_INIT;
	dev->next  = eth_devices;
	dev->index = index++;
	return 0;
}

/*****************************************************************************
 �� �� ��  : eth_unregister
 ��������  : ȡ��ע��
 �������  : struct eth_device *dev  
 �������  : ��
 �� �� ֵ  : 0:ok;-1:error
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��9��20��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

*****************************************************************************/
int eth_unregister(struct eth_device *dev)
{
	struct eth_device *cur;

	/* No device */
	if (!eth_devices)
		return -1;

	for (cur = eth_devices; cur->next != eth_devices && cur->next != dev;
	     cur = cur->next)
		;

	/* Device not found */
	if (cur->next != dev)
		return -1;

	cur->next = dev->next;

	if (eth_devices == dev)
		eth_devices = dev->next == eth_devices ? NULL : dev->next;
	return 0;
}

/*****************************************************************************
 �� �� ��  : eth_init
 ��������  : ethnetwork devices initialize
 �������  : struct eth_device * edev  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��9��25��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

*****************************************************************************/
int  eth_init (void)
{
	
	/* No device */
	if (!eth_devices)
		return -1;
	current_ethdev = eth_devices;
	current_ethdev->init();
	return 0;
}
/*****************************************************************************
 �� �� ��  : eth_send
 ��������  : network send packets
 �������  : struct eth_device *edev  
             void *packet             
             unsigned int length      
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��9��25��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

*****************************************************************************/
int  eth_send(void *packet,unsigned int length)
{
		/* No device */
	if (!current_ethdev)
		return -1;
	return current_ethdev->send(packet,length);
}
/*****************************************************************************
 �� �� ��  : eth_recv
 ��������  : network recives packets
 �������  : struct eth_device *netdev  
             unsigned char* data_src    
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��9��25��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

*****************************************************************************/
int  eth_recv( unsigned char* data_src )
{

	return 0;
}
/*****************************************************************************
 �� �� ��  : eth_halt
 ��������  : network halt
 �������  : struct eth_device *edev  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��9��25��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

*****************************************************************************/
void eth_halt(void)
{

}


/*****************************************************************************
 �� �� ��  : NetReceive
 ��������  : recieve data from netdevices
 �������  : uchar *inpkt  
             int len       
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��10��9��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

*****************************************************************************/
void NetReceive(uchar *inpkt, int len)
{
	t_ETH_HDR *et;
	struct ip_udp_hdr *ip;
	int eth_proto;
#if defined(CONFIG_CMD_CDP)
	int iscdp;
#endif
	ushort cti = 0, vlanid = VLAN_NONE, myvlanid, mynvlanid;

	debug_cond(DEBUG_NET_PKT, "packet received\n");

	et = (t_ETH_HDR *)inpkt;

	/* too small packet? */
	if (len < ETHER_HDR_SIZE)
		return;

#ifdef CONFIG_API
	if (push_packet) {
		(*push_packet)(inpkt, len);
		return;
	}
#endif
	myvlanid = ntohs(NetOurVLAN);
	if (myvlanid == (ushort)-1)
		myvlanid = VLAN_NONE;
	mynvlanid = ntohs(NetOurNativeVLAN);
	if (mynvlanid == (ushort)-1)
		mynvlanid = VLAN_NONE;

	eth_proto = ntohs(et->type);

	if (eth_proto < 1514) {
		struct e802_hdr *et802 = (struct e802_hdr *)et;
		/*
		 *	Got a 802.2 packet.  Check the other protocol field.
		 *	XXX VLAN over 802.2+SNAP not implemented!
		 */
		eth_proto = ntohs(et802->et_prot);

		ip = (struct ip_udp_hdr *)(inpkt + E802_HDR_SIZE);
		len -= E802_HDR_SIZE;

	} else if (eth_proto != PROT_VLAN) {	/* normal packet */
		ip = (struct ip_udp_hdr *)(inpkt + ETHER_HDR_SIZE);
		len -= ETHER_HDR_SIZE;

	} else {			/* VLAN packet */
		struct vlan_ethernet_hdr *vet =
			(struct vlan_ethernet_hdr *)et;

		debug_cond(DEBUG_NET_PKT, "VLAN packet received\n");

		/* too small packet? */
		if (len < VLAN_ETHER_HDR_SIZE)
			return;

		/* if no VLAN active */
		if ((ntohs(NetOurVLAN) & VLAN_IDMASK) == VLAN_NONE)
			return;

		cti = ntohs(vet->vet_tag);
		vlanid = cti & VLAN_IDMASK;
		eth_proto = ntohs(vet->vet_type);

		ip = (struct ip_udp_hdr *)(inpkt + VLAN_ETHER_HDR_SIZE);
		len -= VLAN_ETHER_HDR_SIZE;
	}

	debug_cond(DEBUG_NET_PKT, "Receive from protocol 0x%x\n", eth_proto);

	if ((myvlanid & VLAN_IDMASK) != VLAN_NONE) {
		if (vlanid == VLAN_NONE)
			vlanid = (mynvlanid & VLAN_IDMASK);
		/* not matched? */
		if (vlanid != (myvlanid & VLAN_IDMASK))
			return;
	}

	switch (eth_proto) {

	case PROT_ARP:
		break;

#ifdef CONFIG_CMD_RARP
	case PROT_RARP:
		break;
#endif
	case PROT_IP:
		debug_cond(DEBUG_NET_PKT, "Got IP\n");
		/* Before we start poking the header, make sure it is there */
		if (len < IP_UDP_HDR_SIZE) {
			debug("len bad %d < %lu\n", len,
				(ulong)IP_UDP_HDR_SIZE);
			return;
		}
		/* Check the packet length */
		if (len < ntohs(ip->ip_len)) {
			debug("len bad %d < %d\n", len, ntohs(ip->ip_len));
			return;
		}
		len = ntohs(ip->ip_len);
		debug_cond(DEBUG_NET_PKT, "len=%d, v=%02x\n",
			len, ip->ip_hl_v & 0xff);

		/* Can't deal with anything except IPv4 */
		if ((ip->ip_hl_v & 0xf0) != 0x40)
			return;
		/* Can't deal with IP options (headers != 20 bytes) */
		if ((ip->ip_hl_v & 0x0f) > 0x05)
			return;
		/* Check the Checksum of the header */
		if (!NetCksumOk((uchar *)ip, IP_HDR_SIZE / 2)) {
			debug("checksum bad\n");
			return;
		}
		break;
	}
}


