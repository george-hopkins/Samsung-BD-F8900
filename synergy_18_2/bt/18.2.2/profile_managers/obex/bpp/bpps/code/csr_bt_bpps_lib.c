/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_profiles.h"
#include "csr_pmem.h"
#include "csr_bt_util.h"
#include "csr_bt_bpps_lib.h"
#include "csr_msg_transport.h"

void CsrBtBppsMsgTransport(void* msg)
{
    CsrMsgTransport(CSR_BT_BPPS_IFACEQUEUE, CSR_BT_BPPS_PRIM, msg);
}

CsrBtBppsGetPrinterAttribsRes* CsrBtBppsGetPrinterAttribsResSend_struct(CsrUtf8String                   *pageHeight,
                                                                        CsrUtf8String                   *pageWidth,
                                                                        CsrUtf8String                   *color,
                                                                        CsrUint8                        *documentFormats,
                                                                        CsrUint8                        documentFormatsCount,
                                                                        CsrUint8                        *imageFormats,
                                                                        CsrUint8                        imageFormatsCount,
                                                                        CsrUtf8String                   *maxCopies,
                                                                        CsrUtf8String                   *mediaLoaded,
                                                                        CsrUint8                        *mediaSizes,
                                                                        CsrUint8                        mediaSizesCount,
                                                                        CsrUint8                        *mediaTypes,
                                                                        CsrUint8                        mediaTypesCount,
                                                                        CsrUtf8String                   *numberUp,
                                                                        CsrUint8                        *orientations,
                                                                        CsrUint8                        orientationsCount,
                                                                        CsrUtf8String                   *currentOperator,
                                                                        CsrUtf8String                   *printerLocation,
                                                                        CsrUtf8String                   *printerName,
                                                                        CsrUtf8String                   *printerState,
                                                                        CsrUtf8String                   *printerReasons,
                                                                        CsrUint8                        *printQuality,
                                                                        CsrUint8                        printQualityCount,
                                                                        CsrUtf8String                   *queuedJobCount,
                                                                        CsrUint8                        *sides,
                                                                        CsrUint8                        sidesCount,
                                                                        CsrUint8                        mediaLoadedCount,
                                                                        CsrBtObexOperationStatusCode    operationStatus,
                                                                        CsrBool                         srmpOn)
{
    CsrUint8   i;
    CsrUint8   *ptr;
    CsrBtBppsGetPrinterAttribsRes* prim       = CsrPmemAlloc(sizeof(CsrBtBppsGetPrinterAttribsRes));

    /* These are the trivial parameters */
    prim->type                           = CSR_BT_BPPS_GET_PRINTER_ATTRIBS_RES;
    prim->basicTextPageHeight            = pageHeight;
    prim->basicTextPageWidth             = pageWidth;
    prim->colorSupported                 = color;
    prim->maxCopiesSupported             = maxCopies;
    prim->numberUpSupported              = numberUp;
    prim->printerGeneralCurrentOperator  = currentOperator;
    prim->printerLocation                = printerLocation;
    prim->printerName                    = printerName;
    prim->printerState                   = printerState;
    prim->printerStateReasons            = printerReasons;
    prim->queuedJobCount                 = queuedJobCount;
    prim->operationStatus                = operationStatus;

    /* For the remaining, we need to count the total number of bytes */
    prim->documentFormatsSupported       = documentFormats;
    prim->documentFormatsSupportedCount  = documentFormatsCount;
    prim->documentFormatsSupportedLength = 0;
    for(i=0, ptr=documentFormats; i< documentFormatsCount; i++)
    {
        prim->documentFormatsSupportedLength = (CsrUint16)(prim->documentFormatsSupportedLength + CsrStrLen((char*)ptr) + 1);
        ptr += CsrStrLen((char*)ptr) + 1;
    }

    prim->imageFormatsSupported          = imageFormats;
    prim->imageFormatsSupportedCount     = imageFormatsCount;
    prim->imageFormatsSupportedLength    = 0;
    for(i=0, ptr=imageFormats; i< imageFormatsCount; i++)
    {
        prim->imageFormatsSupportedLength = (CsrUint16)(prim->imageFormatsSupportedLength + CsrStrLen((char*)ptr) + 1);
        ptr += CsrStrLen((char*)ptr) + 1;
    }

    prim->mediaSizesSupported            = mediaSizes;
    prim->mediaSizesSupportedCount       = mediaSizesCount;
    prim->mediaSizesSupportedLength      = 0;
    for(i=0, ptr=mediaSizes; i< mediaSizesCount; i++)
    {
        prim->mediaSizesSupportedLength = (CsrUint16)(prim->mediaSizesSupportedLength + CsrStrLen((char*)ptr) + 1);
        ptr += CsrStrLen((char*)ptr) + 1;
    }

    prim->mediaTypesSupported            = mediaTypes;
    prim->mediaTypesSupportedCount       = mediaTypesCount;
    prim->mediaTypesSupportedLength      = 0;
    for(i=0, ptr=mediaTypes; i< mediaTypesCount; i++)
    {
        prim->mediaTypesSupportedLength = (CsrUint16)(prim->mediaTypesSupportedLength + CsrStrLen((char*)ptr) + 1);
        ptr += CsrStrLen((char*)ptr) + 1;
    }

    prim->orientationsSupported          = orientations;
    prim->orientationsSupportedCount     = orientationsCount;
    prim->orientationsSupportedLength    = 0;
    for(i=0, ptr=orientations; i< orientationsCount; i++)
    {
        prim->orientationsSupportedLength = (CsrUint16)(prim->orientationsSupportedLength + CsrStrLen((char*)ptr) + 1);
        ptr += CsrStrLen((char*)ptr) + 1;
    }

    prim->printQualitySupported          = printQuality;
    prim->printQualitySupportedCount     = printQualityCount;
    prim->printQualitySupportedLength    = 0;
    for(i=0, ptr=printQuality; i< printQualityCount; i++)
    {
        prim->printQualitySupportedLength = (CsrUint16)(prim->printQualitySupportedLength + CsrStrLen((char*)ptr) + 1);
        ptr += CsrStrLen((char*)ptr) + 1;
    }

    prim->sidesSupported                 = sides;
    prim->sidesSupportedCount            = sidesCount;
    prim->sidesSupportedLength           = 0;
    for(i=0, ptr=sides; i< sidesCount; i++)
    {
        prim->sidesSupportedLength = (CsrUint16)(prim->sidesSupportedLength + CsrStrLen((char*)ptr) + 1);
        ptr += CsrStrLen((char*)ptr) + 1;
    }

    prim->mediaLoaded                    = mediaLoaded;
    prim->mediaLoadedCount               = mediaLoadedCount;
    prim->mediaLoadedLength              = 0;

    /* the times 2 factor is inserted because the string contains type/size pairs, hence two strings per loaded media */
    for(i=0, ptr=(CsrUint8*)mediaLoaded; i < mediaLoadedCount * 2; i++)
    {
        prim->mediaLoadedLength = (CsrUint16)(prim->mediaLoadedLength
                                              + CsrStrLen((char*)ptr) + 1);
        ptr += CsrStrLen((char*)ptr) + 1;
    }

    prim->srmpOn                         = srmpOn;

    return prim;
}



