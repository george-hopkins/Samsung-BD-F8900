/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_pmem.h"
#include "csr_bt_util.h"
#include "csr_bt_bppc_handler.h"
#include "csr_bt_bppc_soapbuilder.h"

CsrUint16 CsrBtBppcGetSoapEncodedGetPrinterAttrReqContentLength(CsrBtBppcGetPrinterAttributesReq *pMsg, CsrBool *getAllAttrs)
{
    CsrUint16 attrLength = (CsrUint16)(CsrStrLen(ENVELOPE_START) +
                                       CsrStrLen(BODY_START_GET_PRINTER_ATTR) +
                                       CsrStrLen(ENVELOPE_BODY_END_GET_PRINTER_ATTR));

    /* Are we getting all attributes? */
    CsrBool   gettingAllAttrs = (pMsg->printerName           &&
                                 pMsg->printerLocation       &&
                                 pMsg->printerState          &&
                                 pMsg->printerStateReasons   &&
                                 pMsg->colorSupported        &&
                                 pMsg->maxCopiesSupported    &&
                                 pMsg->sidesSupported        &&
                                 pMsg->numberUpSupported     &&
                                 pMsg->orientationsSupported &&
                                 pMsg->mediaSizesSupported   &&
                                 pMsg->mediaTypesSupported   &&
                                 pMsg->mediaLoaded           &&
                                 pMsg->printQualitySupported &&
                                 pMsg->queuedJobCount        &&
                                 pMsg->imageFormatsSupported &&
                                 pMsg->basicTextPageWidth    &&
                                 pMsg->basicTextPageHeight   &&
                                 pMsg->printerGeneralCurrentOperator);

    CsrUint16 attrStartLength   = (CsrUint16)CsrStrLen(PRINTER_ATTRIBUTE_START);
    CsrUint16 attrEndLength     = (CsrUint16)CsrStrLen(PRINTER_ATTRIBUTE_END);
    

    attrLength += CsrStrLen(REQ_PRINTER_ATTRIBUTES_START) + CsrStrLen(REQ_PRINTER_ATTRIBUTES_END);

    if (pMsg->basicTextPageHeight)
    {
        attrLength = (CsrUint16)(CsrStrLen(BASIC_TEXT_PAGE_HEIGHT) + attrStartLength + attrEndLength + attrLength);
    }
    if (pMsg->basicTextPageWidth)
    {
        attrLength = (CsrUint16)(CsrStrLen(BASIC_TEXT_PAGE_WIDTH) + attrStartLength + attrEndLength + attrLength);
    }
    if (pMsg->colorSupported)
    {
        attrLength = (CsrUint16)(CsrStrLen(COLOR_SUPPORTED) + attrStartLength + attrEndLength + attrLength);
    }
    if (pMsg->documentFormatsSupported)
    {
        attrLength = (CsrUint16)(CsrStrLen(PRINTER_DOCUMENTFORMATS_SUPPORTED) + attrStartLength + attrEndLength + attrLength);
    }
    if (pMsg->imageFormatsSupported)
    {
        attrLength = (CsrUint16)(CsrStrLen(IMAGE_FORMATS_SUPPORTED) + attrStartLength + attrEndLength + attrLength);
    }
    if (pMsg->maxCopiesSupported)
    {
        attrLength = (CsrUint16)(CsrStrLen(MAX_COPIES_SUPPORTED) + attrStartLength + attrEndLength + attrLength);
    }
    if (pMsg->mediaLoaded)
    {
        attrLength = (CsrUint16)(CsrStrLen(MEDIA_LOADED) + attrStartLength + attrEndLength + attrLength);
    }
    if (pMsg->mediaSizesSupported)
    {
        attrLength = (CsrUint16)(CsrStrLen(MEDIA_SIZES_SUPPORTED) + attrStartLength + attrEndLength + attrLength);
    }
    if (pMsg->mediaTypesSupported)
    {
        attrLength = (CsrUint16)(CsrStrLen(MEDIA_TYPES_SUPPORTED) + attrStartLength + attrEndLength + attrLength);
    }
    if (pMsg->numberUpSupported)
    {
        attrLength = (CsrUint16)(CsrStrLen(NUMBER_UP_SUPPORTED) + attrStartLength + attrEndLength + attrLength);
    }
    if (pMsg->orientationsSupported)
    {
        attrLength = (CsrUint16)(CsrStrLen(ORIENTATIONS_SUPPORTED) + attrStartLength + attrEndLength + attrLength);
    }
    if (pMsg->printerGeneralCurrentOperator)
    {
        attrLength = (CsrUint16)(CsrStrLen(PRINTER_GENERAL_CURRENT_OPERATOR) + attrStartLength + attrEndLength + attrLength);
    }
    if (pMsg->printerLocation)
    {
        attrLength = (CsrUint16)(CsrStrLen(PRINTER_LOCATION) + attrStartLength + attrEndLength + attrLength);
    }
    if (pMsg->printerName)
    {
        attrLength = (CsrUint16)(CsrStrLen(PRINTER_NAME) + attrStartLength + attrEndLength + attrLength);
    }
    if (pMsg->printerState)
    {
        attrLength = (CsrUint16)(CsrStrLen(PRINTER_STATE) + attrStartLength + attrEndLength + attrLength);
    }
    if (pMsg->printerStateReasons)
    {
        attrLength = (CsrUint16)(CsrStrLen(PRINTER_STATE_REASONS) + attrStartLength + attrEndLength + attrLength);
    }
    if (pMsg->printQualitySupported)
    {
        attrLength = (CsrUint16)(CsrStrLen(PRINT_QUALITY_SUPPORTED) + attrStartLength + attrEndLength + attrLength);
    }
    if (pMsg->queuedJobCount)
    {
        attrLength = (CsrUint16)(CsrStrLen(QUEUE_JOB_COUNT) + attrStartLength + attrEndLength + attrLength);
    }
    if (pMsg->sidesSupported)
    {
        attrLength = (CsrUint16)(CsrStrLen(SIDES_SUPPORTED) + attrStartLength + attrEndLength + attrLength);
    }

    *getAllAttrs = gettingAllAttrs;
    return attrLength;
}

