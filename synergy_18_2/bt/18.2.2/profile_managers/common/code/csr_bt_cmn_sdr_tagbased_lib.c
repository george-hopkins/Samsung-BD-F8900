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
#include "csr_bt_cm_prim.h"
#include "csr_bt_sd_lib.h"

#include "csr_bt_sdc_support.h"
#include "csr_bt_cmn_sdr_tagbased_lib.h"
#include "csr_bt_cmn_sdp_lib.h"

static CsrBool cmnSdrGetServiceUuidAndResult(CmnCsrBtLinkedListStruct *bll_p,
                                 CsrUintFast16              serviceHandleIndex,
                                 CsrUint16              serviceUuidType,
                                 CsrBtUuid32             *serviceUuid32_p,
                                 CsrBtUuid128           **serviceUuid128_pp,
                                 CsrUint16             *csrBtResult_p,
                                 CsrUint16             *csrBtTask_p,
                                 CsrUint16             *csrBtTaskResult_p)
{
    CsrBool   retBool = FALSE;

    if (bll_p && (
#ifdef CSR_BT_INSTALL_128_BIT_SERVICE_SEARCH
        serviceUuidType == SDP_DATA_ELEMENT_SIZE_128_BITS || 
#endif
        serviceUuidType == SDP_DATA_ELEMENT_SIZE_32_BITS))
    {
        if (serviceHandleIndex < CsrBtUtilBllGetNofEntriesEx(bll_p))
        {
            CmnCsrBtLinkedListStruct *bllTemp_p = NULL;
            bllTemp_p = CsrBtUtilBllGetEntryPtrFromIndexEx(bll_p, serviceHandleIndex);

            if (bllTemp_p && bllTemp_p->data)
            {
                CsrUint8 *p_data = bllTemp_p->data;

#ifdef CSR_BT_INSTALL_128_BIT_SERVICE_SEARCH
                if (SDP_DATA_ELEMENT_SIZE_128_BITS == serviceUuidType)
                {
                    *serviceUuid128_pp   = (CsrBtUuid128 *)(p_data + SDR_ENTRY_INDEX_SERVICE_UUID);
                }
                else
#endif                    
                {
                    CsrMemCpy(serviceUuid32_p, (p_data + SDR_ENTRY_INDEX_SERVICE_UUID), SDR_ENTRY_SIZE_SERVICE_UUID32);
                }
                CsrMemCpy(csrBtResult_p    , (p_data + SDR_ENTRY_INDEX_SERVICE_CSR_BT_RESULT),      SDR_ENTRY_SIZE_SERVICE_UINT16);
                CsrMemCpy(csrBtTask_p      , (p_data + SDR_ENTRY_INDEX_SERVICE_CSR_BT_TASK),        SDR_ENTRY_SIZE_SERVICE_UINT16);
                CsrMemCpy(csrBtTaskResult_p, (p_data + SDR_ENTRY_INDEX_SERVICE_CSR_BT_TASK_RESULT), SDR_ENTRY_SIZE_SERVICE_UINT16);
                retBool              = TRUE;
            }
        }
    }
    return retBool;
}

static CsrBool cmnSdrGetEmptySdrSize(CsrBtUuid32  serviceUuidType,
                                    CsrUint16 *size_p)
{
    CsrBool   retBool         = FALSE;

    /* The minimum size needed to make the SDR without attribute data */
    if (SDP_DATA_ELEMENT_SIZE_32_BITS == serviceUuidType)
    {
        *size_p = (SDR_ENTRY_INDEX_SERVICE_UUID + SDR_ENTRY_SIZE_SERVICE_UUID32);
        retBool = TRUE;
    }
#ifdef CSR_BT_INSTALL_128_BIT_SERVICE_SEARCH
    else if (SDP_DATA_ELEMENT_SIZE_128_BITS == serviceUuidType)
    {
        *size_p = (SDR_ENTRY_INDEX_SERVICE_UUID + SDR_ENTRY_SIZE_SERVICE_UUID128);
        retBool = TRUE;
    }
#endif
    else
    {
        *size_p = 0;
    }
    return retBool;
}

static CsrBool cmnSdrGetIndexForServiceUuid(CmnCsrBtLinkedListStruct *bll_p,
                                CsrUint16              serviceUuidType,
                                CsrBtUuid32              serviceUuid32,
                                CsrBtUuid128            *serviceUuid128_p,
                                CsrUint16             *serviceIndex_p)
{
    CsrBool      retBool = FALSE;
    CsrUintFast16    x, nofEntries;
    CsrBtUuid32    serviceUuid32Temp;
#ifdef CSR_BT_INSTALL_128_BIT_SERVICE_SEARCH
    CsrBtUuid128  *serviceUuid128Temp_p = NULL;
#endif
    CmnCsrBtLinkedListStruct *bllTemp_p = bll_p;

    if (bll_p)
    {
        nofEntries = CsrBtUtilBllGetNofEntriesEx(bll_p);

        for (x=0; x<nofEntries; x++)
        {
            bllTemp_p = bllTemp_p->nextEntry;
            if (SDP_DATA_ELEMENT_SIZE_32_BITS == serviceUuidType)
            {
                if (TRUE == CsrBtUtilSdrGetServiceUuid32(bll_p, x, &serviceUuid32Temp))
                {
                    if (serviceUuid32 == serviceUuid32Temp)
                    {
                        *serviceIndex_p = (CsrUint16)x;
                        retBool = TRUE;
                        /* Now break out of the for-loop! */
                        x               = nofEntries;
                    }
                }
            }
#ifdef CSR_BT_INSTALL_128_BIT_SERVICE_SEARCH
            else if (SDP_DATA_ELEMENT_SIZE_128_BITS == serviceUuidType)
            {
                if (TRUE == CsrBtUtilSdrGetServiceUuid128(bll_p, x, &serviceUuid128Temp_p))
                {
                    if (0 == CsrMemCmp((CsrUint8 *)serviceUuid128_p, (CsrUint8 *)serviceUuid128Temp_p, SDP_DATA_ELEMENT_SIZE_128_BITS))
                    {
                        *serviceIndex_p = (CsrUint16)x;
                        retBool = TRUE;
                        /* Now break out of the for-loop! */
                        x               = nofEntries;
                    }
                }
            }
#endif
        }
    }
    return retBool;
}

