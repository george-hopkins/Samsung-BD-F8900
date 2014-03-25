#ifndef CSR_BT_BPPS_SOAPBUILDER_H__
#define CSR_BT_BPPS_SOAPBUILDER_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#define CONTENT_LENGTH            "CONTENT-LENGTH: "
#define SIZE_OF_CONTENT_LENGTH    (16)
#define CONTENT_TYPE            "CONTENT-TYPE: text/xml; charset=\"utf-8\"\r\n"
#define SIZE_OF_CONTENT_TYPE    (41)
#define HTTP_NEWLINE            "\r\n"
#define SIZE_OF_HTTP_NEWLINE    (2)

#ifdef __cplusplus
extern "C" {
#endif

void soapAddTagValue(char* soapString, char* tagId, char* tagValue);
void soapAddMultiTagValues(char * soapString, char * tagId, char * tagValues, CsrUint8 numOfTags);
void soapAddMultiTagValuesNoS(char * soapString, char * tagId, char * tagValues, CsrUint8 numOfTags);
void soapAddMediaLoaded(char * soapString, char * tagValues, CsrUint8 numOfTags);

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_BPPS_SOAPBUILDER_H__ */
