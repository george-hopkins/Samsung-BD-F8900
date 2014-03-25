#ifndef CSR_BT_HF_DSPM_H__
#define CSR_BT_HF_DSPM_H__

#ifdef CSR_USE_DSPM
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #3 $
****************************************************************************/

#include "csr_bt_hf_main.h"

#define HF_DSPM_DEFAULT_HANDLE 0xFFFF

#ifdef __cplusplus
extern "C" {
#endif

void CsrHfDspmInit(HfMainInstanceData_t *inst);
void CsrHfDspmActivateRspHandler(HfMainInstanceData_t *instData);
void CsrHfDspmEssCfm(HfMainInstanceData_t *inst);
void CsrHfDspmAudioConnect(HfMainInstanceData_t *instData);
void CsrBtHfDspmStreamOpenRspHandle(HfMainInstanceData_t *instData);
void CsrBtHfDspmConfigStreamRspHandler(HfMainInstanceData_t *instData);
void CsrBtHfDspmSendSingleConfigStream(HfMainInstanceData_t *instData, CsrBtHfAudioDspStreamSingleConfig *configData);
void CsrBtHfDspmSingleConfigStreamRspHandler(HfMainInstanceData_t *instData);
void CsrBtHfDspmSendCreateOperators(HfMainInstanceData_t *instData);
void CsrBtHfDspmSendDestroyOldOperators(HfMainInstanceData_t *instData);
void CsrBtHfDspmCreateOperatorsRspHandler(HfMainInstanceData_t *instData);
void CsrBtHfDspmSendConnectStreams(HfMainInstanceData_t *instData);
void CsrBtHfDspmConnectionCreateRspHandler(HfMainInstanceData_t *instData);
void CsrDspmOperatorStartRspHandler(HfMainInstanceData_t *instData);
void CsrBtHfDspmSendDisconnectStreams(HfMainInstanceData_t *instData);
void CsrBtHfDspmOperatorStopRspHandler(HfMainInstanceData_t *instData);
void CsrBtHfDspmConnectionDestroyRspHandler(HfMainInstanceData_t *instData);
void CsrBtHfDspmOperatorDestroyRspHandler(HfMainInstanceData_t *instData);
void CsrBtHfDspmStreamCpCloseRspHandler(HfMainInstanceData_t *instData);
void CsrBtHfDspmCleanUpOperators(HfInstanceData_t *linkPtr);

#ifdef __cplusplus
}
#endif

#endif /*CSR_USE_DSPM*/

#endif /* CSR_BT_HF_DSPM_H__ */