CsrUint8 * CsrBtBppcWriteSoapEncodedGetPrinterAttrReq(CsrBtBppcGetPrinterAttributesReq *pMsg,
                                                      CsrUint16                        contentLength,
                                                      CsrBool                          gettingAllAttrs,
                                                      CsrUint16                        *soapEncodedLength)
{
    CsrUint32 attributeStartLen, attributeEndLen;
    CsrUint16 bytesInBody       = 3;
    CsrUint32 tempLength        = CsrStrLen(CONTENT_LENGTH);
    CsrUint32 index             = tempLength;
    CsrUint8  *body             = NULL;
    CsrUint16 bodyLength;

    CSR_UNUSED(gettingAllAttrs);
    if (contentLength > 999)
    {
        bytesInBody++;
    }
    bodyLength = (CsrUint16)(tempLength                                +
                             CsrStrLen(HTTP_NEWLINE)                   +
                             CsrStrLen(CONTENT_TYPE)                   +
                             CsrStrLen(SOAP_ACTION_GET_PRINTER_ATTR)   +
                             bytesInBody                               +
                             contentLength);

    body = (CsrUint8 *) CsrPmemAlloc(bodyLength);
    CsrMemCpy(body, CONTENT_LENGTH, tempLength);

    if (contentLength > 999)
    {
        body[index] = ((contentLength / 1000) + '0');
        contentLength = (contentLength % 1000);
        index++;
    }

    body[index] = (CsrUint8) ((contentLength / 100) + '0');
    contentLength = (contentLength % 100);
    index++;
    body[index] = (CsrUint8) ((contentLength / 10) + '0');
    contentLength = (contentLength % 10);
    index++;
    body[index] = (CsrUint8) (contentLength + '0');
    index++;

    tempLength = CsrStrLen(HTTP_NEWLINE);
    CsrMemCpy(&body[index], HTTP_NEWLINE, tempLength);
    index += tempLength;

    tempLength = CsrStrLen(CONTENT_TYPE);
    CsrMemCpy(&body[index], CONTENT_TYPE, tempLength);
    index += tempLength;

    tempLength = CsrStrLen(SOAP_ACTION_GET_PRINTER_ATTR);
    CsrMemCpy(&body[index], SOAP_ACTION_GET_PRINTER_ATTR, tempLength);
    index += tempLength;

    tempLength = CsrStrLen(ENVELOPE_START);
    CsrMemCpy(&body[index], ENVELOPE_START, tempLength);
    index += tempLength;

    tempLength = CsrStrLen(BODY_START_GET_PRINTER_ATTR);
    CsrMemCpy(&body[index], BODY_START_GET_PRINTER_ATTR, tempLength);
    index += tempLength;

    tempLength = CsrStrLen(REQ_PRINTER_ATTRIBUTES_START);
    CsrMemCpy(&body[index], REQ_PRINTER_ATTRIBUTES_START, tempLength);
    index += tempLength;

    attributeStartLen = CsrStrLen(PRINTER_ATTRIBUTE_START);
    attributeEndLen   = CsrStrLen(PRINTER_ATTRIBUTE_END);

    if (pMsg->printerName)
    {
        tempLength = CsrStrLen(PRINTER_NAME);
        CsrMemCpy(&body[index], PRINTER_ATTRIBUTE_START, attributeStartLen);
        index += attributeStartLen;
        CsrMemCpy(&body[index], PRINTER_NAME, tempLength);
        index += tempLength;
        CsrMemCpy(&body[index], PRINTER_ATTRIBUTE_END, attributeEndLen);
        index += attributeEndLen;
    }

    if (pMsg->printerLocation)
    {
        tempLength = CsrStrLen(PRINTER_LOCATION);
        CsrMemCpy(&body[index], PRINTER_ATTRIBUTE_START, attributeStartLen);
        index += attributeStartLen;
        CsrMemCpy(&body[index], PRINTER_LOCATION, tempLength);
        index += tempLength;
        CsrMemCpy(&body[index], PRINTER_ATTRIBUTE_END, attributeEndLen);
        index += attributeEndLen;
    }
    if (pMsg->printerState)
    {
        tempLength = CsrStrLen(PRINTER_STATE);
        CsrMemCpy(&body[index], PRINTER_ATTRIBUTE_START, attributeStartLen);
        index += attributeStartLen;
        CsrMemCpy(&body[index], PRINTER_STATE, tempLength);
        index += tempLength;
        CsrMemCpy(&body[index], PRINTER_ATTRIBUTE_END, attributeEndLen);
        index += attributeEndLen;
    }
    if (pMsg->printerStateReasons)
    {
        tempLength = CsrStrLen(PRINTER_STATE_REASONS);
        CsrMemCpy(&body[index], PRINTER_ATTRIBUTE_START, attributeStartLen);
        index += attributeStartLen;
        CsrMemCpy(&body[index], PRINTER_STATE_REASONS, tempLength);
        index += tempLength;
        CsrMemCpy(&body[index], PRINTER_ATTRIBUTE_END, attributeEndLen);
        index += attributeEndLen;
    }
    if (pMsg->documentFormatsSupported)
    {
        tempLength = CsrStrLen(PRINTER_DOCUMENTFORMATS_SUPPORTED);
        CsrMemCpy(&body[index], PRINTER_ATTRIBUTE_START, attributeStartLen);
        index += attributeStartLen;
        CsrMemCpy(&body[index], PRINTER_DOCUMENTFORMATS_SUPPORTED, tempLength);
        index += tempLength;
        CsrMemCpy(&body[index], PRINTER_ATTRIBUTE_END, attributeEndLen);
        index += attributeEndLen;
    }
    if (pMsg->colorSupported)
    {
        tempLength = CsrStrLen(COLOR_SUPPORTED);
        CsrMemCpy(&body[index], PRINTER_ATTRIBUTE_START, attributeStartLen);
        index += attributeStartLen;
        CsrMemCpy(&body[index], COLOR_SUPPORTED, tempLength);
        index += tempLength;
        CsrMemCpy(&body[index], PRINTER_ATTRIBUTE_END, attributeEndLen);
        index += attributeEndLen;
    }
    if (pMsg->maxCopiesSupported)
    {
        tempLength = CsrStrLen(MAX_COPIES_SUPPORTED);
        CsrMemCpy(&body[index], PRINTER_ATTRIBUTE_START, attributeStartLen);
        index += attributeStartLen;
        CsrMemCpy(&body[index], MAX_COPIES_SUPPORTED, tempLength);
        index += tempLength;
        CsrMemCpy(&body[index], PRINTER_ATTRIBUTE_END, attributeEndLen);
        index += attributeEndLen;
    }
    if (pMsg->sidesSupported)
    {
        tempLength = CsrStrLen(SIDES_SUPPORTED);
        CsrMemCpy(&body[index], PRINTER_ATTRIBUTE_START, attributeStartLen);
        index += attributeStartLen;
        CsrMemCpy(&body[index], SIDES_SUPPORTED, tempLength);
        index += tempLength;
        CsrMemCpy(&body[index], PRINTER_ATTRIBUTE_END, attributeEndLen);
        index += attributeEndLen;
    }
    if (pMsg->numberUpSupported)
    {
        tempLength = CsrStrLen(NUMBER_UP_SUPPORTED);
        CsrMemCpy(&body[index], PRINTER_ATTRIBUTE_START, attributeStartLen);
        index += attributeStartLen;
        CsrMemCpy(&body[index], NUMBER_UP_SUPPORTED, tempLength);
        index += tempLength;
        CsrMemCpy(&body[index], PRINTER_ATTRIBUTE_END, attributeEndLen);
        index += attributeEndLen;
    }
    if (pMsg->orientationsSupported)
    {
        tempLength = CsrStrLen(ORIENTATIONS_SUPPORTED);
        CsrMemCpy(&body[index], PRINTER_ATTRIBUTE_START, attributeStartLen);
        index += attributeStartLen;
        CsrMemCpy(&body[index], ORIENTATIONS_SUPPORTED, tempLength);
        index += tempLength;
        CsrMemCpy(&body[index], PRINTER_ATTRIBUTE_END, attributeEndLen);
        index += attributeEndLen;
    }
    if (pMsg->mediaSizesSupported)
    {
        tempLength = CsrStrLen(MEDIA_SIZES_SUPPORTED);
        CsrMemCpy(&body[index], PRINTER_ATTRIBUTE_START, attributeStartLen);
        index += attributeStartLen;
        CsrMemCpy(&body[index], MEDIA_SIZES_SUPPORTED, tempLength);
        index += tempLength;
        CsrMemCpy(&body[index], PRINTER_ATTRIBUTE_END, attributeEndLen);
        index += attributeEndLen;
    }
    if (pMsg->mediaTypesSupported)
    {
        tempLength = CsrStrLen(MEDIA_TYPES_SUPPORTED);
        CsrMemCpy(&body[index], PRINTER_ATTRIBUTE_START, attributeStartLen);
        index += attributeStartLen;
        CsrMemCpy(&body[index], MEDIA_TYPES_SUPPORTED, tempLength);
        index += tempLength;
        CsrMemCpy(&body[index], PRINTER_ATTRIBUTE_END, attributeEndLen);
        index += attributeEndLen;
    }
    if (pMsg->mediaLoaded)
    {
        tempLength = CsrStrLen(MEDIA_LOADED);
        CsrMemCpy(&body[index], PRINTER_ATTRIBUTE_START, attributeStartLen);
        index += attributeStartLen;
        CsrMemCpy(&body[index], MEDIA_LOADED, tempLength);
        index += tempLength;
        CsrMemCpy(&body[index], PRINTER_ATTRIBUTE_END, attributeEndLen);
        index += attributeEndLen;
    }
    if (pMsg->printQualitySupported)
    {
        tempLength = CsrStrLen(PRINT_QUALITY_SUPPORTED);
        CsrMemCpy(&body[index], PRINTER_ATTRIBUTE_START, attributeStartLen);
        index += attributeStartLen;
        CsrMemCpy(&body[index], PRINT_QUALITY_SUPPORTED, tempLength);
        index += tempLength;
        CsrMemCpy(&body[index], PRINTER_ATTRIBUTE_END, attributeEndLen);
        index += attributeEndLen;
    }
    if (pMsg->queuedJobCount)
    {
        tempLength = CsrStrLen(QUEUE_JOB_COUNT);
        CsrMemCpy(&body[index], PRINTER_ATTRIBUTE_START, attributeStartLen);
        index += attributeStartLen;
        CsrMemCpy(&body[index], QUEUE_JOB_COUNT, tempLength);
        index += tempLength;
        CsrMemCpy(&body[index], PRINTER_ATTRIBUTE_END, attributeEndLen);
        index += attributeEndLen;
    }
    if (pMsg->imageFormatsSupported)
    {
        tempLength = CsrStrLen(IMAGE_FORMATS_SUPPORTED);
        CsrMemCpy(&body[index], PRINTER_ATTRIBUTE_START, attributeStartLen);
        index += attributeStartLen;
        CsrMemCpy(&body[index], IMAGE_FORMATS_SUPPORTED, tempLength);
        index += tempLength;
        CsrMemCpy(&body[index], PRINTER_ATTRIBUTE_END, attributeEndLen);
        index += attributeEndLen;
    }
    if (pMsg->basicTextPageWidth)
    {
        tempLength = CsrStrLen(BASIC_TEXT_PAGE_WIDTH);
        CsrMemCpy(&body[index], PRINTER_ATTRIBUTE_START, attributeStartLen);
        index += attributeStartLen;
        CsrMemCpy(&body[index], BASIC_TEXT_PAGE_WIDTH, tempLength);
        index += tempLength;
        CsrMemCpy(&body[index], PRINTER_ATTRIBUTE_END, attributeEndLen);
        index += attributeEndLen;
    }
    if (pMsg->basicTextPageHeight)
    {
        tempLength = CsrStrLen(BASIC_TEXT_PAGE_HEIGHT);
        CsrMemCpy(&body[index], PRINTER_ATTRIBUTE_START, attributeStartLen);
        index += attributeStartLen;
        CsrMemCpy(&body[index], BASIC_TEXT_PAGE_HEIGHT, tempLength );
        index += tempLength;
        CsrMemCpy(&body[index], PRINTER_ATTRIBUTE_END, attributeEndLen);
        index += attributeEndLen;
    }
    if (pMsg->printerGeneralCurrentOperator)
    {
        tempLength = CsrStrLen(PRINTER_GENERAL_CURRENT_OPERATOR);
        CsrMemCpy(&body[index], PRINTER_ATTRIBUTE_START, attributeStartLen);
        index += attributeStartLen;
        CsrMemCpy(&body[index], PRINTER_GENERAL_CURRENT_OPERATOR, tempLength );
        index += tempLength;
        CsrMemCpy(&body[index], PRINTER_ATTRIBUTE_END, attributeEndLen);
        index += attributeEndLen;
    }

    tempLength = CsrStrLen(REQ_PRINTER_ATTRIBUTES_END);
    CsrMemCpy(&body[index], REQ_PRINTER_ATTRIBUTES_END, tempLength);
    index += tempLength;
    
    CsrMemCpy(&body[index], ENVELOPE_BODY_END_GET_PRINTER_ATTR, CsrStrLen(ENVELOPE_BODY_END_GET_PRINTER_ATTR));
    *soapEncodedLength = bodyLength;
    return (body);
}

