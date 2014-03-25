/****************************************************************************

                   (c) CSR plc 2010

                   All rights reserved

FILE:              dm_powerstates.c

DESCRIPTION:       The Powerstates management entity of the
                   BlueStack Device Manager.



****************************************************************************/

#include "csr_synergy.h"


#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "l2cap_prim.h"
#include "dm_prim.h"
#include "dm_acl_powerstates.h"
#include "dm_mgt_interface.h"
#ifdef BUILD_FOR_HOST
#include "csr_bt_core_stack_pmalloc.h"
#endif
#include "csr_bt_core_stack_pmalloc.h"
#include "tbdaddr.h"

#ifdef INSTALL_DM_POWERSTATES_MODULE
#include "buffer.h"
#include "hostio.h"
#include "vm_const.h"
#include "csr_bt_core_stack_pmalloc.h"
#include "csr_bt_common.h"

#include "dm_acl_core.h"
#include "dm_acl_policy_manager.h"
#include "dm_acl_manager.h"
#include "dm_acl_policy_types.h"
#include "longtimer.h"

/*
 * The longest timer we can safely use without hitting any clock wrap issues.
 * Specified in deciseconds.
 */
#define MAX_TIMEOUT ((DECITIME)18000)

/*! Private Function Prototypes */
static void idle_timeout(CsrUint16 handle, void *pv_unused);
static CsrUint8 validate_powerstates(const LP_POWERSTATE_T *const state,
                const CsrUint16 num_states);

/*! \brief  Validates that the list of power states passed are usable.

    \param  state Pointer to powerstate array.
    \param  num_states Number of states in array.
    \returns result code.
*/
static CsrUint8 validate_powerstates(const LP_POWERSTATE_T *const state,
                                    const CsrUint16 num_states)
{
    CsrUint8 i;

    /* Consistency check */
    if ((state == NULL) != (num_states == 0))
    return LP_POWERSTATES_ERROR;

    /* No states is OK (stop using powerstates) */
    if (num_states == 0)
        return LP_POWERSTATES_SUCCESS;

    /* last mode must have a duration of zero */
    if (state[num_states-1].duration != 0)
    return LP_POWERSTATES_BAD_TERMINATION;

    /* check all modes in the list are supported */
    for (i = 0; i < num_states; i++)
    {
    switch (state[i].mode)
    {
        case LP_POWERMODE_ACTIVE:
        case LP_POWERMODE_SNIFF:
        case LP_POWERMODE_PASSIVE:
                break;
        default:
            return LP_POWERSTATES_UNSUPPORTED_MODE;
    }
    }

    return LP_POWERSTATES_SUCCESS;
}

/*! \brief Handle application request to write powerstates.
    It checks the request is sane and if so applies it to the ACL.

    \param  p_uprim Pointer to primitive.
    \returns TRUE
*/
void dm_acl_lp_write_powerstates_req(const DM_UPRIM_T *const p_uprim)
{
    const DM_LP_WRITE_POWERSTATES_REQ_T *const p_req_prim =
            (const DM_LP_WRITE_POWERSTATES_REQ_T*const)p_uprim;
    DM_LP_WRITE_POWERSTATES_CFM_T *p_cfm_prim;
    const BD_ADDR_T *const p_bd_addr = &p_req_prim->bd_addr;
    LP_POWERSTATE_T *states = p_req_prim->states;
    DM_ACL_T *p_acl;
    CsrUint8 result;

    /* validate power table */
    result = validate_powerstates(states, p_req_prim->num_states);
    if (result == LP_POWERSTATES_SUCCESS)
    {
        if ((p_acl = dm_acl_find_by_bd_addr(p_bd_addr)) != NULL)
        {
            /* free any previous states  */
            CsrPmemFree(p_acl->dm_acl_client_policy.powerstates);

            /* store the new power states */
            p_acl->dm_acl_client_policy.powerstates = states;

            /* When a new table is defined we should return to first state */
            DM_ACL_POLICY_UPDATE_LAST_ACL_TIME(p_acl);

            /* kick into action */
            dm_acl_policy_manager_reset(p_acl);
            states = NULL;
        }
        else
            result = LP_POWERSTATES_UNKNOWN_DEVICE;
    }

    /* if we're not going to use the powerstates free them */
    CsrPmemFree(states);

    /* build reply */
    p_cfm_prim = pnew(DM_LP_WRITE_POWERSTATES_CFM_T);
    p_cfm_prim->type = DM_LP_WRITE_POWERSTATES_CFM;
    p_cfm_prim->result = result;
    bd_addr_copy(&p_cfm_prim->bd_addr, p_bd_addr);

    /* post reply */
    DM_SendMessageAMphandle(p_cfm_prim);
}

/*! \brief See if enough time has passed to be worth kicking the policy
    manager again.

    The purpose of this function is to return us to the top
    state in our powerstate table. Powerstate tables have a
    resolution of 1 second, so there's no point in trying to
    reset them much more often than that.

    \param p_acl Pointer to ACL record structure.
*/
CsrBool dm_acl_powerstates_activity(DM_ACL_T *p_acl)
{
    DECITIME time = get_deci_time();

    if (!CsrTimeEq(p_acl->dm_acl_client_policy.last_acl_time, time))
    {
        p_acl->dm_acl_client_policy.last_acl_time = time;
        return TRUE;
    }

    return FALSE;
}

