/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_pmem.h"
#include "csr_sched.h"
#include "bluetooth.h"
#include "csr_bt_uuids.h"
#include "hci_prim.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_prim.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_cm_dm_sc_lib.h"
#include "csr_bt_cm_dm_sc_ssp_lib.h"
#include "csr_bt_sc_prim.h"
#include "csr_bt_sc_main.h"
#include "csr_bt_util.h"
#include "csr_bt_sc_lib_upstream.h"
#include "csr_bt_sc_utils.h"
#include "csr_bt_sc_le.h"
#include "csr_bt_sc_bond_unified.h"
#include "csr_bt_sc_bond_local.h"
#include "csr_bt_sc_bond_legacy.h"
#include "csr_bt_sc_ui.h"

#ifdef CSR_BT_LE_ENABLE

static CsrBool csrBtScLeSecurityLevelOk(CsrUint16 have,
                                        CsrUint16 want)
{
    if ((want & DM_SM_SECURITY_MITM_PROTECTION) &&
        !(have & DM_SM_SECURITY_MITM_PROTECTION))
    {
        return FALSE;
    }
    if ((want & DM_SM_SECURITY_ENCRYPTION)
        && !(have & DM_SM_SECURITY_ENCRYPTION))
    {
        return FALSE;
    }
    return TRUE;
}

static void csrBtScLeNotifyUpper(ScInstanceData_t *scData,
                                 CsrBtSupplier supplier,
                                 CsrBtResultCode result,
                                 CsrUint8 hciResult)
{
    /* CFM sent to requester */
    if(scData->bondData.leAppHandle != CSR_SCHED_QID_INVALID)
    {
        CsrBtScNotifyUpperBondCfm(scData->authorised,
                                  scData->bondData.leAppHandle,
                                  result,
                                  supplier,
                                  hciResult,
                                  scData->deviceAddr,
                                  scData->cod,
                                  scData->addDevice,
                                  scData->bondData.bondAckSent,
                                  CSR_BT_SC_IS_LOW_ENERGY(scData),
                                  CSR_BT_SC_ADDR_TYPE(scData));
    }

    /* IND sent to user app */
    if((scData->appHandle != CSR_SCHED_QID_INVALID)
       || (scData->mainAppHandle != CSR_SCHED_QID_INVALID))
    {
        CsrBtScNotifyUpperBondInd(scData->authorised,
                                  (CsrSchedQid)(scData->appHandle != CSR_SCHED_QID_INVALID
                                                ? scData->appHandle
                                                : scData->mainAppHandle),
                                  scData->deviceAddr,
                                  scData->cod,
                                  result,
                                  supplier,
                                  hciResult,
                                  scData->addDevice,
                                  scData->bondData.bondAckSent,
                                  CSR_BT_SC_IS_LOW_ENERGY(scData),
                                  CSR_BT_SC_ADDR_TYPE(scData));
    }
}

