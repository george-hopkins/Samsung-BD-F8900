/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/
#include "csr_am_lib.h"
#include "csr_pmem.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmInitReqSend
 *
 *  DESCRIPTION
 *      This function prepares the AM for use from an application
 *
 *
 *  PARAMETERS
 *      pHandle       : The identity of the calling process.
 *----------------------------------------------------------------------------*/
CsrAmInitReq *CsrAmInitReq_struct(CsrSchedQid pHandle)
{
    CsrAmInitReq *prim = (CsrAmInitReq *) CsrPmemAlloc(sizeof(CsrAmInitReq));
    prim->type = CSR_AM_INIT_REQ;
    prim->pHandle = pHandle;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmAudioPathConnectReqSend
 *
 *  DESCRIPTION
 *      This function establishes an audio stream.
 *
 *
 *  PARAMETERS
 *      pHandle       : The identity of the calling process.
 *      source        : Type of source endpoint to use (fm, codec,pcm,...).
 *      sourceR       : Type of source endpoint to use for the right side of a stereo connection
 *      sinkCount     : Number of sink endpoint elements in the "sinkId" field
 *      sinkId        : Type of sink to use (fm, codec,pcm,...).
 *      sinkConfigCount : number of configuration entries
 *      sinkConfig    : Set of configruation values for the sinks which configuration is intended
 *      audioType     : FM, WBS, CVSD,.....
 *      stereo        : boolean to indicate whether stereo audio is wanted.
 *----------------------------------------------------------------------------*/
CsrAmAudioPathConnectReq *CsrAmAudioPathConnectReq_struct(CsrSchedQid pHandle,
    CsrEndpointType source,
    CsrEndpointType sourceR,
    CsrUint8 sinkCount,
    CsrAmEndPointDefType *sinkId,
    CsrUint8 sinkConfigCount,
    CsrAmSinkConfigType *sinkConfig,
    CsrAmAudioType audioType,
    CsrBool stereo)
{
    CsrAmAudioPathConnectReq *prim = (CsrAmAudioPathConnectReq *) CsrPmemAlloc(sizeof(CsrAmAudioPathConnectReq));
    prim->type = CSR_AM_AUDIO_PATH_CONNECT_REQ;
    prim->pHandle = pHandle;
    prim->source = source;
    prim->sourceR = sourceR;
    prim->sinks.sinkCount = sinkCount;
    prim->sinks.newSinkId = sinkId;
    prim->sinks.sinkConfigCount = sinkConfigCount;
    prim->sinks.sinkConfig = sinkConfig;
    prim->audioType = audioType;
    prim->stereo = stereo;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmAudioPathAddSinkReqSend
 *
 *  DESCRIPTION
 *      This function adds a sink endpoint to an established audio path
 *
 *
 *  PARAMETERS
 *      pHandle         : The identity of the calling process.
 *      amConnectionId  : connection Id.
 *      sinkCount       : number of sinks to add
 *      newSinkId       : the sink to add.
 *      sinkConfigCount : number of configuration entries
 *      sinkConfig      : Set of configruation values for the sinks which
 *                        configuration is intended
 *----------------------------------------------------------------------------*/
CsrAmAudioPathAddSinkReq *CsrAmAudioPathAddSinkReq_struct(CsrSchedQid pHandle,
    CsrUint16 amConnectionId,
    CsrUint8 sinkCount,
    CsrAmEndPointDefType *newSinkId,
    CsrUint8 sinkConfigCount,
    CsrAmSinkConfigType *sinkConfig)
{
    CsrAmAudioPathAddSinkReq *prim = (CsrAmAudioPathAddSinkReq *) CsrPmemAlloc(sizeof(CsrAmAudioPathAddSinkReq));
    prim->type = CSR_AM_AUDIO_PATH_ADD_SINK_REQ;
    prim->pHandle = pHandle;
    prim->amConnectionId = amConnectionId;
    prim->sinks.sinkCount = sinkCount;
    prim->sinks.newSinkId = newSinkId;
    prim->sinks.sinkConfigCount = sinkConfigCount;
    prim->sinks.sinkConfig = sinkConfig;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmAudioPathRemoveSinkReqSend
 *
 *  DESCRIPTION
 *      This function removes a sink endpoint from an established audio path
 *
 *
 *  PARAMETERS
 *      pHandle         : The identity of the calling process.
 *      amConnectionId  : connection Id.
 *      sinkId          : the sink to remove.
 *----------------------------------------------------------------------------*/
CsrAmAudioPathRemoveSinkReq *CsrAmAudioPathRemoveSinkReq_struct(CsrSchedQid pHandle,
    CsrUint16 amConnectionId,
    CsrAmEndPointDefType sinkId)
{
    CsrAmAudioPathRemoveSinkReq *prim = (CsrAmAudioPathRemoveSinkReq *) CsrPmemAlloc(sizeof(CsrAmAudioPathRemoveSinkReq));
    prim->type = CSR_AM_AUDIO_PATH_REMOVE_SINK_REQ;
    prim->pHandle = pHandle;
    prim->amConnectionId = amConnectionId;
    prim->sinkId = sinkId;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmAudioPathConfigReqSend
 *
 *  DESCRIPTION
 *      This function configures an established audio path
 *
 *
 *  PARAMETERS
 *      pHandle         : The identity of the calling process.
 *      amConnectionId  : connection Id.
 *      configDataLen   : length of the config data
 *      configData      : pointer to the configuration data
 *----------------------------------------------------------------------------*/
CsrAmAudioPathConfigReq *CsrAmAudioPathConfigReq_struct(CsrSchedQid pHandle,
    CsrUint16 amConnectionId,
    CsrAmEndpointType epType,
    CsrUint16 sinkId,
    CsrUint16 configDataLen,
    CsrUint8 *configData)
{
    CsrAmAudioPathConfigReq *prim = (CsrAmAudioPathConfigReq *) CsrPmemAlloc(sizeof(CsrAmAudioPathConfigReq));
    prim->type = CSR_AM_AUDIO_PATH_CONFIG_REQ;
    prim->pHandle = pHandle;
    prim->amConnectionId = amConnectionId;
    prim->epType = epType;
    prim->sinkId = sinkId;
    prim->configDataLen = configDataLen;
    prim->configData = configData;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrAmAudioPathReleaseReqSend
 *
 *  DESCRIPTION
 *      This function releases an established audio path
 *
 *
 *  PARAMETERS
 *      pHandle         : The identity of the calling process.
 *      amConnectionId  : connection Id.
 *----------------------------------------------------------------------------*/
CsrAmAudioPathReleaseReq *CsrAmAudioPathReleaseReq_struct(CsrSchedQid pHandle,
    CsrUint16 amConnectionId)
{
    CsrAmAudioPathReleaseReq *prim = (CsrAmAudioPathReleaseReq *) CsrPmemAlloc(sizeof(CsrAmAudioPathReleaseReq));
    prim->type = CSR_AM_AUDIO_PATH_RELEASE_REQ;
    prim->pHandle = pHandle;
    prim->amConnectionId = amConnectionId;
    return prim;
}
