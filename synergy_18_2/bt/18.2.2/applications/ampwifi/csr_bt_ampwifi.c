/*****************************************************************************

   Copyright Cambridge Silicon Radio Limited and its licensors 2012.
   
   All rights reserved.

*****************************************************************************/

#include "csr_synergy.h"

#include <stdlib.h>
#include <errno.h>

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_sched.h"
#include "csr_unicode.h"
#include "csr_exceptionhandler.h"
#include "csr_ui_lib.h"
#include "csr_app_lib.h"
#include "csr_bt_platform.h"
#include "csr_bt_ui_strings.h"
#include "csr_bt_file.h"
#include "csr_bt_util.h"
#include "csr_bt_cm_prim.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_ampwifi.h"
#include "csr_app_main.h"

#include "csr_bt_ampm_prim.h"
#include "csr_bt_ampm_lib.h"
#include "csr_wifi_sme_prim.h"
#include "csr_wifi_sme_lib.h"
#if ((WIFI_MAJOR_VERSION == 3) || (WIFI_MAJOR_VERSION == 4))
#include "csr_wifi_mib_repository.h"
#else /* (WIFI_MAJOR_VERSION == 3) || (WIFI_MAJOR_VERSION == 4) */
#include "csr_wifi_file_buffer_get.h"
#include "csr_arg_search.h"
#include "csr_util.h"
#include "csr_sched_init.h"
#endif /* (WIFI_MAJOR_VERSION == 3) || (WIFI_MAJOR_VERSION == 4) */

#define CSR_BT_AMPWIFI_LOCAL_ID     ((CsrBtAmpController) 1)

#if (WIFI_MAJOR_VERSION >= 5)
static CsrCharString *mibFileName=NULL;