CsrUint16 CsrBtBppcGetSoapEncodedCreateJobReqContentLength(CsrBtBppcCreateJobReq *pMsg)
{
    CsrUint16 length;
    CsrUint16 attrLength = (CsrUint16)(CsrStrLen(ENVELOPE_START)            +
                                       CsrStrLen(BODY_START_CREATE_JOB)     +
                                       CsrStrLen(CANCEL_ON_LOST_LINK_START) +
                                       CsrStrLen(CANCEL_ON_LOST_LINK_END)   +
                                       CsrStrLen(ENVELOPE_BODY_END_CREATE_JOB));

    if (pMsg->jobName)
    {
        length = (CsrUint16) CsrStrLen((char*) pMsg->jobName);
        if (length > 0)
        {
            attrLength = (CsrUint16) (attrLength + length + CsrStrLen(JOB_NAME_START)+ CsrStrLen(JOB_NAME_END));
        }
    }
    if (pMsg->jobUserName)
    {
        length = (CsrUint16) CsrStrLen((char*)pMsg->jobUserName);
        if (length > 0)
        {
            attrLength = (CsrUint16)(attrLength + length + CsrStrLen(JOB_ORIGINATING_USER_NAME_START) + CsrStrLen(JOB_ORIGINATING_USER_NAME_END));
        }
    }
    if (pMsg->documentFormat)
    {
        length = (CsrUint16) CsrStrLen((char*) pMsg->documentFormat);
        if (length > 0)
        {
            attrLength = (CsrUint16)(attrLength + length + CsrStrLen(DOCUMENT_FORMAT_START) + CsrStrLen(DOCUMENT_FORMAT_END));
        }
    }
    if (pMsg->mediaSize)
    {
        length = (CsrUint16) CsrStrLen((char*) pMsg->mediaSize);
        if (length > 0)
        {
            attrLength = (CsrUint16)(attrLength + length + CsrStrLen(MEDIA_SIZE_START) + CsrStrLen(MEDIA_SIZE_END));
        }
    }
    if (pMsg->mediaType)
    {
        length = (CsrUint16) CsrStrLen((char*) pMsg->mediaType);
        if (length > 0)
        {
            attrLength = (CsrUint16)(attrLength + length + CsrStrLen(MEDIA_TYPE_START) + CsrStrLen(MEDIA_TYPE_END));
        }
    }
    if (pMsg->cancelOnLostLink)
    {
        attrLength = (CsrUint16)(attrLength + CsrStrLen(CANCEL_ON_LOST_LINK_TRUE));
    }
    else
    {
        attrLength = (CsrUint16)(attrLength + CsrStrLen(CANCEL_ON_LOST_LINK_FALSE));
    }
    if (pMsg->copies > 0)
    {
        attrLength = (CsrUint16)(attrLength + CsrStrLen(COPIES_START) + CsrStrLen(COPIES_END));
        if (pMsg->copies > 99)
        {
            attrLength = (CsrUint16)(attrLength + 3);
        }
        else if (pMsg->copies > 9)
        {
            attrLength = (CsrUint16)(attrLength + 2);
        }
        else
        {
            attrLength = (CsrUint16)(attrLength + 1);
        }
    }
    if (pMsg->numberUp > 0)
    {
        attrLength = (CsrUint16)(attrLength + CsrStrLen(NUMBER_UP_START) + CsrStrLen(NUMBER_UP_END));
        if (pMsg->numberUp > 99)
        {
            attrLength = (CsrUint16)(attrLength + 3);
        }
        else if (pMsg->numberUp > 9)
        {
            attrLength = (CsrUint16)(attrLength + 2);
        }
        else
        {
            attrLength = (CsrUint16)(attrLength + 1);
        }
    }
    if (pMsg->orientation)
    {
        length = (CsrUint16) CsrStrLen((char*) pMsg->orientation);

        if (length > 0)
        {
            attrLength = (CsrUint16)(attrLength + length + CsrStrLen(ORIENTATION_REQUESTED_START) + CsrStrLen(ORIENTATION_REQUESTED_END));
        }
    }
    if (pMsg->printQuality)
    {
        length = (CsrUint16) CsrStrLen((char*) pMsg->printQuality);
        if (length > 0)
        {
            attrLength = (CsrUint16)(attrLength + length + CsrStrLen(PRINT_QUALITY_START) + CsrStrLen(PRINT_QUALITY_END));
        }
    }
    if (pMsg->sides)
    {
        length = (CsrUint16) CsrStrLen((char*) pMsg->sides);
        if (length > 0)
        {
            attrLength = (CsrUint16)(attrLength + length + CsrStrLen(SIDES_START) + CsrStrLen(SIDES_END));
        }
    }
    return attrLength;
}


