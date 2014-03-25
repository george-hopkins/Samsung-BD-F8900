/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"
#include <stdio.h>

#include "csr_bt_hfg_app_ui_sef.h"
#include "csr_bt_hfg_app_task.h"
#include "csr_bt_hfg_app_util.h"
#include "csr_ui_lib.h"
#include "csr_pmem.h"
#include "csr_bt_util.h"
#include "csr_exceptionhandler.h"
#include "csr_bt_hfg_lib.h"
#include "csr_bt_hfg_demo_app.h"
#include "csr_bt_hfg_menu_utils.h"
#include "csr_bt_hfg_app_ui_strings.h"

/* Static functions used by the CSR_UI upstream handler functions                 */
static void appUiKeyHandlerHideMics(void * instData,
                    CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* An key event function used to hide the showed UI. This function is used
     by many different APP UI's                                              */
    hfgInstance_t *inst = instData;
    CsrBtHfgCsrUiType    * csrUiVar  = &(inst->csrUiVar);

    CsrUiUieHideReqSend(displayHandle);
    /*if the hfg main menu is hidden, we are going out of HFG menu*/
     if(displayHandle == csrUiVar->displayesHandlers[CSR_BT_HFG_MAIN_MENU_UI].displayHandle)
        csrUiVar->inHfgMenu = FALSE;

    CSR_UNUSED(eventHandle);
    CSR_UNUSED(instData);
    CSR_UNUSED(key);
}

static void appUiKeyHandlerGetInputDialogMisc(void * instData,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from UI's with input dialogs */
    CSR_UNUSED(instData);
    CSR_UNUSED(eventHandle);
    CSR_UNUSED(key);

    CsrUiInputdialogGetReqSend(CsrSchedTaskQueueGet(), displayHandle);
}

/* Handle CSR_BT_HFG_AT_MODE_CFG_CMDS_UI key events releated to this UI   */
static void appUiKeyHandlerATCfgMenu(void * instData,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from the CSR_BT_HFG_AT_MODE_CFG_CMDS_UI */
    hfgInstance_t* inst = instData;
    CsrCharString buf[60];
    CSR_UNUSED(eventHandle);
#ifndef EXCLUDE_CSR_BT_HFG_MODULE_OPTIONAL
    if(inst->parserMode != CSR_BT_HFG_AT_MODE_USER_CONFIG)
    {
            CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);

            sprintf(buf, "AT Commands Configuration is possible only in USER_CONFIG!!");
            CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_FAILED_UCS2,
                                CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
    }
    else
    {
            handleAtCmdChoiceMenu(inst, (CsrUint8)key);

    }
#else
    CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);

    sprintf(buf, "AT Commands Configuration is not enabled in build!!");
    CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_FAILED_UCS2,
                      CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
#endif
    /* Unlock the CSR_BT_HFG_AT_MODE_CFG_CMDS_UI                               */
    CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);
}

static void appCreateHfgATModeCfgUi(hfgInstance_t * inst)
{ /* This function Creates the CSR_BT_HFG_AT_MODE_CFG_CMDS_UI                              */

    CsrBtHfgCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_MENU)
    {
        csrUiVar->eventState                                              = CSR_BT_HFG_CREATE_SK1_EVENT;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerATCfgMenu;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Set the menu on the CSR_BT_HFG_AT_MODE_CFG_CMDS_UI                               */
        CsrUiMenuSetReqSend(prim->handle, TEXT_HFG_CONFIG_AT_COMMANDS_UCS2, TEXT_SELECT_UCS2, NULL);

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    {
        if (csrUiVar->eventState == CSR_BT_HFG_CREATE_SK1_EVENT)
        { /* Save the event handle as SK1, and set input mode to blocked        */

            csrUiVar->eventState                                          = CSR_BT_HFG_CREATE_BACK_EVENT;
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Create a BACK event                                              */
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else
        { /* CSR_BT_HFG_CREATE_BACK_EVENT                                           */

            CsrUieHandle displayHandle  = csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle;
            CsrUieHandle sk1Handle      = csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle;

            /* Save the event handle as Back, and set input mode to blocked     */
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Updated the CSR_BT_HFG_AT_MODE_CFG_CMDS_UI to include all the supported
               client applications                                              */
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_HFG_FWD_UNHANDLED_CMDS_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_AT_FWD_UNHANDLED_CMDS_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_ATA_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_AT_ATA_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_VTS_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_AT_VTS_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_BINP_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_AT_BINP_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_COPS_Q_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_AT_COPS_Q_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_COPS_E_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_AT_COPS_E_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_BLDN_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_AT_BLDN_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_BIA_E_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_AT_BIA_E_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_AT_CMD_HANDLING_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_AT_CMD_HANDLING_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            /* Create  CSR_BT_HFG_AT_CMD_MODE_UI                                        */
            csrUiVar->uiIndex = CSR_BT_HFG_AT_CMD_MODE_UI;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);
        }
    }
}


/* Handle CSR_BT_HFG_AT_CMD_MODE_UI key events releated to this UI   */
static void appUiKeyHandlerATCmdMenu(void * instData,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from the CSR_BT_HFG_AT_CMD_MODE_UI */
    hfgInstance_t* inst = instData;
    CsrCharString buf[40];

    CSR_UNUSED(eventHandle);

    switch (key)
    {
        case CSR_BT_HFG_AT_MODE_FULL_KEY:
        {
            inst->parserMode = CSR_BT_HFG_AT_MODE_FULL;
            sprintf(buf, "AT Command Mode Full Set!!");
            break;
        }
        case CSR_BT_HFG_AT_MODE_SEMI_KEY:
        {
            inst->parserMode = CSR_BT_HFG_AT_MODE_SEMI;
            sprintf(buf, "AT Command Mode Semi Set!!");
            break;
        }
    case CSR_BT_HFG_AT_MODE_TRANPARENT_KEY:
        {
            inst->parserMode = CSR_BT_HFG_AT_MODE_TRANSPARENT;
            sprintf(buf, "AT Command Mode Transparent Set!!");
            break;
        }
        case CSR_BT_HFG_AT_MODE_USER_CFG_KEY:
        {
            inst->parserMode = CSR_BT_HFG_AT_MODE_USER_CONFIG;
            sprintf(buf, "AT Command Mode User Config Set!!");
            break;
        }
        default :
        {
            CsrGeneralException("CSR_BT_HFG", 0, 0,
                           "Unexpected menu key is received");
            break;
        }
    }
    CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                            CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
    /* Show the CSR_BT_HFG_DEFAULT_INFO_UI on the display                   */
    CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);

    /* Unlock the CSR_BT_HFG_AT_CMD_MODE_UI                               */
    CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);

}


static void appCreateHfgATCmdModeUi(hfgInstance_t * inst)
{ /* This function Creates the CSR_BT_HFG_AT_CMD_MODE_UI                              */

    CsrBtHfgCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_MENU)
    {
        csrUiVar->eventState                                              = CSR_BT_HFG_CREATE_SK1_EVENT;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerATCmdMenu;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Set the menu on the CSR_BT_HFG_AT_CMD_MODE_UI                               */
        CsrUiMenuSetReqSend(prim->handle, TEXT_HFG_AT_COMMANDS_UCS2, TEXT_SELECT_UCS2, NULL);

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    {
        if (csrUiVar->eventState == CSR_BT_HFG_CREATE_SK1_EVENT)
        { /* Save the event handle as SK1, and set input mode to blocked        */

            csrUiVar->eventState                                          = CSR_BT_HFG_CREATE_BACK_EVENT;
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Create a BACK event                                              */
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else
        { /* CSR_BT_HFG_CREATE_BACK_EVENT                                           */

            CsrUieHandle displayHandle  = csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle;
            CsrUieHandle sk1Handle      = csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle;

            /* Save the event handle as Back, and set input mode to blocked     */
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Updated the CSR_BT_HFG_AT_MODE_CFG_CMDS_UI to include all the supported
               client applications                                              */
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_HFG_AT_MODE_FULL_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_AT_MODE_FULL_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_AT_MODE_SEMI_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_AT_AT_MODE_SEMI_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_AT_MODE_TRANPARENT_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_AT_AT_MODE_TRANSPARENT_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_AT_MODE_USER_CFG_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_AT_AT_MODE_USER_CONFIG_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_AT_CFG_AT_CMDS_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_CONFIG_AT_COMMANDS_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            /* Create  CSR_BT_HFG_CHANGE_SUP_FEAT_UI                                        */
            csrUiVar->uiIndex = CSR_BT_HFG_CHANGE_SUP_FEAT_UI;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);
        }
    }
}

/* Handle CSR_BT_HFG_CHANGE_SUP_FEAT_UI key events releated to this UI   */
static void appUiKeyHandlerChangeFeatSupMenu(void * instData,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from the CSR_BT_HFG_CHANGE_SUP_FEAT_UI */
    hfgInstance_t* inst = instData;
    CsrCharString buf[60];

    CSR_UNUSED(eventHandle);
    switch (key)
    {
        case CSR_BT_HFG_CHANGE_SUP_FEAT_BM_KEY:
        {
            sprintf((char*)buf, "CSR-2-CSR supported features/indicators:0x%08x\n", inst->hfgSupportedFeatures);

            CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                                    CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
            CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
            /* Unlock the CSR_BT_HFG_CHANGE_SUP_FEAT_UI                               */
            CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);

            return;
        }
        case CSR_BT_HFG_3WAY_CAL_KEY:
        {
            inst->hfgSupportedFeatures ^= CSR_BT_HFG_SUPPORT_THREE_WAY_CALLING;
            break;
        }
        case CSR_BT_HFG_ECHO_NOISE_REDUCTION_KEY:
        {
            inst->hfgSupportedFeatures ^= CSR_BT_HFG_SUPPORT_EC_NR_FUNCTION;
            break;
        }
        case CSR_BT_HFG_VOICE_RECOG_KEY:
        {
                    /* Voice recognition */
                    inst->hfgSupportedFeatures ^= CSR_BT_HFG_SUPPORT_VOICE_RECOGNITION;
                    break;
        }
        case CSR_BT_HFG_INBAND_RING_KEY:
        {
                    /* Inband ringing */
                    inst->hfgSupportedFeatures ^= CSR_BT_HFG_SUPPORT_INBAND_RINGING;
                    break;
        }
        case CSR_BT_HFG_ATTACH_NUMBER_VOICE_KEY:
        {
                    /* Attach number to voice tag */
                    inst->hfgSupportedFeatures ^= CSR_BT_HFG_SUPPORT_ATTACH_NUMBER_TO_VOICE_TAG;
                    break;
        }
        case CSR_BT_HFG_REJ_CALL_KEY:
        {
                    /* Reject call */
                    inst->hfgSupportedFeatures ^= CSR_BT_HFG_SUPPORT_ABILITY_TO_REJECT_CALL;
                    break;
        }
        case CSR_BT_HFG_ENH_CALL_STATUS_KEY:
        {
                    /* Enhanced Call status */
                    inst->hfgSupportedFeatures ^= CSR_BT_HFG_SUPPORT_ENHANCED_CALL_STATUS;
                    break;
        }
        case CSR_BT_HFG_ENH_CALL_CONTROL_KEY:
        {
                    /* Enhanced Call Control */
                    inst->hfgSupportedFeatures ^= CSR_BT_HFG_SUPPORT_ENHANCED_CALL_CONTROL;
                    break;
        }
        case CSR_BT_HFG_EXT_ERROR_CODES_KEY:
        {
                    /* Extended error codes */
                    inst->hfgSupportedFeatures ^= CSR_BT_HFG_SUPPORT_EXTENDED_ERROR_CODES;
                    break;
        }

        default :
        {
            CsrGeneralException("CSR_BT_HFG", 0, 0,
                           "Unexpected menu key is received");
            break;
        }
    }
    sprintf((char*)buf, "Feature Support Toggled!!");
    CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                        CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
    CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
            /* Unlock the CSR_BT_HFG_CHANGE_SUP_FEAT_UI                               */
        CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);

}
static void appCreateChangeSupportFeatUi(hfgInstance_t * inst)
{ /* This function Creates the CSR_BT_HFG_CHANGE_SUP_FEAT_UI                              */

    CsrBtHfgCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_MENU)
    {
        csrUiVar->eventState                                              = CSR_BT_HFG_CREATE_SK1_EVENT;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerChangeFeatSupMenu;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Set the menu on the CSR_BT_HFG_CHANGE_SUP_FEAT_UI                               */
        CsrUiMenuSetReqSend(prim->handle, TEXT_HFG_CHANGE_SUP_FEAT_UCS2, TEXT_CHANGE_UCS2, NULL);

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    {
        if (csrUiVar->eventState == CSR_BT_HFG_CREATE_SK1_EVENT)
        { /* Save the event handle as SK1, and set input mode to blocked        */

            csrUiVar->eventState                                          = CSR_BT_HFG_CREATE_BACK_EVENT;
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Create a BACK event                                              */
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else
        { /* CSR_BT_HFG_CREATE_BACK_EVENT                                           */

            CsrUieHandle displayHandle  = csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle;
            CsrUieHandle sk1Handle      = csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle;

            /* Save the event handle as Back, and set input mode to blocked     */
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_HFG_CHANGE_SUP_FEAT_BM_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_CHANGE_SUP_FEAT_BM_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_3WAY_CAL_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_3WAY_CAL_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_ECHO_NOISE_REDUCTION_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_ECHO_NOISE_REDUCTION_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_VOICE_RECOG_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_VOICE_RECOG_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_INBAND_RING_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_INBAND_RING_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_ATTACH_NUMBER_VOICE_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_ATTACH_NUMBER_VOICE_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_REJ_CALL_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_REJ_CALL_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_ENH_CALL_STATUS_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_ENH_CALL_STATUS_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_ENH_CALL_CONTROL_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_ENH_CALL_CONTROL_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_EXT_ERROR_CODES_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_EXT_ERROR_CODES_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            csrUiVar->uiIndex = CSR_BT_HFG_SUP_FEAT_UI;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);
        }
    }
}


