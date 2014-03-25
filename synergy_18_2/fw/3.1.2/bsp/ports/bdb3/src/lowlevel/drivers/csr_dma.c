/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <nucleus.h>

#include "csr_types.h"
#include "csr_macro.h"
#include "platform/csr_dma.h"
#include "platform/csr_interrupt.h"
#include "platform/csr_mmu.h"

/**********************************************/
/********* Constants **************************/
/**********************************************/
#define MAX_DMA_CONTROLLERS                         2
#define MAX_DMA_CHANNELS                            8

/**********************************************/
/********* States *****************************/
/**********************************************/
#define DMA_STATE_IDLE                              0
#define DMA_STATE_READ                              1
#define DMA_STATE_WRITE                             2

/**********************************************/
/********* DMAC Configuration *****************/
/**********************************************/
#define DMAC_ENABLE(cnt)                          {rDMACConfiguration(cnt) = 1 << 0;}
#define DMAC_DISABLE(cnt)                         {rDMACConfiguration(cnt) = 0 << 0;}

#define SDMA_SEL_SET(val)                         {rSDMA_SEL = (val);}

/**********************************************/
/******** DMAC Synchronization ****************/
/**********************************************/
#define DMAC_SYNC_ENABLE(cnt, src)                {rDMACSync(cnt) |= 0 << (src);}
#define DMAC_SYNC_DISABLE(cnt, src)               {rDMACSync(cnt) |= 1 << (src);}
#define DMAC_SYNC_ENABLE_ALL(cnt)                 {rDMACSync(cnt) = 0;}

/**********************************************/
/*** DMAC Current Source/Destination Address **/
/**********************************************/
#define DMAC_SRC_ADDR_SET(cnt, ch, addr)          {rDMACCSrcAddr(cnt, ch) = ((CsrUint32) (addr));}
#define DMAC_DEST_ADDR_SET(cnt, ch, addr)         {rDMACCDestAddr(cnt, ch) = ((CsrUint32) (addr));}
#define DMAC_SRC_ADDR_GET(cnt, ch)                ((void *) rDMACCSrcAddr(cnt, ch))

/**********************************************/
/*** DMAC Channel Linked Item Register ********/
/**********************************************/
#define DMAC_CLLI_SET(cnt, ch, val)               {rDMACCLLI(cnt, ch) = (val);}

/**********************************************/
/*** DMAC Channel Control *********************/
/**********************************************/
#define DMAC_CCONTROL0_TC_INTR_ENABLE                       (((CsrUint32) 1) << 31)
#define DMAC_CCONTROL0_TC_INTR_DISABLE                      (((CsrUint32) 0) << 31)
#define DMAC_CCONTROL0_PROTECTION_CACHEABLE_ENABLE          (1 << 30)
#define DMAC_CCONTROL0_PROTECTION_CACHEABLE_DISABLE         (0 << 30)
#define DMAC_CCONTROL0_PROTECTION_BUFFERED_ENABLE           (1 << 29)
#define DMAC_CCONTROL0_PROTECTION_BUFFERED_DISABLE          (0 << 29)
#define DMAC_CCONTROL0_PROTECTION_MODE_PRIVILGED            (1 << 28)
#define DMAC_CCONTROL0_PROTECTION_MODE_USER                 (0 << 28)
#define DMAC_CCONTROL0_DEST_INCREMENT_ENABLE                (1 << 27)
#define DMAC_CCONTROL0_DEST_INCREMENT_DISABLE               (0 << 27)
#define DMAC_CCONTROL0_SRC_INCREMENT_ENABLE                 (1 << 26)
#define DMAC_CCONTROL0_SRC_INCREMENT_DISABLE                (0 << 26)
#define DMAC_CCONTROL0_DEST_MASTER_SELECT_PERI              (1 << 25)
#define DMAC_CCONTROL0_DEST_MASTER_SELECT_SYSTEM            (0 << 25)
#define DMAC_CCONTROL0_SRC_MASTER_SELECT_PERI               (1 << 24)
#define DMAC_CCONTROL0_SRC_MASTER_SELECT_SYSTEM             (0 << 24)
#define DMAC_CCONTROL0_DEST_WIDTH(width)                    ((width) << 21)
#define DMAC_CCONTROL0_SRC_WIDTH(width)                     ((width) << 18)
#define DMAC_CCONTROL0_DEST_BURST_SIZE_BYTE                 (0 << 15)
#define DMAC_CCONTROL0_SRC_BURST_SIZE_BYTE                  (0 << 12)

