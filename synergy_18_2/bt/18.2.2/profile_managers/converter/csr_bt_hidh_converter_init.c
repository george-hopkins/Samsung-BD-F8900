/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_synergy.h"
#include "csr_msgconv.h"
#include "csr_pmem.h"
#include "csr_util.h"
#include "csr_bt_hand_coded.h"
#include "csr_bt_hidh_converter_init.h"
#ifndef EXCLUDE_CSR_BT_HIDH_MODULE
#include "csr_bt_hidh_serialize.h"
#include "csr_bt_hidh_prim.h"

static CsrMsgConvMsgEntry csr_bt_hidh_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_HIDH_CANCEL_CONNECT_ACCEPT_CFM
    { CSR_BT_HIDH_CANCEL_CONNECT_ACCEPT_CFM, CsrBtHidhCancelConnectAcceptCfmSizeof, CsrBtHidhCancelConnectAcceptCfmSer, CsrBtHidhCancelConnectAcceptCfmDes, CsrBtHidhCancelConnectAcceptCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HIDH_CANCEL_CONNECT_ACCEPT_CFM */
#ifndef EXCLUDE_CSR_BT_HIDH_CANCEL_CONNECT_ACCEPT_REQ
    { CSR_BT_HIDH_CANCEL_CONNECT_ACCEPT_REQ, CsrBtHidhCancelConnectAcceptReqSizeof, CsrBtHidhCancelConnectAcceptReqSer, CsrBtHidhCancelConnectAcceptReqDes, CsrBtHidhCancelConnectAcceptReqSerFree },
#endif /* EXCLUDE_CSR_BT_HIDH_CANCEL_CONNECT_ACCEPT_REQ */
#ifndef EXCLUDE_CSR_BT_HIDH_CONNECT_ACCEPT_CFM
    { CSR_BT_HIDH_CONNECT_ACCEPT_CFM, CsrBtHidhConnectAcceptCfmSizeof, CsrBtHidhConnectAcceptCfmSer, CsrBtHidhConnectAcceptCfmDes, CsrBtHidhConnectAcceptCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HIDH_CONNECT_ACCEPT_CFM */
#ifndef EXCLUDE_CSR_BT_HIDH_CONNECT_ACCEPT_IND
    { CSR_BT_HIDH_CONNECT_ACCEPT_IND, CsrBtHidhConnectAcceptIndSizeof, CsrBtHidhConnectAcceptIndSer, CsrBtHidhConnectAcceptIndDes, CsrBtHidhConnectAcceptIndSerFree },
#endif /* EXCLUDE_CSR_BT_HIDH_CONNECT_ACCEPT_IND */
#ifndef EXCLUDE_CSR_BT_HIDH_CONNECT_ACCEPT_REQ
    { CSR_BT_HIDH_CONNECT_ACCEPT_REQ, CsrBtHidhConnectAcceptReqSizeof, CsrBtHidhConnectAcceptReqSer, CsrBtHidhConnectAcceptReqDes, CsrBtHidhConnectAcceptReqSerFree },
#endif /* EXCLUDE_CSR_BT_HIDH_CONNECT_ACCEPT_REQ */
#ifndef EXCLUDE_CSR_BT_HIDH_CONNECT_CFM
    { CSR_BT_HIDH_CONNECT_CFM, CsrBtHidhConnectCfmSizeof, CsrBtHidhConnectCfmSer, CsrBtHidhConnectCfmDes, CsrBtHidhConnectCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HIDH_CONNECT_CFM */
#ifndef EXCLUDE_CSR_BT_HIDH_CONNECT_REQ
    { CSR_BT_HIDH_CONNECT_REQ, CsrBtHidhConnectReqSizeof, CsrBtHidhConnectReqSer, CsrBtHidhConnectReqDes, CsrBtHidhConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_HIDH_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_HIDH_CONTROL_IND
    { CSR_BT_HIDH_CONTROL_IND, CsrBtHidhControlIndSizeof, CsrBtHidhControlIndSer, CsrBtHidhControlIndDes, CsrBtHidhControlIndSerFree },
#endif /* EXCLUDE_CSR_BT_HIDH_CONTROL_IND */
#ifndef EXCLUDE_CSR_BT_HIDH_CONTROL_REQ
    { CSR_BT_HIDH_CONTROL_REQ, CsrBtHidhControlReqSizeof, CsrBtHidhControlReqSer, CsrBtHidhControlReqDes, CsrBtHidhControlReqSerFree },
#endif /* EXCLUDE_CSR_BT_HIDH_CONTROL_REQ */
#ifndef EXCLUDE_CSR_BT_HIDH_DATA_IND
    { CSR_BT_HIDH_DATA_IND, CsrBtHidhDataIndSizeof, CsrBtHidhDataIndSer, CsrBtHidhDataIndDes, CsrBtHidhDataIndSerFree },
#endif /* EXCLUDE_CSR_BT_HIDH_DATA_IND */
#ifndef EXCLUDE_CSR_BT_HIDH_DATA_REQ
    { CSR_BT_HIDH_DATA_REQ, CsrBtHidhDataReqSizeof, CsrBtHidhDataReqSer, CsrBtHidhDataReqDes, CsrBtHidhDataReqSerFree },
#endif /* EXCLUDE_CSR_BT_HIDH_DATA_REQ */
#ifndef EXCLUDE_CSR_BT_HIDH_DISCONNECT_CFM
    { CSR_BT_HIDH_DISCONNECT_CFM, CsrBtHidhDisconnectCfmSizeof, CsrBtHidhDisconnectCfmSer, CsrBtHidhDisconnectCfmDes, CsrBtHidhDisconnectCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HIDH_DISCONNECT_CFM */
#ifndef EXCLUDE_CSR_BT_HIDH_DISCONNECT_IND
    { CSR_BT_HIDH_DISCONNECT_IND, CsrBtHidhDisconnectIndSizeof, CsrBtHidhDisconnectIndSer, CsrBtHidhDisconnectIndDes, CsrBtHidhDisconnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_HIDH_DISCONNECT_IND */
#ifndef EXCLUDE_CSR_BT_HIDH_DISCONNECT_REQ
    { CSR_BT_HIDH_DISCONNECT_REQ, CsrBtHidhDisconnectReqSizeof, CsrBtHidhDisconnectReqSer, CsrBtHidhDisconnectReqDes, CsrBtHidhDisconnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_HIDH_DISCONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_HIDH_GET_IDLE_REQ
    { CSR_BT_HIDH_GET_IDLE_REQ, CsrBtHidhGetIdleReqSizeof, CsrBtHidhGetIdleReqSer, CsrBtHidhGetIdleReqDes, CsrBtHidhGetIdleReqSerFree },
#endif /* EXCLUDE_CSR_BT_HIDH_GET_IDLE_REQ */
#ifndef EXCLUDE_CSR_BT_HIDH_GET_PROTOCOL_REQ
    { CSR_BT_HIDH_GET_PROTOCOL_REQ, CsrBtHidhGetProtocolReqSizeof, CsrBtHidhGetProtocolReqSer, CsrBtHidhGetProtocolReqDes, CsrBtHidhGetProtocolReqSerFree },
#endif /* EXCLUDE_CSR_BT_HIDH_GET_PROTOCOL_REQ */
#ifndef EXCLUDE_CSR_BT_HIDH_GET_REPORT_REQ
    { CSR_BT_HIDH_GET_REPORT_REQ, CsrBtHidhGetReportReqSizeof, CsrBtHidhGetReportReqSer, CsrBtHidhGetReportReqDes, CsrBtHidhGetReportReqSerFree },
#endif /* EXCLUDE_CSR_BT_HIDH_GET_REPORT_REQ */
#ifndef EXCLUDE_CSR_BT_HIDH_HANDSHAKE_IND
    { CSR_BT_HIDH_HANDSHAKE_IND, CsrBtHidhHandshakeIndSizeof, CsrBtHidhHandshakeIndSer, CsrBtHidhHandshakeIndDes, CsrBtHidhHandshakeIndSerFree },
#endif /* EXCLUDE_CSR_BT_HIDH_HANDSHAKE_IND */
#ifndef EXCLUDE_CSR_BT_HIDH_REGISTER_USER_CFM
    { CSR_BT_HIDH_REGISTER_USER_CFM, CsrBtHidhRegisterUserCfmSizeof, CsrBtHidhRegisterUserCfmSer, CsrBtHidhRegisterUserCfmDes, CsrBtHidhRegisterUserCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HIDH_REGISTER_USER_CFM */
#ifndef EXCLUDE_CSR_BT_HIDH_REGISTER_USER_REQ
    { CSR_BT_HIDH_REGISTER_USER_REQ, CsrBtHidhRegisterUserReqSizeof, CsrBtHidhRegisterUserReqSer, CsrBtHidhRegisterUserReqDes, CsrBtHidhRegisterUserReqSerFree },
#endif /* EXCLUDE_CSR_BT_HIDH_REGISTER_USER_REQ */
#ifndef EXCLUDE_CSR_BT_HIDH_SECURITY_IN_CFM
    { CSR_BT_HIDH_SECURITY_IN_CFM, CsrBtHidhSecurityInCfmSizeof, CsrBtHidhSecurityInCfmSer, CsrBtHidhSecurityInCfmDes, CsrBtHidhSecurityInCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HIDH_SECURITY_IN_CFM */
#ifndef EXCLUDE_CSR_BT_HIDH_SECURITY_IN_REQ
    { CSR_BT_HIDH_SECURITY_IN_REQ, CsrBtHidhSecurityInReqSizeof, CsrBtHidhSecurityInReqSer, CsrBtHidhSecurityInReqDes, CsrBtHidhSecurityInReqSerFree },
#endif /* EXCLUDE_CSR_BT_HIDH_SECURITY_IN_REQ */
#ifndef EXCLUDE_CSR_BT_HIDH_SECURITY_OUT_CFM
    { CSR_BT_HIDH_SECURITY_OUT_CFM, CsrBtHidhSecurityOutCfmSizeof, CsrBtHidhSecurityOutCfmSer, CsrBtHidhSecurityOutCfmDes, CsrBtHidhSecurityOutCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HIDH_SECURITY_OUT_CFM */
#ifndef EXCLUDE_CSR_BT_HIDH_SECURITY_OUT_REQ
    { CSR_BT_HIDH_SECURITY_OUT_REQ, CsrBtHidhSecurityOutReqSizeof, CsrBtHidhSecurityOutReqSer, CsrBtHidhSecurityOutReqDes, CsrBtHidhSecurityOutReqSerFree },
#endif /* EXCLUDE_CSR_BT_HIDH_SECURITY_OUT_REQ */
#ifndef EXCLUDE_CSR_BT_HIDH_SET_IDLE_REQ
    { CSR_BT_HIDH_SET_IDLE_REQ, CsrBtHidhSetIdleReqSizeof, CsrBtHidhSetIdleReqSer, CsrBtHidhSetIdleReqDes, CsrBtHidhSetIdleReqSerFree },
#endif /* EXCLUDE_CSR_BT_HIDH_SET_IDLE_REQ */
#ifndef EXCLUDE_CSR_BT_HIDH_SET_PROTOCOL_REQ
    { CSR_BT_HIDH_SET_PROTOCOL_REQ, CsrBtHidhSetProtocolReqSizeof, CsrBtHidhSetProtocolReqSer, CsrBtHidhSetProtocolReqDes, CsrBtHidhSetProtocolReqSerFree },
#endif /* EXCLUDE_CSR_BT_HIDH_SET_PROTOCOL_REQ */
#ifndef EXCLUDE_CSR_BT_HIDH_SET_REPORT_REQ
    { CSR_BT_HIDH_SET_REPORT_REQ, CsrBtHidhSetReportReqSizeof, CsrBtHidhSetReportReqSer, CsrBtHidhSetReportReqDes, CsrBtHidhSetReportReqSerFree },
#endif /* EXCLUDE_CSR_BT_HIDH_SET_REPORT_REQ */
#ifndef EXCLUDE_CSR_BT_HIDH_STATUS_IND
    { CSR_BT_HIDH_STATUS_IND, CsrBtHidhStatusIndSizeof, CsrBtHidhStatusIndSer, CsrBtHidhStatusIndDes, CsrBtHidhStatusIndSerFree },
#endif /* EXCLUDE_CSR_BT_HIDH_STATUS_IND */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtHidhConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_HIDH_PRIM, csr_bt_hidh_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_hidh_conv_info = {
    CSR_BT_HIDH_PRIM,
    "CSR_BT_HIDH_PRIM",
    csr_bt_hidh_conv_lut
};

CsrLogPrimitiveInformation* CsrBtHidhTechInfoGet(void)
{
    return &csr_bt_hidh_conv_info;
}
#endif
#endif

