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
#include "csr_bt_ip_prim.h"

#ifndef EXCLUDE_CSR_BT_IP_MODULE

void CsrBtIpPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_IP_ADDRESS_REQ
CsrSize CsrBtIpAddressReqSizeof(void *msg)
{
    CsrBtIpAddressReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_IP_ADDRESS_REQ */

#ifndef EXCLUDE_CSR_BT_IP_ADDRESS_REQ
CsrUint8 *CsrBtIpAddressReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtIpAddressReq *primitive;

    primitive = (CsrBtIpAddressReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, ((const void *) primitive->ipAddress), ((CsrSize) 2 * (2)));
    CsrMemCpySer(buffer, length, ((const void *) primitive->subnetMask), ((CsrSize) 2 * (2)));
    CsrMemCpySer(buffer, length, ((const void *) primitive->gatewayIpAddress), ((CsrSize) 2 * (2)));

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_IP_ADDRESS_REQ */

#ifndef EXCLUDE_CSR_BT_IP_ADDRESS_REQ
void *CsrBtIpAddressReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtIpAddressReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtIpAddressReq *) CsrPmemZalloc(sizeof(CsrBtIpAddressReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->ipAddress), buffer, &offset, ((CsrSize) 2 * (2)));
    CsrMemCpyDes(((void *) primitive->subnetMask), buffer, &offset, ((CsrSize) 2 * (2)));
    CsrMemCpyDes(((void *) primitive->gatewayIpAddress), buffer, &offset, ((CsrSize) 2 * (2)));

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_IP_ADDRESS_REQ */

