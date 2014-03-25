/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_cm_util.h"
#include "csr_bt_cm_main.h"
#include "csr_bt_cm_l2cap.h"
#include "csr_bt_cm_rfc.h"
#include "csr_bt_cm_dm.h"
#include "csr_bt_cm_bnep.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_cm_events_handler.h"
#include "hci_prim.h"

#ifndef EXCLUDE_CSR_BT_SCO_MODULE
#ifndef EXCLUDE_CSR_BT_RFC_MODULE
cmRfcConnElement * returnReserveScoIndexToThisAddress(cmInstanceData_t *cmData, CsrBtDeviceAddr deviceAddr)
{
    /* Try to find the index that match with the given device address,
     * and where the Sco index is reserved. If no match it return
     * ERROR */
    return(CsrBtCmRfcFindRfcConnElementFromDeviceAddrScoHandle(cmData, &deviceAddr, SCOBUSY_ACCEPT));
}

#endif /* EXCLUDE_CSR_BT_RFC_MODULE */
#endif /* EXCLUDE_CSR_BT_SCO_MODULE */

CsrUint8 returnConnectAbleParameters(cmInstanceData_t *cmData)
{
    /* Find out which connectable mode the device must be place into,
     * e.g HCI_SCAN_ENABLE_INQ_AND_PAGE, HCI_SCAN_ENABLE_PAGE,
     * HCI_SCAN_ENABLE_OFF or HCI_SCAN_ENABLE_INQ */
#ifndef EXCLUDE_CSR_BT_RFC_MODULE
    CsrBtConnId     btRfcConnId = CSR_BT_CONN_ID_INVALID;
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
    CsrBtConnId     btL2caConnId  = BTCONN_ID_RESERVED;
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
    CsrUintFast8 i;
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE */

    CsrUint8            mode = HCI_SCAN_ENABLE_OFF;

    if(cmData->globalState == notReady_s)
    {
        return CSR_BT_CM_DEFAULT_STARTUP_CONNECTABLE_MODE;
    }

    if (!cmData->dmVar.disableInquiryScan)
    {
        mode = HCI_SCAN_ENABLE_INQ;
    }

    if (cmData->dmVar.disablePageScan)
    {
        return mode;
    }

    if (cmData->dmVar.connectAblePhandle != CSR_SCHED_QID_INVALID)
    {
        mode = (CsrUint8) (mode | HCI_SCAN_ENABLE_PAGE);
        return mode;
    }

#ifndef EXCLUDE_CSR_BT_RFC_MODULE
    {
        cmRfcConnElement * theElement  = CM_FIND_RFC_ELEMENT(CsrBtCmRfcFindRfcConnElementFromBtConnId, &(btRfcConnId));
        if (theElement)
        {
            if (theElement->cmRfcConnInst->state != cancelConnectAble_s)
            {/* Do not enable scan page if we are cancelling it... */
                mode = (CsrUint8) (mode | HCI_SCAN_ENABLE_PAGE);
                return mode;
            }
        }
    }
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
    if(CM_FIND_L2CA_ELEMENT(CsrBtCmL2caFindL2caConnElementFromBtConnId, &btL2caConnId))
    {
        mode = (CsrUint8)(mode | HCI_SCAN_ENABLE_PAGE);
        return mode;
    }
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
    for ( i = 0; i < CSR_BT_MAX_NUM_OF_SIMULTANEOUS_BNEP_CONNECTIONS; i++ )
    { /* Goes through the bnep connection table */
        if( (cmData->bnepVar.connectVar[i].id == ID_RESERVED) &&
            (cmData->bnepVar.connectVar[i].state == bnepConnectAccept_s) )
        {
            mode = (CsrUint8)(mode | HCI_SCAN_ENABLE_PAGE);
            return mode;
        }
    }
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE */

    return mode;
}

link_policy_settings_t CsrBtCmDmReturnLinkPolicySettings(cmInstanceData_t *cmData, CsrBtDeviceAddr deviceAddr)
{
    link_policy_settings_t linkPolicySettings;
    CsrBtCmReturnLowPowerSettings(cmData, deviceAddr, &linkPolicySettings);
    linkPolicySettings |= CsrBtCmEnableMSSwitchParameter(cmData);

    return linkPolicySettings;
}

