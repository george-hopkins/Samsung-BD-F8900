#ifndef CSR_BT_BSL_SEF_H__
#define CSR_BT_BSL_SEF_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_bt_bsl_main.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Define bsl handler functions */
void CsrBtBslRegisterStateBslRegisterReqHandler(BslInstanceData_t *instData);
void CsrBtBslXStateBslActivateReqHandler(BslInstanceData_t *instData);
void CsrBtBslIdleStateBslConnectReqHandler(BslInstanceData_t *instData);
void CsrBtBslXStateBslServiceSearchReqHandler(BslInstanceData_t *instData);
void CsrBtBslXStateCmBnepConnectIndHandler(BslInstanceData_t *instData);
void CsrBtBslXStateBslDataReqHandler(BslInstanceData_t *instData);
void CsrBtBslXStateBslMulticastDataReqHandler(BslInstanceData_t *instData);
void CsrBtBslXStateBslDisconnectReqHandler(BslInstanceData_t *instData);
void CsrBtBslXStateBslDisconnectResHandler(BslInstanceData_t *instData);
void CsrBtBslXStateSaveMessage(BslInstanceData_t *instData);
void CsrBtBslIgnoreMessage(BslInstanceData_t *instData);
void CsrBtBslSecurityInReqHandler(BslInstanceData_t *instData);
void CsrBtBslSecurityOutReqHandler(BslInstanceData_t *instData);

/* Define CM handler functions */
void CsrBtBslRegisterStateCmReadLocalBdAddrCfmHandler(BslInstanceData_t *instData);
void CsrBtBslSdsRegisterStateCmBnepConnectAcceptCfmHandler(BslInstanceData_t *instData);
void CsrBtBslSdsRegisterStateCmBnepCancelConnectAcceptCfmHandler(BslInstanceData_t *instData);
void CsrBtBslBusyStateCmBnepCancelConnectAcceptCfmHandler(BslInstanceData_t *instData);
void CsrBtBslXStateCmBnepExtendedDataIndHandler(BslInstanceData_t *instData);
void CsrBtBslXStateCmBnepModeChangeIndHandler(BslInstanceData_t *instData);
void CsrBtBslXStateCmBnepExtendedDataIndHandler(BslInstanceData_t *instData);
void CsrBtBslXStateCmBnepDisconnectIndHandler(BslInstanceData_t *instData);
void CsrBtBslXStateCmSwitchRoleIndHandler(BslInstanceData_t *instData);
void CsrBtBslXStateCsrBtCmBnepConnectReqSendTimerHandler(CsrUint16 mv, void *mp);

/* Define CM SDS handler functions */
void CsrBtBslSdsRegisterStateCmSdsRegisterCfmHandler(BslInstanceData_t *instData);
void CsrBtBslSdsRegisterStateCmSdsUnregisterCfmHandler(BslInstanceData_t *instData);

/* Define CM SDC handler functions */
void CsrBtBslBusyStateCmSdcSearchIndHandler(BslInstanceData_t *instData);
void CsrBtBslBusyStateCmSdcSearchCfmHandler(BslInstanceData_t *instData);
void CsrBtBslBusyStateCmSdcAttributeCfmHandler(BslInstanceData_t *instData);
void CsrBtBslBusyStateCmSdcCloseIndHandler(BslInstanceData_t *instData);

/* Define Cancel Connect handlers */
void CsrBtBslXStateCancelConnectReqHandler(BslInstanceData_t *instData);

#ifdef __cplusplus
}
#endif

#endif
