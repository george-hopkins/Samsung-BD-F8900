#ifndef CSR_BT_HIDH_DATA_H__
#define CSR_BT_HIDH_DATA_H__

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

void CsrBtHidhControlReqHandler(hidh_instData_t *instData);
void CsrBtHidhGetReportReqHandler(hidh_instData_t *instData);
void CsrBtHidhSetReportReqHandler(hidh_instData_t *instData);
void CsrBtHidhGetProtocolReqHandler(hidh_instData_t *instData);
void CsrBtHidhSetProtocolReqHandler(hidh_instData_t *instData);
void CsrBtHidhGetIdleReqHandler(hidh_instData_t *instData);
void CsrBtHidhSetIdleReqHandler(hidh_instData_t *instData);
void CsrBtHidhDataReqHandler(hidh_instData_t *instData);

void CsrBtHidhCmL2caDataInd(hidh_instData_t *instData);
void CsrBtHidhCmL2caDataCfm(hidh_instData_t *instData);

#ifdef __cplusplus
}
#endif

#endif /* _HIDH_DATA_H */
