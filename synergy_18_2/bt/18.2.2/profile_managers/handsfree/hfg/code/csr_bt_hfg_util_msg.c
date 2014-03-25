/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_env_prim.h"
#include "csr_bt_result.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_util.h"
#include "csr_exceptionhandler.h"
#include "csr_bt_hfg_prim.h"
#include "csr_bt_hfg_main.h"
#include "csr_bt_hfg_proto.h"
#include "csr_bccmd_lib.h"

void CsrBtHfgMessagePut(CsrSchedQid phandle, void *msg)
{
    CsrSchedMessagePut(phandle, CSR_BT_HFG_PRIM, msg);
}

/* Local: Return application handle given local instance pointer */
static CsrSchedQid csrBtHfgAppHandle(HfgInstance_t *inst)
{
    return CsrBtHfgGetMainInstance(inst)->appHandle;
}

/* Internal: Send house-cleaning to main instance */
void CsrBtHfgMainSendHfgHouseCleaning(HfgMainInstance_t *inst)
{
    CsrBtHfgHouseCleaning *prim;

    prim = (CsrBtHfgHouseCleaning*)CsrPmemAlloc(sizeof(CsrBtHfgHouseCleaning));
    prim->type = CSR_BT_HFG_HOUSE_CLEANING;
    prim->serverChannel = CSR_BT_NO_SERVER;

    inst->restoreFlag = TRUE;
    CsrBtHfgMessagePut(CSR_BT_HFG_IFACEQUEUE, prim);
}

/* Internal: Send house-cleaning for connection instance */
void CsrBtHfgSendHfgHouseCleaning(HfgInstance_t *inst)
{
    CsrBtHfgHouseCleaning *prim;

    prim = (CsrBtHfgHouseCleaning*)CsrPmemAlloc(sizeof(CsrBtHfgHouseCleaning));
    prim->type = CSR_BT_HFG_HOUSE_CLEANING;
    prim->serverChannel = inst->serverChannel;

    inst->restoreFlag = TRUE;
    CsrBtHfgMessagePut(CSR_BT_HFG_IFACEQUEUE, prim);
}

/* Upstream: Incoming security level confirm */
void CsrBtHfgSendCmSecurityInCfm(CsrSchedQid appHandle, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    CsrBtHfgSecurityInCfm *prim;

    if(appHandle != CSR_SCHED_QID_INVALID)
    {
        prim = (CsrBtHfgSecurityInCfm*)CsrPmemAlloc(sizeof(CsrBtHfgSecurityInCfm));
        prim->type = CSR_BT_HFG_SECURITY_IN_CFM;
        prim->resultCode     = resultCode;
        prim->resultSupplier = resultSupplier;
        CsrBtHfgMessagePut(appHandle, prim);
    }
}

/* Upstream: Outgoing security level confirm */
void CsrBtHfgSendCmSecurityOutCfm(CsrSchedQid appHandle, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    CsrBtHfgSecurityOutCfm *prim;
    if(appHandle != CSR_SCHED_QID_INVALID)
    {
        prim = (CsrBtHfgSecurityOutCfm*)CsrPmemAlloc(sizeof(CsrBtHfgSecurityOutCfm));
        prim->type = CSR_BT_HFG_SECURITY_OUT_CFM;
        prim->resultCode     = resultCode;
        prim->resultSupplier = resultSupplier;
        CsrBtHfgMessagePut(appHandle, prim);
    }
}

/* Upstream: Send deactivated confirm */
void CsrBtHfgMainSendHfgDeactivateCfm(HfgMainInstance_t *inst)
{
    CsrBtHfgDeactivateCfm *prim;

    if(inst->appHandle != CSR_SCHED_QID_INVALID)
    {
        prim = (CsrBtHfgDeactivateCfm*)CsrPmemAlloc(sizeof(CsrBtHfgDeactivateCfm));
        prim->type = CSR_BT_HFG_DEACTIVATE_CFM;
        CsrBtHfgMessagePut(inst->appHandle, prim);
    }
}

/* Upstream: Conditional LP status sending */
void CsrBtHfgSendHfgStatusLpInd(HfgInstance_t *inst,
                           CsrUint8 current,
                           CsrUint8 old,
                           CsrUint8 wanted,
                           CsrBool remote,
                           CsrBtResultCode resultCode,
                           CsrBtSupplier resultSupplier)
{
    CsrBtHfgStatusLpInd *prim;
    CsrSchedQid app;

    app = csrBtHfgAppHandle(inst);
    if(app != CSR_SCHED_QID_INVALID)
    {
        prim = (CsrBtHfgStatusLpInd*)CsrPmemAlloc(sizeof(CsrBtHfgStatusLpInd));
        prim->type = CSR_BT_HFG_STATUS_LP_IND;
        prim->connectionId = inst->hfgConnId;
        prim->currentMode = current;
        prim->oldMode = old;
        prim->wantedMode = wanted;
        prim->remoteReq = remote;
        prim->resultCode     = resultCode;
        prim->resultSupplier = resultSupplier;
        CsrBtHfgMessagePut(app, prim);
    }
}

