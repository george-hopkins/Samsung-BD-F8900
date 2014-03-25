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
#include "csr_bt_smls_converter_init.h"
#ifndef EXCLUDE_CSR_BT_SMLS_MODULE
#include "csr_bt_smls_serialize.h"
#include "csr_bt_smls_prim.h"

static CsrMsgConvMsgEntry csr_bt_smls_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_SMLS_ABORT_IND
    { CSR_BT_SMLS_ABORT_IND, CsrBtSmlsAbortIndSizeof, CsrBtSmlsAbortIndSer, CsrBtSmlsAbortIndDes, CsrBtSmlsAbortIndSerFree },
#endif /* EXCLUDE_CSR_BT_SMLS_ABORT_IND */
#ifndef EXCLUDE_CSR_BT_SMLS_ACTIVATE_CFM
    { CSR_BT_SMLS_ACTIVATE_CFM, CsrBtSmlsActivateCfmSizeof, CsrBtSmlsActivateCfmSer, CsrBtSmlsActivateCfmDes, CsrBtSmlsActivateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SMLS_ACTIVATE_CFM */
#ifndef EXCLUDE_CSR_BT_SMLS_ACTIVATE_REQ
    { CSR_BT_SMLS_ACTIVATE_REQ, CsrBtSmlsActivateReqSizeof, CsrBtSmlsActivateReqSer, CsrBtSmlsActivateReqDes, CsrBtSmlsActivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_SMLS_ACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_CFM
    { CSR_BT_SMLS_AUTHENTICATE_CFM, CsrBtSmlsAuthenticateCfmSizeof, CsrBtSmlsAuthenticateCfmSer, CsrBtSmlsAuthenticateCfmDes, CsrBtSmlsAuthenticateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_CFM */
#ifndef EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_IND
    { CSR_BT_SMLS_AUTHENTICATE_IND, CsrBtSmlsAuthenticateIndSizeof, CsrBtSmlsAuthenticateIndSer, CsrBtSmlsAuthenticateIndDes, CsrBtSmlsAuthenticateIndSerFree },
#endif /* EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_IND */
#ifndef EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_REQ
    { CSR_BT_SMLS_AUTHENTICATE_REQ, CsrBtSmlsAuthenticateReqSizeof, CsrBtSmlsAuthenticateReqSer, CsrBtSmlsAuthenticateReqDes, CsrBtSmlsAuthenticateReqSerFree },
#endif /* EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_REQ */
#ifndef EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_RES
    { CSR_BT_SMLS_AUTHENTICATE_RES, CsrBtSmlsAuthenticateResSizeof, CsrBtSmlsAuthenticateResSer, CsrBtSmlsAuthenticateResDes, CsrBtSmlsAuthenticateResSerFree },
#endif /* EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_RES */
#ifndef EXCLUDE_CSR_BT_SMLS_CONNECT_IND
    { CSR_BT_SMLS_CONNECT_IND, CsrBtSmlsConnectIndSizeof, CsrBtSmlsConnectIndSer, CsrBtSmlsConnectIndDes, CsrBtSmlsConnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_SMLS_CONNECT_IND */
#ifndef EXCLUDE_CSR_BT_SMLS_CONNECT_RES
    { CSR_BT_SMLS_CONNECT_RES, CsrBtSmlsConnectResSizeof, CsrBtSmlsConnectResSer, CsrBtSmlsConnectResDes, CsrBtSmlsConnectResSerFree },
#endif /* EXCLUDE_CSR_BT_SMLS_CONNECT_RES */
#ifndef EXCLUDE_CSR_BT_SMLS_DEACTIVATE_CFM
    { CSR_BT_SMLS_DEACTIVATE_CFM, CsrBtSmlsDeactivateCfmSizeof, CsrBtSmlsDeactivateCfmSer, CsrBtSmlsDeactivateCfmDes, CsrBtSmlsDeactivateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SMLS_DEACTIVATE_CFM */
#ifndef EXCLUDE_CSR_BT_SMLS_DEACTIVATE_REQ
    { CSR_BT_SMLS_DEACTIVATE_REQ, CsrBtSmlsDeactivateReqSizeof, CsrBtSmlsDeactivateReqSer, CsrBtSmlsDeactivateReqDes, CsrBtSmlsDeactivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_SMLS_DEACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_SMLS_DISCONNECT_IND
    { CSR_BT_SMLS_DISCONNECT_IND, CsrBtSmlsDisconnectIndSizeof, CsrBtSmlsDisconnectIndSer, CsrBtSmlsDisconnectIndDes, CsrBtSmlsDisconnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_SMLS_DISCONNECT_IND */
#ifndef EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_IND
    { CSR_BT_SMLS_GET_SML_MSG_OBJ_IND, CsrBtSmlsGetSmlMsgObjIndSizeof, CsrBtSmlsGetSmlMsgObjIndSer, CsrBtSmlsGetSmlMsgObjIndDes, CsrBtSmlsGetSmlMsgObjIndSerFree },
#endif /* EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_IND */
#ifndef EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_NEXT_IND
    { CSR_BT_SMLS_GET_SML_MSG_OBJ_NEXT_IND, CsrBtSmlsGetSmlMsgObjNextIndSizeof, CsrBtSmlsGetSmlMsgObjNextIndSer, CsrBtSmlsGetSmlMsgObjNextIndDes, CsrBtSmlsGetSmlMsgObjNextIndSerFree },
#endif /* EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_NEXT_IND */
#ifndef EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_NEXT_RES
    { CSR_BT_SMLS_GET_SML_MSG_OBJ_NEXT_RES, CsrBtSmlsGetSmlMsgObjNextResSizeof, CsrBtSmlsGetSmlMsgObjNextResSer, CsrBtSmlsGetSmlMsgObjNextResDes, CsrBtSmlsGetSmlMsgObjNextResSerFree },
#endif /* EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_NEXT_RES */
#ifndef EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_RES
    { CSR_BT_SMLS_GET_SML_MSG_OBJ_RES, CsrBtSmlsGetSmlMsgObjResSizeof, CsrBtSmlsGetSmlMsgObjResSer, CsrBtSmlsGetSmlMsgObjResDes, CsrBtSmlsGetSmlMsgObjResSerFree },
#endif /* EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_RES */
#ifndef EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_IND
    { CSR_BT_SMLS_PUT_SML_MSG_OBJ_IND, CsrBtSmlsPutSmlMsgObjIndSizeof, CsrBtSmlsPutSmlMsgObjIndSer, CsrBtSmlsPutSmlMsgObjIndDes, CsrBtSmlsPutSmlMsgObjIndSerFree },
#endif /* EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_IND */
#ifndef EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_NEXT_IND
    { CSR_BT_SMLS_PUT_SML_MSG_OBJ_NEXT_IND, CsrBtSmlsPutSmlMsgObjNextIndSizeof, CsrBtSmlsPutSmlMsgObjNextIndSer, CsrBtSmlsPutSmlMsgObjNextIndDes, CsrBtSmlsPutSmlMsgObjNextIndSerFree },
#endif /* EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_NEXT_IND */
#ifndef EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_NEXT_RES
    { CSR_BT_SMLS_PUT_SML_MSG_OBJ_NEXT_RES, CsrBtSmlsPutSmlMsgObjNextResSizeof, CsrBtSmlsPutSmlMsgObjNextResSer, CsrBtSmlsPutSmlMsgObjNextResDes, CsrBtSmlsPutSmlMsgObjNextResSerFree },
#endif /* EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_NEXT_RES */
#ifndef EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_RES
    { CSR_BT_SMLS_PUT_SML_MSG_OBJ_RES, CsrBtSmlsPutSmlMsgObjResSizeof, CsrBtSmlsPutSmlMsgObjResSer, CsrBtSmlsPutSmlMsgObjResDes, CsrBtSmlsPutSmlMsgObjResSerFree },
#endif /* EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_RES */
#ifndef EXCLUDE_CSR_BT_SMLS_SECURITY_IN_CFM
    { CSR_BT_SMLS_SECURITY_IN_CFM, CsrBtSmlsSecurityInCfmSizeof, CsrBtSmlsSecurityInCfmSer, CsrBtSmlsSecurityInCfmDes, CsrBtSmlsSecurityInCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SMLS_SECURITY_IN_CFM */
#ifndef EXCLUDE_CSR_BT_SMLS_SECURITY_IN_REQ
    { CSR_BT_SMLS_SECURITY_IN_REQ, CsrBtSmlsSecurityInReqSizeof, CsrBtSmlsSecurityInReqSer, CsrBtSmlsSecurityInReqDes, CsrBtSmlsSecurityInReqSerFree },
#endif /* EXCLUDE_CSR_BT_SMLS_SECURITY_IN_REQ */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtSmlsConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_SMLS_PRIM, csr_bt_smls_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_smls_conv_info = {
    CSR_BT_SMLS_PRIM,
    "CSR_BT_SMLS_PRIM",
    csr_bt_smls_conv_lut
};

CsrLogPrimitiveInformation* CsrBtSmlsTechInfoGet(void)
{
    return &csr_bt_smls_conv_info;
}
#endif
#endif