#define DMAC_CCONTROL0_DEFAULT                              (DMAC_CCONTROL0_TC_INTR_ENABLE | \
                                                             DMAC_CCONTROL0_PROTECTION_CACHEABLE_DISABLE | \
                                                             DMAC_CCONTROL0_PROTECTION_BUFFERED_DISABLE | \
                                                             DMAC_CCONTROL0_PROTECTION_MODE_PRIVILGED | \
                                                             DMAC_CCONTROL0_DEST_BURST_SIZE_BYTE | \
                                                             DMAC_CCONTROL0_SRC_BURST_SIZE_BYTE)

#define DMAC_CCONTROL0_SET(cnt, ch, di, si, ds, ss, width)  {rDMACCControl0(cnt, ch) = \
                                                                 DMAC_CCONTROL0_DEFAULT | \
                                                                 (di) | \
                                                                 (si) | \
                                                                 (ds) | \
                                                                 (ss) | \
                                                                 DMAC_CCONTROL0_DEST_WIDTH(width) | \
                                                                 DMAC_CCONTROL0_SRC_WIDTH(width);}

#define DMAC_CCONTROL1_SET(cnt, ch, size)                   {rDMACCControl1(cnt, ch) = (size);}

/**********************************************/
/*** DMAC Channel Configuration ***************/
/**********************************************/
#define DMAC_CCONFIGURATION_DMA_REQ_HALT                    (1 << 18)
#define DMAC_CCONFIGURATION_DMA_REQ_ALLOW                   (0 << 18)
#define DMAC_CCONFIGURATION_LOCKED_TRANSFER_ENABLE          (1 << 16)
#define DMAC_CCONFIGURATION_LOCKED_TRANSFER_DISABLE         (0 << 16)
#define DMAC_CCONFIGURATION_INTR_TC_ENABLE                  (1 << 15)
#define DMAC_CCONFIGURATION_INTR_TC_DISABLE                 (0 << 15)
#define DMAC_CCONFIGURATION_INTR_ERROR_ENABLE               (1 << 14)
#define DMAC_CCONFIGURATION_INTR_ERROR_DISABLE              (0 << 14)
#define DMAC_CCONFIGURATION_FLOW_CTRL_MEM2MEM               (0 << 11)
#define DMAC_CCONFIGURATION_FLOW_CTRL_MEM2PERI              (1 << 11)
#define DMAC_CCONFIGURATION_FLOW_CTRL_PERI2MEM              (2 << 11)
#define DMAC_CCONFIGURATION_FLOW_CTRL_PERI2PERI             (3 << 11)
#define DMAC_CCONFIGURATION_DEST_ONE_NAND_MODE_ENABLE       (1 << 10)
#define DMAC_CCONFIGURATION_DEST_ONE_NAND_MODE_DISABLE      (0 << 10)
#define DMAC_CCONFIGURATION_DEST_PERIPHERAL(peri)           ((peri) << 6)
#define DMAC_CCONFIGURATION_SRC_ONE_NAND_MODE_ENABLE        (1 << 5)
#define DMAC_CCONFIGURATION_SRC_ONE_NAND_MODE_DISABLE       (0 << 5)
#define DMAC_CCONFIGURATION_SRC_PERIPHERAL(peri)            ((peri) << 1)
#define DMAC_CCONFIGURATION_CHANNEL_ENABLE                  (1 << 0)
#define DMAC_CCONFIGURATION_CHANNEL_DISABLE                 (0 << 0)

