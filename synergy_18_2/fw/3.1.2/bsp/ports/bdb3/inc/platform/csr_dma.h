#ifndef CSR_DMA_H__
#define CSR_DMA_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_result.h"

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *      DMASource
 *
 *  DESCRIPTION
 *      This type denotes a DMA source attached to a DMA controller.
 *      How these DMA sources are attached to the available DMA controllers
 *      is an implementation detail.
 *
 *      Note: The DMA_SOURCE_MEMORY source can do DMA transfer in both
 *      directions
 *
 *----------------------------------------------------------------------------*/
typedef CsrUint8 DMASource;
#define DMA_SOURCE_SPI0_TX              ((DMASource) 0x0)
#define DMA_SOURCE_SPI0_RX              ((DMASource) 0x1)
#define DMA_SOURCE_AC97_PCMOUT          ((DMASource) 0x2)
#define DMA_SOURCE_AC97_PCMIN           ((DMASource) 0x3)
#define DMA_SOURCE_AC97_MICIN           ((DMASource) 0x4)
#define DMA_SOURCE_MEMORY               ((DMASource) 0x5)

/*----------------------------------------------------------------------------*
 *  NAME
 *      DMAWidth
 *
 *  DESCRIPTION
 *      A DMA operation can be done using 8/16/32 bit width operations.
 *
 *----------------------------------------------------------------------------*/
typedef CsrUint32 DMAWidth;
#define DMA_WIDTH_BYTE                  ((DMAWidth) 0x0)
#define DMA_WIDTH_HALFWORD              ((DMAWidth) 0x1)
#define DMA_WIDTH_WORD                  ((DMAWidth) 0x2)

/*----------------------------------------------------------------------------*
 *  NAME
 *      DMAHandle
 *
 *  DESCRIPTION
 *      This structure represents a handle that ties together a
 *      DMA source and channel. All DMA operations require a valid
 *      DMA handle. All members except 'priv' needs to be initialized
 *      by the caller.
 *
 *  MEMBERS
 *      source - The DMA source.
 *      channel - The (prioritized) channel.
 *      sourceAddress - The DMA source address (the peripheral register address)
 *                   used for DMA operations.
 *      sourceAddressIncrement - Indicate whether 'sourceAddress' should be
 *                               incremented during DMA operations.
 *      driverData - For use by the upper layer.
 *      priv - For use by the lower layer (DMA implementation).
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    DMASource source;
    CsrUint8  channel;
    void     *sourceAddress;
    CsrBool   sourceAddressIncrement;
    void     *driverData;
    void     *priv;
} DMAHandle;

/*----------------------------------------------------------------------------*
 *  NAME
 *      DMACallbackFunction
 *
 *  DESCRIPTION
 *      This callback is used to signal an operation completion.
 *
 *  PARAMETERS
 *      handle - DMA handle for the operation which just completed.
 *      result - The result of the operation.
 *
 *----------------------------------------------------------------------------*/
typedef void (*DMACallbackFunction)(DMAHandle *handle, CsrResult result);

/*----------------------------------------------------------------------------*
 *  NAME
 *      DMARegisterHandler, DMAUnregisterHandler
 *
 *  DESCRIPTION
 *      Register/unregister a DMA handle. The registration will fail if any of
 *      the parameters are invalid or if the channel is already occupied.
 *
 *  PARAMETERS
 *      handle - DMA handle that ties together the DMA source and channel.
 *
 *  RETURNS (DMARegisterHandler only)
 *      CSR_RESULT_SUCCESS - The handle was registered successfully.
 *      CSR_RESULT_FAILURE - One or more parameters were invalid, or the handle
 *                           has already been registered.
 *
 *----------------------------------------------------------------------------*/
CsrResult DMAHandleRegister(DMAHandle *handle);
void DMAHandleUnregister(DMAHandle *handle);

/*----------------------------------------------------------------------------*
 *  NAME
 *      DMAWrite
 *
 *  DESCRIPTION
 *      Perform a DMA Write operation (from memory to peripheral).
 *
 *      Note: The chosen 'width' and 'data' pointer needs to consistent
 *            (properly aligned)
 *
 *  PARAMETERS
 *      handle - DMA handle that ties together the DMA source and channel.
 *      function - Function which will be invoked when operation completes.
 *      width - The width of the transfer
 *      length - The length of the transfer (in units of 'width')
 *      data - Pointer to transmit buffer.
 *
 *----------------------------------------------------------------------------*/
void DMAWrite(DMAHandle *handle, DMACallbackFunction function, DMAWidth width, CsrUint32 length, void *data);

/*----------------------------------------------------------------------------*
 *  NAME
 *      DMARead
 *
 *  DESCRIPTION
 *      Perform a DMA Read operation (from peripheral to memory)
 *
 *      Note: The chosen 'width' and 'data' pointer needs to consistent
 *            (properly aligned)
 *
 *  PARAMETERS
 *      handle - DMA handle that ties together the DMA source and channel.
 *      function - Function which will be invoked when operation completes.
 *      width - The width of the transfer
 *      length - The length of the transfer (in units of 'width')
 *      data - Pointer to receive buffer.
 *
 *----------------------------------------------------------------------------*/
void DMARead(DMAHandle *handle, DMACallbackFunction function, DMAWidth width, CsrUint32 length, void *data);

/*----------------------------------------------------------------------------*
 *  NAME
 *      DMASourceAddressGet
 *
 *  DESCRIPTION
 *      Retrieve the current source address.
 *
 *  PARAMETERS
 *      handle - DMA handle that ties together the DMA source and channel.
 *
 *  RETURNS
 *      The current source address of the specified DMA source.
 *
 *----------------------------------------------------------------------------*/
void *DMASourceAddressGet(DMAHandle *handle);

/*----------------------------------------------------------------------------*
 *  NAME
 *      DMAMask
 *
 *  DESCRIPTION
 *      Mask the DMACallbackFunction for the corresponding DMA source.
 *
 *  PARAMETERS
 *      handle - DMA handle that ties together the DMA source and channel.
 *
 *----------------------------------------------------------------------------*/
void DMAMask(DMAHandle *handle);

/*----------------------------------------------------------------------------*
 *  NAME
 *      DMAUnmask
 *
 *  DESCRIPTION
 *      Unmask the DMACallbackFunction for the corresponding DMA source.
 *
 *  PARAMETERS
 *      handle - DMA handle that ties together the DMA source and channel.
 *
 *----------------------------------------------------------------------------*/
void DMAUnmask(DMAHandle *handle);

/*----------------------------------------------------------------------------*
 *  NAME
 *      DMAStop
 *
 *  DESCRIPTION
 *      Stop the transfer corresponding to the specified DMA handle.
 *
 *  PARAMETERS
 *      handle - DMA handle that ties together the DMA source and channel.
 *
 *----------------------------------------------------------------------------*/
void DMAStop(DMAHandle *handle);

#ifdef __cplusplus
}
#endif

#endif
