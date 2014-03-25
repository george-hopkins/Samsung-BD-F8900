#ifndef CSR_LINUX_USB_DEV_H__
#define CSR_LINUX_USB_DEV_H__
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

void UsbDev_Rx(CsrUint8 devno);
CsrBool UsbDev_Tx(CsrUint8 devno, CsrUint8 channel, CsrUint8 *data, CsrUint16 size);
CsrBool UsbDev_Reset(CsrUint8 devno);
CsrBool UsbDev_Exists(CsrUint8 devno);
int UsbDev_EnterDFU(CsrUint8 devno);

#endif
