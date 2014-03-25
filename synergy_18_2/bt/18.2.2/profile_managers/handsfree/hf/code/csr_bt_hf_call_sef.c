/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #3 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_sched.h"
#include "csr_pmem.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_util.h"
#include "csr_am_lib.h"

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

#include "csr_bt_hf_main.h"
#include "csr_bt_hf_prim.h"
#include "csr_bt_hf_util.h"
#include "csr_bt_hf_lib.h"
#include "csr_bt_hf_at_inter.h"
#include "csr_bt_sdc_support.h"
#include "csr_bt_hfhs_data_sef.h"

#ifdef CSR_USE_DSPM
#include "csr_dspm_lib.h"
#include "csr_bt_hf_dspm.h"
#endif


/*************************************************************************************
    An AT-command is received. Send it to HFG via CM.
************************************************************************************/
void CsrBtHfConnectedStateHfAtCmdReqHandler(HfMainInstanceData_t *instData)
{
    CsrBtHfAtCmdReq *prim;
    prim = (CsrBtHfAtCmdReq *) instData->recvMsgP;
    instData->linkData[instData->index].data->dataReceivedInConnected = TRUE;
    if ((instData->linkData[instData->index].lastAtCmdSent == idleCmd) && ((instData->mainConfig & CSR_BT_HF_AT_MODE_TRANSPARENT_ENABLE) == 0x00000000))
    {
        instData->linkData[instData->index].lastAtCmdSent = other;
    }
    CsrBtHfHsSendCmDataReq(instData, (CsrUint16)CsrStrLen((char *)prim->atCmdString),(CsrUint8 *)prim->atCmdString);
}

/*************************************************************************************
    Handler for CSR_BT_HF_CALL_HANDLING_REQ in Connected State
************************************************************************************/
void CsrBtHfConnectedStateHfChldReqHandler(HfMainInstanceData_t *instData)
{
    CsrBtHfCallHandlingReq * prim;
    CsrUint16 length;
    CsrUint8    * callHdlString;
    prim = (CsrBtHfCallHandlingReq *) instData->recvMsgP;

    if (prim->command < CSR_BT_BTRH_PUT_ON_HOLD)
    {/* CHLD_CMD */
        callHdlString = CsrPmemAlloc(CHLD_COMMAND_LENGTH);
        CsrMemSet(callHdlString,0,CHLD_COMMAND_LENGTH);
        length = CHLD_VALUE_INDEX + 1;
        CsrMemCpy(callHdlString, CHLD_COMMAND, length);
        length -= 1;
        switch (prim->command)
        {/* CHLD COMMANDS */
            case CSR_BT_RELEASE_ALL_HELD_CALL:
                {
                    callHdlString[length] = '0';
                    length++;
                    break;
                }
            case CSR_BT_RELEASE_ACTIVE_ACCEPT:
                {
                    callHdlString[length] = '1';
                    length++;
                    break;
                }
            case CSR_BT_RELEASE_SPECIFIED_CALL:
                {
                    callHdlString[length] = '1';
                    length++;
                    if (prim->index < 10)
                    {
                        callHdlString[length] = prim->index + '0';
                        length++;
                    }
                    else if (prim->index < 100)
                    {
                        callHdlString[length] = (prim->index / 10) + '0';
                        length++;
                        callHdlString[length] = (prim->index % 10) + '0';
                        length++;
                    }
                    else
                    {
                        /* index > 100 */
                        CsrPmemFree (callHdlString);
                        return;
                    }
                    break;
                }
            case CSR_BT_HOLD_ACTIVE_ACCEPT:
                {
                    callHdlString[length] = '2';
                    length++;
                    break;
                }
            case CSR_BT_REQUEST_PRIVATE_WITH_SPECIFIED:
                {
                    callHdlString[length] = '2';
                    length++;
                    if (prim->index < 10)
                    {
                        callHdlString[length] = prim->index + '0';
                        length++;
                    }
                    else if (prim->index < 100)
                    {
                        callHdlString[length] = (prim->index / 10) + '0';
                        length++;
                        callHdlString[length] = (prim->index % 10) + '0';
                        length++;
                    }
                    else
                    {
                        /* index > 100 */
                        CsrPmemFree (callHdlString);
                        return;
                    }
                    break;
                }
            case CSR_BT_ADD_CALL:
                {
                    callHdlString[length] = '3';
                    length++;
                    break;
                }
            case CSR_BT_CONNECT_TWO_CALLS:
                {
                    callHdlString[length] = '4';
                    length++;
                    break;
                }
            default:
                {
                    CsrPmemFree (callHdlString);
                    return;
                }
        }
        if (instData->linkData[instData->index].lastAtCmdSent == idleCmd)
        {
            instData->linkData[instData->index].lastAtCmdSent = chld;
        }
    }
    else
    {/* BTRH */
        callHdlString = CsrPmemAlloc(BTRH_COMMAND_LENGTH);
        CsrMemSet(callHdlString,0,BTRH_COMMAND_LENGTH);
        if (prim->command < CSR_BT_BTRH_READ_STATUS)
        {
            length = BTRH_VALUE_INDEX + 1;
        }
        else
        {
            length = BTRH_VALUE_INDEX;
        }
        CsrMemCpy(callHdlString, BTRH_COMMAND, length);
        length -= 1;
        if (prim->command < CSR_BT_BTRH_READ_STATUS)
        {/* Commands: BTRH_PUT_ON_HOLD, BTRH_ACCEPT_INCOMING or BTRH_REJECT_INCOMING ahsll be mapped to '0','1' or '2' respectively*/
            callHdlString[length] = prim->command - CSR_BT_BTRH_PUT_ON_HOLD + '0';
        }
        else
        {/* Query BTRH status */
            callHdlString[length] = '?';
        }
        length++;
        if (instData->linkData[instData->index].lastAtCmdSent == idleCmd)
        {
            instData->linkData[instData->index].lastAtCmdSent = btrh;
        }
    }

    callHdlString[length] = '\r';
    length++;

    CsrBtHfHsSendCmDataReq(instData, length, callHdlString);
}

/*************************************************************************************
    Handler for CSR_BT_HF_CALL_ANSWER_REQ in Connected State
************************************************************************************/
void CsrBtHfConnectedStateHfAnswerReqHandler(HfMainInstanceData_t *instData)
{
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    if (linkPtr->linkType == CSR_BT_HF_CONNECTION_HS)
    {
        sendCkpd(instData);
        if (linkPtr->lastAtCmdSent == idleCmd)
        {
            linkPtr->lastAtCmdSent = answer;
        }
    }
    else
    {
        CsrUint8         *dataPtr;
        CsrUint16       answerStrLen;

        answerStrLen = (CsrUint16) (CsrStrLen(ANSWER));
        dataPtr      = CsrPmemAlloc(answerStrLen + 1);
        CsrStrCpy((char*) dataPtr, ANSWER);
        if (linkPtr->lastAtCmdSent == idleCmd)
        {
            linkPtr->lastAtCmdSent = answer;
        }
        CsrBtHfHsSendCmDataReq(instData, answerStrLen, dataPtr);
    }
}

