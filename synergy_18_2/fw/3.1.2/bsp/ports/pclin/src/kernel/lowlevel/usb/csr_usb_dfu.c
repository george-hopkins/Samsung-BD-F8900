/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <linux/kernel.h>
#include <linux/version.h>
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 33)
#include <linux/autoconf.h>
#else
#include <generated/autoconf.h>
#endif


#include <linux/module.h>
#include <linux/time.h>
#include <linux/spinlock.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/unistd.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/wait.h>
#include <linux/usb.h>
#include <asm/bitops.h>

#include "csr_sched.h"
#include "csr_pmem.h"

#include "lowlevel/csr_linux_usb_com.h"
#include "lowlevel/csr_linux_usb_dev.h"
#include "lowlevel/csr_linux_usb_extra.h"

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 21))
#include <linux/usb/ch9.h>
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
#include <linux/usb_ch9.h>
#endif

/* Globals (device, settings and driver) */
extern csr_dev_t *csr_dev[BT_USB_COUNT];
extern csr_usb_settings_t csr_setup;
extern struct usb_driver csr_usb_driver;
csr_dev_t *devLookup(CsrUint8 devno);

/*************************************************************
 * NAME:
 *      dfu_op_detach
 *
 * DESCRIPTION:
 *      Send the DETACH command
 *
 * RETURNS:
 *      Error code, 0 on success
 *
 *************************************************************/
static int dfu_op_detach(csr_dev_t *dv)
{
    int res;
    res = usb_control_msg(dv->dev,
        usb_sndctrlpipe(dv->dev, 0),
        DFU_DETACH,
        USB_TYPE_CLASS | USB_DIR_OUT | USB_RECIP_INTERFACE,
        dv->dfu_detach_to,
        dv->dfu_iface,
        NULL,
        0,
        0);
    return res;
}

/*************************************************************
 * NAME:
 *      dfu_op_clear
 *
 * DESCRIPTION:
 *      Send the CLEAR STATUS command
 *
 * RETURNS:
 *      Error code, 0 on success
 *
 *************************************************************/
static int dfu_op_clear(csr_dev_t *dv)
{
    int res;
    res = usb_control_msg(dv->dev,
        usb_sndctrlpipe(dv->dev, 0),
        DFU_CLR_STATUS,
        USB_TYPE_CLASS | USB_DIR_OUT | USB_RECIP_INTERFACE,
        0,
        dv->dfu_iface,
        NULL,
        0,
        0);
    return res;
}

/*************************************************************
 * NAME:
 *      dfu_op_status
 *
 * DESCRIPTION:
 *      Send the GET STATUS command
 *
 * RETURNS:
 *      Error code, 0 on success
 *
 *************************************************************/
static int dfu_op_status(csr_dev_t *dv)
{
    int res;
    struct dfu_status status;

    /* Set invalid values to be sure */
    memset(&status, 0xCD, sizeof(struct dfu_status));

    res = usb_control_msg(dv->dev,
        usb_rcvctrlpipe(dv->dev, 0),
        DFU_GET_STATUS,
        USB_TYPE_CLASS | USB_DIR_IN | USB_RECIP_INTERFACE,
        0,
        dv->dfu_iface,
        &status,
        sizeof(struct dfu_status),
        0);
    if (res == sizeof(struct dfu_status))
    {
        dv->dfu_status = status.bStatus;
        dv->dfu_state = status.bState;
        dv->dfu_detach_to = ((status.bwPollTimeout[0]) +
                             (status.bwPollTimeout[1] << 8) +
                             (status.bwPollTimeout[2] << 16));

        /* Some devices does not answer with correct timeout, so
         * detect this and set the default */
        if (dv->dfu_detach_to == 0)
        {
            dv->dfu_detach_to = DFU_DETACH_TO;
        }

        res = 0;
    }
    return res;
}

/*************************************************************
 * NAME:
 *      UsbDev_EnterDFU
 *
 * DESCRIPTION:
 *      Put device safely into "appDETACH" mode. When the
 *      function has returned success, a USB reset will bring
 *      the interface into the actual "DFU runtime" mode.
 *
 * RETURNS:
 *      Error code, 0 on success
 *
 *************************************************************/
int UsbDev_EnterDFU(CsrUint8 devno)
{
    csr_dev_t *dv;
    int res;

    dv = devLookup(devno);
    if (dv == NULL)
    {
        return -ENODEV;
    }

    if (!test_bit(DFU_READY, &(dv->endpoint_present)) ||
        !test_bit(DEVICE_CONNECTED, &(dv->endpoint_present)))
    {
        printk(PRNPREFIX "DFU interface not present\n");
        up(&dv->devlock);
        return -ENODEV;
    }

    /* Read status */
    printk(PRNPREFIX "Activating DFU engine at interface %i\n",
        dv->dfu_iface);
    if ((res = dfu_op_status(dv)) != 0)
    {
        printk(PRNPREFIX "Could not read DFU engine status, %i\n", res);
        up(&dv->devlock);
        return -EIO;
    }

    /* Wrong state or error, try to clear */
    if ((dv->dfu_state != DFU_APP_IDLE) ||
        (dv->dfu_status != 0))
    {
        printk(PRNPREFIX "DFU engine is not in appIDLE - ");
        printk("state: 0x%02x - status: 0x%02x\n",
            dv->dfu_state, dv->dfu_status);
        if (dfu_op_clear(dv) != 0)
        {
            printk(PRNPREFIX "Could not clear DFU status\n");
            up(&dv->devlock);
            return -EIO;
        }

        if (dfu_op_status(dv) != 0)
        {
            printk(PRNPREFIX "Could not read DFU engine status\n");
            up(&dv->devlock);
            return -EIO;
        }
        if ((dv->dfu_state != DFU_APP_IDLE) ||
            (dv->dfu_status != 0))
        {
            printk(PRNPREFIX "DFU engine is not in appIDLE - ");
            printk("state: 0x%02x - status: 0x%02x\n",
                dv->dfu_state, dv->dfu_status);
            up(&dv->devlock);
            return -EBUSY;
        }
    }

    /* At this point, we're sure we're in appIDLE, so fire everything */
    if (dfu_op_detach(dv) != 0)
    {
        printk(PRNPREFIX "Error sending DETACH command\n");
        up(&dv->devlock);
        return -EIO;
    }

    /* Make sure we entered detach state */
    if (dfu_op_status(dv) != 0)
    {
        printk(PRNPREFIX "Could not read DFU engine status\n");
        up(&dv->devlock);
        return -EIO;
    }

    /* And print last message */
    if ((dv->dfu_state != DFU_APP_DETACH) ||
        (dv->dfu_status != 0))
    {
        printk(PRNPREFIX "DFU engine is not in appDETACH - ");
        printk("state: 0x%02x - status: 0x%02x\n",
            dv->dfu_state, dv->dfu_status);
        up(&dv->devlock);
        return -EBUSY;
    }

    printk(PRNPREFIX "Success: DFU is waiting for USB RESET command\n");
    up(&dv->devlock);
    return 0;
}
