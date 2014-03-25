/****************************************************************************

Copyright Cambridge Silicon Radio Limited and its licensors 2011. All rights reserved.

REVISION:      $Revision: #4 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE

#include "csr_bt_cm_l2cap.h"
#include "csr_bt_cm_events_handler.h"

static void csrBtCmL2caDataWriteCfmMsgSend(CsrSchedQid appHandle, CsrBtConnId btConnId, CsrUint16 context,
                                           CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    CsrBtCmL2caDataCfm *cmPrim;
    cmPrim  = (CsrBtCmL2caDataCfm *) CsrPmemAlloc(sizeof(CsrBtCmL2caDataCfm));
    cmPrim->type = CSR_BT_CM_L2CA_DATA_CFM;
    cmPrim->btConnId = btConnId;
    cmPrim->resultCode = resultCode;
    cmPrim->resultSupplier = resultSupplier;
    cmPrim->context = context;
    CsrBtCmPutMessage(appHandle, cmPrim);
}

static void csrBtCmL2caDataReadIndMsgSend(CsrSchedQid appHandle, CsrBtConnId btConnId,
                                          CsrMblk *mblk, CsrUint16 context)
{
    CsrBtCmL2caDataInd *cmPrim;
    cmPrim = (CsrBtCmL2caDataInd *) CsrPmemAlloc(sizeof(CsrBtCmL2caDataInd));
    cmPrim->type = CSR_BT_CM_L2CA_DATA_IND;
    cmPrim->btConnId = btConnId;
    cmPrim->length = CsrMblkGetLength(mblk);
    cmPrim->payload = CsrBtMblkConsumeToMemory(&mblk);
    cmPrim->context = context;
    CsrBtCmPutMessage(appHandle, cmPrim);
}

#ifdef CSR_BT_INSTALL_CM_PRI_L2CA_DATA_ABORT
static void csrBtCmL2caDataAbortCfmMsgSend(CsrSchedQid appHandle, CsrBtConnId btConnId, CsrUint16 context,
                                           CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    CsrBtCmL2caDataAbortCfm *cmPrim = (CsrBtCmL2caDataAbortCfm *) 
                                       CsrPmemAlloc(sizeof(CsrBtCmL2caDataAbortCfm));

    cmPrim->type            = CSR_BT_CM_L2CA_DATA_ABORT_CFM;
    cmPrim->btConnId        = btConnId;
    cmPrim->resultCode      = resultCode;
    cmPrim->resultSupplier  = resultSupplier;
    cmPrim->context         = context;
    CsrBtCmPutMessage(appHandle, cmPrim);
}
#endif
static void csrBtCmL2caDatareadIndProcess(cmInstanceData_t *cmData,
                                          cmL2caConnInstType *l2capConn)
{
    L2CA_DATAREAD_IND_T *prim;
    prim = cmData->recvMsgP;

    if((l2capConn->rxAppCredits > 0) &&
       (l2capConn->rxQueue == NULL))
    {
        /* Pass to app as we have credits */
        l2capConn->rxAppCredits--;
        csrBtCmL2caDataReadIndMsgSend(l2capConn->appHandle,
                                      CM_CREATE_L2CA_CONN_ID(prim->cid),
                                      prim->data,
                                      l2capConn->context);
        prim->data = NULL;

        /* Ack data as rx queue is empty */
        if(l2capConn->ertm)
        {
            L2CA_DataReadRsp(prim->cid, prim->packets);
        }
    }
    else
    {
        /* Application already processing a data indication. Store on
         * queue and do some clever accounting */
        l2capConn->rxQueueCount++;

        if(l2capConn->ertm)
        {
            /* Enhanced retransmission mode acking */
            if(l2capConn->rxQueueCount < l2capConn->rxQueueMax &&
               l2capConn->rxQueueOverflow == 0)
            {
                L2CA_DataReadRsp(prim->cid, prim->packets);
            }
            else
            {
                /* Peer has sent more than we anticipated, and the RnR
                 * (busy) does not seem to take effect. Don't ack
                 * these packets in order to eventually fill up the
                 * window completely */
                l2capConn->rxQueueOverflow++;
                l2capConn->rxQueueOverpack += prim->packets;
            }
            
            /* Mark as busy if buffer is full */
            if(!l2capConn->rxBusy &&
               (l2capConn->rxQueueCount >= l2capConn->rxQueueMax ||
                l2capConn->rxQueueOverflow > 0))
            {
                l2capConn->rxBusy = TRUE;
                L2CA_BusyReq(prim->cid, l2capConn->rxBusy);
            }
        }
        
        CsrMessageQueuePush(&l2capConn->rxQueue, L2CAP_PRIM, prim);
        cmData->recvMsgP = NULL;

        if(l2capConn->rxAppCredits > 0)
        {
            CsrBtCmL2caRestoreRxQueue(cmData, l2capConn);
        }
    }
}

