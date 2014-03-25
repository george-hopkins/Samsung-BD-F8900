#ifndef CSR_BT_SC_LE_H__
#define CSR_BT_SC_LE_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#ifdef CSR_BT_LE_ENABLE

#include "csr_types.h"
#include "csr_bt_profiles.h"
#include "dm_prim.h"
#include "csr_bt_usr_config.h"
#include "csr_message_queue.h"
#include "csr_exceptionhandler.h"
#include "csr_bt_sc_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Utils */
void CsrBtScLeReadRemoteName(ScInstanceData_t *scData);
void CsrBtScLeReadRemoteNameCancel(ScInstanceData_t *scData);
void CsrBtScLeSspComplete(ScInstanceData_t *scData);
void CsrBtScLeUpdateLocalDbKeys(DM_SM_INIT_CFM_T *init);
void CsrBtScDmLeRemoveDeviceCfmRebondS(ScInstanceData_t *scData);

/* Downstream */
void CsrBtScLeSecurityReqHandler(ScInstanceData_t *scData);
void CsrBtScLeKeyDistributionReqHandler(ScInstanceData_t *scData);
void CsrBtScLeAddServiceChangedReqHandler(ScInstanceData_t *scData);

/* Upstream */
void CsrBtScLeCsrkCounterChangeIndHandler(ScInstanceData_t *scData);
void CsrBtScLeEncryptionChangeHandler(ScInstanceData_t *scData);
void CsrBtScLeSecurityCfmHandler(ScInstanceData_t *scData);
void CsrBtScLeSecurityIndHandler(ScInstanceData_t *scData);
void CsrBtScSmKeyRequestIndHandler(ScInstanceData_t *scData);
void CsrBtScSmKeysIndHandler(ScInstanceData_t *scData);
void CsrBtScGattHandler(ScInstanceData_t *scData);
void CsrBtScCmLeEventConnectionIndHandler(ScInstanceData_t *scData);

#ifdef __cplusplus
}
#endif
#endif /* CSR_BT_LE_ENABLE */
#endif /* CSR_BT_SC_LE_H__ */