void CsrBtCmDmWriteLpSettingsForceLinkPolicyMsgSend(cmInstanceData_t *cmData, CsrBtDeviceAddr deviceAddr, CsrUint8 player, link_policy_settings_t linkPolicySettings)
{
    aclTable *aclConnectionElement;

    returnAclConnectionElement(cmData, deviceAddr, &aclConnectionElement);

    if (player != KEEP_CURRENT_PLAYER)
    {
        CM_CHANGE_PLAYER(&cmData->smVar.activePlayer, player);
    }

    if (aclConnectionElement)
    {
        aclConnectionElement->actualLinkPolicySettings = linkPolicySettings;
    }

    dm_hci_write_lp_settings(&deviceAddr, linkPolicySettings, NULL);
}

void CsrBtCmDmWriteLpSettingsAllMsgSend(cmInstanceData_t *cmData, CsrBtDeviceAddr deviceAddr, CsrUint8 player)
{
    link_policy_settings_t linkPolicySettings;
    CsrBtCmReturnLowPowerSettings(cmData, deviceAddr, &linkPolicySettings);
    linkPolicySettings |= CsrBtCmEnableMSSwitchParameter(cmData);
    CsrBtCmDmWriteLpSettingsForceLinkPolicyMsgSend(cmData, deviceAddr, player, linkPolicySettings);
}

void CsrBtCmDmWriteLpSettingsOnlyLowPowerMsgSend(cmInstanceData_t *cmData, CsrBtDeviceAddr deviceAddr, CsrUint8 player)
{
    link_policy_settings_t linkPolicySettings;
    CsrBtCmReturnLowPowerSettings(cmData, deviceAddr, &linkPolicySettings);
    CsrBtCmDmWriteLpSettingsForceLinkPolicyMsgSend(cmData, deviceAddr, player, linkPolicySettings);
}

void CsrBtCmDmWriteLpSettingsOnlyMssMsgSend(cmInstanceData_t *cmData, CsrBtDeviceAddr deviceAddr, CsrUint8 player)
{
    link_policy_settings_t linkPolicySettings;
    linkPolicySettings = CsrBtCmEnableMSSwitchParameter(cmData);
    CsrBtCmDmWriteLpSettingsForceLinkPolicyMsgSend(cmData, deviceAddr, player, linkPolicySettings);
}

void CsrBtCmDmWriteLpSettingsOverrideMssMsgSend(cmInstanceData_t *cmData, CsrBtDeviceAddr deviceAddr, CsrUint8 player, link_policy_settings_t mssSettings)
{
    link_policy_settings_t linkPolicySettings;
    CsrBtCmReturnLowPowerSettings(cmData, deviceAddr, &linkPolicySettings);
    linkPolicySettings |= mssSettings;
    CsrBtCmDmWriteLpSettingsForceLinkPolicyMsgSend(cmData, deviceAddr, player, linkPolicySettings);
}

#ifdef CSR_BT_INSTALL_CM_WRITE_VOICE_SETTINGS
static void csrBtCmWriteVoiceSettingsCfmMsgSend(CsrSchedQid appHandle, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    CsrBtCmWriteVoiceSettingsCfm   *prim = pnew(CsrBtCmWriteVoiceSettingsCfm);

    prim->type           = CSR_BT_CM_WRITE_VOICE_SETTINGS_CFM;
    prim->resultCode     = resultCode;
    prim->resultSupplier = resultSupplier;
    CsrBtCmPutMessage(appHandle, prim);
}

void CsrBtCmWriteVoiceSettingsReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmWriteVoiceSettingsReq    * prim;

    prim = (CsrBtCmWriteVoiceSettingsReq *) cmData->recvMsgP;

    cmData->dmVar.appHandle = prim->phandle;

    dm_hci_write_voice_setting(prim->voiceSettings, NULL);
}
#endif

