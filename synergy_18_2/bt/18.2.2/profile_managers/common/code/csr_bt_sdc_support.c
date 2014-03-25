/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_pmem.h"
#include "csr_bt_util.h"
#include "csr_bt_sdc_support.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"

void CsrBtSdcGetServiceName(CsrUint8 * nameDestination, CsrUint16 nameDestinationLength, CsrUint16 attributeListLength, CsrUint8 * attributeList)
{
    CsrUint32 dummySize, dummyIndex;

    CsrBtSdcGetTextStringDirect((CsrUint8 *)nameDestination, nameDestinationLength, attributeListLength, attributeList, CSR_BT_SERVICE_NAME_ATTRIBUTE_IDENTIFIER, &dummySize, &dummyIndex);
}

CsrUint8 CsrBtReturnSizeDescriptorValue(CsrUint8 descriptorValue)
{
    CsrUint8 sizeDescriptor = (CsrUint8)(descriptorValue & CSR_BT_DATA_ELEMENT_SIZE_DESCRIPTOR_MASK);

    switch(sizeDescriptor)
    {
        case 0:
        case 5:
            return 1;
        case 1:
        case 6:
            return 2;
        case 2:
        case 7:
            return 4;
        case 3:
            return 8;
        case 4:
            return 16;
        default:
            return 0;
    }
}

static CsrBool returnDataElementSize(CsrUint16 attributeListLength, CsrUint8 * attributeList, CsrUint8 type, int index, CsrUint32 *length, CsrUint8 *addByte)
{
    CsrUint8  addValue        = 0;
    CsrUint32 size            = *length;

    switch (type)
    {
        case CSR_BT_DATA_ELEMENT_NIL_TYPE:
        {
            size        = 0;
            break;
        }
        case CSR_BT_DATA_ELEMENT_UNSIGNED_INTEGER_TYPE:
        case CSR_BT_DATA_ELEMENT_SIGNED_INTEGER_TYPE:
        {
            if (size == 0)
                return FALSE;
            break;
        }
        case CSR_BT_DATA_ELEMENT_UUID_TYPE:
        {
            if( (size !=2) && (size !=4) && (size !=6) && (size !=16))
            {
                return FALSE;
            }
            break;
        }

        case CSR_BT_DATA_ELEMENT_BOOLEAN_TYPE:
        {
            if (size != 1)
                return FALSE;
            break;
        }
        case CSR_BT_DATA_ELEMENT_TEXT_STRING_TYPE:
        case CSR_BT_DATA_ELEMENT_SEQUENCE_TYPE:
        case CSR_BT_DATA_ELEMENT_ALTERNATIVE_TYPE:
        case CSR_BT_DATA_ELEMENT_URL:
        {
            switch(size)
            {
                case 1:
                {
                    if ((index) < attributeListLength)
                    {
                        size        = attributeList[index];
                        addValue    = 1;
                    }
                    else
                        return FALSE;
                    break;
                }
                case 2:
                {
                    if ((index + 1) < attributeListLength)
                    {
                        size        = ((attributeList[index] << 8) | attributeList[index + 1]);
                        addValue    = 2;
                    }
                    else
                        return FALSE;
                    break;
                }
                case 4:
                {
                    if ((index + 3) < attributeListLength)
                    {
                        size        = ( (attributeList[index] << 24) | (attributeList[index + 1] << 16) |
                                        (attributeList[index + 2] << 8)  | (attributeList[index + 3]));
                        addValue    = 4;
                    }
                    else
                        return FALSE;
                    break;
                }
                default :
                    return FALSE;
            }
            break;
        }
        default:
            return FALSE;
    }
    *length        = size;
    *addByte    = addValue;
    return TRUE;
}

