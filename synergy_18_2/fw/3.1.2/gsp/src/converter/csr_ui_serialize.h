#ifndef CSR_UI_SERIALIZE_H__
#define CSR_UI_SERIALIZE_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrUiPfree(void *ptr);

#ifndef EXCLUDE_CSR_UI_STATUS_HEADSET_SET_REQ
CsrSize CsrUiStatusHeadsetSetReqSizeof(void *msg);
CsrUint8 *CsrUiStatusHeadsetSetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrUiStatusHeadsetSetReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrUiStatusHeadsetSetReqSerFree CsrUiPfree
#endif /* EXCLUDE_CSR_UI_STATUS_HEADSET_SET_REQ */

#ifndef EXCLUDE_CSR_UI_UIE_CREATE_CFM
CsrSize CsrUiUieCreateCfmSizeof(void *msg);
CsrUint8 *CsrUiUieCreateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrUiUieCreateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrUiUieCreateCfmSerFree CsrUiPfree
#endif /* EXCLUDE_CSR_UI_UIE_CREATE_CFM */

#ifndef EXCLUDE_CSR_UI_MENU_GETCURSOR_CFM
CsrSize CsrUiMenuGetcursorCfmSizeof(void *msg);
CsrUint8 *CsrUiMenuGetcursorCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrUiMenuGetcursorCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrUiMenuGetcursorCfmSerFree CsrUiPfree
#endif /* EXCLUDE_CSR_UI_MENU_GETCURSOR_CFM */

#ifndef EXCLUDE_CSR_UI_MENU_GETITEM_REQ
CsrSize CsrUiMenuGetitemReqSizeof(void *msg);
CsrUint8 *CsrUiMenuGetitemReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrUiMenuGetitemReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrUiMenuGetitemReqSerFree CsrUiPfree
#endif /* EXCLUDE_CSR_UI_MENU_GETITEM_REQ */

#ifndef EXCLUDE_CSR_UI_INPUTDIALOG_GET_REQ
CsrSize CsrUiInputdialogGetReqSizeof(void *msg);
CsrUint8 *CsrUiInputdialogGetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrUiInputdialogGetReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrUiInputdialogGetReqSerFree CsrUiPfree
#endif /* EXCLUDE_CSR_UI_INPUTDIALOG_GET_REQ */

#ifndef EXCLUDE_CSR_UI_IDLESCREEN_SET_REQ
CsrSize CsrUiIdlescreenSetReqSizeof(void *msg);
CsrUint8 *CsrUiIdlescreenSetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrUiIdlescreenSetReqDes(CsrUint8 *buffer, CsrSize length);
void CsrUiIdlescreenSetReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_UI_IDLESCREEN_SET_REQ */

#ifndef EXCLUDE_CSR_UI_MENU_ADDITEM_REQ
CsrSize CsrUiMenuAdditemReqSizeof(void *msg);
CsrUint8 *CsrUiMenuAdditemReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrUiMenuAdditemReqDes(CsrUint8 *buffer, CsrSize length);
void CsrUiMenuAdditemReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_UI_MENU_ADDITEM_REQ */

#ifndef EXCLUDE_CSR_UI_STATUS_BATTERY_SET_REQ
CsrSize CsrUiStatusBatterySetReqSizeof(void *msg);
CsrUint8 *CsrUiStatusBatterySetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrUiStatusBatterySetReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrUiStatusBatterySetReqSerFree CsrUiPfree
#endif /* EXCLUDE_CSR_UI_STATUS_BATTERY_SET_REQ */

#ifndef EXCLUDE_CSR_UI_STATUS_RADIOMETER_SET_REQ
CsrSize CsrUiStatusRadiometerSetReqSizeof(void *msg);
CsrUint8 *CsrUiStatusRadiometerSetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrUiStatusRadiometerSetReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrUiStatusRadiometerSetReqSerFree CsrUiPfree
#endif /* EXCLUDE_CSR_UI_STATUS_RADIOMETER_SET_REQ */

