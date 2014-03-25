/*******************************************************************************

            (c) Cambridge Silicon Radio Limited 2010

            All rights reserved and confidential information of CSR

*******************************************************************************/

#include "lwip/opt.h"
#include "lwip/nat.h"
#include "lwip/netif.h"
#include "lwip/ip.h"
#include "lwip/inet_chksum.h"
#include "lwip/tcp.h"
#include "lwip/udp.h"
#include "lwip/dhcp.h"

#if IP_FORWARD

struct nat_entry
{
    struct ip_addr src_ip;
    u16_t          src_port;

    struct ip_addr dest_ip;
    u16_t          dest_port;

    struct netif *netif;
    u16_t         nat_port;

    u16_t lifetime;
};

#if LWIP_UDP
#ifndef NAT_UDP_LIFETIME
#define NAT_UDP_LIFETIME    (2)
#endif /* NAT_UDP_LIFETIME */

#ifndef NAT_UDP_ENTRY_LIMIT
#define NAT_UDP_ENTRY_LIMIT 128
#endif

struct nat_entry *nat_udp_list = NULL;
static int nat_udp_listsize;
static int nat_udp_entries = 0;

static struct nat_entry *nat_udp_lookup_out(struct ip_addr *src, u16_t src_port,
    struct ip_addr *dest, u16_t dest_port, struct netif *netif)
{
    struct nat_entry *entry;
    int udp_idx, udp_found;

    entry = NULL;

    for (udp_idx = 0, udp_found = 0;
         udp_found < nat_udp_entries;
         udp_idx++)
    {
        struct nat_entry *lookup;

        lookup = &nat_udp_list[udp_idx];

        if (lookup->lifetime)
        {
            udp_found++;

            if (ip_addr_cmp(&lookup->src_ip, src) &&
                ip_addr_cmp(&lookup->dest_ip, dest) &&
                (lookup->netif == netif) &&
                (lookup->src_port == src_port) &&
                (lookup->dest_port == dest_port))
            {
                entry = lookup;
                break;
            }
        }
    }

    return entry;
}

static struct nat_entry *nat_udp_lookup_in(struct ip_addr *src, u16_t src_port,
    struct ip_addr *dest, u16_t dest_port, struct netif *netif)
{
    struct nat_entry *entry;
    int udp_idx, udp_found;

    entry = NULL;

    for (udp_idx = 0, udp_found = 0;
         udp_found < nat_udp_entries;
         udp_idx++)
    {
        struct nat_entry *lookup;

        lookup = &nat_udp_list[udp_idx];

        if (lookup->lifetime)
        {
            udp_found++;

            if (ip_addr_cmp(src, &lookup->dest_ip) &&
                ip_addr_cmp(dest, &netif->ip_addr) &&
                (netif == lookup->netif) &&
                (dest_port == lookup->nat_port) &&
                (src_port == lookup->dest_port))
            {
                entry = lookup;
                break;
            }
        }
    }

    return entry;
}

#ifndef UDP_NAT_PORT_START
#define UDP_NAT_PORT_START  10240
#endif

#ifndef UDP_NAT_PORT_END
#define UDP_NAT_PORT_END   20480
#endif

static u16_t nat_udp_portget(void)
{
    struct udp_pcb *ipcb;
    u16_t port;

    port = UDP_NAT_PORT_START;

    for (ipcb = udp_pcbs;
        ipcb;
        ipcb = ipcb->next)
    {
        if (ipcb->local_port == port || nat_udp_useport(port))
        {
            port++;
            ipcb = udp_pcbs;
        }
    }

    if (port >= UDP_NAT_PORT_END)
    {
        port = 0;
    }

    return port;
}

/*
 * Find the oldest (in terms of activity) state
 * and nuke it.
 */
static struct nat_entry *nat_udp_recycle()
{
    struct nat_entry *entry;
    int udp_idx, oldest_idx;
    u16_t oldest_val;

    oldest_val = NAT_UDP_LIFETIME + 1;
    oldest_idx = NAT_UDP_ENTRY_LIMIT;

