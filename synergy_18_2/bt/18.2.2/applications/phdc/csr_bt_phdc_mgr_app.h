#ifndef CSR_BT_PHDC_MGR_APP_H__
#define CSR_BT_PHDC_MGR_APP_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_bt_phdc_mgr_app_main.h"


#ifdef __cplusplus
extern "C" {
#endif

void CsrBtPhdcAppTimedEvent(CsrUint16 event, void *pv_arg1);
void CsrBtPhdcAppMgrIfPhdcHandler(PhdcAppMgrInstance *pInstance);

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_PHDC_MGR_APP_H__ */


