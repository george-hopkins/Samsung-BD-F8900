/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_cm_main.h"
#include "csr_bt_cm_l2cap.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_cm_rfc.h"
#include "csr_bt_cm_dm.h"
#include "csr_bt_cm_sdc.h"
#include "csr_bt_cm_util.h"
#include "csr_bt_cm_bnep.h"
#include "csr_bt_cm_events_handler.h"

#ifdef CSR_BT_INSTALL_CM_LINK_POLICY
static void insertDefaultLinkPolicySettings(cmInstanceData_t *cmData)
{
    CsrUintFast8 i;
    aclTable  *aclConnectionElement = NULL;

    for ( i = 0; i < NUM_OF_ACL_CONNECTION; i++)
    {
        if (!CsrBtBdAddrEqZero(&(cmData->roleVar.aclVar[i].deviceAddr)))
        {
            aclConnectionElement                        = &(cmData->roleVar.aclVar[i]);
            aclConnectionElement->sniffSettings         = cmData->dmVar.defaultSniffSettings;
            aclConnectionElement->parkSettings          = cmData->dmVar.defaultParkSettings;
            aclConnectionElement->linkPolicySettings    = cmData->dmVar.defaultLinkPolicySettings;
        }
    }
}

static void csrBtCmWriteLinkPolicyErrorIndMsgSend(CsrSchedQid appHandle,
                                                  CsrBtDeviceAddr deviceAddr,
                                                  CsrBtResultCode resultCode,
                                                  CsrBtSupplier resultSupplier)
{
    CsrBtCmWriteLinkPolicyErrorInd * prim;

    prim                 = (CsrBtCmWriteLinkPolicyErrorInd *)CsrPmemAlloc(sizeof(CsrBtCmWriteLinkPolicyErrorInd));
    prim->type           = CSR_BT_CM_WRITE_LINK_POLICY_ERROR_IND;
    prim->deviceAddr     = deviceAddr;
    prim->resultCode     = resultCode;
    prim->resultSupplier = resultSupplier;
    CsrBtCmPutMessage(appHandle, prim);
}
#endif

