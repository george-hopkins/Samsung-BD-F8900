#ifndef CSR_BT_HF_CONNECT_SEF_H__
#define CSR_BT_HF_CONNECT_SEF_H__

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
void CsrBtHfXStateHfCancelReqHandler(HfMainInstanceData_t *instData);
void CsrBtHfXStateHfDisconnectReqHandler(HfMainInstanceData_t *instData);

/* Define CM handlers */
void CsrBtHfActivateStateCmConnectAcceptCfmHandler(HfMainInstanceData_t *instData);
void CsrBtHfServiceSearchStateCmCancelAcceptCfmHandler(HfMainInstanceData_t * instData);
void CsrBtHfServiceSearchStateCmModeChangeIndHandler(HfMainInstanceData_t *instData);

void CsrBtHfConnectStateCmCancelAcceptConnectCfmHandler(HfMainInstanceData_t *instData);
void CsrBtHfXStateCmConnectAcceptCfmHandler(HfMainInstanceData_t *instData);

void CsrBtHfXStateCmDisconnectIndHandler(HfMainInstanceData_t *instData);
void CsrBtHfConnectedStateCmModeChangeIndHandler(HfMainInstanceData_t *instData);
void CsrBtHfLpEnabledStateCmModeChangeIndHandler(HfMainInstanceData_t *instData);

#ifdef __cplusplus
}
#endif

#endif
