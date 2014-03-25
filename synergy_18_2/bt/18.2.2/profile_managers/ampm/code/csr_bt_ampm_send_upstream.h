#ifndef CSR_BT_AMPM_SEND_UPSTREAM_H
#define CSR_BT_AMPM_SEND_UPSTREAM_H

#include "csr_synergy.h"
/****************************************************************************

   Copyright Cambridge Silicon Radio Limited and its licensors 2012.
   
   All rights reserved.

REVISION:      $Revision: #3 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void CsrBtAmpmMessagePut(CsrSchedQid phandle, void *msg);

extern void CsrBtAmpmUpstreamRegisterCfm(CsrSchedQid qid,
                                         CsrUint32 eventMask,
                                         CsrBtConnId btConnId,
                                         CsrBtDeviceAddr addr,
                                         CsrBtAmpmResult result);

extern void CsrBtAmpmUpstreamDeregisterCfm(CsrSchedQid qid,
                                           CsrBtConnId btConnId,
                                           CsrBtDeviceAddr addr,
                                           CsrBtAmpmResult result);

extern void CsrBtAmpmUpstreamControllerDone(CsrBtAmpmPrim type,
                                            CsrSchedQid qid,
                                            CsrBtConnId btConnId,
                                            CsrBtDeviceAddr addr,
                                            CsrUint8 ampsCount,
                                            CsrBtAmpmControllerEntry *amps,
                                            CsrBtAmpmResult resultCode);

extern void CsrBtAmpmUpstreamMoveDone(CsrBtAmpmPrim type,
                                      CsrSchedQid qid,
                                      CsrBtConnId btConnId,
                                      CsrBtAmpController localId,
                                      CsrBtAmpmResult resultCode,
                                      CsrBtSupplier resultSupplier);

#define CsrBtAmpmUpstreamMoveInd(_qid, _btConnId, _localId) {           \
        CsrBtAmpmMoveInd *prim__ = (CsrBtAmpmMoveInd*)CsrPmemAlloc(sizeof(CsrBtAmpmMoveInd)); \
        prim__->type = CSR_BT_AMPM_MOVE_IND;                            \
        prim__->btConnId = _btConnId;                                   \
        prim__->localId = _localId;                                     \
        CsrBtAmpmMessagePut(_qid, prim__);}


extern void CsrBtAmpmUpstreamPowerOnInd(CsrSchedQid qid,
                                         CsrBtAmpAmpControllerType ampType,
                                         CsrBtAmpController localId,
                                         CsrBool localInit);
                                 
extern void CsrBtAmpmUpstreamPalRegisterCfm(CsrSchedQid qid,
                                            CsrBtAmpmType ampType,
                                            CsrBtAmpmId ampId,
                                            CsrBtAmpmResult result,
                                            CsrUint32 flags);

#define CsrBtAmpmUpstreamPalDeregisterCfm(_qid, _ampId, _result) {      \
        CsrBtAmpmPalDeregisterCfm *prim__ = (CsrBtAmpmPalDeregisterCfm*)CsrPmemAlloc(sizeof(CsrBtAmpmPalDeregisterCfm)); \
        prim__->type = CSR_BT_AMPM_PAL_DEREGISTER_CFM;                  \
        prim__->ampId = _ampId;                                         \
        prim__->result = _result;                                       \
        prim__->resultSupplier = CSR_BT_SUPPLIER_AMPM;                  \
        CsrBtAmpmMessagePut(_qid, prim__);}

#define CsrBtAmpmUpstreamPalHciCommandInd(_ampId, _data, _qid) {        \
        CsrBtAmpmPalHciCommandInd *prim__ = (CsrBtAmpmPalHciCommandInd*)CsrPmemAlloc(sizeof(CsrBtAmpmPalHciCommandInd)); \
        prim__->type = CSR_BT_AMPM_PAL_HCI_COMMAND_IND;                 \
        prim__->ampId = _ampId;                                         \
        prim__->length = (CsrUint16) ((CsrBtAmpHciNoOperationCmd*) _data)->length; \
        prim__->data = _data;                                           \
        CsrBtAmpmMessagePut(_qid, prim__);}

#define CsrBtAmpmUpstreamTestReceivedSduInd(_qid, _btConnId, _bdAddr, _mblk){ \
        CsrBtAmpmTestReceivedSduInd *prim__ = (CsrBtAmpmTestReceivedSduInd*)CsrPmemAlloc(sizeof(CsrBtAmpmTestReceivedSduInd)); \
        prim__->type = CSR_BT_AMPM_TEST_RECEIVED_SDU_IND;               \
        prim__->btConnId = _btConnId;                                   \
        bd_addr_copy(&prim__->bdAddr, _bdAddr);                         \
        prim__->length = CsrMblkGetLength(_mblk);                       \
        prim__->data = CsrPmemAlloc(prim__->length);                    \
        CsrMblkCopyToMemory(_mblk, 0, prim__->length, prim__->data);    \
        CsrBtAmpmMessagePut(_qid, prim__);}

#define CsrBtAmpmUpstreamTestHciInd(_ampId, _data, _length, _qid) {     \
        CsrBtAmpmTestHciInd *prim__ = (CsrBtAmpmTestHciInd*)CsrPmemAlloc(sizeof(CsrBtAmpmTestHciInd)); \
        prim__->type = CSR_BT_AMPM_TEST_HCI_IND;                        \
        prim__->ampId = _ampId;                                         \
        prim__->length = _length;                                       \
        prim__->data = _data;                                           \
        CsrBtAmpmMessagePut(_qid, prim__);}

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_AMPM_SEND_UPSTREAM_H */