#ifndef EXCLUDE_CSR_BT_RFC_MODULE
void CsrBtCmWriteLpSettingsCompleteRfcHandler(cmInstanceData_t *cmData, cmRfcConnElement * theRfcElement)
{
    if (theRfcElement && theRfcElement->cmRfcConnInst)
    {
        cmRfcConnInstType *theLogicalLink = theRfcElement->cmRfcConnInst;

        switch(theLogicalLink->state)
        {
            case connectInit_s :
                { /* The CM has just ensure that the DM only will accept ACTIVE link
                     policy, in order to prevent a link policy change, while a RFCOMM
                     connection is establish */

                    if (!cmData->rfcVar.cancelConnect)
                    {
                        aclTable       * aclConnectionElement;
                        returnAclConnectionElement(cmData, theLogicalLink->deviceAddr, &aclConnectionElement);

                        if (aclConnectionElement)
                        {
                            /*CSR_BT_CM_STATE_CHANGE(theLogicalLink->state, connected_s);
                            CsrBtCmConnectCfmMsgSend(cmData, CSR_BT_RESULT_CODE_CM_SUCCESS, CSR_BT_SUPPLIER_CM);*/

                            CSR_BT_CM_STATE_CHANGE(theLogicalLink->state, connect_s);

                            CsrBtCmRfcStartInitiateConnection(cmData, theLogicalLink);
                            CsrBtCmDmLocalQueueHandler();
                        }
                        else
                        {
                            /* Existing connection has been terminated, setup a new connection */
                            CsrUint16 clockOffset                    = 0;
                            page_scan_mode_t pageScanMode           = 0;
                            page_scan_rep_mode_t pageScanRepMode    = 0;

                            if(isCacheParamsKnown(cmData, theLogicalLink->deviceAddr, &clockOffset, &pageScanMode, &pageScanRepMode))
                            {
                                CsrBtCmDmWriteCacheParamsDirect(cmData,
                                                                theLogicalLink->deviceAddr,
                                                                clockOffset,
                                                                pageScanMode,
                                                                pageScanRepMode,
                                                                RFC_PLAYER,
                                                                0);
                            }
                            else
                            {
                                CSR_BT_CM_STATE_CHANGE(theLogicalLink->state, connect_s);
                                CsrBtCmRfcStartInitiateConnection(cmData, theLogicalLink);
                                CsrBtCmDmLocalQueueHandler();
                            }
                        }
                    }
                    else
                    { /* The application has requested to cancel the connection */
                        CsrUint8 numOfConnection     = CsrBtCmReturnNumOfConnectionsToPeerDevice(cmData, theLogicalLink->deviceAddr);

                        if(numOfConnection == 0)
                        { /* There is no other connection attach to the device address.
                             Inform the application. */
                            CsrBtCmConnectCfmMsgSend(cmData, CSR_BT_RESULT_CODE_CM_CANCELLED, CSR_BT_SUPPLIER_CM);
                            CsrBtCmDmLocalQueueHandler();
                        }
                        else
                        { /* Currently there is at least one more connection attach to the
                             device address. Ensure that SNIFF and park is supported */
                            cmData->smVar.resultCode        = CSR_BT_RESULT_CODE_CM_CANCELLED;
                            cmData->smVar.resultSupplier    = CSR_BT_SUPPLIER_CM;
                            CsrBtCmDmWriteLpSettingsAllMsgSend(cmData, theLogicalLink->deviceAddr, KEEP_CURRENT_PLAYER);
                            CSR_BT_CM_STATE_CHANGE(theLogicalLink->state, connect_s);
                        }
                    }
                    break;
                }
            case connect_s :
                {/* The attempt to establish a RFC connection is finish. Inform the
                    application about the result and restore the local DM and service
                    manager queue */
                    if (cmData->smVar.resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && cmData->smVar.resultSupplier == CSR_BT_SUPPLIER_CM)
                    {
                        if (!cmData->rfcVar.cancelConnect)
                        {
                            CSR_BT_CM_STATE_CHANGE(theLogicalLink->state, connected_s);
                            CsrBtCmConnectCfmMsgSend(cmData, cmData->smVar.resultCode, cmData->smVar.resultSupplier);
                        }
                        else
                        {
                            CSR_BT_CM_STATE_CHANGE(cmData->rfcVar.connectState, CM_RFC_CANCELING);
                            RfcDisconnectReqSend((CsrUint16)theLogicalLink->btConnId);
                        }
                    }
                    else
                    { /* The attempt to establish a RFC connection fail. Check if the link policy need to be change */
                        CSR_BT_CM_STATE_CHANGE(theLogicalLink->state, idle_s);
                        CsrBtCmModeChangeReqMsgSend(cmData, theLogicalLink->deviceAddr);
                        CsrBtCmConnectCfmMsgSend(cmData, cmData->smVar.resultCode, cmData->smVar.resultSupplier);
                    }
                    CsrBtCmDmLocalQueueHandler();
                    break;
                }

            case releaseInit_s :
                { /* Has just ensure that the link policy can not be change, while the RFC
                     connection    is release */
                    CSR_BT_CM_STATE_CHANGE(theLogicalLink->state, release_s);

#ifndef EXCLUDE_CSR_BT_SCO_MODULE
                    if(theLogicalLink->eScoParms.handle < SCOBUSY_ACCEPT)
                    { /* Before closing down the RFC connection the SCO connection
                         has to be release. */
                        cmData->dmVar.contableIndex  = theRfcElement->elementId;
                        cmData->dmVar.closeScoHandle = theLogicalLink->eScoParms.handle;
                        dm_sync_disconnect_req(theLogicalLink->eScoParms.handle,
                                               HCI_ERROR_OETC_USER);
                    }
                    else
#endif
                    { /* There is no SCO connection, just close down the RFC
                         connection. The call is confirmed by RFC_RELEASE_IND. */
                        RfcDisconnectReqSend((CsrUint16)theLogicalLink->btConnId);
                        CsrBtCmDmLocalQueueHandler();
                    }
                    break;
                }
            case releaseFinal_s :
                {
                    CsrBool  status  = CsrBtCmRfcReleaseStatus((CsrBtReasonCode) cmData->smVar.resultCode,
                                                               cmData->smVar.resultSupplier);

                    CsrBtCmModeChangeReqMsgSend(cmData, theLogicalLink->deviceAddr);
                    CsrBtCmDmSyncClearPcmSlotFromTable(cmData, &theLogicalLink->eScoParms);

                    CsrBtCmDisconnectIndMsgCleanupSend(theRfcElement,
                                                       (CsrBtReasonCode) cmData->smVar.resultCode,
                                                       cmData->smVar.resultSupplier,
                                                       status, TRUE);

                    CsrBtCmServiceManagerLocalQueueHandler(cmData);
                    CsrBtCmDmLocalQueueHandler();
                    break;
                }
            case connectAccept_s :
            case connectAcceptFinal_s :
                { /* The CM has just ensure that the host controller will accept
                     CSR_BT_SNIFF_MODE and CSR_BT_PARK_MODE doing the CSR_BT_CM_CONNECT_ACCEPT_REQ routine */
                    cmData->rfcVar.btConnId        = CSR_BT_CONN_ID_INVALID;
                    CsrBtCmConnectAcceptCfmMsgSend(cmData, theRfcElement, cmData->smVar.resultCode, cmData->smVar.resultSupplier);
                    CsrBtCmDmLocalQueueHandler();
                    break;
                }
            default :
                {
                    CsrBtCmServiceManagerLocalQueueHandler(cmData);
                    CsrBtCmDmLocalQueueHandler();
                    break;
                }
        }
    }
    else
    {
        if (theRfcElement)
        { /* If theRfcElement == NULL then this function has allready been called */
            CsrBtCmServiceManagerLocalQueueHandler(cmData);
        }
        CsrBtCmDmLocalQueueHandler();
    }
}
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
void CsrBtCmWriteLpSettingsCompletel2capHandler(cmInstanceData_t *cmData, cmL2caConnElement * theL2caElement)
{
    if (theL2caElement && theL2caElement->cmL2caConnInst)
    {
        cmL2caConnInstType *l2capLink = theL2caElement->cmL2caConnInst;
        CsrBtDeviceAddr remoteAddr = l2capLink->deviceAddr;

        switch(l2capLink->state)
        {
            case l2capReleaseFinal_s :
                { /* The CM has just ensure that SNIFF and PARK mode is supported after
                     the connection is release. Inform the application and restore the
                     local service manager and DM queue */
                    CsrBtCmModeChangeReqMsgSend(cmData, l2capLink->deviceAddr);
                    CsrBtCmL2capDisconnectIndMsgSend(cmData, theL2caElement,
                                                     (CsrBtReasonCode) cmData->smVar.resultCode,
                                                     cmData->smVar.resultSupplier, TRUE);

                    CsrBtCmWriteAutoFlushTimeout(cmData, &remoteAddr);
                    CsrBtCmDmLocalQueueHandler();
                    CsrBtCmServiceManagerLocalQueueHandler(cmData);
                    break;
                }
            case l2capReleaseInit_s :
                { /* The CM has just ensure that the link policy can not be change, while it
                     disconnect a L2cap connection */
                    CSR_BT_CM_STATE_CHANGE(l2capLink->state, l2capRelease_s);

                    /* Release the l2cap connection on restore the local DM queue */
                    L2CA_DisconnectReq(CM_GET_UINT16ID_FROM_BTCONN_ID(l2capLink->btConnId));
                    CsrBtCmDmLocalQueueHandler();
                    break;
                }
            case l2capReleaseInitXover_s:
                { /* Crossover between local and remote disconnect. The CM has just
                     ensure that low power mode is not supported. Make sure that low
                     power mode is supported, and goto l2capReleaseFinal_s              */
                    CsrBtCmL2caFinalReleaseHandler(cmData, theL2caElement, TRUE,
                                                   (CsrBtReasonCode) cmData->smVar.resultCode,
                                                   cmData->smVar.resultSupplier, FALSE);
                    CsrBtCmDmLocalQueueHandler();
                    break;
                }
            case l2capConnectInit_s :
                {/* The CM has just ensure that the DM only will accept ACTIVE link
                    policy, in order to prevent a link policy change, while a L2CAP
                    connection is establish */

                    if (!cmData->l2caVar.cancelConnect)
                    {
                        /* Check whether we need to apply the legacy
                         * pairing ACL detach work around */
                        if(!CsrBtCmL2caCheckLegacyDetach(cmData, l2capLink))
                        {
                            CsrBtCml2caAutoConnectSetup(cmData, l2capLink);
                            CsrBtCmDmLocalQueueHandler();
                        }
                    }
                    else
                    {
                        CsrBtCmL2caConnectCancelCleanup(cmData, theL2caElement, TRUE);
                    }
                    break;
                }
            case l2capConnect_s :
                {/* The attempt to establish a L2CAP connection is finish. Inform the
                    application about the result, and restore the local service manager and DM
                    queue */
                    if (cmData->smVar.resultCode     == CSR_BT_RESULT_CODE_CM_SUCCESS &&
                        cmData->smVar.resultSupplier == CSR_BT_SUPPLIER_CM)
                    {
                        /* A L2CAP connection has been establish with
                         * success. Inform the application and restore
                         * the local DM and service manager queue */
                        if (!cmData->l2caVar.cancelConnect)
                        {
                            CSR_BT_CM_STATE_CHANGE(l2capLink->state, l2capConnected_s);
                            CsrBtCmL2caConnectCfmMsgHandler(cmData, theL2caElement,
                                                            CSR_BT_RESULT_CODE_CM_SUCCESS, CSR_BT_SUPPLIER_CM);
                            CsrBtCmWriteAutoFlushTimeout(cmData, &remoteAddr);
                            
                            CsrBtCmL2caDataStart(cmData, l2capLink);
                        }
                        else
                        {
                            L2CA_DisconnectReq(CM_GET_UINT16ID_FROM_BTCONN_ID(l2capLink->btConnId));
                        }
                    }
                    else
                    { /* The attempt to establish a l2cap connection fail. Inform the
                         application, clean up the l2cap connection table and restore
                         the local service manager and DM queue */
                        CSR_BT_CM_STATE_CHANGE(l2capLink->state, idle_s);
                        CsrBtCmModeChangeReqMsgSend(cmData, l2capLink->deviceAddr);
                        CsrBtCmL2caConnectCfmMsgHandler(cmData,
                                                        theL2caElement,
                                                        cmData->smVar.resultCode,
                                                        cmData->smVar.resultSupplier);
                    }
                    CsrBtCmDmLocalQueueHandler();
                    break;
                }
            case l2capConnectAcceptFinal_s :
                {
                    CSR_BT_CM_STATE_CHANGE(l2capLink->state, l2capConnected_s);
                    CsrBtCmL2caConnectAcceptCfmHandler(cmData, theL2caElement,
                                                       CSR_BT_RESULT_CODE_CM_SUCCESS, CSR_BT_SUPPLIER_CM);

                    CsrBtCmWriteAutoFlushTimeout(cmData, &remoteAddr);
                    break;
                }
            case l2capSaveDisconnect_s :
                {
                    CsrBtCmL2caConnectAcceptCfmHandler(cmData, theL2caElement,
                                                       CSR_BT_RESULT_CODE_CM_SUCCESS, CSR_BT_SUPPLIER_CM);
                    break;
                }
            default :
                {
                    CsrBtCmServiceManagerLocalQueueHandler(cmData);
                    CsrBtCmDmLocalQueueHandler();
                    break;
                }
        }
    }
    else
    {
        if (theL2caElement)
        { /* If theL2caElement == NULL then this function has allready been called */
            CsrBtCmServiceManagerLocalQueueHandler(cmData);
        }
        CsrBtCmDmLocalQueueHandler();
    }
}
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
void CsrBtCmWriteLpSettingsCompleteBnepHandler(cmInstanceData_t *cmData)
{
    DM_HCI_WRITE_LINK_POLICY_SETTINGS_CFM_T        *dmPrim;
    CsrUint8                                    theIndex;
    bnepTable *bnepConnection;

    dmPrim        = (DM_HCI_WRITE_LINK_POLICY_SETTINGS_CFM_T *) cmData->recvMsgP;
    theIndex    = returnIndexToThisBdAddr(cmData, dmPrim->bd_addr);
    bnepConnection = &(cmData->bnepVar.connectVar[theIndex]);

    switch(cmData->bnepVar.connectVar[theIndex].state)
    {
        case bnepConnectInit_s :
            {/* The CM has just ensure that the DM only will accept ACTIVE link
                policy, in order to prevent a link policy change, while a BNEP
                connection is establish */
                CSR_BT_CM_STATE_CHANGE(cmData->bnepVar.connectVar[theIndex].state, bnepConnect_s);
                CSR_BT_CM_STATE_CHANGE(cmData->bnepVar.connectState, CM_BNEP_CONNECT);

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_INQUIRY_PAGE_STATE
                /* Raise event that connection operation is started */
                CsrBtCmPropgateEvent(cmData,
                                    CsrBtCmPropagateInquiryPageEvents,
                                    CSR_BT_CM_EVENT_MASK_SUBSCRIBE_INQUIRY_PAGE_STATE,
                                    HCI_SUCCESS,
                                    NULL,
                                    NULL);
#endif

                if (!cmData->bnepVar.cancelConnect)
                {
                    cmData->bnepVar.connectVar[theIndex].actualMode    = CSR_BT_ACTIVE_MODE;
                    if (!cmData->bnepVar.moreLpSettingsInProgress)
                    {
                        CsrBtBnepConnectReqSend(cmData->bnepVar.connectReqFlags, CsrBtBdAddrToEtherAddr(&cmData->bnepVar.connectVar[theIndex].deviceAddr));
                    }
                    CsrBtCmDmLocalQueueHandler();
                }
                else
                {
                    CsrUint8 numOfConnection      = CsrBtCmReturnNumOfConnectionsToPeerDevice(cmData, bnepConnection->deviceAddr);
                    cmData->smVar.resultCode     = CSR_BT_RESULT_CODE_CM_CANCELLED;
                    cmData->smVar.resultSupplier = CSR_BT_SUPPLIER_CM;

                    CsrBtCmBnepConnectIndMsgSend(cmData, cmData->bnepVar.appHandle, ID_EMPTY,
                                                 CsrBtBdAddrToEtherAddr(&bnepConnection->deviceAddr), 0, 0, 0,
                                                 cmData->smVar.resultCode, cmData->smVar.resultSupplier);

                    if(numOfConnection == 0)
                    { /* There is no other connection attach to the device address.
                         Inform the application, restore the local service manager
                         queue, and clear up the l2cap connection table                    */
                        CsrBtCmBnepClearBnepTableIndex(bnepConnection);
                        CsrBtCmDmLocalQueueHandler();
                        CsrBtCmServiceManagerLocalQueueHandler(cmData);
                    }
                    else
                    { /* Currently there is at least one more connection attach to the
                         device address. Ensure that SNIFF and park is supported            */
                        CsrBtCmDmWriteLpSettingsAllMsgSend(cmData, bnepConnection->deviceAddr, KEEP_CURRENT_PLAYER);
                        CSR_BT_CM_STATE_CHANGE(bnepConnection->state, bnepConnectCfm_s);
                    }
                    CsrBtCmDmUpdateAndClearCachedParamReqSend(bnepConnection->deviceAddr);
                }
                break;
            }
        case bnepConnectCfm_s :
            {/* The attempt to establish a BNEP connection is finish. Inform the
                application about the result, and restore the local service manager and DM
                queue */
                if (cmData->smVar.resultCode     == CSR_BT_RESULT_CODE_CM_SUCCESS &&
                    cmData->smVar.resultSupplier == CSR_BT_SUPPLIER_CM)
                { /* A BNEP connection has been establish with success.
                     Restore the local DM and service manager queue */
                    CSR_BT_CM_STATE_CHANGE(cmData->bnepVar.connectVar[theIndex].state, bnepConnected_s);
                    cmData->bnepVar.moreLpSettingsInProgress        = FALSE;
                }
                else
                { /* The attempt to establish a bnep connection fail. Clean up the
                     bnep connection table and restore the local service manager and DM queue */
                    CSR_BT_CM_STATE_CHANGE(cmData->bnepVar.connectVar[theIndex].state, idle_s);
                    CsrBtCmModeChangeReqMsgSend(cmData, cmData->bnepVar.indexPtr->deviceAddr);
                    CsrBtCmBnepClearBnepTableIndex(cmData->bnepVar.indexPtr);
                }
                CsrBtCmServiceManagerLocalQueueHandler(cmData);
                CsrBtCmDmLocalQueueHandler();
                break;
            }
        case bnepConnectAcceptFinal_s :
            {
                /* The BNEP connection is now establish with success. Inform the
                   application, and restore the  local service manager and DM queue */
                CsrUint8        numOfConnection;
                CsrUint8        newIndex;

                CSR_BT_CM_STATE_CHANGE(cmData->bnepVar.connectVar[theIndex].state, bnepConnected_s);

                newIndex = returnEmptyBnepIdIndex(cmData);
                if(newIndex != CM_ERROR)
                { /* If it is possible to create a new bnep connection */
                    CsrBtCmBnepConnectAcceptReqInitHandler(cmData, newIndex, cmData->bnepVar.classOfDevice);
                }

                numOfConnection        = CsrBtCmReturnNumOfConnectionsToPeerDevice(cmData, cmData->bnepVar.connectVar[theIndex].deviceAddr);
                if(numOfConnection != 0)
                { /* Currently there is at least one more connection attach to the
                     device address. The bnep connection has been accepted with success. */

                    cmData->bnepVar.connectVar[theIndex].actualMode    = CsrBtCmDmReturnActualMode(cmData, cmData->bnepVar.connectVar[theIndex].deviceAddr);

                    if(cmData->bnepVar.connectVar[theIndex].actualMode != CSR_BT_ACTIVE_MODE)
                    {
                        CsrBtCmDmModeChangeReqMsgSend(BNEP_PLAYER, theIndex, CSR_BT_ACTIVE_MODE, cmData->bnepVar.connectVar[theIndex].deviceAddr);
                    }
                    else
                    {
                        CsrBtCmServiceManagerLocalQueueHandler(cmData);
                    }
                }
                else
                {
                    CsrBtCmServiceManagerLocalQueueHandler(cmData);
                }
                CsrBtCmDmLocalQueueHandler();
                break;
            }
        case bnepDisconnectInit_s :
            { /* The CM has just ensure that the link policy can not be change, while it
                 disconnect a bnep connection */
                CSR_BT_CM_STATE_CHANGE(cmData->bnepVar.connectVar[theIndex].state, bnepDisconnectReq_s);
                CsrBtBnepDisconnectReqSend(0, cmData->bnepVar.connectVar[theIndex].id);
                if (cmData->bnepVar.disconnectInd)
                {
                    CsrUint8        numOfConnection;

                    cmData->bnepVar.disconnectInd = FALSE;
                    numOfConnection        = CsrBtCmReturnNumOfConnectionsToPeerDevice(cmData, cmData->bnepVar.connectVar[theIndex].deviceAddr);

                    if(numOfConnection == 0)
                    { /* Currently there is no other connections attach to the given Bluetooth
                         address. clean up the bnep connection table, and restore the local service manager queue */
                        CsrBtCmBnepClearBnepTableIndex(&cmData->bnepVar.connectVar[theIndex]);
                        CsrBtCmServiceManagerLocalQueueHandler(cmData);
                        CsrBtCmDmLocalQueueHandler();
                    }
                    else
                    { /* Currently there is at least one more connection attach to the given device
                         address. Ensure that the SNIFF and PARK mode is supported */
                        CsrBtCmDmWriteLpSettingsAllMsgSend(cmData, cmData->bnepVar.connectVar[theIndex].deviceAddr, BNEP_PLAYER);
                    }
                }
                break;
            }
        case bnepDisconnectReq_s :
            { /* The CM has just ensure that SNIFF and PARK mode is supported after
                 the connection is release. If requested mode is different from active,
                 change to requested mode and restore the local service manager and DM queue */
                CSR_BT_CM_STATE_CHANGE(cmData->bnepVar.connectVar[theIndex].state, idle_s);
                CsrBtCmModeChangeReqMsgSend(cmData, cmData->bnepVar.indexPtr->deviceAddr);
                CsrBtCmBnepClearBnepTableIndex(&cmData->bnepVar.connectVar[theIndex]);
                CsrBtCmServiceManagerLocalQueueHandler(cmData);
                CsrBtCmDmLocalQueueHandler();
                break;
            }
        case bnepDisconnectInd_s :
            { /* The CM has just ensure that SNIFF and PARK mode is supported after
                 the connection is released. If  requested link policy mode is different
                 from active, change to requested mode and clear the bnep connection table */
                CSR_BT_CM_STATE_CHANGE(cmData->bnepVar.connectVar[theIndex].state, idle_s);
                CsrBtCmModeChangeReqMsgSend(cmData, cmData->bnepVar.indexPtr->deviceAddr);
                CsrBtCmBnepClearBnepTableIndex(cmData->bnepVar.indexPtr);
                CsrBtCmDmLocalQueueHandler();
                break;
            }
        default :
            {
                CsrBtCmServiceManagerLocalQueueHandler(cmData);
                CsrBtCmDmLocalQueueHandler();
                break;
            }
    }
}
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE */

