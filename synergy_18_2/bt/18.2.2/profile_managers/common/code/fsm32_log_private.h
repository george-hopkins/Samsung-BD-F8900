#ifndef _FSM32_LOG_PRIVATE_H__
#define _FSM32_LOG_PRIVATE_H__

#include "csr_synergy.h"
/*!
        (c) CSR plc 2010
        All rights reserved

\file   fsm32_log_private.h

\brief  Bluestack debug for fsm32
*/

#include "csr_bt_core_stack_fsm.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(BLUESTACK_FSM_DEBUG) && defined(BUILD_FOR_HOST)
void bluestack_fsm32_log_entry(const FSM_SPARSE32_DEFINITION_T *p_fsm,
                               fsm32_state_t old_state,
                               fsm_event_t event,
                               fsm32_state_t new_state,
                               fsm32_action_t action_index );
void bluestack_fsm32_log_exit(const FSM_SPARSE32_DEFINITION_T *p_fsm,
                            fsm_event_t event);
#define FSM32_LOG_ENTRY(a,b,c,d,e) \
    (bluestack_fsm32_log_entry((a), (b), (c), (d), (e)))
#define FSM32_LOG_EXIT(a,b) \
    (bluestack_fsm32_log_exit((a), (b)))
#else

/* Never use 32 bit extended logging on-chip */
#define FSM32_LOG_ENTRY(a,b,c,d,e)
#define FSM32_LOG_EXIT(a,b)
#endif

#ifdef __cplusplus
}
#endif

#endif
