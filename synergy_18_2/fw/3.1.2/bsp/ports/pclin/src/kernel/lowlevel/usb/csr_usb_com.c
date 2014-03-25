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
#include <linux/slab.h>
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

#include "csr_tm_bluecore_usb.h"
#include "csr_transport.h"

#include "lowlevel/csr_linux_usb_com.h"
#include "lowlevel/csr_linux_usb_dev.h"
#include "lowlevel/csr_linux_usb_extra.h"

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 21))
#include <linux/usb/ch9.h>
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
#include <linux/usb_ch9.h>
#endif

/* Version Information */
#define DRIVER_VERSION          "0.5"
#define DRIVER_AUTHOR           "CSR plc."
#define DRIVER_NAME             "csr_usb"

/* isoc_alt_setting
 * This setting controls which ISOC (SCO) configuration to use.
 * We assume two streams and let the usr_config decide
 * if we use 8bit or 16bit sound. The setting (and others) can be changed
 * dynamically. The possible values are defined by the HCI specification:
 *   1 : packetsize=8,  1 * 8bit
 *   2 : packetsize=17, 1 * 16bit / 2 * 8bit
 *   3 : packetsize=25, 3 * 8bit
 *   4 : packetsize=33, 2 * 16bit
 *   5 : packetsize=49, 3 * 16bit
 */
#if ((VOICE_SETTING & INPUTSAMPLESIZE_16BIT) == INPUTSAMPLESIZE_16BIT)
#define DEFAULT_ISOC_ALT 4
#else
#define DEFAULT_ISOC_ALT 2
#endif

/* Device instance */
csr_dev_t *csr_dev[BT_USB_COUNT];
struct semaphore csr_dev_sem;

/* Default supported USB devices */
struct usb_device_id csr_device_table[] =
{
    {USB_DEVICE(CSR_VENDOR_ID, CSR_PRODUCT_ID)},
    {}
};

/* Global settings with default */
csr_usb_settings_t csr_setup =
{
    .ext_iface = FALSE,
    .ext_probe = NULL,
    .ext_disconnect = NULL,
    .ext_start = NULL,
    .ext_init = NULL,
    .ext_exit = NULL,
    .max_isoc_in_frames = 3,
    .max_isoc_out_frames = 3,
    .isoc_alt_setting = DEFAULT_ISOC_ALT,
    .driver_name = DRIVER_NAME,
    .usb_devices = NULL,
#ifdef	USB_NO_SCHEDULER
	.running_with_scheduler = FALSE,
#else
    .running_with_scheduler = TRUE,
#endif    
};


/* USB driver table */
struct usb_driver csr_usb_driver =
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 16)
    .owner = THIS_MODULE,
#endif
#endif
    .name = DRIVER_NAME,
    .id_table = csr_device_table,
    .probe = csrUsbProbe,
    .disconnect = csrUsbDisconnect
};

/* Local prototypes */
static CsrInt16 usbRxIntr(csr_dev_t *dv);
static CsrInt16 usbRxBulk(csr_dev_t *dv, CsrUint8 number);
static CsrInt16 usbRxIsoc(csr_dev_t *dv, CsrUint8 number);

/*************************************************************
 * NAME:
 *      csrSlotFromUsb
 *
 * DESCRIPTION:
 *      Find a CSR BT USB device corresponding to a
 *      struct usb_device.  To find an empty slot,
 *      pass a NULL argument.
 *
 *      Grabs the csr_dev_sem lock -- caller MUST release.
 *
 * RETURNS:
 *      Slot number with csr_dev_t struct,
 *      or -1 if not found.
 *
 *************************************************************/

static int csrSlotFromUsb(struct usb_device *uDev)
{
    int slot;

    /* Grab and hold lock. */
    down(&csr_dev_sem);

    for (slot = 0; slot < BT_USB_COUNT; slot++)
    {
        if (uDev != NULL)
        {
            if (csr_dev[slot] && (csr_dev[slot]->dev == uDev))
            {
                return slot;
            }
        }
        else /* Looking for free slot */
        {
            if (csr_dev[slot] == NULL)
            {
                return slot;
            }
        }

        if ((slot == 0) && (csr_setup.running_with_scheduler == 1))
        {
            /* Only a single device when running
             * with the stack in the kernel.
             */
            break;
        }
    }
    return -1;
}

/*************************************************************
 * NAME:
 *      usbCleanUrbs
 *
 * DESCRIPTION:
 *      This function is used to remove any outstanding rx-urbs
 *      which must be done on disconnects.
 *
 * RETURNS:
 *      void
 *
 *************************************************************/
void usbCleanUrbs(csr_dev_t *dv)
{
    int i;

    if (dv->intr_urb != NULL)
    {
        DBG_PRINT("Unlink and free Rx INTR\n");
        URB_UNLINK(dv->intr_urb);
        usb_free_urb(dv->intr_urb);
        dv->intr_urb = NULL;
    }
    for (i = 0; i < MAX_RX_BULK_URBS; i++)
    {
        if (dv->bulk_urb[i] != NULL)
        {
            DBG_PRINT("Unlink and free Rx BULK_%i\n", i);
            URB_UNLINK(dv->bulk_urb[i]);
            usb_free_urb(dv->bulk_urb[i]);
            dv->bulk_urb[i] = NULL;
        }
    }
    for (i = 0; i < MAX_RX_ISOC_URBS; i++)
    {
        if (dv->isoc_urb[i] != NULL)
        {
            DBG_PRINT("Unlink and free Rx ISOC_%i\n", i);
            URB_UNLINK(dv->isoc_urb[i]);
            usb_free_urb(dv->isoc_urb[i]);
            dv->isoc_urb[i] = NULL;
        }
    }
}

/*************************************************************
 * NAME:
 *      csrUsbDisconnect
 *
 * DESCRIPTION:
 *      This function is called by the OS when the USB device is
 *      removed from the system - hence some cleanup must be done.
 *
 * RETURNS:
 *      void
 *
 *************************************************************/
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)
void csrUsbDisconnect(struct usb_interface *intf)
#else
void csrUsbDisconnect(struct usb_device *uDev, void *p)
#endif
{
    int devno;
    csr_dev_t *dv;
    wait_queue_head_t settle_wait;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)
    struct usb_device *uDev = interface_to_usbdev(intf);