void CsrBtCmWriteLpSettingsCompleteCmHandler(cmInstanceData_t *cmData)
{
    DM_HCI_WRITE_LINK_POLICY_SETTINGS_CFM_T        *dmPrim;
    CsrUint8                                 numOfConnection;

    CsrUint8 numOfAcl    = returnNumOfAclConnection(cmData);
    dmPrim                = (DM_HCI_WRITE_LINK_POLICY_SETTINGS_CFM_T *) cmData->recvMsgP;
    numOfConnection     = CsrBtCmReturnNumOfConnectionsToPeerDevice(cmData, dmPrim->bd_addr);

    if (numOfAcl == 0 || numOfConnection == 0)
    { /* The local device has no ACL connection or no logical connections
         therefor a role switch cannot be performed. Just restore the queues */
        CsrBtCmDmLocalQueueHandler();
        CsrBtCmServiceManagerLocalQueueHandler(cmData);
    }
    else
    {
        if (cmData->roleVar.roleSwitchFlag)
        {
            dm_hci_switch_role(&dmPrim->bd_addr, cmData->roleVar.requestedRole, NULL);
            cmData->dmVar.roleSwitchInProgress    = TRUE;
            cmData->dmVar.switchRoleDeviceAddr    = dmPrim->bd_addr;
        }
        else
        {
            CsrUint8 actualMode  = CsrBtCmDmReturnActualMode(cmData, dmPrim->bd_addr);

            if (actualMode == CSR_BT_ACTIVE_MODE && cmData->dmVar.roleSwitchLinkMode != CSR_BT_ACTIVE_MODE)
            { /* The link were set in CSR_BT_ACTIVE_MODE only to make a Role Switch.
                 This request change it back again */
                cmData->smVar.operatingBdAddr   = dmPrim->bd_addr;
                cmData->dmVar.requestedLinkMode = cmData->dmVar.roleSwitchLinkMode;
                CSR_BT_CM_STATE_CHANGE(cmData->dmVar.linkPolicyState, LINK_POLICY_ROLE_SWITCH_STATE);
                CsrBtCmDmHciModeChangeFromActive(cmData, dmPrim->bd_addr, cmData->dmVar.requestedLinkMode);
            }
            else
            {
                cmData->dmVar.roleSwitchLinkMode    = CSR_BT_ACTIVE_MODE;
                if (numOfAcl == 1)
                { /* The local device has only one ACL connection. The process is
                     finish restore the queues */
                    CsrBtCmDmLocalQueueHandler();
                    CsrBtCmServiceManagerLocalQueueHandler(cmData);
                }
                else
                { /* The local device has more than one ACL connection. */
                    aclTable   * nextAclConnectionElement;

                    returnNextAclConnectionElement(cmData, &nextAclConnectionElement);

                    if (nextAclConnectionElement)
                    {
                        if (nextAclConnectionElement->role == CSR_BT_MASTER_ROLE)
                        {/* The role is right just make sure that a
                            master slave switch is not supported */
                            cmData->roleVar.roleSwitchFlag        = FALSE;
                            nextAclConnectionElement->roleChecked = TRUE;
                            CsrBtCmDmWriteLpSettingsOnlyLowPowerMsgSend(cmData, nextAclConnectionElement->deviceAddr, CM_PLAYER);
                        }
                        else
                        { /* Need to change make a role switch */
                            CsrBtCmDmModeChangeRoleSwitchHandler(cmData, nextAclConnectionElement->deviceAddr);
                        }
                    }
                    else
                    { /* The process is finish restore the queues */
                        CsrBtCmDmLocalQueueHandler();
                        CsrBtCmServiceManagerLocalQueueHandler(cmData);
                    }
                }
            }

        }
    }
}

