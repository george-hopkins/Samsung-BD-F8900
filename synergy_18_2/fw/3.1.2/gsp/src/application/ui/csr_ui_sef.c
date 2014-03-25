/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_sched.h"
#include "csr_ui_prim.h"
#include "csr_ui_ctrl.h"
#include "csr_ui_uiemanager.h"
#include "csr_ui_exception.h"
#include "csr_ui_displaystack.h"
#include "csr_ui_util.h"
#include "csr_ui_renderer.h"

CsrBool CsrUiUieCreateReqHandler(CsrUiInstanceData *instanceData)               /* CSR_UI_UIE_CREATE_REQ */
{
    CsrUiUieCreateReq *request;
    CsrUiUieCreateCfm *confirm;
    CsrUieHandle hndl;

    request = (CsrUiUieCreateReq *) instanceData->msg;
    hndl = CsrUiUieCreate(instanceData, request->elementType);

    confirm = CsrPmemAlloc(sizeof(CsrUiUieCreateCfm));

    confirm->type = CSR_UI_UIE_CREATE_CFM;
    confirm->handle = hndl;
    confirm->elementType = request->elementType;

    CsrSchedMessagePut(request->phandle, CSR_UI_PRIM, confirm);

    return FALSE;
}

CsrBool CsrUiMenuSetReqHandler(CsrUiInstanceData *instanceData)                 /* CSR_UI_MENU_SET_REQ */
{
    CsrUiMenuSetReq *request;
    Menu *menu;

    request = (CsrUiMenuSetReq *) instanceData->msg;

    menu = (Menu *) CsrUiUieHandle2Pointer(instanceData, request->menu);
    if (*((CsrUieType *) menu) != CSR_UI_UIETYPE_MENU)
    {
        CSR_UIEXCEPTION("Attempted to set a Menu in an invalid UIE type (not a Menu)");
    }

    CsrPmemFree(menu->heading);
    menu->heading = request->heading;
    CsrPmemFree(menu->textSK1);
    menu->textSK1 = request->textSK1;
    CsrPmemFree(menu->textSK2);
    menu->textSK2 = request->textSK2;

    if (instanceData->DisplayStack && (((DSitem *) (instanceData->DisplayStack))->handle == request->menu))
    {
        /* Redraw needed */
        return TRUE;
    }

    return FALSE;
}

CsrBool CsrUiMenuGetReqHandler(CsrUiInstanceData *instanceData)                 /* CSR_UI_MENU_GET_REQ */
{
    CSR_UIEXCEPTION("This get method has not been implemented");
    return FALSE;
}

CsrBool CsrUiMenuAdditemReqHandler(CsrUiInstanceData *instanceData)             /* CSR_UI_MENU_ADDITEM_REQ */
{
    CsrUiMenuAdditemReq *request;
    request = (CsrUiMenuAdditemReq *) instanceData->msg;
    CsrUiUieMenuAdditem(instanceData, request->menu, request->key, request->icon, request->label, request->sublabel, request->actionSK1, request->actionSK2, request->actionBACK, request->actionDEL, request->position);

    if (instanceData->DisplayStack && (((DSitem *) (instanceData->DisplayStack))->handle == request->menu))
    {
        /* Redraw needed */
        return TRUE;
    }

    return FALSE;
}

CsrBool CsrUiMenuRemoveitemReqHandler(CsrUiInstanceData *instanceData)          /* CSR_UI_MENU_REMOVEITEM_REQ */
{
    CsrUiMenuRemoveitemReq *request;
    request = (CsrUiMenuRemoveitemReq *) instanceData->msg;
    CsrUiUieMenuRemoveitem(instanceData, request->menu, request->key);

    if (instanceData->DisplayStack && (((DSitem *) (instanceData->DisplayStack))->handle == request->menu))
    {
        /* Redraw needed */
        return TRUE;
    }

    return FALSE;
}

CsrBool CsrUiMenuRemoveallitemsReqHandler(CsrUiInstanceData *instanceData)          /* CSR_UI_MENU_REMOVEITEM_REQ */
{
    CsrUiMenuRemoveallitemsReq *request;
    request = (CsrUiMenuRemoveallitemsReq *) instanceData->msg;
    CsrUiUieMenuRemoveallitems(instanceData, request->menu);

    if (instanceData->DisplayStack && (((DSitem *) (instanceData->DisplayStack))->handle == request->menu))
    {
        /* Redraw needed */
        return TRUE;
    }

    return FALSE;
}

