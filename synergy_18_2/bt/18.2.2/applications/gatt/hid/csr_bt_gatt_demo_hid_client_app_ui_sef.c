/****************************************************************************

   Copyright Cambridge Silicon Radio Limited and its licensors 2011.
   
   All rights reserved. 



 ****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_sched.h"
#include "csr_ui_lib.h"
#include "csr_app_lib.h"
#include "csr_util.h"
#include "csr_bt_gatt_lib.h"
#include "csr_bt_gatt_demo_app.h"
#include "csr_bt_gatt_demo_hid_client_app.h"
#include "csr_bt_gatt_demo_hid_client_app_strings.h"
#include "csr_bt_gatt_demo_defines.h"


static char *hidc_present_substate[]={"Not Supported", "Unknown", "Not Present", "Present"};
static char *hidc_discharging_substate[]={"Not Supported", "Unknown", "Not Discharging", "Discharging"};
static char *hidc_charging_substate[]={"Unknown", "Not Chargeable", "Not Charging (Chargeable)", "Charging (Chargeable)"};
static char *hidc_critically_low_level_substate[]={"Not Supported", "Unknown", "Good Level", "Critically Low Level"};
static char *hidc_service_required[]={"Unknown","No Service Required","Service Required"};


/* Helper functions */

CsrBtUuid *commonHidcCreateUuid128ForDemo(void)
{
    CsrBtUuid *pUuid = CsrPmemAlloc(sizeof(CsrBtUuid));
    /********** this is only needed because we use a UUID128 until HID uuids are defined ***************/
    pUuid->length    =  CSR_BT_UUID128_SIZE;
    pUuid->uuid[0]   =  0x31;
    pUuid->uuid[1]   =  0x32;
    pUuid->uuid[2]   =  0x33;
    pUuid->uuid[3]   =  0x34;
    pUuid->uuid[4]   =  0x35;
    pUuid->uuid[5]   =  0x36;
    pUuid->uuid[6]   =  0x37;
    pUuid->uuid[7]   =  0x38;
    pUuid->uuid[8]   =  0x31;
    pUuid->uuid[9]   =  0x32;
    pUuid->uuid[10]  =  0x33;
    pUuid->uuid[11]  =  0x34;
    pUuid->uuid[12]  =  0x35;
    pUuid->uuid[13]  =  0x36;
    pUuid->uuid[14]  =  0x37;
    pUuid->uuid[15]  =  0x38;
    return pUuid;
}
CsrUtf16String *commonHidcGetValueString(CsrBtHidcAppInstData *inst, CsrUint16 uuid)
{

    CsrBtGattAppDbPrimServiceElement *pElem;
    char *ret              =  NULL;
    CsrUtf16String *ret16  =  NULL;

    pElem = CSR_BT_GATT_APP_DB_FIND_PRIM_SERVICE_BY_UUID(inst->dbPrimServices,
                                                         /* only until HID UUIDs have been defined*/commonHidcCreateUuid128ForDemo());
    if(pElem != NULL)
    {
        switch(uuid)
        {
            case CSR_BT_GATT_APP_HID_INTERUPT_REPORT_CHARAC_UUID:
            {
                ret = (inst->interuptNotif)?CsrStrDup("Updates: On"):CsrStrDup("Updates: Off");
                break;
            }
            case CSR_BT_GATT_APP_HID_CONTROL_REPORT_CHARAC_UUID:
            {
                ret =  (inst->reportNotif)?CsrStrDup("Updates: On"):CsrStrDup("Updates: Off");
                break;
            }
            case CSR_BT_GATT_APP_HID_REPORT_DESCRIPTOR_CHARAC_UUID:
            {
                CsrBtGattAppDbCharacElement *cElem = CSR_BT_GATT_APP_DB_FIND_CHARAC_BY_UUID(pElem->characList, 
                                                                                            CsrBtHidcAppUuid16ToUuid128(CSR_BT_GATT_APP_HID_REPORT_DESCRIPTOR_CHARAC_UUID));
                if(cElem != NULL && cElem->value != NULL)
                {
                    ret = CsrPmemAlloc(30);
                    sprintf(ret, "Value: 0x%02X%02X%02X%02X", cElem->value[0], cElem->value[1], cElem->value[2], cElem->value[3]);
                }
                break;
            }
            case CSR_BT_GATT_APP_BATTERY_UUID:
            {
                CsrBtGattAppDbCharacElement *cElem = CSR_BT_GATT_APP_DB_FIND_CHARAC_BY_UUID(pElem->characList, 
                                                                                            CsrBtHidcAppUuid16ToUuid128(CSR_BT_GATT_APP_BATT_LEVEL_CHARAC_UUID));
                if(cElem != NULL && cElem->value != NULL)
                {
                    ret = CsrPmemAlloc(12);
                    sprintf(ret, "Level %d%%", (CsrUint8)cElem->value[0]);
                }
                break;
            }
            default:
            {
                ret = (char *)CsrStrDup("Unknown UUID");
            }
        }
    }
    if(ret == NULL)
    {
        if(inst->traversingDb)
        {
            ret = (char *)CsrStrDup("Loading...");
        }
        else 
        {
            ret = (char*)CsrStrDup("Not Found");
        }
    }
    ret16 = CsrUtf82Utf16String((CsrUtf8String *)ret);
    CsrPmemFree(ret);
    return ret16;

}