static CsrBool csrBtCmL2caDataReadCombine(cmL2caConnInstType *l2capConn,
                                          L2CA_DATAREAD_IND_T *prim)
{
    /* Always join new fragment to end */
    if((prim->result == L2CA_DATA_PARTIAL) ||
       (prim->result == L2CA_DATA_PARTIAL_END))
    {
        l2capConn->combine = CsrMblkAddTail(l2capConn->combine,
                                            prim->data);
        
        if(prim->result == L2CA_DATA_PARTIAL_END)
        {
            /* Fully resegmented. Pass data in to normal handler */
            prim->result = L2CA_DATA_SUCCESS;
            prim->data = l2capConn->combine;
            l2capConn->combine = NULL;
            return TRUE;
        }
        else
        {
            /* Still incomplete */
            prim->data = NULL;
        }
    }
    else
    {
        /* Packets lost (supposedly because we're in streaming mode)
         * so clean up and prepare for next batch */
        if(l2capConn->combine != NULL)
        {
            CsrMblkDestroy(l2capConn->combine);
            l2capConn->combine = NULL;
        }
        if(prim->data != NULL)
        {
            CsrMblkDestroy(prim->data);
            prim->data = NULL;
        }
    }

    /* Good case bailed out early */
    return FALSE;
}

void CsrBtCmL2caRestoreRxQueue(cmInstanceData_t *cmData,
                               cmL2caConnInstType *l2capConn)
{
    CsrUint16 eventClass;
    void *msg;
    while((l2capConn->rxAppCredits > 0) &&
          (l2capConn->rxQueueCount > 0) &&
          CsrMessageQueuePop(&l2capConn->rxQueue, &eventClass, &msg))
    {
        L2CA_DATAREAD_IND_T *prim = (L2CA_DATAREAD_IND_T*)msg;
        
        l2capConn->rxAppCredits--;
        l2capConn->rxQueueCount--;
        csrBtCmL2caDataReadIndMsgSend(l2capConn->appHandle,
                                      CM_CREATE_L2CA_CONN_ID(prim->cid),
                                      prim->data,
                                      l2capConn->context);
        CsrPmemFree(prim);
    }
}