CsrBool CsrBtSdcGetAttribute(CsrUint16 attributeListLength, CsrUint8 * attributeList, CsrUint16 theAttribute, CsrUint8 *theType, CsrUint32 *theSize, CsrUint32 * theIndex)
{
    int index            = 0;

    while( index < attributeListLength)
    {
        if ((attributeList[index] & CSR_BT_DATA_ELEMENT_TYPE_DESCRIPTOR_MASK) == CSR_BT_DATA_ELEMENT_UNSIGNED_INTEGER_TYPE)
        {
            if ((index + 4) < attributeListLength)
            {
                CsrUint8     type;
                CsrUint32 size;
                CsrUint8  addValue        = 0;
                CsrUint16 attributeValue = 0x0000;

                attributeValue    = (CsrUint16)((attributeList[index + 1] << 8) | (attributeList[index + 2]));
                type            = (CsrUint8)(attributeList[index + 3] & CSR_BT_DATA_ELEMENT_TYPE_DESCRIPTOR_MASK);
                size            = CsrBtReturnSizeDescriptorValue(attributeList[index + 3]);

                if(!returnDataElementSize(attributeListLength, attributeList, type, (index + 4), &size, &addValue))
                    return FALSE;

                if (attributeValue == theAttribute)
                {
                    *theType    = type;
                    *theSize    = size;
                    *theIndex    = index  + 4 + addValue;
                    return TRUE;
                }
                index = index + size + addValue + 4;
            }
            else
            {
                return FALSE;
            }
        }
        else if ((attributeList[index] & CSR_BT_DATA_ELEMENT_TYPE_DESCRIPTOR_MASK) == CSR_BT_DATA_ELEMENT_SEQUENCE_TYPE)
        {
            CsrUint32        size;

            size            = CsrBtReturnSizeDescriptorValue(attributeList[index]);
            index = index + size + 1;
        }
        else
        {
            return FALSE;
        }
    }
    return FALSE;
}

CsrBool CsrBtSdcGetTypeDescriptorInsideList(CsrUint16 attributeListLength, CsrUint8 * attributeList, CsrUint32 startIndex, CsrUint8 *descriptorType, CsrUint32 *descriptorLength, CsrUint32  *numOfBytesRead)
{
    if (startIndex < attributeListLength)
    {
        CsrUint32    size;
        CsrUint8        type, addValue = 0;

        type    = (CsrUint8)(attributeList[startIndex] & CSR_BT_DATA_ELEMENT_TYPE_DESCRIPTOR_MASK);
        size    = CsrBtReturnSizeDescriptorValue(attributeList[startIndex]);

        if (returnDataElementSize(attributeListLength, attributeList, type, startIndex + 1, &size, &addValue))
        {
            *descriptorType        = type;
            *descriptorLength    = size;
            *numOfBytesRead        = 1 + addValue;
            return TRUE;
        }
    }
    return FALSE;
}

CsrBool CsrBtSdcGetTextStringDirect(CsrUint8 * nameDestination, CsrUint16 nameDestinationLength, CsrUint16 attributeListLength, CsrUint8 * attributeList, CsrUint16 attributeId, CsrUint32 *theSize, CsrUint32 * theIndex)
{
    CsrUint8 type;

    nameDestination[0] = '\0';

    if(CsrBtSdcGetAttribute(attributeListLength, attributeList, attributeId, &type, theSize, theIndex))
    {
        if (type == CSR_BT_DATA_ELEMENT_TEXT_STRING_TYPE)
        {
            CsrUint32 size    = *theSize;
            CsrUint32 index    = *theIndex;

            if (size > nameDestinationLength)
                size = nameDestinationLength;

            if ((index + size - 1) < attributeListLength)
            {
                CsrMemCpy(nameDestination, &attributeList[index], size);
                CsrUtf8StrTruncate((CsrUtf8String *)nameDestination, size);
                return TRUE;
            }
        }
    }
    return FALSE;
}

CsrBool CsrBtSdcGetUint16Direct(CsrUint16 attributeListLength, CsrUint8 * attributeList, CsrUint16 attributeId, CsrUint16 *theValue)
{
    CsrUint8        type;
    CsrUint32    size, index;

    if(CsrBtSdcGetAttribute(attributeListLength, attributeList, attributeId, &type, &size, &index))
    {
        if (type == CSR_BT_DATA_ELEMENT_SEQUENCE_TYPE)
        {
            type        = (CsrUint8)(attributeList[index] & CSR_BT_DATA_ELEMENT_TYPE_DESCRIPTOR_MASK);
            index ++;
        }
        if (type == CSR_BT_DATA_ELEMENT_UNSIGNED_INTEGER_TYPE && index + 1 < attributeListLength)
        {
            *theValue = (CsrUint16) ((attributeList[index] << 8) | (attributeList[index + 1]));
            return TRUE;
        }
    }
    return FALSE;
}

