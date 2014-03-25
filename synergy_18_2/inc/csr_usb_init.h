#ifndef CSR_USB_INIT_H__
#define CSR_USB_INIT_H__
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

void CsrUsbDrvConfigure(const char *device);
#ifdef __cplusplus
}
#endif

#endif /* ifndef CSR_USB_INIT_H */
