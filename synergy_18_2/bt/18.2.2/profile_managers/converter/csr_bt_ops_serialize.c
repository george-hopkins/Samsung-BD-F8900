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
#include "csr_bt_ops_prim.h"

#ifndef EXCLUDE_CSR_BT_OPS_MODULE

void CsrBtOpsPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_OPS_DEACTIVATE_REQ
CsrSize CsrBtOpsDeactivateReqSizeof(void *msg)
{
    CsrBtOpsDeactivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_OPS_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_OPS_DEACTIVATE_REQ
CsrUint8 *CsrBtOpsDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtOpsDeactivateReq *primitive;

    primitive = (CsrBtOpsDeactivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_OPS_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_OPS_DEACTIVATE_REQ
void *CsrBtOpsDeactivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtOpsDeactivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtOpsDeactivateReq *) CsrPmemZalloc(sizeof(CsrBtOpsDeactivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_OPS_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_OPS_SECURITY_IN_REQ
CsrSize CsrBtOpsSecurityInReqSizeof(void *msg)
{
    CsrBtOpsSecurityInReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_OPS_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_OPS_SECURITY_IN_REQ
CsrUint8 *CsrBtOpsSecurityInReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtOpsSecurityInReq *primitive;

    primitive = (CsrBtOpsSecurityInReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_OPS_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_OPS_SECURITY_IN_REQ
void *CsrBtOpsSecurityInReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtOpsSecurityInReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtOpsSecurityInReq *) CsrPmemZalloc(sizeof(CsrBtOpsSecurityInReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_OPS_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_OPS_SECURITY_IN_CFM
CsrSize CsrBtOpsSecurityInCfmSizeof(void *msg)
{
    CsrBtOpsSecurityInCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_OPS_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_OPS_SECURITY_IN_CFM
CsrUint8 *CsrBtOpsSecurityInCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtOpsSecurityInCfm *primitive;

    primitive = (CsrBtOpsSecurityInCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_OPS_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_OPS_SECURITY_IN_CFM
void *CsrBtOpsSecurityInCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtOpsSecurityInCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtOpsSecurityInCfm *) CsrPmemZalloc(sizeof(CsrBtOpsSecurityInCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_OPS_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_OPS_ACTIVATE_REQ
CsrSize CsrBtOpsActivateReqSizeof(void *msg)
{
    CsrBtOpsActivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_OPS_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_OPS_ACTIVATE_REQ
CsrUint8 *CsrBtOpsActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtOpsActivateReq *primitive;

    primitive = (CsrBtOpsActivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->supportedFormats);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->obexMaxPacketSize);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->windowSize);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmEnable);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_OPS_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_OPS_ACTIVATE_REQ
void *CsrBtOpsActivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtOpsActivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtOpsActivateReq *) CsrPmemZalloc(sizeof(CsrBtOpsActivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->supportedFormats, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->obexMaxPacketSize, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->windowSize, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmEnable, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_OPS_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_OPS_GET_NEXT_RES
CsrSize CsrBtOpsGetNextResSizeof(void *msg)
{
    CsrBtOpsGetNextRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->bodyLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_OPS_GET_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_OPS_GET_NEXT_RES
CsrUint8 *CsrBtOpsGetNextResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtOpsGetNextRes *primitive;

    primitive = (CsrBtOpsGetNextRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bodyLength);
    CsrMemCpySer(buffer, length, &primitive->body, sizeof(primitive->body));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    if(primitive->body)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->body), ((CsrSize) (primitive->bodyLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_OPS_GET_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_OPS_GET_NEXT_RES
void *CsrBtOpsGetNextResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtOpsGetNextRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtOpsGetNextRes *) CsrPmemZalloc(sizeof(CsrBtOpsGetNextRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bodyLength, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_OPS_GET_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_OPS_GET_NEXT_RES
void CsrBtOpsGetNextResSerFree(void *msg)
{
    CsrBtOpsGetNextRes *primitive;

    primitive = (CsrBtOpsGetNextRes *) msg;

    if(primitive->body != NULL)
    {
        CsrPmemFree(primitive->body);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_OPS_GET_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_OPS_GET_IND
CsrSize CsrBtOpsGetIndSizeof(void *msg)
{
    CsrBtOpsGetInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_OPS_GET_IND */

#ifndef EXCLUDE_CSR_BT_OPS_GET_IND
CsrUint8 *CsrBtOpsGetIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtOpsGetInd *primitive;

    primitive = (CsrBtOpsGetInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bodyType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_OPS_GET_IND */

#ifndef EXCLUDE_CSR_BT_OPS_GET_IND
void *CsrBtOpsGetIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtOpsGetInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtOpsGetInd *) CsrPmemZalloc(sizeof(CsrBtOpsGetInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bodyType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_OPS_GET_IND */

#ifndef EXCLUDE_CSR_BT_OPS_AUTHENTICATE_RES
CsrSize CsrBtOpsAuthenticateResSizeof(void *msg)
{
    CsrBtOpsAuthenticateRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->passwordLength * sizeof(CsrUint8)) + CsrCharStringSerLen((CsrCharString *) primitive->userId));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_OPS_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_OPS_AUTHENTICATE_RES
CsrUint8 *CsrBtOpsAuthenticateResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtOpsAuthenticateRes *primitive;

    primitive = (CsrBtOpsAuthenticateRes *) msg;
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
#endif /* EXCLUDE_CSR_BT_OPS_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_OPS_AUTHENTICATE_RES
void *CsrBtOpsAuthenticateResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtOpsAuthenticateRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtOpsAuthenticateRes *) CsrPmemZalloc(sizeof(CsrBtOpsAuthenticateRes));

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
#endif /* EXCLUDE_CSR_BT_OPS_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_OPS_AUTHENTICATE_RES
void CsrBtOpsAuthenticateResSerFree(void *msg)
{
    CsrBtOpsAuthenticateRes *primitive;

    primitive = (CsrBtOpsAuthenticateRes *) msg;

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
#endif /* EXCLUDE_CSR_BT_OPS_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_OPS_GET_NEXT_IND
CsrSize CsrBtOpsGetNextIndSizeof(void *msg)
{
    CsrBtOpsGetNextInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_OPS_GET_NEXT_IND */

#ifndef EXCLUDE_CSR_BT_OPS_GET_NEXT_IND
CsrUint8 *CsrBtOpsGetNextIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtOpsGetNextInd *primitive;

    primitive = (CsrBtOpsGetNextInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bodyType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_OPS_GET_NEXT_IND */

#ifndef EXCLUDE_CSR_BT_OPS_GET_NEXT_IND
void *CsrBtOpsGetNextIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtOpsGetNextInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtOpsGetNextInd *) CsrPmemZalloc(sizeof(CsrBtOpsGetNextInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bodyType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_OPS_GET_NEXT_IND */

#ifndef EXCLUDE_CSR_BT_OPS_DEACTIVATE_CFM
CsrSize CsrBtOpsDeactivateCfmSizeof(void *msg)
{
    CsrBtOpsDeactivateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_OPS_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_OPS_DEACTIVATE_CFM
CsrUint8 *CsrBtOpsDeactivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtOpsDeactivateCfm *primitive;

    primitive = (CsrBtOpsDeactivateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_OPS_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_OPS_DEACTIVATE_CFM
void *CsrBtOpsDeactivateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtOpsDeactivateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtOpsDeactivateCfm *) CsrPmemZalloc(sizeof(CsrBtOpsDeactivateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_OPS_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_OPS_ABORT_IND
CsrSize CsrBtOpsAbortIndSizeof(void *msg)
{
    CsrBtOpsAbortInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_OPS_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_OPS_ABORT_IND
CsrUint8 *CsrBtOpsAbortIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtOpsAbortInd *primitive;

    primitive = (CsrBtOpsAbortInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->descriptionOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->descriptionLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_OPS_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_OPS_ABORT_IND
void *CsrBtOpsAbortIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtOpsAbortInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtOpsAbortInd *) CsrPmemZalloc(sizeof(CsrBtOpsAbortInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->descriptionOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->descriptionLength, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_OPS_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_OPS_ABORT_IND
void CsrBtOpsAbortIndSerFree(void *msg)
{
    CsrBtOpsAbortInd *primitive;

    primitive = (CsrBtOpsAbortInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_OPS_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_OPS_GET_RES
CsrSize CsrBtOpsGetResSizeof(void *msg)
{
    CsrBtOpsGetRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrConverterUcs2ByteStrLen((CsrUtf8String*) primitive->ucs2name) + 2 + (primitive->bodyLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_OPS_GET_RES */

#ifndef EXCLUDE_CSR_BT_OPS_GET_RES
CsrUint8 *CsrBtOpsGetResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtOpsGetRes *primitive;

    primitive = (CsrBtOpsGetRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bodyType);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->totalObjectSize);
    CsrMemCpySer(buffer, length, &primitive->ucs2name, sizeof(primitive->ucs2name));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyLength);
    CsrMemCpySer(buffer, length, &primitive->body, sizeof(primitive->body));
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
    if(primitive->body)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->body), ((CsrSize) (primitive->bodyLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_OPS_GET_RES */

#ifndef EXCLUDE_CSR_BT_OPS_GET_RES
void *CsrBtOpsGetResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtOpsGetRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtOpsGetRes *) CsrPmemZalloc(sizeof(CsrBtOpsGetRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bodyType, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->totalObjectSize, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->ucs2name), buffer, &offset, ((CsrSize) sizeof(primitive->ucs2name)));
    CsrUint16Des((CsrUint16 *) &primitive->bodyLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->body), buffer, &offset, ((CsrSize) sizeof(primitive->body)));
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    primitive->ucs2name = CsrPmemZalloc(CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    CsrMemCpy(((CsrUint8*) primitive->ucs2name), ((CsrUint8*) &buffer[offset]),CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    offset += ((CsrSize) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->ucs2name) + 2));
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
#endif /* EXCLUDE_CSR_BT_OPS_GET_RES */

#ifndef EXCLUDE_CSR_BT_OPS_GET_RES
void CsrBtOpsGetResSerFree(void *msg)
{
    CsrBtOpsGetRes *primitive;

    primitive = (CsrBtOpsGetRes *) msg;

    if(primitive->ucs2name != NULL)
    {
        CsrPmemFree(primitive->ucs2name);
    }
    if(primitive->body != NULL)
    {
        CsrPmemFree(primitive->body);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_OPS_GET_RES */

#ifndef EXCLUDE_CSR_BT_OPS_PUT_IND
CsrSize CsrBtOpsPutIndSizeof(void *msg)
{
    CsrBtOpsPutInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_OPS_PUT_IND */

#ifndef EXCLUDE_CSR_BT_OPS_PUT_IND
CsrUint8 *CsrBtOpsPutIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtOpsPutInd *primitive;

    primitive = (CsrBtOpsPutInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->totalObjectSize);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyTypeOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyTypeLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ucs2nameOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyOffset);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bodyLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_OPS_PUT_IND */

#ifndef EXCLUDE_CSR_BT_OPS_PUT_IND
void *CsrBtOpsPutIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtOpsPutInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtOpsPutInd *) CsrPmemZalloc(sizeof(CsrBtOpsPutInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->totalObjectSize, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyTypeOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyTypeLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->ucs2nameOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyOffset, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bodyLength, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_OPS_PUT_IND */

#ifndef EXCLUDE_CSR_BT_OPS_PUT_IND
void CsrBtOpsPutIndSerFree(void *msg)
{
    CsrBtOpsPutInd *primitive;

    primitive = (CsrBtOpsPutInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_OPS_PUT_IND */

#ifndef EXCLUDE_CSR_BT_OPS_CONNECT_RES
CsrSize CsrBtOpsConnectResSizeof(void *msg)
{
    CsrBtOpsConnectRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_OPS_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_OPS_CONNECT_RES
CsrUint8 *CsrBtOpsConnectResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtOpsConnectRes *primitive;

    primitive = (CsrBtOpsConnectRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_OPS_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_OPS_CONNECT_RES
void *CsrBtOpsConnectResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtOpsConnectRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtOpsConnectRes *) CsrPmemZalloc(sizeof(CsrBtOpsConnectRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_OPS_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_OPS_DISCONNECT_IND
CsrSize CsrBtOpsDisconnectIndSizeof(void *msg)
{
    CsrBtOpsDisconnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_OPS_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_OPS_DISCONNECT_IND
CsrUint8 *CsrBtOpsDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtOpsDisconnectInd *primitive;

    primitive = (CsrBtOpsDisconnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_OPS_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_OPS_DISCONNECT_IND
void *CsrBtOpsDisconnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtOpsDisconnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtOpsDisconnectInd *) CsrPmemZalloc(sizeof(CsrBtOpsDisconnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_OPS_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_OPS_PUT_NEXT_RES
CsrSize CsrBtOpsPutNextResSizeof(void *msg)
{
    CsrBtOpsPutNextRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_OPS_PUT_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_OPS_PUT_NEXT_RES
CsrUint8 *CsrBtOpsPutNextResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtOpsPutNextRes *primitive;

    primitive = (CsrBtOpsPutNextRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_OPS_PUT_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_OPS_PUT_NEXT_RES
void *CsrBtOpsPutNextResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtOpsPutNextRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtOpsPutNextRes *) CsrPmemZalloc(sizeof(CsrBtOpsPutNextRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_OPS_PUT_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_OPS_AUTHENTICATE_IND
CsrSize CsrBtOpsAuthenticateIndSizeof(void *msg)
{
    CsrBtOpsAuthenticateInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->realmLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_OPS_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_OPS_AUTHENTICATE_IND
CsrUint8 *CsrBtOpsAuthenticateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtOpsAuthenticateInd *primitive;

    primitive = (CsrBtOpsAuthenticateInd *) msg;
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
#endif /* EXCLUDE_CSR_BT_OPS_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_OPS_AUTHENTICATE_IND
void *CsrBtOpsAuthenticateIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtOpsAuthenticateInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtOpsAuthenticateInd *) CsrPmemZalloc(sizeof(CsrBtOpsAuthenticateInd));

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
#endif /* EXCLUDE_CSR_BT_OPS_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_OPS_AUTHENTICATE_IND
void CsrBtOpsAuthenticateIndSerFree(void *msg)
{
    CsrBtOpsAuthenticateInd *primitive;

    primitive = (CsrBtOpsAuthenticateInd *) msg;

    if(primitive->realm != NULL)
    {
        CsrPmemFree(primitive->realm);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_OPS_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_OPS_PUT_RES
CsrSize CsrBtOpsPutResSizeof(void *msg)
{
    CsrBtOpsPutRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_OPS_PUT_RES */

#ifndef EXCLUDE_CSR_BT_OPS_PUT_RES
CsrUint8 *CsrBtOpsPutResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtOpsPutRes *primitive;

    primitive = (CsrBtOpsPutRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_OPS_PUT_RES */

#ifndef EXCLUDE_CSR_BT_OPS_PUT_RES
void *CsrBtOpsPutResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtOpsPutRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtOpsPutRes *) CsrPmemZalloc(sizeof(CsrBtOpsPutRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_OPS_PUT_RES */

#ifndef EXCLUDE_CSR_BT_OPS_CONNECT_IND
CsrSize CsrBtOpsConnectIndSizeof(void *msg)
{
    CsrBtOpsConnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_OPS_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_OPS_CONNECT_IND
CsrUint8 *CsrBtOpsConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtOpsConnectInd *primitive;

    primitive = (CsrBtOpsConnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->obexPeerMaxPacketSize);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->length);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->count);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_OPS_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_OPS_CONNECT_IND
void *CsrBtOpsConnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtOpsConnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtOpsConnectInd *) CsrPmemZalloc(sizeof(CsrBtOpsConnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->obexPeerMaxPacketSize, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->length, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->count, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_OPS_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_OPS_PUT_NEXT_IND
CsrSize CsrBtOpsPutNextIndSizeof(void *msg)
{
    CsrBtOpsPutNextInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_OPS_PUT_NEXT_IND */

#ifndef EXCLUDE_CSR_BT_OPS_PUT_NEXT_IND
CsrUint8 *CsrBtOpsPutNextIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtOpsPutNextInd *primitive;

    primitive = (CsrBtOpsPutNextInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyTypeOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyTypeLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ucs2nameOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyOffset);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bodyLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_OPS_PUT_NEXT_IND */

#ifndef EXCLUDE_CSR_BT_OPS_PUT_NEXT_IND
void *CsrBtOpsPutNextIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtOpsPutNextInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtOpsPutNextInd *) CsrPmemZalloc(sizeof(CsrBtOpsPutNextInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyTypeOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyTypeLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->ucs2nameOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyOffset, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bodyLength, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_OPS_PUT_NEXT_IND */

#ifndef EXCLUDE_CSR_BT_OPS_PUT_NEXT_IND
void CsrBtOpsPutNextIndSerFree(void *msg)
{
    CsrBtOpsPutNextInd *primitive;

    primitive = (CsrBtOpsPutNextInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_OPS_PUT_NEXT_IND */

#endif /* EXCLUDE_CSR_BT_OPS_MODULE */
