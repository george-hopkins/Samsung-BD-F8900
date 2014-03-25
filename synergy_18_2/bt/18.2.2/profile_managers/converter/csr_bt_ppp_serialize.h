#ifndef CSR_BT_PPP_SERIALIZE_H__
#define CSR_BT_PPP_SERIALIZE_H__
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

void CsrBtPppPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_PPP_DISCONNECT_IND
CsrSize CsrBtPppDisconnectIndSizeof(void *msg);
CsrUint8 *CsrBtPppDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPppDisconnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPppDisconnectIndSerFree CsrBtPppPfree
#endif /* EXCLUDE_CSR_BT_PPP_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_PPP_AUTHENTICATE_IND
CsrSize CsrBtPppAuthenticateIndSizeof(void *msg);
CsrUint8 *CsrBtPppAuthenticateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPppAuthenticateIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPppAuthenticateIndSerFree CsrBtPppPfree
#endif /* EXCLUDE_CSR_BT_PPP_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_PPP_UP_IND
CsrSize CsrBtPppUpIndSizeof(void *msg);
CsrUint8 *CsrBtPppUpIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPppUpIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPppUpIndSerFree CsrBtPppPfree
#endif /* EXCLUDE_CSR_BT_PPP_UP_IND */

#ifndef EXCLUDE_CSR_BT_PPP_CONNECT_REQ
CsrSize CsrBtPppConnectReqSizeof(void *msg);
CsrUint8 *CsrBtPppConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPppConnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPppConnectReqSerFree CsrBtPppPfree
#endif /* EXCLUDE_CSR_BT_PPP_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_PPP_DISCONNECT_REQ
CsrSize CsrBtPppDisconnectReqSizeof(void *msg);
CsrUint8 *CsrBtPppDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPppDisconnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPppDisconnectReqSerFree CsrBtPppPfree
#endif /* EXCLUDE_CSR_BT_PPP_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_PPP_DATA_IND
CsrSize CsrBtPppDataIndSizeof(void *msg);
CsrUint8 *CsrBtPppDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPppDataIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtPppDataIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_PPP_DATA_IND */

#ifndef EXCLUDE_CSR_BT_PPP_CONNECT_CFM
CsrSize CsrBtPppConnectCfmSizeof(void *msg);
CsrUint8 *CsrBtPppConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPppConnectCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPppConnectCfmSerFree CsrBtPppPfree
#endif /* EXCLUDE_CSR_BT_PPP_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_PPP_REGISTER_NETWORK_PROTOCOL_REQ
CsrSize CsrBtPppRegisterNetworkProtocolReqSizeof(void *msg);
CsrUint8 *CsrBtPppRegisterNetworkProtocolReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPppRegisterNetworkProtocolReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPppRegisterNetworkProtocolReqSerFree CsrBtPppPfree
#endif /* EXCLUDE_CSR_BT_PPP_REGISTER_NETWORK_PROTOCOL_REQ */

#ifndef EXCLUDE_CSR_BT_PPP_DATA_REQ
CsrSize CsrBtPppDataReqSizeof(void *msg);
CsrUint8 *CsrBtPppDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPppDataReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtPppDataReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_PPP_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_PPP_NCP_REQ
CsrSize CsrBtPppNcpReqSizeof(void *msg);
CsrUint8 *CsrBtPppNcpReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPppNcpReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtPppNcpReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_PPP_NCP_REQ */

#ifndef EXCLUDE_CSR_BT_PPP_REGISTER_NETWORK_PROTOCOL_CFM
CsrSize CsrBtPppRegisterNetworkProtocolCfmSizeof(void *msg);
CsrUint8 *CsrBtPppRegisterNetworkProtocolCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPppRegisterNetworkProtocolCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPppRegisterNetworkProtocolCfmSerFree CsrBtPppPfree
#endif /* EXCLUDE_CSR_BT_PPP_REGISTER_NETWORK_PROTOCOL_CFM */

#ifndef EXCLUDE_CSR_BT_PPP_DOWN_IND
CsrSize CsrBtPppDownIndSizeof(void *msg);
CsrUint8 *CsrBtPppDownIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPppDownIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPppDownIndSerFree CsrBtPppPfree
#endif /* EXCLUDE_CSR_BT_PPP_DOWN_IND */

#ifndef EXCLUDE_CSR_BT_PPP_NCP_IND
CsrSize CsrBtPppNcpIndSizeof(void *msg);
CsrUint8 *CsrBtPppNcpIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPppNcpIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtPppNcpIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_PPP_NCP_IND */

#ifndef EXCLUDE_CSR_BT_PPP_AUTHENTICATE_RES
CsrSize CsrBtPppAuthenticateResSizeof(void *msg);
CsrUint8 *CsrBtPppAuthenticateResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPppAuthenticateResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtPppAuthenticateResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_PPP_AUTHENTICATE_RES */

#ifdef __cplusplus
}
#endif

#endif