static CsrUint8 ufmib_dat[] = {
 0x55, 0x44, 0x4d, 0x49, 0x01, 0x00, 0xc5, 0x02, 0x00, 0x00, 0x15, 0x00,
 0x30, 0x13, 0x06, 0x0d, 0x2b, 0x06, 0x01, 0x04, 0x01, 0x81, 0xad, 0x14,
 0x01, 0x01, 0x03, 0x04, 0x01, 0x02, 0x02, 0x65, 0x90, 0x14, 0x00, 0x30,
 0x12, 0x06, 0x0d, 0x2b, 0x06, 0x01, 0x04, 0x01, 0x81, 0xad, 0x14, 0x01,
 0x01, 0x03, 0x04, 0x0a, 0x02, 0x01, 0x01, 0x14, 0x00, 0x30, 0x12, 0x06,
 0x0d, 0x2b, 0x06, 0x01, 0x04, 0x01, 0x81, 0xad, 0x14, 0x01, 0x01, 0x03,
 0x04, 0x12, 0x02, 0x01, 0x09, 0x2b, 0x00, 0x30, 0x29, 0x06, 0x11, 0x2b,
 0x06, 0x01, 0x04, 0x01, 0x81, 0xad, 0x14, 0x01, 0x01, 0x03, 0x07, 0x0e,
 0x01, 0x01, 0x02, 0x01, 0x04, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x43, 0x33, 0x20, 0x31, 0x20, 0x31,
 0x40, 0x41, 0x15, 0x00, 0x30, 0x13, 0x06, 0x0e, 0x2b, 0x06, 0x01, 0x04,
 0x01, 0x81, 0xad, 0x14, 0x01, 0x01, 0x03, 0x07, 0x0e, 0x03, 0x02, 0x01,
 0x70, 0x17, 0x00, 0x30, 0x15, 0x06, 0x10, 0x2b, 0x06, 0x01, 0x04, 0x01,
 0x81, 0xad, 0x14, 0x01, 0x01, 0x03, 0x04, 0x09, 0x01, 0x02, 0x02, 0x02,
 0x01, 0x03, 0x14, 0x00, 0x30, 0x12, 0x06, 0x0d, 0x2b, 0x06, 0x01, 0x04,
 0x01, 0x81, 0xad, 0x14, 0x01, 0x01, 0x03, 0x08, 0x01, 0x02, 0x01, 0x03,
 0x15, 0x00, 0x30, 0x13, 0x06, 0x0d, 0x2b, 0x06, 0x01, 0x04, 0x01, 0x81,
 0xad, 0x14, 0x01, 0x03, 0x01, 0x01, 0x01, 0x02, 0x02, 0x00, 0x00, 0x14,
 0x00, 0x30, 0x12, 0x06, 0x0d, 0x2b, 0x06, 0x01, 0x04, 0x01, 0x81, 0xad,
 0x14, 0x01, 0x03, 0x01, 0x01, 0x02, 0x02, 0x01, 0x03, 0x14, 0x00, 0x30,
 0x12, 0x06, 0x0d, 0x2b, 0x06, 0x01, 0x04, 0x01, 0x81, 0xad, 0x14, 0x01,
 0x03, 0x01, 0x01, 0x04, 0x02, 0x01, 0x01, 0x14, 0x00, 0x30, 0x12, 0x06,
 0x0d, 0x2b, 0x06, 0x01, 0x04, 0x01, 0x81, 0xad, 0x14, 0x01, 0x01, 0x03,
 0x09, 0x01, 0x02, 0x01, 0x02, 0x1b, 0x00, 0x30, 0x19, 0x06, 0x13, 0x2b,
 0x06, 0x01, 0x04, 0x01, 0x81, 0xad, 0x14, 0x01, 0x01, 0x03, 0x07, 0x0f,
 0x01, 0x01, 0x03, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0xd0, 0x1a, 0x00,
 0x30, 0x18, 0x06, 0x13, 0x2b, 0x06, 0x01, 0x04, 0x01, 0x81, 0xad, 0x14,
 0x01, 0x01, 0x03, 0x07, 0x0f, 0x01, 0x01, 0x06, 0x01, 0x01, 0x01, 0x02,
 0x01, 0x1e, 0x1a, 0x00, 0x30, 0x18, 0x06, 0x13, 0x2b, 0x06, 0x01, 0x04,
 0x01, 0x81, 0xad, 0x14, 0x01, 0x01, 0x03, 0x07, 0x0f, 0x01, 0x01, 0x07,
 0x01, 0x01, 0x01, 0x02, 0x01, 0x0c, 0x1a, 0x00, 0x30, 0x18, 0x06, 0x13,
 0x2b, 0x06, 0x01, 0x04, 0x01, 0x81, 0xad, 0x14, 0x01, 0x01, 0x03, 0x07,
 0x0f, 0x01, 0x01, 0x08, 0x01, 0x01, 0x01, 0x02, 0x01, 0x05, 0x1a, 0x00,
 0x30, 0x18, 0x06, 0x13, 0x2b, 0x06, 0x01, 0x04, 0x01, 0x81, 0xad, 0x14,
 0x01, 0x01, 0x03, 0x07, 0x0f, 0x01, 0x01, 0x06, 0x01, 0x01, 0x02, 0x02,
 0x01, 0x19, 0x1a, 0x00, 0x30, 0x18, 0x06, 0x13, 0x2b, 0x06, 0x01, 0x04,
 0x01, 0x81, 0xad, 0x14, 0x01, 0x01, 0x03, 0x07, 0x0f, 0x01, 0x01, 0x07,
 0x01, 0x01, 0x02, 0x02, 0x01, 0x0d, 0x1a, 0x00, 0x30, 0x18, 0x06, 0x13,
 0x2b, 0x06, 0x01, 0x04, 0x01, 0x81, 0xad, 0x14, 0x01, 0x01, 0x03, 0x07,
 0x0f, 0x01, 0x01, 0x08, 0x01, 0x01, 0x02, 0x02, 0x01, 0x05, 0x14, 0x00,
 0x30, 0x12, 0x06, 0x0d, 0x2b, 0x06, 0x01, 0x04, 0x01, 0x81, 0xad, 0x14,
 0x01, 0x01, 0x03, 0x06, 0x02, 0x02, 0x01, 0xf0, 0x14, 0x00, 0x30, 0x12,
 0x06, 0x0d, 0x2b, 0x06, 0x01, 0x04, 0x01, 0x81, 0xad, 0x14, 0x01, 0x01,
 0x03, 0x06, 0x08, 0x02, 0x01, 0x08, 0x14, 0x00, 0x30, 0x12, 0x06, 0x0d,
 0x2b, 0x06, 0x01, 0x04, 0x01, 0x81, 0xad, 0x14, 0x01, 0x01, 0x03, 0x06,
 0x0a, 0x02, 0x01, 0x02, 0x5b, 0x00, 0x30, 0x59, 0x06, 0x0c, 0x2b, 0x06,
 0x01, 0x04, 0x01, 0x81, 0xad, 0x14, 0x01, 0x01, 0x02, 0x05, 0x04, 0x49,
 0x24, 0x49, 0x64, 0x3a, 0x20, 0x2f, 0x2f, 0x64, 0x65, 0x70, 0x6f, 0x74,
 0x2f, 0x64, 0x6f, 0x74, 0x31, 0x31, 0x2f, 0x63, 0x69, 0x6e, 0x64, 0x65,
 0x72, 0x65, 0x6c, 0x6c, 0x61, 0x42, 0x30, 0x37, 0x2f, 0x6d, 0x61, 0x63,
 0x2f, 0x6d, 0x69, 0x62, 0x5f, 0x64, 0x65, 0x66, 0x61, 0x75, 0x6c, 0x74,
 0x73, 0x2f, 0x4d, 0x32, 0x31, 0x30, 0x37, 0x5f, 0x52, 0x30, 0x32, 0x2f,
 0x75, 0x66, 0x6d, 0x69, 0x62, 0x2e, 0x74, 0x78, 0x74, 0x23, 0x34, 0x20,
 0x24, 0x13, 0x00, 0x30, 0x11, 0x06, 0x0b, 0x2a, 0x86, 0x48, 0xce, 0x34,
 0x01, 0x08, 0x01, 0x04, 0x01, 0x01, 0x02, 0x02, 0x00, 0x00, 0x13, 0x00,
 0x30, 0x11, 0x06, 0x0b, 0x2a, 0x86, 0x48, 0xce, 0x34, 0x01, 0x08, 0x01,
 0x04, 0x02, 0x01, 0x02, 0x02, 0x00, 0x00, 0x12, 0x00, 0x30, 0x10, 0x06,
 0x0b, 0x2a, 0x86, 0x48, 0xce, 0x34, 0x01, 0x08, 0x01, 0x05, 0x01, 0x01,
 0x02, 0x01, 0x9c, 0x12, 0x00, 0x30, 0x10, 0x06, 0x0b, 0x2a, 0x86, 0x48,
 0xce, 0x34, 0x01, 0x08, 0x01, 0x05, 0x02, 0x01, 0x02, 0x01, 0x9c
};

