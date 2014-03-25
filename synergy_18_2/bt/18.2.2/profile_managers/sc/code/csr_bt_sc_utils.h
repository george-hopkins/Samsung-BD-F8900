#ifndef CSR_BT_SC_UTILS_H__
#define CSR_BT_SC_UTILS_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_sched.h"
#include "dm_prim.h"
#include "csr_bt_common.h"
#include "csr_bt_sd_prim.h"
#include "csr_bt_cm_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    CsrBtScPrim               type;
    CsrBool                  doCancel;
    CsrUint8                 padding[CSRMAX(sizeof(CsrBtCmReadRemoteNameCfm), sizeof(CsrBtSdReadDeviceInfoCfm))];
} SC_PRIV_CANCEL_MSG_T;

#define VOIDP_TO_MSG_TYPE(instData) (*((CsrPrim *) (instData)->recvMsgP))

#if CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1
void CsrBtCmSmSendRepairResNoAccept(ScInstanceData_t * scData);
#else
#define CsrBtCmSmSendRepairResNoAccept(scData) 
#endif

void CsrBtCmScDmAuthoriseResSimpleInclInstReset(ScInstanceData_t * scData, CsrUint16 authorisation);

void CsrBtScRejectPendingMsg(ScInstanceData_t * scData);
void CsrBtScStopBondTimer(ScInstanceData_t *scData);
void CsrBtScStartPairingWatchdog(ScInstanceData_t *scData);


#ifdef CSR_LOG_ENABLE
void CsrBtScChangeState(char *file, CsrUint32 lineno, CsrUint8 *state, CsrUint8 newState, char *newStateName);
#define SC_CHANGE_STATE(state, newState) CsrBtScChangeState(__FILE__, __LINE__, state, newState, #newState)
#else
#define SC_CHANGE_STATE(state, newState) (*(state) = newState)
#endif


#ifdef CSR_LOG_ENABLE
void CsrBtScChangeMsgType(char *file, CsrUint32 lineno, CsrUint8 *pendingMsgType, CsrUint8 newPendingMsgType, char *newPendingMsgTypeName);
#define SC_CHANGE_MSGTYPE(pendingMsgType, newPendingMsgType) CsrBtScChangeMsgType(__FILE__, __LINE__, pendingMsgType, newPendingMsgType, #newPendingMsgType)
#else
#define SC_CHANGE_MSGTYPE(pendingMsgType, newPendingMsgType) (*(pendingMsgType) = newPendingMsgType)
#endif

void CsrBtScGo2Idle(ScInstanceData_t *scData);
void CsrBtScSaveMessage(ScInstanceData_t *scData);
void CsrBtScDmSaveMessage(ScInstanceData_t *scData);
void CsrBtScDmRestoreMessages(ScInstanceData_t *scData);
CsrBool CsrBtScAcceptsPairing(ScInstanceData_t *scData, CsrUint32 remoteAuth);

CsrBool CsrBtScIgnoreMessage(CsrMessageQueueType ** messageQueue, CsrPrim prim1, CsrBtScPrim msgId1);
void CsrBtScIgnorePendingMessages(CsrMessageQueueType ** messageQueue);
void CsrBtScAddPendingMessage(CsrMessageQueueType ** messageQueue, CsrPrim prim1, CsrBtScPrim msgId1);
void CsrBtScRemoveLastPendingMessage(CsrMessageQueueType ** messageQueue);
void CsrBtScReplaceLastPendingMessage(CsrMessageQueueType ** messageQueue, CsrPrim prim1, CsrBtScPrim msgId1);

void CsrBtScCopyDeviceProperties(CsrBtDevicePropertiesType *dest, CsrBtScDbDeviceRecord *source);
void CsrBtScCopyFriendlyName(CsrUtf8String * dest, CsrUtf8String * src);
void CsrBtScClearFriendlyName(CsrUint8 *remoteName);
CsrBool CsrBtScFriendlyNameValid(ScInstanceData_t* scData);

void CsrBtScResetInstData(ScInstanceData_t* scData);

CsrBool CsrBtScCheckProperLinkKey(CsrBtScDbDeviceRecord *devInfo, DM_SM_LINK_KEY_REQUEST_IND_T *dmPrim);

void CsrBtScScDbUpdate(ScInstanceData_t* scData);
void CsrBtScScDbClose(void);
void CsrBtScScDbInitAmp(CsrBtScDbDeviceRecord *db);

void CsrBtScMapToDmKeyType(ScInstanceData_t*scData);
#if 0 /* UNUSED */
CsrBool CsrBtScMapToDmAuthRequirements(CsrUint8 authRequirements);
#endif
CsrUint8 CsrBtScIoLimitier(ScInstanceData_t * scData, CsrUint8 ioCap);
void CsrBtScUpdateAssocModel(ScInstanceData_t *scData);
CsrUint8 CsrBtScGetAssocModel(CsrUint8 localIoCaps, CsrUint8 remoteIoCaps);
CsrUint8 CsrBtScLookupPairedType(ScInstanceData_t *scData);

#define CsrBtScEventIsEnabled(_eventMask, _event) ((_eventMask & _event) != 0)

void ScSetHciStatusIfUnset(ScInstanceData_t* scData, CsrUint8 hciStatus);

void CsrBtScNotifyUpperBondIndSimple(ScInstanceData_t *scData,
                               CsrSchedQid         phandle,
                               CsrBtResultCode     resultCode,
                               hci_return_t        hciStatus,
                               CsrBool             addedToScDbList);

void CsrBtScNotifyUpperBondInd(CsrBool             authorised,
                               CsrSchedQid         phandle,
                               CsrBtDeviceAddr     addr,
                               CsrBtClassOfDevice  cod,
                               CsrBtResultCode     resultCode,
                               CsrBtSupplier       resultSupplier,
                               hci_return_t        hciStatus,
                               CsrBool             addedToScDbList,
                               CsrBool             bondAckSent,
                               CsrBool             lowEnergy,
                               CsrBtAddressType    addrType);

void CsrBtScBondReqCancelHandlerAllowReset(ScInstanceData_t *scData, CsrSchedQid phandle);
void CsrBtScBondReqCancelHandler(ScInstanceData_t *scData,
                                 CsrSchedQid phandle,
                                 CsrBool allowReset);

void CsrBtScNotifyUpperBondCfm(CsrBool authorised,
                               CsrSchedQid phandle,
                               CsrBtResultCode resultCode,
                               CsrBtSupplier resultSupplier,
                               hci_return_t hciStatus,
                               CsrBtDeviceAddr addr,
                               CsrBtClassOfDevice cod,
                               CsrBool addedToScDbList,
                               CsrBool bondAckSent,
                               CsrBool lowEnergy,
                               CsrBtAddressType addrType);

void CsrBtScNotifyUpperDebondCfm(ScInstanceData_t *scData, CsrBtResultCode resultCode);

void CsrBtScDatabaseReqHandler(ScInstanceData_t *scData);

/* For backward compability */
void CsrBtScDmSmLocalUserConfirmationRequestIndMap(ScInstanceData_t *inst,
                                                   CsrUint16 event, void *message);

void CsrBtScAcceptBondReqHandler(ScInstanceData_t *scData);
void CsrBtScCancelAcceptBondReqHandler(ScInstanceData_t *scData);
void CsrBtScCancelAcceptBondIndHandler(ScInstanceData_t *scData);
void csrBtAcceptBondPeriodTimeout(CsrUint16 mi, void *mv);

#ifdef __cplusplus
}
#endif

#endif /* Defined CSR_BT_SC_BOND_UTILS_H__ */
