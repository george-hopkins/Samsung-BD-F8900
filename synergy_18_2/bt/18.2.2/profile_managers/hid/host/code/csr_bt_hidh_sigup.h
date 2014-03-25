#ifndef CSR_BT_HIDH_SIGUP_H__
#define CSR_BT_HIDH_SIGUP_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "csr_bt_hidh_prim.h"

void CsrBtHidhConnectCfmSend(CsrSchedQid appHandle,
                             CsrBtResultCode resultCode,
                             CsrBtSupplier resultSupplier,
                             CsrBtDeviceAddr bdAddr,
                             CsrUint8 deviceId,
                             hidh_subInstData_t *subInst,
                             CsrBtConnId btConnId);
void CsrBtHidhConnectAcceptCfmSend(CsrSchedQid appHandle,
                                   CsrBtResultCode resultCode,
                                   CsrBtSupplier resultSupplier,
                                   CsrBtDeviceAddr bdAddr,
                                   CsrUint8 deviceId,
                                   CsrBtConnId btConnId);
void CsrBtHidhConnectAcceptIndSend(CsrSchedQid appHandle,
                                            CsrBtResultCode resultCode,
                                            CsrBtSupplier resultSupplier,
                                            CsrUint8 deviceId);
void CsrBtHidhCancelConnectAcceptCfmSend(CsrSchedQid appHandle,
                                         CsrUint8 deviceId,
                                         CsrBtResultCode resultCode,
                                         CsrBtSupplier resultSupplier);
void CsrBtHidhDisconnectCfmSend(CsrSchedQid appHandle,
                                        CsrBtResultCode resultCode,
                                        CsrBtSupplier resultSupplier,
                                        CsrUint8 deviceId);
void CsrBtHidhDisconnectIndSend(CsrSchedQid appHandle,
                                       CsrBtReasonCode reasonCode,
                                       CsrBtSupplier reasonSupplier,
                                       CsrUint8 deviceId);
void CsrBtHidhControlIndSend(CsrSchedQid appHandle,
                        CsrUint8 deviceId,
                        CsrBtHidhCtrl operation);
void CsrBtHidhHandshakeIndSend(CsrSchedQid appHandle,
                                       CsrUint8 deviceId,
                                       CsrBtResultCode resultCode);
void CsrBtHidhStatusIndSend(CsrSchedQid appHandle,
                       CsrBtHidhStatus status,
                       CsrUint8 deviceId,
                       hidh_subInstData_t *subInst);
void CsrBtHidhRegisterUserCfmSend(CsrSchedQid appHandle,
                             CsrUint8 slotId,
                             CsrBtReasonCode reasonCode,
                             CsrBtSupplier reasonSupplier);
void CsrBtHidhDataIndSend(CsrSchedQid appHandle,
                     CsrUint8 deviceId,
                     CsrUint16 dataLen,
                     CsrUint8 *data);
void CsrBtHidhSecurityInCfmSend(CsrSchedQid appHandle, CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier);
void CsrBtHidhSecurityOutCfmSend(CsrSchedQid appHandle, CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier);

#ifdef __cplusplus
}
#endif

#endif

