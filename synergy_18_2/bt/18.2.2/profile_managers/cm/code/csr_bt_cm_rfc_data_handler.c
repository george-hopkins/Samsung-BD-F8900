/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_RFC_MODULE

#include "csr_bt_cm_main.h"
#include "csr_bt_cm_rfc.h"
#include "csr_bt_util.h"

static void csrBtCmRfcDataCfmMsgSend(CsrSchedQid appHandle, CsrBtConnId btConnId, CsrUint16 context)
{ /* Build and send CsrBtCmDataCfm signal to the application */
    CsrBtCmDataCfm                 *cmPrim;

    cmPrim = (CsrBtCmDataCfm *)CsrPmemAlloc(sizeof(CsrBtCmDataCfm));

    cmPrim->type                     = CSR_BT_CM_DATA_CFM;
    cmPrim->btConnId                 = btConnId;
    cmPrim->context                  = context;
    CsrBtCmPutMessage(appHandle, cmPrim);
}

static void csrBtCmRfcCreditBasedDataWriteCfmHandler(cmRfcConnInstType * theLogicalLink, RFC_DATAWRITE_CFM_T *rfcPrim)
{
    if (rfcPrim->status == RFC_DATAWRITE_PENDING)
    { /* An RFCOMM data packet is sent to l2cap                     */
        if (theLogicalLink->dataControl.txCount < CSR_BT_CM_RFC_TX_WINDOW_SIZE)
        { /* Build and send CSR_BT_CM_DATA_CFM to the application   */
            theLogicalLink->dataControl.dataCfmPending = FALSE;
            csrBtCmRfcDataCfmMsgSend(theLogicalLink->appHandle, theLogicalLink->btConnId, theLogicalLink->context);
        }
        else
        { /* The CM must ensure that no more than 
             CSR_BT_CM_RFC_TX_WINDOW_SIZE packet is sent to the 
             l2cap buffer.                                          */
            theLogicalLink->dataControl.dataCfmPending = TRUE;
        }
    }
    else
    { /* An RFCOMM data packet is confirm by l2cap                  */
        if (theLogicalLink->dataControl.dataCfmPending == TRUE)
        { /* Build and send CSR_BT_CM_DATA_CFM to the application   */
            theLogicalLink->dataControl.dataCfmPending = FALSE;
            csrBtCmRfcDataCfmMsgSend(theLogicalLink->appHandle, theLogicalLink->btConnId, theLogicalLink->context);
        }
        theLogicalLink->dataControl.txCount--;

        if (theLogicalLink->dataControl.pendingBufferStatus && theLogicalLink->dataControl.txCount == 0)
        {
            theLogicalLink->dataControl.pendingBufferStatus = FALSE;
            CsrBtCmDataBufferEmptyCfmSend(theLogicalLink->appHandle, theLogicalLink->context);
        }
    }
}

static void csrBtCmRfcDataIndMsgSend(CsrSchedQid appHandle, CsrBtConnId btConnId,
                         CsrUint16 payloadLength, CsrUint8 *payload, CsrUint16 context)
{ /* Build and send CsrBtCmDataInd signal to the application */
    CsrBtCmDataInd                 *cmPrim;

    cmPrim = (CsrBtCmDataInd *)CsrPmemAlloc(sizeof(CsrBtCmDataInd));

    cmPrim->type                     = CSR_BT_CM_DATA_IND;
    cmPrim->btConnId                 = btConnId;
    cmPrim->payloadLength            = payloadLength;
    cmPrim->payload                    = payload;
    cmPrim->context                 = context;
    CsrBtCmPutMessage(appHandle, cmPrim);
}

static void csrBtCmRfcDataMsgSave(cmInstanceData_t *cmData, cmRfcConnInstType * theLogicalLink)
{ /* Received a data packet from the remote device, while the application is not
     ready to received it. The CM will try to save the data packet in it receive
     buffer or CsrPmemFree it if buffer is full. This is done to prevent a memory leak */
    RFC_DATAREAD_IND_T *rfcPrim;
    rfcPrim = (RFC_DATAREAD_IND_T *) cmData->recvMsgP;

    if(theLogicalLink->dataControl.receivedBuffer[theLogicalLink->dataControl.saveCount] == NULL)
    { /* Saving the RFC_DATA_IND signal in the receivebuffer */
        theLogicalLink->dataControl.receivedBuffer[theLogicalLink->dataControl.saveCount] = cmData->recvMsgP;
        cmData->recvMsgP = NULL;
        theLogicalLink->dataControl.saveCount++;
        if(theLogicalLink->dataControl.saveCount > (CSR_BT_CM_INIT_CREDIT - 1))
        {
            theLogicalLink->dataControl.saveCount = 0;
        }
    }
    else
    { /* The received buffer is full, CsrPmemFree the payload to prevent a memory leak */
        CsrMblkDestroy((CsrMblk *) rfcPrim->payload);
    }
}

