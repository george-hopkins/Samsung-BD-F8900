/* drivers/hid/csrhid/csrhid_loopback.c
 *
 * HID loopback driver for Synergy
 * Copyright (C) 2009 CSR.
 * Author: Sunil Pillai
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include <linux/kernel.h>
#include <linux/version.h> 
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/mm.h>
#include <linux/mutex.h>
#include <linux/spinlock.h>
#include <asm/unaligned.h>
#include <asm/byteorder.h>
#include <linux/input.h>
#include <linux/wait.h>
#include <linux/workqueue.h>
#include <linux/usb.h>
#include <asm/uaccess.h>

#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/device.h>


#include "csrhid_loopback.h"

#define DEVICE_NAME     "csrhid"
#define REPORT_SIZE 1
#define HID_HDR_SIZE    4

#define BLUETOOTHINPUT_MAJOR 225

csrhid_driver *hCsrhid = NULL;
Csrint8 *csrhid_device_name = "csrbluedev";
Csrint8 report_desc_done = 0;

#ifdef USE_TASKLET
/* Function prototype for Tasklet-Write function */
void csrhid_bdb3_tasklet_write (unsigned long);

/* Initialization TaskLet for performing Write */
DECLARE_TASKLET_DISABLED(csrhid_rx_tasklet, csrhid_bdb3_tasklet_write, 0);
#endif


static const struct hid_device_id csrhid_bdb3_table[] = {
        { HID_USB_DEVICE(HID_ANY_ID, HID_ANY_ID) },
        { }
};

struct hid_driver csrhid_bdb3_driver = {
        .name = "csrhid",
        .id_table = csrhid_bdb3_table,
        /*.raw_event = csrhid_bdb3_raw_event,
         .probe = csrhid_bdb3_probe, */

};

/** csrhid_bdb3_chardev_open Function
 *  This function will perform an register
 *
 *  Parameters :
 *  @file  : File pointer for HID loopback char driver
 *  @inod  :
 *  Returns  ? -  on success
 *           else suitable error code
 */
int csrhid_bdb3_chardev_open(struct inode *inod, struct file *file)
{
    int retval = -ENXIO;

    CSRHID_VER("Inside %s ", __func__);

    if (  (!test_bit(HID_DRIVER_STATE_REGISTERED_BIT, (const volatile void *)&hCsrhid->state)) &&
                (test_bit(HID_DRIVER_STATE_INIT_BIT, (const volatile void *)&hCsrhid->state))  )
    {

        CSRHID_VER("Inside %s ", __func__);
        /* Register the driver with HID Class driver */
        retval = hid_register_driver(&csrhid_bdb3_driver);
        if(retval){
            CSRHID_ERR(" %s: Hid Register Driver Failed..!! retval=%d \n", __func__, retval);
            return CSRHID_ERR_HID_API_REGISTER_FAIL;
        }

        set_bit(HID_DRIVER_STATE_REGISTERED_BIT, (volatile void *)&hCsrhid->state);

        CSRHID_VER(" %s: CSR hid Driver Device allocation Success  \n", __func__);
        /* Initializes devices property like, BUS, name, ll_driver etc */
    } // end of if (!test_bit(HID_DRIVER_ST ......

    return CSRHID_SUCCESS;
}

/** csrhid_bdb3_chardev_read Function
 *  This function will perform an register on ST driver.
 *
 *  Parameters :
 *
 *
 *
 *
 */
ssize_t csrhid_bdb3_chardev_read(struct file *fil, char __user *data, size_t size,
            loff_t *offset)
{
    CSRHID_VER("Inside %s ", __func__);
    return CSRHID_SUCCESS;
}

/** csrhid_bdb3_chardev_write Function
 *  This function will perform an register on ST driver.
 *
 *  Parameters :
 *
 *
 *
 *
 */
