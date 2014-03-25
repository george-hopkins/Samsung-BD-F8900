/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #4 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_env_prim.h"
#include "csr_bt_result.h"
#include "bluetooth.h"
#include "csr_bt_cm_prim.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_util.h"
#include "csr_bt_av_main.h"
#include "csr_bt_sdc_support.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvSecurityInHandler
 *      CsrBtAvSecurityOutHandler
 *
 *  DESCRIPTION
 *      Set incoming/outgoing security
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtAvSecurityInHandler(av_instData_t *instData)
{
    CsrBtResultCode rval;
    CsrBtAvSecurityInReq *prim;
    prim = (CsrBtAvSecurityInReq*)instData->recvMsgP;

    rval = CsrBtScSetSecInLevel(&instData->secIncoming, prim->secLevel,
        CSR_BT_AV_MANDATORY_SECURITY_INCOMING,
        CSR_BT_AV_DEFAULT_SECURITY_INCOMING,
        CSR_BT_RESULT_CODE_AV_SUCCESS,
        CSR_BT_RESULT_CODE_AV_UNACCEPTABLE_PARAMETER);
    CsrBtAvSecurityInCfmSend(prim->appHandle, rval, CSR_BT_SUPPLIER_AV);
}

void CsrBtAvSecurityOutHandler(av_instData_t *instData)
{
    CsrBtResultCode rval;
    CsrBtAvSecurityOutReq *prim;
    prim = (CsrBtAvSecurityOutReq*)instData->recvMsgP;

    rval = CsrBtScSetSecOutLevel(&instData->secOutgoing, prim->secLevel,
        CSR_BT_AV_MANDATORY_SECURITY_OUTGOING,
        CSR_BT_AV_DEFAULT_SECURITY_OUTGOING,
        CSR_BT_RESULT_CODE_AV_SUCCESS,
        CSR_BT_RESULT_CODE_AV_UNACCEPTABLE_PARAMETER);
    CsrBtAvSecurityOutCfmSend(prim->appHandle, rval, CSR_BT_SUPPLIER_AV);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvConnectReqHandler
 *
 *  DESCRIPTION
 *      Handle a request for an AV connection
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvConnectReqHandler( av_instData_t *instData)
{
    CsrUintFast8 i;
    CsrBtAvConnectReq * prim;

    prim = (CsrBtAvConnectReq *) instData->recvMsgP;

    for (i=0; i<CSR_BT_AV_MAX_NUM_CONNECTIONS; i++)
    {
        if(instData->con[i].conState == DISCONNECTED_S)
        {
            break;
        }
    }

    if( i<CSR_BT_AV_MAX_NUM_CONNECTIONS)
    {
        /* found an available connection entry */
        CmnCsrBtLinkedListStruct *sdpTagList = NULL;
        CsrUint16 shIndex;
        instData->appHandle             = prim->phandle;
        instData->con[i].conState       = CONNECTING_S;
        instData->con[i].remoteDevAddr  = prim->deviceAddr;
        /* Start search operation for remote version */
        if ((instData->role == CSR_BT_AV_AUDIO_SINK) || (instData->role == CSR_BT_AV_VIDEO_SINK))
        {
            if (instData->role == CSR_BT_AV_VIDEO_SINK)
            {
                sdpTagList = CsrBtUtilSdrCreateServiceHandleEntryFromUuid32(sdpTagList, CSR_BT_VIDEO_SOURCE_UUID, &shIndex);
            }
            else
            {
                sdpTagList = CsrBtUtilSdrCreateServiceHandleEntryFromUuid32(sdpTagList, CSR_BT_AUDIO_SOURCE_UUID, &shIndex);
            }
        }
        else
        {
            if (instData->role == CSR_BT_AV_VIDEO_SOURCE)
            {
                sdpTagList = CsrBtUtilSdrCreateServiceHandleEntryFromUuid32(sdpTagList, CSR_BT_VIDEO_SINK_UUID, &shIndex);
            }
            else
            {
                sdpTagList = CsrBtUtilSdrCreateServiceHandleEntryFromUuid32(sdpTagList, CSR_BT_AUDIO_SINK_UUID, &shIndex);
            }
        }
        if (NULL != sdpTagList)
        {
            CsrBtUtilSdrCreateAndInsertAttribute(sdpTagList, shIndex, CSR_BT_PROTOCOL_DESCRIPTOR_LIST_ATTRIBUTE_IDENTIFIER, NULL, 0);
            CsrBtUtilSdcSearchStart((void *)instData, instData->sdpAvSearchData, sdpTagList, prim->deviceAddr);
        }
        else
        {
        /* Search operation started */
        CsrBtCml2caConnectReqSend(CSR_BT_AV_IFACEQUEUE,
                             prim->deviceAddr,
                             CSR_BT_AVDTP_PSM,
                             CSR_BT_AVDTP_PSM,
                             CSR_BT_AV_PROFILE_DEFAULT_MTU_SIZE,
                             instData->secOutgoing);
        }
    }
    else
    {
        /* CSRMAX. number of connections reached, reject */
        CsrBtAvConnectCfmSend(prim->phandle, 0,
                              prim->deviceAddr,
                              CSR_BT_RESULT_CODE_AV_MAX_NUM_OF_CONNECTIONS,
                              CSR_BT_SUPPLIER_AV,
                              CSR_BT_CONN_ID_INVALID);
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvCancelConnectReqHandler
 *
 *  DESCRIPTION
 *      Handle a request cancelling AV connection establishment
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvCancelConnectReqHandler( av_instData_t *instData)
{
    CsrUintFast8 i;
    CsrBtAvCancelConnectReq *prim;

    prim = (CsrBtAvCancelConnectReq *) instData->recvMsgP;

    for(i=0; i<CSR_BT_AV_MAX_NUM_CONNECTIONS; i++)
    {
        if( CsrBtBdAddrEq(&instData->con[i].remoteDevAddr, &prim->deviceAddr) )
        {
            if(instData->con[i].conState == CONNECTING_S)
            {
                if(CsrBtUtilSdcSearchCancel((void *)instData, instData->sdpAvSearchData) == TRUE)
                {
                    CsrBtCml2caCancelConnectReqSend(CSR_BT_AV_IFACEQUEUE, prim->deviceAddr, CSR_BT_AVDTP_PSM);
                }
            }
            else if (instData->con[i].conState == CONNECTED_S)
            {
                CsrBtCml2caDisconnectReqSend(instData->con[i].signalCid);
            }

            instData->con[i].conState = CANCEL_CONNECTING_S;
            return;
        }
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      cancelAvConnectReqFromSaveQueue
 *
 *  DESCRIPTION
 *      Remove an AV connection request that was cancelled from the save queue
 *
 *  RETURNS
 *      TRUE if CSR_BT_AV_CONNECT_REQ was removed from queue, otherwise FALSE
 *
 *---------------------------------------------------------------------------*/
static CsrBool cancelAvConnectReqFromSaveQueue(av_instData_t *instData)
{
    CsrUint16                eventClass;
    void                    *msg;
    CsrBool                  cancelMsg   = FALSE;
    CsrMessageQueueType    *tempQueue  = NULL;

    while(CsrMessageQueuePop(&(instData->saveQueue), &eventClass, &msg))
    {
        if (!cancelMsg && eventClass == CSR_BT_AV_PRIM && (*((CsrBtAvPrim *)msg) == CSR_BT_AV_CONNECT_REQ))
        {
           cancelMsg = TRUE;
           CsrPmemFree(msg);
        }
        else
        {
            CsrMessageQueuePush(&tempQueue, eventClass, msg);
        }
    }
    instData->saveQueue = tempQueue;
    return (cancelMsg);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvCancelConnectReqNullStateHandler
 *
 *  DESCRIPTION
 *      Handle a request for cancel an AV connection in the NULL state
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvCancelConnectReqNullStateHandler( av_instData_t *instData)
{
    if (cancelAvConnectReqFromSaveQueue(instData))
    {
        CsrBtAvCancelConnectReq *prim;

        prim = (CsrBtAvCancelConnectReq *) instData->recvMsgP;

        CsrBtAvConnectCfmSend(instData->appHandle, 0, prim->deviceAddr,
                              CSR_BT_RESULT_CODE_AV_CANCEL_CONNECT_ATTEMPT,
                              CSR_BT_SUPPLIER_AV,
                              CSR_BT_CONN_ID_INVALID);
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvDisconnectReqHandler
 *
 *  DESCRIPTION
 *      Handle a request disconnecting an AV connection
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvDisconnectReqHandler( av_instData_t *instData)
{
    CsrBtAvDisconnectReq    *prim;

    prim = (CsrBtAvDisconnectReq *) instData->recvMsgP;

    if(instData->con[prim->connectionId].conState == CONNECTED_S)
    {
        CsrUintFast8 s;

        for(s=0; s<CSR_BT_AV_MAX_NUM_STREAMS; s++)
        {
            if(prim->connectionId == instData->stream[s].conId)
            {
                if ( (instData->stream[s].mediaCid != 0)
                      && (instData->stream[s].streamState >= OPENED_S)
                      && (instData->stream[s].streamState <= PEER_ABORTING_S))
                {
                    CsrBtCml2caDisconnectReqSend(instData->stream[s].mediaCid);
                }
                else if (instData->stream[s].streamState == OPENING_S)
                {
                    instData->stream[s].streamState = TERMINATE_OPENING_S;
                }
            }
        }

        CsrBtCml2caDisconnectReqSend(instData->con[prim->connectionId].signalCid);
        instData->con[prim->connectionId].conState = DISCONNECTING_S;
    }
    else
    {
        CsrBtAvDisconnectIndSend(instData->appHandle, prim->connectionId,
            FALSE,
            CSR_BT_RESULT_CODE_AV_NOT_CONNECTED,
            CSR_BT_SUPPLIER_AV);
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvGetChannelInfoReqHandler
 *
 *  DESCRIPTION
 *      Handle a CSR_BT_AV_GET_CHANNEL_INFO_REQ
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvGetChannelInfoReqHandler(av_instData_t *instData)
{
    CsrBtAvGetChannelInfoReq *prim = (CsrBtAvGetChannelInfoReq *)instData->recvMsgP;
    CsrBtCmL2caGetChannelInfoReqSend(prim->btConnId,CSR_BT_AV_IFACEQUEUE);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvGetStreamChannelInfoReqHandler
 *
 *  DESCRIPTION
 *      Handle a CSR_BT_AV_GET_STREAM_CHANNEL_INFO_REQ
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvGetStreamChannelInfoReqHandler(av_instData_t *instData)
{
    CsrBtAvGetStreamChannelInfoReq *prim = (CsrBtAvGetStreamChannelInfoReq *)instData->recvMsgP;
    CsrBtConnId btConnId = instData->stream[prim->shandle].mediaCid;
    CsrBtCmL2caGetChannelInfoReqSend(btConnId,CSR_BT_AV_IFACEQUEUE);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvCmL2caConnectCfmHandler
 *
 *  DESCRIPTION
 *      Handle a CSR_BT_CM_L2CA_CONNECT_CFM
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvCmL2caConnectCfmHandler( av_instData_t *instData)
{
    CsrBtCmL2caConnectCfm *prim;
    CsrUintFast8 i, s;
    CsrBool knownDevice = FALSE;

    prim = (CsrBtCmL2caConnectCfm*) instData->recvMsgP;

    /* could be a terminating stream being opened, check it */
    for(s=0; s<CSR_BT_AV_MAX_NUM_STREAMS; s++)
    {
        if( instData->stream[s].streamState == TERMINATE_OPENING_S )
        {
            if (prim->resultSupplier == CSR_BT_SUPPLIER_CM &&
                prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS)
            {
                /* immediately disconnect it, we are supposed to terminate it */
                CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_CONTROL_CHANNEL,prim->deviceAddr,prim->btConnId);
                CsrBtCml2caDisconnectReqSend(prim->btConnId);
                knownDevice = TRUE;
            }
            CsrBtAvClearStream(instData, (CsrUint8)s);
            return;
        }
    }

    for (i=0; i<CSR_BT_AV_MAX_NUM_CONNECTIONS; i++)
    {
        if( CsrBtBdAddrEq(&instData->con[i].remoteDevAddr, &prim->deviceAddr))
        {
            knownDevice = TRUE;
            if (prim->resultSupplier == CSR_BT_SUPPLIER_CM &&
                prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS )
            {
                /* the creation of an L2CAP channel was a success */
                if(instData->con[i].conState == CONNECTING_S)
                {
                    CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_CONTROL_CHANNEL,prim->deviceAddr,prim->btConnId);
                    /* we are establishing the signalling channel */
                    instData->con[i].signalCid = prim->btConnId;
                    instData->con[i].signalMtu = prim->mtu;
                    instData->con[i].timerId = 0;
                    CsrBtAvConnectCfmSend(instData->appHandle,
                                          (CsrUint8)i,
                                          prim->deviceAddr,
                                          CSR_BT_RESULT_CODE_AV_SUCCESS,
                                          CSR_BT_SUPPLIER_AV,
                                          prim->btConnId);
                    CsrBtAvStatusIndSend(instData, CSR_BT_AV_CONNECT_CFM, 0,(CsrUint8) i);

                    CsrSchedTimerCancel(instData->con[i].lpSigTimerId,NULL,NULL);
                    instData->con[i].lpSigMode = CSR_BT_LINK_STATUS_CONNECTED;
                    instData->con[i].lpSigTimerId = CsrSchedTimerSet(CSR_BT_AV_SIGNAL_LP_TIMEOUT * 1000, CsrBtAvSignallingLowPowerHandler, (CsrUint8)i, instData);

                    instData->con[i].conState = CONNECTED_S;
                }
                else if (instData->con[i].conState == CONNECTED_S)
                {
                    /* we are establishing the media stream channel */
                    for(s=0; s<CSR_BT_AV_MAX_NUM_STREAMS; s++)
                    {
                        if((instData->stream[s].conId == i) &&
                            (instData->stream[s].streamState == OPENING_S))
                        {
                            CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_CONTROL_CHANNEL,prim->deviceAddr,prim->btConnId);
                            instData->stream[s].streamState = OPENED_S;
                            instData->stream[s].mediaCid = prim->btConnId;
                            instData->stream[s].mediaMtu = prim->mtu;

                            CsrBtAvOpenCfmSend(instData, (CsrUint8)s, instData->stream[s].tLabel,
                                CSR_BT_RESULT_CODE_AV_SUCCESS,
                                CSR_BT_SUPPLIER_AV);
                            CsrBtAvStreamMtuSizeIndSend(instData, (CsrUint8)s, prim->btConnId);
                            instData->stream[s].tLabel = 0xFF; /* invalidate tLabel */

                            instData->stream[s].lpModeCurrent = CSR_BT_LINK_STATUS_CONNECTED;

                            if (instData->allowStartLpNeg)
                            {
                                instData->stream[s].lpModeRequested = CSR_BT_LINK_STATUS_SNIFF;
                                CsrBtCml2caModeChangeReqSend(prim->btConnId, CSR_BT_LINK_STATUS_SNIFF);
                            }
                            else
                            {
                                instData->stream[s].lpModeRequested = CSR_BT_LINK_STATUS_CONNECTED;
                            }
                            break;
                        }
                    }
                }
                else if (instData->con[i].conState == CANCEL_CONNECTING_S)
                {
                    CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_CONTROL_CHANNEL,prim->deviceAddr,prim->btConnId);
                    /* a connect cancel came in too late, need to disconnect */
                    CsrBtCml2caDisconnectReqSend(prim->btConnId);
                }
            }
            else
            {
                /* the creation of an L2CAP connection failed
                 * so propagate the error up to caller */

                if (instData->con[i].conState == CANCEL_CONNECTING_S )
                {
                    /* Except if we cancelled the connection attempt. */
                    CsrBtAvClearConnection(instData, (CsrUint8)i);
                    CsrBtAvConnectCfmSend(instData->appHandle,
                                          0, prim->deviceAddr,
                                          CSR_BT_RESULT_CODE_AV_CANCEL_CONNECT_ATTEMPT,
                                          CSR_BT_SUPPLIER_AV,
                                          CSR_BT_CONN_ID_INVALID);
                }
                else if (instData->con[i].conState != CONNECTED_S)
                {
                    /* we are establishing the signalling channel */
                    CsrBtAvClearConnection(instData, (CsrUint8)i);
                    CsrBtAvConnectCfmSend(instData->appHandle,
                                          0,
                                          prim->deviceAddr,
                                          prim->resultCode,
                                          prim->resultSupplier,
                                          prim->btConnId);
                }
                else
                {
                    /* we are establishing the media stream channel */
                    CsrUintFast8 s;

                    for(s=0; s<CSR_BT_AV_MAX_NUM_STREAMS; s++)
                    {
                        if( (instData->stream[s].conId == i) && (instData->stream[s].streamState == OPENING_S))
                        {
                            instData->stream[s].streamState = CONFIGURED_S;
                            CsrBtAvOpenCfmSend(instData, (CsrUint8)s, instData->stream[s].tLabel,
                                prim->resultCode, prim->resultSupplier);
                            break;
                        }
                    }
                }
            }
            break;
        }
    }

    if (!knownDevice && prim->resultSupplier == CSR_BT_SUPPLIER_CM &&
        prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS)
    {
        /* Disconnect all unknown connections */
        CsrBtCml2caDisconnectReqSend(prim->btConnId);
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvCmL2caConnectAcceptCfmHandler
 *
 *  DESCRIPTION
 *      Handle a CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvCmL2caConnectAcceptCfmHandler( av_instData_t *instData)
{
    CsrUintFast8 i, s;
    CsrBtCmL2caConnectAcceptCfm *prim = (CsrBtCmL2caConnectAcceptCfm *) instData->recvMsgP;

    for (i=0; i<CSR_BT_AV_MAX_NUM_CONNECTIONS; i++)
    {
        if( (instData->con[i].conState == CONNECTED_S)
            && CsrBtBdAddrEq(&instData->con[i].remoteDevAddr, &prim->deviceAddr) )
        {
            /* apparently a stream being opened, lets find that stream */
            for (s=0; s<CSR_BT_AV_MAX_NUM_STREAMS; s++)
            {
                if((instData->stream[s].conId == i) &&
                    ((instData->stream[s].streamState == PEER_OPENING_S) || (instData->stream[s].streamState == PEER_OPENING_TO_STREAM_S)))
                {
                    if( prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
                    {
                        instData->stream[s].mediaCid = prim->btConnId;
                        instData->stream[s].mediaMtu = prim->mtu;

                        CsrBtAvStreamMtuSizeIndSend(instData,(CsrUint8) s, prim->btConnId);

                        if (instData->stream[s].streamState == PEER_OPENING_S)
                        {
                            CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_CONTROL_CHANNEL,prim->deviceAddr,prim->btConnId);
                            instData->stream[s].streamState = OPENED_S;

                            instData->stream[s].lpModeCurrent = CSR_BT_LINK_STATUS_CONNECTED;
                            if (instData->allowStartLpNeg)
                            {
                                instData->stream[s].lpModeRequested = CSR_BT_LINK_STATUS_SNIFF;
                                CsrBtCml2caModeChangeReqSend(prim->btConnId, CSR_BT_LINK_STATUS_SNIFF);
                            }
                            else
                            {
                                instData->stream[s].lpModeRequested = CSR_BT_LINK_STATUS_CONNECTED;
                            }
                        }
                        else
                        {
                            CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_STREAM_CHANNEL,prim->deviceAddr,prim->btConnId);
                            instData->stream[s].streamState = STREAMING_S;
                            /* A start cmd has been received and responded before the streaming channel was opened
                                just start the activity monitor timer and stay in connected state for now */
                            instData->stream[s].lpModeCurrent = CSR_BT_LINK_STATUS_CONNECTED;
                            instData->stream[s].lpModeRequested = CSR_BT_LINK_STATUS_CONNECTED;

#if defined(AV_STREAM_INACTIVITY_LP_ENABLE) && (AV_STREAM_INACTIVITY_LP_ENABLE > 0)
                            CsrSchedTimerCancel(instData->stream[s].lpTimerId,NULL,NULL);
                            instData->stream[s].seqNoAtLastLpCheck = instData->stream[s].seqNo;

                            if (instData->allowStartLpNeg)
                            {
                                instData->stream[s].lpTimerId = AV_START_LP_TIMER(s);
                            }
#endif
                        }
                    }
                    else
                    {
                        instData->stream[s].streamState = CONFIGURED_S;
                        /* throw exception ? */
                    }
                    break;
                }
            }


            if (s == CSR_BT_AV_MAX_NUM_STREAMS)
            {/* Stream not found; already connected: reject this connection  */
                CsrBtCml2caDisconnectReqSend(prim->btConnId);
                /* and make sure to accepting new incoming connections */
                CsrBtAvMakeConnectable(instData);
            }
            return;
        }
    }

    /* could be a terminating stream being opened, check it */
    for(s=0; s<CSR_BT_AV_MAX_NUM_STREAMS; s++)
    {
        if( instData->stream[s].streamState == TERMINATE_PEER_OPENING_S )
        {
            /* immediately disconnect it, we are supposed to terminate it */
            CsrBtCml2caDisconnectReqSend(prim->btConnId);
            CsrBtAvClearStream(instData, (CsrUint8)s);
            return;
        }
    }

    /* then lets assume it is a new (device) signalling connection */
    instData->isConnectable = FALSE;
    if( prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_CONTROL_CHANNEL,prim->deviceAddr,prim->btConnId);

        for (i=0; i<CSR_BT_AV_MAX_NUM_CONNECTIONS; i++)
        {
            if(instData->con[i].conState == DISCONNECTED_S)
            {
                break;
            }
        }

        if( i<CSR_BT_AV_MAX_NUM_CONNECTIONS)
        {
            instData->con[i].conState = CONNECTED_S;
            instData->con[i].incoming = TRUE;
            instData->con[i].remoteDevAddr  = prim->deviceAddr;
            instData->con[i].signalCid = prim->btConnId;
            instData->con[i].signalMtu = prim->mtu;
            instData->con[i].timerId = 0;

            CsrBtAvConnectIndSend(instData->appHandle, (CsrUint8) i,
                                  prim->deviceAddr,
                                  prim->btConnId);
            CsrBtAvStatusIndSend(instData, CSR_BT_AV_CONNECT_IND, 0,(CsrUint8) i);

            CsrSchedTimerCancel(instData->con[i].lpSigTimerId,NULL,NULL);
            instData->con[i].lpSigMode = CSR_BT_LINK_STATUS_CONNECTED;
            if (instData->allowStartLpNeg)
            {
                instData->con[i].lpSigTimerId = CsrSchedTimerSet(CSR_BT_AV_SIGNAL_LP_TIMEOUT * 1000, CsrBtAvSignallingLowPowerHandler,(CsrUint8) i, instData);
            }
        }
        else
        {
            /* ...out of resources, reject this connection */
            CsrBtCml2caDisconnectReqSend(prim->btConnId);
        }

        if( getNumActivations(instData->roleRegister) > getNumIncomingCon(instData) )
        {
            CsrBtAvMakeConnectable(instData);
        }
    }
    else
    {
        /* ...out of connection resources */
        /* throw exception..? */
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvCmL2caCancelConnectAcceptCfmHandler
 *
 *  DESCRIPTION
 *      Handle a CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvCmL2caCancelConnectAcceptCfmHandler( av_instData_t *instData)
{
    CsrUintFast8 s;
    CsrBtCmL2caCancelConnectAcceptCfm *prim;

    prim = (CsrBtCmL2caCancelConnectAcceptCfm *) instData->recvMsgP;

    /* first check if any streams are doing cancel accept connect */
    for(s=0; s<CSR_BT_AV_MAX_NUM_STREAMS; s++)
    {
        if(instData->stream[s].streamState == TERMINATE_PEER_OPENING_S)
        {
            if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
            {
                CsrBtAvClearStream(instData,(CsrUint8) s);
            }
            return;
        }
    }

    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        if(instData->doDeactivate)
        {
            CsrBtAvDeactivateCfmSend(instData->appHandle, CSR_BT_RESULT_CODE_AV_SUCCESS,
                CSR_BT_SUPPLIER_AV);
            instData->doDeactivate = FALSE;
            instData->isConnectable = FALSE;
        }
    }
    else
    {
        /* an incoming connection must be about to arrive */
        if(instData->doDeactivate)
        {
            CsrBtAvDeactivateCfmSend(instData->appHandle, CSR_BT_RESULT_CODE_AV_CANCEL_CONNECT_ATTEMPT,
                CSR_BT_SUPPLIER_AV);
            instData->doDeactivate = FALSE;
        }
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvCmL2caDisconnectIndHandler
 *
 *  DESCRIPTION
 *      Handle a CSR_BT_CM_L2CA_DISCONNECT_IND
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvCmL2caDisconnectIndHandler( av_instData_t *instData)
{
    CsrUintFast8 i, s;
    CsrBtCmL2caDisconnectInd *prim;

    prim = (CsrBtCmL2caDisconnectInd*) instData->recvMsgP;

    for (s=0; s<CSR_BT_AV_MAX_NUM_STREAMS; s++)
    {
        if( prim->btConnId == instData->stream[s].mediaCid)
        {
            if( instData->stream[s].streamState == CLOSING_S)
            {
                CsrBtAvCloseCfmSend(instData,(CsrUint8) s, instData->stream[s].tLabel,
                    CSR_BT_RESULT_CODE_AV_SUCCESS,
                    CSR_BT_SUPPLIER_AV);
            }
            else if( instData->stream[s].streamState == ABORTING_S)
            {
                CsrBtAvAbortCfmSend(instData,(CsrUint8) s, instData->stream[s].tLabel);
            }

            CsrBtCmLogicalChannelTypeReqSend(CSR_BT_NO_ACTIVE_LOGICAL_CHANNEL,
                              instData->con[instData->stream[s].conId].remoteDevAddr,prim->btConnId);

            instData->stream[s].mediaCid = 0;

            CsrBtAvClearStream(instData,(CsrUint8) s);
            return;
        }
    }

    for (i=0; i<CSR_BT_AV_MAX_NUM_CONNECTIONS; i++)
    {
        if(prim->btConnId == instData->con[i].signalCid)
        {
            CsrBtCmLogicalChannelTypeReqSend(CSR_BT_NO_ACTIVE_LOGICAL_CHANNEL,
                              instData->con[i].remoteDevAddr,prim->btConnId);
            if( instData->con[i].conState == CANCEL_CONNECTING_S)
            {
                CsrBtAvDisconnectIndSend(instData->appHandle,(CsrUint8) i,
                    TRUE,
                    CSR_BT_RESULT_CODE_AV_CANCEL_CONNECT_ATTEMPT,
                    CSR_BT_SUPPLIER_AV);
            }
            else
            {
                CsrBtAvDisconnectIndSend(instData->appHandle,(CsrUint8) i,
                    prim->localTerminated,
                    prim->reasonCode, prim->reasonSupplier);
            }
            CsrBtAvStatusIndSend(instData, CSR_BT_AV_DISCONNECT_IND, 0,(CsrUint8) i);

            if( (instData->con[i].incoming == TRUE) && !instData->isConnectable
                && (getNumActivations(instData->roleRegister) > (getNumIncomingCon(instData) - 1)) )
            {
                /* the released connection was an incoming - need to be connectable once again */
                CsrBtAvMakeConnectable(instData);
            }

            CsrBtAvClearConnection(instData,(CsrUint8) i);
            return;
        }
    }

    /* the rests of a disconnect caused by a cancel connect arriving too late */
    for (i=0; i<CSR_BT_AV_MAX_NUM_CONNECTIONS; i++)
    {
        if( instData->con[i].conState == CANCEL_CONNECTING_S)
        {
            CsrBtAvConnectCfmSend(instData->appHandle,
                                  0,
                                  instData->con[i].remoteDevAddr,
                                  CSR_BT_RESULT_CODE_AV_CANCEL_CONNECT_ATTEMPT,
                                  CSR_BT_SUPPLIER_AV,
                                  CSR_BT_CONN_ID_INVALID);
            CsrBtAvClearConnection(instData,(CsrUint8) i);
        }
    }

    /* must have been a rejected connection */
    if( !instData->isConnectable
        && (getNumActivations(instData->roleRegister) > getNumIncomingCon(instData) ) )
    {
        CsrBtAvMakeConnectable(instData);
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvCmL2caConnectCfmHandlerCleanup
 *
 *  DESCRIPTION
 *      Handle a CSR_BT_CM_L2CA_CONNECT_CFM in clean-up state
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvCmL2caConnectCfmHandlerCleanup( av_instData_t *instData)
{
    CsrBtCmL2caConnectCfm *prim;
    CsrUintFast8 i, s;

    prim = (CsrBtCmL2caConnectCfm*) instData->recvMsgP;

    /* could be a terminating stream being opened, check it */
    for (s = 0; s < CSR_BT_AV_MAX_NUM_STREAMS; s++)
    {
        if (instData->stream[s].streamState == TERMINATE_OPENING_S )
        {
            if (prim->resultSupplier == CSR_BT_SUPPLIER_CM &&
                prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS)
            {
                CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_STREAM_CHANNEL,prim->deviceAddr,prim->btConnId);
                /* immediately disconnect it, we are supposed to terminate it */
                CsrBtCml2caDisconnectReqSend(prim->btConnId);
            }
            CsrBtAvClearStream(instData,(CsrUint8) s);
            return;
        }
    }

    for (i = 0; i < CSR_BT_AV_MAX_NUM_CONNECTIONS; i++)
    {
        if (CsrBtBdAddrEq(&instData->con[i].remoteDevAddr, &prim->deviceAddr))
        {
            if (prim->resultSupplier == CSR_BT_SUPPLIER_CM &&
                prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS)
            {
                CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_CONTROL_CHANNEL,prim->deviceAddr,prim->btConnId);
                /* immediately disconnect it, we are supposed to terminate it */
                CsrBtCml2caDisconnectReqSend(prim->btConnId);
            }
            CsrBtAvClearConnection(instData,(CsrUint8) i);
            break;
        }
    }

    CsrBtAvIsCleanUpFinished(instData);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvCmL2caConnectAcceptCfmHandlerCleanup
 *
 *  DESCRIPTION
 *      Handle a CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM in clean-up state
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvCmL2caConnectAcceptCfmHandlerCleanup( av_instData_t *instData)
{
    CsrUintFast8 s;
    CsrBtCmL2caConnectAcceptCfm *prim = (CsrBtCmL2caConnectAcceptCfm *) instData->recvMsgP;

    /* could be a terminating stream being opened, check it */
    for(s=0; s<CSR_BT_AV_MAX_NUM_STREAMS; s++)
    {
        if( instData->stream[s].streamState == TERMINATE_PEER_OPENING_S )
        {
            CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_STREAM_CHANNEL,prim->deviceAddr,prim->btConnId);
            /* immediately disconnect it, we are supposed to terminate it */
            CsrBtCml2caDisconnectReqSend(prim->btConnId);
            CsrBtAvClearStream(instData,(CsrUint8) s);
            break;
        }

        CsrBtAvIsCleanUpFinished(instData);
        if (instData->state == READY_S)
        {
            return;
        }
    }

    /* then lets assume it is a new (device) signalling connection */
    instData->isConnectable = FALSE;
    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_CONTROL_CHANNEL,prim->deviceAddr,prim->btConnId);
        CsrBtCml2caDisconnectReqSend(prim->btConnId);
    }

    CsrBtAvIsCleanUpFinished(instData);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvCmL2caCancelConnectAcceptCfmHandlerCleanup
 *
 *  DESCRIPTION
 *      Handle a CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM in clean-up state
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvCmL2caCancelConnectAcceptCfmHandlerCleanup( av_instData_t *instData)
{
    CsrUintFast8 s;
    CsrBtCmL2caCancelConnectAcceptCfm *prim;

    prim = (CsrBtCmL2caCancelConnectAcceptCfm *) instData->recvMsgP;

    /* first check if any streams are doing cancel accept connect */
    for(s=0; s<CSR_BT_AV_MAX_NUM_STREAMS; s++)
    {
        if(instData->stream[s].streamState == TERMINATE_PEER_OPENING_S)
        {
            if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
            {
                CsrBtAvClearStream(instData,(CsrUint8) s);
            }

            CsrBtAvIsCleanUpFinished(instData);
            return;
        }
    }

    instData->isConnectable = FALSE;

    CsrBtAvIsCleanUpFinished(instData);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvCmL2caDisconnectIndHandlerCleanup
 *
 *  DESCRIPTION
 *      Handle a CSR_BT_CM_L2CA_DISCONNECT_IND in clean-up state
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvCmL2caDisconnectIndHandlerCleanup( av_instData_t *instData)
{
    CsrUintFast8 i, s;
    CsrBtCmL2caDisconnectInd *prim;

    prim = (CsrBtCmL2caDisconnectInd*) instData->recvMsgP;

    for (s=0; s<CSR_BT_AV_MAX_NUM_STREAMS; s++)
    {
        if( prim->btConnId == instData->stream[s].mediaCid)
        {
            CsrBtCmLogicalChannelTypeReqSend(CSR_BT_NO_ACTIVE_LOGICAL_CHANNEL,
                              instData->con[instData->stream[s].conId].remoteDevAddr,prim->btConnId);
            instData->stream[s].mediaCid = 0;

            CsrBtAvClearStream(instData,(CsrUint8) s);
            CsrBtAvIsCleanUpFinished(instData);
            return;
        }
    }

    for (i=0; i<CSR_BT_AV_MAX_NUM_CONNECTIONS; i++)
    {
        if(prim->btConnId == instData->con[i].signalCid)
        {
            CsrBtCmLogicalChannelTypeReqSend(CSR_BT_NO_ACTIVE_LOGICAL_CHANNEL,
                              instData->con[i].remoteDevAddr,prim->btConnId);
            CsrBtAvStatusIndSend(instData, CSR_BT_AV_DISCONNECT_IND, 0,(CsrUint8) i);
            CsrBtAvClearConnection(instData,(CsrUint8) i);
            break;
        }
    }

    CsrBtAvIsCleanUpFinished(instData);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvSignallingLowPowerHandler
 *
 *  DESCRIPTION
 *      Handles a timeout of the signalling low power timer, enter
 *      sniff mode due to inactivity on the signalling channel.
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvSignallingLowPowerHandler(CsrUint16 conId, void *data)
{
    av_instData_t *instData = data;

    if( instData->con[conId].lpSigTimerId != 0)
    {
        instData->con[conId].lpSigTimerId = 0;

        if ((instData->con[conId].lpSigMode != CSR_BT_LINK_STATUS_SNIFF) && 
            (instData->con[conId].conState == CONNECTED_S) &&
            (instData->allowStartLpNeg))
        {
            CsrBtCml2caModeChangeReqSend(instData->con[conId].signalCid, CSR_BT_LINK_STATUS_SNIFF);
        }
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvCmL2caModeChangeIndHandler
 *
 *  DESCRIPTION
 *      Handle a CSR_BT_CM_L2CA_MODE_CHANGE_IND
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvCmL2caModeChangeIndHandler(av_instData_t *instData)
{
    CsrUintFast8 s;
    CsrBtCmL2caModeChangeInd *prim;

    prim = (CsrBtCmL2caModeChangeInd *) instData->recvMsgP;

    /* guard the streaming channel against park mode */
    for (s=0; s<CSR_BT_AV_MAX_NUM_STREAMS; s++)
    {
        if( prim->btConnId == instData->stream[s].mediaCid)
        {
            instData->stream[s].lpModeCurrent = prim->mode;


            if (prim->resultCode == CSR_BT_RESULT_CODE_CM_MODE_CHANGE_REQUEST_OVERRULED &&
                prim->resultSupplier == CSR_BT_SUPPLIER_CM)
            {
                instData->stream[s].modeChangeOverRuleByApp = TRUE;
            }
            else
            {
                instData->stream[s].modeChangeOverRuleByApp = FALSE;
            }

            if( prim->mode == CSR_BT_LINK_STATUS_CONNECTED )
            {
                if (instData->allowStartLpNeg)
                {
                    if( instData->stream[s].lpModeRequested == prim->mode)
                    {
                        /* This trick is to make sure that AV_STREAM_REQ doesn't send more than one mode change request at a time */
                        instData->stream[s].lpModeRequested = CSR_BT_LINK_STATUS_SNIFF;
                    }

#if defined(AV_STREAM_INACTIVITY_LP_ENABLE) && (AV_STREAM_INACTIVITY_LP_ENABLE > 0)
                    CsrSchedTimerCancel(instData->stream[s].lpTimerId,NULL,NULL);
                    instData->stream[s].seqNoAtLastLpCheck = instData->stream[s].seqNo;

                    instData->stream[s].lpTimerId = AV_START_LP_TIMER(s);
#endif
                }
            }
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
            else if( prim->mode == CSR_BT_LINK_STATUS_PARK )
            {
                /* don't accept park, keep current mode */
                CsrBtCml2caModeChangeReqSend(prim->btConnId, instData->stream[s].lpModeRequested);
            }
#endif
            return;
        }
    }

    /* a signalling connection, don't accept park - if park, then force link to back to sniff */
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
    if(prim->mode == CSR_BT_LINK_STATUS_PARK)
    {
        CsrBtCml2caModeChangeReqSend(prim->btConnId, CSR_BT_LINK_STATUS_SNIFF);
    }
#endif
    /* Find out if we requested the mode change and if so proces the queued commands */
    for (s=0; s<CSR_BT_AV_MAX_NUM_CONNECTIONS; s++)
    {
        if( prim->btConnId == instData->con[s].signalCid)
        {
            if (prim->resultCode == CSR_BT_RESULT_CODE_CM_MODE_CHANGE_REQUEST_OVERRULED &&
                 prim->resultSupplier == CSR_BT_SUPPLIER_CM)
            {
                instData->con[s].modeChangeOverRuleByApp = TRUE;
            }
            else
            {
                instData->con[s].modeChangeOverRuleByApp = FALSE;
            }

            instData->con[s].lpSigMode = prim->mode;
            if(prim->mode == CSR_BT_LINK_STATUS_CONNECTED || instData->con[s].modeChangeOverRuleByApp)
            {
                if(instData->con[s].lpModeChangePending)
                {
                    CsrUint16                eventClass;
                    void                    *msg;

                    /* We requested the mode change now we process the pending messages */
                    instData->con[s].lpModeChangePending = FALSE;

                    while(CsrMessageQueuePop(&(instData->con[s].pendingSigQueue), &eventClass, &msg))
                    {
                        CsrBtAvPrim         *primType;

                        primType = (CsrBtAvPrim *)msg;

                        CsrPmemFree(instData->recvMsgP);
                        instData->recvMsgP = msg;

                        switch(*primType)
                        {
                            case CSR_BT_AV_DISCOVER_REQ:
                            {
                                CsrBtAvDiscoverReqHandler(instData); break;
                            }
                            case CSR_BT_AV_GET_CAPABILITIES_REQ:
                            {
                                CsrBtAvGetCapabilitiesReqHandler(instData); break;
                            }
                            case CSR_BT_AV_SET_CONFIGURATION_REQ:
                            {
                                CsrBtAvSetConfigurationReqHandler(instData); break;
                            }
                            case CSR_BT_AV_GET_CONFIGURATION_REQ:
                            {
                                CsrBtAvGetConfigurationReqHandler(instData); break;
                            }
                            case CSR_BT_AV_RECONFIGURE_REQ:
                            {
                                CsrBtAvReconfigureReqHandler(instData); break;
                            }
                            case CSR_BT_AV_OPEN_REQ:
                            {
                                CsrBtAvOpenReqHandler(instData); break;
                            }
                            case CSR_BT_AV_START_REQ:
                            {
                                CsrBtAvStartReqHandler(instData); break;
                            }
                            case CSR_BT_AV_CLOSE_REQ:
                            {
                                if (instData->con[s].conState == CONNECTED_S)
                                {
                                    CsrBtAvCloseReqHandler(instData);
                                }
                                break;
                            }
                            case CSR_BT_AV_SUSPEND_REQ:
                            {
                                CsrBtAvSuspendReqHandler(instData); break;
                            }
                            case CSR_BT_AV_ABORT_REQ:
                            {
                                CsrBtAvAbortReqHandler(instData); break;
                            }
                            case CSR_BT_AV_SECURITY_CONTROL_REQ:
                            {
                                CsrBtAvSecurityReqHandler(instData); break;
                            }
                            default:
                            { /* Ignore */
                                break;
                            }
                        }
                    }
                }
                return;
            }
        }
    }
}

#if defined(AV_STREAM_INACTIVITY_LP_ENABLE) && (AV_STREAM_INACTIVITY_LP_ENABLE > 0)
/*----------------------------------------------------------------------------*
 *  NAME
 *      avLowPowerHandler
 *
 *  DESCRIPTION
 *      Handles a timeout of the low power timer, check if it is time to enter
 *      sniff mode due to inactivity on the stream.
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void avLowPowerHandler(CsrUint16 sid, void *data)
{
    av_instData_t *instData = data;
    av_stream_info_t *stream = &instData->stream[sid];

    if( stream->lpTimerId != 0)
    {
        if( stream->seqNo != stream->seqNoAtLastLpCheck )
        {
            /* activity on stream, remain active */
            stream->seqNoAtLastLpCheck = stream->seqNo;

            if (instData->allowStartLpNeg)
            {
                stream->lpTimerId = AV_START_LP_TIMER(sid); /* restart timer */
            }
            else
            {
                stream->lpTimerId = 0;
            }
        }
        else
        {
            /* no activity on stream, enter sniff mode if currently active */
            if( (stream->lpModeCurrent == CSR_BT_LINK_STATUS_CONNECTED) && (instData->allowStartLpNeg) )
            {
                stream->lpModeRequested = CSR_BT_LINK_STATUS_SNIFF;
                CsrBtCml2caModeChangeReqSend(stream->mediaCid, CSR_BT_LINK_STATUS_SNIFF);
            }
            stream->lpTimerId = 0;
        }
    }
}
#endif /* AV_STREAM_INACTIVITY_LP_ENABLE */


/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvCmL2caChannelInfoCfmHandler
 *
 *  DESCRIPTION
 *      Handles the response message from CM to a channel info request issued
 *      previously
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvCmL2caChannelInfoCfmHandler(av_instData_t *instData)
{
    CsrBtCmL2caGetChannelInfoCfm *prim = (CsrBtCmL2caGetChannelInfoCfm *) instData->recvMsgP;
    CsrBtAvGetChannelInfoCfm *avPrim = (CsrBtAvGetChannelInfoCfm *)CsrPmemAlloc(sizeof(CsrBtAvGetChannelInfoCfm));
    
    avPrim->type          = CSR_BT_AV_GET_CHANNEL_INFO_CFM;
    avPrim->aclHandle     = prim->aclHandle;
    avPrim->remoteCid     = prim->remoteCid;

    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS)
    {
        avPrim->resultCode     = CSR_BT_RESULT_CODE_AV_SUCCESS;
        avPrim->resultSupplier = CSR_BT_SUPPLIER_AV;
    }
    else
    {/* Error message: just forward the error code and supplier received */
        avPrim->resultCode     = prim->resultCode;
        avPrim->resultSupplier = prim->resultSupplier;
    }
    
    CsrSchedMessagePut(instData->appHandle, CSR_BT_AV_PRIM, avPrim);
}

