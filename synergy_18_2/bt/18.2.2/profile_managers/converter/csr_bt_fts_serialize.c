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
#include "csr_bt_fts_prim.h"

#ifndef EXCLUDE_CSR_BT_FTS_MODULE

void CsrBtFtsPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_FTS_AUTHENTICATE_CFM
CsrSize CsrBtFtsAuthenticateCfmSizeof(void *msg)
{
    CsrBtFtsAuthenticateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTS_AUTHENTICATE_CFM */

#ifndef EXCLUDE_CSR_BT_FTS_AUTHENTICATE_CFM
CsrUint8 *CsrBtFtsAuthenticateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtsAuthenticateCfm *primitive;

    primitive = (CsrBtFtsAuthenticateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTS_AUTHENTICATE_CFM */

#ifndef EXCLUDE_CSR_BT_FTS_AUTHENTICATE_CFM
void *CsrBtFtsAuthenticateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtsAuthenticateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtsAuthenticateCfm *) CsrPmemZalloc(sizeof(CsrBtFtsAuthenticateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTS_AUTHENTICATE_CFM */

#ifndef EXCLUDE_CSR_BT_FTS_SET_FOLDER_IND
CsrSize CsrBtFtsSetFolderIndSizeof(void *msg)
{
    CsrBtFtsSetFolderInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTS_SET_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_FTS_SET_FOLDER_IND
CsrUint8 *CsrBtFtsSetFolderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtsSetFolderInd *primitive;

    primitive = (CsrBtFtsSetFolderInd *) msg;
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
#endif /* EXCLUDE_CSR_BT_FTS_SET_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_FTS_SET_FOLDER_IND
void *CsrBtFtsSetFolderIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtsSetFolderInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtsSetFolderInd *) CsrPmemZalloc(sizeof(CsrBtFtsSetFolderInd));

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
#endif /* EXCLUDE_CSR_BT_FTS_SET_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_FTS_SET_FOLDER_IND
void CsrBtFtsSetFolderIndSerFree(void *msg)
{
    CsrBtFtsSetFolderInd *primitive;

    primitive = (CsrBtFtsSetFolderInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_FTS_SET_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_FTS_MOVE_OBJ_RES
CsrSize CsrBtFtsMoveObjResSizeof(void *msg)
{
    CsrBtFtsMoveObjRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTS_MOVE_OBJ_RES */

#ifndef EXCLUDE_CSR_BT_FTS_MOVE_OBJ_RES
CsrUint8 *CsrBtFtsMoveObjResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtsMoveObjRes *primitive;

    primitive = (CsrBtFtsMoveObjRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTS_MOVE_OBJ_RES */

#ifndef EXCLUDE_CSR_BT_FTS_MOVE_OBJ_RES
void *CsrBtFtsMoveObjResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtsMoveObjRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtsMoveObjRes *) CsrPmemZalloc(sizeof(CsrBtFtsMoveObjRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTS_MOVE_OBJ_RES */

#ifndef EXCLUDE_CSR_BT_FTS_PUT_OBJ_NEXT_IND
CsrSize CsrBtFtsPutObjNextIndSizeof(void *msg)
{
    CsrBtFtsPutObjNextInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTS_PUT_OBJ_NEXT_IND */

#ifndef EXCLUDE_CSR_BT_FTS_PUT_OBJ_NEXT_IND
CsrUint8 *CsrBtFtsPutObjNextIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtsPutObjNextInd *primitive;

    primitive = (CsrBtFtsPutObjNextInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
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
#endif /* EXCLUDE_CSR_BT_FTS_PUT_OBJ_NEXT_IND */

#ifndef EXCLUDE_CSR_BT_FTS_PUT_OBJ_NEXT_IND
void *CsrBtFtsPutObjNextIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtsPutObjNextInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtsPutObjNextInd *) CsrPmemZalloc(sizeof(CsrBtFtsPutObjNextInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_FTS_PUT_OBJ_NEXT_IND */

#ifndef EXCLUDE_CSR_BT_FTS_PUT_OBJ_NEXT_IND
void CsrBtFtsPutObjNextIndSerFree(void *msg)
{
    CsrBtFtsPutObjNextInd *primitive;

    primitive = (CsrBtFtsPutObjNextInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_FTS_PUT_OBJ_NEXT_IND */

#ifndef EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_RES
CsrSize CsrBtFtsGetListFolderResSizeof(void *msg)
{
    CsrBtFtsGetListFolderRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->bodyLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_RES */

#ifndef EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_RES
CsrUint8 *CsrBtFtsGetListFolderResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtsGetListFolderRes *primitive;

    primitive = (CsrBtFtsGetListFolderRes *) msg;
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
#endif /* EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_RES */

#ifndef EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_RES
void *CsrBtFtsGetListFolderResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtsGetListFolderRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtsGetListFolderRes *) CsrPmemZalloc(sizeof(CsrBtFtsGetListFolderRes));

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
#endif /* EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_RES */

#ifndef EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_RES
void CsrBtFtsGetListFolderResSerFree(void *msg)
{
    CsrBtFtsGetListFolderRes *primitive;

    primitive = (CsrBtFtsGetListFolderRes *) msg;

    if(primitive->body != NULL)
    {
        CsrPmemFree(primitive->body);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_RES */

#ifndef EXCLUDE_CSR_BT_FTS_DEACTIVATE_REQ
CsrSize CsrBtFtsDeactivateReqSizeof(void *msg)
{
    CsrBtFtsDeactivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTS_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_FTS_DEACTIVATE_REQ
CsrUint8 *CsrBtFtsDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtsDeactivateReq *primitive;

    primitive = (CsrBtFtsDeactivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTS_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_FTS_DEACTIVATE_REQ
void *CsrBtFtsDeactivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtsDeactivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtsDeactivateReq *) CsrPmemZalloc(sizeof(CsrBtFtsDeactivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTS_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_FTS_SET_BACK_FOLDER_RES
CsrSize CsrBtFtsSetBackFolderResSizeof(void *msg)
{
    CsrBtFtsSetBackFolderRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTS_SET_BACK_FOLDER_RES */

#ifndef EXCLUDE_CSR_BT_FTS_SET_BACK_FOLDER_RES
CsrUint8 *CsrBtFtsSetBackFolderResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtsSetBackFolderRes *primitive;

    primitive = (CsrBtFtsSetBackFolderRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTS_SET_BACK_FOLDER_RES */

#ifndef EXCLUDE_CSR_BT_FTS_SET_BACK_FOLDER_RES
void *CsrBtFtsSetBackFolderResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtsSetBackFolderRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtsSetBackFolderRes *) CsrPmemZalloc(sizeof(CsrBtFtsSetBackFolderRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTS_SET_BACK_FOLDER_RES */

#ifndef EXCLUDE_CSR_BT_FTS_CONNECT_RES
CsrSize CsrBtFtsConnectResSizeof(void *msg)
{
    CsrBtFtsConnectRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTS_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_FTS_CONNECT_RES
CsrUint8 *CsrBtFtsConnectResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtsConnectRes *primitive;

    primitive = (CsrBtFtsConnectRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTS_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_FTS_CONNECT_RES
void *CsrBtFtsConnectResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtsConnectRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtsConnectRes *) CsrPmemZalloc(sizeof(CsrBtFtsConnectRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTS_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_FTS_DEACTIVATE_CFM
CsrSize CsrBtFtsDeactivateCfmSizeof(void *msg)
{
    CsrBtFtsDeactivateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTS_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_FTS_DEACTIVATE_CFM
CsrUint8 *CsrBtFtsDeactivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtsDeactivateCfm *primitive;

    primitive = (CsrBtFtsDeactivateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTS_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_FTS_DEACTIVATE_CFM
void *CsrBtFtsDeactivateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtsDeactivateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtsDeactivateCfm *) CsrPmemZalloc(sizeof(CsrBtFtsDeactivateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTS_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_FTS_SET_FOLDER_RES
CsrSize CsrBtFtsSetFolderResSizeof(void *msg)
{
    CsrBtFtsSetFolderRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTS_SET_FOLDER_RES */

#ifndef EXCLUDE_CSR_BT_FTS_SET_FOLDER_RES
CsrUint8 *CsrBtFtsSetFolderResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtsSetFolderRes *primitive;

    primitive = (CsrBtFtsSetFolderRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTS_SET_FOLDER_RES */

#ifndef EXCLUDE_CSR_BT_FTS_SET_FOLDER_RES
void *CsrBtFtsSetFolderResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtsSetFolderRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtsSetFolderRes *) CsrPmemZalloc(sizeof(CsrBtFtsSetFolderRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTS_SET_FOLDER_RES */

#ifndef EXCLUDE_CSR_BT_FTS_SET_ROOT_FOLDER_IND
CsrSize CsrBtFtsSetRootFolderIndSizeof(void *msg)
{
    CsrBtFtsSetRootFolderInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTS_SET_ROOT_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_FTS_SET_ROOT_FOLDER_IND
CsrUint8 *CsrBtFtsSetRootFolderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtsSetRootFolderInd *primitive;

    primitive = (CsrBtFtsSetRootFolderInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTS_SET_ROOT_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_FTS_SET_ROOT_FOLDER_IND
void *CsrBtFtsSetRootFolderIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtsSetRootFolderInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtsSetRootFolderInd *) CsrPmemZalloc(sizeof(CsrBtFtsSetRootFolderInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTS_SET_ROOT_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_FTS_GET_OBJ_NEXT_RES
CsrSize CsrBtFtsGetObjNextResSizeof(void *msg)
{
    CsrBtFtsGetObjNextRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->bodyLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTS_GET_OBJ_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_FTS_GET_OBJ_NEXT_RES
CsrUint8 *CsrBtFtsGetObjNextResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtsGetObjNextRes *primitive;

    primitive = (CsrBtFtsGetObjNextRes *) msg;
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
#endif /* EXCLUDE_CSR_BT_FTS_GET_OBJ_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_FTS_GET_OBJ_NEXT_RES
void *CsrBtFtsGetObjNextResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtsGetObjNextRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtsGetObjNextRes *) CsrPmemZalloc(sizeof(CsrBtFtsGetObjNextRes));

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
#endif /* EXCLUDE_CSR_BT_FTS_GET_OBJ_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_FTS_GET_OBJ_NEXT_RES
void CsrBtFtsGetObjNextResSerFree(void *msg)
{
    CsrBtFtsGetObjNextRes *primitive;

    primitive = (CsrBtFtsGetObjNextRes *) msg;

    if(primitive->body != NULL)
    {
        CsrPmemFree(primitive->body);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_FTS_GET_OBJ_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_FTS_AUTHENTICATE_REQ
CsrSize CsrBtFtsAuthenticateReqSizeof(void *msg)
{
    CsrBtFtsAuthenticateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->realmLength * sizeof(CsrUint8)) + (primitive->passwordLength * sizeof(CsrUint8)) + CsrCharStringSerLen((CsrCharString *) primitive->userId));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTS_AUTHENTICATE_REQ */

#ifndef EXCLUDE_CSR_BT_FTS_AUTHENTICATE_REQ
CsrUint8 *CsrBtFtsAuthenticateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtsAuthenticateReq *primitive;

    primitive = (CsrBtFtsAuthenticateReq *) msg;
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
#endif /* EXCLUDE_CSR_BT_FTS_AUTHENTICATE_REQ */

#ifndef EXCLUDE_CSR_BT_FTS_AUTHENTICATE_REQ
void *CsrBtFtsAuthenticateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtsAuthenticateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtsAuthenticateReq *) CsrPmemZalloc(sizeof(CsrBtFtsAuthenticateReq));

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
#endif /* EXCLUDE_CSR_BT_FTS_AUTHENTICATE_REQ */

#ifndef EXCLUDE_CSR_BT_FTS_AUTHENTICATE_REQ
void CsrBtFtsAuthenticateReqSerFree(void *msg)
{
    CsrBtFtsAuthenticateReq *primitive;

    primitive = (CsrBtFtsAuthenticateReq *) msg;

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
#endif /* EXCLUDE_CSR_BT_FTS_AUTHENTICATE_REQ */

#ifndef EXCLUDE_CSR_BT_FTS_PUT_OBJ_IND
CsrSize CsrBtFtsPutObjIndSizeof(void *msg)
{
    CsrBtFtsPutObjInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTS_PUT_OBJ_IND */

#ifndef EXCLUDE_CSR_BT_FTS_PUT_OBJ_IND
CsrUint8 *CsrBtFtsPutObjIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtsPutObjInd *primitive;

    primitive = (CsrBtFtsPutObjInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->lengthOfObject);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ucs2nameOffset);
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
#endif /* EXCLUDE_CSR_BT_FTS_PUT_OBJ_IND */

#ifndef EXCLUDE_CSR_BT_FTS_PUT_OBJ_IND
void *CsrBtFtsPutObjIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtsPutObjInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtsPutObjInd *) CsrPmemZalloc(sizeof(CsrBtFtsPutObjInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->lengthOfObject, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->ucs2nameOffset, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_FTS_PUT_OBJ_IND */

#ifndef EXCLUDE_CSR_BT_FTS_PUT_OBJ_IND
void CsrBtFtsPutObjIndSerFree(void *msg)
{
    CsrBtFtsPutObjInd *primitive;

    primitive = (CsrBtFtsPutObjInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_FTS_PUT_OBJ_IND */

#ifndef EXCLUDE_CSR_BT_FTS_GET_OBJ_IND
CsrSize CsrBtFtsGetObjIndSizeof(void *msg)
{
    CsrBtFtsGetObjInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTS_GET_OBJ_IND */

#ifndef EXCLUDE_CSR_BT_FTS_GET_OBJ_IND
CsrUint8 *CsrBtFtsGetObjIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtsGetObjInd *primitive;

    primitive = (CsrBtFtsGetObjInd *) msg;
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
#endif /* EXCLUDE_CSR_BT_FTS_GET_OBJ_IND */

#ifndef EXCLUDE_CSR_BT_FTS_GET_OBJ_IND
void *CsrBtFtsGetObjIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtsGetObjInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtsGetObjInd *) CsrPmemZalloc(sizeof(CsrBtFtsGetObjInd));

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
#endif /* EXCLUDE_CSR_BT_FTS_GET_OBJ_IND */

#ifndef EXCLUDE_CSR_BT_FTS_GET_OBJ_IND
void CsrBtFtsGetObjIndSerFree(void *msg)
{
    CsrBtFtsGetObjInd *primitive;

    primitive = (CsrBtFtsGetObjInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_FTS_GET_OBJ_IND */

#ifndef EXCLUDE_CSR_BT_FTS_ACTIVATE_REQ
CsrSize CsrBtFtsActivateReqSizeof(void *msg)
{
    CsrBtFtsActivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTS_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_FTS_ACTIVATE_REQ
CsrUint8 *CsrBtFtsActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtsActivateReq *primitive;

    primitive = (CsrBtFtsActivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->obexMaxPacketSize);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->windowSize);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmEnable);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTS_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_FTS_ACTIVATE_REQ
void *CsrBtFtsActivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtsActivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtsActivateReq *) CsrPmemZalloc(sizeof(CsrBtFtsActivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->obexMaxPacketSize, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->windowSize, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmEnable, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTS_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_FTS_CONNECT_IND
CsrSize CsrBtFtsConnectIndSizeof(void *msg)
{
    CsrBtFtsConnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTS_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_FTS_CONNECT_IND
CsrUint8 *CsrBtFtsConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtsConnectInd *primitive;

    primitive = (CsrBtFtsConnectInd *) msg;
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
#endif /* EXCLUDE_CSR_BT_FTS_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_FTS_CONNECT_IND
void *CsrBtFtsConnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtsConnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtsConnectInd *) CsrPmemZalloc(sizeof(CsrBtFtsConnectInd));

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
#endif /* EXCLUDE_CSR_BT_FTS_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_FTS_GET_OBJ_RES
CsrSize CsrBtFtsGetObjResSizeof(void *msg)
{
    CsrBtFtsGetObjRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->bodyLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTS_GET_OBJ_RES */

#ifndef EXCLUDE_CSR_BT_FTS_GET_OBJ_RES
CsrUint8 *CsrBtFtsGetObjResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtsGetObjRes *primitive;

    primitive = (CsrBtFtsGetObjRes *) msg;
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
#endif /* EXCLUDE_CSR_BT_FTS_GET_OBJ_RES */

#ifndef EXCLUDE_CSR_BT_FTS_GET_OBJ_RES
void *CsrBtFtsGetObjResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtsGetObjRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtsGetObjRes *) CsrPmemZalloc(sizeof(CsrBtFtsGetObjRes));

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
#endif /* EXCLUDE_CSR_BT_FTS_GET_OBJ_RES */

#ifndef EXCLUDE_CSR_BT_FTS_GET_OBJ_RES
void CsrBtFtsGetObjResSerFree(void *msg)
{
    CsrBtFtsGetObjRes *primitive;

    primitive = (CsrBtFtsGetObjRes *) msg;

    if(primitive->body != NULL)
    {
        CsrPmemFree(primitive->body);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_FTS_GET_OBJ_RES */

#ifndef EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_NEXT_RES
CsrSize CsrBtFtsGetListFolderNextResSizeof(void *msg)
{
    CsrBtFtsGetListFolderNextRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->bodyLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_NEXT_RES
CsrUint8 *CsrBtFtsGetListFolderNextResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtsGetListFolderNextRes *primitive;

    primitive = (CsrBtFtsGetListFolderNextRes *) msg;
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
#endif /* EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_NEXT_RES
void *CsrBtFtsGetListFolderNextResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtsGetListFolderNextRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtsGetListFolderNextRes *) CsrPmemZalloc(sizeof(CsrBtFtsGetListFolderNextRes));

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
#endif /* EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_NEXT_RES
void CsrBtFtsGetListFolderNextResSerFree(void *msg)
{
    CsrBtFtsGetListFolderNextRes *primitive;

    primitive = (CsrBtFtsGetListFolderNextRes *) msg;

    if(primitive->body != NULL)
    {
        CsrPmemFree(primitive->body);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_FTS_MOVE_OBJ_IND
CsrSize CsrBtFtsMoveObjIndSizeof(void *msg)
{
    CsrBtFtsMoveObjInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTS_MOVE_OBJ_IND */

#ifndef EXCLUDE_CSR_BT_FTS_MOVE_OBJ_IND
CsrUint8 *CsrBtFtsMoveObjIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtsMoveObjInd *primitive;

    primitive = (CsrBtFtsMoveObjInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ucs2srcNameOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ucs2destNameOffset);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTS_MOVE_OBJ_IND */

#ifndef EXCLUDE_CSR_BT_FTS_MOVE_OBJ_IND
void *CsrBtFtsMoveObjIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtsMoveObjInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtsMoveObjInd *) CsrPmemZalloc(sizeof(CsrBtFtsMoveObjInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->ucs2srcNameOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->ucs2destNameOffset, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_FTS_MOVE_OBJ_IND */

#ifndef EXCLUDE_CSR_BT_FTS_MOVE_OBJ_IND
void CsrBtFtsMoveObjIndSerFree(void *msg)
{
    CsrBtFtsMoveObjInd *primitive;

    primitive = (CsrBtFtsMoveObjInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_FTS_MOVE_OBJ_IND */

#ifndef EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_IND
CsrSize CsrBtFtsGetListFolderIndSizeof(void *msg)
{
    CsrBtFtsGetListFolderInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_IND
CsrUint8 *CsrBtFtsGetListFolderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtsGetListFolderInd *primitive;

    primitive = (CsrBtFtsGetListFolderInd *) msg;
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
#endif /* EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_IND
void *CsrBtFtsGetListFolderIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtsGetListFolderInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtsGetListFolderInd *) CsrPmemZalloc(sizeof(CsrBtFtsGetListFolderInd));

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
#endif /* EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_IND
void CsrBtFtsGetListFolderIndSerFree(void *msg)
{
    CsrBtFtsGetListFolderInd *primitive;

    primitive = (CsrBtFtsGetListFolderInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_FTS_ABORT_IND
CsrSize CsrBtFtsAbortIndSizeof(void *msg)
{
    CsrBtFtsAbortInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTS_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_FTS_ABORT_IND
CsrUint8 *CsrBtFtsAbortIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtsAbortInd *primitive;

    primitive = (CsrBtFtsAbortInd *) msg;
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
#endif /* EXCLUDE_CSR_BT_FTS_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_FTS_ABORT_IND
void *CsrBtFtsAbortIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtsAbortInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtsAbortInd *) CsrPmemZalloc(sizeof(CsrBtFtsAbortInd));

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
#endif /* EXCLUDE_CSR_BT_FTS_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_FTS_ABORT_IND
void CsrBtFtsAbortIndSerFree(void *msg)
{
    CsrBtFtsAbortInd *primitive;

    primitive = (CsrBtFtsAbortInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_FTS_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_FTS_SET_BACK_FOLDER_IND
CsrSize CsrBtFtsSetBackFolderIndSizeof(void *msg)
{
    CsrBtFtsSetBackFolderInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTS_SET_BACK_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_FTS_SET_BACK_FOLDER_IND
CsrUint8 *CsrBtFtsSetBackFolderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtsSetBackFolderInd *primitive;

    primitive = (CsrBtFtsSetBackFolderInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTS_SET_BACK_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_FTS_SET_BACK_FOLDER_IND
void *CsrBtFtsSetBackFolderIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtsSetBackFolderInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtsSetBackFolderInd *) CsrPmemZalloc(sizeof(CsrBtFtsSetBackFolderInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTS_SET_BACK_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_FTS_COPY_OBJ_RES
CsrSize CsrBtFtsCopyObjResSizeof(void *msg)
{
    CsrBtFtsCopyObjRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTS_COPY_OBJ_RES */

#ifndef EXCLUDE_CSR_BT_FTS_COPY_OBJ_RES
CsrUint8 *CsrBtFtsCopyObjResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtsCopyObjRes *primitive;

    primitive = (CsrBtFtsCopyObjRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTS_COPY_OBJ_RES */

#ifndef EXCLUDE_CSR_BT_FTS_COPY_OBJ_RES
void *CsrBtFtsCopyObjResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtsCopyObjRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtsCopyObjRes *) CsrPmemZalloc(sizeof(CsrBtFtsCopyObjRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTS_COPY_OBJ_RES */

#ifndef EXCLUDE_CSR_BT_FTS_SET_ROOT_FOLDER_RES
CsrSize CsrBtFtsSetRootFolderResSizeof(void *msg)
{
    CsrBtFtsSetRootFolderRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTS_SET_ROOT_FOLDER_RES */

#ifndef EXCLUDE_CSR_BT_FTS_SET_ROOT_FOLDER_RES
CsrUint8 *CsrBtFtsSetRootFolderResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtsSetRootFolderRes *primitive;

    primitive = (CsrBtFtsSetRootFolderRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTS_SET_ROOT_FOLDER_RES */

#ifndef EXCLUDE_CSR_BT_FTS_SET_ROOT_FOLDER_RES
void *CsrBtFtsSetRootFolderResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtsSetRootFolderRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtsSetRootFolderRes *) CsrPmemZalloc(sizeof(CsrBtFtsSetRootFolderRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTS_SET_ROOT_FOLDER_RES */

#ifndef EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_NEXT_IND
CsrSize CsrBtFtsGetListFolderNextIndSizeof(void *msg)
{
    CsrBtFtsGetListFolderNextInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_NEXT_IND */

#ifndef EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_NEXT_IND
CsrUint8 *CsrBtFtsGetListFolderNextIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtsGetListFolderNextInd *primitive;

    primitive = (CsrBtFtsGetListFolderNextInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_NEXT_IND */

#ifndef EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_NEXT_IND
void *CsrBtFtsGetListFolderNextIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtsGetListFolderNextInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtsGetListFolderNextInd *) CsrPmemZalloc(sizeof(CsrBtFtsGetListFolderNextInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_NEXT_IND */

#ifndef EXCLUDE_CSR_BT_FTS_DEL_OBJ_RES
CsrSize CsrBtFtsDelObjResSizeof(void *msg)
{
    CsrBtFtsDelObjRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTS_DEL_OBJ_RES */

#ifndef EXCLUDE_CSR_BT_FTS_DEL_OBJ_RES
CsrUint8 *CsrBtFtsDelObjResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtsDelObjRes *primitive;

    primitive = (CsrBtFtsDelObjRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTS_DEL_OBJ_RES */

#ifndef EXCLUDE_CSR_BT_FTS_DEL_OBJ_RES
void *CsrBtFtsDelObjResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtsDelObjRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtsDelObjRes *) CsrPmemZalloc(sizeof(CsrBtFtsDelObjRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTS_DEL_OBJ_RES */

#ifndef EXCLUDE_CSR_BT_FTS_DISCONNECT_IND
CsrSize CsrBtFtsDisconnectIndSizeof(void *msg)
{
    CsrBtFtsDisconnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTS_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_FTS_DISCONNECT_IND
CsrUint8 *CsrBtFtsDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtsDisconnectInd *primitive;

    primitive = (CsrBtFtsDisconnectInd *) msg;
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
#endif /* EXCLUDE_CSR_BT_FTS_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_FTS_DISCONNECT_IND
void *CsrBtFtsDisconnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtsDisconnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtsDisconnectInd *) CsrPmemZalloc(sizeof(CsrBtFtsDisconnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTS_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_FTS_SET_ADD_FOLDER_IND
CsrSize CsrBtFtsSetAddFolderIndSizeof(void *msg)
{
    CsrBtFtsSetAddFolderInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTS_SET_ADD_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_FTS_SET_ADD_FOLDER_IND
CsrUint8 *CsrBtFtsSetAddFolderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtsSetAddFolderInd *primitive;

    primitive = (CsrBtFtsSetAddFolderInd *) msg;
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
#endif /* EXCLUDE_CSR_BT_FTS_SET_ADD_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_FTS_SET_ADD_FOLDER_IND
void *CsrBtFtsSetAddFolderIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtsSetAddFolderInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtsSetAddFolderInd *) CsrPmemZalloc(sizeof(CsrBtFtsSetAddFolderInd));

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
#endif /* EXCLUDE_CSR_BT_FTS_SET_ADD_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_FTS_SET_ADD_FOLDER_IND
void CsrBtFtsSetAddFolderIndSerFree(void *msg)
{
    CsrBtFtsSetAddFolderInd *primitive;

    primitive = (CsrBtFtsSetAddFolderInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_FTS_SET_ADD_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_FTS_PUT_OBJ_RES
CsrSize CsrBtFtsPutObjResSizeof(void *msg)
{
    CsrBtFtsPutObjRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTS_PUT_OBJ_RES */

#ifndef EXCLUDE_CSR_BT_FTS_PUT_OBJ_RES
CsrUint8 *CsrBtFtsPutObjResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtsPutObjRes *primitive;

    primitive = (CsrBtFtsPutObjRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTS_PUT_OBJ_RES */

#ifndef EXCLUDE_CSR_BT_FTS_PUT_OBJ_RES
void *CsrBtFtsPutObjResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtsPutObjRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtsPutObjRes *) CsrPmemZalloc(sizeof(CsrBtFtsPutObjRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTS_PUT_OBJ_RES */

#ifndef EXCLUDE_CSR_BT_FTS_AUTHENTICATE_IND
CsrSize CsrBtFtsAuthenticateIndSizeof(void *msg)
{
    CsrBtFtsAuthenticateInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->realmLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTS_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_FTS_AUTHENTICATE_IND
CsrUint8 *CsrBtFtsAuthenticateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtsAuthenticateInd *primitive;

    primitive = (CsrBtFtsAuthenticateInd *) msg;
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
#endif /* EXCLUDE_CSR_BT_FTS_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_FTS_AUTHENTICATE_IND
void *CsrBtFtsAuthenticateIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtsAuthenticateInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtsAuthenticateInd *) CsrPmemZalloc(sizeof(CsrBtFtsAuthenticateInd));

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
#endif /* EXCLUDE_CSR_BT_FTS_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_FTS_AUTHENTICATE_IND
void CsrBtFtsAuthenticateIndSerFree(void *msg)
{
    CsrBtFtsAuthenticateInd *primitive;

    primitive = (CsrBtFtsAuthenticateInd *) msg;

    if(primitive->realm != NULL)
    {
        CsrPmemFree(primitive->realm);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_FTS_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_FTS_SET_OBJ_PERMISSIONS_RES
CsrSize CsrBtFtsSetObjPermissionsResSizeof(void *msg)
{
    CsrBtFtsSetObjPermissionsRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTS_SET_OBJ_PERMISSIONS_RES */

#ifndef EXCLUDE_CSR_BT_FTS_SET_OBJ_PERMISSIONS_RES
CsrUint8 *CsrBtFtsSetObjPermissionsResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtsSetObjPermissionsRes *primitive;

    primitive = (CsrBtFtsSetObjPermissionsRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTS_SET_OBJ_PERMISSIONS_RES */

#ifndef EXCLUDE_CSR_BT_FTS_SET_OBJ_PERMISSIONS_RES
void *CsrBtFtsSetObjPermissionsResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtsSetObjPermissionsRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtsSetObjPermissionsRes *) CsrPmemZalloc(sizeof(CsrBtFtsSetObjPermissionsRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTS_SET_OBJ_PERMISSIONS_RES */

#ifndef EXCLUDE_CSR_BT_FTS_GET_OBJ_NEXT_IND
CsrSize CsrBtFtsGetObjNextIndSizeof(void *msg)
{
    CsrBtFtsGetObjNextInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTS_GET_OBJ_NEXT_IND */

#ifndef EXCLUDE_CSR_BT_FTS_GET_OBJ_NEXT_IND
CsrUint8 *CsrBtFtsGetObjNextIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtsGetObjNextInd *primitive;

    primitive = (CsrBtFtsGetObjNextInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTS_GET_OBJ_NEXT_IND */

#ifndef EXCLUDE_CSR_BT_FTS_GET_OBJ_NEXT_IND
void *CsrBtFtsGetObjNextIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtsGetObjNextInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtsGetObjNextInd *) CsrPmemZalloc(sizeof(CsrBtFtsGetObjNextInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTS_GET_OBJ_NEXT_IND */

#ifndef EXCLUDE_CSR_BT_FTS_SET_ADD_FOLDER_RES
CsrSize CsrBtFtsSetAddFolderResSizeof(void *msg)
{
    CsrBtFtsSetAddFolderRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTS_SET_ADD_FOLDER_RES */

#ifndef EXCLUDE_CSR_BT_FTS_SET_ADD_FOLDER_RES
CsrUint8 *CsrBtFtsSetAddFolderResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtsSetAddFolderRes *primitive;

    primitive = (CsrBtFtsSetAddFolderRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTS_SET_ADD_FOLDER_RES */

#ifndef EXCLUDE_CSR_BT_FTS_SET_ADD_FOLDER_RES
void *CsrBtFtsSetAddFolderResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtsSetAddFolderRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtsSetAddFolderRes *) CsrPmemZalloc(sizeof(CsrBtFtsSetAddFolderRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTS_SET_ADD_FOLDER_RES */

#ifndef EXCLUDE_CSR_BT_FTS_AUTHENTICATE_RES
CsrSize CsrBtFtsAuthenticateResSizeof(void *msg)
{
    CsrBtFtsAuthenticateRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->passwordLength * sizeof(CsrUint8)) + CsrCharStringSerLen((CsrCharString *) primitive->userId));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTS_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_FTS_AUTHENTICATE_RES
CsrUint8 *CsrBtFtsAuthenticateResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtsAuthenticateRes *primitive;

    primitive = (CsrBtFtsAuthenticateRes *) msg;
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
#endif /* EXCLUDE_CSR_BT_FTS_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_FTS_AUTHENTICATE_RES
void *CsrBtFtsAuthenticateResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtsAuthenticateRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtsAuthenticateRes *) CsrPmemZalloc(sizeof(CsrBtFtsAuthenticateRes));

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
#endif /* EXCLUDE_CSR_BT_FTS_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_FTS_AUTHENTICATE_RES
void CsrBtFtsAuthenticateResSerFree(void *msg)
{
    CsrBtFtsAuthenticateRes *primitive;

    primitive = (CsrBtFtsAuthenticateRes *) msg;

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
#endif /* EXCLUDE_CSR_BT_FTS_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_FTS_COPY_OBJ_IND
CsrSize CsrBtFtsCopyObjIndSizeof(void *msg)
{
    CsrBtFtsCopyObjInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTS_COPY_OBJ_IND */

#ifndef EXCLUDE_CSR_BT_FTS_COPY_OBJ_IND
CsrUint8 *CsrBtFtsCopyObjIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtsCopyObjInd *primitive;

    primitive = (CsrBtFtsCopyObjInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ucs2srcNameOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ucs2destNameOffset);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTS_COPY_OBJ_IND */

#ifndef EXCLUDE_CSR_BT_FTS_COPY_OBJ_IND
void *CsrBtFtsCopyObjIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtsCopyObjInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtsCopyObjInd *) CsrPmemZalloc(sizeof(CsrBtFtsCopyObjInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->ucs2srcNameOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->ucs2destNameOffset, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_FTS_COPY_OBJ_IND */

#ifndef EXCLUDE_CSR_BT_FTS_COPY_OBJ_IND
void CsrBtFtsCopyObjIndSerFree(void *msg)
{
    CsrBtFtsCopyObjInd *primitive;

    primitive = (CsrBtFtsCopyObjInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_FTS_COPY_OBJ_IND */

#ifndef EXCLUDE_CSR_BT_FTS_SET_OBJ_PERMISSIONS_IND
CsrSize CsrBtFtsSetObjPermissionsIndSizeof(void *msg)
{
    CsrBtFtsSetObjPermissionsInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTS_SET_OBJ_PERMISSIONS_IND */

#ifndef EXCLUDE_CSR_BT_FTS_SET_OBJ_PERMISSIONS_IND
CsrUint8 *CsrBtFtsSetObjPermissionsIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtsSetObjPermissionsInd *primitive;

    primitive = (CsrBtFtsSetObjPermissionsInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ucs2nameOffset);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->permissions);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTS_SET_OBJ_PERMISSIONS_IND */

#ifndef EXCLUDE_CSR_BT_FTS_SET_OBJ_PERMISSIONS_IND
void *CsrBtFtsSetObjPermissionsIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtsSetObjPermissionsInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtsSetObjPermissionsInd *) CsrPmemZalloc(sizeof(CsrBtFtsSetObjPermissionsInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->ucs2nameOffset, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->permissions, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_FTS_SET_OBJ_PERMISSIONS_IND */

#ifndef EXCLUDE_CSR_BT_FTS_SET_OBJ_PERMISSIONS_IND
void CsrBtFtsSetObjPermissionsIndSerFree(void *msg)
{
    CsrBtFtsSetObjPermissionsInd *primitive;

    primitive = (CsrBtFtsSetObjPermissionsInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_FTS_SET_OBJ_PERMISSIONS_IND */

#ifndef EXCLUDE_CSR_BT_FTS_SECURITY_IN_CFM
CsrSize CsrBtFtsSecurityInCfmSizeof(void *msg)
{
    CsrBtFtsSecurityInCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTS_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_FTS_SECURITY_IN_CFM
CsrUint8 *CsrBtFtsSecurityInCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtsSecurityInCfm *primitive;

    primitive = (CsrBtFtsSecurityInCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTS_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_FTS_SECURITY_IN_CFM
void *CsrBtFtsSecurityInCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtsSecurityInCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtsSecurityInCfm *) CsrPmemZalloc(sizeof(CsrBtFtsSecurityInCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTS_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_FTS_SECURITY_IN_REQ
CsrSize CsrBtFtsSecurityInReqSizeof(void *msg)
{
    CsrBtFtsSecurityInReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTS_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_FTS_SECURITY_IN_REQ
CsrUint8 *CsrBtFtsSecurityInReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtsSecurityInReq *primitive;

    primitive = (CsrBtFtsSecurityInReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTS_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_FTS_SECURITY_IN_REQ
void *CsrBtFtsSecurityInReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtsSecurityInReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtsSecurityInReq *) CsrPmemZalloc(sizeof(CsrBtFtsSecurityInReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTS_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_FTS_PUT_OBJ_NEXT_RES
CsrSize CsrBtFtsPutObjNextResSizeof(void *msg)
{
    CsrBtFtsPutObjNextRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTS_PUT_OBJ_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_FTS_PUT_OBJ_NEXT_RES
CsrUint8 *CsrBtFtsPutObjNextResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtsPutObjNextRes *primitive;

    primitive = (CsrBtFtsPutObjNextRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_FTS_PUT_OBJ_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_FTS_PUT_OBJ_NEXT_RES
void *CsrBtFtsPutObjNextResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtsPutObjNextRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtsPutObjNextRes *) CsrPmemZalloc(sizeof(CsrBtFtsPutObjNextRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_FTS_PUT_OBJ_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_FTS_DEL_OBJ_IND
CsrSize CsrBtFtsDelObjIndSizeof(void *msg)
{
    CsrBtFtsDelObjInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_FTS_DEL_OBJ_IND */

#ifndef EXCLUDE_CSR_BT_FTS_DEL_OBJ_IND
CsrUint8 *CsrBtFtsDelObjIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtFtsDelObjInd *primitive;

    primitive = (CsrBtFtsDelObjInd *) msg;
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
#endif /* EXCLUDE_CSR_BT_FTS_DEL_OBJ_IND */

#ifndef EXCLUDE_CSR_BT_FTS_DEL_OBJ_IND
void *CsrBtFtsDelObjIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtFtsDelObjInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtFtsDelObjInd *) CsrPmemZalloc(sizeof(CsrBtFtsDelObjInd));

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
#endif /* EXCLUDE_CSR_BT_FTS_DEL_OBJ_IND */

#ifndef EXCLUDE_CSR_BT_FTS_DEL_OBJ_IND
void CsrBtFtsDelObjIndSerFree(void *msg)
{
    CsrBtFtsDelObjInd *primitive;

    primitive = (CsrBtFtsDelObjInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_FTS_DEL_OBJ_IND */

#endif /* EXCLUDE_CSR_BT_FTS_MODULE */
