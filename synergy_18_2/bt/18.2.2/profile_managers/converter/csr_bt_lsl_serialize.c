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
#include "csr_bt_lsl_prim.h"

#ifndef EXCLUDE_CSR_BT_LSL_MODULE

void CsrBtLslPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_LSL_CONNECT_REQ
CsrSize CsrBtLslConnectReqSizeof(void *msg)
{
    CsrBtLslConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_LSL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_LSL_CONNECT_REQ
CsrUint8 *CsrBtLslConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtLslConnectReq *primitive;

    primitive = (CsrBtLslConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_LSL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_LSL_CONNECT_REQ
void *CsrBtLslConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtLslConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtLslConnectReq *) CsrPmemZalloc(sizeof(CsrBtLslConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_LSL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_LSL_DISCONNECT_REQ
CsrSize CsrBtLslDisconnectReqSizeof(void *msg)
{
    CsrBtLslDisconnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_LSL_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_LSL_DISCONNECT_REQ
CsrUint8 *CsrBtLslDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtLslDisconnectReq *primitive;

    primitive = (CsrBtLslDisconnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_LSL_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_LSL_DISCONNECT_REQ
void *CsrBtLslDisconnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtLslDisconnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtLslDisconnectReq *) CsrPmemZalloc(sizeof(CsrBtLslDisconnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_LSL_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_LSL_STATUS_IND
CsrSize CsrBtLslStatusIndSizeof(void *msg)
{
    CsrBtLslStatusInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_LSL_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_LSL_STATUS_IND
CsrUint8 *CsrBtLslStatusIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtLslStatusInd *primitive;

    primitive = (CsrBtLslStatusInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connected);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->reason);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxPayloadLength);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_LSL_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_LSL_STATUS_IND
void *CsrBtLslStatusIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtLslStatusInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtLslStatusInd *) CsrPmemZalloc(sizeof(CsrBtLslStatusInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connected, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->reason, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxPayloadLength, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_LSL_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_LSL_DATA_RES
CsrSize CsrBtLslDataResSizeof(void *msg)
{
    CsrBtLslDataRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_LSL_DATA_RES */

#ifndef EXCLUDE_CSR_BT_LSL_DATA_RES
CsrUint8 *CsrBtLslDataResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtLslDataRes *primitive;

    primitive = (CsrBtLslDataRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_LSL_DATA_RES */

#ifndef EXCLUDE_CSR_BT_LSL_DATA_RES
void *CsrBtLslDataResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtLslDataRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtLslDataRes *) CsrPmemZalloc(sizeof(CsrBtLslDataRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_LSL_DATA_RES */

#ifndef EXCLUDE_CSR_BT_LSL_DATA_IND
CsrSize CsrBtLslDataIndSizeof(void *msg)
{
    CsrBtLslDataInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->length * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_LSL_DATA_IND */

#ifndef EXCLUDE_CSR_BT_LSL_DATA_IND
CsrUint8 *CsrBtLslDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtLslDataInd *primitive;

    primitive = (CsrBtLslDataInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->length);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->length * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_LSL_DATA_IND */

#ifndef EXCLUDE_CSR_BT_LSL_DATA_IND
void *CsrBtLslDataIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtLslDataInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtLslDataInd *) CsrPmemZalloc(sizeof(CsrBtLslDataInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    CsrUint16Des((CsrUint16 *) &primitive->length, buffer, &offset);
    if (primitive->length)
    {
        primitive->payload = CsrPmemZalloc(((CsrUint32) (primitive->length * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->payload), buffer, &offset, ((CsrSize) (primitive->length * sizeof(CsrUint8))));
    }
    else
    {
        primitive->payload = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_LSL_DATA_IND */

#ifndef EXCLUDE_CSR_BT_LSL_DATA_IND
void CsrBtLslDataIndSerFree(void *msg)
{
    CsrBtLslDataInd *primitive;

    primitive = (CsrBtLslDataInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_LSL_DATA_IND */

#ifndef EXCLUDE_CSR_BT_LSL_DATA_REQ
CsrSize CsrBtLslDataReqSizeof(void *msg)
{
    CsrBtLslDataReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->length * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_LSL_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_LSL_DATA_REQ
CsrUint8 *CsrBtLslDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtLslDataReq *primitive;

    primitive = (CsrBtLslDataReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->length);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->length * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_LSL_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_LSL_DATA_REQ
void *CsrBtLslDataReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtLslDataReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtLslDataReq *) CsrPmemZalloc(sizeof(CsrBtLslDataReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->length, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    if (primitive->length)
    {
        primitive->payload = CsrPmemZalloc(((CsrUint32) (primitive->length * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->payload), buffer, &offset, ((CsrSize) (primitive->length * sizeof(CsrUint8))));
    }
    else
    {
        primitive->payload = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_LSL_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_LSL_DATA_REQ
void CsrBtLslDataReqSerFree(void *msg)
{
    CsrBtLslDataReq *primitive;

    primitive = (CsrBtLslDataReq *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_LSL_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_LSL_DATA_CFM
CsrSize CsrBtLslDataCfmSizeof(void *msg)
{
    CsrBtLslDataCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_LSL_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_LSL_DATA_CFM
CsrUint8 *CsrBtLslDataCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtLslDataCfm *primitive;

    primitive = (CsrBtLslDataCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_LSL_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_LSL_DATA_CFM
void *CsrBtLslDataCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtLslDataCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtLslDataCfm *) CsrPmemZalloc(sizeof(CsrBtLslDataCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_LSL_DATA_CFM */

#endif /* EXCLUDE_CSR_BT_LSL_MODULE */