/* Handle CSR_BT_HFG_SUP_FEAT_UI key events releated to this UI   */
static void appUiKeyHandlerSupFeatMenu(void * instData,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from the CSR_BT_HFG_SUP_FEAT_UI */
    hfgInstance_t* inst = instData;
    CsrCharString buf[80];

    CSR_UNUSED(eventHandle);

    switch (key)
    {
        case CSR_BT_HFG_DISP_SUP_FEAT_KEY:
        {
            displayFeatures(inst);
            break;
        }
        case CSR_BT_HFG_CHANGE_SUP_FEAT_KEY:
        {
            if(inst->state == stDeactivated)
            {
                CsrBtHfgShowUi(inst, CSR_BT_HFG_CHANGE_SUP_FEAT_UI, CSR_UI_INPUTMODE_AUTO, 1);
            }
            else
            {
                sprintf((char*)buf, "HFG has to be Deactivated to change the supported features list!");
                CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_FAILED_UCS2,
                                                                        CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
                CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
            }
            break;
        }
        default :
        {
            CsrGeneralException("CSR_BT_HFG", 0, 0,
            "Unexpected menu key is received");
            break;
        }
    }
        /* Unlock the CSR_BT_HFG_SUP_FEAT_UI                               */
        CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);
}
static void appCreateSupportFeatUi(hfgInstance_t * inst)
{ /* This function Creates the CSR_BT_HFG_SUP_FEAT_UI                              */

    CsrBtHfgCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_MENU)
    {
        csrUiVar->eventState                                              = CSR_BT_HFG_CREATE_SK1_EVENT;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerSupFeatMenu;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Set the menu on the CSR_BT_HFG_CHANGE_SUP_FEAT_UI                               */
        CsrUiMenuSetReqSend(prim->handle, TEXT_HFG_SUP_FEAT_UCS2, TEXT_SELECT_UCS2, NULL);

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    {
        if (csrUiVar->eventState == CSR_BT_HFG_CREATE_SK1_EVENT)
        { /* Save the event handle as SK1, and set input mode to blocked        */

            csrUiVar->eventState                                          = CSR_BT_HFG_CREATE_BACK_EVENT;
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Create a BACK event                                              */
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else
        { /* CSR_BT_HFG_CREATE_BACK_EVENT                                           */

            CsrUieHandle displayHandle  = csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle;
            CsrUieHandle sk1Handle      = csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle;

            /* Save the event handle as Back, and set input mode to blocked     */
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_HFG_DISP_SUP_FEAT_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_DISP_SUP_FEAT_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_CHANGE_SUP_FEAT_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_CHANGE_SUP_FEAT_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            csrUiVar->uiIndex = CSR_BT_HFG_ALREADY_CON_DIALOG_UI;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_DIALOG);

        }
    }
}


/* Handle CSR_BT_HFG_ALREADY_CON_DIALOG_UI key events releated to this UI   */
static void appUiKeyHandlerAlreadyConMenu(void * instData,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from the CSR_BT_HFG_SUCC_CON_DIALOG_UI */
    hfgInstance_t* inst = instData;
    CsrBtHfgCsrUiType    * csrUiVar  = &(inst->csrUiVar);

    CSR_UNUSED(displayHandle);
    CSR_UNUSED(eventHandle);
    CSR_UNUSED(key);

    updateMainMenu(inst);
    CsrUiUieHideReqSend(csrUiVar->displayesHandlers[CSR_BT_HFG_ALREADY_CON_DIALOG_UI].displayHandle);
}

static void appCreateAlreadyConDialogUi(hfgInstance_t * inst)
{ /* This function Creates the CSR_BT_HFG_ALREADY_CON_DIALOG_UI                               */

    CsrBtHfgCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_DIALOG)
    {
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerAlreadyConMenu;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerAlreadyConMenu;

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    { /* Save the event handle as SK1/BACK, and set input mode to blocked       */
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle      = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle     = prim->handle;

        CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

        /* Create  CSR_BT_HFG_SUCC_CON_DIALOG_UI                                        */
        csrUiVar->uiIndex = CSR_BT_HFG_SUCC_CON_DIALOG_UI;
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_DIALOG);
    }
}

/* Handle CSR_BT_HFG_SUCC_CON_DIALOG_UI key events releated to this UI   */
static void appUiKeyHandlerSuccConMenu(void * instData,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from the CSR_BT_HFG_SUCC_CON_DIALOG_UI */
    hfgInstance_t* inst = instData;
    CsrBtHfgCsrUiType    * csrUiVar  = &(inst->csrUiVar);

    CSR_UNUSED(displayHandle);
    CSR_UNUSED(eventHandle);
    CSR_UNUSED(key);

    updateMainMenu(inst);
    CsrBtHfgShowUi(inst, CSR_BT_HFG_MAIN_MENU_UI, CSR_UI_INPUTMODE_AUTO, 1);
    inst->csrUiVar.inHfgMenu = TRUE;
    CsrUiUieHideReqSend(csrUiVar->displayesHandlers[CSR_BT_HFG_SUCC_CON_DIALOG_UI].displayHandle);
}

static void appCreateSuccConDialogUi(hfgInstance_t * inst)
{ /* This function Creates the CSR_BT_HFG_SUCC_CON_DIALOG_UI                               */

    CsrBtHfgCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_DIALOG)
    {
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerSuccConMenu;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerSuccConMenu;

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    { /* Save the event handle as SK1/BACK, and set input mode to blocked       */
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle      = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle     = prim->handle;

        CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

        /* Create  CSR_BT_HFG_CANCEL_CON_DIALOG_UI                                        */
        csrUiVar->uiIndex = CSR_BT_HFG_CANCEL_CON_DIALOG_UI;
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_DIALOG);
    }
}


/* Handle CSR_BT_HFG_CANCEL_CON_DIALOG_UI key events releated to this UI   */
static void appUiKeyHandlerCancelConMenu(void * instData,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from the CSR_BT_HFG_CANCEL_CON_DIALOG_UI */
    hfgInstance_t* inst = instData;
    CsrBtHfgCsrUiType    * csrUiVar  = &(inst->csrUiVar);

    CSR_UNUSED(displayHandle);
    CSR_UNUSED(eventHandle);
    CSR_UNUSED(key);

    startCancelConnect(inst);

    CsrUiUieHideReqSend(csrUiVar->displayesHandlers[CSR_BT_HFG_CANCEL_CON_DIALOG_UI].displayHandle);
    inst->dialogShow = FALSE;
}

static void appCreateCancelConDialogUi(hfgInstance_t * inst)
{ /* This function Creates the CSR_BT_HFG_CANCEL_CON_DIALOG_UI                               */

    CsrBtHfgCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_DIALOG)
    {
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerCancelConMenu;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerCancelConMenu;

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    { /* Save the event handle as SK1/BACK, and set input mode to blocked       */
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle      = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle     = prim->handle;

        CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

        csrUiVar->uiIndex = CSR_BT_HFG_DISC_DIALOG_UI;
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_DIALOG);
    }
}

/* Handle CSR_BT_HFG_DISC_DIALOG_UI key events releated to this UI   */
static void appUiKeyHandlerDiscMenu(void * instData,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from the CSR_BT_HFG_DISC_DIALOG_UI */
    hfgInstance_t* inst = instData;
    CsrBtHfgCsrUiType    * csrUiVar  = &(inst->csrUiVar);

    CSR_UNUSED(displayHandle);
    CSR_UNUSED(eventHandle);
    CSR_UNUSED(key);

    CsrUiUieHideReqSend(csrUiVar->displayesHandlers[CSR_BT_HFG_DISC_DIALOG_UI].displayHandle);

    if(inst->dialogShow)
    {
        updateMainMenu(inst);
        inst->dialogShow = FALSE;
    }
    else
    {
        if(csrUiVar->inHfgMenu)
        {
            csrUiVar->goToMainmenu = TRUE;
            CsrUiDisplayGethandleReqSend(CsrSchedTaskQueueGet());
        }
    }
}

static void appCreateDiscDialogUi(hfgInstance_t * inst)
{ /* This function Creates the CSR_BT_HFG_DISC_DIALOG_UI                               */

    CsrBtHfgCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_DIALOG)
    {
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerDiscMenu;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerDiscMenu;

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    { /* Save the event handle as SK1/BACK, and set input mode to blocked       */
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle      = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle     = prim->handle;

        CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

        csrUiVar->uiIndex = CSR_BT_HFG_CALL_HANDLING_UI;
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);
    }
}



/* Handle CSR_BT_HFG_CALL_HANDLING_UI key events releated to this UI   */
static void appUiKeyHandlerCallHandlingMenu(void * instData,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from the CSR_BT_HFG_CALL_HANDLING_UI */
    hfgInstance_t* inst = instData;
    CsrCharString buf[50];

    CSR_UNUSED(eventHandle);

    switch (key)
    {
        case CSR_BT_HFG_TOGGLE_AUDIO_KEY:
        {
            toggleAudio(inst);
            break;
        }
        case CSR_BT_HFG_PLACE_INCOMING_1_KEY:
        {
            startRinging(inst, RING_NUMBER);
            break;
        }
    case CSR_BT_HFG_PLACE_INCOMING_2_KEY:
        {
            startRinging(inst, RING_NUMBER_2);
            break;
        }
        case CSR_BT_HFG_ANSWER_INCOMING_KEY:
        {
            startAnswer(inst);
            break;
        }
        case CSR_BT_HFG_REJECT_INCOMING_KEY:
        {
            startReject(inst);
            break;
        }
        case CSR_BT_HFG_PLACE_INCOMING_ON_HOLD_KEY:
        {
            startPutOnHold(inst);
            break;
        }
    case CSR_BT_HFG_ANSWER_CALL_ON_HOLD_KEY:
        {
            startAnswerCallOnHold(inst);
            break;
        }
    case CSR_BT_HFG_TERM_CALL_ON_HOLD_R_KEY:
        {
            startRejectCallOnHold(inst);
            break;
        }
        case CSR_BT_HFG_TERMINATE_HELD_CALL_KEY:
        {
            startRejectCallOnHold(inst);
            break;
        }
        case CSR_BT_HFG_SHOW_LAST_DIALLED_KEY:
        {
            if (inst->redialNumber[0] == 0)
            {
                sprintf((char*)buf, "No number dialed yet\n");
                CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_FAILED_UCS2,
                                                                        CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
            }
            else
            {
                sprintf((char*)buf, "Last dialed number: %s\n",(char*)(inst->redialNumber));
                CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                                        CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
            }
            CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
            break;
        }
        case CSR_BT_HFG_CLEAR_LAST_DIALLED_KEY:
        {
            CsrMemSet(inst->redialNumber,'\0',MAX_TEXT_LENGTH);
            sprintf((char*)buf, "Last dialed number cleared\n");
            CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                                    CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
            CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
            break;
        }
    case CSR_BT_HFG_SET_LAST_DIALLED_KEY:
        {
            /* Set the menu on the CSR_BT_HFG_LDN_INPUT_DIALG_UI                               */
            CsrBtHfgSetInputDialog(inst, CSR_BT_HFG_LDN_INPUT_DIALG_UI,
                CONVERT_TEXT_STRING_2_UCS2("Last Dial Number Setting"), CONVERT_TEXT_STRING_2_UCS2("Enter the Last Dialled Numeber:"),
                CSR_UI_ICON_KEY, NULL, CSR_BT_LDN_MAX,
                CSR_UI_KEYMAP_NUMERIC,TEXT_OK_UCS2, NULL);
            CsrBtHfgShowUi(inst, CSR_BT_HFG_LDN_INPUT_DIALG_UI, CSR_UI_INPUTMODE_AUTO, 1);
            break;
        }
        case CSR_BT_HFG_TOGGLE_CLCC_SUPPORT_KEY:
        {
            inst->callListCmdSupport = !inst->callListCmdSupport;
            sprintf((char*)buf, "CLCC support is toggled!!");
            CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                                    CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
            CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);

            break;
        }
        case CSR_BT_HFG_TOG_AUTO_AUD_TX_SLC_KEY:
        {
            inst->autoAudioTransfer = !inst->autoAudioTransfer;
            sprintf((char*)buf, "Auto Adio Transfer Option is toggled!!");
            CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                                    CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
            CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
            break;
        }
        case CSR_BT_HFG_PLACE_OUTGOING_KEY:
        {
            startOutgoingCall(inst, RING_NUMBER);
            break;
        }
        default : /*                                         */
        {
        CsrGeneralException("CSR_BT_HFG", 0, 0,
                           "Unexpected menu key is received");
            break;
        }
    }
    /* Unlock the CSR_BT_HFG_CALL_HANDLING_UI                               */
    CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);
}
static void appCreateHfgCallHandlingMenuUi(hfgInstance_t * inst)
{ /* This function Creates the CSR_BT_HFG_CALL_HANDLING_UI                              */

    CsrBtHfgCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_MENU)
    {
        csrUiVar->eventState                                              = CSR_BT_HFG_CREATE_SK1_EVENT;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerCallHandlingMenu;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Set the menu on the CSR_BT_HFG_AT_CMD_MODE_UI                               */
        CsrUiMenuSetReqSend(prim->handle, TEXT_HFG_CALL_HANDLING_UCS2, TEXT_SELECT_UCS2, NULL);

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    {
        if (csrUiVar->eventState == CSR_BT_HFG_CREATE_SK1_EVENT)
        { /* Save the event handle as SK1, and set input mode to blocked        */

            csrUiVar->eventState                                          = CSR_BT_HFG_CREATE_BACK_EVENT;
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Create a BACK event                                              */
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else
        { /* CSR_BT_HFG_CREATE_BACK_EVENT                                           */

            CsrUieHandle displayHandle  = csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle;
            CsrUieHandle sk1Handle      = csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle;

            /* Save the event handle as Back, and set input mode to blocked     */
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Updated the CSR_BT_HFG_AT_MODE_CFG_CMDS_UI to include all the supported
               client applications                                              */
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_HFG_TOGGLE_AUDIO_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_CH_TOGGLE_TRANFER_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_PLACE_INCOMING_1_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_CH_PLACE_INCOMING1_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_PLACE_INCOMING_2_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_CH_PLACE_INCOMING2_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_ANSWER_INCOMING_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_CH_ANSWER_INCOMING_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_HFG_REJECT_INCOMING_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_CH_REJECT_INCOMING_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_PLACE_INCOMING_ON_HOLD_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_CH_PLACE_INCOMING_HOLD_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_ANSWER_CALL_ON_HOLD_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_CH_ANSWER_HOLD_CALL_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_TERM_CALL_ON_HOLD_R_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_CH_TERMINATE_HELD_FROM_REMOTE_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_HFG_TERMINATE_HELD_CALL_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_CH_TERMINATE_HELD_CALL_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_SHOW_LAST_DIALLED_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_CH_SHOW_LAST_DIALLED_NUMBER_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_CLEAR_LAST_DIALLED_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_CH_CLEAR_LAST_DIALLED_NUMBER_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_SET_LAST_DIALLED_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_CH_SET_LAST_DIALLED_NUMBER_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_TOGGLE_CLCC_SUPPORT_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_CH_CLCC_SUPPORT_TOGGLE_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_TOG_AUTO_AUD_TX_SLC_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_CH_TOGGLE_AUTO_AUDIO_TX_AFTER_SLC_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_PLACE_OUTGOING_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_CH_PLACE_OUTGOING_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            /* Create  CSR_BT_HFG_CALL_LIST_CHNGE_PARM_UI                                        */
            csrUiVar->uiIndex = CSR_BT_HFG_CALL_LIST_CHNGE_PARM_UI;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);
        }
    }
}