void CsrBtScLeSecurityCfmHandler(ScInstanceData_t *scData)
{
    DM_SM_SECURITY_CFM_T *dmPrim;
    dmPrim = (DM_SM_SECURITY_CFM_T*)scData->recvMsgP;

    if (CsrBtBdAddrEq(&dmPrim->addrt.addr, &scData->deviceAddr)
        && scData->addressType == dmPrim->addrt.type)
    {
        ScSetHciStatusIfUnset(scData, dmPrim->status);

        if (scData->bondData.scState == SC_CANCELLED)
        {
            /* If the app cancelled we don't care in which state we
             * are and if bonding succeeded or not. We stop here. */
            CsrBtScPairingCancelled(scData);
        }
        else if(scData->hciStatus != HCI_SUCCESS)
        {
            if ((scData->hciStatus == HCI_ERROR_KEY_MISSING) && (scData->bondData.leRebondCnt == 0) && (scData->bondData.leAppHandle != CSR_SCHED_QID_INVALID))
            {/* Rebond */
                scData->bondData.leRebondCnt++;
                
                scData->repairData.repairId  = scData->bondData.leRebondCnt;
                scData->appHandle            = scData->mainAppHandle;
                scData->restoreMsgFlag       = FALSE;
                scData->bondData.initiator   = INITIATOR_REBOND;

                SC_CHANGE_MSGTYPE(&scData->pendingMsgType, MSG_TYPE_REBOND);
                SC_CHANGE_STATE(&scData->state, SC_ST_REBOND);
                SC_CHANGE_STATE(&scData->bondData.scState, SC_ACTIVE);

                if ( CsrBtScEventIsEnabled(scData->eventMask, CSR_BT_SC_EVENT_MASK_REBOND))
                {
                    /* Ask user whether to accept rebonding */
                    CsrBtScStartUi(scData);
                }
                else
                { /* Auto accept */
                    SC_CHANGE_STATE(&scData->bondData.dmState, DM_PENDING);
                    SC_CHANGE_STATE(&scData->bondData.appState, APP_IDLE);
                    SC_CHANGE_STATE(&scData->bondData.uiState, UI_IDLE);
                    CsrBtCmScDmDeleteStoredLinkKeyReq(scData->deviceAddr, DELETE_BDADDR);
                }
            }
            else
            {
                csrBtScLeNotifyUpper(scData,
                                     CSR_BT_SUPPLIER_HCI,
                                     scData->hciStatus,
                                     scData->hciStatus);
                CsrBtScPairingSuccessUpdateDeviceReset(scData);
            }
        }
        else
        {
            switch(scData->pendingMsgType)
            {
                case MSG_TYPE_IO_CAPS:
                    /* Bonding failed before we even started */
                    CsrBtScBondReqCancelHandlerAllowReset(scData, scData->appHandle); /* allow reset */
                    break;

                case MSG_TYPE_OOB:
                    CsrBtScSmBondingCfmOobBondingS(scData);
                    break;

                case MSG_TYPE_LEGACY:
                case MSG_TYPE_JUSTWORKS:
                case MSG_TYPE_PASSKEY:
                case MSG_TYPE_COMPARE:
                case MSG_TYPE_NOTIFICATION:
                    SC_CHANGE_STATE(&scData->bondData.dmState, DM_BOND_COMPLETE);
                    SC_CHANGE_STATE(&scData->bondData.appState, APP_RESPOND);
                    scData->accept = TRUE;
                    scData->addDevice = TRUE;
                    /* fallthrough */

                case MSG_TYPE_NONE:
                    /* fallthrough */
                case MSG_TYPE_LE_ENCRYPTION_CHANGED:
                    csrBtScLeNotifyUpper(scData,
                                         CSR_BT_SUPPLIER_SC,
                                         CSR_BT_RESULT_CODE_SC_SUCCESS,
                                         HCI_SUCCESS);
                    CsrBtScPairingSuccessUpdateDeviceReset(scData);
                    break;

                case MSG_TYPE_REBOND:
                    /* Cannot occur. We only send a bonding req after bothering the user */
                default:
                    CsrBtScStateEventException(CSR_BT_SC_PRIM, scData->pendingMsgType,
                                               scData->bondData.dmState);
                    break;
            }
        }
    }
    else
    {
        CsrStateEventException("LE security req/cfm addr mismatch",
                               scData->pendingMsgType,
                               scData->state,
                               scData->bondData.dmState);
    }
}

void CsrBtScLeSecurityReqHandler(ScInstanceData_t *scData)
{
    CsrBtScLeSecurityReq *prim;
    prim = scData->recvMsgP;

    CsrBtAddrCopyToPublic(&scData->deviceAddr, &prim->address);
    scData->addressType = CsrBtAddrType(prim->address);

    /* LE security may be invoked indirectly, so see if we should
     * redirect app stuff */
    if(scData->appHandle == CSR_SCHED_QID_INVALID)
    {
        scData->appHandle = scData->mainAppHandle;
    }

    scData->bondData.leAppHandle = prim->phandle;
    scData->cod                 = 0;
    scData->restoreMsgFlag      = FALSE;
    scData->bondData.initiator = INITIATOR_BOND;
    scData->bondData.localInitiator = TRUE;
    scData->bondData.lowEnergy = TRUE; 
    scData->bondData.l2capFlags = prim->l2caConFlags;

    SC_CHANGE_STATE(&scData->bondData.dmState, DM_PENDING);
    SC_CHANGE_STATE(&scData->bondData.scState, SC_ACTIVE);
    SC_CHANGE_STATE(&scData->state, SC_ST_LOCAL_POST_LISBON_BONDING);

    if(prim->securityRequirements == CSR_BT_SC_LE_AUTH_IGNORE)
    {
        /* Use local auth */
        prim->securityRequirements = scData->bondData.leLocalAuth;
    }
    else
    {
        /* Override local auth */
        scData->bondData.leLocalAuth = prim->securityRequirements;
    }

    /* Key distribution toggles bonding-mode */
    if(scData->bondData.keyDistribution != 0)
    {
        prim->securityRequirements |= DM_SM_SECURITY_BONDING;
    }
    else
    {
        prim->securityRequirements &= ~DM_SM_SECURITY_BONDING;
    }

    CsrBtCmSmLeSecurityReqSend(prim->address,
                               prim->l2caConFlags,
                               0, /*context*/
                               prim->securityRequirements);
}

