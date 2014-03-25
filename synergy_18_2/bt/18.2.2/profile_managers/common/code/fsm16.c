/*!
        (c) CSR plc 2010
        All rights reserved

\file   fsm16.c

\brief  The 16bit version of the FSM.
*/

#include "csr_synergy.h"

#include "fsm_private.h"
#include "fsm16_log_private.h"

/*! \brief fsm_16bit_run

    The engine function for a 16 bit bitfiled sparse state machine.
    Adheres to the "shake it until it stops rattling" principle,
    driving the state machine until there are no more events.

    \returns fsm_result_t - FSM_RESULT_OK if the event was processed
    FSM_RESULT_FAIL otherwise
*/
fsm_result_t fsm_16bit_run(
    const FSM_SPARSE16_DEFINITION_T *p_fsm, /* The FSM definition */
    fsm16_state_t *p_state,                 /* State variable */
    fsm_event_t event,                      /* Event to process */
    void *pv_arg1,                          /* Two void pointer arguments */
    void *pv_arg2
    )
{
    const FSM_SPARSE16_TRANSITION_T *p_transition, *p_end_trans;
    fsm16_action_t action_index;

    while (event != FSM_EVENT_NULL)
    {
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
                FSM_LOG_ENTRY(p_fsm, *p_state, event, *p_state,
                        FSM16_ACTION_UNDEF);
                FSM_LOG_EXIT(p_fsm, event);
                return FSM_RESULT_FAIL;
            }
            if (p_transition->event == event)
                break;
        }

        /* Cache the action index, we need it a couple of times. */
        action_index = p_transition->action_index;

        /* Log before we perform the transition */
        FSM_LOG_ENTRY(p_fsm, *p_state, event, p_transition->next_state,
                      action_index);

        /* State transition */
        *p_state = p_transition->next_state;

        /* Call the action function if one is specified */
        if (action_index != FSM16_ACTION_NULL)
        {
            event = p_fsm->p_actions[action_index](pv_arg1, pv_arg2);
        }
        else
        {
            event = FSM_EVENT_NULL;
        }

        FSM_LOG_EXIT(p_fsm, event);
    }

    return FSM_RESULT_OK;
}
