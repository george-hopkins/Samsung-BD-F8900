/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #4 $
****************************************************************************/

#include "csr_synergy.h"

#include <stdio.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif

#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_arg_search.h"

#include "csr_bt_util.h"
#include "csr_bt_av_prim.h"
#include "csr_bt_app_prim.h"
#include "csr_bt_av_lib.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_sc_lib.h"
#include "csr_bt_sd_lib.h"

#include "csr_bt_platform.h"
#include "csr_bt_av2.h"
#include "csr_bt_av2_filter.h"
#include "csr_bt_sc_demo_app.h"
#include "csr_bccmd_prim.h"
#include "csr_bccmd_lib.h"
#include "csr_ui_lib.h"
#include "csr_app_lib.h"
#include "csr_exceptionhandler.h"
#include "csr_bt_ui_strings.h"
#include "csr_app_lib.h"
#include "csr_bt_av_app_util.h"
#include "csr_bt_av_app_ui_sef.h"
#include "csr_bt_gap_app_lib.h"
#include "csr_bt_gap_app_prim.h"
#ifdef CSR_USE_DSPM
#include "csr_dspm_lib.h"
#include "csr_bt_av2_dspm.h"
#endif

#define DEFAULT_AV_BAUDRATE  "921600"

#define DEFAULT_AV_BITRATE    256


CsrUint16 targetBitrate       = DEFAULT_AV_BITRATE;

/* Global instance needed for background interrupts */
av2instance_t *Av2Instance ;


const char *DaAvStateAvText[] = {"Disconnected", "Connecting", "Connected", "Connected - open", "Connected - streaming", "Disconnecting"};



static const CsrBtAvEventType csrUiMsgHandlers[CSR_UI_PRIM_UPSTREAM_COUNT] =
{ /* Jump tabel to handle Upstream CSR_UI messages send from the Phone Emulator UI (CSR_UI)     */
    CsrBtAvCsrUiUieCreateCfmHandler,                /* CSR_UI_UIE_CREATE_CFM                  */
    NULL,                                           /* CSR_UI_MENU_GET_CFM                    */
    NULL,                                           /* CSR_UI_MENU_GETITEM_CFM                */
    NULL,                                           /* CSR_UI_MENU_GETCURSOR_CFM              */
    NULL,                                           /* CSR_UI_EVENT_GET_CFM                   */
    NULL,                                           /* CSR_UI_DIALOG_GET_CFM                  */
    CsrBtAvCsrUiInputdialogGetCfmHandler,           /* CSR_UI_INPUTDIALOG_GET_CFM             */
    NULL,                                           /* CSR_UI_IDLESCREEN_GET_CFM              */
    NULL,                                           /* CSR_UI_DISPLAY_GETHANDLE_CFM           */
    NULL,                                           /* CSR_UI_KEYDOWN_IND                     */
    CsrBtAvCsrUiEventIndHandler,                    /* CSR_UI_EVENT_IND                       */
};

static void initAppCsrUiInitDisplayesHandlers(CsrBtAVCsrUiType *csrUiVar)
{
    CsrUint8 i;

    for (i = 0; i < NUMBER_OF_CSR_BT_AV_APP_UI; i++)
    {
        csrUiVar->displayesHandlers[i].displayHandle                 = CSR_UI_DEFAULTACTION;
        csrUiVar->displayesHandlers[i].sk1EventHandle                = CSR_UI_DEFAULTACTION;
        csrUiVar->displayesHandlers[i].sk2EventHandle                = CSR_UI_DEFAULTACTION;
        csrUiVar->displayesHandlers[i].backEventHandle               = CSR_UI_DEFAULTACTION;
        csrUiVar->displayesHandlers[i].deleteEventHandle             = CSR_UI_DEFAULTACTION;
        csrUiVar->displayesHandlers[i].sk1EventHandleFunc            = NULL;
        csrUiVar->displayesHandlers[i].sk2EventHandleFunc            = NULL;
        csrUiVar->displayesHandlers[i].deleteEventHandleFunc         = NULL;
        csrUiVar->displayesHandlers[i].backEventHandleFunc           = NULL;
    }
}

