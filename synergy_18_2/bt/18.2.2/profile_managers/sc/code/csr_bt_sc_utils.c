/****************************************************************************

    Copyright Cambridge Silicon Radio Limited and its licensors 2012. 
            
              All rights reserved.

REVISION:      $Revision: #3 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_sched.h"
#include "csr_pmem.h"
#include "dm_prim.h"
#include "csr_bt_sc_bond_unified.h"
#include "csr_bt_cm_dm_sc_lib.h"
#include "csr_bt_sc_prim.h"
#include "csr_bt_sc_main.h"
#include "csr_bt_util.h"
#include "csr_bt_sd_lib.h"
#include "csr_bt_sc_lib_upstream.h"
#include "csr_bt_sc_bond_legacy.h"
#include "csr_bt_sc_utils.h"
#include "csr_bt_profiles.h"
#include "csr_bt_cm_dm_sc_lib.h"
#include "csr_bt_cm_dm_sc_ssp_lib.h"
#include "csr_bt_sc_bond_local.h"
#include "csr_bt_sc_bond_remote.h"
#include "csr_bt_sc_rebond.h"
#include "csr_bt_sc_oob.h"
#include "csr_bt_sd_private_prim.h"
#include "csr_bt_sd_private_lib.h"
#include "csr_unicode.h"
#ifdef CSR_LOG_ENABLE
#include "csr_log.h"
#endif

#if CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1
void CsrBtCmSmSendRepairResNoAccept(ScInstanceData_t * scData)
{
    CsrBtCmSmSendRepairRes(scData->deviceAddr, FALSE,
                           scData->repairData.repairId,
                           scData->addressType);    
}
#endif

void CsrBtCmScDmAuthoriseResSimpleInclInstReset(ScInstanceData_t * scData, CsrUint16 authorisation)
{
    CsrBtCmScDmAuthoriseRes(scData->deviceAddr,
                            scData->incoming,
                            authorisation,
                            scData->channel,
                            scData->protocolId,
                            scData->addressType);
    
    CsrBtScResetInstData(scData);
}

void CsrBtScRejectPendingMsg(ScInstanceData_t * scData)
{
    switch(scData->pendingMsgType)
    {
        case MSG_TYPE_LEGACY:
            if(scData->bondData.appState == APP_RESPOND)
            {
                /* Treat as if app responded */
                scData->authorised = FALSE;
                CsrBtCmScDmPinRequestNegRes(scData->deviceAddr);
                SC_CHANGE_STATE(&scData->bondData.appState, APP_RESPONDED);
            }
            break;

        case MSG_TYPE_JUSTWORKS:
        case MSG_TYPE_COMPARE:
            if(scData->bondData.appState == APP_RESPOND)
            {
                /* Treat as if app responded */
                CsrBtCmScDmUserConfirmationRequestNegRes(scData->deviceAddr,
                                                         scData->addressType);
                SC_CHANGE_STATE(&scData->bondData.appState, APP_RESPONDED);
            }
            break;

        case MSG_TYPE_PASSKEY:
            if(scData->bondData.appState == APP_RESPOND)
            {
                /* Treat as if app responded */
                CsrBtCmScDmUserPasskeyRequestNegRes(scData->deviceAddr,
                                                    scData->addressType);
                SC_CHANGE_STATE(&scData->bondData.appState, APP_RESPONDED);
            }
            break;

        case MSG_TYPE_IO_CAPS:
            CsrBtCmScDmIoCapabilityRequestNegRes(scData->deviceAddr,
                                                 HCI_ERROR_HOST_BUSY_PAIRING,
                                                 scData->addressType);
            break;

        case MSG_TYPE_REBOND:
            scData->authorised = FALSE;
            CsrBtCmSmSendRepairResNoAccept(scData);
            break;

        case MSG_TYPE_NOTIFICATION:
            /* We can't reject a passkey notification */
            break;

        case MSG_TYPE_AUTHORISE:
            /* D-1567 */
            break;

        case MSG_TYPE_NONE:
            /* To early to make a call */
            break;

        default:
            CsrBtScStateEventException(CSR_BT_SC_PRIM, scData->pendingMsgType, scData->bondData.dmState);
            break;
    }
}

static void csrBtScTimerFireLegacy(ScInstanceData_t *scData)
{
    switch(scData->state)
    {
        case SC_ST_LOCAL_POST_LISBON_BONDING:
            /* NOP. Wait for bonding/auth. confirm instead */
            break;

        case SC_ST_REMOTE_BONDING:
            /* Pretend that the app responded */
            CsrBtScBondReqCancelHandlerAllowReset(scData, scData->mainAppHandle); /* allow reset */
            break;

        default:
            CsrBtScStateEventException(CSR_BT_SC_PRIM, scData->pendingMsgType, scData->state);
            break;
    }
}

static void csrBtScTimerFirePassKeyNotificationBondComplete(ScInstanceData_t *scData)
{
    switch(scData->state)
    {
        case SC_ST_LOCAL_POST_LISBON_BONDING:
            /* Pairing completed. Just indicate failure to the app */
            CsrBtScPairingCancelled(scData);
            break;

        case SC_ST_REBOND:
            /* Pairing completed. Just indicate failure to the app */
            CsrBtScBondReqCancelHandlerAllowReset(scData, scData->appHandle); /* allow reset */
            CsrBtCmSmSendRepairResNoAccept(scData);
            CsrBtScPairingFailedCleanupDeviceReset(scData);
            break;

        case SC_ST_REMOTE_BONDING:
            /* Pretend that the app responded */
            CsrBtScBondReqCancelHandlerAllowReset(scData, scData->mainAppHandle); /* allow reset */
            break;

        default:
            CsrBtScStateEventException(CSR_BT_SC_PRIM, scData->pendingMsgType, scData->state);
            break;
    }
}

static void csrBtScTimerFirePassKeyNotification(ScInstanceData_t *scData)
{
    switch(scData->bondData.dmState)
    {
        case DM_AUTHEN_RECV:
            /* NOP. Wait for bonding/auth. confirm instead */
            SC_CHANGE_STATE(&scData->bondData.dmState, DM_AUTHEN_RESP);
            SC_CHANGE_STATE(&scData->bondData.appState, APP_RESPONDED);
            break;

        case DM_BOND_COMPLETE:
            csrBtScTimerFirePassKeyNotificationBondComplete(scData);
            break;

        default:
            CsrBtScStateEventException(CSR_BT_SC_PRIM, scData->pendingMsgType, scData->state);
            break;
    }
}

