/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_sched.h"
#include "csr_ui_prim.h"
#include "csr_ui_lib.h"
#include "csr_ui_exception.h"
#include "csr_pmem.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *        CsrUiUieCreateReqSend
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrUiUieCreateReq *CsrUiUieCreateReq_struct(CsrSchedQid phandle, CsrUieType elementType)
{
    CsrUiUieCreateReq *prim;

    prim = (CsrUiUieCreateReq *) CsrPmemAlloc(sizeof(CsrUiUieCreateReq));
    prim->type = CSR_UI_UIE_CREATE_REQ;
    prim->phandle = phandle;
    prim->elementType = elementType;
    return prim;
}

/*----------------------------------------------------------------------------*
*  NAME
*        CsrUiMenuSetReqSend
*
*  DESCRIPTION
*
*
*    PARAMETERS
*----------------------------------------------------------------------------*/
CsrUiMenuSetReq *CsrUiMenuSetReq_struct(CsrUieHandle menu, CsrUtf16String *heading, CsrUtf16String *textSK1, CsrUtf16String *textSK2)
{
    CsrUiMenuSetReq *prim;

    prim = (CsrUiMenuSetReq *) CsrPmemAlloc(sizeof(CsrUiMenuSetReq));
    prim->type = CSR_UI_MENU_SET_REQ;
    prim->menu = menu;
    prim->heading = heading;
    prim->textSK1 = textSK1;
    prim->textSK2 = textSK2;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *        CsrUiMenuGetReqSend
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrUiMenuGetReq *CsrUiMenuGetReq_struct(CsrSchedQid phandle, CsrUieHandle menu)
{
    CsrUiMenuGetReq *prim;

    prim = (CsrUiMenuGetReq *) CsrPmemAlloc(sizeof(CsrUiMenuGetReq));
    prim->type = CSR_UI_MENU_GET_REQ;
    prim->phandle = phandle;
    prim->menu = menu;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *        CsrUiMenuAdditemReqSend
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrUiMenuAdditemReq *CsrUiMenuAdditemReq_struct(CsrUieHandle menu, CsrUint16 position, CsrUint16 key, CsrUint16 icon, CsrUtf16String *label, CsrUtf16String *sublabel, CsrUieHandle actionSK1, CsrUieHandle actionSK2, CsrUieHandle actionBACK, CsrUieHandle actionDEL)
{
    CsrUiMenuAdditemReq *prim;

    prim = (CsrUiMenuAdditemReq *) CsrPmemAlloc(sizeof(CsrUiMenuAdditemReq));
    prim->type = CSR_UI_MENU_ADDITEM_REQ;
    prim->menu = menu;
    prim->position = position;
    prim->key = key;
    prim->icon = icon;
    prim->label = label;
    prim->sublabel = sublabel;
    prim->actionSK1 = actionSK1;
    prim->actionSK2 = actionSK2;
    prim->actionBACK = actionBACK;
    prim->actionDEL = actionDEL;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *        CsrUiMenuRemoveitemReqSend
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrUiMenuRemoveitemReq *CsrUiMenuRemoveitemReq_struct(CsrUieHandle menu, CsrUint16 key)
{
    CsrUiMenuRemoveitemReq *prim;

    prim = (CsrUiMenuRemoveitemReq *) CsrPmemAlloc(sizeof(CsrUiMenuRemoveitemReq));
    prim->type = CSR_UI_MENU_REMOVEITEM_REQ;
    prim->menu = menu;
    prim->key = key;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *        CsrUiMenuRemoveallitemsReqSend
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrUiMenuRemoveallitemsReq *CsrUiMenuRemoveallitemsReq_struct(CsrUieHandle menu)
{
    CsrUiMenuRemoveallitemsReq *prim;

    prim = (CsrUiMenuRemoveallitemsReq *) CsrPmemAlloc(sizeof(CsrUiMenuRemoveallitemsReq));
    prim->type = CSR_UI_MENU_REMOVEALLITEMS_REQ;
    prim->menu = menu;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *        CsrUiMenuSetitemReqSend
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrUiMenuSetitemReq *CsrUiMenuSetitemReq_struct(CsrUieHandle menu, CsrUint16 key, CsrUint16 icon, CsrUtf16String *label, CsrUtf16String *sublabel, CsrUieHandle actionSK1, CsrUieHandle actionSK2, CsrUieHandle actionBACK, CsrUieHandle actionDEL)
{
    CsrUiMenuSetitemReq *prim;

    prim = (CsrUiMenuSetitemReq *) CsrPmemAlloc(sizeof(CsrUiMenuSetitemReq));
    prim->type = CSR_UI_MENU_SETITEM_REQ;
    prim->menu = menu;
    prim->key = key;
    prim->icon = icon;
    prim->label = label;
    prim->sublabel = sublabel;
    prim->actionSK1 = actionSK1;
    prim->actionSK2 = actionSK2;
    prim->actionBACK = actionBACK;
    prim->actionDEL = actionDEL;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *        CsrUiMenuGetitemReqSend
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrUiMenuGetitemReq *CsrUiMenuGetitemReq_struct(CsrSchedQid phandle, CsrUieHandle menu, CsrUint16 key)
{
    CsrUiMenuGetitemReq *prim;

    prim = (CsrUiMenuGetitemReq *) CsrPmemAlloc(sizeof(CsrUiMenuGetitemReq));
    prim->type = CSR_UI_MENU_GETITEM_REQ;
    prim->phandle = phandle;
    prim->menu = menu;
    prim->key = key;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *        CsrUiMenuSetcursorReqSend
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrUiMenuSetcursorReq *CsrUiMenuSetcursorReq_struct(CsrUieHandle menu, CsrUint16 key)
{
    CsrUiMenuSetcursorReq *prim;

    prim = (CsrUiMenuSetcursorReq *) CsrPmemAlloc(sizeof(CsrUiMenuSetcursorReq));
    prim->type = CSR_UI_MENU_SETCURSOR_REQ;
    prim->menu = menu;
    prim->key = key;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *        CsrUiMenuGetcursorReqSend
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrUiMenuGetcursorReq *CsrUiMenuGetcursorReq_struct(CsrSchedQid phandle, CsrUieHandle menu)
{
    CsrUiMenuGetcursorReq *prim;

    prim = (CsrUiMenuGetcursorReq *) CsrPmemAlloc(sizeof(CsrUiMenuGetcursorReq));
    prim->type = CSR_UI_MENU_GETCURSOR_REQ;
    prim->phandle = phandle;
    prim->menu = menu;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *        CsrUiEventSetReqSend
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrUiEventSetReq *CsrUiEventSetReq_struct(CsrUieHandle event, CsrUint16 inputMode, CsrSchedQid listener)
{
    CsrUiEventSetReq *prim;

    prim = (CsrUiEventSetReq *) CsrPmemAlloc(sizeof(CsrUiEventSetReq));
    prim->type = CSR_UI_EVENT_SET_REQ;
    prim->event = event;
    prim->inputMode = inputMode;
    prim->listener = listener;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *        CsrUiEventGetReqSend
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrUiEventGetReq *CsrUiEventGetReq_struct(CsrSchedQid phandle, CsrUieHandle event)
{
    CsrUiEventGetReq *prim;

    prim = (CsrUiEventGetReq *) CsrPmemAlloc(sizeof(CsrUiEventGetReq));
    prim->type = CSR_UI_EVENT_GET_REQ;
    prim->phandle = phandle;
    prim->event = event;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *        CsrUiDialogSetReqSend
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrUiDialogSetReq *CsrUiDialogSetReq_struct(CsrUieHandle dialog, CsrUtf16String *heading, CsrUtf16String *message, CsrUtf16String *textSK1, CsrUtf16String *textSK2, CsrUieHandle actionSK1, CsrUieHandle actionSK2, CsrUieHandle actionBACK, CsrUieHandle actionDEL)
{
    CsrUiDialogSetReq *prim;

    prim = (CsrUiDialogSetReq *) CsrPmemAlloc(sizeof(CsrUiDialogSetReq));
    prim->type = CSR_UI_DIALOG_SET_REQ;
    prim->dialog = dialog;
    prim->heading = heading;
    prim->message = message;
    prim->textSK1 = textSK1;
    prim->textSK2 = textSK2;
    prim->actionSK1 = actionSK1;
    prim->actionSK2 = actionSK2;
    prim->actionBACK = actionBACK;
    prim->actionDEL = actionDEL;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *        CsrUiDialogGetReqSend
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrUiDialogGetReq *CsrUiDialogGetReq_struct(CsrSchedQid phandle, CsrUieHandle dialog)
{
    CsrUiDialogGetReq *prim;

    prim = (CsrUiDialogGetReq *) CsrPmemAlloc(sizeof(CsrUiDialogGetReq));
    prim->type = CSR_UI_DIALOG_GET_REQ;
    prim->phandle = phandle;
    prim->dialog = dialog;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *        CsrUiInputdialogSetReqSend
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrUiInputdialogSetReq *CsrUiInputdialogSetReq_struct(CsrUieHandle inputDialog, CsrUtf16String *heading, CsrUtf16String *message, CsrUint16 icon, CsrUint16 *text, CsrUint16 textLength, CsrUint8 keyMap, CsrUtf16String *textSK1, CsrUtf16String *textSK2, CsrUieHandle actionSK1, CsrUieHandle actionSK2, CsrUieHandle actionBACK, CsrUieHandle actionDEL)
{
    CsrUiInputdialogSetReq *prim;

    prim = (CsrUiInputdialogSetReq *) CsrPmemAlloc(sizeof(CsrUiInputdialogSetReq));
    prim->type = CSR_UI_INPUTDIALOG_SET_REQ;
    prim->inputDialog = inputDialog;
    prim->heading = heading;
    prim->message = message;
    prim->icon = icon;
    prim->text = text;
    prim->textLength = textLength;
    prim->keyMap = keyMap;
    prim->textSK1 = textSK1;
    prim->textSK2 = textSK2;
    prim->actionSK1 = actionSK1;
    prim->actionSK2 = actionSK2;
    prim->actionBACK = actionBACK;
    prim->actionDEL = actionDEL;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *        CsrUiInputdialogGetReqSend
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrUiInputdialogGetReq *CsrUiInputdialogGetReq_struct(CsrSchedQid phandle, CsrUieHandle inputDialog)
{
    CsrUiInputdialogGetReq *prim;

    prim = (CsrUiInputdialogGetReq *) CsrPmemAlloc(sizeof(CsrUiInputdialogGetReq));
    prim->type = CSR_UI_INPUTDIALOG_GET_REQ;
    prim->phandle = phandle;
    prim->inputDialog = inputDialog;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *        CsrUiIdlescreenSetReqSend
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrUiIdlescreenSetReq *CsrUiIdlescreenSetReq_struct(CsrUieHandle idleScreen, CsrUtf16String *heading, CsrUint16 *text, CsrUint16 textLength, CsrUtf16String *textSK1, CsrUtf16String *textSK2, CsrUieHandle actionSK1, CsrUieHandle actionSK2, CsrUieHandle actionBACK, CsrUieHandle actionDEL)
{
    CsrUiIdlescreenSetReq *prim;

    prim = (CsrUiIdlescreenSetReq *) CsrPmemAlloc(sizeof(CsrUiIdlescreenSetReq));
    prim->type = CSR_UI_IDLESCREEN_SET_REQ;
    prim->idleScreen = idleScreen;
    prim->heading = heading;
    prim->text = text;
    prim->textLength = textLength;
    prim->textSK1 = textSK1;
    prim->textSK2 = textSK2;
    prim->actionSK1 = actionSK1;
    prim->actionSK2 = actionSK2;
    prim->actionBACK = actionBACK;
    prim->actionDEL = actionDEL;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *        CsrUiIdlescreenGetReqSend
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrUiIdlescreenGetReq *CsrUiIdlescreenGetReq_struct(CsrSchedQid phandle, CsrUieHandle idleScreen)
{
    CsrUiIdlescreenGetReq *prim;

    prim = (CsrUiIdlescreenGetReq *) CsrPmemAlloc(sizeof(CsrUiIdlescreenGetReq));
    prim->type = CSR_UI_IDLESCREEN_GET_REQ;
    prim->phandle = phandle;
    prim->idleScreen = idleScreen;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *        CsrUiUieShowReqSend
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrUiUieShowReq *CsrUiUieShowReq_struct(CsrUieHandle element, CsrSchedQid listener, CsrUint16 inputMode, CsrUint16 priority)
{
    CsrUiUieShowReq *prim;

    prim = (CsrUiUieShowReq *) CsrPmemAlloc(sizeof(CsrUiUieShowReq));
    prim->type = CSR_UI_UIE_SHOW_REQ;
    prim->element = element;
    prim->listener = listener;
    prim->inputMode = inputMode;
    prim->priority = priority;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *        CsrUiUieHideReqSend
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrUiUieHideReq *CsrUiUieHideReq_struct(CsrUieHandle element)
{
    CsrUiUieHideReq *prim;

    prim = (CsrUiUieHideReq *) CsrPmemAlloc(sizeof(CsrUiUieHideReq));
    prim->type = CSR_UI_UIE_HIDE_REQ;
    prim->element = element;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *        CsrUiDisplaySetinputmodeReqSend
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrUiDisplaySetinputmodeReq *CsrUiDisplaySetinputmodeReq_struct(CsrUieHandle element, CsrUint16 inputMode)
{
    CsrUiDisplaySetinputmodeReq *prim;

    prim = (CsrUiDisplaySetinputmodeReq *) CsrPmemAlloc(sizeof(CsrUiDisplaySetinputmodeReq));
    prim->type = CSR_UI_DISPLAY_SETINPUTMODE_REQ;
    prim->element = element;
    prim->inputMode = inputMode;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *        CsrUiDisplayGethandleReqSend
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrUiDisplayGethandleReq *CsrUiDisplayGethandleReq_struct(CsrSchedQid phandle)
{
    CsrUiDisplayGethandleReq *prim;

    prim = (CsrUiDisplayGethandleReq *) CsrPmemAlloc(sizeof(CsrUiDisplayGethandleReq));
    prim->type = CSR_UI_DISPLAY_GETHANDLE_REQ;
    prim->phandle = phandle;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *        CsrUiStatusBatterySetReqSend
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrUiStatusBatterySetReq *CsrUiStatusBatterySetReq_struct(CsrUint8 percentage)
{
    CsrUiStatusBatterySetReq *prim;

    prim = (CsrUiStatusBatterySetReq *) CsrPmemAlloc(sizeof(CsrUiStatusBatterySetReq));
    prim->type = CSR_UI_STATUS_BATTERY_SET_REQ;
    prim->percentage = percentage;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *        CsrUiStatusRadiometerSetReqSend
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrUiStatusRadiometerSetReq *CsrUiStatusRadiometerSetReq_struct(CsrUint8 percentage)
{
    CsrUiStatusRadiometerSetReq *prim;

    prim = (CsrUiStatusRadiometerSetReq *) CsrPmemAlloc(sizeof(CsrUiStatusRadiometerSetReq));
    prim->type = CSR_UI_STATUS_RADIOMETER_SET_REQ;
    prim->percentage = percentage;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *        CsrUiStatusHeadsetSetReqSend
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrUiStatusHeadsetSetReq *CsrUiStatusHeadsetSetReq_struct(CsrInt8 headset1, CsrInt8 headset2)
{
    CsrUiStatusHeadsetSetReq *prim;

    prim = (CsrUiStatusHeadsetSetReq *) CsrPmemAlloc(sizeof(CsrUiStatusHeadsetSetReq));
    prim->type = CSR_UI_STATUS_HEADSET_SET_REQ;
    prim->headset1 = headset1;
    prim->headset2 = headset2;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *        CsrUiStatusBluetoothSetReqSend
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrUiStatusBluetoothSetReq *CsrUiStatusBluetoothSetReq_struct(CsrBool active)
{
    CsrUiStatusBluetoothSetReq *prim;

    prim = (CsrUiStatusBluetoothSetReq *) CsrPmemAlloc(sizeof(CsrUiStatusBluetoothSetReq));
    prim->type = CSR_UI_STATUS_BLUETOOTH_SET_REQ;
    prim->active = active;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *        CsrUiStatusWifiSetReqSend
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrUiStatusWifiSetReq *CsrUiStatusWifiSetReq_struct(CsrBool active)
{
    CsrUiStatusWifiSetReq *prim;

    prim = (CsrUiStatusWifiSetReq *) CsrPmemAlloc(sizeof(CsrUiStatusWifiSetReq));
    prim->type = CSR_UI_STATUS_WIFI_SET_REQ;
    prim->active = active;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *        CsrUiUieDestroyReqSend
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrUiUieDestroyReq *CsrUiUieDestroyReq_struct(CsrUieHandle element)
{
    CsrUiUieDestroyReq *prim;

    prim = (CsrUiUieDestroyReq *) CsrPmemAlloc(sizeof(CsrUiUieDestroyReq));
    prim->type = CSR_UI_UIE_DESTROY_REQ;
    prim->element = element;
    return prim;
}