void CsrBtCmDmHciWriteVoiceSettingCompleteHandler(cmInstanceData_t *cmData)
{ /* Write voice setting complete. If success initialise RFCOMM */
    DM_HCI_WRITE_VOICE_SETTING_CFM_T    *dmPrim;

    dmPrim = (DM_HCI_WRITE_VOICE_SETTING_CFM_T *)cmData->recvMsgP;
#ifdef CSR_BT_INSTALL_CM_WRITE_VOICE_SETTINGS
    if(cmData->globalState != notReady_s)
    {
        if (dmPrim->status == HCI_SUCCESS)
        {
            csrBtCmWriteVoiceSettingsCfmMsgSend(cmData->dmVar.appHandle,
                        CSR_BT_RESULT_CODE_CM_SUCCESS, CSR_BT_SUPPLIER_CM);
        }
        else
        {
            csrBtCmWriteVoiceSettingsCfmMsgSend(cmData->dmVar.appHandle,
                        (CsrBtResultCode) dmPrim->status, CSR_BT_SUPPLIER_HCI);
        }
        CsrBtCmDmRestoreQueueHandler(cmData);
    }
    else
#endif
    {
        if (dmPrim->status == HCI_SUCCESS)
        { /* Makes the local device visible */
            CsrUint8                        mode;
            CsrUint24 classOfDevice        = CsrBtCmReturnClassOfdevice(cmData);
            cmData->smVar.activePlayer    = CM_PLAYER;

            mode = returnConnectAbleParameters(cmData);
            if (classOfDevice != cmData->dmVar.codWrittenToChip)
            {
                cmData->dmVar.pendingCod = classOfDevice;
                dm_hci_write_class_of_device(classOfDevice, NULL);
            }
            else if (mode != cmData->dmVar.currentChipScanMode)
            {
                cmData->dmVar.pendingChipScanMode = mode;
                dm_hci_write_scan_enable(mode, NULL);
            }
            else
            {
                CsrBtCmWriteScanEnableCompleteSwitch(cmData, CSR_BT_RESULT_CODE_CM_SUCCESS, CSR_BT_SUPPLIER_CM);
            }
        }
        else
        { /* Fail, try again */
            dm_hci_write_voice_setting(CSR_BT_VOICE_SETTING, NULL);
        }
    }
}