static CsrUint16 ufmib_dat_len = sizeof(ufmib_dat);
#endif /* WIFI_MAJOR_VERSION >= 5 */

static void csrBtAmpWifiAppSetWifiStatus(CsrBtAmpWifiInst *inst, CsrBool isEnabled)
{
    inst->wifiEnabled = isEnabled;
    CsrUiStatusWifiSetReqSend(isEnabled);
}

static void csrBtAmpWifiAppToggleWifiOnOff(CsrBtAmpWifiInst *inst)
{
    if(!inst->wifiEnabled)
    {
        /* Turn Wifi on */
        CsrWifiSmeDataBlock *mibFiles;
        CsrUint16 mibFilesCount;
        CsrWifiMacAddress macAddress;

#if (WIFI_MAJOR_VERSION >= 5)
        CsrCharString *param,*values;
#endif /* WIFI_MAJOR_VERSION >= 5 */

        mibFiles = NULL;
        mibFilesCount = 0;
        
        /* derive local WIFI MAC address from local BT address (invert last 24 bits) */
        CsrAppMainBluecoreLocalDeviceBluetoothAddressGet(macAddress.a);
        macAddress.a[3] = ~macAddress.a[3];
        macAddress.a[4] = ~macAddress.a[4];
        macAddress.a[5] = ~macAddress.a[5];

        /* MIB */
        mibFilesCount = 1;
        mibFiles = CsrPmemAlloc(sizeof(CsrWifiSmeDataBlock));

#if (WIFI_MAJOR_VERSION >= 5)
        if ((CsrArgSearch(NULL,"--bt-wifi-mib",&param,&values)) == NULL)
        {
            printf("A Mib File is needed. Please specify one with --bt-wifi-mib. Use the name 'static' for using the static array\n");

#ifdef ENABLE_SHUTDOWN
            CsrSchedStop();
#else /* ENABLE_SHUTDOWN */
            exit(0);
#endif /* ENABLE_SHUTDOWN */
        }
        else
        {
            CsrSize length = 0;
            CsrUint8 *buffer = NULL;
            CsrResult result;

            mibFileName = CsrStrDup(values);

            CsrAppBacklogReqSend(TECH_BT,
                                 CsrUtf8StrDup(CSR_BT_AMPWIFI_MAIN_ENTRY_NAME),
                                 FALSE,
                                 "MibFileName=%s\n",mibFileName);

            if (CsrStrCmp(values, "static") == 0)
            {
                mibFiles->length = ufmib_dat_len;
                mibFiles->data   = CsrMemCpy(CsrPmemAlloc(mibFiles->length), ufmib_dat, mibFiles->length);
            }
            else
            {
                result = CsrWifiFileBufferGet(mibFileName,&length,&buffer);

                /* Make sure the length will fit in 16 bits */
                if (result == CSR_RESULT_SUCCESS && length>0 && length<0x10000)
                {
                    mibFiles->length=(CsrUint16)length;
                    mibFiles->data=buffer;
                }
                else
                {
                    printf("MIB file load failed\n");
#ifdef ENABLE_SHUTDOWN
                    CsrSchedStop();
#else /* ENABLE_SHUTDOWN */
                    exit(0);
#endif /* ENABLE_SHUTDOWN */
                }
            }
        }
#else /* WIFI_MAJOR_VERSION >= 5 */
#if defined(CSR_WIFI_DESIGN_M2107_R02)
        mibFiles->length = CsrWifiMibRepositoryDesignM2107R02Length;
        mibFiles->data = CsrMemCpy(CsrPmemAlloc(mibFiles->length), CsrWifiMibRepositoryDesignM2107R02, mibFiles->length);
#elif defined(CSR_WIFI_DESIGN_M2107_R03)
        mibFiles->length = CsrWifiMibRepositoryDesignM2107R03Length;
        mibFiles->data = CsrMemCpy(CsrPmemAlloc(mibFiles->length), CsrWifiMibRepositoryDesignM2107R03, mibFiles->length);
#elif defined(CSR_WIFI_DESIGN_M2399_R03)
        /* Currently there is no MIB-file for M2399 but M2107R03 is used */
        mibFiles->length = CsrWifiMibRepositoryDesignM2107R03Length;
        mibFiles->data = CsrMemCpy(CsrPmemAlloc(mibFiles->length), CsrWifiMibRepositoryDesignM2107R03, mibFiles->length);
#else
#error Unknown Chip
#endif /* CSR_WIFI_DESIGN_M2107_R02 */
#endif /* WIFI_MAJOR_VERSION >= 5 */
        CsrWifiSmeWifiOnReqSend(inst->qid,
                    macAddress, mibFilesCount, mibFiles);
        CsrAppBacklogReqSend(TECH_BT, CsrUtf8StrDup(CSR_BT_AMPWIFI_MAIN_ENTRY_NAME),
                     FALSE, "Sent WifiOnReq, mac address %02x:%02x:%02x:%02x:%02x:%02x",
                     macAddress.a[0],
                     macAddress.a[1],
                     macAddress.a[2],
                     macAddress.a[3],
                     macAddress.a[4],
                     macAddress.a[5]);
    }
    else
    {
        /* Turn Wifi off */
        CsrWifiSmeWifiOffReqSend(inst->qid);
        CsrAppBacklogReqSend(TECH_BT, CsrUtf8StrDup(CSR_BT_AMPWIFI_MAIN_ENTRY_NAME),
                     FALSE, "Sent WifiOffReq");
    }
}

