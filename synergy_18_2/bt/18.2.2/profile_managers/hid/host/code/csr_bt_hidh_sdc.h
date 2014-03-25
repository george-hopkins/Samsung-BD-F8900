#ifndef CSR_BT_HIDH_SDC_H__
#define CSR_BT_HIDH_SDC_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include "csr_bt_hidh_main.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrBtHidhCmSdcSearchInd(hidh_instData_t *instData);
void CsrBtHidhCmSdcSearchCfm(hidh_instData_t *instData);
void CsrBtHidhCmSdcAttributeCfm(hidh_instData_t *instData);
void CsrBtHidhCmSdcCloseInd(hidh_instData_t *instData);

#ifdef __cplusplus
}
#endif

#endif /* _HIDH_SDC_H */
