#ifndef CSR_BT_SC_ENCRYPT_H__
#define CSR_BT_SC_ENCRYPT_H__

#include "csr_synergy.h"
/****************************************************************************

                (c) CSR plc 2008

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include "csr_bt_sc_main.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrBtScEncryptionReqIdleS(ScInstanceData_t *scData);
void CsrBtScDmSmAuthenticateCfmEncryptionS(ScInstanceData_t *scData);
void CsrBtScDmEncryptCfmEncryptionS(ScInstanceData_t *scData);

#ifdef __cplusplus
}
#endif

#endif /* Defined CSR_BT_SC_ENCRYPT_H__ */
