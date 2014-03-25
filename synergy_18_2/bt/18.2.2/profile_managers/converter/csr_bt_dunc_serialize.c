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
#include "csr_bt_dunc_prim.h"

#ifndef EXCLUDE_CSR_BT_DUNC_MODULE

void CsrBtDuncPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_DUNC_PORTNEG_IND
CsrSize CsrBtDuncPortnegIndSizeof(void *msg)
{
    CsrBtDuncPortnegInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DUNC_PORTNEG_IND */

#ifndef EXCLUDE_CSR_BT_DUNC_PORTNEG_IND
CsrUint8 *CsrBtDuncPortnegIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDuncPortnegInd *primitive;

    primitive = (CsrBtDuncPortnegInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->duncInstanceId);
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
#endif /* EXCLUDE_CSR_BT_DUNC_PORTNEG_IND */

#ifndef EXCLUDE_CSR_BT_DUNC_PORTNEG_IND
void *CsrBtDuncPortnegIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDuncPortnegInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDuncPortnegInd *) CsrPmemZalloc(sizeof(CsrBtDuncPortnegInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->duncInstanceId, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_DUNC_PORTNEG_IND */

#ifndef EXCLUDE_CSR_BT_DUNC_CHANGE_DATA_PATH_STATUS_REQ
CsrSize CsrBtDuncChangeDataPathStatusReqSizeof(void *msg)
{
    CsrBtDuncChangeDataPathStatusReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DUNC_CHANGE_DATA_PATH_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_DUNC_CHANGE_DATA_PATH_STATUS_REQ
CsrUint8 *CsrBtDuncChangeDataPathStatusReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDuncChangeDataPathStatusReq *primitive;

    primitive = (CsrBtDuncChangeDataPathStatusReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->duncInstanceId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_DUNC_CHANGE_DATA_PATH_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_DUNC_CHANGE_DATA_PATH_STATUS_REQ
void *CsrBtDuncChangeDataPathStatusReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDuncChangeDataPathStatusReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDuncChangeDataPathStatusReq *) CsrPmemZalloc(sizeof(CsrBtDuncChangeDataPathStatusReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->duncInstanceId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_DUNC_CHANGE_DATA_PATH_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_DUNC_DISCONNECT_IND
CsrSize CsrBtDuncDisconnectIndSizeof(void *msg)
{
    CsrBtDuncDisconnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DUNC_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_DUNC_DISCONNECT_IND
CsrUint8 *CsrBtDuncDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDuncDisconnectInd *primitive;

    primitive = (CsrBtDuncDisconnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->duncInstanceId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->localTerminated);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_DUNC_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_DUNC_DISCONNECT_IND
void *CsrBtDuncDisconnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDuncDisconnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDuncDisconnectInd *) CsrPmemZalloc(sizeof(CsrBtDuncDisconnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->duncInstanceId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->localTerminated, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_DUNC_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_DUNC_DATA_CFM
CsrSize CsrBtDuncDataCfmSizeof(void *msg)
{
    CsrBtDuncDataCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DUNC_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_DUNC_DATA_CFM
CsrUint8 *CsrBtDuncDataCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDuncDataCfm *primitive;

    primitive = (CsrBtDuncDataCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->duncInstanceId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_DUNC_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_DUNC_DATA_CFM
void *CsrBtDuncDataCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDuncDataCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDuncDataCfm *) CsrPmemZalloc(sizeof(CsrBtDuncDataCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->duncInstanceId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_DUNC_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_DUNC_DISCONNECT_REQ
CsrSize CsrBtDuncDisconnectReqSizeof(void *msg)
{
    CsrBtDuncDisconnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DUNC_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_DUNC_DISCONNECT_REQ
CsrUint8 *CsrBtDuncDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDuncDisconnectReq *primitive;

    primitive = (CsrBtDuncDisconnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_DUNC_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_DUNC_DISCONNECT_REQ
void *CsrBtDuncDisconnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDuncDisconnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDuncDisconnectReq *) CsrPmemZalloc(sizeof(CsrBtDuncDisconnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_DUNC_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_DUNC_SECURITY_OUT_CFM
CsrSize CsrBtDuncSecurityOutCfmSizeof(void *msg)
{
    CsrBtDuncSecurityOutCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DUNC_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_DUNC_SECURITY_OUT_CFM
CsrUint8 *CsrBtDuncSecurityOutCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDuncSecurityOutCfm *primitive;

    primitive = (CsrBtDuncSecurityOutCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_DUNC_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_DUNC_SECURITY_OUT_CFM
void *CsrBtDuncSecurityOutCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDuncSecurityOutCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDuncSecurityOutCfm *) CsrPmemZalloc(sizeof(CsrBtDuncSecurityOutCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_DUNC_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_DUNC_DATA_PATH_STATUS_IND
CsrSize CsrBtDuncDataPathStatusIndSizeof(void *msg)
{
    CsrBtDuncDataPathStatusInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DUNC_DATA_PATH_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_DUNC_DATA_PATH_STATUS_IND
CsrUint8 *CsrBtDuncDataPathStatusIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDuncDataPathStatusInd *primitive;

    primitive = (CsrBtDuncDataPathStatusInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->duncInstanceId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_DUNC_DATA_PATH_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_DUNC_DATA_PATH_STATUS_IND
void *CsrBtDuncDataPathStatusIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDuncDataPathStatusInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDuncDataPathStatusInd *) CsrPmemZalloc(sizeof(CsrBtDuncDataPathStatusInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->duncInstanceId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_DUNC_DATA_PATH_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_DUNC_DATA_REQ
CsrSize CsrBtDuncDataReqSizeof(void *msg)
{
    CsrBtDuncDataReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->dataLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DUNC_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_DUNC_DATA_REQ
CsrUint8 *CsrBtDuncDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDuncDataReq *primitive;

    primitive = (CsrBtDuncDataReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->dataLength);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    if(primitive->data)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->data), ((CsrSize) (primitive->dataLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_DUNC_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_DUNC_DATA_REQ
void *CsrBtDuncDataReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDuncDataReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDuncDataReq *) CsrPmemZalloc(sizeof(CsrBtDuncDataReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_DUNC_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_DUNC_DATA_REQ
void CsrBtDuncDataReqSerFree(void *msg)
{
    CsrBtDuncDataReq *primitive;

    primitive = (CsrBtDuncDataReq *) msg;

    if(primitive->data != NULL)
    {
        CsrPmemFree(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_DUNC_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_DUNC_DATA_IND
CsrSize CsrBtDuncDataIndSizeof(void *msg)
{
    CsrBtDuncDataInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->dataLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DUNC_DATA_IND */

#ifndef EXCLUDE_CSR_BT_DUNC_DATA_IND
CsrUint8 *CsrBtDuncDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDuncDataInd *primitive;

    primitive = (CsrBtDuncDataInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->duncInstanceId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->dataLength);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    if(primitive->data)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->data), ((CsrSize) (primitive->dataLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_DUNC_DATA_IND */

#ifndef EXCLUDE_CSR_BT_DUNC_DATA_IND
void *CsrBtDuncDataIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDuncDataInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDuncDataInd *) CsrPmemZalloc(sizeof(CsrBtDuncDataInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->duncInstanceId, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_DUNC_DATA_IND */

#ifndef EXCLUDE_CSR_BT_DUNC_DATA_IND
void CsrBtDuncDataIndSerFree(void *msg)
{
    CsrBtDuncDataInd *primitive;

    primitive = (CsrBtDuncDataInd *) msg;

    if(primitive->data != NULL)
    {
        CsrPmemFree(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_DUNC_DATA_IND */

#ifndef EXCLUDE_CSR_BT_DUNC_REGISTER_DATA_PATH_HANDLE_CFM
CsrSize CsrBtDuncRegisterDataPathHandleCfmSizeof(void *msg)
{
    CsrBtDuncRegisterDataPathHandleCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DUNC_REGISTER_DATA_PATH_HANDLE_CFM */

#ifndef EXCLUDE_CSR_BT_DUNC_REGISTER_DATA_PATH_HANDLE_CFM
CsrUint8 *CsrBtDuncRegisterDataPathHandleCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDuncRegisterDataPathHandleCfm *primitive;

    primitive = (CsrBtDuncRegisterDataPathHandleCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->duncInstanceId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_DUNC_REGISTER_DATA_PATH_HANDLE_CFM */

#ifndef EXCLUDE_CSR_BT_DUNC_REGISTER_DATA_PATH_HANDLE_CFM
void *CsrBtDuncRegisterDataPathHandleCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDuncRegisterDataPathHandleCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDuncRegisterDataPathHandleCfm *) CsrPmemZalloc(sizeof(CsrBtDuncRegisterDataPathHandleCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->duncInstanceId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_DUNC_REGISTER_DATA_PATH_HANDLE_CFM */

#ifndef EXCLUDE_CSR_BT_DUNC_PORTNEG_CFM
CsrSize CsrBtDuncPortnegCfmSizeof(void *msg)
{
    CsrBtDuncPortnegCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DUNC_PORTNEG_CFM */

#ifndef EXCLUDE_CSR_BT_DUNC_PORTNEG_CFM
CsrUint8 *CsrBtDuncPortnegCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDuncPortnegCfm *primitive;

    primitive = (CsrBtDuncPortnegCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->duncInstanceId);
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
#endif /* EXCLUDE_CSR_BT_DUNC_PORTNEG_CFM */

#ifndef EXCLUDE_CSR_BT_DUNC_PORTNEG_CFM
void *CsrBtDuncPortnegCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDuncPortnegCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDuncPortnegCfm *) CsrPmemZalloc(sizeof(CsrBtDuncPortnegCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->duncInstanceId, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_DUNC_PORTNEG_CFM */

#ifndef EXCLUDE_CSR_BT_DUNC_HOUSE_CLEANING
CsrSize CsrBtDuncHouseCleaningSizeof(void *msg)
{
    CsrBtDuncHouseCleaning *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DUNC_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_DUNC_HOUSE_CLEANING
CsrUint8 *CsrBtDuncHouseCleaningSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDuncHouseCleaning *primitive;

    primitive = (CsrBtDuncHouseCleaning *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_DUNC_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_DUNC_HOUSE_CLEANING
void *CsrBtDuncHouseCleaningDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDuncHouseCleaning *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDuncHouseCleaning *) CsrPmemZalloc(sizeof(CsrBtDuncHouseCleaning));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_DUNC_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_DUNC_DATA_RES
CsrSize CsrBtDuncDataResSizeof(void *msg)
{
    CsrBtDuncDataRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DUNC_DATA_RES */

#ifndef EXCLUDE_CSR_BT_DUNC_DATA_RES
CsrUint8 *CsrBtDuncDataResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDuncDataRes *primitive;

    primitive = (CsrBtDuncDataRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_DUNC_DATA_RES */

#ifndef EXCLUDE_CSR_BT_DUNC_DATA_RES
void *CsrBtDuncDataResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDuncDataRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDuncDataRes *) CsrPmemZalloc(sizeof(CsrBtDuncDataRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_DUNC_DATA_RES */

#ifndef EXCLUDE_CSR_BT_DUNC_CONTROL_IND
CsrSize CsrBtDuncControlIndSizeof(void *msg)
{
    CsrBtDuncControlInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DUNC_CONTROL_IND */

#ifndef EXCLUDE_CSR_BT_DUNC_CONTROL_IND
CsrUint8 *CsrBtDuncControlIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDuncControlInd *primitive;

    primitive = (CsrBtDuncControlInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->duncInstanceId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->modemStatus);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->breakSignal);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_DUNC_CONTROL_IND */

#ifndef EXCLUDE_CSR_BT_DUNC_CONTROL_IND
void *CsrBtDuncControlIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDuncControlInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDuncControlInd *) CsrPmemZalloc(sizeof(CsrBtDuncControlInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->duncInstanceId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->modemStatus, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->breakSignal, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_DUNC_CONTROL_IND */

#ifndef EXCLUDE_CSR_BT_DUNC_CANCEL_CONNECT_REQ
CsrSize CsrBtDuncCancelConnectReqSizeof(void *msg)
{
    CsrBtDuncCancelConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DUNC_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_DUNC_CANCEL_CONNECT_REQ
CsrUint8 *CsrBtDuncCancelConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDuncCancelConnectReq *primitive;

    primitive = (CsrBtDuncCancelConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_DUNC_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_DUNC_CANCEL_CONNECT_REQ
void *CsrBtDuncCancelConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDuncCancelConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDuncCancelConnectReq *) CsrPmemZalloc(sizeof(CsrBtDuncCancelConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_DUNC_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_DUNC_PORTNEG_RES
CsrSize CsrBtDuncPortnegResSizeof(void *msg)
{
    CsrBtDuncPortnegRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DUNC_PORTNEG_RES */

#ifndef EXCLUDE_CSR_BT_DUNC_PORTNEG_RES
CsrUint8 *CsrBtDuncPortnegResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDuncPortnegRes *primitive;

    primitive = (CsrBtDuncPortnegRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
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
#endif /* EXCLUDE_CSR_BT_DUNC_PORTNEG_RES */

#ifndef EXCLUDE_CSR_BT_DUNC_PORTNEG_RES
void *CsrBtDuncPortnegResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDuncPortnegRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDuncPortnegRes *) CsrPmemZalloc(sizeof(CsrBtDuncPortnegRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_DUNC_PORTNEG_RES */

#ifndef EXCLUDE_CSR_BT_DUNC_REGISTER_DATA_PATH_HANDLE_REQ
CsrSize CsrBtDuncRegisterDataPathHandleReqSizeof(void *msg)
{
    CsrBtDuncRegisterDataPathHandleReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DUNC_REGISTER_DATA_PATH_HANDLE_REQ */

#ifndef EXCLUDE_CSR_BT_DUNC_REGISTER_DATA_PATH_HANDLE_REQ
CsrUint8 *CsrBtDuncRegisterDataPathHandleReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDuncRegisterDataPathHandleReq *primitive;

    primitive = (CsrBtDuncRegisterDataPathHandleReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->dataAppHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_DUNC_REGISTER_DATA_PATH_HANDLE_REQ */

#ifndef EXCLUDE_CSR_BT_DUNC_REGISTER_DATA_PATH_HANDLE_REQ
void *CsrBtDuncRegisterDataPathHandleReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDuncRegisterDataPathHandleReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDuncRegisterDataPathHandleReq *) CsrPmemZalloc(sizeof(CsrBtDuncRegisterDataPathHandleReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->dataAppHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_DUNC_REGISTER_DATA_PATH_HANDLE_REQ */

#ifndef EXCLUDE_CSR_BT_DUNC_SECURITY_OUT_REQ
CsrSize CsrBtDuncSecurityOutReqSizeof(void *msg)
{
    CsrBtDuncSecurityOutReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DUNC_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_DUNC_SECURITY_OUT_REQ
CsrUint8 *CsrBtDuncSecurityOutReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDuncSecurityOutReq *primitive;

    primitive = (CsrBtDuncSecurityOutReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_DUNC_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_DUNC_SECURITY_OUT_REQ
void *CsrBtDuncSecurityOutReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDuncSecurityOutReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDuncSecurityOutReq *) CsrPmemZalloc(sizeof(CsrBtDuncSecurityOutReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_DUNC_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_DUNC_CONNECT_REQ
CsrSize CsrBtDuncConnectReqSizeof(void *msg)
{
    CsrBtDuncConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DUNC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_DUNC_CONNECT_REQ
CsrUint8 *CsrBtDuncConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDuncConnectReq *primitive;

    primitive = (CsrBtDuncConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ctrlHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bdAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bdAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bdAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->lowPowerSupport);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_DUNC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_DUNC_CONNECT_REQ
void *CsrBtDuncConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDuncConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDuncConnectReq *) CsrPmemZalloc(sizeof(CsrBtDuncConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->ctrlHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bdAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bdAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bdAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->lowPowerSupport, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_DUNC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_DUNC_PORTNEG_REQ
CsrSize CsrBtDuncPortnegReqSizeof(void *msg)
{
    CsrBtDuncPortnegReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DUNC_PORTNEG_REQ */

#ifndef EXCLUDE_CSR_BT_DUNC_PORTNEG_REQ
CsrUint8 *CsrBtDuncPortnegReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDuncPortnegReq *primitive;

    primitive = (CsrBtDuncPortnegReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
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
#endif /* EXCLUDE_CSR_BT_DUNC_PORTNEG_REQ */

#ifndef EXCLUDE_CSR_BT_DUNC_PORTNEG_REQ
void *CsrBtDuncPortnegReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDuncPortnegReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDuncPortnegReq *) CsrPmemZalloc(sizeof(CsrBtDuncPortnegReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_DUNC_PORTNEG_REQ */

#ifndef EXCLUDE_CSR_BT_DUNC_STATUS_IND
CsrSize CsrBtDuncStatusIndSizeof(void *msg)
{
    CsrBtDuncStatusInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DUNC_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_DUNC_STATUS_IND
CsrUint8 *CsrBtDuncStatusIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDuncStatusInd *primitive;

    primitive = (CsrBtDuncStatusInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->duncInstanceId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connected);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxMsgSize);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_DUNC_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_DUNC_STATUS_IND
void *CsrBtDuncStatusIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDuncStatusInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDuncStatusInd *) CsrPmemZalloc(sizeof(CsrBtDuncStatusInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->duncInstanceId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connected, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxMsgSize, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_DUNC_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_DUNC_CONNECT_CFM
CsrSize CsrBtDuncConnectCfmSizeof(void *msg)
{
    CsrBtDuncConnectCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DUNC_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_DUNC_CONNECT_CFM
CsrUint8 *CsrBtDuncConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDuncConnectCfm *primitive;

    primitive = (CsrBtDuncConnectCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->duncInstanceId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxMsgSize);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_DUNC_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_DUNC_CONNECT_CFM
void *CsrBtDuncConnectCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDuncConnectCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDuncConnectCfm *) CsrPmemZalloc(sizeof(CsrBtDuncConnectCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->duncInstanceId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxMsgSize, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_DUNC_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_DUNC_CONTROL_REQ
CsrSize CsrBtDuncControlReqSizeof(void *msg)
{
    CsrBtDuncControlReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DUNC_CONTROL_REQ */

#ifndef EXCLUDE_CSR_BT_DUNC_CONTROL_REQ
CsrUint8 *CsrBtDuncControlReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDuncControlReq *primitive;

    primitive = (CsrBtDuncControlReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->modemStatus);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->breakSignal);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_DUNC_CONTROL_REQ */

#ifndef EXCLUDE_CSR_BT_DUNC_CONTROL_REQ
void *CsrBtDuncControlReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDuncControlReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDuncControlReq *) CsrPmemZalloc(sizeof(CsrBtDuncControlReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->modemStatus, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->breakSignal, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_DUNC_CONTROL_REQ */

#endif /* EXCLUDE_CSR_BT_DUNC_MODULE */