void CsrBtScLeSecurityIndHandler(ScInstanceData_t *scData)
{
    /*FIXME: Implement me */
}

static void csrBtScLeRemoteNameContinue(ScInstanceData_t *scData)
{
    switch(scData->bondData.uiState)
    {
        case UI_IDLE:
            /* ignore */
            break;

        case UI_RNR:
            CsrBtScRemoveLastPendingMessage(&scData->ignoreQueue);
            CsrBtScForwardPendingInd(scData, scData->friendlyName);
            SC_CHANGE_STATE(&scData->bondData.uiState, UI_COMPLETE);
            break;

        case UI_CANCELLED:
            CsrBtScRemoveLastPendingMessage(&scData->ignoreQueue);
            break;

        case UI_COMPLETE:
        case UI_READ_DEV:
        default:
            CsrBtScStateEventException(CSR_BT_CM_PRIM,
                                       VOIDP_TO_MSG_TYPE(scData),
                                       scData->bondData.uiState);
            break;
    }
}

static void csrBtScLeDisconnect(ScInstanceData_t *scData)
{
    if(scData->bondData.btConnId != CSR_BT_CONN_ID_INVALID)
    {
        CsrBtGattDisconnectReqSend(scData->gattId,
                                   scData->bondData.btConnId);
        scData->bondData.btConnId = CSR_BT_CONN_ID_INVALID;
    }
}

static void csrBtScLeCentralPeripCfmHandler(ScInstanceData_t *scData,
                                            CsrBtGattStdBtConnIdCfm *p)
{
    if(p->resultCode == CSR_BT_GATT_RESULT_SUCCESS
       && p->resultSupplier == CSR_BT_SUPPLIER_GATT)
    {
        scData->bondData.btConnId = p->btConnId;
        /* ConnectInd will trigger next step */
    }
    else
    {
        scData->bondData.btConnId = CSR_BT_CONN_ID_INVALID;
        csrBtScLeRemoteNameContinue(scData);
    }
}

static void csrBtScLeConnectIndHandler(ScInstanceData_t *scData,
                                       CsrBtGattConnectInd *p)
{
    if(p->resultCode == CSR_BT_GATT_RESULT_SUCCESS
       && p->resultSupplier == CSR_BT_SUPPLIER_GATT)
    {
        CsrBtGattReadBy16BitUuidReqSend(scData->gattId,
                                        scData->bondData.btConnId,
                                        0x0001, 0xffff, /* start,end */
                                        CSR_BT_GATT_DEVICE_NAME_UUID);
    }
    else
    {
        scData->bondData.btConnId = CSR_BT_CONN_ID_INVALID;
        csrBtScLeRemoteNameContinue(scData);
    }
}

void csrBtScLeReadByUuidIndHandler(ScInstanceData_t *scData,
                                   CsrBtGattReadByUuidInd *p)
{
    /* Store name and wait for read procedure to terminate */
    if(p->value[p->valueLength-1] != '\0')
    {
       CsrUint8 *value =  CsrPmemZalloc(p->valueLength+1);
        CsrMemCpy(value, p->value,p->valueLength);
        CsrBtScCopyFriendlyName(scData->friendlyName, value);
        CsrPmemFree(value);
    }
    else
    {
        CsrBtScCopyFriendlyName(scData->friendlyName, p->value);
    }
}

static void csrBtScLeReadByUuidCfmHandler(ScInstanceData_t *scData,
                                          CsrBtGattReadByUuidCfm *p)
{
    /* Either we got the name or we haven't. Continue */
    csrBtScLeDisconnect(scData);
    csrBtScLeRemoteNameContinue(scData);
}

static void csrBtScLeDisconnectIndHandler(ScInstanceData_t *scData,
                                          CsrBtGattDisconnectInd *p)
{
    scData->bondData.btConnId = CSR_BT_CONN_ID_INVALID;
    if (scData->bondData.uiState == UI_RNR)
    {
        csrBtScLeRemoteNameContinue(scData);
    }
    /* Else - ignore as csrBtScLeRemoteNameContinue has 
              already been called from 
              csrBtScLeReadByUuidCfmHandler */ 
}