#endif

    devno = csrSlotFromUsb(uDev);
    if (devno == -1)
    {
        /*
         * This means detachment for the SCO/USB interfaces.  This
         * means we don't have to do anything.
         */
        up(&csr_dev_sem);
        return;
    }
    else
    {
        /* Grab device and mark slot as CSR_UNUSED. */
        dv = csr_dev[devno];
        csr_dev[devno] = NULL;
        down(&dv->devlock);
    }

    up(&csr_dev_sem);

    if (test_bit(R_THREAD_RUNNING, &(dv->flags)))
    {
        DBG_PRINT("Stopping the reader thread\n");

        clear_bit(R_THREAD_RUNNING, &(dv->flags));
        wake_up_interruptible(&(dv->queue.wait));

        /* Release lock so reader thread can work. */
        up(&dv->devlock);

        wait_for_completion(&dv->reader_thread_exit);
        down(&dv->devlock);

        DBG_PRINT("The reader thread has been stopped\n");
    }
    else
    {
        DBG_PRINT("Reader thread was not running\n");
    }

    /* Set the flags indicating that the module must be stopped */
    clear_bit(DEVICE_CONNECTED, &(dv->flags));
    clear_bit(LISTEN_STARTED, &(dv->flags));
    clear_bit(BULK_IN_READY, &(dv->endpoint_present));
    clear_bit(BULK_OUT_READY, &(dv->endpoint_present));
    clear_bit(INTR_IN_READY, &(dv->endpoint_present));
    clear_bit(ISOC_IN_READY, &(dv->endpoint_present));
    clear_bit(ISOC_OUT_READY, &(dv->endpoint_present));
    clear_bit(EXTRA_INTERFACE_READY, &(dv->endpoint_present));
    clear_bit(DFU_READY, &(dv->endpoint_present));

    init_waitqueue_head(&settle_wait);

    /* Let thing settle, before freeing memory */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)
    set_current_state(TASK_INTERRUPTIBLE);
    schedule_timeout(2 * HZ);
#else
    sleep_on_timeout(&settle_wait, HZ * 2);
#endif

    if (csr_setup.ext_iface == TRUE)
    {
        csr_setup.ext_exit();
    }

    /* Release reassembly buffers */
    if (dv->isoc_reassembly != NULL)
    {
        CsrPmemFree(dv->isoc_reassembly);
        dv->isoc_reassembly = NULL;
    }
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 0)
    if (dv->intr_reassembly != NULL)
    {
        CsrPmemFree(dv->intr_reassembly);
        dv->intr_reassembly = NULL;
    }
#endif

    /* Make sure everything has been freed */
    QueueFree(dv);
    usbCleanUrbs(dv);

    /* Nothing should be using the device at
     * this point, so it's safe to free it.
     */
    CsrPmemFree(dv);

    printk("bt_usb%u: device removed\n", devno);
}

/*************************************************************
 * NAME:
 *      usbTxBulkComplete (internal)
 *
 * DESCRIPTION:
 *      This function is called whenever transmission to the
 *      device has completed. It is a call back function,
 *      hence it is not called from this application. If there
 *      are data on the outgoing queue, they will be sent
 *
 * RETURNS:
 *      void
 *
 *************************************************************/
#if ((LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)) &&   \
    (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 19)))
static void usbTxBulkComplete(struct urb *urb, struct pt_regs *regs)
#else
static void usbTxBulkComplete(struct urb *urb)
#endif
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 4, 20)
    csr_dev_t *dv;
    dv = (csr_dev_t *) urb->context;
#endif

    if ((urb->status != 0) &&
        (urb->status != -ENOENT) &&
        (urb->status != -ECONNRESET) &&
        (urb->status != -ESHUTDOWN))
    {
        printk(PRNPREFIX "Tx BULK completion error, status: %d\n",
            urb->status);
    }
    else
    {
        DBG_VERBOSE("Tx BULK complete\n");
    }

    /* Free the data no matter what */
    kfree(urb->transfer_buffer);

#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 4, 20)
    /* Old kernels can have only one pending URB at the time, so we
     * use a simple tx wait barrier, which must be notified from here */
    up(&(dv->barrier[IDX_BULK]));
#endif
}

/*************************************************************
 * NAME:
 *      usbTxCtrlComplete (internal)
 *
 * DESCRIPTION:
 *      This function is called whenever transmission to the
 *      device has completed. It is a call back function,
 *      hence it is not called from this application. If there
 *      are data on the outgoing queue, they will be sent
 *
 * RETURNS:
 *      void
 *
 *************************************************************/
#if ((LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)) &&   \
    (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 19)))
static void usbTxCtrlComplete(struct urb *urb, struct pt_regs *regs)
#else
static void usbTxCtrlComplete(struct urb *urb)
#endif
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 4, 20)
    csr_dev_t *dv;
    dv = (csr_dev_t *) urb->context;
#endif

    if ((urb->status != 0) &&
        (urb->status != -ENOENT) &&
        (urb->status != -ECONNRESET) &&
        (urb->status != -ESHUTDOWN))
    {
        printk(PRNPREFIX "Tx BULK completion error, status: %d\n",
            urb->status);
    }
    else
    {
        DBG_VERBOSE("Tx BULK complete\n");
    }

    /* Free the data no matter what */
    kfree(urb->transfer_buffer);

#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 4, 20)
    /* Old kernels can have only one pending URB at the time, so we
     * use a simple tx wait barrier, which must be notified from here */
    up(&(dv->barrier[IDX_CONTROL]));
#endif
}

/*************************************************************
 * NAME:
 *      usbTxIsocComplete (internal)
 *
 * DESCRIPTION:
 *      This function is called whenever transmission to the
 *      device has completed. It is a call back function,
 *      hence it is not called from this application. If there
 *      are data on the outgoing queue, they will be sent
 *
 * RETURNS:
 *      void
 *
 *************************************************************/
#if ((LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)) &&   \
    (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 19)))
static void usbTxIsocComplete(struct urb *urb, struct pt_regs *regs)
#else
static void usbTxIsocComplete(struct urb *urb)
#endif
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 4, 20)
    csr_dev_t *dv;
    dv = (csr_dev_t *) urb->context;
#endif

    if ((urb->status != 0) &&
        (urb->status != -ENOENT) &&
        (urb->status != -ECONNRESET) &&
        (urb->status != -ESHUTDOWN))
    {
        printk(PRNPREFIX "Tx ISOC completion error, status: %d\n",
            urb->status);
    }
    else
    {
        DBG_VERBOSE("Tx ISOC complete\n");
    }

    /* Free the data no matter what */
    kfree(urb->transfer_buffer);

#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 4, 20)
    /* Old kernels can have only one pending URB at the time, so we
     * use a simple tx wait barrier, which must be notified from here */
    up(&(dv->barrier[IDX_ISOC]));
#endif
}

/*************************************************************
 * NAME:
 *      fillIsocDesc
 *
 * DESCRIPTION:
 *      This function fills the ISOC description
 *
 * PARAM:
 *      isocUrb: A pointer to the urb that will have its description filled.
 *      size: Total size of transfer_buffer
 *      mtu: The length of the data to be sent (frame size)
 *      CSRMAX: The number of frames
 *
 * RETURNS:
 *      void
 *
 *************************************************************/
static void fillIsocDesc(struct urb *isocUrb,
    int size,
    int mtu,
    int max)
{
    CsrUint16 offset;
    CsrUint16 i;

    for (i = 0, offset = 0;
         (i < max) && (size >= mtu);
         i++, offset += mtu, size -= mtu)
    {
        isocUrb->iso_frame_desc[i].offset = offset;
        isocUrb->iso_frame_desc[i].length = mtu;
    }
    if ((i < max) && size)
    {
        isocUrb->iso_frame_desc[i].offset = offset;
        isocUrb->iso_frame_desc[i].length = size;
        i++;
    }
    isocUrb->number_of_packets = i;
}