static void csrBtCmDmAclCloseSuccessIndHandler(cmInstanceData_t *cmData, CsrBtDeviceAddr deviceAddr, hci_error_t reason)
{
#ifndef EXCLUDE_CSR_BT_RFC_MODULE
    cmRfcConnElement *currentRfcElem, *nextRfcElem;
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
    cmL2caConnElement *currentL2caElem;
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */

    aclTable            *aclConnectionElement;
    CsrUintFast8 i     = 0;

    CsrBtCmRemoveSavedIncomingConnectMessages(cmData, deviceAddr);
    returnAclConnectionElement(cmData, deviceAddr, &aclConnectionElement);

    if (aclConnectionElement)
    {
        bd_addr_zero(&(aclConnectionElement->deviceAddr));
        CsrBtCmDmAclRoleVarsClear(CsrBtCmDmGetAclRoleVars(aclConnectionElement));
        aclConnectionElement->sniffForceByAmp   = FALSE;
        aclConnectionElement->l2capExtendedFeatures = CM_INVALID_L2CAP_EXT_FEAT;

        if ((aclConnectionElement->noOfGuaranteedLogicalChannels != 0) ||
            (aclConnectionElement->logicalChannelTypeMask !=CSR_BT_NO_ACTIVE_LOGICAL_CHANNEL))
        {
            aclConnectionElement->noOfGuaranteedLogicalChannels = 0;
            aclConnectionElement->logicalChannelTypeMask        = CSR_BT_NO_ACTIVE_LOGICAL_CHANNEL;
            /* Issue event CSR_BT_CM_EVENT_MASK_SUBSCRIBE_CHANNEL_TYPE */
        }

        if (aclConnectionElement->rfcCloseTimerId != 0)
        {
            CsrSchedTimerCancel(aclConnectionElement->rfcCloseTimerId, NULL, NULL);
            aclConnectionElement->rfcCloseTimerId = 0;
        }
        else
        {
            ;
        }
        /* Check if this was caused by a detach request */
        CsrBtCmDmAclDetachAclCloseHandler(cmData, aclConnectionElement,
                                          deviceAddr, reason);
    }

    if (cmData->roleVar.alwaysSupportMasterRole)
    {
        ;
    }
    else
    {
        CsrUint8 numOfAcl = returnNumOfAclConnection(cmData);

        if (numOfAcl == 1 && cmData->roleVar.roleSwitchPerformed)
        { /* The local device has one ACL connection. Make sure that Role switch
             is supported */
            returnNextAvailableAclConnectionElement(cmData, &aclConnectionElement);
            CsrBtCmAssertRet(aclConnectionElement);
            CsrBtCmPrivateSwitchRoleReqSend(cmData->roleVar.aclVar[i].deviceAddr, CSR_BT_UNDEFINED_ROLE);
        }
    }

#ifndef EXCLUDE_CSR_BT_RFC_MODULE
    for (currentRfcElem = CM_RFC_GET_FIRST(cmData->rfcVar.connList); currentRfcElem; currentRfcElem = nextRfcElem)
    {
        nextRfcElem = currentRfcElem->next; /* Make a copy of the next pointer, since CsrBtCmConnectCfmMsgSend can free
                                               currentRfcElem. */
        if (currentRfcElem->cmRfcConnInst)
        {
            if(bd_addr_eq(&(deviceAddr), &(currentRfcElem->cmRfcConnInst->deviceAddr)))
            { /* If the device address match, the ACL connection is release without
                 receiving a RFC_RELEASE_IND */
                cmRfcConnInstType *theLogicalLink   = currentRfcElem->cmRfcConnInst;
                CsrBtCmDmSyncClearPcmSlotFromTable(cmData, &theLogicalLink->eScoParms);

                if(theLogicalLink->state == connected_s)
                { /* The above profile think it is still connected, need
                    to build and send CSR_BT_CM_DISCONNECT_IND signal to it.                    */
                    CsrBtCmDisconnectIndMsgCleanupSend(currentRfcElem,
                            (CsrBtReasonCode) reason, CSR_BT_SUPPLIER_HCI, TRUE, FALSE);
                }
                else if (theLogicalLink->state == connect_s)
                {
                    if (cmData->rfcVar.connectState == CM_RFC_SSP_REPAIR)
                    {
                        CsrBtCmSmCancelSppRepairInd(cmData);
                        CsrBtCmConnectCfmMsgSend(cmData, (CsrBtResultCode) reason, CSR_BT_SUPPLIER_HCI);
                    }
                    else
                    {
                        ;
                    }
                }
                else
                {
                    ;
                }
            }
        }
    }
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
    for (currentL2caElem = CM_L2CA_GET_FIRST(cmData->l2caVar.connList); currentL2caElem; currentL2caElem= currentL2caElem->next)
    { /* Search through the L2CAP connection list                     */
        if (currentL2caElem->cmL2caConnInst)
        {
            if( bd_addr_eq(&(deviceAddr), &(currentL2caElem->cmL2caConnInst->deviceAddr)))
            { /* If the device address match, the ACL connection is release without
                 receiving a L2CAP DISCONNECT_IND */
                cmL2caConnInstType *l2CaConnection = currentL2caElem->cmL2caConnInst;

                if(l2CaConnection->state == l2capConnected_s)
                { /* The above profile think it is still connected, need
                     to build and send CSR_BT_CM_DISCONNECT_IND signal to it. */
                    CsrBtCmL2capDisconnectIndMsgSend(cmData, currentL2caElem, (CsrBtReasonCode) reason, CSR_BT_SUPPLIER_HCI, FALSE);
                }
            }
        }
    }
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
    for ( i = 0; i < CSR_BT_MAX_NUM_OF_SIMULTANEOUS_BNEP_CONNECTIONS; i++ )
    { /* Go through the BNEP connection table */
        if( bd_addr_eq(&(deviceAddr), &cmData->bnepVar.connectVar[i].deviceAddr) )
        { /* If the device address match, the ACL connection is release without
             receiving a BNEP_DISCONNECT_IND */
            bnepTable * bnepConnection;

            bnepConnection    = &(cmData->bnepVar.connectVar[i]);
            if( bnepConnection->state == bnepConnected_s)
            { /* The above profile think it is still connected, need
                 to build and send CSR_BT_CM_BNEP_DISCONNECT_IND signal to it. */
                CsrBtCmBnepDisconnectIndMsgSend(bnepConnection->id,
                                                cmData->bnepVar.appHandle,
                                                (CsrBtReasonCode) reason,
                                                CSR_BT_SUPPLIER_HCI);

                CsrBtCmBnepClearBnepTableIndex(bnepConnection);
            }
        }
    }
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE */

    CsrBtCmSdcDecAclRefCountTo(cmData, deviceAddr);
}