static void csrBtCmWriteLpSettingsCompleteCmLinkPolicyHandler(cmInstanceData_t *cmData)
{
    CsrUint8                                 newMode, actualMode;
    DM_HCI_WRITE_LINK_POLICY_SETTINGS_CFM_T        *dmPrim;

    dmPrim        = (DM_HCI_WRITE_LINK_POLICY_SETTINGS_CFM_T *) cmData->recvMsgP;

    actualMode  = CsrBtCmDmReturnActualMode(cmData, dmPrim->bd_addr);
    newMode        = CsrBtCmDmReturnRequestedMode(cmData, dmPrim->bd_addr);

    if(newMode != actualMode)
    { /* The the current link policy must be changed */
        cmData->smVar.operatingBdAddr        = dmPrim->bd_addr;
        CsrBtCmDmHciModeChangeReqMsgSend(cmData, dmPrim->bd_addr, newMode, actualMode);
    }
    else
    {
        CsrBtCmDmLocalQueueHandler();
        CsrBtCmServiceManagerLocalQueueHandler(cmData);
    }

}

static void csrBtCmWriteLpSettingsCompleteCmSwitchRolePlayerHandler(cmInstanceData_t *cmData)
{
    DM_HCI_WRITE_LINK_POLICY_SETTINGS_CFM_T *dmPrim;
    aclTable      *aclConnectionElement;
    aclRoleVars_t *roleVars;
    CsrBool        matched;

    dmPrim = (DM_HCI_WRITE_LINK_POLICY_SETTINGS_CFM_T *) cmData->recvMsgP;

    /* Match requested BD_ADDR against the complete event */
    matched = CsrBtBdAddrEq(&cmData->smVar.operatingBdAddr, &dmPrim->bd_addr);

    returnAclConnectionElement(cmData, dmPrim->bd_addr, &aclConnectionElement);

    roleVars = CsrBtCmDmGetAclRoleVars(aclConnectionElement);

    if(dmPrim->status == HCI_SUCCESS && aclConnectionElement && matched)
    {
        switch(roleVars->state)
        {
            case CM_SWITCH_ROLE_RESTORE_MSS:
                CSR_BT_CM_STATE_CHANGE(roleVars->state, CM_SWITCH_ROLE_IDLE);
                CM_CHANGE_PLAYER(&cmData->smVar.activePlayer, UNDEFINED_PLAYER);
                CsrBtCmRoleSwitchCfmSend(cmData->dmVar.appHandle,
                                         CSR_BT_RESULT_CODE_CM_SUCCESS,
                                         CSR_BT_SUPPLIER_CM,
                                         cmData->smVar.operatingBdAddr,
                                         aclConnectionElement->role,
                                         roleVars->roleType);
                CsrBtCmDmLocalQueueHandler();
                CsrBtCmServiceManagerLocalQueueHandler(cmData);
                break;

            case CM_SWITCH_ROLE_ALLOW_MSS:
                if (roleVars->role != aclConnectionElement->role)
                { /* Role doesn't match. We need to change it */
                    CSR_BT_CM_STATE_CHANGE(roleVars->state, CM_SWITCH_ROLE_DO);
                    dm_hci_switch_role(&cmData->smVar.operatingBdAddr, roleVars->role, NULL);
                    cmData->dmVar.appHandle = cmData->smVar.appHandle;
                    cmData->dmVar.roleSwitchInProgress = TRUE;
                    cmData->dmVar.switchRoleDeviceAddr = dmPrim->bd_addr;
                    break;
                }
                /* !! FALL THROUGH !!*/

                /* Crossover: the peer device changed the role while we preparing for it.
                   This should never happen since we only change the LP setting if we need to */

            case CM_SWITCH_ROLE_DO:
            default:
                CsrBtCmStateEventException(DM_PRIM, dmPrim->type, roleVars->state);
                CM_CHANGE_PLAYER(&cmData->smVar.activePlayer, UNDEFINED_PLAYER);
                CsrBtCmRoleSwitchCfmSend(cmData->dmVar.appHandle,
                                         CSR_BT_RESULT_CODE_CM_INTERNAL_ERROR,
                                         CSR_BT_SUPPLIER_CM,
                                         cmData->smVar.operatingBdAddr,
                                         CSR_BT_UNDEFINED_ROLE,
                                         CSR_BT_CM_SWITCH_ROLE_TYPE_INVALID);
                CsrBtCmDmLocalQueueHandler();
                CsrBtCmServiceManagerLocalQueueHandler(cmData);
                CsrBtCmDmAclRoleVarsClear(roleVars);
                break;
        }
    }
    else
    {
        CsrBtResultCode     resultCode;
        CsrBtSupplier resultSupplier;

        CM_CHANGE_PLAYER(&cmData->smVar.activePlayer, UNDEFINED_PLAYER);

        if (dmPrim->status == HCI_SUCCESS)
        {
            resultSupplier = CSR_BT_SUPPLIER_CM;
            if (!aclConnectionElement)
            {
                resultCode = CSR_BT_RESULT_CODE_CM_UNKNOWN_CONNECTION_IDENTIFIER;
            }
            else
            {
                resultCode = CSR_BT_RESULT_CODE_CM_INTERNAL_ERROR;
            }
        }
        else
        {
            resultSupplier = CSR_BT_SUPPLIER_CM;
            resultCode     = (CsrBtResultCode) dmPrim->status;
        }

        CsrBtCmRoleSwitchCfmSend(cmData->dmVar.appHandle,
                                 resultCode,
                                 resultSupplier,
                                 cmData->smVar.operatingBdAddr,
                                 CSR_BT_UNDEFINED_ROLE,
                                 CSR_BT_CM_SWITCH_ROLE_TYPE_INVALID);

        CsrBtCmDmLocalQueueHandler();
        CsrBtCmServiceManagerLocalQueueHandler(cmData);
        CsrBtCmDmAclRoleVarsClear(roleVars);
    }
}