/* Display call list */
void createDisplayCalllist(hfgInstance_t *inst)
{
    CsrUint16 i, pos;
    CsrCharString buf[50];
    CsrCharString subLabelBuf[MAX_TEXT_LENGTH];
    CsrBtHfgCsrUiType    * csrUiVar  = &(inst->csrUiVar);

    CsrUiMenuRemoveallitemsReqSend(csrUiVar->displayesHandlers[CSR_BT_HFG_CALL_LIST_MENU_UI].displayHandle);
    inst->isAnyMenuItem = FALSE;
    for(i=0; i<MAX_CALLS; i++)
    {
        if(inst->calls[i].isSet)
        {
            sprintf((char*)buf,"%s%smultiparty:%s",
                   callGetStringStatus(inst, (CsrUint8)i),
                   (inst->calls[i].dir == MOBILE_TERMINATED ? "(inc)," : "(out),"),
                   (inst->calls[i].mpty == MULTIPARTY_CALL ? "yes" : "no"));
            sprintf((char*)subLabelBuf,"%s",
                    (char*)inst->calls[i].number);
        pos = (inst->isAnyMenuItem == FALSE) ? CSR_UI_FIRST : CSR_UI_LAST;
        CsrUiMenuAdditemReqSend(csrUiVar->displayesHandlers[CSR_BT_HFG_CALL_LIST_MENU_UI].displayHandle,
                        pos, i, CSR_UI_ICON_NONE, 
                        CONVERT_TEXT_STRING_2_UCS2(buf),
                        CONVERT_TEXT_STRING_2_UCS2(subLabelBuf),
                        csrUiVar->displayesHandlers[CSR_BT_HFG_CALL_LIST_MENU_UI].sk1EventHandle,
                        CSR_UI_DEFAULTACTION, csrUiVar->displayesHandlers[CSR_BT_HFG_CALL_LIST_MENU_UI].backEventHandle, CSR_UI_DEFAULTACTION);
        inst->isAnyMenuItem = TRUE;
        }
    }
}

/* Handle CSR_BT_HFG_CALL_LIST_CHNGE_PARM_UI key events releated to this UI   */
static void appUiKeyHandlerChangeCallParamMenu(void * instData,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from the CSR_BT_HFG_CALL_LIST_CHNGE_PARM_UI */
    hfgInstance_t* inst = instData;
    CsrBtHfgCsrUiType    * csrUiVar  = &(inst->csrUiVar);

    CSR_UNUSED(eventHandle);

    switch (key)
    {
        case CSR_BT_HFG_CL_PLACE_INCOMING_ON_HOLD_KEY:
            startPutOnHold(inst);
            break;
        case CSR_BT_HFG_CL_DISP_CALL_INFO_KEY:
            displayCallOptions(inst);
            break;
        case CSR_BT_HFG_CL_CHANGE_ACTIVE_KEY:
        case CSR_BT_HFG_CL_DROP_CALL_KEY:
        case CSR_BT_HFG_CL_TOGGLE_DIR_KEY:
        case CSR_BT_HFG_CL_TRAVERSE_STATUS_KEY:
        case CSR_BT_HFG_CL_TOGGLE_MULTIPARTY_KEY:
            handleCallAlterMenu(inst, (CsrUint8)key);
            break;
        default : /*                                         */
        {
        CsrGeneralException("CSR_BT_HFG", 0, 0,
                           "Unexpected menu key is received");
            break;
        }
    }
    if(key != CSR_BT_HFG_CL_DROP_CALL_KEY)  /* Unlock the CSR_BT_HFG_CALL_LIST_CHNGE_PARM_UI  */
    {
        CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);
    }
    else
    {
    createDisplayCalllist(inst);
    if(!inst->isAnyMenuItem)
    {
        CsrUiUieHideReqSend(csrUiVar->displayesHandlers[CSR_BT_HFG_CALL_LIST_MENU_UI].displayHandle);
    }
    }

}


/* Static functions to hide the call parameter change menu and update the Call List                 */
static void appUiKeyHandlerHideCallChangeUpdateCalllist(void * instData,
                    CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* An key event function used to hide the showed UI. This function is used
     by many different APP UI's
     */
    hfgInstance_t* inst = instData;
    CsrBtHfgCsrUiType    * csrUiVar  = &(inst->csrUiVar);

    CsrUiUieHideReqSend(displayHandle);
    createDisplayCalllist(inst);
    if(!inst->isAnyMenuItem)
    {
        CsrUiUieHideReqSend(csrUiVar->displayesHandlers[CSR_BT_HFG_CALL_LIST_MENU_UI].displayHandle);
    }
    CSR_UNUSED(eventHandle);
    CSR_UNUSED(key);
}


static void appCreateHfgChangeCallParamUi(hfgInstance_t * inst)
{ /* This function Creates the CSR_BT_HFG_CALL_LIST_CHNGE_PARM_UI                              */

    CsrBtHfgCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_MENU)
    {
        csrUiVar->eventState                                              = CSR_BT_HFG_CREATE_SK1_EVENT;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerChangeCallParamMenu;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideCallChangeUpdateCalllist;

        /* Set the menu on the CSR_BT_HFG_CALL_LIST_CHNGE_PARM_UI                               */
        CsrUiMenuSetReqSend(prim->handle, TEXT_HFG_CL_ALTER_UCS2, TEXT_SELECT_UCS2, NULL);

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    {
        if (csrUiVar->eventState == CSR_BT_HFG_CREATE_SK1_EVENT)
        { /* Save the event handle as SK1, and set input mode to blocked        */

            csrUiVar->eventState                                          = CSR_BT_HFG_CREATE_BACK_EVENT;
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Create a BACK event                                              */
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else
        { /* CSR_BT_HFG_CREATE_BACK_EVENT                                           */

            CsrUieHandle displayHandle  = csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle;
            CsrUieHandle sk1Handle      = csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle;

            /* Save the event handle as Back, and set input mode to blocked     */
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Updated the CSR_BT_HFG_CALL_LIST_CHNGE_PARM_UI to include all the supported
               client applications                                              */
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_HFG_CL_DISP_CALL_INFO_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_CL_ALTER_INDEX_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_CL_CHANGE_ACTIVE_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_CL_CHANGE_ACTIVE_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_CL_DROP_CALL_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_CL_DROP_CALL_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_CL_TOGGLE_DIR_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_CL_TOGGLE_DIR_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_HFG_CL_TRAVERSE_STATUS_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_CL_TRAVERSE_STATUS_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_CL_TOGGLE_MULTIPARTY_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_CL_TOGGLE_MULTIPARTY_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_CL_PLACE_INCOMING_ON_HOLD_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_CL_PUT_INCOMING_CALL_ON_HOLD_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            
            /* Create  CSR_BT_HFG_CALL_LIST_MENU_UI                                        */
            csrUiVar->uiIndex = CSR_BT_HFG_CALL_LIST_MENU_UI;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);
        }
    }
}



/* Handle CSR_BT_HFG_CALL_LIST_MENU_UI key events releated to this UI   */
static void appUiKeyHandlerCallListMenu(void * instData,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from the CSR_BT_HFG_CALL_LIST_MENU_UI */
    hfgInstance_t* inst = instData;
    CSR_UNUSED(eventHandle);

    switch (key)
    {
        default : /*Just store the call index and call the call parameters change menu*/
        {
            if(key >= MAX_CALLS)
            {
                CsrGeneralException("CSR_BT_HFG", 0, 0,
                                   "Unexpected menu key is received");
            }
            else
            {
                inst->callTarget = (CsrUint8)key;
                CsrBtHfgShowUi(inst, CSR_BT_HFG_CALL_LIST_CHNGE_PARM_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
            }
            break;
        }
    }
    /* Unlock the CSR_BT_HFG_CALL_LIST_MENU_UI                               */
    CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);
}
static void appCreateHfgCallListMenuUi(hfgInstance_t * inst)
{ /* This function Creates the CSR_BT_HFG_CALL_LIST_MENU_UI                              */

    CsrBtHfgCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_MENU)
    {
        csrUiVar->eventState                                              = CSR_BT_HFG_CREATE_SK1_EVENT;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerCallListMenu;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Set the menu on the CSR_BT_HFG_CALL_LIST_MENU_UI                               */
        CsrUiMenuSetReqSend(prim->handle, TEXT_HFG_CL_MENU_UCS2, TEXT_CHANGE_UCS2, NULL);

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    {
        if (csrUiVar->eventState == CSR_BT_HFG_CREATE_SK1_EVENT)
        { /* Save the event handle as SK1, and set input mode to blocked        */

            csrUiVar->eventState                                          = CSR_BT_HFG_CREATE_BACK_EVENT;
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Create a BACK event                                              */
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else
        { /* CSR_BT_HFG_CREATE_BACK_EVENT                                           */
            /* Save the event handle as Back, and set input mode to blocked     */
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Menu items would be added dynamically*/

            /* Create  CSR_BT_HFG_SETUP_AUDIO_CODEC_SR_UI                                        */
            csrUiVar->uiIndex = CSR_BT_HFG_SETUP_AUDIO_CODEC_SR_UI;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);
        }
    }
}

