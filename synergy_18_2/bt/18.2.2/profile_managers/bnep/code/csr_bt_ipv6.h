#ifndef IPV6_H
#define IPV6_H

#include "csr_synergy.h"
/****************************************************************************

                (c) CSR plc 2008

                All rights reserved

FILE:            csr_bt_ipv6.h  --  _internet _protocol, _version 6

DESCRIPTION:    $Log: csr_bt_ipv6.h,v $
                Revision 1.1  2003/08/29 06:26:57  ab02
                no message

                Revision 1.3  2002/10/18 16:48:12  mgr
                Implement new NAP/GN-supporting BNEP API.

                Revision 1.2  2002/09/10 15:47:22  bs01
                Merge changes between jbs-pan-branch-start and jbs-pan-branch-mark-a
                onto the main line. Move external pan headers to interface/app/pan directory.

                Revision 1.1.2.2  2002/09/02 15:02:49  bs01
                Pull all external headers used by pan into a single pantypes.h.

                Revision 1.1.2.1  2002/09/02 12:37:27  bs01
                Move pan headers and support from interface/app/bluestack and dev/src/csr/pan
                into interface/app/pan. Update dev side to cope.

                Revision 1.5  2002/08/30 18:01:01  mgr
                Rename IP_ADDRs' s6_addr[] to w[].
                Rename PSKEY_IPVn_FIXED_* to PSKEY_IPVn_STATIC_*.
                Rename PSKEY_IPVn_ROUTER_ADDR to PSKEY_IPVn_STATIC_ROUTER_ADDR.

                Revision 1.4  2002/08/30 18:01:00  mgr
                Rename IP_ADDRs' s6_addr[] to w[].
                Rename PSKEY_IPVn_FIXED_* to PSKEY_IPVn_STATIC_*.
                Rename PSKEY_IPVn_ROUTER_ADDR to PSKEY_IPVn_STATIC_ROUTER_ADDR.

                Revision 1.3  2002/08/20 17:40:56  mgr
                BlueLabify.

                Revision 1.2  2002/07/18 14:05:25  mgr
                Merge from chef-1-branch.

                Revision 1.1.2.2  2002/04/05 18:39:33  mgr
                Make the TCP primitives public.
                Improve the public/private header split.
                Some lint pacification.

                Revision 1.1.2.1  2002/03/18 15:19:09  mgr
                Initial revision.

REVISION:        $Revision: #1 $

****************************************************************************/

#include "csr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************

    struct IPV6_ADDR  --  an IPv6 address

******************************************************************************/

typedef struct IPV6_ADDR
{
    CsrUint16 w[8];
} IPV6_ADDR;


/******************************************************************************

    #defines and macros for IPv6 addressing

Note that reserved addresses include IPv4-mapped, IPv4-compatible,
loopback and unspecified addresses.

******************************************************************************/

#define IPV6_ADDR_LEN 16 /* in octets */
#define IPV6_PREFIX_RESERVED 0 /* First 8 bits */
#define IPV6_PREFIX_LINK_LOCAL_UNICAST 0xfe80U /* First 10 bits */
#define IPV6_PREFIX_SITE_LOCAL_UNICAST 0xfec0U /* First 10 bits */
#define IPV6_PREFIX_MULTICAST 0xffU /* First 8 bits */
#define IPV6_MULTICAST_SCOPE_NODE 1 /* Fourth 4 bits */
#define IPV6_MULTICAST_SCOPE_LINK 2 /* Fourth 4 bits */
#define IPV6_MULTICAST_SCOPE_SITE 5 /* Fourth 4 bits */
#define IPV6_MULTICAST_SCOPE_ORGANIZATION 8 /* Fourth 4 bits */
#define IPV6_MULTICAST_SCOPE_GLOBAL 14 /* Fourth 4 bits */
#define IPV6_MULTICAST_GROUP_ALL_NODES 1 /* Last 112 bits */
#define IPV6_MULTICAST_GROUP_ALL_ROUTERS 2 /* Last 112 bits */
#define IPV6_MULTICAST_GROUP_SOLICITED 0x01ffU /* Last-but-24 88 bits */
#define IPV6_IS_LINK_LOCAL_UNICAST(x) ((x)->w[0] >> 6 == \
        /* fe80::/10 */          IPV6_PREFIX_LINK_LOCAL_UNICAST >> 6)