CsrBool CsrUiMenuSetitemReqHandler(CsrUiInstanceData *instanceData)             /* CSR_UI_MENU_SETITEM_REQ */
{
    CsrUiMenuSetitemReq *request;
    request = (CsrUiMenuSetitemReq *) instanceData->msg;
    CsrUiUieMenuSetitem(instanceData, request->menu, request->key, request->icon, request->label, request->sublabel, request->actionSK1, request->actionSK2, request->actionBACK, request->actionDEL);

    if (instanceData->DisplayStack && (((DSitem *) (instanceData->DisplayStack))->handle == request->menu))
    {
        /* Redraw needed */
        return TRUE;
    }

    return FALSE;
}

CsrBool CsrUiMenuGetitemReqHandler(CsrUiInstanceData *instanceData)             /* CSR_UI_MENU_GETITEM_REQ */
{
    CsrUiMenuGetitemReq *request;
    CsrUiMenuGetitemCfm *confirm;
    MenuItem *menuItem;

    request = (CsrUiMenuGetitemReq *) instanceData->msg;

    menuItem = CsrUiUieMenuGetitem(instanceData, request->menu, request->key);

    confirm = CsrPmemAlloc(sizeof(CsrUiMenuGetitemCfm));

    confirm->type = CSR_UI_MENU_GETITEM_CFM;
    confirm->handle = request->menu;
    confirm->key = request->key;
    confirm->icon = menuItem->icon;
    confirm->label = CsrUiUtilStringCopy(menuItem->label);
    confirm->sublabel = CsrUiUtilStringCopy(menuItem->sublabel);
    confirm->actionSK1 = menuItem->actionSK1;
    confirm->actionSK2 = menuItem->actionSK2;
    confirm->actionBACK = menuItem->actionBACK;
    confirm->actionDEL = menuItem->actionDEL;

    CsrSchedMessagePut(request->phandle, CSR_UI_PRIM, confirm);

    return FALSE;
}

CsrBool CsrUiMenuSetcursorReqHandler(CsrUiInstanceData *instanceData)             /* CSR_UI_MENU_SETCURSOR_REQ */
{
    CsrUiMenuSetcursorReq *request;
    request = (CsrUiMenuSetcursorReq *) instanceData->msg;
    CsrUiUieMenuSetCursorPosition(instanceData, request->menu, request->key);

    if (instanceData->DisplayStack && (((DSitem *) (instanceData->DisplayStack))->handle == request->menu))
    {
        /* Redraw needed */
        return TRUE;
    }

    return FALSE;
}

CsrBool CsrUiMenuGetcursorReqHandler(CsrUiInstanceData *instanceData)             /* CSR_UI_MENU_GETCURSOR_REQ */
{
    CsrUiMenuGetcursorReq *request;
    CsrUiMenuGetcursorCfm *confirm;

    request = (CsrUiMenuGetcursorReq *) instanceData->msg;

    confirm = CsrPmemAlloc(sizeof(CsrUiMenuGetcursorCfm));
    confirm->type = CSR_UI_MENU_GETCURSOR_CFM;
    confirm->handle = request->menu;
    confirm->key = CsrUiUieMenuGetCursorPosition(instanceData, request->menu);

    CsrSchedMessagePut(request->phandle, CSR_UI_PRIM, confirm);

    return FALSE;
}

CsrBool CsrUiEventSetReqHandler(CsrUiInstanceData *instanceData)                /* CSR_UI_EVENT_SET_REQ */
{
    CsrUiEventSetReq *request;
    Event *event;
    request = (CsrUiEventSetReq *) instanceData->msg;

    event = (Event *) CsrUiUieHandle2Pointer(instanceData, request->event);
    if (*((CsrUieType *) event) != CSR_UI_UIETYPE_EVENT)
    {
        CSR_UIEXCEPTION("Attempted to set an Event in an invalid UIE type (not an Event)");
    }

    event->inputMode = request->inputMode;
    event->listener = request->listener;

    return FALSE;
}

CsrBool CsrUiEventGetReqHandler(CsrUiInstanceData *instanceData)                /* CSR_UI_EVENT_GET_REQ */
{
    CSR_UIEXCEPTION("This get method has not been implemented");
    return FALSE;
}

