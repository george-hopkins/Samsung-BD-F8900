#ifndef CSR_DMA_H__
#define CSR_DMA_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "platform/csr_s3c24a0.h"
#include "platform/csr_interrupt.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Callback function type */
typedef void (*DMACallbackFunction)(void);

/* Register a DMA handler. Will be called when the
   corresponding DMA channel generates an interrupt. (thread-safe) */
void DMARegisterHandler(CsrUint32 channel, DMACallbackFunction function);

/* Masking/unmasking of individual DMA sources */
#define DMA_UNMASK_CHANNEL(channel)         INT_SUB_UNMASK(INT_SUB_DMA0 + (channel & 0x3))
#define DMA_UNMASK_CHANNEL_ATOMIC(channel)  INT_SUB_UNMASK_ATOMIC(INT_SUB_DMA0 + (channel & 0x3))
#define DMA_MASK_CHANNEL(channel)           INT_SUB_MASK(INT_SUB_DMA0 + (channel & 0x3))
#define DMA_MASK_CHANNEL_ATOMIC(channel)    INT_SUB_MASK_ATOMIC(INT_SUB_DMA0 + (channel & 0x3))

/* Clear a pending interrupt. This is done automatically, and is only applicable
   for cancelling a pending callback while the callback is masked. */
#define DMA_PENDING_CLEAR(channel)      {INT_SUB_CLEAR(INT_SUB_DMA0 + (channel & 0x3)); \
                                         INT_CLEAR(INT_DMA);}

/* DMA Operation Instructions:
    1) Register a DMA handler using DMARegisterHandler()
    2) Set Source and Destination Address Register
    3) Set Source and Destination Control Register
    4) Set DMA Control Register (must use DMA_CON_INT for handler to be called)
    5) Turn on DMA (Use the DMA_X_ON() macro)
    6) Optionally Trigger the DMA request (if using DMA_CON_SOFTTRIGGER) */

/* Source Address Register */
#define DMA_SOURCE_ADDRESS_SET(channel, source)             {rDISRC(channel) = (unsigned int) (source);}

/* Destination Address Register */
#define DMA_DESTINATION_ADDRESS_SET(channel, destination)   {rDIDST(channel) = (unsigned int) (destination);}

/* Source Control Register
    bus: 0=AHB, 1=APB
    fix: 0=increment, 1=fixed */
#define DMA_SOURCE_CONTROL_SET(channel, bus, fix)           {rDISRCC(channel) = (((bus) & 1) << 1) | ((fix) & 1);}

/* Destination Control Register
    irq: 0=interrupt at TC=0, 1=interrupt after auto-reload
    bus: 0=AHB, 1=APB
    fix: 0=increment, 1=fixed */
#define DMA_DESTINATION_CONTROL_SET(channel, irq, bus, fix) {rDIDSTC(channel) = (((irq) & 1) << 2) | (((bus) & 1) << 1) | ((fix) & 1);}

/* Options for DMA Control Register */
#define DMA_CON_DEMAND              ((CsrUint32) 0 << 31)   /* Demand Mode */
#define DMA_CON_HANDSHAKE           ((CsrUint32) 1 << 31)   /* Handshake Mode (Recommended for External) */
#define DMA_CON_SYNC_APB            (0 << 30)               /* Synchronise DMA req/ack with PCLK */
#define DMA_CON_SYNC_AHB            (1 << 30)               /* Synchronise DMA req/ack with HCLK */
#define DMA_CON_INT                 (1 << 29)               /* Enable interrupt at operation completion */
#define DMA_CON_BURST               (1 << 28)               /* Enable burst4 mode */
#define DMA_CON_SINGLESERVICE       (0 << 27)               /* Enable single-service mode */
#define DMA_CON_WHOLESERVICE        (1 << 27)               /* Enable whole-service mode */
#define DMA_CON_SOFTTRIGGER         (0 << 23)               /* Trigger DMA request by software */
#define DMA_CON_HARDTRIGGER         (1 << 23)               /* Trigger DMA request by hardware */
#define DMA_CON_NOAUTORELOAD        (1 << 22)               /* Disable Auto Reload */
#define DMA_CON_BYTE                (0 << 20)               /* Byte data unit size */
#define DMA_CON_HALFWORD            (1 << 20)               /* Half-word data unit size */
#define DMA_CON_WORD                (2 << 20)               /* Word data unit size */
#define DMA_CON_ACCESS_SIZE(size)   (size << 20)            /* Specified unit size, valid values: 0=BYTE, 1=HALFWORD, 2=WORD */

#define DMA_CON_DEFAULT             (DMA_CON_HANDSHAKE | DMA_CON_SYNC_APB | DMA_CON_INT | DMA_CON_HARDTRIGGER | DMA_CON_NOAUTORELOAD | DMA_CON_WORD)

/* DMA Control Register
    options: Logical or of above options
    hwsrcsel: Controls 8-1 MUX of the DMA request routing (requires option DMA_CON_HARDTRIGGER)
    tc: Number of units to transfer (Data unit size * tc bytes - multiplied by 4 if DMA_CON_BURST is set) */
#define DMA_CON_SET(channel, options, hwsrcsel, tc)         {rDCON(channel) = (options) | ((hwsrcsel) << (24)) | (tc);}

/* DMA Trigger */
#define DMA_STOP(channel)       {rDMASKTRIG(channel) = 4;}
#define DMA_ON(channel)         {rDMASKTRIG(channel) = 2;}
#define DMA_STOP_WAIT(channel)  {DMA_STOP(channel); \
                                 while (BIT_CHECK(rDMASKTRIG(channel), 1)) {}}

/* Current Source Register */
#define DMA_CURRENT_SOURCE_GET(channel)         ((void *) rDCSRC(channel))

#ifdef __cplusplus
}
#endif

#endif
