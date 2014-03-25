#ifndef CSR_BT_HFHS_DATA_SEF_H__
#define CSR_BT_HFHS_DATA_SEF_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "csr_types.h"
#include "csr_bt_result.h"

HfHsData_t *getDataInstance(HfMainInstanceData_t * instData,
                            CsrUint32 theHfConnId);

void CsrBtHfHsXStateCmDataIndHandler(HfMainInstanceData_t *instData);
void CsrBtHfHsXStateCmDataCfmHandler(HfMainInstanceData_t *instData);
void CsrBtHfHsSendCmDataReq(HfMainInstanceData_t *instData,
                       CsrUint16 payloadLen,
                       CsrUint8 *payload);
void CsrBtHfSecurityInCfmSend(CsrSchedQid appHandle, CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier);
void CsrBtHfSecurityOutCfmSend(CsrSchedQid appHandle, CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier);
HfLastAtCmdSent_t FindCurrentCmdFromPayload(CsrUint8 *string);

#ifdef __cplusplus
}
#endif

#endif
