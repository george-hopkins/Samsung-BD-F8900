/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "platform/csr_delay.h"
#include "platform/csr_timer.h"

void DelayUs(CsrUint32 delay)
{
    CsrUint32 now;
    CsrUint32 lasttime;
    CsrUint32 counter = 0;
    CsrUint32 delta = 0;

    lasttime = TIMER1_READ();

    while (TRUE)
    {
        now = TIMER1_READ();

        if (now <= lasttime)
        {
            delta = lasttime - now; /* Note: hardware timers count down */
        }

        counter += delta;

        lasttime = now;

        if (counter >= delay)
        {
            break;
        }
    }
}
