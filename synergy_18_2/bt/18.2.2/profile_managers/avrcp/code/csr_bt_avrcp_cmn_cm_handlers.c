/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_AVRCP_MODULE

#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_avrcp_main.h"
#include "csr_bt_avrcp_prim.h"

void CsrBtAvrcpCmL2caRegisterCfmHandler(AvrcpInstanceData_t *instData)
{
#ifdef CSR_BT_INSTALL_AVRCP_13_14
    CsrBtCmL2caRegisterCfm *prim = (CsrBtCmL2caRegisterCfm *)(instData->recvMsgP);

    if (prim->localPsm == CSR_BT_AVCTP_BROWSING_PSM)
#endif
    {/* Then we assume the the AVCTP PSM has alredy been registered and this is the last one: go to idle */
        CsrBtAvrcpUtilGo2Idle(instData);
    }
}

/***** CM_SDS_X handling *****/
void CsrBtAvrcpCmSdsRegisterCfmHandler(AvrcpInstanceData_t *instData)
{
    CsrBtCmSdsRegisterCfm *prim = (CsrBtCmSdsRegisterCfm *) (instData->recvMsgP);

    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS &&
        prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        /* Update correct service record handle */
        if (AVRCP_ROLE_CONTROLLER == instData->srActiveRole)
        {
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_MODULE
            instData->ctLocal.srHandle = prim->serviceRecHandle;
#endif
        }
        else if (AVRCP_ROLE_TARGET == instData->srActiveRole)
        {
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MODULE
            instData->tgLocal.srHandle = prim->serviceRecHandle;
#endif
        }

        if (!CsrBtAvrcpSdpRegisterSR(instData))
        {/* No more records to register */
            if (instData->srActiveRole != AVRCP_ROLE_INVALID)
            {
                CsrBtAvrcpConfigCfmSend(instData->ctrlHandle,
                    CSR_BT_RESULT_CODE_AVRCP_SUCCESS,
                    CSR_BT_SUPPLIER_AVRCP);
                AVRCP_CHANGE_STATE(instData->srActiveRole, AVRCP_ROLE_INVALID);
                CsrBtAvrcpUtilGo2Idle(instData);
            }
        }
    }
    else
    {/* Registration failed -- propagate error */
        CsrBtAvrcpUtilFreeConfigReq(&instData->srPending);
        CsrBtAvrcpConfigCfmSend(instData->ctrlHandle,
            prim->resultCode, prim->resultSupplier);

        CsrBtAvrcpUtilGo2Idle(instData);
    }
}

void CsrBtAvrcpCmSdsUnregisterCfmHandler(AvrcpInstanceData_t *instData)
{
    CsrBtCmSdsUnregisterCfm *prim = (CsrBtCmSdsUnregisterCfm *) (instData->recvMsgP);
    CsrBool                  unregComplete = FALSE;

    /* Determine if both records are unregistered and update handles */
    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS &&
        prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MODULE
        if (prim->serviceRecHandle == instData->tgLocal.srHandle)
        {
            instData->tgLocal.srHandle = AVRCP_SDP_INVALID_SR_HANDLE;
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_MODULE
            if (instData->ctLocal.srHandle == AVRCP_SDP_INVALID_SR_HANDLE)
#endif
            {/* The controller record is already unregistered */
                unregComplete = TRUE;
            }
        }
#endif
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_MODULE
        if (prim->serviceRecHandle == instData->ctLocal.srHandle)
        {
            instData->ctLocal.srHandle = AVRCP_SDP_INVALID_SR_HANDLE;
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MODULE
            if (instData->tgLocal.srHandle == AVRCP_SDP_INVALID_SR_HANDLE)
#endif
            {
                unregComplete = TRUE;
            }
        }
#endif
        if (unregComplete)
        {/* Determine if new records should be registered */
            if((instData->srPending) && (!instData->srPending->tgDetails.roleSupported) && (!instData->srPending->ctDetails.roleSupported))
            {
                    CsrBtAvrcpUtilGo2Idle(instData);
                    CsrBtAvrcpUtilFreeConfigReq(&instData->srPending);
                    CsrBtAvrcpConfigCfmSend(instData->ctrlHandle,
                        CSR_BT_RESULT_CODE_AVRCP_SUCCESS,
                        CSR_BT_SUPPLIER_AVRCP);
                    AVRCP_CHANGE_STATE(instData->srActiveRole, AVRCP_ROLE_INVALID);
            }
            else if(!CsrBtAvrcpSdpRegisterSR(instData))
            {
                if(instData->srActiveRole != AVRCP_ROLE_INVALID)
                {
                    CsrBtAvrcpUtilGo2Idle(instData);
                    CsrBtAvrcpConfigCfmSend(instData->ctrlHandle,
                        CSR_BT_RESULT_CODE_AVRCP_SUCCESS,
                        CSR_BT_SUPPLIER_AVRCP);
                    AVRCP_CHANGE_STATE(instData->srActiveRole, AVRCP_ROLE_INVALID);
                }
            }
        }
    }
    else
    {
        CsrBtAvrcpUtilFreeConfigReq(&instData->srPending);
        CsrBtAvrcpUtilGo2Idle(instData);
        CsrBtAvrcpConfigCfmSend(instData->ctrlHandle,
            prim->resultCode, prim->resultSupplier);
    }
}

