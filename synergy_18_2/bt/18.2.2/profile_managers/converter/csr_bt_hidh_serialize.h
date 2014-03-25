#ifndef CSR_BT_HIDH_SERIALIZE_H__
#define CSR_BT_HIDH_SERIALIZE_H__
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

void CsrBtHidhPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_HIDH_CONNECT_ACCEPT_REQ
CsrSize CsrBtHidhConnectAcceptReqSizeof(void *msg);
CsrUint8 *CsrBtHidhConnectAcceptReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHidhConnectAcceptReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHidhConnectAcceptReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HIDH_CONNECT_ACCEPT_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_CANCEL_CONNECT_ACCEPT_CFM
CsrSize CsrBtHidhCancelConnectAcceptCfmSizeof(void *msg);
CsrUint8 *CsrBtHidhCancelConnectAcceptCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHidhCancelConnectAcceptCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHidhCancelConnectAcceptCfmSerFree CsrBtHidhPfree
#endif /* EXCLUDE_CSR_BT_HIDH_CANCEL_CONNECT_ACCEPT_CFM */

#ifndef EXCLUDE_CSR_BT_HIDH_DISCONNECT_REQ
CsrSize CsrBtHidhDisconnectReqSizeof(void *msg);
CsrUint8 *CsrBtHidhDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHidhDisconnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHidhDisconnectReqSerFree CsrBtHidhPfree
#endif /* EXCLUDE_CSR_BT_HIDH_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_STATUS_IND
CsrSize CsrBtHidhStatusIndSizeof(void *msg);
CsrUint8 *CsrBtHidhStatusIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHidhStatusIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHidhStatusIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HIDH_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_HIDH_GET_IDLE_REQ
CsrSize CsrBtHidhGetIdleReqSizeof(void *msg);
CsrUint8 *CsrBtHidhGetIdleReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHidhGetIdleReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHidhGetIdleReqSerFree CsrBtHidhPfree
#endif /* EXCLUDE_CSR_BT_HIDH_GET_IDLE_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_HANDSHAKE_IND
CsrSize CsrBtHidhHandshakeIndSizeof(void *msg);
CsrUint8 *CsrBtHidhHandshakeIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHidhHandshakeIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHidhHandshakeIndSerFree CsrBtHidhPfree
#endif /* EXCLUDE_CSR_BT_HIDH_HANDSHAKE_IND */

#ifndef EXCLUDE_CSR_BT_HIDH_DISCONNECT_IND
CsrSize CsrBtHidhDisconnectIndSizeof(void *msg);
CsrUint8 *CsrBtHidhDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHidhDisconnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHidhDisconnectIndSerFree CsrBtHidhPfree
#endif /* EXCLUDE_CSR_BT_HIDH_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HIDH_CANCEL_CONNECT_ACCEPT_REQ
CsrSize CsrBtHidhCancelConnectAcceptReqSizeof(void *msg);
CsrUint8 *CsrBtHidhCancelConnectAcceptReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHidhCancelConnectAcceptReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHidhCancelConnectAcceptReqSerFree CsrBtHidhPfree
#endif /* EXCLUDE_CSR_BT_HIDH_CANCEL_CONNECT_ACCEPT_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_CONNECT_CFM
CsrSize CsrBtHidhConnectCfmSizeof(void *msg);
CsrUint8 *CsrBtHidhConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHidhConnectCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHidhConnectCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HIDH_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_HIDH_CONNECT_ACCEPT_CFM
CsrSize CsrBtHidhConnectAcceptCfmSizeof(void *msg);
CsrUint8 *CsrBtHidhConnectAcceptCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHidhConnectAcceptCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHidhConnectAcceptCfmSerFree CsrBtHidhPfree
#endif /* EXCLUDE_CSR_BT_HIDH_CONNECT_ACCEPT_CFM */

#ifndef EXCLUDE_CSR_BT_HIDH_CONTROL_REQ
CsrSize CsrBtHidhControlReqSizeof(void *msg);
CsrUint8 *CsrBtHidhControlReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHidhControlReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHidhControlReqSerFree CsrBtHidhPfree
#endif /* EXCLUDE_CSR_BT_HIDH_CONTROL_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_SECURITY_IN_REQ
CsrSize CsrBtHidhSecurityInReqSizeof(void *msg);
CsrUint8 *CsrBtHidhSecurityInReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHidhSecurityInReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHidhSecurityInReqSerFree CsrBtHidhPfree
#endif /* EXCLUDE_CSR_BT_HIDH_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_GET_PROTOCOL_REQ
CsrSize CsrBtHidhGetProtocolReqSizeof(void *msg);
CsrUint8 *CsrBtHidhGetProtocolReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHidhGetProtocolReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHidhGetProtocolReqSerFree CsrBtHidhPfree
#endif /* EXCLUDE_CSR_BT_HIDH_GET_PROTOCOL_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_DISCONNECT_CFM
CsrSize CsrBtHidhDisconnectCfmSizeof(void *msg);
CsrUint8 *CsrBtHidhDisconnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHidhDisconnectCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHidhDisconnectCfmSerFree CsrBtHidhPfree
#endif /* EXCLUDE_CSR_BT_HIDH_DISCONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_HIDH_REGISTER_USER_REQ
CsrSize CsrBtHidhRegisterUserReqSizeof(void *msg);
CsrUint8 *CsrBtHidhRegisterUserReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHidhRegisterUserReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHidhRegisterUserReqSerFree CsrBtHidhPfree
#endif /* EXCLUDE_CSR_BT_HIDH_REGISTER_USER_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_CONTROL_IND
CsrSize CsrBtHidhControlIndSizeof(void *msg);
CsrUint8 *CsrBtHidhControlIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHidhControlIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHidhControlIndSerFree CsrBtHidhPfree
#endif /* EXCLUDE_CSR_BT_HIDH_CONTROL_IND */

