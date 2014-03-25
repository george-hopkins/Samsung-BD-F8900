/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #3 $
****************************************************************************/

#include "csr_synergy.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_formatted_io.h"
#include "csr_bt_tasks.h"
#include "bluetooth.h"
#include "csr_bt_profiles.h"
#include "csr_bt_hfg_lib.h"
#include "csr_bt_hfg_prim.h"
#include "csr_bt_sd_lib.h"
#include "csr_bt_sc_prim.h"
#include "csr_bt_sc_lib.h"
#include "csr_bt_util.h"
#include "csr_hci_sco.h"
#include "csr_bt_platform.h"
#include "csr_bt_hfg_app_util.h"

#define DEBUG     1

/****************************************************************************************
*
*
*
*
*****************************************************************************************/
static void handleMptyCallStatus(hfgInstance_t *inst)
{
   CsrUint8 i;
   CsrUint8 numberOfActiveCalls = 0;


   for (i=0;i<MAX_CALLS;i++)
    {
     if ((inst->calls[i].isSet) && (inst->calls[i].status == CS_ACTIVE))
     {
         numberOfActiveCalls++;
     }
    }
    /* Now
       - if no active calls exist:          do nothing;
       - if there is only one active call:  make sure it is not marked as multiparty
       - if there are more than one active calls: marke them all as multiparty       */
    if (numberOfActiveCalls > 1)
    {
       for (i=0;((i<MAX_CALLS) && (numberOfActiveCalls != 0));i++)
        {
            if ((inst->calls[i].isSet) && (inst->calls[i].status == CS_ACTIVE))
            {
               numberOfActiveCalls--;
               inst->calls[i].mpty = MULTIPARTY_CALL;
            }
        }
    }
    else
    {
        if (numberOfActiveCalls == 1)
        {
         for (i=0;((i<MAX_CALLS) && (numberOfActiveCalls != 0));i++)
            {
                if ((inst->calls[i].isSet) && (inst->calls[i].status == CS_ACTIVE))
                {
                   numberOfActiveCalls--;
                   inst->calls[i].mpty = NOT_MULTIPARTY_CALL;
                }
            }
        }
    }
}



/* Handle SLC connection */
static void handleServiceConnectInd(hfgInstance_t *inst, CsrBtHfgServiceConnectInd *prim)
{
    Connection_t *con;
    CsrCharString buf[150], buf1[70];
    CsrBtHfgCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrBool fail = TRUE;

    con = getAddrConnection(inst, &(prim->deviceAddr));
    if(con == NULL)
    {
        con = getUnusedConnection(inst);
    }

#if 0
/*Dialog is already shown in Block mode. It has to be unblocked*/
    if(inst->dialogShow)
        CsrUiDisplaySetinputmodeReqSend(csrUiVar->displayesHandlers[CSR_BT_HFG_DEFAULT_INFO_UI].displayHandle, CSR_UI_INPUTMODE_AUTO);
#endif

    if(con == NULL)
    {
        sprintf(buf, "Could not find instance for new connection\n");
    }
    else
    {
        if(prim->resultCode == CSR_BT_RESULT_CODE_HFG_SUCCESS &&
           prim->resultSupplier == CSR_BT_SUPPLIER_HFG)
        {
            char   *name;
            CsrUint8 call;

            fail = FALSE;

            con->index                = prim->connectionId;
            con->connType             = prim->connectionType;
            con->hfSupportedFeatures  = prim->supportedFeatures;
            con->address              = prim->deviceAddr;
            con->remoteVersion        = prim->remoteVersion;
            con->active               = TRUE;
            con->inbandRinging        = (CsrBool)((inst->hfgSupportedFeatures & CSR_BT_HFG_SUPPORT_INBAND_RINGING) ? TRUE : FALSE);

            inst->current = getIndex(inst, con);

            name = "<n/a>";
            if(prim->serviceName != NULL)
            {
                name = prim->serviceName;
            }

            sprintf(buf, "Established '%s' type service level connection to '%s'\n",
                   (con->connType == CSR_BT_HFG_CONNECTION_HFG ? "HFP" : "old HS"),
                   name);

            if (con->connType == CSR_BT_HFG_CONNECTION_HFG)
            {
                CsrAppBacklogReqSend(TECH_BT, PROFILE_HFG, FALSE, "Remote HFP version supported: 0x%02x \n", con->remoteVersion);
                CsrAppBacklogReqSend(TECH_BT, PROFILE_HFG, FALSE,"Remote device %s codec negotiation.\n",
                    ((con->hfSupportedFeatures & CSR_BT_HF_SUPPORT_CODEC_NEGOTIATION) ? "SUPPORTS" : "DOES NOT SUPPORT") );
                

            }

            /* If we have an incoming call in the list */
            call = callGetStatus(inst, CS_INCOMING);
            if(call != NO_CALL)
            {
                /* Begin ringing... */
                CsrBtHfgStatusIndicatorSetReqSend(CSR_BT_HFG_CONNECTION_ALL,
                                             CSR_BT_CALL_SETUP_STATUS_INDICATOR,
                                             CSR_BT_INCOMING_CALL_SETUP_VALUE);
                CsrAppBacklogReqSend(TECH_BT, PROFILE_HFG, FALSE, "Incoming call in list, sent 'call-setup' indicator...\n");

                if(con->inbandRinging && !con->audioOn)
                {
                    CsrBtHfgAudioConnectReqSend(con->index,PCM_SLOT, PCM_SLOT_REALLOCATE);
                    sprintf(buf1, "Inband ringing is enabled, sent audio open request...\n");
                    CsrStrCat(buf, buf1);
                }

                CsrBtHfgRingReqSend(con->index,
                               RING_REP_RATE,
                               RING_NUM_OF_RINGS,
                               (CsrCharString *) StringDup(RING_NUMBER),
                               (CsrCharString *) StringDup((char*)inst->name),
                               NUMBER_TYPE_INT);
                CsrAppBacklogReqSend(TECH_BT, PROFILE_HFG, FALSE, "Sent ring request...\n");
            }
            else
            {
                call = callGetStatus(inst, CS_ACTIVE);
                if(call != NO_CALL)
                {
                    CsrBtHfgStatusIndicatorSetReqSend(prim->connectionId,
                                                CSR_BT_GATHERED_CALL_INDICATORS,
                                                CSR_BT_CALL_ACTIVE_NO_CALL_SETUP);
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_HFG, FALSE, "Active call indicators sent (no setup, active)...\n");

                    if(!con->audioOn && inst->autoAudioTransfer)
                    {
                        CsrBtHfgAudioConnectReqSend(con->index,PCM_SLOT, PCM_SLOT_REALLOCATE);
                        sprintf(buf1, "Call active, request to open audio...\n");
                        CsrStrCat(buf, buf1);
                    }
                }
            }
        }
        else
        {
            sprintf(buf, "Connection attempt failed, error code 0x%02X\n supplier 0x%02X", prim->resultCode, prim->resultSupplier);

            con->active = FALSE;
        }
        if(fail)
        {
            CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_HIGH_PRIO);

            CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_FAILED_UCS2,
                                CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
        }
        else
        {
            CsrBtHfgSetDialog(inst, CSR_BT_HFG_SUCC_CON_DIALOG_UI, TEXT_SUCCESS_UCS2,
                                            CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
            CsrBtHfgShowUi(inst, CSR_BT_HFG_SUCC_CON_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_HIGH_PRIO);
        }

        if(inst->dialogShow) /*Dialog is shown already becasue its a outgoing con. Hide it*/
        {
            CsrUiUieHideReqSend(csrUiVar->displayesHandlers[CSR_BT_HFG_CANCEL_CON_DIALOG_UI].displayHandle);
            inst->dialogShow = FALSE;
        }
        inst->state = stIdle;
        updateMainMenu(inst);
    }
}

/* Disconnect SLC */
static void handleDisconnectInd(hfgInstance_t *inst, CsrBtHfgDisconnectInd *prim)
{
    Connection_t *con;
    CsrBtDeviceAddr  addr;
    CsrUint8       i;
    CsrCharString buf[100];
    CsrCharString buf1[10] = TEXT_SUCCESS_STRING;
    CsrBtHfgCsrUiType    * csrUiVar  = &(inst->csrUiVar);

    if(inst->dialogShow)
        CsrUiDisplaySetinputmodeReqSend(csrUiVar->displayesHandlers[CSR_BT_HFG_DISC_DIALOG_UI].displayHandle, CSR_UI_INPUTMODE_AUTO);

    CsrSchedTimerCancel(inst->discGuard,NULL,NULL);

    con = getIdConnection(inst, prim->connectionId);
    if(con != NULL)
    {
        con->audioOn = FALSE;
        con->active  = FALSE;

        /* Clear connection instance, but remember address */
        addr.nap         = con->address.nap;
        addr.uap         = con->address.uap;
        addr.lap         = con->address.lap;
        clearConnection(inst, con);
        con->address.nap = addr.nap;
        con->address.uap = addr.uap;
        con->address.lap = addr.lap;
    }

    /* Select a new active instance */
    inst->current = NO_IDX;
    for(i=0; i<MAX_NUM_CONNECTION; i++)
    {
        if(inst->conInst[i].active)
        {
            inst->current = i;
            break;
        }
    }

    if(prim->reasonCode == RFC_ABNORMAL_DISCONNECT && prim->reasonSupplier == CSR_BT_SUPPLIER_RFCOMM)
    {
        sprintf(buf,"Disconnected due to link-loss\n");
        CsrStrCpy(buf1, TEXT_FAILED_STRING);
    }
    else if(!prim->localTerminated)

    {
        sprintf(buf, "Disconnected with error code 0x%02X supplier 0x%02X\n", prim->reasonCode, prim->reasonSupplier);
    }
    else
    {
        sprintf(buf, "Disconnected from local device (code 0x%02X supplier 0x%02X)\n", prim->reasonCode, prim->reasonSupplier);
    }
    CsrBtHfgSetDialog(inst, CSR_BT_HFG_DISC_DIALOG_UI, CONVERT_TEXT_STRING_2_UCS2(buf1),
                                                            CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);

    /* Show the CSR_BT_HFG_DISC_DIALOG_UI on the display                   */
    if(!inst->dialogShow) /*Remote device has disconnected the connection so Dialog UI has to be shown*/
        CsrBtHfgShowUi(inst, CSR_BT_HFG_DISC_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_HIGH_PRIO);

/*    CsrUiUieHideReqSend(csrUiVar->displayesHandlers[CSR_BT_HFG_DISC_DIALOG_UI].displayHandle);*/

}