/*************************************************************************************
    Handler for CSR_BT_HF_CALL_END_REQ in Connected State
************************************************************************************/
void CsrBtHfConnectedStateHfCallEndReqHandler(HfMainInstanceData_t *instData)
{
    CsrUint8   *dataPtr;
    CsrUint16 rejectStrLen;

    rejectStrLen = (CsrUint16) (CsrStrLen(REJECT));
    dataPtr      = CsrPmemAlloc(rejectStrLen + 1);
    CsrStrCpy((char*) dataPtr, REJECT);
    if (instData->linkData[instData->index].lastAtCmdSent == idleCmd)
    {
        instData->linkData[instData->index].lastAtCmdSent = callEnd;
    }
    CsrBtHfHsSendCmDataReq(instData, rejectStrLen, dataPtr);
}

/*************************************************************************************
    Handler for CSR_BT_HF_SPEAKER_GAIN_STATUS_REQ in Connected State
************************************************************************************/
void CsrBtHfConnectedStateHfSpeakerGainStatusReqHandler(HfMainInstanceData_t *instData)
{
    CsrBtHfSpeakerGainStatusReq    *prim;
    CsrUint8                  *dataPtr;
    CsrUint16                spkStrLen;

    prim      = (CsrBtHfSpeakerGainStatusReq *) instData->recvMsgP;
    spkStrLen = (CsrUint16) (CsrStrLen(SPEAKER_GAIN));

    dataPtr   = CsrPmemAlloc(spkStrLen + 1);
    CsrStrCpy((char*) dataPtr, SPEAKER_GAIN);

    if(prim->gain > MAX_SPEAKER_GAIN)
    {
        prim->gain = MAX_SPEAKER_GAIN;
    }
    dataPtr[SPEAKER_GAIN_VALUE_INDEX]   = (char) ((prim->gain / 10) + '0');
    dataPtr[SPEAKER_GAIN_VALUE_INDEX+1] = (char) ((prim->gain % 10) + '0');
    if (instData->linkData[instData->index].lastAtCmdSent == idleCmd)
    {
        instData->linkData[instData->index].lastAtCmdSent = vgs;
    }
    CsrBtHfHsSendCmDataReq(instData, spkStrLen, dataPtr);
}

/*************************************************************************************
    Handler for CSR_BT_HF_MIC_GAIN_STATUS_REQ in Connected State
************************************************************************************/
void CsrBtHfConnectedStateHfMicGainStatusReqHandler(HfMainInstanceData_t *instData)
{
    CsrBtHfMicGainStatusReq    *prim;
    CsrUint8              *dataPtr;
    CsrUint16            micStrLen;

    prim      = (CsrBtHfMicGainStatusReq *) instData->recvMsgP;
    micStrLen = (CsrUint16) (CsrStrLen(MICROPHONE_GAIN));
    dataPtr   = CsrPmemAlloc(micStrLen + 1);
    CsrStrCpy((char*) dataPtr, MICROPHONE_GAIN);

    if(prim->gain > MAX_MICROPHONE_GAIN)
    {
        prim->gain = MAX_MICROPHONE_GAIN;
    }
    dataPtr[MICROPHONE_GAIN_VALUE_INDEX]   = (char) ((prim->gain / 10) + '0');
    dataPtr[MICROPHONE_GAIN_VALUE_INDEX+1] = (char) ((prim->gain % 10) + '0');
    if (instData->linkData[instData->index].lastAtCmdSent == idleCmd)
    {
        instData->linkData[instData->index].lastAtCmdSent = vgm;
    }
    CsrBtHfHsSendCmDataReq(instData, micStrLen, dataPtr);
}

/*************************************************************************************
    Handler for CSR_BT_HF_AUDIO_CONNECT_REQ in Connected State
************************************************************************************/
static void commonAudioHandler(HfMainInstanceData_t *instData,
                               CsrBool              audioOn,
                               hci_pkt_type_t       theAudioQuality,
                               CsrUint32            theTxBandwidth,
                               CsrUint32            theRxBandwidth,
                               CsrUint16            theMaxLatency,
                               CsrUint16            theVoiceSettings,
                               CsrUint8             theReTxEffort,
                               CsrBtHfConnectionId  connectionId,
                               CsrUint8             pcmSlot,
                               CsrBool              pcmReassign)
{
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    /*old stores*/
    linkPtr->audioQuality = theAudioQuality;
    linkPtr->lastAudioReq = audioOn;

    if (FALSE == linkPtr->audioPending)
    {
        if (audioOn == TRUE)
        {
            if (linkPtr->scoHandle == HF_SCO_UNUSED)
            {/* No SCO connection exists yet: establish one */
                linkPtr->pcmSlot     = pcmSlot;
                linkPtr->pcmReassign = pcmReassign;
                CsrBtCmScoCancelReqSend(CSR_BT_HF_IFACEQUEUE, linkPtr->hfConnId);
                if(theAudioQuality  == 0 &&
                   theTxBandwidth   == 0 &&
                   theRxBandwidth   == 0 &&
                   theMaxLatency    == 0 &&
                   theVoiceSettings == 0 &&
                   theReTxEffort    == 0)
                {
                    if(linkPtr->remoteVersion >= CSR_BT_FIRST_HFP_ESCO)
                    {
                        CsrBtHfSendCmScoConnectReq(linkPtr, CSR_BT_ESCO_DEFAULT_CONNECT, NULL);
                    }
                    else
                    {
                        CsrBtHfSendCmScoConnectReq(linkPtr, CSR_BT_SCO_DEFAULT_1P1, NULL);
                    }
                }
                else
                {
                    audioSetupParams_t audioParms;

                    audioParms.theAudioQuality  = theAudioQuality;
                    audioParms.theTxBandwidth   = theTxBandwidth;
                    audioParms.theRxBandwidth   = theRxBandwidth;
                    audioParms.theMaxLatency    = theMaxLatency;
                    audioParms.theVoiceSettings = theVoiceSettings;
                    audioParms.theReTxEffort    = theReTxEffort;
                    audioParms.pcmSlot          = linkPtr->pcmSlot;
                    audioParms.pcmReassign      = pcmReassign;

                    if(linkPtr->remoteVersion >= CSR_BT_FIRST_HFP_ESCO)
                    {
                        CsrBtHfSendCmScoConnectReq(linkPtr, CSR_BT_ESCO_DEFAULT_CONNECT, &audioParms);
                    }
                    else
                    {
                        CsrBtHfSendCmScoConnectReq(linkPtr, CSR_BT_SCO_DEFAULT_1P1, &audioParms);
                    }
                }
                linkPtr->audioPending = TRUE;
            }
            else
            { /* SCO connection already exists... just send indication back with SUCCESS result... */
                CsrBtHfSendHfAudioInd(instData, linkPtr->pcmSlot, 0, 0, 0, 0, 0, 0,
                                                CSR_BT_RESULT_CODE_HF_SUCCESS, 0xDEAD, CSR_BT_SUPPLIER_HF);
            }
        }
        else /* audioOn == FALSE */
        {
            if (linkPtr->scoHandle != HF_SCO_UNUSED)
            {
                CsrBtCmScoDisconnectReqSend(CSR_BT_HF_IFACEQUEUE, linkPtr->hfConnId);
                linkPtr->audioPending = TRUE;
            }
            else
            {
                CsrBtHfSendHfAudioDisconnectCfm(instData, linkPtr->scoHandle, CSR_BT_RESULT_CODE_HF_SUCCESS, CSR_BT_SUPPLIER_HF);
            }
        }
    }
}