ssize_t csrhid_bdb3_chardev_write(struct file *file, const char __user *data,
             size_t size, loff_t *offset)
{
    CsrUint8 deviceIndex = 0;
    Csrint32 retval = size;
    struct hid_device *hid;
    csrhid_message *recv_data = NULL;
    CsrUint8 *report_data = NULL;
    struct sk_buff *skb = NULL;
	int result;

    CSRHID_VER("%s:  Received message Size  is %d \n", __func__, size);

    if(data == NULL) {
        CSRHID_DBG("%s: data received is NULL \n", __func__);
        retval = -EINVAL;
        goto exit_write;
    }

    skb = alloc_skb(size, GFP_ATOMIC);
    if(skb == NULL) {
        CSRHID_DBG("%s: alloc_skb failed \n", __func__);
        retval = -ENOMEM;
        goto exit_write;
    }

    /*  Copy the data from user space in the skb buffer */
    if (copy_from_user(skb_put(skb, size), data, size)) {
        CSRHID_ERR(" Unable to copy from user space");
        retval = CSRHID_ERR_CPY_FRM_USER;
        goto exit_write;
    }

    recv_data =(csrhid_message*) &(skb->data[0]);
    if (recv_data == NULL) {
        CSRHID_DBG("%s: recv_data  is NULL \n", __func__);
        retval = CSRHID_ERR_NULL_PTR;
        goto exit_write;
    }

    /* Sanity check the device index parameter */
    deviceIndex = (CsrUint8) recv_data->deviceIndex;
    if(MAX_DEVICES_SUPPORTED <= deviceIndex)
    {
        CSRHID_VER(" Invalid deviceIndex \n");
        retval = CSRHID_ERR_INVALID_OPER;
        goto exit_write;
    }

    CSRHID_VER("%s:  recv_data rxcount =  %d\n", __func__, hCsrhid->rx_count);
    CSRHID_VER(" deviceIndex = %d \n", deviceIndex);
    CSRHID_VER(" opcode = %d \n", recv_data->opcode);
    CSRHID_VER(" messageSize = %d \n", (CsrUint16) recv_data->messageSize);

    CSRHID_VER(" vendorId = %x \n", (CsrUint16) recv_data->vendorId);
    CSRHID_VER(" productId = %x \n", (CsrUint16) recv_data->productId);
    CSRHID_VER(" deviceVersion = %x \n", (CsrUint16) recv_data->deviceVersion);

    /* For a new connection initialize the device, This will be called for second connection only, since by default on open
    call to "/dev/bluetoothinput" we initialize the first device

    Hence, multiple calls to open "/dev/bluetoothinput will not create & register new devices. Instead for every new device
    except the first default device the below function will be called to initialize it
    */
    switch (recv_data->opcode)
    {
        case CSRHID_DRV_CONNECT:

        hCsrhid->hid_dev[deviceIndex].device = NULL;
        /* Initialize the device if it is not initialized & move device state to init */
        if (!test_bit( HID_DEV_STATE_INIT_BIT , (const volatile void *)&(hCsrhid->hid_dev[deviceIndex].state)) ) {
            CSRHID_VER(" Adding new device %d  \n",deviceIndex);
            /* Initializes devices property like, BUS, name, ll_driver etc */
            hCsrhid->hid_dev[deviceIndex].device = hid_allocate_device();

            if (IS_ERR(hCsrhid->hid_dev[deviceIndex].device)) {
                CSRHID_ERR(" %s: CsrHid device %d allocate Failed..!!\n",__func__, deviceIndex);
                retval = CSRHID_ERR_HID_API_ALLOC_DEV_FAIL;
                goto exit_write;
            }

            /* Initializes devices property like, Bus, name, ll_driver etc */
            csrhid_initialize_device(deviceIndex, recv_data->vendorId, recv_data->productId, recv_data->deviceVersion);

            /*Adds a new device by calling device_add */
            retval = hid_add_device(hCsrhid->hid_dev[deviceIndex].device);
            if (retval) {

                if (retval != -ENODEV)  {
                    printk("can't add csrhid device \n");
                }
                retval = CSRHID_ERR_HID_API_REGISTER_FAIL;
                goto err1_write;
            }
            CSRHID_VER("  new device state %d  \n", hCsrhid->hid_dev[deviceIndex].state);
            hCsrhid->device_count++;
        }

    /*
    Though as per the design, both hid report desc & hid report data had to be queued in the skb buffer and
    handled in the tasklet, we do it only for hid report data.

    In case of report desc we queue it in the skb buffer but he handling is done here itself instead of scheduling
    tasklet & handling in the tasklet function. This was done because handling hid_parse_report inside tasklet
    was creating crash error, details below:

    "Internal error: Oops: 817"
    "Unable to handle kernel NULL pointer dereference at virtual address 00000000"

    This needs to be resolved however.
    */

     /* This one looks like a report desc, obtain and lock & dequeue it  */

    /* Device is Initialized at this point, go ahead and register it with HID CLass Driver */


           CSRHID_VER("%s:  Received Report Descriptor for device %d \n", __func__, deviceIndex);
           retval = hid_parse_report( hCsrhid->hid_dev[deviceIndex].device , (CsrUint8*) recv_data->report, recv_data->messageSize);

            if( retval) {
                CSRHID_DBG("%s: hid_parse_report FAILED \n", __func__);
                retval = CSRHID_ERR_HID_API_PARSE_FAIL;
                goto err1_write;
             }
            else
                CSRHID_DBG("%s: hid_parse_report Successful", __func__);

            retval =  hid_hw_start(hCsrhid->hid_dev[deviceIndex].device, HID_CONNECT_HIDINPUT);
            if(retval) {
                CSRHID_ERR(" %s: hid_hw_start() failed \n", __func__);
                retval = CSRHID_ERR_HID_API_PARSE_FAIL;
                goto err1_write;
            }

            set_bit( HID_DEV_STATE_REGISTERED_BIT , (volatile void *)&(hCsrhid->hid_dev[deviceIndex].state));

        goto exit_write;
        break;

        case CSRHID_DRV_DISCONNECT:
             CSRHID_VER("==>> %s:  Received CSRHID_DRV_DISCONNECT\n", __func__);
            /* This is a disconnect request, which is Blocking, so dont queue it, instead process it here itself */

            if(test_bit(HID_DRIVER_STATE_REGISTERED_BIT, (const volatile void *)&hCsrhid->state)) {

                clear_bit( HID_DEV_STATE_INIT_BIT , (volatile void *)&(hCsrhid->hid_dev[deviceIndex].state));
                clear_bit( HID_DEV_STATE_REGISTERED_BIT , (volatile void *)&(hCsrhid->hid_dev[deviceIndex].state));
                CSRHID_DBG("  %s: CSRHID_DRV_DISCONNECT:  deviceid  %d \n", __func__, deviceIndex);

                hid = (struct hid_device*) ( hCsrhid->hid_dev[deviceIndex].device);
                if (hid == NULL )
                {
                    CSRHID_ERR(" %s: hCsrhid->hid_dev[%d].device = null \n", __func__, deviceIndex);
                    retval = CSRHID_ERR_NULL_PTR;
                    goto exit_write;
                }

                hid_hw_stop(hid);

                /* Below routines return void. Hence we just return size & exit gracefully */
                hidinput_disconnect(hid);
                hid_destroy_device(hid);
                hCsrhid->hid_dev[deviceIndex].device = NULL;
           }

        goto exit_write;
        break;

        case CSRHID_DRV_DATA:
        CSRHID_VER("==>> %s:  Received CSRHID_DRV_DATA \n", __func__);

        if (!test_bit( HID_DEV_STATE_REGISTERED_BIT , (const volatile void*) &(hCsrhid->hid_dev[deviceIndex].state) ) ) {
            CSRHID_ERR(" %s: CsrHid CSRHID_DRV_DATA received in wrong state -  %d \n",
            __func__, hCsrhid->hid_dev[deviceIndex].state);
            retval = CSRHID_ERR_DRV_STATE;
            goto exit_write;
        }


        if (recv_data->report == NULL)
        {
            CSRHID_ERR(" %s: recv_data->report is NULL ", __func__);
            retval = CSRHID_ERR_NULL_PTR;
            goto exit_write;
        }
        /*  Copy the report alone from user space in the skb buffer
            *NOTE THIS IS DONE ONLY IN CASE OF A REPORT DATA, SINCE IT IS HANDLED
            ASYNCHRONOUSLY.

            It is freed in the tasklet
        */
        report_data = (CsrUint8 *) kmalloc( recv_data->messageSize , GFP_KERNEL);
        if (report_data == NULL )
        {
            CSRHID_ERR(" %s: kmalloc failed for report_data, report_data", __func__);
            retval = -ENOMEM;
            goto exit_write;
        }

        if (copy_from_user(report_data, recv_data->report, recv_data->messageSize)) {
            CSRHID_ERR(" Unable to copy report from user space");
            kfree(report_data);
            retval = CSRHID_ERR_CPY_FRM_USER;
            goto exit_write;
        }

        /* save the copied report pointer in the skb report pointer*/
        recv_data->report = report_data;

#ifdef USE_TASKLET

        CSRHID_VER(" SKB added to Rx queue");

        /* Add it to Rx queue */
        skb_queue_tail(&hCsrhid->rx_queue, skb);

        hCsrhid->rx_count++;

        /* This is a HID report Data , schedule the tasklet to process it */
        CSRHID_VER("%s: Tasket Scheduled  hCsrhid->rx_count= %d \n", __func__, hCsrhid->rx_count);

        if ((0 != hCsrhid->rx_count) && (!skb_queue_empty(&hCsrhid->rx_queue))) {

            /* Schedule the Tx-task let */
            tasklet_schedule(&csrhid_rx_tasklet);
        }
#else
	    result = hid_input_report( hCsrhid->hid_dev[deviceIndex].device , HID_INPUT_REPORT, recv_data->report, recv_data->messageSize, 0);
        CSRHID_DBG("  %s:hid_input_report  result is %d\n", __func__, result);
#endif

        goto exit_write;
        break;

        default:
        CSRHID_ERR("%s:  HID device[%d], Improper State- %d. Check \n", __func__, deviceIndex,
        hCsrhid->hid_dev[deviceIndex].state);
        goto exit_write;
        break;
    }

    err1_write:
    if (hCsrhid->hid_dev[deviceIndex].device != NULL) {
        kfree(hCsrhid->hid_dev[deviceIndex].device);
        hid_destroy_device(hCsrhid->hid_dev[deviceIndex].device);
    }

    exit_write:
    if (recv_data->opcode != CSRHID_DRV_DATA)
        kfree_skb(skb);
    return retval;
}

