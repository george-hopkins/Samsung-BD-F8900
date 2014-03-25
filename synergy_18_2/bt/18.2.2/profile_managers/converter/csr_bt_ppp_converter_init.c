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
#include "csr_bt_ppp_converter_init.h"
#ifndef EXCLUDE_CSR_BT_PPP_MODULE
#include "csr_bt_ppp_serialize.h"
#include "csr_bt_ppp_prim.h"

static CsrMsgConvMsgEntry csr_bt_ppp_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_PPP_AUTHENTICATE_IND
    { CSR_BT_PPP_AUTHENTICATE_IND, CsrBtPppAuthenticateIndSizeof, CsrBtPppAuthenticateIndSer, CsrBtPppAuthenticateIndDes, CsrBtPppAuthenticateIndSerFree },
#endif /* EXCLUDE_CSR_BT_PPP_AUTHENTICATE_IND */
#ifndef EXCLUDE_CSR_BT_PPP_AUTHENTICATE_RES
    { CSR_BT_PPP_AUTHENTICATE_RES, CsrBtPppAuthenticateResSizeof, CsrBtPppAuthenticateResSer, CsrBtPppAuthenticateResDes, CsrBtPppAuthenticateResSerFree },
#endif /* EXCLUDE_CSR_BT_PPP_AUTHENTICATE_RES */
#ifndef EXCLUDE_CSR_BT_PPP_CONNECT_CFM
    { CSR_BT_PPP_CONNECT_CFM, CsrBtPppConnectCfmSizeof, CsrBtPppConnectCfmSer, CsrBtPppConnectCfmDes, CsrBtPppConnectCfmSerFree },
#endif /* EXCLUDE_CSR_BT_PPP_CONNECT_CFM */
#ifndef EXCLUDE_CSR_BT_PPP_CONNECT_REQ
    { CSR_BT_PPP_CONNECT_REQ, CsrBtPppConnectReqSizeof, CsrBtPppConnectReqSer, CsrBtPppConnectReqDes, CsrBtPppConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_PPP_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_PPP_DATA_IND
    { CSR_BT_PPP_DATA_IND, CsrBtPppDataIndSizeof, CsrBtPppDataIndSer, CsrBtPppDataIndDes, CsrBtPppDataIndSerFree },
#endif /* EXCLUDE_CSR_BT_PPP_DATA_IND */
#ifndef EXCLUDE_CSR_BT_PPP_DATA_REQ
    { CSR_BT_PPP_DATA_REQ, CsrBtPppDataReqSizeof, CsrBtPppDataReqSer, CsrBtPppDataReqDes, CsrBtPppDataReqSerFree },
#endif /* EXCLUDE_CSR_BT_PPP_DATA_REQ */
#ifndef EXCLUDE_CSR_BT_PPP_DISCONNECT_IND
    { CSR_BT_PPP_DISCONNECT_IND, CsrBtPppDisconnectIndSizeof, CsrBtPppDisconnectIndSer, CsrBtPppDisconnectIndDes, CsrBtPppDisconnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_PPP_DISCONNECT_IND */
#ifndef EXCLUDE_CSR_BT_PPP_DISCONNECT_REQ
    { CSR_BT_PPP_DISCONNECT_REQ, CsrBtPppDisconnectReqSizeof, CsrBtPppDisconnectReqSer, CsrBtPppDisconnectReqDes, CsrBtPppDisconnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_PPP_DISCONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_PPP_DOWN_IND
    { CSR_BT_PPP_DOWN_IND, CsrBtPppDownIndSizeof, CsrBtPppDownIndSer, CsrBtPppDownIndDes, CsrBtPppDownIndSerFree },
#endif /* EXCLUDE_CSR_BT_PPP_DOWN_IND */
#ifndef EXCLUDE_CSR_BT_PPP_NCP_IND
    { CSR_BT_PPP_NCP_IND, CsrBtPppNcpIndSizeof, CsrBtPppNcpIndSer, CsrBtPppNcpIndDes, CsrBtPppNcpIndSerFree },
#endif /* EXCLUDE_CSR_BT_PPP_NCP_IND */
#ifndef EXCLUDE_CSR_BT_PPP_NCP_REQ
    { CSR_BT_PPP_NCP_REQ, CsrBtPppNcpReqSizeof, CsrBtPppNcpReqSer, CsrBtPppNcpReqDes, CsrBtPppNcpReqSerFree },
#endif /* EXCLUDE_CSR_BT_PPP_NCP_REQ */
#ifndef EXCLUDE_CSR_BT_PPP_REGISTER_NETWORK_PROTOCOL_CFM
    { CSR_BT_PPP_REGISTER_NETWORK_PROTOCOL_CFM, CsrBtPppRegisterNetworkProtocolCfmSizeof, CsrBtPppRegisterNetworkProtocolCfmSer, CsrBtPppRegisterNetworkProtocolCfmDes, CsrBtPppRegisterNetworkProtocolCfmSerFree },
#endif /* EXCLUDE_CSR_BT_PPP_REGISTER_NETWORK_PROTOCOL_CFM */
#ifndef EXCLUDE_CSR_BT_PPP_REGISTER_NETWORK_PROTOCOL_REQ
    { CSR_BT_PPP_REGISTER_NETWORK_PROTOCOL_REQ, CsrBtPppRegisterNetworkProtocolReqSizeof, CsrBtPppRegisterNetworkProtocolReqSer, CsrBtPppRegisterNetworkProtocolReqDes, CsrBtPppRegisterNetworkProtocolReqSerFree },
#endif /* EXCLUDE_CSR_BT_PPP_REGISTER_NETWORK_PROTOCOL_REQ */
#ifndef EXCLUDE_CSR_BT_PPP_UP_IND
    { CSR_BT_PPP_UP_IND, CsrBtPppUpIndSizeof, CsrBtPppUpIndSer, CsrBtPppUpIndDes, CsrBtPppUpIndSerFree },
#endif /* EXCLUDE_CSR_BT_PPP_UP_IND */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtPppConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_PPP_PRIM, csr_bt_ppp_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_ppp_conv_info = {
    CSR_BT_PPP_PRIM,
    "CSR_BT_PPP_PRIM",
    csr_bt_ppp_conv_lut
};

CsrLogPrimitiveInformation* CsrBtPppTechInfoGet(void)
{
    return &csr_bt_ppp_conv_info;
}
#endif
#endif