/*************************************************************
 * NAME:
 *      usbTxIsoc
 *
 * DESCRIPTION:
 *      This function is called whenever a ISOC message (SCO) is
 *      to be sent to the usb device.
 *
 * RETURNS:
 *      0 if successful, otherwise an error message
 *
 *************************************************************/
CsrInt16 usbTxIsoc(csr_dev_t *dv, void *data, CsrUint16 length)
{
    struct urb *txisoc;
    CsrUint8 err;

    err = -ENOMEM;
    txisoc = URB_ALLOC(csr_setup.max_isoc_out_frames, GFP_KERNEL);
    if (txisoc != NULL)
    {
        /* ISOC URBs must be filled manually */
        txisoc->dev = dv->dev;
        txisoc->pipe = usb_sndisocpipe(dv->dev,
            dv->isoc_out_ep);
        txisoc->transfer_buffer = data;
        txisoc->complete = usbTxIsocComplete;
        txisoc->transfer_buffer_length = length;
        txisoc->interval = dv->isoc_out_interval;
        txisoc->context = dv;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)
        txisoc->transfer_flags = URB_ISO_ASAP;
#else
        txisoc->transfer_flags = USB_ISO_ASAP;
#endif

        /* Setup frame size and offset */
        fillIsocDesc(txisoc,
            length,
            length / csr_setup.max_isoc_out_frames,
            csr_setup.max_isoc_out_frames);

        err = URB_SUBMIT(txisoc, GFP_KERNEL);

#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 4, 20)
        down_interruptible(&(dv->barrier[IDX_ISOC]));
#endif

        if (err != 0)
        {
            printk(PRNPREFIX "Tx ISOC submit error, status: %d\n",
                err);

            /* Attempt to resubmit */
            err = URB_SUBMIT(txisoc, GFP_KERNEL);
            if (err != 0)
            {
                /* This failed too, so unlink in order to free and up(sem) */
                printk(PRNPREFIX "Tx ISOC re-submit error, status: %d\n",
                    err);
                URB_UNLINK(txisoc);
            }
        }
        else
        {
            DBG_VERBOSE("Transmitted %i ISOC bytes\n", length);
        }

        /* Free the URB */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)
        usb_free_urb(txisoc);
#endif
    }
    else
    {
        printk(PRNPREFIX "Tx ISOC alloc error\n");
    }
    return err;
}

/*************************************************************
 * NAME:
 *      usbTxCtrl (internal)
 *
 * DESCRIPTION:
 *      This function is called whenever a control message
 *      (HCI-command) is to be sent to the usb device.
 *
 * RETURNS:
 *      0 if successful, otherwise an error message
 *
 *************************************************************/
CsrInt16 usbTxCtrl(csr_dev_t *dv, void *data, CsrUint16 length)
{
    CsrInt16 err;
    struct urb *txctrl;

    err = -ENOMEM;
    txctrl = URB_ALLOC(0, GFP_KERNEL);
    if (txctrl != NULL)
    {
        /* Setup packet */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 4, 20)
        dv->ctrl_setup.wLength = cpu_to_le16(length);
#else
        dv->ctrl_setup.length = cpu_to_le16(length);
#endif

        usb_fill_control_urb(txctrl,
            dv->dev,
            usb_sndctrlpipe(dv->dev,
                dv->ctrl_ep),
            (CsrUint8 *) &dv->ctrl_setup,
            data,
            length,
            usbTxCtrlComplete,
            dv);

        err = URB_SUBMIT(txctrl, GFP_KERNEL);

#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 4, 20)
        down_interruptible(&(dv->barrier[IDX_CONTROL]));
#endif

        if (err != 0)
        {
            printk(PRNPREFIX "Tx CTRL submit error, code %d\n", err);

            /* Attempt to resubmit */
            err = URB_SUBMIT(txctrl, GFP_KERNEL);
            if (err != 0)
            {
                /* This failed too, so unlink in order to free and up(sem) */
                printk(PRNPREFIX "Tx CTRL re-submit error, status: %d\n",
                    err);
                URB_UNLINK(txctrl);
            }
        }
        else
        {
            DBG_VERBOSE("Transmitted %i CTRL bytes\n", length);
        }

        /* Free the URB */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)
        usb_free_urb(txctrl);
#endif
    }
    else
    {
        printk(PRNPREFIX "Tx CTRL alloc error, code %d\n", err);
    }

    return err;
}

/*************************************************************
 * NAME:
 *      usbTxBulk
 *
 * DESCRIPTION:
 *      This function is called whenever a bulk message (ACL) is
 *      to be sent to the usb device.
 *
 * RETURNS:
 *      0 if successful, otherwise an error message
 *
 *************************************************************/
CsrInt16 usbTxBulk(csr_dev_t *dv, void *data, CsrUint16 length)
{
    CsrInt16 err;
    struct urb *txbulk;

    err = -ENOMEM;
    txbulk = URB_ALLOC(0, GFP_KERNEL);
    if (txbulk != NULL)
    {
        usb_fill_bulk_urb(txbulk,
            dv->dev,
            usb_sndbulkpipe(dv->dev,
                dv->bulk_out_ep),
            data,
            length,
            usbTxBulkComplete,
            dv);
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 0)
        txbulk->transfer_flags |= USB_QUEUE_BULK;
#endif
        err = URB_SUBMIT(txbulk, GFP_KERNEL);

#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 4, 20)
        down_interruptible(&(dv->barrier[IDX_BULK]));
#endif

        if (err != 0)
        {
            printk(PRNPREFIX "Tx BULK submit error, code %d\n", err);

            /* Attempt to resubmit */
            err = URB_SUBMIT(txbulk, GFP_KERNEL);
            if (err != 0)
            {
                /* This failed too, so unlink in order to free and up(sem) */
                printk(PRNPREFIX "Tx BULK re-submit error, status: %d\n",
                    err);
                URB_UNLINK(txbulk);
            }
        }
        else
        {
            DBG_VERBOSE("Transmitted %i BULK bytes\n", length);
        }

        /* Free the URB */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)
        usb_free_urb(txbulk);
#endif
    }
    else
    {
        printk(PRNPREFIX "Tx BULK alloc error, code %d\n", err);
    }

    return err;
}

/*************************************************************
 * NAME:
 *      reassembleIsoc
 *
 * DESCRIPTION:
 *      This function reassembles ISOC frames to SCO packets.
 *
 * PARAMETERS:
 *      data: A pointer to the data to be reassembled
 *      length: The length of the data block
 *
 * RETURNS:
 *      Error code, zero is success
 *
 *************************************************************/
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 4, 20)
static CsrInt32 reassembleIsoc(csr_dev_t *dv, CsrUint8 *dat,
    struct usb_iso_packet_descriptor *frameDesc)
#else
static CsrInt32 reassembleIsoc(csr_dev_t *dv, CsrUint8 *dat,
    iso_packet_descriptor_t *frameDesc)
