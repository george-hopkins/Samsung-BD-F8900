#ifndef CSR_LINUX_USB_COM_H__
#define CSR_LINUX_USB_COM_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <linux/usb.h>
#include <linux/spinlock.h>
#include <linux/version.h>
#include "csr_types.h"
#include "csr_transport.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Number of supported devices */
#ifndef BT_USB_COUNT
#define BT_USB_COUNT 32
#endif

/* Misc */
#define BT_CTRL_REQUEST         0x20
#define PRNPREFIX               "<1>csr_usb: "

/* CSR nanosira information */
#define CSR_VENDOR_ID           0x0A12
#define CSR_PRODUCT_ID          0x0001

/* Packet sizes */
#define HCI_EVENT_HDR_SIZE      2 /* event=1, length=1 */
#define HCI_COMMAND_HDR_SIZE    3 /* opcode=2, length=1 */
#define HCI_ACL_HDR_SIZE        4 /* handle+flags=2, length=2 */
#define HCI_SCO_HDR_SIZE        3 /* handle=2, length=1 */

#define HCI_COMMAND_DATA_SIZE   255
#define HCI_EVENT_DATA_SIZE     255
#define HCI_ACL_DATA_SIZE       1000

#define MAX_HCI_COMMAND_SIZE    (HCI_COMMAND_HDR_SIZE + HCI_COMMAND_DATA_SIZE)
#define MAX_HCI_EVENT_SIZE      (HCI_EVENT_HDR_SIZE + HCI_EVENT_DATA_SIZE)
#define MAX_HCI_ACL_SIZE        (HCI_ACL_HDR_SIZE + HCI_ACL_DATA_SIZE)

/* Macros to avoid too much versioning inside code */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)
#define URB_ALLOC(no, mf) usb_alloc_urb((no), (mf))
#define URB_SUBMIT(urb, mf) usb_submit_urb((urb), (mf))
#else
#define URB_ALLOC(no, mf) usb_alloc_urb((no))
#define URB_SUBMIT(urb, mf) usb_submit_urb((urb))
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 6)
#define URB_UNLINK(urb) usb_kill_urb((urb));
#else
#define URB_UNLINK(urb) usb_unlink_urb((urb));
#endif

#ifdef DEBUG_ENABLE
#define DBG_PRINT(fmt, args ...) printk(PRNPREFIX fmt, ## args)
#else
#define DBG_PRINT(fmt, args ...)
#endif
#ifdef DEBUG_VERBOSE
#define DBG_VERBOSE(fmt, args ...) printk(PRNPREFIX fmt, ## args)
#else
#define DBG_VERBOSE(fmt, args ...)
#endif

/* Queueing of BULK and ISOC URBs is legal, hence we allow multiple
 * outstanding receive URBs */
#define MAX_RX_BULK_URBS      1
#define MAX_RX_ISOC_URBS      6

/* Flags */
#define LISTEN_STARTED        1
#define DEVICE_CONNECTED      2
#define R_THREAD_RUNNING      3

/* Endpoint availability flags */
#define BULK_IN_READY         1
#define BULK_OUT_READY        2
#define INTR_IN_READY         3
#define ISOC_OUT_READY        4
#define ISOC_IN_READY         5
#define EXTRA_INTERFACE_READY 6
#define DFU_READY             7

/* DFU protocol identifiers */
#define DFU_IFACE_CLASS       0xFE
#define DFU_IFACE_SUB_CLASS   0x01
#define DFU_IFACE_PROTOCOL    0x00

/* Indices for tx barrier (use with old kernels) */
#define IDX_CONTROL           0
#define IDX_BULK              1
#define IDX_ISOC              2

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 4, 20)
#define SETUP_PKT_T struct usb_ctrlrequest
#else
#define SETUP_PKT_T devrequest
#endif

/* Queue element for packets waiting to be consumed by the stack */
struct usb_qe
{
    CsrUint8          chan;            /* BCSP channel id */
    MessageStructure *msg;            /* BCSP data structure */
    struct usb_qe    *next;           /* Pointer to next element */
};

