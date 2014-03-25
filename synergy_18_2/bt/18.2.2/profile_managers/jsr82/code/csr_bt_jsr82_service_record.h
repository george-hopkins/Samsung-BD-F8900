#ifndef CSR_BT_JSR82_SERVICE_RECORD_H__
#define CSR_BT_JSR82_SERVICE_RECORD_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_bt_jsr82_main.h"

#define JSR82_SR_DELAY 500000
#define JSR82_SR_RETRIES 10

#ifdef __cplusplus
extern "C" {
#endif

void CsrBtJsr82SrCreateRecordHandle(Jsr82MainInstanceData *instData);
void CsrBtJsr82SrRegisterReq(Jsr82MainInstanceData *instData);
void CsrBtJsr82SendSrRegisterRecordCfm(Jsr82MainInstanceData *instData);
void CsrBtJsr82SrUnRegisterReq(Jsr82MainInstanceData *instData);
void CsrBtJsr82SendSrRemoveRecordCfm(Jsr82MainInstanceData *instData);
void CsrBtJsr82StartLdUpdateRecord(Jsr82MainInstanceData *instData);
void CsrBtJsr82MidLdUpdateRecord(Jsr82MainInstanceData *instData);
void CsrBtJsr82EndLdUpdateRecord(Jsr82MainInstanceData *instData);
void CsrBtJsr82SrUpdateRegisterReq(Jsr82MainInstanceData *instData);
void CsrBtJsr82SrUpdateUnRegisterReq(Jsr82MainInstanceData *instData);
void CsrBtJsr82SrCleanupUnregisterCfm(Jsr82MainInstanceData *instData);

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_JSR82_SERVICE_RECORD_H__ */