/* Incoming connection established */
void CsrBtAvrcpCmL2caConnectAcceptCfmHandler(AvrcpInstanceData_t *instData)
{
    CsrBtCmL2caConnectAcceptCfm *cmPrim = (CsrBtCmL2caConnectAcceptCfm *)instData->recvMsgP;

    /* A device connected successfully - determine if it is known */
    if (cmPrim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS &&
        cmPrim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        AvrcpConnInstance_t *connInst;
        connInst = AVRCP_LIST_CONN_GET_ADDR(&instData->connList, &cmPrim->deviceAddr);

        if (connInst)
        {/* Device is known */
            if (CSR_BT_AVCTP_PSM == cmPrim->localPsm)
            {/* A control channel was established */
                CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_CONTROL_CHANNEL, cmPrim->deviceAddr,cmPrim->btConnId);
                if ((AVRCP_STATE_CONN_CONNECTING == connInst->control.state) &&
                    (instData->incomingMaximum > instData->incomingCurrent))
                {
                    /* The connection was established from a device in the process of being connected to -
                    attempt to cancel outgoing connection */
                    CsrBtCml2caCancelConnectReqSend(CSR_BT_AVRCP_IFACEQUEUE, connInst->address, CSR_BT_AVCTP_PSM);

                    /* initialize and report this connection to the app */
                    connInst->control.btConnId = cmPrim->btConnId;
                    connInst->control.mtu = cmPrim->mtu;
                    connInst->connDirection = AVRCP_CONN_DIR_INCOMING;
                    instData->incomingCurrent++;
                    connInst->control.dataSendAllowed   = TRUE;
                    instData->sniffReqResendAllowed     = TRUE;
                    instData->lpMode                    = CSR_BT_LINK_STATUS_CONNECTED;
                    instData->lpTimer                   = CsrSchedTimerSet( CSR_BT_AVRCP_LP_TIME,
                                                                            CsrBtAvrcpLpTimeout,
                                                                            (CsrUint16)connInst->control.btConnId,
                                                                            (void *)instData);

                    CsrBtAvrcpConnectCfmSend(instData->ctrlHandle,
                                             &connInst->address,
                                             connInst->control.mtu,
                                             connInst->appConnId,
                                             instData->tgDetails,
                                             instData->ctDetails,
                                             CSR_BT_RESULT_CODE_AVRCP_SUCCESS,
                                             CSR_BT_SUPPLIER_AVRCP,
                                             connInst->control.btConnId);
                    
                    AVRCP_CHANGE_STATE_INDEX(connInst->control.state, AVRCP_STATE_CONN_CONNECTED, connInst->appConnId);
                }
                else
                {/* A connection is already present to the device - disconnect new connection */
                    CsrBtCml2caDisconnectReqSend(cmPrim->btConnId); /* Crossing connections */
                }
            }
#ifdef CSR_BT_INSTALL_AVRCP_13_14
            else
            {/* The browsing channel was established */
                CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_DATA_CHANNEL, cmPrim->deviceAddr,cmPrim->btConnId);
                if (connInst->control.state == AVRCP_STATE_CONN_CONNECTED)
                {
                    AvrcpPendingData_t *pendingData = AVRCP_LIST_TXDATA_GET_FIRST(&connInst->browsing.pendingTxDataList);

                    if (connInst->browsing.state == AVRCP_STATE_CONN_CONNECTED)
                    {  /* Crossing browsing connections */
                        CsrBtCml2caDisconnectReqSend(cmPrim->btConnId);
                        CsrBtCml2caDisconnectReqSend(connInst->browsing.btConnId);

                        if (pendingData)
                        { /* The application has requested the local device to send a browsing
                             command. Set browsing state to AVRCP_STATE_CONN_DISC2RECONNECT 
                             in order to reconnect */
                            AVRCP_CHANGE_STATE_INDEX(connInst->browsing.state, AVRCP_STATE_CONN_DISC2RECONNECT, connInst->appConnId);
                        }
                        else
                        { /* The local device do not need to send any browsing command. E.g
                             let the peer device re-establish the connection or wait to 
                             re-establish until it is needed */
                            ;
                        }
                    }
                    else
                    {
                        connInst->browsing.btConnId         = cmPrim->btConnId;
                        connInst->browsing.mtu              = cmPrim->mtu;
                        AVRCP_CHANGE_STATE_INDEX(connInst->browsing.state, AVRCP_STATE_CONN_CONNECTED, connInst->appConnId);

                        if (pendingData)
                        {/* An incoming browsing channel must be crossing with an outgoing one. The outgoing
                            one were being established due to a pending command - send data */
                            CsrBtCml2caDataReqSend(cmPrim->btConnId, pendingData->dataLen, pendingData->data);
                            pendingData->data = NULL;
                            AVRCP_LIST_TXDATA_REMOVE(&connInst->browsing.pendingTxDataList, pendingData);
                        }
                        else
                        {
                            connInst->browsing.dataSendAllowed  = TRUE;
                        }
                    }
                }
                else
                {/* A browsing channel was established when no control channel is present*/
                    CsrBtCml2caDisconnectReqSend(cmPrim->btConnId);
                }
            }
#endif            
        }
        else
        {/* Device is unknown */
            if ((instData->incomingMaximum > instData->incomingCurrent) && (cmPrim->localPsm == CSR_BT_AVCTP_PSM))
            {/* New incoming connections are allowed - only accept control as first connection */
                CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_CONTROL_CHANNEL, cmPrim->deviceAddr,cmPrim->btConnId);
                connInst = CsrBtAvrcpUtilConnAdd(instData, &cmPrim->deviceAddr);
                AVRCP_CHANGE_STATE_INDEX(connInst->control.state, AVRCP_STATE_CONN_CONNECTED, connInst->appConnId);
                connInst->control.btConnId = cmPrim->btConnId;
                connInst->control.mtu = cmPrim->mtu;
                CsrBtAvrcpUtilNewConnEstablished(instData, connInst, TRUE);
            }
            else
            {/* Incoming connections are not being accepted or this is a browsing channel */
                CsrBtCml2caDisconnectReqSend(cmPrim->btConnId);
            }
        }
    }
    else
    {/* Error - ignore*/
    }

    /* Update the correct states */
    if (cmPrim->localPsm == CSR_BT_AVCTP_PSM)
    {
        AVRCP_CHANGE_STATE(instData->activateStateCont, AVRCP_STATE_ACT_DEACTIVATED);
    }
