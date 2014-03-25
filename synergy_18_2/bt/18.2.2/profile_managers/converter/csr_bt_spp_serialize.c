/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_msgconv.h"
#include "csr_bt_autogen.h"
#include "csr_unicode.h"
#include "csr_bt_spp_prim.h"

#ifndef EXCLUDE_CSR_BT_SPP_MODULE

void CsrBtSppPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_SPP_CONNECT_IND
CsrSize CsrBtSppConnectIndSizeof(void *msg)
{
    CsrBtSppConnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SPP_CONNECT_IND
CsrUint8 *CsrBtSppConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppConnectInd *primitive;

    primitive = (CsrBtSppConnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->queueId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serverChannel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->profileMaxFrameSize);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->validPortPar);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.baud_rate);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.data_bits);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.stop_bits);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.parity);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.parity_type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.flow_ctrl_mask);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.xon);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.xoff);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->portPar.parameter_mask);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SPP_CONNECT_IND
void *CsrBtSppConnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppConnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppConnectInd *) CsrPmemZalloc(sizeof(CsrBtSppConnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->queueId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->serverChannel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->profileMaxFrameSize, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->validPortPar, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.baud_rate, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.data_bits, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.stop_bits, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.parity, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.parity_type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.flow_ctrl_mask, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.xon, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.xoff, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->portPar.parameter_mask, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SPP_PORTNEG_IND
CsrSize CsrBtSppPortnegIndSizeof(void *msg)
{
    CsrBtSppPortnegInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_PORTNEG_IND */

#ifndef EXCLUDE_CSR_BT_SPP_PORTNEG_IND
CsrUint8 *CsrBtSppPortnegIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppPortnegInd *primitive;

    primitive = (CsrBtSppPortnegInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serverChannel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->queueId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.baud_rate);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.data_bits);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.stop_bits);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.parity);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.parity_type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.flow_ctrl_mask);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.xon);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.xoff);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->portPar.parameter_mask);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->request);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_PORTNEG_IND */

#ifndef EXCLUDE_CSR_BT_SPP_PORTNEG_IND
void *CsrBtSppPortnegIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppPortnegInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppPortnegInd *) CsrPmemZalloc(sizeof(CsrBtSppPortnegInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->serverChannel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->queueId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.baud_rate, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.data_bits, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.stop_bits, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.parity, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.parity_type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.flow_ctrl_mask, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.xon, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.xoff, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->portPar.parameter_mask, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->request, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_PORTNEG_IND */

#ifndef EXCLUDE_CSR_BT_SPP_DATA_PATH_STATUS_IND
CsrSize CsrBtSppDataPathStatusIndSizeof(void *msg)
{
    CsrBtSppDataPathStatusInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_DATA_PATH_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_SPP_DATA_PATH_STATUS_IND
CsrUint8 *CsrBtSppDataPathStatusIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppDataPathStatusInd *primitive;

    primitive = (CsrBtSppDataPathStatusInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->queueId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_DATA_PATH_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_SPP_DATA_PATH_STATUS_IND
void *CsrBtSppDataPathStatusIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppDataPathStatusInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppDataPathStatusInd *) CsrPmemZalloc(sizeof(CsrBtSppDataPathStatusInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->queueId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_DATA_PATH_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_SPP_PORTNEG_CFM
CsrSize CsrBtSppPortnegCfmSizeof(void *msg)
{
    CsrBtSppPortnegCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_PORTNEG_CFM */

#ifndef EXCLUDE_CSR_BT_SPP_PORTNEG_CFM
CsrUint8 *CsrBtSppPortnegCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppPortnegCfm *primitive;

    primitive = (CsrBtSppPortnegCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serverChannel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->queueId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.baud_rate);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.data_bits);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.stop_bits);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.parity);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.parity_type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.flow_ctrl_mask);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.xon);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.xoff);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->portPar.parameter_mask);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_PORTNEG_CFM */

