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
#include "csr_bt_bpps_prim.h"

#ifndef EXCLUDE_CSR_BT_BPPS_MODULE

void CsrBtBppsPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_BPPS_DEACTIVATE_CFM
CsrSize CsrBtBppsDeactivateCfmSizeof(void *msg)
{
    CsrBtBppsDeactivateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPS_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_BPPS_DEACTIVATE_CFM
CsrUint8 *CsrBtBppsDeactivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppsDeactivateCfm *primitive;

    primitive = (CsrBtBppsDeactivateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPS_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_BPPS_DEACTIVATE_CFM
void *CsrBtBppsDeactivateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppsDeactivateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppsDeactivateCfm *) CsrPmemZalloc(sizeof(CsrBtBppsDeactivateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPS_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_BPPS_JOB_COMPLETE_CFM
CsrSize CsrBtBppsJobCompleteCfmSizeof(void *msg)
{
    CsrBtBppsJobCompleteCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPS_JOB_COMPLETE_CFM */

#ifndef EXCLUDE_CSR_BT_BPPS_JOB_COMPLETE_CFM
CsrUint8 *CsrBtBppsJobCompleteCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppsJobCompleteCfm *primitive;

    primitive = (CsrBtBppsJobCompleteCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPS_JOB_COMPLETE_CFM */

#ifndef EXCLUDE_CSR_BT_BPPS_JOB_COMPLETE_CFM
void *CsrBtBppsJobCompleteCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppsJobCompleteCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppsJobCompleteCfm *) CsrPmemZalloc(sizeof(CsrBtBppsJobCompleteCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPS_JOB_COMPLETE_CFM */

#ifndef EXCLUDE_CSR_BT_BPPS_NEXT_RES
CsrSize CsrBtBppsNextResSizeof(void *msg)
{
    CsrBtBppsNextRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPS_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_BPPS_NEXT_RES
CsrUint8 *CsrBtBppsNextResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppsNextRes *primitive;

    primitive = (CsrBtBppsNextRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPS_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_BPPS_NEXT_RES
void *CsrBtBppsNextResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppsNextRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppsNextRes *) CsrPmemZalloc(sizeof(CsrBtBppsNextRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPS_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_REQ
CsrSize CsrBtBppsGetReferencedObjReqSizeof(void *msg)
{
    CsrBtBppsGetReferencedObjReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->objName));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_REQ */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_REQ
CsrUint8 *CsrBtBppsGetReferencedObjReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppsGetReferencedObjReq *primitive;

    primitive = (CsrBtBppsGetReferencedObjReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->objName, sizeof(primitive->objName));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->offset);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->count);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->getFileSize);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    CsrUtf8StringSer(buffer, length, primitive->objName);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_REQ */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_REQ
void *CsrBtBppsGetReferencedObjReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppsGetReferencedObjReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppsGetReferencedObjReq *) CsrPmemZalloc(sizeof(CsrBtBppsGetReferencedObjReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->objName), buffer, &offset, ((CsrSize) sizeof(primitive->objName)));
    CsrUint32Des((CsrUint32 *) &primitive->offset, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->count, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->getFileSize, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    CsrUtf8StringDes(&primitive->objName, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_REQ */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_REQ
void CsrBtBppsGetReferencedObjReqSerFree(void *msg)
{
    CsrBtBppsGetReferencedObjReq *primitive;

    primitive = (CsrBtBppsGetReferencedObjReq *) msg;

    if(primitive->objName != NULL)
    {
        CsrPmemFree(primitive->objName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_REQ */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_PRINTER_ATTRIBS_IND
CsrSize CsrBtBppsGetPrinterAttribsIndSizeof(void *msg)
{
    CsrBtBppsGetPrinterAttribsInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPS_GET_PRINTER_ATTRIBS_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_PRINTER_ATTRIBS_IND
CsrUint8 *CsrBtBppsGetPrinterAttribsIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppsGetPrinterAttribsInd *primitive;

    primitive = (CsrBtBppsGetPrinterAttribsInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->printerName);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->printerLocation);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->printerState);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->printerStateReasons);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->documentFormatsSupported);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->colorSupported);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->maxCopiesSupported);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->sidesSupported);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->numberUpSupported);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->orientationsSupported);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mediaSizesSupported);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mediaTypesSupported);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mediaLoaded);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->printQualitySupported);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->queuedJobCount);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->imageFormatsSupported);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->basicTextPageWidth);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->basicTextPageHeight);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->printerGeneralCurrentOperator);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPS_GET_PRINTER_ATTRIBS_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_PRINTER_ATTRIBS_IND
void *CsrBtBppsGetPrinterAttribsIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppsGetPrinterAttribsInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppsGetPrinterAttribsInd *) CsrPmemZalloc(sizeof(CsrBtBppsGetPrinterAttribsInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->printerName, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->printerLocation, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->printerState, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->printerStateReasons, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->documentFormatsSupported, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->colorSupported, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->maxCopiesSupported, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->sidesSupported, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->numberUpSupported, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->orientationsSupported, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mediaSizesSupported, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mediaTypesSupported, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mediaLoaded, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->printQualitySupported, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->queuedJobCount, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->imageFormatsSupported, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->basicTextPageWidth, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->basicTextPageHeight, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->printerGeneralCurrentOperator, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPS_GET_PRINTER_ATTRIBS_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_JOB_ATTRIBS_IND
CsrSize CsrBtBppsGetJobAttribsIndSizeof(void *msg)
{
    CsrBtBppsGetJobAttribsInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->jobAttribsLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPS_GET_JOB_ATTRIBS_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_JOB_ATTRIBS_IND
CsrUint8 *CsrBtBppsGetJobAttribsIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppsGetJobAttribsInd *primitive;

    primitive = (CsrBtBppsGetJobAttribsInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->jobId);
    CsrMemCpySer(buffer, length, &primitive->jobAttribs, sizeof(primitive->jobAttribs));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->jobAttribsCount);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->jobAttribsLength);
    if(primitive->jobAttribs)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->jobAttribs), ((CsrSize) (primitive->jobAttribsLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPS_GET_JOB_ATTRIBS_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_JOB_ATTRIBS_IND
void *CsrBtBppsGetJobAttribsIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppsGetJobAttribsInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppsGetJobAttribsInd *) CsrPmemZalloc(sizeof(CsrBtBppsGetJobAttribsInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->jobId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->jobAttribs), buffer, &offset, ((CsrSize) sizeof(primitive->jobAttribs)));
    CsrUint8Des((CsrUint8 *) &primitive->jobAttribsCount, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->jobAttribsLength, buffer, &offset);
    if (primitive->jobAttribsLength)
    {
        primitive->jobAttribs = CsrPmemZalloc(((CsrUint32) (primitive->jobAttribsLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->jobAttribs), buffer, &offset, ((CsrSize) (primitive->jobAttribsLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->jobAttribs = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPS_GET_JOB_ATTRIBS_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_JOB_ATTRIBS_IND
void CsrBtBppsGetJobAttribsIndSerFree(void *msg)
{
    CsrBtBppsGetJobAttribsInd *primitive;

    primitive = (CsrBtBppsGetJobAttribsInd *) msg;

    if(primitive->jobAttribs != NULL)
    {
        CsrPmemFree(primitive->jobAttribs);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BPPS_GET_JOB_ATTRIBS_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_CONNECT_RES
CsrSize CsrBtBppsConnectResSizeof(void *msg)
{
    CsrBtBppsConnectRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPS_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_BPPS_CONNECT_RES
CsrUint8 *CsrBtBppsConnectResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppsConnectRes *primitive;

    primitive = (CsrBtBppsConnectRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPS_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_BPPS_CONNECT_RES
void *CsrBtBppsConnectResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppsConnectRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppsConnectRes *) CsrPmemZalloc(sizeof(CsrBtBppsConnectRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPS_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_BPPS_DISCONNECT_IND
CsrSize CsrBtBppsDisconnectIndSizeof(void *msg)
{
    CsrBtBppsDisconnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPS_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_DISCONNECT_IND
CsrUint8 *CsrBtBppsDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppsDisconnectInd *primitive;

    primitive = (CsrBtBppsDisconnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPS_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_DISCONNECT_IND
void *CsrBtBppsDisconnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppsDisconnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppsDisconnectInd *) CsrPmemZalloc(sizeof(CsrBtBppsDisconnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPS_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_SEND_DOCUMENT_IND
CsrSize CsrBtBppsSendDocumentIndSizeof(void *msg)
{
    CsrBtBppsSendDocumentInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->documentType) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->fileName) + (primitive->documentBodyLength * sizeof(CsrUint8)) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->docTypeDependentInfo));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPS_SEND_DOCUMENT_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_SEND_DOCUMENT_IND
CsrUint8 *CsrBtBppsSendDocumentIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppsSendDocumentInd *primitive;

    primitive = (CsrBtBppsSendDocumentInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->documentTypeLength);
    CsrMemCpySer(buffer, length, &primitive->documentType, sizeof(primitive->documentType));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->jobId);
    CsrMemCpySer(buffer, length, &primitive->fileName, sizeof(primitive->fileName));
    CsrMemCpySer(buffer, length, &primitive->documentBody, sizeof(primitive->documentBody));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->documentBodyLength);
    CsrMemCpySer(buffer, length, &primitive->docTypeDependentInfo, sizeof(primitive->docTypeDependentInfo));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->docTypeDependentInfoLength);
    CsrUtf8StringSer(buffer, length, primitive->documentType);
    CsrUtf8StringSer(buffer, length, primitive->fileName);
    if(primitive->documentBody)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->documentBody), ((CsrSize) (primitive->documentBodyLength * sizeof(CsrUint8))));
    }
    CsrUtf8StringSer(buffer, length, primitive->docTypeDependentInfo);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPS_SEND_DOCUMENT_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_SEND_DOCUMENT_IND
void *CsrBtBppsSendDocumentIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppsSendDocumentInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppsSendDocumentInd *) CsrPmemZalloc(sizeof(CsrBtBppsSendDocumentInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->documentTypeLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->documentType), buffer, &offset, ((CsrSize) sizeof(primitive->documentType)));
    CsrUint32Des((CsrUint32 *) &primitive->jobId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->fileName), buffer, &offset, ((CsrSize) sizeof(primitive->fileName)));
    CsrMemCpyDes(((void *) &primitive->documentBody), buffer, &offset, ((CsrSize) sizeof(primitive->documentBody)));
    CsrUint32Des((CsrUint32 *) &primitive->documentBodyLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->docTypeDependentInfo), buffer, &offset, ((CsrSize) sizeof(primitive->docTypeDependentInfo)));
    CsrUint16Des((CsrUint16 *) &primitive->docTypeDependentInfoLength, buffer, &offset);
    CsrUtf8StringDes(&primitive->documentType, buffer, &offset);
    CsrUtf8StringDes(&primitive->fileName, buffer, &offset);
    if (primitive->documentBodyLength)
    {
        primitive->documentBody = CsrPmemZalloc(((CsrUint32) (primitive->documentBodyLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->documentBody), buffer, &offset, ((CsrSize) (primitive->documentBodyLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->documentBody = NULL;
    }
    CsrUtf8StringDes(&primitive->docTypeDependentInfo, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPS_SEND_DOCUMENT_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_SEND_DOCUMENT_IND
void CsrBtBppsSendDocumentIndSerFree(void *msg)
{
    CsrBtBppsSendDocumentInd *primitive;

    primitive = (CsrBtBppsSendDocumentInd *) msg;

    if(primitive->documentType != NULL)
    {
        CsrPmemFree(primitive->documentType);
    }
    if(primitive->fileName != NULL)
    {
        CsrPmemFree(primitive->fileName);
    }
    if(primitive->documentBody != NULL)
    {
        CsrPmemFree(primitive->documentBody);
    }
    if(primitive->docTypeDependentInfo != NULL)
    {
        CsrPmemFree(primitive->docTypeDependentInfo);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BPPS_SEND_DOCUMENT_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_NEXT_IND
CsrSize CsrBtBppsNextIndSizeof(void *msg)
{
    CsrBtBppsNextInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->bodyLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPS_NEXT_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_NEXT_IND
CsrUint8 *CsrBtBppsNextIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppsNextInd *primitive;

    primitive = (CsrBtBppsNextInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrMemCpySer(buffer, length, &primitive->body, sizeof(primitive->body));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyLength);
    if(primitive->body)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->body), ((CsrSize) (primitive->bodyLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPS_NEXT_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_NEXT_IND
void *CsrBtBppsNextIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppsNextInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppsNextInd *) CsrPmemZalloc(sizeof(CsrBtBppsNextInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->body), buffer, &offset, ((CsrSize) sizeof(primitive->body)));
    CsrUint16Des((CsrUint16 *) &primitive->bodyLength, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_BPPS_NEXT_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_NEXT_IND
void CsrBtBppsNextIndSerFree(void *msg)
{
    CsrBtBppsNextInd *primitive;

    primitive = (CsrBtBppsNextInd *) msg;

    if(primitive->body != NULL)
    {
        CsrPmemFree(primitive->body);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BPPS_NEXT_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_SECURITY_IN_REQ
CsrSize CsrBtBppsSecurityInReqSizeof(void *msg)
{
    CsrBtBppsSecurityInReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPS_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_BPPS_SECURITY_IN_REQ
CsrUint8 *CsrBtBppsSecurityInReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppsSecurityInReq *primitive;

    primitive = (CsrBtBppsSecurityInReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPS_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_BPPS_SECURITY_IN_REQ
void *CsrBtBppsSecurityInReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppsSecurityInReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppsSecurityInReq *) CsrPmemZalloc(sizeof(CsrBtBppsSecurityInReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPS_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_BPPS_DEACTIVATE_REQ
CsrSize CsrBtBppsDeactivateReqSizeof(void *msg)
{
    CsrBtBppsDeactivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPS_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_BPPS_DEACTIVATE_REQ
CsrUint8 *CsrBtBppsDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppsDeactivateReq *primitive;

    primitive = (CsrBtBppsDeactivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPS_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_BPPS_DEACTIVATE_REQ
void *CsrBtBppsDeactivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppsDeactivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppsDeactivateReq *) CsrPmemZalloc(sizeof(CsrBtBppsDeactivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPS_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_BPPS_SECURITY_IN_CFM
CsrSize CsrBtBppsSecurityInCfmSizeof(void *msg)
{
    CsrBtBppsSecurityInCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPS_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_BPPS_SECURITY_IN_CFM
CsrUint8 *CsrBtBppsSecurityInCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppsSecurityInCfm *primitive;

    primitive = (CsrBtBppsSecurityInCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPS_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_BPPS_SECURITY_IN_CFM
void *CsrBtBppsSecurityInCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppsSecurityInCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppsSecurityInCfm *) CsrPmemZalloc(sizeof(CsrBtBppsSecurityInCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPS_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_BPPS_CANCEL_JOB_IND
CsrSize CsrBtBppsCancelJobIndSizeof(void *msg)
{
    CsrBtBppsCancelJobInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPS_CANCEL_JOB_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_CANCEL_JOB_IND
CsrUint8 *CsrBtBppsCancelJobIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppsCancelJobInd *primitive;

    primitive = (CsrBtBppsCancelJobInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->jobId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPS_CANCEL_JOB_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_CANCEL_JOB_IND
void *CsrBtBppsCancelJobIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppsCancelJobInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppsCancelJobInd *) CsrPmemZalloc(sizeof(CsrBtBppsCancelJobInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->jobId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPS_CANCEL_JOB_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_REQ
CsrSize CsrBtBppsAuthenticateReqSizeof(void *msg)
{
    CsrBtBppsAuthenticateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->realmLength * sizeof(CsrUint8)) + (primitive->passwordLength * sizeof(CsrUint8)) + CsrCharStringSerLen((CsrCharString *) primitive->userId));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_REQ */

#ifndef EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_REQ
CsrUint8 *CsrBtBppsAuthenticateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppsAuthenticateReq *primitive;

    primitive = (CsrBtBppsAuthenticateReq *) msg;
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
#endif /* EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_REQ */

#ifndef EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_REQ
void *CsrBtBppsAuthenticateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppsAuthenticateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppsAuthenticateReq *) CsrPmemZalloc(sizeof(CsrBtBppsAuthenticateReq));

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
#endif /* EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_REQ */

#ifndef EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_REQ
void CsrBtBppsAuthenticateReqSerFree(void *msg)
{
    CsrBtBppsAuthenticateReq *primitive;

    primitive = (CsrBtBppsAuthenticateReq *) msg;

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
#endif /* EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_REQ */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_CFM
CsrSize CsrBtBppsGetReferencedObjCfmSizeof(void *msg)
{
    CsrBtBppsGetReferencedObjCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_CFM */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_CFM
CsrUint8 *CsrBtBppsGetReferencedObjCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppsGetReferencedObjCfm *primitive;

    primitive = (CsrBtBppsGetReferencedObjCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->filesize);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_CFM */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_CFM
void *CsrBtBppsGetReferencedObjCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppsGetReferencedObjCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppsGetReferencedObjCfm *) CsrPmemZalloc(sizeof(CsrBtBppsGetReferencedObjCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->payloadLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    CsrUint32Des((CsrUint32 *) &primitive->filesize, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_CFM */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_CFM
void CsrBtBppsGetReferencedObjCfmSerFree(void *msg)
{
    CsrBtBppsGetReferencedObjCfm *primitive;

    primitive = (CsrBtBppsGetReferencedObjCfm *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_CFM */

#ifndef EXCLUDE_CSR_BT_BPPS_CONNECT_IND
CsrSize CsrBtBppsConnectIndSizeof(void *msg)
{
    CsrBtBppsConnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPS_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_CONNECT_IND
CsrUint8 *CsrBtBppsConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppsConnectInd *primitive;

    primitive = (CsrBtBppsConnectInd *) msg;
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
#endif /* EXCLUDE_CSR_BT_BPPS_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_CONNECT_IND
void *CsrBtBppsConnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppsConnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppsConnectInd *) CsrPmemZalloc(sizeof(CsrBtBppsConnectInd));

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
#endif /* EXCLUDE_CSR_BT_BPPS_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_CFM
CsrSize CsrBtBppsAuthenticateCfmSizeof(void *msg)
{
    CsrBtBppsAuthenticateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_CFM */

#ifndef EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_CFM
CsrUint8 *CsrBtBppsAuthenticateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppsAuthenticateCfm *primitive;

    primitive = (CsrBtBppsAuthenticateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_CFM */

#ifndef EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_CFM
void *CsrBtBppsAuthenticateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppsAuthenticateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppsAuthenticateCfm *) CsrPmemZalloc(sizeof(CsrBtBppsAuthenticateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_CFM */

#ifndef EXCLUDE_CSR_BT_BPPS_ABORT_IND
CsrSize CsrBtBppsAbortIndSizeof(void *msg)
{
    CsrBtBppsAbortInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPS_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_ABORT_IND
CsrUint8 *CsrBtBppsAbortIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppsAbortInd *primitive;

    primitive = (CsrBtBppsAbortInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPS_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_ABORT_IND
void *CsrBtBppsAbortIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppsAbortInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppsAbortInd *) CsrPmemZalloc(sizeof(CsrBtBppsAbortInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPS_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_PRINTER_ATTRIBS_RES
CsrSize CsrBtBppsGetPrinterAttribsResSizeof(void *msg)
{
    CsrBtBppsGetPrinterAttribsRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->printerName) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->printerLocation) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->printerState) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->printerStateReasons) + (primitive->documentFormatsSupportedLength * sizeof(CsrUint8)) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->colorSupported) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->maxCopiesSupported) + (primitive->sidesSupportedLength * sizeof(CsrUint8)) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->numberUpSupported) + (primitive->orientationsSupportedLength * sizeof(CsrUint8)) + (primitive->mediaSizesSupportedLength * sizeof(CsrUint8)) + (primitive->mediaTypesSupportedLength * sizeof(CsrUint8)) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->mediaLoaded) + (primitive->printQualitySupportedLength * sizeof(CsrUint8)) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->queuedJobCount) + (primitive->imageFormatsSupportedLength * sizeof(CsrUint8)) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->basicTextPageWidth) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->basicTextPageHeight) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->printerGeneralCurrentOperator));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPS_GET_PRINTER_ATTRIBS_RES */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_PRINTER_ATTRIBS_RES
CsrUint8 *CsrBtBppsGetPrinterAttribsResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppsGetPrinterAttribsRes *primitive;

    primitive = (CsrBtBppsGetPrinterAttribsRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->printerName, sizeof(primitive->printerName));
    CsrMemCpySer(buffer, length, &primitive->printerLocation, sizeof(primitive->printerLocation));
    CsrMemCpySer(buffer, length, &primitive->printerState, sizeof(primitive->printerState));
    CsrMemCpySer(buffer, length, &primitive->printerStateReasons, sizeof(primitive->printerStateReasons));
    CsrMemCpySer(buffer, length, &primitive->documentFormatsSupported, sizeof(primitive->documentFormatsSupported));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->documentFormatsSupportedCount);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->documentFormatsSupportedLength);
    CsrMemCpySer(buffer, length, &primitive->colorSupported, sizeof(primitive->colorSupported));
    CsrMemCpySer(buffer, length, &primitive->maxCopiesSupported, sizeof(primitive->maxCopiesSupported));
    CsrMemCpySer(buffer, length, &primitive->sidesSupported, sizeof(primitive->sidesSupported));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->sidesSupportedCount);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->sidesSupportedLength);
    CsrMemCpySer(buffer, length, &primitive->numberUpSupported, sizeof(primitive->numberUpSupported));
    CsrMemCpySer(buffer, length, &primitive->orientationsSupported, sizeof(primitive->orientationsSupported));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->orientationsSupportedCount);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->orientationsSupportedLength);
    CsrMemCpySer(buffer, length, &primitive->mediaSizesSupported, sizeof(primitive->mediaSizesSupported));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mediaSizesSupportedCount);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mediaSizesSupportedLength);
    CsrMemCpySer(buffer, length, &primitive->mediaTypesSupported, sizeof(primitive->mediaTypesSupported));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mediaTypesSupportedCount);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mediaTypesSupportedLength);
    CsrMemCpySer(buffer, length, &primitive->mediaLoaded, sizeof(primitive->mediaLoaded));
    CsrMemCpySer(buffer, length, &primitive->printQualitySupported, sizeof(primitive->printQualitySupported));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->printQualitySupportedCount);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->printQualitySupportedLength);
    CsrMemCpySer(buffer, length, &primitive->queuedJobCount, sizeof(primitive->queuedJobCount));
    CsrMemCpySer(buffer, length, &primitive->imageFormatsSupported, sizeof(primitive->imageFormatsSupported));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->imageFormatsSupportedCount);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->imageFormatsSupportedLength);
    CsrMemCpySer(buffer, length, &primitive->basicTextPageWidth, sizeof(primitive->basicTextPageWidth));
    CsrMemCpySer(buffer, length, &primitive->basicTextPageHeight, sizeof(primitive->basicTextPageHeight));
    CsrMemCpySer(buffer, length, &primitive->printerGeneralCurrentOperator, sizeof(primitive->printerGeneralCurrentOperator));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mediaLoadedCount);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mediaLoadedLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->operationStatus);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    CsrUtf8StringSer(buffer, length, primitive->printerName);
    CsrUtf8StringSer(buffer, length, primitive->printerLocation);
    CsrUtf8StringSer(buffer, length, primitive->printerState);
    CsrUtf8StringSer(buffer, length, primitive->printerStateReasons);
    if(primitive->documentFormatsSupported)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->documentFormatsSupported), ((CsrSize) (primitive->documentFormatsSupportedLength * sizeof(CsrUint8))));
    }
    CsrUtf8StringSer(buffer, length, primitive->colorSupported);
    CsrUtf8StringSer(buffer, length, primitive->maxCopiesSupported);
    if(primitive->sidesSupported)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->sidesSupported), ((CsrSize) (primitive->sidesSupportedLength * sizeof(CsrUint8))));
    }
    CsrUtf8StringSer(buffer, length, primitive->numberUpSupported);
    if(primitive->orientationsSupported)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->orientationsSupported), ((CsrSize) (primitive->orientationsSupportedLength * sizeof(CsrUint8))));
    }
    if(primitive->mediaSizesSupported)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->mediaSizesSupported), ((CsrSize) (primitive->mediaSizesSupportedLength * sizeof(CsrUint8))));
    }
    if(primitive->mediaTypesSupported)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->mediaTypesSupported), ((CsrSize) (primitive->mediaTypesSupportedLength * sizeof(CsrUint8))));
    }
    CsrUtf8StringSer(buffer, length, primitive->mediaLoaded);
    if(primitive->printQualitySupported)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->printQualitySupported), ((CsrSize) (primitive->printQualitySupportedLength * sizeof(CsrUint8))));
    }
    CsrUtf8StringSer(buffer, length, primitive->queuedJobCount);
    if(primitive->imageFormatsSupported)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->imageFormatsSupported), ((CsrSize) (primitive->imageFormatsSupportedLength * sizeof(CsrUint8))));
    }
    CsrUtf8StringSer(buffer, length, primitive->basicTextPageWidth);
    CsrUtf8StringSer(buffer, length, primitive->basicTextPageHeight);
    CsrUtf8StringSer(buffer, length, primitive->printerGeneralCurrentOperator);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPS_GET_PRINTER_ATTRIBS_RES */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_PRINTER_ATTRIBS_RES
void *CsrBtBppsGetPrinterAttribsResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppsGetPrinterAttribsRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppsGetPrinterAttribsRes *) CsrPmemZalloc(sizeof(CsrBtBppsGetPrinterAttribsRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->printerName), buffer, &offset, ((CsrSize) sizeof(primitive->printerName)));
    CsrMemCpyDes(((void *) &primitive->printerLocation), buffer, &offset, ((CsrSize) sizeof(primitive->printerLocation)));
    CsrMemCpyDes(((void *) &primitive->printerState), buffer, &offset, ((CsrSize) sizeof(primitive->printerState)));
    CsrMemCpyDes(((void *) &primitive->printerStateReasons), buffer, &offset, ((CsrSize) sizeof(primitive->printerStateReasons)));
    CsrMemCpyDes(((void *) &primitive->documentFormatsSupported), buffer, &offset, ((CsrSize) sizeof(primitive->documentFormatsSupported)));
    CsrUint8Des((CsrUint8 *) &primitive->documentFormatsSupportedCount, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->documentFormatsSupportedLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->colorSupported), buffer, &offset, ((CsrSize) sizeof(primitive->colorSupported)));
    CsrMemCpyDes(((void *) &primitive->maxCopiesSupported), buffer, &offset, ((CsrSize) sizeof(primitive->maxCopiesSupported)));
    CsrMemCpyDes(((void *) &primitive->sidesSupported), buffer, &offset, ((CsrSize) sizeof(primitive->sidesSupported)));
    CsrUint8Des((CsrUint8 *) &primitive->sidesSupportedCount, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->sidesSupportedLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->numberUpSupported), buffer, &offset, ((CsrSize) sizeof(primitive->numberUpSupported)));
    CsrMemCpyDes(((void *) &primitive->orientationsSupported), buffer, &offset, ((CsrSize) sizeof(primitive->orientationsSupported)));
    CsrUint8Des((CsrUint8 *) &primitive->orientationsSupportedCount, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->orientationsSupportedLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->mediaSizesSupported), buffer, &offset, ((CsrSize) sizeof(primitive->mediaSizesSupported)));
    CsrUint8Des((CsrUint8 *) &primitive->mediaSizesSupportedCount, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mediaSizesSupportedLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->mediaTypesSupported), buffer, &offset, ((CsrSize) sizeof(primitive->mediaTypesSupported)));
    CsrUint8Des((CsrUint8 *) &primitive->mediaTypesSupportedCount, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mediaTypesSupportedLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->mediaLoaded), buffer, &offset, ((CsrSize) sizeof(primitive->mediaLoaded)));
    CsrMemCpyDes(((void *) &primitive->printQualitySupported), buffer, &offset, ((CsrSize) sizeof(primitive->printQualitySupported)));
    CsrUint8Des((CsrUint8 *) &primitive->printQualitySupportedCount, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->printQualitySupportedLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->queuedJobCount), buffer, &offset, ((CsrSize) sizeof(primitive->queuedJobCount)));
    CsrMemCpyDes(((void *) &primitive->imageFormatsSupported), buffer, &offset, ((CsrSize) sizeof(primitive->imageFormatsSupported)));
    CsrUint8Des((CsrUint8 *) &primitive->imageFormatsSupportedCount, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->imageFormatsSupportedLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->basicTextPageWidth), buffer, &offset, ((CsrSize) sizeof(primitive->basicTextPageWidth)));
    CsrMemCpyDes(((void *) &primitive->basicTextPageHeight), buffer, &offset, ((CsrSize) sizeof(primitive->basicTextPageHeight)));
    CsrMemCpyDes(((void *) &primitive->printerGeneralCurrentOperator), buffer, &offset, ((CsrSize) sizeof(primitive->printerGeneralCurrentOperator)));
    CsrUint8Des((CsrUint8 *) &primitive->mediaLoadedCount, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mediaLoadedLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->operationStatus, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    CsrUtf8StringDes(&primitive->printerName, buffer, &offset);
    CsrUtf8StringDes(&primitive->printerLocation, buffer, &offset);
    CsrUtf8StringDes(&primitive->printerState, buffer, &offset);
    CsrUtf8StringDes(&primitive->printerStateReasons, buffer, &offset);
    if (primitive->documentFormatsSupportedLength)
    {
        primitive->documentFormatsSupported = CsrPmemZalloc(((CsrUint32) (primitive->documentFormatsSupportedLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->documentFormatsSupported), buffer, &offset, ((CsrSize) (primitive->documentFormatsSupportedLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->documentFormatsSupported = NULL;
    }
    CsrUtf8StringDes(&primitive->colorSupported, buffer, &offset);
    CsrUtf8StringDes(&primitive->maxCopiesSupported, buffer, &offset);
    if (primitive->sidesSupportedLength)
    {
        primitive->sidesSupported = CsrPmemZalloc(((CsrUint32) (primitive->sidesSupportedLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->sidesSupported), buffer, &offset, ((CsrSize) (primitive->sidesSupportedLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->sidesSupported = NULL;
    }
    CsrUtf8StringDes(&primitive->numberUpSupported, buffer, &offset);
    if (primitive->orientationsSupportedLength)
    {
        primitive->orientationsSupported = CsrPmemZalloc(((CsrUint32) (primitive->orientationsSupportedLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->orientationsSupported), buffer, &offset, ((CsrSize) (primitive->orientationsSupportedLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->orientationsSupported = NULL;
    }
    if (primitive->mediaSizesSupportedLength)
    {
        primitive->mediaSizesSupported = CsrPmemZalloc(((CsrUint32) (primitive->mediaSizesSupportedLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->mediaSizesSupported), buffer, &offset, ((CsrSize) (primitive->mediaSizesSupportedLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->mediaSizesSupported = NULL;
    }
    if (primitive->mediaTypesSupportedLength)
    {
        primitive->mediaTypesSupported = CsrPmemZalloc(((CsrUint32) (primitive->mediaTypesSupportedLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->mediaTypesSupported), buffer, &offset, ((CsrSize) (primitive->mediaTypesSupportedLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->mediaTypesSupported = NULL;
    }
    CsrUtf8StringDes(&primitive->mediaLoaded, buffer, &offset);
    if (primitive->printQualitySupportedLength)
    {
        primitive->printQualitySupported = CsrPmemZalloc(((CsrUint32) (primitive->printQualitySupportedLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->printQualitySupported), buffer, &offset, ((CsrSize) (primitive->printQualitySupportedLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->printQualitySupported = NULL;
    }
    CsrUtf8StringDes(&primitive->queuedJobCount, buffer, &offset);
    if (primitive->imageFormatsSupportedLength)
    {
        primitive->imageFormatsSupported = CsrPmemZalloc(((CsrUint32) (primitive->imageFormatsSupportedLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->imageFormatsSupported), buffer, &offset, ((CsrSize) (primitive->imageFormatsSupportedLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->imageFormatsSupported = NULL;
    }
    CsrUtf8StringDes(&primitive->basicTextPageWidth, buffer, &offset);
    CsrUtf8StringDes(&primitive->basicTextPageHeight, buffer, &offset);
    CsrUtf8StringDes(&primitive->printerGeneralCurrentOperator, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPS_GET_PRINTER_ATTRIBS_RES */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_PRINTER_ATTRIBS_RES
void CsrBtBppsGetPrinterAttribsResSerFree(void *msg)
{
    CsrBtBppsGetPrinterAttribsRes *primitive;

    primitive = (CsrBtBppsGetPrinterAttribsRes *) msg;

    if(primitive->printerName != NULL)
    {
        CsrPmemFree(primitive->printerName);
    }
    if(primitive->printerLocation != NULL)
    {
        CsrPmemFree(primitive->printerLocation);
    }
    if(primitive->printerState != NULL)
    {
        CsrPmemFree(primitive->printerState);
    }
    if(primitive->printerStateReasons != NULL)
    {
        CsrPmemFree(primitive->printerStateReasons);
    }
    if(primitive->documentFormatsSupported != NULL)
    {
        CsrPmemFree(primitive->documentFormatsSupported);
    }
    if(primitive->colorSupported != NULL)
    {
        CsrPmemFree(primitive->colorSupported);
    }
    if(primitive->maxCopiesSupported != NULL)
    {
        CsrPmemFree(primitive->maxCopiesSupported);
    }
    if(primitive->sidesSupported != NULL)
    {
        CsrPmemFree(primitive->sidesSupported);
    }
    if(primitive->numberUpSupported != NULL)
    {
        CsrPmemFree(primitive->numberUpSupported);
    }
    if(primitive->orientationsSupported != NULL)
    {
        CsrPmemFree(primitive->orientationsSupported);
    }
    if(primitive->mediaSizesSupported != NULL)
    {
        CsrPmemFree(primitive->mediaSizesSupported);
    }
    if(primitive->mediaTypesSupported != NULL)
    {
        CsrPmemFree(primitive->mediaTypesSupported);
    }
    if(primitive->mediaLoaded != NULL)
    {
        CsrPmemFree(primitive->mediaLoaded);
    }
    if(primitive->printQualitySupported != NULL)
    {
        CsrPmemFree(primitive->printQualitySupported);
    }
    if(primitive->queuedJobCount != NULL)
    {
        CsrPmemFree(primitive->queuedJobCount);
    }
    if(primitive->imageFormatsSupported != NULL)
    {
        CsrPmemFree(primitive->imageFormatsSupported);
    }
    if(primitive->basicTextPageWidth != NULL)
    {
        CsrPmemFree(primitive->basicTextPageWidth);
    }
    if(primitive->basicTextPageHeight != NULL)
    {
        CsrPmemFree(primitive->basicTextPageHeight);
    }
    if(primitive->printerGeneralCurrentOperator != NULL)
    {
        CsrPmemFree(primitive->printerGeneralCurrentOperator);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BPPS_GET_PRINTER_ATTRIBS_RES */

#ifndef EXCLUDE_CSR_BT_BPPS_ACTIVATE_REQ
CsrSize CsrBtBppsActivateReqSizeof(void *msg)
{
    CsrBtBppsActivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->documentFormatsSupported) + (primitive->characterRepertoiresSupportedLength * sizeof(CsrUint8)) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->imageFormatsSupported) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->ieee1284Id));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPS_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_BPPS_ACTIVATE_REQ
CsrUint8 *CsrBtBppsActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppsActivateReq *primitive;

    primitive = (CsrBtBppsActivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrMemCpySer(buffer, length, &primitive->documentFormatsSupported, sizeof(primitive->documentFormatsSupported));
    CsrMemCpySer(buffer, length, &primitive->characterRepertoiresSupported, sizeof(primitive->characterRepertoiresSupported));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->characterRepertoiresSupportedLength);
    CsrMemCpySer(buffer, length, &primitive->imageFormatsSupported, sizeof(primitive->imageFormatsSupported));
    CsrMemCpySer(buffer, length, &primitive->ieee1284Id, sizeof(primitive->ieee1284Id));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->obexMaxPacketSize);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->windowSize);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmEnable);
    CsrUtf8StringSer(buffer, length, primitive->documentFormatsSupported);
    if(primitive->characterRepertoiresSupported)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->characterRepertoiresSupported), ((CsrSize) (primitive->characterRepertoiresSupportedLength * sizeof(CsrUint8))));
    }
    CsrUtf8StringSer(buffer, length, primitive->imageFormatsSupported);
    CsrUtf8StringSer(buffer, length, primitive->ieee1284Id);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPS_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_BPPS_ACTIVATE_REQ
void *CsrBtBppsActivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppsActivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppsActivateReq *) CsrPmemZalloc(sizeof(CsrBtBppsActivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->documentFormatsSupported), buffer, &offset, ((CsrSize) sizeof(primitive->documentFormatsSupported)));
    CsrMemCpyDes(((void *) &primitive->characterRepertoiresSupported), buffer, &offset, ((CsrSize) sizeof(primitive->characterRepertoiresSupported)));
    CsrUint16Des((CsrUint16 *) &primitive->characterRepertoiresSupportedLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->imageFormatsSupported), buffer, &offset, ((CsrSize) sizeof(primitive->imageFormatsSupported)));
    CsrMemCpyDes(((void *) &primitive->ieee1284Id), buffer, &offset, ((CsrSize) sizeof(primitive->ieee1284Id)));
    CsrUint16Des((CsrUint16 *) &primitive->obexMaxPacketSize, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->windowSize, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmEnable, buffer, &offset);
    CsrUtf8StringDes(&primitive->documentFormatsSupported, buffer, &offset);
    if (primitive->characterRepertoiresSupportedLength)
    {
        primitive->characterRepertoiresSupported = CsrPmemZalloc(((CsrUint32) (primitive->characterRepertoiresSupportedLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->characterRepertoiresSupported), buffer, &offset, ((CsrSize) (primitive->characterRepertoiresSupportedLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->characterRepertoiresSupported = NULL;
    }
    CsrUtf8StringDes(&primitive->imageFormatsSupported, buffer, &offset);
    CsrUtf8StringDes(&primitive->ieee1284Id, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPS_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_BPPS_ACTIVATE_REQ
void CsrBtBppsActivateReqSerFree(void *msg)
{
    CsrBtBppsActivateReq *primitive;

    primitive = (CsrBtBppsActivateReq *) msg;

    if(primitive->documentFormatsSupported != NULL)
    {
        CsrPmemFree(primitive->documentFormatsSupported);
    }
    if(primitive->characterRepertoiresSupported != NULL)
    {
        CsrPmemFree(primitive->characterRepertoiresSupported);
    }
    if(primitive->imageFormatsSupported != NULL)
    {
        CsrPmemFree(primitive->imageFormatsSupported);
    }
    if(primitive->ieee1284Id != NULL)
    {
        CsrPmemFree(primitive->ieee1284Id);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BPPS_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_JOB_ATTRIBS_RES
CsrSize CsrBtBppsGetJobAttribsResSizeof(void *msg)
{
    CsrBtBppsGetJobAttribsRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->jobState) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->jobName) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->jobOriginatingUserName));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPS_GET_JOB_ATTRIBS_RES */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_JOB_ATTRIBS_RES
CsrUint8 *CsrBtBppsGetJobAttribsResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppsGetJobAttribsRes *primitive;

    primitive = (CsrBtBppsGetJobAttribsRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->jobId);
    CsrMemCpySer(buffer, length, &primitive->jobState, sizeof(primitive->jobState));
    CsrMemCpySer(buffer, length, &primitive->jobName, sizeof(primitive->jobName));
    CsrMemCpySer(buffer, length, &primitive->jobOriginatingUserName, sizeof(primitive->jobOriginatingUserName));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->jobMediaSheetsCompleted);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->numberOfInterveningJobs);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->operationStatus);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    CsrUtf8StringSer(buffer, length, primitive->jobState);
    CsrUtf8StringSer(buffer, length, primitive->jobName);
    CsrUtf8StringSer(buffer, length, primitive->jobOriginatingUserName);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPS_GET_JOB_ATTRIBS_RES */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_JOB_ATTRIBS_RES
void *CsrBtBppsGetJobAttribsResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppsGetJobAttribsRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppsGetJobAttribsRes *) CsrPmemZalloc(sizeof(CsrBtBppsGetJobAttribsRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->jobId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->jobState), buffer, &offset, ((CsrSize) sizeof(primitive->jobState)));
    CsrMemCpyDes(((void *) &primitive->jobName), buffer, &offset, ((CsrSize) sizeof(primitive->jobName)));
    CsrMemCpyDes(((void *) &primitive->jobOriginatingUserName), buffer, &offset, ((CsrSize) sizeof(primitive->jobOriginatingUserName)));
    CsrUint32Des((CsrUint32 *) &primitive->jobMediaSheetsCompleted, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->numberOfInterveningJobs, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->operationStatus, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    CsrUtf8StringDes(&primitive->jobState, buffer, &offset);
    CsrUtf8StringDes(&primitive->jobName, buffer, &offset);
    CsrUtf8StringDes(&primitive->jobOriginatingUserName, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPS_GET_JOB_ATTRIBS_RES */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_JOB_ATTRIBS_RES
void CsrBtBppsGetJobAttribsResSerFree(void *msg)
{
    CsrBtBppsGetJobAttribsRes *primitive;

    primitive = (CsrBtBppsGetJobAttribsRes *) msg;

    if(primitive->jobState != NULL)
    {
        CsrPmemFree(primitive->jobState);
    }
    if(primitive->jobName != NULL)
    {
        CsrPmemFree(primitive->jobName);
    }
    if(primitive->jobOriginatingUserName != NULL)
    {
        CsrPmemFree(primitive->jobOriginatingUserName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BPPS_GET_JOB_ATTRIBS_RES */

#ifndef EXCLUDE_CSR_BT_BPPS_CREATE_JOB_IND
CsrSize CsrBtBppsCreateJobIndSizeof(void *msg)
{
    CsrBtBppsCreateJobInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->jobName) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->jobOriginatingUserName) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->documentFormat) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->copies) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->sides) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->numberUp) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->orientationRequested) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->mediaSize) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->mediaType) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->printQuality) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->cancelOnLostLink));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPS_CREATE_JOB_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_CREATE_JOB_IND
CsrUint8 *CsrBtBppsCreateJobIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppsCreateJobInd *primitive;

    primitive = (CsrBtBppsCreateJobInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->jobName, sizeof(primitive->jobName));
    CsrMemCpySer(buffer, length, &primitive->jobOriginatingUserName, sizeof(primitive->jobOriginatingUserName));
    CsrMemCpySer(buffer, length, &primitive->documentFormat, sizeof(primitive->documentFormat));
    CsrMemCpySer(buffer, length, &primitive->copies, sizeof(primitive->copies));
    CsrMemCpySer(buffer, length, &primitive->sides, sizeof(primitive->sides));
    CsrMemCpySer(buffer, length, &primitive->numberUp, sizeof(primitive->numberUp));
    CsrMemCpySer(buffer, length, &primitive->orientationRequested, sizeof(primitive->orientationRequested));
    CsrMemCpySer(buffer, length, &primitive->mediaSize, sizeof(primitive->mediaSize));
    CsrMemCpySer(buffer, length, &primitive->mediaType, sizeof(primitive->mediaType));
    CsrMemCpySer(buffer, length, &primitive->printQuality, sizeof(primitive->printQuality));
    CsrMemCpySer(buffer, length, &primitive->cancelOnLostLink, sizeof(primitive->cancelOnLostLink));
    CsrUtf8StringSer(buffer, length, primitive->jobName);
    CsrUtf8StringSer(buffer, length, primitive->jobOriginatingUserName);
    CsrUtf8StringSer(buffer, length, primitive->documentFormat);
    CsrUtf8StringSer(buffer, length, primitive->copies);
    CsrUtf8StringSer(buffer, length, primitive->sides);
    CsrUtf8StringSer(buffer, length, primitive->numberUp);
    CsrUtf8StringSer(buffer, length, primitive->orientationRequested);
    CsrUtf8StringSer(buffer, length, primitive->mediaSize);
    CsrUtf8StringSer(buffer, length, primitive->mediaType);
    CsrUtf8StringSer(buffer, length, primitive->printQuality);
    CsrUtf8StringSer(buffer, length, primitive->cancelOnLostLink);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPS_CREATE_JOB_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_CREATE_JOB_IND
void *CsrBtBppsCreateJobIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppsCreateJobInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppsCreateJobInd *) CsrPmemZalloc(sizeof(CsrBtBppsCreateJobInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->jobName), buffer, &offset, ((CsrSize) sizeof(primitive->jobName)));
    CsrMemCpyDes(((void *) &primitive->jobOriginatingUserName), buffer, &offset, ((CsrSize) sizeof(primitive->jobOriginatingUserName)));
    CsrMemCpyDes(((void *) &primitive->documentFormat), buffer, &offset, ((CsrSize) sizeof(primitive->documentFormat)));
    CsrMemCpyDes(((void *) &primitive->copies), buffer, &offset, ((CsrSize) sizeof(primitive->copies)));
    CsrMemCpyDes(((void *) &primitive->sides), buffer, &offset, ((CsrSize) sizeof(primitive->sides)));
    CsrMemCpyDes(((void *) &primitive->numberUp), buffer, &offset, ((CsrSize) sizeof(primitive->numberUp)));
    CsrMemCpyDes(((void *) &primitive->orientationRequested), buffer, &offset, ((CsrSize) sizeof(primitive->orientationRequested)));
    CsrMemCpyDes(((void *) &primitive->mediaSize), buffer, &offset, ((CsrSize) sizeof(primitive->mediaSize)));
    CsrMemCpyDes(((void *) &primitive->mediaType), buffer, &offset, ((CsrSize) sizeof(primitive->mediaType)));
    CsrMemCpyDes(((void *) &primitive->printQuality), buffer, &offset, ((CsrSize) sizeof(primitive->printQuality)));
    CsrMemCpyDes(((void *) &primitive->cancelOnLostLink), buffer, &offset, ((CsrSize) sizeof(primitive->cancelOnLostLink)));
    CsrUtf8StringDes(&primitive->jobName, buffer, &offset);
    CsrUtf8StringDes(&primitive->jobOriginatingUserName, buffer, &offset);
    CsrUtf8StringDes(&primitive->documentFormat, buffer, &offset);
    CsrUtf8StringDes(&primitive->copies, buffer, &offset);
    CsrUtf8StringDes(&primitive->sides, buffer, &offset);
    CsrUtf8StringDes(&primitive->numberUp, buffer, &offset);
    CsrUtf8StringDes(&primitive->orientationRequested, buffer, &offset);
    CsrUtf8StringDes(&primitive->mediaSize, buffer, &offset);
    CsrUtf8StringDes(&primitive->mediaType, buffer, &offset);
    CsrUtf8StringDes(&primitive->printQuality, buffer, &offset);
    CsrUtf8StringDes(&primitive->cancelOnLostLink, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPS_CREATE_JOB_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_CREATE_JOB_IND
void CsrBtBppsCreateJobIndSerFree(void *msg)
{
    CsrBtBppsCreateJobInd *primitive;

    primitive = (CsrBtBppsCreateJobInd *) msg;

    if(primitive->jobName != NULL)
    {
        CsrPmemFree(primitive->jobName);
    }
    if(primitive->jobOriginatingUserName != NULL)
    {
        CsrPmemFree(primitive->jobOriginatingUserName);
    }
    if(primitive->documentFormat != NULL)
    {
        CsrPmemFree(primitive->documentFormat);
    }
    if(primitive->copies != NULL)
    {
        CsrPmemFree(primitive->copies);
    }
    if(primitive->sides != NULL)
    {
        CsrPmemFree(primitive->sides);
    }
    if(primitive->numberUp != NULL)
    {
        CsrPmemFree(primitive->numberUp);
    }
    if(primitive->orientationRequested != NULL)
    {
        CsrPmemFree(primitive->orientationRequested);
    }
    if(primitive->mediaSize != NULL)
    {
        CsrPmemFree(primitive->mediaSize);
    }
    if(primitive->mediaType != NULL)
    {
        CsrPmemFree(primitive->mediaType);
    }
    if(primitive->printQuality != NULL)
    {
        CsrPmemFree(primitive->printQuality);
    }
    if(primitive->cancelOnLostLink != NULL)
    {
        CsrPmemFree(primitive->cancelOnLostLink);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BPPS_CREATE_JOB_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_IND
CsrSize CsrBtBppsGetReferencedObjIndSizeof(void *msg)
{
    CsrBtBppsGetReferencedObjInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_IND
CsrUint8 *CsrBtBppsGetReferencedObjIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppsGetReferencedObjInd *primitive;

    primitive = (CsrBtBppsGetReferencedObjInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->filesize);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyLength);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_IND
void *CsrBtBppsGetReferencedObjIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppsGetReferencedObjInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppsGetReferencedObjInd *) CsrPmemZalloc(sizeof(CsrBtBppsGetReferencedObjInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->payloadLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    CsrUint32Des((CsrUint32 *) &primitive->filesize, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyLength, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_IND
void CsrBtBppsGetReferencedObjIndSerFree(void *msg)
{
    CsrBtBppsGetReferencedObjInd *primitive;

    primitive = (CsrBtBppsGetReferencedObjInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_EVENT_RES
CsrSize CsrBtBppsGetEventResSizeof(void *msg)
{
    CsrBtBppsGetEventRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->jobState) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->printerState) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->printerStateReasons));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPS_GET_EVENT_RES */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_EVENT_RES
CsrUint8 *CsrBtBppsGetEventResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppsGetEventRes *primitive;

    primitive = (CsrBtBppsGetEventRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->jobId);
    CsrMemCpySer(buffer, length, &primitive->jobState, sizeof(primitive->jobState));
    CsrMemCpySer(buffer, length, &primitive->printerState, sizeof(primitive->printerState));
    CsrMemCpySer(buffer, length, &primitive->printerStateReasons, sizeof(primitive->printerStateReasons));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->operationStatus);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    CsrUtf8StringSer(buffer, length, primitive->jobState);
    CsrUtf8StringSer(buffer, length, primitive->printerState);
    CsrUtf8StringSer(buffer, length, primitive->printerStateReasons);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPS_GET_EVENT_RES */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_EVENT_RES
void *CsrBtBppsGetEventResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppsGetEventRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppsGetEventRes *) CsrPmemZalloc(sizeof(CsrBtBppsGetEventRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->jobId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->jobState), buffer, &offset, ((CsrSize) sizeof(primitive->jobState)));
    CsrMemCpyDes(((void *) &primitive->printerState), buffer, &offset, ((CsrSize) sizeof(primitive->printerState)));
    CsrMemCpyDes(((void *) &primitive->printerStateReasons), buffer, &offset, ((CsrSize) sizeof(primitive->printerStateReasons)));
    CsrUint16Des((CsrUint16 *) &primitive->operationStatus, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    CsrUtf8StringDes(&primitive->jobState, buffer, &offset);
    CsrUtf8StringDes(&primitive->printerState, buffer, &offset);
    CsrUtf8StringDes(&primitive->printerStateReasons, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPS_GET_EVENT_RES */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_EVENT_RES
void CsrBtBppsGetEventResSerFree(void *msg)
{
    CsrBtBppsGetEventRes *primitive;

    primitive = (CsrBtBppsGetEventRes *) msg;

    if(primitive->jobState != NULL)
    {
        CsrPmemFree(primitive->jobState);
    }
    if(primitive->printerState != NULL)
    {
        CsrPmemFree(primitive->printerState);
    }
    if(primitive->printerStateReasons != NULL)
    {
        CsrPmemFree(primitive->printerStateReasons);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BPPS_GET_EVENT_RES */

#ifndef EXCLUDE_CSR_BT_BPPS_JOB_COMPLETE_REQ
CsrSize CsrBtBppsJobCompleteReqSizeof(void *msg)
{
    CsrBtBppsJobCompleteReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPS_JOB_COMPLETE_REQ */

#ifndef EXCLUDE_CSR_BT_BPPS_JOB_COMPLETE_REQ
CsrUint8 *CsrBtBppsJobCompleteReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppsJobCompleteReq *primitive;

    primitive = (CsrBtBppsJobCompleteReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPS_JOB_COMPLETE_REQ */

#ifndef EXCLUDE_CSR_BT_BPPS_JOB_COMPLETE_REQ
void *CsrBtBppsJobCompleteReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppsJobCompleteReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppsJobCompleteReq *) CsrPmemZalloc(sizeof(CsrBtBppsJobCompleteReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPS_JOB_COMPLETE_REQ */

#ifndef EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_IND
CsrSize CsrBtBppsAuthenticateIndSizeof(void *msg)
{
    CsrBtBppsAuthenticateInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->realmLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_IND
CsrUint8 *CsrBtBppsAuthenticateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppsAuthenticateInd *primitive;

    primitive = (CsrBtBppsAuthenticateInd *) msg;
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
#endif /* EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_IND
void *CsrBtBppsAuthenticateIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppsAuthenticateInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppsAuthenticateInd *) CsrPmemZalloc(sizeof(CsrBtBppsAuthenticateInd));

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
#endif /* EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_IND
void CsrBtBppsAuthenticateIndSerFree(void *msg)
{
    CsrBtBppsAuthenticateInd *primitive;

    primitive = (CsrBtBppsAuthenticateInd *) msg;

    if(primitive->realm != NULL)
    {
        CsrPmemFree(primitive->realm);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_RES
CsrSize CsrBtBppsAuthenticateResSizeof(void *msg)
{
    CsrBtBppsAuthenticateRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->passwordLength * sizeof(CsrUint8)) + CsrCharStringSerLen((CsrCharString *) primitive->userId));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_RES
CsrUint8 *CsrBtBppsAuthenticateResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppsAuthenticateRes *primitive;

    primitive = (CsrBtBppsAuthenticateRes *) msg;
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
#endif /* EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_RES
void *CsrBtBppsAuthenticateResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppsAuthenticateRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppsAuthenticateRes *) CsrPmemZalloc(sizeof(CsrBtBppsAuthenticateRes));

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
#endif /* EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_RES
void CsrBtBppsAuthenticateResSerFree(void *msg)
{
    CsrBtBppsAuthenticateRes *primitive;

    primitive = (CsrBtBppsAuthenticateRes *) msg;

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
#endif /* EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_BPPS_CREATE_JOB_RES
CsrSize CsrBtBppsCreateJobResSizeof(void *msg)
{
    CsrBtBppsCreateJobRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPS_CREATE_JOB_RES */

#ifndef EXCLUDE_CSR_BT_BPPS_CREATE_JOB_RES
CsrUint8 *CsrBtBppsCreateJobResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppsCreateJobRes *primitive;

    primitive = (CsrBtBppsCreateJobRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->jobId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->operationStatus);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPS_CREATE_JOB_RES */

#ifndef EXCLUDE_CSR_BT_BPPS_CREATE_JOB_RES
void *CsrBtBppsCreateJobResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppsCreateJobRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppsCreateJobRes *) CsrPmemZalloc(sizeof(CsrBtBppsCreateJobRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->jobId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->operationStatus, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPS_CREATE_JOB_RES */

#ifndef EXCLUDE_CSR_BT_BPPS_CANCEL_JOB_RES
CsrSize CsrBtBppsCancelJobResSizeof(void *msg)
{
    CsrBtBppsCancelJobRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPS_CANCEL_JOB_RES */

#ifndef EXCLUDE_CSR_BT_BPPS_CANCEL_JOB_RES
CsrUint8 *CsrBtBppsCancelJobResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppsCancelJobRes *primitive;

    primitive = (CsrBtBppsCancelJobRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->jobId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->operationStatus);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPS_CANCEL_JOB_RES */

#ifndef EXCLUDE_CSR_BT_BPPS_CANCEL_JOB_RES
void *CsrBtBppsCancelJobResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppsCancelJobRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppsCancelJobRes *) CsrPmemZalloc(sizeof(CsrBtBppsCancelJobRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->jobId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->operationStatus, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPS_CANCEL_JOB_RES */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_RES
CsrSize CsrBtBppsGetReferencedObjResSizeof(void *msg)
{
    CsrBtBppsGetReferencedObjRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_RES */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_RES
CsrUint8 *CsrBtBppsGetReferencedObjResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppsGetReferencedObjRes *primitive;

    primitive = (CsrBtBppsGetReferencedObjRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_RES */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_RES
void *CsrBtBppsGetReferencedObjResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppsGetReferencedObjRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppsGetReferencedObjRes *) CsrPmemZalloc(sizeof(CsrBtBppsGetReferencedObjRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_RES */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_EVENT_IND
CsrSize CsrBtBppsGetEventIndSizeof(void *msg)
{
    CsrBtBppsGetEventInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPS_GET_EVENT_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_EVENT_IND
CsrUint8 *CsrBtBppsGetEventIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppsGetEventInd *primitive;

    primitive = (CsrBtBppsGetEventInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->jobId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->updateEvents);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPS_GET_EVENT_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_EVENT_IND
void *CsrBtBppsGetEventIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppsGetEventInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppsGetEventInd *) CsrPmemZalloc(sizeof(CsrBtBppsGetEventInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->jobId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->updateEvents, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPS_GET_EVENT_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_ACTIVATE_CFM
CsrSize CsrBtBppsActivateCfmSizeof(void *msg)
{
    CsrBtBppsActivateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPS_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_BPPS_ACTIVATE_CFM
CsrUint8 *CsrBtBppsActivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppsActivateCfm *primitive;

    primitive = (CsrBtBppsActivateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPS_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_BPPS_ACTIVATE_CFM
void *CsrBtBppsActivateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppsActivateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppsActivateCfm *) CsrPmemZalloc(sizeof(CsrBtBppsActivateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPS_ACTIVATE_CFM */

#endif /* EXCLUDE_CSR_BT_BPPS_MODULE */
