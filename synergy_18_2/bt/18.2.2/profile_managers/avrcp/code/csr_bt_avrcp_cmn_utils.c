/****************************************************************************

            Copyright Cambridge Silicon Radio Limited and its licensors 2011.

            All rights reserved.

REVISION:   $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_AVRCP_MODULE

#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_avrcp_main.h"
#include "csr_bt_avrcp_prim.h"
#include "csr_bt_avrcp_lib.h"

/***** Static functions *****/

static CsrUint8 csrBtAvrcpUtilGetAppConnId(AvrcpInstanceData_t *instData)
{
    AvrcpConnInstance_t *connInst = AVRCP_LIST_CONN_GET_FIRST(&instData->connList);
    CsrUint8 currConnId = 0;

    while (connInst)
    {
        if (connInst->appConnId == currConnId)
        {/* Connection ID already in use - skip to next number and restart */
            currConnId++;
            connInst = AVRCP_LIST_CONN_GET_FIRST(&instData->connList);
        }
        else
        {
            connInst = connInst->next;
        }
    }

    /* Lowest possible connection ID found */
    return currConnId;
}

static void avrcpSendHouseCleaning(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpHouseCleaning *prim = pnew(CsrBtAvrcpHouseCleaning);

    prim->type = CSR_BT_AVRCP_HOUSE_CLEANING;
    instData->restoreFlag = TRUE;
    CsrBtAvrcpMessagePut(CSR_BT_AVRCP_IFACEQUEUE, prim);
}

/***** Public functions *****/
#ifdef CSR_BT_INSTALL_AVRCP_13_14
L2CA_FLOW_T *CsrBtAvrcpUtilGetFlow(l2ca_mtu_t mtu)
{
    L2CA_FLOW_T *flow = pnew(L2CA_FLOW_T);

    flow->mode              = L2CA_FLOW_MODE_ENHANCED_RETRANS;
    flow->tx_window         = 1;
    flow->max_retransmit    = 0; /* Maximum as specified by spec: infinite */
    flow->retrans_timeout   = CSR_BT_AVRCP_QOS_RETRANSMIT_TIMEOUT;
    flow->monitor_timeout   = CSR_BT_AVRCP_QOS_MONITOR_TIMEOUT;
    flow->maximum_pdu       = mtu;
    return flow;
}
#endif

void CsrBtAvrcpUtilConnect(AvrcpConnInstance_t *connInst)
{
#ifdef CSR_BT_INSTALL_AVRCP_13_14
    if ((connInst->control.state == AVRCP_STATE_CONN_CONNECTED) &&
        ((connInst->browsing.state == AVRCP_STATE_CONN_DISCONNECTED) ||
         (connInst->browsing.state == AVRCP_STATE_CONN_DISC2RECONNECT)))
    {/* Connect browsing */
        CsrBtCml2caFecConnectReqSendExt(CSR_BT_AVRCP_IFACEQUEUE,
                             connInst->address,
                             CSR_BT_AVCTP_BROWSING_PSM,
                             CSR_BT_AVCTP_BROWSING_PSM,
                             connInst->instData->mtu,
                             L2CA_FLUSH_TO_INFINITE,                    /* Flush timeout */
                             NULL,                                      /* QoS */
                             CsrBtAvrcpUtilGetFlow(connInst->instData->mtu), /* Flow */
                             FALSE,                                     /* Fallback basic-mode */
                             connInst->instData->secOutgoingBrow);

        AVRCP_CHANGE_STATE_INDEX(connInst->browsing.state, AVRCP_STATE_CONN_CONNECTING, connInst->appConnId);
    }
    else if ((connInst->control.state == AVRCP_STATE_CONN_DISCONNECTED) ||
             (connInst->control.state == AVRCP_STATE_CONN_PENDING) ||
             (connInst->control.state == AVRCP_STATE_CONN_DISC2RECONNECT))
#else        
    if ((connInst->control.state == AVRCP_STATE_CONN_DISCONNECTED) ||
             (connInst->control.state == AVRCP_STATE_CONN_PENDING) ||
             (connInst->control.state == AVRCP_STATE_CONN_DISC2RECONNECT))
#endif
    {/* When making outgoing connection, always try basic mode for the control channel. Done to
        avoid IOP problems with existing implementations of AVRCP 1.4 and implementations of older
        versions of AVRCP. */
        CsrBtCml2caConnectReqSend(CSR_BT_AVRCP_IFACEQUEUE,
                             connInst->address,
                             CSR_BT_AVCTP_PSM,
                             CSR_BT_AVCTP_PSM,
                             connInst->instData->mtu,
                             connInst->instData->secOutgoingCont);

        AVRCP_CHANGE_STATE_INDEX(connInst->control.state, AVRCP_STATE_CONN_CONNECTING, connInst->appConnId);
    }
}

