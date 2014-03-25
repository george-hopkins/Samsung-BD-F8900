#ifndef CSR_BT_SC_BOND_LEGACY_H__
#define CSR_BT_SC_BOND_LEGACY_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/
#include "csr_types.h"
#include "csr_bt_profiles.h"

#include "csr_bt_sc_main.h"

#ifdef __cplusplus
extern "C" {
#endif

CsrBool CsrBtScAclPresent(ScInstanceData_t *scData, BD_ADDR_T bdAddr, CsrUintFast8 *index);
void CsrBtScAddAcl2List(ScInstanceData_t *scData, BD_ADDR_T bdAddr, CsrBtClassOfDevice cod);
void CsrBtScRemoveAclFromList(ScInstanceData_t *scData, BD_ADDR_T bdAddr);

void CsrBtScStartRemoteBondTimer(ScInstanceData_t *scData);

void CsrBtScRemotePairingFailed(ScInstanceData_t *scData);
void CsrBtScCmAclConnectIndAnyS(ScInstanceData_t *scData);
void CsrBtScCmAclConnectIndRemoteBondingS(ScInstanceData_t *scData);
void CsrBtScCmAclDisconnectIndAnyS(ScInstanceData_t *scData);
void CsrBtScCmAclDisconnectIndRemoteBondingS(ScInstanceData_t *scData);

void CsrBtScCmConnectionRejSecurityIndRemoteBondingS(ScInstanceData_t *scData);
void CsrBtScDmAccessIndRemoteBondingS(ScInstanceData_t *scData);
void CsrBtScDmHciCreateConnectionCancelCompleteRemoteBonding(ScInstanceData_t *scData);

#ifdef __cplusplus
}
#endif

#endif
