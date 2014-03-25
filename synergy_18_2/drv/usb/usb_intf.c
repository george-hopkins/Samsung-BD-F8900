/****************************************************************************

               (c) CSR plc 2008

               All rights reserved

FILE:          usb_intf.c

DESCRIPTION:   Linux USB driver interface to the BCHS stack.
               The interface functions are those directly involved in
               sending data to/from BCHS and the low-level USB driver.

REVISION:      $Revision: #17 $

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
#include "include/linux_usb_dev.h"
#include "include/linux_usb_extra.h"

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0) & LINUX_VERSION_CODE < KERNEL_VERSION(2,6,24))
#include <linux/usb_ch9.h>
#endif

/* Globals (device, settings and driver) */
extern csr_dev_t *csr_dev[BT_USB_COUNT];
extern struct semaphore csr_dev_sem;

extern csr_usb_settings_t csr_setup;
extern struct usb_driver csr_usb_driver;

/*************************************************************
 * NAME:
 *      devLookup
 *
 * DESCRIPTION:
 *      Takes a device number parameter, grabs device
 *      semaphore, and returns pointer to the csr_dev_t
 *      struct, or NULL if not found.
 *      Caller MUST release device semaphore if non-NULL
 *      is returned.
 *
 * RETURNS:
 *      csr_dev_t pointer, or NULL.
 *
 *************************************************************/
csr_dev_t *devLookup(uint8_t devno)
{
    csr_dev_t *dv;

    if ((csr_setup.running_with_scheduler && devno > 0) ||
        devno > BT_USB_COUNT - 1)
    {
        dv = NULL;
    }
    else
    {
        dv = csr_dev[devno];

	 /* Sunny@CSR : 2011.09.27 fix NULL pointer exception during BT activation without USB module(BC8510) */
	 if(dv)
 	{
	        down(&dv->devlock); /* XXX: _interruptible? */
 	}
    }

    return dv;
}

/*************************************************************
 * NAME:
 *      readerThread
 *
 * DESCRIPTION:
 *      This function runs as a thread, and is responsible for
 *      calling the bg_interrupt in the Scheduler signalling
 *      that data is ready for being fetched. The thread is signalled
 *      whenever data has arrived from the USB device.
 *
 * RETURNS:
 *      Thread completion code (zero)
 *
 *************************************************************/
int readerThread(void *parm)
{
    csr_dev_t *dv;

    dv = (csr_dev_t *)parm;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0)
    daemonize("bt_usb%u", dv->minor);
#else
    daemonize();
#endif

    set_bit(R_THREAD_RUNNING, &(dv->flags));

    /* Do run while the boss says so */
    while (test_bit(R_THREAD_RUNNING, &(dv->flags)))
    {
        /* Wait for background interrupts */
        while((test_bit(R_THREAD_RUNNING, &(dv->flags))) &&
              (atomic_read(&(dv->await_bg_int)) == 0))
        {
            DBG_VERBOSE("Reader thread entering sleep\n");
            if(wait_event_interruptible(dv->queue.wait,
                                        !test_bit(R_THREAD_RUNNING, &(dv->flags)) ||
                                        atomic_read(&(dv->await_bg_int))!=0))
            {
                break;
            }
        }
        DBG_VERBOSE("Reader thread woke up\n");

        /* Fire the background interrupt */
        if(test_bit(R_THREAD_RUNNING, &(dv->flags)))
        {
            if(csr_setup.running_with_scheduler)
            {
                // bg_int1();
            }
            else
            {
                UsbDev_Rx(dv->minor);
            }
            atomic_dec(&(dv->await_bg_int));
        }
    }

    /* Done */
    clear_bit(R_THREAD_RUNNING, &(dv->flags));
    complete_and_exit(&dv->reader_thread_exit, 0);

    return 0;
}

/*************************************************************
 * NAME:
 *      UsbDriverStarted
 *
 * RETURNS:
 *      A bool_t indicating if the driver is started
 *
 *************************************************************/
