/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>
#include <sys/uio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_panic.h"
#include "csr_usb_com.h"
#include "csr_transport.h"
#include "csr_tm_bluecore_usb.h"

#define BT_USB_IOC_MAGIC     'c'
#define BT_USB_IOC_ENTER_DFU _IO(BT_USB_IOC_MAGIC, 0)
#define BT_USB_IOC_RESET     _IO(BT_USB_IOC_MAGIC, 1)
#define BT_USB_IOC_MAX_NO    (2)

#define DEFAULT_BT_USB_DEVICE_NAME  "/dev/bt_usb"
#define BT_USB_IOV_SIZE             (2)
#define USB_INIT_MAX_RETRIES		(10)

/*#define DEBUG_ENABLE*/

/* For debugging */
#ifdef DEBUG_ENABLE
#define DBG_PRINT(fmt_args) printf("csr_usb: ");printf fmt_args
#else
#define DBG_PRINT(fmt_args) /* not debugging anything */
#endif

/* For normal printing */
#define PRINT(fmt_args) printf("csr_usb: ");printf fmt_args

typedef struct read_q_elm_t
{
    CsrUint8             channel;
    CsrUint32            length;
    void                *data;
    struct read_q_elm_t *next;
} read_q_elm_t;

typedef struct
{
    read_q_elm_t   *first;
    read_q_elm_t   *last;
    int             count;
    pthread_mutex_t access;
} read_queue_t;

static read_queue_t *rx_queue;
static int bt_usb_fd = -1;
static CsrBool usb_run;
static pthread_t rx_thread_id = 0;
static CsrUint8 *read_buf;

static CsrSchedBgint bgint_usb_rx = CSR_SCHED_BGINT_INVALID;
static char devName[255] = DEFAULT_BT_USB_DEVICE_NAME;

#ifdef DEBUG_ENABLE
static void dump_data(CsrUint32 length, CsrUint8 *data)
{
    int i;

    DBG_PRINT(("Dumping data begin:\n"));

    for (i = 0; i < length; i++)
    {
        printf("0x%02x ", data[i]);

        if ((i + 1) % 16 == 0)
        {
            printf("\n");
        }
    }
    printf("\n");
    DBG_PRINT(("Dumping data end.\n"));
}

#endif

#ifdef ANDROID
static void usb_exit_sighandler(int sig)
{
    pthread_exit(NULL);
}

#endif
/*************************************************************
 * NAME:
 *      CsrUsbDrvRx
 *
 * DESCRIPTION:
 *      This function is called by a background interrupt each
 *      time a packet (ACL, HCI or SCO) is received from the
 *      USB device. The function pass the data to the higher
 *      layers.
 *
 * RETURNS:
 *      -
 *
 *************************************************************/
void CsrUsbDrvRx(void *unused)
{
    MessageStructure upMsg;
    read_q_elm_t *ptr;

    DBG_PRINT(("CsrUsbDrvRx\n"));
    pthread_mutex_lock(&rx_queue->access);

    if (rx_queue->first)
    {
        ptr = rx_queue->first;

        rx_queue->first = ptr->next;

        if (rx_queue->last == ptr)
        {
            rx_queue->last = NULL;
        }
        rx_queue->count--;

        pthread_mutex_unlock(&rx_queue->access);

        upMsg.buf = ptr->data;
        upMsg.buflen = ptr->length;
        upMsg.chan = ptr->channel;

        CsrTmBlueCoreUsbDeliverMsg(&upMsg, 0);

        /* Free the queue element as data has been transferred */
        CsrPmemFree(ptr);

        if (rx_queue->first != NULL)
        {
            DBG_PRINT(("The queue is not empty\n"));
            CsrSchedBgintSet(bgint_usb_rx);
        }
    }
    else
    {
        pthread_mutex_unlock(&rx_queue->access);
    }
}