void CsrBtScLeReadRemoteName(ScInstanceData_t *scData)
{
    l2ca_conflags_t cf;
    cf = ((scData->bondData.l2capFlags & L2CA_CONFLAG_ENUM_MASK)
          >> L2CA_CONFLAG_ENUM_OFFSET);
    CsrMemSet(scData->friendlyName, 0, sizeof(CsrBtDeviceName));

    if(scData->bondData.lowEnergy)
    {
        CsrBtTypedAddr ad;
        ad.type = scData->addressType;
        ad.addr = scData->deviceAddr;
        if((cf == L2CA_CONNECTION_LE_MASTER_DIRECTED)
           || (cf == L2CA_CONNECTION_LE_MASTER_WHITELIST))
        {
            /* Central/master */
            CsrBtGattCentralReqSend(scData->gattId, ad,
                                    CSR_BT_GATT_FLAGS_EXCLUSIVE,
                                    0);
        }
        else
        {
            /* Peripheral/slave */
            CsrBtGattPeripheralReqSend(scData->gattId, ad,
                                       CSR_BT_GATT_FLAGS_EXCLUSIVE,
                                       0);

        }
    }
    else
    {
        /* Traditional BR/EDR RnR */
        CsrBtCmReadRemoteNameReqSend(CSR_BT_SC_IFACEQUEUE,
                                     scData->deviceAddr);
    }
}

void CsrBtScLeReadRemoteNameCancel(ScInstanceData_t *scData)
{
    if(scData->bondData.lowEnergy)
    {
        csrBtScLeDisconnect(scData);
    }
    else
    {
        CsrBtCmCancelReadRemoteNameReqSend(CSR_BT_SC_IFACEQUEUE,
                                           scData->deviceAddr);
    }
}

void CsrBtScGattHandler(ScInstanceData_t *scData)
{
    switch(*(CsrUint16*)scData->recvMsgP)
    {
        case CSR_BT_GATT_REGISTER_CFM:
        {
            CsrBtGattRegisterCfm *p;
            p = (CsrBtGattRegisterCfm*)scData->recvMsgP;
            scData->gattId = p->gattId;
            break;
        }
        case CSR_BT_GATT_CENTRAL_CFM:
            /* fallthrough */
        case CSR_BT_GATT_PERIPHERAL_CFM:
        {
            CsrBtGattStdBtConnIdCfm *p;
            p = (CsrBtGattStdBtConnIdCfm*)scData->recvMsgP;
            csrBtScLeCentralPeripCfmHandler(scData, p);
            break;
        }

        case CSR_BT_GATT_CONNECT_IND:
        {
            CsrBtGattConnectInd *p;
            p = (CsrBtGattConnectInd*)scData->recvMsgP;
            csrBtScLeConnectIndHandler(scData, p);
            break;
        }

        case CSR_BT_GATT_DISCONNECT_IND:
        {
            CsrBtGattDisconnectInd *p;
            p = (CsrBtGattDisconnectInd*)scData->recvMsgP;
            csrBtScLeDisconnectIndHandler(scData, p);
            break;
        }

        case CSR_BT_GATT_READ_BY_UUID_IND:
        {
            CsrBtGattReadByUuidInd *p;
            p = (CsrBtGattReadByUuidInd*)scData->recvMsgP;
            csrBtScLeReadByUuidIndHandler(scData, p);
            break;
        }

        case CSR_BT_GATT_READ_BY_UUID_CFM:
        {
            CsrBtGattReadByUuidCfm *p;
            p = (CsrBtGattReadByUuidCfm*)scData->recvMsgP;
            csrBtScLeReadByUuidCfmHandler(scData, p);
            break;
        }
        case CSR_BT_GATT_SERVICE_CHANGED_IND:
            break;
        default:
        {
            CsrBtScStateEventException(CSR_BT_GATT_PRIM,
                                       *(CsrUint16*)scData->recvMsgP,
                                       scData->state);
            break;
        }
    }
}

void CsrBtScLeSspComplete(ScInstanceData_t *scData)
{
    DM_SM_SIMPLE_PAIRING_COMPLETE_IND_T *dmPrim;
    dmPrim = (DM_SM_SIMPLE_PAIRING_COMPLETE_IND_T*)scData->recvMsgP;
    
    /* Stop our watchdog timer */
    CsrBtScStopBondTimer(scData);

    if (dmPrim->status == HCI_SUCCESS)
    {
        /* ACL is already up. Go ahead. */
        if (scData->accept)
        {
            csrBtScLeNotifyUpper(scData,
                                 CSR_BT_SUPPLIER_SC,
                                 CSR_BT_RESULT_CODE_SC_SUCCESS,
                                 HCI_SUCCESS);
            CsrBtScPairingSuccessUpdateDeviceReset(scData);
        }
        else
        {
            CsrBtScBondReqCancelHandlerAllowReset(scData, scData->appHandle); /* allow reset */
        }
    }

}