CsrBool CsrBtSdcGetUint8Direct(CsrUint16 attributeListLength, CsrUint8 * attributeList, CsrUint16 attributeId, CsrUint8 *theValue)
{
    CsrUint8        type;
    CsrUint32    size, index;
    if(CsrBtSdcGetAttribute(attributeListLength, attributeList, attributeId, &type, &size, &index))
    {
        if (type == CSR_BT_DATA_ELEMENT_SEQUENCE_TYPE)
        {
            type        = (CsrUint8)(attributeList[index] & CSR_BT_DATA_ELEMENT_TYPE_DESCRIPTOR_MASK);
            index ++;
        }

        if (type == CSR_BT_DATA_ELEMENT_UNSIGNED_INTEGER_TYPE && index < attributeListLength)
        {
            *theValue = attributeList[index];
            return TRUE;
        }
    }
    return FALSE;
}

CsrBool CsrBtSdcGetBoolDirect(CsrUint16 attributeListLength, CsrUint8 * attributeList, CsrUint16 attributeId, CsrUint8 *theValue)
{
    CsrUint8        type;
    CsrUint32    size, index;
    if(CsrBtSdcGetAttribute(attributeListLength, attributeList, attributeId, &type, &size, &index))
    {
        if (type == CSR_BT_DATA_ELEMENT_SEQUENCE_TYPE)
        {
            type        = (CsrUint8)(attributeList[index] & CSR_BT_DATA_ELEMENT_TYPE_DESCRIPTOR_MASK);
            index ++;
        }

        if (type == CSR_BT_DATA_ELEMENT_BOOLEAN_TYPE && index < attributeListLength)
        {
            *theValue = attributeList[index];
            return TRUE;
        }
    }
    return FALSE;
}

CsrBool CsrBtSdcGetUint32Direct(CsrUint16 attributeListLength, CsrUint8 * attributeList, CsrUint16 attributeId, CsrUint32 *theValue)
{
    CsrUint8        type;
    CsrUint32    size, index;
    if(CsrBtSdcGetAttribute(attributeListLength, attributeList, attributeId, &type, &size, &index))
    {
        if (type == CSR_BT_DATA_ELEMENT_SEQUENCE_TYPE)
        {
            type        = (CsrUint8)(attributeList[index] & CSR_BT_DATA_ELEMENT_TYPE_DESCRIPTOR_MASK);
            index ++;
        }
        if (type == CSR_BT_DATA_ELEMENT_UNSIGNED_INTEGER_TYPE && index + 3 < attributeListLength)
        {
            *theValue = ((attributeList[index] << 24) | (attributeList[index + 1] << 16) | (attributeList[index + 2] << 8) | (attributeList[index + 3]));
            return TRUE;
        }
    }
    return FALSE;
}

CsrBool CsrBtSdcGetUint64ByteStringDirect(CsrUint16 attributeListLength, CsrUint8 * attributeList, CsrUint16 attributeId, CsrUint8 *byteString)
{
    CsrUint8        type;
    CsrUint32    size, index;
    if(CsrBtSdcGetAttribute(attributeListLength, attributeList, attributeId, &type, &size, &index))
    {
        if (type == CSR_BT_DATA_ELEMENT_SEQUENCE_TYPE)
        {
            type        = (CsrUint8)(attributeList[index] & CSR_BT_DATA_ELEMENT_TYPE_DESCRIPTOR_MASK);
            index ++;
        }
        if (type == CSR_BT_DATA_ELEMENT_UNSIGNED_INTEGER_TYPE && index + 7 < attributeListLength)
        {
            byteString[0] =  attributeList[index];
            byteString[1] =  attributeList[index + 1];
            byteString[2] =  attributeList[index + 2];
            byteString[3] =  attributeList[index + 3];
            byteString[4] =  attributeList[index + 4];
            byteString[5] =  attributeList[index + 5];
            byteString[6] =  attributeList[index + 6];
            byteString[7] =  attributeList[index + 7];
            return TRUE;
        }
    }
    return FALSE;
}