#endif
{
    CsrUint8 len;
    CsrUint32 length;
    CsrUint8 *data;

    data = dat + frameDesc->offset;
    length = frameDesc->actual_length;

    /* The frame was not ok, remove possible previously received data to
     * be reassembled with this part */
    if (frameDesc->status != 0)
    {
        if (dv->isoc_reassembly != NULL)
        {
            CsrPmemFree(dv->isoc_reassembly);
            dv->isoc_reassembly = NULL;
            dv->isoc_total = 0;
            if (length <= dv->isoc_remain)
            {
                dv->isoc_remain -= length;
                return frameDesc->status;
            }
            else
            {
                length -= dv->isoc_remain;
                data += length;
            }
        }
    }

    /* Status of frame is OK, but the previous received frame to be
     * reassembled with this one was not ok, hence discard this frame
     * as well */
    if ((dv->isoc_reassembly == NULL) &&
        (dv->isoc_remain != 0))
    {
        if (length <= dv->isoc_remain)
        {
            dv->isoc_remain -= length;
            return -1;
        }
        else
        {
            length -= dv->isoc_remain;
            data += length;
        }
    }

    /* Everything is fine - start normal reassembly procedure */
    while (length)
    {
        len = 0;
        /* Beginning of frame */
        if (dv->isoc_reassembly == NULL)
        {
            if (length >= HCI_SCO_HDR_SIZE)
            {
                /* The size is specified in byte 3, therefore "+2" */
                dv->isoc_total = *(CsrUint8 *) (data + 2) + HCI_SCO_HDR_SIZE;
                dv->isoc_remain = dv->isoc_total;
            }
            else
            {
                return -EILSEQ;
            }

            dv->isoc_reassembly = (CsrUint8 *) CsrPmemAlloc(dv->isoc_total);
        }
        /* Continuation of packet */
        else
        {
        }

        len = CSRMIN(dv->isoc_remain, length);
        memcpy(&dv->isoc_reassembly[dv->isoc_total - dv->isoc_remain],
            data,
            len);
        dv->isoc_remain -= len;

        /* The frame is complete */
        if (!dv->isoc_remain)
        {
            /* Let the queue consume the data (don't free it!) */
            QueueAppend(dv, TRANSPORT_CHANNEL_SCO,
                dv->isoc_reassembly,
                dv->isoc_total,
                FALSE);
            dv->isoc_reassembly = NULL;
            atomic_inc(&(dv->await_bg_int));
            wake_up_interruptible(&(dv->queue.wait));
        }
        length -= len;
        data += len;
    }

    return 0;
}

/*************************************************************
 * NAME:
 *      reassembleIntr
 *
 * DESCRIPTION:
 *      Reassmeble fragmented INTR packets. This functionality
 *      is needed for 2.4.x kernels that does not handle this
 *      automatically.
 *
 * RETURNS:
 *
 *************************************************************/
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 0)
static CsrInt32 reassembleIntr(struct urb *urb)
{
    csr_dev_t *dv;
    CsrUint32 length;
    CsrUint32 len;
    CsrUint8 *data;

    dv = (csr_dev_t *) urb->context;

    data = urb->transfer_buffer;
    length = urb->actual_length;

    while (length)
    {
        len = 0;

        /* Beginning of frame */
        if (dv->intr_reassembly == NULL)
        {
            if (length >= HCI_EVENT_HDR_SIZE)
            {
                /* The size is specified in byte 2, therefore "+1" */
                dv->intr_total = *(CsrUint8 *) (data + 1) +
                                 HCI_EVENT_HDR_SIZE;
                dv->intr_remain = dv->intr_total;
            }
            else
            {
                return -EILSEQ;
            }

            dv->intr_reassembly = (CsrUint8 *) CsrPmemAlloc(dv->intr_total);
        }
        /* Continuation of packet */
        else
        {
        }

        len = CSRMIN(dv->intr_remain, length);

        memcpy(&dv->intr_reassembly[dv->intr_total - dv->intr_remain],
            data,
            len);

        dv->intr_remain -= len;

        /* The frame is complete */
        if (dv->intr_remain == 0)
        {
            /* Let the queue consume the data (don't free it!) */
            QueueAppend(dv, TRANSPORT_CHANNEL_HCI,
                dv->intr_reassembly,
                dv->intr_total,
                FALSE);
            dv->intr_reassembly = NULL;
            atomic_inc(&(dv->await_bg_int));
            wake_up_interruptible(&(dv->queue.wait));
        }
        length -= len;
        data += len;
    }
    return 0;
}

#endif

/*************************************************************
 * NAME:
 *      usbRxIntrComplete
 *
 * DESCRIPTION:
 *     This function is called asynchronous whenever hci
 *     events are present on the USB device. The received data is copied
 *     to a buffer structure and the scheduler is informed by issuing a
 *     bg_interrupt.  Finally, the function makes a new data read request
 *     to the device.
 *
 * RETURNS:
 *      void
 *
 *************************************************************/
#if ((LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)) &&   \
    (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 19)))
static void usbRxIntrComplete(struct urb *urb, struct pt_regs *regs)
#else
static void usbRxIntrComplete(struct urb *urb)
#endif
{
    csr_dev_t *dv;

    dv = (csr_dev_t *) urb->context;

    /* Data is available */
    if ((urb->status == 0) && (urb->actual_length > 0))
    {
        DBG_PRINT("Rx INTR complete, %d bytes received\n", urb->actual_length);
#if 0
        DBG_VERBOSE("Rx INTR complete, %d bytes received\n",
            urb->actual_length);
#endif

        if (test_bit(DEVICE_CONNECTED, &(dv->flags)))
        {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)
            /* Copy data to queue so URB can be resubmitted */
            QueueAppend(dv, TRANSPORT_CHANNEL_HCI,
                urb->transfer_buffer,
                urb->actual_length,
                TRUE);

            atomic_inc(&(dv->await_bg_int));
            wake_up_interruptible(&(dv->queue.wait));
#else
reassembleIntr(urb);
#endif
        }
        else
        {
            DBG_PRINT("Received INTR data - not sending up!\n");
        }
    }
    if ((urb->status != 0) &&
        (urb->status != -ENOENT) &&
        (urb->status != -ECONNRESET) &&
        (urb->status != -ESHUTDOWN))
    {
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 0)
        URB_UNLINK(urb);
#endif
        DBG_VERBOSE("Rx INTR complete error, code %d\n", urb->status);
    }

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)
    if (test_bit(DEVICE_CONNECTED, &(dv->flags)))
    {
        CsrInt16 err;

        /* Resubmit the urb */
        urb->dev = dv->dev;
        urb->actual_length = 0;
        err = URB_SUBMIT(urb, GFP_ATOMIC);

        /* Success or disconnect not errors */
        if ((err != 0) && (err != -ENODEV))
        {
            printk("bt_usb%u: Rx INTR resubmit error, status %d\n",
                dv->minor, err);
        }
    }
#endif
}