static void csrBtAmpWifiAppToggleCoexistence(CsrBtAmpWifiInst *inst)
{
    CsrWifiSmeCoexConfigGetReqSend(inst->qid);
    CsrAppBacklogReqSend(TECH_BT, CsrUtf8StrDup(CSR_BT_AMPWIFI_MAIN_ENTRY_NAME),
                         FALSE, "Get COEX configuration sent");

}

static void csrBtAmpWifiAppHandleUiCreateCfm(CsrBtAmpWifiInst *inst,
                                             CsrUiUieCreateCfm *cfm)
{
    if(cfm->elementType == CSR_UI_UIETYPE_EVENT)
    {
        if (!inst->uiBackEvent)
        {
            CsrUiEventSetReqSend(cfm->handle, CSR_UI_INPUTMODE_AUTO, inst->qid);
            inst->uiBackEvent = cfm->handle;
            CsrUiUieCreateReqSend(inst->qid, CSR_UI_UIETYPE_EVENT);
        }
        else if (!inst->uiSk1Event)
        {
            CsrUiEventSetReqSend(cfm->handle, CSR_UI_INPUTMODE_AUTO, inst->qid);
            inst->uiSk1Event = cfm->handle;
            CsrUiUieCreateReqSend(inst->qid, CSR_UI_UIETYPE_EVENT);
        }
        else if (!inst->uiSk2Event)
        {
            CsrUiEventSetReqSend(cfm->handle, CSR_UI_INPUTMODE_BLOCK, inst->qid);
            inst->uiSk2Event = cfm->handle;
            CsrUiUieCreateReqSend(inst->qid, CSR_UI_UIETYPE_MENU);
        }
    }
    else if(cfm->elementType == CSR_UI_UIETYPE_MENU)
    {
        if(!inst->uiMainMenu)
        {
            CsrUiMenuSetReqSend(cfm->handle,
                                CsrUtf82Utf16String(CSR_BT_AMPWIFI_MAIN_ENTRY_NAME),
                                TEXT_SELECT_UCS2, NULL);
            inst->uiMainMenu = cfm->handle;

            CsrAppBacklogReqSend(TECH_BT, CsrUtf8StrDup(CSR_BT_AMPWIFI_MAIN_ENTRY_NAME),
                                 FALSE, "UI elements created");
        }
    }
}

