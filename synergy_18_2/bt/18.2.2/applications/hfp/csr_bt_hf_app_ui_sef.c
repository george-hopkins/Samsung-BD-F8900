/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #3 $
****************************************************************************/

#include "csr_synergy.h"
#include <stdio.h>

#include "csr_bt_hf_app_ui_sef.h"
#include "csr_bt_hf_app_task.h"
#include "csr_ui_lib.h"
#include "csr_pmem.h"
#include "csr_formatted_io.h"
#include "csr_bt_util.h"
#include "csr_exceptionhandler.h"
#include "csr_bt_hf_lib.h"
#include "csr_bt_hf_demo_app.h"
#include "csr_bt_hf_app_ui_strings.h"
#include "csr_bt_cm_prim.h"
#include "csr_bccmd_lib.h"
#include "csr_bt_sd_lib.h"
#include "csr_bt_gap_app_lib.h"

#define HF_DISC_GAURD (20*CSR_SCHED_SECOND)
#define HF_DEACT_GAURD (20*CSR_SCHED_SECOND)

void CsrBtHfDeactTimeout(CsrUint16 mi, void *mv)
{
    DemoInstdata_t *inst;
    CsrBtHfCsrUiType    * csrUiVar;

    inst = (DemoInstdata_t*) mv;
    csrUiVar  = &(inst->csrUiVar);

    initInstanceData(inst);
    inst->serverActivated = FALSE;

    CsrUiUieHideReqSend(csrUiVar->displayesHandlers[CSR_BT_HF_DEFAULT_INFO_UI].displayHandle);
    CsrBtHfSetDialog(inst, CSR_BT_HF_DEFAULT_INFO_UI, TEXT_FAILED_UCS2,
                                    CONVERT_TEXT_STRING_2_UCS2("Deactivate confirm is not recevied!!"), TEXT_OK_UCS2, NULL);
    /* Show the CSR_BT_HF_DEFAULT_INFO_UI on the display                   */
    CsrBtHfShowUi(inst, CSR_BT_HF_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HF_STD_PRIO);

    updateHfMainMenu(inst);
}

#if 0
void CsrBtHfDiscTimeout(CsrUint16 mi, void *mv)
{
    DemoInstdata_t *instData;
    CsrCharString buf[40];

    instData = (DemoInstdata_t*) mv;

    instData->conInstData[instData->currentConnection].startup = STARTUP_MIC;
    instData->conInstData[instData->currentConnection].audioOn = FALSE;
    instData->conInstData[instData->currentConnection].linkState = disconnected_s;
    instData->conInstData[instData->currentConnection].conId    = 0xFF;
    instData->conInstData[instData->currentConnection].incScoPending = FALSE;
    if(instData->conInstData[instData->currentConnection].cindString)
        CsrPmemFree(instData->conInstData[instData->currentConnection].cindString);
    if(instData->conInstData[instData->currentConnection].cindValues)
        CsrPmemFree(instData->conInstData[instData->currentConnection].cindValues);
    instData->conInstData[instData->currentConnection].indicatorActivation = instData->conInstData[instData->currentConnection].tmpIndicatorActivation = 0xFFFF;

    findFirstActiveConnection(instData);
    sprintf(buf, "Disconnect confirm is not recevied!!");
    updateHfMainMenu(instData);

    CsrBtHfSetDialog(instData, CSR_BT_HF_DEFAULT_INFO_UI, TEXT_FAILED_UCS2,
                                    CONVERT_TEXT_STRING_2_UCS2("Disconnect confirm is not recevied!!"), TEXT_OK_UCS2, NULL);
    /* Show the CSR_BT_HF_DEFAULT_INFO_UI on the display                   */
    CsrBtHfShowUi(instData, CSR_BT_HF_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HF_STD_PRIO);


}
#endif

/* Static functions used by the CSR_UI upstream handler functions                 */
static void appUiKeyHandlerHideMics(void * instData,
                    CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* An key event function used to hide the showed UI. This function is used
     by many different APP UI's                                              */
    DemoInstdata_t *inst = instData;
    CsrBtHfCsrUiType    * csrUiVar  = &(inst->csrUiVar);

    CsrUiUieHideReqSend(displayHandle);
    /*if the hf main menu is hidden, we are going out of HF menu*/
     if(displayHandle == csrUiVar->displayesHandlers[CSR_BT_HF_MAIN_MENU_UI].displayHandle)
        csrUiVar->inHfMenu = FALSE;

    CSR_UNUSED(eventHandle);
    CSR_UNUSED(instData);
    CSR_UNUSED(key);
}

static void appHfUiKeyHandlerGetInputDialogMisc(void * instData,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from UI's with input dialogs */
    CSR_UNUSED(instData);
    CSR_UNUSED(eventHandle);
    CSR_UNUSED(key);

    CsrUiInputdialogGetReqSend(CsrSchedTaskQueueGet(), displayHandle);
}


