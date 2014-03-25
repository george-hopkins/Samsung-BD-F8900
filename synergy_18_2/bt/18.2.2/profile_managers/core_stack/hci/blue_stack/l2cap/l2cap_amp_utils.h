/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2cap_amp_utils.h

\brief  Different AMP queue and utility functions

*/
#ifndef _L2CAP_AMP_UTILS_
#define _L2CAP_AMP_UTILS_

#include "csr_synergy.h"
#ifdef INSTALL_AMP_SUPPORT

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "l2cap_prim.h"
#include "csr_mblk.h"
#include "csr_bt_common.h"
#include "l2cap_config.h"
#include "l2cap_control.h"
#include "l2cap_cid.h"
#include "dm_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Sanity checking for AMP event functions etc. */
#ifdef INSTALL_L2CAP_DEBUG
#define AMP_InfoSanity(cidcb) {if(!CID_IsAmp((cidcb))) { BLUESTACK_PANIC(CSR_BT_PANIC_MYSTERY); }}
#else
#define AMP_InfoSanity(cidcb)
#endif

#define AMP_GetState(cidcb) ((cidcb)->amp_info->amp_state)
#define AMP_SetState(cidcb, state) ((cidcb)->amp_info->amp_state = (state))

/* Instance data handling */
extern void AMP_AllocInfo(CIDCB_T *cidcb);
extern void AMP_FreeInfo(CIDCB_T *cidcb);

/* Generic AMP information handling */
extern void AMP_RevertQueue(CIDCB_T *cidcb);
extern void AMP_FinaliseSetup(CIDCB_T *cidcb);
extern void AMP_SwapControllerIds(CIDCB_T *cidcb);
extern void AMP_DisconnectReq(CIDCB_T *cidcb, CsrBool isnew);
extern LOGIC_Q_LOOKUP_T **AMP_LogicalLookup(amp_link_id_t logical_handle);

/* FSM helper functions */
extern void AMP_KickLogicalChannelReq(CIDCB_T *cidcb);
extern void AMP_KickConfirmationReq(CIDCB_T *cidcb);

/* Transmit queue handling */
extern void AMP_PruneQueues(L2CAP_CHCB_T *chcb);
extern TXQUEUE_T *AMP_GetTxQueue(CIDCB_T *cidcb);
extern TXQUEUE_T *AMP_CreateQueue(CIDCB_T *cidcb, DM_AMPM_CONNECT_CHANNEL_RSP_T *prim);
extern CsrBool AMP_LinkQueue(CIDCB_T *cidcb,
                            amp_link_id_t logical);
#ifdef __cplusplus
}
#endif
#endif /* AMP_SUPPORT */
#endif /* AMP_UTILS */