    for (udp_idx = 0;
        udp_idx < nat_udp_listsize;
        udp_idx++)
    {
        entry = &nat_udp_list[udp_idx];

        if (entry->lifetime == 1)
        {
            oldest_idx = udp_idx;
            break;
        }
        else if (entry->lifetime < oldest_val)
        {
            oldest_val = entry->lifetime;
            oldest_idx = udp_idx;
        }
    }

    entry = &nat_udp_list[oldest_idx];

    return entry;
}

/*
 * Returns a pointer to a UDP NAT state entry
 * with the NAT port to use for this connection.
 */
static struct nat_entry *nat_udp_alloc(void)
{
    struct nat_entry *entry;

    if (nat_udp_entries < nat_udp_listsize)
    {
        /* There's free elements in the current list */

        u16_t nat_port;

        nat_port = nat_udp_portget();

        entry = NULL;

        if (nat_port)
        {
            int idx;
            /* Find free element */

            for (idx = 0; idx < nat_udp_listsize; idx++)
            {
                entry = &nat_udp_list[idx];

                if (entry->lifetime == 0)
                {
                    entry->nat_port = nat_port;
                    break;
                }
            }

            nat_udp_entries++;
        }
        else
        {
            entry = nat_udp_recycle();
        }
    }
    else if (nat_udp_listsize < NAT_UDP_ENTRY_LIMIT)
    {
        /* Reallocate */
        struct nat_entry *newList;

        /* Reallocate double size to amortize copy cost */
        newList = CsrPmemAlloc(sizeof(*newList) * nat_udp_listsize * 2);
        CsrMemCpy(newList, nat_udp_list, nat_udp_listsize * sizeof(*newList));
        CsrMemSet(&newList[nat_udp_listsize], 0,
            nat_udp_listsize * sizeof(*newList));

        CsrPmemFree(nat_udp_list);
        nat_udp_list = newList;
        nat_udp_listsize *= 2;

        /* Recurse once to obtain entry */
        entry = nat_udp_alloc();
    }
    else
    {
        /*
         * Table is full -- recycle a state entry.
         */

        entry = nat_udp_recycle();        
    }

    return entry;
}

/*
 * The lint directive below is required because
 * lint thinks nat_udp_alloc() may return NULL.
 */
/*lint -e{794} suppress conceivable use of null pointer */
static void nat_udp_output(struct pbuf *p, struct ip_addr *src,
    struct ip_addr *dest, struct netif *netif)
{
    struct ip_hdr *iphdr;
    struct udp_hdr *udphdr;
    struct nat_entry *entry;
    u16_t src_port, dest_port;
    s16_t hlen;

    iphdr = p->payload;
    hlen = IPH_HL(iphdr);
    hlen *= 4;

    udphdr = (struct udp_hdr *) (((char *) p->payload) + hlen);
    src_port = ntohs(udphdr->src);
    dest_port = ntohs(udphdr->dest);

    if (!(src_port == DHCP_CLIENT_PORT && dest_port == DHCP_SERVER_PORT))
    {
        entry = nat_udp_lookup_out(src, src_port, dest, dest_port, netif);

        if (entry == NULL)
        {
            /* Entry not found in state table.  Make new or recycle one. */
            entry = nat_udp_alloc();

            ip_addr_set(&entry->src_ip, src);
            ip_addr_set(&entry->dest_ip, dest);
            entry->netif = netif;
            entry->src_port = src_port;
            entry->dest_port = dest_port;
        }

        entry->lifetime = NAT_UDP_LIFETIME;

        ip_addr_set(&iphdr->src, &netif->ip_addr);
        udphdr->src = htons(entry->nat_port);

        if (udphdr->chksum != 0)
        {
            pbuf_header(p, (s16_t) -hlen);

            udphdr->chksum = 0;
            udphdr->chksum = inet_chksum_pseudo(p, src, dest,
                IP_PROTO_UDP, ntohs(udphdr->len));

            pbuf_header(p, hlen);
        }

        IPH_CHKSUM_SET(iphdr, 0);
#if CHECKSUM_GEN_IP
        IPH_CHKSUM_SET(iphdr, inet_chksum(iphdr, hlen));
#endif
    }
}

