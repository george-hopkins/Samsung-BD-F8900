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
#include "csr_bt_ppp_prim.h"

#ifndef EXCLUDE_CSR_BT_PPP_MODULE

void CsrBtPppPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_PPP_DISCONNECT_IND
CsrSize CsrBtPppDisconnectIndSizeof(void *msg)
{
    CsrBtPppDisconnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PPP_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_PPP_DISCONNECT_IND
CsrUint8 *CsrBtPppDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPppDisconnectInd *primitive;

    primitive = (CsrBtPppDisconnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->reason);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PPP_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_PPP_DISCONNECT_IND
void *CsrBtPppDisconnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPppDisconnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPppDisconnectInd *) CsrPmemZalloc(sizeof(CsrBtPppDisconnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->reason, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PPP_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_PPP_AUTHENTICATE_IND
CsrSize CsrBtPppAuthenticateIndSizeof(void *msg)
{
    CsrBtPppAuthenticateInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PPP_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_PPP_AUTHENTICATE_IND
CsrUint8 *CsrBtPppAuthenticateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPppAuthenticateInd *primitive;

    primitive = (CsrBtPppAuthenticateInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PPP_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_PPP_AUTHENTICATE_IND
void *CsrBtPppAuthenticateIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPppAuthenticateInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPppAuthenticateInd *) CsrPmemZalloc(sizeof(CsrBtPppAuthenticateInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PPP_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_PPP_UP_IND
CsrSize CsrBtPppUpIndSizeof(void *msg)
{
    CsrBtPppUpInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PPP_UP_IND */

#ifndef EXCLUDE_CSR_BT_PPP_UP_IND
CsrUint8 *CsrBtPppUpIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPppUpInd *primitive;

    primitive = (CsrBtPppUpInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PPP_UP_IND */

#ifndef EXCLUDE_CSR_BT_PPP_UP_IND
void *CsrBtPppUpIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPppUpInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPppUpInd *) CsrPmemZalloc(sizeof(CsrBtPppUpInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PPP_UP_IND */

#ifndef EXCLUDE_CSR_BT_PPP_CONNECT_REQ
CsrSize CsrBtPppConnectReqSizeof(void *msg)
{
    CsrBtPppConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PPP_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_PPP_CONNECT_REQ
CsrUint8 *CsrBtPppConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPppConnectReq *primitive;

    primitive = (CsrBtPppConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->applicationQueue);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PPP_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_PPP_CONNECT_REQ
void *CsrBtPppConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPppConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPppConnectReq *) CsrPmemZalloc(sizeof(CsrBtPppConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->applicationQueue, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PPP_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_PPP_DISCONNECT_REQ
CsrSize CsrBtPppDisconnectReqSizeof(void *msg)
{
    CsrBtPppDisconnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PPP_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_PPP_DISCONNECT_REQ
CsrUint8 *CsrBtPppDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPppDisconnectReq *primitive;

    primitive = (CsrBtPppDisconnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PPP_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_PPP_DISCONNECT_REQ
void *CsrBtPppDisconnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPppDisconnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPppDisconnectReq *) CsrPmemZalloc(sizeof(CsrBtPppDisconnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PPP_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_PPP_DATA_IND
CsrSize CsrBtPppDataIndSizeof(void *msg)
{
    CsrBtPppDataInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PPP_DATA_IND */

#ifndef EXCLUDE_CSR_BT_PPP_DATA_IND
CsrUint8 *CsrBtPppDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPppDataInd *primitive;

    primitive = (CsrBtPppDataInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->protocol);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PPP_DATA_IND */

#ifndef EXCLUDE_CSR_BT_PPP_DATA_IND
void *CsrBtPppDataIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPppDataInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPppDataInd *) CsrPmemZalloc(sizeof(CsrBtPppDataInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->protocol, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_PPP_DATA_IND */

#ifndef EXCLUDE_CSR_BT_PPP_DATA_IND
void CsrBtPppDataIndSerFree(void *msg)
{
    CsrBtPppDataInd *primitive;

    primitive = (CsrBtPppDataInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_PPP_DATA_IND */

#ifndef EXCLUDE_CSR_BT_PPP_CONNECT_CFM
CsrSize CsrBtPppConnectCfmSizeof(void *msg)
{
    CsrBtPppConnectCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PPP_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_PPP_CONNECT_CFM
CsrUint8 *CsrBtPppConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPppConnectCfm *primitive;

    primitive = (CsrBtPppConnectCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxPayloadLength);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PPP_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_PPP_CONNECT_CFM
void *CsrBtPppConnectCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPppConnectCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPppConnectCfm *) CsrPmemZalloc(sizeof(CsrBtPppConnectCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxPayloadLength, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PPP_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_PPP_REGISTER_NETWORK_PROTOCOL_REQ
CsrSize CsrBtPppRegisterNetworkProtocolReqSizeof(void *msg)
{
    CsrBtPppRegisterNetworkProtocolReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PPP_REGISTER_NETWORK_PROTOCOL_REQ */

#ifndef EXCLUDE_CSR_BT_PPP_REGISTER_NETWORK_PROTOCOL_REQ
CsrUint8 *CsrBtPppRegisterNetworkProtocolReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPppRegisterNetworkProtocolReq *primitive;

    primitive = (CsrBtPppRegisterNetworkProtocolReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->responseQueue);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ncpProtocol);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->dataQueue);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->dataProtocol);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PPP_REGISTER_NETWORK_PROTOCOL_REQ */

#ifndef EXCLUDE_CSR_BT_PPP_REGISTER_NETWORK_PROTOCOL_REQ
void *CsrBtPppRegisterNetworkProtocolReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPppRegisterNetworkProtocolReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPppRegisterNetworkProtocolReq *) CsrPmemZalloc(sizeof(CsrBtPppRegisterNetworkProtocolReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->responseQueue, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->ncpProtocol, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->dataQueue, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->dataProtocol, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PPP_REGISTER_NETWORK_PROTOCOL_REQ */

#ifndef EXCLUDE_CSR_BT_PPP_DATA_REQ
CsrSize CsrBtPppDataReqSizeof(void *msg)
{
    CsrBtPppDataReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PPP_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_PPP_DATA_REQ
CsrUint8 *CsrBtPppDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPppDataReq *primitive;

    primitive = (CsrBtPppDataReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->protocol);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PPP_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_PPP_DATA_REQ
void *CsrBtPppDataReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPppDataReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPppDataReq *) CsrPmemZalloc(sizeof(CsrBtPppDataReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->protocol, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_PPP_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_PPP_DATA_REQ
void CsrBtPppDataReqSerFree(void *msg)
{
    CsrBtPppDataReq *primitive;

    primitive = (CsrBtPppDataReq *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_PPP_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_PPP_NCP_REQ
CsrSize CsrBtPppNcpReqSizeof(void *msg)
{
    CsrBtPppNcpReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PPP_NCP_REQ */

#ifndef EXCLUDE_CSR_BT_PPP_NCP_REQ
CsrUint8 *CsrBtPppNcpReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPppNcpReq *primitive;

    primitive = (CsrBtPppNcpReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->protocol);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PPP_NCP_REQ */

#ifndef EXCLUDE_CSR_BT_PPP_NCP_REQ
void *CsrBtPppNcpReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPppNcpReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPppNcpReq *) CsrPmemZalloc(sizeof(CsrBtPppNcpReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->protocol, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_PPP_NCP_REQ */

#ifndef EXCLUDE_CSR_BT_PPP_NCP_REQ
void CsrBtPppNcpReqSerFree(void *msg)
{
    CsrBtPppNcpReq *primitive;

    primitive = (CsrBtPppNcpReq *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_PPP_NCP_REQ */

#ifndef EXCLUDE_CSR_BT_PPP_REGISTER_NETWORK_PROTOCOL_CFM
CsrSize CsrBtPppRegisterNetworkProtocolCfmSizeof(void *msg)
{
    CsrBtPppRegisterNetworkProtocolCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PPP_REGISTER_NETWORK_PROTOCOL_CFM */

#ifndef EXCLUDE_CSR_BT_PPP_REGISTER_NETWORK_PROTOCOL_CFM
CsrUint8 *CsrBtPppRegisterNetworkProtocolCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPppRegisterNetworkProtocolCfm *primitive;

    primitive = (CsrBtPppRegisterNetworkProtocolCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PPP_REGISTER_NETWORK_PROTOCOL_CFM */

#ifndef EXCLUDE_CSR_BT_PPP_REGISTER_NETWORK_PROTOCOL_CFM
void *CsrBtPppRegisterNetworkProtocolCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPppRegisterNetworkProtocolCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPppRegisterNetworkProtocolCfm *) CsrPmemZalloc(sizeof(CsrBtPppRegisterNetworkProtocolCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PPP_REGISTER_NETWORK_PROTOCOL_CFM */

#ifndef EXCLUDE_CSR_BT_PPP_DOWN_IND
CsrSize CsrBtPppDownIndSizeof(void *msg)
{
    CsrBtPppDownInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PPP_DOWN_IND */

#ifndef EXCLUDE_CSR_BT_PPP_DOWN_IND
CsrUint8 *CsrBtPppDownIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPppDownInd *primitive;

    primitive = (CsrBtPppDownInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PPP_DOWN_IND */

#ifndef EXCLUDE_CSR_BT_PPP_DOWN_IND
void *CsrBtPppDownIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPppDownInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPppDownInd *) CsrPmemZalloc(sizeof(CsrBtPppDownInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PPP_DOWN_IND */

#ifndef EXCLUDE_CSR_BT_PPP_NCP_IND
CsrSize CsrBtPppNcpIndSizeof(void *msg)
{
    CsrBtPppNcpInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PPP_NCP_IND */

#ifndef EXCLUDE_CSR_BT_PPP_NCP_IND
CsrUint8 *CsrBtPppNcpIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPppNcpInd *primitive;

    primitive = (CsrBtPppNcpInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->protocol);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PPP_NCP_IND */

#ifndef EXCLUDE_CSR_BT_PPP_NCP_IND
void *CsrBtPppNcpIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPppNcpInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPppNcpInd *) CsrPmemZalloc(sizeof(CsrBtPppNcpInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->protocol, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_PPP_NCP_IND */

#ifndef EXCLUDE_CSR_BT_PPP_NCP_IND
void CsrBtPppNcpIndSerFree(void *msg)
{
    CsrBtPppNcpInd *primitive;

    primitive = (CsrBtPppNcpInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_PPP_NCP_IND */

#ifndef EXCLUDE_CSR_BT_PPP_AUTHENTICATE_RES
CsrSize CsrBtPppAuthenticateResSizeof(void *msg)
{
    CsrBtPppAuthenticateRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->loginName) + CsrCharStringSerLen((CsrCharString *) primitive->password));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PPP_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_PPP_AUTHENTICATE_RES
CsrUint8 *CsrBtPppAuthenticateResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPppAuthenticateRes *primitive;

    primitive = (CsrBtPppAuthenticateRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->loginName, sizeof(primitive->loginName));
    CsrMemCpySer(buffer, length, &primitive->password, sizeof(primitive->password));
    CsrCharStringSer(buffer, length, primitive->loginName);
    CsrCharStringSer(buffer, length, primitive->password);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PPP_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_PPP_AUTHENTICATE_RES
void *CsrBtPppAuthenticateResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPppAuthenticateRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPppAuthenticateRes *) CsrPmemZalloc(sizeof(CsrBtPppAuthenticateRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->loginName), buffer, &offset, ((CsrSize) sizeof(primitive->loginName)));
    CsrMemCpyDes(((void *) &primitive->password), buffer, &offset, ((CsrSize) sizeof(primitive->password)));
    CsrCharStringDes(&primitive->loginName, buffer, &offset);
    CsrCharStringDes(&primitive->password, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PPP_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_PPP_AUTHENTICATE_RES
void CsrBtPppAuthenticateResSerFree(void *msg)
{
    CsrBtPppAuthenticateRes *primitive;

    primitive = (CsrBtPppAuthenticateRes *) msg;

    if(primitive->loginName != NULL)
    {
        CsrPmemFree(primitive->loginName);
    }
    if(primitive->password != NULL)
    {
        CsrPmemFree(primitive->password);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_PPP_AUTHENTICATE_RES */

#endif /* EXCLUDE_CSR_BT_PPP_MODULE */
