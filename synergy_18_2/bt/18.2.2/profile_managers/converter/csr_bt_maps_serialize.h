#ifndef CSR_BT_MAPS_SERIALIZE_H__
#define CSR_BT_MAPS_SERIALIZE_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_types.h"

#include "csr_synergy.h"
#ifdef __cplusplus
extern "C" {
#endif

void CsrBtMapsPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_MAPS_SET_FOLDER_RES
CsrSize CsrBtMapsSetFolderResSizeof(void *msg);
CsrUint8 *CsrBtMapsSetFolderResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsSetFolderResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapsSetFolderResSerFree CsrBtMapsPfree
#endif /* EXCLUDE_CSR_BT_MAPS_SET_FOLDER_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_ACTIVATE_CFM
CsrSize CsrBtMapsActivateCfmSizeof(void *msg);
CsrUint8 *CsrBtMapsActivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsActivateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapsActivateCfmSerFree CsrBtMapsPfree
#endif /* EXCLUDE_CSR_BT_MAPS_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_MAPS_SELECT_MAS_INSTANCE_IND
CsrSize CsrBtMapsSelectMasInstanceIndSizeof(void *msg);
CsrUint8 *CsrBtMapsSelectMasInstanceIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsSelectMasInstanceIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtMapsSelectMasInstanceIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_MAPS_SELECT_MAS_INSTANCE_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_HEADER_RES
CsrSize CsrBtMapsGetMessageHeaderResSizeof(void *msg);
CsrUint8 *CsrBtMapsGetMessageHeaderResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsGetMessageHeaderResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapsGetMessageHeaderResSerFree CsrBtMapsPfree
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_HEADER_IND
CsrSize CsrBtMapsGetMessageListingHeaderIndSizeof(void *msg);
CsrUint8 *CsrBtMapsGetMessageListingHeaderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsGetMessageListingHeaderIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtMapsGetMessageListingHeaderIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_IND
CsrSize CsrBtMapsGetMessageListingIndSizeof(void *msg);
CsrUint8 *CsrBtMapsGetMessageListingIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsGetMessageListingIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapsGetMessageListingIndSerFree CsrBtMapsPfree
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_NOTIFICATION_REGISTRATION_RES
CsrSize CsrBtMapsNotificationRegistrationResSizeof(void *msg);
CsrUint8 *CsrBtMapsNotificationRegistrationResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsNotificationRegistrationResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapsNotificationRegistrationResSerFree CsrBtMapsPfree
#endif /* EXCLUDE_CSR_BT_MAPS_NOTIFICATION_REGISTRATION_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_SET_ROOT_FOLDER_IND
CsrSize CsrBtMapsSetRootFolderIndSizeof(void *msg);
CsrUint8 *CsrBtMapsSetRootFolderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsSetRootFolderIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapsSetRootFolderIndSerFree CsrBtMapsPfree
#endif /* EXCLUDE_CSR_BT_MAPS_SET_ROOT_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_SET_FOLDER_IND
CsrSize CsrBtMapsSetFolderIndSizeof(void *msg);
CsrUint8 *CsrBtMapsSetFolderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsSetFolderIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtMapsSetFolderIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_MAPS_SET_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_IND
CsrSize CsrBtMapsPushMessageIndSizeof(void *msg);
CsrUint8 *CsrBtMapsPushMessageIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsPushMessageIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtMapsPushMessageIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_NOTIFICATION_REGISTRATION_COMPLETE_IND
CsrSize CsrBtMapsNotificationRegistrationCompleteIndSizeof(void *msg);
CsrUint8 *CsrBtMapsNotificationRegistrationCompleteIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsNotificationRegistrationCompleteIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapsNotificationRegistrationCompleteIndSerFree CsrBtMapsPfree
#endif /* EXCLUDE_CSR_BT_MAPS_NOTIFICATION_REGISTRATION_COMPLETE_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_SELECT_MAS_INSTANCE_RES
CsrSize CsrBtMapsSelectMasInstanceResSizeof(void *msg);
CsrUint8 *CsrBtMapsSelectMasInstanceResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsSelectMasInstanceResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapsSelectMasInstanceResSerFree CsrBtMapsPfree
#endif /* EXCLUDE_CSR_BT_MAPS_SELECT_MAS_INSTANCE_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_FOLDER_LISTING_HEADER_RES
CsrSize CsrBtMapsGetFolderListingHeaderResSizeof(void *msg);
CsrUint8 *CsrBtMapsGetFolderListingHeaderResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsGetFolderListingHeaderResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapsGetFolderListingHeaderResSerFree CsrBtMapsPfree
#endif /* EXCLUDE_CSR_BT_MAPS_GET_FOLDER_LISTING_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_HEADER_IND
CsrSize CsrBtMapsPushMessageHeaderIndSizeof(void *msg);
CsrUint8 *CsrBtMapsPushMessageHeaderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsPushMessageHeaderIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtMapsPushMessageHeaderIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_HEADER_RES
CsrSize CsrBtMapsGetMessageListingHeaderResSizeof(void *msg);
CsrUint8 *CsrBtMapsGetMessageListingHeaderResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsGetMessageListingHeaderResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtMapsGetMessageListingHeaderResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_SECURITY_LEVEL_REQ
CsrSize CsrBtMapsSecurityLevelReqSizeof(void *msg);
CsrUint8 *CsrBtMapsSecurityLevelReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsSecurityLevelReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapsSecurityLevelReqSerFree CsrBtMapsPfree
#endif /* EXCLUDE_CSR_BT_MAPS_SECURITY_LEVEL_REQ */

#ifndef EXCLUDE_CSR_BT_MAPS_SET_ROOT_FOLDER_RES
CsrSize CsrBtMapsSetRootFolderResSizeof(void *msg);
CsrUint8 *CsrBtMapsSetRootFolderResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsSetRootFolderResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapsSetRootFolderResSerFree CsrBtMapsPfree
#endif /* EXCLUDE_CSR_BT_MAPS_SET_ROOT_FOLDER_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_EVENT_NOTIFICATION_CFM
CsrSize CsrBtMapsEventNotificationCfmSizeof(void *msg);
CsrUint8 *CsrBtMapsEventNotificationCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsEventNotificationCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapsEventNotificationCfmSerFree CsrBtMapsPfree
#endif /* EXCLUDE_CSR_BT_MAPS_EVENT_NOTIFICATION_CFM */

#ifndef EXCLUDE_CSR_BT_MAPS_SET_BACK_FOLDER_RES
CsrSize CsrBtMapsSetBackFolderResSizeof(void *msg);
CsrUint8 *CsrBtMapsSetBackFolderResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsSetBackFolderResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapsSetBackFolderResSerFree CsrBtMapsPfree
#endif /* EXCLUDE_CSR_BT_MAPS_SET_BACK_FOLDER_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_NOTIFICATION_REGISTRATION_IND
CsrSize CsrBtMapsNotificationRegistrationIndSizeof(void *msg);
CsrUint8 *CsrBtMapsNotificationRegistrationIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsNotificationRegistrationIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapsNotificationRegistrationIndSerFree CsrBtMapsPfree
#endif /* EXCLUDE_CSR_BT_MAPS_NOTIFICATION_REGISTRATION_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_DEACTIVATE_REQ
CsrSize CsrBtMapsDeactivateReqSizeof(void *msg);
CsrUint8 *CsrBtMapsDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsDeactivateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapsDeactivateReqSerFree CsrBtMapsPfree
#endif /* EXCLUDE_CSR_BT_MAPS_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_MAPS_EVENT_NOTIFICATION_IND
CsrSize CsrBtMapsEventNotificationIndSizeof(void *msg);
CsrUint8 *CsrBtMapsEventNotificationIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsEventNotificationIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapsEventNotificationIndSerFree CsrBtMapsPfree
#endif /* EXCLUDE_CSR_BT_MAPS_EVENT_NOTIFICATION_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_HEADER_IND
CsrSize CsrBtMapsGetMessageHeaderIndSizeof(void *msg);
CsrUint8 *CsrBtMapsGetMessageHeaderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsGetMessageHeaderIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtMapsGetMessageHeaderIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_CONNECT_IND
CsrSize CsrBtMapsConnectIndSizeof(void *msg);
CsrUint8 *CsrBtMapsConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsConnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapsConnectIndSerFree CsrBtMapsPfree
#endif /* EXCLUDE_CSR_BT_MAPS_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_SET_MESSAGE_STATUS_RES
CsrSize CsrBtMapsSetMessageStatusResSizeof(void *msg);
CsrUint8 *CsrBtMapsSetMessageStatusResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsSetMessageStatusResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapsSetMessageStatusResSerFree CsrBtMapsPfree
#endif /* EXCLUDE_CSR_BT_MAPS_SET_MESSAGE_STATUS_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_INSTANCE_IDS_CFM
CsrSize CsrBtMapsGetInstanceIdsCfmSizeof(void *msg);
CsrUint8 *CsrBtMapsGetInstanceIdsCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsGetInstanceIdsCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtMapsGetInstanceIdsCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_MAPS_GET_INSTANCE_IDS_CFM */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_FOLDER_LISTING_IND
CsrSize CsrBtMapsGetFolderListingIndSizeof(void *msg);
CsrUint8 *CsrBtMapsGetFolderListingIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsGetFolderListingIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapsGetFolderListingIndSerFree CsrBtMapsPfree
#endif /* EXCLUDE_CSR_BT_MAPS_GET_FOLDER_LISTING_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_SET_BACK_FOLDER_IND
CsrSize CsrBtMapsSetBackFolderIndSizeof(void *msg);
CsrUint8 *CsrBtMapsSetBackFolderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsSetBackFolderIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtMapsSetBackFolderIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_MAPS_SET_BACK_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_IND
CsrSize CsrBtMapsGetMessageIndSizeof(void *msg);
CsrUint8 *CsrBtMapsGetMessageIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsGetMessageIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapsGetMessageIndSerFree CsrBtMapsPfree
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_SECURITY_LEVEL_CFM
CsrSize CsrBtMapsSecurityLevelCfmSizeof(void *msg);
CsrUint8 *CsrBtMapsSecurityLevelCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsSecurityLevelCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapsSecurityLevelCfmSerFree CsrBtMapsPfree
#endif /* EXCLUDE_CSR_BT_MAPS_SECURITY_LEVEL_CFM */

#ifndef EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_HEADER_RES
CsrSize CsrBtMapsPushMessageHeaderResSizeof(void *msg);
CsrUint8 *CsrBtMapsPushMessageHeaderResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsPushMessageHeaderResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtMapsPushMessageHeaderResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_SET_MESSAGE_STATUS_IND
CsrSize CsrBtMapsSetMessageStatusIndSizeof(void *msg);
CsrUint8 *CsrBtMapsSetMessageStatusIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsSetMessageStatusIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtMapsSetMessageStatusIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_MAPS_SET_MESSAGE_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_FOLDER_LISTING_HEADER_IND
CsrSize CsrBtMapsGetFolderListingHeaderIndSizeof(void *msg);
CsrUint8 *CsrBtMapsGetFolderListingHeaderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsGetFolderListingHeaderIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapsGetFolderListingHeaderIndSerFree CsrBtMapsPfree
#endif /* EXCLUDE_CSR_BT_MAPS_GET_FOLDER_LISTING_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_FOLDER_LISTING_RES
CsrSize CsrBtMapsGetFolderListingResSizeof(void *msg);
CsrUint8 *CsrBtMapsGetFolderListingResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsGetFolderListingResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtMapsGetFolderListingResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_MAPS_GET_FOLDER_LISTING_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_RES
CsrSize CsrBtMapsPushMessageResSizeof(void *msg);
CsrUint8 *CsrBtMapsPushMessageResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsPushMessageResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtMapsPushMessageResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_CONNECT_RES
CsrSize CsrBtMapsConnectResSizeof(void *msg);
CsrUint8 *CsrBtMapsConnectResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsConnectResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapsConnectResSerFree CsrBtMapsPfree
#endif /* EXCLUDE_CSR_BT_MAPS_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_INSTANCE_IDS_REQ
CsrSize CsrBtMapsGetInstanceIdsReqSizeof(void *msg);
CsrUint8 *CsrBtMapsGetInstanceIdsReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsGetInstanceIdsReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapsGetInstanceIdsReqSerFree CsrBtMapsPfree
#endif /* EXCLUDE_CSR_BT_MAPS_GET_INSTANCE_IDS_REQ */

#ifndef EXCLUDE_CSR_BT_MAPS_ACTIVATE_REQ
CsrSize CsrBtMapsActivateReqSizeof(void *msg);
CsrUint8 *CsrBtMapsActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsActivateReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtMapsActivateReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_MAPS_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_MAPS_DISCONNECT_IND
CsrSize CsrBtMapsDisconnectIndSizeof(void *msg);
CsrUint8 *CsrBtMapsDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsDisconnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapsDisconnectIndSerFree CsrBtMapsPfree
#endif /* EXCLUDE_CSR_BT_MAPS_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_DEACTIVATE_CFM
CsrSize CsrBtMapsDeactivateCfmSizeof(void *msg);
CsrUint8 *CsrBtMapsDeactivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsDeactivateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapsDeactivateCfmSerFree CsrBtMapsPfree
#endif /* EXCLUDE_CSR_BT_MAPS_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_MAPS_REGISTER_QID_REQ
CsrSize CsrBtMapsRegisterQidReqSizeof(void *msg);
CsrUint8 *CsrBtMapsRegisterQidReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsRegisterQidReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapsRegisterQidReqSerFree CsrBtMapsPfree
#endif /* EXCLUDE_CSR_BT_MAPS_REGISTER_QID_REQ */

#ifndef EXCLUDE_CSR_BT_MAPS_UPDATE_INBOX_RES
CsrSize CsrBtMapsUpdateInboxResSizeof(void *msg);
CsrUint8 *CsrBtMapsUpdateInboxResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsUpdateInboxResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapsUpdateInboxResSerFree CsrBtMapsPfree
#endif /* EXCLUDE_CSR_BT_MAPS_UPDATE_INBOX_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_EVENT_NOTIFICATION_RES
CsrSize CsrBtMapsEventNotificationResSizeof(void *msg);
CsrUint8 *CsrBtMapsEventNotificationResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsEventNotificationResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtMapsEventNotificationResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_MAPS_EVENT_NOTIFICATION_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_RES
CsrSize CsrBtMapsGetMessageResSizeof(void *msg);
CsrUint8 *CsrBtMapsGetMessageResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsGetMessageResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtMapsGetMessageResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_RES
CsrSize CsrBtMapsGetMessageListingResSizeof(void *msg);
CsrUint8 *CsrBtMapsGetMessageListingResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsGetMessageListingResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtMapsGetMessageListingResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_RES */

#ifndef EXCLUDE_CSR_BT_MAPS_ABORT_IND
CsrSize CsrBtMapsAbortIndSizeof(void *msg);
CsrUint8 *CsrBtMapsAbortIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsAbortIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtMapsAbortIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_MAPS_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_MAPS_EVENT_NOTIFICATION_REQ
CsrSize CsrBtMapsEventNotificationReqSizeof(void *msg);
CsrUint8 *CsrBtMapsEventNotificationReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsEventNotificationReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapsEventNotificationReqSerFree CsrBtMapsPfree
#endif /* EXCLUDE_CSR_BT_MAPS_EVENT_NOTIFICATION_REQ */

#ifndef EXCLUDE_CSR_BT_MAPS_UPDATE_INBOX_IND
CsrSize CsrBtMapsUpdateInboxIndSizeof(void *msg);
CsrUint8 *CsrBtMapsUpdateInboxIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapsUpdateInboxIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapsUpdateInboxIndSerFree CsrBtMapsPfree
#endif /* EXCLUDE_CSR_BT_MAPS_UPDATE_INBOX_IND */

#ifdef __cplusplus
}
#endif

#endif
