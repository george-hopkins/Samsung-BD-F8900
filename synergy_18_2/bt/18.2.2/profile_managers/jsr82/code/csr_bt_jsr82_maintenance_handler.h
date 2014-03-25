#ifndef CSR_BT_JSR82_MAINTENANCE_HANDLER_H__
#define CSR_BT_JSR82_MAINTENANCE_HANDLER_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_types.h"
#include "csr_bt_result.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrBtJsr82MessagePut(CsrSchedQid phandle, void *msg);

void CsrBtJsr82SaveMessage(Jsr82MainInstanceData *instData);
void CsrBtJsr82PendingMessage(Jsr82MainInstanceData *instData);
void CsrBtJsr82RestoreSavedMessages(Jsr82MainInstanceData *instData);
void CsrBtJsr82SetBusy(Jsr82MainInstanceData *instData);
void CsrBtJsr82SetDiscovery(Jsr82MainInstanceData *instData);
void CsrBtJsr82SetIdle(Jsr82MainInstanceData *instData);
void CsrBtJsr82SetServSearch(Jsr82MainInstanceData *instData);
void CsrBtJsr82SetUpdatingRecord(Jsr82MainInstanceData *instData);
void CsrBtJsr82SetState(Jsr82MainInstanceData *instData, Jsr82State state);
void CsrBtJsr82SetWriteCod(Jsr82MainInstanceData *instData);
void CsrBtJsr82SetCleanup(Jsr82MainInstanceData *instData);
void CsrBtJsr82SetRdName(Jsr82MainInstanceData *instData);
void CsrBtJsr82SetSelectService(Jsr82MainInstanceData *instData);
void CsrBtJsr82FreeMessage(Jsr82MainInstanceData *instData);
CsrUint16 CsrBtJsr82SetSecurity(CsrUint32 attrs, CsrBool incoming);
void CsrBtJsr82LeaveCleanup(Jsr82MainInstanceData *instData);
void CsrBtJsr82CmDynQueueHandler(Jsr82MainInstanceData *instData);
void CsrBtJsr82SdDynQueueHandler(Jsr82MainInstanceData *instData);
void CsrBtJsr82ScDynQueueHandler(Jsr82MainInstanceData *instData);
void CsrBtJsr82SetEventMaskReqHandler(Jsr82MainInstanceData *instData);

void CsrBtJsr82PropgateRfcDisconnectIndEvent(Jsr82MainInstanceData *instData,
    CsrUint8 serverChannel, CsrBtResultCode result,
    CsrBool localTerminated);

void CsrBtJsr82PropgateL2caDisconnectEvent(Jsr82MainInstanceData *instData,
    CsrBtConnId btConnId, CsrBtResultCode result,
    CsrBool localTerminated);

void CsrBtJsr82SendCmMsgDown(Jsr82MainInstanceData *instData,
                        CsrBtJsr82Prim **orgMsg,
                        void *reqMsg,
                        const jsr82DynSignalHandler *signalHandlerList);
void CsrBtJsr82SendSdMsgDown(Jsr82MainInstanceData *instData,
                        CsrBtJsr82Prim **orgMsg,
                        void *reqMsg,
                        const jsr82DynSignalHandler *signalHandlerList);
void CsrBtJsr82SendScMsgDown(Jsr82MainInstanceData *instData,
                        CsrBtJsr82Prim **orgMsg,
                        void *reqMsg,
                        const jsr82DynSignalHandler *signalHandlerList);


#ifdef __cplusplus
}
#endif

#endif
