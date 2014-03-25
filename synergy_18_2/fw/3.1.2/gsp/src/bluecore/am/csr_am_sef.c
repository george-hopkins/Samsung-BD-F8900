/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_sched.h"
#include "csr_log_text_2.h"

#include "csr_am_handler.h"
#include "csr_am_sef.h"
#include "csr_am_util.h"
#include "csr_am_prim.h"
#include "csr_am_task.h"
#include "csr_bccmd_lib.h"
#include "csr_log_text.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmInitCfmSend
 *
 *  DESCRIPTION
 *      Sends the CSR_AM_INIT_CFM message to the app handle
 *----------------------------------------------------------------------------*/
void CsrAmInitCfmSend(AmInstanceDataType *inst, CsrSchedQid appHandle, CsrResult result)
{
    CsrAmInitCfm *prim = (CsrAmInitCfm *) CsrPmemAlloc(sizeof(CsrAmInitCfm));

    prim->type = CSR_AM_INIT_CFM;
    prim->result = result;
    prim->buildId = inst->buildId;

    CsrSchedMessagePut(appHandle, CSR_AM_PRIM, prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmAudioPathConnectCfmSend
 *
 *  DESCRIPTION
 *      Sends the CSR_AM_AUDIO_PATH_CONNECT_CFM message to the app handle
 *----------------------------------------------------------------------------*/
void CsrAmAudioPathConnectCfmSend(AmInstanceDataType *inst, CsrResult result, CsrUint16 connId)
{
    CsrAmAudioPathConnectCfm *prim = (CsrAmAudioPathConnectCfm *) CsrPmemAlloc(sizeof(CsrAmAudioPathConnectCfm));

    prim->type = CSR_AM_AUDIO_PATH_CONNECT_CFM;
    prim->result = result;
    prim->amConnectionId = connId;

    CsrSchedMessagePut(inst->currentAppHandle, CSR_AM_PRIM, prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmAudioPathAddSinkCfmSend
 *
 *  DESCRIPTION
 *      Sends the CSR_AM_AUDIO_PATH_ADD_SINK_CFM message to the app handle
 *----------------------------------------------------------------------------*/
void CsrAmAudioPathAddSinkCfmSend(AmInstanceDataType *inst, CsrResult result, CsrUint16 connId)
{
    CsrAmAudioPathAddSinkCfm *prim = (CsrAmAudioPathAddSinkCfm *) CsrPmemAlloc(sizeof(CsrAmAudioPathAddSinkCfm));

    prim->type = CSR_AM_AUDIO_PATH_ADD_SINK_CFM;
    prim->result = result;
    prim->amConnectionId = connId;

    CsrSchedMessagePut(inst->currentAppHandle, CSR_AM_PRIM, prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmAudioPathRemoveSinkCfmSend
 *
 *  DESCRIPTION
 *      Sends the CSR_AM_AUDIO_PATH_REMOVE_SINK_CFM message to the app handle
 *----------------------------------------------------------------------------*/
void CsrAmAudioPathRemoveSinkCfmSend(AmInstanceDataType *inst, CsrResult result, CsrUint16 connId)
{
    CsrAmAudioPathRemoveSinkCfm *prim = (CsrAmAudioPathRemoveSinkCfm *) CsrPmemAlloc(sizeof(CsrAmAudioPathRemoveSinkCfm));

    prim->type = CSR_AM_AUDIO_PATH_REMOVE_SINK_CFM;
    prim->result = result;
    prim->amConnectionId = connId;

    CsrSchedMessagePut(inst->currentAppHandle, CSR_AM_PRIM, prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmAudioPathConfigCfmSend
 *
 *  DESCRIPTION
 *      Sends the CSR_AM_AUDIO_PATH_CONFIG_CFM message to the app handle
 *----------------------------------------------------------------------------*/
void CsrAmAudioPathConfigCfmSend(AmInstanceDataType *inst, CsrResult result, CsrUint16 connId)
{
    CsrAmAudioPathConfigCfm *prim = (CsrAmAudioPathConfigCfm *) CsrPmemAlloc(sizeof(CsrAmAudioPathConfigCfm));

    prim->type = CSR_AM_AUDIO_PATH_CONFIG_CFM;
    prim->result = result;
    prim->amConnectionId = connId;

    CsrSchedMessagePut(inst->currentAppHandle, CSR_AM_PRIM, prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmAudioPathReleaseCfmSend
 *
 *  DESCRIPTION
 *      Sends the CSR_AM_AUDIO_PATH_RELEASE_CFM message to the app handle
 *----------------------------------------------------------------------------*/
void CsrAmAudioPathReleaseCfmSend(AmInstanceDataType *inst, CsrResult result, CsrUint16 connId)
{
    CsrAmAudioPathReleaseCfm *prim = (CsrAmAudioPathReleaseCfm *) CsrPmemAlloc(sizeof(CsrAmAudioPathReleaseCfm));

    prim->type = CSR_AM_AUDIO_PATH_RELEASE_CFM;
    prim->result = result;
    prim->amConnectionId = connId;

    CsrSchedMessagePut(inst->currentAppHandle, CSR_AM_PRIM, prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmAudioPathReleaseIndSend
 *
 *  DESCRIPTION
 *      Sends the CSR_AM_AUDIO_PATH_RELEASE_IND message to the app handle
 *----------------------------------------------------------------------------*/
void CsrAmAudioPathReleaseIndSend(AmInstanceDataType *inst, CsrUint16 connId)
{
    CsrAmAudioPathReleaseInd *prim = (CsrAmAudioPathReleaseInd *) CsrPmemAlloc(sizeof(CsrAmAudioPathReleaseInd));

    prim->type = CSR_AM_AUDIO_PATH_RELEASE_IND;
    prim->amConnectionId = connId;

    CsrSchedMessagePut(inst->currentAppHandle, CSR_AM_PRIM, prim);
}

/****************** Handler functions ********************************/


/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmSaveMessage
 *
 *  DESCRIPTION
 *      Stores a message in the instance queue for later handling
 *----------------------------------------------------------------------------*/
void CsrAmSaveMessage(AmInstanceDataType *inst)
{
    CsrMessageQueuePush(&inst->saveQueue, inst->msgClass, inst->rxMsgP);
    inst->rxMsgP = NULL;
    inst->msgClass = 0;
    inst->restoreFlag = FALSE;
}

void CsrAmDiscardMessage(AmInstanceDataType *inst)
{
    CsrAmFreeDownstreamMessageContents(CSR_AM_PRIM, inst->rxMsgP);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmInitReqHandler
 *
 *  DESCRIPTION
 *      Handles the CSR_AM_INIT_REQ primitive
 *----------------------------------------------------------------------------*/
void CsrAmInitReqHandler(AmInstanceDataType *inst)
{
    CsrAmInitReq *prim = (CsrAmInitReq *) inst->rxDownstreamMsgP;
    if ((inst->chipVer >= 7) && (inst->buildId >= FIRST_AUDIO_STREAM_BUILD_ID)) /* Issue confimr message */
    {
        CsrAmInitCfmSend(inst, prim->pHandle, CSR_RESULT_SUCCESS);
    }
    else /* Not supported */
    {
        CsrAmInitCfmSend(inst, prim->pHandle, CSR_AM_RESULT_UNSUPPORTED);
    }
    CsrAmFreeOperationMessage(inst);
}

/* Same as above, but to be used in busy state to avoid additional if-statements to determine what to free */
void CsrAmInitReqBusyHandler(AmInstanceDataType *inst)
{
    CsrAmInitReq *prim = (CsrAmInitReq *) inst->rxMsgP;
    if ((inst->chipVer >= 7) && (inst->buildId >= FIRST_AUDIO_STREAM_BUILD_ID)) /* Issue confimr message */
    {
        CsrAmInitCfmSend(inst, prim->pHandle, CSR_RESULT_SUCCESS);
    }
    else /* Not supported */
    {
        CsrAmInitCfmSend(inst, prim->pHandle, CSR_AM_RESULT_UNSUPPORTED);
    }
    CsrAmFreeDownstreamMessageContents(CSR_AM_PRIM, prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmAudioPathConnectReqHandler
 *
 *  DESCRIPTION
 *      Handles the CSR_AM_AUDIO_PATH_CONNECT_REQ primitive
 *----------------------------------------------------------------------------*/
void CsrAmAudioPathConnectReqHandler(AmInstanceDataType *inst)
{
    CsrAmAudioPathConnectReq *prim = (CsrAmAudioPathConnectReq *) inst->rxDownstreamMsgP;
    CsrAmEndPointDefType *sinkEndPoint;
    CsrBool sinkFm = FALSE;
    CsrUint8 i;

    inst->currentAppHandle = prim->pHandle;

    for (i = 0; i < prim->sinks.sinkCount; i++)
    {
        sinkEndPoint = (CsrAmEndPointDefType *) &(prim->sinks.newSinkId[i]);
        if (sinkEndPoint->endPoint == CSR_AM_ENDPOINT_FM)
        {
            i = prim->sinks.sinkCount;
            sinkFm = TRUE;
        }
    }

    if ((inst->buildId == 5985) && (!sinkFm) && (prim->source.endPoint.endPoint != CSR_AM_ENDPOINT_FM)) /* BC7820 and not FM audio: not supported! FM must be either source or sink on BC7820; else the 'old' audio
        setup method has to be used */
    {
        CsrAmAudioPathConnectCfmSend(inst, CSR_AM_RESULT_UNSUPPORTED, CSR_AM_NO_CONN_ID);
        CsrAmFreeOperationMessage(inst);
    }
    else
    {
        StreamType *srcStream = NULL, *sinkStream = NULL;
        if (CsrAmIsSrcInUse(inst, prim->source, &srcStream))
        {
            CsrAmAudioPathConnectCfmSend(inst, CSR_AM_RESULT_ERROR_SRC_BUSY, CSR_AM_NO_CONN_ID);
            CsrAmFreeOperationMessage(inst);
        }
        else
        {
            for (i = 0; i < prim->sinks.sinkCount; i++)
            {
                sinkEndPoint = (CsrAmEndPointDefType *) &(prim->sinks.newSinkId[i]);
                if (CsrAmIsSinkInUse(inst, *sinkEndPoint, &sinkStream)) /* Sink busy */
                {
                    CsrAmAudioPathConnectCfmSend(inst, CSR_AM_RESULT_ERROR_SINK_BUSY, CSR_AM_NO_CONN_ID);
                    CsrAmFreeOperationMessage(inst);
                    return;
                }
            }

            /* Handle request */
            CSR_AM_CHANGE_STATE(inst->myState, AM_BUSY_STATE);
            if (prim->source.endPoint.instance == CSR_AM_INSTANCE_AUTO)
            {
                prim->source.endPoint.instance = CSR_AM_INSTANCE_0;
            }
            if (prim->source.endPoint.channel == CSR_AM_CHANNEL_AUTO)
            {
                prim->source.endPoint.channel = CSR_AM_CHANNEL_0;
            }
            /* Allocate the source and create the stream element. */
            CsrAmIssueBccmdSetSource(prim->source.endPoint.endPoint, prim->source.endPoint.instance, prim->source.endPoint.channel);
            inst->amCallback = CsrAmSrcAllocCfm;
            CsrAmCreateNewStreamElement(inst, prim);
        }
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmAudioPathAddSinkReqHandler
 *
 *  DESCRIPTION
 *      Handles the CSR_AM_AUDIO_PATH_ADD_SINK_REQ primitive
 *----------------------------------------------------------------------------*/
void CsrAmAudioPathAddSinkReqHandler(AmInstanceDataType *inst)
{
    CsrAmAudioPathAddSinkReq *prim = (CsrAmAudioPathAddSinkReq *) inst->rxDownstreamMsgP;

    inst->currentAppHandle = prim->pHandle;
    inst->currentStream = CsrAmFindStreamByAppConnId(inst, prim->amConnectionId);

    if (inst->currentStream == NULL) /* No stream with that connection Id found: error */
    {
        CsrAmAudioPathAddSinkCfmSend(inst, CSR_AM_RESULT_UNKNOWN_CONNECTION, prim->amConnectionId);
        CsrAmFreeOperationMessage(inst);
    }
    else /* If the requested sink is in use, reject this request */
    {
        CsrUint8 i;
        CsrAmEndPointDefType *sinkEndPoint;
        CsrAmSinksToAddType sinks = prim->sinks;
        StreamType *sinkStream = NULL;

        for (i = 0; i < sinks.sinkCount; i++)
        {
            sinkEndPoint = (CsrAmEndPointDefType *) &(sinks.newSinkId[i]);
            if (CsrAmIsSinkInUse(inst, *sinkEndPoint, &sinkStream)) /* Sink busy */
            {
                CsrAmAudioPathAddSinkCfmSend(inst, CSR_AM_RESULT_ERROR_SINK_BUSY, prim->amConnectionId);
                CsrAmFreeOperationMessage(inst);
                return;
            }
        }

        /* Handle request */
        CSR_AM_CHANGE_STATE(inst->myState, AM_BUSY_STATE);
        /* Create the list of new sinks to add and start with the first sink in the list */
        CsrAmCreateLinkOfSinksToAdd(inst);
        inst->sinksToAdd = (EndpointType *) CsrCmnListElementGetFirst(&inst->newSinkList);
        CsrAmIssueBccmdSetSink(inst->sinksToAdd->amStreamAudioEndPointType.endPoint,
            inst->sinksToAdd->amStreamAudioEndPointType.instance,
            inst->sinksToAdd->amStreamAudioEndPointType.channel);
        inst->amCallback = CsrAmSinkAllocCfm;
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmAudioPathRemoveSinkReqHandler
 *
 *  DESCRIPTION
 *      Handles the CSR_AM_AUDIO_PATH_REMOVE_SINK_REQ primitive
 *----------------------------------------------------------------------------*/
void CsrAmAudioPathRemoveSinkReqHandler(AmInstanceDataType *inst)
{
    CsrAmAudioPathRemoveSinkReq *prim = (CsrAmAudioPathRemoveSinkReq *) inst->rxDownstreamMsgP;

    inst->currentAppHandle = prim->pHandle;
    inst->currentStream = CsrAmFindStreamByAppConnId(inst, prim->amConnectionId);

    if (inst->currentStream == NULL) /* No stream with that connection Id found: error */
    {
        CsrAmAudioPathRemoveSinkCfmSend(inst, CSR_AM_RESULT_UNKNOWN_CONNECTION, prim->amConnectionId);
        CsrAmFreeOperationMessage(inst);
    }
    else
    {
        /* Is the sink ID part of this stream? */
        if (CsrAmFindStreamByAppSinkType(inst, prim->sinkId) != inst->currentStream) /* The sink requested is not part of the stream with the connection Id given */
        {
            CsrAmAudioPathRemoveSinkCfmSend(inst, CSR_AM_RESULT_UNKNOWN_CONNECTION, prim->amConnectionId);
            CsrAmFreeOperationMessage(inst);
        }
        else /* Handle request */
        {
            CSR_AM_CHANGE_STATE(inst->myState, AM_BUSY_STATE);
            /* Always handle the 'left' side first; in case of mono connections it will be the only one present.*/
            if ((inst->streamTransOpen) || (!inst->streamTransSupported))
            {
                CsrAmIssueBccmdStreamDisconnect(inst->currentStream->amStreamTransformIdL);
                inst->amCallback = CsrAmStreamDisconnectCfm;
            }
            else
            {
                CsrAmIssueBccmdStreamTransStart();
                inst->amCallback = CsrAmStreamTransStartCfm;
            }
        }
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmAudioPathConfigReqHandler
 *
 *  DESCRIPTION
 *      Handles the CSR_AM_AUDIO_PATH_CONFIG_REQ primitive
 *----------------------------------------------------------------------------*/
void CsrAmAudioPathConfigReqHandler(AmInstanceDataType *inst)
{
    CsrAmAudioPathConfigReq *prim = (CsrAmAudioPathConfigReq *) inst->rxDownstreamMsgP;

    inst->currentAppHandle = prim->pHandle;
    inst->currentStream = CsrAmFindStreamByAppConnId(inst, prim->amConnectionId);

    if (inst->currentStream == NULL) /* No stream with that connection Id found: error */
    {
        CsrAmAudioPathConfigCfmSend(inst, CSR_AM_RESULT_UNKNOWN_CONNECTION, prim->amConnectionId);
        CsrAmFreeOperationMessage(inst);
    }
    else /* Handle request */
    {
        /* Do not change to AM_BUSY_STATE unless the configuration command is actually issued!
           If ONLY audio gain is to be configured, don't need to start transactions and/or disconnect
           the stream; just configure the gain.
           Likewise, if the endpoint to configure is not connected, it is only needed to configure it,
           no matter what key or keys need to be configured. The latter is though irrelevant here as the
           AM API demands that the configuration request contains a connection ID, so only endpoints in an
           existing connection can be configured....
        */
        /* First find the endpoint or endpoints to configure;
           then copy the configuration information into the endpoint structure;
           finally configure the endpoint(s)
        */
        switch (prim->epType)
        {
            case CSR_AM_EP_SOURCE_LEFT:
            case CSR_AM_EP_SOURCE_RIGHT:
            case CSR_AM_EP_SOURCE_ALL:
            {
                CSR_AM_CHANGE_STATE(inst->myState, AM_BUSY_STATE);
                inst->currentStream->amSource.amStreamAudioEndPointConfigLen = (CsrUint8) (prim->configDataLen / sizeof(AmEpConfigType));
                inst->currentStream->amSource.amStreamAudioEndPointConfig = CsrPmemAlloc(prim->configDataLen);
                CsrMemCpy(inst->currentStream->amSource.amStreamAudioEndPointConfig, prim->configData, prim->configDataLen);
                break;
            }

            case CSR_AM_EP_SINK_LEFT: /* Only the left sink endpoint designated by sinkId */
            case CSR_AM_EP_SINK_RIGHT: /* Only the right sink endpoint designated by sinkId */
            case CSR_AM_EP_SINK_BOTH_SIDES: /* Both left and right sink endpoint (use sinkId) */
            {
                EndpointType *sinkToConfig = CsrAmGetEndpointToConfigBySinkType(&inst->currentStream->sinkList, prim->sinkId);
                if (sinkToConfig != NULL)
                {
                    CSR_AM_CHANGE_STATE(inst->myState, AM_BUSY_STATE);
                    sinkToConfig->amStreamAudioEndPointConfigLen = (CsrUint8) (prim->configDataLen / sizeof(AmEpConfigType));
                    sinkToConfig->amStreamAudioEndPointConfig = CsrPmemAlloc(prim->configDataLen);
                    CsrMemCpy(sinkToConfig->amStreamAudioEndPointConfig, prim->configData, prim->configDataLen);
                }
                else /* Wrong sink ID parameter. */
                {
                    CsrAmAudioPathConfigCfmSend(inst, CSR_AM_RESULT_UNKNOWN_ID, prim->amConnectionId);
                    CsrAmFreeOperationMessage(inst);
                    return; /* bail out */
                }
                break;
            }

            case CSR_AM_EP_ALL_LEFT_SINKS: /* All the left sinks in the connection stream (ignore sinkId) */
            case CSR_AM_EP_ALL_RIGHT_SINKS: /* All the right sinks in the connection stream (ignore sinkId) */
            case CSR_AM_EP_ALL_SINKS_BOTH_SIDES: /* All the sinks in the connection stream (ignore sinkId) */
            {
                EndpointType *sinkToConfig = (EndpointType *) CsrCmnListElementGetFirst(&inst->currentStream->sinkList);
                if (sinkToConfig == NULL)
                {
                    CsrAmAudioPathConfigCfmSend(inst, CSR_RESULT_FAILURE, prim->amConnectionId);
                    CsrAmFreeOperationMessage(inst);
                    return; /* bail out */
                }
                else
                {
                    CSR_AM_CHANGE_STATE(inst->myState, AM_BUSY_STATE);
                    do
                    {
                        sinkToConfig->amStreamAudioEndPointConfigLen = (CsrUint8) (prim->configDataLen / sizeof(AmEpConfigType));
                        sinkToConfig->amStreamAudioEndPointConfig = CsrPmemAlloc(prim->configDataLen);
                        CsrMemCpy(sinkToConfig->amStreamAudioEndPointConfig, prim->configData, prim->configDataLen);
                        sinkToConfig = sinkToConfig->next;
                    } while (sinkToConfig != NULL);
                }
                break;
            }

            case CSR_AM_EP_ALL_LEFT: /* All the left sinks and the left source in the connection stream (ignore sinkId) */
            case CSR_AM_EP_ALL_RIGHT: /* All the right sinks and the right source in the connection stream (ignore sinkId) */
            case CSR_AM_EP_ALL_BOTH_SIDES: /* All the endpoints in the stream */
            {
                EndpointType *sinkToConfig = (EndpointType *) CsrCmnListElementGetFirst(&inst->currentStream->sinkList);
                CSR_AM_CHANGE_STATE(inst->myState, AM_BUSY_STATE);
                /* Source side */
                inst->currentStream->amSource.amStreamAudioEndPointConfigLen = (CsrUint8) (prim->configDataLen / sizeof(AmEpConfigType));
                inst->currentStream->amSource.amStreamAudioEndPointConfig = CsrPmemAlloc(prim->configDataLen);
                CsrMemCpy(inst->currentStream->amSource.amStreamAudioEndPointConfig, prim->configData, prim->configDataLen);
                /* sinks */
                if (sinkToConfig != NULL)
                {
                    do
                    {
                        sinkToConfig->amStreamAudioEndPointConfigLen = (CsrUint8) (prim->configDataLen / sizeof(AmEpConfigType));
                        sinkToConfig->amStreamAudioEndPointConfig = CsrPmemAlloc(prim->configDataLen);
                        CsrMemCpy(sinkToConfig->amStreamAudioEndPointConfig, prim->configData, prim->configDataLen);
                        sinkToConfig = sinkToConfig->next;
                    } while (sinkToConfig != NULL);
                }
                break;
            }

            default:
            {
                break;
            }
        }


        if (CsrAmNonAudioGainConfigPresent(prim->configDataLen, prim->configData))
        {
            if ((inst->streamTransOpen) || (!inst->streamTransSupported))
            {
                CsrAmIssueBccmdStreamDisconnect(inst->currentStream->amStreamTransformIdL);
                inst->amCallback = CsrAmStreamDisconnectCfm;
            }
            else
            {
                CsrAmIssueBccmdStreamTransStart();
                inst->amCallback = CsrAmStreamTransStartCfm;
            }
        }
        else /* Just issue the configure command(s).
            Start configuring the last key of the first endpoint that needs configuring.
            When confirmation arrives, if the other side needs configuring too, do it; otherwise
            decrease the config counter and configure the next ("the new-last") key.
            When all keys from one configuration pointer/list are removed, deallocate the config
            pointer and go on to the next endpoint until all requested endpoints have been configured.
         */
        {
            if ((!inst->streamTransOpen) && (inst->streamTransSupported)) /* Start transaction first*/
            {
                CsrAmIssueBccmdStreamTransStart();
                inst->amCallback = CsrAmStreamTransStartCfm;
            }
            else
            {
                CsrUint16 sid;
                EndpointType *ep = CsrAmGetEndpointToConfig(inst->currentStream, &sid, prim->epType, FALSE);
                if (ep != NULL)
                {
                    AmEpConfigType *configData = &ep->amStreamAudioEndPointConfig[ep->amStreamAudioEndPointConfigLen - 1];
                    CsrAmIssueBccmdConfigStream(sid, configData->key, configData->value);
                    inst->amCallback = CsrAmStreamConfigCfm;
                }
                else
                {
                    CsrAmAudioPathConfigCfmSend(inst, CSR_RESULT_FAILURE, prim->amConnectionId);
                    CsrAmFreeOperationMessage(inst);
                    /* At this point the AM_BUSY_STATE must not have been entered, otherwise the ep cannot bet NULL */
                }
            }
        }
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmAudioPathReleaseReqHandler
 *
 *  DESCRIPTION
 *      Handles the CSR_AM_AUDIO_PATH_RELEASE_REQ primitive
 *----------------------------------------------------------------------------*/
void CsrAmAudioPathReleaseReqHandler(AmInstanceDataType *inst)
{
    CsrAmAudioPathReleaseReq *prim = (CsrAmAudioPathReleaseReq *) inst->rxDownstreamMsgP;

    inst->currentAppHandle = prim->pHandle;
    inst->currentStream = CsrAmFindStreamByAppConnId(inst, prim->amConnectionId);

    if (inst->currentStream == NULL) /* No stream with that connection Id found: this is considered as a success... */
    {
        CsrAmAudioPathReleaseCfmSend(inst, CSR_RESULT_SUCCESS, prim->amConnectionId);
        CsrAmFreeOperationMessage(inst);
    }
    else /* Handle request */
    {
        CSR_AM_CHANGE_STATE(inst->myState, AM_BUSY_STATE);
        /* Always handle the 'left' side first; in case of mono connections it will be the only one present.*/
        if ((inst->streamTransOpen) || (!inst->streamTransSupported))
        {
            CsrAmIssueBccmdStreamDisconnect(inst->currentStream->amStreamTransformIdL);
            inst->amCallback = CsrAmStreamDisconnectCfm;
        }
        else
        {
            CsrAmIssueBccmdStreamTransStart();
            inst->amCallback = CsrAmStreamTransStartCfm;
        }
    }
}

#ifdef CSR_CHIP_MANAGER_ENABLE
/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmResetIndHandler
 *
 *  DESCRIPTION
 *      Handles the Reset indication primitive
 *----------------------------------------------------------------------------*/
void CsrAmResetIndHandler(AmInstanceDataType *inst)
{
    CsrUint16 msg_type;
    void *msg_data;
    CsrBool found = TRUE;
    CsrAmPrim *amPrim;
    CsrMessageQueueType *tmpSaveQueue = NULL;

    inst->requestType = AM_PRIM_ID_NONE;
    inst->myState = AM_IDLE_STATE;

    if (inst->rxDownstreamMsgP != NULL)
    {
        amPrim = (CsrAmPrim *) inst->rxDownstreamMsgP;
        switch (*amPrim) /* Any ongoing operation will fail, except for AM_INIT_REQ! */
        {
            case CSR_AM_INIT_REQ:
            {
                CsrAmInitReq *prim = (CsrAmInitReq *) inst->rxDownstreamMsgP;
                if ((inst->chipVer >= 7) && (inst->buildId >= FIRST_AUDIO_STREAM_BUILD_ID))
                {
                    CsrAmInitCfmSend(inst, prim->pHandle, CSR_RESULT_SUCCESS);
                }
                else
                {
                    if ((AM_VALUE_UNKNOWN == inst->chipVer) || (inst->buildId == AM_VALUE_UNKNOWN))
                    {
                        inst->myState = AM_INIT_STATE;
                        CsrMessageQueuePush(&tmpSaveQueue, CSR_AM_PRIM, inst->rxDownstreamMsgP);
                        inst->rxDownstreamMsgP = NULL;
                    }
                    else
                    {
                        CsrAmInitCfmSend(inst, prim->pHandle, CSR_AM_RESULT_UNSUPPORTED);
                    }
                }
                break;
            }
            case CSR_AM_AUDIO_PATH_CONNECT_REQ:
            {
                CsrAmAudioPathConnectCfmSend(inst, CSR_RESULT_FAILURE, CSR_AM_NO_CONN_ID);
                break;
            }
            case CSR_AM_AUDIO_PATH_ADD_SINK_REQ:
            {
                CsrAmAudioPathAddSinkReq *prim = (CsrAmAudioPathAddSinkReq *) amPrim;
                CsrAmAudioPathAddSinkCfmSend(inst, CSR_RESULT_FAILURE, prim->amConnectionId);
                break;
            }
            case CSR_AM_AUDIO_PATH_REMOVE_SINK_REQ:
            { /* Return success.... the sink is removed anyway! */
                CsrAmAudioPathRemoveSinkCfmSend(inst, CSR_RESULT_SUCCESS, ((CsrAmAudioPathRemoveSinkReq *) amPrim)->amConnectionId);
                break;
            }
            case CSR_AM_AUDIO_PATH_CONFIG_REQ:
            {
                CsrAmAudioPathConfigReq *prim = (CsrAmAudioPathConfigReq *) amPrim;
                CsrAmAudioPathConfigCfmSend(inst, CSR_RESULT_FAILURE, prim->amConnectionId);
                break;
            }
            case CSR_AM_AUDIO_PATH_RELEASE_REQ:
            { /* Error to begin with; when the connection instance is found, it will be de-allocated and issue a release_ind instead. */
                CsrAmAudioPathReleaseCfmSend(inst, CSR_RESULT_FAILURE, ((CsrAmAudioPathReleaseReq *) amPrim)->amConnectionId);
                break;
            }
        }
        CsrAmFreeOperationMessage(inst);
        amPrim = NULL;
    }

    inst->amCallback = NULL;
    inst->connId = STREAM_ID_NONE;
    inst->streamTransOpen = FALSE;
    inst->restoreFlag = FALSE;

    while (found)
    {
        /* Pop off global save queue first */
        msg_data = NULL;
        msg_type = 0;
        found = CsrMessageQueuePop(&inst->saveQueue, &msg_type, &msg_data);

        /* And finally, try scheduler queue */
        if (!found)
        {
            found = CsrSchedMessageGet(&msg_type, &msg_data);
        }

        if (found && (msg_data != NULL))
        {
            if (msg_type == CSR_AM_PRIM)
            {
                CsrAmPrim *primType = (CsrAmPrim *) msg_data;
                switch (*primType)
                {
                    case CSR_AM_AUDIO_PATH_CONNECT_REQ:
                    {
                        CsrAmAudioPathConnectReq *prim = (CsrAmAudioPathConnectReq *) primType;
                        inst->currentAppHandle = prim->pHandle;
                        CsrAmAudioPathConnectCfmSend(inst, CSR_RESULT_FAILURE, CSR_AM_NO_CONN_ID);
                        break;
                    }
                    case CSR_AM_AUDIO_PATH_ADD_SINK_REQ:
                    {
                        CsrAmAudioPathAddSinkReq *prim = (CsrAmAudioPathAddSinkReq *) primType;
                        inst->currentAppHandle = prim->pHandle;
                        CsrAmAudioPathAddSinkCfmSend(inst, CSR_RESULT_FAILURE, prim->amConnectionId);
                        break;
                    }
                    case CSR_AM_AUDIO_PATH_REMOVE_SINK_REQ:
                    { /* Return success.... the sink is removed anyway! */
                        CsrAmAudioPathRemoveSinkReq *prim = (CsrAmAudioPathRemoveSinkReq *) primType;
                        inst->currentAppHandle = prim->pHandle;
                        CsrAmAudioPathRemoveSinkCfmSend(inst, CSR_RESULT_SUCCESS, prim->amConnectionId);
                        break;
                    }
                    case CSR_AM_AUDIO_PATH_CONFIG_REQ:
                    {
                        CsrAmAudioPathConfigReq *prim = (CsrAmAudioPathConfigReq *) primType;
                        inst->currentAppHandle = prim->pHandle;
                        CsrAmAudioPathConfigCfmSend(inst, CSR_RESULT_FAILURE, prim->amConnectionId);
                        break;
                    }
                    case CSR_AM_AUDIO_PATH_RELEASE_REQ:
                    { /* Error to begin with; when the connection instance is found, it will be de-allocated and issue a release_ind instead. */
                        CsrAmAudioPathReleaseReq *prim = (CsrAmAudioPathReleaseReq *) primType;
                        inst->currentAppHandle = prim->pHandle;
                        CsrAmAudioPathReleaseCfmSend(inst, CSR_RESULT_FAILURE, prim->amConnectionId);
                        break;
                    }
                }

                if (*primType == CSR_AM_INIT_REQ) /* Do not remove init_req from the savequeue */
                {
                    CsrMessageQueuePush(&tmpSaveQueue, CSR_AM_PRIM, msg_data);
                }
                else
                {
                    CsrAmFreeDownstreamMessageContents(msg_type, msg_data);
                    CsrPmemFree(msg_data);
                }
            }
        }
    }

    inst->saveQueue = tmpSaveQueue;

    CsrCmnListDeinit(&inst->newSinkList);

    inst->currentStream = (StreamType *) CsrCmnListElementGetFirst(&inst->streamList);
    while (inst->currentStream != NULL)
    {
        inst->currentAppHandle = inst->currentStream->amConnectionOwner;
        inst->currentStream->amSinkList = (EndpointType *) CsrCmnListElementGetFirst(&inst->currentStream->sinkList);

        while (inst->currentStream->amSinkList != NULL)
        {
            CsrPmemFree(inst->currentStream->amSinkList->amStreamAudioEndPointConfig);
            CsrCmnListElementRemove(&inst->currentStream->sinkList, (CsrCmnListElm_t *) inst->currentStream->amSinkList);
            inst->currentStream->amSinkList = (EndpointType *) CsrCmnListElementGetFirst(&inst->currentStream->sinkList);
        }

        CsrAmAudioPathReleaseIndSend(inst, inst->currentStream->amAppStreamConnectionId);
        CsrCmnListElementRemove(&inst->streamList, (CsrCmnListElm_t *) inst->currentStream);
        inst->currentStream = (StreamType *) CsrCmnListElementGetFirst(&inst->streamList);
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmRestartIndHandler
 *
 *  DESCRIPTION
 *      Handles the Restart indication primitive
 *----------------------------------------------------------------------------*/
void CsrAmRestartIndHandler(AmInstanceDataType *inst) /* Check whether there is a message in the queue and handle it */
{
    if (AM_VALUE_UNKNOWN == inst->chipVer) /* Need to get the chip version */
    {
        CsrAmIssueAmGetChipVer();
        inst->amCallback = CsrAmGetChipVersionCfm;
    }
    else if (inst->buildId == AM_VALUE_UNKNOWN)
    {
        CsrAmIssueGetBuildId();
        inst->amCallback = CsrAmGetBuildIdCfm;
    }
    if (inst->saveQueue) /* This can only be an INIT_REQ */
    {
        CsrAmRestoreSavedMessages(inst);
    }
}

#endif
