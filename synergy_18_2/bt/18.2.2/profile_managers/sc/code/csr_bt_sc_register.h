#ifndef CSR_BT_SC_REGISTER_H__
#define CSR_BT_SC_REGISTER_H__

#include "csr_synergy.h"
/****************************************************************************

Copyright Cambridge Silicon Radio Limited and its licensors 2011. All rights reserved.

REVISION:      $Revision: #2 $
****************************************************************************/
#include "csr_bt_sc_main.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrBtScEncryptionKeySizeReqHandler(ScInstanceData_t *scData);
void CsrBtScSetSecurityModeReqIdleS(ScInstanceData_t *scData);
void CsrBtScDmSmInitCfmPendingS(ScInstanceData_t *scData);
void CsrBtScRegisterReqAnyS(ScInstanceData_t *scData);
void CsrBtScDeregisterReqAnyS(ScInstanceData_t *scData);
void CsrBtScReadDeviceRecordReqHandler(ScInstanceData_t *scData);
void CsrBtScUpdateDeviceReqHandler(ScInstanceData_t *scData);
void getUuidOfRegisteredChannel(ScInstanceData_t *scData, CsrUint16 * service);
void CsrBtScConfigReqHandler(ScInstanceData_t *scData);
CsrUint8 CsrBtScGetCompatibleAuthRequirement(ScInstanceData_t *scData);
void CsrBtScCmEncryptionKeySizeCfmHandler(ScInstanceData_t *scData);
#ifdef CSR_BT_LE_ENABLE
void CsrBtScSetEncryptionKeySizeReqHandler(ScInstanceData_t *scData);
#endif
CsrUint8 CsrBtScGetHighestAuthRequirement(ScInstanceData_t *scData);
#ifdef __cplusplus
}
#endif

#endif /* ndef */
