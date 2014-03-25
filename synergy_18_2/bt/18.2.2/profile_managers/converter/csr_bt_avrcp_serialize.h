#ifndef CSR_BT_AVRCP_SERIALIZE_H__
#define CSR_BT_AVRCP_SERIALIZE_H__
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

void CsrBtAvrcpPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_AVRCP_SECURITY_OUT_REQ
CsrSize CsrBtAvrcpSecurityOutReqSizeof(void *msg);
CsrUint8 *CsrBtAvrcpSecurityOutReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpSecurityOutReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpSecurityOutReqSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_CHANGE_PATH_REQ
CsrSize CsrBtAvrcpCtChangePathReqSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtChangePathReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtChangePathReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpCtChangePathReqSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_CHANGE_PATH_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_CHANGE_PATH_IND
CsrSize CsrBtAvrcpTgChangePathIndSizeof(void *msg);
CsrUint8 *CsrBtAvrcpTgChangePathIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpTgChangePathIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpTgChangePathIndSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_CHANGE_PATH_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PLAY_REQ
CsrSize CsrBtAvrcpCtPlayReqSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtPlayReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtPlayReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpCtPlayReqSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PLAY_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PLAY_IND
CsrSize CsrBtAvrcpTgPlayIndSizeof(void *msg);
CsrUint8 *CsrBtAvrcpTgPlayIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpTgPlayIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpTgPlayIndSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PLAY_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_REQ
CsrSize CsrBtAvrcpCtGetAttributesReqSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtGetAttributesReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtGetAttributesReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpCtGetAttributesReqSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_DISCONNECT_REQ
CsrSize CsrBtAvrcpDisconnectReqSizeof(void *msg);
CsrUint8 *CsrBtAvrcpDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpDisconnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpDisconnectReqSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CONFIG_REQ
CsrSize CsrBtAvrcpConfigReqSizeof(void *msg);
CsrUint8 *CsrBtAvrcpConfigReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpConfigReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvrcpConfigReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AVRCP_CONFIG_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_CFM
CsrSize CsrBtAvrcpTgSetAddressedPlayerCfmSizeof(void *msg);
CsrUint8 *CsrBtAvrcpTgSetAddressedPlayerCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpTgSetAddressedPlayerCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpTgSetAddressedPlayerCfmSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_REQ
CsrSize CsrBtAvrcpTgSetAddressedPlayerReqSizeof(void *msg);
CsrUint8 *CsrBtAvrcpTgSetAddressedPlayerReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpTgSetAddressedPlayerReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpTgSetAddressedPlayerReqSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SET_BROWSED_PLAYER_IND
CsrSize CsrBtAvrcpTgSetBrowsedPlayerIndSizeof(void *msg);
CsrUint8 *CsrBtAvrcpTgSetBrowsedPlayerIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpTgSetBrowsedPlayerIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpTgSetBrowsedPlayerIndSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SET_BROWSED_PLAYER_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_GET_PLAY_STATUS_CFM
CsrSize CsrBtAvrcpCtGetPlayStatusCfmSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtGetPlayStatusCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtGetPlayStatusCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpCtGetPlayStatusCfmSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_GET_PLAY_STATUS_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_ADD_TO_NOW_PLAYING_CFM
CsrSize CsrBtAvrcpCtAddToNowPlayingCfmSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtAddToNowPlayingCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtAddToNowPlayingCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpCtAddToNowPlayingCfmSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_ADD_TO_NOW_PLAYING_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_REQ
CsrSize CsrBtAvrcpCtPasValTxtReqSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtPasValTxtReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtPasValTxtReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvrcpCtPasValTxtReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_REQ
CsrSize CsrBtAvrcpTgPasSetReqSizeof(void *msg);
CsrUint8 *CsrBtAvrcpTgPasSetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpTgPasSetReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvrcpTgPasSetReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PASS_THROUGH_IND
CsrSize CsrBtAvrcpTgPassThroughIndSizeof(void *msg);
CsrUint8 *CsrBtAvrcpTgPassThroughIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpTgPassThroughIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpTgPassThroughIndSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PASS_THROUGH_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PASS_THROUGH_CFM
CsrSize CsrBtAvrcpCtPassThroughCfmSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtPassThroughCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtPassThroughCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpCtPassThroughCfmSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PASS_THROUGH_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SET_VOLUME_CFM
CsrSize CsrBtAvrcpCtSetVolumeCfmSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtSetVolumeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtSetVolumeCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpCtSetVolumeCfmSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SET_VOLUME_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CONFIG_CFM
CsrSize CsrBtAvrcpConfigCfmSizeof(void *msg);
CsrUint8 *CsrBtAvrcpConfigCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpConfigCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpConfigCfmSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_CONFIG_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_NOTI_CFM
CsrSize CsrBtAvrcpTgNotiCfmSizeof(void *msg);
CsrUint8 *CsrBtAvrcpTgNotiCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpTgNotiCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpTgNotiCfmSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_NOTI_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SET_VOLUME_IND
CsrSize CsrBtAvrcpTgSetVolumeIndSizeof(void *msg);
CsrUint8 *CsrBtAvrcpTgSetVolumeIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpTgSetVolumeIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpTgSetVolumeIndSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SET_VOLUME_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_GET_FOLDER_ITEMS_REQ
CsrSize CsrBtAvrcpCtGetFolderItemsReqSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtGetFolderItemsReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtGetFolderItemsReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpCtGetFolderItemsReqSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_GET_FOLDER_ITEMS_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_IND
CsrSize CsrBtAvrcpCtPasValTxtIndSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtPasValTxtIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtPasValTxtIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvrcpCtPasValTxtIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_IND
CsrSize CsrBtAvrcpTgPasSetIndSizeof(void *msg);
CsrUint8 *CsrBtAvrcpTgPasSetIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpTgPasSetIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvrcpTgPasSetIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_PLAYBACK_POS_IND
CsrSize CsrBtAvrcpCtNotiPlaybackPosIndSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtNotiPlaybackPosIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtNotiPlaybackPosIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpCtNotiPlaybackPosIndSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_PLAYBACK_POS_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_IND
CsrSize CsrBtAvrcpCtSetAddressedPlayerIndSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtSetAddressedPlayerIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtSetAddressedPlayerIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpCtSetAddressedPlayerIndSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MP_UNREGISTER_REQ
CsrSize CsrBtAvrcpTgMpUnregisterReqSizeof(void *msg);
CsrUint8 *CsrBtAvrcpTgMpUnregisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpTgMpUnregisterReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpTgMpUnregisterReqSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_MP_UNREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_INFORM_BATTERY_STATUS_REQ
CsrSize CsrBtAvrcpCtInformBatteryStatusReqSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtInformBatteryStatusReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtInformBatteryStatusReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpCtInformBatteryStatusReqSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_INFORM_BATTERY_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CANCEL_CONNECT_REQ
CsrSize CsrBtAvrcpCancelConnectReqSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCancelConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCancelConnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpCancelConnectReqSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_ACTIVATE_REQ
CsrSize CsrBtAvrcpActivateReqSizeof(void *msg);
CsrUint8 *CsrBtAvrcpActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpActivateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpActivateReqSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PASS_THROUGH_REQ
CsrSize CsrBtAvrcpCtPassThroughReqSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtPassThroughReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtPassThroughReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpCtPassThroughReqSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PASS_THROUGH_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_NOW_PLAYING_IND
CsrSize CsrBtAvrcpCtNotiNowPlayingIndSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtNotiNowPlayingIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtNotiNowPlayingIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpCtNotiNowPlayingIndSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_NOW_PLAYING_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_REQ
CsrSize CsrBtAvrcpCtSetAddressedPlayerReqSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtSetAddressedPlayerReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtSetAddressedPlayerReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpCtSetAddressedPlayerReqSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_ADD_TO_NOW_PLAYING_RES
CsrSize CsrBtAvrcpTgAddToNowPlayingResSizeof(void *msg);
CsrUint8 *CsrBtAvrcpTgAddToNowPlayingResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpTgAddToNowPlayingResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpTgAddToNowPlayingResSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_ADD_TO_NOW_PLAYING_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_BATTERY_STATUS_IND
CsrSize CsrBtAvrcpCtNotiBatteryStatusIndSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtNotiBatteryStatusIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtNotiBatteryStatusIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpCtNotiBatteryStatusIndSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_BATTERY_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_INFORM_BATTERY_STATUS_CFM
CsrSize CsrBtAvrcpCtInformBatteryStatusCfmSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtInformBatteryStatusCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtInformBatteryStatusCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpCtInformBatteryStatusCfmSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_INFORM_BATTERY_STATUS_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_ID_REQ
CsrSize CsrBtAvrcpCtPasValIdReqSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtPasValIdReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtPasValIdReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpCtPasValIdReqSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_ID_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_CFM
CsrSize CsrBtAvrcpCtPasAttTxtCfmSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtPasAttTxtCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtPasAttTxtCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvrcpCtPasAttTxtCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_CFM
CsrSize CsrBtAvrcpCtSetAddressedPlayerCfmSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtSetAddressedPlayerCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtSetAddressedPlayerCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpCtSetAddressedPlayerCfmSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PAS_CURRENT_IND
CsrSize CsrBtAvrcpTgPasCurrentIndSizeof(void *msg);
CsrUint8 *CsrBtAvrcpTgPasCurrentIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpTgPasCurrentIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvrcpTgPasCurrentIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PAS_CURRENT_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_RES
CsrSize CsrBtAvrcpCtPasValTxtResSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtPasValTxtResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtPasValTxtResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpCtPasValTxtResSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_GET_PLAY_STATUS_RES
CsrSize CsrBtAvrcpTgGetPlayStatusResSizeof(void *msg);
CsrUint8 *CsrBtAvrcpTgGetPlayStatusResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpTgGetPlayStatusResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpTgGetPlayStatusResSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_GET_PLAY_STATUS_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MP_REGISTER_REQ
CsrSize CsrBtAvrcpTgMpRegisterReqSizeof(void *msg);
CsrUint8 *CsrBtAvrcpTgMpRegisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpTgMpRegisterReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvrcpTgMpRegisterReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_MP_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_INFORM_DISP_CHARSET_CFM
CsrSize CsrBtAvrcpCtInformDispCharsetCfmSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtInformDispCharsetCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtInformDispCharsetCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpCtInformDispCharsetCfmSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_INFORM_DISP_CHARSET_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SET_BROWSED_PLAYER_REQ
CsrSize CsrBtAvrcpCtSetBrowsedPlayerReqSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtSetBrowsedPlayerReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtSetBrowsedPlayerReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpCtSetBrowsedPlayerReqSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SET_BROWSED_PLAYER_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_SECURITY_OUT_CFM
CsrSize CsrBtAvrcpSecurityOutCfmSizeof(void *msg);
CsrUint8 *CsrBtAvrcpSecurityOutCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpSecurityOutCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpSecurityOutCfmSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_INFORM_DISP_CHARSET_REQ
CsrSize CsrBtAvrcpCtInformDispCharsetReqSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtInformDispCharsetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtInformDispCharsetReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvrcpCtInformDispCharsetReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_INFORM_DISP_CHARSET_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_GET_PLAY_STATUS_IND
CsrSize CsrBtAvrcpTgGetPlayStatusIndSizeof(void *msg);
CsrUint8 *CsrBtAvrcpTgGetPlayStatusIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpTgGetPlayStatusIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpTgGetPlayStatusIndSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_GET_PLAY_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CONNECT_CFM
CsrSize CsrBtAvrcpConnectCfmSizeof(void *msg);
CsrUint8 *CsrBtAvrcpConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpConnectCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvrcpConnectCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AVRCP_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_REGISTER_REQ
CsrSize CsrBtAvrcpCtNotiRegisterReqSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtNotiRegisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtNotiRegisterReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpCtNotiRegisterReqSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_VOLUME_IND
CsrSize CsrBtAvrcpCtNotiVolumeIndSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtNotiVolumeIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtNotiVolumeIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpCtNotiVolumeIndSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_VOLUME_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_GET_PLAY_STATUS_REQ
CsrSize CsrBtAvrcpCtGetPlayStatusReqSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtGetPlayStatusReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtGetPlayStatusReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpCtGetPlayStatusReqSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_GET_PLAY_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_GET_ATTRIBUTES_RES
CsrSize CsrBtAvrcpTgGetAttributesResSizeof(void *msg);
CsrUint8 *CsrBtAvrcpTgGetAttributesResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpTgGetAttributesResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvrcpTgGetAttributesResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_GET_ATTRIBUTES_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PASS_THROUGH_RES
CsrSize CsrBtAvrcpTgPassThroughResSizeof(void *msg);
CsrUint8 *CsrBtAvrcpTgPassThroughResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpTgPassThroughResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpTgPassThroughResSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PASS_THROUGH_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_NOTI_REQ
CsrSize CsrBtAvrcpTgNotiReqSizeof(void *msg);
CsrUint8 *CsrBtAvrcpTgNotiReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpTgNotiReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpTgNotiReqSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_NOTI_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_UIDS_IND
CsrSize CsrBtAvrcpCtNotiUidsIndSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtNotiUidsIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtNotiUidsIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpCtNotiUidsIndSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_UIDS_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_DEACTIVATE_CFM
CsrSize CsrBtAvrcpDeactivateCfmSizeof(void *msg);
CsrUint8 *CsrBtAvrcpDeactivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpDeactivateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpDeactivateCfmSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_RES
CsrSize CsrBtAvrcpTgPasSetResSizeof(void *msg);
CsrUint8 *CsrBtAvrcpTgPasSetResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpTgPasSetResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpTgPasSetResSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_REGISTER_CFM
CsrSize CsrBtAvrcpCtNotiRegisterCfmSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtNotiRegisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtNotiRegisterCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpCtNotiRegisterCfmSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_REGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SET_VOLUME_RES
CsrSize CsrBtAvrcpTgSetVolumeResSizeof(void *msg);
CsrUint8 *CsrBtAvrcpTgSetVolumeResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpTgSetVolumeResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpTgSetVolumeResSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SET_VOLUME_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_DEACTIVATE_REQ
CsrSize CsrBtAvrcpDeactivateReqSizeof(void *msg);
CsrUint8 *CsrBtAvrcpDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpDeactivateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpDeactivateReqSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_GET_FOLDER_ITEMS_RES
CsrSize CsrBtAvrcpTgGetFolderItemsResSizeof(void *msg);
CsrUint8 *CsrBtAvrcpTgGetFolderItemsResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpTgGetFolderItemsResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvrcpTgGetFolderItemsResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_GET_FOLDER_ITEMS_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SEARCH_CFM
CsrSize CsrBtAvrcpCtSearchCfmSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtSearchCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtSearchCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpCtSearchCfmSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SEARCH_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_AVAILABLE_PLAYERS_IND
CsrSize CsrBtAvrcpCtNotiAvailablePlayersIndSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtNotiAvailablePlayersIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtNotiAvailablePlayersIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpCtNotiAvailablePlayersIndSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_AVAILABLE_PLAYERS_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_PLAYBACK_STATUS_IND
CsrSize CsrBtAvrcpCtNotiPlaybackStatusIndSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtNotiPlaybackStatusIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtNotiPlaybackStatusIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpCtNotiPlaybackStatusIndSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_PLAYBACK_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_IND
CsrSize CsrBtAvrcpCtPasAttTxtIndSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtPasAttTxtIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtPasAttTxtIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvrcpCtPasAttTxtIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_REMOTE_FEATURES_IND
CsrSize CsrBtAvrcpRemoteFeaturesIndSizeof(void *msg);
CsrUint8 *CsrBtAvrcpRemoteFeaturesIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpRemoteFeaturesIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvrcpRemoteFeaturesIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AVRCP_REMOTE_FEATURES_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_CFM
CsrSize CsrBtAvrcpCtGetAttributesCfmSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtGetAttributesCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtGetAttributesCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvrcpCtGetAttributesCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_RES
CsrSize CsrBtAvrcpCtPasAttTxtResSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtPasAttTxtResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtPasAttTxtResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpCtPasAttTxtResSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_TRACK_CHANGED_IND
CsrSize CsrBtAvrcpCtNotiTrackChangedIndSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtNotiTrackChangedIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtNotiTrackChangedIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpCtNotiTrackChangedIndSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_TRACK_CHANGED_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_GET_FOLDER_ITEMS_IND
CsrSize CsrBtAvrcpTgGetFolderItemsIndSizeof(void *msg);
CsrUint8 *CsrBtAvrcpTgGetFolderItemsIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpTgGetFolderItemsIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpTgGetFolderItemsIndSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_GET_FOLDER_ITEMS_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_UNIT_INFO_CMD_REQ
CsrSize CsrBtAvrcpCtUnitInfoCmdReqSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtUnitInfoCmdReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtUnitInfoCmdReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvrcpCtUnitInfoCmdReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_UNIT_INFO_CMD_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_CHANGE_PATH_RES
CsrSize CsrBtAvrcpTgChangePathResSizeof(void *msg);
CsrUint8 *CsrBtAvrcpTgChangePathResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpTgChangePathResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpTgChangePathResSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_CHANGE_PATH_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PAS_CURRENT_RES
CsrSize CsrBtAvrcpTgPasCurrentResSizeof(void *msg);
CsrUint8 *CsrBtAvrcpTgPasCurrentResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpTgPasCurrentResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvrcpTgPasCurrentResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PAS_CURRENT_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_RES
CsrSize CsrBtAvrcpTgSetAddressedPlayerResSizeof(void *msg);
CsrUint8 *CsrBtAvrcpTgSetAddressedPlayerResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpTgSetAddressedPlayerResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpTgSetAddressedPlayerResSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SEARCH_RES
CsrSize CsrBtAvrcpTgSearchResSizeof(void *msg);
CsrUint8 *CsrBtAvrcpTgSearchResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpTgSearchResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpTgSearchResSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SEARCH_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_SECURITY_IN_CFM
CsrSize CsrBtAvrcpSecurityInCfmSizeof(void *msg);
CsrUint8 *CsrBtAvrcpSecurityInCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpSecurityInCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpSecurityInCfmSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SET_BROWSED_PLAYER_CFM
CsrSize CsrBtAvrcpCtSetBrowsedPlayerCfmSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtSetBrowsedPlayerCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtSetBrowsedPlayerCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvrcpCtSetBrowsedPlayerCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SET_BROWSED_PLAYER_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_CHANGE_PATH_CFM
CsrSize CsrBtAvrcpCtChangePathCfmSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtChangePathCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtChangePathCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpCtChangePathCfmSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_CHANGE_PATH_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_INFORM_DISP_CHARSET_IND
CsrSize CsrBtAvrcpTgInformDispCharsetIndSizeof(void *msg);
CsrUint8 *CsrBtAvrcpTgInformDispCharsetIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpTgInformDispCharsetIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvrcpTgInformDispCharsetIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_INFORM_DISP_CHARSET_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_SET_IND
CsrSize CsrBtAvrcpCtPasSetIndSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtPasSetIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtPasSetIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvrcpCtPasSetIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_SET_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_ACTIVATE_CFM
CsrSize CsrBtAvrcpActivateCfmSizeof(void *msg);
CsrUint8 *CsrBtAvrcpActivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpActivateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpActivateCfmSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CONNECT_IND
CsrSize CsrBtAvrcpConnectIndSizeof(void *msg);
CsrUint8 *CsrBtAvrcpConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpConnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpConnectIndSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_UNIT_INFO_CMD_CFM
CsrSize CsrBtAvrcpCtUnitInfoCmdCfmSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtUnitInfoCmdCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtUnitInfoCmdCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvrcpCtUnitInfoCmdCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_UNIT_INFO_CMD_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_CFM
CsrSize CsrBtAvrcpCtPasValTxtCfmSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtPasValTxtCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtPasValTxtCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvrcpCtPasValTxtCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM
CsrSize CsrBtAvrcpCtGetFolderItemsCfmSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtGetFolderItemsCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtGetFolderItemsCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvrcpCtGetFolderItemsCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_ID_CFM
CsrSize CsrBtAvrcpCtPasAttIdCfmSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtPasAttIdCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtPasAttIdCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvrcpCtPasAttIdCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_ID_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_CURRENT_CFM
CsrSize CsrBtAvrcpCtPasCurrentCfmSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtPasCurrentCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtPasCurrentCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvrcpCtPasCurrentCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_CURRENT_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MP_UNREGISTER_CFM
CsrSize CsrBtAvrcpTgMpUnregisterCfmSizeof(void *msg);
CsrUint8 *CsrBtAvrcpTgMpUnregisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpTgMpUnregisterCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpTgMpUnregisterCfmSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_MP_UNREGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_HOUSE_CLEANING
CsrSize CsrBtAvrcpHouseCleaningSizeof(void *msg);
CsrUint8 *CsrBtAvrcpHouseCleaningSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpHouseCleaningDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpHouseCleaningSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_AVRCP_CONNECT_REQ
CsrSize CsrBtAvrcpConnectReqSizeof(void *msg);
CsrUint8 *CsrBtAvrcpConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpConnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpConnectReqSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SEARCH_REQ
CsrSize CsrBtAvrcpCtSearchReqSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtSearchReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtSearchReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvrcpCtSearchReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_SECURITY_IN_REQ
CsrSize CsrBtAvrcpSecurityInReqSizeof(void *msg);
CsrUint8 *CsrBtAvrcpSecurityInReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpSecurityInReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpSecurityInReqSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_CFM
CsrSize CsrBtAvrcpTgPasSetCfmSizeof(void *msg);
CsrUint8 *CsrBtAvrcpTgPasSetCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpTgPasSetCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpTgPasSetCfmSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_DISCONNECT_IND
CsrSize CsrBtAvrcpDisconnectIndSizeof(void *msg);
CsrUint8 *CsrBtAvrcpDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpDisconnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpDisconnectIndSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SUB_UNIT_INFO_CMD_REQ
CsrSize CsrBtAvrcpCtSubUnitInfoCmdReqSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtSubUnitInfoCmdReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtSubUnitInfoCmdReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvrcpCtSubUnitInfoCmdReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SUB_UNIT_INFO_CMD_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_TRACK_END_IND
CsrSize CsrBtAvrcpCtNotiTrackEndIndSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtNotiTrackEndIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtNotiTrackEndIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpCtNotiTrackEndIndSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_TRACK_END_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MP_REGISTER_CFM
CsrSize CsrBtAvrcpTgMpRegisterCfmSizeof(void *msg);
CsrUint8 *CsrBtAvrcpTgMpRegisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpTgMpRegisterCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpTgMpRegisterCfmSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_MP_REGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_NOTI_IND
CsrSize CsrBtAvrcpTgNotiIndSizeof(void *msg);
CsrUint8 *CsrBtAvrcpTgNotiIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpTgNotiIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpTgNotiIndSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_NOTI_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SEARCH_IND
CsrSize CsrBtAvrcpTgSearchIndSizeof(void *msg);
CsrUint8 *CsrBtAvrcpTgSearchIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpTgSearchIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvrcpTgSearchIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SEARCH_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_CURRENT_REQ
CsrSize CsrBtAvrcpCtPasCurrentReqSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtPasCurrentReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtPasCurrentReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvrcpCtPasCurrentReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_CURRENT_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_TRACK_START_IND
CsrSize CsrBtAvrcpCtNotiTrackStartIndSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtNotiTrackStartIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtNotiTrackStartIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpCtNotiTrackStartIndSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_TRACK_START_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_RES
CsrSize CsrBtAvrcpCtGetAttributesResSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtGetAttributesResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtGetAttributesResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpCtGetAttributesResSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_ID_REQ
CsrSize CsrBtAvrcpCtPasAttIdReqSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtPasAttIdReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtPasAttIdReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpCtPasAttIdReqSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_ID_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_IND
CsrSize CsrBtAvrcpTgSetAddressedPlayerIndSizeof(void *msg);
CsrUint8 *CsrBtAvrcpTgSetAddressedPlayerIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpTgSetAddressedPlayerIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpTgSetAddressedPlayerIndSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_ADD_TO_NOW_PLAYING_REQ
CsrSize CsrBtAvrcpCtAddToNowPlayingReqSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtAddToNowPlayingReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtAddToNowPlayingReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpCtAddToNowPlayingReqSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_ADD_TO_NOW_PLAYING_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_ADD_TO_NOW_PLAYING_IND
CsrSize CsrBtAvrcpTgAddToNowPlayingIndSizeof(void *msg);
CsrUint8 *CsrBtAvrcpTgAddToNowPlayingIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpTgAddToNowPlayingIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpTgAddToNowPlayingIndSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_ADD_TO_NOW_PLAYING_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_SYSTEM_STATUS_IND
CsrSize CsrBtAvrcpCtNotiSystemStatusIndSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtNotiSystemStatusIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtNotiSystemStatusIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpCtNotiSystemStatusIndSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_SYSTEM_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_IND
CsrSize CsrBtAvrcpCtGetAttributesIndSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtGetAttributesIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtGetAttributesIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvrcpCtGetAttributesIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_GET_ATTRIBUTES_IND
CsrSize CsrBtAvrcpTgGetAttributesIndSizeof(void *msg);
CsrUint8 *CsrBtAvrcpTgGetAttributesIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpTgGetAttributesIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpTgGetAttributesIndSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_GET_ATTRIBUTES_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PLAY_CFM
CsrSize CsrBtAvrcpCtPlayCfmSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtPlayCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtPlayCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpCtPlayCfmSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PLAY_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SET_VOLUME_REQ
CsrSize CsrBtAvrcpCtSetVolumeReqSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtSetVolumeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtSetVolumeReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpCtSetVolumeReqSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SET_VOLUME_REQ */