CsrBool CsrBtSdcGetUint128ByteStringDirect(CsrUint16 attributeListLength, CsrUint8 * attributeList, CsrUint16 attributeId, CsrUint8 *byteString)
{
    CsrUint8        type;
    CsrUint32    size, index;
    if(CsrBtSdcGetAttribute(attributeListLength, attributeList, attributeId, &type, &size, &index))
    {
        if (type == CSR_BT_DATA_ELEMENT_SEQUENCE_TYPE)
        {
            type        = (CsrUint8)(attributeList[index] & CSR_BT_DATA_ELEMENT_TYPE_DESCRIPTOR_MASK);
            index ++;
        }
        if (type == CSR_BT_DATA_ELEMENT_UNSIGNED_INTEGER_TYPE && index + 15 < attributeListLength)
        {
            byteString[0]  =  attributeList[index];
            byteString[1]  =  attributeList[index + 1];
            byteString[2]  =  attributeList[index + 2];
            byteString[3]  =  attributeList[index + 3];
            byteString[4]  =  attributeList[index + 4];
            byteString[5]  =  attributeList[index + 5];
            byteString[6]  =  attributeList[index + 6];
            byteString[7]  =  attributeList[index + 7];
            byteString[8]  =  attributeList[index + 8];
            byteString[9]  =  attributeList[index + 9];
            byteString[10] =  attributeList[index + 10];
            byteString[11] =  attributeList[index + 11];
            byteString[12] =  attributeList[index + 12];
            byteString[13] =  attributeList[index + 13];
            byteString[14] =  attributeList[index + 14];
            byteString[15] =  attributeList[index + 15];
            return TRUE;
        }
    }
    return FALSE;
}

CsrBool CsrBtSdcGetUint16Value(CsrUint16 attributeListLength, CsrUint8 * attributeList, CsrUint32 index, CsrUint16 *returnValue)
{
    if (index + 2 < attributeListLength)
    {
        *returnValue = (CsrUint16)((attributeList[index + 1] << 8) | (attributeList[index + 2]));
        return TRUE;
    }
    return FALSE;
}

CsrBool CsrBtSdcGetUint8Value(CsrUint16 attributeListLength, CsrUint8 * attributeList, CsrUint32 index, CsrUint8 *returnValue)
{
    if (index + 1 < attributeListLength)
    {
        *returnValue = attributeList[index + 1];
        return TRUE;
    }
    return FALSE;
}

CsrBool CsrBtGetNumOfLanguageBaseAttributeId(CsrUint16 attributeListLength, CsrUint8 * attributeList, CsrUint32 *startIndex, CsrUint16 *numOfAttributeIdElements)
{
    CsrUint8        type;
    CsrUint32    size, index;


    *numOfAttributeIdElements = 0;

    if(CsrBtSdcGetAttribute(attributeListLength, attributeList, CSR_BT_LANGUAGE_BASE_ATTRIBUTE_ID_LIST_ATTRIBUTE_IDENTIFIER, &type, &size, &index))
    {
        if (type == CSR_BT_DATA_ELEMENT_SEQUENCE_TYPE && size == 9)
        {
            *numOfAttributeIdElements = 1;
            *startIndex                  = index;
            return TRUE;
        }
        else
        {
            CsrUint32 listLength, readBytes = 0, read = 0;;

            listLength        = size;

            *startIndex        = index + (CsrBtReturnSizeDescriptorValue(attributeList[index])) + 1;

            while (read < listLength)
            {
                if(CsrBtSdcGetTypeDescriptorInsideList(attributeListLength, attributeList, index + read, &type, &size, &readBytes))
                {
                    read += (readBytes + size);
                    (*numOfAttributeIdElements)++;
                }
                else
                    return FALSE;
            }
            if (*numOfAttributeIdElements > 0)
                return TRUE;
            else
                return FALSE;
        }
    }
    return FALSE;
}

