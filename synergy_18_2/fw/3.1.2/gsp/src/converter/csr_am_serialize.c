/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_msgconv.h"
#include "csr_autogen.h"
#include "csr_unicode.h"
#include "csr_am_prim.h"

#ifndef EXCLUDE_CSR_AM_MODULE

void CsrAmPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_CONFIG_CFM
CsrSize CsrAmAudioPathConfigCfmSizeof(void *msg)
{
    CsrAmAudioPathConfigCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_CONFIG_CFM */

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_CONFIG_CFM
CsrUint8 *CsrAmAudioPathConfigCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrAmAudioPathConfigCfm *primitive;

    primitive = (CsrAmAudioPathConfigCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->amConnectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_CONFIG_CFM */

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_CONFIG_CFM
void *CsrAmAudioPathConfigCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrAmAudioPathConfigCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrAmAudioPathConfigCfm *) CsrPmemZalloc(sizeof(CsrAmAudioPathConfigCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->amConnectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_CONFIG_CFM */

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_CONNECT_CFM
CsrSize CsrAmAudioPathConnectCfmSizeof(void *msg)
{
    CsrAmAudioPathConnectCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_CONNECT_CFM */

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_CONNECT_CFM
CsrUint8 *CsrAmAudioPathConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrAmAudioPathConnectCfm *primitive;

    primitive = (CsrAmAudioPathConnectCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->amConnectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_CONNECT_CFM */

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_CONNECT_CFM
void *CsrAmAudioPathConnectCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrAmAudioPathConnectCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrAmAudioPathConnectCfm *) CsrPmemZalloc(sizeof(CsrAmAudioPathConnectCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->amConnectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_CONNECT_CFM */

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_REMOVE_SINK_CFM
CsrSize CsrAmAudioPathRemoveSinkCfmSizeof(void *msg)
{
    CsrAmAudioPathRemoveSinkCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_REMOVE_SINK_CFM */

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_REMOVE_SINK_CFM
CsrUint8 *CsrAmAudioPathRemoveSinkCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrAmAudioPathRemoveSinkCfm *primitive;

    primitive = (CsrAmAudioPathRemoveSinkCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->amConnectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_REMOVE_SINK_CFM */

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_REMOVE_SINK_CFM
void *CsrAmAudioPathRemoveSinkCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrAmAudioPathRemoveSinkCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrAmAudioPathRemoveSinkCfm *) CsrPmemZalloc(sizeof(CsrAmAudioPathRemoveSinkCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->amConnectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_REMOVE_SINK_CFM */

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_ADD_SINK_CFM
CsrSize CsrAmAudioPathAddSinkCfmSizeof(void *msg)
{
    CsrAmAudioPathAddSinkCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_ADD_SINK_CFM */

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_ADD_SINK_CFM
CsrUint8 *CsrAmAudioPathAddSinkCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrAmAudioPathAddSinkCfm *primitive;

    primitive = (CsrAmAudioPathAddSinkCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->amConnectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_ADD_SINK_CFM */

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_ADD_SINK_CFM
void *CsrAmAudioPathAddSinkCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrAmAudioPathAddSinkCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrAmAudioPathAddSinkCfm *) CsrPmemZalloc(sizeof(CsrAmAudioPathAddSinkCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->amConnectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_ADD_SINK_CFM */

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_CONFIG_REQ
CsrSize CsrAmAudioPathConfigReqSizeof(void *msg)
{
    CsrAmAudioPathConfigReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->configDataLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_CONFIG_REQ */

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_CONFIG_REQ
CsrUint8 *CsrAmAudioPathConfigReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrAmAudioPathConfigReq *primitive;

    primitive = (CsrAmAudioPathConfigReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->amConnectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->epType);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->sinkId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->configDataLen);
    CsrMemCpySer(buffer, length, &primitive->configData, sizeof(primitive->configData));
    if(primitive->configData)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->configData), ((CsrSize) (primitive->configDataLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_CONFIG_REQ */

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_CONFIG_REQ
void *CsrAmAudioPathConfigReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrAmAudioPathConfigReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrAmAudioPathConfigReq *) CsrPmemZalloc(sizeof(CsrAmAudioPathConfigReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->amConnectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->epType, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->sinkId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->configDataLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->configData), buffer, &offset, ((CsrSize) sizeof(primitive->configData)));
    if (primitive->configDataLen)
    {
        primitive->configData = CsrPmemZalloc(((CsrUint32) (primitive->configDataLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->configData), buffer, &offset, ((CsrSize) (primitive->configDataLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->configData = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_CONFIG_REQ */

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_CONFIG_REQ
void CsrAmAudioPathConfigReqSerFree(void *msg)
{
    CsrAmAudioPathConfigReq *primitive;

    primitive = (CsrAmAudioPathConfigReq *) msg;

    if(primitive->configData != NULL)
    {
        CsrPmemFree(primitive->configData);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_CONFIG_REQ */

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_RELEASE_REQ
CsrSize CsrAmAudioPathReleaseReqSizeof(void *msg)
{
    CsrAmAudioPathReleaseReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_RELEASE_REQ */

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_RELEASE_REQ
CsrUint8 *CsrAmAudioPathReleaseReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrAmAudioPathReleaseReq *primitive;

    primitive = (CsrAmAudioPathReleaseReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->amConnectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_RELEASE_REQ */

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_RELEASE_REQ
void *CsrAmAudioPathReleaseReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrAmAudioPathReleaseReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrAmAudioPathReleaseReq *) CsrPmemZalloc(sizeof(CsrAmAudioPathReleaseReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->amConnectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_RELEASE_REQ */

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_RELEASE_CFM
CsrSize CsrAmAudioPathReleaseCfmSizeof(void *msg)
{
    CsrAmAudioPathReleaseCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_RELEASE_CFM */

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_RELEASE_CFM
CsrUint8 *CsrAmAudioPathReleaseCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrAmAudioPathReleaseCfm *primitive;

    primitive = (CsrAmAudioPathReleaseCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->amConnectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_RELEASE_CFM */

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_RELEASE_CFM
void *CsrAmAudioPathReleaseCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrAmAudioPathReleaseCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrAmAudioPathReleaseCfm *) CsrPmemZalloc(sizeof(CsrAmAudioPathReleaseCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->amConnectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_RELEASE_CFM */

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_REMOVE_SINK_REQ
CsrSize CsrAmAudioPathRemoveSinkReqSizeof(void *msg)
{
    CsrAmAudioPathRemoveSinkReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_REMOVE_SINK_REQ */

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_REMOVE_SINK_REQ
CsrUint8 *CsrAmAudioPathRemoveSinkReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrAmAudioPathRemoveSinkReq *primitive;

    primitive = (CsrAmAudioPathRemoveSinkReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->amConnectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->sinkId.endPoint);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->sinkId.instance);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->sinkId.channel);

    return buffer;
}
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_REMOVE_SINK_REQ */

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_REMOVE_SINK_REQ
void *CsrAmAudioPathRemoveSinkReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrAmAudioPathRemoveSinkReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrAmAudioPathRemoveSinkReq *) CsrPmemZalloc(sizeof(CsrAmAudioPathRemoveSinkReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->amConnectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->sinkId.endPoint, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->sinkId.instance, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->sinkId.channel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_REMOVE_SINK_REQ */

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_CONNECT_REQ
CsrSize CsrAmAudioPathConnectReqSizeof(void *msg)
{
    CsrAmAudioPathConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->source.configDataCount * sizeof(CsrUint32)) + (primitive->sourceR.configDataCount * sizeof(CsrUint32)) + (primitive->sinks.newSinkId ? sizeof(*primitive->sinks.newSinkId) : 0) + (primitive->sinks.sinkConfigCount * sizeof(CsrAmSinkConfigType)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_CONNECT_REQ */

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_CONNECT_REQ
CsrUint8 *CsrAmAudioPathConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrAmAudioPathConnectReq *primitive;

    primitive = (CsrAmAudioPathConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->source.endPoint.endPoint);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->source.endPoint.instance);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->source.endPoint.channel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->source.configDataCount);
    CsrMemCpySer(buffer, length, &primitive->source.configData, sizeof(primitive->source.configData));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->sourceR.endPoint.endPoint);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->sourceR.endPoint.instance);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->sourceR.endPoint.channel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->sourceR.configDataCount);
    CsrMemCpySer(buffer, length, &primitive->sourceR.configData, sizeof(primitive->sourceR.configData));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->sinks.sinkCount);
    CsrMemCpySer(buffer, length, &primitive->sinks.newSinkId, sizeof(primitive->sinks.newSinkId));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->sinks.sinkConfigCount);
    CsrMemCpySer(buffer, length, &primitive->sinks.sinkConfig, sizeof(primitive->sinks.sinkConfig));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->audioType);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->stereo);
    if(primitive->source.configData)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->source.configData), ((CsrSize) (primitive->source.configDataCount * sizeof(CsrUint32))));
    }
    if(primitive->sourceR.configData)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->sourceR.configData), ((CsrSize) (primitive->sourceR.configDataCount * sizeof(CsrUint32))));
    }
    if(primitive->sinks.newSinkId)
    {
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->sinks.newSinkId->endPoint);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->sinks.newSinkId->instance);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->sinks.newSinkId->channel);
    }
    if(primitive->sinks.sinkConfig)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->sinks.sinkConfig), ((CsrSize) (primitive->sinks.sinkConfigCount * sizeof(CsrAmSinkConfigType))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_CONNECT_REQ */

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_CONNECT_REQ
void *CsrAmAudioPathConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrAmAudioPathConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrAmAudioPathConnectReq *) CsrPmemZalloc(sizeof(CsrAmAudioPathConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->source.endPoint.endPoint, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->source.endPoint.instance, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->source.endPoint.channel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->source.configDataCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->source.configData), buffer, &offset, ((CsrSize) sizeof(primitive->source.configData)));
    CsrUint16Des((CsrUint16 *) &primitive->sourceR.endPoint.endPoint, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->sourceR.endPoint.instance, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->sourceR.endPoint.channel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->sourceR.configDataCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->sourceR.configData), buffer, &offset, ((CsrSize) sizeof(primitive->sourceR.configData)));
    CsrUint8Des((CsrUint8 *) &primitive->sinks.sinkCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->sinks.newSinkId), buffer, &offset, ((CsrSize) sizeof(primitive->sinks.newSinkId)));
    CsrUint8Des((CsrUint8 *) &primitive->sinks.sinkConfigCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->sinks.sinkConfig), buffer, &offset, ((CsrSize) sizeof(primitive->sinks.sinkConfig)));
    CsrUint16Des((CsrUint16 *) &primitive->audioType, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->stereo, buffer, &offset);
    if (primitive->source.configDataCount)
    {
        primitive->source.configData = CsrPmemZalloc(((CsrUint32) (primitive->source.configDataCount * sizeof(CsrUint32))));
        CsrMemCpyDes(((void *) primitive->source.configData), buffer, &offset, ((CsrSize) (primitive->source.configDataCount * sizeof(CsrUint32))));
    }
    else
    {
        primitive->source.configData = NULL;
    }
    if (primitive->sourceR.configDataCount)
    {
        primitive->sourceR.configData = CsrPmemZalloc(((CsrUint32) (primitive->sourceR.configDataCount * sizeof(CsrUint32))));
        CsrMemCpyDes(((void *) primitive->sourceR.configData), buffer, &offset, ((CsrSize) (primitive->sourceR.configDataCount * sizeof(CsrUint32))));
    }
    else
    {
        primitive->sourceR.configData = NULL;
    }
    if (primitive->sinks.newSinkId)
    {
        primitive->sinks.newSinkId = CsrPmemZalloc((CsrUint32) (sizeof(*primitive->sinks.newSinkId)));
    CsrUint16Des((CsrUint16 *) &primitive->sinks.newSinkId->endPoint, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->sinks.newSinkId->instance, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->sinks.newSinkId->channel, buffer, &offset);
    }
    if (primitive->sinks.sinkConfigCount)
    {
        primitive->sinks.sinkConfig = CsrPmemZalloc(((CsrUint32) (primitive->sinks.sinkConfigCount * sizeof(CsrAmSinkConfigType))));
        CsrMemCpyDes(((void *) primitive->sinks.sinkConfig), buffer, &offset, ((CsrSize) (primitive->sinks.sinkConfigCount * sizeof(CsrAmSinkConfigType))));
    }
    else
    {
        primitive->sinks.sinkConfig = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_CONNECT_REQ */

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_CONNECT_REQ
void CsrAmAudioPathConnectReqSerFree(void *msg)
{
    CsrAmAudioPathConnectReq *primitive;

    primitive = (CsrAmAudioPathConnectReq *) msg;

    if(primitive->source.configData != NULL)
    {
        CsrPmemFree(primitive->source.configData);
    }
    if(primitive->sourceR.configData != NULL)
    {
        CsrPmemFree(primitive->sourceR.configData);
    }
    if(primitive->sinks.newSinkId != NULL)
    {
        CsrPmemFree(primitive->sinks.newSinkId);
    }
    if(primitive->sinks.sinkConfig != NULL)
    {
        CsrPmemFree(primitive->sinks.sinkConfig);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_CONNECT_REQ */

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_ADD_SINK_REQ
CsrSize CsrAmAudioPathAddSinkReqSizeof(void *msg)
{
    CsrAmAudioPathAddSinkReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->sinks.newSinkId ? sizeof(*primitive->sinks.newSinkId) : 0) + (primitive->sinks.sinkConfigCount * sizeof(CsrAmSinkConfigType)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_ADD_SINK_REQ */

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_ADD_SINK_REQ
CsrUint8 *CsrAmAudioPathAddSinkReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrAmAudioPathAddSinkReq *primitive;

    primitive = (CsrAmAudioPathAddSinkReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->amConnectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->sinks.sinkCount);
    CsrMemCpySer(buffer, length, &primitive->sinks.newSinkId, sizeof(primitive->sinks.newSinkId));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->sinks.sinkConfigCount);
    CsrMemCpySer(buffer, length, &primitive->sinks.sinkConfig, sizeof(primitive->sinks.sinkConfig));
    if(primitive->sinks.newSinkId)
    {
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->sinks.newSinkId->endPoint);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->sinks.newSinkId->instance);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->sinks.newSinkId->channel);
    }
    if(primitive->sinks.sinkConfig)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->sinks.sinkConfig), ((CsrSize) (primitive->sinks.sinkConfigCount * sizeof(CsrAmSinkConfigType))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_ADD_SINK_REQ */

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_ADD_SINK_REQ
void *CsrAmAudioPathAddSinkReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrAmAudioPathAddSinkReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrAmAudioPathAddSinkReq *) CsrPmemZalloc(sizeof(CsrAmAudioPathAddSinkReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->amConnectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->sinks.sinkCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->sinks.newSinkId), buffer, &offset, ((CsrSize) sizeof(primitive->sinks.newSinkId)));
    CsrUint8Des((CsrUint8 *) &primitive->sinks.sinkConfigCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->sinks.sinkConfig), buffer, &offset, ((CsrSize) sizeof(primitive->sinks.sinkConfig)));
    if (primitive->sinks.newSinkId)
    {
        primitive->sinks.newSinkId = CsrPmemZalloc((CsrUint32) (sizeof(*primitive->sinks.newSinkId)));
    CsrUint16Des((CsrUint16 *) &primitive->sinks.newSinkId->endPoint, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->sinks.newSinkId->instance, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->sinks.newSinkId->channel, buffer, &offset);
    }
    if (primitive->sinks.sinkConfigCount)
    {
        primitive->sinks.sinkConfig = CsrPmemZalloc(((CsrUint32) (primitive->sinks.sinkConfigCount * sizeof(CsrAmSinkConfigType))));
        CsrMemCpyDes(((void *) primitive->sinks.sinkConfig), buffer, &offset, ((CsrSize) (primitive->sinks.sinkConfigCount * sizeof(CsrAmSinkConfigType))));
    }
    else
    {
        primitive->sinks.sinkConfig = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_ADD_SINK_REQ */

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_ADD_SINK_REQ
void CsrAmAudioPathAddSinkReqSerFree(void *msg)
{
    CsrAmAudioPathAddSinkReq *primitive;

    primitive = (CsrAmAudioPathAddSinkReq *) msg;

    if(primitive->sinks.newSinkId != NULL)
    {
        CsrPmemFree(primitive->sinks.newSinkId);
    }
    if(primitive->sinks.sinkConfig != NULL)
    {
        CsrPmemFree(primitive->sinks.sinkConfig);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_ADD_SINK_REQ */

#ifndef EXCLUDE_CSR_AM_INIT_REQ
CsrSize CsrAmInitReqSizeof(void *msg)
{
    CsrAmInitReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_AM_INIT_REQ */

#ifndef EXCLUDE_CSR_AM_INIT_REQ
CsrUint8 *CsrAmInitReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrAmInitReq *primitive;

    primitive = (CsrAmInitReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_AM_INIT_REQ */

#ifndef EXCLUDE_CSR_AM_INIT_REQ
void *CsrAmInitReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrAmInitReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrAmInitReq *) CsrPmemZalloc(sizeof(CsrAmInitReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_AM_INIT_REQ */

#ifndef EXCLUDE_CSR_AM_INIT_CFM
CsrSize CsrAmInitCfmSizeof(void *msg)
{
    CsrAmInitCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_AM_INIT_CFM */

#ifndef EXCLUDE_CSR_AM_INIT_CFM
CsrUint8 *CsrAmInitCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrAmInitCfm *primitive;

    primitive = (CsrAmInitCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->buildId);

    return buffer;
}
#endif /* EXCLUDE_CSR_AM_INIT_CFM */

#ifndef EXCLUDE_CSR_AM_INIT_CFM
void *CsrAmInitCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrAmInitCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrAmInitCfm *) CsrPmemZalloc(sizeof(CsrAmInitCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->buildId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_AM_INIT_CFM */

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_RELEASE_IND
CsrSize CsrAmAudioPathReleaseIndSizeof(void *msg)
{
    CsrAmAudioPathReleaseInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_RELEASE_IND */

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_RELEASE_IND
CsrUint8 *CsrAmAudioPathReleaseIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrAmAudioPathReleaseInd *primitive;

    primitive = (CsrAmAudioPathReleaseInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->amConnectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_RELEASE_IND */

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_RELEASE_IND
void *CsrAmAudioPathReleaseIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrAmAudioPathReleaseInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrAmAudioPathReleaseInd *) CsrPmemZalloc(sizeof(CsrAmAudioPathReleaseInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->amConnectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_RELEASE_IND */

#endif /* EXCLUDE_CSR_AM_MODULE */
