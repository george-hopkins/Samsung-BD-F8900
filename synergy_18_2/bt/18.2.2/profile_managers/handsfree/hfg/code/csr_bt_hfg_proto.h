#ifndef CSR_BT_HFG_PROTO_H__
#define CSR_BT_HFG_PROTO_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "csr_types.h"
#include "csr_bt_result.h"
#ifdef CSR_USE_DSPM
#include "csr_dspm_prim.h"
#endif

/* Special jump table handlers */
void CsrBtHfgMainXSecondHfg(HfgMainInstance_t *inst);
void CsrBtHfgMainXSecondCm(HfgMainInstance_t *inst);
void CsrBtHfgMainXIgnore(HfgMainInstance_t *inst);
void CsrBtHfgXIgnore(HfgInstance_t *inst);

/* Top-level upstream sefs */
void CsrBtHfgMainXCmPortnegIndHandler(HfgMainInstance_t *inst);
void CsrBtHfgMainNullCmRegisterCfmHandler(HfgMainInstance_t *inst);
void CsrBtHfgMainNullCmSdsRegisterCfmHandler(HfgMainInstance_t *inst);
void CsrBtHfgMainIdleCmSdsRegisterCfmHandler(HfgMainInstance_t *inst);
void CsrBtHfgMainActiveCmCancelAcceptConnectCfmHandler(HfgMainInstance_t *inst);
void CsrBtHfgMainActiveCmConnectAcceptCfmHandler(HfgMainInstance_t *inst);
void CsrBtHfgMainActiveCmSdsRegisterCfmHandler(HfgMainInstance_t *inst);
void CsrBtHfgMainActiveCmSdsUnregisterCfmHandler(HfgMainInstance_t *inst);
void CsrBtHfgMainDeactivateCmCancelAcceptConnectCfmHandler(HfgMainInstance_t *inst);
void CsrBtHfgMainDeactivateCmConnectCfmHandler(HfgMainInstance_t *inst);
void CsrBtHfgMainDeactivateCmConnectAcceptCfmHandler(HfgMainInstance_t *inst);
void CsrBtHfgMainDeactivateCmDisconnectIndHandler(HfgMainInstance_t *inst);
void CsrBtHfgMainDeactivateCmScoConnectCfmHandler(HfgMainInstance_t *inst);
void CsrBtHfgMainDeactivateCmScoDisconnectIndHandler(HfgMainInstance_t *inst);
void CsrBtHfgMainDeactivateCmScoAcceptConnectCfmHandler(HfgMainInstance_t *inst);
void CsrBtHfgMainDeactivateCmDataIndHandler(HfgMainInstance_t *inst);
void CsrBtHfgMainDeactivateCmDataCfmHandler(HfgMainInstance_t *inst);
void HfgMainDeactivateCmSdcReleaseResourcesCfmHandler(HfgMainInstance_t *inst);
void CsrBtHfgMainDeactivateCmSdsRegisterCfmHandler(HfgMainInstance_t *inst);
void CsrBtHfgMainDeactivateCmSdsUnregisterCfmHandler(HfgMainInstance_t *inst);
void HfgMainCleanupCmSdsRegisterCfmHandler(HfgMainInstance_t *inst);
void HfgMainCleanupCmSdsUnregisterCfmHandler(HfgMainInstance_t *inst);
void CsrBtHfgMainXCmSdsUnregisterCfmHandler(HfgMainInstance_t *inst);

