#ifndef CSR_BT_SYNCC_SERIALIZE_H__
#define CSR_BT_SYNCC_SERIALIZE_H__
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

void CsrBtSynccPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_SYNCC_DISCONNECT_IND
CsrSize CsrBtSynccDisconnectIndSizeof(void *msg);
CsrUint8 *CsrBtSynccDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSynccDisconnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSynccDisconnectIndSerFree CsrBtSynccPfree
#endif /* EXCLUDE_CSR_BT_SYNCC_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCC_SYNC_COMMAND_IND
CsrSize CsrBtSynccSyncCommandIndSizeof(void *msg);
CsrUint8 *CsrBtSynccSyncCommandIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSynccSyncCommandIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSynccSyncCommandIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCC_SYNC_COMMAND_IND */

#ifndef EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_RES
CsrSize CsrBtSynccModifyObjectResSizeof(void *msg);
CsrUint8 *CsrBtSynccModifyObjectResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSynccModifyObjectResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSynccModifyObjectResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_SYNCC_DISCONNECT_REQ
CsrSize CsrBtSynccDisconnectReqSizeof(void *msg);
CsrUint8 *CsrBtSynccDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSynccDisconnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSynccDisconnectReqSerFree CsrBtSynccPfree
#endif /* EXCLUDE_CSR_BT_SYNCC_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_DELETE_OBJECT_CFM
CsrSize CsrBtSynccDeleteObjectCfmSizeof(void *msg);
CsrUint8 *CsrBtSynccDeleteObjectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSynccDeleteObjectCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSynccDeleteObjectCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCC_DELETE_OBJECT_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCC_CONNECT_REQ
CsrSize CsrBtSynccConnectReqSizeof(void *msg);
CsrUint8 *CsrBtSynccConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSynccConnectReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSynccConnectReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_REQ
CsrSize CsrBtSynccModifyObjectReqSizeof(void *msg);
CsrUint8 *CsrBtSynccModifyObjectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSynccModifyObjectReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSynccModifyObjectReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_REQ
CsrSize CsrBtSynccAddObjectReqSizeof(void *msg);
CsrUint8 *CsrBtSynccAddObjectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSynccAddObjectReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSynccAddObjectReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_AUTHENTICATE_IND
CsrSize CsrBtSynccAuthenticateIndSizeof(void *msg);
CsrUint8 *CsrBtSynccAuthenticateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSynccAuthenticateIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSynccAuthenticateIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCC_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_SYNCC_CANCEL_CONNECT_REQ
CsrSize CsrBtSynccCancelConnectReqSizeof(void *msg);
CsrUint8 *CsrBtSynccCancelConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSynccCancelConnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSynccCancelConnectReqSerFree CsrBtSynccPfree
#endif /* EXCLUDE_CSR_BT_SYNCC_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_ABORT_CFM
CsrSize CsrBtSynccAbortCfmSizeof(void *msg);
CsrUint8 *CsrBtSynccAbortCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSynccAbortCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSynccAbortCfmSerFree CsrBtSynccPfree
#endif /* EXCLUDE_CSR_BT_SYNCC_ABORT_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_RES
CsrSize CsrBtSynccAddObjectResSizeof(void *msg);
CsrUint8 *CsrBtSynccAddObjectResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSynccAddObjectResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSynccAddObjectResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_SYNCC_DEACTIVATE_SYNC_COMMAND_CFM
CsrSize CsrBtSynccDeactivateSyncCommandCfmSizeof(void *msg);
CsrUint8 *CsrBtSynccDeactivateSyncCommandCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSynccDeactivateSyncCommandCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSynccDeactivateSyncCommandCfmSerFree CsrBtSynccPfree
#endif /* EXCLUDE_CSR_BT_SYNCC_DEACTIVATE_SYNC_COMMAND_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCC_ABORT_REQ
CsrSize CsrBtSynccAbortReqSizeof(void *msg);
CsrUint8 *CsrBtSynccAbortReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSynccAbortReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSynccAbortReqSerFree CsrBtSynccPfree
#endif /* EXCLUDE_CSR_BT_SYNCC_ABORT_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_SECURITY_IN_REQ
CsrSize CsrBtSynccSecurityInReqSizeof(void *msg);
CsrUint8 *CsrBtSynccSecurityInReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSynccSecurityInReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSynccSecurityInReqSerFree CsrBtSynccPfree
#endif /* EXCLUDE_CSR_BT_SYNCC_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_AUTHENTICATE_RES
CsrSize CsrBtSynccAuthenticateResSizeof(void *msg);
CsrUint8 *CsrBtSynccAuthenticateResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSynccAuthenticateResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSynccAuthenticateResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCC_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_SYNCC_ACTIVATE_SYNC_COMMAND_REQ
CsrSize CsrBtSynccActivateSyncCommandReqSizeof(void *msg);
CsrUint8 *CsrBtSynccActivateSyncCommandReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSynccActivateSyncCommandReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSynccActivateSyncCommandReqSerFree CsrBtSynccPfree
#endif /* EXCLUDE_CSR_BT_SYNCC_ACTIVATE_SYNC_COMMAND_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_SYNC_COMMAND_RES
CsrSize CsrBtSynccSyncCommandResSizeof(void *msg);
CsrUint8 *CsrBtSynccSyncCommandResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSynccSyncCommandResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSynccSyncCommandResSerFree CsrBtSynccPfree
#endif /* EXCLUDE_CSR_BT_SYNCC_SYNC_COMMAND_RES */

