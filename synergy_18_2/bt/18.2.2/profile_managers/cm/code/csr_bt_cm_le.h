#ifndef _CSR_BT_CM_LE_H__
#define _CSR_BT_CM_LE_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #4 $
****************************************************************************/

#include "csr_bt_cm_main.h"
#include "csr_bt_cm_dm.h"
#include "csr_bt_cm_prim.h"
#include "dm_prim.h"
#include "dmlib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_cm_util.h"
#include "csr_bt_cm_callback_q.h"
#include "csr_bt_cm_events_handler.h"

#ifdef CSR_BT_LE_ENABLE
#ifdef __cplusplus
extern "C" {
#endif

void CsrBtCmLeGapRegisterReqHandler(cmInstanceData_t *cmData);
void CsrBtCmLeScanReqHandler(cmInstanceData_t *cmData);
void CsrBtCmLeAdvertiseReqHandler(cmInstanceData_t *cmData);
void CsrBtCmLeWhitelistSetReqHandler(cmInstanceData_t *cmData);
void CsrBtCmLeConnparamReqHandler(cmInstanceData_t *cmData);
void CsrBtCmLeConnparamUpdateReqHandler(cmInstanceData_t *cmData);
void CsrBtCmLeAcceptConnparamUpdateResHandler(cmInstanceData_t *cmData);
void CsrBtCmLeReceiverTestReqHandler(cmInstanceData_t *cmData);
void CsrBtCmLeTransmitterTestReqHandler(cmInstanceData_t *cmData);
void CsrBtCmLeTestEndReqHandler(cmInstanceData_t *cmData);

void CsrBtCmLeReportIndHandler(cmInstanceData_t *cmInst,
                               DM_HCI_ULP_ADVERTISING_REPORT_IND_T *report);
void CsrBtCmLeConnectionUpdateCmpIndHandler(cmInstanceData_t *cmInst,
                                            DM_HCI_ULP_CONNECTION_UPDATE_COMPLETE_IND_T* dmPrim);
void CsrBtCmLeAclConnHandleIndHandler(cmInstanceData_t *cmData);
void CsrBtCmLeAcceptConnparamUpdateIndHandler(cmInstanceData_t *cmData);
void CsrBtCmLeAclClosedIndHandler(cmInstanceData_t *cmData);
void CsrBtCmLePhysicalLinkStatusReqHandler(cmInstanceData_t *cmData);
void CsrBtCmLeDeinit(cmInstanceData_t *cmData);

#ifdef __cplusplus
}
#endif
#endif /* CSR_BT_LE_ENABLE */
#endif /* _CSR_BT_CM_LE_H__ */