static void csrBtCmDmAclConnHandleSuccessIndHandler(cmInstanceData_t *cmData, 
                                                    CsrBtDeviceAddr deviceAddr, 
                                                    CsrBool incoming, 
                                                    CsrUint24 cod,
                                                    CsrUint16 aclConnHandle)
{
    aclTable           *aclConnectionElement;
    CsrBtDeviceAddr        devAddr;

    returnAclConnectionElement(cmData, deviceAddr, &aclConnectionElement);

    CsrBtCmStateEventExceptionOn(aclConnectionElement, CSR_BT_CM_PRIM, DM_ACL_CONN_HANDLE_IND, 0);

    bd_addr_zero(&devAddr);
    returnAclConnectionElement(cmData, devAddr, &aclConnectionElement);

    if (aclConnectionElement)
    {
        CsrMemSet(aclConnectionElement->remoteFeatures, 0xFF, sizeof(aclConnectionElement->remoteFeatures));

        bd_addr_copy(&(aclConnectionElement->deviceAddr), &(deviceAddr));
        aclConnectionElement->remoteFeaturesValid      = FALSE;
        aclConnectionElement->roleChecked              = FALSE;
        aclConnectionElement->lmpVersion               = CSR_BT_CM_INVALID_LMP_VERSION;
        aclConnectionElement->actualLinkPolicySettings = DISABLE_ALL_LM_MODES;
        aclConnectionElement->linkPolicySettings       = cmData->dmVar.defaultLinkPolicySettings;
        aclConnectionElement->sniffSettings            = cmData->dmVar.defaultSniffSettings;
        aclConnectionElement->parkSettings             = cmData->dmVar.defaultParkSettings;
        /* Zero initialize (not valid) */
        CsrMemSet(&aclConnectionElement->curSsrSettings, 0, sizeof(aclConnectionElement->curSsrSettings));
        aclConnectionElement->encrypted                = FALSE;
        aclConnectionElement->flushTo                  = L2CA_FLUSH_TO_DEFAULT;
        aclConnectionElement->detachRequested          = FALSE;
        aclConnectionElement->lsto                     = CSR_BT_HCI_DEFAULT_LSTO;
        aclConnectionElement->mode                     = CSR_BT_ACTIVE_MODE;
        aclConnectionElement->interval                 = 0;
        aclConnectionElement->cod                      = cod;
        aclConnectionElement->incoming                 = incoming;
        aclConnectionElement->logicalChannelTypeMask   = CSR_BT_NO_ACTIVE_LOGICAL_CHANNEL;
        aclConnectionElement->noOfGuaranteedLogicalChannels = 0;
        aclConnectionElement->l2capExtendedFeatures    = CM_INVALID_L2CAP_EXT_FEAT;
        aclConnectionElement->sniffForceByAmp          = FALSE;
        aclConnectionElement->unsolicitedQosSetup      = FALSE;
        aclConnectionElement->aclConnHandle            = aclConnHandle;

        if (cmData->dmVar.appControlsAllLowPower)
        {
            aclConnectionElement->appControlsLowPower  = TRUE;
        }
        else
        {
            aclConnectionElement->appControlsLowPower  = FALSE;
        }

        CsrBtCmDmAclRoleVarsClear(CsrBtCmDmGetAclRoleVars(aclConnectionElement));

        if (incoming)
        {
            aclConnectionElement->role = CSR_BT_SLAVE_ROLE;
        }
        else
        {
            aclConnectionElement->role = CSR_BT_MASTER_ROLE;
        }
    }

    CsrBtCmReadRemoteFeaturesReqSend(CSR_BT_CM_IFACEQUEUE, deviceAddr);
    CsrBtCmReadRemoteVersionReqSend(CSR_BT_CM_IFACEQUEUE, deviceAddr);
    CsrBtCmRoleDiscoveryReqSend(CSR_BT_CM_IFACEQUEUE, deviceAddr);
}