static void csrBtScTimerFireRebond(ScInstanceData_t *scData)
{
    CsrBtScBondReqCancelHandlerAllowReset(scData, scData->appHandle); /* allow reset */
}

static void csrBtScTimerFirePair(ScInstanceData_t *scData)
{
    switch(scData->state)
    {
        case SC_ST_LOCAL_POST_LISBON_BONDING:
            /* Pairing completed. Wait for bonding confirm */
            break;

        case SC_ST_REMOTE_BONDING:
            /* Pretend that the app responded */
            CsrBtScBondReqCancelHandlerAllowReset(scData, scData->appHandle); /* allow reset */
            break;

        default:
            CsrBtScStateEventException(CSR_BT_SC_PRIM, scData->pendingMsgType, scData->state);
            break;
    }
}

static void csrBtScTimerFireWatchdog(CsrUint16 mi, ScInstanceData_t *scData)
{
    CSR_UNUSED(mi);
    scData->bondData.wdTimer = 0;

    /* Reject current pending message */
    CsrBtScRejectPendingMsg(scData);

    switch(scData->pendingMsgType)
    {
        case MSG_TYPE_LEGACY:
            csrBtScTimerFireLegacy(scData);
            break;

        case MSG_TYPE_NOTIFICATION:
            csrBtScTimerFirePassKeyNotification(scData);
            break;

        case MSG_TYPE_REBOND:
            csrBtScTimerFireRebond(scData);
            break;

        case MSG_TYPE_IO_CAPS:
            csrBtScTimerFirePair(scData);
            break;

        case MSG_TYPE_JUSTWORKS:
        case MSG_TYPE_COMPARE:
        case MSG_TYPE_PASSKEY:
            /* Move forward as if though the app had responded */
            SC_CHANGE_STATE(&scData->bondData.dmState, DM_AUTHEN_RESP);
            SC_CHANGE_STATE(&scData->bondData.appState, APP_RESPONDED);
            break;

        case MSG_TYPE_AUTHORISE:
            /* The application did not respond to a CSR_BT_SC_AUTHORISE_IND */
            CsrBtCmScDmAuthoriseResSimpleInclInstReset(scData, DM_SM_AUTHORISE_REJECT_ONCE);
            break;

        default:
            CsrBtScStateEventException(CSR_BT_SC_PRIM, scData->pendingMsgType, scData->state);
            break;
    }
}

void CsrBtScStartPairingWatchdog(ScInstanceData_t *scData)
{
    CsrBtScStopBondTimer(scData);
    scData->bondData.wdTimer = CsrSchedTimerSet(scData->appTimeout,
                                                (void (*) (CsrUint16, void *)) csrBtScTimerFireWatchdog,
                                                CSR_BT_SC_IFACEQUEUE,
                                                scData);
}

void CsrBtScStopBondTimer(ScInstanceData_t *scData)
{
    if (scData->bondData.wdTimer)
    {
        CsrSchedTimerCancel(scData->bondData.wdTimer, NULL, NULL);
        scData->bondData.wdTimer = 0;
    }
}


#ifdef CSR_LOG_ENABLE
void CsrBtScChangeState(char *file, CsrUint32 lineno, CsrUint8 *state, CsrUint8 newState, char *newStateName)
{
    CsrLogStateTransition((CSR_LOG_STATE_TRANSITION_MASK_FSM_NAME |
                           CSR_LOG_STATE_TRANSITION_MASK_NEXT_STATE |
                           CSR_LOG_STATE_TRANSITION_MASK_NEXT_STATE_STR |
                           CSR_LOG_STATE_TRANSITION_MASK_PREV_STATE),
                          0,
                          NULL,
                          *state,
                          NULL,
                          0,
                          NULL,
                          newState,
                          newStateName,
                          lineno,
                          file);

    *state = newState;
}
#endif

#ifdef CSR_LOG_ENABLE
void CsrBtScChangeMsgType(char *file, CsrUint32 lineno, CsrUint8 *pendingMsgType, CsrUint8 newPendingMsgType, char *newPendingMsgTypeName)
{
    CsrLogStateTransition((CSR_LOG_STATE_TRANSITION_MASK_FSM_NAME |
                           CSR_LOG_STATE_TRANSITION_MASK_NEXT_STATE |
                           CSR_LOG_STATE_TRANSITION_MASK_NEXT_STATE_STR |
                           CSR_LOG_STATE_TRANSITION_MASK_PREV_STATE),
                          0,
                          NULL,
                          *pendingMsgType,
                          NULL,
                          0,
                          NULL,
                          newPendingMsgType,
                          newPendingMsgTypeName,
                          lineno,
                          file);
    *pendingMsgType = newPendingMsgType;
}
#endif

void CsrBtScGo2Idle(ScInstanceData_t *scData)
{
    CsrBtScPrim  *scPrim;

    scData->restoreMsgFlag = TRUE;

    scPrim  = CsrPmemAlloc(sizeof(CsrBtScPrim));
    *scPrim = CSR_BT_SC_HOUSE_CLEANING;
    CsrBtScMessagePut(CSR_BT_SC_IFACEQUEUE, scPrim);
}

CsrBool CsrBtScAcceptsPairing(ScInstanceData_t *scData, CsrUint32 remoteAuth)
{
    CsrBool allowPairing;

    switch(scData->bondData.bondable)
    {
        case CSR_BT_SEC_MODE_PAIRABLE:
            allowPairing = TRUE;
            break;

        case CSR_BT_SEC_MODE_NON_PAIRABLE:
            allowPairing = FALSE;
            break;

        case CSR_BT_SEC_MODE_NON_BONDABLE:
            {
                if ((remoteAuth == CSR_BT_SEC_MODE_MAX) ||
                    (remoteAuth & SC_AUTH_REQ_DEDICATED_BONDING) != 0 ||
                    (remoteAuth & SC_AUTH_REQ_GENERAL_BONDING) != 0)
                {
                    allowPairing = FALSE;
                }
                else
                {
                    allowPairing = TRUE;
                }
                break;
            }

        default:
            allowPairing = FALSE;
            break;
    }

    return allowPairing;
}