/* Handle CSR_BT_HFG_SETUP_AUDIO_CODEC_SR_UI key events releated to this UI   */
static void appUiKeyHandlerSetupAudioCodecSr(void * instData,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from the CSR_BT_HFG_SETUP_AUDIO_CODEC_SR_UI */
    Connection_t *con;
    CsrBool keyOK = FALSE;
    CsrCharString buf[250];
#ifdef CSR_BT_INSTALL_HFG_CONFIG_AUDIO
    CsrCharString buf1[50];
#endif
    hfgInstance_t* inst = instData;
    CSR_UNUSED(eventHandle);

    switch(key)
    {
        case CSR_BT_HFG_CURRENT_CODEC_DETAILS_KEY:

            sprintf((char*)buf, "Select Audio codec (current is %d) and sample rate to use (current is %d)\n",
                    inst->c2cIndicators[CSR_BT_C2C_ADPCM_IND],
                    inst->c2cIndicators[CSR_BT_C2C_SAMPLE_RATE_IND]);
            CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
            CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);

            break;
        case CSR_BT_HFG_CVSD_KEY:
            inst->supportedAudioCodecMask=CSR_BT_C2C_ADPCM_OFF_VALUE;
            keyOK = TRUE;
            break;
        case CSR_BT_HFG_ADPCM_2BIT_KEY:
            inst->supportedAudioCodecMask=CSR_BT_C2C_ADPCM_2BIT_VALUE;
            keyOK = TRUE;
            break;
        case CSR_BT_HFG_ADPCM_4BIT_KEY:
            inst->supportedAudioCodecMask=CSR_BT_C2C_ADPCM_4BIT_VALUE;
            keyOK = TRUE;
            break;
        case CSR_BT_HFG_8KHZ_SAMPLE_KEY:
            inst->supportedSampleRate=CSR_BT_C2C_SAMPLE_8KHZ_VALUE;
            keyOK = TRUE;
            break;
        case CSR_BT_HFG_16KHZ_SAMPLE_KEY:
            inst->supportedSampleRate=CSR_BT_C2C_SAMPLE_16KHZ_VALUE;
            keyOK = TRUE;
            break;
        default:
            keyOK = FALSE;
            break;
    }
    if(keyOK)
    {
#ifdef CSR_BT_INSTALL_HFG_CONFIG_AUDIO
        CsrBool keyAdpcm = FALSE;
#endif
        /* disconnect SCO if connected */
        con = getActiveConnection(inst);
        if (con!=NULL)
        {
            if(con->audioOn)
            {
                toggleAudio(inst);
            }
        }

#ifdef CSR_BT_INSTALL_HFG_CONFIG_AUDIO
        if ((key >= CSR_BT_HFG_CVSD_KEY) && (key <= CSR_BT_HFG_ADPCM_4BIT_KEY))
        {/* ADPCM */
            CsrBtHfgAudioCodecType *value;
            keyAdpcm = TRUE;


            value = CsrPmemAlloc(sizeof(CsrBtHfgAudioCodecType));
            CsrMemCpy(value, &inst->supportedAudioCodecMask,sizeof(CsrBtHfgAudioCodecType));
            if (con == NULL)
            {
                CsrBtHfgConfigAudioReqSend(0xFF, CSR_BT_HFG_AUDIO_CODEC_SETUP,value,sizeof(CsrBtHfgAudioCodecType) );
            }
            else
            {/* Let the profile know what codec it should setup */
                CsrBtHfgConfigAudioReqSend(con->index, CSR_BT_HFG_AUDIO_CODEC_SETUP,value,sizeof(CsrBtHfgAudioCodecType) );
            }
        }
        else
        {/* Sample rate*/
            CsrBtHfgAudioSampleRateType *value;
            value = CsrPmemAlloc(sizeof(CsrBtHfgAudioSampleRateType));
            CsrMemCpy(value, &inst->supportedSampleRate,sizeof(CsrBtHfgAudioSampleRateType));
            if (con == NULL)
            {
                CsrBtHfgConfigAudioReqSend(0xFF, CSR_BT_HFG_AUDIO_SAMPLE_RATE_SETUP,value,sizeof(CsrBtHfgAudioSampleRateType) );
            }
            else
            {/* Let the profile know what sample rate it should setup */
                CsrBtHfgConfigAudioReqSend(con->index, CSR_BT_HFG_AUDIO_SAMPLE_RATE_SETUP,value,sizeof(CsrBtHfgAudioSampleRateType) );
            }

        }

        if (keyAdpcm)
        {
            sprintf((char*)buf, "Sent C2C codec preferred settings to the HFG (%d).\nThe value will be used next time an audio connection is performed\n",inst->supportedAudioCodecMask);
        }
        else
        {
            sprintf((char*)buf, "Sent C2C sample rate preferred settings to the HFG (%d).\nThe value will be used next time an audio connection is performed\n",inst->supportedSampleRate);
        }
        if(!con->audioOn) /* CsrBtHfgAudioConnectReqSend is not sent. We dont have to wait*/
        {
            CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                            CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
            CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
        }
        else /* CsrBtHfgAudioConnectReqSend is sent and CSR_BT_HFG_DEFAULT_INFO_UI is already displayed and is in BLOCK mode.*/
        {
            sprintf((char*)buf1, "HFG_AUDIO_CONNECT_REQ is sent. Wait for a response!!\n");
            CsrStrCat(buf1, buf);
            CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_PLEASE_WAIT_UCS2,
                                                            CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
        }
#else
        sprintf((char*)buf, "CsrBtHfgConfigAudioReqSend NOT sent as EXCLUDE_CSR_BT_HFG_MODULE_OPTIONAL is set\n");
        CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                          CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
        CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
#endif
    }
    CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);
}
static void appCreateHfgSetupAudioCodeSrUi(hfgInstance_t * inst)
{ /* This function Creates the CSR_BT_HFG_SETUP_AUDIO_CODEC_SR_UI                              */

    CsrBtHfgCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_MENU)
    {
        csrUiVar->eventState                                              = CSR_BT_HFG_CREATE_SK1_EVENT;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerSetupAudioCodecSr;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Set the menu on the CSR_BT_HFG_SETUP_AUDIO_CODEC_SR_UI                               */
        CsrUiMenuSetReqSend(prim->handle, TEXT_HFG_AUDIO_CODEC_MENU_UCS2, TEXT_SELECT_UCS2, NULL);

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    {
        if (csrUiVar->eventState == CSR_BT_HFG_CREATE_SK1_EVENT)
        { /* Save the event handle as SK1, and set input mode to blocked        */

            csrUiVar->eventState                                          = CSR_BT_HFG_CREATE_BACK_EVENT;
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Create a BACK event                                              */
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else
        { /* CSR_BT_HFG_CREATE_BACK_EVENT                                           */

            CsrUieHandle displayHandle  = csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle;
            CsrUieHandle sk1Handle      = csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle;

            /* Save the event handle as Back, and set input mode to blocked     */
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Updated the CSR_BT_HFG_SETUP_AUDIO_CODEC_SR_UI to include all the supported
               client applications                                              */
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_HFG_CURRENT_CODEC_DETAILS_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_CURRENT_CODEC_DETAILS_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_CVSD_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_CVSD_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_ADPCM_2BIT_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_ADPCM_2BIT_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_ADPCM_4BIT_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_ADPCM_4BIT_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_HFG_8KHZ_SAMPLE_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_8KHZ_SAMPLE_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_16KHZ_SAMPLE_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_16KHZ_SAMPLE_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            /* Create  CSR_BT_HFG_C2C_CALLER_NAME_UI                                        */
            csrUiVar->uiIndex = CSR_BT_HFG_C2C_CALLER_NAME_UI;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_INPUTDIALOG);
        }
    }
}

static void appCreateC2CCallerNameInputUi(hfgInstance_t * inst)
{ /* This function Creates the CSR_BT_HFG_C2C_CALLER_NAME_UI                               */

    CsrBtHfgCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_INPUTDIALOG)
    {
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerGetInputDialogMisc;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    { /* Save the event handle as SK1/BACK, and set input mode to blocked       */
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle      = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle     = prim->handle;

        CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

        csrUiVar->uiIndex = CSR_BT_HFG_C2C_TEXT_STRING_UI;
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_INPUTDIALOG);
    }
}

static void appCreateC2CTextStringInputUi(hfgInstance_t * inst)
{ /* This function Creates the CSR_BT_HFG_C2C_TEXT_STRING_UI                               */

    CsrBtHfgCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_INPUTDIALOG)
    {
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerGetInputDialogMisc;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    { /* Save the event handle as SK1/BACK, and set input mode to blocked       */
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle      = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle     = prim->handle;

        CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

        csrUiVar->uiIndex = CSR_BT_HFG_C2C_UI;
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);
    }
}

/* Handle CSR_BT_HFG_C2C_UI key events releated to this UI   */
static void appUiKeyHandlerC2CMenu(void * instData,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from the CSR_BT_HFG_C2C_UI */
    hfgInstance_t* inst = instData;
    CsrCharString buf[50];

    CSR_UNUSED(eventHandle);

    switch (key)
    {
        case CSR_BT_HFG_CALLER_NAME_KEY:
    {
            /* Set the input dialog on the CSR_BT_HFG_C2C_CALLER_NAME_UI                               */
            CsrBtHfgSetInputDialog(inst, CSR_BT_HFG_C2C_CALLER_NAME_UI,
                CONVERT_TEXT_STRING_2_UCS2("Caller Name Setting"), CONVERT_TEXT_STRING_2_UCS2("Enter the Calller Name:"),
                CSR_UI_ICON_KEY, NULL, MAX_TEXT_LENGTH,
                CSR_UI_KEYMAP_ALPHANUMERIC, TEXT_OK_UCS2, NULL);
            CsrBtHfgShowUi(inst, CSR_BT_HFG_C2C_CALLER_NAME_UI, CSR_UI_INPUTMODE_AUTO, 1);
            break;
        }
        case CSR_BT_HFG_TOGGLE_NAME_KEY:
        case CSR_BT_HFG_TOGGLE_TEXT_KEY:
        case CSR_BT_HFG_TOGGLE_SMS_KEY:
        case CSR_BT_HFG_TOGGLE_BATTERY_KEY:
        case CSR_BT_HFG_TOGGLE_POWER_KEY:
        {
#ifdef CSR_BT_INSTALL_HFG_C2C
            /* Send C2C indicator update */
            CsrUint8 ind;
            CsrUint8 val;

            ind = (key >= CSR_BT_HFG_TOGGLE_NAME_KEY && key <= CSR_BT_HFG_TOGGLE_POWER_KEY) ? key : 0;
            val = (~inst->c2cIndicators[ind]) & 0x01;

            inst->c2cIndicators[ind] = val;
            CsrBtHfgC2cSfReqSend(CSR_BT_HFG_CONNECTION_ALL, ind, val);
            sprintf((char*)buf, "%s the Feature and Sent C2C indication!!\n ", val == 0 ? "Disabled" : "Enabled");
            CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
            CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
#else
            sprintf((char*)buf, "CSR2CSR features disabled in build!!\n");
            CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
            CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
#endif
            break;
        }
        case CSR_BT_HFG_SEND_SMS_IND_KEY:
        {
#ifdef CSR_BT_INSTALL_HFG_C2C
                Connection_t *con;
                CsrCharString buf[50];

                /* Send SMS indication */
                con = getActiveConnection(inst);
                if(con != NULL)
                {
                    CsrBtHfgC2cSmsArriveReqSend(con->index,
                                           42,
                                           (CsrCharString *) StringDup(RING_NUMBER),
                                           (CsrCharString *) StringDup((char*)inst->name));
                    sprintf(buf, "Send SMS arrival (from %s) indication...\n", RING_NUMBER);
                    CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                        CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
                    CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
                }
#else
                sprintf((char*)buf, "CSR2CSR features disabled in build!!\n");
                CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                    CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
                CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
#endif
                break;
        }
        case CSR_BT_HFG_SEND_TEXT_STRING_KEY:
        {
            getActiveConnection(inst);

            /* Set the input dialog on the CSR_BT_HFG_C2C_TEXT_STRING_UI                               */
            CsrBtHfgSetInputDialog(inst, CSR_BT_HFG_C2C_TEXT_STRING_UI,
                CONVERT_TEXT_STRING_2_UCS2("Send Text String"), CONVERT_TEXT_STRING_2_UCS2("Enter the Text String:"),
                CSR_UI_ICON_KEY, NULL, MAX_TEXT_LENGTH,
                CSR_UI_KEYMAP_ALPHANUMERIC, TEXT_OK_UCS2, NULL);
            CsrBtHfgShowUi(inst, CSR_BT_HFG_C2C_TEXT_STRING_UI, CSR_UI_INPUTMODE_AUTO, 1);

            break;
        }
        case CSR_BT_HFG_SETUP_AUDIO_CODEC_SR_KEY:
            break;
        case CSR_BT_HFG_ADPCM_SUPPORT_QUERY_KEY:
        {
            CsrBtHfgGetC2CAdpcmLocalSupportedReqSend();
            CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_PLEASE_WAIT_UCS2,
                                                CONVERT_TEXT_STRING_2_UCS2("Local ADPCM support query sent. Waiting for answer..."), TEXT_SELECT_UCS2, NULL);
            CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);

            break;
        }
        default : /*                                         */
        {
        CsrGeneralException("CSR_BT_HFG", 0, 0,
                           "Unexpected menu key is received");
            break;
        }
    }
    CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);
}
static void appCreateHfgC2CMenuUi(hfgInstance_t * inst)
{ /* This function Creates the CSR_BT_HFG_C2C_UI                              */

    CsrBtHfgCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_MENU)
    {
        csrUiVar->eventState                                              = CSR_BT_HFG_CREATE_SK1_EVENT;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerC2CMenu;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Set the menu on the CSR_BT_HFG_C2C_UI                               */
        CsrUiMenuSetReqSend(prim->handle, TEXT_HFG_C2C_MENU_UCS2, TEXT_SELECT_UCS2, NULL);

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    {
        if (csrUiVar->eventState == CSR_BT_HFG_CREATE_SK1_EVENT)
        { /* Save the event handle as SK1, and set input mode to blocked        */

            csrUiVar->eventState                                          = CSR_BT_HFG_CREATE_BACK_EVENT;
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Create a BACK event                                              */
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else
        { /* CSR_BT_HFG_CREATE_BACK_EVENT                                           */

            CsrUieHandle displayHandle  = csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle;
            CsrUieHandle sk1Handle      = csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle;

            /* Save the event handle as Back, and set input mode to blocked     */
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Updated the CSR_BT_HFG_C2C_UI to include all the supported
               client applications                                              */
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_HFG_CALLER_NAME_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_C2C_ENTER_NAME_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_TOGGLE_NAME_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_C2C_TOGGLE_NAME_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_TOGGLE_TEXT_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_C2C_TOGGLE_TEXT_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_TOGGLE_SMS_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_C2C_TOGGLE_SMS_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_HFG_TOGGLE_BATTERY_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_C2C_TOGGLE_BATTERY_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_TOGGLE_POWER_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_C2C_TOGGLE_POWER_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_SEND_SMS_IND_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_C2C_IN_SMS_IND_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_SEND_TEXT_STRING_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_C2C_SEND_TEXT_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_HFG_SETUP_AUDIO_CODEC_SR_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_C2C_SETUP_CODEC_SR_UCS2, NULL, csrUiVar->displayesHandlers[CSR_BT_HFG_SETUP_AUDIO_CODEC_SR_UI].displayHandle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_ADPCM_SUPPORT_QUERY_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_C2C_QUERY_ADPCM_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            /* Create  CSR_BT_HFG_SETTINGS_IND_GLOBAL_UI                                        */
            csrUiVar->uiIndex = CSR_BT_HFG_SETTINGS_IND_GLOBAL_UI;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);
        }
    }
}

