/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "platform/csr_ac97.h"
#include "platform/csr_s3c24a0.h"
#include "platform/csr_interrupt.h"
#include "platform/csr_dma.h"
#include "platform/csr_mmu.h"
#include "csr_ac97_data.h"

#define AC97_MODE_OFF()     REG_SET(rAC_GLBCTRL, 9, 8, 0)
#define AC97_MODE_DMA()     REG_SET(rAC_GLBCTRL, 9, 8, 0x3)

/* Static state data */
static CsrInt32 state = AC97_STATE_DISABLE;
static CsrUint8 dma;
static FrameBufferMic *pendingList;
static FrameBufferMic buffers[AC97_FRAMEBUFFER_SLOTS];

static void startDMA(CsrUint32 length, CsrUint16 *data)
{
    DMA_SOURCE_ADDRESS_SET(dma + 1, &rAC_MICDATA);
    DMA_SOURCE_CONTROL_SET(dma + 1, 1, 1);
    DMA_DESTINATION_ADDRESS_SET(dma + 1, data);
    DMA_DESTINATION_CONTROL_SET(dma + 1, 0, 0, 0);
    DMA_CON_SET(dma + 1, DMA_CON_HANDSHAKE | DMA_CON_SYNC_APB | DMA_CON_INT | DMA_CON_SINGLESERVICE | DMA_CON_HARDTRIGGER | DMA_CON_NOAUTORELOAD | DMA_CON_HALFWORD, 4 + dma, length);
    DMA_ON(dma + 1);
}

static void eventDMA(void)
{
    CsrUint8 paused;

    /* Stop the DMA */
    DMA_STOP(dma + 1);

    /* Clean and Invalidate Data Cache */
    MMUInvalidateDCacheRegion((CsrUint32) pendingList->data, 2 * pendingList->length);

    /* Callback */
    pendingList->callback(pendingList->length, pendingList->data, TRUE);

    /* Check if going into pause */
    paused = pendingList->paused;

    /* Free top element of list */
    pendingList->data = NULL;

    /* Pop off top element of list */
    pendingList = pendingList->next;

    /* If any elements are left in list, restart the DMA with next element */
    if ((pendingList != NULL) && (!paused))
    {
        startDMA(pendingList->length, pendingList->data);
    }
}

CsrInt32 AC97MicInputStart(CsrUint8 dmaChannel)
{
    /* Check argument */
    if ((dmaChannel != AC97_DMA_CHANNEL_DEFAULT) && (dmaChannel != AC97_DMA_CHANNEL_ALTERNATIVE))
    {
        return AC97_RESULT_FAILURE;
    }

    if (state == AC97_STATE_DISABLE)
    {
        CsrInt32 i;

        /* Initialisation of buffers */
        for (i = 0; i < AC97_FRAMEBUFFER_SLOTS; i++)
        {
            buffers[i].data = NULL;
        }
        pendingList = NULL;
    }
    else
    {
        /* Reconfiguration - Stop first */
        AC97MicInputStop();
    }

    /* Set started state */
    state = AC97_STATE_TRANSFER;

    /* Save selected DMA channel */
    dma = dmaChannel;

    /* Switch hardware to DMA mode (shared register) */
    INT_ATOMIC_EXECUTE(AC97_MODE_DMA());

    /* Register relevant DMA handler function */
    DMARegisterHandler(dma + 1, eventDMA);

    return AC97_RESULT_SUCCESS;
}

CsrInt32 AC97MicInputStop()
{
    if (state != AC97_STATE_DISABLE)
    {
        CsrInt32 i;

        /* Stop listening to DMA channel */
        DMARegisterHandler(dma + 1, NULL);

        /* Stop DMA channel */
        DMA_STOP(dma + 1);

        /* Switch hardware to disabled mode (shared register) */
        {
            INT_ATOMIC_EXECUTE_HEAD();

            AC97_MODE_OFF();

            /* Set disabled state */
            state = AC97_STATE_DISABLE;

            /* Release buffer slots */
            for (i = 0; i < AC97_FRAMEBUFFER_SLOTS; i++)
            {
                if (buffers[i].data != NULL)
                {
                    buffers[i].callback(buffers[i].length, buffers[i].data, FALSE);
                    buffers[i].data = NULL;
                }
            }

            INT_ATOMIC_EXECUTE_TAIL();
        }
    }

    return AC97_RESULT_SUCCESS;
}

