#ifndef CSR_BT_HIDH_UTIL_H__
#define CSR_BT_HIDH_UTIL_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_bt_hidh_main.h"

#ifdef __cplusplus
extern "C" {
#endif

CsrSchedQid getSlotHandle(hidh_instData_t *instData, CsrUint8 slot);
CsrUint8   getNewSubInst(hidh_instData_t *instData);
void      freeSubInst(hidh_instData_t *instData, CsrUint8 deviceId);
CsrUint8   getDeviceIdFromCid(CsrBtConnId cid, hidh_instData_t *instData);
CsrUint8   getDeviceIdFromBdAddr(CsrBtDeviceAddr bdAddr, hidh_instData_t *instData);
CsrUint8   getDeviceIdFromState(CsrUint32 state, hidh_instData_t *instData);

/* Prototypes from hidh_free_down.c */
void CsrBtHidhFreeDownstreamMessageContents(CsrUint16 eventClass, void * message);

#ifdef __cplusplus
}
#endif

#endif


