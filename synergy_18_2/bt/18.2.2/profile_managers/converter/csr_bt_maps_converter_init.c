/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_synergy.h"
#include "csr_msgconv.h"
#include "csr_pmem.h"
#include "csr_util.h"
#include "csr_bt_hand_coded.h"
#include "csr_bt_maps_converter_init.h"
#ifndef EXCLUDE_CSR_BT_MAPS_MODULE
#include "csr_bt_maps_serialize.h"
#include "csr_bt_maps_prim.h"

static CsrMsgConvMsgEntry csr_bt_maps_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_MAPS_ABORT_IND
    { CSR_BT_MAPS_ABORT_IND, CsrBtMapsAbortIndSizeof, CsrBtMapsAbortIndSer, CsrBtMapsAbortIndDes, CsrBtMapsAbortIndSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_ABORT_IND */
#ifndef EXCLUDE_CSR_BT_MAPS_ACTIVATE_CFM
    { CSR_BT_MAPS_ACTIVATE_CFM, CsrBtMapsActivateCfmSizeof, CsrBtMapsActivateCfmSer, CsrBtMapsActivateCfmDes, CsrBtMapsActivateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_ACTIVATE_CFM */
#ifndef EXCLUDE_CSR_BT_MAPS_ACTIVATE_REQ
    { CSR_BT_MAPS_ACTIVATE_REQ, CsrBtMapsActivateReqSizeof, CsrBtMapsActivateReqSer, CsrBtMapsActivateReqDes, CsrBtMapsActivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_ACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_MAPS_CONNECT_IND
    { CSR_BT_MAPS_CONNECT_IND, CsrBtMapsConnectIndSizeof, CsrBtMapsConnectIndSer, CsrBtMapsConnectIndDes, CsrBtMapsConnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_CONNECT_IND */
#ifndef EXCLUDE_CSR_BT_MAPS_CONNECT_RES
    { CSR_BT_MAPS_CONNECT_RES, CsrBtMapsConnectResSizeof, CsrBtMapsConnectResSer, CsrBtMapsConnectResDes, CsrBtMapsConnectResSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_CONNECT_RES */
#ifndef EXCLUDE_CSR_BT_MAPS_DEACTIVATE_CFM
    { CSR_BT_MAPS_DEACTIVATE_CFM, CsrBtMapsDeactivateCfmSizeof, CsrBtMapsDeactivateCfmSer, CsrBtMapsDeactivateCfmDes, CsrBtMapsDeactivateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_DEACTIVATE_CFM */
#ifndef EXCLUDE_CSR_BT_MAPS_DEACTIVATE_REQ
    { CSR_BT_MAPS_DEACTIVATE_REQ, CsrBtMapsDeactivateReqSizeof, CsrBtMapsDeactivateReqSer, CsrBtMapsDeactivateReqDes, CsrBtMapsDeactivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_DEACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_MAPS_DISCONNECT_IND
    { CSR_BT_MAPS_DISCONNECT_IND, CsrBtMapsDisconnectIndSizeof, CsrBtMapsDisconnectIndSer, CsrBtMapsDisconnectIndDes, CsrBtMapsDisconnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_DISCONNECT_IND */
#ifndef EXCLUDE_CSR_BT_MAPS_EVENT_NOTIFICATION_CFM
    { CSR_BT_MAPS_EVENT_NOTIFICATION_CFM, CsrBtMapsEventNotificationCfmSizeof, CsrBtMapsEventNotificationCfmSer, CsrBtMapsEventNotificationCfmDes, CsrBtMapsEventNotificationCfmSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_EVENT_NOTIFICATION_CFM */
#ifndef EXCLUDE_CSR_BT_MAPS_EVENT_NOTIFICATION_IND
    { CSR_BT_MAPS_EVENT_NOTIFICATION_IND, CsrBtMapsEventNotificationIndSizeof, CsrBtMapsEventNotificationIndSer, CsrBtMapsEventNotificationIndDes, CsrBtMapsEventNotificationIndSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_EVENT_NOTIFICATION_IND */
#ifndef EXCLUDE_CSR_BT_MAPS_EVENT_NOTIFICATION_REQ
    { CSR_BT_MAPS_EVENT_NOTIFICATION_REQ, CsrBtMapsEventNotificationReqSizeof, CsrBtMapsEventNotificationReqSer, CsrBtMapsEventNotificationReqDes, CsrBtMapsEventNotificationReqSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_EVENT_NOTIFICATION_REQ */
#ifndef EXCLUDE_CSR_BT_MAPS_EVENT_NOTIFICATION_RES
    { CSR_BT_MAPS_EVENT_NOTIFICATION_RES, CsrBtMapsEventNotificationResSizeof, CsrBtMapsEventNotificationResSer, CsrBtMapsEventNotificationResDes, CsrBtMapsEventNotificationResSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_EVENT_NOTIFICATION_RES */
#ifndef EXCLUDE_CSR_BT_MAPS_GET_FOLDER_LISTING_HEADER_IND
    { CSR_BT_MAPS_GET_FOLDER_LISTING_HEADER_IND, CsrBtMapsGetFolderListingHeaderIndSizeof, CsrBtMapsGetFolderListingHeaderIndSer, CsrBtMapsGetFolderListingHeaderIndDes, CsrBtMapsGetFolderListingHeaderIndSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_GET_FOLDER_LISTING_HEADER_IND */
#ifndef EXCLUDE_CSR_BT_MAPS_GET_FOLDER_LISTING_HEADER_RES
    { CSR_BT_MAPS_GET_FOLDER_LISTING_HEADER_RES, CsrBtMapsGetFolderListingHeaderResSizeof, CsrBtMapsGetFolderListingHeaderResSer, CsrBtMapsGetFolderListingHeaderResDes, CsrBtMapsGetFolderListingHeaderResSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_GET_FOLDER_LISTING_HEADER_RES */
#ifndef EXCLUDE_CSR_BT_MAPS_GET_FOLDER_LISTING_IND
    { CSR_BT_MAPS_GET_FOLDER_LISTING_IND, CsrBtMapsGetFolderListingIndSizeof, CsrBtMapsGetFolderListingIndSer, CsrBtMapsGetFolderListingIndDes, CsrBtMapsGetFolderListingIndSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_GET_FOLDER_LISTING_IND */
#ifndef EXCLUDE_CSR_BT_MAPS_GET_FOLDER_LISTING_RES
    { CSR_BT_MAPS_GET_FOLDER_LISTING_RES, CsrBtMapsGetFolderListingResSizeof, CsrBtMapsGetFolderListingResSer, CsrBtMapsGetFolderListingResDes, CsrBtMapsGetFolderListingResSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_GET_FOLDER_LISTING_RES */
#ifndef EXCLUDE_CSR_BT_MAPS_GET_INSTANCE_IDS_CFM
    { CSR_BT_MAPS_GET_INSTANCE_IDS_CFM, CsrBtMapsGetInstanceIdsCfmSizeof, CsrBtMapsGetInstanceIdsCfmSer, CsrBtMapsGetInstanceIdsCfmDes, CsrBtMapsGetInstanceIdsCfmSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_GET_INSTANCE_IDS_CFM */
#ifndef EXCLUDE_CSR_BT_MAPS_GET_INSTANCE_IDS_REQ
    { CSR_BT_MAPS_GET_INSTANCE_IDS_REQ, CsrBtMapsGetInstanceIdsReqSizeof, CsrBtMapsGetInstanceIdsReqSer, CsrBtMapsGetInstanceIdsReqDes, CsrBtMapsGetInstanceIdsReqSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_GET_INSTANCE_IDS_REQ */
#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_HEADER_IND
    { CSR_BT_MAPS_GET_MESSAGE_HEADER_IND, CsrBtMapsGetMessageHeaderIndSizeof, CsrBtMapsGetMessageHeaderIndSer, CsrBtMapsGetMessageHeaderIndDes, CsrBtMapsGetMessageHeaderIndSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_HEADER_IND */
#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_HEADER_RES
    { CSR_BT_MAPS_GET_MESSAGE_HEADER_RES, CsrBtMapsGetMessageHeaderResSizeof, CsrBtMapsGetMessageHeaderResSer, CsrBtMapsGetMessageHeaderResDes, CsrBtMapsGetMessageHeaderResSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_HEADER_RES */
#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_IND
    { CSR_BT_MAPS_GET_MESSAGE_IND, CsrBtMapsGetMessageIndSizeof, CsrBtMapsGetMessageIndSer, CsrBtMapsGetMessageIndDes, CsrBtMapsGetMessageIndSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_IND */
#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_HEADER_IND
    { CSR_BT_MAPS_GET_MESSAGE_LISTING_HEADER_IND, CsrBtMapsGetMessageListingHeaderIndSizeof, CsrBtMapsGetMessageListingHeaderIndSer, CsrBtMapsGetMessageListingHeaderIndDes, CsrBtMapsGetMessageListingHeaderIndSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_HEADER_IND */
#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_HEADER_RES
    { CSR_BT_MAPS_GET_MESSAGE_LISTING_HEADER_RES, CsrBtMapsGetMessageListingHeaderResSizeof, CsrBtMapsGetMessageListingHeaderResSer, CsrBtMapsGetMessageListingHeaderResDes, CsrBtMapsGetMessageListingHeaderResSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_HEADER_RES */
#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_IND
    { CSR_BT_MAPS_GET_MESSAGE_LISTING_IND, CsrBtMapsGetMessageListingIndSizeof, CsrBtMapsGetMessageListingIndSer, CsrBtMapsGetMessageListingIndDes, CsrBtMapsGetMessageListingIndSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_IND */
#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_RES
    { CSR_BT_MAPS_GET_MESSAGE_LISTING_RES, CsrBtMapsGetMessageListingResSizeof, CsrBtMapsGetMessageListingResSer, CsrBtMapsGetMessageListingResDes, CsrBtMapsGetMessageListingResSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_RES */
#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_RES
    { CSR_BT_MAPS_GET_MESSAGE_RES, CsrBtMapsGetMessageResSizeof, CsrBtMapsGetMessageResSer, CsrBtMapsGetMessageResDes, CsrBtMapsGetMessageResSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_RES */
#ifndef EXCLUDE_CSR_BT_MAPS_NOTIFICATION_REGISTRATION_COMPLETE_IND
    { CSR_BT_MAPS_NOTIFICATION_REGISTRATION_COMPLETE_IND, CsrBtMapsNotificationRegistrationCompleteIndSizeof, CsrBtMapsNotificationRegistrationCompleteIndSer, CsrBtMapsNotificationRegistrationCompleteIndDes, CsrBtMapsNotificationRegistrationCompleteIndSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_NOTIFICATION_REGISTRATION_COMPLETE_IND */
#ifndef EXCLUDE_CSR_BT_MAPS_NOTIFICATION_REGISTRATION_IND
    { CSR_BT_MAPS_NOTIFICATION_REGISTRATION_IND, CsrBtMapsNotificationRegistrationIndSizeof, CsrBtMapsNotificationRegistrationIndSer, CsrBtMapsNotificationRegistrationIndDes, CsrBtMapsNotificationRegistrationIndSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_NOTIFICATION_REGISTRATION_IND */
#ifndef EXCLUDE_CSR_BT_MAPS_NOTIFICATION_REGISTRATION_RES
    { CSR_BT_MAPS_NOTIFICATION_REGISTRATION_RES, CsrBtMapsNotificationRegistrationResSizeof, CsrBtMapsNotificationRegistrationResSer, CsrBtMapsNotificationRegistrationResDes, CsrBtMapsNotificationRegistrationResSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_NOTIFICATION_REGISTRATION_RES */
#ifndef EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_HEADER_IND
    { CSR_BT_MAPS_PUSH_MESSAGE_HEADER_IND, CsrBtMapsPushMessageHeaderIndSizeof, CsrBtMapsPushMessageHeaderIndSer, CsrBtMapsPushMessageHeaderIndDes, CsrBtMapsPushMessageHeaderIndSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_HEADER_IND */
#ifndef EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_HEADER_RES
    { CSR_BT_MAPS_PUSH_MESSAGE_HEADER_RES, CsrBtMapsPushMessageHeaderResSizeof, CsrBtMapsPushMessageHeaderResSer, CsrBtMapsPushMessageHeaderResDes, CsrBtMapsPushMessageHeaderResSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_HEADER_RES */
#ifndef EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_IND
    { CSR_BT_MAPS_PUSH_MESSAGE_IND, CsrBtMapsPushMessageIndSizeof, CsrBtMapsPushMessageIndSer, CsrBtMapsPushMessageIndDes, CsrBtMapsPushMessageIndSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_IND */
#ifndef EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_RES
    { CSR_BT_MAPS_PUSH_MESSAGE_RES, CsrBtMapsPushMessageResSizeof, CsrBtMapsPushMessageResSer, CsrBtMapsPushMessageResDes, CsrBtMapsPushMessageResSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_RES */
#ifndef EXCLUDE_CSR_BT_MAPS_REGISTER_QID_REQ
    { CSR_BT_MAPS_REGISTER_QID_REQ, CsrBtMapsRegisterQidReqSizeof, CsrBtMapsRegisterQidReqSer, CsrBtMapsRegisterQidReqDes, CsrBtMapsRegisterQidReqSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_REGISTER_QID_REQ */
#ifndef EXCLUDE_CSR_BT_MAPS_SECURITY_LEVEL_CFM
    { CSR_BT_MAPS_SECURITY_LEVEL_CFM, CsrBtMapsSecurityLevelCfmSizeof, CsrBtMapsSecurityLevelCfmSer, CsrBtMapsSecurityLevelCfmDes, CsrBtMapsSecurityLevelCfmSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_SECURITY_LEVEL_CFM */
#ifndef EXCLUDE_CSR_BT_MAPS_SECURITY_LEVEL_REQ
    { CSR_BT_MAPS_SECURITY_LEVEL_REQ, CsrBtMapsSecurityLevelReqSizeof, CsrBtMapsSecurityLevelReqSer, CsrBtMapsSecurityLevelReqDes, CsrBtMapsSecurityLevelReqSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_SECURITY_LEVEL_REQ */
#ifndef EXCLUDE_CSR_BT_MAPS_SELECT_MAS_INSTANCE_IND
    { CSR_BT_MAPS_SELECT_MAS_INSTANCE_IND, CsrBtMapsSelectMasInstanceIndSizeof, CsrBtMapsSelectMasInstanceIndSer, CsrBtMapsSelectMasInstanceIndDes, CsrBtMapsSelectMasInstanceIndSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_SELECT_MAS_INSTANCE_IND */
#ifndef EXCLUDE_CSR_BT_MAPS_SELECT_MAS_INSTANCE_RES
    { CSR_BT_MAPS_SELECT_MAS_INSTANCE_RES, CsrBtMapsSelectMasInstanceResSizeof, CsrBtMapsSelectMasInstanceResSer, CsrBtMapsSelectMasInstanceResDes, CsrBtMapsSelectMasInstanceResSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_SELECT_MAS_INSTANCE_RES */
#ifndef EXCLUDE_CSR_BT_MAPS_SET_BACK_FOLDER_IND
    { CSR_BT_MAPS_SET_BACK_FOLDER_IND, CsrBtMapsSetBackFolderIndSizeof, CsrBtMapsSetBackFolderIndSer, CsrBtMapsSetBackFolderIndDes, CsrBtMapsSetBackFolderIndSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_SET_BACK_FOLDER_IND */
#ifndef EXCLUDE_CSR_BT_MAPS_SET_BACK_FOLDER_RES
    { CSR_BT_MAPS_SET_BACK_FOLDER_RES, CsrBtMapsSetBackFolderResSizeof, CsrBtMapsSetBackFolderResSer, CsrBtMapsSetBackFolderResDes, CsrBtMapsSetBackFolderResSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_SET_BACK_FOLDER_RES */
#ifndef EXCLUDE_CSR_BT_MAPS_SET_FOLDER_IND
    { CSR_BT_MAPS_SET_FOLDER_IND, CsrBtMapsSetFolderIndSizeof, CsrBtMapsSetFolderIndSer, CsrBtMapsSetFolderIndDes, CsrBtMapsSetFolderIndSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_SET_FOLDER_IND */
#ifndef EXCLUDE_CSR_BT_MAPS_SET_FOLDER_RES
    { CSR_BT_MAPS_SET_FOLDER_RES, CsrBtMapsSetFolderResSizeof, CsrBtMapsSetFolderResSer, CsrBtMapsSetFolderResDes, CsrBtMapsSetFolderResSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_SET_FOLDER_RES */
#ifndef EXCLUDE_CSR_BT_MAPS_SET_MESSAGE_STATUS_IND
    { CSR_BT_MAPS_SET_MESSAGE_STATUS_IND, CsrBtMapsSetMessageStatusIndSizeof, CsrBtMapsSetMessageStatusIndSer, CsrBtMapsSetMessageStatusIndDes, CsrBtMapsSetMessageStatusIndSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_SET_MESSAGE_STATUS_IND */
#ifndef EXCLUDE_CSR_BT_MAPS_SET_MESSAGE_STATUS_RES
    { CSR_BT_MAPS_SET_MESSAGE_STATUS_RES, CsrBtMapsSetMessageStatusResSizeof, CsrBtMapsSetMessageStatusResSer, CsrBtMapsSetMessageStatusResDes, CsrBtMapsSetMessageStatusResSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_SET_MESSAGE_STATUS_RES */
#ifndef EXCLUDE_CSR_BT_MAPS_SET_ROOT_FOLDER_IND
    { CSR_BT_MAPS_SET_ROOT_FOLDER_IND, CsrBtMapsSetRootFolderIndSizeof, CsrBtMapsSetRootFolderIndSer, CsrBtMapsSetRootFolderIndDes, CsrBtMapsSetRootFolderIndSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_SET_ROOT_FOLDER_IND */
#ifndef EXCLUDE_CSR_BT_MAPS_SET_ROOT_FOLDER_RES
    { CSR_BT_MAPS_SET_ROOT_FOLDER_RES, CsrBtMapsSetRootFolderResSizeof, CsrBtMapsSetRootFolderResSer, CsrBtMapsSetRootFolderResDes, CsrBtMapsSetRootFolderResSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_SET_ROOT_FOLDER_RES */
#ifndef EXCLUDE_CSR_BT_MAPS_UPDATE_INBOX_IND
    { CSR_BT_MAPS_UPDATE_INBOX_IND, CsrBtMapsUpdateInboxIndSizeof, CsrBtMapsUpdateInboxIndSer, CsrBtMapsUpdateInboxIndDes, CsrBtMapsUpdateInboxIndSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_UPDATE_INBOX_IND */
#ifndef EXCLUDE_CSR_BT_MAPS_UPDATE_INBOX_RES
    { CSR_BT_MAPS_UPDATE_INBOX_RES, CsrBtMapsUpdateInboxResSizeof, CsrBtMapsUpdateInboxResSer, CsrBtMapsUpdateInboxResDes, CsrBtMapsUpdateInboxResSerFree },
#endif /* EXCLUDE_CSR_BT_MAPS_UPDATE_INBOX_RES */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtMapsConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_MAPS_PRIM, csr_bt_maps_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_maps_conv_info = {
    CSR_BT_MAPS_PRIM,
    "CSR_BT_MAPS_PRIM",
    csr_bt_maps_conv_lut
};

CsrLogPrimitiveInformation* CsrBtMapsTechInfoGet(void)
{
    return &csr_bt_maps_conv_info;
}
#endif
#endif