static CsrUint8 *CsrBtUtilSdrGetInnerAttrData(CmnCsrBtLinkedListStruct *bll_p,
                                              CsrUintFast16                serviceHandleIndex,
                                              CsrUint16                attributeUuid,
                                              CsrUint16                *dataLen)
{
    CsrUintFast16   nofAttributes=0;
    CsrUint8    *data = NULL;

    if (TRUE == CsrBtUtilSdrGetNofAttributes(bll_p, serviceHandleIndex, &nofAttributes))
    {
        CsrUintFast16 x;
        CsrUint16 tempVar, nofBytesToAttribute;
        CsrUint8  *att_p;

        for (x = 0; x < nofAttributes; x++)
        {
            att_p = CsrBtUtilSdrGetAttributePointer(bll_p, serviceHandleIndex, x, &nofBytesToAttribute);
            /* Check if the UUID in the 'outer' attribute struct is correct */
            CsrMemCpy(&tempVar, att_p + SDR_ENTRY_INDEX_ATTRIBUTE_UUID, SDR_ENTRY_SIZE_SERVICE_UINT16);

            if (tempVar == attributeUuid)
            {
                CsrUint16 emptyAttSize, attDataLen, consumedBytes;
                CsrUint32 returnValue;

                CsrBtUtilSdrGetEmptyAttributeSize(&emptyAttSize);
                CsrMemCpy(&tempVar, att_p, SDR_ENTRY_SIZE_SERVICE_UINT16);
                attDataLen = tempVar - emptyAttSize + SDR_ENTRY_SIZE_TAG_LENGTH;

                /* First extract the attribute uuid from the attribute SDP data */
                if (TRUE == CsrBtUtilSdpExtractUint(att_p + SDR_ENTRY_INDEX_ATTRIBUTE_DATA,
                                           attDataLen,
                                           &returnValue,
                                           &consumedBytes,
                                           FALSE))
                {
                    /* Check if the UUID in the 'inner' attribute sdp data struct is correct */
                    if (returnValue == attributeUuid)
                    {
                        attDataLen = attDataLen - consumedBytes;
                        *dataLen   = attDataLen;
                        data       = att_p + SDR_ENTRY_INDEX_ATTRIBUTE_DATA + consumedBytes;
                        return (data);
                    }
                }
            }
        }
    }
    return (data);
}

/* **************************************************** */
/* **************************************************** */
/* GET SDR FUNCTIONS */
/* **************************************************** */
/* **************************************************** */
CsrBool CsrBtUtilSdrGetServiceUuidType(CmnCsrBtLinkedListStruct *bll_p,
                            CsrUintFast16              serviceHandleIndex,
                            CsrUint16             *serviceUuidType_p)
{
    CsrBool   retBool = FALSE;
    CmnCsrBtLinkedListStruct *bllTemp_p = bll_p;

    if (bll_p)
    {
        if (serviceHandleIndex < CsrBtUtilBllGetNofEntriesEx(bll_p))
        {
            bllTemp_p = CsrBtUtilBllGetEntryPtrFromIndexEx(bll_p, serviceHandleIndex);

            if (bllTemp_p && bllTemp_p->data)
            {
                CsrUint8 *p_data = bllTemp_p->data;
                retBool = TRUE;
                CsrMemCpy(serviceUuidType_p, (p_data + SDR_ENTRY_INDEX_SERVICE_UUID_TYPE), SDR_ENTRY_SIZE_SERVICE_UINT16);
            }
        }
    }
    return retBool;
}

CsrBool CsrBtUtilSdrGetServiceUuid32 (CmnCsrBtLinkedListStruct *bll_p,
                           CsrUintFast16              serviceHandleIndex,
                           CsrBtUuid32             *serviceUuid_p)
{
    CsrUint16 temp1, temp2, temp3;

    return CsrBtUtilSdrGetServiceUuid32AndResult (bll_p,
                                        serviceHandleIndex,
                                        serviceUuid_p,
                                        &temp1, &temp2, &temp3);
}

CsrBool CsrBtUtilSdrGetServiceUuid32AndResult (CmnCsrBtLinkedListStruct *bll_p,
                                    CsrUintFast16              serviceHandleIndex,
                                    CsrBtUuid32             *serviceUuid_p,
                                    CsrUint16             *csrBtResult_p,
                                    CsrUint16             *csrBtTask_p,
                                    CsrUint16             *csrBtTaskResult_p)
{
    CsrBtUuid128  temp;
    CsrBtUuid128 *temp_p = &temp;

    return cmnSdrGetServiceUuidAndResult(bll_p,
                                     serviceHandleIndex,
                                     SDP_DATA_ELEMENT_SIZE_32_BITS,
                                     serviceUuid_p,
                                     &temp_p,
                                     csrBtResult_p,
                                     csrBtTask_p,
                                     csrBtTaskResult_p);
}

#ifdef CSR_BT_INSTALL_128_BIT_SERVICE_SEARCH
CsrBool CsrBtUtilSdrGetServiceUuid128(CmnCsrBtLinkedListStruct *bll_p,
                           CsrUintFast16              serviceHandleIndex,
                           CsrBtUuid128           **serviceUuid_pp)
{
    CsrUint16 temp1, temp2, temp3;

    return CsrBtUtilSdrGetServiceUuid128AndResult(bll_p,
                                        serviceHandleIndex,
                                        serviceUuid_pp,
                                        &temp1, &temp2, &temp3);
}

CsrBool CsrBtUtilSdrGetServiceUuid128AndResult(CmnCsrBtLinkedListStruct *bll_p,
                                    CsrUintFast16              serviceHandleIndex,
                                    CsrBtUuid128           **serviceUuid_pp,
                                    CsrUint16             *csrBtResult_p,
                                    CsrUint16             *csrBtTask_p,
                                    CsrUint16             *csrBtTaskResult_p)
{
    CsrBtUuid32 temp;

    return cmnSdrGetServiceUuidAndResult(bll_p,
                                     serviceHandleIndex,
                                     SDP_DATA_ELEMENT_SIZE_128_BITS,
                                     &temp,
                                     serviceUuid_pp,
                                     csrBtResult_p,
                                     csrBtTask_p,
                                     csrBtTaskResult_p);
}
#endif
CsrBool CsrBtUtilSdrGetServiceHandle(CmnCsrBtLinkedListStruct *bll_p,
                          CsrUintFast16           serviceHandleIndex,
                          CsrBtUuid32             *serviceHandle_p)
{
    CsrBool   retBool = FALSE;

    if (bll_p)
    {
        if (serviceHandleIndex < CsrBtUtilBllGetNofEntriesEx(bll_p))
        {
            CmnCsrBtLinkedListStruct *bllTemp_p = NULL;

            bllTemp_p = CsrBtUtilBllGetEntryPtrFromIndexEx(bll_p, serviceHandleIndex);

            if (bllTemp_p && bllTemp_p->data)
            {
                CsrUint8 *p_data = bllTemp_p->data;
                retBool = TRUE;
                CsrMemCpy(serviceHandle_p, (p_data + SDR_ENTRY_INDEX_SERVICE_HANDLE), SDR_ENTRY_SIZE_SERVICE_UUID32);
            }
        }
    }
    return retBool;
}

CsrBool CsrBtUtilSdrGetIndexForServiceUuid32(CmnCsrBtLinkedListStruct *bll_p,
                                  CsrBtUuid32              serviceUuid,
                                  CsrUint16             *serviceIndex_p)
{
    return cmnSdrGetIndexForServiceUuid(bll_p,
                                    SDP_DATA_ELEMENT_SIZE_32_BITS,
                                    serviceUuid,
                                    NULL,
                                    serviceIndex_p);
}

#ifdef CSR_BT_INSTALL_128_BIT_SERVICE_SEARCH
CsrBool CsrBtUtilSdrGetIndexForServiceUuid128(CmnCsrBtLinkedListStruct *bll_p,
                                   CsrBtUuid128            *serviceUuid_p,
                                   CsrUint16             *serviceIndex_p)
{
    return cmnSdrGetIndexForServiceUuid(bll_p,
                                    SDP_DATA_ELEMENT_SIZE_128_BITS,
                                    0,
                                    serviceUuid_p,
                                    serviceIndex_p);
}
#endif