static void csrBtCmRfcReceivedDataPacketHandler(cmInstanceData_t *cmData, cmRfcConnInstType *theLogicalLink)
{
    RFC_DATAREAD_IND_T        *rfcPrim;

    rfcPrim = (RFC_DATAREAD_IND_T *) cmData->recvMsgP;

    if(theLogicalLink->state == connected_s)
    { /* This connection is in a state where the application is ready
         to received data. */
        if(theLogicalLink->dataControl.dataResReceived)
        { /* The application is ready to receive data. Build and send
             the data packet to the application */
            theLogicalLink->dataControl.dataResReceived = FALSE;
            csrBtCmRfcDataIndMsgSend(theLogicalLink->appHandle,
                                     theLogicalLink->btConnId,
                                     rfcPrim->payload_length,
                                     CsrBtMblkConsumeToMemory((CsrMblk **) &rfcPrim->payload),
                                     theLogicalLink->context);
        }
        else
        { /* The application is not ready to receive the data packet.
             Place it on the received buffer, or release it if
             buffer is full */
            csrBtCmRfcDataMsgSave(cmData, theLogicalLink);
        }
    }
    else
    {/* This connection is in a state where the application is not
        ready to received data. */
        if(theLogicalLink->state == connect_s        ||
           theLogicalLink->state == connectAccept_s    ||
           theLogicalLink->state == connectAcceptFinal_s)
        { /* RFC_DATA_IND received before application have received a
             CSR_BT_CM_CONNECT_CFM or CSR_BT_CM_CONNECT_ACCEPT_CFM. Save it */
            csrBtCmRfcDataMsgSave(cmData, theLogicalLink);
        }
        else
        { /* This connection is in a state where the profile is not ready
             to received data. Therefor the payload is CsrPmemFree */
            CsrMblkDestroy((CsrMblk *) rfcPrim->payload);
        }
    }
}

void CsrBtCmRfcRestoreDataInReceiveBuffer(cmRfcConnInstType *theLogicalLink)
{ /* The application is now ready to receive data. Restore RFC_DATA_IND a
     signal from the receive buffer, and build and send a CSR_BT_CM_DATA_IND
     signal to the application */
    RFC_DATAREAD_IND_T  *prim;

    prim = (RFC_DATAREAD_IND_T *) theLogicalLink->dataControl.receivedBuffer[theLogicalLink->dataControl.restoreCount];

    csrBtCmRfcDataIndMsgSend(theLogicalLink->appHandle,
                        theLogicalLink->btConnId,
                        prim->payload_length,
                        CsrBtMblkConsumeToMemory((CsrMblk **) &prim->payload),
                        theLogicalLink->context);

    CsrMblkDestroy((CsrMblk *) prim->payload);
    CsrPmemFree(prim);

    theLogicalLink->dataControl.dataResReceived = FALSE;
    theLogicalLink->dataControl.receivedBuffer[theLogicalLink->dataControl.restoreCount] = NULL;
    theLogicalLink->dataControl.restoreCount++;

    if(theLogicalLink->dataControl.restoreCount > (CSR_BT_CM_INIT_CREDIT - 1))
    {
        theLogicalLink->dataControl.restoreCount = 0;
    }
}



void CsrBtCmRfcDataReqHandler(cmInstanceData_t *cmData)
{ /* This event request the CM to transmit the data given in the payload.
     With this function finds out if this RFCOMM using credit based
     flowcontrol or not. */
    CsrBtCmDataReq   *cmPrim      = (CsrBtCmDataReq *) cmData->recvMsgP;
    cmRfcConnElement *theElement  = CM_FIND_RFC_ELEMENT(CsrBtCmRfcFindRfcConnElementFromBtConnId, &(cmPrim->btConnId));

    if(theElement)
    { /* The CM has identify the index in the connection table */
        cmRfcConnInstType *theLogicalLink = theElement->cmRfcConnInst;

        if(theLogicalLink->state == connected_s)
        {
            theLogicalLink->dataControl.txCount++;
            RfcDatawriteReqSend(CM_GET_UINT16ID_FROM_BTCONN_ID(cmPrim->btConnId), 0, cmPrim->payloadLength, cmPrim->payload);
        }
        else
        { /* This connection is in a state where the profile is not allow
             to send data. Therefor the payload is CsrPmemFree */
            CsrPmemFree(cmPrim->payload);
        }
    }
    else
    { /* Could not identify the conneciton id and server channel, CsrPmemFree the payload to
         prevent a memory leak */
        CsrPmemFree(cmPrim->payload);
    }
}