CsrUint8 * CsrBtBppcWriteSoapEncodedCreateJobReq(CsrBtBppcCreateJobReq  *pMsg,
                                                 CsrUint16              contentLength,
                                                 CsrUint16              *soapEncodedLength)
{
    CsrUint32 length;
    CsrUint16 bytesInBody       = 3;
    CsrUint32 tempLength        = CsrStrLen(CONTENT_LENGTH);
    CsrUint32 index             = tempLength;
    CsrUint8  *body             = NULL;
    CsrUint16 bodyLength;

    if (contentLength > 999)
    {
        bytesInBody++;
    }

    bodyLength    = (CsrUint16)(tempLength                          +
                                CsrStrLen(HTTP_NEWLINE)             +
                                CsrStrLen(CONTENT_TYPE)             +
                                CsrStrLen(SOAP_ACTION_CREATE_JOB)   +
                                bytesInBody                         +
                                contentLength);

    body = (CsrUint8 *) CsrPmemAlloc(bodyLength);

    CsrMemCpy(body, CONTENT_LENGTH, tempLength);


    if (contentLength > 999)
    {
        body[index] = ((contentLength / 1000) + '0');
        contentLength = (contentLength % 1000);
        index++;
    }

    body[index] = (CsrUint8) ((contentLength / 100) + '0');
    contentLength = (contentLength % 100);
    index++;

    body[index] = (CsrUint8) ((contentLength / 10) + '0');
    contentLength = (contentLength % 10);
    index++;

    body[index] = (CsrUint8) (contentLength + '0');
    index++;

    tempLength = CsrStrLen(HTTP_NEWLINE);
    CsrMemCpy(&body[index], HTTP_NEWLINE, tempLength);
    index += tempLength;

    tempLength = CsrStrLen(CONTENT_TYPE);
    CsrMemCpy(&body[index], CONTENT_TYPE, tempLength);
    index += tempLength;

    tempLength = CsrStrLen(SOAP_ACTION_CREATE_JOB);
    CsrMemCpy(&body[index], SOAP_ACTION_CREATE_JOB, tempLength);
    index += tempLength;

    tempLength = CsrStrLen(ENVELOPE_START);
    CsrMemCpy(&body[index], ENVELOPE_START, tempLength);
    index += tempLength;

    tempLength = CsrStrLen(BODY_START_CREATE_JOB);
    CsrMemCpy(&body[index], BODY_START_CREATE_JOB, tempLength);
    index += tempLength;

    if (pMsg->jobName)
    {
        length = (CsrUint16) CsrStrLen((char*) pMsg->jobName);
        if (length > 0)
        {
            tempLength = CsrStrLen(JOB_NAME_START);
            CsrMemCpy(&body[index], JOB_NAME_START, tempLength);
            index += tempLength;
            CsrMemCpy(&body[index], pMsg->jobName, length);
            index += length;
            tempLength = CsrStrLen(JOB_NAME_END);
            CsrMemCpy(&body[index], JOB_NAME_END, tempLength);
            index += tempLength;
        }
        CsrPmemFree(pMsg->jobName);
    }
    if (pMsg->jobUserName)
    {
        length = (CsrUint16) CsrStrLen((char*) pMsg->jobUserName);
        if (length > 0)
        {
            tempLength = CsrStrLen(JOB_ORIGINATING_USER_NAME_START);
            CsrMemCpy(&body[index], JOB_ORIGINATING_USER_NAME_START, tempLength);
            index += tempLength;
            CsrMemCpy(&body[index], pMsg->jobUserName, length);
            index += length;
            tempLength = CsrStrLen(JOB_ORIGINATING_USER_NAME_END);
            CsrMemCpy(&body[index], JOB_ORIGINATING_USER_NAME_END, tempLength);
            index += tempLength;
        }
        CsrPmemFree(pMsg->jobUserName);
    }
    if (pMsg->documentFormat)
    {
        length = (CsrUint16) CsrStrLen((char*) pMsg->documentFormat);
        if (length > 0)
        {
            tempLength = CsrStrLen(DOCUMENT_FORMAT_START);
            CsrMemCpy(&body[index], DOCUMENT_FORMAT_START, tempLength);
            index += tempLength;
            CsrMemCpy(&body[index], pMsg->documentFormat, length);
            index += length;
            tempLength = CsrStrLen(DOCUMENT_FORMAT_END);
            CsrMemCpy(&body[index], DOCUMENT_FORMAT_END, tempLength);
            index += tempLength;
        }
        CsrPmemFree(pMsg->documentFormat);
    }
    if (pMsg->copies > 0)
    {
        tempLength = CsrStrLen(COPIES_START);
        CsrMemCpy(&body[index], COPIES_START, tempLength);
        index += tempLength;
        if (pMsg->copies > 99)
        {
            body[index] = (CsrUint8) ((pMsg->copies / 100) + '0');
            pMsg->copies = (pMsg->copies % 100);
            index++;
        }
        if (pMsg->copies > 9)
        {
            body[index] = (CsrUint8) ((pMsg->copies / 10) + '0');
            pMsg->copies = (pMsg->copies % 10);
            index++;
        }
        body[index] = (CsrUint8) (pMsg->copies + '0');
        index++;
        tempLength = CsrStrLen(COPIES_END);
        CsrMemCpy(&body[index], COPIES_END, tempLength);
        index += tempLength;
    }
    if (pMsg->sides)
    {
        length = (CsrUint16) CsrStrLen((char*) pMsg->sides);
        if (length > 0)
        {
            tempLength = CsrStrLen(SIDES_START);
            CsrMemCpy(&body[index], SIDES_START, tempLength);
            index += tempLength;
            CsrMemCpy(&body[index], pMsg->sides, length);
            index += length;
            tempLength = CsrStrLen(SIDES_END);
            CsrMemCpy(&body[index], SIDES_END, tempLength);
            index += tempLength;
        }
        CsrPmemFree(pMsg->sides);
    }
    if (pMsg->numberUp > 0)
    {
        tempLength = CsrStrLen(NUMBER_UP_START);
        CsrMemCpy(&body[index], NUMBER_UP_START, tempLength);
        index += tempLength;
        if (pMsg->numberUp > 99)
        {
            body[index] = (CsrUint8) ((pMsg->numberUp / 100) + '0');
            pMsg->numberUp = (pMsg->numberUp % 100);
            index++;
        }
        if (pMsg->numberUp > 9)
        {
            body[index] = (CsrUint8) ((pMsg->numberUp / 10) + '0');
            pMsg->numberUp = (pMsg->numberUp % 10);
            index++;
        }
        body[index] = (CsrUint8) (pMsg->numberUp + '0');
        index++;
        tempLength = CsrStrLen(NUMBER_UP_END);
        CsrMemCpy(&body[index], NUMBER_UP_END, tempLength);
        index += tempLength;
    }
    if (pMsg->orientation)
    {
        length = (CsrUint16) CsrStrLen((char*) pMsg->orientation);
        if (length > 0)
        {
            tempLength = CsrStrLen(ORIENTATION_REQUESTED_START);
            CsrMemCpy(&body[index], ORIENTATION_REQUESTED_START, tempLength);
            index += tempLength;
            CsrMemCpy(&body[index], pMsg->orientation, length);
            index += length;
            tempLength = CsrStrLen(ORIENTATION_REQUESTED_END);
            CsrMemCpy(&body[index], ORIENTATION_REQUESTED_END, tempLength);
            index += tempLength;
        }
        CsrPmemFree(pMsg->orientation);
    }
    if (pMsg->mediaSize)
    {
        length = (CsrUint16) CsrStrLen((char*) pMsg->mediaSize);
        if (length > 0)
        {
            tempLength = CsrStrLen(MEDIA_SIZE_START);
            CsrMemCpy(&body[index], MEDIA_SIZE_START, tempLength);
            index += tempLength;
            CsrMemCpy(&body[index], pMsg->mediaSize, length);
            index += length;
            tempLength = CsrStrLen(MEDIA_SIZE_END);
            CsrMemCpy(&body[index], MEDIA_SIZE_END, tempLength);
            index += tempLength;
        }
        CsrPmemFree(pMsg->mediaSize);
    }
    if (pMsg->mediaType)
    {
        length = (CsrUint16) CsrStrLen((char*) pMsg->mediaType);
        if (length > 0)
        {
            tempLength = CsrStrLen(MEDIA_TYPE_START);
            CsrMemCpy(&body[index], MEDIA_TYPE_START, tempLength);
            index += tempLength;
            CsrMemCpy(&body[index], pMsg->mediaType, length);
            index += length;
            tempLength = CsrStrLen(MEDIA_TYPE_END);
            CsrMemCpy(&body[index], MEDIA_TYPE_END, tempLength);
            index += tempLength;
        }
        CsrPmemFree(pMsg->mediaType);
    }
    if (pMsg->printQuality)
    {
        length = (CsrUint16) CsrStrLen((char*) pMsg->printQuality);
        if (length > 0)
        {
            tempLength = CsrStrLen(PRINT_QUALITY_START);
            CsrMemCpy(&body[index], PRINT_QUALITY_START, tempLength);
            index += tempLength;
            CsrMemCpy(&body[index], pMsg->printQuality, length);
            index += length;
            tempLength = CsrStrLen(PRINT_QUALITY_END);
            CsrMemCpy(&body[index], PRINT_QUALITY_END, tempLength);
            index += tempLength;
        }
        CsrPmemFree(pMsg->printQuality);
    }
    tempLength = CsrStrLen(CANCEL_ON_LOST_LINK_START);
    CsrMemCpy(&body[index], CANCEL_ON_LOST_LINK_START, tempLength);
    index += tempLength;
    if (pMsg->cancelOnLostLink)
    {
        tempLength = CsrStrLen(CANCEL_ON_LOST_LINK_TRUE);
        CsrMemCpy(&body[index], CANCEL_ON_LOST_LINK_TRUE, tempLength);
        index += tempLength;
    }
    else
    {
        tempLength = CsrStrLen(CANCEL_ON_LOST_LINK_FALSE);
        CsrMemCpy(&body[index], CANCEL_ON_LOST_LINK_FALSE, tempLength);
        index += tempLength;
    }
    tempLength = CsrStrLen(CANCEL_ON_LOST_LINK_END);
    CsrMemCpy(&body[index], CANCEL_ON_LOST_LINK_END, tempLength);
    index += tempLength;

    CsrMemCpy(&body[index], ENVELOPE_BODY_END_CREATE_JOB, CsrStrLen(ENVELOPE_BODY_END_CREATE_JOB));
    *soapEncodedLength = bodyLength;
    return (body);
}