/* Handle CSR_BT_HFG_SETTINGS_IND_GLOBAL_UI key events releated to this UI   */
static void appUiKeyHandlerGlobalSettingsIndicatorsMenu(void * instData,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from the CSR_BT_HFG_SETTINGS_IND_GLOBAL_UI */
    hfgInstance_t* inst = instData;

    CSR_UNUSED(eventHandle);

    switch (key)
    {
        case CSR_BT_HFG_SET_IND_TOG_ROAM_KEY:
            toggleRoam(inst);
            break;
        case CSR_BT_HFG_SET_TOG_NET_REG_KEY:
            toggleCellRegStatus(inst);
            break;
        case CSR_BT_HFG_SET_DEC_SIGNAL_STRENGTH_KEY:
            toggleSignal(inst, FALSE);
            break;
        case CSR_BT_HFG_SET_INC_SIGNAL_STRENGTH_KEY:
            toggleSignal(inst, TRUE);
            break;
        case CSR_BT_HFG_SET_DEC_BAT_LEVEL_KEY:
            toggleBattery(inst, FALSE);
            break;
        case CSR_BT_HFG_SET_INC_BAT_LEVEL_KEY:
            toggleBattery(inst, TRUE);
            break;
        default :
        {
            CsrGeneralException("CSR_BT_HFG", 0, 0,
                               "Unexpected menu key is received");
            break;
        }
    }
    CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);
}

static void appCreateHfgGlobalSettingIndicatorsUi(hfgInstance_t * inst)
{ /* This function Creates the CSR_BT_HFG_SETTINGS_IND_GLOBAL_UI                              */

    CsrBtHfgCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_MENU)
    {
        csrUiVar->eventState                                              = CSR_BT_HFG_CREATE_SK1_EVENT;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerGlobalSettingsIndicatorsMenu;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Set the menu on the CSR_BT_HFG_C2C_UI                               */
        CsrUiMenuSetReqSend(prim->handle, TEXT_HFG_SETTINGS_IND_MENU_UCS2, TEXT_SELECT_UCS2, NULL);

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    {
        if (csrUiVar->eventState == CSR_BT_HFG_CREATE_SK1_EVENT)
        { /* Save the event handle as SK1, and set input mode to blocked        */

            csrUiVar->eventState                                          = CSR_BT_HFG_CREATE_BACK_EVENT;
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Create a BACK event                                              */
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else
        { /* CSR_BT_HFG_CREATE_BACK_EVENT                                           */

            CsrUieHandle displayHandle  = csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle;
            CsrUieHandle sk1Handle      = csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle;

            /* Save the event handle as Back, and set input mode to blocked     */
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Updated the CSR_BT_HFG_C2C_UI to include all the supported
               client applications                                              */
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_HFG_SET_IND_TOG_ROAM_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_SET_IND_TOG_ROAM_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_SET_TOG_NET_REG_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_SET_TOG_NET_REG_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_SET_DEC_SIGNAL_STRENGTH_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_SET_DEC_SIGNAL_STRENGTH_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_SET_INC_SIGNAL_STRENGTH_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_SET_INC_SIGNAL_STRENGTH_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_HFG_SET_DEC_BAT_LEVEL_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_SET_DEC_BAT_LEVEL_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_SET_INC_BAT_LEVEL_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_SET_INC_BAT_LEVEL_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            /* Create  CSR_BT_HFG_SETTINGS_IND_ACTIVE_UI                                        */
            csrUiVar->uiIndex = CSR_BT_HFG_SETTINGS_IND_ACTIVE_UI;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);
        }
    }
}


/* Handle CSR_BT_HFG_SETTINGS_IND_ACTIVE_UI key events releated to this UI   */
static void appUiKeyHandlerCurActiveSettingsIndicatorsMenu(void * instData,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from the CSR_BT_HFG_SETTINGS_IND_ACTIVE_UI */
    hfgInstance_t* inst = instData;

    CSR_UNUSED(eventHandle);

    switch (key)
    {
        case CSR_BT_HFG_SET_IND_CUR_CON_MIC_INC_KEY:
            toggleMic(inst, TRUE);
            break;
        case CSR_BT_HFG_SET_IND_CUR_CON_MIC_DEC_KEY:
            toggleMic(inst, FALSE);
            break;
        case CSR_BT_HFG_SET_IND_CUR_CON_SPE_INC_KEY:
            toggleSpeaker(inst, TRUE);
            break;
        case CSR_BT_HFG_SET_IND_CUR_CON_SPE_DEC_KEY:
            toggleSpeaker(inst, FALSE);
            break;
        case CSR_BT_HFG_SET_IND_CUR_CON_IBR_ON_KEY:
            toggleInband(inst, TRUE);
            break;
        case CSR_BT_HFG_SET_IND_CUR_CON_IBR_OFF_KEY:
            toggleInband(inst, FALSE);
            break;
        case CSR_BT_HFG_SET_IND_CUR_CON_VR_ON_KEY:
            toggleVoiceRecog(inst, TRUE);
            break;
        case CSR_BT_HFG_SET_IND_CUR_CON_VR_OFF_KEY:
            toggleVoiceRecog(inst, FALSE);
            break;
        default :
        {
            CsrGeneralException("CSR_BT_HFG", 0, 0,
                               "Unexpected menu key is received");
            break;
        }
    }
    CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);
}
static void appCreateHfgCurActiveSettingIndicatorsUi(hfgInstance_t * inst)
{ /* This function Creates the CSR_BT_HFG_SETTINGS_IND_ACTIVE_UI                              */

    CsrBtHfgCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_MENU)
    {
        csrUiVar->eventState                                              = CSR_BT_HFG_CREATE_SK1_EVENT;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerCurActiveSettingsIndicatorsMenu;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Set the menu on the CSR_BT_HFG_C2C_UI                               */
        CsrUiMenuSetReqSend(prim->handle, TEXT_HFG_SETTINGS_IND_MENU_UCS2, TEXT_SELECT_UCS2, NULL);

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    {
        if (csrUiVar->eventState == CSR_BT_HFG_CREATE_SK1_EVENT)
        { /* Save the event handle as SK1, and set input mode to blocked        */

            csrUiVar->eventState                                          = CSR_BT_HFG_CREATE_BACK_EVENT;
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Create a BACK event                                              */
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else
        { /* CSR_BT_HFG_CREATE_BACK_EVENT                                           */

            CsrUieHandle displayHandle  = csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle;
            CsrUieHandle sk1Handle      = csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle;

            /* Save the event handle as Back, and set input mode to blocked     */
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Updated the CSR_BT_HFG_C2C_UI to include all the supported
               client applications                                              */
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_HFG_SET_IND_CUR_CON_MIC_INC_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_SET_IND_CUR_CON_MIC_INC_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_SET_IND_CUR_CON_MIC_DEC_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_SET_IND_CUR_CON_MIC_DEC_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_SET_IND_CUR_CON_SPE_INC_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_SET_IND_CUR_CON_SPE_INC_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_SET_IND_CUR_CON_SPE_DEC_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_SET_IND_CUR_CON_SPE_DEC_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_HFG_SET_IND_CUR_CON_IBR_ON_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_SET_IND_CUR_CON_IBR_ON_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_SET_IND_CUR_CON_IBR_OFF_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_SET_IND_CUR_CON_IBR_OFF_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_HFG_SET_IND_CUR_CON_VR_ON_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_SET_IND_CUR_CON_VR_ON_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_SET_IND_CUR_CON_VR_OFF_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_SET_IND_CUR_CON_VR_OFF_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            /* Create  CSR_BT_HFG_SETTINGS_INDICATORS_UI                                        */
            csrUiVar->uiIndex = CSR_BT_HFG_SETTINGS_INDICATORS_UI;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);
        }
    }
}
/* Handle CSR_BT_HFG_SETTINGS_INDICATORS_UI key events releated to this UI   */
static void appUiKeyHandlerSettingsIndicatorsMenu(void * instData,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from the CSR_BT_HFG_SETTINGS_INDICATORS_UI */
    hfgInstance_t* inst = instData;
    Connection_t *con;
    CSR_UNUSED(eventHandle);

    switch (key)
    {
        case CSR_BT_HFG_SET_IND_GLOBAL_KEY:
            break;
        case CSR_BT_HFG_SET_IND_CUR_ACTIVE_KEY:
            con = getActiveConnection(inst);
            if(con != NULL)
            {
                CsrBtHfgShowUi(inst, CSR_BT_HFG_SETTINGS_IND_ACTIVE_UI, CSR_UI_INPUTMODE_AUTO, 1);
            }
            else
            {
                CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_FAILED_UCS2,
                                                                        TEXT_HFG_NO_ACTIVE_CONNNECTIONS, TEXT_OK_UCS2, NULL);
                /* Show the CSR_BT_HFG_DEFAULT_INFO_UI on the display                   */
                CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
            }

            break;
        default :
        {
            CsrGeneralException("CSR_BT_HFG", 0, 0,
                               "Unexpected menu key is received");
            break;
        }
    }
    CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);
}

static void appCreateHfgSettingIndicatorsUi(hfgInstance_t * inst)
{ /* This function Creates the CSR_BT_HFG_SETTINGS_INDICATORS_UI                              */

    CsrBtHfgCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_MENU)
    {
        csrUiVar->eventState                                              = CSR_BT_HFG_CREATE_SK1_EVENT;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerSettingsIndicatorsMenu;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Set the menu on the CSR_BT_HFG_C2C_UI                               */
        CsrUiMenuSetReqSend(prim->handle, TEXT_HFG_SETTINGS_IND_MENU_UCS2, TEXT_SELECT_UCS2, NULL);

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    {
        if (csrUiVar->eventState == CSR_BT_HFG_CREATE_SK1_EVENT)
        { /* Save the event handle as SK1, and set input mode to blocked        */

            csrUiVar->eventState                                          = CSR_BT_HFG_CREATE_BACK_EVENT;
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Create a BACK event                                              */
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else
        { /* CSR_BT_HFG_CREATE_BACK_EVENT                                           */

            CsrUieHandle displayHandle  = csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle;
            CsrUieHandle sk1Handle      = csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle;

            /* Save the event handle as Back, and set input mode to blocked     */
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_HFG_SET_IND_GLOBAL_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_SETTINGS_IND_GLOBAL_UCS2, NULL, csrUiVar->displayesHandlers[CSR_BT_HFG_SETTINGS_IND_GLOBAL_UI].displayHandle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_SET_IND_CUR_ACTIVE_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_SET_IND_TOG_CUR_CON_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);


            /* Create  CSR_BT_HFG_SCO_ESCO_INPUT_UI                                        */
            csrUiVar->uiIndex = CSR_BT_HFG_SCO_ESCO_INPUT_UI;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_INPUTDIALOG);
        }
    }
}

static void appCreateScoEscoInputUi(hfgInstance_t * inst)
{ /* This function Creates the CSR_BT_HFG_SCO_ESCO_INPUT_UI                               */

    CsrBtHfgCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_INPUTDIALOG)
    {
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerGetInputDialogMisc;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    { /* Save the event handle as SK1/BACK, and set input mode to blocked       */
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle      = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle     = prim->handle;

        CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

        csrUiVar->uiIndex = CSR_BT_HFG_SCO_ESCO_UI;
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);
    }
}

