#ifndef CSR_BT_IP_SERIALIZE_H__
#define CSR_BT_IP_SERIALIZE_H__
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

void CsrBtIpPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_IP_ADDRESS_REQ
CsrSize CsrBtIpAddressReqSizeof(void *msg);
CsrUint8 *CsrBtIpAddressReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtIpAddressReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtIpAddressReqSerFree CsrBtIpPfree
#endif /* EXCLUDE_CSR_BT_IP_ADDRESS_REQ */

#ifndef EXCLUDE_CSR_BT_IP_SHUTDOWN_IND
CsrSize CsrBtIpShutdownIndSizeof(void *msg);
CsrUint8 *CsrBtIpShutdownIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtIpShutdownIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtIpShutdownIndSerFree CsrBtIpPfree
#endif /* EXCLUDE_CSR_BT_IP_SHUTDOWN_IND */

#ifndef EXCLUDE_CSR_BT_IP_DATA_IND
CsrSize CsrBtIpDataIndSizeof(void *msg);
CsrUint8 *CsrBtIpDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtIpDataIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtIpDataIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_IP_DATA_IND */

#ifndef EXCLUDE_CSR_BT_IP_ADDRESS_IND
CsrSize CsrBtIpAddressIndSizeof(void *msg);
CsrUint8 *CsrBtIpAddressIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtIpAddressIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtIpAddressIndSerFree CsrBtIpPfree
#endif /* EXCLUDE_CSR_BT_IP_ADDRESS_IND */

#ifndef EXCLUDE_CSR_BT_IP_DATA_REQ
CsrSize CsrBtIpDataReqSizeof(void *msg);
CsrUint8 *CsrBtIpDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtIpDataReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtIpDataReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_IP_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_IP_MAC_ADDRESS_REQ
CsrSize CsrBtIpMacAddressReqSizeof(void *msg);
CsrUint8 *CsrBtIpMacAddressReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtIpMacAddressReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtIpMacAddressReqSerFree CsrBtIpPfree
#endif /* EXCLUDE_CSR_BT_IP_MAC_ADDRESS_REQ */

#ifndef EXCLUDE_CSR_BT_IP_SHUTDOWN_REQ
CsrSize CsrBtIpShutdownReqSizeof(void *msg);
CsrUint8 *CsrBtIpShutdownReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtIpShutdownReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtIpShutdownReqSerFree CsrBtIpPfree
#endif /* EXCLUDE_CSR_BT_IP_SHUTDOWN_REQ */

#ifdef __cplusplus
}
#endif

#endif