/*! \brief Timer callback used when the idle_timer expires.

    \param handle HCI connection handle for ACL.
    \param pv_unused Unused.
*/
static void idle_timeout(CsrUint16 handle, void *pv_unused)
{
    DM_ACL_T *p_acl;

    CSR_UNUSED(pv_unused);

    if ((p_acl = dm_acl_find_by_handle(
                    (hci_connection_handle_t)handle)) != NULL)
    {
        TIMER_EXPIRED(p_acl->dm_acl_client_policy.idle_timer);

        /* The policy manager will decide what to do next */
        kick_policy_manager(p_acl);
    }
}

/*! \brief Changes the mode of an ACL based on the list of powerstates.

    \param p_acl Pointer to ACL record structure.
*/
void dm_acl_policy_powerstates(DM_ACL_T *const p_acl)
{
    /* find idle time */
    LP_POWERSTATE_T *state;
    DECITIME state_end_time = p_acl->dm_acl_client_policy.last_acl_time;
    const DECITIME now = get_deci_time();

    /* cancel any timer */
    timer_cancel(&p_acl->dm_acl_client_policy.idle_timer);

    /* Find out which state we're currently in. */
    for(state = p_acl->dm_acl_client_policy.powerstates; state->duration != 0; ++state)
    {
    /* calculate when this state ends
       (duration is in seconds and end_time is DECITIME) */
    state_end_time += ((DECITIME)state->duration) * 10;

        /* If the state is still active then we're done. */
    if (CsrTimeGt(state_end_time, now))
        break;
    }

    /* check we're in the correct mode */
    switch (state->mode)
    {
        case LP_POWERMODE_PASSIVE:
        /* We don't care about the power mode, so enable all that the
               client allows and let the remote device choose what to do! */
            dm_acl_set_link_policy(p_acl, 0xFFFF);
        break;

        case LP_POWERMODE_ACTIVE:
        /* Update the policy settings to disable all LP modes. */
            dm_acl_set_link_policy(p_acl, DM_ACL_NO_LP_MASK);

            /* Return to ACTIVE. */
            (void)check_mode_change(p_acl, HCI_BT_MODE_ACTIVE, NULL);
        break;

        case LP_POWERMODE_SNIFF:
        /* Update the link policy settings to only allow sniff LP mode. */
            dm_acl_set_link_policy(p_acl, (DM_ACL_NO_LP_MASK | ENABLE_SNIFF));

            /* Enter SNIFF. */
            if (p_acl->dm_acl_client_policy.mode_change_holdoff_timer == 0
                    && p_acl->dm_acl_client_policy.mode_change_retries != 0
                    && check_mode_change(p_acl, HCI_BT_MODE_SNIFF, state)
                    && (p_acl->flags & DM_ACL_RSF_SNIFF) != 0)
            {
                /* Store current mode and request mode change.
                   (note we are overloading default link policy values) */
        send_hci_sniff_mode(&TBDADDR_ADDR(p_acl->addrt),
                        (p_acl->dm_acl_client_policy.sniff.max_interval = state->max_interval),
                        (p_acl->dm_acl_client_policy.sniff.min_interval = state->min_interval),
                        (p_acl->dm_acl_client_policy.sniff.attempt = state->attempt),
                        (p_acl->dm_acl_client_policy.sniff.timeout = state->timeout));

        p_acl->flags |= DM_ACL_MODE_CHANGE_REQUESTED;
        }
        break;
    }

    /* Set timer to kick us when this state expires. */
    if ((p_acl->flags & DM_ACL_MODE_CHANGE_REQUESTED) == 0 && state->duration)
    {
    /* If the time until the next state is longer than
       the max timeout, we'll have to come back earlier
       and hence set up another timer. */
    DECITIME in = CsrTimeSub(state_end_time, now);

    if (CsrTimeGt(in, MAX_TIMEOUT))
        in = MAX_TIMEOUT;

    timer_start(&p_acl->dm_acl_client_policy.idle_timer,
                    decitime_to_time(in),
                    idle_timeout,
                    p_acl->handle,
                    NULL);
    }
}

void dm_acl_client_deinit_policy_powerstates(DM_POLICY_T *p_policy)
{
    CsrPmemFree(p_policy->powerstates);
    p_policy->powerstates = NULL;

    timer_cancel(&p_policy->idle_timer);
}

#else /* INSTALL_DM_POWERSTATES_MODULE */

/* \brief Powerstates not supported, so send rejection. */
void dm_acl_lp_write_powerstates_req(const DM_UPRIM_T *const p_uprim)
{
    DM_LP_WRITE_POWERSTATES_CFM_T *p_cfm_prim;

    /* Free the powerstates */
    CsrPmemFree(p_uprim->dm_lp_write_powerstates_req.states);

    /* Send rejection */
    p_cfm_prim = pnew(DM_LP_WRITE_POWERSTATES_CFM_T);
    p_cfm_prim->type = DM_LP_WRITE_POWERSTATES_CFM;
    p_cfm_prim->result = LP_POWERSTATES_ERROR;
    bd_addr_copy(&p_cfm_prim->bd_addr,
            &p_uprim->dm_lp_write_powerstates_req.bd_addr);
    DM_SendMessageAMphandle(p_cfm_prim);
}

#endif /* INSTALL_DM_POWERSTATES_MODULE */