static void initAppCsrUiVar(CsrBtAVCsrUiType *csrUiVar)
{ /* Initialize the csrUiVar under inst                                                  */
    csrUiVar->eventState     = CSR_BT_AV_CREATE_SK1_EVENT;
    csrUiVar->uiIndex        = CSR_BT_AV_DEFAULT_INFO_UI;
    csrUiVar->popupTimerId   = 0;
    initAppCsrUiInitDisplayesHandlers(csrUiVar);
}

static void parseCommandline(av2instance_t *instData)
{
    CsrCharString *parameter, *value;

#ifdef USE_ALSA
    
    if ((CsrArgSearch(NULL, "--bt-av-alsa-device", &parameter, &value)) && (value != NULL))
    {
        instData->alsa_device_name = CsrStrDup(value);
    }
    else
    {
        instData->alsa_device_name = CsrStrDup(CSR_BT_AV_ALSA_DEFAULT_DEVICE);
    }
#endif
    
#ifdef USE_WAVE
    if ((CsrArgSearch(NULL, "--bt-av-wav-file-in", &parameter, &value)) && (value != NULL))
    {
        instData->wav_filename_in = CsrStrDup(value);
    }
    else
    {
        instData->wav_filename_in = CsrStrDup(CSR_BT_AV_WAV_DEFAULT_FILE_IN);
    }

    if ((CsrArgSearch(NULL, "--bt-av-wav-file-out", &parameter, &value)) && (value != NULL))
    {
        instData->wav_filename_out = CsrStrDup(value);
    }
    else
    {
        instData->wav_filename_out = CsrStrDup(CSR_BT_AV_WAV_DEFAULT_FILE_OUT);
    }
#endif
    
#ifdef USE_SBCFILE
    if ((CsrArgSearch(NULL, "--bt-av-sbc-file-in", &parameter, &value)) && (value != NULL))
    {
        instData->sbc_filename_in = CsrStrDup(value);
    }
    else
    {
        instData->sbc_filename_in = CsrStrDup(CSR_BT_AV_SBC_DEFAULT_FILE_IN);
    }

    if ((CsrArgSearch(NULL, "--bt-av-sbc-file-out", &parameter, &value)) && (value != NULL))
    {
        instData->sbc_filename_out = CsrStrDup(value);
    }
    else
    {
        instData->sbc_filename_out = CsrStrDup(CSR_BT_AV_SBC_DEFAULT_FILE_OUT);
    }
#endif
    
#ifdef USE_MP3
    if ((CsrArgSearch(NULL, "--bt-av-mp3-file", &parameter, &value)) && (value != NULL))
    {
        instData->mp3_filename_in = CsrStrDup(value);
    }
    else
    {
        instData->mp3_filename_in = CsrStrDup(CSR_BT_AV_MP3_DEFAULT_FILE_IN);
    }
#endif

#ifdef USE_AAC
    if ((CsrArgSearch(NULL, "--bt-av-aac-file", &parameter, &value)) && (value != NULL))
    {
        instData->aac_filename_in = CsrStrDup(value);
    }
    else
    {
        instData->aac_filename_in = CsrStrDup(CSR_BT_AV_AAC_DEFAULT_FILE_IN);
    }
#endif

    if ((CsrArgSearch(NULL, "--bt-av-dump-file", &parameter, &value)) && (value != NULL))
    {
        instData->dump_filename = CsrStrDup(value);
    }
    else
    {
        instData->dump_filename = CsrStrDup(CSR_BT_AV_DUMP_DEFAULT_FILE_OUT);
    }
}

/**************************************************************************************************
 * CsrBtAppInit
 **************************************************************************************************/
void CsrBtAvAppInit(void **gash)
{
    av2instance_t *instData;

                       #ifndef _WIN32_WCE
#ifdef _WIN32
    /* Windows is sloppy, so boost ourselves */
    SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
#endif
#endif
    /* Allocate and initialise instance data space  */
    *gash       = (void *) CsrPmemAlloc(sizeof(av2instance_t));
    instData    = (av2instance_t*)*gash;
    Av2Instance = instData;
    CsrMemSet(instData, 0, sizeof(av2instance_t));
    av2InitInstanceData(instData);
    initAppCsrUiVar(&instData->csrUiVar);
    filtersSetup(instData);

    parseCommandline(instData);
    
    /* Start creation cycle */
    CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_DIALOG);
    CsrAppRegisterReqSend(CsrSchedTaskQueueGet(), TECH_BT, PROFILE_AV);
