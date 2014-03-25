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
#include "csr_bt_avrcp_converter_init.h"
#ifndef EXCLUDE_CSR_BT_AVRCP_MODULE
#include "csr_bt_avrcp_serialize.h"
#include "csr_bt_avrcp_prim.h"

static CsrMsgConvMsgEntry csr_bt_avrcp_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_AVRCP_ACTIVATE_CFM
    { CSR_BT_AVRCP_ACTIVATE_CFM, CsrBtAvrcpActivateCfmSizeof, CsrBtAvrcpActivateCfmSer, CsrBtAvrcpActivateCfmDes, CsrBtAvrcpActivateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_ACTIVATE_CFM */
#ifndef EXCLUDE_CSR_BT_AVRCP_ACTIVATE_REQ
    { CSR_BT_AVRCP_ACTIVATE_REQ, CsrBtAvrcpActivateReqSizeof, CsrBtAvrcpActivateReqSer, CsrBtAvrcpActivateReqDes, CsrBtAvrcpActivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_ACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_AVRCP_CANCEL_CONNECT_REQ
    { CSR_BT_AVRCP_CANCEL_CONNECT_REQ, CsrBtAvrcpCancelConnectReqSizeof, CsrBtAvrcpCancelConnectReqSer, CsrBtAvrcpCancelConnectReqDes, CsrBtAvrcpCancelConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CANCEL_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_AVRCP_CONFIG_CFM
    { CSR_BT_AVRCP_CONFIG_CFM, CsrBtAvrcpConfigCfmSizeof, CsrBtAvrcpConfigCfmSer, CsrBtAvrcpConfigCfmDes, CsrBtAvrcpConfigCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CONFIG_CFM */
#ifndef EXCLUDE_CSR_BT_AVRCP_CONFIG_REQ
    { CSR_BT_AVRCP_CONFIG_REQ, CsrBtAvrcpConfigReqSizeof, CsrBtAvrcpConfigReqSer, CsrBtAvrcpConfigReqDes, CsrBtAvrcpConfigReqSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CONFIG_REQ */
#ifndef EXCLUDE_CSR_BT_AVRCP_CONNECT_CFM
    { CSR_BT_AVRCP_CONNECT_CFM, CsrBtAvrcpConnectCfmSizeof, CsrBtAvrcpConnectCfmSer, CsrBtAvrcpConnectCfmDes, CsrBtAvrcpConnectCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CONNECT_CFM */
#ifndef EXCLUDE_CSR_BT_AVRCP_CONNECT_IND
    { CSR_BT_AVRCP_CONNECT_IND, CsrBtAvrcpConnectIndSizeof, CsrBtAvrcpConnectIndSer, CsrBtAvrcpConnectIndDes, CsrBtAvrcpConnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CONNECT_IND */
#ifndef EXCLUDE_CSR_BT_AVRCP_CONNECT_REQ
    { CSR_BT_AVRCP_CONNECT_REQ, CsrBtAvrcpConnectReqSizeof, CsrBtAvrcpConnectReqSer, CsrBtAvrcpConnectReqDes, CsrBtAvrcpConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_ADD_TO_NOW_PLAYING_CFM
    { CSR_BT_AVRCP_CT_ADD_TO_NOW_PLAYING_CFM, CsrBtAvrcpCtAddToNowPlayingCfmSizeof, CsrBtAvrcpCtAddToNowPlayingCfmSer, CsrBtAvrcpCtAddToNowPlayingCfmDes, CsrBtAvrcpCtAddToNowPlayingCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_ADD_TO_NOW_PLAYING_CFM */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_ADD_TO_NOW_PLAYING_REQ
    { CSR_BT_AVRCP_CT_ADD_TO_NOW_PLAYING_REQ, CsrBtAvrcpCtAddToNowPlayingReqSizeof, CsrBtAvrcpCtAddToNowPlayingReqSer, CsrBtAvrcpCtAddToNowPlayingReqDes, CsrBtAvrcpCtAddToNowPlayingReqSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_ADD_TO_NOW_PLAYING_REQ */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_CHANGE_PATH_CFM
    { CSR_BT_AVRCP_CT_CHANGE_PATH_CFM, CsrBtAvrcpCtChangePathCfmSizeof, CsrBtAvrcpCtChangePathCfmSer, CsrBtAvrcpCtChangePathCfmDes, CsrBtAvrcpCtChangePathCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_CHANGE_PATH_CFM */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_CHANGE_PATH_REQ
    { CSR_BT_AVRCP_CT_CHANGE_PATH_REQ, CsrBtAvrcpCtChangePathReqSizeof, CsrBtAvrcpCtChangePathReqSer, CsrBtAvrcpCtChangePathReqDes, CsrBtAvrcpCtChangePathReqSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_CHANGE_PATH_REQ */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_CFM
    { CSR_BT_AVRCP_CT_GET_ATTRIBUTES_CFM, CsrBtAvrcpCtGetAttributesCfmSizeof, CsrBtAvrcpCtGetAttributesCfmSer, CsrBtAvrcpCtGetAttributesCfmDes, CsrBtAvrcpCtGetAttributesCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_CFM */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_IND
    { CSR_BT_AVRCP_CT_GET_ATTRIBUTES_IND, CsrBtAvrcpCtGetAttributesIndSizeof, CsrBtAvrcpCtGetAttributesIndSer, CsrBtAvrcpCtGetAttributesIndDes, CsrBtAvrcpCtGetAttributesIndSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_IND */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_REQ
    { CSR_BT_AVRCP_CT_GET_ATTRIBUTES_REQ, CsrBtAvrcpCtGetAttributesReqSizeof, CsrBtAvrcpCtGetAttributesReqSer, CsrBtAvrcpCtGetAttributesReqDes, CsrBtAvrcpCtGetAttributesReqSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_REQ */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_RES
    { CSR_BT_AVRCP_CT_GET_ATTRIBUTES_RES, CsrBtAvrcpCtGetAttributesResSizeof, CsrBtAvrcpCtGetAttributesResSer, CsrBtAvrcpCtGetAttributesResDes, CsrBtAvrcpCtGetAttributesResSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_RES */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM
    { CSR_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM, CsrBtAvrcpCtGetFolderItemsCfmSizeof, CsrBtAvrcpCtGetFolderItemsCfmSer, CsrBtAvrcpCtGetFolderItemsCfmDes, CsrBtAvrcpCtGetFolderItemsCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_GET_FOLDER_ITEMS_REQ
    { CSR_BT_AVRCP_CT_GET_FOLDER_ITEMS_REQ, CsrBtAvrcpCtGetFolderItemsReqSizeof, CsrBtAvrcpCtGetFolderItemsReqSer, CsrBtAvrcpCtGetFolderItemsReqDes, CsrBtAvrcpCtGetFolderItemsReqSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_GET_FOLDER_ITEMS_REQ */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_GET_PLAY_STATUS_CFM
    { CSR_BT_AVRCP_CT_GET_PLAY_STATUS_CFM, CsrBtAvrcpCtGetPlayStatusCfmSizeof, CsrBtAvrcpCtGetPlayStatusCfmSer, CsrBtAvrcpCtGetPlayStatusCfmDes, CsrBtAvrcpCtGetPlayStatusCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_GET_PLAY_STATUS_CFM */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_GET_PLAY_STATUS_REQ
    { CSR_BT_AVRCP_CT_GET_PLAY_STATUS_REQ, CsrBtAvrcpCtGetPlayStatusReqSizeof, CsrBtAvrcpCtGetPlayStatusReqSer, CsrBtAvrcpCtGetPlayStatusReqDes, CsrBtAvrcpCtGetPlayStatusReqSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_GET_PLAY_STATUS_REQ */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_INFORM_BATTERY_STATUS_CFM
    { CSR_BT_AVRCP_CT_INFORM_BATTERY_STATUS_CFM, CsrBtAvrcpCtInformBatteryStatusCfmSizeof, CsrBtAvrcpCtInformBatteryStatusCfmSer, CsrBtAvrcpCtInformBatteryStatusCfmDes, CsrBtAvrcpCtInformBatteryStatusCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_INFORM_BATTERY_STATUS_CFM */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_INFORM_BATTERY_STATUS_REQ
    { CSR_BT_AVRCP_CT_INFORM_BATTERY_STATUS_REQ, CsrBtAvrcpCtInformBatteryStatusReqSizeof, CsrBtAvrcpCtInformBatteryStatusReqSer, CsrBtAvrcpCtInformBatteryStatusReqDes, CsrBtAvrcpCtInformBatteryStatusReqSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_INFORM_BATTERY_STATUS_REQ */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_INFORM_DISP_CHARSET_CFM
    { CSR_BT_AVRCP_CT_INFORM_DISP_CHARSET_CFM, CsrBtAvrcpCtInformDispCharsetCfmSizeof, CsrBtAvrcpCtInformDispCharsetCfmSer, CsrBtAvrcpCtInformDispCharsetCfmDes, CsrBtAvrcpCtInformDispCharsetCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_INFORM_DISP_CHARSET_CFM */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_INFORM_DISP_CHARSET_REQ
    { CSR_BT_AVRCP_CT_INFORM_DISP_CHARSET_REQ, CsrBtAvrcpCtInformDispCharsetReqSizeof, CsrBtAvrcpCtInformDispCharsetReqSer, CsrBtAvrcpCtInformDispCharsetReqDes, CsrBtAvrcpCtInformDispCharsetReqSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_INFORM_DISP_CHARSET_REQ */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_AVAILABLE_PLAYERS_IND
    { CSR_BT_AVRCP_CT_NOTI_AVAILABLE_PLAYERS_IND, CsrBtAvrcpCtNotiAvailablePlayersIndSizeof, CsrBtAvrcpCtNotiAvailablePlayersIndSer, CsrBtAvrcpCtNotiAvailablePlayersIndDes, CsrBtAvrcpCtNotiAvailablePlayersIndSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_AVAILABLE_PLAYERS_IND */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_BATTERY_STATUS_IND
    { CSR_BT_AVRCP_CT_NOTI_BATTERY_STATUS_IND, CsrBtAvrcpCtNotiBatteryStatusIndSizeof, CsrBtAvrcpCtNotiBatteryStatusIndSer, CsrBtAvrcpCtNotiBatteryStatusIndDes, CsrBtAvrcpCtNotiBatteryStatusIndSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_BATTERY_STATUS_IND */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_NOW_PLAYING_IND
    { CSR_BT_AVRCP_CT_NOTI_NOW_PLAYING_IND, CsrBtAvrcpCtNotiNowPlayingIndSizeof, CsrBtAvrcpCtNotiNowPlayingIndSer, CsrBtAvrcpCtNotiNowPlayingIndDes, CsrBtAvrcpCtNotiNowPlayingIndSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_NOW_PLAYING_IND */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_PLAYBACK_POS_IND
    { CSR_BT_AVRCP_CT_NOTI_PLAYBACK_POS_IND, CsrBtAvrcpCtNotiPlaybackPosIndSizeof, CsrBtAvrcpCtNotiPlaybackPosIndSer, CsrBtAvrcpCtNotiPlaybackPosIndDes, CsrBtAvrcpCtNotiPlaybackPosIndSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_PLAYBACK_POS_IND */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_PLAYBACK_STATUS_IND
    { CSR_BT_AVRCP_CT_NOTI_PLAYBACK_STATUS_IND, CsrBtAvrcpCtNotiPlaybackStatusIndSizeof, CsrBtAvrcpCtNotiPlaybackStatusIndSer, CsrBtAvrcpCtNotiPlaybackStatusIndDes, CsrBtAvrcpCtNotiPlaybackStatusIndSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_PLAYBACK_STATUS_IND */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_REGISTER_CFM
    { CSR_BT_AVRCP_CT_NOTI_REGISTER_CFM, CsrBtAvrcpCtNotiRegisterCfmSizeof, CsrBtAvrcpCtNotiRegisterCfmSer, CsrBtAvrcpCtNotiRegisterCfmDes, CsrBtAvrcpCtNotiRegisterCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_REGISTER_CFM */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_REGISTER_REQ
    { CSR_BT_AVRCP_CT_NOTI_REGISTER_REQ, CsrBtAvrcpCtNotiRegisterReqSizeof, CsrBtAvrcpCtNotiRegisterReqSer, CsrBtAvrcpCtNotiRegisterReqDes, CsrBtAvrcpCtNotiRegisterReqSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_REGISTER_REQ */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_SYSTEM_STATUS_IND
    { CSR_BT_AVRCP_CT_NOTI_SYSTEM_STATUS_IND, CsrBtAvrcpCtNotiSystemStatusIndSizeof, CsrBtAvrcpCtNotiSystemStatusIndSer, CsrBtAvrcpCtNotiSystemStatusIndDes, CsrBtAvrcpCtNotiSystemStatusIndSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_SYSTEM_STATUS_IND */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_TRACK_CHANGED_IND
    { CSR_BT_AVRCP_CT_NOTI_TRACK_CHANGED_IND, CsrBtAvrcpCtNotiTrackChangedIndSizeof, CsrBtAvrcpCtNotiTrackChangedIndSer, CsrBtAvrcpCtNotiTrackChangedIndDes, CsrBtAvrcpCtNotiTrackChangedIndSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_TRACK_CHANGED_IND */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_TRACK_END_IND
    { CSR_BT_AVRCP_CT_NOTI_TRACK_END_IND, CsrBtAvrcpCtNotiTrackEndIndSizeof, CsrBtAvrcpCtNotiTrackEndIndSer, CsrBtAvrcpCtNotiTrackEndIndDes, CsrBtAvrcpCtNotiTrackEndIndSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_TRACK_END_IND */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_TRACK_START_IND
    { CSR_BT_AVRCP_CT_NOTI_TRACK_START_IND, CsrBtAvrcpCtNotiTrackStartIndSizeof, CsrBtAvrcpCtNotiTrackStartIndSer, CsrBtAvrcpCtNotiTrackStartIndDes, CsrBtAvrcpCtNotiTrackStartIndSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_TRACK_START_IND */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_UIDS_IND
    { CSR_BT_AVRCP_CT_NOTI_UIDS_IND, CsrBtAvrcpCtNotiUidsIndSizeof, CsrBtAvrcpCtNotiUidsIndSer, CsrBtAvrcpCtNotiUidsIndDes, CsrBtAvrcpCtNotiUidsIndSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_UIDS_IND */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_VOLUME_IND
    { CSR_BT_AVRCP_CT_NOTI_VOLUME_IND, CsrBtAvrcpCtNotiVolumeIndSizeof, CsrBtAvrcpCtNotiVolumeIndSer, CsrBtAvrcpCtNotiVolumeIndDes, CsrBtAvrcpCtNotiVolumeIndSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_VOLUME_IND */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_ID_CFM
    { CSR_BT_AVRCP_CT_PAS_ATT_ID_CFM, CsrBtAvrcpCtPasAttIdCfmSizeof, CsrBtAvrcpCtPasAttIdCfmSer, CsrBtAvrcpCtPasAttIdCfmDes, CsrBtAvrcpCtPasAttIdCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_ID_CFM */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_ID_REQ
    { CSR_BT_AVRCP_CT_PAS_ATT_ID_REQ, CsrBtAvrcpCtPasAttIdReqSizeof, CsrBtAvrcpCtPasAttIdReqSer, CsrBtAvrcpCtPasAttIdReqDes, CsrBtAvrcpCtPasAttIdReqSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_ID_REQ */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_CFM
    { CSR_BT_AVRCP_CT_PAS_ATT_TXT_CFM, CsrBtAvrcpCtPasAttTxtCfmSizeof, CsrBtAvrcpCtPasAttTxtCfmSer, CsrBtAvrcpCtPasAttTxtCfmDes, CsrBtAvrcpCtPasAttTxtCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_CFM */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_IND
    { CSR_BT_AVRCP_CT_PAS_ATT_TXT_IND, CsrBtAvrcpCtPasAttTxtIndSizeof, CsrBtAvrcpCtPasAttTxtIndSer, CsrBtAvrcpCtPasAttTxtIndDes, CsrBtAvrcpCtPasAttTxtIndSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_IND */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_REQ
    { CSR_BT_AVRCP_CT_PAS_ATT_TXT_REQ, CsrBtAvrcpCtPasAttTxtReqSizeof, CsrBtAvrcpCtPasAttTxtReqSer, CsrBtAvrcpCtPasAttTxtReqDes, CsrBtAvrcpCtPasAttTxtReqSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_REQ */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_RES
    { CSR_BT_AVRCP_CT_PAS_ATT_TXT_RES, CsrBtAvrcpCtPasAttTxtResSizeof, CsrBtAvrcpCtPasAttTxtResSer, CsrBtAvrcpCtPasAttTxtResDes, CsrBtAvrcpCtPasAttTxtResSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_RES */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_CURRENT_CFM
    { CSR_BT_AVRCP_CT_PAS_CURRENT_CFM, CsrBtAvrcpCtPasCurrentCfmSizeof, CsrBtAvrcpCtPasCurrentCfmSer, CsrBtAvrcpCtPasCurrentCfmDes, CsrBtAvrcpCtPasCurrentCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_CURRENT_CFM */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_CURRENT_REQ
    { CSR_BT_AVRCP_CT_PAS_CURRENT_REQ, CsrBtAvrcpCtPasCurrentReqSizeof, CsrBtAvrcpCtPasCurrentReqSer, CsrBtAvrcpCtPasCurrentReqDes, CsrBtAvrcpCtPasCurrentReqSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_CURRENT_REQ */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_SET_CFM
    { CSR_BT_AVRCP_CT_PAS_SET_CFM, CsrBtAvrcpCtPasSetCfmSizeof, CsrBtAvrcpCtPasSetCfmSer, CsrBtAvrcpCtPasSetCfmDes, CsrBtAvrcpCtPasSetCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_SET_CFM */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_SET_IND
    { CSR_BT_AVRCP_CT_PAS_SET_IND, CsrBtAvrcpCtPasSetIndSizeof, CsrBtAvrcpCtPasSetIndSer, CsrBtAvrcpCtPasSetIndDes, CsrBtAvrcpCtPasSetIndSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_SET_IND */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_SET_REQ
    { CSR_BT_AVRCP_CT_PAS_SET_REQ, CsrBtAvrcpCtPasSetReqSizeof, CsrBtAvrcpCtPasSetReqSer, CsrBtAvrcpCtPasSetReqDes, CsrBtAvrcpCtPasSetReqSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_SET_REQ */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_ID_CFM
    { CSR_BT_AVRCP_CT_PAS_VAL_ID_CFM, CsrBtAvrcpCtPasValIdCfmSizeof, CsrBtAvrcpCtPasValIdCfmSer, CsrBtAvrcpCtPasValIdCfmDes, CsrBtAvrcpCtPasValIdCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_ID_CFM */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_ID_REQ
    { CSR_BT_AVRCP_CT_PAS_VAL_ID_REQ, CsrBtAvrcpCtPasValIdReqSizeof, CsrBtAvrcpCtPasValIdReqSer, CsrBtAvrcpCtPasValIdReqDes, CsrBtAvrcpCtPasValIdReqSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_ID_REQ */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_CFM
    { CSR_BT_AVRCP_CT_PAS_VAL_TXT_CFM, CsrBtAvrcpCtPasValTxtCfmSizeof, CsrBtAvrcpCtPasValTxtCfmSer, CsrBtAvrcpCtPasValTxtCfmDes, CsrBtAvrcpCtPasValTxtCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_CFM */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_IND
    { CSR_BT_AVRCP_CT_PAS_VAL_TXT_IND, CsrBtAvrcpCtPasValTxtIndSizeof, CsrBtAvrcpCtPasValTxtIndSer, CsrBtAvrcpCtPasValTxtIndDes, CsrBtAvrcpCtPasValTxtIndSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_IND */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_REQ
    { CSR_BT_AVRCP_CT_PAS_VAL_TXT_REQ, CsrBtAvrcpCtPasValTxtReqSizeof, CsrBtAvrcpCtPasValTxtReqSer, CsrBtAvrcpCtPasValTxtReqDes, CsrBtAvrcpCtPasValTxtReqSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_REQ */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_RES
    { CSR_BT_AVRCP_CT_PAS_VAL_TXT_RES, CsrBtAvrcpCtPasValTxtResSizeof, CsrBtAvrcpCtPasValTxtResSer, CsrBtAvrcpCtPasValTxtResDes, CsrBtAvrcpCtPasValTxtResSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_RES */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PASS_THROUGH_CFM
    { CSR_BT_AVRCP_CT_PASS_THROUGH_CFM, CsrBtAvrcpCtPassThroughCfmSizeof, CsrBtAvrcpCtPassThroughCfmSer, CsrBtAvrcpCtPassThroughCfmDes, CsrBtAvrcpCtPassThroughCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PASS_THROUGH_CFM */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PASS_THROUGH_REQ
    { CSR_BT_AVRCP_CT_PASS_THROUGH_REQ, CsrBtAvrcpCtPassThroughReqSizeof, CsrBtAvrcpCtPassThroughReqSer, CsrBtAvrcpCtPassThroughReqDes, CsrBtAvrcpCtPassThroughReqSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PASS_THROUGH_REQ */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PLAY_CFM
    { CSR_BT_AVRCP_CT_PLAY_CFM, CsrBtAvrcpCtPlayCfmSizeof, CsrBtAvrcpCtPlayCfmSer, CsrBtAvrcpCtPlayCfmDes, CsrBtAvrcpCtPlayCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PLAY_CFM */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PLAY_REQ
    { CSR_BT_AVRCP_CT_PLAY_REQ, CsrBtAvrcpCtPlayReqSizeof, CsrBtAvrcpCtPlayReqSer, CsrBtAvrcpCtPlayReqDes, CsrBtAvrcpCtPlayReqSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PLAY_REQ */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SEARCH_CFM
    { CSR_BT_AVRCP_CT_SEARCH_CFM, CsrBtAvrcpCtSearchCfmSizeof, CsrBtAvrcpCtSearchCfmSer, CsrBtAvrcpCtSearchCfmDes, CsrBtAvrcpCtSearchCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SEARCH_CFM */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SEARCH_REQ
    { CSR_BT_AVRCP_CT_SEARCH_REQ, CsrBtAvrcpCtSearchReqSizeof, CsrBtAvrcpCtSearchReqSer, CsrBtAvrcpCtSearchReqDes, CsrBtAvrcpCtSearchReqSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SEARCH_REQ */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_CFM
    { CSR_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_CFM, CsrBtAvrcpCtSetAddressedPlayerCfmSizeof, CsrBtAvrcpCtSetAddressedPlayerCfmSer, CsrBtAvrcpCtSetAddressedPlayerCfmDes, CsrBtAvrcpCtSetAddressedPlayerCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_CFM */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_IND
    { CSR_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_IND, CsrBtAvrcpCtSetAddressedPlayerIndSizeof, CsrBtAvrcpCtSetAddressedPlayerIndSer, CsrBtAvrcpCtSetAddressedPlayerIndDes, CsrBtAvrcpCtSetAddressedPlayerIndSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_IND */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_REQ
    { CSR_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_REQ, CsrBtAvrcpCtSetAddressedPlayerReqSizeof, CsrBtAvrcpCtSetAddressedPlayerReqSer, CsrBtAvrcpCtSetAddressedPlayerReqDes, CsrBtAvrcpCtSetAddressedPlayerReqSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_REQ */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SET_BROWSED_PLAYER_CFM
    { CSR_BT_AVRCP_CT_SET_BROWSED_PLAYER_CFM, CsrBtAvrcpCtSetBrowsedPlayerCfmSizeof, CsrBtAvrcpCtSetBrowsedPlayerCfmSer, CsrBtAvrcpCtSetBrowsedPlayerCfmDes, CsrBtAvrcpCtSetBrowsedPlayerCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SET_BROWSED_PLAYER_CFM */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SET_BROWSED_PLAYER_REQ
    { CSR_BT_AVRCP_CT_SET_BROWSED_PLAYER_REQ, CsrBtAvrcpCtSetBrowsedPlayerReqSizeof, CsrBtAvrcpCtSetBrowsedPlayerReqSer, CsrBtAvrcpCtSetBrowsedPlayerReqDes, CsrBtAvrcpCtSetBrowsedPlayerReqSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SET_BROWSED_PLAYER_REQ */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SET_VOLUME_CFM
    { CSR_BT_AVRCP_CT_SET_VOLUME_CFM, CsrBtAvrcpCtSetVolumeCfmSizeof, CsrBtAvrcpCtSetVolumeCfmSer, CsrBtAvrcpCtSetVolumeCfmDes, CsrBtAvrcpCtSetVolumeCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SET_VOLUME_CFM */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SET_VOLUME_REQ
    { CSR_BT_AVRCP_CT_SET_VOLUME_REQ, CsrBtAvrcpCtSetVolumeReqSizeof, CsrBtAvrcpCtSetVolumeReqSer, CsrBtAvrcpCtSetVolumeReqDes, CsrBtAvrcpCtSetSSVolumeReqSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SET_VOLUME_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SS_SET_VOLUME_REQ
		{ CSR_BT_AVRCP_CT_SET_SS_VOLUME_REQ, CsrBtAvrcpCtSetSSVolumeReqSizeof, CsrBtAvrcpCtSetSSVolumeReqSer, CsrBtAvrcpCtSetSSVolumeReqDes, CsrBtAvrcpCtSetSSVolumeReqSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SS_SET_VOLUME_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SUB_UNIT_INFO_CMD_CFM
    { CSR_BT_AVRCP_CT_SUB_UNIT_INFO_CMD_CFM, CsrBtAvrcpCtSubUnitInfoCmdCfmSizeof, CsrBtAvrcpCtSubUnitInfoCmdCfmSer, CsrBtAvrcpCtSubUnitInfoCmdCfmDes, CsrBtAvrcpCtSubUnitInfoCmdCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SUB_UNIT_INFO_CMD_CFM */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SUB_UNIT_INFO_CMD_REQ
    { CSR_BT_AVRCP_CT_SUB_UNIT_INFO_CMD_REQ, CsrBtAvrcpCtSubUnitInfoCmdReqSizeof, CsrBtAvrcpCtSubUnitInfoCmdReqSer, CsrBtAvrcpCtSubUnitInfoCmdReqDes, CsrBtAvrcpCtSubUnitInfoCmdReqSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SUB_UNIT_INFO_CMD_REQ */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_UNIT_INFO_CMD_CFM
    { CSR_BT_AVRCP_CT_UNIT_INFO_CMD_CFM, CsrBtAvrcpCtUnitInfoCmdCfmSizeof, CsrBtAvrcpCtUnitInfoCmdCfmSer, CsrBtAvrcpCtUnitInfoCmdCfmDes, CsrBtAvrcpCtUnitInfoCmdCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_UNIT_INFO_CMD_CFM */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_UNIT_INFO_CMD_REQ
    { CSR_BT_AVRCP_CT_UNIT_INFO_CMD_REQ, CsrBtAvrcpCtUnitInfoCmdReqSizeof, CsrBtAvrcpCtUnitInfoCmdReqSer, CsrBtAvrcpCtUnitInfoCmdReqDes, CsrBtAvrcpCtUnitInfoCmdReqSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_UNIT_INFO_CMD_REQ */
#ifndef EXCLUDE_CSR_BT_AVRCP_DEACTIVATE_CFM
    { CSR_BT_AVRCP_DEACTIVATE_CFM, CsrBtAvrcpDeactivateCfmSizeof, CsrBtAvrcpDeactivateCfmSer, CsrBtAvrcpDeactivateCfmDes, CsrBtAvrcpDeactivateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_DEACTIVATE_CFM */
#ifndef EXCLUDE_CSR_BT_AVRCP_DEACTIVATE_REQ
    { CSR_BT_AVRCP_DEACTIVATE_REQ, CsrBtAvrcpDeactivateReqSizeof, CsrBtAvrcpDeactivateReqSer, CsrBtAvrcpDeactivateReqDes, CsrBtAvrcpDeactivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_DEACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_AVRCP_DISCONNECT_IND
    { CSR_BT_AVRCP_DISCONNECT_IND, CsrBtAvrcpDisconnectIndSizeof, CsrBtAvrcpDisconnectIndSer, CsrBtAvrcpDisconnectIndDes, CsrBtAvrcpDisconnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_DISCONNECT_IND */
#ifndef EXCLUDE_CSR_BT_AVRCP_DISCONNECT_REQ
    { CSR_BT_AVRCP_DISCONNECT_REQ, CsrBtAvrcpDisconnectReqSizeof, CsrBtAvrcpDisconnectReqSer, CsrBtAvrcpDisconnectReqDes, CsrBtAvrcpDisconnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_DISCONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_AVRCP_HOUSE_CLEANING
    { CSR_BT_AVRCP_HOUSE_CLEANING, CsrBtAvrcpHouseCleaningSizeof, CsrBtAvrcpHouseCleaningSer, CsrBtAvrcpHouseCleaningDes, CsrBtAvrcpHouseCleaningSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_HOUSE_CLEANING */
#ifndef EXCLUDE_CSR_BT_AVRCP_REMOTE_FEATURES_IND
    { CSR_BT_AVRCP_REMOTE_FEATURES_IND, CsrBtAvrcpRemoteFeaturesIndSizeof, CsrBtAvrcpRemoteFeaturesIndSer, CsrBtAvrcpRemoteFeaturesIndDes, CsrBtAvrcpRemoteFeaturesIndSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_REMOTE_FEATURES_IND */
#ifndef EXCLUDE_CSR_BT_AVRCP_SECURITY_IN_CFM
    { CSR_BT_AVRCP_SECURITY_IN_CFM, CsrBtAvrcpSecurityInCfmSizeof, CsrBtAvrcpSecurityInCfmSer, CsrBtAvrcpSecurityInCfmDes, CsrBtAvrcpSecurityInCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_SECURITY_IN_CFM */
#ifndef EXCLUDE_CSR_BT_AVRCP_SECURITY_IN_REQ
    { CSR_BT_AVRCP_SECURITY_IN_REQ, CsrBtAvrcpSecurityInReqSizeof, CsrBtAvrcpSecurityInReqSer, CsrBtAvrcpSecurityInReqDes, CsrBtAvrcpSecurityInReqSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_SECURITY_IN_REQ */
#ifndef EXCLUDE_CSR_BT_AVRCP_SECURITY_OUT_CFM
    { CSR_BT_AVRCP_SECURITY_OUT_CFM, CsrBtAvrcpSecurityOutCfmSizeof, CsrBtAvrcpSecurityOutCfmSer, CsrBtAvrcpSecurityOutCfmDes, CsrBtAvrcpSecurityOutCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_SECURITY_OUT_CFM */
#ifndef EXCLUDE_CSR_BT_AVRCP_SECURITY_OUT_REQ
    { CSR_BT_AVRCP_SECURITY_OUT_REQ, CsrBtAvrcpSecurityOutReqSizeof, CsrBtAvrcpSecurityOutReqSer, CsrBtAvrcpSecurityOutReqDes, CsrBtAvrcpSecurityOutReqSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_SECURITY_OUT_REQ */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_ADD_TO_NOW_PLAYING_IND
    { CSR_BT_AVRCP_TG_ADD_TO_NOW_PLAYING_IND, CsrBtAvrcpTgAddToNowPlayingIndSizeof, CsrBtAvrcpTgAddToNowPlayingIndSer, CsrBtAvrcpTgAddToNowPlayingIndDes, CsrBtAvrcpTgAddToNowPlayingIndSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_ADD_TO_NOW_PLAYING_IND */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_ADD_TO_NOW_PLAYING_RES
    { CSR_BT_AVRCP_TG_ADD_TO_NOW_PLAYING_RES, CsrBtAvrcpTgAddToNowPlayingResSizeof, CsrBtAvrcpTgAddToNowPlayingResSer, CsrBtAvrcpTgAddToNowPlayingResDes, CsrBtAvrcpTgAddToNowPlayingResSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_ADD_TO_NOW_PLAYING_RES */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_CHANGE_PATH_IND
    { CSR_BT_AVRCP_TG_CHANGE_PATH_IND, CsrBtAvrcpTgChangePathIndSizeof, CsrBtAvrcpTgChangePathIndSer, CsrBtAvrcpTgChangePathIndDes, CsrBtAvrcpTgChangePathIndSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_CHANGE_PATH_IND */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_CHANGE_PATH_RES
    { CSR_BT_AVRCP_TG_CHANGE_PATH_RES, CsrBtAvrcpTgChangePathResSizeof, CsrBtAvrcpTgChangePathResSer, CsrBtAvrcpTgChangePathResDes, CsrBtAvrcpTgChangePathResSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_CHANGE_PATH_RES */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_GET_ATTRIBUTES_IND
    { CSR_BT_AVRCP_TG_GET_ATTRIBUTES_IND, CsrBtAvrcpTgGetAttributesIndSizeof, CsrBtAvrcpTgGetAttributesIndSer, CsrBtAvrcpTgGetAttributesIndDes, CsrBtAvrcpTgGetAttributesIndSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_GET_ATTRIBUTES_IND */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_GET_ATTRIBUTES_RES
    { CSR_BT_AVRCP_TG_GET_ATTRIBUTES_RES, CsrBtAvrcpTgGetAttributesResSizeof, CsrBtAvrcpTgGetAttributesResSer, CsrBtAvrcpTgGetAttributesResDes, CsrBtAvrcpTgGetAttributesResSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_GET_ATTRIBUTES_RES */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_GET_FOLDER_ITEMS_IND
    { CSR_BT_AVRCP_TG_GET_FOLDER_ITEMS_IND, CsrBtAvrcpTgGetFolderItemsIndSizeof, CsrBtAvrcpTgGetFolderItemsIndSer, CsrBtAvrcpTgGetFolderItemsIndDes, CsrBtAvrcpTgGetFolderItemsIndSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_GET_FOLDER_ITEMS_IND */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_GET_FOLDER_ITEMS_RES
    { CSR_BT_AVRCP_TG_GET_FOLDER_ITEMS_RES, CsrBtAvrcpTgGetFolderItemsResSizeof, CsrBtAvrcpTgGetFolderItemsResSer, CsrBtAvrcpTgGetFolderItemsResDes, CsrBtAvrcpTgGetFolderItemsResSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_GET_FOLDER_ITEMS_RES */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_GET_PLAY_STATUS_IND
    { CSR_BT_AVRCP_TG_GET_PLAY_STATUS_IND, CsrBtAvrcpTgGetPlayStatusIndSizeof, CsrBtAvrcpTgGetPlayStatusIndSer, CsrBtAvrcpTgGetPlayStatusIndDes, CsrBtAvrcpTgGetPlayStatusIndSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_GET_PLAY_STATUS_IND */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_GET_PLAY_STATUS_RES
    { CSR_BT_AVRCP_TG_GET_PLAY_STATUS_RES, CsrBtAvrcpTgGetPlayStatusResSizeof, CsrBtAvrcpTgGetPlayStatusResSer, CsrBtAvrcpTgGetPlayStatusResDes, CsrBtAvrcpTgGetPlayStatusResSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_GET_PLAY_STATUS_RES */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_INFORM_BATTERY_STATUS_IND
    { CSR_BT_AVRCP_TG_INFORM_BATTERY_STATUS_IND, CsrBtAvrcpTgInformBatteryStatusIndSizeof, CsrBtAvrcpTgInformBatteryStatusIndSer, CsrBtAvrcpTgInformBatteryStatusIndDes, CsrBtAvrcpTgInformBatteryStatusIndSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_INFORM_BATTERY_STATUS_IND */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_INFORM_DISP_CHARSET_IND
    { CSR_BT_AVRCP_TG_INFORM_DISP_CHARSET_IND, CsrBtAvrcpTgInformDispCharsetIndSizeof, CsrBtAvrcpTgInformDispCharsetIndSer, CsrBtAvrcpTgInformDispCharsetIndDes, CsrBtAvrcpTgInformDispCharsetIndSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_INFORM_DISP_CHARSET_IND */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MP_REGISTER_CFM
    { CSR_BT_AVRCP_TG_MP_REGISTER_CFM, CsrBtAvrcpTgMpRegisterCfmSizeof, CsrBtAvrcpTgMpRegisterCfmSer, CsrBtAvrcpTgMpRegisterCfmDes, CsrBtAvrcpTgMpRegisterCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_MP_REGISTER_CFM */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MP_REGISTER_REQ
    { CSR_BT_AVRCP_TG_MP_REGISTER_REQ, CsrBtAvrcpTgMpRegisterReqSizeof, CsrBtAvrcpTgMpRegisterReqSer, CsrBtAvrcpTgMpRegisterReqDes, CsrBtAvrcpTgMpRegisterReqSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_MP_REGISTER_REQ */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MP_UNREGISTER_CFM
    { CSR_BT_AVRCP_TG_MP_UNREGISTER_CFM, CsrBtAvrcpTgMpUnregisterCfmSizeof, CsrBtAvrcpTgMpUnregisterCfmSer, CsrBtAvrcpTgMpUnregisterCfmDes, CsrBtAvrcpTgMpUnregisterCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_MP_UNREGISTER_CFM */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MP_UNREGISTER_REQ
    { CSR_BT_AVRCP_TG_MP_UNREGISTER_REQ, CsrBtAvrcpTgMpUnregisterReqSizeof, CsrBtAvrcpTgMpUnregisterReqSer, CsrBtAvrcpTgMpUnregisterReqDes, CsrBtAvrcpTgMpUnregisterReqSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_MP_UNREGISTER_REQ */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_NOTI_CFM
    { CSR_BT_AVRCP_TG_NOTI_CFM, CsrBtAvrcpTgNotiCfmSizeof, CsrBtAvrcpTgNotiCfmSer, CsrBtAvrcpTgNotiCfmDes, CsrBtAvrcpTgNotiCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_NOTI_CFM */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_NOTI_IND
    { CSR_BT_AVRCP_TG_NOTI_IND, CsrBtAvrcpTgNotiIndSizeof, CsrBtAvrcpTgNotiIndSer, CsrBtAvrcpTgNotiIndDes, CsrBtAvrcpTgNotiIndSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_NOTI_IND */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_NOTI_REQ
    { CSR_BT_AVRCP_TG_NOTI_REQ, CsrBtAvrcpTgNotiReqSizeof, CsrBtAvrcpTgNotiReqSer, CsrBtAvrcpTgNotiReqDes, CsrBtAvrcpTgNotiReqSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_NOTI_REQ */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_NOTI_RES
    { CSR_BT_AVRCP_TG_NOTI_RES, CsrBtAvrcpTgNotiResSizeof, CsrBtAvrcpTgNotiResSer, CsrBtAvrcpTgNotiResDes, CsrBtAvrcpTgNotiResSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_NOTI_RES */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PAS_CURRENT_IND
    { CSR_BT_AVRCP_TG_PAS_CURRENT_IND, CsrBtAvrcpTgPasCurrentIndSizeof, CsrBtAvrcpTgPasCurrentIndSer, CsrBtAvrcpTgPasCurrentIndDes, CsrBtAvrcpTgPasCurrentIndSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PAS_CURRENT_IND */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PAS_CURRENT_RES
    { CSR_BT_AVRCP_TG_PAS_CURRENT_RES, CsrBtAvrcpTgPasCurrentResSizeof, CsrBtAvrcpTgPasCurrentResSer, CsrBtAvrcpTgPasCurrentResDes, CsrBtAvrcpTgPasCurrentResSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PAS_CURRENT_RES */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_CFM
    { CSR_BT_AVRCP_TG_PAS_SET_CFM, CsrBtAvrcpTgPasSetCfmSizeof, CsrBtAvrcpTgPasSetCfmSer, CsrBtAvrcpTgPasSetCfmDes, CsrBtAvrcpTgPasSetCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_CFM */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_IND
    { CSR_BT_AVRCP_TG_PAS_SET_IND, CsrBtAvrcpTgPasSetIndSizeof, CsrBtAvrcpTgPasSetIndSer, CsrBtAvrcpTgPasSetIndDes, CsrBtAvrcpTgPasSetIndSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_IND */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_REQ
    { CSR_BT_AVRCP_TG_PAS_SET_REQ, CsrBtAvrcpTgPasSetReqSizeof, CsrBtAvrcpTgPasSetReqSer, CsrBtAvrcpTgPasSetReqDes, CsrBtAvrcpTgPasSetReqSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_REQ */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_RES
    { CSR_BT_AVRCP_TG_PAS_SET_RES, CsrBtAvrcpTgPasSetResSizeof, CsrBtAvrcpTgPasSetResSer, CsrBtAvrcpTgPasSetResDes, CsrBtAvrcpTgPasSetResSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_RES */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PASS_THROUGH_IND
    { CSR_BT_AVRCP_TG_PASS_THROUGH_IND, CsrBtAvrcpTgPassThroughIndSizeof, CsrBtAvrcpTgPassThroughIndSer, CsrBtAvrcpTgPassThroughIndDes, CsrBtAvrcpTgPassThroughIndSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PASS_THROUGH_IND */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PASS_THROUGH_RES
    { CSR_BT_AVRCP_TG_PASS_THROUGH_RES, CsrBtAvrcpTgPassThroughResSizeof, CsrBtAvrcpTgPassThroughResSer, CsrBtAvrcpTgPassThroughResDes, CsrBtAvrcpTgPassThroughResSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PASS_THROUGH_RES */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PLAY_IND
    { CSR_BT_AVRCP_TG_PLAY_IND, CsrBtAvrcpTgPlayIndSizeof, CsrBtAvrcpTgPlayIndSer, CsrBtAvrcpTgPlayIndDes, CsrBtAvrcpTgPlayIndSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PLAY_IND */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PLAY_RES
    { CSR_BT_AVRCP_TG_PLAY_RES, CsrBtAvrcpTgPlayResSizeof, CsrBtAvrcpTgPlayResSer, CsrBtAvrcpTgPlayResDes, CsrBtAvrcpTgPlayResSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PLAY_RES */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SEARCH_IND
    { CSR_BT_AVRCP_TG_SEARCH_IND, CsrBtAvrcpTgSearchIndSizeof, CsrBtAvrcpTgSearchIndSer, CsrBtAvrcpTgSearchIndDes, CsrBtAvrcpTgSearchIndSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SEARCH_IND */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SEARCH_RES
    { CSR_BT_AVRCP_TG_SEARCH_RES, CsrBtAvrcpTgSearchResSizeof, CsrBtAvrcpTgSearchResSer, CsrBtAvrcpTgSearchResDes, CsrBtAvrcpTgSearchResSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SEARCH_RES */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_CFM
    { CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_CFM, CsrBtAvrcpTgSetAddressedPlayerCfmSizeof, CsrBtAvrcpTgSetAddressedPlayerCfmSer, CsrBtAvrcpTgSetAddressedPlayerCfmDes, CsrBtAvrcpTgSetAddressedPlayerCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_CFM */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_IND
    { CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_IND, CsrBtAvrcpTgSetAddressedPlayerIndSizeof, CsrBtAvrcpTgSetAddressedPlayerIndSer, CsrBtAvrcpTgSetAddressedPlayerIndDes, CsrBtAvrcpTgSetAddressedPlayerIndSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_IND */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_REQ
    { CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_REQ, CsrBtAvrcpTgSetAddressedPlayerReqSizeof, CsrBtAvrcpTgSetAddressedPlayerReqSer, CsrBtAvrcpTgSetAddressedPlayerReqDes, CsrBtAvrcpTgSetAddressedPlayerReqSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_REQ */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_RES
    { CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_RES, CsrBtAvrcpTgSetAddressedPlayerResSizeof, CsrBtAvrcpTgSetAddressedPlayerResSer, CsrBtAvrcpTgSetAddressedPlayerResDes, CsrBtAvrcpTgSetAddressedPlayerResSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_RES */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SET_BROWSED_PLAYER_IND
    { CSR_BT_AVRCP_TG_SET_BROWSED_PLAYER_IND, CsrBtAvrcpTgSetBrowsedPlayerIndSizeof, CsrBtAvrcpTgSetBrowsedPlayerIndSer, CsrBtAvrcpTgSetBrowsedPlayerIndDes, CsrBtAvrcpTgSetBrowsedPlayerIndSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SET_BROWSED_PLAYER_IND */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SET_BROWSED_PLAYER_RES
    { CSR_BT_AVRCP_TG_SET_BROWSED_PLAYER_RES, CsrBtAvrcpTgSetBrowsedPlayerResSizeof, CsrBtAvrcpTgSetBrowsedPlayerResSer, CsrBtAvrcpTgSetBrowsedPlayerResDes, CsrBtAvrcpTgSetBrowsedPlayerResSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SET_BROWSED_PLAYER_RES */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SET_VOLUME_IND
    { CSR_BT_AVRCP_TG_SET_VOLUME_IND, CsrBtAvrcpTgSetVolumeIndSizeof, CsrBtAvrcpTgSetVolumeIndSer, CsrBtAvrcpTgSetVolumeIndDes, CsrBtAvrcpTgSetVolumeIndSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SET_VOLUME_IND */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SET_VOLUME_RES
    { CSR_BT_AVRCP_TG_SET_VOLUME_RES, CsrBtAvrcpTgSetVolumeResSizeof, CsrBtAvrcpTgSetVolumeResSer, CsrBtAvrcpTgSetVolumeResDes, CsrBtAvrcpTgSetVolumeResSerFree },
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SET_VOLUME_RES */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtAvrcpConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_AVRCP_PRIM, csr_bt_avrcp_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_avrcp_conv_info = {
    CSR_BT_AVRCP_PRIM,
    "CSR_BT_AVRCP_PRIM",
    csr_bt_avrcp_conv_lut
};

CsrLogPrimitiveInformation* CsrBtAvrcpTechInfoGet(void)
{
    return &csr_bt_avrcp_conv_info;
}
#endif
#endif

