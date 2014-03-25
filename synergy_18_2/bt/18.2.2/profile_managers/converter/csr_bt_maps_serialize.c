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
#include "csr_bt_maps_prim.h"

#ifndef EXCLUDE_CSR_BT_MAPS_MODULE

void CsrBtMapsPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_MAPS_SET_FOLDER_RES
CsrSize CsrBtMapsSetFolderResSizeof(void *msg)
{
    CsrBtMapsSetFolderRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_SET_FOLDER_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_SET_FOLDER_RES
CsrUint8 *CsrBtMapsSetFolderResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsSetFolderRes *primitive;

    primitive = (CsrBtMapsSetFolderRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_SET_FOLDER_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_SET_FOLDER_RES
void *CsrBtMapsSetFolderResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsSetFolderRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsSetFolderRes *) CsrPmemZalloc(sizeof(CsrBtMapsSetFolderRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_SET_FOLDER_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_ACTIVATE_CFM
CsrSize CsrBtMapsActivateCfmSizeof(void *msg)
{
    CsrBtMapsActivateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_MAPS_ACTIVATE_CFM
CsrUint8 *CsrBtMapsActivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsActivateCfm *primitive;

    primitive = (CsrBtMapsActivateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_MAPS_ACTIVATE_CFM
void *CsrBtMapsActivateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsActivateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsActivateCfm *) CsrPmemZalloc(sizeof(CsrBtMapsActivateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_MAPS_SELECT_MAS_INSTANCE_IND
CsrSize CsrBtMapsSelectMasInstanceIndSizeof(void *msg)
{
    CsrBtMapsSelectMasInstanceInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->masInstanceListSize * sizeof(CsrBtMapsMasInstance)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_SELECT_MAS_INSTANCE_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_SELECT_MAS_INSTANCE_IND
CsrUint8 *CsrBtMapsSelectMasInstanceIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsSelectMasInstanceInd *primitive;

    primitive = (CsrBtMapsSelectMasInstanceInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);
    CsrMemCpySer(buffer, length, &primitive->masInstanceList, sizeof(primitive->masInstanceList));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->masInstanceListSize);
    if(primitive->masInstanceList)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->masInstanceList), ((CsrSize) (primitive->masInstanceListSize * sizeof(CsrBtMapsMasInstance))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_SELECT_MAS_INSTANCE_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_SELECT_MAS_INSTANCE_IND
void *CsrBtMapsSelectMasInstanceIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsSelectMasInstanceInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsSelectMasInstanceInd *) CsrPmemZalloc(sizeof(CsrBtMapsSelectMasInstanceInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->masInstanceList), buffer, &offset, ((CsrSize) sizeof(primitive->masInstanceList)));
    CsrUint16Des((CsrUint16 *) &primitive->masInstanceListSize, buffer, &offset);
    if (primitive->masInstanceListSize)
    {
        primitive->masInstanceList = CsrPmemZalloc(((CsrUint32) (primitive->masInstanceListSize * sizeof(CsrBtMapsMasInstance))));
        CsrMemCpyDes(((void *) primitive->masInstanceList), buffer, &offset, ((CsrSize) (primitive->masInstanceListSize * sizeof(CsrBtMapsMasInstance))));
    }
    else
    {
        primitive->masInstanceList = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_SELECT_MAS_INSTANCE_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_SELECT_MAS_INSTANCE_IND
void CsrBtMapsSelectMasInstanceIndSerFree(void *msg)
{
    CsrBtMapsSelectMasInstanceInd *primitive;

    primitive = (CsrBtMapsSelectMasInstanceInd *) msg;

    if(primitive->masInstanceList != NULL)
    {
        CsrPmemFree(primitive->masInstanceList);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_MAPS_SELECT_MAS_INSTANCE_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_HEADER_RES
CsrSize CsrBtMapsGetMessageHeaderResSizeof(void *msg)
{
    CsrBtMapsGetMessageHeaderRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_HEADER_RES
CsrUint8 *CsrBtMapsGetMessageHeaderResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsGetMessageHeaderRes *primitive;

    primitive = (CsrBtMapsGetMessageHeaderRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->fractionDeliver);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_HEADER_RES
void *CsrBtMapsGetMessageHeaderResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsGetMessageHeaderRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsGetMessageHeaderRes *) CsrPmemZalloc(sizeof(CsrBtMapsGetMessageHeaderRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->fractionDeliver, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_HEADER_IND
CsrSize CsrBtMapsGetMessageListingHeaderIndSizeof(void *msg)
{
    CsrBtMapsGetMessageListingHeaderInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrConverterUcs2ByteStrLen((CsrUtf8String*) primitive->folderName) + 2 + CsrUtf8StringSerLen((CsrUtf8String*) primitive->filterPeriodBegin) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->filterPeriodEnd) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->filterRecipient) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->filterOriginator));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_HEADER_IND
CsrUint8 *CsrBtMapsGetMessageListingHeaderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsGetMessageListingHeaderInd *primitive;

    primitive = (CsrBtMapsGetMessageListingHeaderInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);
    CsrMemCpySer(buffer, length, &primitive->folderName, sizeof(primitive->folderName));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxListCount);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->listStartOffset);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->maxSubjectLength);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->parameterMask);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->filterMessageType);
    CsrMemCpySer(buffer, length, &primitive->filterPeriodBegin, sizeof(primitive->filterPeriodBegin));
    CsrMemCpySer(buffer, length, &primitive->filterPeriodEnd, sizeof(primitive->filterPeriodEnd));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->filterReadStatus);
    CsrMemCpySer(buffer, length, &primitive->filterRecipient, sizeof(primitive->filterRecipient));
    CsrMemCpySer(buffer, length, &primitive->filterOriginator, sizeof(primitive->filterOriginator));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->filterPriority);
    if (primitive->folderName)
    {
        CsrMemCpy(((CsrUint8*) &buffer[*length]), ((CsrUint8*) primitive->folderName), CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->folderName) + 2);
        *length += ((CsrUint16) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->folderName) + 2));
    }
    else
    {
        buffer[*length] = '\0';
        buffer[*length + 1] = '\0';
        *length += 2;
    }
    CsrUtf8StringSer(buffer, length, primitive->filterPeriodBegin);
    CsrUtf8StringSer(buffer, length, primitive->filterPeriodEnd);
    CsrUtf8StringSer(buffer, length, primitive->filterRecipient);
    CsrUtf8StringSer(buffer, length, primitive->filterOriginator);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_HEADER_IND
void *CsrBtMapsGetMessageListingHeaderIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsGetMessageListingHeaderInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsGetMessageListingHeaderInd *) CsrPmemZalloc(sizeof(CsrBtMapsGetMessageListingHeaderInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->folderName), buffer, &offset, ((CsrSize) sizeof(primitive->folderName)));
    CsrUint16Des((CsrUint16 *) &primitive->maxListCount, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->listStartOffset, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->maxSubjectLength, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->parameterMask, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->filterMessageType, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->filterPeriodBegin), buffer, &offset, ((CsrSize) sizeof(primitive->filterPeriodBegin)));
    CsrMemCpyDes(((void *) &primitive->filterPeriodEnd), buffer, &offset, ((CsrSize) sizeof(primitive->filterPeriodEnd)));
    CsrUint8Des((CsrUint8 *) &primitive->filterReadStatus, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->filterRecipient), buffer, &offset, ((CsrSize) sizeof(primitive->filterRecipient)));
    CsrMemCpyDes(((void *) &primitive->filterOriginator), buffer, &offset, ((CsrSize) sizeof(primitive->filterOriginator)));
    CsrUint8Des((CsrUint8 *) &primitive->filterPriority, buffer, &offset);
    primitive->folderName = CsrPmemZalloc(CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    CsrMemCpy(((CsrUint8*) primitive->folderName), ((CsrUint8*) &buffer[offset]),CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    offset += ((CsrSize) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->folderName) + 2));
    CsrUtf8StringDes(&primitive->filterPeriodBegin, buffer, &offset);
    CsrUtf8StringDes(&primitive->filterPeriodEnd, buffer, &offset);
    CsrUtf8StringDes(&primitive->filterRecipient, buffer, &offset);
    CsrUtf8StringDes(&primitive->filterOriginator, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_HEADER_IND
void CsrBtMapsGetMessageListingHeaderIndSerFree(void *msg)
{
    CsrBtMapsGetMessageListingHeaderInd *primitive;

    primitive = (CsrBtMapsGetMessageListingHeaderInd *) msg;

    if(primitive->folderName != NULL)
    {
        CsrPmemFree(primitive->folderName);
    }
    if(primitive->filterPeriodBegin != NULL)
    {
        CsrPmemFree(primitive->filterPeriodBegin);
    }
    if(primitive->filterPeriodEnd != NULL)
    {
        CsrPmemFree(primitive->filterPeriodEnd);
    }
    if(primitive->filterRecipient != NULL)
    {
        CsrPmemFree(primitive->filterRecipient);
    }
    if(primitive->filterOriginator != NULL)
    {
        CsrPmemFree(primitive->filterOriginator);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_IND
CsrSize CsrBtMapsGetMessageListingIndSizeof(void *msg)
{
    CsrBtMapsGetMessageListingInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_IND
CsrUint8 *CsrBtMapsGetMessageListingIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsGetMessageListingInd *primitive;

    primitive = (CsrBtMapsGetMessageListingInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->obexResponsePacketLength);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_IND
void *CsrBtMapsGetMessageListingIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsGetMessageListingInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsGetMessageListingInd *) CsrPmemZalloc(sizeof(CsrBtMapsGetMessageListingInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->obexResponsePacketLength, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_NOTIFICATION_REGISTRATION_RES
CsrSize CsrBtMapsNotificationRegistrationResSizeof(void *msg)
{
    CsrBtMapsNotificationRegistrationRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_NOTIFICATION_REGISTRATION_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_NOTIFICATION_REGISTRATION_RES
CsrUint8 *CsrBtMapsNotificationRegistrationResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsNotificationRegistrationRes *primitive;

    primitive = (CsrBtMapsNotificationRegistrationRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_NOTIFICATION_REGISTRATION_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_NOTIFICATION_REGISTRATION_RES
void *CsrBtMapsNotificationRegistrationResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsNotificationRegistrationRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsNotificationRegistrationRes *) CsrPmemZalloc(sizeof(CsrBtMapsNotificationRegistrationRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_NOTIFICATION_REGISTRATION_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_SET_ROOT_FOLDER_IND
CsrSize CsrBtMapsSetRootFolderIndSizeof(void *msg)
{
    CsrBtMapsSetRootFolderInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_SET_ROOT_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_SET_ROOT_FOLDER_IND
CsrUint8 *CsrBtMapsSetRootFolderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsSetRootFolderInd *primitive;

    primitive = (CsrBtMapsSetRootFolderInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_SET_ROOT_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_SET_ROOT_FOLDER_IND
void *CsrBtMapsSetRootFolderIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsSetRootFolderInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsSetRootFolderInd *) CsrPmemZalloc(sizeof(CsrBtMapsSetRootFolderInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_SET_ROOT_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_SET_FOLDER_IND
CsrSize CsrBtMapsSetFolderIndSizeof(void *msg)
{
    CsrBtMapsSetFolderInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrConverterUcs2ByteStrLen((CsrUtf8String*) primitive->folderName) + 2);
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_SET_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_SET_FOLDER_IND
CsrUint8 *CsrBtMapsSetFolderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsSetFolderInd *primitive;

    primitive = (CsrBtMapsSetFolderInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);
    CsrMemCpySer(buffer, length, &primitive->folderName, sizeof(primitive->folderName));
    if (primitive->folderName)
    {
        CsrMemCpy(((CsrUint8*) &buffer[*length]), ((CsrUint8*) primitive->folderName), CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->folderName) + 2);
        *length += ((CsrUint16) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->folderName) + 2));
    }
    else
    {
        buffer[*length] = '\0';
        buffer[*length + 1] = '\0';
        *length += 2;
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_SET_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_SET_FOLDER_IND
void *CsrBtMapsSetFolderIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsSetFolderInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsSetFolderInd *) CsrPmemZalloc(sizeof(CsrBtMapsSetFolderInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->folderName), buffer, &offset, ((CsrSize) sizeof(primitive->folderName)));
    primitive->folderName = CsrPmemZalloc(CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    CsrMemCpy(((CsrUint8*) primitive->folderName), ((CsrUint8*) &buffer[offset]),CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    offset += ((CsrSize) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->folderName) + 2));

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_SET_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_SET_FOLDER_IND
void CsrBtMapsSetFolderIndSerFree(void *msg)
{
    CsrBtMapsSetFolderInd *primitive;

    primitive = (CsrBtMapsSetFolderInd *) msg;

    if(primitive->folderName != NULL)
    {
        CsrPmemFree(primitive->folderName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_MAPS_SET_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_IND
CsrSize CsrBtMapsPushMessageIndSizeof(void *msg)
{
    CsrBtMapsPushMessageInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->bodyLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_IND
CsrUint8 *CsrBtMapsPushMessageIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsPushMessageInd *primitive;

    primitive = (CsrBtMapsPushMessageInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyLength);
    CsrMemCpySer(buffer, length, &primitive->body, sizeof(primitive->body));
    if(primitive->body)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->body), ((CsrSize) (primitive->bodyLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_IND
void *CsrBtMapsPushMessageIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsPushMessageInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsPushMessageInd *) CsrPmemZalloc(sizeof(CsrBtMapsPushMessageInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->body), buffer, &offset, ((CsrSize) sizeof(primitive->body)));
    if (primitive->bodyLength)
    {
        primitive->body = CsrPmemZalloc(((CsrUint32) (primitive->bodyLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->body), buffer, &offset, ((CsrSize) (primitive->bodyLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->body = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_IND
void CsrBtMapsPushMessageIndSerFree(void *msg)
{
    CsrBtMapsPushMessageInd *primitive;

    primitive = (CsrBtMapsPushMessageInd *) msg;

    if(primitive->body != NULL)
    {
        CsrPmemFree(primitive->body);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_NOTIFICATION_REGISTRATION_COMPLETE_IND
CsrSize CsrBtMapsNotificationRegistrationCompleteIndSizeof(void *msg)
{
    CsrBtMapsNotificationRegistrationCompleteInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_NOTIFICATION_REGISTRATION_COMPLETE_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_NOTIFICATION_REGISTRATION_COMPLETE_IND
CsrUint8 *CsrBtMapsNotificationRegistrationCompleteIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsNotificationRegistrationCompleteInd *primitive;

    primitive = (CsrBtMapsNotificationRegistrationCompleteInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_NOTIFICATION_REGISTRATION_COMPLETE_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_NOTIFICATION_REGISTRATION_COMPLETE_IND
void *CsrBtMapsNotificationRegistrationCompleteIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsNotificationRegistrationCompleteInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsNotificationRegistrationCompleteInd *) CsrPmemZalloc(sizeof(CsrBtMapsNotificationRegistrationCompleteInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_NOTIFICATION_REGISTRATION_COMPLETE_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_SELECT_MAS_INSTANCE_RES
CsrSize CsrBtMapsSelectMasInstanceResSizeof(void *msg)
{
    CsrBtMapsSelectMasInstanceRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_SELECT_MAS_INSTANCE_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_SELECT_MAS_INSTANCE_RES
CsrUint8 *CsrBtMapsSelectMasInstanceResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsSelectMasInstanceRes *primitive;

    primitive = (CsrBtMapsSelectMasInstanceRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->proceedWithConnection);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->masInstanceId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_SELECT_MAS_INSTANCE_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_SELECT_MAS_INSTANCE_RES
void *CsrBtMapsSelectMasInstanceResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsSelectMasInstanceRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsSelectMasInstanceRes *) CsrPmemZalloc(sizeof(CsrBtMapsSelectMasInstanceRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->proceedWithConnection, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->masInstanceId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_SELECT_MAS_INSTANCE_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_FOLDER_LISTING_HEADER_RES
CsrSize CsrBtMapsGetFolderListingHeaderResSizeof(void *msg)
{
    CsrBtMapsGetFolderListingHeaderRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_FOLDER_LISTING_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_FOLDER_LISTING_HEADER_RES
CsrUint8 *CsrBtMapsGetFolderListingHeaderResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsGetFolderListingHeaderRes *primitive;

    primitive = (CsrBtMapsGetFolderListingHeaderRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->fullFolderListingSize);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_FOLDER_LISTING_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_FOLDER_LISTING_HEADER_RES
void *CsrBtMapsGetFolderListingHeaderResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsGetFolderListingHeaderRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsGetFolderListingHeaderRes *) CsrPmemZalloc(sizeof(CsrBtMapsGetFolderListingHeaderRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->fullFolderListingSize, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_FOLDER_LISTING_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_HEADER_IND
CsrSize CsrBtMapsPushMessageHeaderIndSizeof(void *msg)
{
    CsrBtMapsPushMessageHeaderInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrConverterUcs2ByteStrLen((CsrUtf8String*) primitive->folderName) + 2);
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_HEADER_IND
CsrUint8 *CsrBtMapsPushMessageHeaderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsPushMessageHeaderInd *primitive;

    primitive = (CsrBtMapsPushMessageHeaderInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);
    CsrMemCpySer(buffer, length, &primitive->folderName, sizeof(primitive->folderName));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->transparent);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->retry);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->charset);
    if (primitive->folderName)
    {
        CsrMemCpy(((CsrUint8*) &buffer[*length]), ((CsrUint8*) primitive->folderName), CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->folderName) + 2);
        *length += ((CsrUint16) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->folderName) + 2));
    }
    else
    {
        buffer[*length] = '\0';
        buffer[*length + 1] = '\0';
        *length += 2;
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_HEADER_IND
void *CsrBtMapsPushMessageHeaderIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsPushMessageHeaderInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsPushMessageHeaderInd *) CsrPmemZalloc(sizeof(CsrBtMapsPushMessageHeaderInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->folderName), buffer, &offset, ((CsrSize) sizeof(primitive->folderName)));
    CsrUint8Des((CsrUint8 *) &primitive->transparent, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->retry, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->charset, buffer, &offset);
    primitive->folderName = CsrPmemZalloc(CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    CsrMemCpy(((CsrUint8*) primitive->folderName), ((CsrUint8*) &buffer[offset]),CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    offset += ((CsrSize) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->folderName) + 2));

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_HEADER_IND
void CsrBtMapsPushMessageHeaderIndSerFree(void *msg)
{
    CsrBtMapsPushMessageHeaderInd *primitive;

    primitive = (CsrBtMapsPushMessageHeaderInd *) msg;

    if(primitive->folderName != NULL)
    {
        CsrPmemFree(primitive->folderName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_HEADER_RES
CsrSize CsrBtMapsGetMessageListingHeaderResSizeof(void *msg)
{
    CsrBtMapsGetMessageListingHeaderRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->mseTime));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_HEADER_RES
CsrUint8 *CsrBtMapsGetMessageListingHeaderResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsGetMessageListingHeaderRes *primitive;

    primitive = (CsrBtMapsGetMessageListingHeaderRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->newMessages);
    CsrMemCpySer(buffer, length, &primitive->mseTime, sizeof(primitive->mseTime));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->fullMessageListingSize);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    CsrUtf8StringSer(buffer, length, primitive->mseTime);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_HEADER_RES
void *CsrBtMapsGetMessageListingHeaderResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsGetMessageListingHeaderRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsGetMessageListingHeaderRes *) CsrPmemZalloc(sizeof(CsrBtMapsGetMessageListingHeaderRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->newMessages, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->mseTime), buffer, &offset, ((CsrSize) sizeof(primitive->mseTime)));
    CsrUint16Des((CsrUint16 *) &primitive->fullMessageListingSize, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    CsrUtf8StringDes(&primitive->mseTime, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_HEADER_RES
void CsrBtMapsGetMessageListingHeaderResSerFree(void *msg)
{
    CsrBtMapsGetMessageListingHeaderRes *primitive;

    primitive = (CsrBtMapsGetMessageListingHeaderRes *) msg;

    if(primitive->mseTime != NULL)
    {
        CsrPmemFree(primitive->mseTime);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_SECURITY_LEVEL_REQ
CsrSize CsrBtMapsSecurityLevelReqSizeof(void *msg)
{
    CsrBtMapsSecurityLevelReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_SECURITY_LEVEL_REQ */

#ifndef EXCLUDE_CSR_BT_MAPS_SECURITY_LEVEL_REQ
CsrUint8 *CsrBtMapsSecurityLevelReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsSecurityLevelReq *primitive;

    primitive = (CsrBtMapsSecurityLevelReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_SECURITY_LEVEL_REQ */

#ifndef EXCLUDE_CSR_BT_MAPS_SECURITY_LEVEL_REQ
void *CsrBtMapsSecurityLevelReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsSecurityLevelReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsSecurityLevelReq *) CsrPmemZalloc(sizeof(CsrBtMapsSecurityLevelReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_SECURITY_LEVEL_REQ */

#ifndef EXCLUDE_CSR_BT_MAPS_SET_ROOT_FOLDER_RES
CsrSize CsrBtMapsSetRootFolderResSizeof(void *msg)
{
    CsrBtMapsSetRootFolderRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_SET_ROOT_FOLDER_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_SET_ROOT_FOLDER_RES
CsrUint8 *CsrBtMapsSetRootFolderResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsSetRootFolderRes *primitive;

    primitive = (CsrBtMapsSetRootFolderRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_SET_ROOT_FOLDER_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_SET_ROOT_FOLDER_RES
void *CsrBtMapsSetRootFolderResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsSetRootFolderRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsSetRootFolderRes *) CsrPmemZalloc(sizeof(CsrBtMapsSetRootFolderRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_SET_ROOT_FOLDER_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_EVENT_NOTIFICATION_CFM
CsrSize CsrBtMapsEventNotificationCfmSizeof(void *msg)
{
    CsrBtMapsEventNotificationCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_EVENT_NOTIFICATION_CFM */

#ifndef EXCLUDE_CSR_BT_MAPS_EVENT_NOTIFICATION_CFM
CsrUint8 *CsrBtMapsEventNotificationCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsEventNotificationCfm *primitive;

    primitive = (CsrBtMapsEventNotificationCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_EVENT_NOTIFICATION_CFM */

#ifndef EXCLUDE_CSR_BT_MAPS_EVENT_NOTIFICATION_CFM
void *CsrBtMapsEventNotificationCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsEventNotificationCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsEventNotificationCfm *) CsrPmemZalloc(sizeof(CsrBtMapsEventNotificationCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_EVENT_NOTIFICATION_CFM */

#ifndef EXCLUDE_CSR_BT_MAPS_SET_BACK_FOLDER_RES
CsrSize CsrBtMapsSetBackFolderResSizeof(void *msg)
{
    CsrBtMapsSetBackFolderRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_SET_BACK_FOLDER_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_SET_BACK_FOLDER_RES
CsrUint8 *CsrBtMapsSetBackFolderResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsSetBackFolderRes *primitive;

    primitive = (CsrBtMapsSetBackFolderRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_SET_BACK_FOLDER_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_SET_BACK_FOLDER_RES
void *CsrBtMapsSetBackFolderResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsSetBackFolderRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsSetBackFolderRes *) CsrPmemZalloc(sizeof(CsrBtMapsSetBackFolderRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_SET_BACK_FOLDER_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_NOTIFICATION_REGISTRATION_IND
CsrSize CsrBtMapsNotificationRegistrationIndSizeof(void *msg)
{
    CsrBtMapsNotificationRegistrationInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_NOTIFICATION_REGISTRATION_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_NOTIFICATION_REGISTRATION_IND
CsrUint8 *CsrBtMapsNotificationRegistrationIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsNotificationRegistrationInd *primitive;

    primitive = (CsrBtMapsNotificationRegistrationInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->notificationStatus);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_NOTIFICATION_REGISTRATION_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_NOTIFICATION_REGISTRATION_IND
void *CsrBtMapsNotificationRegistrationIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsNotificationRegistrationInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsNotificationRegistrationInd *) CsrPmemZalloc(sizeof(CsrBtMapsNotificationRegistrationInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->notificationStatus, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_NOTIFICATION_REGISTRATION_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_DEACTIVATE_REQ
CsrSize CsrBtMapsDeactivateReqSizeof(void *msg)
{
    CsrBtMapsDeactivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_MAPS_DEACTIVATE_REQ
CsrUint8 *CsrBtMapsDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsDeactivateReq *primitive;

    primitive = (CsrBtMapsDeactivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_MAPS_DEACTIVATE_REQ
void *CsrBtMapsDeactivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsDeactivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsDeactivateReq *) CsrPmemZalloc(sizeof(CsrBtMapsDeactivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_MAPS_EVENT_NOTIFICATION_IND
CsrSize CsrBtMapsEventNotificationIndSizeof(void *msg)
{
    CsrBtMapsEventNotificationInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_EVENT_NOTIFICATION_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_EVENT_NOTIFICATION_IND
CsrUint8 *CsrBtMapsEventNotificationIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsEventNotificationInd *primitive;

    primitive = (CsrBtMapsEventNotificationInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->obexResponsePacketLength);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_EVENT_NOTIFICATION_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_EVENT_NOTIFICATION_IND
void *CsrBtMapsEventNotificationIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsEventNotificationInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsEventNotificationInd *) CsrPmemZalloc(sizeof(CsrBtMapsEventNotificationInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->obexResponsePacketLength, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_EVENT_NOTIFICATION_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_HEADER_IND
CsrSize CsrBtMapsGetMessageHeaderIndSizeof(void *msg)
{
    CsrBtMapsGetMessageHeaderInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrConverterUcs2ByteStrLen((CsrUtf8String*) primitive->messageHandle) + 2);
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_HEADER_IND
CsrUint8 *CsrBtMapsGetMessageHeaderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsGetMessageHeaderInd *primitive;

    primitive = (CsrBtMapsGetMessageHeaderInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);
    CsrMemCpySer(buffer, length, &primitive->messageHandle, sizeof(primitive->messageHandle));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->attachment);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->charset);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->fractionRequest);
    if (primitive->messageHandle)
    {
        CsrMemCpy(((CsrUint8*) &buffer[*length]), ((CsrUint8*) primitive->messageHandle), CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->messageHandle) + 2);
        *length += ((CsrUint16) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->messageHandle) + 2));
    }
    else
    {
        buffer[*length] = '\0';
        buffer[*length + 1] = '\0';
        *length += 2;
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_HEADER_IND
void *CsrBtMapsGetMessageHeaderIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsGetMessageHeaderInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsGetMessageHeaderInd *) CsrPmemZalloc(sizeof(CsrBtMapsGetMessageHeaderInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->messageHandle), buffer, &offset, ((CsrSize) sizeof(primitive->messageHandle)));
    CsrUint8Des((CsrUint8 *) &primitive->attachment, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->charset, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->fractionRequest, buffer, &offset);
    primitive->messageHandle = CsrPmemZalloc(CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    CsrMemCpy(((CsrUint8*) primitive->messageHandle), ((CsrUint8*) &buffer[offset]),CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    offset += ((CsrSize) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->messageHandle) + 2));

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_HEADER_IND
void CsrBtMapsGetMessageHeaderIndSerFree(void *msg)
{
    CsrBtMapsGetMessageHeaderInd *primitive;

    primitive = (CsrBtMapsGetMessageHeaderInd *) msg;

    if(primitive->messageHandle != NULL)
    {
        CsrPmemFree(primitive->messageHandle);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_CONNECT_IND
CsrSize CsrBtMapsConnectIndSizeof(void *msg)
{
    CsrBtMapsConnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_CONNECT_IND
CsrUint8 *CsrBtMapsConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsConnectInd *primitive;

    primitive = (CsrBtMapsConnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->length);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->count);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_CONNECT_IND
void *CsrBtMapsConnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsConnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsConnectInd *) CsrPmemZalloc(sizeof(CsrBtMapsConnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->length, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->count, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_SET_MESSAGE_STATUS_RES
CsrSize CsrBtMapsSetMessageStatusResSizeof(void *msg)
{
    CsrBtMapsSetMessageStatusRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_SET_MESSAGE_STATUS_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_SET_MESSAGE_STATUS_RES
CsrUint8 *CsrBtMapsSetMessageStatusResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsSetMessageStatusRes *primitive;

    primitive = (CsrBtMapsSetMessageStatusRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_SET_MESSAGE_STATUS_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_SET_MESSAGE_STATUS_RES
void *CsrBtMapsSetMessageStatusResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsSetMessageStatusRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsSetMessageStatusRes *) CsrPmemZalloc(sizeof(CsrBtMapsSetMessageStatusRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_SET_MESSAGE_STATUS_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_INSTANCE_IDS_CFM
CsrSize CsrBtMapsGetInstanceIdsCfmSizeof(void *msg)
{
    CsrBtMapsGetInstanceIdsCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->instanceIdsListSize * sizeof(CsrSchedQid)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_INSTANCE_IDS_CFM */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_INSTANCE_IDS_CFM
CsrUint8 *CsrBtMapsGetInstanceIdsCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsGetInstanceIdsCfm *primitive;

    primitive = (CsrBtMapsGetInstanceIdsCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->instanceIdsListSize);
    CsrMemCpySer(buffer, length, &primitive->instanceIdsList, sizeof(primitive->instanceIdsList));
    if(primitive->instanceIdsList)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->instanceIdsList), ((CsrSize) (primitive->instanceIdsListSize * sizeof(CsrSchedQid))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_INSTANCE_IDS_CFM */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_INSTANCE_IDS_CFM
void *CsrBtMapsGetInstanceIdsCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsGetInstanceIdsCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsGetInstanceIdsCfm *) CsrPmemZalloc(sizeof(CsrBtMapsGetInstanceIdsCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->instanceIdsListSize, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->instanceIdsList), buffer, &offset, ((CsrSize) sizeof(primitive->instanceIdsList)));
    if (primitive->instanceIdsListSize)
    {
        primitive->instanceIdsList = CsrPmemZalloc(((CsrUint32) (primitive->instanceIdsListSize * sizeof(CsrSchedQid))));
        CsrMemCpyDes(((void *) primitive->instanceIdsList), buffer, &offset, ((CsrSize) (primitive->instanceIdsListSize * sizeof(CsrSchedQid))));
    }
    else
    {
        primitive->instanceIdsList = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_INSTANCE_IDS_CFM */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_INSTANCE_IDS_CFM
void CsrBtMapsGetInstanceIdsCfmSerFree(void *msg)
{
    CsrBtMapsGetInstanceIdsCfm *primitive;

    primitive = (CsrBtMapsGetInstanceIdsCfm *) msg;

    if(primitive->instanceIdsList != NULL)
    {
        CsrPmemFree(primitive->instanceIdsList);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_INSTANCE_IDS_CFM */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_FOLDER_LISTING_IND
CsrSize CsrBtMapsGetFolderListingIndSizeof(void *msg)
{
    CsrBtMapsGetFolderListingInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_FOLDER_LISTING_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_FOLDER_LISTING_IND
CsrUint8 *CsrBtMapsGetFolderListingIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsGetFolderListingInd *primitive;

    primitive = (CsrBtMapsGetFolderListingInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->obexResponsePacketLength);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_FOLDER_LISTING_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_FOLDER_LISTING_IND
void *CsrBtMapsGetFolderListingIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsGetFolderListingInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsGetFolderListingInd *) CsrPmemZalloc(sizeof(CsrBtMapsGetFolderListingInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->obexResponsePacketLength, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_FOLDER_LISTING_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_SET_BACK_FOLDER_IND
CsrSize CsrBtMapsSetBackFolderIndSizeof(void *msg)
{
    CsrBtMapsSetBackFolderInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrConverterUcs2ByteStrLen((CsrUtf8String*) primitive->folderName) + 2);
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_SET_BACK_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_SET_BACK_FOLDER_IND
CsrUint8 *CsrBtMapsSetBackFolderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsSetBackFolderInd *primitive;

    primitive = (CsrBtMapsSetBackFolderInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);
    CsrMemCpySer(buffer, length, &primitive->folderName, sizeof(primitive->folderName));
    if (primitive->folderName)
    {
        CsrMemCpy(((CsrUint8*) &buffer[*length]), ((CsrUint8*) primitive->folderName), CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->folderName) + 2);
        *length += ((CsrUint16) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->folderName) + 2));
    }
    else
    {
        buffer[*length] = '\0';
        buffer[*length + 1] = '\0';
        *length += 2;
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_SET_BACK_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_SET_BACK_FOLDER_IND
void *CsrBtMapsSetBackFolderIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsSetBackFolderInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsSetBackFolderInd *) CsrPmemZalloc(sizeof(CsrBtMapsSetBackFolderInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->folderName), buffer, &offset, ((CsrSize) sizeof(primitive->folderName)));
    primitive->folderName = CsrPmemZalloc(CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    CsrMemCpy(((CsrUint8*) primitive->folderName), ((CsrUint8*) &buffer[offset]),CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    offset += ((CsrSize) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->folderName) + 2));

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_SET_BACK_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_SET_BACK_FOLDER_IND
void CsrBtMapsSetBackFolderIndSerFree(void *msg)
{
    CsrBtMapsSetBackFolderInd *primitive;

    primitive = (CsrBtMapsSetBackFolderInd *) msg;

    if(primitive->folderName != NULL)
    {
        CsrPmemFree(primitive->folderName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_MAPS_SET_BACK_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_IND
CsrSize CsrBtMapsGetMessageIndSizeof(void *msg)
{
    CsrBtMapsGetMessageInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_IND
CsrUint8 *CsrBtMapsGetMessageIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsGetMessageInd *primitive;

    primitive = (CsrBtMapsGetMessageInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->obexResponsePacketLength);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_IND
void *CsrBtMapsGetMessageIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsGetMessageInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsGetMessageInd *) CsrPmemZalloc(sizeof(CsrBtMapsGetMessageInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->obexResponsePacketLength, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_SECURITY_LEVEL_CFM
CsrSize CsrBtMapsSecurityLevelCfmSizeof(void *msg)
{
    CsrBtMapsSecurityLevelCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_SECURITY_LEVEL_CFM */

#ifndef EXCLUDE_CSR_BT_MAPS_SECURITY_LEVEL_CFM
CsrUint8 *CsrBtMapsSecurityLevelCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsSecurityLevelCfm *primitive;

    primitive = (CsrBtMapsSecurityLevelCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_SECURITY_LEVEL_CFM */

#ifndef EXCLUDE_CSR_BT_MAPS_SECURITY_LEVEL_CFM
void *CsrBtMapsSecurityLevelCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsSecurityLevelCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsSecurityLevelCfm *) CsrPmemZalloc(sizeof(CsrBtMapsSecurityLevelCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_SECURITY_LEVEL_CFM */

#ifndef EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_HEADER_RES
CsrSize CsrBtMapsPushMessageHeaderResSizeof(void *msg)
{
    CsrBtMapsPushMessageHeaderRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->messageHandle));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_HEADER_RES
CsrUint8 *CsrBtMapsPushMessageHeaderResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsPushMessageHeaderRes *primitive;

    primitive = (CsrBtMapsPushMessageHeaderRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->messageHandle, sizeof(primitive->messageHandle));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    CsrCharStringSer(buffer, length, primitive->messageHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_HEADER_RES
void *CsrBtMapsPushMessageHeaderResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsPushMessageHeaderRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsPushMessageHeaderRes *) CsrPmemZalloc(sizeof(CsrBtMapsPushMessageHeaderRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->messageHandle), buffer, &offset, ((CsrSize) sizeof(primitive->messageHandle)));
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    CsrCharStringDes(&primitive->messageHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_HEADER_RES
void CsrBtMapsPushMessageHeaderResSerFree(void *msg)
{
    CsrBtMapsPushMessageHeaderRes *primitive;

    primitive = (CsrBtMapsPushMessageHeaderRes *) msg;

    if(primitive->messageHandle != NULL)
    {
        CsrPmemFree(primitive->messageHandle);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_SET_MESSAGE_STATUS_IND
CsrSize CsrBtMapsSetMessageStatusIndSizeof(void *msg)
{
    CsrBtMapsSetMessageStatusInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrConverterUcs2ByteStrLen((CsrUtf8String*) primitive->messageHandle) + 2);
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_SET_MESSAGE_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_SET_MESSAGE_STATUS_IND
CsrUint8 *CsrBtMapsSetMessageStatusIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsSetMessageStatusInd *primitive;

    primitive = (CsrBtMapsSetMessageStatusInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);
    CsrMemCpySer(buffer, length, &primitive->messageHandle, sizeof(primitive->messageHandle));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->statusIndicator);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->statusValue);
    if (primitive->messageHandle)
    {
        CsrMemCpy(((CsrUint8*) &buffer[*length]), ((CsrUint8*) primitive->messageHandle), CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->messageHandle) + 2);
        *length += ((CsrUint16) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->messageHandle) + 2));
    }
    else
    {
        buffer[*length] = '\0';
        buffer[*length + 1] = '\0';
        *length += 2;
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_SET_MESSAGE_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_SET_MESSAGE_STATUS_IND
void *CsrBtMapsSetMessageStatusIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsSetMessageStatusInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsSetMessageStatusInd *) CsrPmemZalloc(sizeof(CsrBtMapsSetMessageStatusInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->messageHandle), buffer, &offset, ((CsrSize) sizeof(primitive->messageHandle)));
    CsrUint8Des((CsrUint8 *) &primitive->statusIndicator, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->statusValue, buffer, &offset);
    primitive->messageHandle = CsrPmemZalloc(CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    CsrMemCpy(((CsrUint8*) primitive->messageHandle), ((CsrUint8*) &buffer[offset]),CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    offset += ((CsrSize) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->messageHandle) + 2));

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_SET_MESSAGE_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_SET_MESSAGE_STATUS_IND
void CsrBtMapsSetMessageStatusIndSerFree(void *msg)
{
    CsrBtMapsSetMessageStatusInd *primitive;

    primitive = (CsrBtMapsSetMessageStatusInd *) msg;

    if(primitive->messageHandle != NULL)
    {
        CsrPmemFree(primitive->messageHandle);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_MAPS_SET_MESSAGE_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_FOLDER_LISTING_HEADER_IND
CsrSize CsrBtMapsGetFolderListingHeaderIndSizeof(void *msg)
{
    CsrBtMapsGetFolderListingHeaderInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_FOLDER_LISTING_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_FOLDER_LISTING_HEADER_IND
CsrUint8 *CsrBtMapsGetFolderListingHeaderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsGetFolderListingHeaderInd *primitive;

    primitive = (CsrBtMapsGetFolderListingHeaderInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxListCount);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->listStartOffset);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_FOLDER_LISTING_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_FOLDER_LISTING_HEADER_IND
void *CsrBtMapsGetFolderListingHeaderIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsGetFolderListingHeaderInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsGetFolderListingHeaderInd *) CsrPmemZalloc(sizeof(CsrBtMapsGetFolderListingHeaderInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxListCount, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->listStartOffset, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_FOLDER_LISTING_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_FOLDER_LISTING_RES
CsrSize CsrBtMapsGetFolderListingResSizeof(void *msg)
{
    CsrBtMapsGetFolderListingRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->bodyLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_FOLDER_LISTING_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_FOLDER_LISTING_RES
CsrUint8 *CsrBtMapsGetFolderListingResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsGetFolderListingRes *primitive;

    primitive = (CsrBtMapsGetFolderListingRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyLength);
    CsrMemCpySer(buffer, length, &primitive->body, sizeof(primitive->body));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    if(primitive->body)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->body), ((CsrSize) (primitive->bodyLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_FOLDER_LISTING_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_FOLDER_LISTING_RES
void *CsrBtMapsGetFolderListingResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsGetFolderListingRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsGetFolderListingRes *) CsrPmemZalloc(sizeof(CsrBtMapsGetFolderListingRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->body), buffer, &offset, ((CsrSize) sizeof(primitive->body)));
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    if (primitive->bodyLength)
    {
        primitive->body = CsrPmemZalloc(((CsrUint32) (primitive->bodyLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->body), buffer, &offset, ((CsrSize) (primitive->bodyLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->body = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_FOLDER_LISTING_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_FOLDER_LISTING_RES
void CsrBtMapsGetFolderListingResSerFree(void *msg)
{
    CsrBtMapsGetFolderListingRes *primitive;

    primitive = (CsrBtMapsGetFolderListingRes *) msg;

    if(primitive->body != NULL)
    {
        CsrPmemFree(primitive->body);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_FOLDER_LISTING_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_RES
CsrSize CsrBtMapsPushMessageResSizeof(void *msg)
{
    CsrBtMapsPushMessageRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->messageHandle));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_RES
CsrUint8 *CsrBtMapsPushMessageResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsPushMessageRes *primitive;

    primitive = (CsrBtMapsPushMessageRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    CsrMemCpySer(buffer, length, &primitive->messageHandle, sizeof(primitive->messageHandle));
    CsrCharStringSer(buffer, length, primitive->messageHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_RES
void *CsrBtMapsPushMessageResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsPushMessageRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsPushMessageRes *) CsrPmemZalloc(sizeof(CsrBtMapsPushMessageRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->messageHandle), buffer, &offset, ((CsrSize) sizeof(primitive->messageHandle)));
    CsrCharStringDes(&primitive->messageHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_RES
void CsrBtMapsPushMessageResSerFree(void *msg)
{
    CsrBtMapsPushMessageRes *primitive;

    primitive = (CsrBtMapsPushMessageRes *) msg;

    if(primitive->messageHandle != NULL)
    {
        CsrPmemFree(primitive->messageHandle);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_CONNECT_RES
CsrSize CsrBtMapsConnectResSizeof(void *msg)
{
    CsrBtMapsConnectRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_CONNECT_RES
CsrUint8 *CsrBtMapsConnectResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsConnectRes *primitive;

    primitive = (CsrBtMapsConnectRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_CONNECT_RES
void *CsrBtMapsConnectResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsConnectRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsConnectRes *) CsrPmemZalloc(sizeof(CsrBtMapsConnectRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_INSTANCE_IDS_REQ
CsrSize CsrBtMapsGetInstanceIdsReqSizeof(void *msg)
{
    CsrBtMapsGetInstanceIdsReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_INSTANCE_IDS_REQ */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_INSTANCE_IDS_REQ
CsrUint8 *CsrBtMapsGetInstanceIdsReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsGetInstanceIdsReq *primitive;

    primitive = (CsrBtMapsGetInstanceIdsReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_INSTANCE_IDS_REQ */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_INSTANCE_IDS_REQ
void *CsrBtMapsGetInstanceIdsReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsGetInstanceIdsReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsGetInstanceIdsReq *) CsrPmemZalloc(sizeof(CsrBtMapsGetInstanceIdsReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_INSTANCE_IDS_REQ */

#ifndef EXCLUDE_CSR_BT_MAPS_ACTIVATE_REQ
CsrSize CsrBtMapsActivateReqSizeof(void *msg)
{
    CsrBtMapsActivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->nameLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_MAPS_ACTIVATE_REQ
CsrUint8 *CsrBtMapsActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsActivateReq *primitive;

    primitive = (CsrBtMapsActivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->supportedMessages);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->obexMaxPacketSize);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->windowSize);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmEnable);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->instanceId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->nameLen);
    CsrMemCpySer(buffer, length, &primitive->name, sizeof(primitive->name));
    if(primitive->name)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->name), ((CsrSize) (primitive->nameLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_MAPS_ACTIVATE_REQ
void *CsrBtMapsActivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsActivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsActivateReq *) CsrPmemZalloc(sizeof(CsrBtMapsActivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->supportedMessages, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->obexMaxPacketSize, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->windowSize, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmEnable, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->instanceId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->nameLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->name), buffer, &offset, ((CsrSize) sizeof(primitive->name)));
    if (primitive->nameLen)
    {
        primitive->name = CsrPmemZalloc(((CsrUint32) (primitive->nameLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->name), buffer, &offset, ((CsrSize) (primitive->nameLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->name = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_MAPS_ACTIVATE_REQ
void CsrBtMapsActivateReqSerFree(void *msg)
{
    CsrBtMapsActivateReq *primitive;

    primitive = (CsrBtMapsActivateReq *) msg;

    if(primitive->name != NULL)
    {
        CsrPmemFree(primitive->name);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_MAPS_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_MAPS_DISCONNECT_IND
CsrSize CsrBtMapsDisconnectIndSizeof(void *msg)
{
    CsrBtMapsDisconnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_DISCONNECT_IND
CsrUint8 *CsrBtMapsDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsDisconnectInd *primitive;

    primitive = (CsrBtMapsDisconnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_DISCONNECT_IND
void *CsrBtMapsDisconnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsDisconnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsDisconnectInd *) CsrPmemZalloc(sizeof(CsrBtMapsDisconnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_DEACTIVATE_CFM
CsrSize CsrBtMapsDeactivateCfmSizeof(void *msg)
{
    CsrBtMapsDeactivateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_MAPS_DEACTIVATE_CFM
CsrUint8 *CsrBtMapsDeactivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsDeactivateCfm *primitive;

    primitive = (CsrBtMapsDeactivateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_MAPS_DEACTIVATE_CFM
void *CsrBtMapsDeactivateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsDeactivateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsDeactivateCfm *) CsrPmemZalloc(sizeof(CsrBtMapsDeactivateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_MAPS_REGISTER_QID_REQ
CsrSize CsrBtMapsRegisterQidReqSizeof(void *msg)
{
    CsrBtMapsRegisterQidReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_REGISTER_QID_REQ */

#ifndef EXCLUDE_CSR_BT_MAPS_REGISTER_QID_REQ
CsrUint8 *CsrBtMapsRegisterQidReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsRegisterQidReq *primitive;

    primitive = (CsrBtMapsRegisterQidReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mapsInstanceId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_REGISTER_QID_REQ */

#ifndef EXCLUDE_CSR_BT_MAPS_REGISTER_QID_REQ
void *CsrBtMapsRegisterQidReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsRegisterQidReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsRegisterQidReq *) CsrPmemZalloc(sizeof(CsrBtMapsRegisterQidReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mapsInstanceId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_REGISTER_QID_REQ */

#ifndef EXCLUDE_CSR_BT_MAPS_UPDATE_INBOX_RES
CsrSize CsrBtMapsUpdateInboxResSizeof(void *msg)
{
    CsrBtMapsUpdateInboxRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_UPDATE_INBOX_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_UPDATE_INBOX_RES
CsrUint8 *CsrBtMapsUpdateInboxResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsUpdateInboxRes *primitive;

    primitive = (CsrBtMapsUpdateInboxRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_UPDATE_INBOX_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_UPDATE_INBOX_RES
void *CsrBtMapsUpdateInboxResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsUpdateInboxRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsUpdateInboxRes *) CsrPmemZalloc(sizeof(CsrBtMapsUpdateInboxRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_UPDATE_INBOX_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_EVENT_NOTIFICATION_RES
CsrSize CsrBtMapsEventNotificationResSizeof(void *msg)
{
    CsrBtMapsEventNotificationRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->bodyLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_EVENT_NOTIFICATION_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_EVENT_NOTIFICATION_RES
CsrUint8 *CsrBtMapsEventNotificationResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsEventNotificationRes *primitive;

    primitive = (CsrBtMapsEventNotificationRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyLength);
    CsrMemCpySer(buffer, length, &primitive->body, sizeof(primitive->body));
    if(primitive->body)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->body), ((CsrSize) (primitive->bodyLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_EVENT_NOTIFICATION_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_EVENT_NOTIFICATION_RES
void *CsrBtMapsEventNotificationResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsEventNotificationRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsEventNotificationRes *) CsrPmemZalloc(sizeof(CsrBtMapsEventNotificationRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->body), buffer, &offset, ((CsrSize) sizeof(primitive->body)));
    if (primitive->bodyLength)
    {
        primitive->body = CsrPmemZalloc(((CsrUint32) (primitive->bodyLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->body), buffer, &offset, ((CsrSize) (primitive->bodyLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->body = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_EVENT_NOTIFICATION_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_EVENT_NOTIFICATION_RES
void CsrBtMapsEventNotificationResSerFree(void *msg)
{
    CsrBtMapsEventNotificationRes *primitive;

    primitive = (CsrBtMapsEventNotificationRes *) msg;

    if(primitive->body != NULL)
    {
        CsrPmemFree(primitive->body);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_MAPS_EVENT_NOTIFICATION_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_RES
CsrSize CsrBtMapsGetMessageResSizeof(void *msg)
{
    CsrBtMapsGetMessageRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->bodyLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_RES
CsrUint8 *CsrBtMapsGetMessageResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsGetMessageRes *primitive;

    primitive = (CsrBtMapsGetMessageRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyLength);
    CsrMemCpySer(buffer, length, &primitive->body, sizeof(primitive->body));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    if(primitive->body)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->body), ((CsrSize) (primitive->bodyLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_RES
void *CsrBtMapsGetMessageResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsGetMessageRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsGetMessageRes *) CsrPmemZalloc(sizeof(CsrBtMapsGetMessageRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->body), buffer, &offset, ((CsrSize) sizeof(primitive->body)));
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    if (primitive->bodyLength)
    {
        primitive->body = CsrPmemZalloc(((CsrUint32) (primitive->bodyLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->body), buffer, &offset, ((CsrSize) (primitive->bodyLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->body = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_RES
void CsrBtMapsGetMessageResSerFree(void *msg)
{
    CsrBtMapsGetMessageRes *primitive;

    primitive = (CsrBtMapsGetMessageRes *) msg;

    if(primitive->body != NULL)
    {
        CsrPmemFree(primitive->body);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_RES
CsrSize CsrBtMapsGetMessageListingResSizeof(void *msg)
{
    CsrBtMapsGetMessageListingRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->bodyLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_RES
CsrUint8 *CsrBtMapsGetMessageListingResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsGetMessageListingRes *primitive;

    primitive = (CsrBtMapsGetMessageListingRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyLength);
    CsrMemCpySer(buffer, length, &primitive->body, sizeof(primitive->body));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    if(primitive->body)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->body), ((CsrSize) (primitive->bodyLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_RES
void *CsrBtMapsGetMessageListingResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsGetMessageListingRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsGetMessageListingRes *) CsrPmemZalloc(sizeof(CsrBtMapsGetMessageListingRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->body), buffer, &offset, ((CsrSize) sizeof(primitive->body)));
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    if (primitive->bodyLength)
    {
        primitive->body = CsrPmemZalloc(((CsrUint32) (primitive->bodyLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->body), buffer, &offset, ((CsrSize) (primitive->bodyLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->body = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_RES
void CsrBtMapsGetMessageListingResSerFree(void *msg)
{
    CsrBtMapsGetMessageListingRes *primitive;

    primitive = (CsrBtMapsGetMessageListingRes *) msg;

    if(primitive->body != NULL)
    {
        CsrPmemFree(primitive->body);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_ABORT_IND
CsrSize CsrBtMapsAbortIndSizeof(void *msg)
{
    CsrBtMapsAbortInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_ABORT_IND
CsrUint8 *CsrBtMapsAbortIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsAbortInd *primitive;

    primitive = (CsrBtMapsAbortInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->descriptionOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->descriptionLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_ABORT_IND
void *CsrBtMapsAbortIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsAbortInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsAbortInd *) CsrPmemZalloc(sizeof(CsrBtMapsAbortInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->descriptionOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->descriptionLength, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_MAPS_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_ABORT_IND
void CsrBtMapsAbortIndSerFree(void *msg)
{
    CsrBtMapsAbortInd *primitive;

    primitive = (CsrBtMapsAbortInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_MAPS_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_EVENT_NOTIFICATION_REQ
CsrSize CsrBtMapsEventNotificationReqSizeof(void *msg)
{
    CsrBtMapsEventNotificationReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_EVENT_NOTIFICATION_REQ */

#ifndef EXCLUDE_CSR_BT_MAPS_EVENT_NOTIFICATION_REQ
CsrUint8 *CsrBtMapsEventNotificationReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsEventNotificationReq *primitive;

    primitive = (CsrBtMapsEventNotificationReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_EVENT_NOTIFICATION_REQ */

#ifndef EXCLUDE_CSR_BT_MAPS_EVENT_NOTIFICATION_REQ
void *CsrBtMapsEventNotificationReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsEventNotificationReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsEventNotificationReq *) CsrPmemZalloc(sizeof(CsrBtMapsEventNotificationReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_EVENT_NOTIFICATION_REQ */

#ifndef EXCLUDE_CSR_BT_MAPS_UPDATE_INBOX_IND
CsrSize CsrBtMapsUpdateInboxIndSizeof(void *msg)
{
    CsrBtMapsUpdateInboxInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPS_UPDATE_INBOX_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_UPDATE_INBOX_IND
CsrUint8 *CsrBtMapsUpdateInboxIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapsUpdateInboxInd *primitive;

    primitive = (CsrBtMapsUpdateInboxInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPS_UPDATE_INBOX_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_UPDATE_INBOX_IND
void *CsrBtMapsUpdateInboxIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapsUpdateInboxInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapsUpdateInboxInd *) CsrPmemZalloc(sizeof(CsrBtMapsUpdateInboxInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPS_UPDATE_INBOX_IND */

#endif /* EXCLUDE_CSR_BT_MAPS_MODULE */