#ifdef CSR_BT_INSTALL_AVRCP_13_14
    else if (cmPrim->localPsm == CSR_BT_AVCTP_BROWSING_PSM)
    {
        AVRCP_CHANGE_STATE(instData->activateStateBrow, AVRCP_STATE_ACT_DEACTIVATED);
    }
#endif
    /* Determine if more incoming connections should be accepted */
    CsrBtAvrcpUtilConnectAccept(instData);
}

/* Incoming connections rejected */
void CsrBtAvrcpCmL2caCancelConnectAcceptCfmHandler(AvrcpInstanceData_t *instData)
{
    if (instData->pendingCtrlPrim == CSR_BT_AVRCP_DEACTIVATE_REQ)
    {/* ConnectAccept was cancelled due to deactivation */
        CsrBtCmL2caCancelConnectAcceptCfm *cmPrim = (CsrBtCmL2caCancelConnectAcceptCfm *)instData->recvMsgP;

        if (cmPrim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS &&
            cmPrim->resultSupplier == CSR_BT_SUPPLIER_CM)
        {
            CsrBtAvrcpDeactivateCfmSend(instData->ctrlHandle,
                CSR_BT_RESULT_CODE_AVRCP_SUCCESS,
                CSR_BT_SUPPLIER_AVRCP);
        }
        else
        {
            CsrBtAvrcpDeactivateCfmSend(instData->ctrlHandle, cmPrim->resultCode, cmPrim->resultSupplier);
        }
        CsrBtAvrcpUtilGo2Idle(instData);
    }
}

