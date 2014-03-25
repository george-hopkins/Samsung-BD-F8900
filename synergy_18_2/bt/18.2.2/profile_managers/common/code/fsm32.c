/*!
        (c) CSR plc 2010
        All rights reserved

\file   fsm32.c

\brief  The 32bit version of the FSM.
*/

#include "csr_synergy.h"

#include "fsm_private.h"
#include "fsm32_log_private.h"

/*! \brief fsm_32bit_run

    The engine function for a 32 bit bitfield sparse state machine.
    Adheres to the "shake it until it stops rattling" principle,
    driving the state machine until there are no more events.

   \returns  fsm_result_t - FSM_RESULT_OK if the event was processed
   FSM_RESULT_FAIL otherwise
*/
fsm_result_t fsm_32bit_run(
    const FSM_SPARSE32_DEFINITION_T *p_fsm, /* The FSM definition */
    fsm32_state_t *p_state,                 /* State variable */
    fsm_event_t event,                      /* Event to process */
    void *pv_arg1,                          /* Two void pointer arguments */
    void *pv_arg2,
    fsm_logging_t log                       /* Logging control */
    )
{
    const FSM_SPARSE32_TRANSITION_T *p_transition, *p_end_trans;
    CsrUint16 action_index;
#ifdef CSR_FSM_LOG
    CsrUint16 oldstate = *p_state ;
#endif

#ifdef FSM_POINTER_CHECK
    /* Make sure p_state is in RAM as we need to write to it */
    if ((uintptr) p_state < (uintptr) RAM_START)
    panic(PANIC_FSM_BAD_POINTER);
#endif

    while (event != FSM32_EVENT_NULL)
    {
        CsrUint16 orig_state;
#ifndef BUILD_FOR_HOST
        /* B-39952 Caution shared will be called from multiple places */
        patch_fn(fsm_shared_patchpoint);

        /* Produce logging if required for on-chip */
        if (log)
        {
            FSM_OUTPUT_LOGGING();
        }
#endif

        /* Point to the first transition defined for the current state */
        /* Search the current state's transition list for the event,
         * terminating the search at FSM_EVENT_NULL
         */
        for(p_transition = p_fsm->p_table[*p_state].p_transitions,
                p_end_trans  = p_fsm->p_table[*p_state+1].p_transitions; ;
            p_transition++)
        {
            /* Have we reached the end of the list */
            if (p_transition == p_end_trans)
            {
                /* Unhandled event */
                FSM32_LOG_ENTRY(p_fsm, *p_state, event, *p_state,
                                FSM32_ACTION_UNDEF);
                FSM32_LOG_EXIT(p_fsm, event);
                return FSM_RESULT_FAIL;
            }
            if (TRANSITION32_FIELD(EVENT, p_transition->trans) == event)
                break;
        }

        /* Cache the action index, we need it a couple of times. */
        action_index = TRANSITION32_FIELD(ACTION, p_transition->trans);

        /* Log before we perform the transition */
        FSM32_LOG_ENTRY(p_fsm, *p_state, event,
                        TRANSITION32_FIELD(NEW_STATE, p_transition->trans),
                        action_index);

        /* State transition */
        orig_state = *p_state;
        *p_state = TRANSITION32_FIELD(NEW_STATE, p_transition->trans);

        /* Call the action function if one is specified */
        if (action_index != FSM32_ACTION_NULL)
        {
            fsm_event_t orig_event = event;
            event = p_fsm->p_actions[action_index](pv_arg1, pv_arg2);
            if ( FSM32_EVENT_REPOST == event )
            {
                /* Re-post the original event after the state transition -
                 * this handler was a "pass through" */
                event = orig_event;
            }
            else if ( FSM32_EVENT_ABORT == event )
            {
                /* Abort the state transition, as if nothing happened:
                 * restore the state, return non-event (for testing) */
                *p_state = orig_state;
                event = FSM32_EVENT_NULL;
            }
        }
        else
        {
            event = FSM32_EVENT_NULL;
        }

        FSM32_LOG_EXIT(p_fsm, event);
    }

    return FSM_RESULT_OK;
}