/* Handle CSR_BT_HFG_SCO_ESCO_UI key events releated to this UI   */
static void appUiKeyHandlerScoEscoMenu(void * instData,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from the CSR_BT_HFG_SCO_ESCO_UI */
    hfgInstance_t* inst = instData;
    CSR_UNUSED(eventHandle);

    switch (key)
    {
        case CSR_BT_HFG_CHANGE_TX_PATH_KEY:
        case CSR_BT_HFG_CHANGE_RX_PATH_KEY:
        case CSR_BT_HFG_CHANGE_MAX_LATENCY_KEY:
        case CSR_BT_HFG_CHANGE_VOICE_CODEC_KEY:
        case CSR_BT_HFG_CHANGE_AUDIO_QUALITY_KEY:
        case CSR_BT_HFG_CHANGE_RETRAN_EFFORT_KEY:
            inst->scoKey = (CsrUint8) key;
            /* Set the input dialog on the CSR_BT_HFG_C2C_TEXT_STRING_UI                               */
            CsrBtHfgSetInputDialog(inst, CSR_BT_HFG_SCO_ESCO_INPUT_UI,
                CONVERT_TEXT_STRING_2_UCS2("SCO Parameter Setting"), CONVERT_TEXT_STRING_2_UCS2("Enter the SCO Parameter:"),
                CSR_UI_ICON_KEY, NULL, MAX_TEXT_LENGTH,
                CSR_UI_KEYMAP_ALPHANUMERIC, TEXT_OK_UCS2, NULL);
            CsrBtHfgShowUi(inst, CSR_BT_HFG_SCO_ESCO_INPUT_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
            break;

        default :
        {
            CsrGeneralException("CSR_BT_HFG", 0, 0,
                               "Unexpected menu key is received");
            break;
        }
    }
    CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);
}
static void appCreateHfgScoEscoUi(hfgInstance_t * inst)
{ /* This function Creates the CSR_BT_HFG_SCO_ESCO_UI                              */

    CsrBtHfgCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_MENU)
    {
        csrUiVar->eventState                                              = CSR_BT_HFG_CREATE_SK1_EVENT;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerScoEscoMenu;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Set the menu on the CSR_BT_HFG_SCO_ESCO_UI                               */
        CsrUiMenuSetReqSend(prim->handle, TEXT_HFG_SCO_ESCO_MENU_UCS2, TEXT_SELECT_UCS2, NULL);

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    {
        if (csrUiVar->eventState == CSR_BT_HFG_CREATE_SK1_EVENT)
        { /* Save the event handle as SK1, and set input mode to blocked        */

            csrUiVar->eventState                                          = CSR_BT_HFG_CREATE_BACK_EVENT;
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Create a BACK event                                              */
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else
        { /* CSR_BT_HFG_CREATE_BACK_EVENT                                           */

            CsrUieHandle displayHandle  = csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle;
            CsrUieHandle sk1Handle      = csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle;

            /* Save the event handle as Back, and set input mode to blocked     */
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_HFG_CHANGE_TX_PATH_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_SCO_CHANGE_TX_BW_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_CHANGE_RX_PATH_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_SCO_CHANGE_RX_BW_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_CHANGE_MAX_LATENCY_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_SCO_CHANGE_MAX_LAT_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_CHANGE_VOICE_CODEC_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_SCO_CHANGE_VOICE_CODEC_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_HFG_CHANGE_AUDIO_QUALITY_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_SCO_CHANGE_AUDIO_Q_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_CHANGE_RETRAN_EFFORT_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_SCO_CHANGE_RETRAN_EFFORT_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            /* Create  CSR_BT_HFG_DIAL_INFO_DIALOG_UI                                        */
            csrUiVar->uiIndex = CSR_BT_HFG_DIAL_INFO_DIALOG_UI;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_DIALOG);
        }
    }
}


/* Handle CSR_BT_HFG_DIAL_INFO_DIALOG_UI key events releated to this UI   */
static void appUiKeyHandlerDialInfoDialog(void * instData,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from the CSR_BT_HFG_DIAL_INFO_DIALOG_UI */
    hfgInstance_t* inst = instData;
    CsrBtHfgCsrUiType    * csrUiVar  = &(inst->csrUiVar);

    CSR_UNUSED(eventHandle);
    CSR_UNUSED(key);

    CsrUiUieHideReqSend(csrUiVar->displayesHandlers[CSR_BT_HFG_DIAL_INFO_DIALOG_UI].displayHandle);

    CsrBtHfgShowUi(inst, CSR_BT_HFG_DIAL_IND_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
}

static void appCreateDialInfoDialogUi(hfgInstance_t * inst)
{ /* This function Creates the CSR_BT_HFG_DIAL_INFO_DIALOG_UI                               */

    CsrBtHfgCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_DIALOG)
    {
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerDialInfoDialog;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;
        csrUiVar->eventState                                              = CSR_BT_HFG_CREATE_SK1_EVENT;

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else if(csrUiVar->eventState == CSR_BT_HFG_CREATE_SK1_EVENT)
    { /* Save the event handle as SK1, and set input mode to blocked       */
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle      = prim->handle;
        csrUiVar->eventState = CSR_BT_HFG_CREATE_BACK_EVENT;

        /* Create a BACK event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);

        CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());
    }
    else
    {
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle     = prim->handle;
        csrUiVar->uiIndex = CSR_BT_HFG_DIAL_IND_UI;
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);
    }
}
/* Handle CSR_BT_HFG_DIAL_IND_UI key events releated to this UI   */
static void appUiKeyHandlerDialIndMenu(void * instData,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from the CSR_BT_HFG_DIAL_IND_UI */
    hfgInstance_t* inst = instData;
    Connection_t *con;
    CsrUint8 call;
    CsrCharString buf[300] = "";
    CsrCharString buf1[100] = "";
    CsrBool heading = TRUE;

    CSR_UNUSED(eventHandle);

    con = getActiveConnection(inst);
    if(con == NULL)
    {
        sprintf(buf,"No active connection selected\n");
        CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_FAILED_UCS2,
                                                                CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
        /* Show the CSR_BT_HFG_DEFAULT_INFO_UI on the display                   */
        CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
    }
    else
    {
        /* We can only send dial-response error as the
         * first one */
        if(!con->dialOk && (key == CSR_BT_HFG_DIAL_IND_SEND_ERROR_KEY))
        {
            callUnsetStatus(inst, CS_DIALING);
            CsrBtHfgDialResSend(con->index,
                           CSR_BT_CME_NO_NETWORK_SERVICE);
            sprintf(buf,"Send dial response error code...\n");
            CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_FAILED_UCS2,
                                                                    CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
            /* Show the CSR_BT_HFG_DEFAULT_INFO_UI on the display                   */
            CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
        }
        else if((key == CSR_BT_HFG_DIAL_IND_REM_ALERT_KEY) ||
                (key == CSR_BT_HFG_DIAL_IND_REM_ANS_KEY) ||
                (key == CSR_BT_HFG_DIAL_IND_REM_REJ_KEY))
        {
            /* If this is the first dial response keypress, we must send
             * the dial-response-ok and setup the outgoing call, audio,
             * etc. */
            if(!con->dialOk)
            {
                con->dialOk = TRUE;

                CsrBtHfgDialResSend(con->index,
                               CSR_BT_CME_SUCCESS);

                CsrBtHfgStatusIndicatorSetReqSend(con->index,
                                             CSR_BT_CALL_SETUP_STATUS_INDICATOR,
                                             CSR_BT_OUTGOING_CALL_SETUP_VALUE);

                /* Do we have any calls on hold? */
                call = callGetStatus(inst, CS_HELD);
                if(call != NO_CALL)
                {
                    if ( callGetStatus(inst, CS_ACTIVE) == NO_CALL)
                    { /* No active calls */
                        CsrBtHfgStatusIndicatorSetReqSend(con->index,
                                                     CSR_BT_CALL_HELD_INDICATOR,
                                                     CSR_BT_CALL_HELD_NO_ACTIVE_CALL_VALUE);
                    }
                    else
                    {/* Active call(s) exist */
                        CsrBtHfgStatusIndicatorSetReqSend(con->index,
                                                     CSR_BT_CALL_HELD_INDICATOR,
                                                     CSR_BT_CALL_HELD_RETRIEVE_OTHER_CALL_VALUE);
                    }
                }

                sprintf(buf,  "Sent 'outgoing-call-setup' indicator...\n");
            }

            /* Handle the actual keypress */
            switch(key)
            {
                case CSR_BT_HFG_DIAL_IND_REM_ALERT_KEY:
                    {
                        /* Alerting */
                        call = callChangeStatus(inst, CS_DIALING, CS_ALERTING);
                        if(call != NO_CALL)
                        {
                            sprintf(buf1, "Dialing call is now alerting (index %i)\n", call);
                            CsrStrCat(buf, buf1);
                        }

                        CsrBtHfgStatusIndicatorSetReqSend(CSR_BT_HFG_CONNECTION_ALL,
                                                     CSR_BT_CALL_SETUP_STATUS_INDICATOR,
                                                     CSR_BT_OUTGOING_REMOTE_ALERT_VALUE);
                        sprintf(buf1, "Sent 'alerting-remote' indicator...\n");
                        CsrStrCat(buf, buf1);

                        if(!con->audioOn)
                        {
                            CsrBtHfgAudioConnectReqSend(con->index,PCM_SLOT, PCM_SLOT_REALLOCATE);
                            sprintf(buf1, "\n\nRequested audio on for outgoing call alerting...\n");
                            CsrStrCat(buf, buf1);
                            heading = FALSE; /* to make it as  Please wait string*/
                        }

                        break;
                    }

                case CSR_BT_HFG_DIAL_IND_REM_ANS_KEY:
                    {
                        /* Put alerting/dialing call on active */
                        call = callChangeStatus(inst, CS_ALERTING, CS_ACTIVE);
                        if(call != NO_CALL)
                        {
                            sprintf(buf1, "Alerting call is now active (index %i)\n", call);
                            CsrStrCat(buf, buf1);
                        }
                        else
                        {
                            call = callChangeStatus(inst, CS_DIALING, CS_ACTIVE);
                            if(call != NO_CALL)
                            {
                                sprintf(buf1, "Dialing call is now active (index %i)\n", call);
                                CsrStrCat(buf, buf1);
                            }
                        }

                        if (callGetStatus(inst, CS_HELD) == NO_CALL)
                        {
                            CsrBtHfgStatusIndicatorSetReqSend(CSR_BT_HFG_CONNECTION_ALL,
                                                         CSR_BT_CALL_STATUS_INDICATOR,
                                                         CSR_BT_CALL_ACTIVE_VALUE);
                        }
                        else
                        {
                            CsrBtHfgStatusIndicatorSetReqSend(CSR_BT_HFG_CONNECTION_ALL,
                                                         CSR_BT_CALL_HELD_INDICATOR,
                                                         CSR_BT_CALL_HELD_RETRIEVE_OTHER_CALL_VALUE);
                        }

                        CsrBtHfgStatusIndicatorSetReqSend(CSR_BT_HFG_CONNECTION_ALL,
                                                     CSR_BT_CALL_SETUP_STATUS_INDICATOR,
                                                     CSR_BT_NO_CALL_SETUP_VALUE);
                        sprintf(buf1, "Sent indicators for 'call-active' and 'no-call-setup'...\n");
                        CsrStrCat(buf, buf1);

                        if(!con->audioOn)
                        {
                            CsrBtHfgAudioConnectReqSend(con->index,PCM_SLOT, PCM_SLOT_REALLOCATE);
                            sprintf(buf1, "\n\nRequested audio on for outgoing call...\n");
                            CsrStrCat(buf, buf1);
                            heading = FALSE; /* to make it as  Please wait string*/
                        }

                        sprintf(buf, "Sent 'call-active' and 'no-call-setup' indicators...\n");
                        CsrStrCat(buf, buf1);
                        break;
                    }

                case CSR_BT_HFG_DIAL_IND_REM_REJ_KEY:
                    {
                        /* Reject dialing/alerting call */
                        call = callUnsetStatus(inst, CS_DIALING);
                        if(call != NO_CALL)
                        {
                            sprintf(buf1, "Removed dialing call from list (index %i)\n", call);
                            CsrStrCat(buf, buf1);
                        }
                        else
                        {
                            call = callUnsetStatus(inst, CS_ALERTING);
                            if(call != NO_CALL)
                            {
                                sprintf(buf1, "Removed alerting call from list (index %i)\n", call);
                                CsrStrCat(buf, buf1);
                            }
                        }

                        CsrBtHfgStatusIndicatorSetReqSend(CSR_BT_HFG_CONNECTION_ALL,
                                                     CSR_BT_CALL_SETUP_STATUS_INDICATOR,
                                                     CSR_BT_NO_CALL_SETUP_VALUE);
                        sprintf(buf1, "Remote site reject call, sent status indicators (no setup, no active)...\n");
                        CsrStrCat(buf, buf1);

                        if(con->audioOn)
                        {
                            CsrBtHfgAudioDisconnectReqSend(con->index);
                            sprintf(buf1, "\n\nRequested audio off for outgoing call rejected...\n");
                            CsrStrCat(buf, buf1);
                            heading = FALSE; /* to make it as  Please wait string*/
                        }

                        break;
                    }
            }
        }
        CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, heading == TRUE ? TEXT_SUCCESS_UCS2 : TEXT_PLEASE_WAIT_UCS2,
                                                                CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
        /* Show the CSR_BT_HFG_DEFAULT_INFO_UI on the display                   */
        CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);

        /* Unlock the CSR_BT_HFG_DIAL_IND_UI                               */
        CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);

    }
}
static void appCreateHfgDialMenuUi(hfgInstance_t * inst)
{ /* This function Creates the CSR_BT_HFG_DIAL_IND_UI                              */

    CsrBtHfgCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_MENU)
    {
        csrUiVar->eventState                                              = CSR_BT_HFG_CREATE_SK1_EVENT;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerDialIndMenu;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Set the menu on the CSR_BT_HFG_DIAL_IND_UI                               */
        CsrUiMenuSetReqSend(prim->handle, TEXT_HFG_DIAL_IND_MENU_UCS2, TEXT_SELECT_UCS2, NULL);

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    {
        if (csrUiVar->eventState == CSR_BT_HFG_CREATE_SK1_EVENT)
        { /* Save the event handle as SK1, and set input mode to blocked        */

            csrUiVar->eventState                                          = CSR_BT_HFG_CREATE_BACK_EVENT;
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Create a BACK event                                              */
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else
        { /* CSR_BT_HFG_CREATE_BACK_EVENT                                           */

            CsrUieHandle displayHandle  = csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle;
            CsrUieHandle sk1Handle      = csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle;

            /* Save the event handle as Back, and set input mode to blocked     */
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_HFG_DIAL_IND_SEND_ERROR_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_DIAL_IND_SEND_ERROR_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_DIAL_IND_REM_ALERT_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_DIAL_IND_REM_ALERT_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_DIAL_IND_REM_ANS_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_DIAL_IND_REM_ANS_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_DIAL_IND_REM_REJ_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_DIAL_IND_REM_REJ_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            csrUiVar->uiIndex = CSR_BT_HFG_MAIN_MENU_UI;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);

        }
    }
}

void updateMainMenu(hfgInstance_t *inst)
{
    CsrBtHfgCsrUiType    * csrUiVar = &(inst->csrUiVar);
    CsrUieHandle displayHandle = csrUiVar->displayesHandlers[CSR_BT_HFG_MAIN_MENU_UI].displayHandle;
    CsrUieHandle sk1EventHandle = csrUiVar->displayesHandlers[CSR_BT_HFG_MAIN_MENU_UI].sk1EventHandle;
    CsrUieHandle backEventHandle = csrUiVar->displayesHandlers[CSR_BT_HFG_MAIN_MENU_UI].backEventHandle;

    Connection_t *con;

    con = getActiveConnection(inst);

    CsrUiMenuRemoveallitemsReqSend(csrUiVar->displayesHandlers[CSR_BT_HFG_MAIN_MENU_UI].displayHandle);

    if(inst->state == stDeactivated)
    {
        CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_HFG_ACT_NO_OPR_NAME_SUP_KEY,
                        CSR_UI_ICON_NONE, TEXT_HFG_ACTIVATE_WO_OP_NAME_UCS2, NULL, sk1EventHandle,
                        CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);

        CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_ACT_OPR_NAME_SUP_KEY,
                        CSR_UI_ICON_NONE, TEXT_HFG_ACTIVATE_WITH_OP_NAME_UCS2, NULL, sk1EventHandle,
                        CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);

        CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_AT_CMD_KEY,
                        CSR_UI_ICON_NONE, TEXT_HFG_AT_COMMANDS_UCS2, NULL, csrUiVar->displayesHandlers[CSR_BT_HFG_AT_CMD_MODE_UI].displayHandle ,
                        CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);

        CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_SUPPORTED_FEATURES_KEY,
                        CSR_UI_ICON_NONE, TEXT_HFG_MAIN_SUPPORTED_FEATURE_UCS2, NULL, csrUiVar->displayesHandlers[CSR_BT_HFG_SUP_FEAT_UI].displayHandle,
                        CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);

    }
    else if((inst->state != stDeactivated) && (con != NULL) && (con->active))
    {
        CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_HFG_DISCONNECT_KEY,
                        CSR_UI_ICON_NONE, TEXT_HFG_DISCONNECT_UCS2, NULL, sk1EventHandle,
                        CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);

        CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_TOGGLE_RF_KEY,
                        CSR_UI_ICON_NONE, TEXT_HFG_MAIN_TOGGLE_RF_UCS2, NULL, sk1EventHandle,
                        CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);

        CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_AT_CMD_MODE_CONFIG_KEY,
                        CSR_UI_ICON_NONE, TEXT_HFG_MAIN_AT_CMD_MODE_CONFIG_UCS2, NULL, csrUiVar->displayesHandlers[CSR_BT_HFG_AT_MODE_CFG_CMDS_UI].displayHandle,
                        CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);

        CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_CALL_HANDLING_KEY,
                        CSR_UI_ICON_NONE, TEXT_HFG_MAIN_CALL_HANDLING_UCS2, NULL, csrUiVar->displayesHandlers[CSR_BT_HFG_CALL_HANDLING_UI].displayHandle,
                        CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);

        CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_CALL_LIST_KEY,
                        CSR_UI_ICON_NONE, TEXT_HFG_MAIN_CALL_LIST_UCS2, NULL, sk1EventHandle,
                        CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);

        CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_C2C_KEY,
                        CSR_UI_ICON_NONE, TEXT_HFG_MAIN_C2C_UCS2, NULL, csrUiVar->displayesHandlers[CSR_BT_HFG_C2C_UI].displayHandle,
                        CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);

        CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_SETTINGS_INDICATORS_KEY,
                        CSR_UI_ICON_NONE, TEXT_HFG_MAIN_SETTINGS_IND_UCS2, NULL, csrUiVar->displayesHandlers[CSR_BT_HFG_SETTINGS_INDICATORS_UI].displayHandle,
                        CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);

        CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_SCO_ESCO_KEY,
                        CSR_UI_ICON_NONE, TEXT_HFG_MAIN_SCO_ESCO_UCS2, NULL, csrUiVar->displayesHandlers[CSR_BT_HFG_SCO_ESCO_UI].displayHandle,
                        CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);

        CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_TRAVERSE_CONN_KEY,
                        CSR_UI_ICON_NONE, TEXT_HFG_TRAVERSE_CONNECTIONS_UCS2, NULL, sk1EventHandle,
                        CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);
    }
    else
    {
        CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_HFG_DEACTIVATE_KEY,
                        CSR_UI_ICON_NONE, TEXT_HFG_DEACTIVATE_UCS2, NULL, sk1EventHandle,
                        CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);

        CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_CON_USING_HFG_KEY,
                        CSR_UI_ICON_NONE, TEXT_HFG_CONNECT_TO_SELECTED_HFP_UCS2, NULL, sk1EventHandle,
                        CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);

        CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_CON_USING_HS_KEY,
                        CSR_UI_ICON_NONE, TEXT_HFG_CONNECT_TO_SELECTED_HSP_UCS2, NULL, sk1EventHandle,
                        CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);

        CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_CALL_SIMULATE_KEY,
                        CSR_UI_ICON_NONE, TEXT_HFG_CALL_SIMULATE_UCS2, NULL, csrUiVar->displayesHandlers[CSR_BT_HFG_CALL_SIMULATE_UI].displayHandle,
                        CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);

        CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_TOGGLE_RF_KEY,
                        CSR_UI_ICON_NONE, TEXT_HFG_MAIN_TOGGLE_RF_UCS2, NULL, sk1EventHandle,
                        CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);
    }
}
static void appUiKeyHandlerHfgConMainMenu(void * instData,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from the CSR_BT_HFG_MAIN_MENU_UI  */
    hfgInstance_t * inst = instData;
    CSR_UNUSED(eventHandle);

    switch (key)
    {
        case CSR_BT_HFG_DEACTIVATE_KEY:
        {
            toggleActivation(inst);
            break;
        }
        case CSR_BT_HFG_CON_USING_HFG_KEY:
        {
            startHfgConnecting(inst, CSR_BT_HFG_CONNECTION_UNKNOWN);
            break;
        }
        case CSR_BT_HFG_CON_USING_HS_KEY:
        {
            startHfgConnecting(inst, CSR_BT_HFG_CONNECTION_AG);
            break;
        }
    case CSR_BT_HFG_ACT_OPR_NAME_SUP_KEY:
    {
        toggleActivation(inst);
        break;
    }
    case CSR_BT_HFG_ACT_NO_OPR_NAME_SUP_KEY:
    {
        hfgActivate(inst);
        break;
    }
    case CSR_BT_HFG_AT_CMD_KEY:
    {
        break;
    }
    case CSR_BT_HFG_SUPPORTED_FEATURES_KEY:
    {
        break;
    }
        case CSR_BT_HFG_DISCONNECT_KEY:
    {
            startDisconnecting(inst);
            break;
        }
        case CSR_BT_HFG_TOGGLE_RF_KEY:
        {
             startToggleRfShield(inst);
        break;
        }
        case CSR_BT_HFG_AT_CMD_MODE_CONFIG_KEY:
        break;
        case CSR_BT_HFG_CALL_HANDLING_KEY:
        break;
        case CSR_BT_HFG_CALL_LIST_KEY:
    {
        createDisplayCalllist(inst);
        if(inst->isAnyMenuItem)
        {
            /* Show the CSR_BT_HFG_CALL_LIST_MENU_UI on the display                                  */
            CsrBtHfgShowUi(inst, CSR_BT_HFG_CALL_LIST_MENU_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
        }
        else
        {
            CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_FAILED_UCS2,
                                            CONVERT_TEXT_STRING_2_UCS2("There is no Avctive Call!!"), TEXT_OK_UCS2, NULL);
            CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
        }
        break;
    }
        case CSR_BT_HFG_C2C_KEY:

        break;
        case CSR_BT_HFG_SETTINGS_INDICATORS_KEY:
        break;
        case CSR_BT_HFG_SCO_ESCO_KEY:
        break;
        case CSR_BT_HFG_TRAVERSE_CONN_KEY:
        {
            toggleHfgConnections(inst);
            break;
        }
    default :
        {
            CsrGeneralException("CSR_BT_HFG", 0, 0,
                           "Unexpected menu key is received");
            break;
        }
    }
    CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);
}