CsrBool CsrBtUtilSdrGetLocalServerChannel(CmnCsrBtLinkedListStruct *bll_p,
                                   CsrUintFast16                 serviceHandleIndex,
                                   CsrUint16                 *localServerChannel_p)
{
    CsrBool   retBool = FALSE;
    CmnCsrBtLinkedListStruct *bllTemp_p = NULL;

    if (bll_p)
    {
        if (serviceHandleIndex < CsrBtUtilBllGetNofEntriesEx(bll_p))
        {
            bllTemp_p = CsrBtUtilBllGetEntryPtrFromIndexEx(bll_p, serviceHandleIndex);

            if (bllTemp_p && bllTemp_p->data)
            {
                CsrUint8 *p_data = bllTemp_p->data;
                retBool = TRUE;
                CsrMemCpy(localServerChannel_p, (p_data + SDR_ENTRY_INDEX_SERVICE_LOCAL_SERVER_CH), SDR_ENTRY_SIZE_SERVICE_LOCAL_SERVER_CH);
            }
        }
    }
    return retBool;
}

CsrBool CsrBtUtilSdrPerformExtendedSearch(CmnCsrBtLinkedListStruct *bll_p,
                                          CsrUintFast16                serviceHandleIndex)
{
    CsrBool   retBool = FALSE;
    CmnCsrBtLinkedListStruct *bllTemp_p = NULL;

    if (bll_p)
    {
        if (serviceHandleIndex < CsrBtUtilBllGetNofEntriesEx(bll_p))
        {
            bllTemp_p = CsrBtUtilBllGetEntryPtrFromIndexEx(bll_p, serviceHandleIndex);

            if (bllTemp_p && bllTemp_p->data)
            {
                CsrUint16 temp;
                CsrBool *p_data = bllTemp_p->data;
                CsrMemCpy(&temp, (p_data + SDR_ENTRY_INDEX_SERVICE_EXTENDED_SEARCH), SDR_ENTRY_SIZE_SERVICE_EXTENDED_SEARCH);
                if (temp == SDR_SDC_PERFORM_EXT_RFC_SDC_SEARCH)
                {
                    retBool = TRUE;
                }
            }
        }
    }
    return retBool;
}

/* **************************************************** */
/* **************************************************** */
/* GET ATTRIBUTE FUNCTIONS */
/* **************************************************** */
/* **************************************************** */

CsrBool CsrBtUtilSdrGetNofAttributes(CmnCsrBtLinkedListStruct *bll_p,
                          CsrUintFast16              serviceHandleIndex,
                          CsrUintFast16             *nofAttributes_p)
{
    CsrBool   retBool = FALSE;
    CsrUint16 nofAtt;
    if (bll_p)
    {
        if (serviceHandleIndex < CsrBtUtilBllGetNofEntriesEx(bll_p))
        {
            CmnCsrBtLinkedListStruct *bllTemp_p = NULL;

            bllTemp_p = CsrBtUtilBllGetEntryPtrFromIndexEx(bll_p, serviceHandleIndex);

            if (bllTemp_p && bllTemp_p->data)
            {
                CsrUint8 *p_data = bllTemp_p->data;
                retBool = TRUE;
                CsrMemCpy(&nofAtt, (p_data + SDR_ENTRY_INDEX_SERVICE_NOF_ATTRIBUTES), SDR_ENTRY_SIZE_SERVICE_UINT16);
                *nofAttributes_p =nofAtt;
            }
        }
    }
    return retBool;
}

CsrUint8 *CsrBtUtilSdrGetAttributePointer(CmnCsrBtLinkedListStruct *bll_p,
                                  CsrUintFast16              serviceHandleIndex,
                                  CsrUintFast16              attributeIndex,
                                  CsrUint16             *nofBytesToAttribute_p)
{
    CsrBool    found = FALSE;
    CsrUint16  sdrHeaderLength, tempVar;
    CsrUint16  attLen, sdrUuidType, tempLen;
    CsrUint16 serviceUuidType;
    CsrUintFast16 x, nofAttributes=0;
    CsrUint8  *sdr_p = CsrBtUtilBllGetDataPointerEx(bll_p, serviceHandleIndex, &tempLen);

    if (sdr_p)
    {
        if (TRUE == CsrBtUtilSdrGetServiceUuidType(bll_p, serviceHandleIndex, &serviceUuidType))
        {
            if (TRUE == cmnSdrGetEmptySdrSize(serviceUuidType, nofBytesToAttribute_p))
            {
                if (TRUE == CsrBtUtilSdrGetNofAttributes(bll_p, serviceHandleIndex, &nofAttributes))
                {
                    if (nofAttributes > attributeIndex)
                    {
                        if (TRUE == CsrBtUtilSdrGetServiceUuidType(bll_p, serviceHandleIndex, &sdrUuidType))
                        {
                            if (TRUE == cmnSdrGetEmptySdrSize(sdrUuidType, &sdrHeaderLength))
                            {
                                sdr_p += sdrHeaderLength;
                                for (x=0; x<attributeIndex; x++)
                                {
                                    CsrMemCpy(&tempVar, sdr_p, SDR_ENTRY_SIZE_SERVICE_UINT16);

                                    attLen                  = tempVar + SDR_ENTRY_SIZE_TAG_LENGTH;
                                    *nofBytesToAttribute_p += attLen;
                                    sdr_p                  += attLen;
                                }
                                found                 = TRUE;
                            }
                        }
                    }
                }
            }
        }
    }

    if (FALSE == found)
    {
        sdr_p = NULL;
        *nofBytesToAttribute_p = 0;
    }
    return sdr_p;
}

CsrBool CsrBtUtilSdrGetAttributeUuid(CmnCsrBtLinkedListStruct *bll_p,
                          CsrUintFast16              serviceHandleIndex,
                          CsrUintFast16              attributeIndex,
                          CsrUint16             *attributeUuid_p)
{
    CsrBool    retBool = FALSE;
    CsrUint8  *att_p;
    CsrUint16  nofBytesToAttribute;

    att_p = CsrBtUtilSdrGetAttributePointer(bll_p, serviceHandleIndex, attributeIndex, &nofBytesToAttribute);

    if (att_p)
    {
        CsrMemCpy(attributeUuid_p, (att_p + SDR_ENTRY_INDEX_ATTRIBUTE_UUID), SDR_ENTRY_SIZE_SERVICE_UINT16);
        retBool = TRUE;
    }
    return retBool;
}

CsrBool CsrBtUtilSdrGetUintAttributeDataValueFromAttributeUuid(CmnCsrBtLinkedListStruct *bll_p,
                                                    CsrUintFast16              serviceHandleIndex,
                                                    CsrUint16              attributeUuid,
                                                    CsrUint32             *returnValue_p)
{
    CsrUint16  dataLen;
    CsrUint8   *data;
    CsrBool    retBool = FALSE;

    data = CsrBtUtilSdrGetInnerAttrData(bll_p, serviceHandleIndex, attributeUuid, &dataLen);

    if (data)
    { /* Then find the data pointer */
        CsrUint16 consumedBytes;

        /* Then find the real value, either an unsigned int or UUID type */
        if (CsrBtUtilSdpExtractUint(data, dataLen, returnValue_p, &consumedBytes, TRUE))
        {
            retBool = TRUE;
        }
    }
    return retBool;
}