/* Static functions used by the CSR_UI upstream handler functions                 */
static void appUiKeyHandlerHfDefaultInfoDialog(void * inst,
                    CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* An key event function used to hide the showed UI. This function is used
     by many different APP UI's                                              */
    DemoInstdata_t *instData = inst;
    CsrBtHfCsrUiType    * csrUiVar  = &(instData->csrUiVar);
    CsrCharString buf[70];
    CSR_UNUSED(eventHandle);
    CSR_UNUSED(key);

    switch(instData->state)
    {
        case cancelState:
        {
            if (instData->connReqPending)
            {/*  cancel connect request */
                BD_ADDR_T       theAddr;
                extern BD_ADDR_T defGlobalBdAddr;
                if ((instData->remoteAddr.lap == 0) &&
                       (instData->remoteAddr.uap == 0) &&
                       (instData->remoteAddr.nap == 0))
                {
                    if ((defGlobalBdAddr.lap != 0) ||
                       (defGlobalBdAddr.uap != 0) ||
                       (defGlobalBdAddr.nap != 0))
                    {
                        theAddr.lap = defGlobalBdAddr.lap;
                        theAddr.uap = defGlobalBdAddr.uap;
                        theAddr.nap = defGlobalBdAddr.nap;
                    }
                }
                else
                {
                    theAddr.lap = instData->remoteAddr.lap;
                    theAddr.uap = instData->remoteAddr.uap;
                    theAddr.nap = instData->remoteAddr.nap;
                }


                sprintf(buf, "Send cancel connect request towards: %04X:%02X:%06X\n", theAddr.nap, theAddr.uap, theAddr.lap);
                CsrBtHfCancelConnectReqSend(theAddr);
            }
            else
            {
                sprintf(buf, "No connection pending!\n");
            }
            CsrBtHfSetDialog(instData, CSR_BT_HF_DEFAULT_INFO_UI, CONVERT_TEXT_STRING_2_UCS2("Cancel SLC"),
                                        CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
            CsrBtHfShowUi(instData, CSR_BT_HF_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HF_STD_PRIO);
            break;
        }
        case acc_rej_audio:
        {
            CsrBtHfAudioAcceptResSend(instData->conInstData[instData->currentConnection].conId, HCI_SUCCESS, NULL, PCM_SLOT, PCM_SLOT_REALLOCATE);
            instData->conInstData[instData->currentConnection].incScoPending = FALSE;
            sprintf(buf, "Audio Connection is Accepted!\n");
            CsrBtHfSetDialog(instData, CSR_BT_HF_DEFAULT_INFO_UI, CONVERT_TEXT_STRING_2_UCS2("Audio Connection Setup"),
                                        CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
            CsrBtHfShowUi(instData, CSR_BT_HF_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HF_STD_PRIO);
            break;
        }
        default:
        {
            CsrUiUieHideReqSend(displayHandle);
            break;
        }
    }

    /*if the hf main menu is hidden, we are going out of HF menu*/
     if(displayHandle == csrUiVar->displayesHandlers[CSR_BT_HF_MAIN_MENU_UI].displayHandle)
        csrUiVar->inHfMenu = FALSE;
    instData->state = idle;
}


static void appUiKeyHandlerHfDefaultInfoDialogSk2(void * inst,
                    CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{
    DemoInstdata_t *instData = inst;
    CsrCharString buf[32];
    CSR_UNUSED(eventHandle);
    CSR_UNUSED(key);

    switch(instData->state)
    {
        case acc_rej_audio:
        {
            CsrBtHfAudioAcceptResSend(instData->conInstData[instData->currentConnection].conId, HCI_ERROR_REJ_BY_REMOTE_PERS, NULL, PCM_SLOT, PCM_SLOT_REALLOCATE);
            instData->conInstData[instData->currentConnection].incScoPending = FALSE;
            sprintf(buf, "Audio Connection is rejected!\n");
            CsrBtHfSetDialog(instData, CSR_BT_HF_DEFAULT_INFO_UI, CONVERT_TEXT_STRING_2_UCS2("Audio Connection Setup"),
                                        CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
            CsrBtHfShowUi(instData, CSR_BT_HF_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HF_STD_PRIO);
            break;
        }
        default:
        {
            break;
        }
    }
    instData->state = idle;
}

static void appCreateHfDefaultInfoDialogUi(DemoInstdata_t * inst)
{ /* This function Creates the CSR_BT_HF_DEFAULT_INFO_UI                               */

    CsrBtHfCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_DIALOG)
    {
        csrUiVar->eventState                                              = CSR_BT_HF_CREATE_SK1_EVENT;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerHfDefaultInfoDialog;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk2EventHandleFunc  = appUiKeyHandlerHfDefaultInfoDialogSk2;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHfDefaultInfoDialog;

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    { /* Save the event handle as SK1/BACK, and set input mode to blocked       */
        if(csrUiVar->eventState == CSR_BT_HF_CREATE_SK1_EVENT)
        {
            csrUiVar->eventState                                          = CSR_BT_HF_CREATE_SK2_EVENT;
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle      = prim->handle;
            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else if(csrUiVar->eventState == CSR_BT_HF_CREATE_SK2_EVENT)
        {
            csrUiVar->eventState                                          = CSR_BT_HF_CREATE_BACK_EVENT;
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk2EventHandle      = prim->handle;
            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else
        {
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk2EventHandle      = prim->handle;
            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());
            csrUiVar->uiIndex = CSR_BT_HF_REACTIVATE_SERVER_UI;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);
        }
    }
}

static void appUiKeyHandlerReactServerMenu(void * instData,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from the CSR_BT_HF_REACTIVATE_SERVER_UI */
    DemoInstdata_t* inst = instData;
    CsrBtHfCsrUiType    * csrUiVar  = &(inst->csrUiVar);

    CSR_UNUSED(eventHandle);

    inst->reactKey = (CsrUint8) key;

    switch (key)
    {
        case CSR_BT_HF_MAX_NUM_HF_RECORDS_KEY:
        {
            CsrBtHfSetInputDialog(inst, CSR_BT_HF_DEFAULT_INPUT_UI,
                CONVERT_TEXT_STRING_2_UCS2("Max HF Records Setting"), CONVERT_TEXT_STRING_2_UCS2("Enter max number of HF Records:"),
                CSR_UI_ICON_KEY, NULL, 1,
                CSR_UI_KEYMAP_NUMERIC,TEXT_OK_UCS2, NULL);
            break;
        }
        case CSR_BT_HF_MAX_NUM_HS_RECORDS_KEY:
        {
            CsrBtHfSetInputDialog(inst, CSR_BT_HF_DEFAULT_INPUT_UI,
                CONVERT_TEXT_STRING_2_UCS2("Max HS Records Setting"), CONVERT_TEXT_STRING_2_UCS2("Enter max number of HS Records:"),
                CSR_UI_ICON_KEY, NULL, 1,
                CSR_UI_KEYMAP_NUMERIC,TEXT_OK_UCS2, NULL);
            break;
        }
        case CSR_BT_HF_MAX_NUM_SIMULT_CON_KEY:
        {
            CsrBtHfSetInputDialog(inst, CSR_BT_HF_DEFAULT_INPUT_UI,
                CONVERT_TEXT_STRING_2_UCS2("Max Simultaneous Con"), CONVERT_TEXT_STRING_2_UCS2("Enter max number of simultaneous connections:"),
                CSR_UI_ICON_KEY, NULL, 1,
                CSR_UI_KEYMAP_NUMERIC,TEXT_OK_UCS2, NULL);
            break;
        }
        case CSR_BT_HF_SEND_ACT_HS_HF_1_SIMULT2_KEY:
        {
            CsrBtHfActivateReqSend(inst->app_hdl, inst->maxHFrecords,inst->maxHSrecords,
                      inst->maxSimulCons,inst->conInstData[inst->currentConnection].localSupportedFeatures,
                      CSR_BT_HF_CNF_DISABLE_AUTOMATIC_CLIP_ACTIVATION|CSR_BT_HF_CNF_DISABLE_AUTOMATIC_CCWA_ACTIVATION|CSR_BT_HF_CNF_DISABLE_AUTOMATIC_CMEE_ACTIVATION,5);
            CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);
            return;
        }
        default :
        {
            CsrGeneralException("CSR_BT_HF", 0, 0,
            "Unexpected menu key is received");
            break;
        }
    }
    inst->state = enteringReactParam;
    CsrBtHfShowUi(inst, CSR_BT_HF_DEFAULT_INPUT_UI, CSR_UI_INPUTMODE_AUTO, 1);
    CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);
    CsrUiDisplaySetinputmodeReqSend(csrUiVar->displayesHandlers[CSR_BT_HF_DEFAULT_INPUT_UI].displayHandle,
                                                                CSR_UI_INPUTMODE_AUTO);
}
static void appCreateHfReactivateServerUi(DemoInstdata_t * inst)
{ /* This function Creates the CSR_BT_HF_REACTIVATE_SERVER_UI                              */

    CsrBtHfCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_MENU)
    {
        csrUiVar->eventState                                              = CSR_BT_HF_CREATE_SK1_EVENT;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerReactServerMenu;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Set the menu on the CSR_BT_HF_REACTIVATE_SERVER_UI                               */
        CsrUiMenuSetReqSend(prim->handle, TEXT_HF_REACTIVATE_SERVER_UCS2, TEXT_SELECT_UCS2, NULL);

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    {
        if (csrUiVar->eventState == CSR_BT_HF_CREATE_SK1_EVENT)
        { /* Save the event handle as SK1, and set input mode to blocked        */

            csrUiVar->eventState                                          = CSR_BT_HF_CREATE_BACK_EVENT;
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Create a BACK event                                              */
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else
        { /* CSR_BT_HF_CREATE_BACK_EVENT                                           */

            CsrUieHandle displayHandle  = csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle;
            CsrUieHandle sk1Handle      = csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle;

            /* Save the event handle as Back, and set input mode to blocked     */
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_HF_MAX_NUM_HF_RECORDS_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_MAX_NUM_HF_RECORDS_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_MAX_NUM_HS_RECORDS_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_MAX_NUM_HS_RECORDS_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_MAX_NUM_SIMULT_CON_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_MAX_NUM_SIMULT_CON_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_SEND_ACT_HS_HF_1_SIMULT2_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_SEND_ACT_HS_HF_1_SIMULT2_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            csrUiVar->uiIndex = CSR_BT_REQUEST_SCO_SPL_SETTINGS_UI;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);

        }
    }
}

static void appUiKeyHandlerScoSplSettingsMenu(void * instData,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from the CSR_BT_REQUEST_SCO_SPL_SETTINGS_UI */
    DemoInstdata_t* inst = instData;
    CsrCharString buf[60];

#ifndef EXCLUDE_CSR_BT_HF_MODULE_OPTIONAL

    CsrBtHfAudioLinkParameterListConfig  *audSetting;

    audSetting = (CsrBtHfAudioLinkParameterListConfig  *)CsrPmemAlloc(sizeof(CsrBtHfAudioLinkParameterListConfig));
    audSetting->packetType = SCO_PACKET_TYPE;
    audSetting->txBandwidth = SCO_TX_BANDWIDTH;
    audSetting->rxBandwidth = SCO_RX_BANDWIDTH;
    audSetting->maxLatency = SCO_MAX_LATENCY;
    audSetting->voiceSettings = SCO_VOICE_SETTINGS;
    audSetting->reTxEffort = SCO_RETRANSMISSION_EFFORT;

    CSR_UNUSED(eventHandle);


    switch (key)
    {
        case CSR_BT_HF_ESCO_18MS_LATENCY_KEY:
        {
                /* Latency 18 msec. */
                audSetting->maxLatency = 18;

                CsrBtHfAudioConfigReqSend(inst->conInstData[inst->currentConnection].conId,
                                    CSR_BT_HF_AUDIO_OUTGOING_PARAMETER_LIST_CONFIG,
                                    (void *)audSetting,
                                    sizeof(CsrBtHfAudioLinkParameterListConfig));

                CsrBtHfAudioConnectReqSend(inst->conInstData[inst->currentConnection].conId, 0,NULL,
                               PCM_SLOT,
                               PCM_SLOT_REALLOCATE);
                sprintf(buf, "Request audio on, with CSRMAX latency = 18 msec \n");
                break;
        }
        case CSR_BT_HF_ESCO_RXTX_BW_6K_KEY:
        {
                /* Rx-/Txbandwidth = 6000 */
                audSetting->txBandwidth = 6000;
                audSetting->rxBandwidth = 6000;

                CsrBtHfAudioConfigReqSend(inst->conInstData[inst->currentConnection].conId,
                                    CSR_BT_HF_AUDIO_OUTGOING_PARAMETER_LIST_CONFIG,
                                    (void *)audSetting,
                                    sizeof(CsrBtHfAudioLinkParameterListConfig));

                CsrBtHfAudioConnectReqSend(inst->conInstData[inst->currentConnection].conId, 0,NULL,
                               PCM_SLOT,
                               PCM_SLOT_REALLOCATE);
                sprintf(buf, "Request audio on, with Rx-Tx bandwith = 6000 \n");
                break;
        }
        case CSR_BT_HF_ESCO_RXTX_BW_12K_KEY:
        {
                /* Rx-/Txbandwidth = 12000 */
                audSetting->txBandwidth = 12000;
                audSetting->rxBandwidth = 12000;

                CsrBtHfAudioConfigReqSend(inst->conInstData[inst->currentConnection].conId,
                                    CSR_BT_HF_AUDIO_OUTGOING_PARAMETER_LIST_CONFIG,
                                    (void *)audSetting,
                                    sizeof(CsrBtHfAudioLinkParameterListConfig));

                CsrBtHfAudioConnectReqSend(inst->conInstData[inst->currentConnection].conId, 0,NULL,
                               PCM_SLOT,
                               PCM_SLOT_REALLOCATE);
                sprintf(buf, "Request audio on, with Rx-Tx bandwith = 12000 \n");
                break;
        }
        case CSR_BT_HF_ESCO_S1_SETTINGS_KEY:
        {
            audSetting->packetType = CSR_BT_ESCO_DEFAULT_1P2_S1_AUDIO_QUALITY;
            audSetting->txBandwidth = CSR_BT_ESCO_DEFAULT_1P2_S1_TX_BANDWIDTH;
            audSetting->rxBandwidth = CSR_BT_ESCO_DEFAULT_1P2_S1_RX_BANDWIDTH;
            audSetting->maxLatency = CSR_BT_ESCO_DEFAULT_1P2_S1_MAX_LATENCY;
            audSetting->voiceSettings = CSR_BT_ESCO_DEFAULT_1P2_S1_VOICE_SETTINGS;
            audSetting->reTxEffort = CSR_BT_ESCO_DEFAULT_1P2_S1_RE_TX_EFFORT;

            CsrBtHfAudioConfigReqSend(inst->conInstData[inst->currentConnection].conId,
                                CSR_BT_HF_AUDIO_OUTGOING_PARAMETER_LIST_CONFIG,
                                (void *)audSetting,
                                sizeof(CsrBtHfAudioLinkParameterListConfig));

            CsrBtHfAudioConnectReqSend(inst->conInstData[inst->currentConnection].conId, 0,NULL,
                           PCM_SLOT,
                           PCM_SLOT_REALLOCATE);
            sprintf(buf, "Request audio on, with S1 settings\n");
            break;
        }
        case CSR_BT_HF_SCO_SETTINGS_KEY:
        {
            audSetting->packetType = CSR_BT_SCO_DEFAULT_1P1_AUDIO_QUALITY;
            audSetting->txBandwidth = CSR_BT_SCO_DEFAULT_1P1_TX_BANDWIDTH;
            audSetting->rxBandwidth = CSR_BT_SCO_DEFAULT_1P1_RX_BANDWIDTH;
            audSetting->maxLatency = CSR_BT_SCO_DEFAULT_1P1_MAX_LATENCY;
            audSetting->voiceSettings = CSR_BT_SCO_DEFAULT_1P1_VOICE_SETTINGS;
            audSetting->reTxEffort = CSR_BT_SCO_DEFAULT_1P1_RE_TX_EFFORT;

            CsrBtHfAudioConfigReqSend(inst->conInstData[inst->currentConnection].conId,
                                CSR_BT_HF_AUDIO_OUTGOING_PARAMETER_LIST_CONFIG,
                                (void *)audSetting,
                                sizeof(CsrBtHfAudioLinkParameterListConfig));

            CsrBtHfAudioConnectReqSend(inst->conInstData[inst->currentConnection].conId, 0,NULL,
                           PCM_SLOT,
                           PCM_SLOT_REALLOCATE);
            sprintf(buf, "Request audio on, with S1 settings\n");
            break;
        }
        default :
        {
            CsrGeneralException("CSR_BT_HF", 0, 0,
            "Unexpected menu key is received");
            break;
        }
    }
    CsrBtHfSetDialog(inst, CSR_BT_HF_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                            CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
#else
    sprintf((char*)buf, "CsrBtHfAudioConfigReqSend NOT sent as EXCLUDE_CSR_BT_HF_MODULE_OPTIONAL is set\n");
    CsrBtHfSetDialog(inst, CSR_BT_HF_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                            CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
#endif /* EXCLUDE_CSR_BT_HF_MODULE_OPTIONAL */
    /* Show the CSR_BT_HF_DEFAULT_INFO_UI on the display                   */
    CsrBtHfShowUi(inst, CSR_BT_HF_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HF_STD_PRIO);
    CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);

}

static void appCreateHfReqScoSplSettingsUi(DemoInstdata_t * inst)
{ /* This function Creates the CSR_BT_REQUEST_SCO_SPL_SETTINGS_UI                              */

    CsrBtHfCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_MENU)
    {
        csrUiVar->eventState                                              = CSR_BT_HF_CREATE_SK1_EVENT;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerScoSplSettingsMenu;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Set the menu on the CSR_BT_REQUEST_SCO_SPL_SETTINGS_UI                               */
        CsrUiMenuSetReqSend(prim->handle, TEXT_HF_REQUEST_SCO_SPL_SETTINGS_UCS2, TEXT_SELECT_UCS2, NULL);

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    {
        if (csrUiVar->eventState == CSR_BT_HF_CREATE_SK1_EVENT)
        { /* Save the event handle as SK1, and set input mode to blocked        */

            csrUiVar->eventState                                          = CSR_BT_HF_CREATE_BACK_EVENT;
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Create a BACK event                                              */
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else
        { /* CSR_BT_HF_CREATE_BACK_EVENT                                           */

            CsrUieHandle displayHandle  = csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle;
            CsrUieHandle sk1Handle      = csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle;

            /* Save the event handle as Back, and set input mode to blocked     */
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_HF_ESCO_18MS_LATENCY_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_ESCO_18MS_LATENCY_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_ESCO_RXTX_BW_6K_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_ESCO_RXTX_BW_6K_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_ESCO_RXTX_BW_12K_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_ESCO_RXTX_BW_12K_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_ESCO_S1_SETTINGS_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_ESCO_S1_SETTINGS_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_SCO_SETTINGS_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_SCO_SETTINGS_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            csrUiVar->uiIndex = CSR_BT_ENH_CALL_HANDLING_MENU_UI;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);

        }
    }
}

static void appUiKeyHandlerEnhCallHandlingMenu(void * inst,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from the CSR_BT_ENH_CALL_HANDLING_MENU_UI */
    DemoInstdata_t* instData = inst;
    CsrCharString buf[75];
    CsrBtHfCsrUiType    * csrUiVar  = &(instData->csrUiVar);
    CSR_UNUSED(eventHandle);


    switch (key)
    {
        case CSR_BT_HF_LIST_ACTIVE_HELD_CALLS_KEY:
        {
            sprintf(buf, "Querying List of Current Calls\n");
            CsrBtHfGetCurrentCallListReqSend(instData->conInstData[instData->currentConnection].conId);
            CsrUiMenuRemoveallitemsReqSend(csrUiVar->displayesHandlers[CSR_BT_HF_CALL_LIST_UI].displayHandle);
            instData->callIndex = 0;
            break;
        }
        case CSR_BT_HF_AT_CHLD_0_KEY:
        { /*Release all held calls(or set UDUB)*/
            sprintf(buf, "Release all held calls(or set UDUB\n");
            CsrBtHfCallHandlingReqSend(CSR_BT_RELEASE_ALL_HELD_CALL,0,instData->conInstData[instData->currentConnection].conId);
            break;
        }
        case CSR_BT_HF_AT_CHLD_1_KEY:
        {/*Release all active calls and accept held or waiting*/
            sprintf(buf, "Release all active calls and accept held or waiting\n");
            CsrBtHfCallHandlingReqSend(CSR_BT_RELEASE_ACTIVE_ACCEPT,0,instData->conInstData[instData->currentConnection].conId);
            break;
        }
        case CSR_BT_HF_AT_CHLD_2_KEY:
        {/*Place all active calls on hold and accept held or waiting*/
            sprintf(buf, "Place all active calls on hold and accept held or waiting\n");
            CsrBtHfCallHandlingReqSend(CSR_BT_HOLD_ACTIVE_ACCEPT,0,instData->conInstData[instData->currentConnection].conId);
            break;
        }
        case CSR_BT_HF_AT_CHLD_3_KEY:
        {/*Add held call to conversation*/
            sprintf(buf, "Add held call to conversation\n");
            CsrBtHfCallHandlingReqSend(CSR_BT_ADD_CALL,0,instData->conInstData[instData->currentConnection].conId);
            break;
        }
        case CSR_BT_HF_AT_CHLD_4_KEY:
        {/*Connect two calls and disconnect from both (Explicit Call Transfer)*/
            sprintf(buf, "Connect two calls and disconnect from both (Explicit Call Transfer)\n");
            CsrBtHfCallHandlingReqSend(CSR_BT_CONNECT_TWO_CALLS,0,instData->conInstData[instData->currentConnection].conId);
            break;
        }
        default :
        {
            CsrGeneralException("CSR_BT_HF", 0, 0,
            "Unexpected menu key is received");
            break;
        }
    }
    CsrBtHfSetDialog(instData, CSR_BT_HF_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                            CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
    /* Show the CSR_BT_HF_DEFAULT_INFO_UI on the display                   */
    CsrBtHfShowUi(instData, CSR_BT_HF_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HF_STD_PRIO);
    CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);

}
static void appCreateHfEnhCallHandlingUi(DemoInstdata_t * inst)
{ /* This function Creates the CSR_BT_ENH_CALL_HANDLING_MENU_UI                              */

    CsrBtHfCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_MENU)
    {
        csrUiVar->eventState                                              = CSR_BT_HF_CREATE_SK1_EVENT;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerEnhCallHandlingMenu;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Set the menu on the CSR_BT_ENH_CALL_HANDLING_MENU_UI                               */
        CsrUiMenuSetReqSend(prim->handle, TEXT_HF_ENH_CALL_HANDLING_MENU_UCS2, TEXT_SELECT_UCS2, NULL);

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    {
        if (csrUiVar->eventState == CSR_BT_HF_CREATE_SK1_EVENT)
        { /* Save the event handle as SK1, and set input mode to blocked        */

            csrUiVar->eventState                                          = CSR_BT_HF_CREATE_BACK_EVENT;
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Create a BACK event                                              */
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else
        { /* CSR_BT_HF_CREATE_BACK_EVENT                                           */

            CsrUieHandle displayHandle  = csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle;
            CsrUieHandle sk1Handle      = csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle;

            /* Save the event handle as Back, and set input mode to blocked     */
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_HF_LIST_ACTIVE_HELD_CALLS_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_LIST_ACTIVE_HELD_CALLS_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_AT_CHLD_0_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_AT_CHLD_0_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_AT_CHLD_1_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_AT_CHLD_1_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_AT_CHLD_2_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_AT_CHLD_2_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_AT_CHLD_3_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_AT_CHLD_3_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_AT_CHLD_4_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_AT_CHLD_4_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            csrUiVar->uiIndex = CSR_BT_RESPONSE_HOLD_OPTIONS_UI;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);

        }
    }
}


static void appUiKeyHandlerResHoldOptionsMenu(void * instData,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from the CSR_BT_RESPONSE_HOLD_OPTIONS_UI */
    DemoInstdata_t* inst = instData;
    CsrCharString buf[60];
    CSR_UNUSED(eventHandle);

    switch (key)
    {
        case CSR_BT_HF_QUERY_RES_HOLD_STATUS_KEY:
        {
            if (inst->conInstData[inst->currentConnection].linkType == CSR_BT_HF_CONNECTION_HF)
            {
                sprintf(buf, "Query response and hold status ...\n");
                CsrBtHfCallHandlingReqSend(CSR_BT_BTRH_READ_STATUS,0,inst->conInstData[inst->currentConnection].conId);
            }
            else
            {
                sprintf(buf, "Select active HF connection before requesting action\n");
            }
            break;
        }
        case CSR_BT_HF_PUT_INC_CALL_ON_HOLD_KEY:
        {
            sprintf(buf, "Put incoming call in hold ...\n");
            CsrBtHfCallHandlingReqSend(CSR_BT_BTRH_PUT_ON_HOLD,0,inst->conInstData[inst->currentConnection].conId);
            break;
        }
        case CSR_BT_HF_ACCEPT_INC_CALL_ON_HOLD_KEY:
        {
            if (inst->conInstData[inst->currentConnection].incScoPending)
            {
                CsrBtHfAudioAcceptResSend(inst->conInstData[inst->currentConnection].conId, HCI_ERROR_REJ_BY_REMOTE_PERS, NULL, PCM_SLOT, PCM_SLOT_REALLOCATE);
                inst->conInstData[inst->currentConnection].incScoPending = FALSE;
                sprintf(buf, "Hf Audio Accept Response with Error Sent!!");
            }
            else
            {
                sprintf(buf, "Accept held incoming call ...\n");
                CsrBtHfCallHandlingReqSend(CSR_BT_BTRH_ACCEPT_INCOMING,0,inst->conInstData[inst->currentConnection].conId);
            }
            break;
        }
        case CSR_BT_HF_REJ_INC_CALL_ON_HOLD_KEY:
        {
            if (inst->conInstData[inst->currentConnection].incScoPending)
            {
                CsrBtHfAudioAcceptResSend(inst->conInstData[inst->currentConnection].conId, HCI_ERROR_REJ_BY_REMOTE_PERS, NULL, PCM_SLOT, PCM_SLOT_REALLOCATE);
                inst->conInstData[inst->currentConnection].incScoPending = FALSE;
                sprintf(buf, "Hf Audio Accept Response with Error Sent!!");
            }
            else
            {
                sprintf(buf, "Reject held incoming call ...\n");
                CsrBtHfCallHandlingReqSend(CSR_BT_BTRH_REJECT_INCOMING,0,inst->conInstData[inst->currentConnection].conId);
            }
            break;
        }
        default :
        {
            CsrGeneralException("CSR_BT_HF", 0, 0,
            "Unexpected menu key is received");
            break;
        }
    }
    CsrBtHfSetDialog(inst, CSR_BT_HF_DEFAULT_INFO_UI, CONVERT_TEXT_STRING_2_UCS2("Response and Hold Options!!"),
                                CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
    CsrBtHfShowUi(inst, CSR_BT_HF_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HF_STD_PRIO);
    CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);
}

static void appCreateHfResHoldOptionsUi(DemoInstdata_t * inst)
{ /* This function Creates the CSR_BT_RESPONSE_HOLD_OPTIONS_UI                              */

    CsrBtHfCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_MENU)
    {
        csrUiVar->eventState                                              = CSR_BT_HF_CREATE_SK1_EVENT;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerResHoldOptionsMenu;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Set the menu on the CSR_BT_RESPONSE_HOLD_OPTIONS_UI                               */
        CsrUiMenuSetReqSend(prim->handle, TEXT_HF_RESPONSE_HOLD_OPTIONS_UCS2, TEXT_SELECT_UCS2, NULL);

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    {
        if (csrUiVar->eventState == CSR_BT_HF_CREATE_SK1_EVENT)
        { /* Save the event handle as SK1, and set input mode to blocked        */

            csrUiVar->eventState                                          = CSR_BT_HF_CREATE_BACK_EVENT;
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Create a BACK event                                              */
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else
        { /* CSR_BT_HF_CREATE_BACK_EVENT                                           */

            CsrUieHandle displayHandle  = csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle;
            CsrUieHandle sk1Handle      = csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle;

            /* Save the event handle as Back, and set input mode to blocked     */
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_HF_QUERY_RES_HOLD_STATUS_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_QUERY_RES_HOLD_STATUS_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_PUT_INC_CALL_ON_HOLD_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_PUT_INC_CALL_ON_HOLD_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_ACCEPT_INC_CALL_ON_HOLD_KEY,
                            CSR_UI_ICON_NONE, TEXTT_HF_ACCEPT_INC_CALL_ON_HOLD_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_REJ_INC_CALL_ON_HOLD_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_REJ_INC_CALL_ON_HOLD_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            csrUiVar->uiIndex = CSR_BT_CALL_HANDLING_MENU_UI;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);

        }
    }
}

static void appUiKeyHfCallHandlingMenu(void * instData,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from the CSR_BT_CALL_HANDLING_MENU_UI */
    DemoInstdata_t* inst = instData;
    CsrBtHfCsrUiType    * csrUiVar  = &(inst->csrUiVar);

    CsrCharString buf[200];

    CSR_UNUSED(eventHandle);

    inst->reactKey = (CsrUint8)key;

    switch (key)
    {
        case CSR_BT_HF_TOG_AUDIO_KEY:
        {
            /*  audio on/off */
            CsrBool   audioOn = FALSE;

            if (inst->conInstData[inst->currentConnection].linkType != CSR_BT_HF_CONNECTION_UNKNOWN)
            {
                if (inst->conInstData[inst->currentConnection].audioOn == TRUE)
                { /* Audio off*/
                    CsrBtHfAudioDisconnectReqSend(inst->conInstData[inst->currentConnection].conId, 0xFFFF);
                }
                else
                {/* audio On */
                    audioOn = TRUE;
                    CsrBtHfAudioConnectReqSend(inst->conInstData[inst->currentConnection].conId, 0,NULL,
                                   PCM_SLOT,
                                   PCM_SLOT_REALLOCATE);

                }

                sprintf(buf, "Request audio %s\n", (audioOn ? "ON" : "OFF"));
            }
            else
            {
                sprintf(buf, "Select active connection before requesting Audio change\n");
            }
            CsrBtHfSetDialog(inst, CSR_BT_HF_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                            CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
            CsrBtHfShowUi(inst, CSR_BT_HF_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HF_STD_PRIO);

            break;
        }
        case CSR_BT_HF_CALL_SPEC_NO_KEY:
        {
            CsrBtHfSetInputDialog(inst, CSR_BT_HF_DEFAULT_INPUT_UI,
                CONVERT_TEXT_STRING_2_UCS2("Dial Number"), CONVERT_TEXT_STRING_2_UCS2("Enter the mobile number to dial:"),
                CSR_UI_ICON_KEY, NULL, 50,
                CSR_UI_KEYMAP_CONTROLNUMERIC,TEXT_OK_UCS2, NULL);
            inst->state = dialing;
            CsrBtHfShowUi(inst, CSR_BT_HF_DEFAULT_INPUT_UI, CSR_UI_INPUTMODE_AUTO, 1);
            break;
        }
        case CSR_BT_HF_ANS_IN_CALL_KEY:
        {
            if (inst->conInstData[inst->currentConnection].linkType != CSR_BT_HF_CONNECTION_UNKNOWN)
            {
                CsrBtHfAnswerReqSend(inst->conInstData[inst->currentConnection].conId);
                sprintf(buf, "Answer incoming call\n");
            }
            else
            {
                sprintf(buf, "Select active connection before answering call\n");
            }
            CsrBtHfSetDialog(inst, CSR_BT_HF_DEFAULT_INFO_UI, CONVERT_TEXT_STRING_2_UCS2("HF Answer"),
                                            CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
            CsrBtHfShowUi(inst, CSR_BT_HF_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HF_STD_PRIO);

            break;
        }
        case CSR_BT_HF_REJ_IN_CALL_KEY:
        {
            CsrBtHfCallEndReqSend(inst->conInstData[inst->currentConnection].conId);
            sprintf(buf, "Reject incoming call Sent!!\n");
            CsrBtHfSetDialog(inst, CSR_BT_HF_DEFAULT_INFO_UI, CONVERT_TEXT_STRING_2_UCS2("HF Reject"),
                                            CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
            CsrBtHfShowUi(inst, CSR_BT_HF_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HF_STD_PRIO);
            break;
        }
        case CSR_BT_HF_CALL_LINE_ID_KEY:
        {
            if (inst->conInstData[inst->currentConnection].linkType == CSR_BT_HF_CONNECTION_HF)
            {
                sprintf(buf, "CLIP request send\n");
                CsrBtHfSetCallNotificationIndicationReqSend(inst->conInstData[inst->currentConnection].conId,TRUE);
            }
            else
            {
                sprintf(buf, "CLIP request is only applicable when HF connection is active\n");
            }
            CsrBtHfSetDialog(inst, CSR_BT_HF_DEFAULT_INFO_UI, CONVERT_TEXT_STRING_2_UCS2("CLIP"),
                                CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
            CsrBtHfShowUi(inst, CSR_BT_HF_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HF_STD_PRIO);
            break;
        }
        case CSR_BT_HF_LAST_NO_REDIAL_KEY:
        {
            if (inst->conInstData[inst->currentConnection].linkType == CSR_BT_HF_CONNECTION_HF)
            {
                CsrBtHfDialReqSend(inst->conInstData[inst->currentConnection].conId,CSR_BT_HF_DIAL_REDIAL,NULL);
                sprintf(buf, "Calling the last number dialed\n");
            }
            else
            {
                sprintf(buf, "BLDN request is only applicable when HF connection is active\n");
            }
            CsrBtHfSetDialog(inst, CSR_BT_HF_DEFAULT_INFO_UI, CONVERT_TEXT_STRING_2_UCS2("HF Redial"),
                    CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
            CsrBtHfShowUi(inst, CSR_BT_HF_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HF_STD_PRIO);
            break;
        }
        case CSR_BT_HF_SEND_DTMF_KEY:
        {
            CsrBtHfSetInputDialog(inst, CSR_BT_HF_DEFAULT_INPUT_UI,
                CONVERT_TEXT_STRING_2_UCS2("DTMF Entry"), CONVERT_TEXT_STRING_2_UCS2("Enter the DTMF Tone:"),
                CSR_UI_ICON_KEY, NULL, 1,
                CSR_UI_KEYMAP_CONTROLNUMERIC, TEXT_OK_UCS2, NULL);
            inst->state = enteringDTMF;
            CsrBtHfShowUi(inst, CSR_BT_HF_DEFAULT_INPUT_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HF_STD_PRIO);
            break;
        }
        case CSR_BT_HF_REQ_SCO_SPL_SET_KEY:
        {
            if ((inst->conInstData[inst->currentConnection].linkType == CSR_BT_HF_CONNECTION_UNKNOWN) &&
                        (inst->conInstData[inst->currentConnection].audioOn))
            {
                sprintf(buf,"Error!! Need Service Connection and there should not be Audio Connection!!\n");
                CsrBtHfSetDialog(inst, CSR_BT_HF_DEFAULT_INFO_UI, TEXT_FAILED_UCS2,
                                                                        CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
                /* Show the CSR_BT_HF_DEFAULT_INFO_UI on the display                   */
                CsrBtHfShowUi(inst, CSR_BT_HF_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HF_STD_PRIO);

            }
            else
            {
                CsrBtHfShowUi(inst, CSR_BT_REQUEST_SCO_SPL_SETTINGS_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HF_STD_PRIO);
            }
            break;
        }
        case CSR_BT_HF_ENH_CALL_HANDLING_KEY:
        {
            break;
        }
        case CSR_BT_HF_TOG_INBAND_RING_KEY:
        {
            if (inst->conInstData[inst->currentConnection].linkType != CSR_BT_HF_CONNECTION_UNKNOWN)
            {
                if (inst->conInstData[inst->currentConnection].inbandRingingActivatedInHf)
                {
                    inst->conInstData[inst->currentConnection].inbandRingingActivatedInHf = FALSE;
                    sprintf(buf, "Inband Ringing Deactivated!!\n");
                }
                else
                {
                    inst->conInstData[inst->currentConnection].inbandRingingActivatedInHf = TRUE;
                    sprintf(buf, "Inband Ringing Activated!!\n");
                }
            }
            else
            {
                sprintf(buf, "Select active connection before requesting action\n");
            }
            CsrBtHfSetDialog(inst, CSR_BT_HF_DEFAULT_INFO_UI, CONVERT_TEXT_STRING_2_UCS2("Inband Ringing Setting"),
                                                                    CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
            /* Show the CSR_BT_HF_DEFAULT_INFO_UI on the display                   */
            CsrBtHfShowUi(inst, CSR_BT_HF_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HF_STD_PRIO);
            break;
        }
        case CSR_BT_HF_QUERY_LIST_CUR_CALLS_KEY:
        {
            if (inst->conInstData[inst->currentConnection].linkType == CSR_BT_HF_CONNECTION_HF)
            {
                sprintf(buf, "Sent Query list of current calls ...\n");
                CsrBtHfGetCurrentCallListReqSend(inst->conInstData[inst->currentConnection].conId);
                CsrUiMenuRemoveallitemsReqSend(csrUiVar->displayesHandlers[CSR_BT_HF_CALL_LIST_UI].displayHandle);
                inst->callIndex = 0;
            }
            else
            {
                sprintf(buf, "Select active HF connection before requesting action\n");
            }
            CsrBtHfSetDialog(inst, CSR_BT_HF_DEFAULT_INFO_UI, CONVERT_TEXT_STRING_2_UCS2("Query Current call list"),
                                                                    CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
            /* Show the CSR_BT_HF_DEFAULT_INFO_UI on the display                   */
            CsrBtHfShowUi(inst, CSR_BT_HF_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HF_STD_PRIO);
            break;
        }
        case CSR_BT_HF_REQ_PHONE_NO_VOICE_TAG_KEY:
        {
            if ((inst->conInstData[inst->currentConnection].linkType != CSR_BT_HF_CONNECTION_UNKNOWN) &&
                (inst->conInstData[inst->currentConnection].localSupportedFeatures & CSR_BT_HF_SUPPORT_VOICE_RECOGNITION) &&
                (inst->conInstData[inst->currentConnection].localActivatedFeatures & CSR_BT_HF_SUPPORT_VOICE_RECOGNITION))
            {
                sprintf(buf, "Sending AT+BINP=1 command to Hf\n");
                CsrBtHfBtInputReqSend(inst->conInstData[inst->currentConnection].conId,1);
            }
            else
            {
                sprintf(buf, "Error:Service level connection is not established!!");
            }
            break;
        }
        case CSR_BT_HF_RESPONSE_HOLD_OPTIONS_KEY:
        {
            break;
        }
        case CSR_BT_HF_CALL_NO_STORED_SPEC_MEM_KEY:
        {
            CsrBtHfSetInputDialog(inst, CSR_BT_HF_DEFAULT_INPUT_UI,
                CONVERT_TEXT_STRING_2_UCS2("Call specific HFG mem location"), CONVERT_TEXT_STRING_2_UCS2("Call specific HFG mem location:"),
                CSR_UI_ICON_KEY, NULL, 50,
                CSR_UI_KEYMAP_NUMERIC,TEXT_OK_UCS2, NULL);

            CsrBtHfShowUi(inst, CSR_BT_HF_DEFAULT_INPUT_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HF_STD_PRIO);
            inst->state = memDialling;
            CsrUiDisplaySetinputmodeReqSend(csrUiVar->displayesHandlers[CSR_BT_HF_DEFAULT_INPUT_UI].displayHandle,
                                                                        CSR_UI_INPUTMODE_AUTO);
            break;
        }
        case CSR_BT_HF_EXT_AUDIO_GATEWAY_ERR_CODE_KEY:
        {
            if (inst->conInstData[inst->currentConnection].linkType == CSR_BT_HF_CONNECTION_HF)
            {
                if(inst->conInstData[inst->currentConnection].cmeErrorCodes == FALSE)
                {
                    sprintf(buf, "Toggled!!CMEE error codes will be used\n");
                    inst->conInstData[inst->currentConnection].cmeErrorCodes = TRUE;
                }
                else
                {
                    sprintf(buf, "Toggled!! CMEE error codes will not be used\n");
                    inst->conInstData[inst->currentConnection].cmeErrorCodes = FALSE;
                }
                CsrBtHfSetExtendedAgErrorResultCodeReqSend(inst->conInstData[inst->currentConnection].conId,inst->conInstData[inst->currentConnection].cmeErrorCodes);
            }
            else
            {
                sprintf(buf, "Select active HF connection before requesting action\n");
            }
            CsrBtHfSetDialog(inst, CSR_BT_HF_DEFAULT_INFO_UI, CONVERT_TEXT_STRING_2_UCS2("Toggle CMEE error Codes"),
                                                                    CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
            /* Show the CSR_BT_HF_DEFAULT_INFO_UI on the display                   */
            CsrBtHfShowUi(inst, CSR_BT_HF_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HF_STD_PRIO);
            break;
        }
        default :
        {
            CsrGeneralException("CSR_BT_HF", 0, 0,
            "Unexpected menu key is received");
            break;
        }
    }
    CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);
}

static void appCreateHfCallHandlingUi(DemoInstdata_t * inst)
{ /* This function Creates the CSR_BT_CALL_HANDLING_MENU_UI                              */

    CsrBtHfCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_MENU)
    {
        csrUiVar->eventState                                              = CSR_BT_HF_CREATE_SK1_EVENT;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHfCallHandlingMenu;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Set the menu on the CSR_BT_CALL_HANDLING_MENU_UI                               */
        CsrUiMenuSetReqSend(prim->handle, TEXT_HF_CALL_HANDLING_UCS2, TEXT_SELECT_UCS2, NULL);

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    {
        if (csrUiVar->eventState == CSR_BT_HF_CREATE_SK1_EVENT)
        { /* Save the event handle as SK1, and set input mode to blocked        */

            csrUiVar->eventState                                          = CSR_BT_HF_CREATE_BACK_EVENT;
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Create a BACK event                                              */
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else
        { /* CSR_BT_HF_CREATE_BACK_EVENT                                           */

            CsrUieHandle displayHandle  = csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle;
            CsrUieHandle sk1Handle      = csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle;

            /* Save the event handle as Back, and set input mode to blocked     */
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_HF_TOG_AUDIO_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_TOG_AUDIO_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_CALL_SPEC_NO_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_CALL_SPEC_NO_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_ANS_IN_CALL_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_ANS_IN_CALL_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_REJ_IN_CALL_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_REJ_IN_CALL_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_CALL_LINE_ID_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_CALL_LINE_ID_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_LAST_NO_REDIAL_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_LAST_NO_REDIAL_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_SEND_DTMF_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_SEND_DTMF_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_REQ_SCO_SPL_SET_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_REQ_SCO_SPL_SET_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_ENH_CALL_HANDLING_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_ENH_CALL_HANDLING_UCS2, NULL,
                            csrUiVar->displayesHandlers[CSR_BT_ENH_CALL_HANDLING_MENU_UI].displayHandle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_TOG_INBAND_RING_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_TOG_INBAND_RING_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_QUERY_LIST_CUR_CALLS_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_QUERY_LIST_CUR_CALLS_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_REQ_PHONE_NO_VOICE_TAG_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_REQ_PHONE_NO_VOICE_TAG_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_RESPONSE_HOLD_OPTIONS_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_RESPONSE_HOLD_OPTIONS_UCS2, NULL,
                            csrUiVar->displayesHandlers[CSR_BT_RESPONSE_HOLD_OPTIONS_UI].displayHandle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_CALL_NO_STORED_SPEC_MEM_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_HF_CALL_NO_STORED_SPEC_MEM_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_EXT_AUDIO_GATEWAY_ERR_CODE_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_EXT_AUDIO_GATEWAY_ERR_CODE_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            csrUiVar->uiIndex = CSR_BT_CHANGE_AUDIO_CODEC_UI;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);

        }
    }
}

static void appUiKeyHandlerChangeAudioCodecMenu(void * inst,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from the CSR_BT_CHANGE_AUDIO_CODEC_UI */
    DemoInstdata_t* instData = inst;
    CsrCharString buf[70];
    CSR_UNUSED(eventHandle);

    switch (key)
    {
        case CSR_BT_HF_ADPCM_OFF_KEY:
        { /* CVSD, 8KHz*/
            instData->audioCodecType = CSR_BT_C2C_ADPCM_OFF_VALUE;
            sendAudioCodec(instData->conInstData[instData->currentConnection].conId,CSR_BT_C2C_ADPCM_OFF_VALUE,0);
            sprintf(buf, "Disabling ADPCM audio codec...\n");
            break;
        }
        case CSR_BT_HF_2_8KHZ_NO_SR_NEG_KEY:
        {   /*ADPCM 2bit/sample*/
            instData->audioCodecType = CSR_BT_C2C_ADPCM_2BIT_VALUE;
            sendAudioCodec(instData->conInstData[instData->currentConnection].conId,CSR_BT_C2C_ADPCM_2BIT_VALUE,0);
            sprintf(buf, "ADPCM audio codec set to 2bits/sample.\n");
            break;
        }
        case CSR_BT_HF_2_8KHZ_SR_NEG_KEY:
        {
            /*ADPCM 2bit/sample @ 8 KHZ - new spec format with sample rate negotiation */
            instData->audioCodecType = CSR_BT_C2C_ADPCM_2BIT_VALUE;
            instData->audioSampleRate = CSR_BT_C2C_SAMPLE_8KHZ_VALUE;
            sendAudioCodec(instData->conInstData[instData->currentConnection].conId,CSR_BT_C2C_ADPCM_2BIT_VALUE,CSR_BT_C2C_SAMPLE_8KHZ_VALUE);
            sprintf(buf, "ADPCM audio codec set to 2bits/sample @ 8KHz sampling rate.\n");
            break;
        }
        case CSR_BT_HF_4_8KHZ_NO_SR_NEG_KEY:
        {   /*ADPCM 4bit/sample*/
            instData->audioCodecType = CSR_BT_C2C_ADPCM_4BIT_VALUE;
            sendAudioCodec(instData->conInstData[instData->currentConnection].conId,CSR_BT_C2C_ADPCM_4BIT_VALUE,0);
            sprintf(buf, "ADPCM audio codec set to 4bits/sample.\n");
            break;
        }

        case CSR_BT_HF_4_8KHZ_SR_NEG_KEY:
        {
            /*ADPCM 4bit/sample @ 8 KHZ - new spec format with sample rate negotiation */
            instData->audioCodecType = CSR_BT_C2C_ADPCM_4BIT_VALUE;
            instData->audioSampleRate = CSR_BT_C2C_SAMPLE_8KHZ_VALUE;
            sendAudioCodec(instData->conInstData[instData->currentConnection].conId,CSR_BT_C2C_ADPCM_4BIT_VALUE,CSR_BT_C2C_SAMPLE_8KHZ_VALUE);
            sprintf(buf, "ADPCM audio codec set to 4bits/sample @ 8KHz sampling rate.\n");
            break;
        }
        case CSR_BT_HF_2_16KHZ_SR_NEG_KEY:
        {
            /*ADPCM 2bit/sample @ 16 KHZ - new spec format with sample rate negotiation */
            instData->audioCodecType = CSR_BT_C2C_ADPCM_2BIT_VALUE;
            instData->audioSampleRate = CSR_BT_C2C_SAMPLE_16KHZ_VALUE;
            sendAudioCodec(instData->conInstData[instData->currentConnection].conId,instData->audioCodecType,instData->audioSampleRate);
            sprintf(buf, "ADPCM audio codec set to 2bits/sample @ 16KHz sampling rate.\n");
            break;
        }
        case CSR_BT_HF_4_16KHZ_SR_NEG_KEY:
        {
            /*ADPCM 4bit/sample @ 16 KHZ - new spec format with sample rate negotiation */
            instData->audioCodecType = CSR_BT_C2C_ADPCM_4BIT_VALUE;
            instData->audioSampleRate = CSR_BT_C2C_SAMPLE_16KHZ_VALUE;
            sendAudioCodec(instData->conInstData[instData->currentConnection].conId,instData->audioCodecType,instData->audioSampleRate);
            sprintf(buf, "ADPCM audio codec set to 4bits/sample @ 16KHz sampling rate.\n");
            break;
        }
        default :
        {
            CsrGeneralException("CSR_BT_HF", 0, 0,
            "Unexpected menu key is received");
            break;
        }
    }
    CsrBtHfSetDialog(instData, CSR_BT_HF_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                            CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
    /* Show the CSR_BT_HF_DEFAULT_INFO_UI on the display                   */
    CsrBtHfShowUi(instData, CSR_BT_HF_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HF_STD_PRIO);
    CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);
}

static void appCreateHfChangeAudioCodecUi(DemoInstdata_t * inst)
{ /* This function Creates the CSR_BT_CHANGE_AUDIO_CODEC_UI                              */

    CsrBtHfCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_MENU)
    {
        csrUiVar->eventState                                              = CSR_BT_HF_CREATE_SK1_EVENT;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerChangeAudioCodecMenu;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Set the menu on the CSR_BT_CHANGE_AUDIO_CODEC_UI                               */
        CsrUiMenuSetReqSend(prim->handle, TEXT_HF_CHANGE_AUDIO_CODEC_UCS2, TEXT_SELECT_UCS2, NULL);

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    {
        if (csrUiVar->eventState == CSR_BT_HF_CREATE_SK1_EVENT)
        { /* Save the event handle as SK1, and set input mode to blocked        */

            csrUiVar->eventState                                          = CSR_BT_HF_CREATE_BACK_EVENT;
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Create a BACK event                                              */
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else
        { /* CSR_BT_HF_CREATE_BACK_EVENT                                           */

            CsrUieHandle displayHandle  = csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle;
            CsrUieHandle sk1Handle      = csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle;

            /* Save the event handle as Back, and set input mode to blocked     */
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_HF_ADPCM_OFF_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_ADPCM_OFF_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_2_8KHZ_NO_SR_NEG_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_2_8KHZ_NO_SR_NEG_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_2_8KHZ_SR_NEG_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_2_8KHZ_SR_NEG_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_4_8KHZ_NO_SR_NEG_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_4_8KHZ_NO_SR_NEG_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_4_8KHZ_SR_NEG_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_4_8KHZ_SR_NEG_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_2_16KHZ_SR_NEG_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_2_16KHZ_SR_NEG_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_4_16KHZ_SR_NEG_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_4_16KHZ_SR_NEG_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            csrUiVar->uiIndex = CSR_BT_C2C_MENU_UI;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);

        }
    }
}


static void appUiKeyHandlerC2CMenu(void * instData,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from the CSR_BT_C2C_MENU_UI */
    DemoInstdata_t* inst = instData;
    HfHsCommonInstData_t *link = &(inst->conInstData[inst->currentConnection]);
    CsrCharString buf[70];

    CSR_UNUSED(eventHandle);


    switch (key)
    {
        case CSR_BT_HF_REM_C2C_SUPP_FEATURES_KEY:
        {
            CsrCharString buf1[200];
            CsrSnprintf(buf1, 200, "C2C_NAME_IND\t\t: %d\nC2C_TXT_IND\t\t: %d\nC2C_SMS_IND\t\t: %d\nC2C_BAT_IND\t\t: %d\nC2C_PWR_IND\t\t: %d\n \
                                    C2C_ADPCM_IND\t\t: %d\nC2C_SAMPLE_RATE_IND\t\t: %d\n", link->hfgC2c[0], link->hfgC2c[1],link->hfgC2c[2], link->hfgC2c[3],
                                    link->hfgC2c[4], link->hfgC2c[5], link->hfgC2c[6]);
            CsrBtHfSetDialog(inst, CSR_BT_HF_DEFAULT_INFO_UI, CONVERT_TEXT_STRING_2_UCS2("Remote supported C2C Features"),
                                                                    CONVERT_TEXT_STRING_2_UCS2(buf1), TEXT_OK_UCS2, NULL);
            /* Show the CSR_BT_HF_DEFAULT_INFO_UI on the display                   */
            CsrBtHfShowUi(inst, CSR_BT_HF_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HF_STD_PRIO);
            CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);
            return;
        }
        case CSR_BT_HF_SEND_C2C_INIT_CMD_KEY:
        {
            sendC2cInit(inst, inst->conInstData[inst->currentConnection].conId,TRUE);
            sprintf(buf, "C2C Init Command sent!!\n");
            break;
        }
        case CSR_BT_HF_REQ_SMS_KEY:
        {
            if (inst->conInstData[inst->currentConnection].conId != 0xFF)
            {
                CsrUint8 smsIndex = 42; /* Should be any number, or a number received from the AG.... Shall be fixed */

                CsrBtHfSetC2CGetSmsReqSend(inst->conInstData[inst->currentConnection].conId,smsIndex);
                sprintf(buf, "SMS (%d) requested\n", smsIndex);
            }
            else
            {
                sprintf(buf, "No connection found. Not possible to send command\n");
            }
            break;
        }
        case CSR_BT_HF_INC_BAT_LEVEL_KEY:
        case CSR_BT_HF_DEC_BAT_LEVEL_KEY:
        {
            if (inst->conInstData[inst->currentConnection].conId != 0xFF)
            {
                if (key == CSR_BT_HF_INC_BAT_LEVEL_KEY)
                {/* Increase battery level */
                    if (inst->batteryLevel < 9)
                    {
                        inst->batteryLevel++;
                    }
                }
                else
                {/* Decrease battery level */
                    if (inst->batteryLevel > 0)
                    {
                        inst->batteryLevel--;
                    }
                }

                sendBatteryLevel(inst->conInstData[inst->currentConnection].conId,inst->batteryLevel);
                sprintf(buf, "Battery level is sent!!\n");
            }
            else
            {
                sprintf(buf, "No connection found. Not possible to send command\n");
            }
            break;
        }
        case CSR_BT_HF_SEND_PWR_STATUS_BAT_KEY:
        case CSR_BT_HF_SEND_PWR_STATUS_EXT_KEY:
        {
            CsrUint8 val;
            if (key == CSR_BT_HF_SEND_PWR_STATUS_BAT_KEY)
            {
                val = 1;
            }
            else
            {
                val = 2;
            }
            if (inst->conInstData[inst->currentConnection].conId != 0xFF)
            {
                CsrBtHfC2CPowerReqSend(inst->conInstData[inst->currentConnection].conId, val);
                sprintf(buf, "C2C Power Req command is sent!!\n");
            }
            else
            {
                sprintf(buf, "No connection found. Not possible to send command\n");
            }
            break;
        }
        case CSR_BT_HF_SEND_AT_CSR_ACT_KEY:
        case CSR_BT_HF_SEND_AT_CSR_DEACT_KEY:
        {
            CsrBool enable = TRUE;
            if (key == CSR_BT_HF_SEND_AT_CSR_DEACT_KEY)
            {
                enable = FALSE;
            }

            if (inst->conInstData[inst->currentConnection].conId != 0xFF)
            {
                if (inst->conInstData[inst->currentConnection].linkType == CSR_BT_HF_CONNECTION_HF)
                {
                    sendC2cInit(inst,inst->conInstData[inst->currentConnection].conId, enable);
                    sprintf(buf, "C2C Init is sent!!\n");
                }
                else
                {
                    sprintf(buf, "HS connection chosen as active. Please choose a HF connection instead\n");
                }
            }
            else
            {
                sprintf(buf, "No connection found. Not possible to send command\n");
            }
            break;
        }
        case CSR_BT_HF_SEND_AT_CSRFN_KEY:
        {
            if (inst->adpcmLocalSupported)
            {
                if(inst->conInstData[inst->currentConnection].linkState == connected_s)
                {
                    CsrBtHfShowUi(inst, CSR_BT_CHANGE_AUDIO_CODEC_UI, CSR_UI_INPUTMODE_AUTO, 1);
                    return;
                }
                else
                {
                    sprintf(buf, "\nAudio Codec can only be changed when connected\n");
                }
            }
            else
            {
                sprintf(buf, "\nADPCM not supported locally. Request not sent\n");
            }
            break;
        }
        default :
        {
            CsrGeneralException("CSR_BT_HF", 0, 0,
            "Unexpected menu key is received");
            break;
        }
    }
    CsrBtHfSetDialog(inst, CSR_BT_HF_DEFAULT_INFO_UI, CONVERT_TEXT_STRING_2_UCS2("C2C menu setting"),
                                                            CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
    /* Show the CSR_BT_HF_DEFAULT_INFO_UI on the display                   */
    CsrBtHfShowUi(inst, CSR_BT_HF_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HF_STD_PRIO);
    CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);

}
static void appCreateHfC2CUi(DemoInstdata_t * inst)
{ /* This function Creates the CSR_BT_C2C_MENU_UI                              */

    CsrBtHfCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_MENU)
    {
        csrUiVar->eventState                                              = CSR_BT_HF_CREATE_SK1_EVENT;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerC2CMenu;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Set the menu on the CSR_BT_C2C_MENU_UI                               */
        CsrUiMenuSetReqSend(prim->handle, TEXT_HF_C2C_UCS2, TEXT_SELECT_UCS2, NULL);

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    {
        if (csrUiVar->eventState == CSR_BT_HF_CREATE_SK1_EVENT)
        { /* Save the event handle as SK1, and set input mode to blocked        */

            csrUiVar->eventState                                          = CSR_BT_HF_CREATE_BACK_EVENT;
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Create a BACK event                                              */
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else
        { /* CSR_BT_HF_CREATE_BACK_EVENT                                           */

            CsrUieHandle displayHandle  = csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle;
            CsrUieHandle sk1Handle      = csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle;

            /* Save the event handle as Back, and set input mode to blocked     */
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_HF_REM_C2C_SUPP_FEATURES_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_REM_C2C_SUPP_FEATURES_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_SEND_C2C_INIT_CMD_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_SEND_C2C_INIT_CMD_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_REQ_SMS_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_REQ_SMS_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_INC_BAT_LEVEL_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_INC_BAT_LEVEL_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_DEC_BAT_LEVEL_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_DEC_BAT_LEVEL_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_SEND_PWR_STATUS_BAT_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_SEND_PWR_STATUS_BAT_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_SEND_PWR_STATUS_EXT_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_SEND_PWR_STATUS_EXT_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_SEND_AT_CSR_ACT_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_SEND_AT_CSR_ACT_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_SEND_AT_CSR_DEACT_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_SEND_AT_CSR_DEACT_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_SEND_AT_CSRFN_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_SEND_AT_CSRFN_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            csrUiVar->uiIndex = CSR_BT_COD_SELECTION_MENU_UI;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);

        }
    }
}

static void appUiKeyHandlerCodSelectionMenu(void * instData,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from the CSR_BT_COD_SELECTION_MENU_UI */
    DemoInstdata_t* inst = instData;
    CsrBtCmWriteCodReq *prim;
    CsrCharString buf[35];
    CsrUint32 cod = 0;

    inst->reactKey = (CsrUint8)key;
    CSR_UNUSED(eventHandle);

    switch (key)
    {
        case CSR_BT_HF_COD_200404_KEY:
        {
            cod = 0x200404;
            break;
        }
        case CSR_BT_HF_COD_202404_KEY:
        {
            cod = 0x202404;
            break;
        }
        case CSR_BT_HF_COD_200408_KEY:
        {
            cod = 0x200408;
            break;
        }
        case CSR_BT_HF_COD_20080C_KEY:
        {
            cod = 0x20080c;
            break;
        }
        default :
        {
            CsrGeneralException("CSR_BT_HF", 0, 0,
            "Unexpected menu key is received");
            break;
        }
    }
    prim = CsrPmemAlloc(sizeof(CsrBtCmWriteCodReq));
    prim->type = CSR_BT_CM_WRITE_COD_REQ;
    prim->appHandle = inst->app_hdl;
    prim->updateFlags = (CSR_BT_CM_WRITE_COD_UPDATE_FLAG_SERVICE_CLASS |
                         CSR_BT_CM_WRITE_COD_UPDATE_FLAG_MAJOR_MINOR_CLASS);
    prim->serviceClassOfDevice = cod;
    prim->majorClassOfDevice = cod;
    prim->minorClassOfDevice = cod;

    CsrSchedMessagePut(CSR_BT_CM_IFACEQUEUE, CSR_BT_CM_PRIM, prim);

    sprintf(buf, "CSR_BT_CM_WRITE_COD_REQ is sent!!");
    CsrBtHfSetDialog(inst, CSR_BT_HF_DEFAULT_INFO_UI, CONVERT_TEXT_STRING_2_UCS2("COD setting"),
                                                            CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
    /* Show the CSR_BT_HF_DEFAULT_INFO_UI on the display                   */
    CsrBtHfShowUi(inst, CSR_BT_HF_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HF_STD_PRIO);
    CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);
}

static void appCreateHfCodSelectionUi(DemoInstdata_t * inst)
{ /* This function Creates the CSR_BT_COD_SELECTION_MENU_UI                              */

    CsrBtHfCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_MENU)
    {
        csrUiVar->eventState                                              = CSR_BT_HF_CREATE_SK1_EVENT;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerCodSelectionMenu;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Set the menu on the CSR_BT_COD_SELECTION_MENU_UI                               */
        CsrUiMenuSetReqSend(prim->handle, TEXT_HF_COD_SEL_UCS2, TEXT_SELECT_UCS2, NULL);

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    {
        if (csrUiVar->eventState == CSR_BT_HF_CREATE_SK1_EVENT)
        { /* Save the event handle as SK1, and set input mode to blocked        */

            csrUiVar->eventState                                          = CSR_BT_HF_CREATE_BACK_EVENT;
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Create a BACK event                                              */
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else
        { /* CSR_BT_HF_CREATE_BACK_EVENT                                           */

            CsrUieHandle displayHandle  = csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle;
            CsrUieHandle sk1Handle      = csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle;

            /* Save the event handle as Back, and set input mode to blocked     */
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_HF_COD_200404_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_COD_200404_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_COD_202404_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_COD_202404_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_COD_200408_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_COD_200408_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_COD_20080C_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_COD_20080C_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            csrUiVar->uiIndex = CSR_BT_IND_INDICATOR_ACT_UI;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);

        }
    }
}


static void appUiKeyHandlerIndIndicatorMenu(void * inst,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from the CSR_BT_IND_INDICATOR_ACT_UI */
    DemoInstdata_t* instData = inst;
    CsrCharString buf[35];

    instData->reactKey = (CsrUint8)key;
    CSR_UNUSED(eventHandle);

    switch (key)
    {
        case CSR_BT_HF_TOG_SERVICE_KEY:
        case CSR_BT_HF_TOG_CALL_KEY:
        case CSR_BT_HF_TOG_CALLSETUP_KEY:
        case CSR_BT_HF_TOG_CALLHELD_KEY:
        case CSR_BT_HF_TOG_SIGNAL_KEY:
        case CSR_BT_HF_TOG_ROAM_KEY:
        case CSR_BT_HF_TOG_BAT_CHG_KEY:
        {
            CsrUint16 idx = (CsrUint16)(1 << key);

            if (instData->conInstData[instData->currentConnection].tmpIndicatorActivation & idx)
            {/* The bit is set: reset it */
                instData->conInstData[instData->currentConnection].tmpIndicatorActivation &= ~idx;
                sprintf(buf, "Indicator is reset!!");
            }
            else
            { /* The bit is reset: set it*/
                instData->conInstData[instData->currentConnection].tmpIndicatorActivation |= idx;
                sprintf(buf, "Indicator is set!!");
            }
            CsrBtHfSetDialog(instData, CSR_BT_HF_DEFAULT_INFO_UI, CONVERT_TEXT_STRING_2_UCS2("Indicator setting"),
                                                                    CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
            break;
        }
        case CSR_BT_HF_DISP_IND_STATUS_KEY:
        {
            CsrUint16 i,j;
            CsrCharString buf1[200] = "";
            for (i=0, j=0; instData->conInstData[instData->currentConnection].cindString[i] != 0 ;i++)
            {
                if ((instData->conInstData[instData->currentConnection].cindString[i] == '(') &&
                    (instData->conInstData[instData->currentConnection].cindString[i+1] == '"'))
                {
                    i += 2;
                    for (;instData->conInstData[instData->currentConnection].cindString[i] != '"';i++)
                    {
                        sprintf(buf, "%c",instData->conInstData[instData->currentConnection].cindString[i]);
                        CsrStrCat(buf1, buf);
                    }
                    sprintf(buf, " %s\n",((instData->conInstData[instData->currentConnection].tmpIndicatorActivation & (1 << j)) ? "(ENABLED)" : "(DISABLED)"));
                    CsrStrCat(buf1, buf);
                    j++;
                }
            }
            CsrBtHfSetDialog(instData, CSR_BT_HF_DEFAULT_INFO_UI, CONVERT_TEXT_STRING_2_UCS2("Display Indicator Status"),
                                                                    CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
            break;
        }
        case CSR_BT_HF_SEND_IND_SETTING_KEY:
        {/* Done: send AT+BIA */
            CsrBtHfIndicatorActivationReqSend(instData->conInstData[instData->currentConnection].tmpIndicatorActivation,
                        instData->conInstData[instData->currentConnection].conId);
            sprintf(buf, "Indicator info sent!!");
            CsrBtHfSetDialog(instData, CSR_BT_HF_DEFAULT_INFO_UI, CONVERT_TEXT_STRING_2_UCS2("Send Indicator Status"),
                                                                    CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
            break;
        }
        default :
        {
            CsrGeneralException("CSR_BT_HF", 0, 0,
            "Unexpected menu key is received");
            break;
        }
    }

    /* Show the CSR_BT_HF_DEFAULT_INFO_UI on the display                   */
    CsrBtHfShowUi(instData, CSR_BT_HF_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HF_STD_PRIO);
    CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);
}

static void appCreateHfIndIndicatorActUi(DemoInstdata_t * inst)
{ /* This function Creates the CSR_BT_IND_INDICATOR_ACT_UI                              */

    CsrBtHfCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_MENU)
    {
        csrUiVar->eventState                                              = CSR_BT_HF_CREATE_SK1_EVENT;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerIndIndicatorMenu;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Set the menu on the CSR_BT_IND_INDICATOR_ACT_UI                               */
        CsrUiMenuSetReqSend(prim->handle, TEXT_HF_IND_INDICATOR_UCS2, TEXT_SELECT_UCS2, NULL);

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    {
        if (csrUiVar->eventState == CSR_BT_HF_CREATE_SK1_EVENT)
        { /* Save the event handle as SK1, and set input mode to blocked        */

            csrUiVar->eventState                                          = CSR_BT_HF_CREATE_BACK_EVENT;
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Create a BACK event                                              */
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else
        { /* CSR_BT_HF_CREATE_BACK_EVENT                                           */

            CsrUieHandle displayHandle  = csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle;
            CsrUieHandle sk1Handle      = csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle;

            /* Save the event handle as Back, and set input mode to blocked     */
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_HF_TOG_SERVICE_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_TOG_SERVICE_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_TOG_CALL_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_TOG_CALL_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_TOG_CALLSETUP_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_TOG_CALLSETUP_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_TOG_CALLHELD_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_TOG_CALLHELD_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_TOG_SIGNAL_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_TOG_SIGNAL_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_TOG_ROAM_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_TOG_ROAM_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_TOG_BAT_CHG_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_TOG_BAT_CHG_UCS2Y, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_DISP_IND_STATUS_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_DISP_IND_STATUS_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_SEND_IND_SETTING_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_SEND_IND_SETTING_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            csrUiVar->uiIndex = CSR_BT_CHOOSE_ACTIVE_CON_UI;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);

        }
    }
}


static void appUiKeyHandlerChooseActiveConMenu(void * inst,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from the CSR_BT_CHOOSE_ACTIVE_CON_UI */
    DemoInstdata_t* instData = inst;
    CsrCharString buf[35];
    HfHsCommonInstData_t *link = &(instData->conInstData[key]);

    CSR_UNUSED(eventHandle);

    if(key <= 1)
    {
        if (link->linkState != connected_s)
        {
            sprintf(buf, "Choose a valid connection index\n");
        }
        else
        {
            instData->currentConnection = (CsrUint8)key;
            sprintf(buf, "Active connection selected\n");
        }
    }
    else
    {
        sprintf(buf, "Invalid key\n");
    }
    CsrBtHfSetDialog(instData, CSR_BT_HF_DEFAULT_INFO_UI, CONVERT_TEXT_STRING_2_UCS2("Active Connection selection"),
                                                            CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
    /* Show the CSR_BT_HF_DEFAULT_INFO_UI on the display                   */
    CsrBtHfShowUi(instData, CSR_BT_HF_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HF_STD_PRIO);
    CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);
}

static void appCreateHfChooseActiveConUi(DemoInstdata_t * inst)
{ /* This function Creates the CSR_BT_CHOOSE_ACTIVE_CON_UI                              */

    CsrBtHfCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_MENU)
    {
        csrUiVar->eventState                                              = CSR_BT_HF_CREATE_SK1_EVENT;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerChooseActiveConMenu;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Set the menu on the CSR_BT_CHOOSE_ACTIVE_CON_UI                               */
        CsrUiMenuSetReqSend(prim->handle, TEXT_HF_MAIN_MENU_UCS2, TEXT_SELECT_UCS2, NULL);

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    {
        if (csrUiVar->eventState == CSR_BT_HF_CREATE_SK1_EVENT)
        { /* Save the event handle as SK1, and set input mode to blocked        */

            csrUiVar->eventState                                          = CSR_BT_HF_CREATE_BACK_EVENT;
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Create a BACK event                                              */
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else
        { /* CSR_BT_HF_CREATE_BACK_EVENT                                           */
            /* Save the event handle as Back, and set input mode to blocked     */
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            csrUiVar->uiIndex = CSR_BT_HF_MAIN_MENU_UI;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);

        }
    }
}

void printLocalSupportedFeatures(DemoInstdata_t * instData)
{
    CsrCharString *buf;
    /* print out local supported HF features status */
    if (instData->conInstData[instData->currentConnection].linkType != CSR_BT_HF_CONNECTION_UNKNOWN)
    {
        if (instData->conInstData[instData->currentConnection].linkType == CSR_BT_HF_CONNECTION_HF)
        {
            buf = (CsrCharString*)CsrPmemAlloc(750);
            sprintf(buf, "HF Supported Feature bitmap number: %u\n \
                    bit 0    SUPPORT_EC_NR_FUNCTION (currently %s)\n \
                    bit 1    SUPPORT_CALL_WAITING_THREE_WAY_CALLING (currently %s)\n \
                    bit 2    SUPPORT_CLI_PRESENTATION_CAPABILITY    (currently %s)\n \
                    bit 3    SUPPORT_VOICE_RECOGNITION              (currently %s)\n \
                    bit 4    SUPPORT_REMOTE_VOLUME_CONTROL          (currently %s)\n \
                    bit 5    SUPPORT_SUPPORT_ENHANCED_CALL_STATUS   (currently %s)\n \
                    bit 6    SUPPORT_SUPPORT_ENHANCED_CALL_CONTROL  (currently %s)\n \
                    HF Activated Feature bitmap number: %u\n \
                    bit 0    SUPPORT_EC_NR_FUNCTION                 (currently %s) \
                    bit 3    SUPPORT_VOICE_RECOGNITION              (currently %s)\n",
                   instData->conInstData[instData->currentConnection].localSupportedFeatures,
                   ((instData->conInstData[instData->currentConnection].localSupportedFeatures & CSR_BT_HF_SUPPORT_EC_NR_FUNCTION) ? "ON" : "OFF"),
                   ((instData->conInstData[instData->currentConnection].localSupportedFeatures & CSR_BT_HF_SUPPORT_CALL_WAITING_THREE_WAY_CALLING) ? "ON" : "OFF"),
                   ((instData->conInstData[instData->currentConnection].localSupportedFeatures & CSR_BT_HF_SUPPORT_CLI_PRESENTATION_CAPABILITY) ? "ON" : "OFF"),
                   ((instData->conInstData[instData->currentConnection].localSupportedFeatures & CSR_BT_HF_SUPPORT_VOICE_RECOGNITION) ? "ON" : "OFF"),
                   ((instData->conInstData[instData->currentConnection].localSupportedFeatures & CSR_BT_HF_SUPPORT_REMOTE_VOLUME_CONTROL) ? "ON" : "OFF"),
                   ((instData->conInstData[instData->currentConnection].localSupportedFeatures & CSR_BT_HF_SUPPORT_ENHANCED_CALL_STATUS) ? "ON" : "OFF"),
                   ((instData->conInstData[instData->currentConnection].localSupportedFeatures & CSR_BT_HF_SUPPORT_ENHANCED_CALL_CONTROL) ? "ON" : "OFF"),
                   instData->conInstData[instData->currentConnection].localActivatedFeatures,
                   ((instData->conInstData[instData->currentConnection].localActivatedFeatures & CSR_BT_HF_SUPPORT_EC_NR_FUNCTION) ? "ON" : "OFF"),
                   ((instData->conInstData[instData->currentConnection].localActivatedFeatures & CSR_BT_HF_SUPPORT_VOICE_RECOGNITION) ? "ON" : "OFF"));
        }
        else if (instData->conInstData[instData->currentConnection].linkType == CSR_BT_HF_CONNECTION_HS)
        {
            buf = (CsrCharString*)CsrPmemAlloc(120);
            sprintf(buf, "HS Supported Feature bitmap number: %u\nSUPPORT_REMOTE_VOLUME_CONTROL (currently %s)\n",
                   instData->conInstData[instData->currentConnection].localSupportedFeatures,
                   ((instData->conInstData[instData->currentConnection].localSupportedFeatures & CSR_BT_HF_SUPPORT_REMOTE_VOLUME_CONTROL) ? "ON" : "OFF"));
        }
    }
    else
    {
        buf = (CsrCharString*)CsrPmemAlloc(80);
        sprintf(buf, "Select active connection before requesting local supported functionality\n");
    }
    CsrBtHfSetDialog(instData, CSR_BT_HF_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                    CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
    /* Show the CSR_BT_HF_DEFAULT_INFO_UI on the display                   */
    CsrBtHfShowUi(instData, CSR_BT_HF_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HF_STD_PRIO);
    CsrPmemFree(buf);
}

void printHfgSupportedFeatures(DemoInstdata_t * instData, CsrUint32 features)
{
    CsrCharString buf[600];
    sprintf(buf, "HFG Feature bitmap number: %u\n \
    bit 0    SUPPORT_THREE_WAY_CALLING (currently %s)\n \
    bit 1    SUPPORT_EC_NR_FUNCTION (currently %s)\n \
    bit 2    SUPPORT_VOICE_RECOGNITION (currently %s)\n \
    bit 3    SUPPORT_INBAND_RINGING (currently %s)\n \
    bit 4    SUPPORT_ATTACH_NUMBER_TO_VOICE_TAG (currently %s)\n \
    bit 5    SUPPORT_ABILITY_TO_REJECT_CALL (currently %s)\n \
    bit 6    SUPPORT_ENHANCED_CALL_STATUS (currently %s)\n \
    bit 7    SUPPORT_ENHANCED_CALL_CONTROL (currently %s)\n \
    bit 8    SUPPORT_EXTENDED_ERROR_CODES (currently %s)\n", features, ((features & CSR_BT_HFG_SUPPORT_THREE_WAY_CALLING) ? "ON" : "OFF"),
        ((features & CSR_BT_HFG_SUPPORT_EC_NR_FUNCTION) ? "ON" : "OFF"), ((features & CSR_BT_HFG_SUPPORT_VOICE_RECOGNITION) ? "ON" : "OFF"),
        ((features & CSR_BT_HFG_SUPPORT_INBAND_RINGING) ? "ON" : "OFF"), ((features & CSR_BT_HFG_SUPPORT_ATTACH_NUMBER_TO_VOICE_TAG) ? "ON" : "OFF"),
        ((features & CSR_BT_HFG_SUPPORT_ABILITY_TO_REJECT_CALL) ? "ON" : "OFF"), ((features & CSR_BT_HFG_SUPPORT_ENHANCED_CALL_STATUS) ? "ON" : "OFF"),
        ((features & CSR_BT_HFG_SUPPORT_ENHANCED_CALL_CONTROL) ? "ON" : "OFF"), ((features & CSR_BT_HFG_SUPPORT_EXTENDED_ERROR_CODES) ? "ON" : "OFF"));
    CsrBtHfSetDialog(instData, CSR_BT_HF_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                    CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
    /* Show the CSR_BT_HF_DEFAULT_INFO_UI on the display                   */
    CsrBtHfShowUi(instData, CSR_BT_HF_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HF_STD_PRIO);
}


static void appUiKeyHfMainMenu(void * inst,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from the CSR_BT_HF_REACTIVATE_SERVER_UI */
    DemoInstdata_t* instData = inst;
    CsrBtHfCsrUiType    * csrUiVar  = &(instData->csrUiVar);

    CsrCharString buf[100];
    CsrCharString buf1[25] = TEXT_SUCCESS_STRING;
    CsrCharString buf2[15] = TEXT_OK_STRING;

    CsrUint8 inputMode = CSR_UI_INPUTMODE_AUTO;
    CSR_UNUSED(eventHandle);

    instData->reactKey = (CsrUint8)key;

    switch (key)
    {
        case CSR_BT_HF_ACT_REM_AUDIO_CTRL_ENA_KEY:
        {
            CsrBtHfActivateReqSend(instData->app_hdl, 1,1, 2, CSR_BT_HF_SUPPORT_ALL_FUNCTIONS,0,5);
            instData->serverActivated = TRUE;
            sprintf(buf, "HF activated with support for remote volume control\n");
            updateHfMainMenu(inst);
            break;
        }
        case CSR_BT_HF_ACT_REM_AUDIO_CTRL_DIS_KEY:
        {
            CsrBtHfActivateReqSend(instData->app_hdl, 1,1, 2, CSR_BT_HF_SUPPORT_ALL_FUNCTIONS, CSR_BT_HF_CNF_DISABLE_REMOTE_VOLUME_CONTROL,5);
            instData->serverActivated = TRUE;
            sprintf(buf, "HF activated without support for remote volume control\n");
            updateHfMainMenu(inst);
            break;
        }
        case CSR_BT_HF_ACT_NO_VOICE_RECOG_KEY:
        {
            /* Activate HF without voice recognition */
            CsrBtHfActivateReqSend(instData->app_hdl, 1, 1, 2,CSR_BT_HF_SUPPORT_ALL_BUT_VR,0,5);
            instData->serverActivated = TRUE;
            sprintf(buf, "HF activated without support for voice recognition\n");
            updateHfMainMenu(inst);
            break;
        }
        case CSR_BT_HF_ACT_NO_HS_KEY:
        {
            /* Activate HF without HSP support; with up to two simultaneous connections */
            CsrBtHfActivateReqSend(instData->app_hdl, 2,0,2, CSR_BT_HF_SUPPORT_ALL_BUT_VR, 0,5);
            instData->serverActivated = TRUE;
            sprintf(buf, "HF activated without HSP support\n");
            updateHfMainMenu(inst);
            break;
        }
        case CSR_BT_HF_DEACT_HF_SERVER_KEY:
        {
            /*  Deactivate HF */
            CsrBtHfDeactivateReqSend();
            instData->deactGuard = CsrSchedTimerSet(HF_DEACT_GAURD,
                                           CsrBtHfDeactTimeout,
                                           0,
                                           (void*)instData);

            inputMode = CSR_UI_INPUTMODE_BLOCK;
            sprintf(buf, "HF_DEACTIVATE_REQ is sent!!Waiting for the Confirm.");
            sprintf(buf1, "Please Wait");
            break;
        }
        case CSR_BT_HF_REACT_HF_SERVER_KEY:
        {
            break;
        }
        case CSR_BT_HF_EST_SLC_KEY:
        {
            if (!instData->connReqPending)
            {/*  start a connection to address specified */
                BD_ADDR_T       theAddr;
                extern BD_ADDR_T defGlobalBdAddr;
                if ((instData->remoteAddr.lap == 0) &&
                       (instData->remoteAddr.uap == 0) &&
                       (instData->remoteAddr.nap == 0))
                {/* Perform proximity pairing.... */
                    if ((defGlobalBdAddr.lap != 0) ||
                       (defGlobalBdAddr.uap != 0) ||
                       (defGlobalBdAddr.nap != 0))
                    {
                        CsrMemCpy(&theAddr, &defGlobalBdAddr, sizeof(BD_ADDR_T));
                        sprintf(buf, "Send connect request to: %04X:%02X:%06X\n", theAddr.nap, theAddr.uap, theAddr.lap);
                        sprintf(buf1, TEXT_PLEASE_WAIT_STRING);
                        sprintf(buf2, "Cancel");

                        CsrBtHfServiceConnectReqSend(theAddr,CSR_BT_HF_CONNECTION_UNKNOWN);
                        instData->connReqPending = TRUE;
                        instData->state = cancelState;
                    }
                    else
                    {
                        CsrBtGapAppPairProximityDeviceReqSend(CsrSchedTaskQueueGet(),0,0, FALSE);
                        sprintf(buf, "Searching for device to connect to\n");
                        sprintf(buf1, TEXT_PLEASE_WAIT_STRING);
                    }
                }
                else
                {
                    theAddr.lap = instData->remoteAddr.lap;
                    theAddr.uap = instData->remoteAddr.uap;
                    theAddr.nap = instData->remoteAddr.nap;

                    sprintf(buf, "Send connect request to: %04X:%02X:%06X\n", theAddr.nap, theAddr.uap, theAddr.lap);
                    sprintf(buf1, TEXT_PLEASE_WAIT_STRING);
                    sprintf(buf2, "Cancel");

                    CsrBtHfServiceConnectReqSend(theAddr,CSR_BT_HF_CONNECTION_UNKNOWN);
                    instData->connReqPending = TRUE;
                    instData->state = cancelState;
                }
            }
            else
            {
                sprintf(buf, "Connection already pending!\n");
                sprintf(buf1, "SLC Establishment");
            }
            /* Show the CSR_BT_HF_DEFAULT_INFO_UI on the display                   */
            CsrBtHfShowUi(instData, CSR_BT_HF_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HF_STD_PRIO);
            break;
        }
        case CSR_BT_HF_DISC_SLC_KEY:
        {
            if (instData->conInstData[instData->currentConnection].linkType != CSR_BT_HF_CONNECTION_UNKNOWN)
            {
                CsrBtHfDisconnectReqSend(instData->conInstData[instData->currentConnection].conId);
                sprintf(buf, "Send Disconnect request \n");
                /* instData->discGuard = CsrSchedTimerSet(HF_DISC_GAURD,
                                               CsrBtHfDiscTimeout,
                                               0,
                                               inst);            */
                inputMode = CSR_UI_INPUTMODE_BLOCK;
                sprintf(buf1, TEXT_PLEASE_WAIT_STRING);
            }
            else
            {
                sprintf(buf, "No Connection to Diaconnect!!\n");
                sprintf(buf1, TEXT_FAILED_STRING);
            }
            break;
        }
        case CSR_BT_HF_CUR_CON_DETAILS_KEY:
        {
            HfHsCommonInstData_t *link;

            link = &(instData->conInstData[instData->currentConnection]);
            if (link->linkState == connected_s)
            {
                sprintf(buf, "CUR==>%s; \n Address:%04X:%02X:%06X; \ncon ID: %u",
                    ((link->linkType == CSR_BT_HF_CONNECTION_HF)? "HF":"HS"),link->bdAddr.nap, link->bdAddr.uap,link->bdAddr.lap,link->conId);
            }
            else
            {
            sprintf(buf, "There is no Active connection!!");
            }
            break;
        }
        case CSR_BT_HF_CALL_HANDLING_KEY:
        {
            break;
        }
        case CSR_BT_HF_CSR_CSR_KEY:
        {
            break;
        }
        case CSR_BT_HF_PRINT_LOCAL_SUP_FEAT_KEY:
        {
            printLocalSupportedFeatures(instData);
            CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);
            return;
        }
        case CSR_BT_HF_PRINT_SUP_HFG_FEAT_KEY:
        {
            printHfgSupportedFeatures(instData, instData->conInstData[instData->currentConnection].hfgSupportedFeatures);
            CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);
            return;
        }
        case CSR_BT_HF_AT_CMD_PROMPT_KEY:
        {
            instData->state = enteringCommand;
            CsrBtHfSetInputDialog(instData, CSR_BT_HF_DEFAULT_INPUT_UI,
               CONVERT_TEXT_STRING_2_UCS2("AT Cmd Prompt"), CONVERT_TEXT_STRING_2_UCS2("Enter the AT Command string:"),
               CSR_UI_ICON_KEY, NULL, 128, CSR_UI_KEYMAP_ALPHANUMERIC,TEXT_OK_UCS2, NULL);
            CsrBtHfShowUi(instData, CSR_BT_HF_DEFAULT_INPUT_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HF_STD_PRIO);
            CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);
            return;
        }
        case CSR_BT_HF_OPR_NW_NAME_KEY:
        {
            if (instData->state == operatorQuery)
            {
                sprintf(buf, "Quey already sent. Waiting for answer...\n");
            }
            else if (instData->conInstData[instData->currentConnection].linkType == CSR_BT_HF_CONNECTION_HF)
            {
                sprintf(buf, "Query network operator is sent!!\n");
                instData->state= operatorQuery;
                CsrBtHfCopsReqSend('3','0',TRUE, instData->conInstData[instData->currentConnection].conId);
            }
            else
            {
                sprintf(buf, "Select active HF connection before requesting action\n");
                sprintf(buf1, TEXT_FAILED_STRING);
            }
            break;
        }
        case CSR_BT_HF_QUERY_SUBSC_NO_INFO_KEY:
        {
            if (instData->conInstData[instData->currentConnection].linkType == CSR_BT_HF_CONNECTION_HF)
            {
                sprintf(buf, "Query subscriber number information ...\n");
                CsrBtHfGetSubscriberNumberInformationReqSend(instData->conInstData[instData->currentConnection].conId);
            }
            else
            {
                sprintf(buf, "Select active HF connection before requesting action\n");
            }
            break;
        }
        case CSR_BT_HF_TOG_LOC_VOICE_RECOG_KEY:
        {
            if (instData->conInstData[instData->currentConnection].linkType == CSR_BT_HF_CONNECTION_HF)
            {
                /* Toggle HF's local voice recognition */
                if (instData->conInstData[instData->currentConnection].localSupportedFeatures & CSR_BT_HF_SUPPORT_VOICE_RECOGNITION)
                {
                    if (instData->conInstData[instData->currentConnection].localActivatedFeatures & CSR_BT_HF_SUPPORT_VOICE_RECOGNITION)
                    {
                        /* True */
                        CsrUint32 mask = 0xffffffff ^ CSR_BT_HF_SUPPORT_VOICE_RECOGNITION;
                        /* remove the bit */
                        instData->conInstData[instData->currentConnection].localActivatedFeatures &= mask;
                    }
                    else
                    {
                        /* FALSE */
                        /* add the bit */
                        instData->conInstData[instData->currentConnection].localActivatedFeatures |= CSR_BT_HF_SUPPORT_VOICE_RECOGNITION;
                    }

                    sprintf(buf, "The HF Voice Recognition is: %s\n",
                           ((instData->conInstData[instData->currentConnection].localActivatedFeatures & CSR_BT_HF_SUPPORT_VOICE_RECOGNITION) ? "On" : "Off"));
                }
                else
                {
                    sprintf(buf, "Voice recognition is not supported.\n");
                    sprintf(buf1, TEXT_FAILED_STRING);
                }
            }
            else
            {
                sprintf(buf, "HF Voice recognition On/Off request is only applicable when a HF connection is active\n");
                sprintf(buf1, TEXT_FAILED_STRING);
            }
            break;
        }
        case CSR_BT_HF_TOG_HFG_VOICE_RECOG_KEY:
        /* HFG Voice recognition activation/deactivation */
        {
            if ((instData->conInstData[instData->currentConnection].linkType == CSR_BT_HF_CONNECTION_HF) &&
                (instData->conInstData[instData->currentConnection].hfgSupportedFeatures & CSR_BT_HFG_SUPPORT_VOICE_RECOGNITION))
            {
                if ( instData->voiceRecognitionOn )
                {
                    /* True */
                    instData->voiceRecognitionOn = FALSE;
                    CsrBtHfSetVoiceRecognitionReqSend(instData->conInstData[instData->currentConnection].conId,FALSE);
                }
                else
                {
                    instData->voiceRecognitionOn = TRUE;
                    CsrBtHfSetVoiceRecognitionReqSend(instData->conInstData[instData->currentConnection].conId,TRUE);
                }
                sprintf(buf, "The HFG's voice recognition is set to: %s\n",((instData->voiceRecognitionOn) ? "On" : "Off"));
            }
            else
            {
                sprintf(buf, "HFG Voice recognition On/Off request is only applicable when HF connection is active\n");
                sprintf(buf1, TEXT_FAILED_STRING);
            }
            break;
        }
        case CSR_BT_HF_MIC_VOL_INCREASE_KEY:
        {
            if (instData->conInstData[instData->currentConnection].linkType != CSR_BT_HF_CONNECTION_UNKNOWN)
            {
                if (instData->conInstData[instData->currentConnection].micGain < MAX_MIC_GAIN)
                {
                    instData->conInstData[instData->currentConnection].micGain++;
                    CsrBtHfMicGainStatusReqSend(instData->conInstData[instData->currentConnection].micGain,
                                     instData->conInstData[instData->currentConnection].conId);
                    sprintf(buf, "Mic. volume up  : %i\n", instData->conInstData[instData->currentConnection].micGain);
                }
                else
                {
                    sprintf(buf, "Mic. volume up  : %i >= Max %i\n", instData->conInstData[instData->currentConnection].micGain, MAX_MIC_GAIN);
                }
            }
            else
            {
                sprintf(buf, "Select active connection before requesting mic functionality.\n");
                sprintf(buf1, TEXT_FAILED_STRING);
            }
            break;
        }
        case CSR_BT_HF_MIC_VOL_DECREASE_KEY:
        {
            if (instData->conInstData[instData->currentConnection].linkType != CSR_BT_HF_CONNECTION_UNKNOWN)
            {
                if (instData->conInstData[instData->currentConnection].micGain > MIN_MIC_GAIN)
                {
                    instData->conInstData[instData->currentConnection].micGain--;
                    CsrBtHfMicGainStatusReqSend(instData->conInstData[instData->currentConnection].micGain,
                                     instData->conInstData[instData->currentConnection].conId);
                    sprintf(buf, "Mic. volume down: %i\n", instData->conInstData[instData->currentConnection].micGain);
                }
                else
                {
                    sprintf(buf, "Mic. volume down: %i <= Min %i\n", instData->conInstData[instData->currentConnection].micGain, MIN_MIC_GAIN);
                }
            }
            else
            {
                sprintf(buf, "Select active connection before requesting mic functionality.\n");
                sprintf(buf1, TEXT_FAILED_STRING);
            }
            break;
        }
        case CSR_BT_HF_SPEAKER_VOL_INCREASE_KEY:
        {
            if (instData->conInstData[instData->currentConnection].linkType != CSR_BT_HF_CONNECTION_UNKNOWN)
            {
                if (instData->conInstData[instData->currentConnection].speakerGain < MAX_SPK_GAIN)
                {
                    instData->conInstData[instData->currentConnection].speakerGain++;
                    CsrBtHfSpeakerGainStatusReqSend(instData->conInstData[instData->currentConnection].speakerGain,
                                         instData->conInstData[instData->currentConnection].conId);
                    sprintf(buf, "Spk. volume up  : %i\n", instData->conInstData[instData->currentConnection].speakerGain);
                }
                else
                {
                    sprintf(buf, "Spk. volume up  : %i >= Max %i\n", instData->conInstData[instData->currentConnection].speakerGain, MAX_SPK_GAIN);
                }
            }
            else
            {
                sprintf(buf, "Select active connection before requesting speaker functionality.\n");
                sprintf(buf1, TEXT_FAILED_STRING);
            }
            break;
        }
        case CSR_BT_HF_SPEAKER_VOL_DECREASE_KEY:
        {
            if (instData->conInstData[instData->currentConnection].linkType != CSR_BT_HF_CONNECTION_UNKNOWN)
            {
                if (instData->conInstData[instData->currentConnection].speakerGain > MIN_SPK_GAIN)
                {
                    instData->conInstData[instData->currentConnection].speakerGain--;
                    CsrBtHfSpeakerGainStatusReqSend(instData->conInstData[instData->currentConnection].speakerGain,
                                         instData->conInstData[instData->currentConnection].conId);
                    sprintf(buf, "Spk. volume down: %i\n", instData->conInstData[instData->currentConnection].speakerGain);
                }
                else
                {
                    sprintf(buf, "Spk. volume up  : %i <= Min %i\n", instData->conInstData[instData->currentConnection].speakerGain, MIN_SPK_GAIN);
                }
            }
            else
            {
                sprintf(buf, "Select active connection before requesting speaker functionality.\n");
                sprintf(buf1, TEXT_FAILED_STRING);
            }
            break;
            }
        case CSR_BT_HF_TOG_CIEV_CMDS_KEY:
        {
            if (instData->conInstData[instData->currentConnection].linkType != CSR_BT_HF_CONNECTION_UNKNOWN)
            {
                if (instData->conInstData[instData->currentConnection].cmerStatus == FALSE)
                {
                    CsrBtHfSetExtendedAgErrorResultCodeReqSend(instData->conInstData[instData->currentConnection].conId, TRUE);
                    instData->conInstData[instData->currentConnection].cmerStatus = TRUE;
                    sprintf(buf, "Sending AT+CMER to HFG to enable CIEV to be send again\n");
                }
                else
                {
                    CsrBtHfSetExtendedAgErrorResultCodeReqSend(instData->conInstData[instData->currentConnection].conId, FALSE);
                    instData->conInstData[instData->currentConnection].cmerStatus = FALSE;
                    sprintf(buf, "Sending AT+CMER to HFG to disable CIEV to be send\n");
                }
            }
            else
            {
                sprintf(buf, "Select active connection before requesting CIEV functionality\n");
                sprintf(buf1, TEXT_FAILED_STRING);
            }
            break;
        }
        case CSR_BT_HF_TOG_RF_SHIELD_KEY:
        {
            if (instData->rfShield == FALSE)
            {
                instData->rfShield = TRUE;
                CsrBccmdWriteReqSend(CSR_BT_HF_APP_IFACEQUEUE, DISABLE_TX_VARID, 0, 0, NULL);
                sprintf(buf, "RF shield activated\n");
            }
            else
            {
                instData->rfShield = FALSE;
                CsrBccmdWriteReqSend(CSR_BT_HF_APP_IFACEQUEUE, ENABLE_TX_VARID, 0, 0, NULL);
                sprintf(buf, "RF shield deactivated\n");
            }
            break;
        }
        case CSR_BT_HF_SELECT_COD_KEY:
        {
            break;
        }
        case CSR_BT_HF_TOG_NR_EC_KEY:
        {
            /* Toggle HF's local Noise Reduction and Echo Cancellation */
            if (instData->conInstData[instData->currentConnection].localSupportedFeatures & CSR_BT_HF_SUPPORT_EC_NR_FUNCTION)
            {
                if (instData->conInstData[instData->currentConnection].localActivatedFeatures & CSR_BT_HF_SUPPORT_EC_NR_FUNCTION)
                {
                    /* True */
                    CsrUint32 mask = 0xffffffff ^ CSR_BT_HF_SUPPORT_EC_NR_FUNCTION;
                    /* remove the bit */
                    instData->conInstData[instData->currentConnection].localActivatedFeatures &= mask;
                }
                else
                {
                    /* FALSE */
                    /* add the bit */
                    instData->conInstData[instData->currentConnection].localActivatedFeatures |= CSR_BT_HF_SUPPORT_EC_NR_FUNCTION;
                }
                sprintf(buf, "The HF Noise Reduction and Echo Cancellation is: %s\n",
                       ((instData->conInstData[instData->currentConnection].localActivatedFeatures & CSR_BT_HF_SUPPORT_EC_NR_FUNCTION) ? "On" : "Off"));
            }
            else
            {
                sprintf(buf, "Noise Reduction and Echo Cancellation not supported.\n");
                sprintf(buf1, TEXT_FAILED_STRING);
            }
            break;
        }
        case CSR_BT_HF_TOG_HFG_NR_EC_KEY:
        {
            /* Toggle HFG's Noise Reduction and Echo Cancellation */
            CsrBool  enable = FALSE;

            if (instData->conInstData[instData->currentConnection].hfgSupportedFeatures & CSR_BT_HFG_SUPPORT_EC_NR_FUNCTION)
            {   /* True: disable */
                CsrUint32 mask = 0xffffffff ^ CSR_BT_HFG_SUPPORT_EC_NR_FUNCTION;
                /* remove the bit */
                instData->conInstData[instData->currentConnection].hfgSupportedFeatures &= mask;
            }
            else
            {
                /* FALSE: enable */
                /* add the bit */
                instData->conInstData[instData->currentConnection].hfgSupportedFeatures |= CSR_BT_HFG_SUPPORT_EC_NR_FUNCTION;

                enable = TRUE;
            }
            CsrBtHfSetEchoAndNoiseReqSend(instData->conInstData[instData->currentConnection].conId,enable);
            sprintf(buf, "The HFG's Noise Reduction and Echo Cancellation is set to: %s\n",((instData->conInstData[instData->currentConnection].hfgSupportedFeatures & CSR_BT_HFG_SUPPORT_EC_NR_FUNCTION) ? "On" : "Off"));
            break;
        }
        case CSR_BT_HF_IND_INDICATOR_ACT_KEY:
        {/* Enter IIA menu */
            if (HfGetNrActiveConnections(instData) > 0)
            {
                if (instData->conInstData[instData->currentConnection].cindString != NULL)
                {
                    CsrBtHfShowUi(instData, CSR_BT_IND_INDICATOR_ACT_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HF_STD_PRIO);
                    CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);
                    return;
                }
                else
                {/* No CIND string found */
                    sprintf(buf, "No indicator string found for the current connection. Please choose a HFP connection.\n");
                }
            }
            else
            {
                sprintf(buf, "No connections exist; access denied to this submenu! \n");
            }
            sprintf(buf1, "Indicators setting");
            break;
        }
        case CSR_BT_HF_CHOOSE_ACTIVE_CON_KEY:
        {
            HfHsCommonInstData_t *link;
            CsrUint8 i;
            if (HfGetNrActiveConnections(instData) == 1)
            {
                sprintf(buf, "Only one connection exists\n");
                sprintf(buf1, "Choose active connection");
            }
            else if(HfGetNrActiveConnections(instData) == 2)
            {
                for (i=0; i<MAX_NUM_CONNECTION ;i++)
                {
                    link = &(instData->conInstData[i]);
                    if (link->linkState == connected_s)
                    {
                        if (i == instData->currentConnection)
                        {
                            sprintf(buf, "CUR==>%s; %04X:%02X:%06X; con ID: %u",
                                ((link->linkType == CSR_BT_HF_CONNECTION_HF)? "HF":"HS"),link->bdAddr.nap, link->bdAddr.uap,link->bdAddr.lap,link->conId);
                        }
                        else
                        {
                            sprintf(buf, "CUR==>%s; %04X:%02X:%06X; con ID: %u",
                                ((link->linkType == CSR_BT_HF_CONNECTION_HF)? "HF":"HS"),link->bdAddr.nap, link->bdAddr.uap,link->bdAddr.lap,link->conId);
                        }
                    }
                    else
                    {
                        sprintf(buf, "NO_CONNECTION");
                        sprintf(buf1, TEXT_FAILED_STRING);
                    }
                    CsrUiMenuAdditemReqSend(csrUiVar->displayesHandlers[CSR_BT_CHOOSE_ACTIVE_CON_UI].displayHandle,
                                    CSR_UI_LAST, i, CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2(buf),
                                    NULL, csrUiVar->displayesHandlers[CSR_BT_CHOOSE_ACTIVE_CON_UI].sk1EventHandle,
                                    CSR_UI_DEFAULTACTION, csrUiVar->displayesHandlers[CSR_BT_CHOOSE_ACTIVE_CON_UI].backEventHandle,
                                    CSR_UI_DEFAULTACTION);
                }
                CsrBtHfShowUi(instData, CSR_BT_CHOOSE_ACTIVE_CON_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HF_STD_PRIO);
                CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);
                return;
            }
            break;
        }
        case CSR_BT_HF_TOG_CODECS_SUPPORT_KEY:
        {
            /* Enable/disable WBS_MSBC_CODEC */
            if (instData->codecEnabled)
            {
                instData->codecEnabled = FALSE;
                sprintf(buf, "Codec disabled\n");
            }
            else
            {
                instData->codecEnabled = TRUE;
                sprintf(buf, "Codec enabled\n");
            }            
            CsrBtHfUpdateSupportedCodecReqSend(CSR_BT_WBS_MSBC_CODEC_MASK, instData->codecEnabled, TRUE);
            break;
        }

        case CSR_BT_HF_TOG_CODECS_SUPPORT_DONT_SEND_KEY:
        {
            /* Enable/disable WBS_MSBC_CODEC */
            if (instData->codecEnabled)
            {
                instData->codecEnabled = FALSE;
                sprintf(buf, "Codec disabled, but not sent\n");
            }
            else
            {
                instData->codecEnabled = TRUE;
                sprintf(buf, "Codec enabled, but not sent\n");
            }            
            CsrBtHfUpdateSupportedCodecReqSend(CSR_BT_WBS_MSBC_CODEC_MASK, instData->codecEnabled, FALSE);
            break;
        }
        default :
        {
            CsrGeneralException("CSR_BT_HF", 0, 0,
            "Unexpected menu key is received");
            break;
        }
    }
    CsrBtHfSetDialog(instData, CSR_BT_HF_DEFAULT_INFO_UI, CONVERT_TEXT_STRING_2_UCS2(buf1),
                                    CONVERT_TEXT_STRING_2_UCS2(buf), CONVERT_TEXT_STRING_2_UCS2(buf2), NULL);
    /* Show the CSR_BT_HF_DEFAULT_INFO_UI on the display                   */
    CsrBtHfShowUi(instData, CSR_BT_HF_DEFAULT_INFO_UI, inputMode, CSR_BT_HF_STD_PRIO);
    CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);
}

void updateHfMainMenu(DemoInstdata_t *inst)
{
    CsrBtHfCsrUiType    * csrUiVar = &(inst->csrUiVar);
    CsrUieHandle displayHandle = csrUiVar->displayesHandlers[CSR_BT_HF_MAIN_MENU_UI].displayHandle;
    CsrUieHandle sk1EventHandle = csrUiVar->displayesHandlers[CSR_BT_HF_MAIN_MENU_UI].sk1EventHandle;
    CsrUieHandle backEventHandle = csrUiVar->displayesHandlers[CSR_BT_HF_MAIN_MENU_UI].backEventHandle;


    CsrUiMenuRemoveallitemsReqSend(csrUiVar->displayesHandlers[CSR_BT_HF_MAIN_MENU_UI].displayHandle);

    if(!inst->serverActivated)
    {
        CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_HF_ACT_REM_AUDIO_CTRL_ENA_KEY,
                        CSR_UI_ICON_NONE, TEXT_HF_ACT_REM_AUD_CTRL_ENA_UCS2, NULL, sk1EventHandle,
                        CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);

        CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_ACT_REM_AUDIO_CTRL_DIS_KEY,
                        CSR_UI_ICON_NONE, TEXT_HF_ACT_REM_AUD_CTRL_DIS_UCS2, NULL, sk1EventHandle,
                        CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);

        CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_ACT_NO_VOICE_RECOG_KEY,
                        CSR_UI_ICON_NONE, TEXT_HF_ACT_NO_VOICE_RECOG_UCS2, NULL, sk1EventHandle ,
                        CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);

        CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_ACT_NO_HS_KEY,
                        CSR_UI_ICON_NONE, TEXT_HF_ACT_NO_HS_UCS2, NULL, sk1EventHandle,
                        CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);

    }
    else if(inst->serverActivated && ((inst->conInstData[CSR_BT_HF_CONNECTION_HF-1].linkState == CSR_BT_HF_CONNECTION_UNKNOWN) &&
                    (inst->conInstData[CSR_BT_HF_CONNECTION_HS-1].linkState == CSR_BT_HF_CONNECTION_UNKNOWN) ))
    {
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_HF_DEACT_HF_SERVER_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_DEACT_HF_SERVER_UCS2, NULL, sk1EventHandle,
                            CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_REACT_HF_SERVER_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_REACT_HF_SERVER_UCS2, NULL,
                            csrUiVar->displayesHandlers[CSR_BT_HF_REACTIVATE_SERVER_UI].displayHandle,
                            CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_EST_SLC_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_EST_SLC_UCS2, NULL, sk1EventHandle,
                            CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_SELECT_COD_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_SELECT_COD_UCS2, NULL,
                            csrUiVar->displayesHandlers[CSR_BT_COD_SELECTION_MENU_UI].displayHandle,
                            CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_PRINT_LOCAL_SUP_FEAT_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_PRINT_LOCAL_SUP_FEAT_UCS2, NULL, sk1EventHandle,
                            CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_PRINT_SUP_HFG_FEAT_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_PRINT_SUP_HFG_FEAT_UCS2, NULL, sk1EventHandle,
                            CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_TOG_RF_SHIELD_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_TOG_RF_SHIELD_UCS2, NULL, sk1EventHandle,
                            CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);
    }
    else
    {
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_DISC_SLC_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_DISC_SLC_UCS2, NULL, sk1EventHandle,
                            CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_CUR_CON_DETAILS_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_CUR_CON_DETAILS_UCS2, NULL, sk1EventHandle,
                            CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_CALL_HANDLING_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_CALL_HANDLING_UCS2, NULL,
                            csrUiVar->displayesHandlers[CSR_BT_CALL_HANDLING_MENU_UI].displayHandle,
                            CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_CSR_CSR_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_CSR_CSR_UCS2, NULL,
                            csrUiVar->displayesHandlers[CSR_BT_C2C_MENU_UI].displayHandle,
                            CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_PRINT_LOCAL_SUP_FEAT_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_PRINT_LOCAL_SUP_FEAT_UCS2, NULL, sk1EventHandle,
                            CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_PRINT_SUP_HFG_FEAT_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_PRINT_SUP_HFG_FEAT_UCS2, NULL, sk1EventHandle,
                            CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_AT_CMD_PROMPT_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_AT_CMD_PROMPT_UCS2, NULL, sk1EventHandle,
                            CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_OPR_NW_NAME_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_OPR_NW_NAME_UCS2, NULL, sk1EventHandle,
                            CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_QUERY_SUBSC_NO_INFO_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_QUERY_SUBSC_NO_INFO_UCS2, NULL, sk1EventHandle,
                            CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_TOG_LOC_VOICE_RECOG_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_TOG_LOC_VOICE_RECOG_UCS2, NULL, sk1EventHandle,
                            CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_TOG_HFG_VOICE_RECOG_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_TOG_HFG_VOICE_RECOG_UCS2, NULL, sk1EventHandle,
                            CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_MIC_VOL_INCREASE_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_MIC_VOL_INCREASE_UCS2, NULL, sk1EventHandle,
                            CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_MIC_VOL_DECREASE_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_MIC_VOL_DECREASE_UCS2, NULL, sk1EventHandle,
                            CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_SPEAKER_VOL_INCREASE_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_SPEAKER_VOL_INCREASE_UCS2, NULL, sk1EventHandle,
                            CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_SPEAKER_VOL_DECREASE_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_SPEAKER_VOL_DECREASE_UCS2, NULL, sk1EventHandle,
                            CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_TOG_CIEV_CMDS_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_TOG_CIEV_CMDS_UCS2, NULL, sk1EventHandle,
                            CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_TOG_RF_SHIELD_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_TOG_RF_SHIELD_UCS2, NULL, sk1EventHandle,
                            CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_TOG_NR_EC_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_TOG_NR_EC_UCS2, NULL, sk1EventHandle,
                            CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_TOG_HFG_NR_EC_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_TOG_HFG_NR_EC_UCS2, NULL, sk1EventHandle,
                            CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_IND_INDICATOR_ACT_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_IND_INDICATOR_ACT_UCS2, NULL, sk1EventHandle,
                            CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_CHOOSE_ACTIVE_CON_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_CHOOSE_ACTIVE_CON_UCS2, NULL, sk1EventHandle,
                            CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_TOG_CODECS_SUPPORT_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_TOG_CODECS_SUPPORT_UCS2, NULL, sk1EventHandle,
                            CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_TOG_CODECS_SUPPORT_DONT_SEND_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_TOG_CODECS_SUPPORT_DONT_SEND_UCS2, NULL, sk1EventHandle,
                            CSR_UI_DEFAULTACTION, backEventHandle, CSR_UI_DEFAULTACTION);

    }
}
static void appCreateHfMainMenuUi(DemoInstdata_t * inst)
{ /* This function Creates the CSR_BT_HF_MAIN_MENU_UI                              */

    CsrBtHfCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_MENU)
    {
        csrUiVar->eventState                                              = CSR_BT_HF_CREATE_SK1_EVENT;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHfMainMenu;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Set the menu on the CSR_BT_HF_MAIN_MENU_UI                               */
        CsrUiMenuSetReqSend(prim->handle, TEXT_MAIN_MENU_UCS2, TEXT_SELECT_UCS2, NULL);

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    {
        if (csrUiVar->eventState == CSR_BT_HF_CREATE_SK1_EVENT)
        { /* Save the event handle as SK1, and set input mode to blocked        */

            csrUiVar->eventState                                          = CSR_BT_HF_CREATE_BACK_EVENT;
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Create a BACK event                                              */
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else
        { /* CSR_BT_HF_CREATE_BACK_EVENT                                           */
            /* Save the event handle as Back, and set input mode to blocked     */
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            csrUiVar->uiIndex = CSR_BT_HF_CALL_LIST_UI;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);

        }
    }
}

static void appUiKeyHandlerCalllistMenu(void * instData,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from the CSR_BT_HF_REACTIVATE_SERVER_UI */
    DemoInstdata_t* inst = instData;

    CSR_UNUSED(eventHandle);

    inst->selectedCall.index = (CsrUint8)key;

    CsrBtHfShowUi(inst, CSR_BT_HF_CHLD_SPECIFIC_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HF_STD_PRIO);
    CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);
}

static void appCreateHfCalllistUi(DemoInstdata_t * inst)
{ /* This function Creates the CSR_BT_HF_CALL_LIST_UI                              */

    CsrBtHfCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_MENU)
    {
        csrUiVar->eventState                                              = CSR_BT_HF_CREATE_SK1_EVENT;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerCalllistMenu;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Set the menu on the CSR_BT_HF_CALL_LIST_UI                               */
        CsrUiMenuSetReqSend(prim->handle, TEXT_MAIN_MENU_UCS2, TEXT_SELECT_UCS2, NULL);

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    {
        if (csrUiVar->eventState == CSR_BT_HF_CREATE_SK1_EVENT)
        { /* Save the event handle as SK1, and set input mode to blocked        */

            csrUiVar->eventState                                          = CSR_BT_HF_CREATE_BACK_EVENT;
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Create a BACK event                                              */
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else
        { /* CSR_BT_HF_CREATE_BACK_EVENT                                           */
            /* Save the event handle as Back, and set input mode to blocked     */
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());


            csrUiVar->uiIndex = CSR_BT_HF_CHLD_SPECIFIC_UI;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);

        }
    }
}