void CsrBtScSmKeyRequestIndHandler(ScInstanceData_t *scData)
{
    DM_SM_KEY_REQUEST_IND_T *dmPrim;
    CsrBtScDbDeviceRecord devInfo;
    CsrUint16 keySize;
    CsrUint8 *key;
    CsrUint16 secreq;

    dmPrim = (DM_SM_KEY_REQUEST_IND_T*)scData->recvMsgP;

    /* Pessimistic defaults */
    keySize = 0;
    key = NULL;
    secreq = 0;

    if (CsrBtScDbRead(&(dmPrim->addrt.addr), &devInfo)
        && (devInfo.addressType == dmPrim->addrt.type))
    {
        secreq = devInfo.leSecurityRequirements;
        switch(dmPrim->key_type)
        {
            case DM_SM_KEY_ENC_BREDR:
                /* BR/EDR link key */
                if (devInfo.linkkeyValid
                    && csrBtScLeSecurityLevelOk(secreq, dmPrim->security_requirements))
                {
                    DM_SM_KEY_ENC_BREDR_T *bredr;
                    keySize = sizeof(DM_SM_KEY_ENC_BREDR_T);
                    bredr = CsrPmemAlloc(keySize);
                    bredr->link_key_type = devInfo.linkkeyType;
                    CsrMemCpy(bredr->link_key, devInfo.Linkkey, sizeof(CsrBtDeviceLinkkey));
                    key = (CsrUint8*)bredr;
                }
                break;

            case DM_SM_KEY_ENC_CENTRAL:
                /* LTK + EDIV + RAND for central */
                if ((devInfo.leKeyValid & CSR_BT_SC_DB_LE_KEY_ENC_CENTRAL)
                    && csrBtScLeSecurityLevelOk(secreq, dmPrim->security_requirements))
                {
                    DM_SM_KEY_ENC_CENTRAL_T *cen;
                    keySize = sizeof(DM_SM_KEY_ENC_CENTRAL_T);
                    cen = CsrPmemAlloc(keySize);
                    CsrMemCpy(cen, &(devInfo.leKeyEncCentral), sizeof(DM_SM_KEY_ENC_CENTRAL_T));
                    key = (CsrUint8*)cen;

                    /* Set our mode to 'waiting for encryption to
                     * start' now that the key is being re-used */
                    if (scData->state == SC_ST_IDLE)
                    {
                        SC_CHANGE_STATE(&scData->pendingMsgType, MSG_TYPE_LE_WAIT_ENCR_ENABLE);
                        scData->bondData.lowEnergy = TRUE; 
                    }
                }
                break;

            case DM_SM_KEY_DIV:
                /* DIV for peripheral. Must match the SM provided
                 * diversifier otherwise we can't recreate the key */
                if ((devInfo.leKeyValid & CSR_BT_SC_DB_LE_KEY_DIV)
                    && (dmPrim->diversifier == devInfo.leKeyDiv)
                    && csrBtScLeSecurityLevelOk(secreq, dmPrim->security_requirements))
                {
                    CsrUint16 *div;
                    keySize = sizeof(CsrUint16);
                    div = CsrPmemAlloc(keySize);
                    *div = devInfo.leKeyDiv;
                    key = (CsrUint8*)div;
                    
                    /* Set our mode to 'waiting for encryption to
                     * start' now that the key is being re-used */
                    if (scData->state == SC_ST_IDLE)
                    {
                        SC_CHANGE_STATE(&scData->pendingMsgType, MSG_TYPE_LE_WAIT_ENCR_ENABLE);
                        scData->bondData.lowEnergy = TRUE; 
                    }
                }
                /* else, failure mode already set */
                break;

            case DM_SM_KEY_DIV_CSRK:
                /* DIV for signing */
                if ((devInfo.leKeyValid & CSR_BT_SC_DB_LE_KEY_DIV)
                    && csrBtScLeSecurityLevelOk(secreq, dmPrim->security_requirements))
                {
                    CsrUint16 *div;
                    keySize = sizeof(CsrUint16);
                    div = CsrPmemAlloc(keySize);
                    *div = devInfo.leKeyDiv;
                    key = (CsrUint8*)div;
                }
                /* else, failure mode already set */
                break;

            case DM_SM_KEY_SIGN:
                /* CSRK */
                if ((devInfo.leKeyValid & CSR_BT_SC_DB_LE_KEY_SIGN)
                    && csrBtScLeSecurityLevelOk(secreq, dmPrim->security_requirements))
                {
                    DM_SM_KEY_SIGN_T *sign;
                    keySize = sizeof(DM_SM_KEY_SIGN_T);
                    sign = CsrPmemAlloc(keySize);
                    CsrMemCpy(sign, &(devInfo.leKeySign), keySize);
                    sign->counter = devInfo.leKeySignCounter;
                    key = (CsrUint8*)sign;
                }
                break;

            case DM_SM_KEY_ID:
                /* IRK */
                if ((devInfo.leKeyValid & CSR_BT_SC_DB_LE_KEY_ID)
                    && csrBtScLeSecurityLevelOk(secreq, dmPrim->security_requirements))
                {
                    DM_SM_KEY_ID_T *id;
                    keySize = sizeof(DM_SM_KEY_ID_T);
                    id = CsrPmemAlloc(keySize);
                    CsrMemCpy(id, &(devInfo.leKeyId), sizeof(devInfo.leKeyId));
                    key = (CsrUint8*)id;
                }
                break;

            case DM_SM_KEY_NONE:
                /* fallthrough */
            default:
                /* use default "none" values */
                break;
        }
    }
    else if (CsrBtAddrEqWithType(&(dmPrim->addrt), scData->addressType, &(scData->deviceAddr))
            && scData->state == SC_ST_REBOND)
    {
        secreq = scData->bondData.leLocalAuth;
    }

    scData->bondData.remoteAuth = dmPrim->security_requirements;
    CsrBtCmSmKeyRequestResSend(&(dmPrim->addrt.addr),
                               secreq,
                               (CsrUint16)dmPrim->key_type,
                               keySize, key,
                               dmPrim->addrt.type);
}