static void nat_udp_input(struct pbuf *p, struct ip_hdr *iphdr,
    u16_t hlen, u16_t len, struct netif *netif)
{
    struct udp_hdr *udphdr;
    struct nat_entry *entry;
    u16_t src_port, dest_port;

    udphdr = (struct udp_hdr *) (((char *) p->payload) + hlen);

    src_port = ntohs(udphdr->src);
    dest_port = ntohs(udphdr->dest);

    if (!(src_port == DHCP_SERVER_PORT && dest_port == DHCP_CLIENT_PORT))
    {
        entry = nat_udp_lookup_in(&iphdr->src, src_port,
            &iphdr->dest, dest_port, netif);

        if (entry)
        {
            entry->lifetime = NAT_UDP_LIFETIME;

            ip_addr_set(&iphdr->dest, &entry->src_ip);
            udphdr->dest = htons(entry->src_port);

            if (udphdr->chksum != 0)
            {
                struct ip_addr src_ip, dest_ip;

                ip_addr_set(&src_ip, &iphdr->src);
                ip_addr_set(&dest_ip, &iphdr->dest);

                pbuf_header(p, (s16_t) -hlen);

                udphdr->chksum = 0;
                udphdr->chksum = inet_chksum_pseudo(p, &src_ip, &dest_ip,
                    IP_PROTO_UDP, ntohs(udphdr->len));

                if (udphdr->chksum == 0)
                {
                    udphdr->chksum = 0xffff;
                }

                pbuf_header(p, hlen);
            }

            IPH_CHKSUM_SET(iphdr, 0);
#if CHECKSUM_GEN_IP
            IPH_CHKSUM_SET(iphdr, inet_chksum(iphdr, hlen));
#endif
        }
    }
}
#endif /* LWIP_UDP */

#if LWIP_TCP
#ifndef NAT_TCP_LIFETIME
#define NAT_TCP_LIFETIME    (10)
#endif /* NAT_TCP_LIFETIME */

#ifndef NAT_TCP_ENTRY_LIMIT
#define NAT_TCP_ENTRY_LIMIT 512
#endif

struct nat_entry *nat_tcp_list = NULL;
static int nat_tcp_listsize;
static int nat_tcp_entries = 0;

static struct nat_entry *nat_tcp_lookup_out(struct ip_addr *src, u16_t src_port,
    struct ip_addr *dest, u16_t dest_port, struct netif *netif)
{
    struct nat_entry *entry;
    int tcp_idx, tcp_found;

    entry = NULL;

    for (tcp_idx = 0, tcp_found = 0;
         tcp_found < nat_tcp_entries;
         tcp_idx++)
    {
        struct nat_entry *lookup;
        
        lookup = &nat_tcp_list[tcp_idx];

        if (lookup->lifetime)
        {
            tcp_found++;

            if (ip_addr_cmp(&lookup->src_ip, src) &&
                ip_addr_cmp(&lookup->dest_ip, dest) &&
                (lookup->netif == netif) &&
                (lookup->src_port == src_port) &&
                (lookup->dest_port == dest_port))
            {
                entry = lookup;
                break;
            }
        }
    }

    return entry;
}

static struct nat_entry *nat_tcp_lookup_in(struct ip_addr *src, u16_t src_port,
    struct ip_addr *dest, u16_t dest_port, struct netif *netif)
{
    struct nat_entry *entry;
    int tcp_idx, tcp_found;

    entry = NULL;

    for (tcp_idx = 0, tcp_found = 0;
         tcp_found < nat_tcp_entries;
         tcp_idx++)
    {
        struct nat_entry *lookup;

        lookup = &nat_tcp_list[tcp_idx];

        if (lookup->lifetime)
        {
            tcp_found++;

            if (ip_addr_cmp(src, &lookup->dest_ip) &&
                ip_addr_cmp(dest, &netif->ip_addr) &&
                (netif == lookup->netif) &&
                (dest_port == lookup->nat_port) &&
                (src_port == lookup->dest_port))
            {
                entry = lookup;
                break;
            }
        }
    }

    return entry;
}

#ifndef TCP_NAT_PORT_START
#define TCP_NAT_PORT_START  10240
#endif

#ifndef TCP_NAT_PORT_END
#define TCP_NAT_PORT_END   20480
#endif