/* Incoming SCO loopback */
static void handleIncomingSco(CsrUint8 *data)
{
    CsrHciSendScoData(data);
}

/* Audio connection */
static void handleAudioInd(hfgInstance_t *inst, CsrBtHfgAudioConnectInd *prim)
{
    Connection_t *con;
    CsrCharString buf[25];
    CsrBtHfgCsrUiType    * csrUiVar  = &(inst->csrUiVar);

    con = getIdConnection(inst, prim->connectionId);

    /* Always notify */
    sprintf((char *)buf, "Audio has been turned %s", (((prim->resultCode == CSR_BT_RESULT_CODE_HFG_SUCCESS) && (prim->resultSupplier == CSR_BT_SUPPLIER_HFG)) ? "on" : "off"));

    if(con == NULL)
    {
        sprintf(buf, "NO_CONNECTION\n");
    }
    else
    {
        /* Setup SCO-over-HCI */
        if((prim->resultCode == CSR_BT_RESULT_CODE_HFG_SUCCESS) && (prim->resultSupplier == CSR_BT_SUPPLIER_HFG))
        {
            CsrAppBacklogReqSend(TECH_BT, PROFILE_HFG, FALSE, " (gain %i)", con->speakerGain);
            CsrHciRegisterScoHandle(prim->scoHandle, (CsrHciScoHandlerFuncType)handleIncomingSco);
            CsrAppBacklogReqSend(TECH_BT, PROFILE_HFG, FALSE, " - using PCM slot %i\n", prim->pcmSlot);
        }

        /* Store settings */
        con->audioOn   = (((prim->resultCode == CSR_BT_RESULT_CODE_HFG_SUCCESS) && (prim->resultSupplier == CSR_BT_SUPPLIER_HFG)) ? TRUE : FALSE);
        con->scoHandle = prim->scoHandle;
    }

    CsrAppBacklogReqSend(TECH_BT, PROFILE_HFG, FALSE, "\nCodec used: %d\n",((CsrBtHfgAudioConnectInd*)prim)->codecUsed);
    CsrAppBacklogReqSend(TECH_BT, PROFILE_HFG, FALSE, "Sample rate: %d\n",((CsrBtHfgAudioConnectInd*)prim)->sampleRate);

    CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                            CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);

    if(!inst->dialogShow) /* If the dialog is already not shown*/
        CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
    else
        CsrUiDisplaySetinputmodeReqSend(csrUiVar->displayesHandlers[CSR_BT_HFG_DEFAULT_INFO_UI].displayHandle, CSR_UI_INPUTMODE_AUTO);

    inst->dialogShow = FALSE;
}

/* Audio settings indication */
static void handleStatusAudioInd(hfgInstance_t *inst, CsrBtHfgStatusAudioInd *prim)
{
    CsrCharString buf[600], buf1[25];

    switch(prim->audioType)
    {
        case CSR_BT_HFG_AUDIO_SCO_STATUS:
            {
                CsrBtHfgAudioScoStatus *audio;
                audio = (CsrBtHfgAudioScoStatus *)prim->audioSetting;

                CsrSnprintf(buf, 600, "SCO audio status indication for instance %u received:\n \
                                        linkType:       0x%02x\n \
                                        txInterval:     0x%02x\n \
                                        weSco:          0x%02x\n \
                                        rxPacketLength: 0x%04x\n \
                                        txPacketLength: 0x%04x\n \
                                        airMode:        0x%02x\n \
                                        resultCode:     0x%02x\n \
                                        resultSupplier: 0x%02x\n",
                                        prim->connectionId,
                                        audio->linkType,
                                        audio->txInterval,
                                        audio->weSco,
                                        audio->rxPacketLength,
                                        audio->txPacketLength,
                                        audio->airMode,
                                        audio->resultCode,
                                        audio->resultSupplier);
            }
            break;
        case CSR_BT_HFG_AUDIO_CODEC_STATUS:
            {
                /*Connection_t          *con;*/
                CsrBtHfgAudioCodecStatus *codec;

                codec = (CsrBtHfgAudioCodecStatus *)prim->audioSetting;
                CsrSnprintf(buf, 600, "Audio Codec Status Indication for instance %u received:\n \
                                        CVSD:           %s\n \
                                        ADPCM 2bit:     %s\n \
                                        ADPCM 4bit:     %s\n \
                                        Current Codec:    ",
                                        prim->connectionId,
                                        (codec->supportedMask & 0x01)?"supported":"unsupported",
                                        (codec->supportedMask & 0x02)?"supported":"unsupported",
                                        (codec->supportedMask & 0x08)?"supported":"unsupported");

                switch(codec->currentCodec)
                {
                    case CSR_BT_C2C_ADPCM_OFF_VALUE:
                        sprintf(buf1, "CVSD\n");
                        break;
                    case CSR_BT_C2C_ADPCM_2BIT_VALUE:
                        sprintf(buf1, "ADPCM 2bits/sample\n");
                        break;
                    case CSR_BT_C2C_ADPCM_4BIT_VALUE:
                        sprintf(buf1, "ADPCM 4bits/sample\n");
                        break;
                    default:
                        sprintf(buf1, "None\n");
                        break;
                }
                inst->supportedAudioCodecMask = codec->currentCodec;
                /*inst->c2cIndicators[C2C_ADPCM_IND] = codec->currentCodec;*/
                CsrStrCat(buf,buf1);
            }
            break;
        default:
            sprintf(buf, "Unknown audio status indication received, type 0x%02X\n",
                   prim->audioType);
            break;
    }
    CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);

    CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                    CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);

}

/* Answer call */
static void handleAnswerInd(hfgInstance_t *inst, CsrBtHfgAnswerInd *prim)
{
    Connection_t *con;
    CsrUint8 call;
    CsrCharString buf[200] = "";
    CsrCharString buf1[100] = "";

    CsrUint8 inputMode = CSR_UI_INPUTMODE_AUTO;

    con = getIdConnection(inst, prim->connectionId);
    if(con == NULL)
    {
        sprintf(buf, "Answer indication received for unknown connection\n");
        CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_FAILED_UCS2,
                                                        CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
        CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
        return;
    }

    CsrBtHfgStatusIndicatorSetReqSend(prim->connectionId,
                                CSR_BT_GATHERED_CALL_INDICATORS,
                                CSR_BT_CALL_ACTIVE_NO_CALL_SETUP);
    sprintf(buf,  "Answer indication received, indicators to activate call sent...\n");

    /* Set incoming as active */
    call = callChangeStatus(inst, CS_INCOMING, CS_ACTIVE);
    if(call != NO_CALL)
    {
        sprintf(buf1, "Incoming call is now active (index %i)\n", call);
        CsrStrCat(buf,buf1);
    }

    /* Setup audio */
    if(!con->audioOn)
    {
        CsrBtHfgAudioConnectReqSend(prim->connectionId,PCM_SLOT, PCM_SLOT_REALLOCATE);
        sprintf(buf1, "Incoming call is now active (index %i) Hfg Audio Connect Req is sent!Wait for response\n", call);
        inputMode = CSR_UI_INPUTMODE_BLOCK;
        inst->dialogShow = TRUE;
        CsrStrCat(buf,buf1);
    }
    CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                    CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
    CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, inputMode, CSR_BT_HFG_STD_PRIO);
}

