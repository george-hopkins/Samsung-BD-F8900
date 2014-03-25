/****************************************************************************

               (c) CSR plc 2008

               All rights reserved

FILE:          usb_dfu.c

DESCRIPTION:   Simple DFU interface. This does not contain the actual
               upgrader but merely the means to put a CSR USB device
               into the detach (ie. enter "appDETACH" state)

REVISION:      $Revision: #10 $

****************************************************************************/

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

#include "include/sched/coal_sched.h"
#include "include/sched/pmalloc.h"
#include "include/hci_hc_com.h"
#include "include/linux_usb_com.h"
#include "include/linux_usb_extra.h"

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0) & LINUX_VERSION_CODE < KERNEL_VERSION(2,6,24))
#include <linux/usb_ch9.h>
#endif

/* Globals (device, settings and driver) */
extern csr_dev_t *csr_dev;
extern csr_usb_settings_t csr_setup;
extern struct usb_driver csr_usb_driver;

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
static int dfu_op_detach(void)
{
    int res;
    res = usb_control_msg(csr_dev->dev,
                          usb_sndctrlpipe(csr_dev->dev, 0),
                          DFU_DETACH,
                          USB_TYPE_CLASS|USB_DIR_OUT|USB_RECIP_INTERFACE,
                          csr_dev->dfu_detach_to,
                          csr_dev->dfu_iface,
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
static int dfu_op_clear(void)
{
    int res;
    res = usb_control_msg(csr_dev->dev,
                          usb_sndctrlpipe(csr_dev->dev, 0),
                          DFU_CLR_STATUS,
                          USB_TYPE_CLASS|USB_DIR_OUT|USB_RECIP_INTERFACE,
                          0,
                          csr_dev->dfu_iface,
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
static int dfu_op_status(void)
{
    int res;
    struct dfu_status status;

    /* Set invalid values to be sure */
    memset(&status, 0xCD, sizeof(struct dfu_status));

    res = usb_control_msg(csr_dev->dev,
                          usb_rcvctrlpipe(csr_dev->dev, 0),
                          DFU_GET_STATUS,
                          USB_TYPE_CLASS|USB_DIR_IN|USB_RECIP_INTERFACE,
                          0,
                          csr_dev->dfu_iface,
                          &status,
                          sizeof(struct dfu_status),
                          0);
    if(res == sizeof(struct dfu_status))
    {
        csr_dev->dfu_status    = status.bStatus;
        csr_dev->dfu_state     = status.bState;
        csr_dev->dfu_detach_to = ((status.bwPollTimeout[0]) +
                                  (status.bwPollTimeout[1] << 8) +
                                  (status.bwPollTimeout[2] << 16));

        /* Some devices does not answer with correct timeout, so
         * detect this and set the default */
        if(csr_dev->dfu_detach_to == 0)
        {
            csr_dev->dfu_detach_to = DFU_DETACH_TO;
        }

        res = 0;
    }
    return res;
}

/*************************************************************
 * NAME:
 *      UsbDrv_EnterDFU
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
int UsbDrv_EnterDFU(void)
{
    int res;

    if(!test_bit(DFU_READY, &(csr_dev->endpoint_present)) ||
       !test_bit(DEVICE_CONNECTED, &(csr_dev->endpoint_present)))
    {
        printk(PRNPREFIX "DFU interface not present\n");
        return -ENODEV;
    }

    /* Read status */
    printk(PRNPREFIX "Activating DFU engine at interface %i\n",
           csr_dev->dfu_iface);
    if((res = dfu_op_status()) != 0)
    {
        printk(PRNPREFIX "Could not read DFU engine status, %i\n", res);
        return -EIO;
    }

    /* Wrong state or error, try to clear */
    if((csr_dev->dfu_state != DFU_APP_IDLE) ||
       (csr_dev->dfu_status != 0))
    {
        printk(PRNPREFIX "DFU engine is not in appIDLE - ");
        printk("state: 0x%02x - status: 0x%02x\n",
               csr_dev->dfu_state, csr_dev->dfu_status);
        if(dfu_op_clear() != 0)        {
            printk(PRNPREFIX "Could not clear DFU status\n");
            return -EIO;
        }

        if(dfu_op_status() != 0)
        {
            printk(PRNPREFIX "Could not read DFU engine status\n");
            return -EIO;
        }
        if((csr_dev->dfu_state != DFU_APP_IDLE) ||
           (csr_dev->dfu_status != 0))
        {
            printk(PRNPREFIX "DFU engine is not in appIDLE - ");
            printk("state: 0x%02x - status: 0x%02x\n",
                   csr_dev->dfu_state, csr_dev->dfu_status);
            return -EBUSY;
        }
    }

    /* At this point, we're sure we're in appIDLE, so fire everything */
    if(dfu_op_detach() != 0)
    {
        printk(PRNPREFIX "Error sending DETACH command\n");
        return -EIO;
    }

    /* Make sure we entered detach state */
    if(dfu_op_status() != 0)
    {
        printk(PRNPREFIX "Could not read DFU engine status\n");
        return -EIO;
    }

    /* And print last message */
    if((csr_dev->dfu_state != DFU_APP_DETACH) ||
       (csr_dev->dfu_status != 0))
    {
        printk(PRNPREFIX "DFU engine is not in appDETACH - ");
        printk("state: 0x%02x - status: 0x%02x\n",
               csr_dev->dfu_state, csr_dev->dfu_status);
        return -EBUSY;
    }

    printk(PRNPREFIX "Success: DFU is waiting for USB RESET command\n");
    return 0;
}