static void csrBtCmDmHciRemoteNameFailedHandler(cmInstanceData_t *cmData, CsrBtDeviceAddr deviceAddr)
{
    if (bd_addr_eq(&deviceAddr, &cmData->dmVar.readNameDeviceAddr) &&
        cmData->dmVar.readingName == TRUE)
    {
        DM_HCI_REMOTE_NAME_CFM_T   *dmPrim;

        dmPrim          = zpnew(DM_HCI_REMOTE_NAME_CFM_T);
        dmPrim->type    = DM_HCI_REMOTE_NAME_CFM;
        dmPrim->phandle = CSR_BT_CM_IFACEQUEUE;
        dmPrim->status  = HCI_ERROR_PAGE_TIMEOUT;
        bd_addr_copy(&dmPrim->bd_addr, &deviceAddr);
        CsrSchedMessagePut(CSR_BT_CM_IFACEQUEUE, DM_PRIM, dmPrim);
    }
}

void CsrBtCmDmAclConnHandleIndHandler(cmInstanceData_t *cmData)
{
    DM_ACL_CONN_HANDLE_IND_T * dmPrim = (DM_ACL_CONN_HANDLE_IND_T *) cmData->recvMsgP;

    if ( dmPrim->status == HCI_SUCCESS)
    {
        csrBtCmDmAclConnHandleSuccessIndHandler(cmData, dmPrim->addrt.addr,
                                               (CsrBool)(dmPrim->flags & DM_ACL_FLAG_INCOMING ? TRUE : FALSE),
                                               dmPrim->dev_class,
                                               dmPrim->acl_conn_handle);
    }
    else
    {
        csrBtCmDmHciRemoteNameFailedHandler(cmData, dmPrim->addrt.addr);
    }
}

void CsrBtCmDmAclCloseIndHandler(cmInstanceData_t *cmData)
{
    DM_ACL_CLOSED_IND_T    * dmPrim;

    dmPrim = (DM_ACL_CLOSED_IND_T *) cmData->recvMsgP;

    csrBtCmDmAclCloseSuccessIndHandler(cmData,
                                       dmPrim->addrt.addr,
                                       dmPrim->reason);
}

CsrUint8 returnAclConnectionElement(cmInstanceData_t *cmData, CsrBtDeviceAddr devAddr, aclTable **aclConnectionElement)
{
    CsrUintFast8 i;

    for ( i = 0; i < NUM_OF_ACL_CONNECTION; i++)
    {
        if( bd_addr_eq(&devAddr, &(cmData->roleVar.aclVar[i].deviceAddr)))
        {
            *aclConnectionElement = &(cmData->roleVar.aclVar[i]);
            return (CsrUint8)i;
        }
    }
    *aclConnectionElement = NULL;
    return CM_ERROR;
}

void returnAclConnectionFromIndex(cmInstanceData_t *cmData, CsrUint8 index, aclTable **aclConnectionElement)
{
    *aclConnectionElement = &(cmData->roleVar.aclVar[index]);
}

void returnNextAvailableAclConnectionElement(cmInstanceData_t *cmData, aclTable **aclConnectionElement)
{
    CsrUintFast8 i;

    for ( i = 0; i < NUM_OF_ACL_CONNECTION; i++)
    {
        if (!CsrBtBdAddrEqZero(&cmData->roleVar.aclVar[i].deviceAddr))
        {
            *aclConnectionElement = &(cmData->roleVar.aclVar[i]);
            return;
        }
    }
    *aclConnectionElement = NULL;
}

void returnNextAclConnectionElement(cmInstanceData_t *cmData, aclTable **aclConnectionElement)
{
    CsrUintFast8 i;

    for ( i = 0; i < NUM_OF_ACL_CONNECTION; i++)
    {
        if (!CsrBtBdAddrEqZero(&(cmData->roleVar.aclVar[i].deviceAddr)) &&
            !cmData->roleVar.aclVar[i].roleChecked)
        {
            *aclConnectionElement = &(cmData->roleVar.aclVar[i]);
            return;
        }
    }
    *aclConnectionElement = NULL;
}

