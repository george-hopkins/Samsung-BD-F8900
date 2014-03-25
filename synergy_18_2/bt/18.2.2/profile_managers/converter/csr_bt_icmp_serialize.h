#ifndef CSR_BT_ICMP_SERIALIZE_H__
#define CSR_BT_ICMP_SERIALIZE_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_types.h"

#include "csr_synergy.h"
#ifdef __cplusplus
extern "C" {
#endif

void CsrBtIcmpPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_ICMP_PING_REQ
CsrSize CsrBtIcmpPingReqSizeof(void *msg);
CsrUint8 *CsrBtIcmpPingReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtIcmpPingReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtIcmpPingReqSerFree CsrBtIcmpPfree
#endif /* EXCLUDE_CSR_BT_ICMP_PING_REQ */

#ifndef EXCLUDE_CSR_BT_ICMP_PING_CFM
CsrSize CsrBtIcmpPingCfmSizeof(void *msg);
CsrUint8 *CsrBtIcmpPingCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtIcmpPingCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtIcmpPingCfmSerFree CsrBtIcmpPfree
#endif /* EXCLUDE_CSR_BT_ICMP_PING_CFM */

#ifndef EXCLUDE_CSR_BT_ICMP_DESTINATION_UNREACHABLE_REQ
CsrSize CsrBtIcmpDestinationUnreachableReqSizeof(void *msg);
CsrUint8 *CsrBtIcmpDestinationUnreachableReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtIcmpDestinationUnreachableReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtIcmpDestinationUnreachableReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_ICMP_DESTINATION_UNREACHABLE_REQ */

#ifndef EXCLUDE_CSR_BT_ICMP_PORT_UNREACHABLE_IND
CsrSize CsrBtIcmpPortUnreachableIndSizeof(void *msg);
CsrUint8 *CsrBtIcmpPortUnreachableIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtIcmpPortUnreachableIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtIcmpPortUnreachableIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_ICMP_PORT_UNREACHABLE_IND */

#ifdef __cplusplus
}
#endif

#endif