void CsrBtAvrcpUtilDisconnect(AvrcpConnInstance_t *connInst)
{
    CsrBtAvrcpConnDetails *connDetails = &connInst->browsing; /* Browsing must be disconnected first */

    while (connDetails)
    {
        switch (connDetails->state)
        {
            case AVRCP_STATE_CONN_PENDING:
            {
                AVRCP_CHANGE_STATE_INDEX(connDetails->state, AVRCP_STATE_CONN_DISCONNECTED, connInst->appConnId);
                break;
            }

            case AVRCP_STATE_CONN_CONNECTING:
            {/* Attempt to cancel outgoing connection */
                CsrBtCml2caCancelConnectReqSend(CSR_BT_AVRCP_IFACEQUEUE,
                                                connInst->address,
                                                (psm_t)(connDetails == &connInst->control ? CSR_BT_AVCTP_PSM : CSR_BT_AVCTP_BROWSING_PSM));

                AVRCP_CHANGE_STATE_INDEX(connDetails->state, AVRCP_STATE_CONN_CANCELLING, connInst->appConnId);
                break;
            }

            case AVRCP_STATE_CONN_CONNECTED:
            {/* Connection was already established and should be disconnected */
                CsrBtCml2caDisconnectReqSend(connDetails->btConnId);
                AVRCP_CHANGE_STATE_INDEX(connDetails->state, AVRCP_STATE_CONN_DISCONNECTING, connInst->appConnId);
                break;
            }

            case AVRCP_STATE_CONN_DISCONNECTING:
            {/* Should not occur - ignore */
                break;
            }

            case AVRCP_STATE_CONN_CANCELLING:
            {/* Should not occur - ignore */
                break;
            }

            default:
            {
                break;
            }
        }

        if (connDetails == &connInst->browsing)
        {
            connDetails = &connInst->control;
        }
        else
        {
            break;
        }
    }

    if (connInst->sdpState == AVRCP_STATE_SDP_ACTIVE)
    {/* Active SDP search should be cancelled */
        CsrBtAvrcpSdpSearchCancel(connInst);
    }
}

void CsrBtAvrcpUtilConnectAccept(AvrcpInstanceData_t *instData)
{
    /* Determine if more incoming connections should be accepted */
    if (instData->incomingMaximum > instData->incomingCurrent)
    {
        CsrBool tgSupport = FALSE;

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MODULE
        if (instData->tgLocal.srHandle)
        {
            tgSupport = TRUE;
        }
#endif
        if (instData->activateStateCont == AVRCP_STATE_ACT_DEACTIVATED)
        {/* Incoming connections on control channel should be accepted
            To avoid IOP problems, accept only basic mode for the control channel */
            CsrBtCml2caConnectAcceptReqSend(CSR_BT_AVRCP_IFACEQUEUE,
                                       CSR_BT_AVCTP_PSM,
                                       (CsrUint24)(tgSupport ? CSR_BT_CAPTURING_MAJOR_SERVICE_MASK : 0), /* CoD */
                                       instData->secIncomingCont,
                                       instData->mtu,
                                       (uuid16_t)(tgSupport ? CSR_BT_AV_REMOTE_CONTROL_TARGET_UUID : CSR_BT_AV_REMOTE_CONTROL_UUID)); /* UUID for use in authorise indications */

            AVRCP_CHANGE_STATE(instData->activateStateCont, AVRCP_STATE_ACT_ACTIVATED);
        }

#ifdef CSR_BT_INSTALL_AVRCP_13_14
        /* Only accept connections on browsing channel if AVRCP version 1.4 or higher is supported */
        if ((instData->srAvrcpVersionHighest >= CSR_BT_AVRCP_CONFIG_SR_VERSION_14) &&
            (instData->activateStateBrow == AVRCP_STATE_ACT_DEACTIVATED))
        {/* Incoming connections on browsing channel should be accepted */
            CsrBtCml2caFecConnectAcceptSecondaryReqSendExt(CSR_BT_AVRCP_IFACEQUEUE,
                                                           CSR_BT_AVCTP_BROWSING_PSM,
                                                           (CsrUint24)(tgSupport ? CSR_BT_CAPTURING_MAJOR_SERVICE_MASK : 0), /* CoD */
                                                           instData->secIncomingBrow,
                                                           instData->mtu,
                                                           L2CA_FLUSH_TO_INFINITE,            /* Flush timeout */
                                                           NULL,                              /* QoS */
                                                           CsrBtAvrcpUtilGetFlow(instData->mtu),   /* Flow */
                                                           FALSE,                             /* Fallback basic-mode */
                                                           (uuid16_t)(tgSupport ? CSR_BT_AV_REMOTE_CONTROL_TARGET_UUID : CSR_BT_AV_REMOTE_CONTROL_UUID)); /* UUID for use in authorise indications */

            AVRCP_CHANGE_STATE(instData->activateStateBrow, AVRCP_STATE_ACT_ACTIVATED);
        }
#endif        
    }
}

