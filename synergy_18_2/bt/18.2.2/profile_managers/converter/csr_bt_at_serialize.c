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
#include "csr_bt_at_prim.h"

#ifndef EXCLUDE_CSR_BT_AT_MODULE

void CsrBtAtPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_AT_DG_CONNECT_IND
CsrSize CsrBtAtDgConnectIndSizeof(void *msg)
{
    CsrBtAtDgConnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_CONNECT_IND
CsrUint8 *CsrBtAtDgConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgConnectInd *primitive;

    primitive = (CsrBtAtDgConnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serverChannel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->profileMaxFrameSize);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_CONNECT_IND
void *CsrBtAtDgConnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgConnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgConnectInd *) CsrPmemZalloc(sizeof(CsrBtAtDgConnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->serverChannel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->profileMaxFrameSize, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_D_IND
CsrSize CsrBtAtDgAtcDIndSizeof(void *msg)
{
    CsrBtAtDgAtcDInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_D_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_D_IND
CsrUint8 *CsrBtAtDgAtcDIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcDInd *primitive;

    primitive = (CsrBtAtDgAtcDInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->value);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_D_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_D_IND
void *CsrBtAtDgAtcDIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcDInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcDInd *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcDInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->payloadLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    CsrUint8Des((CsrUint8 *) &primitive->value, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_D_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_D_IND
void CsrBtAtDgAtcDIndSerFree(void *msg)
{
    CsrBtAtDgAtcDInd *primitive;

    primitive = (CsrBtAtDgAtcDInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_D_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_E_IND
CsrSize CsrBtAtDgAtcEIndSizeof(void *msg)
{
    CsrBtAtDgAtcEInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_E_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_E_IND
CsrUint8 *CsrBtAtDgAtcEIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcEInd *primitive;

    primitive = (CsrBtAtDgAtcEInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->value);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_E_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_E_IND
void *CsrBtAtDgAtcEIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcEInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcEInd *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcEInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->value, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_E_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_UNKNOWN_EXTENDED_CMD_RES
CsrSize CsrBtAtDgAtcUnknownExtendedCmdResSizeof(void *msg)
{
    CsrBtAtDgAtcUnknownExtendedCmdRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_UNKNOWN_EXTENDED_CMD_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_UNKNOWN_EXTENDED_CMD_RES
CsrUint8 *CsrBtAtDgAtcUnknownExtendedCmdResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcUnknownExtendedCmdRes *primitive;

    primitive = (CsrBtAtDgAtcUnknownExtendedCmdRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_UNKNOWN_EXTENDED_CMD_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_UNKNOWN_EXTENDED_CMD_RES
void *CsrBtAtDgAtcUnknownExtendedCmdResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcUnknownExtendedCmdRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcUnknownExtendedCmdRes *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcUnknownExtendedCmdRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_UNKNOWN_EXTENDED_CMD_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_UNKNOWN_EXTENDED_CMD_RES
void CsrBtAtDgAtcUnknownExtendedCmdResSerFree(void *msg)
{
    CsrBtAtDgAtcUnknownExtendedCmdRes *primitive;

    primitive = (CsrBtAtDgAtcUnknownExtendedCmdRes *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_UNKNOWN_EXTENDED_CMD_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S8Q_IND
CsrSize CsrBtAtDgAtcS8qIndSizeof(void *msg)
{
    CsrBtAtDgAtcS8qInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S8Q_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S8Q_IND
CsrUint8 *CsrBtAtDgAtcS8qIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcS8qInd *primitive;

    primitive = (CsrBtAtDgAtcS8qInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S8Q_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S8Q_IND
void *CsrBtAtDgAtcS8qIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcS8qInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcS8qInd *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcS8qInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S8Q_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_PORTNEG_IND
CsrSize CsrBtAtDgPortnegIndSizeof(void *msg)
{
    CsrBtAtDgPortnegInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_PORTNEG_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_PORTNEG_IND
CsrUint8 *CsrBtAtDgPortnegIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgPortnegInd *primitive;

    primitive = (CsrBtAtDgPortnegInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
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
#endif /* EXCLUDE_CSR_BT_AT_DG_PORTNEG_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_PORTNEG_IND
void *CsrBtAtDgPortnegIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgPortnegInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgPortnegInd *) CsrPmemZalloc(sizeof(CsrBtAtDgPortnegInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_AT_DG_PORTNEG_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_ABORT_IND
CsrSize CsrBtAtDgAtcAbortIndSizeof(void *msg)
{
    CsrBtAtDgAtcAbortInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_ABORT_IND
CsrUint8 *CsrBtAtDgAtcAbortIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcAbortInd *primitive;

    primitive = (CsrBtAtDgAtcAbortInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_ABORT_IND
void *CsrBtAtDgAtcAbortIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcAbortInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcAbortInd *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcAbortInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_M_IND
CsrSize CsrBtAtDgAtcMIndSizeof(void *msg)
{
    CsrBtAtDgAtcMInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_M_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_M_IND
CsrUint8 *CsrBtAtDgAtcMIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcMInd *primitive;

    primitive = (CsrBtAtDgAtcMInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->value);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_M_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_M_IND
void *CsrBtAtDgAtcMIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcMInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcMInd *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcMInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->value, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_M_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_DATA_REQ
CsrSize CsrBtAtDgDataReqSizeof(void *msg)
{
    CsrBtAtDgDataReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_AT_DG_DATA_REQ
CsrUint8 *CsrBtAtDgDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgDataReq *primitive;

    primitive = (CsrBtAtDgDataReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_AT_DG_DATA_REQ
void *CsrBtAtDgDataReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgDataReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgDataReq *) CsrPmemZalloc(sizeof(CsrBtAtDgDataReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_AT_DG_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_AT_DG_DATA_REQ
void CsrBtAtDgDataReqSerFree(void *msg)
{
    CsrBtAtDgDataReq *primitive;

    primitive = (CsrBtAtDgDataReq *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AT_DG_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_AT_DG_DISCONNECT_REQ
CsrSize CsrBtAtDgDisconnectReqSizeof(void *msg)
{
    CsrBtAtDgDisconnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_AT_DG_DISCONNECT_REQ
CsrUint8 *CsrBtAtDgDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgDisconnectReq *primitive;

    primitive = (CsrBtAtDgDisconnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_AT_DG_DISCONNECT_REQ
void *CsrBtAtDgDisconnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgDisconnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgDisconnectReq *) CsrPmemZalloc(sizeof(CsrBtAtDgDisconnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_AT_DG_CONTROL_REQ
CsrSize CsrBtAtDgControlReqSizeof(void *msg)
{
    CsrBtAtDgControlReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_CONTROL_REQ */

#ifndef EXCLUDE_CSR_BT_AT_DG_CONTROL_REQ
CsrUint8 *CsrBtAtDgControlReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgControlReq *primitive;

    primitive = (CsrBtAtDgControlReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->modemstatus);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->break_signal);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_CONTROL_REQ */

#ifndef EXCLUDE_CSR_BT_AT_DG_CONTROL_REQ
void *CsrBtAtDgControlReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgControlReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgControlReq *) CsrPmemZalloc(sizeof(CsrBtAtDgControlReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->modemstatus, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->break_signal, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_CONTROL_REQ */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S6Q_RES
CsrSize CsrBtAtDgAtcS6qResSizeof(void *msg)
{
    CsrBtAtDgAtcS6qRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S6Q_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S6Q_RES
CsrUint8 *CsrBtAtDgAtcS6qResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcS6qRes *primitive;

    primitive = (CsrBtAtDgAtcS6qRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->value);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S6Q_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S6Q_RES
void *CsrBtAtDgAtcS6qResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcS6qRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcS6qRes *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcS6qRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->value, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S6Q_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S0_IND
CsrSize CsrBtAtDgAtcS0IndSizeof(void *msg)
{
    CsrBtAtDgAtcS0Ind *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S0_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S0_IND
CsrUint8 *CsrBtAtDgAtcS0IndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcS0Ind *primitive;

    primitive = (CsrBtAtDgAtcS0Ind *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->value);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S0_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S0_IND
void *CsrBtAtDgAtcS0IndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcS0Ind *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcS0Ind *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcS0Ind));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->value, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S0_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S7_IND
CsrSize CsrBtAtDgAtcS7IndSizeof(void *msg)
{
    CsrBtAtDgAtcS7Ind *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S7_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S7_IND
CsrUint8 *CsrBtAtDgAtcS7IndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcS7Ind *primitive;

    primitive = (CsrBtAtDgAtcS7Ind *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->value);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S7_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S7_IND
void *CsrBtAtDgAtcS7IndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcS7Ind *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcS7Ind *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcS7Ind));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->value, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S7_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_DATA_CFM
CsrSize CsrBtAtDgDataCfmSizeof(void *msg)
{
    CsrBtAtDgDataCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_AT_DG_DATA_CFM
CsrUint8 *CsrBtAtDgDataCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgDataCfm *primitive;

    primitive = (CsrBtAtDgDataCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_AT_DG_DATA_CFM
void *CsrBtAtDgDataCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgDataCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgDataCfm *) CsrPmemZalloc(sizeof(CsrBtAtDgDataCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S3_IND
CsrSize CsrBtAtDgAtcS3IndSizeof(void *msg)
{
    CsrBtAtDgAtcS3Ind *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S3_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S3_IND
CsrUint8 *CsrBtAtDgAtcS3IndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcS3Ind *primitive;

    primitive = (CsrBtAtDgAtcS3Ind *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->value);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S3_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S3_IND
void *CsrBtAtDgAtcS3IndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcS3Ind *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcS3Ind *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcS3Ind));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->value, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S3_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_AND_D_IND
CsrSize CsrBtAtDgAtcAndDIndSizeof(void *msg)
{
    CsrBtAtDgAtcAndDInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_AND_D_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_AND_D_IND
CsrUint8 *CsrBtAtDgAtcAndDIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcAndDInd *primitive;

    primitive = (CsrBtAtDgAtcAndDInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->value);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_AND_D_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_AND_D_IND
void *CsrBtAtDgAtcAndDIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcAndDInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcAndDInd *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcAndDInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->value, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_AND_D_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_Q_IND
CsrSize CsrBtAtDgAtcQIndSizeof(void *msg)
{
    CsrBtAtDgAtcQInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_Q_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_Q_IND
CsrUint8 *CsrBtAtDgAtcQIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcQInd *primitive;

    primitive = (CsrBtAtDgAtcQInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->value);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_Q_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_Q_IND
void *CsrBtAtDgAtcQIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcQInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcQInd *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcQInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->value, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_Q_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S6_IND
CsrSize CsrBtAtDgAtcS6IndSizeof(void *msg)
{
    CsrBtAtDgAtcS6Ind *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S6_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S6_IND
CsrUint8 *CsrBtAtDgAtcS6IndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcS6Ind *primitive;

    primitive = (CsrBtAtDgAtcS6Ind *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->value);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S6_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S6_IND
void *CsrBtAtDgAtcS6IndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcS6Ind *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcS6Ind *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcS6Ind));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->value, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S6_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S7Q_IND
CsrSize CsrBtAtDgAtcS7qIndSizeof(void *msg)
{
    CsrBtAtDgAtcS7qInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S7Q_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S7Q_IND
CsrUint8 *CsrBtAtDgAtcS7qIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcS7qInd *primitive;

    primitive = (CsrBtAtDgAtcS7qInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S7Q_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S7Q_IND
void *CsrBtAtDgAtcS7qIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcS7qInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcS7qInd *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcS7qInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S7Q_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_AND_F_IND
CsrSize CsrBtAtDgAtcAndFIndSizeof(void *msg)
{
    CsrBtAtDgAtcAndFInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_AND_F_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_AND_F_IND
CsrUint8 *CsrBtAtDgAtcAndFIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcAndFInd *primitive;

    primitive = (CsrBtAtDgAtcAndFInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->value);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_AND_F_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_AND_F_IND
void *CsrBtAtDgAtcAndFIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcAndFInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcAndFInd *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcAndFInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->value, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_AND_F_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S3Q_IND
CsrSize CsrBtAtDgAtcS3qIndSizeof(void *msg)
{
    CsrBtAtDgAtcS3qInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S3Q_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S3Q_IND
CsrUint8 *CsrBtAtDgAtcS3qIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcS3qInd *primitive;

    primitive = (CsrBtAtDgAtcS3qInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S3Q_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S3Q_IND
void *CsrBtAtDgAtcS3qIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcS3qInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcS3qInd *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcS3qInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S3Q_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_GMI_IND
CsrSize CsrBtAtDgAtcGmiIndSizeof(void *msg)
{
    CsrBtAtDgAtcGmiInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_GMI_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_GMI_IND
CsrUint8 *CsrBtAtDgAtcGmiIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcGmiInd *primitive;

    primitive = (CsrBtAtDgAtcGmiInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_GMI_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_GMI_IND
void *CsrBtAtDgAtcGmiIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcGmiInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcGmiInd *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcGmiInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_GMI_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S4Q_IND
CsrSize CsrBtAtDgAtcS4qIndSizeof(void *msg)
{
    CsrBtAtDgAtcS4qInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S4Q_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S4Q_IND
CsrUint8 *CsrBtAtDgAtcS4qIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcS4qInd *primitive;

    primitive = (CsrBtAtDgAtcS4qInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S4Q_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S4Q_IND
void *CsrBtAtDgAtcS4qIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcS4qInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcS4qInd *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcS4qInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S4Q_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_L_IND
CsrSize CsrBtAtDgAtcLIndSizeof(void *msg)
{
    CsrBtAtDgAtcLInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_L_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_L_IND
CsrUint8 *CsrBtAtDgAtcLIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcLInd *primitive;

    primitive = (CsrBtAtDgAtcLInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->value);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_L_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_L_IND
void *CsrBtAtDgAtcLIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcLInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcLInd *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcLInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->value, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_L_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_GMR_RES
CsrSize CsrBtAtDgAtcGmrResSizeof(void *msg)
{
    CsrBtAtDgAtcGmrRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_GMR_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_GMR_RES
CsrUint8 *CsrBtAtDgAtcGmrResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcGmrRes *primitive;

    primitive = (CsrBtAtDgAtcGmrRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_GMR_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_GMR_RES
void *CsrBtAtDgAtcGmrResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcGmrRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcGmrRes *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcGmrRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_GMR_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_GMR_RES
void CsrBtAtDgAtcGmrResSerFree(void *msg)
{
    CsrBtAtDgAtcGmrRes *primitive;

    primitive = (CsrBtAtDgAtcGmrRes *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_GMR_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S5_IND
CsrSize CsrBtAtDgAtcS5IndSizeof(void *msg)
{
    CsrBtAtDgAtcS5Ind *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S5_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S5_IND
CsrUint8 *CsrBtAtDgAtcS5IndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcS5Ind *primitive;

    primitive = (CsrBtAtDgAtcS5Ind *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->value);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S5_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S5_IND
void *CsrBtAtDgAtcS5IndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcS5Ind *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcS5Ind *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcS5Ind));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->value, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S5_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S0Q_IND
CsrSize CsrBtAtDgAtcS0qIndSizeof(void *msg)
{
    CsrBtAtDgAtcS0qInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S0Q_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S0Q_IND
CsrUint8 *CsrBtAtDgAtcS0qIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcS0qInd *primitive;

    primitive = (CsrBtAtDgAtcS0qInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S0Q_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S0Q_IND
void *CsrBtAtDgAtcS0qIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcS0qInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcS0qInd *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcS0qInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S0Q_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S10Q_IND
CsrSize CsrBtAtDgAtcS10qIndSizeof(void *msg)
{
    CsrBtAtDgAtcS10qInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S10Q_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S10Q_IND
CsrUint8 *CsrBtAtDgAtcS10qIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcS10qInd *primitive;

    primitive = (CsrBtAtDgAtcS10qInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S10Q_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S10Q_IND
void *CsrBtAtDgAtcS10qIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcS10qInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcS10qInd *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcS10qInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S10Q_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_V_IND
CsrSize CsrBtAtDgAtcVIndSizeof(void *msg)
{
    CsrBtAtDgAtcVInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_V_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_V_IND
CsrUint8 *CsrBtAtDgAtcVIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcVInd *primitive;

    primitive = (CsrBtAtDgAtcVInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->value);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_V_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_V_IND
void *CsrBtAtDgAtcVIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcVInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcVInd *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcVInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->value, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_V_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_O_IND
CsrSize CsrBtAtDgAtcOIndSizeof(void *msg)
{
    CsrBtAtDgAtcOInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_O_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_O_IND
CsrUint8 *CsrBtAtDgAtcOIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcOInd *primitive;

    primitive = (CsrBtAtDgAtcOInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->value);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_O_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_O_IND
void *CsrBtAtDgAtcOIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcOInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcOInd *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcOInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->value, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_O_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_A_IND
CsrSize CsrBtAtDgAtcAIndSizeof(void *msg)
{
    CsrBtAtDgAtcAInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_A_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_A_IND
CsrUint8 *CsrBtAtDgAtcAIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcAInd *primitive;

    primitive = (CsrBtAtDgAtcAInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_A_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_A_IND
void *CsrBtAtDgAtcAIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcAInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcAInd *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcAInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_A_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_GMR_IND
CsrSize CsrBtAtDgAtcGmrIndSizeof(void *msg)
{
    CsrBtAtDgAtcGmrInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_GMR_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_GMR_IND
CsrUint8 *CsrBtAtDgAtcGmrIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcGmrInd *primitive;

    primitive = (CsrBtAtDgAtcGmrInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_GMR_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_GMR_IND
void *CsrBtAtDgAtcGmrIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcGmrInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcGmrInd *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcGmrInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_GMR_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_DISCONNECT_IND
CsrSize CsrBtAtDgDisconnectIndSizeof(void *msg)
{
    CsrBtAtDgDisconnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_DISCONNECT_IND
CsrUint8 *CsrBtAtDgDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgDisconnectInd *primitive;

    primitive = (CsrBtAtDgDisconnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->localTerminated);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_DISCONNECT_IND
void *CsrBtAtDgDisconnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgDisconnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgDisconnectInd *) CsrPmemZalloc(sizeof(CsrBtAtDgDisconnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->localTerminated, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S4_IND
CsrSize CsrBtAtDgAtcS4IndSizeof(void *msg)
{
    CsrBtAtDgAtcS4Ind *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S4_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S4_IND
CsrUint8 *CsrBtAtDgAtcS4IndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcS4Ind *primitive;

    primitive = (CsrBtAtDgAtcS4Ind *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->value);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S4_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S4_IND
void *CsrBtAtDgAtcS4IndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcS4Ind *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcS4Ind *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcS4Ind));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->value, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S4_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_GMM_RES
CsrSize CsrBtAtDgAtcGmmResSizeof(void *msg)
{
    CsrBtAtDgAtcGmmRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_GMM_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_GMM_RES
CsrUint8 *CsrBtAtDgAtcGmmResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcGmmRes *primitive;

    primitive = (CsrBtAtDgAtcGmmRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_GMM_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_GMM_RES
void *CsrBtAtDgAtcGmmResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcGmmRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcGmmRes *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcGmmRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_GMM_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_GMM_RES
void CsrBtAtDgAtcGmmResSerFree(void *msg)
{
    CsrBtAtDgAtcGmmRes *primitive;

    primitive = (CsrBtAtDgAtcGmmRes *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_GMM_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_DATA_PATH_STATUS_IND
CsrSize CsrBtAtDgDataPathStatusIndSizeof(void *msg)
{
    CsrBtAtDgDataPathStatusInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_DATA_PATH_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_DATA_PATH_STATUS_IND
CsrUint8 *CsrBtAtDgDataPathStatusIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgDataPathStatusInd *primitive;

    primitive = (CsrBtAtDgDataPathStatusInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->dgInstanceQueue);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_DATA_PATH_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_DATA_PATH_STATUS_IND
void *CsrBtAtDgDataPathStatusIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgDataPathStatusInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgDataPathStatusInd *) CsrPmemZalloc(sizeof(CsrBtAtDgDataPathStatusInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->dgInstanceQueue, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_DATA_PATH_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_DATA_RES
CsrSize CsrBtAtDgDataResSizeof(void *msg)
{
    CsrBtAtDgDataRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_DATA_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_DATA_RES
CsrUint8 *CsrBtAtDgDataResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgDataRes *primitive;

    primitive = (CsrBtAtDgDataRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_DATA_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_DATA_RES
void *CsrBtAtDgDataResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgDataRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgDataRes *) CsrPmemZalloc(sizeof(CsrBtAtDgDataRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_DATA_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S7Q_RES
CsrSize CsrBtAtDgAtcS7qResSizeof(void *msg)
{
    CsrBtAtDgAtcS7qRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S7Q_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S7Q_RES
CsrUint8 *CsrBtAtDgAtcS7qResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcS7qRes *primitive;

    primitive = (CsrBtAtDgAtcS7qRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->value);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S7Q_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S7Q_RES
void *CsrBtAtDgAtcS7qResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcS7qRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcS7qRes *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcS7qRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->value, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S7Q_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_Z_IND
CsrSize CsrBtAtDgAtcZIndSizeof(void *msg)
{
    CsrBtAtDgAtcZInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_Z_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_Z_IND
CsrUint8 *CsrBtAtDgAtcZIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcZInd *primitive;

    primitive = (CsrBtAtDgAtcZInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->value);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_Z_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_Z_IND
void *CsrBtAtDgAtcZIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcZInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcZInd *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcZInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->value, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_Z_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S0Q_RES
CsrSize CsrBtAtDgAtcS0qResSizeof(void *msg)
{
    CsrBtAtDgAtcS0qRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S0Q_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S0Q_RES
CsrUint8 *CsrBtAtDgAtcS0qResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcS0qRes *primitive;

    primitive = (CsrBtAtDgAtcS0qRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->value);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S0Q_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S0Q_RES
void *CsrBtAtDgAtcS0qResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcS0qRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcS0qRes *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcS0qRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->value, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S0Q_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S5Q_RES
CsrSize CsrBtAtDgAtcS5qResSizeof(void *msg)
{
    CsrBtAtDgAtcS5qRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S5Q_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S5Q_RES
CsrUint8 *CsrBtAtDgAtcS5qResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcS5qRes *primitive;

    primitive = (CsrBtAtDgAtcS5qRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->value);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S5Q_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S5Q_RES
void *CsrBtAtDgAtcS5qResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcS5qRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcS5qRes *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcS5qRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->value, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S5Q_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S4Q_RES
CsrSize CsrBtAtDgAtcS4qResSizeof(void *msg)
{
    CsrBtAtDgAtcS4qRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S4Q_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S4Q_RES
CsrUint8 *CsrBtAtDgAtcS4qResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcS4qRes *primitive;

    primitive = (CsrBtAtDgAtcS4qRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->value);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S4Q_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S4Q_RES
void *CsrBtAtDgAtcS4qResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcS4qRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcS4qRes *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcS4qRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->value, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S4Q_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_P_IND
CsrSize CsrBtAtDgAtcPIndSizeof(void *msg)
{
    CsrBtAtDgAtcPInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_P_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_P_IND
CsrUint8 *CsrBtAtDgAtcPIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcPInd *primitive;

    primitive = (CsrBtAtDgAtcPInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_P_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_P_IND
void *CsrBtAtDgAtcPIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcPInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcPInd *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcPInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_P_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_DEACTIVATE_CFM
CsrSize CsrBtAtDgDeactivateCfmSizeof(void *msg)
{
    CsrBtAtDgDeactivateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_AT_DG_DEACTIVATE_CFM
CsrUint8 *CsrBtAtDgDeactivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgDeactivateCfm *primitive;

    primitive = (CsrBtAtDgDeactivateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_AT_DG_DEACTIVATE_CFM
void *CsrBtAtDgDeactivateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgDeactivateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgDeactivateCfm *) CsrPmemZalloc(sizeof(CsrBtAtDgDeactivateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_GCAP_IND
CsrSize CsrBtAtDgAtcGcapIndSizeof(void *msg)
{
    CsrBtAtDgAtcGcapInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_GCAP_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_GCAP_IND
CsrUint8 *CsrBtAtDgAtcGcapIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcGcapInd *primitive;

    primitive = (CsrBtAtDgAtcGcapInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_GCAP_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_GCAP_IND
void *CsrBtAtDgAtcGcapIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcGcapInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcGcapInd *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcGcapInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_GCAP_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_RESULT_CODE_RES
CsrSize CsrBtAtDgAtcResultCodeResSizeof(void *msg)
{
    CsrBtAtDgAtcResultCodeRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_RESULT_CODE_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_RESULT_CODE_RES
CsrUint8 *CsrBtAtDgAtcResultCodeResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcResultCodeRes *primitive;

    primitive = (CsrBtAtDgAtcResultCodeRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->atResult);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_RESULT_CODE_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_RESULT_CODE_RES
void *CsrBtAtDgAtcResultCodeResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcResultCodeRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcResultCodeRes *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcResultCodeRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->atResult, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_RESULT_CODE_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_DATA_IND
CsrSize CsrBtAtDgDataIndSizeof(void *msg)
{
    CsrBtAtDgDataInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_DATA_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_DATA_IND
CsrUint8 *CsrBtAtDgDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgDataInd *primitive;

    primitive = (CsrBtAtDgDataInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_DATA_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_DATA_IND
void *CsrBtAtDgDataIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgDataInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgDataInd *) CsrPmemZalloc(sizeof(CsrBtAtDgDataInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_AT_DG_DATA_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_DATA_IND
void CsrBtAtDgDataIndSerFree(void *msg)
{
    CsrBtAtDgDataInd *primitive;

    primitive = (CsrBtAtDgDataInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AT_DG_DATA_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_AND_C_IND
CsrSize CsrBtAtDgAtcAndCIndSizeof(void *msg)
{
    CsrBtAtDgAtcAndCInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_AND_C_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_AND_C_IND
CsrUint8 *CsrBtAtDgAtcAndCIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcAndCInd *primitive;

    primitive = (CsrBtAtDgAtcAndCInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->value);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_AND_C_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_AND_C_IND
void *CsrBtAtDgAtcAndCIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcAndCInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcAndCInd *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcAndCInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->value, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_AND_C_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_PORTNEG_RES
CsrSize CsrBtAtDgPortnegResSizeof(void *msg)
{
    CsrBtAtDgPortnegRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_PORTNEG_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_PORTNEG_RES
CsrUint8 *CsrBtAtDgPortnegResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgPortnegRes *primitive;

    primitive = (CsrBtAtDgPortnegRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
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
#endif /* EXCLUDE_CSR_BT_AT_DG_PORTNEG_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_PORTNEG_RES
void *CsrBtAtDgPortnegResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgPortnegRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgPortnegRes *) CsrPmemZalloc(sizeof(CsrBtAtDgPortnegRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_AT_DG_PORTNEG_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S8Q_RES
CsrSize CsrBtAtDgAtcS8qResSizeof(void *msg)
{
    CsrBtAtDgAtcS8qRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S8Q_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S8Q_RES
CsrUint8 *CsrBtAtDgAtcS8qResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcS8qRes *primitive;

    primitive = (CsrBtAtDgAtcS8qRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->value);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S8Q_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S8Q_RES
void *CsrBtAtDgAtcS8qResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcS8qRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcS8qRes *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcS8qRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->value, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S8Q_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_FACTORY_SETTING_RES
CsrSize CsrBtAtDgAtcFactorySettingResSizeof(void *msg)
{
    CsrBtAtDgAtcFactorySettingRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_FACTORY_SETTING_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_FACTORY_SETTING_RES
CsrUint8 *CsrBtAtDgAtcFactorySettingResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcFactorySettingRes *primitive;

    primitive = (CsrBtAtDgAtcFactorySettingRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->parameterSetting.s3Value);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->parameterSetting.qValue);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->parameterSetting.vValue);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->parameterSetting.s4Value);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->parameterSetting.eValue);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->parameterSetting.s5Value);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->parameterSetting.andDValue);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->parameterSetting.s0Value);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->parameterSetting.s6Value);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->parameterSetting.s7Value);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->parameterSetting.s8Value);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->parameterSetting.s10Value);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_FACTORY_SETTING_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_FACTORY_SETTING_RES
void *CsrBtAtDgAtcFactorySettingResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcFactorySettingRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcFactorySettingRes *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcFactorySettingRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->parameterSetting.s3Value, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->parameterSetting.qValue, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->parameterSetting.vValue, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->parameterSetting.s4Value, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->parameterSetting.eValue, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->parameterSetting.s5Value, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->parameterSetting.andDValue, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->parameterSetting.s0Value, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->parameterSetting.s6Value, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->parameterSetting.s7Value, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->parameterSetting.s8Value, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->parameterSetting.s10Value, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_FACTORY_SETTING_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_X_IND
CsrSize CsrBtAtDgAtcXIndSizeof(void *msg)
{
    CsrBtAtDgAtcXInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_X_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_X_IND
CsrUint8 *CsrBtAtDgAtcXIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcXInd *primitive;

    primitive = (CsrBtAtDgAtcXInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->value);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_X_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_X_IND
void *CsrBtAtDgAtcXIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcXInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcXInd *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcXInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->value, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_X_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_GCAP_RES
CsrSize CsrBtAtDgAtcGcapResSizeof(void *msg)
{
    CsrBtAtDgAtcGcapRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_GCAP_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_GCAP_RES
CsrUint8 *CsrBtAtDgAtcGcapResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcGcapRes *primitive;

    primitive = (CsrBtAtDgAtcGcapRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_GCAP_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_GCAP_RES
void *CsrBtAtDgAtcGcapResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcGcapRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcGcapRes *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcGcapRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_GCAP_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_GCAP_RES
void CsrBtAtDgAtcGcapResSerFree(void *msg)
{
    CsrBtAtDgAtcGcapRes *primitive;

    primitive = (CsrBtAtDgAtcGcapRes *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_GCAP_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_GMI_RES
CsrSize CsrBtAtDgAtcGmiResSizeof(void *msg)
{
    CsrBtAtDgAtcGmiRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_GMI_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_GMI_RES
CsrUint8 *CsrBtAtDgAtcGmiResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcGmiRes *primitive;

    primitive = (CsrBtAtDgAtcGmiRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_GMI_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_GMI_RES
void *CsrBtAtDgAtcGmiResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcGmiRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcGmiRes *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcGmiRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_GMI_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_GMI_RES
void CsrBtAtDgAtcGmiResSerFree(void *msg)
{
    CsrBtAtDgAtcGmiRes *primitive;

    primitive = (CsrBtAtDgAtcGmiRes *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_GMI_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_T_IND
CsrSize CsrBtAtDgAtcTIndSizeof(void *msg)
{
    CsrBtAtDgAtcTInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_T_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_T_IND
CsrUint8 *CsrBtAtDgAtcTIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcTInd *primitive;

    primitive = (CsrBtAtDgAtcTInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_T_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_T_IND
void *CsrBtAtDgAtcTIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcTInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcTInd *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcTInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_T_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ACTIVATE_REQ
CsrSize CsrBtAtDgActivateReqSizeof(void *msg)
{
    CsrBtAtDgActivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_AT_DG_ACTIVATE_REQ
CsrUint8 *CsrBtAtDgActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgActivateReq *primitive;

    primitive = (CsrBtAtDgActivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->timeout);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_AT_DG_ACTIVATE_REQ
void *CsrBtAtDgActivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgActivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgActivateReq *) CsrPmemZalloc(sizeof(CsrBtAtDgActivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->timeout, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S10_IND
CsrSize CsrBtAtDgAtcS10IndSizeof(void *msg)
{
    CsrBtAtDgAtcS10Ind *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S10_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S10_IND
CsrUint8 *CsrBtAtDgAtcS10IndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcS10Ind *primitive;

    primitive = (CsrBtAtDgAtcS10Ind *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->value);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S10_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S10_IND
void *CsrBtAtDgAtcS10IndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcS10Ind *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcS10Ind *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcS10Ind));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->value, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S10_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S6Q_IND
CsrSize CsrBtAtDgAtcS6qIndSizeof(void *msg)
{
    CsrBtAtDgAtcS6qInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S6Q_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S6Q_IND
CsrUint8 *CsrBtAtDgAtcS6qIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcS6qInd *primitive;

    primitive = (CsrBtAtDgAtcS6qInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S6Q_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S6Q_IND
void *CsrBtAtDgAtcS6qIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcS6qInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcS6qInd *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcS6qInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S6Q_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S3Q_RES
CsrSize CsrBtAtDgAtcS3qResSizeof(void *msg)
{
    CsrBtAtDgAtcS3qRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S3Q_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S3Q_RES
CsrUint8 *CsrBtAtDgAtcS3qResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcS3qRes *primitive;

    primitive = (CsrBtAtDgAtcS3qRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->value);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S3Q_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S3Q_RES
void *CsrBtAtDgAtcS3qResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcS3qRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcS3qRes *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcS3qRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->value, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S3Q_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S8_IND
CsrSize CsrBtAtDgAtcS8IndSizeof(void *msg)
{
    CsrBtAtDgAtcS8Ind *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S8_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S8_IND
CsrUint8 *CsrBtAtDgAtcS8IndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcS8Ind *primitive;

    primitive = (CsrBtAtDgAtcS8Ind *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->value);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S8_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S8_IND
void *CsrBtAtDgAtcS8IndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcS8Ind *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcS8Ind *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcS8Ind));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->value, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S8_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_H_IND
CsrSize CsrBtAtDgAtcHIndSizeof(void *msg)
{
    CsrBtAtDgAtcHInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_H_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_H_IND
CsrUint8 *CsrBtAtDgAtcHIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcHInd *primitive;

    primitive = (CsrBtAtDgAtcHInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_H_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_H_IND
void *CsrBtAtDgAtcHIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcHInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcHInd *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcHInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_H_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S10Q_RES
CsrSize CsrBtAtDgAtcS10qResSizeof(void *msg)
{
    CsrBtAtDgAtcS10qRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S10Q_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S10Q_RES
CsrUint8 *CsrBtAtDgAtcS10qResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcS10qRes *primitive;

    primitive = (CsrBtAtDgAtcS10qRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->value);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S10Q_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S10Q_RES
void *CsrBtAtDgAtcS10qResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcS10qRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcS10qRes *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcS10qRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->value, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S10Q_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_GMM_IND
CsrSize CsrBtAtDgAtcGmmIndSizeof(void *msg)
{
    CsrBtAtDgAtcGmmInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_GMM_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_GMM_IND
CsrUint8 *CsrBtAtDgAtcGmmIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcGmmInd *primitive;

    primitive = (CsrBtAtDgAtcGmmInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_GMM_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_GMM_IND
void *CsrBtAtDgAtcGmmIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcGmmInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcGmmInd *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcGmmInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_GMM_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_DEACTIVATE_REQ
CsrSize CsrBtAtDgDeactivateReqSizeof(void *msg)
{
    CsrBtAtDgDeactivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_AT_DG_DEACTIVATE_REQ
CsrUint8 *CsrBtAtDgDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgDeactivateReq *primitive;

    primitive = (CsrBtAtDgDeactivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_AT_DG_DEACTIVATE_REQ
void *CsrBtAtDgDeactivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgDeactivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgDeactivateReq *) CsrPmemZalloc(sizeof(CsrBtAtDgDeactivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S5Q_IND
CsrSize CsrBtAtDgAtcS5qIndSizeof(void *msg)
{
    CsrBtAtDgAtcS5qInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S5Q_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S5Q_IND
CsrUint8 *CsrBtAtDgAtcS5qIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcS5qInd *primitive;

    primitive = (CsrBtAtDgAtcS5qInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S5Q_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S5Q_IND
void *CsrBtAtDgAtcS5qIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcS5qInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcS5qInd *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcS5qInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S5Q_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_CONNECT_TEXT_RES
CsrSize CsrBtAtDgAtcConnectTextResSizeof(void *msg)
{
    CsrBtAtDgAtcConnectTextRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_CONNECT_TEXT_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_CONNECT_TEXT_RES
CsrUint8 *CsrBtAtDgAtcConnectTextResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcConnectTextRes *primitive;

    primitive = (CsrBtAtDgAtcConnectTextRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->atResult);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_CONNECT_TEXT_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_CONNECT_TEXT_RES
void *CsrBtAtDgAtcConnectTextResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcConnectTextRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcConnectTextRes *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcConnectTextRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->payloadLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    CsrUint8Des((CsrUint8 *) &primitive->atResult, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_CONNECT_TEXT_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_CONNECT_TEXT_RES
void CsrBtAtDgAtcConnectTextResSerFree(void *msg)
{
    CsrBtAtDgAtcConnectTextRes *primitive;

    primitive = (CsrBtAtDgAtcConnectTextRes *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_CONNECT_TEXT_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_DEFAULT_SETTING_RES
CsrSize CsrBtAtDgAtcDefaultSettingResSizeof(void *msg)
{
    CsrBtAtDgAtcDefaultSettingRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_DEFAULT_SETTING_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_DEFAULT_SETTING_RES
CsrUint8 *CsrBtAtDgAtcDefaultSettingResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcDefaultSettingRes *primitive;

    primitive = (CsrBtAtDgAtcDefaultSettingRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->parameterSetting.s3Value);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->parameterSetting.qValue);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->parameterSetting.vValue);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->parameterSetting.s4Value);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->parameterSetting.eValue);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->parameterSetting.s5Value);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->parameterSetting.andDValue);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->parameterSetting.s0Value);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->parameterSetting.s6Value);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->parameterSetting.s7Value);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->parameterSetting.s8Value);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->parameterSetting.s10Value);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_DEFAULT_SETTING_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_DEFAULT_SETTING_RES
void *CsrBtAtDgAtcDefaultSettingResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcDefaultSettingRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcDefaultSettingRes *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcDefaultSettingRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->parameterSetting.s3Value, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->parameterSetting.qValue, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->parameterSetting.vValue, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->parameterSetting.s4Value, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->parameterSetting.eValue, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->parameterSetting.s5Value, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->parameterSetting.andDValue, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->parameterSetting.s0Value, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->parameterSetting.s6Value, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->parameterSetting.s7Value, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->parameterSetting.s8Value, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->parameterSetting.s10Value, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_DEFAULT_SETTING_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_CONTROL_IND
CsrSize CsrBtAtDgControlIndSizeof(void *msg)
{
    CsrBtAtDgControlInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_CONTROL_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_CONTROL_IND
CsrUint8 *CsrBtAtDgControlIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgControlInd *primitive;

    primitive = (CsrBtAtDgControlInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->modemstatus);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->break_signal);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_CONTROL_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_CONTROL_IND
void *CsrBtAtDgControlIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgControlInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgControlInd *) CsrPmemZalloc(sizeof(CsrBtAtDgControlInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->modemstatus, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->break_signal, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_CONTROL_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_UNKNOWN_EXTENDED_CMD_IND
CsrSize CsrBtAtDgAtcUnknownExtendedCmdIndSizeof(void *msg)
{
    CsrBtAtDgAtcUnknownExtendedCmdInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_UNKNOWN_EXTENDED_CMD_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_UNKNOWN_EXTENDED_CMD_IND
CsrUint8 *CsrBtAtDgAtcUnknownExtendedCmdIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAtDgAtcUnknownExtendedCmdInd *primitive;

    primitive = (CsrBtAtDgAtcUnknownExtendedCmdInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_UNKNOWN_EXTENDED_CMD_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_UNKNOWN_EXTENDED_CMD_IND
void *CsrBtAtDgAtcUnknownExtendedCmdIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAtDgAtcUnknownExtendedCmdInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAtDgAtcUnknownExtendedCmdInd *) CsrPmemZalloc(sizeof(CsrBtAtDgAtcUnknownExtendedCmdInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_UNKNOWN_EXTENDED_CMD_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_UNKNOWN_EXTENDED_CMD_IND
void CsrBtAtDgAtcUnknownExtendedCmdIndSerFree(void *msg)
{
    CsrBtAtDgAtcUnknownExtendedCmdInd *primitive;

    primitive = (CsrBtAtDgAtcUnknownExtendedCmdInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_UNKNOWN_EXTENDED_CMD_IND */

#endif /* EXCLUDE_CSR_BT_AT_MODULE */