void commonHidcPopupSet(CsrBtHidcAppInstData *inst, char *dialogHeading, char *dialogText)
{
    CsrUiDialogSetReqSend(inst->csrUiVar.hCommonDialog, 
                          CONVERT_TEXT_STRING_2_UCS2(dialogHeading), 
                          CONVERT_TEXT_STRING_2_UCS2(dialogText),
                          NULL, NULL, 0,0,0,0);
}

void commonHidcAcceptPopupSet(CsrBtHidcAppInstData *inst, char *dialogHeading, char *dialogText)
{
    CsrUiDialogSetReqSend(inst->csrUiVar.hCommonDialog, 
                          CONVERT_TEXT_STRING_2_UCS2(dialogHeading), 
                          CONVERT_TEXT_STRING_2_UCS2(dialogText),
                          TEXT_HIDC_APP_OK_UCS2, 
                          NULL, 
                          inst->csrUiVar.hBackEvent,
                          0,
                          inst->csrUiVar.hBackEvent,
                          0);
}

void commonHidcPopupShow(CsrBtHidcAppInstData *inst)
{
    CsrUiUieShowReqSend(inst->csrUiVar.hCommonDialog,
                        CsrSchedTaskQueueGet(), 
                        CSR_UI_INPUTMODE_BLOCK, 
                        CSR_BT_HIDC_APP_CSR_UI_PRI);
}

void commonHidcAcceptPopupShow(CsrBtHidcAppInstData *inst)
{
    CsrUiUieShowReqSend(inst->csrUiVar.hCommonDialog, 
                        CsrSchedTaskQueueGet(), 
                        CSR_UI_INPUTMODE_AUTO, 
                        CSR_BT_HIDC_APP_CSR_UI_PRI);
}
void commonHidcAcceptInputPopupShow(CsrBtHidcAppInstData *inst, char *dialogHeading, char *dialogText)
{
    CsrUiUieShowReqSend(inst->csrUiVar.hInputDialog, 
                        CsrSchedTaskQueueGet(), 
                        CSR_UI_INPUTMODE_AUTO, 
                        CSR_BT_HIDC_APP_CSR_UI_PRI);
}

void commonHidcPopupHide(CsrBtHidcAppInstData *inst)
{
    CsrUiUieHideReqSend(inst->csrUiVar.hCommonDialog);
}