/** csrhid_bdb3_chardev_ioctl Function
 *  This function will perform an register on ST driver.
 *
 *  Parameters :
 *
 *
 *
 *
 */
static int csrhid_bdb3_chardev_ioctl(struct inode *inode, struct file *file,
                unsigned int cmd, unsigned long arg)
{
    CSRHID_VER("Inside %s ", __func__);
    return 0;
}

/** csrhid_bdb3_chardev_release Function
 *  This function will perform an register on ST driver.
 *
 *  Parameters :
 *
 *
 *
 *
 */
int csrhid_bdb3_chardev_release(struct inode *inod, struct file *file)
{
    CSRHID_VER("Inside %s ", __func__);
    return 0;
}





/* Hid Loopback I/O Interface Char driver function pointers
 * These functions are called from User space by performing File Operations
 * on /dev/SynergyInput node exposed by this driver during init
 */
struct file_operations csrhid_bdb3_chrdev_ops = {
    .owner = THIS_MODULE,
    .open = csrhid_bdb3_chardev_open,
    .read = csrhid_bdb3_chardev_read,
    .write = csrhid_bdb3_chardev_write,
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 35)
    .ioctl        = csrhid_bdb3_chardev_ioctl,
#else
    .unlocked_ioctl    = csrhid_bdb3_chardev_ioctl,