/* Outgoing connection established */
void CsrBtAvrcpCmL2caConnectCfmHandler(AvrcpInstanceData_t *instData)
{
    CsrBtCmL2caConnectCfm *cmPrim = (CsrBtCmL2caConnectCfm*)instData->recvMsgP;
    AvrcpConnInstance_t *connInst = AVRCP_LIST_CONN_GET_ADDR(&instData->connList, &cmPrim->deviceAddr);

    if (connInst)
    {/* Connection instance is known */
        switch (connInst->control.state)
        {
            case AVRCP_STATE_CONN_CONNECTING:
            {/* Correct connection identified */
                if (cmPrim->resultSupplier == CSR_BT_SUPPLIER_CM &&
                    cmPrim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS &&
                    cmPrim->localPsm == CSR_BT_AVCTP_PSM)
                {/* New outgoing control connection established by request from app */
                    connInst->control.btConnId = cmPrim->btConnId;
                    connInst->control.mtu = cmPrim->mtu;

                    connInst->control.dataSendAllowed   = TRUE;

                    AVRCP_CHANGE_STATE_INDEX(connInst->control.state, AVRCP_STATE_CONN_CONNECTED, connInst->appConnId);

                    CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_CONTROL_CHANNEL, cmPrim->deviceAddr,cmPrim->btConnId);
                    
#ifdef CSR_BT_INSTALL_AVRCP_13_14
                    if (!CSR_MASK_IS_SET(connInst->remoteFeatures, CSR_BT_AVRCP_CONFIG_SR_FEAT_BROWSING))
#endif
                    {/* Browsing is not supported - outgoing connection is complete */
                        CsrBtAvrcpUtilNewConnEstablished(instData, connInst, FALSE);
                        CsrBtAvrcpUtilGo2Idle(instData);
                    }
#ifdef CSR_BT_INSTALL_AVRCP_13_14
                    else
                    {/* Establish the browsing channel */
                        CsrBtAvrcpUtilConnect(connInst);
                    }
#endif
                }
                else
                {/* Connection failed - report error */
                    CsrBtAvrcpConnectCfmSend(instData->ctrlHandle,
                                             &cmPrim->deviceAddr,
                                             AVRCP_MTU_INVALID,
                                             CSR_BT_AVRCP_CONNECTION_ID_INVALID,
                                             NULL,
                                             NULL,
                                             cmPrim->resultCode,
                                             cmPrim->resultSupplier,
                                             CSR_BT_CONN_ID_INVALID);

                    AVRCP_LIST_CONN_REMOVE(&instData->connList, connInst);
                    CsrBtAvrcpUtilGo2Idle(instData);
                }
                break;
            }

            case AVRCP_STATE_CONN_CANCELLING:
            {/* Connection is in the process of being cancelled */
                if (cmPrim->resultSupplier == CSR_BT_SUPPLIER_CM &&
                    cmPrim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS)
                {/* Cancel did not succeed - disconnect */
                    connInst->control.btConnId = cmPrim->btConnId;
                    CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_CONTROL_CHANNEL, cmPrim->deviceAddr,cmPrim->btConnId);
                    CsrBtCml2caDisconnectReqSend(cmPrim->btConnId);
                }
                else
                {/* Connection was cancelled or failed - just ignore */
                    CsrBtAvrcpConnectCfmSend(instData->ctrlHandle,
                                             &cmPrim->deviceAddr,
                                             AVRCP_MTU_INVALID,
                                             CSR_BT_AVRCP_CONNECTION_ID_INVALID,
                                             NULL,
                                             NULL,
                                             CSR_BT_RESULT_CODE_AVRCP_CONNECT_ATTEMPT_CANCELLED,
                                             CSR_BT_SUPPLIER_AVRCP,
                                             CSR_BT_CONN_ID_INVALID);

                    AVRCP_LIST_CONN_REMOVE(&instData->connList, connInst);
                    CsrBtAvrcpUtilGo2Idle(instData);
                }
                break;
            }

            case AVRCP_STATE_CONN_CONNECTED:
            {/* Control channel is already established */
                if (cmPrim->resultSupplier == CSR_BT_SUPPLIER_CM &&
                    cmPrim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS)
                {
                    if (cmPrim->localPsm == CSR_BT_AVCTP_PSM)
                    {/* Crossing control connections */
                        AVRCP_CHANGE_STATE_INDEX(connInst->control.state, AVRCP_STATE_CONN_DISC2RECONNECT, connInst->appConnId);
                        CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_CONTROL_CHANNEL, cmPrim->deviceAddr,cmPrim->btConnId);
                        CsrBtCml2caDisconnectReqSend(cmPrim->btConnId);
                        CsrBtCml2caDisconnectReqSend(connInst->control.btConnId);
                    }
#ifdef CSR_BT_INSTALL_AVRCP_13_14
                    else 
                    { /* The browsing channel has been connected */
                        CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_DATA_CHANNEL, cmPrim->deviceAddr,cmPrim->btConnId);
                        if (connInst->browsing.state == AVRCP_STATE_CONN_CONNECTED)
                        {/* Crossing browsing connections. Release both browsing channels */
                            AVRCP_CHANGE_STATE_INDEX(connInst->browsing.state, AVRCP_STATE_CONN_DISC2RECONNECT, connInst->appConnId);
                            CsrBtCml2caDisconnectReqSend(cmPrim->btConnId);
                            CsrBtCml2caDisconnectReqSend(connInst->browsing.btConnId);
                        }
                        else if (connInst->browsing.state == AVRCP_STATE_CONN_CONNECTING)
                        {
                            AvrcpPendingData_t *pendingData = AVRCP_LIST_TXDATA_GET_FIRST(&connInst->browsing.pendingTxDataList);
                            connInst->browsing.btConnId = cmPrim->btConnId;
                            connInst->browsing.mtu = cmPrim->mtu;
                            AVRCP_CHANGE_STATE_INDEX(connInst->browsing.state, AVRCP_STATE_CONN_CONNECTED, connInst->appConnId);
                            
                            if (pendingData)
                            {/* Browsing channel was established due to a pending command - send data */
                                CsrBtCml2caDataReqSend(cmPrim->btConnId, pendingData->dataLen, pendingData->data);
                                pendingData->data = NULL;
                                AVRCP_LIST_TXDATA_REMOVE(&connInst->browsing.pendingTxDataList, pendingData);
                            }
                            else
                            { /* A browsing channel were establish right after setting up the control channel */
                                connInst->browsing.dataSendAllowed  = TRUE;
                                CsrBtAvrcpUtilNewConnEstablished(instData, connInst, FALSE);
                            }
                        }
                    }
#endif                    
                }
                else
                {
                    if (cmPrim->localPsm == CSR_BT_AVCTP_PSM)
                    { /* Crossing control connections attempts. In this case the outgoing connection 
                         fails, E.g. AVRCP has only one control connection. Keep it */ 
                        ;
                    }
#ifdef CSR_BT_INSTALL_AVRCP_13_14
                    else 
                    { /* Failed to establish a browsing channel */
                        if (connInst->browsing.state == AVRCP_STATE_CONN_CONNECTED)
                        {/* Crossing browsing connections. In this case the outgoing connection 
                         fails, E.g. AVRCP has only one browsing connection. Keep it */
                            ;
                        }
                        else
                        {
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_MODULE
                            if (AVRCP_LIST_TXDATA_GET_FIRST(&connInst->browsing.pendingTxDataList))
                            {/* Browsing channel was attempted established due to a pending command (CT only) - find and complete pending msg */
                                CsrBtAvrcpCtUtilPendingMsgCompleteFromPsm(connInst,
                                                                          CSR_BT_AVCTP_BROWSING_PSM,
                                                                          CSR_BT_RESULT_CODE_AVRCP_CHANNEL_NOT_CONNECTED,
                                                                          CSR_BT_SUPPLIER_AVRCP);
                            }
                            else                        
#endif
                            { /* Browsing channel was attempted established just after control channel were established. 
                                 Send a connect confirm to the application with success, as AVRCP shall not use the 
                                 browsing channel right now, and the application may never use it */
                                 CsrBtAvrcpUtilNewConnEstablished(instData, connInst, FALSE);
                            }
                            AVRCP_CHANGE_STATE_INDEX(connInst->browsing.state, AVRCP_STATE_CONN_DISCONNECTED, connInst->appConnId);
                        }
                    }
#endif                    
                }
                CsrBtAvrcpUtilGo2Idle(instData);
                break;
            }
            default:
            {/* Connection in invalid state - should not occur */
#ifdef CSR_BT_INSTALL_AVRCP_13_14
                if (cmPrim->localPsm == CSR_BT_AVCTP_BROWSING_PSM &&
                    cmPrim->resultSupplier == CSR_BT_SUPPLIER_CM &&
                    cmPrim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS)
                {/* A browsing channel is established before control channel - disconnect */
                    CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_DATA_CHANNEL, cmPrim->deviceAddr,cmPrim->btConnId);
                    CsrBtCml2caDisconnectReqSend(cmPrim->btConnId);
                }