#ifndef EXCLUDE_CSR_UI_INTERNAL_KEYDOWN_IND
CsrSize CsrUiInternalKeydownIndSizeof(void *msg);
CsrUint8 *CsrUiInternalKeydownIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrUiInternalKeydownIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrUiInternalKeydownIndSerFree CsrUiPfree
#endif /* EXCLUDE_CSR_UI_INTERNAL_KEYDOWN_IND */

#ifndef EXCLUDE_CSR_UI_EVENT_IND
CsrSize CsrUiEventIndSizeof(void *msg);
CsrUint8 *CsrUiEventIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrUiEventIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrUiEventIndSerFree CsrUiPfree
#endif /* EXCLUDE_CSR_UI_EVENT_IND */

#ifndef EXCLUDE_CSR_UI_MENU_SETCURSOR_REQ
CsrSize CsrUiMenuSetcursorReqSizeof(void *msg);
CsrUint8 *CsrUiMenuSetcursorReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrUiMenuSetcursorReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrUiMenuSetcursorReqSerFree CsrUiPfree
#endif /* EXCLUDE_CSR_UI_MENU_SETCURSOR_REQ */

#ifndef EXCLUDE_CSR_UI_MENU_GETCURSOR_REQ
CsrSize CsrUiMenuGetcursorReqSizeof(void *msg);
CsrUint8 *CsrUiMenuGetcursorReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrUiMenuGetcursorReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrUiMenuGetcursorReqSerFree CsrUiPfree
#endif /* EXCLUDE_CSR_UI_MENU_GETCURSOR_REQ */

#ifndef EXCLUDE_CSR_UI_DISPLAY_GETHANDLE_REQ
CsrSize CsrUiDisplayGethandleReqSizeof(void *msg);
CsrUint8 *CsrUiDisplayGethandleReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrUiDisplayGethandleReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrUiDisplayGethandleReqSerFree CsrUiPfree
#endif /* EXCLUDE_CSR_UI_DISPLAY_GETHANDLE_REQ */

#ifndef EXCLUDE_CSR_UI_MENU_GET_REQ
CsrSize CsrUiMenuGetReqSizeof(void *msg);
CsrUint8 *CsrUiMenuGetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrUiMenuGetReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrUiMenuGetReqSerFree CsrUiPfree
#endif /* EXCLUDE_CSR_UI_MENU_GET_REQ */

#ifndef EXCLUDE_CSR_UI_STATUS_BLUETOOTH_SET_REQ
CsrSize CsrUiStatusBluetoothSetReqSizeof(void *msg);
CsrUint8 *CsrUiStatusBluetoothSetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrUiStatusBluetoothSetReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrUiStatusBluetoothSetReqSerFree CsrUiPfree
#endif /* EXCLUDE_CSR_UI_STATUS_BLUETOOTH_SET_REQ */

#ifndef EXCLUDE_CSR_UI_MENU_REMOVEALLITEMS_REQ
CsrSize CsrUiMenuRemoveallitemsReqSizeof(void *msg);
CsrUint8 *CsrUiMenuRemoveallitemsReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrUiMenuRemoveallitemsReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrUiMenuRemoveallitemsReqSerFree CsrUiPfree
#endif /* EXCLUDE_CSR_UI_MENU_REMOVEALLITEMS_REQ */

#ifndef EXCLUDE_CSR_UI_KEYDOWN_IND
CsrSize CsrUiKeydownIndSizeof(void *msg);
CsrUint8 *CsrUiKeydownIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrUiKeydownIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrUiKeydownIndSerFree CsrUiPfree
#endif /* EXCLUDE_CSR_UI_KEYDOWN_IND */

#ifndef EXCLUDE_CSR_UI_MENU_REMOVEITEM_REQ
CsrSize CsrUiMenuRemoveitemReqSizeof(void *msg);
CsrUint8 *CsrUiMenuRemoveitemReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrUiMenuRemoveitemReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrUiMenuRemoveitemReqSerFree CsrUiPfree
#endif /* EXCLUDE_CSR_UI_MENU_REMOVEITEM_REQ */