/*************************************************************
 * NAME:
 *      usbRxBulkComplete
 *
 * DESCRIPTION:
 *      This function is called asynchronous whenever ACL data is
 *      present on the USB device. The received data is copied to a
 *      buffer structure and the scheduler is informed by issuing a
 *      bg_interrupt.  Finally, the function makes a new data read
 *      request to the device.
 *
 * RETURNS:
 *      void
 *
 *************************************************************/
#if ((LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)) &&   \
    (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 19)))
static void usbRxBulkComplete(struct urb *urb, struct pt_regs *regs)
#else
static void usbRxBulkComplete(struct urb *urb)
#endif
{
    csr_dev_t *dv;
    CsrInt16 err;

    dv = (csr_dev_t *) urb->context;

    /* Data is available */
    if ((urb->status == 0) && (urb->actual_length > 0))
    {
        DBG_VERBOSE("Rx BULK complete, %d bytes received\n",
            urb->actual_length);

        if (test_bit(DEVICE_CONNECTED, &(dv->flags)))
        {
            /* Copy data to queue so URB can be resubmitted */
            QueueAppend(dv, TRANSPORT_CHANNEL_ACL,
                urb->transfer_buffer,
                urb->actual_length,
                TRUE);

            atomic_inc(&(dv->await_bg_int));
            wake_up_interruptible(&(dv->queue.wait));
        }
        else
        {
            DBG_PRINT("BULK data received - not sending up!\n");
        }
    }
    if ((urb->status != 0) &&
        (urb->status != -ENOENT) &&
        (urb->status != -ECONNRESET) &&
        (urb->status != -ESHUTDOWN))
    {
        DBG_VERBOSE("Rx BULK complete error, code %d\n", urb->status);
    }

    if (test_bit(DEVICE_CONNECTED, &(dv->flags)))
    {
        /* Resubmit the urb */
        urb->dev = dv->dev;
        err = URB_SUBMIT(urb, GFP_ATOMIC);

        /* Success or disconnect not errors */
        if ((err != 0) && (err != -ENODEV))
        {
            printk("bt_usb%u: Rx BULK resubmit error, code %d\n",
                dv->minor, err);
        }
    }
}

/*************************************************************
 * NAME:
 *      usbRxIsocComplete
 *
 * DESCRIPTION:
 *      This function is called asynchronous whenever sco data
 *      are present on the USB device. The received data is copied to a
 *      buffer structure and the scheduler is informed by issuing a
 *      bg_interrupt.  Finally, the function makes a new data read request
 *      to the device.
 *
 * RETURNS:
 *      void
 *
 *************************************************************/
#if ((LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)) &&   \
    (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 19)))
static void usbRxIsocComplete(struct urb *urb, struct pt_regs *regs)
#else
static void usbRxIsocComplete(struct urb *urb)
#endif
{
    csr_dev_t *dv;
    CsrInt16 err, i;

    dv = (csr_dev_t *) urb->context;

    /* Data is available */
    err = 0;
    if ((urb->status == 0) && (urb->actual_length > 0))
    {
#if 0
        DBG_VERBOSE("Rx ISOC complete, %d bytes received\n",
            urb->actual_length);
#endif

if (test_bit(DEVICE_CONNECTED, &(dv->flags)))
{
    for (i = 0; i < urb->number_of_packets; i++)
    {
        reassembleIsoc(dv, (CsrUint8 *) urb->transfer_buffer,
            &urb->iso_frame_desc[i]);
    }
}
else
{
    DBG_PRINT("ISOC data received - not sending up!\n");
}
}
if ((urb->status != 0) &&
    (urb->status != -ENOENT) &&
    (urb->status != -ECONNRESET) &&
    (urb->status != -ESHUTDOWN))
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 0)
    URB_UNLINK(urb);
#endif
    DBG_VERBOSE("Rx ISOC complete error, code %d\n", urb->status);
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)
if (test_bit(DEVICE_CONNECTED, &(dv->flags)))
{
    /* Resubmit the URB */
    urb->dev = dv->dev;
    err = URB_SUBMIT(urb, GFP_ATOMIC);

    /* Success or disconnect not errors */
    if ((err != 0) && (err != -ENODEV))
    {
        printk("bt_usb%u: Rx ISOC resubmit error, code %d\n",
            dv->minor, err);
    }
}
#endif
}

/*************************************************************
 * NAME:
 *      usbRxIntr (internal)
 *
 * DESCRIPTION:
 *      This function is called whenever a read for HCI events
 *      is wanted. The request will result in a asynchronous
 *      call of the usbRxComplete function, when data has been
 *      read from the device.
 *
 * RETURNS:
 *      0 on success, otherwise a number indicating the error
 *
 *************************************************************/
static CsrInt16 usbRxIntr(csr_dev_t *dv)
{
    CsrInt16 err;
    void *buf;
    struct urb *rxintr;

    err = -ENOMEM;
    rxintr = URB_ALLOC(0, GFP_ATOMIC);
    buf = CsrPmemAlloc(MAX_HCI_EVENT_SIZE);

    if ((rxintr != NULL) && (buf != NULL))
    {
        usb_fill_int_urb(rxintr,
            dv->dev,
            usb_rcvintpipe(dv->dev,
                dv->intr_ep),
            buf,
            dv->intr_size,
            usbRxIntrComplete,
            dv,
            dv->intr_interval);


        err = URB_SUBMIT(rxintr, GFP_ATOMIC);
        if (err != 0)
        {
            URB_UNLINK(rxintr);
            usb_free_urb(rxintr);
            dv->intr_urb = NULL;
            printk(PRNPREFIX "Rx INTR submit error, code %d\n", err);
        }
        else
        {
            dv->intr_urb = rxintr;
        }
    }
    else
    {
        printk(PRNPREFIX "Rx INTR alloc error, code %d\n", err);
        if (rxintr)
        {
            CsrPmemFree(rxintr);
        }
        if (buf)
        {
            CsrPmemFree(buf);
        }
    }

    return err;
}

/*************************************************************
 * NAME:
 *      usbRxBulk (internal)
 *
 * DESCRIPTION:
 *      This function is called whenever a read for BULK data
 *      is wanted. The request will result in a asynchronous
 *      call of the usbRxComplete function, when data has been
 *      read from the device.
 *
 * PARAMS:
 *      number: is the urb number in the bulk urb array
 *
 * RETURNS:
 *      0 on success, otherwise a number indicating the error
 *
 *************************************************************/
static CsrInt16 usbRxBulk(csr_dev_t *dv, CsrUint8 number)
{
    CsrInt16 err;
    void *buf;
    struct urb *rxbulk;

    err = -ENOMEM;
    rxbulk = URB_ALLOC(0, GFP_ATOMIC);
    buf = CsrPmemAlloc(MAX_HCI_ACL_SIZE);
    if ((rxbulk != NULL) && (buf != NULL))
    {
        usb_fill_bulk_urb(rxbulk,
            dv->dev,
            usb_rcvbulkpipe(dv->dev,
                dv->bulk_in_ep),
            buf,
            MAX_HCI_ACL_SIZE,
            usbRxBulkComplete,
            dv);

        err = URB_SUBMIT(rxbulk, GFP_ATOMIC);
        if (err != 0)
        {
            URB_UNLINK(rxbulk);
            usb_free_urb(rxbulk);
            dv->bulk_urb[number] = NULL;
            printk(PRNPREFIX "Rx BULK submit error, code %d\n",
                err);
        }
        else
        {
            dv->bulk_urb[number] = rxbulk;
        }
    }
    else
    {
        printk(PRNPREFIX "Rx BULK alloc error, code %d\n", err);
        if (rxbulk)
        {
            CsrPmemFree(rxbulk);
        }
        if (buf)
        {
            CsrPmemFree(buf);
        }
    }
    return err;
}