/* Upstream: Microphone gain indication */
void CsrBtHfgSendHfgMicGainInd(HfgInstance_t *inst, CsrUint8 returnValue)
{
    CsrBtHfgMicGainInd *prim;
    CsrSchedQid app;

    app = csrBtHfgAppHandle(inst);
    if(app != CSR_SCHED_QID_INVALID)
    {
        prim = (CsrBtHfgMicGainInd *)CsrPmemAlloc(sizeof(CsrBtHfgMicGainInd));
        prim->type = CSR_BT_HFG_MIC_GAIN_IND;
        prim->gain = returnValue;
        prim->connectionId = inst->hfgConnId;
        CsrBtHfgMessagePut(app, prim);
    }
}

/* Upstream: Speaker gain indication */
void CsrBtHfgSendHfgSpeakerGainInd(HfgInstance_t *inst, CsrUint8 returnValue)
{
    CsrBtHfgSpeakerGainInd *prim;
    CsrSchedQid app;

    app = csrBtHfgAppHandle(inst);
    if(app != CSR_SCHED_QID_INVALID)
    {
        prim = (CsrBtHfgSpeakerGainInd *)CsrPmemAlloc(sizeof(CsrBtHfgSpeakerGainInd));
        prim->type = CSR_BT_HFG_SPEAKER_GAIN_IND;
        prim->gain = returnValue;
        prim->connectionId = inst->hfgConnId;
        CsrBtHfgMessagePut(app, prim);
    }
}

/* Upstream: Send service level connection indication */
void CsrBtHfgSendHfgServiceConnectInd(HfgInstance_t *inst, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    CsrBtHfgServiceConnectInd *prim;
    CsrSchedQid app;
    
    if (inst->conGuardTimer != 0)
    {
        void *mv;
        CsrUint16 mi;
        
        CsrSchedTimerCancel(inst->conGuardTimer ,&mi, &mv);
        inst->conGuardTimer = 0;
    }
    app = csrBtHfgAppHandle(inst);
    if(app != CSR_SCHED_QID_INVALID)
    {
        prim = (CsrBtHfgServiceConnectInd *)CsrPmemAlloc(sizeof(CsrBtHfgServiceConnectInd));
        prim->type = CSR_BT_HFG_SERVICE_CONNECT_IND;
        prim->connectionId = inst->hfgConnId;
        prim->connectionType = inst->linkType;
        prim->deviceAddr = inst->address;
        prim->supportedFeatures = inst->remSupFeatures;
        prim->remoteVersion = inst->remoteVersion;
        prim->resultCode = resultCode;
        prim->resultSupplier = resultSupplier;
        prim->btConnId = CSR_BT_CONN_ID_INVALID;

        if(resultCode != CSR_BT_RESULT_CODE_HFG_SUCCESS ||
           resultSupplier != CSR_BT_SUPPLIER_HFG)
        {
            prim->connectionType = CSR_BT_HFG_CONNECTION_UNKNOWN;
            prim->supportedFeatures = 0;
            prim->serviceName = NULL;
        }
        else
        {
            prim->serviceName = CsrPmemAlloc(CsrStrLen((char*)inst->serviceName)+1);
            CsrStrCpy((char*)prim->serviceName,
                    (char*)inst->serviceName);
        }

        CsrBtHfgMessagePut(app, prim);
    }
}

/* Upstream: Send disconnect indication */
void CsrBtHfgSendHfgDisconnectInd(HfgInstance_t *inst, CsrBool localTerminated, CsrBtResultCode reasonCode, CsrBtSupplier reasonSupplier)
{
    CsrBtHfgDisconnectInd *prim;
    CsrSchedQid app;

    app = csrBtHfgAppHandle(inst);
    if(app != CSR_SCHED_QID_INVALID)
    {
        prim = (CsrBtHfgDisconnectInd *)CsrPmemAlloc(sizeof(CsrBtHfgDisconnectInd));
        prim->type = CSR_BT_HFG_DISCONNECT_IND;
        prim->deviceAddr = inst->address;
        prim->connectionId = inst->hfgConnId;
        prim->reasonCode  = reasonCode;
        prim->reasonSupplier  = reasonSupplier;
        prim->localTerminated = localTerminated;
        CsrBtHfgMessagePut(app, prim);
    }
}