static u16_t nat_tcp_portget(void)
{
    struct tcp_pcb *pcb;
    u16_t port;

    port = TCP_NAT_PORT_START;

    while (port < TCP_NAT_PORT_END)
    {
        /* Check the listen pcbs. */
        for (pcb = (struct tcp_pcb *) tcp_listen_pcbs.pcbs;
            pcb != NULL;
            pcb = pcb->next) {
            if (pcb->local_port == port) {
                break;
            }
        }

        if (pcb != NULL)
        {
            port++;
            continue;
        }
            
        /* Check the connected pcbs. */
        for (pcb = tcp_active_pcbs;
            pcb != NULL;
            pcb = pcb->next) {
            if (pcb->local_port == port)
            {
                break;
            }
        }

        if (pcb != NULL)
        {
            port++;
            continue;
        }

        /* Check the bound, not yet connected pcbs. */
        for (pcb = tcp_bound_pcbs;
            pcb != NULL;
            pcb = pcb->next)
        {
            if (pcb->local_port == port) {
                break;
            }
        }

        if (pcb != NULL)
        {
            port++;
            continue;
        }


        /* Check the pcbs in TIME-WAIT state. */
        for(pcb = tcp_tw_pcbs;
            pcb != NULL;
            pcb = pcb->next)
        {
            if (pcb->local_port == port) {
                port++;
                break;
            }
        }

        if (pcb != NULL)
        {
            port++;
            continue;
        }

        if (nat_tcp_useport(port))
        {
            port++;
            continue;
        }
        else
        {
            break;
        }
    }

    if (port >= TCP_NAT_PORT_END)
    {
        port = 0;
    }

    return port;
}

/*
 * Find the oldest (in terms of activity) state
 * and nuke it.
 */
static struct nat_entry *nat_tcp_recycle()
{
    struct nat_entry *entry;
    int tcp_idx, oldest_idx;
    u16_t oldest_val;

    oldest_val = NAT_TCP_LIFETIME + 1;
    oldest_idx = NAT_TCP_ENTRY_LIMIT;
    
    for (tcp_idx = 0;
        tcp_idx < nat_tcp_listsize;
        tcp_idx++)
    {
        entry = &nat_tcp_list[tcp_idx];

        if (entry->lifetime == 1)
        {
            oldest_idx = tcp_idx;
            break;
        }
        else if (entry->lifetime < oldest_val)
        {
            oldest_val = entry->lifetime;
            oldest_idx = tcp_idx;
        }
    }

    entry = &nat_tcp_list[oldest_idx];

    return entry;
}

/*
 * Returns a pointer to a TCP NAT state entry
 * with the NAT port to use for this connection.
 */
static struct nat_entry *nat_tcp_alloc(void)
{
    struct nat_entry *entry;

    if (nat_tcp_entries < nat_tcp_listsize)
    {
        /* There's free elements in the current list */

        u16_t nat_port;

        nat_port = nat_tcp_portget();

        entry = NULL;

        if (nat_port)
        {
            int idx;
            /* Find free element */

            for (idx = 0; idx < nat_tcp_listsize; idx++)
            {
                entry = &nat_tcp_list[idx];

                if (entry->lifetime == 0)
                {
                    entry->nat_port = nat_port;
                    break;
                }
            }

            nat_tcp_entries++;
        }
        else
        {
            entry = nat_tcp_recycle();
        }
    }
    else if (nat_tcp_listsize < NAT_TCP_ENTRY_LIMIT)
    {
        /* Reallocate */
        struct nat_entry *newList;

        /* Reallocate double size to amortize copy cost */
        newList = CsrPmemAlloc(sizeof(*newList) * nat_tcp_listsize * 2);
        CsrMemCpy(newList, nat_tcp_list, nat_tcp_listsize * sizeof(*newList));
        CsrMemSet(&newList[nat_tcp_listsize], 0,
            nat_tcp_listsize * sizeof(*newList));

        CsrPmemFree(nat_tcp_list);
        nat_tcp_list = newList;
        nat_tcp_listsize *= 2;

        /* Recurse once to obtain entry */
        entry = nat_tcp_alloc();
    }
    else
    {
        /*
         * Table is full -- recycle a state entry.
         */

        entry = nat_tcp_recycle();        
    }

    return entry;
}

/*
 * The lint directive below is required because
 * lint thinks nat_tcp_alloc() may return NULL.
 */
