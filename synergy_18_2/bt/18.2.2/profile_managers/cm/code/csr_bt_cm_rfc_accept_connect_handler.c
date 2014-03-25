/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_RFC_MODULE

#include "csr_bt_cm_main.h"
#include "csr_bt_cm_rfc.h"
#include "csr_bt_cm_dm.h"
#include "csr_bt_cm_util.h"
#include "csr_bt_sc_private_lib.h"


static void csrBtCmAcceptConnectCfmMsgSend(CsrSchedQid                  appHandle,
                                           BD_ADDR_T            deviceAddr,
                                           CsrBtConnId           btConnId,
                                           CsrUint8              localServerChannel,
                                           CsrUint16             profileMaxFrameSize,
                                           CsrBtResultCode      resultCode,
                                           CsrBtSupplier  resultSupplier,
                                           CsrUint16     context)
{
    CsrBtCmConnectAcceptCfm        *prim;

    prim                        = (CsrBtCmConnectAcceptCfm *)CsrPmemAlloc(sizeof(CsrBtCmConnectAcceptCfm));
    prim->type                  = CSR_BT_CM_CONNECT_ACCEPT_CFM;
    prim->deviceAddr            = deviceAddr;
    prim->btConnId              = btConnId;
    prim->serverChannel         = localServerChannel;
    prim->profileMaxFrameSize   = profileMaxFrameSize;
    prim->resultCode            = resultCode;
    prim->resultSupplier        = resultSupplier;
    prim->context               = context;
    CsrBtCmPutMessage(appHandle, prim);
}

static void csrBtCmAcceptConnectErrorHandler(cmInstanceData_t     *cmData,
                                             CsrSchedQid               appHandle,
                                             CsrUint8        theServer,
                                             CsrBtResultCode      resultCode,
                                             CsrBtSupplier        resultSupplier,
                                             CsrUint16            context)
{
    CsrBtDeviceAddr        deviceAddr;

    CsrBtBdAddrZero(&deviceAddr);
    csrBtCmAcceptConnectCfmMsgSend(appHandle, deviceAddr, CSR_BT_CONN_ID_INVALID, theServer, 0, resultCode, resultSupplier, context);
    CsrBtCmServiceManagerLocalQueueHandler(cmData);
}



void CsrBtCmConnectAcceptCfmMsgSend(cmInstanceData_t    *cmData,
                             cmRfcConnElement    *theElement,
                             CsrBtResultCode     resultCode,
                             CsrBtSupplier resultSupplier)
{/* Send a Connect accept cfm signal to the application                            */
    cmRfcConnInstType * theLogicalLink = theElement->cmRfcConnInst;

    csrBtCmAcceptConnectCfmMsgSend(theLogicalLink->appHandle,
                                   theLogicalLink->deviceAddr,
                                   theLogicalLink->btConnId,
                                   theLogicalLink->serverChannel,
                                   theLogicalLink->profileMaxFrameSize,
                                   resultCode,
                                   resultSupplier,
                                   theLogicalLink->context);

    if(resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && resultSupplier == CSR_BT_SUPPLIER_CM)
    { /* A new connection is establish with success                                */
        aclTable *aclElement;
        
        CSR_BT_CM_STATE_CHANGE(theLogicalLink->state, connected_s);

        returnAclConnectionElement(cmData, theLogicalLink->deviceAddr, &aclElement);
        if( aclElement != NULL )
        {
            theLogicalLink->sniffSettings = aclElement->sniffSettings;
            theLogicalLink->parkSettings  = aclElement->parkSettings;
        }
        
        cmDmAclKickRoleSwitch(cmData);

        if(theLogicalLink->controlSignalQueue != NULL)
        { /* There is control signals on the controlSignalQueue. Restore
             signal and send it to the application                                */
            RFC_MODEM_STATUS_IND_T  *prim;

            prim        = (RFC_MODEM_STATUS_IND_T *) theLogicalLink->controlSignalQueue;
            CsrBtCmControlIndMsgSend(theLogicalLink, prim->modem_signal, prim->break_signal);
            CsrPmemFree(prim);
            theLogicalLink->controlSignalQueue = NULL;
        }

        if(theLogicalLink->dataControl.receivedBuffer[theLogicalLink->dataControl.restoreCount] != NULL)
        { /* There is data in the receivebuffer. Restore signal and send the
             payload to the application                                            */
            CsrBtCmRfcRestoreDataInReceiveBuffer(theLogicalLink);
        }
    }
    else
    { /* The attempt of creating a new connection fail, clean up the
         connection table                                                        */
        CsrBtScDeregisterReqSend(SEC_PROTOCOL_RFCOMM, theLogicalLink->serverChannel);
        CsrBtCmDmSyncClearPcmSlotFromTable(cmData, &theLogicalLink->eScoParms);
        cleanUpConnectionTable(&(theElement->cmRfcConnInst));
    }
    CsrBtCmServiceManagerLocalQueueHandler(cmData);
}

void CsrBtCmRfcAcceptConnectReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmConnectAcceptReq *cmPrim = (CsrBtCmConnectAcceptReq *) cmData->recvMsgP;
    cmConnIdServerType  connIdServ        = CsrBtCmReturnConnIdServerStruct(CSR_BT_CONN_ID_INVALID, cmPrim->serverChannel);
    cmRfcConnElement * theElement   = CM_FIND_RFC_ELEMENT(CsrBtCmRfcFindRfcConnElementFromConnIdServer, &(connIdServ));

    if(theElement == NULL && CsrBtCmRfcSetElementCounter(cmData))
    { /* Make sure that the server channel only can accept one connection,
         at the time. Store received parameters in the connection table,
         and start the connectable routine                                        */
        cmRfcConnElement  *theElement       = (cmRfcConnElement *) CsrCmnListElementAddLast(&(cmData->rfcVar.connList), sizeof(cmRfcConnElement));
        cmRfcConnInstType *connInst         = theElement->cmRfcConnInst;
        theElement->elementId               = cmData->rfcVar.elementCounter;
        cmData->rfcVar.connElement          = theElement;
        cmData->rfcVar.connectAcceptTimeOut = cmPrim->timeout;
        connInst->serverChannel             = cmPrim->serverChannel;
        connInst->profileMaxFrameSize       = cmPrim->profileMaxFrameSize;
        connInst->btConnId                  = CSR_BT_CONN_ID_INVALID;
        connInst->appHandle                 = cmPrim->appHandle;
        connInst->classOfDevice             = cmPrim->classOfDevice;
        connInst->sniffSettings             = cmData->dmVar.defaultSniffSettings;
        connInst->parkSettings              = cmData->dmVar.defaultParkSettings;
        connInst->context                   = cmPrim->context;
        connInst->modemStatus               = cmPrim->modemStatus;
        connInst->signalBreak               = cmPrim->breakSignal;
        connInst->mscTimeout                = cmPrim->mscTimeout;
        CSR_BT_CM_STATE_CHANGE(connInst->state, connectAble_s);

        /*
        Send a register request to SC for registering in the SM database.
        This is for incoming connection, i.e. when the device is server
        and activates a service. Use the profile uuid16_t and the allocated
        server channel                                                          */
        CsrBtScRegisterReqSend(cmPrim->profileUuid, connInst->serverChannel,
                          FALSE, /* Also applies for outgoing connections */
                          SEC_PROTOCOL_RFCOMM,
                          cmPrim->secLevel);

        /* This function starts the connectable routine                            */
        CsrBtCmWriteClassOfDevice(RFC_PLAYER);
    }
    else
    { /* The given server channel is already accepting a connection            */
        if(theElement)
        {
            csrBtCmAcceptConnectErrorHandler(cmData,
                                             cmPrim->appHandle,
                                             cmPrim->serverChannel,
                                             CSR_BT_RESULT_CODE_CM_ALREADY_CONNECTING,
                                             CSR_BT_SUPPLIER_CM, cmPrim->context);
        }
        else
        {
            csrBtCmAcceptConnectErrorHandler(cmData,
                                             cmPrim->appHandle,
                                             cmPrim->serverChannel,
                                             CSR_BT_RESULT_CODE_CM_INTERNAL_ERROR,
                                             CSR_BT_SUPPLIER_CM,
                                             cmPrim->context);
        }
    }
}

void CsrBtCmRfcConnectAcceptTimeoutHandler(cmInstanceData_t *cmData)
{
    CsrBtCmAcceptConnectTimeout *cmPrim = (CsrBtCmAcceptConnectTimeout *) cmData->recvMsgP;
    cmRfcConnElement *theElement = CsrBtCmRfcFindRfcConnElementFromServerState(cmData, cmPrim->serverChannel, cancelTimer_s);

    if(theElement)
    {
        cmData->rfcVar.connElement      = theElement;
        theElement->cmRfcConnInst->btConnId = CSR_BT_CONN_ID_INVALID;
        CsrBtCmWriteClassOfDevice(RFC_PLAYER);
    }
    else
    {/* Restore RFC queue                                                    */
        CsrBtCmServiceManagerLocalQueueHandler(cmData);
    }
}

#endif /* #ifndef EXCLUDE_CSR_BT_RFC_MODULE */