static void appUiKeyHandlerChldMenu(void * instData,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from the CSR_BT_HF_REACTIVATE_SERVER_UI */
    DemoInstdata_t* inst = instData;
    CsrCharString buf[50];

    CSR_UNUSED(eventHandle);

    inst->reactKey = (CsrUint8)key;

    switch (key)
    {
        case CSR_BT_HF_CHLD_1X_KEY:
        {
            /*Specify call and release*/
                sprintf(buf, "Release requested with index %i\n", inst->selectedCall.index);
                CsrBtHfCallHandlingReqSend(CSR_BT_RELEASE_SPECIFIED_CALL, inst->selectedCall.index, inst->conInstData[inst->currentConnection].conId);
            break;
        }
        case CSR_BT_HF_CHLD_2X_KEY:
        {
            sprintf(buf, "Private consultation requested with index %i\n", inst->selectedCall.index);
            CsrBtHfCallHandlingReqSend(CSR_BT_REQUEST_PRIVATE_WITH_SPECIFIED, inst->selectedCall.index, inst->conInstData[inst->currentConnection].conId);
            inst->selectedCall.index = 0xFF;
            break;
        }
        default :
        {
            CsrGeneralException("CSR_BT_HF", 0, 0,
            "Unexpected menu key is received");
            break;
        }
    }
    CsrBtHfSetDialog(inst, CSR_BT_HF_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
    CsrBtHfShowUi(inst, CSR_BT_HF_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HF_STD_PRIO);
    CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);
}

