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
#include "csr_hq_prim.h"

#ifndef EXCLUDE_CSR_HQ_MODULE

void CsrHqPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_HQ_DEREGISTER_REQ
CsrSize CsrHqDeregisterReqSizeof(void *msg)
{
    CsrHqDeregisterReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_HQ_DEREGISTER_REQ */

#ifndef EXCLUDE_CSR_HQ_DEREGISTER_REQ
CsrUint8 *CsrHqDeregisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrHqDeregisterReq *primitive;

    primitive = (CsrHqDeregisterReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->varId);

    return buffer;
}
#endif /* EXCLUDE_CSR_HQ_DEREGISTER_REQ */

#ifndef EXCLUDE_CSR_HQ_DEREGISTER_REQ
void *CsrHqDeregisterReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrHqDeregisterReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrHqDeregisterReq *) CsrPmemZalloc(sizeof(CsrHqDeregisterReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->varId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_HQ_DEREGISTER_REQ */

#ifndef EXCLUDE_CSR_HQ_MSG_RES
CsrSize CsrHqMsgResSizeof(void *msg)
{
    CsrHqMsgRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_HQ_MSG_RES */

#ifndef EXCLUDE_CSR_HQ_MSG_RES
CsrUint8 *CsrHqMsgResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrHqMsgRes *primitive;

    primitive = (CsrHqMsgRes *) msg;
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
#endif /* EXCLUDE_CSR_HQ_MSG_RES */

#ifndef EXCLUDE_CSR_HQ_MSG_RES
void *CsrHqMsgResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrHqMsgRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrHqMsgRes *) CsrPmemZalloc(sizeof(CsrHqMsgRes));

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
#endif /* EXCLUDE_CSR_HQ_MSG_RES */

#ifndef EXCLUDE_CSR_HQ_MSG_RES
void CsrHqMsgResSerFree(void *msg)
{
    CsrHqMsgRes *primitive;

    primitive = (CsrHqMsgRes *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_HQ_MSG_RES */

#ifndef EXCLUDE_CSR_HQ_DEREGISTER_CFM
CsrSize CsrHqDeregisterCfmSizeof(void *msg)
{
    CsrHqDeregisterCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_HQ_DEREGISTER_CFM */

#ifndef EXCLUDE_CSR_HQ_DEREGISTER_CFM
CsrUint8 *CsrHqDeregisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrHqDeregisterCfm *primitive;

    primitive = (CsrHqDeregisterCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->varId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_HQ_DEREGISTER_CFM */

#ifndef EXCLUDE_CSR_HQ_DEREGISTER_CFM
void *CsrHqDeregisterCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrHqDeregisterCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrHqDeregisterCfm *) CsrPmemZalloc(sizeof(CsrHqDeregisterCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->varId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_HQ_DEREGISTER_CFM */

#ifndef EXCLUDE_CSR_HQ_REGISTER_REQ
CsrSize CsrHqRegisterReqSizeof(void *msg)
{
    CsrHqRegisterReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_HQ_REGISTER_REQ */

#ifndef EXCLUDE_CSR_HQ_REGISTER_REQ
CsrUint8 *CsrHqRegisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrHqRegisterReq *primitive;

    primitive = (CsrHqRegisterReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->varId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->response);

    return buffer;
}
#endif /* EXCLUDE_CSR_HQ_REGISTER_REQ */

#ifndef EXCLUDE_CSR_HQ_REGISTER_REQ
void *CsrHqRegisterReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrHqRegisterReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrHqRegisterReq *) CsrPmemZalloc(sizeof(CsrHqRegisterReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->varId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->response, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_HQ_REGISTER_REQ */

#ifndef EXCLUDE_CSR_HQ_REGISTER_CFM
CsrSize CsrHqRegisterCfmSizeof(void *msg)
{
    CsrHqRegisterCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_HQ_REGISTER_CFM */

#ifndef EXCLUDE_CSR_HQ_REGISTER_CFM
CsrUint8 *CsrHqRegisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrHqRegisterCfm *primitive;

    primitive = (CsrHqRegisterCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->varId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_HQ_REGISTER_CFM */

#ifndef EXCLUDE_CSR_HQ_REGISTER_CFM
void *CsrHqRegisterCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrHqRegisterCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrHqRegisterCfm *) CsrPmemZalloc(sizeof(CsrHqRegisterCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->varId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_HQ_REGISTER_CFM */

#ifndef EXCLUDE_CSR_HQ_MSG_IND
CsrSize CsrHqMsgIndSizeof(void *msg)
{
    CsrHqMsgInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_HQ_MSG_IND */

#ifndef EXCLUDE_CSR_HQ_MSG_IND
CsrUint8 *CsrHqMsgIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrHqMsgInd *primitive;

    primitive = (CsrHqMsgInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cmdType);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->seqNo);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->varId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_HQ_MSG_IND */

#ifndef EXCLUDE_CSR_HQ_MSG_IND
void *CsrHqMsgIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrHqMsgInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrHqMsgInd *) CsrPmemZalloc(sizeof(CsrHqMsgInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cmdType, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->seqNo, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->varId, buffer, &offset);
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
#endif /* EXCLUDE_CSR_HQ_MSG_IND */

#ifndef EXCLUDE_CSR_HQ_MSG_IND
void CsrHqMsgIndSerFree(void *msg)
{
    CsrHqMsgInd *primitive;

    primitive = (CsrHqMsgInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_HQ_MSG_IND */

#endif /* EXCLUDE_CSR_HQ_MODULE */