#endif
    .release = csrhid_bdb3_chardev_release,
};




/** csrhid_bdb3_ll_parse Function
 *  This function will....
 *
 *  Parameters :
 *
 *
 *
 *
 */
static int csrhid_bdb3_ll_parse(struct hid_device *hid)
{
    CSRHID_VER("Inside %s ", __func__);
    return 0;
}


/** csrhid_bdb3_ll_start Function
 *  This function will....
 *
 *  Parameters :
 *
 *
 *
 *
 */
static int csrhid_bdb3_ll_start(struct hid_device *hid)
{
    CSRHID_VER("Inside %s ", __func__);
    return 0;
}

/** csrhid_bdb3_ll_stop Function
 *  This function will....
 *
 *  Parameters :
 *
 *
 *
 *
 */
static void csrhid_bdb3_ll_stop (struct hid_device *hid)
{
    CSRHID_VER("Inside %s ", __func__);
}

/** csrhid_bdb3_ll_open Function
 *  This function will....
 *
 *  Parameters :
 *
 *
 *
 *
 */
int csrhid_bdb3_ll_open(struct hid_device *hid)
{
    CSRHID_VER("Inside %s ", __func__);
    return 0;
}

/** csrhid_bdb3_ll_close Function
 *  This function will....
 *
 *  Parameters :
 *
 */