void CsrBtGattAppHandleHidcCsrUiRegisterUnregister(CsrBtHidcAppInstData *inst, CsrBool registered)
{
    CsrBtHidcAppUiInstData *csrUiVar = &inst->csrUiVar;

    if(registered)
    {
        /* Update main menu so it is no longer possible to activate but just to deactivate */
        CsrUiMenuAdditemReqSend(csrUiVar->hMainMenu, 
                                CSR_UI_LAST, 
                                CSR_BT_HIDC_APP_UNREGISTER_KEY, 
                                CSR_UI_ICON_NONE, 
                                TEXT_HIDC_APP_UNREGISTER_UCS2, 
                                NULL,
                                csrUiVar->hSk1Event, 
                                csrUiVar->hSk2Event, 
                                csrUiVar->hBackEvent, 
                                0);
        CsrUiMenuAdditemReqSend(csrUiVar->hMainMenu, 
                                CSR_UI_LAST, 
                                CSR_BT_HIDC_APP_CONNECT_KEY, 
                                CSR_UI_ICON_CONNECT, 
                                TEXT_HIDC_APP_CONNECT_UCS2, 
                                TEXT_HIDC_APP_CONNECT_SUB_UCS2,
                                csrUiVar->hSk1Event, 
                                csrUiVar->hSk2Event, 
                                csrUiVar->hBackEvent, 
                                0);
        CsrUiMenuAdditemReqSend(csrUiVar->hMainMenu, 
                                CSR_UI_LAST, 
                                CSR_BT_HIDC_APP_CONNECT_WHITELIST_KEY, 
                                CSR_UI_ICON_CONNECT, 
                                TEXT_HIDC_APP_CONNECT_WHITELIST_UCS2, 
                                TEXT_HIDC_APP_CONNECT_WHITELIST_SUB_UCS2,
                                csrUiVar->hSk1Event, 
                                csrUiVar->hSk2Event, 
                                csrUiVar->hBackEvent, 
                                0);



        CsrUiMenuRemoveitemReqSend(csrUiVar->hMainMenu, 
                                   CSR_BT_HIDC_APP_REGISTER_KEY);
    }
    else
    {
        /* Update main menu so it is no longer possible to unRegister but just to register */
        CsrUiMenuAdditemReqSend(csrUiVar->hMainMenu, 
                                CSR_UI_LAST, 
                                CSR_BT_HIDC_APP_REGISTER_KEY, 
                                CSR_UI_ICON_NONE, 
                                TEXT_HIDC_APP_REGISTER_UCS2, 
                                NULL,
                                csrUiVar->hSk1Event, 
                                csrUiVar->hSk2Event, 
                                csrUiVar->hBackEvent, 
                                0);

        CsrUiMenuRemoveitemReqSend(csrUiVar->hMainMenu, 
                                   CSR_BT_HIDC_APP_UNREGISTER_KEY);
        if(inst->btConnId == 0xff)
        {
            CsrUiMenuRemoveitemReqSend(csrUiVar->hMainMenu, 
                                       CSR_BT_HIDC_APP_CONNECT_KEY);
            CsrUiMenuRemoveitemReqSend(csrUiVar->hMainMenu, 
                                       CSR_BT_HIDC_APP_CONNECT_WHITELIST_KEY);
        }
        else
        {
            CsrUiMenuRemoveitemReqSend(csrUiVar->hMainMenu, 
                                       CSR_BT_HIDC_APP_DISCONNECT_KEY);
        }

    }
    /* Remove popup */
    commonHidcPopupHide(inst);
}

void CsrBtGattAppHandleHidcCsrUiMenuUpdate(CsrBtHidcAppInstData *inst)
{

    /*to prevent a panic in UI task */
    if(inst->btConnId != 0xff)
    {
        CsrBtHidcAppUiInstData *csrUiVar = &inst->csrUiVar;
        CsrUiMenuSetitemReqSend(csrUiVar->hMainMenu, 
                                CSR_BT_HIDC_APP_READ_REPORT_DESCRIPTOR_KEY, 
                                CSR_UI_ICON_MARK_INFORMATION, 
                                TEXT_HIDC_APP_READ_REPORT_DESCRIPTOR_UCS2, 
                                commonHidcGetValueString(inst, CSR_BT_GATT_APP_HID_REPORT_DESCRIPTOR_CHARAC_UUID),
                                csrUiVar->hSk1Event, 
                                csrUiVar->hSk2Event, 
                                csrUiVar->hBackEvent, 
                                0);
/*        CsrUiMenuSetitemReqSend(csrUiVar->hMainMenu, 
                                CSR_BT_HIDC_APP_TOGGLE_CONTROL_REPORT_KEY, 
                                CSR_UI_ICON_MARK_INFORMATION, 
                                TEXT_HIDC_APP_TOGGLE_CONTROL_REPORT_UCS2, 
                                commonHidcGetValueString(inst, CSR_BT_GATT_APP_HID_CONTROL_REPORT_CHARAC_UUID),
                                csrUiVar->hSk1Event, 
                                csrUiVar->hSk2Event, 
                                csrUiVar->hBackEvent, 
                                0);
 */
        CsrUiMenuSetitemReqSend(csrUiVar->hMainMenu, 
                                CSR_BT_HIDC_APP_TOGGLE_INTERUPT_REPORT_KEY, 
                                CSR_UI_ICON_MARK_INFORMATION, 
                                TEXT_HIDC_APP_TOGGLE_INTERUPT_REPORT_UCS2, 
                                commonHidcGetValueString(inst, CSR_BT_GATT_APP_HID_INTERUPT_REPORT_CHARAC_UUID),
                                csrUiVar->hSk1Event, 
                                csrUiVar->hSk2Event, 
                                csrUiVar->hBackEvent, 
                                0);
        CsrUiMenuSetitemReqSend(csrUiVar->hMainMenu, 
                                CSR_BT_HIDC_APP_BATTERY_INFO_KEY, 
                                CSR_UI_ICON_MARK_INFORMATION, 
                                TEXT_HIDC_APP_BATTERY_INFO_UCS2, 
                                commonHidcGetValueString(inst, CSR_BT_GATT_APP_BATTERY_UUID),
                                csrUiVar->hSk1Event, 
                                csrUiVar->hSk2Event, 
                                csrUiVar->hBackEvent, 
                                0);
    }

}

