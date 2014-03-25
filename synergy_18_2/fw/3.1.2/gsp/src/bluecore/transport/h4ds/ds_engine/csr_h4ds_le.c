/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/
#include "csr_h4ds_private.h"
#include "csr_sched.h"

#ifdef CSR_LOG_ENABLE
#include "csr_log_gsp.h"
#include "csr_formatted_io.h"
#endif

/* H4DS-LE state machine states. */
#define state_shy           (0x0)
#define state_curious       (0x1)
#define state_garrulous     (0x2)
#define state_exit          (0x3)

/* These variables must be set on different bits */
#define paused              (0x1)
#define paused_tsync_te     (0x2)
#define paused_tconf_te     (0x4)

/* H4DS-LE state information. */
static CsrUint8 ds_le_state = 0;
static CsrUint8 pause_flags = 0;


/****************************************************************************
NAME
        CsrH4DSLeInit  -  initialise the link establishment entity
*/

void CsrH4DSLeInit(void)
{
    /* Attempt to prevent any existing timed event timers firing. */
    H4DS_CANCEL_TSYNC_TIMER();
    H4DS_CANCEL_TCONF_TIMER();

    /* Initialise the H4DS-LE engine state. */
    ds_le_state = 0;
    pause_flags = 0;

    /* Emit the first sync message. */
    CsrH4DSReqTxLeSync();

    /* Arrange to be called after a respectful interval. */
    H4DS_START_TSYNC_TIMER();
}

/****************************************************************************
NAME
        CsrH4DSLeFsm  -  the h4ds link establishment entity state machine

FUNCTION
        Feeds the message "msg" into the h4ds link establishment state
        machine.

IMPLEMENTATION NOTE
        The code doesn't generally bother to cancel timers - they time
        out harmlessly, so there's no point.
*/

void CsrH4DSLeFsm(lemsgid msg)
{
#ifdef CSR_LOG_ENABLE
    CsrCharString logmsg[100];
#endif

    switch (ds_le_state)
    {
        case state_shy:
        {
            switch (msg)
            {
                case lemsgid_tsync_timeout:
                {
                    CsrH4DSReqTxLeSync();
                    H4DS_START_TSYNC_TIMER();
                    break;
                }
                case lemsgid_sync:
                {
                    CsrH4DSReqTxLeSack();
                    break;
                }
                case lemsgid_sack:
                {
                    H4DS_EVENT(H4DS_EVT_LE_SYNC);
                    CsrH4DSReqTxLeConf();
                    H4DS_START_TCONF_TIMER();
                    ds_le_state = state_curious;
                    break;
                }
                default:
                {
#ifdef CSR_LOG_ENABLE
                    CsrSnprintf(logmsg, sizeof(logmsg), "h4ds_le.c: function CsrH4DSLeFsm() received message: %#x in state: %#x", msg, ds_le_state);
                    CSR_LOG_TEXT(logmsg);
#endif
                    break;
                }
            }
            break;
        }
        case state_curious:
        {
            switch (msg)
            {
                case lemsgid_tconf_timeout:
                {
                    CsrH4DSReqTxLeConf();
                    H4DS_START_TCONF_TIMER();
                    break;
                }
                case lemsgid_sync:
                {
                    CsrH4DSReqTxLeSack();
                    break;
                }
                case lemsgid_conf:
                {
                    CsrH4DSReqTxLeCack();
                    break;
                }
                case lemsgid_cack:
                {
                    H4DS_EVENT(H4DS_EVT_LE_CONF);
                    ds_le_state = state_garrulous;
                    break;
                }
                default:
                {
#ifdef CSR_LOG_ENABLE
                    CsrSnprintf(logmsg, sizeof(logmsg), "h4ds_le.c: function CsrH4DSLeFsm() received message: %#x in state: %#x", msg, ds_le_state);
                    CSR_LOG_TEXT(logmsg);
#endif
                    break;
                }
            }
            break;
        }
        case state_garrulous:
            switch (msg)
            {
                case lemsgid_conf:
                {
                    CsrH4DSReqTxLeCack();
                    break;
                }
                case lemsgid_sync:
                {
                    /* Peer has apparently restarted. */
                    H4DS_EVENT(H4DS_EVT_PEER_RESTART);
                    ds_le_state = state_exit;
                    break;
                }
                default:
                {
#ifdef CSR_LOG_ENABLE
                    CsrSnprintf(logmsg, sizeof(logmsg), "h4ds_le.c: function CsrH4DSLeFsm() received message: %#x in state: %#x", msg, ds_le_state);
                    CSR_LOG_TEXT(logmsg);
#endif
                    break;
                }
            }

            break;
        case state_exit:
        {
            /* Block all h4 traffic (until reinitialised). */
            break;
        }
        default:
        {
            H4DS_PANIC(H4DS_PANIC_INT_CORRUPTION);
            break;
        }
    }
}