/* Upstream: Send answered */
void CsrBtHfgSendHfgAnswerInd(HfgInstance_t *inst)
{
    CsrBtHfgAnswerInd *prim;
    CsrSchedQid app;

    app = csrBtHfgAppHandle(inst);
    if(app != CSR_SCHED_QID_INVALID)
    {
        prim = (CsrBtHfgAnswerInd *)CsrPmemAlloc(sizeof(CsrBtHfgAnswerInd));
        prim->type = CSR_BT_HFG_ANSWER_IND;
        prim->connectionId = inst->hfgConnId;
        CsrBtHfgMessagePut(app, prim);
    }
}

/* Upstream: Send answered */
void CsrBtHfgSendHfgRejectInd(HfgInstance_t *inst)
{
    CsrBtHfgRejectInd *prim;
    CsrSchedQid app;

    app = csrBtHfgAppHandle(inst);
    if(app != CSR_SCHED_QID_INVALID)
    {
        prim = (CsrBtHfgRejectInd *)CsrPmemAlloc(sizeof(CsrBtHfgRejectInd));
        prim->type = CSR_BT_HFG_REJECT_IND;
        prim->connectionId = inst->hfgConnId;
        CsrBtHfgMessagePut(app, prim);
    }
}

/* Voice activation toggle */
void CsrBtHfgSendHfgVoiceRecogInd(HfgInstance_t *inst, CsrBool enable)
{
    CsrBtHfgVoiceRecogInd *prim;
    CsrSchedQid app;

    app = csrBtHfgAppHandle(inst);
    if(app != CSR_SCHED_QID_INVALID)
    {
        prim = (CsrBtHfgVoiceRecogInd*)CsrPmemAlloc(sizeof(CsrBtHfgVoiceRecogInd));
        prim->type = CSR_BT_HFG_VOICE_RECOG_IND;
        prim->connectionId = inst->hfgConnId;
        prim->bvra = enable;
        CsrBtHfgMessagePut(app, prim);
    }
}

/* Dial indication for number/memory/redial */
void CsrBtHfgSendHfgDialInd(HfgInstance_t *inst, CsrBtHfgDialCommand cmd, CsrCharString *num)
{
    CsrBtHfgDialInd *prim;
    CsrSchedQid app;

    app = csrBtHfgAppHandle(inst);
    if(app != CSR_SCHED_QID_INVALID)
    {
        prim = (CsrBtHfgDialInd*)CsrPmemAlloc(sizeof(CsrBtHfgDialInd));
        prim->type = CSR_BT_HFG_DIAL_IND;
        prim->connectionId = inst->hfgConnId;
        prim->command = cmd;
        prim->number = num;
        CsrBtHfgMessagePut(app, prim);
    }
}

/* Call handling update (chld/btrh) */
void CsrBtHfgSendHfgCallHandlingInd(HfgInstance_t *inst, CsrUint8 cmd, CsrUint8 idx)
{
    CsrBtHfgCallHandlingInd *prim;
    CsrSchedQid app;

    app = csrBtHfgAppHandle(inst);
    if(app != CSR_SCHED_QID_INVALID)
    {
        prim = (CsrBtHfgCallHandlingInd*)CsrPmemAlloc(sizeof(CsrBtHfgCallHandlingInd));
        prim->type = CSR_BT_HFG_CALL_HANDLING_IND;
        prim->connectionId = inst->hfgConnId;
        prim->value = cmd;
        prim->index = idx;
        CsrBtHfgMessagePut(app, prim);
    }
}

/* Noise reduction and echo cancellation indicator send */
void CsrBtHfgSendHfgNoiseEchoInd(HfgInstance_t *inst, CsrBool nrec)
{
    CsrBtHfgNoiseEchoInd *prim;
    CsrSchedQid app;

    app = csrBtHfgAppHandle(inst);
    if(app != CSR_SCHED_QID_INVALID)
    {
        prim = (CsrBtHfgNoiseEchoInd*)CsrPmemAlloc(sizeof(CsrBtHfgNoiseEchoInd));
        prim->type = CSR_BT_HFG_NOISE_ECHO_IND;
        prim->connectionId = inst->hfgConnId;
        prim->nrec = nrec;
        CsrBtHfgMessagePut(app, prim);
    }
}