void CsrBtGattAppHandleHidcCsrUiNoSelectedDevAddr(CsrBtHidcAppInstData *inst)
{
    /*need to remove previous popup in order to show the one with ok button*/
    commonHidcPopupHide(inst);
    commonHidcAcceptPopupSet(inst, "Connecting", "No deviceAddr selected in GAP");
    commonHidcAcceptPopupShow(inst);
}

void CsrBtGattAppHandleHidcCsrUiStillTraversingDb(CsrBtHidcAppInstData *inst)
{
    /*need to remove previous popup in order to show the one with ok button*/
    commonHidcPopupHide(inst);
    commonHidcAcceptPopupSet(inst, "Warning", "Client is still traversing the remote database. please wait a few seconds and try again!");
    commonHidcAcceptPopupShow(inst);
}

void CsrBtGattAppHandleHidcCsrUiShowError(CsrBtHidcAppInstData *inst, char *title, char *error)
{
    /*need to remove previous popup in order to show the one with ok button*/
    commonHidcAcceptPopupSet(inst, title, error);
    commonHidcAcceptPopupShow(inst);
}

void CsrBtGattAppHandleHidcCsrUiReadValue(CsrBtHidcAppInstData *inst, char *str, CsrBool success)
{
    if(success)
    {
        /*need to remove previous popup in order to show the one with ok button*/
        commonHidcPopupHide(inst);
        CsrBtGattAppHandleHidcCsrUiMenuUpdate(inst);
    }
    else
    {
        /*need to remove previous popup in order to show the one with ok button*/
        commonHidcPopupHide(inst);
        commonHidcAcceptPopupSet(inst, "Reading", "Could not read value");
        commonHidcAcceptPopupShow(inst);
        CsrBtGattAppHandleHidcCsrUiMenuUpdate(inst);
    }
}
void CsrBtGattAppHandleHidcCsrUiWriteValue(CsrBtHidcAppInstData *inst, CsrBool success)
{
    if(success)
    {
        /*need to remove previous popup in order to show the one with ok button */
        commonHidcPopupHide(inst);
        CsrBtGattAppHandleHidcCsrUiMenuUpdate(inst);
    }
    else
    {
        /*need to remove previous popup in order to show the one with ok button*/
        commonHidcPopupHide(inst);
        commonHidcAcceptPopupSet(inst, "Writing", "Could not set value");
        commonHidcAcceptPopupShow(inst);
        CsrBtGattAppHandleHidcCsrUiMenuUpdate(inst);
    }
}