static void appCreateHfChldSpecificUi(DemoInstdata_t * inst)
{ /* This function Creates the CSR_BT_HF_CHLD_SPECIFIC_UI                              */

    CsrBtHfCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_MENU)
    {
        csrUiVar->eventState                                              = CSR_BT_HF_CREATE_SK1_EVENT;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerChldMenu;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Set the menu on the CSR_BT_HF_CHLD_SPECIFIC_UI                               */
        CsrUiMenuSetReqSend(prim->handle, TEXT_MAIN_MENU_UCS2, TEXT_SELECT_UCS2, NULL);

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    {
        if (csrUiVar->eventState == CSR_BT_HF_CREATE_SK1_EVENT)
        { /* Save the event handle as SK1, and set input mode to blocked        */

            csrUiVar->eventState                                          = CSR_BT_HF_CREATE_BACK_EVENT;
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Create a BACK event                                              */
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else
        { /* CSR_BT_HF_CREATE_BACK_EVENT                                           */

            CsrUieHandle displayHandle  = csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle;
            CsrUieHandle sk1Handle      = csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle;

            /* Save the event handle as Back, and set input mode to blocked     */
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_HF_CHLD_1X_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_CHLD_1X_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_HF_CHLD_2X_KEY,
                            CSR_UI_ICON_NONE, TEXT_HF_CHLD_2X_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            csrUiVar->uiIndex = CSR_BT_HF_DEFAULT_INPUT_UI;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_INPUTDIALOG);

        }
    }
}


