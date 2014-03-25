/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_RFC_MODULE

#include "csr_bt_cm_main.h"
#include "csr_bt_cm_rfc.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_cm_dm.h"
#include "csr_bt_cm_sdc.h"
#include "csr_bt_cm_util.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_cm_events_handler.h"
#include "csr_bt_cm_l2cap.h"

void CsrBtCmRfcCommonErrorHandler(cmInstanceData_t *cmData, cmRfcConnInstType * theLogicalLink)
{
    CsrUint8 numOfConnection = CsrBtCmReturnNumOfConnectionsToPeerDevice(cmData, theLogicalLink->deviceAddr);

    if(numOfConnection == 0)
    { /* There is no other connection attach to the device address.
         Inform the application. */
        CsrBtCmConnectCfmMsgSend(cmData, cmData->smVar.resultCode, cmData->smVar.resultSupplier);
    }
    else
    { /* Currently there is at least one more connection attach to the
         device address. Ensure that SNIFF and park is supported */
        CsrBtCmDmWriteLpSettingsReqMsgSend(theLogicalLink->deviceAddr, RFC_PLAYER);
    }
}

static cmRfcConnElement * returnCancelConnectIndex(cmInstanceData_t *cmData, CsrUint8 server, CsrSchedQid phandle, CsrBtDeviceAddr deviceAddr, CsrBtCmPrim type)
{
    if (cmData->smVar.smInProgress && cmData->smVar.smMsgTypeInProgress == type)
    {
        cmRfcConnElement *currentElem;

        for (currentElem = CM_RFC_GET_FIRST(cmData->rfcVar.connList); currentElem; currentElem = currentElem->next)
        {
            if (currentElem->cmRfcConnInst)
            {
                if (server == currentElem->cmRfcConnInst->serverChannel)
                {
                    if (phandle == currentElem->cmRfcConnInst->appHandle)
                    {
                        if(bd_addr_eq(&(deviceAddr), &(currentElem->cmRfcConnInst->deviceAddr)))
                        {
                            if (currentElem->cmRfcConnInst->state == connect_s ||
                                currentElem->cmRfcConnInst->state == connectInit_s || 
                                currentElem->cmRfcConnInst->state == access_s)
                            {
                                return currentElem;
                            }
                        }
                    }
                }
            }
        }
    }
    return NULL;
}

void CsrBtCmRfcStartInitiateConnection(cmInstanceData_t *cmData, cmRfcConnInstType * theLogicalLink)
{
    RfcClientConnectReqSend(CSR_BT_CM_IFACEQUEUE,
                            &theLogicalLink->deviceAddr,
                            theLogicalLink->remoteServerChan,
                            CSR_BT_RFCOMM_PREFERRED_MODE, /* from usr_config */
                            theLogicalLink->context,theLogicalLink->serverChannel,
                            theLogicalLink->profileMaxFrameSize,
                            0, /* priority */
                            CSR_BT_CM_INIT_CREDIT,
                            L2CA_AMP_CONTROLLER_BT,
                            L2CA_AMP_CONTROLLER_BT,
                            0, /* reserved length */
                            NULL, /* reserved */
                            theLogicalLink->modemStatus, 
                            theLogicalLink->signalBreak, 
                            theLogicalLink->mscTimeout); 
}

static CsrBool getSdcRemoteServerCh(cmInstanceData_t * cmData, CsrSchedQid appHandle, CsrBtUuid32 serviceHandle,
                                    CsrBtDeviceAddr deviceAddr, CsrUint8 * server)
{
    if (cmData->sdcVar.sdcSearchList)
    {
        sdcSearchElement    * theElement;
        theElement     = cmData->sdcVar.sdcSearchList;
        do
        {
            if (theElement->appHandle == appHandle && bd_addr_eq(&(theElement->deviceAddr), &deviceAddr))
            {
                CsrUintFast16 i, t;

                for (i = 0; i < theElement->numOfSearchPtr; i++)
                {
                    if (theElement->searchPtrArray[i])
                    {
                        for (t = 0; t < theElement->searchPtrArray[i]->resultListSize; t++)
                        {
                            if (theElement->searchPtrArray[i]->serviceHandle[t] == serviceHandle)
                            {
                                *server = theElement->searchPtrArray[i]->serverChannelList[t];
                                return TRUE;
                            }
                        }
                    }
                }
                *server = CSR_BT_NO_SERVER;
                return FALSE;
            }
            theElement = theElement->nextSdcElement;
        } while (theElement);
    }
    *server = CSR_BT_NO_SERVER;
    return FALSE;
}

