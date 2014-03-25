/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/


#include "csr_ip_util.h"
#include "lwip/ip_addr.h"
#include "csr_ip_handler.h"

void CsrIpUtilLwipToCsrIp(const struct ip_addr *ipaddr, CsrUint8 *csrIp)
{
    CsrMemCpy(csrIp, &ipaddr->addr, 4);
}

void CsrIpUtilCsrIpToLwIp(const CsrUint8 *csrIp, struct ip_addr *ipaddr)
{
    CsrMemCpy(&ipaddr->addr, csrIp, 4);
}

CsrResult CsrIpUtilHandleAllocate(CsrIpInstanceData *instanceData, struct netif *netif, CsrUint32 *ifHandle)
{
    for (*ifHandle = 0; *ifHandle < CSR_IP_MAX_INTERFACES; ++(*ifHandle))
    {
        if (instanceData->interfaces[*ifHandle] == NULL)
        {
            instanceData->interfaces[*ifHandle] = netif;
            return CSR_RESULT_SUCCESS;
        }
    }
    return CSR_IP_ETHER_RESULT_NO_MORE_INTERFACES;
}

void CsrIpUtilHandleRelease(CsrIpInstanceData *instanceData, CsrUint32 ifHandle)
{
    instanceData->interfaces[ifHandle] = NULL;
}