CsrUint16 CsrBtBppcGetSoapEncodedGetJobAttrReqContentLength(CsrBtBppcGetJobAttributesReq *pMsg)
{
    char jobIdLength[11];
    CsrUint16 attrLength         = (CsrUint16)(CsrStrLen(ENVELOPE_START)            +
                                               CsrStrLen(BODY_START_GET_JOB_ATTR)   +
                                               CsrStrLen(REQ_JOB_ATTRIBUTES_START)  +
                                               CsrStrLen(REQ_JOB_ATTRIBUTES_END)    +
                                               CsrStrLen(ENVELOPE_BODY_END_GET_JOB_ATTR));

    CsrUint16 attrStartLength    = (CsrUint16) CsrStrLen(JOB_ATTR_START);
    CsrUint16 attrEndLength      = (CsrUint16) CsrStrLen(JOB_ATTR_END);

    CsrIntToBase10(pMsg->jobId, jobIdLength);
    attrLength = (CsrUint16)(attrLength + CsrStrLen(JOBID_START) + CsrStrLen(JOBID_END) + CsrStrLen(jobIdLength));

    if (pMsg->jobState)
    {
        attrLength = (CsrUint16)(attrLength + CsrStrLen(JOB_STATE) + attrStartLength + attrEndLength);
    }
    if (pMsg->jobName)
    {
        attrLength = (CsrUint16)(attrLength + CsrStrLen(JOB_NAME) + attrStartLength + attrEndLength);
    }
    if (pMsg->jobOriginatingUserName)
    {
        attrLength = (CsrUint16)(attrLength + CsrStrLen(JOB_ORIGINATING_USER_NAME) + attrStartLength + attrEndLength);
    }
    if (pMsg->jobMediaSheetsCompleted)
    {
        attrLength = (CsrUint16)(attrLength + CsrStrLen(JOB_MEDIA_SHEETS_COMPLETED) + attrStartLength + attrEndLength);
    }
    if (pMsg->numberOfInterveningJobs)
    {
        attrLength = (CsrUint16)(attrLength + CsrStrLen(NUMBER_OF_INTERVENING_JOBS) + attrStartLength + attrEndLength);
    }
    return attrLength;
}