void CsrBtCmDmHciWriteLpSettingsCompleteHandler(cmInstanceData_t *cmData)
{
    switch(cmData->smVar.activePlayer)
    {
#ifndef EXCLUDE_CSR_BT_RFC_MODULE
        case RFC_PLAYER:
            {
                CsrBtCmWriteLpSettingsCompleteRfcHandler(cmData, cmData->rfcVar.connElement);
                break;
            }
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
        case L2CAP_PLAYER:
            {
                CsrBtCmWriteLpSettingsCompletel2capHandler(cmData, cmData->l2caVar.connElement);
                break;
            }
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
        case BNEP_PLAYER:
            {
                CsrBtCmWriteLpSettingsCompleteBnepHandler(cmData);
                break;
            }
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE */

        case CM_PLAYER:
            {
                CsrBtCmWriteLpSettingsCompleteCmHandler(cmData);
                break;
            }

        case CM_LINK_POLICY_PLAYER:
            {
                csrBtCmWriteLpSettingsCompleteCmLinkPolicyHandler(cmData);
                break;
            }

        case CM_ROLE_SWITCH_PLAYER:
            {
                csrBtCmWriteLpSettingsCompleteCmSwitchRolePlayerHandler(cmData);
                break;
            }

        default:
            {
                CsrBtCmServiceManagerLocalQueueHandler(cmData);
                CsrBtCmDmLocalQueueHandler();
                break;
            }
    }
}

