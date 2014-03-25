/****************************************************************************

   Copyright Cambridge Silicon Radio Limited and its licensors 2011.
   
   All rights reserved. 

REVISION:      $Revision: #1 $
 ****************************************************************************/

#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_sched.h"
#include "csr_ui_lib.h"
#include "csr_app_lib.h"
#include "csr_util.h"
#include "csr_bt_gatt_demo_generic_srv_prim.h"
#include "csr_bt_gatt_demo_generic_srv_lib.h"
#include "csr_bt_gatt_demo_app.h"
#include "csr_bt_gatt_demo_defines.h"
#include "csr_bt_gatt_demo_generic_server_app.h"
#include "csr_bt_gatt_demo_generic_server_app_strings.h"

void CsrBtGattAppHandleGenericSrvCsrUiActivateDeactivate(CsrBtGenericSrvAppInstData *inst, 
                                                   CsrBool activated, 
                                                   CsrBool success);
/* Helper functions */

CsrUtf8String *commonGenericSrvGetValueString( CsrUint16 value)
{
        switch(value)
        {
            case CSR_BT_GATT_APP_ALERT_LEVEL_OFF:
                {
                    return (CsrUtf8String *)CsrStrDup("Alert level changed to: Off");            
                }
            case CSR_BT_GATT_APP_ALERT_LEVEL_LOW:
                {
                    return (CsrUtf8String *)CsrStrDup("Alert level changed to: Low");
                }
            case CSR_BT_GATT_APP_ALERT_LEVEL_HIGH:
                {
                    return (CsrUtf8String *)CsrStrDup("Alert levet changed to: High");
                }
            default:
                {
                    return (CsrUtf8String *)CsrStrDup("Unknown");
                }
        }
}
void commonGenericSrvPopupSet(CsrBtGenericSrvAppInstData *inst, char *dialogHeading, char *dialogText)
{
    CsrUiDialogSetReqSend(inst->csrUiVar.hCommonDialog, 
                          CONVERT_TEXT_STRING_2_UCS2(dialogHeading), 
                          CONVERT_TEXT_STRING_2_UCS2(dialogText),
                          NULL, NULL, 0,0,0,0);
}

void commonGenericSrvAcceptPopupSet(CsrBtGenericSrvAppInstData *inst, char *dialogHeading, char *dialogText)
{
    CsrUiDialogSetReqSend(inst->csrUiVar.hCommonDialog, 
                          CONVERT_TEXT_STRING_2_UCS2(dialogHeading), 
                          CONVERT_TEXT_STRING_2_UCS2(dialogText),
                          TEXT_GENERIC_SRV_APP_OK_UCS2, 
                          NULL, 
                          inst->csrUiVar.hBackEvent,
                          0,
                          inst->csrUiVar.hBackEvent,
                          0);
}

void commonGenericSrvPopupShow(CsrBtGenericSrvAppInstData *inst)
{
    CsrUiUieShowReqSend(inst->csrUiVar.hCommonDialog,
                        CsrSchedTaskQueueGet(), 
                        CSR_UI_INPUTMODE_BLOCK, 
                        CSR_BT_GENERIC_SRV_APP_CSR_UI_PRI);
}

void commonGenericSrvAcceptPopupShow(CsrBtGenericSrvAppInstData *inst)
{
    CsrUiUieShowReqSend(inst->csrUiVar.hCommonDialog, 
                        CsrSchedTaskQueueGet(), 
                        CSR_UI_INPUTMODE_AUTO, 
                        CSR_BT_GENERIC_SRV_APP_CSR_UI_PRI);
}

void commonGenericSrvPopupHide(CsrBtGenericSrvAppInstData *inst)
{
    CsrUiUieHideReqSend(inst->csrUiVar.hCommonDialog);
}

void CsrBtGattAppAcceptPopup(CsrBtGenericSrvAppInstData *inst, char *dialogHeading, char *dialogText)
{
    commonGenericSrvAcceptPopupSet(inst, dialogHeading, dialogText);
    commonGenericSrvAcceptPopupShow(inst);
}