void CsrBtCmRfcDataIndHandler(cmInstanceData_t *cmData)
{ /* This event indicates that data has arrived from the remote device
     and is stored in the location given by payload. */
    RFC_DATAREAD_IND_T   *rfcPrim     = (RFC_DATAREAD_IND_T *) cmData->recvMsgP;
    CsrBtConnId val = CM_CREATE_RFC_CONN_ID(rfcPrim->conn_id);
    cmRfcConnElement * theElement    = CM_FIND_RFC_ELEMENT(CsrBtCmRfcFindRfcConnElementFromBtConnId, &(val));

    if(theElement)
    { /* The CM has identify the index in the connection table */
        cmRfcConnInstType *theLogicalLink   = theElement->cmRfcConnInst;
        
        if(rfcPrim->payload != NULL)
        {
            rfcPrim->payload_length = CsrMblkGetLength((CsrMblk *)rfcPrim->payload);
            if(rfcPrim->payload_length > 0)
            { /* The received data packet is valid to the application. If the
                 application is ready to received data, the function will send
                 the data packet to it, else the packet is place on the receive
                 buffer */
                csrBtCmRfcReceivedDataPacketHandler(cmData, theLogicalLink);
            }
            else
            { /* Just a credit update */
                CsrMblkDestroy((CsrMblk *) rfcPrim->payload);
            }
        }
    }
    else
    { /* Could not identify the connection id and server channel, CsrPmemFree the payload to
         prevent a memory leak */
         CsrMblkDestroy((CsrMblk *) rfcPrim->payload);
    }
}

void CsrBtCmRfcDataResHandler(cmInstanceData_t *cmData)
{ /* This event indicates that the application is ready to received data */
    CsrBtCmDataRes    *cmPrim      = (CsrBtCmDataRes *) cmData->recvMsgP;
    cmRfcConnElement * theElement  = CM_FIND_RFC_ELEMENT(CsrBtCmRfcFindRfcConnElementFromBtConnId, &(cmPrim->btConnId));

    if(theElement)
    { /* The CM has identify the index in the connection table */
        cmRfcConnInstType *theLogicalLink   = theElement->cmRfcConnInst;

        if(theLogicalLink->state == connected_s)
        { /* The connection is in a state where it is ready to received
             this signal */
            RfcDatareadRspSend((CsrUint16)(theLogicalLink->btConnId & CSR_BT_CONN_ID_GET_MASK));

            if(theLogicalLink->dataControl.receivedBuffer[theLogicalLink->dataControl.restoreCount] != NULL)
            { /* There is data in the receivebuffer. Restore signal and
                 send the payload to the application */
                CsrBtCmRfcRestoreDataInReceiveBuffer(theLogicalLink);
            }
            else
            { /* Indicates that the application is ready to received data */
                theLogicalLink->dataControl.dataResReceived = TRUE;
            }
        }
    }
    else
    { /* No owner, just ignore                                              */
        ;
    }
}

void CsrBtCmRfcDataWriteCfmHandler(cmInstanceData_t *cmData)
{ /* This event indicates that the previous RFC_DATAWRITE_REQ was
     successfully sent from L2CAP                                   */
    RFC_DATAWRITE_CFM_T *rfcPrim  = (RFC_DATAWRITE_CFM_T *) cmData->recvMsgP;
    CsrBtConnId   value = CM_CREATE_RFC_CONN_ID(rfcPrim->conn_id);
    cmRfcConnElement * theElement    = CM_FIND_RFC_ELEMENT(CsrBtCmRfcFindRfcConnElementFromBtConnId, &(value));

    if(theElement)
    { /* The CM has identify the index in the connection table      */
        csrBtCmRfcCreditBasedDataWriteCfmHandler(theElement->cmRfcConnInst, rfcPrim);
    }
    else
    { /* No owner, just ignore                                      */
        ;
    }
}


#endif /* #ifndef EXCLUDE_CSR_BT_RFC_MODULE */
