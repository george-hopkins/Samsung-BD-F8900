#ifndef CSR_BT_MAPC_SERIALIZE_H__
#define CSR_BT_MAPC_SERIALIZE_H__
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

void CsrBtMapcPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_MAPC_SECURITY_OUT_CFM
CsrSize CsrBtMapcSecurityOutCfmSizeof(void *msg);
CsrUint8 *CsrBtMapcSecurityOutCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcSecurityOutCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapcSecurityOutCfmSerFree CsrBtMapcPfree
#endif /* EXCLUDE_CSR_BT_MAPC_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_UPDATE_INBOX_CFM
CsrSize CsrBtMapcUpdateInboxCfmSizeof(void *msg);
CsrUint8 *CsrBtMapcUpdateInboxCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcUpdateInboxCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapcUpdateInboxCfmSerFree CsrBtMapcPfree
#endif /* EXCLUDE_CSR_BT_MAPC_UPDATE_INBOX_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_CFM
CsrSize CsrBtMapcGetFolderListingCfmSizeof(void *msg);
CsrUint8 *CsrBtMapcGetFolderListingCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcGetFolderListingCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtMapcGetFolderListingCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_RES
CsrSize CsrBtMapcGetMessageListingResSizeof(void *msg);
CsrUint8 *CsrBtMapcGetMessageListingResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcGetMessageListingResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapcGetMessageListingResSerFree CsrBtMapcPfree
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_RES */

#ifndef EXCLUDE_CSR_BT_MAPC_SET_ROOT_FOLDER_REQ
CsrSize CsrBtMapcSetRootFolderReqSizeof(void *msg);
CsrUint8 *CsrBtMapcSetRootFolderReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcSetRootFolderReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapcSetRootFolderReqSerFree CsrBtMapcPfree
#endif /* EXCLUDE_CSR_BT_MAPC_SET_ROOT_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_UPDATE_INBOX_REQ
CsrSize CsrBtMapcUpdateInboxReqSizeof(void *msg);
CsrUint8 *CsrBtMapcUpdateInboxReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcUpdateInboxReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapcUpdateInboxReqSerFree CsrBtMapcPfree
#endif /* EXCLUDE_CSR_BT_MAPC_UPDATE_INBOX_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_REGISTER_QID_REQ
CsrSize CsrBtMapcRegisterQidReqSizeof(void *msg);
CsrUint8 *CsrBtMapcRegisterQidReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcRegisterQidReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapcRegisterQidReqSerFree CsrBtMapcPfree
#endif /* EXCLUDE_CSR_BT_MAPC_REGISTER_QID_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_NOTIFICATION_REGISTRATION_CFM
CsrSize CsrBtMapcNotificationRegistrationCfmSizeof(void *msg);
CsrUint8 *CsrBtMapcNotificationRegistrationCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcNotificationRegistrationCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapcNotificationRegistrationCfmSerFree CsrBtMapcPfree
#endif /* EXCLUDE_CSR_BT_MAPC_NOTIFICATION_REGISTRATION_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_SECURITY_IN_REQ
CsrSize CsrBtMapcSecurityInReqSizeof(void *msg);
CsrUint8 *CsrBtMapcSecurityInReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcSecurityInReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapcSecurityInReqSerFree CsrBtMapcPfree
#endif /* EXCLUDE_CSR_BT_MAPC_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_SECURITY_OUT_REQ
CsrSize CsrBtMapcSecurityOutReqSizeof(void *msg);
CsrUint8 *CsrBtMapcSecurityOutReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcSecurityOutReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapcSecurityOutReqSerFree CsrBtMapcPfree
#endif /* EXCLUDE_CSR_BT_MAPC_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_INSTANCE_IDS_CFM
CsrSize CsrBtMapcGetInstanceIdsCfmSizeof(void *msg);
CsrUint8 *CsrBtMapcGetInstanceIdsCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcGetInstanceIdsCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtMapcGetInstanceIdsCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_MAPC_GET_INSTANCE_IDS_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_REQ
CsrSize CsrBtMapcGetMessageReqSizeof(void *msg);
CsrUint8 *CsrBtMapcGetMessageReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcGetMessageReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtMapcGetMessageReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_DISCONNECT_REQ
CsrSize CsrBtMapcDisconnectReqSizeof(void *msg);
CsrUint8 *CsrBtMapcDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcDisconnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapcDisconnectReqSerFree CsrBtMapcPfree
#endif /* EXCLUDE_CSR_BT_MAPC_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_EVENT_NOTIFICATION_IND
CsrSize CsrBtMapcEventNotificationIndSizeof(void *msg);
CsrUint8 *CsrBtMapcEventNotificationIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcEventNotificationIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtMapcEventNotificationIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_MAPC_EVENT_NOTIFICATION_IND */