CsrInt32 AC97MicInputPause(void)
{
    if (state == AC97_STATE_TRANSFER)
    {
        state = AC97_STATE_PAUSE;

        /* Stop the DMA (as a side-effect, this protects pendingList) */
        DMA_STOP_WAIT(dma + 1);

        /* Retrieve DMA position if currently transfering */
        if (pendingList != NULL)
        {
            pendingList->current = DMA_CURRENT_SOURCE_GET(dma + 1);

            /* Mark this buffer as paused, to prevent any pending DMA events from resuming */
            pendingList->paused = TRUE;
        }

        /* Stop piping hardware FIFO data to AC-Link (shared register) */
        INT_ATOMIC_EXECUTE(AC97_MODE_OFF());
    }

    return AC97_RESULT_SUCCESS;
}

CsrInt32 AC97MicInputResume(void)
{
    if (state == AC97_STATE_PAUSE)
    {
        state = AC97_STATE_TRANSFER;

        /* Note: pendingList is implicitly protected here */
        if (pendingList != NULL)
        {
            if (pendingList->paused)
            {
                CsrInt32 offset = pendingList->current - pendingList->data;
                pendingList->paused = FALSE;
                if (pendingList->current >= pendingList->data)
                {
                    if (pendingList->length > offset)
                    {
                        /* Start where the DMA was stopped (or from the start if it is a new buffer) */
                        startDMA(pendingList->length - offset, pendingList->current);
                    }
                    else if (pendingList->length == offset)
                    {
                        /* The DMA was stopped just before it completed, but
                           the interrupt did not trigger, restart the DMA on
                           the last sample. */
                        startDMA(1, pendingList->current - 1);
                    }
                    else
                    {
                        /* Current is after data + length - must never happen */
                    }
                }
                else
                {
                    /* Current is before data - must never happen */
                }
            }
            else
            {
                /* A new buffer is ready for playback */
                startDMA(pendingList->length, pendingList->data);
            }
        }

        /* Start data transfers from hardware FIFO again (shared register) */
        INT_ATOMIC_EXECUTE(AC97_MODE_DMA());
    }

    return AC97_RESULT_SUCCESS;
}

CsrInt32 AC97MicInputEnqueue(CsrUint32 length, CsrUint16 *data, AC97MicCallbackFunction callback)
{
    CsrInt32 i;
    FrameBufferMic *ptr;

    if ((state == AC97_STATE_DISABLE) || (callback == NULL) || !length || (data == NULL))
    {
        return AC97_RESULT_FAILURE;
    }

    /* Find a free buffer slot */
    for (i = 0; i < AC97_FRAMEBUFFER_SLOTS; i++)
    {
        if (buffers[i].data == NULL)
        {
            /* Found a free slot */
            buffers[i].next = NULL;
            buffers[i].length = length;
            buffers[i].callback = callback;
            buffers[i].paused = FALSE;
            buffers[i].current = data;
            buffers[i].data = data;
            break;
        }
    }
    if (i == AC97_FRAMEBUFFER_SLOTS)
    {
        /* No free slots */
        return AC97_RESULT_NO_MORE_SLOTS;
    }

    /* Writeback from cache to main memory to prepare for DMA transfer */
    MMUCleanDCacheRegion((CsrUint32) data, 2 * length);
    MMUDrainWriteBuffer();

    {
        INT_ATOMIC_EXECUTE_HEAD();

        /* Insert in pending list */
        if (pendingList == NULL)
        {
            /* Nothing in list, insert at start */
            pendingList = &buffers[i];

            /* Start the DMA with the given buffer (only if not in pause) */
            if (state != AC97_STATE_PAUSE)
            {
                startDMA(length, data);
            }
        }
        else
        {
            for (ptr = pendingList; ptr->next != NULL; ptr = ptr->next)
            {
                /* Find the end of the list */
            }

            /* Insert at end of the list */
            ptr->next = &buffers[i];
        }

        INT_ATOMIC_EXECUTE_TAIL();
    }

    return AC97_RESULT_SUCCESS;
}

void AC97MicInputCallbackMask(void)
{
    if (state != AC97_STATE_DISABLE)
    {
        DMA_MASK_CHANNEL_ATOMIC(dma + 1);
    }
}

void AC97MicInputCallbackUnmask(void)
{
    if (state != AC97_STATE_DISABLE)
    {
        DMA_UNMASK_CHANNEL_ATOMIC(dma + 1);
    }
}
