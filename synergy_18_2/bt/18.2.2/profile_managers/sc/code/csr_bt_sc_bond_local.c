/****************************************************************************

    Copyright Cambridge Silicon Radio Limited and its licensors 2012. 
            
              All rights reserved.

REVISION:      $Revision: #3 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_bt_sc_main.h"
#include "csr_bt_util.h"
#include "dm_prim.h"
#include "csr_bt_cm_dm_sc_lib.h"
#include "csr_bt_cm_dm_sc_ssp_lib.h"
#include "csr_bt_sc_prim.h"
#include "csr_bt_sc_lib_upstream.h"
#include "csr_bt_sc_utils.h"
#include "csr_bt_sc_bond_local.h"
#include "csr_bt_sc_bond_unified.h"
#include "csr_bt_sd_lib.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_sc_ui.h"
#include "csr_bt_sc_authorise.h"
#include "csr_bt_sc_oob.h"
#include "csr_bt_sd_private_prim.h"
#include "csr_bt_sd_private_lib.h"

static void CsrBtScNotifyUpperBondCfmSimpleInclUpdDevReset(ScInstanceData_t *scData)
{
    CsrBool lowEnergy      = FALSE;
    CsrSchedQid appHandle  = scData->appHandle;
#ifdef CSR_BT_LE_ENABLE
        lowEnergy = (CSR_BT_SC_IS_LOW_ENERGY(scData) && (scData->bondData.leAppHandle != CSR_SCHED_QID_INVALID));

        if (lowEnergy)
        {
            appHandle = scData->bondData.leAppHandle;
        }
#endif
    
    CsrBtScNotifyUpperBondCfm(scData->authorised,
                              appHandle,
                              CSR_BT_RESULT_CODE_SC_SUCCESS,
                              CSR_BT_SUPPLIER_SC,
                              HCI_SUCCESS,
                              scData->deviceAddr,
                              scData->cod,
                              scData->addDevice,
                              scData->bondData.bondAckSent,
                              lowEnergy,
                              CSR_BT_SC_ADDR_TYPE(scData));
    CsrBtScPairingSuccessUpdateDeviceReset(scData);
}


/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScBondReqIdlePostLisbonS
*
*  DESCRIPTION
*      Start dedicated bonding. The DM will delete the link key in its
*      volatile storage and in the controller before starting pairing.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScBondReqIdlePostLisbonS(ScInstanceData_t *scData)
{
    CsrBtScBondReq  *scPrim;

    scPrim = (CsrBtScBondReq *)scData->recvMsgP;

    CsrBtCmScDmBondingReq(scPrim->deviceAddr, scData->bondData.localAuth);
}

/*----------------------------------------------------------------------------*
*  NAME
*      ScCancelQueuedBondReq
*
*  DESCRIPTION
*       Cancel a bond request from the queue.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
static CsrBool csrBtScCancelQueuedBondReq(ScInstanceData_t *scData,
                                                CsrBtDeviceAddr     deviceAddr,
                                                CsrSchedQid        phandle)
{
    CsrUint16             msgType;
    CsrBtScPrim            *msg;
    void *popMsg;
    CsrBool               msgCancelled  = FALSE;
    CsrMessageQueueType *tempQueue = NULL;

    while(CsrMessageQueuePop(&scData->saveQueue, &msgType, &popMsg))
    {
        msg = (CsrBtScPrim*)popMsg;

        if (!msgCancelled && msgType == CSR_BT_SC_PRIM && *msg == CSR_BT_SC_BOND_REQ)
        {
            CsrBtScBondReq *prim = (CsrBtScBondReq *) msg;

            if (prim->phandle == phandle && CsrBtBdAddrEq(&prim->deviceAddr, &deviceAddr))
            {
                msgCancelled = TRUE;
                CsrPmemFree(msg);
            }
            else
            {
                CsrMessageQueuePush(&tempQueue, msgType, msg);
            }
        }
        else
        {
            CsrMessageQueuePush(&tempQueue, msgType, msg);
        }
    }
    scData->saveQueue = tempQueue;
    return (msgCancelled);
}

/*----------------------------------------------------------------------------*
*  NAME
*      csrBtScCancelBondReq
*
*  DESCRIPTION
*       Cancel a bonding request according to the msg type.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
static void csrBtScCancelBondReq(ScInstanceData_t *scData)
{
    CsrBtScCancelBondReq  *scPrim;
    CsrBool force;

    scPrim  = (CsrBtScCancelBondReq *) scData->recvMsgP;

    /* We do abnormal disconnect if we're doing passkey notification.
     * That's the only option we have */
    force = (scData->pendingMsgType == MSG_TYPE_NOTIFICATION);
    CsrBtCmScDmBondingCancelReq(scPrim->deviceAddr,
                                force,
                                scPrim->addressType);

    /* Stop UI statemachine */
    CsrBtScCancelUiS(scData);
    /* Stop watchdog timer */
    CsrBtScStopBondTimer(scData);

    scData->accept    = FALSE;
    scData->addDevice = FALSE;

    CsrBtScRejectPendingMsg(scData);

    SC_CHANGE_STATE(&scData->bondData.scState, SC_CANCELLED);
    CsrBtScBondReqCancelHandler(scData, scPrim->phandle, FALSE);
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScCancelBondReqBondingPostLisbonS
*
*  DESCRIPTION
*       Cancel a bonding request, both currently active and pending.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScCancelBondReqBondingPostLisbonS(ScInstanceData_t *scData)
{
    CsrBtScCancelBondReq  *scPrim;

    scPrim  = (CsrBtScCancelBondReq *) scData->recvMsgP;

    if (CsrBtBdAddrEq(&scPrim->deviceAddr, &scData->deviceAddr) &&
        scData->appHandle == scPrim->phandle &&
        scData->bondData.scState == SC_ACTIVE)
    {
        csrBtScCancelBondReq(scData);
    }
    else
    {
        CsrBtScCancelBondReqAnyS(scData);
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScCancelBondReqAnyS
*
*  DESCRIPTION
*       Cancel a queued bonding request.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScCancelBondReqAnyS(ScInstanceData_t *scData)
{
    CsrBtScCancelBondReq  *scPrim;

    scPrim  = (CsrBtScCancelBondReq *) scData->recvMsgP;

    if (csrBtScCancelQueuedBondReq(scData, scPrim->deviceAddr, scPrim->phandle))
    {
        CsrBool lowEnergy     = FALSE;
        CsrSchedQid appHandle = scData->appHandle;

#ifdef CSR_BT_LE_ENABLE
        lowEnergy = (CSR_BT_SC_IS_LOW_ENERGY(scData) && (scData->bondData.leAppHandle != CSR_SCHED_QID_INVALID));

        if (lowEnergy)
        {
            appHandle = scData->bondData.leAppHandle;
        }
#endif
        CsrBtScNotifyUpperBondCfm(FALSE,
                                  appHandle,
                                  CSR_BT_RESULT_CODE_SC_BONDING_CANCELLED,
                                  CSR_BT_SUPPLIER_SC,
                                  HCI_ERROR_UNSPECIFIED,
                                  scPrim->deviceAddr,
                                  0,
                                  FALSE,
                                  FALSE,
                                  lowEnergy,
                                  CSR_BT_SC_ADDR_TYPE(scData));

        CsrBtScPairingFailedCleanupDeviceReset(scData);
        CsrBtScDbRemove(&scData->deviceAddr);
    }
    else
    {
        if (scData->state == SC_ST_IDLE)
        { /* This message must be crossing CSR_BT_SC_BOND_CFM, just ignore it */
            ;
        }
        else
        {
            CsrBtScStateEventException(CSR_BT_SC_PRIM, scData->pendingMsgType, scData->bondData.dmState);
        }
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScDmSmLinkKeyRequestIndLocalBondingS
*
*  DESCRIPTION
*      See if we have the (proper) link key in the SC DB.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScDmSmLinkKeyRequestIndLocalBondingS(ScInstanceData_t *scData)
{
    DM_SM_LINK_KEY_REQUEST_IND_T *dmPrim;

    dmPrim = (DM_SM_LINK_KEY_REQUEST_IND_T *) scData->recvMsgP;

    /* We give the DM the link key (assuming we have it) if:
     *
     * 1) The app has cancelled the bonding procedure or
     * 2) If it is _not_ the device we're currently bonding with
     *
     * Note condition 1) above. If we instead rejected the link
     * key request we would actually encourage pairing. */
    if (scData->bondData.scState == SC_CANCELLED ||
        !CsrBtBdAddrEq(&scData->deviceAddr, &dmPrim->bd_addr))
    {
        CsrBtScDmSmLinkKeyRequestIndIdleS(scData);
    }
    else
    {
        CsrBtCmScDmSspLinkKeyRequestNegRes(dmPrim->bd_addr,
                                           CSR_BT_ADDR_PUBLIC);/* BR/EDR public only */
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScSmIoCapabilityRequestIndLocalBondingS
*
*  DESCRIPTION
*       We're in the process of pairing (locally or remotely initiated).
*       The controller has asked us for the IO capability.

*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScSmIoCapabilityRequestIndLocalBondingS(ScInstanceData_t *scData)
{
    DM_SM_IO_CAPABILITY_REQUEST_IND_T *dmPrim;

    dmPrim = (DM_SM_IO_CAPABILITY_REQUEST_IND_T *) scData->recvMsgP;

    if (!(CsrBtBdAddrEq(&scData->deviceAddr, &dmPrim->addrt.addr)
          && (scData->addressType == dmPrim->addrt.type)))
    {
        CsrBtCmScDmIoCapabilityRequestNegRes(dmPrim->addrt.addr,
                                             HCI_ERROR_HOST_BUSY_PAIRING,
                                             dmPrim->addrt.type);
    }
    else if (scData->bondData.scState == SC_CANCELLED)
    {
        CsrBtScSetCod(scData, dmPrim->dev_class);
        CsrBtCmScDmIoCapabilityRequestNegRes(dmPrim->addrt.addr,
                                             HCI_ERROR_HOST_BUSY_PAIRING,
                                             dmPrim->addrt.type);
    }
    else
    {
        switch(scData->bondData.dmState)
        {
            case DM_PENDING:
                scData->bondData.localInitiator = (CsrBool)((dmPrim->flags & DM_SM_FLAGS_INITIATOR)
                                                            ? TRUE
                                                            : FALSE);
                CsrBtScSmIoCapabilityRequestIndDmPendingS(scData);
                break;

            case DM_BOND_COMPLETE:
                /* The previous pairing has compled but the app still hasn't responded
                 * to the CSR_BT_SC_SSP_JUSTWORKS_IND, so we need to save this one */
                CsrBtScDmSaveMessage(scData);
                break;

            case DM_AUTHEN_RECV:
            case DM_AUTHEN_RESP:
            case DM_IO_CAPS_RECV:
            default:
                CsrBtScStateEventException(CSR_BT_SC_PRIM, scData->pendingMsgType, scData->bondData.uiState);
                break;
        }
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScSmIoCapabilityResponseIndLocalBondingS
*
*  DESCRIPTION
*       The app has initiated bonding request and we've received a
*       DM_SM_IO_CAPABILITY_RESPONSE_IND_T.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScSmIoCapabilityResponseIndLocalBondingS(ScInstanceData_t *scData)
{
    DM_SM_IO_CAPABILITY_RESPONSE_IND_T *dmPrim;

    dmPrim = (DM_SM_IO_CAPABILITY_RESPONSE_IND_T *) scData->recvMsgP;

    if (scData->bondData.scState == SC_CANCELLED ||
        !(CsrBtBdAddrEq(&scData->deviceAddr, &dmPrim->addrt.addr)
          && scData->addressType == dmPrim->addrt.type))
    {
        /* Just ignore */;
    }
    else
    {
        switch(scData->bondData.dmState)
        {
            case DM_PENDING:
            case DM_IO_CAPS_RECV:
                CsrBtScSmIoCapabilityResponseIndBondingS(scData);
                break;

            case DM_BOND_COMPLETE:
                /* The previous pairing has compled but the app still hasn't responded
                 * to the CSR_BT_SC_SSP_JUSTWORKS_IND, so we need to save this one */
                CsrBtScDmSaveMessage(scData);
                break;

            case DM_AUTHEN_RECV:
            case DM_AUTHEN_RESP:
            default:
                CsrBtScStateEventException(CSR_BT_SC_PRIM, scData->pendingMsgType, scData->bondData.uiState);
                break;
        }
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScDmSmUserConfirmationRequestIndLocalBondingS
*
*  DESCRIPTION
*      The DM has requested us to compare a numeric value.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScDmSmUserConfirmationRequestIndLocalBondingS(ScInstanceData_t *scData)
{
    SC_CHANGE_STATE(&scData->bondData.dmState, DM_AUTHEN_RECV);

    if (scData->bondData.scState == SC_CANCELLED)
    {
        CsrBtCmScDmUserConfirmationRequestNegRes(scData->deviceAddr,
                                                 scData->addressType);
    }
    else
    {
        CsrBtScDmSmUserConfirmationRequestIndBondingS(scData);
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScDmSmLocalUserConfirmationRequestIndLocalBondingS
*
*  DESCRIPTION
*       Do local confirmation
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScDmSmLocalUserConfirmationRequestIndLocalBondingS(ScInstanceData_t *scData)
{
    SC_CHANGE_STATE(&scData->bondData.dmState, DM_AUTHEN_RECV);

    if (scData->bondData.scState == SC_CANCELLED)
    {
        CsrBtCmScDmUserConfirmationRequestNegRes(scData->deviceAddr,
                                                 scData->addressType);
    }
    else
    {
        CsrBtScDmSmLocalUserConfirmationRequestIndBondingS(scData);
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScDmSmUserPasskeyRequestIndLocalBondingS
*
*  DESCRIPTION
*      The DM has requested us to provide a passkey.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScDmSmUserPasskeyRequestIndLocalBondingS(ScInstanceData_t *scData)
{
    SC_CHANGE_STATE(&scData->bondData.dmState, DM_AUTHEN_RECV);

    if (scData->bondData.scState == SC_CANCELLED)
    {
        CsrBtCmScDmUserPasskeyRequestNegRes(scData->deviceAddr,
                                            scData->addressType);
    }
    else
    {
        CsrBtScDmSmUserPasskeyRequestIndBondingS(scData);
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScDmSmUserPasskeyNotificationIndLocalBondingS
*
*  DESCRIPTION
*      The DM has requested us to show a passkey.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScDmSmUserPasskeyNotificationIndLocalBondingS(ScInstanceData_t *scData)
{
    /* If the app has cancelled, just ignore this one.
     * There's no way to reject a passkey notification */
    if (scData->bondData.scState == SC_CANCELLED)
    {
        SC_CHANGE_STATE(&scData->bondData.dmState, DM_AUTHEN_RECV);
    }
    else
    {
        CsrBtScDmSmUserPasskeyNotificationIndBondingS(scData);
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScDmSmPinRequestIndLocalBondingS
*
*  DESCRIPTION
*      The DM has requested a legacy pin code, due to our dedicated
*      bonding request.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScDmSmPinRequestIndLocalBondingS(ScInstanceData_t *scData)
{
    DM_SM_PIN_REQUEST_IND_T *dmPrim;

    dmPrim = (DM_SM_PIN_REQUEST_IND_T *)scData->recvMsgP;

    SC_CHANGE_STATE(&scData->bondData.dmState, DM_AUTHEN_RECV);

    switch(scData->bondData.scState)
    {
        case SC_ACTIVE:
            CsrBtScDmSmPinRequestIndBondingS(scData);
            break;

        case SC_CANCELLED:
            CsrBtCmScDmPinRequestNegRes(dmPrim->addrt.addr);
            break;

        default:
            CsrBtScStateEventException(CSR_BT_SC_PRIM,
                                       scData->pendingMsgType,
                                       scData->bondData.dmState);
            break;
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScSspPasskeyResLocalBondingS
*
*  DESCRIPTION
*      The app has replied with a passkey.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScSspPasskeyResLocalBondingS(ScInstanceData_t *scData)
{
    CsrBtScSspPasskeyRes *scPrim;

    scPrim = (CsrBtScSspPasskeyRes *)scData->recvMsgP;

    if (scData->bondData.appState == APP_RESPOND &&
        CsrBtBdAddrEq(&scData->deviceAddr, &scPrim->deviceAddr)
        && scData->addressType == scPrim->addressType)
    {
        if (scData->bondData.scState == SC_CANCELLED)
        {
            SC_CHANGE_STATE(&scData->bondData.appState, APP_RESPONDED);
            SC_CHANGE_STATE(&scData->bondData.dmState, DM_AUTHEN_RESP);

            CsrBtCmScDmUserPasskeyRequestNegRes(scData->deviceAddr,
                                                scData->addressType);
        }
        else
        {
            CsrBtScSspPasskeyResBondingS(scData);
        }
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScSspCompareResLocalBondingS
*
*  DESCRIPTION
*      The app has replied with a passkey.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScSspCompareResLocalBondingS(ScInstanceData_t *scData)
{
    if (scData->bondData.appState == APP_RESPOND)
    {
        if (scData->bondData.scState == SC_CANCELLED)
        {
            SC_CHANGE_STATE(&scData->bondData.appState, APP_RESPONDED);
            SC_CHANGE_STATE(&scData->bondData.dmState, DM_AUTHEN_RESP);

            CsrBtCmScDmUserConfirmationRequestNegRes(scData->deviceAddr,
                                                     scData->addressType);
        }
        else
        {
            CsrBtScSspCompareResBondingS(scData);
        }
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScSspNotificationResLocalAuthRecvS
*
*  DESCRIPTION
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScSspNotificationResLocalAuthRecvS(ScInstanceData_t *scData)
{
    if (scData->bondData.scState == SC_CANCELLED)
    {
        SC_CHANGE_STATE(&scData->bondData.appState, APP_RESPONDED);
        SC_CHANGE_STATE(&scData->bondData.dmState, DM_AUTHEN_RESP);
    }
    else
    {
        CsrBtScSspNotificationResBondingS(scData);
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScSspNotificationResLocalBondCompleteS
*
*  DESCRIPTION
*      The app has responded to the passkey notification _after_ the bonding
*      has completed.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScSspNotificationResLocalBondCompleteS(ScInstanceData_t *scData)
{
    CsrBtScSspNotificationRes *scPrim;

    scPrim = (CsrBtScSspNotificationRes *) scData->recvMsgP;

    scData->addDevice   = scPrim->addDevice;
    scData->authorised  = scPrim->authorised;
    scData->accept      = scPrim->accept;

    if (scPrim->accept && scData->bondData.scState != SC_CANCELLED)
    {
        CsrBtScNotifyUpperBondCfmSimpleInclUpdDevReset(scData);
    }
    else
    {
        /* Bonding has alredy completed but the app has cancelled/reject the request */
        CsrBtScPairingCancelled(scData);
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScSspNotificationResLocalBondingS
*
*  DESCRIPTION
*      The app has replied to a passkey notification. Note that
*      we require that the app display the numeric value _while_
*      the pairing takes place. There is a subtle issue with
*      Passkey notification, because RD is able to complete pairing
*      without user actually responding. So here we need to find out
*      if pairing has completed or still ongoing.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScSspNotificationResLocalBondingS(ScInstanceData_t *scData)
{
    if (scData->bondData.appState == APP_RESPOND)
    {
        switch(scData->bondData.dmState)
        {
            case DM_AUTHEN_RECV:
                CsrBtScSspNotificationResLocalAuthRecvS(scData);
                break;

            case DM_BOND_COMPLETE:
                CsrBtScSspNotificationResLocalBondCompleteS(scData);
                break;

            default:
                CsrBtScStateEventException(CSR_BT_SC_PRIM, scData->pendingMsgType, scData->bondData.dmState);
                break;
        }
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScPasskeyResLocalActiveBondingS
*
*  DESCRIPTION
*      The app has replied with a pin.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScPasskeyResLocalActiveBondingS(ScInstanceData_t *scData)
{
    CsrBtScPasskeyRes *scPrim;

    scPrim = (CsrBtScPasskeyRes *)scData->recvMsgP;

    scData->addDevice   = scPrim->addDevice;
    scData->authorised = scPrim->authorised;
    scData->accept      = scPrim->accept;

    SC_CHANGE_STATE(&scData->bondData.appState, APP_RESPONDED);
    SC_CHANGE_STATE(&scData->bondData.dmState, DM_AUTHEN_RESP);

    /* Stop watchdog timer */
    CsrBtScStopBondTimer(scData);

    if (scPrim->accept)
    {
        /* Special handling of empty pin code */
        if (scPrim->passKeyLength == 0)
        {
            scPrim->passKeyLength = 1;
            scPrim->passKey[0]    = '\0';
        }
        CsrBtCmScDmPinRequestRes(scData->deviceAddr,
                            scPrim->passKeyLength,
                            scPrim->passKey);
    }
    else
    {
        CsrBtCmScDmPinRequestNegRes(scData->deviceAddr);
        CsrBtScBondReqCancelHandlerAllowReset(scData, scData->appHandle); /* allow reset */
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScPasskeyResLocalCancelledBondingS
*
*  DESCRIPTION
*      The app has replied with a pin after the bonding procedure
*      has been cancelled.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScPasskeyResLocalCancelledBondingS(ScInstanceData_t *scData)
{
    CsrBtCmScDmPinRequestNegRes(scData->deviceAddr);
    SC_CHANGE_STATE(&scData->bondData.appState, APP_RESPONDED);
    SC_CHANGE_STATE(&scData->bondData.dmState, DM_AUTHEN_RESP);
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScPasskeyResLocalBondingS
*
*  DESCRIPTION
*      The app has replied with a pin.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScPasskeyResLocalBondingS(ScInstanceData_t *scData)
{
    CsrBtScPasskeyRes *scPrim;

     scPrim = (CsrBtScPasskeyRes *)scData->recvMsgP;

    if (scData->bondData.appState == APP_RESPOND &&
        CsrBtBdAddrEq(&scData->deviceAddr, &scPrim->deviceAddr))
    {
        switch(scData->bondData.scState)
        {
            case SC_CANCELLED:
                CsrBtScPasskeyResLocalCancelledBondingS(scData);
                break;

            case SC_ACTIVE:
                CsrBtScPasskeyResLocalActiveBondingS(scData);
                break;

            default:
                CsrBtScStateEventException(CSR_BT_SC_PRIM, scData->pendingMsgType, scData->bondData.dmState);
                break;
        }
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScDmSmLinkKeyIndLocalBondingS
*
*  DESCRIPTION
*      A link key has been generated due to legacy bonding. We don't care
*      if the key was generated due to legacy pairing or SSP.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScDmSmLinkKeyIndLocalBondingS(ScInstanceData_t *scData)
{
    DM_SM_LINK_KEY_IND_T    *dmPrim;

    CsrBtScMapToDmKeyType(scData);

    dmPrim = (DM_SM_LINK_KEY_IND_T *)scData->recvMsgP;

    if (!CsrBtBdAddrEq(&dmPrim->bd_addr, &scData->deviceAddr))
    {
        /* Here we actually check the bd_addr of the primitive. The reason
         * is that this link key might not be generated due to pairing
         * procedure. This might be a product of a remote connected device
         * issuing a HCI_CHANGE_CONN_LINK_KEY request. */
         CsrBtScDmSmLinkKeyIndIdleS(scData);
    }
    else
    {
        if (scData->bondData.scState == SC_CANCELLED)
        {
            /* Now wait for the bonding confirm to arrive */ ;
        }
        else
        {
            /* Wait for the confirm to do update the SC DB, DM SM
             * There is no point in doing that now if the app cancels
             * while we are waiting for the confirm */
            CsrMemCpy(&scData->bondData.linkKey, &dmPrim->key, SIZE_LINK_KEY);
            scData->bondData.linkKeyType = dmPrim->key_type;
        }
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScDmSimplePairingCompleteIndLocalBondingS
*
*  DESCRIPTION
*       Simple pairing has completed.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScDmSimplePairingCompleteIndLocalBondingS(ScInstanceData_t *scData)
{
    DM_SM_SIMPLE_PAIRING_COMPLETE_IND_T *dmPrim;

    dmPrim = (DM_SM_SIMPLE_PAIRING_COMPLETE_IND_T *) scData->recvMsgP;

    if (CsrBtBdAddrEq(&dmPrim->addrt.addr, &scData->deviceAddr))
    {
        ScSetHciStatusIfUnset(scData, dmPrim->status);
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      csrBtScSmBondingCfmSspDmAuthRspS
*
*  DESCRIPTION
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
static void csrBtScSmBondingCfmSspDmAuthRspS(ScInstanceData_t *scData)
{
    DM_SM_BONDING_CFM_T *dmPrim;

    dmPrim = (DM_SM_BONDING_CFM_T *) scData->recvMsgP;

    if (dmPrim->status == HCI_SUCCESS &&
        scData->accept &&
        scData->bondData.scState != SC_CANCELLED)
    {
        CsrBtScNotifyUpperBondCfmSimpleInclUpdDevReset(scData);
    }
    else
    {
        CsrBtScBondReqCancelHandlerAllowReset(scData, scData->appHandle); /* allow reset */
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      ScSmBondingCfmSspAuthRecvS
*
*  DESCRIPTION
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
static void csrBtScSmBondingCfmSspDmAuthRecvS(ScInstanceData_t *scData)
{
    DM_SM_BONDING_CFM_T *dmPrim;

    dmPrim = (DM_SM_BONDING_CFM_T *) scData->recvMsgP;

    if (dmPrim->status == HCI_SUCCESS)
    {
        SC_CHANGE_STATE(&scData->bondData.dmState, DM_BOND_COMPLETE);
        SC_CHANGE_STATE(&scData->bondData.appState, APP_RESPOND);
    }
    else
    {
        CsrBtScBondReqCancelHandlerAllowReset(scData, scData->appHandle); /* allow reset */
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScSmBondingCfmSspBondingS
*
*  DESCRIPTION
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScSmBondingCfmSspBondingS(ScInstanceData_t *scData)
{
    switch(scData->bondData.dmState)
    {
        case DM_AUTHEN_RECV:
            /* Passkey notification */
            csrBtScSmBondingCfmSspDmAuthRecvS(scData);
            break;

        case DM_AUTHEN_RESP:
            /* Passkey, notification and compare */
            csrBtScSmBondingCfmSspDmAuthRspS(scData);
            break;

        case DM_PENDING:
        case DM_CONNECTING:
        case DM_BOND_COMPLETE:
        default:
            CsrBtScStateEventException(CSR_BT_SC_PRIM, scData->pendingMsgType, scData->bondData.dmState);
            break;
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScSmBondingCfmOobBondingS
*
*  DESCRIPTION
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScSmBondingCfmOobBondingS(ScInstanceData_t *scData)
{
    DM_SM_BONDING_CFM_T *dmPrim;

    dmPrim = (DM_SM_BONDING_CFM_T *) scData->recvMsgP;

    if (dmPrim->status == HCI_SUCCESS &&
        scData->bondData.scState != SC_CANCELLED)
    {
        /* Automatic add (due to OOB where the app might not receive an indication) */
        scData->accept = TRUE;
        scData->addDevice = TRUE;

        CsrBtScNotifyUpperBondCfmSimpleInclUpdDevReset(scData);
    }
    else
    {
        CsrBtScBondReqCancelHandlerAllowReset(scData, scData->appHandle); /* allow reset */
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScSmBondingCfmBondingS
*
*  DESCRIPTION
*       Dispatch the bonding confirm to the correct handler.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScSmBondingCfmBondingS(ScInstanceData_t *scData)
{
    DM_SM_BONDING_CFM_T *dmPrim;

    dmPrim = (DM_SM_BONDING_CFM_T *) scData->recvMsgP;

    if (CsrBtBdAddrEq(&dmPrim->addrt.addr, &scData->deviceAddr))
    {
        ScSetHciStatusIfUnset(scData, dmPrim->status);

        if (scData->bondData.scState == SC_CANCELLED)
        {
            /* If the app cancelled we don't care in which state we
             * are and if bonding succeeded or not. We stop here. */
            CsrBtScPairingCancelled(scData);
        }
        else
        {
            switch(scData->pendingMsgType)
            {
                case MSG_TYPE_NONE:
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
                    CsrBtScSmBondingCfmSspBondingS(scData);
                    break;

                case MSG_TYPE_REBOND:
                    /* Cannot occur. We only send a bonding req after bothering the user */
                default:
                    CsrBtScStateEventException(CSR_BT_SC_PRIM, scData->pendingMsgType, scData->bondData.dmState);
                    break;
            }
        }
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScPairingCancelled
*
*  DESCRIPTION
*      Cleanup handler for a bonding request that has been cancelled.
*
*  POSTCONDITION:
*      The RD is removed from SC DB, DM SM and controller.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScPairingCancelled(ScInstanceData_t *scData)
{
    CsrBtScBondReqCancelHandlerAllowReset(scData, scData->appHandle); /* allow reset */
}

