/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "csr_sched.h"
#ifdef CSR_LOG_ENABLE
#include "csr_log.h"
#endif
#include "csr_panic.h"

#include "platform/csr_panic_init.h"
#include "platform/csr_led.h"
#include "platform/csr_delay.h"

static CsrPanicHandler panic = NULL;

void CsrPanicInit(CsrPanicHandler cb)
{
    panic = cb;
}

#define LED_DELAY   (100 * 1000)

void CsrPanic(CsrUint8 tech, CsrUint16 reason, const char *p)
{
    if (panic)
    {
        panic(tech, reason, p);
    }

    /* Mask and clear all interrupts */
    INT_GLOBAL_MASK_CLEAR();

    while (1)
    {
        LED1_ON();
        DelayUs(LED_DELAY);
        LED2_ON();
        DelayUs(LED_DELAY);
        LED1_OFF();
        LED2_OFF();
        DelayUs(LED_DELAY);
        LED1_ON();
        DelayUs(LED_DELAY);
        LED2_ON();
        DelayUs(LED_DELAY);
        LED1_OFF();
        LED2_OFF();
        DelayUs(1000000 - 5 * LED_DELAY);
    }
}