#ifdef CSR_BT_INSTALL_SC_LE_KEY_DISTRIBUTION
void CsrBtScLeKeyDistributionReqHandler(ScInstanceData_t *scData)
{
    CsrBtScLeKeyDistributionReq *prim;
    prim = (CsrBtScLeKeyDistributionReq*)scData->recvMsgP;
    scData->bondData.keyDistribution = prim->keyDistribution;
}
#endif /* CSR_BT_INSTALL_SC_LE_KEY_DISTRIBUTION */

void CsrBtScLeAddServiceChangedReqHandler(ScInstanceData_t *scData)
{
    CsrBtScDbDeviceRecord devInfo;
    CsrBtScLeAddServiceChangedReq * prim;

    prim = (CsrBtScLeAddServiceChangedReq *) scData->recvMsgP;

    if(CsrBtScDbRead(&prim->addr, &devInfo) &&
       prim->addressType == devInfo.addressType)
    {
        devInfo.serviceChangedHandle = prim->serviceChangedHandle;
        CsrBtScDbWrite(&devInfo.deviceAddr, &devInfo);
    }
    /* Else - Not found just ignore */
}

void CsrBtScSmKeysIndHandler(ScInstanceData_t *scData)
{
    DM_SM_KEYS_IND_T *dmPrim;
    CsrBtTypedAddr *peer;
    CsrUintFast8 i;
    CsrUint16 present;
    CsrBtScLeLinks *link;

    dmPrim = (DM_SM_KEYS_IND_T*)scData->recvMsgP;
    
    peer = (CsrBtTypedAddr*)(!CsrBtBdAddrEqZero(&dmPrim->id_addrt.addr)
                             ? &(dmPrim->id_addrt)
                             : &(dmPrim->addrt));

    CsrBtAddrCopyToPublic(&scData->deviceAddr, peer);
    scData->addressType = CsrBtAddrType(*peer);

    /* Add to encryption key size database */
    link = CsrPmemAlloc(sizeof(CsrBtScLeLinks));
    CsrBtAddrCopy(&(link->addr), peer);
    link->encryptionKeySize = dmPrim->keys.encryption_key_size;
    link->next = scData->leLinks;
    scData->leLinks = link;    

    /* Replace keys means delete all existing ones */
    if(!(dmPrim->keys.present & DM_SM_KEYS_UPDATE_EXISTING))
    {
        /* Invalidate all keys - preserver signing counter */
        scData->bondData.leKeyValid = (scData->bondData.leKeyValid
                                       & CSR_BT_SC_DB_LE_KEY_SIGN_COUNTER);

        CsrPmemFree(scData->bondData.leKeyEncCentral);
        scData->bondData.leKeyEncCentral = NULL;
        CsrPmemFree(scData->bondData.leKeyId);
        scData->bondData.leKeyId = NULL;
        CsrPmemFree(scData->bondData.leKeySign);
        scData->bondData.leKeySign = NULL;
        scData->bondData.leKeyDiv = 0;
        scData->bondData.leKeySignCounter = 0;
    }

    /* Store keys */
    scData->bondData.leSecurityRequirements = dmPrim->keys.security_requirements;
    scData->bondData.leKeySize = dmPrim->keys.encryption_key_size;

    for(i=0, present = dmPrim->keys.present;
        i<DM_SM_MAX_NUM_KEYS;
        i++, present >>= DM_SM_NUM_KEY_BITS)
    {
        switch(present & 0x7)
        {
            case DM_SM_KEY_NONE:
                /* no key, ignore */
                break;

            case DM_SM_KEY_ENC_BREDR:
                if(dmPrim->keys.u[i].enc_bredr != NULL)
                {
                    CsrMemCpy(scData->bondData.linkKey,
                              dmPrim->keys.u[i].enc_bredr->link_key,
                              SIZE_LINK_KEY);
                    scData->bondData.linkKeyType = (CsrUint8)dmPrim->keys.u[i].enc_bredr->link_key_type;
                }
                else
                {
                    CsrStateEventException("Missing BR/EDR key",
                                           present,
                                           dmPrim->keys.present,
                                           scData->bondData.dmState);
                }
                break;

            case DM_SM_KEY_ENC_CENTRAL:
                if(dmPrim->keys.u[i].enc_central != NULL)
                {
                    scData->bondData.leKeyValid |= CSR_BT_SC_DB_LE_KEY_ENC_CENTRAL;
                    if(scData->bondData.leKeyEncCentral == NULL)
                    {
                        scData->bondData.leKeyEncCentral = CsrPmemAlloc(sizeof(DM_SM_KEY_ENC_CENTRAL_T));
                    }
                    CsrMemCpy(scData->bondData.leKeyEncCentral,
                              dmPrim->keys.u[i].enc_central,
                              sizeof(DM_SM_KEY_ENC_CENTRAL_T));
                }
                else
                {
                    CsrStateEventException("Missing central key",
                                           present,
                                           dmPrim->keys.present,
                                           scData->bondData.dmState);
                }
                break;

            case DM_SM_KEY_DIV:
                scData->bondData.leKeyValid |= CSR_BT_SC_DB_LE_KEY_DIV;
                scData->bondData.leKeyDiv = dmPrim->keys.u[i].div;
                break;

            case DM_SM_KEY_SIGN:
                if(dmPrim->keys.u[i].sign != NULL)
                {
                    scData->bondData.leKeyValid |= CSR_BT_SC_DB_LE_KEY_SIGN;
                    if(scData->bondData.leKeySign == NULL)
                    {
                        scData->bondData.leKeySign = CsrPmemAlloc(sizeof(DM_SM_KEY_SIGN_T));
                    }
                    CsrMemCpy(scData->bondData.leKeySign,
                              dmPrim->keys.u[i].sign,
                              sizeof(DM_SM_KEY_SIGN_T));
                    scData->bondData.leKeySignCounter = dmPrim->keys.u[i].sign->counter;
                }
                else
                {
                    CsrStateEventException("Missing signing key",
                                           present,
                                           dmPrim->keys.present,
                                           scData->bondData.dmState);
                }
                break;

            case DM_SM_KEY_ID:
                if(dmPrim->keys.u[i].id != NULL)
                {
                    scData->bondData.leKeyValid |= CSR_BT_SC_DB_LE_KEY_ID;
                    if(scData->bondData.leKeyId == NULL)
                    {
                        scData->bondData.leKeyId = CsrPmemAlloc(sizeof(DM_SM_KEY_ID_T));
                    }
                    CsrMemCpy(scData->bondData.leKeyId,
                              dmPrim->keys.u[i].id,
                              sizeof(DM_SM_KEY_ID_T));
                }
                else
                {
                    CsrStateEventException("Missing ID key",
                                           present,
                                           dmPrim->keys.present,
                                           scData->bondData.dmState);
                }
                break;

            default:
                CsrStateEventException("Unknown key type",
                                       present,
                                       dmPrim->keys.present,
                                       scData->bondData.dmState);
                break;
        }
    }

    /* And 'update' to store/update  everything */
    CsrBtScScDbUpdate(scData);
}