void CsrBtScSaveMessage(ScInstanceData_t *scData)
{
    CsrMessageQueuePush(&scData->saveQueue, CSR_BT_SC_PRIM, scData->recvMsgP);
    scData->recvMsgP = NULL;
}

void CsrBtScDmSaveMessage(ScInstanceData_t *scData)
{
    CsrMessageQueuePush(&scData->saveQueue, DM_PRIM, scData->recvMsgP);
    scData->recvMsgP = NULL;
}

void CsrBtScDmRestoreMessages(ScInstanceData_t *scData)
{
    (void ) CsrMessageQueueRestore(&scData->saveQueue, CSR_BT_SC_IFACEQUEUE);
}

void CsrBtScAddPendingMessage(CsrMessageQueueType ** messageQueue, CsrPrim prim1, CsrBtScPrim msgId1)
{
    SC_PRIV_CANCEL_MSG_T *msgId2;

    msgId2 = zpnew(SC_PRIV_CANCEL_MSG_T);
    msgId2->type= msgId1;
    msgId2->doCancel = FALSE;

    CsrMessageQueuePush(messageQueue, prim1, msgId2);
}

void CsrBtScRemoveLastPendingMessage(CsrMessageQueueType ** messageQueue)
{
    CsrUint16  prim2;
    void *msgId2;

    (void) CsrMessageQueuePop(messageQueue, &prim2, &msgId2);

    CsrPmemFree(msgId2);
}

void CsrBtScReplaceLastPendingMessage(CsrMessageQueueType ** messageQueue, CsrPrim prim1, CsrBtScPrim msgId1)
{
    CsrBtScRemoveLastPendingMessage(messageQueue);
    CsrBtScAddPendingMessage(messageQueue, prim1, msgId1);
}

void CsrBtScIgnorePendingMessages(CsrMessageQueueType ** messageQueue)
{
    CsrUint16             prim1;
    SC_PRIV_CANCEL_MSG_T *msgId1;
    void *popMsg;
    CsrMessageQueueType *tempQueue = NULL;

    while(CsrMessageQueuePop(messageQueue, &prim1, &popMsg))
    {
        msgId1 = (SC_PRIV_CANCEL_MSG_T *)popMsg;

        msgId1->doCancel = TRUE;
        CsrMessageQueuePush(&tempQueue, prim1, msgId1);
    }
    *messageQueue = tempQueue;
}

CsrBool CsrBtScIgnoreMessage(CsrMessageQueueType ** messageQueue, CsrPrim prim1, CsrBtScPrim msgId1)
{
    CsrUint16             prim2;
    SC_PRIV_CANCEL_MSG_T *msgId2;
    void *popMsg;
    CsrBool               msgIgnored = FALSE;
    CsrMessageQueueType *tempQueue = NULL;

    while(CsrMessageQueuePop(messageQueue, &prim2, &popMsg))
    {
        msgId2 = (SC_PRIV_CANCEL_MSG_T *)popMsg;

        if (!msgIgnored && prim2 == prim1 && msgId2->type == msgId1 && msgId2->doCancel)
        {
            msgIgnored = TRUE;
            CsrPmemFree(msgId2);
        }
        else
        {
            CsrMessageQueuePush(&tempQueue, prim2, msgId2);
        }
    }
    *messageQueue = tempQueue;

    return (msgIgnored);
}

void CsrBtScCopyFriendlyName(CsrUtf8String * dest, CsrUtf8String * src)
{
    CsrUtf8StrNCpy(dest, src, CSR_BT_MAX_FRIENDLY_NAME_LEN+1);
    CsrUtf8StrTruncate(dest, CSR_BT_MAX_FRIENDLY_NAME_LEN);
}

CsrBool CsrBtScFriendlyNameValid(ScInstanceData_t* scData)
{
    return (scData->friendlyName[0] != '\0');
}

void CsrBtScClearFriendlyName(CsrUint8 *remoteName)
{
    CsrMemSet(remoteName, '\0', CSR_BT_MAX_FRIENDLY_NAME_LEN + 1);
}

