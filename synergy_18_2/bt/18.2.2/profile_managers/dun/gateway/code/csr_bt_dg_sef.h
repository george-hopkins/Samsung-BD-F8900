#ifndef CSR_BT_DG_SEF_H__
#define CSR_BT_DG_SEF_H__

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_DG_MODULE
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_types.h"
#include "csr_bt_result.h"
#include "csr_bt_profiles.h"
#include "rfcomm_prim.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "csr_sched.h"
#include "csr_message_queue.h"
#include "csr_bt_dg_main.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Define Dg handlers */
void CsrBtDgEnvironmentCleanupHandler(DgInstanceData_t *instData);
void DgIdleStateDgConnectReqHandler(DgInstanceData_t *instData);
void CsrBtDgIdleStateDgActivateReqHandler(DgInstanceData_t *instData);
void CsrBtDgConnectedStateDgDataReqHandler(DgInstanceData_t *instData);
void CsrBtDgConnectedStateDgDataResHandler(DgInstanceData_t *instData);
void CsrBtDgConnectedStateDgControlReqHandler(DgInstanceData_t *instData);
void CsrBtDgConnectedStateDgDisconnectReqHandler(DgInstanceData_t *instData);
void CsrBtDgActivateStateDgDeactivateReqHandler(DgInstanceData_t *instData);
void DgActivateStateDgConnectReqHandler(DgInstanceData_t *instData);
void CsrBtDgNotConnectedDgDataReqHandler(DgInstanceData_t *instData);
void CsrBtDgConnectedStateDgPortnegResHandler(DgInstanceData_t *instData);
void CsrBtDgConnectedStateDgDeactivateReqHandler(DgInstanceData_t *instData);
void CsrBtDgXStateRegisterDataPathHandleReqHandler(DgInstanceData_t *instData);
void CsrBtDgConnectedStateDgDataPathStatusReqHandler(DgInstanceData_t *instData);
void CsrBtDgSecurityInReqHandler(DgInstanceData_t *instData);
void CsrBtDgSecurityInCfmSend(CsrSchedQid appHandle, CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier);

/* Define cm handlers */
void CsrBtDgInitStateCmRegisterCfmHandler(DgInstanceData_t *instData);
void CsrBtDgInitStateSdsUnregisterCfm(DgInstanceData_t *instData);
void DgIdleStateCmConnectCfmHandler(DgInstanceData_t *instData);
void CsrBtDgIdleStateCmDisconnectIndHandler(DgInstanceData_t *instData);
void CsrBtDgIdleStateDgDeactivateReqHandler(DgInstanceData_t *instData);
void CsrBtDgConnectedStateCmDataIndHandler(DgInstanceData_t *instData);
void CsrBtDgConnectedStateCmDataCfmHandler(DgInstanceData_t *instData);
void CsrBtDgConnectedStateCmControlIndHandler(DgInstanceData_t *instData);
void CsrBtDgConnectedStateCmDisconnectIndHandler(DgInstanceData_t *instData);
void CsrBtDgActivateStateCmCancelAcceptConnectCfmHandler(DgInstanceData_t *instData);
void CsrBtDgActivateStateCmDisconnectIndHandler(DgInstanceData_t *instData);
void DgDeactivateStateCmCancelAcceptConnectCfmHandler(DgInstanceData_t *instData);
void CsrBtDgDeOrActivateStateCmConnectAcceptCfmHandler(DgInstanceData_t *instData);
void CsrBtDgNotConnectedCmDataIndHandler(DgInstanceData_t *instData);
void CsrBtDgConnectedStateCmPortnegIndHandler(DgInstanceData_t *instData);
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
void CsrBtDgModeChangeIndHandler(DgInstanceData_t *instData);
#else
#define CsrBtDgModeChangeIndHandler NULL
#endif
/* Define sdp handlers */
void CsrBtDgInitStateSdsRegisterCfm(DgInstanceData_t *instData);

/* Due to race conditions (during disconnect) just call empty function */
void CsrBtDgIgnoreMessageHandler(DgInstanceData_t *instData);

/* Prototypes from dg_free_down.c */
void CsrBtDgFreeDownstreamMessageContents(CsrUint16 eventClass, void * message);


#ifdef __cplusplus
}
#endif

#endif

#endif