void csrhid_bdb3_ll_close(struct hid_device *hid)
{
    CSRHID_VER("Inside %s ", __func__);
}

/** csrhid_bdb3_ll_input_event Function
 *  This function will....
 *
 *  Parameters :
 *
 *
 */
static int csrhid_bdb3_ll_input_event(struct input_dev *dev, unsigned int type, unsigned int code, int value)
{
    CSRHID_VER("Inside %s ", __func__);
    return 0;
}





#if 0
Csrint32 csrhid_bdb3_probe(struct hid_device *dev, const struct hid_device_id *id)
{
        CSRHID_VER("Inside %s ", __func__);
        return 0;
}


Csrint32  csrhid_bdb3_raw_event(struct hid_device *hdev, struct hid_report *report,
            u8 *data, Csrint32 size)
{
    CSRHID_VER("Inside %s ", __func__);

    //struct hid_report_enum *report_enum = hdev->report_enum + HID_INPUT_REPORT;

    if (!report)
    {
        CSRHID_ERR("%s : hid_get_report Failed", __func__);
        return -1;
    }

    if (hdev->claimed & HID_CLAIMED_INPUT)
        hidinput_report_event(hdev, report);

    return 0;
}
#endif


/* Hid Loopback HID class driver initialisations */

static struct hid_ll_driver csrhid_bdb3_ll_driver = {
        .parse = csrhid_bdb3_ll_parse,
        .start = csrhid_bdb3_ll_start,
        .stop = csrhid_bdb3_ll_stop,
        .open = csrhid_bdb3_ll_open,
        .close = csrhid_bdb3_ll_close,
        .hidinput_input_event = csrhid_bdb3_ll_input_event,
};