void CsrBtGattAppHandleHidcCsrUiShowBattInfo(CsrBtHidcAppInstData *inst)
{
    CsrBtGattAppDbPrimServiceElement *pElem;
    CsrBtGattAppDbCharacElement *cElem;
    CsrBtGattAppDbCharacElement *cElem2;
    
    pElem = CSR_BT_GATT_APP_DB_FIND_PRIM_SERVICE_BY_UUID(inst->dbPrimServices,
                                                         CsrBtHidcAppUuid16ToUuid128(CSR_BT_GATT_APP_BATTERY_UUID));
    cElem = CSR_BT_GATT_APP_DB_FIND_CHARAC_BY_UUID(pElem->characList, 
                                                   CsrBtHidcAppUuid16ToUuid128(CSR_BT_GATT_APP_BATT_MASK_CHARAC_UUID));
    
    cElem2 = CSR_BT_GATT_APP_DB_FIND_CHARAC_BY_UUID(pElem->characList, 
                                                   CsrBtHidcAppUuid16ToUuid128(CSR_BT_GATT_APP_BATT_SERVICE_REQUIRED_CHARAC_UUID));
    if(cElem != NULL && cElem->value != NULL && cElem2 != NULL && cElem2->value != NULL)
    {
        char *ret = CsrPmemZalloc(300);
        sprintf(ret, "Batt. present: %s\nDischarging: %s\nCharging: %s\nCritical: %s\nService Required: %s",
                    hidc_present_substate[(CsrUint8)cElem->value[0] & CSR_BT_GATT_BATT_STATUS_MASK],
                    hidc_discharging_substate[((CsrUint8)cElem->value[0]>>2) & CSR_BT_GATT_BATT_STATUS_MASK],
                    hidc_charging_substate[((CsrUint8)cElem->value[0]>>4) & CSR_BT_GATT_BATT_STATUS_MASK],
                    hidc_critically_low_level_substate[((CsrUint8)cElem->value[0]>>6) & CSR_BT_GATT_BATT_STATUS_MASK],
                    hidc_service_required[(CsrUint8)cElem2->value[0] & CSR_BT_GATT_BATT_STATUS_MASK]);
        
        commonHidcPopupHide(inst);
        commonHidcAcceptPopupSet(inst, "Battery Info", ret);
        commonHidcAcceptPopupShow(inst);
        CsrBtGattAppHandleHidcCsrUiMenuUpdate(inst);
    }

            

}
void CsrBtGattAppHandleHidcCsrUiWhitelistAdd(CsrBtHidcAppInstData *inst, 
                                             CsrBool success)
{
    CsrBtHidcAppUiInstData *csrUiVar = &inst->csrUiVar;

    if(success)
    {
        CsrUiMenuSetitemReqSend(csrUiVar->hMainMenu, 
                                CSR_BT_HIDC_APP_ADD_WHITELIST_KEY, 
                                CSR_UI_ICON_MARK_ACCEPT, 
                                TEXT_HIDC_APP_ADD_WHITELIST_UCS2,
                                CsrUtf82Utf16String((CsrUtf8String *)CsrStrDup("Address added!")) , 
                                csrUiVar->hSk1Event, 
                                csrUiVar->hSk2Event, 
                                csrUiVar->hBackEvent, 
                                0);

    }
    else
    {
        CsrUiMenuSetitemReqSend(csrUiVar->hMainMenu, 
                                CSR_BT_HIDC_APP_ADD_WHITELIST_KEY, 
                                CSR_UI_ICON_MARK_DELETE, 
                                TEXT_HIDC_APP_ADD_WHITELIST_SUB_UCS2,
                                CsrUtf82Utf16String((CsrUtf8String *)CsrStrDup("Add Failed!")) , 
                                csrUiVar->hSk1Event, 
                                csrUiVar->hSk2Event, 
                                csrUiVar->hBackEvent, 
                                0);
    }

    if(csrUiVar->localInitiatedAction)
    {
        csrUiVar->localInitiatedAction = FALSE;

        /* Remove popup */
        commonHidcPopupHide(inst);

        CsrUiDisplaySetinputmodeReqSend(csrUiVar->hMainMenu, CSR_UI_INPUTMODE_AUTO);
    }
}

