/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_profiles.h"
#include "csr_pmem.h"
#include "csr_bt_util.h"
#include "csr_bt_sd_prim.h"
#include "csr_bt_sd_lib.h"

#include "csr_bt_sdc_support.h"
#include "csr_bt_cmn_sdp_lib.h"


/* **************************************************** */
/* **************************************************** */
/* CREATE SDP DATA ELEMENTS */
/* **************************************************** */
/* **************************************************** */

static CsrBool cmnSdpCombineMemory(CsrUint8   *data1_p,
                                  CsrUint16   data1Len,
                                  CsrUint8   *data2_p,
                                  CsrUint16   data2Len,
                                  CsrUint8  **resultingData_pp,
                                  CsrUint16  *resultingDataLen_p)
{
    CsrBool  retBool = FALSE;

    if ((data1Len + data2Len) > 0)
    {
        *resultingDataLen_p = data1Len + data2Len;
        *resultingData_pp  = CsrPmemAlloc(*resultingDataLen_p);

        if (0 < data1Len)
        {
            CsrMemCpy(*resultingData_pp, data1_p, data1Len);
        }

        if (0 < data2Len)
        {
            CsrMemCpy(*resultingData_pp + data1Len, data2_p, data2Len);
        }
        retBool = TRUE;
    }
    else
    {
        *resultingDataLen_p = 0;
        *resultingData_pp   = NULL;
    }
    return retBool;
}

CsrBool CsrBtUtilSdpCombineMemoryAndFree(CsrUint8   **data1_pp,
                               CsrUint16    data1Len,
                               CsrUint8   **data2_pp,
                               CsrUint16    data2Len,
                               CsrUint8   **resultingData_pp,
                               CsrUint16   *resultingDataLen_p)
{
    CsrBool  retBool = FALSE;

    if (TRUE == cmnSdpCombineMemory(*data1_pp,
                                 data1Len,
                                 *data2_pp,
                                 data2Len,
                                 resultingData_pp,
                                 resultingDataLen_p))
    {
        CsrPmemFree(*data1_pp);
        CsrPmemFree(*data2_pp);

        *data1_pp = NULL;
        *data2_pp = NULL;
        retBool   = TRUE;
    }
    return retBool;
}

/* **************************************************** */
/* **************************************************** */
/* **************************************************** */

static void  cmnSdpCreate16bitsDataElement(CsrUint8  **data_pp,
                                  CsrUint16  *dataLen_p,
                                  CsrUint16   value,
                                  CsrUint8    dataElementType)
{
    *dataLen_p   = 3;
    *data_pp     = CsrPmemAlloc(*dataLen_p);
    *data_pp[0]                 = dataElementType + CSR_BT_DATA_ELEMENT_SIZE_2_BYTES;
    CSR_COPY_UINT16_TO_BIG_ENDIAN(value, ((*data_pp) + 1));
}


void CsrBtUtilSdpCreateUint16DataElement(CsrUint8  **data_pp,
                                  CsrUint16  *dataLen_p,
                                  CsrUint16   value)
{
    cmnSdpCreate16bitsDataElement(data_pp,
                                  dataLen_p,
                                  value,
                                  CSR_BT_DATA_ELEMENT_UNSIGNED_INTEGER_TYPE);
}

void CsrBtUtilSdpCreateUuid16DataElement(CsrUint8  **data_pp,
                                  CsrUint16  *dataLen_p,
                                  CsrUint16   value)
{
    cmnSdpCreate16bitsDataElement(data_pp,
                                  dataLen_p,
                                  value,
                                  CSR_BT_DATA_ELEMENT_UUID_TYPE);
}

void CsrBtUtilSdpCreateBooleanDataElement(CsrUint8  **data_pp,
                                   CsrUint16  *dataLen_p,
                                   CsrBool     value)
{
    CsrUint8 boolVar = 0;

    if (FALSE != value)
    {
        /* set boolVar = 0 if value=FALSE, otherwise set boolVar=1 */
        boolVar = 1;
    }
    *dataLen_p   = 2;
    *data_pp     = CsrPmemAlloc(*dataLen_p);
    *data_pp[0]               = CSR_BT_DATA_ELEMENT_BOOLEAN_TYPE + CSR_BT_DATA_ELEMENT_SIZE_1_BYTE;
    *(CsrBool *)(*data_pp + 1) = boolVar;
}

