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
#include "csr_bt_mapc_converter_init.h"
#ifndef EXCLUDE_CSR_BT_MAPC_MODULE
#include "csr_bt_mapc_serialize.h"
#include "csr_bt_mapc_prim.h"

static CsrMsgConvMsgEntry csr_bt_mapc_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_MAPC_ABORT_CFM
    { CSR_BT_MAPC_ABORT_CFM, CsrBtMapcAbortCfmSizeof, CsrBtMapcAbortCfmSer, CsrBtMapcAbortCfmDes, CsrBtMapcAbortCfmSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_ABORT_CFM */
#ifndef EXCLUDE_CSR_BT_MAPC_ABORT_REQ
    { CSR_BT_MAPC_ABORT_REQ, CsrBtMapcAbortReqSizeof, CsrBtMapcAbortReqSer, CsrBtMapcAbortReqDes, CsrBtMapcAbortReqSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_ABORT_REQ */
#ifndef EXCLUDE_CSR_BT_MAPC_CANCEL_CONNECT_REQ
    { CSR_BT_MAPC_CANCEL_CONNECT_REQ, CsrBtMapcCancelConnectReqSizeof, CsrBtMapcCancelConnectReqSer, CsrBtMapcCancelConnectReqDes, CsrBtMapcCancelConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_CANCEL_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_MAPC_CONNECT_CFM
    { CSR_BT_MAPC_CONNECT_CFM, CsrBtMapcConnectCfmSizeof, CsrBtMapcConnectCfmSer, CsrBtMapcConnectCfmDes, CsrBtMapcConnectCfmSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_CONNECT_CFM */
#ifndef EXCLUDE_CSR_BT_MAPC_CONNECT_REQ
    { CSR_BT_MAPC_CONNECT_REQ, CsrBtMapcConnectReqSizeof, CsrBtMapcConnectReqSer, CsrBtMapcConnectReqDes, CsrBtMapcConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_MAPC_DISCONNECT_IND
    { CSR_BT_MAPC_DISCONNECT_IND, CsrBtMapcDisconnectIndSizeof, CsrBtMapcDisconnectIndSer, CsrBtMapcDisconnectIndDes, CsrBtMapcDisconnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_DISCONNECT_IND */
#ifndef EXCLUDE_CSR_BT_MAPC_DISCONNECT_REQ
    { CSR_BT_MAPC_DISCONNECT_REQ, CsrBtMapcDisconnectReqSizeof, CsrBtMapcDisconnectReqSer, CsrBtMapcDisconnectReqDes, CsrBtMapcDisconnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_DISCONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_MAPC_EVENT_NOTIFICATION_ABORT_IND
    { CSR_BT_MAPC_EVENT_NOTIFICATION_ABORT_IND, CsrBtMapcEventNotificationAbortIndSizeof, CsrBtMapcEventNotificationAbortIndSer, CsrBtMapcEventNotificationAbortIndDes, CsrBtMapcEventNotificationAbortIndSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_EVENT_NOTIFICATION_ABORT_IND */
#ifndef EXCLUDE_CSR_BT_MAPC_EVENT_NOTIFICATION_IND
    { CSR_BT_MAPC_EVENT_NOTIFICATION_IND, CsrBtMapcEventNotificationIndSizeof, CsrBtMapcEventNotificationIndSer, CsrBtMapcEventNotificationIndDes, CsrBtMapcEventNotificationIndSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_EVENT_NOTIFICATION_IND */
#ifndef EXCLUDE_CSR_BT_MAPC_EVENT_NOTIFICATION_RES
    { CSR_BT_MAPC_EVENT_NOTIFICATION_RES, CsrBtMapcEventNotificationResSizeof, CsrBtMapcEventNotificationResSer, CsrBtMapcEventNotificationResDes, CsrBtMapcEventNotificationResSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_EVENT_NOTIFICATION_RES */
#ifndef EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_CFM
    { CSR_BT_MAPC_GET_FOLDER_LISTING_CFM, CsrBtMapcGetFolderListingCfmSizeof, CsrBtMapcGetFolderListingCfmSer, CsrBtMapcGetFolderListingCfmDes, CsrBtMapcGetFolderListingCfmSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_CFM */
#ifndef EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_IND
    { CSR_BT_MAPC_GET_FOLDER_LISTING_IND, CsrBtMapcGetFolderListingIndSizeof, CsrBtMapcGetFolderListingIndSer, CsrBtMapcGetFolderListingIndDes, CsrBtMapcGetFolderListingIndSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_IND */
#ifndef EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_REQ
    { CSR_BT_MAPC_GET_FOLDER_LISTING_REQ, CsrBtMapcGetFolderListingReqSizeof, CsrBtMapcGetFolderListingReqSer, CsrBtMapcGetFolderListingReqDes, CsrBtMapcGetFolderListingReqSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_REQ */
#ifndef EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_RES
    { CSR_BT_MAPC_GET_FOLDER_LISTING_RES, CsrBtMapcGetFolderListingResSizeof, CsrBtMapcGetFolderListingResSer, CsrBtMapcGetFolderListingResDes, CsrBtMapcGetFolderListingResSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_RES */
#ifndef EXCLUDE_CSR_BT_MAPC_GET_INSTANCE_IDS_CFM
    { CSR_BT_MAPC_GET_INSTANCE_IDS_CFM, CsrBtMapcGetInstanceIdsCfmSizeof, CsrBtMapcGetInstanceIdsCfmSer, CsrBtMapcGetInstanceIdsCfmDes, CsrBtMapcGetInstanceIdsCfmSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_GET_INSTANCE_IDS_CFM */
#ifndef EXCLUDE_CSR_BT_MAPC_GET_INSTANCE_IDS_REQ
    { CSR_BT_MAPC_GET_INSTANCE_IDS_REQ, CsrBtMapcGetInstanceIdsReqSizeof, CsrBtMapcGetInstanceIdsReqSer, CsrBtMapcGetInstanceIdsReqDes, CsrBtMapcGetInstanceIdsReqSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_GET_INSTANCE_IDS_REQ */
#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_CFM
    { CSR_BT_MAPC_GET_MESSAGE_CFM, CsrBtMapcGetMessageCfmSizeof, CsrBtMapcGetMessageCfmSer, CsrBtMapcGetMessageCfmDes, CsrBtMapcGetMessageCfmSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_CFM */
#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_IND
    { CSR_BT_MAPC_GET_MESSAGE_IND, CsrBtMapcGetMessageIndSizeof, CsrBtMapcGetMessageIndSer, CsrBtMapcGetMessageIndDes, CsrBtMapcGetMessageIndSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_IND */
#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_CFM
    { CSR_BT_MAPC_GET_MESSAGE_LISTING_CFM, CsrBtMapcGetMessageListingCfmSizeof, CsrBtMapcGetMessageListingCfmSer, CsrBtMapcGetMessageListingCfmDes, CsrBtMapcGetMessageListingCfmSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_CFM */
#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_IND
    { CSR_BT_MAPC_GET_MESSAGE_LISTING_IND, CsrBtMapcGetMessageListingIndSizeof, CsrBtMapcGetMessageListingIndSer, CsrBtMapcGetMessageListingIndDes, CsrBtMapcGetMessageListingIndSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_IND */
#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_REQ
    { CSR_BT_MAPC_GET_MESSAGE_LISTING_REQ, CsrBtMapcGetMessageListingReqSizeof, CsrBtMapcGetMessageListingReqSer, CsrBtMapcGetMessageListingReqDes, CsrBtMapcGetMessageListingReqSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_REQ */
#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_RES
    { CSR_BT_MAPC_GET_MESSAGE_LISTING_RES, CsrBtMapcGetMessageListingResSizeof, CsrBtMapcGetMessageListingResSer, CsrBtMapcGetMessageListingResDes, CsrBtMapcGetMessageListingResSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_RES */
#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_REQ
    { CSR_BT_MAPC_GET_MESSAGE_REQ, CsrBtMapcGetMessageReqSizeof, CsrBtMapcGetMessageReqSer, CsrBtMapcGetMessageReqDes, CsrBtMapcGetMessageReqSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_REQ */
#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_RES
    { CSR_BT_MAPC_GET_MESSAGE_RES, CsrBtMapcGetMessageResSizeof, CsrBtMapcGetMessageResSer, CsrBtMapcGetMessageResDes, CsrBtMapcGetMessageResSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_RES */
#ifndef EXCLUDE_CSR_BT_MAPC_NOTIFICATION_REGISTRATION_CFM
    { CSR_BT_MAPC_NOTIFICATION_REGISTRATION_CFM, CsrBtMapcNotificationRegistrationCfmSizeof, CsrBtMapcNotificationRegistrationCfmSer, CsrBtMapcNotificationRegistrationCfmDes, CsrBtMapcNotificationRegistrationCfmSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_NOTIFICATION_REGISTRATION_CFM */
#ifndef EXCLUDE_CSR_BT_MAPC_NOTIFICATION_REGISTRATION_OFF_IND
    { CSR_BT_MAPC_NOTIFICATION_REGISTRATION_OFF_IND, CsrBtMapcNotificationRegistrationOffIndSizeof, CsrBtMapcNotificationRegistrationOffIndSer, CsrBtMapcNotificationRegistrationOffIndDes, CsrBtMapcNotificationRegistrationOffIndSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_NOTIFICATION_REGISTRATION_OFF_IND */
#ifndef EXCLUDE_CSR_BT_MAPC_NOTIFICATION_REGISTRATION_REQ
    { CSR_BT_MAPC_NOTIFICATION_REGISTRATION_REQ, CsrBtMapcNotificationRegistrationReqSizeof, CsrBtMapcNotificationRegistrationReqSer, CsrBtMapcNotificationRegistrationReqDes, CsrBtMapcNotificationRegistrationReqSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_NOTIFICATION_REGISTRATION_REQ */
#ifndef EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_CFM
    { CSR_BT_MAPC_PUSH_MESSAGE_CFM, CsrBtMapcPushMessageCfmSizeof, CsrBtMapcPushMessageCfmSer, CsrBtMapcPushMessageCfmDes, CsrBtMapcPushMessageCfmSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_CFM */
#ifndef EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_IND
    { CSR_BT_MAPC_PUSH_MESSAGE_IND, CsrBtMapcPushMessageIndSizeof, CsrBtMapcPushMessageIndSer, CsrBtMapcPushMessageIndDes, CsrBtMapcPushMessageIndSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_IND */
#ifndef EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_REQ
    { CSR_BT_MAPC_PUSH_MESSAGE_REQ, CsrBtMapcPushMessageReqSizeof, CsrBtMapcPushMessageReqSer, CsrBtMapcPushMessageReqDes, CsrBtMapcPushMessageReqSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_REQ */
#ifndef EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_RES
    { CSR_BT_MAPC_PUSH_MESSAGE_RES, CsrBtMapcPushMessageResSizeof, CsrBtMapcPushMessageResSer, CsrBtMapcPushMessageResDes, CsrBtMapcPushMessageResSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_RES */
#ifndef EXCLUDE_CSR_BT_MAPC_REGISTER_QID_REQ
    { CSR_BT_MAPC_REGISTER_QID_REQ, CsrBtMapcRegisterQidReqSizeof, CsrBtMapcRegisterQidReqSer, CsrBtMapcRegisterQidReqDes, CsrBtMapcRegisterQidReqSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_REGISTER_QID_REQ */
#ifndef EXCLUDE_CSR_BT_MAPC_SECURITY_IN_CFM
    { CSR_BT_MAPC_SECURITY_IN_CFM, CsrBtMapcSecurityInCfmSizeof, CsrBtMapcSecurityInCfmSer, CsrBtMapcSecurityInCfmDes, CsrBtMapcSecurityInCfmSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_SECURITY_IN_CFM */
#ifndef EXCLUDE_CSR_BT_MAPC_SECURITY_IN_REQ
    { CSR_BT_MAPC_SECURITY_IN_REQ, CsrBtMapcSecurityInReqSizeof, CsrBtMapcSecurityInReqSer, CsrBtMapcSecurityInReqDes, CsrBtMapcSecurityInReqSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_SECURITY_IN_REQ */
#ifndef EXCLUDE_CSR_BT_MAPC_SECURITY_OUT_CFM
    { CSR_BT_MAPC_SECURITY_OUT_CFM, CsrBtMapcSecurityOutCfmSizeof, CsrBtMapcSecurityOutCfmSer, CsrBtMapcSecurityOutCfmDes, CsrBtMapcSecurityOutCfmSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_SECURITY_OUT_CFM */
#ifndef EXCLUDE_CSR_BT_MAPC_SECURITY_OUT_REQ
    { CSR_BT_MAPC_SECURITY_OUT_REQ, CsrBtMapcSecurityOutReqSizeof, CsrBtMapcSecurityOutReqSer, CsrBtMapcSecurityOutReqDes, CsrBtMapcSecurityOutReqSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_SECURITY_OUT_REQ */
#ifndef EXCLUDE_CSR_BT_MAPC_SELECT_MAS_INSTANCE_IND
    { CSR_BT_MAPC_SELECT_MAS_INSTANCE_IND, CsrBtMapcSelectMasInstanceIndSizeof, CsrBtMapcSelectMasInstanceIndSer, CsrBtMapcSelectMasInstanceIndDes, CsrBtMapcSelectMasInstanceIndSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_SELECT_MAS_INSTANCE_IND */
#ifndef EXCLUDE_CSR_BT_MAPC_SELECT_MAS_INSTANCE_RES
    { CSR_BT_MAPC_SELECT_MAS_INSTANCE_RES, CsrBtMapcSelectMasInstanceResSizeof, CsrBtMapcSelectMasInstanceResSer, CsrBtMapcSelectMasInstanceResDes, CsrBtMapcSelectMasInstanceResSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_SELECT_MAS_INSTANCE_RES */
#ifndef EXCLUDE_CSR_BT_MAPC_SET_BACK_FOLDER_CFM
    { CSR_BT_MAPC_SET_BACK_FOLDER_CFM, CsrBtMapcSetBackFolderCfmSizeof, CsrBtMapcSetBackFolderCfmSer, CsrBtMapcSetBackFolderCfmDes, CsrBtMapcSetBackFolderCfmSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_SET_BACK_FOLDER_CFM */
#ifndef EXCLUDE_CSR_BT_MAPC_SET_BACK_FOLDER_REQ
    { CSR_BT_MAPC_SET_BACK_FOLDER_REQ, CsrBtMapcSetBackFolderReqSizeof, CsrBtMapcSetBackFolderReqSer, CsrBtMapcSetBackFolderReqDes, CsrBtMapcSetBackFolderReqSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_SET_BACK_FOLDER_REQ */
#ifndef EXCLUDE_CSR_BT_MAPC_SET_FOLDER_CFM
    { CSR_BT_MAPC_SET_FOLDER_CFM, CsrBtMapcSetFolderCfmSizeof, CsrBtMapcSetFolderCfmSer, CsrBtMapcSetFolderCfmDes, CsrBtMapcSetFolderCfmSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_SET_FOLDER_CFM */
#ifndef EXCLUDE_CSR_BT_MAPC_SET_FOLDER_REQ
    { CSR_BT_MAPC_SET_FOLDER_REQ, CsrBtMapcSetFolderReqSizeof, CsrBtMapcSetFolderReqSer, CsrBtMapcSetFolderReqDes, CsrBtMapcSetFolderReqSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_SET_FOLDER_REQ */
#ifndef EXCLUDE_CSR_BT_MAPC_SET_MESSAGE_STATUS_CFM
    { CSR_BT_MAPC_SET_MESSAGE_STATUS_CFM, CsrBtMapcSetMessageStatusCfmSizeof, CsrBtMapcSetMessageStatusCfmSer, CsrBtMapcSetMessageStatusCfmDes, CsrBtMapcSetMessageStatusCfmSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_SET_MESSAGE_STATUS_CFM */
#ifndef EXCLUDE_CSR_BT_MAPC_SET_MESSAGE_STATUS_REQ
    { CSR_BT_MAPC_SET_MESSAGE_STATUS_REQ, CsrBtMapcSetMessageStatusReqSizeof, CsrBtMapcSetMessageStatusReqSer, CsrBtMapcSetMessageStatusReqDes, CsrBtMapcSetMessageStatusReqSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_SET_MESSAGE_STATUS_REQ */
#ifndef EXCLUDE_CSR_BT_MAPC_SET_ROOT_FOLDER_CFM
    { CSR_BT_MAPC_SET_ROOT_FOLDER_CFM, CsrBtMapcSetRootFolderCfmSizeof, CsrBtMapcSetRootFolderCfmSer, CsrBtMapcSetRootFolderCfmDes, CsrBtMapcSetRootFolderCfmSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_SET_ROOT_FOLDER_CFM */
#ifndef EXCLUDE_CSR_BT_MAPC_SET_ROOT_FOLDER_REQ
    { CSR_BT_MAPC_SET_ROOT_FOLDER_REQ, CsrBtMapcSetRootFolderReqSizeof, CsrBtMapcSetRootFolderReqSer, CsrBtMapcSetRootFolderReqDes, CsrBtMapcSetRootFolderReqSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_SET_ROOT_FOLDER_REQ */
#ifndef EXCLUDE_CSR_BT_MAPC_UPDATE_INBOX_CFM
    { CSR_BT_MAPC_UPDATE_INBOX_CFM, CsrBtMapcUpdateInboxCfmSizeof, CsrBtMapcUpdateInboxCfmSer, CsrBtMapcUpdateInboxCfmDes, CsrBtMapcUpdateInboxCfmSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_UPDATE_INBOX_CFM */
#ifndef EXCLUDE_CSR_BT_MAPC_UPDATE_INBOX_REQ
    { CSR_BT_MAPC_UPDATE_INBOX_REQ, CsrBtMapcUpdateInboxReqSizeof, CsrBtMapcUpdateInboxReqSer, CsrBtMapcUpdateInboxReqDes, CsrBtMapcUpdateInboxReqSerFree },
#endif /* EXCLUDE_CSR_BT_MAPC_UPDATE_INBOX_REQ */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtMapcConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_MAPC_PRIM, csr_bt_mapc_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_mapc_conv_info = {
    CSR_BT_MAPC_PRIM,
    "CSR_BT_MAPC_PRIM",
    csr_bt_mapc_conv_lut
};

CsrLogPrimitiveInformation* CsrBtMapcTechInfoGet(void)
{
    return &csr_bt_mapc_conv_info;
}
#endif
#endif

