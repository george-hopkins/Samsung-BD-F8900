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
#include "csr_bccmd_private_prim.h"

#ifndef EXCLUDE_CSR_BCCMD_PRIVATE_MODULE

void CsrBccmdPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BCCMD_REQ
CsrSize CsrBccmdReqSizeof(void *msg)
{
    CsrBccmdReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BCCMD_REQ */

#ifndef EXCLUDE_CSR_BCCMD_REQ
CsrUint8 *CsrBccmdReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBccmdReq *primitive;

    primitive = (CsrBccmdReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BCCMD_REQ */

#ifndef EXCLUDE_CSR_BCCMD_REQ
void *CsrBccmdReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBccmdReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBccmdReq *) CsrPmemZalloc(sizeof(CsrBccmdReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BCCMD_REQ */

#ifndef EXCLUDE_CSR_BCCMD_REQ
void CsrBccmdReqSerFree(void *msg)
{
    CsrBccmdReq *primitive;

    primitive = (CsrBccmdReq *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BCCMD_REQ */

#ifndef EXCLUDE_CSR_BCCMD_QUERY_REQ
CsrSize CsrBccmdQueryReqSizeof(void *msg)
{
    CsrBccmdQueryReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BCCMD_QUERY_REQ */

#ifndef EXCLUDE_CSR_BCCMD_QUERY_REQ
CsrUint8 *CsrBccmdQueryReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBccmdQueryReq *primitive;

    primitive = (CsrBccmdQueryReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BCCMD_QUERY_REQ */

#ifndef EXCLUDE_CSR_BCCMD_QUERY_REQ
void *CsrBccmdQueryReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBccmdQueryReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBccmdQueryReq *) CsrPmemZalloc(sizeof(CsrBccmdQueryReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BCCMD_QUERY_REQ */

#ifndef EXCLUDE_CSR_BCCMD_FLUSH_REQ
CsrSize CsrBccmdFlushReqSizeof(void *msg)
{
    CsrBccmdFlushReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BCCMD_FLUSH_REQ */

#ifndef EXCLUDE_CSR_BCCMD_FLUSH_REQ
CsrUint8 *CsrBccmdFlushReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBccmdFlushReq *primitive;

    primitive = (CsrBccmdFlushReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BCCMD_FLUSH_REQ */

#ifndef EXCLUDE_CSR_BCCMD_FLUSH_REQ
void *CsrBccmdFlushReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBccmdFlushReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBccmdFlushReq *) CsrPmemZalloc(sizeof(CsrBccmdFlushReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BCCMD_FLUSH_REQ */

#ifndef EXCLUDE_CSR_BCCMD_RESET_IND
CsrSize CsrBccmdResetIndSizeof(void *msg)
{
    CsrBccmdResetInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BCCMD_RESET_IND */

#ifndef EXCLUDE_CSR_BCCMD_RESET_IND
CsrUint8 *CsrBccmdResetIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBccmdResetInd *primitive;

    primitive = (CsrBccmdResetInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BCCMD_RESET_IND */

#ifndef EXCLUDE_CSR_BCCMD_RESET_IND
void *CsrBccmdResetIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBccmdResetInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBccmdResetInd *) CsrPmemZalloc(sizeof(CsrBccmdResetInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BCCMD_RESET_IND */

#ifndef EXCLUDE_CSR_BCCMD_BOOTSTRAP_CFM
CsrSize CsrBccmdBootstrapCfmSizeof(void *msg)
{
    CsrBccmdBootstrapCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BCCMD_BOOTSTRAP_CFM */

#ifndef EXCLUDE_CSR_BCCMD_BOOTSTRAP_CFM
CsrUint8 *CsrBccmdBootstrapCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBccmdBootstrapCfm *primitive;

    primitive = (CsrBccmdBootstrapCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_CSR_BCCMD_BOOTSTRAP_CFM */

#ifndef EXCLUDE_CSR_BCCMD_BOOTSTRAP_CFM
void *CsrBccmdBootstrapCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBccmdBootstrapCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBccmdBootstrapCfm *) CsrPmemZalloc(sizeof(CsrBccmdBootstrapCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BCCMD_BOOTSTRAP_CFM */

#ifndef EXCLUDE_CSR_BCCMD_PANIC_IND
CsrSize CsrBccmdPanicIndSizeof(void *msg)
{
    CsrBccmdPanicInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BCCMD_PANIC_IND */

#ifndef EXCLUDE_CSR_BCCMD_PANIC_IND
CsrUint8 *CsrBccmdPanicIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBccmdPanicInd *primitive;

    primitive = (CsrBccmdPanicInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BCCMD_PANIC_IND */

#ifndef EXCLUDE_CSR_BCCMD_PANIC_IND
void *CsrBccmdPanicIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBccmdPanicInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBccmdPanicInd *) CsrPmemZalloc(sizeof(CsrBccmdPanicInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BCCMD_PANIC_IND */

#ifndef EXCLUDE_CSR_BCCMD_RESET_RES
CsrSize CsrBccmdResetResSizeof(void *msg)
{
    CsrBccmdResetRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BCCMD_RESET_RES */

#ifndef EXCLUDE_CSR_BCCMD_RESET_RES
CsrUint8 *CsrBccmdResetResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBccmdResetRes *primitive;

    primitive = (CsrBccmdResetRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_CSR_BCCMD_RESET_RES */

#ifndef EXCLUDE_CSR_BCCMD_RESET_RES
void *CsrBccmdResetResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBccmdResetRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBccmdResetRes *) CsrPmemZalloc(sizeof(CsrBccmdResetRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BCCMD_RESET_RES */

#ifndef EXCLUDE_CSR_BCCMD_GET_BUILD_ID_REQ
CsrSize CsrBccmdGetBuildIdReqSizeof(void *msg)
{
    CsrBccmdGetBuildIdReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BCCMD_GET_BUILD_ID_REQ */

#ifndef EXCLUDE_CSR_BCCMD_GET_BUILD_ID_REQ
CsrUint8 *CsrBccmdGetBuildIdReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBccmdGetBuildIdReq *primitive;

    primitive = (CsrBccmdGetBuildIdReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BCCMD_GET_BUILD_ID_REQ */

#ifndef EXCLUDE_CSR_BCCMD_GET_BUILD_ID_REQ
void *CsrBccmdGetBuildIdReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBccmdGetBuildIdReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBccmdGetBuildIdReq *) CsrPmemZalloc(sizeof(CsrBccmdGetBuildIdReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BCCMD_GET_BUILD_ID_REQ */

#ifndef EXCLUDE_CSR_BCCMD_GET_BUILD_ID_CFM
CsrSize CsrBccmdGetBuildIdCfmSizeof(void *msg)
{
    CsrBccmdGetBuildIdCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BCCMD_GET_BUILD_ID_CFM */

#ifndef EXCLUDE_CSR_BCCMD_GET_BUILD_ID_CFM
CsrUint8 *CsrBccmdGetBuildIdCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBccmdGetBuildIdCfm *primitive;

    primitive = (CsrBccmdGetBuildIdCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->status);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->buildId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BCCMD_GET_BUILD_ID_CFM */

#ifndef EXCLUDE_CSR_BCCMD_GET_BUILD_ID_CFM
void *CsrBccmdGetBuildIdCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBccmdGetBuildIdCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBccmdGetBuildIdCfm *) CsrPmemZalloc(sizeof(CsrBccmdGetBuildIdCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->status, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->buildId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BCCMD_GET_BUILD_ID_CFM */

#ifndef EXCLUDE_CSR_BCCMD_FLUSH_CFM
CsrSize CsrBccmdFlushCfmSizeof(void *msg)
{
    CsrBccmdFlushCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BCCMD_FLUSH_CFM */

#ifndef EXCLUDE_CSR_BCCMD_FLUSH_CFM
CsrUint8 *CsrBccmdFlushCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBccmdFlushCfm *primitive;

    primitive = (CsrBccmdFlushCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_BCCMD_FLUSH_CFM */

#ifndef EXCLUDE_CSR_BCCMD_FLUSH_CFM
void *CsrBccmdFlushCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBccmdFlushCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBccmdFlushCfm *) CsrPmemZalloc(sizeof(CsrBccmdFlushCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BCCMD_FLUSH_CFM */

#ifndef EXCLUDE_CSR_BCCMD_CFM
CsrSize CsrBccmdCfmSizeof(void *msg)
{
    CsrBccmdCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BCCMD_CFM */

#ifndef EXCLUDE_CSR_BCCMD_CFM
CsrUint8 *CsrBccmdCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBccmdCfm *primitive;

    primitive = (CsrBccmdCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cmdType);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->seqNo);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->varId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->status);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BCCMD_CFM */

#ifndef EXCLUDE_CSR_BCCMD_CFM
void *CsrBccmdCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBccmdCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBccmdCfm *) CsrPmemZalloc(sizeof(CsrBccmdCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cmdType, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->seqNo, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->varId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->status, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BCCMD_CFM */

#ifndef EXCLUDE_CSR_BCCMD_CFM
void CsrBccmdCfmSerFree(void *msg)
{
    CsrBccmdCfm *primitive;

    primitive = (CsrBccmdCfm *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BCCMD_CFM */

#ifndef EXCLUDE_CSR_BCCMD_PANIC_ARGS_REQ
CsrSize CsrBccmdPanicArgsReqSizeof(void *msg)
{
    CsrBccmdPanicArgsReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BCCMD_PANIC_ARGS_REQ */

#ifndef EXCLUDE_CSR_BCCMD_PANIC_ARGS_REQ
CsrUint8 *CsrBccmdPanicArgsReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBccmdPanicArgsReq *primitive;

    primitive = (CsrBccmdPanicArgsReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BCCMD_PANIC_ARGS_REQ */

#ifndef EXCLUDE_CSR_BCCMD_PANIC_ARGS_REQ
void *CsrBccmdPanicArgsReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBccmdPanicArgsReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBccmdPanicArgsReq *) CsrPmemZalloc(sizeof(CsrBccmdPanicArgsReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BCCMD_PANIC_ARGS_REQ */

#ifndef EXCLUDE_CSR_BCCMD_BOOTSTRAP_REQ
CsrSize CsrBccmdBootstrapReqSizeof(void *msg)
{
    CsrBccmdBootstrapReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BCCMD_BOOTSTRAP_REQ */

#ifndef EXCLUDE_CSR_BCCMD_BOOTSTRAP_REQ
CsrUint8 *CsrBccmdBootstrapReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBccmdBootstrapReq *primitive;

    primitive = (CsrBccmdBootstrapReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BCCMD_BOOTSTRAP_REQ */

#ifndef EXCLUDE_CSR_BCCMD_BOOTSTRAP_REQ
void *CsrBccmdBootstrapReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBccmdBootstrapReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBccmdBootstrapReq *) CsrPmemZalloc(sizeof(CsrBccmdBootstrapReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BCCMD_BOOTSTRAP_REQ */

#ifndef EXCLUDE_CSR_BCCMD_BOOTSTRAP_REQ
void CsrBccmdBootstrapReqSerFree(void *msg)
{
    CsrBccmdBootstrapReq *primitive;

    primitive = (CsrBccmdBootstrapReq *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BCCMD_BOOTSTRAP_REQ */

#ifndef EXCLUDE_CSR_BCCMD_PANIC_ARGS_CFM
CsrSize CsrBccmdPanicArgsCfmSizeof(void *msg)
{
    CsrBccmdPanicArgsCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BCCMD_PANIC_ARGS_CFM */

#ifndef EXCLUDE_CSR_BCCMD_PANIC_ARGS_CFM
CsrUint8 *CsrBccmdPanicArgsCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBccmdPanicArgsCfm *primitive;

    primitive = (CsrBccmdPanicArgsCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->panicStatus);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->panicArg);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->faultStatus);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->faultArg);

    return buffer;
}
#endif /* EXCLUDE_CSR_BCCMD_PANIC_ARGS_CFM */

#ifndef EXCLUDE_CSR_BCCMD_PANIC_ARGS_CFM
void *CsrBccmdPanicArgsCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBccmdPanicArgsCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBccmdPanicArgsCfm *) CsrPmemZalloc(sizeof(CsrBccmdPanicArgsCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->panicStatus, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->panicArg, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->faultStatus, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->faultArg, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BCCMD_PANIC_ARGS_CFM */

#endif /* EXCLUDE_CSR_BCCMD_PRIVATE_MODULE */