static void csrBtAmpWifiAppHandleUiEventInd(CsrBtAmpWifiInst *inst,
                                            CsrUiEventInd *event)
{
    if(event->event == inst->uiBackEvent)
    {
        CsrUiUieHideReqSend(event->displayElement);
    }
    else if(event->displayElement == inst->uiMainMenu &&
            event->event == inst->uiSk1Event)
    {
        switch(event->key)
        {
            case CSR_BT_AMPWIFI_KEY_TOGGLE_WIFI_ONOFF:
                csrBtAmpWifiAppToggleWifiOnOff(inst);
                break;
                
            case CSR_BT_AMPWIFI_KEY_TOGGLE_COEX:
                csrBtAmpWifiAppToggleCoexistence(inst);
                break;
        }
    }
}

static void csrBtAmpWifiAppMenu(CsrBtAmpWifiInst *inst)
{
    CsrUiMenuRemoveallitemsReqSend(inst->uiMainMenu);

    CsrUiMenuAdditemReqSend(inst->uiMainMenu,
                            CSR_UI_LAST,
                            CSR_BT_AMPWIFI_KEY_TOGGLE_WIFI_ONOFF,
                            CSR_UI_ICON_NONE,
                            CsrUtf82Utf16String(CSR_BT_AMPWIFI_TOGGLE_WIFI_STRING),
                            NULL,
                            inst->uiSk1Event, inst->uiSk2Event, inst->uiBackEvent, 0);
    
    CsrUiMenuAdditemReqSend(inst->uiMainMenu,
                            CSR_UI_LAST,
                            CSR_BT_AMPWIFI_KEY_TOGGLE_COEX,
                            CSR_UI_ICON_NONE,
                            CsrUtf82Utf16String(CSR_BT_AMPWIFI_TOGGLE_COEX_STRING),
                            NULL,
                            inst->uiSk1Event, inst->uiSk2Event, inst->uiBackEvent, 0);
}