CsrBool CsrBtUtilSdrGetServiceClassIdList(CmnCsrBtLinkedListStruct *bll_p,
                               CsrUintFast16              serviceHandleIndex,
                               CsrUint16             *nofServiceClassIds_p,
                               CsrUint32            **serviceClassIdList_pp)
{
    CsrBool    retBool = FALSE;

    CsrUint8  *att_p;
    CsrUint16  attDataLen, nofBytesToAttribute, emptyAttSize, consumedBytes, tempVar;
    CsrUintFast16 x, nofAttributes=0;

    if (TRUE == CsrBtUtilSdrGetNofAttributes(bll_p, serviceHandleIndex, &nofAttributes))
    {
        for (x=0; x<nofAttributes; x++)
        {
            att_p = CsrBtUtilSdrGetAttributePointer(bll_p, serviceHandleIndex, x, &nofBytesToAttribute);
            CsrMemCpy(&tempVar, att_p + SDR_ENTRY_INDEX_ATTRIBUTE_UUID, SDR_ENTRY_SIZE_SERVICE_UINT16);
            if (CSR_BT_SERVICE_CLASS_ID_LIST == tempVar)
            {
                CsrBtUtilSdrGetEmptyAttributeSize(&emptyAttSize);
                CsrMemCpy(&tempVar, att_p, SDR_ENTRY_SIZE_SERVICE_UINT16);
                attDataLen = tempVar - emptyAttSize + SDR_ENTRY_SIZE_TAG_LENGTH;
                if (TRUE == CsrBtUtilSdpExtractServiceClassIdList(att_p + SDR_ENTRY_INDEX_ATTRIBUTE_DATA,
                                                         attDataLen,
                                                         nofServiceClassIds_p,
                                                         serviceClassIdList_pp,
                                                         &consumedBytes))
                {
                    retBool = TRUE;
                }
            }
        }
    }
    return retBool;
}

CsrBool CsrBtUtilSdrGetStringAttributeFromAttributeUuid(CmnCsrBtLinkedListStruct *bll_p,
                                                 CsrUintFast16              serviceHandleIndex,
                                                 CsrUint16              attributeUuid,
                                                 CsrUint8             **string_pp,
                                                 CsrUint16             *stringLen_p)
{
    CsrUint16  dataLen;
    CsrUint8   *data;
    CsrBool    retBool = FALSE;

    data = CsrBtUtilSdrGetInnerAttrData(bll_p, serviceHandleIndex, attributeUuid, &dataLen);

    if (data)
    { /* Then find the data pointer */
        CsrUint16 consumedBytes;

        if (CsrBtUtilSdpExtractString(data, dataLen, string_pp, stringLen_p, &consumedBytes))
        {
            retBool = TRUE;
        }
    }
    return retBool;
}



CsrBool CsrBtUtilSdrGetDataElementSequenceDataAttributeFromAttributeUuid(CmnCsrBtLinkedListStruct *bll_p,
                                                                  CsrUintFast16              serviceHandleIndex,
                                                                  CsrUint16              attributeUuid,
                                                                  CsrUint8               **data_pp,
                                                                  CsrUint32              *dataLen_p)
{
    CsrUint16  dataLen;
    CsrUint8   *data;
    CsrBool    retBool = FALSE;

    data = CsrBtUtilSdrGetInnerAttrData(bll_p, serviceHandleIndex, attributeUuid, &dataLen);

    if (data)
    { /* Then find the data pointer */
        CsrUint16 consumedBytes;
        if (CsrBtUtilSdpExtractDataElementSequence(data, dataLen, dataLen_p, &consumedBytes))
        {
            *data_pp = data + consumedBytes;
            retBool  = TRUE;
        }
    }
    return retBool;
}

CsrBool CsrBtUtilSdrGetUintDataAttributeFromAttributeUuid(CmnCsrBtLinkedListStruct *bll_p,
                                                          CsrUintFast16        serviceHandleIndex,
                                                          CsrUint16        attributeUuid,
                                                          CsrUint8         **data_pp,
                                                          CsrUint32        *dataLen_p)
{
    CsrUint16  dataLen;
    CsrUint8   *data;
    CsrBool    retBool = FALSE;

    data = CsrBtUtilSdrGetInnerAttrData(bll_p, serviceHandleIndex, attributeUuid, &dataLen);

    if (data)
    { /* Then find the data pointer */
        CsrUint16 consumedBytes;
        CsrUint32 uintDataSize;

        if (CsrBtUtilSdpUintData(data, dataLen, &uintDataSize, &consumedBytes))
        {
            *data_pp    = data + consumedBytes;
            *dataLen_p  = uintDataSize;
            retBool     = TRUE;
        }
    }
    return retBool;
}

CsrBool CsrBtUtilSdrGetUuidDataAttributeFromAttributeUuid(CmnCsrBtLinkedListStruct *bll_p,
                                                          CsrUintFast16        serviceHandleIndex,
                                                          CsrUint16        attributeUuid,
                                                          CsrUint8         **data_pp,
                                                          CsrUint32        *dataLen_p)
{
    CsrUint16  dataLen;
    CsrUint8   *data;
    CsrBool    retBool = FALSE;

    data = CsrBtUtilSdrGetInnerAttrData(bll_p, serviceHandleIndex, attributeUuid, &dataLen);

    if (data)
    { /* Then find the data pointer */
        CsrUint16 consumedBytes;
        CsrUint32 uintDataSize;

        if (CsrBtUtilSdpUuidData(data, dataLen, &uintDataSize, &consumedBytes))
        {
            *data_pp    = data + consumedBytes;
            *dataLen_p  = uintDataSize;
            retBool     = TRUE;
        }
    }
    return retBool;
}


/* **************************************************** */
/* **************************************************** */
/* CREATE FUNCTIONS */
/* **************************************************** */
/* **************************************************** */

static CsrBool cmnSdrCheckForNewUuidType(CmnCsrBtLinkedListStruct *bll_p,
                             CsrBtUuid32              serviceUuidType)
{
    CsrBool retBool = FALSE;

    CsrUintFast16 nofEntries = CsrBtUtilBllGetNofEntriesEx(bll_p);
    uuid16_t serviceUuidTypeTemp;
    CsrUintFast16 x;

    for (x=0; x<nofEntries; x++)
    {
        if (TRUE == CsrBtUtilSdrGetServiceUuidType(bll_p, x, &serviceUuidTypeTemp))
        {
            if (serviceUuidTypeTemp != serviceUuidType)
            {
                retBool = TRUE;
            }
        }
    }

    return retBool;
}

