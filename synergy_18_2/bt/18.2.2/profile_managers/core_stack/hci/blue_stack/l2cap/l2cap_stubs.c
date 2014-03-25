/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2cap_stubs.c

\brief  If an L2CAP feature is disabled, but the FSM still contain
        references to the feature's FSM event function, we need to
        provide a stub for this function to keep linkers happy.
*/

#include "csr_synergy.h"

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "l2cap_cid.h"
#include "l2cap_cid_fsm_definitions.h"

#ifdef BUILD_FOR_HOST
#include "csr_macro.h"
#endif

/*!\ brief All stubs are the same, so use a macro */
#define L2CAP_STUB \
    {                                           \
        PARAM_UNUSED(arg1);                     \
        PARAM_UNUSED(arg2);                     \
        return CID_EVENT_NULL;                  \
    }

/*! \brief L2CAP AMP stubs */
#if !defined(INSTALL_AMP_SUPPORT) && !defined(DISABLE_AMP_FSM_SUPPORT)
fsm_event_t CID_FsmAmpCreateInd(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmAmpCreateIndPhysOk(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmAmpCreateIndPhysNok(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmAmpCreateReq(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmAmpCreateReqPhysOk(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmAmpCreateReqPhysNok(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmAmpCreateReqVerifyOk(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmAmpMoveReqPhysOk(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmAmpMoveReqPhysNok(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmAmpMoveReqVerifyOk(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmAmpCreateCfmOk(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmAmpCreateCfmPnd(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmAmpCreateCfmNok(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmAmpCreateCfmError(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmAmpCreateCfmErrorSec(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmAmpCreateCfmTimeout(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmAmpCreateCfmAbort(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmAmpCreateSmCfmOk(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmAmpCreateSmCfmNok(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmAmpCreateRspPnd(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmAmpCreateRspNok(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmAmpCreateRspOk(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmAmpCreateIndRetry(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmAmpCreateIndRetryActive(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmAmpMoveReq(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmAmpMoveInd(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmAmpMoveIndPhysOk(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmAmpMoveIndPhysNok(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmAmpMoveIndCollide(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmAmpMoveRspOk(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmAmpMoveRspNok(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmAmpMoveRemLogicalCfmOk(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmAmpMoveRemLogicalCfmNok(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmAmpMoveCfmNok(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmAmpMoveCfmPnd(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmAmpMoveCfmOk(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmAmpMoveLocLogicalCfmOk(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmAmpMoveLocLogicalCfmNok(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmAmpMoveCfmTimeout(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmAmpMoveReqBusy(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmAmpMoveIndBusy(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmAmpConfigLogicalCfmOk(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmAmpConfigLogicalCfmNok(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmAmpConfirmInd(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmAmpConfirmCfm(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmAmpDisconnectInd(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmAmpCleanupReq(void *arg1, void *arg2) L2CAP_STUB
#endif

/*! \brief L2CAP fixed channel stubs */
#if !defined(INSTALL_L2CAP_FIXED_CHANNEL_BASE_SUPPORT) && !defined(DISABLE_L2CAP_FIXED_CHANNEL_FSM_SUPPORT)
fsm_event_t CID_FsmMapFixedCidReq(void *arg1, void *arg2) L2CAP_STUB
fsm_event_t CID_FsmMapFixedCidInd(void *arg1, void *arg2) L2CAP_STUB
#endif