#ifndef EXCLUDE_CSR_BT_HIDH_REGISTER_USER_CFM
CsrSize CsrBtHidhRegisterUserCfmSizeof(void *msg);
CsrUint8 *CsrBtHidhRegisterUserCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHidhRegisterUserCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHidhRegisterUserCfmSerFree CsrBtHidhPfree
#endif /* EXCLUDE_CSR_BT_HIDH_REGISTER_USER_CFM */

#ifndef EXCLUDE_CSR_BT_HIDH_SECURITY_OUT_CFM
CsrSize CsrBtHidhSecurityOutCfmSizeof(void *msg);
CsrUint8 *CsrBtHidhSecurityOutCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHidhSecurityOutCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHidhSecurityOutCfmSerFree CsrBtHidhPfree
#endif /* EXCLUDE_CSR_BT_HIDH_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_HIDH_SET_IDLE_REQ
CsrSize CsrBtHidhSetIdleReqSizeof(void *msg);
CsrUint8 *CsrBtHidhSetIdleReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHidhSetIdleReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHidhSetIdleReqSerFree CsrBtHidhPfree
#endif /* EXCLUDE_CSR_BT_HIDH_SET_IDLE_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_SET_PROTOCOL_REQ
CsrSize CsrBtHidhSetProtocolReqSizeof(void *msg);
CsrUint8 *CsrBtHidhSetProtocolReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHidhSetProtocolReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHidhSetProtocolReqSerFree CsrBtHidhPfree
#endif /* EXCLUDE_CSR_BT_HIDH_SET_PROTOCOL_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_CONNECT_REQ
CsrSize CsrBtHidhConnectReqSizeof(void *msg);
CsrUint8 *CsrBtHidhConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHidhConnectReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHidhConnectReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HIDH_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_DATA_REQ
CsrSize CsrBtHidhDataReqSizeof(void *msg);
CsrUint8 *CsrBtHidhDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHidhDataReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHidhDataReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HIDH_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_SET_REPORT_REQ
CsrSize CsrBtHidhSetReportReqSizeof(void *msg);
CsrUint8 *CsrBtHidhSetReportReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHidhSetReportReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHidhSetReportReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HIDH_SET_REPORT_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_SECURITY_IN_CFM
CsrSize CsrBtHidhSecurityInCfmSizeof(void *msg);
CsrUint8 *CsrBtHidhSecurityInCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHidhSecurityInCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHidhSecurityInCfmSerFree CsrBtHidhPfree
#endif /* EXCLUDE_CSR_BT_HIDH_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_HIDH_GET_REPORT_REQ
CsrSize CsrBtHidhGetReportReqSizeof(void *msg);
CsrUint8 *CsrBtHidhGetReportReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHidhGetReportReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHidhGetReportReqSerFree CsrBtHidhPfree
#endif /* EXCLUDE_CSR_BT_HIDH_GET_REPORT_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_DATA_IND
CsrSize CsrBtHidhDataIndSizeof(void *msg);
CsrUint8 *CsrBtHidhDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHidhDataIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHidhDataIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HIDH_DATA_IND */

#ifndef EXCLUDE_CSR_BT_HIDH_SECURITY_OUT_REQ
CsrSize CsrBtHidhSecurityOutReqSizeof(void *msg);
CsrUint8 *CsrBtHidhSecurityOutReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHidhSecurityOutReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHidhSecurityOutReqSerFree CsrBtHidhPfree
#endif /* EXCLUDE_CSR_BT_HIDH_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_CONNECT_ACCEPT_IND
CsrSize CsrBtHidhConnectAcceptIndSizeof(void *msg);
CsrUint8 *CsrBtHidhConnectAcceptIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHidhConnectAcceptIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHidhConnectAcceptIndSerFree CsrBtHidhPfree
#endif /* EXCLUDE_CSR_BT_HIDH_CONNECT_ACCEPT_IND */

#ifdef __cplusplus
}
#endif

#endif