/* Reject call */
static void handleRejectInd(hfgInstance_t *inst, CsrBtHfgRejectInd *prim)
{
    Connection_t *con;
    CsrUint8 call;
    CsrCharString buf[200] = "";
    CsrCharString buf1[100] = "";
    CsrUint8 inputMode = CSR_UI_INPUTMODE_AUTO;

    con = getIdConnection(inst, prim->connectionId);
    if(con == NULL)
    {
        sprintf(buf, "Reject received for unknown connection\n");
        CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_FAILED_UCS2,
                                                        CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
        CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);

        return;
    }

    
    
    call = callUnsetStatus(inst, CS_ALERTING);
    if(call != NO_CALL)
    {
        sprintf(buf, "Outgoing/alerting call rejected (index %i)\n", call);
    }
    else
    {
        call = callUnsetStatus(inst, CS_DIALING);
        if(call != NO_CALL)
        {
            sprintf(buf, "Outgoing/dialing call rejected (index %i)\n", call);
        }
    }

    
    /* Incoming/ongoing call being rejected */
    /* Release active calls */
    if ( callGetStatus(inst,CS_ACTIVE) != NO_CALL)
    {
        do
        {
            call = callUnsetStatus(inst, CS_ACTIVE);
            if(call != NO_CALL)
            {
                sprintf(buf, "Released ongoing call (index %i)\n", call);
            }
        }while(call != NO_CALL);
    }
    /* At least one active call has been removed */
    /* Send the proper indication to the HF */
    if ( callGetStatus(inst,CS_HELD) == NO_CALL)
    { /* No held calls found  and no active calls found at this stage either */
           CsrBtHfgStatusIndicatorSetReqSend(con->index,
                                   CSR_BT_CALL_STATUS_INDICATOR,
                                   CSR_BT_NO_CALL_ACTIVE_VALUE);
    }
    else
    { /* Held call(s) found */
           CsrBtHfgStatusIndicatorSetReqSend(con->index,
                                   CSR_BT_CALL_HELD_INDICATOR,
                                   CSR_BT_CALL_HELD_NO_ACTIVE_CALL_VALUE);
    }
    /* Now reject incoming calls if any */
    if ( callGetStatus(inst,CS_INCOMING) != NO_CALL)
    {
        do
        {
            call = callUnsetStatus(inst, CS_INCOMING);
            if(call != NO_CALL)
            {
                sprintf(buf, "Rejected incoming call (index %i)\n", call);
            }
        }while(call != NO_CALL);
    }
    /* Put waiting call as incoming (if it exist) */
    call = callChangeStatus(inst, CS_WAITING, CS_INCOMING);
    if(call != NO_CALL)
    {
        /* The call setup indicator is not changed, so do not send an indicator */
        sprintf(buf1, "Waiting call added and set as incoming (index %i)\n", call);
        CsrStrCat(buf, buf1);
    }
    else
    {/* At this point no incoming call exists.. */
        CsrBtHfgStatusIndicatorSetReqSend(con->index,
                                     CSR_BT_CALL_SETUP_STATUS_INDICATOR,
                                     CSR_BT_NO_CALL_SETUP_VALUE);
    }

    sprintf(buf1, "Outgoing call rejected, status indicators set for no-active, etc...\n");
    CsrStrCat(buf,buf1);

    /* are there any active calls left? Shouldn't be */
    call = callGetStatus(inst,CS_ACTIVE);
    /* Turn audio on or off */
    if(!con->audioOn &&
       (call != NO_CALL))
    {
        /* Audio was off, while active call existed; open it */
        CsrBtHfgAudioConnectReqSend(prim->connectionId,PCM_SLOT, PCM_SLOT_REALLOCATE);
        sprintf(buf1, "Audio requested on...\n");
        inputMode = CSR_UI_INPUTMODE_BLOCK;
    }
    else if((con->audioOn) &&
            (call == NO_CALL))
    {
        /* Audio was on, close it */
        CsrBtHfgAudioDisconnectReqSend(prim->connectionId);
        sprintf(buf1, "Audio requested off...\n");
        inputMode = CSR_UI_INPUTMODE_BLOCK;
    }
    if(inputMode == CSR_UI_INPUTMODE_BLOCK)
    {
        CsrStrCat(buf,buf1);
        inst->dialogShow = TRUE;
    }
    CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                    CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
    CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, inputMode, CSR_BT_HFG_STD_PRIO);

}

static void handleCallHandlingBtrhInd(hfgInstance_t *inst,
                                      Connection_t *con,
                                      CsrBtHfgCallHandlingInd *prim)
{
    CsrUint8 call;
    CsrCharString buf[100];
    CsrUint8 inputMode = CSR_UI_INPUTMODE_AUTO;
    CsrCharString heading[30] = TEXT_SUCCESS_STRING;

    switch(prim->value)
    {
        case CSR_BT_BTRH_PUT_ON_HOLD:
            {
                call = callChangeStatus(inst, CS_INCOMING, CS_HELD);
                if(call != NO_CALL)
                {
                    sprintf(buf, "BTRH: Put incoming call on hold (index %i)\n",
                           call);

                    /* Stop ringing */
                    CsrBtHfgRingReqSend(con->index,
                                   0,
                                   0,
                                   NULL,
                                   NULL,
                                   0);

                    /* Call is held (in network) */
                    CsrBtHfgCallHandlingResSend(prim->connectionId,
                                           CSR_BT_CME_SUCCESS,
                                           CSR_BT_HFG_BTRH_INCOMING_ON_HOLD);

                    /* But is actually active... */

                    CsrBtHfgStatusIndicatorSetReqSend(prim->connectionId,
                                                CSR_BT_GATHERED_CALL_INDICATORS,
                                                CSR_BT_CALL_ACTIVE_NO_CALL_SETUP);
                    /* If no other active calls exist, close audio if open */
                    if ((con->audioOn) && (callGetStatus(inst, CS_ACTIVE) == NO_CALL))
                    {
                        CsrBtHfgAudioDisconnectReqSend(con->index);
                        inputMode = CSR_UI_INPUTMODE_BLOCK;
                        CsrStrCpy(heading, TEXT_PLEASE_WAIT_STRING);
                    }

                }
                else
                {
                    sprintf(buf, "BTRH: Hold non-existing incoming call\n");
                    CsrBtHfgCallHandlingResSend(prim->connectionId,
                                           CSR_BT_CME_NO_NETWORK_SERVICE,
                                           CSR_BT_HFG_BTRH_IGNORE);
                }
                break;
            }

        case CSR_BT_BTRH_ACCEPT_INCOMING:
            {
                call = callChangeStatus(inst, CS_HELD, CS_ACTIVE);
                if(call != NO_CALL)
                {
                    sprintf(buf, "BTRH: Activate held call  (index %i)\n",
                           call);

                    CsrBtHfgCallHandlingResSend(prim->connectionId,
                                           CSR_BT_CME_SUCCESS,
                                           CSR_BT_HFG_BTRH_ACCEPT_INCOMING);
                    if(!con->audioOn)
                    {
                        CsrBtHfgAudioConnectReqSend(con->index,PCM_SLOT, PCM_SLOT_REALLOCATE);
                        inputMode = CSR_UI_INPUTMODE_BLOCK;
                        CsrStrCat(heading, TEXT_PLEASE_WAIT_STRING);
                        sprintf(buf, "Active calls now present, request audio on...\n");
                    }
                }
                else
                {
                    sprintf(buf, "BTRH: Accept non-existing held call\n");
                    CsrBtHfgCallHandlingResSend(prim->connectionId,
                                           CSR_BT_CME_NO_NETWORK_SERVICE,
                                           CSR_BT_HFG_BTRH_IGNORE);
                }
                break;
            }

        case CSR_BT_BTRH_REJECT_INCOMING:
            {
                call = callUnsetStatus(inst, CS_INCOMING);
                if (call == NO_CALL)
                {  /* There is no incoming alerting call: is there a held call? */
                    call = callUnsetStatus(inst, CS_HELD);
                }
                if(call != NO_CALL)
                {
                    sprintf(buf, "BTRH: Reject incoming call (index %i)\n", call);

                    /* Stop ringing */
                    CsrBtHfgRingReqSend(con->index,
                                   0,
                                   0,
                                   NULL,
                                   NULL,
                                   0);

                    CsrBtHfgCallHandlingResSend(prim->connectionId,
                                           CSR_BT_CME_SUCCESS,
                                           CSR_BT_HFG_BTRH_REJECT_INCOMING);
                    /* No calls are active anymore */
                    CsrBtHfgStatusIndicatorSetReqSend(prim->connectionId,
                                                 CSR_BT_CALL_STATUS_INDICATOR,
                                                 CSR_BT_NO_CALL_ACTIVE_VALUE);

                }
                else
                {
                    sprintf(buf, "BTRH: Reject non-existing incoming call\n");
                    CsrBtHfgCallHandlingResSend(prim->connectionId,
                                           CSR_BT_CME_NO_NETWORK_SERVICE,
                                           CSR_BT_HFG_BTRH_ACCEPT_INCOMING);
                }
                break;
            }

        case CSR_BT_BTRH_READ_STATUS:
            {
                /* If any calls are held, we must reply with a BTRH
                 * 'held' as well as ok.  If none on hold, just answer
                 * ok */
                call = callGetStatus(inst, CS_HELD);
                if(call != NO_CALL)
                {
                    sprintf(buf, "BTRH: Query status received - call is being held...\n");
                    CsrBtHfgCallHandlingResSend(prim->connectionId,
                                           CSR_BT_CME_SUCCESS,
                                           CSR_BT_HFG_BTRH_INCOMING_ON_HOLD);
                }
                else
                {
                    sprintf(buf, "BTRH: Query status received - no current calls on hold...\n");
                    CsrBtHfgCallHandlingResSend(prim->connectionId,
                                           CSR_BT_CME_SUCCESS,
                                           CSR_BT_HFG_BTRH_IGNORE);
                }
                break;
            }
    }
    CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CONVERT_TEXT_STRING_2_UCS2(heading),
                                                    CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
    CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, inputMode, CSR_BT_HFG_STD_PRIO);
}