/* Top-level downstream sefs */
void CsrBtHfgMainNullIdleHfgCancelConnectReqHandler(HfgMainInstance_t *inst);
void CsrBtHfgMainIdleDeactivateReqHandler(HfgMainInstance_t *inst);
void CsrBtHfgMainIndicatorSetupReqHandler(HfgMainInstance_t *inst);
void CsrBtHfgMainXSecurityInReqHandler(HfgMainInstance_t *inst);
void CsrBtHfgMainXSecurityOutReqHandler(HfgMainInstance_t *inst);
void CsrBtHfgMainNullHfgActivateReqHandler(HfgMainInstance_t * inst);
void CsrBtHfgMainActiveHfgDeactivateReqHandler(HfgMainInstance_t *inst);
void HfgMainCleanupHfgActivateReqHandler(HfgMainInstance_t *inst);
void CsrBtHfgMainActiveHfgServiceConnectReqHandler(HfgMainInstance_t *inst);
void CsrBtHfgMainXHfgConfigSniffReqHandler(HfgMainInstance_t *inst);
void CsrBtHfgMainIdleActiveEnvCleanupHandler(HfgMainInstance_t *inst);
#ifdef CSR_BT_INSTALL_HFG_CONFIG_ATCMD_HANDLING
void CsrBtHfgXConfigAtCmdHandling(HfgMainInstance_t *inst);
#endif
#ifdef CSR_BT_INSTALL_HFG_CONFIG_SINGLE_ATCMD
void CsrBtHfgXConfigSingleAtcmd(HfgMainInstance_t *inst);
#endif
void CsrBtHfgMainLocalAdpcmSupportReqHandler(HfgMainInstance_t *inst);
void CsrBtHfgMainXSetDeregisterTimeReqHandler(HfgMainInstance_t *inst);

/* Top-level connection setup helpers */
CsrBool CsrBtHfgConnectionUpdate(HfgMainInstance_t *inst);
void CsrBtHfgConnectionConfirm(HfgMainInstance_t *inst, CsrUint8 chan);
void CsrBtHfgConnectionStart(HfgInstance_t *linkData);
CsrBool CsrBtHfgConnectionClose(HfgInstance_t *inst);
CsrBool CsrBtHfgConnectionGoIdle(HfgMainInstance_t *inst);

/* Top-level service record (SDS) setup helpers */
CsrBool CsrBtHfgRecordUpdate(HfgMainInstance_t *inst);
void CsrBtHfgRecordRegisterConfirm(HfgMainInstance_t *inst, CsrUint32 recHandle);
void CsrBtHfgRecordUnregisterConfirm(HfgMainInstance_t *inst, CsrUint32 recHandle);
CsrBool CsrBtHfgRecordExists(HfgMainInstance_t *inst, CsrUint32 recHandle);

/* Connection-level upstream sefs */
void CsrBtHfgConnectCmCancelAcceptConnectCfmHandler(HfgInstance_t *inst);
void CsrBtHfgConnectedCmModeChangeIndHandler(HfgInstance_t *inst);
void CsrBtHfgXCmDisconnectIndHandler(HfgInstance_t *inst);
void CsrBtHfgXCmDataIndHandler(HfgInstance_t *inst);
void CsrBtHfgXCmDataCfmHandler(HfgInstance_t *inst);
void CsrBtHfgXSecondBccmdCfmHandler(HfgInstance_t *inst);

/* Connection-level up and downstream SDC/SDS sefs */
void CsrBtHfgStartSdcSearch(HfgInstance_t *inst);
void HfgServiceSearchCmConnectCfmHandler(HfgInstance_t *inst);
void CsrBtHfgServiceSearchCmDisconnectIndHandler(HfgInstance_t *inst);
void CsrBtHfgServiceSearchCmModeChangeIndHandler(HfgInstance_t *inst);
void HfgServiceSearchCmSdcSearchIndHandler(HfgInstance_t *inst);
void HfgServiceSearchCmSdcSearchCfmHandler(HfgInstance_t *inst);
void HfgServiceSearchCmSdcAttributeListCfmHandler(HfgInstance_t *inst);
void HfgServiceSearchCmSdcCloseIndHandler(HfgInstance_t *inst);
void CsrBtHfgServiceSearchHfgCancelConnectReqHandler(HfgInstance_t *inst);
void CsrBtHfgServiceSearchHfgDisconnectReqHandler(HfgInstance_t *inst);
void CsrBtHfgServiceSearchHfgXReqHandler(HfgInstance_t *inst);
void CsrBtHfgServiceSearchHfgManualIndicatorResHandler(HfgInstance_t *inst);

