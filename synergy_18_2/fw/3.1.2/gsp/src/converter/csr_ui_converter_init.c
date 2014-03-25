/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_msgconv.h"
#include "csr_pmem.h"
#include "csr_util.h"
#include "csr_hand_coded.h"
#include "csr_ui_converter_init.h"
#ifndef EXCLUDE_CSR_UI_MODULE
#include "csr_ui_serialize.h"
#include "csr_ui_private_prim.h"

static CsrMsgConvMsgEntry csr_ui_conv_lut[] = {
#ifndef EXCLUDE_CSR_UI_DIALOG_GET_CFM
    { CSR_UI_DIALOG_GET_CFM, CsrUiDialogGetCfmSizeof, CsrUiDialogGetCfmSer, CsrUiDialogGetCfmDes, CsrUiDialogGetCfmSerFree },
#endif /* EXCLUDE_CSR_UI_DIALOG_GET_CFM */
#ifndef EXCLUDE_CSR_UI_DIALOG_GET_REQ
    { CSR_UI_DIALOG_GET_REQ, CsrUiDialogGetReqSizeof, CsrUiDialogGetReqSer, CsrUiDialogGetReqDes, CsrUiDialogGetReqSerFree },
#endif /* EXCLUDE_CSR_UI_DIALOG_GET_REQ */
#ifndef EXCLUDE_CSR_UI_DIALOG_SET_REQ
    { CSR_UI_DIALOG_SET_REQ, CsrUiDialogSetReqSizeof, CsrUiDialogSetReqSer, CsrUiDialogSetReqDes, CsrUiDialogSetReqSerFree },
#endif /* EXCLUDE_CSR_UI_DIALOG_SET_REQ */
#ifndef EXCLUDE_CSR_UI_DISPLAY_GETHANDLE_CFM
    { CSR_UI_DISPLAY_GETHANDLE_CFM, CsrUiDisplayGethandleCfmSizeof, CsrUiDisplayGethandleCfmSer, CsrUiDisplayGethandleCfmDes, CsrUiDisplayGethandleCfmSerFree },
#endif /* EXCLUDE_CSR_UI_DISPLAY_GETHANDLE_CFM */
#ifndef EXCLUDE_CSR_UI_DISPLAY_GETHANDLE_REQ
    { CSR_UI_DISPLAY_GETHANDLE_REQ, CsrUiDisplayGethandleReqSizeof, CsrUiDisplayGethandleReqSer, CsrUiDisplayGethandleReqDes, CsrUiDisplayGethandleReqSerFree },
#endif /* EXCLUDE_CSR_UI_DISPLAY_GETHANDLE_REQ */
#ifndef EXCLUDE_CSR_UI_DISPLAY_SETINPUTMODE_REQ
    { CSR_UI_DISPLAY_SETINPUTMODE_REQ, CsrUiDisplaySetinputmodeReqSizeof, CsrUiDisplaySetinputmodeReqSer, CsrUiDisplaySetinputmodeReqDes, CsrUiDisplaySetinputmodeReqSerFree },
#endif /* EXCLUDE_CSR_UI_DISPLAY_SETINPUTMODE_REQ */
#ifndef EXCLUDE_CSR_UI_EVENT_GET_CFM
    { CSR_UI_EVENT_GET_CFM, CsrUiEventGetCfmSizeof, CsrUiEventGetCfmSer, CsrUiEventGetCfmDes, CsrUiEventGetCfmSerFree },
#endif /* EXCLUDE_CSR_UI_EVENT_GET_CFM */
#ifndef EXCLUDE_CSR_UI_EVENT_GET_REQ
    { CSR_UI_EVENT_GET_REQ, CsrUiEventGetReqSizeof, CsrUiEventGetReqSer, CsrUiEventGetReqDes, CsrUiEventGetReqSerFree },
#endif /* EXCLUDE_CSR_UI_EVENT_GET_REQ */
#ifndef EXCLUDE_CSR_UI_EVENT_IND
    { CSR_UI_EVENT_IND, CsrUiEventIndSizeof, CsrUiEventIndSer, CsrUiEventIndDes, CsrUiEventIndSerFree },
#endif /* EXCLUDE_CSR_UI_EVENT_IND */
#ifndef EXCLUDE_CSR_UI_EVENT_SET_REQ
    { CSR_UI_EVENT_SET_REQ, CsrUiEventSetReqSizeof, CsrUiEventSetReqSer, CsrUiEventSetReqDes, CsrUiEventSetReqSerFree },
#endif /* EXCLUDE_CSR_UI_EVENT_SET_REQ */
#ifndef EXCLUDE_CSR_UI_IDLESCREEN_GET_CFM
    { CSR_UI_IDLESCREEN_GET_CFM, CsrUiIdlescreenGetCfmSizeof, CsrUiIdlescreenGetCfmSer, CsrUiIdlescreenGetCfmDes, CsrUiIdlescreenGetCfmSerFree },
#endif /* EXCLUDE_CSR_UI_IDLESCREEN_GET_CFM */
#ifndef EXCLUDE_CSR_UI_IDLESCREEN_GET_REQ
    { CSR_UI_IDLESCREEN_GET_REQ, CsrUiIdlescreenGetReqSizeof, CsrUiIdlescreenGetReqSer, CsrUiIdlescreenGetReqDes, CsrUiIdlescreenGetReqSerFree },
#endif /* EXCLUDE_CSR_UI_IDLESCREEN_GET_REQ */
#ifndef EXCLUDE_CSR_UI_IDLESCREEN_SET_REQ
    { CSR_UI_IDLESCREEN_SET_REQ, CsrUiIdlescreenSetReqSizeof, CsrUiIdlescreenSetReqSer, CsrUiIdlescreenSetReqDes, CsrUiIdlescreenSetReqSerFree },
#endif /* EXCLUDE_CSR_UI_IDLESCREEN_SET_REQ */
#ifndef EXCLUDE_CSR_UI_INPUTDIALOG_GET_CFM
    { CSR_UI_INPUTDIALOG_GET_CFM, CsrUiInputdialogGetCfmSizeof, CsrUiInputdialogGetCfmSer, CsrUiInputdialogGetCfmDes, CsrUiInputdialogGetCfmSerFree },
#endif /* EXCLUDE_CSR_UI_INPUTDIALOG_GET_CFM */
#ifndef EXCLUDE_CSR_UI_INPUTDIALOG_GET_REQ
    { CSR_UI_INPUTDIALOG_GET_REQ, CsrUiInputdialogGetReqSizeof, CsrUiInputdialogGetReqSer, CsrUiInputdialogGetReqDes, CsrUiInputdialogGetReqSerFree },
#endif /* EXCLUDE_CSR_UI_INPUTDIALOG_GET_REQ */
#ifndef EXCLUDE_CSR_UI_INPUTDIALOG_SET_REQ
    { CSR_UI_INPUTDIALOG_SET_REQ, CsrUiInputdialogSetReqSizeof, CsrUiInputdialogSetReqSer, CsrUiInputdialogSetReqDes, CsrUiInputdialogSetReqSerFree },
#endif /* EXCLUDE_CSR_UI_INPUTDIALOG_SET_REQ */
#ifndef EXCLUDE_CSR_UI_INTERNAL_KEYDOWN_IND
    { CSR_UI_INTERNAL_KEYDOWN_IND, CsrUiInternalKeydownIndSizeof, CsrUiInternalKeydownIndSer, CsrUiInternalKeydownIndDes, CsrUiInternalKeydownIndSerFree },
#endif /* EXCLUDE_CSR_UI_INTERNAL_KEYDOWN_IND */
#ifndef EXCLUDE_CSR_UI_KEYDOWN_IND
    { CSR_UI_KEYDOWN_IND, CsrUiKeydownIndSizeof, CsrUiKeydownIndSer, CsrUiKeydownIndDes, CsrUiKeydownIndSerFree },
#endif /* EXCLUDE_CSR_UI_KEYDOWN_IND */
#ifndef EXCLUDE_CSR_UI_MENU_ADDITEM_REQ
    { CSR_UI_MENU_ADDITEM_REQ, CsrUiMenuAdditemReqSizeof, CsrUiMenuAdditemReqSer, CsrUiMenuAdditemReqDes, CsrUiMenuAdditemReqSerFree },
#endif /* EXCLUDE_CSR_UI_MENU_ADDITEM_REQ */
#ifndef EXCLUDE_CSR_UI_MENU_GET_CFM
    { CSR_UI_MENU_GET_CFM, CsrUiMenuGetCfmSizeof, CsrUiMenuGetCfmSer, CsrUiMenuGetCfmDes, CsrUiMenuGetCfmSerFree },
#endif /* EXCLUDE_CSR_UI_MENU_GET_CFM */
#ifndef EXCLUDE_CSR_UI_MENU_GET_REQ
    { CSR_UI_MENU_GET_REQ, CsrUiMenuGetReqSizeof, CsrUiMenuGetReqSer, CsrUiMenuGetReqDes, CsrUiMenuGetReqSerFree },
#endif /* EXCLUDE_CSR_UI_MENU_GET_REQ */
#ifndef EXCLUDE_CSR_UI_MENU_GETCURSOR_CFM
    { CSR_UI_MENU_GETCURSOR_CFM, CsrUiMenuGetcursorCfmSizeof, CsrUiMenuGetcursorCfmSer, CsrUiMenuGetcursorCfmDes, CsrUiMenuGetcursorCfmSerFree },
#endif /* EXCLUDE_CSR_UI_MENU_GETCURSOR_CFM */
#ifndef EXCLUDE_CSR_UI_MENU_GETCURSOR_REQ
    { CSR_UI_MENU_GETCURSOR_REQ, CsrUiMenuGetcursorReqSizeof, CsrUiMenuGetcursorReqSer, CsrUiMenuGetcursorReqDes, CsrUiMenuGetcursorReqSerFree },
#endif /* EXCLUDE_CSR_UI_MENU_GETCURSOR_REQ */
#ifndef EXCLUDE_CSR_UI_MENU_GETITEM_CFM
    { CSR_UI_MENU_GETITEM_CFM, CsrUiMenuGetitemCfmSizeof, CsrUiMenuGetitemCfmSer, CsrUiMenuGetitemCfmDes, CsrUiMenuGetitemCfmSerFree },
#endif /* EXCLUDE_CSR_UI_MENU_GETITEM_CFM */
#ifndef EXCLUDE_CSR_UI_MENU_GETITEM_REQ
    { CSR_UI_MENU_GETITEM_REQ, CsrUiMenuGetitemReqSizeof, CsrUiMenuGetitemReqSer, CsrUiMenuGetitemReqDes, CsrUiMenuGetitemReqSerFree },
#endif /* EXCLUDE_CSR_UI_MENU_GETITEM_REQ */
#ifndef EXCLUDE_CSR_UI_MENU_REMOVEALLITEMS_REQ
    { CSR_UI_MENU_REMOVEALLITEMS_REQ, CsrUiMenuRemoveallitemsReqSizeof, CsrUiMenuRemoveallitemsReqSer, CsrUiMenuRemoveallitemsReqDes, CsrUiMenuRemoveallitemsReqSerFree },
#endif /* EXCLUDE_CSR_UI_MENU_REMOVEALLITEMS_REQ */
#ifndef EXCLUDE_CSR_UI_MENU_REMOVEITEM_REQ
    { CSR_UI_MENU_REMOVEITEM_REQ, CsrUiMenuRemoveitemReqSizeof, CsrUiMenuRemoveitemReqSer, CsrUiMenuRemoveitemReqDes, CsrUiMenuRemoveitemReqSerFree },
#endif /* EXCLUDE_CSR_UI_MENU_REMOVEITEM_REQ */
#ifndef EXCLUDE_CSR_UI_MENU_SET_REQ
    { CSR_UI_MENU_SET_REQ, CsrUiMenuSetReqSizeof, CsrUiMenuSetReqSer, CsrUiMenuSetReqDes, CsrUiMenuSetReqSerFree },
#endif /* EXCLUDE_CSR_UI_MENU_SET_REQ */
#ifndef EXCLUDE_CSR_UI_MENU_SETCURSOR_REQ
    { CSR_UI_MENU_SETCURSOR_REQ, CsrUiMenuSetcursorReqSizeof, CsrUiMenuSetcursorReqSer, CsrUiMenuSetcursorReqDes, CsrUiMenuSetcursorReqSerFree },
#endif /* EXCLUDE_CSR_UI_MENU_SETCURSOR_REQ */
#ifndef EXCLUDE_CSR_UI_MENU_SETITEM_REQ
    { CSR_UI_MENU_SETITEM_REQ, CsrUiMenuSetitemReqSizeof, CsrUiMenuSetitemReqSer, CsrUiMenuSetitemReqDes, CsrUiMenuSetitemReqSerFree },
#endif /* EXCLUDE_CSR_UI_MENU_SETITEM_REQ */
#ifndef EXCLUDE_CSR_UI_STATUS_BATTERY_SET_REQ
    { CSR_UI_STATUS_BATTERY_SET_REQ, CsrUiStatusBatterySetReqSizeof, CsrUiStatusBatterySetReqSer, CsrUiStatusBatterySetReqDes, CsrUiStatusBatterySetReqSerFree },
#endif /* EXCLUDE_CSR_UI_STATUS_BATTERY_SET_REQ */
#ifndef EXCLUDE_CSR_UI_STATUS_BLUETOOTH_SET_REQ
    { CSR_UI_STATUS_BLUETOOTH_SET_REQ, CsrUiStatusBluetoothSetReqSizeof, CsrUiStatusBluetoothSetReqSer, CsrUiStatusBluetoothSetReqDes, CsrUiStatusBluetoothSetReqSerFree },
#endif /* EXCLUDE_CSR_UI_STATUS_BLUETOOTH_SET_REQ */
#ifndef EXCLUDE_CSR_UI_STATUS_HEADSET_SET_REQ
    { CSR_UI_STATUS_HEADSET_SET_REQ, CsrUiStatusHeadsetSetReqSizeof, CsrUiStatusHeadsetSetReqSer, CsrUiStatusHeadsetSetReqDes, CsrUiStatusHeadsetSetReqSerFree },
#endif /* EXCLUDE_CSR_UI_STATUS_HEADSET_SET_REQ */
#ifndef EXCLUDE_CSR_UI_STATUS_RADIOMETER_SET_REQ
    { CSR_UI_STATUS_RADIOMETER_SET_REQ, CsrUiStatusRadiometerSetReqSizeof, CsrUiStatusRadiometerSetReqSer, CsrUiStatusRadiometerSetReqDes, CsrUiStatusRadiometerSetReqSerFree },
#endif /* EXCLUDE_CSR_UI_STATUS_RADIOMETER_SET_REQ */
#ifndef EXCLUDE_CSR_UI_STATUS_WIFI_SET_REQ
    { CSR_UI_STATUS_WIFI_SET_REQ, CsrUiStatusWifiSetReqSizeof, CsrUiStatusWifiSetReqSer, CsrUiStatusWifiSetReqDes, CsrUiStatusWifiSetReqSerFree },
#endif /* EXCLUDE_CSR_UI_STATUS_WIFI_SET_REQ */
#ifndef EXCLUDE_CSR_UI_UIE_CREATE_CFM
    { CSR_UI_UIE_CREATE_CFM, CsrUiUieCreateCfmSizeof, CsrUiUieCreateCfmSer, CsrUiUieCreateCfmDes, CsrUiUieCreateCfmSerFree },
#endif /* EXCLUDE_CSR_UI_UIE_CREATE_CFM */
#ifndef EXCLUDE_CSR_UI_UIE_CREATE_REQ
    { CSR_UI_UIE_CREATE_REQ, CsrUiUieCreateReqSizeof, CsrUiUieCreateReqSer, CsrUiUieCreateReqDes, CsrUiUieCreateReqSerFree },
#endif /* EXCLUDE_CSR_UI_UIE_CREATE_REQ */
#ifndef EXCLUDE_CSR_UI_UIE_DESTROY_REQ
    { CSR_UI_UIE_DESTROY_REQ, CsrUiUieDestroyReqSizeof, CsrUiUieDestroyReqSer, CsrUiUieDestroyReqDes, CsrUiUieDestroyReqSerFree },
#endif /* EXCLUDE_CSR_UI_UIE_DESTROY_REQ */
#ifndef EXCLUDE_CSR_UI_UIE_HIDE_REQ
    { CSR_UI_UIE_HIDE_REQ, CsrUiUieHideReqSizeof, CsrUiUieHideReqSer, CsrUiUieHideReqDes, CsrUiUieHideReqSerFree },
#endif /* EXCLUDE_CSR_UI_UIE_HIDE_REQ */
#ifndef EXCLUDE_CSR_UI_UIE_SHOW_REQ
    { CSR_UI_UIE_SHOW_REQ, CsrUiUieShowReqSizeof, CsrUiUieShowReqSer, CsrUiUieShowReqDes, CsrUiUieShowReqSerFree },
#endif /* EXCLUDE_CSR_UI_UIE_SHOW_REQ */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrUiConverterInit(void)
{
    CsrMsgConvInsert(CSR_UI_PRIM, csr_ui_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_ui_conv_info = {
    CSR_UI_PRIM,
    "CSR_UI_PRIM",
    csr_ui_conv_lut
};

CsrLogPrimitiveInformation* CsrUiTechInfoGet(void)
{
    return &csr_ui_conv_info;
}
#endif
#endif

