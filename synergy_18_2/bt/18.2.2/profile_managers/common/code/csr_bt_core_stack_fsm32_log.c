/*!
        Copyright (C) CCL, CSR plc 2008
        All rights reserved

\file   csr_bt_core_stack_fsm32_log.c

\brief  Bluestack debug for fsm32
*/

#include "csr_synergy.h"

#include "csr_bt_core_stack_pmalloc.h"
#include "csr_bt_bluestack_types.h"
#include "fsm32_log_private.h"

#ifdef CSR_LOG_ENABLE
void bluestack_fsm32_log_entry(const FSM_SPARSE32_DEFINITION_T *p_fsm,
        fsm32_state_t old_state,
        fsm_event_t event,
        fsm32_state_t new_state,
        fsm32_action_t action_index )
{
    const char * eventname = NULL;
    bitmask16_t mask = CSR_LOG_STATE_TRANSITION_MASK_FSM_NAME |
                       CSR_LOG_STATE_TRANSITION_MASK_NEXT_STATE |
                       CSR_LOG_STATE_TRANSITION_MASK_NEXT_STATE_STR |
                       CSR_LOG_STATE_TRANSITION_MASK_PREV_STATE |
                       CSR_LOG_STATE_TRANSITION_MASK_PREV_STATE_STR |
                       CSR_LOG_STATE_TRANSITION_MASK_EVENT |
                       CSR_LOG_STATE_TRANSITION_MASK_EVENT_STR;

    eventname = event != FSM32_EVENT_NULL ? p_fsm->event_names[event] : "FSM32_EVENT_NULL";

    CsrLogStateTransition(mask,
                          action_index,
                          (const CsrCharString *)(p_fsm->fsm_name),
                          old_state,
                          (const CsrCharString *)(p_fsm->state_names[old_state]),
                          event,
                          (const CsrCharString *)eventname,
                          new_state,
                          (const CsrCharString *)(p_fsm->state_names[new_state]),
                          __LINE__,
                          (const CsrCharString *)__FILE__);
}

void bluestack_fsm32_log_exit(const FSM_SPARSE32_DEFINITION_T *p_fsm,
        fsm_event_t event)
{
    CSR_UNUSED(p_fsm);
    CSR_UNUSED(event);
}
#endif