CsrBool CsrBtGetLanguageBaseAttributeIdElement(CsrUint16 attributeListLength, CsrUint8 * attributeList, CsrUint32 index, CsrBtLanguageElement *languageElement, CsrUint32 *nextIndex)
{
    *nextIndex = 0;

    if(CsrBtSdcGetUint16Value(attributeListLength, attributeList, index, &languageElement->nationalLanguageId))
    {
        index += 3;
        if(CsrBtSdcGetUint16Value(attributeListLength, attributeList, index, &languageElement->characterEncoding))
        {
            index += 3;
            if(CsrBtSdcGetUint16Value(attributeListLength, attributeList, index, &languageElement->attributeId))
            {
                index += 3;
                if ( index + 1 < attributeListLength)
                {
                    CsrUint8 type;

                    type = (CsrUint8)(attributeList[index] & CSR_BT_DATA_ELEMENT_TYPE_DESCRIPTOR_MASK);

                    if ( type == CSR_BT_DATA_ELEMENT_SEQUENCE_TYPE)
                    {
                        CsrUint8        addValue;
                        CsrUint32    size;

                        addValue    = 0;
                        size        = CsrBtReturnSizeDescriptorValue(attributeList[index]);

                        if (returnDataElementSize(attributeListLength, attributeList, type, (index + 1), &size, &addValue))
                        {
                            *nextIndex = index + addValue + 1;
                        }
                    }
                }
                return TRUE;
            }
        }
    }
    return FALSE;
}


CsrBool CsrBtSdcFindServiceClassUuid(CsrUint16 attributeListLength, CsrUint8 * attributeList, CsrBtUuid32 serviceClassUuid)
{
    CsrUint8            type;
    CsrUint32        size, index;

    if(CsrBtSdcGetAttribute(attributeListLength, attributeList, CSR_BT_SERVICE_CLASS_ID_LIST, &type, &size, &index))
    {
        CsrUint32    listLength, readBytes = 0, read = 0;

        listLength        = size;
        while (read < listLength)
        {
            if(CsrBtSdcGetTypeDescriptorInsideList(attributeListLength, attributeList, index, &type, &size, &readBytes))
            {
                CsrUint16 value;
                if (type != CSR_BT_DATA_ELEMENT_SEQUENCE_TYPE)
                    readBytes +=  size;


                if (type == CSR_BT_DATA_ELEMENT_UUID_TYPE)
                {
                    CsrUint32 newIndex = index;

                    if (size != 2)
                        newIndex += 2;

                    if (CsrBtSdcGetUint16Value(attributeListLength, attributeList, newIndex, &value))
                    {
                        if (value == serviceClassUuid)
                            return TRUE;
                    }
                }
                index    += readBytes;
                read    += readBytes;
            }
            else
                return FALSE;
        }
    }
    return FALSE;
}

CsrBool CsrBtSdcIndexOfUuid(CsrUint16 attributeListLength, CsrUint8 * attributeList, CsrBtUuid32 serviceClassUuid, CsrUint32 *uuidIndex)
{
    CsrUint8 type;
    CsrUint32 size, index = 0;
    CsrUint32 listLength, readBytes = 0, read = 0;

    listLength = attributeListLength;
    while (read < listLength)
    {
        if(CsrBtSdcGetTypeDescriptorInsideList(attributeListLength, attributeList, index, &type, &size, &readBytes))
        {
                CsrUint16 value;
                if (type != CSR_BT_DATA_ELEMENT_SEQUENCE_TYPE)
                    readBytes +=  size;


                if (type == CSR_BT_DATA_ELEMENT_UUID_TYPE)
                {
                    CsrUint32 newIndex;
                    newIndex = index;

                    if (size != 2)
                        newIndex += 2;

                    if (CsrBtSdcGetUint16Value(attributeListLength, attributeList, newIndex, &value))
                    {
                        if (value == serviceClassUuid)
                        {
                            *uuidIndex = newIndex;
                            return TRUE;
                        }
                    }
                }
                index += readBytes;
                read += readBytes;
            }
            else
                return FALSE;
        }
    return FALSE;
}