#ifndef EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_CFM
CsrSize CsrBtMapcPushMessageCfmSizeof(void *msg);
CsrUint8 *CsrBtMapcPushMessageCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcPushMessageCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtMapcPushMessageCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_EVENT_NOTIFICATION_RES
CsrSize CsrBtMapcEventNotificationResSizeof(void *msg);
CsrUint8 *CsrBtMapcEventNotificationResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcEventNotificationResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapcEventNotificationResSerFree CsrBtMapcPfree
#endif /* EXCLUDE_CSR_BT_MAPC_EVENT_NOTIFICATION_RES */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_CFM
CsrSize CsrBtMapcGetMessageCfmSizeof(void *msg);
CsrUint8 *CsrBtMapcGetMessageCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcGetMessageCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtMapcGetMessageCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_IND
CsrSize CsrBtMapcGetMessageListingIndSizeof(void *msg);
CsrUint8 *CsrBtMapcGetMessageListingIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcGetMessageListingIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtMapcGetMessageListingIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_IND */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_INSTANCE_IDS_REQ
CsrSize CsrBtMapcGetInstanceIdsReqSizeof(void *msg);
CsrUint8 *CsrBtMapcGetInstanceIdsReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcGetInstanceIdsReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapcGetInstanceIdsReqSerFree CsrBtMapcPfree
#endif /* EXCLUDE_CSR_BT_MAPC_GET_INSTANCE_IDS_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_RES
CsrSize CsrBtMapcGetMessageResSizeof(void *msg);
CsrUint8 *CsrBtMapcGetMessageResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcGetMessageResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapcGetMessageResSerFree CsrBtMapcPfree
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_RES */

#ifndef EXCLUDE_CSR_BT_MAPC_SELECT_MAS_INSTANCE_IND
CsrSize CsrBtMapcSelectMasInstanceIndSizeof(void *msg);
CsrUint8 *CsrBtMapcSelectMasInstanceIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcSelectMasInstanceIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtMapcSelectMasInstanceIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_MAPC_SELECT_MAS_INSTANCE_IND */

#ifndef EXCLUDE_CSR_BT_MAPC_NOTIFICATION_REGISTRATION_OFF_IND
CsrSize CsrBtMapcNotificationRegistrationOffIndSizeof(void *msg);
CsrUint8 *CsrBtMapcNotificationRegistrationOffIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcNotificationRegistrationOffIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapcNotificationRegistrationOffIndSerFree CsrBtMapcPfree
#endif /* EXCLUDE_CSR_BT_MAPC_NOTIFICATION_REGISTRATION_OFF_IND */

