#ifndef CSR_BT_HF_CALL_SEF_H__
#define CSR_BT_HF_CALL_SEF_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_bt_hf_main.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Define Hf handlers */
void CsrBtHfConnectedStateHfAtCmdReqHandler(HfMainInstanceData_t *instData);
void CsrBtHfConnectedStateHfChldReqHandler(HfMainInstanceData_t *instData);
void CsrBtHfXStateMapScoPcmIndHandler(HfMainInstanceData_t *instData);
void CsrBtHfLpEnabledHfXReqHandler(HfMainInstanceData_t *instData);
void CsrBtHfConnectedStateHfAudioDisconnectReqHandler(HfMainInstanceData_t *instData);
void CsrBtHfConnectedStateHfAudioReqHandler(HfMainInstanceData_t *instData);
void CsrBtHfConnectedStateHfSpeakerGainStatusReqHandler(HfMainInstanceData_t *instData);
void CsrBtHfConnectedStateHfMicGainStatusReqHandler(HfMainInstanceData_t *instData);
void CsrBtHfConnectedStateHfAnswerReqHandler(HfMainInstanceData_t *instData);
void CsrBtHfConnectedStateHfCallEndReqHandler(HfMainInstanceData_t *instData);

/* Define CM handlers */
void HfXStateCmDataIndHandler(HfMainInstanceData_t *instData);
void HfXStateCmDataCfmHandler(HfMainInstanceData_t *instData);
void CsrBtHfConnectedStateCmScoConnectCfmHandler(HfMainInstanceData_t *instData);
void CsrBtHfConnectedStateCmScoDisconnectIndHandler(HfMainInstanceData_t *instData);
void CsrBtHfConnectedStateCmScoAcceptConnectCfmHandler(HfMainInstanceData_t *instData);

void CsrBtHfActivateStateCmSdsRegisterCfmHandler(HfMainInstanceData_t * instData);
void HsConnectStateCmSdsUnRegisterCfmHandler(HfMainInstanceData_t * instData);
void CsrBtHfXStateCmSdsUnRegisterCfmHandler(HfMainInstanceData_t * instData);

#ifdef __cplusplus
}
#endif

#endif

