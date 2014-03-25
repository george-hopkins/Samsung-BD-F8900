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
#include "csr_bt_dg_prim.h"

#ifndef EXCLUDE_CSR_BT_DG_MODULE

void CsrBtDgPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_DG_PORTNEG_CFM
CsrSize CsrBtDgPortnegCfmSizeof(void *msg)
{
    CsrBtDgPortnegCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DG_PORTNEG_CFM */

#ifndef EXCLUDE_CSR_BT_DG_PORTNEG_CFM
CsrUint8 *CsrBtDgPortnegCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDgPortnegCfm *primitive;

    primitive = (CsrBtDgPortnegCfm *) msg;
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
#endif /* EXCLUDE_CSR_BT_DG_PORTNEG_CFM */

#ifndef EXCLUDE_CSR_BT_DG_PORTNEG_CFM
void *CsrBtDgPortnegCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDgPortnegCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDgPortnegCfm *) CsrPmemZalloc(sizeof(CsrBtDgPortnegCfm));

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
#endif /* EXCLUDE_CSR_BT_DG_PORTNEG_CFM */

#ifndef EXCLUDE_CSR_BT_DG_PORTNEG_RES
CsrSize CsrBtDgPortnegResSizeof(void *msg)
{
    CsrBtDgPortnegRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DG_PORTNEG_RES */

#ifndef EXCLUDE_CSR_BT_DG_PORTNEG_RES
CsrUint8 *CsrBtDgPortnegResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDgPortnegRes *primitive;

    primitive = (CsrBtDgPortnegRes *) msg;
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
#endif /* EXCLUDE_CSR_BT_DG_PORTNEG_RES */

#ifndef EXCLUDE_CSR_BT_DG_PORTNEG_RES
void *CsrBtDgPortnegResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDgPortnegRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDgPortnegRes *) CsrPmemZalloc(sizeof(CsrBtDgPortnegRes));

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
#endif /* EXCLUDE_CSR_BT_DG_PORTNEG_RES */

#ifndef EXCLUDE_CSR_BT_DG_ACTIVATE_REQ
CsrSize CsrBtDgActivateReqSizeof(void *msg)
{
    CsrBtDgActivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DG_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_DG_ACTIVATE_REQ
CsrUint8 *CsrBtDgActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDgActivateReq *primitive;

    primitive = (CsrBtDgActivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->timeout);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->role);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_DG_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_DG_ACTIVATE_REQ
