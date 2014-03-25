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
#include "csr_ip_socket_prim.h"

#ifndef EXCLUDE_CSR_IP_SOCKET_MODULE

void CsrIp_socketPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_UNSUBSCRIBE_REQ
CsrSize CsrIpSocketUdpMulticastUnsubscribeReqSizeof(void *msg)
{
    CsrIpSocketUdpMulticastUnsubscribeReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_UNSUBSCRIBE_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_UNSUBSCRIBE_REQ
CsrUint8 *CsrIpSocketUdpMulticastUnsubscribeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpSocketUdpMulticastUnsubscribeReq *primitive;

    primitive = (CsrIpSocketUdpMulticastUnsubscribeReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->socketHandle);
    CsrMemCpySer(buffer, length, ((const void *) primitive->interfaceIp), ((CsrSize) (16)));
    CsrMemCpySer(buffer, length, ((const void *) primitive->group), ((CsrSize) (16)));

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_UNSUBSCRIBE_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_UNSUBSCRIBE_REQ
void *CsrIpSocketUdpMulticastUnsubscribeReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpSocketUdpMulticastUnsubscribeReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpSocketUdpMulticastUnsubscribeReq *) CsrPmemZalloc(sizeof(CsrIpSocketUdpMulticastUnsubscribeReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->socketHandle, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->interfaceIp), buffer, &offset, ((CsrSize) (16)));
    CsrMemCpyDes(((void *) primitive->group), buffer, &offset, ((CsrSize) (16)));

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_UNSUBSCRIBE_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_OPTIONS_REQ
CsrSize CsrIpSocketOptionsReqSizeof(void *msg)
{
    CsrIpSocketOptionsReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_SOCKET_OPTIONS_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_OPTIONS_REQ
CsrUint8 *CsrIpSocketOptionsReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpSocketOptionsReq *primitive;

    primitive = (CsrIpSocketOptionsReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->txWindow);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->rxWindow);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->socketHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->nagle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->keepAlive);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->broadcast);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->dscp);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->validOptions);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_OPTIONS_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_OPTIONS_REQ
void *CsrIpSocketOptionsReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpSocketOptionsReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpSocketOptionsReq *) CsrPmemZalloc(sizeof(CsrIpSocketOptionsReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->txWindow, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->rxWindow, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->socketHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->nagle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->keepAlive, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->broadcast, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->dscp, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->validOptions, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_OPTIONS_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_OPTIONS_CFM
CsrSize CsrIpSocketOptionsCfmSizeof(void *msg)
{
    CsrIpSocketOptionsCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_SOCKET_OPTIONS_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_OPTIONS_CFM
CsrUint8 *CsrIpSocketOptionsCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpSocketOptionsCfm *primitive;

    primitive = (CsrIpSocketOptionsCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->socketHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->txWindow);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->rxWindow);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->nagle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->keepAlive);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->broadcast);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->dscp);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->validOptions);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_OPTIONS_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_OPTIONS_CFM
void *CsrIpSocketOptionsCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpSocketOptionsCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpSocketOptionsCfm *) CsrPmemZalloc(sizeof(CsrIpSocketOptionsCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->socketHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->txWindow, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->rxWindow, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->nagle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->keepAlive, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->broadcast, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->dscp, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->validOptions, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_OPTIONS_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_RAW_BIND_CFM
CsrSize CsrIpSocketRawBindCfmSizeof(void *msg)
{
    CsrIpSocketRawBindCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_SOCKET_RAW_BIND_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_RAW_BIND_CFM
CsrUint8 *CsrIpSocketRawBindCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpSocketRawBindCfm *primitive;

    primitive = (CsrIpSocketRawBindCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->socketHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_RAW_BIND_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_RAW_BIND_CFM
void *CsrIpSocketRawBindCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpSocketRawBindCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpSocketRawBindCfm *) CsrPmemZalloc(sizeof(CsrIpSocketRawBindCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->socketHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_RAW_BIND_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_RAW_DATA_IND
CsrSize CsrIpSocketRawDataIndSizeof(void *msg)
{
    CsrIpSocketRawDataInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->dataLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_SOCKET_RAW_DATA_IND */

#ifndef EXCLUDE_CSR_IP_SOCKET_RAW_DATA_IND
CsrUint8 *CsrIpSocketRawDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpSocketRawDataInd *primitive;

    primitive = (CsrIpSocketRawDataInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->socketHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->socketFamily);
    CsrMemCpySer(buffer, length, ((const void *) primitive->ipAddress), ((CsrSize) (16)));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->dataLength);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    if(primitive->data)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->data), ((CsrSize) (primitive->dataLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_RAW_DATA_IND */

#ifndef EXCLUDE_CSR_IP_SOCKET_RAW_DATA_IND
void *CsrIpSocketRawDataIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpSocketRawDataInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpSocketRawDataInd *) CsrPmemZalloc(sizeof(CsrIpSocketRawDataInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->socketHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->socketFamily, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->ipAddress), buffer, &offset, ((CsrSize) (16)));
    CsrUint16Des((CsrUint16 *) &primitive->dataLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
    if (primitive->dataLength)
    {
        primitive->data = CsrPmemZalloc(((CsrUint32) (primitive->dataLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->data), buffer, &offset, ((CsrSize) (primitive->dataLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->data = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_RAW_DATA_IND */

#ifndef EXCLUDE_CSR_IP_SOCKET_RAW_DATA_IND
void CsrIpSocketRawDataIndSerFree(void *msg)
{
    CsrIpSocketRawDataInd *primitive;

    primitive = (CsrIpSocketRawDataInd *) msg;

    if(primitive->data != NULL)
    {
        CsrPmemFree(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_IP_SOCKET_RAW_DATA_IND */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_SUBSCRIBE_REQ
CsrSize CsrIpSocketUdpMulticastSubscribeReqSizeof(void *msg)
{
    CsrIpSocketUdpMulticastSubscribeReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_SUBSCRIBE_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_SUBSCRIBE_REQ
CsrUint8 *CsrIpSocketUdpMulticastSubscribeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpSocketUdpMulticastSubscribeReq *primitive;

    primitive = (CsrIpSocketUdpMulticastSubscribeReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->socketHandle);
    CsrMemCpySer(buffer, length, ((const void *) primitive->interfaceIp), ((CsrSize) (16)));
    CsrMemCpySer(buffer, length, ((const void *) primitive->group), ((CsrSize) (16)));

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_SUBSCRIBE_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_SUBSCRIBE_REQ
void *CsrIpSocketUdpMulticastSubscribeReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpSocketUdpMulticastSubscribeReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpSocketUdpMulticastSubscribeReq *) CsrPmemZalloc(sizeof(CsrIpSocketUdpMulticastSubscribeReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->socketHandle, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->interfaceIp), buffer, &offset, ((CsrSize) (16)));
    CsrMemCpyDes(((void *) primitive->group), buffer, &offset, ((CsrSize) (16)));

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_SUBSCRIBE_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_RAW_DATA_CFM
CsrSize CsrIpSocketRawDataCfmSizeof(void *msg)
{
    CsrIpSocketRawDataCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_SOCKET_RAW_DATA_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_RAW_DATA_CFM
CsrUint8 *CsrIpSocketRawDataCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpSocketRawDataCfm *primitive;

    primitive = (CsrIpSocketRawDataCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->socketHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_RAW_DATA_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_RAW_DATA_CFM
void *CsrIpSocketRawDataCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpSocketRawDataCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpSocketRawDataCfm *) CsrPmemZalloc(sizeof(CsrIpSocketRawDataCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->socketHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_RAW_DATA_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_NEW_CFM
CsrSize CsrIpSocketTcpNewCfmSizeof(void *msg)
{
    CsrIpSocketTcpNewCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_NEW_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_NEW_CFM
CsrUint8 *CsrIpSocketTcpNewCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpSocketTcpNewCfm *primitive;

    primitive = (CsrIpSocketTcpNewCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->socketHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_NEW_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_NEW_CFM
void *CsrIpSocketTcpNewCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpSocketTcpNewCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpSocketTcpNewCfm *) CsrPmemZalloc(sizeof(CsrIpSocketTcpNewCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->socketHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_NEW_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_DATA_REQ
CsrSize CsrIpSocketUdpDataReqSizeof(void *msg)
{
    CsrIpSocketUdpDataReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->dataLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_DATA_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_DATA_REQ
CsrUint8 *CsrIpSocketUdpDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpSocketUdpDataReq *primitive;

    primitive = (CsrIpSocketUdpDataReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->socketHandle);
    CsrMemCpySer(buffer, length, ((const void *) primitive->ipAddress), ((CsrSize) (16)));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->port);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->dataLength);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    if(primitive->data)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->data), ((CsrSize) (primitive->dataLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_DATA_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_DATA_REQ
void *CsrIpSocketUdpDataReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpSocketUdpDataReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpSocketUdpDataReq *) CsrPmemZalloc(sizeof(CsrIpSocketUdpDataReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->socketHandle, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->ipAddress), buffer, &offset, ((CsrSize) (16)));
    CsrUint16Des((CsrUint16 *) &primitive->port, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->dataLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
    if (primitive->dataLength)
    {
        primitive->data = CsrPmemZalloc(((CsrUint32) (primitive->dataLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->data), buffer, &offset, ((CsrSize) (primitive->dataLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->data = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_DATA_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_DATA_REQ
void CsrIpSocketUdpDataReqSerFree(void *msg)
{
    CsrIpSocketUdpDataReq *primitive;

    primitive = (CsrIpSocketUdpDataReq *) msg;

    if(primitive->data != NULL)
    {
        CsrPmemFree(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_DATA_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_ABORT_REQ
CsrSize CsrIpSocketTcpAbortReqSizeof(void *msg)
{
    CsrIpSocketTcpAbortReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_ABORT_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_ABORT_REQ
CsrUint8 *CsrIpSocketTcpAbortReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpSocketTcpAbortReq *primitive;

    primitive = (CsrIpSocketTcpAbortReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->socketHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_ABORT_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_ABORT_REQ
void *CsrIpSocketTcpAbortReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpSocketTcpAbortReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpSocketTcpAbortReq *) CsrPmemZalloc(sizeof(CsrIpSocketTcpAbortReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->socketHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_ABORT_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_NEW_REQ
CsrSize CsrIpSocketTcpNewReqSizeof(void *msg)
{
    CsrIpSocketTcpNewReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_NEW_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_NEW_REQ
CsrUint8 *CsrIpSocketTcpNewReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpSocketTcpNewReq *primitive;

    primitive = (CsrIpSocketTcpNewReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->socketFamily);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_NEW_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_NEW_REQ
void *CsrIpSocketTcpNewReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpSocketTcpNewReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpSocketTcpNewReq *) CsrPmemZalloc(sizeof(CsrIpSocketTcpNewReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->socketFamily, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_NEW_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_UNSUBSCRIBE_CFM
CsrSize CsrIpSocketUdpMulticastUnsubscribeCfmSizeof(void *msg)
{
    CsrIpSocketUdpMulticastUnsubscribeCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_UNSUBSCRIBE_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_UNSUBSCRIBE_CFM
CsrUint8 *CsrIpSocketUdpMulticastUnsubscribeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpSocketUdpMulticastUnsubscribeCfm *primitive;

    primitive = (CsrIpSocketUdpMulticastUnsubscribeCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->socketHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->socketFamily);
    CsrMemCpySer(buffer, length, ((const void *) primitive->interfaceIp), ((CsrSize) (16)));
    CsrMemCpySer(buffer, length, ((const void *) primitive->group), ((CsrSize) (16)));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_UNSUBSCRIBE_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_UNSUBSCRIBE_CFM
void *CsrIpSocketUdpMulticastUnsubscribeCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpSocketUdpMulticastUnsubscribeCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpSocketUdpMulticastUnsubscribeCfm *) CsrPmemZalloc(sizeof(CsrIpSocketUdpMulticastUnsubscribeCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->socketHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->socketFamily, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->interfaceIp), buffer, &offset, ((CsrSize) (16)));
    CsrMemCpyDes(((void *) primitive->group), buffer, &offset, ((CsrSize) (16)));
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_UNSUBSCRIBE_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_CLOSE_REQ
CsrSize CsrIpSocketTcpCloseReqSizeof(void *msg)
{
    CsrIpSocketTcpCloseReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_CLOSE_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_CLOSE_REQ
CsrUint8 *CsrIpSocketTcpCloseReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpSocketTcpCloseReq *primitive;

    primitive = (CsrIpSocketTcpCloseReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->socketHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_CLOSE_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_CLOSE_REQ
void *CsrIpSocketTcpCloseReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpSocketTcpCloseReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpSocketTcpCloseReq *) CsrPmemZalloc(sizeof(CsrIpSocketTcpCloseReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->socketHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_CLOSE_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_CONNECT_REQ
CsrSize CsrIpSocketTcpConnectReqSizeof(void *msg)
{
    CsrIpSocketTcpConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_CONNECT_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_CONNECT_REQ
CsrUint8 *CsrIpSocketTcpConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpSocketTcpConnectReq *primitive;

    primitive = (CsrIpSocketTcpConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->socketHandle);
    CsrMemCpySer(buffer, length, ((const void *) primitive->ipAddress), ((CsrSize) (16)));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->port);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_CONNECT_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_CONNECT_REQ
void *CsrIpSocketTcpConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpSocketTcpConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpSocketTcpConnectReq *) CsrPmemZalloc(sizeof(CsrIpSocketTcpConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->socketHandle, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->ipAddress), buffer, &offset, ((CsrSize) (16)));
    CsrUint16Des((CsrUint16 *) &primitive->port, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_CONNECT_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_INTERFACE_REQ
CsrSize CsrIpSocketUdpMulticastInterfaceReqSizeof(void *msg)
{
    CsrIpSocketUdpMulticastInterfaceReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_INTERFACE_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_INTERFACE_REQ
CsrUint8 *CsrIpSocketUdpMulticastInterfaceReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpSocketUdpMulticastInterfaceReq *primitive;

    primitive = (CsrIpSocketUdpMulticastInterfaceReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->socketHandle);
    CsrMemCpySer(buffer, length, ((const void *) primitive->interfaceIp), ((CsrSize) (16)));

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_INTERFACE_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_INTERFACE_REQ
void *CsrIpSocketUdpMulticastInterfaceReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpSocketUdpMulticastInterfaceReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpSocketUdpMulticastInterfaceReq *) CsrPmemZalloc(sizeof(CsrIpSocketUdpMulticastInterfaceReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->socketHandle, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->interfaceIp), buffer, &offset, ((CsrSize) (16)));

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_INTERFACE_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_DNS_RESOLVE_NAME_REQ
CsrSize CsrIpSocketDnsResolveNameReqSizeof(void *msg)
{
    CsrIpSocketDnsResolveNameReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->name));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_SOCKET_DNS_RESOLVE_NAME_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_DNS_RESOLVE_NAME_REQ
CsrUint8 *CsrIpSocketDnsResolveNameReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpSocketDnsResolveNameReq *primitive;

    primitive = (CsrIpSocketDnsResolveNameReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrMemCpySer(buffer, length, &primitive->name, sizeof(primitive->name));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->socketFamilyMax);
    CsrCharStringSer(buffer, length, primitive->name);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_DNS_RESOLVE_NAME_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_DNS_RESOLVE_NAME_REQ
void *CsrIpSocketDnsResolveNameReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpSocketDnsResolveNameReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpSocketDnsResolveNameReq *) CsrPmemZalloc(sizeof(CsrIpSocketDnsResolveNameReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->name), buffer, &offset, ((CsrSize) sizeof(primitive->name)));
    CsrUint8Des((CsrUint8 *) &primitive->socketFamilyMax, buffer, &offset);
    CsrCharStringDes(&primitive->name, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_DNS_RESOLVE_NAME_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_DNS_RESOLVE_NAME_REQ
void CsrIpSocketDnsResolveNameReqSerFree(void *msg)
{
    CsrIpSocketDnsResolveNameReq *primitive;

    primitive = (CsrIpSocketDnsResolveNameReq *) msg;

    if(primitive->name != NULL)
    {
        CsrPmemFree(primitive->name);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_IP_SOCKET_DNS_RESOLVE_NAME_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_INTERFACE_CFM
CsrSize CsrIpSocketUdpMulticastInterfaceCfmSizeof(void *msg)
{
    CsrIpSocketUdpMulticastInterfaceCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_INTERFACE_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_INTERFACE_CFM
CsrUint8 *CsrIpSocketUdpMulticastInterfaceCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpSocketUdpMulticastInterfaceCfm *primitive;

    primitive = (CsrIpSocketUdpMulticastInterfaceCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->socketHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_INTERFACE_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_INTERFACE_CFM
void *CsrIpSocketUdpMulticastInterfaceCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpSocketUdpMulticastInterfaceCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpSocketUdpMulticastInterfaceCfm *) CsrPmemZalloc(sizeof(CsrIpSocketUdpMulticastInterfaceCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->socketHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_INTERFACE_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_RAW_NEW_REQ
CsrSize CsrIpSocketRawNewReqSizeof(void *msg)
{
    CsrIpSocketRawNewReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_SOCKET_RAW_NEW_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_RAW_NEW_REQ
CsrUint8 *CsrIpSocketRawNewReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpSocketRawNewReq *primitive;

    primitive = (CsrIpSocketRawNewReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->socketFamily);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->protocolNumber);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_RAW_NEW_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_RAW_NEW_REQ
void *CsrIpSocketRawNewReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpSocketRawNewReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpSocketRawNewReq *) CsrPmemZalloc(sizeof(CsrIpSocketRawNewReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->socketFamily, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->protocolNumber, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_RAW_NEW_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_SUBSCRIBE_CFM
CsrSize CsrIpSocketUdpMulticastSubscribeCfmSizeof(void *msg)
{
    CsrIpSocketUdpMulticastSubscribeCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_SUBSCRIBE_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_SUBSCRIBE_CFM
CsrUint8 *CsrIpSocketUdpMulticastSubscribeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpSocketUdpMulticastSubscribeCfm *primitive;

    primitive = (CsrIpSocketUdpMulticastSubscribeCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->socketHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->socketFamily);
    CsrMemCpySer(buffer, length, ((const void *) primitive->interfaceIp), ((CsrSize) (16)));
    CsrMemCpySer(buffer, length, ((const void *) primitive->group), ((CsrSize) (16)));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_SUBSCRIBE_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_SUBSCRIBE_CFM
void *CsrIpSocketUdpMulticastSubscribeCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpSocketUdpMulticastSubscribeCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpSocketUdpMulticastSubscribeCfm *) CsrPmemZalloc(sizeof(CsrIpSocketUdpMulticastSubscribeCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->socketHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->socketFamily, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->interfaceIp), buffer, &offset, ((CsrSize) (16)));
    CsrMemCpyDes(((void *) primitive->group), buffer, &offset, ((CsrSize) (16)));
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_SUBSCRIBE_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_DATA_IND
CsrSize CsrIpSocketUdpDataIndSizeof(void *msg)
{
    CsrIpSocketUdpDataInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->dataLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_DATA_IND */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_DATA_IND
CsrUint8 *CsrIpSocketUdpDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpSocketUdpDataInd *primitive;

    primitive = (CsrIpSocketUdpDataInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->socketHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->socketFamily);
    CsrMemCpySer(buffer, length, ((const void *) primitive->ipAddress), ((CsrSize) (16)));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->port);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->dataLength);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    if(primitive->data)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->data), ((CsrSize) (primitive->dataLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_DATA_IND */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_DATA_IND
void *CsrIpSocketUdpDataIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpSocketUdpDataInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpSocketUdpDataInd *) CsrPmemZalloc(sizeof(CsrIpSocketUdpDataInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->socketHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->socketFamily, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->ipAddress), buffer, &offset, ((CsrSize) (16)));
    CsrUint16Des((CsrUint16 *) &primitive->port, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->dataLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
    if (primitive->dataLength)
    {
        primitive->data = CsrPmemZalloc(((CsrUint32) (primitive->dataLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->data), buffer, &offset, ((CsrSize) (primitive->dataLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->data = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_DATA_IND */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_DATA_IND
void CsrIpSocketUdpDataIndSerFree(void *msg)
{
    CsrIpSocketUdpDataInd *primitive;

    primitive = (CsrIpSocketUdpDataInd *) msg;

    if(primitive->data != NULL)
    {
        CsrPmemFree(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_DATA_IND */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_BIND_CFM
CsrSize CsrIpSocketTcpBindCfmSizeof(void *msg)
{
    CsrIpSocketTcpBindCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_BIND_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_BIND_CFM
CsrUint8 *CsrIpSocketTcpBindCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpSocketTcpBindCfm *primitive;

    primitive = (CsrIpSocketTcpBindCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->socketHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->port);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_BIND_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_BIND_CFM
void *CsrIpSocketTcpBindCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpSocketTcpBindCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpSocketTcpBindCfm *) CsrPmemZalloc(sizeof(CsrIpSocketTcpBindCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->socketHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->port, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_BIND_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_BIND_REQ
CsrSize CsrIpSocketUdpBindReqSizeof(void *msg)
{
    CsrIpSocketUdpBindReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_BIND_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_BIND_REQ
CsrUint8 *CsrIpSocketUdpBindReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpSocketUdpBindReq *primitive;

    primitive = (CsrIpSocketUdpBindReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->socketHandle);
    CsrMemCpySer(buffer, length, ((const void *) primitive->ipAddress), ((CsrSize) (16)));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->port);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_BIND_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_BIND_REQ
void *CsrIpSocketUdpBindReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpSocketUdpBindReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpSocketUdpBindReq *) CsrPmemZalloc(sizeof(CsrIpSocketUdpBindReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->socketHandle, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->ipAddress), buffer, &offset, ((CsrSize) (16)));
    CsrUint16Des((CsrUint16 *) &primitive->port, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_BIND_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_BIND_CFM
CsrSize CsrIpSocketUdpBindCfmSizeof(void *msg)
{
    CsrIpSocketUdpBindCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_BIND_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_BIND_CFM
CsrUint8 *CsrIpSocketUdpBindCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpSocketUdpBindCfm *primitive;

    primitive = (CsrIpSocketUdpBindCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->socketHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->port);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_BIND_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_BIND_CFM
void *CsrIpSocketUdpBindCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpSocketUdpBindCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpSocketUdpBindCfm *) CsrPmemZalloc(sizeof(CsrIpSocketUdpBindCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->socketHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->port, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_BIND_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_CLOSE_IND
CsrSize CsrIpSocketTcpCloseIndSizeof(void *msg)
{
    CsrIpSocketTcpCloseInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_CLOSE_IND */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_CLOSE_IND
CsrUint8 *CsrIpSocketTcpCloseIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpSocketTcpCloseInd *primitive;

    primitive = (CsrIpSocketTcpCloseInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->socketHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_CLOSE_IND */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_CLOSE_IND
void *CsrIpSocketTcpCloseIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpSocketTcpCloseInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpSocketTcpCloseInd *) CsrPmemZalloc(sizeof(CsrIpSocketTcpCloseInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->socketHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_CLOSE_IND */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_ACCEPT_IND
CsrSize CsrIpSocketTcpAcceptIndSizeof(void *msg)
{
    CsrIpSocketTcpAcceptInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_ACCEPT_IND */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_ACCEPT_IND
CsrUint8 *CsrIpSocketTcpAcceptIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpSocketTcpAcceptInd *primitive;

    primitive = (CsrIpSocketTcpAcceptInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->socketHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->socketFamily);
    CsrMemCpySer(buffer, length, ((const void *) primitive->ipAddress), ((CsrSize) (16)));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->port);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->listenSocketHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_ACCEPT_IND */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_ACCEPT_IND
void *CsrIpSocketTcpAcceptIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpSocketTcpAcceptInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpSocketTcpAcceptInd *) CsrPmemZalloc(sizeof(CsrIpSocketTcpAcceptInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->socketHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->socketFamily, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->ipAddress), buffer, &offset, ((CsrSize) (16)));
    CsrUint16Des((CsrUint16 *) &primitive->port, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->listenSocketHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_ACCEPT_IND */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_DATA_CFM
CsrSize CsrIpSocketTcpDataCfmSizeof(void *msg)
{
    CsrIpSocketTcpDataCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_DATA_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_DATA_CFM
CsrUint8 *CsrIpSocketTcpDataCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpSocketTcpDataCfm *primitive;

    primitive = (CsrIpSocketTcpDataCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->socketHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_DATA_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_DATA_CFM
void *CsrIpSocketTcpDataCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpSocketTcpDataCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpSocketTcpDataCfm *) CsrPmemZalloc(sizeof(CsrIpSocketTcpDataCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->socketHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_DATA_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_BIND_REQ
CsrSize CsrIpSocketTcpBindReqSizeof(void *msg)
{
    CsrIpSocketTcpBindReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_BIND_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_BIND_REQ
CsrUint8 *CsrIpSocketTcpBindReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpSocketTcpBindReq *primitive;

    primitive = (CsrIpSocketTcpBindReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->socketHandle);
    CsrMemCpySer(buffer, length, ((const void *) primitive->ipAddress), ((CsrSize) (16)));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->port);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_BIND_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_BIND_REQ
void *CsrIpSocketTcpBindReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpSocketTcpBindReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpSocketTcpBindReq *) CsrPmemZalloc(sizeof(CsrIpSocketTcpBindReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->socketHandle, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->ipAddress), buffer, &offset, ((CsrSize) (16)));
    CsrUint16Des((CsrUint16 *) &primitive->port, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_BIND_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_RAW_NEW_CFM
CsrSize CsrIpSocketRawNewCfmSizeof(void *msg)
{
    CsrIpSocketRawNewCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_SOCKET_RAW_NEW_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_RAW_NEW_CFM
CsrUint8 *CsrIpSocketRawNewCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpSocketRawNewCfm *primitive;

    primitive = (CsrIpSocketRawNewCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->socketHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_RAW_NEW_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_RAW_NEW_CFM
void *CsrIpSocketRawNewCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpSocketRawNewCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpSocketRawNewCfm *) CsrPmemZalloc(sizeof(CsrIpSocketRawNewCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->socketHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_RAW_NEW_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_RAW_BIND_REQ
CsrSize CsrIpSocketRawBindReqSizeof(void *msg)
{
    CsrIpSocketRawBindReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_SOCKET_RAW_BIND_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_RAW_BIND_REQ
CsrUint8 *CsrIpSocketRawBindReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpSocketRawBindReq *primitive;

    primitive = (CsrIpSocketRawBindReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->socketHandle);
    CsrMemCpySer(buffer, length, ((const void *) primitive->ipAddress), ((CsrSize) (16)));

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_RAW_BIND_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_RAW_BIND_REQ
void *CsrIpSocketRawBindReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpSocketRawBindReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpSocketRawBindReq *) CsrPmemZalloc(sizeof(CsrIpSocketRawBindReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->socketHandle, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->ipAddress), buffer, &offset, ((CsrSize) (16)));

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_RAW_BIND_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_DATA_IND
CsrSize CsrIpSocketTcpDataIndSizeof(void *msg)
{
    CsrIpSocketTcpDataInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->dataLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_DATA_IND */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_DATA_IND
CsrUint8 *CsrIpSocketTcpDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpSocketTcpDataInd *primitive;

    primitive = (CsrIpSocketTcpDataInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->socketHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->dataLength);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    if(primitive->data)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->data), ((CsrSize) (primitive->dataLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_DATA_IND */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_DATA_IND
void *CsrIpSocketTcpDataIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpSocketTcpDataInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpSocketTcpDataInd *) CsrPmemZalloc(sizeof(CsrIpSocketTcpDataInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->socketHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->dataLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
    if (primitive->dataLength)
    {
        primitive->data = CsrPmemZalloc(((CsrUint32) (primitive->dataLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->data), buffer, &offset, ((CsrSize) (primitive->dataLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->data = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_DATA_IND */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_DATA_IND
void CsrIpSocketTcpDataIndSerFree(void *msg)
{
    CsrIpSocketTcpDataInd *primitive;

    primitive = (CsrIpSocketTcpDataInd *) msg;

    if(primitive->data != NULL)
    {
        CsrPmemFree(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_DATA_IND */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_NEW_CFM
CsrSize CsrIpSocketUdpNewCfmSizeof(void *msg)
{
    CsrIpSocketUdpNewCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_NEW_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_NEW_CFM
CsrUint8 *CsrIpSocketUdpNewCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpSocketUdpNewCfm *primitive;

    primitive = (CsrIpSocketUdpNewCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->socketHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_NEW_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_NEW_CFM
void *CsrIpSocketUdpNewCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpSocketUdpNewCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpSocketUdpNewCfm *) CsrPmemZalloc(sizeof(CsrIpSocketUdpNewCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->socketHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_NEW_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_RAW_CLOSE_REQ
CsrSize CsrIpSocketRawCloseReqSizeof(void *msg)
{
    CsrIpSocketRawCloseReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_SOCKET_RAW_CLOSE_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_RAW_CLOSE_REQ
CsrUint8 *CsrIpSocketRawCloseReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpSocketRawCloseReq *primitive;

    primitive = (CsrIpSocketRawCloseReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->socketHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_RAW_CLOSE_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_RAW_CLOSE_REQ
void *CsrIpSocketRawCloseReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpSocketRawCloseReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpSocketRawCloseReq *) CsrPmemZalloc(sizeof(CsrIpSocketRawCloseReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->socketHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_RAW_CLOSE_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_DNS_RESOLVE_NAME_CFM
CsrSize CsrIpSocketDnsResolveNameCfmSizeof(void *msg)
{
    CsrIpSocketDnsResolveNameCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->name));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_SOCKET_DNS_RESOLVE_NAME_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_DNS_RESOLVE_NAME_CFM
CsrUint8 *CsrIpSocketDnsResolveNameCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpSocketDnsResolveNameCfm *primitive;

    primitive = (CsrIpSocketDnsResolveNameCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->name, sizeof(primitive->name));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->socketFamily);
    CsrMemCpySer(buffer, length, ((const void *) primitive->ipAddress), ((CsrSize) (16)));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrCharStringSer(buffer, length, primitive->name);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_DNS_RESOLVE_NAME_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_DNS_RESOLVE_NAME_CFM
void *CsrIpSocketDnsResolveNameCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpSocketDnsResolveNameCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpSocketDnsResolveNameCfm *) CsrPmemZalloc(sizeof(CsrIpSocketDnsResolveNameCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->name), buffer, &offset, ((CsrSize) sizeof(primitive->name)));
    CsrUint8Des((CsrUint8 *) &primitive->socketFamily, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->ipAddress), buffer, &offset, ((CsrSize) (16)));
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrCharStringDes(&primitive->name, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_DNS_RESOLVE_NAME_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_DNS_RESOLVE_NAME_CFM
void CsrIpSocketDnsResolveNameCfmSerFree(void *msg)
{
    CsrIpSocketDnsResolveNameCfm *primitive;

    primitive = (CsrIpSocketDnsResolveNameCfm *) msg;

    if(primitive->name != NULL)
    {
        CsrPmemFree(primitive->name);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_IP_SOCKET_DNS_RESOLVE_NAME_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_DATA_REQ
CsrSize CsrIpSocketTcpDataReqSizeof(void *msg)
{
    CsrIpSocketTcpDataReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->dataLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_DATA_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_DATA_REQ
CsrUint8 *CsrIpSocketTcpDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpSocketTcpDataReq *primitive;

    primitive = (CsrIpSocketTcpDataReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->socketHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->dataLength);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    if(primitive->data)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->data), ((CsrSize) (primitive->dataLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_DATA_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_DATA_REQ
void *CsrIpSocketTcpDataReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpSocketTcpDataReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpSocketTcpDataReq *) CsrPmemZalloc(sizeof(CsrIpSocketTcpDataReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->socketHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->dataLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
    if (primitive->dataLength)
    {
        primitive->data = CsrPmemZalloc(((CsrUint32) (primitive->dataLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->data), buffer, &offset, ((CsrSize) (primitive->dataLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->data = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_DATA_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_DATA_REQ
void CsrIpSocketTcpDataReqSerFree(void *msg)
{
    CsrIpSocketTcpDataReq *primitive;

    primitive = (CsrIpSocketTcpDataReq *) msg;

    if(primitive->data != NULL)
    {
        CsrPmemFree(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_DATA_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_LISTEN_CFM
CsrSize CsrIpSocketTcpListenCfmSizeof(void *msg)
{
    CsrIpSocketTcpListenCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_LISTEN_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_LISTEN_CFM
CsrUint8 *CsrIpSocketTcpListenCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpSocketTcpListenCfm *primitive;

    primitive = (CsrIpSocketTcpListenCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->socketHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_LISTEN_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_LISTEN_CFM
void *CsrIpSocketTcpListenCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpSocketTcpListenCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpSocketTcpListenCfm *) CsrPmemZalloc(sizeof(CsrIpSocketTcpListenCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->socketHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_LISTEN_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_NEW_REQ
CsrSize CsrIpSocketUdpNewReqSizeof(void *msg)
{
    CsrIpSocketUdpNewReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_NEW_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_NEW_REQ
CsrUint8 *CsrIpSocketUdpNewReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpSocketUdpNewReq *primitive;

    primitive = (CsrIpSocketUdpNewReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->socketFamily);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_NEW_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_NEW_REQ
void *CsrIpSocketUdpNewReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpSocketUdpNewReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpSocketUdpNewReq *) CsrPmemZalloc(sizeof(CsrIpSocketUdpNewReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->socketFamily, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_NEW_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_RAW_DATA_REQ
CsrSize CsrIpSocketRawDataReqSizeof(void *msg)
{
    CsrIpSocketRawDataReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->dataLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_SOCKET_RAW_DATA_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_RAW_DATA_REQ
CsrUint8 *CsrIpSocketRawDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpSocketRawDataReq *primitive;

    primitive = (CsrIpSocketRawDataReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->socketHandle);
    CsrMemCpySer(buffer, length, ((const void *) primitive->ipAddress), ((CsrSize) (16)));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->dataLength);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    if(primitive->data)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->data), ((CsrSize) (primitive->dataLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_RAW_DATA_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_RAW_DATA_REQ
void *CsrIpSocketRawDataReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpSocketRawDataReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpSocketRawDataReq *) CsrPmemZalloc(sizeof(CsrIpSocketRawDataReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->socketHandle, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->ipAddress), buffer, &offset, ((CsrSize) (16)));
    CsrUint16Des((CsrUint16 *) &primitive->dataLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
    if (primitive->dataLength)
    {
        primitive->data = CsrPmemZalloc(((CsrUint32) (primitive->dataLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->data), buffer, &offset, ((CsrSize) (primitive->dataLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->data = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_RAW_DATA_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_RAW_DATA_REQ
void CsrIpSocketRawDataReqSerFree(void *msg)
{
    CsrIpSocketRawDataReq *primitive;

    primitive = (CsrIpSocketRawDataReq *) msg;

    if(primitive->data != NULL)
    {
        CsrPmemFree(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_IP_SOCKET_RAW_DATA_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_LISTEN_REQ
CsrSize CsrIpSocketTcpListenReqSizeof(void *msg)
{
    CsrIpSocketTcpListenReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_LISTEN_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_LISTEN_REQ
CsrUint8 *CsrIpSocketTcpListenReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpSocketTcpListenReq *primitive;

    primitive = (CsrIpSocketTcpListenReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->socketHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_LISTEN_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_LISTEN_REQ
void *CsrIpSocketTcpListenReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpSocketTcpListenReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpSocketTcpListenReq *) CsrPmemZalloc(sizeof(CsrIpSocketTcpListenReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->socketHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_LISTEN_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_CONNECT_CFM
CsrSize CsrIpSocketTcpConnectCfmSizeof(void *msg)
{
    CsrIpSocketTcpConnectCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_CONNECT_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_CONNECT_CFM
CsrUint8 *CsrIpSocketTcpConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpSocketTcpConnectCfm *primitive;

    primitive = (CsrIpSocketTcpConnectCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->socketHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_CONNECT_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_CONNECT_CFM
void *CsrIpSocketTcpConnectCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpSocketTcpConnectCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpSocketTcpConnectCfm *) CsrPmemZalloc(sizeof(CsrIpSocketTcpConnectCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->socketHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_CONNECT_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_DATA_RES
CsrSize CsrIpSocketTcpDataResSizeof(void *msg)
{
    CsrIpSocketTcpDataRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_DATA_RES */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_DATA_RES
CsrUint8 *CsrIpSocketTcpDataResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpSocketTcpDataRes *primitive;

    primitive = (CsrIpSocketTcpDataRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->socketHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_DATA_RES */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_DATA_RES
void *CsrIpSocketTcpDataResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpSocketTcpDataRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpSocketTcpDataRes *) CsrPmemZalloc(sizeof(CsrIpSocketTcpDataRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->socketHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_DATA_RES */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_DATA_CFM
CsrSize CsrIpSocketUdpDataCfmSizeof(void *msg)
{
    CsrIpSocketUdpDataCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_DATA_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_DATA_CFM
CsrUint8 *CsrIpSocketUdpDataCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpSocketUdpDataCfm *primitive;

    primitive = (CsrIpSocketUdpDataCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->socketHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_DATA_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_DATA_CFM
void *CsrIpSocketUdpDataCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpSocketUdpDataCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpSocketUdpDataCfm *) CsrPmemZalloc(sizeof(CsrIpSocketUdpDataCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->socketHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_DATA_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_CLOSE_REQ
CsrSize CsrIpSocketUdpCloseReqSizeof(void *msg)
{
    CsrIpSocketUdpCloseReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_CLOSE_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_CLOSE_REQ
CsrUint8 *CsrIpSocketUdpCloseReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpSocketUdpCloseReq *primitive;

    primitive = (CsrIpSocketUdpCloseReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->socketHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_CLOSE_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_CLOSE_REQ
void *CsrIpSocketUdpCloseReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpSocketUdpCloseReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpSocketUdpCloseReq *) CsrPmemZalloc(sizeof(CsrIpSocketUdpCloseReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->socketHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_CLOSE_REQ */

#endif /* EXCLUDE_CSR_IP_SOCKET_MODULE */