#endif
                break;
            }
        }
    }
    else
    {/* Unknown connection */
        if (cmPrim->resultSupplier == CSR_BT_SUPPLIER_CM &&
            cmPrim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS)
        {/* Make sure the unknown connection is disconnected */
            CsrBtCml2caDisconnectReqSend(cmPrim->btConnId);
        }
        else
        {/* Just ignore */
        }
    }
}

/* Connection disconnected */
void CsrBtAvrcpCmL2caDisconnectIndHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpConnDetails          *connDetails;
    CsrBtCmL2caDisconnectInd    *cmPrim = (CsrBtCmL2caDisconnectInd *) instData->recvMsgP;
    AvrcpConnInstance_t         *connInst = CsrBtAvrcpUtilGetConnFromL2caCid(instData, cmPrim->btConnId, &connDetails);

    if (connInst)
    {
        if (instData->lpTimer != 0)
        {/* Make sure to stop the timer if it is running! */
            CsrSchedTimerCancel(instData->lpTimer, NULL, NULL);
            instData->lpTimer = 0;
        }

        CsrBtCmLogicalChannelTypeReqSend(CSR_BT_NO_ACTIVE_LOGICAL_CHANNEL, connInst->address,cmPrim->btConnId);

        if (((connDetails == &connInst->control) && (connInst->control.state == AVRCP_STATE_CONN_DISC2RECONNECT)) 
#ifdef CSR_BT_INSTALL_AVRCP_13_14
            || ((connDetails == &connInst->browsing) && (connInst->browsing.state == AVRCP_STATE_CONN_DISC2RECONNECT))
#endif
            )
        {
            CsrBtAvrcpUtilConnect(connInst);
        }
        else
        {
            AVRCP_CHANGE_STATE_INDEX(connDetails->state, AVRCP_STATE_CONN_DISCONNECTED, connInst->appConnId);

            if (connInst->control.state == AVRCP_STATE_CONN_DISCONNECTED)
            {/* Control is disconnected - cleanup */
                CsrUint8 conDir = connInst->connDirection;
                
#ifdef CSR_BT_INSTALL_AVRCP_13_14
                if (connInst->browsing.state == AVRCP_STATE_CONN_DISCONNECTED)
                {
#endif                    
                    switch (instData->pendingCtrlPrim)
                    {
                        case CSR_BT_AVRCP_CONNECT_REQ:
                        {
                            CsrBtAvrcpConnectCfmSend(instData->ctrlHandle,
                                                     &connInst->address,
                                                     AVRCP_MTU_INVALID,
                                                     CSR_BT_AVRCP_CONNECTION_ID_INVALID,
                                                     NULL,
                                                     NULL,
                                                     cmPrim->reasonCode,
                                                     cmPrim->reasonSupplier,
                                                     CSR_BT_CONN_ID_INVALID);

                            CsrBtAvrcpUtilGo2Idle(instData);
                            break;
                        }

                        case CSR_BT_AVRCP_CANCEL_CONNECT_REQ:
                        {
                            CsrBtAvrcpConnectCfmSend(instData->ctrlHandle,
                                                     &connInst->address,
                                                     AVRCP_MTU_INVALID,
                                                     CSR_BT_AVRCP_CONNECTION_ID_INVALID,
                                                     NULL,
                                                     NULL,
                                                     CSR_BT_RESULT_CODE_AVRCP_CONNECT_ATTEMPT_CANCELLED,
                                                     CSR_BT_SUPPLIER_AVRCP,
                                                     CSR_BT_CONN_ID_INVALID);

                            CsrBtAvrcpUtilGo2Idle(instData);
                            break;
                        }

                        case CSR_BT_AVRCP_DISCONNECT_REQ:
                        {
                            CsrBtAvrcpDisconnectIndSend(instData->ctrlHandle,
                                connInst->appConnId,
                                cmPrim->reasonCode,
                                cmPrim->reasonSupplier,
                                cmPrim->localTerminated);
                            CsrBtAvrcpUtilGo2Idle(instData);
                            break;
                        }

                        case CSR_BT_AVRCP_DEACTIVATE_REQ: /* Fall-through */
                        case CSR_BT_AVRCP_CONFIG_REQ: /* Fall-through */
                        default:
                        {
                            CsrBtAvrcpDisconnectIndSend(instData->ctrlHandle,
                                connInst->appConnId,
                                cmPrim->reasonCode,
                                cmPrim->reasonSupplier,
                                cmPrim->localTerminated);
                            break;
                        }
                    }
#ifdef CSR_BT_INSTALL_AVRCP_13_14
                }
                else
                {/* Browsing is not disconnected - always disconnect if control is disconnected */
                    CsrBtAvrcpUtilDisconnect(connInst);
                    /* Browsing is disconnected: inform application about release as no further indication will be received about that */
                    CsrBtAvrcpDisconnectIndSend(instData->ctrlHandle,
                        connInst->appConnId,
                        cmPrim->reasonCode,
                        cmPrim->reasonSupplier,
                        cmPrim->localTerminated);
                }
#endif
                if (connInst->sdpState == AVRCP_STATE_SDP_ACTIVE)
                {/* SDP search is in progress and should be cancelled */
                    CsrBtAvrcpSdpSearchCancel(connInst);
                }

                AVRCP_LIST_CONN_REMOVE(&instData->connList, connInst);

                if(conDir == AVRCP_CONN_DIR_INCOMING)
                {
                    CsrBtAvrcpUtilConnectAccept(instData);
                }
            }
#ifdef CSR_BT_INSTALL_AVRCP_13_14
            else
            {
                /* do nothing if in the process of disconnecting or if already havning a pending
                   accept connect on the browse channel */
                if( (connInst->control.state != AVRCP_STATE_CONN_DISCONNECTING) 
                    && (instData->activateStateBrow == AVRCP_STATE_ACT_DEACTIVATED))
                {
                    CsrBool tgSupport = FALSE;

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MODULE
                    if (instData->tgLocal.srHandle)
                    {
                        tgSupport = TRUE;
                    }
#endif 
              
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
            }
#endif            
        }
    }
}

