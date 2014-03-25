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
#include "csr_bt_pac_prim.h"

#ifndef EXCLUDE_CSR_BT_PAC_MODULE

void CsrBtPacPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_PAC_SET_FOLDER_REQ
CsrSize CsrBtPacSetFolderReqSizeof(void *msg)
{
    CsrBtPacSetFolderReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrConverterUcs2ByteStrLen((CsrUtf8String*) primitive->ucs2name) + 2);
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAC_SET_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_SET_FOLDER_REQ
CsrUint8 *CsrBtPacSetFolderReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPacSetFolderReq *primitive;

    primitive = (CsrBtPacSetFolderReq *) msg;
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
#endif /* EXCLUDE_CSR_BT_PAC_SET_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_SET_FOLDER_REQ
void *CsrBtPacSetFolderReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPacSetFolderReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPacSetFolderReq *) CsrPmemZalloc(sizeof(CsrBtPacSetFolderReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->ucs2name), buffer, &offset, ((CsrSize) sizeof(primitive->ucs2name)));
    primitive->ucs2name = CsrPmemZalloc(CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    CsrMemCpy(((CsrUint8*) primitive->ucs2name), ((CsrUint8*) &buffer[offset]),CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    offset += ((CsrSize) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->ucs2name) + 2));

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAC_SET_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_SET_FOLDER_REQ
void CsrBtPacSetFolderReqSerFree(void *msg)
{
    CsrBtPacSetFolderReq *primitive;

    primitive = (CsrBtPacSetFolderReq *) msg;

    if(primitive->ucs2name != NULL)
    {
        CsrPmemFree(primitive->ucs2name);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_PAC_SET_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_SET_FOLDER_CFM
CsrSize CsrBtPacSetFolderCfmSizeof(void *msg)
{
    CsrBtPacSetFolderCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAC_SET_FOLDER_CFM */

#ifndef EXCLUDE_CSR_BT_PAC_SET_FOLDER_CFM
CsrUint8 *CsrBtPacSetFolderCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPacSetFolderCfm *primitive;

    primitive = (CsrBtPacSetFolderCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAC_SET_FOLDER_CFM */

#ifndef EXCLUDE_CSR_BT_PAC_SET_FOLDER_CFM
void *CsrBtPacSetFolderCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPacSetFolderCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPacSetFolderCfm *) CsrPmemZalloc(sizeof(CsrBtPacSetFolderCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAC_SET_FOLDER_CFM */

#ifndef EXCLUDE_CSR_BT_PAC_SET_ROOT_FOLDER_REQ
CsrSize CsrBtPacSetRootFolderReqSizeof(void *msg)
{
    CsrBtPacSetRootFolderReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAC_SET_ROOT_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_SET_ROOT_FOLDER_REQ
CsrUint8 *CsrBtPacSetRootFolderReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPacSetRootFolderReq *primitive;

    primitive = (CsrBtPacSetRootFolderReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAC_SET_ROOT_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_SET_ROOT_FOLDER_REQ
void *CsrBtPacSetRootFolderReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPacSetRootFolderReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPacSetRootFolderReq *) CsrPmemZalloc(sizeof(CsrBtPacSetRootFolderReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAC_SET_ROOT_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_SET_BACK_FOLDER_REQ
CsrSize CsrBtPacSetBackFolderReqSizeof(void *msg)
{
    CsrBtPacSetBackFolderReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAC_SET_BACK_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_SET_BACK_FOLDER_REQ
CsrUint8 *CsrBtPacSetBackFolderReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPacSetBackFolderReq *primitive;

    primitive = (CsrBtPacSetBackFolderReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAC_SET_BACK_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_SET_BACK_FOLDER_REQ
void *CsrBtPacSetBackFolderReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPacSetBackFolderReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPacSetBackFolderReq *) CsrPmemZalloc(sizeof(CsrBtPacSetBackFolderReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAC_SET_BACK_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_PB_RES
CsrSize CsrBtPacPullPbResSizeof(void *msg)
{
    CsrBtPacPullPbRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAC_PULL_PB_RES */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_PB_RES
CsrUint8 *CsrBtPacPullPbResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPacPullPbRes *primitive;

    primitive = (CsrBtPacPullPbRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAC_PULL_PB_RES */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_PB_RES
void *CsrBtPacPullPbResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPacPullPbRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPacPullPbRes *) CsrPmemZalloc(sizeof(CsrBtPacPullPbRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAC_PULL_PB_RES */

#ifndef EXCLUDE_CSR_BT_PAC_SECURITY_OUT_CFM
CsrSize CsrBtPacSecurityOutCfmSizeof(void *msg)
{
    CsrBtPacSecurityOutCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAC_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_PAC_SECURITY_OUT_CFM
CsrUint8 *CsrBtPacSecurityOutCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPacSecurityOutCfm *primitive;

    primitive = (CsrBtPacSecurityOutCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAC_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_PAC_SECURITY_OUT_CFM
void *CsrBtPacSecurityOutCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPacSecurityOutCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPacSecurityOutCfm *) CsrPmemZalloc(sizeof(CsrBtPacSecurityOutCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAC_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_PAC_AUTHENTICATE_IND
CsrSize CsrBtPacAuthenticateIndSizeof(void *msg)
{
    CsrBtPacAuthenticateInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->realmLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAC_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_PAC_AUTHENTICATE_IND
CsrUint8 *CsrBtPacAuthenticateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPacAuthenticateInd *primitive;

    primitive = (CsrBtPacAuthenticateInd *) msg;
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
#endif /* EXCLUDE_CSR_BT_PAC_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_PAC_AUTHENTICATE_IND
void *CsrBtPacAuthenticateIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPacAuthenticateInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPacAuthenticateInd *) CsrPmemZalloc(sizeof(CsrBtPacAuthenticateInd));

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
#endif /* EXCLUDE_CSR_BT_PAC_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_PAC_AUTHENTICATE_IND
void CsrBtPacAuthenticateIndSerFree(void *msg)
{
    CsrBtPacAuthenticateInd *primitive;

    primitive = (CsrBtPacAuthenticateInd *) msg;

    if(primitive->realm != NULL)
    {
        CsrPmemFree(primitive->realm);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_PAC_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_PAC_ABORT_REQ
CsrSize CsrBtPacAbortReqSizeof(void *msg)
{
    CsrBtPacAbortReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAC_ABORT_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_ABORT_REQ
CsrUint8 *CsrBtPacAbortReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPacAbortReq *primitive;

    primitive = (CsrBtPacAbortReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAC_ABORT_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_ABORT_REQ
void *CsrBtPacAbortReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPacAbortReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPacAbortReq *) CsrPmemZalloc(sizeof(CsrBtPacAbortReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAC_ABORT_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_PB_REQ
CsrSize CsrBtPacPullPbReqSizeof(void *msg)
{
    CsrBtPacPullPbReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrConverterUcs2ByteStrLen((CsrUtf8String*) primitive->ucs2name) + 2);
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAC_PULL_PB_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_PB_REQ
CsrUint8 *CsrBtPacPullPbReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPacPullPbReq *primitive;

    primitive = (CsrBtPacPullPbReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->ucs2name, sizeof(primitive->ucs2name));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->src);
    CsrMemCpySer(buffer, length, ((const void *) primitive->filter), ((CsrSize) (8)));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->format);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxListCnt);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->listStartOffset);
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
#endif /* EXCLUDE_CSR_BT_PAC_PULL_PB_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_PB_REQ
void *CsrBtPacPullPbReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPacPullPbReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPacPullPbReq *) CsrPmemZalloc(sizeof(CsrBtPacPullPbReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->ucs2name), buffer, &offset, ((CsrSize) sizeof(primitive->ucs2name)));
    CsrUint16Des((CsrUint16 *) &primitive->src, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->filter), buffer, &offset, ((CsrSize) (8)));
    CsrUint8Des((CsrUint8 *) &primitive->format, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxListCnt, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->listStartOffset, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    primitive->ucs2name = CsrPmemZalloc(CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    CsrMemCpy(((CsrUint8*) primitive->ucs2name), ((CsrUint8*) &buffer[offset]),CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    offset += ((CsrSize) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->ucs2name) + 2));

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAC_PULL_PB_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_PB_REQ
void CsrBtPacPullPbReqSerFree(void *msg)
{
    CsrBtPacPullPbReq *primitive;

    primitive = (CsrBtPacPullPbReq *) msg;

    if(primitive->ucs2name != NULL)
    {
        CsrPmemFree(primitive->ucs2name);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_PAC_PULL_PB_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_VCARD_ENTRY_RES
CsrSize CsrBtPacPullVcardEntryResSizeof(void *msg)
{
    CsrBtPacPullVcardEntryRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAC_PULL_VCARD_ENTRY_RES */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_VCARD_ENTRY_RES
CsrUint8 *CsrBtPacPullVcardEntryResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPacPullVcardEntryRes *primitive;

    primitive = (CsrBtPacPullVcardEntryRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAC_PULL_VCARD_ENTRY_RES */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_VCARD_ENTRY_RES
void *CsrBtPacPullVcardEntryResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPacPullVcardEntryRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPacPullVcardEntryRes *) CsrPmemZalloc(sizeof(CsrBtPacPullVcardEntryRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAC_PULL_VCARD_ENTRY_RES */

#ifndef EXCLUDE_CSR_BT_PAC_DISCONNECT_REQ
CsrSize CsrBtPacDisconnectReqSizeof(void *msg)
{
    CsrBtPacDisconnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAC_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_DISCONNECT_REQ
CsrUint8 *CsrBtPacDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPacDisconnectReq *primitive;

    primitive = (CsrBtPacDisconnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->normalDisconnect);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAC_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_DISCONNECT_REQ
void *CsrBtPacDisconnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPacDisconnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPacDisconnectReq *) CsrPmemZalloc(sizeof(CsrBtPacDisconnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->normalDisconnect, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAC_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_VCARD_ENTRY_REQ
CsrSize CsrBtPacPullVcardEntryReqSizeof(void *msg)
{
    CsrBtPacPullVcardEntryReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrConverterUcs2ByteStrLen((CsrUtf8String*) primitive->ucs2name) + 2);
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAC_PULL_VCARD_ENTRY_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_VCARD_ENTRY_REQ
CsrUint8 *CsrBtPacPullVcardEntryReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPacPullVcardEntryReq *primitive;

    primitive = (CsrBtPacPullVcardEntryReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->ucs2name, sizeof(primitive->ucs2name));
    CsrMemCpySer(buffer, length, ((const void *) primitive->filter), ((CsrSize) (8)));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->format);
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
#endif /* EXCLUDE_CSR_BT_PAC_PULL_VCARD_ENTRY_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_VCARD_ENTRY_REQ
void *CsrBtPacPullVcardEntryReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPacPullVcardEntryReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPacPullVcardEntryReq *) CsrPmemZalloc(sizeof(CsrBtPacPullVcardEntryReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->ucs2name), buffer, &offset, ((CsrSize) sizeof(primitive->ucs2name)));
    CsrMemCpyDes(((void *) primitive->filter), buffer, &offset, ((CsrSize) (8)));
    CsrUint8Des((CsrUint8 *) &primitive->format, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    primitive->ucs2name = CsrPmemZalloc(CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    CsrMemCpy(((CsrUint8*) primitive->ucs2name), ((CsrUint8*) &buffer[offset]),CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    offset += ((CsrSize) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->ucs2name) + 2));

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAC_PULL_VCARD_ENTRY_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_VCARD_ENTRY_REQ
void CsrBtPacPullVcardEntryReqSerFree(void *msg)
{
    CsrBtPacPullVcardEntryReq *primitive;

    primitive = (CsrBtPacPullVcardEntryReq *) msg;

    if(primitive->ucs2name != NULL)
    {
        CsrPmemFree(primitive->ucs2name);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_PAC_PULL_VCARD_ENTRY_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_HOUSE_CLEANING
CsrSize CsrBtPacHouseCleaningSizeof(void *msg)
{
    CsrBtPacHouseCleaning *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAC_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_PAC_HOUSE_CLEANING
CsrUint8 *CsrBtPacHouseCleaningSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPacHouseCleaning *primitive;

    primitive = (CsrBtPacHouseCleaning *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAC_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_PAC_HOUSE_CLEANING
void *CsrBtPacHouseCleaningDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPacHouseCleaning *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPacHouseCleaning *) CsrPmemZalloc(sizeof(CsrBtPacHouseCleaning));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAC_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_VCARD_LIST_REQ
CsrSize CsrBtPacPullVcardListReqSizeof(void *msg)
{
    CsrBtPacPullVcardListReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrConverterUcs2ByteStrLen((CsrUtf8String*) primitive->ucs2name) + 2 + (primitive->searchValLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAC_PULL_VCARD_LIST_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_VCARD_LIST_REQ
CsrUint8 *CsrBtPacPullVcardListReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPacPullVcardListReq *primitive;

    primitive = (CsrBtPacPullVcardListReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->ucs2name, sizeof(primitive->ucs2name));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->order);
    CsrMemCpySer(buffer, length, &primitive->searchVal, sizeof(primitive->searchVal));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->searchValLen);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->searchAtt);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxListCnt);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->listStartOffset);
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
    if(primitive->searchVal)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->searchVal), ((CsrSize) (primitive->searchValLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAC_PULL_VCARD_LIST_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_VCARD_LIST_REQ
void *CsrBtPacPullVcardListReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPacPullVcardListReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPacPullVcardListReq *) CsrPmemZalloc(sizeof(CsrBtPacPullVcardListReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->ucs2name), buffer, &offset, ((CsrSize) sizeof(primitive->ucs2name)));
    CsrUint8Des((CsrUint8 *) &primitive->order, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->searchVal), buffer, &offset, ((CsrSize) sizeof(primitive->searchVal)));
    CsrUint16Des((CsrUint16 *) &primitive->searchValLen, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->searchAtt, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxListCnt, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->listStartOffset, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    primitive->ucs2name = CsrPmemZalloc(CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    CsrMemCpy(((CsrUint8*) primitive->ucs2name), ((CsrUint8*) &buffer[offset]),CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    offset += ((CsrSize) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->ucs2name) + 2));
    if (primitive->searchValLen)
    {
        primitive->searchVal = CsrPmemZalloc(((CsrUint32) (primitive->searchValLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->searchVal), buffer, &offset, ((CsrSize) (primitive->searchValLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->searchVal = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAC_PULL_VCARD_LIST_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_VCARD_LIST_REQ
void CsrBtPacPullVcardListReqSerFree(void *msg)
{
    CsrBtPacPullVcardListReq *primitive;

    primitive = (CsrBtPacPullVcardListReq *) msg;

    if(primitive->ucs2name != NULL)
    {
        CsrPmemFree(primitive->ucs2name);
    }
    if(primitive->searchVal != NULL)
    {
        CsrPmemFree(primitive->searchVal);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_PAC_PULL_VCARD_LIST_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_VCARD_ENTRY_CFM
CsrSize CsrBtPacPullVcardEntryCfmSizeof(void *msg)
{
    CsrBtPacPullVcardEntryCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAC_PULL_VCARD_ENTRY_CFM */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_VCARD_ENTRY_CFM
CsrUint8 *CsrBtPacPullVcardEntryCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPacPullVcardEntryCfm *primitive;

    primitive = (CsrBtPacPullVcardEntryCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAC_PULL_VCARD_ENTRY_CFM */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_VCARD_ENTRY_CFM
void *CsrBtPacPullVcardEntryCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPacPullVcardEntryCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPacPullVcardEntryCfm *) CsrPmemZalloc(sizeof(CsrBtPacPullVcardEntryCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAC_PULL_VCARD_ENTRY_CFM */

#ifndef EXCLUDE_CSR_BT_PAC_AUTHENTICATE_RES
CsrSize CsrBtPacAuthenticateResSizeof(void *msg)
{
    CsrBtPacAuthenticateRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->passwordLength * sizeof(CsrUint8)) + CsrCharStringSerLen((CsrCharString *) primitive->userId));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAC_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_PAC_AUTHENTICATE_RES
CsrUint8 *CsrBtPacAuthenticateResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPacAuthenticateRes *primitive;

    primitive = (CsrBtPacAuthenticateRes *) msg;
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
#endif /* EXCLUDE_CSR_BT_PAC_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_PAC_AUTHENTICATE_RES
void *CsrBtPacAuthenticateResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPacAuthenticateRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPacAuthenticateRes *) CsrPmemZalloc(sizeof(CsrBtPacAuthenticateRes));

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
#endif /* EXCLUDE_CSR_BT_PAC_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_PAC_AUTHENTICATE_RES
void CsrBtPacAuthenticateResSerFree(void *msg)
{
    CsrBtPacAuthenticateRes *primitive;

    primitive = (CsrBtPacAuthenticateRes *) msg;

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
#endif /* EXCLUDE_CSR_BT_PAC_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_PAC_CANCEL_CONNECT_REQ
CsrSize CsrBtPacCancelConnectReqSizeof(void *msg)
{
    CsrBtPacCancelConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAC_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_CANCEL_CONNECT_REQ
CsrUint8 *CsrBtPacCancelConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPacCancelConnectReq *primitive;

    primitive = (CsrBtPacCancelConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAC_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_CANCEL_CONNECT_REQ
void *CsrBtPacCancelConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPacCancelConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPacCancelConnectReq *) CsrPmemZalloc(sizeof(CsrBtPacCancelConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAC_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_PB_CFM
CsrSize CsrBtPacPullPbCfmSizeof(void *msg)
{
    CsrBtPacPullPbCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAC_PULL_PB_CFM */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_PB_CFM
CsrUint8 *CsrBtPacPullPbCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPacPullPbCfm *primitive;

    primitive = (CsrBtPacPullPbCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pbSize);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->newMissedCall);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAC_PULL_PB_CFM */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_PB_CFM
void *CsrBtPacPullPbCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPacPullPbCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPacPullPbCfm *) CsrPmemZalloc(sizeof(CsrBtPacPullPbCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pbSize, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->newMissedCall, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAC_PULL_PB_CFM */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_VCARD_LIST_IND
CsrSize CsrBtPacPullVcardListIndSizeof(void *msg)
{
    CsrBtPacPullVcardListInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAC_PULL_VCARD_LIST_IND */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_VCARD_LIST_IND
CsrUint8 *CsrBtPacPullVcardListIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPacPullVcardListInd *primitive;

    primitive = (CsrBtPacPullVcardListInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyOffset);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAC_PULL_VCARD_LIST_IND */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_VCARD_LIST_IND
void *CsrBtPacPullVcardListIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPacPullVcardListInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPacPullVcardListInd *) CsrPmemZalloc(sizeof(CsrBtPacPullVcardListInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyOffset, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_PAC_PULL_VCARD_LIST_IND */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_VCARD_LIST_IND
void CsrBtPacPullVcardListIndSerFree(void *msg)
{
    CsrBtPacPullVcardListInd *primitive;

    primitive = (CsrBtPacPullVcardListInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_PAC_PULL_VCARD_LIST_IND */

#ifndef EXCLUDE_CSR_BT_PAC_SET_ROOT_FOLDER_CFM
CsrSize CsrBtPacSetRootFolderCfmSizeof(void *msg)
{
    CsrBtPacSetRootFolderCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAC_SET_ROOT_FOLDER_CFM */

#ifndef EXCLUDE_CSR_BT_PAC_SET_ROOT_FOLDER_CFM
CsrUint8 *CsrBtPacSetRootFolderCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPacSetRootFolderCfm *primitive;

    primitive = (CsrBtPacSetRootFolderCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAC_SET_ROOT_FOLDER_CFM */

#ifndef EXCLUDE_CSR_BT_PAC_SET_ROOT_FOLDER_CFM
void *CsrBtPacSetRootFolderCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPacSetRootFolderCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPacSetRootFolderCfm *) CsrPmemZalloc(sizeof(CsrBtPacSetRootFolderCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAC_SET_ROOT_FOLDER_CFM */

#ifndef EXCLUDE_CSR_BT_PAC_ABORT_CFM
CsrSize CsrBtPacAbortCfmSizeof(void *msg)
{
    CsrBtPacAbortCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAC_ABORT_CFM */

#ifndef EXCLUDE_CSR_BT_PAC_ABORT_CFM
CsrUint8 *CsrBtPacAbortCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPacAbortCfm *primitive;

    primitive = (CsrBtPacAbortCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAC_ABORT_CFM */

#ifndef EXCLUDE_CSR_BT_PAC_ABORT_CFM
void *CsrBtPacAbortCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPacAbortCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPacAbortCfm *) CsrPmemZalloc(sizeof(CsrBtPacAbortCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAC_ABORT_CFM */

#ifndef EXCLUDE_CSR_BT_PAC_DISCONNECT_IND
CsrSize CsrBtPacDisconnectIndSizeof(void *msg)
{
    CsrBtPacDisconnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAC_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_PAC_DISCONNECT_IND
CsrUint8 *CsrBtPacDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPacDisconnectInd *primitive;

    primitive = (CsrBtPacDisconnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAC_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_PAC_DISCONNECT_IND
void *CsrBtPacDisconnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPacDisconnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPacDisconnectInd *) CsrPmemZalloc(sizeof(CsrBtPacDisconnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAC_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_VCARD_LIST_CFM
CsrSize CsrBtPacPullVcardListCfmSizeof(void *msg)
{
    CsrBtPacPullVcardListCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAC_PULL_VCARD_LIST_CFM */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_VCARD_LIST_CFM
CsrUint8 *CsrBtPacPullVcardListCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPacPullVcardListCfm *primitive;

    primitive = (CsrBtPacPullVcardListCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pbSize);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->newMissedCall);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAC_PULL_VCARD_LIST_CFM */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_VCARD_LIST_CFM
void *CsrBtPacPullVcardListCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPacPullVcardListCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPacPullVcardListCfm *) CsrPmemZalloc(sizeof(CsrBtPacPullVcardListCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pbSize, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->newMissedCall, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAC_PULL_VCARD_LIST_CFM */

#ifndef EXCLUDE_CSR_BT_PAC_SET_BACK_FOLDER_CFM
CsrSize CsrBtPacSetBackFolderCfmSizeof(void *msg)
{
    CsrBtPacSetBackFolderCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAC_SET_BACK_FOLDER_CFM */

#ifndef EXCLUDE_CSR_BT_PAC_SET_BACK_FOLDER_CFM
CsrUint8 *CsrBtPacSetBackFolderCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPacSetBackFolderCfm *primitive;

    primitive = (CsrBtPacSetBackFolderCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAC_SET_BACK_FOLDER_CFM */

#ifndef EXCLUDE_CSR_BT_PAC_SET_BACK_FOLDER_CFM
void *CsrBtPacSetBackFolderCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPacSetBackFolderCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPacSetBackFolderCfm *) CsrPmemZalloc(sizeof(CsrBtPacSetBackFolderCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAC_SET_BACK_FOLDER_CFM */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_PB_IND
CsrSize CsrBtPacPullPbIndSizeof(void *msg)
{
    CsrBtPacPullPbInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAC_PULL_PB_IND */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_PB_IND
CsrUint8 *CsrBtPacPullPbIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPacPullPbInd *primitive;

    primitive = (CsrBtPacPullPbInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyOffset);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAC_PULL_PB_IND */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_PB_IND
void *CsrBtPacPullPbIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPacPullPbInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPacPullPbInd *) CsrPmemZalloc(sizeof(CsrBtPacPullPbInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyOffset, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_PAC_PULL_PB_IND */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_PB_IND
void CsrBtPacPullPbIndSerFree(void *msg)
{
    CsrBtPacPullPbInd *primitive;

    primitive = (CsrBtPacPullPbInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_PAC_PULL_PB_IND */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_VCARD_LIST_RES
CsrSize CsrBtPacPullVcardListResSizeof(void *msg)
{
    CsrBtPacPullVcardListRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAC_PULL_VCARD_LIST_RES */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_VCARD_LIST_RES
CsrUint8 *CsrBtPacPullVcardListResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPacPullVcardListRes *primitive;

    primitive = (CsrBtPacPullVcardListRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAC_PULL_VCARD_LIST_RES */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_VCARD_LIST_RES
void *CsrBtPacPullVcardListResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPacPullVcardListRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPacPullVcardListRes *) CsrPmemZalloc(sizeof(CsrBtPacPullVcardListRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAC_PULL_VCARD_LIST_RES */

#ifndef EXCLUDE_CSR_BT_PAC_SECURITY_OUT_REQ
CsrSize CsrBtPacSecurityOutReqSizeof(void *msg)
{
    CsrBtPacSecurityOutReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAC_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_SECURITY_OUT_REQ
CsrUint8 *CsrBtPacSecurityOutReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPacSecurityOutReq *primitive;

    primitive = (CsrBtPacSecurityOutReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAC_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_SECURITY_OUT_REQ
void *CsrBtPacSecurityOutReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPacSecurityOutReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPacSecurityOutReq *) CsrPmemZalloc(sizeof(CsrBtPacSecurityOutReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAC_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_CONNECT_CFM
CsrSize CsrBtPacConnectCfmSizeof(void *msg)
{
    CsrBtPacConnectCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAC_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_PAC_CONNECT_CFM
CsrUint8 *CsrBtPacConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPacConnectCfm *primitive;

    primitive = (CsrBtPacConnectCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->obexPeerMaxPacketSize);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->supportedRepositories);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAC_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_PAC_CONNECT_CFM
void *CsrBtPacConnectCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPacConnectCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPacConnectCfm *) CsrPmemZalloc(sizeof(CsrBtPacConnectCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->obexPeerMaxPacketSize, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->supportedRepositories, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAC_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_VCARD_ENTRY_IND
CsrSize CsrBtPacPullVcardEntryIndSizeof(void *msg)
{
    CsrBtPacPullVcardEntryInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAC_PULL_VCARD_ENTRY_IND */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_VCARD_ENTRY_IND
CsrUint8 *CsrBtPacPullVcardEntryIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPacPullVcardEntryInd *primitive;

    primitive = (CsrBtPacPullVcardEntryInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyOffset);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAC_PULL_VCARD_ENTRY_IND */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_VCARD_ENTRY_IND
void *CsrBtPacPullVcardEntryIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPacPullVcardEntryInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPacPullVcardEntryInd *) CsrPmemZalloc(sizeof(CsrBtPacPullVcardEntryInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyOffset, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_PAC_PULL_VCARD_ENTRY_IND */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_VCARD_ENTRY_IND
void CsrBtPacPullVcardEntryIndSerFree(void *msg)
{
    CsrBtPacPullVcardEntryInd *primitive;

    primitive = (CsrBtPacPullVcardEntryInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_PAC_PULL_VCARD_ENTRY_IND */

#ifndef EXCLUDE_CSR_BT_PAC_CONNECT_REQ
CsrSize CsrBtPacConnectReqSizeof(void *msg)
{
    CsrBtPacConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->realmLength * sizeof(CsrUint8)) + (primitive->passwordLength * sizeof(CsrUint8)) + CsrCharStringSerLen((CsrCharString *) primitive->userId));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_CONNECT_REQ
CsrUint8 *CsrBtPacConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPacConnectReq *primitive;

    primitive = (CsrBtPacConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxPacketSize);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->destination.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->destination.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->destination.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->authorize);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->realmLength);
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
#endif /* EXCLUDE_CSR_BT_PAC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_CONNECT_REQ
void *CsrBtPacConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPacConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPacConnectReq *) CsrPmemZalloc(sizeof(CsrBtPacConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxPacketSize, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->destination.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->destination.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->destination.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->authorize, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->realmLength, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_PAC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_CONNECT_REQ
void CsrBtPacConnectReqSerFree(void *msg)
{
    CsrBtPacConnectReq *primitive;

    primitive = (CsrBtPacConnectReq *) msg;

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
#endif /* EXCLUDE_CSR_BT_PAC_CONNECT_REQ */

#endif /* EXCLUDE_CSR_BT_PAC_MODULE */