void CsrBtCmL2caDataReadIndHandler(cmInstanceData_t *cmData)
{
    L2CA_DATAREAD_IND_T *prim = (L2CA_DATAREAD_IND_T *) cmData->recvMsgP;
    cmL2caConnElement *connElem;
    cmL2caConnInstType *l2capConn;
    CsrBtConnId btConnId;

    btConnId = CM_CREATE_L2CA_CONN_ID(prim->cid);
    connElem = CM_FIND_L2CA_ELEMENT(CsrBtCmL2caFindL2caConnElementFromBtConnId,
                                    &btConnId);
    l2capConn = (cmL2caConnInstType*)(connElem != NULL
                                      ? connElem->cmL2caConnInst
                                      : NULL);

    if(l2capConn != NULL)
    {
        if(((L2CA_RESULT_T)prim->result == L2CA_RESULT_READ_PARTIAL) ||
           (l2capConn->combine != NULL))
        {
            if(!csrBtCmL2caDataReadCombine(l2capConn, prim))
            {
                /* Partial packet not complete, so ack the fragment we
                 * just received and bail out. The packet counter here
                 * isn't related to the l2capConn->rxPackets one */
                L2CA_DataReadRsp(CM_GET_UINT16ID_FROM_BTCONN_ID(l2capConn->btConnId),
                                 prim->packets);
                return;
            }
        }

        if(prim->result == L2CA_DATA_SUCCESS)
        {
            if((l2capConn->state == l2capConnected_s) ||
               (l2capConn->state == l2capConnectAccept_s) ||
               (l2capConn->state == l2capConnect_s) ||
               (l2capConn->state == l2capConnectAcceptFinal_s) ||
               (l2capConn->state == l2capSaveDisconnect_s))
            {
                csrBtCmL2caDatareadIndProcess(cmData, l2capConn);
            }
            else
            {
                /* The application is not ready to received any
                 * data. In order to prevent a memory leak the payload
                 * is freed */
                L2CA_DataReadRsp(CM_GET_UINT16ID_FROM_BTCONN_ID(l2capConn->btConnId),
                                 prim->packets);
                CsrMblkDestroy(prim->data);
                prim->data = NULL;
            }
        }
        else
        {
            /* Data error. Discard and no ack */
            CsrMblkDestroy(prim->data);
            prim->data = NULL;
        }
    }
#ifdef CSR_BT_INSTALL_L2CAP_CONNLESS_SUPPORT
    else if(CsrBtCmL2caConnlessDatareadIndHandler(cmData))
    {
        /* Processed by connectionless data handler */
    }
#endif
    else
    {
        /* Unknown connection */
        CsrMblkDestroy(prim->data);
        prim->data = NULL;
    }
}

void CsrBtCmL2caDataResHandler(cmInstanceData_t *cmData)
{
    CsrBtCmL2caDataRes *cmPrim = (CsrBtCmL2caDataRes*)cmData->recvMsgP;
    cmL2caConnElement *connElem = CM_FIND_L2CA_ELEMENT(CsrBtCmL2caFindL2caConnElementFromBtConnId,
                                                       &(cmPrim->btConnId));
    
    if (connElem && connElem->cmL2caConnInst)
    {
        cmL2caConnInstType *l2capConn;
        l2capConn = connElem->cmL2caConnInst;

        if(l2capConn->ertm)
        {
            /* If the queue had overflowed we need to give back
             * credits here */
            if(l2capConn->rxQueueOverflow > 0)
            {
                /* Keep the 'overflowed packets' counter as close to
                 * the number of 'overflowed indications' */
                CsrUint16 packets;
                l2capConn->rxQueueOverflow--;
                packets = (CsrUint16)(l2capConn->rxQueueOverpack >= l2capConn->rxQueueOverflow
                                      ? l2capConn->rxQueueOverpack - l2capConn->rxQueueOverflow
                                      : 0);
                l2capConn->rxQueueOverpack -= packets;
                L2CA_DataReadRsp(CM_GET_UINT16ID_FROM_BTCONN_ID(cmPrim->btConnId),
                                 packets);
            }

            /* If buffer level is low again, un-busy */
            if(l2capConn->rxBusy &&
               (l2capConn->rxQueueOverflow == 0) &&
               (l2capConn->rxQueueCount < l2capConn->rxQueueMax))
            {
                l2capConn->rxBusy = FALSE;
                L2CA_BusyReq(CM_GET_UINT16ID_FROM_BTCONN_ID(cmPrim->btConnId), l2capConn->rxBusy);
            }
        }

        /* Process waiting data */
        if(l2capConn->rxAppCredits > 0)
        {
            CsrBtCmGeneralException(CSR_BT_CM_PRIM, CSR_BT_CM_L2CA_DATA_RES,
                                    "Rx RSP flow control violated");
        }
        l2capConn->rxAppCredits++;
        CsrBtCmL2caRestoreRxQueue(cmData, l2capConn);
    }
}