#define DMAC_CCONFIGURATION_DEFAULT                         (DMAC_CCONFIGURATION_DMA_REQ_ALLOW | \
                                                             DMAC_CCONFIGURATION_LOCKED_TRANSFER_DISABLE | \
                                                             DMAC_CCONFIGURATION_INTR_TC_ENABLE | \
                                                             DMAC_CCONFIGURATION_INTR_ERROR_ENABLE | \
                                                             DMAC_CCONFIGURATION_DEST_ONE_NAND_MODE_DISABLE | \
                                                             DMAC_CCONFIGURATION_SRC_ONE_NAND_MODE_DISABLE | \
                                                             DMAC_CCONFIGURATION_CHANNEL_DISABLE)

#define DMAC_CCONFIGURATION_SET(cnt, ch, flow, dp, sp)      {rDMACCConfiguration(cnt, ch) = \
                                                                 DMAC_CCONFIGURATION_DEFAULT | \
                                                                 (flow) | \
                                                                 (dp) | \
                                                                 (sp);}
/* DMAC Controller Operations */
#define DMAC_START(cnt, chan)                     {rDMACCConfiguration(cnt, chan) |= \
                                                       DMAC_CCONFIGURATION_CHANNEL_ENABLE;}

#define DMAC_STOP(cnt, chan)                      {rDMACCConfiguration(cnt, chan) &= \
                                                       ~DMAC_CCONFIGURATION_CHANNEL_ENABLE;}

#define DMAC_MASK(cnt, chan)                      {rDMACCConfiguration(cnt, chan) &= \
                                                       ~(DMAC_CCONFIGURATION_INTR_TC_ENABLE | \
                                                         DMAC_CCONFIGURATION_INTR_ERROR_ENABLE);}

#define DMAC_UNMASK(cnt, chan)                    {rDMACCConfiguration(cnt, chan) |= \
                                                       DMAC_CCONFIGURATION_INTR_TC_ENABLE | \
                                                       DMAC_CCONFIGURATION_INTR_ERROR_ENABLE;}

#define DMAC_INT_TC_CLEAR(cnt, chan)              {rDMACIntTCClear(cnt) = 1 << (chan);}
#define DMAC_INT_ERR_CLEAR(cnt, chan)             {rDMACIntErrClr(cnt) = 1 << (chan);}
#define DMAC_INT_CLEAR_MASK(cnt, mask)            {rDMACIntTCClear(cnt) = (mask); \
                                                   rDMACIntErrClr(cnt) = (mask);}
#define DMAC_INT_CLEAR_ALL(cnt)                   {rDMACIntTCClear(cnt) = 0xFF; \
                                                   rDMACIntErrClr(cnt) = 0xFF;}

typedef struct
{
    DMAHandle *handle;
    CsrUint8   controller;
    CsrUint32  peripheral;

    /* Client DMA operation complete callback */
    CsrUint8            state;
    DMACallbackFunction function;
    DMAWidth            width;
    CsrUint32           length;
    void               *data;
} DMAHandleContext;

static CsrBool controllersInitialised[MAX_DMA_CONTROLLERS]; /* Implicitly initialised to FALSE */
static DMAHandleContext contexts[MAX_DMA_CONTROLLERS][MAX_DMA_CHANNELS]; /* Implicitly initialised to NULL */

static CsrUint8 getNumberOfRegisteredHandlers(CsrUint8 controller)
{
    CsrUint8 handlerIdx = 0;
    CsrUint8 handlerCount = 0;

    for ( ; handlerIdx < CSR_ARRAY_SIZE(contexts[controller]); ++handlerIdx)
    {
        if (contexts[controller][handlerIdx].handle != NULL)
        {
            handlerCount++;
        }
    }

    return handlerCount;
}

