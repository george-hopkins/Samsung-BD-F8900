#ifndef CSR_UI_SEF_H__
#define CSR_UI_SEF_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_ui_prim.h"
#include "csr_ui_ctrl.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Prototypes for state event functions */
CsrBool CsrUiUieCreateReqHandler(CsrUiInstanceData *instanceData);               /* CSR_UI_UIE_CREATE_REQ */
CsrBool CsrUiMenuSetReqHandler(CsrUiInstanceData *instanceData);                 /* CSR_UI_MENU_SET_REQ */
CsrBool CsrUiMenuGetReqHandler(CsrUiInstanceData *instanceData);                 /* CSR_UI_MENU_GET_REQ */
CsrBool CsrUiMenuAdditemReqHandler(CsrUiInstanceData *instanceData);             /* CSR_UI_MENU_ADDITEM_REQ */
CsrBool CsrUiMenuRemoveitemReqHandler(CsrUiInstanceData *instanceData);          /* CSR_UI_MENU_REMOVEITEM_REQ */
CsrBool CsrUiMenuRemoveallitemsReqHandler(CsrUiInstanceData *instanceData);      /* CSR_UI_MENU_REMOVEALLITEMS_REQ */
CsrBool CsrUiMenuSetitemReqHandler(CsrUiInstanceData *instanceData);             /* CSR_UI_MENU_SETITEM_REQ */
CsrBool CsrUiMenuGetitemReqHandler(CsrUiInstanceData *instanceData);             /* CSR_UI_MENU_GETITEM_REQ */
CsrBool CsrUiMenuSetcursorReqHandler(CsrUiInstanceData *instanceData);           /* CSR_UI_MENU_SETCURSOR_REQ */
CsrBool CsrUiMenuGetcursorReqHandler(CsrUiInstanceData *instanceData);           /* CSR_UI_MENU_GETCURSOR_REQ */
CsrBool CsrUiEventSetReqHandler(CsrUiInstanceData *instanceData);                /* CSR_UI_EVENT_SET_REQ */
CsrBool CsrUiEventGetReqHandler(CsrUiInstanceData *instanceData);                /* CSR_UI_EVENT_GET_REQ */
CsrBool CsrUiDialogSetReqHandler(CsrUiInstanceData *instanceData);               /* CSR_UI_DIALOG_SET_REQ */
CsrBool CsrUiDialogGetReqHandler(CsrUiInstanceData *instanceData);               /* CSR_UI_DIALOG_GET_REQ */
CsrBool CsrUiInputdialogSetReqHandler(CsrUiInstanceData *instanceData);          /* CSR_UI_INPUTDIALOG_SET_REQ */
CsrBool CsrUiInputdialogGetReqHandler(CsrUiInstanceData *instanceData);          /* CSR_UI_INPUTDIALOG_GET_REQ */
CsrBool CsrUiIdlescreenSetReqHandler(CsrUiInstanceData *instanceData);           /* CSR_UI_IDLESCREEN_SET_REQ */
CsrBool CsrUiIdlescreenGetReqHandler(CsrUiInstanceData *instanceData);           /* CSR_UI_IDLESCREEN_GET_REQ */
CsrBool CsrUiUieShowReqHandler(CsrUiInstanceData *instanceData);                 /* CSR_UI_UIE_SHOW_REQ */
CsrBool CsrUiUieHideReqHandler(CsrUiInstanceData *instanceData);                 /* CSR_UI_UIE_HIDE_REQ */
CsrBool CsrUiDisplaySetinputmodeReqHandler(CsrUiInstanceData *instanceData);     /* CSR_UI_DISPLAY_SETINPUTMODE_REQ */
CsrBool CsrUiDisplayGethandleReqHandler(CsrUiInstanceData *instanceData);        /* CSR_UI_DISPLAY_GETHANDLE_REQ */
CsrBool CsrUiStatusBatterySetReqHandler(CsrUiInstanceData *instanceData);        /* CSR_UI_STATUS_BATTERY_SET_REQ */
CsrBool CsrUiStatusRadiometerSetReqHandler(CsrUiInstanceData *instanceData);     /* CSR_UI_STATUS_RADIOMETER_SET_REQ */
CsrBool CsrUiStatusHeadsetSetReqHandler(CsrUiInstanceData *instanceData);        /* CSR_UI_STATUS_HEADSET_SET_REQ */
CsrBool CsrUiStatusBluetoothSetReqHandler(CsrUiInstanceData *instanceData);      /* CSR_UI_STATUS_BLUETOOTH_SET_REQ */
CsrBool CsrUiStatusWifiSetReqHandler(CsrUiInstanceData *instanceData);             /* CSR_UI_STATUS_WIFI_SET_REQ */
CsrBool CsrUiUieDestroyReqHandler(CsrUiInstanceData *instanceData);              /* CSR_UI_UIE_DESTROY_REQ */

void CsrUiFreeDownstreamMessageContents(CsrUint16 eventClass, void *message);

#ifdef __cplusplus
}
#endif

#endif /* CSR_UI_SEF_H__ */