void *CsrBtDgActivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDgActivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDgActivateReq *) CsrPmemZalloc(sizeof(CsrBtDgActivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->timeout, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->role, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_DG_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_DG_DISCONNECT_IND
CsrSize CsrBtDgDisconnectIndSizeof(void *msg)
{
    CsrBtDgDisconnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DG_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_DG_DISCONNECT_IND
CsrUint8 *CsrBtDgDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDgDisconnectInd *primitive;

    primitive = (CsrBtDgDisconnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->localTerminated);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_DG_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_DG_DISCONNECT_IND
void *CsrBtDgDisconnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDgDisconnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDgDisconnectInd *) CsrPmemZalloc(sizeof(CsrBtDgDisconnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->localTerminated, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_DG_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_DG_CONTROL_IND
CsrSize CsrBtDgControlIndSizeof(void *msg)
{
    CsrBtDgControlInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DG_CONTROL_IND */

#ifndef EXCLUDE_CSR_BT_DG_CONTROL_IND
CsrUint8 *CsrBtDgControlIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDgControlInd *primitive;

    primitive = (CsrBtDgControlInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->modemstatus);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->break_signal);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_DG_CONTROL_IND */

#ifndef EXCLUDE_CSR_BT_DG_CONTROL_IND
void *CsrBtDgControlIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDgControlInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDgControlInd *) CsrPmemZalloc(sizeof(CsrBtDgControlInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->modemstatus, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->break_signal, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_DG_CONTROL_IND */

#ifndef EXCLUDE_CSR_BT_DG_CONTROL_REQ
CsrSize CsrBtDgControlReqSizeof(void *msg)
{
    CsrBtDgControlReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DG_CONTROL_REQ */

#ifndef EXCLUDE_CSR_BT_DG_CONTROL_REQ
CsrUint8 *CsrBtDgControlReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDgControlReq *primitive;

    primitive = (CsrBtDgControlReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->modemstatus);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->break_signal);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_DG_CONTROL_REQ */

#ifndef EXCLUDE_CSR_BT_DG_CONTROL_REQ
void *CsrBtDgControlReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDgControlReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDgControlReq *) CsrPmemZalloc(sizeof(CsrBtDgControlReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->modemstatus, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->break_signal, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_DG_CONTROL_REQ */

#ifndef EXCLUDE_CSR_BT_DG_DATA_IND
CsrSize CsrBtDgDataIndSizeof(void *msg)
{
    CsrBtDgDataInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DG_DATA_IND */

#ifndef EXCLUDE_CSR_BT_DG_DATA_IND
CsrUint8 *CsrBtDgDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDgDataInd *primitive;

    primitive = (CsrBtDgDataInd *) msg;
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
#endif /* EXCLUDE_CSR_BT_DG_DATA_IND */

#ifndef EXCLUDE_CSR_BT_DG_DATA_IND
void *CsrBtDgDataIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDgDataInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDgDataInd *) CsrPmemZalloc(sizeof(CsrBtDgDataInd));

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
#endif /* EXCLUDE_CSR_BT_DG_DATA_IND */

#ifndef EXCLUDE_CSR_BT_DG_DATA_IND
void CsrBtDgDataIndSerFree(void *msg)
{
    CsrBtDgDataInd *primitive;

    primitive = (CsrBtDgDataInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_DG_DATA_IND */

#ifndef EXCLUDE_CSR_BT_DG_DATA_PATH_STATUS_REQ
CsrSize CsrBtDgDataPathStatusReqSizeof(void *msg)
{
    CsrBtDgDataPathStatusReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DG_DATA_PATH_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_DG_DATA_PATH_STATUS_REQ
CsrUint8 *CsrBtDgDataPathStatusReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDgDataPathStatusReq *primitive;

    primitive = (CsrBtDgDataPathStatusReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->dgInstanceQueue);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_DG_DATA_PATH_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_DG_DATA_PATH_STATUS_REQ
void *CsrBtDgDataPathStatusReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDgDataPathStatusReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDgDataPathStatusReq *) CsrPmemZalloc(sizeof(CsrBtDgDataPathStatusReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->dgInstanceQueue, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_DG_DATA_PATH_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_DG_DEACTIVATE_REQ
CsrSize CsrBtDgDeactivateReqSizeof(void *msg)
{
    CsrBtDgDeactivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DG_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_DG_DEACTIVATE_REQ
CsrUint8 *CsrBtDgDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDgDeactivateReq *primitive;

    primitive = (CsrBtDgDeactivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_DG_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_DG_DEACTIVATE_REQ
void *CsrBtDgDeactivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDgDeactivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDgDeactivateReq *) CsrPmemZalloc(sizeof(CsrBtDgDeactivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_DG_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_DG_STATUS_IND
CsrSize CsrBtDgStatusIndSizeof(void *msg)
{
    CsrBtDgStatusInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DG_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_DG_STATUS_IND
CsrUint8 *CsrBtDgStatusIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDgStatusInd *primitive;

    primitive = (CsrBtDgStatusInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connect);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxMsgSize);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_DG_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_DG_STATUS_IND
void *CsrBtDgStatusIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDgStatusInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDgStatusInd *) CsrPmemZalloc(sizeof(CsrBtDgStatusInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connect, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxMsgSize, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_DG_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_DG_DEACTIVATE_CFM
CsrSize CsrBtDgDeactivateCfmSizeof(void *msg)
{
    CsrBtDgDeactivateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DG_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_DG_DEACTIVATE_CFM
CsrUint8 *CsrBtDgDeactivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDgDeactivateCfm *primitive;

    primitive = (CsrBtDgDeactivateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_DG_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_DG_DEACTIVATE_CFM
void *CsrBtDgDeactivateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDgDeactivateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDgDeactivateCfm *) CsrPmemZalloc(sizeof(CsrBtDgDeactivateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_DG_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_DG_REGISTER_DATA_PATH_HANDLE_REQ
CsrSize CsrBtDgRegisterDataPathHandleReqSizeof(void *msg)
{
    CsrBtDgRegisterDataPathHandleReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DG_REGISTER_DATA_PATH_HANDLE_REQ */

#ifndef EXCLUDE_CSR_BT_DG_REGISTER_DATA_PATH_HANDLE_REQ
CsrUint8 *CsrBtDgRegisterDataPathHandleReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDgRegisterDataPathHandleReq *primitive;

    primitive = (CsrBtDgRegisterDataPathHandleReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->dataAppHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_DG_REGISTER_DATA_PATH_HANDLE_REQ */

#ifndef EXCLUDE_CSR_BT_DG_REGISTER_DATA_PATH_HANDLE_REQ
void *CsrBtDgRegisterDataPathHandleReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDgRegisterDataPathHandleReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDgRegisterDataPathHandleReq *) CsrPmemZalloc(sizeof(CsrBtDgRegisterDataPathHandleReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->dataAppHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_DG_REGISTER_DATA_PATH_HANDLE_REQ */

#ifndef EXCLUDE_CSR_BT_DG_DATA_CFM
CsrSize CsrBtDgDataCfmSizeof(void *msg)
{
    CsrBtDgDataCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DG_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_DG_DATA_CFM
CsrUint8 *CsrBtDgDataCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDgDataCfm *primitive;

    primitive = (CsrBtDgDataCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_DG_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_DG_DATA_CFM
void *CsrBtDgDataCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDgDataCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDgDataCfm *) CsrPmemZalloc(sizeof(CsrBtDgDataCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_DG_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_DG_DISCONNECT_REQ
CsrSize CsrBtDgDisconnectReqSizeof(void *msg)
{
    CsrBtDgDisconnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DG_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_DG_DISCONNECT_REQ
CsrUint8 *CsrBtDgDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDgDisconnectReq *primitive;

    primitive = (CsrBtDgDisconnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_DG_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_DG_DISCONNECT_REQ
void *CsrBtDgDisconnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDgDisconnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDgDisconnectReq *) CsrPmemZalloc(sizeof(CsrBtDgDisconnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_DG_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_DG_HOUSE_CLEANING
CsrSize CsrBtDgHouseCleaningSizeof(void *msg)
{
    CsrBtDgHouseCleaning *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DG_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_DG_HOUSE_CLEANING
CsrUint8 *CsrBtDgHouseCleaningSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDgHouseCleaning *primitive;

    primitive = (CsrBtDgHouseCleaning *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_DG_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_DG_HOUSE_CLEANING
void *CsrBtDgHouseCleaningDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDgHouseCleaning *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDgHouseCleaning *) CsrPmemZalloc(sizeof(CsrBtDgHouseCleaning));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_DG_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_DG_PORTNEG_IND
CsrSize CsrBtDgPortnegIndSizeof(void *msg)
{
    CsrBtDgPortnegInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DG_PORTNEG_IND */

#ifndef EXCLUDE_CSR_BT_DG_PORTNEG_IND
CsrUint8 *CsrBtDgPortnegIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDgPortnegInd *primitive;

    primitive = (CsrBtDgPortnegInd *) msg;
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
#endif /* EXCLUDE_CSR_BT_DG_PORTNEG_IND */

#ifndef EXCLUDE_CSR_BT_DG_PORTNEG_IND
void *CsrBtDgPortnegIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDgPortnegInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDgPortnegInd *) CsrPmemZalloc(sizeof(CsrBtDgPortnegInd));

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
#endif /* EXCLUDE_CSR_BT_DG_PORTNEG_IND */

#ifndef EXCLUDE_CSR_BT_DG_REGISTER_DATA_PATH_HANDLE_CFM
CsrSize CsrBtDgRegisterDataPathHandleCfmSizeof(void *msg)
{
    CsrBtDgRegisterDataPathHandleCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DG_REGISTER_DATA_PATH_HANDLE_CFM */

#ifndef EXCLUDE_CSR_BT_DG_REGISTER_DATA_PATH_HANDLE_CFM
CsrUint8 *CsrBtDgRegisterDataPathHandleCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDgRegisterDataPathHandleCfm *primitive;

    primitive = (CsrBtDgRegisterDataPathHandleCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_DG_REGISTER_DATA_PATH_HANDLE_CFM */

#ifndef EXCLUDE_CSR_BT_DG_REGISTER_DATA_PATH_HANDLE_CFM
void *CsrBtDgRegisterDataPathHandleCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDgRegisterDataPathHandleCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDgRegisterDataPathHandleCfm *) CsrPmemZalloc(sizeof(CsrBtDgRegisterDataPathHandleCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_DG_REGISTER_DATA_PATH_HANDLE_CFM */

#ifndef EXCLUDE_CSR_BT_DG_SECURITY_IN_REQ
CsrSize CsrBtDgSecurityInReqSizeof(void *msg)
{
    CsrBtDgSecurityInReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DG_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_DG_SECURITY_IN_REQ
CsrUint8 *CsrBtDgSecurityInReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDgSecurityInReq *primitive;

    primitive = (CsrBtDgSecurityInReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_DG_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_DG_SECURITY_IN_REQ
void *CsrBtDgSecurityInReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDgSecurityInReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDgSecurityInReq *) CsrPmemZalloc(sizeof(CsrBtDgSecurityInReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_DG_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_DG_SECURITY_IN_CFM
CsrSize CsrBtDgSecurityInCfmSizeof(void *msg)
{
    CsrBtDgSecurityInCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DG_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_DG_SECURITY_IN_CFM
CsrUint8 *CsrBtDgSecurityInCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDgSecurityInCfm *primitive;

    primitive = (CsrBtDgSecurityInCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_DG_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_DG_SECURITY_IN_CFM
void *CsrBtDgSecurityInCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDgSecurityInCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDgSecurityInCfm *) CsrPmemZalloc(sizeof(CsrBtDgSecurityInCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_DG_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_DG_PORTNEG_REQ
CsrSize CsrBtDgPortnegReqSizeof(void *msg)
{
    CsrBtDgPortnegReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DG_PORTNEG_REQ */

#ifndef EXCLUDE_CSR_BT_DG_PORTNEG_REQ
CsrUint8 *CsrBtDgPortnegReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDgPortnegReq *primitive;

    primitive = (CsrBtDgPortnegReq *) msg;
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
#endif /* EXCLUDE_CSR_BT_DG_PORTNEG_REQ */

#ifndef EXCLUDE_CSR_BT_DG_PORTNEG_REQ
void *CsrBtDgPortnegReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDgPortnegReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDgPortnegReq *) CsrPmemZalloc(sizeof(CsrBtDgPortnegReq));

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
#endif /* EXCLUDE_CSR_BT_DG_PORTNEG_REQ */

#ifndef EXCLUDE_CSR_BT_DG_CONNECT_IND
CsrSize CsrBtDgConnectIndSizeof(void *msg)
{
    CsrBtDgConnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DG_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_DG_CONNECT_IND
CsrUint8 *CsrBtDgConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDgConnectInd *primitive;

    primitive = (CsrBtDgConnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serverChannel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->profileMaxFrameSize);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_DG_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_DG_CONNECT_IND
void *CsrBtDgConnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDgConnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDgConnectInd *) CsrPmemZalloc(sizeof(CsrBtDgConnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->serverChannel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->profileMaxFrameSize, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_DG_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_DG_DATA_REQ
CsrSize CsrBtDgDataReqSizeof(void *msg)
{
    CsrBtDgDataReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DG_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_DG_DATA_REQ
CsrUint8 *CsrBtDgDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDgDataReq *primitive;

    primitive = (CsrBtDgDataReq *) msg;
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
#endif /* EXCLUDE_CSR_BT_DG_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_DG_DATA_REQ
void *CsrBtDgDataReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDgDataReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDgDataReq *) CsrPmemZalloc(sizeof(CsrBtDgDataReq));

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
#endif /* EXCLUDE_CSR_BT_DG_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_DG_DATA_REQ
void CsrBtDgDataReqSerFree(void *msg)
{
    CsrBtDgDataReq *primitive;

    primitive = (CsrBtDgDataReq *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_DG_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_DG_DATA_RES
CsrSize CsrBtDgDataResSizeof(void *msg)
{
    CsrBtDgDataRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DG_DATA_RES */

#ifndef EXCLUDE_CSR_BT_DG_DATA_RES
CsrUint8 *CsrBtDgDataResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDgDataRes *primitive;

    primitive = (CsrBtDgDataRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_DG_DATA_RES */

#ifndef EXCLUDE_CSR_BT_DG_DATA_RES
void *CsrBtDgDataResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDgDataRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDgDataRes *) CsrPmemZalloc(sizeof(CsrBtDgDataRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_DG_DATA_RES */

#ifndef EXCLUDE_CSR_BT_DG_DATA_PATH_STATUS_IND
CsrSize CsrBtDgDataPathStatusIndSizeof(void *msg)
{
    CsrBtDgDataPathStatusInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DG_DATA_PATH_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_DG_DATA_PATH_STATUS_IND
CsrUint8 *CsrBtDgDataPathStatusIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDgDataPathStatusInd *primitive;

    primitive = (CsrBtDgDataPathStatusInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->dgInstanceQueue);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_DG_DATA_PATH_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_DG_DATA_PATH_STATUS_IND
void *CsrBtDgDataPathStatusIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDgDataPathStatusInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDgDataPathStatusInd *) CsrPmemZalloc(sizeof(CsrBtDgDataPathStatusInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->dgInstanceQueue, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_DG_DATA_PATH_STATUS_IND */

#endif /* EXCLUDE_CSR_BT_DG_MODULE */