CsrBool CsrUiDialogSetReqHandler(CsrUiInstanceData *instanceData)               /* CSR_UI_DIALOG_SET_REQ */
{
    CsrUiDialogSetReq *request;
    Dialog *dialog;
    request = (CsrUiDialogSetReq *) instanceData->msg;

    dialog = (Dialog *) CsrUiUieHandle2Pointer(instanceData, request->dialog);
    if (*((CsrUieType *) dialog) != CSR_UI_UIETYPE_DIALOG)
    {
        CSR_UIEXCEPTION("Attempted to set a Dialog in an invalid UIE type (not a Dialog)");
    }

    dialog->actionSK1 = request->actionSK1;
    dialog->actionSK2 = request->actionSK2;
    dialog->actionBACK = request->actionBACK;
    dialog->actionDEL = request->actionDEL;
    CsrPmemFree(dialog->heading);
    dialog->heading = request->heading;
    CsrPmemFree(dialog->message);
    dialog->message = request->message;
    CsrPmemFree(dialog->textSK1);
    dialog->textSK1 = request->textSK1;
    CsrPmemFree(dialog->textSK2);
    dialog->textSK2 = request->textSK2;

    if (instanceData->DisplayStack && (((DSitem *) (instanceData->DisplayStack))->handle == request->dialog))
    {
        /* Redraw needed */
        return TRUE;
    }

    return FALSE;
}

CsrBool CsrUiDialogGetReqHandler(CsrUiInstanceData *instanceData)               /* CSR_UI_DIALOG_GET_REQ */
{
    CSR_UIEXCEPTION("This get method has not been implemented");
    return FALSE;
}

CsrBool CsrUiInputdialogSetReqHandler(CsrUiInstanceData *instanceData)          /* CSR_UI_INPUTDIALOG_SET_REQ */
{
    CsrUiInputdialogSetReq *request;
    InputDialog *inputDialog;
    request = (CsrUiInputdialogSetReq *) instanceData->msg;

    inputDialog = (InputDialog *) CsrUiUieHandle2Pointer(instanceData, request->inputDialog);
    if (*((CsrUieType *) inputDialog) != CSR_UI_UIETYPE_INPUTDIALOG)
    {
        CSR_UIEXCEPTION("Attempted to set an Input Dialog in an invalid UIE type (not an Input Dialog)");
    }

    inputDialog->actionSK1 = request->actionSK1;
    inputDialog->actionSK2 = request->actionSK2;
    inputDialog->actionBACK = request->actionBACK;
    inputDialog->actionDEL = request->actionDEL;

    CsrPmemFree(inputDialog->heading);
    inputDialog->heading = request->heading;
    CsrPmemFree(inputDialog->message);
    inputDialog->message = request->message;
    inputDialog->icon = request->icon;
    inputDialog->textLength = request->textLength;
    inputDialog->keyMap = request->keyMap;

    /* Reallocate buffer to match new size */
    CsrPmemFree(inputDialog->text);
    inputDialog->text = CsrPmemZalloc(2 * (inputDialog->textLength + 1));

    /* Copy new string into buffer */
    if (request->text != NULL)
    {
        CsrUint16 *ptr1;
        CsrUint16 *ptr2;
        CsrUint16 c = 0;
        ptr1 = inputDialog->text;
        for (ptr2 = request->text; (*ptr2 != 0) && (c < inputDialog->textLength); c++)
        {
            *ptr1++ = *ptr2++;
        }
    }
    CsrPmemFree(request->text);

    CsrPmemFree(inputDialog->textSK1);
    inputDialog->textSK1 = request->textSK1;
    CsrPmemFree(inputDialog->textSK2);
    inputDialog->textSK2 = request->textSK2;

    if (instanceData->DisplayStack && (((DSitem *) (instanceData->DisplayStack))->handle == request->inputDialog))
    {
        /* Redraw needed */
        return TRUE;
    }

    return FALSE;
}

