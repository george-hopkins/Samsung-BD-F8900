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
#include "csr_bt_opc_prim.h"

#ifndef EXCLUDE_CSR_BT_OPC_MODULE

void CsrBtOpcPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_OPC_DISCONNECT_IND
CsrSize CsrBtOpcDisconnectIndSizeof(void *msg)
{
    CsrBtOpcDisconnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_OPC_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_OPC_DISCONNECT_IND
CsrUint8 *CsrBtOpcDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtOpcDisconnectInd *primitive;

    primitive = (CsrBtOpcDisconnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_OPC_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_OPC_DISCONNECT_IND
void *CsrBtOpcDisconnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtOpcDisconnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtOpcDisconnectInd *) CsrPmemZalloc(sizeof(CsrBtOpcDisconnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_OPC_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_OPC_GET_OBJECT_REQ
CsrSize CsrBtOpcGetObjectReqSizeof(void *msg)
{
    CsrBtOpcGetObjectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_OPC_GET_OBJECT_REQ */

#ifndef EXCLUDE_CSR_BT_OPC_GET_OBJECT_REQ
CsrUint8 *CsrBtOpcGetObjectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtOpcGetObjectReq *primitive;

    primitive = (CsrBtOpcGetObjectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_OPC_GET_OBJECT_REQ */

#ifndef EXCLUDE_CSR_BT_OPC_GET_OBJECT_REQ
void *CsrBtOpcGetObjectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtOpcGetObjectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtOpcGetObjectReq *) CsrPmemZalloc(sizeof(CsrBtOpcGetObjectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_OPC_GET_OBJECT_REQ */

#ifndef EXCLUDE_CSR_BT_OPC_CANCEL_CONNECT_REQ
CsrSize CsrBtOpcCancelConnectReqSizeof(void *msg)
{
    CsrBtOpcCancelConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_OPC_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_OPC_CANCEL_CONNECT_REQ
CsrUint8 *CsrBtOpcCancelConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtOpcCancelConnectReq *primitive;

    primitive = (CsrBtOpcCancelConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_OPC_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_OPC_CANCEL_CONNECT_REQ
void *CsrBtOpcCancelConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtOpcCancelConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtOpcCancelConnectReq *) CsrPmemZalloc(sizeof(CsrBtOpcCancelConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_OPC_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_OPC_GET_HEADER_CFM
CsrSize CsrBtOpcGetHeaderCfmSizeof(void *msg)
{
    CsrBtOpcGetHeaderCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_OPC_GET_HEADER_CFM */

#ifndef EXCLUDE_CSR_BT_OPC_GET_HEADER_CFM
CsrUint8 *CsrBtOpcGetHeaderCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtOpcGetHeaderCfm *primitive;

    primitive = (CsrBtOpcGetHeaderCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->totalObjectSize);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->objectBodyOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->objectBodyLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ucs2nameOffset);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_OPC_GET_HEADER_CFM */

#ifndef EXCLUDE_CSR_BT_OPC_GET_HEADER_CFM
void *CsrBtOpcGetHeaderCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtOpcGetHeaderCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtOpcGetHeaderCfm *) CsrPmemZalloc(sizeof(CsrBtOpcGetHeaderCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->totalObjectSize, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->objectBodyOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->objectBodyLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->ucs2nameOffset, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_OPC_GET_HEADER_CFM */

#ifndef EXCLUDE_CSR_BT_OPC_GET_HEADER_CFM
void CsrBtOpcGetHeaderCfmSerFree(void *msg)
{
    CsrBtOpcGetHeaderCfm *primitive;

    primitive = (CsrBtOpcGetHeaderCfm *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_OPC_GET_HEADER_CFM */

#ifndef EXCLUDE_CSR_BT_OPC_DISCONNECT_REQ
CsrSize CsrBtOpcDisconnectReqSizeof(void *msg)
{
    CsrBtOpcDisconnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_OPC_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_OPC_DISCONNECT_REQ
CsrUint8 *CsrBtOpcDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtOpcDisconnectReq *primitive;

    primitive = (CsrBtOpcDisconnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->normalDisconnect);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_OPC_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_OPC_DISCONNECT_REQ
void *CsrBtOpcDisconnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtOpcDisconnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtOpcDisconnectReq *) CsrPmemZalloc(sizeof(CsrBtOpcDisconnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->normalDisconnect, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_OPC_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_OPC_ABORT_REQ
CsrSize CsrBtOpcAbortReqSizeof(void *msg)
{
    CsrBtOpcAbortReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_OPC_ABORT_REQ */

#ifndef EXCLUDE_CSR_BT_OPC_ABORT_REQ
CsrUint8 *CsrBtOpcAbortReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtOpcAbortReq *primitive;

    primitive = (CsrBtOpcAbortReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_OPC_ABORT_REQ */

#ifndef EXCLUDE_CSR_BT_OPC_ABORT_REQ
void *CsrBtOpcAbortReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtOpcAbortReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtOpcAbortReq *) CsrPmemZalloc(sizeof(CsrBtOpcAbortReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_OPC_ABORT_REQ */

#ifndef EXCLUDE_CSR_BT_OPC_ABORT_CFM
CsrSize CsrBtOpcAbortCfmSizeof(void *msg)
{
    CsrBtOpcAbortCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_OPC_ABORT_CFM */

#ifndef EXCLUDE_CSR_BT_OPC_ABORT_CFM
CsrUint8 *CsrBtOpcAbortCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtOpcAbortCfm *primitive;

    primitive = (CsrBtOpcAbortCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_OPC_ABORT_CFM */

#ifndef EXCLUDE_CSR_BT_OPC_ABORT_CFM
void *CsrBtOpcAbortCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtOpcAbortCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtOpcAbortCfm *) CsrPmemZalloc(sizeof(CsrBtOpcAbortCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_OPC_ABORT_CFM */

#ifndef EXCLUDE_CSR_BT_OPC_AUTHENTICATE_IND
CsrSize CsrBtOpcAuthenticateIndSizeof(void *msg)
{
    CsrBtOpcAuthenticateInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->realmLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_OPC_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_OPC_AUTHENTICATE_IND
CsrUint8 *CsrBtOpcAuthenticateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtOpcAuthenticateInd *primitive;

    primitive = (CsrBtOpcAuthenticateInd *) msg;
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
#endif /* EXCLUDE_CSR_BT_OPC_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_OPC_AUTHENTICATE_IND
void *CsrBtOpcAuthenticateIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtOpcAuthenticateInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtOpcAuthenticateInd *) CsrPmemZalloc(sizeof(CsrBtOpcAuthenticateInd));

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
#endif /* EXCLUDE_CSR_BT_OPC_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_OPC_AUTHENTICATE_IND
void CsrBtOpcAuthenticateIndSerFree(void *msg)
{
    CsrBtOpcAuthenticateInd *primitive;

    primitive = (CsrBtOpcAuthenticateInd *) msg;

    if(primitive->realm != NULL)
    {
        CsrPmemFree(primitive->realm);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_OPC_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_OPC_SECURITY_OUT_CFM
CsrSize CsrBtOpcSecurityOutCfmSizeof(void *msg)
{
    CsrBtOpcSecurityOutCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_OPC_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_OPC_SECURITY_OUT_CFM
CsrUint8 *CsrBtOpcSecurityOutCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtOpcSecurityOutCfm *primitive;

    primitive = (CsrBtOpcSecurityOutCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_OPC_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_OPC_SECURITY_OUT_CFM
void *CsrBtOpcSecurityOutCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtOpcSecurityOutCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtOpcSecurityOutCfm *) CsrPmemZalloc(sizeof(CsrBtOpcSecurityOutCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_OPC_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_OPC_PUT_CFM
CsrSize CsrBtOpcPutCfmSizeof(void *msg)
{
    CsrBtOpcPutCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_OPC_PUT_CFM */

#ifndef EXCLUDE_CSR_BT_OPC_PUT_CFM
CsrUint8 *CsrBtOpcPutCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtOpcPutCfm *primitive;

    primitive = (CsrBtOpcPutCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_OPC_PUT_CFM */

#ifndef EXCLUDE_CSR_BT_OPC_PUT_CFM
void *CsrBtOpcPutCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtOpcPutCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtOpcPutCfm *) CsrPmemZalloc(sizeof(CsrBtOpcPutCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_OPC_PUT_CFM */

#ifndef EXCLUDE_CSR_BT_OPC_PUT_OBJECT_RES
CsrSize CsrBtOpcPutObjectResSizeof(void *msg)
{
    CsrBtOpcPutObjectRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->objectLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_OPC_PUT_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_OPC_PUT_OBJECT_RES
CsrUint8 *CsrBtOpcPutObjectResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtOpcPutObjectRes *primitive;

    primitive = (CsrBtOpcPutObjectRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->objectLength);
    CsrMemCpySer(buffer, length, &primitive->object, sizeof(primitive->object));
    if(primitive->object)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->object), ((CsrSize) (primitive->objectLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_OPC_PUT_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_OPC_PUT_OBJECT_RES
void *CsrBtOpcPutObjectResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtOpcPutObjectRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtOpcPutObjectRes *) CsrPmemZalloc(sizeof(CsrBtOpcPutObjectRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->objectLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->object), buffer, &offset, ((CsrSize) sizeof(primitive->object)));
    if (primitive->objectLength)
    {
        primitive->object = CsrPmemZalloc(((CsrUint32) (primitive->objectLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->object), buffer, &offset, ((CsrSize) (primitive->objectLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->object = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_OPC_PUT_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_OPC_PUT_OBJECT_RES
void CsrBtOpcPutObjectResSerFree(void *msg)
{
    CsrBtOpcPutObjectRes *primitive;

    primitive = (CsrBtOpcPutObjectRes *) msg;

    if(primitive->object != NULL)
    {
        CsrPmemFree(primitive->object);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_OPC_PUT_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_OPC_AUTHENTICATE_RES
CsrSize CsrBtOpcAuthenticateResSizeof(void *msg)
{
    CsrBtOpcAuthenticateRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->authPasswordLength * sizeof(CsrUint8)) + CsrCharStringSerLen((CsrCharString *) primitive->authUserId) + (primitive->chalRealmLength * sizeof(CsrUint8)) + (primitive->chalPasswordLength * sizeof(CsrUint8)) + CsrCharStringSerLen((CsrCharString *) primitive->chalUserId));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_OPC_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_OPC_AUTHENTICATE_RES
CsrUint8 *CsrBtOpcAuthenticateResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtOpcAuthenticateRes *primitive;

    primitive = (CsrBtOpcAuthenticateRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->authPassword, sizeof(primitive->authPassword));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->authPasswordLength);
    CsrMemCpySer(buffer, length, &primitive->authUserId, sizeof(primitive->authUserId));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->chalRealmLength);
    CsrMemCpySer(buffer, length, &primitive->chalRealm, sizeof(primitive->chalRealm));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->chalPasswordLength);
    CsrMemCpySer(buffer, length, &primitive->chalPassword, sizeof(primitive->chalPassword));
    CsrMemCpySer(buffer, length, &primitive->chalUserId, sizeof(primitive->chalUserId));
    if(primitive->authPassword)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->authPassword), ((CsrSize) (primitive->authPasswordLength * sizeof(CsrUint8))));
    }
    CsrCharStringSer(buffer, length, primitive->authUserId);
    if(primitive->chalRealm)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->chalRealm), ((CsrSize) (primitive->chalRealmLength * sizeof(CsrUint8))));
    }
    if(primitive->chalPassword)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->chalPassword), ((CsrSize) (primitive->chalPasswordLength * sizeof(CsrUint8))));
    }
    CsrCharStringSer(buffer, length, primitive->chalUserId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_OPC_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_OPC_AUTHENTICATE_RES
void *CsrBtOpcAuthenticateResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtOpcAuthenticateRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtOpcAuthenticateRes *) CsrPmemZalloc(sizeof(CsrBtOpcAuthenticateRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->authPassword), buffer, &offset, ((CsrSize) sizeof(primitive->authPassword)));
    CsrUint16Des((CsrUint16 *) &primitive->authPasswordLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->authUserId), buffer, &offset, ((CsrSize) sizeof(primitive->authUserId)));
    CsrUint16Des((CsrUint16 *) &primitive->chalRealmLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->chalRealm), buffer, &offset, ((CsrSize) sizeof(primitive->chalRealm)));
    CsrUint16Des((CsrUint16 *) &primitive->chalPasswordLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->chalPassword), buffer, &offset, ((CsrSize) sizeof(primitive->chalPassword)));
    CsrMemCpyDes(((void *) &primitive->chalUserId), buffer, &offset, ((CsrSize) sizeof(primitive->chalUserId)));
    if (primitive->authPasswordLength)
    {
        primitive->authPassword = CsrPmemZalloc(((CsrUint32) (primitive->authPasswordLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->authPassword), buffer, &offset, ((CsrSize) (primitive->authPasswordLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->authPassword = NULL;
    }
    CsrCharStringDes(&primitive->authUserId, buffer, &offset);
    if (primitive->chalRealmLength)
    {
        primitive->chalRealm = CsrPmemZalloc(((CsrUint32) (primitive->chalRealmLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->chalRealm), buffer, &offset, ((CsrSize) (primitive->chalRealmLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->chalRealm = NULL;
    }
    if (primitive->chalPasswordLength)
    {
        primitive->chalPassword = CsrPmemZalloc(((CsrUint32) (primitive->chalPasswordLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->chalPassword), buffer, &offset, ((CsrSize) (primitive->chalPasswordLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->chalPassword = NULL;
    }
    CsrCharStringDes(&primitive->chalUserId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_OPC_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_OPC_AUTHENTICATE_RES
void CsrBtOpcAuthenticateResSerFree(void *msg)
{
    CsrBtOpcAuthenticateRes *primitive;

    primitive = (CsrBtOpcAuthenticateRes *) msg;

    if(primitive->authPassword != NULL)
    {
        CsrPmemFree(primitive->authPassword);
    }
    if(primitive->authUserId != NULL)
    {
        CsrPmemFree(primitive->authUserId);
    }
    if(primitive->chalRealm != NULL)
    {
        CsrPmemFree(primitive->chalRealm);
    }
    if(primitive->chalPassword != NULL)
    {
        CsrPmemFree(primitive->chalPassword);
    }
    if(primitive->chalUserId != NULL)
    {
        CsrPmemFree(primitive->chalUserId);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_OPC_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_OPC_PUT_OBJECT_IND
CsrSize CsrBtOpcPutObjectIndSizeof(void *msg)
{
    CsrBtOpcPutObjectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_OPC_PUT_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_OPC_PUT_OBJECT_IND
CsrUint8 *CsrBtOpcPutObjectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtOpcPutObjectInd *primitive;

    primitive = (CsrBtOpcPutObjectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->objectLength);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_OPC_PUT_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_OPC_PUT_OBJECT_IND
void *CsrBtOpcPutObjectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtOpcPutObjectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtOpcPutObjectInd *) CsrPmemZalloc(sizeof(CsrBtOpcPutObjectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->objectLength, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_OPC_PUT_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_OPC_GET_OBJECT_CFM
CsrSize CsrBtOpcGetObjectCfmSizeof(void *msg)
{
    CsrBtOpcGetObjectCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_OPC_GET_OBJECT_CFM */

#ifndef EXCLUDE_CSR_BT_OPC_GET_OBJECT_CFM
CsrUint8 *CsrBtOpcGetObjectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtOpcGetObjectCfm *primitive;

    primitive = (CsrBtOpcGetObjectCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->objectBodyOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->objectBodyLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_OPC_GET_OBJECT_CFM */

#ifndef EXCLUDE_CSR_BT_OPC_GET_OBJECT_CFM
void *CsrBtOpcGetObjectCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtOpcGetObjectCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtOpcGetObjectCfm *) CsrPmemZalloc(sizeof(CsrBtOpcGetObjectCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->objectBodyOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->objectBodyLength, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_OPC_GET_OBJECT_CFM */

#ifndef EXCLUDE_CSR_BT_OPC_GET_OBJECT_CFM
void CsrBtOpcGetObjectCfmSerFree(void *msg)
{
    CsrBtOpcGetObjectCfm *primitive;

    primitive = (CsrBtOpcGetObjectCfm *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_OPC_GET_OBJECT_CFM */

#ifndef EXCLUDE_CSR_BT_OPC_CONNECT_REQ
CsrSize CsrBtOpcConnectReqSizeof(void *msg)
{
    CsrBtOpcConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->realmLength * sizeof(CsrUint8)) + (primitive->passwordLength * sizeof(CsrUint8)) + CsrCharStringSerLen((CsrCharString *) primitive->userId));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_OPC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_OPC_CONNECT_REQ
CsrUint8 *CsrBtOpcConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtOpcConnectReq *primitive;

    primitive = (CsrBtOpcConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxPacketSize);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->count);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->realmLength);
    CsrMemCpySer(buffer, length, &primitive->realm, sizeof(primitive->realm));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->passwordLength);
    CsrMemCpySer(buffer, length, &primitive->password, sizeof(primitive->password));
    CsrMemCpySer(buffer, length, &primitive->userId, sizeof(primitive->userId));
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
#endif /* EXCLUDE_CSR_BT_OPC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_OPC_CONNECT_REQ
void *CsrBtOpcConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtOpcConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtOpcConnectReq *) CsrPmemZalloc(sizeof(CsrBtOpcConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxPacketSize, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->count, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->realmLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->realm), buffer, &offset, ((CsrSize) sizeof(primitive->realm)));
    CsrUint16Des((CsrUint16 *) &primitive->passwordLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->password), buffer, &offset, ((CsrSize) sizeof(primitive->password)));
    CsrMemCpyDes(((void *) &primitive->userId), buffer, &offset, ((CsrSize) sizeof(primitive->userId)));
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
#endif /* EXCLUDE_CSR_BT_OPC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_OPC_CONNECT_REQ
void CsrBtOpcConnectReqSerFree(void *msg)
{
    CsrBtOpcConnectReq *primitive;

    primitive = (CsrBtOpcConnectReq *) msg;

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
#endif /* EXCLUDE_CSR_BT_OPC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_OPC_CONNECT_CFM
CsrSize CsrBtOpcConnectCfmSizeof(void *msg)
{
    CsrBtOpcConnectCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_OPC_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_OPC_CONNECT_CFM
CsrUint8 *CsrBtOpcConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtOpcConnectCfm *primitive;

    primitive = (CsrBtOpcConnectCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxPeerPacketSize);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->supportedFormats);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_OPC_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_OPC_CONNECT_CFM
void *CsrBtOpcConnectCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtOpcConnectCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtOpcConnectCfm *) CsrPmemZalloc(sizeof(CsrBtOpcConnectCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxPeerPacketSize, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->supportedFormats, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_OPC_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_OPC_SECURITY_OUT_REQ
CsrSize CsrBtOpcSecurityOutReqSizeof(void *msg)
{
    CsrBtOpcSecurityOutReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_OPC_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_OPC_SECURITY_OUT_REQ
CsrUint8 *CsrBtOpcSecurityOutReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtOpcSecurityOutReq *primitive;

    primitive = (CsrBtOpcSecurityOutReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_OPC_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_OPC_SECURITY_OUT_REQ
void *CsrBtOpcSecurityOutReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtOpcSecurityOutReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtOpcSecurityOutReq *) CsrPmemZalloc(sizeof(CsrBtOpcSecurityOutReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_OPC_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_OPC_PUT_REQ
CsrSize CsrBtOpcPutReqSizeof(void *msg)
{
    CsrBtOpcPutReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->bodyTypeLength * sizeof(CsrUint8)) + CsrConverterUcs2ByteStrLen((CsrUtf8String*) primitive->ucs2name) + 2);
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_OPC_PUT_REQ */

#ifndef EXCLUDE_CSR_BT_OPC_PUT_REQ
CsrUint8 *CsrBtOpcPutReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtOpcPutReq *primitive;

    primitive = (CsrBtOpcPutReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->bodyType, sizeof(primitive->bodyType));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyTypeLength);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->lengthOfObject);
    CsrMemCpySer(buffer, length, &primitive->ucs2name, sizeof(primitive->ucs2name));
    if(primitive->bodyType)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->bodyType), ((CsrSize) (primitive->bodyTypeLength * sizeof(CsrUint8))));
    }
    if (primitive->ucs2name)
    {
        CsrMemCpy(((CsrUint8*) &buffer[*length]), ((CsrUint8*) primitive->ucs2name), CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->ucs2name) + 2);
        *length += ((CsrUint16) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->ucs2name) + 2));
    }
    else
    {
        buffer[*length] = '\0';
        buffer[*length + 1] = '\0';
        *length += 2;
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_OPC_PUT_REQ */

#ifndef EXCLUDE_CSR_BT_OPC_PUT_REQ
void *CsrBtOpcPutReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtOpcPutReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtOpcPutReq *) CsrPmemZalloc(sizeof(CsrBtOpcPutReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->bodyType), buffer, &offset, ((CsrSize) sizeof(primitive->bodyType)));
    CsrUint16Des((CsrUint16 *) &primitive->bodyTypeLength, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->lengthOfObject, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->ucs2name), buffer, &offset, ((CsrSize) sizeof(primitive->ucs2name)));
    if (primitive->bodyTypeLength)
    {
        primitive->bodyType = CsrPmemZalloc(((CsrUint32) (primitive->bodyTypeLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->bodyType), buffer, &offset, ((CsrSize) (primitive->bodyTypeLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->bodyType = NULL;
    }
    primitive->ucs2name = CsrPmemZalloc(CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    CsrMemCpy(((CsrUint8*) primitive->ucs2name), ((CsrUint8*) &buffer[offset]),CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    offset += ((CsrSize) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->ucs2name) + 2));

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_OPC_PUT_REQ */

#ifndef EXCLUDE_CSR_BT_OPC_PUT_REQ
void CsrBtOpcPutReqSerFree(void *msg)
{
    CsrBtOpcPutReq *primitive;

    primitive = (CsrBtOpcPutReq *) msg;

    if(primitive->bodyType != NULL)
    {
        CsrPmemFree(primitive->bodyType);
    }
    if(primitive->ucs2name != NULL)
    {
        CsrPmemFree(primitive->ucs2name);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_OPC_PUT_REQ */

#ifndef EXCLUDE_CSR_BT_OPC_GET_HEADER_REQ
CsrSize CsrBtOpcGetHeaderReqSizeof(void *msg)
{
    CsrBtOpcGetHeaderReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_OPC_GET_HEADER_REQ */

#ifndef EXCLUDE_CSR_BT_OPC_GET_HEADER_REQ
CsrUint8 *CsrBtOpcGetHeaderReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtOpcGetHeaderReq *primitive;

    primitive = (CsrBtOpcGetHeaderReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bodyType);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_OPC_GET_HEADER_REQ */

#ifndef EXCLUDE_CSR_BT_OPC_GET_HEADER_REQ
void *CsrBtOpcGetHeaderReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtOpcGetHeaderReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtOpcGetHeaderReq *) CsrPmemZalloc(sizeof(CsrBtOpcGetHeaderReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bodyType, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_OPC_GET_HEADER_REQ */

#endif /* EXCLUDE_CSR_BT_OPC_MODULE */
