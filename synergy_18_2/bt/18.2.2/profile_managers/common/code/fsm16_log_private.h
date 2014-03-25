#ifndef _FSM16_LOG_PRIVATE_H_
#define _FSM16_LOG_PRIVATE_H_

#include "csr_synergy.h"
/*!
        (c) CSR plc 2010
        All rights reserved

\file   fsm16_log_private.h

\brief  Bluestack debug for fsm16
*/

#include "csr_bt_core_stack_fsm.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BLUESTACK_FSM_DEBUG
void bluestack_fsm_log_entry(const FSM_SPARSE16_DEFINITION_T *p_fsm,
                             fsm16_state_t old_state,
                             fsm_event_t event,
                             fsm16_state_t new_state,
                             fsm16_action_t action_index );
void bluestack_fsm_log_exit(const FSM_SPARSE16_DEFINITION_T *p_fsm,
                            fsm_event_t event);
#define FSM_LOG_ENTRY(a,b,c,d,e) \
    (bluestack_fsm_log_entry((a), (b), (c), (d), (e)))
#define FSM_LOG_EXIT(a,b) (bluestack_fsm_log_exit((a), (b)))
#else
#define FSM_LOG_ENTRY(a,b,c,d,e)
#define FSM_LOG_EXIT(a,b)
#endif

#ifdef __cplusplus
}
#endif

#endif