CsrUint8 * CsrBtBppcWriteSoapEncodedGetJobAttrReq(CsrBtBppcGetJobAttributesReq *pMsg,
                                                  CsrUint16                    contentLength,
                                                  CsrUint16                    *soapEncodedLength)
{

    CsrUint16   bodyLength;
    char        jobIdString[11];
    CsrUint16   bytesInBody         = 3;
    CsrUint8    *body               = NULL;
    CsrUint32   tempLength          = CsrStrLen(CONTENT_LENGTH);
    CsrUint32   index               = tempLength;
    CsrUint32   attributeStartLen   = CsrStrLen(JOB_ATTR_START);
    CsrUint32   attributeEndLen     = CsrStrLen(JOB_ATTR_END);

    if (contentLength > 999)
    {
        bytesInBody++;
    }

    bodyLength  = (CsrUint16)(tempLength                            +
                              CsrStrLen(HTTP_NEWLINE)               +
                              CsrStrLen(CONTENT_TYPE)               +
                              CsrStrLen(SOAP_ACTION_GET_JOB_ATTR )  +
                              bytesInBody                           +
                              contentLength);

    body = (CsrUint8 *) CsrPmemAlloc(bodyLength);

    CsrMemCpy(body, CONTENT_LENGTH, tempLength);

    if (contentLength > 999)
    {
        body[index] = ((contentLength / 1000) + '0');
        contentLength = (contentLength % 1000);
        index++;
    }

    body[index] = (CsrUint8) ((contentLength / 100) + '0');
    contentLength = (contentLength % 100);
    index++;

    body[index] = (CsrUint8) ((contentLength / 10) + '0');
    contentLength = (contentLength % 10);
    index++;

    body[index] = (CsrUint8) (contentLength + '0');
    index++;

    tempLength = CsrStrLen(HTTP_NEWLINE);
    CsrMemCpy(&body[index], HTTP_NEWLINE, tempLength);
    index += tempLength;

    tempLength = CsrStrLen(CONTENT_TYPE);
    CsrMemCpy(&body[index], CONTENT_TYPE, tempLength);
    index += tempLength;

    tempLength = CsrStrLen(SOAP_ACTION_GET_JOB_ATTR);
    CsrMemCpy(&body[index], SOAP_ACTION_GET_JOB_ATTR, tempLength);
    index += tempLength;

    tempLength = CsrStrLen(ENVELOPE_START);
    CsrMemCpy(&body[index], ENVELOPE_START, tempLength);
    index += tempLength;

    tempLength = CsrStrLen(BODY_START_GET_JOB_ATTR);
    CsrMemCpy(&body[index], BODY_START_GET_JOB_ATTR, tempLength);
    index += tempLength;

    tempLength = CsrStrLen(JOBID_START);
    CsrMemCpy(&body[index], JOBID_START, tempLength);
    index += tempLength;


    CsrIntToBase10(pMsg->jobId, jobIdString);
    tempLength = CsrStrLen(jobIdString);
    CsrMemCpy(&body[index], jobIdString, tempLength);
    index += tempLength;

    tempLength = CsrStrLen(JOBID_END);
    CsrMemCpy(&body[index], JOBID_END, tempLength);
    index += tempLength;

    tempLength = CsrStrLen(REQ_JOB_ATTRIBUTES_START);
    CsrMemCpy(&body[index], REQ_JOB_ATTRIBUTES_START, tempLength);
    index += tempLength;

    if (pMsg->jobState)
    {
        tempLength = CsrStrLen(JOB_STATE);
        CsrMemCpy(&body[index], JOB_ATTR_START, attributeStartLen);
        index += attributeStartLen;
        CsrMemCpy(&body[index], JOB_STATE, tempLength);
        index += tempLength;
        CsrMemCpy(&body[index], JOB_ATTR_END, attributeEndLen);
        index += attributeEndLen;
    }

    if (pMsg->jobName)
    {
        tempLength = CsrStrLen(JOB_NAME);
        CsrMemCpy(&body[index], JOB_ATTR_START, attributeStartLen);
        index += attributeStartLen;
        CsrMemCpy(&body[index], JOB_NAME, tempLength);
        index += tempLength;
        CsrMemCpy(&body[index], JOB_ATTR_END, attributeEndLen);
        index += attributeEndLen;
    }

    if (pMsg->jobOriginatingUserName)
    {
        tempLength = CsrStrLen(JOB_ORIGINATING_USER_NAME);
        CsrMemCpy(&body[index], JOB_ATTR_START, attributeStartLen);
        index += attributeStartLen;
        CsrMemCpy(&body[index], JOB_ORIGINATING_USER_NAME, tempLength);
        index += tempLength;
        CsrMemCpy(&body[index], JOB_ATTR_END, attributeEndLen);
        index += attributeEndLen;
    }

    if (pMsg->jobMediaSheetsCompleted)
    {
        tempLength = CsrStrLen(JOB_MEDIA_SHEETS_COMPLETED);
        CsrMemCpy(&body[index], JOB_ATTR_START, attributeStartLen);
        index += attributeStartLen;
        CsrMemCpy(&body[index], JOB_MEDIA_SHEETS_COMPLETED, tempLength);
        index += tempLength;
        CsrMemCpy(&body[index], JOB_ATTR_END, attributeEndLen);
        index += attributeEndLen;
    }

    if (pMsg->numberOfInterveningJobs)
    {
        tempLength = CsrStrLen(NUMBER_OF_INTERVENING_JOBS);
        CsrMemCpy(&body[index], JOB_ATTR_START, attributeStartLen);
        index += attributeStartLen;
        CsrMemCpy(&body[index], NUMBER_OF_INTERVENING_JOBS, tempLength);
        index += tempLength;
        CsrMemCpy(&body[index], JOB_ATTR_END, attributeEndLen);
        index += attributeEndLen;
    }

    tempLength = CsrStrLen(REQ_JOB_ATTRIBUTES_END);
    CsrMemCpy(&body[index], REQ_JOB_ATTRIBUTES_END, tempLength);
    index += tempLength;

    CsrMemCpy(&body[index], ENVELOPE_BODY_END_GET_JOB_ATTR, CsrStrLen(ENVELOPE_BODY_END_GET_JOB_ATTR));
    *soapEncodedLength = bodyLength;
    return (body);
}