/* Connection-level up and downstream audio sefs */
void CsrBtHfgConnectedHfgAudioDisconnectReqHandler(HfgInstance_t *inst);
void CsrBtHfgConnectedHfgAudioReqHandler(HfgInstance_t *inst);
void CsrBtHfgConnectedHfgMapScoPcmResHandler(HfgInstance_t *inst);
void CsrBtHfgConnectedCmScoConnectCfmHandler(HfgInstance_t *inst);
void CsrBtHfgConnectedCmScoDisconnectIndHandler(HfgInstance_t *inst);
void CsrBtHfgConnectedCmScoAcceptConnectCfmHandler(HfgInstance_t *inst);
#ifdef CSR_BT_INSTALL_HFG_CONFIG_AUDIO
void CsrBtHfgXHfgConfigAudioReqHandler(HfgInstance_t *inst);
#endif
void CsrBtHfgXCmMapScoPcmIndHandler(HfgInstance_t *inst);
void csrBtHfgSendCmScoConnectReq(HfgInstance_t *inst, CsrUint8 setting);

/* Connection-level downstream sefs */
void CsrBtHfgXHfgDisconnectReqHandler(HfgInstance_t *inst);
void CsrBtHfgConnectHfgCancelConnectReqHandler(HfgInstance_t *inst);
void CsrBtHfgConnectedHfgDisconnectReqHandler(HfgInstance_t *inst);
void CsrBtHfgConnectedHfgCancelConnectReqHandler(HfgInstance_t *inst);
void CsrBtHfgConnectedHfgRingReqHandler(HfgInstance_t *inst);
void CsrBtHfgConnectedHfgCallWaitingReqHandler(HfgInstance_t *inst);
void CsrBtHfgConnectedHfgCallHandlingReqHandler(HfgInstance_t *inst);
void CsrBtHfgConnectedHfgCallHandlingResHandler(HfgInstance_t *inst);
void CsrBtHfgConnectedHfgDialResHandler(HfgInstance_t *inst);
void CsrBtHfgConnectedHfgSpeakerGainReqHandler(HfgInstance_t *inst);
void CsrBtHfgConnectedHfgMicGainReqHandler(HfgInstance_t *inst);
void CsrBtHfgConnectedHfgAtCmdReqHandler(HfgInstance_t *inst);
void CsrBtHfgConnectedHfgOperatorResHandler(HfgInstance_t *inst);
void CsrBtHfgConnectedHfgCallListResHandler(HfgInstance_t *inst);
void CsrBtHfgConnectedHfgSubscriberNumberResHandler(HfgInstance_t *inst);
void CsrBtHfgXHfgStatusIndicatorSetReqHandler(HfgInstance_t *inst);
void CsrBtHfgConnectedHfgInbandRingingReqHandler(HfgInstance_t *inst);
void CsrBtHfgConnectedHfgBtInputResHandler(HfgInstance_t *inst);
void CsrBtHfgConnectedHfgVoiceRecogReqHandler(HfgInstance_t *inst);
void CsrBtHfgXHfgC2cSfReqHandler(HfgInstance_t *inst);
void CsrBtHfgConnectedHfgC2cTxtReqHandler(HfgInstance_t *inst);
void CsrBtHfgConnectedHfgC2cSmsArriveReqHandler(HfgInstance_t *inst);
void CsrBtHfgConnectedHfgC2cSmsTxtReqHandler(HfgInstance_t *inst);
void CsrBtHfgConnectedHfgVoiceRecogResHandler(HfgInstance_t *inst);

