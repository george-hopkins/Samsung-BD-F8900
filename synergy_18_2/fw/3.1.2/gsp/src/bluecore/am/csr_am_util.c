/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_pmem.h"

#include "csr_am_handler.h"
#include "csr_am_sef.h"
#include "csr_am_util.h"
#include "csr_am_task.h"

#include "csr_bccmd_lib.h"

#include "csr_log_text_2.h"

/* Definitions */
/* BCCmd  Message Types */
#define GETREQ            0x00
#define GETRESP           0x01
#define SETREQ            0x02

#define SEQ_NO_GET        ((CsrUint16) 0)
#define SEQ_NO_SET        ((CsrUint16) 1)


/********************* Static funcitons ***************************/
/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmIssueBcAmCmd
 *
 *  DESCRIPTION
 *      Used to create and send a BCCMD
 *----------------------------------------------------------------------------*/
static void CsrAmIssueBcAmCmd(CsrUint8 Direction,
    CsrUint16 SeqNo,
    CsrUint16 VarId,
    CsrUint8 RegisterAddress,
    CsrUint16 Value)
{
    CsrUint8 *payload = (CsrUint8 *) CsrPmemAlloc(4);
    CsrUint8 *tmpPayload = payload;

    CSR_ADD_UINT8_TO_XAP(tmpPayload, RegisterAddress);
    CSR_ADD_UINT16_TO_XAP(tmpPayload, Value);

    if (Direction == GETREQ)
    {
        CsrBccmdReadReqSend(CSR_AM_IFACEQUEUE, VarId, SeqNo, 4, payload);
    }
    else
    {
        CsrBccmdWriteReqSend(CSR_AM_IFACEQUEUE, VarId, SeqNo, 4, payload);
    }
}

/********************* Utilily functions: requests ***************************/
/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmIssueAmGetChipVer
 *
 *  DESCRIPTION
 *      Used to get the version of the chip used
 *----------------------------------------------------------------------------*/
