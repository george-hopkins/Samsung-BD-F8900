/****************************************************************************

    Copyright Cambridge Silicon Radio Limited and its licensors 2012. 
        
        All rights reserved.

REVISION:      $Revision: #4 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_bt_result.h"
#include "csr_bt_util.h"
#include "csr_bt_av_main.h"
#include "csr_bt_av_prim.h"
#include "csr_bt_av_lib.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cmn_sdc_rfc_util.h"
#include "csr_bt_cmn_sdr_tagbased_lib.h"
#include "csr_bt_cmn_sdp_lib.h"
#include "csr_bt_sdc_support.h"

void CsrBtAvMessagePut(CsrSchedQid phandle, void *msg)
{
    CsrSchedMessagePut(phandle, CSR_BT_AV_PRIM, msg);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvClearTxSignallingQ
 *
 *  DESCRIPTION
 *      Free memory hanging on Tx signalling queue
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvClearTxSignallingQ(av_connection_info_t *con)
{
    while(con->qFirst != NULL)
    {
        q_element_t *tmp = con->qFirst;
        con->qFirst = con->qFirst->next;

        CsrPmemFree(tmp->data);
        CsrPmemFree(tmp);
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvClearConnectionSignaling
 *
 *  DESCRIPTION
 *      Free memory hanging on signaling queue
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvClearConnectionSignaling(av_connection_info_t *con)
{
    CsrUint16                eventClass;
    void                    *pMsg;

    while(CsrMessageQueuePop(&con->pendingSigQueue, &eventClass, &pMsg))
    {
        CsrBtAvPrim         *primType;

        primType = (CsrBtAvPrim *)pMsg;

        switch(*primType)
        {
            case CSR_BT_AV_SET_CONFIGURATION_REQ:
            {
                CsrBtAvSetConfigurationReq *prim = pMsg;
                CsrPmemFree(prim->appServCapData);
                break;
            }
            case CSR_BT_AV_RECONFIGURE_REQ:
            {
                CsrBtAvReconfigureReq *prim = pMsg;
                CsrPmemFree(prim->servCapData);
                break;
            }
            case CSR_BT_AV_START_REQ:
            {
                CsrBtAvStartReq *prim = pMsg;
                CsrPmemFree(prim->list);
                break;
            }
            case CSR_BT_AV_SUSPEND_REQ:
            {
                CsrBtAvSuspendReq *prim = pMsg;
                CsrPmemFree(prim->list);
                break;
            }
            case CSR_BT_AV_SECURITY_CONTROL_REQ:
            {
                CsrBtAvSecurityControlReq *prim = pMsg;
                CsrPmemFree(prim->contProtMethodData);
                break;
            }
            case CSR_BT_AV_DISCOVER_REQ:
            case CSR_BT_AV_GET_CAPABILITIES_REQ:
            case CSR_BT_AV_GET_CONFIGURATION_REQ:
            case CSR_BT_AV_OPEN_REQ:
            case CSR_BT_AV_CLOSE_REQ:
            case CSR_BT_AV_ABORT_REQ:
                /* just fall through and CsrPmemFree */
            default:
            {
                break;
            }
        }

        CsrPmemFree(pMsg);
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvClearConnection
 *
 *  DESCRIPTION
 *      Clear/reset connection information
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvClearConnection(av_instData_t *instData, CsrUint8 conId)
{
    CsrUintFast8 s;

    if(instData->con[conId].timerId != 0)
    {
        CsrSchedTimerCancel(instData->con[conId].timerId, NULL, NULL);
        instData->con[conId].timerId = 0;
    }

    if(instData->con[conId].lpSigTimerId != 0)
    {
        CsrSchedTimerCancel(instData->con[conId].lpSigTimerId, NULL, NULL);
        instData->con[conId].lpSigTimerId = 0;
    }

    /* streams depending on this connection needs cleaning too */
    for(s=0; s<CSR_BT_AV_MAX_NUM_STREAMS; s++)
    {
        if(instData->stream[s].conId == conId)
        {
            CsrBtAvClearStream(instData, (CsrUint8)s);
        }
    }

    /* all signaling stored for this connection needs cleaning too */
    CsrBtAvClearConnectionSignaling(&instData->con[conId]);

    /* free any rx signalling message fragments stored internally */
    CsrBtAvFreeFragments( &instData->fragmentHeadPtr, instData->con[conId].signalCid);

    CsrBtAvClearTxSignallingQ( &instData->con[conId]);

    CsrMemSet(&instData->con[conId], 0, sizeof(av_connection_info_t));
    instData->con[conId].modeChangeOverRuleByApp = FALSE;
    instData->con[conId].conState = DISCONNECTED_S;
    instData->con[conId].remoteAVDTPVersion = DEFAULT_AVDTP_VERSION;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvClearStreamSignaling
 *
 *  DESCRIPTION
 *      Free memory hanging on signaling queue according to the stream that the
 *      signaling are sent for
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvClearStreamSignaling(av_connection_info_t *con, CsrUint8 shandle)
{
    CsrUint16                eventClass;
    void                    *pMsg;
    CsrMessageQueueType    *tempQueue  = NULL;

    while(CsrMessageQueuePop(&con->pendingSigQueue, &eventClass, &pMsg))
    {
        CsrBtAvPrim         *primType;
        CsrBool              CsrPmemFreePrim = FALSE;

        primType = (CsrBtAvPrim *)pMsg;

        switch(*primType)
        {
            case CSR_BT_AV_GET_CONFIGURATION_REQ:
            {
                CsrBtAvGetConfigurationReq *prim = pMsg;

                if(prim->shandle == shandle)
                {
                    CsrPmemFreePrim = TRUE;
                }
                break;
            }
            case CSR_BT_AV_RECONFIGURE_REQ:
            {
                CsrBtAvReconfigureReq *prim = pMsg;

                if(prim->shandle == shandle)
                {
                    CsrPmemFree(prim->servCapData);
                    CsrPmemFreePrim = TRUE;
                }
                break;
            }
            case CSR_BT_AV_OPEN_REQ:
            {
                CsrBtAvOpenReq *prim = pMsg;

                if(prim->shandle == shandle)
                {
                    CsrPmemFreePrim = TRUE;
                }
                break;
            }
            case CSR_BT_AV_START_REQ:
            {
                CsrBtAvStartReq *prim = pMsg;

                if(prim->list[0] == shandle)
                {
                    CsrPmemFree(prim->list);
                    CsrPmemFreePrim = TRUE;
                }
                break;
            }
            case CSR_BT_AV_CLOSE_REQ:
            {
                CsrBtAvCloseReq *prim = pMsg;

                if(prim->shandle == shandle)
                {
                    CsrPmemFreePrim = TRUE;
                }
                break;
            }
            case CSR_BT_AV_SUSPEND_REQ:
            {
                CsrBtAvSuspendReq *prim = pMsg;

                if(prim->list[0] == shandle)
                {
                    CsrPmemFree(prim->list);
                    CsrPmemFreePrim = TRUE;
                }
                break;
            }
            case CSR_BT_AV_ABORT_REQ:
            {
                CsrBtAvAbortReq *prim = pMsg;

                if(prim->shandle == shandle)
                {
                    CsrPmemFreePrim = TRUE;
                }
                break;
            }
            case CSR_BT_AV_SECURITY_CONTROL_REQ:
            {
                CsrBtAvSecurityControlReq *prim = pMsg;

                if(prim->shandle == shandle)
                {
                    CsrPmemFree(prim->contProtMethodData);
                    CsrPmemFreePrim = TRUE;
                }
                break;
            }
            case CSR_BT_AV_DISCOVER_REQ:
            case CSR_BT_AV_GET_CAPABILITIES_REQ:
            case CSR_BT_AV_SET_CONFIGURATION_REQ:
            case CSR_BT_AV_DELAY_REPORT_REQ:
                /* Ignore these primitives here because they're not stream but connection related */
            default:
            { /* Ignore */
                break;
            }
        }

        if(CsrPmemFreePrim)
        {
            CsrPmemFree(pMsg);
        }
        else
        {
            CsrMessageQueuePush(&tempQueue, eventClass, pMsg);
        }
    }
    con->pendingSigQueue = tempQueue;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvClearStreamQ
 *
 *  DESCRIPTION
 *      Free memory hanging on stream queue
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvClearStreamQ(av_stream_info_t *stream)
{
    CsrUintFast8 i;

    stream->sentBufFullInd = FALSE;

    for( i=0; i<CSR_BT_AV_MEDIA_BUFFER_SIZE; i++)
    {
        if( stream->fifoQ[i] != NULL)
        {
            CsrMblkDestroy(((CsrBtCmL2caDataReq *) stream->fifoQ[i])->payload);
            CsrPmemFree(stream->fifoQ[i]);
            stream->fifoQ[i] = NULL;
        }
    }
        stream->fifoQIn = 0;
        stream->fifoQOut = 0;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvClearStream
 *
 *  DESCRIPTION
 *      Clear/reset stream information, disconnect/cancel connect accept
 *      depending on state
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvClearStream(av_instData_t *instData, CsrUint8 s)
{
    if(instData->stream[s].timerId != 0)
    {
        CsrSchedTimerCancel(instData->stream[s].timerId, NULL, NULL);
        instData->stream[s].timerId = 0;
    }

#if defined(AV_STREAM_INACTIVITY_LP_ENABLE) && (AV_STREAM_INACTIVITY_LP_ENABLE > 0)
    if(instData->stream[s].lpTimerId != 0)
    {
        CsrSchedTimerCancel(instData->stream[s].lpTimerId, NULL, NULL);
        instData->stream[s].lpTimerId = 0;
    }
#endif

    CsrBtAvClearStreamQ(&instData->stream[s]);

    CsrBtAvClearStreamSignaling(&instData->con[instData->stream[s].conId], s);

    if( instData->stream[s].mediaCid != 0 )
    {
        if( instData->stream[s].streamState != PEER_ABORTING_S)
        {   /* Do not disconnect if remote is aborting */
            CsrBtCml2caDisconnectReqSend(instData->stream[s].mediaCid);
        }
    }
    else
    {
        if (instData->stream[s].streamState == PEER_OPENING_S)
        {
            CsrBtCml2caCancelConnectAcceptReqSend(CSR_BT_AV_IFACEQUEUE, CSR_BT_AVDTP_PSM);
            instData->stream[s].streamState = TERMINATE_PEER_OPENING_S; /* Use this state even when handling an ABORT CMD */
        }
        else if ( instData->stream[s].streamState == OPENING_S)
        {
            CsrBtCml2caCancelConnectReqSend(CSR_BT_AV_IFACEQUEUE,
                                            instData->con[instData->stream[s].conId].remoteDevAddr,
                                            CSR_BT_AVDTP_PSM);
            
            instData->stream[s].streamState = TERMINATE_OPENING_S;
        }
        else
        {
            /* time to recycle the stream info */
            if (CSR_BT_A2DP_BIT_RATE_STREAM_CLOSED != instData->stream[s].bitRate)
            {
                CsrBtCmA2dpBitRateReqSend(instData->con[instData->stream[s].conId].remoteDevAddr,s,CSR_BT_A2DP_BIT_RATE_STREAM_CLOSED);
            }
            
            CsrMemSet(&instData->stream[s], 0, sizeof(av_stream_info_t));
            instData->stream[s].tLabel                  = 0xFF;
            instData->stream[s].streamState             = IDLE_S;
            instData->stream[s].bitRate  = CSR_BT_A2DP_BIT_RATE_STREAM_CLOSED;
        }
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvIsCleanUpFinished
 *
 *  DESCRIPTION
 *      Check if a cleanup has been completed, enter READY state when done
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvIsCleanUpFinished(av_instData_t *instData)
{
    CsrUintFast8 i;

    for( i=0; i<4; i++)
    {
        if( instData->serviceRecordHandles[i] != 0 )
        {
            return;
        }
    }

    for(i=0; i<CSR_BT_AV_MAX_NUM_CONNECTIONS; i++)
    {
        if(instData->con[i].conState != DISCONNECTED_S)
        {
            return;
        }
    }

    /* if reaching here, then everything should have been cleaned,
       - start providing service again */
    instData->state = READY_S;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvSendHouseCleaning
 *
 *  DESCRIPTION
 *      Send a 'house cleaning' message to AV itself
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvSendHouseCleaning(av_instData_t *instData)
{
    CsrBtAvHouseCleaning    *prim;

    prim = (CsrBtAvHouseCleaning *) CsrPmemAlloc(sizeof(CsrBtAvHouseCleaning));
    prim->type = CSR_BT_AV_HOUSE_CLEANING;
    instData->restoreFlag = TRUE;
    CsrBtAvMessagePut(CSR_BT_AV_IFACEQUEUE, prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvSaveMessage
 *
 *  DESCRIPTION
 *      Put a signal on the save queue
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvSaveMessage(av_instData_t *instData)
{
    CsrMessageQueuePush(&instData->saveQueue, CSR_BT_AV_PRIM, instData->recvMsgP);
    instData->recvMsgP = NULL;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      getNumActivations
 *
 *  DESCRIPTION
 *      Find the total number of current activations
 *
 *  RETURNS
 *      Number of successful activations made by the application
 *
 *---------------------------------------------------------------------------*/
CsrUint8 getNumActivations(CsrUint8 *roleRegister)
{
    return (roleRegister[0] + roleRegister[1] + roleRegister[2] + roleRegister[3]);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      getNumIncomingCon
 *
 *  DESCRIPTION
 *      Finds current number of incoming connections(initiated by peer device)
 *
 *  RETURNS
 *      Number of current incoming connections
 *
 *---------------------------------------------------------------------------*/
CsrUint8 getNumIncomingCon(av_instData_t *instData)
{
    CsrUintFast8 i, t;

    t = 0;

    for(i=0; i<CSR_BT_AV_MAX_NUM_CONNECTIONS; i++)
    {
        if( (instData->con[i].incoming == TRUE) && (instData->con[i].conState != DISCONNECTED_S))
        {
            t++;
        }
    }

    return (CsrUint8)t;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvMakeConnectable
 *
 *  DESCRIPTION
 *      Send a CSR_BT_CM_L2CA_CONNECT_ACCEPT_REQ based on registered services, making
 *      AV connectable
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvMakeConnectable(av_instData_t *instData)
{
    CsrBtClassOfDevice cod = 0;
    uuid16_t        uuid;

    if (instData->serviceRecordHandles[CSR_BT_AV_AUDIO_SOURCE] || instData->serviceRecordHandles[CSR_BT_AV_VIDEO_SOURCE])
    {
        cod |= CSR_BT_CAPTURING_MAJOR_SERVICE_MASK;
    }

    if (instData->serviceRecordHandles[CSR_BT_AV_AUDIO_SINK] || instData->serviceRecordHandles[CSR_BT_AV_VIDEO_SINK])
    {
        cod |= CSR_BT_RENDERING_MAJOR_SERVICE_MASK;
    }

    if (instData->serviceRecordHandles[CSR_BT_AV_AUDIO_SOURCE] || instData->serviceRecordHandles[CSR_BT_AV_AUDIO_SINK])
    {
        uuid = CSR_BT_ADVANCED_AUDIO_PROFILE_UUID;
    }
    else
    {/* Only VDP is supported */
        uuid = CSR_BT_VIDEO_DISTRIBUTION_UUID;
    }

    CsrBtCml2caConnectAcceptReqSend(CSR_BT_AV_IFACEQUEUE,
                               CSR_BT_AVDTP_PSM,
                               cod,
                               instData->secIncoming,
                               CSR_BT_AV_PROFILE_DEFAULT_MTU_SIZE,
                               uuid);

    instData->isConnectable = TRUE;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvRegisterStreamHandleCfmSend
 *
 *  DESCRIPTION
 *      Build and send primitive
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvRegisterStreamHandleCfmSend(CsrSchedQid handle)
{
    CsrBtAvRegisterStreamHandleCfm *prim;

    prim        = (CsrBtAvRegisterStreamHandleCfm*)CsrPmemAlloc(sizeof(CsrBtAvRegisterStreamHandleCfm));
    prim->type  = CSR_BT_AV_REGISTER_STREAM_HANDLE_CFM;
    CsrBtAvMessagePut(handle, prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvSecurityInCfmSend
 *
 *  DESCRIPTION
 *      Send confirm on security changes
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtAvSecurityInCfmSend(CsrSchedQid appHandle, CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier)
{
    CsrBtAvSecurityInCfm *prim;
    prim = (CsrBtAvSecurityInCfm*)CsrPmemAlloc(sizeof(CsrBtAvSecurityInCfm));
    prim->type = CSR_BT_AV_SECURITY_IN_CFM;
    prim->resultCode = resultCode;
    prim->resultSupplier = resultSupplier;
    CsrBtAvMessagePut(appHandle, prim);
}

void CsrBtAvSecurityOutCfmSend(CsrSchedQid appHandle, CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier)
{
    CsrBtAvSecurityOutCfm *prim;
    prim = (CsrBtAvSecurityOutCfm*)CsrPmemAlloc(sizeof(CsrBtAvSecurityOutCfm));
    prim->type = CSR_BT_AV_SECURITY_OUT_CFM;
    prim->resultCode = resultCode;
    prim->resultSupplier = resultSupplier;
    CsrBtAvMessagePut(appHandle, prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvActivateCfmSend
 *
 *  DESCRIPTION
 *      Build and send primitive
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvActivateCfmSend(CsrSchedQid handle,
    CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier)
{
    CsrBtAvActivateCfm *prim;

    prim            = (CsrBtAvActivateCfm *) CsrPmemAlloc(sizeof(CsrBtAvActivateCfm));
    prim->type      = CSR_BT_AV_ACTIVATE_CFM;
    prim->avResultCode     = resultCode;
    prim->avResultSupplier = resultSupplier;

    CsrBtAvMessagePut(handle, prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvDeactivateCfmSend
 *
 *  DESCRIPTION
 *      Build and send primitive
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvDeactivateCfmSend(CsrSchedQid handle, CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier)
{
    CsrBtAvDeactivateCfm        *prim;

    prim            = (CsrBtAvDeactivateCfm *) CsrPmemAlloc(sizeof(CsrBtAvDeactivateCfm));
    prim->type      = CSR_BT_AV_DEACTIVATE_CFM;
    prim->avResultCode     = resultCode;
    prim->avResultSupplier = resultSupplier;
    CsrBtAvMessagePut(handle, prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvStatusIndSend
 *
 *  DESCRIPTION
 *      Build and send primitive
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvStatusIndSend(av_instData_t *instData, CsrBtAvPrim signalId, CsrBtAvRole role, CsrUint8 conId)
{
    if( (instData->streamAppHandle != CSR_SCHED_QID_INVALID) && (instData->streamAppHandle != instData->appHandle) )
    {
        CsrBtAvStatusInd *prim;

        prim                = (CsrBtAvStatusInd *) CsrPmemAlloc(sizeof(CsrBtAvStatusInd));
        prim->type          = CSR_BT_AV_STATUS_IND;
        prim->connectionId  = conId;
        prim->statusType    = signalId;
        prim->roleType      = role;
        prim->appHandle     = instData->appHandle;

        CsrBtAvMessagePut(instData->streamAppHandle, prim);
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvConnectCfmSend
 *
 *  DESCRIPTION
 *      Build and send primitive
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvConnectCfmSend(CsrSchedQid handle,
                           CsrUint8 connectionId,
                           CsrBtDeviceAddr devAddr,
                           CsrBtResultCode resultCode,
                           CsrBtSupplier resultSupplier,
                           CsrBtConnId btConnId)
{
    CsrBtAvConnectCfm *prim;

    prim                = (CsrBtAvConnectCfm *) CsrPmemAlloc(sizeof(CsrBtAvConnectCfm));
    prim->type          = CSR_BT_AV_CONNECT_CFM;
    prim->avResultCode     = resultCode;
    prim->avResultSupplier = resultSupplier;
    prim->connectionId  = connectionId;
    prim->deviceAddr    = devAddr;
    prim->btConnId      = btConnId;

    CsrBtAvMessagePut(handle, prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvConnectIndSend
 *
 *  DESCRIPTION
 *      Build and send primitive
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvConnectIndSend(CsrSchedQid handle,
                           CsrUint8 connectionId,
                           CsrBtDeviceAddr devAddr,
                           CsrBtConnId btConnId)
{
    CsrBtAvConnectInd *prim;

    prim                = (CsrBtAvConnectInd *) CsrPmemAlloc(sizeof(CsrBtAvConnectInd));
    prim->type          = CSR_BT_AV_CONNECT_IND;
    prim->connectionId  = connectionId;
    prim->deviceAddr    = devAddr;
    prim->btConnId      = btConnId;

    CsrBtAvMessagePut(handle, prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvDisconnectIndSend
 *
 *  DESCRIPTION
 *      Build and send primitive
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvDisconnectIndSend(CsrSchedQid handle, CsrUint8 connectionId,
    CsrBool localTerminated,
    CsrBtReasonCode reasonCode,
    CsrBtSupplier reasonSupplier)
{
    CsrBtAvDisconnectInd *prim;

    prim                = (CsrBtAvDisconnectInd *) CsrPmemAlloc(sizeof(CsrBtAvDisconnectInd));
    prim->type          = CSR_BT_AV_DISCONNECT_IND;
    prim->connectionId  = connectionId;
    prim->localTerminated = localTerminated;
    prim->reasonCode      = reasonCode;
    prim->reasonSupplier  = reasonSupplier;

    CsrBtAvMessagePut(handle, prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvStreamMtuSizeIndSend
 *
 *  DESCRIPTION
 *      Build and send primitive
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvStreamMtuSizeIndSend(av_instData_t *instData, CsrUint8 shandle, CsrBtConnId btConnId)
{
    CsrBtAvStreamMtuSizeInd *prim;

    prim                = (CsrBtAvStreamMtuSizeInd *) CsrPmemAlloc(sizeof(CsrBtAvStreamMtuSizeInd));
    prim->type          = CSR_BT_AV_STREAM_MTU_SIZE_IND;
    prim->shandle       = shandle;
    prim->remoteMtuSize = instData->stream[shandle].mediaMtu;
    prim->btConnId      = btConnId;

    if(instData->streamAppHandle != CSR_SCHED_QID_INVALID)
    {
        CsrBtAvMessagePut(instData->streamAppHandle, prim);
    }
    else
    {
        CsrBtAvMessagePut(instData->appHandle, prim);
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvDiscoverCfmSendReject
 *
 *  DESCRIPTION
 *      Build and send primitive
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvDiscoverCfmSendReject(av_instData_t *instData, CsrUint8 conId, CsrUint8 label,
    CsrBtResultCode avResultCode, CsrBtSupplier avResultSupplier)
{
    CsrBtAvDiscoverCfm *prim;

    prim                = (CsrBtAvDiscoverCfm *) CsrPmemAlloc(sizeof(CsrBtAvDiscoverCfm));
    prim->type          = CSR_BT_AV_DISCOVER_CFM;
    prim->connectionId  = conId;
    prim->tLabel        = label;
    prim->avResultCode     = avResultCode;
    prim->avResultSupplier = avResultSupplier;
    prim->seidInfoCount = 0;
    prim->seidInfo      = NULL;

    PUT_PRIM_TO_APP(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvGetCapabilitiesCfmSendReject
 *
 *  DESCRIPTION
 *      Build and send primitive
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvGetCapabilitiesCfmSendReject(av_instData_t *instData, CsrUint8 conId, CsrUint8 label,
    CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    CsrBtAvGetCapabilitiesCfm *prim;

    prim                = (CsrBtAvGetCapabilitiesCfm *) CsrPmemAlloc(sizeof(CsrBtAvGetCapabilitiesCfm));
    prim->type          = CSR_BT_AV_GET_CAPABILITIES_CFM;
    prim->connectionId  = conId;
    prim->tLabel        = label;
    prim->avResultCode     = resultCode;
    prim->avResultSupplier = resultSupplier;
    prim->servCapLen    = 0;
    prim->servCapData   = NULL;

    PUT_PRIM_TO_APP(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvSetConfigurationCfmSend
 *
 *  DESCRIPTION
 *      Build and send primitive
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvSetConfigurationCfmSend(av_instData_t *instData,
                               CsrUint8 conId,
                               CsrUint8 label,
                               CsrUint8 shandle,
                               CsrUint8 servCategory,
                               CsrBtResultCode resultCode,
                               CsrBtSupplier resultSupplier)
{
    CsrBtAvSetConfigurationCfm *prim;

    prim                = (CsrBtAvSetConfigurationCfm *) CsrPmemAlloc(sizeof(CsrBtAvSetConfigurationCfm));
    prim->type          = CSR_BT_AV_SET_CONFIGURATION_CFM;
    prim->connectionId  = conId;
    prim->tLabel        = label;
    prim->avResultCode     = resultCode;
    prim->avResultSupplier = resultSupplier;
    prim->shandle       = shandle;
    prim->servCategory  = servCategory;

    PUT_PRIM_TO_APP(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvGetConfigurationCfmSend
 *
 *  DESCRIPTION
 *      Build and send primitive
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvGetConfigurationCfmSend(av_instData_t *instData, CsrUint8 label, CsrUint8 shandle, CsrUint16 servCapLen, CsrUint8 *servCapData,
    CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    CsrBtAvGetConfigurationCfm *prim;

    prim                = (CsrBtAvGetConfigurationCfm *) CsrPmemAlloc(sizeof(CsrBtAvGetConfigurationCfm));
    prim->type          = CSR_BT_AV_GET_CONFIGURATION_CFM;
    prim->shandle       = shandle;
    prim->tLabel        = label;
    prim->servCapLen    = servCapLen;
    prim->servCapData   = servCapData;
    prim->avResultCode     = resultCode;
    prim->avResultSupplier = resultSupplier;

    PUT_PRIM_TO_APP(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvReconfigureCfmSend
 *
 *  DESCRIPTION
 *      Build and send primitive
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvReconfigureCfmSend(av_instData_t *instData, CsrUint8 label, CsrUint8 shandle, CsrUint8 servCategory,
    CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier)
{
    CsrBtAvReconfigureCfm *prim;

    prim                = (CsrBtAvReconfigureCfm *) CsrPmemAlloc(sizeof(CsrBtAvReconfigureCfm));
    prim->type          = CSR_BT_AV_RECONFIGURE_CFM;
    prim->shandle       = shandle;
    prim->tLabel        = label;
    prim->avResultCode     = resultCode;
    prim->avResultSupplier = resultSupplier;
    prim->servCategory  = servCategory;

    PUT_PRIM_TO_APP(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvOpenCfmSend
 *
 *  DESCRIPTION
 *      Build and send primitive
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvOpenCfmSend(av_instData_t *instData, CsrUint8 shandle, CsrUint8 label,
    CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    CsrBtAvOpenCfm *prim;

    prim                = (CsrBtAvOpenCfm *) CsrPmemAlloc(sizeof(CsrBtAvOpenCfm));
    prim->type          = CSR_BT_AV_OPEN_CFM;
    prim->shandle       = shandle;
    prim->tLabel        = label;
    prim->avResultCode     = resultCode;
    prim->avResultSupplier = resultSupplier;

    PUT_PRIM_TO_APP(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvStartCfmSend
 *
 *  DESCRIPTION
 *      Build and send primitive
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvStartCfmSend(av_instData_t *instData,
                    CsrUint8 shandle,
                    CsrUint8 label,
                    CsrBtResultCode resultCode,
                    CsrBtSupplier resultSupplier)
{
    CsrBtAvStartCfm *prim;

    prim                   = (CsrBtAvStartCfm *) CsrPmemAlloc(sizeof(CsrBtAvStartCfm));
    prim->type             = CSR_BT_AV_START_CFM;
    if (shandle < CSR_BT_AV_MAX_NUM_STREAMS)
    {
        prim->connectionId = instData->stream[shandle].conId;
    }
    else
    {
        prim->connectionId = 0;
    }
    prim->tLabel           = label;
    prim->avResultCode     = resultCode;
    prim->avResultSupplier = resultSupplier;
    prim->reject_shandle = shandle;

    PUT_PRIM_TO_APP(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvCloseCfmSend
 *
 *  DESCRIPTION
 *      Build and send primitive
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvCloseCfmSend(av_instData_t *instData,
                    CsrUint8 shandle,
                    CsrUint8 label,
                    CsrBtResultCode resultCode,
                    CsrBtSupplier resultSupplier)
{
    CsrBtAvCloseCfm *prim;

    prim                = (CsrBtAvCloseCfm *) CsrPmemAlloc(sizeof(CsrBtAvCloseCfm));
    prim->type          = CSR_BT_AV_CLOSE_CFM;
    prim->tLabel        = label;
    prim->avResultCode     = resultCode;
    prim->avResultSupplier = resultSupplier;
    prim->shandle       = shandle;

    PUT_PRIM_TO_APP(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvSuspendCfmSend
 *
 *  DESCRIPTION
 *      Build and send primitive
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvSuspendCfmSend(av_instData_t *instData,
                      CsrUint8 shandle,
                      CsrUint8 label,
                      CsrBtResultCode resultCode,
                      CsrBtSupplier resultSupplier)
{
    CsrBtAvSuspendCfm *prim;

    prim                    = (CsrBtAvSuspendCfm *) CsrPmemAlloc(sizeof(CsrBtAvSuspendCfm));
    prim->type              = CSR_BT_AV_SUSPEND_CFM;
    if (shandle < CSR_BT_AV_MAX_NUM_STREAMS)
    {
        prim->connectionId   = instData->stream[shandle].conId;
    }
    else
    {
        prim->connectionId   = 0;
    }
    prim->tLabel         = label;
    prim->reject_shandle = shandle;
    prim->avResultCode   = resultCode;
    prim->avResultSupplier = resultSupplier;

    PUT_PRIM_TO_APP(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvAbortCfmSend
 *
 *  DESCRIPTION
 *      Build and send primitive
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvAbortCfmSend(av_instData_t *instData, CsrUint8 shandle, CsrUint8 label)
{
    CsrBtAvAbortCfm *prim;

    prim                = (CsrBtAvAbortCfm *) CsrPmemAlloc(sizeof(CsrBtAvAbortCfm));
    prim->type          = CSR_BT_AV_ABORT_CFM;
    prim->shandle       = shandle;
    prim->tLabel        = label;

    PUT_PRIM_TO_APP(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvSecurityControlCfmSend
 *
 *  DESCRIPTION
 *      Build and send primitive
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvSecurityControlCfmSend(av_instData_t *instData,
                              CsrUint8 shandle,
                              CsrUint8 label,
                              CsrUint16 length,
                              CsrUint8 *data,
                              CsrBtResultCode resultCode,
                              CsrBtSupplier resultSupplier)
{
    CsrBtAvSecurityControlCfm *prim;

    prim                = (CsrBtAvSecurityControlCfm *) CsrPmemAlloc(sizeof(CsrBtAvSecurityControlCfm));
    prim->type          = CSR_BT_AV_SECURITY_CONTROL_CFM;
    prim->tLabel        = label;
    prim->avResultCode     = resultCode;
    prim->avResultSupplier = resultSupplier;
    prim->shandle       = shandle;
    prim->contProtMethodLen = length;
    prim->contProtMethodData = data;

    PUT_PRIM_TO_APP(prim);
}


/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvQosIndSend
 *
 *  DESCRIPTION
 *      Build and send primitive
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvQosIndSend(av_instData_t *instData, CsrUint8 shandle, CsrUint16 bufferStatus)
{
    CsrBtAvQosInd * prim;

    prim = (CsrBtAvQosInd *) CsrPmemAlloc(sizeof(CsrBtAvQosInd));
    prim->type = CSR_BT_AV_QOS_IND;
    prim->shandle = shandle;
    prim->bufferStatus = bufferStatus;

    PUT_PRIM_TO_APP(prim);
}

#ifdef CSR_BT_INSTALL_AV_SET_QOS_INTERVAL
void CsrBtAvSetQosIntervalReqHandler(av_instData_t *instData)
{
    CsrBtAvSetQosIntervalReq *prim;

    prim = (CsrBtAvSetQosIntervalReq *) instData->recvMsgP;
    instData->qosInterval = prim->qosInterval;
}
#endif


void CsrBtAvLpNegConfigReqHandler(av_instData_t *instData)
{
    CsrBtAvLpNegConfigReq *prim;

    prim = (CsrBtAvLpNegConfigReq *) instData->recvMsgP;
    instData->allowStartLpNeg = prim->enable;
}


static CsrBool csrBtAvSdpGetRemoteAvdtpVersion( CmnCsrBtLinkedListStruct *sdpTagList,
                                                CsrUint16  serviceHandleIndex,
                                                CsrUint16  *version)
{
    CsrUint16    tmp;
    CsrUint32    uuid, value, protocol;
    CsrUint16    dummy1, dummy2;

    if (CsrBtUtilSdrGetServiceUuid32AndResult(sdpTagList,
            serviceHandleIndex,
            &uuid,
            &tmp,
            &dummy1,
            &dummy2))
    {
        if (CSR_BT_RESULT_CODE_CM_SUCCESS == tmp)
        {
            CsrUint16 attDataLen, emptyAttSize, consumedBytes, index = 0;
            CsrUint8 *att_p = CsrBtUtilSdrGetAttributePointer(sdpTagList, serviceHandleIndex, 0, &dummy1);

            CsrBtUtilSdrGetEmptyAttributeSize(&emptyAttSize);

            CsrMemCpy(&tmp, att_p, SDR_ENTRY_SIZE_SERVICE_UINT16);
            attDataLen = tmp - emptyAttSize + SDR_ENTRY_SIZE_TAG_LENGTH;

            if ( CsrBtUtilSdpExtractUint(att_p + SDR_ENTRY_INDEX_ATTRIBUTE_DATA + index,
                 attDataLen,
                 &value,
                 &consumedBytes,
                 TRUE))
            {
                attDataLen -= consumedBytes;
                index += consumedBytes;

                while( attDataLen > 0 )
                {
                    if ( CsrBtUtilSdpExtractUint(att_p + SDR_ENTRY_INDEX_ATTRIBUTE_DATA + index,
                         attDataLen,
                         &protocol,
                         &consumedBytes,
                         TRUE))
                    {
                        attDataLen -= consumedBytes;
                        index += consumedBytes;

                        if( CsrBtUtilSdpExtractUint(att_p + SDR_ENTRY_INDEX_ATTRIBUTE_DATA + index,
                                attDataLen,
                                &value,
                                &consumedBytes,
                                TRUE))
                        {
                            attDataLen -= consumedBytes;
                            index += consumedBytes;
                            
                            /* Now find the value if it is AVDTP protocol UUID */
                            if ( protocol == CSR_BT_PROTOCOL_AVDTP_UUID )
                            {
                                 *version = (CsrUint16)value;
                                 return TRUE;
                                
                            }
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }
    return FALSE;
}

void CsrBtAvSdcResultHandler(void                     * inst,
                             CmnCsrBtLinkedListStruct * sdpTagList,
                             CsrBtDeviceAddr          deviceAddr,
                             CsrBtResultCode          resultCode,
                             CsrBtSupplier            resultSupplier)
{
    av_instData_t *instData = (av_instData_t *) inst;
    av_connection_info_t    *con = &(instData->con[0]);
    CsrUintFast8  i = 0;
    CsrBool found = FALSE;

    for (i = 0; ((i < CSR_BT_AV_MAX_NUM_CONNECTIONS ) && (con != NULL)); i++)
    {
      con = &(instData->con[i]);
      if (con != NULL)
      {
          if (!CsrMemCmp(&con->remoteDevAddr,&deviceAddr, sizeof(CsrBtDeviceAddr)))
          {/* Found*/
              found = TRUE;
              break;
          }
      }
    }
    
    if(found && (instData->con[i].conState == CANCEL_CONNECTING_S))
    {
        /* Except if we cancelled the connection attempt. */
        CsrBtAvClearConnection(instData, (CsrUint8)i);
        CsrBtAvConnectCfmSend(instData->appHandle,
                              0, deviceAddr,
                              CSR_BT_RESULT_CODE_AV_CANCEL_CONNECT_ATTEMPT,
                              CSR_BT_SUPPLIER_AV,
                              CSR_BT_CONN_ID_INVALID);
    }
    else
    {
        /* Search operation started */
        CsrBtCml2caConnectReqSend(CSR_BT_AV_IFACEQUEUE,
                             deviceAddr,
                             CSR_BT_AVDTP_PSM,
                             CSR_BT_AVDTP_PSM,
                             CSR_BT_AV_PROFILE_DEFAULT_MTU_SIZE,
                             instData->secOutgoing);

        if (resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS &&
            resultSupplier == CSR_BT_SUPPLIER_CM  &&
            sdpTagList != NULL)
        {
            CsrUintFast16    numOfSdpRecords = CsrBtUtilBllGetNofEntriesEx(sdpTagList);
            CsrUintFast16    sdpRecordIndex;
            CsrUint16    tmpResult;
            CsrUint16    dummy1, dummy2; /* Currently CSR_UNUSED */
            CsrUint16    version = 0;
            CsrBtUuid32  tmpUuid = 0;

            for (sdpRecordIndex = 0; sdpRecordIndex < numOfSdpRecords; sdpRecordIndex++)
            {
                if (CsrBtUtilSdrGetServiceUuid32AndResult(sdpTagList,
                                                sdpRecordIndex,
                                                &tmpUuid,
                                                &tmpResult,
                                                &dummy1,
                                                &dummy2))
                {
                    if (tmpResult == SDR_SDC_SEARCH_SUCCESS)
                    {
                        if ((tmpUuid == CSR_BT_AUDIO_SOURCE_UUID) || (tmpUuid == CSR_BT_AUDIO_SINK_UUID) ||
                            (tmpUuid == CSR_BT_VIDEO_SOURCE_UUID) || (tmpUuid == CSR_BT_VIDEO_SINK_UUID))
                        { 
                            if (TRUE == csrBtAvSdpGetRemoteAvdtpVersion(sdpTagList, (CsrUint16)sdpRecordIndex, &version))
                            {
                                con->remoteAVDTPVersion = version;

                                /* workaround to avoid IOP issues with a larger range of SE handsets
                                   incorrectly reporting AVDTP version 10.0 instead of 1.0
                                   (K800x, K610x, K618x, V630x, K790x, W712x, Z712x, W710x, Z710x,
                                   K610im, Z610x, W850x, W830x, W880x, W888x, K550x, W610, W660x,
                                   K810x, K818x, K530x, S500x, W580x, T650x, T658x, K770x) */
                                if ( version == 0x1000 )
                                {
                                    con->remoteAVDTPVersion = 0x0100;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    CsrBtUtilBllFreeLinkedList(&sdpTagList, CsrBtUtilBllPfreeWrapper);
}


/*----------------------------------------------------------------------------*
 *  NAME
 *      csrBtAvCalculateStreamBitRate
 *
 *  DESCRIPTION
 *      calculates the bit rate to use for the stream connection and returns
 *      the result or the "Unknown" value if not possible to calculate.
 *      Indicates the bit rate to the CM that will propagate the value to
 *      any application that subscribes to that information.
 *
 *  RETURNS
 *      CsrUint32 bit rate
 *
 *---------------------------------------------------------------------------*/
CsrUint32 csrBtAvCalculateStreamBitRate(av_instData_t *instData, CsrUint8 *servCap, CsrUint16 servCapLen, CsrUint8 strIdx)
{
    CsrUint32 bitRate = CSR_BT_A2DP_BIT_RATE_UNKNOWN;
    CsrBool found = FALSE;
    CsrUint16 index = 0;

    servCap = CsrBtAvGetServiceCap(CSR_BT_AV_SC_MEDIA_CODEC,
                                   servCap,
                                   servCapLen,
                                   &index);

    if (servCap && servCap[CSR_BT_AV_MEDIA_TYPE_INDEX] == CSR_BT_AV_AUDIO_MEDIA_TYPE)
    {/* Media type data found */
        CsrUint8 audioCodec = servCap[CSR_BT_AV_CODEC_TYPE_INDEX]>>4;
        found = TRUE;

        switch (audioCodec)
        {
            case CSR_BT_AV_AUDIO_SBC_CODEC: /* 0 */
            {   /*
                   bit rate = 8 * frame_length * sampling freq / no. subbands / no. blocks

                   If (Mono or Dual channel channel mode)
                     frame length = 4 + (4 * no. subbands * no. channels) / 8 + [no. blocks * no. channels * bitpool / 8]
                   Else (--> stereo and joint stereo)
                     frame length = 4 + (4 * no. subbands * no. channels) / 8 + [(join * no. subbands + no. blocks * bitpool) / 8]
                            (join = 1 if joint stereo channel mode; otherwise 0).
                */
                CsrUint32 frame_length = 0;  /* In bytes */
                CsrUint32 sampling_freq = 0; /* In Hz */
                CsrUint8 subBands = 0, blocks = 0, channels = 0, bitpool = 0, channel_mode = 0;
                /* First get the data out of the capabilities received */
                    /* sampling frequency */
                switch (servCap[CSR_BT_AV_SFREQ_AND_CMODE_INDEX] & CSR_BT_AV_HIGH_NIBBLE_MASK)
                {
                    case 0x10:
                        sampling_freq = 48000;
                    break;

                    case 0x20:
                        sampling_freq = 44100;
                    break;

                    case 0x40:
                        sampling_freq = 32000;
                    break;

                    case 0x80:
                    default:
                        sampling_freq = 16000;
                    break;
                }
                    /* channel mode */
                channel_mode = (servCap[CSR_BT_AV_SFREQ_AND_CMODE_INDEX] & CSR_BT_AV_LOW_NIBBLE_MASK);
                if (channel_mode == CSR_BT_AV_MONO_CMODE)
                {
                    channels = 1;
                }
                else
                {
                    channels = 2;
                }
                    /* no. blocks */
                switch (servCap[CSR_BT_AV_BLOCKS_SUBBAND_METHOD_INDEX] & CSR_BT_AV_HIGH_NIBBLE_MASK)
                {
                    case 0x10:
                        blocks = 16;
                    break;
                    case 0x20:
                        blocks = 12;
                    break;
                    case 0x40:
                        blocks = 8;
                    break;
                    case 0x80:
                    default:
                        blocks = 4;
                    break;
                }
                    /* no. of sub-bands */
                if ((servCap[CSR_BT_AV_BLOCKS_SUBBAND_METHOD_INDEX] & CSR_BT_AV_SUBBANDS_MASK) == CSR_BT_AV_4_SUBBANDS)
                {
                    subBands = 4;
                }
                else
                {
                    subBands = 8;
                }
                    /* max bitpool */
                bitpool = servCap[CSR_BT_AV_MAX_BITPOOL_INDEX];
                /* Now calculate frame length */
                if ((channel_mode == CSR_BT_AV_MONO_CMODE) || (channel_mode == CSR_BT_AV_DUAL_CMODE))
                {
                    frame_length = (CsrUint32) (4 + ((4 * subBands * channels) / 8) + ((blocks * channels * bitpool) / 8) );
                }
                else
                {
                    CsrUint8 join = 0;
                    if (channel_mode == CSR_BT_AV_JOINT_STEREO_CMODE)
                    {
                        join = 1;
                    }

                    frame_length = (CsrUint32) (4 + ((4 * subBands * channels) / 8) + (((join * subBands) + (blocks * bitpool)) / 8) );
                }
                /* Finally, calculate the max bit rate */
                bitRate = (CsrUint32)(((8 * frame_length * sampling_freq) / subBands) / blocks);
                break;
            }

            case CSR_BT_AV_AUDIO_MPEG_2_4_AAC_CODEC: /* 2 */
            {
                bitRate = (CsrUint32)(servCap[CSR_BT_AV_BIT_RATE_HIGH_IDX] & 0xEF);
                bitRate = (CsrUint32)((bitRate << 8) | (CsrUint32)(servCap[CSR_BT_AV_BIT_RATE_MED_IDX]));
                bitRate = (CsrUint32)((bitRate << 8) | (CsrUint32)(servCap[CSR_BT_AV_BIT_RATE_LOW_IDX]));
                break;
            }

            case CSR_BT_AV_NON_A2DP_CODEC:
            {
                CsrUint32 vendorId = (CsrUint32)((servCap[CSR_BT_AV_APTX_VENDOR_ID_LSB_INDEX+3] << 24) | (servCap[CSR_BT_AV_APTX_VENDOR_ID_LSB_INDEX+2] << 16) | (servCap[CSR_BT_AV_APTX_VENDOR_ID_LSB_INDEX+1] << 8) | servCap[CSR_BT_AV_APTX_VENDOR_ID_LSB_INDEX]);
                CsrUint16 codecId  = (CsrUint16)((servCap[CSR_BT_AV_APTX_CODEC_ID_LSB_INDEX+1] << 8) | servCap[CSR_BT_AV_APTX_CODEC_ID_LSB_INDEX]);
                /* Find out whether this is an AptX codec and if so, calculate the bitrate; if not, just ignore. */
                if (((vendorId == CSR_BT_AV_APTX_VENDOR_ID1) || (vendorId == CSR_BT_AV_APTX_VENDOR_ID2)) && 
                    ((codecId  == CSR_BT_AV_APTX_CODEC_ID1)  || (codecId  == CSR_BT_AV_APTX_CODEC_ID2)))
                {/* Aptx codec: calculate the bitrate. The bitrate is fixed, compression ratio 1:4 (f.ex. 352.8 kbps at 44.1 Khz)  */
                    
                    CsrUint32 sampling_freq = 0; /* In Hz */
                    /* extract the sampling frequency out of the capabilities received */
                    switch (servCap[CSR_BT_AV_APTX_SFREQ_AND_CMODE_INDEX] & CSR_BT_AV_HIGH_NIBBLE_MASK)
                    {
                        case 0x10:
                            sampling_freq = 48000;
                        break;

                        case 0x20:
                            sampling_freq = 44100;
                        break;

                        case 0x40:
                            sampling_freq = 32000;
                        break;

                        case 0x80:
                        default:
                            sampling_freq = 16000;
                        break;
                    }
                    /* calculate the bit rate */
                    bitRate = (CsrUint32)(8 * sampling_freq);
                }
                break;
            }
            case CSR_BT_AV_AUDIO_MPEG_1_2_CODEC: /* 1 */
            case CSR_BT_AV_AUDIO_ATRAC_CODEC: /* 4 */
            default:
                break;
        }
    }
    if ((bitRate != instData->stream[strIdx].bitRate) && found)
    {/* Indicate the change! */
        CsrBtCmA2dpBitRateReqSend(instData->con[instData->stream[strIdx].conId].remoteDevAddr,strIdx,bitRate);
    }
    return bitRate;
}