CsrBool CsrBtAvrcpUtilConnectAcceptCancel(AvrcpInstanceData_t *instData)
{
    CsrBool cancelSent = FALSE;

    if (instData->activateStateCont == AVRCP_STATE_ACT_ACTIVATED)
    {
        AVRCP_CHANGE_STATE(instData->activateStateCont, AVRCP_STATE_ACT_DEACTIVATED);
        CsrBtCml2caCancelConnectAcceptReqSend(CSR_BT_AVRCP_IFACEQUEUE, CSR_BT_AVCTP_PSM);
        cancelSent = TRUE;
    }
#ifdef CSR_BT_INSTALL_AVRCP_13_14
    if (instData->activateStateBrow == AVRCP_STATE_ACT_ACTIVATED)
    {
        AVRCP_CHANGE_STATE(instData->activateStateBrow, AVRCP_STATE_ACT_DEACTIVATED);
        CsrBtCml2caCancelConnectAcceptReqSend(CSR_BT_AVRCP_IFACEQUEUE, CSR_BT_AVCTP_BROWSING_PSM);
        cancelSent = TRUE;
    }
#endif
    return cancelSent;
}

void CsrBtAvrcpLpTimeoutHandler(AvrcpInstanceData_t *inst, AvrcpConnInstance_t *connInst)
{
    CsrBtCml2caModeChangeReqSend(connInst->control.btConnId, inst->lpWantedMode);
#ifdef CSR_BT_INSTALL_AVRCP_13_14
    CsrBtCml2caModeChangeReqSend(connInst->browsing.btConnId, inst->lpWantedMode);
#endif
}

void CsrBtAvrcpLpTimeout(CsrUint16 mi, void *mv)

{
    AvrcpInstanceData_t *inst = (AvrcpInstanceData_t*) mv;
    AvrcpConnInstance_t *connInst = AVRCP_LIST_CONN_GET_C_CID(&inst->connList, ((CsrBtConnId) (CSR_BT_CONN_ID_L2CAP_TECH_MASK | mi)));
    /* Timer was fired */
    inst->lpTimer = 0;
    if ((inst->lpWantedMode != inst->lpMode) && connInst)
    {
        CsrBtAvrcpLpTimeoutHandler(inst, connInst);
    }
}

void CsrBtAvrcpUtilNewConnEstablished(AvrcpInstanceData_t *instData,
                                      AvrcpConnInstance_t *connInst,
                                      CsrBool incoming)
{
    if (incoming)
    {
        instData->incomingCurrent++;
        connInst->connDirection = AVRCP_CONN_DIR_INCOMING;
        CsrBtAvrcpConnectIndSend(instData->ctrlHandle, &connInst->address,
                                 connInst->control.mtu, connInst->appConnId,
                                 connInst->control.btConnId);

        /* Start a SDP search */
        CsrBtAvrcpSdpSearchStart(instData, connInst);
    }
    else
    {
        connInst->connDirection = AVRCP_CONN_DIR_OUTGOING;
        CsrBtAvrcpConnectCfmSend(instData->ctrlHandle,
                                 &connInst->address,
                                 connInst->control.mtu,
                                 connInst->appConnId,
                                 instData->tgDetails,
                                 instData->ctDetails,
                                 CSR_BT_RESULT_CODE_AVRCP_SUCCESS,
                                 CSR_BT_SUPPLIER_AVRCP,
                                 connInst->control.btConnId);
    }

    connInst->control.dataSendAllowed   = TRUE;
    instData->sniffReqResendAllowed     = TRUE;
    instData->lpMode                    = CSR_BT_LINK_STATUS_CONNECTED;
    instData->lpTimer                   = CsrSchedTimerSet(CSR_BT_AVRCP_LP_TIME, CsrBtAvrcpLpTimeout, (CsrUint16)connInst->control.btConnId,(void *)instData);
}