CsrBool CsrUiInputdialogGetReqHandler(CsrUiInstanceData *instanceData)          /* CSR_UI_INPUTDIALOG_GET_REQ */
{
    CsrUiInputdialogGetReq *request;
    CsrUiInputdialogGetCfm *confirm;
    InputDialog *inputDialog;
    CsrUint32 c;

    request = (CsrUiInputdialogGetReq *) instanceData->msg;

    inputDialog = (InputDialog *) CsrUiUieHandle2Pointer(instanceData, request->inputDialog);
    if (*((CsrUieType *) inputDialog) != CSR_UI_UIETYPE_INPUTDIALOG)
    {
        CSR_UIEXCEPTION("Attempted to get an Input Dialog in an invalid UIE type (not an Input Dialog)");
    }

    confirm = CsrPmemAlloc(sizeof(CsrUiInputdialogGetCfm));

    confirm->type = CSR_UI_INPUTDIALOG_GET_CFM;
    confirm->handle = request->inputDialog;
    confirm->text = CsrPmemAlloc((inputDialog->textLength + 1) * 2);
    c = (inputDialog->textLength + 1) * 2;
    while (c--)
    {
        ((CsrUint8 *) confirm->text)[c] = ((CsrUint8 *) inputDialog->text)[c];
    }

    CsrSchedMessagePut(request->phandle, CSR_UI_PRIM, confirm);

    return FALSE;
}

CsrBool CsrUiIdlescreenSetReqHandler(CsrUiInstanceData *instanceData)           /* CSR_UI_IDLESCREEN_SET_REQ */
{
    CsrUiIdlescreenSetReq *request;
    IdleScreen *idleScreen;
    request = (CsrUiIdlescreenSetReq *) instanceData->msg;

    idleScreen = (IdleScreen *) CsrUiUieHandle2Pointer(instanceData, request->idleScreen);
    if (*((CsrUieType *) idleScreen) != CSR_UI_UIETYPE_IDLESCREEN)
    {
        CSR_UIEXCEPTION("Attempted to set an Idle Screen in an invalid UIE type (not an Idle Screen)");
    }

    idleScreen->actionSK1 = request->actionSK1;
    idleScreen->actionSK2 = request->actionSK2;
    idleScreen->actionBACK = request->actionBACK;
    idleScreen->actionDEL = request->actionDEL;

    CsrPmemFree(idleScreen->heading);
    idleScreen->heading = request->heading;
    idleScreen->textLength = request->textLength;

    /* Reallocate buffer to match new size */
    CsrPmemFree(idleScreen->text);
    idleScreen->text = CsrPmemZalloc(2 * (idleScreen->textLength + 1));

    /* Copy new string into buffer */
    if (request->text != NULL)
    {
        CsrUint16 *ptr1;
        CsrUint16 *ptr2;
        CsrUint16 c = 0;
        ptr1 = idleScreen->text;
        for (ptr2 = request->text; (*ptr2 != 0) && (c < idleScreen->textLength); c++)
        {
            *ptr1++ = *ptr2++;
        }
    }
    CsrPmemFree(request->text);

    CsrPmemFree(idleScreen->textSK1);
    idleScreen->textSK1 = request->textSK1;
    CsrPmemFree(idleScreen->textSK2);
    idleScreen->textSK2 = request->textSK2;

    if (instanceData->DisplayStack && (((DSitem *) (instanceData->DisplayStack))->handle == request->idleScreen))
    {
        /* Redraw needed */
        return TRUE;
    }

    return FALSE;
}

CsrBool CsrUiIdlescreenGetReqHandler(CsrUiInstanceData *instanceData)           /* CSR_UI_IDLESCREEN_GET_REQ */
{
    CsrUiIdlescreenGetReq *request;
    CsrUiIdlescreenGetCfm *confirm;
    IdleScreen *idleScreen;
    CsrUint32 c;

    request = (CsrUiIdlescreenGetReq *) instanceData->msg;

    idleScreen = (IdleScreen *) CsrUiUieHandle2Pointer(instanceData, request->idleScreen);
    if (*((CsrUieType *) idleScreen) != CSR_UI_UIETYPE_IDLESCREEN)
    {
        CSR_UIEXCEPTION("Attempted to get an Idle Screen in an invalid UIE type (not an Idle Screen)");
    }

    confirm = CsrPmemAlloc(sizeof(CsrUiIdlescreenGetCfm));

    confirm->type = CSR_UI_IDLESCREEN_GET_CFM;
    confirm->handle = request->idleScreen;
    confirm->text = CsrPmemAlloc((idleScreen->textLength + 1) * 2);
    c = (idleScreen->textLength + 1) * 2;
    while (c--)
    {
        ((CsrUint8 *) confirm->text)[c] = ((CsrUint8 *) idleScreen->text)[c];
    }

    CsrSchedMessagePut(request->phandle, CSR_UI_PRIM, confirm);

    return FALSE;
}

