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
#include "csr_bt_ftc_prim.h"

#ifndef EXCLUDE_CSR_BT_FTC_MODULE

void CsrBtFtcPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_FTC_PUT_OBJ_HEADER_REQ
CsrSize CsrBtFtcPutObjHeaderReqSizeof(void *msg)
{
    CsrBtFtcPutObjHeaderReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrConverterUcs2ByteStrLen((CsrUtf8String*) primitive->ucs2name) + 2);
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTC_PUT_OBJ_HEADER_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_PUT_OBJ_HEADER_REQ
CsrUint8 *CsrBtFtcPutObjHeaderReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtcPutObjHeaderReq *primitive;

    primitive = (CsrBtFtcPutObjHeaderReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->ucs2name, sizeof(primitive->ucs2name));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->lengthOfObject);
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
#endif /* EXCLUDE_CSR_BT_FTC_PUT_OBJ_HEADER_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_PUT_OBJ_HEADER_REQ
void *CsrBtFtcPutObjHeaderReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtcPutObjHeaderReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtcPutObjHeaderReq *) CsrPmemZalloc(sizeof(CsrBtFtcPutObjHeaderReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->ucs2name), buffer, &offset, ((CsrSize) sizeof(primitive->ucs2name)));
    CsrUint32Des((CsrUint32 *) &primitive->lengthOfObject, buffer, &offset);
    primitive->ucs2name = CsrPmemZalloc(CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    CsrMemCpy(((CsrUint8*) primitive->ucs2name), ((CsrUint8*) &buffer[offset]),CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    offset += ((CsrSize) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->ucs2name) + 2));

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTC_PUT_OBJ_HEADER_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_PUT_OBJ_HEADER_REQ
void CsrBtFtcPutObjHeaderReqSerFree(void *msg)
{
    CsrBtFtcPutObjHeaderReq *primitive;

    primitive = (CsrBtFtcPutObjHeaderReq *) msg;

    if(primitive->ucs2name != NULL)
    {
        CsrPmemFree(primitive->ucs2name);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_FTC_PUT_OBJ_HEADER_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_SET_FOLDER_REQ
CsrSize CsrBtFtcSetFolderReqSizeof(void *msg)
{
    CsrBtFtcSetFolderReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrConverterUcs2ByteStrLen((CsrUtf8String*) primitive->ucs2name) + 2);
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTC_SET_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_SET_FOLDER_REQ
CsrUint8 *CsrBtFtcSetFolderReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtcSetFolderReq *primitive;

    primitive = (CsrBtFtcSetFolderReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->ucs2name, sizeof(primitive->ucs2name));
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
#endif /* EXCLUDE_CSR_BT_FTC_SET_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_SET_FOLDER_REQ
void *CsrBtFtcSetFolderReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtcSetFolderReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtcSetFolderReq *) CsrPmemZalloc(sizeof(CsrBtFtcSetFolderReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->ucs2name), buffer, &offset, ((CsrSize) sizeof(primitive->ucs2name)));
    primitive->ucs2name = CsrPmemZalloc(CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    CsrMemCpy(((CsrUint8*) primitive->ucs2name), ((CsrUint8*) &buffer[offset]),CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    offset += ((CsrSize) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->ucs2name) + 2));

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTC_SET_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_SET_FOLDER_REQ
void CsrBtFtcSetFolderReqSerFree(void *msg)
{
    CsrBtFtcSetFolderReq *primitive;

    primitive = (CsrBtFtcSetFolderReq *) msg;

    if(primitive->ucs2name != NULL)
    {
        CsrPmemFree(primitive->ucs2name);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_FTC_SET_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_CONNECT_REQ
CsrSize CsrBtFtcConnectReqSizeof(void *msg)
{
    CsrBtFtcConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->realmLength * sizeof(CsrUint8)) + (primitive->passwordLength * sizeof(CsrUint8)) + CsrCharStringSerLen((CsrCharString *) primitive->userId));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_CONNECT_REQ
CsrUint8 *CsrBtFtcConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtcConnectReq *primitive;

    primitive = (CsrBtFtcConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxPacketSize);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->destination.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->destination.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->destination.nap);
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
#endif /* EXCLUDE_CSR_BT_FTC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_CONNECT_REQ
void *CsrBtFtcConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtcConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtcConnectReq *) CsrPmemZalloc(sizeof(CsrBtFtcConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxPacketSize, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->destination.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->destination.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->destination.nap, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_FTC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_CONNECT_REQ
void CsrBtFtcConnectReqSerFree(void *msg)
{
    CsrBtFtcConnectReq *primitive;

    primitive = (CsrBtFtcConnectReq *) msg;

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
#endif /* EXCLUDE_CSR_BT_FTC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_SET_FOLDER_CFM
CsrSize CsrBtFtcSetFolderCfmSizeof(void *msg)
{
    CsrBtFtcSetFolderCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTC_SET_FOLDER_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_SET_FOLDER_CFM
CsrUint8 *CsrBtFtcSetFolderCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtcSetFolderCfm *primitive;

    primitive = (CsrBtFtcSetFolderCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTC_SET_FOLDER_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_SET_FOLDER_CFM
void *CsrBtFtcSetFolderCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtcSetFolderCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtcSetFolderCfm *) CsrPmemZalloc(sizeof(CsrBtFtcSetFolderCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTC_SET_FOLDER_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_DISCONNECT_REQ
CsrSize CsrBtFtcDisconnectReqSizeof(void *msg)
{
    CsrBtFtcDisconnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTC_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_DISCONNECT_REQ
CsrUint8 *CsrBtFtcDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtcDisconnectReq *primitive;

    primitive = (CsrBtFtcDisconnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->normalDisconnect);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTC_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_DISCONNECT_REQ
void *CsrBtFtcDisconnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtcDisconnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtcDisconnectReq *) CsrPmemZalloc(sizeof(CsrBtFtcDisconnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->normalDisconnect, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTC_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_ABORT_CFM
CsrSize CsrBtFtcAbortCfmSizeof(void *msg)
{
    CsrBtFtcAbortCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTC_ABORT_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_ABORT_CFM
CsrUint8 *CsrBtFtcAbortCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtcAbortCfm *primitive;

    primitive = (CsrBtFtcAbortCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTC_ABORT_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_ABORT_CFM
void *CsrBtFtcAbortCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtcAbortCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtcAbortCfm *) CsrPmemZalloc(sizeof(CsrBtFtcAbortCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTC_ABORT_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_CONNECT_CFM
CsrSize CsrBtFtcConnectCfmSizeof(void *msg)
{
    CsrBtFtcConnectCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTC_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_CONNECT_CFM
CsrUint8 *CsrBtFtcConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtcConnectCfm *primitive;

    primitive = (CsrBtFtcConnectCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->obexPeerMaxPacketSize);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTC_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_CONNECT_CFM
void *CsrBtFtcConnectCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtcConnectCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtcConnectCfm *) CsrPmemZalloc(sizeof(CsrBtFtcConnectCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->obexPeerMaxPacketSize, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTC_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_PUT_OBJ_BODY_REQ
CsrSize CsrBtFtcPutObjBodyReqSizeof(void *msg)
{
    CsrBtFtcPutObjBodyReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->bodyLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTC_PUT_OBJ_BODY_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_PUT_OBJ_BODY_REQ
CsrUint8 *CsrBtFtcPutObjBodyReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtcPutObjBodyReq *primitive;

    primitive = (CsrBtFtcPutObjBodyReq *) msg;
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
#endif /* EXCLUDE_CSR_BT_FTC_PUT_OBJ_BODY_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_PUT_OBJ_BODY_REQ
void *CsrBtFtcPutObjBodyReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtcPutObjBodyReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtcPutObjBodyReq *) CsrPmemZalloc(sizeof(CsrBtFtcPutObjBodyReq));

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
#endif /* EXCLUDE_CSR_BT_FTC_PUT_OBJ_BODY_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_PUT_OBJ_BODY_REQ
void CsrBtFtcPutObjBodyReqSerFree(void *msg)
{
    CsrBtFtcPutObjBodyReq *primitive;

    primitive = (CsrBtFtcPutObjBodyReq *) msg;

    if(primitive->body != NULL)
    {
        CsrPmemFree(primitive->body);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_FTC_PUT_OBJ_BODY_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_COPYING_OBJ_CFM
CsrSize CsrBtFtcCopyingObjCfmSizeof(void *msg)
{
    CsrBtFtcCopyingObjCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTC_COPYING_OBJ_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_COPYING_OBJ_CFM
CsrUint8 *CsrBtFtcCopyingObjCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtcCopyingObjCfm *primitive;

    primitive = (CsrBtFtcCopyingObjCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTC_COPYING_OBJ_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_COPYING_OBJ_CFM
void *CsrBtFtcCopyingObjCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtcCopyingObjCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtcCopyingObjCfm *) CsrPmemZalloc(sizeof(CsrBtFtcCopyingObjCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTC_COPYING_OBJ_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_SET_ROOT_FOLDER_CFM
CsrSize CsrBtFtcSetRootFolderCfmSizeof(void *msg)
{
    CsrBtFtcSetRootFolderCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTC_SET_ROOT_FOLDER_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_SET_ROOT_FOLDER_CFM
CsrUint8 *CsrBtFtcSetRootFolderCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtcSetRootFolderCfm *primitive;

    primitive = (CsrBtFtcSetRootFolderCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTC_SET_ROOT_FOLDER_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_SET_ROOT_FOLDER_CFM
void *CsrBtFtcSetRootFolderCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtcSetRootFolderCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtcSetRootFolderCfm *) CsrPmemZalloc(sizeof(CsrBtFtcSetRootFolderCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTC_SET_ROOT_FOLDER_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_BODY_CFM
CsrSize CsrBtFtcGetListFolderBodyCfmSizeof(void *msg)
{
    CsrBtFtcGetListFolderBodyCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_BODY_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_BODY_CFM
CsrUint8 *CsrBtFtcGetListFolderBodyCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtcGetListFolderBodyCfm *primitive;

    primitive = (CsrBtFtcGetListFolderBodyCfm *) msg;
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
#endif /* EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_BODY_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_BODY_CFM
void *CsrBtFtcGetListFolderBodyCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtcGetListFolderBodyCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtcGetListFolderBodyCfm *) CsrPmemZalloc(sizeof(CsrBtFtcGetListFolderBodyCfm));

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
#endif /* EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_BODY_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_BODY_CFM
void CsrBtFtcGetListFolderBodyCfmSerFree(void *msg)
{
    CsrBtFtcGetListFolderBodyCfm *primitive;

    primitive = (CsrBtFtcGetListFolderBodyCfm *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_BODY_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_SET_OBJ_PERMISSIONS_REQ
CsrSize CsrBtFtcSetObjPermissionsReqSizeof(void *msg)
{
    CsrBtFtcSetObjPermissionsReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrConverterUcs2ByteStrLen((CsrUtf8String*) primitive->ucs2name) + 2);
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTC_SET_OBJ_PERMISSIONS_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_SET_OBJ_PERMISSIONS_REQ
CsrUint8 *CsrBtFtcSetObjPermissionsReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtcSetObjPermissionsReq *primitive;

    primitive = (CsrBtFtcSetObjPermissionsReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->ucs2name, sizeof(primitive->ucs2name));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->permissions);
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
#endif /* EXCLUDE_CSR_BT_FTC_SET_OBJ_PERMISSIONS_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_SET_OBJ_PERMISSIONS_REQ
void *CsrBtFtcSetObjPermissionsReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtcSetObjPermissionsReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtcSetObjPermissionsReq *) CsrPmemZalloc(sizeof(CsrBtFtcSetObjPermissionsReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->ucs2name), buffer, &offset, ((CsrSize) sizeof(primitive->ucs2name)));
    CsrUint32Des((CsrUint32 *) &primitive->permissions, buffer, &offset);
    primitive->ucs2name = CsrPmemZalloc(CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    CsrMemCpy(((CsrUint8*) primitive->ucs2name), ((CsrUint8*) &buffer[offset]),CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    offset += ((CsrSize) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->ucs2name) + 2));

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTC_SET_OBJ_PERMISSIONS_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_SET_OBJ_PERMISSIONS_REQ
void CsrBtFtcSetObjPermissionsReqSerFree(void *msg)
{
    CsrBtFtcSetObjPermissionsReq *primitive;

    primitive = (CsrBtFtcSetObjPermissionsReq *) msg;

    if(primitive->ucs2name != NULL)
    {
        CsrPmemFree(primitive->ucs2name);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_FTC_SET_OBJ_PERMISSIONS_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_COPYING_OBJ_REQ
CsrSize CsrBtFtcCopyingObjReqSizeof(void *msg)
{
    CsrBtFtcCopyingObjReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrConverterUcs2ByteStrLen((CsrUtf8String*) primitive->ucs2SrcName) + 2 + CsrConverterUcs2ByteStrLen((CsrUtf8String*) primitive->ucs2DestName) + 2);
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTC_COPYING_OBJ_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_COPYING_OBJ_REQ
CsrUint8 *CsrBtFtcCopyingObjReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtcCopyingObjReq *primitive;

    primitive = (CsrBtFtcCopyingObjReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->ucs2SrcName, sizeof(primitive->ucs2SrcName));
    CsrMemCpySer(buffer, length, &primitive->ucs2DestName, sizeof(primitive->ucs2DestName));
    if (primitive->ucs2SrcName)
    {
        CsrMemCpy(((CsrUint8*) &buffer[*length]), ((CsrUint8*) primitive->ucs2SrcName), CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->ucs2SrcName) + 2);
        *length += ((CsrUint16) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->ucs2SrcName) + 2));
    }
    else
    {
        buffer[*length] = '\0';
        buffer[*length + 1] = '\0';
        *length += 2;
    }
    if (primitive->ucs2DestName)
    {
        CsrMemCpy(((CsrUint8*) &buffer[*length]), ((CsrUint8*) primitive->ucs2DestName), CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->ucs2DestName) + 2);
        *length += ((CsrUint16) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->ucs2DestName) + 2));
    }
    else
    {
        buffer[*length] = '\0';
        buffer[*length + 1] = '\0';
        *length += 2;
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTC_COPYING_OBJ_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_COPYING_OBJ_REQ
void *CsrBtFtcCopyingObjReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtcCopyingObjReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtcCopyingObjReq *) CsrPmemZalloc(sizeof(CsrBtFtcCopyingObjReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->ucs2SrcName), buffer, &offset, ((CsrSize) sizeof(primitive->ucs2SrcName)));
    CsrMemCpyDes(((void *) &primitive->ucs2DestName), buffer, &offset, ((CsrSize) sizeof(primitive->ucs2DestName)));
    primitive->ucs2SrcName = CsrPmemZalloc(CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    CsrMemCpy(((CsrUint8*) primitive->ucs2SrcName), ((CsrUint8*) &buffer[offset]),CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    offset += ((CsrSize) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->ucs2SrcName) + 2));
    primitive->ucs2DestName = CsrPmemZalloc(CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    CsrMemCpy(((CsrUint8*) primitive->ucs2DestName), ((CsrUint8*) &buffer[offset]),CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    offset += ((CsrSize) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->ucs2DestName) + 2));

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTC_COPYING_OBJ_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_COPYING_OBJ_REQ
void CsrBtFtcCopyingObjReqSerFree(void *msg)
{
    CsrBtFtcCopyingObjReq *primitive;

    primitive = (CsrBtFtcCopyingObjReq *) msg;

    if(primitive->ucs2SrcName != NULL)
    {
        CsrPmemFree(primitive->ucs2SrcName);
    }
    if(primitive->ucs2DestName != NULL)
    {
        CsrPmemFree(primitive->ucs2DestName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_FTC_COPYING_OBJ_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_MOVING_OBJ_REQ
CsrSize CsrBtFtcMovingObjReqSizeof(void *msg)
{
    CsrBtFtcMovingObjReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrConverterUcs2ByteStrLen((CsrUtf8String*) primitive->ucs2SrcName) + 2 + CsrConverterUcs2ByteStrLen((CsrUtf8String*) primitive->ucs2DestName) + 2);
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTC_MOVING_OBJ_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_MOVING_OBJ_REQ
CsrUint8 *CsrBtFtcMovingObjReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtcMovingObjReq *primitive;

    primitive = (CsrBtFtcMovingObjReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->ucs2SrcName, sizeof(primitive->ucs2SrcName));
    CsrMemCpySer(buffer, length, &primitive->ucs2DestName, sizeof(primitive->ucs2DestName));
    if (primitive->ucs2SrcName)
    {
        CsrMemCpy(((CsrUint8*) &buffer[*length]), ((CsrUint8*) primitive->ucs2SrcName), CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->ucs2SrcName) + 2);
        *length += ((CsrUint16) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->ucs2SrcName) + 2));
    }
    else
    {
        buffer[*length] = '\0';
        buffer[*length + 1] = '\0';
        *length += 2;
    }
    if (primitive->ucs2DestName)
    {
        CsrMemCpy(((CsrUint8*) &buffer[*length]), ((CsrUint8*) primitive->ucs2DestName), CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->ucs2DestName) + 2);
        *length += ((CsrUint16) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->ucs2DestName) + 2));
    }
    else
    {
        buffer[*length] = '\0';
        buffer[*length + 1] = '\0';
        *length += 2;
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTC_MOVING_OBJ_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_MOVING_OBJ_REQ
void *CsrBtFtcMovingObjReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtcMovingObjReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtcMovingObjReq *) CsrPmemZalloc(sizeof(CsrBtFtcMovingObjReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->ucs2SrcName), buffer, &offset, ((CsrSize) sizeof(primitive->ucs2SrcName)));
    CsrMemCpyDes(((void *) &primitive->ucs2DestName), buffer, &offset, ((CsrSize) sizeof(primitive->ucs2DestName)));
    primitive->ucs2SrcName = CsrPmemZalloc(CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    CsrMemCpy(((CsrUint8*) primitive->ucs2SrcName), ((CsrUint8*) &buffer[offset]),CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    offset += ((CsrSize) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->ucs2SrcName) + 2));
    primitive->ucs2DestName = CsrPmemZalloc(CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    CsrMemCpy(((CsrUint8*) primitive->ucs2DestName), ((CsrUint8*) &buffer[offset]),CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    offset += ((CsrSize) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->ucs2DestName) + 2));

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTC_MOVING_OBJ_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_MOVING_OBJ_REQ
void CsrBtFtcMovingObjReqSerFree(void *msg)
{
    CsrBtFtcMovingObjReq *primitive;

    primitive = (CsrBtFtcMovingObjReq *) msg;

    if(primitive->ucs2SrcName != NULL)
    {
        CsrPmemFree(primitive->ucs2SrcName);
    }
    if(primitive->ucs2DestName != NULL)
    {
        CsrPmemFree(primitive->ucs2DestName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_FTC_MOVING_OBJ_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_SET_BACK_FOLDER_REQ
CsrSize CsrBtFtcSetBackFolderReqSizeof(void *msg)
{
    CsrBtFtcSetBackFolderReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTC_SET_BACK_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_SET_BACK_FOLDER_REQ
CsrUint8 *CsrBtFtcSetBackFolderReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtcSetBackFolderReq *primitive;

    primitive = (CsrBtFtcSetBackFolderReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTC_SET_BACK_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_SET_BACK_FOLDER_REQ
void *CsrBtFtcSetBackFolderReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtcSetBackFolderReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtcSetBackFolderReq *) CsrPmemZalloc(sizeof(CsrBtFtcSetBackFolderReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTC_SET_BACK_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_SECURITY_OUT_REQ
CsrSize CsrBtFtcSecurityOutReqSizeof(void *msg)
{
    CsrBtFtcSecurityOutReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTC_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_SECURITY_OUT_REQ
CsrUint8 *CsrBtFtcSecurityOutReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtcSecurityOutReq *primitive;

    primitive = (CsrBtFtcSecurityOutReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTC_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_SECURITY_OUT_REQ
void *CsrBtFtcSecurityOutReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtcSecurityOutReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtcSecurityOutReq *) CsrPmemZalloc(sizeof(CsrBtFtcSecurityOutReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTC_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_REQ
CsrSize CsrBtFtcGetListFolderReqSizeof(void *msg)
{
    CsrBtFtcGetListFolderReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrConverterUcs2ByteStrLen((CsrUtf8String*) primitive->ucs2name) + 2);
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_REQ
CsrUint8 *CsrBtFtcGetListFolderReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtcGetListFolderReq *primitive;

    primitive = (CsrBtFtcGetListFolderReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->ucs2name, sizeof(primitive->ucs2name));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
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
#endif /* EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_REQ
void *CsrBtFtcGetListFolderReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtcGetListFolderReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtcGetListFolderReq *) CsrPmemZalloc(sizeof(CsrBtFtcGetListFolderReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->ucs2name), buffer, &offset, ((CsrSize) sizeof(primitive->ucs2name)));
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    primitive->ucs2name = CsrPmemZalloc(CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    CsrMemCpy(((CsrUint8*) primitive->ucs2name), ((CsrUint8*) &buffer[offset]),CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    offset += ((CsrSize) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->ucs2name) + 2));

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_REQ
void CsrBtFtcGetListFolderReqSerFree(void *msg)
{
    CsrBtFtcGetListFolderReq *primitive;

    primitive = (CsrBtFtcGetListFolderReq *) msg;

    if(primitive->ucs2name != NULL)
    {
        CsrPmemFree(primitive->ucs2name);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_CANCEL_CONNECT_REQ
CsrSize CsrBtFtcCancelConnectReqSizeof(void *msg)
{
    CsrBtFtcCancelConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTC_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_CANCEL_CONNECT_REQ
CsrUint8 *CsrBtFtcCancelConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtcCancelConnectReq *primitive;

    primitive = (CsrBtFtcCancelConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTC_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_CANCEL_CONNECT_REQ
void *CsrBtFtcCancelConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtcCancelConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtcCancelConnectReq *) CsrPmemZalloc(sizeof(CsrBtFtcCancelConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTC_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_DEL_OBJ_CFM
CsrSize CsrBtFtcDelObjCfmSizeof(void *msg)
{
    CsrBtFtcDelObjCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTC_DEL_OBJ_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_DEL_OBJ_CFM
CsrUint8 *CsrBtFtcDelObjCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtcDelObjCfm *primitive;

    primitive = (CsrBtFtcDelObjCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTC_DEL_OBJ_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_DEL_OBJ_CFM
void *CsrBtFtcDelObjCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtcDelObjCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtcDelObjCfm *) CsrPmemZalloc(sizeof(CsrBtFtcDelObjCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTC_DEL_OBJ_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_SET_BACK_FOLDER_CFM
CsrSize CsrBtFtcSetBackFolderCfmSizeof(void *msg)
{
    CsrBtFtcSetBackFolderCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTC_SET_BACK_FOLDER_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_SET_BACK_FOLDER_CFM
CsrUint8 *CsrBtFtcSetBackFolderCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtcSetBackFolderCfm *primitive;

    primitive = (CsrBtFtcSetBackFolderCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTC_SET_BACK_FOLDER_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_SET_BACK_FOLDER_CFM
void *CsrBtFtcSetBackFolderCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtcSetBackFolderCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtcSetBackFolderCfm *) CsrPmemZalloc(sizeof(CsrBtFtcSetBackFolderCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTC_SET_BACK_FOLDER_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_AUTHENTICATE_RES
CsrSize CsrBtFtcAuthenticateResSizeof(void *msg)
{
    CsrBtFtcAuthenticateRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->passwordLength * sizeof(CsrUint8)) + CsrCharStringSerLen((CsrCharString *) primitive->userId));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTC_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_FTC_AUTHENTICATE_RES
CsrUint8 *CsrBtFtcAuthenticateResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtcAuthenticateRes *primitive;

    primitive = (CsrBtFtcAuthenticateRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->password, sizeof(primitive->password));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->passwordLength);
    CsrMemCpySer(buffer, length, &primitive->userId, sizeof(primitive->userId));
    if(primitive->password)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->password), ((CsrSize) (primitive->passwordLength * sizeof(CsrUint8))));
    }
    CsrCharStringSer(buffer, length, primitive->userId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTC_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_FTC_AUTHENTICATE_RES
void *CsrBtFtcAuthenticateResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtcAuthenticateRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtcAuthenticateRes *) CsrPmemZalloc(sizeof(CsrBtFtcAuthenticateRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->password), buffer, &offset, ((CsrSize) sizeof(primitive->password)));
    CsrUint16Des((CsrUint16 *) &primitive->passwordLength, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_FTC_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_FTC_AUTHENTICATE_RES
void CsrBtFtcAuthenticateResSerFree(void *msg)
{
    CsrBtFtcAuthenticateRes *primitive;

    primitive = (CsrBtFtcAuthenticateRes *) msg;

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
#endif /* EXCLUDE_CSR_BT_FTC_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_BODY_REQ
CsrSize CsrBtFtcGetListFolderBodyReqSizeof(void *msg)
{
    CsrBtFtcGetListFolderBodyReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_BODY_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_BODY_REQ
CsrUint8 *CsrBtFtcGetListFolderBodyReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtcGetListFolderBodyReq *primitive;

    primitive = (CsrBtFtcGetListFolderBodyReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_BODY_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_BODY_REQ
void *CsrBtFtcGetListFolderBodyReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtcGetListFolderBodyReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtcGetListFolderBodyReq *) CsrPmemZalloc(sizeof(CsrBtFtcGetListFolderBodyReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_BODY_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_PUT_OBJ_HEADER_CFM
CsrSize CsrBtFtcPutObjHeaderCfmSizeof(void *msg)
{
    CsrBtFtcPutObjHeaderCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTC_PUT_OBJ_HEADER_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_PUT_OBJ_HEADER_CFM
CsrUint8 *CsrBtFtcPutObjHeaderCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtcPutObjHeaderCfm *primitive;

    primitive = (CsrBtFtcPutObjHeaderCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodySize);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTC_PUT_OBJ_HEADER_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_PUT_OBJ_HEADER_CFM
void *CsrBtFtcPutObjHeaderCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtcPutObjHeaderCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtcPutObjHeaderCfm *) CsrPmemZalloc(sizeof(CsrBtFtcPutObjHeaderCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodySize, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTC_PUT_OBJ_HEADER_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_GET_OBJ_REQ
CsrSize CsrBtFtcGetObjReqSizeof(void *msg)
{
    CsrBtFtcGetObjReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrConverterUcs2ByteStrLen((CsrUtf8String*) primitive->ucs2name) + 2);
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTC_GET_OBJ_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_GET_OBJ_REQ
CsrUint8 *CsrBtFtcGetObjReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtcGetObjReq *primitive;

    primitive = (CsrBtFtcGetObjReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->ucs2name, sizeof(primitive->ucs2name));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
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
#endif /* EXCLUDE_CSR_BT_FTC_GET_OBJ_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_GET_OBJ_REQ
void *CsrBtFtcGetObjReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtcGetObjReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtcGetObjReq *) CsrPmemZalloc(sizeof(CsrBtFtcGetObjReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->ucs2name), buffer, &offset, ((CsrSize) sizeof(primitive->ucs2name)));
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    primitive->ucs2name = CsrPmemZalloc(CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    CsrMemCpy(((CsrUint8*) primitive->ucs2name), ((CsrUint8*) &buffer[offset]),CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    offset += ((CsrSize) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->ucs2name) + 2));

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTC_GET_OBJ_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_GET_OBJ_REQ
void CsrBtFtcGetObjReqSerFree(void *msg)
{
    CsrBtFtcGetObjReq *primitive;

    primitive = (CsrBtFtcGetObjReq *) msg;

    if(primitive->ucs2name != NULL)
    {
        CsrPmemFree(primitive->ucs2name);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_FTC_GET_OBJ_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_SET_ROOT_FOLDER_REQ
CsrSize CsrBtFtcSetRootFolderReqSizeof(void *msg)
{
    CsrBtFtcSetRootFolderReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTC_SET_ROOT_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_SET_ROOT_FOLDER_REQ
CsrUint8 *CsrBtFtcSetRootFolderReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtcSetRootFolderReq *primitive;

    primitive = (CsrBtFtcSetRootFolderReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTC_SET_ROOT_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_SET_ROOT_FOLDER_REQ
void *CsrBtFtcSetRootFolderReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtcSetRootFolderReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtcSetRootFolderReq *) CsrPmemZalloc(sizeof(CsrBtFtcSetRootFolderReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTC_SET_ROOT_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_SET_ADD_FOLDER_CFM
CsrSize CsrBtFtcSetAddFolderCfmSizeof(void *msg)
{
    CsrBtFtcSetAddFolderCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTC_SET_ADD_FOLDER_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_SET_ADD_FOLDER_CFM
CsrUint8 *CsrBtFtcSetAddFolderCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtcSetAddFolderCfm *primitive;

    primitive = (CsrBtFtcSetAddFolderCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTC_SET_ADD_FOLDER_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_SET_ADD_FOLDER_CFM
void *CsrBtFtcSetAddFolderCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtcSetAddFolderCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtcSetAddFolderCfm *) CsrPmemZalloc(sizeof(CsrBtFtcSetAddFolderCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTC_SET_ADD_FOLDER_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_GET_OBJ_CFM
CsrSize CsrBtFtcGetObjCfmSizeof(void *msg)
{
    CsrBtFtcGetObjCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTC_GET_OBJ_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_GET_OBJ_CFM
CsrUint8 *CsrBtFtcGetObjCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtcGetObjCfm *primitive;

    primitive = (CsrBtFtcGetObjCfm *) msg;
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
#endif /* EXCLUDE_CSR_BT_FTC_GET_OBJ_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_GET_OBJ_CFM
void *CsrBtFtcGetObjCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtcGetObjCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtcGetObjCfm *) CsrPmemZalloc(sizeof(CsrBtFtcGetObjCfm));

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
#endif /* EXCLUDE_CSR_BT_FTC_GET_OBJ_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_GET_OBJ_CFM
void CsrBtFtcGetObjCfmSerFree(void *msg)
{
    CsrBtFtcGetObjCfm *primitive;

    primitive = (CsrBtFtcGetObjCfm *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_FTC_GET_OBJ_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_CFM
CsrSize CsrBtFtcGetListFolderCfmSizeof(void *msg)
{
    CsrBtFtcGetListFolderCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_CFM
CsrUint8 *CsrBtFtcGetListFolderCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtcGetListFolderCfm *primitive;

    primitive = (CsrBtFtcGetListFolderCfm *) msg;
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
#endif /* EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_CFM
void *CsrBtFtcGetListFolderCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtcGetListFolderCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtcGetListFolderCfm *) CsrPmemZalloc(sizeof(CsrBtFtcGetListFolderCfm));

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
#endif /* EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_CFM
void CsrBtFtcGetListFolderCfmSerFree(void *msg)
{
    CsrBtFtcGetListFolderCfm *primitive;

    primitive = (CsrBtFtcGetListFolderCfm *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_ABORT_REQ
CsrSize CsrBtFtcAbortReqSizeof(void *msg)
{
    CsrBtFtcAbortReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTC_ABORT_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_ABORT_REQ
CsrUint8 *CsrBtFtcAbortReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtcAbortReq *primitive;

    primitive = (CsrBtFtcAbortReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTC_ABORT_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_ABORT_REQ
void *CsrBtFtcAbortReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtcAbortReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtcAbortReq *) CsrPmemZalloc(sizeof(CsrBtFtcAbortReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTC_ABORT_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_MOVING_OBJ_CFM
CsrSize CsrBtFtcMovingObjCfmSizeof(void *msg)
{
    CsrBtFtcMovingObjCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTC_MOVING_OBJ_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_MOVING_OBJ_CFM
CsrUint8 *CsrBtFtcMovingObjCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtcMovingObjCfm *primitive;

    primitive = (CsrBtFtcMovingObjCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTC_MOVING_OBJ_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_MOVING_OBJ_CFM
void *CsrBtFtcMovingObjCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtcMovingObjCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtcMovingObjCfm *) CsrPmemZalloc(sizeof(CsrBtFtcMovingObjCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTC_MOVING_OBJ_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_GET_OBJ_BODY_REQ
CsrSize CsrBtFtcGetObjBodyReqSizeof(void *msg)
{
    CsrBtFtcGetObjBodyReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTC_GET_OBJ_BODY_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_GET_OBJ_BODY_REQ
CsrUint8 *CsrBtFtcGetObjBodyReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtcGetObjBodyReq *primitive;

    primitive = (CsrBtFtcGetObjBodyReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTC_GET_OBJ_BODY_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_GET_OBJ_BODY_REQ
void *CsrBtFtcGetObjBodyReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtcGetObjBodyReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtcGetObjBodyReq *) CsrPmemZalloc(sizeof(CsrBtFtcGetObjBodyReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTC_GET_OBJ_BODY_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_DISCONNECT_IND
CsrSize CsrBtFtcDisconnectIndSizeof(void *msg)
{
    CsrBtFtcDisconnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTC_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_FTC_DISCONNECT_IND
CsrUint8 *CsrBtFtcDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtcDisconnectInd *primitive;

    primitive = (CsrBtFtcDisconnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTC_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_FTC_DISCONNECT_IND
void *CsrBtFtcDisconnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtcDisconnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtcDisconnectInd *) CsrPmemZalloc(sizeof(CsrBtFtcDisconnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTC_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_FTC_PUT_OBJ_BODY_CFM
CsrSize CsrBtFtcPutObjBodyCfmSizeof(void *msg)
{
    CsrBtFtcPutObjBodyCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTC_PUT_OBJ_BODY_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_PUT_OBJ_BODY_CFM
CsrUint8 *CsrBtFtcPutObjBodyCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtcPutObjBodyCfm *primitive;

    primitive = (CsrBtFtcPutObjBodyCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodySize);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTC_PUT_OBJ_BODY_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_PUT_OBJ_BODY_CFM
void *CsrBtFtcPutObjBodyCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtcPutObjBodyCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtcPutObjBodyCfm *) CsrPmemZalloc(sizeof(CsrBtFtcPutObjBodyCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodySize, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTC_PUT_OBJ_BODY_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_SET_OBJ_PERMISSIONS_CFM
CsrSize CsrBtFtcSetObjPermissionsCfmSizeof(void *msg)
{
    CsrBtFtcSetObjPermissionsCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTC_SET_OBJ_PERMISSIONS_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_SET_OBJ_PERMISSIONS_CFM
CsrUint8 *CsrBtFtcSetObjPermissionsCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtcSetObjPermissionsCfm *primitive;

    primitive = (CsrBtFtcSetObjPermissionsCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTC_SET_OBJ_PERMISSIONS_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_SET_OBJ_PERMISSIONS_CFM
void *CsrBtFtcSetObjPermissionsCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtcSetObjPermissionsCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtcSetObjPermissionsCfm *) CsrPmemZalloc(sizeof(CsrBtFtcSetObjPermissionsCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTC_SET_OBJ_PERMISSIONS_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_SET_ADD_FOLDER_REQ
CsrSize CsrBtFtcSetAddFolderReqSizeof(void *msg)
{
    CsrBtFtcSetAddFolderReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrConverterUcs2ByteStrLen((CsrUtf8String*) primitive->ucs2name) + 2);
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTC_SET_ADD_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_SET_ADD_FOLDER_REQ
CsrUint8 *CsrBtFtcSetAddFolderReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtcSetAddFolderReq *primitive;

    primitive = (CsrBtFtcSetAddFolderReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->ucs2name, sizeof(primitive->ucs2name));
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
#endif /* EXCLUDE_CSR_BT_FTC_SET_ADD_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_SET_ADD_FOLDER_REQ
void *CsrBtFtcSetAddFolderReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtcSetAddFolderReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtcSetAddFolderReq *) CsrPmemZalloc(sizeof(CsrBtFtcSetAddFolderReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->ucs2name), buffer, &offset, ((CsrSize) sizeof(primitive->ucs2name)));
    primitive->ucs2name = CsrPmemZalloc(CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    CsrMemCpy(((CsrUint8*) primitive->ucs2name), ((CsrUint8*) &buffer[offset]),CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    offset += ((CsrSize) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->ucs2name) + 2));

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTC_SET_ADD_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_SET_ADD_FOLDER_REQ
void CsrBtFtcSetAddFolderReqSerFree(void *msg)
{
    CsrBtFtcSetAddFolderReq *primitive;

    primitive = (CsrBtFtcSetAddFolderReq *) msg;

    if(primitive->ucs2name != NULL)
    {
        CsrPmemFree(primitive->ucs2name);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_FTC_SET_ADD_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_AUTHENTICATE_IND
CsrSize CsrBtFtcAuthenticateIndSizeof(void *msg)
{
    CsrBtFtcAuthenticateInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->realmLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTC_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_FTC_AUTHENTICATE_IND
CsrUint8 *CsrBtFtcAuthenticateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtcAuthenticateInd *primitive;

    primitive = (CsrBtFtcAuthenticateInd *) msg;
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
#endif /* EXCLUDE_CSR_BT_FTC_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_FTC_AUTHENTICATE_IND
void *CsrBtFtcAuthenticateIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtcAuthenticateInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtcAuthenticateInd *) CsrPmemZalloc(sizeof(CsrBtFtcAuthenticateInd));

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
#endif /* EXCLUDE_CSR_BT_FTC_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_FTC_AUTHENTICATE_IND
void CsrBtFtcAuthenticateIndSerFree(void *msg)
{
    CsrBtFtcAuthenticateInd *primitive;

    primitive = (CsrBtFtcAuthenticateInd *) msg;

    if(primitive->realm != NULL)
    {
        CsrPmemFree(primitive->realm);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_FTC_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_FTC_SECURITY_OUT_CFM
CsrSize CsrBtFtcSecurityOutCfmSizeof(void *msg)
{
    CsrBtFtcSecurityOutCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTC_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_SECURITY_OUT_CFM
CsrUint8 *CsrBtFtcSecurityOutCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtcSecurityOutCfm *primitive;

    primitive = (CsrBtFtcSecurityOutCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTC_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_SECURITY_OUT_CFM
void *CsrBtFtcSecurityOutCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtcSecurityOutCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtcSecurityOutCfm *) CsrPmemZalloc(sizeof(CsrBtFtcSecurityOutCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTC_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_DEL_OBJ_REQ
CsrSize CsrBtFtcDelObjReqSizeof(void *msg)
{
    CsrBtFtcDelObjReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrConverterUcs2ByteStrLen((CsrUtf8String*) primitive->ucs2name) + 2);
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTC_DEL_OBJ_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_DEL_OBJ_REQ
CsrUint8 *CsrBtFtcDelObjReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtcDelObjReq *primitive;

    primitive = (CsrBtFtcDelObjReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->ucs2name, sizeof(primitive->ucs2name));
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
#endif /* EXCLUDE_CSR_BT_FTC_DEL_OBJ_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_DEL_OBJ_REQ
void *CsrBtFtcDelObjReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtcDelObjReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtcDelObjReq *) CsrPmemZalloc(sizeof(CsrBtFtcDelObjReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->ucs2name), buffer, &offset, ((CsrSize) sizeof(primitive->ucs2name)));
    primitive->ucs2name = CsrPmemZalloc(CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    CsrMemCpy(((CsrUint8*) primitive->ucs2name), ((CsrUint8*) &buffer[offset]),CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    offset += ((CsrSize) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->ucs2name) + 2));

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTC_DEL_OBJ_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_DEL_OBJ_REQ
void CsrBtFtcDelObjReqSerFree(void *msg)
{
    CsrBtFtcDelObjReq *primitive;

    primitive = (CsrBtFtcDelObjReq *) msg;

    if(primitive->ucs2name != NULL)
    {
        CsrPmemFree(primitive->ucs2name);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_FTC_DEL_OBJ_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_GET_OBJ_BODY_CFM
CsrSize CsrBtFtcGetObjBodyCfmSizeof(void *msg)
{
    CsrBtFtcGetObjBodyCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTC_GET_OBJ_BODY_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_GET_OBJ_BODY_CFM
CsrUint8 *CsrBtFtcGetObjBodyCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtcGetObjBodyCfm *primitive;

    primitive = (CsrBtFtcGetObjBodyCfm *) msg;
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
#endif /* EXCLUDE_CSR_BT_FTC_GET_OBJ_BODY_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_GET_OBJ_BODY_CFM
void *CsrBtFtcGetObjBodyCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtcGetObjBodyCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtcGetObjBodyCfm *) CsrPmemZalloc(sizeof(CsrBtFtcGetObjBodyCfm));

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
#endif /* EXCLUDE_CSR_BT_FTC_GET_OBJ_BODY_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_GET_OBJ_BODY_CFM
void CsrBtFtcGetObjBodyCfmSerFree(void *msg)
{
    CsrBtFtcGetObjBodyCfm *primitive;

    primitive = (CsrBtFtcGetObjBodyCfm *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_FTC_GET_OBJ_BODY_CFM */

#endif /* EXCLUDE_CSR_BT_FTC_MODULE */
