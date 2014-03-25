/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_random.h"
#include "csr_sched.h"
#include "csr_util.h"

#include "csr_abcsp_intf_hl.h"
#include "csr_abcsp.h"

/* A timed event identifier. */

static CsrSchedTid bcsp_tid = 0;
static CsrSchedTid tshy_tid = 0;
static CsrSchedTid tconf_tid = 0;


/* Forward references. */
static void bcsp_timed_evt(CsrUint16, void *);
static void tshy_timed_evt(CsrUint16, void *);
static void tconf_timed_evt(CsrUint16, void *);

void CsrAbcspStartBcspTimer(void)
{
    CsrTime bcspPeriod;
    CsrInt8 jitter;
#if CSR_BCSP_RETRANSMISSION_JITTER > 0
    CsrUint32 rnd;

    rnd = CsrRandom(CsrAbcspInstanceData.randomState);

    /* Jitter +/- N ms */
    jitter = (CsrInt8) rnd % (CSR_BCSP_RETRANSMISSION_JITTER / 1000);

    /* If high bit is set, jitter is negative. */
    if (rnd & (1U << 31))
    {
        jitter = -jitter;
    }
#else
    jitter = 0;
#endif

    CsrAbcspCancelBcspTimer();
#ifndef CSR_BCSP_RETRANSMISSION_TIMER
    bcspPeriod = CsrAbcspInstanceData.retransmitTimerPeriod;
#else
    bcspPeriod = CSR_BCSP_RETRANSMISSION_TIMER;
#endif

#if CSR_BCSP_RETRANSMISSION_JITTER > 0
    /* Enforce a minimum period. */
    if ((CsrInt32) bcspPeriod + jitter * 1000 < CSR_BCSP_RETRANSMISSION_MINIMUM)
    {
        jitter = 0;
        bcspPeriod = CSR_BCSP_RETRANSMISSION_MINIMUM;
    }
#endif

    bcsp_tid = CsrSchedTimerSet(bcspPeriod + jitter * 1000L,
        bcsp_timed_evt, 0, NULL);
}

void CsrAbcspStartTshyTimer(void)
{
    CsrAbcspCancelTshyTimer();
    tshy_tid = CsrSchedTimerSet(CSR_BCSP_TSHY_TIMER, tshy_timed_evt, 0, NULL);
}

void CsrAbcspStartTconfTimer(void)
{
    CsrAbcspCancelTconfTimer();
    tconf_tid = CsrSchedTimerSet(CSR_BCSP_TCONF_TIMER, tconf_timed_evt, 0, NULL);
}

void CsrAbcspCancelBcspTimer(void)
{
    CsrSchedTimerCancel(bcsp_tid, 0, NULL);
    bcsp_tid = CSR_SCHED_TID_INVALID;
}

void CsrAbcspCancelTshyTimer(void)
{
    CsrSchedTimerCancel(tshy_tid, NULL, NULL);
    tshy_tid = CSR_SCHED_TID_INVALID;
}

void CsrAbcspCancelTconfTimer(void)
{
    CsrSchedTimerCancel(tconf_tid, NULL, NULL);
    tconf_tid = CSR_SCHED_TID_INVALID;
}

static void bcsp_timed_evt(CsrUint16 tmp, void *tmp2)
{
    /* Reset timer id to avoid timers being cancelled later */
    bcsp_tid = CSR_SCHED_TID_INVALID;
    CsrAbcspBcspTimedEvent();
}

static void tshy_timed_evt(CsrUint16 tmp, void *tmp2)
{
    tshy_tid = CSR_SCHED_TID_INVALID;
    CsrAbcspTshyTimedEvent();
}

static void tconf_timed_evt(CsrUint16 tmp, void *tmp2)
{
    tconf_tid = CSR_SCHED_TID_INVALID;
    CsrAbcspTconfTimedEvent();
}
