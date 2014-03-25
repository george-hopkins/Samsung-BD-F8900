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
#include "csr_bt_mapc_prim.h"

#ifndef EXCLUDE_CSR_BT_MAPC_MODULE

void CsrBtMapcPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_MAPC_SECURITY_OUT_CFM
CsrSize CsrBtMapcSecurityOutCfmSizeof(void *msg)
{
    CsrBtMapcSecurityOutCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_SECURITY_OUT_CFM
CsrUint8 *CsrBtMapcSecurityOutCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcSecurityOutCfm *primitive;

    primitive = (CsrBtMapcSecurityOutCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPC_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_SECURITY_OUT_CFM
void *CsrBtMapcSecurityOutCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcSecurityOutCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcSecurityOutCfm *) CsrPmemZalloc(sizeof(CsrBtMapcSecurityOutCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPC_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_UPDATE_INBOX_CFM
CsrSize CsrBtMapcUpdateInboxCfmSizeof(void *msg)
{
    CsrBtMapcUpdateInboxCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_UPDATE_INBOX_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_UPDATE_INBOX_CFM
CsrUint8 *CsrBtMapcUpdateInboxCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcUpdateInboxCfm *primitive;

    primitive = (CsrBtMapcUpdateInboxCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPC_UPDATE_INBOX_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_UPDATE_INBOX_CFM
void *CsrBtMapcUpdateInboxCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcUpdateInboxCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcUpdateInboxCfm *) CsrPmemZalloc(sizeof(CsrBtMapcUpdateInboxCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPC_UPDATE_INBOX_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_CFM
CsrSize CsrBtMapcGetFolderListingCfmSizeof(void *msg)
{
    CsrBtMapcGetFolderListingCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_CFM
CsrUint8 *CsrBtMapcGetFolderListingCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcGetFolderListingCfm *primitive;

    primitive = (CsrBtMapcGetFolderListingCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->result);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->fullFolderListingSize);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_CFM
void *CsrBtMapcGetFolderListingCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcGetFolderListingCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcGetFolderListingCfm *) CsrPmemZalloc(sizeof(CsrBtMapcGetFolderListingCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->result, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->fullFolderListingSize, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyLength, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_CFM
void CsrBtMapcGetFolderListingCfmSerFree(void *msg)
{
    CsrBtMapcGetFolderListingCfm *primitive;

    primitive = (CsrBtMapcGetFolderListingCfm *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_RES
CsrSize CsrBtMapcGetMessageListingResSizeof(void *msg)
{
    CsrBtMapcGetMessageListingRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_RES */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_RES
CsrUint8 *CsrBtMapcGetMessageListingResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcGetMessageListingRes *primitive;

    primitive = (CsrBtMapcGetMessageListingRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_RES */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_RES
void *CsrBtMapcGetMessageListingResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcGetMessageListingRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcGetMessageListingRes *) CsrPmemZalloc(sizeof(CsrBtMapcGetMessageListingRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_RES */

#ifndef EXCLUDE_CSR_BT_MAPC_SET_ROOT_FOLDER_REQ
CsrSize CsrBtMapcSetRootFolderReqSizeof(void *msg)
{
    CsrBtMapcSetRootFolderReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_SET_ROOT_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_SET_ROOT_FOLDER_REQ
CsrUint8 *CsrBtMapcSetRootFolderReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcSetRootFolderReq *primitive;

    primitive = (CsrBtMapcSetRootFolderReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPC_SET_ROOT_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_SET_ROOT_FOLDER_REQ
void *CsrBtMapcSetRootFolderReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcSetRootFolderReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcSetRootFolderReq *) CsrPmemZalloc(sizeof(CsrBtMapcSetRootFolderReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPC_SET_ROOT_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_UPDATE_INBOX_REQ
CsrSize CsrBtMapcUpdateInboxReqSizeof(void *msg)
{
    CsrBtMapcUpdateInboxReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_UPDATE_INBOX_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_UPDATE_INBOX_REQ
CsrUint8 *CsrBtMapcUpdateInboxReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcUpdateInboxReq *primitive;

    primitive = (CsrBtMapcUpdateInboxReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPC_UPDATE_INBOX_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_UPDATE_INBOX_REQ
void *CsrBtMapcUpdateInboxReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcUpdateInboxReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcUpdateInboxReq *) CsrPmemZalloc(sizeof(CsrBtMapcUpdateInboxReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPC_UPDATE_INBOX_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_REGISTER_QID_REQ
CsrSize CsrBtMapcRegisterQidReqSizeof(void *msg)
{
    CsrBtMapcRegisterQidReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_REGISTER_QID_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_REGISTER_QID_REQ
CsrUint8 *CsrBtMapcRegisterQidReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcRegisterQidReq *primitive;

    primitive = (CsrBtMapcRegisterQidReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mapcInstanceId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPC_REGISTER_QID_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_REGISTER_QID_REQ
void *CsrBtMapcRegisterQidReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcRegisterQidReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcRegisterQidReq *) CsrPmemZalloc(sizeof(CsrBtMapcRegisterQidReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mapcInstanceId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPC_REGISTER_QID_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_NOTIFICATION_REGISTRATION_CFM
CsrSize CsrBtMapcNotificationRegistrationCfmSizeof(void *msg)
{
    CsrBtMapcNotificationRegistrationCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_NOTIFICATION_REGISTRATION_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_NOTIFICATION_REGISTRATION_CFM
CsrUint8 *CsrBtMapcNotificationRegistrationCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcNotificationRegistrationCfm *primitive;

    primitive = (CsrBtMapcNotificationRegistrationCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPC_NOTIFICATION_REGISTRATION_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_NOTIFICATION_REGISTRATION_CFM
void *CsrBtMapcNotificationRegistrationCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcNotificationRegistrationCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcNotificationRegistrationCfm *) CsrPmemZalloc(sizeof(CsrBtMapcNotificationRegistrationCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPC_NOTIFICATION_REGISTRATION_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_SECURITY_IN_REQ
CsrSize CsrBtMapcSecurityInReqSizeof(void *msg)
{
    CsrBtMapcSecurityInReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_SECURITY_IN_REQ
CsrUint8 *CsrBtMapcSecurityInReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcSecurityInReq *primitive;

    primitive = (CsrBtMapcSecurityInReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPC_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_SECURITY_IN_REQ
void *CsrBtMapcSecurityInReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcSecurityInReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcSecurityInReq *) CsrPmemZalloc(sizeof(CsrBtMapcSecurityInReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPC_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_SECURITY_OUT_REQ
CsrSize CsrBtMapcSecurityOutReqSizeof(void *msg)
{
    CsrBtMapcSecurityOutReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_SECURITY_OUT_REQ
CsrUint8 *CsrBtMapcSecurityOutReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcSecurityOutReq *primitive;

    primitive = (CsrBtMapcSecurityOutReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPC_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_SECURITY_OUT_REQ
void *CsrBtMapcSecurityOutReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcSecurityOutReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcSecurityOutReq *) CsrPmemZalloc(sizeof(CsrBtMapcSecurityOutReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPC_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_INSTANCE_IDS_CFM
CsrSize CsrBtMapcGetInstanceIdsCfmSizeof(void *msg)
{
    CsrBtMapcGetInstanceIdsCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->instanceIdsListSize * sizeof(CsrSchedQid)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_GET_INSTANCE_IDS_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_INSTANCE_IDS_CFM
CsrUint8 *CsrBtMapcGetInstanceIdsCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcGetInstanceIdsCfm *primitive;

    primitive = (CsrBtMapcGetInstanceIdsCfm *) msg;
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
#endif /* EXCLUDE_CSR_BT_MAPC_GET_INSTANCE_IDS_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_INSTANCE_IDS_CFM
void *CsrBtMapcGetInstanceIdsCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcGetInstanceIdsCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcGetInstanceIdsCfm *) CsrPmemZalloc(sizeof(CsrBtMapcGetInstanceIdsCfm));

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
#endif /* EXCLUDE_CSR_BT_MAPC_GET_INSTANCE_IDS_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_INSTANCE_IDS_CFM
void CsrBtMapcGetInstanceIdsCfmSerFree(void *msg)
{
    CsrBtMapcGetInstanceIdsCfm *primitive;

    primitive = (CsrBtMapcGetInstanceIdsCfm *) msg;

    if(primitive->instanceIdsList != NULL)
    {
        CsrPmemFree(primitive->instanceIdsList);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_MAPC_GET_INSTANCE_IDS_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_REQ
CsrSize CsrBtMapcGetMessageReqSizeof(void *msg)
{
    CsrBtMapcGetMessageReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->messageHandle));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_REQ
CsrUint8 *CsrBtMapcGetMessageReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcGetMessageReq *primitive;

    primitive = (CsrBtMapcGetMessageReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->messageHandle, sizeof(primitive->messageHandle));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->attachment);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->charset);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->fractionRequest);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    CsrUtf8StringSer(buffer, length, primitive->messageHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_REQ
void *CsrBtMapcGetMessageReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcGetMessageReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcGetMessageReq *) CsrPmemZalloc(sizeof(CsrBtMapcGetMessageReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->messageHandle), buffer, &offset, ((CsrSize) sizeof(primitive->messageHandle)));
    CsrUint8Des((CsrUint8 *) &primitive->attachment, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->charset, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->fractionRequest, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    CsrUtf8StringDes(&primitive->messageHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_REQ
void CsrBtMapcGetMessageReqSerFree(void *msg)
{
    CsrBtMapcGetMessageReq *primitive;

    primitive = (CsrBtMapcGetMessageReq *) msg;

    if(primitive->messageHandle != NULL)
    {
        CsrPmemFree(primitive->messageHandle);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_DISCONNECT_REQ
CsrSize CsrBtMapcDisconnectReqSizeof(void *msg)
{
    CsrBtMapcDisconnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_DISCONNECT_REQ
CsrUint8 *CsrBtMapcDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcDisconnectReq *primitive;

    primitive = (CsrBtMapcDisconnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->normalObexDisconnect);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPC_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_DISCONNECT_REQ
void *CsrBtMapcDisconnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcDisconnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcDisconnectReq *) CsrPmemZalloc(sizeof(CsrBtMapcDisconnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->normalObexDisconnect, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPC_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_EVENT_NOTIFICATION_IND
CsrSize CsrBtMapcEventNotificationIndSizeof(void *msg)
{
    CsrBtMapcEventNotificationInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_EVENT_NOTIFICATION_IND */

#ifndef EXCLUDE_CSR_BT_MAPC_EVENT_NOTIFICATION_IND
CsrUint8 *CsrBtMapcEventNotificationIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcEventNotificationInd *primitive;

    primitive = (CsrBtMapcEventNotificationInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPC_EVENT_NOTIFICATION_IND */

#ifndef EXCLUDE_CSR_BT_MAPC_EVENT_NOTIFICATION_IND
void *CsrBtMapcEventNotificationIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcEventNotificationInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcEventNotificationInd *) CsrPmemZalloc(sizeof(CsrBtMapcEventNotificationInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyLength, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_MAPC_EVENT_NOTIFICATION_IND */

#ifndef EXCLUDE_CSR_BT_MAPC_EVENT_NOTIFICATION_IND
void CsrBtMapcEventNotificationIndSerFree(void *msg)
{
    CsrBtMapcEventNotificationInd *primitive;

    primitive = (CsrBtMapcEventNotificationInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_MAPC_EVENT_NOTIFICATION_IND */

#ifndef EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_CFM
CsrSize CsrBtMapcPushMessageCfmSizeof(void *msg)
{
    CsrBtMapcPushMessageCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->messageHandle));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_CFM
CsrUint8 *CsrBtMapcPushMessageCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcPushMessageCfm *primitive;

    primitive = (CsrBtMapcPushMessageCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->result);
    CsrMemCpySer(buffer, length, &primitive->messageHandle, sizeof(primitive->messageHandle));
    CsrUtf8StringSer(buffer, length, primitive->messageHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_CFM
void *CsrBtMapcPushMessageCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcPushMessageCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcPushMessageCfm *) CsrPmemZalloc(sizeof(CsrBtMapcPushMessageCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->result, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->messageHandle), buffer, &offset, ((CsrSize) sizeof(primitive->messageHandle)));
    CsrUtf8StringDes(&primitive->messageHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_CFM
void CsrBtMapcPushMessageCfmSerFree(void *msg)
{
    CsrBtMapcPushMessageCfm *primitive;

    primitive = (CsrBtMapcPushMessageCfm *) msg;

    if(primitive->messageHandle != NULL)
    {
        CsrPmemFree(primitive->messageHandle);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_EVENT_NOTIFICATION_RES
CsrSize CsrBtMapcEventNotificationResSizeof(void *msg)
{
    CsrBtMapcEventNotificationRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_EVENT_NOTIFICATION_RES */

#ifndef EXCLUDE_CSR_BT_MAPC_EVENT_NOTIFICATION_RES
CsrUint8 *CsrBtMapcEventNotificationResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcEventNotificationRes *primitive;

    primitive = (CsrBtMapcEventNotificationRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->response);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPC_EVENT_NOTIFICATION_RES */

#ifndef EXCLUDE_CSR_BT_MAPC_EVENT_NOTIFICATION_RES
void *CsrBtMapcEventNotificationResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcEventNotificationRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcEventNotificationRes *) CsrPmemZalloc(sizeof(CsrBtMapcEventNotificationRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->response, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPC_EVENT_NOTIFICATION_RES */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_CFM
CsrSize CsrBtMapcGetMessageCfmSizeof(void *msg)
{
    CsrBtMapcGetMessageCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_CFM
CsrUint8 *CsrBtMapcGetMessageCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcGetMessageCfm *primitive;

    primitive = (CsrBtMapcGetMessageCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->result);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->fractionDeliver);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_CFM
void *CsrBtMapcGetMessageCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcGetMessageCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcGetMessageCfm *) CsrPmemZalloc(sizeof(CsrBtMapcGetMessageCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->result, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->fractionDeliver, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyLength, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_CFM
void CsrBtMapcGetMessageCfmSerFree(void *msg)
{
    CsrBtMapcGetMessageCfm *primitive;

    primitive = (CsrBtMapcGetMessageCfm *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_IND
CsrSize CsrBtMapcGetMessageListingIndSizeof(void *msg)
{
    CsrBtMapcGetMessageListingInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->mseTime) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_IND */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_IND
CsrUint8 *CsrBtMapcGetMessageListingIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcGetMessageListingInd *primitive;

    primitive = (CsrBtMapcGetMessageListingInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->newMessages);
    CsrMemCpySer(buffer, length, &primitive->mseTime, sizeof(primitive->mseTime));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->fullMessageListingSize);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->messageListingRetrieved);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrUtf8StringSer(buffer, length, primitive->mseTime);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_IND */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_IND
void *CsrBtMapcGetMessageListingIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcGetMessageListingInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcGetMessageListingInd *) CsrPmemZalloc(sizeof(CsrBtMapcGetMessageListingInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->newMessages, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->mseTime), buffer, &offset, ((CsrSize) sizeof(primitive->mseTime)));
    CsrUint16Des((CsrUint16 *) &primitive->fullMessageListingSize, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->messageListingRetrieved, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    CsrUint16Des((CsrUint16 *) &primitive->payloadLength, buffer, &offset);
    CsrUtf8StringDes(&primitive->mseTime, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_IND */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_IND
void CsrBtMapcGetMessageListingIndSerFree(void *msg)
{
    CsrBtMapcGetMessageListingInd *primitive;

    primitive = (CsrBtMapcGetMessageListingInd *) msg;

    if(primitive->mseTime != NULL)
    {
        CsrPmemFree(primitive->mseTime);
    }
    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_IND */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_INSTANCE_IDS_REQ
CsrSize CsrBtMapcGetInstanceIdsReqSizeof(void *msg)
{
    CsrBtMapcGetInstanceIdsReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_GET_INSTANCE_IDS_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_INSTANCE_IDS_REQ
CsrUint8 *CsrBtMapcGetInstanceIdsReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcGetInstanceIdsReq *primitive;

    primitive = (CsrBtMapcGetInstanceIdsReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPC_GET_INSTANCE_IDS_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_INSTANCE_IDS_REQ
void *CsrBtMapcGetInstanceIdsReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcGetInstanceIdsReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcGetInstanceIdsReq *) CsrPmemZalloc(sizeof(CsrBtMapcGetInstanceIdsReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPC_GET_INSTANCE_IDS_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_RES
CsrSize CsrBtMapcGetMessageResSizeof(void *msg)
{
    CsrBtMapcGetMessageRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_RES */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_RES
CsrUint8 *CsrBtMapcGetMessageResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcGetMessageRes *primitive;

    primitive = (CsrBtMapcGetMessageRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_RES */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_RES
void *CsrBtMapcGetMessageResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcGetMessageRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcGetMessageRes *) CsrPmemZalloc(sizeof(CsrBtMapcGetMessageRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_RES */

#ifndef EXCLUDE_CSR_BT_MAPC_SELECT_MAS_INSTANCE_IND
CsrSize CsrBtMapcSelectMasInstanceIndSizeof(void *msg)
{
    CsrBtMapcSelectMasInstanceInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->masInstanceListSize * sizeof(CsrBtMapcMasInstance)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_SELECT_MAS_INSTANCE_IND */

#ifndef EXCLUDE_CSR_BT_MAPC_SELECT_MAS_INSTANCE_IND
CsrUint8 *CsrBtMapcSelectMasInstanceIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcSelectMasInstanceInd *primitive;

    primitive = (CsrBtMapcSelectMasInstanceInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);
    CsrMemCpySer(buffer, length, &primitive->masInstanceList, sizeof(primitive->masInstanceList));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->masInstanceListSize);
    if(primitive->masInstanceList)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->masInstanceList), ((CsrSize) (primitive->masInstanceListSize * sizeof(CsrBtMapcMasInstance))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPC_SELECT_MAS_INSTANCE_IND */

#ifndef EXCLUDE_CSR_BT_MAPC_SELECT_MAS_INSTANCE_IND
void *CsrBtMapcSelectMasInstanceIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcSelectMasInstanceInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcSelectMasInstanceInd *) CsrPmemZalloc(sizeof(CsrBtMapcSelectMasInstanceInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->masInstanceList), buffer, &offset, ((CsrSize) sizeof(primitive->masInstanceList)));
    CsrUint16Des((CsrUint16 *) &primitive->masInstanceListSize, buffer, &offset);
    if (primitive->masInstanceListSize)
    {
        primitive->masInstanceList = CsrPmemZalloc(((CsrUint32) (primitive->masInstanceListSize * sizeof(CsrBtMapcMasInstance))));
        CsrMemCpyDes(((void *) primitive->masInstanceList), buffer, &offset, ((CsrSize) (primitive->masInstanceListSize * sizeof(CsrBtMapcMasInstance))));
    }
    else
    {
        primitive->masInstanceList = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPC_SELECT_MAS_INSTANCE_IND */

#ifndef EXCLUDE_CSR_BT_MAPC_SELECT_MAS_INSTANCE_IND
void CsrBtMapcSelectMasInstanceIndSerFree(void *msg)
{
    CsrBtMapcSelectMasInstanceInd *primitive;

    primitive = (CsrBtMapcSelectMasInstanceInd *) msg;

    if(primitive->masInstanceList != NULL)
    {
        CsrPmemFree(primitive->masInstanceList);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_MAPC_SELECT_MAS_INSTANCE_IND */

#ifndef EXCLUDE_CSR_BT_MAPC_NOTIFICATION_REGISTRATION_OFF_IND
CsrSize CsrBtMapcNotificationRegistrationOffIndSizeof(void *msg)
{
    CsrBtMapcNotificationRegistrationOffInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_NOTIFICATION_REGISTRATION_OFF_IND */

#ifndef EXCLUDE_CSR_BT_MAPC_NOTIFICATION_REGISTRATION_OFF_IND
CsrUint8 *CsrBtMapcNotificationRegistrationOffIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcNotificationRegistrationOffInd *primitive;

    primitive = (CsrBtMapcNotificationRegistrationOffInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPC_NOTIFICATION_REGISTRATION_OFF_IND */

#ifndef EXCLUDE_CSR_BT_MAPC_NOTIFICATION_REGISTRATION_OFF_IND
void *CsrBtMapcNotificationRegistrationOffIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcNotificationRegistrationOffInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcNotificationRegistrationOffInd *) CsrPmemZalloc(sizeof(CsrBtMapcNotificationRegistrationOffInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPC_NOTIFICATION_REGISTRATION_OFF_IND */

#ifndef EXCLUDE_CSR_BT_MAPC_ABORT_REQ
CsrSize CsrBtMapcAbortReqSizeof(void *msg)
{
    CsrBtMapcAbortReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_ABORT_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_ABORT_REQ
CsrUint8 *CsrBtMapcAbortReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcAbortReq *primitive;

    primitive = (CsrBtMapcAbortReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPC_ABORT_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_ABORT_REQ
void *CsrBtMapcAbortReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcAbortReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcAbortReq *) CsrPmemZalloc(sizeof(CsrBtMapcAbortReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPC_ABORT_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_SET_ROOT_FOLDER_CFM
CsrSize CsrBtMapcSetRootFolderCfmSizeof(void *msg)
{
    CsrBtMapcSetRootFolderCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_SET_ROOT_FOLDER_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_SET_ROOT_FOLDER_CFM
CsrUint8 *CsrBtMapcSetRootFolderCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcSetRootFolderCfm *primitive;

    primitive = (CsrBtMapcSetRootFolderCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPC_SET_ROOT_FOLDER_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_SET_ROOT_FOLDER_CFM
void *CsrBtMapcSetRootFolderCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcSetRootFolderCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcSetRootFolderCfm *) CsrPmemZalloc(sizeof(CsrBtMapcSetRootFolderCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPC_SET_ROOT_FOLDER_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_CFM
CsrSize CsrBtMapcGetMessageListingCfmSizeof(void *msg)
{
    CsrBtMapcGetMessageListingCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->mseTime) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_CFM
CsrUint8 *CsrBtMapcGetMessageListingCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcGetMessageListingCfm *primitive;

    primitive = (CsrBtMapcGetMessageListingCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->result);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->newMessages);
    CsrMemCpySer(buffer, length, &primitive->mseTime, sizeof(primitive->mseTime));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->fullMessageListingSize);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrUtf8StringSer(buffer, length, primitive->mseTime);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_CFM
void *CsrBtMapcGetMessageListingCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcGetMessageListingCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcGetMessageListingCfm *) CsrPmemZalloc(sizeof(CsrBtMapcGetMessageListingCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->result, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->newMessages, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->mseTime), buffer, &offset, ((CsrSize) sizeof(primitive->mseTime)));
    CsrUint16Des((CsrUint16 *) &primitive->fullMessageListingSize, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    CsrUint16Des((CsrUint16 *) &primitive->payloadLength, buffer, &offset);
    CsrUtf8StringDes(&primitive->mseTime, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_CFM
void CsrBtMapcGetMessageListingCfmSerFree(void *msg)
{
    CsrBtMapcGetMessageListingCfm *primitive;

    primitive = (CsrBtMapcGetMessageListingCfm *) msg;

    if(primitive->mseTime != NULL)
    {
        CsrPmemFree(primitive->mseTime);
    }
    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_SET_MESSAGE_STATUS_CFM
CsrSize CsrBtMapcSetMessageStatusCfmSizeof(void *msg)
{
    CsrBtMapcSetMessageStatusCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_SET_MESSAGE_STATUS_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_SET_MESSAGE_STATUS_CFM
CsrUint8 *CsrBtMapcSetMessageStatusCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcSetMessageStatusCfm *primitive;

    primitive = (CsrBtMapcSetMessageStatusCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPC_SET_MESSAGE_STATUS_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_SET_MESSAGE_STATUS_CFM
void *CsrBtMapcSetMessageStatusCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcSetMessageStatusCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcSetMessageStatusCfm *) CsrPmemZalloc(sizeof(CsrBtMapcSetMessageStatusCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPC_SET_MESSAGE_STATUS_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_IND
CsrSize CsrBtMapcPushMessageIndSizeof(void *msg)
{
    CsrBtMapcPushMessageInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_IND */

#ifndef EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_IND
CsrUint8 *CsrBtMapcPushMessageIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcPushMessageInd *primitive;

    primitive = (CsrBtMapcPushMessageInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxAllowedPayloadSize);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_IND */

#ifndef EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_IND
void *CsrBtMapcPushMessageIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcPushMessageInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcPushMessageInd *) CsrPmemZalloc(sizeof(CsrBtMapcPushMessageInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxAllowedPayloadSize, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_IND */

#ifndef EXCLUDE_CSR_BT_MAPC_NOTIFICATION_REGISTRATION_REQ
CsrSize CsrBtMapcNotificationRegistrationReqSizeof(void *msg)
{
    CsrBtMapcNotificationRegistrationReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_NOTIFICATION_REGISTRATION_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_NOTIFICATION_REGISTRATION_REQ
CsrUint8 *CsrBtMapcNotificationRegistrationReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcNotificationRegistrationReq *primitive;

    primitive = (CsrBtMapcNotificationRegistrationReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->enableNotifications);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mnsController);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPC_NOTIFICATION_REGISTRATION_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_NOTIFICATION_REGISTRATION_REQ
void *CsrBtMapcNotificationRegistrationReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcNotificationRegistrationReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcNotificationRegistrationReq *) CsrPmemZalloc(sizeof(CsrBtMapcNotificationRegistrationReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->enableNotifications, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mnsController, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPC_NOTIFICATION_REGISTRATION_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_CANCEL_CONNECT_REQ
CsrSize CsrBtMapcCancelConnectReqSizeof(void *msg)
{
    CsrBtMapcCancelConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_CANCEL_CONNECT_REQ
CsrUint8 *CsrBtMapcCancelConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcCancelConnectReq *primitive;

    primitive = (CsrBtMapcCancelConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPC_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_CANCEL_CONNECT_REQ
void *CsrBtMapcCancelConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcCancelConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcCancelConnectReq *) CsrPmemZalloc(sizeof(CsrBtMapcCancelConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPC_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_IND
CsrSize CsrBtMapcGetMessageIndSizeof(void *msg)
{
    CsrBtMapcGetMessageInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_IND */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_IND
CsrUint8 *CsrBtMapcGetMessageIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcGetMessageInd *primitive;

    primitive = (CsrBtMapcGetMessageInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_IND */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_IND
void *CsrBtMapcGetMessageIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcGetMessageInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcGetMessageInd *) CsrPmemZalloc(sizeof(CsrBtMapcGetMessageInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyLength, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_IND */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_IND
void CsrBtMapcGetMessageIndSerFree(void *msg)
{
    CsrBtMapcGetMessageInd *primitive;

    primitive = (CsrBtMapcGetMessageInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_IND */

#ifndef EXCLUDE_CSR_BT_MAPC_SET_BACK_FOLDER_REQ
CsrSize CsrBtMapcSetBackFolderReqSizeof(void *msg)
{
    CsrBtMapcSetBackFolderReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_SET_BACK_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_SET_BACK_FOLDER_REQ
CsrUint8 *CsrBtMapcSetBackFolderReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcSetBackFolderReq *primitive;

    primitive = (CsrBtMapcSetBackFolderReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPC_SET_BACK_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_SET_BACK_FOLDER_REQ
void *CsrBtMapcSetBackFolderReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcSetBackFolderReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcSetBackFolderReq *) CsrPmemZalloc(sizeof(CsrBtMapcSetBackFolderReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPC_SET_BACK_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_IND
CsrSize CsrBtMapcGetFolderListingIndSizeof(void *msg)
{
    CsrBtMapcGetFolderListingInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_IND */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_IND
CsrUint8 *CsrBtMapcGetFolderListingIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcGetFolderListingInd *primitive;

    primitive = (CsrBtMapcGetFolderListingInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->fullFolderListingSize);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->folderListingRetrieved);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_IND */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_IND
void *CsrBtMapcGetFolderListingIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcGetFolderListingInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcGetFolderListingInd *) CsrPmemZalloc(sizeof(CsrBtMapcGetFolderListingInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->fullFolderListingSize, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->folderListingRetrieved, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyLength, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_IND */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_IND
void CsrBtMapcGetFolderListingIndSerFree(void *msg)
{
    CsrBtMapcGetFolderListingInd *primitive;

    primitive = (CsrBtMapcGetFolderListingInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_IND */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_REQ
CsrSize CsrBtMapcGetMessageListingReqSizeof(void *msg)
{
    CsrBtMapcGetMessageListingReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->folderName) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->filterPeriodBegin) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->filterPeriodEnd) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->filterRecipient) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->filterOriginator));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_REQ
CsrUint8 *CsrBtMapcGetMessageListingReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcGetMessageListingReq *primitive;

    primitive = (CsrBtMapcGetMessageListingReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
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
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    CsrUtf8StringSer(buffer, length, primitive->folderName);
    CsrUtf8StringSer(buffer, length, primitive->filterPeriodBegin);
    CsrUtf8StringSer(buffer, length, primitive->filterPeriodEnd);
    CsrUtf8StringSer(buffer, length, primitive->filterRecipient);
    CsrUtf8StringSer(buffer, length, primitive->filterOriginator);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_REQ
void *CsrBtMapcGetMessageListingReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcGetMessageListingReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcGetMessageListingReq *) CsrPmemZalloc(sizeof(CsrBtMapcGetMessageListingReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
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
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    CsrUtf8StringDes(&primitive->folderName, buffer, &offset);
    CsrUtf8StringDes(&primitive->filterPeriodBegin, buffer, &offset);
    CsrUtf8StringDes(&primitive->filterPeriodEnd, buffer, &offset);
    CsrUtf8StringDes(&primitive->filterRecipient, buffer, &offset);
    CsrUtf8StringDes(&primitive->filterOriginator, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_REQ
void CsrBtMapcGetMessageListingReqSerFree(void *msg)
{
    CsrBtMapcGetMessageListingReq *primitive;

    primitive = (CsrBtMapcGetMessageListingReq *) msg;

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
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_CONNECT_CFM
CsrSize CsrBtMapcConnectCfmSizeof(void *msg)
{
    CsrBtMapcConnectCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_CONNECT_CFM
CsrUint8 *CsrBtMapcConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcConnectCfm *primitive;

    primitive = (CsrBtMapcConnectCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);
    CsrMemCpySer(buffer, length, ((const void *) primitive->serviceName), ((CsrSize) ((50) + 1)));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->masInstanceId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->supportedMessages);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->obexPeerMaxPacketSize);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPC_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_CONNECT_CFM
void *CsrBtMapcConnectCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcConnectCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcConnectCfm *) CsrPmemZalloc(sizeof(CsrBtMapcConnectCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->serviceName), buffer, &offset, ((CsrSize) ((50) + 1)));
    CsrUint8Des((CsrUint8 *) &primitive->masInstanceId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->supportedMessages, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->obexPeerMaxPacketSize, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPC_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_CONNECT_REQ
CsrSize CsrBtMapcConnectReqSizeof(void *msg)
{
    CsrBtMapcConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_CONNECT_REQ
CsrUint8 *CsrBtMapcConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcConnectReq *primitive;

    primitive = (CsrBtMapcConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxPacketSize);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->count);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->windowSize);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmEnable);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_CONNECT_REQ
void *CsrBtMapcConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcConnectReq *) CsrPmemZalloc(sizeof(CsrBtMapcConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxPacketSize, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->count, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->windowSize, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmEnable, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_SET_FOLDER_CFM
CsrSize CsrBtMapcSetFolderCfmSizeof(void *msg)
{
    CsrBtMapcSetFolderCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_SET_FOLDER_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_SET_FOLDER_CFM
CsrUint8 *CsrBtMapcSetFolderCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcSetFolderCfm *primitive;

    primitive = (CsrBtMapcSetFolderCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPC_SET_FOLDER_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_SET_FOLDER_CFM
void *CsrBtMapcSetFolderCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcSetFolderCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcSetFolderCfm *) CsrPmemZalloc(sizeof(CsrBtMapcSetFolderCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPC_SET_FOLDER_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_RES
CsrSize CsrBtMapcPushMessageResSizeof(void *msg)
{
    CsrBtMapcPushMessageRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_RES */

#ifndef EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_RES
CsrUint8 *CsrBtMapcPushMessageResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcPushMessageRes *primitive;

    primitive = (CsrBtMapcPushMessageRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_RES */

#ifndef EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_RES
void *CsrBtMapcPushMessageResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcPushMessageRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcPushMessageRes *) CsrPmemZalloc(sizeof(CsrBtMapcPushMessageRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_RES */

#ifndef EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_RES
void CsrBtMapcPushMessageResSerFree(void *msg)
{
    CsrBtMapcPushMessageRes *primitive;

    primitive = (CsrBtMapcPushMessageRes *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_RES */

#ifndef EXCLUDE_CSR_BT_MAPC_SET_MESSAGE_STATUS_REQ
CsrSize CsrBtMapcSetMessageStatusReqSizeof(void *msg)
{
    CsrBtMapcSetMessageStatusReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->messageHandle));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_SET_MESSAGE_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_SET_MESSAGE_STATUS_REQ
CsrUint8 *CsrBtMapcSetMessageStatusReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcSetMessageStatusReq *primitive;

    primitive = (CsrBtMapcSetMessageStatusReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->messageHandle, sizeof(primitive->messageHandle));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->statusIndicator);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->statusValue);
    CsrUtf8StringSer(buffer, length, primitive->messageHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPC_SET_MESSAGE_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_SET_MESSAGE_STATUS_REQ
void *CsrBtMapcSetMessageStatusReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcSetMessageStatusReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcSetMessageStatusReq *) CsrPmemZalloc(sizeof(CsrBtMapcSetMessageStatusReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->messageHandle), buffer, &offset, ((CsrSize) sizeof(primitive->messageHandle)));
    CsrUint8Des((CsrUint8 *) &primitive->statusIndicator, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->statusValue, buffer, &offset);
    CsrUtf8StringDes(&primitive->messageHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPC_SET_MESSAGE_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_SET_MESSAGE_STATUS_REQ
void CsrBtMapcSetMessageStatusReqSerFree(void *msg)
{
    CsrBtMapcSetMessageStatusReq *primitive;

    primitive = (CsrBtMapcSetMessageStatusReq *) msg;

    if(primitive->messageHandle != NULL)
    {
        CsrPmemFree(primitive->messageHandle);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_MAPC_SET_MESSAGE_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_DISCONNECT_IND
CsrSize CsrBtMapcDisconnectIndSizeof(void *msg)
{
    CsrBtMapcDisconnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_MAPC_DISCONNECT_IND
CsrUint8 *CsrBtMapcDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcDisconnectInd *primitive;

    primitive = (CsrBtMapcDisconnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPC_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_MAPC_DISCONNECT_IND
void *CsrBtMapcDisconnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcDisconnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcDisconnectInd *) CsrPmemZalloc(sizeof(CsrBtMapcDisconnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPC_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_MAPC_SET_FOLDER_REQ
CsrSize CsrBtMapcSetFolderReqSizeof(void *msg)
{
    CsrBtMapcSetFolderReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->folderName));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_SET_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_SET_FOLDER_REQ
CsrUint8 *CsrBtMapcSetFolderReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcSetFolderReq *primitive;

    primitive = (CsrBtMapcSetFolderReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->folderName, sizeof(primitive->folderName));
    CsrUtf8StringSer(buffer, length, primitive->folderName);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPC_SET_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_SET_FOLDER_REQ
void *CsrBtMapcSetFolderReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcSetFolderReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcSetFolderReq *) CsrPmemZalloc(sizeof(CsrBtMapcSetFolderReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->folderName), buffer, &offset, ((CsrSize) sizeof(primitive->folderName)));
    CsrUtf8StringDes(&primitive->folderName, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPC_SET_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_SET_FOLDER_REQ
void CsrBtMapcSetFolderReqSerFree(void *msg)
{
    CsrBtMapcSetFolderReq *primitive;

    primitive = (CsrBtMapcSetFolderReq *) msg;

    if(primitive->folderName != NULL)
    {
        CsrPmemFree(primitive->folderName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_MAPC_SET_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_RES
CsrSize CsrBtMapcGetFolderListingResSizeof(void *msg)
{
    CsrBtMapcGetFolderListingRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_RES */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_RES
CsrUint8 *CsrBtMapcGetFolderListingResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcGetFolderListingRes *primitive;

    primitive = (CsrBtMapcGetFolderListingRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_RES */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_RES
void *CsrBtMapcGetFolderListingResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcGetFolderListingRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcGetFolderListingRes *) CsrPmemZalloc(sizeof(CsrBtMapcGetFolderListingRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_RES */

#ifndef EXCLUDE_CSR_BT_MAPC_ABORT_CFM
CsrSize CsrBtMapcAbortCfmSizeof(void *msg)
{
    CsrBtMapcAbortCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_ABORT_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_ABORT_CFM
CsrUint8 *CsrBtMapcAbortCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcAbortCfm *primitive;

    primitive = (CsrBtMapcAbortCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPC_ABORT_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_ABORT_CFM
void *CsrBtMapcAbortCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcAbortCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcAbortCfm *) CsrPmemZalloc(sizeof(CsrBtMapcAbortCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPC_ABORT_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_REQ
CsrSize CsrBtMapcGetFolderListingReqSizeof(void *msg)
{
    CsrBtMapcGetFolderListingReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_REQ
CsrUint8 *CsrBtMapcGetFolderListingReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcGetFolderListingReq *primitive;

    primitive = (CsrBtMapcGetFolderListingReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxListCount);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->listStartOffset);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_REQ
void *CsrBtMapcGetFolderListingReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcGetFolderListingReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcGetFolderListingReq *) CsrPmemZalloc(sizeof(CsrBtMapcGetFolderListingReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxListCount, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->listStartOffset, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_SECURITY_IN_CFM
CsrSize CsrBtMapcSecurityInCfmSizeof(void *msg)
{
    CsrBtMapcSecurityInCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_SECURITY_IN_CFM
CsrUint8 *CsrBtMapcSecurityInCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcSecurityInCfm *primitive;

    primitive = (CsrBtMapcSecurityInCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPC_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_SECURITY_IN_CFM
void *CsrBtMapcSecurityInCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcSecurityInCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcSecurityInCfm *) CsrPmemZalloc(sizeof(CsrBtMapcSecurityInCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPC_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_REQ
CsrSize CsrBtMapcPushMessageReqSizeof(void *msg)
{
    CsrBtMapcPushMessageReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->folderName));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_REQ
CsrUint8 *CsrBtMapcPushMessageReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcPushMessageReq *primitive;

    primitive = (CsrBtMapcPushMessageReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->folderName, sizeof(primitive->folderName));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->lengthOfObject);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->transparent);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->retry);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->charset);
    CsrUtf8StringSer(buffer, length, primitive->folderName);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_REQ
void *CsrBtMapcPushMessageReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcPushMessageReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcPushMessageReq *) CsrPmemZalloc(sizeof(CsrBtMapcPushMessageReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->folderName), buffer, &offset, ((CsrSize) sizeof(primitive->folderName)));
    CsrUint32Des((CsrUint32 *) &primitive->lengthOfObject, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->transparent, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->retry, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->charset, buffer, &offset);
    CsrUtf8StringDes(&primitive->folderName, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_REQ
void CsrBtMapcPushMessageReqSerFree(void *msg)
{
    CsrBtMapcPushMessageReq *primitive;

    primitive = (CsrBtMapcPushMessageReq *) msg;

    if(primitive->folderName != NULL)
    {
        CsrPmemFree(primitive->folderName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_EVENT_NOTIFICATION_ABORT_IND
CsrSize CsrBtMapcEventNotificationAbortIndSizeof(void *msg)
{
    CsrBtMapcEventNotificationAbortInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_EVENT_NOTIFICATION_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_MAPC_EVENT_NOTIFICATION_ABORT_IND
CsrUint8 *CsrBtMapcEventNotificationAbortIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcEventNotificationAbortInd *primitive;

    primitive = (CsrBtMapcEventNotificationAbortInd *) msg;
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
#endif /* EXCLUDE_CSR_BT_MAPC_EVENT_NOTIFICATION_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_MAPC_EVENT_NOTIFICATION_ABORT_IND
void *CsrBtMapcEventNotificationAbortIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcEventNotificationAbortInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcEventNotificationAbortInd *) CsrPmemZalloc(sizeof(CsrBtMapcEventNotificationAbortInd));

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
#endif /* EXCLUDE_CSR_BT_MAPC_EVENT_NOTIFICATION_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_MAPC_EVENT_NOTIFICATION_ABORT_IND
void CsrBtMapcEventNotificationAbortIndSerFree(void *msg)
{
    CsrBtMapcEventNotificationAbortInd *primitive;

    primitive = (CsrBtMapcEventNotificationAbortInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_MAPC_EVENT_NOTIFICATION_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_MAPC_SELECT_MAS_INSTANCE_RES
CsrSize CsrBtMapcSelectMasInstanceResSizeof(void *msg)
{
    CsrBtMapcSelectMasInstanceRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_SELECT_MAS_INSTANCE_RES */

#ifndef EXCLUDE_CSR_BT_MAPC_SELECT_MAS_INSTANCE_RES
CsrUint8 *CsrBtMapcSelectMasInstanceResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcSelectMasInstanceRes *primitive;

    primitive = (CsrBtMapcSelectMasInstanceRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->proceedWithConnection);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->masInstanceId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPC_SELECT_MAS_INSTANCE_RES */

#ifndef EXCLUDE_CSR_BT_MAPC_SELECT_MAS_INSTANCE_RES
void *CsrBtMapcSelectMasInstanceResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcSelectMasInstanceRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcSelectMasInstanceRes *) CsrPmemZalloc(sizeof(CsrBtMapcSelectMasInstanceRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->proceedWithConnection, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->masInstanceId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPC_SELECT_MAS_INSTANCE_RES */

#ifndef EXCLUDE_CSR_BT_MAPC_SET_BACK_FOLDER_CFM
CsrSize CsrBtMapcSetBackFolderCfmSizeof(void *msg)
{
    CsrBtMapcSetBackFolderCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MAPC_SET_BACK_FOLDER_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_SET_BACK_FOLDER_CFM
CsrUint8 *CsrBtMapcSetBackFolderCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMapcSetBackFolderCfm *primitive;

    primitive = (CsrBtMapcSetBackFolderCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->instanceId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MAPC_SET_BACK_FOLDER_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_SET_BACK_FOLDER_CFM
void *CsrBtMapcSetBackFolderCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMapcSetBackFolderCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMapcSetBackFolderCfm *) CsrPmemZalloc(sizeof(CsrBtMapcSetBackFolderCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->instanceId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MAPC_SET_BACK_FOLDER_CFM */

#endif /* EXCLUDE_CSR_BT_MAPC_MODULE */