CsrUint16 CsrBtBppcGetSoapEncodedCancelJobReqContentLength(CsrUint32 jobId)
{
    char jobIdLength[11];

    CsrUint16 attrLength = (CsrUint16)(CsrStrLen(ENVELOPE_START)+
                                       CsrStrLen(BODY_START_CANCEL_JOB) +
                                       CsrStrLen(ENVELOPE_BODY_END_CANCEL_JOB) +
                                       CsrStrLen(JOBID_START) +
                                       CsrStrLen(JOBID_END));

    CsrIntToBase10(jobId, jobIdLength);
    attrLength = (CsrUint16)(attrLength + CsrStrLen(jobIdLength));
    return attrLength;
}

CsrUint8 * CsrBtBppcWriteSoapEncodedCancelJobReq(CsrUint16        contentLength,
                                                 CsrUint32        jobId,
                                                 CsrUint16        *soapEncodedLength)
{
    CsrUint16   bodyLength;
    char        jobIdString[11];

    CsrUint8    *body       = NULL;
    CsrUint32   bytesInBody = 3;
    CsrUint32   tempLength  = CsrStrLen(CONTENT_LENGTH);
    CsrUint32   index       = tempLength;

    if (contentLength > 999)
    {
        bytesInBody++;
    }

    bodyLength  = (CsrUint16)(tempLength                        +
                              CsrStrLen(HTTP_NEWLINE)           +
                              CsrStrLen(CONTENT_TYPE)           +
                              CsrStrLen(SOAP_ACTION_CANCEL_JOB) +
                              bytesInBody                       +
                              contentLength);

    body = (CsrUint8 *) CsrPmemAlloc(bodyLength);

    CsrMemCpy(body, CONTENT_LENGTH, tempLength);

    if (contentLength > 999)
    {
        body[index] = ((contentLength / 1000) + '0');
        contentLength = (contentLength % 1000);
        index++;
    }

    body[index] = (CsrUint8) ((contentLength / 100) + '0');
    contentLength = (contentLength % 100);
    index++;

    body[index] = (CsrUint8) ((contentLength / 10) + '0');
    contentLength = (contentLength % 10);
    index++;

    body[index] = (CsrUint8) (contentLength + '0');
    index++;

    tempLength = CsrStrLen(HTTP_NEWLINE);
    CsrMemCpy(&body[index], HTTP_NEWLINE, tempLength);
    index += tempLength;

    tempLength = CsrStrLen(CONTENT_TYPE);
    CsrMemCpy(&body[index], CONTENT_TYPE, tempLength);
    index += tempLength;

    tempLength = CsrStrLen(SOAP_ACTION_CANCEL_JOB);
    CsrMemCpy(&body[index], SOAP_ACTION_CANCEL_JOB, tempLength);
    index += tempLength;

    tempLength = CsrStrLen(ENVELOPE_START);
    CsrMemCpy(&body[index], ENVELOPE_START, tempLength);
    index += tempLength;

    tempLength = CsrStrLen(BODY_START_CANCEL_JOB);
    CsrMemCpy(&body[index], BODY_START_CANCEL_JOB, tempLength);
    index += tempLength;

    tempLength = CsrStrLen(JOBID_START);
    CsrMemCpy(&body[index], JOBID_START, tempLength);
    index += tempLength;

    CsrIntToBase10(jobId, jobIdString);
    tempLength = CsrStrLen(jobIdString);
    CsrMemCpy(&body[index], jobIdString, tempLength);
    index += tempLength;

    tempLength = CsrStrLen(JOBID_END);
    CsrMemCpy(&body[index], JOBID_END, tempLength);
    index += tempLength;
    CsrMemCpy(&body[index], ENVELOPE_BODY_END_CANCEL_JOB, CsrStrLen(ENVELOPE_BODY_END_CANCEL_JOB));
    *soapEncodedLength = bodyLength;
    return (body);
}