CsrBool CsrBtAvrcpUtilCancelSavedMessage(AvrcpInstanceData_t *instData, CsrBtAvrcpPrim primType, CsrBtDeviceAddr *addr)
{
    CsrUint16                eventClass;
    void                    *msg;
    CsrBool                  cancelledMsg    = FALSE;
    CsrMessageQueueType    *tempQueue      = NULL;

    while(CsrMessageQueuePop(&instData->saveQueue, &eventClass, &msg))
    {
        if (!cancelledMsg && (CSR_BT_AVRCP_PRIM == eventClass) && (primType == (*((CsrBtCmPrim *) msg))))
        {
            switch (primType)
            {
                case CSR_BT_AVRCP_CONNECT_REQ:
                {
                    CsrBtAvrcpConnectReq * prim = (CsrBtAvrcpConnectReq *) msg;

                    if (CsrBtBdAddrEq(&prim->deviceAddr, addr))
                    {
                        cancelledMsg = TRUE;
                        CsrPmemFree(msg);
                    }
                    else
                    {
                        CsrMessageQueuePush(&tempQueue, eventClass, msg);
                    }
                    break;
                }
                default:
                {
                    CsrMessageQueuePush(&tempQueue, eventClass, msg);
                    break;
                }
            }
        }
        else
        {
            CsrMessageQueuePush(&tempQueue, eventClass, msg);
        }
    }
    instData->saveQueue = tempQueue;
    return (cancelledMsg);
}

void CsrBtAvrcpUtilGo2Busy(AvrcpInstanceData_t *instData, CsrBtAvrcpPrim primType)
{
    AVRCP_CHANGE_STATE(instData->appState, AVRCP_STATE_APP_BUSY);
    instData->pendingCtrlPrim = primType;
}

void CsrBtAvrcpUtilGo2Idle(AvrcpInstanceData_t *instData)
{
    AVRCP_CHANGE_STATE(instData->appState, AVRCP_STATE_APP_IDLE);

    if (instData->saveQueue)
    {
        instData->restoreFlag = TRUE;
        avrcpSendHouseCleaning(instData);
    }
    CsrPmemFree(instData->tgDetails);
    CsrPmemFree(instData->ctDetails);
    instData->tgDetails = NULL;
    instData->ctDetails = NULL;

    instData->pendingCtrlPrim = CSR_BT_AVRCP_HOUSE_CLEANING;
}

void CsrBtAvrcpUtilSaveMessage(AvrcpInstanceData_t *instData)
{
    CsrMessageQueuePush(&instData->saveQueue, CSR_BT_AVRCP_PRIM, instData->recvMsgP);
    instData->recvMsgP = NULL;
}

AvrcpConnInstance_t *CsrBtAvrcpUtilGetConnFromL2caCid(AvrcpInstanceData_t *instData, CsrBtConnId btConnId, CsrBtAvrcpConnDetails **connDetails)
{/* Return the correct connInst and connDetails from L2CAP btConnId */
    AvrcpConnInstance_t *connInst = AVRCP_LIST_CONN_GET_C_CID(&instData->connList, btConnId);

    if (connInst)
    {
        *connDetails = &connInst->control;
    }
    else
    {
        connInst = AVRCP_LIST_CONN_GET_B_CID(&instData->connList, btConnId);

        if (connInst)
        {
            *connDetails = &connInst->browsing;
        }
        else
        {
            *connDetails = NULL;
        }
    }

    return connInst;
}

