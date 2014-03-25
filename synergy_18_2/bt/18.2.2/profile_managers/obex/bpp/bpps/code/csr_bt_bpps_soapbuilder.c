/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_pmem.h"
#include "csr_bt_util.h"
#include "csr_bt_bpps_handler.h"
#include "csr_bt_bpps_soapbuilder.h"

void soapAddTagValue(char * soapString, char * tagId, char * tagValue)
{
    if (tagValue)
    {
        CsrStrCat(soapString, "<");
        CsrStrCat(soapString, tagId);
        CsrStrCat(soapString, ">");
        CsrStrCat(soapString, tagValue);
        CsrStrCat(soapString, "</");
        CsrStrCat(soapString, tagId);
        CsrStrCat(soapString, ">\n");
    }
}

void soapAddMultiTagValues(char * soapString, char * tagId, char * tagValues, CsrUint8 numOfTags)
{
    if (tagValues)
    {
        CsrUint16 index = 0;
        char * ptr;

        CsrStrCat(soapString, "<");
        CsrStrCat(soapString, tagId);
        CsrStrCat(soapString, "sSupported>\n");


        for(index=0, ptr=tagValues; index< numOfTags; index++)
        {
            soapAddTagValue(soapString, tagId, (char *)ptr);
            ptr += CsrStrLen((char*)ptr) + 1;
        }
        CsrStrCat(soapString, "</");
        CsrStrCat(soapString, tagId);
        CsrStrCat(soapString, "sSupported>\n");
    }
        }

void soapAddMultiTagValuesNoS(char * soapString, char * tagId, char * tagValues, CsrUint8 numOfTags)
{
    if (tagValues)
    {
        CsrUint16 index = 0;
        char * ptr;

        CsrStrCat(soapString, "<");
        CsrStrCat(soapString, tagId);
        CsrStrCat(soapString, "Supported>\n");


        for(index=0, ptr=tagValues; index< numOfTags; index++)
        {
            soapAddTagValue(soapString, tagId, (char *)ptr);
            ptr += CsrStrLen((char*)ptr) + 1;
        }
        CsrStrCat(soapString, "</");
        CsrStrCat(soapString, tagId);
        CsrStrCat(soapString, "Supported>\n");
    }
}

void soapAddMediaLoaded(char * soapString, char * tagValues, CsrUint8 numOfTags)
{
    CsrUint8 index = 0;
    char * ptr = NULL;

    CsrStrCat(soapString, "<");
    CsrStrCat(soapString, "MediaLoaded");
    CsrStrCat(soapString, ">\n");

    for(index = 0, ptr=tagValues; index < numOfTags; index++)
    {
        CsrStrCat(soapString, "<");
        CsrStrCat(soapString, "LoadedMediumDetails");
        CsrStrCat(soapString, ">\n");

        soapAddTagValue(soapString, "LoadedMediumSize", ptr);
        ptr += CsrStrLen((char*)ptr) + 1;
        soapAddTagValue(soapString, "LoadedMediumType", ptr);
        ptr += CsrStrLen((char*)ptr) + 1;

        CsrStrCat(soapString, "</");
        CsrStrCat(soapString, "LoadedMediumDetails");
        CsrStrCat(soapString, ">\n");
    }
    CsrStrCat(soapString, "</");
    CsrStrCat(soapString, "MediaLoaded");
    CsrStrCat(soapString, ">\n");
    }