void CsrBtCmL2caDataWriteReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmL2caDataReq *cmPrim;
    cmL2caConnElement *connElem;

    cmPrim = (CsrBtCmL2caDataReq*)cmData->recvMsgP;
    connElem = CM_FIND_L2CA_ELEMENT(CsrBtCmL2caFindL2caConnElementFromBtConnId,
                                    &cmPrim->btConnId);

    if (connElem)
    {
        cmL2caConnInstType *l2capConn = connElem->cmL2caConnInst;
        CsrBool tx;

        l2capConn->txCount++;

        if(l2capConn->txCount < l2capConn->txMaxCount)
        {
            /* Immediate ack */
            tx = TRUE;
            csrBtCmL2caDataWriteCfmMsgSend(l2capConn->appHandle,
                                           cmPrim->btConnId,
                                           cmPrim->context,
                                           CSR_BT_RESULT_CODE_CM_SUCCESS,
                                           CSR_BT_SUPPLIER_CM);
        }
        else if(l2capConn->txCount == l2capConn->txMaxCount)
        {
            /* Buffer full, no ack. Store context number */
            tx = TRUE;
            l2capConn->txPendingContext = cmPrim->context;
        }
        else
        {
            /* Buffer overflow - discard */
            tx = FALSE;
            l2capConn->txCount--;
            CsrBtCmGeneralException(CSR_BT_CM_PRIM, cmPrim->type,
                                    "Tx REQ flow control violated");
            csrBtCmL2caDataWriteCfmMsgSend(l2capConn->appHandle,
                                           cmPrim->btConnId,
                                           cmPrim->context,
                                           CSR_BT_RESULT_CODE_CM_FLOW_CONTROL_VIOLATED,
                                           CSR_BT_SUPPLIER_CM);
            CsrMblkDestroy(cmPrim->payload);
            cmPrim->payload = NULL;
        }

        if(tx)
        {
#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_HIGH_PRIORITY_DATA
            if (l2capConn->txCount == 1)
            {
                if (l2capConn->dataPriority == CSR_BT_CM_PRIORITY_HIGH ||
                    l2capConn->dataPriority == CSR_BT_CM_PRIORITY_HIGHEST)
                {
                    CsrBool start = TRUE;

                    CsrBtCmPropgateEvent(cmData,
                                         CsrBtCmPropagateHighPriorityIndStatusEvents,
                                         CSR_BT_CM_EVENT_MASK_SUBSCRIBE_HIGH_PRIORITY_DATA,
                                         HCI_SUCCESS,
                                         &l2capConn->deviceAddr,
                                         &start);
                }
            }
#endif
            /* Pass to L2CAP. L2CAP has it's own tx queue so there is no
             * need to have one here too */
            L2CA_DataWriteReqEx(CM_GET_UINT16ID_FROM_BTCONN_ID(cmPrim->btConnId),
                                cmPrim->length,
                                cmPrim->payload,
                                cmPrim->context);
        }
    }
    else
    {
        /* Unknown connection, ignore it */
        CsrMblkDestroy(cmPrim->payload);
        cmPrim->payload = NULL;
    }
}

