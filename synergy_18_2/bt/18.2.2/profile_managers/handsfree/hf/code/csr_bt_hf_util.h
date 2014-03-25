#ifndef CSR_BT_HF_UTIL_H__
#define CSR_BT_HF_UTIL_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_sched.h"
#include "csr_pmem.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_util.h"
#include "csr_exceptionhandler.h"
#include "sds_prim.h"
#include "csr_bt_hf_main.h"
#include "csr_bt_hf_prim.h"
#include "csr_bt_hf_dspm.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrBtHfMessagePut(CsrSchedQid phandle, void *msg);

void CsrBtHfSaveMessage(HfMainInstanceData_t *instData);
void CsrBtHfSendHfHouseCleaning(HfMainInstanceData_t *instData);
void CsrBtHfInitInstanceData(HfMainInstanceData_t *instData);
void CsrBtHfSaveQueueCleanUp(HfMainInstanceData_t *instData);
void CsrBtHfSendSdsRegisterReq(HfMainInstanceData_t *instData);
void HsSendSdsRegisterReq(HfMainInstanceData_t *instData);

void CsrBtHfSendConfirmMessage(HfMainInstanceData_t *instData, CsrBtHfPrim type);
void CsrBtHfSendHfMicGainInd(HfMainInstanceData_t *instData, CsrUint8 returnValue);
void CsrBtHfSendHfSpeakerGainInd(HfMainInstanceData_t *instData, CsrUint8 returnValue);
void CsrBtHfSendHfRingInd(HfMainInstanceData_t *instData);
void CsrBtHfSendHfServiceConnectInd(HfMainInstanceData_t *instData,
                                    CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);
void CsrBtHfSendHfAudioDisconnectCfm(HfMainInstanceData_t *instData,
                           CsrUint16 scoHandle,
                           CsrBtResultCode      resultCode,
                           CsrBtSupplier  resultSupplier);
void CsrBtHfSendHfAudioDisconnectInd(HfMainInstanceData_t *instData,
                           CsrUint16 scoHandle,
                           CsrBtReasonCode      reasonCode,
                           CsrBtSupplier  resultSupplier);
void CsrBtHfSendHfAudioInd(HfMainInstanceData_t *instData,
                           CsrUint8              pcmSlot,
                           CsrUint8              theScoLinkType,
                           CsrUint8              weSco,
                           CsrUint16             rxPacketLength,
                           CsrUint16             txPacketLength,
                           CsrUint8              airMode,
                           CsrUint8              txInterval,
                           CsrBtResultCode      resultCode,
                           CsrBtReasonCode      reasonCode,
                           CsrBtSupplier  resultSupplier);

void CsrBtHfSendIndicatorsUpdateCfm(HfMainInstanceData_t *instData, CsrUint16 result);
void CsrBtHfSendHfDisconnectInd(HfMainInstanceData_t *instData,
                                CsrBtResultCode reasonCode, CsrBtSupplier reasonSupplier);
void CsrBtHfSendHfStatusInd(HfMainInstanceData_t *instData, CsrUint8 status, CsrUint8 old,
                            CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);
void CsrBtHfSendHfActivateCfm(HfMainInstanceData_t *instData,
                              CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);
void CsrBtHfSendHfDeactivateCfm(HfMainInstanceData_t *instData,
                                CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);
void CsrBtHfSendHfGeneralCfmMsg(HfMainInstanceData_t *instData, CsrUint16 result, CsrBtHfPrim type);

void CsrBtHfSendHfCmeeInd(HfMainInstanceData_t *instData, CsrUint8 *atTextString);
void HfSendHfCopsCfm(HfMainInstanceData_t *instData, CsrUint8 *atTextString);
void CsrBtHfSendHfCallHandlingInd(HfMainInstanceData_t *instData, CsrUint8 *atTextString);

