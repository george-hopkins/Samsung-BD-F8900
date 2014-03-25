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
#include "csr_bt_hidh_prim.h"

#ifndef EXCLUDE_CSR_BT_HIDH_MODULE

void CsrBtHidhPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_HIDH_CONNECT_ACCEPT_REQ
CsrSize CsrBtHidhConnectAcceptReqSizeof(void *msg)
{
    CsrBtHidhConnectAcceptReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->qosIntrCount * sizeof(L2CA_QOS_T)) + (primitive->qosCtrlCount * sizeof(L2CA_QOS_T)) + (primitive->sdpInfoCount * sizeof(CsrBtHidhSdpInfo)) + CsrCharStringSerLen((CsrCharString *) primitive->serviceName) + (primitive->descriptorLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDH_CONNECT_ACCEPT_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_CONNECT_ACCEPT_REQ
CsrUint8 *CsrBtHidhConnectAcceptReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHidhConnectAcceptReq *primitive;

    primitive = (CsrBtHidhConnectAcceptReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ctrlHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->slotId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flushTimeout);
    CsrMemCpySer(buffer, length, &primitive->qosIntr, sizeof(primitive->qosIntr));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->qosIntrCount);
    CsrMemCpySer(buffer, length, &primitive->qosCtrl, sizeof(primitive->qosCtrl));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->qosCtrlCount);
    CsrMemCpySer(buffer, length, &primitive->sdpInfo, sizeof(primitive->sdpInfo));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->sdpInfoCount);
    CsrMemCpySer(buffer, length, &primitive->serviceName, sizeof(primitive->serviceName));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->descriptorLength);
    CsrMemCpySer(buffer, length, &primitive->descriptor, sizeof(primitive->descriptor));
    if(primitive->qosIntr)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->qosIntr), ((CsrSize) (primitive->qosIntrCount * sizeof(L2CA_QOS_T))));
    }
    if(primitive->qosCtrl)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->qosCtrl), ((CsrSize) (primitive->qosCtrlCount * sizeof(L2CA_QOS_T))));
    }
    if(primitive->sdpInfo)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->sdpInfo), ((CsrSize) (primitive->sdpInfoCount * sizeof(CsrBtHidhSdpInfo))));
    }
    CsrCharStringSer(buffer, length, primitive->serviceName);
    if(primitive->descriptor)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->descriptor), ((CsrSize) (primitive->descriptorLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDH_CONNECT_ACCEPT_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_CONNECT_ACCEPT_REQ
void *CsrBtHidhConnectAcceptReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHidhConnectAcceptReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHidhConnectAcceptReq *) CsrPmemZalloc(sizeof(CsrBtHidhConnectAcceptReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->ctrlHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->slotId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flushTimeout, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->qosIntr), buffer, &offset, ((CsrSize) sizeof(primitive->qosIntr)));
    CsrUint8Des((CsrUint8 *) &primitive->qosIntrCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->qosCtrl), buffer, &offset, ((CsrSize) sizeof(primitive->qosCtrl)));
    CsrUint8Des((CsrUint8 *) &primitive->qosCtrlCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->sdpInfo), buffer, &offset, ((CsrSize) sizeof(primitive->sdpInfo)));
    CsrUint8Des((CsrUint8 *) &primitive->sdpInfoCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->serviceName), buffer, &offset, ((CsrSize) sizeof(primitive->serviceName)));
    CsrUint16Des((CsrUint16 *) &primitive->descriptorLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->descriptor), buffer, &offset, ((CsrSize) sizeof(primitive->descriptor)));
    if (primitive->qosIntrCount)
    {
        primitive->qosIntr = CsrPmemZalloc(((CsrUint32) (primitive->qosIntrCount * sizeof(L2CA_QOS_T))));
        CsrMemCpyDes(((void *) primitive->qosIntr), buffer, &offset, ((CsrSize) (primitive->qosIntrCount * sizeof(L2CA_QOS_T))));
    }
    else
    {
        primitive->qosIntr = NULL;
    }
    if (primitive->qosCtrlCount)
    {
        primitive->qosCtrl = CsrPmemZalloc(((CsrUint32) (primitive->qosCtrlCount * sizeof(L2CA_QOS_T))));
        CsrMemCpyDes(((void *) primitive->qosCtrl), buffer, &offset, ((CsrSize) (primitive->qosCtrlCount * sizeof(L2CA_QOS_T))));
    }
    else
    {
        primitive->qosCtrl = NULL;
    }
    if (primitive->sdpInfoCount)
    {
        primitive->sdpInfo = CsrPmemZalloc(((CsrUint32) (primitive->sdpInfoCount * sizeof(CsrBtHidhSdpInfo))));
        CsrMemCpyDes(((void *) primitive->sdpInfo), buffer, &offset, ((CsrSize) (primitive->sdpInfoCount * sizeof(CsrBtHidhSdpInfo))));
    }
    else
    {
        primitive->sdpInfo = NULL;
    }
    CsrCharStringDes(&primitive->serviceName, buffer, &offset);
    if (primitive->descriptorLength)
    {
        primitive->descriptor = CsrPmemZalloc(((CsrUint32) (primitive->descriptorLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->descriptor), buffer, &offset, ((CsrSize) (primitive->descriptorLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->descriptor = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDH_CONNECT_ACCEPT_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_CONNECT_ACCEPT_REQ
void CsrBtHidhConnectAcceptReqSerFree(void *msg)
{
    CsrBtHidhConnectAcceptReq *primitive;

    primitive = (CsrBtHidhConnectAcceptReq *) msg;

    if(primitive->qosIntr != NULL)
    {
        CsrPmemFree(primitive->qosIntr);
    }
    if(primitive->qosCtrl != NULL)
    {
        CsrPmemFree(primitive->qosCtrl);
    }
    if(primitive->sdpInfo != NULL)
    {
        CsrPmemFree(primitive->sdpInfo);
    }
    if(primitive->serviceName != NULL)
    {
        CsrPmemFree(primitive->serviceName);
    }
    if(primitive->descriptor != NULL)
    {
        CsrPmemFree(primitive->descriptor);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HIDH_CONNECT_ACCEPT_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_CANCEL_CONNECT_ACCEPT_CFM
CsrSize CsrBtHidhCancelConnectAcceptCfmSizeof(void *msg)
{
    CsrBtHidhCancelConnectAcceptCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDH_CANCEL_CONNECT_ACCEPT_CFM */

#ifndef EXCLUDE_CSR_BT_HIDH_CANCEL_CONNECT_ACCEPT_CFM
CsrUint8 *CsrBtHidhCancelConnectAcceptCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHidhCancelConnectAcceptCfm *primitive;

    primitive = (CsrBtHidhCancelConnectAcceptCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDH_CANCEL_CONNECT_ACCEPT_CFM */

#ifndef EXCLUDE_CSR_BT_HIDH_CANCEL_CONNECT_ACCEPT_CFM
void *CsrBtHidhCancelConnectAcceptCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHidhCancelConnectAcceptCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHidhCancelConnectAcceptCfm *) CsrPmemZalloc(sizeof(CsrBtHidhCancelConnectAcceptCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDH_CANCEL_CONNECT_ACCEPT_CFM */

#ifndef EXCLUDE_CSR_BT_HIDH_DISCONNECT_REQ
CsrSize CsrBtHidhDisconnectReqSizeof(void *msg)
{
    CsrBtHidhDisconnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDH_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_DISCONNECT_REQ
CsrUint8 *CsrBtHidhDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHidhDisconnectReq *primitive;

    primitive = (CsrBtHidhDisconnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDH_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_DISCONNECT_REQ
void *CsrBtHidhDisconnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHidhDisconnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHidhDisconnectReq *) CsrPmemZalloc(sizeof(CsrBtHidhDisconnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDH_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_STATUS_IND
CsrSize CsrBtHidhStatusIndSizeof(void *msg)
{
    CsrBtHidhStatusInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->sdpInfoCount * sizeof(CsrBtHidhSdpInfo)) + CsrCharStringSerLen((CsrCharString *) primitive->serviceName) + (primitive->descriptorLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDH_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_HIDH_STATUS_IND
CsrUint8 *CsrBtHidhStatusIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHidhStatusInd *primitive;

    primitive = (CsrBtHidhStatusInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrMemCpySer(buffer, length, &primitive->sdpInfo, sizeof(primitive->sdpInfo));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->sdpInfoCount);
    CsrMemCpySer(buffer, length, &primitive->serviceName, sizeof(primitive->serviceName));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->descriptorLength);
    CsrMemCpySer(buffer, length, &primitive->descriptor, sizeof(primitive->descriptor));
    if(primitive->sdpInfo)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->sdpInfo), ((CsrSize) (primitive->sdpInfoCount * sizeof(CsrBtHidhSdpInfo))));
    }
    CsrCharStringSer(buffer, length, primitive->serviceName);
    if(primitive->descriptor)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->descriptor), ((CsrSize) (primitive->descriptorLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDH_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_HIDH_STATUS_IND
void *CsrBtHidhStatusIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHidhStatusInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHidhStatusInd *) CsrPmemZalloc(sizeof(CsrBtHidhStatusInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->sdpInfo), buffer, &offset, ((CsrSize) sizeof(primitive->sdpInfo)));
    CsrUint8Des((CsrUint8 *) &primitive->sdpInfoCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->serviceName), buffer, &offset, ((CsrSize) sizeof(primitive->serviceName)));
    CsrUint16Des((CsrUint16 *) &primitive->descriptorLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->descriptor), buffer, &offset, ((CsrSize) sizeof(primitive->descriptor)));
    if (primitive->sdpInfoCount)
    {
        primitive->sdpInfo = CsrPmemZalloc(((CsrUint32) (primitive->sdpInfoCount * sizeof(CsrBtHidhSdpInfo))));
        CsrMemCpyDes(((void *) primitive->sdpInfo), buffer, &offset, ((CsrSize) (primitive->sdpInfoCount * sizeof(CsrBtHidhSdpInfo))));
    }
    else
    {
        primitive->sdpInfo = NULL;
    }
    CsrCharStringDes(&primitive->serviceName, buffer, &offset);
    if (primitive->descriptorLength)
    {
        primitive->descriptor = CsrPmemZalloc(((CsrUint32) (primitive->descriptorLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->descriptor), buffer, &offset, ((CsrSize) (primitive->descriptorLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->descriptor = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDH_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_HIDH_STATUS_IND
void CsrBtHidhStatusIndSerFree(void *msg)
{
    CsrBtHidhStatusInd *primitive;

    primitive = (CsrBtHidhStatusInd *) msg;

    if(primitive->sdpInfo != NULL)
    {
        CsrPmemFree(primitive->sdpInfo);
    }
    if(primitive->serviceName != NULL)
    {
        CsrPmemFree(primitive->serviceName);
    }
    if(primitive->descriptor != NULL)
    {
        CsrPmemFree(primitive->descriptor);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HIDH_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_HIDH_GET_IDLE_REQ
CsrSize CsrBtHidhGetIdleReqSizeof(void *msg)
{
    CsrBtHidhGetIdleReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDH_GET_IDLE_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_GET_IDLE_REQ
CsrUint8 *CsrBtHidhGetIdleReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHidhGetIdleReq *primitive;

    primitive = (CsrBtHidhGetIdleReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDH_GET_IDLE_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_GET_IDLE_REQ
void *CsrBtHidhGetIdleReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHidhGetIdleReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHidhGetIdleReq *) CsrPmemZalloc(sizeof(CsrBtHidhGetIdleReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDH_GET_IDLE_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_HANDSHAKE_IND
CsrSize CsrBtHidhHandshakeIndSizeof(void *msg)
{
    CsrBtHidhHandshakeInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDH_HANDSHAKE_IND */

#ifndef EXCLUDE_CSR_BT_HIDH_HANDSHAKE_IND
CsrUint8 *CsrBtHidhHandshakeIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHidhHandshakeInd *primitive;

    primitive = (CsrBtHidhHandshakeInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDH_HANDSHAKE_IND */

#ifndef EXCLUDE_CSR_BT_HIDH_HANDSHAKE_IND
void *CsrBtHidhHandshakeIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHidhHandshakeInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHidhHandshakeInd *) CsrPmemZalloc(sizeof(CsrBtHidhHandshakeInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDH_HANDSHAKE_IND */

#ifndef EXCLUDE_CSR_BT_HIDH_DISCONNECT_IND
CsrSize CsrBtHidhDisconnectIndSizeof(void *msg)
{
    CsrBtHidhDisconnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDH_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HIDH_DISCONNECT_IND
CsrUint8 *CsrBtHidhDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHidhDisconnectInd *primitive;

    primitive = (CsrBtHidhDisconnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonSupplier);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDH_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HIDH_DISCONNECT_IND
void *CsrBtHidhDisconnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHidhDisconnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHidhDisconnectInd *) CsrPmemZalloc(sizeof(CsrBtHidhDisconnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonSupplier, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDH_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HIDH_CANCEL_CONNECT_ACCEPT_REQ
CsrSize CsrBtHidhCancelConnectAcceptReqSizeof(void *msg)
{
    CsrBtHidhCancelConnectAcceptReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDH_CANCEL_CONNECT_ACCEPT_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_CANCEL_CONNECT_ACCEPT_REQ
CsrUint8 *CsrBtHidhCancelConnectAcceptReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHidhCancelConnectAcceptReq *primitive;

    primitive = (CsrBtHidhCancelConnectAcceptReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDH_CANCEL_CONNECT_ACCEPT_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_CANCEL_CONNECT_ACCEPT_REQ
void *CsrBtHidhCancelConnectAcceptReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHidhCancelConnectAcceptReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHidhCancelConnectAcceptReq *) CsrPmemZalloc(sizeof(CsrBtHidhCancelConnectAcceptReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDH_CANCEL_CONNECT_ACCEPT_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_CONNECT_CFM
CsrSize CsrBtHidhConnectCfmSizeof(void *msg)
{
    CsrBtHidhConnectCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->sdpInfoCount * sizeof(CsrBtHidhSdpInfo)) + CsrCharStringSerLen((CsrCharString *) primitive->serviceName) + (primitive->descriptorLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDH_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_HIDH_CONNECT_CFM
CsrUint8 *CsrBtHidhConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHidhConnectCfm *primitive;

    primitive = (CsrBtHidhConnectCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceId);
    CsrMemCpySer(buffer, length, &primitive->sdpInfo, sizeof(primitive->sdpInfo));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->sdpInfoCount);
    CsrMemCpySer(buffer, length, &primitive->serviceName, sizeof(primitive->serviceName));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->descriptorLength);
    CsrMemCpySer(buffer, length, &primitive->descriptor, sizeof(primitive->descriptor));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    if(primitive->sdpInfo)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->sdpInfo), ((CsrSize) (primitive->sdpInfoCount * sizeof(CsrBtHidhSdpInfo))));
    }
    CsrCharStringSer(buffer, length, primitive->serviceName);
    if(primitive->descriptor)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->descriptor), ((CsrSize) (primitive->descriptorLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDH_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_HIDH_CONNECT_CFM
void *CsrBtHidhConnectCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHidhConnectCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHidhConnectCfm *) CsrPmemZalloc(sizeof(CsrBtHidhConnectCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->sdpInfo), buffer, &offset, ((CsrSize) sizeof(primitive->sdpInfo)));
    CsrUint8Des((CsrUint8 *) &primitive->sdpInfoCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->serviceName), buffer, &offset, ((CsrSize) sizeof(primitive->serviceName)));
    CsrUint16Des((CsrUint16 *) &primitive->descriptorLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->descriptor), buffer, &offset, ((CsrSize) sizeof(primitive->descriptor)));
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    if (primitive->sdpInfoCount)
    {
        primitive->sdpInfo = CsrPmemZalloc(((CsrUint32) (primitive->sdpInfoCount * sizeof(CsrBtHidhSdpInfo))));
        CsrMemCpyDes(((void *) primitive->sdpInfo), buffer, &offset, ((CsrSize) (primitive->sdpInfoCount * sizeof(CsrBtHidhSdpInfo))));
    }
    else
    {
        primitive->sdpInfo = NULL;
    }
    CsrCharStringDes(&primitive->serviceName, buffer, &offset);
    if (primitive->descriptorLength)
    {
        primitive->descriptor = CsrPmemZalloc(((CsrUint32) (primitive->descriptorLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->descriptor), buffer, &offset, ((CsrSize) (primitive->descriptorLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->descriptor = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDH_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_HIDH_CONNECT_CFM
void CsrBtHidhConnectCfmSerFree(void *msg)
{
    CsrBtHidhConnectCfm *primitive;

    primitive = (CsrBtHidhConnectCfm *) msg;

    if(primitive->sdpInfo != NULL)
    {
        CsrPmemFree(primitive->sdpInfo);
    }
    if(primitive->serviceName != NULL)
    {
        CsrPmemFree(primitive->serviceName);
    }
    if(primitive->descriptor != NULL)
    {
        CsrPmemFree(primitive->descriptor);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HIDH_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_HIDH_CONNECT_ACCEPT_CFM
CsrSize CsrBtHidhConnectAcceptCfmSizeof(void *msg)
{
    CsrBtHidhConnectAcceptCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDH_CONNECT_ACCEPT_CFM */

#ifndef EXCLUDE_CSR_BT_HIDH_CONNECT_ACCEPT_CFM
CsrUint8 *CsrBtHidhConnectAcceptCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHidhConnectAcceptCfm *primitive;

    primitive = (CsrBtHidhConnectAcceptCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDH_CONNECT_ACCEPT_CFM */

#ifndef EXCLUDE_CSR_BT_HIDH_CONNECT_ACCEPT_CFM
void *CsrBtHidhConnectAcceptCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHidhConnectAcceptCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHidhConnectAcceptCfm *) CsrPmemZalloc(sizeof(CsrBtHidhConnectAcceptCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDH_CONNECT_ACCEPT_CFM */

#ifndef EXCLUDE_CSR_BT_HIDH_CONTROL_REQ
CsrSize CsrBtHidhControlReqSizeof(void *msg)
{
    CsrBtHidhControlReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDH_CONTROL_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_CONTROL_REQ
CsrUint8 *CsrBtHidhControlReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHidhControlReq *primitive;

    primitive = (CsrBtHidhControlReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->operation);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDH_CONTROL_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_CONTROL_REQ
void *CsrBtHidhControlReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHidhControlReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHidhControlReq *) CsrPmemZalloc(sizeof(CsrBtHidhControlReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->operation, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDH_CONTROL_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_SECURITY_IN_REQ
CsrSize CsrBtHidhSecurityInReqSizeof(void *msg)
{
    CsrBtHidhSecurityInReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDH_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_SECURITY_IN_REQ
CsrUint8 *CsrBtHidhSecurityInReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHidhSecurityInReq *primitive;

    primitive = (CsrBtHidhSecurityInReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDH_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_SECURITY_IN_REQ
void *CsrBtHidhSecurityInReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHidhSecurityInReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHidhSecurityInReq *) CsrPmemZalloc(sizeof(CsrBtHidhSecurityInReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDH_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_GET_PROTOCOL_REQ
CsrSize CsrBtHidhGetProtocolReqSizeof(void *msg)
{
    CsrBtHidhGetProtocolReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDH_GET_PROTOCOL_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_GET_PROTOCOL_REQ
CsrUint8 *CsrBtHidhGetProtocolReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHidhGetProtocolReq *primitive;

    primitive = (CsrBtHidhGetProtocolReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDH_GET_PROTOCOL_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_GET_PROTOCOL_REQ
void *CsrBtHidhGetProtocolReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHidhGetProtocolReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHidhGetProtocolReq *) CsrPmemZalloc(sizeof(CsrBtHidhGetProtocolReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDH_GET_PROTOCOL_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_DISCONNECT_CFM
CsrSize CsrBtHidhDisconnectCfmSizeof(void *msg)
{
    CsrBtHidhDisconnectCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDH_DISCONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_HIDH_DISCONNECT_CFM
CsrUint8 *CsrBtHidhDisconnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHidhDisconnectCfm *primitive;

    primitive = (CsrBtHidhDisconnectCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDH_DISCONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_HIDH_DISCONNECT_CFM
void *CsrBtHidhDisconnectCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHidhDisconnectCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHidhDisconnectCfm *) CsrPmemZalloc(sizeof(CsrBtHidhDisconnectCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDH_DISCONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_HIDH_REGISTER_USER_REQ
CsrSize CsrBtHidhRegisterUserReqSizeof(void *msg)
{
    CsrBtHidhRegisterUserReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDH_REGISTER_USER_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_REGISTER_USER_REQ
CsrUint8 *CsrBtHidhRegisterUserReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHidhRegisterUserReq *primitive;

    primitive = (CsrBtHidhRegisterUserReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->slotId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->userHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDH_REGISTER_USER_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_REGISTER_USER_REQ
void *CsrBtHidhRegisterUserReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHidhRegisterUserReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHidhRegisterUserReq *) CsrPmemZalloc(sizeof(CsrBtHidhRegisterUserReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->slotId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->userHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDH_REGISTER_USER_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_CONTROL_IND
CsrSize CsrBtHidhControlIndSizeof(void *msg)
{
    CsrBtHidhControlInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDH_CONTROL_IND */

#ifndef EXCLUDE_CSR_BT_HIDH_CONTROL_IND
CsrUint8 *CsrBtHidhControlIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHidhControlInd *primitive;

    primitive = (CsrBtHidhControlInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->operation);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDH_CONTROL_IND */

#ifndef EXCLUDE_CSR_BT_HIDH_CONTROL_IND
void *CsrBtHidhControlIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHidhControlInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHidhControlInd *) CsrPmemZalloc(sizeof(CsrBtHidhControlInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->operation, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDH_CONTROL_IND */

#ifndef EXCLUDE_CSR_BT_HIDH_REGISTER_USER_CFM
CsrSize CsrBtHidhRegisterUserCfmSizeof(void *msg)
{
    CsrBtHidhRegisterUserCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDH_REGISTER_USER_CFM */

#ifndef EXCLUDE_CSR_BT_HIDH_REGISTER_USER_CFM
CsrUint8 *CsrBtHidhRegisterUserCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHidhRegisterUserCfm *primitive;

    primitive = (CsrBtHidhRegisterUserCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->slotId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDH_REGISTER_USER_CFM */

#ifndef EXCLUDE_CSR_BT_HIDH_REGISTER_USER_CFM
void *CsrBtHidhRegisterUserCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHidhRegisterUserCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHidhRegisterUserCfm *) CsrPmemZalloc(sizeof(CsrBtHidhRegisterUserCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->slotId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDH_REGISTER_USER_CFM */

#ifndef EXCLUDE_CSR_BT_HIDH_SECURITY_OUT_CFM
CsrSize CsrBtHidhSecurityOutCfmSizeof(void *msg)
{
    CsrBtHidhSecurityOutCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDH_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_HIDH_SECURITY_OUT_CFM
CsrUint8 *CsrBtHidhSecurityOutCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHidhSecurityOutCfm *primitive;

    primitive = (CsrBtHidhSecurityOutCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDH_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_HIDH_SECURITY_OUT_CFM
void *CsrBtHidhSecurityOutCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHidhSecurityOutCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHidhSecurityOutCfm *) CsrPmemZalloc(sizeof(CsrBtHidhSecurityOutCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDH_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_HIDH_SET_IDLE_REQ
CsrSize CsrBtHidhSetIdleReqSizeof(void *msg)
{
    CsrBtHidhSetIdleReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDH_SET_IDLE_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_SET_IDLE_REQ
CsrUint8 *CsrBtHidhSetIdleReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHidhSetIdleReq *primitive;

    primitive = (CsrBtHidhSetIdleReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->idleRate);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDH_SET_IDLE_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_SET_IDLE_REQ
void *CsrBtHidhSetIdleReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHidhSetIdleReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHidhSetIdleReq *) CsrPmemZalloc(sizeof(CsrBtHidhSetIdleReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->idleRate, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDH_SET_IDLE_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_SET_PROTOCOL_REQ
CsrSize CsrBtHidhSetProtocolReqSizeof(void *msg)
{
    CsrBtHidhSetProtocolReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDH_SET_PROTOCOL_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_SET_PROTOCOL_REQ
CsrUint8 *CsrBtHidhSetProtocolReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHidhSetProtocolReq *primitive;

    primitive = (CsrBtHidhSetProtocolReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->protocol);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDH_SET_PROTOCOL_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_SET_PROTOCOL_REQ
void *CsrBtHidhSetProtocolReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHidhSetProtocolReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHidhSetProtocolReq *) CsrPmemZalloc(sizeof(CsrBtHidhSetProtocolReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->protocol, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDH_SET_PROTOCOL_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_CONNECT_REQ
CsrSize CsrBtHidhConnectReqSizeof(void *msg)
{
    CsrBtHidhConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->qosCtrlCount * sizeof(L2CA_QOS_T)) + (primitive->qosIntrCount * sizeof(L2CA_QOS_T)) + (primitive->sdpInfoCount * sizeof(CsrBtHidhSdpInfo)) + CsrCharStringSerLen((CsrCharString *) primitive->serviceName) + (primitive->descriptorLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDH_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_CONNECT_REQ
CsrUint8 *CsrBtHidhConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHidhConnectReq *primitive;

    primitive = (CsrBtHidhConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ctrlHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->slotId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flushTimeout);
    CsrMemCpySer(buffer, length, &primitive->qosCtrl, sizeof(primitive->qosCtrl));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->qosCtrlCount);
    CsrMemCpySer(buffer, length, &primitive->qosIntr, sizeof(primitive->qosIntr));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->qosIntrCount);
    CsrMemCpySer(buffer, length, &primitive->sdpInfo, sizeof(primitive->sdpInfo));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->sdpInfoCount);
    CsrMemCpySer(buffer, length, &primitive->serviceName, sizeof(primitive->serviceName));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->descriptorLength);
    CsrMemCpySer(buffer, length, &primitive->descriptor, sizeof(primitive->descriptor));
    if(primitive->qosCtrl)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->qosCtrl), ((CsrSize) (primitive->qosCtrlCount * sizeof(L2CA_QOS_T))));
    }
    if(primitive->qosIntr)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->qosIntr), ((CsrSize) (primitive->qosIntrCount * sizeof(L2CA_QOS_T))));
    }
    if(primitive->sdpInfo)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->sdpInfo), ((CsrSize) (primitive->sdpInfoCount * sizeof(CsrBtHidhSdpInfo))));
    }
    CsrCharStringSer(buffer, length, primitive->serviceName);
    if(primitive->descriptor)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->descriptor), ((CsrSize) (primitive->descriptorLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDH_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_CONNECT_REQ
void *CsrBtHidhConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHidhConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHidhConnectReq *) CsrPmemZalloc(sizeof(CsrBtHidhConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->ctrlHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->slotId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flushTimeout, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->qosCtrl), buffer, &offset, ((CsrSize) sizeof(primitive->qosCtrl)));
    CsrUint8Des((CsrUint8 *) &primitive->qosCtrlCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->qosIntr), buffer, &offset, ((CsrSize) sizeof(primitive->qosIntr)));
    CsrUint8Des((CsrUint8 *) &primitive->qosIntrCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->sdpInfo), buffer, &offset, ((CsrSize) sizeof(primitive->sdpInfo)));
    CsrUint8Des((CsrUint8 *) &primitive->sdpInfoCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->serviceName), buffer, &offset, ((CsrSize) sizeof(primitive->serviceName)));
    CsrUint16Des((CsrUint16 *) &primitive->descriptorLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->descriptor), buffer, &offset, ((CsrSize) sizeof(primitive->descriptor)));
    if (primitive->qosCtrlCount)
    {
        primitive->qosCtrl = CsrPmemZalloc(((CsrUint32) (primitive->qosCtrlCount * sizeof(L2CA_QOS_T))));
        CsrMemCpyDes(((void *) primitive->qosCtrl), buffer, &offset, ((CsrSize) (primitive->qosCtrlCount * sizeof(L2CA_QOS_T))));
    }
    else
    {
        primitive->qosCtrl = NULL;
    }
    if (primitive->qosIntrCount)
    {
        primitive->qosIntr = CsrPmemZalloc(((CsrUint32) (primitive->qosIntrCount * sizeof(L2CA_QOS_T))));
        CsrMemCpyDes(((void *) primitive->qosIntr), buffer, &offset, ((CsrSize) (primitive->qosIntrCount * sizeof(L2CA_QOS_T))));
    }
    else
    {
        primitive->qosIntr = NULL;
    }
    if (primitive->sdpInfoCount)
    {
        primitive->sdpInfo = CsrPmemZalloc(((CsrUint32) (primitive->sdpInfoCount * sizeof(CsrBtHidhSdpInfo))));
        CsrMemCpyDes(((void *) primitive->sdpInfo), buffer, &offset, ((CsrSize) (primitive->sdpInfoCount * sizeof(CsrBtHidhSdpInfo))));
    }
    else
    {
        primitive->sdpInfo = NULL;
    }
    CsrCharStringDes(&primitive->serviceName, buffer, &offset);
    if (primitive->descriptorLength)
    {
        primitive->descriptor = CsrPmemZalloc(((CsrUint32) (primitive->descriptorLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->descriptor), buffer, &offset, ((CsrSize) (primitive->descriptorLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->descriptor = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDH_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_CONNECT_REQ
void CsrBtHidhConnectReqSerFree(void *msg)
{
    CsrBtHidhConnectReq *primitive;

    primitive = (CsrBtHidhConnectReq *) msg;

    if(primitive->qosCtrl != NULL)
    {
        CsrPmemFree(primitive->qosCtrl);
    }
    if(primitive->qosIntr != NULL)
    {
        CsrPmemFree(primitive->qosIntr);
    }
    if(primitive->sdpInfo != NULL)
    {
        CsrPmemFree(primitive->sdpInfo);
    }
    if(primitive->serviceName != NULL)
    {
        CsrPmemFree(primitive->serviceName);
    }
    if(primitive->descriptor != NULL)
    {
        CsrPmemFree(primitive->descriptor);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HIDH_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_DATA_REQ
CsrSize CsrBtHidhDataReqSizeof(void *msg)
{
    CsrBtHidhDataReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->dataLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDH_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_DATA_REQ
CsrUint8 *CsrBtHidhDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHidhDataReq *primitive;

    primitive = (CsrBtHidhDataReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->reportType);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->dataLen);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    if(primitive->data)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->data), ((CsrSize) (primitive->dataLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDH_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_DATA_REQ
void *CsrBtHidhDataReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHidhDataReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHidhDataReq *) CsrPmemZalloc(sizeof(CsrBtHidhDataReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->reportType, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->dataLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
    if (primitive->dataLen)
    {
        primitive->data = CsrPmemZalloc(((CsrUint32) (primitive->dataLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->data), buffer, &offset, ((CsrSize) (primitive->dataLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->data = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDH_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_DATA_REQ
void CsrBtHidhDataReqSerFree(void *msg)
{
    CsrBtHidhDataReq *primitive;

    primitive = (CsrBtHidhDataReq *) msg;

    if(primitive->data != NULL)
    {
        CsrPmemFree(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HIDH_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_SET_REPORT_REQ
CsrSize CsrBtHidhSetReportReqSizeof(void *msg)
{
    CsrBtHidhSetReportReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->reportLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDH_SET_REPORT_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_SET_REPORT_REQ
CsrUint8 *CsrBtHidhSetReportReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHidhSetReportReq *primitive;

    primitive = (CsrBtHidhSetReportReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->reportType);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reportLen);
    CsrMemCpySer(buffer, length, &primitive->report, sizeof(primitive->report));
    if(primitive->report)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->report), ((CsrSize) (primitive->reportLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDH_SET_REPORT_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_SET_REPORT_REQ
void *CsrBtHidhSetReportReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHidhSetReportReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHidhSetReportReq *) CsrPmemZalloc(sizeof(CsrBtHidhSetReportReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->reportType, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reportLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->report), buffer, &offset, ((CsrSize) sizeof(primitive->report)));
    if (primitive->reportLen)
    {
        primitive->report = CsrPmemZalloc(((CsrUint32) (primitive->reportLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->report), buffer, &offset, ((CsrSize) (primitive->reportLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->report = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDH_SET_REPORT_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_SET_REPORT_REQ
void CsrBtHidhSetReportReqSerFree(void *msg)
{
    CsrBtHidhSetReportReq *primitive;

    primitive = (CsrBtHidhSetReportReq *) msg;

    if(primitive->report != NULL)
    {
        CsrPmemFree(primitive->report);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HIDH_SET_REPORT_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_SECURITY_IN_CFM
CsrSize CsrBtHidhSecurityInCfmSizeof(void *msg)
{
    CsrBtHidhSecurityInCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDH_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_HIDH_SECURITY_IN_CFM
CsrUint8 *CsrBtHidhSecurityInCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHidhSecurityInCfm *primitive;

    primitive = (CsrBtHidhSecurityInCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDH_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_HIDH_SECURITY_IN_CFM
void *CsrBtHidhSecurityInCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHidhSecurityInCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHidhSecurityInCfm *) CsrPmemZalloc(sizeof(CsrBtHidhSecurityInCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDH_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_HIDH_GET_REPORT_REQ
CsrSize CsrBtHidhGetReportReqSizeof(void *msg)
{
    CsrBtHidhGetReportReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDH_GET_REPORT_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_GET_REPORT_REQ
CsrUint8 *CsrBtHidhGetReportReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHidhGetReportReq *primitive;

    primitive = (CsrBtHidhGetReportReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->reportType);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->reportId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bufferSize);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDH_GET_REPORT_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_GET_REPORT_REQ
void *CsrBtHidhGetReportReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHidhGetReportReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHidhGetReportReq *) CsrPmemZalloc(sizeof(CsrBtHidhGetReportReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->reportType, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->reportId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bufferSize, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDH_GET_REPORT_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_DATA_IND
CsrSize CsrBtHidhDataIndSizeof(void *msg)
{
    CsrBtHidhDataInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->dataLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDH_DATA_IND */

#ifndef EXCLUDE_CSR_BT_HIDH_DATA_IND
CsrUint8 *CsrBtHidhDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHidhDataInd *primitive;

    primitive = (CsrBtHidhDataInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->reportType);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->dataLen);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    if(primitive->data)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->data), ((CsrSize) (primitive->dataLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDH_DATA_IND */

#ifndef EXCLUDE_CSR_BT_HIDH_DATA_IND
void *CsrBtHidhDataIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHidhDataInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHidhDataInd *) CsrPmemZalloc(sizeof(CsrBtHidhDataInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->reportType, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->dataLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
    if (primitive->dataLen)
    {
        primitive->data = CsrPmemZalloc(((CsrUint32) (primitive->dataLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->data), buffer, &offset, ((CsrSize) (primitive->dataLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->data = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDH_DATA_IND */

#ifndef EXCLUDE_CSR_BT_HIDH_DATA_IND
void CsrBtHidhDataIndSerFree(void *msg)
{
    CsrBtHidhDataInd *primitive;

    primitive = (CsrBtHidhDataInd *) msg;

    if(primitive->data != NULL)
    {
        CsrPmemFree(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HIDH_DATA_IND */

#ifndef EXCLUDE_CSR_BT_HIDH_SECURITY_OUT_REQ
CsrSize CsrBtHidhSecurityOutReqSizeof(void *msg)
{
    CsrBtHidhSecurityOutReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDH_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_SECURITY_OUT_REQ
CsrUint8 *CsrBtHidhSecurityOutReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHidhSecurityOutReq *primitive;

    primitive = (CsrBtHidhSecurityOutReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDH_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_SECURITY_OUT_REQ
void *CsrBtHidhSecurityOutReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHidhSecurityOutReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHidhSecurityOutReq *) CsrPmemZalloc(sizeof(CsrBtHidhSecurityOutReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDH_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_HIDH_CONNECT_ACCEPT_IND
CsrSize CsrBtHidhConnectAcceptIndSizeof(void *msg)
{
    CsrBtHidhConnectAcceptInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDH_CONNECT_ACCEPT_IND */

#ifndef EXCLUDE_CSR_BT_HIDH_CONNECT_ACCEPT_IND
CsrUint8 *CsrBtHidhConnectAcceptIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHidhConnectAcceptInd *primitive;

    primitive = (CsrBtHidhConnectAcceptInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDH_CONNECT_ACCEPT_IND */

#ifndef EXCLUDE_CSR_BT_HIDH_CONNECT_ACCEPT_IND
void *CsrBtHidhConnectAcceptIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHidhConnectAcceptInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHidhConnectAcceptInd *) CsrPmemZalloc(sizeof(CsrBtHidhConnectAcceptInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDH_CONNECT_ACCEPT_IND */

#endif /* EXCLUDE_CSR_BT_HIDH_MODULE */