static void csrBtCmDmWriteLpSettingsReqRoleSwitchHandler(cmInstanceData_t *cmData)
{
    aclTable *aclConnectionElement;

    aclRoleVars_t *roleVars;
    CsrBtCmDmWriteLpSettingsReq *prim;

    prim = (CsrBtCmDmWriteLpSettingsReq *) cmData->recvMsgP;

    returnAclConnectionElement(cmData, prim->deviceAddr, &aclConnectionElement);
    roleVars = CsrBtCmDmGetAclRoleVars(aclConnectionElement);

    if (aclConnectionElement && CsrBtBdAddrEq(&cmData->smVar.operatingBdAddr, &prim->deviceAddr))
    {
        cmData->dmVar.appHandle = cmData->smVar.appHandle;
        CsrBtCmDmWriteLpSettingsOverrideMssMsgSend(cmData,
                                                   prim->deviceAddr,
                                                   prim->activePlayer,
                                                   prim->link_policy_settings);
    }
    else
    {
        /* No ACL/match. Restore queue */
        CsrBtResultCode     resultCode;

        if (!aclConnectionElement)
        {
            resultCode = CSR_BT_RESULT_CODE_CM_UNKNOWN_CONNECTION_IDENTIFIER;
        }
        else
        {
            resultCode = CSR_BT_RESULT_CODE_CM_INTERNAL_ERROR;
        }
        CM_CHANGE_PLAYER(&cmData->smVar.activePlayer, UNDEFINED_PLAYER);
        CsrBtCmRoleSwitchCfmSend(cmData->smVar.appHandle,
                                 resultCode,
                                 CSR_BT_SUPPLIER_CM,
                                 cmData->smVar.operatingBdAddr,
                                 CSR_BT_UNDEFINED_ROLE,
                                 CSR_BT_CM_SWITCH_ROLE_TYPE_INVALID);
        CsrBtCmDmLocalQueueHandler();
        CsrBtCmServiceManagerLocalQueueHandler(cmData);
        CsrBtCmDmAclRoleVarsClear(roleVars);
    }
}

void CsrBtCmDmWriteLpSettingsReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmDmWriteLpSettingsReq *prim;

    prim = (CsrBtCmDmWriteLpSettingsReq *) cmData->recvMsgP;

    switch(prim->activePlayer)
    {
        case CM_ROLE_SWITCH_PLAYER:
            csrBtCmDmWriteLpSettingsReqRoleSwitchHandler(cmData);
            break;

        default:
            CsrBtCmDmWriteLpSettingsAllMsgSend(cmData, prim->deviceAddr, prim->activePlayer);
            break;
    }
}

void CsrBtCmDmWriteLpSettingsReqMsgSendExt(CsrBtDeviceAddr deviceAddr, CsrUint8 player, link_policy_settings_t link_policy_settings)
{ /* Build the intern CM_WRITE_LP_SETTINGS_REQ primitive and send it to itself.
     Hereby it will be control by the CsrBtCmDmProvider */
    CsrBtCmDmWriteLpSettingsReq *prim;

    prim                       = (CsrBtCmDmWriteLpSettingsReq *) CsrPmemAlloc(sizeof(CsrBtCmDmWriteLpSettingsReq));
    prim->type                 = CSR_BT_CM_DM_WRITE_LP_SETTINGS_REQ;
    prim->deviceAddr           = deviceAddr;
    prim->link_policy_settings = link_policy_settings;
    prim->activePlayer         = player;
    CsrBtCmPutMessage(CSR_BT_CM_IFACEQUEUE, prim);
}