/*
    This routine initiazes a device provided the device_count param, which is the
    device number. device_count is zero for first device. This function gets called during
    driver initialization (csr_bdb3_init) with device_count = 0.
*/
void  csrhid_initialize_device(CsrUint8 deviceIndex, CsrUint16 vendorId, CsrUint16 productId, CsrUint16 deviceVersion)
{
    CSRHID_VER("Inside %s deviceIndex= %d \n", __func__, deviceIndex);
    hCsrhid->hid_dev[deviceIndex].device->bus     = BUS_BLUETOOTH;
    hCsrhid->hid_dev[deviceIndex].device->vendor = vendorId;                            /* Vendor ID */
    hCsrhid->hid_dev[deviceIndex].device->product = productId;                           /* Product ID */
    hCsrhid->hid_dev[deviceIndex].device->version = deviceVersion;                      /* HID version */
   printk("%s:vendorID %x productId %x \n", __func__, vendorId, productId);
    switch (vendorId)
    {
        case VENDOR_LOGITECH:
            strncpy(csrhid_device_name, "logitech", 12);
            strncpy( hCsrhid->hid_dev[deviceIndex].device->name, csrhid_device_name, 12); //name = csrbluedev
        break;

        case VENDOR_MICROSOFT:
            if (productId == VENDOR_MICROSOFT_6000)
            {
            strncpy(csrhid_device_name, "microsoft", 12);
            strncpy( hCsrhid->hid_dev[deviceIndex].device->name, csrhid_device_name, 12);
            }
            else
            { /* Use default keylayout since it is not a Microsoft Mobile 6000 Keyboard */ }
        break;

        default:
            printk("%s: Using Default CSR keylayout file %s for vendorId %x \n", __func__, csrhid_device_name, vendorId);
        break;
    };

    printk("%s: Using  keylayout %s file \n", __func__ , csrhid_device_name);
    strncpy( hCsrhid->hid_dev[deviceIndex].device->name, csrhid_device_name, 12);
    hCsrhid->hid_dev[deviceIndex].device->ll_driver = &csrhid_bdb3_ll_driver;
    hCsrhid->hid_dev[deviceIndex].device->driver   = &csrhid_bdb3_driver;
    set_bit(  HID_DEV_STATE_INIT_BIT , (volatile void *)&(hCsrhid->hid_dev[deviceIndex].state));

    CSRHID_VER("Inside %s device State = %d ", __func__, hCsrhid->hid_dev[deviceIndex].state);
}

static int __init csrhid_bdb3_init(void)
{
    Csrint32 retval = CSRHID_SUCCESS;

    /*Allocate memory for global handler for hid loopback driver */
    hCsrhid = kzalloc(sizeof(csrhid_driver), GFP_KERNEL);

    /* HID loopback I/O interface(chardriver interface) Initialisations */
    if (!(hCsrhid)) {
        CSRHID_ERR("%s: Can't allocate HID data structure", __func__);
        retval = -ENFILE;
        goto exit_init;
    }
    hCsrhid->char_drv = kzalloc(sizeof(csrhid_bdb3_chrdev_data), GFP_KERNEL);
    if (!(hCsrhid->char_drv)) {
        CSRHID_ERR("%s: Can't allocate HID data structure for hCsrhid->char_drv", __func__);
        retval = -ENFILE;
        goto err1_init;
    }

    retval = initialize_chardriver();
    if (retval < 0)
    {
        CSRHID_ERR("%s: initialize_chardriver Failed..!", __func__);
        retval =  CSRHID_ERR_CHAR_DRV_API_FAIL;
        goto err2_init;
    }
    else
    {
        set_bit(HID_DRIVER_STATE_INIT_BIT, (volatile void *)&hCsrhid->state);

        /* Initialize wait queue, skb queue head and registration complete strucuture   */
        skb_queue_head_init(&hCsrhid->rx_queue);
        hCsrhid->rx_count = 0;
        hCsrhid->tx_count = 0;
#ifdef USE_TASKLET
        tasklet_enable(&csrhid_rx_tasklet);
#endif
        goto exit_init;
    }

    /*
    * Should check where does this code actually go?
    * Either here itself or in write after receving HID REPORT DESCRIPTOR
    *
    * This Confusion arises because:
    *   hid_connect fails because csrhid_device->maxcollection=0
    *   maxcollection parameter is set, probably when HID REPORT DESCRIPTOR is rcvd in write
    *   and is passed to hid_parse_report
    * Register the input hid device; Configure the input layer interface for this device
    * Read all reports and initialize the absolute field values.
    * hid_hw_start redirects to hidinput_connect() in hid-input.c
    */
    #if 0
       retval =  hid_hw_start(csrhid_device, HID_CONNECT_HIDINPUT);
    #endif


    err2_init:
    kfree(hCsrhid->char_drv);
    err1_init:
    kfree(hCsrhid);
    exit_init:
    return retval;
}


