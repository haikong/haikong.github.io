#ifndef __UIP_APP_H__
#define __UIP_APP_H__


void my_udp_app(void);

void uip_app_init(void);

void my_tcp_app(void);

#define UIP_APPCALL my_tcp_app 
#if UIP_UDP
#define UIP_UDP_APPCALL my_udp_app
#endif

#endif
