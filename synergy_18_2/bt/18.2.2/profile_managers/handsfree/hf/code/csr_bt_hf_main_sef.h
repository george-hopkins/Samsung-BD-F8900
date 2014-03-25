#ifndef CSR_BT_HF_MAIN_SEF_H__
#define CSR_BT_HF_MAIN_SEF_H__

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
#include "csr_bt_sdc_support.h"
#include "csr_bt_hf_main.h"
#include "csr_bt_hf_util.h"
#include "csr_bt_hf_lib.h"

#ifdef __cplusplus
extern "C" {
#endif

/* General functions */
void CsrBtHfCleanup(HfMainInstanceData_t *instData);
void CsrBtHfCleanup_reinit_instance(HfMainInstanceData_t *instData);

/* Define CM handlers */
void CsrBtHfNullStateCmRegisterCfmHandler(HfMainInstanceData_t *instData);
void CsrBtHfNullStateCmSdsRegisterCfmHandler(HfMainInstanceData_t * instData);

/* Activate State, upstream */
void CsrBtHfActivatedStateCmCancelAcceptConnectCfmHandler(HfMainInstanceData_t * instData);
void CsrBtHfActivatedStateCmConnectAcceptCfmHandler(HfMainInstanceData_t * instData);
void CsrBtHfActivatedStateCmDisconnectIndHandler(HfMainInstanceData_t * instData);
void CsrBtHfActivatedStateCmScoConnectCfmHandler(HfMainInstanceData_t * instData);
void CsrBtHfActivatedStateCmScoDisconnectIndHandler(HfMainInstanceData_t * instData);
void CsrBtHfActivatedStateCmScoAcceptConnectCfmHandler(HfMainInstanceData_t * instData);
void CsrBtHfActivatedStateCmDataIndHandler(HfMainInstanceData_t * instData);
void CsrBtHfActivatedStateCmDataCfmHandler(HfMainInstanceData_t * instData);
void CsrBtHfXStateIgnoreCmControlIndHandler(HfMainInstanceData_t * instData);
void CsrBtHfActivatedStateCmModeChangeIndHandler(HfMainInstanceData_t * instData);
void CsrBtHfActivatedStateCmPortnegIndHandler(HfMainInstanceData_t * instData);
void CsrBtHfActivatedStateCmSdsRegisterCfmHandler(HfMainInstanceData_t * instData);
void CsrBtHfActivatedStateCmSdsUnregisterCfmHandler(HfMainInstanceData_t * instData);

/* Deactivate State, upstream */
void CsrBtHfDeactivateStateCmCancelAcceptConnectCfmHandler(HfMainInstanceData_t * instData);
void CsrBtHfDeactivateStateCmConnectAcceptCfmHandler(HfMainInstanceData_t * instData);
void CsrBtHfDeactivateStateCmDisconnectIndHandler(HfMainInstanceData_t * instData);
void CsrBtHfDeactivateStateCmScoConnectCfmHandler(HfMainInstanceData_t * instData);
void CsrBtHfDeactivateStateCmScoDisconnectIndHandler(HfMainInstanceData_t * instData);
void CsrBtHfDeactivateStateCmScoAcceptConnectCfm(HfMainInstanceData_t * instData);
void CsrBtHfDeactivateStateCmDataIndHandler(HfMainInstanceData_t * instData);
void CsrBtHfDeactivateStateCmDataCfmHandler(HfMainInstanceData_t * instData);
void CsrBtHfDeactivateStateCmControlIndHandler(HfMainInstanceData_t * instData);
void CsrBtHfDeactivateStateCmSdsUnregisterCfmHandler(HfMainInstanceData_t * instData);
void CsrBtHfDeactivateStateCmModeChangeIndHandler(HfMainInstanceData_t * instData);
void CsrBtHfDeactivateStateCmSdsRegisterCfmHandler(HfMainInstanceData_t * instData);

/* Define handlers for HF prims */
void CsrBtHfXStateConfigLowPowerReqHandler(HfMainInstanceData_t * instData);
#ifdef CSR_BT_INSTALL_HF_CONFIG_AUDIO
void CsrBtHfXStateConfigAudioReqHandler(HfMainInstanceData_t * instData);
#endif
void CsrBtHfNullStateDeactivateReqHandler(HfMainInstanceData_t * instData);
void CsrBtHfXStateActivateReqHandler(HfMainInstanceData_t * instData);
void HsHfSecurityInReqHandler(HfMainInstanceData_t * instData);
void HsHfSecurityOutReqHandler(HfMainInstanceData_t * instData);
void CsrBtHfActivatedStateHfDeactivateReqHandler(HfMainInstanceData_t * instData);
void CsrBtHfActivatedStateHfCancelReqHandler(HfMainInstanceData_t * instData);
void CsrBtHfActivatedStateHfDisconnectReqHandler(HfMainInstanceData_t * instData);
void CsrBtHfActivatedStateHfServiceConnectReq(HfMainInstanceData_t * instData);
void CsrBtHfActivatedStateAudioDisconnectReqHandler(HfMainInstanceData_t * instData);
void CsrBtHfActivatedStateAudioReqHandler(HfMainInstanceData_t * instData);
void CsrBtHfMainXStateMapScoPcmIndHandler(HfMainInstanceData_t * instData);
void CsrBtHfActivatedStateMapScoPcmResHandler(HfMainInstanceData_t * instData);
void CsrBtHfActivatedStateAnswerReqHandler(HfMainInstanceData_t * instData);
void CsrBtHfActivatedStateRejectReqHandler(HfMainInstanceData_t * instData);
void CsrBtHfActivatedStateSpeakerGainStatusReqHandler(HfMainInstanceData_t * instData);
void CsrBtHfActivatedStateMicGainStatusReqHandler(HfMainInstanceData_t * instData);
void CsrBtHfActivatedStateAtCmdReqHandler(HfMainInstanceData_t * instData);
void CsrBtHfActivatedStateChldReqHandler(HfMainInstanceData_t * instData);
void CsrBtHfXStateHfCopsReqHandler(HfMainInstanceData_t *instData);
void CsrBtHfXStateHfGetAllStatusReqHandler(HfMainInstanceData_t *instData);
void CsrBtHfXStateHfCallListReqHandler(HfMainInstanceData_t *instData);
void CsrBtHfXStateHfSubscriberReqHandler(HfMainInstanceData_t *instData);
void CsrBtHfXStateHfSetExtErrorReqHandler(HfMainInstanceData_t *instData);
void CsrBtHfXStateHfSetClipHandler(HfMainInstanceData_t *instData);
void CsrBtHfXStateHfSetCcwaHandler(HfMainInstanceData_t *instData);
void CsrBtHfXStateHfSetNrecHandler(HfMainInstanceData_t *instData);
void CsrBtHfXStateHfSetBvraHandler(HfMainInstanceData_t *instData);
void CsrBtHfXStateDTMFReqHandler(HfMainInstanceData_t *instData);
void DeactivateHfHandler(HfMainInstanceData_t *instData);
void DeactivateHsHandler(HfMainInstanceData_t *instData);
void CsrBtHfGetC2CAdpcmLocalSupportedReqHandler(HfMainInstanceData_t *instData);
void CsrBtHfActivatedSetStatusIndUpdateReqHandler(HfMainInstanceData_t *instData);
void CsrBtHfActivatedBtInputReqHandler(HfMainInstanceData_t *instData);
void CsrBtHfActivatedDialRequest(HfMainInstanceData_t *instData);
void CsrBtHfMainIgnoreMessage(HfMainInstanceData_t *instData);
void CsrBtHfXStateHfCommonAtCmdReqHandler(HfMainInstanceData_t *instData);
void CsrBtHfCommonAtCmdPrimReqHandler(HfMainInstanceData_t *instData, CsrBtHfPrim *primType);
void CsrBtHfXStateSetDeregisterTimeReqHandler(HfMainInstanceData_t *instData);
void CsrBtHfXStateIndicatorActivationReqHandler(HfMainInstanceData_t *instData);
void CsrBtHfXStateHfC2cSfReqHandler(HfMainInstanceData_t *instData);
void CsrBtHfActivatedStateC2CPwrReqHandler(HfMainInstanceData_t *instData);
void CsrBtHfActivatedStateC2CBattReqHandler(HfMainInstanceData_t *instData);
void CsrBtHfActivatedStateC2CGetSMSReqHandler(HfMainInstanceData_t *instData);
void CsrBtHfXStateHfUpdateCodecSupportReqHandler(HfMainInstanceData_t *instData);

/* Prototypes from hf_free_down.c */
void CsrBtHfFreeDownstreamMessageContents(CsrUint16 eventClass, void * message);

#ifdef __cplusplus
}
#endif

#endif
