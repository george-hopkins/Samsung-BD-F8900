#ifndef CSR_BT_SC_AUTHORISE_H__
#define CSR_BT_SC_AUTHORISE_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include "csr_bt_sc_main.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrBtScDmSmAuthoriseIndIdleS(ScInstanceData_t *scData);
void CsrBtScSetTrustLevelReqAnyS(ScInstanceData_t *scData);
void CsrBtScAuthoriseResAuthoriseS(ScInstanceData_t *scData);
CsrBool CsrBtScCancelQueuedAuthoriseReq(ScInstanceData_t *scData);

#ifdef __cplusplus
}
#endif

#endif