void CsrBtGattAppHandleHidcCsrUiConnectDisconnect(CsrBtHidcAppInstData *inst, 
                                                   CsrBool connected, 
                                                   CsrBool success)
{
    CsrBtHidcAppUiInstData *csrUiVar = &inst->csrUiVar;

    if(connected)
    {
        if(success)
        {
                char *addr = CsrPmemAlloc(15);
                sprintf(addr, "%04X:%02X:%06X", inst->selectedDeviceAddr.addr.nap, inst->selectedDeviceAddr.addr.uap, inst->selectedDeviceAddr.addr.lap);
                /* Update main menu so it is no longer possible to connect and activate but just to disconnect and CsrBtHidcAppSend file */
                CsrUiMenuRemoveallitemsReqSend(csrUiVar->hMainMenu);

                CsrUiMenuAdditemReqSend(csrUiVar->hMainMenu, 
                                        CSR_UI_FIRST, 
                                        CSR_BT_HIDC_APP_DISCONNECT_KEY, 
                                        CSR_UI_ICON_DISCONNECT, 
                                        TEXT_HIDC_APP_DISCONNECT_UCS2, 
                                        CsrUtf82Utf16String((CsrUtf8String *) addr),
                                        csrUiVar->hSk1Event, 
                                        csrUiVar->hSk2Event, 
                                        csrUiVar->hBackEvent, 
                                        0);
                CsrUiMenuAdditemReqSend(csrUiVar->hMainMenu, 
                                        CSR_UI_LAST, 
                                        CSR_BT_HIDC_APP_ADD_WHITELIST_KEY, 
                                        CSR_UI_ICON_MARK_ADD, 
                                        TEXT_HIDC_APP_ADD_WHITELIST_UCS2, 
                                        TEXT_HIDC_APP_ADD_WHITELIST_SUB_UCS2,
                                        csrUiVar->hSk1Event, 
                                        csrUiVar->hSk2Event, 
                                        csrUiVar->hBackEvent, 
                                        0);
                CsrUiMenuAdditemReqSend(csrUiVar->hMainMenu, 
                                        CSR_UI_LAST, 
                                        CSR_BT_HIDC_APP_READ_REPORT_DESCRIPTOR_KEY, 
                                        CSR_UI_ICON_MARK_INFORMATION, 
                                        TEXT_HIDC_APP_READ_REPORT_DESCRIPTOR_UCS2, 
                                        commonHidcGetValueString(inst, CSR_BT_GATT_APP_HID_REPORT_DESCRIPTOR_CHARAC_UUID),
                                        csrUiVar->hSk1Event, 
                                        csrUiVar->hSk2Event, 
                                        csrUiVar->hBackEvent, 
                                        0);
                CsrUiMenuAdditemReqSend(csrUiVar->hMainMenu, 
                                        CSR_UI_LAST, 
                                        CSR_BT_HIDC_APP_SHOW_INPUT_DIALOG_KEY, 
                                        CSR_UI_ICON_TEXT_WRITE, 
                                        TEXT_HIDC_APP_SHOW_INPUT_DIALOG_UCS2, 
                                        TEXT_HIDC_APP_SHOW_INPUT_DIALOG_SUB_UCS2,
                                        csrUiVar->hSk1Event, 
                                        csrUiVar->hSk2Event, 
                                        csrUiVar->hBackEvent, 
                                        0);
/*                CsrUiMenuAdditemReqSend(csrUiVar->hMainMenu, 
                                        CSR_UI_LAST, 
                                        CSR_BT_HIDC_APP_TOGGLE_CONTROL_REPORT_KEY, 
                                        CSR_UI_ICON_MARK_INFORMATION, 
                                        TEXT_HIDC_APP_TOGGLE_CONTROL_REPORT_UCS2, 
                                        commonHidcGetValueString(inst, CSR_BT_GATT_APP_HID_CONTROL_REPORT_CHARAC_UUID),
                                        csrUiVar->hSk1Event, 
                                        csrUiVar->hSk2Event, 
                                        csrUiVar->hBackEvent, 
                                        0);
*/
                CsrUiMenuAdditemReqSend(csrUiVar->hMainMenu, 
                                        CSR_UI_LAST, 
                                        CSR_BT_HIDC_APP_TOGGLE_INTERUPT_REPORT_KEY, 
                                        CSR_UI_ICON_MARK_INFORMATION, 
                                        TEXT_HIDC_APP_TOGGLE_INTERUPT_REPORT_UCS2, 
                                        commonHidcGetValueString(inst, CSR_BT_GATT_APP_HID_INTERUPT_REPORT_CHARAC_UUID),
                                        csrUiVar->hSk1Event, 
                                        csrUiVar->hSk2Event, 
                                        csrUiVar->hBackEvent, 
                                        0);
                CsrUiMenuAdditemReqSend(csrUiVar->hMainMenu,
                                        CSR_UI_LAST,
                                        CSR_BT_HIDC_APP_BATTERY_INFO_KEY, 
                                        CSR_UI_ICON_MARK_INFORMATION, 
                                        TEXT_HIDC_APP_BATTERY_INFO_UCS2, 
                                        commonHidcGetValueString(inst, CSR_BT_GATT_APP_BATTERY_UUID),
                                        csrUiVar->hSk1Event, 
                                        csrUiVar->hSk2Event, 
                                        csrUiVar->hBackEvent, 
                                        0);
        }
    }
    else
    {
        /* Update main menu so it is no longer possible to disconnect but just to connect and activate */
        CsrUiMenuRemoveallitemsReqSend(csrUiVar->hMainMenu);

        CsrUiMenuAdditemReqSend(csrUiVar->hMainMenu, 
                                CSR_UI_FIRST, 
                                CSR_BT_HIDC_APP_UNREGISTER_KEY, 
                                CSR_UI_ICON_NONE, 
                                TEXT_HIDC_APP_UNREGISTER_UCS2, 
                                NULL,
                                csrUiVar->hSk1Event, 
                                csrUiVar->hSk2Event, 
                                csrUiVar->hBackEvent, 
                                0);
        CsrUiMenuAdditemReqSend(csrUiVar->hMainMenu, 
                                CSR_UI_LAST, 
                                CSR_BT_HIDC_APP_CONNECT_KEY, 
                                CSR_UI_ICON_CONNECT, 
                                TEXT_HIDC_APP_CONNECT_UCS2, 
                                TEXT_HIDC_APP_CONNECT_SUB_UCS2,
                                csrUiVar->hSk1Event, 
                                csrUiVar->hSk2Event, 
                                csrUiVar->hBackEvent, 
                                0);
        CsrUiMenuAdditemReqSend(csrUiVar->hMainMenu, 
                                CSR_UI_LAST, 
                                CSR_BT_HIDC_APP_CONNECT_WHITELIST_KEY, 
                                CSR_UI_ICON_CONNECT, 
                                TEXT_HIDC_APP_CONNECT_WHITELIST_UCS2, 
                                TEXT_HIDC_APP_CONNECT_WHITELIST_SUB_UCS2,
                                csrUiVar->hSk1Event, 
                                csrUiVar->hSk2Event, 
                                csrUiVar->hBackEvent, 
                                0);
    }

    if(csrUiVar->localInitiatedAction && !inst->whiteListConnect)
    {
        csrUiVar->localInitiatedAction = FALSE;

        /* Remove popup */
        commonHidcPopupHide(inst);

        CsrUiDisplaySetinputmodeReqSend(csrUiVar->hMainMenu, CSR_UI_INPUTMODE_AUTO);
    }
}