void CsrAmIssueAmGetChipVer(void)
{
    CsrAmIssueBcAmCmd(GETREQ, 0, BCCMDVARID_CHIP_VER, 0, 0);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmIssueGetBuildId
 *
 *  DESCRIPTION
 *      Used to get the Build ID of the firmware used
 *----------------------------------------------------------------------------*/
void CsrAmIssueGetBuildId(void)
{
    CsrAmIssueBcAmCmd(GETREQ, 0, BCCMDVARID_BUILD_ID, 0, 0);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmIssueBccmdSetSource
 *
 *  DESCRIPTION
 *      Allocates an audio source endpoint
 *----------------------------------------------------------------------------*/
void CsrAmIssueBccmdSetSource(CsrUint16 source, CsrUint16 instance, CsrUint16 channel)
{
    CsrUint8 *payload = (CsrUint8 *) CsrPmemAlloc(6);
    CsrUint8 *tmpPayload = payload;

    CSR_ADD_UINT16_TO_XAP(tmpPayload, source);
    CSR_ADD_UINT16_TO_XAP(tmpPayload, instance);
    CSR_ADD_UINT16_TO_XAP(tmpPayload, channel);

    CsrBccmdWriteReqSend(CSR_AM_IFACEQUEUE, BCCMDVARID_STREAM_SOURCE, 0, 6, payload);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmIssueBccmdSetSink
 *
 *  DESCRIPTION
 *      Allocates an audio sink endpoint
 *----------------------------------------------------------------------------*/
void CsrAmIssueBccmdSetSink(CsrUint16 sink, CsrUint16 instance, CsrUint16 channel)
{
    CsrUint8 *payload = (CsrUint8 *) CsrPmemAlloc(6);
    CsrUint8 *tmpPayload = payload;

    CSR_ADD_UINT16_TO_XAP(tmpPayload, sink);
    CSR_ADD_UINT16_TO_XAP(tmpPayload, instance);
    CSR_ADD_UINT16_TO_XAP(tmpPayload, channel);

    CsrBccmdWriteReqSend(CSR_AM_IFACEQUEUE, BCCMDVARID_STREAM_SINK, 0, 6, payload);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmIssueBccmdCloseSource
 *
 *  DESCRIPTION
 *      De-allocates an audio source endpoint
 *----------------------------------------------------------------------------*/
void CsrAmIssueBccmdCloseSource(CsrUint16 sourceId)
{
    CsrUint8 *payload = (CsrUint8 *) CsrPmemAlloc(2);
    CsrUint8 *tmpPayload = payload;

    CSR_ADD_UINT16_TO_XAP(tmpPayload, sourceId);

    CsrBccmdWriteReqSend(CSR_AM_IFACEQUEUE, BCCMDVARID_STREAM_CLOSE_SOURCE, 0, 2, payload);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmIssueBccmdCloseSink
 *
 *  DESCRIPTION
 *      De-allocates an audio sink endpoint
 *----------------------------------------------------------------------------*/
void CsrAmIssueBccmdCloseSink(CsrUint16 sinkId)
{
    CsrUint8 *payload = (CsrUint8 *) CsrPmemAlloc(2);
    CsrUint8 *tmpPayload = payload;

    CSR_ADD_UINT16_TO_XAP(tmpPayload, sinkId);

    CsrBccmdWriteReqSend(CSR_AM_IFACEQUEUE, BCCMDVARID_STREAM_CLOSE_SINK, 0, 2, payload);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmIssueBccmdStreamTransStart
 *
 *  DESCRIPTION
 *      Starts a stream transaction
 *----------------------------------------------------------------------------*/
void CsrAmIssueBccmdStreamTransStart(void)
{
    CsrAmIssueBcAmCmd(SETREQ, 0, BCCMDVARID_STREAM_TRANS_START, 0, 0);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmIssueBccmdStreamTransComplete
 *
 *  DESCRIPTION
 *      Terminates an open stream transaction
 *----------------------------------------------------------------------------*/
void CsrAmIssueBccmdStreamTransComplete(void)
{
    CsrAmIssueBcAmCmd(SETREQ, 0, BCCMDVARID_STREAM_TRANS_COMPLETE, 0, 0);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmIssueBccmdStreamConnect
 *
 *  DESCRIPTION
 *      Establishes a stream between one source and one sink and its aliases
 *----------------------------------------------------------------------------*/
void CsrAmIssueBccmdStreamConnect(CsrUint16 src, CsrUint16 snk)
{
    CsrUint8 *payload = (CsrUint8 *) CsrPmemAlloc(4);
    CsrUint8 *tmpPayload = payload;

    CSR_ADD_UINT16_TO_XAP(tmpPayload, src);
    CSR_ADD_UINT16_TO_XAP(tmpPayload, snk);

    CsrBccmdWriteReqSend(CSR_AM_IFACEQUEUE, BCCMDVARID_STREAM_CONNECT, 0, 4, payload);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmIssueBccmdStreamDisconnect
 *
 *  DESCRIPTION
 *      Releases a stream
 *----------------------------------------------------------------------------*/
void CsrAmIssueBccmdStreamDisconnect(CsrUint16 streamId)
{
    CsrUint8 *payload = (CsrUint8 *) CsrPmemAlloc(2);
    CsrUint8 *tmpPayload = payload;

    CSR_ADD_UINT16_TO_XAP(tmpPayload, streamId);

    CsrBccmdWriteReqSend(CSR_AM_IFACEQUEUE, BCCMDVARID_STREAM_TRANS_DISCONNECT, 0, 2, payload);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmIssueBccmdStreamSnkAlias
 *
 *  DESCRIPTION
 *      Couples a sink with a newly allocated sink
 *----------------------------------------------------------------------------*/
void CsrAmIssueBccmdStreamSnkAlias(CsrUint16 snk1, CsrUint16 snk2)
{
    CsrUint8 *payload;
    CsrUint8 *tmpPayload;
    CsrUint8 payloadLen = 2 * sizeof(CsrUint16);

    payload = (CsrUint8 *) CsrPmemAlloc(payloadLen);
    tmpPayload = payload;

    CSR_ADD_UINT16_TO_XAP(tmpPayload, snk1);
    CSR_ADD_UINT16_TO_XAP(tmpPayload, snk2);

    CsrBccmdWriteReqSend(CSR_AM_IFACEQUEUE, BCCMDVARID_STREAM_SINK_ALIAS, 0, payloadLen, payload);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmIssueBccmdStreamSyncEndpoints
 *
 *  DESCRIPTION
 *      Performs a sync operation between two endpoints
 *----------------------------------------------------------------------------*/
void CsrAmIssueBccmdStreamSyncEndpoints(CsrUint16 ep1, CsrUint16 ep2)
{
    CsrUint8 *payload;
    CsrUint8 *tmpPayload;
    CsrUint8 payloadLen = 2 * sizeof(CsrUint16);

    payload = (CsrUint8 *) CsrPmemAlloc(payloadLen);
    tmpPayload = payload;

    CSR_ADD_UINT16_TO_XAP(tmpPayload, ep1);
    CSR_ADD_UINT16_TO_XAP(tmpPayload, ep2);

    CsrBccmdWriteReqSend(CSR_AM_IFACEQUEUE, BCCMDVARID_STREAM_SYNC_SID, 0, payloadLen, payload);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmIssueBccmdConfigStream
 *
 *  DESCRIPTION
 *      Configures an audio endpoint
 *----------------------------------------------------------------------------*/
void CsrAmIssueBccmdConfigStream(CsrUint16 sid, CsrUint32 key, CsrUint32 value)
{
    CsrUint8 *payload;
    CsrUint8 *tmpPayload;
    CsrUint8 payloadLen = 4 * sizeof(CsrUint16);

    payload = (CsrUint8 *) CsrPmemAlloc(payloadLen);
    tmpPayload = payload;

    CSR_ADD_UINT16_TO_XAP(tmpPayload, sid);
    CSR_ADD_UINT16_TO_XAP(tmpPayload, (CsrUint16) key);
    CSR_ADD_UINT32_TO_XAP(tmpPayload, value);

    CsrBccmdWriteReqSend(CSR_AM_IFACEQUEUE, BCCMDVARID_STREAM_CONFIGURE, 0, payloadLen, payload);
}

/********************* Utilily functions: callbacks ***************************/
/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmIssueAmGetChipVer
 *
 *  DESCRIPTION
 *      Used to get the version of the chip used
 *----------------------------------------------------------------------------*/
extern void CsrAmFreeDownstreamMessageContents(CsrUint16 eventClass, void *message);
void CsrAmFreeOperationMessage(AmInstanceDataType *inst)
{
    if (inst->rxDownstreamMsgP != NULL)
    {
        CsrAmFreeDownstreamMessageContents(CSR_AM_PRIM, inst->rxDownstreamMsgP);
        CsrPmemFree(inst->rxDownstreamMsgP);
        inst->rxDownstreamMsgP = NULL;
    }
}

void CsrAmFreeOperationMessageAndQueuedMessages(AmInstanceDataType *inst)
{
    CsrUint16 event = 0;
    while ((inst->saveQueue != NULL) || (inst->rxDownstreamMsgP != NULL))
    {
        CsrAmFreeOperationMessage(inst);
        (void) CsrMessageQueuePop(&inst->saveQueue, &event, &inst->rxDownstreamMsgP);
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmGetBcValue
 *
 *  DESCRIPTION
 *      Extracts a value from a BCCMD confirm message received
 *----------------------------------------------------------------------------*/
CsrUint16 CsrAmGetBcValue(CsrBccmdCfm *bcCmdMsg)
{
    CsrUint16 value;

    value = (CsrUint16) (((CsrUint16) (bcCmdMsg->payload[1]) << 8) | ((CsrUint16) (bcCmdMsg->payload[0])));

    return value;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmGetBcValue32bit
 *
 *  DESCRIPTION
 *      Extracts a value from a BCCMD confirm message received
 *----------------------------------------------------------------------------*/
CsrUint32 CsrAmGetBcValue32bit(CsrBccmdCfm *bcCmdMsg, CsrUint8 offset)
{
    CsrUint32 value;
    CsrUint32 tmpVal1, tmpVal2;

    tmpVal1 = (CsrUint32) (((CsrUint32) (bcCmdMsg->payload[1 + offset]) << 8) | ((CsrUint32) (bcCmdMsg->payload[0 + offset])));
    tmpVal2 = (CsrUint32) (((CsrUint32) (bcCmdMsg->payload[3 + offset]) << 8) | ((CsrUint32) (bcCmdMsg->payload[2 + offset])));

    value = ((tmpVal1 << 16) | tmpVal2);

    return value;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmCreateNewStreamElement
 *
 *  DESCRIPTION
 *      Creates and initialises a new stream element in the stream queue.
 *----------------------------------------------------------------------------*/
void CsrAmCreateNewStreamElement(AmInstanceDataType *inst, CsrAmAudioPathConnectReq *prim)
{
    inst->currentStream = (StreamType *) CsrCmnListElementAddLast(&inst->streamList, sizeof(StreamType));
    /* Fill in data... */
    /* Source */
    inst->currentStream->amSource.next = NULL;
    inst->currentStream->amSource.prev = NULL;
    inst->currentStream->amSource.amStreamAudioEndPointL = STREAM_TYPE_NO_ENDPOINT;
    inst->currentStream->amSource.amStreamAudioEndPointR = STREAM_TYPE_NO_ENDPOINT;
    CsrMemCpy(&(inst->currentStream->amSource.amStreamAudioEndPointType), &(prim->source.endPoint), sizeof(CsrAmEndPointDefType));
    CsrMemCpy(&(inst->currentStream->amSource.amStreamAudioEndPointTypeR), &(prim->sourceR.endPoint), sizeof(CsrAmEndPointDefType));
    inst->currentStream->amSource.amStreamAudioEndPointConfigLen = prim->source.configDataCount + prim->sourceR.configDataCount;
    inst->currentStream->amSource.amStreamAudioEndPointConfig = NULL;
    inst->currentStream->amSource.amStreamAudioEndPointLConfigDone = FALSE;
    if (inst->currentStream->amSource.amStreamAudioEndPointConfigLen > 0)
    {
        AmEpConfigType *tmpConfig = CsrPmemAlloc(inst->currentStream->amSource.amStreamAudioEndPointConfigLen * sizeof(AmEpConfigType));
        CsrUint16 offset = prim->source.configDataCount * sizeof(AmEpConfigType);

        if (prim->source.configDataCount > 0)
        {
            CsrMemCpy(tmpConfig, prim->source.configData, offset);
        }

        if (prim->sourceR.configDataCount > 0)
        {
            CsrMemCpy(&tmpConfig[prim->source.configDataCount], prim->sourceR.configData, prim->sourceR.configDataCount * sizeof(AmEpConfigType));
        }

        inst->currentStream->amSource.amStreamAudioEndPointConfig = tmpConfig;
        CsrPmemFree(prim->source.configData);
        prim->source.configData = NULL;
        CsrPmemFree(prim->sourceR.configData);
        prim->sourceR.configData = NULL;
    }
    inst->currentStream->amSource.amStreamAudioEndPointSynced = FALSE;
    inst->currentStream->amSource.amStreamAudioEndPointAliased = TRUE;   /* Alias not relevant for source endpoints */
    inst->currentStream->amSource.amStreamAudioEndPointAliasedR = TRUE;
    /* Sinks to add. Stored in the newSinkList to begin with...
       Initialise the stream sink list though. */
    CsrCmnListInit(&(inst->currentStream->sinkList), 0, NULL, NULL);
    inst->currentStream->amSinkList = NULL;
    CsrAmCreateLinkOfSinksToAdd(inst);
    /* General data */
    inst->currentStream->amStereo = prim->stereo;
    inst->currentStream->amStreamType = prim->audioType;
    inst->currentStream->amAppStreamConnectionId = STREAM_ID_NONE;
    inst->currentStream->amConnectionOwner = prim->pHandle;
    inst->currentStream->amStreamTransformIdL = STREAM_ID_NONE;
    inst->currentStream->amStreamTransformIdR = STREAM_ID_NONE;
    inst->currentStream->amStreamState = AM_STREAM_INITIALISED_STATE;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmFindStreamByAppConnId
 *
 *  DESCRIPTION
 *      Finds a stream connection given its connection ID. Returns a pointer
 *      to the stream is found or a NULL pointer if not found.
 *----------------------------------------------------------------------------*/
StreamType *CsrAmFindStreamByAppConnId(AmInstanceDataType *inst, CsrUint16 connId)
{
    StreamType *currentStream = NULL;

    for (currentStream = (StreamType *) CsrCmnListElementGetFirst(&inst->streamList); currentStream; currentStream = currentStream->next)
    {
        if (currentStream->amAppStreamConnectionId == connId)
        {
            return currentStream;
        }
    }


    return NULL;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmFindStreamByAppSrcType
 *
 *  DESCRIPTION
 *      Finds a stream connection out of the endpoint type of the source
 *----------------------------------------------------------------------------*/
StreamType *CsrAmFindStreamByAppSrcType(AmInstanceDataType *inst, CsrEndpointType sourceType)
{
    StreamType *currentStream = NULL;
    for (currentStream = (StreamType *) CsrCmnListElementGetFirst(&inst->streamList); currentStream; currentStream = currentStream->next)
    {
        if ((currentStream->amSource.amStreamAudioEndPointType.endPoint == sourceType.endPoint.endPoint) &&
            ((currentStream->amSource.amStreamAudioEndPointType.instance == sourceType.endPoint.instance) || (sourceType.endPoint.instance == CSR_AM_INSTANCE_AUTO)) &&
            ((currentStream->amSource.amStreamAudioEndPointType.channel == sourceType.endPoint.channel) || (sourceType.endPoint.channel == CSR_AM_CHANNEL_AUTO)))
        {
            return currentStream;
        }
    }

    return NULL;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmFindStreamByAppSinkType
 *
 *  DESCRIPTION
 *      Finds a stream connection out of the endpoint type of one of the sinks
 *----------------------------------------------------------------------------*/
StreamType *CsrAmFindStreamByAppSinkType(AmInstanceDataType *inst, CsrAmEndPointDefType sinkType)
{
    StreamType *currentStream = NULL;
    for (currentStream = (StreamType *) CsrCmnListElementGetFirst(&inst->streamList); currentStream; currentStream = currentStream->next)
    {
        EndpointType *sinkList = NULL;
        for (sinkList = (EndpointType *) CsrCmnListElementGetFirst(&currentStream->sinkList); sinkList; sinkList = sinkList->next)
        {
            if ((sinkList->amStreamAudioEndPointType.endPoint == sinkType.endPoint) &&
                ((sinkList->amStreamAudioEndPointType.instance == sinkType.instance) || (sinkType.instance == CSR_AM_INSTANCE_AUTO)) &&
                ((sinkList->amStreamAudioEndPointType.channel == sinkType.channel) || (sinkType.channel == CSR_AM_CHANNEL_AUTO)))
            {
                return currentStream;
            }
        }
    }

    return NULL;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmIsSrcInUse
 *
 *  DESCRIPTION
 *      Determines whether a source endpoint is in use
 *----------------------------------------------------------------------------*/
CsrBool CsrAmIsSrcInUse(AmInstanceDataType *inst, CsrEndpointType source, StreamType **srcStream)
{
    *srcStream = CsrAmFindStreamByAppSrcType(inst, source);
    if (*srcStream == NULL)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmIsSinkInUse
 *
 *  DESCRIPTION
 *      Determines whether a sink endpoint is in use
 *----------------------------------------------------------------------------*/
CsrBool CsrAmIsSinkInUse(AmInstanceDataType *inst, CsrAmEndPointDefType sink, StreamType **sinkStream)
{
    *sinkStream = CsrAmFindStreamByAppSinkType(inst, sink);
    if (*sinkStream == NULL)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmFindEndPointBySinkId
 *
 *  DESCRIPTION
 *      Returns an endpoint element within a sink list given a sink ID
 *----------------------------------------------------------------------------*/
EndpointType *CsrAmFindEndPointBySinkId(CsrCmnList_t *list, CsrUint16 sinkId)
{
    EndpointType *retPtr = (EndpointType *) CsrCmnListElementGetFirst(list);

    while (retPtr != NULL)
    {
        if ((retPtr->amStreamAudioEndPointL == sinkId) ||
            (retPtr->amStreamAudioEndPointR == sinkId) ||
            (sinkId == CSR_AM_ENDPOINT_ALL))
        {
            return retPtr;
        }
        else
        {
            retPtr = retPtr->next;
        }
    }

    return retPtr;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmFindEndPointBySinkType
 *
 *  DESCRIPTION
 *      Returns an endpoint element within a sink list given a sink type
 *----------------------------------------------------------------------------*/
EndpointType *CsrAmFindEndPointBySinkType(EndpointType *sinkList, CsrAmEndPointDefType sinkType)
{
    EndpointType *retPtr = sinkList;

    while (retPtr != NULL)
    {
        if (((retPtr->amStreamAudioEndPointType.endPoint == sinkType.endPoint) &&
             ((retPtr->amStreamAudioEndPointType.instance == sinkType.instance) || (sinkType.instance == CSR_AM_INSTANCE_AUTO)) &&
             ((retPtr->amStreamAudioEndPointType.channel == sinkType.channel) || (sinkType.channel == CSR_AM_CHANNEL_AUTO)))
            ||
            ((retPtr->amStreamAudioEndPointTypeR.endPoint == sinkType.endPoint) &&
             ((retPtr->amStreamAudioEndPointTypeR.instance == sinkType.instance) || (sinkType.instance == CSR_AM_INSTANCE_AUTO)) &&
             ((retPtr->amStreamAudioEndPointTypeR.channel == sinkType.channel) || (sinkType.channel == CSR_AM_CHANNEL_AUTO))))

        {
            return retPtr;
        }
        else
        {
            retPtr = retPtr->next;
        }
    }

    return retPtr;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmFindAndRemoveSinkFromSinkList
 *
 *  DESCRIPTION
 *      Removes a sink endpoint entry out of a sink list from a stream
 *----------------------------------------------------------------------------*/
void CsrAmFindAndRemoveSinkFromSinkList(StreamType *stream, CsrAmEndPointDefType sinkType)
{
    EndpointType *sinkToRemove;
    stream->amSinkList = (EndpointType *) CsrCmnListElementGetFirst(&stream->sinkList);
    sinkToRemove = CsrAmFindEndPointBySinkType(stream->amSinkList, sinkType);
    if (sinkToRemove != NULL) /* found: now remove it */
    {
        CsrCmnListElementRemove(&stream->sinkList, (CsrCmnListElm_t *) sinkToRemove);
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmCreateLinkOfSinksToAdd
 *
 *  DESCRIPTION
 *      Creates a list of sinks and their configurations out of the sinks and
 *      configurations received in a CSR_AM_AUDIO_PATH_ADD_SINK_REQ
 *----------------------------------------------------------------------------*/
void CsrAmCreateLinkOfSinksToAdd(AmInstanceDataType *instData)
{
    CsrAmAudioPathAddSinkReq *prim = (CsrAmAudioPathAddSinkReq *) instData->rxDownstreamMsgP;
    CsrUint8 i, j, configCnt = 0;
    CsrAmEndPointDefType *sinkEndPoint;
    AmEpConfigType *tmpConfigDataPtr = NULL;
    CsrAmSinksToAddType sinks;

    if (prim->type == CSR_AM_AUDIO_PATH_ADD_SINK_REQ)
    {
        sinks = prim->sinks;
    }
    else
    {
        sinks = ((CsrAmAudioPathConnectReq *) prim)->sinks;
    }

    for (i = 0; i < sinks.sinkCount; i++)
    {
        sinkEndPoint = (CsrAmEndPointDefType *) &(sinks.newSinkId[i]);

        instData->sinksToAdd = (EndpointType *) CsrCmnListElementGetLast(&instData->newSinkList);

        if (instData->sinksToAdd == NULL)
        {
            instData->sinksToAdd = (EndpointType *) CsrCmnListElementAddLast(&instData->newSinkList, sizeof(EndpointType));
            instData->sinksToAdd->amStreamAudioEndPointL = instData->sinksToAdd->amStreamAudioEndPointR = STREAM_TYPE_NO_ENDPOINT;
            instData->sinksToAdd->amStreamAudioEndPointType.endPoint = sinkEndPoint->endPoint;
            instData->sinksToAdd->amStreamAudioEndPointType.instance = sinkEndPoint->instance;
            instData->sinksToAdd->amStreamAudioEndPointType.channel = sinkEndPoint->channel;
            instData->sinksToAdd->amStreamAudioEndPointTypeR.endPoint = STREAM_TYPE_NO_ENDPOINT;
            instData->sinksToAdd->amStreamAudioEndPointTypeR.instance = CSR_AM_INSTANCE_AUTO;
            instData->sinksToAdd->amStreamAudioEndPointTypeR.channel = CSR_AM_CHANNEL_AUTO;
            instData->sinksToAdd->amStreamAudioEndPointConfigLen = 0;
            instData->sinksToAdd->amStreamAudioEndPointConfig = NULL;
            instData->sinksToAdd->amStreamAudioEndPointSynced = FALSE;
            instData->sinksToAdd->amStreamAudioEndPointAliased = FALSE;
            instData->sinksToAdd->amStreamAudioEndPointAliasedR = FALSE;
            instData->sinksToAdd->amStreamAudioEndPointLConfigDone = FALSE;
        }
        else
        {
            if (instData->sinksToAdd->amStreamAudioEndPointType.endPoint != sinkEndPoint->endPoint)
            {
                instData->sinksToAdd = (EndpointType *) CsrCmnListElementAddLast(&instData->newSinkList, sizeof(EndpointType));
                instData->sinksToAdd->amStreamAudioEndPointL = instData->sinksToAdd->amStreamAudioEndPointR = STREAM_TYPE_NO_ENDPOINT;
                instData->sinksToAdd->amStreamAudioEndPointType.endPoint = sinkEndPoint->endPoint;
                instData->sinksToAdd->amStreamAudioEndPointType.instance = sinkEndPoint->instance;
                instData->sinksToAdd->amStreamAudioEndPointType.channel = sinkEndPoint->channel;
                instData->sinksToAdd->amStreamAudioEndPointTypeR.endPoint = STREAM_TYPE_NO_ENDPOINT;
                instData->sinksToAdd->amStreamAudioEndPointTypeR.instance = CSR_AM_INSTANCE_AUTO;
                instData->sinksToAdd->amStreamAudioEndPointTypeR.channel = CSR_AM_CHANNEL_AUTO;
                instData->sinksToAdd->amStreamAudioEndPointConfigLen = 0;
                instData->sinksToAdd->amStreamAudioEndPointConfig = NULL;
                instData->sinksToAdd->amStreamAudioEndPointSynced = FALSE;
                instData->sinksToAdd->amStreamAudioEndPointAliased = FALSE;
                instData->sinksToAdd->amStreamAudioEndPointAliasedR = FALSE;
                instData->sinksToAdd->amStreamAudioEndPointLConfigDone = FALSE;
            }
            else
            {
                instData->sinksToAdd->amStreamAudioEndPointTypeR.endPoint = sinkEndPoint->endPoint;
                instData->sinksToAdd->amStreamAudioEndPointTypeR.instance = sinkEndPoint->instance;
                instData->sinksToAdd->amStreamAudioEndPointTypeR.channel = sinkEndPoint->channel;
            }
        }
        if (sinks.sinkConfig != NULL)
        {
            configCnt = 0;
            for (j = 0; j < sinks.sinkConfigCount; j++)
            {
                if (sinks.sinkConfig[j].sinkIndex == i) /* A configuration element for the current new sink is found */
                {
                    configCnt++;
                }
                else if (configCnt > 0) /* Now "configCount" contains the total number of configuration elements for this new sink */
                {
                    j = sinks.sinkConfigCount;
                }
            }

            if (configCnt > 0) /* each config entry consists of two fields 32-bit-long each */
            {
                CsrUint8 idx = 0;
                tmpConfigDataPtr = CsrPmemAlloc(configCnt * sizeof(AmEpConfigType));
                for (j = 0; j < sinks.sinkConfigCount; j++)
                {
                    if (sinks.sinkConfig[j].sinkIndex == i)
                    {
                        tmpConfigDataPtr[idx].key = sinks.sinkConfig[j].key;
                        tmpConfigDataPtr[idx].value = sinks.sinkConfig[j].value;
                        idx++;
                    }
                }
            }
        }
        if (instData->sinksToAdd->amStreamAudioEndPointConfig == NULL)
        {
            instData->sinksToAdd->amStreamAudioEndPointConfig = tmpConfigDataPtr;
            instData->sinksToAdd->amStreamAudioEndPointConfigLen = configCnt;
        }
        else
        {
            if (tmpConfigDataPtr != NULL) /* Need to merge the old config pointer with the new data */
            {
                AmEpConfigType *tmp = instData->sinksToAdd->amStreamAudioEndPointConfig;
                instData->sinksToAdd->amStreamAudioEndPointConfig = CsrPmemAlloc((instData->sinksToAdd->amStreamAudioEndPointConfigLen + configCnt) * sizeof(AmEpConfigType));
                CsrMemCpy(instData->sinksToAdd->amStreamAudioEndPointConfig, tmp, instData->sinksToAdd->amStreamAudioEndPointConfigLen * sizeof(AmEpConfigType));
                CsrPmemFree(tmp);
                tmp = &instData->sinksToAdd->amStreamAudioEndPointConfig[instData->sinksToAdd->amStreamAudioEndPointConfigLen];
                CsrMemCpy(tmp, tmpConfigDataPtr, configCnt * sizeof(AmEpConfigType));
                instData->sinksToAdd->amStreamAudioEndPointConfigLen += configCnt;
                CsrPmemFree(tmpConfigDataPtr);
            }
        }

        tmpConfigDataPtr = NULL;
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmDestroyLinkOfSinksToAdd
 *
 *  DESCRIPTION
 *      Destroys the list of sinks and their configurations that are being
 *      added to a stream
 *----------------------------------------------------------------------------*/
void CsrAmDestroyLinkOfSinksToAdd(AmInstanceDataType *instData)
{
    CsrUint8 cnt = (CsrUint8) CsrCmnListGetCount(&instData->newSinkList);
    CsrUint8 i;

    for (i = 0; i < cnt; i++)
    {
        instData->sinksToAdd = (EndpointType *) CsrCmnListElementGetFirst(&instData->newSinkList);
        CsrCmnListElementRemove(&instData->newSinkList, (CsrCmnListElm_t *) instData->sinksToAdd);
    }
    instData->sinksToAdd = NULL;
    instData->sinkAddedcount = 0;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmFindNewSinkToConfig
 *
 *  DESCRIPTION
 *      Finds the sink and sinkId to configure next among the new sinks
 *      allocated in a connect or add sink operation
 *----------------------------------------------------------------------------*/
EndpointType *CsrAmFindNewSinkToConfig(AmInstanceDataType *inst, CsrBool remove, CsrUint16 *sinkId)
{
    EndpointType *ep = (EndpointType *) CsrCmnListElementGetFirst(&inst->newSinkList);

    if (ep != NULL)
    {
        CsrBool found = FALSE;
        do
        {
            if ((ep->amStreamAudioEndPointConfigLen > 0) &&
                ((ep->amStreamAudioEndPointR != STREAM_ID_NONE) || (ep->amStreamAudioEndPointL != STREAM_ID_NONE)))
            { /* Sink found: determine the sid */
                if (remove && (ep->amStreamAudioEndPointLConfigDone == FALSE))
                {
                    ep->amStreamAudioEndPointConfigLen--;
                }

                if (ep->amStreamAudioEndPointConfigLen > 0)
                {
                    found = TRUE;
                    if ((ep->amStreamAudioEndPointLConfigDone == TRUE))
                    {
                        *sinkId = ep->amStreamAudioEndPointR;
                        ep->amStreamAudioEndPointLConfigDone = FALSE;
                    }
                    else
                    {
                        *sinkId = ep->amStreamAudioEndPointL;
                        ep->amStreamAudioEndPointLConfigDone = TRUE;
                    }
                }
                else /* Sink configured: remove the config pointer and check the next sink */
                {
                    CsrPmemFree(ep->amStreamAudioEndPointConfig);
                    ep->amStreamAudioEndPointConfig = NULL;
                    ep = ep->next;
                }
            }
            else /* Not found; keep on looking until the end of the list */
            {
                ep = ep->next;
            }
        } while ((!found) && (ep != NULL));
    }

    return ep;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmStoreNewSinkId
 *
 *  DESCRIPTION
 *      Stores the endpointId of the new sink allocated in the proper endpoint
 *      entry in the list of new sinks and returns a pointer to that entry
 *----------------------------------------------------------------------------*/
EndpointType *CsrAmStoreNewSinkId(AmInstanceDataType *inst, CsrUint16 sinkId)
{
    CsrUint8 i;
    EndpointType *ep = (EndpointType *) CsrCmnListElementGetFirst(&inst->newSinkList);

    for (i = 0; i < inst->sinkAddedcount; i++)
    {
        ep = ep->next;
    }

    if (ep != NULL)
    {
        if (ep->amStreamAudioEndPointL == STREAM_TYPE_NO_ENDPOINT)
        {
            ep->amStreamAudioEndPointL = sinkId;
        }
        else /* The left side has already a value; this must be the right side */
        {
            ep->amStreamAudioEndPointR = sinkId;
        }
    }
    return ep;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmFindAndRemoveConfigItemFromSinkList
 *
 *  DESCRIPTION
 *      Finds an endpoint entry where the given config key is present and
 *      removes it form the list because the key has been configured.
 *      The config counter is decreased and a pointer to the endpoint
 *      is returned in order to configure the next key if needed.
 *----------------------------------------------------------------------------*/
EndpointType *CsrAmFindAndRemoveConfigItemFromSinkList(AmInstanceDataType *inst, CsrUint32 key, CsrUint16 sid)
{
    EndpointType *retPtr = (EndpointType *) CsrCmnListElementGetFirst(&inst->newSinkList);
    CsrUint8 i;

    for (i = 0; i < inst->sinkAddedcount; i++)
    {
        retPtr = retPtr->next;
        if (retPtr->amStreamAudioEndPointL == sid)
        {
            if (retPtr->amStreamAudioEndPointConfigLen > 0)
            {
                /* Pointer found; now find the configuration entry: it is always the "last" */
                if (retPtr->amStreamAudioEndPointConfig[retPtr->amStreamAudioEndPointConfigLen - 1].key == key)
                {
                    retPtr->amStreamAudioEndPointConfig[retPtr->amStreamAudioEndPointConfigLen - 1].key = 0xFFFF;
                    retPtr->amStreamAudioEndPointConfigLen--;
                }
            }
            return retPtr;
        }
    }

    return retPtr;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmGetNewSinkToAlloc
 *
 *  DESCRIPTION
 *      Returns the endpoint to allocate during connect or add sink operations.
 *      Returns NULL if no more sinks to allocate.
 *----------------------------------------------------------------------------*/
CsrAmEndPointDefType *CsrAmGetNewSinkToAlloc(AmInstanceDataType *inst)
{
    EndpointType *retPtr = (EndpointType *) CsrCmnListElementGetFirst(&inst->newSinkList);
    CsrUint8 i;

    for (i = 0; i < inst->sinkAddedcount; i++) /* Get the next after 'sinkAddedcount' if any...*/
    {
        retPtr = retPtr->next;
    }

    if (retPtr != NULL)
    {
        if (retPtr->amStreamAudioEndPointL == STREAM_TYPE_NO_ENDPOINT)
        {
            return (CsrAmEndPointDefType *) &(retPtr->amStreamAudioEndPointType);
        }
        else if (retPtr->amStreamAudioEndPointR == STREAM_TYPE_NO_ENDPOINT)
        {
            return (CsrAmEndPointDefType *) &(retPtr->amStreamAudioEndPointTypeR);
        }
    }

    return NULL;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmAddNewSinkToStreamSinkList
 *
 *  DESCRIPTION
 *      Takes the endpoint to add during connect or add sink operations and
 *      adds it to the list of sinks in the stream, and removes it from the
 *      "to-add" list.
 *----------------------------------------------------------------------------*/
void CsrAmAddNewSinkToStreamSinkList(AmInstanceDataType *inst)
{
    CsrUint8 i;
    EndpointType *newSinkPtr = (EndpointType *) CsrCmnListElementGetFirst(&inst->newSinkList);

    for (i = 0; ((i < inst->sinkAddedcount) && (newSinkPtr != NULL)); i++)
    {
        inst->currentStream->amSinkList = (EndpointType *) CsrCmnListElementAddLast(&inst->currentStream->sinkList, sizeof(EndpointType));
        inst->currentStream->amSinkList->amStreamAudioEndPointL = newSinkPtr->amStreamAudioEndPointL;
        inst->currentStream->amSinkList->amStreamAudioEndPointR = newSinkPtr->amStreamAudioEndPointR;
        CsrMemCpy(&inst->currentStream->amSinkList->amStreamAudioEndPointType,
            &newSinkPtr->amStreamAudioEndPointType, sizeof(CsrAmEndPointDefType));
        CsrMemCpy(&inst->currentStream->amSinkList->amStreamAudioEndPointTypeR,
            &newSinkPtr->amStreamAudioEndPointTypeR, sizeof(CsrAmEndPointDefType));
        inst->currentStream->amSinkList->amStreamAudioEndPointSynced = newSinkPtr->amStreamAudioEndPointSynced;
        inst->currentStream->amSinkList->amStreamAudioEndPointAliased = newSinkPtr->amStreamAudioEndPointAliased;
        inst->currentStream->amSinkList->amStreamAudioEndPointAliasedR = newSinkPtr->amStreamAudioEndPointAliasedR;
        /*if (inst->currentStream->amSinkList->amStreamAudioEndPointR != STREAM_TYPE_NO_ENDPOINT)
        {/ * Two entries in one... right and left sides of a stereo connection * /
            i++;
        }*/
        /* Entry moved; remove it from the newSink list */
        inst->currentStream->amSinkList->amStreamAudioEndPointConfig = newSinkPtr->amStreamAudioEndPointConfig;
        inst->currentStream->amSinkList->amStreamAudioEndPointConfigLen = newSinkPtr->amStreamAudioEndPointConfigLen;
        inst->currentStream->amSinkList->amStreamAudioEndPointLConfigDone = newSinkPtr->amStreamAudioEndPointLConfigDone;
        newSinkPtr->amStreamAudioEndPointConfig = NULL;
        newSinkPtr->amStreamAudioEndPointConfigLen = 0;
        CsrCmnListElementRemove(&inst->newSinkList, (CsrCmnListElm_t *) newSinkPtr);
        /* Get the next entry */
        newSinkPtr = (EndpointType *) CsrCmnListElementGetFirst(&inst->newSinkList);
    }
    /* Reset counter */
    inst->sinkAddedcount = 0;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmGenerateAppConnId
 *
 *  DESCRIPTION
 *      Checks the list of streams for connection Id values and generates a new
 *      one. The connection Ids will range from 0 to 0xFFFF
 *----------------------------------------------------------------------------*/
CsrUint16 CsrAmGenerateAppConnId(AmInstanceDataType *inst)
{
    CsrUint16 newValue = 0;
    StreamType *tmpStream = (StreamType *) CsrCmnListElementGetFirst(&inst->streamList);

    while (tmpStream != NULL)
    {
        if (tmpStream->amAppStreamConnectionId == newValue)
        {
            /* Try a new value - search from start of list again */
            tmpStream = (StreamType *) CsrCmnListElementGetFirst(&inst->streamList);
            newValue++;

            CSR_LOG_TEXT_CONDITIONAL_CRITICAL(newValue == 0,
                (CsrAmLto, 0, "No values left for connection ID"));
        }
        else
        {
            tmpStream = tmpStream->next;
        }
    }

    inst->currentStream->amAppStreamConnectionId = newValue;

    return newValue;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmMarkAllSyncEndpoints
 *
 *  DESCRIPTION
 *      Marks all endpoints are already sync'ed. Used when the sync command
 *      is not understood by the FW (returns NO_SUCH_VARID).
 *----------------------------------------------------------------------------*/
void CsrAmMarkAllSyncEndpoints(AmInstanceDataType *inst)
{
    EndpointType *ep = (EndpointType *) CsrCmnListElementGetFirst(&inst->currentStream->sinkList);

    while (ep != NULL)
    {
        ep->amStreamAudioEndPointSynced = TRUE;
        ep = ep->next;
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmNeedToSyncEndpoints
 *
 *  DESCRIPTION
 *      Searches for the first entry in the stream's sink list that needs
 *      to have sync'ed the endpoints. Returns FALSE if none found.
 *      If found, a pointer to the endpoint is returned as well.
 *----------------------------------------------------------------------------*/
CsrBool CsrAmNeedToSyncEndpoints(AmInstanceDataType *inst, EndpointType **retEp)
{
    CsrBool needToSync = FALSE;
    EndpointType *ep = (EndpointType *) CsrCmnListElementGetFirst(&inst->currentStream->sinkList);

    *retEp = NULL;
    if (ep != NULL)
    {
        do
        {
            if (ep->amStreamAudioEndPointSynced)
            {
                ep = ep->next;
            }
            else
            {
                needToSync = TRUE;
                *retEp = ep;
            }
        } while ((ep != NULL) && (!needToSync));
    }

    return needToSync;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmFindAndDoSinkToAlias
 *
 *  DESCRIPTION
 *      Searches for the first entry in the stream's sink list that needs
 *      to be aliased and performs an alias operation is needed.
 *      Returns FALSE if none needed.
 *----------------------------------------------------------------------------*/
CsrBool CsrAmFindAndDoSinkToAlias(AmInstanceDataType *inst)
{
    CsrBool needAlias = FALSE;
    CsrUint16 firstSinkIdL, firstSinkIdR;
    EndpointType *ep = (EndpointType *) CsrCmnListElementGetFirst(&inst->currentStream->sinkList);

    firstSinkIdL = ep->amStreamAudioEndPointL;
    firstSinkIdR = ep->amStreamAudioEndPointR;
    /* The first sinks are "aliased" in principle... cannot be aliased with themselves! */
    ep->amStreamAudioEndPointAliased = TRUE;
    ep->amStreamAudioEndPointAliasedR = TRUE;
    /* FIND */
    do
    {
        if ((ep->amStreamAudioEndPointAliased) && (ep->amStreamAudioEndPointAliasedR))
        {
            ep = ep->next;
        }
        else /* DO */
        {
            if (!ep->amStreamAudioEndPointAliased)
            {
                CsrAmIssueBccmdStreamSnkAlias(firstSinkIdL, ep->amStreamAudioEndPointL);
            }
            else
            {
                CsrAmIssueBccmdStreamSnkAlias(firstSinkIdR, ep->amStreamAudioEndPointR);
            }
            inst->amCallback = CsrAmStreamAliasAddCfm;
            needAlias = TRUE;
        }
    } while ((ep != NULL) && (!needAlias));

    return needAlias;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmNonAudioGainConfigPresent
 *
 *  DESCRIPTION
 *      Searches the list of configuration keys requested from the app.
 *      Returns TRUE if a key is found in the list that is not related
 *      to configuring the audio gains.
 *----------------------------------------------------------------------------*/
CsrBool CsrAmNonAudioGainConfigPresent(CsrUint16 configLen, CsrUint8 *configData)
{
    /*
    CsrUint8 i;
    CsrBool retVal = TRUE;/ *FALSE;* /
    CsrUint8 len = (CsrUint8) (configLen / sizeof(AmEpConfigType));

    / * The config list contains pairs of type: key (32-bit)- value (32-bit)* /
    for (i=0; ((i < len) && (retVal == FALSE)); i++)
    {/ * Get the next key to configure and finc out whether it is related to the audio gain * /
        CsrUint32 key = ((AmEpConfigType *)&configData[i*sizeof(AmEpConfigType)])->key;

        if ((key != CSR_AM_FM_INPUT_GAIN) && (key != CSR_AM_FM_OUTPUT_GAIN) &&
            (key != CSR_AM_CODEC_DAC_GAIN) && (key != CSR_AM_CODEC_ADC_GAIN))
        {
            retVal = TRUE;
        }
    }

    return retVal;*/
    /* Found out that the configuration of the audio gain key demands disconnecting the stream and reconnecting
       it afterwards, which was not expected. Thus return TRUE so far. It might need changing when future chips
       and/or build ID's are used! */
    return TRUE;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmConfigDone
 *
 *  DESCRIPTION
 *      Decides whether the configuration operation has been performed or
 *      whether it is still to be done. Returns TRUE if performed and FALSE
 *      if configuration is ongoing or about to start.
 *----------------------------------------------------------------------------*/
CsrBool CsrAmConfigDone(StreamType *str)
{
    CsrBool sinkConfigExists = FALSE;

    str->amSinkList = (EndpointType *) CsrCmnListElementGetFirst(&str->sinkList);
    if (str->amSinkList != NULL)
    {
        do
        {
            if (str->amSinkList->amStreamAudioEndPointConfigLen > 0)
            {
                sinkConfigExists = TRUE;
            }
            else
            {
                str->amSinkList = str->amSinkList->next;
            }
        } while ((str->amSinkList != NULL) && (!sinkConfigExists));
    }

    if ((str->amSource.amStreamAudioEndPointConfigLen > 0) ||
        (sinkConfigExists == TRUE))
    { /* Configuration not performed yet */
        return FALSE;
    }

    /* Configuration performed */
    return TRUE;
}

void CsrAmFreeStreamListAndNewSinkList(AmInstanceDataType *inst)
{
    CsrCmnListDeinit(&inst->newSinkList);

    inst->currentStream = (StreamType *) CsrCmnListElementGetFirst(&inst->streamList);
    while (inst->currentStream != NULL)
    {
        inst->currentStream->amSinkList = (EndpointType *) CsrCmnListElementGetFirst(&inst->currentStream->sinkList);
        while (inst->currentStream->amSinkList != NULL)
        {
            CsrPmemFree(inst->currentStream->amSinkList->amStreamAudioEndPointConfig);
            CsrCmnListElementRemove(&inst->currentStream->sinkList, (CsrCmnListElm_t *) inst->currentStream->amSinkList);
            inst->currentStream->amSinkList = (EndpointType *) CsrCmnListElementGetFirst(&inst->currentStream->sinkList);
        }
        CsrCmnListElementRemove(&inst->streamList, (CsrCmnListElm_t *) inst->currentStream);
        inst->currentStream = (StreamType *) CsrCmnListElementGetFirst(&inst->streamList);
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmGetEndpointToConfigBySinkType
 *
 *  DESCRIPTION
 *      Finds the endpoint in a stream that needs configuring and returns
 *      a pointer to it.
 *----------------------------------------------------------------------------*/
EndpointType *CsrAmGetEndpointToConfigBySinkType(CsrCmnList_t *list, CsrUint16 epType)
{
    EndpointType *ep = (EndpointType *) CsrCmnListElementGetFirst(list);

    while (ep != NULL)
    {
        if (ep->amStreamAudioEndPointType.endPoint == epType)
        {
            return ep;
        }
        else
        {
            ep = ep->next;
        }
    }

    return ep;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmGetEndpointToConfig
 *
 *  DESCRIPTION
 *      Finds the first endpoint in a stream that needs configuring ans returns
 *      a pointer to it and finds the endpoint Id associated. It removes the
 *      configuration item if needed.
 *----------------------------------------------------------------------------*/
EndpointType *CsrAmGetEndpointToConfig(StreamType *str, CsrUint16 *sid, CsrAmEndpointType epType, CsrBool removeItem)
{
    if (str->amSource.amStreamAudioEndPointConfigLen > 0) /* The source endpoint may need configuring */
    {
        if (removeItem && (str->amSource.amStreamAudioEndPointLConfigDone == FALSE))
        {
            str->amSource.amStreamAudioEndPointConfigLen--;
        }

        if (str->amSource.amStreamAudioEndPointConfigLen > 0) /* The source endpoint needs configuring */
        {
            if ((epType == CSR_AM_EP_SOURCE_RIGHT) || (epType == CSR_AM_EP_ALL_RIGHT) || (str->amSource.amStreamAudioEndPointLConfigDone == TRUE)) /* Only the right side */
            {
                *sid = str->amSource.amStreamAudioEndPointR;
                str->amSource.amStreamAudioEndPointLConfigDone = FALSE;
            }
            else
            {
                *sid = str->amSource.amStreamAudioEndPointL;
                if ((epType == CSR_AM_EP_SOURCE_ALL) || (epType == CSR_AM_EP_ALL_BOTH_SIDES)) /* Make sure the right side is configured also before removing the configure key */
                {
                    str->amSource.amStreamAudioEndPointLConfigDone = TRUE;
                }
            }
            return (EndpointType *) &(str->amSource);
        }
        else /* Source configuration done: remove config list*/
        {
            CsrPmemFree(str->amSource.amStreamAudioEndPointConfig);
            str->amSource.amStreamAudioEndPointConfig = NULL;
        }
    }


    /* Find the first sink in the list with configuration data */
    str->amSinkList = (EndpointType *) CsrCmnListElementGetFirst(&str->sinkList);

    if (str->amSinkList == NULL)
    {
        return NULL;
    }
    else
    {
        CsrBool found = FALSE;
        do
        {
            if (str->amSinkList->amStreamAudioEndPointConfigLen > 0) /* Sink found: determine the sid */
            {
                if (removeItem && (str->amSinkList->amStreamAudioEndPointLConfigDone == FALSE))
                {
                    str->amSinkList->amStreamAudioEndPointConfigLen--;
                }

                if (str->amSinkList->amStreamAudioEndPointConfigLen > 0)
                {
                    found = TRUE;
                    if ((epType == CSR_AM_EP_SINK_RIGHT) || (epType == CSR_AM_EP_ALL_RIGHT_SINKS) || (epType == CSR_AM_EP_ALL_RIGHT) ||
                        (str->amSinkList->amStreamAudioEndPointLConfigDone == TRUE))
                    {
                        *sid = str->amSinkList->amStreamAudioEndPointR;
                        str->amSinkList->amStreamAudioEndPointLConfigDone = FALSE;
                    }
                    else
                    {
                        *sid = str->amSinkList->amStreamAudioEndPointL;
                        if ((epType == CSR_AM_EP_SINK_BOTH_SIDES) || (epType == CSR_AM_EP_ALL_SINKS_BOTH_SIDES) ||
                            (epType == CSR_AM_EP_ALL_BOTH_SIDES))
                        { /* Make sure the right side is configured also before removing the configure key */
                            str->amSinkList->amStreamAudioEndPointLConfigDone = TRUE;
                        }
                    }
                }
                else /* Sink configured: remove the config pointer and check the next sink */
                {
                    CsrPmemFree(str->amSinkList->amStreamAudioEndPointConfig);
                    str->amSinkList->amStreamAudioEndPointConfig = NULL;
                    str->amSinkList = str->amSinkList->next;
                }
            }
            else /* Not found; keep on looking until the end of the list */
            {
                str->amSinkList = str->amSinkList->next;
            }
        } while ((!found) && (str->amSinkList != NULL));


        return str->amSinkList;
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmRestoreSavedMessages
 *
 *  DESCRIPTION
 *      Changes back to idle state and issues a house cleaning primitive
 *      to handle the stored messages.
 *----------------------------------------------------------------------------*/
void CsrAmRestoreSavedMessages(AmInstanceDataType *inst)
{
    CSR_AM_CHANGE_STATE(inst->myState, AM_IDLE_STATE);

    if (inst->saveQueue) /* Downstream messages received during previous handling in
         busy state. Make sure that the messages are restored    */
    {
        inst->restoreFlag = TRUE;
        CsrSchedMessagePut(CsrSchedTaskQueueGet(), CSR_AM_PRIM, NULL);
    }
    else /* No downstream messages received during previous handling in
        busy state. Ignore the message                       */
    {
    }
}

/*************************************************** Callback functions *******************************************/
/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmSrcAllocCfm
 *
 *  DESCRIPTION
 *      Callback function when a source is allocated
 *----------------------------------------------------------------------------*/
void CsrAmSrcAllocCfm(void *inst)
{
    AmInstanceDataType *instData = (AmInstanceDataType *) inst;
    CsrAmPrim *appPrim = (CsrAmPrim *) instData->rxDownstreamMsgP;

    switch (*appPrim)
    {
        case CSR_AM_AUDIO_PATH_CONNECT_REQ:
        {
            CsrAmAudioPathConnectReq *prim = (CsrAmAudioPathConnectReq *) appPrim;
            CsrUint16 srcId = CsrAmGetBcValue((CsrBccmdCfm *) instData->rxMsgP);

            if (instData->currentStream->amSource.amStreamAudioEndPointL == STREAM_TYPE_NO_ENDPOINT)
            {
                instData->currentStream->amSource.amStreamAudioEndPointL = srcId;

                if (instData->currentStream->amStereo) /* Allocate a right side of the source */
                {
                    CsrAmEndPointDefType ep;

                    if (prim->sourceR.endPoint.endPoint != CSR_AM_ENDPOINT_ALL)
                    {
                        ep.endPoint = prim->sourceR.endPoint.endPoint;
                        if (prim->sourceR.endPoint.instance == CSR_AM_INSTANCE_AUTO)
                        {
                            prim->sourceR.endPoint.instance = prim->source.endPoint.instance;
                        }

                        ep.instance = prim->sourceR.endPoint.instance;

                        if (prim->sourceR.endPoint.channel == CSR_AM_CHANNEL_AUTO)
                        {
                            prim->sourceR.endPoint.channel = CSR_AM_CHANNEL_0;
                            if ((prim->sourceR.endPoint.endPoint == prim->source.endPoint.endPoint) &&
                                (prim->sourceR.endPoint.channel == prim->source.endPoint.channel))
                            {
                                prim->sourceR.endPoint.channel = (prim->source.endPoint.channel + 1) % CSR_AM_NUMBER_OF_CHANNELS;
                            }
                        }

                        ep.channel = prim->sourceR.endPoint.channel;
                    }
                    else /* Same endpoint as the left side if no right side specified. */
                    {
                        ep.endPoint = prim->sourceR.endPoint.endPoint = prim->source.endPoint.endPoint;

                        ep.instance = prim->sourceR.endPoint.instance = prim->source.endPoint.instance;
                        ep.channel = prim->sourceR.endPoint.channel = (prim->source.endPoint.channel + 1) % CSR_AM_NUMBER_OF_CHANNELS;
                    }
                    CsrAmIssueBccmdSetSource(ep.endPoint, ep.instance, ep.channel);
                    return;
                }
            }
            else
            {
                instData->currentStream->amSource.amStreamAudioEndPointR = srcId;
            }

            /* Now, shall the source be configured or is it time to allocate the sinks? */
            if (instData->currentStream->amSource.amStreamAudioEndPointConfigLen > 0) /* config sources. The configuration data has been copied already when the stream was created */
            {
                StreamType *str = instData->currentStream;
                AmEpConfigType *configData = &str->amSource.amStreamAudioEndPointConfig[str->amSource.amStreamAudioEndPointConfigLen - 1];
                /* Find out which sid to use... */
                CsrUint16 sid = str->amSource.amStreamAudioEndPointL;
                if (prim->sourceR.configDataCount)
                {
                    sid = str->amSource.amStreamAudioEndPointR;
                }
                CsrAmIssueBccmdConfigStream(sid, configData->key, configData->value);
                instData->amCallback = CsrAmStreamConfigCfm;
            }
            else /* Add sinks */
            {
                instData->sinksToAdd = (EndpointType *) CsrCmnListElementGetFirst(&instData->newSinkList);
                CsrAmIssueBccmdSetSink(instData->sinksToAdd->amStreamAudioEndPointType.endPoint,
                    instData->sinksToAdd->amStreamAudioEndPointType.instance,
                    instData->sinksToAdd->amStreamAudioEndPointType.channel);
                instData->amCallback = CsrAmSinkAllocCfm;
            }
            break;
        }
        default:
        {
            CSR_LOG_TEXT_ERROR((CsrAmLto, 0, "Unexpected message, handling request 0x%04X", *appPrim));
            break;
        }
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmSinkAllocCfm
 *
 *  DESCRIPTION
 *      Callback function when a sink is allocated
 *----------------------------------------------------------------------------*/
void CsrAmSinkAllocCfm(void *inst)
{
    AmInstanceDataType *instData = (AmInstanceDataType *) inst;
    CsrAmPrim *amPrim = (CsrAmPrim *) instData->rxDownstreamMsgP;
    /* If error: send confirm to app and de-allocate source(s) and sink(s) if needed
       else, this can be due to connect or to add sink, configure the endpoints.
       If due to connect, go on to connect the stream when all the endpoints are
       configured;
       if due to add sink, disconnect the existing stream when the new sinks are configured,
       alias the new sinks to the first sink in the list, and then re-connect */
    if (((CsrBccmdCfm *) instData->rxMsgP)->status != CSR_RESULT_SUCCESS) /* error. This can only be a CSR_AM_AUDIO_PATH_CONNECT_REQ or a CSR_AM_AUDIO_PATH_ADD_SINK_REQ */
    {
        if (*amPrim == CSR_AM_AUDIO_PATH_ADD_SINK_REQ) /* If only one new sink allocated, just bail out: send confirm, go back to idle and
            restore the message queue. The stream has not even been disconnected yet */
        {
            if (instData->sinkAddedcount == 0) /* No need to keep the new sinks list: de-allocate */
            {
                CsrAmDestroyLinkOfSinksToAdd(instData);
                CsrAmAudioPathAddSinkCfmSend(instData, CSR_RESULT_FAILURE, ((CsrAmAudioPathAddSinkReq *) amPrim)->amConnectionId);
                CsrAmFreeOperationMessage(instData);
                /* Go to idle and restore message queue */
                CsrAmRestoreSavedMessages(instData);
            }
            else /* Need to deallocate the allocated sinks before sending the confirm.
                Start de-allocating the first element in the list, remove it when de-allocated and
                decrease the counter of allocated sinks until the counter is 0. Then remove the whole
                list of new sinks, send confirm, go back to idle and */
            {
                instData->sinksToAdd = (EndpointType *) CsrCmnListElementGetFirst(&instData->newSinkList);
                /* Always close the left side first... */
                CsrAmIssueBccmdCloseSink(instData->sinksToAdd->amStreamAudioEndPointL);
                instData->amCallback = CsrAmSinkDeallocCfm;
            }
        }
        else /* CSR_AM_AUDIO_PATH_CONNECT_REQ: the sink allocation failed, we need to deallocate any sinks
            that may have been allocated already and the source(s) that will have been allocated already */
        {
            CsrAmIssueBccmdCloseSource(instData->currentStream->amSource.amStreamAudioEndPointL);
            instData->amCallback = CsrAmSrcDeallocCfm;
        }
    }
    else /* Allocation went well: store the new Id in the proper place and increase the counter */
    {
        CsrUint16 sinkId = CsrAmGetBcValue((CsrBccmdCfm *) instData->rxMsgP);
        EndpointType *ep = CsrAmStoreNewSinkId(instData, sinkId);
        /* No configuration needed: update the counter */
        CsrAmEndPointDefType *epDef;
        if ((ep->amStreamAudioEndPointR == STREAM_TYPE_NO_ENDPOINT) &&
            (ep->amStreamAudioEndPointTypeR.endPoint != STREAM_TYPE_NO_ENDPOINT))
        { /* Remember the right side */
            CsrAmIssueBccmdSetSink(ep->amStreamAudioEndPointTypeR.endPoint, ep->amStreamAudioEndPointTypeR.instance, ep->amStreamAudioEndPointTypeR.channel);
        }
        else
        {
            EndpointType *configEp;
            CsrUint16 sid;

            instData->sinkAddedcount++;
            epDef = CsrAmGetNewSinkToAlloc(instData);

            if (epDef != NULL)
            {
                CsrAmIssueBccmdSetSink(epDef->endPoint, epDef->instance, epDef->channel);
            }
            else if ((configEp = CsrAmFindNewSinkToConfig(instData, FALSE, &sid)) != NULL) /* Need to configure the allocated sinks. Always configure the last key received first;
                when config confirm arrives, update the length variable appropriately and configure
                the next config key if any. When length is 0, de-allocate the config pointer and
                forget all about the configuration setting */
            {
                AmEpConfigType *configData = &configEp->amStreamAudioEndPointConfig[configEp->amStreamAudioEndPointConfigLen - 1];

                CsrAmIssueBccmdConfigStream(sid, configData->key, configData->value);
                instData->amCallback = CsrAmStreamConfigCfm;
            }
            else /* All allocated, stored and configured: remove list of new sinks to add and
                either alias this/these to the first sink in the existing stream (if more than one sink
                exists) or connect first sink to the source. */
            {
                CsrBool doAlias = ((instData->sinkAddedcount > 1) || (*amPrim == CSR_AM_AUDIO_PATH_ADD_SINK_REQ));

                if (!doAlias)
                {
                    ep->amStreamAudioEndPointAliased = TRUE;
                    ep->amStreamAudioEndPointAliasedR = TRUE;
                }
                CsrAmAddNewSinkToStreamSinkList(instData);

                if ((!instData->streamTransOpen) && (instData->streamTransSupported))
                {
                    CsrAmIssueBccmdStreamTransStart();
                    instData->amCallback = CsrAmStreamTransStartCfm;
                }
                else if (doAlias) /* Alias */
                {
                    CsrAmFindAndDoSinkToAlias(instData);
                }
                else /* Connect */
                {
                    StreamType *str = instData->currentStream;

                    str->amSinkList = (EndpointType *) CsrCmnListElementGetFirst(&str->sinkList);
                    CsrAmIssueBccmdStreamConnect(str->amSource.amStreamAudioEndPointL, str->amSinkList->amStreamAudioEndPointL);
                    instData->amCallback = CsrAmStreamConnectCfm;
                }
            }
        }
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmStreamTransStartCfm
 *
 *  DESCRIPTION
 *      Callback function after starting a transaction
 *----------------------------------------------------------------------------*/
void CsrAmStreamTransStartCfm(void *inst)
{
    AmInstanceDataType *instData = (AmInstanceDataType *) inst;
    CsrAmPrim *amPrim = (CsrAmPrim *) instData->rxDownstreamMsgP;

    if (((CsrBccmdCfm *) instData->rxMsgP)->status == CSR_RESULT_SUCCESS)
    {
        instData->streamTransOpen = TRUE;
    }
    else /* Start transaction failed: the FW does not support it! */
    {
        instData->streamTransSupported = FALSE;
    }

    switch (*amPrim)
    {
        case CSR_AM_AUDIO_PATH_REMOVE_SINK_REQ:
        {
            if (instData->currentStream->amStreamTransformIdL != STREAM_ID_NONE) /* Now go on to release the stream */
            {
                CsrAmIssueBccmdStreamDisconnect(instData->currentStream->amStreamTransformIdL);
                instData->amCallback = CsrAmStreamDisconnectCfm;
            }
            else /* Transaction open while removing sink and no connection exists: this must be a re-connection. */
            {
                EndpointType *ep = (EndpointType *) CsrCmnListElementGetFirst(&instData->currentStream->sinkList);
                CsrAmIssueBccmdStreamConnect(instData->currentStream->amSource.amStreamAudioEndPointL, ep->amStreamAudioEndPointL);
                instData->amCallback = CsrAmStreamConnectCfm;
            }
            break;
        }

        case CSR_AM_AUDIO_PATH_CONFIG_REQ:
        {
            CsrAmAudioPathConfigReq *prim = (CsrAmAudioPathConfigReq *) amPrim;

            if (!CsrAmNonAudioGainConfigPresent(prim->configDataLen, prim->configData)) /* Audio config: just send config without disconnecting the stream first! */
            {
                CsrUint16 sid;
                EndpointType *ep = CsrAmGetEndpointToConfig(instData->currentStream, &sid, prim->epType, FALSE);
                if (ep != NULL)
                {
                    AmEpConfigType *configData = &ep->amStreamAudioEndPointConfig[ep->amStreamAudioEndPointConfigLen - 1];
                    CsrAmIssueBccmdConfigStream(sid, configData->key, configData->value);
                    instData->amCallback = CsrAmStreamConfigCfm;
                }
                else
                {
                    CsrAmAudioPathConfigCfmSend(instData, CSR_RESULT_FAILURE, prim->amConnectionId);
                    CsrAmRestoreSavedMessages(instData);
                    CsrAmFreeOperationMessage(instData);
                }
            }
            else
            {
                StreamType *str = instData->currentStream;
                if (str->amStreamTransformIdL != STREAM_ID_NONE) /* Need to disconect first...*/
                {
                    CsrAmIssueBccmdStreamDisconnect(str->amStreamTransformIdL);
                    instData->amCallback = CsrAmStreamDisconnectCfm;
                }
                else
                {
                    CsrUint16 sid;
                    EndpointType *ep = CsrAmGetEndpointToConfig(instData->currentStream, &sid, ((CsrAmAudioPathConfigReq *) amPrim)->epType, TRUE);
                    if (ep != NULL)
                    {
                        AmEpConfigType *configData = &ep->amStreamAudioEndPointConfig[ep->amStreamAudioEndPointConfigLen - 1];
                        CsrAmIssueBccmdConfigStream(sid, configData->key, configData->value);
                        instData->amCallback = CsrAmStreamConfigCfm;
                    }
                    else /* All configured. Now Alias, re-connect, etc... if needed */
                    {
                        str = instData->currentStream;
                        str->amSinkList = (EndpointType *) CsrCmnListElementGetFirst(&str->sinkList);
                        CsrAmIssueBccmdStreamConnect(str->amSource.amStreamAudioEndPointL, str->amSinkList->amStreamAudioEndPointL);
                        instData->amCallback = CsrAmStreamConnectCfm;
                    }
                }
            }
            break;
        }
        case CSR_AM_AUDIO_PATH_RELEASE_REQ:
        { /* Now go on to release the stream */
            CsrAmIssueBccmdStreamDisconnect(instData->currentStream->amStreamTransformIdL);
            instData->amCallback = CsrAmStreamDisconnectCfm;
            break;
        }

        case CSR_AM_AUDIO_PATH_ADD_SINK_REQ:
        { /* Alias */
            CsrAmAddNewSinkToStreamSinkList(instData);
            CsrAmFindAndDoSinkToAlias(instData);
            break;
        }

        case CSR_AM_AUDIO_PATH_CONNECT_REQ:
        {
            /* Add the new sinks to the list of sinks in the stream,
               alias all the new sinks if needed and connect source to first sink entry... */
            if (!CsrAmFindAndDoSinkToAlias(instData))
            {
                EndpointType *ep = (EndpointType *) CsrCmnListElementGetFirst(&instData->currentStream->sinkList);
                CsrAmIssueBccmdStreamConnect(instData->currentStream->amSource.amStreamAudioEndPointL, ep->amStreamAudioEndPointL);
                instData->amCallback = CsrAmStreamConnectCfm;
            }
            /* else a bccmd has been sent; wait for callback function to be called */
            break;
        }

        default:
        {
            break;
        }
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmStreamTransCompleteCfm
 *
 *  DESCRIPTION
 *      Callback function after completing a transaction
 *----------------------------------------------------------------------------*/
void CsrAmStreamTransCompleteCfm(void *inst)
{
    AmInstanceDataType *instData = (AmInstanceDataType *) inst;
    CsrAmPrim *amPrim = (CsrAmPrim *) instData->rxDownstreamMsgP;

    instData->streamTransOpen = FALSE;

    switch (*amPrim)
    {
        case CSR_AM_AUDIO_PATH_RELEASE_REQ:
        {
            CsrAmIssueBccmdCloseSource(instData->currentStream->amSource.amStreamAudioEndPointL);
            instData->amCallback = CsrAmSrcDeallocCfm;
            break;
        }
        case CSR_AM_AUDIO_PATH_REMOVE_SINK_REQ:
        {
            /* Remove the sink requested (alias) from the list of sinks and re-connect all the others
               If the sinklist is empty after that, then handle this as a path release: de-allocate
               the source(s) and send both a remove sink cfm and a path release ind to the app.
               Remember to deallocate the sink(s) before removing all information about it/them   */
            EndpointType *sinkToRemove;

            instData->currentStream->amSinkList = (EndpointType *) CsrCmnListElementGetFirst(&instData->currentStream->sinkList);
            sinkToRemove = CsrAmFindEndPointBySinkType(instData->currentStream->amSinkList,
                ((CsrAmAudioPathRemoveSinkReq *) amPrim)->sinkId);
            if (sinkToRemove != NULL) /* Removing sink(s) */
            {
                CsrAmIssueBccmdCloseSink(sinkToRemove->amStreamAudioEndPointL);
                instData->amCallback = CsrAmSinkDeallocCfm;
            }
            else /* Sink(s) removed. Now find out what to do; if re-connect needed, the transaction needs to be opened again */
            {
                sinkToRemove = (EndpointType *) CsrCmnListElementGetFirst(&instData->currentStream->sinkList);
                if ((instData->currentStream->amStreamTransformIdL == STREAM_ID_NONE) && (sinkToRemove != NULL) && (instData->streamTransSupported)) /* Need to connect */
                {
                    CsrAmIssueBccmdStreamTransStart();
                    instData->amCallback = CsrAmStreamTransStartCfm;
                }
                else /* The sink(s) removed is the last one and the sources have been removed as well... this is infact a disconnection */
                {
                    EndpointType *ep = &(instData->currentStream->amSource);
                    CsrAmAudioPathRemoveSinkCfmSend(instData, CSR_RESULT_SUCCESS, ((CsrAmAudioPathRemoveSinkReq *) amPrim)->amConnectionId);

                    if ((ep->amStreamAudioEndPointL == STREAM_TYPE_NO_ENDPOINT) &&
                        ((ep->amStreamAudioEndPointR == STREAM_TYPE_NO_ENDPOINT) || (!instData->currentStream->amStereo)))
                    { /* Source(s) removed as well */
                        CsrAmAudioPathReleaseIndSend(instData, ((CsrAmAudioPathRemoveSinkReq *) amPrim)->amConnectionId);
                        /* Remove stream from the list */
                        CsrCmnListElementRemove(&instData->streamList, (CsrCmnListElm_t *) instData->currentStream);
                    }
                    /* Restore queue and go back to idle */
                    CsrAmRestoreSavedMessages(instData);
                    CsrAmFreeOperationMessage(instData);
                }
            }
            break;
        }

        case CSR_AM_AUDIO_PATH_ADD_SINK_REQ:
        case CSR_AM_AUDIO_PATH_CONNECT_REQ:
        {
            if (CSR_AM_AUDIO_PATH_ADD_SINK_REQ == *amPrim)
            {
                CsrAmAudioPathAddSinkCfmSend(instData, CSR_RESULT_SUCCESS, ((CsrAmAudioPathAddSinkReq *) amPrim)->amConnectionId);
            }
            else /* Connect */
            {
                CsrUint16 connId;
                /* Generate a new connection Id for the app. This will remain unchanged until the stream instance
                    is completely removed */
                connId = CsrAmGenerateAppConnId(instData);
                if (connId != STREAM_ID_NONE)
                {
                    CsrAmAudioPathConnectCfmSend(instData, CSR_RESULT_SUCCESS, connId);
                }
                else /* This should not happen at all! And if it did happen, the sources and sinks should be de-allocated first.
                    The whole stream would need to be taken down.....*/
                {
                    CsrAmAudioPathConnectCfmSend(instData, CSR_RESULT_FAILURE, connId);
                }
            }
            CsrAmRestoreSavedMessages(instData);
            CsrAmFreeOperationMessage(instData);
            break;
        }

        case CSR_AM_AUDIO_PATH_CONFIG_REQ:
        {
            if (instData->currentStream->amStreamTransformIdL != STREAM_ID_NONE)
            {
                CsrAmAudioPathConfigCfmSend(instData, CSR_RESULT_SUCCESS, ((CsrAmAudioPathConfigReq *) amPrim)->amConnectionId);
                CsrAmRestoreSavedMessages(instData);
                CsrAmFreeOperationMessage(instData);
            }
            else /* Disconnected: now configure....*/
            {
                CsrAmAudioPathConfigReq *prim = (CsrAmAudioPathConfigReq *) amPrim;
                CsrUint16 sid;
                EndpointType *ep = CsrAmGetEndpointToConfig(instData->currentStream, &sid, prim->epType, FALSE);
                if (ep != NULL)
                {
                    AmEpConfigType *configData = &ep->amStreamAudioEndPointConfig[ep->amStreamAudioEndPointConfigLen - 1];
                    CsrAmIssueBccmdConfigStream(sid, configData->key, configData->value);
                    instData->amCallback = CsrAmStreamConfigCfm;
                }
                else /* Start re-connection... */
                {
                    StreamType *str = instData->currentStream;

                    str->amSinkList = (EndpointType *) CsrCmnListElementGetFirst(&str->sinkList);
                    CsrAmIssueBccmdStreamConnect(str->amSource.amStreamAudioEndPointL, str->amSinkList->amStreamAudioEndPointL);
                    instData->amCallback = CsrAmStreamConnectCfm;
                }
            }
            break;
        }

        default:
        {
            break;
        }
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmStreamConnectCfm
 *
 *  DESCRIPTION
 *      Callback function after a connect bccmd
 *----------------------------------------------------------------------------*/
void CsrAmStreamConnectCfm(void *inst)
{
    AmInstanceDataType *instData = (AmInstanceDataType *) inst;
    CsrUint16 strTransformId = CsrAmGetBcValue((CsrBccmdCfm *) instData->rxMsgP);
    StreamType *str = instData->currentStream;

    if (str->amStreamTransformIdL == STREAM_ID_NONE)
    {
        str->amStreamTransformIdL = strTransformId;
        /* Is this a stereo connection? */
        if ((str->amStereo) &&
            (str->amSource.amStreamAudioEndPointR != STREAM_TYPE_NO_ENDPOINT))
        {
            CsrAmIssueBccmdStreamConnect(str->amSource.amStreamAudioEndPointR, str->amSinkList->amStreamAudioEndPointR);
        }
    }
    else /* Left side connected; this must be the right side */
    {
        str->amStreamTransformIdR = strTransformId;
    }

    if ((str->amStreamTransformIdL != STREAM_TYPE_NO_ENDPOINT) &&
        ((str->amStreamTransformIdR != STREAM_TYPE_NO_ENDPOINT) || (!str->amStereo)))
    {
        /* Finished connecting: either the audio connection is mono or both sides connected.
           Now find out what to do next */
        CsrAmPrim *amPrim = (CsrAmPrim *) instData->rxDownstreamMsgP;
        switch (*amPrim)
        {
            case CSR_AM_AUDIO_PATH_REMOVE_SINK_REQ:
            {
                if ((instData->streamTransOpen) && (instData->streamTransSupported)) /* Close stream transaction */
                {
                    CsrAmIssueBccmdStreamTransComplete();
                    instData->amCallback = CsrAmStreamTransCompleteCfm;
                }
                else
                {
                    CsrAmAudioPathRemoveSinkCfmSend(instData, CSR_RESULT_SUCCESS, ((CsrAmAudioPathRemoveSinkReq *) amPrim)->amConnectionId);
                    CsrAmRestoreSavedMessages(instData);
                    CsrAmFreeOperationMessage(instData);
                }
                break;
            }

            case CSR_AM_AUDIO_PATH_CONFIG_REQ:
            {
                if ((instData->streamTransOpen) && (instData->streamTransSupported)) /* Close stream transaction */
                {
                    CsrAmIssueBccmdStreamTransComplete();
                    instData->amCallback = CsrAmStreamTransCompleteCfm;
                }
                else
                {
                    CsrAmAudioPathConfigCfmSend(instData, CSR_RESULT_SUCCESS, ((CsrAmAudioPathConfigReq *) amPrim)->amConnectionId);
                    CsrAmRestoreSavedMessages(instData);
                    CsrAmFreeOperationMessage(instData);
                }
                break;
            }

            case CSR_AM_AUDIO_PATH_ADD_SINK_REQ:
            case CSR_AM_AUDIO_PATH_CONNECT_REQ:
            { /* Close transform if open and no alias needed */
                if (!CsrAmFindAndDoSinkToAlias(instData))
                {
                    if ((instData->streamTransOpen) && (instData->streamTransSupported)) /* Close stream transaction */
                    {
                        CsrAmIssueBccmdStreamTransComplete();
                        instData->amCallback = CsrAmStreamTransCompleteCfm;
                    }
                    else /* Need for alias? */
                    {
                        CsrUint16 connId;
                        /* Generate a new connection Id for the app. This will remain unchanged until the stream instance
                            is completely removed */
                        connId = CsrAmGenerateAppConnId(instData);
                        if (connId != STREAM_ID_NONE)
                        {
                            CsrAmAudioPathConnectCfmSend(instData, CSR_RESULT_SUCCESS, connId);
                        }
                        else /* This should not happen at all! And if it did happen, the sources and sinks should be de-allocated first.
                            The whole stream would need to be taken down.....*/
                        {
                            CsrAmAudioPathConnectCfmSend(instData, CSR_RESULT_FAILURE, connId);
                        }
                        CsrAmRestoreSavedMessages(instData);
                        CsrAmFreeOperationMessage(instData);
                    }
                }
                /* else the find and do sink to alias will have issued an alias bccmd: wait for response */
                break;
            }

            default:
            {
                break;
            }
        }
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmStreamSyncSidCfm
 *
 *  DESCRIPTION
 *      Callback function after a sync bccmd. Needed at connect and add sink
 *----------------------------------------------------------------------------*/
void CsrAmStreamSyncSidCfm(void *inst)
{
    AmInstanceDataType *instData = (AmInstanceDataType *) inst;
    EndpointType *ep;

    /* At this point, at least one entry needed to get sync'ed... */
    CsrAmNeedToSyncEndpoints(inst, &ep);
    ep->amStreamAudioEndPointSynced = TRUE;
    if (((CsrBccmdCfm *) instData->rxMsgP)->status == CSR_BCCMD_RESULT_NO_SUCH_VARID) /* Sync command not understood by the FW; just skip sync operation */
    {
        CsrAmMarkAllSyncEndpoints(inst);
    }
    /* Any more needed ? */
    if (CsrAmNeedToSyncEndpoints(inst, &ep)) /* Yes. Keep on doing it */
    {
        CsrAmIssueBccmdStreamSyncEndpoints(ep->amStreamAudioEndPointL, ep->amStreamAudioEndPointR);
    }
    else /* No. What next? */
    {   /* Connect stream: open transform and connect source to first sink */
        if ((!instData->streamTransOpen) && (instData->streamTransSupported))
        {
            CsrAmIssueBccmdStreamTransStart();
            instData->amCallback = CsrAmStreamTransStartCfm;
        }
        else /* Transform already open; just connect */
        {
            CsrAmPrim *amPrim = (CsrAmPrim *) instData->rxDownstreamMsgP;
            if (*amPrim == CSR_AM_AUDIO_PATH_CONNECT_REQ)
            {
                ep = (EndpointType *) CsrCmnListElementGetFirst(&instData->currentStream->sinkList);
                CsrAmIssueBccmdStreamConnect(instData->currentStream->amSource.amStreamAudioEndPointL, ep->amStreamAudioEndPointL);
                instData->amCallback = CsrAmStreamConnectCfm;
            }
            else /* Adding sink: first disconnect, then connect and alias sinks */
            {
                CsrAmIssueBccmdStreamDisconnect(instData->currentStream->amStreamTransformIdL);
                instData->amCallback = CsrAmStreamDisconnectCfm;
            }
        }
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmStreamAliasAddCfm
 *
 *  DESCRIPTION
 *      Callback function after an alias bccmd. Needed at connect and add sink
 *----------------------------------------------------------------------------*/
void CsrAmStreamAliasAddCfm(void *inst)
{
    AmInstanceDataType *instData = (AmInstanceDataType *) inst;
    /* Update aliased entry (it is the first one with a FALSE alias entry */
    EndpointType *ep = (EndpointType *) CsrCmnListElementGetFirst(&instData->currentStream->sinkList);
    CsrBool handled = FALSE;
    CsrAmPrim *amPrim = (CsrAmPrim *) instData->rxDownstreamMsgP;

    do
    {
        if (!ep->amStreamAudioEndPointAliased)
        {
            ep->amStreamAudioEndPointAliased = TRUE;
            handled = TRUE;
        }
        else if (!ep->amStreamAudioEndPointAliasedR)
        {
            ep->amStreamAudioEndPointAliasedR = TRUE;
            handled = TRUE;
        }
        else
        {
            ep = ep->next;
        }
    } while ((ep != NULL) && (handled == FALSE));

    /* Now check whether more alias are needed */
    if (!CsrAmFindAndDoSinkToAlias(instData))
    {
        if (*amPrim == CSR_AM_AUDIO_PATH_CONNECT_REQ) /* Now connect stream */
        {
            StreamType *str = instData->currentStream;

            str->amSinkList = (EndpointType *) CsrCmnListElementGetFirst(&str->sinkList);
            CsrAmIssueBccmdStreamConnect(str->amSource.amStreamAudioEndPointL, str->amSinkList->amStreamAudioEndPointL);
            instData->amCallback = CsrAmStreamConnectCfm;
        }
        else if ((instData->streamTransOpen) && (instData->streamTransSupported)) /* Close stream transaction */
        {
            CsrAmIssueBccmdStreamTransComplete();
            instData->amCallback = CsrAmStreamTransCompleteCfm;
        }
        else
        {
            if (*amPrim == CSR_AM_AUDIO_PATH_ADD_SINK_REQ)
            {
                CsrAmAudioPathAddSinkCfmSend(instData, CSR_RESULT_SUCCESS, ((CsrAmAudioPathAddSinkReq *) amPrim)->amConnectionId);
            }
            else /* Connecting */
            {
                CsrUint16 connId;
                /* Generate a new connection Id for the app. This will remain unchanged until the stream instance
                    is completely removed */
                connId = CsrAmGenerateAppConnId(instData);
                if (connId != STREAM_ID_NONE)
                {
                    CsrAmAudioPathConnectCfmSend(instData, CSR_RESULT_SUCCESS, connId);
                }
                else /* This should not happen at all! And if it did happen, the sources and sinks should be de-allocated first.
                    The whole stream would need to be taken down.....*/
                {
                    CsrAmAudioPathConnectCfmSend(instData, CSR_RESULT_FAILURE, connId);
                }
            }

            CsrAmRestoreSavedMessages(instData);
            CsrAmFreeOperationMessage(instData);
        }
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmStreamConfigCfm
 *
 *  DESCRIPTION
 *      Callback function after a configuration bccmd
 *----------------------------------------------------------------------------*/
void CsrAmStreamConfigCfm(void *inst)
{
    AmInstanceDataType *instData = (AmInstanceDataType *) inst;
    CsrUint16 sid = CsrAmGetBcValue((CsrBccmdCfm *) instData->rxMsgP);
    CsrAmPrim *amPrim = (CsrAmPrim *) instData->rxDownstreamMsgP;
    /* Should we check whether this went well during add sink or connect
       or should it be relevant only during configure? */
    if (*amPrim == CSR_AM_AUDIO_PATH_CONFIG_REQ)
    {
        CsrBccmdCfm *bcPrim = (CsrBccmdCfm *) instData->rxMsgP;
        EndpointType *ep = CsrAmGetEndpointToConfig(instData->currentStream, &sid, ((CsrAmAudioPathConfigReq *) amPrim)->epType, TRUE);

        if ((bcPrim->status == CSR_RESULT_SUCCESS) && (ep != NULL))
        {
            AmEpConfigType *configData = &ep->amStreamAudioEndPointConfig[ep->amStreamAudioEndPointConfigLen - 1];
            CsrAmIssueBccmdConfigStream(sid, configData->key, configData->value);
        }
        else /* All configured. Now Alias, re-connect, etc... if needed */
        {
            if ((!instData->streamTransOpen) && (instData->streamTransSupported))
            {
                CsrAmIssueBccmdStreamTransStart();
                instData->amCallback = CsrAmStreamTransStartCfm;
            }
            else /* re-connect */
            {
                StreamType *str = instData->currentStream;

                str->amSinkList = (EndpointType *) CsrCmnListElementGetFirst(&str->sinkList);
                CsrAmIssueBccmdStreamConnect(str->amSource.amStreamAudioEndPointL, str->amSinkList->amStreamAudioEndPointL);
                instData->amCallback = CsrAmStreamConnectCfm;
            }
        }
    }
    else /* CSR_AM_AUDIO_PATH_CONNECT_REQ or CSR_AM_AUDIO_PATH_ADD_SINK_REQ */
    {
        CsrBool removeItem = TRUE;

        if (instData->currentStream->amSource.amStreamAudioEndPointConfigLen) /* Source configuration needed first. This can only be a connect request */
        {
            StreamType *str = instData->currentStream;
            AmEpConfigType *configData;
            CsrAmAudioPathConnectReq *prim = (CsrAmAudioPathConnectReq *) amPrim;
            CsrUint16 srcId = str->amSource.amStreamAudioEndPointL;

            removeItem = FALSE;
            str->amSource.amStreamAudioEndPointConfigLen--;
            if (prim->sourceR.configDataCount)
            {
                prim->sourceR.configDataCount--;
            }
            else
            {
                prim->source.configDataCount--;
            }
            if (prim->sourceR.configDataCount)
            {
                srcId = str->amSource.amStreamAudioEndPointR;
            }
            if (str->amSource.amStreamAudioEndPointConfigLen)
            {
                configData = &str->amSource.amStreamAudioEndPointConfig[str->amSource.amStreamAudioEndPointConfigLen - 1];
                CsrAmIssueBccmdConfigStream(srcId, configData->key, configData->value);
                return;
            }
            else /* Source configuration done: deallocate the pointer */
            {
                CsrPmemFree(str->amSource.amStreamAudioEndPointConfig);
                str->amSource.amStreamAudioEndPointConfig = NULL;
            }
        }

        {
            EndpointType *configEp;
            CsrUint16 snkId;
            if ((configEp = CsrAmFindNewSinkToConfig(instData, removeItem, &snkId)) != NULL)
            {
                AmEpConfigType *configData = &configEp->amStreamAudioEndPointConfig[configEp->amStreamAudioEndPointConfigLen - 1];
                CsrAmIssueBccmdConfigStream(snkId, configData->key, configData->value);
            }
            else
            {
                CsrAmEndPointDefType *epDef = CsrAmGetNewSinkToAlloc(instData);
                if (epDef != NULL)
                {
                    CsrAmIssueBccmdSetSink(epDef->endPoint, epDef->instance, epDef->channel);
                    instData->amCallback = CsrAmSinkAllocCfm;
                }
                else /* Done allocating sinks: now what? */
                {
                    /* Both int he case of adding sink(s) and establishing the stream connection, the operations to perform are:
                           - The sink(s) must all be added to the stream instance
                           - Left and right endpoint sides need to by sync'ed (if Jemima A12, A14 or A16 the operation will fail...)
                           - The new sink(s) must be aliased to the first sink;
                           - The source endpoint(s) shall be connected to the sink endpoint(s).
                           - The transform must be completed and the proper confirm sent to the application
                           - The sinks-to-add list and the configuration data pointers in it must be de-allocated.               */
                    instData->sinkAddedcount++;
                    CsrAmAddNewSinkToStreamSinkList(instData);
                    if (instData->currentStream->amStereo) /* Sync left and right sides. Just sync all of them even if they were sync'ed already.... */
                    {
                        EndpointType *epToSync;
                        if (CsrAmNeedToSyncEndpoints(instData, &epToSync))
                        {
                            CsrAmIssueBccmdStreamSyncEndpoints(epToSync->amStreamAudioEndPointL, epToSync->amStreamAudioEndPointR);
                            instData->amCallback = CsrAmStreamSyncSidCfm;
                        }
                        else /* No need to sync'e. Start the alias operation. At least one sink needs to be aliased with the first one.
                           Before being able to sync, the transform needs to be started and the stream needs to be disconnected (if adding sink)*/
                        {
                            if ((!instData->streamTransOpen) && (instData->streamTransSupported))
                            {
                                CsrAmIssueBccmdStreamTransStart();
                                instData->amCallback = CsrAmStreamTransStartCfm;
                            }
                            else
                            {
                                /* Just alias. No connection established yet. */
                                CsrAmFindAndDoSinkToAlias(instData);
                            }
                        }
                    }
                }
            }
        }
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmSrcDeallocCfm
 *
 *  DESCRIPTION
 *      Callback function after closing a source endpoint
 *----------------------------------------------------------------------------*/
void CsrAmSrcDeallocCfm(void *inst)
{
    AmInstanceDataType *instData = (AmInstanceDataType *) inst;
    CsrUint16 srcId = CsrAmGetBcValue((CsrBccmdCfm *) instData->rxMsgP);
    EndpointType *ep = &(instData->currentStream->amSource);

    if (ep->amStreamAudioEndPointL == srcId)
    {
        ep->amStreamAudioEndPointL = STREAM_TYPE_NO_ENDPOINT;
    }
    else if (ep->amStreamAudioEndPointR == srcId)
    {
        ep->amStreamAudioEndPointR = STREAM_TYPE_NO_ENDPOINT;
    }

    /* Remember to deallocate the configuration data! */
    if (ep->amStreamAudioEndPointConfigLen)
    {
        ep->amStreamAudioEndPointConfigLen = 0;
        CsrPmemFree(ep->amStreamAudioEndPointConfig);
        ep->amStreamAudioEndPointConfig = NULL;
    }

    if ((ep->amStreamAudioEndPointR != STREAM_TYPE_NO_ENDPOINT) &&
        (instData->currentStream->amStereo))
    { /* Stereo connection and right source side still allocated: de-allocate it */
        CsrAmIssueBccmdCloseSource(ep->amStreamAudioEndPointR);
        return;
    }
    else /* Source de-allocated (both left and right side). Find out what to do now  */
    {
        CsrAmPrim *amPrim = (CsrAmPrim *) instData->rxDownstreamMsgP;

        switch (*amPrim)
        {
            case CSR_AM_AUDIO_PATH_RELEASE_REQ:
            { /* At this point, we need to remove the sink(s) as well */
              /* Surely there is at least one sink? */
                instData->currentStream->amSinkList = (EndpointType *) CsrCmnListElementGetFirst(&instData->currentStream->sinkList);
                if (instData->currentStream->amSinkList != NULL)
                {
                    if (instData->currentStream->amSinkList->amStreamAudioEndPointL != STREAM_TYPE_NO_ENDPOINT)
                    {
                        CsrAmIssueBccmdCloseSink(instData->currentStream->amSinkList->amStreamAudioEndPointL);
                        instData->amCallback = CsrAmSinkDeallocCfm;
                    }
                }
                else
                {
                    CsrAmAudioPathReleaseCfmSend(instData, CSR_RESULT_SUCCESS, instData->currentStream->amAppStreamConnectionId);
                    CsrCmnListElementRemove(&instData->streamList, (CsrCmnListElm_t *) instData->currentStream);
                    CsrAmRestoreSavedMessages(instData);
                    CsrAmFreeOperationMessage(instData);
                }
                break;
            }

            case CSR_AM_AUDIO_PATH_CONNECT_REQ:
            { /* Now release sinks*/
                EndpointType *sinkToRemove = (EndpointType *) CsrCmnListElementGetFirst(&instData->currentStream->sinkList);
                if (sinkToRemove == NULL) /*The sink may have been allocated but not copied to the currentStream yet....*/
                {
                    sinkToRemove = (EndpointType *) CsrCmnListElementGetFirst(&instData->newSinkList);
                }
                if (sinkToRemove != NULL) /* Sink found */
                {
                    CsrAmIssueBccmdCloseSink(sinkToRemove->amStreamAudioEndPointL);
                    instData->amCallback = CsrAmSinkDeallocCfm;
                }
                else /* No sinks had been allocated yet*/
                {
                    CsrCmnListElementRemove(&instData->streamList, (CsrCmnListElm_t *) instData->currentStream);
                    CsrAmAudioPathConnectCfmSend(instData, CSR_RESULT_FAILURE, CSR_AM_NO_CONN_ID);
                    CsrAmFreeOperationMessage(instData);
                    /* Go to idle and restore message queue */
                    CsrAmRestoreSavedMessages(instData);
                }
                break;
            }
            case CSR_AM_AUDIO_PATH_REMOVE_SINK_REQ:
            {
                /* If the sources are removed when removing a sink, it means that the removed sink is the last one and
                   the stream is taken down. The last source endpoint is deallocated so we just need to either send the
                   confirm and the release indication or close the transaction and send those signals from there... */
                if ((instData->streamTransOpen) && (instData->streamTransSupported)) /* Close stream transaction */
                {
                    CsrAmIssueBccmdStreamTransComplete();
                    instData->amCallback = CsrAmStreamTransCompleteCfm;
                }
                else
                {
                    CsrAmAudioPathRemoveSinkCfmSend(instData, CSR_RESULT_SUCCESS, ((CsrAmAudioPathRemoveSinkReq *) amPrim)->amConnectionId);
                    CsrAmAudioPathReleaseIndSend(instData, ((CsrAmAudioPathRemoveSinkReq *) amPrim)->amConnectionId);
                    /* Remove stream from the list */
                    CsrCmnListElementRemove(&instData->streamList, (CsrCmnListElm_t *) instData->currentStream);
                    CsrAmRestoreSavedMessages(instData);
                    CsrAmFreeOperationMessage(instData);
                }
                break;
            }

            default:
                break;
        }
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmGetChipVersionCfm
 *
 *  DESCRIPTION
 *      Callback function after reading the chip version
 *----------------------------------------------------------------------------*/
void CsrAmGetChipVersionCfm(void *inst)
{
    AmInstanceDataType *instData = (AmInstanceDataType *) inst;
    CsrBccmdCfm *prim = instData->rxMsgP;
    if (prim->varId == BCCMDVARID_CHIP_VER) /* If this is not a BCCMDVARID_CHIP_VER the ResetInd will ensure a new request is send */
    {
        instData->chipVer = CsrAmGetBcValue(prim);
        CsrAmIssueGetBuildId();
        instData->amCallback = CsrAmGetBuildIdCfm;
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmGetBuildIdCfm
 *
 *  DESCRIPTION
 *      Callback function after reading the build Id
 *----------------------------------------------------------------------------*/
void CsrAmGetBuildIdCfm(void *inst)
{
    AmInstanceDataType *instData = (AmInstanceDataType *) inst;
    CsrBccmdCfm *prim = instData->rxMsgP;
    if (prim->varId == BCCMDVARID_BUILD_ID) /* If this is not a BCCMDVARID_BUILD_ID the ResetInd will ensure a new request is send */
    {
        instData->buildId = CsrAmGetBcValue(prim);
        instData->amCallback = NULL;
        /* Now ready to run; change state and restore queue */
        CsrAmRestoreSavedMessages(instData);
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmSinkDeallocCfm
 *
 *  DESCRIPTION
 *      Callback function after closing a sink endpoint
 *----------------------------------------------------------------------------*/
void CsrAmSinkDeallocCfm(void *inst)
{
    AmInstanceDataType *instData = (AmInstanceDataType *) inst;
    CsrUint16 sinkId = CsrAmGetBcValue((CsrBccmdCfm *) instData->rxMsgP);
    EndpointType *ep;
    CsrAmPrim *amPrim = (CsrAmPrim *) instData->rxDownstreamMsgP;
    CsrCmnList_t *list;

    if ((*amPrim == CSR_AM_AUDIO_PATH_ADD_SINK_REQ) ||
        (*amPrim == CSR_AM_AUDIO_PATH_CONNECT_REQ))
    { /* remove list of sinks to add */
        list = &instData->newSinkList;
    }
    else /* remove list of sinks in currentStream */
    {
        list = &instData->currentStream->sinkList;
    }

    ep = CsrAmFindEndPointBySinkId(list, sinkId);
    if ((ep == NULL) && (*amPrim == CSR_AM_AUDIO_PATH_CONNECT_REQ)) /* The sink could be in the stream list already... */
    {
        list = &instData->currentStream->sinkList;
        ep = CsrAmFindEndPointBySinkId(list, sinkId);
    }

    if (ep == NULL)
    {
        CSR_LOG_TEXT_ERROR((CsrAmLto, 0, "Sink-Id not found deallocating sink"));
    }
    else
    {
        if (ep->amStreamAudioEndPointL == sinkId)
        {
            ep->amStreamAudioEndPointL = STREAM_TYPE_NO_ENDPOINT;
        }
        else if (ep->amStreamAudioEndPointR == sinkId)
        {
            ep->amStreamAudioEndPointR = STREAM_TYPE_NO_ENDPOINT;
        }
        if (ep->amStreamAudioEndPointConfigLen)
        {
            ep->amStreamAudioEndPointConfigLen = 0;
            CsrPmemFree(ep->amStreamAudioEndPointConfig);
            ep->amStreamAudioEndPointConfig = NULL;
        }
        if (ep->amStreamAudioEndPointR != STREAM_TYPE_NO_ENDPOINT)
        {
            CsrAmIssueBccmdCloseSink(ep->amStreamAudioEndPointR);
            instData->amCallback = CsrAmSinkDeallocCfm;
            return;
        }
        /* Are there any more sinks to de-allocate? */
        if ((*amPrim == CSR_AM_AUDIO_PATH_ADD_SINK_REQ) ||
            (*amPrim == CSR_AM_AUDIO_PATH_CONNECT_REQ) ||
            (*amPrim == CSR_AM_AUDIO_PATH_RELEASE_REQ))
        {
            CsrCmnListElementRemove(list, (CsrCmnListElm_t *) ep);
            ep = (EndpointType *) CsrCmnListElementGetFirst(list);
        }
        else /* CSR_AM_AUDIO_PATH_REMOVE_SINK_REQ: Only possible to remove
            one sink from the stream at a time so far... */
        {
            CsrCmnListElementRemove(list, (CsrCmnListElm_t *) ep);
            ep = NULL;
        }

        if (ep != NULL)
        {
            CsrAmIssueBccmdCloseSink(ep->amStreamAudioEndPointL);
            instData->amCallback = CsrAmSinkDeallocCfm;
        }
        else /* Done here: find out what to do next */
        {
            if ((instData->streamTransOpen) && (instData->streamTransSupported)) /* Close stream transaction. When connecting or adding sink, the
                transaction will not be open.... */
            {
                CsrAmIssueBccmdStreamTransComplete();
                instData->amCallback = CsrAmStreamTransCompleteCfm;
            }
            else
            {
                switch (*amPrim)
                {
                    case CSR_AM_AUDIO_PATH_RELEASE_REQ:
                    {
                        /* Are there any more sinks left to de-allocate? */
                        instData->currentStream->amSinkList = (EndpointType *) CsrCmnListElementGetFirst(&instData->currentStream->sinkList);
                        if (instData->currentStream->amSinkList != NULL)
                        {
                            CsrAmIssueBccmdCloseSink(instData->currentStream->amSinkList->amStreamAudioEndPointL);
                            instData->amCallback = CsrAmSinkDeallocCfm;
                        }
                        else /* DONE */
                        {
                            CsrAmAudioPathReleaseCfmSend(instData, CSR_RESULT_SUCCESS, instData->currentStream->amAppStreamConnectionId);
                            CsrCmnListElementRemove(&instData->streamList, (CsrCmnListElm_t *) instData->currentStream);
                            CsrAmRestoreSavedMessages(instData);
                            CsrAmFreeOperationMessage(instData);
                        }
                        break;
                    }

                    case CSR_AM_AUDIO_PATH_REMOVE_SINK_REQ:
                    {
                        StreamType *str = instData->currentStream;

                        CsrAmFindAndRemoveSinkFromSinkList(str, ((CsrAmAudioPathRemoveSinkReq *) amPrim)->sinkId);
                        str->amSinkList = (EndpointType *) CsrCmnListElementGetFirst(&str->sinkList);
                        if (str->amSinkList == NULL) /* This was the last sink. De allocate the source(s) and send the release indication */
                        {
                            CsrAmIssueBccmdCloseSource(str->amSource.amStreamAudioEndPointL);
                            instData->amCallback = CsrAmSrcDeallocCfm;
                        }
                        else if (instData->streamTransSupported) /* Re-connect: start transaction */
                        {
                            CsrAmIssueBccmdStreamTransStart();
                            instData->amCallback = CsrAmStreamTransStartCfm;
                        }
                        else
                        {
                            if (instData->currentStream->amStreamTransformIdL != STREAM_ID_NONE) /* Now go on to release the stream */
                            {
                                CsrAmIssueBccmdStreamDisconnect(instData->currentStream->amStreamTransformIdL);
                                instData->amCallback = CsrAmStreamDisconnectCfm;
                            }
                            else /* Transaction open while removing sink and no connection exists: this must be a re-connection. */
                            {
                                ep = (EndpointType *) CsrCmnListElementGetFirst(&instData->currentStream->sinkList);
                                CsrAmIssueBccmdStreamConnect(instData->currentStream->amSource.amStreamAudioEndPointL, ep->amStreamAudioEndPointL);
                                instData->amCallback = CsrAmStreamConnectCfm;
                            }
                        }
                        break;
                    }
                    case CSR_AM_AUDIO_PATH_ADD_SINK_REQ:
                    { /* Finished: bail out! */
                        CsrAmDestroyLinkOfSinksToAdd(instData);
                        CsrAmAudioPathAddSinkCfmSend(instData, CSR_RESULT_FAILURE, ((CsrAmAudioPathAddSinkReq *) amPrim)->amConnectionId);
                        CsrAmFreeOperationMessage(instData);
                        /* Go to idle and restore message queue */
                        CsrAmRestoreSavedMessages(instData);
                        break;
                    }
                    case CSR_AM_AUDIO_PATH_CONNECT_REQ:
                    { /* Sources and sinks de-allocated: bail out */
                        CsrAmDestroyLinkOfSinksToAdd(instData);
                        CsrCmnListElementRemove(&instData->streamList, (CsrCmnListElm_t *) instData->currentStream);
                        CsrAmAudioPathConnectCfmSend(instData, CSR_RESULT_FAILURE, ((CsrAmAudioPathAddSinkReq *) amPrim)->amConnectionId);
                        CsrAmFreeOperationMessage(instData);
                        /* Go to idle and restore message queue */
                        CsrAmRestoreSavedMessages(instData);
                        break;
                    }
                    default:
                        break;
                }
            }
        }
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmStreamDisconnectCfm
 *
 *  DESCRIPTION
 *      Callback function after stream disconnect bccmd
 *----------------------------------------------------------------------------*/
void CsrAmStreamDisconnectCfm(void *inst)
{
    AmInstanceDataType *instData = (AmInstanceDataType *) inst;
    CsrUint16 streamId = CsrAmGetBcValue((CsrBccmdCfm *) instData->rxMsgP);

    if (instData->currentStream->amStreamTransformIdL == streamId)
    {
        instData->currentStream->amStreamTransformIdL = STREAM_ID_NONE;
    }
    else if (instData->currentStream->amStreamTransformIdR == streamId)
    {
        instData->currentStream->amStreamTransformIdR = STREAM_ID_NONE;
    }

    if ((instData->currentStream->amStreamTransformIdR != STREAM_ID_NONE) &&
        (instData->currentStream->amStereo))
    { /* Stereo connection and right side still connected: disconnect */
        CsrAmIssueBccmdStreamDisconnect(instData->currentStream->amStreamTransformIdR);
        return;
    }
    else /* Stream released; check what to do next */
    {
        CsrAmPrim *amPrim = (CsrAmPrim *) instData->rxDownstreamMsgP;
        switch (*amPrim)
        {
            case CSR_AM_AUDIO_PATH_CONFIG_REQ:
            {
                if ((instData->streamTransSupported) && (instData->streamTransOpen)) /* Need to connect */
                {
                    CsrAmIssueBccmdStreamTransComplete();
                    instData->amCallback = CsrAmStreamTransCompleteCfm;
                }
                else
                {
                    CsrUint16 sid;
                    EndpointType *ep = CsrAmGetEndpointToConfig(instData->currentStream, &sid, ((CsrAmAudioPathConfigReq *) amPrim)->epType, FALSE);
                    if (ep != NULL)
                    {
                        AmEpConfigType *configData = &ep->amStreamAudioEndPointConfig[ep->amStreamAudioEndPointConfigLen - 1];
                        CsrAmIssueBccmdConfigStream(sid, configData->key, configData->value);
                        instData->amCallback = CsrAmStreamConfigCfm;
                    }
                    else /* All configured. Now Alias, re-connect, etc... if needed */
                    {
                        StreamType *str = instData->currentStream;

                        str->amSinkList = (EndpointType *) CsrCmnListElementGetFirst(&str->sinkList);
                        CsrAmIssueBccmdStreamConnect(str->amSource.amStreamAudioEndPointL, str->amSinkList->amStreamAudioEndPointL);
                        instData->amCallback = CsrAmStreamConnectCfm;
                    }
                }
                break;
            }
            case CSR_AM_AUDIO_PATH_REMOVE_SINK_REQ:
            case CSR_AM_AUDIO_PATH_RELEASE_REQ:
            {
                /* Connection released and we are releasing: close transaction, de-allocate the source(s) and sink(s)
                   and remove the entry from the list of entries before sending confirm to the application */
                if (instData->streamTransSupported)
                {
                    CsrAmIssueBccmdStreamTransComplete();
                    instData->amCallback = CsrAmStreamTransCompleteCfm;
                }
                else if (*amPrim == CSR_AM_AUDIO_PATH_RELEASE_REQ)
                {
                    CsrAmIssueBccmdCloseSource(instData->currentStream->amSource.amStreamAudioEndPointL);
                    instData->amCallback = CsrAmSrcDeallocCfm;
                }
                else /* REMOVE SINK */
                {
                    EndpointType *sinkToRemove;

                    instData->currentStream->amSinkList = (EndpointType *) CsrCmnListElementGetFirst(&instData->currentStream->sinkList);
                    sinkToRemove = CsrAmFindEndPointBySinkType(instData->currentStream->amSinkList,
                        ((CsrAmAudioPathRemoveSinkReq *) amPrim)->sinkId);
                    if (sinkToRemove != NULL) /* Removing sink(s) */
                    {
                        CsrAmIssueBccmdCloseSink(sinkToRemove->amStreamAudioEndPointL);
                        instData->amCallback = CsrAmSinkDeallocCfm;
                    }
                    else /* Sink(s) removed. Now find out what to do; if re-connect needed, the transaction needs to be opened again */
                    {
                        sinkToRemove = (EndpointType *) CsrCmnListElementGetFirst(&instData->currentStream->sinkList);
                        if ((instData->currentStream->amStreamTransformIdL == STREAM_ID_NONE) && (sinkToRemove != NULL) && (instData->streamTransSupported)) /* Need to connect */
                        {
                            CsrAmIssueBccmdStreamTransStart();
                            instData->amCallback = CsrAmStreamTransStartCfm;
                        }
                        else /* The sink(s) removed is the last one and the sources have been removed as well... this is infact a disconnection */
                        {
                            EndpointType *ep = &(instData->currentStream->amSource);
                            CsrAmAudioPathRemoveSinkCfmSend(instData, CSR_RESULT_SUCCESS, ((CsrAmAudioPathRemoveSinkReq *) amPrim)->amConnectionId);

                            if ((ep->amStreamAudioEndPointL == STREAM_TYPE_NO_ENDPOINT) &&
                                ((ep->amStreamAudioEndPointR == STREAM_TYPE_NO_ENDPOINT) || (!instData->currentStream->amStereo)))
                            { /* Source(s) removed as well */
                                CsrAmAudioPathReleaseIndSend(instData, ((CsrAmAudioPathRemoveSinkReq *) amPrim)->amConnectionId);
                                /* Remove stream from the list */
                                CsrCmnListElementRemove(&instData->streamList, (CsrCmnListElm_t *) instData->currentStream);
                            }
                            /* Restore queue and go back to idle */
                            CsrAmRestoreSavedMessages(instData);
                            CsrAmFreeOperationMessage(instData);
                        }
                    }
                }
                break;
            }

            case CSR_AM_AUDIO_PATH_ADD_SINK_REQ:
            {
                /* Stream disconnected. This means: all new sinks allocated, add to the list, sync'ed if needed, aliased and
                   transform open. So now we need to connect the source ad first sink and then complete the transform. */
                StreamType *str = instData->currentStream;
                /* Re-connect */
                CsrAmIssueBccmdStreamConnect(str->amSource.amStreamAudioEndPointL, str->amSinkList->amStreamAudioEndPointL);
                instData->amCallback = CsrAmStreamConnectCfm;
                break;
            }

            default:
                break;
        }
    }
}
