#ifndef CSR_SERIAL_COM_H__
#define CSR_SERIAL_COM_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_sched.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************

    NAME
        CsrUartDrvDataRx

    DESCRIPTION
        This callback type is used for the callback that is registerd in the
        CsrUartDrvRegister function and called by the CsrUartDrvRx function.
        Please refer to the description of these functions for further
        information.

    PARAMETERS
        data - pointer to the data.
        dataLength - the number of contiguous data bytes available.

    RETURNS
        The number of data bytes consumed from the data pointer.

*****************************************************************************/
typedef CsrUint32 (*CsrUartDrvDataRx)(const CsrUint8 *data, CsrUint32 dataLength);


/*****************************************************************************

    NAME
        CsrUartDrvStart

    DESCRIPTION
        This function is called to start (open) the UART driver.

    PARAMETERS
        handle - handle of UART driver instance.
        reset - number of previous resets. This can be used by the
            UART driver to determine the baud rate to use.

    RETURNS
        TRUE if the UART driver was successfully started and FALSE otherwise.

*****************************************************************************/
CsrBool CsrUartDrvStart(void *handle, CsrUint8 reset);


/*****************************************************************************

    NAME
        CsrUartDrvStop

    DESCRIPTION
        This function is called to stop (close) the UART driver. It undoes all
        actions performed by CsrUartDrvStart, freeing any allocated resources.

    PARAMETERS
        handle - handle of UART driver instance.

    RETURNS
        TRUE if UART driver was successfully stopped and FALSE otherwise.

*****************************************************************************/
CsrBool CsrUartDrvStop(void *handle);


/*****************************************************************************

    NAME
        CsrUartDrvGetTxSpace

    DESCRIPTION
        Obtain amount of available free space in the UART driver transmit
        buffer. Should be able to buffer at least this number of bytes in a
        call to CsrUartDrvTx.

    PARAMETERS
        handle - handle of UART driver instance.

    RETURNS
        The number of bytes that can be written to the UART driver transmit
        buffer.

*****************************************************************************/
CsrUint32 CsrUartDrvGetTxSpace(void *handle);


/*****************************************************************************

    NAME
        CsrUartDrvTx

    DESCRIPTION
        Buffer the supplied data for transmission. If the free space in the
        transmit buffer is less than length of the specified data, the
        implementation shall either attempt to buffer as many bytes as
        possible and return the actual number of bytes buffered in the numSent
        variable, or simply set numSent to 0 and in both cases return FALSE.

    PARAMETERS
        handle - handle of UART driver instance.
        data - pointer to data.
        dataLength - number of bytes to buffer for transmission.
        numSent - the actual number of bytes buffered is returned in this
            variable.

    RETURNS
        Returns TRUE if all the supplied data has been buffered (numSent is
        equal to dataLength), else return FALSE.

*****************************************************************************/
CsrBool CsrUartDrvTx(void *handle, const CsrUint8 *data, CsrUint32 dataLength, CsrUint32 *numSent);


/*****************************************************************************

    NAME
        CsrUartDrvGetRxAvailable

    DESCRIPTION
        Obtain amount of data available in the UART driver receive buffer.
        Should be able to receive at least this number of bytes in a call to
        the CsrUartDrvRx or CsrUartDrvLowLevelTransportRx function.

    PARAMETERS
        handle - handle of UART driver instance.

    RETURNS
        The number of bytes that can be read from the UART driver receive
        buffer.

*****************************************************************************/
CsrUint32 CsrUartDrvGetRxAvailable(void *handle);


/*****************************************************************************

    NAME
        CsrUartDrvRx

    DESCRIPTION
        Calling this function will in turn cause the CsrUartDrvDataRx callback
        function registered in the call to CsrUartDrvRegister to be called.
        The CsrUartDrvDataRx callback function passes a pointer (and length)
        to the upper layer. The upper layer may either process this data
        in-place or copy it to a local buffer for later processing. The return
        value from the CsrUartDrvDataRx function indicates the number of bytes
        that was consumed (regardles of whether it was processed or copied) by
        the upper layer.

    PARAMETERS
        handle - handle of UART driver instance.

*****************************************************************************/
void CsrUartDrvRx(void *handle);


/*****************************************************************************

    NAME
        CsrUartDrvLowLevelTransportRx

    DESCRIPTION
        Copies up to the specified number of bytes from the receive buffer to
        the supplied data buffer.

    PARAMETERS
        handle - handle of UART driver instance.
        dataLength - the number of byte to retrieve.
        data - buffer to copy the data into.

    RETURNS
        The number of bytes actually copied. Note that this may be less than
        the length of the supplied buffer.

*****************************************************************************/
CsrUint32 CsrUartDrvLowLevelTransportRx(void *handle, CsrUint32 dataLength, CsrUint8 *data);


/*****************************************************************************

    NAME
        CsrUartDrvLowLevelTransportTxBufLevel

    DESCRIPTION
        Retrieve the number of bytes in the transmit buffer waiting to be
        transmitted.

    PARAMETERS
        handle - handle of UART driver instance.

    RETURNS
        The number of bytes in the transmit buffer waiting to be transmitted.

*****************************************************************************/
CsrUint32 CsrUartDrvLowLevelTransportTxBufLevel(void *handle);


/*****************************************************************************

    NAME
        CsrUartDrvGetBaudrate

    DESCRIPTION
        Retrieve the currently configured baud rate.

    PARAMETERS
        handle - handle of UART driver instance.

    RETURNS
        The currently configured baud rate.

*****************************************************************************/
CsrUint32 CsrUartDrvGetBaudrate(void *handle);


/*****************************************************************************

    NAME
        CsrUartDrvRegister

    DESCRIPTION
        Register a callback function and background interrupt handle to
        provide the UART driver a means for indicating reception of data to
        the upper layer.

    PARAMETERS
        handle - handle of UART driver instance.
        rxDataFn - this function is called when the upper layer calls the
            CsrUartDrvRx function to receive data.
        rxBgintHandle - this background interrupt is triggered when data is
            available in the receive buffer.

*****************************************************************************/
void CsrUartDrvRegister(void *handle, CsrUartDrvDataRx rxDataFn, CsrSchedBgint rxBgintHandle);

#ifdef __cplusplus
}
#endif

#endif
