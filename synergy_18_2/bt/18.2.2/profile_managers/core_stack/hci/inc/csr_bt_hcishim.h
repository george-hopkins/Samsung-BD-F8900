#ifndef _CSR_BT_HCISHIM_H_
#define _CSR_BT_HCISHIM_H_

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_types.h"
#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "dm_prim.h"
#include "l2cap_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

extern CsrUint8 CsrBtHcishimHciCommandReq(HCI_UPRIM_T *cmd);
extern void CsrBtHcishimAclDataReq(l2ca_controller_t controller,
                                   CsrUint8 physical,
                                   CsrUint16 logical,
                                   CsrMblk *data,
                                   CsrSchedQid qid);
extern void CsrBtHcishimInit(void);
#ifdef ENABLE_SHUTDOWN
extern void CsrBtHcishimDeinit(void);
#endif

#ifdef __cplusplus
}
#endif

#endif