static const CsrUint8    baseUuid[12] = {0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB};

CsrBool CsrBtSdcFindServiceClass128Uuid(CsrUint16 attributeListLength, CsrUint8 * attributeList, CsrBtUuid128 serviceClassUuid)
{
    CsrUint8            type;
    CsrUint32        size, index;

    if(CsrBtSdcGetAttribute(attributeListLength, attributeList, CSR_BT_SERVICE_CLASS_ID_LIST, &type, &size, &index))
    {
        CsrUint32    listLength, readBytes = 0, read = 0;

        listLength        = size;
        while (read < listLength)
        {
            if(CsrBtSdcGetTypeDescriptorInsideList(attributeListLength, attributeList, index, &type, &size, &readBytes))
            {
                if (type != CSR_BT_DATA_ELEMENT_SEQUENCE_TYPE)
                    readBytes +=  size;

                if (type == CSR_BT_DATA_ELEMENT_UUID_TYPE)
                {
                    if (size == 16)
                    {
                        if (index + 16 < attributeListLength)
                        {
                            if (!CsrMemCmp(&attributeList[index + 1], serviceClassUuid, 16))
                                return TRUE;
                        }
                        return FALSE;
                    }
                    else
                    {
                        if (!CsrMemCmp((CsrUint8 *)baseUuid, &serviceClassUuid[4], 12))
                        {
                            CsrUint16 value;
                            CsrUint32 newIndex = index;

                            if (size != 2)
                                newIndex += 2;

                            if (CsrBtSdcGetUint16Value(attributeListLength, attributeList, newIndex, &value))
                            {
                                CsrUint32 service = (CsrUint32) ((serviceClassUuid[0] << 24) | serviceClassUuid[1] << 16 | serviceClassUuid[2] << 8 | serviceClassUuid[3]);
                                if (value == service)
                                    return TRUE;
                            }
                        }
                        return FALSE;
                    }
                }
                index    += readBytes;
                read    += readBytes;
            }
            else
                return FALSE;
        }
    }
    return FALSE;
}