CsrBool CsrBtScCheckProperLinkKey(CsrBtScDbDeviceRecord *devInfo, DM_SM_LINK_KEY_REQUEST_IND_T *dmPrim)
{
    if (dmPrim->authenticated == FALSE ||
        (dmPrim->authenticated == TRUE &&
         devInfo->linkkeyType  == DM_SM_LINK_KEY_AUTHENTICATED))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void CsrBtScCopyDeviceProperties(CsrBtDevicePropertiesType *dest, CsrBtScDbDeviceRecord *source)
{
    CsrBtScCopyFriendlyName(dest->friendlyName, source->remoteName);

    dest->classOfDevice          = source->classOfDevice;
    dest->deviceAddr             = source->deviceAddr;
    dest->authorised             = source->authorised;
    dest->knownServices11_00_31 = source->knownServices11_00_31;
    dest->knownServices11_32_63 = source->knownServices11_32_63;
    dest->knownServices12_00_31 = source->knownServices12_00_31;
    dest->knownServices13_00_31 = source->knownServices13_00_31;
}

void CsrBtScScDbUpdate(ScInstanceData_t* scData)
{
    CsrBtScDbDeviceRecord dbInfo;

    CsrMemSet(&dbInfo, 0, sizeof(CsrBtScDbDeviceRecord));

    if (!CsrBtScDbRead(&(scData->deviceAddr),&dbInfo))
    {
        CsrBtBdAddrCopy(&dbInfo.deviceAddr, &scData->deviceAddr);

#ifdef CSR_AMP_ENABLE
        /* AMP keys start as invalid */
        dbInfo.ampKeyGenericType = HCI_KEY_TYPE_UNKNOWN;
        dbInfo.ampKeyDot11Type = HCI_KEY_TYPE_UNKNOWN;
        dbInfo.ampKeyUwbType = HCI_KEY_TYPE_UNKNOWN;
#endif
    }

    dbInfo.authorised    = scData->authorised;
    dbInfo.classOfDevice = scData->cod;
    dbInfo.linkkeyValid  = TRUE;
    dbInfo.linkkeyLen    = SIZE_LINK_KEY;
    dbInfo.linkkeyType   = scData->bondData.linkKeyType;
    CsrBtScCopyFriendlyName(dbInfo.remoteName, scData->friendlyName);
    CsrMemCpy(&dbInfo.Linkkey, &scData->bondData.linkKey, SIZE_LINK_KEY);

#ifdef CSR_BT_LE_ENABLE
    dbInfo.addressType = CSR_BT_SC_ADDR_TYPE(scData);

    /* Low energy keys */
    if(CSR_BT_SC_IS_LOW_ENERGY(scData))
    {
        CsrUint16 valid;
        valid = scData->bondData.leKeyValid;
        dbInfo.leKeyValid = valid;
        if(valid & CSR_BT_SC_DB_LE_KEY_ENC_CENTRAL)
        {
            if(scData->bondData.leKeyEncCentral)
            {
                CsrMemCpy(&dbInfo.leKeyEncCentral,
                          scData->bondData.leKeyEncCentral,
                          sizeof(DM_SM_KEY_ENC_CENTRAL_T));
            }
            else
            {
                CsrMemSet(&dbInfo.leKeyEncCentral, 0, sizeof(DM_SM_KEY_ENC_CENTRAL_T));
            }
        }
        if(valid & CSR_BT_SC_DB_LE_KEY_ID)
        {
            if(scData->bondData.leKeyId)
            {
                CsrMemCpy(&dbInfo.leKeyId,
                          scData->bondData.leKeyId,
                          sizeof(DM_SM_KEY_ID_T));
            }
            else
            {
                CsrMemSet(&dbInfo.leKeyId, 0, sizeof(DM_SM_KEY_ID_T));
            }
        }
        if(valid & CSR_BT_SC_DB_LE_KEY_SIGN)
        {
            if(scData->bondData.leKeySign)
            {
                CsrMemCpy(&dbInfo.leKeySign,
                          scData->bondData.leKeySign,
                          sizeof(DM_SM_KEY_SIGN_T));
            }
            else
            {
                CsrMemSet(&dbInfo.leKeySign, 0, sizeof(DM_SM_KEY_SIGN_T));
            }
        }
        if(valid & CSR_BT_SC_DB_LE_KEY_DIV)
        {
            dbInfo.leKeyDiv = scData->bondData.leKeyDiv;
        }
        if(valid & CSR_BT_SC_DB_LE_KEY_SIGN_COUNTER)
        {
            dbInfo.leKeySignCounter = scData->bondData.leKeySignCounter;
        }
        dbInfo.leKeySize = scData->bondData.leKeySize;
        dbInfo.leSecurityRequirements = scData->bondData.leSecurityRequirements;
    }
#endif

    CsrBtScDbWrite(&dbInfo.deviceAddr, &dbInfo);
}


void CsrBtScScDbClose(void)
{
    CsrBtScDbDeviceRecord record;
    CsrBool recordRead;
    do {
        recordRead = CsrBtScDbReadNext(&record);
    } while (recordRead);
}

static void csrBtScResetOobInstData(ScInstanceData_t* scData)
{
    scData->sspData.remoteOobDataPresent = FALSE;
    CsrBtBdAddrZero(&scData->sspData.deviceAddr);
    CsrMemSet(&scData->sspData.oobHashC, 0, SIZE_OOB_DATA);
    CsrMemSet(&scData->sspData.oobRandR, 0, SIZE_OOB_DATA);
}

void CsrBtScResetInstData(ScInstanceData_t* scData)
{
    csrBtScResetOobInstData(scData);

    scData->appHandle              = CSR_SCHED_QID_INVALID;
    scData->bondData.initiator     = INITIATOR_NONE;
    scData->bondData.remoteIoCaps  = HCI_IO_CAP_MAX+1;
    scData->bondData.remoteAuth    = HCI_MITM_PROTECTION_MAX+1;
    scData->bondData.linkKeyType   = HCI_KEY_TYPE_UNKNOWN;
    scData->bondData.bondAckSent   = FALSE;

    SC_CHANGE_MSGTYPE(&scData->pendingMsgType, MSG_TYPE_NONE);
    SC_CHANGE_STATE(&scData->state, SC_ST_IDLE);
    SC_CHANGE_STATE(&scData->bondData.appState, APP_IDLE);
    SC_CHANGE_STATE(&scData->bondData.dmState, DM_IDLE);
    SC_CHANGE_STATE(&scData->bondData.scState, SC_IDLE);
    SC_CHANGE_STATE(&scData->bondData.uiState, UI_IDLE);
    SC_CHANGE_STATE(&scData->dmSmInitState, SM_INIT_ST_IDLE);

    scData->cod                  = 0;
    scData->paired               = CSR_BT_SC_PAIRING_NONE;
    scData->addDevice            = FALSE;
    scData->authorised           = FALSE;
    scData->accept               = FALSE;
    scData->hciStatus            = HCI_ERROR_UNSPECIFIED;
    scData->repairData.repairId  = 0;
    CsrBtScClearFriendlyName((CsrUint8 *) scData->friendlyName);

    CsrBtBdAddrZero(&scData->deviceAddr);

    CsrBtScStopBondTimer(scData);

    CsrBtScIgnorePendingMessages(&scData->ignoreQueue);

#ifdef CSR_BT_LE_ENABLE
    scData->bondData.lowEnergy = FALSE;
    scData->bondData.l2capFlags = 0;
    scData->bondData.btConnId = CSR_BT_CONN_ID_INVALID;
    scData->bondData.leKeyValid = 0;
    CsrPmemFree(scData->bondData.leKeyEncCentral);
    scData->bondData.leKeyEncCentral = NULL;
    CsrPmemFree(scData->bondData.leKeyId);
    scData->bondData.leKeyId = NULL;
    CsrPmemFree(scData->bondData.leKeySign);
    scData->bondData.leKeySign = NULL;
    scData->bondData.leKeyDiv = 0;
    scData->bondData.leKeySignCounter = 0;
    scData->bondData.leAppHandle = CSR_SCHED_QID_INVALID;
    scData->bondData.leLocalAuth = scData->bondData.leDefaultLocalAuth; /* Restore the default authentication settings to use */
    scData->bondData.leRebondCnt = 0;
#endif

    if (scData->saveQueue != NULL)
    {
        CsrBtScGo2Idle(scData);
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtScMapToDmKeyType
 *
 *  DESCRIPTION
 *      Maps the given linkkey type so the one expected by the DM.
 *      This is done according to the LMP/controller version.
 *
 *  RETURNS/MODIFIES
 *
 *----------------------------------------------------------------------------*/
void CsrBtScMapToDmKeyType(ScInstanceData_t *scData)
{
    static const CsrUint8 keyTypeMapping[] =
        {
            DM_SM_LINK_KEY_LEGACY,              /* HCI_COMBINATION_KEY */
            DM_SM_LINK_KEY_LEGACY,              /* HCI_LOCAL_UNIT_KEY */
            DM_SM_LINK_KEY_LEGACY,              /* HCI_REMOTE_UNIT_KEY */
            DM_SM_LINK_KEY_DEBUG,               /* HCI_DEBUG_COMBINATION_KEY */
            DM_SM_LINK_KEY_UNAUTHENTICATED,     /* HCI_UNAUTHENTICATED_COMBINATION_KEY */
            DM_SM_LINK_KEY_AUTHENTICATED,       /* HCI_AUTHENTICATED_COMBINATION_KEY */
            DM_SM_LINK_KEY_LEGACY,              /* HCI_CHANGED_COMBINATION_KEY */
        };

    DM_SM_LINK_KEY_IND_T *dmPrim;

    dmPrim = (DM_SM_LINK_KEY_IND_T *) scData->recvMsgP;

    dmPrim->key_type = (dmPrim->key_type < sizeof(keyTypeMapping))
        ? keyTypeMapping[dmPrim->key_type]
        : DM_SM_LINK_KEY_NONE;
}

/* Limit I/O capabilities: Low energy has a KEYBOARD_DISPLAY
 * capability that BT2.1 SSP does not. So, if the local device tries
 * the new setting, we automatically roll back to the most compatible
 * BT2.1 capability */
CsrUint8 CsrBtScIoLimitier(ScInstanceData_t * scData, CsrUint8 ioCap)
{
    if((ioCap == HCI_IO_CAP_KEYBOARD_DISPLAY)
#ifdef CSR_BT_LE_ENABLE
        && !scData->bondData.lowEnergy
#endif
       )
    {
        return HCI_IO_CAP_DISPLAY_YES_NO;
    }
    return ioCap;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtScMapToDmAuthRequirements
 *
 *  DESCRIPTION
 *
 *  RETURNS/MODIFIES
 *
 *----------------------------------------------------------------------------*/
#if 0 /* UNUSED */
CsrBool CsrBtScMapToDmAuthRequirements(CsrUint8 authRequirements)
{
    static const CsrBool authRequireMapping[] =
        {
            FALSE,  /* HCI_MITM_NOT_REQUIRED_NO_BONDING */
            FALSE,  /* HCI_MITM_REQUIRED_NO_BONDING */
            FALSE,  /* HCI_MITM_NOT_REQUIRED_DEDICATED_BONDING */
            FALSE,  /* HCI_MITM_REQUIRED_DEDICATED_BONDING */
            TRUE,   /* HCI_MITM_NOT_REQUIRED_GENERAL_BONDING */
            TRUE,   /* HCI_MITM_REQUIRED_GENERAL_BONDING */
        };

    return (authRequirements < sizeof(authRequireMapping))
        ? authRequireMapping[authRequirements]
        : HCI_MITM_PROTECTION_MAX+1;
}
#endif

CsrUint8 CsrBtScGetAssocModel(CsrUint8 localIoCaps, CsrUint8 remoteIoCaps)
{
    static const CsrUint8 authStage1[HCI_IO_CAP_MAX+1][HCI_IO_CAP_MAX+1] =
    {
        /* DISPLAY_ONLY,     DISPLAY_YES_NO,     KEYBOARD_ONLY,         NO_INPUT_NO_OUTPUT, KEYBOARD_DISPLAY */
        {MSG_TYPE_JUSTWORKS, MSG_TYPE_JUSTWORKS, MSG_TYPE_NOTIFICATION, MSG_TYPE_JUSTWORKS, MSG_TYPE_PASSKEY},  /* DISPLAY_ONLY */
        {MSG_TYPE_JUSTWORKS, MSG_TYPE_COMPARE,   MSG_TYPE_NOTIFICATION, MSG_TYPE_JUSTWORKS, MSG_TYPE_PASSKEY},  /* DISPLAY_YES_NO */
        {MSG_TYPE_PASSKEY,   MSG_TYPE_PASSKEY,   MSG_TYPE_PASSKEY,      MSG_TYPE_JUSTWORKS, MSG_TYPE_PASSKEY},  /* KEYBOARD_ONLY */
        {MSG_TYPE_JUSTWORKS, MSG_TYPE_JUSTWORKS, MSG_TYPE_JUSTWORKS,    MSG_TYPE_JUSTWORKS, MSG_TYPE_JUSTWORKS},/* NO_INPUT_NO_OUTPUT */
        {MSG_TYPE_JUSTWORKS, MSG_TYPE_PASSKEY,   MSG_TYPE_PASSKEY,      MSG_TYPE_JUSTWORKS, MSG_TYPE_PASSKEY}   /* KEYBOARD_DISPLAY */
    };

    if (remoteIoCaps <= HCI_IO_CAP_MAX &&
        localIoCaps <= HCI_IO_CAP_MAX)
    {
        return authStage1[localIoCaps][remoteIoCaps];
    }
    else
    {
        return MSG_TYPE_NONE;
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtScUpdateAssocModel
 *
 *  DESCRIPTION
 *      Determines which authentication association model, we'll be using.
 *
 *  RETURNS/MODIFIES
 *
 *----------------------------------------------------------------------------*/
void CsrBtScUpdateAssocModel(ScInstanceData_t *scData)
{
#ifdef CSR_BT_INSTALL_SC_OOB
    if (CsrBtScHasOobDataFor(scData, scData->deviceAddr) ||
        scData->sspData.remoteOobDataPresent)
#else
    if (scData->sspData.remoteOobDataPresent)
#endif
    {
        /* OOB will be used if either side has any OOB data */
        SC_CHANGE_MSGTYPE(&scData->pendingMsgType, MSG_TYPE_OOB);
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtScLookupPairedType
 *
 *  DESCRIPTION
 *
 *  RETURNS/MODIFIES
 *
 *----------------------------------------------------------------------------*/
CsrUint8 CsrBtScLookupPairedType(ScInstanceData_t *scData)
{
    CsrBtScDbDeviceRecord dbInfo;
    CsrBool dbRead;

    static const CsrUint8 pairingType[] =
        {
            CSR_BT_SC_PAIRING_LEGACY,  /* HCI_COMBINATION_KEY */
            CSR_BT_SC_PAIRING_LEGACY,  /* HCI_LOCAL_UNIT_KEY */
            CSR_BT_SC_PAIRING_LEGACY,  /* HCI_REMOTE_UNIT_KEY */
            CSR_BT_SC_PAIRING_LEGACY,  /* HCI_DEBUG_COMBINATION_KEY */
            CSR_BT_SC_PAIRING_NO_MITM, /* DM_SM_LINK_KEY_UNAUTHENTICATED */
            CSR_BT_SC_PAIRING_MITM,    /* DM_SM_LINK_KEY_AUTHENTICATED */
        };

    dbRead = CsrBtScDbRead(&scData->deviceAddr, &dbInfo);

    return (dbRead && dbInfo.linkkeyValid)
        ? pairingType[dbInfo.linkkeyType]
        : CSR_BT_SC_PAIRING_NONE;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      ScSetHciStatusIfUnset
 *
 *  DESCRIPTION
 *      Store the error code from DM_SM (HCI) if it's not already set.
 *
 *  RETURNS/MODIFIES
 *
 *----------------------------------------------------------------------------*/
void ScSetHciStatusIfUnset(ScInstanceData_t* scData, CsrUint8 hciStatus)
{
    if (scData->hciStatus == HCI_ERROR_UNSPECIFIED)
    {
        scData->hciStatus = hciStatus;
    }
}

/* For backward compability */
void CsrBtScDmSmLocalUserConfirmationRequestIndMap(ScInstanceData_t *inst,
                                                   CsrUint16 event, void *message)
{
    DM_SM_USER_CONFIRMATION_REQUEST_IND_T *dmPrim;
    dmPrim = (DM_SM_USER_CONFIRMATION_REQUEST_IND_T *) message;

    if ((event == DM_PRIM) &&
        (dmPrim->type == DM_SM_USER_CONFIRMATION_REQUEST_IND))
    {
        /* If we are using just works, it may not be required to
         * display anything to the user. This is to be called
         * 'local_only' mode */
        if(CsrBtScGetAssocModel(CsrBtScIoLimitier(inst, inst->bondData.localIoCaps),
                                CsrBtScIoLimitier(inst, inst->bondData.remoteIoCaps))
           == MSG_TYPE_JUSTWORKS)
    {
            dmPrim->type = DM_SM_LOCAL_USER_CONFIRMATION_REQUEST_IND;
        }
    }
}

static CsrUint32 csrBtScMapBondIndDetails(CsrBool authorised,
                                          CsrBtResultCode resultCode,
                                          CsrBtSupplier resultSupplier,
                                          CsrBool addedToScDbList)
{
    return ( ((resultCode == CSR_BT_RESULT_CODE_SC_SUCCESS &&
               resultSupplier == CSR_BT_SUPPLIER_SC && addedToScDbList)
              ? SD_UPDATE_DEVICE_BOND
              : SD_UPDATE_DEVICE_IGNORE) |
             (authorised
              ? SD_UPDATE_DEVICE_TRUSTED
              : 0) );
}

static CsrUint32 csrBtScMapBondCfmDetails(CsrBool authorised,
                                          CsrBtResultCode resultCode,
                                          CsrBtSupplier resultSupplier,
                                          CsrBool addedToScDbList)
{
    return ( ((resultCode == CSR_BT_RESULT_CODE_SC_SUCCESS &&
               resultSupplier == CSR_BT_SUPPLIER_SC &&
               addedToScDbList)
              ? SD_UPDATE_DEVICE_BOND
              : SD_UPDATE_DEVICE_DEBOND) |
             (authorised
              ? SD_UPDATE_DEVICE_TRUSTED
              : 0) );
}

CsrBtResultCode csrBtScMapScResult(ScInstanceData_t *scData)
{
    CsrBtResultCode result  = ((scData->bondData.scState == SC_CANCELLED)
                               ? CSR_BT_RESULT_CODE_SC_BONDING_CANCELLED
                               : CSR_BT_RESULT_CODE_SC_BONDING_FAILED);
    return result;
}

/* For code-reduction: Reduce the number of parameters */
void CsrBtScBondReqCancelHandlerAllowReset(ScInstanceData_t *scData, CsrSchedQid phandle)
{
    CsrBtScBondReqCancelHandler(scData, phandle, TRUE);
}
void CsrBtScBondReqCancelHandler(ScInstanceData_t *scData, CsrSchedQid phandle,
                                 CsrBool allowReset)
{
    switch (scData->state)
    {
        case SC_ST_LOCAL_POST_LISBON_BONDING:
            {
                CsrSchedQid appHandle = phandle;
                CsrBool lowEnergy     = FALSE;
                CsrBtScDbRemove(&scData->deviceAddr);
#ifdef CSR_BT_LE_ENABLE
                lowEnergy = CSR_BT_SC_IS_LOW_ENERGY(scData) && (scData->bondData.leAppHandle != CSR_SCHED_QID_INVALID);
                if(lowEnergy)
                {
                    appHandle = scData->bondData.leAppHandle;
                }
#endif
                CsrBtScNotifyUpperBondCfm(scData->authorised,
                                          appHandle,
                                          csrBtScMapScResult(scData),
                                          CSR_BT_SUPPLIER_SC,
                                          scData->hciStatus,
                                          scData->deviceAddr,
                                          scData->cod,
                                          FALSE,
                                          scData->bondData.bondAckSent,
                                          lowEnergy,
                                          scData->addressType);
                scData->bondData.bondAckSent = TRUE;
                if(allowReset)
                {
                    CsrBtScPairingFailedCleanupDeviceReset(scData);
                }
                break;
            }
        case SC_ST_REMOTE_BONDING:
            /* fallthrough */
        case SC_ST_REBOND:
            {
                CsrBtScNotifyUpperBondIndSimple(scData,
                                                phandle,
                                                csrBtScMapScResult(scData),
                                                scData->hciStatus,
                                                FALSE);
#ifdef CSR_BT_LE_ENABLE
                if(CSR_BT_SC_IS_LOW_ENERGY(scData) && (scData->bondData.leAppHandle != CSR_SCHED_QID_INVALID))
                {
                    CsrBtScLeSecurityCfmSend(CSR_BT_SC_LE_SECURITY_CFM,
                                             scData->bondData.leAppHandle,
                                             csrBtScMapScResult(scData),
                                             CSR_BT_SUPPLIER_SC,
                                             scData->deviceAddr,
                                             scData->addressType,
                                             FALSE);
                }
#endif
                scData->bondData.bondAckSent = TRUE;
                if(allowReset)
                {
                    CsrBtScPairingFailedCleanupDeviceReset(scData);
                }
                break;
            }
        default:
            { /* Just ignore it */
                break;
            }
    }
}

static void csrBtMapScHciResultCode(CsrBtResultCode *resultCode,
                                    CsrBtSupplier       *resultSupplier,
                                    hci_return_t        hciStatus)
{
    if ((*resultSupplier == CSR_BT_SUPPLIER_SC) &&
        (*resultCode != CSR_BT_RESULT_CODE_SC_SUCCESS) &&
        (hciStatus != HCI_ERROR_UNSPECIFIED) &&
        (hciStatus != HCI_SUCCESS))
    {
        /* Always use HCI result codes in case of failure, and only
         * allow SC SUCCESS to propagate to upper layer */
        *resultCode = (CsrBtResultCode) hciStatus;
        *resultSupplier = CSR_BT_SUPPLIER_HCI;
    }
}

void CsrBtScNotifyUpperBondIndSimple(ScInstanceData_t *scData,
                               CsrSchedQid         phandle,
                               CsrBtResultCode     resultCode,
                               hci_return_t        hciStatus,
                               CsrBool             addedToScDbList)             
{
    CsrBtScNotifyUpperBondInd(scData->authorised,
                              phandle,
                              scData->deviceAddr,
                              scData->cod,
                              resultCode,
                              CSR_BT_SUPPLIER_SC,
                              hciStatus,
                              addedToScDbList,
                              scData->bondData.bondAckSent,
                              CSR_BT_SC_IS_LOW_ENERGY(scData),
                              scData->addressType);
    
}
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
                               CsrBtAddressType    addrType)             
{
    if (!bondAckSent)
    {
        csrBtMapScHciResultCode(&resultCode, &resultSupplier, hciStatus);

#ifdef CSR_BT_LE_ENABLE
        if(lowEnergy)
        {
            CsrBtScLeSecurityCfmSend(CSR_BT_SC_LE_SECURITY_IND,
                                     phandle,
                                     resultCode,
                                     resultSupplier,
                                     addr,
                                     addrType,
                                     addedToScDbList);
        }
        else
#endif
        {
            CsrBtScBondIndSend(phandle, addr, cod, resultCode,
                               resultSupplier, addedToScDbList,
                               addrType);
        }
        CsrBtSdUpdateDeviceReqSendEx(addr, cod,
                                     csrBtScMapBondIndDetails(authorised,
                                                              resultCode,
                                                              resultSupplier,
                                                              addedToScDbList),
                                     addrType);
    }
}

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
                               CsrBtAddressType addrType)
{
    if (!bondAckSent)
    {
        csrBtMapScHciResultCode(&resultCode, &resultSupplier, hciStatus);
        
#ifdef CSR_BT_LE_ENABLE
        if(lowEnergy)
        {
            CsrBtScLeSecurityCfmSend(CSR_BT_SC_LE_SECURITY_CFM,
                                     phandle,
                                     resultCode,
                                     resultSupplier,
                                     addr,
                                     addrType,
                                     addedToScDbList);
        }
        else
#endif
        {
            CsrBtScBondCfmSend(phandle, resultCode, resultSupplier,
                               addr, cod, addedToScDbList,
                               addrType);
        }

        CsrBtSdUpdateDeviceReqSendEx(addr, cod,
                                     csrBtScMapBondCfmDetails(authorised,
                                                              resultCode,
                                                              resultSupplier,
                                                              addedToScDbList),
                                     addrType);
    }
}

void CsrBtScNotifyUpperDebondCfm(ScInstanceData_t *scData, CsrBtResultCode resultCode)
{
    CsrBtScDebondCfmSend(scData->appHandle, resultCode, CSR_BT_SUPPLIER_SC, scData->deviceAddr,
                         scData->addressType);
    CsrBtSdUpdateDeviceReqSendEx(scData->deviceAddr, scData->cod,
                                 (resultCode == CSR_BT_RESULT_CODE_SC_SUCCESS
                                  ? SD_UPDATE_DEVICE_DEBOND
                                  : SD_UPDATE_DEVICE_IGNORE),
                                 scData->addressType);
}

void CsrBtScDatabaseReqHandler(ScInstanceData_t *scData)
{
#ifdef CSR_AMP_ENABLE
    CsrBtScDbDeviceRecord dbInfo;
    CsrBtScDatabaseReq *prim = (CsrBtScDatabaseReq*)scData->recvMsgP;
    CsrUint8 *data;
    CsrUint8 keyType;
    CsrUint8 length;

    CsrMemSet(&dbInfo, 0, sizeof(CsrBtScDbDeviceRecord));

    if (!CsrBtScDbRead(&(prim->deviceAddr), &dbInfo))
    {
        /* Allow (valid) writes to create new entries */
        if((prim->opcode == CSR_BT_SC_DB_OP_WRITE) &&
           (prim->keyType != HCI_KEY_TYPE_UNKNOWN))
        {
            CsrMemSet(&dbInfo, 0, sizeof(CsrBtScDbDeviceRecord));
            CsrBtBdAddrCopy(&dbInfo.deviceAddr, &prim->deviceAddr);
            dbInfo.ampKeyGenericType = HCI_KEY_TYPE_UNKNOWN;
            dbInfo.ampKeyDot11Type = HCI_KEY_TYPE_UNKNOWN;
            dbInfo.ampKeyUwbType = HCI_KEY_TYPE_UNKNOWN;
        }
        else
        {
            /* Read or discard of nonexistant entry, abort */
            CsrBtScDatabaseCfmSend(prim->appHandle, &prim->deviceAddr, prim->opcode, prim->entry,
                                   HCI_KEY_TYPE_UNKNOWN,
                                   0, /* length */
                                   NULL, /* data */
                                   CSR_BT_RESULT_CODE_SC_NO_SUCH_DEVICE,
                                   prim->addressType);
            return;
        }
    }

    /* UWB is highest entry, WRITE is highest opcode */
    if((prim->entry > CSR_BT_SC_DB_ENTRY_AMP_KEY_UWB) ||
       (prim->opcode > CSR_BT_SC_DB_OP_WRITE))
    {
        CsrBtScDatabaseCfmSend(prim->appHandle, &prim->deviceAddr, prim->opcode, prim->entry,
                               HCI_KEY_TYPE_UNKNOWN,
                               0, /* length */
                               NULL, /* data */
                               CSR_BT_RESULT_CODE_SC_ERROR_UNSPECIFIED,
                               prim->addressType);
        return;
    }

    /* At this point the DB entry exists and both entry and opcode
     * have been verified - simply perform operation and return */
    length  = 0;
    data    = NULL;
    keyType = prim->keyType;

    if(prim->opcode == CSR_BT_SC_DB_OP_READ)
    {
        switch(prim->entry)
        {
            case CSR_BT_SC_DB_ENTRY_AMP_KEY_SSP:
                if(dbInfo.linkkeyValid)
                {
                    length  = dbInfo.linkkeyLen;
                    data    = dbInfo.Linkkey;
                    keyType = dbInfo.linkkeyType;
                }
                else
                {
                    length  = 0;
                    data    = NULL;
                    keyType = HCI_KEY_TYPE_UNKNOWN;
                }
                break;

            case CSR_BT_SC_DB_ENTRY_AMP_KEY_GAMP:
                length  = CSR_BT_AMP_LINK_KEY_SIZE_GAMP;
                data    = dbInfo.ampKeyGenericKey;
                keyType = dbInfo.ampKeyGenericType;
                break;

            case CSR_BT_SC_DB_ENTRY_AMP_KEY_DOT11:
                length  = CSR_BT_AMP_LINK_KEY_SIZE_802_11;
                data    = dbInfo.ampKeyDot11Key;
                keyType = dbInfo.ampKeyDot11Type;
                break;

            case CSR_BT_SC_DB_ENTRY_AMP_KEY_UWB:
                length  = CSR_BT_AMP_LINK_KEY_SIZE_ECMA_368;
                data    = dbInfo.ampKeyUwbKey;
                keyType = dbInfo.ampKeyUwbType;
                break;
        }
    }
    else
    {
        switch(prim->entry)
        {
            case CSR_BT_SC_DB_ENTRY_AMP_KEY_SSP:
                /* Do NOT allow AMP to write SSP key! */
                CsrBtScDatabaseCfmSend(prim->appHandle, &prim->deviceAddr, prim->opcode, prim->entry,
                                       HCI_KEY_TYPE_UNKNOWN,
                                       0, /* length */
                                       NULL, /* data */
                                       CSR_BT_RESULT_CODE_SC_INTERNAL_ERROR,
                                       prim->addressType);
                return;

            case CSR_BT_SC_DB_ENTRY_AMP_KEY_GAMP:
                if(prim->length == CSR_BT_AMP_LINK_KEY_SIZE_GAMP)
                {
                    dbInfo.ampKeyGenericType = prim->keyType;
                    CsrMemCpy(dbInfo.ampKeyGenericKey, prim->data, prim->length);
                }
                else
                {
                    dbInfo.ampKeyGenericType = HCI_KEY_TYPE_UNKNOWN;
                    CsrMemSet(dbInfo.ampKeyGenericKey, 0, CSR_BT_AMP_LINK_KEY_SIZE_GAMP);
                }
                break;

            case CSR_BT_SC_DB_ENTRY_AMP_KEY_DOT11:
                if(prim->length == CSR_BT_AMP_LINK_KEY_SIZE_802_11)
                {
                    dbInfo.ampKeyDot11Type = prim->keyType;
                    CsrMemCpy(dbInfo.ampKeyDot11Key, prim->data, prim->length);
                }
                else
                {
                    dbInfo.ampKeyDot11Type = HCI_KEY_TYPE_UNKNOWN;
                    CsrMemSet(dbInfo.ampKeyDot11Key, 0, CSR_BT_AMP_LINK_KEY_SIZE_802_11);
                }
                break;

            case CSR_BT_SC_DB_ENTRY_AMP_KEY_UWB:
                if(prim->length == CSR_BT_AMP_LINK_KEY_SIZE_ECMA_368)
                {
                    dbInfo.ampKeyUwbType = prim->keyType;
                    CsrMemCpy(dbInfo.ampKeyUwbKey, prim->data, prim->length);
                }
                else
                {
                    dbInfo.ampKeyUwbType = HCI_KEY_TYPE_UNKNOWN;
                    CsrMemSet(dbInfo.ampKeyUwbKey, 0, CSR_BT_AMP_LINK_KEY_SIZE_ECMA_368);
                }
                break;
        }

        /* Write operation */
        CsrBtScDbWrite(&dbInfo.deviceAddr, &dbInfo);
    }

    CsrBtScDatabaseCfmSend(prim->appHandle, &prim->deviceAddr, prim->opcode, prim->entry,
                           keyType, length, data,
                           CSR_BT_RESULT_CODE_SC_SUCCESS,
                           prim->addressType);

#else

    /* Non-AMP builds doesn't support this operation */
    CsrBtScDatabaseReq *prim = (CsrBtScDatabaseReq*)scData->recvMsgP;
    CsrBtScDatabaseCfmSend(prim->appHandle, &prim->deviceAddr,
                           prim->opcode, prim->entry,
                           HCI_KEY_TYPE_UNKNOWN,
                           0, /* length */
                           NULL, /* data */
                           CSR_BT_RESULT_CODE_SC_INTERNAL_ERROR,
                           prim->addressType);

#endif
}
