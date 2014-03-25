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
#include "csr_bt_syncc_converter_init.h"
#ifndef EXCLUDE_CSR_BT_SYNCC_MODULE
#include "csr_bt_syncc_serialize.h"
#include "csr_bt_syncc_prim.h"

static CsrMsgConvMsgEntry csr_bt_syncc_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_SYNCC_ABORT_CFM
    { CSR_BT_SYNCC_ABORT_CFM, CsrBtSynccAbortCfmSizeof, CsrBtSynccAbortCfmSer, CsrBtSynccAbortCfmDes, CsrBtSynccAbortCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCC_ABORT_CFM */
#ifndef EXCLUDE_CSR_BT_SYNCC_ABORT_REQ
    { CSR_BT_SYNCC_ABORT_REQ, CsrBtSynccAbortReqSizeof, CsrBtSynccAbortReqSer, CsrBtSynccAbortReqDes, CsrBtSynccAbortReqSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCC_ABORT_REQ */
#ifndef EXCLUDE_CSR_BT_SYNCC_ACTIVATE_SYNC_COMMAND_CFM
    { CSR_BT_SYNCC_ACTIVATE_SYNC_COMMAND_CFM, CsrBtSynccActivateSyncCommandCfmSizeof, CsrBtSynccActivateSyncCommandCfmSer, CsrBtSynccActivateSyncCommandCfmDes, CsrBtSynccActivateSyncCommandCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCC_ACTIVATE_SYNC_COMMAND_CFM */
#ifndef EXCLUDE_CSR_BT_SYNCC_ACTIVATE_SYNC_COMMAND_REQ
    { CSR_BT_SYNCC_ACTIVATE_SYNC_COMMAND_REQ, CsrBtSynccActivateSyncCommandReqSizeof, CsrBtSynccActivateSyncCommandReqSer, CsrBtSynccActivateSyncCommandReqDes, CsrBtSynccActivateSyncCommandReqSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCC_ACTIVATE_SYNC_COMMAND_REQ */
#ifndef EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_CFM
    { CSR_BT_SYNCC_ADD_OBJECT_CFM, CsrBtSynccAddObjectCfmSizeof, CsrBtSynccAddObjectCfmSer, CsrBtSynccAddObjectCfmDes, CsrBtSynccAddObjectCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_CFM */
#ifndef EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_IND
    { CSR_BT_SYNCC_ADD_OBJECT_IND, CsrBtSynccAddObjectIndSizeof, CsrBtSynccAddObjectIndSer, CsrBtSynccAddObjectIndDes, CsrBtSynccAddObjectIndSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_IND */
#ifndef EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_REQ
    { CSR_BT_SYNCC_ADD_OBJECT_REQ, CsrBtSynccAddObjectReqSizeof, CsrBtSynccAddObjectReqSer, CsrBtSynccAddObjectReqDes, CsrBtSynccAddObjectReqSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_REQ */
#ifndef EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_RES
    { CSR_BT_SYNCC_ADD_OBJECT_RES, CsrBtSynccAddObjectResSizeof, CsrBtSynccAddObjectResSer, CsrBtSynccAddObjectResDes, CsrBtSynccAddObjectResSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_RES */
#ifndef EXCLUDE_CSR_BT_SYNCC_AUTHENTICATE_IND
    { CSR_BT_SYNCC_AUTHENTICATE_IND, CsrBtSynccAuthenticateIndSizeof, CsrBtSynccAuthenticateIndSer, CsrBtSynccAuthenticateIndDes, CsrBtSynccAuthenticateIndSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCC_AUTHENTICATE_IND */
#ifndef EXCLUDE_CSR_BT_SYNCC_AUTHENTICATE_RES
    { CSR_BT_SYNCC_AUTHENTICATE_RES, CsrBtSynccAuthenticateResSizeof, CsrBtSynccAuthenticateResSer, CsrBtSynccAuthenticateResDes, CsrBtSynccAuthenticateResSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCC_AUTHENTICATE_RES */
#ifndef EXCLUDE_CSR_BT_SYNCC_CANCEL_CONNECT_REQ
    { CSR_BT_SYNCC_CANCEL_CONNECT_REQ, CsrBtSynccCancelConnectReqSizeof, CsrBtSynccCancelConnectReqSer, CsrBtSynccCancelConnectReqDes, CsrBtSynccCancelConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCC_CANCEL_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_SYNCC_CONNECT_CFM
    { CSR_BT_SYNCC_CONNECT_CFM, CsrBtSynccConnectCfmSizeof, CsrBtSynccConnectCfmSer, CsrBtSynccConnectCfmDes, CsrBtSynccConnectCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCC_CONNECT_CFM */
#ifndef EXCLUDE_CSR_BT_SYNCC_CONNECT_REQ
    { CSR_BT_SYNCC_CONNECT_REQ, CsrBtSynccConnectReqSizeof, CsrBtSynccConnectReqSer, CsrBtSynccConnectReqDes, CsrBtSynccConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCC_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_SYNCC_DEACTIVATE_SYNC_COMMAND_CFM
    { CSR_BT_SYNCC_DEACTIVATE_SYNC_COMMAND_CFM, CsrBtSynccDeactivateSyncCommandCfmSizeof, CsrBtSynccDeactivateSyncCommandCfmSer, CsrBtSynccDeactivateSyncCommandCfmDes, CsrBtSynccDeactivateSyncCommandCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCC_DEACTIVATE_SYNC_COMMAND_CFM */
#ifndef EXCLUDE_CSR_BT_SYNCC_DEACTIVATE_SYNC_COMMAND_REQ
    { CSR_BT_SYNCC_DEACTIVATE_SYNC_COMMAND_REQ, CsrBtSynccDeactivateSyncCommandReqSizeof, CsrBtSynccDeactivateSyncCommandReqSer, CsrBtSynccDeactivateSyncCommandReqDes, CsrBtSynccDeactivateSyncCommandReqSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCC_DEACTIVATE_SYNC_COMMAND_REQ */
#ifndef EXCLUDE_CSR_BT_SYNCC_DELETE_OBJECT_CFM
    { CSR_BT_SYNCC_DELETE_OBJECT_CFM, CsrBtSynccDeleteObjectCfmSizeof, CsrBtSynccDeleteObjectCfmSer, CsrBtSynccDeleteObjectCfmDes, CsrBtSynccDeleteObjectCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCC_DELETE_OBJECT_CFM */
#ifndef EXCLUDE_CSR_BT_SYNCC_DELETE_OBJECT_REQ
    { CSR_BT_SYNCC_DELETE_OBJECT_REQ, CsrBtSynccDeleteObjectReqSizeof, CsrBtSynccDeleteObjectReqSer, CsrBtSynccDeleteObjectReqDes, CsrBtSynccDeleteObjectReqSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCC_DELETE_OBJECT_REQ */
#ifndef EXCLUDE_CSR_BT_SYNCC_DISCONNECT_IND
    { CSR_BT_SYNCC_DISCONNECT_IND, CsrBtSynccDisconnectIndSizeof, CsrBtSynccDisconnectIndSer, CsrBtSynccDisconnectIndDes, CsrBtSynccDisconnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCC_DISCONNECT_IND */
#ifndef EXCLUDE_CSR_BT_SYNCC_DISCONNECT_REQ
    { CSR_BT_SYNCC_DISCONNECT_REQ, CsrBtSynccDisconnectReqSizeof, CsrBtSynccDisconnectReqSer, CsrBtSynccDisconnectReqDes, CsrBtSynccDisconnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCC_DISCONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_CFM
    { CSR_BT_SYNCC_GET_OBJECT_CFM, CsrBtSynccGetObjectCfmSizeof, CsrBtSynccGetObjectCfmSer, CsrBtSynccGetObjectCfmDes, CsrBtSynccGetObjectCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_CFM */
#ifndef EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_IND
    { CSR_BT_SYNCC_GET_OBJECT_IND, CsrBtSynccGetObjectIndSizeof, CsrBtSynccGetObjectIndSer, CsrBtSynccGetObjectIndDes, CsrBtSynccGetObjectIndSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_IND */
#ifndef EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_REQ
    { CSR_BT_SYNCC_GET_OBJECT_REQ, CsrBtSynccGetObjectReqSizeof, CsrBtSynccGetObjectReqSer, CsrBtSynccGetObjectReqDes, CsrBtSynccGetObjectReqSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_REQ */
#ifndef EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_RES
    { CSR_BT_SYNCC_GET_OBJECT_RES, CsrBtSynccGetObjectResSizeof, CsrBtSynccGetObjectResSer, CsrBtSynccGetObjectResDes, CsrBtSynccGetObjectResSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_RES */
#ifndef EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_CFM
    { CSR_BT_SYNCC_MODIFY_OBJECT_CFM, CsrBtSynccModifyObjectCfmSizeof, CsrBtSynccModifyObjectCfmSer, CsrBtSynccModifyObjectCfmDes, CsrBtSynccModifyObjectCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_CFM */
#ifndef EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_IND
    { CSR_BT_SYNCC_MODIFY_OBJECT_IND, CsrBtSynccModifyObjectIndSizeof, CsrBtSynccModifyObjectIndSer, CsrBtSynccModifyObjectIndDes, CsrBtSynccModifyObjectIndSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_IND */
#ifndef EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_REQ
    { CSR_BT_SYNCC_MODIFY_OBJECT_REQ, CsrBtSynccModifyObjectReqSizeof, CsrBtSynccModifyObjectReqSer, CsrBtSynccModifyObjectReqDes, CsrBtSynccModifyObjectReqSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_REQ */
#ifndef EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_RES
    { CSR_BT_SYNCC_MODIFY_OBJECT_RES, CsrBtSynccModifyObjectResSizeof, CsrBtSynccModifyObjectResSer, CsrBtSynccModifyObjectResDes, CsrBtSynccModifyObjectResSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_RES */
#ifndef EXCLUDE_CSR_BT_SYNCC_SECURITY_IN_CFM
    { CSR_BT_SYNCC_SECURITY_IN_CFM, CsrBtSynccSecurityInCfmSizeof, CsrBtSynccSecurityInCfmSer, CsrBtSynccSecurityInCfmDes, CsrBtSynccSecurityInCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCC_SECURITY_IN_CFM */
#ifndef EXCLUDE_CSR_BT_SYNCC_SECURITY_IN_REQ
    { CSR_BT_SYNCC_SECURITY_IN_REQ, CsrBtSynccSecurityInReqSizeof, CsrBtSynccSecurityInReqSer, CsrBtSynccSecurityInReqDes, CsrBtSynccSecurityInReqSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCC_SECURITY_IN_REQ */
#ifndef EXCLUDE_CSR_BT_SYNCC_SECURITY_OUT_CFM
    { CSR_BT_SYNCC_SECURITY_OUT_CFM, CsrBtSynccSecurityOutCfmSizeof, CsrBtSynccSecurityOutCfmSer, CsrBtSynccSecurityOutCfmDes, CsrBtSynccSecurityOutCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCC_SECURITY_OUT_CFM */
#ifndef EXCLUDE_CSR_BT_SYNCC_SECURITY_OUT_REQ
    { CSR_BT_SYNCC_SECURITY_OUT_REQ, CsrBtSynccSecurityOutReqSizeof, CsrBtSynccSecurityOutReqSer, CsrBtSynccSecurityOutReqDes, CsrBtSynccSecurityOutReqSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCC_SECURITY_OUT_REQ */
#ifndef EXCLUDE_CSR_BT_SYNCC_SYNC_COMMAND_ABORT_IND
    { CSR_BT_SYNCC_SYNC_COMMAND_ABORT_IND, CsrBtSynccSyncCommandAbortIndSizeof, CsrBtSynccSyncCommandAbortIndSer, CsrBtSynccSyncCommandAbortIndDes, CsrBtSynccSyncCommandAbortIndSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCC_SYNC_COMMAND_ABORT_IND */
#ifndef EXCLUDE_CSR_BT_SYNCC_SYNC_COMMAND_IND
    { CSR_BT_SYNCC_SYNC_COMMAND_IND, CsrBtSynccSyncCommandIndSizeof, CsrBtSynccSyncCommandIndSer, CsrBtSynccSyncCommandIndDes, CsrBtSynccSyncCommandIndSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCC_SYNC_COMMAND_IND */
#ifndef EXCLUDE_CSR_BT_SYNCC_SYNC_COMMAND_RES
    { CSR_BT_SYNCC_SYNC_COMMAND_RES, CsrBtSynccSyncCommandResSizeof, CsrBtSynccSyncCommandResSer, CsrBtSynccSyncCommandResDes, CsrBtSynccSyncCommandResSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCC_SYNC_COMMAND_RES */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtSynccConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_SYNCC_PRIM, csr_bt_syncc_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_syncc_conv_info = {
    CSR_BT_SYNCC_PRIM,
    "CSR_BT_SYNCC_PRIM",
    csr_bt_syncc_conv_lut
};

CsrLogPrimitiveInformation* CsrBtSynccTechInfoGet(void)
{
    return &csr_bt_syncc_conv_info;
}
#endif
#endif