/* Mode change occurred */
void CsrBtAvrcpCmModeChangeIndHandler(AvrcpInstanceData_t *instData)
{
    CsrBtCmL2caModeChangeInd *cmPrim = (CsrBtCmL2caModeChangeInd *) instData->recvMsgP;
    AvrcpConnInstance_t *connInst = AVRCP_LIST_CONN_GET_C_CID(&instData->connList, cmPrim->btConnId);

    /* First find the relevant connection and find the control channel's btConnId. */
    if (connInst == NULL)
    {/* This is a browsing channel's btConnId; but we need the control btConnId... */
        connInst = AVRCP_LIST_CONN_GET_B_CID(&instData->connList, cmPrim->btConnId);
    }
    /* At this point 'connInst' will always have a meaningful value */

    /* If the LP timer is running, make sure to stop it */
    if (instData->lpTimer)
    {
        CsrSchedTimerCancel(instData->lpTimer, NULL, NULL);
        instData->lpTimer = 0;
    }

    /* Then decide what to do */
    /* we do not want PARK mode, go to SNIFF or ACTIVE mode */
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
    if (cmPrim->mode == CSR_BT_PARK_MODE)
    {
        instData->sniffReqResendAllowed = FALSE;
        CsrBtCml2caModeChangeReqSend(cmPrim->btConnId, instData->lpWantedMode);
    }
    else
#endif
    {
        instData->lpMode = cmPrim->mode;
        if ((instData->lpMode != instData->lpWantedMode) && (connInst != NULL))
        {
            /* Start timer and try to change lpMode when it times out */
            instData->lpTimer  = CsrSchedTimerSet(CSR_BT_AVRCP_LP_TIME, CsrBtAvrcpLpTimeout, (CsrUint16)connInst->control.btConnId,(void *)instData);
        }
    }
}

