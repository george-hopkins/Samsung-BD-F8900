/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "nucleus.h"
#include "csr_types.h"
#include "platform/csr_dma.h"
#include "platform/csr_interrupt.h"

/* Function Pointers for each of the DMA channels */
static DMACallbackFunction handlers[4] = {0, 0, 0, 0};

/* Function Pointer and channel for Fast Mode (used when a single handler is registered) */
static DMACallbackFunction handlerSingle;
static CsrInt32 channelSingle;

/* Interrupt Service Routine (used when multiple handlers are registered) */
static void dmaAck(void)
{
    int pending;

    pending = REG_CHECK(INT_SUB_PENDING_GET(), INT_SUB_DMA3, INT_SUB_DMA0);

    if (pending & 1)        /* INT_SUB_DMA0 */
    {
        INT_SUB_CLEAR(INT_SUB_DMA0);
        INT_CLEAR(INT_DMA);
        if (handlers[0])
        {
            handlers[0]();
        }
    }

    if (pending & 2)   /* INT_SUB_DMA1 */
    {
        INT_SUB_CLEAR(INT_SUB_DMA1);
        INT_CLEAR(INT_DMA);
        if (handlers[1])
        {
            handlers[1]();
        }
    }

    if (pending & 4)   /* INT_SUB_DMA2 */
    {
        INT_SUB_CLEAR(INT_SUB_DMA2);
        INT_CLEAR(INT_DMA);
        if (handlers[2])
        {
            handlers[2]();
        }
    }

    if (pending & 8)   /* INT_SUB_DMA3 */
    {
        INT_SUB_CLEAR(INT_SUB_DMA3);
        INT_CLEAR(INT_DMA);
        if (handlers[3])
        {
            handlers[3]();
        }
    }
}

/* Fast Interrupt Service Routine (used when a single handler is registered) */
static void dmaAckFast(void)
{
    INT_SUB_CLEAR(channelSingle);
    INT_CLEAR(INT_DMA);
    if (handlerSingle)
    {
        handlerSingle();
    }
}

static void setupServiceRoutine(void)
{
    CsrInt32 i, handlerCount;

    handlerCount = 0;
    for (i = 0; i < 4; i++)
    {
        if (handlers[i])
        {
            handlerCount++;
            handlerSingle = handlers[i];
            channelSingle = INT_SUB_DMA0 + i;
        }
    }

    switch (handlerCount)
    {
        case 0: /* No handlers registered */
        {
            INT_MASK(INT_DMA);
            INT_CLEAR(INT_DMA);
            {
                VOID (*old)(INT);
                NU_Register_LISR(INT_DMA, NU_NULL, &old);
            }
            break;
        }
        case 1: /* Only one handler registered - use fast mode */
        {
            VOID (*old)(INT);
            NU_Register_LISR(INT_DMA, (VOID (*)(INT))dmaAckFast, &old);
            INT_CLEAR(INT_DMA);
            INT_UNMASK(INT_DMA);
            break;
        }
        default: /* Multiple handlers registered */
        {
            VOID (*old)(INT);
            NU_Register_LISR(INT_DMA, (VOID (*)(INT))dmaAck, &old);
            INT_CLEAR(INT_DMA);
            INT_UNMASK(INT_DMA);
        }
    }
}

/* Register a handler and setup interrupt service routine */
void DMARegisterHandler(CsrUint32 channel, DMACallbackFunction function)
{
    INT_ATOMIC_EXECUTE_HEAD();

    channel = channel & 0x3;

    if (function)
    {
        if (handlers[channel] != NULL)
        {
            /* Already registered - just replace the handler (and handlerSingle) */
            handlerSingle = handlers[channel] = function;
        }
        else
        {
            /* New registration */
            handlers[channel] = function;

            /* Clear and unmask */
            INT_SUB_CLEAR(INT_SUB_DMA0 + channel);
            INT_SUB_UNMASK(INT_SUB_DMA0 + channel);

            /* Setup appropriate service routine */
            setupServiceRoutine();
        }
    }
    else
    {
        if (handlers[channel])
        {
            handlers[channel] = NULL;

            /* Mask and clear */
            INT_SUB_MASK(INT_SUB_DMA0 + channel);
            INT_SUB_CLEAR(INT_SUB_DMA0 + channel);

            /* Setup appropriate service routine */
            setupServiceRoutine();
        }
    }

    INT_ATOMIC_EXECUTE_TAIL();
}
