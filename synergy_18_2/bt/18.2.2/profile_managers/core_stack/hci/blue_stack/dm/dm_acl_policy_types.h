/*!
        (c) CSR plc 2010
        All rights reserved

\file   dm_sync_types.h

\brief  Policy Manager definitions required for the ACL record structure.
*/

#ifndef _DM_ACL_POLICY_CLIENT_H_
#define _DM_ACL_POLICY_CLIENT_H_

#include "csr_synergy.h"

#ifdef INSTALL_DM_POWERSTATES_MODULE
#include "longtimer.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef DISABLE_DM_BREDR
typedef struct
{
    /* Mode management for this connection */
    link_policy_settings_t link_policy;             /* Link policy set by application. */
    link_policy_settings_t current_link_policy;     /* Current link policy, possibly masked by state. */

    CsrUint16 active;                /* 1 bit per client. Set to lock us in active mode. */
    CsrUint16 mode_change_retries;   /* Number of times to retry a mode change. */
    CsrSchedTid mode_change_holdoff_timer;  /* Prevent rapid mode change requests. */

    /* Power states configuration. */
#ifdef INSTALL_DM_POWERSTATES_MODULE
    DECITIME last_acl_time;         /* Time of last ACL packet. */

    LP_POWERSTATE_T *powerstates;   /* Pointer to Powerstates table. */
    CsrSchedTid idle_timer;                 /* Time duration of power states. */
    struct
    {
        CsrUint16 max_interval;          /* Max sniff interval */
        CsrUint16 min_interval;          /* Min sniff interval */
        CsrUint16 attempt;               /* Sniff attempt */
        CsrUint16 timeout;               /* Sniff timeout */
    } sniff;
#endif
} DM_POLICY_T;
#endif

#ifdef __cplusplus
}
#endif

#endif