/* Call handling (CHLD/BTRH) */
static void handleCallHandlingInd(hfgInstance_t *inst, CsrBtHfgCallHandlingInd *prim)
{
    CsrUint8       waitingBefore;
    CsrUint8       waitingAfter;
    CsrUint8       activeBefore;
    CsrUint8       activeAfter;
    CsrUint8       call;
    Connection_t *con;
    CsrCharString buf[400], buf1[75];

    con = getIdConnection(inst, prim->connectionId);
    if(con == NULL)
    {
        sprintf(buf, "Call handling command received for unknown connection\n");
    }

#ifdef DEBUG
    CsrAppBacklogReqSend(TECH_BT, PROFILE_HFG, FALSE, "Call handling command received. Opcode %i, index %i\n",
           prim->value, prim->index);
#endif

    waitingBefore = callGetStatus(inst, CS_WAITING);
    activeBefore  = callGetStatus(inst, CS_ACTIVE);

    switch(prim->value)
    {
        case CSR_BT_RELEASE_ALL_HELD_CALL:
            {
                /* Send call handling response and handle the command
                 * afterwards */

                CsrUint8 releasedNofHeldCalls = 0;

                sprintf(buf, "Call handling command: Release all held calls\n");
                CsrBtHfgCallHandlingResSend(prim->connectionId,
                                       CSR_BT_CME_SUCCESS,
                                       CSR_BT_HFG_BTRH_IGNORE);

                /* Release held calls */
                do
                {
                    call = callUnsetStatus(inst, CS_HELD);
                    if(call != NO_CALL)
                    {
                        sprintf(buf1, "Released held call (index %i)\n", call);
                        CsrStrCat(buf, buf1);
                        releasedNofHeldCalls++;
                    }
                }
                while(call != NO_CALL);


                if (releasedNofHeldCalls != 0)
                {/* Update status flags! There are no more held calls and some held call(s) has/have been released */
                    CsrBtHfgStatusIndicatorSetReqSend(CSR_BT_HFG_CONNECTION_ALL,
                                     CSR_BT_CALL_HELD_INDICATOR,
                                     CSR_BT_NO_CALL_HELD_VALUE);
                }

                /* Notify if no more active calls */
                if(callGetStatus(inst, CS_ACTIVE) == NO_CALL)
                {
                    CsrBtHfgStatusIndicatorSetReqSend(CSR_BT_HFG_CONNECTION_ALL,
                                                 CSR_BT_CALL_STATUS_INDICATOR,
                                                 CSR_BT_NO_CALL_ACTIVE_VALUE);
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_HFG, FALSE, "No more active/held calls - no-call-active indicator sent...\n");
                }

                /* AT+CHLD=0 sets the User Determined User Busy (UDUB) status for incoming and waiting calls; that is
                   waiting calls are rejected! Do it */
                do
                {
                    call = callUnsetStatus(inst, CS_WAITING);
                    if(call != NO_CALL)
                    {
                        sprintf(buf1, "Rejected waiting call (index %i)\n", call);
                        CsrStrCat(buf, buf1);
                    }
                }
                while(call != NO_CALL);
                /* Check multiparty status of calls */
                handleMptyCallStatus(inst);
                break;
            }

        case CSR_BT_RELEASE_ACTIVE_ACCEPT:
            {
                /* Send call handling response and handle the command
                 * afterwards */
                sprintf(buf, "Call handling command: Release active call, accept held, waiting or incoming.\n");
                CsrBtHfgCallHandlingResSend(prim->connectionId,
                                       CSR_BT_CME_SUCCESS,
                                       CSR_BT_HFG_BTRH_IGNORE);
                CsrAppBacklogReqSend(TECH_BT, PROFILE_HFG, FALSE, "Sent call handling response...\n");

                /* Release active calls */
                do
                {
                    call = callUnsetStatus(inst, CS_ACTIVE);
                    if(call != NO_CALL)
                    {
                        sprintf(buf1, "Released active call (index %i)\n", call);
                        CsrStrCat(buf, buf1);
                    }
                }
                while(call != NO_CALL);

                /* Put waiting as active, otherwise put held as active, otherwise put incoming as active */
                call = callChangeStatus(inst, CS_HELD, CS_ACTIVE);
                if(call != NO_CALL)
                {
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_HFG, FALSE, "Held call set as active (index %i)\n", call);

                    if (callGetStatus(inst, CS_HELD) == NO_CALL)
                    {/* No held calls left */
                     /* Send indication that there is an active call */
                       CsrBtHfgStatusIndicatorSetReqSend(CSR_BT_HFG_CONNECTION_ALL,
                                                    CSR_BT_CALL_STATUS_INDICATOR,
                                                    CSR_BT_CALL_ACTIVE_VALUE);

                       CsrBtHfgStatusIndicatorSetReqSend(CSR_BT_HFG_CONNECTION_ALL,
                                                    CSR_BT_CALL_HELD_INDICATOR,
                                                    CSR_BT_NO_CALL_HELD_VALUE);

                    }
                    else
                    {/* Still some held call(s) */
                        CsrBtHfgStatusIndicatorSetReqSend(CSR_BT_HFG_CONNECTION_ALL,
                                                    CSR_BT_CALL_HELD_INDICATOR,
                                                    CSR_BT_CALL_HELD_RETRIEVE_OTHER_CALL_VALUE);
                    }
                }
                else
                {
                    call = callChangeStatus(inst, CS_WAITING, CS_ACTIVE);
                    if(call != NO_CALL)
                    {
                        CsrAppBacklogReqSend(TECH_BT, PROFILE_HFG, FALSE, "Waiting call set as active (index %i)\n", call);
                    }
                    else
                    {
                        call = callChangeStatus(inst, CS_INCOMING, CS_ACTIVE);
                        if(call != NO_CALL)
                        {
                            /* This is not totally in complience with the specification (3GPP 22.030),
                               but is implemented to be able to interopt with specific headsets,
                               which send down CHLD=1 even though no held or waiting calls are present,
                               only an incoming call */
                            CsrAppBacklogReqSend(TECH_BT, PROFILE_HFG, FALSE, "Incoming call set as active (index %i)\n", call);
                            CsrBtHfgStatusIndicatorSetReqSend(CSR_BT_HFG_CONNECTION_ALL,
                                                 CSR_BT_CALL_SETUP_STATUS_INDICATOR,
                                                 CSR_BT_NO_CALL_SETUP_VALUE);
                        }
                    }
                }

                /* Notify if call(s) are still incoming (or waiting) */
                if ((callGetStatus(inst,CS_WAITING) == NO_CALL) && (callGetStatus(inst,CS_INCOMING) == NO_CALL))
                    {
                        CsrBtHfgStatusIndicatorSetReqSend(CSR_BT_HFG_CONNECTION_ALL,
                                                 CSR_BT_CALL_SETUP_STATUS_INDICATOR,
                                                 CSR_BT_NO_CALL_SETUP_VALUE);
                    }
                /* Notify if call(s) are still active */
                if(callGetStatus(inst, CS_ACTIVE) != NO_CALL)
                {
                    /* New calls active */
                    CsrBtHfgStatusIndicatorSetReqSend(CSR_BT_HFG_CONNECTION_ALL,
                                                 CSR_BT_CALL_STATUS_INDICATOR,
                                                 CSR_BT_CALL_ACTIVE_VALUE);
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_HFG, FALSE, "Active call present - sent active-call indicator...\n");
                }
                /* Check multiparty status of calls */
                handleMptyCallStatus(inst);
                break;
            }

        case CSR_BT_RELEASE_SPECIFIED_CALL:
            {
                /* Send call handling response and handle the command
                 * afterwards */
                CsrAppBacklogReqSend(TECH_BT, PROFILE_HFG, FALSE, "Call handling command: Release specified call (%i)\n", prim->index);
                CsrBtHfgCallHandlingResSend(prim->connectionId,
                                       CSR_BT_CME_SUCCESS,
                                       CSR_BT_HFG_BTRH_IGNORE);

                /* Find specific call */
                if((prim->index < MAX_CALLS) && (prim->index > 0) &&
                   (inst->calls[prim->index-1].isSet))
                {
                    inst->calls[prim->index-1].isSet = FALSE;
                    sprintf(buf, "Released specific call (index %i)\n", prim->index);
                }

                /* Notify if no more active calls */
                if (callGetStatus(inst, CS_HELD) == NO_CALL)
                {/* No held calls left */
                   if ( callGetStatus(inst, CS_ACTIVE) == NO_CALL)
                        { /* No active calls */
                            CsrAppBacklogReqSend(TECH_BT, PROFILE_HFG, FALSE, "No held or active calls exist - send proper active-calls indicator...\n");
                            CsrBtHfgStatusIndicatorSetReqSend(con->index,
                                                         CSR_BT_CALL_STATUS_INDICATOR,
                                                         CSR_BT_NO_CALL_ACTIVE_VALUE);
                        }

                }
                else
                {
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_HFG, FALSE, "Held calls exist - send proper held-calls indicator...\n");
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

                /* Check multiparty status of calls */
                handleMptyCallStatus(inst);
                break;
            }

        case CSR_BT_HOLD_ACTIVE_ACCEPT:
            {
                CsrUint8 pickup;

                /* Send call handling response and handle the command
                 * afterwards */
                sprintf(buf, "Call handling command: Hold active, accept held\n");

                /* Find currently held call */
                pickup = callGetStatus(inst, CS_HELD);
                if (NO_CALL == pickup)
                {
                    pickup = callGetStatus(inst, CS_WAITING);
                    if (NO_CALL == pickup)
                    {
                        pickup = callGetStatus(inst, CS_INCOMING);
                    }
                }

                if (pickup != NO_CALL)
                {
                    CsrBtHfgCallHandlingResSend(prim->connectionId,
                                           CSR_BT_CME_SUCCESS,
                                           CSR_BT_HFG_BTRH_IGNORE);
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_HFG, FALSE, "Sent call handling response...\n");
                    /* Put active calls on hold*/
                    do
                    {
                        call = callChangeStatus(inst, CS_ACTIVE, CS_HELD);
                        if(call != NO_CALL)
                        {
                            sprintf(buf1, "Active call put on hold (index %i)\n", call);
                            CsrBtHfgStatusIndicatorSetReqSend(CSR_BT_HFG_CONNECTION_ALL,
                                                         CSR_BT_CALL_HELD_INDICATOR,
                                                         CSR_BT_CALL_HELD_RETRIEVE_OTHER_CALL_VALUE);
                            CsrStrCat(buf, buf1);
                        }
                    }
                    while(call != NO_CALL);

                    /* Put call on active */
                    inst->calls[pickup-1].status = CS_ACTIVE;
                    sprintf(buf1, "Call (index %i) is now active.\n", pickup-1);
                    CsrStrCat(buf, buf1);
                }
                else
                {
                    CsrBtHfgCallHandlingResSend(prim->connectionId,
                                           CSR_BT_CME_UNKNOWN,
                                           CSR_BT_HFG_BTRH_IGNORE);
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_HFG, FALSE, "Sent call handling response...\n");
                }
                /* Check multiparty status of calls */
                handleMptyCallStatus(inst);
                break;
            }
        case CSR_BT_REQUEST_PRIVATE_WITH_SPECIFIED:
            {
                CsrUint8 i;

                /* Send call handling response and handle the command
                 * afterwards */
                sprintf(buf, "Call handling command: Private consultation mode with call (%i)\n",
                       prim->index);
                CsrBtHfgCallHandlingResSend(prim->connectionId,
                                       CSR_BT_CME_SUCCESS,
                                       CSR_BT_HFG_BTRH_IGNORE);

                /* Put all calls on hold, except specific which is to be active */
                for(i=0; i<MAX_CALLS; i++)
                {
                    if(inst->calls[i].isSet)
                    {
                        if((i) != prim->index - 1)
                        {
                            inst->calls[i].status = CS_HELD;
                            sprintf(buf1, "Private consultation, put call on hold (index %i)\n", i);
                            CsrStrCat(buf, buf1);
                            /* and send held indication... */
                            CsrBtHfgStatusIndicatorSetReqSend(CSR_BT_HFG_CONNECTION_ALL,
                                                         CSR_BT_CALL_HELD_INDICATOR,
                                                         CSR_BT_CALL_HELD_RETRIEVE_OTHER_CALL_VALUE);
                        }
                    }
                }

                /* Put specific call on active */
                if((prim->index < MAX_CALLS) && (prim->index > 0) &&
                   (inst->calls[prim->index-1].isSet))
                {
                    inst->calls[prim->index-1].status = CS_ACTIVE;
                    sprintf(buf1, "Private consultation, set call as active (index %i)\n",
                           prim->index);
                    CsrStrCat(buf, buf1);
                }
                /* Check multiparty status of calls */
                handleMptyCallStatus(inst);
                break;
            }

        case CSR_BT_ADD_CALL:
            {
                /* Send call handling response and handle the command
                 * afterwards */
                sprintf(buf, "Call handling command: Add call\n");
                CsrBtHfgCallHandlingResSend(prim->connectionId,
                                       CSR_BT_CME_SUCCESS,
                                       CSR_BT_HFG_BTRH_IGNORE);

                /* Put held on active */
                call = callChangeStatus(inst, CS_HELD, CS_ACTIVE);
                if(call != NO_CALL)
                {
                    sprintf(buf1, "Held call added and set as active (index %i)\n", call);
                    CsrStrCat(buf, buf1);
                    CsrBtHfgStatusIndicatorSetReqSend(CSR_BT_HFG_CONNECTION_ALL,
                                                 CSR_BT_CALL_SETUP_STATUS_INDICATOR,
                                                 CSR_BT_NO_CALL_SETUP_VALUE);
                    sprintf(buf1, "No calls being setup, sent no-call-setup indicator...\n");
                    CsrStrCat(buf, buf1);
                    if (callGetStatus(inst, CS_HELD) == NO_CALL)
                    {
                        CsrBtHfgStatusIndicatorSetReqSend(CSR_BT_HFG_CONNECTION_ALL,
                                                     CSR_BT_CALL_HELD_INDICATOR,
                                                     CSR_BT_NO_CALL_SETUP_VALUE);
                        sprintf(buf1,"No calls held, sent held-calls indicator...\n");
                        CsrStrCat(buf, buf1);
                    }
                }
                /* Check multiparty status of calls */
                handleMptyCallStatus(inst);
                break;
            }

        case CSR_BT_CONNECT_TWO_CALLS:
            {
                /* Send call handling response and handle the command
                 * afterwards */
                sprintf(buf, "Call handling command: Explicit call transfer\n");
                if((callGetStatus(inst, CS_ACTIVE) != NO_CALL) && (callGetStatus(inst, CS_HELD) != NO_CALL))
                {
                    CsrBtHfgCallHandlingResSend(prim->connectionId,
                                           CSR_BT_CME_SUCCESS,
                                           CSR_BT_HFG_BTRH_IGNORE);

                    /*  Explicit call transfer */
                    call = callUnsetStatus(inst, CS_ACTIVE);
                    if(call != NO_CALL)
                    {
                        sprintf(buf1, "Explicit call transfer, dropped active call (index %i)\n", call);
                        CsrStrCat(buf, buf1);
                    }
                    call = callUnsetStatus(inst, CS_HELD);
                    if(call != NO_CALL)
                    {
                        sprintf(buf1,"Explicit call transfer, dropped held call (index %i)\n", call);
                        CsrStrCat(buf, buf1);
                    }
                    CsrBtHfgStatusIndicatorSetReqSend(CSR_BT_HFG_CONNECTION_ALL,
                                                 CSR_BT_CALL_SETUP_STATUS_INDICATOR,
                                                 CSR_BT_NO_CALL_SETUP_VALUE);
                    CsrBtHfgStatusIndicatorSetReqSend(CSR_BT_HFG_CONNECTION_ALL,
                                                 CSR_BT_CALL_HELD_INDICATOR,
                                                 CSR_BT_NO_CALL_HELD_VALUE);

                    /* Put waiting call as active (if it exist) */
                    call = callChangeStatus(inst, CS_WAITING, CS_ACTIVE);
                    if(call != NO_CALL)
                    {
                        sprintf(buf1, "Waiting call added and set as active (index %i)\n", call);
                        CsrStrCat(buf, buf1);
                        CsrBtHfgStatusIndicatorSetReqSend(CSR_BT_HFG_CONNECTION_ALL,
                                                     CSR_BT_CALL_STATUS_INDICATOR,
                                                     CSR_BT_CALL_ACTIVE_VALUE);
                    }

                    if (callGetStatus(inst, CS_ACTIVE) == NO_CALL)
                    {/* No active calls left */
                        CsrBtHfgStatusIndicatorSetReqSend(CSR_BT_HFG_CONNECTION_ALL,
                                                     CSR_BT_CALL_STATUS_INDICATOR,
                                                     CSR_BT_NO_CALL_ACTIVE_VALUE);
                    }

                }
                else
                {
                    /* there is NOT an active call AND a held call, which can be connected */
                    CsrBtHfgCallHandlingResSend(prim->connectionId,
                                           CSR_BT_CME_UNKNOWN,
                                           CSR_BT_HFG_BTRH_IGNORE);
                    sprintf(buf1, "Sent call handling response (ERROR, no two calls exist to connect).\n");
                    CsrStrCat(buf, buf1);
                }
                break;
            }

        case CSR_BT_BTRH_PUT_ON_HOLD:
            /* Fall through */
        case CSR_BT_BTRH_ACCEPT_INCOMING:
            /* Fall through */
        case CSR_BT_BTRH_REJECT_INCOMING:
            /* Fall through */
        case CSR_BT_BTRH_READ_STATUS:
            {
                /* Handle BTRHs in special function */
                handleCallHandlingBtrhInd(inst, con, prim);
                /* Check multiparty status of calls */
                handleMptyCallStatus(inst);
                /* Break out completely! */
                return;
            }

        default:
            {
                sprintf(buf, "Unknown call handling command received. Opcode %i, index %i\n",
                       prim->value, prim->index);
                CsrBtHfgCallHandlingResSend(prim->connectionId,
                                       CSR_BT_CME_NO_NETWORK_SERVICE,
                                       CSR_BT_HFG_BTRH_IGNORE);

                /* Break out completely */
                return;
            }
    }

    waitingAfter = callGetStatus(inst, CS_WAITING);
    activeAfter = callGetStatus(inst, CS_ACTIVE);

    if((waitingBefore != NO_CALL) &&
       (waitingAfter == NO_CALL))
    {
        /* We went from waiting calls to no waiting calls */
        CsrBtHfgStatusIndicatorSetReqSend(CSR_BT_HFG_CONNECTION_ALL,
                                     CSR_BT_CALL_SETUP_STATUS_INDICATOR,
                                     CSR_BT_NO_CALL_SETUP_VALUE);
    }

    if((activeBefore != NO_CALL) &&
       (activeAfter == NO_CALL) &&
       (con->audioOn))
    {
        /* We went from active call(s) to no active call(s),
         * so turn audio off */
        CsrBtHfgAudioDisconnectReqSend(con->index);
        sprintf(buf1, "No more active calls, audio requested off...\n");
        CsrStrCat(buf, buf1);
    }

    if((activeAfter != NO_CALL) &&
       (activeBefore == NO_CALL) &&
       !(con->audioOn))
    {
        /* We went from no active calls to active calls, so
         * request audio */
        CsrBtHfgAudioConnectReqSend(con->index,PCM_SLOT, PCM_SLOT_REALLOCATE);
        sprintf(buf1, "Active calls now present, request audio on...\n");
        CsrStrCat(buf, buf1);
    }
    CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                        CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
    CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);

}