static VOID dmaIrq(INT interruptNumber)
{
    CsrUint8 controller = interruptNumber - INT_DMA0;
    CsrUint8 intTcStatus = (CsrUint8) rDMACIntTCStatus(controller);
    CsrUint8 intErrorStatus = (CsrUint8) rDMACIntErrorStatus(controller);
    CsrUint8 channel;

    /* Clear Terminal Count and Error interrupt register */
    DMAC_INT_CLEAR_MASK(controller, intTcStatus | intErrorStatus);

    for (channel = 0; channel < MAX_DMA_CHANNELS; ++channel)
    {
        if ((intTcStatus & (1 << channel)) || (intErrorStatus & (1 << channel)))
        {
            DMAHandleContext *context = &contexts[controller][channel];
            DMAHandle *dmaHandle = context->handle;

            /* Stop DMA */
            DMAC_STOP(controller, channel);

            if (context->function != NULL)
            {
                CsrResult result = (intErrorStatus & (1 << channel))
                                   ? CSR_RESULT_FAILURE
                                   : CSR_RESULT_SUCCESS;

                if (context->state == DMA_STATE_READ)
                {
                    MMUInvalidateDCacheRegion((CsrUint32) context->data, context->length * (1 << context->width));
                }
                else if ((dmaHandle->source == DMA_SOURCE_MEMORY) && (context->state == DMA_STATE_WRITE))
                {
                    MMUInvalidateDCacheRegion((CsrUint32) dmaHandle->sourceAddress, context->length * (1 << context->width));
                }

                context->state = DMA_STATE_IDLE;
                context->function(context->handle, result);
            }
        }
    }

    INT_CLEAR();
}

static CsrBool verifyDMAHandle(DMAHandle *handle)
{
    DMAHandleContext *context;

    if (handle == NULL)
    {
        return FALSE;
    }

    if (handle->channel >= MAX_DMA_CHANNELS)
    {
        return FALSE;
    }

    if (handle->source > DMA_SOURCE_MEMORY)
    {
        return FALSE;
    }

    if ((context = handle->priv) == NULL)
    {
        return FALSE;
    }

    if (context->handle != handle)
    {
        return FALSE;
    }

    return TRUE;
}

static CsrBool verifyDMAHandleRegister(DMAHandle *handle)
{
    if (handle == NULL)
    {
        return FALSE;
    }

    if (handle->channel >= MAX_DMA_CHANNELS)
    {
        return FALSE;
    }

    if (handle->source > DMA_SOURCE_MEMORY)
    {
        return FALSE;
    }

    return TRUE;
}

static CsrUint8 getController(DMASource source)
{
    static const CsrUint8 map[] =
    {
        /********************/
        /* DMA Controller 0 */
        /********************/
        0, /* DMA_SOURCE_SPI0_TX */
        0, /* DMA_SOURCE_SPI0_RX */

        /********************/
        /* DMA Controller 1 */
        /********************/
        1, /* DMA_SOURCE_AC_PCMOUT */
        1, /* DMA_SOURCE_AC_PCMIN */
        1, /* DMA_SOURCE_AC_MICIN */
        1, /* DMA_SOURCE_MEMORY */
    };

    return map[source];
}

static CsrUint8 getPeripheralArgument(DMASource source)
{
    static const CsrUint8 map[] =
    {
        /********************/
        /* DMA Controller 0 */
        /********************/
        12, /* DMA_SOURCE_SPI0_TX */
        13, /* DMA_SOURCE_SPI0_RX */

        /********************/
        /* DMA Controller 1 */
        /********************/
        6,  /* DMA_SOURCE_AC_PCMOUT */
        7,  /* DMA_SOURCE_AC_PCMIN */
        8,  /* DMA_SOURCE_AC_MICIN */
        /* Note: these values are ignored */
        0,  /* DMA_SOURCE_MEMORY */
    };

    return map[source];
}

CsrResult DMAHandleRegister(DMAHandle *handle)
{
    CsrUint8 controller;
    CsrUint8 channel;

    INT_ATOMIC_EXECUTE_HEAD();

    if (!verifyDMAHandleRegister(handle))
    {
        /* Invalid arguments */
        INT_ATOMIC_EXECUTE_TAIL();
        return CSR_RESULT_FAILURE;
    }

    channel = handle->channel;
    controller = getController(handle->source);
    if (contexts[controller][channel].handle != NULL)
    {
        /* Channel is already in use */
        INT_ATOMIC_EXECUTE_TAIL();
        return CSR_RESULT_FAILURE;
    }

    if (!controllersInitialised[controller])
    {
        static CsrBool dmaSystemInitiased = FALSE;
        if (!dmaSystemInitiased)
        {
            /* For now, always use general DMA */
            SDMA_SEL_SET(0x0FFFFFFF);
        }

        /* Enable DMA controller */
        DMAC_ENABLE(controller);
        /* Fow now, synchronize all DMA request signals */
        DMAC_SYNC_ENABLE_ALL(controller);
        controllersInitialised[controller] = TRUE;
    }

    if (getNumberOfRegisteredHandlers(controller) == 0)
    {
        VOID (*old)(INT);
        NU_Register_LISR(INT_DMA0 + controller, dmaIrq, &old);
        INT_UNMASK(INT_DMA0 + controller);
    }

    handle->priv = &contexts[controller][channel];
    contexts[controller][channel].handle = handle;
    contexts[controller][channel].controller = controller;
    contexts[controller][channel].peripheral = getPeripheralArgument(handle->source);
    contexts[controller][channel].state = DMA_STATE_IDLE;

    /* Clear Terminal Count and Error interrupt register */
    DMAC_INT_TC_CLEAR(controller, channel);
    DMAC_INT_ERR_CLEAR(controller, channel);

    INT_ATOMIC_EXECUTE_TAIL();

    return CSR_RESULT_SUCCESS;
}