/* Connection-level AT down-stream and helper functions */
void CsrBtHfgAtInterpret(HfgInstance_t *inst);
void CsrBtHfgSendAtRing(HfgInstance_t *inst);
void CsrBtHfgSendAtOk(HfgInstance_t *inst);
void CsrBtHfgSendAtError(HfgInstance_t *inst);
void CsrBtHfgSendAtResponse(HfgInstance_t *inst, CsrUint16 cme);
void CsrBtHfgSendAtCmee(HfgInstance_t *inst, CsrUint16 cmee);
void CsrBtHfgSendAtCiev(HfgInstance_t *inst, CsrUint8 ind, CsrUint8 value);
void CsrBtHfgSendAtSpeakerGain(HfgInstance_t *inst, CsrUint8 gain);
void CsrBtHfgSendAtMicGain(HfgInstance_t *inst, CsrUint8 gain);
void CsrBtHfgSendAtCcwa(HfgInstance_t *inst, CsrCharString *number, CsrCharString *name, CsrUint8 numType);
void CsrBtHfgSendAtClip(HfgInstance_t *inst, CsrCharString *number, CsrCharString *name, CsrUint8 numType);
void CsrBtHfgSendAtBtrh(HfgInstance_t *inst, CsrUint8 btrh);
void CsrBtHfgSendAtClcc(HfgInstance_t *inst,
                   CsrUint8 idx,
                   CsrUint8 dir,
                   CsrUint8 state,
                   CsrUint8 mode,
                   CsrUint8 mpy,
                   CsrCharString *number,
                   CsrUint8 numType);
void CsrBtHfgSendAtCnum(HfgInstance_t *inst, CsrCharString *number, CsrUint8 numType, CsrUint8 service);
void CsrBtHfgSendAtBsir(HfgInstance_t *inst, CsrBool inband);
void CsrBtHfgSendAtBinp(HfgInstance_t *inst, CsrCharString *response);
void CsrBtHfgSendAtCsrTxt(HfgInstance_t *inst, CsrCharString *txt);
void CsrBtHfgSendAtCsrSms(HfgInstance_t *inst, CsrUint16 index, CsrCharString *number, CsrCharString *name);
void CsrBtHfgSendAtCsrGetSms(HfgInstance_t *inst, CsrCharString *txt);
void CsrBtHfgSendAtBvra(HfgInstance_t *inst, CsrUint8 bvra);
void CsrBtHfgSendAtCops(HfgInstance_t *inst, CsrUint8 mode, CsrCharString *operator);
void CsrBtHfgSendAtChldSupport(HfgInstance_t *inst);
void CsrBtHfgSendAtBrsf(HfgInstance_t *inst);
void CsrBtHfgSendAtCindStatus(HfgInstance_t *inst);
void CsrBtHfgSendAtCindSupport(HfgInstance_t *inst);
void CsrBtHfgSendAtCsrSf(HfgInstance_t *inst);
void CsrBtHfgSendAtCsr(HfgInstance_t *inst, CsrUint8 ind, CsrUint8 val);
void CsrBtHfgSendAtCsrFn(HfgInstance_t *inst, CsrUint8 ind, CsrUint8 val);
void CsrBtHfgSendCombinedAtCiev(HfgInstance_t *inst, CsrUint8 indicatorMask);
void CsrBtHfgSendCodecNegMsg(HfgInstance_t *inst);