/* Update local persistent ER/IR/div keys */
void CsrBtScLeUpdateLocalDbKeys(DM_SM_INIT_CFM_T *init)
{
    CsrBtScDbDeviceRecord db;
    CsrBtDeviceAddr addr;
    
    CsrMemSet(&db, 0, sizeof(CsrBtScDbDeviceRecord));
    CsrMemSet(&addr, 0xFF, sizeof(CsrBtDeviceAddr));

    /* Initialise local entry */
    if (!CsrBtScDbRead(&addr, &db))
    {
        CsrBtBdAddrCopy(&db.deviceAddr, &addr);
        db.addressType = 0xFF;
    }

    /* ER is stored in central LTK */
    CsrMemCpy(db.leKeyEncCentral.ltk,
              init->sm_key_state.er,
              sizeof(CsrUint16)*8);
    
    /* IR is stored in IRK */
    CsrMemCpy(db.leKeyId.irk,
              init->sm_key_state.ir,
              sizeof(CsrUint16)*8);
    
    db.leKeyDiv = init->sm_div_state;
    db.leKeySignCounter = init->sm_sign_counter;
    
    CsrBtScDbWrite(&db.deviceAddr, &db);
}

void CsrBtScDmLeRemoveDeviceCfmRebondS(ScInstanceData_t *scData)
{/* Debond done: Retry bonding.*/
    CsrBtTypedAddr deviceAddr;
    
    deviceAddr.addr = scData->deviceAddr;
    deviceAddr.type = scData->addressType;

    CsrBtCmSmLeSecurityReqSend(deviceAddr,
                               scData->bondData.l2capFlags,
                               0, /*context*/
                               scData->bondData.leLocalAuth);
}