CsrUint8 CsrBtSdcGetRfcommServerChannel(CsrUint16 attributeListLength, CsrUint8 * attributeList)
{
    CsrUint8            type;
    CsrUint32        size, index;
    CsrUint8    server    = 0xff;

    if(CsrBtSdcGetAttribute(attributeListLength, attributeList, CSR_BT_PROTOCOL_DESCRIPTOR_LIST_ATTRIBUTE_IDENTIFIER, &type, &size, &index))
    {
        CsrBool        foundUuid = FALSE;
        CsrUint32    listLength, readBytes = 0, read = 0;

        listLength        = size;
        while (read < listLength)
        {
            if(CsrBtSdcGetTypeDescriptorInsideList(attributeListLength, attributeList, index, &type, &size, &readBytes))
            {
                CsrUint16 value;
                if (type != CSR_BT_DATA_ELEMENT_SEQUENCE_TYPE)
                    readBytes +=  size;


                if (type == CSR_BT_DATA_ELEMENT_UUID_TYPE)
                {
                    CsrUint32 newIndex = index;

                    if (size != 2)
                        newIndex += 2;

                    if (CsrBtSdcGetUint16Value(attributeListLength, attributeList, newIndex, &value))
                    {
                        if (value == CSR_BT_RFCOMM_PROTOCOL_UUID16_IDENTIFIER)
                            foundUuid = TRUE;
                    }
                }

                if (foundUuid)
                {
                    if (type == CSR_BT_DATA_ELEMENT_UNSIGNED_INTEGER_TYPE)
                    {
                        if (size == 1)
                        {
                            if(CsrBtSdcGetUint8Value(attributeListLength, attributeList, index, &server))
                                return server;
                            else
                                return 0xff;
                        }
                    }
                }
                index    += readBytes;
                read    += readBytes;
            }
            else
                return server;
        }
    }
    return server;
}
CsrUint8 CsrBtSdcGetExtendedRfcommServerChannel(CsrUint16 attributeListLength, CsrUint8 * attributeList)
{
    CsrUint8         type;
    CsrUint32        size, index;
    CsrUint8    server    = 0xff;

    if(CsrBtSdcGetAttribute(attributeListLength, attributeList, CSR_BT_ADDITIONAL_PROTOCOL_DESCRIPTOR_LIST_ATTRIBUTE_IDENTIFIER, &type, &size, &index))
    {
        CsrBool        foundUuid = FALSE;
        CsrUint32    listLength, readBytes = 0, read = 0;

        listLength        = size;
        while (read < listLength)
        {
            if(CsrBtSdcGetTypeDescriptorInsideList(attributeListLength, attributeList, index, &type, &size, &readBytes))
            {
                CsrUint16 value;
                if (type != CSR_BT_DATA_ELEMENT_SEQUENCE_TYPE)
                    readBytes +=  size;


                if (type == CSR_BT_DATA_ELEMENT_UUID_TYPE)
                {
                    CsrUint32 newIndex = index;

                    if (size != 2)
                        newIndex += 2;

                    if (CsrBtSdcGetUint16Value(attributeListLength, attributeList, newIndex, &value))
                    {
                        if (value == CSR_BT_RFCOMM_PROTOCOL_UUID16_IDENTIFIER)
                            foundUuid = TRUE;
                    }
                }

                if (foundUuid)
                {
                    if (type == CSR_BT_DATA_ELEMENT_UNSIGNED_INTEGER_TYPE)
                    {
                        if (size == 1)
                        {
                            if(CsrBtSdcGetUint8Value(attributeListLength, attributeList, index, &server))
                                return server;
                            else
                                return 0xff;
                        }
                    }
                }
                index    += readBytes;
                read    += readBytes;
            }
            else
                return server;
        }
    }

    return server;
}
/* End new search pattern */

psm_t CsrBtSdcGetL2capPsm(CsrUint16 attributeListLength, CsrUint8 * attributeList)
{
    CsrUint8            type;
    CsrUint32        size, index;
    psm_t            psm    = 0x0000;

    if(CsrBtSdcGetAttribute(attributeListLength, attributeList, CSR_BT_PROTOCOL_DESCRIPTOR_LIST_ATTRIBUTE_IDENTIFIER, &type, &size, &index))
    {
        CsrBool        foundUuid = FALSE;
        CsrUint32    listLength, readBytes = 0, read = 0;
        listLength        = size;
        while (read < listLength)
        {
            if(CsrBtSdcGetTypeDescriptorInsideList(attributeListLength, attributeList, index, &type, &size, &readBytes))
            {
                CsrUint16 value;
                if (type != CSR_BT_DATA_ELEMENT_SEQUENCE_TYPE)
                {
                    readBytes +=  size;
                }

                if (type == CSR_BT_DATA_ELEMENT_UUID_TYPE)
                {
                    CsrUint32 newIndex = index;

                    if (foundUuid)
                    {
                        return 0x0000;
                    }

                    if (size != 2)
                    {
                        newIndex += 2;
                    }

                    if (CsrBtSdcGetUint16Value(attributeListLength, attributeList, newIndex, &value))
                    {
                        if (value == CSR_BT_L2CAP_PROTOCOL_UUID16_IDENTIFIER)
                        {
                            foundUuid = TRUE;
                        }
                    }
                }

                if (foundUuid)
                {
                    if ((type == CSR_BT_DATA_ELEMENT_UNSIGNED_INTEGER_TYPE) && (size == 2))
                    {
                        if(CsrBtSdcGetUint16Value(attributeListLength, attributeList, index, &psm))
                        {
                            return psm;
                        }
                        else
                        {
                            return 0x0000;
                        }
                    }
                }
                index    += readBytes;
                read    += readBytes;
            }
            else
            {
                return psm;
            }
        }
    }
    return psm;
}

