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
#include "csr_bt_smlc_converter_init.h"
#ifndef EXCLUDE_CSR_BT_SMLC_MODULE
#include "csr_bt_smlc_serialize.h"
#include "csr_bt_smlc_prim.h"

static CsrMsgConvMsgEntry csr_bt_smlc_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_SMLC_ABORT_CFM
    { CSR_BT_SMLC_ABORT_CFM, CsrBtSmlcAbortCfmSizeof, CsrBtSmlcAbortCfmSer, CsrBtSmlcAbortCfmDes, CsrBtSmlcAbortCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SMLC_ABORT_CFM */
#ifndef EXCLUDE_CSR_BT_SMLC_ABORT_REQ
    { CSR_BT_SMLC_ABORT_REQ, CsrBtSmlcAbortReqSizeof, CsrBtSmlcAbortReqSer, CsrBtSmlcAbortReqDes, CsrBtSmlcAbortReqSerFree },
#endif /* EXCLUDE_CSR_BT_SMLC_ABORT_REQ */
#ifndef EXCLUDE_CSR_BT_SMLC_ACTIVATE_CFM
    { CSR_BT_SMLC_ACTIVATE_CFM, CsrBtSmlcActivateCfmSizeof, CsrBtSmlcActivateCfmSer, CsrBtSmlcActivateCfmDes, CsrBtSmlcActivateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SMLC_ACTIVATE_CFM */
#ifndef EXCLUDE_CSR_BT_SMLC_ACTIVATE_REQ
    { CSR_BT_SMLC_ACTIVATE_REQ, CsrBtSmlcActivateReqSizeof, CsrBtSmlcActivateReqSer, CsrBtSmlcActivateReqDes, CsrBtSmlcActivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_SMLC_ACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_SMLC_AUTHENTICATE_IND
    { CSR_BT_SMLC_AUTHENTICATE_IND, CsrBtSmlcAuthenticateIndSizeof, CsrBtSmlcAuthenticateIndSer, CsrBtSmlcAuthenticateIndDes, CsrBtSmlcAuthenticateIndSerFree },
#endif /* EXCLUDE_CSR_BT_SMLC_AUTHENTICATE_IND */
#ifndef EXCLUDE_CSR_BT_SMLC_AUTHENTICATE_RES
    { CSR_BT_SMLC_AUTHENTICATE_RES, CsrBtSmlcAuthenticateResSizeof, CsrBtSmlcAuthenticateResSer, CsrBtSmlcAuthenticateResDes, CsrBtSmlcAuthenticateResSerFree },
#endif /* EXCLUDE_CSR_BT_SMLC_AUTHENTICATE_RES */
#ifndef EXCLUDE_CSR_BT_SMLC_CANCEL_CONNECT_REQ
    { CSR_BT_SMLC_CANCEL_CONNECT_REQ, CsrBtSmlcCancelConnectReqSizeof, CsrBtSmlcCancelConnectReqSer, CsrBtSmlcCancelConnectReqDes, CsrBtSmlcCancelConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_SMLC_CANCEL_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_SMLC_CONNECT_CFM
    { CSR_BT_SMLC_CONNECT_CFM, CsrBtSmlcConnectCfmSizeof, CsrBtSmlcConnectCfmSer, CsrBtSmlcConnectCfmDes, CsrBtSmlcConnectCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SMLC_CONNECT_CFM */
#ifndef EXCLUDE_CSR_BT_SMLC_CONNECT_REQ
    { CSR_BT_SMLC_CONNECT_REQ, CsrBtSmlcConnectReqSizeof, CsrBtSmlcConnectReqSer, CsrBtSmlcConnectReqDes, CsrBtSmlcConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_SMLC_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_SMLC_DEACTIVATE_CFM
    { CSR_BT_SMLC_DEACTIVATE_CFM, CsrBtSmlcDeactivateCfmSizeof, CsrBtSmlcDeactivateCfmSer, CsrBtSmlcDeactivateCfmDes, CsrBtSmlcDeactivateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SMLC_DEACTIVATE_CFM */
#ifndef EXCLUDE_CSR_BT_SMLC_DEACTIVATE_REQ
    { CSR_BT_SMLC_DEACTIVATE_REQ, CsrBtSmlcDeactivateReqSizeof, CsrBtSmlcDeactivateReqSer, CsrBtSmlcDeactivateReqDes, CsrBtSmlcDeactivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_SMLC_DEACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_SMLC_DISCONNECT_IND
    { CSR_BT_SMLC_DISCONNECT_IND, CsrBtSmlcDisconnectIndSizeof, CsrBtSmlcDisconnectIndSer, CsrBtSmlcDisconnectIndDes, CsrBtSmlcDisconnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_SMLC_DISCONNECT_IND */
#ifndef EXCLUDE_CSR_BT_SMLC_DISCONNECT_REQ
    { CSR_BT_SMLC_DISCONNECT_REQ, CsrBtSmlcDisconnectReqSizeof, CsrBtSmlcDisconnectReqSer, CsrBtSmlcDisconnectReqDes, CsrBtSmlcDisconnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_SMLC_DISCONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_SMLC_GET_NEXT_CHUNK_SML_MSG_OBJ_CFM
    { CSR_BT_SMLC_GET_NEXT_CHUNK_SML_MSG_OBJ_CFM, CsrBtSmlcGetNextChunkSmlMsgObjCfmSizeof, CsrBtSmlcGetNextChunkSmlMsgObjCfmSer, CsrBtSmlcGetNextChunkSmlMsgObjCfmDes, CsrBtSmlcGetNextChunkSmlMsgObjCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SMLC_GET_NEXT_CHUNK_SML_MSG_OBJ_CFM */
#ifndef EXCLUDE_CSR_BT_SMLC_GET_NEXT_CHUNK_SML_MSG_OBJ_REQ
    { CSR_BT_SMLC_GET_NEXT_CHUNK_SML_MSG_OBJ_REQ, CsrBtSmlcGetNextChunkSmlMsgObjReqSizeof, CsrBtSmlcGetNextChunkSmlMsgObjReqSer, CsrBtSmlcGetNextChunkSmlMsgObjReqDes, CsrBtSmlcGetNextChunkSmlMsgObjReqSerFree },
#endif /* EXCLUDE_CSR_BT_SMLC_GET_NEXT_CHUNK_SML_MSG_OBJ_REQ */
#ifndef EXCLUDE_CSR_BT_SMLC_GET_SML_MSG_OBJ_CFM
    { CSR_BT_SMLC_GET_SML_MSG_OBJ_CFM, CsrBtSmlcGetSmlMsgObjCfmSizeof, CsrBtSmlcGetSmlMsgObjCfmSer, CsrBtSmlcGetSmlMsgObjCfmDes, CsrBtSmlcGetSmlMsgObjCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SMLC_GET_SML_MSG_OBJ_CFM */
#ifndef EXCLUDE_CSR_BT_SMLC_GET_SML_MSG_OBJ_REQ
    { CSR_BT_SMLC_GET_SML_MSG_OBJ_REQ, CsrBtSmlcGetSmlMsgObjReqSizeof, CsrBtSmlcGetSmlMsgObjReqSer, CsrBtSmlcGetSmlMsgObjReqDes, CsrBtSmlcGetSmlMsgObjReqSerFree },
#endif /* EXCLUDE_CSR_BT_SMLC_GET_SML_MSG_OBJ_REQ */
#ifndef EXCLUDE_CSR_BT_SMLC_PUT_NEXT_CHUNK_SML_MSG_OBJ_CFM
    { CSR_BT_SMLC_PUT_NEXT_CHUNK_SML_MSG_OBJ_CFM, CsrBtSmlcPutNextChunkSmlMsgObjCfmSizeof, CsrBtSmlcPutNextChunkSmlMsgObjCfmSer, CsrBtSmlcPutNextChunkSmlMsgObjCfmDes, CsrBtSmlcPutNextChunkSmlMsgObjCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SMLC_PUT_NEXT_CHUNK_SML_MSG_OBJ_CFM */
#ifndef EXCLUDE_CSR_BT_SMLC_PUT_NEXT_CHUNK_SML_MSG_OBJ_REQ
    { CSR_BT_SMLC_PUT_NEXT_CHUNK_SML_MSG_OBJ_REQ, CsrBtSmlcPutNextChunkSmlMsgObjReqSizeof, CsrBtSmlcPutNextChunkSmlMsgObjReqSer, CsrBtSmlcPutNextChunkSmlMsgObjReqDes, CsrBtSmlcPutNextChunkSmlMsgObjReqSerFree },
#endif /* EXCLUDE_CSR_BT_SMLC_PUT_NEXT_CHUNK_SML_MSG_OBJ_REQ */
#ifndef EXCLUDE_CSR_BT_SMLC_PUT_SML_MSG_OBJ_CFM
    { CSR_BT_SMLC_PUT_SML_MSG_OBJ_CFM, CsrBtSmlcPutSmlMsgObjCfmSizeof, CsrBtSmlcPutSmlMsgObjCfmSer, CsrBtSmlcPutSmlMsgObjCfmDes, CsrBtSmlcPutSmlMsgObjCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SMLC_PUT_SML_MSG_OBJ_CFM */
#ifndef EXCLUDE_CSR_BT_SMLC_PUT_SML_MSG_OBJ_REQ
    { CSR_BT_SMLC_PUT_SML_MSG_OBJ_REQ, CsrBtSmlcPutSmlMsgObjReqSizeof, CsrBtSmlcPutSmlMsgObjReqSer, CsrBtSmlcPutSmlMsgObjReqDes, CsrBtSmlcPutSmlMsgObjReqSerFree },
#endif /* EXCLUDE_CSR_BT_SMLC_PUT_SML_MSG_OBJ_REQ */
#ifndef EXCLUDE_CSR_BT_SMLC_SECURITY_IN_CFM
    { CSR_BT_SMLC_SECURITY_IN_CFM, CsrBtSmlcSecurityInCfmSizeof, CsrBtSmlcSecurityInCfmSer, CsrBtSmlcSecurityInCfmDes, CsrBtSmlcSecurityInCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SMLC_SECURITY_IN_CFM */
#ifndef EXCLUDE_CSR_BT_SMLC_SECURITY_IN_REQ
    { CSR_BT_SMLC_SECURITY_IN_REQ, CsrBtSmlcSecurityInReqSizeof, CsrBtSmlcSecurityInReqSer, CsrBtSmlcSecurityInReqDes, CsrBtSmlcSecurityInReqSerFree },
#endif /* EXCLUDE_CSR_BT_SMLC_SECURITY_IN_REQ */
#ifndef EXCLUDE_CSR_BT_SMLC_SECURITY_OUT_CFM
    { CSR_BT_SMLC_SECURITY_OUT_CFM, CsrBtSmlcSecurityOutCfmSizeof, CsrBtSmlcSecurityOutCfmSer, CsrBtSmlcSecurityOutCfmDes, CsrBtSmlcSecurityOutCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SMLC_SECURITY_OUT_CFM */
#ifndef EXCLUDE_CSR_BT_SMLC_SECURITY_OUT_REQ
    { CSR_BT_SMLC_SECURITY_OUT_REQ, CsrBtSmlcSecurityOutReqSizeof, CsrBtSmlcSecurityOutReqSer, CsrBtSmlcSecurityOutReqDes, CsrBtSmlcSecurityOutReqSerFree },
#endif /* EXCLUDE_CSR_BT_SMLC_SECURITY_OUT_REQ */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtSmlcConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_SMLC_PRIM, csr_bt_smlc_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_smlc_conv_info = {
    CSR_BT_SMLC_PRIM,
    "CSR_BT_SMLC_PRIM",
    csr_bt_smlc_conv_lut
};

CsrLogPrimitiveInformation* CsrBtSmlcTechInfoGet(void)
{
    return &csr_bt_smlc_conv_info;
}
#endif
#endif

