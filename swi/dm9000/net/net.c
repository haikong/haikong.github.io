#include <net.h>

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

