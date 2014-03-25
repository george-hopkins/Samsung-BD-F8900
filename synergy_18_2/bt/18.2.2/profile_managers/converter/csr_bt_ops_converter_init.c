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
#include "csr_bt_ops_converter_init.h"
#ifndef EXCLUDE_CSR_BT_OPS_MODULE
#include "csr_bt_ops_serialize.h"
#include "csr_bt_ops_prim.h"

static CsrMsgConvMsgEntry csr_bt_ops_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_OPS_ABORT_IND
    { CSR_BT_OPS_ABORT_IND, CsrBtOpsAbortIndSizeof, CsrBtOpsAbortIndSer, CsrBtOpsAbortIndDes, CsrBtOpsAbortIndSerFree },
#endif /* EXCLUDE_CSR_BT_OPS_ABORT_IND */
#ifndef EXCLUDE_CSR_BT_OPS_ACTIVATE_REQ
    { CSR_BT_OPS_ACTIVATE_REQ, CsrBtOpsActivateReqSizeof, CsrBtOpsActivateReqSer, CsrBtOpsActivateReqDes, CsrBtOpsActivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_OPS_ACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_OPS_AUTHENTICATE_IND
    { CSR_BT_OPS_AUTHENTICATE_IND, CsrBtOpsAuthenticateIndSizeof, CsrBtOpsAuthenticateIndSer, CsrBtOpsAuthenticateIndDes, CsrBtOpsAuthenticateIndSerFree },
#endif /* EXCLUDE_CSR_BT_OPS_AUTHENTICATE_IND */
#ifndef EXCLUDE_CSR_BT_OPS_AUTHENTICATE_RES
    { CSR_BT_OPS_AUTHENTICATE_RES, CsrBtOpsAuthenticateResSizeof, CsrBtOpsAuthenticateResSer, CsrBtOpsAuthenticateResDes, CsrBtOpsAuthenticateResSerFree },
#endif /* EXCLUDE_CSR_BT_OPS_AUTHENTICATE_RES */
#ifndef EXCLUDE_CSR_BT_OPS_CONNECT_IND
    { CSR_BT_OPS_CONNECT_IND, CsrBtOpsConnectIndSizeof, CsrBtOpsConnectIndSer, CsrBtOpsConnectIndDes, CsrBtOpsConnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_OPS_CONNECT_IND */
#ifndef EXCLUDE_CSR_BT_OPS_CONNECT_RES
    { CSR_BT_OPS_CONNECT_RES, CsrBtOpsConnectResSizeof, CsrBtOpsConnectResSer, CsrBtOpsConnectResDes, CsrBtOpsConnectResSerFree },
#endif /* EXCLUDE_CSR_BT_OPS_CONNECT_RES */
#ifndef EXCLUDE_CSR_BT_OPS_DEACTIVATE_CFM
    { CSR_BT_OPS_DEACTIVATE_CFM, CsrBtOpsDeactivateCfmSizeof, CsrBtOpsDeactivateCfmSer, CsrBtOpsDeactivateCfmDes, CsrBtOpsDeactivateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_OPS_DEACTIVATE_CFM */
#ifndef EXCLUDE_CSR_BT_OPS_DEACTIVATE_REQ
    { CSR_BT_OPS_DEACTIVATE_REQ, CsrBtOpsDeactivateReqSizeof, CsrBtOpsDeactivateReqSer, CsrBtOpsDeactivateReqDes, CsrBtOpsDeactivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_OPS_DEACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_OPS_DISCONNECT_IND
    { CSR_BT_OPS_DISCONNECT_IND, CsrBtOpsDisconnectIndSizeof, CsrBtOpsDisconnectIndSer, CsrBtOpsDisconnectIndDes, CsrBtOpsDisconnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_OPS_DISCONNECT_IND */
#ifndef EXCLUDE_CSR_BT_OPS_GET_IND
    { CSR_BT_OPS_GET_IND, CsrBtOpsGetIndSizeof, CsrBtOpsGetIndSer, CsrBtOpsGetIndDes, CsrBtOpsGetIndSerFree },
#endif /* EXCLUDE_CSR_BT_OPS_GET_IND */
#ifndef EXCLUDE_CSR_BT_OPS_GET_NEXT_IND
    { CSR_BT_OPS_GET_NEXT_IND, CsrBtOpsGetNextIndSizeof, CsrBtOpsGetNextIndSer, CsrBtOpsGetNextIndDes, CsrBtOpsGetNextIndSerFree },
#endif /* EXCLUDE_CSR_BT_OPS_GET_NEXT_IND */
#ifndef EXCLUDE_CSR_BT_OPS_GET_NEXT_RES
    { CSR_BT_OPS_GET_NEXT_RES, CsrBtOpsGetNextResSizeof, CsrBtOpsGetNextResSer, CsrBtOpsGetNextResDes, CsrBtOpsGetNextResSerFree },
#endif /* EXCLUDE_CSR_BT_OPS_GET_NEXT_RES */
#ifndef EXCLUDE_CSR_BT_OPS_GET_RES
    { CSR_BT_OPS_GET_RES, CsrBtOpsGetResSizeof, CsrBtOpsGetResSer, CsrBtOpsGetResDes, CsrBtOpsGetResSerFree },
#endif /* EXCLUDE_CSR_BT_OPS_GET_RES */
#ifndef EXCLUDE_CSR_BT_OPS_PUT_IND
    { CSR_BT_OPS_PUT_IND, CsrBtOpsPutIndSizeof, CsrBtOpsPutIndSer, CsrBtOpsPutIndDes, CsrBtOpsPutIndSerFree },
#endif /* EXCLUDE_CSR_BT_OPS_PUT_IND */
#ifndef EXCLUDE_CSR_BT_OPS_PUT_NEXT_IND
    { CSR_BT_OPS_PUT_NEXT_IND, CsrBtOpsPutNextIndSizeof, CsrBtOpsPutNextIndSer, CsrBtOpsPutNextIndDes, CsrBtOpsPutNextIndSerFree },
#endif /* EXCLUDE_CSR_BT_OPS_PUT_NEXT_IND */
#ifndef EXCLUDE_CSR_BT_OPS_PUT_NEXT_RES
    { CSR_BT_OPS_PUT_NEXT_RES, CsrBtOpsPutNextResSizeof, CsrBtOpsPutNextResSer, CsrBtOpsPutNextResDes, CsrBtOpsPutNextResSerFree },
#endif /* EXCLUDE_CSR_BT_OPS_PUT_NEXT_RES */
#ifndef EXCLUDE_CSR_BT_OPS_PUT_RES
    { CSR_BT_OPS_PUT_RES, CsrBtOpsPutResSizeof, CsrBtOpsPutResSer, CsrBtOpsPutResDes, CsrBtOpsPutResSerFree },
#endif /* EXCLUDE_CSR_BT_OPS_PUT_RES */
#ifndef EXCLUDE_CSR_BT_OPS_SECURITY_IN_CFM
    { CSR_BT_OPS_SECURITY_IN_CFM, CsrBtOpsSecurityInCfmSizeof, CsrBtOpsSecurityInCfmSer, CsrBtOpsSecurityInCfmDes, CsrBtOpsSecurityInCfmSerFree },
#endif /* EXCLUDE_CSR_BT_OPS_SECURITY_IN_CFM */
#ifndef EXCLUDE_CSR_BT_OPS_SECURITY_IN_REQ
    { CSR_BT_OPS_SECURITY_IN_REQ, CsrBtOpsSecurityInReqSizeof, CsrBtOpsSecurityInReqSer, CsrBtOpsSecurityInReqDes, CsrBtOpsSecurityInReqSerFree },
#endif /* EXCLUDE_CSR_BT_OPS_SECURITY_IN_REQ */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtOpsConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_OPS_PRIM, csr_bt_ops_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_ops_conv_info = {
    CSR_BT_OPS_PRIM,
    "CSR_BT_OPS_PRIM",
    csr_bt_ops_conv_lut
};

CsrLogPrimitiveInformation* CsrBtOpsTechInfoGet(void)
{
    return &csr_bt_ops_conv_info;
}
#endif
#endif