/* DTMF tone generation indication */
void CsrBtHfgSendHfgGenerateDtmfInd(HfgInstance_t *inst, char dtmf)
{
    CsrBtHfgGenerateDtmfInd *prim;
    CsrSchedQid app;

    app = csrBtHfgAppHandle(inst);
    if(app != CSR_SCHED_QID_INVALID)
    {
        prim = (CsrBtHfgGenerateDtmfInd*)CsrPmemAlloc(sizeof(CsrBtHfgGenerateDtmfInd));
        prim->type = CSR_BT_HFG_GENERATE_DTMF_IND;
        prim->connectionId = inst->hfgConnId;
        prim->dtmf = dtmf;
        CsrBtHfgMessagePut(app, prim);
    }
}

/* Indication to request call list */
void CsrBtHfgSendHfgCallListInd(HfgInstance_t *inst)
{
    CsrBtHfgCallListInd *prim;
    CsrSchedQid app;

    app = csrBtHfgAppHandle(inst);
    if(app != CSR_SCHED_QID_INVALID)
    {
        prim = (CsrBtHfgCallListInd*)CsrPmemAlloc(sizeof(CsrBtHfgCallListInd));
        prim->type = CSR_BT_HFG_CALL_LIST_IND;
        prim->connectionId = inst->hfgConnId;
        CsrBtHfgMessagePut(app, prim);
    }
}

/* Indication to request subscriber numbers */
void CsrBtHfgSendHfgSubscriberNumberInd(HfgInstance_t *inst)
{
    CsrBtHfgSubscriberNumberInd *prim;
    CsrSchedQid app;

    app = csrBtHfgAppHandle(inst);
    if(app != CSR_SCHED_QID_INVALID)
    {
        prim = (CsrBtHfgSubscriberNumberInd*)CsrPmemAlloc(sizeof(CsrBtHfgSubscriberNumberInd));
        prim->type = CSR_BT_HFG_SUBSCRIBER_NUMBER_IND;
        prim->connectionId = inst->hfgConnId;
        CsrBtHfgMessagePut(app, prim);
    }
}

/* Send operator information request */
void CsrBtHfgSendHfgOperatorInd(HfgInstance_t *inst)
{
    CsrBtHfgOperatorInd *prim;
    CsrSchedQid app;

    app = csrBtHfgAppHandle(inst);
    if(app != CSR_SCHED_QID_INVALID)
    {
        prim = (CsrBtHfgOperatorInd*)CsrPmemAlloc(sizeof(CsrBtHfgOperatorInd));
        prim->type = CSR_BT_HFG_OPERATOR_IND;
        prim->connectionId = inst->hfgConnId;
        CsrBtHfgMessagePut(app, prim);
    }
}

/* Send Bluetooth Input request indication */
void CsrBtHfgSendHfgBtInputInd(HfgInstance_t *inst, CsrUint8 req)
{
    CsrBtHfgBtInputInd *prim;
    CsrSchedQid app;

    app = csrBtHfgAppHandle(inst);
    if(app != CSR_SCHED_QID_INVALID)
    {
        prim = (CsrBtHfgBtInputInd*)CsrPmemAlloc(sizeof(CsrBtHfgBtInputInd));
        prim->type = CSR_BT_HFG_BT_INPUT_IND;
        prim->connectionId = inst->hfgConnId;
        prim->request = req;
        CsrBtHfgMessagePut(app, prim);
    }
}

/* Upstream: Send AT command indication */
void CsrBtHfgSendHfgAtCmdInd(HfgInstance_t *inst,
                        CsrBool cme)
{
    CsrBtHfgAtCmdInd *prim;
    CsrSchedQid app;

    app = csrBtHfgAppHandle(inst);
    if(app != CSR_SCHED_QID_INVALID)
    {
        prim = (CsrBtHfgAtCmdInd *)CsrPmemAlloc(sizeof(CsrBtHfgAtCmdInd));
        prim->type = CSR_BT_HFG_AT_CMD_IND;
        prim->connectionId = inst->hfgConnId;
        prim->cmeeEnabled = cme;

        /* Transfer ownership of received AT command to app */
        prim->command = (CsrCharString *)(inst->atCmd);
        prim->command[inst->atLen-1] = '\0';
        inst->atCmd = NULL;
        inst->atLen = 0;

        CsrBtHfgMessagePut(app, prim);
    }
}

/* Upstream: Send ring completed indication */
void CsrBtHfgSendHfgRingCfm(HfgInstance_t *inst)
{
    CsrBtHfgRingCfm *prim;
    CsrSchedQid app;

    app = csrBtHfgAppHandle(inst);
    if(app != CSR_SCHED_QID_INVALID)
    {
        prim = (CsrBtHfgRingCfm *) CsrPmemAlloc(sizeof(CsrBtHfgRingCfm));
        prim->type = CSR_BT_HFG_RING_CFM;
        prim->connectionId = inst->hfgConnId;
        CsrBtHfgMessagePut(app, prim);
    }
}