#ifdef CSR_USE_DSPM
    {
        CsrUint8 i;
        /* Now register to the DSPM */
        CsrDspmActivateReqSend(instData->CsrSchedQid);
        instData->nextEncoderToUse   = CSR_DSPM_CAPABILITY_SBC_ENCODER;
        instData->nextEndPointType   = CSR_DSPM_CP_TYPE_CODEC;
        instData->av2DspmCallBack    = CsrAv2DspmActivateRspHandler;
        for (i=0; i<MAX_CONNECTIONS; i++)
        {
            CsrMemSet(&(instData->avCon[i].avDspmInst), 0 ,sizeof(AvDspmType));
            instData->avCon[i].avDspmInst.remoteCid       = CSR_BT_AV2_UNKNOWN_ID;
            instData->avCon[i].avDspmInst.aclHandle       = CSR_BT_AV2_UNKNOWN_ID;
            instData->avCon[i].avDspmInst.operatorEncoder = CSR_BT_AV2_UNKNOWN_ID;
            instData->avCon[i].avDspmInst.num_tokens      = CSR_BT_AV2_DSPM_DEFAULT_NUM_TOKENS;
            instData->avCon[i].avDspmInst.endPointType    = CSR_DSPM_CP_TYPE_CODEC;
            instData->avCon[i].avDspmInst.encoderToUse    = CSR_DSPM_CAPABILITY_SBC_ENCODER;
        }
    }
#endif
}

/**************************************************************************************************
 * CsrBtAppDeinit
 **************************************************************************************************/
#ifdef ENABLE_SHUTDOWN
void CsrBtAvAppDeinit(void **gash)
{
    CsrUint16 msg_type=0;
    void *msg_data=NULL;
    av2instance_t *instData;

    instData = (av2instance_t *) (*gash);

    while(CsrSchedMessageGet(&msg_type, &msg_data))
    {
        switch (msg_type)
        {
            case CSR_BT_AV_PRIM:
            {
                CsrBtAvFreeUpstreamMessageContents(msg_type, msg_data);
                break;
            }
            case CSR_BT_CM_PRIM:
            {
                CsrBtCmFreeUpstreamMessageContents(msg_type, msg_data);
                break;
            }
            case CSR_BCCMD_PRIM:
            {
                CsrBccmdFreeUpstreamMessageContents(msg_type, msg_data);
                break;
            }
            default:
            {
                break;
            }
        }
        CsrPmemFree(msg_data);
    }

#ifdef USE_ALSA
    CsrPmemFree(instData->alsa_device_name);
#endif
    
#ifdef USE_WAVE
    CsrPmemFree(instData->wav_filename_in);
    CsrPmemFree(instData->wav_filename_out);
#endif
    
#ifdef USE_SBCFILE
    CsrPmemFree(instData->sbc_filename_in);
    CsrPmemFree(instData->sbc_filename_out);
#endif
    
#ifdef USE_MP3
    CsrPmemFree(instData->mp3_filename_in);
#endif

#ifdef USE_AAC
    CsrPmemFree(instData->aac_filename_in);
#endif

    CsrPmemFree(instData->dump_filename);
    CsrPmemFree(instData);

    *gash = NULL;
    Av2Instance = NULL;
}
#else
void CsrBtAvAppDeinit(void **gash)
{
    CSR_UNUSED(gash);
}
#endif


/**************************************************************************************************
 * CsrBtAppHandler
 **************************************************************************************************/