void CsrBtHfConnectedStateHfAudioDisconnectReqHandler(HfMainInstanceData_t *instData)
{
    CsrBtHfAudioDisconnectReq *prim;
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    prim = (CsrBtHfAudioDisconnectReq *) instData->recvMsgP;

    linkPtr->outgoingAudio = TRUE;
    commonAudioHandler(instData,
                       FALSE,
                       linkPtr->audioSetupParams.theAudioQuality,
                       linkPtr->audioSetupParams.theTxBandwidth,
                       linkPtr->audioSetupParams.theRxBandwidth,
                       linkPtr->audioSetupParams.theMaxLatency,
                       linkPtr->audioSetupParams.theVoiceSettings,
                       linkPtr->audioSetupParams.theReTxEffort,
                       prim->connectionId,
                       0,
                       FALSE);
}

void CsrBtHfConnectedStateHfAudioReqHandler(HfMainInstanceData_t *instData)
{
    CsrBtHfAudioConnectReq *prim;
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);
    CsrBool wbsAudio = FALSE;

    prim = (CsrBtHfAudioConnectReq *) instData->recvMsgP;

    if (/* (linkPtr->remoteVersion == CSR_BT_FIRST_HFP_CODEC_NEG_ESCO) &&*/
        (linkPtr->supportedFeatures & CSR_BT_HFG_SUPPORT_CODEC_NEGOTIATION) &&
        (instData->localSupportedFeatures & CSR_BT_HF_SUPPORT_CODEC_NEGOTIATION))
    {
        wbsAudio = TRUE;
    }

    if (wbsAudio)
    {
        CsrBtCmScoCancelReqSend(CSR_BT_HF_IFACEQUEUE, linkPtr->hfConnId);
        linkPtr->scoConnectAcceptPending = FALSE;
        CsrBtHfSendAtBcc(instData);
    }
    else
    {/* Just call the audio handler function */
        commonAudioHandler(instData,
                           TRUE,
                           linkPtr->audioSetupParams.theAudioQuality,
                           linkPtr->audioSetupParams.theTxBandwidth,
                           linkPtr->audioSetupParams.theRxBandwidth,
                           linkPtr->audioSetupParams.theMaxLatency,
                           linkPtr->audioSetupParams.theVoiceSettings,
                           linkPtr->audioSetupParams.theReTxEffort,
                           prim->connectionId,
                           prim->pcmSlot,
                           prim->pcmRealloc);
    }
    CsrPmemFree(prim->audioParameters);
}


void CsrBtHfXStateMapScoPcmIndHandler(HfMainInstanceData_t *instData)
{
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);
    CsrBtCmMapScoPcmInd *prim;
    prim     = (CsrBtCmMapScoPcmInd *) instData->recvMsgP;


    if (TRUE == linkPtr->pcmMappingReceived)
    {
        CsrBtCmMapScoPcmResSend(linkPtr->hfConnId,
                           linkPtr->pcmSlot,
                           linkPtr->pcmReassign);
    }
#ifdef CSR_USE_DSPM
    else if (linkPtr->outgoingAudio)
    {
        CsrBtCmMapScoPcmResSend(linkPtr->hfConnId,
                           CSR_BT_PCM_DONT_MAP,
                           linkPtr->pcmReassign);
    }
#endif
    else
    {
        CsrBtHfAudioAcceptConnectInd *msg;
        msg                 = (CsrBtHfAudioAcceptConnectInd *)CsrPmemAlloc(sizeof(CsrBtHfAudioAcceptConnectInd));
        msg->type           = CSR_BT_HF_AUDIO_ACCEPT_CONNECT_IND;
        msg->connectionId   = linkPtr->hfConnId;
        msg->linkType       = prim->linkType;
        CsrBtHfMessagePut(instData->appHandle, msg);
        linkPtr->audioAcceptPending = TRUE;
    }
}