#ifdef CSR_BT_INSTALL_HFG_C2C
/* Upstream: C2C supported features indication */
void CsrBtHfgSendHfgC2cSfInd(HfgInstance_t *inst)
{
    CsrBtHfgC2cSfInd *prim;
    CsrSchedQid app;

    app = csrBtHfgAppHandle(inst);
    if(app != CSR_SCHED_QID_INVALID)
    {
        prim = (CsrBtHfgC2cSfInd *) CsrPmemAlloc(sizeof(CsrBtHfgC2cSfInd));
        prim->type = CSR_BT_HFG_C2C_SF_IND;
        prim->connectionId = inst->hfgConnId;
        prim->indicators = (CsrUint8*)CsrPmemAlloc(CSR_BT_C2C_NUMBER_OF_INDICATORS);
        prim->indicatorsLength = CSR_BT_C2C_NUMBER_OF_INDICATORS;
        CsrMemCpy(prim->indicators,
               inst->ind.c2c,
               CSR_BT_C2C_NUMBER_OF_INDICATORS);
        CsrBtHfgMessagePut(app, prim);
    }
}

/* Upstream: C2C battery level changed */
void CsrBtHfgSendHfgC2cBatteryInd(HfgInstance_t *inst, CsrUint8 c2cBatteryLevel)
{
    CsrBtHfgC2cBatteryInd *prim;
    CsrSchedQid app;

    app = csrBtHfgAppHandle(inst);
    if(app != CSR_SCHED_QID_INVALID)
    {
        prim = (CsrBtHfgC2cBatteryInd *) CsrPmemAlloc(sizeof(CsrBtHfgC2cBatteryInd));
        prim->type = CSR_BT_HFG_C2C_BATTERY_IND;
        prim->connectionId = inst->hfgConnId;
        prim->batteryLevel = c2cBatteryLevel;
        CsrBtHfgMessagePut(app, prim);
    }
}

/* Upstream: C2C power mode changed */
void CsrBtHfgSendHfgC2cPowerInd(HfgInstance_t *inst, CsrUint8 c2cPowerStatus)
{
    CsrBtHfgC2cPowerInd *prim;
    CsrSchedQid app;

    app = csrBtHfgAppHandle(inst);
    if(app != CSR_SCHED_QID_INVALID)
    {
        prim = (CsrBtHfgC2cPowerInd *) CsrPmemAlloc(sizeof(CsrBtHfgC2cPowerInd));
        prim->type = CSR_BT_HFG_C2C_POWER_IND;
        prim->connectionId = inst->hfgConnId;
        prim->powerStatus = c2cPowerStatus;
        CsrBtHfgMessagePut(app, prim);
    }
}

/* Upstream: C2C request SMS */
void CsrBtHfgSendHfgC2cSmsGetInd(HfgInstance_t *inst, CsrUint16 c2cSmsIndex)
{
    CsrBtHfgC2cSmsGetInd *prim;
    CsrSchedQid app;

    app = csrBtHfgAppHandle(inst);
    if(app != CSR_SCHED_QID_INVALID)
    {
        prim = (CsrBtHfgC2cSmsGetInd *) CsrPmemAlloc(sizeof(CsrBtHfgC2cSmsGetInd));
        prim->type = CSR_BT_HFG_C2C_SMS_GET_IND;
        prim->connectionId = inst->hfgConnId;
        prim->smsIndex = c2cSmsIndex;
        CsrBtHfgMessagePut(app, prim);
    }
}
#endif /* CSR_BT_INSTALL_HFG_C2C */

/* Upstream: Send audio disconnnect indication */
void CsrBtHfgSendHfgAudioDisconnectInd(HfgInstance_t *inst,
                                    CsrUint16 scoHandle,
                                    CsrBtReasonCode reasonCode,
                                    CsrBtSupplier reasonSupplier)
{
    CsrBtHfgAudioDisconnectInd *prim;
    CsrSchedQid app;

    app = csrBtHfgAppHandle(inst);
    if(app != CSR_SCHED_QID_INVALID)
    {
        prim = CsrPmemAlloc(sizeof(CsrBtHfgAudioDisconnectInd));
        prim->type = CSR_BT_HFG_AUDIO_DISCONNECT_IND;
        prim->connectionId = inst->hfgConnId;
        prim->scoHandle = scoHandle;
        prim->reasonCode = reasonCode;
        prim->reasonSupplier = reasonSupplier;
        CsrBtHfgMessagePut(app, prim);
    }
}