CsrUint16 CsrBtBppcGetSoapEncodedGetEventReqContentLength(CsrUint32 jobId)
{
    char jobIdLength[11];

    CsrUint16 attrLength = (CsrUint16)(CsrStrLen(ENVELOPE_START)                +
                                       CsrStrLen(BODY_START_GET_EVENT)          +
                                       CsrStrLen(ENVELOPE_BODY_END_GET_EVENT)   +
                                       CsrStrLen(JOBID_START)                   +
                                       CsrStrLen(JOBID_END));

    CsrIntToBase10(jobId, jobIdLength);
    attrLength = (CsrUint16)(attrLength + CsrStrLen(jobIdLength));
    return attrLength;
}


CsrUint8 * CsrBtBppcWriteSoapEncodedGetEventReq(CsrUint16        contentLength,
                                                CsrUint32        jobId,
                                                CsrUint16        *soapEncodedLength)
{
    CsrUint16  bodyLength;
    char       jobIdString[11];
    CsrUint8   *body       = NULL;
    CsrUint32  bytesInBody = 3;
    CsrUint32  tempLength  = CsrStrLen(CONTENT_LENGTH);
    CsrUint32  index       = tempLength;

    if (contentLength > 999)
    {
        bytesInBody++;
    }

    bodyLength = (CsrUint16)(tempLength                         +
                             CsrStrLen(HTTP_NEWLINE)            +
                             CsrStrLen(CONTENT_TYPE)            +
                             CsrStrLen(SOAP_ACTION_GET_EVENT)   +
                             bytesInBody                        +
                             contentLength);

    body = (CsrUint8 *) CsrPmemAlloc(bodyLength);

    CsrMemCpy(body, CONTENT_LENGTH, tempLength);

    if (contentLength > 999)
    {
        body[index] = ((contentLength / 1000) + '0');
        contentLength = (contentLength % 1000);
        index++;
    }

    body[index] = (CsrUint8) ((contentLength / 100) + '0');
    contentLength = (contentLength % 100);
    index++;

    body[index] = (CsrUint8) ((contentLength / 10) + '0');
    contentLength = (contentLength % 10);
    index++;

    body[index] = (CsrUint8) (contentLength + '0');
    index++;

    tempLength = CsrStrLen(HTTP_NEWLINE);
    CsrMemCpy(&body[index], HTTP_NEWLINE, tempLength);
    index += tempLength;

    tempLength = CsrStrLen(CONTENT_TYPE);
    CsrMemCpy(&body[index], CONTENT_TYPE, tempLength);
    index += tempLength;

    tempLength = CsrStrLen(SOAP_ACTION_GET_EVENT);
    CsrMemCpy(&body[index], SOAP_ACTION_GET_EVENT, tempLength);
    index += tempLength;

    tempLength = CsrStrLen(ENVELOPE_START);
    CsrMemCpy(&body[index], ENVELOPE_START, tempLength);
    index += tempLength;

    tempLength = CsrStrLen(BODY_START_GET_EVENT);
    CsrMemCpy(&body[index], BODY_START_GET_EVENT, tempLength);
    index += tempLength;

    tempLength = CsrStrLen(JOBID_START);
    CsrMemCpy(&body[index], JOBID_START, tempLength);
    index += tempLength;

    CsrIntToBase10(jobId, jobIdString);
    tempLength = CsrStrLen(jobIdString);
    CsrMemCpy(&body[index], jobIdString, tempLength);
    index += tempLength;

    tempLength = CsrStrLen(JOBID_END);
    CsrMemCpy(&body[index], JOBID_END, tempLength);
    index += tempLength;

    CsrMemCpy(&body[index], ENVELOPE_BODY_END_GET_EVENT, CsrStrLen(ENVELOPE_BODY_END_GET_EVENT));
    *soapEncodedLength = bodyLength;
    return (body);
}

