#ifndef CSR_BT_PHDC_MGR_DEV_SPEC_H__
#define CSR_BT_PHDC_MGR_DEV_SPEC_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

#include "csr_sched.h"
#include "csr_bt_phdc_mgr_main.h"


void DecodeObsevationScanFixed(CsrSchedQid qId,
                               PhdcMgrDeviceListEntry *pDevice,
                               OID_Type eventType,
                               PersonId personId,
                               CsrUint16   objHandle,
                               ObservationScanFixed *pScan);

CsrBool DecodeObsevationScanVar(CsrSchedQid qId,
                             PhdcMgrDeviceListEntry *pDevice,
                             OID_Type eventType,
                             PersonId personId,
                             CsrUint16   objHandle,
                             ObservationScan *pScan);


#ifdef __cplusplus
}
#endif /* extern "C" */


#endif /*  CSR_BT_PHDC_MGR_DEV_SPEC_H */

