/*******************************************************************************

            (c) Cambridge Silicon Radio Limited 2010

            All rights reserved and confidential information of CSR

*******************************************************************************/

#ifndef __LWIP_NAT_H__
#define __LWIP_NAT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "lwip/opt.h"
#include "lwip/ip.h"

void nat_tmr(void);
void nat_init(void);

#if LWIP_UDP
u8_t nat_udp_useport(u16_t port);
#endif

#if LWIP_TCP
u8_t nat_tcp_useport(u16_t port);
#endif

void nat_output(struct pbuf *p, struct ip_addr *src, struct ip_addr *dest,
    u8_t proto, struct netif *netif);
void nat_input(struct pbuf *p, struct ip_hdr *iphdr,
    u16_t iphdr_hlen, u16_t iphdr_len, struct netif *netif);

#ifdef __cplusplus
}
#endif

#endif /* __LWIP_NAT_H__ */