static CsrUint8 *cmnSdrCreateEmptySdrData(CsrUint16    serviceUuidType,
                              CsrBtUuid32    serviceUuid32,
                              CsrBtUuid128  *serviceUuid128_p,
                              CsrUint16   *emptySdrSize_p)
{
    CsrUint8 *return_p = NULL;
    CsrUint16 tempVal;

    /* Create 'empty' SDR shell */
    if (TRUE == cmnSdrGetEmptySdrSize(serviceUuidType, emptySdrSize_p))
    {
        return_p = (CsrUint8 *)CsrPmemAlloc(*emptySdrSize_p);

        /* Insert data into new entry from template */
        CsrMemSet(return_p, 0x00, *emptySdrSize_p);

        /* Insert total tag size, not including the length of the length field itself */
        tempVal = *emptySdrSize_p - SDR_ENTRY_SIZE_TAG_LENGTH;
        CsrMemCpy(return_p, &tempVal, SDR_ENTRY_SIZE_SERVICE_UINT16);

        /* Insert the SDR tag identifier (CSR_BT_TAG_SDR_ENTRY) */
        tempVal = CSR_BT_TAG_SDR_ENTRY;
        CsrMemCpy(return_p + SDR_ENTRY_INDEX_TAG_ID, &tempVal, SDR_ENTRY_SIZE_SERVICE_UINT16);

        /* Insert the UUID type (32 or 128 bits length) */
        tempVal = serviceUuidType;
        CsrMemCpy(return_p + SDR_ENTRY_INDEX_SERVICE_UUID_TYPE, &tempVal, SDR_ENTRY_SIZE_SERVICE_UINT16);

        /* Insert the actual UUID value */
        if (SDP_DATA_ELEMENT_SIZE_32_BITS == serviceUuidType)
        {
            CsrMemCpy((return_p + SDR_ENTRY_INDEX_SERVICE_UUID), &serviceUuid32, SDR_ENTRY_SIZE_SERVICE_UUID32);
        }
#ifdef CSR_BT_INSTALL_128_BIT_SERVICE_SEARCH
        else if (SDP_DATA_ELEMENT_SIZE_128_BITS == serviceUuidType)
        {
            CsrMemCpy((return_p + SDR_ENTRY_INDEX_SERVICE_UUID), serviceUuid128_p, SDR_ENTRY_SIZE_SERVICE_UUID128);
        }
#endif
        /* Insert default result data */
        tempVal = SDR_SDC_SEARCH_FAILED;
        CsrMemCpy(return_p + SDR_ENTRY_INDEX_SERVICE_CSR_BT_RESULT, &tempVal, SDR_ENTRY_SIZE_SERVICE_UINT16);

        tempVal = SDR_DUMMY_TASK;
        CsrMemCpy(return_p + SDR_ENTRY_INDEX_SERVICE_CSR_BT_TASK, &tempVal, SDR_ENTRY_SIZE_SERVICE_UINT16);

        tempVal = SDR_SDC_SEARCH_FAILED;
        CsrMemCpy(return_p + SDR_ENTRY_INDEX_SERVICE_CSR_BT_TASK_RESULT, &tempVal, SDR_ENTRY_SIZE_SERVICE_UINT16);

        tempVal = CMN_SDR_INVALID_SERVICE_HANDLE;
        CsrMemCpy(return_p + SDR_ENTRY_INDEX_SERVICE_HANDLE, &tempVal, SDR_ENTRY_SIZE_SERVICE_UINT16);

        /* Insert local server channel */
        tempVal = CSR_BT_NO_SERVER;
        CsrMemCpy(return_p + SDR_ENTRY_INDEX_SERVICE_LOCAL_SERVER_CH, &tempVal, SDR_ENTRY_SIZE_SERVICE_LOCAL_SERVER_CH);
    }
    return return_p;
}

static CmnCsrBtLinkedListStruct *cmnSdrCreateServiceHandleEntryFromUuid(CmnCsrBtLinkedListStruct *bll_p,
                                                         CsrUint16              serviceUuidType,
                                                         CsrBtUuid32              serviceUuid32,
                                                         CsrBtUuid128            *serviceUuid128_p,
                                                         CsrUint16             *newIndex_p)
{
    CmnCsrBtLinkedListStruct *newBllEntry_p = NULL, *returnBll_p = NULL;

    CsrUint16 emptySdrSize   = 0;
    CsrUintFast16 lastEntryIndex = 0;

    if (FALSE == cmnSdrCheckForNewUuidType(bll_p, serviceUuidType))
    {
        /* Create new Entry */
        newBllEntry_p = (CmnCsrBtLinkedListStruct *)CsrPmemAlloc(sizeof(CmnCsrBtLinkedListStruct));
        if (NULL == bll_p)
        {
            /* Create new Entry and make that the new linked list */
            newBllEntry_p->data      = NULL;
            newBllEntry_p->dataLen   = 0;
            newBllEntry_p->nextEntry = NULL;
            newBllEntry_p->prevEntry = NULL;
            returnBll_p              = newBllEntry_p;
        }
        else
        {
            /* Create new Entry to add to the bll linked list */
            lastEntryIndex = CsrBtUtilBllGetNofEntriesEx(bll_p);

            newBllEntry_p->data      = NULL;
            newBllEntry_p->dataLen   = 0;
            newBllEntry_p->nextEntry = NULL;
            newBllEntry_p->prevEntry = CsrBtUtilBllGetEntryPtrFromIndexEx(bll_p, (CsrUintFast16)(lastEntryIndex-1));
            if (newBllEntry_p->prevEntry)
            {
                (newBllEntry_p->prevEntry)->nextEntry = newBllEntry_p;
            }
            returnBll_p              = bll_p;
        }
        *newIndex_p = (CsrUint16)lastEntryIndex;

        newBllEntry_p->data    = cmnSdrCreateEmptySdrData(serviceUuidType,
                                                      serviceUuid32,
                                                      serviceUuid128_p,
                                                      &emptySdrSize);
        newBllEntry_p->dataLen = emptySdrSize;
    }
    return returnBll_p;
}

CmnCsrBtLinkedListStruct *CsrBtUtilSdrCreateServiceHandleEntryFromUuid32(CmnCsrBtLinkedListStruct *bll_p,
                                                           CsrBtUuid32              serviceUuid32,
                                                           CsrUint16             *newIndex_p)
{
    return cmnSdrCreateServiceHandleEntryFromUuid(bll_p,
                                              SDP_DATA_ELEMENT_SIZE_32_BITS,
                                              serviceUuid32,
                                              NULL,
                                              newIndex_p);
}

#ifdef CSR_BT_INSTALL_128_BIT_SERVICE_SEARCH
CmnCsrBtLinkedListStruct *CsrBtUtilSdrCreateServiceHandleEntryFromUuid128(CmnCsrBtLinkedListStruct *bll_p,
                                                            CsrBtUuid128            *serviceUuid128_p,
                                                            CsrUint16             *newIndex_p)
{
    return cmnSdrCreateServiceHandleEntryFromUuid(bll_p,
                                              SDP_DATA_ELEMENT_SIZE_128_BITS,
                                              0,
                                              serviceUuid128_p,
                                              newIndex_p);
}
#endif

/* **************************************************** */
/* **************************************************** */
/* INSERT SDR FUNCTIONS */
/* **************************************************** */
/* **************************************************** */

CsrBool CsrBtUtilSdrInsertSdrLength(CmnCsrBtLinkedListStruct *bll_p,
                         CsrUintFast16              serviceHandleIndex,
                         CsrUint16              sdrDataLength)
{
    CmnCsrBtLinkedListStruct *bllEntry_p = NULL;

    CsrBool    retBool = FALSE;

    bllEntry_p = CsrBtUtilBllGetEntryPtrFromIndexEx(bll_p, serviceHandleIndex);
    if (bllEntry_p)
    {
        if (0 != sdrDataLength && bllEntry_p->data)
        {
            *(CsrUint16 *)bllEntry_p->data = (sdrDataLength - SDR_ENTRY_SIZE_TAG_LENGTH);
        }
        else
        {
            if (bllEntry_p->data)
            {
                CsrPmemFree(bllEntry_p->data);
                bllEntry_p->data = NULL;
            }
            sdrDataLength = 0;
        }
        bllEntry_p->dataLen           = sdrDataLength;

        retBool = TRUE;
    }

    return retBool;
}

