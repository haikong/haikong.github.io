#include <stdio.h>
#include "uip_app.h"
#include "uip-conf.h"
#include "smtp.h"
typedef int uip_tcp_appstate_t;
typedef int uip_udp_appstate_t;

#include "uip.h"
#include <string.h>

extern void uip_send(const void *data, int len);
extern void uip_listen(u16_t port);
extern void telnetd_init(void);
extern void telnetd_appcall(void);
extern void dhcpc_appcall(void);
extern void dhcpc_init(const void *mac_addr, int mac_len);
extern void resolv_appcall(void);
extern void resolv_init(void);
extern void resolv_conf(u16_t *dnsserver);
extern void resolv_query(char *name);
extern void httpd_init(void);
extern void httpd_appcall(void);
extern void hello_world_appcall(void);
extern void hello_world_init(void);
extern void webclient_init(void);
extern void webclient_appcall(void);

extern u16_t uip_len;
extern void* uip_appdata;
extern struct uip_udp_conn *uip_udp_conn;
extern uip_ipaddr_t uip_hostaddr;
extern struct uip_eth_addr uip_ethaddr;


/*****************************************************************************
 �� �� ��  : udp_default_app
 ��������  : a dup default application
 �������  : void  
 �������  : ��
 �� �� ֵ  : static
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��4��17��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

*****************************************************************************/
static void udp_default_app(void)
{
	printf("the udp protocol can't support this,closed!\n\r");
	uip_close();
}
#if UIP_UDP 
/*****************************************************************************
 �� �� ��  : my_udp8899_init
 ��������  : a uip udp function for local port is 8899
 �������  : void  
 �������  : ��
 �� �� ֵ  : static
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��4��17��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

*****************************************************************************/
static void my_udp8899_init(void)
{
	uip_ipaddr_t remote_addr;
	uip_ipaddr(remote_addr, 192,168,1,8);
	struct uip_udp_conn *my_udp_con = uip_udp_new(&remote_addr,HTONS(8899));
	if(my_udp_con != NULL)
	{
		uip_udp_bind(my_udp_con,HTONS(8899));
	}
}
#endif  

/*****************************************************************************
 �� �� ��  : udp_8899_app
 ��������  : a uip udp function for local port is 8899
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��4��17��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

*****************************************************************************/
static void udp_8899_app(void)
{
	if(uip_poll())
	{
		char *tmp_dat = "the auto send!\n\r";
		uip_send(tmp_dat,strlen(tmp_dat));
	}
	if(uip_newdata())
	{
		char *tmp_data = "receive the data!\n\r";
		uip_send(tmp_data,strlen(tmp_data));
	}
}
#if UIP_UDP
/*****************************************************************************
 �� �� ��  : my_udp_app
 ��������  : the uip for udp application function
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��4��17��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

*****************************************************************************/
void my_udp_app(void)
{
	printf("my_udp_app...\n\r");
	switch(uip_udp_conn->rport)
	{
	case HTONS(8899):
		udp_8899_app();
		break;
	case HTONS(67):
		dhcpc_appcall();
		break;
	case HTONS(68):
		dhcpc_appcall();
		break;
	case HTONS(53):
		resolv_appcall();
		break;
	default:
		udp_default_app();
		break;
	}
}
#endif
//example 2 states
struct example2_state {
	enum {WELCOME_SENT, WELCOME_ACKED} state;
};

/*****************************************************************************
 �� �� ��  : example2_init
 ��������  : just a example function for tcp connect
 �������  : void  
 �������  : ��
 �� �� ֵ  : static
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��4��17��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

*****************************************************************************/
static void example2_init(void) 
{
	uip_listen(HTONS(2345));
}

/*****************************************************************************
 �� �� ��  : example2_app
 ��������  : just a example function for tcp connect
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��4��17��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

*****************************************************************************/
static void example2_app(void) 
{
	struct example2_state *s;
	s = (struct example2_state *)uip_conn->appstate;
	if(uip_connected()) {
		s->state = WELCOME_SENT;
		uip_send("Welcome!\n\r", 10);
		return;
	}
	if(uip_acked() && s->state == WELCOME_SENT) {
		s->state = WELCOME_ACKED;
	}
	if(uip_newdata()) { 
		uip_send("ok!You are trying to connect to UIP.\n\r", 43);
	}
	if(uip_rexmit()) {
		switch(s->state) {
		case WELCOME_SENT:
			uip_send("Welcome,the rexmit!\n\r", 20);
			break;
		case WELCOME_ACKED:
			uip_send("ok!this is retransmit.\n\r",30);
			break;
		default:
			uip_send("the rexmit ,Your's command is something error.\n\r", 50);
			break;
		}
	}
}

/*****************************************************************************
 �� �� ��  : example1_init
 ��������  : just a example function for tcp connect
 �������  : void  
 �������  : ��
 �� �� ֵ  : static
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��4��17��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

*****************************************************************************/
static void example1_init(void) {
	uip_listen(HTONS(1234));
}

/*****************************************************************************
 �� �� ��  : example1_app
 ��������  : just a example function for tcp connect
 �������  : void  
 �������  : ��
 �� �� ֵ  : static
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��4��17��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

*****************************************************************************/
static void example1_app(void) {
	if(uip_newdata() || uip_rexmit()) {
		uip_send("ok\n", 3);
	}
}

/*****************************************************************************
 �� �� ��  : uip_app_init
 ��������  : the uip appliacation function
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��4��17��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

*****************************************************************************/
void uip_app_init(void)
{
	uip_ipaddr_t ipaddr;
	uip_ipaddr(ipaddr, 8,8,8,8);
	
	/*	hello_world_init();*/
	

	
	/*
	  resolv_init();
	  uip_ipaddr(ipaddr, 195,54,122,204);
	  resolv_conf(ipaddr);
	  resolv_query("www.sics.se");*/

	example1_init();
	example2_init();
	telnetd_init();
	smtp_init();
	uip_ipaddr(ipaddr, 127,0,0,1);
	smtp_configure("localhost", ipaddr);
	SMTP_SEND("adam@sics.se", NULL, "uip-testing@example.com",
		  "Testing SMTP from uIP",
		  "Test message sent by uIP\r\n");	
	hello_world_init();
	httpd_init();
	
#if UIP_UDP
	my_udp8899_init();
	dhcpc_init(&uip_ethaddr,6);
	resolv_init();
    resolv_conf(ipaddr);
    resolv_query("www.baidu.com");	
#endif
}

/*****************************************************************************
 �� �� ��  : my_tcp_app
 ��������  : tcp initialize for uip
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��4��17��
    ��    ��   : QSWWD
    �޸�����   : �����ɺ���

*****************************************************************************/
void my_tcp_app(void)
{
	printf("UIP_APPCALL...\n\r");	
	switch(uip_conn->lport) {
	case HTONS(1234):
		example1_app();
		break;
	case HTONS(2345):
		example2_app();
		break;
	case HTONS(23):
		telnetd_appcall();
		break;	
	case HTONS(80):
		httpd_appcall();
		break;
	case HTONS(1000):
		hello_world_appcall();
		break;
	default:
		printf("The protocal not support!\n\r");
		break;
	}	
}