void csrBtAmpWifiAppHandleUiPrim(CsrBtAmpWifiInst *inst)
{
    switch(inst->msgType)
    {
        case CSR_UI_UIE_CREATE_CFM:
            csrBtAmpWifiAppHandleUiCreateCfm(inst, inst->msgData);
            break;

        case CSR_UI_EVENT_IND:
            csrBtAmpWifiAppHandleUiEventInd(inst, inst->msgData);
            break;

        default:
            CsrGeneralException(CSR_BT_AMPWIFI_MODULE_NAME,
                                inst->msgClass, inst->msgType,
                                "Unhandled CSR_APP event in CsrBtAmpWifiAppHandler");
            break;
    }
}

void csrBtAmpWifiHandleWifiSmePrim(CsrBtAmpWifiInst *inst)
{
    switch(inst->msgType)
    {
    case CSR_WIFI_SME_COEX_CONFIG_GET_CFM:
    {
        CsrWifiSmeCoexConfigGetCfm *prim;
        prim = (CsrWifiSmeCoexConfigGetCfm*)inst->msgData;

        /* This toggles coex */
        inst->coexEnable = !prim->coexConfig.coexEnableSchemeManagement;
        prim->coexConfig.coexEnableSchemeManagement = inst->coexEnable;
        CsrWifiSmeCoexConfigSetReqSend(inst->qid, prim->coexConfig);
        CsrAppBacklogReqSend(TECH_BT, CsrUtf8StrDup(CSR_BT_AMPWIFI_MAIN_ENTRY_NAME),
                               FALSE, "Sent WifiSmeCoexconfigSetReq, toggle %s",
                               (inst->coexEnable ? "ON" : "OFF"));
    }
    break;

    case CSR_WIFI_SME_COEX_CONFIG_SET_CFM:
    {
        CsrWifiSmeCoexConfigSetCfm *prim;
        prim = (CsrWifiSmeCoexConfigSetCfm*)inst->msgData;

        if(prim->status == CSR_RESULT_SUCCESS)
        {
            if(inst->coexEnable == FALSE)
            {
                CsrAppBacklogReqSend(TECH_BT, CsrUtf8StrDup(CSR_BT_AMPWIFI_MAIN_ENTRY_NAME),
                                     FALSE, "Co-exsistence has been disabled");
            }
            else
            {
                CsrAppBacklogReqSend(TECH_BT, CsrUtf8StrDup(CSR_BT_AMPWIFI_MAIN_ENTRY_NAME),
                             FALSE, "Co-exsistence has been enabled");
            }
        }
    }
    break;

    case CSR_WIFI_SME_ACTIVATE_CFM:
    {
        CsrWifiSmeActivateCfm *prim;

        prim = (CsrWifiSmeActivateCfm*)inst->msgData;
        CsrAppBacklogReqSend(TECH_BT, CsrUtf8StrDup(CSR_BT_AMPWIFI_MAIN_ENTRY_NAME),
                     FALSE, "Received CSR_WIFI_SME_ACTIVATE_CFM, code 0x%04x",
                     prim->status);

        csrBtAmpWifiAppToggleWifiOnOff(inst);
    }
    break;

    case CSR_WIFI_SME_WIFI_ON_CFM:
    {
        CsrWifiSmeWifiOnCfm *prim;
        prim = (CsrWifiSmeWifiOnCfm*)inst->msgData;

        if( prim->status == CSR_RESULT_SUCCESS )
        {
            csrBtAmpWifiAppSetWifiStatus(inst, TRUE);
        }

        if( inst->moveInitiatedWifiOn )
        {
            CsrBtAmpmPowerOnResSend(CSR_BT_AMP_AMP_CONTROLLER_TYPE_802_11,
                                    CSR_BT_AMPWIFI_LOCAL_ID,
                                    inst->wifiEnabled);
            
            inst->moveInitiatedWifiOn = FALSE;
        }
        
        CsrAppBacklogReqSend(TECH_BT, CsrUtf8StrDup(CSR_BT_AMPWIFI_MAIN_ENTRY_NAME),
                     FALSE, "Received CSR_WIFI_SME_WIFI_ON_CFM, code 0x%04x",
                     prim->status);
    }
    break;

    case CSR_WIFI_SME_WIFI_OFF_CFM:
    {
        CsrWifiSmeWifiOffCfm *prim;
        prim = (CsrWifiSmeWifiOffCfm*)inst->msgData;

        if( prim->status == CSR_RESULT_SUCCESS )
        {
            csrBtAmpWifiAppSetWifiStatus(inst, FALSE);
        }
        
        CsrAppBacklogReqSend(TECH_BT, CsrUtf8StrDup(CSR_BT_AMPWIFI_MAIN_ENTRY_NAME),
                     FALSE, "Received CSR_WIFI_SME_WIFI_OFF_CFM, code 0x%04x",
                     prim->status);
    }
    break;

    case CSR_WIFI_SME_WIFI_OFF_IND:
    {
        CsrWifiSmeWifiOffInd *prim;
        prim = (CsrWifiSmeWifiOffInd*)inst->msgData;

        csrBtAmpWifiAppSetWifiStatus(inst, FALSE);
                
        CsrAppBacklogReqSend(TECH_BT, CsrUtf8StrDup(CSR_BT_AMPWIFI_MAIN_ENTRY_NAME),
                     FALSE, "Received CSR_WIFI_SME_WIFI_OFF_IND, reason code 0x%04x",
                     prim->reason);
    }
    break;

    default:
    {
        CsrGeneralException(CSR_BT_AMPWIFI_MODULE_NAME,
                    inst->msgClass, inst->msgType,
                    "Unhandled CSR_WIFI_SME event in csrBtAmpWifiHandleWifiSmePrim");
    }
    break;
    }
}