static CsrBool cmnSdrInsertServiceHandleAtUuid (CmnCsrBtLinkedListStruct *bll_p,
                                               CsrUint16              serviceUuidType,
                                               CsrBtUuid32              serviceUuid32,
                                               CsrBtUuid128            *serviceUuid128_p,
                                               CsrBtUuid32              serviceHandle)
{
    CsrBool      retBool = FALSE, continueVal;
    CsrUintFast16    nofEntries, x;
    CsrUint16 temp1, temp2, temp3, tempVal;
    CsrBtUuid32    serviceHandleTemp = 0;
    CsrBtUuid32    serviceUuid32Temp = 0;
    CsrBtUuid128  *serviceUuid128Temp_p = NULL;

    CmnCsrBtLinkedListStruct *bllTemp_p = NULL;

    if (bll_p)
    {
        nofEntries = CsrBtUtilBllGetNofEntriesEx(bll_p);
        for (x=0; x<nofEntries; x++)
        {
            continueVal = FALSE;
            if (TRUE == cmnSdrGetServiceUuidAndResult(bll_p,
                                                      x,
                                                      serviceUuidType,
                                                      &serviceUuid32Temp,
                                                      &serviceUuid128Temp_p,
                                                      &temp1,
                                                      &temp2,
                                                      &temp3))
            {
                if (SDP_DATA_ELEMENT_SIZE_32_BITS == serviceUuidType)
                {
                    if (serviceUuid32Temp == serviceUuid32)
                    {
                        continueVal = TRUE;
                    }
                }
#ifdef CSR_BT_INSTALL_128_BIT_SERVICE_SEARCH
                else if (SDP_DATA_ELEMENT_SIZE_128_BITS == serviceUuidType)
                {
                    if (0 == CsrMemCmp((CsrUint8 *)serviceUuid128_p, (CsrUint8 *)serviceUuid128Temp_p, SDP_DATA_ELEMENT_SIZE_128_BITS))
                    {
                        continueVal = TRUE;
                    }
                }
#endif
            }
            if (TRUE == continueVal)
            {
                if (TRUE == CsrBtUtilSdrGetServiceHandle(bll_p, x, &serviceHandleTemp))
                {
                    if (CMN_SDR_INVALID_SERVICE_HANDLE == serviceHandleTemp)
                    {
                        CsrUint8 *p_data;

                        bllTemp_p = CsrBtUtilBllGetEntryPtrFromIndexEx(bll_p, x);

                        if (bllTemp_p && bllTemp_p->data)
                        {
                            p_data = bllTemp_p->data;

                            CsrMemCpy((p_data + SDR_ENTRY_INDEX_SERVICE_HANDLE), &serviceHandle, SDR_ENTRY_SIZE_SERVICE_UUID32);
                            /* Insert default OK-result data */
                            tempVal = SDR_SDC_SEARCH_SUCCESS;
                            CsrMemCpy((p_data + SDR_ENTRY_INDEX_SERVICE_CSR_BT_RESULT), &tempVal, SDR_ENTRY_SIZE_SERVICE_UINT16);

                            tempVal = SDR_SDC_SEARCH_SUCCESS;
                            CsrMemCpy((p_data + SDR_ENTRY_INDEX_SERVICE_CSR_BT_TASK), &tempVal, SDR_ENTRY_SIZE_SERVICE_UINT16);

                            tempVal = SDR_SDC_SEARCH_SUCCESS;
                            CsrMemCpy((p_data + SDR_ENTRY_INDEX_SERVICE_CSR_BT_TASK_RESULT), &tempVal, SDR_ENTRY_SIZE_SERVICE_UINT16);

                            retBool = TRUE;
                            /* Now break out of the for-loop! */
                            x = nofEntries;
                        }
                    }
                }
            }
        }
    }
    return retBool;
}

CsrBool CsrBtUtilSdrInsertServiceHandleAtUuid32 (CmnCsrBtLinkedListStruct *bll_p,
                                      CsrBtUuid32              serviceUuid,
                                      CsrBtUuid32              serviceHandle)
{
    return cmnSdrInsertServiceHandleAtUuid (bll_p,
                                        SDP_DATA_ELEMENT_SIZE_32_BITS,
                                        serviceUuid,
                                        NULL,
                                        serviceHandle);
}

#ifdef CSR_BT_INSTALL_128_BIT_SERVICE_SEARCH
CsrBool CsrBtUtilSdrInsertServiceHandleAtUuid128(CmnCsrBtLinkedListStruct *bll_p,
                                      CsrBtUuid128            *serviceUuid_p,
                                      CsrBtUuid32              serviceHandle)
{
    return cmnSdrInsertServiceHandleAtUuid (bll_p,
                                        SDP_DATA_ELEMENT_SIZE_128_BITS,
                                        0,
                                        serviceUuid_p,
                                        serviceHandle);
}
#endif
CsrBool CsrBtUtilSdrInsertLocalServerChannel(CmnCsrBtLinkedListStruct *bll_p,
                                      CsrUintFast16                serviceHandleIndex,
                                      CsrUint16                localServerChannel)
{
    CmnCsrBtLinkedListStruct *bllEntry_p = NULL;

    CsrBool retBool = FALSE;

    bllEntry_p = CsrBtUtilBllGetEntryPtrFromIndexEx(bll_p, serviceHandleIndex);

    if (bllEntry_p)
    {
        CsrUint8 *p_data = bllEntry_p->data;
        CsrMemCpy((p_data + SDR_ENTRY_INDEX_SERVICE_LOCAL_SERVER_CH), &localServerChannel, SDR_ENTRY_SIZE_SERVICE_LOCAL_SERVER_CH);

        retBool = TRUE;
    }

    return retBool;
}

CsrBool CsrBtUtilSdrInsertPerformExtendedSearch(CmnCsrBtLinkedListStruct *bll_p,
                                                CsrUintFast16                serviceHandleIndex)
{
    CmnCsrBtLinkedListStruct *bllEntry_p = NULL;

    CsrBool retBool = FALSE;

    bllEntry_p = CsrBtUtilBllGetEntryPtrFromIndexEx(bll_p, serviceHandleIndex);

    if (bllEntry_p)
    {
        CsrUint8 *p_data = bllEntry_p->data;
        CsrUint16 temp   = SDR_SDC_PERFORM_EXT_RFC_SDC_SEARCH;
        CsrMemCpy((p_data + SDR_ENTRY_INDEX_SERVICE_EXTENDED_SEARCH), &temp, SDR_ENTRY_SIZE_SERVICE_EXTENDED_SEARCH);
    }
    return retBool;
}

/* **************************************************** */
/* **************************************************** */
/* INSERT ATTRIBUTE FUNCTIONS */
/* **************************************************** */
/* **************************************************** */