#ifndef EXCLUDE_CSR_BT_SPP_PORTNEG_CFM
void *CsrBtSppPortnegCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppPortnegCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppPortnegCfm *) CsrPmemZalloc(sizeof(CsrBtSppPortnegCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->serverChannel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->queueId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.baud_rate, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.data_bits, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.stop_bits, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.parity, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.parity_type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.flow_ctrl_mask, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.xon, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.xoff, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->portPar.parameter_mask, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_PORTNEG_CFM */

#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_RENEGOTIATE_REQ
CsrSize CsrBtSppAudioRenegotiateReqSizeof(void *msg)
{
    CsrBtSppAudioRenegotiateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_RENEGOTIATE_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_RENEGOTIATE_REQ
CsrUint8 *CsrBtSppAudioRenegotiateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppAudioRenegotiateReq *primitive;

    primitive = (CsrBtSppAudioRenegotiateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serverChannel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->audioQuality);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxLatency);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->reTxEffort);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->scoHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_RENEGOTIATE_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_RENEGOTIATE_REQ
void *CsrBtSppAudioRenegotiateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppAudioRenegotiateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppAudioRenegotiateReq *) CsrPmemZalloc(sizeof(CsrBtSppAudioRenegotiateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->serverChannel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->audioQuality, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxLatency, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->reTxEffort, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->scoHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_RENEGOTIATE_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_GET_INSTANCES_QID_REQ
CsrSize CsrBtSppGetInstancesQidReqSizeof(void *msg)
{
    CsrBtSppGetInstancesQidReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_GET_INSTANCES_QID_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_GET_INSTANCES_QID_REQ
CsrUint8 *CsrBtSppGetInstancesQidReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppGetInstancesQidReq *primitive;

    primitive = (CsrBtSppGetInstancesQidReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_GET_INSTANCES_QID_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_GET_INSTANCES_QID_REQ
void *CsrBtSppGetInstancesQidReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppGetInstancesQidReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppGetInstancesQidReq *) CsrPmemZalloc(sizeof(CsrBtSppGetInstancesQidReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_GET_INSTANCES_QID_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_DISCONNECT_REQ
CsrSize CsrBtSppDisconnectReqSizeof(void *msg)
{
    CsrBtSppDisconnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_DISCONNECT_REQ
CsrUint8 *CsrBtSppDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppDisconnectReq *primitive;

    primitive = (CsrBtSppDisconnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serverChannel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_DISCONNECT_REQ
void *CsrBtSppDisconnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppDisconnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppDisconnectReq *) CsrPmemZalloc(sizeof(CsrBtSppDisconnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->serverChannel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_CONNECT_REQ
CsrSize CsrBtSppAudioConnectReqSizeof(void *msg)
{
    CsrBtSppAudioConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->audioParametersLength * sizeof(CsrBtSppAudioParameterList)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_CONNECT_REQ
CsrUint8 *CsrBtSppAudioConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppAudioConnectReq *primitive;

    primitive = (CsrBtSppAudioConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serverChannel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->audioParametersLength);
    CsrMemCpySer(buffer, length, &primitive->audioParameters, sizeof(primitive->audioParameters));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->pcmSlot);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->pcmReassign);
    if(primitive->audioParameters)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->audioParameters), ((CsrSize) (primitive->audioParametersLength * sizeof(CsrBtSppAudioParameterList))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_CONNECT_REQ
void *CsrBtSppAudioConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppAudioConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppAudioConnectReq *) CsrPmemZalloc(sizeof(CsrBtSppAudioConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->serverChannel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->audioParametersLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->audioParameters), buffer, &offset, ((CsrSize) sizeof(primitive->audioParameters)));
    CsrUint8Des((CsrUint8 *) &primitive->pcmSlot, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->pcmReassign, buffer, &offset);
    if (primitive->audioParametersLength)
    {
        primitive->audioParameters = CsrPmemZalloc(((CsrUint32) (primitive->audioParametersLength * sizeof(CsrBtSppAudioParameterList))));
        CsrMemCpyDes(((void *) primitive->audioParameters), buffer, &offset, ((CsrSize) (primitive->audioParametersLength * sizeof(CsrBtSppAudioParameterList))));
    }
    else
    {
        primitive->audioParameters = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_CONNECT_REQ
void CsrBtSppAudioConnectReqSerFree(void *msg)
{
    CsrBtSppAudioConnectReq *primitive;

    primitive = (CsrBtSppAudioConnectReq *) msg;

    if(primitive->audioParameters != NULL)
    {
        CsrPmemFree(primitive->audioParameters);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_CONNECT_CFM
CsrSize CsrBtSppAudioConnectCfmSizeof(void *msg)
{
    CsrBtSppAudioConnectCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_CONNECT_CFM
CsrUint8 *CsrBtSppAudioConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppAudioConnectCfm *primitive;

    primitive = (CsrBtSppAudioConnectCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->queueId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serverChannel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->linkType);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->txInterval);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->weSco);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->rxPacketLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->txPacketLength);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->airMode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->scoHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->pcmSlot);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_CONNECT_CFM
void *CsrBtSppAudioConnectCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppAudioConnectCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppAudioConnectCfm *) CsrPmemZalloc(sizeof(CsrBtSppAudioConnectCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->queueId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->serverChannel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->linkType, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->txInterval, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->weSco, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->rxPacketLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->txPacketLength, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->airMode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->scoHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->pcmSlot, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_DISCONNECT_REQ
CsrSize CsrBtSppAudioDisconnectReqSizeof(void *msg)
{
    CsrBtSppAudioDisconnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_DISCONNECT_REQ
CsrUint8 *CsrBtSppAudioDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppAudioDisconnectReq *primitive;

    primitive = (CsrBtSppAudioDisconnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serverChannel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->scoHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_DISCONNECT_REQ
void *CsrBtSppAudioDisconnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppAudioDisconnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppAudioDisconnectReq *) CsrPmemZalloc(sizeof(CsrBtSppAudioDisconnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->serverChannel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->scoHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_SECURITY_OUT_CFM
CsrSize CsrBtSppSecurityOutCfmSizeof(void *msg)
{
    CsrBtSppSecurityOutCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_SPP_SECURITY_OUT_CFM
CsrUint8 *CsrBtSppSecurityOutCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppSecurityOutCfm *primitive;

    primitive = (CsrBtSppSecurityOutCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_SPP_SECURITY_OUT_CFM
void *CsrBtSppSecurityOutCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppSecurityOutCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppSecurityOutCfm *) CsrPmemZalloc(sizeof(CsrBtSppSecurityOutCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_SPP_CONTROL_IND
CsrSize CsrBtSppControlIndSizeof(void *msg)
{
    CsrBtSppControlInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_CONTROL_IND */

#ifndef EXCLUDE_CSR_BT_SPP_CONTROL_IND
CsrUint8 *CsrBtSppControlIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppControlInd *primitive;

    primitive = (CsrBtSppControlInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->queueId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serverChannel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->modemstatus);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->break_signal);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_CONTROL_IND */

#ifndef EXCLUDE_CSR_BT_SPP_CONTROL_IND
void *CsrBtSppControlIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppControlInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppControlInd *) CsrPmemZalloc(sizeof(CsrBtSppControlInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->queueId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->serverChannel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->modemstatus, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->break_signal, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_CONTROL_IND */

#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_DISCONNECT_CFM
CsrSize CsrBtSppAudioDisconnectCfmSizeof(void *msg)
{
    CsrBtSppAudioDisconnectCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_DISCONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_DISCONNECT_CFM
CsrUint8 *CsrBtSppAudioDisconnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppAudioDisconnectCfm *primitive;

    primitive = (CsrBtSppAudioDisconnectCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->queueId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serverChannel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->scoHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_DISCONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_DISCONNECT_CFM
void *CsrBtSppAudioDisconnectCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppAudioDisconnectCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppAudioDisconnectCfm *) CsrPmemZalloc(sizeof(CsrBtSppAudioDisconnectCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->queueId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->serverChannel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->scoHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_DISCONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_ACCEPT_CONNECT_IND
CsrSize CsrBtSppAudioAcceptConnectIndSizeof(void *msg)
{
    CsrBtSppAudioAcceptConnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_ACCEPT_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_ACCEPT_CONNECT_IND
CsrUint8 *CsrBtSppAudioAcceptConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppAudioAcceptConnectInd *primitive;

    primitive = (CsrBtSppAudioAcceptConnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->queueId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serverChannel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->linkType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_ACCEPT_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_ACCEPT_CONNECT_IND
void *CsrBtSppAudioAcceptConnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppAudioAcceptConnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppAudioAcceptConnectInd *) CsrPmemZalloc(sizeof(CsrBtSppAudioAcceptConnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->queueId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->serverChannel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->linkType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_ACCEPT_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SPP_SECURITY_OUT_REQ
CsrSize CsrBtSppSecurityOutReqSizeof(void *msg)
{
    CsrBtSppSecurityOutReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_SECURITY_OUT_REQ
CsrUint8 *CsrBtSppSecurityOutReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppSecurityOutReq *primitive;

    primitive = (CsrBtSppSecurityOutReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_SECURITY_OUT_REQ
void *CsrBtSppSecurityOutReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppSecurityOutReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppSecurityOutReq *) CsrPmemZalloc(sizeof(CsrBtSppSecurityOutReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_ACTIVATE_REQ
CsrSize CsrBtSppActivateReqSizeof(void *msg)
{
    CsrBtSppActivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->serviceName));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_ACTIVATE_REQ
CsrUint8 *CsrBtSppActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppActivateReq *primitive;

    primitive = (CsrBtSppActivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->timeout);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->role);
    CsrMemCpySer(buffer, length, &primitive->serviceName, sizeof(primitive->serviceName));
    CsrCharStringSer(buffer, length, primitive->serviceName);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_ACTIVATE_REQ
void *CsrBtSppActivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppActivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppActivateReq *) CsrPmemZalloc(sizeof(CsrBtSppActivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->timeout, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->role, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->serviceName), buffer, &offset, ((CsrSize) sizeof(primitive->serviceName)));
    CsrCharStringDes(&primitive->serviceName, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_ACTIVATE_REQ
void CsrBtSppActivateReqSerFree(void *msg)
{
    CsrBtSppActivateReq *primitive;

    primitive = (CsrBtSppActivateReq *) msg;

    if(primitive->serviceName != NULL)
    {
        CsrPmemFree(primitive->serviceName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SPP_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_MODE_CHANGE_REQ
CsrSize CsrBtSppModeChangeReqSizeof(void *msg)
{
    CsrBtSppModeChangeReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_MODE_CHANGE_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_MODE_CHANGE_REQ
CsrUint8 *CsrBtSppModeChangeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppModeChangeReq *primitive;

    primitive = (CsrBtSppModeChangeReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serverChannel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_MODE_CHANGE_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_MODE_CHANGE_REQ
void *CsrBtSppModeChangeReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppModeChangeReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppModeChangeReq *) CsrPmemZalloc(sizeof(CsrBtSppModeChangeReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->serverChannel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_MODE_CHANGE_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_EXTENDED_UUID_CONNECT_REQ
CsrSize CsrBtSppExtendedUuidConnectReqSizeof(void *msg)
{
    CsrBtSppExtendedUuidConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_EXTENDED_UUID_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_EXTENDED_UUID_CONNECT_REQ
CsrUint8 *CsrBtSppExtendedUuidConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppExtendedUuidConnectReq *primitive;

    primitive = (CsrBtSppExtendedUuidConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->role);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->validPortPar);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->requestPortPar);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.baud_rate);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.data_bits);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.stop_bits);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.parity);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.parity_type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.flow_ctrl_mask);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.xon);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.xoff);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->portPar.parameter_mask);
    CsrMemCpySer(buffer, length, ((const void *) primitive->profileUuid), ((CsrSize) (16)));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_EXTENDED_UUID_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_EXTENDED_UUID_CONNECT_REQ
void *CsrBtSppExtendedUuidConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppExtendedUuidConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppExtendedUuidConnectReq *) CsrPmemZalloc(sizeof(CsrBtSppExtendedUuidConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->role, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->validPortPar, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->requestPortPar, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.baud_rate, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.data_bits, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.stop_bits, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.parity, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.parity_type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.flow_ctrl_mask, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.xon, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.xoff, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->portPar.parameter_mask, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->profileUuid), buffer, &offset, ((CsrSize) (16)));
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_EXTENDED_UUID_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_SECURITY_IN_CFM
CsrSize CsrBtSppSecurityInCfmSizeof(void *msg)
{
    CsrBtSppSecurityInCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_SPP_SECURITY_IN_CFM
CsrUint8 *CsrBtSppSecurityInCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppSecurityInCfm *primitive;

    primitive = (CsrBtSppSecurityInCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_SPP_SECURITY_IN_CFM
void *CsrBtSppSecurityInCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppSecurityInCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppSecurityInCfm *) CsrPmemZalloc(sizeof(CsrBtSppSecurityInCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_SPP_MODE_CHANGE_IND
CsrSize CsrBtSppModeChangeIndSizeof(void *msg)
{
    CsrBtSppModeChangeInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_MODE_CHANGE_IND */

#ifndef EXCLUDE_CSR_BT_SPP_MODE_CHANGE_IND
CsrUint8 *CsrBtSppModeChangeIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppModeChangeInd *primitive;

    primitive = (CsrBtSppModeChangeInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->queueId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serverChannel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_MODE_CHANGE_IND */

#ifndef EXCLUDE_CSR_BT_SPP_MODE_CHANGE_IND
void *CsrBtSppModeChangeIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppModeChangeInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppModeChangeInd *) CsrPmemZalloc(sizeof(CsrBtSppModeChangeInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->queueId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->serverChannel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_MODE_CHANGE_IND */

#ifndef EXCLUDE_CSR_BT_SPP_EXTENDED_CONNECT_REQ
CsrSize CsrBtSppExtendedConnectReqSizeof(void *msg)
{
    CsrBtSppExtendedConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_EXTENDED_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_EXTENDED_CONNECT_REQ
CsrUint8 *CsrBtSppExtendedConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppExtendedConnectReq *primitive;

    primitive = (CsrBtSppExtendedConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->role);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->validPortPar);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->requestPortPar);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.baud_rate);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.data_bits);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.stop_bits);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.parity);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.parity_type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.flow_ctrl_mask);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.xon);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.xoff);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->portPar.parameter_mask);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->profileUuid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_EXTENDED_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_EXTENDED_CONNECT_REQ
void *CsrBtSppExtendedConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppExtendedConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppExtendedConnectReq *) CsrPmemZalloc(sizeof(CsrBtSppExtendedConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->role, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->validPortPar, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->requestPortPar, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.baud_rate, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.data_bits, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.stop_bits, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.parity, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.parity_type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.flow_ctrl_mask, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.xon, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.xoff, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->portPar.parameter_mask, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->profileUuid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_EXTENDED_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_ACCEPT_CONNECT_RES
CsrSize CsrBtSppAudioAcceptConnectResSizeof(void *msg)
{
    CsrBtSppAudioAcceptConnectRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->acceptParametersLength * sizeof(CsrBtSppAudioIncomingAcceptParameters)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_ACCEPT_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_ACCEPT_CONNECT_RES
CsrUint8 *CsrBtSppAudioAcceptConnectResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppAudioAcceptConnectRes *primitive;

    primitive = (CsrBtSppAudioAcceptConnectRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serverChannel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->acceptResponse);
    CsrMemCpySer(buffer, length, &primitive->acceptParameters, sizeof(primitive->acceptParameters));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->acceptParametersLength);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->pcmSlot);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->pcmReassign);
    if(primitive->acceptParameters)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->acceptParameters), ((CsrSize) (primitive->acceptParametersLength * sizeof(CsrBtSppAudioIncomingAcceptParameters))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_ACCEPT_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_ACCEPT_CONNECT_RES
void *CsrBtSppAudioAcceptConnectResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppAudioAcceptConnectRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppAudioAcceptConnectRes *) CsrPmemZalloc(sizeof(CsrBtSppAudioAcceptConnectRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->serverChannel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->acceptResponse, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->acceptParameters), buffer, &offset, ((CsrSize) sizeof(primitive->acceptParameters)));
    CsrUint8Des((CsrUint8 *) &primitive->acceptParametersLength, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->pcmSlot, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->pcmReassign, buffer, &offset);
    if (primitive->acceptParametersLength)
    {
        primitive->acceptParameters = CsrPmemZalloc(((CsrUint32) (primitive->acceptParametersLength * sizeof(CsrBtSppAudioIncomingAcceptParameters))));
        CsrMemCpyDes(((void *) primitive->acceptParameters), buffer, &offset, ((CsrSize) (primitive->acceptParametersLength * sizeof(CsrBtSppAudioIncomingAcceptParameters))));
    }
    else
    {
        primitive->acceptParameters = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_ACCEPT_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_ACCEPT_CONNECT_RES
void CsrBtSppAudioAcceptConnectResSerFree(void *msg)
{
    CsrBtSppAudioAcceptConnectRes *primitive;

    primitive = (CsrBtSppAudioAcceptConnectRes *) msg;

    if(primitive->acceptParameters != NULL)
    {
        CsrPmemFree(primitive->acceptParameters);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_ACCEPT_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_SPP_PORTNEG_RES
CsrSize CsrBtSppPortnegResSizeof(void *msg)
{
    CsrBtSppPortnegRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_PORTNEG_RES */

#ifndef EXCLUDE_CSR_BT_SPP_PORTNEG_RES
CsrUint8 *CsrBtSppPortnegResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppPortnegRes *primitive;

    primitive = (CsrBtSppPortnegRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serverChannel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.baud_rate);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.data_bits);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.stop_bits);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.parity);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.parity_type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.flow_ctrl_mask);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.xon);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.xoff);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->portPar.parameter_mask);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_PORTNEG_RES */

#ifndef EXCLUDE_CSR_BT_SPP_PORTNEG_RES
void *CsrBtSppPortnegResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppPortnegRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppPortnegRes *) CsrPmemZalloc(sizeof(CsrBtSppPortnegRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->serverChannel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.baud_rate, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.data_bits, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.stop_bits, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.parity, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.parity_type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.flow_ctrl_mask, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.xon, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.xoff, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->portPar.parameter_mask, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_PORTNEG_RES */

#ifndef EXCLUDE_CSR_BT_SPP_REGISTER_DATA_PATH_HANDLE_CFM
CsrSize CsrBtSppRegisterDataPathHandleCfmSizeof(void *msg)
{
    CsrBtSppRegisterDataPathHandleCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_REGISTER_DATA_PATH_HANDLE_CFM */

#ifndef EXCLUDE_CSR_BT_SPP_REGISTER_DATA_PATH_HANDLE_CFM
CsrUint8 *CsrBtSppRegisterDataPathHandleCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppRegisterDataPathHandleCfm *primitive;

    primitive = (CsrBtSppRegisterDataPathHandleCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_REGISTER_DATA_PATH_HANDLE_CFM */

#ifndef EXCLUDE_CSR_BT_SPP_REGISTER_DATA_PATH_HANDLE_CFM
void *CsrBtSppRegisterDataPathHandleCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppRegisterDataPathHandleCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppRegisterDataPathHandleCfm *) CsrPmemZalloc(sizeof(CsrBtSppRegisterDataPathHandleCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_REGISTER_DATA_PATH_HANDLE_CFM */

#ifndef EXCLUDE_CSR_BT_SPP_DEACTIVATE_REQ
CsrSize CsrBtSppDeactivateReqSizeof(void *msg)
{
    CsrBtSppDeactivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_DEACTIVATE_REQ
CsrUint8 *CsrBtSppDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppDeactivateReq *primitive;

    primitive = (CsrBtSppDeactivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_DEACTIVATE_REQ
void *CsrBtSppDeactivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppDeactivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppDeactivateReq *) CsrPmemZalloc(sizeof(CsrBtSppDeactivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_REGISTER_DATA_PATH_HANDLE_REQ
CsrSize CsrBtSppRegisterDataPathHandleReqSizeof(void *msg)
{
    CsrBtSppRegisterDataPathHandleReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_REGISTER_DATA_PATH_HANDLE_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_REGISTER_DATA_PATH_HANDLE_REQ
CsrUint8 *CsrBtSppRegisterDataPathHandleReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppRegisterDataPathHandleReq *primitive;

    primitive = (CsrBtSppRegisterDataPathHandleReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->dataAppHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_REGISTER_DATA_PATH_HANDLE_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_REGISTER_DATA_PATH_HANDLE_REQ
void *CsrBtSppRegisterDataPathHandleReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppRegisterDataPathHandleReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppRegisterDataPathHandleReq *) CsrPmemZalloc(sizeof(CsrBtSppRegisterDataPathHandleReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->dataAppHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_REGISTER_DATA_PATH_HANDLE_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_SECURITY_IN_REQ
CsrSize CsrBtSppSecurityInReqSizeof(void *msg)
{
    CsrBtSppSecurityInReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_SECURITY_IN_REQ
CsrUint8 *CsrBtSppSecurityInReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppSecurityInReq *primitive;

    primitive = (CsrBtSppSecurityInReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_SECURITY_IN_REQ
void *CsrBtSppSecurityInReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppSecurityInReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppSecurityInReq *) CsrPmemZalloc(sizeof(CsrBtSppSecurityInReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_DATA_CFM
CsrSize CsrBtSppDataCfmSizeof(void *msg)
{
    CsrBtSppDataCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_SPP_DATA_CFM
CsrUint8 *CsrBtSppDataCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppDataCfm *primitive;

    primitive = (CsrBtSppDataCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->queueId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serverChannel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_SPP_DATA_CFM
void *CsrBtSppDataCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppDataCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppDataCfm *) CsrPmemZalloc(sizeof(CsrBtSppDataCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->queueId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->serverChannel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_SPP_CONTROL_REQ
CsrSize CsrBtSppControlReqSizeof(void *msg)
{
    CsrBtSppControlReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_CONTROL_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_CONTROL_REQ
CsrUint8 *CsrBtSppControlReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppControlReq *primitive;

    primitive = (CsrBtSppControlReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serverChannel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->modemstatus);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->break_signal);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_CONTROL_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_CONTROL_REQ
void *CsrBtSppControlReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppControlReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppControlReq *) CsrPmemZalloc(sizeof(CsrBtSppControlReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->serverChannel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->modemstatus, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->break_signal, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_CONTROL_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_EXTENDED_ACTIVATE_REQ
CsrSize CsrBtSppExtendedActivateReqSizeof(void *msg)
{
    CsrBtSppExtendedActivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->serviceRecordSize * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_EXTENDED_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_EXTENDED_ACTIVATE_REQ
CsrUint8 *CsrBtSppExtendedActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppExtendedActivateReq *primitive;

    primitive = (CsrBtSppExtendedActivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->timeout);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->serverChannelIndex);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->serviceRecordSize);
    CsrMemCpySer(buffer, length, &primitive->serviceRecord, sizeof(primitive->serviceRecord));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->classOfDevice);
    if(primitive->serviceRecord)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->serviceRecord), ((CsrSize) (primitive->serviceRecordSize * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_EXTENDED_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_EXTENDED_ACTIVATE_REQ
void *CsrBtSppExtendedActivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppExtendedActivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppExtendedActivateReq *) CsrPmemZalloc(sizeof(CsrBtSppExtendedActivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->timeout, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->serverChannelIndex, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->serviceRecordSize, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->serviceRecord), buffer, &offset, ((CsrSize) sizeof(primitive->serviceRecord)));
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->classOfDevice, buffer, &offset);
    if (primitive->serviceRecordSize)
    {
        primitive->serviceRecord = CsrPmemZalloc(((CsrUint32) (primitive->serviceRecordSize * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->serviceRecord), buffer, &offset, ((CsrSize) (primitive->serviceRecordSize * sizeof(CsrUint8))));
    }
    else
    {
        primitive->serviceRecord = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_EXTENDED_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_EXTENDED_ACTIVATE_REQ
void CsrBtSppExtendedActivateReqSerFree(void *msg)
{
    CsrBtSppExtendedActivateReq *primitive;

    primitive = (CsrBtSppExtendedActivateReq *) msg;

    if(primitive->serviceRecord != NULL)
    {
        CsrPmemFree(primitive->serviceRecord);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SPP_EXTENDED_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_SERVICE_NAME_IND
CsrSize CsrBtSppServiceNameIndSizeof(void *msg)
{
    CsrBtSppServiceNameInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->serviceNameListSize * sizeof(CsrBtSppServiceName)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_SERVICE_NAME_IND */

#ifndef EXCLUDE_CSR_BT_SPP_SERVICE_NAME_IND
CsrUint8 *CsrBtSppServiceNameIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppServiceNameInd *primitive;

    primitive = (CsrBtSppServiceNameInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->queueId);
    CsrMemCpySer(buffer, length, &primitive->serviceNameList, sizeof(primitive->serviceNameList));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->serviceNameListSize);
    if(primitive->serviceNameList)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->serviceNameList), ((CsrSize) (primitive->serviceNameListSize * sizeof(CsrBtSppServiceName))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_SERVICE_NAME_IND */

#ifndef EXCLUDE_CSR_BT_SPP_SERVICE_NAME_IND
void *CsrBtSppServiceNameIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppServiceNameInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppServiceNameInd *) CsrPmemZalloc(sizeof(CsrBtSppServiceNameInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->queueId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->serviceNameList), buffer, &offset, ((CsrSize) sizeof(primitive->serviceNameList)));
    CsrUint16Des((CsrUint16 *) &primitive->serviceNameListSize, buffer, &offset);
    if (primitive->serviceNameListSize)
    {
        primitive->serviceNameList = CsrPmemZalloc(((CsrUint32) (primitive->serviceNameListSize * sizeof(CsrBtSppServiceName))));
        CsrMemCpyDes(((void *) primitive->serviceNameList), buffer, &offset, ((CsrSize) (primitive->serviceNameListSize * sizeof(CsrBtSppServiceName))));
    }
    else
    {
        primitive->serviceNameList = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_SERVICE_NAME_IND */

#ifndef EXCLUDE_CSR_BT_SPP_SERVICE_NAME_IND
void CsrBtSppServiceNameIndSerFree(void *msg)
{
    CsrBtSppServiceNameInd *primitive;

    primitive = (CsrBtSppServiceNameInd *) msg;

    if(primitive->serviceNameList != NULL)
    {
        CsrPmemFree(primitive->serviceNameList);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SPP_SERVICE_NAME_IND */

#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_RENEGOTIATE_CFM
CsrSize CsrBtSppAudioRenegotiateCfmSizeof(void *msg)
{
    CsrBtSppAudioRenegotiateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_RENEGOTIATE_CFM */

#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_RENEGOTIATE_CFM
CsrUint8 *CsrBtSppAudioRenegotiateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppAudioRenegotiateCfm *primitive;

    primitive = (CsrBtSppAudioRenegotiateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->queueId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serverChannel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->scoHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_RENEGOTIATE_CFM */

#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_RENEGOTIATE_CFM
void *CsrBtSppAudioRenegotiateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppAudioRenegotiateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppAudioRenegotiateCfm *) CsrPmemZalloc(sizeof(CsrBtSppAudioRenegotiateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->queueId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->serverChannel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->scoHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_RENEGOTIATE_CFM */

#ifndef EXCLUDE_CSR_BT_SPP_ACCEPT_AUDIO_REQ
CsrSize CsrBtSppAcceptAudioReqSizeof(void *msg)
{
    CsrBtSppAcceptAudioReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_ACCEPT_AUDIO_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_ACCEPT_AUDIO_REQ
CsrUint8 *CsrBtSppAcceptAudioReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppAcceptAudioReq *primitive;

    primitive = (CsrBtSppAcceptAudioReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serverChannel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->audioQuality);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->txBandwidth);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->rxBandwidth);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxLatency);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->voiceSettings);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->reTxEffort);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_ACCEPT_AUDIO_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_ACCEPT_AUDIO_REQ
void *CsrBtSppAcceptAudioReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppAcceptAudioReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppAcceptAudioReq *) CsrPmemZalloc(sizeof(CsrBtSppAcceptAudioReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->serverChannel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->audioQuality, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->txBandwidth, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->rxBandwidth, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxLatency, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->voiceSettings, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->reTxEffort, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_ACCEPT_AUDIO_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_DATA_IND
CsrSize CsrBtSppDataIndSizeof(void *msg)
{
    CsrBtSppDataInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_DATA_IND */

#ifndef EXCLUDE_CSR_BT_SPP_DATA_IND
CsrUint8 *CsrBtSppDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppDataInd *primitive;

    primitive = (CsrBtSppDataInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->queueId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serverChannel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_DATA_IND */

#ifndef EXCLUDE_CSR_BT_SPP_DATA_IND
void *CsrBtSppDataIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppDataInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppDataInd *) CsrPmemZalloc(sizeof(CsrBtSppDataInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->queueId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->serverChannel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->payloadLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    if (primitive->payloadLength)
    {
        primitive->payload = CsrPmemZalloc(((CsrUint32) (primitive->payloadLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->payload), buffer, &offset, ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->payload = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_DATA_IND */

#ifndef EXCLUDE_CSR_BT_SPP_DATA_IND
void CsrBtSppDataIndSerFree(void *msg)
{
    CsrBtSppDataInd *primitive;

    primitive = (CsrBtSppDataInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SPP_DATA_IND */

#ifndef EXCLUDE_CSR_BT_SPP_DISCONNECT_IND
CsrSize CsrBtSppDisconnectIndSizeof(void *msg)
{
    CsrBtSppDisconnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SPP_DISCONNECT_IND
CsrUint8 *CsrBtSppDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppDisconnectInd *primitive;

    primitive = (CsrBtSppDisconnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->queueId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serverChannel);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonSupplier);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->localTerminated);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SPP_DISCONNECT_IND
void *CsrBtSppDisconnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppDisconnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppDisconnectInd *) CsrPmemZalloc(sizeof(CsrBtSppDisconnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->queueId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->serverChannel, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonSupplier, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->localTerminated, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SPP_GET_INSTANCES_QID_CFM
CsrSize CsrBtSppGetInstancesQidCfmSizeof(void *msg)
{
    CsrBtSppGetInstancesQidCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->phandlesListSize * sizeof(CsrSchedQid)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_GET_INSTANCES_QID_CFM */

#ifndef EXCLUDE_CSR_BT_SPP_GET_INSTANCES_QID_CFM
CsrUint8 *CsrBtSppGetInstancesQidCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppGetInstancesQidCfm *primitive;

    primitive = (CsrBtSppGetInstancesQidCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->phandlesListSize);
    CsrMemCpySer(buffer, length, &primitive->phandlesList, sizeof(primitive->phandlesList));
    if(primitive->phandlesList)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->phandlesList), ((CsrSize) (primitive->phandlesListSize * sizeof(CsrSchedQid))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_GET_INSTANCES_QID_CFM */

#ifndef EXCLUDE_CSR_BT_SPP_GET_INSTANCES_QID_CFM
void *CsrBtSppGetInstancesQidCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppGetInstancesQidCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppGetInstancesQidCfm *) CsrPmemZalloc(sizeof(CsrBtSppGetInstancesQidCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->phandlesListSize, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->phandlesList), buffer, &offset, ((CsrSize) sizeof(primitive->phandlesList)));
    if (primitive->phandlesListSize)
    {
        primitive->phandlesList = CsrPmemZalloc(((CsrUint32) (primitive->phandlesListSize * sizeof(CsrSchedQid))));
        CsrMemCpyDes(((void *) primitive->phandlesList), buffer, &offset, ((CsrSize) (primitive->phandlesListSize * sizeof(CsrSchedQid))));
    }
    else
    {
        primitive->phandlesList = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_GET_INSTANCES_QID_CFM */

#ifndef EXCLUDE_CSR_BT_SPP_GET_INSTANCES_QID_CFM
void CsrBtSppGetInstancesQidCfmSerFree(void *msg)
{
    CsrBtSppGetInstancesQidCfm *primitive;

    primitive = (CsrBtSppGetInstancesQidCfm *) msg;

    if(primitive->phandlesList != NULL)
    {
        CsrPmemFree(primitive->phandlesList);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SPP_GET_INSTANCES_QID_CFM */

#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_RENEGOTIATE_IND
CsrSize CsrBtSppAudioRenegotiateIndSizeof(void *msg)
{
    CsrBtSppAudioRenegotiateInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_RENEGOTIATE_IND */

#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_RENEGOTIATE_IND
CsrUint8 *CsrBtSppAudioRenegotiateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppAudioRenegotiateInd *primitive;

    primitive = (CsrBtSppAudioRenegotiateInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->queueId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serverChannel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->scoHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_RENEGOTIATE_IND */

#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_RENEGOTIATE_IND
void *CsrBtSppAudioRenegotiateIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppAudioRenegotiateInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppAudioRenegotiateInd *) CsrPmemZalloc(sizeof(CsrBtSppAudioRenegotiateInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->queueId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->serverChannel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->scoHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_RENEGOTIATE_IND */

#ifndef EXCLUDE_CSR_BT_SPP_CONNECT_REQ
CsrSize CsrBtSppConnectReqSizeof(void *msg)
{
    CsrBtSppConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_CONNECT_REQ
CsrUint8 *CsrBtSppConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppConnectReq *primitive;

    primitive = (CsrBtSppConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->role);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->validPortPar);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->requestPortPar);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.baud_rate);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.data_bits);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.stop_bits);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.parity);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.parity_type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.flow_ctrl_mask);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.xon);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.xoff);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->portPar.parameter_mask);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_CONNECT_REQ
void *CsrBtSppConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppConnectReq *) CsrPmemZalloc(sizeof(CsrBtSppConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->role, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->validPortPar, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->requestPortPar, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.baud_rate, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.data_bits, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.stop_bits, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.parity, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.parity_type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.flow_ctrl_mask, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.xon, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.xoff, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->portPar.parameter_mask, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_DISCONNECT_IND
CsrSize CsrBtSppAudioDisconnectIndSizeof(void *msg)
{
    CsrBtSppAudioDisconnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_DISCONNECT_IND
CsrUint8 *CsrBtSppAudioDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppAudioDisconnectInd *primitive;

    primitive = (CsrBtSppAudioDisconnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->queueId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serverChannel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->scoHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_DISCONNECT_IND
void *CsrBtSppAudioDisconnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppAudioDisconnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppAudioDisconnectInd *) CsrPmemZalloc(sizeof(CsrBtSppAudioDisconnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->queueId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->serverChannel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->scoHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SPP_CANCEL_ACCEPT_AUDIO_REQ
CsrSize CsrBtSppCancelAcceptAudioReqSizeof(void *msg)
{
    CsrBtSppCancelAcceptAudioReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_CANCEL_ACCEPT_AUDIO_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_CANCEL_ACCEPT_AUDIO_REQ
CsrUint8 *CsrBtSppCancelAcceptAudioReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppCancelAcceptAudioReq *primitive;

    primitive = (CsrBtSppCancelAcceptAudioReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serverChannel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_CANCEL_ACCEPT_AUDIO_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_CANCEL_ACCEPT_AUDIO_REQ
void *CsrBtSppCancelAcceptAudioReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppCancelAcceptAudioReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppCancelAcceptAudioReq *) CsrPmemZalloc(sizeof(CsrBtSppCancelAcceptAudioReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->serverChannel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_CANCEL_ACCEPT_AUDIO_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_CONNECT_IND
CsrSize CsrBtSppAudioConnectIndSizeof(void *msg)
{
    CsrBtSppAudioConnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_CONNECT_IND
CsrUint8 *CsrBtSppAudioConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppAudioConnectInd *primitive;

    primitive = (CsrBtSppAudioConnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->queueId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serverChannel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->linkType);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->txInterval);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->weSco);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->rxPacketLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->txPacketLength);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->airMode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->scoHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->pcmSlot);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_CONNECT_IND
void *CsrBtSppAudioConnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppAudioConnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppAudioConnectInd *) CsrPmemZalloc(sizeof(CsrBtSppAudioConnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->queueId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->serverChannel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->linkType, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->txInterval, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->weSco, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->rxPacketLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->txPacketLength, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->airMode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->scoHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->pcmSlot, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SPP_STATUS_IND
CsrSize CsrBtSppStatusIndSizeof(void *msg)
{
    CsrBtSppStatusInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_SPP_STATUS_IND
CsrUint8 *CsrBtSppStatusIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppStatusInd *primitive;

    primitive = (CsrBtSppStatusInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->queueId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serverChannel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connect);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxMsgSize);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_SPP_STATUS_IND
void *CsrBtSppStatusIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppStatusInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppStatusInd *) CsrPmemZalloc(sizeof(CsrBtSppStatusInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->queueId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->serverChannel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connect, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxMsgSize, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_SPP_HOUSE_CLEANING
CsrSize CsrBtSppHouseCleaningSizeof(void *msg)
{
    CsrBtSppHouseCleaning *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_SPP_HOUSE_CLEANING
CsrUint8 *CsrBtSppHouseCleaningSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppHouseCleaning *primitive;

    primitive = (CsrBtSppHouseCleaning *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_SPP_HOUSE_CLEANING
void *CsrBtSppHouseCleaningDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppHouseCleaning *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppHouseCleaning *) CsrPmemZalloc(sizeof(CsrBtSppHouseCleaning));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_SPP_DATA_PATH_STATUS_REQ
CsrSize CsrBtSppDataPathStatusReqSizeof(void *msg)
{
    CsrBtSppDataPathStatusReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_DATA_PATH_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_DATA_PATH_STATUS_REQ
CsrUint8 *CsrBtSppDataPathStatusReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppDataPathStatusReq *primitive;

    primitive = (CsrBtSppDataPathStatusReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_DATA_PATH_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_DATA_PATH_STATUS_REQ
void *CsrBtSppDataPathStatusReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppDataPathStatusReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppDataPathStatusReq *) CsrPmemZalloc(sizeof(CsrBtSppDataPathStatusReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_DATA_PATH_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_CANCEL_CONNECT_REQ
CsrSize CsrBtSppCancelConnectReqSizeof(void *msg)
{
    CsrBtSppCancelConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_CANCEL_CONNECT_REQ
CsrUint8 *CsrBtSppCancelConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppCancelConnectReq *primitive;

    primitive = (CsrBtSppCancelConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_CANCEL_CONNECT_REQ
void *CsrBtSppCancelConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppCancelConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppCancelConnectReq *) CsrPmemZalloc(sizeof(CsrBtSppCancelConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_PORTNEG_REQ
CsrSize CsrBtSppPortnegReqSizeof(void *msg)
{
    CsrBtSppPortnegReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_PORTNEG_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_PORTNEG_REQ
CsrUint8 *CsrBtSppPortnegReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppPortnegReq *primitive;

    primitive = (CsrBtSppPortnegReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serverChannel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.baud_rate);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.data_bits);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.stop_bits);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.parity);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.parity_type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.flow_ctrl_mask);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.xon);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.xoff);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->portPar.parameter_mask);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_PORTNEG_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_PORTNEG_REQ
void *CsrBtSppPortnegReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppPortnegReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppPortnegReq *) CsrPmemZalloc(sizeof(CsrBtSppPortnegReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->serverChannel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.baud_rate, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.data_bits, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.stop_bits, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.parity, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.parity_type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.flow_ctrl_mask, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.xon, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.xoff, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->portPar.parameter_mask, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_PORTNEG_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_ACTIVATE_CFM
CsrSize CsrBtSppActivateCfmSizeof(void *msg)
{
    CsrBtSppActivateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_SPP_ACTIVATE_CFM
CsrUint8 *CsrBtSppActivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppActivateCfm *primitive;

    primitive = (CsrBtSppActivateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->queueId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serverChannel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_SPP_ACTIVATE_CFM
void *CsrBtSppActivateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppActivateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppActivateCfm *) CsrPmemZalloc(sizeof(CsrBtSppActivateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->queueId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->serverChannel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_SPP_DATA_RES
CsrSize CsrBtSppDataResSizeof(void *msg)
{
    CsrBtSppDataRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_DATA_RES */

#ifndef EXCLUDE_CSR_BT_SPP_DATA_RES
CsrUint8 *CsrBtSppDataResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppDataRes *primitive;

    primitive = (CsrBtSppDataRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serverChannel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_DATA_RES */

#ifndef EXCLUDE_CSR_BT_SPP_DATA_RES
void *CsrBtSppDataResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppDataRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppDataRes *) CsrPmemZalloc(sizeof(CsrBtSppDataRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->serverChannel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_DATA_RES */

#ifndef EXCLUDE_CSR_BT_SPP_REGISTER_QID_REQ
CsrSize CsrBtSppRegisterQidReqSizeof(void *msg)
{
    CsrBtSppRegisterQidReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_REGISTER_QID_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_REGISTER_QID_REQ
CsrUint8 *CsrBtSppRegisterQidReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppRegisterQidReq *primitive;

    primitive = (CsrBtSppRegisterQidReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_REGISTER_QID_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_REGISTER_QID_REQ
void *CsrBtSppRegisterQidReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppRegisterQidReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppRegisterQidReq *) CsrPmemZalloc(sizeof(CsrBtSppRegisterQidReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_REGISTER_QID_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_DEACTIVATE_CFM
CsrSize CsrBtSppDeactivateCfmSizeof(void *msg)
{
    CsrBtSppDeactivateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_SPP_DEACTIVATE_CFM
CsrUint8 *CsrBtSppDeactivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppDeactivateCfm *primitive;

    primitive = (CsrBtSppDeactivateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_SPP_DEACTIVATE_CFM
void *CsrBtSppDeactivateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppDeactivateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppDeactivateCfm *) CsrPmemZalloc(sizeof(CsrBtSppDeactivateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_SPP_DATA_REQ
CsrSize CsrBtSppDataReqSizeof(void *msg)
{
    CsrBtSppDataReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_DATA_REQ
CsrUint8 *CsrBtSppDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppDataReq *primitive;

    primitive = (CsrBtSppDataReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serverChannel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_DATA_REQ
void *CsrBtSppDataReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppDataReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppDataReq *) CsrPmemZalloc(sizeof(CsrBtSppDataReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->serverChannel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->payloadLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    if (primitive->payloadLength)
    {
        primitive->payload = CsrPmemZalloc(((CsrUint32) (primitive->payloadLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->payload), buffer, &offset, ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->payload = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_DATA_REQ
void CsrBtSppDataReqSerFree(void *msg)
{
    CsrBtSppDataReq *primitive;

    primitive = (CsrBtSppDataReq *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SPP_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_SERVICE_NAME_RES
CsrSize CsrBtSppServiceNameResSizeof(void *msg)
{
    CsrBtSppServiceNameRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SPP_SERVICE_NAME_RES */

#ifndef EXCLUDE_CSR_BT_SPP_SERVICE_NAME_RES
CsrUint8 *CsrBtSppServiceNameResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSppServiceNameRes *primitive;

    primitive = (CsrBtSppServiceNameRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->serviceHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->accept);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SPP_SERVICE_NAME_RES */

#ifndef EXCLUDE_CSR_BT_SPP_SERVICE_NAME_RES
void *CsrBtSppServiceNameResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSppServiceNameRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSppServiceNameRes *) CsrPmemZalloc(sizeof(CsrBtSppServiceNameRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->serviceHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->accept, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SPP_SERVICE_NAME_RES */

#endif /* EXCLUDE_CSR_BT_SPP_MODULE */