#ifndef EXCLUDE_CSR_UI_UIE_SHOW_REQ
CsrSize CsrUiUieShowReqSizeof(void *msg);
CsrUint8 *CsrUiUieShowReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrUiUieShowReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrUiUieShowReqSerFree CsrUiPfree
#endif /* EXCLUDE_CSR_UI_UIE_SHOW_REQ */

#ifndef EXCLUDE_CSR_UI_INPUTDIALOG_SET_REQ
CsrSize CsrUiInputdialogSetReqSizeof(void *msg);
CsrUint8 *CsrUiInputdialogSetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrUiInputdialogSetReqDes(CsrUint8 *buffer, CsrSize length);
void CsrUiInputdialogSetReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_UI_INPUTDIALOG_SET_REQ */

#ifndef EXCLUDE_CSR_UI_MENU_GET_CFM
CsrSize CsrUiMenuGetCfmSizeof(void *msg);
CsrUint8 *CsrUiMenuGetCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrUiMenuGetCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrUiMenuGetCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_UI_MENU_GET_CFM */

#ifndef EXCLUDE_CSR_UI_DIALOG_GET_REQ
CsrSize CsrUiDialogGetReqSizeof(void *msg);
CsrUint8 *CsrUiDialogGetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrUiDialogGetReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrUiDialogGetReqSerFree CsrUiPfree
#endif /* EXCLUDE_CSR_UI_DIALOG_GET_REQ */

#ifndef EXCLUDE_CSR_UI_DISPLAY_SETINPUTMODE_REQ
CsrSize CsrUiDisplaySetinputmodeReqSizeof(void *msg);
CsrUint8 *CsrUiDisplaySetinputmodeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrUiDisplaySetinputmodeReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrUiDisplaySetinputmodeReqSerFree CsrUiPfree
#endif /* EXCLUDE_CSR_UI_DISPLAY_SETINPUTMODE_REQ */

#ifndef EXCLUDE_CSR_UI_UIE_DESTROY_REQ
CsrSize CsrUiUieDestroyReqSizeof(void *msg);
CsrUint8 *CsrUiUieDestroyReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrUiUieDestroyReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrUiUieDestroyReqSerFree CsrUiPfree
#endif /* EXCLUDE_CSR_UI_UIE_DESTROY_REQ */

#ifndef EXCLUDE_CSR_UI_DISPLAY_GETHANDLE_CFM
CsrSize CsrUiDisplayGethandleCfmSizeof(void *msg);
CsrUint8 *CsrUiDisplayGethandleCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrUiDisplayGethandleCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrUiDisplayGethandleCfmSerFree CsrUiPfree
#endif /* EXCLUDE_CSR_UI_DISPLAY_GETHANDLE_CFM */

#ifndef EXCLUDE_CSR_UI_MENU_SETITEM_REQ
CsrSize CsrUiMenuSetitemReqSizeof(void *msg);
CsrUint8 *CsrUiMenuSetitemReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrUiMenuSetitemReqDes(CsrUint8 *buffer, CsrSize length);
void CsrUiMenuSetitemReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_UI_MENU_SETITEM_REQ */

#ifndef EXCLUDE_CSR_UI_UIE_CREATE_REQ
CsrSize CsrUiUieCreateReqSizeof(void *msg);
CsrUint8 *CsrUiUieCreateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrUiUieCreateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrUiUieCreateReqSerFree CsrUiPfree
#endif /* EXCLUDE_CSR_UI_UIE_CREATE_REQ */

#ifndef EXCLUDE_CSR_UI_IDLESCREEN_GET_CFM
CsrSize CsrUiIdlescreenGetCfmSizeof(void *msg);
CsrUint8 *CsrUiIdlescreenGetCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrUiIdlescreenGetCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrUiIdlescreenGetCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_UI_IDLESCREEN_GET_CFM */

