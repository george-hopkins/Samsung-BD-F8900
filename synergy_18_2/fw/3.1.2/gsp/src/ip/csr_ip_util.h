#ifndef CSR_IP_UTIL_H__
#define CSR_IP_UTIL_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_util.h"
#include "csr_types.h"
#include "csr_result.h"
#include "csr_ip_handler.h"
#include "lwip/ip_addr.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrIpUtilLwipToCsrIp(const struct ip_addr *ipaddr, CsrUint8 *csrip);
void CsrIpUtilCsrIpToLwIp(const CsrUint8 *csrip, struct ip_addr *ipaddr);
CsrResult CsrIpUtilHandleAllocate(CsrIpInstanceData *instanceData, struct netif *netif, CsrUint32 *ifHandle);
void CsrIpUtilHandleRelease(CsrIpInstanceData *instanceData, CsrUint32 ifHandle);

#ifdef __cplusplus
}
#endif

#endif