void CsrBtAvAppHandler(void **gash)
{
    av2instance_t *instData;
    CsrUint16 eventType = 0;
    void *msg;

    /* Get a message from the demoapplication message queue. The
     * message is returned in prim and the event type in eventType
     */
    instData = (av2instance_t *) (*gash);
    CsrSchedMessageGet(&eventType, &instData->recvMsgP);

    msg = instData->recvMsgP;

    switch (eventType)
    {
        case CSR_BT_AV_PRIM:
        {
            handleAvPrim(instData, msg);
            break;
        }
        case CSR_BCCMD_PRIM:
        {
            CsrBccmdFreeUpstreamMessageContents(CSR_BCCMD_PRIM, msg);
            break;
        }
        case CSR_BT_CM_PRIM:
        {
            CsrPrim *type = instData->recvMsgP;
            switch(*type)
            {
                case CSR_BT_CM_WRITE_COD_CFM:
                {/* Just ignore */
                    break;
                }
                case CSR_BT_CM_SET_EVENT_MASK_CFM:
                {
#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_CHANNEL_TYPE
                    CsrBtCmSetEventMaskCfm *prim = (CsrBtCmSetEventMaskCfm *)msg;
                    CsrBtAvHideUi(instData, CSR_BT_AV_DEFAULT_INFO_UI);
                    if ((prim->eventMask & CSR_BT_CM_EVENT_MASK_SUBSCRIBE_CHANNEL_TYPE) == CSR_BT_CM_EVENT_MASK_SUBSCRIBE_CHANNEL_TYPE)
                    {
                       instData->registeredForEvent = TRUE;

                       CsrBtAvSetDialog(instData, CSR_BT_AV_DEFAULT_INFO_UI, NULL,
                              CONVERT_TEXT_STRING_2_UCS2("Event registration success") , TEXT_OK_UCS2, NULL);
                    }
                    else
                    {
                       instData->registeredForEvent = FALSE;
                       CsrBtAvSetDialog(instData, CSR_BT_AV_DEFAULT_INFO_UI, NULL,
                              CONVERT_TEXT_STRING_2_UCS2("Event deregistration success") , TEXT_OK_UCS2, NULL);

                    }
                    /* Show the CSR_BT_AV_DEFAULT_INFO_UI on the display                   */
                    CsrBtAvShowUi(instData, CSR_BT_AV_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AV_STD_PRIO);
#endif
                    break;
                }
                case CSR_BT_CM_LOGICAL_CHANNEL_TYPES_IND:
                {
                    CsrCharString temp[100];
                    CsrCharString displayString[500];
                    CsrBtCmLogicalChannelTypesInd *prim = (CsrBtCmLogicalChannelTypesInd *)msg;

                       #ifndef _WIN32_WCE
                    sprintf((char*)displayString, "CSR_BT_CM_LOGICAL_CHANNEL_TYPES_IND received\n");
                    sprintf((char*)temp, "logicalChannelTypeMask:            0x%x \n",prim->logicalChannelTypeMask);
                    CsrStrCat(displayString, temp);
                    sprintf((char*)temp, "numberOfGuaranteedLogicalChannels: 0x%x \n",prim->numberOfGuaranteedLogicalChannels);
                    CsrStrCat(displayString, temp);
#endif
                    CsrBtAvSetDialog(instData, CSR_BT_AV_EVENT_NOTFCN_1_DIALOG_UI, CONVERT_TEXT_STRING_2_UCS2("Event-Notification"),
                              CONVERT_TEXT_STRING_2_UCS2(displayString) , TEXT_OK_UCS2, NULL);
                    /* Show the CSR_BT_AV_DEFAULT_INFO_UI on the display                   */
                    CsrBtAvShowUi(instData, CSR_BT_AV_EVENT_NOTFCN_1_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AV_STD_PRIO);
                    break;
                }
                case CSR_BT_CM_ACL_CONNECT_IND:
                {
                    CsrCharString temp[100];
                    CsrCharString displayString[500];
                    CsrBtCmAclConnectInd *prim = (CsrBtCmAclConnectInd *)msg;

                       #ifndef _WIN32_WCE
                    sprintf((char*)displayString, "CSR_BT_CM_ACL_CONNECT_IND received\n");
                    sprintf((char*)temp, "deviceAddr:     %04x:%02x;%06x\n",prim->deviceAddr.nap, prim->deviceAddr.uap, prim->deviceAddr.lap);
                    CsrStrCat(displayString, temp);
                    sprintf((char*)temp, "Class-Of-Device: 0x%x\n",prim->cod);
                    CsrStrCat(displayString, temp);
                    sprintf((char*)temp, "Acl Conn Handle: 0x%x\n",prim->aclConnHandle);
                    CsrStrCat(displayString, temp);
                    sprintf((char*)temp, "incoming: %x\n",prim->incoming);
                    CsrStrCat(displayString, temp);
#endif
                    CsrBtAvSetDialog(instData, CSR_BT_AV_EVENT_NOTFCN_2_DIALOG_UI, CONVERT_TEXT_STRING_2_UCS2("Event-Notification"),
                              CONVERT_TEXT_STRING_2_UCS2(displayString) , TEXT_OK_UCS2, NULL);
                    /* Show the CSR_BT_AV_DEFAULT_INFO_UI on the display                   */
                    CsrBtAvShowUi(instData, CSR_BT_AV_EVENT_NOTFCN_2_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AV_STD_PRIO);
                    break;
                }
                case CSR_BT_CM_ACL_DISCONNECT_IND:
                {
                    CsrCharString temp[100];
                    CsrCharString displayString[500];
                    CsrBtCmAclDisconnectInd *prim = (CsrBtCmAclDisconnectInd *)msg;

                       #ifndef _WIN32_WCE
                    sprintf((char*)displayString, "CSR_BT_CM_ACL_DISCONNECT_IND received\n");
                    sprintf((char*)temp, "deviceAddr:     %04x:%02x;%06x\n",prim->deviceAddr.nap, prim->deviceAddr.uap, prim->deviceAddr.lap);
                    CsrStrCat(displayString, temp);
#endif
                    CsrBtAvSetDialog(instData, CSR_BT_AV_EVENT_NOTFCN_3_DIALOG_UI, CONVERT_TEXT_STRING_2_UCS2("Event-Notification"),
                              CONVERT_TEXT_STRING_2_UCS2(displayString) , TEXT_OK_UCS2, NULL);
                    /* Show the CSR_BT_AV_DEFAULT_INFO_UI on the display                   */
                    CsrBtAvShowUi(instData, CSR_BT_AV_EVENT_NOTFCN_3_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AV_STD_PRIO);
                    break;
                }
                case CSR_BT_CM_ROLE_DISCOVERY_CFM:
                {
                    CsrBtCmRoleDiscoveryCfm *ptr = (CsrBtCmRoleDiscoveryCfm *)instData->recvMsgP;
                    instData->avCon[instData->currentConnection].currentRole = ptr->role;
                    break;
                }
                case CSR_BT_CM_SWITCH_ROLE_CFM:
                {
                    CsrCharString displayString[200];
                    CsrBtCmSwitchRoleCfm *ptr = (CsrBtCmSwitchRoleCfm *)msg;

                       #ifndef _WIN32_WCE
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_AV, FALSE,"CSR_BT_CM_SWITCH_ROLE_CFM received with resultCode: 0x%x; role: %s\n",ptr->resultCode,GetRoleText(ptr->role));
                    sprintf((char*)displayString, "CSR_BT_CM_SWITCH_ROLE_CFM received with resultCode: 0x%x; role: %s\n", ptr->resultCode,GetRoleText(ptr->role));
#endif
                    if (ptr->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS)
                    {
                        instData->avCon[instData->currentConnection].currentRole = ptr->role;
                    }

                    CsrBtAvSetDialog(instData, CSR_BT_AV_DEFAULT_INFO_UI, NULL,
                              CONVERT_TEXT_STRING_2_UCS2(displayString) , TEXT_OK_UCS2, NULL);
                    /* Show the CSR_BT_AV_DEFAULT_INFO_UI on the display                   */
                    CsrBtAvShowUi(instData, CSR_BT_AV_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AV_STD_PRIO);
                    break;
                }
                default:
                {
                    printf("CSR_BT_CM_PRIM message arrived (0x%04X)\n", *(CsrUint16 *)msg);
                    CsrBtCmFreeUpstreamMessageContents(eventType, instData->recvMsgP);
                    break;
                }
            }
            break;
        }
        case CSR_APP_PRIM:
        {

            CsrPrim *type = instData->recvMsgP;

            if (*type == CSR_APP_TAKE_CONTROL_IND)
            {
                CsrAppBacklogReqSend(TECH_BT, PROFILE_AV, FALSE, "Received Take Control Ind in AV");
                CsrBtAvShowUi(instData, CSR_BT_AV_MAIN_MENU_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AV_STD_PRIO);
                CsrBtAvRegisterStreamHandleReqSend(instData->CsrSchedQid);
                CsrBtGapAppGetSelectedDeviceReqSend(instData->CsrSchedQid);
            }
            else if (*type == CSR_APP_REGISTER_CFM)
            { /* Just ignore this primitive since we only register one menu item we don't need to remember the controlId */

            }
            break;

        }
        case CSR_UI_PRIM:
        {
            /*CsrBtAvCsrUiUieCreateCfmHandler(instData);*/
            CsrUiPrim * primType = (CsrUiPrim *) instData->recvMsgP;
            CsrUint16   index    = (CsrUint16)(*primType - CSR_UI_PRIM_UPSTREAM_LOWEST);

            if (index < CSR_UI_PRIM_UPSTREAM_COUNT && csrUiMsgHandlers[index] != NULL)
            { /* Call the state event function which handle this incoming CSR_UI event message*/
                csrUiMsgHandlers[index](instData);
            }
            else
            { /* An unexpected CSR_UI message is received                                     */
                CsrGeneralException("CSR_APP", 0, *primType,
                                    "Did not handle CSR_UI Upstream message");
            }
            break;
        }
        
        case CSR_BT_GAP_APP_PRIM:
        {
            CsrPrim *prim = (CsrPrim *) instData->recvMsgP;
                
            if (*prim == CSR_BT_GAP_APP_GET_SELECTED_DEVICE_CFM)
            {
                CsrBtGapAppGetSelectedDeviceCfm *msg = (CsrBtGapAppGetSelectedDeviceCfm *) prim;

                instData->selectedDevice.lap = msg->deviceAddr.lap;
                instData->selectedDevice.nap = msg->deviceAddr.nap;
                instData->selectedDevice.uap = msg->deviceAddr.uap;
            }
            else
            {
                /* unexpected primitive received */
                CsrGeneralException("CSR_BT_AV_APP", 0, *prim, "####### Unhandled CSR_BT_GAP_APP_PRIM in CsrBtAvAppHandler");
            }
            break;
        }
#ifdef CSR_USE_DSPM
        case CSR_DSPM_PRIM:
        {
            if (instData->av2DspmCallBack != NULL)
            {/* Just call the call back function */
                instData->av2DspmCallBack(instData);
            }
            else
            {
                /* Do nothing... */
            }
            CsrDspmFreeUpstreamMessageContents(CSR_DSPM_PRIM, instData->recvMsgP);
            break;
        }
#endif
        default:
        {
            /* unexpected primitive received */
            CsrGeneralException("CSR_BT_AV_PRIM", 0, *((CsrPrim *) msg), "####### CsrBtAppHandler");
        }
    }

    /* Free the received signal again. If the signal for some reason
     * must be stored in the application the pointer (prim) must be
     * set to NULL in order not to free it here */
    CsrPmemFree(instData->recvMsgP);
}

