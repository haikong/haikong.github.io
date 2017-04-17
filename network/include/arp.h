#ifndef _ARP_H__
#define _ARP_H__
#include "net.h"

void mac_send_pack(unsigned char *datas,unsigned int length,e_MAC_PROTO proto);
unsigned short ipchecksum(unsigned char *dp);
unsigned short IP_pack(unsigned char *datas,unsigned short length,unsigned char type);
unsigned short ARP_pack(unsigned char *datas,unsigned short length,e_MAC_PROTO proto);
unsigned short udpchecksum(unsigned char *head,unsigned char *datah, unsigned short lengthh);
unsigned char UDP_pack(unsigned char *datas,unsigned short length);

#endif