void CsrBtGattAppHandleHidcSetMainMenuHeader(CsrBtHidcAppInstData *inst)
{
    if (inst->csrUiVar.hMainMenu)
    { /* If the Main menu generation completed before we got the profile name we set it here */
        CsrUiMenuSetReqSend(inst->csrUiVar.hMainMenu, 
                            CONVERT_TEXT_STRING_2_UCS2("LE HID Host"),
                            TEXT_SELECT_UCS2,
                            NULL);
    }
}

/* handler functions */
static void handleCsrUiUieCreateCfm(CsrBtHidcAppInstData *inst, CsrUiUieCreateCfm *confirm)
{
    CsrBtHidcAppUiInstData *csrUiVar = &inst->csrUiVar;

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
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_INPUTDIALOG);
        }
        else if (!csrUiVar->hInputSk1Event)
        {
            CsrUiEventSetReqSend(confirm->handle, CSR_UI_INPUTMODE_AUTO, CsrSchedTaskQueueGet());
            csrUiVar->hInputSk1Event = confirm->handle;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_INPUTDIALOG);
        }
    }
    if (confirm->elementType == CSR_UI_UIETYPE_INPUTDIALOG)
    {
        if (!csrUiVar->hInputDialog)
        {
            CsrUiInputdialogSetReqSend(confirm->handle,CONVERT_TEXT_STRING_2_UCS2("Input test:"),
                                       CONVERT_TEXT_STRING_2_UCS2("input:"),
                                       CSR_UI_ICON_NONE,
                                       NULL, 1000, CSR_UI_KEYMAP_ALPHANUMERIC, TEXT_OK_UCS2, NULL, csrUiVar->hInputSk1Event,0,csrUiVar->hBackEvent,0);
            csrUiVar->hInputDialog = confirm->handle;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_DIALOG);
        }
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
                                CSR_BT_HIDC_APP_REGISTER_KEY,
                                CSR_UI_ICON_NONE,
                                TEXT_HIDC_APP_REGISTER_UCS2,
                                NULL,
                                csrUiVar->hSk1Event,
                                csrUiVar->hSk2Event,
                                csrUiVar->hBackEvent, 0);

        csrUiVar->hMainMenu = confirm->handle;
    }
}

static void handleCsrUiInputDialogGetCfm(CsrBtHidcAppInstData *inst, CsrUiInputdialogGetCfm *confirm)
{
    CsrBtHidcAppUiInstData *csrUiVar = &inst->csrUiVar;

    /* Hide Filename input dialog */
    CsrUiUieHideReqSend(csrUiVar->hInputDialog);

    CsrPmemFree(confirm->text);

}

