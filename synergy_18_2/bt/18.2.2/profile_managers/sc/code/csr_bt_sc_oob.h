#ifndef CSR_BT_SC_OOB_H__
#define CSR_BT_SC_OOB_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/
#include "csr_bt_sc_main.h"
#include "hci_prim.h"
#ifdef __cplusplus
extern "C" {
#endif

void CsrBtScReadLocalOobDataReqIdleS(ScInstanceData_t *scData);
void CsrBtScAddRemoteOobDataReqHandler(ScInstanceData_t *scData);
void CsrBtScSmReadLocalOobDataCfmPendingS(ScInstanceData_t *scData);
#define CsrBtScRemoteAuthValid(_remoteAuthRequirements) (_remoteAuthRequirements <= HCI_MITM_PROTECTION_MAX)
CsrBool CsrBtScHasOobDataFor(ScInstanceData_t *scData, CsrBtDeviceAddr deviceAddr);

#ifdef __cplusplus
}
#endif

#endif /* Defined CSR_BT_SC_OOB_H__ */
