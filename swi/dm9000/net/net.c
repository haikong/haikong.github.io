#include <net.h>
#include <types.h>
#include <stdio.h>
#include <string.h>
#include <io.h>
/*ethnet devices*/
static struct eth_device *eth_devices;

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
	if((dev == NULL) || (strlen(dev->name) < sizeof(dev->name)))
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
int  eth_init (struct eth_device * edev)
{
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
int  eth_send(struct eth_device *edev, void *packet,unsigned int length)
{
	return 0;
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
int  eth_recv(struct eth_device *netdev, unsigned char* data_src )
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
void eth_halt(struct eth_device *edev)
{

}