/* Dial indication */
static void handleDialInd(hfgInstance_t *inst, CsrBtHfgDialInd *prim)
{
    char *num;
    CsrUint8 call,numLen;
    Connection_t *con;
    CsrCharString buf[400] = "";
    CsrCharString buf1[100] = "";

    con = getIdConnection(inst, prim->connectionId);
    if(con == NULL)
    {
        sprintf(buf, "Dial command received for unknown connection\n");
        CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_FAILED_UCS2,
                                                            CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
        CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
        return;
    }

    /* Number as string */
    if(prim->number != NULL)
    {
        num = (char *) prim->number;
        /* update last dialed number storage */
        CsrStrCpy((char*)(inst->redialNumber), num);
        inst->redialNumber[strlen(num)-1] = 0;
        sprintf(buf, "Redial list updated\n");
    }
    else
    {
        if ( prim->command == CSR_BT_HFG_DIAL_REDIAL )
        {/* Redial command received */
           if ( CsrStrLen((char*)(inst->redialNumber)) == 0 )
           {/* But no last dialed number registered */
               CsrBtHfgDialResSend(con->index,CSR_BT_CME_NO_NETWORK_SERVICE);
               num = "<n/a>";
            sprintf(buf, "CSR_BT_HFG_DIAL_REDIAL command is receievd and redialNumber is NULL!!\n");
            CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_FAILED_UCS2,
                                                                CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
            CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
            return;
           }
           else
           {/* Use redial number */
                num =(char*)&(inst->redialNumber);
           }
        }
        else
        {
            num = "<n/a>";
        }
    }

    /* Any active calls should be placed on hold now */
    call = callChangeStatus(inst, CS_ACTIVE, CS_HELD);
    if(call != NO_CALL)
    {
        sprintf(buf1, "Active call put on hold (index %i)\n", call);
        CsrStrCat(buf, buf1);
    }

    /* Add new call to list */
    call = callGetUnused(inst);
    if(call != NO_CALL)
    {
        inst->calls[call].isSet = TRUE;
        inst->calls[call].dir = MOBILE_ORIGINATED;
        inst->calls[call].status = CS_DIALING;
        inst->calls[call].mode = VOICE;
        inst->calls[call].mpty = NOT_MULTIPARTY_CALL;

        if(num[0] == '+')
        {/*if + is present num type shall be 145*/
            inst->calls[call].numType = NUMBER_TYPE_INT;
        }
        else
        {
            inst->calls[call].numType = NUMBER_TYPE_STD;
        }

        numLen = (CsrUint8) CsrStrLen(num);
        CsrMemCpy(inst->calls[call].number,
               num,
               numLen+1);
        sprintf(buf1, "Added new outgoing call to list (index %i)\n", (CsrUint8 )(call+1));
        CsrStrCat(buf, buf1);
    }
    else
    {
        sprintf(buf1, "Could not find empty call list entry for new call\n");
        CsrStrCat(buf, buf1);
    }

    /* Show special menu */
    inst->current = getIndex(inst, con);
    inst->outgoingCall = TRUE;
    con->dialOk = FALSE;

    sprintf(buf1, "Dial command received:");
    CsrStrCat(buf, buf1);
    switch(prim->command)
    {
        case CSR_BT_HFG_DIAL_NUMBER:
            {
                sprintf(buf1, "Dial number: '%s'\n", num);
                CsrStrCat(buf, buf1);
                inst->outgoingCall = TRUE;
                CsrBtHfgSetDialog(inst, CSR_BT_HFG_DIAL_INFO_DIALOG_UI, TEXT_SUCCESS_UCS2,
                                                    CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);

                CsrUiDialogSetReqSend(inst->csrUiVar.displayesHandlers[CSR_BT_HFG_DIAL_INFO_DIALOG_UI].displayHandle,
                                                            TEXT_SUCCESS_UCS2, CONVERT_TEXT_STRING_2_UCS2(buf),
                                                            TEXT_OK_UCS2, NULL,
                                                            inst->csrUiVar.displayesHandlers[CSR_BT_HFG_DIAL_INFO_DIALOG_UI].sk1EventHandle, CSR_UI_DEFAULTACTION,
                                                            inst->csrUiVar.displayesHandlers[CSR_BT_HFG_DIAL_INFO_DIALOG_UI].backEventHandle, CSR_UI_DEFAULTACTION);

                CsrBtHfgShowUi(inst, CSR_BT_HFG_DIAL_INFO_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
                break;
            }

        case CSR_BT_HFG_DIAL_MEMORY:
            {
                /* Simulating that valid memory location number are 0-9 */
                if (strlen(num) <= 2)
                {
                    if (num[0] >= 0x30 && num[0] <= 0x39)
                    {
                        sprintf(buf, "Dial memory: '%s'\n", num);
                        inst->outgoingCall = TRUE;

                        CsrBtHfgSetDialog(inst, CSR_BT_HFG_DIAL_INFO_DIALOG_UI, TEXT_SUCCESS_UCS2,
                                                            CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);

                        CsrUiDialogSetReqSend(inst->csrUiVar.displayesHandlers[CSR_BT_HFG_DIAL_INFO_DIALOG_UI].displayHandle,
                                                                    TEXT_SUCCESS_UCS2, CONVERT_TEXT_STRING_2_UCS2(buf),
                                                                    TEXT_OK_UCS2, NULL,
                                                                    inst->csrUiVar.displayesHandlers[CSR_BT_HFG_DIAL_INFO_DIALOG_UI].sk1EventHandle, CSR_UI_DEFAULTACTION,
                                                                    inst->csrUiVar.displayesHandlers[CSR_BT_HFG_DIAL_INFO_DIALOG_UI].backEventHandle, CSR_UI_DEFAULTACTION);

                        CsrBtHfgShowUi(inst, CSR_BT_HFG_DIAL_INFO_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
                    }
                    else
                    {
                        sprintf(buf, "The memory location number given (%c) is invalid. Valid numbers are 0-9\n \
                                                Returning an error message...\n ", num[0]);

                        CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_FAILED_UCS2,
                                                                            CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
                        CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);

                        CsrBtHfgDialResSend(con->index,CSR_BT_CME_UNKNOWN);
                    }
                }
                else
                {
                    sprintf(buf, "The memory location number given (%s) is invalid. Valid numbers are 0-9\n \
                                                Returning an error message...\n", num);
                    CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_FAILED_UCS2,
                                                                        CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
                    CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);

                    CsrBtHfgDialResSend(con->index,CSR_BT_CME_UNKNOWN);
                }
                break;
            }

        case CSR_BT_HFG_DIAL_REDIAL:
            {
                sprintf(buf, "Redial last number: '%s'\n",num);
                CsrBtHfgSetDialog(inst, CSR_BT_HFG_DIAL_INFO_DIALOG_UI, TEXT_SUCCESS_UCS2,
                                                                    CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
                CsrBtHfgShowUi(inst, CSR_BT_HFG_DIAL_INFO_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);

                break;
            }

        default:
            {
                sprintf(buf, "Unknown command '%i'\n",
                       prim->command);
                CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_FAILED_UCS2,
                                                                    CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
                CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
                return;
            }
    }
}

/* Call list */
static void handleCallListInd(hfgInstance_t *inst, CsrBtHfgCallListInd *prim)
{
    CsrUint8 i;
    CsrUint8 j;
    CsrBool last;

    /* Count number of calls to be able to detect the last one */
    j = 0;
    for(i=0; i<MAX_CALLS; i++)
    {
        if(inst->calls[i].isSet)
        {
            j++;
            last = (j+1 >= callGetCount(inst)
                    ? TRUE
                    : FALSE);

            /* Send entry */
            CsrBtHfgCallListResSend(prim->connectionId,
                               last,
                               (CsrUint8) (i+1),
                               inst->calls[i].dir,
                               inst->calls[i].status,
                               inst->calls[i].mode,
                               inst->calls[i].mpty,
                               (CsrCharString *) StringDup((char*)(inst->calls[i].number)),
                               inst->calls[i].numType,
                               CSR_BT_CME_SUCCESS);
        }
    }

    /* Send empty notification? */
    if(j == 0)
    {
        CsrBtHfgCallListResSend(prim->connectionId,
                           TRUE, 0, 0, 0, 0, 0, NULL, 0, CSR_BT_CME_SUCCESS);
    }

}

/* Handle HFG primitives */
void handleHfgPrim(hfgInstance_t *inst)
{
    CsrBtHfgPrim *msg;
    CsrUint16  *event;
    CsrBtHfgCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrCharString buf[400] = "";
    CsrCharString buf1[60] = "";
#ifndef EXCLUDE_CSR_BT_HFG_MODULE_OPTIONAL
    CsrUint8 *parserCmdList = (CsrUint8*) CsrPmemAlloc(CSR_BT_HFG_APP_NUMBER_AT_CMD);
#endif
    msg   = (CsrBtHfgPrim*)inst->recvMsgP;
    event = (CsrBtHfgPrim*)msg;

#if 0
    /* Dialog is shown in BLOCK mode. so unblock it*/
    if(inst->dialogShow)
    {
        CsrUiDisplaySetinputmodeReqSend(csrUiVar->displayesHandlers[CSR_BT_HFG_DEFAULT_INFO_UI].displayHandle, CSR_UI_INPUTMODE_AUTO);
    }
#endif
    switch(*event)
    {
        case CSR_BT_HFG_DEACTIVATE_CFM:
            {
                CsrSchedTimerCancel(inst->deactGuard,NULL,NULL);

                initHfgData(inst);
                inst->state = stDeactivated;

/*                CsrUiDisplaySetinputmodeReqSend(csrUiVar->displayesHandlers[CSR_BT_HFG_DEFAULT_INFO_UI].displayHandle, CSR_UI_INPUTMODE_AUTO);*/
/*                CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_HFG_DEACTIVATION_UCS2,*/
/*                                    TEXT_HFG_DEACTIVATED_UCS2, TEXT_OK_UCS2, NULL);*/
                CsrUiUieHideReqSend(csrUiVar->displayesHandlers[CSR_BT_HFG_DEFAULT_INFO_UI].displayHandle);
/*                CsrUiUieHideReqSend(csrUiVar->displayesHandlers[CSR_BT_HFG_DEACT_UI].displayHandle);*/

                updateMainMenu(inst);

                break;
            }

        case CSR_BT_HFG_SERVICE_CONNECT_IND:
            {
                CsrBtHfgServiceConnectInd *prim;
                prim = (CsrBtHfgServiceConnectInd*)msg;

                handleServiceConnectInd(inst, prim);

                CsrPmemFree(prim->serviceName);
            }
            break;

        case CSR_BT_HFG_DISCONNECT_IND:
            {
                CsrBtHfgDisconnectInd *prim;
                prim = (CsrBtHfgDisconnectInd*)msg;

                handleDisconnectInd(inst, prim);
            }
            break;

        case CSR_BT_HFG_AUDIO_DISCONNECT_CFM:
            {
                Connection_t *con;
                CsrBtHfgAudioDisconnectCfm *prim;
                prim = (CsrBtHfgAudioDisconnectCfm*)msg;
                con = getIdConnection(inst, prim->connectionId);

                /*CSR_BT_HFG_AUDIO_DISCONNECT_REQ is already sent and dialog is displayed in BLOCK mode*/
                CsrUiDisplaySetinputmodeReqSend(csrUiVar->displayesHandlers[CSR_BT_HFG_DEFAULT_INFO_UI].displayHandle, CSR_UI_INPUTMODE_AUTO);

                con->audioOn = FALSE;

                if((prim->resultCode == CSR_BT_RESULT_CODE_HFG_SUCCESS) && (prim->resultSupplier == CSR_BT_SUPPLIER_HFG))
                {
                    sprintf(buf, "Audio has been turned off\n");
                    CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                                            CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);

                    CsrHciDeRegisterScoHandle(prim->scoHandle);
                }
                else
                {
                    sprintf(buf, "Audio OFF failed with resultCode=0x%04X and resultSupplier=0x%04X\n", prim->resultCode, prim->resultSupplier);
                    CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_FAILED_UCS2,
                                                                            CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
                }
                inst->dialogShow = FALSE;
            }
            break;

        case CSR_BT_HFG_AUDIO_DISCONNECT_IND:
            {
                Connection_t *con;
                CsrBtHfgAudioDisconnectInd *prim;

                prim = (CsrBtHfgAudioDisconnectInd*)msg;
                con = getIdConnection(inst, prim->connectionId);

                CsrHciDeRegisterScoHandle(prim->scoHandle);

                con->audioOn = FALSE;

                sprintf(buf, "Audio has been turned OFF by peer with reasonCode=0x%04X and reasonSupplier=0x%04X\n", prim->reasonCode, prim->reasonSupplier);
                CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_FAILED_UCS2,
                                                                    CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
                CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
            }
            break;

        case CSR_BT_HFG_AUDIO_CONNECT_IND:
        case CSR_BT_HFG_AUDIO_CONNECT_CFM:
            {
                CsrBtHfgAudioConnectInd *prim;
                prim = (CsrBtHfgAudioConnectInd*)msg;

                handleAudioInd(inst, prim);
            }
            break;

        case CSR_BT_HFG_AUDIO_ACCEPT_CONNECT_IND:
            {
                CsrBtHfgAudioAcceptConnectInd *prim;
                prim = (CsrBtHfgAudioAcceptConnectInd*)msg;

                sprintf(buf, "Received HFG Audio Accept Connect Ind, sending Response...\n");
                /* Simple reply */
                CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                                    CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
                CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);

                CsrBtHfgAudioAcceptConnectResSend(prim->connectionId, HCI_SUCCESS, NULL,
                                    PCM_SLOT,
                                    PCM_SLOT_REALLOCATE);
            }
            break;

        case CSR_BT_HFG_SECURITY_IN_CFM:
            {
                CsrBtHfgSecurityInCfm *prim;
                prim = (CsrBtHfgSecurityInCfm*)msg;

                /* Inform user */
                sprintf(buf, "Incoming security level changed, error code 0x%02X, supplier 0x%02X\n",
                       prim->resultCode, prim->resultSupplier);
                CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                                    CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
                CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
            }
            break;

        case CSR_BT_HFG_SECURITY_OUT_CFM:
            {
                CsrBtHfgSecurityOutCfm *prim;
                prim = (CsrBtHfgSecurityOutCfm*)msg;

                /* Inform user */
                sprintf(buf, "Outgoing security level changed, error code 0x%02X, supplier 0x%02X\n",
                       prim->resultCode, prim->resultSupplier);
                CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                                    CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
                CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);

            }
            break;

        case CSR_BT_HFG_STATUS_LP_IND:
            {
                CsrBtHfgStatusLpInd *prim;
                const char *modes[] =
                    {
                        "active",
                        "hold",
                        "sniff",
                        "park",
                        "disconnected"
                    };

                prim = (CsrBtHfgStatusLpInd*)msg;
                if (prim->currentMode == CSR_BT_LINK_STATUS_DISCONNECTED)
                {
                    prim->currentMode = 4;
                }
                if (prim->oldMode == CSR_BT_LINK_STATUS_DISCONNECTED)
                {
                    prim->oldMode = 4;
                }
                if (prim->wantedMode == CSR_BT_LINK_STATUS_DISCONNECTED)
                {
                    prim->wantedMode = 4;
                }
                /* Inform user */
                CsrAppBacklogReqSend(TECH_BT, PROFILE_HFG, FALSE, "Mode change: id: 0x%04X - code: 0x%02X (suppl 0x%02X) - cur: %s - old: %s - want: %s - %s request\n",
                       prim->connectionId,
                       prim->resultCode,
                       prim->resultSupplier,
                       modes[prim->currentMode],
                       modes[prim->oldMode],
                       modes[prim->wantedMode],
                       (prim->remoteReq ? "remote" : "local"));
            }
            break;

        case CSR_BT_HFG_STATUS_AUDIO_IND:
            {
                CsrBtHfgStatusAudioInd *prim;
                prim = (CsrBtHfgStatusAudioInd*)msg;

                handleStatusAudioInd(inst, prim);

                CsrPmemFree(prim->audioSetting);
            }
            break;

        case CSR_BT_HFG_RING_CFM:
            {
                CsrBtHfgRingCfm *prim;
                prim = (CsrBtHfgRingCfm*)msg;

                /* Inform user */
                sprintf(buf, "Ringing completed for connection 0x%04X\n",
                       prim->connectionId);
                CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                                CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
                CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
            }
            break;

        case CSR_BT_HFG_ANSWER_IND:
            {
                CsrBtHfgAnswerInd *prim;
                prim = (CsrBtHfgAnswerInd*)msg;

                handleAnswerInd(inst, prim);
            }
            break;

        case CSR_BT_HFG_REJECT_IND:
            {
                CsrBtHfgRejectInd *prim;
                prim = (CsrBtHfgRejectInd*)msg;

                handleRejectInd(inst, prim);
            }
            break;

        case CSR_BT_HFG_CALL_HANDLING_IND:
            {
                CsrBtHfgCallHandlingInd *prim;
                prim = (CsrBtHfgCallHandlingInd*)msg;

                handleCallHandlingInd(inst, prim);
            }
            break;

        case CSR_BT_HFG_DIAL_IND:
            {
                CsrBtHfgDialInd *prim;
                prim = (CsrBtHfgDialInd*)msg;

                handleDialInd(inst, prim);

                CsrPmemFree(prim->number);
            }
            break;

        case CSR_BT_HFG_SPEAKER_GAIN_IND:
            {
                CsrBtHfgSpeakerGainInd *prim;
                Connection_t *con;
                prim = (CsrBtHfgSpeakerGainInd*)msg;

                /* Inform user and store setting */
                con = getIdConnection(inst, prim->connectionId);
                if(con != NULL)
                {
                    con->speakerGain = prim->gain;
                }
                sprintf(buf, "Speaker gain set by headset to %i\n",
                       prim->gain);
                CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                                CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
                CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
            }
            break;

        case CSR_BT_HFG_MIC_GAIN_IND:
            {
                CsrBtHfgMicGainInd *prim;
                Connection_t *con;
                prim = (CsrBtHfgMicGainInd*)msg;

                /* Inform user and store setting */
                con = getIdConnection(inst, prim->connectionId);
                if(con != NULL)
                {
                    con->micGain = prim->gain;
                }
                sprintf(buf, "Microphone gain set by headset to %i\n",
                       prim->gain);
                CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                                CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
                CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
            }
            break;

        case CSR_BT_HFG_AT_CMD_IND:
            {
                CsrBtHfgAtCmdInd *prim;
                prim = (CsrBtHfgAtCmdInd*)msg;

                sprintf(buf, "Received raw AT command - cmee is %s:\n >>>%s<<<\n",
                       (prim->cmeeEnabled ? "on" : "off"), prim->command);
                if (inst->parserMode != CSR_BT_HFG_AT_MODE_FULL)
                {
                    sprintf(buf1, "AT cmd received not handled as it should be...\n");
                }
                CsrStrCat(buf, buf1);
                CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                                CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
                CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);

                CsrPmemFree(prim->command);
            }
            break;

        case CSR_BT_HFG_OPERATOR_IND:
            {
                CsrBtHfgOperatorInd *prim;
                prim = (CsrBtHfgOperatorInd*)msg;

                /* Inform user and reply */
                sprintf(buf, "Received a operator indication, sending reply...\n");
                if (inst->copsSupport)
                {
                    CsrBtHfgOperatorResSend(prim->connectionId,
                                       3, /* mode always 3 */
                                       (CsrCharString *) StringDup(OPERATOR_NAME),
                                       CSR_BT_CME_SUCCESS);
                    sprintf(buf1, "\tName: '%s', mode %i\n",
                               OPERATOR_NAME, 3);
                }
                else
                {
                    CsrBtHfgOperatorResSend(prim->connectionId,
                                       3, /* mode always 3 */
                                       NULL,
                                       CSR_BT_CME_OPERATION_NOT_SUPPORTED); /* No operator name support */
                    sprintf(buf1, "\tOperator name not available\n");
                }
                CsrStrCat(buf,buf1);
                CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                                CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
                CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);

            }
            break;

        case CSR_BT_HFG_CALL_LIST_IND:
            {
                CsrBtHfgCallListInd *prim;
                prim = (CsrBtHfgCallListInd*)msg;
                if (inst->callListCmdSupport)
                {
                    handleCallListInd(inst, prim);
                }
                else
                {
                    CsrCharString *body = CsrPmemAlloc(10);
                    body[0] = '\r';
                    body[1] = '\n';
                    body[2] = 'E';
                    body[3] = 'R';
                    body[4] = 'R';
                    body[5] = 'O';
                    body[6] = 'R';
                    body[7] = '\r';
                    body[8] = '\n';
                    body[9] = 0;

                    sprintf(buf, "Call list indication not supported: answer ERROR\n");
                    CsrBtHfgAtCmdReqSend(prim->connectionId,body);
                    CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                                    CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
                    CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
                }

            }
            break;

        case CSR_BT_HFG_SUBSCRIBER_NUMBER_IND:
            {
                CsrBtHfgSubscriberNumberInd *prim;
                prim = (CsrBtHfgSubscriberNumberInd*)msg;

                /* Send two numbers and inform user */
                sprintf(buf, "Subscriber-number indicator received, sending replies...\n \
                                                     \tNumber: '%s', number-type: %i, service: %i\n \
                                                     \tNumber: '%s', number-type: %i, service: %i\n",
                                                    SUB_NUMBER_1, NUMBER_TYPE_INT, VOICE,
                                                    SUB_NUMBER_2, NUMBER_TYPE_INT, VOICE);
                CsrBtHfgSubscriberNumberResSend(prim->connectionId,
                                           FALSE,
                                           (CsrCharString *) StringDup(SUB_NUMBER_1),
                                           NUMBER_TYPE_INT,
                                           VOICE,
                                           CSR_BT_CME_SUCCESS);

                CsrBtHfgSubscriberNumberResSend(prim->connectionId,
                                           TRUE,
                                           (CsrCharString *) StringDup(SUB_NUMBER_2),
                                           NUMBER_TYPE_INT,
                                           VOICE,
                                           CSR_BT_CME_SUCCESS);
                CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                                CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
                CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
            }
            break;

        case CSR_BT_HFG_GENERATE_DTMF_IND:
            {
                CsrBtHfgGenerateDtmfInd *prim;
                prim = (CsrBtHfgGenerateDtmfInd*)msg;

                /* Inform user */
                sprintf(buf, "Received DTMF indication for tone '%c' (0x%02x)\n",
                       prim->dtmf, prim->dtmf);
                CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                                CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
                CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
            }
            break;

        case CSR_BT_HFG_NOISE_ECHO_IND:
            {
                CsrBtHfgNoiseEchoInd *prim;
                Connection_t *con;
                prim = (CsrBtHfgNoiseEchoInd*)msg;

                con = getIdConnection(inst, prim->connectionId);
                /* Inform user and store setting */
                if (prim->nrec == TRUE)
                {
                    con->nrec = TRUE;
                    /* TRUE */
                }
                else
                {
                    con->nrec = FALSE;
                }
                sprintf(buf, "Received noise/echo supported feature '%s'\n", (con->nrec ? "on" : "off"));
                CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                                CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
                CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
            }
            break;

        case CSR_BT_HFG_BT_INPUT_IND:
            {
                CsrBtHfgBtInputInd *prim;
                prim = (CsrBtHfgBtInputInd*)msg;

                /* Inform user, send response */
                if(prim->request == 1)
                {
                    CsrBtHfgBtInputResSend(prim->connectionId,
                                      CSR_BT_CME_SUCCESS,
                                      (CsrCharString *) StringDup(BINP_RES));
                    sprintf(buf, "Received a BINP 1 'voice tag' indication, sent number reply..\n");
                }
                else
                {
                    CsrBtHfgBtInputResSend(prim->connectionId,
                                      CSR_BT_CME_NO_NETWORK_SERVICE,
                                      NULL);
                    sprintf(buf, buf, "Received unknown BINP request '%i', sent error reply...\n", prim->request);
                }
                CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                                CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
                CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
            }
            break;

        case CSR_BT_HFG_VOICE_RECOG_IND:
            {
                CsrBtHfgVoiceRecogInd *prim;
                Connection_t *con;
                CsrCharString heading[30] = TEXT_SUCCESS_STRING;
                CsrUint8 inputMode = CSR_UI_INPUTMODE_AUTO;

                prim = (CsrBtHfgVoiceRecogInd*)msg;

                /* Inform user and store setting */

                con = getIdConnection(inst, prim->connectionId);
                if (prim->bvra)
                {
                    /* TRUE */
                    con->voiceRecognition = TRUE;
                }
                else
                {
                    con->voiceRecognition = FALSE;
                }
                sprintf(buf, "Received voice recognition setting '%s'\n", (con->voiceRecognition ? "on" : "off"));

                if(con != NULL)
                {
                    con->voiceRecognition = prim->bvra;

                    if((!con->audioOn && con->voiceRecognition) ||
                       (con->audioOn && !con->voiceRecognition))
                    {
                        sprintf(buf1, "Requesting audio %s for voice recognition...\n",
                               (con->voiceRecognition ? "on" : "off"));
                        inputMode = CSR_UI_INPUTMODE_BLOCK;
                        CsrStrCat(heading, TEXT_PLEASE_WAIT_STRING);
                        if(con->voiceRecognition)
                        {
                            CsrBtHfgAudioConnectReqSend(con->index,PCM_SLOT, PCM_SLOT_REALLOCATE);
                        }
                        else
                        {
                            CsrBtHfgAudioDisconnectReqSend(con->index);
                        }

                        CsrBtHfgVoiceRecogResSend(prim->connectionId, CSR_BT_CME_SUCCESS);
                    }
                    else
                    {
                        sprintf(buf1, "%s voice recognition...\n", (con->voiceRecognition ? "Activating" : "Deactivating"));
                        CsrBtHfgVoiceRecogResSend(prim->connectionId, CSR_BT_CME_SUCCESS);
                    }
                    CsrStrCat(buf, buf1);
                }
                else
                {
                    CsrBtHfgVoiceRecogResSend(prim->connectionId, CSR_BT_CME_OPERATION_NOT_ALLOWED);
                }
                CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CONVERT_TEXT_STRING_2_UCS2(heading),
                                                                CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
                CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, inputMode, CSR_BT_HFG_STD_PRIO);
            }
            break;
