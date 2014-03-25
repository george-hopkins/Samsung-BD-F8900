#ifndef CSR_BT_HIDH_CON_H__
#define CSR_BT_HIDH_CON_H__

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

void CsrBtHidhConnectReqHandler(hidh_instData_t *instData);
void CsrBtHidhConnectAcceptReqHandler(hidh_instData_t *instData);
void CsrBtHidhCancelConnectAcceptReqHandler(hidh_instData_t *instData);
void CsrBtHidhDisconnectReqHandler(hidh_instData_t *instData);
void CsrBtHidhRegisterUserReqHandler(hidh_instData_t *instData);
void CsrBtHidhSecurityInReqHandler(hidh_instData_t *instData);
void CsrBtHidhSecurityOutReqHandler(hidh_instData_t *instData);

void CsrBtHidhCmL2caDisconnectInd(hidh_instData_t *instData);
void CsrBtHidhCmL2caConnectCfm(hidh_instData_t *instData);
void CsrBtHidhCmL2caConnectAcceptCfm(hidh_instData_t *instData);
void CsrBtHidhCmL2caCancelConnectAcceptCfm(hidh_instData_t *instData);
void cmL2caModeChangeInd(hidh_instData_t *instData);
void CsrBtHidhCmL2caModeChangeInd(hidh_instData_t *instData);

#ifdef __cplusplus
}
#endif

#endif /* _HIDH_CON_H */
