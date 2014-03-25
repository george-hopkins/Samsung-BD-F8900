#ifndef LINUX_USB_COM_H__
#define LINUX_USB_COM_H__
/*****************************************************

              (c) CSR plc 2008

              All rights reserved

FILE:         linux_usb_com.h

DESCRIPTION:  Implementation of a USB communication module
              for the kernel of the Linux platform

REVISION:     $Revision: #18 $

******************************************************/

#ifndef __KERNEL__
#error This file can only be used for Linux kernel builds
#endif

#include <linux/usb.h>
#include <linux/spinlock.h>
#include <linux/version.h>
#include "sched/csr_types.h"
#include "hci_hc_com.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Sunny@CSR : 2011.09.27 debug usb deriver for deadlock during BT sched init withoug USB module(BC8520) */
/* #define DEBUG_ENABLE */
/* #define DEBUG_VERBOSE */

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
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0)
#define URB_ALLOC(no, mf) usb_alloc_urb((no), (mf))
#define URB_SUBMIT(urb, mf) usb_submit_urb((urb), (mf))
#else
#define URB_ALLOC(no, mf) usb_alloc_urb((no))
#define URB_SUBMIT(urb, mf) usb_submit_urb((urb))
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,6)
#define URB_UNLINK(urb) usb_kill_urb((urb));
#else
#define URB_UNLINK(urb) usb_unlink_urb((urb));
#endif

#ifdef DEBUG_ENABLE
#define DBG_PRINT(fmt, args...) printk(PRNPREFIX fmt, ## args)
#else
#define DBG_PRINT(fmt, args...)
#endif
#ifdef DEBUG_VERBOSE
#define DBG_VERBOSE(fmt, args...) printk(PRNPREFIX fmt, ## args)
#else
#define DBG_VERBOSE(fmt, args...)
#endif

/* Queueing of BULK and ISOC URBs is legal, hence we allow multiple
 * outstanding receive URBs */
#define MAX_RX_BULK_URBS      1
#define MAX_RX_ISOC_URBS      6

/* Flags */
#define LISTEN_STARTED        1
#define DEVICE_CONNECTED      2
#define R_THREAD_RUNNING      3
#define DEVICE_SUSPENDED      4

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

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,4,20)
#define SETUP_PKT_T struct usb_ctrlrequest
#else
#define SETUP_PKT_T devrequest
#endif

/* Queue element for packets waiting to be consumed by the stack */
struct usb_qe
{
    uint8_t                    chan;  /* BCSP channel id */
    MessageStructure           *msg;  /* BCSP data structure */
    struct usb_qe              *next; /* Pointer to next element */
};

/* Placeholder for the queue and locking */
typedef struct
{
    struct usb_qe              *head;
    struct usb_qe              *tail;
    atomic_t                   count;
    spinlock_t                 lock;
    wait_queue_head_t          wait;
} usb_queue_t;

/* Structure to hold the run-time device specific information */
typedef struct
{
    /* General driver settings */
    struct usb_device            *dev;
    int                          minor;
    struct completion            reader_thread_exit;
    struct semaphore             devlock;
    void                         *extraInterface;
    unsigned long                endpoint_present;
    atomic_t                     await_bg_int;
    unsigned long                flags;
    usb_queue_t                  queue;
    int                          dfu_iface;
    int                          dfu_detach_to;
    unsigned char                dfu_status;
    unsigned char                dfu_state;
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,4,20)
    struct semaphore             barrier[3];
#endif

    /* CTRL-in-out endpoint (hci) */
    uint8_t                      ctrl_ep;
    SETUP_PKT_T                  ctrl_setup;

    /* INTR-in endpoint (hci) */
    uint8_t                      intr_ep;
    uint8_t                      intr_interval;
    uint16_t                     intr_size;
    struct urb                   *intr_urb;
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
    uint8_t                      *intr_reassembly;
    uint16_t                     intr_remain;
    uint16_t                     intr_total;
#endif

    /* BULK-in endpoint (acl) */
    uint8_t                      bulk_in_ep;
    struct urb                   *bulk_urb[MAX_RX_BULK_URBS];

    /* BULK-out endpoint (acl) */
    uint8_t                      bulk_out_ep;

    /* ISOC-in endpont (sco) */
    uint8_t                      isoc_in_ep;
    uint16_t                     isoc_in_size;
    uint8_t                      isoc_in_interval;
    uint8_t                      *isoc_reassembly;
    uint8_t                      isoc_remain;
    uint8_t                      isoc_total;
    struct urb                   *isoc_urb[MAX_RX_ISOC_URBS];

    /* ISOC-out endpont (sco) */
    uint8_t                      isoc_out_ep;
    uint16_t                     isoc_out_size;
    uint8_t                      isoc_out_interval;
} csr_dev_t;

/* Function prototypes */
void UsbDrv_Rx(void);

int readerThread(void *parm);
int16_t usbTxCtrl(csr_dev_t *dv, void *data, uint16_t length);
int16_t usbTxBulk(csr_dev_t *dv, void *data, uint16_t length);
int16_t usbTxIsoc(csr_dev_t *dv, void *data, uint16_t length);
void usbCleanUrbs(csr_dev_t *dv);

void QueueAppend(csr_dev_t *dv, uint8_t chan, void *data, uint16_t len, bool_t copy);
struct usb_qe *QueuePop(csr_dev_t *dv);
void QueuePrepare(csr_dev_t *dv);
void QueueFree(csr_dev_t *dv);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0)
int csrUsbProbe(struct usb_interface *intf,
                       const struct usb_device_id *id);
void csrUsbDisconnect(struct usb_interface *intf);
#else
void *csrUsbProbe(struct usb_device *uDev,
                         unsigned int ifnum,
                         const struct usb_device_id *id);
void csrUsbDisconnect(struct usb_device *dev, void *p);
#endif

#ifdef CONFIG_USB_SUSPEND
int csrUsbSuspend(struct usb_interface *intf, pm_message_t state);
int csrUsbResume(struct usb_interface *intf);
int csrUsbResetResume(struct usb_interface *intf);

void UsbDev_PmDisable(uint8_t devno);
void UsbDev_PmEnable(uint8_t devno);
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