static CsrBool cmnSdrInsertAttributeLength(CmnCsrBtLinkedListStruct *bll_p,
                                          CsrUintFast16              serviceHandleIndex,
                                          CsrUintFast16              attributeIndex,
                                          CsrUint16              length)
{
    CsrBool    retBool = FALSE;

    CsrUint8  *att_p;
    CsrUint16  tempLen, tempVar;

    att_p = CsrBtUtilSdrGetAttributePointer(bll_p, serviceHandleIndex, attributeIndex, &tempLen);

    if (att_p)
    {
        tempVar = (length - SDR_ENTRY_SIZE_TAG_LENGTH);
        CsrMemCpy(att_p, &tempVar, SDR_ENTRY_SIZE_SERVICE_UINT16);

        retBool = TRUE;
    }

    return retBool;
}

static CsrBool cmnSdrInsertAttribute(CmnCsrBtLinkedListStruct *bll_p,
                                    CsrUintFast16              serviceHandleIndex,
                                    CsrUint8              *attribute_p)
{
    CsrBool   retBool = FALSE;

    CsrUint8 *sdr_p = NULL, *newSdr_p = NULL;
    CsrUint16 sdrSize=0, attributeSize=0, newTotalSize, tempLen, tempVal;
    CsrUintFast16 nofAttributes = 0;

    CmnCsrBtLinkedListStruct *bllEntry_p;

    sdr_p         = CsrBtUtilBllGetDataPointerEx(bll_p, serviceHandleIndex, &tempLen);

    CsrMemCpy(&tempVal, sdr_p, SDR_ENTRY_SIZE_SERVICE_UINT16);
    sdrSize       = (tempVal + SDR_ENTRY_SIZE_TAG_LENGTH);

    CsrMemCpy(&tempVal, attribute_p, SDR_ENTRY_SIZE_SERVICE_UINT16);
    attributeSize = (tempVal + SDR_ENTRY_SIZE_TAG_LENGTH);
    newTotalSize  = sdrSize + attributeSize;

    if (newTotalSize > 0)
    {
        newSdr_p      = CsrPmemAlloc(newTotalSize);
        CsrMemCpy(newSdr_p, sdr_p, sdrSize);
        CsrMemCpy(newSdr_p + sdrSize, attribute_p, attributeSize);

        if (TRUE == CsrBtUtilSdrGetNofAttributes(bll_p, serviceHandleIndex, &nofAttributes))
        {
            tempVal = nofAttributes + 1;
            CsrMemCpy((newSdr_p + SDR_ENTRY_INDEX_SERVICE_NOF_ATTRIBUTES), &tempVal, SDR_ENTRY_SIZE_SERVICE_UINT16);

            bllEntry_p = CsrBtUtilBllGetEntryPtrFromIndexEx(bll_p, serviceHandleIndex);

            if (bllEntry_p)
            {
                CsrPmemFree(bllEntry_p->data);

                bllEntry_p->data    = newSdr_p;

                CsrBtUtilSdrInsertSdrLength(bll_p, serviceHandleIndex, newTotalSize);

                retBool             = TRUE;
            }
            else
            {
                CsrPmemFree(newSdr_p);
            }
        }
        else
        {
            CsrPmemFree(newSdr_p);
        }
    }
    return retBool;
}

CsrBool CsrBtUtilSdrInsertAttributeDataAtAttributeUuid(CmnCsrBtLinkedListStruct *bll_p,
                                            CsrUintFast16          serviceHandleIndex,
                                            CsrUintFast16          attributeUuidIndex,
                                            CsrUint16              attDataLen,
                                            CsrUint8               *attData_p)
{
    CsrBool    retBool = FALSE;
    CsrUint8  *att_p = NULL, *newSdr_p = NULL;
    CsrUint16  emptyAttributeSize, oldAttributeDataLen, newAttributeLen, sizeUntilAtt, restDataLen, newSdrSize, tempVar;

    CmnCsrBtLinkedListStruct *bllEntry_p = NULL;

    if (bll_p)
    {
        bllEntry_p = CsrBtUtilBllGetEntryPtrFromIndexEx(bll_p, serviceHandleIndex);

        if (bllEntry_p && bllEntry_p->data)
        {
            att_p = CsrBtUtilSdrGetAttributePointer(bll_p, serviceHandleIndex, attributeUuidIndex, &sizeUntilAtt);
            if (att_p)
            {
                CsrBtUtilSdrGetEmptyAttributeSize(&emptyAttributeSize);

                CsrMemCpy(&tempVar, att_p, SDR_ENTRY_SIZE_SERVICE_UINT16);
                oldAttributeDataLen = ((tempVar + SDR_ENTRY_SIZE_TAG_LENGTH) - emptyAttributeSize);

                newSdrSize          = bllEntry_p->dataLen + attDataLen;
                newAttributeLen     = emptyAttributeSize + attDataLen;

                if (newSdrSize > 0)
                {
                    newSdr_p = CsrPmemAlloc(newSdrSize);

                    /* copy everything until the data area in the attribute starts */
                    CsrMemCpy(newSdr_p, bllEntry_p->data, (sizeUntilAtt+emptyAttributeSize));
                    /* Copy the new attribute data */
                    CsrMemCpy(newSdr_p + (sizeUntilAtt + emptyAttributeSize), attData_p, attDataLen);

                    /* Copy the rest of the sdr-data (if any) */
                    restDataLen = bllEntry_p->dataLen - (sizeUntilAtt + emptyAttributeSize + oldAttributeDataLen);
                    if (0 < restDataLen)
                    {
                        CsrUint8 *p_data = bllEntry_p->data;
                        CsrMemCpy(newSdr_p + (sizeUntilAtt + newAttributeLen),
                               p_data   + (sizeUntilAtt + emptyAttributeSize + oldAttributeDataLen),
                            restDataLen);
                    }
                }
                else
                {
                    newSdr_p = NULL;
                }

                CsrPmemFree(bllEntry_p->data);
                bllEntry_p->data    = newSdr_p;
                cmnSdrInsertAttributeLength(bll_p, serviceHandleIndex, attributeUuidIndex, newAttributeLen);
                CsrBtUtilSdrInsertSdrLength(bll_p, serviceHandleIndex, newSdrSize);
                retBool = TRUE;
            }
        }
    }
    return retBool;
}

/* **************************************************** */
/* **************************************************** */
/* CREATE ATTRIBUTE FUNCTIONS */
/* **************************************************** */
/* **************************************************** */