#ifndef EXCLUDE_CSR_UI_EVENT_GET_REQ
CsrSize CsrUiEventGetReqSizeof(void *msg);
CsrUint8 *CsrUiEventGetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrUiEventGetReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrUiEventGetReqSerFree CsrUiPfree
#endif /* EXCLUDE_CSR_UI_EVENT_GET_REQ */

#ifndef EXCLUDE_CSR_UI_MENU_GETITEM_CFM
CsrSize CsrUiMenuGetitemCfmSizeof(void *msg);
CsrUint8 *CsrUiMenuGetitemCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrUiMenuGetitemCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrUiMenuGetitemCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_UI_MENU_GETITEM_CFM */

#ifndef EXCLUDE_CSR_UI_IDLESCREEN_GET_REQ
CsrSize CsrUiIdlescreenGetReqSizeof(void *msg);
CsrUint8 *CsrUiIdlescreenGetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrUiIdlescreenGetReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrUiIdlescreenGetReqSerFree CsrUiPfree
#endif /* EXCLUDE_CSR_UI_IDLESCREEN_GET_REQ */

#ifndef EXCLUDE_CSR_UI_DIALOG_SET_REQ
CsrSize CsrUiDialogSetReqSizeof(void *msg);
CsrUint8 *CsrUiDialogSetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrUiDialogSetReqDes(CsrUint8 *buffer, CsrSize length);
void CsrUiDialogSetReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_UI_DIALOG_SET_REQ */

#ifndef EXCLUDE_CSR_UI_INPUTDIALOG_GET_CFM
CsrSize CsrUiInputdialogGetCfmSizeof(void *msg);
CsrUint8 *CsrUiInputdialogGetCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrUiInputdialogGetCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrUiInputdialogGetCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_UI_INPUTDIALOG_GET_CFM */

#ifndef EXCLUDE_CSR_UI_EVENT_SET_REQ
CsrSize CsrUiEventSetReqSizeof(void *msg);
CsrUint8 *CsrUiEventSetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrUiEventSetReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrUiEventSetReqSerFree CsrUiPfree
#endif /* EXCLUDE_CSR_UI_EVENT_SET_REQ */

#ifndef EXCLUDE_CSR_UI_EVENT_GET_CFM
CsrSize CsrUiEventGetCfmSizeof(void *msg);
CsrUint8 *CsrUiEventGetCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrUiEventGetCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrUiEventGetCfmSerFree CsrUiPfree
#endif /* EXCLUDE_CSR_UI_EVENT_GET_CFM */

#ifndef EXCLUDE_CSR_UI_STATUS_WIFI_SET_REQ
CsrSize CsrUiStatusWifiSetReqSizeof(void *msg);
CsrUint8 *CsrUiStatusWifiSetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrUiStatusWifiSetReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrUiStatusWifiSetReqSerFree CsrUiPfree
#endif /* EXCLUDE_CSR_UI_STATUS_WIFI_SET_REQ */

#ifndef EXCLUDE_CSR_UI_UIE_HIDE_REQ
CsrSize CsrUiUieHideReqSizeof(void *msg);
CsrUint8 *CsrUiUieHideReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrUiUieHideReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrUiUieHideReqSerFree CsrUiPfree
#endif /* EXCLUDE_CSR_UI_UIE_HIDE_REQ */

#ifndef EXCLUDE_CSR_UI_MENU_SET_REQ
CsrSize CsrUiMenuSetReqSizeof(void *msg);
CsrUint8 *CsrUiMenuSetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrUiMenuSetReqDes(CsrUint8 *buffer, CsrSize length);
void CsrUiMenuSetReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_UI_MENU_SET_REQ */

#ifndef EXCLUDE_CSR_UI_DIALOG_GET_CFM
CsrSize CsrUiDialogGetCfmSizeof(void *msg);
CsrUint8 *CsrUiDialogGetCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrUiDialogGetCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrUiDialogGetCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_UI_DIALOG_GET_CFM */

#ifdef __cplusplus
}
#endif

#endif
