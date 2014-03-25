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
#include "csr_bt_udp_prim.h"

#ifndef EXCLUDE_CSR_BT_UDP_MODULE

void CsrBtUdpPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_UDP_REGISTER_PORT_CFM
CsrSize CsrBtUdpRegisterPortCfmSizeof(void *msg)
{
    CsrBtUdpRegisterPortCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_UDP_REGISTER_PORT_CFM */

#ifndef EXCLUDE_CSR_BT_UDP_REGISTER_PORT_CFM
CsrUint8 *CsrBtUdpRegisterPortCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtUdpRegisterPortCfm *primitive;

    primitive = (CsrBtUdpRegisterPortCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->port);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_UDP_REGISTER_PORT_CFM */

#ifndef EXCLUDE_CSR_BT_UDP_REGISTER_PORT_CFM
void *CsrBtUdpRegisterPortCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtUdpRegisterPortCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtUdpRegisterPortCfm *) CsrPmemZalloc(sizeof(CsrBtUdpRegisterPortCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->port, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_UDP_REGISTER_PORT_CFM */

#ifndef EXCLUDE_CSR_BT_UDP_DATA_IND
CsrSize CsrBtUdpDataIndSizeof(void *msg)
{
    CsrBtUdpDataInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->udpDataLength * sizeof(CsrUint16)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_UDP_DATA_IND */

#ifndef EXCLUDE_CSR_BT_UDP_DATA_IND
CsrUint8 *CsrBtUdpDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtUdpDataInd *primitive;

    primitive = (CsrBtUdpDataInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, ((const void *) primitive->sourceAddress), ((CsrSize) 2 * (2)));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->udpDataLength);
    CsrMemCpySer(buffer, length, &primitive->udpData, sizeof(primitive->udpData));
    if(primitive->udpData)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->udpData), ((CsrSize) (primitive->udpDataLength * sizeof(CsrUint16))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_UDP_DATA_IND */

#ifndef EXCLUDE_CSR_BT_UDP_DATA_IND
void *CsrBtUdpDataIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtUdpDataInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtUdpDataInd *) CsrPmemZalloc(sizeof(CsrBtUdpDataInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->sourceAddress), buffer, &offset, ((CsrSize) 2 * (2)));
    CsrUint16Des((CsrUint16 *) &primitive->udpDataLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->udpData), buffer, &offset, ((CsrSize) sizeof(primitive->udpData)));
    if (primitive->udpDataLength)
    {
        primitive->udpData = CsrPmemZalloc(((CsrUint32) (primitive->udpDataLength * sizeof(CsrUint16))));
        CsrMemCpyDes(((void *) primitive->udpData), buffer, &offset, ((CsrSize) (primitive->udpDataLength * sizeof(CsrUint16))));
    }
    else
    {
        primitive->udpData = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_UDP_DATA_IND */

#ifndef EXCLUDE_CSR_BT_UDP_DATA_IND
void CsrBtUdpDataIndSerFree(void *msg)
{
    CsrBtUdpDataInd *primitive;

    primitive = (CsrBtUdpDataInd *) msg;

    if(primitive->udpData != NULL)
    {
        CsrPmemFree(primitive->udpData);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_UDP_DATA_IND */

#ifndef EXCLUDE_CSR_BT_UDP_PORT_UNREACHABLE_IND
CsrSize CsrBtUdpPortUnreachableIndSizeof(void *msg)
{
    CsrBtUdpPortUnreachableInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_UDP_PORT_UNREACHABLE_IND */

#ifndef EXCLUDE_CSR_BT_UDP_PORT_UNREACHABLE_IND
CsrUint8 *CsrBtUdpPortUnreachableIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtUdpPortUnreachableInd *primitive;

    primitive = (CsrBtUdpPortUnreachableInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->localPort);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->remotePort);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_UDP_PORT_UNREACHABLE_IND */

#ifndef EXCLUDE_CSR_BT_UDP_PORT_UNREACHABLE_IND
void *CsrBtUdpPortUnreachableIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtUdpPortUnreachableInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtUdpPortUnreachableInd *) CsrPmemZalloc(sizeof(CsrBtUdpPortUnreachableInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->localPort, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->remotePort, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_UDP_PORT_UNREACHABLE_IND */

#ifndef EXCLUDE_CSR_BT_UDP_DEREGISTER_PORT_REQ
CsrSize CsrBtUdpDeregisterPortReqSizeof(void *msg)
{
    CsrBtUdpDeregisterPortReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_UDP_DEREGISTER_PORT_REQ */

#ifndef EXCLUDE_CSR_BT_UDP_DEREGISTER_PORT_REQ
CsrUint8 *CsrBtUdpDeregisterPortReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtUdpDeregisterPortReq *primitive;

    primitive = (CsrBtUdpDeregisterPortReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->port);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_UDP_DEREGISTER_PORT_REQ */

#ifndef EXCLUDE_CSR_BT_UDP_DEREGISTER_PORT_REQ
void *CsrBtUdpDeregisterPortReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtUdpDeregisterPortReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtUdpDeregisterPortReq *) CsrPmemZalloc(sizeof(CsrBtUdpDeregisterPortReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->port, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_UDP_DEREGISTER_PORT_REQ */

#ifndef EXCLUDE_CSR_BT_UDP_REGISTER_PORT_REQ
CsrSize CsrBtUdpRegisterPortReqSizeof(void *msg)
{
    CsrBtUdpRegisterPortReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_UDP_REGISTER_PORT_REQ */

#ifndef EXCLUDE_CSR_BT_UDP_REGISTER_PORT_REQ
CsrUint8 *CsrBtUdpRegisterPortReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtUdpRegisterPortReq *primitive;

    primitive = (CsrBtUdpRegisterPortReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->port);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->destinationQueue);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_UDP_REGISTER_PORT_REQ */

#ifndef EXCLUDE_CSR_BT_UDP_REGISTER_PORT_REQ
void *CsrBtUdpRegisterPortReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtUdpRegisterPortReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtUdpRegisterPortReq *) CsrPmemZalloc(sizeof(CsrBtUdpRegisterPortReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->port, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->destinationQueue, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_UDP_REGISTER_PORT_REQ */

#ifndef EXCLUDE_CSR_BT_UDP_DATA_REQ
CsrSize CsrBtUdpDataReqSizeof(void *msg)
{
    CsrBtUdpDataReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->udpDataLength * sizeof(CsrUint16)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_UDP_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_UDP_DATA_REQ
CsrUint8 *CsrBtUdpDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtUdpDataReq *primitive;

    primitive = (CsrBtUdpDataReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->typeOfService);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->timeToLive);
    CsrMemCpySer(buffer, length, ((const void *) primitive->destinationAddress), ((CsrSize) 2 * (2)));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->udpDataLength);
    CsrMemCpySer(buffer, length, &primitive->udpData, sizeof(primitive->udpData));
    if(primitive->udpData)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->udpData), ((CsrSize) (primitive->udpDataLength * sizeof(CsrUint16))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_UDP_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_UDP_DATA_REQ
void *CsrBtUdpDataReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtUdpDataReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtUdpDataReq *) CsrPmemZalloc(sizeof(CsrBtUdpDataReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->typeOfService, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->timeToLive, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->destinationAddress), buffer, &offset, ((CsrSize) 2 * (2)));
    CsrUint16Des((CsrUint16 *) &primitive->udpDataLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->udpData), buffer, &offset, ((CsrSize) sizeof(primitive->udpData)));
    if (primitive->udpDataLength)
    {
        primitive->udpData = CsrPmemZalloc(((CsrUint32) (primitive->udpDataLength * sizeof(CsrUint16))));
        CsrMemCpyDes(((void *) primitive->udpData), buffer, &offset, ((CsrSize) (primitive->udpDataLength * sizeof(CsrUint16))));
    }
    else
    {
        primitive->udpData = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_UDP_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_UDP_DATA_REQ
void CsrBtUdpDataReqSerFree(void *msg)
{
    CsrBtUdpDataReq *primitive;

    primitive = (CsrBtUdpDataReq *) msg;

    if(primitive->udpData != NULL)
    {
        CsrPmemFree(primitive->udpData);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_UDP_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_UDP_SHUTDOWN_IND
CsrSize CsrBtUdpShutdownIndSizeof(void *msg)
{
    CsrBtUdpShutdownInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_UDP_SHUTDOWN_IND */

#ifndef EXCLUDE_CSR_BT_UDP_SHUTDOWN_IND
CsrUint8 *CsrBtUdpShutdownIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtUdpShutdownInd *primitive;

    primitive = (CsrBtUdpShutdownInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_UDP_SHUTDOWN_IND */

#ifndef EXCLUDE_CSR_BT_UDP_SHUTDOWN_IND
void *CsrBtUdpShutdownIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtUdpShutdownInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtUdpShutdownInd *) CsrPmemZalloc(sizeof(CsrBtUdpShutdownInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_UDP_SHUTDOWN_IND */

#endif /* EXCLUDE_CSR_BT_UDP_MODULE */