#ifndef EXCLUDE_CSR_BT_MAPC_ABORT_REQ
CsrSize CsrBtMapcAbortReqSizeof(void *msg);
CsrUint8 *CsrBtMapcAbortReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcAbortReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapcAbortReqSerFree CsrBtMapcPfree
#endif /* EXCLUDE_CSR_BT_MAPC_ABORT_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_SET_ROOT_FOLDER_CFM
CsrSize CsrBtMapcSetRootFolderCfmSizeof(void *msg);
CsrUint8 *CsrBtMapcSetRootFolderCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcSetRootFolderCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapcSetRootFolderCfmSerFree CsrBtMapcPfree
#endif /* EXCLUDE_CSR_BT_MAPC_SET_ROOT_FOLDER_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_CFM
CsrSize CsrBtMapcGetMessageListingCfmSizeof(void *msg);
CsrUint8 *CsrBtMapcGetMessageListingCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcGetMessageListingCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtMapcGetMessageListingCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_SET_MESSAGE_STATUS_CFM
CsrSize CsrBtMapcSetMessageStatusCfmSizeof(void *msg);
CsrUint8 *CsrBtMapcSetMessageStatusCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcSetMessageStatusCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapcSetMessageStatusCfmSerFree CsrBtMapcPfree
#endif /* EXCLUDE_CSR_BT_MAPC_SET_MESSAGE_STATUS_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_IND
CsrSize CsrBtMapcPushMessageIndSizeof(void *msg);
CsrUint8 *CsrBtMapcPushMessageIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcPushMessageIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapcPushMessageIndSerFree CsrBtMapcPfree
#endif /* EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_IND */

#ifndef EXCLUDE_CSR_BT_MAPC_NOTIFICATION_REGISTRATION_REQ
CsrSize CsrBtMapcNotificationRegistrationReqSizeof(void *msg);
CsrUint8 *CsrBtMapcNotificationRegistrationReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcNotificationRegistrationReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapcNotificationRegistrationReqSerFree CsrBtMapcPfree
#endif /* EXCLUDE_CSR_BT_MAPC_NOTIFICATION_REGISTRATION_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_CANCEL_CONNECT_REQ
CsrSize CsrBtMapcCancelConnectReqSizeof(void *msg);
CsrUint8 *CsrBtMapcCancelConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcCancelConnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapcCancelConnectReqSerFree CsrBtMapcPfree
#endif /* EXCLUDE_CSR_BT_MAPC_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_IND
CsrSize CsrBtMapcGetMessageIndSizeof(void *msg);
CsrUint8 *CsrBtMapcGetMessageIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcGetMessageIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtMapcGetMessageIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_IND */

#ifndef EXCLUDE_CSR_BT_MAPC_SET_BACK_FOLDER_REQ
CsrSize CsrBtMapcSetBackFolderReqSizeof(void *msg);
CsrUint8 *CsrBtMapcSetBackFolderReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcSetBackFolderReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapcSetBackFolderReqSerFree CsrBtMapcPfree
#endif /* EXCLUDE_CSR_BT_MAPC_SET_BACK_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_IND
CsrSize CsrBtMapcGetFolderListingIndSizeof(void *msg);
CsrUint8 *CsrBtMapcGetFolderListingIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcGetFolderListingIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtMapcGetFolderListingIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_IND */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_REQ
CsrSize CsrBtMapcGetMessageListingReqSizeof(void *msg);
CsrUint8 *CsrBtMapcGetMessageListingReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcGetMessageListingReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtMapcGetMessageListingReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_CONNECT_CFM
CsrSize CsrBtMapcConnectCfmSizeof(void *msg);
CsrUint8 *CsrBtMapcConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcConnectCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapcConnectCfmSerFree CsrBtMapcPfree
#endif /* EXCLUDE_CSR_BT_MAPC_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_CONNECT_REQ
CsrSize CsrBtMapcConnectReqSizeof(void *msg);
CsrUint8 *CsrBtMapcConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcConnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapcConnectReqSerFree CsrBtMapcPfree
#endif /* EXCLUDE_CSR_BT_MAPC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_SET_FOLDER_CFM
CsrSize CsrBtMapcSetFolderCfmSizeof(void *msg);
CsrUint8 *CsrBtMapcSetFolderCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcSetFolderCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapcSetFolderCfmSerFree CsrBtMapcPfree
#endif /* EXCLUDE_CSR_BT_MAPC_SET_FOLDER_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_RES
CsrSize CsrBtMapcPushMessageResSizeof(void *msg);
CsrUint8 *CsrBtMapcPushMessageResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcPushMessageResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtMapcPushMessageResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_RES */