static void cmnSdpCreateStringDataElement(CsrUint8  **dataElement_pp,
                                  CsrUint16  *dataElementLen_p,
                                  CsrUint8   *string_p,
                                  CsrUint16   stringLen,
                                  CsrUint8    typeDescriptor)
{
    *dataElementLen_p = stringLen;

    if (0xff >= stringLen)
    {
        *dataElementLen_p += 2;
        typeDescriptor    += CSR_BT_DATA_ELEMENT_SIZE_NEXT_1_BYTE;
    }
    else
    {
        *dataElementLen_p += 3;
        typeDescriptor    += CSR_BT_DATA_ELEMENT_SIZE_NEXT_2_BYTES;
    }

    *dataElement_pp = CsrPmemAlloc(*dataElementLen_p);
    *dataElement_pp[0] = typeDescriptor;

    if (0xff >= stringLen)
    {
        *(CsrUint8 *)(*dataElement_pp + 1) = (CsrUint8)stringLen;
        CsrMemCpy(*dataElement_pp + 2, string_p, stringLen);
    }
    else
    {
        CSR_COPY_UINT16_TO_BIG_ENDIAN((CsrUint16)stringLen, ((*dataElement_pp) + 1));
        CsrMemCpy(*dataElement_pp + 3, string_p, stringLen);
    }

}

static void cmnSdpCreateDataElementSequenceHeader(CsrUint8  **data_pp,
                                          CsrUint16  *dataLen_p,
                                          CsrUint32   dataElementSize)
{
    CsrUint8 dataElementSizeDescriptor;

    if (0xff > dataElementSize)
    {
        *dataLen_p = 2;
        dataElementSizeDescriptor = CSR_BT_DATA_ELEMENT_SIZE_NEXT_1_BYTE;
    }
    else if (0xffff > dataElementSize)
    {
        *dataLen_p = 3;
        dataElementSizeDescriptor = CSR_BT_DATA_ELEMENT_SIZE_NEXT_2_BYTES;
    }
    else
    {
        *dataLen_p = 5;
        dataElementSizeDescriptor = CSR_BT_DATA_ELEMENT_SIZE_NEXT_4_BYTES;
    }

    *data_pp    = CsrPmemAlloc(*dataLen_p);
    *data_pp[0] = CSR_BT_DATA_ELEMENT_SEQUENCE_TYPE + dataElementSizeDescriptor;

    if (0xff > dataElementSize)
    {
        *(CsrUint8 *)(*data_pp + 1) = (CsrUint8)dataElementSize;
    }
    else if (0xffff > dataElementSize)
    {
        CSR_COPY_UINT16_TO_BIG_ENDIAN((CsrUint16)dataElementSize, ((*data_pp) + 1));
    }
    else
    {
        CSR_COPY_UINT32_TO_BIG_ENDIAN((CsrUint32)dataElementSize, ((*data_pp) + 1));
    }
}

CsrBool CsrBtUtilSdpCreateServiceClassIdList(CsrUint8  **data_pp,
                                   CsrUint16  *dataLen_p,
                                   CsrUint16   nofUuid,
                                   CsrUint16  *uuid_p)
{
    CsrBool  retBool = FALSE;

    if (nofUuid > 0 && uuid_p)
    {

        CsrUint8  *tempData_p = NULL;

        CsrUint16 tempLen, x, byteIndex=0;

        *dataLen_p = (SDP_DATA_ELEMENT_BYTELENGTH_UUID16 +
                      SDP_DATA_ELEMENT_BYTELENGTH_SEQUENCE_1_BYTE +
                      (nofUuid * SDP_DATA_ELEMENT_BYTELENGTH_UUID16));
        *data_pp = CsrPmemAlloc(*dataLen_p);

        CsrBtUtilSdpCreateUint16DataElement(&tempData_p, &tempLen, CSR_BT_SERVICE_CLASS_ID_LIST);
        CsrMemCpy(*data_pp+byteIndex, tempData_p, tempLen);
        CsrPmemFree(tempData_p);
        tempData_p = NULL;
        byteIndex += tempLen;
        cmnSdpCreateDataElementSequenceHeader(&tempData_p, &tempLen, (nofUuid * SDP_DATA_ELEMENT_BYTELENGTH_UUID16));
        CsrMemCpy(*data_pp+byteIndex, tempData_p, tempLen);
        CsrPmemFree(tempData_p);
        tempData_p = NULL;
        byteIndex += tempLen;

        for (x=0; x<nofUuid; x++)
        {
            retBool = FALSE;
            CsrBtUtilSdpCreateUuid16DataElement(&tempData_p, &tempLen, uuid_p[x]);
            retBool = TRUE;
            CsrMemCpy(*data_pp+byteIndex, tempData_p, tempLen);
            CsrPmemFree(tempData_p);
            tempData_p = NULL;
            byteIndex += tempLen;
        }
    }
    return retBool;
}

