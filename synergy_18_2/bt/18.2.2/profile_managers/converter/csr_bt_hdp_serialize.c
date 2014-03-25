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
#include "csr_bt_hdp_prim.h"

#ifndef EXCLUDE_CSR_BT_HDP_MODULE

void CsrBtHdpPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_HDP_DATA_IND
CsrSize CsrBtHdpDataIndSizeof(void *msg)
{
    CsrBtHdpDataInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->length * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HDP_DATA_IND */

#ifndef EXCLUDE_CSR_BT_HDP_DATA_IND
CsrUint8 *CsrBtHdpDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHdpDataInd *primitive;

    primitive = (CsrBtHdpDataInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->assocChId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->length);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    if(primitive->data)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->data), ((CsrSize) (primitive->length * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HDP_DATA_IND */

#ifndef EXCLUDE_CSR_BT_HDP_DATA_IND
void *CsrBtHdpDataIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHdpDataInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHdpDataInd *) CsrPmemZalloc(sizeof(CsrBtHdpDataInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->assocChId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->length, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
    if (primitive->length)
    {
        primitive->data = CsrPmemZalloc(((CsrUint32) (primitive->length * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->data), buffer, &offset, ((CsrSize) (primitive->length * sizeof(CsrUint8))));
    }
    else
    {
        primitive->data = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HDP_DATA_IND */

#ifndef EXCLUDE_CSR_BT_HDP_DATA_IND
void CsrBtHdpDataIndSerFree(void *msg)
{
    CsrBtHdpDataInd *primitive;

    primitive = (CsrBtHdpDataInd *) msg;

    if(primitive->data != NULL)
    {
        CsrPmemFree(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HDP_DATA_IND */

#ifndef EXCLUDE_CSR_BT_HDP_DEACTIVATE_CFM
CsrSize CsrBtHdpDeactivateCfmSizeof(void *msg)
{
    CsrBtHdpDeactivateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HDP_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_HDP_DEACTIVATE_CFM
CsrUint8 *CsrBtHdpDeactivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHdpDeactivateCfm *primitive;

    primitive = (CsrBtHdpDeactivateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HDP_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_HDP_DEACTIVATE_CFM
void *CsrBtHdpDeactivateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHdpDeactivateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHdpDeactivateCfm *) CsrPmemZalloc(sizeof(CsrBtHdpDeactivateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HDP_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_HDP_SUSPEND_CFM
CsrSize CsrBtHdpSuspendCfmSizeof(void *msg)
{
    CsrBtHdpSuspendCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HDP_SUSPEND_CFM */

#ifndef EXCLUDE_CSR_BT_HDP_SUSPEND_CFM
CsrUint8 *CsrBtHdpSuspendCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHdpSuspendCfm *primitive;

    primitive = (CsrBtHdpSuspendCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->assocChId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HDP_SUSPEND_CFM */

#ifndef EXCLUDE_CSR_BT_HDP_SUSPEND_CFM
void *CsrBtHdpSuspendCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHdpSuspendCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHdpSuspendCfm *) CsrPmemZalloc(sizeof(CsrBtHdpSuspendCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->assocChId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HDP_SUSPEND_CFM */

#ifndef EXCLUDE_CSR_BT_HDP_ACCEPT_CONNECT_VIRTUAL_CHANNEL_RES
CsrSize CsrBtHdpAcceptConnectVirtualChannelResSizeof(void *msg)
{
    CsrBtHdpAcceptConnectVirtualChannelRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HDP_ACCEPT_CONNECT_VIRTUAL_CHANNEL_RES */

#ifndef EXCLUDE_CSR_BT_HDP_ACCEPT_CONNECT_VIRTUAL_CHANNEL_RES
CsrUint8 *CsrBtHdpAcceptConnectVirtualChannelResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHdpAcceptConnectVirtualChannelRes *primitive;

    primitive = (CsrBtHdpAcceptConnectVirtualChannelRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->virtualChannelId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxPacketLength);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->virtualChannelConfig);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HDP_ACCEPT_CONNECT_VIRTUAL_CHANNEL_RES */

#ifndef EXCLUDE_CSR_BT_HDP_ACCEPT_CONNECT_VIRTUAL_CHANNEL_RES
void *CsrBtHdpAcceptConnectVirtualChannelResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHdpAcceptConnectVirtualChannelRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHdpAcceptConnectVirtualChannelRes *) CsrPmemZalloc(sizeof(CsrBtHdpAcceptConnectVirtualChannelRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->virtualChannelId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxPacketLength, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->virtualChannelConfig, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HDP_ACCEPT_CONNECT_VIRTUAL_CHANNEL_RES */

#ifndef EXCLUDE_CSR_BT_HDP_CONNECT_VIRTUAL_CHANNEL_CFM
CsrSize CsrBtHdpConnectVirtualChannelCfmSizeof(void *msg)
{
    CsrBtHdpConnectVirtualChannelCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HDP_CONNECT_VIRTUAL_CHANNEL_CFM */

#ifndef EXCLUDE_CSR_BT_HDP_CONNECT_VIRTUAL_CHANNEL_CFM
CsrUint8 *CsrBtHdpConnectVirtualChannelCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHdpConnectVirtualChannelCfm *primitive;

    primitive = (CsrBtHdpConnectVirtualChannelCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->assocChannelId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->virtualChannelId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxPacketLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HDP_CONNECT_VIRTUAL_CHANNEL_CFM */

#ifndef EXCLUDE_CSR_BT_HDP_CONNECT_VIRTUAL_CHANNEL_CFM
void *CsrBtHdpConnectVirtualChannelCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHdpConnectVirtualChannelCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHdpConnectVirtualChannelCfm *) CsrPmemZalloc(sizeof(CsrBtHdpConnectVirtualChannelCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->assocChannelId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->virtualChannelId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxPacketLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HDP_CONNECT_VIRTUAL_CHANNEL_CFM */

#ifndef EXCLUDE_CSR_BT_HDP_RESUME_REQ
CsrSize CsrBtHdpResumeReqSizeof(void *msg)
{
    CsrBtHdpResumeReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HDP_RESUME_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_RESUME_REQ
CsrUint8 *CsrBtHdpResumeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHdpResumeReq *primitive;

    primitive = (CsrBtHdpResumeReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->assocChId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HDP_RESUME_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_RESUME_REQ
void *CsrBtHdpResumeReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHdpResumeReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHdpResumeReq *) CsrPmemZalloc(sizeof(CsrBtHdpResumeReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->assocChId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HDP_RESUME_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_RESUME_CFM
CsrSize CsrBtHdpResumeCfmSizeof(void *msg)
{
    CsrBtHdpResumeCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HDP_RESUME_CFM */

#ifndef EXCLUDE_CSR_BT_HDP_RESUME_CFM
CsrUint8 *CsrBtHdpResumeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHdpResumeCfm *primitive;

    primitive = (CsrBtHdpResumeCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->assocChId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HDP_RESUME_CFM */

#ifndef EXCLUDE_CSR_BT_HDP_RESUME_CFM
void *CsrBtHdpResumeCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHdpResumeCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHdpResumeCfm *) CsrPmemZalloc(sizeof(CsrBtHdpResumeCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->assocChId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HDP_RESUME_CFM */

#ifndef EXCLUDE_CSR_BT_HDP_DEACTIVATE_REQ
CsrSize CsrBtHdpDeactivateReqSizeof(void *msg)
{
    CsrBtHdpDeactivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HDP_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_DEACTIVATE_REQ
CsrUint8 *CsrBtHdpDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHdpDeactivateReq *primitive;

    primitive = (CsrBtHdpDeactivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HDP_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_DEACTIVATE_REQ
void *CsrBtHdpDeactivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHdpDeactivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHdpDeactivateReq *) CsrPmemZalloc(sizeof(CsrBtHdpDeactivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HDP_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_DISCONNECT_VIRTUAL_CHANNEL_REQ
CsrSize CsrBtHdpDisconnectVirtualChannelReqSizeof(void *msg)
{
    CsrBtHdpDisconnectVirtualChannelReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HDP_DISCONNECT_VIRTUAL_CHANNEL_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_DISCONNECT_VIRTUAL_CHANNEL_REQ
CsrUint8 *CsrBtHdpDisconnectVirtualChannelReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHdpDisconnectVirtualChannelReq *primitive;

    primitive = (CsrBtHdpDisconnectVirtualChannelReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->virtualChId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HDP_DISCONNECT_VIRTUAL_CHANNEL_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_DISCONNECT_VIRTUAL_CHANNEL_REQ
void *CsrBtHdpDisconnectVirtualChannelReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHdpDisconnectVirtualChannelReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHdpDisconnectVirtualChannelReq *) CsrPmemZalloc(sizeof(CsrBtHdpDisconnectVirtualChannelReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->virtualChId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HDP_DISCONNECT_VIRTUAL_CHANNEL_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_DISCONNECT_ASSOCIATE_CHANNEL_IND
CsrSize CsrBtHdpDisconnectAssociateChannelIndSizeof(void *msg)
{
    CsrBtHdpDisconnectAssociateChannelInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HDP_DISCONNECT_ASSOCIATE_CHANNEL_IND */

#ifndef EXCLUDE_CSR_BT_HDP_DISCONNECT_ASSOCIATE_CHANNEL_IND
CsrUint8 *CsrBtHdpDisconnectAssociateChannelIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHdpDisconnectAssociateChannelInd *primitive;

    primitive = (CsrBtHdpDisconnectAssociateChannelInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->assocChId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HDP_DISCONNECT_ASSOCIATE_CHANNEL_IND */

#ifndef EXCLUDE_CSR_BT_HDP_DISCONNECT_ASSOCIATE_CHANNEL_IND
void *CsrBtHdpDisconnectAssociateChannelIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHdpDisconnectAssociateChannelInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHdpDisconnectAssociateChannelInd *) CsrPmemZalloc(sizeof(CsrBtHdpDisconnectAssociateChannelInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->assocChId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HDP_DISCONNECT_ASSOCIATE_CHANNEL_IND */

#ifndef EXCLUDE_CSR_BT_HDP_REGISTER_IND
CsrSize CsrBtHdpRegisterIndSizeof(void *msg)
{
    CsrBtHdpRegisterInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HDP_REGISTER_IND */

#ifndef EXCLUDE_CSR_BT_HDP_REGISTER_IND
CsrUint8 *CsrBtHdpRegisterIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHdpRegisterInd *primitive;

    primitive = (CsrBtHdpRegisterInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mdepId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HDP_REGISTER_IND */

#ifndef EXCLUDE_CSR_BT_HDP_REGISTER_IND
void *CsrBtHdpRegisterIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHdpRegisterInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHdpRegisterInd *) CsrPmemZalloc(sizeof(CsrBtHdpRegisterInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mdepId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HDP_REGISTER_IND */

#ifndef EXCLUDE_CSR_BT_HDP_CONNECT_ASSOCIATE_CHANNEL_REQ
CsrSize CsrBtHdpConnectAssociateChannelReqSizeof(void *msg)
{
    CsrBtHdpConnectAssociateChannelReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HDP_CONNECT_ASSOCIATE_CHANNEL_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_CONNECT_ASSOCIATE_CHANNEL_REQ
CsrUint8 *CsrBtHdpConnectAssociateChannelReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHdpConnectAssociateChannelReq *primitive;

    primitive = (CsrBtHdpConnectAssociateChannelReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->hdpInstanceId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mdepId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mdepDataType);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mdepRole);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxPacketLength);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HDP_CONNECT_ASSOCIATE_CHANNEL_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_CONNECT_ASSOCIATE_CHANNEL_REQ
void *CsrBtHdpConnectAssociateChannelReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHdpConnectAssociateChannelReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHdpConnectAssociateChannelReq *) CsrPmemZalloc(sizeof(CsrBtHdpConnectAssociateChannelReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->hdpInstanceId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mdepId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mdepDataType, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mdepRole, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxPacketLength, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HDP_CONNECT_ASSOCIATE_CHANNEL_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_CTRL_GET_CAPAB_CFM
CsrSize CsrBtHdpCtrlGetCapabCfmSizeof(void *msg)
{
    CsrBtHdpCtrlGetCapabCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HDP_CTRL_GET_CAPAB_CFM */

#ifndef EXCLUDE_CSR_BT_HDP_CTRL_GET_CAPAB_CFM
CsrUint8 *CsrBtHdpCtrlGetCapabCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHdpCtrlGetCapabCfm *primitive;

    primitive = (CsrBtHdpCtrlGetCapabCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HDP_CTRL_GET_CAPAB_CFM */

#ifndef EXCLUDE_CSR_BT_HDP_CTRL_GET_CAPAB_CFM
void *CsrBtHdpCtrlGetCapabCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHdpCtrlGetCapabCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHdpCtrlGetCapabCfm *) CsrPmemZalloc(sizeof(CsrBtHdpCtrlGetCapabCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HDP_CTRL_GET_CAPAB_CFM */

#ifndef EXCLUDE_CSR_BT_HDP_SECURITY_IN_REQ
CsrSize CsrBtHdpSecurityInReqSizeof(void *msg)
{
    CsrBtHdpSecurityInReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HDP_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_SECURITY_IN_REQ
CsrUint8 *CsrBtHdpSecurityInReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHdpSecurityInReq *primitive;

    primitive = (CsrBtHdpSecurityInReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HDP_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_SECURITY_IN_REQ
void *CsrBtHdpSecurityInReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHdpSecurityInReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHdpSecurityInReq *) CsrPmemZalloc(sizeof(CsrBtHdpSecurityInReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HDP_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_SECURITY_OUT_REQ
CsrSize CsrBtHdpSecurityOutReqSizeof(void *msg)
{
    CsrBtHdpSecurityOutReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HDP_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_SECURITY_OUT_REQ
CsrUint8 *CsrBtHdpSecurityOutReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHdpSecurityOutReq *primitive;

    primitive = (CsrBtHdpSecurityOutReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HDP_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_SECURITY_OUT_REQ
void *CsrBtHdpSecurityOutReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHdpSecurityOutReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHdpSecurityOutReq *) CsrPmemZalloc(sizeof(CsrBtHdpSecurityOutReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HDP_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_DATA_REQ
CsrSize CsrBtHdpDataReqSizeof(void *msg)
{
    CsrBtHdpDataReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->dataLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HDP_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_DATA_REQ
CsrUint8 *CsrBtHdpDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHdpDataReq *primitive;

    primitive = (CsrBtHdpDataReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->chId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->dataLen);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    if(primitive->data)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->data), ((CsrSize) (primitive->dataLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HDP_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_DATA_REQ
void *CsrBtHdpDataReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHdpDataReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHdpDataReq *) CsrPmemZalloc(sizeof(CsrBtHdpDataReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->chId, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_HDP_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_DATA_REQ
void CsrBtHdpDataReqSerFree(void *msg)
{
    CsrBtHdpDataReq *primitive;

    primitive = (CsrBtHdpDataReq *) msg;

    if(primitive->data != NULL)
    {
        CsrPmemFree(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HDP_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_REGISTER_RSP
CsrSize CsrBtHdpRegisterRspSizeof(void *msg)
{
    CsrBtHdpRegisterRsp *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->description));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HDP_REGISTER_RSP */

#ifndef EXCLUDE_CSR_BT_HDP_REGISTER_RSP
CsrUint8 *CsrBtHdpRegisterRspSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHdpRegisterRsp *primitive;

    primitive = (CsrBtHdpRegisterRsp *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->datatype);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->role);
    CsrMemCpySer(buffer, length, &primitive->description, sizeof(primitive->description));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->reuseMdepId);
    CsrUtf8StringSer(buffer, length, primitive->description);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HDP_REGISTER_RSP */

#ifndef EXCLUDE_CSR_BT_HDP_REGISTER_RSP
void *CsrBtHdpRegisterRspDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHdpRegisterRsp *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHdpRegisterRsp *) CsrPmemZalloc(sizeof(CsrBtHdpRegisterRsp));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->datatype, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->role, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->description), buffer, &offset, ((CsrSize) sizeof(primitive->description)));
    CsrUint8Des((CsrUint8 *) &primitive->reuseMdepId, buffer, &offset);
    CsrUtf8StringDes(&primitive->description, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HDP_REGISTER_RSP */

#ifndef EXCLUDE_CSR_BT_HDP_REGISTER_RSP
void CsrBtHdpRegisterRspSerFree(void *msg)
{
    CsrBtHdpRegisterRsp *primitive;

    primitive = (CsrBtHdpRegisterRsp *) msg;

    if(primitive->description != NULL)
    {
        CsrPmemFree(primitive->description);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HDP_REGISTER_RSP */

#ifndef EXCLUDE_CSR_BT_HDP_RESUME_RSP
CsrSize CsrBtHdpResumeRspSizeof(void *msg)
{
    CsrBtHdpResumeRsp *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HDP_RESUME_RSP */

#ifndef EXCLUDE_CSR_BT_HDP_RESUME_RSP
CsrUint8 *CsrBtHdpResumeRspSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHdpResumeRsp *primitive;

    primitive = (CsrBtHdpResumeRsp *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->chId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mtu);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->resume);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HDP_RESUME_RSP */

#ifndef EXCLUDE_CSR_BT_HDP_RESUME_RSP
void *CsrBtHdpResumeRspDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHdpResumeRsp *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHdpResumeRsp *) CsrPmemZalloc(sizeof(CsrBtHdpResumeRsp));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->chId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mtu, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->resume, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HDP_RESUME_RSP */

#ifndef EXCLUDE_CSR_BT_HDP_RESUME_IND
CsrSize CsrBtHdpResumeIndSizeof(void *msg)
{
    CsrBtHdpResumeInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HDP_RESUME_IND */

#ifndef EXCLUDE_CSR_BT_HDP_RESUME_IND
CsrUint8 *CsrBtHdpResumeIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHdpResumeInd *primitive;

    primitive = (CsrBtHdpResumeInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->chId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HDP_RESUME_IND */

#ifndef EXCLUDE_CSR_BT_HDP_RESUME_IND
void *CsrBtHdpResumeIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHdpResumeInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHdpResumeInd *) CsrPmemZalloc(sizeof(CsrBtHdpResumeInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->chId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HDP_RESUME_IND */

#ifndef EXCLUDE_CSR_BT_HDP_DATA_CFM
CsrSize CsrBtHdpDataCfmSizeof(void *msg)
{
    CsrBtHdpDataCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HDP_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_HDP_DATA_CFM
CsrUint8 *CsrBtHdpDataCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHdpDataCfm *primitive;

    primitive = (CsrBtHdpDataCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->assocChId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HDP_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_HDP_DATA_CFM
void *CsrBtHdpDataCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHdpDataCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHdpDataCfm *) CsrPmemZalloc(sizeof(CsrBtHdpDataCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->assocChId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HDP_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_HDP_UNREGISTER_REQ
CsrSize CsrBtHdpUnregisterReqSizeof(void *msg)
{
    CsrBtHdpUnregisterReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HDP_UNREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_UNREGISTER_REQ
CsrUint8 *CsrBtHdpUnregisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHdpUnregisterReq *primitive;

    primitive = (CsrBtHdpUnregisterReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HDP_UNREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_UNREGISTER_REQ
void *CsrBtHdpUnregisterReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHdpUnregisterReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHdpUnregisterReq *) CsrPmemZalloc(sizeof(CsrBtHdpUnregisterReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HDP_UNREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_ACCEPT_CONNECT_VIRTUAL_CHANNEL_IND
CsrSize CsrBtHdpAcceptConnectVirtualChannelIndSizeof(void *msg)
{
    CsrBtHdpAcceptConnectVirtualChannelInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HDP_ACCEPT_CONNECT_VIRTUAL_CHANNEL_IND */

#ifndef EXCLUDE_CSR_BT_HDP_ACCEPT_CONNECT_VIRTUAL_CHANNEL_IND
CsrUint8 *CsrBtHdpAcceptConnectVirtualChannelIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHdpAcceptConnectVirtualChannelInd *primitive;

    primitive = (CsrBtHdpAcceptConnectVirtualChannelInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->assocChannelId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->virtualChannelId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->virtualChannelConfig);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HDP_ACCEPT_CONNECT_VIRTUAL_CHANNEL_IND */

#ifndef EXCLUDE_CSR_BT_HDP_ACCEPT_CONNECT_VIRTUAL_CHANNEL_IND
void *CsrBtHdpAcceptConnectVirtualChannelIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHdpAcceptConnectVirtualChannelInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHdpAcceptConnectVirtualChannelInd *) CsrPmemZalloc(sizeof(CsrBtHdpAcceptConnectVirtualChannelInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->assocChannelId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->virtualChannelId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->virtualChannelConfig, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HDP_ACCEPT_CONNECT_VIRTUAL_CHANNEL_IND */

#ifndef EXCLUDE_CSR_BT_HDP_SUSPEND_REQ
CsrSize CsrBtHdpSuspendReqSizeof(void *msg)
{
    CsrBtHdpSuspendReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HDP_SUSPEND_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_SUSPEND_REQ
CsrUint8 *CsrBtHdpSuspendReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHdpSuspendReq *primitive;

    primitive = (CsrBtHdpSuspendReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->assocChId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HDP_SUSPEND_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_SUSPEND_REQ
void *CsrBtHdpSuspendReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHdpSuspendReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHdpSuspendReq *) CsrPmemZalloc(sizeof(CsrBtHdpSuspendReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->assocChId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HDP_SUSPEND_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_ACCEPT_CONNECT_ASSOCIATE_CHANNEL_IND
CsrSize CsrBtHdpAcceptConnectAssociateChannelIndSizeof(void *msg)
{
    CsrBtHdpAcceptConnectAssociateChannelInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HDP_ACCEPT_CONNECT_ASSOCIATE_CHANNEL_IND */

#ifndef EXCLUDE_CSR_BT_HDP_ACCEPT_CONNECT_ASSOCIATE_CHANNEL_IND
CsrUint8 *CsrBtHdpAcceptConnectAssociateChannelIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHdpAcceptConnectAssociateChannelInd *primitive;

    primitive = (CsrBtHdpAcceptConnectAssociateChannelInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mdepId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->assocChannelId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HDP_ACCEPT_CONNECT_ASSOCIATE_CHANNEL_IND */

#ifndef EXCLUDE_CSR_BT_HDP_ACCEPT_CONNECT_ASSOCIATE_CHANNEL_IND
void *CsrBtHdpAcceptConnectAssociateChannelIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHdpAcceptConnectAssociateChannelInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHdpAcceptConnectAssociateChannelInd *) CsrPmemZalloc(sizeof(CsrBtHdpAcceptConnectAssociateChannelInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mdepId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->assocChannelId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HDP_ACCEPT_CONNECT_ASSOCIATE_CHANNEL_IND */

#ifndef EXCLUDE_CSR_BT_HDP_REGISTER_CFM
CsrSize CsrBtHdpRegisterCfmSizeof(void *msg)
{
    CsrBtHdpRegisterCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HDP_REGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_HDP_REGISTER_CFM
CsrUint8 *CsrBtHdpRegisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHdpRegisterCfm *primitive;

    primitive = (CsrBtHdpRegisterCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HDP_REGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_HDP_REGISTER_CFM
void *CsrBtHdpRegisterCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHdpRegisterCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHdpRegisterCfm *) CsrPmemZalloc(sizeof(CsrBtHdpRegisterCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HDP_REGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_HDP_DISCONNECT_VIRTUAL_CHANNEL_CFM
CsrSize CsrBtHdpDisconnectVirtualChannelCfmSizeof(void *msg)
{
    CsrBtHdpDisconnectVirtualChannelCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HDP_DISCONNECT_VIRTUAL_CHANNEL_CFM */

#ifndef EXCLUDE_CSR_BT_HDP_DISCONNECT_VIRTUAL_CHANNEL_CFM
CsrUint8 *CsrBtHdpDisconnectVirtualChannelCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHdpDisconnectVirtualChannelCfm *primitive;

    primitive = (CsrBtHdpDisconnectVirtualChannelCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->virtualChId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HDP_DISCONNECT_VIRTUAL_CHANNEL_CFM */

#ifndef EXCLUDE_CSR_BT_HDP_DISCONNECT_VIRTUAL_CHANNEL_CFM
void *CsrBtHdpDisconnectVirtualChannelCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHdpDisconnectVirtualChannelCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHdpDisconnectVirtualChannelCfm *) CsrPmemZalloc(sizeof(CsrBtHdpDisconnectVirtualChannelCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->virtualChId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HDP_DISCONNECT_VIRTUAL_CHANNEL_CFM */

#ifndef EXCLUDE_CSR_BT_HDP_CONNECT_ASSOCIATE_CHANNEL_IND
CsrSize CsrBtHdpConnectAssociateChannelIndSizeof(void *msg)
{
    CsrBtHdpConnectAssociateChannelInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HDP_CONNECT_ASSOCIATE_CHANNEL_IND */

#ifndef EXCLUDE_CSR_BT_HDP_CONNECT_ASSOCIATE_CHANNEL_IND
CsrUint8 *CsrBtHdpConnectAssociateChannelIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHdpConnectAssociateChannelInd *primitive;

    primitive = (CsrBtHdpConnectAssociateChannelInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->hdpInstanceId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mdepId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->dataType);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->assocChannelId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxPacketLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HDP_CONNECT_ASSOCIATE_CHANNEL_IND */

#ifndef EXCLUDE_CSR_BT_HDP_CONNECT_ASSOCIATE_CHANNEL_IND
void *CsrBtHdpConnectAssociateChannelIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHdpConnectAssociateChannelInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHdpConnectAssociateChannelInd *) CsrPmemZalloc(sizeof(CsrBtHdpConnectAssociateChannelInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->hdpInstanceId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mdepId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->dataType, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->assocChannelId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxPacketLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HDP_CONNECT_ASSOCIATE_CHANNEL_IND */

#ifndef EXCLUDE_CSR_BT_HDP_CTRL_GET_CAPAB_IND
CsrSize CsrBtHdpCtrlGetCapabIndSizeof(void *msg)
{
    CsrBtHdpCtrlGetCapabInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->serviceName) + CsrCharStringSerLen((CsrCharString *) primitive->serviceDescrip) + CsrCharStringSerLen((CsrCharString *) primitive->providerName) + (primitive->supportedFeatureListLen * sizeof(CsrBtHdpMdep)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HDP_CTRL_GET_CAPAB_IND */

#ifndef EXCLUDE_CSR_BT_HDP_CTRL_GET_CAPAB_IND
CsrUint8 *CsrBtHdpCtrlGetCapabIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHdpCtrlGetCapabInd *primitive;

    primitive = (CsrBtHdpCtrlGetCapabInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->hdpInstanceId);
    CsrMemCpySer(buffer, length, &primitive->serviceName, sizeof(primitive->serviceName));
    CsrMemCpySer(buffer, length, &primitive->serviceDescrip, sizeof(primitive->serviceDescrip));
    CsrMemCpySer(buffer, length, &primitive->providerName, sizeof(primitive->providerName));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->dataexchangeSpecification);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->supportedProcedures);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->supportedFeatureListLen);
    CsrMemCpySer(buffer, length, &primitive->supportedFeatureList, sizeof(primitive->supportedFeatureList));
    CsrCharStringSer(buffer, length, primitive->serviceName);
    CsrCharStringSer(buffer, length, primitive->serviceDescrip);
    CsrCharStringSer(buffer, length, primitive->providerName);
    if(primitive->supportedFeatureList)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->supportedFeatureList), ((CsrSize) (primitive->supportedFeatureListLen * sizeof(CsrBtHdpMdep))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HDP_CTRL_GET_CAPAB_IND */

#ifndef EXCLUDE_CSR_BT_HDP_CTRL_GET_CAPAB_IND
void *CsrBtHdpCtrlGetCapabIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHdpCtrlGetCapabInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHdpCtrlGetCapabInd *) CsrPmemZalloc(sizeof(CsrBtHdpCtrlGetCapabInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->hdpInstanceId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->serviceName), buffer, &offset, ((CsrSize) sizeof(primitive->serviceName)));
    CsrMemCpyDes(((void *) &primitive->serviceDescrip), buffer, &offset, ((CsrSize) sizeof(primitive->serviceDescrip)));
    CsrMemCpyDes(((void *) &primitive->providerName), buffer, &offset, ((CsrSize) sizeof(primitive->providerName)));
    CsrUint8Des((CsrUint8 *) &primitive->dataexchangeSpecification, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->supportedProcedures, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->supportedFeatureListLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->supportedFeatureList), buffer, &offset, ((CsrSize) sizeof(primitive->supportedFeatureList)));
    CsrCharStringDes(&primitive->serviceName, buffer, &offset);
    CsrCharStringDes(&primitive->serviceDescrip, buffer, &offset);
    CsrCharStringDes(&primitive->providerName, buffer, &offset);
    if (primitive->supportedFeatureListLen)
    {
        primitive->supportedFeatureList = CsrPmemZalloc(((CsrUint32) (primitive->supportedFeatureListLen * sizeof(CsrBtHdpMdep))));
        CsrMemCpyDes(((void *) primitive->supportedFeatureList), buffer, &offset, ((CsrSize) (primitive->supportedFeatureListLen * sizeof(CsrBtHdpMdep))));
    }
    else
    {
        primitive->supportedFeatureList = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HDP_CTRL_GET_CAPAB_IND */

#ifndef EXCLUDE_CSR_BT_HDP_CTRL_GET_CAPAB_IND
void CsrBtHdpCtrlGetCapabIndSerFree(void *msg)
{
    CsrBtHdpCtrlGetCapabInd *primitive;

    primitive = (CsrBtHdpCtrlGetCapabInd *) msg;

    if(primitive->serviceName != NULL)
    {
        CsrPmemFree(primitive->serviceName);
    }
    if(primitive->serviceDescrip != NULL)
    {
        CsrPmemFree(primitive->serviceDescrip);
    }
    if(primitive->providerName != NULL)
    {
        CsrPmemFree(primitive->providerName);
    }
    if(primitive->supportedFeatureList != NULL)
    {
        CsrPmemFree(primitive->supportedFeatureList);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HDP_CTRL_GET_CAPAB_IND */

#ifndef EXCLUDE_CSR_BT_HDP_CONNECT_ASSOCIATE_CHANNEL_CFM
CsrSize CsrBtHdpConnectAssociateChannelCfmSizeof(void *msg)
{
    CsrBtHdpConnectAssociateChannelCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HDP_CONNECT_ASSOCIATE_CHANNEL_CFM */

#ifndef EXCLUDE_CSR_BT_HDP_CONNECT_ASSOCIATE_CHANNEL_CFM
CsrUint8 *CsrBtHdpConnectAssociateChannelCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHdpConnectAssociateChannelCfm *primitive;

    primitive = (CsrBtHdpConnectAssociateChannelCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->hdpInstanceId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mdepId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->dataType);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->assocChannelId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxPacketLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HDP_CONNECT_ASSOCIATE_CHANNEL_CFM */

#ifndef EXCLUDE_CSR_BT_HDP_CONNECT_ASSOCIATE_CHANNEL_CFM
void *CsrBtHdpConnectAssociateChannelCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHdpConnectAssociateChannelCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHdpConnectAssociateChannelCfm *) CsrPmemZalloc(sizeof(CsrBtHdpConnectAssociateChannelCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->hdpInstanceId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mdepId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->dataType, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->assocChannelId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxPacketLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HDP_CONNECT_ASSOCIATE_CHANNEL_CFM */

#ifndef EXCLUDE_CSR_BT_HDP_DISCONNECT_ASSOCIATE_CHANNEL_REQ
CsrSize CsrBtHdpDisconnectAssociateChannelReqSizeof(void *msg)
{
    CsrBtHdpDisconnectAssociateChannelReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HDP_DISCONNECT_ASSOCIATE_CHANNEL_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_DISCONNECT_ASSOCIATE_CHANNEL_REQ
CsrUint8 *CsrBtHdpDisconnectAssociateChannelReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHdpDisconnectAssociateChannelReq *primitive;

    primitive = (CsrBtHdpDisconnectAssociateChannelReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->assocChId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HDP_DISCONNECT_ASSOCIATE_CHANNEL_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_DISCONNECT_ASSOCIATE_CHANNEL_REQ
void *CsrBtHdpDisconnectAssociateChannelReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHdpDisconnectAssociateChannelReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHdpDisconnectAssociateChannelReq *) CsrPmemZalloc(sizeof(CsrBtHdpDisconnectAssociateChannelReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->assocChId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HDP_DISCONNECT_ASSOCIATE_CHANNEL_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_CONNECT_VIRTUAL_CHANNEL_IND
CsrSize CsrBtHdpConnectVirtualChannelIndSizeof(void *msg)
{
    CsrBtHdpConnectVirtualChannelInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HDP_CONNECT_VIRTUAL_CHANNEL_IND */

#ifndef EXCLUDE_CSR_BT_HDP_CONNECT_VIRTUAL_CHANNEL_IND
CsrUint8 *CsrBtHdpConnectVirtualChannelIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHdpConnectVirtualChannelInd *primitive;

    primitive = (CsrBtHdpConnectVirtualChannelInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->assocChannelId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->virtualChannelId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxPacketLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HDP_CONNECT_VIRTUAL_CHANNEL_IND */

#ifndef EXCLUDE_CSR_BT_HDP_CONNECT_VIRTUAL_CHANNEL_IND
void *CsrBtHdpConnectVirtualChannelIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHdpConnectVirtualChannelInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHdpConnectVirtualChannelInd *) CsrPmemZalloc(sizeof(CsrBtHdpConnectVirtualChannelInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->assocChannelId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->virtualChannelId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxPacketLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HDP_CONNECT_VIRTUAL_CHANNEL_IND */

#ifndef EXCLUDE_CSR_BT_HDP_ACTIVATE_CFM
CsrSize CsrBtHdpActivateCfmSizeof(void *msg)
{
    CsrBtHdpActivateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HDP_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_HDP_ACTIVATE_CFM
CsrUint8 *CsrBtHdpActivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHdpActivateCfm *primitive;

    primitive = (CsrBtHdpActivateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HDP_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_HDP_ACTIVATE_CFM
void *CsrBtHdpActivateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHdpActivateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHdpActivateCfm *) CsrPmemZalloc(sizeof(CsrBtHdpActivateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HDP_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_HDP_ACTIVATE_REQ
CsrSize CsrBtHdpActivateReqSizeof(void *msg)
{
    CsrBtHdpActivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HDP_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_ACTIVATE_REQ
CsrUint8 *CsrBtHdpActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHdpActivateReq *primitive;

    primitive = (CsrBtHdpActivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->noOfConnections);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HDP_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_ACTIVATE_REQ
void *CsrBtHdpActivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHdpActivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHdpActivateReq *) CsrPmemZalloc(sizeof(CsrBtHdpActivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->noOfConnections, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HDP_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_REGISTER_REQ
CsrSize CsrBtHdpRegisterReqSizeof(void *msg)
{
    CsrBtHdpRegisterReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->serviceName) + CsrCharStringSerLen((CsrCharString *) primitive->serviceDescription) + CsrCharStringSerLen((CsrCharString *) primitive->providerName));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HDP_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_REGISTER_REQ
CsrUint8 *CsrBtHdpRegisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHdpRegisterReq *primitive;

    primitive = (CsrBtHdpRegisterReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);
    CsrMemCpySer(buffer, length, &primitive->serviceName, sizeof(primitive->serviceName));
    CsrMemCpySer(buffer, length, &primitive->serviceDescription, sizeof(primitive->serviceDescription));
    CsrMemCpySer(buffer, length, &primitive->providerName, sizeof(primitive->providerName));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->dataExchangeSpecification);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->sniffTimeOut);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->numOfMdep);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->supportedProcedures);
    CsrCharStringSer(buffer, length, primitive->serviceName);
    CsrCharStringSer(buffer, length, primitive->serviceDescription);
    CsrCharStringSer(buffer, length, primitive->providerName);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HDP_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_REGISTER_REQ
void *CsrBtHdpRegisterReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHdpRegisterReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHdpRegisterReq *) CsrPmemZalloc(sizeof(CsrBtHdpRegisterReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->serviceName), buffer, &offset, ((CsrSize) sizeof(primitive->serviceName)));
    CsrMemCpyDes(((void *) &primitive->serviceDescription), buffer, &offset, ((CsrSize) sizeof(primitive->serviceDescription)));
    CsrMemCpyDes(((void *) &primitive->providerName), buffer, &offset, ((CsrSize) sizeof(primitive->providerName)));
    CsrUint8Des((CsrUint8 *) &primitive->dataExchangeSpecification, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->sniffTimeOut, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->numOfMdep, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->supportedProcedures, buffer, &offset);
    CsrCharStringDes(&primitive->serviceName, buffer, &offset);
    CsrCharStringDes(&primitive->serviceDescription, buffer, &offset);
    CsrCharStringDes(&primitive->providerName, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HDP_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_REGISTER_REQ
void CsrBtHdpRegisterReqSerFree(void *msg)
{
    CsrBtHdpRegisterReq *primitive;

    primitive = (CsrBtHdpRegisterReq *) msg;

    if(primitive->serviceName != NULL)
    {
        CsrPmemFree(primitive->serviceName);
    }
    if(primitive->serviceDescription != NULL)
    {
        CsrPmemFree(primitive->serviceDescription);
    }
    if(primitive->providerName != NULL)
    {
        CsrPmemFree(primitive->providerName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HDP_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_CTRL_GET_CAPAB_REQ
CsrSize CsrBtHdpCtrlGetCapabReqSizeof(void *msg)
{
    CsrBtHdpCtrlGetCapabReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HDP_CTRL_GET_CAPAB_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_CTRL_GET_CAPAB_REQ
CsrUint8 *CsrBtHdpCtrlGetCapabReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHdpCtrlGetCapabReq *primitive;

    primitive = (CsrBtHdpCtrlGetCapabReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mdepDataTypeMask);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->mdepDataTypeConditionMask);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mdepRoleMask);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HDP_CTRL_GET_CAPAB_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_CTRL_GET_CAPAB_REQ
void *CsrBtHdpCtrlGetCapabReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHdpCtrlGetCapabReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHdpCtrlGetCapabReq *) CsrPmemZalloc(sizeof(CsrBtHdpCtrlGetCapabReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mdepDataTypeMask, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->mdepDataTypeConditionMask, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mdepRoleMask, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HDP_CTRL_GET_CAPAB_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_CONNECT_VIRTUAL_CHANNEL_REQ
CsrSize CsrBtHdpConnectVirtualChannelReqSizeof(void *msg)
{
    CsrBtHdpConnectVirtualChannelReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HDP_CONNECT_VIRTUAL_CHANNEL_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_CONNECT_VIRTUAL_CHANNEL_REQ
CsrUint8 *CsrBtHdpConnectVirtualChannelReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHdpConnectVirtualChannelReq *primitive;

    primitive = (CsrBtHdpConnectVirtualChannelReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->assocChannelId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->virtualChannelConfig);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxPacketLength);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HDP_CONNECT_VIRTUAL_CHANNEL_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_CONNECT_VIRTUAL_CHANNEL_REQ
void *CsrBtHdpConnectVirtualChannelReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHdpConnectVirtualChannelReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHdpConnectVirtualChannelReq *) CsrPmemZalloc(sizeof(CsrBtHdpConnectVirtualChannelReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->assocChannelId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->virtualChannelConfig, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxPacketLength, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HDP_CONNECT_VIRTUAL_CHANNEL_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_UNREGISTER_CFM
CsrSize CsrBtHdpUnregisterCfmSizeof(void *msg)
{
    CsrBtHdpUnregisterCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HDP_UNREGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_HDP_UNREGISTER_CFM
CsrUint8 *CsrBtHdpUnregisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHdpUnregisterCfm *primitive;

    primitive = (CsrBtHdpUnregisterCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HDP_UNREGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_HDP_UNREGISTER_CFM
void *CsrBtHdpUnregisterCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHdpUnregisterCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHdpUnregisterCfm *) CsrPmemZalloc(sizeof(CsrBtHdpUnregisterCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HDP_UNREGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_HDP_ACCEPT_CONNECT_ASSOCIATE_CHANNEL_RES
CsrSize CsrBtHdpAcceptConnectAssociateChannelResSizeof(void *msg)
{
    CsrBtHdpAcceptConnectAssociateChannelRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HDP_ACCEPT_CONNECT_ASSOCIATE_CHANNEL_RES */

#ifndef EXCLUDE_CSR_BT_HDP_ACCEPT_CONNECT_ASSOCIATE_CHANNEL_RES
CsrUint8 *CsrBtHdpAcceptConnectAssociateChannelResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHdpAcceptConnectAssociateChannelRes *primitive;

    primitive = (CsrBtHdpAcceptConnectAssociateChannelRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mtu);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->assocChannelId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HDP_ACCEPT_CONNECT_ASSOCIATE_CHANNEL_RES */

#ifndef EXCLUDE_CSR_BT_HDP_ACCEPT_CONNECT_ASSOCIATE_CHANNEL_RES
void *CsrBtHdpAcceptConnectAssociateChannelResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHdpAcceptConnectAssociateChannelRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHdpAcceptConnectAssociateChannelRes *) CsrPmemZalloc(sizeof(CsrBtHdpAcceptConnectAssociateChannelRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mtu, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->assocChannelId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HDP_ACCEPT_CONNECT_ASSOCIATE_CHANNEL_RES */

#ifndef EXCLUDE_CSR_BT_HDP_DISCONNECT_VIRTUAL_CHANNEL_IND
CsrSize CsrBtHdpDisconnectVirtualChannelIndSizeof(void *msg)
{
    CsrBtHdpDisconnectVirtualChannelInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HDP_DISCONNECT_VIRTUAL_CHANNEL_IND */

#ifndef EXCLUDE_CSR_BT_HDP_DISCONNECT_VIRTUAL_CHANNEL_IND
CsrUint8 *CsrBtHdpDisconnectVirtualChannelIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHdpDisconnectVirtualChannelInd *primitive;

    primitive = (CsrBtHdpDisconnectVirtualChannelInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->virtualChId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HDP_DISCONNECT_VIRTUAL_CHANNEL_IND */

#ifndef EXCLUDE_CSR_BT_HDP_DISCONNECT_VIRTUAL_CHANNEL_IND
void *CsrBtHdpDisconnectVirtualChannelIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHdpDisconnectVirtualChannelInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHdpDisconnectVirtualChannelInd *) CsrPmemZalloc(sizeof(CsrBtHdpDisconnectVirtualChannelInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->virtualChId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HDP_DISCONNECT_VIRTUAL_CHANNEL_IND */

#ifndef EXCLUDE_CSR_BT_HDP_DISCONNECT_ASSOCIATE_CHANNEL_CFM
CsrSize CsrBtHdpDisconnectAssociateChannelCfmSizeof(void *msg)
{
    CsrBtHdpDisconnectAssociateChannelCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HDP_DISCONNECT_ASSOCIATE_CHANNEL_CFM */

#ifndef EXCLUDE_CSR_BT_HDP_DISCONNECT_ASSOCIATE_CHANNEL_CFM
CsrUint8 *CsrBtHdpDisconnectAssociateChannelCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHdpDisconnectAssociateChannelCfm *primitive;

    primitive = (CsrBtHdpDisconnectAssociateChannelCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->assocChId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HDP_DISCONNECT_ASSOCIATE_CHANNEL_CFM */

#ifndef EXCLUDE_CSR_BT_HDP_DISCONNECT_ASSOCIATE_CHANNEL_CFM
void *CsrBtHdpDisconnectAssociateChannelCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHdpDisconnectAssociateChannelCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHdpDisconnectAssociateChannelCfm *) CsrPmemZalloc(sizeof(CsrBtHdpDisconnectAssociateChannelCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->assocChId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HDP_DISCONNECT_ASSOCIATE_CHANNEL_CFM */

#endif /* EXCLUDE_CSR_BT_HDP_MODULE */