void CsrBtCmDmWriteLpSettingsReqMsgSend(CsrBtDeviceAddr deviceAddr, CsrUint8 player)
{
    /* 0 = link_policy_settings parameter CSR_UNUSED */
    CsrBtCmDmWriteLpSettingsReqMsgSendExt(deviceAddr, player, 0);
}

#ifdef CSR_BT_INSTALL_CM_LINK_POLICY
void CsrBtCmWriteLinkPolicyReqHandler(cmInstanceData_t *cmData)
{

    CsrBtCmWriteLinkPolicyReq * cmPrim = (CsrBtCmWriteLinkPolicyReq *) cmData->recvMsgP;

    if (CsrBtBdAddrEqZero(&cmPrim->deviceAddr))
    {
        link_policy_settings_t policy = ((link_policy_settings_t)
                                         (cmPrim->linkPolicySetting & (ENABLE_SNIFF | ENABLE_PARK | ENABLE_MS_SWITCH)));

        if (cmPrim->sniffSettings)
        {
            cmData->dmVar.defaultSniffSettings  = *cmPrim->sniffSettings;
        }

        if (cmPrim->parkSettings)
        {
            cmData->dmVar.defaultParkSettings   = *cmPrim->parkSettings;
        }

        if (cmPrim->setupLinkPolicySetting)
        { /* cmData->dmVar.defaultLinkPolicySettings is ONLY use to control 
             which low Power modes are supported */ 
            cmData->dmVar.defaultLinkPolicySettings = ((link_policy_settings_t)
                                                       (cmPrim->linkPolicySetting & (ENABLE_SNIFF | ENABLE_PARK)));
        }

        insertDefaultLinkPolicySettings(cmData);
        cmData->smVar.appHandle = cmPrim->appHandle;
        cmData->smVar.operatingBdAddr = cmPrim->deviceAddr;

        /* This sets both DM and HCI default policy */
        dm_set_default_link_policy_req(policy, policy, NULL);
    }
    else
    {
        aclTable       * aclConnectionElement;

        CsrUint8 index = returnAclConnectionElement(cmData, cmPrim->deviceAddr, &aclConnectionElement);

        if (aclConnectionElement)
        {
            if (cmPrim->sniffSettings)
            {
                aclConnectionElement->sniffSettings = *cmPrim->sniffSettings;
            }
            if (cmPrim->parkSettings)
            {
                aclConnectionElement->parkSettings  = *cmPrim->parkSettings;
            }
            if (cmPrim->setupLinkPolicySetting)
            {
                aclConnectionElement->linkPolicySettings = ((link_policy_settings_t)
                                                            (cmPrim->linkPolicySetting & (ENABLE_SNIFF | ENABLE_PARK)));
            }

#ifndef EXCLUDE_CSR_BT_RFC_MODULE
            {
                cmRfcConnElement *currentRfcElem;

                for (currentRfcElem = CM_RFC_GET_FIRST(cmData->rfcVar.connList); currentRfcElem; currentRfcElem = currentRfcElem->next)
                { /* Search through the RFC connection list                     */
                    if (currentRfcElem->cmRfcConnInst)
                    {
                        if(currentRfcElem->cmRfcConnInst->state == connected_s)
                        { /* The application has a RFCOMM connection */
                            if(bd_addr_eq(&cmPrim->deviceAddr, &(currentRfcElem->cmRfcConnInst->deviceAddr)))
                            { /* The given device address is right. Build and send
                                 CSR_BT_CM_RFC_MODE_CHANGE_IND to the application with the current
                                 link policy. */
                                cmRfcConnInstType *theLogicalLink   = currentRfcElem->cmRfcConnInst;

                                if (cmPrim->sniffSettings)
                                {
                                    theLogicalLink->sniffSettings    = *cmPrim->sniffSettings;
                                    theLogicalLink->lowPowerPriority = DEFAULT_PRIORITY;
                                }

                                if (cmPrim->parkSettings)
                                {
                                    theLogicalLink->parkSettings  = *cmPrim->parkSettings;
                                }
                            }
                        }
                    }
                }
            }
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
            {
                cmL2caConnElement *currentL2caElem;

                for (currentL2caElem = CM_L2CA_GET_FIRST(cmData->l2caVar.connList); currentL2caElem; currentL2caElem = currentL2caElem->next)
                { /* Search through the L2ca connection list                     */
                    if (currentL2caElem->cmL2caConnInst)
                    {
                        if(currentL2caElem->cmL2caConnInst->state == l2capConnected_s)
                        { /* The application has a L2cap connection */
                            if(bd_addr_eq(&cmPrim->deviceAddr, &(currentL2caElem->cmL2caConnInst->deviceAddr)))
                            { /* The given device address is right. Build and send
                                 CSR_BT_CM_L2CA_MODE_CHANGE_IND to the application with the current
                                 link policy. */
                                cmL2caConnInstType *l2CaConnection = currentL2caElem->cmL2caConnInst;

                                if (cmPrim->sniffSettings)
                                {
                                    l2CaConnection->sniffSettings = *cmPrim->sniffSettings;
                                    l2CaConnection->lowPowerPriority = DEFAULT_PRIORITY;
                                }

                                if (cmPrim->parkSettings)
                                {
                                    l2CaConnection->parkSettings  = *cmPrim->parkSettings;
                                }
                            }
                        }
                    }
                }
            }
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
            {
                CsrUintFast8 i;

                for( i = 0; i < CSR_BT_MAX_NUM_OF_SIMULTANEOUS_BNEP_CONNECTIONS; i++ )
                { /* Search through the BNEP connection table */

                    if( cmData->bnepVar.connectVar[i].state == bnepConnected_s ||
                        cmData->bnepVar.connectVar[i].state == bnepDisconnectInit_s)
                    {
                        if(bd_addr_eq(&cmPrim->deviceAddr, &(cmData->bnepVar.connectVar[i].deviceAddr)))
                        { /* Thie given address is right. Store the actual link policy in the connection table */
                            bnepTable *bnepConnection;

                            bnepConnection    = &(cmData->bnepVar.connectVar[i]);

                            if (cmPrim->sniffSettings)
                            {
                                bnepConnection->sniffSettings = *cmPrim->sniffSettings;
                                bnepConnection->lowPowerPriority = DEFAULT_PRIORITY;
                            }

                            if (cmPrim->parkSettings)
                            {
                                bnepConnection->parkSettings  = *cmPrim->parkSettings;
                            }
                        }
                    }
                }
            }
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE */

            CsrBtCmDmModeChangeReqMsgSend(CM_LINK_POLICY_PLAYER, index, CSR_BT_ACTIVE_MODE, cmPrim->deviceAddr);
        }
        else
        {
            csrBtCmWriteLinkPolicyErrorIndMsgSend(cmPrim->appHandle, cmPrim->deviceAddr, CSR_BT_RESULT_CODE_CM_UNKNOWN_CONNECTION_IDENTIFIER, CSR_BT_SUPPLIER_CM);
            CsrBtCmServiceManagerLocalQueueHandler(cmData);
        }
    }
    CsrPmemFree(cmPrim->parkSettings);
    CsrPmemFree(cmPrim->sniffSettings);
}

void CsrBtCmReadLinkPolicyReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmReadLinkPolicyReq        * cmPrim;
    CsrBtCmReadLinkPolicyCfm        * prim;

    cmPrim                          = (CsrBtCmReadLinkPolicyReq *) cmData->recvMsgP;
    prim                            = (CsrBtCmReadLinkPolicyCfm *)CsrPmemAlloc(sizeof(CsrBtCmReadLinkPolicyCfm));
    prim->type                      = CSR_BT_CM_READ_LINK_POLICY_CFM;
    prim->deviceAddr                = cmPrim->deviceAddr;
    prim->resultSupplier            = CSR_BT_SUPPLIER_CM;

    if (CsrBtBdAddrEqZero(&cmPrim->deviceAddr))
    {
        prim->resultCode            = CSR_BT_RESULT_CODE_CM_SUCCESS;
        prim->actualMode            = CSR_BT_ACTIVE_MODE;
        prim->linkPolicySetting     = cmData->dmVar.defaultLinkPolicySettings;
        prim->parkSettings          = cmData->dmVar.defaultParkSettings;
        prim->sniffSettings         = cmData->dmVar.defaultSniffSettings;
    }
    else
    {
        aclTable       * aclConnectionElement;

        returnAclConnectionElement(cmData, cmPrim->deviceAddr, &aclConnectionElement);

        if (aclConnectionElement)
        {
            prim->resultCode        = CSR_BT_RESULT_CODE_CM_SUCCESS;
            prim->actualMode        = CsrBtCmDmReturnActualMode(cmData, cmPrim->deviceAddr);
            prim->linkPolicySetting = aclConnectionElement->linkPolicySettings;
            prim->parkSettings      = aclConnectionElement->parkSettings;
            prim->sniffSettings     = aclConnectionElement->sniffSettings;
        }
        else
        {
            prim->resultCode        = CSR_BT_RESULT_CODE_CM_UNKNOWN_CONNECTION_IDENTIFIER;
            prim->actualMode        = CSR_BT_ACTIVE_MODE;
            prim->linkPolicySetting = cmData->dmVar.defaultLinkPolicySettings;
            prim->parkSettings      = cmData->dmVar.defaultParkSettings;
            prim->sniffSettings     = cmData->dmVar.defaultSniffSettings;
        }

    }
    CsrBtCmPutMessage(cmPrim->appHandle, prim);
    CsrBtCmServiceManagerLocalQueueHandler(cmData);
}
#endif
void CsrBtCmDmHciWriteDefaultLinkPolicySettingsCompleteHandler(cmInstanceData_t *cmData)
{
    DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_CFM_T *dmPrim;
    dmPrim = (DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_CFM_T*)cmData->recvMsgP;

    if(cmData->globalState == notReady_s)
    {
        /* We're still initialising the CM */

        if(dmPrim->status != HCI_SUCCESS)
        {
            /* Can't do much but throw an exception and continue */
            CsrBtCmGeneralException(DM_PRIM, dmPrim->type, "HCI command failed");
        }

        /* Next init command */
        dm_hci_write_page_scan_type(cmData->dmVar.pagescanType,
                                    NULL);

        /* The init-sequence does not use locking */
    }
    else
    {
        /* Normal CM operation. Send error indication to application
         * in case the HCI command failed, otherwise keep quiet as
         * there is no CFM for this */
#ifdef CSR_BT_INSTALL_CM_LINK_POLICY
        if((dmPrim->status != HCI_SUCCESS) &&
           (cmData->smVar.appHandle != CSR_BT_CM_IFACEQUEUE))
        {
            csrBtCmWriteLinkPolicyErrorIndMsgSend(cmData->smVar.appHandle,
                                                  cmData->smVar.operatingBdAddr,
                                                  dmPrim->status,
                                                  CSR_BT_SUPPLIER_HCI);
        }
#endif
        /* Unlock queue */
        CsrBtCmServiceManagerLocalQueueHandler(cmData);
    }
}

#ifdef CSR_BT_INSTALL_CM_SWITCH_ROLE_PUBLIC
void CsrBtCmAlwaysMasterDevicesReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmAlwaysMasterDevicesReq * cmPrim = (CsrBtCmAlwaysMasterDevicesReq *) cmData->recvMsgP;
    cmData->dmVar.appHandle                = cmPrim->phandle; 
    cmData->dmVar.operatingBdAddr          = cmPrim->deviceAddr; 
    dm_lp_write_always_master_devices_req(cmPrim->operation,
                                          &cmPrim->deviceAddr,
                                          NULL);
}   
void CsrBtCmAlwaysMasterDevicesCfmHandler(cmInstanceData_t *cmData)
{
    DM_LP_WRITE_ALWAYS_MASTER_DEVICES_CFM_T *dmPrim = (DM_LP_WRITE_ALWAYS_MASTER_DEVICES_CFM_T*) cmData->recvMsgP;

    CsrBtCmAlwaysMasterDevicesCfm *cmPrim = (CsrBtCmAlwaysMasterDevicesCfm *)
                                             CsrPmemAlloc(sizeof(CsrBtCmAlwaysMasterDevicesCfm));


    cmPrim->type            = CSR_BT_CM_ALWAYS_MASTER_DEVICES_CFM;
    cmPrim->deviceAddr      = cmData->dmVar.operatingBdAddr;
    cmPrim->resultSupplier  = CSR_BT_SUPPLIER_CM;
    cmPrim->resultCode      = CSR_BT_RESULT_CODE_CM_SUCCESS;  
    
    if(dmPrim->status != HCI_SUCCESS)
    {
        cmPrim->resultSupplier = CSR_BT_SUPPLIER_HCI;
        cmPrim->resultCode     = (CsrBtResultCode) dmPrim->status;
    }
    CsrBtCmPutMessage(cmData->dmVar.appHandle, cmPrim);
    CsrBtCmDmLocalQueueHandler();
}
#endif


