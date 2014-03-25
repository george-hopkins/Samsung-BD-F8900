#ifndef CSR_LINUX_USB_IOCTL_H__
#define CSR_LINUX_USB_IOCTL_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <linux/ioctl.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BT_USB_IOC_MAGIC     'c'
#define BT_USB_IOC_ENTER_DFU _IO(BT_USB_IOC_MAGIC, 0)
#define BT_USB_IOC_RESET     _IO(BT_USB_IOC_MAGIC, 1)
#define BT_USB_IOC_MAX_NO    (2)

#ifdef __cplusplus
}
#endif

#endif