void DMAHandleUnregister(DMAHandle *handle)
{
    INT_ATOMIC_EXECUTE_HEAD();

    if (verifyDMAHandle(handle))
    {
        DMAHandleContext *context = handle->priv;

        /* Stop any ongoing transfer (if any) */
        DMAStop(handle);

        context->handle = NULL;
        handle->priv = NULL;

        if (getNumberOfRegisteredHandlers(context->controller) == 0)
        {
            VOID (*old)(INT);
            NU_Register_LISR(INT_DMA0 + context->controller, NU_NULL, &old);
            /* No registered handlers so mask all IRQ */
            INT_MASK(INT_DMA0 + context->controller);
            /* Disable DMA controller */
            DMAC_DISABLE(context->controller);
            controllersInitialised[context->controller] = FALSE;
        }
    }

    INT_ATOMIC_EXECUTE_TAIL();
}

void DMAWrite(DMAHandle *handle, DMACallbackFunction function, DMAWidth width, CsrUint32 length, void *data)
{
    if (verifyDMAHandle(handle))
    {
        DMAHandleContext *context = handle->priv;
        CsrUint8 controller = context->controller;
        CsrUint8 channel = handle->channel;

        /* Store client operation complete callback function */
        context->function = function;

        /* Clear Terminal Count and Error interrupt register */
        DMAC_INT_TC_CLEAR(controller, channel);
        DMAC_INT_ERR_CLEAR(controller, channel);

        /* Write source/destination address */
        DMAC_SRC_ADDR_SET(controller, channel, data);
        DMAC_DEST_ADDR_SET(controller, channel, handle->sourceAddress);

        /* Setup DMAC Channel Control Register */
        DMAC_CCONTROL0_SET(controller, channel,
            handle->sourceAddressIncrement
            ? DMAC_CCONTROL0_DEST_INCREMENT_ENABLE
            : DMAC_CCONTROL0_DEST_INCREMENT_DISABLE,
            DMAC_CCONTROL0_SRC_INCREMENT_ENABLE,
            handle->source == DMA_SOURCE_MEMORY
            ? DMAC_CCONTROL0_DEST_MASTER_SELECT_SYSTEM
            : DMAC_CCONTROL0_DEST_MASTER_SELECT_PERI,
            DMAC_CCONTROL0_SRC_MASTER_SELECT_SYSTEM,
            width);

        /* Set transfer size */
        DMAC_CCONTROL1_SET(controller, channel, length);

        /* Setup DMAC Channel Configuration Register */
        DMAC_CCONFIGURATION_SET(controller, channel,
            handle->source == DMA_SOURCE_MEMORY
            ? DMAC_CCONFIGURATION_FLOW_CTRL_MEM2MEM
            : DMAC_CCONFIGURATION_FLOW_CTRL_MEM2PERI,
            DMAC_CCONFIGURATION_DEST_PERIPHERAL(context->peripheral),
            DMAC_CCONFIGURATION_SRC_PERIPHERAL(0 /* ignored on MEM -> PERI/MEM */));

        /* Writeback from cache to main memory before starting the DMA */
        MMUCleanDCacheRegion((CsrUint32) data, length * (1 << width));
        if (handle->source == DMA_SOURCE_MEMORY)
        {
            MMUCleanDCacheRegion((CsrUint32) handle->sourceAddress, length * (1 << width));
        }
        MMUDrainWriteBuffer();

        /* Setup context and start DMA transfer */
        context->state = DMA_STATE_WRITE;
        context->function = function;
        context->width = width;
        context->length = length;
        context->data = NULL;
        DMAC_START(controller, channel);
    }
}