/*************************************************************
 * NAME:
 *      usbRxIsoc
 *
 * DESCRIPTION:
 *      This function is called whenever a read for ISOC data
 *      is wanted. The request will result in an asynchronous
 *      call of the usbRxComplete function, when data has been
 *      read from the device.
 *
 * RETURNS:
 *      0 on success, otherwise a number indicating the error
 *
 *************************************************************/
static CsrInt16 usbRxIsoc(csr_dev_t *dv, CsrUint8 number)
{
    CsrInt16 err;
    CsrUint32 size;
    void *buf;
    struct urb *rxisoc;

    err = -ENOMEM;
    size = dv->isoc_in_size * csr_setup.max_isoc_in_frames;
    rxisoc = URB_ALLOC(csr_setup.max_isoc_in_frames, GFP_ATOMIC);
    buf = CsrPmemAlloc(size);
    if ((rxisoc != NULL) && (buf != NULL))
    {
        rxisoc->dev = dv->dev;
        rxisoc->context = dv;
        rxisoc->pipe = usb_rcvisocpipe(dv->dev,
            dv->isoc_in_ep);
        rxisoc->complete = usbRxIsocComplete;
        rxisoc->interval = dv->isoc_in_interval;
        rxisoc->transfer_buffer = buf;
        rxisoc->transfer_buffer_length = size;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)
        rxisoc->transfer_flags = URB_ISO_ASAP;
#else
rxisoc->transfer_flags = USB_ISO_ASAP;
#endif

        fillIsocDesc(rxisoc,
            size,
            size / csr_setup.max_isoc_in_frames,
            csr_setup.max_isoc_in_frames);

        err = URB_SUBMIT(rxisoc, GFP_ATOMIC);
        if (err != 0)
        {
            URB_UNLINK(rxisoc);
            usb_free_urb(rxisoc);
            dv->isoc_urb[number] = NULL;

            printk(PRNPREFIX "Rx ISOC submit error, code %d\n",
                err);
        }
        else
        {
            dv->isoc_urb[number] = rxisoc;
        }
    }
    else
    {
        printk(PRNPREFIX "Rx ISOC alloc error, code %d\n",
            err);
        if (rxisoc)
        {
            CsrPmemFree(rxisoc);
        }
        if (buf)
        {
            CsrPmemFree(buf);
        }
    }
    return err;
}

/*************************************************************
 * NAME:
 *      startListen
 *
 * DESCRIPTION:
 *      This functions transmits the first receive URBs.
 *      It is called when the probe function has found all interfaces.
 *
 * RETURNS:
 *      void
 *
 *************************************************************/
static void startListen(csr_dev_t *dv)
{
    int res;
    int i;

    set_bit(LISTEN_STARTED, &(dv->flags));

    /* Start listening for ACL and HCI events */
    res = usbRxIntr(dv);
    if (res != 0)
    {
        printk(PRNPREFIX "Error starting Rx INTR, code %d\n", res);
    }
    else
    {
        for (i = 0; (i < MAX_RX_BULK_URBS) && (res == 0); i++)
        {
            res = usbRxBulk(dv, i);
            if (res != 0)
            {
                printk(PRNPREFIX "Error starting Rx BULK, code %d\n", res);
            }
        }

        for (i = 0; (i < MAX_RX_ISOC_URBS) && (res == 0); i++)
        {
            res = usbRxIsoc(dv, i);
            if (res != 0)
            {
                printk(PRNPREFIX "Error starting Rx ISOC, code %d\n", res);
            }
        }
    }

    DBG_PRINT("Listen loop started, code %i\n", res);
}

/*************************************************************
 * NAME:
 *      handleIsocInEndpoint
 *
 * DESCRIPTION:
 *      Probe helper for ISOC-IN endpoints
 *
 * RETURNS:
 *      void
 *
 *************************************************************/
static void handleIsocInEndpoint(csr_dev_t *dv, int alt,
    struct usb_endpoint_descriptor *endpoint,
    int ifnum)
{
    if (alt == csr_setup.isoc_alt_setting)
    {
        dv->isoc_reassembly = NULL;
        if (usb_set_interface(dv->dev, ifnum,
                csr_setup.isoc_alt_setting))
        {
            printk("bt_usb%u: Cannot set ISOC interface alternate settings\n",
                dv->minor);
        }
        else
        {
            /*
             * The interval of the USB descriptor of the isochronous
             * interface is expressed as 2**(N-1), where N is the value to
             * read from the descriptor.
             * When setting up the isochronous interface we must specify the
             * interval as number of (micro)frames, hence we must convert
             * N to nr of frames.
             */
            dv->isoc_in_interval = 1 << (endpoint->bInterval - 1);
            dv->isoc_in_size = endpoint->wMaxPacketSize;
            dv->isoc_in_ep = endpoint->bEndpointAddress;

            set_bit(ISOC_IN_READY, &dv->endpoint_present);

            printk("bt_usb%u: ISOC in int %u mps %u\n",
                dv->minor, dv->isoc_in_interval, dv->isoc_in_size);
        }
    }
}

/*************************************************************
 * NAME:
 *      handleIsocOutEndpoint
 *
 * DESCRIPTION:
 *      Probe helper for ISOC-OUT endpoints
 *
 * RETURNS:
 *      void
 *
 *************************************************************/
static void handleIsocOutEndpoint(csr_dev_t *dv, int alt,
    struct usb_endpoint_descriptor *endpoint)
{
    if (alt == csr_setup.isoc_alt_setting)
    {
        dv->isoc_out_size = endpoint->wMaxPacketSize;
        dv->isoc_out_interval = 1 << (endpoint->bInterval - 1);
        dv->isoc_out_ep = endpoint->bEndpointAddress;

        set_bit(ISOC_OUT_READY, &dv->endpoint_present);

        printk("bt_usb%u: ISOC out int %u mps %u\n",
            dv->minor, dv->isoc_out_interval, dv->isoc_out_size);
    }
}

/*************************************************************
 * NAME:
 *      handleBulkInEndpoint
 *
 * DESCRIPTION:
 *      Probe helper for BULK-IN endpoints
 *
 * RETURNS:
 *      void
 *
 *************************************************************/
static void handleBulkInEndpoint(csr_dev_t *dv, int alt,
    struct usb_endpoint_descriptor *endpoint)
{
    dv->bulk_in_ep = endpoint->bEndpointAddress;

    set_bit(BULK_IN_READY, &dv->endpoint_present);

