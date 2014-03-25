#ifndef CSR_IP_NETLINK_PARSER_H__
#define CSR_IP_NETLINK_PARSER_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <sys/socket.h>
#include <asm/types.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <net/if_packet.h>

#include "csr_types.h"
#include "csr_sched.h"

#include "csr_ip_ifconfig_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

CsrUint32 csrIfAddrMsgIndexGet(struct nlmsghdr *nh);
CsrUint32 csrIfInfoMsgIndexGet(struct nlmsghdr *nh);
void csrIpIfconfigDevLookup(int fd,
    CsrUint32 ifIdx,
    CsrCharString **ifName,
    CsrIpIfconfigIfDetails *ifd);

#ifdef __cplusplus
}
#endif

#endif
