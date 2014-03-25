/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

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
#include "csr_bt_sc_lib_upstream.h"
#include "csr_bt_sc_utils.h"
#include "csr_bt_sc_bond_unified.h"
#include "csr_bt_sc_bondable_mode.h"
#include "csr_bt_sc_bond_legacy.h"
#include "csr_bt_sc_bond_remote.h"
#include "csr_bt_sc_bond_local.h"
#include "csr_bt_sd_lib.h"
#include "csr_bt_sc_ui.h"
#include "csr_bt_sc_authorise.h"
#include "csr_bt_sc_oob.h"
#include "csr_bt_sd_private_prim.h"
#include "csr_bt_sd_private_lib.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_sc_le.h"

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScDmSmPinRequestIndIdleS
*
*  DESCRIPTION
*      The DM has requested a legacy pin code, so a remote device trying to
*      pair with us or we're enforcing outgoing security.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScDmSmPinRequestIndIdleS(ScInstanceData_t *scData)
{
    DM_SM_PIN_REQUEST_IND_T *dmPrim;
    dmPrim = (DM_SM_PIN_REQUEST_IND_T *)scData->recvMsgP;

    if (scData->bondData.bondable == CSR_BT_SEC_MODE_PAIRABLE)
    {
        /* Only allow in bondable mode */
        CsrBtBdAddrCopy(&scData->deviceAddr, &dmPrim->addrt.addr);

        scData->appHandle           = scData->mainAppHandle;
        scData->restoreMsgFlag      = FALSE;
        scData->bondData.initiator = INITIATOR_REMOTE;
        scData->bondData.localInitiator = dmPrim->initiator;
        SC_CHANGE_MSGTYPE(&scData->pendingMsgType, MSG_TYPE_LEGACY);
        SC_CHANGE_STATE(&scData->state, SC_ST_REMOTE_BONDING);
        SC_CHANGE_STATE(&scData->bondData.dmState, DM_AUTHEN_RECV);
        SC_CHANGE_STATE(&scData->bondData.scState, SC_ACTIVE);

        CsrBtScSetCod(scData, dmPrim->dev_class);
        CsrBtScStartUi(scData);
    }
    else
    {
        CsrBtScRejectDmSmPinRequestInd(scData);
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScDmSmLinkKeyRequestIndRemoteBondingS
*
*  DESCRIPTION
*       Authentication requested while pairing.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScDmSmLinkKeyRequestIndRemoteBondingS(ScInstanceData_t *scData)
{
    DM_SM_LINK_KEY_REQUEST_IND_T *dmPrim;

    dmPrim = (DM_SM_LINK_KEY_REQUEST_IND_T *) scData->recvMsgP;

    if (CsrBtBdAddrEq(&dmPrim->bd_addr, &scData->deviceAddr))
    {
        CsrBtScDmSaveMessage(scData);
    }
    else
    {
        CsrBtScDmSmLinkKeyRequestIndIdleS(scData);
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScSmIoCapabilityRequestIndIdleS
*
*  DESCRIPTION
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScSmIoCapabilityRequestIndIdleS(ScInstanceData_t *scData)
{
    DM_SM_IO_CAPABILITY_REQUEST_IND_T *dmPrim;

    dmPrim = (DM_SM_IO_CAPABILITY_REQUEST_IND_T *) scData->recvMsgP;

    if (CsrBtScAcceptsPairing(scData, scData->bondData.remoteAuth))
    {
        scData->restoreMsgFlag     = FALSE;
        scData->appHandle          = scData->mainAppHandle;
        scData->bondData.initiator = INITIATOR_REMOTE;
        scData->bondData.localInitiator = (CsrBool)((dmPrim->flags & DM_SM_FLAGS_INITIATOR)
                                                    ? TRUE
                                                    : FALSE);
        CsrBtBdAddrCopy(&scData->deviceAddr, &dmPrim->addrt.addr);
        scData->addressType = dmPrim->addrt.type;

        SC_CHANGE_STATE(&scData->bondData.dmState, DM_IO_CAPS_RECV);
        SC_CHANGE_STATE(&scData->bondData.scState, SC_ACTIVE);
        SC_CHANGE_STATE(&scData->state, SC_ST_REMOTE_BONDING);
        CsrBtScSmIoCapabilityRequestIndDmPendingS(scData);
    }
    else
    {
        CsrBtCmScDmIoCapabilityRequestNegRes(dmPrim->addrt.addr,
                                             HCI_ERROR_PAIRING_NOT_ALLOWED,
                                             dmPrim->addrt.type);
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScSmIoCapabilityRequestIndRemoteBondingS
*
*  DESCRIPTION
*
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScSmIoCapabilityRequestIndRemoteBondingS(ScInstanceData_t *scData)
{
    DM_SM_IO_CAPABILITY_REQUEST_IND_T *dmPrim;

    dmPrim = (DM_SM_IO_CAPABILITY_REQUEST_IND_T *) scData->recvMsgP;

    if (CsrBtScAcceptsPairing(scData, scData->bondData.remoteAuth))
    {
        CsrBtScSmIoCapabilityRequestIndLocalBondingS(scData);
    }
    else
    {
        CsrBtCmScDmIoCapabilityRequestNegRes(dmPrim->addrt.addr,
                                             HCI_ERROR_HOST_BUSY_PAIRING,
                                             dmPrim->addrt.type);
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScSmIoCapabilityResponseIndIdleS
*
*  DESCRIPTION
*       A remote device has initiated pairing with us. If we are bondable
*       we accept the request.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScSmIoCapabilityResponseIndIdleS(ScInstanceData_t *scData)
{
    DM_SM_IO_CAPABILITY_RESPONSE_IND_T *dmPrim;

    dmPrim = (DM_SM_IO_CAPABILITY_RESPONSE_IND_T *) scData->recvMsgP;

    if (CsrBtScAcceptsPairing(scData, dmPrim->authentication_requirements))
    {
        scData->appHandle              = scData->mainAppHandle;
        scData->bondData.remoteAuth    = dmPrim->authentication_requirements;
        scData->bondData.remoteIoCaps = dmPrim->io_capability;
        scData->bondData.initiator     = INITIATOR_REMOTE;
        scData->sspData.remoteOobDataPresent = dmPrim->oob_data_present;
        scData->bondData.localInitiator = TRUE;

        SC_CHANGE_STATE(&scData->state, SC_ST_REMOTE_BONDING);
        SC_CHANGE_STATE(&scData->bondData.dmState, DM_PENDING);
        SC_CHANGE_STATE(&scData->bondData.scState, SC_ACTIVE);

        CsrBtBdAddrCopy(&scData->deviceAddr, &dmPrim->addrt.addr);
        scData->addressType = dmPrim->addrt.type;

        CsrBtScUpdateAssocModel(scData);
    }
    else
    {
        ; /* Ignore it. We are not in pairable mode */
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScSmIoCapabilityResponseIndRemoteBondingS
*
*  DESCRIPTION
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScSmIoCapabilityResponseIndRemoteBondingS(ScInstanceData_t *scData)
{
    DM_SM_IO_CAPABILITY_RESPONSE_IND_T *dmPrim;

    dmPrim = (DM_SM_IO_CAPABILITY_RESPONSE_IND_T *) scData->recvMsgP;

    if (CsrBtScAcceptsPairing(scData, dmPrim->authentication_requirements))
    {
        CsrBtScSmIoCapabilityResponseIndLocalBondingS(scData);
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScDmSmPinRequestIndAppRemoteBondingS
*
*  DESCRIPTION
*      When we get here we've either waiting for the app to respond or
*      answered a pin code request, but authentication
*      failed. We can only detect this failure _if_ the remote device disconnects
*      the ACL link. In this case it hasn't and has instead opted to retry pairing.
*      Now we backtrack by restarting the timer and notifying the app about the
*      failure. This is needed because the app is waiting for a bond indication.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScDmSmPinRequestIndAppRemoteBondingS(ScInstanceData_t *scData)
{
    DM_SM_PIN_REQUEST_IND_T *dmPrim;

    dmPrim = (DM_SM_PIN_REQUEST_IND_T *) scData->recvMsgP;

    /* Revert the states */
    SC_CHANGE_STATE(&scData->bondData.dmState, DM_AUTHEN_RECV);
    SC_CHANGE_STATE(&scData->bondData.appState, APP_RESPOND);

    /* Restart our watchdog timer */
    CsrBtScStartPairingWatchdog(scData);
    scData->authorised = FALSE;
    CsrBtScBondReqCancelHandler(scData, scData->mainAppHandle,
                                FALSE); /* do NOT allow reset */
    scData->bondData.bondAckSent = FALSE;
    CsrBtScPasskeyIndSend(scData->mainAppHandle,
                          scData->cod,
                          &scData->deviceAddr,
                          scData->friendlyName,
                          scData->paired,
                          dmPrim->initiator,
                          scData->addressType);
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScDmSmPinRequestIndRemoteBondingS
*
*  DESCRIPTION
*      The DM has requested a legacy pin code. This can either be:
*
*      1) The first remotely initiated attempt
*      2) Authentication failed but we're we don't know that because the
*         RD hasn't disconnected the ACL link.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScDmSmPinRequestIndRemoteBondingS(ScInstanceData_t *scData)
{
    DM_SM_PIN_REQUEST_IND_T *dmPrim;

    dmPrim = (DM_SM_PIN_REQUEST_IND_T *)scData->recvMsgP;

    if (scData->bondData.scState == SC_CANCELLED ||
        !CsrBtBdAddrEq(&scData->deviceAddr, &dmPrim->addrt.addr))
    {
        /* Reject */
        CsrBtCmScDmPinRequestNegRes(dmPrim->addrt.addr);

        scData->bondData.localInitiator = dmPrim->initiator;

        if (scData->bondData.scState == SC_CANCELLED
            && CsrBtBdAddrEq(&scData->deviceAddr, &dmPrim->addrt.addr))
        {
            CsrBtScSetCod(scData, dmPrim->dev_class);
            scData->authorised = FALSE;
            CsrBtScBondReqCancelHandlerAllowReset(scData, scData->mainAppHandle); /* allow reset */
        }
    }
    else
    {
        CsrBtScSetCod(scData, dmPrim->dev_class);

        switch(scData->bondData.appState)
        {
            case APP_IDLE:
                /* Notify the app the bonding has failed and _don't_ reset anything but we
                 * just continue processing the new request */
                scData->authorised = FALSE;
                CsrBtScBondReqCancelHandler(scData, scData->appHandle,
                                            FALSE); /* do NOT reset */
                scData->bondData.bondAckSent = FALSE;
                /* Prolong the pain */
                CsrBtScStartPairingWatchdog(scData);
                break;

            case APP_RESPOND:
            case APP_RESPONDED:
                CsrBtScDmSmPinRequestIndAppRemoteBondingS(scData);
                break;

            default:
                CsrBtScStateEventException(CSR_BT_SC_PRIM, scData->pendingMsgType, scData->bondData.uiState);
                break;
        }
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScSspNotificationResRemoteBondCompleteS
*
*  DESCRIPTION
*      Bonding completed while the app was still processing the request.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScSspNotificationResRemoteBondCompleteS(ScInstanceData_t *scData)
{
    CsrBtScSspNotificationRes *scPrim;

    scPrim = (CsrBtScSspNotificationRes *) scData->recvMsgP;

    if (scData->bondData.appState == APP_RESPOND &&
        scData->bondData.dmState  == DM_BOND_COMPLETE)
    {
        scData->addDevice   = scPrim->addDevice;
        scData->authorised  = scPrim->authorised;
        scData->accept      = scPrim->accept;

        /* Stop watchdog timer */
        CsrBtScStopBondTimer(scData);

        if (scPrim->accept)
        {
            CsrBtScNotifyUpperBondIndSimple(scData,
                                      scData->mainAppHandle,
                                      CSR_BT_RESULT_CODE_SC_SUCCESS,
                                      HCI_SUCCESS,
                                      scData->addDevice);

            CsrBtScPairingSuccessUpdateDeviceReset(scData);
        }
        else
        {
            /* Remove any pending authorisation requests */
            CsrBtScCancelQueuedAuthoriseReq(scData);

            CsrBtScBondReqCancelHandlerAllowReset(scData, scData->mainAppHandle); /* allow reset */
        }

        /* Some messages could be saved due to the app not responding in timely manner.
         * We need to restore those now */
        CsrBtScDmRestoreMessages(scData);
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScSspNotificationResRemoteBondingS
*
*  DESCRIPTION
*      The app has replied on a passkey notification. Note that
*      we require that the app display the numeric value _while_
*      the pairing takes place. There is a subtle issue with
*      Passkey notification, because RD is able to complete pairing
*      without user actually responding. So here we need to find out
*      if pairing has completed or still ongoing.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScSspNotificationResRemoteBondingS(ScInstanceData_t *scData)
{
    switch(scData->bondData.dmState)
    {
        case DM_AUTHEN_RECV:
            CsrBtScSspNotificationResBondingS(scData);
            break;

        case DM_BOND_COMPLETE:
            CsrBtScSspNotificationResRemoteBondCompleteS(scData);
            break;

        default:
            CsrBtScStateEventException(CSR_BT_SC_PRIM, scData->pendingMsgType, scData->bondData.dmState);
            break;
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScPasskeyResRemoteBondingS
*
*  DESCRIPTION
*      The app has replied with a pin.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScPasskeyResRemoteBondingS(ScInstanceData_t *scData)
{
    CsrBtScPasskeyRes *scPrim;

    scPrim = (CsrBtScPasskeyRes *)scData->recvMsgP;

    if ((scData->bondData.appState == APP_RESPOND) &&
        (CsrBtBdAddrEq(&scData->deviceAddr, &scPrim->deviceAddr))
        && scData->addressType == scPrim->addressType)
    {
        scData->addDevice   = scPrim->addDevice;
        scData->authorised = scPrim->authorised;
        scData->accept      = scPrim->accept;

        if (scPrim->accept)
        {
            /* Special handling of empty pin code */
            if (scPrim->passKeyLength == 0)
            {
                scPrim->passKeyLength = 1;
                scPrim->passKey[0]     = '\0';
            }

            /* Start a watchdog timer to see if pairing was successful or not
             * This is not optimal but we can't do much more than this because
             * we are not notified by the controller if pairing fails.
             * Our only hope is that the remote device will disconnect the
             * link which we will be able trigger on */
            CsrBtScStartRemoteBondTimer(scData);

            CsrBtCmScDmPinRequestRes(scData->deviceAddr,
                                scPrim->passKeyLength,
                                scPrim->passKey);

            SC_CHANGE_STATE(&scData->bondData.appState, APP_RESPONDED);
            SC_CHANGE_STATE(&scData->bondData.dmState, DM_AUTHEN_RESP);
        }
        else
        {
            CsrBtCmScDmPinRequestNegRes(scData->deviceAddr);
            CsrBtScBondReqCancelHandlerAllowReset(scData, scData->mainAppHandle); /* allow reset */
        }
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScDmSimplePairingCompleteIndRemoteIoCapsRecvS
*
*  DESCRIPTION
*       A SSP complete event arrived while pending (this means that we haven't
*       decided which security model to use). Note that this can happens due
*       the RD not supporting SSP. In this case the DM will retry legacy pairing.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScDmSimplePairingCompleteIndRemoteIoCapsRecvS(ScInstanceData_t *scData)
{
    DM_SM_SIMPLE_PAIRING_COMPLETE_IND_T *dmPrim;
    
    dmPrim = (DM_SM_SIMPLE_PAIRING_COMPLETE_IND_T *) scData->recvMsgP;

    if (dmPrim->status == HCI_SUCCESS)
    {
        ; /* SSP succeeded due to OOB */
    }
    else
    {
        CsrBtScBondReqCancelHandlerAllowReset(scData, scData->appHandle); /* allow reset */
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScDmSimplePairingCompleteIndRemoteDmAuthS
*
*  DESCRIPTION
*       A SSP complete event arrived, while we were waiting for a response
*       from the application. We don't remove the remote device from
*       SC DB since the app has never told us to do so.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScDmSimplePairingCompleteIndRemoteDmAuthS(ScInstanceData_t *scData)
{
    DM_SM_SIMPLE_PAIRING_COMPLETE_IND_T *dmPrim;

    dmPrim = (DM_SM_SIMPLE_PAIRING_COMPLETE_IND_T *) scData->recvMsgP;

    if (dmPrim->status == HCI_SUCCESS)
    {
        ; /* SSP succeeded due to OOB */
    }
    else
    {
        CsrBtScBondReqCancelHandlerAllowReset(scData, scData->mainAppHandle); /* allow reset */
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScDmSimplePairingCompleteIndRemoteBondingS
*
*  DESCRIPTION
*       Simple pairing has completed.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScDmSimplePairingCompleteIndRemoteBondingS(ScInstanceData_t *scData)
{
    DM_SM_SIMPLE_PAIRING_COMPLETE_IND_T *dmPrim;

    dmPrim = (DM_SM_SIMPLE_PAIRING_COMPLETE_IND_T *) scData->recvMsgP;

    if (CsrBtBdAddrEq(&dmPrim->addrt.addr, &scData->deviceAddr)
        && dmPrim->addrt.type == scData->addressType)
    {
        ScSetHciStatusIfUnset(scData, dmPrim->status);

        switch(scData->bondData.dmState)
        {
            case DM_PENDING:
            case DM_IO_CAPS_RECV:
                CsrBtScDmSimplePairingCompleteIndRemoteIoCapsRecvS(scData);
                break;

            case DM_AUTHEN_RECV:
            case DM_AUTHEN_RESP:
#ifdef CSR_BT_LE_ENABLE
                if(scData->bondData.lowEnergy)
                {
                    /* In low energy, the SSP complete event is the
                       * last thing we receive.  Keys have already
                       * been exchanged at this point */
                    CsrBtScLeSspComplete(scData);
                }
#endif
                {
                    CsrBtScDmSimplePairingCompleteIndRemoteDmAuthS(scData);
                }
                break;

            default:
                CsrBtScStateEventException(CSR_BT_SC_PRIM, scData->pendingMsgType, scData->bondData.dmState);
                break;
        }
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScDmSmLinkKeyIndRemoteAuthRecvS
*
*  DESCRIPTION
*      A link key has been generated while we were awaiting a response from the
*      app. This only happens when we're doing passkey notification. Basically
*      the RD has raced with us and completed pairing before the app was
*      able to respond.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScDmSmLinkKeyIndRemoteAuthRecvS(ScInstanceData_t *scData)
{
    SC_CHANGE_STATE(&scData->bondData.dmState, DM_BOND_COMPLETE);
    SC_CHANGE_STATE(&scData->bondData.appState, APP_RESPOND);
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScDmSmLinkKeyIndRemoteAuthRespBondingS
*
*  DESCRIPTION
*      A link key has been generated. We don't send up the ack before we
*      have an ACL up. This way we avoid that the app starts to
*      connect to the RD prematurely.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScDmSmLinkKeyIndRemoteAuthRespBondingS(ScInstanceData_t *scData)
{
    CsrUintFast8 index;

    /* Stop our watchdog timer */
    CsrBtScStopBondTimer(scData);

    if (CsrBtScAclPresent(scData, scData->deviceAddr, &index))
    {
        /* ACL is already up. Go ahead. */
        if (scData->accept)
        {
            CsrBtScNotifyUpperBondIndSimple(scData,
                                      scData->appHandle,
                                      CSR_BT_RESULT_CODE_SC_SUCCESS,
                                      HCI_SUCCESS,
                                      scData->addDevice);

            CsrBtScPairingSuccessUpdateDeviceReset(scData);
        }
        else
        {
            CsrBtScBondReqCancelHandlerAllowReset(scData, scData->appHandle); /* allow reset */
        }
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      csrBtScDmSmLinkKeyIndSspPostLisbonS
*
*  DESCRIPTION
*      A link key has been generated due to remote bonding.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
static void csrBtScDmSmLinkKeyIndSspPostLisbonS(ScInstanceData_t *scData)
{
    DM_SM_LINK_KEY_IND_T    *dmPrim;

    dmPrim = (DM_SM_LINK_KEY_IND_T *)scData->recvMsgP;

    if (CsrBtBdAddrEq(&dmPrim->bd_addr, &scData->deviceAddr))
    {
        switch(scData->bondData.dmState)
        {
            case DM_AUTHEN_RECV:
                /* Passkey notification */
                CsrBtScDmSmLinkKeyIndRemoteAuthRecvS(scData);
                break;

            case DM_AUTHEN_RESP:
                /* Passkey, notification and compare */
                CsrBtScDmSmLinkKeyIndRemoteAuthRespBondingS(scData);
                break;

            default:
                CsrBtScStateEventException(CSR_BT_SC_PRIM, scData->pendingMsgType, scData->bondData.dmState);
                break;
        }
    }
    else
    {
        CsrBtScDmSmLinkKeyIndIdleS(scData);
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      csrBtScDmSmLinkKeyIndOobPostLisbonS
*
*  DESCRIPTION
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
static void csrBtScDmSmLinkKeyIndOobPostLisbonS(ScInstanceData_t *scData)
{
    /* Automatic add (due to OOB where the app might not receive an indication) */
    scData->accept = TRUE;
    scData->addDevice = TRUE;

    CsrBtScNotifyUpperBondIndSimple(scData,
                              scData->mainAppHandle,
                              CSR_BT_RESULT_CODE_SC_SUCCESS,
                              HCI_SUCCESS,
                              scData->addDevice);
    
    CsrBtScPairingSuccessUpdateDeviceReset(scData);

}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScDmSmLinkKeyIndRemoteBondingPostLisbonS
*
*  DESCRIPTION
*      A link key has been generated due to remote bonding.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScDmSmLinkKeyIndRemoteBondingPostLisbonS(ScInstanceData_t *scData)
{
    DM_SM_LINK_KEY_IND_T    *dmPrim;

    CsrBtScMapToDmKeyType(scData);

    dmPrim = (DM_SM_LINK_KEY_IND_T *)scData->recvMsgP;

    if (CsrBtBdAddrEq(&dmPrim->bd_addr, &scData->deviceAddr))
    {
        CsrMemCpy(&scData->bondData.linkKey, &dmPrim->key, SIZE_LINK_KEY);
        scData->bondData.linkKeyType = dmPrim->key_type;

        switch(scData->pendingMsgType)
        {
            case MSG_TYPE_NONE:
            case MSG_TYPE_IO_CAPS:
                /* Bonding failed before we even started */
                CsrBtScBondReqCancelHandlerAllowReset(scData, scData->appHandle); /* allow reset */
                break;

            case MSG_TYPE_OOB:
                csrBtScDmSmLinkKeyIndOobPostLisbonS(scData);
                break;

            case MSG_TYPE_LEGACY:
            case MSG_TYPE_JUSTWORKS:
            case MSG_TYPE_PASSKEY:
            case MSG_TYPE_COMPARE:
            case MSG_TYPE_NOTIFICATION:
                csrBtScDmSmLinkKeyIndSspPostLisbonS(scData);
                break;

            case MSG_TYPE_REBOND:
            default:
                CsrBtScStateEventException(CSR_BT_SC_PRIM, scData->pendingMsgType, scData->bondData.dmState);
                break;
        }
    }
    else
    {
        CsrBtScDmSmLinkKeyIndIdleS(scData);
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScCmConnectionRejSecurityIndRemoteBondingS
*
*  DESCRIPTION
*       The remote device rejected our connection attempts most probably
*       due to authentication failure.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScCmConnectionRejSecurityIndRemoteBondingS(ScInstanceData_t *scData)
{
    CsrBtCmConnectionRejSecurityInd *prim;

    prim = (CsrBtCmConnectionRejSecurityInd *)scData->recvMsgP;

    if (CsrBtBdAddrEq(&prim->deviceAddr,&scData->deviceAddr))
    {
        if (prim->cancelInitiated)
        {
            if (scData->bondData.scState == SC_ACTIVE)
            {
                SC_CHANGE_STATE(&scData->bondData.scState, SC_CANCELLED);

                if (scData->bondData.appState == APP_RESPOND || scData->bondData.uiState != UI_IDLE)
                {
                    /* Stop UI statemachine */
                    CsrBtScCancelUiS(scData);

                    switch(scData->pendingMsgType)
                    {

                        case MSG_TYPE_NOTIFICATION:
                            /* Answer on behalf off the application */
                            SC_CHANGE_STATE(&scData->bondData.appState, APP_RESPONDED);
                    
                            /* Detach the ACL if no connections are using it */
                            CsrBtCmAclDetachReqSend(CSR_BT_SC_IFACEQUEUE, scData->deviceAddr, CSR_BT_CM_ACL_DETACH_EXCLUDE_ALL);
                            CsrBtScBondReqCancelHandlerAllowReset(scData, scData->mainAppHandle); /* allow reset */
                            break;

                        case MSG_TYPE_LEGACY:
                        case MSG_TYPE_REBOND:
                        case MSG_TYPE_JUSTWORKS:
                        case MSG_TYPE_COMPARE:
                        case MSG_TYPE_PASSKEY:
                        case MSG_TYPE_IO_CAPS:
                            CsrBtScRejectPendingMsg(scData);
                            CsrBtScBondReqCancelHandlerAllowReset(scData, scData->mainAppHandle); /* allow reset */
                            break;

                        case MSG_TYPE_NONE:
                        default:
                            /* Answer on behalf off the application */
                            SC_CHANGE_STATE(&scData->bondData.appState, APP_RESPONDED);
                            CsrBtScStateEventException(CSR_BT_SC_PRIM, scData->pendingMsgType, scData->bondData.dmState);
                            break;
                    }
                }
            }
        }
        else if (scData->pendingMsgType == MSG_TYPE_LEGACY)
        {
            /* In legacy pairing we never receive any indication that pairing has failed */
            CsrBtScRemotePairingFailed(scData);
        }
    }
}

