/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "platform/csr_interrupt.h"

static INTCallbackFunction handlerSDSlot = 0;
static INTCallbackFunction handlerGSMRing = 0;

static void __irq isrShared(void)
{
    CsrUint32 pending;

    pending = REG_CHECK(INT_EXT_PENDING_GET(), INT_EXT_EINT9, INT_EXT_EINT7);

    if (pending & 1)    /* INT_EXT_EINT7 */
    {
        if (handlerSDSlot)
        {
            handlerSDSlot();
        }
        INT_EXT_CLEAR(INT_EXT_EINT7);
    }
    else if (pending & 1) /* INT_EXT_EINT8 */
    {
        if (handlerGSMRing)
        {
            handlerGSMRing();
        }
        INT_EXT_CLEAR(INT_EXT_EINT8);
    }
    INT_CLEAR(INT_EINT7_10);
}

void INTSharedRegisterHandlerSDSlot(INTCallbackFunction function)
{
    handlerSDSlot = function;
    INT_SR_SET(INT_EINT7_10, isrShared);
    INT_UNMASK(INT_EINT7_10);
    INT_SHARED_UNMASK_SDSLOT()
}

void INTSharedRegisterHandlerGSMRing(INTCallbackFunction function)
{
    handlerGSMRing = function;
    INT_SR_SET(INT_EINT7_10, isrShared);
    INT_UNMASK(INT_EINT7_10);
    INT_SHARED_UNMASK_GSMRING();
}

void INTSharedUnregisterHandlerSDSlot(void)
{
    handlerSDSlot = 0;
    INT_SHARED_MASK_SDSLOT()
    INT_EXT_CLEAR(INT_EXT_EINT7);
    if (!handlerGSMRing)
    {
        INT_MASK(INT_EINT7_10);
    }
}

void INTSharedUnregisterHandlerGSMRing(void)
{
    handlerGSMRing = 0;
    INT_SHARED_MASK_GSMRING();
    INT_EXT_CLEAR(INT_EXT_EINT8);
    if (!handlerSDSlot)
    {
        INT_MASK(INT_EINT7_10);
    }
}
