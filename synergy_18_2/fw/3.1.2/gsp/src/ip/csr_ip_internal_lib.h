#ifndef CSR_IP_INTERNAL_LIB_H__
#define CSR_IP_INTERNAL_LIB_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "lwip/pbuf.h"
#include "lwip/netif.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrIpInternalEtherPacketReqSend(CsrUint16 ethType, struct pbuf *buffer, struct netif **netif);

#ifdef __cplusplus
}
#endif

#endif