#define IPV6_IS_MULTICAST(x) ((x)->w[0] >> 8 == \
        /* ff00::/8 */           IPV6_PREFIX_MULTICAST)
#define IPV6_IS_RESERVED(x) ((x)->w[0] >> 8 == \
        /* ::/8 */               IPV6_PREFIX_RESERVED)
#define IPV6_IS_UNSPECIFIED(x) (( \
        /* :: */                 (x)->w[0] | \
                                 (x)->w[1] | \
                                 (x)->w[2] | \
                                 (x)->w[3] | \
                                 (x)->w[4] | \
                                 (x)->w[5] | \
                                 (x)->w[6] | \
                                 (x)->w[7] \
                               ) == 0)
#define IPV6_IS_LOOPBACK(x) (( \
        /* ::1 */                (x)->w[0] | \
                                 (x)->w[1] | \
                                 (x)->w[2] | \
                                 (x)->w[3] | \
                                 (x)->w[4] | \
                                 (x)->w[5] | \
                                 (x)->w[6] | \
                                 ((x)->w[7] ^ 1) \
                               ) == 0)
#define IPV6_IS_LINK_LOCAL_ALL_NODES_MULTICAST(x) (( \
        /* ff02::1 */            ((x)->w[0] ^ (IPV6_PREFIX_MULTICAST << 8 | IPV6_MULTICAST_SCOPE_LINK)) | \
                                 (x)->w[1] | \
                                 (x)->w[2] | \
                                 (x)->w[3] | \
                                 (x)->w[4] | \
                                 (x)->w[5] | \
                                 (x)->w[6] | \
                                 ((x)->w[7] ^ IPV6_MULTICAST_GROUP_ALL_NODES) \
                               ) == 0)
#define IPV6_IS_SOLICITED_MULTICAST(x,y) (( \
        /* ff02::1:ffyy:yyyy */  ((x)->w[0] ^ (IPV6_PREFIX_MULTICAST << 8 | IPV6_MULTICAST_SCOPE_LINK)) | \
                                 (x)->w[1] | \
                                 (x)->w[2] | \
                                 (x)->w[3] | \
                                 (x)->w[4] | \
                                 ((x)->w[5] ^ (IPV6_MULTICAST_GROUP_SOLICITED >> 8)) | \
                                 ((x)->w[6] ^ ((IPV6_MULTICAST_GROUP_SOLICITED & 0xff) << 8 & 0xff00 | ((y)->w[6] & 0xff))) | \
                                 ((x)->w[7] ^ (y)->w[7]) \
                               ) == 0)
#define IPV6_IS_IPV4MAPPED(x) (( \
        /* ::ffff:x.y.z.w */     (x)->w[0] | \
                                 (x)->w[1] | \
                                 (x)->w[2] | \
                                 (x)->w[3] | \
                                 (x)->w[4] | \
                                 ~(x)->w[5] \
                               ) == 0)
#define IPV6_IS_IPV4COMPATIBLE(x) (( \
        /* ::x.y.z.w */          (x)->w[0] | \
                                 (x)->w[1] | \
                                 (x)->w[2] | \
                                 (x)->w[3] | \
                                 (x)->w[4] | \
                                 (x)->w[5] \
                               ) == 0)
#define IPV6_IS_SAME_ADDR(x,y) (( \
                                 ((x)->w[0] ^ (y)->w[0]) | \
                                 ((x)->w[1] ^ (y)->w[1]) | \
                                 ((x)->w[2] ^ (y)->w[2]) | \
                                 ((x)->w[3] ^ (y)->w[3]) | \
                                 ((x)->w[4] ^ (y)->w[4]) | \
                                 ((x)->w[5] ^ (y)->w[5]) | \
                                 ((x)->w[6] ^ (y)->w[6]) | \
                                 ((x)->w[7] ^ (y)->w[7]) \
                               ) == 0)

#ifdef __cplusplus
}
#endif

#endif /* IPV6_H */