static void handleCsrUiUieEventInd(CsrBtHidcAppInstData *inst, CsrUiEventInd *indication)
{
    CsrBtHidcAppUiInstData *csrUiVar = &inst->csrUiVar;;

    if (indication->event == csrUiVar->hBackEvent)
    {
        /*hide current shown displayelement*/
        CsrUiUieHideReqSend(indication->displayElement);
    }
    else if(indication->event == csrUiVar->hInputSk1Event)
    {
        CsrUiInputdialogGetReqSend(CsrSchedTaskQueueGet(), csrUiVar->hInputDialog);
    }
    else if (indication->event == csrUiVar->hSk1Event)
    {
        /* Main Menu handling */
        switch(indication->key)
        {
            case CSR_BT_HIDC_APP_REGISTER_KEY:
                {
                    commonHidcPopupSet(inst, "Registering", "Please wait");
                    commonHidcPopupShow(inst);
                    CsrBtHidcAppStartRegister(inst);
                    break;
                }
            case CSR_BT_HIDC_APP_UNREGISTER_KEY:
                {
                    commonHidcPopupSet(inst, "Unregistering", "Please wait");
                    commonHidcPopupShow(inst);
                    CsrBtHidcAppStartUnregister(inst);
                    break;
                }
            case CSR_BT_HIDC_APP_CONNECT_KEY:
                {
                    commonHidcPopupSet(inst, "Connecting", "Please wait");
                    commonHidcPopupShow(inst);
                    CsrBtHidcAppStartConnecting(inst);
                    inst->csrUiVar.localInitiatedAction = TRUE;
                    break;
                }
            case CSR_BT_HIDC_APP_CONNECT_WHITELIST_KEY:
                {
                    commonHidcPopupSet(inst, "Connecting", "Please wait");
                    commonHidcPopupShow(inst);
                    CsrBtHidcAppStartConnectWhitelist(inst);
                    inst->csrUiVar.localInitiatedAction = TRUE;
                    break;
                }
            case CSR_BT_HIDC_APP_DISCONNECT_KEY:
                {
                    commonHidcPopupSet(inst, "Disconnecting", "Please wait");
                    commonHidcPopupShow(inst);
                    CsrBtHidcAppStartDisconnect(inst);
                    inst->csrUiVar.localInitiatedAction = TRUE;
                    inst->whiteListConnect = FALSE;
                    break;
                }
            case CSR_BT_HIDC_APP_SHOW_INPUT_DIALOG_KEY:
                {
                    
                      commonHidcAcceptInputPopupShow(inst,"test input", "");
                      break;
                }
            case CSR_BT_HIDC_APP_TOGGLE_CONTROL_REPORT_KEY:
                {
                    
                   if(inst->traversingDb)
                    {
                        CsrBtGattAppHandleHidcCsrUiStillTraversingDb(inst);
                    }
                    else
                    {
                        CsrUint16 val = CSR_BT_GATT_CLIENT_CHARAC_CONFIG_NOTIFICATION; 
                        /********** this is only needed because we use a UUID128 until HID uuids are defined ***************/
                        CsrBtUuid *pUuid = commonHidcCreateUuid128ForDemo();
                        /***************************************************************************************************/
                        commonHidcPopupSet(inst, "Reports", "Toggling Report notifications.");
                        commonHidcPopupShow(inst);
                        if(inst->reportNotif)
                        {
                            val = CSR_BT_GATT_CLIENT_CHARAC_CONFIG_DEFAULT;
                            inst->reportNotif = FALSE;
                        }
                        else 
                        {
                            inst->reportNotif = TRUE;
                        }
                        CsrBtHidcAppStartWriteClientConfig(inst,
                                                           pUuid,
                                                           CsrBtHidcAppUuid16ToUuid128(CSR_BT_GATT_APP_HID_CONTROL_REPORT_CHARAC_UUID),
                                                           val);
                    }
                    break;
                }
            case CSR_BT_HIDC_APP_TOGGLE_INTERUPT_REPORT_KEY :
                {
                    if(inst->traversingDb)
                    {
                        CsrBtGattAppHandleHidcCsrUiStillTraversingDb(inst);
                    }
                    else
                    {
                        CsrUint16 val = CSR_BT_GATT_CLIENT_CHARAC_CONFIG_NOTIFICATION; 
                        /********** this is only needed because we use a UUID128 until HID uuids are defined ***************/
                        CsrBtUuid *pUuid = commonHidcCreateUuid128ForDemo();
                        /***************************************************************************************************/
                        commonHidcPopupSet(inst, "Reports", "Toggling Interupt notifications.");
                        commonHidcPopupShow(inst);
                        if(inst->interuptNotif)
                        {
                            val = CSR_BT_GATT_CLIENT_CHARAC_CONFIG_DEFAULT;
                            inst->interuptNotif = FALSE;
                        }
                        else 
                        {
                            inst->interuptNotif = TRUE;
                        }

                        CsrBtHidcAppStartWriteClientConfig(inst,
                                                           pUuid,
                                                           CsrBtHidcAppUuid16ToUuid128(CSR_BT_GATT_APP_HID_INTERUPT_REPORT_CHARAC_UUID),
                                                           val);
                    }
                    break;
                }
            case CSR_BT_HIDC_APP_ADD_WHITELIST_KEY :
                {
                        commonHidcAcceptPopupSet(inst, "Whitelist", "Adding currently connected device to whitelist. Afterwards you can reconnect using whitelist and wait for incoming HID events");
                        commonHidcAcceptPopupShow(inst);
                        CsrBtHidcAppStartAddWhitelist(inst, inst->connectedDeviceAddr);
                    
                    break;
                }
            case CSR_BT_HIDC_APP_BATTERY_INFO_KEY:
                {
                    if(inst->traversingDb)
                    {
                        CsrBtGattAppHandleHidcCsrUiStillTraversingDb(inst);
                    }
                    else
                    {
                        /*we have already found the valuehandle */
                        CsrBtGattAppHandleHidcCsrUiShowBattInfo(inst);
                    }
                    break;
                }
            case CSR_BT_HIDC_APP_OK_KEY:
                {
                    commonHidcPopupHide(inst);
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


void CsrBtGattAppHandleHidcCsrUiPrim(CsrBtHidcAppInstData *inst)
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