/* Upstream: Send audio disconnect confirm */
void CsrBtHfgSendHfgAudioDisconnectCfm(HfgInstance_t *inst,
                                    CsrUint16 scoHandle,
                                    CsrBtResultCode resultCode,
                                    CsrBtSupplier resultSupplier)
{
    CsrBtHfgAudioDisconnectCfm *prim;
    CsrSchedQid app;

    app = csrBtHfgAppHandle(inst);
    if(app != CSR_SCHED_QID_INVALID)
    {
        prim = CsrPmemAlloc(sizeof(CsrBtHfgAudioDisconnectCfm));
        prim->type = CSR_BT_HFG_AUDIO_DISCONNECT_CFM;
        prim->connectionId = inst->hfgConnId;
        prim->scoHandle = scoHandle;
        prim->resultCode = resultCode;
        prim->resultSupplier = resultSupplier;
        CsrBtHfgMessagePut(app, prim);
    }
}

/* Upstream: Send extended audio indication */
void CsrBtHfgSendHfgExtendedAudioInd(HfgInstance_t *inst,
                                    CsrUint16 sco,
                                    CsrUint8 pcm,
                                    CsrUint8              theScoLinkType,
                                    CsrUint8              weSco,
                                    CsrUint16             rxPacketLength,
                                    CsrUint16             txPacketLength,
                                    CsrUint8              airMode,
                                    CsrUint8              txInterval,
                                    CsrBtHfgAudioCodecType  audioCodec,
                                    CsrBtHfgAudioSampleRateType sampleRate,
                                    CsrBtResultCode resultCode,
                                    CsrBtSupplier resultSupplier)
{

    CsrBtHfgAudioConnectInd *prim;
    CsrSchedQid app;

    app = csrBtHfgAppHandle(inst);
    if(app != CSR_SCHED_QID_INVALID)
    {
        prim = CsrPmemAlloc(sizeof(CsrBtHfgAudioConnectInd));

        if(inst->pendingScoConfirm)
        {
            inst->pendingScoConfirm = FALSE;
            prim->type = CSR_BT_HFG_AUDIO_CONNECT_CFM;
        }
        else
        {
            prim->type = CSR_BT_HFG_AUDIO_CONNECT_IND;
        }
        prim->connectionId = inst->hfgConnId;
        prim->scoHandle = sco;
        prim->pcmSlot = pcm;
        prim->linkType = theScoLinkType;
        prim->txInterval = txInterval;
        prim->weSco = weSco;
        prim->rxPacketLength = rxPacketLength;
        prim->txPacketLength = txPacketLength;
        prim->airMode = airMode;
        prim->resultCode = resultCode;
        prim->resultSupplier = resultSupplier;

        if (audioCodec <= CSR_BT_C2C_ADPCM_OFF_VALUE)
        { /* 0  is not a legal value, but the audioCodec value 0 is used to make sure no
            codec negotiation takes place. Make sure to deliver the proper value to the application!*/
            prim->codecUsed = CSR_BT_C2C_ADPCM_OFF_VALUE; /* CVSD audio */
        }
        else
        {
            prim->codecUsed = audioCodec;
        }
        if (sampleRate == 0)
        {/* 0  is not a legal value, but the sampleRate value 0 is used to make sure no
            codec negotiation takes place. Make sure to deliver the proper value to the application!*/
            prim->sampleRate = CSR_BT_C2C_SAMPLE_8KHZ_VALUE;
        }
        else
        {
            prim->sampleRate = sampleRate;
        }
        CsrBtHfgMessagePut(app, prim);
    }
}

/* Upstream: Send audio config indication */
void CsrBtHfgSendScoHfgStatusAudioInd(HfgInstance_t *inst,
                                 CsrBtHfgAudioScoStatus *set)
{
    if (CsrBtHfgGetMainInstance(inst)->hfgConfig & CSR_BT_HFG_CNF_AUDIO_STATUS)
    {
        CsrBtHfgStatusAudioInd *prim;
        CsrSchedQid app;

        app = csrBtHfgAppHandle(inst);
        if(app != CSR_SCHED_QID_INVALID)
        {
            prim = CsrPmemAlloc(sizeof(CsrBtHfgStatusAudioInd));
            prim->type = CSR_BT_HFG_STATUS_AUDIO_IND;
            prim->connectionId = inst->hfgConnId;
            prim->audioType = CSR_BT_HFG_AUDIO_SCO_STATUS;
            prim->audioSetting = (CsrUint8 *)set;
            prim->audioSettingLen = sizeof(CsrBtHfgAudioScoStatus);
            CsrBtHfgMessagePut(app, prim);
        }
    }
}