void CsrBtGattAppHandleGenericSrvCsrUiMenuUpdate(CsrBtGenericSrvAppInstData *inst, CsrBool indication, CsrBool notify)
{
    CsrBtGenericSrvAppUiInstData *csrUiVar = &inst->csrUiVar;
    CsrBtGattAppHandleGenericSrvCsrUiActivateDeactivate(inst, TRUE, TRUE);

    if(indication == TRUE)
    {
        CsrUiMenuAdditemReqSend(csrUiVar->hMainMenu, 
                                CSR_UI_LAST, 
                                CSR_BT_GENERIC_SRV_APP_INDICATE_KEY,
                                CSR_UI_ICON_BELL, 
                                TEXT_GENERIC_SRV_APP_INDICATE_UCS2, 
                                CONVERT_TEXT_STRING_2_UCS2("  "),
                                csrUiVar->hSk1Event, 
                                csrUiVar->hSk2Event, 
                                csrUiVar->hBackEvent, 
                                0);
    }

    if(notify == TRUE)
    {
        CsrUiMenuAdditemReqSend(csrUiVar->hMainMenu, 
                                CSR_UI_LAST, 
                                CSR_BT_GENERIC_SRV_APP_NOTIFY_KEY,
                                CSR_UI_ICON_BELL, 
                                TEXT_GENERIC_SRV_APP_NOTIFY_UCS2, 
                                CONVERT_TEXT_STRING_2_UCS2("  "),
                                csrUiVar->hSk1Event, 
                                csrUiVar->hSk2Event, 
                                csrUiVar->hBackEvent, 
                                0);
    }

    commonGenericSrvPopupHide(inst);
                        
}
void CsrBtGattAppHandleGenericSrvCsrUiValueUpdate(CsrBtGenericSrvAppInstData *inst, 
                                                   CsrUint16 handle, 
                                                   CsrUint16 value)
{

    switch(handle - inst->dbStartHandle)
    {
        case CSR_BT_GENERIC_SRV_TEMP_MEASUREMENT_CC_HANDLE:
            {
                commonGenericSrvAcceptPopupSet(inst, "Temperature measurement", (char*)commonGenericSrvGetValueString(value));
                commonGenericSrvAcceptPopupShow(inst);
                break;
            }
        default:
            {
                CsrUtf8String *ret = CsrPmemAlloc(16);
                sprintf((char*)ret, "handle: 0x%04X", handle-inst->dbStartHandle);
                commonGenericSrvAcceptPopupSet(inst, "Error", (char*)ret);
                commonGenericSrvAcceptPopupShow(inst);
            }
    }

    
}


void CsrBtGattAppHandleGenericSrvCsrUiActivateDeactivate(CsrBtGenericSrvAppInstData *inst, 
                                                   CsrBool activated, 
                                                   CsrBool success)
{
    CsrBtGenericSrvAppUiInstData *csrUiVar = &inst->csrUiVar;

    if(activated)
    {
        if(success)
        {
            /* Update main menu so it is no longer possible to connect and activate but just to deactivate */
            char *addr = CsrPmemAlloc(15);
            sprintf(addr, "%04X:%02X:%06X", inst->connectedDeviceAddr.addr.nap, inst->connectedDeviceAddr.addr.uap, inst->connectedDeviceAddr.addr.lap);

            CsrUiMenuRemoveallitemsReqSend(csrUiVar->hMainMenu);

            CsrUiMenuAdditemReqSend(csrUiVar->hMainMenu, 
                                    CSR_UI_FIRST, 
                                    CSR_BT_GENERIC_SRV_APP_DEACTIVATE_KEY, 
                                    CSR_UI_ICON_DISCONNECT, 
                                    TEXT_GENERIC_SRV_APP_DEACTIVATE_UCS2, 
                                    CsrUtf82Utf16String((CsrUtf8String *) addr),
                                    csrUiVar->hSk1Event, 
                                    csrUiVar->hSk2Event, 
                                    csrUiVar->hBackEvent, 
                                    0);
            commonGenericSrvPopupSet(inst, "Advertising", "Please wait");
            commonGenericSrvPopupShow(inst);
            CsrPmemFree(addr);
        }
    }
    else
    {
        /* Update main menu so it is no longer possible to disconnect but just to connect and activate */
        CsrUiMenuRemoveallitemsReqSend(csrUiVar->hMainMenu);

        CsrUiMenuAdditemReqSend(csrUiVar->hMainMenu, 
                                CSR_UI_LAST, 
                                CSR_BT_GENERIC_SRV_APP_ACTIVATE_KEY, 
                                CSR_UI_ICON_NONE, 
                                TEXT_GENERIC_SRV_APP_ACTIVATE_UCS2, 
                                NULL,
                                csrUiVar->hSk1Event, 
                                csrUiVar->hSk2Event, 
                                csrUiVar->hBackEvent, 
                                0);
    }

    if(csrUiVar->localInitiatedAction)
    {
        csrUiVar->localInitiatedAction = FALSE;

        /* Remove popup */
        commonGenericSrvPopupHide(inst);

        CsrUiDisplaySetinputmodeReqSend(csrUiVar->hMainMenu, CSR_UI_INPUTMODE_AUTO);
    }
}



void CsrBtGattAppHandleGenericSrvSetMainMenuHeader(CsrBtGenericSrvAppInstData *inst)
{
    if (inst->csrUiVar.hMainMenu)
    { /* If the Main menu generation completed before we got the profile name we set it here */
        CsrUiMenuSetReqSend(inst->csrUiVar.hMainMenu, 
                            CONVERT_TEXT_STRING_2_UCS2("Generic Server"),
                            TEXT_SELECT_UCS2,
                            NULL);
    }
}