static void appCreateHfgMainMenuUi(hfgInstance_t * inst)
{ /* This function Creates the CSR_BT_HFG_MAIN_MENU_UI                              */

    CsrBtHfgCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_MENU)
    {
        csrUiVar->eventState                                              = CSR_BT_HFG_CREATE_SK1_EVENT;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerHfgConMainMenu;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Set the menu on the CSR_BT_HFG_MAIN_MENU_UI                               */
        CsrUiMenuSetReqSend(prim->handle, TEXT_MAIN_MENU_UCS2, TEXT_SELECT_UCS2, NULL);

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    {
        if (csrUiVar->eventState == CSR_BT_HFG_CREATE_SK1_EVENT)
        { /* Save the event handle as SK1, and set input mode to blocked        */

            csrUiVar->eventState                                          = CSR_BT_HFG_CREATE_BACK_EVENT;
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Create a BACK event                                              */
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else
        { /* CSR_BT_HFG_CREATE_BACK_EVENT                                           */

            CsrUieHandle displayHandle  = csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle;
            CsrUieHandle sk1Handle      = csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle;

            /* Save the event handle as Back, and set input mode to blocked     */
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_HFG_DEACTIVATE_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_DEACTIVATE_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_CON_USING_HFG_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_CONNECT_TO_SELECTED_HFP_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_CON_USING_HS_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_CONNECT_TO_SELECTED_HSP_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_CALL_SIMULATE_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_CALL_SIMULATE_UCS2, NULL, csrUiVar->displayesHandlers[CSR_BT_HFG_CALL_SIMULATE_UI].displayHandle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            csrUiVar->uiIndex = CSR_BT_HFG_LDN_INPUT_DIALG_UI;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_INPUTDIALOG);

        }
    }
}
static void appCreateDefaultInfoDialogUi(hfgInstance_t * inst)
{ /* This function Creates the CSR_BT_HFG_DEFAULT_INFO_UI                               */

    CsrBtHfgCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_DIALOG)
    {
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerHideMics;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    { /* Save the event handle as SK1/BACK, and set input mode to blocked       */
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle      = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle     = prim->handle;

        CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

        csrUiVar->uiIndex = CSR_BT_HFG_CALL_SIMULATE_UI;
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);
    }
}

static void appUiKeyHandlerCallSimulateMenu(void * instData,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from the CSR_BT_HFG_CALL_SIMULATE_UI  */
    hfgInstance_t * inst = instData;
    CSR_UNUSED(eventHandle);

    switch (key)
    {
        case CSR_BT_HFG_CS_PLACE_INCOMING_HFG_KEY:
        {
            startRinging(inst, RING_NUMBER);
            break;
        }
        case CSR_BT_HFG_CS_CALL_LIST_KEY:
        {
            createDisplayCalllist(inst);
            if(inst->isAnyMenuItem)
            {
                /* Show the CSR_BT_HFG_CALL_LIST_MENU_UI on the display                                  */
                CsrBtHfgShowUi(inst, CSR_BT_HFG_CALL_LIST_MENU_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
            }
            else
            {
                CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_FAILED_UCS2,
                                                CONVERT_TEXT_STRING_2_UCS2("There is no Avctive Call!!"), TEXT_OK_UCS2, NULL);
                CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
            }
            break;
        }
        case CSR_BT_HFG_CS_PLACE_OUTGOING_HFG_KEY:
        {
            startOutgoingCall(inst, RING_NUMBER_2);
            break;
        }
        default :
        {
            CsrGeneralException("CSR_BT_HFG", 0, 0,
                           "Unexpected menu key is received");
            break;
        }
    }
    CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);
}

static void appCreateHfgCallSimulateUi(hfgInstance_t * inst)
{ /* This function Creates the CSR_BT_HFG_CALL_SIMULATE_UI                               */

    CsrBtHfgCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_MENU)
    {
        csrUiVar->eventState                                              = CSR_BT_HFG_CREATE_SK1_EVENT;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerCallSimulateMenu;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Set the menu on the CSR_BT_HFG_CALL_SIMULATE_UI                               */
        CsrUiMenuSetReqSend(prim->handle, TEXT_HFG_CALL_SIMULATE_UCS2, TEXT_SELECT_UCS2, NULL);

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    {
        if (csrUiVar->eventState == CSR_BT_HFG_CREATE_SK1_EVENT)
        { /* Save the event handle as SK1, and set input mode to blocked        */

            csrUiVar->eventState                                          = CSR_BT_HFG_CREATE_BACK_EVENT;
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Create a BACK event                                              */
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else
        { /* CSR_BT_HFG_CREATE_BACK_EVENT                                           */

            CsrUieHandle displayHandle  = csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle;
            CsrUieHandle sk1Handle      = csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle;

            /* Save the event handle as Back, and set input mode to blocked     */
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_HFG_CS_PLACE_INCOMING_HFG_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_CH_PLACE_INCOMING1_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_CS_CALL_LIST_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_MAIN_CALL_LIST_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HFG_CS_PLACE_OUTGOING_HFG_KEY,
                            CSR_UI_ICON_NONE, TEXT_HFG_CH_PLACE_OUTGOING_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            /* Create  CSR_BT_HFG_AT_MODE_CFG_CMDS_UI                                        */
            csrUiVar->uiIndex = CSR_BT_HFG_AT_MODE_CFG_CMDS_UI;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);
        }
    }
}
static void appCreateLdnInfoInputUi(hfgInstance_t * inst)
{ /* This function Creates the CSR_BT_HFG_LDN_INPUT_DIALG_UI                               */

    CsrBtHfgCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_INPUTDIALOG)
    {
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerGetInputDialogMisc;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    { /* Save the event handle as SK1/BACK, and set input mode to blocked       */
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle      = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle     = prim->handle;

        CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

    }
}