#ifdef CSR_BT_INSTALL_HFG_C2C
        case CSR_BT_HFG_C2C_SF_IND:
            {
                CsrBtHfgC2cSfInd *prim;
                Connection_t *con;
                prim = (CsrBtHfgC2cSfInd*)msg;

                con = getIdConnection(inst, prim->connectionId);
                if(con != NULL)
                {
                    CsrMemCpy(con->c2cIndicators,
                           prim->indicators,
                           CSRMAX(CSR_BT_C2C_NUMBER_OF_INDICATORS, prim->indicatorsLength));
                }
                CsrPmemFree(prim->indicators);
                sprintf(buf, "Received a new set of CSR-2-CSR supported features\n");
                CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                                CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
                CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
            }
            break;

        case CSR_BT_HFG_C2C_SMS_GET_IND:
            {
                CsrBtHfgC2cSmsGetInd *prim;
                prim = (CsrBtHfgC2cSmsGetInd*)msg;

                /* Send reply, inform user */
                CsrBtHfgC2cSmsTxtReqSend(prim->connectionId, (CsrCharString *) StringDup(SMS_TEXT));
                sprintf(buf, "Received indication to send SMS with index %i\n", prim->smsIndex);
                CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                                CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
                CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
            }
            break;

        case CSR_BT_HFG_C2C_BATTERY_IND:
            {
                CsrBtHfgC2cBatteryInd *prim;
                prim = (CsrBtHfgC2cBatteryInd*)msg;

                /* Notify user */
                sprintf(buf, "Received battery indication '%i'\n", prim->batteryLevel);
                CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                                CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
                CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
            }
            break;

        case CSR_BT_HFG_C2C_POWER_IND:
            {
                CsrBtHfgC2cPowerInd *prim;
                prim = (CsrBtHfgC2cPowerInd*)msg;

                /* Notify user */
                sprintf(buf, "Received power status indication '%i'\n", prim->powerStatus);
                CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                                CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
                CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
            }
            break;
