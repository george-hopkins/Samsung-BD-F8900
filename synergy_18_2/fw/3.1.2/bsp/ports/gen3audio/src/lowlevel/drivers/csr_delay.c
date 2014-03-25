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
    CsrUint16 now;
    CsrUint16 lasttime;
    CsrUint32 counter = 0;
    CsrUint16 delta = 0;

    lasttime = TIMER4_READ();

    while (TRUE)
    {
        now = TIMER4_READ();

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