CsrBool CsrBtUtilSdpCreateUint16String(CsrUint8  **dataElement_pp,
                             CsrUint16  *dataElementLen_p,
                             CsrUint16   uuid,
                             CsrUint8   *string_p,
                             CsrUint16   stringLen,
                             CsrUint8    typeDescriptor)
{
    CsrBool      retBool = FALSE;

    CsrUint8    *stringDataElement_p, *uuid_p;
    CsrUint16    stringDataElementLen, uuidLen;

    CsrBtUtilSdpCreateUint16DataElement(&uuid_p, &uuidLen, uuid);
    cmnSdpCreateStringDataElement(&stringDataElement_p,
                                  &stringDataElementLen,
                                  string_p,
                                  stringLen,
                                  typeDescriptor);

    if (TRUE == CsrBtUtilSdpCombineMemoryAndFree(&uuid_p,
                                                 uuidLen,
                                                 &stringDataElement_p,
                                                 stringDataElementLen,
                                                 dataElement_pp,
                                                 dataElementLen_p))
    {
        retBool = TRUE;
    }
    else
    {
        CsrPmemFree(uuid_p);
        CsrPmemFree(stringDataElement_p);
    }
    return retBool;
}

CsrBool CsrBtUtilSdpCreateUint16Uint16(CsrUint16    value1,
                                CsrUint16    value2,
                                CsrUint8   **data_pp,
                                CsrUint16   *dataLen_p)
{
    CsrBool      retBool = FALSE;

    CsrUint8    *value1_p, *value2_p;
    CsrUint16    value1Len, value2Len;

    CsrBtUtilSdpCreateUint16DataElement(&value1_p, &value1Len, value1);
    CsrBtUtilSdpCreateUint16DataElement(&value2_p, &value2Len, value2);

    if (TRUE == CsrBtUtilSdpCombineMemoryAndFree(&value1_p,
                                           value1Len,
                                           &value2_p,
                                           value2Len,
                                           data_pp,
                                           dataLen_p))
    {
        retBool = TRUE;
    }
    else
    {
        CsrPmemFree(value1_p);
        CsrPmemFree(value2_p);
    }
    return retBool;
}

CsrBool CsrBtUtilSdpCreateUint16Boolean(CsrUint16    value1,
                                 CsrBool      value2,
                                 CsrUint8   **data_pp,
                                 CsrUint16   *dataLen_p)
{
    CsrBool      retBool = FALSE;

    CsrUint8    *value1_p, *value2_p;
    CsrUint16    value1Len, value2Len;

    CsrBtUtilSdpCreateUint16DataElement(&value1_p, &value1Len, value1);
    CsrBtUtilSdpCreateBooleanDataElement(&value2_p, &value2Len, value2);

    if (TRUE == CsrBtUtilSdpCombineMemoryAndFree(&value1_p,
                                           value1Len,
                                           &value2_p,
                                           value2Len,
                                           data_pp,
                                           dataLen_p))
    {
        retBool = TRUE;
    }
    else
    {
        CsrPmemFree(value1_p);
        CsrPmemFree(value2_p);
    }
    return retBool;
}

/* **************************************************** */
/* **************************************************** */
/* EXTRACT SDP DATA ELEMENTS */
/* **************************************************** */
/* **************************************************** */