CsrBool CsrUiUieShowReqHandler(CsrUiInstanceData *instanceData)                 /* CSR_UI_UIE_SHOW_REQ */
{
    CsrUiUieShowReq *request;
    request = (CsrUiUieShowReq *) instanceData->msg;

    return CsrUiDisplayUieShow(instanceData, request->element, request->priority, request->inputMode, request->listener);
}

CsrBool CsrUiUieHideReqHandler(CsrUiInstanceData *instanceData)                 /* CSR_UI_UIE_HIDE_REQ */
{
    CsrUiUieHideReq *request;
    request = (CsrUiUieHideReq *) instanceData->msg;

    return CsrUiDisplayUieHide(instanceData, request->element);
}

CsrBool CsrUiDisplaySetinputmodeReqHandler(CsrUiInstanceData *instanceData)     /* CSR_UI_DISPLAY_SETINPUTMODE_REQ */
{
    CsrUiDisplaySetinputmodeReq *request;
    request = (CsrUiDisplaySetinputmodeReq *) instanceData->msg;

    CsrUiDisplaySetinputmode(instanceData, request->element, request->inputMode);

    return FALSE;
}

CsrBool CsrUiDisplayGethandleReqHandler(CsrUiInstanceData *instanceData)        /* CSR_UI_DISPLAY_GETHANDLE_REQ */
{
    CsrUiDisplayGethandleReq *request;
    CsrUiDisplayGethandleCfm *confirm;
    CsrUieHandle hndl;
    request = (CsrUiDisplayGethandleReq *) instanceData->msg;

    hndl = CsrUiDisplayGethandle(instanceData);
    confirm = CsrPmemAlloc(sizeof(CsrUiDisplayGethandleCfm));

    confirm->type = CSR_UI_DISPLAY_GETHANDLE_CFM;
    confirm->handle = hndl;

    CsrSchedMessagePut(request->phandle, CSR_UI_PRIM, confirm);

    return FALSE;
}

CsrBool CsrUiStatusBatterySetReqHandler(CsrUiInstanceData *instanceData)      /* CSR_UI_STATUS_BATTERY_SET_REQ */
{
    CsrUiStatusBatterySetReq *request;

    request = (CsrUiStatusBatterySetReq *) instanceData->msg;

    instanceData->batteryPercentage = request->percentage;

    return TRUE;
}

CsrBool CsrUiStatusRadiometerSetReqHandler(CsrUiInstanceData *instanceData)   /* CSR_UI_STATUS_RADIOMETER_SET_REQ */
{
    CsrUiStatusRadiometerSetReq *request;

    request = (CsrUiStatusRadiometerSetReq *) instanceData->msg;

    instanceData->rssiPercentage = request->percentage;

    return TRUE;
}

CsrBool CsrUiStatusHeadsetSetReqHandler(CsrUiInstanceData *instanceData)      /* CSR_UI_STATUS_HEADSET_SET_REQ */
{
    CsrUiStatusHeadsetSetReq *request;

    request = (CsrUiStatusHeadsetSetReq *) instanceData->msg;

    if (request->headset1 != CSR_UI_STATUS_HEADSET_PRESERVE)
    {
        instanceData->headset1 = request->headset1;
    }
    if (request->headset2 != CSR_UI_STATUS_HEADSET_PRESERVE)
    {
        instanceData->headset2 = request->headset2;
    }

    return TRUE;
}

CsrBool CsrUiStatusBluetoothSetReqHandler(CsrUiInstanceData *instanceData)    /* CSR_UI_STATUS_BLUETOOTH_SET_REQ */
{
    CsrUiStatusBluetoothSetReq *request;

    request = (CsrUiStatusBluetoothSetReq *) instanceData->msg;

    instanceData->bluetoothActive = request->active;

    return TRUE;
}

CsrBool CsrUiStatusWifiSetReqHandler(CsrUiInstanceData *instanceData)           /* CSR_UI_STATUS_WIFI_SET_REQ */
{
    CsrUiStatusWifiSetReq *request;

    request = (CsrUiStatusWifiSetReq *) instanceData->msg;

    instanceData->wifiActive = request->active;

    return TRUE;
}

CsrBool CsrUiUieDestroyReqHandler(CsrUiInstanceData *instanceData)              /* CSR_UI_UIE_DESTROY_REQ */
{
    CsrUiUieDestroyReq *request;
    request = (CsrUiUieDestroyReq *) instanceData->msg;

    CsrUiUieDestroy(instanceData, request->element);

    return FALSE;
}
