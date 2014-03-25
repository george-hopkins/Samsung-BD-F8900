#ifndef CSR_BT_SC_DEBOND_H__
#define CSR_BT_SC_DEBOND_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include "csr_types.h"
#include "csr_bt_sc_main.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrBtScDebondReqIdleS(ScInstanceData_t *scData);
void CsrBtScDmHciDeleteStoredLinkKeyCompletePendingS(ScInstanceData_t *scData);
void CsrBtScDmRemoveDeviceCfmPendingS(ScInstanceData_t *scData);

#ifdef __cplusplus
}
#endif

#endif /* Defined CSR_BT_SC_DEBOND_H__ */

