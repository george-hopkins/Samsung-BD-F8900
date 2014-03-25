/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_bt_sc_main.h"
#include "csr_bt_util.h"
#include "dm_prim.h"
#include "csr_bt_sc_prim.h"
#include "csr_bt_cm_dm_sc_lib.h"
#include "csr_bt_cm_dm_sc_ssp_lib.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_sc_lib_upstream.h"
#include "csr_bt_sc_utils.h"
#include "csr_bt_sc_bond_unified.h"
#include "csr_bt_sc_bondable_mode.h"
#include "csr_bt_sc_bond_legacy.h"
#include "csr_bt_sc_bond_local.h"
#include "csr_bt_sc_bond_remote.h"
#include "csr_bt_sd_lib.h"
#include "csr_bt_sc_ui.h"
#include "csr_bt_sc_oob.h"
#include "csr_bt_sd_private_prim.h"
#include "csr_bt_sd_private_lib.h"
#include "csr_bt_sc_register.h"

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScBondReqIdleS
*
*  DESCRIPTION
*      Start dedicated bonding.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScBondReqIdleS(ScInstanceData_t *scData)
{
    CsrBtScBondReq  *scPrim;

    scPrim                     = (CsrBtScBondReq *)scData->recvMsgP;

    CsrBtBdAddrCopy(&scData->deviceAddr, &scPrim->deviceAddr);

    scData->appHandle               = scPrim->phandle;
    scData->cod                     = 0;
    scData->restoreMsgFlag          = FALSE;
    scData->bondData.initiator      = INITIATOR_BOND;
    scData->bondData.localInitiator = TRUE;
    /* needed to reset addr type if previous connection was LE private */
    scData->addressType             = 0;

    SC_CHANGE_STATE(&scData->bondData.dmState, DM_PENDING);
    SC_CHANGE_STATE(&scData->bondData.scState, SC_ACTIVE);
    SC_CHANGE_STATE(&scData->state, SC_ST_LOCAL_POST_LISBON_BONDING);
    CsrBtScBondReqIdlePostLisbonS(scData);
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScDmSmLinkKeyRequestIndIdleS
*
*  DESCRIPTION
*      See if we have the (proper) link key in the SC DB.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScDmSmLinkKeyRequestIndIdleS(ScInstanceData_t *scData)
{
    DM_SM_LINK_KEY_REQUEST_IND_T *dmPrim;
    CsrBtScDbDeviceRecord                devInfo;

    dmPrim = (DM_SM_LINK_KEY_REQUEST_IND_T *) scData->recvMsgP;

    if (CsrBtScDbRead(&dmPrim->bd_addr, &devInfo) &&
        CsrBtScCheckProperLinkKey(&devInfo, dmPrim))
    {
        CsrBtCmScDmSspLinkKeyRequestRes(dmPrim->bd_addr,
                                        devInfo.linkkeyType,
                                        CsrMemDup(devInfo.Linkkey, SIZE_LINK_KEY),
                                        CSR_BT_ADDR_PUBLIC); /* BR/EDR public only */
    }
    else
    {
        CsrBtCmScDmSspLinkKeyRequestNegRes(dmPrim->bd_addr,
                                           CSR_BT_ADDR_PUBLIC);/* BR/EDR public only */
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScDmSmLinkKeyIndIdleS
*
*  DESCRIPTION
*      Handle a link key due to a HCI_CHANGE_CONN_LINK_KEY request issued
*      by one of our connected devices.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScDmSmLinkKeyIndIdleS(ScInstanceData_t *scData)
{
    DM_SM_LINK_KEY_IND_T *dmPrim;
    CsrBtScDbDeviceRecord  scDbDeviceInfo;

    CsrBtScMapToDmKeyType(scData);

    CsrMemSet(&scDbDeviceInfo, 0, sizeof(CsrBtScDbDeviceRecord));
    dmPrim = (DM_SM_LINK_KEY_IND_T *) scData->recvMsgP;

    if (CsrBtScDbRead(&dmPrim->bd_addr,&scDbDeviceInfo))
    {
        /* The device is already place in the trusted device list. Update the key */
        CsrMemCpy(&scDbDeviceInfo.Linkkey, &dmPrim->key, SIZE_LINK_KEY);
        scDbDeviceInfo.linkkeyLen   = SIZE_LINK_KEY;
        scDbDeviceInfo.linkkeyValid = TRUE;
        scDbDeviceInfo.linkkeyType  = dmPrim->key_type;
        CsrBtScDbWrite(&scDbDeviceInfo.deviceAddr, &scDbDeviceInfo);
    }
    else
    { /* If a legacy pairing procedure receive a CSR_BT_CM_CONNECTION_REJ_SECURITY_IND
         message just after the SC has send a CSR_BT_CM_SM_PIN_REQUEST_RES the SC will
         receive a DM_SM_LINK_KEY_IND message in idle state. Therefor this message
         must be ignore                                                     */
        CSR_LOG_TEXT_WARNING((LOG_TEXT_TASK_ID, LOG_TEXT_SO_SCDB,
                              "IGNORE %04X:%02X:%06X",
                              scDbDeviceInfo.deviceAddr.nap,
                              scDbDeviceInfo.deviceAddr.uap,
                              scDbDeviceInfo.deviceAddr.lap));
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScSmIoCapabilityRequestIndDmPendingS
*
*  DESCRIPTION
*
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScSmIoCapabilityRequestIndDmPendingS(ScInstanceData_t *scData)
{
    DM_SM_IO_CAPABILITY_REQUEST_IND_T *dmPrim;

    dmPrim = (DM_SM_IO_CAPABILITY_REQUEST_IND_T *) scData->recvMsgP;

    SC_CHANGE_STATE(&scData->bondData.dmState, DM_IO_CAPS_RECV);
    CsrBtScSetCod(scData, dmPrim->dev_class);
    CsrBtScUpdateAssocModel(scData);

#ifdef CSR_BT_LE_ENABLE
    scData->bondData.lowEnergy = (dmPrim->flags & DM_SM_FLAGS_SECURITY_MANAGER
                                  ? TRUE : FALSE);
#endif
    scData->bondData.localInitiator = (CsrBool)((dmPrim->flags & DM_SM_FLAGS_INITIATOR)
                                                ? TRUE
                                                : FALSE);

    if (CsrBtScEventIsEnabled(scData->eventMask, CSR_BT_SC_EVENT_MASK_PAIR))
    {
        SC_CHANGE_MSGTYPE(&scData->pendingMsgType, MSG_TYPE_IO_CAPS);
        CsrBtScStartUi(scData);
    }
    else
    {
        CsrBtCmScDmIoCapabilityRequestRes(dmPrim->addrt.addr,
                                          CsrBtScIoLimitier(scData, scData->bondData.localIoCaps),
                                          CsrBtScGetCompatibleAuthRequirement(scData),
#ifdef CSR_BT_INSTALL_SC_OOB
                                          CsrBtScHasOobDataFor(scData, dmPrim->addrt.addr),
#else
                                          FALSE,
#endif
                                          scData->sspData.oobHashC,
                                          scData->sspData.oobRandR,
                                          dmPrim->addrt.type,
                                          CSR_BT_SC_KEY_DIST(scData));
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScSmIoCapabilityResponseIndBondingS
*
*  DESCRIPTION
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScSmIoCapabilityResponseIndBondingS(ScInstanceData_t *scData)
{
    DM_SM_IO_CAPABILITY_RESPONSE_IND_T *dmPrim;

    dmPrim = (DM_SM_IO_CAPABILITY_RESPONSE_IND_T *) scData->recvMsgP;

    if (CsrBtBdAddrEq(&scData->deviceAddr, &dmPrim->addrt.addr)
        && scData->addressType == dmPrim->addrt.type)
    {
#ifdef CSR_BT_LE_ENABLE
        scData->bondData.lowEnergy = (dmPrim->flags & DM_SM_FLAGS_SECURITY_MANAGER
                                      ? TRUE : FALSE);
#endif
        scData->bondData.remoteAuth          = dmPrim->authentication_requirements;
        scData->bondData.remoteIoCaps        = dmPrim->io_capability;
        scData->sspData.remoteOobDataPresent = dmPrim->oob_data_present;

        CsrBtScUpdateAssocModel(scData);
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScSspPairingResBondingS
*
*  DESCRIPTION
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScSspPairingResBondingS(ScInstanceData_t *scData)
{
    CsrBtScSspPairingRes *scPrim;

    scPrim = (CsrBtScSspPairingRes *) scData->recvMsgP;

    if (scData->bondData.appState == APP_RESPOND &&
        CsrBtBdAddrEq(&scData->deviceAddr, &scPrim->deviceAddr)
        && scData->addressType == scPrim->addressType)
    {
        SC_CHANGE_STATE(&scData->bondData.appState, APP_IDLE);
        SC_CHANGE_STATE(&scData->bondData.uiState, UI_IDLE);

        /* Stop watchdog timer */
        CsrBtScStopBondTimer(scData);

        CsrBtScUpdateAssocModel(scData);

        if (scData->bondData.scState == SC_CANCELLED || !scPrim->accept)
        {
            CsrBtCmScDmIoCapabilityRequestNegRes(scPrim->deviceAddr,
                                                 HCI_ERROR_HOST_BUSY_PAIRING,
                                                 scPrim->addressType);
        }
        else
        {
            /* Automatic accept (due to OOB where the app might not receive an indication) */
            scData->accept     = TRUE;
            scData->authorised = scPrim->authorised;
            scData->addDevice  = scPrim->addDevice;
            CsrBtCmScDmIoCapabilityRequestRes(scPrim->deviceAddr,
                                              CsrBtScIoLimitier(scData, scData->bondData.localIoCaps),
                                              CsrBtScGetCompatibleAuthRequirement(scData),
#ifdef CSR_BT_INSTALL_SC_OOB
                                              CsrBtScHasOobDataFor(scData, scPrim->deviceAddr),
#else
                                              FALSE,
#endif
                                              scData->sspData.oobHashC,
                                              scData->sspData.oobRandR,
                                              scPrim->addressType,
                                              CSR_BT_SC_KEY_DIST(scData));
        }
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScDmSmUserConfirmationRequestIndBondingS
*
*  DESCRIPTION
*      The DM has requested us to compare a numeric value.
*      Both locally and remotely initiated SSP comparations end
*      up here just before we notify the user.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScDmSmUserConfirmationRequestIndBondingS(ScInstanceData_t *scData)
{
    DM_SM_USER_CONFIRMATION_REQUEST_IND_T *dmPrim;

    dmPrim = (DM_SM_USER_CONFIRMATION_REQUEST_IND_T *) scData->recvMsgP;

    SC_CHANGE_STATE(&scData->bondData.dmState, DM_AUTHEN_RECV);
    scData->bondData.numValue = dmPrim->numeric_value;

    /* COMPARE is also used for JUST_WORKS. Detect which is really
     * supposed to be used by looking at the effecive IO capabilities;
     * If there is no display support, we use just works */
    if(((dmPrim->io_cap_local != HCI_IO_CAP_DISPLAY_ONLY) &&
        (dmPrim->io_cap_local != HCI_IO_CAP_DISPLAY_YES_NO)) ||
       ((dmPrim->io_cap_remote != HCI_IO_CAP_DISPLAY_ONLY) &&
        (dmPrim->io_cap_remote != HCI_IO_CAP_DISPLAY_YES_NO)))
    {
        SC_CHANGE_MSGTYPE(&scData->pendingMsgType, MSG_TYPE_JUSTWORKS);
    }
#ifdef CSR_BT_LE_ENABLE
    /* Low energy does not have compare, so this will be a just-works */
    else if(scData->bondData.lowEnergy)
    {
        SC_CHANGE_MSGTYPE(&scData->pendingMsgType, MSG_TYPE_JUSTWORKS);        
    }    
#endif
    else
    {
        SC_CHANGE_MSGTYPE(&scData->pendingMsgType, MSG_TYPE_COMPARE);
    }


    CsrBtScSetCod(scData, dmPrim->dev_class);
    CsrBtScStartUi(scData);
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScDmSmLocalUserConfirmationRequestIndBondingS
*
*  DESCRIPTION
*       Do local confirmation
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScDmSmLocalUserConfirmationRequestIndBondingS(ScInstanceData_t *scData)
{
    DM_SM_USER_CONFIRMATION_REQUEST_IND_T *dmPrim;

    dmPrim = (DM_SM_USER_CONFIRMATION_REQUEST_IND_T *) scData->recvMsgP;

    SC_CHANGE_STATE(&scData->bondData.dmState, DM_AUTHEN_RECV);
    SC_CHANGE_MSGTYPE(&scData->pendingMsgType, MSG_TYPE_JUSTWORKS);

    CsrBtScSetCod(scData, dmPrim->dev_class);
    CsrBtScStartUi(scData);
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScDmSmUserPasskeyRequestIndBondingS
*
*  DESCRIPTION
*      The DM has requested us to provide a passkey.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScDmSmUserPasskeyRequestIndBondingS(ScInstanceData_t *scData)
{
    DM_SM_USER_PASSKEY_REQUEST_IND_T *dmPrim;

    dmPrim = (DM_SM_USER_PASSKEY_REQUEST_IND_T *) scData->recvMsgP;

    SC_CHANGE_STATE(&scData->bondData.dmState, DM_AUTHEN_RECV);
    SC_CHANGE_MSGTYPE(&scData->pendingMsgType, MSG_TYPE_PASSKEY);

    CsrBtScSetCod(scData, dmPrim->dev_class);
    CsrBtScStartUi(scData);
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScDmSmUserPasskeyNotificationIndBondingS
*
*  DESCRIPTION
*      The DM has requested us to show a passkey.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScDmSmUserPasskeyNotificationIndBondingS(ScInstanceData_t *scData)
{
    DM_SM_USER_PASSKEY_NOTIFICATION_IND_T *dmPrim;

    dmPrim = (DM_SM_USER_PASSKEY_NOTIFICATION_IND_T *) scData->recvMsgP;

    SC_CHANGE_STATE(&scData->bondData.dmState, DM_AUTHEN_RECV);
    SC_CHANGE_MSGTYPE(&scData->pendingMsgType, MSG_TYPE_NOTIFICATION);
    scData->bondData.numValue = dmPrim->passkey;

    CsrBtScSetCod(scData, dmPrim->dev_class);
    CsrBtScStartUi(scData);
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScDmSmPinRequestIndIdleS
*
*  DESCRIPTION
*      The DM has requested a legacy pin code, due to our dedicated
*      bonding request.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScDmSmPinRequestIndBondingS(ScInstanceData_t *scData)
{
    DM_SM_PIN_REQUEST_IND_T *dmPrim;

    dmPrim = (DM_SM_PIN_REQUEST_IND_T *)scData->recvMsgP;

    if (!CsrBtBdAddrEq(&dmPrim->addrt.addr, &scData->deviceAddr))
    {
        CsrBtCmScDmPinRequestNegRes(dmPrim->addrt.addr);
    }
    else
    {
        SC_CHANGE_MSGTYPE(&scData->pendingMsgType, MSG_TYPE_LEGACY);
        SC_CHANGE_STATE(&scData->bondData.dmState, DM_AUTHEN_RECV);
        CsrBtScSetCod(scData, dmPrim->dev_class);
        CsrBtScStartUi(scData);
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScSspPasskeyResBondingS
*
*  DESCRIPTION
*      The app has replied with a passkey.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScSspPasskeyResBondingS(ScInstanceData_t *scData)
{
    CsrBtScSspPasskeyRes *scPrim;

    scPrim = (CsrBtScSspPasskeyRes *) scData->recvMsgP;

    if (scData->bondData.appState == APP_RESPOND &&
        CsrBtBdAddrEq(&scData->deviceAddr, &scPrim->deviceAddr)
        && scData->addressType == scPrim->addressType)
    {
        scData->addDevice   = scPrim->addDevice;
        scData->authorised = scPrim->authorised;
        scData->accept      = scPrim->accept;

        /* Stop watchdog timer */
        CsrBtScStopBondTimer(scData);

        if (!scPrim->accept)
        {
            CsrBtCmScDmUserPasskeyRequestNegRes(scData->deviceAddr,
                                                scData->addressType);
            
            /* If this is security over LE, the SM will send us the
             * final DM_SM_SECURITY_CFM later in the process */
            if(!CSR_BT_SC_IS_LOW_ENERGY(scData))
            {
                CsrBtScBondReqCancelHandlerAllowReset(scData, scData->mainAppHandle); /* allow reset */
            }
        }
        else
        {
            CsrBtCmScDmUserPasskeyRequestRes(scData->deviceAddr,
                                             scPrim->numericValue,
                                             scData->addressType);
        }

        SC_CHANGE_STATE(&scData->bondData.appState, APP_RESPONDED);
        SC_CHANGE_STATE(&scData->bondData.dmState, DM_AUTHEN_RESP);
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScSspNotificationResBondingS
*
*  DESCRIPTION
*      The app has replied on a passkey notification. Note that
*      we require that the app display the numeric value _while_
*      the pairing takes place.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScSspNotificationResBondingS(ScInstanceData_t *scData)
{
    CsrBtScSspNotificationRes *scPrim;

    scPrim = (CsrBtScSspNotificationRes *) scData->recvMsgP;

    if (scData->bondData.appState == APP_RESPOND &&
        CsrBtBdAddrEq(&scData->deviceAddr, &scPrim->deviceAddr)
        && scData->addressType == scPrim->addressType)
    {
        scData->addDevice   = scPrim->addDevice;
        scData->authorised = scPrim->authorised;
        scData->accept      = scPrim->accept;

        /* Stop watchdog timer */
        CsrBtScStopBondTimer(scData);

        /* Passkey entry association model doesn't give us much to work
         * with, so if the app has cancelled or rejected the pairing,
         * we can't do much about it. We simply have to wait it out. */
        SC_CHANGE_STATE(&scData->bondData.appState, APP_RESPONDED);
        SC_CHANGE_STATE(&scData->bondData.dmState, DM_AUTHEN_RESP);

        if (!scPrim->accept)
        {
            /* Detach the ACL if no connections are using it */
            CsrBtCmAclDetachReqSend(CSR_BT_SC_IFACEQUEUE, scPrim->deviceAddr, CSR_BT_CM_ACL_DETACH_EXCLUDE_ALL);
            CsrBtScBondReqCancelHandlerAllowReset(scData, scData->mainAppHandle); /* allow reset */
        }
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScSspCompareResBondingS
*
*  DESCRIPTION
*      The app has compare a numeric value.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScSspCompareResBondingS(ScInstanceData_t *scData)
{
    CsrBtScSspCompareRes *scPrim;

    scPrim = (CsrBtScSspCompareRes *) scData->recvMsgP;

    if (scData->bondData.appState == APP_RESPOND &&
        CsrBtBdAddrEq(&scData->deviceAddr, &scPrim->deviceAddr)
        && scData->addressType == scPrim->addressType)
    {
        scData->addDevice   = scPrim->addDevice;
        scData->authorised  = scPrim->authorised;
        scData->accept      = scPrim->accept;
        /* Stop watchdog timer */
        CsrBtScStopBondTimer(scData);

        if (!scPrim->accept)
        {
            CsrBtCmScDmUserConfirmationRequestNegRes(scData->deviceAddr,
                                                     scData->addressType);
            CsrBtScBondReqCancelHandlerAllowReset(scData, scData->mainAppHandle); /* allow reset */
        }
        else
        {
            CsrBtCmScDmUserConfirmationRequestRes(scData->deviceAddr,
                                                  scData->addressType);
        }
        SC_CHANGE_STATE(&scData->bondData.appState, APP_RESPONDED);
        SC_CHANGE_STATE(&scData->bondData.dmState, DM_AUTHEN_RESP);
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScSspKeypressNotificationReqBondingS
*
*  DESCRIPTION
*      The app has requested to notify the remote side that we
*      are currently entering a passkey. Simply forward it.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScSspKeypressNotificationReqBondingS(ScInstanceData_t *scData)
{
    CsrBtScSspKeypressNotificationReq *scPrim;

#ifdef CSR_BT_LE_ENABLE
    /* Low energy SSP does not have keypress notification */
    if(!scData->bondData.lowEnergy)
#endif
    {
        scPrim = (CsrBtScSspKeypressNotificationReq *) scData->recvMsgP;
        CsrBtCmScDmSendKeypressNotificationReq(scData->deviceAddr,
                                               scPrim->notificationType,
                                               scData->addressType);
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScDmSmKeypressNotificationIndBondingS
*
*  DESCRIPTION
*      The remote side has notified us that it is currently entering a
*      passkey. Simply forward the notification to the app.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScDmSmKeypressNotificationIndBondingS(ScInstanceData_t *scData)
{
    DM_SM_KEYPRESS_NOTIFICATION_IND_T *dmPrim;

    dmPrim = (DM_SM_KEYPRESS_NOTIFICATION_IND_T *) scData->recvMsgP;

    CsrBtScSspKeypressNotificationIndSend(scData->mainAppHandle,
                                          &dmPrim->addrt.addr,
                                          dmPrim->notification_type,
                                          dmPrim->addrt.type);
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScDmSmLinkKeyIndRemoteBondingS
*
*  DESCRIPTION
*      A link key has been generated due to remote bonding.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScDmSmLinkKeyIndRemoteBondingS(ScInstanceData_t *scData)
{
    DM_SM_LINK_KEY_IND_T    *dmPrim;

    dmPrim = (DM_SM_LINK_KEY_IND_T *)scData->recvMsgP;

    if (!CsrBtBdAddrEq(&dmPrim->bd_addr, &scData->deviceAddr))
    {
        CsrBtScDmSmLinkKeyIndIdleS(scData);
    }
    else
    {
        CsrBtScDmSmLinkKeyIndRemoteBondingPostLisbonS(scData);
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScPairingSuccessUpdateDeviceReset
*
*  DESCRIPTION
*      Complete pairing by adding device to;
*
*      1) DM SM
*      2) SC DB (if the app has requested this)
*      3) Send out bonding confirm to SD
*
*      Note that we only do this if an ACL is up and running. This
*      condition always hold for locally initiated bonding.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScPairingSuccessUpdateDeviceReset(ScInstanceData_t *scData)
{
    CsrBtScPairingSuccessUpdateDevice(scData);

    CsrBtScResetInstData(scData);
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScPairingSuccessUpdateDevice
*
*  DESCRIPTION
*      Pairing has completed successfully. We need to update the SC DB
*      and Trusted Device List.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScPairingSuccessUpdateDevice(ScInstanceData_t *scData)
{
    if (scData->accept)
    {
        if (scData->addDevice)
        {
            CsrBtScScDbUpdate(scData);
        }
        else
        {
            CsrBtScDbRemove(&scData->deviceAddr);
        }
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScPairingFailedCleanupDeviceReset
*
*  DESCRIPTION
*      Pairing failed, so we need to cleanup the SCDB / Trusted Device List.
*      For bonding, we always clear the entry.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScPairingFailedCleanupDeviceReset(ScInstanceData_t *scData)
{
    if (scData->state == SC_ST_LOCAL_POST_LISBON_BONDING ||
        scData->state == SC_ST_REBOND ||
        scData->accept)
    {
        /* Remove the entry if this is bonding or if the user has accepted the request */
        CsrBtScDbRemove(&scData->deviceAddr);
    }

    CsrBtScResetInstData(scData);
}

