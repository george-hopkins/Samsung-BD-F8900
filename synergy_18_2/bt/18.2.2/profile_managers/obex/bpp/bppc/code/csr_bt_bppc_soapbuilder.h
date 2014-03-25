#ifndef CSR_BT_BPPC_SOAPBUILDER_H__
#define CSR_BT_BPPC_SOAPBUILDER_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/* Defines used both for GetPrinterAttributes request and CreateJob request */
#define CONTENT_LENGTH                           "CONTENT-LENGTH: "
#define CONTENT_TYPE                             "CONTENT-TYPE: text/xml; charset=\"utf-8\"\r\n"
#define HTTP_NEWLINE                             "\r\n"
#define ENVELOPE_START                           "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">\n"

/* Defines used for GetPrinterAttributes request */
#define SOAP_ACTION_GET_PRINTER_ATTR              "SOAPACTION:\"urn:schemas-bluetooth-org:service:Printer:1#GetPrinterAttributes\"\r\n\r\n"
#define BODY_START_GET_PRINTER_ATTR               "<s:Body>\n<u:GetPrinterAttributes xmlns:u=\"urn:schemas-bluetooth-org:service:Printer:1\">\n"
#define ENVELOPE_BODY_END_GET_PRINTER_ATTR        "</u:GetPrinterAttributes>\n</s:Body>\n</s:Envelope>"
#define REQ_PRINTER_ATTRIBUTES_START              "<RequestedPrinterAttributes>\n"
#define REQ_PRINTER_ATTRIBUTES_END                "</RequestedPrinterAttributes>\n"
#define PRINTER_ATTRIBUTE_START                   "<PrinterAttribute>"
#define PRINTER_ATTRIBUTE_END                     "</PrinterAttribute>\n"
#define PRINTER_NAME                              "PrinterName"
#define PRINTER_LOCATION                          "PrinterLocation"
#define PRINTER_STATE                             "PrinterState"
#define PRINTER_STATE_REASONS                     "PrinterStateReasons"
#define PRINTER_DOCUMENTFORMATS_SUPPORTED         "DocumentFormatsSupported"
#define COLOR_SUPPORTED                           "ColorSupported"
#define MAX_COPIES_SUPPORTED                      "MaxCopiesSupported"
#define SIDES_SUPPORTED                           "SidesSupported"
#define NUMBER_UP_SUPPORTED                       "NumberUpSupported"
#define ORIENTATIONS_SUPPORTED                    "OrientationsSupported"
#define MEDIA_SIZES_SUPPORTED                     "MediaSizesSupported"
#define MEDIA_TYPES_SUPPORTED                     "MediaTypesSupported"
#define MEDIA_LOADED                              "MediaLoaded"
#define PRINT_QUALITY_SUPPORTED                   "PrintQualitySupported"
#define QUEUE_JOB_COUNT                           "QueuedJobCount"
#define IMAGE_FORMATS_SUPPORTED                   "ImageFormatsSupported"
#define BASIC_TEXT_PAGE_WIDTH                     "BasicTextPageWidth"
#define BASIC_TEXT_PAGE_HEIGHT                    "BasicTextPageHeight"
#define PRINTER_GENERAL_CURRENT_OPERATOR          "PrinterGeneralCurrentOperator"

/*Defines used for GetJobAttributes request , CancelJob request, and getEvent request*/
#define JOBID_START                               "<JobId>"
#define JOBID_END                                 "</JobId>\n"

/* Defines used for GetJobAttributes request */
#define SOAP_ACTION_GET_JOB_ATTR                  "SOAPACTION:\"urn:schemas-bluetooth-org:service:Printer:1#GetJobAttributes\"\r\n\r\n"
#define BODY_START_GET_JOB_ATTR                   "<s:Body>\n<u:GetJobAttributes xmlns:u=\"urn:schemas-bluetooth-org:service:Printer:1\">\n"
#define ENVELOPE_BODY_END_GET_JOB_ATTR            "</u:GetJobAttributes>\n</s:Body>\n</s:Envelope>"
#define REQ_JOB_ATTRIBUTES_START                  "<RequestedJobAttributes>\n"
#define REQ_JOB_ATTRIBUTES_END                    "</RequestedJobAttributes>\n"
#define JOB_ATTR_START                            "<JobAttribute>"
#define JOB_ATTR_END                              "</JobAttribute>\n"
#define JOB_STATE                                 "JobState"
#define JOB_NAME                                  "JobName"
#define JOB_ORIGINATING_USER_NAME                 "JobOriginatingUserName"
#define JOB_MEDIA_SHEETS_COMPLETED                "JobMediaSheetsCompleted"
#define NUMBER_OF_INTERVENING_JOBS                "NumberOfInterveningJobs"

/* Defines used for CancelJob request */
#define SOAP_ACTION_CANCEL_JOB                    "SOAPACTION:\"urn:schemas-bluetooth-org:service:Printer:1#CancelJob\"\r\n\r\n"
#define BODY_START_CANCEL_JOB                     "<s:Body>\n<u:CancelJob xmlns:u=\"urn:schemas-bluetooth-org:service:Printer:1\">\n"
#define ENVELOPE_BODY_END_CANCEL_JOB              "</u:CancelJob>\n </s:Body>\n</s:Envelope>"