void DMARead(DMAHandle *handle, DMACallbackFunction function, DMAWidth width, CsrUint32 length, void *data)
{
    if (verifyDMAHandle(handle))
    {
        DMAHandleContext *context = handle->priv;
        CsrUint8 controller = context->controller;
        CsrUint8 channel = handle->channel;

        /* Store client operation complete callback function */
        context->function = function;

        /* Clear Terminal Count and Error interrupt register */
        DMAC_INT_TC_CLEAR(controller, channel);
        DMAC_INT_ERR_CLEAR(controller, channel);

        /* Write source/destination address */
        DMAC_SRC_ADDR_SET(controller, channel, handle->sourceAddress);
        DMAC_DEST_ADDR_SET(controller, channel, data);

        /* Setup DMAC Channel Control Register */
        DMAC_CCONTROL0_SET(controller, channel,
            DMAC_CCONTROL0_DEST_INCREMENT_ENABLE,
            handle->sourceAddressIncrement
            ? DMAC_CCONTROL0_SRC_INCREMENT_ENABLE
            : DMAC_CCONTROL0_SRC_INCREMENT_DISABLE,
            DMAC_CCONTROL0_DEST_MASTER_SELECT_SYSTEM,
            handle->source == DMA_SOURCE_MEMORY
            ? DMAC_CCONTROL0_SRC_MASTER_SELECT_SYSTEM
            : DMAC_CCONTROL0_SRC_MASTER_SELECT_PERI,
            width);

        /* Set transfer size */
        DMAC_CCONTROL1_SET(controller, channel, length);

        /* Setup DMAC Channel Configuration Register */
        DMAC_CCONFIGURATION_SET(controller, channel,
            handle->source == DMA_SOURCE_MEMORY
            ? DMAC_CCONFIGURATION_FLOW_CTRL_MEM2MEM
            : DMAC_CCONFIGURATION_FLOW_CTRL_PERI2MEM,
            DMAC_CCONFIGURATION_DEST_PERIPHERAL(0 /* ignored on PERI/MEM -> MEM */),
            DMAC_CCONFIGURATION_SRC_PERIPHERAL(context->peripheral));

        /* Writeback from cache to main memory before starting the DMA */
        MMUCleanDCacheRegion((CsrUint32) data, length * (1 << width));
        if (handle->source == DMA_SOURCE_MEMORY)
        {
            MMUCleanDCacheRegion((CsrUint32) handle->sourceAddress, length * (1 << width));
        }
        MMUDrainWriteBuffer();

        /* Setup context and start DMA transfer */
        context->state = DMA_STATE_READ;
        context->function = function;
        context->width = width;
        context->length = length;
        context->data = data;
        DMAC_START(controller, channel);
    }
}

void DMAMask(DMAHandle *handle)
{
    if (verifyDMAHandle(handle))
    {
        DMAHandleContext *context = handle->priv;
        DMAC_MASK(context->controller, handle->channel);
    }
}

void DMAUnmask(DMAHandle *handle)
{
    if (verifyDMAHandle(handle))
    {
        DMAHandleContext *context = handle->priv;
        DMAC_UNMASK(context->controller, handle->channel);
    }
}

void DMAStop(DMAHandle *handle)
{
    if (verifyDMAHandle(handle))
    {
        DMAHandleContext *context = handle->priv;
        /* Stop ongoing DMA transfer. All data in the FIFO's will be lost */
        DMAMask(handle);
        DMAC_STOP(context->controller, handle->channel);
    }
}

void *DMASourceAddressGet(DMAHandle *handle)
{
    if (verifyDMAHandle(handle))
    {
        DMAHandleContext *context = handle->priv;
        return DMAC_SRC_ADDR_GET(context->controller, handle->channel);
    }
    else
    {
        return NULL;
    }
}