void CsrBtAvAppUsage(void)
{
#ifdef USE_ALSA
    printf("--bt-av-alsa-device <device> - The ALSA device to use. Default = %s\n",CSR_BT_AV_ALSA_DEFAULT_DEVICE);
#endif
    
#ifdef USE_WAVE
    printf("--bt-av-wav-file-in <filename> - Name of the input WAV file to use. Default = %s\n",CSR_BT_AV_WAV_DEFAULT_FILE_IN);
    printf("--bt-av-wav-file-out <filename> - Name of the output WAV file to use. Default = %s\n",CSR_BT_AV_WAV_DEFAULT_FILE_OUT);
#endif
    
#ifdef USE_SBCFILE
    printf("--bt-av-sbc-file-in <filename> - Name of the input SBC file to use. Default = %s\n",CSR_BT_AV_SBC_DEFAULT_FILE_IN);
    printf("--bt-av-sbc-file-out <filename> - Name of the output SBC file to use. Default = %s\n", CSR_BT_AV_SBC_DEFAULT_FILE_OUT);
#endif
    
#ifdef USE_MP3
    printf("--bt-av-mp3-file <filename> - Name of the MP3 file to use. Default = %s\n",CSR_BT_AV_MP3_DEFAULT_FILE_IN);
#endif

#ifdef USE_AAC
    printf("--bt-av-aac-file <filename> - Name of the MP3 file to use. Default = %s\n",CSR_BT_AV_AAC_DEFAULT_FILE_IN);
#endif

    printf("--bt-av-dump-file <filename> - Name of the file to dump data to. Default = %s\n",CSR_BT_AV_DUMP_DEFAULT_FILE_OUT);
}



