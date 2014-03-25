#ifndef LINUX_USB_IOCTL_H__
#define LINUX_USB_IOCTL_H__
/*****************************************************

              (c) CSR plc 2007

              All rights reserved

FILE:         linux_usb_ioctl.h

DESCRIPTION:  IOCTL definitions for the Linux kernel USB
              driver.

REVISION:     $Revision: #6 $

******************************************************/

#ifdef __linux__

#include <linux/ioctl.h>
#include "sched/csr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BT_USB_IOC_MAGIC     'c'
#define BT_USB_IOC_ENTER_DFU			_IO(BT_USB_IOC_MAGIC, 0)
#define BT_USB_IOC_RESET				_IO(BT_USB_IOC_MAGIC, 1)
#ifdef CONFIG_USB_SUSPEND
#	define BT_USB_IOC_ENABLE_SUSPEND	_IO(BT_USB_IOC_MAGIC, 2)
#	define BT_USB_IOC_DISABLE_SUSPEND	_IO(BT_USB_IOC_MAGIC, 3)
#	define BT_USB_IOC_MAX_NO    (4)
#else
#	define BT_USB_IOC_MAX_NO    (2)
#endif

#ifdef __cplusplus
}
#endif

#endif

#endif