CsrUint8 returnNumOfAclConnection(cmInstanceData_t *cmData)
{
    CsrUintFast8 i, t = 0;

    for ( i = 0; i < NUM_OF_ACL_CONNECTION; i++)
    {
        if (!CsrBtBdAddrEqZero(&(cmData->roleVar.aclVar[i].deviceAddr)))
        {
            t++;
        }
    }
    return ((CsrUint8)t);
}


void CsrBtCmReturnLowPowerSettings(cmInstanceData_t *cmData, CsrBtDeviceAddr devAddr, link_policy_settings_t *linkPolicySettings)
{
    CsrUintFast8 i;

    for ( i = 0; i < NUM_OF_ACL_CONNECTION; i++)
    {
        if( bd_addr_eq(&devAddr, &(cmData->roleVar.aclVar[i].deviceAddr)))
        {
            *linkPolicySettings = cmData->roleVar.aclVar[i].linkPolicySettings;
            return;
        }
    }

    *linkPolicySettings = cmData->dmVar.defaultLinkPolicySettings;
}

void CsrBtCmDmSmEncryptionChangeHandler(cmInstanceData_t *cmData)
{
    aclTable                   * aclConnectionElement;
    DM_SM_ENCRYPTION_CHANGE_IND_T    * prim;

    prim = (DM_SM_ENCRYPTION_CHANGE_IND_T *) cmData->recvMsgP;

    returnAclConnectionElement(cmData,
                               prim->addrt.addr,
                               &aclConnectionElement);

    if (aclConnectionElement)
    {
        aclConnectionElement->encrypted = prim->encrypted;
#if (CSR_BT_DEFAULT_LINK_SUPERVISION_TIMEOUT != CSR_BT_HCI_DEFAULT_LSTO)
        if (aclConnectionElement->role == CSR_BT_MASTER_ROLE &&
            aclConnectionElement->lsto != CSR_BT_DEFAULT_LINK_SUPERVISION_TIMEOUT)
        {
            CsrBtCmWriteDmLinkSuperVisionTimeoutHandler(cmData,
                                                        CSR_BT_CM_IFACEQUEUE,
                                                        CSR_BT_DEFAULT_LINK_SUPERVISION_TIMEOUT,
                                                        prim->addrt.addr);
        }
#endif
    }
    else
    {
        ;
    }
}

#ifdef CSR_BT_INSTALL_SC_ENCRYPTION
CsrUint8 CsrBtCmDmSmEncryptCfmHandler(cmInstanceData_t *cmData)
{
    CsrUint8 status            = HCI_SUCCESS;
    DM_SM_ENCRYPT_CFM_T * prim = (DM_SM_ENCRYPT_CFM_T *) cmData->recvMsgP;

    if (prim->success)
    {
        aclTable               * aclConnectionElement;

        returnAclConnectionElement(cmData, prim->bd_addr, &aclConnectionElement);

        if (aclConnectionElement)
        {
            aclConnectionElement->encrypted = prim->encrypted;
        }
        else
        {
            ;
        }
    }
    else
    {
        status = HCI_ERROR_UNSPECIFIED;
    }

    prim->phandle = cmData->dmVar.appHandle;
    CsrBtCmDmLocalQueueHandler();
    CsrSchedMessagePut(cmData->scHandle, DM_PRIM, cmData->recvMsgP);
    cmData->recvMsgP = NULL;
    return (status);
}
#endif

