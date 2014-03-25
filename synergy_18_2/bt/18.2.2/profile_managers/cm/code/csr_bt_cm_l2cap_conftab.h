#ifndef _CSR_BT_CM_PRIVATE_CONFTAB_H__
#define _CSR_BT_CM_PRIVATE_CONFTAB_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_types.h"
#include "hci_prim.h"
#include "csr_bt_cm_prim.h"
#include "dm_prim.h"
#include "l2cap_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BKV_SEP_END(ref) (BKV_BARRIER | ((ref) & BKV_MASK_VALUE))
#define CSR_BT_CM_L2CA_AUTOPT_MAX_SIZE 100

extern void CsrBtCmL2caConftabInit(CsrUint16 **pConfTab, CsrUint16 *conftabCount);
extern void CsrBtCmL2caConftabCull(CsrUint16 **pConfTab, CsrUint16 *pConftabCount);
extern void CsrBtCmL2caConftabEnlarge(CsrUint16 **pConfTab, CsrUint16 *pConftabCount);

extern void CsrBtCmL2caConftabMtu(CsrUint16 *confTab, CsrUint16 *conftabCount, CsrBool incoming, l2ca_mtu_t mtu);
extern void CsrBtCmL2caConftabMps(CsrUint16 *confTab, CsrUint16 *conftabCount, CsrBool incoming, l2ca_mtu_t min, l2ca_mtu_t max);
extern void CsrBtCmL2caConftabExtendedFeatures(CsrUint16 *confTab, CsrUint16 *conftabCount, CsrUint32 extFeat);
extern void CsrBtCmL2caConftabFlushTo(CsrUint16 *confTab, CsrUint16 *conftabCount, CsrBool incoming, CsrUint16 flushTo);
extern void CsrBtCmL2caConftabFlushToAllowAnyPeer(CsrUint16 *confTab, CsrUint16 *conftabCount);
extern void CsrBtCmL2caConftabTxwinAllowAnyPeer(CsrUint16 *confTab, CsrUint16 *conftabCount);
extern void CsrBtCmL2caConftabFlow(CsrUint16 *confTab, CsrUint16 *conftabCount, L2CA_FLOW_T *flow, CsrBool allowFallback);
#ifdef CSR_AMP_ENABLE
extern void CsrBtCmL2caConftabFcs(CsrUint16 *confTab, CsrUint16 *conftabCount, CsrBool useFcs);
#endif
extern void CsrBtCmL2caConftabHighDataPriority(CsrUint16 *confTab, CsrUint16 *conftabCount, CsrUint16 highPrio);
extern void CsrBtCmL2caConftabNoReconf(CsrUint16 *confTab, CsrUint16 *conftabCount);
extern void CsrBtCmL2caConftabFinalize(CsrUint16 *confTab, CsrUint16 *conftabCount);


#ifdef __cplusplus
}
#endif

#endif