/* Placeholder for the queue and locking */
typedef struct
{
    struct usb_qe    *head;
    struct usb_qe    *tail;
    atomic_t          count;
    spinlock_t        lock;
    wait_queue_head_t wait;
} usb_queue_t;

/* Structure to hold the run-time device specific information */
typedef struct
{
    /* General driver settings */
    struct usb_device *dev;
    int                minor;
    struct completion  reader_thread_exit;
    struct semaphore   devlock;
    void              *extraInterface;
    unsigned long      endpoint_present;
    atomic_t           await_bg_int;
    unsigned long      flags;
    usb_queue_t        queue;
    int                dfu_iface;
    int                dfu_detach_to;
    unsigned char      dfu_status;
    unsigned char      dfu_state;
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 4, 20)
    struct semaphore barrier[3];
#endif

    /* CTRL-in-out endpoint (hci) */
    CsrUint8    ctrl_ep;
    SETUP_PKT_T ctrl_setup;

    /* INTR-in endpoint (hci) */
    CsrUint8    intr_ep;
    CsrUint8    intr_interval;
    CsrUint16   intr_size;
    struct urb *intr_urb;
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 0)
    CsrUint8 *intr_reassembly;
    CsrUint16 intr_remain;
    CsrUint16 intr_total;
#endif

    /* BULK-in endpoint (acl) */
    CsrUint8    bulk_in_ep;
    struct urb *bulk_urb[MAX_RX_BULK_URBS];

    /* BULK-out endpoint (acl) */
    CsrUint8 bulk_out_ep;

    /* ISOC-in endpont (sco) */
    CsrUint8    isoc_in_ep;
    CsrUint16   isoc_in_size;
    CsrUint8    isoc_in_interval;
    CsrUint8   *isoc_reassembly;
    CsrUint8    isoc_remain;
    CsrUint8    isoc_total;
    struct urb *isoc_urb[MAX_RX_ISOC_URBS];

    /* ISOC-out endpont (sco) */
    CsrUint8  isoc_out_ep;
    CsrUint16 isoc_out_size;
    CsrUint8  isoc_out_interval;
} csr_dev_t;

/* Function prototypes */
void CsrUsbDrvRx(void *);

int readerThread(void *parm);
CsrInt16 usbTxCtrl(csr_dev_t *dv, void *data, CsrUint16 length);
CsrInt16 usbTxBulk(csr_dev_t *dv, void *data, CsrUint16 length);
CsrInt16 usbTxIsoc(csr_dev_t *dv, void *data, CsrUint16 length);
void usbCleanUrbs(csr_dev_t *dv);

void QueueAppend(csr_dev_t *dv, CsrUint8 chan, void *data, CsrUint16 len, CsrBool copy);
struct usb_qe *QueuePop(csr_dev_t *dv);
void QueuePrepare(csr_dev_t *dv);
void QueueFree(csr_dev_t *dv);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)
int csrUsbProbe(struct usb_interface *intf,
    const struct usb_device_id *id);
void csrUsbDisconnect(struct usb_interface *intf);
#else
void *csrUsbProbe(struct usb_device *uDev,
    CsrUint32 ifnum,
    const struct usb_device_id *id);
void csrUsbDisconnect(struct usb_device *dev, void *p);
#endif

/* Minimalistic DFU support */
struct dfu_status
{
    __u8 bStatus;
    __u8 bwPollTimeout[3];
    __u8 bState;
    __u8 iString;
} __attribute__ ((packed));
#define DFU_DETACH      0x00
#define DFU_GET_STATUS  0x03
#define DFU_CLR_STATUS  0x04
#define DFU_APP_IDLE    0x00
#define DFU_APP_DETACH  0x01
#define DFU_ERROR       0x0A
#define DFU_DETACH_TO   0x1388

#ifdef __cplusplus
}
#endif

#endif