void CsrBtAvrcpCmL2caDataIndHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpConnDetails  *connDetails;
    CsrBtCmL2caDataInd  *cmPrim = (CsrBtCmL2caDataInd *) instData->recvMsgP;
    AvrcpConnInstance_t *connInst = CsrBtAvrcpUtilGetConnFromL2caCid(instData, cmPrim->btConnId, &connDetails);

    CsrBtCmL2caDataResSend(cmPrim->btConnId);

    if (connInst && (connDetails->state == AVRCP_STATE_CONN_CONNECTED))
    {/* Connection is valid */
        CsrSchedTimerCancel(instData->lpTimer,NULL,NULL);
        instData->lpTimer  = CsrSchedTimerSet(CSR_BT_AVRCP_LP_TIME, CsrBtAvrcpLpTimeout, (CsrUint16)connInst->control.btConnId,(void *)instData);

        if (CsrBtAvrcpUtilDataCheckAvctp(cmPrim->length, cmPrim->payload))
        {/* AVCTP header is valid */
            if (CsrBtAvrcpUtilDataFragRxHandle(connInst, &cmPrim->length, &cmPrim->payload))
            {/* AVCTP packet is not fragmented or a complete frame has been assembled */
                switch (AVRCP_DATA_AVCTP_CR_GET(cmPrim->payload))
                {
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_MODULE
                    case AVRCP_DATA_AVCTP_CR_RES:
                    {/* Response */
                        if (connDetails == &connInst->control)
                        {/* AV/C response on control channel */
                            CsrBtAvrcpCtRxControlHandler(connInst, cmPrim->length, &cmPrim->payload);
                        }
#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
                        else if (connDetails == &connInst->browsing)
                        {/* Response on browsing channel */
                            CsrBtAvrcpCtRxBrowsingHandler(connInst, cmPrim->length, &cmPrim->payload);
                        }
#endif
                        break;
                    }
#endif
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MODULE
                    case AVRCP_DATA_AVCTP_CR_CMD:
                    {/* Command */
                        if (connDetails == &connInst->control)
                        {/* AV/C command on control channel */
                            CsrBtAvrcpTgRxControlHandler(connInst, cmPrim->length, &cmPrim->payload);
                        }
#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
                        else if (connDetails == &connInst->browsing)
                        {/* Command on browsing channel */
                            CsrBtAvrcpTgRxBrowsingHandler(connInst, cmPrim->length, &cmPrim->payload);
                        }
#endif                        
                        break;
                    }
#endif

                    default:
                    {/* Can not occur - C/R field is only one bit */
                        break;
                    }
                }
            }
        }
        else
        {/* AVCTP packet is invalid */
            CsrUint8 *txData = CsrPmemAlloc(AVRCP_DATA_AVCTP_SINGLE_MI_INDEX);
            CsrUint8 tLabel = AVRCP_TLABEL_GET(cmPrim->payload);

            /* Send response with invalid PID */
            /* Invalid PID + command/response info */
            txData[0]  = AVRCP_DATA_AVCTP_IPID_MASK | ((AVRCP_DATA_AVCTP_CR_RES & AVRCP_DATA_AVCTP_CR_MASK) << AVRCP_DATA_AVCTP_CR_SHIFT);
            /* Packet type */
            txData[0] |= (AVRCP_DATA_AVCTP_PACKET_TYPE_SINGLE & AVRCP_DATA_AVCTP_PACKET_TYPE_MASK) << AVRCP_DATA_AVCTP_PACKET_TYPE_SHIFT;
            /* Transaction label as received from remote */
            txData[0] |= (tLabel & AVRCP_DATA_AVCTP_TLABEL_MASK) << AVRCP_DATA_AVCTP_TLABEL_SHIFT;
            /* Incorrect profile id (UUID) as received from remote */
            CsrMemCpy(&txData[AVRCP_DATA_AVCTP_SINGLE_PID_INDEX],&cmPrim->payload[AVRCP_DATA_AVCTP_SINGLE_PID_INDEX], sizeof(CsrUint16));

            CsrBtAvrcpControlDataSend(connInst, AVRCP_DATA_AVCTP_SINGLE_MI_INDEX, txData);
        }
    }

    CsrPmemFree(cmPrim->payload);

}

void CsrBtAvrcpCmL2caDataCfmHandler(AvrcpInstanceData_t *instData)
{
    CsrBtCmL2caDataCfm    *cmPrim = (CsrBtCmL2caDataCfm *) instData->recvMsgP;
    
    
    if (cmPrim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS)
    {/* Data request successful; check if there are any pending messages to send */
        CsrBtAvrcpConnDetails    *connDetails;
        AvrcpConnInstance_t   *connInst = CsrBtAvrcpUtilGetConnFromL2caCid(instData, cmPrim->btConnId, &connDetails);

        if (connInst)
        {/* Device is known and connected */
            CsrBtAvrcpUtilPendingDataSend(connInst, cmPrim->btConnId);
        }
    }
    /* else what to do???? */
}

#endif

