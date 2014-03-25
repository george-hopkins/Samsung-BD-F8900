#ifndef CSR_USB_COM_H__
#define CSR_USB_COM_H__
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

CsrBool CsrUsbDrvStart(CsrUint8 reset);
CsrBool CsrUsbDrvStop(void);
CsrBool CsrUsbDrvTx(CsrUint8 channel, CsrUint8 *data, CsrUint32 size);

#ifdef __cplusplus
}
#endif

#endif