#endif
#ifdef CSR_BT_INSTALL_HFG_CONFIG_AUDIO
        case CSR_BT_HFG_CONFIG_ATCMD_HANDLING_CFM:
            {
                CsrBtHfgConfigAtcmdHandlingCfm *prim;

                prim = (CsrBtHfgConfigAtcmdHandlingCfm *)msg;

                if (!prim->result) /* HFG_SUCCESS */
                {
                    sprintf(buf, "AT cmd configuration successful! \n");
                }
                else
                {
                    sprintf(buf, "AT cmd configuration failed. Error code: %d\n",prim->result);
                }
                /* No matter what, update the local settings with the actual settings....*/
                CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                                CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
                CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);

                CsrMemCpy(inst->parserCmdList,prim->bitwiseIndicators,CSR_BT_HFG_APP_NUMBER_AT_CMD);
                CsrPmemFree(prim->bitwiseIndicators);
            }
            break;
#endif
#ifdef CSR_BT_INSTALL_HFG_CONFIG_SINGLE_ATCMD
        case CSR_BT_HFG_CONFIG_SINGLE_ATCMD_CFM:
            {
                CsrBtHfgConfigSingleAtcmdCfm *prim;

                prim = (CsrBtHfgConfigSingleAtcmdCfm *)msg;

                if (!prim->result) /* HFG_SUCCESS */
                {
                    sprintf(buf, "Single AT cmd configuration successful! \n");
                }
                else
                {
                    sprintf(buf, "Single AT cmd configuration failed. Error code: %d\n",prim->result);
                }
                /* We need to sync'e with the profile...*/
                CsrMemCpy(parserCmdList, inst->parserCmdList, CSR_BT_HFG_APP_NUMBER_AT_CMD);
                CsrBtHfgConfigAtcmdHandlingReqSend(CSR_BT_HFG_APP_IFACEQUEUE, parserCmdList, CSR_BT_HFG_APP_NUMBER_AT_CMD);
                CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                                CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
                CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
            }
            break;
