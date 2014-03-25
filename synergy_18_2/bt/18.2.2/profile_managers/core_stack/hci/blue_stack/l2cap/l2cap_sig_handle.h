/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2cap_sig_handle.h

\brief  This file defines the prototypes et.al. for the L2CAP signal
        handlers.
*/

#ifndef _L2CAP_SIG_HANDLE_H
#define _L2CAP_SIG_HANDLE_H

#include "csr_synergy.h"

#include "csr_mblk.h"
#include "csr_bt_common.h"
#include "l2cap_signal.h"
#include "dm_acl_core.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void SIGH_DuplicateSetResponse(L2CAP_CHCB_T *chcb, SIG_SIGNAL_T *signal);
extern void SIGH_SignalSend(L2CAP_CHCB_T *chcb, SIG_SIGNAL_T *signal_block);
extern void SIGH_SignalPresend(L2CAP_CHCB_T *chcb, TXQE_T *txqe);
extern void SIGH_SignalQueueEmptyWithCid(SIG_SIGNAL_T **sig_list, l2ca_cid_t cid);
extern void SIGH_SignalQueueEmpty(SIG_SIGNAL_T **sig_list, const BD_ADDR_T *const p_bd_addr);
extern void SIGH_SignalReceive(L2CAP_CHCB_T *chcb, CsrMblk **mblk, CsrUint16 signal_size);
extern void SIGH_SignalMtuExceeded(L2CAP_CHCB_T *chcb, CsrMblk *mblk, CsrUint16 signal_size);

#ifdef INSTALL_ULP
extern void SIGH_LeSignalReceive(L2CAP_CHCB_T *chcb, CsrMblk **mblk, CsrUint16 signal_size);
#endif

#ifdef __cplusplus
}
#endif
#endif
