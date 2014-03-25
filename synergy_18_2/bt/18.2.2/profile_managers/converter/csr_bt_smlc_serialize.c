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
#include "csr_bt_smlc_prim.h"

#ifndef EXCLUDE_CSR_BT_SMLC_MODULE

void CsrBtSmlcPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_SMLC_CANCEL_CONNECT_REQ
CsrSize CsrBtSmlcCancelConnectReqSizeof(void *msg)
{
    CsrBtSmlcCancelConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLC_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_CANCEL_CONNECT_REQ
CsrUint8 *CsrBtSmlcCancelConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlcCancelConnectReq *primitive;

    primitive = (CsrBtSmlcCancelConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SMLC_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_CANCEL_CONNECT_REQ
void *CsrBtSmlcCancelConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlcCancelConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlcCancelConnectReq *) CsrPmemZalloc(sizeof(CsrBtSmlcCancelConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SMLC_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_GET_SML_MSG_OBJ_REQ
CsrSize CsrBtSmlcGetSmlMsgObjReqSizeof(void *msg)
{
    CsrBtSmlcGetSmlMsgObjReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLC_GET_SML_MSG_OBJ_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_GET_SML_MSG_OBJ_REQ
CsrUint8 *CsrBtSmlcGetSmlMsgObjReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlcGetSmlMsgObjReq *primitive;

    primitive = (CsrBtSmlcGetSmlMsgObjReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mimeType);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SMLC_GET_SML_MSG_OBJ_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_GET_SML_MSG_OBJ_REQ
void *CsrBtSmlcGetSmlMsgObjReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlcGetSmlMsgObjReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlcGetSmlMsgObjReq *) CsrPmemZalloc(sizeof(CsrBtSmlcGetSmlMsgObjReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mimeType, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SMLC_GET_SML_MSG_OBJ_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_DEACTIVATE_CFM
CsrSize CsrBtSmlcDeactivateCfmSizeof(void *msg)
{
    CsrBtSmlcDeactivateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLC_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_SMLC_DEACTIVATE_CFM
CsrUint8 *CsrBtSmlcDeactivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlcDeactivateCfm *primitive;

    primitive = (CsrBtSmlcDeactivateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SMLC_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_SMLC_DEACTIVATE_CFM
void *CsrBtSmlcDeactivateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlcDeactivateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlcDeactivateCfm *) CsrPmemZalloc(sizeof(CsrBtSmlcDeactivateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SMLC_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_SMLC_GET_NEXT_CHUNK_SML_MSG_OBJ_CFM
CsrSize CsrBtSmlcGetNextChunkSmlMsgObjCfmSizeof(void *msg)
{
    CsrBtSmlcGetNextChunkSmlMsgObjCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLC_GET_NEXT_CHUNK_SML_MSG_OBJ_CFM */

#ifndef EXCLUDE_CSR_BT_SMLC_GET_NEXT_CHUNK_SML_MSG_OBJ_CFM
CsrUint8 *CsrBtSmlcGetNextChunkSmlMsgObjCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlcGetNextChunkSmlMsgObjCfm *primitive;

    primitive = (CsrBtSmlcGetNextChunkSmlMsgObjCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SMLC_GET_NEXT_CHUNK_SML_MSG_OBJ_CFM */

#ifndef EXCLUDE_CSR_BT_SMLC_GET_NEXT_CHUNK_SML_MSG_OBJ_CFM
void *CsrBtSmlcGetNextChunkSmlMsgObjCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlcGetNextChunkSmlMsgObjCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlcGetNextChunkSmlMsgObjCfm *) CsrPmemZalloc(sizeof(CsrBtSmlcGetNextChunkSmlMsgObjCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    CsrUint16Des((CsrUint16 *) &primitive->payloadLength, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_SMLC_GET_NEXT_CHUNK_SML_MSG_OBJ_CFM */

#ifndef EXCLUDE_CSR_BT_SMLC_GET_NEXT_CHUNK_SML_MSG_OBJ_CFM
void CsrBtSmlcGetNextChunkSmlMsgObjCfmSerFree(void *msg)
{
    CsrBtSmlcGetNextChunkSmlMsgObjCfm *primitive;

    primitive = (CsrBtSmlcGetNextChunkSmlMsgObjCfm *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SMLC_GET_NEXT_CHUNK_SML_MSG_OBJ_CFM */

#ifndef EXCLUDE_CSR_BT_SMLC_ACTIVATE_REQ
CsrSize CsrBtSmlcActivateReqSizeof(void *msg)
{
    CsrBtSmlcActivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLC_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_ACTIVATE_REQ
CsrUint8 *CsrBtSmlcActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlcActivateReq *primitive;

    primitive = (CsrBtSmlcActivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->advEnable);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->acceptServConnect);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SMLC_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_ACTIVATE_REQ
void *CsrBtSmlcActivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlcActivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlcActivateReq *) CsrPmemZalloc(sizeof(CsrBtSmlcActivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->advEnable, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->acceptServConnect, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SMLC_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_PUT_NEXT_CHUNK_SML_MSG_OBJ_CFM
CsrSize CsrBtSmlcPutNextChunkSmlMsgObjCfmSizeof(void *msg)
{
    CsrBtSmlcPutNextChunkSmlMsgObjCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLC_PUT_NEXT_CHUNK_SML_MSG_OBJ_CFM */

#ifndef EXCLUDE_CSR_BT_SMLC_PUT_NEXT_CHUNK_SML_MSG_OBJ_CFM
CsrUint8 *CsrBtSmlcPutNextChunkSmlMsgObjCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlcPutNextChunkSmlMsgObjCfm *primitive;

    primitive = (CsrBtSmlcPutNextChunkSmlMsgObjCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxBodySize);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SMLC_PUT_NEXT_CHUNK_SML_MSG_OBJ_CFM */

#ifndef EXCLUDE_CSR_BT_SMLC_PUT_NEXT_CHUNK_SML_MSG_OBJ_CFM
void *CsrBtSmlcPutNextChunkSmlMsgObjCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlcPutNextChunkSmlMsgObjCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlcPutNextChunkSmlMsgObjCfm *) CsrPmemZalloc(sizeof(CsrBtSmlcPutNextChunkSmlMsgObjCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxBodySize, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SMLC_PUT_NEXT_CHUNK_SML_MSG_OBJ_CFM */

#ifndef EXCLUDE_CSR_BT_SMLC_SECURITY_OUT_CFM
CsrSize CsrBtSmlcSecurityOutCfmSizeof(void *msg)
{
    CsrBtSmlcSecurityOutCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLC_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_SMLC_SECURITY_OUT_CFM
CsrUint8 *CsrBtSmlcSecurityOutCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlcSecurityOutCfm *primitive;

    primitive = (CsrBtSmlcSecurityOutCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SMLC_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_SMLC_SECURITY_OUT_CFM
void *CsrBtSmlcSecurityOutCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlcSecurityOutCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlcSecurityOutCfm *) CsrPmemZalloc(sizeof(CsrBtSmlcSecurityOutCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SMLC_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_SMLC_GET_NEXT_CHUNK_SML_MSG_OBJ_REQ
CsrSize CsrBtSmlcGetNextChunkSmlMsgObjReqSizeof(void *msg)
{
    CsrBtSmlcGetNextChunkSmlMsgObjReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLC_GET_NEXT_CHUNK_SML_MSG_OBJ_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_GET_NEXT_CHUNK_SML_MSG_OBJ_REQ
CsrUint8 *CsrBtSmlcGetNextChunkSmlMsgObjReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlcGetNextChunkSmlMsgObjReq *primitive;

    primitive = (CsrBtSmlcGetNextChunkSmlMsgObjReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SMLC_GET_NEXT_CHUNK_SML_MSG_OBJ_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_GET_NEXT_CHUNK_SML_MSG_OBJ_REQ
void *CsrBtSmlcGetNextChunkSmlMsgObjReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlcGetNextChunkSmlMsgObjReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlcGetNextChunkSmlMsgObjReq *) CsrPmemZalloc(sizeof(CsrBtSmlcGetNextChunkSmlMsgObjReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SMLC_GET_NEXT_CHUNK_SML_MSG_OBJ_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_AUTHENTICATE_RES
CsrSize CsrBtSmlcAuthenticateResSizeof(void *msg)
{
    CsrBtSmlcAuthenticateRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->passwordLength * sizeof(CsrUint8)) + CsrCharStringSerLen((CsrCharString *) primitive->userId));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLC_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_SMLC_AUTHENTICATE_RES
CsrUint8 *CsrBtSmlcAuthenticateResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlcAuthenticateRes *primitive;

    primitive = (CsrBtSmlcAuthenticateRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->passwordLength);
    CsrMemCpySer(buffer, length, &primitive->password, sizeof(primitive->password));
    CsrMemCpySer(buffer, length, &primitive->userId, sizeof(primitive->userId));
    if(primitive->password)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->password), ((CsrSize) (primitive->passwordLength * sizeof(CsrUint8))));
    }
    CsrCharStringSer(buffer, length, primitive->userId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SMLC_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_SMLC_AUTHENTICATE_RES
void *CsrBtSmlcAuthenticateResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlcAuthenticateRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlcAuthenticateRes *) CsrPmemZalloc(sizeof(CsrBtSmlcAuthenticateRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->passwordLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->password), buffer, &offset, ((CsrSize) sizeof(primitive->password)));
    CsrMemCpyDes(((void *) &primitive->userId), buffer, &offset, ((CsrSize) sizeof(primitive->userId)));
    if (primitive->passwordLength)
    {
        primitive->password = CsrPmemZalloc(((CsrUint32) (primitive->passwordLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->password), buffer, &offset, ((CsrSize) (primitive->passwordLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->password = NULL;
    }
    CsrCharStringDes(&primitive->userId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SMLC_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_SMLC_AUTHENTICATE_RES
void CsrBtSmlcAuthenticateResSerFree(void *msg)
{
    CsrBtSmlcAuthenticateRes *primitive;

    primitive = (CsrBtSmlcAuthenticateRes *) msg;

    if(primitive->password != NULL)
    {
        CsrPmemFree(primitive->password);
    }
    if(primitive->userId != NULL)
    {
        CsrPmemFree(primitive->userId);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SMLC_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_SMLC_GET_SML_MSG_OBJ_CFM
CsrSize CsrBtSmlcGetSmlMsgObjCfmSizeof(void *msg)
{
    CsrBtSmlcGetSmlMsgObjCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLC_GET_SML_MSG_OBJ_CFM */

#ifndef EXCLUDE_CSR_BT_SMLC_GET_SML_MSG_OBJ_CFM
CsrUint8 *CsrBtSmlcGetSmlMsgObjCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlcGetSmlMsgObjCfm *primitive;

    primitive = (CsrBtSmlcGetSmlMsgObjCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->lengthOfObject);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SMLC_GET_SML_MSG_OBJ_CFM */

#ifndef EXCLUDE_CSR_BT_SMLC_GET_SML_MSG_OBJ_CFM
void *CsrBtSmlcGetSmlMsgObjCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlcGetSmlMsgObjCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlcGetSmlMsgObjCfm *) CsrPmemZalloc(sizeof(CsrBtSmlcGetSmlMsgObjCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->lengthOfObject, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    CsrUint16Des((CsrUint16 *) &primitive->payloadLength, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_SMLC_GET_SML_MSG_OBJ_CFM */

#ifndef EXCLUDE_CSR_BT_SMLC_GET_SML_MSG_OBJ_CFM
void CsrBtSmlcGetSmlMsgObjCfmSerFree(void *msg)
{
    CsrBtSmlcGetSmlMsgObjCfm *primitive;

    primitive = (CsrBtSmlcGetSmlMsgObjCfm *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SMLC_GET_SML_MSG_OBJ_CFM */

#ifndef EXCLUDE_CSR_BT_SMLC_PUT_SML_MSG_OBJ_REQ
CsrSize CsrBtSmlcPutSmlMsgObjReqSizeof(void *msg)
{
    CsrBtSmlcPutSmlMsgObjReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->bodyLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLC_PUT_SML_MSG_OBJ_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_PUT_SML_MSG_OBJ_REQ
CsrUint8 *CsrBtSmlcPutSmlMsgObjReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlcPutSmlMsgObjReq *primitive;

    primitive = (CsrBtSmlcPutSmlMsgObjReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mimeType);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->lengthOfObject);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyLength);
    CsrMemCpySer(buffer, length, &primitive->body, sizeof(primitive->body));
    if(primitive->body)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->body), ((CsrSize) (primitive->bodyLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SMLC_PUT_SML_MSG_OBJ_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_PUT_SML_MSG_OBJ_REQ
void *CsrBtSmlcPutSmlMsgObjReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlcPutSmlMsgObjReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlcPutSmlMsgObjReq *) CsrPmemZalloc(sizeof(CsrBtSmlcPutSmlMsgObjReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mimeType, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->lengthOfObject, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->body), buffer, &offset, ((CsrSize) sizeof(primitive->body)));
    if (primitive->bodyLength)
    {
        primitive->body = CsrPmemZalloc(((CsrUint32) (primitive->bodyLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->body), buffer, &offset, ((CsrSize) (primitive->bodyLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->body = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SMLC_PUT_SML_MSG_OBJ_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_PUT_SML_MSG_OBJ_REQ
void CsrBtSmlcPutSmlMsgObjReqSerFree(void *msg)
{
    CsrBtSmlcPutSmlMsgObjReq *primitive;

    primitive = (CsrBtSmlcPutSmlMsgObjReq *) msg;

    if(primitive->body != NULL)
    {
        CsrPmemFree(primitive->body);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SMLC_PUT_SML_MSG_OBJ_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_ABORT_REQ
CsrSize CsrBtSmlcAbortReqSizeof(void *msg)
{
    CsrBtSmlcAbortReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLC_ABORT_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_ABORT_REQ
CsrUint8 *CsrBtSmlcAbortReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlcAbortReq *primitive;

    primitive = (CsrBtSmlcAbortReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SMLC_ABORT_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_ABORT_REQ
void *CsrBtSmlcAbortReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlcAbortReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlcAbortReq *) CsrPmemZalloc(sizeof(CsrBtSmlcAbortReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SMLC_ABORT_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_ABORT_CFM
CsrSize CsrBtSmlcAbortCfmSizeof(void *msg)
{
    CsrBtSmlcAbortCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLC_ABORT_CFM */

#ifndef EXCLUDE_CSR_BT_SMLC_ABORT_CFM
CsrUint8 *CsrBtSmlcAbortCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlcAbortCfm *primitive;

    primitive = (CsrBtSmlcAbortCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SMLC_ABORT_CFM */

#ifndef EXCLUDE_CSR_BT_SMLC_ABORT_CFM
void *CsrBtSmlcAbortCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlcAbortCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlcAbortCfm *) CsrPmemZalloc(sizeof(CsrBtSmlcAbortCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SMLC_ABORT_CFM */

#ifndef EXCLUDE_CSR_BT_SMLC_DISCONNECT_IND
CsrSize CsrBtSmlcDisconnectIndSizeof(void *msg)
{
    CsrBtSmlcDisconnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLC_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SMLC_DISCONNECT_IND
CsrUint8 *CsrBtSmlcDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlcDisconnectInd *primitive;

    primitive = (CsrBtSmlcDisconnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SMLC_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SMLC_DISCONNECT_IND
void *CsrBtSmlcDisconnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlcDisconnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlcDisconnectInd *) CsrPmemZalloc(sizeof(CsrBtSmlcDisconnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SMLC_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SMLC_SECURITY_OUT_REQ
CsrSize CsrBtSmlcSecurityOutReqSizeof(void *msg)
{
    CsrBtSmlcSecurityOutReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLC_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_SECURITY_OUT_REQ
CsrUint8 *CsrBtSmlcSecurityOutReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlcSecurityOutReq *primitive;

    primitive = (CsrBtSmlcSecurityOutReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SMLC_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_SECURITY_OUT_REQ
void *CsrBtSmlcSecurityOutReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlcSecurityOutReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlcSecurityOutReq *) CsrPmemZalloc(sizeof(CsrBtSmlcSecurityOutReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SMLC_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_DEACTIVATE_REQ
CsrSize CsrBtSmlcDeactivateReqSizeof(void *msg)
{
    CsrBtSmlcDeactivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLC_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_DEACTIVATE_REQ
CsrUint8 *CsrBtSmlcDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlcDeactivateReq *primitive;

    primitive = (CsrBtSmlcDeactivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SMLC_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_DEACTIVATE_REQ
void *CsrBtSmlcDeactivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlcDeactivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlcDeactivateReq *) CsrPmemZalloc(sizeof(CsrBtSmlcDeactivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SMLC_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_CONNECT_CFM
CsrSize CsrBtSmlcConnectCfmSizeof(void *msg)
{
    CsrBtSmlcConnectCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLC_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_SMLC_CONNECT_CFM
CsrUint8 *CsrBtSmlcConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlcConnectCfm *primitive;

    primitive = (CsrBtSmlcConnectCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->obexPeerMaxPacketSize);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->availablePutBodySize);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SMLC_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_SMLC_CONNECT_CFM
void *CsrBtSmlcConnectCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlcConnectCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlcConnectCfm *) CsrPmemZalloc(sizeof(CsrBtSmlcConnectCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->obexPeerMaxPacketSize, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->availablePutBodySize, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SMLC_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_SMLC_PUT_SML_MSG_OBJ_CFM
CsrSize CsrBtSmlcPutSmlMsgObjCfmSizeof(void *msg)
{
    CsrBtSmlcPutSmlMsgObjCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLC_PUT_SML_MSG_OBJ_CFM */

#ifndef EXCLUDE_CSR_BT_SMLC_PUT_SML_MSG_OBJ_CFM
CsrUint8 *CsrBtSmlcPutSmlMsgObjCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlcPutSmlMsgObjCfm *primitive;

    primitive = (CsrBtSmlcPutSmlMsgObjCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxBodySize);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SMLC_PUT_SML_MSG_OBJ_CFM */

#ifndef EXCLUDE_CSR_BT_SMLC_PUT_SML_MSG_OBJ_CFM
void *CsrBtSmlcPutSmlMsgObjCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlcPutSmlMsgObjCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlcPutSmlMsgObjCfm *) CsrPmemZalloc(sizeof(CsrBtSmlcPutSmlMsgObjCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxBodySize, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SMLC_PUT_SML_MSG_OBJ_CFM */

#ifndef EXCLUDE_CSR_BT_SMLC_SECURITY_IN_REQ
CsrSize CsrBtSmlcSecurityInReqSizeof(void *msg)
{
    CsrBtSmlcSecurityInReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLC_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_SECURITY_IN_REQ
CsrUint8 *CsrBtSmlcSecurityInReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlcSecurityInReq *primitive;

    primitive = (CsrBtSmlcSecurityInReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SMLC_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_SECURITY_IN_REQ
void *CsrBtSmlcSecurityInReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlcSecurityInReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlcSecurityInReq *) CsrPmemZalloc(sizeof(CsrBtSmlcSecurityInReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SMLC_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_ACTIVATE_CFM
CsrSize CsrBtSmlcActivateCfmSizeof(void *msg)
{
    CsrBtSmlcActivateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLC_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_SMLC_ACTIVATE_CFM
CsrUint8 *CsrBtSmlcActivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlcActivateCfm *primitive;

    primitive = (CsrBtSmlcActivateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->advEnabled);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->acceptEnabled);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SMLC_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_SMLC_ACTIVATE_CFM
void *CsrBtSmlcActivateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlcActivateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlcActivateCfm *) CsrPmemZalloc(sizeof(CsrBtSmlcActivateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->advEnabled, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->acceptEnabled, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SMLC_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_SMLC_PUT_NEXT_CHUNK_SML_MSG_OBJ_REQ
CsrSize CsrBtSmlcPutNextChunkSmlMsgObjReqSizeof(void *msg)
{
    CsrBtSmlcPutNextChunkSmlMsgObjReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->bodyLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLC_PUT_NEXT_CHUNK_SML_MSG_OBJ_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_PUT_NEXT_CHUNK_SML_MSG_OBJ_REQ
CsrUint8 *CsrBtSmlcPutNextChunkSmlMsgObjReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlcPutNextChunkSmlMsgObjReq *primitive;

    primitive = (CsrBtSmlcPutNextChunkSmlMsgObjReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyLength);
    CsrMemCpySer(buffer, length, &primitive->body, sizeof(primitive->body));
    if(primitive->body)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->body), ((CsrSize) (primitive->bodyLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SMLC_PUT_NEXT_CHUNK_SML_MSG_OBJ_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_PUT_NEXT_CHUNK_SML_MSG_OBJ_REQ
void *CsrBtSmlcPutNextChunkSmlMsgObjReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlcPutNextChunkSmlMsgObjReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlcPutNextChunkSmlMsgObjReq *) CsrPmemZalloc(sizeof(CsrBtSmlcPutNextChunkSmlMsgObjReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->body), buffer, &offset, ((CsrSize) sizeof(primitive->body)));
    if (primitive->bodyLength)
    {
        primitive->body = CsrPmemZalloc(((CsrUint32) (primitive->bodyLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->body), buffer, &offset, ((CsrSize) (primitive->bodyLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->body = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SMLC_PUT_NEXT_CHUNK_SML_MSG_OBJ_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_PUT_NEXT_CHUNK_SML_MSG_OBJ_REQ
void CsrBtSmlcPutNextChunkSmlMsgObjReqSerFree(void *msg)
{
    CsrBtSmlcPutNextChunkSmlMsgObjReq *primitive;

    primitive = (CsrBtSmlcPutNextChunkSmlMsgObjReq *) msg;

    if(primitive->body != NULL)
    {
        CsrPmemFree(primitive->body);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SMLC_PUT_NEXT_CHUNK_SML_MSG_OBJ_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_CONNECT_REQ
CsrSize CsrBtSmlcConnectReqSizeof(void *msg)
{
    CsrBtSmlcConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->realmLength * sizeof(CsrUint8)) + (primitive->passwordLength * sizeof(CsrUint8)) + CsrCharStringSerLen((CsrCharString *) primitive->userId));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_CONNECT_REQ
CsrUint8 *CsrBtSmlcConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlcConnectReq *primitive;

    primitive = (CsrBtSmlcConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxPacketSize);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->destination.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->destination.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->destination.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->targetService);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->authorize);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->realmLength);
    CsrMemCpySer(buffer, length, &primitive->realm, sizeof(primitive->realm));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->passwordLength);
    CsrMemCpySer(buffer, length, &primitive->password, sizeof(primitive->password));
    CsrMemCpySer(buffer, length, &primitive->userId, sizeof(primitive->userId));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->count);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->windowSize);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmEnable);
    if(primitive->realm)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->realm), ((CsrSize) (primitive->realmLength * sizeof(CsrUint8))));
    }
    if(primitive->password)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->password), ((CsrSize) (primitive->passwordLength * sizeof(CsrUint8))));
    }
    CsrCharStringSer(buffer, length, primitive->userId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SMLC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_CONNECT_REQ
void *CsrBtSmlcConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlcConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlcConnectReq *) CsrPmemZalloc(sizeof(CsrBtSmlcConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxPacketSize, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->destination.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->destination.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->destination.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->targetService, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->authorize, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->realmLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->realm), buffer, &offset, ((CsrSize) sizeof(primitive->realm)));
    CsrUint16Des((CsrUint16 *) &primitive->passwordLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->password), buffer, &offset, ((CsrSize) sizeof(primitive->password)));
    CsrMemCpyDes(((void *) &primitive->userId), buffer, &offset, ((CsrSize) sizeof(primitive->userId)));
    CsrUint32Des((CsrUint32 *) &primitive->count, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->windowSize, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmEnable, buffer, &offset);
    if (primitive->realmLength)
    {
        primitive->realm = CsrPmemZalloc(((CsrUint32) (primitive->realmLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->realm), buffer, &offset, ((CsrSize) (primitive->realmLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->realm = NULL;
    }
    if (primitive->passwordLength)
    {
        primitive->password = CsrPmemZalloc(((CsrUint32) (primitive->passwordLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->password), buffer, &offset, ((CsrSize) (primitive->passwordLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->password = NULL;
    }
    CsrCharStringDes(&primitive->userId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SMLC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_CONNECT_REQ
void CsrBtSmlcConnectReqSerFree(void *msg)
{
    CsrBtSmlcConnectReq *primitive;

    primitive = (CsrBtSmlcConnectReq *) msg;

    if(primitive->realm != NULL)
    {
        CsrPmemFree(primitive->realm);
    }
    if(primitive->password != NULL)
    {
        CsrPmemFree(primitive->password);
    }
    if(primitive->userId != NULL)
    {
        CsrPmemFree(primitive->userId);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SMLC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_AUTHENTICATE_IND
CsrSize CsrBtSmlcAuthenticateIndSizeof(void *msg)
{
    CsrBtSmlcAuthenticateInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->realmLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLC_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_SMLC_AUTHENTICATE_IND
CsrUint8 *CsrBtSmlcAuthenticateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlcAuthenticateInd *primitive;

    primitive = (CsrBtSmlcAuthenticateInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->options);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->realmLength);
    CsrMemCpySer(buffer, length, &primitive->realm, sizeof(primitive->realm));
    if(primitive->realm)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->realm), ((CsrSize) (primitive->realmLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SMLC_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_SMLC_AUTHENTICATE_IND
void *CsrBtSmlcAuthenticateIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlcAuthenticateInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlcAuthenticateInd *) CsrPmemZalloc(sizeof(CsrBtSmlcAuthenticateInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->options, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->realmLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->realm), buffer, &offset, ((CsrSize) sizeof(primitive->realm)));
    if (primitive->realmLength)
    {
        primitive->realm = CsrPmemZalloc(((CsrUint32) (primitive->realmLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->realm), buffer, &offset, ((CsrSize) (primitive->realmLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->realm = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SMLC_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_SMLC_AUTHENTICATE_IND
void CsrBtSmlcAuthenticateIndSerFree(void *msg)
{
    CsrBtSmlcAuthenticateInd *primitive;

    primitive = (CsrBtSmlcAuthenticateInd *) msg;

    if(primitive->realm != NULL)
    {
        CsrPmemFree(primitive->realm);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SMLC_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_SMLC_SECURITY_IN_CFM
CsrSize CsrBtSmlcSecurityInCfmSizeof(void *msg)
{
    CsrBtSmlcSecurityInCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLC_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_SMLC_SECURITY_IN_CFM
CsrUint8 *CsrBtSmlcSecurityInCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlcSecurityInCfm *primitive;

    primitive = (CsrBtSmlcSecurityInCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SMLC_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_SMLC_SECURITY_IN_CFM
void *CsrBtSmlcSecurityInCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlcSecurityInCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlcSecurityInCfm *) CsrPmemZalloc(sizeof(CsrBtSmlcSecurityInCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SMLC_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_SMLC_DISCONNECT_REQ
CsrSize CsrBtSmlcDisconnectReqSizeof(void *msg)
{
    CsrBtSmlcDisconnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLC_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_DISCONNECT_REQ
CsrUint8 *CsrBtSmlcDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlcDisconnectReq *primitive;

    primitive = (CsrBtSmlcDisconnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->normalDisconnect);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SMLC_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_DISCONNECT_REQ
void *CsrBtSmlcDisconnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlcDisconnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlcDisconnectReq *) CsrPmemZalloc(sizeof(CsrBtSmlcDisconnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->normalDisconnect, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SMLC_DISCONNECT_REQ */

#endif /* EXCLUDE_CSR_BT_SMLC_MODULE */
