/****************************************************************************

                (c) CSR plc 2008

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE

#include "csr_bt_cm_bnep.h"
#include "csr_bt_cm_dm.h"
#include "csr_bt_cm_util.h"
#include "csr_bt_cm_l2cap.h"
#include "csr_bt_cm_events_handler.h"

static CsrUint8 returnCancelBnepConnectIndex(cmInstanceData_t *cmData, CsrBtDeviceAddr deviceAddr)
{
    if (cmData->smVar.smInProgress && cmData->smVar.smMsgTypeInProgress == CSR_BT_CM_BNEP_CONNECT_REQ)
    {
        CsrUintFast8 i;

        for ( i = 0; i < CSR_BT_MAX_NUM_OF_SIMULTANEOUS_BNEP_CONNECTIONS; i++ )
        {
            if (bd_addr_eq(&(cmData->bnepVar.connectVar[i].deviceAddr), &(deviceAddr)))
            {
                if (cmData->bnepVar.connectVar[i].state == bnepConnect_s ||
                    cmData->bnepVar.connectVar[i].state == bnepConnectInit_s ||
                    cmData->bnepVar.connectVar[i].state == bnepConnectCfm_s)
                {
                    return (CsrUint8)i;
                }
            }
        }
    }
    return CM_ERROR;
}

void CsrBtCmBnepConnectAcceptCfmMsgSend(cmInstanceData_t *cmData, CsrSchedQid appHandle,
                                        CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{ /* Send a CM_BNEP_ACCEPT_CONNECT_CFM signal to the application, and restore
     the local service manager queue */
    CsrBtCmBnepConnectAcceptCfm    * cmPrim;

    cmPrim    = (CsrBtCmBnepConnectAcceptCfm *)CsrPmemAlloc(sizeof(CsrBtCmBnepConnectAcceptCfm));

    cmPrim->type            = CSR_BT_CM_BNEP_CONNECT_ACCEPT_CFM;
    cmPrim->resultCode      = resultCode;
    cmPrim->resultSupplier  = resultSupplier;
    CsrBtCmPutMessage(appHandle, cmPrim);
    CsrBtCmServiceManagerLocalQueueHandler(cmData);
}