    printk("bt_usb%u: BULK in int %u mps %u\n",
        dv->minor, endpoint->bInterval, endpoint->wMaxPacketSize);
}

/*************************************************************
 * NAME:
 *      handleBulkOutEndpoint
 *
 * DESCRIPTION:
 *      Probe helper for BULK-OUT endpoints
 *
 * RETURNS:
 *      void
 *
 *************************************************************/
static void handleBulkOutEndpoint(csr_dev_t *dv, int alt,
    struct usb_endpoint_descriptor *endpoint)
{
    dv->bulk_out_ep = endpoint->bEndpointAddress;

    set_bit(BULK_OUT_READY, &dv->endpoint_present);
    printk("bt_usb%u: BULK out int %u mps %u\n",
        dv->minor, endpoint->bInterval, endpoint->wMaxPacketSize);
}

/*************************************************************
 * NAME:
 *      handleIntrInEndpoint
 *
 * DESCRIPTION:
 *      Probe helper for INTR-IN endpoints
 *
 * RETURNS:
 *      void
 *
 *************************************************************/
static void handleIntrInEndpoint(csr_dev_t *dv, int alt,
    struct usb_endpoint_descriptor *endpoint)
{
    dv->intr_ep = endpoint->bEndpointAddress;
    dv->intr_interval = endpoint->bInterval;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)
    dv->intr_size = MAX_HCI_EVENT_SIZE;
#else
    dv->intr_size = endpoint->wMaxPacketSize;
#endif

    set_bit(INTR_IN_READY, &dv->endpoint_present);

    printk("bt_usb%u: INTR in int %u mps %u\n",
        dv->minor, endpoint->bInterval, dv->intr_size);
}

/*************************************************************
 * NAME:
 *      handleDfuInterface
 *
 * DESCRIPTION:
 *      Probe helper for DFU interfaces
 *
 * RETURNS:
 *      void
 *
 *************************************************************/
static void handleDfuInterface(csr_dev_t *dv, int ifnum)
{
    dv->dfu_iface = ifnum;
    set_bit(DFU_READY, &dv->endpoint_present);
    printk("bt_usb%u: DFU interface %u\n", dv->minor, ifnum);
}

/*************************************************************
 * NAME:
 *      csrUsbProbe
 *
 * DESCRIPTION:
 *      Is called whenever a USB device is inserted, which
 *      is believed to be of interest for this driver.
 *
 * RETURNS:
 *
 *
 *************************************************************/
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)
int csrUsbProbe(struct usb_interface *intf,
    const struct usb_device_id *id)
#else
void *csrUsbProbe(struct usb_device *uDev,
    CsrUint32 ifnum,
    const struct usb_device_id *id)
#endif
{
    char buf[16];
    struct usb_endpoint_descriptor *endpoint;
    struct usb_interface *sco_intf;
    struct usb_interface *dfu_intf;
    csr_dev_t *dv;
    int alt;
    int devno;
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 4, 20)
    int i;
#endif
    CsrBool res;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)
    struct usb_device *uDev = interface_to_usbdev(intf);
    CsrUint32 ifnum;
#else
struct usb_interface *intf = usb_ifnum_to_if(uDev, ifnum);
#endif

    /* Look up a free slot. */
    devno = csrSlotFromUsb(NULL);
    if (devno == -1)
    {
        up(&csr_dev_sem);
        printk(PRNPREFIX "no more free slots, exiting.\n");
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)
        return 0;
#else
        return NULL;
#endif
    }

    dv = csr_dev[devno] = CsrPmemZalloc(sizeof(*dv));

    /* Lock device and release device list lock. */
#ifndef init_MUTEX_LOCKED
	sema_init(&dv->devlock,0);
#else
	init_MUTEX_LOCKED(&dv->devlock);
#endif
    dv->dev = uDev;
    up(&csr_dev_sem);

    dv->minor = devno;
    init_completion(&dv->reader_thread_exit);
    atomic_set(&dv->await_bg_int, 0);

    if (uDev->descriptor.iSerialNumber)
    {
        if (usb_string(uDev, uDev->descriptor.iSerialNumber,
                buf, sizeof(buf)) > 0)
        {
            /* The format is ``lap-uap-nap''.  Substitute
             * '\0' for '-' in the string for easy printing.
             */
            buf[6] = 0;
            buf[9] = 0;
            printk("bt_usb%u: %s:%s:%s\n", devno,
                &buf[10], &buf[7], &buf[0]);
        }
    }
    printk("bt_usb%u: probing interfaces\n", devno);

    /* SCO interface is mandatory */
    sco_intf = usb_ifnum_to_if(uDev, 1);
    if (sco_intf == NULL)
    {
        printk("bt_usb%u: SCO interface missing, exiting.\n", devno);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)
        return 0;
#else
        return NULL;
#endif
    }
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)
    else if (usb_driver_claim_interface(&csr_usb_driver, sco_intf,
                 NULL) != 0)
#else
    /* The API on 2.4 is horrible, so do it like this:
     * Check if the interface is unclaimed, try to claim
     * it, and check if it is still unclaimed.
     */
    else if (usb_interface_claimed(sco_intf) == 0)
    {
        usb_driver_claim_interface(&csr_usb_driver, sco_intf, NULL);
        if (usb_interface_claimed(sco_intf) == 0)
        {
            printk("bt_usb%u: couldn't claim SCO interface, exiting.\n", devno);
            return NULL;
        }
    }
#endif
    {
        printk("bt_usb%u: couldn't claim SCO interface, exiting.\n", devno);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)
        return 0;
#else
        return NULL;
#endif
    }

    dfu_intf = usb_ifnum_to_if(uDev, 2);
    if (dfu_intf == NULL)
    {
        printk("bt_usb%u: warning: DFU interface missing.\n", devno);
    }
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)
    else if (usb_driver_claim_interface(&csr_usb_driver, dfu_intf,
                 NULL) != 0)
#else
    /* The API on 2.4 is horrible, so do it like this:
     * Check if the interface is unclaimed, try to claim
     * it, and check if it is still unclaimed.
     */
    else if (usb_interface_claimed(dfu_intf) == 0)
    {
        usb_driver_claim_interface(&csr_usb_driver, dfu_intf, NULL);
        if (usb_interface_claimed(dfu_intf) == 0)
        {
            printk("bt_usb%u: couldn't claim DFU interface, exiting.\n", devno);
            return NULL;
        }
    }
#endif
    {
        printk("bt_usb%u: couldn't claim DFU interface, exiting.\n", devno);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)
        return 0;
#else
        return NULL;
#endif
    }

    res = FALSE;

    /* Initialize pending queue */
    QueuePrepare(dv);

    dv->isoc_reassembly = NULL;
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 0)
    dv->intr_reassembly = NULL;
#endif

    /* Set values for the control request header stuff */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 4, 20)
    dv->ctrl_setup.bRequestType = BT_CTRL_REQUEST;
    dv->ctrl_setup.bRequest = 0;
    dv->ctrl_setup.wValue = 0;
    dv->ctrl_setup.wIndex = 0;