/*lint -e{794} suppress conceivable use of null pointer */
static void nat_tcp_output(struct pbuf *p, struct ip_addr *src,
    struct ip_addr *dest, struct netif *netif)
{
    struct ip_hdr *iphdr;
    struct nat_entry *entry;
    struct tcp_hdr *tcphdr;
    u16_t src_port, dest_port;
    u8_t hlen;

    iphdr = p->payload;
    hlen = IPH_HL(iphdr);
    hlen *= 4;

    tcphdr = (struct tcp_hdr *) (((char *) p->payload) + hlen);
    src_port = ntohs(tcphdr->src);
    dest_port = ntohs(tcphdr->dest);

    entry = nat_tcp_lookup_out(src, src_port, dest, dest_port, netif);

    if (entry == NULL)
    {
        /* Entry not found in state table.  Make new or recycle one. */
        entry = nat_tcp_alloc();

        ip_addr_set(&entry->src_ip, src);
        ip_addr_set(&entry->dest_ip, dest);
        entry->netif = netif;
        entry->src_port = src_port;
        entry->dest_port = dest_port;
    }

    entry->lifetime = NAT_TCP_LIFETIME;

    ip_addr_set(&iphdr->src, &netif->ip_addr);
    tcphdr->src = htons(entry->nat_port);

    pbuf_header(p, (s16_t) -hlen);

    tcphdr->chksum = 0;
    tcphdr->chksum = inet_chksum_pseudo(p, src, dest,
        IP_PROTO_TCP, (u16_t) p->tot_len);

    pbuf_header(p, hlen);

    IPH_CHKSUM_SET(iphdr, 0);
#if CHECKSUM_GEN_IP
    IPH_CHKSUM_SET(iphdr, inet_chksum(iphdr, hlen));
#endif
}

static void nat_tcp_input(struct pbuf *p, struct ip_hdr *iphdr,
    u16_t hlen, u16_t len, struct netif *netif)

{
    struct tcp_hdr *tcphdr;
    struct nat_entry *entry;
    u16_t src_port, dest_port;

    tcphdr = (struct tcp_hdr *) (((char *) p->payload) + hlen);

    src_port = ntohs(tcphdr->src);
    dest_port = ntohs(tcphdr->dest);

    entry = nat_tcp_lookup_in(&iphdr->src, src_port,
        &iphdr->dest, dest_port, netif);

    if (entry)
    {
        struct ip_addr src_ip, dest_ip;

        entry->lifetime = NAT_TCP_LIFETIME;

        ip_addr_set(&iphdr->dest, &entry->src_ip);
        tcphdr->dest = htons(entry->src_port);

        ip_addr_set(&src_ip, &iphdr->src);
        ip_addr_set(&dest_ip, &iphdr->dest);

        pbuf_header(p, (s16_t) -hlen);

        tcphdr->chksum = 0;
        tcphdr->chksum = inet_chksum_pseudo(p, &src_ip, &dest_ip,
            IP_PROTO_TCP, (u16_t) (len - hlen));

        if (tcphdr->chksum == 0)
        {
            tcphdr->chksum = 0xffff;
        }

        pbuf_header(p, hlen);

        IPH_CHKSUM_SET(iphdr, 0);
#if CHECKSUM_GEN_IP
        IPH_CHKSUM_SET(iphdr, inet_chksum(iphdr, hlen));
#endif
    }
}
#endif /* LWIP_TCP */

#if 0
static void nat_icmp_output(struct pbuf *p, struct ip_addr *src,
    struct ip_addr *dest, struct netif *netif)
{
}

static void nat_icmp_input(struct pbuf *p, struct ip_hdr *iphdr,
    u16_t iphdr_hlen, u16_t iphdr_len, struct netif *netif)
{
}
#endif

/*
 * Public interface.
 */