CsrBool CsrBtUtilSdpExtractString(CsrUint8   *attData_p,
                           CsrUint16   attDataLen,
                           CsrUint8  **string_pp,
                           CsrUint16  *stringLen_p,
                           CsrUint16  *consumedBytes)
{
    CsrBool  retBool = FALSE;

    if ((0 < attDataLen) && (attData_p))
    {
        if ((*attData_p == (CSR_BT_DATA_ELEMENT_TEXT_STRING_TYPE + SDP_DATA_ELEMENT_SIZE_NEXT_8_BITS)) ||
            (*attData_p == (CSR_BT_DATA_ELEMENT_URL              + SDP_DATA_ELEMENT_SIZE_NEXT_8_BITS)))
        {
            *stringLen_p   = *(CsrUint8 *)(attData_p + 1);
            *string_pp     = attData_p + 2;
            *consumedBytes = *stringLen_p + 2;
            retBool        = TRUE;
        }
        else if ((*attData_p == (CSR_BT_DATA_ELEMENT_TEXT_STRING_TYPE + SDP_DATA_ELEMENT_SIZE_NEXT_16_BITS))  ||
                 (*attData_p == (CSR_BT_DATA_ELEMENT_URL              + SDP_DATA_ELEMENT_SIZE_NEXT_16_BITS)))
        {
            *stringLen_p   = CSR_GET_UINT16_FROM_BIG_ENDIAN(attData_p + 1);
            *string_pp     = attData_p + 3;
            *consumedBytes = *stringLen_p + 3;
            retBool        = TRUE;
        }
    }
    else
    {
        *string_pp = NULL;
        *stringLen_p = 0;
        *consumedBytes = 0;
    }
    return retBool;
}

CsrBool CsrBtUtilSdpExtractDataElementSequence(CsrUint8   *attData_p,
                                        CsrUint16   attDataLen,
                                        CsrUint32  *sequenceSize,
                                        CsrUint16  *consumedBytes)
{
    CsrBool  retBool = FALSE;

    if ((0 < attDataLen) && (attData_p))
    {
        if (*attData_p == (CSR_BT_DATA_ELEMENT_SEQUENCE_TYPE + SDP_DATA_ELEMENT_SIZE_NEXT_8_BITS))
        {
            *sequenceSize  = *(CsrUint8 *)(attData_p + 1);
            *consumedBytes = 2;
            retBool        = TRUE;
        }
        else if (*attData_p == (CSR_BT_DATA_ELEMENT_SEQUENCE_TYPE + SDP_DATA_ELEMENT_SIZE_NEXT_16_BITS))
        {
            *sequenceSize  = CSR_GET_UINT16_FROM_BIG_ENDIAN(attData_p + 1);
            *consumedBytes = 3;
            retBool        = TRUE;
        }
        else if (*attData_p == (CSR_BT_DATA_ELEMENT_SEQUENCE_TYPE + SDP_DATA_ELEMENT_SIZE_NEXT_32_BITS))
        {
            *sequenceSize  = CSR_GET_UINT32_FROM_BIG_ENDIAN(attData_p + 1);
            *consumedBytes = 5;
            retBool        = TRUE;
        }
    }
    else
    {
        *sequenceSize = 0;
        *consumedBytes = 0;
    }
    return retBool;
}

CsrBool CsrBtUtilSdpUintData(CsrUint8   *attData_p,
                             CsrUint16   attDataLen,
                             CsrUint32  *uintDataSize,
                             CsrUint16  *consumedBytes)
{
    CsrBool  retBool = FALSE;
    *uintDataSize   = 0;
    *consumedBytes  = 0;

    if ((0 < attDataLen) && (attData_p))
    {
        *consumedBytes  = 1;
        retBool         = TRUE;

        if (*attData_p == (CSR_BT_DATA_ELEMENT_UNSIGNED_INTEGER_TYPE + CSR_BT_DATA_ELEMENT_SIZE_1_BYTE))
        {
            *uintDataSize   = 1;
        }
        else if (*attData_p == (CSR_BT_DATA_ELEMENT_UNSIGNED_INTEGER_TYPE + CSR_BT_DATA_ELEMENT_SIZE_2_BYTES))
        {
            *uintDataSize   = 2;
        }
        else if (*attData_p == (CSR_BT_DATA_ELEMENT_UNSIGNED_INTEGER_TYPE + CSR_BT_DATA_ELEMENT_SIZE_4_BYTES))
        {
            *uintDataSize   = 4;
        }
        else if (*attData_p == (CSR_BT_DATA_ELEMENT_UNSIGNED_INTEGER_TYPE + CSR_BT_DATA_ELEMENT_SIZE_8_BYTES))
        {
            *uintDataSize   = 8;
        }
        else if (*attData_p == (CSR_BT_DATA_ELEMENT_UNSIGNED_INTEGER_TYPE + CSR_BT_DATA_ELEMENT_SIZE_16_BYTES))
        {
            *uintDataSize   = 16;
        }
        else
        {
            retBool         = FALSE;
            *consumedBytes  = 0;
        }
    }
    return retBool;
}

