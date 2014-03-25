/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_sd_main.h"

void CsrBtSdMsgTransport(void* msg)
{
    CsrMsgTransport(CSR_BT_SD_IFACEQUEUE, CSR_BT_SD_PRIM, msg);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

static CsrUint16 csrBtConsumeUint16(CsrUint8 *info, CsrUint32 infoLen, CsrUint16 *readPtr)
{
    CsrUint16 data = 0;

    data |= info[(*readPtr)++];
    data |= ((CsrUint16)info[(*readPtr)++]) << 8;
    return data;
}

static CsrUint32 csrBtConsumeUint32(CsrUint8 *info, CsrUint32 infoLen, CsrUint16 *readPtr)
{
    CsrUint32 data = 0;

    data |= info[(*readPtr)++];
    data |= ((CsrUint32)info[(*readPtr)++]) << 8;
    data |= ((CsrUint32)info[(*readPtr)++]) << 16;
    data |= ((CsrUint32)info[(*readPtr)++]) << 24;
    return data;
}

#ifdef CSR_BT_INSTALL_128_BIT_SERVICE_SEARCH
static const CsrUint8 uuidBase[12] = {0xFB, 0x34, 0x9B, 0x5F, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00};

static CsrUint32 csrBtConsumeUuid128As32(CsrUint8 *info, CsrUint32 infoLen, CsrUint16 *readPtr)
{
    CsrUint32 data = 0;

    if (CsrMemCmp(info + (*readPtr), uuidBase, 12) == 0)
    {
        *readPtr += 12;

        data |= info[(*readPtr)++];
        data |= ((CsrUint32)info[(*readPtr)++]) << 8;
        data |= ((CsrUint32)info[(*readPtr)++]) << 16;
        data |= ((CsrUint32)info[(*readPtr)++]) << 24;
        return data;
    }
    else
    {
        *readPtr += 16;
        return 0;
    }
}
#endif

static CsrUint16 csrBtReadUint16(CsrUint8 *info, CsrUint32 infoLen, CsrUint16 readPtr)
{
    CsrUint16 data = 0;

    data |= info[readPtr++];
    data |= ((CsrUint16)info[readPtr]) << 8;
    return data;
}

CsrUint8 CsrBtSdDecodeFriendlyName(CsrUint8 *info, CsrUint32 infoLen, CsrUint8 **friendlyName)
{
    CsrUint16 readPtr = 0;
    CsrUint8 nameLength = 0;

    *friendlyName = NULL;

    while (readPtr < infoLen)
    {
        CsrUint16 length;
        CsrUint16 type;

        length = csrBtConsumeUint16(info, infoLen, &readPtr);
        type = csrBtReadUint16(info, infoLen, readPtr);
        if (type == CSR_BT_EIR_DATA_TYPE_COMPLETE_LOCAL_NAME)
        {
            nameLength = length - 2;
            readPtr += 2;
            *friendlyName = CsrPmemAlloc(nameLength + 1);
            CsrMemCpy(*friendlyName, info + readPtr, nameLength);
            (*friendlyName)[nameLength] = '\0';
            break;
        }
        else
        {
            readPtr += length;
        }
    }
    return nameLength;
}

CsrBool CsrBtSdIsServicePresent(CsrUint8 *info, CsrUint32 infoLen, CsrUint32 uuid)
{
    CsrUint16 readPtr = 0;

    while (readPtr < infoLen)
    {
        CsrUint16 length;
        CsrUint16 type;

        length = csrBtConsumeUint16(info, infoLen, &readPtr);
        type = csrBtReadUint16(info, infoLen, readPtr);
        if ((type == CSR_BT_EIR_DATA_TYPE_COMPLETE_16_BIT_UUID) ||
            (type == CSR_BT_EIR_DATA_TYPE_MORE_16_BIT_UUID))
        {
            CsrUint16 nextField;
            nextField = readPtr + length;
            readPtr += 2;
            while (readPtr < nextField)
            {
                CsrUint32 tempUuid;
                tempUuid = csrBtConsumeUint16(info, infoLen, &readPtr);
                if (tempUuid == uuid)
                {
                    return TRUE;
                }
            }
        }
        else if ((type == CSR_BT_EIR_DATA_TYPE_COMPLETE_32_BIT_UUID) ||
                 (type == CSR_BT_EIR_DATA_TYPE_MORE_32_BIT_UUID))
        {
            CsrUint16 nextField;
            nextField = readPtr + length;
            readPtr += 2;
            while (readPtr < nextField)
            {
                CsrUint32 tempUuid;
                tempUuid = csrBtConsumeUint32(info, infoLen, &readPtr);
                if (tempUuid == uuid)
                {
                    return TRUE;
                }
            }
        }
#ifdef CSR_BT_INSTALL_128_BIT_SERVICE_SEARCH
        else if ((type == CSR_BT_EIR_DATA_TYPE_COMPLETE_128_BIT_UUID) ||
                 (type == CSR_BT_EIR_DATA_TYPE_MORE_128_BIT_UUID))
        {
            CsrUint16 nextField;
            nextField = readPtr + length;
            readPtr += 2;
            while (readPtr < nextField)
            {
                CsrUint32 tempUuid;
                tempUuid = csrBtConsumeUuid128As32(info, infoLen, &readPtr);
                if ((tempUuid == uuid) && (tempUuid != 0))
                {
                    return TRUE;
                }
            }
        }
#endif
        else
        {
            readPtr += length;
        }
    }
    return FALSE;
}

static CsrUint16 csrBtCountServices(CsrUint8 *info, CsrUint32 infoLen)
{
    CsrUint16 readPtr = 0;
    CsrUint16 serviceCount = 0;

    while (readPtr < infoLen)
    {
        CsrUint16 length;
        CsrUint16 type;

        length = csrBtConsumeUint16(info, infoLen, &readPtr);
        type = csrBtReadUint16(info, infoLen, readPtr);
        if ((type == CSR_BT_EIR_DATA_TYPE_COMPLETE_16_BIT_UUID) ||
            (type == CSR_BT_EIR_DATA_TYPE_MORE_16_BIT_UUID))
        {
            serviceCount += (length - 2) / 2;
            readPtr += length;
        }
        else if ((type == CSR_BT_EIR_DATA_TYPE_COMPLETE_32_BIT_UUID) ||
                 (type == CSR_BT_EIR_DATA_TYPE_MORE_32_BIT_UUID))
        {
            serviceCount += (length - 2) / 4;
            readPtr += length;
        }
#ifdef CSR_BT_INSTALL_128_BIT_SERVICE_SEARCH
        else if ((type == CSR_BT_EIR_DATA_TYPE_COMPLETE_128_BIT_UUID) ||
                 (type == CSR_BT_EIR_DATA_TYPE_MORE_128_BIT_UUID))
        {
            serviceCount += (length - 2) / 16;
            readPtr += length;
        }
#endif
        else
        {
            readPtr += length;
        }
    }
    return serviceCount;
}

CsrUint16 CsrBtSdReadServiceList(CsrUint8 *info, CsrUint32 infoLen, CsrUint32 **serviceList)
{
    CsrUint16 serviceCount;
    CsrUint16 serviceListPtr = 0;

    *serviceList = NULL;

    serviceCount = csrBtCountServices(info, infoLen);
    if (serviceCount > 0)
    {
        CsrUint16 readPtr = 0;
        *serviceList = CsrPmemAlloc(sizeof(CsrUint32) * serviceCount);

        while (readPtr < infoLen)
        {
            CsrUint16 length;
            CsrUint16 type;

            length = csrBtConsumeUint16(info, infoLen, &readPtr);
            type = csrBtReadUint16(info, infoLen, readPtr);
            if ((type == CSR_BT_EIR_DATA_TYPE_COMPLETE_16_BIT_UUID) ||
                (type == CSR_BT_EIR_DATA_TYPE_MORE_16_BIT_UUID))
            {
                CsrUint16 nextField;
                nextField = readPtr + length;
                readPtr += 2;
                while ( (readPtr+2) <= nextField)
                {
                    CsrUint32 tempUuid;
                    tempUuid = csrBtConsumeUint16(info, infoLen, &readPtr);
                    (*serviceList)[serviceListPtr++] = tempUuid;
                }
                readPtr = nextField;
            }
            else if ((type == CSR_BT_EIR_DATA_TYPE_COMPLETE_32_BIT_UUID) ||
                     (type == CSR_BT_EIR_DATA_TYPE_MORE_32_BIT_UUID))
            {
                CsrUint16 nextField;
                nextField = readPtr + length;
                readPtr += 2;
                while ( (readPtr+4) <= nextField)
                {
                    CsrUint32 tempUuid;
                    tempUuid = csrBtConsumeUint32(info, infoLen, &readPtr);
                    (*serviceList)[serviceListPtr++] = tempUuid;
                }
                readPtr = nextField;
            }
#ifdef CSR_BT_INSTALL_128_BIT_SERVICE_SEARCH
            else if ((type == CSR_BT_EIR_DATA_TYPE_COMPLETE_128_BIT_UUID) ||
                     (type == CSR_BT_EIR_DATA_TYPE_MORE_128_BIT_UUID))
            {
                CsrUint16 nextField;
                nextField = readPtr + length;
                readPtr += 2;
                while ( (readPtr+16) <= nextField)
                {
                    CsrUint32 tempUuid;
                    tempUuid = csrBtConsumeUuid128As32(info, infoLen, &readPtr);
                    if (tempUuid != 0)
                    {
                        (*serviceList)[serviceListPtr++] = tempUuid;
                    }
                }
                readPtr = nextField;
            }
#endif
            else
            {
                readPtr += length;
            }
        }
    }
    return serviceListPtr;
}

/* Check if a specific tag is present in the info */
CsrBool CsrBtSdInfoCheckTag(CsrUint16 infoLen, CsrUint8 *info, CsrUint16 tag)
{
    CsrUintFast16 index;

    for (index = 0;
         /* At least a header must still remain */
         (index + CSR_BT_SD_DEVICE_INFO_FORMAT_HEADER_SIZE < infoLen);
         /* Skip the length of the tag (type+value) + the size of the length field */
         index += CSR_GET_UINT16_FROM_LITTLE_ENDIAN(info + index) + CSR_BT_SD_DEVICE_INFO_FORMAT_LEN_SIZE)
    {
        if (CSR_GET_UINT16_FROM_LITTLE_ENDIAN(info + index + CSR_BT_SD_DEVICE_INFO_FORMAT_LEN_SIZE) == tag)
        {
            /* Correct tag found */
            return TRUE;
        }
    }

    return FALSE;
}

/* Get a pointer to the value of a specific tag in the info and the length of the value */
CsrUint16 CsrBtSdInfoGetTag(CsrUint16 infoLen, CsrUint8 *info, CsrUint8 **tagVal, CsrUint16 tag)
{
    CsrIntFast16 index;

    for (index = 0;
         /* At least a header must still remain */
         (index + CSR_BT_SD_DEVICE_INFO_FORMAT_HEADER_SIZE < infoLen);
         /* Skip the length of the tag (type+value) + the size of the length field */
         index += CSR_GET_UINT16_FROM_LITTLE_ENDIAN(info + index) + CSR_BT_SD_DEVICE_INFO_FORMAT_LEN_SIZE)
    {
        if (CSR_GET_UINT16_FROM_LITTLE_ENDIAN(info + index + CSR_BT_SD_DEVICE_INFO_FORMAT_LEN_SIZE) == tag)
        {
            /* Correct tag found */
            if (CSR_GET_UINT16_FROM_LITTLE_ENDIAN(info + index) <= (infoLen - index - CSR_BT_SD_DEVICE_INFO_FORMAT_LEN_SIZE))
            {
                /* Complete tag is available - point to the start of
                 * the actual value */
                *tagVal = info + index + CSR_BT_SD_DEVICE_INFO_FORMAT_HEADER_SIZE;
                return CSR_GET_UINT16_FROM_LITTLE_ENDIAN(info + index)
                    - CSR_BT_SD_DEVICE_INFO_FORMAT_LEN_SIZE; /* Length of tag value */
            }
            else
            {
                /* Incomplete tag available */
                break;
            }
        }
    }

    *tagVal = NULL;
    return 0;
}

#ifndef EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE
/* ************************************************************* */
/* ************************************************************* */
/* Device Identification Service Record                          */
/* ************************************************************* */
/* ************************************************************* */
static const CsrUint8 serviceRecordBrowseGroup[] =
{
    /* BrowseGroupList    */
    0x09, 0x00, 0x05,    /* AttrId = BrowseGroupList */
    0x35, 0x03,          /* Data element seq. 3 bytes */
    0x19, 0x10, 0x02    /* 2 byte UUID, PublicBrowseGroup = 0x1002 */
};

void CsrBtSdRegisterDiServiceRecordV13(CsrSchedQid apphandle,
                                    CsrUint16  vendor,
                                    CsrUint16  productId,
                                    CsrUint16  version,
                                    CsrBool    primaryRecord,
                                    CsrUint16  vendorIdSource,
                                    CsrUint8  *serviceString_p,
                                    CsrUint16  serviceStringLen,
                                    CsrUint8  *clientExecutableUrl_p,
                                    CsrUint16  clientExecutableUrlLen,
                                    CsrUint8  *documentationUrl_p,
                                    CsrUint16  documentationUrlLen)
{
    CsrUint8  *data_p;
    CsrUint16 dataLen;

    /* Just for information: The UUID used for the DI service record
     * is called PNP_INFORMATION_UUID according to the Spec! */
    CsrUint16 uuid = CSR_BT_PNP_INFORMATION_UUID, byteIndex = 0;

    CsrUint8  *sciData_p;
    CsrUint16  sciDataLen;

    CsrUint8  *specificationIdData_p, *vendorIdData_p, *productIdData_p, *versionData_p, *primaryRecordData_p, *vendorIdSourceData_p;
    CsrUint8  *serviceStringData_p = NULL, *clientExecutableUrlData_p = NULL, *documentationUrlData_p = NULL;

    CsrUint16  specificationIdDataLen, vendorIdDataLen, productIdDataLen, versionDataLen, primaryRecordDataLen, vendorIdSourceDataLen, browseGroupListDataLen;
    CsrUint16  serviceStringDataLen = 0, clientExecutableUrlDataLen = 0, documentationUrlDataLen = 0;

    CsrBtUtilSdpCreateServiceClassIdList(&sciData_p, &sciDataLen, 1, &uuid);

    /* browsegrouplist len  */
    browseGroupListDataLen = (CsrUint16)(sizeof(serviceRecordBrowseGroup));

    /* Insert specification version 1.3 */
    CsrBtUtilSdpCreateUint16Uint16(CSR_BT_SPECIFICATION_ID_ATTRIBUTE_IDENTIFIER, 0x0103, &specificationIdData_p, &specificationIdDataLen);

    /* Vendor ID for CSR=10, found on bluetooth.org */
    CsrBtUtilSdpCreateUint16Uint16(CSR_BT_VENDOR_ID_ATTRIBUTE_IDENTIFIER, vendor, &vendorIdData_p, &vendorIdDataLen);

    /* Product IDs are managed by the vendor */
    CsrBtUtilSdpCreateUint16Uint16(CSR_BT_PRODUCT_ID_ATTRIBUTE_IDENTIFIER, productId, &productIdData_p, &productIdDataLen);

    /* Version is a vendor-assigned field, fx 16.1 */
    CsrBtUtilSdpCreateUint16Uint16(CSR_BT_VERSION_ATTRIBUTE_IDENTIFIER, version, &versionData_p, &versionDataLen);

    /* Primary record */
    CsrBtUtilSdpCreateUint16Boolean(CSR_BT_PRIMARY_RECORD_ATTRIBUTE_IDENTIFIER, primaryRecord, &primaryRecordData_p, &primaryRecordDataLen);

    /* Vendor ID source, Bluetooth SIG = 0x0001 */
    CsrBtUtilSdpCreateUint16Uint16(CSR_BT_VENDOR_ID_SOURCE_ATTRIBUTE_IDENTIFIER, vendorIdSource, &vendorIdSourceData_p, &vendorIdSourceDataLen);

    /* Service description string */
    if ((NULL != serviceString_p) && (0 < serviceStringLen))
    {
        CsrBtUtilSdpCreateUint16String(&serviceStringData_p,
                              &serviceStringDataLen,
                              CSR_BT_SERVICE_DESCRIPTION_ATTRIBUTE_IDENTIFIER,
                              serviceString_p,
                              serviceStringLen,
                              CSR_BT_DATA_ELEMENT_TEXT_STRING_TYPE);
    }

    CsrPmemFree(serviceString_p);

    /* Client Executable URL */
    if ((NULL != clientExecutableUrl_p) && (0 < clientExecutableUrlLen))
    {
        CsrBtUtilSdpCreateUint16String(&clientExecutableUrlData_p,
                                          &clientExecutableUrlDataLen,
                                          CSR_BT_CLIENT_EXECUTABLE_URL_ATTRIBUTE_IDENTIFIER,
                                          clientExecutableUrl_p,
                                          clientExecutableUrlLen,
                                          CSR_BT_DATA_ELEMENT_URL);
    }

    CsrPmemFree(clientExecutableUrl_p);

    /* Documentation URL */
    if ((NULL != documentationUrl_p) && (0 < documentationUrlLen))
    {
        CsrBtUtilSdpCreateUint16String(&documentationUrlData_p,
                                          &documentationUrlDataLen,
                                          CSR_BT_DOCUMENTATION_URL_ATTRIBUTE_IDENTIFIER,
                                          documentationUrl_p,
                                          documentationUrlLen,
                                          CSR_BT_DATA_ELEMENT_URL);
    }

    CsrPmemFree(documentationUrl_p);

    /* Mandatory elements length */
    dataLen  = sciDataLen + specificationIdDataLen + vendorIdDataLen + productIdDataLen + versionDataLen + primaryRecordDataLen + vendorIdSourceDataLen + browseGroupListDataLen;
    /* Optional elements length */
    dataLen  = dataLen + serviceStringDataLen + clientExecutableUrlDataLen + documentationUrlDataLen;

    data_p = CsrPmemAlloc(dataLen);

    CsrMemCpy(data_p, sciData_p, sciDataLen);
    CsrPmemFree(sciData_p);
    byteIndex += sciDataLen;

    CsrMemCpy(data_p + byteIndex, serviceRecordBrowseGroup, browseGroupListDataLen);
    byteIndex += browseGroupListDataLen;

    CsrMemCpy(data_p + byteIndex, specificationIdData_p, specificationIdDataLen);
    CsrPmemFree(specificationIdData_p);
    byteIndex += specificationIdDataLen;

    CsrMemCpy(data_p + byteIndex, vendorIdData_p, vendorIdDataLen);
    CsrPmemFree(vendorIdData_p);
    byteIndex += vendorIdDataLen;

    CsrMemCpy(data_p + byteIndex, productIdData_p, productIdDataLen);
    CsrPmemFree(productIdData_p);
    byteIndex += productIdDataLen;

    CsrMemCpy(data_p + byteIndex, versionData_p, versionDataLen);
    CsrPmemFree(versionData_p);
    byteIndex += versionDataLen;

    CsrMemCpy(data_p + byteIndex, primaryRecordData_p, primaryRecordDataLen);
    CsrPmemFree(primaryRecordData_p);
    byteIndex += primaryRecordDataLen;

    CsrMemCpy(data_p + byteIndex, vendorIdSourceData_p, vendorIdSourceDataLen);
    CsrPmemFree(vendorIdSourceData_p);
    byteIndex += vendorIdSourceDataLen;

    if (clientExecutableUrlData_p)
    {
        CsrMemCpy(data_p + byteIndex, clientExecutableUrlData_p, clientExecutableUrlDataLen);
        byteIndex += clientExecutableUrlDataLen;
    }
    CsrPmemFree(clientExecutableUrlData_p);
    if (serviceStringData_p)
    {
        CsrMemCpy(data_p + byteIndex, serviceStringData_p, serviceStringDataLen);
        byteIndex += serviceStringDataLen;
    }
    CsrPmemFree(serviceStringData_p);
    if (documentationUrlData_p)
    {
        CsrMemCpy(data_p + byteIndex, documentationUrlData_p, documentationUrlDataLen);
        byteIndex += documentationUrlDataLen;
    }
    CsrPmemFree(documentationUrlData_p);

    CsrBtSdRegisterServiceRecordReqSend(apphandle, 0, dataLen, data_p);
}

void CsrBtSdReadDiServiceRecordV13(CsrSchedQid apphandle, CsrBtDeviceAddr deviceAddr)
{
    CsrUint16  newIndex;

    CmnCsrBtLinkedListStruct *bll_p = NULL;

    bll_p = CsrBtUtilSdrCreateServiceHandleEntryFromUuid32(bll_p, CSR_BT_PNP_INFORMATION_UUID, &newIndex);

    CsrBtUtilSdrCreateAndInsertAttribute(bll_p, newIndex, CSR_BT_SPECIFICATION_ID_ATTRIBUTE_IDENTIFIER, NULL, 0);
    CsrBtUtilSdrCreateAndInsertAttribute(bll_p, newIndex, CSR_BT_VENDOR_ID_ATTRIBUTE_IDENTIFIER, NULL, 0);
    CsrBtUtilSdrCreateAndInsertAttribute(bll_p, newIndex, CSR_BT_PRODUCT_ID_ATTRIBUTE_IDENTIFIER, NULL, 0);
    CsrBtUtilSdrCreateAndInsertAttribute(bll_p, newIndex, CSR_BT_VERSION_ATTRIBUTE_IDENTIFIER, NULL, 0);
    CsrBtUtilSdrCreateAndInsertAttribute(bll_p, newIndex, CSR_BT_PRIMARY_RECORD_ATTRIBUTE_IDENTIFIER, NULL, 0);
    CsrBtUtilSdrCreateAndInsertAttribute(bll_p, newIndex, CSR_BT_VENDOR_ID_SOURCE_ATTRIBUTE_IDENTIFIER, NULL, 0);

    /* Now starting on the optional elements */
    CsrBtUtilSdrCreateAndInsertAttribute(bll_p, newIndex, CSR_BT_SERVICE_DESCRIPTION_ATTRIBUTE_IDENTIFIER, NULL, 0);
    CsrBtUtilSdrCreateAndInsertAttribute(bll_p, newIndex, CSR_BT_CLIENT_EXECUTABLE_URL_ATTRIBUTE_IDENTIFIER, NULL, 0);
    CsrBtUtilSdrCreateAndInsertAttribute(bll_p, newIndex, CSR_BT_DOCUMENTATION_URL_ATTRIBUTE_IDENTIFIER, NULL, 0);
    CsrBtSdReadServiceRecordReqSend(apphandle, deviceAddr, 0, bll_p->dataLen, bll_p->data);

    CsrPmemFree(bll_p);
}

void CsrBtSdInitDiServiceRecordV13Struct(CsrBtSdDiServiceRecordV13Struct *v13)
{
    v13->specificationIdValid = FALSE;
    v13->specificationIdValue = 0;

    v13->vendorIdValid = FALSE;
    v13->vendorIdValue = 0;

    v13->productIdValid = FALSE;
    v13->productIdValue = 0;

    v13->versionValid = FALSE;
    v13->versionValue = 0;

    v13->primaryRecordValid = FALSE;
    v13->primaryRecordValue = FALSE;

    v13->vendorIdSourceValid = FALSE;
    v13->vendorIdSourceValue = 0;

    v13->clientExecutableUrlValid    = FALSE;
    v13->clientExecutableUrlValue    = NULL;
    v13->clientExecutableUrlValueLen = 0;

    v13->serviceDescriptionValid    = FALSE;
    v13->serviceDescriptionValue    = NULL;
    v13->serviceDescriptionValueLen = 0;

    v13->documentationUrlValid    = FALSE;
    v13->documentationUrlValue    = NULL;
    v13->documentationUrlValueLen = 0;
}

void CsrBtSdExtractDiServiceRecordV13Data(CsrUint8 *data_p,
                                     CsrUint16 dataLen,
                                     CsrBtSdDiServiceRecordV13Struct *v13)
{
    CsrUint8 *string;
    CsrUint16 stringLen;

    CsrUint32 returnValue;

    CmnCsrBtLinkedListStruct *smurf = CsrBtUtilBllCreateNewEntry(NULL, data_p, dataLen);

    CsrBtSdInitDiServiceRecordV13Struct(v13);

    if (TRUE == (v13->specificationIdValid = CsrBtUtilSdrGetUintAttributeDataValueFromAttributeUuid(smurf, 0, CSR_BT_SPECIFICATION_ID_ATTRIBUTE_IDENTIFIER, &returnValue)))
    {
        v13->specificationIdValue = (CsrUint16) returnValue;
    }
    if (TRUE == (v13->vendorIdValid = CsrBtUtilSdrGetUintAttributeDataValueFromAttributeUuid(smurf, 0, CSR_BT_VENDOR_ID_ATTRIBUTE_IDENTIFIER, &returnValue)))
    {
        v13->vendorIdValue = (CsrUint16) returnValue;
    }
    if (TRUE == (v13->productIdValid = CsrBtUtilSdrGetUintAttributeDataValueFromAttributeUuid(smurf, 0, CSR_BT_PRODUCT_ID_ATTRIBUTE_IDENTIFIER, &returnValue)))
    {
        v13->productIdValue = (CsrUint16) returnValue;
    }
    if (TRUE == (v13->versionValid = CsrBtUtilSdrGetUintAttributeDataValueFromAttributeUuid(smurf, 0, CSR_BT_VERSION_ATTRIBUTE_IDENTIFIER, &returnValue)))
    {
        v13->versionValue = (CsrUint16) returnValue;
    }
    if (TRUE == (v13->primaryRecordValid = CsrBtUtilSdrGetUintAttributeDataValueFromAttributeUuid(smurf, 0, CSR_BT_PRIMARY_RECORD_ATTRIBUTE_IDENTIFIER, &returnValue)))
    {
        if (0 != returnValue)
        {
            v13->primaryRecordValue = TRUE;
        }
    }
    if (TRUE == (v13->vendorIdSourceValid = CsrBtUtilSdrGetUintAttributeDataValueFromAttributeUuid(smurf, 0, CSR_BT_VENDOR_ID_SOURCE_ATTRIBUTE_IDENTIFIER, &returnValue)))
    {
        v13->vendorIdSourceValue = (CsrUint16) returnValue;
    }

    if (TRUE == (v13->serviceDescriptionValid = CsrBtUtilSdrGetStringAttributeFromAttributeUuid(smurf, 0, CSR_BT_SERVICE_DESCRIPTION_ATTRIBUTE_IDENTIFIER, &string, &stringLen)))
    {
        v13->serviceDescriptionValue    = string;
        v13->serviceDescriptionValueLen = stringLen;
    }
    if (TRUE == (v13->clientExecutableUrlValid = CsrBtUtilSdrGetStringAttributeFromAttributeUuid(smurf, 0, CSR_BT_CLIENT_EXECUTABLE_URL_ATTRIBUTE_IDENTIFIER, &string, &stringLen)))
    {
        v13->clientExecutableUrlValue    = string;
        v13->clientExecutableUrlValueLen = stringLen;
    }
    if (TRUE == (v13->documentationUrlValid = CsrBtUtilSdrGetStringAttributeFromAttributeUuid(smurf, 0, CSR_BT_DOCUMENTATION_URL_ATTRIBUTE_IDENTIFIER, &string, &stringLen)))
    {
        v13->documentationUrlValue    = string;
        v13->documentationUrlValueLen = stringLen;
    }

    CsrPmemFree(smurf);
}
#endif /* EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE*/