void sendBrsf(HfMainInstanceData_t * instData);
void sendCodecSupport(HfMainInstanceData_t * instData);
void sendCindSupport(HfMainInstanceData_t * instData);
void sendCindStatus(HfMainInstanceData_t * instData);
void sendSetCmer(HfMainInstanceData_t * instData,CsrBool enable);
void sendCallHoldStatus(HfMainInstanceData_t * instData);
void sendCkpd(HfMainInstanceData_t * instData);
void startSdcFeatureSearch(HfMainInstanceData_t * instData, CsrBool outgoing);
void startAtFeatureSearch(HfMainInstanceData_t * instData);
void sendBia(HfMainInstanceData_t * instData);
void CsrBtHfSendCmScoConnectReq(HfInstanceData_t *instData, CsrUint8 default_setting, audioSetupParams_t *audioParms);

void CsrBtHfSendHfInBandRingToneInd(HfMainInstanceData_t *instData,
                               CsrUint8               returnValue);

void HfBrsfTimeout(CsrUint16 mi, void *mv);
void CsrBtHfAtResponseTimeout(CsrUint16 mi, void *mv);
void CsrBtHfAcceptIncomingSco(HfMainInstanceData_t *instData);
void HsDeregisterTimeout(CsrUint16 mi, void *mv);

void CsrBtHfAtCopsSetCommandSend(HfMainInstanceData_t *instData,CsrUint8 mode, CsrUint8 format);
void CsrBtHfAtCopsQuerySend(HfMainInstanceData_t *instData);
void CsrBtHfAtClccSend(HfMainInstanceData_t *instData);
void CsrBtHfAtCnumSend(HfMainInstanceData_t *instData);
void CsrBtHfAtCmeeSetCommandSend(HfMainInstanceData_t *instData,CsrUint8 mode);
void CsrBtHfAtCcwaSend(HfMainInstanceData_t *instData, CsrBool enable);
void CsrBtHfAtClipSend(HfMainInstanceData_t *instData, CsrBool enable);
void CsrBtHfAtNrecSend(HfMainInstanceData_t *instData, CsrBool enable);
void CsrBtHfAtBvraSend(HfMainInstanceData_t *instData, CsrBool start);
void CsrBtHfAtVtsSend(HfMainInstanceData_t *instData, CsrUint8 dtmf);
void CsrBtHfAtStatusIndValueSend(HfMainInstanceData_t *instData);
void CsrBtHfAtBinpSend(HfMainInstanceData_t *instData, CsrUint32 dataRequest);
void CsrBtHfAtDialSend(HfMainInstanceData_t *instData, CsrBtHfDialCommand  command, CsrCharString *number);
void CsrBtHfSendC2CInitMsgReq(HfMainInstanceData_t *instData);
void CsrBtHfSendC2CSf(HfMainInstanceData_t *instData);
void CsrBtHfSendC2CBatt(HfMainInstanceData_t *instData);
void CsrBtHfSendC2CPwr(HfMainInstanceData_t *instData);
void CsrBtHfSendC2CGetSMS(HfMainInstanceData_t *instData);
void CsrBtHfSendAtBcs(HfMainInstanceData_t *instData);
void CsrBtHfSendAtBcc(HfMainInstanceData_t *instData);

void CsrBtHfLpStop(HfMainInstanceData_t *inst);
void CsrBtHfLpStart(HfMainInstanceData_t *inst, CsrBool now);

CsrUint8 CsrBtHfGetNumberOfRecordsInUse(HfMainInstanceData_t *instData,CsrUint8 *nrActiveHf,CsrUint8 *nrActiveHs);
void CsrBtHfCancelAcceptCheck(HfMainInstanceData_t *instData);
void CsrBtHfAllowConnectCheck(HfMainInstanceData_t *instData);
CsrBool getLocC2CEnabled(HfMainInstanceData_t *instData);

void CsrBtHfSendHfC2CSfInd(HfMainInstanceData_t *instData,CsrUint8 *atTextString);
void CsrBtHfSendHfC2CBattInd(HfMainInstanceData_t *instData);
void CsrBtHfSendHfC2CSmsInd(HfMainInstanceData_t *instData, CsrUint8 index);
void CsrBtHfSendHfC2CGetSMSCfm(HfMainInstanceData_t *instData, CsrUint8 *atTextString);
void CsrBtHfSendHfC2CTxtInd(HfMainInstanceData_t *instData, CsrUint8 *atTextString);

void CsrBtHfSendUpdateCodecSupportedCfm(HfMainInstanceData_t *instData);

#ifdef __cplusplus
}
#endif

#endif
