/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_sched.h"
#include "csr_util.h"

#include "csr_ip_ifconfig_prim.h"
#include "csr_ip_internal_prim.h"
#include "csr_ip_internal_lib.h"
#include "csr_ip_handler.h"
#include "csr_ip_task.h"

#include "lwip/netif.h"
#include "lwip/pbuf.h"

void CsrIpInternalEtherPacketReqSend(CsrUint16 ethType, struct pbuf *buffer, struct netif **netif)
{
    CsrIpInternalEtherPacketReq *request = CsrPmemAlloc(sizeof(CsrIpInternalEtherPacketReq));
    request->type = CSR_IP_INTERNAL_ETHER_PACKET_REQ;
    request->ethType = ethType;
    request->buffer = buffer;
    request->netif = netif;
    CsrSchedMessagePut(CSR_IP_IFACEQUEUE, CSR_IP_INTERNAL_PRIM, request);
}