static const CsrBtHfgEventType csrUiHfgUieCreateCfmHandlers[NUMBER_OF_CSR_BT_HFG_APP_UI] =
{
    appCreateHfgMainMenuUi,                              /* CSR_BT_HFG_MAIN_MENU_UI           */
    appCreateHfgCallSimulateUi,         /* CSR_BT_HFG_CALL_SIMULATE_UI */
    appCreateHfgATCmdModeUi,               /* CSR_BT_HFG_AT_CMD_MODE_UI                   */
    appCreateHfgCallHandlingMenuUi,      /* CSR_BT_HFG_CALL_HANDLING_UI                   */
    appCreateHfgCallListMenuUi,        /* CSR_BT_HFG_CALL_LIST_MENU_UI                   */
    appCreateHfgC2CMenuUi,                      /* CSR_BT_HFG_C2C_UI              */
    appCreateHfgSettingIndicatorsUi,                        /* CSR_BT_HFG_SETTINGS_INDICATORS_UI */
    appCreateHfgScoEscoUi,                        /* CSR_BT_HFG_SCO_ESCO_UI */
    appCreateDiscDialogUi,                /* CSR_BT_HFG_DISC_DIALOG_UI */
    appCreateAlreadyConDialogUi,                /*  CSR_BT_HFG_ALREADY_CON_DIALOG_UI    */
    appCreateCancelConDialogUi,                 /*CSR_BT_HFG_CANCEL_CON_DIALOG_UI  */
    appCreateSuccConDialogUi,                   /*    CSR_BT_HFG_SUCC_CON_DIALOG_UI */
    appCreateHfgATModeCfgUi,            /* CSR_BT_HFG_AT_MODE_CFG_CMDS_UI                   */
    appCreateHfgChangeCallParamUi,    /* CSR_BT_HFG_CALL_LIST_CHNGE_PARM_UI                   */
    appCreateHfgGlobalSettingIndicatorsUi,        /* CSR_BT_HFG_SETTINGS_IND_GLOBAL_UI */
    appCreateHfgCurActiveSettingIndicatorsUi,    /* CSR_BT_HFG_SETTINGS_IND_ACTIVE_UI */
    appCreateC2CCallerNameInputUi,     /*CSR_BT_HFG_C2C_CALLER_NAME_UI */
    appCreateC2CTextStringInputUi,     /*CSR_BT_HFG_C2C_TEXT_STRING_UI */
    appCreateHfgSetupAudioCodeSrUi,            /* CSR_BT_HFG_SETUP_AUDIO_CODEC_SR_UI                   */
    appCreateScoEscoInputUi,            /*CSR_BT_HFG_SCO_ESCO_INPUT_UI */
    appCreateHfgDialMenuUi,                           /* CSR_BT_HFG_DIAL_IND_UI */
    appCreateDialInfoDialogUi,       /* CSR_BT_HFG_DIAL_INFO_DIALOG_UI */
    appCreateDefaultInfoDialogUi,   /* CSR_BT_HFG_DEFAULT_INFO_UI           */
    appCreateLdnInfoInputUi,    /* CSR_BT_HFG_LDN_INPUT_DIALG_UI */
    appCreateChangeSupportFeatUi,    /* CSR_BT_HFG_CHANGE_SUP_FEAT_UI */
    appCreateSupportFeatUi,    /* CSR_BT_HFG_SUP_FEAT_UI */
};

/* CSR_UI upstream handler functions                                              */
void CsrBtHfgCsrUiUieCreateCfmHandler(hfgInstance_t * inst)
{
    if (inst->csrUiVar.uiIndex < NUMBER_OF_CSR_BT_HFG_APP_UI)
    {
         csrUiHfgUieCreateCfmHandlers[inst->csrUiVar.uiIndex](inst);
    }
    else
    { /* An unexpected number of CSR_UI CsrUiUieCreateCfm messages
         has been received                                                      */
    }
}

void CsrBtHfgCsrUiInputdialogGetCfmHandler(hfgInstance_t* inst)
{
    CsrUiInputdialogGetCfm * prim = (CsrUiInputdialogGetCfm *) inst->recvMsgP;
    CsrCharString buf[40];

    if (prim->handle == inst->csrUiVar.displayesHandlers[CSR_BT_HFG_LDN_INPUT_DIALG_UI].displayHandle)
    {
        CsrUint8 *ldn = CsrUtf16String2Utf8(prim->text);
        CsrStrCpy((char*)inst->redialNumber, (char*)ldn);
        if(CsrStrLen((char*)ldn))
            sprintf(buf, "Last Dialled Number Set!!");
        else
            sprintf(buf, "Last Dialled Number is not entered!!");
        CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                                CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
        CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
        CsrUiUieHideReqSend(inst->csrUiVar.displayesHandlers[CSR_BT_HFG_LDN_INPUT_DIALG_UI].displayHandle);
    }
    else if (prim->handle == inst->csrUiVar.displayesHandlers[CSR_BT_HFG_C2C_CALLER_NAME_UI].displayHandle)
    {
        CsrUint8 *callerName = CsrUtf16String2Utf8(prim->text);

        CsrStrCpy((char*)inst->name, (char*)callerName);
        inst->nameLen = CsrStrLen((char*)callerName);
        if(inst->nameLen)
            sprintf(buf, "C2C name is set!!");
        else
            sprintf(buf, "C2C name is not entered!!");

        CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                                CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
        CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
        CsrUiUieHideReqSend(inst->csrUiVar.displayesHandlers[CSR_BT_HFG_C2C_CALLER_NAME_UI].displayHandle);
    }
    else if (prim->handle == inst->csrUiVar.displayesHandlers[CSR_BT_HFG_C2C_TEXT_STRING_UI].displayHandle)
    {
#ifdef CSR_BT_INSTALL_HFG_C2C
        Connection_t *con;
        CsrUint8 *c2cString = CsrUtf16String2Utf8(prim->text);

        CsrStrCpy((char*)inst->text, (char*)c2cString);
        inst->textLen = CsrStrLen((char*)c2cString);

        con = getActiveConnection(inst);
        if(inst->textLen)
        {
            sprintf(buf, "C2C Text Req is sent!!");
            if(con != NULL)
            {
                CsrBtHfgC2cTxtReqSend(con->index,
                                 (CsrCharString *) StringDup((char*)inst->text));
                inst->textLen = 0;
                inst->text[0] = '\0';
            }
        }
        else
        {
            sprintf(buf, "C2C Text is not entered!!");
        }

        CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                                CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
        CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
#else
        sprintf((char*)buf, "CSR2CSR features disabled in build!!\n");
        CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                                CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
        CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
#endif
        CsrUiUieHideReqSend(inst->csrUiVar.displayesHandlers[CSR_BT_HFG_C2C_TEXT_STRING_UI].displayHandle);
    }
    else if(prim->handle == inst->csrUiVar.displayesHandlers[CSR_BT_HFG_SCO_ESCO_INPUT_UI].displayHandle)
    {
        CsrUint8 *scoEscoInput = CsrUtf16String2Utf8(prim->text);

        CsrStrCpy((char*)inst->input, (char*)scoEscoInput);
        inst->inputLength= CsrStrLen((char*)scoEscoInput);
        if(inst->inputLength)
        {
#ifndef EXCLUDE_CSR_BT_HFG_MODULE_OPTIONAL
            sprintf(buf, "SCO Config Done!!");
            handleScoInput(inst, inst->scoKey);
#else
            sprintf(buf, "SCO Config IGNORED!! (EXCLUDE_CSR_BT_HFG_MODULE_OPTIONAL is set)");
#endif
        }
        else
        {
            sprintf(buf, "SCO Config is not entered!!");
        }
        CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                                CONVERT_TEXT_STRING_2_UCS2("SCO Config Done!!"), TEXT_OK_UCS2, NULL);
        CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
        CsrUiUieHideReqSend(inst->csrUiVar.displayesHandlers[CSR_BT_HFG_SCO_ESCO_INPUT_UI].displayHandle);
    }
    else
    { /* An unexpected CsrUiInputdialogGetCfm is received                        */
        CsrGeneralException("CSR_BT_HFG", 0, prim->type,
                           "Unexpected CsrUiInputdialogGetCfm is received");
    }

    CsrPmemFree(prim->text);
}

void CsrBtHfgCsrUiDisplayGetHandleCfmHandler(hfgInstance_t* inst)
{
    CsrUiDisplayGethandleCfm *prim = (CsrUiDisplayGethandleCfm*) inst->recvMsgP;

    if(inst->csrUiVar.goToMainmenu && inst->csrUiVar.inHfgMenu
            && (prim->handle != inst->csrUiVar.displayesHandlers[CSR_BT_HFG_MAIN_MENU_UI].displayHandle))
    {
        CsrUiUieHideReqSend(prim->handle);
        CsrUiDisplayGethandleReqSend(CsrSchedTaskQueueGet());
    }
    else if(prim->handle == inst->csrUiVar.displayesHandlers[CSR_BT_HFG_MAIN_MENU_UI].displayHandle)
    {
        updateMainMenu(inst);
        inst->csrUiVar.goToMainmenu = FALSE;
    }
}

void CsrBtHfgCsrUiEventIndHandler(hfgInstance_t * inst)
{ /* A CsrUiEventInd is received from the CSR_UI layer                             */
    CsrUint8     i;

    CsrBtHfgDisplayHandlesType * uiHandlers = inst->csrUiVar.displayesHandlers;
    CsrUiEventInd            * prim       = (CsrUiEventInd *) inst->recvMsgP;

    for ( i = 0; i < NUMBER_OF_CSR_BT_HFG_APP_UI; i++)
    { /* Go through the list of known displayes handles to find the one
         that is equal to the received prim->displayElement                     */
        if (prim->displayElement == uiHandlers[i].displayHandle)
        { /* Found a displayed handle that match                                */
            if (prim->event == uiHandlers[i].sk1EventHandle)
            { /* The event is a SK1                                             */
                if (uiHandlers[i].sk1EventHandleFunc)
                { /* Called the function that is linked to the event            */
                    uiHandlers[i].sk1EventHandleFunc(inst,
                                prim->displayElement, prim->event, prim->key);
                }
                else
                { /* An exception has occurred. No function is assign to
                     this event                                                 */
                    CsrGeneralException("CSR_BT_HFG", 0, prim->type,
                        "No function is assign to this SK1 event");
                }
            }
            else if (prim->event == uiHandlers[i].sk2EventHandle)
            { /* The event is a SK2                                             */
                if (uiHandlers[i].sk2EventHandleFunc)
                { /* Called the function that is linked to the event            */
                    uiHandlers[i].sk2EventHandleFunc(inst,
                                prim->displayElement, prim->event, prim->key);
                }
                else
                { /* An exception has occurred. No function is assign to
                     this event                                                 */
                    CsrGeneralException("CSR_BT_HFG", 0, prim->type,
                        "No function is assign to this SK2 event");
                }
            }
            else if (prim->event == uiHandlers[i].backEventHandle)
            { /* The event is a BACK                                            */
                if (uiHandlers[i].backEventHandleFunc)
                { /* Called the function that is linked to the event            */
                    uiHandlers[i].backEventHandleFunc(inst,
                                prim->displayElement, prim->event, prim->key);
                }
                else
                { /* An exception has occurred. No function is assign to
                     this event                                                 */
                    CsrGeneralException("CSR_BT_HFG", 0, prim->type,
                        "No function is assign to this BACK event");
                }
            }
            else if (prim->event == uiHandlers[i].deleteEventHandle)
            { /* The event is a DEL                                             */
                if (uiHandlers[i].deleteEventHandleFunc)
                { /* Called the function that is linked to the event            */
                    uiHandlers[i].deleteEventHandleFunc(inst,
                                          prim->displayElement, prim->event, prim->key);
                }
                else
                { /* An exception has occurred. No function is assign to
                     this event                                                 */
                    CsrGeneralException("CSR_BT_HFG", 0, prim->type,
                        "No function is assign to this DEL event");
                }
            }
            else
            { /* An exception has occurred. No UIEHandle is assign to
                 this event                                                     */
                    CsrGeneralException("CSR_BT_HFG", 0, prim->type,
                        "No UIEHandle is assign to this event");
            }
            return;
        }
        else
        { /* Continue                                                           */
            ;
        }
    }

    /* An exception has occurred. None of the know displayes handles match
       the received                                                             */
    CsrGeneralException("CSR_BT_HFG", 0, prim->type,
        "None of the know displayes handles match the received one");
}

