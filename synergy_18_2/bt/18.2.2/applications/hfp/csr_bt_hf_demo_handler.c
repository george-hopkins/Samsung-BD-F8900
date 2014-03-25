/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

/*  standard include files */
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <stdarg.h>

/*  profile manager include files */
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_env_prim.h"
#include "csr_bt_tasks.h"
#include "bluetooth.h"
#include "csr_bt_profiles.h"
#include "csr_bt_cm_prim.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_sc_prim.h"
#include "csr_bt_sc_lib.h"
#include "csr_bt_util.h"
#include "csr_bt_hf_prim.h"
#include "csr_bt_hf_lib.h"
#include "csr_hci_sco.h"
#include "csr_bt_hf_demo_app.h"
#include "csr_bt_platform.h"
#include "csr_bt_sc_demo_app.h"
#include "csr_bccmd_prim.h"
#include "csr_bt_hf_app_ui_sef.h"
#include "csr_app_lib.h"


void sendC2cInit(DemoInstdata_t *instData,CsrBtHfConnectionId connectionId, CsrBool enable)
{
    if (enable)
    {
        CsrBtHfC2CSfReqSend(connectionId,CSR_BT_C2C_NAME_IND,CSR_BT_C2C_NAME_ON_VALUE);
        CsrBtHfC2CSfReqSend(connectionId,CSR_BT_C2C_TXT_IND,CSR_BT_C2C_TXT_ON_VALUE);
        CsrBtHfC2CSfReqSend(connectionId,CSR_BT_C2C_SMS_IND,CSR_BT_C2C_SMS_ON_VALUE);
        CsrBtHfC2CSfReqSend(connectionId,CSR_BT_C2C_BAT_IND,CSR_BT_C2C_BAT_ON_VALUE);
        CsrBtHfC2CSfReqSend(connectionId,CSR_BT_C2C_PWR_IND,CSR_BT_C2C_PWR_ON_VALUE);
    }
    else
    {
        CsrBtHfC2CSfReqSend(connectionId,CSR_BT_C2C_NAME_IND,CSR_BT_C2C_NAME_OFF_VALUE);
        CsrBtHfC2CSfReqSend(connectionId,CSR_BT_C2C_TXT_IND,CSR_BT_C2C_TXT_OFF_VALUE);
        CsrBtHfC2CSfReqSend(connectionId,CSR_BT_C2C_SMS_IND,CSR_BT_C2C_SMS_OFF_VALUE);
        CsrBtHfC2CSfReqSend(connectionId,CSR_BT_C2C_BAT_IND,CSR_BT_C2C_BAT_OFF_VALUE);
        CsrBtHfC2CSfReqSend(connectionId,CSR_BT_C2C_PWR_IND,CSR_BT_C2C_PWR_OFF_VALUE);
    }

    if(instData->adpcmLocalSupported && enable)
    {/* enable */
        CsrBtHfC2CSfReqSend(connectionId,CSR_BT_C2C_ADPCM_IND,CSR_BT_C2C_ADPCM_OFF_VALUE|CSR_BT_C2C_ADPCM_2BIT_VALUE|CSR_BT_C2C_ADPCM_4BIT_VALUE);
        CsrBtHfC2CSfReqSend(connectionId,CSR_BT_C2C_SAMPLE_RATE_IND,CSR_BT_C2C_SAMPLE_8KHZ_VALUE|CSR_BT_C2C_SAMPLE_16KHZ_VALUE);
    }
    else
    {/* disable */
        CsrBtHfC2CSfReqSend(connectionId,CSR_BT_C2C_ADPCM_IND,0);
        CsrBtHfC2CSfReqSend(connectionId,CSR_BT_C2C_SAMPLE_RATE_IND,0);
    }

}

CsrUint8 HfFindConIdxFromconId(DemoInstdata_t *inst,CsrBtHfConnectionId connId)
{
    CsrUint8 i;

    for (i=0;i<MAX_NUM_CONNECTION;i++)
    {
        if (inst->conInstData[i].conId == connId)
        {
            return i;
        }
    }
    return 0xFF;
}

CsrUint8 HfFindFirstConIdxFree(DemoInstdata_t *inst)
{
    CsrUint8 i;

    for (i=0; i<MAX_NUM_CONNECTION ; i++)
    {
        if (inst->conInstData[i].conId == 0xFF)
        {
            return i;
        }
    }

    return 0xFF;
}

