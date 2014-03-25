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
#include "csr_bt_icmp_prim.h"

#ifndef EXCLUDE_CSR_BT_ICMP_MODULE

void CsrBtIcmpPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_ICMP_PING_REQ
CsrSize CsrBtIcmpPingReqSizeof(void *msg)
{
    CsrBtIcmpPingReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_ICMP_PING_REQ */

#ifndef EXCLUDE_CSR_BT_ICMP_PING_REQ
CsrUint8 *CsrBtIcmpPingReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtIcmpPingReq *primitive;

    primitive = (CsrBtIcmpPingReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->applicationQueue);
    CsrMemCpySer(buffer, length, ((const void *) primitive->destinationAddress), ((CsrSize) 2 * (2)));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->dataLength);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_ICMP_PING_REQ */

#ifndef EXCLUDE_CSR_BT_ICMP_PING_REQ
void *CsrBtIcmpPingReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtIcmpPingReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtIcmpPingReq *) CsrPmemZalloc(sizeof(CsrBtIcmpPingReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->applicationQueue, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->destinationAddress), buffer, &offset, ((CsrSize) 2 * (2)));
    CsrUint16Des((CsrUint16 *) &primitive->dataLength, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_ICMP_PING_REQ */

#ifndef EXCLUDE_CSR_BT_ICMP_PING_CFM
CsrSize CsrBtIcmpPingCfmSizeof(void *msg)
{
    CsrBtIcmpPingCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_ICMP_PING_CFM */

#ifndef EXCLUDE_CSR_BT_ICMP_PING_CFM
CsrUint8 *CsrBtIcmpPingCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtIcmpPingCfm *primitive;

    primitive = (CsrBtIcmpPingCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->responseTime);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_ICMP_PING_CFM */

#ifndef EXCLUDE_CSR_BT_ICMP_PING_CFM
void *CsrBtIcmpPingCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtIcmpPingCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtIcmpPingCfm *) CsrPmemZalloc(sizeof(CsrBtIcmpPingCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->responseTime, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_ICMP_PING_CFM */

#ifndef EXCLUDE_CSR_BT_ICMP_DESTINATION_UNREACHABLE_REQ
CsrSize CsrBtIcmpDestinationUnreachableReqSizeof(void *msg)
{
    CsrBtIcmpDestinationUnreachableReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->ipDataLength * sizeof(CsrUint16)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_ICMP_DESTINATION_UNREACHABLE_REQ */

#ifndef EXCLUDE_CSR_BT_ICMP_DESTINATION_UNREACHABLE_REQ
CsrUint8 *CsrBtIcmpDestinationUnreachableReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtIcmpDestinationUnreachableReq *primitive;

    primitive = (CsrBtIcmpDestinationUnreachableReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->code);
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
#endif /* EXCLUDE_CSR_BT_ICMP_DESTINATION_UNREACHABLE_REQ */

#ifndef EXCLUDE_CSR_BT_ICMP_DESTINATION_UNREACHABLE_REQ
void *CsrBtIcmpDestinationUnreachableReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtIcmpDestinationUnreachableReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtIcmpDestinationUnreachableReq *) CsrPmemZalloc(sizeof(CsrBtIcmpDestinationUnreachableReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->code, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_ICMP_DESTINATION_UNREACHABLE_REQ */

#ifndef EXCLUDE_CSR_BT_ICMP_DESTINATION_UNREACHABLE_REQ
void CsrBtIcmpDestinationUnreachableReqSerFree(void *msg)
{
    CsrBtIcmpDestinationUnreachableReq *primitive;

    primitive = (CsrBtIcmpDestinationUnreachableReq *) msg;

    if(primitive->ipData != NULL)
    {
        CsrPmemFree(primitive->ipData);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_ICMP_DESTINATION_UNREACHABLE_REQ */

#ifndef EXCLUDE_CSR_BT_ICMP_PORT_UNREACHABLE_IND
CsrSize CsrBtIcmpPortUnreachableIndSizeof(void *msg)
{
    CsrBtIcmpPortUnreachableInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->ipDataLength * sizeof(CsrUint16)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_ICMP_PORT_UNREACHABLE_IND */

#ifndef EXCLUDE_CSR_BT_ICMP_PORT_UNREACHABLE_IND
CsrUint8 *CsrBtIcmpPortUnreachableIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtIcmpPortUnreachableInd *primitive;

    primitive = (CsrBtIcmpPortUnreachableInd *) msg;
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
#endif /* EXCLUDE_CSR_BT_ICMP_PORT_UNREACHABLE_IND */

#ifndef EXCLUDE_CSR_BT_ICMP_PORT_UNREACHABLE_IND
void *CsrBtIcmpPortUnreachableIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtIcmpPortUnreachableInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtIcmpPortUnreachableInd *) CsrPmemZalloc(sizeof(CsrBtIcmpPortUnreachableInd));

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
#endif /* EXCLUDE_CSR_BT_ICMP_PORT_UNREACHABLE_IND */

#ifndef EXCLUDE_CSR_BT_ICMP_PORT_UNREACHABLE_IND
void CsrBtIcmpPortUnreachableIndSerFree(void *msg)
{
    CsrBtIcmpPortUnreachableInd *primitive;

    primitive = (CsrBtIcmpPortUnreachableInd *) msg;

    if(primitive->ipData != NULL)
    {
        CsrPmemFree(primitive->ipData);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_ICMP_PORT_UNREACHABLE_IND */

#endif /* EXCLUDE_CSR_BT_ICMP_MODULE */