static void appCreateHfDefaultInput(DemoInstdata_t * inst)
{ /* This function Creates the CSR_BT_HF_DEFAULT_INPUT_UI                               */

    CsrBtHfCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_INPUTDIALOG)
    {
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appHfUiKeyHandlerGetInputDialogMisc;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        csrUiVar->eventState = CSR_BT_HF_CREATE_SK1_EVENT;
    }
    else
    { /* Save the event handle as SK1, and set input mode to blocked       */
        if (csrUiVar->eventState == CSR_BT_HF_CREATE_SK1_EVENT)
        { /* Save the event handle as SK1, and set input mode to blocked        */

            csrUiVar->eventState                                          = CSR_BT_HF_CREATE_BACK_EVENT;
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Create a BACK event                                              */
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else
        { /* CSR_BT_GAP_CREATE_BACK_EVENT                                           */
            /* Save the event handle as Back, and set input mode to blocked     */
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());
        }


    }
}

static const CsrBtHfEventType csrUiHfUieCreateCfmHandlers[NUMBER_OF_CSR_BT_HF_APP_UI] =
{
    appCreateHfDefaultInfoDialogUi,   /* CSR_BT_HF_DEFAULT_INFO_UI           */
    appCreateHfReactivateServerUi,                              /* CSR_BT_HF_REACTIVATE_SERVER_UI           */
    appCreateHfReqScoSplSettingsUi,         /* CSR_BT_REQUEST_SCO_SPL_SETTINGS_UI */
    appCreateHfEnhCallHandlingUi,               /* CSR_BT_ENH_CALL_HANDLING_MENU_UI                   */
    appCreateHfResHoldOptionsUi,      /* CSR_BT_RESPONSE_HOLD_OPTIONS_UI                   */
    appCreateHfCallHandlingUi,        /* CSR_BT_CALL_HANDLING_MENU_UI                   */
    appCreateHfChangeAudioCodecUi,                      /* CSR_BT_CHANGE_AUDIO_CODEC_UI              */
    appCreateHfC2CUi,                        /* CSR_BT_C2C_MENU_UI */
    appCreateHfCodSelectionUi,                        /* CSR_BT_COD_SELECTION_MENU_UI */
    appCreateHfIndIndicatorActUi,                /* CSR_BT_IND_INDICATOR_ACT_UI */
    appCreateHfChooseActiveConUi,                /*  CSR_BT_CHOOSE_ACTIVE_CON_UI    */
    appCreateHfMainMenuUi,                /*  CSR_BT_HF_MAIN_MENU_UI    */
    appCreateHfCalllistUi,                       /* CSR_BT_HF_CALL_LIST_UI */
    appCreateHfChldSpecificUi,                       /* CSR_BT_HF_CHLD_SPECIFIC_UI */
    appCreateHfDefaultInput                       /* CSR_BT_HF_DEFAULT_INPUT_UI */
};