Csrint32 initialize_chardriver()
{
    Csrint32 retval =  CSRHID_SUCCESS;
    hCsrhid->device_count = 0;

    /* Register the driver with the character subsystem */
    hCsrhid->char_drv->csrhid_chrdev_major = register_chrdev(BLUETOOTHINPUT_MAJOR, DEVICE_NAME,
                        &csrhid_bdb3_chrdev_ops);

    if(hCsrhid->char_drv->csrhid_chrdev_major < 0) {
        CSRHID_ERR(" %s: register_chrdev failed \n", __func__);
        retval = CSRHID_ERR_HID_API_REGISTER_FAIL;
        goto exit_initchardrv;
    }
    /*  udev Initializations */
    hCsrhid->char_drv->csrhid_chrdev_class = class_create(THIS_MODULE, DEVICE_NAME);
    if (IS_ERR(hCsrhid->char_drv->csrhid_chrdev_class)) {
        CSRHID_ERR(" %s: class_create failed \n", __func__);
        retval = CSRHID_ERR_CHAR_DRV_API_FAIL;
        goto err1_initchardrv;
    }
    CSRHID_VER(" %s: Class_create Success \n", __func__);

    hCsrhid->char_drv->csrhid_chrdev_device =
    device_create(hCsrhid->char_drv->csrhid_chrdev_class, NULL,
     MKDEV(hCsrhid->char_drv->csrhid_chrdev_major, 0), NULL, DEVICE_NAME);

    if (IS_ERR(hCsrhid->char_drv->csrhid_chrdev_device)) {
        CSRHID_DBG(" %s: Device create failed \n", __func__);
        retval =  CSRHID_ERR_CHAR_DRV_API_FAIL;
        goto err2_initchardrv;
    }

    CSRHID_VER(" %s:Char Device create Successful \n", __func__);
    goto exit_initchardrv;
    err2_initchardrv:
    class_destroy(hCsrhid->char_drv->csrhid_chrdev_class);
    err1_initchardrv:
    /* Unregister the char driver */
    unregister_chrdev(hCsrhid->char_drv->csrhid_chrdev_major, DEVICE_NAME);
    exit_initchardrv:
    return retval;
}

#ifdef USE_TASKLET

