#ifndef CSR_BT_BSL_UTIL_H__
#define CSR_BT_BSL_UTIL_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_types.h"
#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_bt_result.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_util.h"
#include "csr_exceptionhandler.h"
#include "csr_bt_bsl_main.h"
#include "csr_bt_bnep_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrBtPanMessagePut(CsrSchedQid phandle, void *msg);
void StateChange(BslInstanceData_t *instData, BslState_t newState);

void SendBslHouseCleaning(BslInstanceData_t *instData);
void Go2Idle(BslInstanceData_t *instData);
void CsrBtBslSendSdsRegisterReq(CsrBtBslPanRole role);
CsrBool SdsRoleDeregister(BslInstanceData_t *instData);
CsrBool SdsRoleRegister(BslInstanceData_t *instData);
void CsrBtBslSendCmSdcAttributeReq(BslInstanceData_t *instData);
void CsrBtBslSendBslConnectInd(CsrSchedQid phandle,
                          CsrUint16 id,
                          BD_ADDR_T *remAddr,
                          CsrBtBslPanRole remoteRole,
                          CsrBtBslPanRole localRole,
                          CsrBtResultCode resultCode,
                          CsrBtSupplier resultSupplier);
void CsrBtBslSendBslDisconnectInd(CsrSchedQid appHandle, CsrUint16 id,
    CsrBtReasonCode reasonCode, CsrBtSupplier reasonSupplier);
void CsrBtBslSendBslStatusInd(BslInstanceData_t *instData, CsrUint16 id, CsrBtBslStatus event, CsrUint8 status);
void CsrBtBslSendBslServiceSearchCfm(BslInstanceData_t *instData,
    CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);
void CsrBtBslSendBslServiceSearchResultInd(BslInstanceData_t *instData);
void PutSdsVariables(BslInstanceData_t *instData, CsrUint16 attributeListLength, CsrUint8 * attributeList);

CsrBtBslPanRole CsrBtBslGetRoleFromUuid(CsrUint16 uuid16_t);
CsrUint16 CsrBtBslGetUuidFromRole(CsrBtBslPanRole role);
ETHER_ADDR CsrBtBslConvNativeEther2EtherBig (ETHER_ADDR *nativeAddr);
ETHER_ADDR CsrBtBslConvEtherBig2Native (ETHER_ADDR *etherAddr);

CsrUint8 CsrBtBslFindEmptyIndex(BslInstanceData_t *instData);
CsrUint8 CsrBtBslFindBdAddrIndex(BslInstanceData_t *instData, BD_ADDR_T *bdAddr);
CsrUint8 CsrBtBslFindIdIndex(BslInstanceData_t *instData, CsrUint16 id);
void CsrBtBslClearConnection(BslConnection_t *conn);

#ifdef __cplusplus
}
#endif

#endif
