/**
 * \addtogroup resolv
 * @{
 */
/**
 * \file
 * DNS resolver code header file.
 * \author Adam Dunkels <adam@dunkels.com>
 */

/*
 * Copyright (c) 2002-2003, Adam Dunkels.
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
 * 3. The name of the author may not be used to endorse or promote
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
 * $Id: resolv.h,v 1.4 2006/06/11 21:46:37 adam Exp $
 *
 */
#ifndef __RESOLV_H__
#define __RESOLV_H__

typedef int uip_tcp_appstate_t;
typedef int uip_udp_appstate_t;

#include "uip.h"

#include "uipopt.h"


/** \internal The maximum number of retries when asking for a name. */
#define MAX_RETRIES 8

/** \internal The DNS message header. */
struct dns_hdr {
  u16_t id;
  u8_t flags1, flags2;
#define DNS_FLAG1_RESPONSE        0x80
#define DNS_FLAG1_OPCODE_STATUS   0x10
#define DNS_FLAG1_OPCODE_INVERSE  0x08
#define DNS_FLAG1_OPCODE_STANDARD 0x00
#define DNS_FLAG1_AUTHORATIVE     0x04
#define DNS_FLAG1_TRUNC           0x02
#define DNS_FLAG1_RD              0x01
#define DNS_FLAG2_RA              0x80
#define DNS_FLAG2_ERR_MASK        0x0f
#define DNS_FLAG2_ERR_NONE        0x00
#define DNS_FLAG2_ERR_NAME        0x03
  u16_t numquestions;
  u16_t numanswers;
  u16_t numauthrr;
  u16_t numextrarr;
};

/** \internal The DNS answer message structure. */
struct dns_answer {
  /* DNS answer record starts with either a domain name or a pointer
     to a name already present somewhere in the packet. */
  u16_t type;
  u16_t class;
  u16_t ttl[2];
  u16_t len;
  uip_ipaddr_t ipaddr;
};

struct namemap {
#define STATE_UNUSED 0
#define STATE_NEW    1
#define STATE_ASKING 2
#define STATE_DONE   3
#define STATE_ERROR  4
  u8_t state;
  u8_t tmr;
  u8_t retries;
  u8_t seqno;
  u8_t err;
  char name[32];
  uip_ipaddr_t ipaddr;
};

#ifndef UIP_CONF_RESOLV_ENTRIES
#define RESOLV_ENTRIES 4
#else /* UIP_CONF_RESOLV_ENTRIES */
#define RESOLV_ENTRIES UIP_CONF_RESOLV_ENTRIES
#endif /* UIP_CONF_RESOLV_ENTRIES */

/**
 * Callback function which is called when a hostname is found.
 *
 * This function must be implemented by the module that uses the DNS
 * resolver. It is called when a hostname is found, or when a hostname
 * was not found.
 *
 * \param name A pointer to the name that was looked up.  \param
 * ipaddr A pointer to a 4-byte array containing the IP address of the
 * hostname, or NULL if the hostname could not be found.
 */
void resolv_found(char *name, u16_t *ipaddr);

/* Functions. */
void resolv_conf(u16_t *dnsserver);
u16_t *resolv_getserver(void);
void resolv_init(void);
u16_t *resolv_lookup(char *name);
void resolv_query(char *name);
void resolv_appcall(void);



#endif /* __RESOLV_H__ */

/** @} */
