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
#include "csr_bt_pas_prim.h"

#ifndef EXCLUDE_CSR_BT_PAS_MODULE

void CsrBtPasPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_MCH_LIST_IND
CsrSize CsrBtPasPullVcardMchListIndSizeof(void *msg)
{
    CsrBtPasPullVcardMchListInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_MCH_LIST_IND */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_MCH_LIST_IND
CsrUint8 *CsrBtPasPullVcardMchListIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPasPullVcardMchListInd *primitive;

    primitive = (CsrBtPasPullVcardMchListInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->order);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->uft8SearchValOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->searchValLength);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->searchAtt);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxListCnt);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->listStartOffset);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_MCH_LIST_IND */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_MCH_LIST_IND
void *CsrBtPasPullVcardMchListIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPasPullVcardMchListInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPasPullVcardMchListInd *) CsrPmemZalloc(sizeof(CsrBtPasPullVcardMchListInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->order, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->uft8SearchValOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->searchValLength, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->searchAtt, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxListCnt, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->listStartOffset, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_MCH_LIST_IND */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_MCH_LIST_IND
void CsrBtPasPullVcardMchListIndSerFree(void *msg)
{
    CsrBtPasPullVcardMchListInd *primitive;

    primitive = (CsrBtPasPullVcardMchListInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_MCH_LIST_IND */

#ifndef EXCLUDE_CSR_BT_PAS_SECURITY_IN_REQ
CsrSize CsrBtPasSecurityInReqSizeof(void *msg)
{
    CsrBtPasSecurityInReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAS_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_PAS_SECURITY_IN_REQ
CsrUint8 *CsrBtPasSecurityInReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPasSecurityInReq *primitive;

    primitive = (CsrBtPasSecurityInReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAS_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_PAS_SECURITY_IN_REQ
void *CsrBtPasSecurityInReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPasSecurityInReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPasSecurityInReq *) CsrPmemZalloc(sizeof(CsrBtPasSecurityInReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAS_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_PAS_SET_FOLDER_IND
CsrSize CsrBtPasSetFolderIndSizeof(void *msg)
{
    CsrBtPasSetFolderInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAS_SET_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_PAS_SET_FOLDER_IND
CsrUint8 *CsrBtPasSetFolderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPasSetFolderInd *primitive;

    primitive = (CsrBtPasSetFolderInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ucs2nameOffset);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAS_SET_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_PAS_SET_FOLDER_IND
void *CsrBtPasSetFolderIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPasSetFolderInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPasSetFolderInd *) CsrPmemZalloc(sizeof(CsrBtPasSetFolderInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_PAS_SET_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_PAS_SET_FOLDER_IND
void CsrBtPasSetFolderIndSerFree(void *msg)
{
    CsrBtPasSetFolderInd *primitive;

    primitive = (CsrBtPasSetFolderInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_PAS_SET_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_PB_RES
CsrSize CsrBtPasPullPbResSizeof(void *msg)
{
    CsrBtPasPullPbRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_PB_RES */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_PB_RES
CsrUint8 *CsrBtPasPullPbResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPasPullPbRes *primitive;

    primitive = (CsrBtPasPullPbRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_PB_RES */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_PB_RES
void *CsrBtPasPullPbResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPasPullPbRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPasPullPbRes *) CsrPmemZalloc(sizeof(CsrBtPasPullPbRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_PB_RES */

#ifndef EXCLUDE_CSR_BT_PAS_AUTHENTICATE_REQ
CsrSize CsrBtPasAuthenticateReqSizeof(void *msg)
{
    CsrBtPasAuthenticateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->realmLength * sizeof(CsrUint8)) + (primitive->passwordLength * sizeof(CsrUint8)) + CsrCharStringSerLen((CsrCharString *) primitive->userId));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAS_AUTHENTICATE_REQ */

#ifndef EXCLUDE_CSR_BT_PAS_AUTHENTICATE_REQ
CsrUint8 *CsrBtPasAuthenticateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPasAuthenticateReq *primitive;

    primitive = (CsrBtPasAuthenticateReq *) msg;
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
#endif /* EXCLUDE_CSR_BT_PAS_AUTHENTICATE_REQ */

#ifndef EXCLUDE_CSR_BT_PAS_AUTHENTICATE_REQ
void *CsrBtPasAuthenticateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPasAuthenticateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPasAuthenticateReq *) CsrPmemZalloc(sizeof(CsrBtPasAuthenticateReq));

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
#endif /* EXCLUDE_CSR_BT_PAS_AUTHENTICATE_REQ */

#ifndef EXCLUDE_CSR_BT_PAS_AUTHENTICATE_REQ
void CsrBtPasAuthenticateReqSerFree(void *msg)
{
    CsrBtPasAuthenticateReq *primitive;

    primitive = (CsrBtPasAuthenticateReq *) msg;

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
#endif /* EXCLUDE_CSR_BT_PAS_AUTHENTICATE_REQ */

#ifndef EXCLUDE_CSR_BT_PAS_DEACTIVATE_REQ
CsrSize CsrBtPasDeactivateReqSizeof(void *msg)
{
    CsrBtPasDeactivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAS_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_PAS_DEACTIVATE_REQ
CsrUint8 *CsrBtPasDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPasDeactivateReq *primitive;

    primitive = (CsrBtPasDeactivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAS_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_PAS_DEACTIVATE_REQ
void *CsrBtPasDeactivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPasDeactivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPasDeactivateReq *) CsrPmemZalloc(sizeof(CsrBtPasDeactivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAS_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_PAS_AUTHENTICATE_IND
CsrSize CsrBtPasAuthenticateIndSizeof(void *msg)
{
    CsrBtPasAuthenticateInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->realmLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAS_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_PAS_AUTHENTICATE_IND
CsrUint8 *CsrBtPasAuthenticateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPasAuthenticateInd *primitive;

    primitive = (CsrBtPasAuthenticateInd *) msg;
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
#endif /* EXCLUDE_CSR_BT_PAS_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_PAS_AUTHENTICATE_IND
void *CsrBtPasAuthenticateIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPasAuthenticateInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPasAuthenticateInd *) CsrPmemZalloc(sizeof(CsrBtPasAuthenticateInd));

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
#endif /* EXCLUDE_CSR_BT_PAS_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_PAS_AUTHENTICATE_IND
void CsrBtPasAuthenticateIndSerFree(void *msg)
{
    CsrBtPasAuthenticateInd *primitive;

    primitive = (CsrBtPasAuthenticateInd *) msg;

    if(primitive->realm != NULL)
    {
        CsrPmemFree(primitive->realm);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_PAS_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_PB_SIZE_IND
CsrSize CsrBtPasPullPbSizeIndSizeof(void *msg)
{
    CsrBtPasPullPbSizeInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_PB_SIZE_IND */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_PB_SIZE_IND
CsrUint8 *CsrBtPasPullPbSizeIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPasPullPbSizeInd *primitive;

    primitive = (CsrBtPasPullPbSizeInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ucs2nameOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->src);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_PB_SIZE_IND */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_PB_SIZE_IND
void *CsrBtPasPullPbSizeIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPasPullPbSizeInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPasPullPbSizeInd *) CsrPmemZalloc(sizeof(CsrBtPasPullPbSizeInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->ucs2nameOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->src, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_PAS_PULL_PB_SIZE_IND */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_PB_SIZE_IND
void CsrBtPasPullPbSizeIndSerFree(void *msg)
{
    CsrBtPasPullPbSizeInd *primitive;

    primitive = (CsrBtPasPullPbSizeInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_PB_SIZE_IND */

#ifndef EXCLUDE_CSR_BT_PAS_SECURITY_IN_CFM
CsrSize CsrBtPasSecurityInCfmSizeof(void *msg)
{
    CsrBtPasSecurityInCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAS_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_PAS_SECURITY_IN_CFM
CsrUint8 *CsrBtPasSecurityInCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPasSecurityInCfm *primitive;

    primitive = (CsrBtPasSecurityInCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAS_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_PAS_SECURITY_IN_CFM
void *CsrBtPasSecurityInCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPasSecurityInCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPasSecurityInCfm *) CsrPmemZalloc(sizeof(CsrBtPasSecurityInCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAS_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_PAS_CONNECT_IND
CsrSize CsrBtPasConnectIndSizeof(void *msg)
{
    CsrBtPasConnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAS_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_PAS_CONNECT_IND
CsrUint8 *CsrBtPasConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPasConnectInd *primitive;

    primitive = (CsrBtPasConnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->obexPeerMaxPacketSize);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->length);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->count);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAS_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_PAS_CONNECT_IND
void *CsrBtPasConnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPasConnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPasConnectInd *) CsrPmemZalloc(sizeof(CsrBtPasConnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->obexPeerMaxPacketSize, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->length, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->count, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAS_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_PAS_AUTHENTICATE_CFM
CsrSize CsrBtPasAuthenticateCfmSizeof(void *msg)
{
    CsrBtPasAuthenticateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAS_AUTHENTICATE_CFM */

#ifndef EXCLUDE_CSR_BT_PAS_AUTHENTICATE_CFM
CsrUint8 *CsrBtPasAuthenticateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPasAuthenticateCfm *primitive;

    primitive = (CsrBtPasAuthenticateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAS_AUTHENTICATE_CFM */

#ifndef EXCLUDE_CSR_BT_PAS_AUTHENTICATE_CFM
void *CsrBtPasAuthenticateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPasAuthenticateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPasAuthenticateCfm *) CsrPmemZalloc(sizeof(CsrBtPasAuthenticateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAS_AUTHENTICATE_CFM */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_SIZE_RES
CsrSize CsrBtPasPullVcardListSizeResSizeof(void *msg)
{
    CsrBtPasPullVcardListSizeRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_SIZE_RES */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_SIZE_RES
CsrUint8 *CsrBtPasPullVcardListSizeResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPasPullVcardListSizeRes *primitive;

    primitive = (CsrBtPasPullVcardListSizeRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pbSize);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_SIZE_RES */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_SIZE_RES
void *CsrBtPasPullVcardListSizeResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPasPullVcardListSizeRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPasPullVcardListSizeRes *) CsrPmemZalloc(sizeof(CsrBtPasPullVcardListSizeRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pbSize, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_SIZE_RES */

#ifndef EXCLUDE_CSR_BT_PAS_CONNECT_RES
CsrSize CsrBtPasConnectResSizeof(void *msg)
{
    CsrBtPasConnectRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAS_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_PAS_CONNECT_RES
CsrUint8 *CsrBtPasConnectResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPasConnectRes *primitive;

    primitive = (CsrBtPasConnectRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAS_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_PAS_CONNECT_RES
void *CsrBtPasConnectResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPasConnectRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPasConnectRes *) CsrPmemZalloc(sizeof(CsrBtPasConnectRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAS_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_ENTRY_RES
CsrSize CsrBtPasPullVcardEntryResSizeof(void *msg)
{
    CsrBtPasPullVcardEntryRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_ENTRY_RES */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_ENTRY_RES
CsrUint8 *CsrBtPasPullVcardEntryResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPasPullVcardEntryRes *primitive;

    primitive = (CsrBtPasPullVcardEntryRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_ENTRY_RES */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_ENTRY_RES
void *CsrBtPasPullVcardEntryResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPasPullVcardEntryRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPasPullVcardEntryRes *) CsrPmemZalloc(sizeof(CsrBtPasPullVcardEntryRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_ENTRY_RES */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_MCH_SIZE_RES
CsrSize CsrBtPasPullMchSizeResSizeof(void *msg)
{
    CsrBtPasPullMchSizeRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_MCH_SIZE_RES */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_MCH_SIZE_RES
CsrUint8 *CsrBtPasPullMchSizeResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPasPullMchSizeRes *primitive;

    primitive = (CsrBtPasPullMchSizeRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pbSize);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->newMissedCall);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_MCH_SIZE_RES */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_MCH_SIZE_RES
void *CsrBtPasPullMchSizeResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPasPullMchSizeRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPasPullMchSizeRes *) CsrPmemZalloc(sizeof(CsrBtPasPullMchSizeRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pbSize, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->newMissedCall, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_MCH_SIZE_RES */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_MCH_SIZE_IND
CsrSize CsrBtPasPullMchSizeIndSizeof(void *msg)
{
    CsrBtPasPullMchSizeInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_MCH_SIZE_IND */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_MCH_SIZE_IND
CsrUint8 *CsrBtPasPullMchSizeIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPasPullMchSizeInd *primitive;

    primitive = (CsrBtPasPullMchSizeInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->src);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_MCH_SIZE_IND */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_MCH_SIZE_IND
void *CsrBtPasPullMchSizeIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPasPullMchSizeInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPasPullMchSizeInd *) CsrPmemZalloc(sizeof(CsrBtPasPullMchSizeInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->src, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_MCH_SIZE_IND */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_MCH_IND
CsrSize CsrBtPasPullMchIndSizeof(void *msg)
{
    CsrBtPasPullMchInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_MCH_IND */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_MCH_IND
CsrUint8 *CsrBtPasPullMchIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPasPullMchInd *primitive;

    primitive = (CsrBtPasPullMchInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrMemCpySer(buffer, length, ((const void *) primitive->filter), ((CsrSize) (8)));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->src);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->format);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxListCnt);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->listStartOffset);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_MCH_IND */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_MCH_IND
void *CsrBtPasPullMchIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPasPullMchInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPasPullMchInd *) CsrPmemZalloc(sizeof(CsrBtPasPullMchInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->filter), buffer, &offset, ((CsrSize) (8)));
    CsrUint16Des((CsrUint16 *) &primitive->src, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->format, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxListCnt, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->listStartOffset, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_MCH_IND */

#ifndef EXCLUDE_CSR_BT_PAS_DEACTIVATE_CFM
CsrSize CsrBtPasDeactivateCfmSizeof(void *msg)
{
    CsrBtPasDeactivateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAS_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_PAS_DEACTIVATE_CFM
CsrUint8 *CsrBtPasDeactivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPasDeactivateCfm *primitive;

    primitive = (CsrBtPasDeactivateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAS_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_PAS_DEACTIVATE_CFM
void *CsrBtPasDeactivateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPasDeactivateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPasDeactivateCfm *) CsrPmemZalloc(sizeof(CsrBtPasDeactivateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAS_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_IND
CsrSize CsrBtPasPullVcardListIndSizeof(void *msg)
{
    CsrBtPasPullVcardListInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_IND */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_IND
CsrUint8 *CsrBtPasPullVcardListIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPasPullVcardListInd *primitive;

    primitive = (CsrBtPasPullVcardListInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ucs2nameOffset);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->order);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->uft8SearchValOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->searchValLength);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->searchAtt);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxListCnt);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->listStartOffset);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_IND */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_IND
void *CsrBtPasPullVcardListIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPasPullVcardListInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPasPullVcardListInd *) CsrPmemZalloc(sizeof(CsrBtPasPullVcardListInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->ucs2nameOffset, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->order, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->uft8SearchValOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->searchValLength, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->searchAtt, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxListCnt, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->listStartOffset, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_IND */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_IND
void CsrBtPasPullVcardListIndSerFree(void *msg)
{
    CsrBtPasPullVcardListInd *primitive;

    primitive = (CsrBtPasPullVcardListInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_IND */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_PB_SIZE_RES
CsrSize CsrBtPasPullPbSizeResSizeof(void *msg)
{
    CsrBtPasPullPbSizeRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_PB_SIZE_RES */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_PB_SIZE_RES
CsrUint8 *CsrBtPasPullPbSizeResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPasPullPbSizeRes *primitive;

    primitive = (CsrBtPasPullPbSizeRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pbSize);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_PB_SIZE_RES */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_PB_SIZE_RES
void *CsrBtPasPullPbSizeResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPasPullPbSizeRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPasPullPbSizeRes *) CsrPmemZalloc(sizeof(CsrBtPasPullPbSizeRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pbSize, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_PB_SIZE_RES */

#ifndef EXCLUDE_CSR_BT_PAS_SET_FOLDER_RES
CsrSize CsrBtPasSetFolderResSizeof(void *msg)
{
    CsrBtPasSetFolderRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAS_SET_FOLDER_RES */

#ifndef EXCLUDE_CSR_BT_PAS_SET_FOLDER_RES
CsrUint8 *CsrBtPasSetFolderResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPasSetFolderRes *primitive;

    primitive = (CsrBtPasSetFolderRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAS_SET_FOLDER_RES */

#ifndef EXCLUDE_CSR_BT_PAS_SET_FOLDER_RES
void *CsrBtPasSetFolderResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPasSetFolderRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPasSetFolderRes *) CsrPmemZalloc(sizeof(CsrBtPasSetFolderRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAS_SET_FOLDER_RES */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_ENTRY_IND
CsrSize CsrBtPasPullVcardEntryIndSizeof(void *msg)
{
    CsrBtPasPullVcardEntryInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_ENTRY_IND */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_ENTRY_IND
CsrUint8 *CsrBtPasPullVcardEntryIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPasPullVcardEntryInd *primitive;

    primitive = (CsrBtPasPullVcardEntryInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ucs2nameOffset);
    CsrMemCpySer(buffer, length, ((const void *) primitive->filter), ((CsrSize) (8)));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->format);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_ENTRY_IND */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_ENTRY_IND
void *CsrBtPasPullVcardEntryIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPasPullVcardEntryInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPasPullVcardEntryInd *) CsrPmemZalloc(sizeof(CsrBtPasPullVcardEntryInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->ucs2nameOffset, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->filter), buffer, &offset, ((CsrSize) (8)));
    CsrUint8Des((CsrUint8 *) &primitive->format, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_ENTRY_IND */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_ENTRY_IND
void CsrBtPasPullVcardEntryIndSerFree(void *msg)
{
    CsrBtPasPullVcardEntryInd *primitive;

    primitive = (CsrBtPasPullVcardEntryInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_ENTRY_IND */

#ifndef EXCLUDE_CSR_BT_PAS_AUTHENTICATE_RES
CsrSize CsrBtPasAuthenticateResSizeof(void *msg)
{
    CsrBtPasAuthenticateRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->passwordLength * sizeof(CsrUint8)) + CsrCharStringSerLen((CsrCharString *) primitive->userId));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAS_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_PAS_AUTHENTICATE_RES
CsrUint8 *CsrBtPasAuthenticateResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPasAuthenticateRes *primitive;

    primitive = (CsrBtPasAuthenticateRes *) msg;
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
#endif /* EXCLUDE_CSR_BT_PAS_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_PAS_AUTHENTICATE_RES
void *CsrBtPasAuthenticateResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPasAuthenticateRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPasAuthenticateRes *) CsrPmemZalloc(sizeof(CsrBtPasAuthenticateRes));

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
#endif /* EXCLUDE_CSR_BT_PAS_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_PAS_AUTHENTICATE_RES
void CsrBtPasAuthenticateResSerFree(void *msg)
{
    CsrBtPasAuthenticateRes *primitive;

    primitive = (CsrBtPasAuthenticateRes *) msg;

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
#endif /* EXCLUDE_CSR_BT_PAS_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_MCH_RES
CsrSize CsrBtPasPullMchResSizeof(void *msg)
{
    CsrBtPasPullMchRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_MCH_RES */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_MCH_RES
CsrUint8 *CsrBtPasPullMchResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPasPullMchRes *primitive;

    primitive = (CsrBtPasPullMchRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->newMissedCall);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_MCH_RES */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_MCH_RES
void *CsrBtPasPullMchResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPasPullMchRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPasPullMchRes *) CsrPmemZalloc(sizeof(CsrBtPasPullMchRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->newMissedCall, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_MCH_RES */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_MCH_LIST_RES
CsrSize CsrBtPasPullVcardMchListResSizeof(void *msg)
{
    CsrBtPasPullVcardMchListRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_MCH_LIST_RES */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_MCH_LIST_RES
CsrUint8 *CsrBtPasPullVcardMchListResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPasPullVcardMchListRes *primitive;

    primitive = (CsrBtPasPullVcardMchListRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->newMissedCall);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_MCH_LIST_RES */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_MCH_LIST_RES
void *CsrBtPasPullVcardMchListResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPasPullVcardMchListRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPasPullVcardMchListRes *) CsrPmemZalloc(sizeof(CsrBtPasPullVcardMchListRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->newMissedCall, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_MCH_LIST_RES */

#ifndef EXCLUDE_CSR_BT_PAS_SET_ROOT_FOLDER_IND
CsrSize CsrBtPasSetRootFolderIndSizeof(void *msg)
{
    CsrBtPasSetRootFolderInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAS_SET_ROOT_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_PAS_SET_ROOT_FOLDER_IND
CsrUint8 *CsrBtPasSetRootFolderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPasSetRootFolderInd *primitive;

    primitive = (CsrBtPasSetRootFolderInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAS_SET_ROOT_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_PAS_SET_ROOT_FOLDER_IND
void *CsrBtPasSetRootFolderIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPasSetRootFolderInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPasSetRootFolderInd *) CsrPmemZalloc(sizeof(CsrBtPasSetRootFolderInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAS_SET_ROOT_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_RES
CsrSize CsrBtPasPullVcardListResSizeof(void *msg)
{
    CsrBtPasPullVcardListRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_RES */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_RES
CsrUint8 *CsrBtPasPullVcardListResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPasPullVcardListRes *primitive;

    primitive = (CsrBtPasPullVcardListRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_RES */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_RES
void *CsrBtPasPullVcardListResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPasPullVcardListRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPasPullVcardListRes *) CsrPmemZalloc(sizeof(CsrBtPasPullVcardListRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_RES */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_MCH_LIST_SIZE_RES
CsrSize CsrBtPasPullVcardMchListSizeResSizeof(void *msg)
{
    CsrBtPasPullVcardMchListSizeRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_MCH_LIST_SIZE_RES */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_MCH_LIST_SIZE_RES
CsrUint8 *CsrBtPasPullVcardMchListSizeResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPasPullVcardMchListSizeRes *primitive;

    primitive = (CsrBtPasPullVcardMchListSizeRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pbSize);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->newMissedCall);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_MCH_LIST_SIZE_RES */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_MCH_LIST_SIZE_RES
void *CsrBtPasPullVcardMchListSizeResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPasPullVcardMchListSizeRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPasPullVcardMchListSizeRes *) CsrPmemZalloc(sizeof(CsrBtPasPullVcardMchListSizeRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pbSize, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->newMissedCall, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_MCH_LIST_SIZE_RES */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_PB_IND
CsrSize CsrBtPasPullPbIndSizeof(void *msg)
{
    CsrBtPasPullPbInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_PB_IND */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_PB_IND
CsrUint8 *CsrBtPasPullPbIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPasPullPbInd *primitive;

    primitive = (CsrBtPasPullPbInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ucs2nameOffset);
    CsrMemCpySer(buffer, length, ((const void *) primitive->filter), ((CsrSize) (8)));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->src);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->format);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxListCnt);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->listStartOffset);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_PB_IND */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_PB_IND
void *CsrBtPasPullPbIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPasPullPbInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPasPullPbInd *) CsrPmemZalloc(sizeof(CsrBtPasPullPbInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->ucs2nameOffset, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->filter), buffer, &offset, ((CsrSize) (8)));
    CsrUint16Des((CsrUint16 *) &primitive->src, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->format, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxListCnt, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->listStartOffset, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_PAS_PULL_PB_IND */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_PB_IND
void CsrBtPasPullPbIndSerFree(void *msg)
{
    CsrBtPasPullPbInd *primitive;

    primitive = (CsrBtPasPullPbInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_PB_IND */

#ifndef EXCLUDE_CSR_BT_PAS_DISCONNECT_IND
CsrSize CsrBtPasDisconnectIndSizeof(void *msg)
{
    CsrBtPasDisconnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAS_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_PAS_DISCONNECT_IND
CsrUint8 *CsrBtPasDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPasDisconnectInd *primitive;

    primitive = (CsrBtPasDisconnectInd *) msg;
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
#endif /* EXCLUDE_CSR_BT_PAS_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_PAS_DISCONNECT_IND
void *CsrBtPasDisconnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPasDisconnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPasDisconnectInd *) CsrPmemZalloc(sizeof(CsrBtPasDisconnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAS_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_PAS_NEXT_IND
CsrSize CsrBtPasNextIndSizeof(void *msg)
{
    CsrBtPasNextInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAS_NEXT_IND */

#ifndef EXCLUDE_CSR_BT_PAS_NEXT_IND
CsrUint8 *CsrBtPasNextIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPasNextInd *primitive;

    primitive = (CsrBtPasNextInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAS_NEXT_IND */

#ifndef EXCLUDE_CSR_BT_PAS_NEXT_IND
void *CsrBtPasNextIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPasNextInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPasNextInd *) CsrPmemZalloc(sizeof(CsrBtPasNextInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAS_NEXT_IND */

#ifndef EXCLUDE_CSR_BT_PAS_ACTIVATE_REQ
CsrSize CsrBtPasActivateReqSizeof(void *msg)
{
    CsrBtPasActivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAS_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_PAS_ACTIVATE_REQ
CsrUint8 *CsrBtPasActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPasActivateReq *primitive;

    primitive = (CsrBtPasActivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->feature);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->repository);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->obexMaxPacketSize);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->windowSize);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmEnable);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAS_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_PAS_ACTIVATE_REQ
void *CsrBtPasActivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPasActivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPasActivateReq *) CsrPmemZalloc(sizeof(CsrBtPasActivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->feature, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->repository, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->obexMaxPacketSize, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->windowSize, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmEnable, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAS_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_SIZE_IND
CsrSize CsrBtPasPullVcardListSizeIndSizeof(void *msg)
{
    CsrBtPasPullVcardListSizeInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_SIZE_IND */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_SIZE_IND
CsrUint8 *CsrBtPasPullVcardListSizeIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPasPullVcardListSizeInd *primitive;

    primitive = (CsrBtPasPullVcardListSizeInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ucs2nameOffset);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_SIZE_IND */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_SIZE_IND
void *CsrBtPasPullVcardListSizeIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPasPullVcardListSizeInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPasPullVcardListSizeInd *) CsrPmemZalloc(sizeof(CsrBtPasPullVcardListSizeInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_SIZE_IND */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_SIZE_IND
void CsrBtPasPullVcardListSizeIndSerFree(void *msg)
{
    CsrBtPasPullVcardListSizeInd *primitive;

    primitive = (CsrBtPasPullVcardListSizeInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_SIZE_IND */

#ifndef EXCLUDE_CSR_BT_PAS_ABORT_IND
CsrSize CsrBtPasAbortIndSizeof(void *msg)
{
    CsrBtPasAbortInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAS_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_PAS_ABORT_IND
CsrUint8 *CsrBtPasAbortIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPasAbortInd *primitive;

    primitive = (CsrBtPasAbortInd *) msg;
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
#endif /* EXCLUDE_CSR_BT_PAS_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_PAS_ABORT_IND
void *CsrBtPasAbortIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPasAbortInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPasAbortInd *) CsrPmemZalloc(sizeof(CsrBtPasAbortInd));

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
#endif /* EXCLUDE_CSR_BT_PAS_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_PAS_ABORT_IND
void CsrBtPasAbortIndSerFree(void *msg)
{
    CsrBtPasAbortInd *primitive;

    primitive = (CsrBtPasAbortInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_PAS_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_PAS_NEXT_RES
CsrSize CsrBtPasNextResSizeof(void *msg)
{
    CsrBtPasNextRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->bodyLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAS_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_PAS_NEXT_RES
CsrUint8 *CsrBtPasNextResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPasNextRes *primitive;

    primitive = (CsrBtPasNextRes *) msg;
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
#endif /* EXCLUDE_CSR_BT_PAS_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_PAS_NEXT_RES
void *CsrBtPasNextResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPasNextRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPasNextRes *) CsrPmemZalloc(sizeof(CsrBtPasNextRes));

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
#endif /* EXCLUDE_CSR_BT_PAS_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_PAS_NEXT_RES
void CsrBtPasNextResSerFree(void *msg)
{
    CsrBtPasNextRes *primitive;

    primitive = (CsrBtPasNextRes *) msg;

    if(primitive->body != NULL)
    {
        CsrPmemFree(primitive->body);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_PAS_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_MCH_LIST_SIZE_IND
CsrSize CsrBtPasPullVcardMchListSizeIndSizeof(void *msg)
{
    CsrBtPasPullVcardMchListSizeInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_MCH_LIST_SIZE_IND */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_MCH_LIST_SIZE_IND
CsrUint8 *CsrBtPasPullVcardMchListSizeIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPasPullVcardMchListSizeInd *primitive;

    primitive = (CsrBtPasPullVcardMchListSizeInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_MCH_LIST_SIZE_IND */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_MCH_LIST_SIZE_IND
void *CsrBtPasPullVcardMchListSizeIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPasPullVcardMchListSizeInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPasPullVcardMchListSizeInd *) CsrPmemZalloc(sizeof(CsrBtPasPullVcardMchListSizeInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_MCH_LIST_SIZE_IND */

#ifndef EXCLUDE_CSR_BT_PAS_SET_BACK_FOLDER_IND
CsrSize CsrBtPasSetBackFolderIndSizeof(void *msg)
{
    CsrBtPasSetBackFolderInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PAS_SET_BACK_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_PAS_SET_BACK_FOLDER_IND
CsrUint8 *CsrBtPasSetBackFolderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPasSetBackFolderInd *primitive;

    primitive = (CsrBtPasSetBackFolderInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PAS_SET_BACK_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_PAS_SET_BACK_FOLDER_IND
void *CsrBtPasSetBackFolderIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPasSetBackFolderInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPasSetBackFolderInd *) CsrPmemZalloc(sizeof(CsrBtPasSetBackFolderInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PAS_SET_BACK_FOLDER_IND */

#endif /* EXCLUDE_CSR_BT_PAS_MODULE */
