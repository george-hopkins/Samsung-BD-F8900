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
#include "csr_bt_smls_prim.h"

#ifndef EXCLUDE_CSR_BT_SMLS_MODULE

void CsrBtSmlsPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_SMLS_CONNECT_IND
CsrSize CsrBtSmlsConnectIndSizeof(void *msg)
{
    CsrBtSmlsConnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLS_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SMLS_CONNECT_IND
CsrUint8 *CsrBtSmlsConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlsConnectInd *primitive;

    primitive = (CsrBtSmlsConnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->obexPeerMaxPacketSize);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->targetService);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->length);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->count);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SMLS_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SMLS_CONNECT_IND
void *CsrBtSmlsConnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlsConnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlsConnectInd *) CsrPmemZalloc(sizeof(CsrBtSmlsConnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->obexPeerMaxPacketSize, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->targetService, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->length, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->count, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SMLS_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_CFM
CsrSize CsrBtSmlsAuthenticateCfmSizeof(void *msg)
{
    CsrBtSmlsAuthenticateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_CFM */

#ifndef EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_CFM
CsrUint8 *CsrBtSmlsAuthenticateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlsAuthenticateCfm *primitive;

    primitive = (CsrBtSmlsAuthenticateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_CFM */

#ifndef EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_CFM
void *CsrBtSmlsAuthenticateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlsAuthenticateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlsAuthenticateCfm *) CsrPmemZalloc(sizeof(CsrBtSmlsAuthenticateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_CFM */

#ifndef EXCLUDE_CSR_BT_SMLS_ACTIVATE_REQ
CsrSize CsrBtSmlsActivateReqSizeof(void *msg)
{
    CsrBtSmlsActivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLS_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SMLS_ACTIVATE_REQ
CsrUint8 *CsrBtSmlsActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlsActivateReq *primitive;

    primitive = (CsrBtSmlsActivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->obexMaxPacketSize);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->windowSize);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmEnable);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SMLS_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SMLS_ACTIVATE_REQ
void *CsrBtSmlsActivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlsActivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlsActivateReq *) CsrPmemZalloc(sizeof(CsrBtSmlsActivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->obexMaxPacketSize, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->windowSize, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmEnable, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SMLS_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SMLS_DEACTIVATE_CFM
CsrSize CsrBtSmlsDeactivateCfmSizeof(void *msg)
{
    CsrBtSmlsDeactivateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLS_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_SMLS_DEACTIVATE_CFM
CsrUint8 *CsrBtSmlsDeactivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlsDeactivateCfm *primitive;

    primitive = (CsrBtSmlsDeactivateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SMLS_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_SMLS_DEACTIVATE_CFM
void *CsrBtSmlsDeactivateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlsDeactivateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlsDeactivateCfm *) CsrPmemZalloc(sizeof(CsrBtSmlsDeactivateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SMLS_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_RES
CsrSize CsrBtSmlsPutSmlMsgObjResSizeof(void *msg)
{
    CsrBtSmlsPutSmlMsgObjRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_RES */

#ifndef EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_RES
CsrUint8 *CsrBtSmlsPutSmlMsgObjResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlsPutSmlMsgObjRes *primitive;

    primitive = (CsrBtSmlsPutSmlMsgObjRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_RES */

#ifndef EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_RES
void *CsrBtSmlsPutSmlMsgObjResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlsPutSmlMsgObjRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlsPutSmlMsgObjRes *) CsrPmemZalloc(sizeof(CsrBtSmlsPutSmlMsgObjRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_RES */

#ifndef EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_IND
CsrSize CsrBtSmlsGetSmlMsgObjIndSizeof(void *msg)
{
    CsrBtSmlsGetSmlMsgObjInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_IND */

#ifndef EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_IND
CsrUint8 *CsrBtSmlsGetSmlMsgObjIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlsGetSmlMsgObjInd *primitive;

    primitive = (CsrBtSmlsGetSmlMsgObjInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mimeType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_IND */

#ifndef EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_IND
void *CsrBtSmlsGetSmlMsgObjIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlsGetSmlMsgObjInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlsGetSmlMsgObjInd *) CsrPmemZalloc(sizeof(CsrBtSmlsGetSmlMsgObjInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mimeType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_IND */

#ifndef EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_RES
CsrSize CsrBtSmlsGetSmlMsgObjResSizeof(void *msg)
{
    CsrBtSmlsGetSmlMsgObjRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->bodyLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_RES */

#ifndef EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_RES
CsrUint8 *CsrBtSmlsGetSmlMsgObjResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlsGetSmlMsgObjRes *primitive;

    primitive = (CsrBtSmlsGetSmlMsgObjRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->lengthOfObject);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyLength);
    CsrMemCpySer(buffer, length, &primitive->body, sizeof(primitive->body));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    if(primitive->body)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->body), ((CsrSize) (primitive->bodyLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_RES */

#ifndef EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_RES
void *CsrBtSmlsGetSmlMsgObjResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlsGetSmlMsgObjRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlsGetSmlMsgObjRes *) CsrPmemZalloc(sizeof(CsrBtSmlsGetSmlMsgObjRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->lengthOfObject, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->body), buffer, &offset, ((CsrSize) sizeof(primitive->body)));
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_RES */

#ifndef EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_RES
void CsrBtSmlsGetSmlMsgObjResSerFree(void *msg)
{
    CsrBtSmlsGetSmlMsgObjRes *primitive;

    primitive = (CsrBtSmlsGetSmlMsgObjRes *) msg;

    if(primitive->body != NULL)
    {
        CsrPmemFree(primitive->body);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_RES */

#ifndef EXCLUDE_CSR_BT_SMLS_ABORT_IND
CsrSize CsrBtSmlsAbortIndSizeof(void *msg)
{
    CsrBtSmlsAbortInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLS_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_SMLS_ABORT_IND
CsrUint8 *CsrBtSmlsAbortIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlsAbortInd *primitive;

    primitive = (CsrBtSmlsAbortInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->descriptionOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->descriptionLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SMLS_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_SMLS_ABORT_IND
void *CsrBtSmlsAbortIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlsAbortInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlsAbortInd *) CsrPmemZalloc(sizeof(CsrBtSmlsAbortInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->descriptionOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->descriptionLength, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_SMLS_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_SMLS_ABORT_IND
void CsrBtSmlsAbortIndSerFree(void *msg)
{
    CsrBtSmlsAbortInd *primitive;

    primitive = (CsrBtSmlsAbortInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SMLS_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_NEXT_IND
CsrSize CsrBtSmlsGetSmlMsgObjNextIndSizeof(void *msg)
{
    CsrBtSmlsGetSmlMsgObjNextInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_NEXT_IND */

#ifndef EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_NEXT_IND
CsrUint8 *CsrBtSmlsGetSmlMsgObjNextIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlsGetSmlMsgObjNextInd *primitive;

    primitive = (CsrBtSmlsGetSmlMsgObjNextInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mimeType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_NEXT_IND */

#ifndef EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_NEXT_IND
void *CsrBtSmlsGetSmlMsgObjNextIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlsGetSmlMsgObjNextInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlsGetSmlMsgObjNextInd *) CsrPmemZalloc(sizeof(CsrBtSmlsGetSmlMsgObjNextInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mimeType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_NEXT_IND */

#ifndef EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_REQ
CsrSize CsrBtSmlsAuthenticateReqSizeof(void *msg)
{
    CsrBtSmlsAuthenticateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->realmLength * sizeof(CsrUint8)) + (primitive->passwordLength * sizeof(CsrUint8)) + CsrCharStringSerLen((CsrCharString *) primitive->userId));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_REQ */

#ifndef EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_REQ
CsrUint8 *CsrBtSmlsAuthenticateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlsAuthenticateReq *primitive;

    primitive = (CsrBtSmlsAuthenticateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->realmLength);
    CsrMemCpySer(buffer, length, &primitive->realm, sizeof(primitive->realm));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->passwordLength);
    CsrMemCpySer(buffer, length, &primitive->password, sizeof(primitive->password));
    CsrMemCpySer(buffer, length, &primitive->userId, sizeof(primitive->userId));
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
#endif /* EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_REQ */

#ifndef EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_REQ
void *CsrBtSmlsAuthenticateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlsAuthenticateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlsAuthenticateReq *) CsrPmemZalloc(sizeof(CsrBtSmlsAuthenticateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->realmLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->realm), buffer, &offset, ((CsrSize) sizeof(primitive->realm)));
    CsrUint16Des((CsrUint16 *) &primitive->passwordLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->password), buffer, &offset, ((CsrSize) sizeof(primitive->password)));
    CsrMemCpyDes(((void *) &primitive->userId), buffer, &offset, ((CsrSize) sizeof(primitive->userId)));
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
#endif /* EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_REQ */

#ifndef EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_REQ
void CsrBtSmlsAuthenticateReqSerFree(void *msg)
{
    CsrBtSmlsAuthenticateReq *primitive;

    primitive = (CsrBtSmlsAuthenticateReq *) msg;

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
#endif /* EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_REQ */

#ifndef EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_NEXT_RES
CsrSize CsrBtSmlsPutSmlMsgObjNextResSizeof(void *msg)
{
    CsrBtSmlsPutSmlMsgObjNextRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_NEXT_RES
CsrUint8 *CsrBtSmlsPutSmlMsgObjNextResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlsPutSmlMsgObjNextRes *primitive;

    primitive = (CsrBtSmlsPutSmlMsgObjNextRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_NEXT_RES
void *CsrBtSmlsPutSmlMsgObjNextResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlsPutSmlMsgObjNextRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlsPutSmlMsgObjNextRes *) CsrPmemZalloc(sizeof(CsrBtSmlsPutSmlMsgObjNextRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_IND
CsrSize CsrBtSmlsPutSmlMsgObjIndSizeof(void *msg)
{
    CsrBtSmlsPutSmlMsgObjInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_IND */

#ifndef EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_IND
CsrUint8 *CsrBtSmlsPutSmlMsgObjIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlsPutSmlMsgObjInd *primitive;

    primitive = (CsrBtSmlsPutSmlMsgObjInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->lengthOfObject);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mimeType);
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
#endif /* EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_IND */

#ifndef EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_IND
void *CsrBtSmlsPutSmlMsgObjIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlsPutSmlMsgObjInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlsPutSmlMsgObjInd *) CsrPmemZalloc(sizeof(CsrBtSmlsPutSmlMsgObjInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->lengthOfObject, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mimeType, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_IND */

#ifndef EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_IND
void CsrBtSmlsPutSmlMsgObjIndSerFree(void *msg)
{
    CsrBtSmlsPutSmlMsgObjInd *primitive;

    primitive = (CsrBtSmlsPutSmlMsgObjInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_IND */

#ifndef EXCLUDE_CSR_BT_SMLS_SECURITY_IN_CFM
CsrSize CsrBtSmlsSecurityInCfmSizeof(void *msg)
{
    CsrBtSmlsSecurityInCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLS_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_SMLS_SECURITY_IN_CFM
CsrUint8 *CsrBtSmlsSecurityInCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlsSecurityInCfm *primitive;

    primitive = (CsrBtSmlsSecurityInCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SMLS_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_SMLS_SECURITY_IN_CFM
void *CsrBtSmlsSecurityInCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlsSecurityInCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlsSecurityInCfm *) CsrPmemZalloc(sizeof(CsrBtSmlsSecurityInCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SMLS_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_SMLS_ACTIVATE_CFM
CsrSize CsrBtSmlsActivateCfmSizeof(void *msg)
{
    CsrBtSmlsActivateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLS_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_SMLS_ACTIVATE_CFM
CsrUint8 *CsrBtSmlsActivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlsActivateCfm *primitive;

    primitive = (CsrBtSmlsActivateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SMLS_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_SMLS_ACTIVATE_CFM
void *CsrBtSmlsActivateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlsActivateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlsActivateCfm *) CsrPmemZalloc(sizeof(CsrBtSmlsActivateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SMLS_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_IND
CsrSize CsrBtSmlsAuthenticateIndSizeof(void *msg)
{
    CsrBtSmlsAuthenticateInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->realmLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_IND
CsrUint8 *CsrBtSmlsAuthenticateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlsAuthenticateInd *primitive;

    primitive = (CsrBtSmlsAuthenticateInd *) msg;
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
#endif /* EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_IND
void *CsrBtSmlsAuthenticateIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlsAuthenticateInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlsAuthenticateInd *) CsrPmemZalloc(sizeof(CsrBtSmlsAuthenticateInd));

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
#endif /* EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_IND
void CsrBtSmlsAuthenticateIndSerFree(void *msg)
{
    CsrBtSmlsAuthenticateInd *primitive;

    primitive = (CsrBtSmlsAuthenticateInd *) msg;

    if(primitive->realm != NULL)
    {
        CsrPmemFree(primitive->realm);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_NEXT_RES
CsrSize CsrBtSmlsGetSmlMsgObjNextResSizeof(void *msg)
{
    CsrBtSmlsGetSmlMsgObjNextRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->bodyLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_NEXT_RES
CsrUint8 *CsrBtSmlsGetSmlMsgObjNextResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlsGetSmlMsgObjNextRes *primitive;

    primitive = (CsrBtSmlsGetSmlMsgObjNextRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyLength);
    CsrMemCpySer(buffer, length, &primitive->body, sizeof(primitive->body));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    if(primitive->body)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->body), ((CsrSize) (primitive->bodyLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_NEXT_RES
void *CsrBtSmlsGetSmlMsgObjNextResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlsGetSmlMsgObjNextRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlsGetSmlMsgObjNextRes *) CsrPmemZalloc(sizeof(CsrBtSmlsGetSmlMsgObjNextRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->body), buffer, &offset, ((CsrSize) sizeof(primitive->body)));
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_NEXT_RES
void CsrBtSmlsGetSmlMsgObjNextResSerFree(void *msg)
{
    CsrBtSmlsGetSmlMsgObjNextRes *primitive;

    primitive = (CsrBtSmlsGetSmlMsgObjNextRes *) msg;

    if(primitive->body != NULL)
    {
        CsrPmemFree(primitive->body);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_SMLS_DISCONNECT_IND
CsrSize CsrBtSmlsDisconnectIndSizeof(void *msg)
{
    CsrBtSmlsDisconnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLS_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SMLS_DISCONNECT_IND
CsrUint8 *CsrBtSmlsDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlsDisconnectInd *primitive;

    primitive = (CsrBtSmlsDisconnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SMLS_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SMLS_DISCONNECT_IND
void *CsrBtSmlsDisconnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlsDisconnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlsDisconnectInd *) CsrPmemZalloc(sizeof(CsrBtSmlsDisconnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SMLS_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SMLS_CONNECT_RES
CsrSize CsrBtSmlsConnectResSizeof(void *msg)
{
    CsrBtSmlsConnectRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLS_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_SMLS_CONNECT_RES
CsrUint8 *CsrBtSmlsConnectResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlsConnectRes *primitive;

    primitive = (CsrBtSmlsConnectRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->targetService);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SMLS_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_SMLS_CONNECT_RES
void *CsrBtSmlsConnectResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlsConnectRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlsConnectRes *) CsrPmemZalloc(sizeof(CsrBtSmlsConnectRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->targetService, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SMLS_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_SMLS_SECURITY_IN_REQ
CsrSize CsrBtSmlsSecurityInReqSizeof(void *msg)
{
    CsrBtSmlsSecurityInReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLS_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_SMLS_SECURITY_IN_REQ
CsrUint8 *CsrBtSmlsSecurityInReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlsSecurityInReq *primitive;

    primitive = (CsrBtSmlsSecurityInReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SMLS_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_SMLS_SECURITY_IN_REQ
void *CsrBtSmlsSecurityInReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlsSecurityInReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlsSecurityInReq *) CsrPmemZalloc(sizeof(CsrBtSmlsSecurityInReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SMLS_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_NEXT_IND
CsrSize CsrBtSmlsPutSmlMsgObjNextIndSizeof(void *msg)
{
    CsrBtSmlsPutSmlMsgObjNextInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_NEXT_IND */

#ifndef EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_NEXT_IND
CsrUint8 *CsrBtSmlsPutSmlMsgObjNextIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlsPutSmlMsgObjNextInd *primitive;

    primitive = (CsrBtSmlsPutSmlMsgObjNextInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mimeType);
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
#endif /* EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_NEXT_IND */

#ifndef EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_NEXT_IND
void *CsrBtSmlsPutSmlMsgObjNextIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlsPutSmlMsgObjNextInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlsPutSmlMsgObjNextInd *) CsrPmemZalloc(sizeof(CsrBtSmlsPutSmlMsgObjNextInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mimeType, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_NEXT_IND */

#ifndef EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_NEXT_IND
void CsrBtSmlsPutSmlMsgObjNextIndSerFree(void *msg)
{
    CsrBtSmlsPutSmlMsgObjNextInd *primitive;

    primitive = (CsrBtSmlsPutSmlMsgObjNextInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_NEXT_IND */

#ifndef EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_RES
CsrSize CsrBtSmlsAuthenticateResSizeof(void *msg)
{
    CsrBtSmlsAuthenticateRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->passwordLength * sizeof(CsrUint8)) + CsrCharStringSerLen((CsrCharString *) primitive->userId));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_RES
CsrUint8 *CsrBtSmlsAuthenticateResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlsAuthenticateRes *primitive;

    primitive = (CsrBtSmlsAuthenticateRes *) msg;
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
#endif /* EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_RES
void *CsrBtSmlsAuthenticateResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlsAuthenticateRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlsAuthenticateRes *) CsrPmemZalloc(sizeof(CsrBtSmlsAuthenticateRes));

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
#endif /* EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_RES
void CsrBtSmlsAuthenticateResSerFree(void *msg)
{
    CsrBtSmlsAuthenticateRes *primitive;

    primitive = (CsrBtSmlsAuthenticateRes *) msg;

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
#endif /* EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_SMLS_DEACTIVATE_REQ
CsrSize CsrBtSmlsDeactivateReqSizeof(void *msg)
{
    CsrBtSmlsDeactivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SMLS_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SMLS_DEACTIVATE_REQ
CsrUint8 *CsrBtSmlsDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSmlsDeactivateReq *primitive;

    primitive = (CsrBtSmlsDeactivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SMLS_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SMLS_DEACTIVATE_REQ
void *CsrBtSmlsDeactivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSmlsDeactivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSmlsDeactivateReq *) CsrPmemZalloc(sizeof(CsrBtSmlsDeactivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SMLS_DEACTIVATE_REQ */

#endif /* EXCLUDE_CSR_BT_SMLS_MODULE */