/* Define used for getEvent request */
#define SOAP_ACTION_GET_EVENT                     "SOAPACTION:\"urn:schemas-bluetooth-org:service:Printer:1#GetEvent\"\r\n\r\n"
#define BODY_START_GET_EVENT                      "<s:Body>\n<u:GetEvent xmlns:u=\"urn:schemas-bluetooth-org:service:Printer:1\">\n"
#define ENVELOPE_BODY_END_GET_EVENT               "</u:GetEvent>\n </s:Body>\n</s:Envelope>"

/* Defines used for CreateJob request */
#define SOAP_ACTION_CREATE_JOB                    "SOAPACTION:\"urn:schemas-bluetooth-org:service:Printer:1#CreateJob\"\r\n\r\n"
#define BODY_START_CREATE_JOB                     "<s:Body>\n<u:CreateJob xmlns:u=\"urn:schemas-bluetooth-org:service:Printer:1\">\n"
#define ENVELOPE_BODY_END_CREATE_JOB              "</u:CreateJob>\n </s:Body>\n</s:Envelope>"
#define JOB_NAME_START                            "<JobName>"
#define JOB_NAME_END                              "</JobName>\n"
#define JOB_ORIGINATING_USER_NAME_START           "<JobOriginatingUserName>"
#define JOB_ORIGINATING_USER_NAME_END             "</JobOriginatingUserName>\n"
#define DOCUMENT_FORMAT_START                     "<DocumentFormat>"
#define DOCUMENT_FORMAT_END                       "</DocumentFormat>\n"
#define COPIES_START                              "<Copies>"
#define COPIES_END                                "</Copies>\n"
#define SIDES_START                               "<Sides>"
#define SIDES_END                                 "</Sides>\n"
#define NUMBER_UP_START                           "<NumberUp>"
#define NUMBER_UP_END                             "</NumberUp>\n"
#define ORIENTATION_REQUESTED_START               "<OrientationRequested>"
#define ORIENTATION_REQUESTED_END                 "</OrientationRequested>\n"
#define MEDIA_SIZE_START                          "<MediaSize>"
#define MEDIA_SIZE_END                            "</MediaSize>\n"
#define MEDIA_TYPE_START                          "<MediaType>"
#define MEDIA_TYPE_END                            "</MediaType>\n"
#define PRINT_QUALITY_START                       "<PrintQuality>"
#define PRINT_QUALITY_END                         "</PrintQuality>\n"
#define CANCEL_ON_LOST_LINK_START                 "<CancelOnLostLink>"
#define CANCEL_ON_LOST_LINK_END                   "</CancelOnLostLink>\n"
#define CANCEL_ON_LOST_LINK_TRUE                  "true"
#define CANCEL_ON_LOST_LINK_FALSE                 "false"
#define SIDES_ONE_SIDED                           "one-sided"
#define SIDES_TWO_SIDED_LONG_EDGE                 "two-sided-long-edge"
#define SIDES_TWO_SIDED_SHORT_EDGE                "two-sided-short-edge"
#define ORIENTATION_REQUESTED_PORTRAIT            "portrait"
#define ORIENTATION_REQUESTED_LANDSCAPE           "landscape"
#define ORIENTATION_REQUESTED_REVERSE_LANDSCAPE   "reverse-landscape"
#define ORIENTATION_REQUESTED_REVERSE_PORTRAIT    "reverse-portrait"
#define PRINT_QUALITY_DRAFT                       "draft"
#define PRINT_QUALITY_NORMAL                      "normal"
#define PRINT_QUALITY_HIGH                        "high"

/* Prototypes */
CsrUint16  CsrBtBppcGetSoapEncodedGetPrinterAttrReqContentLength(CsrBtBppcGetPrinterAttributesReq   *pMsg,
                                                                 CsrBool                            *getAllAttrs);

CsrUint8 * CsrBtBppcWriteSoapEncodedGetPrinterAttrReq(CsrBtBppcGetPrinterAttributesReq *pMsg,
                                                      CsrUint16                        contentLength,
                                                      CsrBool                          gettingAllAttrs,
                                                      CsrUint16                        *soapEncodedLength);

CsrUint16  CsrBtBppcGetSoapEncodedCreateJobReqContentLength(CsrBtBppcCreateJobReq *pMsg);

CsrUint8 * CsrBtBppcWriteSoapEncodedCreateJobReq(CsrBtBppcCreateJobReq  *pMsg,
                                                 CsrUint16              contentLength,
                                                 CsrUint16              *soapEncodedLength);

CsrUint16  CsrBtBppcGetSoapEncodedGetJobAttrReqContentLength(CsrBtBppcGetJobAttributesReq *pMsg);

CsrUint8 * CsrBtBppcWriteSoapEncodedGetJobAttrReq(CsrBtBppcGetJobAttributesReq *pMsg,
                                                  CsrUint16                    contentLength,
                                                  CsrUint16                    *soapEncodedLength);

CsrUint16  CsrBtBppcGetSoapEncodedCancelJobReqContentLength(CsrUint32 jobId);

CsrUint8 * CsrBtBppcWriteSoapEncodedCancelJobReq(CsrUint16        contentLength,
                                                 CsrUint32        jobId,
                                                 CsrUint16        *soapEncodedLength);

CsrUint16  CsrBtBppcGetSoapEncodedGetEventReqContentLength(CsrUint32 jobId);

CsrUint8 * CsrBtBppcWriteSoapEncodedGetEventReq(CsrUint16        contentLength,
                                                CsrUint32        jobId,
                                                CsrUint16        *soapEncodedLength);

#ifdef __cplusplus
}
#endif

#endif /* _BPPC_SOAP_BUILDER_H */