CsrBool CsrBtUtilSdpUuidData(CsrUint8   *attData_p,
                             CsrUint16  attDataLen,
                             CsrUint32  *uintDataSize,
                             CsrUint16  *consumedBytes)
{
    CsrBool  retBool = FALSE;
    *uintDataSize   = 0;
    *consumedBytes  = 0;

    if ((0 < attDataLen) && (attData_p))
    {
        *consumedBytes  = 1;
        retBool         = TRUE;

        if (*attData_p == (CSR_BT_DATA_ELEMENT_UUID_TYPE + CSR_BT_DATA_ELEMENT_SIZE_2_BYTES))
        {
            *uintDataSize   = 2;
        }
        else if (*attData_p == (CSR_BT_DATA_ELEMENT_UUID_TYPE + CSR_BT_DATA_ELEMENT_SIZE_4_BYTES))
        {
            *uintDataSize   = 4;
        }
        else if (*attData_p == (CSR_BT_DATA_ELEMENT_UUID_TYPE + CSR_BT_DATA_ELEMENT_SIZE_16_BYTES))
        {
            *uintDataSize   = 16;
        }
        else
        {
            retBool         = FALSE;
            *consumedBytes  = 0;
        }
    }
    return retBool;
}

CsrBool CsrBtUtilSdpExtractUint(CsrUint8   *attData_p,
                         CsrUint16   attDataLen,
                         CsrUint32  *uuid_p,
                         CsrUint16  *consumedBytes,
                         CsrBool     allowDataElementSequence)
{
    CsrBool    retBool = FALSE, foundDataElementSequence = FALSE;
    CsrUint16  totalConsumedBytes = 0, restLen, consumedSequenceBytes;
    CsrUint32  sequenceSize = 0;

    restLen = attDataLen;

    if (TRUE == allowDataElementSequence)
    {
        CsrBool flag = TRUE;

        while (flag)
        {
            flag = CsrBtUtilSdpExtractDataElementSequence((attData_p + totalConsumedBytes),
                                                        restLen,
                                                        &sequenceSize,
                                                        &consumedSequenceBytes);
            if (flag)
            {
                foundDataElementSequence = TRUE;
                totalConsumedBytes      += consumedSequenceBytes;
                if (totalConsumedBytes < attDataLen)
                {
                    restLen = attDataLen - totalConsumedBytes;
                }
                else
                {
                    flag = FALSE;
                }
            }
        }
    }

    if ((TRUE == foundDataElementSequence) && (0 == sequenceSize) && (TRUE ==  allowDataElementSequence))
    {
        *consumedBytes = 0;
    }
    else
    {
        if(attDataLen > totalConsumedBytes)
        {
            if ((*(attData_p + totalConsumedBytes) == (CSR_BT_DATA_ELEMENT_UNSIGNED_INTEGER_TYPE + CSR_BT_DATA_ELEMENT_SIZE_1_BYTE)) ||
                (*(attData_p + totalConsumedBytes) == (CSR_BT_DATA_ELEMENT_UUID_TYPE             + CSR_BT_DATA_ELEMENT_SIZE_1_BYTE)) ||
                (*(attData_p + totalConsumedBytes) == (CSR_BT_DATA_ELEMENT_BOOLEAN_TYPE          + CSR_BT_DATA_ELEMENT_SIZE_1_BYTE)))
            {
                *uuid_p        = *(CsrUint8 *)(attData_p + totalConsumedBytes + 1);
                *consumedBytes = 2;
                retBool        = TRUE;
            }
            else if ((*(attData_p + totalConsumedBytes) == (CSR_BT_DATA_ELEMENT_UNSIGNED_INTEGER_TYPE + CSR_BT_DATA_ELEMENT_SIZE_2_BYTES)) ||
                     (*(attData_p + totalConsumedBytes) == (CSR_BT_DATA_ELEMENT_UUID_TYPE             + CSR_BT_DATA_ELEMENT_SIZE_2_BYTES)))
            {
                *uuid_p        = CSR_GET_UINT16_FROM_BIG_ENDIAN(attData_p + totalConsumedBytes + 1);
                *consumedBytes = 3;
                retBool        = TRUE;
            }
            else if ((*(attData_p + totalConsumedBytes) == (CSR_BT_DATA_ELEMENT_UNSIGNED_INTEGER_TYPE + CSR_BT_DATA_ELEMENT_SIZE_4_BYTES)) ||
                     (*(attData_p + totalConsumedBytes) == (CSR_BT_DATA_ELEMENT_UUID_TYPE             + CSR_BT_DATA_ELEMENT_SIZE_4_BYTES)))
            {
                *uuid_p        = CSR_GET_UINT32_FROM_BIG_ENDIAN(attData_p + totalConsumedBytes + 1);
                *consumedBytes = 5;
                retBool        = TRUE;
            }
            else
            {
                *consumedBytes = 0;
            }
        }
        else
        {
            *consumedBytes = 0;
        }
    }
    *consumedBytes += totalConsumedBytes;

    return retBool;
}