/* Connection-level AT handler functions */
void HfgAtCkpdMissingTimeout(CsrUint16 mi, void *mv);
void CsrBtHfgAtAtaExec(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq);
void CsrBtHfgAtVgmSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq);
void CsrBtHfgAtVgsSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq);
void CsrBtHfgAtCkpdSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq);
void CsrBtHfgAtChupExec(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq);
void CsrBtHfgAtChldTest(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq);
void CsrBtHfgAtChldSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq);
void CsrBtHfgAtAtdNumberExec(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq);
void CsrBtHfgAtAtdMemoryExec(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq);
void CsrBtHfgAtBacSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq);
void CsrBtHfgAtBccExec(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq);
void CsrBtHfgAtBcsSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq);
void CsrBtHfgAtBldnExec(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq);
void CsrBtHfgAtBrsfSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq);
void CsrBtHfgAtBtrhRead(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq);
void CsrBtHfgAtBtrhSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq);
void CsrBtHfgAtCindRead(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq);
void CsrBtHfgAtCindTest(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq);
void CsrBtHfgAtCmerSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq);
void CsrBtHfgAtCmeeSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq);
void CsrBtHfgAtCcwaSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq);
void CsrBtHfgAtClipSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq);
void CsrBtHfgAtClccExec(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq);
void CsrBtHfgAtCnumExec(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq);
void CsrBtHfgAtCopsRead(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq);
void CsrBtHfgAtCopsSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq);
void CsrBtHfgAtBvraSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq);
void CsrBtHfgAtBinpSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq);
void CsrBtHfgAtCsrSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq);
void CsrBtHfgAtCsrBatSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq);
void CsrBtHfgAtCsrGetsmsSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq);
void CsrBtHfgAtCsrPwrSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq);
void CsrBtHfgAtCsrSfSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq);
void CsrBtHfgAtCsrFnSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq);
void CsrBtHfgAtBiaSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq);
void CsrBtHfgAtNrecSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq);
void CsrBtHfgAtVtsSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq);

/* Message sender helpers */
void CsrBtHfgMessagePut(CsrSchedQid phandle, void *msg);
void CsrBtHfgMainSendHfgDeactivateCfm(HfgMainInstance_t *inst);
void CsrBtHfgMainSendHfgHouseCleaning(HfgMainInstance_t *inst);
void CsrBtHfgSendHfgHouseCleaning(HfgInstance_t *inst);
void CsrBtHfgSendHfgStatusLpInd(HfgInstance_t *inst,
                           CsrUint8 current,
                           CsrUint8 old,
                           CsrUint8 wanted,
                           CsrBool remote,
                           CsrBtResultCode resultCode, 
                           CsrBtSupplier resultSupplier);
void CsrBtHfgSendHfgMicGainInd(HfgInstance_t *inst, CsrUint8 returnValue);
void CsrBtHfgSendHfgSpeakerGainInd(HfgInstance_t *inst, CsrUint8 returnValue);
void CsrBtHfgSendHfgServiceConnectInd(HfgInstance_t *inst, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);
void CsrBtHfgSendHfgDisconnectInd(HfgInstance_t *inst, CsrBool localTerminated, CsrBtResultCode reasonCode, CsrBtSupplier reasonSupplier);
void CsrBtHfgSendHfgAnswerInd(HfgInstance_t *inst);
void CsrBtHfgSendHfgRejectInd(HfgInstance_t *inst);
void CsrBtHfgSendHfgVoiceRecogInd(HfgInstance_t *inst, CsrBool enable);
void CsrBtHfgSendHfgDialInd(HfgInstance_t *inst, CsrBtHfgDialCommand cmd, CsrCharString *num);
void CsrBtHfgSendHfgCallHandlingInd(HfgInstance_t *inst, CsrUint8 cmd, CsrUint8 idx);
void CsrBtHfgSendHfgNoiseEchoInd(HfgInstance_t *inst, CsrBool nrec);
void CsrBtHfgSendHfgGenerateDtmfInd(HfgInstance_t *inst, char dtmf);
void CsrBtHfgSendHfgBtInputInd(HfgInstance_t *inst, CsrUint8 req);
void CsrBtHfgSendHfgAtCmdInd(HfgInstance_t *inst, CsrBool cme);
void CsrBtHfgSendHfgRingCfm(HfgInstance_t *inst);
void CsrBtHfgSendHfgC2cSfInd(HfgInstance_t *inst);
void CsrBtHfgSendHfgC2cBatteryInd(HfgInstance_t *inst, CsrUint8 c2cBatteryLevel);
void CsrBtHfgSendHfgC2cPowerInd(HfgInstance_t *inst, CsrUint8 c2cPowerStatus);
void CsrBtHfgSendHfgC2cSmsGetInd(HfgInstance_t *inst, CsrUint16 c2cSmsIndex);
void CsrBtHfgSendHfgMapScoPcmInd(HfgInstance_t *inst);
void CsrBtHfgSendHfgAudioDisconnectInd(HfgInstance_t *inst,
                                    CsrUint16 scoHandle,
                                    CsrBtReasonCode reasonCode,
                                    CsrBtSupplier reasonSupplier);
