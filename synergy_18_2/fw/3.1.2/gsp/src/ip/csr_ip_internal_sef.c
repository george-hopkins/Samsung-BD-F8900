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
#include "csr_ip_internal_sef.h"
#include "csr_ip_internal_prim.h"
#include "csr_ip_handler.h"
#include "csr_ip_util.h"

#include "lwip/opt.h"
#include "lwip/netif.h"
#include "netif/etharp.h"

void CsrIpInternalEtherPacketReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpInternalEtherPacketReq *request = (CsrIpInternalEtherPacketReq *) instanceData->msg;

    struct netif *netif = *request->netif;
    if (netif != NULL)
    {
        switch (request->ethType)
        {
            case ETHTYPE_IP:
            {
                ip_input(request->buffer, netif);
                return;
            }
            case ETHTYPE_ARP:
            {
                etharp_arp_input(netif, (struct eth_addr *) (netif->hwaddr), request->buffer);
                return;
            }
            default:
            {
                /* This cannot happen (see frameRxFunction) */
                break;
            }
        }
    }
    pbuf_free(request->buffer);
    request->buffer = NULL;
}