#ifndef EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_IND
CsrSize CsrBtSynccAddObjectIndSizeof(void *msg);
CsrUint8 *CsrBtSynccAddObjectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSynccAddObjectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSynccAddObjectIndSerFree CsrBtSynccPfree
#endif /* EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_IND
CsrSize CsrBtSynccModifyObjectIndSizeof(void *msg);
CsrUint8 *CsrBtSynccModifyObjectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSynccModifyObjectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSynccModifyObjectIndSerFree CsrBtSynccPfree
#endif /* EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_RES
CsrSize CsrBtSynccGetObjectResSizeof(void *msg);
CsrUint8 *CsrBtSynccGetObjectResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSynccGetObjectResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSynccGetObjectResSerFree CsrBtSynccPfree
#endif /* EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_CFM
CsrSize CsrBtSynccGetObjectCfmSizeof(void *msg);
CsrUint8 *CsrBtSynccGetObjectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSynccGetObjectCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSynccGetObjectCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCC_SECURITY_OUT_CFM
CsrSize CsrBtSynccSecurityOutCfmSizeof(void *msg);
CsrUint8 *CsrBtSynccSecurityOutCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSynccSecurityOutCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSynccSecurityOutCfmSerFree CsrBtSynccPfree
#endif /* EXCLUDE_CSR_BT_SYNCC_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCC_SYNC_COMMAND_ABORT_IND
CsrSize CsrBtSynccSyncCommandAbortIndSizeof(void *msg);
CsrUint8 *CsrBtSynccSyncCommandAbortIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSynccSyncCommandAbortIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSynccSyncCommandAbortIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCC_SYNC_COMMAND_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_IND
CsrSize CsrBtSynccGetObjectIndSizeof(void *msg);
CsrUint8 *CsrBtSynccGetObjectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSynccGetObjectIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSynccGetObjectIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCC_DEACTIVATE_SYNC_COMMAND_REQ
CsrSize CsrBtSynccDeactivateSyncCommandReqSizeof(void *msg);
CsrUint8 *CsrBtSynccDeactivateSyncCommandReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSynccDeactivateSyncCommandReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSynccDeactivateSyncCommandReqSerFree CsrBtSynccPfree
#endif /* EXCLUDE_CSR_BT_SYNCC_DEACTIVATE_SYNC_COMMAND_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_CFM
CsrSize CsrBtSynccModifyObjectCfmSizeof(void *msg);
CsrUint8 *CsrBtSynccModifyObjectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSynccModifyObjectCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSynccModifyObjectCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_REQ
CsrSize CsrBtSynccGetObjectReqSizeof(void *msg);
CsrUint8 *CsrBtSynccGetObjectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSynccGetObjectReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSynccGetObjectReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_SECURITY_IN_CFM
CsrSize CsrBtSynccSecurityInCfmSizeof(void *msg);
CsrUint8 *CsrBtSynccSecurityInCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSynccSecurityInCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSynccSecurityInCfmSerFree CsrBtSynccPfree
#endif /* EXCLUDE_CSR_BT_SYNCC_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCC_SECURITY_OUT_REQ
CsrSize CsrBtSynccSecurityOutReqSizeof(void *msg);
CsrUint8 *CsrBtSynccSecurityOutReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSynccSecurityOutReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSynccSecurityOutReqSerFree CsrBtSynccPfree
#endif /* EXCLUDE_CSR_BT_SYNCC_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_CONNECT_CFM
CsrSize CsrBtSynccConnectCfmSizeof(void *msg);
CsrUint8 *CsrBtSynccConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSynccConnectCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSynccConnectCfmSerFree CsrBtSynccPfree
#endif /* EXCLUDE_CSR_BT_SYNCC_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_CFM
CsrSize CsrBtSynccAddObjectCfmSizeof(void *msg);
CsrUint8 *CsrBtSynccAddObjectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSynccAddObjectCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSynccAddObjectCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCC_DELETE_OBJECT_REQ
CsrSize CsrBtSynccDeleteObjectReqSizeof(void *msg);
CsrUint8 *CsrBtSynccDeleteObjectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSynccDeleteObjectReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSynccDeleteObjectReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCC_DELETE_OBJECT_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_ACTIVATE_SYNC_COMMAND_CFM
CsrSize CsrBtSynccActivateSyncCommandCfmSizeof(void *msg);
CsrUint8 *CsrBtSynccActivateSyncCommandCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSynccActivateSyncCommandCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSynccActivateSyncCommandCfmSerFree CsrBtSynccPfree
#endif /* EXCLUDE_CSR_BT_SYNCC_ACTIVATE_SYNC_COMMAND_CFM */

#ifdef __cplusplus
}
#endif

#endif