/***********************************************************************************
    result of sco connect request. Inform app layer.
************************************************************************************/
void CsrBtHfConnectedStateCmScoConnectCfmHandler(HfMainInstanceData_t *instData)
{
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    if (linkPtr->linkType == CSR_BT_HF_CONNECTION_HF)
    {/* if sco already in use a race condition may have occurred so ignore the confirm */
        CsrBtCmScoConnectCfm *prim;
        prim     = (CsrBtCmScoConnectCfm *) instData->recvMsgP;

        if (linkPtr->scoHandle == HF_SCO_UNUSED)
        {
            if ((TRUE == linkPtr->lastAudioReq) && (TRUE == linkPtr->audioPending))
            {
                if(prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
                {
                    linkPtr->pcmMappingReceived = FALSE;
                    linkPtr->scoHandle          = prim->eScoHandle;
#ifdef CSR_USE_DSPM
                    if(instData->hfDspmOpPending == DSPM_NOP)
                    {
                        if (linkPtr->dspmInst.operatorEncoder == HF_DSPM_DEFAULT_HANDLE)
                        {
                            CsrBtHfDspmSendCreateOperators(instData);
                        }
                        else
                        {
                            CsrHfDspmAudioConnect(instData);
                        }
                        instData->hfDspmOpPending = instData->index;
                    }
#endif
                    CsrBtHfSendHfAudioInd(instData,
                                          prim->pcmSlot,
                                          prim->linkType,
                                          prim->weSco,
                                          prim->rxPacketLength,
                                          prim->txPacketLength,
                                          prim->airMode,
                                          prim->txInterval,
                                          CSR_BT_RESULT_CODE_HF_SUCCESS,
                                          0xDEAD,
                                          CSR_BT_SUPPLIER_HF);

                    linkPtr->audioPending = FALSE;
                }
                else
                {
                    /* Send result to the application */
                    CsrBtHfSendHfAudioInd(instData,
                                          0,
                                          prim->linkType,
                                          prim->weSco,
                                          prim->rxPacketLength,
                                          prim->txPacketLength,
                                          prim->airMode,
                                          prim->txInterval,
                                          prim->resultCode,
                                          0xDEAD,
                                          prim->resultSupplier);

                    linkPtr->audioPending = FALSE;
                    /* No more trials; make sure to accept eSCO/SCO connections after this! */
                    /* In this case no sco connect accept can be pending... */
                    linkPtr->scoConnectAcceptPending = TRUE;
                    CsrBtCmScoAcceptConnectReqSend(CSR_BT_HF_IFACEQUEUE,
                                              linkPtr->hfConnId,
                                              linkPtr->audioSetupParams.theAudioQuality,
                                              linkPtr->audioSetupParams.theTxBandwidth,
                                              linkPtr->audioSetupParams.theRxBandwidth,
                                              linkPtr->audioSetupParams.theMaxLatency,
                                              linkPtr->audioSetupParams.theVoiceSettings,
                                              linkPtr->audioSetupParams.theReTxEffort);
                }
            }
            else
            {
                if (TRUE == linkPtr->audioPending)
                {
                    if(prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
                    {
                    /*Remember to release DSPM resourses here if using DSPM*/
                        linkPtr->pcmMappingReceived = FALSE;
                        CsrBtCmScoDisconnectReqSend(CSR_BT_HF_IFACEQUEUE, linkPtr->hfConnId);
                    }
                    else
                    {
                        CsrBtHfSendHfAudioInd(instData,
                                              0,
                                              0,
                                              prim->weSco,
                                              prim->rxPacketLength,
                                              prim->txPacketLength,
                                              prim->airMode,
                                              prim->txInterval,
                                              prim->resultCode,
                                              0xDEAD,
                                              prim->resultSupplier);

                        linkPtr->scoConnectAcceptPending = TRUE;
                        CsrBtCmScoAcceptConnectReqSend(CSR_BT_HF_IFACEQUEUE,
                                                  linkPtr->hfConnId,
                                                  linkPtr->audioSetupParams.theAudioQuality,
                                                  linkPtr->audioSetupParams.theTxBandwidth,
                                                  linkPtr->audioSetupParams.theRxBandwidth,
                                                  linkPtr->audioSetupParams.theMaxLatency,
                                                  linkPtr->audioSetupParams.theVoiceSettings,
                                                  linkPtr->audioSetupParams.theReTxEffort);
                    }
                }
                else
                {
                    if(prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
                    {
#ifdef CSR_USE_DSPM
                        if (instData->hfDspmOpPending == DSPM_NOP)
                        {
                            if (linkPtr->dspmInst.operatorEncoder == HF_DSPM_DEFAULT_HANDLE)
                            {
                                CsrBtHfDspmSendCreateOperators(instData);
                            }
                            else
                            {
                                CsrHfDspmAudioConnect(instData);
                            }
                            instData->hfDspmOpPending = instData->index;
                        }
#endif
                        linkPtr->pcmMappingReceived = FALSE;
                        CsrBtHfSendHfAudioInd(instData,
                                              prim->pcmSlot,
                                              prim->linkType,
                                              prim->weSco,
                                              prim->rxPacketLength,
                                              prim->txPacketLength,
                                              prim->airMode,
                                              prim->txInterval,
                                              CSR_BT_RESULT_CODE_HF_SUCCESS,
                                              0xDEAD,
                                              CSR_BT_SUPPLIER_HF);
                    }
                }
            }
        }
    }
}

/***********************************************************************************
    Handle CSR_BT_CM_SCO_DISCONNECT_IND in Connected state
************************************************************************************/
void CsrBtHfConnectedStateCmScoDisconnectIndHandler(HfMainInstanceData_t *instData)
{
    CsrBtCmScoDisconnectInd    *prim;
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    prim    = (CsrBtCmScoDisconnectInd *) instData->recvMsgP;
    if (linkPtr->linkType == CSR_BT_HF_CONNECTION_HF)
    {
        /* if sco already disconnected a race condition may have occurred so ignore the message */
        if (linkPtr->scoHandle != HF_SCO_UNUSED)
        {
            if (linkPtr->updateAudioSettingsFromMain)
            {/* Take the new audio config settings to use them at a later connection */
                CsrMemCpy(&linkPtr->audioSetupParams , &instData->generalAudioParams, sizeof(audioSetupParams_t));
                linkPtr->updateAudioSettingsFromMain = FALSE;
            }
            if ((linkPtr->lastAudioReq == FALSE) && (TRUE == linkPtr->audioPending))
            {
                if (prim->status)
                {
                    CsrUint16 voiceSettings = linkPtr->audioSetupParams.theVoiceSettings;
                    CsrUint32 txBandwidth   = linkPtr->audioSetupParams.theTxBandwidth;
                    CsrUint32 rxBandwidth   = linkPtr->audioSetupParams.theRxBandwidth;
                    CsrUint16 audioQuality  = linkPtr->audioSetupParams.theAudioQuality;

                    linkPtr->scoHandle    = HF_SCO_UNUSED;
                    linkPtr->audioPending = FALSE;
                    if (linkPtr->outgoingAudio)
                    {
                        CsrBtHfSendHfAudioDisconnectCfm(instData, prim->eScoHandle, CSR_BT_RESULT_CODE_HF_SUCCESS, CSR_BT_SUPPLIER_HF);
                    }
                    else
                    {
                        CsrBtHfSendHfAudioDisconnectInd(instData, prim->eScoHandle, prim->reasonCode, prim->reasonSupplier);
                    }

                    linkPtr->scoConnectAcceptPending = TRUE;
                    CsrBtCmScoAcceptConnectReqSend(CSR_BT_HF_IFACEQUEUE,
                                              linkPtr->hfConnId,
                                              audioQuality,
                                              txBandwidth,
                                              rxBandwidth,
                                              linkPtr->audioSetupParams.theMaxLatency,
                                              voiceSettings,
                                              linkPtr->audioSetupParams.theReTxEffort);
#ifdef CSR_USE_DSPM                    
                    if (instData->hfDspmOpPending == DSPM_NOP)
                    {
                        CsrBtHfDspmSendDisconnectStreams(instData);
                        instData->hfDspmOpPending = instData->index;
                    }
#endif
                }
                else
                {
                    /* sco disconnect failed but it must be disconnected so we try again */
                    CsrBtCmScoDisconnectReqSend(CSR_BT_HF_IFACEQUEUE, linkPtr->hfConnId);
                }
            }
            else
            {
                if (TRUE == linkPtr->audioPending)
                {
                    /* app requested ON while waiting for off */
                    if (prim->status)
                    {
#ifdef CSR_USE_DSPM
                        if (instData->hfDspmOpPending == DSPM_NOP)
                        {
                            if (linkPtr->dspmInst.operatorEncoder == HF_DSPM_DEFAULT_HANDLE)
                            {
                                CsrBtHfDspmSendCreateOperators(instData);
                            }
                            else
                            {
                                CsrHfDspmAudioConnect(instData);
                            }
                            instData->hfDspmOpPending = instData->index;
                        }
#endif
                        if(linkPtr->remoteVersion >= CSR_BT_FIRST_HFP_ESCO)
                        {
                            CsrBtHfSendCmScoConnectReq(linkPtr, CSR_BT_ESCO_DEFAULT_CONNECT, NULL);
                        }
                        else
                        {
                            CsrBtHfSendCmScoConnectReq(linkPtr, CSR_BT_SCO_DEFAULT_1P1, NULL);
                        }
                    }
                    else
                    {
                        /* failed disconnecting */
                        CsrBtHfSendHfAudioInd(instData, linkPtr->pcmSlot,0,0,0,0,0,0,
                                    CSR_BT_RESULT_CODE_HF_SUCCESS, 0xDEAD, CSR_BT_SUPPLIER_HF);
                    }
                }
                else
                {
                    CsrUint16 voiceSettings = linkPtr->audioSetupParams.theVoiceSettings;/*CSR_BT_ESCO_DEFAULT_CONNECT_VOICE_SETTINGS; */ /*CVSD*/
                    CsrUint32 txBandwidth   = linkPtr->audioSetupParams.theTxBandwidth;/*CSR_BT_ESCO_DEFAULT_CONNECT_TX_BANDWIDTH;   */ /*8000*/
                    CsrUint32 rxBandwidth   = linkPtr->audioSetupParams.theRxBandwidth;/*CSR_BT_ESCO_DEFAULT_CONNECT_RX_BANDWIDTH;   */ /*8000*/
                    CsrUint16 audioQuality  = linkPtr->audioSetupParams.theAudioQuality;/*CSR_BT_COMPLETE_SCO_DEFAULT_ACCEPT_AUDIO_QUALITY;*/ /* 0x38F */

                    linkPtr->scoHandle = HF_SCO_UNUSED;
                    if (linkPtr->outgoingAudio)
                    {
                        CsrBtHfSendHfAudioDisconnectCfm(instData, prim->eScoHandle, CSR_BT_RESULT_CODE_HF_SUCCESS, CSR_BT_SUPPLIER_HF);
                    }
                    else
                    {
                        CsrBtHfSendHfAudioDisconnectInd(instData, prim->eScoHandle, prim->reasonCode, prim->reasonSupplier);
                    }

                    linkPtr->scoConnectAcceptPending = TRUE;
                    CsrBtCmScoAcceptConnectReqSend(CSR_BT_HF_IFACEQUEUE,
                                              linkPtr->hfConnId,
                                              audioQuality,
                                              txBandwidth,
                                              rxBandwidth,
                                              linkPtr->audioSetupParams.theMaxLatency,
                                              voiceSettings,
                                              linkPtr->audioSetupParams.theReTxEffort);
#ifdef CSR_USE_DSPM                    
                    if (instData->hfDspmOpPending == DSPM_NOP)
                    {
                        CsrBtHfDspmSendDisconnectStreams(instData);
                        instData->hfDspmOpPending = instData->index;
                    }
#endif
                }
            }
        }
    }
    else /* HSP */
    { /* if sco already disconnected a race condition may have occurred so ignore the message */
        if (linkPtr->scoHandle != HF_SCO_UNUSED)
        {
            linkPtr->pcmMappingReceived = FALSE;
            if((linkPtr->lastAudioReq == FALSE) &&
               (linkPtr->audioPending == TRUE))
            {
                if (prim->status)
                {
                    linkPtr->scoHandle = HF_SCO_UNUSED;
                    linkPtr->state = Connected_s;
                    if (linkPtr->outgoingAudio)
                    {
                        CsrBtHfSendHfAudioDisconnectCfm(instData, prim->eScoHandle, CSR_BT_RESULT_CODE_HF_SUCCESS, CSR_BT_SUPPLIER_HF);
                    }
                    else
                    {
                        CsrBtHfSendHfAudioDisconnectInd(instData, prim->eScoHandle, prim->reasonCode, prim->reasonSupplier);
                    }

                    linkPtr->scoConnectAcceptPending = TRUE;

                    CsrBtCmScoAcceptConnectReqSend(CSR_BT_HF_IFACEQUEUE,
                                              linkPtr->hfConnId,
                                              linkPtr->audioSetupParams.theAudioQuality,
                                              linkPtr->audioSetupParams.theTxBandwidth,
                                              linkPtr->audioSetupParams.theRxBandwidth,
                                              linkPtr->audioSetupParams.theMaxLatency,
                                              linkPtr->audioSetupParams.theVoiceSettings,
                                              linkPtr->audioSetupParams.theReTxEffort);
                    linkPtr->audioPending = FALSE;
                }
                else
                {
                    /* sco disconnect failed but it must be disconnected so we try again */
                    CsrBtCmScoDisconnectReqSend(CSR_BT_HF_IFACEQUEUE, linkPtr->hfConnId);
                }
            }
            else
            {
                if (linkPtr->audioPending)
                {
                    /* app has requested ON while waiting for OFF */
                    if (prim->status)
                    {
                        /* Connect SCO again */
                        linkPtr->scoConnectAcceptPending = TRUE;

                        CsrBtCmScoAcceptConnectReqSend(CSR_BT_HF_IFACEQUEUE,
                                                  linkPtr->hfConnId,
                                                  linkPtr->audioSetupParams.theAudioQuality,
                                                  linkPtr->audioSetupParams.theTxBandwidth,
                                                  linkPtr->audioSetupParams.theRxBandwidth,
                                                  linkPtr->audioSetupParams.theMaxLatency,
                                                  linkPtr->audioSetupParams.theVoiceSettings,
                                                  linkPtr->audioSetupParams.theReTxEffort);
                        linkPtr->scoHandle = HF_SCO_UNUSED;
                        linkPtr->state = Connected_s;
                        sendCkpd(instData);
                    }
                    else
                    {
                        /* failed disconnecting. This means we are still connected */
                        linkPtr->audioPending = FALSE;
                        CsrBtHfSendHfAudioInd(instData, linkPtr->pcmSlot,0,0,0,0,0,0,
                                    CSR_BT_RESULT_CODE_HF_SUCCESS, 0xDEAD, CSR_BT_SUPPLIER_HF);
                    }
                }
                else
                {
                    linkPtr->scoHandle = HF_SCO_UNUSED;
                    linkPtr->state = Connected_s;
                    if (linkPtr->outgoingAudio)
                    {
                        CsrBtHfSendHfAudioDisconnectCfm(instData, prim->eScoHandle, CSR_BT_RESULT_CODE_HF_SUCCESS, CSR_BT_SUPPLIER_HF);
                    }
                    else
                    {
                        CsrBtHfSendHfAudioDisconnectInd(instData, prim->eScoHandle, prim->reasonCode, prim->reasonSupplier);
                    }
                    linkPtr->scoConnectAcceptPending = TRUE;

                    CsrBtCmScoAcceptConnectReqSend(CSR_BT_HF_IFACEQUEUE,
                                              linkPtr->hfConnId,
                                              linkPtr->audioSetupParams.theAudioQuality,
                                              linkPtr->audioSetupParams.theTxBandwidth,
                                              linkPtr->audioSetupParams.theRxBandwidth,
                                              linkPtr->audioSetupParams.theMaxLatency,
                                              linkPtr->audioSetupParams.theVoiceSettings,
                                              linkPtr->audioSetupParams.theReTxEffort);
                }
            }
        }
    }
}

/***********************************************************************************
    Handle CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM in Connected State
************************************************************************************/
void CsrBtHfConnectedStateCmScoAcceptConnectCfmHandler(HfMainInstanceData_t *instData)
{
    CsrBtCmScoAcceptConnectCfm    *prim;
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    prim    = (CsrBtCmScoAcceptConnectCfm *) instData->recvMsgP;

    linkPtr->scoConnectAcceptPending = FALSE;

    if (linkPtr->linkType == CSR_BT_HF_CONNECTION_HF)
    {
        if ((linkPtr->state == Connected_s) || (linkPtr->state == LpEnabled_s))
        { /* if sco already connected ignore the message */
            if (linkPtr->scoHandle == HF_SCO_UNUSED)
            {
                if ((linkPtr->lastAudioReq == TRUE) && (TRUE == linkPtr->audioPending))
                {
                    if(prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
                    {
                        linkPtr->pcmMappingReceived = FALSE;
                        linkPtr->scoHandle = prim->eScoHandle;
                        CsrBtHfSendHfAudioInd(instData,
                                              prim->pcmSlot,
                                              prim->linkType,
                                              prim->weSco,
                                              prim->rxPacketLength,
                                              prim->txPacketLength,
                                              prim->airMode,
                                              prim->txInterval,
                                              CSR_BT_RESULT_CODE_HF_SUCCESS,
                                              0xDEAD,
                                              CSR_BT_SUPPLIER_HF);
                        linkPtr->audioPending = FALSE;
                    }
                    else
                    {
                        CsrUint16 voiceSettings = linkPtr->audioSetupParams.theVoiceSettings;
                        CsrUint32 txBandwidth   = linkPtr->audioSetupParams.theTxBandwidth;
                        CsrUint32 rxBandwidth   = linkPtr->audioSetupParams.theRxBandwidth;
                        CsrUint16 audioQuality  = linkPtr->audioSetupParams.theAudioQuality;

                        linkPtr->scoConnectAcceptPending = TRUE;
                        CsrBtCmScoAcceptConnectReqSend(CSR_BT_HF_IFACEQUEUE,
                                                  linkPtr->hfConnId,
                                                  audioQuality,
                                                  txBandwidth,
                                                  rxBandwidth,
                                                  linkPtr->audioSetupParams.theMaxLatency,
                                                  voiceSettings,
                                                  linkPtr->audioSetupParams.theReTxEffort);
                        /* And let the app know... */
                        CsrBtHfSendHfAudioInd(instData,
                                              prim->pcmSlot,
                                              prim->linkType,
                                              prim->weSco,
                                              prim->rxPacketLength,
                                              prim->txPacketLength,
                                              prim->airMode,
                                              prim->txInterval,
                                              prim->resultCode,
                                              0xDEAD,
                                              prim->resultSupplier);
                    }
                }
                else /*if ((linkPtr->lastAudioReq == TRUE) && (TRUE == linkPtr->audioPending))*/
                {
                    linkPtr->scoHandle = prim->eScoHandle;
                    if (TRUE == linkPtr->audioPending)
                    {
                        linkPtr->pcmMappingReceived = FALSE;
                        /* app requested off while waiting for audio */
                        CsrBtCmScoDisconnectReqSend(CSR_BT_HF_IFACEQUEUE, linkPtr->hfConnId);
                    }
                    else
                    {

                        if(prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier  == CSR_BT_SUPPLIER_CM)
                        {
                            linkPtr->pcmMappingReceived = FALSE;
                            CsrBtHfSendHfAudioInd(instData,
                                                  prim->pcmSlot,
                                                  prim->linkType,
                                                  prim->weSco,
                                                  prim->rxPacketLength,
                                                  prim->txPacketLength,
                                                  prim->airMode,
                                                  prim->txInterval,
                                                  CSR_BT_RESULT_CODE_HF_SUCCESS,
                                                  0xDEAD,
                                                  CSR_BT_SUPPLIER_HF);
#ifdef CSR_USE_DSPM
                            if (linkPtr->dspmInst.operatorEncoder == HF_DSPM_DEFAULT_HANDLE)
                            {
                                CsrBtHfDspmSendCreateOperators(instData);
                            }
                            else
                            {
                                CsrHfDspmAudioConnect(instData);
                            }
                            instData->hfDspmOpPending = instData->index;
#endif
                        }
                        else
                        {
                            CsrUint16 voiceSettings = linkPtr->audioSetupParams.theVoiceSettings;
                            CsrUint32 txBandwidth   = linkPtr->audioSetupParams.theTxBandwidth;   /*8000*/
                            CsrUint32 rxBandwidth   = linkPtr->audioSetupParams.theRxBandwidth;   /*8000*/
                            CsrUint16 audioQuality  = linkPtr->audioSetupParams.theAudioQuality; /* 0x38F */

                            linkPtr->scoConnectAcceptPending = TRUE;
                            CsrBtCmScoAcceptConnectReqSend(CSR_BT_HF_IFACEQUEUE,
                                                      linkPtr->hfConnId,
                                                      audioQuality,
                                                      txBandwidth,
                                                      rxBandwidth,
                                                      linkPtr->audioSetupParams.theMaxLatency,
                                                      voiceSettings,
                                                      linkPtr->audioSetupParams.theReTxEffort);
                            /* And let the app know... */
                            CsrBtHfSendHfAudioInd(instData,
                                                  prim->pcmSlot,
                                                  prim->linkType,
                                                  prim->weSco,
                                                  prim->rxPacketLength,
                                                  prim->txPacketLength,
                                                  prim->airMode,
                                                  prim->txInterval,
                                                  prim->resultCode,
                                                  0xDEAD,
                                                  prim->resultSupplier);
                            linkPtr->pcmMappingReceived = FALSE;
                            linkPtr->scoHandle = HF_SCO_UNUSED;
                        }
                    }
                }
            }
        }
    }
    else /* HSP */
    {
        if (linkPtr->scoHandle == HF_SCO_UNUSED)
        {
            if (linkPtr->state == Connect_s)
            {
                linkPtr->scoConnectAcceptPending = FALSE;
                /* if sco already connected ignore the message */
                if(prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier  == CSR_BT_SUPPLIER_CM)
                {
                    /* Save the ScoHandle, but wait to Connect Procedure is finished before sending CSR_BT_HF_AUDIO_CONNECT_IND */
                    linkPtr->scoHandle = prim->eScoHandle;
                    linkPtr->pcmMappingReceived = FALSE;
                }
            }
            else
            {
                if (linkPtr->state == Connected_s)
                {
                    linkPtr->scoConnectAcceptPending = FALSE;

                    if ((linkPtr->lastAudioReq == TRUE) &&
                        (linkPtr->audioPending == TRUE))
                    {
                        if(prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
                        {
                            linkPtr->pcmMappingReceived = FALSE;
                            linkPtr->scoHandle = prim->eScoHandle;
                            CsrBtHfSendHfAudioInd(instData,
                                                  prim->pcmSlot,
                                                  prim->linkType,
                                                  prim->weSco,
                                                  prim->rxPacketLength,
                                                  prim->txPacketLength,
                                                  prim->airMode,
                                                  prim->txInterval,
                                                  CSR_BT_RESULT_CODE_HF_SUCCESS,
                                                  0xDEAD,
                                                  CSR_BT_SUPPLIER_HF);
                            linkPtr->audioPending = FALSE;
                        }
                        else
                        {
                            /* In case SCO fails, try again using safe parameters */
                            linkPtr->scoConnectAcceptPending = TRUE;
                            CsrBtCmScoAcceptConnectReqSend(CSR_BT_HF_IFACEQUEUE,
                                                      linkPtr->hfConnId,
                                                      linkPtr->audioSetupParams.theAudioQuality,
                                                      linkPtr->audioSetupParams.theTxBandwidth,
                                                      linkPtr->audioSetupParams.theRxBandwidth,
                                                      linkPtr->audioSetupParams.theMaxLatency,
                                                      linkPtr->audioSetupParams.theVoiceSettings,
                                                      linkPtr->audioSetupParams.theReTxEffort);
                        }
                    }
                    else
                    {
                        if (linkPtr->audioPending)
                        {
                            /* app requested audio off while trying to connect */
                            if(prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier  == CSR_BT_SUPPLIER_CM)
                            {
                                linkPtr->pcmMappingReceived = FALSE;
                                linkPtr->scoHandle = prim->eScoHandle;
                                sendCkpd(instData);
                            }
                            else
                            {
                                /* In case SCO fails, try again using safe parameters */
                                linkPtr->scoConnectAcceptPending = TRUE;
                                CsrBtCmScoAcceptConnectReqSend(CSR_BT_HF_IFACEQUEUE,
                                                          linkPtr->hfConnId,
                                                          linkPtr->audioSetupParams.theAudioQuality,
                                                          linkPtr->audioSetupParams.theTxBandwidth,
                                                          linkPtr->audioSetupParams.theRxBandwidth,
                                                          linkPtr->audioSetupParams.theMaxLatency,
                                                          linkPtr->audioSetupParams.theVoiceSettings,
                                                          linkPtr->audioSetupParams.theReTxEffort);
                            }
                        }
                        else
                        {
                            linkPtr->scoHandle = prim->eScoHandle;
                            linkPtr->pcmMappingReceived = FALSE;

                            if(prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier  == CSR_BT_SUPPLIER_CM)
                            {
                                CsrBtHfSendHfAudioInd(instData,
                                                      prim->pcmSlot,
                                                      prim->linkType,
                                                      prim->weSco,
                                                      prim->rxPacketLength,
                                                      prim->txPacketLength,
                                                      prim->airMode,
                                                      prim->txInterval,
                                                      CSR_BT_RESULT_CODE_HF_SUCCESS,
                                                      0xDEAD,
                                                      CSR_BT_SUPPLIER_HF);
                            }
                            else
                            {
                                CsrBtHfSendHfAudioInd(instData,
                                                      prim->pcmSlot,
                                                      prim->linkType,
                                                      prim->weSco,
                                                      prim->rxPacketLength,
                                                      prim->txPacketLength,
                                                      prim->airMode,
                                                      prim->txInterval,
                                                      prim->resultCode,
                                                      0xDEAD,
                                                      prim->resultSupplier);
                            }
                        }
                    }
                }
            }
        }
    }
}

/*************************************************************************************
    Common handling of downstream messages in low power mode
************************************************************************************/
void CsrBtHfLpEnabledHfXReqHandler(HfMainInstanceData_t *instData)
{
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    /* save the message until in active mode */
    CsrBtHfSaveMessage(instData);
    instData->restoreFlag = FALSE;
    switch (linkPtr->linkState)
    {
        /* low power enabled - change to active */
        case CSR_BT_LINK_STATUS_SNIFF:            /* sniff */
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
        case CSR_BT_LINK_STATUS_PARK:            /* park */
#endif
            {
                CsrBtCmRfcModeChangeReqSend(linkPtr->hfConnId,
                                    CSR_BT_LINK_STATUS_CONNECTED);
                break;
            }
            /* connection inactive - make new connection */
        case CSR_BT_LINK_STATUS_DISCONNECTED:
            {
                /*CsrBtUuid32 service = CSR_BT_HFG_PROFILE_UUID;*/
                CsrBtHfInitInstanceData(instData);
                linkPtr->state = Connect_s;
                linkPtr->oldState = Connect_s;
                linkPtr->linkRecoveryCounter = HF_LINK_RECOVER_NUMBER_OF_TIMES;
                linkPtr->serviceHandleListCount        = 0;
                startSdcFeatureSearch(instData,FALSE);
                break;
            }
        case CSR_BT_LINK_STATUS_CONNECTED:        /* connected */
        default:
            {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                CsrGeneralException("HF",linkPtr->state, 0xFF,"Unexpected link state in lp handing");/* State/Event ERROR! */
#endif
                break;
            }
    }
}

/*************************************************************************************
    Mode change result returned from exit from low power mode or remote side
    changed the link state
************************************************************************************/
void CsrBtHfLpEnabledStateCmModeChangeIndHandler(HfMainInstanceData_t *instData)
{
    CsrBtCmRfcModeChangeInd *prim;
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);
    CsrUint8 old = linkPtr->lpMode;

    prim                        = (CsrBtCmRfcModeChangeInd *) instData->recvMsgP;
    linkPtr->linkState = prim->mode;
    /* success so restore the save queue and change to connected state to handle the stored signals */
    if (prim->resultSupplier == CSR_BT_SUPPLIER_CM &&
        (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS ||
         prim->resultCode == CSR_BT_RESULT_CODE_CM_MODE_CHANGE_REQUEST_OVERRULED))
    {
        if(prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS)
        {
            linkPtr->lpMode = prim->mode;
        }

        if ( (prim->mode == CSR_BT_LINK_STATUS_CONNECTED) ||
             ((prim->resultCode == CSR_BT_RESULT_CODE_CM_MODE_CHANGE_REQUEST_OVERRULED) && (linkPtr->linkType == CSR_BT_HF_CONNECTION_HF)) ||
             ((prim->mode == CSR_BT_SNIFF_MODE) && (linkPtr->linkType == CSR_BT_HF_CONNECTION_HS)) )
        {
            linkPtr->state = Connected_s;
            CsrBtHfSendHfHouseCleaning(instData);
            if (linkPtr->linkType == CSR_BT_HF_CONNECTION_HS)
            {
                if (linkPtr->audioPending && (linkPtr->lastAudioReq == TRUE || linkPtr->scoHandle != HF_SCO_UNUSED))
                { /* app requested audio operation in lp-mode */
                    sendCkpd(instData);
                }
                else
                {
                    if (!linkPtr->scoConnectAcceptPending)
                    {
                        linkPtr->scoConnectAcceptPending = TRUE;
                        CsrBtCmScoAcceptConnectReqSend(CSR_BT_HF_IFACEQUEUE,
                                                  linkPtr->hfConnId,
                                                  linkPtr->audioSetupParams.theAudioQuality,
                                                  linkPtr->audioSetupParams.theTxBandwidth,
                                                  linkPtr->audioSetupParams.theRxBandwidth,
                                                  linkPtr->audioSetupParams.theMaxLatency,
                                                  linkPtr->audioSetupParams.theVoiceSettings,
                                                  linkPtr->audioSetupParams.theReTxEffort);
                    }
                }
            }/* else: HF connection */
            else if (!linkPtr->scoConnectAcceptPending)
            {
                linkPtr->scoConnectAcceptPending = TRUE;
                CsrBtCmScoAcceptConnectReqSend(CSR_BT_HF_IFACEQUEUE,
                                          linkPtr->hfConnId,
                                          linkPtr->audioSetupParams.theAudioQuality,
                                          linkPtr->audioSetupParams.theTxBandwidth,
                                          linkPtr->audioSetupParams.theRxBandwidth,
                                          linkPtr->audioSetupParams.theMaxLatency,
                                          linkPtr->audioSetupParams.theVoiceSettings,
                                          linkPtr->audioSetupParams.theReTxEffort);
            }
        }
        else
        {
            /* remain in lp mode */
        }
    }
    switch(prim->mode)
    {
        case CSR_BT_LINK_STATUS_CONNECTED:
            CsrBtHfSendHfStatusInd(instData, CSR_BT_LINK_STATUS_CONNECTED, old, prim->resultCode, prim->resultSupplier);
            break;
        case CSR_BT_LINK_STATUS_HOLD:
            CsrBtHfSendHfStatusInd(instData, CSR_BT_LINK_STATUS_HOLD, old, prim->resultCode, prim->resultSupplier);
            break;
        case CSR_BT_LINK_STATUS_SNIFF:
            CsrBtHfSendHfStatusInd(instData, CSR_BT_LINK_STATUS_SNIFF, old, prim->resultCode, prim->resultSupplier);
            break;
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
        case CSR_BT_LINK_STATUS_PARK:
            CsrBtHfSendHfStatusInd(instData, CSR_BT_LINK_STATUS_PARK, old, prim->resultCode, prim->resultSupplier);
            break;
#endif
        default:
            break;
    }
    linkPtr->pendingModeChange = FALSE;
}


void CsrBtHfActivateStateCmSdsRegisterCfmHandler(HfMainInstanceData_t * instData)
{
    CsrBtCmSdsRegisterCfm * prim;
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    prim = (CsrBtCmSdsRegisterCfm *) instData->recvMsgP;

    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        linkPtr->serviceRecHandle = prim->serviceRecHandle;
        linkPtr->sdsRegistered = TRUE;
        if (instData->linkRegistering == CSR_BT_HF_CONNECTION_HS)
        {
            if (linkPtr->oldState == Connect_s && linkPtr->disconnectReqReceived == FALSE)
            {
                CsrBtHfSendHfServiceConnectInd(instData, CSR_BT_RESULT_CODE_HF_CONNECT_ATTEMPT_FAILED, CSR_BT_SUPPLIER_HF);
            }
            else
            {
                CsrBtHfSendHfDisconnectInd(instData, linkPtr->disconnectResult, linkPtr->disconnectSupplier);
                linkPtr->disconnectReqReceived = FALSE;
            }
            CsrBtHfInitInstanceData(instData);
            CsrBtHfAllowConnectCheck(instData);
            if (instData->registerPending)
            {
                /* this means that HF has tried to register */
                CsrBtHfSendSdsRegisterReq(instData);
                instData->registerPending = FALSE;
            }
            else
            {
                instData->linkRegistering = CSR_BT_HF_CONNECTION_UNKNOWN;
            }
        }
        else
        {/*CSR_BT_HF_CONNECTION_HF*/
            CsrBtHfAllowConnectCheck(instData);
            if ((linkPtr->disconnectReqReceived) || (!linkPtr->disconnectReqReceived &&
                (!linkPtr->disconnectLocalTerminated || linkPtr->disconnectResult != CSR_BT_RESULT_CODE_CM_SUCCESS || linkPtr->disconnectSupplier != CSR_BT_SUPPLIER_CM) &&
                linkPtr->atSequenceState >= serviceLevel))
            {
                CsrBtHfSendHfDisconnectInd(instData, linkPtr->disconnectResult, linkPtr->disconnectSupplier);
                if (linkPtr->disconnectReqReceived)
                {
                    linkPtr->disconnectReqReceived = FALSE;
                }
            }

            if (instData->registerPending)
            {
                /* this means that HS has tried to register */
                HsSendSdsRegisterReq(instData);
                instData->registerPending = FALSE;
            }
            else
            {
                instData->linkRegistering = CSR_BT_HF_CONNECTION_UNKNOWN;
            }
            CsrBtHfInitInstanceData(instData);
        }
    }
    else
    {
        if (instData->linkRegistering == CSR_BT_HF_CONNECTION_HS)
        {
            instData->linkRegistering = CSR_BT_HF_CONNECTION_UNKNOWN;
            HsSendSdsRegisterReq(instData);
        }
        else
        {
            CsrBtHfSendSdsRegisterReq(instData);
        }
    }
}

void HsConnectStateCmSdsUnRegisterCfmHandler(HfMainInstanceData_t * instData)
{
    CsrBtCmSdsUnregisterCfm * prim;
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    prim = (CsrBtCmSdsUnregisterCfm *) instData->recvMsgP;

    instData->numberOfUnregister++;
    if ((prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM) ||
        instData->numberOfUnregister == HF_NUMBER_OF_UNREGISTER)
    {
        if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
        {
            linkPtr->sdsRegistered = FALSE;
            linkPtr->serviceRecHandle = 0;
        }
        else
        {
            linkPtr->sdsRegistered = TRUE;
        }
        if (linkPtr->oldState == Activate_s)
        {
            linkPtr->scoConnectAcceptPending = TRUE;

            CsrBtCmScoAcceptConnectReqSend(CSR_BT_HF_IFACEQUEUE,
                                      linkPtr->hfConnId,
                                      linkPtr->audioSetupParams.theAudioQuality,
                                      linkPtr->audioSetupParams.theTxBandwidth,
                                      linkPtr->audioSetupParams.theRxBandwidth,
                                      linkPtr->audioSetupParams.theMaxLatency,
                                      linkPtr->audioSetupParams.theVoiceSettings,
                                      linkPtr->audioSetupParams.theReTxEffort);
            CsrBtHfSendHfServiceConnectInd(instData, CSR_BT_RESULT_CODE_HF_SUCCESS, CSR_BT_SUPPLIER_HF);
            linkPtr->state = Connected_s;
        }
    }
    else
    {
        CsrBtCmSdsUnRegisterReqSend(CSR_BT_HF_IFACEQUEUE, linkPtr->serviceRecHandle);
    }
}

void CsrBtHfXStateCmSdsUnRegisterCfmHandler(HfMainInstanceData_t * instData)
{
    CsrBtCmSdsUnregisterCfm * prim;
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    prim = (CsrBtCmSdsUnregisterCfm *) instData->recvMsgP;

    instData->numberOfUnregister++;

    if ((prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM) ||
        instData->numberOfUnregister == HF_NUMBER_OF_UNREGISTER)
    {
        if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
        {
            linkPtr->sdsRegistered = FALSE;
            linkPtr->serviceRecHandle = 0;
        }
        else
        {
            linkPtr->sdsRegistered = TRUE;
        }
    }
    else
    {
        CsrBtCmSdsUnRegisterReqSend(CSR_BT_HF_IFACEQUEUE, linkPtr->serviceRecHandle);
    }
}