static void csrBtCmConnectCfmMsgSend(CsrSchedQid appHandle, CsrBtDeviceAddr deviceAddr, CsrBtConnId btConnId,
                                     CsrUint16 profileMaxFrameSize, CsrBool validPortPar,
                                     RFC_PORTNEG_VALUES_T portPar, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier, CsrUint16 context)
{
    CsrBtCmConnectCfm * prim;

    prim                        = (CsrBtCmConnectCfm *)CsrPmemAlloc(sizeof(CsrBtCmConnectCfm));
    prim->type                  = CSR_BT_CM_CONNECT_CFM;
    prim->btConnId              = btConnId;
    prim->validPortPar          = validPortPar;
    prim->portPar               = portPar;
    prim->profileMaxFrameSize   = profileMaxFrameSize;
    prim->deviceAddr            = deviceAddr;
    prim->resultCode            = resultCode;
    prim->resultSupplier        = resultSupplier;
    prim->context               = context;
    CsrBtCmPutMessage(appHandle, prim);
}

static void csrBtCmCommonRfcConnectHandle(cmInstanceData_t *cmData, CsrSchedQid appHandle, CsrUint8 localServer,
                                          CsrUint8 remoteServer, CsrUint16 profileMaxFrameSize, CsrBool reqPortPar,
                                          CsrBool validPortPar, RFC_PORTNEG_VALUES_T portPar, CsrBtDeviceAddr devAddr,
                                          dm_security_level_t secLevel, CsrUint16 context, CsrUint8 modemStatus,
                                          CsrUint8 breakSignal, CsrUint8 mscTimeout)
{
    if (remoteServer != CSR_BT_NO_SERVER && CsrBtCmRfcSetElementCounter(cmData))
    {
        aclTable *aclConnectionElement;
        CsrUint8 numOfConnection       = CsrBtCmReturnNumOfConnectionsToPeerDevice(cmData, devAddr);
        cmRfcConnElement  *theElement = (cmRfcConnElement *) CsrCmnListElementAddLast(&(cmData->rfcVar.connList), sizeof(cmRfcConnElement));
        cmRfcConnInstType *connInst   = theElement->cmRfcConnInst;
        theElement->elementId         = cmData->rfcVar.elementCounter;
        cmData->rfcVar.connElement    = theElement;
        cmData->rfcVar.cancelConnect  = FALSE;

        CSR_BT_CM_STATE_CHANGE(cmData->rfcVar.connectState, CM_RFC_IDLE);
        CsrBtCmDmSmClearRebondData(cmData);

        connInst->appHandle           = appHandle;
        connInst->deviceAddr          = devAddr;
        connInst->serverChannel       = localServer;
        connInst->btConnId            = CSR_BT_CONN_ID_INVALID;
        connInst->remoteServerChan    = remoteServer;
        connInst->profileMaxFrameSize = CsrBtCmRfcDetermineMtu(cmData, devAddr, profileMaxFrameSize);
        connInst->requestPortPar      = reqPortPar;
        connInst->validPortPar        = validPortPar;
        connInst->portPar             = portPar;
        connInst->context             = context;
        connInst->modemStatus         = modemStatus;
        connInst->signalBreak         = breakSignal;
        connInst->mscTimeout          = mscTimeout;
        dm_sm_register_outgoing_req(CSR_BT_CM_IFACEQUEUE,
                                    0, /* context */
                                    &devAddr,
                                    SEC_PROTOCOL_RFCOMM,
                                    localServer,
                                    secLevel,
                                    0,
                                    NULL);

        returnAclConnectionElement(cmData, devAddr, &aclConnectionElement);

        if( aclConnectionElement != NULL )
        {
            connInst->sniffSettings   = aclConnectionElement->sniffSettings;
            connInst->parkSettings    = aclConnectionElement->parkSettings;
        }
        else
        {
            connInst->sniffSettings   = cmData->dmVar.defaultSniffSettings;
            connInst->parkSettings    = cmData->dmVar.defaultParkSettings;
        }

        if(numOfConnection == 0)
        { /* Currently there is no connection attach to the given Bluetooth address. Establish a new RFCOMM connection */
            CsrUint16 clockOffset                   = 0;
            page_scan_mode_t pageScanMode           = 0;
            page_scan_rep_mode_t pageScanRepMode    = 0;
            CsrUint8 featIndex = LMP_FEATURES_SIMPLE_PAIRING_BIT/8;
            CsrUint8 featOffsetBit = LMP_FEATURES_SIMPLE_PAIRING_BIT%8;

            if(!aclConnectionElement && isCacheParamsKnown(cmData, devAddr, &clockOffset, &pageScanMode, &pageScanRepMode))
            {
                CSR_BT_CM_STATE_CHANGE(connInst->state, connectInit_s);
                /* start writing cache params. via DM queue */
                CsrBtCmDmWriteCacheParamsReqSend(RFC_PLAYER, devAddr, clockOffset, pageScanMode, pageScanRepMode);
            }
            else if ((aclConnectionElement && !CSR_BIT_IS_SET(aclConnectionElement->remoteFeatures[featIndex], featOffsetBit)) ||
                     !aclConnectionElement)
            {
                CSR_BT_CM_STATE_CHANGE(connInst->state, connect_s);
                CsrBtCmRfcStartInitiateConnection(cmData, connInst);
            }
            else /* if (aclConnectionElement) */
            {
                CSR_BT_CM_STATE_CHANGE(connInst->state, access_s);
                CsrBtCmDmSmAccessReqMsgSend();
            }
        }
        else
        {
            CSR_BT_CM_STATE_CHANGE(connInst->state, connectInit_s);
            CsrBtCmDmModeChangeReqMsgSend(RFC_PLAYER, theElement->elementId, CSR_BT_ACTIVE_MODE, devAddr);
        }

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_INQUIRY_PAGE_STATE
        /* Raise event that connection operation is started */ 
        CsrBtCmPropgateEvent(cmData,
                             CsrBtCmPropagateInquiryPageEvents,
                             CSR_BT_CM_EVENT_MASK_SUBSCRIBE_INQUIRY_PAGE_STATE,
                             HCI_SUCCESS,
                             connInst,
                             NULL);
#endif
    }
    else
    {
        RFC_PORTNEG_VALUES_T portPar;
        CsrBtPortParDefault(&portPar);

        if (remoteServer == CSR_BT_NO_SERVER)
        {
            csrBtCmConnectCfmMsgSend(appHandle, devAddr, CSR_BT_CONN_ID_INVALID, 0,
                                     FALSE, portPar, CSR_BT_RESULT_CODE_CM_COMMAND_DISALLOWED, CSR_BT_SUPPLIER_CM, context);
        }
        else
        {
            csrBtCmConnectCfmMsgSend(appHandle, devAddr, CSR_BT_CONN_ID_INVALID, 0,
                                     FALSE, portPar, CSR_BT_RESULT_CODE_CM_INTERNAL_ERROR, CSR_BT_SUPPLIER_CM, context);
        }
        CsrBtCmServiceManagerLocalQueueHandler(cmData);
    }
}