#if LWIP_UDP
u8_t nat_udp_useport(u16_t port)
{
    struct nat_entry *entry;
    int udp_idx, udp_found;

    for (udp_idx = 0, udp_found = 0;
         udp_found < nat_udp_entries;
         udp_idx++)
    {
        entry = &nat_udp_list[udp_idx];

        if (entry->lifetime)
        {
            udp_found++;

            if (entry->nat_port == port)
            {
                break;
            }
        }
    }

    if (udp_found == nat_udp_entries)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
#endif

#if LWIP_TCP
u8_t nat_tcp_useport(u16_t port)
{
    struct nat_entry *entry;
    int tcp_idx, tcp_found;

    for (tcp_idx = 0, tcp_found = 0;
         tcp_found < nat_tcp_entries;
         tcp_idx++)
    {
        entry = &nat_tcp_list[tcp_idx];

        if (entry->lifetime)
        {
            tcp_found++;

            if (entry->nat_port == port)
            {
                break;
            }
        }
    }

    if (tcp_found == nat_tcp_entries)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
#endif

void nat_output(struct pbuf *p, struct ip_addr *src, struct ip_addr *dest,
    u8_t proto, struct netif *netif)
{
    switch (proto)
    {
#if LWIP_TCP
        case IP_PROTO_TCP:
            nat_tcp_output(p, src, dest, netif);
            break;
#endif /* LWIP_TCP */

#if LWIP_UDP
        case IP_PROTO_UDP:
            nat_udp_output(p, src, dest, netif);
            break;
#endif /* LWIP_UDP */

#if 0
        case IP_PROTO_ICMP:
            nat_icmp_output(p, src, dest, netif);
            break;
#endif

        default:
            /*
             * If we don't support the protocol, ignore the packet.
             */
            break;
    }
}

void nat_input(struct pbuf *p, struct ip_hdr *iphdr,
    u16_t iphdr_hlen, u16_t iphdr_len, struct netif *netif)
{
    u8_t proto;

    proto = IPH_PROTO(iphdr);

    switch (proto)
    {
#if LWIP_TCP
        case IP_PROTO_TCP:
            nat_tcp_input(p, iphdr, iphdr_hlen, iphdr_len, netif);
            break;
#endif /* LWIP_TCP */

#if LWIP_UDP
        case IP_PROTO_UDP:
            nat_udp_input(p, iphdr, iphdr_hlen, iphdr_len, netif);
            break;
#endif /* LWIP_UDP */

#if 0
        case IP_PROTO_ICMP:
            nat_icmp_input(p, iphdr, iphdr_hlen, iphdr_len, netif);
            break;
#endif

        default:
            /*
             * If we don't support the protocol, ignore the packet.
             */
            break;
    }
}

/**
 * Periodic check for expired NAT entries.
 */
void nat_tmr(void)
{
#if LWIP_UDP
    struct nat_entry *udp_entry;
    int udp_idx, udp_count, udp_found;
#endif
#if LWIP_TCP
    struct nat_entry *tcp_entry;
    int tcp_idx, tcp_count, tcp_found;
#endif

#if LWIP_UDP
    for (udp_idx = 0, udp_found = 0, udp_count = nat_udp_entries;
         udp_found < udp_count;
         udp_idx++)
    {
        udp_entry = &nat_udp_list[udp_idx];

        if (udp_entry->lifetime)
        {
            udp_found++;
        
            if (udp_entry->lifetime == 1)
            {
                CsrMemSet(udp_entry, 0, sizeof(*udp_entry));
                nat_udp_entries--;
            }
            else
            {
                udp_entry->lifetime--;
            }
        }
    }
#endif

#if LWIP_TCP
    for (tcp_idx = 0, tcp_found = 0, tcp_count = nat_tcp_entries;
         tcp_found < tcp_count;
         tcp_idx++)
    {
        tcp_entry = &nat_tcp_list[tcp_idx];

        if (tcp_entry->lifetime)
        {
            tcp_found++;
        
            if (tcp_entry->lifetime == 1)
            {
                CsrMemSet(tcp_entry, 0, sizeof(*tcp_entry));
                nat_tcp_entries--;
            }
            else
            {
                tcp_entry->lifetime--;
            }
        }
    }
#endif
}

void nat_init(void)
{
#if LWIP_TCP
    nat_tcp_listsize = 32;
    nat_tcp_list = CsrPmemZalloc(sizeof(*nat_tcp_list) * nat_tcp_listsize);
#endif

#if LWIP_UDP
    nat_udp_listsize = 8;
    nat_udp_list = CsrPmemZalloc(sizeof(*nat_udp_list) * nat_udp_listsize);
#endif
}

#endif /* IP_FORWARD */