#else
    dv->ctrl_setup.requesttype = BT_CTRL_REQUEST;
    dv->ctrl_setup.request = 0;
    dv->ctrl_setup.value = 0;
    dv->ctrl_setup.index = 0;
#endif

    /* The control endpoint is mandatory for all usb devices,
       and can therefore be setup statically */
    dv->ctrl_ep = USB_ENDPOINT_XFER_CONTROL;

#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 4, 20)
    for (i = 0; i < 3; i++)
    {
        init_MUTEX(&(dv->barrier[i]));
    }
#endif

    /* Scan HCI/ACL interface for endpoints */
    for (alt = 0; alt < intf->num_altsetting; alt++)
    {
        CsrUint8 i, num_ep;
        struct usb_interface_descriptor *ifaceDesc;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)
        struct usb_host_interface *host;
        host = &(intf->altsetting[alt]);
        ifaceDesc = &(host->desc);
        num_ep = ifaceDesc->bNumEndpoints;
        ifnum = ifaceDesc->bInterfaceNumber;
#else
        ifaceDesc = &(intf->altsetting[alt]);
        num_ep = ifaceDesc->bNumEndpoints;
#endif

        /* Scan endpoints */
        for (i = 0; i < num_ep; i++)
        {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)
            endpoint = &host->endpoint[i].desc;
#else
            endpoint = &ifaceDesc->endpoint[i];
#endif

            /* BULK in */
            if ((endpoint->bEndpointAddress & USB_DIR_IN) &&
                ((endpoint->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK)
                 == USB_ENDPOINT_XFER_BULK))
            {
                if (csr_setup.ext_iface)
                {
                    res = csr_setup.ext_probe(dv, ifnum, intf, alt);
                }
                if ((res == FALSE) &&
                    !test_bit(BULK_IN_READY, &dv->endpoint_present))
                {
                    handleBulkInEndpoint(dv, alt, endpoint);
                }
            }
            /* BULK out */
            else if (!(endpoint->bEndpointAddress & USB_DIR_IN) &&
                     ((endpoint->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK)
                      == USB_ENDPOINT_XFER_BULK))
            {
                if (csr_setup.ext_iface)
                {
                    res = csr_setup.ext_probe(dv, ifnum, intf, alt);
                }
                if ((res == FALSE) &&
                    !test_bit(BULK_OUT_READY, &dv->endpoint_present))
                {
                    handleBulkOutEndpoint(dv, alt, endpoint);
                }
            }
            /* INTR in */
            else if ((endpoint->bEndpointAddress & USB_DIR_IN) &&
                     ((endpoint->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK)
                      == USB_ENDPOINT_XFER_INT))
            {
                if (csr_setup.ext_iface)
                {
                    res = csr_setup.ext_probe(dv, ifnum, intf, alt);
                }
                if ((res == FALSE) &&
                    !test_bit(INTR_IN_READY, &dv->endpoint_present))
                {
                    handleIntrInEndpoint(dv, alt, endpoint);
                }
            }
            else
            {
                printk("bt_usb%u: Unknown endpoint\n", devno);
            }
        }
    }

    /* Scan SCO interface for endpoints */
    for (alt = 0; alt < sco_intf->num_altsetting; alt++)
    {
        CsrUint8 i, num_ep;
        struct usb_interface_descriptor *ifaceDesc;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)
        struct usb_host_interface *host;
        host = &(sco_intf->altsetting[alt]);
        ifaceDesc = &(host->desc);
        num_ep = ifaceDesc->bNumEndpoints;
        ifnum = ifaceDesc->bInterfaceNumber;
#else
        ifaceDesc = &(sco_intf->altsetting[alt]);
        num_ep = ifaceDesc->bNumEndpoints;
#endif

        /* Scan endpoints */
        for (i = 0; i < num_ep; i++)
        {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)
            endpoint = &host->endpoint[i].desc;
#else
            endpoint = &ifaceDesc->endpoint[i];
#endif

            /* ISOC in */
            if ((endpoint->bEndpointAddress & USB_DIR_IN) &&
                ((endpoint->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK)
                 == USB_ENDPOINT_XFER_ISOC))
            {
                if (csr_setup.ext_iface)
                {
                    res = csr_setup.ext_probe(dv, ifnum, sco_intf, alt);
                }
                if ((res == FALSE) &&
                    !test_bit(ISOC_IN_READY, &dv->endpoint_present))
                {
                    handleIsocInEndpoint(dv, alt, endpoint, ifnum);
                }
            }
            /* ISOC out */
            else if (!(endpoint->bEndpointAddress & USB_DIR_IN) &&
                     ((endpoint->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK)
                      == USB_ENDPOINT_XFER_ISOC))
            {
                if (csr_setup.ext_iface)
                {
                    res = csr_setup.ext_probe(dv, ifnum,
                        sco_intf, alt);
                }
                if ((res == FALSE) &&
                    !test_bit(ISOC_OUT_READY, &dv->endpoint_present))
                {
                    handleIsocOutEndpoint(dv, alt, endpoint);
                }
            }
            else
            {
                printk("bt_usb%u: Unknown endpoint\n", devno);
            }
        }
    }

    /* Scan DFU interface for endpoints */
    for (alt = 0; alt < dfu_intf->num_altsetting; alt++)
    {
        struct usb_interface_descriptor *ifaceDesc;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)
        struct usb_host_interface *host;
        host = &(dfu_intf->altsetting[alt]);
        ifaceDesc = &(host->desc);
        ifnum = ifaceDesc->bInterfaceNumber;
#else
        ifaceDesc = &(dfu_intf->altsetting[alt]);
#endif

        /* Detect DFU interface */
        if ((ifaceDesc->bNumEndpoints == 0) &&
            (ifaceDesc->bInterfaceClass == DFU_IFACE_CLASS) &&
            (ifaceDesc->bInterfaceSubClass == DFU_IFACE_SUB_CLASS) &&
            (ifaceDesc->bInterfaceProtocol == DFU_IFACE_PROTOCOL))
        {
            if (!test_bit(DFU_READY, &dv->endpoint_present))
            {
                handleDfuInterface(dv, ifnum);
            }
        }
        else
        {
            printk("bt_usb%u: no valid DFU interface found\n", devno);
        }
    }

    if (test_bit(BULK_IN_READY, &dv->endpoint_present) &&
        test_bit(BULK_OUT_READY, &dv->endpoint_present) &&
        test_bit(ISOC_OUT_READY, &dv->endpoint_present) &&
        test_bit(ISOC_IN_READY, &dv->endpoint_present) &&
        test_bit(INTR_IN_READY, &dv->endpoint_present) &&
        !test_bit(LISTEN_STARTED, &(dv->flags)))
    {
        DBG_PRINT("All required endpoints found, starting loop\n");
        set_bit(DEVICE_CONNECTED, &(dv->flags));
        startListen(dv);

        /* Start the reader thread */
        kernel_thread(readerThread, dv, 0);
    }

    /*
     * Let go of device lock so the
     * reader thread can start.
     */
    up(&dv->devlock);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)
    return 0;
#else
    return dv;
#endif
}