CsrBool CsrBtUtilSdpExtractServiceClassIdList(CsrUint8   *attData_p,
                                       CsrUint16   attDataLen,
                                       CsrUint16  *nofServiceClassIds_p,
                                       CsrUint32 **serviceClassIdList_pp,
                                       CsrUint16  *consumedBytes)
{
    CsrBool  retBool = FALSE;

    CsrUint16  consumedBytesTemp, totalConsumedBytes = 0, tempLen, restLen, uuidListLen;
    CsrUint32  uuid = 0, sequenceSize = 0, *tempData_p;

    if (TRUE == CsrBtUtilSdpExtractUint((attData_p + totalConsumedBytes),
                               attDataLen,
                               &uuid,
                               &consumedBytesTemp,
                               FALSE))
    {
        /* check that the first entry in the service record is indeed a service class list */
        if (uuid == CSR_BT_SERVICE_CLASS_ID_LIST)
        {
            totalConsumedBytes += consumedBytesTemp;
            restLen = attDataLen - totalConsumedBytes;
            while ((attDataLen > totalConsumedBytes) &&
                   (TRUE == CsrBtUtilSdpExtractDataElementSequence((attData_p + totalConsumedBytes),
                                                             restLen,
                                                             &sequenceSize,
                                                             &consumedBytesTemp)))
            {
                CsrUint16 totalServiceClassListConsumedBytes = 0;
                totalConsumedBytes    += consumedBytesTemp;
                restLen                = attDataLen - totalConsumedBytes;
                *nofServiceClassIds_p  = 0;
                *serviceClassIdList_pp = NULL;
                if (0 != sequenceSize)
                {
                    while ((totalConsumedBytes < attDataLen) &&
                           (totalServiceClassListConsumedBytes < sequenceSize) &&
                           (TRUE == CsrBtUtilSdpExtractUint((attData_p + totalConsumedBytes),
                                                      restLen,
                                                      &uuid,
                                                      &consumedBytesTemp,
                                                      FALSE)))
                    {
                        uuidListLen = (sizeof(CsrUint32) * (*nofServiceClassIds_p));
                        cmnSdpCombineMemory((CsrUint8 *)(*serviceClassIdList_pp),
                                            uuidListLen,
                                            (CsrUint8 *)&uuid,
                                            sizeof(CsrUint32),
                                            (CsrUint8 **)&tempData_p,
                                            &tempLen);

                        totalConsumedBytes    += consumedBytesTemp;
                        totalServiceClassListConsumedBytes += consumedBytesTemp;
                        *nofServiceClassIds_p += 1;
                        CsrPmemFree(*serviceClassIdList_pp);
                        *serviceClassIdList_pp = tempData_p;
                        uuid                   = 0;
                        *consumedBytes         = totalConsumedBytes;
                        retBool                = TRUE;

                        /* Prep for next loop */
                        tempData_p = NULL;
                        tempLen = 0;
                    }
                }
                else
                {
                    *consumedBytes = totalConsumedBytes;
                }
            }
        }
    }
    return retBool;
}