/****************************************************************************
NAME
        CsrH4DSLeTxChoked  -  is the h4ds transmit path choked?
*/

CsrBool CsrH4DSLeTxChoked(void)
{
    return (CsrBool) (ds_le_state != state_garrulous);
}

/****************************************************************************
NAME
        CsrH4DSLeRxChoked  -  is the h4ds receive path choked?
*/

CsrBool CsrH4DSLeRxChoked(void)
{
    return (CsrBool) !(ds_le_state == state_garrulous
                       || ds_le_state == state_curious);
}

/****************************************************************************
NAME
        CsrH4DSLePause  -  pause the link establishment engine

FUNCTION
        Pause the link establishment state machine until it is unpaused by a
        call to CsrH4DSLeUnpause().

        This function exists to allow the deep sleep state machine to shut
        down the link establishment state machine when the peer cannot be
        contacted (e.g., UART wire drop out or peer crash).  In this state it
        is pointless to continue to run local code to try to rouse the peer
        as it just wastes power.  Hence the pause function to hold the link
        establishment state machine.

IMPLEMENTATION NOTES
        We know that the pause function will only be called from the ds code,
        and that in that state it will not be able to receive any messages
        from there peer.  Hence, the le engine only has to worry about timed
        events in its paused state.

        The code in the pause and unpause functions, and related code in the
        two timed event catcher functions, combines to record timed events in
        the paused state and pass any recorded timed events into the state
        machine when it is unpaused.  Integrating the pause/unpause logic
        into the state machine would create a horrid mess - of code and of
        the design.
*/

void CsrH4DSLePause(void)
{
    pause_flags = (paused | pause_flags);
}

/****************************************************************************
NAME
        CsrH4DSLeUnpause  -  unpause the link establishment engine

IMPLEMENTATION NOTES
        See the comments for CsrH4DSLePause().
*/

void CsrH4DSLeUnpause(void)
{
    pause_flags = (pause_flags & (~paused));
    if (paused_tsync_te & pause_flags)
    {
        pause_flags = (pause_flags & (~paused_tsync_te));
        CsrH4DSTsyncTimedEvent();
    }
    if (paused_tconf_te & pause_flags)
    {
        pause_flags = (pause_flags & (~paused_tconf_te));
        CsrH4DSTconfTimedEvent();
    }
}

/****************************************************************************
NAME
        CsrH4DSTsyncTimedEvent  -  report a tsync timeout event to the fsm

IMPLEMENTATION NOTES
        See the comments for CsrH4DSLePause().
*/

void CsrH4DSTsyncTimedEvent(void)
{
    if (paused & pause_flags)
    {
        pause_flags = (pause_flags | paused_tsync_te);
    }
    else
    {
        CsrH4DSLeFsm(lemsgid_tsync_timeout);
    }
}

/****************************************************************************
NAME
        CsrH4DSTconfTimedEvent  -  report a tconf timeout event to the fsm

IMPLEMENTATION NOTES
        See the comments for CsrH4DSLePause().
*/

void CsrH4DSTconfTimedEvent(void)
{
    if (paused & pause_flags)
    {
        pause_flags = (pause_flags | paused_tconf_te);
    }
    else
    {
        CsrH4DSLeFsm(lemsgid_tconf_timeout);
    }
}