void CsrBtHfgSendHfgAudioDisconnectCfm(HfgInstance_t *inst,
                                    CsrUint16 scoHandle,
                                    CsrBtResultCode resultCode,
                                    CsrBtSupplier resultSupplier);
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
                                    CsrBtSupplier resultSupplier);
void CsrBtHfgSendHfgCallListInd(HfgInstance_t *inst);
void CsrBtHfgSendHfgSubscriberNumberInd(HfgInstance_t *inst);
void CsrBtHfgSendHfgOperatorInd(HfgInstance_t *inst);
void CsrBtHfgSendScoHfgStatusAudioInd(HfgInstance_t *inst,
                                 CsrBtHfgAudioScoStatus *set);
void CsrBtHfgSendAudioCodecHfgStatusAudioInd(HfgInstance_t *inst,
                                        CsrBtHfgAudioCodecStatus *set);
void CsrBtHfgSendCmDataReq(HfgInstance_t *inst, CsrUint16 len, CsrUint8 *data);
void CsrBtHfgSendCmControlReq(HfgInstance_t *inst);
void CsrBtHfgSendCmSecurityInCfm(CsrSchedQid appHandle, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);
void CsrBtHfgSendCmSecurityOutCfm(CsrSchedQid appHandle, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);
void CsrBtHfgSendCmMapScoPcmRes(HfgInstance_t *inst, CsrUint8 slot, CsrBool realloc);
void CsrBtHfgSendHfgManualIndicatorInd(HfgInstance_t *inst);
#ifdef CSR_BT_INSTALL_HFG_CONFIG_ATCMD_HANDLING
void CsrBtHfgSendHfgConfigAtCmdHandlingCfm(CsrSchedQid appHandle,
                                      CsrBtHfgAtCmdResultCodes result, 
                                      CsrUint8 *bitwiseIndicators,
                                      CsrUint8 length);
#endif
#ifdef CSR_BT_INSTALL_HFG_CONFIG_SINGLE_ATCMD
void CsrBtHfgSendHfgConfigSingleAtCmdCfm(CsrSchedQid appHandle,
                                    CsrBtHfgAtCmdResultCodes result);
#endif
void CsrBtHfgSendHfgLocalAdpcmSupportInd(HfgMainInstance_t *inst, CsrBool result);
void CsrBtHfgSendHfgDeregisterTimeCfm(CsrSchedQid appHandle,CsrBtHfgAtCmdResultCodes result);
/*Adpcm specific functions */
void CsrBtHfgSetC2CAudioCodec(CsrUint16 scoHandle,
                         CsrUint8 value,
                         CsrUint16 seqNo);
void CsrBtHfgMainGetC2CAdpcmSupported(CsrUint16 seqNo);
void CsrBtHfgMainBccmdCfmHandler(HfgMainInstance_t *inst);