#ifdef CSR_BT_INSTALL_HFG_C2C
/* Upstream: Send audio codec config indication */
void CsrBtHfgSendAudioCodecHfgStatusAudioInd(HfgInstance_t *inst,
                                 CsrBtHfgAudioCodecStatus *set)
{
    if (CsrBtHfgGetMainInstance(inst)->hfgConfig & CSR_BT_HFG_CNF_AUDIO_STATUS)
    {
        CsrBtHfgStatusAudioInd *prim;
        CsrSchedQid app;

        app = csrBtHfgAppHandle(inst);
        if(app != CSR_SCHED_QID_INVALID)
        {
            prim = CsrPmemAlloc(sizeof(CsrBtHfgStatusAudioInd));
            prim->type = CSR_BT_HFG_STATUS_AUDIO_IND;
            prim->connectionId = inst->hfgConnId;
            prim->audioType = CSR_BT_HFG_AUDIO_CODEC_STATUS;
            prim->audioSetting = (CsrUint8 *)set;
            prim->audioSettingLen = sizeof(CsrBtHfgAudioScoStatus);
            CsrBtHfgMessagePut(app, prim);
        }
        else
        {
            CsrPmemFree(set);

        }
    }
    else
    {
        CsrPmemFree(set);
    }
}
#endif

/* Upstream: Send map-sco-pcm to app */
void CsrBtHfgSendHfgMapScoPcmInd(HfgInstance_t *inst)
{
    CsrBtHfgAudioAcceptConnectInd *prim;
    CsrBtCmMapScoPcmInd *msg = inst->msg;
    CsrSchedQid app;

    app = csrBtHfgAppHandle(inst);
    if(app != CSR_SCHED_QID_INVALID)
    {
        prim = (CsrBtHfgAudioAcceptConnectInd*)CsrPmemAlloc(sizeof(CsrBtHfgAudioAcceptConnectInd));
        prim->type = CSR_BT_HFG_AUDIO_ACCEPT_CONNECT_IND;
        prim->connectionId = inst->hfgConnId;
        prim->linkType = msg->linkType;
        CsrBtHfgMessagePut(app, prim);
    }
}



/* Upstream: Send manual-cind-indication to app */
void CsrBtHfgSendHfgManualIndicatorInd(HfgInstance_t *inst)
{
    CsrBtHfgManualIndicatorInd *prim;
    CsrSchedQid app;

    app = csrBtHfgAppHandle(inst);
    if(app != CSR_SCHED_QID_INVALID)
    {
        prim = (CsrBtHfgManualIndicatorInd*)CsrPmemAlloc(sizeof(CsrBtHfgManualIndicatorInd));
        prim->type = CSR_BT_HFG_MANUAL_INDICATOR_IND;
        prim->connectionId = inst->hfgConnId;
        CsrBtHfgMessagePut(app, prim);
    }

}

/* Downstream: Send data to CM */
void CsrBtHfgSendCmDataReq(HfgInstance_t *inst,
                      CsrUint16 len,
                      CsrUint8 *data)
{
    CsrUint16 dataSent;
    CsrBool save;

    dataSent = 0;
    while(dataSent < len)
    {
        CsrBtCmDataReq *cmPrim;

        cmPrim = CsrPmemAlloc(sizeof(CsrBtCmDataReq));
        cmPrim->type = CSR_BT_CM_DATA_REQ;
        cmPrim->btConnId = inst->hfgConnId;

        if((len - dataSent) <= inst->maxFrameSize)
        {
            cmPrim->payloadLength = (len - dataSent);
        }
        else
        {
            cmPrim->payloadLength = inst->maxFrameSize;
        }
        cmPrim->payload = CsrPmemAlloc(cmPrim->payloadLength);
        CsrMemCpy(cmPrim->payload, &(data[dataSent]), cmPrim->payloadLength);
        dataSent = dataSent + cmPrim->payloadLength;;
        save = TRUE;

#ifdef CSR_BT_INSTALL_CM_PARK_MODE
        if(inst->lpMode == CSR_BT_LINK_STATUS_PARK)
        {
            /* If we're in park-mode, we must kick link back into
             * active */
            inst->pendingModeChange = TRUE;
            inst->lpWantedMode = CSR_BT_LINK_STATUS_SNIFF;
            CsrBtCmRfcModeChangeReqSend(inst->hfgConnId,
                                CSR_BT_LINK_STATUS_CONNECTED);
        }
        else if(inst->cmTxReady)
#else
        if(inst->cmTxReady)
#endif
        {
            /* Ready to transmit */
            CsrSchedMessagePut(CSR_BT_CM_IFACEQUEUE, CSR_BT_CM_PRIM, cmPrim);
            inst->cmTxReady = FALSE;
            save = FALSE;
        }

        /* Save it on queue due to flow control or park mode? */
        if(save)
        {
            CsrMessageQueuePush(&inst->cmTxQueue,
                        CSR_BT_CM_PRIM,
                        cmPrim);
        }
    }
    CsrPmemFree(data);
}