static void *rx_thread(void *thread_param)
{
    int n;
    CsrUint8 channel;
    struct iovec io[BT_USB_IOV_SIZE];
    struct iovec iosz[1];

    read_q_elm_t *p;
#ifdef ANDROID
    signal(SIGUSR1, usb_exit_sighandler);
#endif

    DBG_PRINT(("rx_thread running \n"));
    while (usb_run)
    {
        CsrUint32 buflen; /* MessageStructure.buflen is of type CsrUint32 */

        iosz[0].iov_base = &buflen;
        iosz[0].iov_len = sizeof(buflen);

        DBG_PRINT(("reading size...\n"));
        n = readv(bt_usb_fd, iosz, 1);
        DBG_PRINT(("readv retval %d\n", n));

        if (n > 0)
        {
            read_buf = CsrPmemAlloc(buflen);
        }
        else
        {
            /* read error, bail out */
            break;
        }


        io[0].iov_base = &channel;
        io[0].iov_len = sizeof(CsrUint8);
        io[1].iov_base = read_buf;
        io[1].iov_len = buflen;

        DBG_PRINT(("read...\n"));
        n = readv(bt_usb_fd, io, BT_USB_IOV_SIZE);
        DBG_PRINT(("readv return: n=%d\n", n));

        if (!usb_run)
        {
            break;
        }

        if (n >= BT_USB_IOV_SIZE)
        {
            p = CsrPmemAlloc(sizeof(read_q_elm_t));
            p->channel = channel;
            p->length = n - sizeof(channel);
            p->data = read_buf;
            p->next = NULL;

            read_buf = NULL;

            pthread_mutex_lock(&rx_queue->access);

            if (rx_queue->first == NULL)
            {
                rx_queue->count++;
                rx_queue->first = p;
                rx_queue->last = p;
            }
            else
            {
                rx_queue->count++;
                rx_queue->last->next = p;
                rx_queue->last = p;
            }

            pthread_mutex_unlock(&rx_queue->access);
            CsrSchedBgintSet(bgint_usb_rx);
        }
        else if (n > 0)
        {
            PRINT(("error (%s:%d): Read vector too little data, got %i wanted %i\n",
                   __FILE__, __LINE__, n, BT_USB_IOV_SIZE));
            usb_run = FALSE;
        }
        else if (n < 0)
        {
            PRINT(("error (%s:%d): Read vector returned error %i\n",
                   __FILE__, __LINE__, n));
            usb_run = FALSE;
        }
    }

    /* Free data buffer if it exists */
    if (read_buf != NULL)
    {
        CsrPmemFree(read_buf);
        read_buf = NULL;
    }

    DBG_PRINT(("USB reader thread stopping...\n"));

    return NULL;
}

/*************************************************************
 * NAME:
 *      CsrUsbDrvTx
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
CsrBool CsrUsbDrvTx(CsrUint8 channel, CsrUint8 *data, CsrUint32 size)
{
    struct iovec out[BT_USB_IOV_SIZE];
//    int n;

    DBG_PRINT(("CsrUsbDrvTx: channel=%d, size=%d\n", channel, size));

#ifdef DEBUG_ENABLE
    dump_data(size, data);
#endif

    out[0].iov_base = &channel;
    out[0].iov_len = sizeof(CsrUint8);
    out[1].iov_base = data;
    out[1].iov_len = size;

 /*   n = */writev(bt_usb_fd, out, BT_USB_IOV_SIZE);

    CsrPmemFree(data);

    return TRUE;
}

/*************************************************************
 * NAME:
 *      CsrUsbDrvStop
 *
 * DESCRIPTION:
 *
 * RETURNS:
 *      -
 *************************************************************/
CsrBool CsrUsbDrvStop(void)
{
    read_q_elm_t *q;

    if (close(bt_usb_fd) == 0)
    {
        CsrSchedBgintUnreg(bgint_usb_rx);
        bgint_usb_rx = CSR_SCHED_BGINT_INVALID;
        bt_usb_fd = -1;
    }
    else
    {
        return FALSE;
    }


    /* Stop thread */
    usb_run = FALSE;
    if (rx_thread_id != 0)
    {
#ifdef ANDROID
        pthread_kill(rx_thread_id, SIGUSR1);
#else
        pthread_cancel(rx_thread_id);
#endif
        DBG_PRINT(("CsrUsbDrvStop: reader thread has been cancelled\n"));
        rx_thread_id = 0;
    }

    /* Free the queue elements */
    while ((q = rx_queue->first) != NULL)
    {
        rx_queue->first = q->next;

        /* Free data and element */
        if (q->data != NULL)
        {
            CsrPmemFree(q->data);
        }
        CsrPmemFree(q);
    }

    /* Free read-buffer */
    if (read_buf != NULL)
    {
        CsrPmemFree(read_buf);
        read_buf = NULL;
    }

    /* Free the queue itself */
    CsrPmemFree(rx_queue);
    rx_queue = NULL;

	PRINT(("[CSR USB] USB Drv Stop waiting 0 sec !!!\n"));
    return TRUE;
}
#ifdef CSR_ENABLE_WARM_RESET