void HfSetC2CAudioCodecIndHandler(DemoInstdata_t *inst)
{
    CsrBtHfSetC2cAudioCodecInd * msg;
    CsrCharString buf[60];

    msg = (CsrBtHfSetC2cAudioCodecInd*)inst->recvMsgP;
    inst->currentConnection = HfFindConIdxFromconId(inst,msg->connectionId);
    inst->conInstData[inst->currentConnection].audioCodecType = msg->value;

    sprintf(buf, "Audio Codec correctly set in the chip,\n Audio connected!\n");

    CsrBtHfSetDialog(inst, CSR_BT_HF_DEFAULT_INFO_UI, CONVERT_TEXT_STRING_2_UCS2("C2C Audio codec"),
                                                            CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
    /* Show the CSR_BT_HF_DEFAULT_INFO_UI on the display                   */
    CsrBtHfShowUi(inst, CSR_BT_HF_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HF_STD_PRIO);
}

void HfGetC2CAdpcmLocalSupportedIndHandler(DemoInstdata_t *inst)
{
    CsrBtHfGetC2cAdpcmLocalSupportedInd * msg;
    msg = (CsrBtHfGetC2cAdpcmLocalSupportedInd*)inst->recvMsgP;

    if(msg->result)
    {
        inst->adpcmLocalSupported = TRUE;
        CsrAppBacklogReqSend(TECH_BT, PROFILE_HF, FALSE, "TADPCM supported by chip");
    }
    sendC2cInit(inst,0xFF,TRUE);
}

void inComingBcspSound(CsrUint8 * theData)
{
    CsrHciSendScoData(theData);
}

static void startup_mic_gain(CsrUint16 m, void *v)
{
    DemoInstdata_t * instData;
    instData = (DemoInstdata_t*)v;
    CsrBtHfMicGainStatusReqSend(instData->conInstData[m].micGain, instData->conInstData[m].conId);
}

char * getHfHsString(CsrBtHfConnectionType theConnectionType)
{
    switch (theConnectionType)
    {
        case CSR_BT_HF_CONNECTION_HF:
            {
                return "HF";
            }
        case CSR_BT_HF_CONNECTION_HS:
            {
                return "HS";
            }
        default:
            {
                break;
            }
    }
    return "None";
}


CsrUint16 handleHfAtCmdInd(DemoInstdata_t *instData, CsrBtHfAtCmdInd *myPrim)
{
    CsrUint16    result;

    CsrUint16    charsConsumed;
    char       *continueFrom;
#if (PRINT_AT_CMD_VERBOSE != 0) || (PRINT_AT_CMD != 0)
    CsrCharString buf[50];
#endif

#if (PRINT_AT_CMD_VERBOSE != 0)
    {
        CsrUint16 i;
        sprintf(buf, "\nReceived AT data: ");
        for (i=0; i< (CsrUint16)CsrStrLen((char *) myPrim->atCmdString); i++)
        {
            switch (myPrim->atCmdString[i])
            {
                case '\r':
                    {
                        CsrStrCat(buf, "\\r");
                        break;
                    }
                case '\n':
                    {
                        CsrStrCat(buf, "\\n");
                        break;
                    }
                default:
                    {
                        CsrCharString b[2];
                        b[0] = myPrim->atCmdString[i];
                        b[1] = '\0';
                        CsrStrCat(buf, b);
                        break;
                    }
            }
        }
    }
#endif

    result = HfGetAtToken( (CsrUint8*)myPrim->atCmdString, &continueFrom, &charsConsumed);

    switch (result)
    {
        case CSR_BT_OK_TOKEN:
            {
#if (PRINT_AT_CMD != 0)
                CsrStrCat(buf, "\nOK_TOKEN\n");
#endif
                }
            break;
        case CSR_BT_ERROR_TOKEN:
            {
#if (PRINT_AT_CMD != 0)
                CsrStrCat(buf, "\nERROR_TOKEN\n");
#endif
            }
            break;

        default:
            {
#if (PRINT_AT_CMD != 0)
                CsrStrCat(buf, "\nUnhandled AT cmd\n");
#endif
            }
            break;
    }

#if (PRINT_AT_CMD_VERBOSE != 0) || (PRINT_AT_CMD != 0)
    CsrBtHfSetDialog(instData, CSR_BT_HF_DEFAULT_INFO_UI, CONVERT_TEXT_STRING_2_UCS2("AT Data"),
                                                            CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
    /* Show the CSR_BT_HF_DEFAULT_INFO_UI on the display                   */
    CsrBtHfShowUi(instData, CSR_BT_HF_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HF_STD_PRIO);
#endif

    return result;
}


char *extractLinktypeString(CsrUint8 linktype)
{
    char *ls = NULL;

    if (linktype == 0)
    {
        ls = LINKTYPE_SCO;
    }
    else if (linktype == 1)
    {
        ls = LINKTYPE_ACL;
    }
    else if (linktype == 2)
    {
        ls = LINKTYPE_ESCO;
    }
    else
    {
        ls = LINKTYPE_RESERVED;
    }
    return ls;
}

/**************************************************************************************************
 *
 *   an Hfg event is received.
 *
 **************************************************************************************************/
void handleHfPrim(DemoInstdata_t * instData)
{
    CsrBtHfPrim    *prim;
    CsrUint16    *primType;
    void        *thePrim = instData->recvMsgP;
    CsrBtHfCsrUiType    * csrUiVar  = &(instData->csrUiVar);
    CsrUint8 inputMode = CSR_UI_INPUTMODE_AUTO;
    CsrCharString buf[100];
    CsrCharString buf1[25] = TEXT_SUCCESS_STRING;

    prim     = (CsrBtHfPrim *) thePrim;
    primType = (CsrUint16 *) prim;

    switch (*primType)
    {
        /******************************************************** INDICATION MESSAGES *********************************/
        case CSR_BT_HF_AUDIO_ACCEPT_CONNECT_IND:
            {
                CsrBtHfAudioAcceptConnectInd *hfPrim;
                hfPrim = (CsrBtHfAudioAcceptConnectInd *)primType;
                instData->currentConnection = HfFindConIdxFromconId(instData,hfPrim->connectionId);

                sprintf(buf, "Incoming audio received from %04X:%02X:%06X accept?(Use Call Handling menu)",
                         instData->conInstData[instData->currentConnection].bdAddr.nap,
                         instData->conInstData[instData->currentConnection].bdAddr.uap,
                         instData->conInstData[instData->currentConnection].bdAddr.lap);
                instData->conInstData[instData->currentConnection].incScoPending = TRUE;
                CsrBtHfSetDialog(instData, CSR_BT_HF_DEFAULT_INFO_UI, CONVERT_TEXT_STRING_2_UCS2("Audio Connection"),
                                                CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_YES_UCS2, TEXT_NO_UCS2);
                /* Show the CSR_BT_HF_DEFAULT_INFO_UI on the display                   */
                CsrBtHfShowUi(instData, CSR_BT_HF_DEFAULT_INFO_UI, inputMode, CSR_BT_HF_STD_PRIO);
                instData->state = acc_rej_audio;
                return;;
            }
        case CSR_BT_HF_DISCONNECT_IND:
        case CSR_BT_HF_DISCONNECT_CFM:
            {
                CsrBtHfDisconnectInd *prim;

                prim = (CsrBtHfDisconnectInd *) thePrim;

                instData->currentConnection = HfFindConIdxFromconId(instData,prim->connectionId);
                if (instData->currentConnection != 0xFF)
                {
                instData->conInstData[instData->currentConnection].startup = STARTUP_MIC;
                instData->conInstData[instData->currentConnection].audioOn = FALSE;
                instData->conInstData[instData->currentConnection].linkState = disconnected_s;
                instData->conInstData[instData->currentConnection].conId    = 0xFF;
                instData->conInstData[instData->currentConnection].incScoPending = FALSE;
                CsrPmemFree(instData->conInstData[instData->currentConnection].cindString);
                CsrPmemFree(instData->conInstData[instData->currentConnection].cindValues);
                instData->conInstData[instData->currentConnection].cindString = NULL;
                instData->conInstData[instData->currentConnection].cindValues = NULL;
                instData->conInstData[instData->currentConnection].indicatorActivation = instData->conInstData[instData->currentConnection].tmpIndicatorActivation = 0xFFFF;
                }

                findFirstActiveConnection(instData);

                /*if(instData->discGuard)
                    CsrSchedTimerCancel(instData->discGuard, NULL , NULL);*/

                if (prim->reasonCode != CSR_BT_RESULT_CODE_HF_SUCCESS ||
                    prim->reasonSupplier != CSR_BT_SUPPLIER_HF)
                {
                    sprintf(buf, "Link lost to %s\n", (instData->conInstData[instData->currentConnection].linkType == CSR_BT_HF_CONNECTION_HF ? "HF" : "HS"));
                }
                else
                {
                    if (*primType == CSR_BT_HF_DISCONNECT_CFM)
                    {
                        sprintf(buf, "CSR_BT_HF_DISCONNECT_CFM received \n");
                    }
                    else
                    {
                        sprintf(buf, "CSR_BT_HF_DISCONNECT_IND received \n");
                    }
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_HF, FALSE, "%s connection completely released; connection Id:  0x%04x\n",
                           (instData->conInstData[instData->currentConnection].linkType == CSR_BT_HF_CONNECTION_HF) ? "Handsfree Gateway\0" : "Audio Gateway\0", prim->connectionId);
                }

                if(*primType == CSR_BT_HF_DISCONNECT_CFM)
                {
                    updateHfMainMenu(instData);
                    CsrBtHfSetDialog(instData, CSR_BT_HF_DEFAULT_INFO_UI, CONVERT_TEXT_STRING_2_UCS2("SLC Disconnect Confirm"),
                    CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
                    CsrBtHfShowUi(instData, CSR_BT_HF_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HF_STD_PRIO);
                }
                else if(csrUiVar->inHfMenu) /*in case of remote disconnect, update the menu*/
                {
                    csrUiVar->goToMainmenu = TRUE;
                    CsrUiDisplayGethandleReqSend(CsrSchedTaskQueueGet());
                }
                break;
            }
        case CSR_BT_HF_AUDIO_DISCONNECT_CFM:
            {
                CsrBtHfAudioDisconnectCfm * prim;
                prim = (CsrBtHfAudioDisconnectCfm *) thePrim;
                instData->currentConnection = HfFindConIdxFromconId(instData,prim->connectionId);
                instData->conInstData[instData->currentConnection].audioOn = FALSE;
                if ((prim->resultSupplier == CSR_BT_SUPPLIER_HF) && (prim->resultCode == CSR_BT_RESULT_CODE_HF_SUCCESS))
                {
                    CsrHciDeRegisterScoHandle(prim->scoHandle);
                }
                sprintf(buf, "CSR_BT_HF_AUDIO_DISCONNECT_CFM received \n");
                break;
            }
        case CSR_BT_HF_AUDIO_DISCONNECT_IND:
            {
                CsrBtHfAudioDisconnectInd * prim;
                prim = (CsrBtHfAudioDisconnectInd *) thePrim;
                instData->currentConnection = HfFindConIdxFromconId(instData,prim->connectionId);
                instData->conInstData[instData->currentConnection].audioOn = FALSE;
                CsrHciDeRegisterScoHandle(prim->scoHandle);
                sprintf(buf, "CSR_BT_HF_AUDIO_DISCONNECT_IND received \n");
                break;
            }
        case CSR_BT_HF_AUDIO_CONNECT_CFM:
        case CSR_BT_HF_AUDIO_CONNECT_IND:
            {
                CsrBool retVal;
                CsrBtHfAudioConnectInd *prim;
                prim = (CsrBtHfAudioConnectInd *) thePrim;

                instData->currentConnection = HfFindConIdxFromconId(instData,prim->connectionId);
                instData->conInstData[instData->currentConnection].audioOn = TRUE;
                sprintf(buf, "%s Audio is %s (Linktype %s)\n",
                       getHfHsString(instData->conInstData[instData->currentConnection].linkType),
                       ((prim->resultSupplier == CSR_BT_SUPPLIER_HF) && (prim->resultCode == CSR_BT_RESULT_CODE_HF_SUCCESS) ? "ON" : "OFF"),
                       extractLinktypeString(prim->linkType));
                instData->conInstData[instData->currentConnection].scoHandle = prim->scoHandle;
                if ((prim->resultSupplier == CSR_BT_SUPPLIER_HF) && (prim->resultCode == CSR_BT_RESULT_CODE_HF_SUCCESS))
                {
                    retVal = CsrHciRegisterScoHandle(prim->scoHandle, (CsrHciScoHandlerFuncType)inComingBcspSound);
                    if (TRUE == retVal)
                    {
                        CsrAppBacklogReqSend(TECH_BT, PROFILE_HF, FALSE, "Telling Chip to use audio codec: 0x%02x\n", instData->audioCodecType);
                    }
                    else
                    {
                        CsrAppBacklogReqSend(TECH_BT, PROFILE_HF, FALSE, "SCO connection was not set up due to missing free SCO handles.\n");
                    }
                }
                break;
            }

        case CSR_BT_HF_SERVICE_CONNECT_CFM:
        case CSR_BT_HF_SERVICE_CONNECT_IND:
            {
                CsrBtHfServiceConnectInd *myPrim;

                myPrim = (CsrBtHfServiceConnectInd *) thePrim;

                instData->currentConnection = HfFindFirstConIdxFree(instData);
                instData->connReqPending = FALSE;
                if(instData->state == cancelState)
                {
                    CsrUiUieHideReqSend(csrUiVar->displayesHandlers[CSR_BT_HF_DEFAULT_INFO_UI].displayHandle);
                    instData->state = idle;
                }
                if ((myPrim->resultCode == CSR_BT_RESULT_CODE_HF_SUCCESS) &&
                    (myPrim->resultSupplier == CSR_BT_SUPPLIER_HF) &&
                    (instData->currentConnection < MAX_NUM_CONNECTION))
                {
                    instData->conInstData[instData->currentConnection].conId  = myPrim->connectionId;

                    if (myPrim->connectionType == CSR_BT_HF_CONNECTION_HF)
                    {
                        sprintf(buf, "SLC established to: %s Remote HFP version supported: 0x%02x \n", myPrim->serviceName, myPrim->remoteVersion);

                        /* Send microphone level */
                        if(instData->conInstData[instData->currentConnection].startup == STARTUP_MIC)
                        {
                            CsrSchedTimerSet(CSR_SCHED_SECOND*2,
                                           startup_mic_gain,
                                           instData->currentConnection,
                                           instData);
                        }
                        instData->conInstData[instData->currentConnection].hfgSupportedFeatures = myPrim->supportedFeatures;
                        instData->conInstData[instData->currentConnection].remoteVersion  = myPrim->remoteVersion;
                    }
                    else
                    {
                        instData->conInstData[instData->currentConnection].localSupportedFeatures   = (CSR_BT_HF_SUPPORT_ALL_FUNCTIONS);
                        instData->conInstData[instData->currentConnection].localActivatedFeatures   = (CSR_BT_HF_SUPPORT_ALL_FUNCTIONS);
                        sprintf(buf, "Headset connected to %s\n", myPrim->serviceName);
                    }

                    instData->conInstData[instData->currentConnection].bdAddr.lap    = myPrim->deviceAddr.lap;
                    instData->conInstData[instData->currentConnection].bdAddr.uap    = myPrim->deviceAddr.uap;
                    instData->conInstData[instData->currentConnection].bdAddr.nap    = myPrim->deviceAddr.nap;
                    instData->conInstData[instData->currentConnection].linkState     = connected_s;
                    instData->conInstData[instData->currentConnection].linkType      = myPrim->connectionType;

                    if (myPrim->indicatorSupported != NULL)
                    {
                        CsrUint16 len = (CsrUint16)(CsrStrLen((char*)myPrim->indicatorSupported) + 1);
                        CsrAppBacklogReqSend(TECH_BT, PROFILE_HF, FALSE, "Remote indicators supported: %s\n",myPrim->indicatorSupported);
                        instData->conInstData[instData->currentConnection].cindString = CsrPmemAlloc(len);
                       CsrStrNCpyZero((char *)instData->conInstData[instData->currentConnection].cindString,
                                       (char*)myPrim->indicatorSupported,len);
                        CsrPmemFree(myPrim->indicatorSupported);
                    }
                    if (myPrim->indicatorValue != NULL)
                    {
                        CsrUint16 len = (CsrUint16)(CsrStrLen((char*)myPrim->indicatorValue) + 1);
                        CsrUint16 i;
                        CsrAppBacklogReqSend(TECH_BT, PROFILE_HF, FALSE, "Actual indicator values: %s \n",myPrim->indicatorValue);
                        instData->conInstData[instData->currentConnection].cindValues = CsrPmemAlloc(len);
                        CsrStrNCpyZero((char *)instData->conInstData[instData->currentConnection].cindValues,
                                       (char*)myPrim->indicatorValue,len);
                        instData->conInstData[instData->currentConnection].indicatorActivation = 1; /* at least one indicator present */
                        for (i=0;myPrim->indicatorValue[i] != 0; i++)
                        {/* Every comma found means an indicator; and all indicators found at this point must be activated */
                            if (myPrim->indicatorValue[i] == ',')
                            {
                                instData->conInstData[instData->currentConnection].indicatorActivation <<= 1;
                                instData->conInstData[instData->currentConnection].indicatorActivation |= 1;
                            }
                        }
                        instData->conInstData[instData->currentConnection].tmpIndicatorActivation = instData->conInstData[instData->currentConnection].indicatorActivation;
                        CsrPmemFree(myPrim->indicatorValue);
                    }
                    if (myPrim->chldString != NULL)
                    {
                        CsrAppBacklogReqSend(TECH_BT, PROFILE_HF, FALSE,"Call hold supported string: %s\n", myPrim->chldString);
                        CsrPmemFree(myPrim->chldString);
                    }
                    CsrPmemFree(myPrim->serviceName);
                }
                else
                {
                    if (instData->currentConnection >= MAX_NUM_CONNECTION)
                    {
                        sprintf(buf, "Service level connection not possible for APP: max. number of connections achieved!\n");
                    }
                    else
                    {
                        sprintf(buf, "Service level connect failed, code: 0x%02X, supplier: 0x%02X\n", myPrim->resultCode, myPrim->resultSupplier);
                    }
                    sprintf(buf1, TEXT_FAILED_STRING);
                    instData->state = idle;
                }
                updateHfMainMenu(instData);
                if(*prim == CSR_BT_HF_SERVICE_CONNECT_IND)
                {
                    CsrBtHfShowUi(instData, CSR_BT_HF_MAIN_MENU_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HF_STD_PRIO);
                }
                break;
            }
        case CSR_BT_HF_STATUS_LOW_POWER_IND:
            {
                CsrBtHfStatusLowPowerInd *myPrim;
                myPrim = (CsrBtHfStatusLowPowerInd *) thePrim;

                instData->currentConnection = HfFindConIdxFromconId(instData,myPrim->connectionId);
                if (instData->currentConnection < MAX_NUM_CONNECTION)
                {
                    if (instData->state == idle)
                    {
                        sprintf(buf, "%s Link status: %d\n", getHfHsString(instData->conInstData[instData->currentConnection].linkType), myPrim->currentMode);
                    }
                    else
                    {
                        sprintf(buf, "CSR_BT_HF_STATUS_LOW_POWER_IND received");
                    }
                    if (myPrim->currentMode == CSR_BT_LINK_STATUS_DISCONNECTED)
                    {
                        instData->conInstData[instData->currentConnection].linkState = disconnected_s;
                    }
                }
                /* else ignore */
                break;
            }

        case CSR_BT_HF_SPEAKER_GAIN_IND:
            {
                CsrBtHfSpeakerGainInd *myPrim;

                myPrim = (CsrBtHfSpeakerGainInd *) thePrim;
                instData->currentConnection = HfFindConIdxFromconId(instData,myPrim->connectionId);
                sprintf(buf, "%s Speaker vol change to: %i\n", getHfHsString(instData->conInstData[instData->currentConnection].linkType), myPrim->gain);
                instData->conInstData[instData->currentConnection].speakerGain = myPrim->gain;
                break;
            }
        case CSR_BT_HF_MIC_GAIN_IND:
            {
                CsrBtHfMicGainInd *myPrim;

                myPrim = (CsrBtHfMicGainInd *) thePrim;

                instData->currentConnection = HfFindConIdxFromconId(instData,myPrim->connectionId);
                sprintf(buf, "%s Mic vol change to: %i\n", getHfHsString(instData->conInstData[instData->currentConnection].linkType), myPrim->gain);
                instData->conInstData[instData->currentConnection].micGain = myPrim->gain;
                break;
            }
        case CSR_BT_HF_INBAND_RING_SETTING_CHANGED_IND:
            {
                CsrBtHfInbandRingSettingChangedInd *myPrim;

                myPrim = (CsrBtHfInbandRingSettingChangedInd *) thePrim;
                instData->currentConnection = HfFindConIdxFromconId(instData,myPrim->connectionId);
                sprintf(buf, "%s Inband Ringing in HF changed to: %s\n",
                       getHfHsString(instData->conInstData[instData->currentConnection].linkType), (myPrim->inbandRingingActivated ? "ON" : "OFF"));
                instData->conInstData[instData->currentConnection].inbandRingingActivatedInHfg = myPrim->inbandRingingActivated;
                break;
            }
        case CSR_BT_HF_CALL_RINGING_IND:
            {
                CsrBtHfCallRingingInd *myPrim;

                myPrim = (CsrBtHfCallRingingInd *) thePrim;
                instData->currentConnection = HfFindConIdxFromconId(instData,myPrim->connectionId);
                CsrAppBacklogReqSend(TECH_BT, PROFILE_HF, FALSE, "Ring received using ");
                if ((instData->conInstData[instData->currentConnection].hfgSupportedFeatures & CSR_BT_HFG_SUPPORT_INBAND_RINGING) &&
                    (instData->conInstData[instData->currentConnection].inbandRingingActivatedInHfg) &&
                    (instData->conInstData[instData->currentConnection].inbandRingingActivatedInHf))
                {
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_HF, FALSE,  "Inband ringtone provided from HFG\n");
                }
                else
                {
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_HF, FALSE, "local alert signal\n");
                }
                return;
            }
        case CSR_BT_HF_STATUS_INDICATOR_UPDATE_IND:
            {
                CsrBtHfStatusIndicatorUpdateInd * prim;
                prim = (CsrBtHfStatusIndicatorUpdateInd *) thePrim;
                instData->currentConnection = HfFindConIdxFromconId(instData,prim->connectionId);

                if (CsrStrCmp((char *) prim->name, "battchg") == 0)
                {
                    sprintf(buf, "Battery charge indicator changed to: %d\n", prim->value);
                    CsrUiStatusBatterySetReqSend((CsrUint8)(prim->value*20));
                } 
                else if (CsrStrCmp((char *) prim->name, "signal") == 0)
                {
                    sprintf(buf, "Signal strength indicator changed to:%d\n", prim->value);
                    CsrUiStatusRadiometerSetReqSend((CsrUint8)(prim->value*20));
                }
                else if (CsrStrCmp((char *) prim->name, "roam") == 0)
                {
                    sprintf(buf, "\n\nRoaming status changed to: %s\n", (prim->value == CSR_BT_SERVICE_PRESENT_VALUE) ? "Roaming" : "Not Roaming");
                    CsrBtHfSetDialog(instData, CSR_BT_HF_DEFAULT_INFO_UI, CONVERT_TEXT_STRING_2_UCS2("Roaming status:"),
                                                    CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
                    CsrBtHfShowUi(instData, CSR_BT_HF_DEFAULT_INFO_UI, inputMode, CSR_BT_HF_STD_PRIO);
                }
                else if (CsrStrCmp((char *) prim->name, "service") == 0)
                {
                    sprintf(buf, "\n\nCellular registration is turned: %s\n", (prim->value == CSR_BT_SERVICE_PRESENT_VALUE) ? "on" : "off");
                    CsrBtHfSetDialog(instData, CSR_BT_HF_DEFAULT_INFO_UI, CONVERT_TEXT_STRING_2_UCS2("Cellular registration:"),
                                                    CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
                    CsrBtHfShowUi(instData, CSR_BT_HF_DEFAULT_INFO_UI, inputMode, CSR_BT_HF_STD_PRIO);
                }
                else
                {
                    if ( (CsrStrCmp((char *) prim->name, "call") == 0) &&
                         (prim->value == 1) &&
                         (instData->conInstData[instData->currentConnection].linkType == CSR_BT_HF_CONNECTION_HF) &&
                         (instData->conInstData[instData->currentConnection].startup != STARTUP_DONE) )
                    {  /* Call active indication and still establishing SLC: query answer and hold status */
                        CsrAppBacklogReqSend(TECH_BT, PROFILE_HF, FALSE, "Query current call status information ...\n");
                        CsrBtHfGetCurrentCallListReqSend(prim->connectionId);
                        CsrUiMenuRemoveallitemsReqSend(csrUiVar->displayesHandlers[CSR_BT_HF_CALL_LIST_UI].displayHandle);
                        instData->callIndex = 0;
                    }
                    sprintf(buf, "CSR_BT_HF_STATUS_INDICATOR_UPDATE_IND: %s = %d\n", prim->name, prim->value);
                }
                CsrPmemFree(prim->name);
                break;
            }

        case CSR_BT_HF_CALL_HANDLING_IND:
            {
                CsrBtHfCallHandlingInd *prim;
                prim = (CsrBtHfCallHandlingInd *) thePrim;

                instData->currentConnection = HfFindConIdxFromconId(instData,prim->connectionId);
                sprintf(buf, "\nResponse and hold status: %c\n", prim->event);
                switch(prim->event)
                {
                    case CSR_BT_BTRH_INCOMING_ON_HOLD:
                        {
                            CsrStrCat(buf, " - Incoming call on hold\n");
                            break;
                        }
                    case CSR_BT_BTRH_INCOMING_ACCEPTED:
                        {
                            CsrStrCat(buf,  " - Held incoming call accepted\n");
                            break;
                        }
                    case CSR_BT_BTRH_INCOMING_REJECTED:
                        {
                            CsrStrCat(buf, " - Held incoming call rejected\n");
                            break;
                        }
                    default:
                        {
                            CsrStrCat(buf,  "... error in btrh indication - unspecified value received\n");
                        }
                }
                break;
            }
        case CSR_BT_HF_CALL_NOTIFICATION_IND:
            {
                CsrBtHfCallNotificationInd *prim;

                prim = (CsrBtHfCallNotificationInd *) thePrim;
                instData->currentConnection = HfFindConIdxFromconId(instData,prim->connectionId);
                CsrAppBacklogReqSend(TECH_BT, PROFILE_HF, FALSE, "Call Notification Ind received in AG\n\n");
                CsrAppBacklogReqSend(TECH_BT, PROFILE_HF, FALSE, "CLIP data:  %s\n", prim->clipString);

                CsrPmemFree(prim->clipString);
                return;
             }
        case CSR_BT_HF_SET_VOICE_RECOGNITION_IND:
            {
                CsrBtHfSetVoiceRecognitionInd *myPrim;
                myPrim = (CsrBtHfSetVoiceRecognitionInd *)thePrim;
                instData->currentConnection = HfFindConIdxFromconId(instData,myPrim->connectionId);

                if (myPrim->started)
                {
                    sprintf(buf, "Voice Recognition started from HFG\n");
                }
                else
                {
                    sprintf(buf, "Voice Recognition stopped from HFG\n");
                }
                break;
            }
        case CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_IND:
            {
                CsrBtHfGetSubscriberNumberInformationInd *prim;
                CsrBool displaySubscriberInfo = FALSE;
                CsrUint8  *tempString;
                CsrUint16 lengthInd, subscriberInfoLen = 0;
                prim = (CsrBtHfGetSubscriberNumberInformationInd *) thePrim;
                
                

                if (prim->cnumString != NULL)
                {   
                    lengthInd  = (CsrUint16)(CsrStrLen((char*)prim->cnumString));
                    if (instData->subscriberInfo != NULL) 
                    {   
                        subscriberInfoLen     = (CsrUint16)(CsrStrLen((char*)instData->subscriberInfo)) ;
                        displaySubscriberInfo = TRUE;
                        lengthInd            += TEXT_HF_SUBSCRIBER_DETAILS_LEN;
                        tempString            = CsrPmemAlloc(subscriberInfoLen + lengthInd);
                        CsrStrNCpyZero((char*)tempString,(char*)instData->subscriberInfo,subscriberInfoLen);
                        CsrPmemFree(instData->subscriberInfo);
                        instData->subscriberInfo = NULL;

                        sprintf(buf, "\n%s%s",prim->cnumString,TEXT_HF_SUBSCRIBER_DETAILS);
                        subscriberInfoLen--; /* Decreased length to overwrite string terminator */

                    } else 
                    {   
                        lengthInd += TEXT_HF_SUBSCRIBER_INFO_LEN ;
                        tempString = CsrPmemAlloc(lengthInd);                
                        sprintf(buf, "%s%s",TEXT_HF_SUBSCRIBER_INFO,prim->cnumString);
                    }    
                    CsrPmemFree(prim->cnumString);
                }
                else
                {   
                    sprintf(buf, "No subscriber number information received \n");
                    lengthInd              = (CsrUint16)(CsrStrLen((char*)buf));
                    tempString             = CsrPmemAlloc(lengthInd);                
                    displaySubscriberInfo  = TRUE;
                }

                instData->subscriberInfo = tempString;
                tempString = tempString + subscriberInfoLen;
                CsrStrNCpyZero((char*)tempString,(char*)buf,lengthInd);

                if (displaySubscriberInfo) /* Subscriber info are displayed only when second indication is received */
                {
                    CsrBtHfSetDialog(instData, CSR_BT_HF_DEFAULT_INFO_UI, CONVERT_TEXT_STRING_2_UCS2("Subscriber info:"),
                                                    CONVERT_TEXT_STRING_2_UCS2(instData->subscriberInfo), TEXT_OK_UCS2, NULL);
                    CsrBtHfShowUi(instData, CSR_BT_HF_DEFAULT_INFO_UI, inputMode, CSR_BT_HF_STD_PRIO);
                    CsrPmemFree(instData->subscriberInfo);
                    instData->subscriberInfo = NULL;
                }
                break;
            }
        case CSR_BT_HF_SET_C2C_AUDIO_CODEC_IND:
            {
                HfSetC2CAudioCodecIndHandler(instData);
                return;
            }
        case CSR_BT_HF_GET_C2C_ADPCM_LOCAL_SUPPORTED_IND:
            {
                HfGetC2CAdpcmLocalSupportedIndHandler(instData);
                return;
            }
        case CSR_BT_HF_CALL_WAITING_NOTIFICATION_IND:
            {
                CsrBtHfCallWaitingNotificationInd *myPrim;
                myPrim = (CsrBtHfCallWaitingNotificationInd *)thePrim;

                sprintf(buf, "Call waiting received: %s\n", myPrim->ccwaString);

                CsrPmemFree(myPrim->ccwaString);
                break;
            }
        case CSR_BT_HF_AT_CMD_IND:
            {
                CsrBtHfAtCmdInd *myPrim;
                myPrim = (CsrBtHfAtCmdInd *) thePrim;

                instData->currentConnection = HfFindConIdxFromconId(instData,myPrim->connectionId);
                handleHfAtCmdInd(instData, myPrim);

                if(instData->conInstData[instData->currentConnection].startup == STARTUP_MIC)
                {
                    instData->conInstData[instData->currentConnection].startup = STARTUP_DONE;
                    CsrBtHfSpeakerGainStatusReqSend(instData->conInstData[instData->currentConnection].speakerGain,
                                         myPrim->connectionId);
                }

                CsrPmemFree(myPrim->atCmdString);
                return; /*nothing else to be done*/
            }
        case CSR_BT_HF_GET_CURRENT_CALL_LIST_IND:
            {
                CsrBtHfGetCurrentCallListInd *myPrim;
                CsrBtHfCsrUiType    * csrUiVar  = &(instData->csrUiVar);

                myPrim = (CsrBtHfGetCurrentCallListInd *) thePrim;
                instData->currentConnection = HfFindConIdxFromconId(instData,myPrim->connectionId);
                sprintf((char*)buf, "ConnectionId:%u\n\t Call info:%s",myPrim->connectionId,myPrim->clccString);

                CsrUiMenuAdditemReqSend(csrUiVar->displayesHandlers[CSR_BT_HF_CALL_LIST_UI].displayHandle,
                                CSR_UI_LAST, (CsrUint16)(myPrim->clccString[0]-'0'), CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2(buf),
                                NULL, csrUiVar->displayesHandlers[CSR_BT_HF_CALL_LIST_UI].sk1EventHandle,
                                CSR_UI_DEFAULTACTION, csrUiVar->displayesHandlers[CSR_BT_HF_CALL_LIST_UI].backEventHandle, CSR_UI_DEFAULTACTION);
                instData->callIndex++;
                CsrPmemFree(myPrim->clccString);
                break;
            }
        case CSR_BT_HF_C2C_SF_IND:
            {
                CsrBtHfC2cSfInd *myPrim = (CsrBtHfC2cSfInd *) thePrim;
                CsrUint8 i;
                HfHsCommonInstData_t *link;

                instData->currentConnection = HfFindConIdxFromconId(instData,myPrim->connectionId);
                link = &(instData->conInstData[instData->currentConnection]);
                sprintf(buf, "C2C exchange features received on connectionId 0x%4X: %s\n",myPrim->connectionId, (char*)(myPrim->indicators));
                if (myPrim->indicators[0] == '(')
                {/* This is an update on some particular feature: +CSR:; the format will be: (<index>,<value>)[,(<index>,<value>)] */
                    CsrUint8 indexFound = 0xFF;

                    for (i=0; i<myPrim->indicatorsLength;i++)
                    {
                        if (indexFound == 0xFF)
                        {
                            if ((myPrim->indicators[i] >= '0') && (myPrim->indicators[i] < (CSR_BT_C2C_NUMBER_OF_INDICATORS +'0')))
                            {/* Index found */
                                indexFound = myPrim->indicators[i] - '0';
                            }
                        }
                        else
                        {
                            if ((myPrim->indicators[i] >= '0') && (myPrim->indicators[i] < '9'))
                            {
                                link->hfgC2c[indexFound] = myPrim->indicators[i] - '0';
                                /* Make sure to get the next index first, if there are several updates in the same command */
                                indexFound = 0xFF;
                            }
                        }
                    }
                }
                else
                {/* This is the initial exchange: +CSRSF: ; the format will be: <value1>,<value2>,....*/
                    CsrUint8 j = 1; /* Remember: the index runs from 1- CSRMAX; index 0 is reserved in the profile... */

                    for (i=0; i<myPrim->indicatorsLength;i++)
                    {
                        if ((myPrim->indicators[i] >= '0') && (myPrim->indicators[i] < '9'))
                        {
                            link->hfgC2c[j] = myPrim->indicators[i] - '0';
                            j++;
                        }
                    }
                }

                CsrPmemFree(myPrim->indicators);
                break;
            }
        case CSR_BT_HF_C2C_BATT_IND:
            {/* The remote device asks about the battery status: send it */
                CsrBtHfC2cBattInd *myPrim = (CsrBtHfC2cBattInd *)thePrim;

                sprintf(buf, "Battery level requested\n");
                sendBatteryLevel(myPrim->connectionId, instData->batteryLevel);
                break;
            }
        case CSR_BT_HF_C2C_TXT_IND:
            {
                CsrCharString *buf2;
                CsrBtHfC2cTxtInd *myPrim = (CsrBtHfC2cTxtInd *)thePrim;

                buf2 = CsrPmemAlloc(sizeof(myPrim->txtString) + 75);
                sprintf(buf2, "CSR_BT_HF_C2C_TXT_IND received on connectionID: 0x%4X. \nText: %s",myPrim->connectionId,myPrim->txtString);

                CsrBtHfSetDialog(instData, CSR_BT_HF_DEFAULT_INFO_UI, CONVERT_TEXT_STRING_2_UCS2("Text Ind"),
                                                                        CONVERT_TEXT_STRING_2_UCS2(buf2), TEXT_OK_UCS2, NULL);
                /* Show the CSR_BT_HF_DEFAULT_INFO_UI on the display                   */
                CsrBtHfShowUi(instData, CSR_BT_HF_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HF_STD_PRIO);

                CsrPmemFree(myPrim->txtString);
                return;
            }
        case CSR_BT_HF_C2C_SMS_IND:
            {/* New SMS received: get text of SMS 'index' */
                CsrBtHfC2cSmsInd *myPrim = (CsrBtHfC2cSmsInd *)thePrim;
                sprintf(buf, "New SMS notification received. Retrieving SMS on connection Id: 0x%4x; sms index: %d\n",myPrim->connectionId,myPrim->index);
                CsrBtHfSetC2CGetSmsReqSend(myPrim->connectionId,myPrim->index);
                break;
            }
         /***************************************** CONFIRMATION MESSAGES **********************************/
        case CSR_BT_HF_CONFIG_AUDIO_CFM:
            {
                sprintf(buf, "CSR_BT_HF_CONFIG_AUDIO_CFM received\n");
                break;
            }
        case CSR_BT_HF_CONFIG_LOW_POWER_CFM:
            {
                sprintf(buf, "CSR_BT_HF_CONFIG_LOW_POWER_CFM received\n");
                break;
            }
        case CSR_BT_HF_ACTIVATE_CFM:
            {
                sprintf(buf, "Activated. result: 0x%02X, supplier: 0x%02X\n",((CsrBtHfActivateCfm *)thePrim)->resultCode,
                                                                       ((CsrBtHfActivateCfm *)thePrim)->resultSupplier);
                break;
            }

        case CSR_BT_HF_MIC_GAIN_STATUS_CFM:
            {
                CsrBtHfMicGainStatusCfm *myPrim;
                myPrim = (CsrBtHfMicGainStatusCfm *)thePrim;
                instData->currentConnection = HfFindConIdxFromconId(instData,myPrim->connectionId);
                if (myPrim->cmeeResultCode == CSR_BT_CME_SUCCESS)
                {
                    sprintf(buf, "Microphone gain operation completed successfully\n");
                }
                else
                {
                    sprintf(buf, "Microphone gain operation failed; result: 0x%x\n",myPrim->cmeeResultCode);
                }
                if(instData->conInstData[instData->currentConnection].startup == STARTUP_MIC)
                {
                    instData->conInstData[instData->currentConnection].startup = STARTUP_DONE;
                    CsrBtHfSpeakerGainStatusReqSend(instData->conInstData[instData->currentConnection].speakerGain,
                                         myPrim->connectionId);
                }
                break;
            }
        case CSR_BT_HF_SPEAKER_GAIN_STATUS_CFM:
            {
                CsrBtHfSpeakerGainStatusCfm *myPrim;
                myPrim = (CsrBtHfSpeakerGainStatusCfm *)thePrim;

                if (myPrim->cmeeResultCode == CSR_BT_CME_SUCCESS)
                {
                    sprintf(buf, "Speaker gain operation completed successfully\n");
                }
                else
                {
                    sprintf(buf, "Speaker gain operation failed; result: 0x%x\n",myPrim->cmeeResultCode);
                }
                break;
            }
        case CSR_BT_HF_SET_CALL_WAITING_NOTIFICATION_CFM:
            {
                CsrBtHfSetCallWaitingNotificationCfm *myPrim;
                myPrim = (CsrBtHfSetCallWaitingNotificationCfm *)thePrim;

                if (myPrim->cmeeResultCode == CSR_BT_CME_SUCCESS)
                {
                    sprintf(buf, "Set call waiting operation completed successfully\n");
                }
                else
                {
                    sprintf(buf, "Set call waiting operation failed; result: 0x%x\n",myPrim->cmeeResultCode);
                }
                break;
            }
        case CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_CFM:
            {
                CsrBtHfGetSubscriberNumberInformationCfm *myPrim;
                myPrim = (CsrBtHfGetSubscriberNumberInformationCfm *)thePrim;

                if (myPrim->cmeeResultCode == CSR_BT_CME_SUCCESS)
                {
                    sprintf(buf, "The CNUM command completed successfully\n");
                }
                else
                {
                    sprintf(buf, "The CNUM command failed; result: 0x%x\n",myPrim->cmeeResultCode);
                }
                break;
            }
        case CSR_BT_HF_SET_VOICE_RECOGNITION_CFM:
            {
                CsrBtHfSetVoiceRecognitionCfm *myPrim;
                myPrim = (CsrBtHfSetVoiceRecognitionCfm *)thePrim;

                if (myPrim->cmeeResultCode == CSR_BT_CME_SUCCESS)
                {
                    sprintf(buf, "The Voice Recognition command completed successfully\n");
                }
                else
                {
                    sprintf(buf, "The Voice Recognition command failed; result: 0x%x\n",myPrim->cmeeResultCode);
                }
                break;
            }
        case CSR_BT_HF_SET_ECHO_AND_NOISE_CFM:
            {
                CsrBtHfSetEchoAndNoiseCfm *myPrim;
                myPrim = (CsrBtHfSetEchoAndNoiseCfm *)thePrim;

                if (myPrim->cmeeResultCode == CSR_BT_CME_SUCCESS)
                {
                    sprintf(buf, "AT+NREC operation completed successfully\n");
                }
                else
                {
                    sprintf(buf, "AT+NREC operation failed; result: 0x%x\n",myPrim->cmeeResultCode);
                }
                break;
            }
        case CSR_BT_HF_GENERATE_DTMF_CFM:
            {
                CsrBtHfGenerateDtmfCfm *myPrim;
                myPrim = (CsrBtHfGenerateDtmfCfm *)thePrim;

                if (myPrim->cmeeResultCode == CSR_BT_CME_SUCCESS)
                {
                    sprintf(buf, "DTMF generation completed successfully\n");
                }
                else
                {
                    sprintf(buf, "The DTMF generation command failed; result: 0x%x\n",myPrim->cmeeResultCode);
                }
                break;
            }
        case CSR_BT_HF_GET_CURRENT_OPERATOR_SELECTION_CFM:
            {
                CsrBtHfGetCurrentOperatorSelectionCfm * myPrim;
                CsrBool displayOperatorName = FALSE;
                CsrUint16 lengthInd;
                myPrim = (CsrBtHfGetCurrentOperatorSelectionCfm *) thePrim;
                instData->state = idle; /* Make sure not to send COPS_QUERY again unless intended! */
                
                if(myPrim->cmeeResultCode == CSR_BT_CME_SUCCESS)
                {
                    if (myPrim->copsString != NULL)
                    {
                        sprintf(buf, "Network operator name:\n%s", myPrim->copsString);
                    }
                    else
                    {
                        sprintf(buf, "COPS query operation completed successfully\n");
                    }
                }
                else
                {
                    sprintf(buf, "Error retrieving network operator name\n");
                }


                if (instData->operatorName != NULL) 
                {   
                    displayOperatorName = TRUE;
                } 
                else 
                {   
                    lengthInd              = (CsrUint16)(CsrStrLen((char*)buf));
                    instData->operatorName = CsrPmemAlloc(lengthInd);
                    CsrStrNCpyZero((char*)instData->operatorName,(char*)buf,lengthInd);
                }
                if (displayOperatorName) /* Operator name is displayed only when second indication is received */
                {
                    CsrBtHfSetDialog(instData, CSR_BT_HF_DEFAULT_INFO_UI, CONVERT_TEXT_STRING_2_UCS2("Operator name:"),
                                                CONVERT_TEXT_STRING_2_UCS2(instData->operatorName), TEXT_OK_UCS2, NULL);
                    CsrBtHfShowUi(instData, CSR_BT_HF_DEFAULT_INFO_UI, inputMode, CSR_BT_HF_STD_PRIO);
                    CsrPmemFree(instData->operatorName);
                    instData->operatorName = NULL;
                }
                CsrPmemFree(myPrim->copsString);
                break;
            }
        case CSR_BT_HF_SET_EXTENDED_AG_ERROR_RESULT_CODE_CFM:
            {
                CsrBtHfSetExtendedAgErrorResultCodeCfm *myPrim;
                myPrim = (CsrBtHfSetExtendedAgErrorResultCodeCfm *)thePrim;

                if (myPrim->cmeeResultCode == CSR_BT_CME_SUCCESS)
                {
                    sprintf(buf, "The CMEE command completed successfully\n");
                }
                else
                {
                    sprintf(buf, "The CMEE command failed; result: 0x%x\n",myPrim->cmeeResultCode);
                }

                break;
            }
        case CSR_BT_HF_SET_CALL_NOTIFICATION_INDICATION_CFM:
            {
                CsrBtHfSetCallNotificationIndicationCfm *myPrim;
                myPrim = (CsrBtHfSetCallNotificationIndicationCfm *)thePrim;

                if (myPrim->cmeeResultCode == CSR_BT_CME_SUCCESS)
                {
                    sprintf(buf, "The CLIP command completed successfully\n");
                }
                else
                {
                    sprintf(buf, "The CLIP command failed; result: 0x%x\n",myPrim->cmeeResultCode);
                }
                break;
            }
        case CSR_BT_HF_BT_INPUT_CFM:
            {
                CsrBtHfBtInputCfm *myPrim;
                myPrim = (CsrBtHfBtInputCfm *)thePrim;
                if(myPrim->cmeeResultCode == CSR_BT_CME_SUCCESS)
                {
                    if (myPrim->dataRespString == NULL)
                    {
                            sprintf(buf, "BINP operation completed successfully, No phone Number\n");
                    }
                    else
                    {
                            sprintf(buf, "Phone number received: %s\n", myPrim->dataRespString);
                    }
                }
                else
                {
                    sprintf(buf, "Error during BINP operation: 0x%x\n",myPrim->cmeeResultCode);
                }

                CsrPmemFree(myPrim->dataRespString);
                break;
            }

        case CSR_BT_HF_DEACTIVATE_CFM:
            {
                CsrBtHfDeactivateCfm *myPrim = (CsrBtHfDeactivateCfm *)thePrim;

                CsrSchedTimerCancel(instData->deactGuard,NULL,NULL);

                if (myPrim->resultCode == CSR_BT_RESULT_CODE_HF_SUCCESS && myPrim->resultSupplier == CSR_BT_SUPPLIER_HF)
                {
                    initInstanceData(instData);
                    instData->serverActivated = FALSE;
                    sprintf(buf, "Deactivated Successfully!!");
                }
                else
                {
                    sprintf(buf, "Deactivation operation failed. Result: 0x%02X. Supplier: 0x%02X\n", myPrim->resultCode, myPrim->resultSupplier);
                }
                updateHfMainMenu(instData);
                CsrBtHfSetDialog(instData, CSR_BT_HF_DEFAULT_INFO_UI, CONVERT_TEXT_STRING_2_UCS2("Deactivate Complete"),
                                            CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
                CsrBtHfShowUi(instData, CSR_BT_HF_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HF_STD_PRIO);
                break;
            }
        case CSR_BT_HF_GET_CURRENT_CALL_LIST_CFM:
            {
                CsrBtHfGetCurrentCallListCfm *myPrim;
                myPrim = (CsrBtHfGetCurrentCallListCfm *) thePrim;

                if (myPrim->cmeeResultCode == CSR_BT_CME_SUCCESS)
                {
                    if(instData->callIndex)
                    {
                        /* Show the CSR_BT_HF_CALL_LIST_UI on the display                                  */
                        CsrBtHfShowUi(instData, CSR_BT_HF_CALL_LIST_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HF_STD_PRIO);
                        return;
                    }
                    else
                    {
                        sprintf(buf, "CLCC operation success!! No Active calls!!");
                    }
                }
                else
                {
                    sprintf(buf, "CLCC operation failed. Result code: 0x%x\n",myPrim->cmeeResultCode);
                }
                break;
            }
        case CSR_BT_HF_DIAL_CFM:
            {
                CsrBtHfDialCfm *myPrim;
                myPrim = (CsrBtHfDialCfm *) thePrim;

                if (myPrim->cmeeResultCode == CSR_BT_CME_SUCCESS)
                {
                    sprintf(buf, "The DIAL operation was handled successfully\n");
                }
                else
                {
                    sprintf(buf, "The DIAL command failed; result: 0x%x\n",myPrim->cmeeResultCode);
                }
                break;
            }
        case CSR_BT_HF_CALL_HANDLING_CFM:
            {
                CsrBtHfCallHandlingCfm *myPrim;
                CsrUieHandle displayHandle;

                myPrim = (CsrBtHfCallHandlingCfm *) thePrim;

                if (myPrim->cmeeResultCode == CSR_BT_CME_SUCCESS)
                {
                    sprintf(buf, "The call handling operation was handled successfully\n");
                    if(instData->selectedCall.index != 0xFF)
                    {
                        displayHandle = instData->csrUiVar.displayesHandlers[CSR_BT_HF_CALL_LIST_UI].displayHandle;

                        CsrUiMenuRemoveitemReqSend(displayHandle, instData->selectedCall.index);
                        instData->callIndex--;
                        if(!instData->callIndex)
                            CsrUiUieHideReqSend(displayHandle);
                    }
                }
                else
                {
                    sprintf(buf, "The call handling command failed; result: 0x%x\n",myPrim->cmeeResultCode);
                }
                instData->selectedCall.index = 0xFF; /* reset for case at+chld=2x*/
                break;
            }
        case CSR_BT_HF_CALL_ANSWER_CFM:
            {
                CsrBtHfCallAnswerCfm *myPrim;
                myPrim = (CsrBtHfCallAnswerCfm *) thePrim;

                if (myPrim->cmeeResultCode == CSR_BT_CME_SUCCESS)
                {
                    sprintf(buf, "The call answer operation was handled successfully\n");
                }
                else
                {
                    sprintf(buf, "The call answer command failed; result: 0x%x\n",myPrim->cmeeResultCode);
                }
                break;
            }
        case CSR_BT_HF_CALL_END_CFM:
            {
                CsrBtHfCallEndCfm *myPrim;
                myPrim = (CsrBtHfCallEndCfm *) thePrim;

                if (myPrim->cmeeResultCode == CSR_BT_CME_SUCCESS)
                {
                    sprintf(buf, "The call end operation was handled successfully\n");
                }
                else
                {
                    sprintf(buf, "The call end command failed; result: 0x%x\n",myPrim->cmeeResultCode);
                }
                break;
            }
        case CSR_BT_HF_AT_CMD_CFM:
            {
                CsrBtHfAtCmdCfm *myPrim = (CsrBtHfAtCmdCfm *)thePrim;
                if (myPrim->cmeeResultCode == CSR_BT_CME_SUCCESS)
                {
                    sprintf(buf, "The AT-cmd request on connection 0x%4X was handled successfully\n",myPrim->connectionId);
                }
                else
                {
                    sprintf(buf, "The AT command on connection 0x%4X failed; result: 0x%x\n",myPrim->connectionId,myPrim->cmeeResultCode);
                }
                break;
            }
        case CSR_BT_HF_GET_ALL_STATUS_INDICATORS_CFM:
            {
                CsrBtHfGetAllStatusIndicatorsCfm *myPrim = (CsrBtHfGetAllStatusIndicatorsCfm *)thePrim;
                if (myPrim->cmeeResultCode == CSR_BT_CME_SUCCESS)
                {
                    sprintf(buf, "The remote device's status indicators are: %s \n and have the values: %s\n",myPrim->indicatorSupported,myPrim->indicatorValue);
                    CsrPmemFree(myPrim->indicatorSupported);
                    CsrPmemFree(myPrim->indicatorValue);
                }
                else
                {
                    sprintf(buf, "The AT command on connection 0x%4X failed; result: 0x%x\n",myPrim->connectionId,myPrim->cmeeResultCode);
                }
                break;
            }
        case CSR_BT_HF_C2C_BATT_CFM:
            {
                CsrBtHfC2cBattCfm *myPrim = (CsrBtHfC2cBattCfm *)thePrim;
                sprintf(buf, "CSR_BT_HF_C2C_BATT_CFM received; result code: %d\n", myPrim->cmeeResultCode);
                break;
            }
        case CSR_BT_HF_C2C_PWR_CFM:
            {
                CsrBtHfC2cPwrCfm *myPrim = (CsrBtHfC2cPwrCfm *)thePrim;
                sprintf(buf, "CSR_BT_HF_C2C_PWR_CFM received; result code: %d\n", myPrim->cmeeResultCode);
                break;
            }
        case CSR_BT_HF_C2C_GET_SMS_CFM:
            {
                CsrBtHfC2cGetSmsCfm *myPrim = (CsrBtHfC2cGetSmsCfm *)thePrim;
                if ((myPrim->cmeeResultCode == CSR_BT_CME_SUCCESS) && (myPrim->smsString != NULL))
                {
                    sprintf(buf, "SMS retrieved: %s \n",myPrim->smsString);
                }
                else
                {
                    sprintf(buf, "CSR_BT_HF_C2C_GET_SMS_CFM received; result Code: %d \n",myPrim->cmeeResultCode);
                }

                CsrPmemFree(myPrim->smsString);
                break;
            }
        case CSR_BT_HF_INDICATOR_ACTIVATION_CFM:
            {
                CsrBtHfIndicatorActivationCfm *myPrim = (CsrBtHfIndicatorActivationCfm  *)thePrim;
                instData->currentConnection = HfFindConIdxFromconId(instData,myPrim->connectionId);
                if (myPrim->result == CSR_BT_CME_SUCCESS)
                {
                    sprintf(buf, "INDICATOR ACTIVATION operation successfull;\n \tConnection Id: %d \t Activation mask: 0x%4x\n",
                        instData->currentConnection, instData->conInstData[instData->currentConnection].tmpIndicatorActivation);
                    instData->conInstData[instData->currentConnection].indicatorActivation = instData->conInstData[instData->currentConnection].tmpIndicatorActivation;
                }
                else
                {
                    instData->conInstData[instData->currentConnection].tmpIndicatorActivation = instData->conInstData[instData->currentConnection].indicatorActivation;
                    sprintf(buf, "INDICATOR ACTIVATION operation FAILED;\n \tConnection Id: %d \t Activation mask: 0x%4x\n",
                        instData->currentConnection, instData->conInstData[instData->currentConnection].tmpIndicatorActivation);
                }
                break;
            }
        case CSR_BT_HF_UPDATE_SUPPORTED_CODEC_CFM:
            {
                CsrBtHfUpdateSupportedCodecCfm *myPrim = (CsrBtHfUpdateSupportedCodecCfm  *)thePrim;
                sprintf(buf, "CSR_BT_HF_UPDATE_SUPPORTED_CODEC_CFM received; result code: %d\n", myPrim->resultCode);
                break;
            }
        default:
            {
                sprintf(buf, "####### default in hf prim handler 0x%04x,\n", *primType);
                CsrBtHfFreeUpstreamMessageContents(*primType, thePrim);
                /*  unexpected primitive received   */
                break;
            }
    }

    CsrAppBacklogReqSend(TECH_BT, PROFILE_HF, FALSE, buf);
}

/**************************************************************************************************
 *
 *   a Cm event is received.
 *
 **************************************************************************************************/
void hfHandleCmPrim(DemoInstdata_t *instData)
{
    CsrBtCmPrim *primType = (CsrBtCmPrim *) instData->recvMsgP;

    if (*primType == CSR_BT_CM_WRITE_COD_CFM)
    {
        /* this cfm is received */;
    }
    CsrBtCmFreeUpstreamMessageContents(*primType, instData->recvMsgP);
}

void hfHandleCsrBccmdPrim(DemoInstdata_t *instData)
{
    CsrBccmdPrim *primType = (CsrBccmdPrim *) instData->recvMsgP;
    CsrCharString *buf;
#ifdef DEBUG
    CsrCharString *buf1;
#endif

    if (*primType == CSR_BCCMD_CFM)
    {
        CsrBccmdCfm * prim =  (CsrBccmdCfm *) instData->recvMsgP;

#ifdef DEBUG
        CsrUint8   x = 0;
        buf = CsrPmemAlloc(prim->payloadLength + 70);


        sprintf(buf, "CSR_BCCMD_CFM: Type:%i, payloadLength:%i, Seqnr:%i, Status:%i\n", prim->cmdType, prim->payloadLength, prim->seqNo, prim->status);

        buf1 = CsrPmemAlloc(prim->payloadLength + 1);
        CsrMemCpy(buf1, prim->payload, prim->payloadLength);
        buf1[prim->payloadLength] = '\0';
        CsrStrCat(buf, buf1);
#else
        buf = CsrPmemAlloc(25);

        sprintf(buf, "CSR_BCCMD_CFM received!!");
#endif
        CsrPmemFree(prim->payload);
    }
    else
    {
        buf = CsrPmemAlloc(75);

        sprintf(buf, "####### csr_bt_hf_demo_app.c: default in handleCsrBccmdPrim %x,\n", *primType);
        /*  unexpected primitive received   */
    }
    CsrBtHfSetDialog(instData, CSR_BT_HF_DEFAULT_INFO_UI, CONVERT_TEXT_STRING_2_UCS2("BCCMD Prim"),
                                    CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
    /* Show the CSR_BT_HF_DEFAULT_INFO_UI on the display                   */
    CsrBtHfShowUi(instData, CSR_BT_HF_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HF_STD_PRIO);

}