#endif
#ifdef CSR_BT_INSTALL_HFG_C2C
        case CSR_BT_HFG_GET_C2C_ADPCM_LOCAL_SUPPORTED_IND:
            {
                CsrBtHfgGetC2cAdpcmLocalSupportedInd *prim;

                prim = (CsrBtHfgGetC2cAdpcmLocalSupportedInd *)msg;
                if (prim->result)
                {
                    sprintf(buf, "ADPCM codec supported locally\n");
                    /* This is just a guess, so let it be set correctly when config message arrives.... */
                    inst->supportedAudioCodecMask = CSR_BT_C2C_ADPCM_OFF_VALUE|CSR_BT_C2C_ADPCM_4BIT_VALUE|CSR_BT_C2C_ADPCM_2BIT_VALUE;
                }
                else
                {
                    sprintf(buf, "No local ADPCM support\n");
                }
                CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_SUCCESS_UCS2,
                                                                CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
                CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
                break;
            }
#endif
        default:
            sprintf(buf, "Unknown HFG primitive received: 0x%04x,\n", *event);
            CsrBtHfgSetDialog(inst, CSR_BT_HFG_DEFAULT_INFO_UI, TEXT_FAILED_UCS2,
                                                            CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL);
            CsrBtHfgShowUi(inst, CSR_BT_HFG_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_HFG_STD_PRIO);
            CsrBtHfgFreeUpstreamMessageContents(CSR_BT_HFG_PRIM, event);
            break;
    }
}
