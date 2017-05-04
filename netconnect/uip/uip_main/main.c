/*
 * Copyright (c) 2001, Adam Dunkels.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed by Adam Dunkels.
 * 4. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * This file is part of the uIP TCP/IP stack.
 *
 * $Id: main.c,v 1.16 2006/06/11 21:55:03 adam Exp $
 *
 */
 
#include "uip_arp.h"
#include "uip.h"
#include "tapdev.h"
#include "uip_timer.h"
#include <net.h>
#include <string.h>
#include <stdio.h>
#include "uip_app.h"
#include <dm9000.h>
#define BUF ((struct uip_eth_hdr *)&uip_buf[0])

#ifndef NULL
#define NULL (void *)0
#endif /* NULL */

/*---------------------------------------------------------------------------*/
/*****************************************************************************
 函 数 名  : uip_main
 功能描述  : uip main looplock function
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2017年4月17日
    作    者   : QSWWD
    修改内容   : 新生成函数

*****************************************************************************/
void uip_main(void)
{
  int i;
  uip_ipaddr_t ipaddr;
  struct timer periodic_timer, arp_timer;

  timer_set(&periodic_timer, CLOCK_SECOND / 2);
  timer_set(&arp_timer, CLOCK_SECOND * 10);
  
  uip_init();

  uip_ipaddr(ipaddr, 192,168,1,6);
  uip_sethostaddr(ipaddr);
  uip_ipaddr(ipaddr, 192,168,1,1);
  uip_setdraddr(ipaddr);
  uip_ipaddr(ipaddr, 255,255,255,0);
  uip_setnetmask(ipaddr);
  /*MAC*/
  memcpy(&uip_ethaddr,eth_mac_addr(),6);
  /*app initialize*/
  uip_app_init();
  //main loop
  while(1) {
    	uip_len = tapdev_read(uip_buf);
    if(uip_len > 0) 
	{
		
      if(BUF->type == htons(UIP_ETHTYPE_IP)) 
	  {
		uip_arp_ipin();
		uip_input();
		/* If the above function invocation resulted in data that
	   	should be sent out on the network, the global variable
	   	uip_len is set to a value > 0. */
		if(uip_len > 0) {
	  		uip_arp_out();
	  		tapdev_send(uip_buf,uip_len);
		}
      } 
	  else if(BUF->type == htons(UIP_ETHTYPE_ARP)) 
	  {
			uip_arp_arpin();
		/* If the above function invocation resulted in data that
		   should be sent out on the network, the global variable
		   uip_len is set to a value > 0. */
		if(uip_len > 0) {
		  tapdev_send(uip_buf,uip_len);
		}
      }

    } 
	else if(timer_expired(&periodic_timer)) 
	{
     	timer_reset(&periodic_timer);
      	for(i = 0; i < UIP_CONNS; i++) 
		{
			uip_periodic(i);
			/* If the above function invocation resulted in data that
	   		should be sent out on the network, the global variable
	   		uip_len is set to a value > 0. */
			if(uip_len > 0) 
			{
	  			uip_arp_out();
	  			tapdev_send(uip_buf,uip_len);
			}
      	}

#if UIP_UDP
      for(i = 0; i < UIP_UDP_CONNS; i++) 
	  {
		uip_udp_periodic(i);
		/* If the above function invocation resulted in data that
	   		should be sent out on the network, the global variable
	   		uip_len is set to a value > 0.
	   	*/
		if(uip_len > 0) 
		{
		  uip_arp_out();
		  tapdev_send(uip_buf,uip_len);
		}
      }
#endif /* UIP_UDP */
      
      /* Call the ARP timer function every 10 seconds. */
     if(timer_expired(&arp_timer)) 
	 {
		timer_reset(&arp_timer);
		uip_arp_timer();
      }
    }
  }

}
/*---------------------------------------------------------------------------*/
void
uip_log(char *m)
{
  printf("uIP log message: %s\n\r", m);
}
void
resolv_found(char *name, u16_t *ipaddr)
{
  u16_t *ipaddr2;
  ipaddr2 = ipaddr;
  if(ipaddr == NULL) {
    printf("Host '%s' not found.\n", name);
  } else {
    printf("Found name '%s' = %d.%d.%d.%d\n", name,
	   htons(ipaddr[0]) >> 8,
	   htons(ipaddr[0]) & 0xff,
	   htons(ipaddr[1]) >> 8,
	   htons(ipaddr[1]) & 0xff);
    /*    webclient_get("www.sics.se", 80, "/~adam/uip");*/
  }
}

void
smtp_done(unsigned char code)
{
  printf("SMTP done with code %d\n", code);
}
void
webclient_closed(void)
{
  printf("Webclient: connection closed\n");
}
void
webclient_aborted(void)
{
  printf("Webclient: connection aborted\n");
}
void
webclient_timedout(void)
{
  printf("Webclient: connection timed out\n");
}
void
webclient_connected(void)
{
  printf("Webclient: connected, waiting for data...\n");
}
void
webclient_datahandler(char *data, u16_t len)
{
  printf("Webclient: got %d bytes of data.\n", len);
}
/*---------------------------------------------------------------------------*/
