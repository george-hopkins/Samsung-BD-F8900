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
#include "csr_bt_bppc_prim.h"

#ifndef EXCLUDE_CSR_BT_BPPC_MODULE

void CsrBtBppcPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_BPPC_DEACTIVATE_CFM
CsrSize CsrBtBppcDeactivateCfmSizeof(void *msg)
{
    CsrBtBppcDeactivateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPC_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_BPPC_DEACTIVATE_CFM
CsrUint8 *CsrBtBppcDeactivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppcDeactivateCfm *primitive;

    primitive = (CsrBtBppcDeactivateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPC_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_BPPC_DEACTIVATE_CFM
void *CsrBtBppcDeactivateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppcDeactivateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppcDeactivateCfm *) CsrPmemZalloc(sizeof(CsrBtBppcDeactivateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPC_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_BPPC_CONNECT_RES
CsrSize CsrBtBppcConnectResSizeof(void *msg)
{
    CsrBtBppcConnectRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPC_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_BPPC_CONNECT_RES
CsrUint8 *CsrBtBppcConnectResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppcConnectRes *primitive;

    primitive = (CsrBtBppcConnectRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPC_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_BPPC_CONNECT_RES
void *CsrBtBppcConnectResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppcConnectRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppcConnectRes *) CsrPmemZalloc(sizeof(CsrBtBppcConnectRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPC_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_BPPC_CONNECT_IND
CsrSize CsrBtBppcConnectIndSizeof(void *msg)
{
    CsrBtBppcConnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPC_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_BPPC_CONNECT_IND
CsrUint8 *CsrBtBppcConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppcConnectInd *primitive;

    primitive = (CsrBtBppcConnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->obexPeerMaxPacketSize);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPC_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_BPPC_CONNECT_IND
void *CsrBtBppcConnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppcConnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppcConnectInd *) CsrPmemZalloc(sizeof(CsrBtBppcConnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->obexPeerMaxPacketSize, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPC_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_EVENT_RES
CsrSize CsrBtBppcGetEventResSizeof(void *msg)
{
    CsrBtBppcGetEventRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPC_GET_EVENT_RES */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_EVENT_RES
CsrUint8 *CsrBtBppcGetEventResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppcGetEventRes *primitive;

    primitive = (CsrBtBppcGetEventRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPC_GET_EVENT_RES */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_EVENT_RES
void *CsrBtBppcGetEventResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppcGetEventRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppcGetEventRes *) CsrPmemZalloc(sizeof(CsrBtBppcGetEventRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPC_GET_EVENT_RES */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_CFM
CsrSize CsrBtBppcGetJobAttributesCfmSizeof(void *msg)
{
    CsrBtBppcGetJobAttributesCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_CFM */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_CFM
CsrUint8 *CsrBtBppcGetJobAttributesCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppcGetJobAttributesCfm *primitive;

    primitive = (CsrBtBppcGetJobAttributesCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->jobAttributeObjectLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->jobAttributeObjectOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_CFM */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_CFM
void *CsrBtBppcGetJobAttributesCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppcGetJobAttributesCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppcGetJobAttributesCfm *) CsrPmemZalloc(sizeof(CsrBtBppcGetJobAttributesCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->jobAttributeObjectLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->jobAttributeObjectOffset, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_CFM */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_CFM
void CsrBtBppcGetJobAttributesCfmSerFree(void *msg)
{
    CsrBtBppcGetJobAttributesCfm *primitive;

    primitive = (CsrBtBppcGetJobAttributesCfm *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_CFM */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_EVENT_IND
CsrSize CsrBtBppcGetEventIndSizeof(void *msg)
{
    CsrBtBppcGetEventInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPC_GET_EVENT_IND */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_EVENT_IND
CsrUint8 *CsrBtBppcGetEventIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppcGetEventInd *primitive;

    primitive = (CsrBtBppcGetEventInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->eventObjectLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->eventObjectOffset);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPC_GET_EVENT_IND */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_EVENT_IND
void *CsrBtBppcGetEventIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppcGetEventInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppcGetEventInd *) CsrPmemZalloc(sizeof(CsrBtBppcGetEventInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->eventObjectLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->eventObjectOffset, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_BPPC_GET_EVENT_IND */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_EVENT_IND
void CsrBtBppcGetEventIndSerFree(void *msg)
{
    CsrBtBppcGetEventInd *primitive;

    primitive = (CsrBtBppcGetEventInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BPPC_GET_EVENT_IND */

#ifndef EXCLUDE_CSR_BT_BPPC_CANCEL_JOB_CFM
CsrSize CsrBtBppcCancelJobCfmSizeof(void *msg)
{
    CsrBtBppcCancelJobCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPC_CANCEL_JOB_CFM */

#ifndef EXCLUDE_CSR_BT_BPPC_CANCEL_JOB_CFM
CsrUint8 *CsrBtBppcCancelJobCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppcCancelJobCfm *primitive;

    primitive = (CsrBtBppcCancelJobCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->jobId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->operationStatus);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPC_CANCEL_JOB_CFM */

#ifndef EXCLUDE_CSR_BT_BPPC_CANCEL_JOB_CFM
void *CsrBtBppcCancelJobCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppcCancelJobCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppcCancelJobCfm *) CsrPmemZalloc(sizeof(CsrBtBppcCancelJobCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->jobId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->operationStatus, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPC_CANCEL_JOB_CFM */

#ifndef EXCLUDE_CSR_BT_BPPC_CONNECT_REQ
CsrSize CsrBtBppcConnectReqSizeof(void *msg)
{
    CsrBtBppcConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_CONNECT_REQ
CsrUint8 *CsrBtBppcConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppcConnectReq *primitive;

    primitive = (CsrBtBppcConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxPacketSize);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->count);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->windowSize);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmEnable);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_CONNECT_REQ
void *CsrBtBppcConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppcConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppcConnectReq *) CsrPmemZalloc(sizeof(CsrBtBppcConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxPacketSize, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->count, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->windowSize, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmEnable, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_REQ
CsrSize CsrBtBppcGetPrinterAttributesReqSizeof(void *msg)
{
    CsrBtBppcGetPrinterAttributesReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_REQ
CsrUint8 *CsrBtBppcGetPrinterAttributesReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppcGetPrinterAttributesReq *primitive;

    primitive = (CsrBtBppcGetPrinterAttributesReq *) msg;
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
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_REQ
void *CsrBtBppcGetPrinterAttributesReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppcGetPrinterAttributesReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppcGetPrinterAttributesReq *) CsrPmemZalloc(sizeof(CsrBtBppcGetPrinterAttributesReq));

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
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_RES
CsrSize CsrBtBppcGetJobAttributesResSizeof(void *msg)
{
    CsrBtBppcGetJobAttributesRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_RES */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_RES
CsrUint8 *CsrBtBppcGetJobAttributesResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppcGetJobAttributesRes *primitive;

    primitive = (CsrBtBppcGetJobAttributesRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_RES */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_RES
void *CsrBtBppcGetJobAttributesResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppcGetJobAttributesRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppcGetJobAttributesRes *) CsrPmemZalloc(sizeof(CsrBtBppcGetJobAttributesRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_RES */

#ifndef EXCLUDE_CSR_BT_BPPC_ABORT_CFM
CsrSize CsrBtBppcAbortCfmSizeof(void *msg)
{
    CsrBtBppcAbortCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPC_ABORT_CFM */

#ifndef EXCLUDE_CSR_BT_BPPC_ABORT_CFM
CsrUint8 *CsrBtBppcAbortCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppcAbortCfm *primitive;

    primitive = (CsrBtBppcAbortCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->channelId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPC_ABORT_CFM */

#ifndef EXCLUDE_CSR_BT_BPPC_ABORT_CFM
void *CsrBtBppcAbortCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppcAbortCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppcAbortCfm *) CsrPmemZalloc(sizeof(CsrBtBppcAbortCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->channelId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPC_ABORT_CFM */

#ifndef EXCLUDE_CSR_BT_BPPC_SECURITY_OUT_REQ
CsrSize CsrBtBppcSecurityOutReqSizeof(void *msg)
{
    CsrBtBppcSecurityOutReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPC_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_SECURITY_OUT_REQ
CsrUint8 *CsrBtBppcSecurityOutReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppcSecurityOutReq *primitive;

    primitive = (CsrBtBppcSecurityOutReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPC_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_SECURITY_OUT_REQ
void *CsrBtBppcSecurityOutReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppcSecurityOutReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppcSecurityOutReq *) CsrPmemZalloc(sizeof(CsrBtBppcSecurityOutReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPC_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_CFM
CsrSize CsrBtBppcGetPrinterAttributesCfmSizeof(void *msg)
{
    CsrBtBppcGetPrinterAttributesCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_CFM */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_CFM
CsrUint8 *CsrBtBppcGetPrinterAttributesCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppcGetPrinterAttributesCfm *primitive;

    primitive = (CsrBtBppcGetPrinterAttributesCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->printerAttributeObjectLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->printerAttributeObjectOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_CFM */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_CFM
void *CsrBtBppcGetPrinterAttributesCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppcGetPrinterAttributesCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppcGetPrinterAttributesCfm *) CsrPmemZalloc(sizeof(CsrBtBppcGetPrinterAttributesCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->printerAttributeObjectLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->printerAttributeObjectOffset, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_CFM */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_CFM
void CsrBtBppcGetPrinterAttributesCfmSerFree(void *msg)
{
    CsrBtBppcGetPrinterAttributesCfm *primitive;

    primitive = (CsrBtBppcGetPrinterAttributesCfm *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_CFM */

#ifndef EXCLUDE_CSR_BT_BPPC_SECURITY_OUT_CFM
CsrSize CsrBtBppcSecurityOutCfmSizeof(void *msg)
{
    CsrBtBppcSecurityOutCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPC_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_BPPC_SECURITY_OUT_CFM
CsrUint8 *CsrBtBppcSecurityOutCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppcSecurityOutCfm *primitive;

    primitive = (CsrBtBppcSecurityOutCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPC_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_BPPC_SECURITY_OUT_CFM
void *CsrBtBppcSecurityOutCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppcSecurityOutCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppcSecurityOutCfm *) CsrPmemZalloc(sizeof(CsrBtBppcSecurityOutCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPC_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_IND
CsrSize CsrBtBppcGetJobAttributesIndSizeof(void *msg)
{
    CsrBtBppcGetJobAttributesInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_IND */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_IND
CsrUint8 *CsrBtBppcGetJobAttributesIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppcGetJobAttributesInd *primitive;

    primitive = (CsrBtBppcGetJobAttributesInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->jobAttributeObjectLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->jobAttributeObjectOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_IND */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_IND
void *CsrBtBppcGetJobAttributesIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppcGetJobAttributesInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppcGetJobAttributesInd *) CsrPmemZalloc(sizeof(CsrBtBppcGetJobAttributesInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->jobAttributeObjectLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->jobAttributeObjectOffset, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_IND */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_IND
void CsrBtBppcGetJobAttributesIndSerFree(void *msg)
{
    CsrBtBppcGetJobAttributesInd *primitive;

    primitive = (CsrBtBppcGetJobAttributesInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_IND */

#ifndef EXCLUDE_CSR_BT_BPPC_ABORT_REQ
CsrSize CsrBtBppcAbortReqSizeof(void *msg)
{
    CsrBtBppcAbortReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPC_ABORT_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_ABORT_REQ
CsrUint8 *CsrBtBppcAbortReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppcAbortReq *primitive;

    primitive = (CsrBtBppcAbortReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->channelId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPC_ABORT_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_ABORT_REQ
void *CsrBtBppcAbortReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppcAbortReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppcAbortReq *) CsrPmemZalloc(sizeof(CsrBtBppcAbortReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->channelId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPC_ABORT_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_REFERENCE_OBJECT_IND
CsrSize CsrBtBppcGetReferenceObjectIndSizeof(void *msg)
{
    CsrBtBppcGetReferenceObjectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->fileName));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPC_GET_REFERENCE_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_REFERENCE_OBJECT_IND
CsrUint8 *CsrBtBppcGetReferenceObjectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppcGetReferenceObjectInd *primitive;

    primitive = (CsrBtBppcGetReferenceObjectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->fileName, sizeof(primitive->fileName));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->count);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->offset);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->lastPacket);
    CsrUtf8StringSer(buffer, length, primitive->fileName);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPC_GET_REFERENCE_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_REFERENCE_OBJECT_IND
void *CsrBtBppcGetReferenceObjectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppcGetReferenceObjectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppcGetReferenceObjectInd *) CsrPmemZalloc(sizeof(CsrBtBppcGetReferenceObjectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->fileName), buffer, &offset, ((CsrSize) sizeof(primitive->fileName)));
    CsrUint32Des((CsrUint32 *) &primitive->count, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->offset, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->lastPacket, buffer, &offset);
    CsrUtf8StringDes(&primitive->fileName, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPC_GET_REFERENCE_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_REFERENCE_OBJECT_IND
void CsrBtBppcGetReferenceObjectIndSerFree(void *msg)
{
    CsrBtBppcGetReferenceObjectInd *primitive;

    primitive = (CsrBtBppcGetReferenceObjectInd *) msg;

    if(primitive->fileName != NULL)
    {
        CsrPmemFree(primitive->fileName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BPPC_GET_REFERENCE_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_RES
CsrSize CsrBtBppcSendDocumentResSizeof(void *msg)
{
    CsrBtBppcSendDocumentRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->printContentLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_RES */

#ifndef EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_RES
CsrUint8 *CsrBtBppcSendDocumentResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppcSendDocumentRes *primitive;

    primitive = (CsrBtBppcSendDocumentRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->printContentLength);
    CsrMemCpySer(buffer, length, &primitive->printContent, sizeof(primitive->printContent));
    if(primitive->printContent)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->printContent), ((CsrSize) (primitive->printContentLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_RES */

#ifndef EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_RES
void *CsrBtBppcSendDocumentResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppcSendDocumentRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppcSendDocumentRes *) CsrPmemZalloc(sizeof(CsrBtBppcSendDocumentRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->printContentLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->printContent), buffer, &offset, ((CsrSize) sizeof(primitive->printContent)));
    if (primitive->printContentLength)
    {
        primitive->printContent = CsrPmemZalloc(((CsrUint32) (primitive->printContentLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->printContent), buffer, &offset, ((CsrSize) (primitive->printContentLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->printContent = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_RES */

#ifndef EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_RES
void CsrBtBppcSendDocumentResSerFree(void *msg)
{
    CsrBtBppcSendDocumentRes *primitive;

    primitive = (CsrBtBppcSendDocumentRes *) msg;

    if(primitive->printContent != NULL)
    {
        CsrPmemFree(primitive->printContent);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_RES */

#ifndef EXCLUDE_CSR_BT_BPPC_AUTHENTICATE_IND
CsrSize CsrBtBppcAuthenticateIndSizeof(void *msg)
{
    CsrBtBppcAuthenticateInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->realmLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPC_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_BPPC_AUTHENTICATE_IND
CsrUint8 *CsrBtBppcAuthenticateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppcAuthenticateInd *primitive;

    primitive = (CsrBtBppcAuthenticateInd *) msg;
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
#endif /* EXCLUDE_CSR_BT_BPPC_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_BPPC_AUTHENTICATE_IND
void *CsrBtBppcAuthenticateIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppcAuthenticateInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppcAuthenticateInd *) CsrPmemZalloc(sizeof(CsrBtBppcAuthenticateInd));

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
#endif /* EXCLUDE_CSR_BT_BPPC_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_BPPC_AUTHENTICATE_IND
void CsrBtBppcAuthenticateIndSerFree(void *msg)
{
    CsrBtBppcAuthenticateInd *primitive;

    primitive = (CsrBtBppcAuthenticateInd *) msg;

    if(primitive->realm != NULL)
    {
        CsrPmemFree(primitive->realm);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BPPC_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_RES
CsrSize CsrBtBppcGetPrinterAttributesResSizeof(void *msg)
{
    CsrBtBppcGetPrinterAttributesRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_RES */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_RES
CsrUint8 *CsrBtBppcGetPrinterAttributesResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppcGetPrinterAttributesRes *primitive;

    primitive = (CsrBtBppcGetPrinterAttributesRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_RES */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_RES
void *CsrBtBppcGetPrinterAttributesResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppcGetPrinterAttributesRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppcGetPrinterAttributesRes *) CsrPmemZalloc(sizeof(CsrBtBppcGetPrinterAttributesRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_RES */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_IND
CsrSize CsrBtBppcGetPrinterAttributesIndSizeof(void *msg)
{
    CsrBtBppcGetPrinterAttributesInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_IND */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_IND
CsrUint8 *CsrBtBppcGetPrinterAttributesIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppcGetPrinterAttributesInd *primitive;

    primitive = (CsrBtBppcGetPrinterAttributesInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->printerAttributeObjectLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->printerAttributeObjectOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_IND */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_IND
void *CsrBtBppcGetPrinterAttributesIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppcGetPrinterAttributesInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppcGetPrinterAttributesInd *) CsrPmemZalloc(sizeof(CsrBtBppcGetPrinterAttributesInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->printerAttributeObjectLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->printerAttributeObjectOffset, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_IND */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_IND
void CsrBtBppcGetPrinterAttributesIndSerFree(void *msg)
{
    CsrBtBppcGetPrinterAttributesInd *primitive;

    primitive = (CsrBtBppcGetPrinterAttributesInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_IND */

#ifndef EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_REQ
CsrSize CsrBtBppcSendDocumentReqSizeof(void *msg)
{
    CsrBtBppcSendDocumentReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->mimeMediaType) + CsrConverterUcs2ByteStrLen((CsrUtf8String*) primitive->ucs2documentName) + 2 + CsrConverterUcs2ByteStrLen((CsrUtf8String*) primitive->docTypeDependentInfo) + 2);
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_REQ
CsrUint8 *CsrBtBppcSendDocumentReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppcSendDocumentReq *primitive;

    primitive = (CsrBtBppcSendDocumentReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->jobId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->transferModel);
    CsrMemCpySer(buffer, length, &primitive->mimeMediaType, sizeof(primitive->mimeMediaType));
    CsrMemCpySer(buffer, length, &primitive->ucs2documentName, sizeof(primitive->ucs2documentName));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->docTypeDependentInfoLength);
    CsrMemCpySer(buffer, length, &primitive->docTypeDependentInfo, sizeof(primitive->docTypeDependentInfo));
    CsrUtf8StringSer(buffer, length, primitive->mimeMediaType);
    if (primitive->ucs2documentName)
    {
        CsrMemCpy(((CsrUint8*) &buffer[*length]), ((CsrUint8*) primitive->ucs2documentName), CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->ucs2documentName) + 2);
        *length += ((CsrUint16) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->ucs2documentName) + 2));
    }
    else
    {
        buffer[*length] = '\0';
        buffer[*length + 1] = '\0';
        *length += 2;
    }
    if (primitive->docTypeDependentInfo)
    {
        CsrMemCpy(((CsrUint8*) &buffer[*length]), ((CsrUint8*) primitive->docTypeDependentInfo), CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->docTypeDependentInfo) + 2);
        *length += ((CsrUint16) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->docTypeDependentInfo) + 2));
    }
    else
    {
        buffer[*length] = '\0';
        buffer[*length + 1] = '\0';
        *length += 2;
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_REQ
void *CsrBtBppcSendDocumentReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppcSendDocumentReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppcSendDocumentReq *) CsrPmemZalloc(sizeof(CsrBtBppcSendDocumentReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->jobId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->transferModel, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->mimeMediaType), buffer, &offset, ((CsrSize) sizeof(primitive->mimeMediaType)));
    CsrMemCpyDes(((void *) &primitive->ucs2documentName), buffer, &offset, ((CsrSize) sizeof(primitive->ucs2documentName)));
    CsrUint16Des((CsrUint16 *) &primitive->docTypeDependentInfoLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->docTypeDependentInfo), buffer, &offset, ((CsrSize) sizeof(primitive->docTypeDependentInfo)));
    CsrUtf8StringDes(&primitive->mimeMediaType, buffer, &offset);
    primitive->ucs2documentName = CsrPmemZalloc(CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    CsrMemCpy(((CsrUint8*) primitive->ucs2documentName), ((CsrUint8*) &buffer[offset]),CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    offset += ((CsrSize) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->ucs2documentName) + 2));
    primitive->docTypeDependentInfo = CsrPmemZalloc(CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    CsrMemCpy(((CsrUint8*) primitive->docTypeDependentInfo), ((CsrUint8*) &buffer[offset]),CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    offset += ((CsrSize) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->docTypeDependentInfo) + 2));

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_REQ
void CsrBtBppcSendDocumentReqSerFree(void *msg)
{
    CsrBtBppcSendDocumentReq *primitive;

    primitive = (CsrBtBppcSendDocumentReq *) msg;

    if(primitive->mimeMediaType != NULL)
    {
        CsrPmemFree(primitive->mimeMediaType);
    }
    if(primitive->ucs2documentName != NULL)
    {
        CsrPmemFree(primitive->ucs2documentName);
    }
    if(primitive->docTypeDependentInfo != NULL)
    {
        CsrPmemFree(primitive->docTypeDependentInfo);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_EVENT_REQ
CsrSize CsrBtBppcGetEventReqSizeof(void *msg)
{
    CsrBtBppcGetEventReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPC_GET_EVENT_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_EVENT_REQ
CsrUint8 *CsrBtBppcGetEventReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppcGetEventReq *primitive;

    primitive = (CsrBtBppcGetEventReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->jobId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPC_GET_EVENT_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_EVENT_REQ
void *CsrBtBppcGetEventReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppcGetEventReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppcGetEventReq *) CsrPmemZalloc(sizeof(CsrBtBppcGetEventReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->jobId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPC_GET_EVENT_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_CANCEL_JOB_REQ
CsrSize CsrBtBppcCancelJobReqSizeof(void *msg)
{
    CsrBtBppcCancelJobReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPC_CANCEL_JOB_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_CANCEL_JOB_REQ
CsrUint8 *CsrBtBppcCancelJobReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppcCancelJobReq *primitive;

    primitive = (CsrBtBppcCancelJobReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->jobId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPC_CANCEL_JOB_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_CANCEL_JOB_REQ
void *CsrBtBppcCancelJobReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppcCancelJobReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppcCancelJobReq *) CsrPmemZalloc(sizeof(CsrBtBppcCancelJobReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->jobId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPC_CANCEL_JOB_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_CREATE_JOB_REQ
CsrSize CsrBtBppcCreateJobReqSizeof(void *msg)
{
    CsrBtBppcCreateJobReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->sides) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->orientation) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->printQuality) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->jobName) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->jobUserName) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->documentFormat) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->mediaSize) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->mediaType));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPC_CREATE_JOB_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_CREATE_JOB_REQ
CsrUint8 *CsrBtBppcCreateJobReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppcCreateJobReq *primitive;

    primitive = (CsrBtBppcCreateJobReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->cancelOnLostLink);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->copies);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->numberUp);
    CsrMemCpySer(buffer, length, &primitive->sides, sizeof(primitive->sides));
    CsrMemCpySer(buffer, length, &primitive->orientation, sizeof(primitive->orientation));
    CsrMemCpySer(buffer, length, &primitive->printQuality, sizeof(primitive->printQuality));
    CsrMemCpySer(buffer, length, &primitive->jobName, sizeof(primitive->jobName));
    CsrMemCpySer(buffer, length, &primitive->jobUserName, sizeof(primitive->jobUserName));
    CsrMemCpySer(buffer, length, &primitive->documentFormat, sizeof(primitive->documentFormat));
    CsrMemCpySer(buffer, length, &primitive->mediaSize, sizeof(primitive->mediaSize));
    CsrMemCpySer(buffer, length, &primitive->mediaType, sizeof(primitive->mediaType));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    CsrUtf8StringSer(buffer, length, primitive->sides);
    CsrUtf8StringSer(buffer, length, primitive->orientation);
    CsrUtf8StringSer(buffer, length, primitive->printQuality);
    CsrUtf8StringSer(buffer, length, primitive->jobName);
    CsrUtf8StringSer(buffer, length, primitive->jobUserName);
    CsrUtf8StringSer(buffer, length, primitive->documentFormat);
    CsrUtf8StringSer(buffer, length, primitive->mediaSize);
    CsrUtf8StringSer(buffer, length, primitive->mediaType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPC_CREATE_JOB_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_CREATE_JOB_REQ
void *CsrBtBppcCreateJobReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppcCreateJobReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppcCreateJobReq *) CsrPmemZalloc(sizeof(CsrBtBppcCreateJobReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->cancelOnLostLink, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->copies, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->numberUp, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->sides), buffer, &offset, ((CsrSize) sizeof(primitive->sides)));
    CsrMemCpyDes(((void *) &primitive->orientation), buffer, &offset, ((CsrSize) sizeof(primitive->orientation)));
    CsrMemCpyDes(((void *) &primitive->printQuality), buffer, &offset, ((CsrSize) sizeof(primitive->printQuality)));
    CsrMemCpyDes(((void *) &primitive->jobName), buffer, &offset, ((CsrSize) sizeof(primitive->jobName)));
    CsrMemCpyDes(((void *) &primitive->jobUserName), buffer, &offset, ((CsrSize) sizeof(primitive->jobUserName)));
    CsrMemCpyDes(((void *) &primitive->documentFormat), buffer, &offset, ((CsrSize) sizeof(primitive->documentFormat)));
    CsrMemCpyDes(((void *) &primitive->mediaSize), buffer, &offset, ((CsrSize) sizeof(primitive->mediaSize)));
    CsrMemCpyDes(((void *) &primitive->mediaType), buffer, &offset, ((CsrSize) sizeof(primitive->mediaType)));
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    CsrUtf8StringDes(&primitive->sides, buffer, &offset);
    CsrUtf8StringDes(&primitive->orientation, buffer, &offset);
    CsrUtf8StringDes(&primitive->printQuality, buffer, &offset);
    CsrUtf8StringDes(&primitive->jobName, buffer, &offset);
    CsrUtf8StringDes(&primitive->jobUserName, buffer, &offset);
    CsrUtf8StringDes(&primitive->documentFormat, buffer, &offset);
    CsrUtf8StringDes(&primitive->mediaSize, buffer, &offset);
    CsrUtf8StringDes(&primitive->mediaType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPC_CREATE_JOB_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_CREATE_JOB_REQ
void CsrBtBppcCreateJobReqSerFree(void *msg)
{
    CsrBtBppcCreateJobReq *primitive;

    primitive = (CsrBtBppcCreateJobReq *) msg;

    if(primitive->sides != NULL)
    {
        CsrPmemFree(primitive->sides);
    }
    if(primitive->orientation != NULL)
    {
        CsrPmemFree(primitive->orientation);
    }
    if(primitive->printQuality != NULL)
    {
        CsrPmemFree(primitive->printQuality);
    }
    if(primitive->jobName != NULL)
    {
        CsrPmemFree(primitive->jobName);
    }
    if(primitive->jobUserName != NULL)
    {
        CsrPmemFree(primitive->jobUserName);
    }
    if(primitive->documentFormat != NULL)
    {
        CsrPmemFree(primitive->documentFormat);
    }
    if(primitive->mediaSize != NULL)
    {
        CsrPmemFree(primitive->mediaSize);
    }
    if(primitive->mediaType != NULL)
    {
        CsrPmemFree(primitive->mediaType);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BPPC_CREATE_JOB_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_REQ
CsrSize CsrBtBppcGetJobAttributesReqSizeof(void *msg)
{
    CsrBtBppcGetJobAttributesReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_REQ
CsrUint8 *CsrBtBppcGetJobAttributesReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppcGetJobAttributesReq *primitive;

    primitive = (CsrBtBppcGetJobAttributesReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->jobId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->jobState);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->jobName);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->jobOriginatingUserName);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->jobMediaSheetsCompleted);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->numberOfInterveningJobs);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_REQ
void *CsrBtBppcGetJobAttributesReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppcGetJobAttributesReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppcGetJobAttributesReq *) CsrPmemZalloc(sizeof(CsrBtBppcGetJobAttributesReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->jobId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->jobState, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->jobName, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->jobOriginatingUserName, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->jobMediaSheetsCompleted, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->numberOfInterveningJobs, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_CREATE_JOB_CFM
CsrSize CsrBtBppcCreateJobCfmSizeof(void *msg)
{
    CsrBtBppcCreateJobCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPC_CREATE_JOB_CFM */

#ifndef EXCLUDE_CSR_BT_BPPC_CREATE_JOB_CFM
CsrUint8 *CsrBtBppcCreateJobCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppcCreateJobCfm *primitive;

    primitive = (CsrBtBppcCreateJobCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->jobId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->operationStatus);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPC_CREATE_JOB_CFM */

#ifndef EXCLUDE_CSR_BT_BPPC_CREATE_JOB_CFM
void *CsrBtBppcCreateJobCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppcCreateJobCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppcCreateJobCfm *) CsrPmemZalloc(sizeof(CsrBtBppcCreateJobCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->jobId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->operationStatus, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPC_CREATE_JOB_CFM */

#ifndef EXCLUDE_CSR_BT_BPPC_CONNECT_CFM
CsrSize CsrBtBppcConnectCfmSizeof(void *msg)
{
    CsrBtBppcConnectCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->xhtmlPrintImageFormats) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->documentFormatsSupported) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->mediaTypesSupported) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->printerModelId));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPC_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_BPPC_CONNECT_CFM
CsrUint8 *CsrBtBppcConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppcConnectCfm *primitive;

    primitive = (CsrBtBppcConnectCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->colorSupported);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->duplexSupported);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxPeerPacketSize);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxMediaWidth);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxMediaLength);
    CsrMemCpySer(buffer, length, ((const void *) primitive->characterRepertoires), ((CsrSize) (16)));
    CsrMemCpySer(buffer, length, &primitive->xhtmlPrintImageFormats, sizeof(primitive->xhtmlPrintImageFormats));
    CsrMemCpySer(buffer, length, &primitive->documentFormatsSupported, sizeof(primitive->documentFormatsSupported));
    CsrMemCpySer(buffer, length, &primitive->mediaTypesSupported, sizeof(primitive->mediaTypesSupported));
    CsrMemCpySer(buffer, length, &primitive->printerModelId, sizeof(primitive->printerModelId));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUtf8StringSer(buffer, length, primitive->xhtmlPrintImageFormats);
    CsrUtf8StringSer(buffer, length, primitive->documentFormatsSupported);
    CsrUtf8StringSer(buffer, length, primitive->mediaTypesSupported);
    CsrUtf8StringSer(buffer, length, primitive->printerModelId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPC_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_BPPC_CONNECT_CFM
void *CsrBtBppcConnectCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppcConnectCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppcConnectCfm *) CsrPmemZalloc(sizeof(CsrBtBppcConnectCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->colorSupported, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->duplexSupported, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxPeerPacketSize, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxMediaWidth, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxMediaLength, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->characterRepertoires), buffer, &offset, ((CsrSize) (16)));
    CsrMemCpyDes(((void *) &primitive->xhtmlPrintImageFormats), buffer, &offset, ((CsrSize) sizeof(primitive->xhtmlPrintImageFormats)));
    CsrMemCpyDes(((void *) &primitive->documentFormatsSupported), buffer, &offset, ((CsrSize) sizeof(primitive->documentFormatsSupported)));
    CsrMemCpyDes(((void *) &primitive->mediaTypesSupported), buffer, &offset, ((CsrSize) sizeof(primitive->mediaTypesSupported)));
    CsrMemCpyDes(((void *) &primitive->printerModelId), buffer, &offset, ((CsrSize) sizeof(primitive->printerModelId)));
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUtf8StringDes(&primitive->xhtmlPrintImageFormats, buffer, &offset);
    CsrUtf8StringDes(&primitive->documentFormatsSupported, buffer, &offset);
    CsrUtf8StringDes(&primitive->mediaTypesSupported, buffer, &offset);
    CsrUtf8StringDes(&primitive->printerModelId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPC_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_BPPC_CONNECT_CFM
void CsrBtBppcConnectCfmSerFree(void *msg)
{
    CsrBtBppcConnectCfm *primitive;

    primitive = (CsrBtBppcConnectCfm *) msg;

    if(primitive->xhtmlPrintImageFormats != NULL)
    {
        CsrPmemFree(primitive->xhtmlPrintImageFormats);
    }
    if(primitive->documentFormatsSupported != NULL)
    {
        CsrPmemFree(primitive->documentFormatsSupported);
    }
    if(primitive->mediaTypesSupported != NULL)
    {
        CsrPmemFree(primitive->mediaTypesSupported);
    }
    if(primitive->printerModelId != NULL)
    {
        CsrPmemFree(primitive->printerModelId);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BPPC_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_BPPC_DISCONNECT_IND
CsrSize CsrBtBppcDisconnectIndSizeof(void *msg)
{
    CsrBtBppcDisconnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPC_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_BPPC_DISCONNECT_IND
CsrUint8 *CsrBtBppcDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppcDisconnectInd *primitive;

    primitive = (CsrBtBppcDisconnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->channelId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPC_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_BPPC_DISCONNECT_IND
void *CsrBtBppcDisconnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppcDisconnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppcDisconnectInd *) CsrPmemZalloc(sizeof(CsrBtBppcDisconnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->channelId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPC_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_CFM
CsrSize CsrBtBppcSendDocumentCfmSizeof(void *msg)
{
    CsrBtBppcSendDocumentCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_CFM */

#ifndef EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_CFM
CsrUint8 *CsrBtBppcSendDocumentCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppcSendDocumentCfm *primitive;

    primitive = (CsrBtBppcSendDocumentCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_CFM */

#ifndef EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_CFM
void *CsrBtBppcSendDocumentCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppcSendDocumentCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppcSendDocumentCfm *) CsrPmemZalloc(sizeof(CsrBtBppcSendDocumentCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_CFM */

#ifndef EXCLUDE_CSR_BT_BPPC_CANCEL_CONNECT_REQ
CsrSize CsrBtBppcCancelConnectReqSizeof(void *msg)
{
    CsrBtBppcCancelConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPC_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_CANCEL_CONNECT_REQ
CsrUint8 *CsrBtBppcCancelConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppcCancelConnectReq *primitive;

    primitive = (CsrBtBppcCancelConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPC_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_CANCEL_CONNECT_REQ
void *CsrBtBppcCancelConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppcCancelConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppcCancelConnectReq *) CsrPmemZalloc(sizeof(CsrBtBppcCancelConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPC_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_IND
CsrSize CsrBtBppcSendDocumentIndSizeof(void *msg)
{
    CsrBtBppcSendDocumentInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_IND */

#ifndef EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_IND
CsrUint8 *CsrBtBppcSendDocumentIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppcSendDocumentInd *primitive;

    primitive = (CsrBtBppcSendDocumentInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->printContentLength);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_IND */

#ifndef EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_IND
void *CsrBtBppcSendDocumentIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppcSendDocumentInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppcSendDocumentInd *) CsrPmemZalloc(sizeof(CsrBtBppcSendDocumentInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->printContentLength, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_IND */

#ifndef EXCLUDE_CSR_BT_BPPC_DISCONNECT_REQ
CsrSize CsrBtBppcDisconnectReqSizeof(void *msg)
{
    CsrBtBppcDisconnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPC_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_DISCONNECT_REQ
CsrUint8 *CsrBtBppcDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppcDisconnectReq *primitive;

    primitive = (CsrBtBppcDisconnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->normalDisconnect);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPC_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_DISCONNECT_REQ
void *CsrBtBppcDisconnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppcDisconnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppcDisconnectReq *) CsrPmemZalloc(sizeof(CsrBtBppcDisconnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->normalDisconnect, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPC_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_AUTHENTICATE_RES
CsrSize CsrBtBppcAuthenticateResSizeof(void *msg)
{
    CsrBtBppcAuthenticateRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->passwordLength * sizeof(CsrUint8)) + CsrCharStringSerLen((CsrCharString *) primitive->userId));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPC_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_BPPC_AUTHENTICATE_RES
CsrUint8 *CsrBtBppcAuthenticateResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppcAuthenticateRes *primitive;

    primitive = (CsrBtBppcAuthenticateRes *) msg;
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
#endif /* EXCLUDE_CSR_BT_BPPC_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_BPPC_AUTHENTICATE_RES
void *CsrBtBppcAuthenticateResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppcAuthenticateRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppcAuthenticateRes *) CsrPmemZalloc(sizeof(CsrBtBppcAuthenticateRes));

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
#endif /* EXCLUDE_CSR_BT_BPPC_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_BPPC_AUTHENTICATE_RES
void CsrBtBppcAuthenticateResSerFree(void *msg)
{
    CsrBtBppcAuthenticateRes *primitive;

    primitive = (CsrBtBppcAuthenticateRes *) msg;

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
#endif /* EXCLUDE_CSR_BT_BPPC_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_REFERENCE_OBJECT_RES
CsrSize CsrBtBppcGetReferenceObjectResSizeof(void *msg)
{
    CsrBtBppcGetReferenceObjectRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->count * sizeof(CsrUint8)) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->fileName));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPC_GET_REFERENCE_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_REFERENCE_OBJECT_RES
CsrUint8 *CsrBtBppcGetReferenceObjectResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppcGetReferenceObjectRes *primitive;

    primitive = (CsrBtBppcGetReferenceObjectRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrMemCpySer(buffer, length, &primitive->file, sizeof(primitive->file));
    CsrMemCpySer(buffer, length, &primitive->fileName, sizeof(primitive->fileName));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->count);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->offset);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->fileSize);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    if(primitive->file)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->file), ((CsrSize) (primitive->count * sizeof(CsrUint8))));
    }
    CsrUtf8StringSer(buffer, length, primitive->fileName);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPC_GET_REFERENCE_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_REFERENCE_OBJECT_RES
void *CsrBtBppcGetReferenceObjectResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppcGetReferenceObjectRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppcGetReferenceObjectRes *) CsrPmemZalloc(sizeof(CsrBtBppcGetReferenceObjectRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->file), buffer, &offset, ((CsrSize) sizeof(primitive->file)));
    CsrMemCpyDes(((void *) &primitive->fileName), buffer, &offset, ((CsrSize) sizeof(primitive->fileName)));
    CsrUint32Des((CsrUint32 *) &primitive->count, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->offset, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->fileSize, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    if (primitive->count)
    {
        primitive->file = CsrPmemZalloc(((CsrUint32) (primitive->count * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->file), buffer, &offset, ((CsrSize) (primitive->count * sizeof(CsrUint8))));
    }
    else
    {
        primitive->file = NULL;
    }
    CsrUtf8StringDes(&primitive->fileName, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPC_GET_REFERENCE_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_REFERENCE_OBJECT_RES
void CsrBtBppcGetReferenceObjectResSerFree(void *msg)
{
    CsrBtBppcGetReferenceObjectRes *primitive;

    primitive = (CsrBtBppcGetReferenceObjectRes *) msg;

    if(primitive->file != NULL)
    {
        CsrPmemFree(primitive->file);
    }
    if(primitive->fileName != NULL)
    {
        CsrPmemFree(primitive->fileName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BPPC_GET_REFERENCE_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BPPC_DEACTIVATE_REQ
CsrSize CsrBtBppcDeactivateReqSizeof(void *msg)
{
    CsrBtBppcDeactivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPC_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_DEACTIVATE_REQ
CsrUint8 *CsrBtBppcDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppcDeactivateReq *primitive;

    primitive = (CsrBtBppcDeactivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPC_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_DEACTIVATE_REQ
void *CsrBtBppcDeactivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppcDeactivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppcDeactivateReq *) CsrPmemZalloc(sizeof(CsrBtBppcDeactivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPC_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_ACTIVATE_REQ
CsrSize CsrBtBppcActivateReqSizeof(void *msg)
{
    CsrBtBppcActivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BPPC_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_ACTIVATE_REQ
CsrUint8 *CsrBtBppcActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBppcActivateReq *primitive;

    primitive = (CsrBtBppcActivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->supportedProfiles);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->obexMaxPacketSize);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->windowSize);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmEnable);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BPPC_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_ACTIVATE_REQ
void *CsrBtBppcActivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBppcActivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBppcActivateReq *) CsrPmemZalloc(sizeof(CsrBtBppcActivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->supportedProfiles, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->obexMaxPacketSize, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->windowSize, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmEnable, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BPPC_ACTIVATE_REQ */

#endif /* EXCLUDE_CSR_BT_BPPC_MODULE */