void csrhid_bdb3_tasklet_write (unsigned long data)
{
    struct sk_buff *skb = NULL;
    static CsrUint8 *report, deviceIndex = 0;
    csrhid_message *recv_data = NULL;
    message_opcode opcode;
    int result = -1;

    /* Perform sanity check of verifying the status to perform an st_write */
    if ((0 == hCsrhid->rx_count)) {
        CSRHID_ERR("  %s: 0 == hCsrhid->rx_countL", __func__);
        return;
    }

    if ( !skb_queue_empty(&hCsrhid->rx_queue) )
    {
        skb = skb_dequeue(&hCsrhid->rx_queue);
        /* Decrement the count of data in the queue */
        hCsrhid->rx_count--;
        CSRHID_DBG("  %s: skb_dequeue Done, rx_count =%d ", __func__, hCsrhid->rx_count);
    }
    else
        CSRHID_ERR("  %s: skb_queue_empty Failed \n", __func__);

    if(skb == NULL) {
        CSRHID_DBG("  %s: skb is NULL", __func__);
        return;
    }

    recv_data =(csrhid_message*) &(skb->data[0]);
    opcode = (CsrUint8) (recv_data->opcode);
    deviceIndex = (CsrUint8) (recv_data->deviceIndex);

    /* Handle all opcode operations on the received data in the Rx Queue */
    switch(opcode)
    {
        case CSRHID_DRV_CONNECT:
            CSRHID_DBG("  %s: CSRHID_DRV_CONNECT:  Device Already Connected \n", __func__);
            return;
        break;

        case CSRHID_DRV_DISCONNECT:
            CSRHID_DBG("  %s: CSRHID_DRV_DISCONNECT:  <Not to be Handled Here> \n", __func__);

        /* TODO: If this works then try putting all this code into  hid_hw_stop instead*/
            /* We have made Driver Disconnect as Blocking Call */

        break;

        case CSRHID_DRV_DATA:
            CSRHID_DBG("  %s: CSRHID_DRV_DATA:   recv_data->messageSize=%d \n", __func__, recv_data->messageSize);

            report = (CsrUint8*)recv_data->report;

        if (  report != NULL) {
            result = hid_input_report( hCsrhid->hid_dev[deviceIndex].device , HID_INPUT_REPORT, report, recv_data->messageSize, 0);
            CSRHID_DBG("  %s:hid_input_report  result is %d\n", __func__, result);
            }
            else
                CSRHID_ERR("  %s: <Check> Null report passed to hid_input_report  \n", __func__);
        break;
    }

    /* Free the dequeued message pointers*/
    if(recv_data->report != NULL) {
        kfree (recv_data->report);
    }

    kfree_skb (skb);

    if ((0 != hCsrhid->rx_count) && (!skb_queue_empty(&hCsrhid->rx_queue))) {
        /* Schedule the Tx-task let */
        CSRHID_DBG("  %s: Scheduling tasklet for next data \n", __func__);
        tasklet_schedule(&csrhid_rx_tasklet);
    }
}

#endif


static void __exit csrhid_bdb3_exit(void)
{
    CsrUint8 cnt = 0;
    csrhid_bdb3_chrdev_data *chardriver = hCsrhid->char_drv;

    CSRHID_DBG("Inside %s ", __func__);

    skb_queue_purge(&hCsrhid->rx_queue);
    /* calls unregister and destroys the device that was created for char driver */
    device_destroy(hCsrhid->char_drv->csrhid_chrdev_class,  MKDEV(hCsrhid->char_drv->csrhid_chrdev_major, 0));

    /* Unregister the char device class created */
    class_unregister(chardriver->csrhid_chrdev_class);

    /* Destroy the char device class object created  */
    class_destroy(chardriver->csrhid_chrdev_class);

    /* Unregister the char driver */
    unregister_chrdev(chardriver->csrhid_chrdev_major, DEVICE_NAME);

    /* HID driver interfaces cleanup */



    for( cnt = 0; cnt < hCsrhid->device_count; cnt ++)
    {
        CSRHID_DBG("  %s: Destroying device %d \n", __func__, cnt);
        /*  Destroy the hid device */
        if (hCsrhid->hid_dev[cnt].device != NULL)
        {
            hid_hw_stop(hCsrhid->hid_dev[cnt].device);
            hidinput_disconnect(hCsrhid->hid_dev[cnt].device);
            hid_destroy_device(hCsrhid->hid_dev[cnt].device);
        }
    }
#ifdef USE_TASKLET
    tasklet_kill(&csrhid_rx_tasklet);
#endif

    if (test_bit(HID_DRIVER_STATE_REGISTERED_BIT, (const volatile void *)&hCsrhid->state))
        hid_unregister_driver(&csrhid_bdb3_driver);

    if(hCsrhid != NULL){
        kfree(hCsrhid);
    }

    CSRHID_DBG("Exiting %s ", __func__);
}

module_init(csrhid_bdb3_init);
module_exit(csrhid_bdb3_exit);

MODULE_DESCRIPTION("CSR Synergy HID loopback driver");
MODULE_AUTHOR("Feb7 2010, Cambridge Silicon Radio Ltd.");
MODULE_LICENSE("GPL and additional rights");
