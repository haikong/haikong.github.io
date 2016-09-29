#include <net.h>
#include <types.h>
#include <stdio.h>
#include <string.h>
#include <io.h>
/*ethnet devices*/
static struct eth_device *eth_devices;

/*****************************************************************************
 函 数 名  : cal_chksum
 功能描述  : caculate the checksum
 输入参数  : unsigned short *addr  
             int len               
 输出参数  : 无
 返 回 值  : unsigned
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年5月18日
    作    者   : QSWWD
    修改内容   : 新生成函数

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
 函 数 名  : eth_register
 功能描述  : eth_register
 输入参数  : struct eth_device *dev  
 输出参数  : 无
 返 回 值  : 0:ok;-1:error
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年9月20日
    作    者   : QSWWD
    修改内容   : 新生成函数

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
 函 数 名  : eth_unregister
 功能描述  : 取消注册
 输入参数  : struct eth_device *dev  
 输出参数  : 无
 返 回 值  : 0:ok;-1:error
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年9月20日
    作    者   : QSWWD
    修改内容   : 新生成函数

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
 函 数 名  : eth_init
 功能描述  : ethnetwork devices initialize
 输入参数  : struct eth_device * edev  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年9月25日
    作    者   : QSWWD
    修改内容   : 新生成函数

*****************************************************************************/
int  eth_init (struct eth_device * edev)
{
	return 0;
}
/*****************************************************************************
 函 数 名  : eth_send
 功能描述  : network send packets
 输入参数  : struct eth_device *edev  
             void *packet             
             unsigned int length      
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年9月25日
    作    者   : QSWWD
    修改内容   : 新生成函数

*****************************************************************************/
int  eth_send(struct eth_device *edev, void *packet,unsigned int length)
{
	return 0;
}
/*****************************************************************************
 函 数 名  : eth_recv
 功能描述  : network recives packets
 输入参数  : struct eth_device *netdev  
             unsigned char* data_src    
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年9月25日
    作    者   : QSWWD
    修改内容   : 新生成函数

*****************************************************************************/
int  eth_recv(struct eth_device *netdev, unsigned char* data_src )
{
	return 0;
}
/*****************************************************************************
 函 数 名  : eth_halt
 功能描述  : network halt
 输入参数  : struct eth_device *edev  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年9月25日
    作    者   : QSWWD
    修改内容   : 新生成函数

*****************************************************************************/
void eth_halt(struct eth_device *edev)
{

}