/* CSR_UI upstream handler functions                                              */
void CsrBtHfCsrUiUieCreateCfmHandler(DemoInstdata_t * inst)
{
    if (inst->csrUiVar.uiIndex < NUMBER_OF_CSR_BT_HF_APP_UI)
    {
         csrUiHfUieCreateCfmHandlers[inst->csrUiVar.uiIndex](inst);
    }
    else
    { /* An unexpected number of CSR_UI CsrUiUieCreateCfm messages
         has been received                                                      */
    }
}

void CsrBtHfCsrUiInputdialogGetCfmHandler(DemoInstdata_t* inst)
{
    CsrUiInputdialogGetCfm * prim = (CsrUiInputdialogGetCfm *) inst->recvMsgP;
    CsrCharString buf[35];

    if (prim->handle == inst->csrUiVar.displayesHandlers[CSR_BT_HF_DEFAULT_INPUT_UI].displayHandle)
    {
        switch(inst->state)
        {
            case enteringReactParam:
            {
                CsrUint8 *reactVal = CsrUtf16String2Utf8(prim->text);
                if(CsrStrLen((CsrCharString *) reactVal) != 0)
                {
                    if(inst->reactKey == CSR_BT_HF_MAX_NUM_HF_RECORDS_KEY)
                    {
                        inst->maxHFrecords = *reactVal - '0';
                    }
                    else if(inst->reactKey == CSR_BT_HF_MAX_NUM_HS_RECORDS_KEY)
                    {
                        inst->maxHSrecords = *reactVal - '0';
                    }
                    else if((inst->reactKey == CSR_BT_HF_MAX_NUM_SIMULT_CON_KEY) && (CsrStrLen((CsrCharString *) reactVal) != 0))
                    {
                        inst->maxSimulCons = *reactVal - '0';
                    }
                    sprintf(buf, "Reactivate parameters set!!");
                }
                else
                {
                    sprintf(buf, "Nothing is entered!!");
                }
                break;
            }
            case dialing:
            {
                CsrUint8     *body;
                CsrUint8 *phoneNum = CsrUtf16String2Utf8(prim->text);

                if(CsrStrLen((CsrCharString *) phoneNum) != 0)
                {
                    CsrStrCpy((CsrCharString *) inst->myPhoneNumber.phoneNumber, (CsrCharString *) phoneNum);
                    inst->myPhoneNumber.length = CsrStrLen((CsrCharString *) phoneNum);
                    inst->myPhoneNumber.phoneNumber[inst->myPhoneNumber.length++] = ';';
                    inst->myPhoneNumber.phoneNumber[inst->myPhoneNumber.length++] = '\r';

                    body = CsrPmemAlloc(inst->myPhoneNumber.length+1);
                    CsrMemCpy(body,inst->myPhoneNumber.phoneNumber,inst->myPhoneNumber.length);
                    body[inst->myPhoneNumber.length] = '\0';
                    CsrBtHfDialReqSend(inst->conInstData[inst->currentConnection].conId,CSR_BT_HF_DIAL_NUMBER, (CsrCharString *) body);
                    inst->myPhoneNumber.length = 0;
                    sprintf(buf, "Dial Request is sent!!");
                }
                else
                {
                    sprintf(buf, "Nothing is entered!!");
                }
                break;
            }
            case enteringDTMF:
            {
                CsrUint8 *dtmfTone = CsrUtf16String2Utf8(prim->text);

                if (((*dtmfTone >= '0') && (*dtmfTone <= '9')) || /* 0 -> 9  */
                    (*dtmfTone >= 'A' && *dtmfTone <= 'D')     || /* A -> D  */
                    (*dtmfTone == '#') || (*dtmfTone == '*') )    /* # and * */
                {
                    CsrBtHfGenerateDTMFReqSend(inst->conInstData[inst->currentConnection].conId, *dtmfTone);
                    sprintf(buf, "\n %c DTMF tone is sent!!\n",*dtmfTone);
                }
                else
                {
                    sprintf(buf, "\n %c is not a valid DTMF tone\n",*dtmfTone);
                }
                break;
            }
            case enteringCommand:
            {
                CsrUint8     *body;
                CsrUint8 *atCmd = CsrUtf16String2Utf8(prim->text);
                if(CsrStrLen((CsrCharString *) atCmd) != 0)
                {
                    if(inst->myATCommand.ATCommandLen == 1 && inst->myATCommand.ATCommand[0] == 'Q')
                    {
                        inst->myATCommand.ATCommandLen = 0;
                        inst->myATCommand.ATCommand[0] = '\0';
                    }
                    else
                    {
                        CsrStrCpy((CsrCharString *) inst->myATCommand.ATCommand, (CsrCharString *) atCmd);
                        inst->myATCommand.ATCommandLen = CsrStrLen((CsrCharString *) atCmd);
                        inst->myATCommand.ATCommand[inst->myATCommand.ATCommandLen++] = '\r';

                        body = CsrPmemAlloc(inst->myATCommand.ATCommandLen+1);
                        CsrMemSet(body,0,inst->myATCommand.ATCommandLen+1);

                        CsrMemCpy(body, inst->myATCommand.ATCommand, inst->myATCommand.ATCommandLen);

                        CsrBtHfAtCmdReqSend((CsrUint8) (inst->myATCommand.ATCommandLen+1),body, inst->conInstData[inst->currentConnection].conId);

                        /*Reset command parameter*/
                        inst->myATCommand.ATCommandLen = 0;
                        inst->myATCommand.ATCommand[0] = '\0';
                        sprintf(buf, "\n At Cmd Req is sent!!\n");
                    }
                }
                else
                {
                    sprintf(buf, "Nothing is entered!!");
                }
                break;
            }
            case memDialling:
            {
                CsrUint8 *phoneNum = CsrUtf16String2Utf8(prim->text);
                if (CsrStrLen((CsrCharString *) phoneNum) == 0)
                {
                    sprintf(buf, "No HFG memory location to dial");
                }
                else
                {
                    CsrUint8* body;

                    CsrStrCpy((CsrCharString *) inst->myMemNumber.phoneNumber, (CsrCharString *) phoneNum);
                    inst->myMemNumber.length = CsrStrLen((CsrCharString *) phoneNum);

                    inst->myMemNumber.phoneNumber[inst->myMemNumber.length++] = ';';
                    inst->myMemNumber.phoneNumber[inst->myMemNumber.length++] = '\r';
                    body = CsrPmemAlloc(inst->myMemNumber.length + 1);
                    CsrMemCpy(body,inst->myMemNumber.phoneNumber,inst->myMemNumber.length);
                    body[inst->myMemNumber.length] = '\0';
                    CsrBtHfDialReqSend(inst->conInstData[inst->currentConnection].conId,CSR_BT_HF_DIAL_MEMORY, (CsrCharString *) body);
                    inst->myMemNumber.length = 0;

                    sprintf(buf, "\n Atd> is sent!!\n");
                }
                break;
            }
            default:
            {
                    sprintf(buf, "Unknown state  !!");
            }
        }
    }
    else
    { /* An unexpected CsrUiInputdialogGetCfm is received                        */
        CsrGeneralException("CSR_BT_HF", 0, prim->type,
                           "Unexpected CsrUiInputdialogGetCfm is received");
    }
    CsrBtHfSetDialog(inst, CSR_BT_HF_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
    CsrBtHfShowUi(inst, CSR_BT_HF_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HF_STD_PRIO);
    CsrUiUieHideReqSend(inst->csrUiVar.displayesHandlers[CSR_BT_HF_DEFAULT_INPUT_UI].displayHandle);
    inst->state = idle;
    CsrPmemFree(prim->text);
}


void CsrBtHfCsrUiDisplayGetHandleCfmHandler(DemoInstdata_t* inst)
{
    CsrUiDisplayGethandleCfm *prim = (CsrUiDisplayGethandleCfm*) inst->recvMsgP;

    if(inst->csrUiVar.goToMainmenu && inst->csrUiVar.inHfMenu
            && (prim->handle != inst->csrUiVar.displayesHandlers[CSR_BT_HF_MAIN_MENU_UI].displayHandle))
    {
        CsrUiUieHideReqSend(prim->handle);
        CsrUiDisplayGethandleReqSend(CsrSchedTaskQueueGet());
    }
    else if(prim->handle == inst->csrUiVar.displayesHandlers[CSR_BT_HF_MAIN_MENU_UI].displayHandle)
    {
        updateHfMainMenu(inst);
        inst->csrUiVar.goToMainmenu = FALSE;
    }
}

void CsrBtHfCsrUiEventIndHandler(DemoInstdata_t* inst)
{ /* A CsrUiEventInd is received from the CSR_UI layer                             */
    CsrUint8     i;

    CsrBtHfDisplayHandlesType * uiHandlers = inst->csrUiVar.displayesHandlers;
    CsrUiEventInd            * prim       = (CsrUiEventInd *) inst->recvMsgP;

    for ( i = 0; i < NUMBER_OF_CSR_BT_HF_APP_UI; i++)
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
                    CsrGeneralException("CSR_BT_HF", 0, prim->type,
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
                    CsrGeneralException("CSR_BT_HF", 0, prim->type,
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
                    CsrGeneralException("CSR_BT_HF", 0, prim->type,
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
                    CsrGeneralException("CSR_BT_HF", 0, prim->type,
                        "No function is assign to this DEL event");
                }
            }
            else
            { /* An exception has occurred. No UIEHandle is assign to
                 this event                                                     */
                    CsrGeneralException("CSR_BT_HF", 0, prim->type,
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
    CsrGeneralException("CSR_BT_HF", 0, prim->type,
        "None of the know displayes handles match the received one");
}

void CsrBtHfSetDialog(DemoInstdata_t * inst, CsrUint8 UiIndex,
                            CsrUint16 *heading, CsrUint16 *message,
                            CsrUint16 *textSK1, CsrUint16 *textSK2)
{ /* Set a Dialog Menu on the UI                                            */
    CsrUieHandle dialog, sk1Handle, sk2Handle, backHandle, delHandle;

    dialog      = inst->csrUiVar.displayesHandlers[UiIndex].displayHandle;
    sk1Handle   = inst->csrUiVar.displayesHandlers[UiIndex].sk1EventHandle;
    sk2Handle   = inst->csrUiVar.displayesHandlers[UiIndex].sk2EventHandle;
    backHandle  = inst->csrUiVar.displayesHandlers[UiIndex].backEventHandle;
    delHandle   = inst->csrUiVar.displayesHandlers[UiIndex].deleteEventHandle;

    CsrUiDialogSetReqSend(dialog, heading, message, textSK1, textSK2,
                            sk1Handle, sk2Handle, backHandle, delHandle);
}

void CsrBtHfSetInputDialog(DemoInstdata_t * inst, CsrUint8 UiIndex,
                         CsrUint16 *heading, CsrUint16 *message,
                         CsrUint16 icon, CsrUint16 *text, CsrUint16 textLength,
                         CsrUint8 keyMap, CsrUint16 *textSK1, CsrUint16 *textSK2)
{ /* Set a input Dialog Menu on the UI                                      */
    CsrUieHandle dialog, sk1Handle, sk2Handle, backHandle, delHandle;

    dialog      = inst->csrUiVar.displayesHandlers[UiIndex].displayHandle;
    sk1Handle   = inst->csrUiVar.displayesHandlers[UiIndex].sk1EventHandle;
    sk2Handle   = inst->csrUiVar.displayesHandlers[UiIndex].sk2EventHandle;
    backHandle  = inst->csrUiVar.displayesHandlers[UiIndex].backEventHandle;
    delHandle   = inst->csrUiVar.displayesHandlers[UiIndex].deleteEventHandle;

    CsrUiInputdialogSetReqSend(dialog, heading, message, icon, text, textLength, keyMap,
                            textSK1, textSK2, sk1Handle, sk2Handle, backHandle, delHandle);
}



void CsrBtHfShowUi(DemoInstdata_t * inst, CsrUint8 UiIndex,
                                CsrUint16 inputMode, CsrUint16 priority)
{ /* Show a given display/window from the UI                                */
    CsrUieHandle displayHandle;

    displayHandle = inst->csrUiVar.displayesHandlers[UiIndex].displayHandle;

    CsrUiUieShowReqSend(displayHandle, CsrSchedTaskQueueGet(), inputMode, priority);
    if(UiIndex == CSR_BT_HF_MAIN_MENU_UI)
        inst->csrUiVar.inHfMenu = TRUE;
}