void CsrBtCmConnectCfmMsgSend(cmInstanceData_t *cmData, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{/* Send a CSR_BT_CM_CONNECT_CFM signal to the application */
    cmRfcConnInstType  * theLogicalLink = cmData->rfcVar.connElement->cmRfcConnInst;

    CsrBtCmDmUpdateAndClearCachedParamReqSend(theLogicalLink->deviceAddr);


    csrBtCmConnectCfmMsgSend(theLogicalLink->appHandle, theLogicalLink->deviceAddr,
                             theLogicalLink->btConnId,
                             theLogicalLink->profileMaxFrameSize,
                             theLogicalLink->validPortPar, theLogicalLink->portPar,
                             resultCode, resultSupplier, theLogicalLink->context);

    if(resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && resultSupplier == CSR_BT_SUPPLIER_CM)
    { /* The attempt to create a connection is a SUCCESS */
        CSR_BT_CM_STATE_CHANGE(theLogicalLink->state, connected_s);

        if(theLogicalLink->controlSignalQueue != NULL)
        { /* There is control signals on the controlSignalQueue. Restore signal and send it to the application */
            RFC_MODEM_STATUS_IND_T  *prim;

            prim = (RFC_MODEM_STATUS_IND_T *) theLogicalLink->controlSignalQueue;
            CsrBtCmControlIndMsgSend(theLogicalLink, prim->modem_signal, prim->break_signal);
            CsrPmemFree(prim);
            theLogicalLink->controlSignalQueue = NULL;
        }
        if(theLogicalLink->dataControl.receivedBuffer[theLogicalLink->dataControl.restoreCount] != NULL)
        { /* There is data in the receivebuffer. Restore signal and send the payload to the application */
            CsrBtCmRfcRestoreDataInReceiveBuffer(theLogicalLink);
        }

        cmDmAclKickRoleSwitch(cmData);
    }
    else
    { /* The attempt to create a connection fail. Clean up the RFC connection table */
        dm_sm_unregister_outgoing_req(CSR_BT_CM_IFACEQUEUE,
                                      0, /* context */
                                      &theLogicalLink->deviceAddr,
                                      SEC_PROTOCOL_RFCOMM,
                                      theLogicalLink->remoteServerChan,
                                      NULL);
        CsrBtCmDmSyncClearPcmSlotFromTable(cmData, &theLogicalLink->eScoParms);
        cleanUpConnectionTable(&(cmData->rfcVar.connElement->cmRfcConnInst));
    }
    cmData->rfcVar.cancelConnect = FALSE;

    CsrBtCmDmSmClearRebondData(cmData);

    CsrBtCmServiceManagerLocalQueueHandler(cmData);

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_INQUIRY_PAGE_STATE
    /* Raise event that connection operation is stopped */
    CsrBtCmPropgateEvent(cmData,
                         CsrBtCmPropagateInquiryPageEvents,
                         CSR_BT_CM_EVENT_MASK_SUBSCRIBE_INQUIRY_PAGE_STATE,
                         HCI_SUCCESS,
                         NULL,
                         NULL);
#endif
}

#ifdef CSR_BT_INSTALL_CM_PRI_CONNECT_EXT
void CsrBtCmRfcConnectReqExtHandler(cmInstanceData_t *cmData)
{ /* This event is used then the application request to create a new connection. */

    CsrBtCmConnectExtReq    * cmPrim;

    cmPrim                        = (CsrBtCmConnectExtReq *) cmData->recvMsgP;

    csrBtCmCommonRfcConnectHandle(cmData, cmPrim->appHandle, cmPrim->localServerCh, cmPrim->remoteServerCh,
                                  cmPrim->profileMaxFrameSize, cmPrim->requestPortPar, cmPrim->validPortPar,
                                  cmPrim->portPar, cmPrim->deviceAddr, cmPrim->secLevel, CSR_BT_CM_CONTEXT_UNUSED,
                                  cmPrim->modemStatus, cmPrim->breakSignal, cmPrim->mscTimeout);
}
#endif

void CsrBtCmRfcConnectReqHandler(cmInstanceData_t *cmData)
{ /* This event is used then the application request to create a new connection. */
    CsrUint8        remoteServerCh;
    CsrBtCmConnectReq    * cmPrim;

    cmPrim    = (CsrBtCmConnectReq *) cmData->recvMsgP;

    getSdcRemoteServerCh(cmData, cmPrim->appHandle, cmPrim->serviceHandle,
                         cmPrim->deviceAddr, &remoteServerCh);

    csrBtCmCommonRfcConnectHandle(cmData, cmPrim->appHandle, cmPrim->localServerCh, remoteServerCh,
                                  cmPrim->profileMaxFrameSize, cmPrim->requestPortPar, cmPrim->validPortPar,
                                  cmPrim->portPar, cmPrim->deviceAddr, cmPrim->secLevel, cmPrim->context,
                                  cmPrim->modemStatus, cmPrim->breakSignal, cmPrim->mscTimeout);
}

static CsrBool CsrBtCmReconnect(cmInstanceData_t *cmData, CsrUint16 result, CsrBtDeviceAddr deviceAddr, CsrBool hciErrorType)
{
    CsrBool knownError = FALSE;

    if (hciErrorType)
    {
        hci_return_t errorCode = (hci_return_t)(result & 0x00FF);

        if (errorCode == HCI_ERROR_KEY_MISSING || errorCode == HCI_ERROR_AUTH_FAIL || errorCode == HCI_ERROR_PAIRING_NOT_ALLOWED)
        {
            knownError = TRUE;
        }
    }
    else
    {/* NOTE: l2cap error type occationally received in RFC connect confirm message */
        l2ca_conn_result_t errorCode = (l2ca_conn_result_t)result;
        RFC_RESPONSE_T rfcErrorCode = (RFC_RESPONSE_T)result;
        if (errorCode == L2CA_CONNECT_REJ_SECURITY || errorCode == L2CA_CONNECT_KEY_MISSING
            || rfcErrorCode == RFC_CONNECTION_REJ_SECURITY)
        {
            knownError = TRUE;
        }
    }
    
    if (knownError)
    {
        if (!cmData->rfcVar.cancelConnect && CsrBtCmDmSmRebondNeeded(cmData))
        {
            return TRUE;
        }
        else
        {
            CsrBtCmScRejectedForSecurityReasonMsgSend(cmData,
                                                      deviceAddr, FALSE);
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }
}

void CsrBtCmDmSmAccessReqMsgSend(void)
{
    CsrBtCmSmAccessReq * prim;

    prim       = (CsrBtCmSmAccessReq *)CsrPmemAlloc(sizeof(CsrBtCmSmAccessReq));
    prim->type = CSR_BT_CM_SM_ACCESS_REQ;

    CsrBtCmPutMessage(CSR_BT_CM_IFACEQUEUE, prim);
}

void CsrBtCmSmAccessReqHandler(cmInstanceData_t *cmData)
{
    cmRfcConnInstType *theLogicalLink;

    theLogicalLink = cmData->rfcVar.connElement->cmRfcConnInst;

    dm_sm_access_req(CSR_BT_CM_IFACEQUEUE,
                     &theLogicalLink->deviceAddr,
                     SEC_PROTOCOL_RFCOMM,
                     theLogicalLink->remoteServerChan,
                     FALSE,
                     CSR_BT_CM_CONTEXT_UNUSED, /* unused: context */
                     NULL);

    /* Restore queue immediately since this can trigger authentication */
    CsrBtCmDmLocalQueueHandler();
}

void CsrBtCmDmSmAccessCfmHandler(cmInstanceData_t *cmData)
{
    DM_SM_ACCESS_CFM_T *dmPrim;
    cmRfcConnInstType *theLogicalLink;
    dmPrim = (DM_SM_ACCESS_CFM_T *) cmData->recvMsgP;

    /* In some cases, we may not even have a connection */
    if(cmData->rfcVar.connElement == NULL)
    {
        /* FIXME - send error here? */
        return;
    }

    theLogicalLink = cmData->rfcVar.connElement->cmRfcConnInst;
    switch(dmPrim->status)
    {
        case HCI_SUCCESS:
        {
            if (!cmData->rfcVar.cancelConnect)
            {
                CSR_BT_CM_STATE_CHANGE(theLogicalLink->state, connect_s);
                CsrBtCmRfcStartInitiateConnection(cmData, theLogicalLink);
            }
            else
            {
                cmData->smVar.resultCode     = CSR_BT_RESULT_CODE_CM_CANCELLED;
                cmData->smVar.resultSupplier = CSR_BT_SUPPLIER_CM;
                CsrBtCmRfcCommonErrorHandler(cmData, theLogicalLink);
            }
            break;
        }
        
        case HCI_ERROR_NO_CONNECTION:
        { 
            if (!cmData->rfcVar.cancelConnect)
            {
                if (theLogicalLink->state == access_s)
                {
                    CSR_BT_CM_STATE_CHANGE(theLogicalLink->state, connect_s);
                    CsrBtCmRfcStartInitiateConnection(cmData, theLogicalLink);
                }
                else
                {
                    cmData->smVar.resultCode     = (CsrBtResultCode) dmPrim->status;
                    cmData->smVar.resultSupplier = CSR_BT_SUPPLIER_HCI;
                    CsrBtCmRfcCommonErrorHandler(cmData, theLogicalLink);
                }
            }
            else
            {
                cmData->smVar.resultCode     = CSR_BT_RESULT_CODE_CM_CANCELLED;
                cmData->smVar.resultSupplier = CSR_BT_SUPPLIER_CM;
                CsrBtCmRfcCommonErrorHandler(cmData, theLogicalLink);
            }
            break;
        }

        default:
        {
            if (CsrBtCmReconnect(cmData, (CsrUint16)dmPrim->status, theLogicalLink->deviceAddr, TRUE))
            { /* The connect attempt fail because of security reason, e.g the local
                 device had a SSP link key and the remote device did not */
                CsrBtCmSmSppRepairIndSend(cmData, theLogicalLink->deviceAddr);
                CSR_BT_CM_STATE_CHANGE(cmData->rfcVar.connectState, CM_RFC_SSP_REPAIR);
            }
            else
            {
                cmData->smVar.resultCode     = (CsrBtResultCode) dmPrim->status;
                cmData->smVar.resultSupplier = CSR_BT_SUPPLIER_HCI;
                CsrBtCmRfcCommonErrorHandler(cmData, theLogicalLink);
            }
            break;
        }
    }
}

void CsrBtCmCancelConnectReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmCancelConnectReq *prim = (CsrBtCmCancelConnectReq *) cmData->recvMsgP;
    cmRfcConnElement *theElement  = returnCancelConnectIndex(cmData, prim->localServerCh,
                                                             prim->appHandle, prim->deviceAddr, prim->typeToCancel);

    if(theElement == NULL)
    { /* The Connect Request Msg is not placed in the connection table */
        CsrUint8 dummy;
        CsrUint16     context;
        if(cancelServiceManagerMsg(cmData, prim->typeToCancel, prim->appHandle, prim->deviceAddr, prim->localServerCh, NO_LOCAL_PSM, &dummy, &context))
        { /* The Connect request msg is found and removed from the local
             SM queue */
            RFC_PORTNEG_VALUES_T      portPar;
            CsrBtPortParDefault(&portPar);
            csrBtCmConnectCfmMsgSend(prim->appHandle, prim->deviceAddr, (CsrUint32)(prim->localServerCh), 0, FALSE, portPar,
                                     CSR_BT_RESULT_CODE_CM_CANCELLED, CSR_BT_SUPPLIER_CM, context);
        }
        else
        { /* Nothing to cancel just ignore */
            ;
        }
    }
    else
    {
        cmRfcConnInstType *theLogicalLink   = theElement->cmRfcConnInst;
        cmData->rfcVar.cancelConnect        = TRUE;
        CsrBtCmScRejectedForSecurityReasonMsgSend(cmData,
                                                  prim->deviceAddr, TRUE);

        switch (cmData->rfcVar.connectState)
        {
            case CM_RFC_IDLE:
            { /* The CM is about to or is setting an outgoing RFCOMM up             */
                if (CsrBtCmDmCancelPageOrDetach(cmData, prim->deviceAddr))
                { /* The ACL is being detach change state to ensure that
                     it not cancel twice                                            */
                    CSR_BT_CM_STATE_CHANGE(cmData->rfcVar.connectState, CM_RFC_CANCELING);
                }
                else if (theLogicalLink->pendingBtConnId != CSR_BT_CONN_ID_INVALID)
                { /* Request RFCOMM to cancel the outgoing connection               */
                    CSR_BT_CM_STATE_CHANGE(cmData->rfcVar.connectState, CM_RFC_CANCELING);
                    RfcDisconnectReqSend((CsrUint16)theLogicalLink->pendingBtConnId);
                    theLogicalLink->btConnId = theLogicalLink->pendingBtConnId;
                }
                else
                { /* Must Wait until CM receives the right Confirm Msg              */
                    ;
                }
                break;
            }
            case CM_RFC_SSP_REPAIR:
            {
                cmData->smVar.resultCode     = CSR_BT_RESULT_CODE_CM_CANCELLED;
                cmData->smVar.resultSupplier = CSR_BT_SUPPLIER_CM;
                CsrBtCmSmCancelSppRepairInd(cmData);
                CSR_BT_CM_STATE_CHANGE(cmData->rfcVar.connectState, CM_RFC_CANCELING);
                CsrBtCmRfcCommonErrorHandler(cmData, theLogicalLink);
                break;
            }
            default:
            { /* Must Wait until CM receives the right Confirm Msg */
                break;
            }
        }
    }
}

void CsrBtCmRfcClientConnectCfmHandler(cmInstanceData_t *cmData)
{
    RFC_CLIENT_CONNECT_CFM_T *rfcPrim = (RFC_CLIENT_CONNECT_CFM_T *) cmData->recvMsgP;
    /* We do not have a connection Id stored yet, as it is known first now; therefore seach for the server channel
       and the "no-connection-id" field */
    cmConnIdServerType  connIdServ       = CsrBtCmReturnConnIdServerStruct(CSR_BT_CONN_ID_INVALID, rfcPrim->serv_chan);
    cmRfcConnElement * theElement = CM_FIND_RFC_ELEMENT(CsrBtCmRfcFindRfcConnElementFromConnIdRemoteServer, &(connIdServ));

    if ((theElement == NULL) && cmData->rfcVar.cancelConnect)
    {/* This may be because we have issued a disconnect_req already.... */
        CsrBtConnId val = CM_CREATE_RFC_CONN_ID(rfcPrim->conn_id);
        connIdServ      = CsrBtCmReturnConnIdServerStruct(val, rfcPrim->serv_chan);
        theElement      = CM_FIND_RFC_ELEMENT(CsrBtCmRfcFindRfcConnElementFromConnIdRemoteServer, &(connIdServ));
    }

    if(theElement)
    {
        cmRfcConnInstType *theLogicalLink   = theElement->cmRfcConnInst;

        if (RFC_SUCCESS == rfcPrim->status)
        {/* Connection succeeded */
            aclTable *aclConnectionElement  = NULL;

            theLogicalLink->btConnId    = CM_CREATE_RFC_CONN_ID(rfcPrim->conn_id);
            theLogicalLink->pendingBtConnId = CSR_BT_CONN_ID_INVALID;
            theLogicalLink->context     = rfcPrim->context;
            theLogicalLink->profileMaxFrameSize    = rfcPrim->max_payload_size;
            returnAclConnectionElement(cmData, rfcPrim->bd_addr, &aclConnectionElement);
            if (aclConnectionElement)
            { /* An ACL connection is present */
                aclConnectionElement->maxRfcLinkFrameSize   = theLogicalLink->maxLinkFrameSize;
            }
            if (!cmData->rfcVar.cancelConnect)
            {
                cmData->smVar.resultCode     = CSR_BT_RESULT_CODE_CM_SUCCESS;
                cmData->smVar.resultSupplier = CSR_BT_SUPPLIER_CM;
                CSR_BT_CM_STATE_CHANGE(cmData->rfcVar.connectState, CM_RFC_CONNECTED);
                CsrBtCmDmWriteLpSettingsReqMsgSend(theLogicalLink->deviceAddr, RFC_PLAYER);
            }
            else
            {/* Crossing release and connect cfm */
                CSR_BT_CM_STATE_CHANGE(cmData->rfcVar.connectState, CM_RFC_CANCELING);
                RfcDisconnectReqSend((CsrUint16)theLogicalLink->btConnId);
            }
        }
        else if (CsrBtCmReconnect(cmData, (CsrUint16)rfcPrim->status, theLogicalLink->deviceAddr, FALSE))
        { /* The connect attempt fail because of security reason, e.g the local
             device had a SSP link key and the remote device did not */
            CsrBtCmSmSppRepairIndSend(cmData, theLogicalLink->deviceAddr);
            CSR_BT_CM_STATE_CHANGE(cmData->rfcVar.connectState, CM_RFC_SSP_REPAIR);
        }
        else
        {
            if (RFC_CONNECTION_PENDING != rfcPrim->status)
            {
                CsrBtResultCode      resultCode;
                RFC_PORTNEG_VALUES_T portPar;
                CsrBtPortParDefault(&portPar);
              
                theLogicalLink->pendingBtConnId = CSR_BT_CONN_ID_INVALID;
                if (theLogicalLink->remoteServerChan == CSR_BT_NO_SERVER)
                {
                    resultCode = CSR_BT_RESULT_CODE_CM_COMMAND_DISALLOWED;
                }
                else
                {
                    resultCode = CSR_BT_RESULT_CODE_CM_INTERNAL_ERROR;
                }
                CsrBtCmConnectCfmMsgSend(cmData, resultCode, CSR_BT_SUPPLIER_CM);
            }
            else
            {/* pending! */
                theLogicalLink->pendingBtConnId = CM_CREATE_RFC_CONN_ID(rfcPrim->conn_id);
                
                if (cmData->rfcVar.cancelConnect && 
                    cmData->rfcVar.connectState != CM_RFC_CANCELING)
                { /* Request RFCOMM to cancel the outgoing connection. E.g. 
                     CsrBtCmCancelConnectReq were received before CM receives 
                     this message*/
                    CSR_BT_CM_STATE_CHANGE(cmData->rfcVar.connectState, CM_RFC_CANCELING);
                    RfcDisconnectReqSend((CsrUint16)theLogicalLink->pendingBtConnId);
                    theLogicalLink->btConnId = theLogicalLink->pendingBtConnId;
                }
            }
        }
    }
    else
    { 
        ;
    }
}

void CsrBtCmRfcServerConnectCfmHandler(cmInstanceData_t *cmData)
{
    RFC_SERVER_CONNECT_CFM_T *rfcPrim = (RFC_SERVER_CONNECT_CFM_T *) cmData->recvMsgP;
    CsrBtConnId val = CM_CREATE_RFC_CONN_ID(rfcPrim->conn_id);
    cmConnIdServerType  connIdServ       = CsrBtCmReturnConnIdServerStruct(val, rfcPrim->serv_chan);
    cmRfcConnElement * rfcElement = CM_FIND_RFC_ELEMENT(CsrBtCmRfcFindRfcConnElementFromConnIdServer, &(connIdServ));

    if(rfcElement)
    {
        cmRfcConnInstType *rfcLogicalLink   = rfcElement->cmRfcConnInst;
        cmData->rfcVar.connElement          = rfcElement;
        rfcLogicalLink->profileMaxFrameSize = rfcPrim->max_payload_size;
        CSR_BT_CM_STATE_CHANGE(rfcLogicalLink->state, connectAcceptFinal_s);

        if (RFC_SUCCESS == rfcPrim->status)
        {/* Connection succeeded */
            cmData->smVar.resultCode        = CSR_BT_RESULT_CODE_CM_SUCCESS;
            cmData->smVar.resultSupplier    = CSR_BT_SUPPLIER_CM;
        }
        else
        {
            rfcLogicalLink->btConnId        = CSR_BT_CONN_ID_INVALID;
            cmData->smVar.resultCode        = rfcPrim->status;
            cmData->smVar.resultSupplier    = CSR_BT_SUPPLIER_RFCOMM;
        }
        CsrBtCmWriteClassOfDevice(RFC_PLAYER);
    }
    else
    { /* No owner, just ignore */
        ;
    }
}

void CsrBtCmRfcServerConnectIndHandler(cmInstanceData_t *cmData)
{
    RFC_SERVER_CONNECT_IND_T *rfcPrim = (RFC_SERVER_CONNECT_IND_T *) cmData->recvMsgP;
    
    /* We do not have a connection Id stored yet, as it is known first now; 
       therefore seach for the server channel and the "no-connection-id" field  */
    cmConnIdServerType  connIdServ = CsrBtCmReturnConnIdServerStruct(CSR_BT_CONN_ID_INVALID, rfcPrim->loc_serv_chan);
    cmRfcConnElement * rfcElement  = CM_FIND_RFC_ELEMENT(CsrBtCmRfcFindRfcConnElementFromConnIdServer, &(connIdServ));

    if (rfcElement)
    {
        cmRfcConnInstType *rfcLogicalLink   = rfcElement->cmRfcConnInst;

        if(rfcLogicalLink->timerFlag)
        {/* Reset the timer */
            CsrSchedTimerCancel(rfcLogicalLink->timerId, NULL, NULL);
            rfcLogicalLink->timerFlag = FALSE;
        }

        if(rfcPrim->local_l2cap_control != L2CA_AMP_CONTROLLER_BT)
        { /* Reject AMP connections. We support move, but not create channel */
            RfcServerConnectResSend(rfcPrim->flags,
                                    rfcPrim->conn_id,
                                    RFC_DECLINE_SERVER_CONNECTION,
                                    0,
                                    0,
                                    CSR_BT_CM_INIT_CREDIT,
                                    L2CA_AMP_CONTROLLER_BT,
                                    L2CA_AMP_CONTROLLER_BT,
                                    CSR_BT_DEFAULT_MODEM_STATUS, 
                                    CSR_BT_DEFAULT_BREAK_SIGNAL, 
                                    CSR_BT_DEFAULT_MSC_TIMEOUT); 
        }
        else
        {
            if(cmData->smVar.smInProgress)
            {/* Another service is in progress */
                CsrMessageQueuePush(&cmData->smVar.SaveQueue, RFCOMM_PRIM, cmData->recvMsgP);
                cmData->recvMsgP = NULL;
            }
            else
            {
                rfcLogicalLink->btConnId            = CM_CREATE_RFC_CONN_ID(rfcPrim->conn_id);
                rfcLogicalLink->deviceAddr          = rfcPrim->bd_addr;
                rfcLogicalLink->profileMaxFrameSize = CsrBtCmRfcDetermineMtu(cmData,rfcLogicalLink->deviceAddr,0);
                cmData->rfcVar.connElement          = rfcElement;
                CSR_BT_CM_STATE_CHANGE(rfcLogicalLink->state, connectAccept_s);

                RfcServerConnectResSend(rfcPrim->flags,
                                        rfcPrim->conn_id,
                                        RFC_ACCEPT_SERVER_CONNECTION,
                                        rfcLogicalLink->profileMaxFrameSize,
                                        0,
                                        CSR_BT_CM_INIT_CREDIT,
                                        L2CA_AMP_CONTROLLER_BT,
                                        L2CA_AMP_CONTROLLER_BT,
                                        rfcLogicalLink->modemStatus,
                                        rfcLogicalLink->signalBreak,
                                        rfcLogicalLink->mscTimeout);

                cmData->smVar.smInProgress = TRUE;
            }
        }
    }
    else
    {
        RfcServerConnectResSend(rfcPrim->flags,
                                rfcPrim->conn_id,
                                RFC_DECLINE_SERVER_CONNECTION,
                                0,
                                0,
                                CSR_BT_CM_INIT_CREDIT,
                                L2CA_AMP_CONTROLLER_BT,
                                L2CA_AMP_CONTROLLER_BT,
                                CSR_BT_DEFAULT_MODEM_STATUS,
                                CSR_BT_DEFAULT_BREAK_SIGNAL,
                                CSR_BT_DEFAULT_MSC_TIMEOUT);

        if (cmData->smVar.popFromSaveQueue)
        { /* The RFC_SERVER_CONNECT_IND message has been restore
             from the SM queue. Restore it and lock it again
             in order to make sure that no message can use
             the SM before the CSR_BT_CM_SM_HOUSE_CLEANING
             is received. */
            cmData->smVar.smInProgress = TRUE;
            CsrBtCmServiceManagerLocalQueueHandler(cmData);
        }
    }
}

#endif /* #ifndef EXCLUDE_CSR_BT_RFC_MODULE */