static void csrBtCmSmSppRepairIndDelaySend(CsrUint8 dummy, cmInstanceData_t *cmData)
{
    if (cmData->dmVar.rebond.keyMissingTimerId != 0)
    {
        CsrBtCmSmRepairInd * prim;

        prim = (CsrBtCmSmRepairInd *) CsrPmemAlloc(sizeof(CsrBtCmSmRepairInd));

        cmData->dmVar.rebond.keyMissingTimerId = 0;
        cmData->dmVar.rebond.keyMissingId++;

        prim->type          = CSR_BT_CM_SM_REPAIR_IND;
        prim->deviceAddr    = cmData->dmVar.rebond.keyMissingDeviceAddr;
        prim->repairId      = cmData->dmVar.rebond.keyMissingId;
        prim->linkKeyType   = cmData->dmVar.rebond.linkKeyType;
        prim->addressType   = CSR_BT_ADDR_PUBLIC;
        CsrBtCmPutMessage(cmData->scHandle, prim);
    }
    else
    {
        ;
    }
    CSR_UNUSED(dummy);
}

void CsrBtCmSmSppRepairIndSend(cmInstanceData_t *cmData, CsrBtDeviceAddr deviceAddr)
{
    cmData->dmVar.rebond.keyMissingDeviceAddr  = deviceAddr;
    cmData->dmVar.rebond.keyMissingTimerId     = CsrSchedTimerSet(SSP_REPAIR_DELAY,
                    (void (*) (CsrUint16, void *)) csrBtCmSmSppRepairIndDelaySend, 0, (void *) cmData);
}

void CsrBtCmSmCancelSppRepairInd(cmInstanceData_t *cmData)
{
    if (cmData->dmVar.rebond.keyMissingTimerId != 0)
    {
        CsrSchedTimerCancel(cmData->dmVar.rebond.keyMissingTimerId, NULL, NULL);
        cmData->dmVar.rebond.keyMissingTimerId = 0;
    }
    else
    {
        ;
    }
    cmData->dmVar.rebond.keyMissingId++;
}

void CsrBtCmLogicalChannelTypeHandler(cmInstanceData_t *cmData)
{/* Update the RFC or L2CAP table and, if needed, the ACL table as well.
    Issue event if needed. */
    aclTable      *aclConnectionElement = NULL;
    CsrBtCmLogicalChannelTypeReq * prim = (CsrBtCmLogicalChannelTypeReq *)cmData->recvMsgP;

    if (CSR_BT_CONN_ID_IS_RFC(prim->btConnId))
    {/* RFC connection */
#ifndef EXCLUDE_CSR_BT_RFC_MODULE
        cmRfcConnElement * theElement = CM_FIND_RFC_ELEMENT(CsrBtCmRfcFindRfcConnElementFromBtConnId, &(prim->btConnId));
        if (theElement)
        { /* Update RFC table */
            theElement->cmRfcConnInst->logicalChannelTypeMask = prim->logicalChannelTypeMask;
        }
#endif
    }
    else
    {/* L2CAP */
#ifndef  EXCLUDE_CSR_BT_L2CA_MODULE
        cmL2caConnElement *theElement = CM_FIND_L2CA_ELEMENT(CsrBtCmL2caFindL2caConnElementFromBtConnId, &(prim->btConnId));
        if (theElement)
        { /* Update L2CAP table */
            theElement->cmL2caConnInst->logicalChannelTypeMask = prim->logicalChannelTypeMask;
        }
#endif
    }
    
    /* find ACL link */
    returnAclConnectionElement(cmData, prim->deviceAddr, &aclConnectionElement);

    if (aclConnectionElement)
    {
        if (updateLogicalChannelTypeMaskAndNumber(cmData,&aclConnectionElement->deviceAddr))
        {/* Changed: issue event */
#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_CHANNEL_TYPE
            CsrBtCmPropgateEvent(cmData,
                                CsrBtCmPropgateLogicalChannelTypeEvents,
                                CSR_BT_CM_EVENT_MASK_SUBSCRIBE_CHANNEL_TYPE,
                                HCI_SUCCESS,
                                (void *)aclConnectionElement,
                                NULL);
#endif
        }
    }
}

void CsrBtCmA2DPBitrateHandler(cmInstanceData_t *cmData)
{
#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_A2DP_BIT_RATE
    CsrBtCmPropgateEvent(cmData,
                        CsrBtCmPropagateA2DPBitRateEvents,
                        CSR_BT_CM_EVENT_MASK_SUBSCRIBE_A2DP_BIT_RATE,
                        HCI_SUCCESS,
                        (void *)cmData->recvMsgP,
                        NULL);
#endif
}