bool_t UsbDriverStarted(void)
{
    return TRUE;
}

/*************************************************************
 * NAME:
 *      UsbDeviceProbed
 *
 * RETURNS:
 *      A bool_t indicating if the driver has been probed
 *
 *************************************************************/
bool_t UsbDeviceProbed(csr_dev_t *dv)
{
    if(test_bit(DEVICE_CONNECTED, &(dv->flags)))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*************************************************************
 * NAME:
 *      UsbDriverStopRequested
 *
 * RETURNS:
 *      A bool_t indicating if the driver has been requested to stop
 *
 *************************************************************/
bool_t UsbDriverStopRequested(void)
{
    return FALSE;
}

/*************************************************************
 * NAME:
 *      ExtraUsbInterfaceReady
 *
 * DESCRIPTION
 *      Signal the driver that the extra interface (extension)
 *      is ready for action
 *
 * RETURNS:
 *      void
 *
 *************************************************************/
void ExtraUsbInterfaceReady(csr_dev_t *dv)
{
    set_bit(EXTRA_INTERFACE_READY,&(dv->endpoint_present));
}

/*************************************************************
 * NAME:
 *      SetupUsb
 *
 * DESCRIPTION
 *      Set the extra (extension) interface function pointers.
 *
 * RETURNS:
 *      void
 *
 *************************************************************/
void SetupUsb(uint8_t interfaceNumber,
              void (*interfaceFunc)(void),
              probeExtraInterface_t pEI,
              disconnectExtraInterface_t dEI,
              startDrvExtraInterface_t sDEI,
              initExtraInterface_t iEI,
              setupExtraInterface_t sEI,
              exitExtraInterface_t eEI)
{
    printk(PRNPREFIX "Setup to use extra interface\n");

    csr_setup.ext_iface = TRUE;
    sEI(interfaceNumber, interfaceFunc);
    csr_setup.ext_probe = pEI;
    csr_setup.ext_disconnect = dEI;
    csr_setup.ext_start = sDEI;
    csr_setup.ext_init = iEI;
    csr_setup.ext_exit = eEI;
}

/*************************************************************
 * NAME:
 *      SetSupportedUsbDevices
 *
 * DESCRIPTION
 *      Replace the default list of supported USB devices
 *
 * RETURNS:
 *      void
 *
 *************************************************************/
void SetSupportedUsbDevices(struct usb_device_id *devs)
{
    csr_usb_driver.id_table = devs;
}

/*************************************************************
 * NAME:
 *      SetUsbDriverName
 *
 * DESCRIPTION
 *      Rename default USB driver name
 *
 * RETURNS:
 *      void
 *
 *************************************************************/
void SetUsbDriverName(char *name)
{
    csr_usb_driver.name = name;
}

/*************************************************************
 * NAME:
 *      SetUsbMaxIsocOutFrames
 *
 * DESCRIPTION
 *      Set number of concurrent ISOC-tx frames
 *
 * RETURNS:
 *      void
 *
 *************************************************************/
void SetUsbMaxIsocOutFrames(int out)
{
    csr_setup.max_isoc_out_frames = out;
}

/*************************************************************
 * NAME:
 *      SetUsbMaxIsocInFrames
 *
 * DESCRIPTION
 *      Set number of concurrent ISOC-rx frames
 *
 * RETURNS:
 *      void
 *
 *************************************************************/
void SetUsbMaxIsocInFrames(int in)
{
    csr_setup.max_isoc_in_frames = in;
}

/*************************************************************
 * NAME:
 *      SetUsbIsocAltSetting
 *
 * DESCRIPTION
 *      Set alternate setting for ISOC interface
 *
 * RETURNS:
 *      void
 *
 *************************************************************/
void SetUsbIsocAltSetting(int alt)
{
    csr_setup.isoc_alt_setting = alt;
}

/*************************************************************
 * NAME:
 *      CsrUsbInit
 *
 * DESCRIPTION
 *      Initialize (register) USB driver. When this function
 *      is called, the USB driver is registerd in the kernel
 *      and the other settings can be altered anymore
 *
 * RETURNS:
 *      void
 *
 *************************************************************/
int CsrUsbInit(void)
{
    int err;

    err = usb_register(&csr_usb_driver);
    if(err < 0)
    {
        printk(PRNPREFIX "Failed to register CSR USB driver, code %d\n",err);
    }
    else
    {
        printk(PRNPREFIX "CSR USB driver registered\n");
    }

    return err;
}

/*************************************************************
 * NAME:
 *      CsrUsbExit
 *
 * DESCRIPTION
 *      Shutdown the USB driver and unregister it from the kernel
 *
 * RETURNS:
 *      void
 *
 *************************************************************/
void CsrUsbExit(void)
{
    printk(PRNPREFIX "Shutting down USB driver\n");

    usb_deregister(&csr_usb_driver);

}

/*************************************************************
 * NAME:
 *      UsbDrv_Stop
 *
 * DESCRIPTION
 *      Stop USB device driver. This is not supported!
 *
 * RETURNS:
 *      void
 *
 *************************************************************/
void UsbDrv_Stop(void)
{
    if (csr_setup.running_with_scheduler)
    {
	// register_bg_int(1, NULL);
    }

    CsrUsbExit();

    return;
}

/*************************************************************
 * NAME:
 *      UsbDrv_Reset
 *
 * DESCRIPTION
 *      Perform an USB bus reset of the device
 *
 * RETURNS:
 *      void
 *
 *************************************************************/
void UsbDrv_Reset(void)
{
    UsbDev_Reset(0);
}

/*************************************************************
 * NAME:
 *      UsbDrv_Rx
 *
 * DESCRIPTION:
 *      This function is called by a background interrupt each
 *      time a packet (ACL, SCO or HCI) is received from the
 *      USB device. The function passes the data to the higher
 *      layers.
 *
 * RETURNS:
 *      void
 *
 *************************************************************/
void UsbDrv_Rx(void)
{
    /* Just pass control on to device 0 rx. */
    UsbDev_Rx(0);
}

/*************************************************************
 * NAME:
 *      UsbDrv_Tx
 *
 * DESCRIPTION:
 *      Interface to the above protocol stack, when data is to
 *      be transmitted over the USB interface. Both ACL, SCO
 *      and HCI commands are supported. The type of data is
 *      specified by the channel parameter. A pointer to the
 *      data must be specified in the data parameter. Finally,
 *      the size parameter specifies the size of the data.
 *
 * RETURNS:
 *      TRUE if the data can be buffered for transmission,
 *      otherwise FALSE
 *************************************************************/
bool_t UsbDrv_Tx(uint8_t channel, uint8_t *data, uint16_t size)
{
    return UsbDev_Tx(0, channel, data, size);
}

/*************************************************************
 * NAME:
 *      UsbDrv_Start
 *
 * DESCRIPTION:
 *      This function is called to initialize this module. The
 *      deviceName parameter is not used, but is only present
 *      in order to provide the same interface to BCHS.
 *
 * RETURNS:
 *      TRUE if the device was opened successfully,
 *      otherwise FALSE
 *************************************************************/
bool_t UsbDrv_Start(char *deviceName)
{
    memset(csr_dev, 0, sizeof(csr_dev));
#ifndef init_MUTEX
		sema_init(&csr_dev_sem,1);
#else
		init_MUTEX(&csr_dev_sem);
#endif

    /* Register USB driver */
    CsrUsbInit();

    DBG_PRINT("The BCHS USB driver has been started\n");

    if (csr_setup.running_with_scheduler)
    {
        // register_bg_int(1, UsbDrv_Rx);
    }

    if(csr_setup.ext_iface == TRUE)
    {
        csr_setup.ext_start();
    }

    return TRUE;
}

void UsbDev_Rx(uint8_t devno)
{
    csr_dev_t *dv;
    char *tmpbuf;
    struct usb_qe *qe;

    dv = devLookup(devno);

    if(dv)
    	{
	    if((atomic_read(&(dv->queue.count)) > 0 ) &&
	       test_bit(DEVICE_CONNECTED, &(dv->flags)))
	    {
	        do
	        {
	            qe = QueuePop(dv);
	            if(qe != NULL)
	            {
	                DBG_VERBOSE("Sent %i bytes to stack\n",
	                            qe->msg->buflen);

	                /* Send to stack, which consumes the message buffer */
	                tmpbuf = qe->msg->buf;

	                hcCom_ReceiveMsg(qe->msg, qe->chan,
	                    csr_setup.running_with_scheduler ? 0 : dv->minor);

	                /* The SCO data itself must be freed */
	                if ((csr_setup.running_with_scheduler) &&
	                    (qe->chan == BCSP_CHANNEL_SCO))
	                {
	                    pfree(tmpbuf);
	                }

	                if (csr_setup.running_with_scheduler != FALSE)
	                {
	                    /*
	                     * User mode Linux steals this pointer
	                     * so only free it with in-kernel stack.
	                     */
	                    pfree(qe->msg);
	                }

	                /* Free the queue-element */
	                pfree(qe);
	            }
	        }
	        while (qe != NULL);
	    }

	    up(&dv->devlock);
	}

    return;
}

bool_t UsbDev_Tx(uint8_t devno, uint8_t channel, uint8_t *data, uint16_t size)
{
    csr_dev_t *dv;
    bool_t res;

    DBG_VERBOSE("UsbDev_Tx : Have received devno<%d> <%i> bytes from Synergy BT\n", devno, size);

    dv = devLookup(devno);

    res = FALSE;
	if(dv)
	{
	    DBG_VERBOSE("UsbDev_Tx : dv is NOT NULL \n");

		if((size > 0) &&
	       test_bit(DEVICE_CONNECTED, &(dv->flags)))
	    {
	        if(channel == BCSP_CHANNEL_HCI)
	        {
	            usbTxCtrl(dv, data, size);
	            res = TRUE;
	        }
	        else if(channel == BCSP_CHANNEL_ACL)
	        {
	            usbTxBulk(dv, data, size);
	            res = TRUE;
	        }
	        else if(channel == BCSP_CHANNEL_SCO)
	        {
	            usbTxIsoc(dv, data, size);
	            res = TRUE;
	        }
	    }
		
	    up(&dv->devlock);
	}


    return res;
}

void UsbDev_Reset(uint8_t devno)
{
    csr_dev_t *dv;

    printk(PRNPREFIX "Resetting USB device..\n");

    dv = devLookup(devno);

    if(dv)
   {
	    usb_reset_device(dv->dev);

	    up(&dv->devlock);
    	}
}

#ifdef CONFIG_USB_SUSPEND
void UsbDev_PmDisable(uint8_t devno)
{
    csr_dev_t *dv;

	struct usb_interface* usb_intf;

    dv = devLookup(devno);
	up(&dv->devlock);
	usb_intf = usb_find_interface(&csr_usb_driver, devno);
	
    usb_autopm_disable(usb_intf);
	printk(PRNPREFIX "bt_usb%u: disabling power-save mode (cnt: %d)\n", devno, usb_intf->pm_usage_cnt);
}

void UsbDev_PmEnable(uint8_t devno)
{
    csr_dev_t *dv;

	struct usb_interface* usb_intf;

    dv = devLookup(devno);
	up(&dv->devlock);
	usb_intf = usb_find_interface(&csr_usb_driver, devno);

    usb_autopm_enable(usb_intf);
	printk(PRNPREFIX "bt_usb%u: enabling power-save mode (cnt: %d)\n", devno, usb_intf->pm_usage_cnt);
}
#endif
