/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <nucleus.h>

#include "csr_types.h"
#include "platform/csr_timer.h"
#include "platform/csr_interrupt.h"

extern volatile UNSIGNED TMD_System_Clock;

CsrUint32 GetTime(CsrUint32 *highPart)
{
    CsrUint16 tickInterval;
    CsrUint16 tl1, tl2;
    CsrUint32 th;
    CsrUint32 pending;
    CsrUint64 time;

    /* Read out both parts of timer.

       This is not atomic and it can be interrupted by the timer interrupt if
       interrupts are enabled or the lower part can wrap around without the high
       part being updated if interrupts are disabled. If this is the case, the
       lower part (tl) will not correspond to the high part (th).

       If interrupts are enabled and a timer interrupt occurs between or after
       the two operations the high part will be greater than th. If the interrupt
       occurs between the two operations tl will not correspond to th. By repeating
       the read-out until a wrap-free interval is encountered, the read-out
       operation will be virtually atomic.

       If interrupts are disabled, the high part will not increment when the timer
       wraps. By reading out the high part and the interrupt pending bit in a wrap
       free interval, it can be determined if the high part should be incremented
       by one to compensate.

       However, if the timer is at 0, the high part may already have been
       incremented prior to entering this function. This means that the lower
       part will not correspond to the high part until the timer wraps. To
       avoid this, the read-out is repeated until tl2 != 0. This also solves
       the problem of falsely incrementing the high part before the wrap
       occurs when the interrupt pending bit is set when the timer is at 0. */

    do
    {
        tl1 = TIMER3_READ();
        th = TMD_System_Clock;
        pending = INT_SUB_PENDING_CHECK(INT_SUB_TIMER3);
        tl2 = TIMER3_READ();
    } while ((tl2 > tl1) || (tl2 == 0));

    if (pending)
    {
        th++;
    }

    tickInterval = rTCNTB3 + 1;
    time = (CsrUint64) th * tickInterval + (tickInterval - 1 - tl1);

    if (highPart != NULL)
    {
        *highPart = (CsrUint32) ((time >> 32) & 0xFFFFFFFF);
    }

    return (CsrUint32) (time & 0xFFFFFFFF);
}