#ifndef EXCLUDE_CSR_BT_IP_SHUTDOWN_IND
CsrSize CsrBtIpShutdownIndSizeof(void *msg)
{
    CsrBtIpShutdownInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_IP_SHUTDOWN_IND */

#ifndef EXCLUDE_CSR_BT_IP_SHUTDOWN_IND
CsrUint8 *CsrBtIpShutdownIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtIpShutdownInd *primitive;

    primitive = (CsrBtIpShutdownInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_IP_SHUTDOWN_IND */

#ifndef EXCLUDE_CSR_BT_IP_SHUTDOWN_IND
void *CsrBtIpShutdownIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtIpShutdownInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtIpShutdownInd *) CsrPmemZalloc(sizeof(CsrBtIpShutdownInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_IP_SHUTDOWN_IND */

#ifndef EXCLUDE_CSR_BT_IP_DATA_IND
CsrSize CsrBtIpDataIndSizeof(void *msg)
{
    CsrBtIpDataInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->ipDataLength * sizeof(CsrUint16)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_IP_DATA_IND */

#ifndef EXCLUDE_CSR_BT_IP_DATA_IND
CsrUint8 *CsrBtIpDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtIpDataInd *primitive;

    primitive = (CsrBtIpDataInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ipHeaderLength);
    CsrMemCpySer(buffer, length, ((const void *) primitive->ipHeader), ((CsrSize) 2 * (32)));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ipDataLength);
    CsrMemCpySer(buffer, length, &primitive->ipData, sizeof(primitive->ipData));
    if(primitive->ipData)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->ipData), ((CsrSize) (primitive->ipDataLength * sizeof(CsrUint16))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_IP_DATA_IND */

#ifndef EXCLUDE_CSR_BT_IP_DATA_IND
void *CsrBtIpDataIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtIpDataInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtIpDataInd *) CsrPmemZalloc(sizeof(CsrBtIpDataInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->ipHeaderLength, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->ipHeader), buffer, &offset, ((CsrSize) 2 * (32)));
    CsrUint16Des((CsrUint16 *) &primitive->ipDataLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->ipData), buffer, &offset, ((CsrSize) sizeof(primitive->ipData)));
    if (primitive->ipDataLength)
    {
        primitive->ipData = CsrPmemZalloc(((CsrUint32) (primitive->ipDataLength * sizeof(CsrUint16))));
        CsrMemCpyDes(((void *) primitive->ipData), buffer, &offset, ((CsrSize) (primitive->ipDataLength * sizeof(CsrUint16))));
    }
    else
    {
        primitive->ipData = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_IP_DATA_IND */

#ifndef EXCLUDE_CSR_BT_IP_DATA_IND
void CsrBtIpDataIndSerFree(void *msg)
{
    CsrBtIpDataInd *primitive;

    primitive = (CsrBtIpDataInd *) msg;

    if(primitive->ipData != NULL)
    {
        CsrPmemFree(primitive->ipData);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_IP_DATA_IND */

#ifndef EXCLUDE_CSR_BT_IP_ADDRESS_IND
CsrSize CsrBtIpAddressIndSizeof(void *msg)
{
    CsrBtIpAddressInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_IP_ADDRESS_IND */

#ifndef EXCLUDE_CSR_BT_IP_ADDRESS_IND
CsrUint8 *CsrBtIpAddressIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtIpAddressInd *primitive;

    primitive = (CsrBtIpAddressInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, ((const void *) primitive->ipAddress), ((CsrSize) 2 * (2)));

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_IP_ADDRESS_IND */

#ifndef EXCLUDE_CSR_BT_IP_ADDRESS_IND
void *CsrBtIpAddressIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtIpAddressInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtIpAddressInd *) CsrPmemZalloc(sizeof(CsrBtIpAddressInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->ipAddress), buffer, &offset, ((CsrSize) 2 * (2)));

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_IP_ADDRESS_IND */

#ifndef EXCLUDE_CSR_BT_IP_DATA_REQ
CsrSize CsrBtIpDataReqSizeof(void *msg)
{
    CsrBtIpDataReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->ipDataLength * sizeof(CsrUint16)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_IP_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_IP_DATA_REQ
CsrUint8 *CsrBtIpDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtIpDataReq *primitive;

    primitive = (CsrBtIpDataReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->typeOfService);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->timeToLive);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->protocol);
    CsrMemCpySer(buffer, length, ((const void *) primitive->destinationAddress), ((CsrSize) 2 * (2)));
    CsrMemCpySer(buffer, length, ((const void *) primitive->sourceAddress), ((CsrSize) 2 * (2)));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ipDataLength);
    CsrMemCpySer(buffer, length, &primitive->ipData, sizeof(primitive->ipData));
    if(primitive->ipData)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->ipData), ((CsrSize) (primitive->ipDataLength * sizeof(CsrUint16))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_IP_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_IP_DATA_REQ
void *CsrBtIpDataReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtIpDataReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtIpDataReq *) CsrPmemZalloc(sizeof(CsrBtIpDataReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->typeOfService, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->timeToLive, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->protocol, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->destinationAddress), buffer, &offset, ((CsrSize) 2 * (2)));
    CsrMemCpyDes(((void *) primitive->sourceAddress), buffer, &offset, ((CsrSize) 2 * (2)));
    CsrUint16Des((CsrUint16 *) &primitive->ipDataLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->ipData), buffer, &offset, ((CsrSize) sizeof(primitive->ipData)));
    if (primitive->ipDataLength)
    {
        primitive->ipData = CsrPmemZalloc(((CsrUint32) (primitive->ipDataLength * sizeof(CsrUint16))));
        CsrMemCpyDes(((void *) primitive->ipData), buffer, &offset, ((CsrSize) (primitive->ipDataLength * sizeof(CsrUint16))));
    }
    else
    {
        primitive->ipData = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_IP_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_IP_DATA_REQ
void CsrBtIpDataReqSerFree(void *msg)
{
    CsrBtIpDataReq *primitive;

    primitive = (CsrBtIpDataReq *) msg;

    if(primitive->ipData != NULL)
    {
        CsrPmemFree(primitive->ipData);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_IP_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_IP_MAC_ADDRESS_REQ
CsrSize CsrBtIpMacAddressReqSizeof(void *msg)
{
    CsrBtIpMacAddressReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_IP_MAC_ADDRESS_REQ */

#ifndef EXCLUDE_CSR_BT_IP_MAC_ADDRESS_REQ
CsrUint8 *CsrBtIpMacAddressReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtIpMacAddressReq *primitive;

    primitive = (CsrBtIpMacAddressReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, ((const void *) primitive->macAddress), ((CsrSize) 2 * (3)));

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_IP_MAC_ADDRESS_REQ */

#ifndef EXCLUDE_CSR_BT_IP_MAC_ADDRESS_REQ
void *CsrBtIpMacAddressReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtIpMacAddressReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtIpMacAddressReq *) CsrPmemZalloc(sizeof(CsrBtIpMacAddressReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->macAddress), buffer, &offset, ((CsrSize) 2 * (3)));

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_IP_MAC_ADDRESS_REQ */

#ifndef EXCLUDE_CSR_BT_IP_SHUTDOWN_REQ
CsrSize CsrBtIpShutdownReqSizeof(void *msg)
{
    CsrBtIpShutdownReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_IP_SHUTDOWN_REQ */

#ifndef EXCLUDE_CSR_BT_IP_SHUTDOWN_REQ
CsrUint8 *CsrBtIpShutdownReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtIpShutdownReq *primitive;

    primitive = (CsrBtIpShutdownReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_IP_SHUTDOWN_REQ */

#ifndef EXCLUDE_CSR_BT_IP_SHUTDOWN_REQ
void *CsrBtIpShutdownReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtIpShutdownReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtIpShutdownReq *) CsrPmemZalloc(sizeof(CsrBtIpShutdownReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_IP_SHUTDOWN_REQ */

#endif /* EXCLUDE_CSR_BT_IP_MODULE */