#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SS_SET_VOLUME_REQ
CsrSize CsrBtAvrcpCtSetSSVolumeReqSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtSetSSVolumeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtSetSSVolumeReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpCtSetSSVolumeReqSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SS_SET_VOLUME_REQ */


#ifndef EXCLUDE_CSR_BT_AVRCP_TG_NOTI_RES
CsrSize CsrBtAvrcpTgNotiResSizeof(void *msg);
CsrUint8 *CsrBtAvrcpTgNotiResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpTgNotiResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpTgNotiResSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_NOTI_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PLAY_RES
CsrSize CsrBtAvrcpTgPlayResSizeof(void *msg);
CsrUint8 *CsrBtAvrcpTgPlayResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpTgPlayResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpTgPlayResSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PLAY_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_SET_CFM
CsrSize CsrBtAvrcpCtPasSetCfmSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtPasSetCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtPasSetCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpCtPasSetCfmSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_SET_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_INFORM_BATTERY_STATUS_IND
CsrSize CsrBtAvrcpTgInformBatteryStatusIndSizeof(void *msg);
CsrUint8 *CsrBtAvrcpTgInformBatteryStatusIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpTgInformBatteryStatusIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvrcpTgInformBatteryStatusIndSerFree CsrBtAvrcpPfree
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_INFORM_BATTERY_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SET_BROWSED_PLAYER_RES
CsrSize CsrBtAvrcpTgSetBrowsedPlayerResSizeof(void *msg);
CsrUint8 *CsrBtAvrcpTgSetBrowsedPlayerResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpTgSetBrowsedPlayerResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvrcpTgSetBrowsedPlayerResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SET_BROWSED_PLAYER_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SUB_UNIT_INFO_CMD_CFM
CsrSize CsrBtAvrcpCtSubUnitInfoCmdCfmSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtSubUnitInfoCmdCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtSubUnitInfoCmdCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvrcpCtSubUnitInfoCmdCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SUB_UNIT_INFO_CMD_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_REQ
CsrSize CsrBtAvrcpCtPasAttTxtReqSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtPasAttTxtReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtPasAttTxtReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvrcpCtPasAttTxtReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_SET_REQ
CsrSize CsrBtAvrcpCtPasSetReqSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtPasSetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtPasSetReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvrcpCtPasSetReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_SET_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_ID_CFM
CsrSize CsrBtAvrcpCtPasValIdCfmSizeof(void *msg);
CsrUint8 *CsrBtAvrcpCtPasValIdCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvrcpCtPasValIdCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvrcpCtPasValIdCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_ID_CFM */

#ifdef __cplusplus
}
#endif

#endif