void CsrBtCmL2caDataWriteCfmHandler(cmInstanceData_t *cmData)
{
    /* Confirmation of L2CAP packet is sent */
    L2CA_DATAWRITE_CFM_T *prim = (L2CA_DATAWRITE_CFM_T *) cmData->recvMsgP;
    cmL2caConnElement *connElem;
    cmL2caConnInstType *l2capConn;
    CsrBtConnId btConnId;

    btConnId = CM_CREATE_L2CA_CONN_ID(prim->cid);
    connElem = CM_FIND_L2CA_ELEMENT(CsrBtCmL2caFindL2caConnElementFromBtConnId, &btConnId);
    l2capConn = (cmL2caConnInstType*)(connElem != NULL
                                      ? connElem->cmL2caConnInst
                                      : NULL);

    if (l2capConn)
    {
        /* We can not really do anything about failed datawrites as we
         * have already passed up the CFM to the application. In case
         * of a link loss, the app is sure to know anyway... */

        if(l2capConn->txCount >= l2capConn->txMaxCount)
        {
            /* Buffer was full, so this will be the unlocker. Send up
             * stored context */
            if (prim->result != L2CA_DATA_LOCAL_ABORTED)
            {
                csrBtCmL2caDataWriteCfmMsgSend(l2capConn->appHandle,
                                               btConnId,
                                               l2capConn->txPendingContext,
                                               CSR_BT_RESULT_CODE_CM_SUCCESS,
                                               CSR_BT_SUPPLIER_CM);
                l2capConn->txPendingContext = 0xFFFF;
            }
        }
        else
        {
            /* Buffer wasn't full. Ack already sent */
        }
        l2capConn->txCount--;
#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_HIGH_PRIORITY_DATA
        if (l2capConn->txCount == 0)
        {
            if (l2capConn->dataPriority == CSR_BT_CM_PRIORITY_HIGH ||
                l2capConn->dataPriority == CSR_BT_CM_PRIORITY_HIGHEST)
            {
                CsrBool start = FALSE;

                CsrBtCmPropgateEvent(cmData,
                                     CsrBtCmPropagateHighPriorityIndStatusEvents,
                                     CSR_BT_CM_EVENT_MASK_SUBSCRIBE_HIGH_PRIORITY_DATA,
                                     HCI_SUCCESS,
                                     &l2capConn->deviceAddr,
                                     &start);
            }
        }
#endif
        if (l2capConn->pendingBufferStatus && l2capConn->txCount == 0)
        {
            l2capConn->pendingBufferStatus = FALSE;
            CsrBtCmDataBufferEmptyCfmSend(l2capConn->appHandle, l2capConn->context);
        }
    }
#ifdef CSR_BT_INSTALL_L2CAP_CONNLESS_SUPPORT
    else if(CsrBtCmL2caConnlessDatawriteCfmHandler(cmData))
    {
        /* Processed by connectionless data handler */
    }
#endif
    else
    {
        /* Ignore, unknown connection */        
    }
}

void CsrBtCmL2caBusyIndHandler(cmInstanceData_t *cmData)
{
    /* Ignore these events. L2CAP will ensure we don't send any new I
     * frames, so rely on the window to fill up */
}

/* Get L2CAP channel information */
void CsrBtCmL2caGetChannelInfoCfmHandler(cmInstanceData_t *cmData)
{
    L2CA_GET_CHANNEL_INFO_CFM_T *prim = (L2CA_GET_CHANNEL_INFO_CFM_T *) cmData->recvMsgP;
    CsrBtConnId btConnId             = CM_CREATE_L2CA_CONN_ID(prim->cid);
    cmL2caConnElement *connElem      = CM_FIND_L2CA_ELEMENT(CsrBtCmL2caFindL2caConnElementFromBtConnId, &btConnId);
    cmL2caConnInstType *l2capConn    = (cmL2caConnInstType*)(connElem != NULL ? connElem->cmL2caConnInst : NULL);

    if (l2capConn)
    {    
        CsrBtCmL2caGetChannelInfoCfm *cmPrim = (CsrBtCmL2caGetChannelInfoCfm *)CsrPmemAlloc(sizeof(CsrBtCmL2caGetChannelInfoCfm));

        cmPrim->type             = CSR_BT_CM_L2CA_GET_CHANNEL_INFO_CFM;
        cmPrim->aclHandle        = prim->conn_handle;
        cmPrim->remoteCid        = prim->remote_cid;
        cmPrim->resultCode       = CSR_BT_RESULT_CODE_CM_SUCCESS;
        cmPrim->resultSupplier   = CSR_BT_SUPPLIER_CM;

        CsrSchedMessagePut(l2capConn->appHandle, CSR_BT_CM_PRIM, cmPrim);
    }
}

