#ifndef CSR_SERIAL_INIT_H__
#define CSR_SERIAL_INIT_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef CsrUint8 CsrUartDrvParity;
#define CSR_UART_DRV_PARITY_NONE    0
#define CSR_UART_DRV_PARITY_EVEN    2

typedef CsrUint8 CsrUartDrvDataBits;
#define CSR_UART_DRV_DATA_BITS_7    7
#define CSR_UART_DRV_DATA_BITS_8    8

typedef CsrUint8 CsrUartDrvStopBits;
#define CSR_UART_DRV_STOP_BITS_1    0
#define CSR_UART_DRV_STOP_BITS_2    2

void *CsrUartDrvInitialise(const CsrCharString *device,
    CsrUint32 baud, CsrUint32 resetBaud,
    CsrUartDrvDataBits dataBits,
    CsrUartDrvParity parity,
    CsrUartDrvStopBits stopBits,
    CsrBool flowControl,
    const CsrUint8 *token);

void CsrUartDrvDeinitialise(void *handle);

CsrUint32 CsrUartDrvBaudrateActualGet(CsrUint32 wantedBaudrate);

#ifdef __cplusplus
}
#endif

#endif