#ifndef EXCLUDE_CSR_BT_MAPC_SET_MESSAGE_STATUS_REQ
CsrSize CsrBtMapcSetMessageStatusReqSizeof(void *msg);
CsrUint8 *CsrBtMapcSetMessageStatusReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcSetMessageStatusReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtMapcSetMessageStatusReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_MAPC_SET_MESSAGE_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_DISCONNECT_IND
CsrSize CsrBtMapcDisconnectIndSizeof(void *msg);
CsrUint8 *CsrBtMapcDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcDisconnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapcDisconnectIndSerFree CsrBtMapcPfree
#endif /* EXCLUDE_CSR_BT_MAPC_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_MAPC_SET_FOLDER_REQ
CsrSize CsrBtMapcSetFolderReqSizeof(void *msg);
CsrUint8 *CsrBtMapcSetFolderReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcSetFolderReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtMapcSetFolderReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_MAPC_SET_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_RES
CsrSize CsrBtMapcGetFolderListingResSizeof(void *msg);
CsrUint8 *CsrBtMapcGetFolderListingResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcGetFolderListingResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapcGetFolderListingResSerFree CsrBtMapcPfree
#endif /* EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_RES */

#ifndef EXCLUDE_CSR_BT_MAPC_ABORT_CFM
CsrSize CsrBtMapcAbortCfmSizeof(void *msg);
CsrUint8 *CsrBtMapcAbortCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcAbortCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapcAbortCfmSerFree CsrBtMapcPfree
#endif /* EXCLUDE_CSR_BT_MAPC_ABORT_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_REQ
CsrSize CsrBtMapcGetFolderListingReqSizeof(void *msg);
CsrUint8 *CsrBtMapcGetFolderListingReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcGetFolderListingReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapcGetFolderListingReqSerFree CsrBtMapcPfree
#endif /* EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_SECURITY_IN_CFM
CsrSize CsrBtMapcSecurityInCfmSizeof(void *msg);
CsrUint8 *CsrBtMapcSecurityInCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcSecurityInCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapcSecurityInCfmSerFree CsrBtMapcPfree
#endif /* EXCLUDE_CSR_BT_MAPC_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_REQ
CsrSize CsrBtMapcPushMessageReqSizeof(void *msg);
CsrUint8 *CsrBtMapcPushMessageReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcPushMessageReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtMapcPushMessageReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_REQ */

#ifndef EXCLUDE_CSR_BT_MAPC_EVENT_NOTIFICATION_ABORT_IND
CsrSize CsrBtMapcEventNotificationAbortIndSizeof(void *msg);
CsrUint8 *CsrBtMapcEventNotificationAbortIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcEventNotificationAbortIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtMapcEventNotificationAbortIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_MAPC_EVENT_NOTIFICATION_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_MAPC_SELECT_MAS_INSTANCE_RES
CsrSize CsrBtMapcSelectMasInstanceResSizeof(void *msg);
CsrUint8 *CsrBtMapcSelectMasInstanceResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcSelectMasInstanceResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapcSelectMasInstanceResSerFree CsrBtMapcPfree
#endif /* EXCLUDE_CSR_BT_MAPC_SELECT_MAS_INSTANCE_RES */

#ifndef EXCLUDE_CSR_BT_MAPC_SET_BACK_FOLDER_CFM
CsrSize CsrBtMapcSetBackFolderCfmSizeof(void *msg);
CsrUint8 *CsrBtMapcSetBackFolderCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMapcSetBackFolderCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMapcSetBackFolderCfmSerFree CsrBtMapcPfree
#endif /* EXCLUDE_CSR_BT_MAPC_SET_BACK_FOLDER_CFM */

#ifdef __cplusplus
}
#endif

#endif