AvrcpConnInstance_t *CsrBtAvrcpUtilConnAdd(AvrcpInstanceData_t *instData, CsrBtDeviceAddr *addr)
{
    AvrcpConnInstance_t *connInst = AVRCP_LIST_CONN_ADD_FIRST(&instData->connList);

    /* Misc */
    connInst->instData                  = instData;
    connInst->appConnId                 = 0xFF; /* To avoid UMR in next step */
    connInst->appConnId                 = csrBtAvrcpUtilGetAppConnId(instData); /* Assign a unique connection ID */
    connInst->connDirection             = AVRCP_CONN_DIR_INVALID;
    connInst->reconnectTid              = 0;
    connInst->address                   = *addr;
    connInst->remoteFeatures            = 0;
    AVRCP_CHANGE_STATE_INDEX(connInst->sdpState, AVRCP_STATE_SDP_PENDING, connInst->appConnId);

    /* Control specific */
    connInst->control.btConnId       = AVRCP_CID_INVALID;
    connInst->control.mtu               = AVRCP_MTU_INVALID;
    connInst->control.ctTLabel          = 0;
    connInst->control.dataSendAllowed   = FALSE;
    connInst->pendingRxDataBufferLen    = 0;
    connInst->pendingRxDataBuffer       = NULL;
    CsrCmnListInit(&connInst->control.pendingTxDataList, 0, NULL, CsrBtAvrcpUtilPendingDataRemoveCB);
    AVRCP_CHANGE_STATE_INDEX(connInst->control.state, AVRCP_STATE_CONN_DISCONNECTED, connInst->appConnId);

#ifdef CSR_BT_INSTALL_AVRCP_13_14
    /* Browsing specific */
    connInst->browsing.btConnId      = AVRCP_CID_INVALID;
    connInst->browsing.mtu              = AVRCP_MTU_INVALID;
    connInst->browsing.ctTLabel         = 0;
    connInst->browsing.dataSendAllowed  = FALSE;
    CsrCmnListInit(&connInst->browsing.pendingTxDataList, 0, NULL, CsrBtAvrcpUtilPendingDataRemoveCB);
    AVRCP_CHANGE_STATE_INDEX(connInst->browsing.state, AVRCP_STATE_CONN_DISCONNECTED, connInst->appConnId);
#endif
    /* Target specific */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MODULE
    CsrBtAvrcpTgUtilInitConnLocal(instData, &connInst->tgLocal);
#endif

    /* Controller specific */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_MODULE
    CsrBtAvrcpCtUtilInitConnLocal(&connInst->ctLocal);
#endif
    return connInst;
}

void CsrBtAvrcpUtilConnRemoveCB(CsrCmnListElm_t *elem)
{
    AvrcpConnInstance_t *connInst = (AvrcpConnInstance_t *)elem;

    CsrCmnListDeinit(&connInst->control.pendingTxDataList);
#ifdef CSR_BT_INSTALL_AVRCP_13_14
    CsrCmnListDeinit(&connInst->browsing.pendingTxDataList);
#endif
    CsrPmemFree(connInst->pendingRxDataBuffer);
    CsrSchedTimerCancel(connInst->reconnectTid, NULL, NULL);

    if (connInst->connDirection == AVRCP_CONN_DIR_INCOMING)
    {
        connInst->instData->incomingCurrent--;
    }

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_MODULE
    CsrCmnListDeinit(&connInst->ctLocal.pendingMsgList);
#endif

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MODULE
    CsrCmnListDeinit(&connInst->tgLocal.pendingMsgList);
#endif
}

void CsrBtAvrcpUtilPendingDataAddLast(CsrBtAvrcpConnDetails *connDetails, CsrUint16 dataLen, CsrUint8 *data)
{
    AvrcpPendingData_t *pendingData = AVRCP_LIST_TXDATA_ADD_LAST(&connDetails->pendingTxDataList);
    pendingData->dataLen = dataLen;
    pendingData->data    = data;
}

void CsrBtAvrcpUtilPendingDataRemoveCB(CsrCmnListElm_t *elem)
{
    CsrPmemFree(((AvrcpPendingData_t *)elem)->data);
}

void CsrBtAvrcpUtilPendingDataSend(AvrcpConnInstance_t *connInst, CsrBtConnId connId)
{
    AvrcpPendingData_t *pendingData;
    CsrBtAvrcpConnDetails *connDetails = &connInst->control;

    if (connDetails)
    {
        if (connDetails->btConnId != connId)
        {/* Make sure that the correct connection is used */
            connDetails = &connInst->browsing;
        }
    
        if (!connDetails->dataSendAllowed && (connDetails->state == AVRCP_STATE_CONN_CONNECTED))
        {
            pendingData = AVRCP_LIST_TXDATA_GET_FIRST(&connDetails->pendingTxDataList);

            if (pendingData)
            {
                CsrBtCml2caDataReqSend(connDetails->btConnId, pendingData->dataLen, pendingData->data);
                pendingData->data = NULL;
                AVRCP_LIST_TXDATA_REMOVE(&connDetails->pendingTxDataList, pendingData);
            }
            else
            {/* No more pending messages: allow app data to be sent immediately in the future */
                connDetails->dataSendAllowed = TRUE;
            }
        }
    }
}

void CsrBtAvrcpUtilFreeConfigReq(CsrBtAvrcpConfigReq **prim)
{
    CsrBtAvrcpUtilFreeRoleDetails(&((*prim)->tgDetails));
    CsrBtAvrcpUtilFreeRoleDetails(&((*prim)->ctDetails));
    CsrPmemFree(*prim);
    *prim = NULL;
}


#endif