void CsrBtAmpWifiAppHandler(void **gash)
{
    CsrBtAmpWifiInst *inst;
    inst = (CsrBtAmpWifiInst*)*gash;

    if(CsrSchedMessageGet(&(inst->msgClass), &(inst->msgData)))
    {
        inst->msgType = *(CsrPrim*)inst->msgData;
        switch (inst->msgClass)
        {
            case CSR_WIFI_SME_PRIM:
            {
                csrBtAmpWifiHandleWifiSmePrim(inst);
            }
            break;

            case CSR_APP_PRIM:
            {
                switch(inst->msgType)
                {
                    case CSR_APP_TAKE_CONTROL_IND:
                    {
                        csrBtAmpWifiAppMenu(inst);
                        CsrUiUieShowReqSend(inst->uiMainMenu, inst->qid,
                                            CSR_UI_INPUTMODE_AUTO, CSR_BT_AMPWIFI_PRIORITY);
                    }
                    break;

                    case CSR_APP_REGISTER_CFM:
                    {
                        /* Ignore */
                    }
                    break;

                    default:
                    {
                        CsrGeneralException(CSR_BT_AMPWIFI_MODULE_NAME,
                                            inst->msgClass, inst->msgType,
                                            "Unhandled CSR_APP event in CsrBtAmpWifiAppHandler");

                    }
                    break;
                }
            }
            break;

            case CSR_UI_PRIM:
            {
                csrBtAmpWifiAppHandleUiPrim(inst);
            }
            break;

            case CSR_BT_AMPM_PRIM:
            {
                if(inst->msgType == CSR_BT_AMPM_POWER_ON_IND)
                {
                    CsrBtAmpmPowerOnInd *prim = (CsrBtAmpmPowerOnInd *) inst->msgData;

                    if( (prim->ampType == CSR_BT_AMP_AMP_CONTROLLER_TYPE_802_11)
                        && (prim->localId == CSR_BT_AMPWIFI_LOCAL_ID))
                    {
                        csrBtAmpWifiAppToggleWifiOnOff(inst);
                        inst->moveInitiatedWifiOn = TRUE;
                    }
                    else
                    {
                        CsrBtAmpmPowerOnResSend(prim->ampType, prim->localId, FALSE);
                    }
                }
                else
                {
                    CsrGeneralException(CSR_BT_AMPWIFI_MODULE_NAME,
                                        inst->msgClass, inst->msgType,
                                        "Unhandled CSR_BT_AMPM event in CsrBtAmpWifiAppHandler");
                }
            }
            break;
            
            default:
            {
                CsrGeneralException(CSR_BT_AMPWIFI_MODULE_NAME,
                                    inst->msgClass, inst->msgType,
                                    "Unhandled event class in CsrBtAmpWifiAppHandler");
            }
            break;
        }

        if(inst->msgData != NULL)
        {
            CsrPmemFree(inst->msgData);
            inst->msgData = NULL;
            inst->msgClass = 0;
        }
    }
}