void CsrBtCmBnepConnectIndMsgSend(cmInstanceData_t *cmData, CsrSchedQid appHandle, CsrUint16 theId,
                             ETHER_ADDR theRem_addr, CsrUint16 theRem_uuid, CsrUint16 theLoc_uuid, CsrUint16 theFrameSize,
                             CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{ /* Send a CSR_BT_CM_BNEP_CONNECT_IND signal to the application */

    CsrBtCmBnepConnectInd    * cmPrim;

    cmPrim    = (CsrBtCmBnepConnectInd *)CsrPmemAlloc(sizeof(CsrBtCmBnepConnectInd));

    cmPrim->type                 = CSR_BT_CM_BNEP_CONNECT_IND;
    cmPrim->resultCode           = resultCode;
    cmPrim->resultSupplier       = resultSupplier;
    cmPrim->id                   = theId;
    cmPrim->rem_addr             = theRem_addr;
    cmPrim->rem_uuid             = theRem_uuid;
    cmPrim->loc_uuid             = theLoc_uuid;
    cmPrim->profileMaxFrameSize = theFrameSize;
    CsrBtCmPutMessage(appHandle, cmPrim);

    if (resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        CsrBtDeviceAddr   deviceAddr = CsrBtBdAddrFromEtherAddr (&theRem_addr);
        cmDmAclKickRoleSwitch(cmData);
        /* AMP-Coexistance */
        /* Update ACL table and trigger event if needed */
        if (updateLogicalChannelTypeMaskAndNumber(cmData,&deviceAddr))
        {/* Changed: issue event */
#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_CHANNEL_TYPE
            aclTable *aclConnectionElement;

            returnAclConnectionElement(cmData,deviceAddr,&aclConnectionElement);

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

void CsrBtCmBnepCancelConnectAcceptCfmMsgSend(CsrSchedQid appHandle, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{ /* Send a CSR_BT_CM_BNEP_CANCEL_CONNECT_ACCEPT_CFM signal to the application, and
     restore the local service manager queue */
    CsrBtCmBnepCancelConnectAcceptCfm    * cmPrim;

    cmPrim    = (CsrBtCmBnepCancelConnectAcceptCfm *)CsrPmemAlloc(sizeof(CsrBtCmBnepCancelConnectAcceptCfm));

    cmPrim->type            = CSR_BT_CM_BNEP_CANCEL_CONNECT_ACCEPT_CFM;
    cmPrim->resultCode      = resultCode;
    cmPrim->resultSupplier  = resultSupplier;
    CsrBtCmPutMessage(appHandle, cmPrim);
}

/*************************************************************************************
  CsrBtCmBnepConnectReqHandler
************************************************************************************/
void CsrBtCmBnepConnectReqHandler(cmInstanceData_t *cmData)
{ /* The application request to create a new bnep connection. */
    CsrUint8                    theIndex;
    CsrBtCmBnepConnectReq    *cmPrim;

    cmPrim    = (CsrBtCmBnepConnectReq *) cmData->recvMsgP;

    if (returnIndexToThisBdAddr(cmData, CsrBtBdAddrFromEtherAddr(&cmPrim->rem_addr)) != CM_ERROR)
    { /* Currently there is already a connection on this device address, Reject the
         connection req only one connection possible per. device. */
        CsrBtCmBnepConnectIndMsgSend(cmData, cmData->bnepVar.appHandle, ID_EMPTY, cmPrim->rem_addr,
                    0, 0, 0, CSR_BT_RESULT_CODE_CM_REJECTED_DUE_TO_LIMITED_RESOURCES, CSR_BT_SUPPLIER_CM);
        CsrBtCmServiceManagerLocalQueueHandler(cmData);
    }
    else
    {
        theIndex = returnEmptyBnepIdIndex(cmData);

        if(theIndex != CM_ERROR)
        {
            aclTable *aclElement;
            CsrUint8       numOfConnection;
            bnepTable *bnepConnection;

            cmData->bnepVar.indexPtr                = &(cmData->bnepVar.connectVar[theIndex]);
            bnepConnection                         = cmData->bnepVar.indexPtr;
            cmData->bnepVar.connectTypeInProgress   = CONNECT;
            cmData->bnepVar.cancelConnect           = FALSE;
            CSR_BT_CM_STATE_CHANGE(cmData->bnepVar.connectState, CM_BNEP_IDLE);

            bnepConnection->deviceAddr              = CsrBtBdAddrFromEtherAddr (&cmPrim->rem_addr);
            bnepConnection->requestedMode           = CSR_BT_ACTIVE_MODE;
            bnepConnection->initConnect             = TRUE;
            bnepConnection->connectionSucceeded     = FALSE;

#if CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1
            bnepConnection->ssrAccepted             = FALSE;
#endif
            bnepConnection->lowPowerPriority        = DEFAULT_PRIORITY;

            returnAclConnectionElement(cmData, bnepConnection->deviceAddr, &aclElement);
            if( aclElement != NULL )
            {
                bnepConnection->sniffSettings       = aclElement->sniffSettings;
                bnepConnection->parkSettings        = aclElement->parkSettings;
            }
            else
            {
                bnepConnection->sniffSettings       = cmData->dmVar.defaultSniffSettings;
                bnepConnection->parkSettings        = cmData->dmVar.defaultParkSettings;
            }

            numOfConnection                         = CsrBtCmReturnNumOfConnectionsToPeerDevice(cmData, bnepConnection->deviceAddr);

            CsrBtCmDmSmClearRebondData(cmData);

            if(numOfConnection == 0)
            { /* Currently there is no connection attach to the given Bluetooth
                 address. Establish a new bnep connection */

                CsrUint16 clockOffset                    = 0;
                page_scan_mode_t pageScanMode           = 0;
                page_scan_rep_mode_t pageScanRepMode    = 0;

                if(isCacheParamsKnown(cmData, bnepConnection->deviceAddr, &clockOffset, &pageScanMode, &pageScanRepMode))
                {
                    CSR_BT_CM_STATE_CHANGE(bnepConnection->state, bnepConnectInit_s);
                    cmData->bnepVar.connectReqFlags            = cmPrim->flags;

                    /* start writing cache params. via DM queue */
                    CsrBtCmDmWriteCacheParamsReqSend(BNEP_PLAYER, bnepConnection->deviceAddr, clockOffset, pageScanMode, pageScanRepMode);
                }
                else
                {
                    CSR_BT_CM_STATE_CHANGE(bnepConnection->state, bnepConnect_s);
                    bnepConnection->actualMode      = CSR_BT_ACTIVE_MODE;
                    CSR_BT_CM_STATE_CHANGE(cmData->bnepVar.connectState, CM_BNEP_CONNECT);
                    CsrBtBnepConnectReqSend(cmPrim->flags, cmPrim->rem_addr);

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_INQUIRY_PAGE_STATE
                    /* Raise event that connection operation is started */
                    CsrBtCmPropgateEvent(cmData,
                                        CsrBtCmPropagateInquiryPageEvents,
                                        CSR_BT_CM_EVENT_MASK_SUBSCRIBE_INQUIRY_PAGE_STATE,
                                        HCI_SUCCESS,
                                        NULL,
                                        NULL);
#endif
                }
            }
            else
            { /* Currently there is at least one connection attach to the given
                 device address. Return the actual mode the link is in */
                CSR_BT_CM_STATE_CHANGE(bnepConnection->state, bnepConnectInit_s);
                cmData->bnepVar.connectReqFlags            = cmPrim->flags;
                CsrBtCmDmModeChangeReqMsgSend(BNEP_PLAYER, theIndex, CSR_BT_ACTIVE_MODE, bnepConnection->deviceAddr);
            }
        }
        else
        { /* It is not possible to make a BNEP connection.    Inform the application
             and restore the local service manager queue */
            CsrBtCmBnepConnectIndMsgSend(cmData, cmData->bnepVar.appHandle, ID_EMPTY,
                                         cmPrim->rem_addr, 0, 0, 0,
                                         CSR_BT_RESULT_CODE_CM_BNEP_CONNECTION_LIMIT_EXCEEDED,
                                         CSR_BT_SUPPLIER_CM);

            CsrBtCmServiceManagerLocalQueueHandler(cmData);
        }
    }
}

void CsrBtCmBnepCancelConnectReqHandler(cmInstanceData_t *cmData)
{
    CsrUint8                         theIndex;
    CsrBtCmCancelBnepConnectReq    *cmPrim;

    cmPrim    = (CsrBtCmCancelBnepConnectReq *) cmData->recvMsgP;
    theIndex   = returnCancelBnepConnectIndex(cmData, CsrBtBdAddrFromEtherAddr (&cmPrim->rem_addr));

    if(theIndex == CM_ERROR)
    { /* The CSR_BT_CM_BNEP_CONNECT_REQ msg is not placed in the bnep connection
         table                                                              */
        CsrUint8 dummy;
        CsrUint16     dummy2;
        if(cancelServiceManagerMsg(cmData, CSR_BT_CM_BNEP_CONNECT_REQ, 0, CsrBtBdAddrFromEtherAddr (&cmPrim->rem_addr), CSR_BT_NO_SERVER, NO_LOCAL_PSM, &dummy, &dummy2))
        { /* The CSR_BT_CM_BNEP_CONNECT_REQ msg is found and removed from the local
             SM queue                                                       */
            CsrBtCmBnepConnectIndMsgSend(cmData, cmData->bnepVar.appHandle, ID_EMPTY,
                        cmPrim->rem_addr, 0, 0, 0, CSR_BT_RESULT_CODE_CM_CANCELLED, CSR_BT_SUPPLIER_CM);
        }
        else
        { /* Nothing to cancel just ignore                                  */
            ;
        }
    }
    else
    {
        bnepTable *bnepConnection;

        bnepConnection = &(cmData->bnepVar.connectVar[theIndex]);
        switch (bnepConnection->state)
        {
            case bnepConnectCfm_s:
            {
                if (!bnepConnection->connectionSucceeded)
                {
                    CsrBtCmDmCancelPageOrDetach(cmData, CsrBtBdAddrFromEtherAddr(&cmPrim->rem_addr));
                    CsrBtCmScRejectedForSecurityReasonMsgSend(cmData,
                                                              CsrBtBdAddrFromEtherAddr(&cmPrim->rem_addr),
                                                              TRUE);
                    cmData->bnepVar.cancelConnect = TRUE;
                }
                break;
            }
            case bnepConnectInit_s:
            case bnepConnect_s:
            {
                cmData->bnepVar.cancelConnect = TRUE;
                CsrBtCmScRejectedForSecurityReasonMsgSend(cmData,
                                                          CsrBtBdAddrFromEtherAddr(&cmPrim->rem_addr),
                                                          TRUE);
                break;
            }
            default:
            {
                break;
            }
        }
        if (cmData->bnepVar.connectState == CM_BNEP_SSP_REPAIR)
        {
            ETHER_ADDR    remAddr;

            CsrBtCmScRejectedForSecurityReasonMsgSend(cmData,
                                                      CsrBtBdAddrFromEtherAddr(&cmPrim->rem_addr),
                                                      TRUE);
            CsrBtCmSmCancelSppRepairInd(cmData);
            remAddr = CsrBtBdAddrToEtherAddr(&(bnepConnection->deviceAddr));
            CsrBtCmBnepConnectErrorIndHandler(cmData, bnepConnection->deviceAddr, bnepConnection,
                            ID_EMPTY, 0, 0, remAddr, CSR_BT_RESULT_CODE_CM_CANCELLED, CSR_BT_SUPPLIER_CM);

        }
    }
}

/*************************************************************************************
  CsrBtCmBnepConnectAcceptReqInitHandler
************************************************************************************/
void CsrBtCmBnepConnectAcceptReqInitHandler(cmInstanceData_t *cmData, CsrUint8 theIndex, CsrUint24 classOfDevice)
{
        cmData->bnepVar.indexPtr                                = &(cmData->bnepVar.connectVar[theIndex]);
        CSR_BT_CM_STATE_CHANGE(cmData->bnepVar.connectVar[theIndex].state, bnepConnectAccept_s);
        cmData->bnepVar.connectVar[theIndex].id                 = ID_RESERVED;
        cmData->bnepVar.connectVar[theIndex].initConnect        = FALSE;
        cmData->bnepVar.connectVar[theIndex].sniffSettings      = cmData->dmVar.defaultSniffSettings;
        cmData->bnepVar.connectVar[theIndex].parkSettings       = cmData->dmVar.defaultParkSettings;
#if CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1
        cmData->bnepVar.connectVar[theIndex].ssrAccepted        = FALSE;
#endif
        cmData->bnepVar.connectVar[theIndex].lowPowerPriority   = DEFAULT_PRIORITY;
        cmData->bnepVar.connectTypeInProgress                   = CONNECT_ACCEPT;
        cmData->bnepVar.classOfDevice                           = classOfDevice;
}

/*************************************************************************************
  CsrBtCmBnepConnectAcceptReqHandler
************************************************************************************/
void CsrBtCmBnepConnectAcceptReqHandler(cmInstanceData_t *cmData)
{ /* The application request to accept a new l2cap connection, create from
     a peer device */
    CsrUint8                            theIndex;
    CsrBtCmBnepConnectAcceptReq    * cmPrim;

    cmPrim      = (CsrBtCmBnepConnectAcceptReq *) cmData->recvMsgP;
    theIndex    = returnEmptyBnepIdIndex(cmData);

    if(theIndex != CM_ERROR)
    { /* If it is possible to create a new bnep connection */
        CsrBtCmBnepConnectAcceptReqInitHandler(cmData, theIndex, cmPrim->classOfDevice);
        CsrBtBnepConnectReqSend(cmPrim->flags, cmPrim->rem_addr);
    }
    else
    { /* It is not possible to accept a BNEP connection. Inform the application
         and restore the local Bnep queue */
        CsrBtCmBnepConnectAcceptCfmMsgSend(cmData,cmData->bnepVar.appHandle,
                CSR_BT_RESULT_CODE_CM_REJECTED_DUE_TO_LIMITED_RESOURCES, CSR_BT_SUPPLIER_CM);
    }
}

/*************************************************************************************
  CsrBtCmBnepCancelConnectAcceptReqHandler
************************************************************************************/
void CsrBtCmBnepCancelConnectAcceptReqHandler(cmInstanceData_t *cmData)
{ /* The application request to cancel the previous connect accept request
     signal */
    CsrUint8                                theIndex;

    theIndex    = returnReserveBnepIdIndex(cmData);

    if(theIndex != CM_ERROR)
    { /* Remove COD and cancel the service */
        BNEP_CONNECT_REQ_FLAGS    flags;
        ETHER_ADDR                remoteEtherAddr;

        CsrMemSet(&flags, 0, sizeof(flags));

        cmData->bnepVar.indexPtr                    = &(cmData->bnepVar.connectVar[theIndex]);
        CSR_BT_CM_STATE_CHANGE(cmData->bnepVar.connectVar[theIndex].state, bnepCancelConnectAccept_s);
        cmData->bnepVar.connectTypeInProgress       = CONNECT_ACCEPT;
        flags.accept_in          = FALSE;
        flags.lgn                = FALSE;
        flags.lnap               = FALSE;
        flags.lpanu              = TRUE;
        flags.rgn                = FALSE;
        flags.rnap               = FALSE;
        flags.rpanu              = TRUE;
        flags.single_user        = TRUE;
        remoteEtherAddr.w[0]     = ETHER_UNKNOWN;
        CsrBtBnepConnectReqSend(flags, remoteEtherAddr);
    }
    else
    { /* The service can't be cancel. Send CM_BNEP_CANCEL_ACCEPT_CONNECT_CFM
         to the application and restore the local service manager queue. */
        CsrBtCmBnepCancelConnectAcceptCfmMsgSend(cmData->bnepVar.appHandle,
                            CSR_BT_RESULT_CODE_CM_NOTHING_TO_CANCEL, CSR_BT_SUPPLIER_CM);
        CsrBtCmServiceManagerLocalQueueHandler(cmData);
    }
}

void CsrBtCmBnepConnectCfmHandler(cmInstanceData_t *cmData)
{ /* Confirmation to earlier BNEP connection request passive or active */
    BNEP_CONNECT_CFM_T    * prim;
    bnepTable *bnepConnection;

    prim = (BNEP_CONNECT_CFM_T *) cmData->recvMsgP;
    bnepConnection = cmData->bnepVar.indexPtr;

    if (prim->status == BNEP_CONNECT_CFM_STATUS_SUCCESS)
    {
        switch (bnepConnection->state)
        {
            case bnepConnectAccept_s :
                {
                    if (cmData->bnepVar.connectTypeInProgress == CONNECT_ACCEPT)
                    { /* CSR_BT_CM_BNEP_CONNECT_ACCEPT_REQ in progress */
                        /* This function starts the connectable routine */
                        CsrBtCmWriteClassOfDevice(BNEP_PLAYER);
                    }
                    break;
                }

            case bnepCancelConnectAccept_s :
                { /* CSR_BT_CM_BNEP_CANCEL_CONNECT_ACCEPT_REQ in progress */
                    cmData->bnepVar.classOfDevice                        = 0x000000;
                    CsrBtCmWriteClassOfDevice(BNEP_PLAYER);
                    break;
                }
            default :
                {
                    break;
                }
        }
        if (cmData->bnepVar.connectTypeInProgress == CONNECT)
        { /* CSR_BT_CM_BNEP_CONNECT_REQ in progress */
            CSR_BT_CM_STATE_CHANGE(bnepConnection->state, bnepConnectCfm_s);
            if (cmData->bnepVar.cancelConnect)
            {
                CsrBtCmDmCancelPageOrDetach(cmData, bnepConnection->deviceAddr);
            }
        }
    }
    else
    {
        /* The attempt to establish BNEP connection fail */
        if (cmData->bnepVar.connectTypeInProgress == CONNECT)
        { /* CSR_BT_CM_BNEP_CONNECT_REQ in progress */
            CsrUint8        numOfConnection;
            ETHER_ADDR    theRemAddr;

            numOfConnection    = CsrBtCmReturnNumOfConnectionsToPeerDevice(cmData, cmData->bnepVar.indexPtr->deviceAddr);
            if(numOfConnection == 0)
            { /* There is no other connection attach to the device address.
                 Inform the application, restore the local service manager
                 queue, and clear up the bnep connection table */
                CsrBtCmBnepClearBnepTableIndex(cmData->bnepVar.indexPtr);
                CsrBtCmServiceManagerLocalQueueHandler(cmData);
            }
            else
            {    /* Currently there is at least one more connection attach to the
                    device address. Ensure that SNIFF and park is supported */
                cmData->smVar.resultCode        = (CsrBtResultCode) prim->status;
                cmData->smVar.resultSupplier    = CSR_BT_SUPPLIER_BNEP_CONNECT_CFM;
                CsrBtCmDmWriteLpSettingsReqMsgSend(cmData->bnepVar.indexPtr->deviceAddr, BNEP_PLAYER);
            }
            theRemAddr = CsrBtBdAddrToEtherAddr(&cmData->bnepVar.indexPtr->deviceAddr);
            CsrBtCmBnepConnectIndMsgSend(cmData, cmData->bnepVar.appHandle, ID_EMPTY, theRemAddr, 0, 0, 0,
                                            (CsrBtResultCode) prim->status, CSR_BT_SUPPLIER_BNEP_CONNECT_CFM);
            CsrBtCmDmUpdateAndClearCachedParamReqSend(bnepConnection->deviceAddr);
        }
        else
        { /* CSR_BT_CM_BNEP_CONNECT_ACCEPT_REQ in progress */
            switch (bnepConnection->state)
            {
                case bnepConnectAccept_s :
                    { /* CSR_BT_CM_BNEP_CONNECT_ACCEPT_REQ in progress and failed, find reserved index and delete it from table */
                        CsrBtCmBnepClearBnepTableIndex(cmData->bnepVar.indexPtr);
                        break;
                    }
                default :
                    {
                        break;
                    }
            }
            CsrBtCmBnepConnectAcceptCfmMsgSend(cmData, cmData->bnepVar.appHandle,
                                (CsrBtResultCode) prim->status, CSR_BT_SUPPLIER_BNEP_CONNECT_CFM);
        }
    }
}

void CsrBtCmBnepConnectErrorIndHandler(cmInstanceData_t *cmData, CsrBtDeviceAddr deviceAddr,
                                  bnepTable *bnepConnection, CsrUint16 theId, CsrUint16 rem_uuid, CsrUint16 loc_uuid,
                                  ETHER_ADDR rem_addr, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    CsrUint8        numOfConnection;

    CSR_BT_CM_STATE_CHANGE(cmData->bnepVar.connectState, CM_BNEP_IDLE);

    numOfConnection = CsrBtCmReturnNumOfConnectionsToPeerDevice(cmData, deviceAddr);

    if(numOfConnection == 0)
    { /* Currently there is no connection attach to the given Bluetooth
         address. Clean up the bnep table and restore the local service manager queue */
        CsrBtCmBnepClearBnepTableIndex(bnepConnection);
        CsrBtCmServiceManagerLocalQueueHandler(cmData);
    }
    else
    { /* Currently there is at least one connection attach to the given
         device address. Return the actual mode the link is in */
        cmData->smVar.resultCode        = resultCode;
        cmData->smVar.resultSupplier    = resultSupplier;
        CsrBtCmDmWriteLpSettingsReqMsgSend(bnepConnection->deviceAddr, BNEP_PLAYER);
    }
    CsrBtCmBnepConnectIndMsgSend(cmData, cmData->bnepVar.appHandle, theId, rem_addr,
                                    rem_uuid, loc_uuid, CM_BNEP_MTU, resultCode, resultSupplier);

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_INQUIRY_PAGE_STATE
    /* Raise event that connection operation is ended */
    CsrBtCmPropgateEvent(cmData,
                        CsrBtCmPropagateInquiryPageEvents,
                        CSR_BT_CM_EVENT_MASK_SUBSCRIBE_INQUIRY_PAGE_STATE,
                        HCI_SUCCESS,
                        NULL,
                        NULL);
#endif
}


/*************************************************************************************
  CsrBtCmBnepConnectIndHandler
************************************************************************************/
void CsrBtCmBnepConnectIndHandler(cmInstanceData_t *cmData)
{/* Incoming connect from the remote device */
    CsrUint8                    theIndex;
    CsrBtDeviceAddr            deviceAddr;
    BNEP_CONNECT_IND_T        * prim;

    prim        = (BNEP_CONNECT_IND_T *) cmData->recvMsgP;
    deviceAddr    = CsrBtBdAddrFromEtherAddr (&prim->rem_addr);
    theIndex    = returnIndexToThisBdAddr(cmData, deviceAddr);
    if (theIndex != CM_ERROR)
    { /* CSR_BT_CM_BNEP_CONNECT_REQ in progress */
        bnepTable *bnepConnection;

        bnepConnection = cmData->bnepVar.indexPtr;
        CsrBtCmDmUpdateAndClearCachedParamReqSend(bnepConnection->deviceAddr);
        if (prim->status == BNEP_CONNECT_IND_STATUS_SUCCESSFUL_OUT)
        {
            bnepConnection->id                  = prim->id;
            bnepConnection->logicalChannelTypeMask = CSR_BT_ACTIVE_DATA_CHANNEL;

            CSR_BT_CM_STATE_CHANGE(cmData->bnepVar.connectState, CM_BNEP_IDLE);

            CsrBtCmDmSmClearRebondData(cmData);

            if (!cmData->bnepVar.cancelConnect)
            {
                /* The Bnep connection is up running. Ensure that SNIFF
                  and park is supported                                        */
                cmData->smVar.resultCode        = CSR_BT_RESULT_CODE_CM_SUCCESS;
                cmData->smVar.resultSupplier    = CSR_BT_SUPPLIER_CM;
                cmData->bnepVar.moreLpSettingsInProgress    = TRUE;
                CsrBtCmDmWriteLpSettingsReqMsgSend(deviceAddr, BNEP_PLAYER);
                CsrBtCmBnepConnectIndMsgSend(cmData, cmData->bnepVar.appHandle, prim->id, prim->rem_addr,
                                        prim->rem_uuid, prim->loc_uuid, CM_BNEP_MTU, cmData->smVar.resultCode,
                                        cmData->smVar.resultSupplier);
                bnepConnection->connectionSucceeded = TRUE;
            }
            else
            {
                CsrBtBnepDisconnectReqSend(0, prim->id);
            }
        }
        else
        {
            if (prim->status == BNEP_CONNECT_IND_STATUS_REJ_SECURITY &&
                !cmData->bnepVar.cancelConnect                       &&
                CsrBtCmDmSmRebondNeeded(cmData))
            {
                CSR_BT_CM_STATE_CHANGE(cmData->bnepVar.connectState, CM_BNEP_SSP_REPAIR);
                CsrBtCmSmSppRepairIndSend(cmData, deviceAddr);
            }
            else
            {
                if (cmData->bnepVar.cancelConnect)
                {
                    CsrBtCmBnepConnectErrorIndHandler(cmData, deviceAddr, bnepConnection,
                                    prim->id, prim->rem_uuid, prim->loc_uuid, prim->rem_addr,
                                    CSR_BT_RESULT_CODE_CM_CANCELLED, CSR_BT_SUPPLIER_CM);
                }
                else
                {
                    CsrBtCmBnepConnectErrorIndHandler(cmData, deviceAddr, bnepConnection,
                            prim->id, prim->rem_uuid, prim->loc_uuid, prim->rem_addr,
                            (CsrBtResultCode) prim->status, CSR_BT_SUPPLIER_BNEP_CONNECT_CFM);
                }
            }
        }

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_INQUIRY_PAGE_STATE
        /* Raise event that connection operation is started */
        CsrBtCmPropgateEvent(cmData,
                            CsrBtCmPropagateInquiryPageEvents,
                            CSR_BT_CM_EVENT_MASK_SUBSCRIBE_INQUIRY_PAGE_STATE,
                            HCI_SUCCESS,
                            NULL,
                            NULL);
#endif
    }
    else
    { /* CSR_BT_CM_BNEP_CONNECT_ACCEPT_REQ in progress */
        theIndex    = returnReserveBnepIdIndex(cmData);
        if (theIndex != CM_ERROR)
        {
            if (prim->status == BNEP_CONNECT_IND_STATUS_SUCCESSFUL_IN)
            { /* A new BNEP connection have just been establish with with success.
                 Write lp settings and inform the application */
                CsrUint8        numOfConnection;

                cmData->bnepVar.moreLpSettingsInProgress    = TRUE;
                cmData->bnepVar.connectVar[theIndex].id            = prim->id;
                cmData->bnepVar.connectVar[theIndex].deviceAddr    = deviceAddr;
                cmData->bnepVar.connectVar[theIndex].logicalChannelTypeMask = CSR_BT_ACTIVE_DATA_CHANNEL;

                numOfConnection        = CsrBtCmReturnNumOfConnectionsToPeerDevice(cmData, cmData->bnepVar.connectVar[theIndex].deviceAddr);
                if(numOfConnection == 0)
                { /* There is no other connection attach to the device address.
                     Ensure that SNIFF and PARK mode is supported */
                    cmData->smVar.smInProgress                        = TRUE;
                    CSR_BT_CM_STATE_CHANGE(cmData->bnepVar.connectVar[theIndex].state, bnepConnectAcceptFinal_s);
                    CsrBtCmDmWriteLpSettingsReqMsgSend(deviceAddr, BNEP_PLAYER);
                }
                else
                { /* Currently there is at least one more connection attach to the
                     device address. The l2cap connection has been accepted with with success.
                     Inform the application, and restore the local service manager and DM queue */

                    CSR_BT_CM_STATE_CHANGE(cmData->bnepVar.connectVar[theIndex].state, bnepConnected_s);

                    cmData->bnepVar.connectVar[theIndex].actualMode    = CsrBtCmDmReturnActualMode(cmData, cmData->bnepVar.connectVar[theIndex].deviceAddr);
                    if(cmData->bnepVar.connectVar[theIndex].actualMode != CSR_BT_ACTIVE_MODE)
                    {
                        cmData->smVar.smInProgress                    = TRUE;
                        CsrBtCmDmModeChangeReqMsgSend(BNEP_PLAYER, theIndex, CSR_BT_ACTIVE_MODE, cmData->bnepVar.connectVar[theIndex].deviceAddr);
                    }
                }

                CsrBtCmBnepConnectIndMsgSend(cmData, cmData->bnepVar.appHandle, prim->id, prim->rem_addr,
                                             prim->rem_uuid, prim->loc_uuid, CM_BNEP_MTU,
                                             CSR_BT_RESULT_CODE_CM_SUCCESS, CSR_BT_SUPPLIER_CM);
            }
        }
        else
        { /* can't accept connection from the remote device, send error to the BSL */
            CsrBtCmBnepConnectIndMsgSend(cmData, cmData->bnepVar.appHandle, prim->id, prim->rem_addr,
                                            prim->rem_uuid, prim->loc_uuid, 0,
                                            CSR_BT_RESULT_CODE_CM_BNEP_CONNECTION_LIMIT_EXCEEDED,
                                            CSR_BT_SUPPLIER_CM);
        }
    }
}
#endif

