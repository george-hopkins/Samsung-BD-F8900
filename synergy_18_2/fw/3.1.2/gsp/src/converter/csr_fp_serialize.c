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
#include "csr_fp_prim.h"

#ifndef EXCLUDE_CSR_FP_MODULE

void CsrFpPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_FP_CLEAR_REQ
CsrSize CsrFpClearReqSizeof(void *msg)
{
    CsrFpClearReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_FP_CLEAR_REQ */

#ifndef EXCLUDE_CSR_FP_CLEAR_REQ
CsrUint8 *CsrFpClearReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrFpClearReq *primitive;

    primitive = (CsrFpClearReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->fpHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_FP_CLEAR_REQ */

#ifndef EXCLUDE_CSR_FP_CLEAR_REQ
void *CsrFpClearReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrFpClearReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrFpClearReq *) CsrPmemZalloc(sizeof(CsrFpClearReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->fpHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_FP_CLEAR_REQ */

#ifndef EXCLUDE_CSR_FP_WRITE_CFM
CsrSize CsrFpWriteCfmSizeof(void *msg)
{
    CsrFpWriteCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_FP_WRITE_CFM */

#ifndef EXCLUDE_CSR_FP_WRITE_CFM
CsrUint8 *CsrFpWriteCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrFpWriteCfm *primitive;

    primitive = (CsrFpWriteCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->fpHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_FP_WRITE_CFM */

#ifndef EXCLUDE_CSR_FP_WRITE_CFM
void *CsrFpWriteCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrFpWriteCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrFpWriteCfm *) CsrPmemZalloc(sizeof(CsrFpWriteCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->fpHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_FP_WRITE_CFM */

#ifndef EXCLUDE_CSR_FP_CLEAR_CFM
CsrSize CsrFpClearCfmSizeof(void *msg)
{
    CsrFpClearCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_FP_CLEAR_CFM */

#ifndef EXCLUDE_CSR_FP_CLEAR_CFM
CsrUint8 *CsrFpClearCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrFpClearCfm *primitive;

    primitive = (CsrFpClearCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->fpHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_FP_CLEAR_CFM */

#ifndef EXCLUDE_CSR_FP_CLEAR_CFM
void *CsrFpClearCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrFpClearCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrFpClearCfm *) CsrPmemZalloc(sizeof(CsrFpClearCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->fpHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_FP_CLEAR_CFM */

#ifndef EXCLUDE_CSR_FP_READ_IND
CsrSize CsrFpReadIndSizeof(void *msg)
{
    CsrFpReadInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrMblkGetLength(primitive->data) + sizeof(CsrUint16));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_FP_READ_IND */

#ifndef EXCLUDE_CSR_FP_READ_IND
CsrUint8 *CsrFpReadIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrFpReadInd *primitive;

    primitive = (CsrFpReadInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->fpHandle);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    if (primitive->data)
    {
        CsrUint16 mblkLength = CsrMblkGetLength(primitive->data);
        CsrUint16Ser(buffer, length, mblkLength);
        CsrMblkCopyToMemory(primitive->data, 0, mblkLength, &buffer[*length]);
        *length += mblkLength;
    }
    else
    {
        buffer[*length] = 0;
        buffer[*length + 1] = 0;
        *length += 2;
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_FP_READ_IND */

#ifndef EXCLUDE_CSR_FP_READ_IND
void *CsrFpReadIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrFpReadInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrFpReadInd *) CsrPmemZalloc(sizeof(CsrFpReadInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->fpHandle, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
    {
        CsrUint8 *mblkData;
        CsrUint16 mblkLength;
        CsrUint16Des((CsrUint16 *) &mblkLength, buffer, &offset);
        primitive->data = CsrMblkMallocCreate((void **) &mblkData, mblkLength);
        CsrMemCpy(mblkData,&buffer[offset], mblkLength);
        offset += mblkLength;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_FP_READ_IND */

#ifndef EXCLUDE_CSR_FP_READ_IND
void CsrFpReadIndSerFree(void *msg)
{
    CsrFpReadInd *primitive;

    primitive = (CsrFpReadInd *) msg;

    if(primitive->data != NULL)
    {
        CsrMblkDestroy(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_FP_READ_IND */

#ifndef EXCLUDE_CSR_FP_DESTROY_REQ
CsrSize CsrFpDestroyReqSizeof(void *msg)
{
    CsrFpDestroyReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_FP_DESTROY_REQ */

#ifndef EXCLUDE_CSR_FP_DESTROY_REQ
CsrUint8 *CsrFpDestroyReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrFpDestroyReq *primitive;

    primitive = (CsrFpDestroyReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->fpHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_FP_DESTROY_REQ */

#ifndef EXCLUDE_CSR_FP_DESTROY_REQ
void *CsrFpDestroyReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrFpDestroyReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrFpDestroyReq *) CsrPmemZalloc(sizeof(CsrFpDestroyReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->fpHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_FP_DESTROY_REQ */

#ifndef EXCLUDE_CSR_FP_CREATE_REQ
CsrSize CsrFpCreateReqSizeof(void *msg)
{
    CsrFpCreateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_FP_CREATE_REQ */

#ifndef EXCLUDE_CSR_FP_CREATE_REQ
CsrUint8 *CsrFpCreateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrFpCreateReq *primitive;

    primitive = (CsrFpCreateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->overheadHost);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->capacityRxHost);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->requiredTxController);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->desiredTxController);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->requiredRxController);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->desiredRxController);

    return buffer;
}
#endif /* EXCLUDE_CSR_FP_CREATE_REQ */

#ifndef EXCLUDE_CSR_FP_CREATE_REQ
void *CsrFpCreateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrFpCreateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrFpCreateReq *) CsrPmemZalloc(sizeof(CsrFpCreateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->overheadHost, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->capacityRxHost, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->requiredTxController, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->desiredTxController, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->requiredRxController, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->desiredRxController, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_FP_CREATE_REQ */

#ifndef EXCLUDE_CSR_FP_WRITE_REQ
CsrSize CsrFpWriteReqSizeof(void *msg)
{
    CsrFpWriteReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrMblkGetLength(primitive->data) + sizeof(CsrUint16));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_FP_WRITE_REQ */

#ifndef EXCLUDE_CSR_FP_WRITE_REQ
CsrUint8 *CsrFpWriteReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrFpWriteReq *primitive;

    primitive = (CsrFpWriteReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->fpHandle);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    if (primitive->data)
    {
        CsrUint16 mblkLength = CsrMblkGetLength(primitive->data);
        CsrUint16Ser(buffer, length, mblkLength);
        CsrMblkCopyToMemory(primitive->data, 0, mblkLength, &buffer[*length]);
        *length += mblkLength;
    }
    else
    {
        buffer[*length] = 0;
        buffer[*length + 1] = 0;
        *length += 2;
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_FP_WRITE_REQ */

#ifndef EXCLUDE_CSR_FP_WRITE_REQ
void *CsrFpWriteReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrFpWriteReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrFpWriteReq *) CsrPmemZalloc(sizeof(CsrFpWriteReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->fpHandle, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
    {
        CsrUint8 *mblkData;
        CsrUint16 mblkLength;
        CsrUint16Des((CsrUint16 *) &mblkLength, buffer, &offset);
        primitive->data = CsrMblkMallocCreate((void **) &mblkData, mblkLength);
        CsrMemCpy(mblkData,&buffer[offset], mblkLength);
        offset += mblkLength;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_FP_WRITE_REQ */

#ifndef EXCLUDE_CSR_FP_WRITE_REQ
void CsrFpWriteReqSerFree(void *msg)
{
    CsrFpWriteReq *primitive;

    primitive = (CsrFpWriteReq *) msg;

    if(primitive->data != NULL)
    {
        CsrMblkDestroy(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_FP_WRITE_REQ */

#ifndef EXCLUDE_CSR_FP_DESTROY_CFM
CsrSize CsrFpDestroyCfmSizeof(void *msg)
{
    CsrFpDestroyCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_FP_DESTROY_CFM */

#ifndef EXCLUDE_CSR_FP_DESTROY_CFM
CsrUint8 *CsrFpDestroyCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrFpDestroyCfm *primitive;

    primitive = (CsrFpDestroyCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->fpHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_FP_DESTROY_CFM */

#ifndef EXCLUDE_CSR_FP_DESTROY_CFM
void *CsrFpDestroyCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrFpDestroyCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrFpDestroyCfm *) CsrPmemZalloc(sizeof(CsrFpDestroyCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->fpHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_FP_DESTROY_CFM */

#ifndef EXCLUDE_CSR_FP_CREATE_CFM
CsrSize CsrFpCreateCfmSizeof(void *msg)
{
    CsrFpCreateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_FP_CREATE_CFM */

#ifndef EXCLUDE_CSR_FP_CREATE_CFM
CsrUint8 *CsrFpCreateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrFpCreateCfm *primitive;

    primitive = (CsrFpCreateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->fpHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->overheadController);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->capacityTxController);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->capacityRxController);

    return buffer;
}
#endif /* EXCLUDE_CSR_FP_CREATE_CFM */

#ifndef EXCLUDE_CSR_FP_CREATE_CFM
void *CsrFpCreateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrFpCreateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrFpCreateCfm *) CsrPmemZalloc(sizeof(CsrFpCreateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->fpHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->overheadController, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->capacityTxController, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->capacityRxController, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_FP_CREATE_CFM */

#endif /* EXCLUDE_CSR_FP_MODULE */