static CsrUint8 *cmnSdrCreateAttributeData(CsrUint16  attributeUuid,
                               CsrUint8  *attributeData_p,
                               CsrUint16  attributeDataLen)
{
    CsrUint8 *return_p = NULL;
    CsrUint16 emptyAttributeSize, tempVal;

    /* Create 'empty' attribute shell */
    CsrBtUtilSdrGetEmptyAttributeSize(&emptyAttributeSize);

    if ((emptyAttributeSize + attributeDataLen) > 0)
    {
        return_p = (CsrUint8 *)CsrPmemAlloc(emptyAttributeSize + attributeDataLen);

        /* Insert data into new entry from template */
        CsrMemSet(return_p, 0x00, emptyAttributeSize + attributeDataLen);

        /* Insert total tag size, not including the length of the length field itself */
        tempVal = emptyAttributeSize + attributeDataLen - SDR_ENTRY_SIZE_TAG_LENGTH;
        CsrMemCpy(return_p, &tempVal, SDR_ENTRY_SIZE_SERVICE_UINT16);

        /* Insert the SDR attribute tag identifier (CSR_BT_TAG_SDR_ATTRIBUTE_ENTRY) */
        tempVal = CSR_BT_TAG_SDR_ATTRIBUTE_ENTRY;
        CsrMemCpy(return_p + SDR_ENTRY_INDEX_TAG_ID, &tempVal, SDR_ENTRY_SIZE_SERVICE_UINT16);

        /* Insert the actual attribute UUID value */
        tempVal = attributeUuid;
        CsrMemCpy(return_p + SDR_ENTRY_INDEX_ATTRIBUTE_UUID, &tempVal, SDR_ENTRY_SIZE_SERVICE_UINT16);

        /* Insert default result data */
        tempVal = SDR_SDC_SEARCH_FAILED;
        CsrMemCpy(return_p + SDR_ENTRY_INDEX_ATTRIBUTE_CSR_BT_RESULT, &tempVal, SDR_ENTRY_SIZE_SERVICE_UINT16);

        tempVal = SDR_DUMMY_TASK;
        CsrMemCpy(return_p + SDR_ENTRY_INDEX_ATTRIBUTE_CSR_BT_TASK, &tempVal, SDR_ENTRY_SIZE_SERVICE_UINT16);

        tempVal = SDR_SDC_SEARCH_FAILED;
        CsrMemCpy(return_p + SDR_ENTRY_INDEX_ATTRIBUTE_CSR_BT_TASK_RESULT, &tempVal, SDR_ENTRY_SIZE_SERVICE_UINT16);

        if (0 != attributeDataLen)
        {
            CsrMemCpy (return_p + SDR_ENTRY_INDEX_ATTRIBUTE_DATA, attributeData_p, attributeDataLen);
        }
    }
    return return_p;
}

CsrBool CsrBtUtilSdrCreateAndInsertAttribute(CmnCsrBtLinkedListStruct *bll_p,
                                  CsrUintFast16              serviceHandleIndex,
                                  CsrUint16              attributeUuid,
                                  CsrUint8              *attributeData_p,
                                  CsrUint16              attributeDataLen)
{
    CsrBool  retBool = FALSE;

    CsrUint8 *attribute_p;

    attribute_p = cmnSdrCreateAttributeData(attributeUuid, attributeData_p, attributeDataLen);
    if (attribute_p)
    {
        cmnSdrInsertAttribute(bll_p, serviceHandleIndex, attribute_p);
        CsrPmemFree(attribute_p);
        retBool = TRUE;
    }
    return retBool;
}


/* **************************************************** */
/* **************************************************** */
/* CREATE COMBINED SDR AND ATTRIBUTE */
/* **************************************************** */
/* **************************************************** */

CmnCsrBtLinkedListStruct *CsrBtUtilSdrCreateServiceHandleEntryFromTemplate(CmnCsrBtLinkedListStruct *bll_p,
                                                             CsrUint16             *newBllIndex_p,
                                                             CmnCsrBtLinkedListStruct *templateBll_p,
                                                             CsrUintFast16              templateIndex)
{
    CmnCsrBtLinkedListStruct *returnBll_p = NULL;

    CsrUint8   *attribute_p = NULL;
    CsrUintFast16   x, nofAttributes=0;
    CsrUint16 attributeUuid;
    CsrBtUuid32   serviceUuid32     = 0;
    CsrBtUuid128 *serviceUuid128_p  = NULL;
    CsrBool     continueVar       = FALSE;
    /* Silence compiler warnings */
    CsrUint16   serviceUuidTypeTemp = 0, localServerChannel = 0;

    /* Check first that only the same UUID type is used in all theentries in the *bll linked list
       and the new wanted entry from the template !!!!! */

    if (templateBll_p)
    {
        if (templateIndex < CsrBtUtilBllGetNofEntriesEx(templateBll_p))
        {
            if (TRUE == CsrBtUtilSdrGetLocalServerChannel(templateBll_p, templateIndex, &localServerChannel))
            {
                if (TRUE == CsrBtUtilSdrGetServiceUuidType(templateBll_p, templateIndex, &serviceUuidTypeTemp))
                {
                    if (SDP_DATA_ELEMENT_SIZE_32_BITS == serviceUuidTypeTemp)
                    {
                        if (TRUE == CsrBtUtilSdrGetServiceUuid32 (templateBll_p, templateIndex, &serviceUuid32))
                        {
                            continueVar = TRUE;
                        }
                    }
#ifdef CSR_BT_INSTALL_128_BIT_SERVICE_SEARCH
                    else if (SDP_DATA_ELEMENT_SIZE_128_BITS == serviceUuidTypeTemp)
                    {
                        if (TRUE == CsrBtUtilSdrGetServiceUuid128 (templateBll_p, templateIndex, &serviceUuid128_p))
                        {
                            continueVar = TRUE;
                        }
                    }
#endif
                }
            }
        }
    }
    if (TRUE == continueVar)
    {
        returnBll_p = cmnSdrCreateServiceHandleEntryFromUuid(bll_p,
                                                         serviceUuidTypeTemp,
                                                         serviceUuid32,
                                                         serviceUuid128_p,
                                                         newBllIndex_p);

        if (TRUE == CsrBtUtilSdrInsertLocalServerChannel(returnBll_p, *newBllIndex_p, localServerChannel))
        {
        /* Add the attribute structures (if any) */
        if (TRUE == CsrBtUtilSdrGetNofAttributes(templateBll_p, templateIndex, &nofAttributes))
        {
            for (x=0; x<nofAttributes; x++)
            {
                if (TRUE ==  CsrBtUtilSdrGetAttributeUuid(templateBll_p, templateIndex, x, &attributeUuid))
                {
                    attribute_p = cmnSdrCreateAttributeData(attributeUuid, NULL, 0);
                    cmnSdrInsertAttribute(returnBll_p, *newBllIndex_p, attribute_p);
                    CsrPmemFree(attribute_p);
                }
            }
        }
    }
    }
    return returnBll_p;
}

/* **************************************************** */
/* **************************************************** */
/* Cleanup functions */
/* **************************************************** */
/* **************************************************** */

CmnCsrBtLinkedListStruct *CsrBtUtilSdrRemoveNonSuccessStatusEntries(CmnCsrBtLinkedListStruct *bll_p)
{
    CsrBtUuid32   serviceUuid32;
    CsrBtUuid128 *serviceUuid128_p;
    CsrUint16   csrBtResult;
    CsrUint16   csrBtTask;
    CsrUint16   csrBtTaskResult;
    CsrIntFast16    x;

    CmnCsrBtLinkedListStruct *newBll_p = bll_p;

    CsrIntFast16   nofEntries = (CsrIntFast16) CsrBtUtilBllGetNofEntriesEx(newBll_p);

    for (x=0; x<nofEntries; x++)
    {
        if (TRUE == cmnSdrGetServiceUuidAndResult(newBll_p,
                                                  x,
                                                  SDP_DATA_ELEMENT_SIZE_32_BITS,
                                                  &serviceUuid32,
                                                  &serviceUuid128_p,
                                                  &csrBtResult,
                                                  &csrBtTask,
                                                  &csrBtTaskResult))
        {
            if (SDR_SDC_SEARCH_SUCCESS != csrBtResult)
            {
                newBll_p    = CsrBtUtilBllFreeLinkedListEntryEx(newBll_p, x, CsrBtUtilBllPfreeWrapper);
                nofEntries -=1;
                x          -=1;
            }
        }
    }

    return newBll_p;
}