CsrBool CsrUsbDrvWarmReset()
{
    struct iovec out[BT_USB_IOV_SIZE];
    CsrUint8 channel = 5;
    CsrUint8 BcCmdReset[22] = {  0x00, 0xFC, 0x13, 0xC2,
                                0x02, 0x00, 0x09, 0x00, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    DBG_PRINT(("[CSR USB] CsrUsbDrvWarmReset\n"));



	/* Send fake warm reset */
    out[0].iov_base = &channel;
    out[0].iov_len  = sizeof(CsrUint8);
    out[1].iov_base = BcCmdReset;
    out[1].iov_len  = 22;
    writev(bt_usb_fd, out, BT_USB_IOV_SIZE);

	return TRUE;
}
#endif
/*************************************************************
 * NAME:
 *      CsrUsbDrvStart
 *
 * DESCRIPTION:
 *      This function is called to initialize this module, and
 *      to open the USB device specified by CsrUsbSetDevice.
 *      If none specified the defualt is \\.\\CSR0
 *
 * RETURNS:
 *      TRUE if the device was opened successfully;
 *      otherwise FALSE
 *************************************************************/
CsrBool CsrUsbDrvStart(CsrUint8 reset)
{
    CsrBool retval;
    CsrUint8 count;
	static CsrBool fakeReset = FALSE;

    /* Dummy checks */
    if ((rx_thread_id != 0) || (usb_run == TRUE))
    {
        PRINT(("error (%s:%d): USB driver already running\n",
               __FILE__, __LINE__));
        return FALSE;
    }

    read_buf = NULL;

	/* Try to reopen the device */
    count = 0;
    while(count < USB_INIT_MAX_RETRIES)
    {
        /* Open file */
        bt_usb_fd = open(devName, O_RDWR);
        count++;

		DBG_PRINT(("[CSR USB] open number[%d] \n", bt_usb_fd));
        if(bt_usb_fd != -1)
            break;

		/* Allow chip to Boot */
         usleep(2);;
    }	
    
    if (bt_usb_fd == -1)
    {
        PRINT(("error (%s:%d): Could not open devName file: %s\n",
               __FILE__, __LINE__, devName));
        retval = FALSE;
    }
    else
    {
    
#ifdef CSR_ENABLE_WARM_RESET // Sean added this definition for non-warm-reset version
			if(!fakeReset)
			{
				if(!CsrUsbDrvWarmReset())
				{
					DBG_PRINT(("[CSR_USB] error (%s:%d): Warm reset failed.\n",
						  __FILE__, __LINE__, devName));
					return FALSE;
				}
		
		//			fakeReset = TRUE;
			}
#endif
        rx_queue = CsrPmemAlloc(sizeof(read_queue_t));
        rx_queue->first = NULL;
        rx_queue->last = NULL;
        rx_queue->count = 0;
        if (pthread_mutex_init(&rx_queue->access, NULL) != 0)
        {
            PRINT(("error (%s:%d): Could not create mutex\n",
                   __FILE__, __LINE__));
            retval = FALSE;
        }

#ifdef ANDROID
        {
            /* Ensure required signals are unmasked */
            sigset_t set;

            sigemptyset(&set);
            sigaddset(&set, SIGUSR1);
            sigprocmask(SIG_UNBLOCK, &set, NULL);
        }
#endif
        /* Start thread */
        usb_run = TRUE;
        retval = pthread_create(&rx_thread_id, NULL, rx_thread, NULL);
        if (retval != 0)
        {
            PRINT(("error (%s:%d): Could not create reader-thread\n",
                   __FILE__, __LINE__));
            return FALSE;
        }

        bgint_usb_rx = CsrSchedBgintReg(CsrUsbDrvRx, NULL, "usbdrv_rx");
        retval = TRUE;
    }

    return retval;
}

int CsrUsbDrvEnterDFU(void)
{
    int ret;

    /* Send the IOCTL */
    if (bt_usb_fd > 0)
    {
        ret = ioctl(bt_usb_fd, BT_USB_IOC_ENTER_DFU);
    }
    else
    {
        ret = -ENODEV;
    }

    return ret;
}

void CsrUsbDrvConfigure(const char *device)
{
    if (device != NULL)
    {
        strcpy(devName, device);
    }
}
