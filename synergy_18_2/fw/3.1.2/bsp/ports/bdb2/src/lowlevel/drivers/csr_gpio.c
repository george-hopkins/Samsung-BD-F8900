/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "platform/csr_s3c24a0.h"
#include "platform/csr_gpio.h"

void GPIOPinConfigure(CsrUint8 pin, CsrUint8 function, CsrUint8 pullUp, CsrUint8 trigger)
{
    pin = pin & 0x1F;
    function = function & 0x3;
    trigger = trigger & 0x7;

    /* Configure trigger */
    if (pin < 3)
    {
        REG_SET(rEXTINTC0, pin * 4 + 2, pin * 4, trigger);
    }
    else if (pin < 10)
    {
        REG_SET(rEXTINTC1, (pin - 3) * 4 + 2, (pin - 3) * 4, trigger);
    }
    else if ((pin < 19) && (pin > 10))
    {
        REG_SET(rEXTINTC2, (pin - 11) * 4 + 2, (pin - 11) * 4, trigger);
    }

    /* Configure Pull-up */
    if (pullUp == GPIO_PULLUP_ON)
    {
        BIT_CLEAR(rGPPU, pin);
    }
    else
    {
        BIT_SET(rGPPU, pin);
    }

    /* Configure function */
    if (pin < 11)
    {
        REG_SET(rGPCON_L, pin * 2 + 1, pin * 2, function);
    }
    else if (pin < 19)
    {
        REG_SET(rGPCON_M, (pin - 11) * 2 + 1, (pin - 11) * 2, function);
    }
    else if (pin < 32)
    {
        REG_SET(rGPCON_U, (pin - 19) * 2 + 1, (pin - 19) * 2, function);
    }
}