/* Link encryption status changed. This may indicate a sucessful
 * pairing attempt when re-using a persistent key */
void CsrBtScLeEncryptionChangeHandler(ScInstanceData_t *scData)
{
    DM_SM_ENCRYPTION_CHANGE_IND_T *ind;
    ind = (DM_SM_ENCRYPTION_CHANGE_IND_T*)scData->recvMsgP;

    if (ind->encrypted == TRUE)
    {
        if ((scData->pendingMsgType == MSG_TYPE_NONE)
            && ((scData->state == SC_ST_LOCAL_POST_LISBON_BONDING)
                || (scData->state == SC_ST_REMOTE_BONDING)))
        {
            SC_CHANGE_MSGTYPE(&scData->pendingMsgType, MSG_TYPE_LE_ENCRYPTION_CHANGED);
        }
        else if (scData->pendingMsgType == MSG_TYPE_LE_WAIT_ENCR_ENABLE)
        {
            /* Diversified matched, and encryption now enabled, so LE
             * security succeeded */
            scData->deviceAddr  = ind->addrt.addr;
            scData->addressType = ind->addrt.type;

            csrBtScLeNotifyUpper(scData,
                                 CSR_BT_SUPPLIER_SC,
                                 CSR_BT_RESULT_CODE_SC_SUCCESS,
                                 HCI_SUCCESS);
            CsrBtScPairingSuccessUpdateDeviceReset(scData);
        }
    }
}

/* Signing counter update for local or remote device. Must write to
 * SC_DB immediately */
void CsrBtScLeCsrkCounterChangeIndHandler(ScInstanceData_t *scData)
{
    CsrBtScDbDeviceRecord db;
    DM_SM_CSRK_COUNTER_CHANGE_IND_T *csrk;
    
    csrk = (DM_SM_CSRK_COUNTER_CHANGE_IND_T*)scData->recvMsgP;

    CsrMemSet(&db, 0, sizeof(CsrBtScDbDeviceRecord));
    
    /* Ensure we use the correct NULL-local address */
    if(csrk->local_csrk)
    {
        CsrMemSet(&csrk->addrt, 0xFF, sizeof(CsrBtTypedAddr));
    }

    /* Initialise local entry */
    if (!CsrBtScDbRead(&CsrBtAddrAddr(csrk->addrt), &db))
    {
        CsrBtAddrCopyToPublic(&db.deviceAddr, &csrk->addrt);
        db.addressType = CsrBtAddrType(csrk->addrt);
    }

    db.leKeySignCounter = csrk->counter;

    CsrBtScDbWrite(&db.deviceAddr, &db);
}

/* LE ACL open/close */
void CsrBtScCmLeEventConnectionIndHandler(ScInstanceData_t *scData)
{
    CsrBtCmLeEventConnectionInd *prim;
    prim = (CsrBtCmLeEventConnectionInd*)scData->recvMsgP;

    if (prim->event == CSR_BT_CM_LE_MODE_OFF)
    {
        /* LE ACL dropped. Clear online DB for this device */
        CsrBtScLeLinks *cur;
        CsrBtScLeLinks **ppn;

        /* Double indirection. Search and destroy */
        for(ppn = &(scData->leLinks);
            (cur = *ppn) != NULL;
            ppn = &(cur->next))
        {
            if (CsrBtAddrEq(&(cur->addr), &(prim->deviceAddr)))
            {
                CsrBtScCmConnectionRejSecurityIndRemoteBondingS(scData);
                *ppn = cur->next;
                CsrPmemFree(cur);
                break;
            }
        }
    }
}

#endif /* CSR_BT_LE_ENABLE */
