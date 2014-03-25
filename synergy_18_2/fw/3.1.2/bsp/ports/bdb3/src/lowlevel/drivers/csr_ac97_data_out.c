/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <csr_types.h>
#include <platform/csr_ac97.h>
#include <platform/csr_s3c6410.h>
#include <platform/csr_interrupt.h>
#include <platform/csr_dma.h>
#include <platform/csr_mmu.h>
#include "csr_ac97_data.h"

#define AC97_MODE_OFF()     REG_SET(rAC_GLBCTRL, 13, 12, 0)
#define AC97_MODE_DMA()     REG_SET(rAC_GLBCTRL, 13, 12, 0x3)

/* Static state data */
static CsrInt32 state = AC97_STATE_DISABLE;
static FrameBufferPcm *pendingList;
static FrameBufferPcm buffers[AC97_FRAMEBUFFER_SLOTS];
static DMAHandle dmaHandle;

static void eventDMA(DMAHandle *handle, CsrResult result);

static void startDMA(CsrUint32 length, CsrUint32 *data)
{
    DMAWrite(&dmaHandle, eventDMA, DMA_WIDTH_WORD, length, data);
}

static void eventDMA(DMAHandle *handle, CsrResult result)
{
    CsrUint32 *data;
    CsrUint8 paused;

    /* Free top element of list */
    data = pendingList->data;
    pendingList->data = NULL;

    paused = pendingList->paused;

    /* Callback */
    pendingList->callback(pendingList->length, data, TRUE);

    /* Pop off top element of list */
    pendingList = pendingList->next;

    /* If any elements are left in list, restart the DMA with next element */
    if ((pendingList != NULL) && (!paused))
    {
        startDMA(pendingList->length, pendingList->data);
    }
    else
    {
        /* Stop the host */
        AC97_MODE_OFF();
    }
}

CsrInt32 AC97PcmOutputStart(CsrUint8 dmaChannel)
{
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
        AC97PcmOutputStop();
    }

    /* Set started state */
    state = AC97_STATE_TRANSFER;

    /* Save selected DMA channel */
    dmaHandle.channel = dmaChannel;
    dmaHandle.source = DMA_SOURCE_AC97_PCMOUT;
    dmaHandle.sourceAddress = (void *) AC_PCMDATA;
    dmaHandle.sourceAddressIncrement = FALSE;

    /* Register relevant DMA handler function */
    DMAHandleRegister(&dmaHandle);

    return AC97_RESULT_SUCCESS;
}

CsrInt32 AC97PcmOutputStop(void)
{
    if (state != AC97_STATE_DISABLE)
    {
        CsrInt32 i;

        /* Stop DMA channel */
        DMAStop(&dmaHandle);

        /* Stop listening to DMA channel */
        DMAHandleUnregister(&dmaHandle);

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

CsrInt32 AC97PcmOutputPause(void)
{
    if (state == AC97_STATE_TRANSFER)
    {
        state = AC97_STATE_PAUSE;

        /* Stop the DMA (as a side-effect, this protects pendingList) */
        DMAStop(&dmaHandle);

        /* Retrieve DMA position if currently transfering */
        if (pendingList != NULL)
        {
            pendingList->current = DMASourceAddressGet(&dmaHandle);

            /* Mark this buffer as paused, to prevent any pending DMA events from resuming */
            pendingList->paused = TRUE;
        }

        /* Stop piping hardware FIFO data to AC-Link (shared register) */
        INT_ATOMIC_EXECUTE(AC97_MODE_OFF());
    }

    return AC97_RESULT_SUCCESS;
}

CsrInt32 AC97PcmOutputResume(void)
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

            /* Start data transfers from hardware FIFO again (shared register) */
            INT_ATOMIC_EXECUTE(AC97_MODE_DMA());
        }
    }

    return AC97_RESULT_SUCCESS;
}

CsrInt32 AC97PcmOutputEnqueue(CsrUint32 length, CsrUint32 *data, AC97PcmCallbackFunction callback)
{
    CsrInt32 i;
    FrameBufferPcm *ptr;

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
    MMUCleanDCacheRegion((CsrUint32) data, 4 * length);
    MMUDrainWriteBuffer();

    {
        INT_ATOMIC_EXECUTE_HEAD();

        DMAMask(&dmaHandle);

        /* Insert in pending list */
        if (pendingList == NULL)
        {
            /* Nothing in list, insert at start */
            pendingList = &buffers[i];

            /* Start the DMA with the given buffer (only if not in pause) */
            if (state != AC97_STATE_PAUSE)
            {
                startDMA(length, data);
                AC97_MODE_DMA();
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

        DMAUnmask(&dmaHandle);

        INT_ATOMIC_EXECUTE_TAIL();
    }

    return AC97_RESULT_SUCCESS;
}

void AC97PcmOutputCallbackMask(void)
{
    if (state != AC97_STATE_DISABLE)
    {
        DMAMask(&dmaHandle);
    }
}

void AC97PcmOutputCallbackUnmask(void)
{
    if (state != AC97_STATE_DISABLE)
    {
        DMAUnmask(&dmaHandle);
    }
}