/* Instance utility functions */
CsrUint8 CsrBtHfgFindIndexWithHfgPrim(HfgMainInstance_t *inst, CsrUint16 type);
CsrUint8 CsrBtHfgFindIndexWithAddr(HfgMainInstance_t *inst, CsrBtDeviceAddr *addr);
CsrUint8 CsrBtHfgFindIndexWithConnId(HfgMainInstance_t *inst, CsrUint32 hfgConnId);
HfgInstance_t *CsrBtHfgFindLinkWithCmPrim(HfgMainInstance_t *inst, CsrUint16 type);
HfgInstance_t *CsrBtHfgFindLinkWithServerChan(HfgMainInstance_t *inst, CsrUint8 sc);
HfgInstance_t *CsrBtHfgFindLinkIndexWithServerChan(HfgMainInstance_t *inst, CsrUint8 sc, CsrUint8 *index);
HfgInstance_t *CsrBtHfgFindLinkIndexAvailable(HfgMainInstance_t *inst, CsrUint8 *index);
HfgInstance_t *CsrBtHfgFindLinkWithAddr(HfgMainInstance_t *inst, CsrBtDeviceAddr *addr);
HfgInstance_t *CsrBtHfgFindLinkAvailable(HfgMainInstance_t *inst);
CsrUint8 CsrBtHfgFindFreeServerChannel(HfgMainInstance_t *inst);
#define CsrBtHfgGetMainInstance(inst) ((HfgMainInstance_t *) (inst->main))
CsrBool CsrBtHfgOccupiedServerChannel(HfgMainInstance_t *inst, CsrUint8 sc);
HfgInstance_t *CsrBtHfgFindLinkWithSeqNo(HfgMainInstance_t *inst, CsrUint16 seqNo);
HfgInstance_t *CsrBtHfgFindLinkWithConnId(HfgMainInstance_t *inst, CsrUint32 hfgConnId);
HfgInstance_t *CsrBtHfgFindLinkIndexWithConnId(HfgMainInstance_t *inst, CsrUint32 hfgConnId, CsrUint8 *index);

/* Miscellaneous utility functions */
void CsrBtHfgLinkConnectSuccess(HfgInstance_t *inst);
void CsrBtHfgLinkConnectFailed(HfgInstance_t *inst);
void CsrBtHfgLinkDisconnect(HfgInstance_t *inst, CsrBool localTerminated, CsrBtResultCode reasonCode, CsrBtSupplier reasonSupplier);
void CsrBtHfgLpTimeoutHandler(HfgInstance_t *inst);
void CsrBtHfgLpTimeout(CsrUint16 mi, void *mv);
void CsrBtHfgLpStop(HfgInstance_t *inst);
void CsrBtHfgLpStart(HfgInstance_t *inst, CsrBool now);
void CsrBtHfgRingTimeout(CsrUint16 mi, void *mv);
void CsrBtHfgHandleRingEvent(HfgInstance_t *inst);
void CsrBtHfgRingStop(HfgInstance_t *inst);
void CsrBtHfgInitializeIndicators(HfgInstance_t *inst);
void CsrBtHfgInitializeConnInstance(HfgMainInstance_t *inst, CsrUint8 index);
void CsrBtHfgMainStoreMessageData(HfgMainInstance_t *inst);
void CsrBtHfgMainSaveMessage(HfgMainInstance_t *inst);
void CsrBtHfgSaveMessage(HfgInstance_t *inst);
void CsrBtHfgEmptySaveQueue(HfgInstance_t *inst);
void CsrBtHfgResetProfile(HfgMainInstance_t *inst);
void CsrBtHfgFreeMessage(CsrUint16 class, void *msg);
CsrBool CsrBtHfgCsrFeature(HfgInstance_t *inst, CsrUint8 feat);
CsrBool CsrBtHfgUpdateService(HfgMainInstance_t *inst);
void CsrBtHfgDeRegisterTimeout(CsrUint16 mi, void *mv);
void CsrBtHfgConnectionTimeout(CsrUint16 mi, void *mv);

void csrBtHfgSlcHfgDone(HfgInstance_t *inst);

/* Prototypes from hfg_free_down.c */
void CsrBtHfgFreeDownstreamMessageContents(CsrUint16 eventClass, void * message);


#ifdef __cplusplus
}
#endif

#endif