void CsrBtCmL2caGetChannelInfoReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmL2caGetChannelInfoReq *cmPrim  = (CsrBtCmL2caGetChannelInfoReq *) cmData->recvMsgP;
    if (CM_FIND_L2CA_ELEMENT(CsrBtCmL2caFindL2caConnElementFromBtConnId, &cmPrim->btConnId))
    {
        L2CA_GET_CHANNEL_INFO_REQ_T *ptr = CsrPmemAlloc(sizeof(L2CA_GET_CHANNEL_INFO_REQ_T));
            
        ptr->type = L2CA_GET_CHANNEL_INFO_REQ;
        ptr->cid  = (l2ca_cid_t)(cmPrim->btConnId);
        CsrSchedMessagePut(L2CAP_IFACEQUEUE, L2CAP_PRIM, ptr);
    }
    else
    { /* Unknown connection: let the requesting application know */
        CsrBtCmL2caGetChannelInfoCfm *prim = (CsrBtCmL2caGetChannelInfoCfm *)CsrPmemAlloc(sizeof(CsrBtCmL2caGetChannelInfoCfm));
        prim->type             = CSR_BT_CM_L2CA_GET_CHANNEL_INFO_CFM;
        prim->aclHandle        = 0xFFFF;
        prim->remoteCid        = 0xFFFF;
        prim->resultCode       = CSR_BT_RESULT_CODE_CM_UNKNOWN_CONNECTION_IDENTIFIER;
        prim->resultSupplier   = CSR_BT_SUPPLIER_CM;
        CsrSchedMessagePut(cmPrim->appHandle, CSR_BT_CM_PRIM, prim);
    }
}

#ifdef CSR_BT_INSTALL_CM_PRI_L2CA_DATA_ABORT
void CsrBtCmL2caDataAbortReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmL2caDataAbortReq *cmPrim  = (CsrBtCmL2caDataAbortReq *) cmData->recvMsgP;

    if (CM_FIND_L2CA_ELEMENT(CsrBtCmL2caFindL2caConnElementFromBtConnId, &cmPrim->btConnId))
    {
        L2CA_DataWriteAbortReq(CM_GET_UINT16ID_FROM_BTCONN_ID(cmPrim->btConnId));
    }
    else
    { /* Unknown connection, ignore it */

    }
}
void CsrBtCmL2caDatawriteAbortCfmHandler(cmInstanceData_t *cmData)
{
    L2CA_DATAWRITE_ABORT_CFM_T *prim = (L2CA_DATAWRITE_ABORT_CFM_T *) cmData->recvMsgP;
    CsrBtConnId btConnId             = CM_CREATE_L2CA_CONN_ID(prim->cid);
    cmL2caConnElement *connElem      = CM_FIND_L2CA_ELEMENT(CsrBtCmL2caFindL2caConnElementFromBtConnId, &btConnId);
    cmL2caConnInstType *l2capConn    = (cmL2caConnInstType*)(connElem != NULL ? connElem->cmL2caConnInst : NULL);

    if (l2capConn && l2capConn->state == l2capConnected_s)
    {
        csrBtCmL2caDataAbortCfmMsgSend(l2capConn->appHandle,
                                       btConnId,
                                       l2capConn->context,
                                       CSR_BT_RESULT_CODE_CM_SUCCESS,
                                       CSR_BT_SUPPLIER_CM);
    }
    else
    { /* Unknown connection, ignore it */
        ;
    }
}
#endif

void CsrBtCmL2caDataStart(cmInstanceData_t *cmData, cmL2caConnInstType *l2capConn)
{
    /* Connection is ready. Provide the initial application credit and
     * start the processing loop */
    l2capConn->rxAppCredits = 1;
    CsrBtCmL2caRestoreRxQueue(cmData, l2capConn);
}

#endif /* #ifndef EXCLUDE_CSR_BT_L2CA_MODULE */