/* handler functions */
static void handleCsrUiUieCreateCfm(CsrBtGenericSrvAppInstData *inst, CsrUiUieCreateCfm *confirm)
{
    CsrBtGenericSrvAppUiInstData *csrUiVar = &inst->csrUiVar;

    if (confirm->elementType == CSR_UI_UIETYPE_EVENT)
    {
        if (!csrUiVar->hBackEvent)
        {
            CsrUiEventSetReqSend(confirm->handle, CSR_UI_INPUTMODE_AUTO, CsrSchedTaskQueueGet());
            csrUiVar->hBackEvent = confirm->handle;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else if (!csrUiVar->hSk1Event)
        {
            CsrUiEventSetReqSend(confirm->handle, CSR_UI_INPUTMODE_AUTO, CsrSchedTaskQueueGet());
            csrUiVar->hSk1Event = confirm->handle;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else if (!csrUiVar->hSk2Event)
        {
            CsrUiEventSetReqSend(confirm->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());
            csrUiVar->hSk2Event = confirm->handle;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_DIALOG);
        }
    }
    if (confirm->elementType == CSR_UI_UIETYPE_INPUTDIALOG)
    {
        /* not actually needed for this demo */  
    }
    if (confirm->elementType == CSR_UI_UIETYPE_DIALOG)
    {
        if (!csrUiVar->hCommonDialog)
        {
            csrUiVar->hCommonDialog = confirm->handle;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);
        }
    }
    if (confirm->elementType == CSR_UI_UIETYPE_MENU)
    {
        CsrUiMenuSetReqSend(confirm->handle,
                            CONVERT_TEXT_STRING_2_UCS2(inst->profileName),
                            TEXT_SELECT_UCS2, 
                            NULL);

        CsrUiMenuAdditemReqSend(confirm->handle,
                                CSR_UI_LAST, 
                                CSR_BT_GENERIC_SRV_APP_ACTIVATE_KEY,
                                CSR_UI_ICON_NONE,
                                TEXT_GENERIC_SRV_APP_ACTIVATE_UCS2,
                                NULL,
                                csrUiVar->hSk1Event,
                                csrUiVar->hSk2Event,
                                csrUiVar->hBackEvent, 0);

        csrUiVar->hMainMenu = confirm->handle;
    }
}

static void handleCsrUiInputDialogGetCfm(CsrBtGenericSrvAppInstData *inst, CsrUiInputdialogGetCfm *confirm)
{
    CsrBtGenericSrvAppUiInstData *csrUiVar = &inst->csrUiVar;

    /* Hide Filename input dialog */
    CsrUiUieHideReqSend(csrUiVar->hSelectFileInputDialog);

    CsrPmemFree(confirm->text);

}

static void handleCsrUiUieEventInd(CsrBtGenericSrvAppInstData *inst, CsrUiEventInd *indication)
{
    CsrBtGenericSrvAppUiInstData *csrUiVar = &inst->csrUiVar;;

    if (indication->event == csrUiVar->hBackEvent)
    {
       /*hide current shown displayelement*/
        CsrUiUieHideReqSend(indication->displayElement);
    }
    else if(indication->event == csrUiVar->hSelectFileInputSk1Event)
    {
        CsrUiInputdialogGetReqSend(CsrSchedTaskQueueGet(), csrUiVar->hSelectFileInputDialog);
    }
    else if (indication->event == csrUiVar->hSk1Event)
    {
        /* Main Menu handling */
        switch(indication->key)
        {
            case CSR_BT_GENERIC_SRV_APP_ACTIVATE_KEY:
                {
                    commonGenericSrvPopupSet(inst, "Advertising", "Please wait");
                    commonGenericSrvPopupShow(inst);
                    CsrBtGenericSrvAppStartActivate(inst);
                    inst->csrUiVar.localInitiatedAction = TRUE;
                    break;
                }
            case CSR_BT_GENERIC_SRV_APP_DEACTIVATE_KEY:
                {
                    commonGenericSrvPopupSet(inst, "Deactivating", "Please wait");
                    commonGenericSrvPopupShow(inst);
                    CsrBtGenericSrvAppStartDeactivate(inst);
                    inst->csrUiVar.localInitiatedAction = TRUE;
                    break;
                }
            case CSR_BT_GENERIC_SRV_APP_NOTIFY_KEY:
                {
                    break;
                }
            case CSR_BT_GENERIC_SRV_APP_INDICATE_KEY:
                {
                    break;
                }
            case CSR_BT_GENERIC_SRV_APP_OK_KEY:
                {
                    commonGenericSrvPopupHide(inst);
                    break;
                }
            default:
                {
                    break;
                }
        }
    }
    else if (indication->event == csrUiVar->hSk2Event)
    {
        CsrUiUieHideReqSend(indication->displayElement);
    }
}


void CsrBtGattAppHandleGenericSrvCsrUiPrim(CsrBtGenericSrvAppInstData *inst)
{
    switch(*((CsrUint16 *) inst->recvMsgP))
    {
        case CSR_UI_UIE_CREATE_CFM:
            {
                handleCsrUiUieCreateCfm(inst, inst->recvMsgP);
                break;
            }

        case CSR_UI_INPUTDIALOG_GET_CFM:
            {
                handleCsrUiInputDialogGetCfm(inst, inst->recvMsgP);
                break;
            }

        case CSR_UI_KEYDOWN_IND:
            {
                break;
            }

        case CSR_UI_EVENT_IND:
            {
                handleCsrUiUieEventInd(inst, inst->recvMsgP);
                break;
            }
        default:
            {
                break;
            }
    }
}