/* Downstream: Send control parameter request */
void CsrBtHfgSendCmControlReq(HfgInstance_t *inst)
{
    CsrUint8 modemStatus;
    modemStatus = CsrBtMapSendingControlSignal(CSR_BT_MODEM_CTS_MASK | CSR_BT_MODEM_DSR_MASK, CSR_BT_DCE);
    CsrBtCmControlReqSend(inst->hfgConnId,
                     modemStatus,
                     0);
}

#ifdef CSR_BT_INSTALL_HFG_C2C
/* change the audio codec */
void CsrBtHfgSetC2CAudioCodec(CsrUint16 scoHandle, CsrUint8 value, CsrUint16 seqNo)
{
    CsrUint8  *payload    = (CsrUint8 *) CsrPmemAlloc(4);
    CsrUint8  *tmpPayload = payload;

    CSR_ADD_UINT16_TO_XAP(tmpPayload, scoHandle);

    switch(value)
    {
        case CSR_BT_C2C_ADPCM_2BIT_VALUE:  /* 2bit/sample */
            CSR_ADD_UINT16_TO_XAP(tmpPayload, 0x0001);
            break;
        case CSR_BT_C2C_ADPCM_4BIT_VALUE:  /* 4bit/sample */
            CSR_ADD_UINT16_TO_XAP(tmpPayload, 0x0000);
            break;
        default:
            CSR_ADD_UINT16_TO_XAP(tmpPayload, 0x0000);
            break;
    }
    CsrBccmdWriteReqSend(CSR_BT_HFG_IFACEQUEUE, 0x5037, seqNo, 4, payload);
}

/* Internal: check if ADPCM is supported by chip */
void CsrBtHfgMainGetC2CAdpcmSupported(CsrUint16 seqNo)
{
    CsrBccmdReadReqSend(CSR_BT_HFG_IFACEQUEUE, 0x2859, seqNo, 0, NULL);
}
#endif

#ifdef CSR_BT_INSTALL_HFG_CONFIG_ATCMD_HANDLING
void CsrBtHfgSendHfgConfigAtCmdHandlingCfm(CsrSchedQid appHandle,
                                      CsrBtHfgAtCmdResultCodes result,
                                      CsrUint8 *bitwiseIndicators,
                                      CsrUint8 length)
{
    CsrBtHfgConfigAtcmdHandlingCfm *prim;
    prim = CsrPmemAlloc(sizeof(CsrBtHfgConfigAtcmdHandlingCfm));

    prim->type = CSR_BT_HFG_CONFIG_ATCMD_HANDLING_CFM;
    prim->result = result;
    prim->bitwiseIndicatorsLength = length;
    prim->bitwiseIndicators = bitwiseIndicators;

    CsrBtHfgMessagePut(appHandle, prim);
}
#endif

#ifdef CSR_BT_INSTALL_HFG_CONFIG_SINGLE_ATCMD
void CsrBtHfgSendHfgConfigSingleAtCmdCfm(CsrSchedQid appHandle,
                                    CsrBtHfgAtCmdResultCodes result)
{
    CsrBtHfgConfigSingleAtcmdCfm *prim;
    prim = CsrPmemAlloc(sizeof(CsrBtHfgConfigSingleAtcmdCfm));

    prim->type = CSR_BT_HFG_CONFIG_SINGLE_ATCMD_CFM;
    prim->result = result;

    CsrBtHfgMessagePut(appHandle, prim);
}

void CsrBtHfgSendHfgLocalAdpcmSupportInd(HfgMainInstance_t *inst,CsrBool result)
{
    CsrBtHfgGetC2cAdpcmLocalSupportedInd *prim;

    prim = CsrPmemAlloc(sizeof(CsrBtHfgGetC2cAdpcmLocalSupportedInd));
    prim->type = CSR_BT_HFG_GET_C2C_ADPCM_LOCAL_SUPPORTED_IND;
    prim-> result = result;
    CsrBtHfgMessagePut(inst->appHandle, prim);
}
#endif

void CsrBtHfgSendHfgDeregisterTimeCfm(CsrSchedQid appHandle,CsrBtHfgAtCmdResultCodes result)
{
    CsrBtHfgDeregisterTimeCfm *prim = CsrPmemAlloc(sizeof(CsrBtHfgDeregisterTimeReq));
    prim->type = CSR_BT_HFG_DEREGISTER_TIME_CFM;
    prim->result = result;

    CsrBtHfgMessagePut(appHandle, prim);
}