void CsrBtAmpWifiAppInit(void **gash)
{
    CsrBtAmpWifiInst *inst;
    inst = CsrPmemZalloc(sizeof(CsrBtAmpWifiInst));
    *gash = inst;

    inst->qid = CsrSchedTaskQueueGet();
    inst->uiMainMenu = CSR_UI_DEFAULTACTION;
    inst->uiBackEvent = CSR_UI_DEFAULTACTION;
    inst->uiSk1Event = CSR_UI_DEFAULTACTION;
    inst->uiSk2Event = CSR_UI_DEFAULTACTION;

    /* Kick-start UI element registration */
    CsrAppRegisterReqSend(inst->qid, TECH_BT, CsrUtf8StrDup(CSR_BT_AMPWIFI_MAIN_ENTRY_NAME));
    CsrUiUieCreateReqSend(inst->qid, CSR_UI_UIETYPE_EVENT);
    CsrAppBacklogReqSend(TECH_BT, CsrUtf8StrDup(CSR_BT_AMPWIFI_MAIN_ENTRY_NAME),
                         FALSE, "UI creation started");

    CsrWifiSmeActivateReqSend(inst->qid);
    CsrAppBacklogReqSend(TECH_BT, CsrUtf8StrDup(CSR_BT_AMPWIFI_MAIN_ENTRY_NAME),
                         FALSE, "Sent WifiActivateReq");

    /* this instance handles WiFi power on request */
    CsrBtAmpmRegisterPowerOnReqSend(inst->qid);
}

void CsrBtAmpWifiAppDeinit(void **gash)
{
    CsrBtAmpWifiInst *inst;
    CsrUint16 msgClass;
    void *msgData;
    inst = (CsrBtAmpWifiInst*)*gash;

    while(CsrSchedMessageGet(&msgClass, &msgData))
    {
        switch (msgClass)
        {
            case CSR_WIFI_SME_PRIM:
                CsrWifiSmeFreeUpstreamMessageContents(msgClass, msgData);
                break;

            default:
            break;
        }
        CsrPmemFree(msgData);
    }

    CsrPmemFree(inst);
    *gash = NULL;
}

void CsrBtAmpWiFiAppUsage(void)
{
  printf("--bt-wifi-mib <filename> - Name of the MIB to use\n");

}
