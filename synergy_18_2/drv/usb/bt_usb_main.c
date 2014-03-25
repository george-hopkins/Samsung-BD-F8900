/******************************************************************************

                 (c) CSR plc 2008

                 All rights reserved

FILE:            bt_usb_main.c

DESCRIPTION:     USB character device node driver

REVISION:        $Revision: #25 $

******************************************************************************/

#include <linux/kernel.h>
#include <linux/version.h>

#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 33)
#include <linux/autoconf.h>
#else
#include <generated/autoconf.h>
#endif

#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/unistd.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/interrupt.h>
#include <linux/ptrace.h>
#include <linux/poll.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/delay.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,24)
#include <linux/semaphore.h>
#else
#include <asm/semaphore.h>
#endif

#include <linux/list.h>
#include <linux/uio.h>
#include <linux/time.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <asm/bitops.h>

#include "include/linux_usb_com.h"
#include "include/linux_usb_dev.h"
#include "include/linux_usb_ioctl.h"
#include "include/sched/coal_sched.h"
#include "include/sched/pmalloc.h"
#include "include/hci_hc_com.h"
#include "include/usb_com.h"
#include "include/linux_usb_extra.h"


void *zpmalloc(size_t size)
{
    return memset(pmalloc(size), 0x00, size);
}

void pfree(void *ptr)
{
    kfree(ptr);
}

void *pmalloc(size_t size)
{
    return kmalloc(size, GFP_ATOMIC | GFP_DMA);
}

typedef struct read_q_elm_t
{
    uint8_t channel;
    MessageStructure *msg;
    struct read_q_elm_t *next;
} read_q_elm_t;

typedef struct
{
    wait_queue_head_t read_q;
    read_q_elm_t *first;
    read_q_elm_t *last;
    uint16_t count;
    struct semaphore access_sem;
    uint8_t minor;
} bt_usb_instance_t;


#define BT_USB_MAJOR 227

static int bt_usb_major = BT_USB_MAJOR;
static bt_usb_instance_t *instances[BT_USB_COUNT];
static struct semaphore inst_sem;
extern csr_usb_settings_t csr_setup;

#ifdef BT_USB_DEBUG
static void dumpData(uint8_t *data, uint16_t len)
{
    char buffer[1000];
    int idx;
    int i;

    idx = 0;

    for(i=0;i<len;i++)
    {
        idx+=sprintf(&buffer[idx],"0x%02x ",data[i]);

        if (((i+1)%16) == 0)
        {
            printk("%s\n",buffer);
            idx=0;
        }
    }

    if (idx)
    {
        printk("%s\n",buffer);
    }
}
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,19)
static ssize_t bt_usb_readv(struct kiocb *kiocbp, const struct iovec *io,
                            unsigned long count, loff_t f_pos)
#else
static ssize_t bt_usb_readv(struct file *filp, const struct iovec *io ,
                            unsigned long count, loff_t *f_pos)
#endif
{
    bt_usb_instance_t *inst;
    unsigned int i;
    ssize_t len;
    ssize_t no;
    read_q_elm_t *ptr;

    inst = (bt_usb_instance_t *)kiocbp->ki_filp->private_data;

    for (i = 0, len = 0; i < count; i++)
    {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,11)
        if (!access_ok(VERIFY_WRITE, io[i].iov_base, io[i].iov_len))
#else
        if (verify_area(VERIFY_WRITE, io[i].iov_base, io[i].iov_len))
#endif
        {
            return -EFAULT;
        }

        len += io[i].iov_len;
    }

    /* Grab lock and wait for data in queue */
    if (down_interruptible(&inst->access_sem))
    {
        return(-ERESTARTSYS);
    }

    while (inst->first == NULL)
    {
#ifdef BT_USB_DEBUG
        printk("<1>Read queue empty,  count = %d\n",
               inst->count);
#endif
        up(&inst->access_sem);

        if (wait_event_interruptible(inst->read_q,
                                     (inst->first != NULL)))
        {
            return(-ERESTARTSYS);
        }

        if (down_interruptible(&inst->access_sem))
        {
            return(-ERESTARTSYS);
        }
    }

    switch (count)
    {
        case 1: /* Getting size of next message */
            ptr = inst->first;

            no = min(io[0].iov_len, sizeof(ptr->msg->buflen));

            copy_to_user(io[0].iov_base, &ptr->msg->buflen, no);
        break;

        case 2: /* Getting actual message (pop off queue) */
            ptr = inst->first;

            inst->first = ptr->next;

            if (inst->last == ptr)
            {
                inst->last = NULL;
            }
            inst->count--;

            no = copy_to_user(io[0].iov_base, &ptr->channel,
                min(io[0].iov_len, sizeof(ptr->channel)));
            if (no == 0)
            {
                no = copy_to_user(io[1].iov_base, ptr->msg->buf,
                    min(io[1].iov_len, ptr->msg->buflen));
            }
            if (no == 0)
            {
#ifdef BT_USB_DEBUG
                printk("Channel=%d\n", ptr->channel);
                dumpData((uint8_t*)ptr->msg->buf, ptr->msg->buflen);
#endif

                no = min(io[0].iov_len, sizeof(ptr->channel)) +
                    min(io[1].iov_len, ptr->msg->buflen);
            }

            pfree(ptr->msg->buf);
            pfree(ptr->msg);
            pfree(ptr);

        break;
        default:
            no = -EINVAL;
        break;
    }

    up(&inst->access_sem);

#ifdef BT_USB_DEBUG
    printk("<1>BT_USB: readv: no = %d\n", no);
#endif
    return (no);
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,19)
static ssize_t bt_usb_writev(struct kiocb *kiocbp, const struct iovec *io,
                             unsigned long count,loff_t f_pos)
#else
static ssize_t bt_usb_writev(struct file *filp, const struct iovec *io,
                             unsigned long count,loff_t *f_pos)
#endif
{
    bt_usb_instance_t *inst;
    int i;
    uint8_t channel;
    ssize_t len=0;
    unsigned char *buf;
    uint16_t length;

#ifdef BT_USB_DEBUG
    printk(KERN_ALERT "BT_USB: writev, count = %ld\n",count);
#endif

    inst = (bt_usb_instance_t *)kiocbp->ki_filp->private_data;

    for (i = 0, len = 0; i < count; i++)
    {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,11)
        if (!access_ok(VERIFY_READ, io[i].iov_base, io[i].iov_len))
#else
        if (verify_area(VERIFY_READ, io[i].iov_base, io[i].iov_len))
#endif
        {
            return -EFAULT;
        }

        len+=io[i].iov_len;
    }

    /* Things can go wrong when getting data from userspace */
    i = copy_from_user(&channel,io[0].iov_base,io[0].iov_len);
    if(i == 0)
    {
        length = io[1].iov_len;
        buf = pmalloc(io[1].iov_len);
        i = copy_from_user(buf,io[1].iov_base,io[1].iov_len);

        if(i == 0)
        {
#ifdef BT_USB_DEBUG
            printk("Channel=%d\n",channel);
            dumpData(buf,length);
#endif
#ifndef CSR_BLUECORE_USB_IGNORE_WRITE_FAIL // Sean added for USB driver check
            if(!UsbDev_Tx(inst->minor, channel, buf, length))
            {
                return -EFAULT;
            }
#else // original
            UsbDev_Tx(inst->minor, channel, buf, length);
#endif
            i = len;
        }
    }

    return i;
}

static int bt_usb_open(struct inode *inode, struct file *filp)
{
    bt_usb_instance_t *inst;
    int minor;

    minor = MINOR(inode->i_rdev);

#ifdef BT_USB_DEBUG
    printk(KERN_ALERT "bt_usb%u: Open called\n", minor);
#endif

    /* Is device minor within range? */
    if (minor >= BT_USB_COUNT)
    {
        return -ENXIO;
    }

    down(&inst_sem);

    if (instances[minor] != NULL)
    {
        up(&inst_sem);
        return -EBUSY;
    }

    inst = instances[minor] = zpmalloc(sizeof(*inst));
    up(&inst_sem);

    inst->minor = minor;
    inst->first = NULL;
    inst->last = NULL;
    inst->count = 0;
    init_waitqueue_head(&inst->read_q);
    sema_init(&inst->access_sem, 1);

    filp->private_data = inst;

#ifdef CONFIG_USB_SUSPEND
	UsbDev_PmDisable(minor);
#endif

    return 0;
}

static int bt_usb_release(struct inode *inode, struct file *filp)
{
    bt_usb_instance_t *inst;
    read_q_elm_t *ptr;
    read_q_elm_t *next;

    inst = (bt_usb_instance_t *)filp->private_data;

    /*
     * Mark device as dead so no more
     * messages are added to the queue.
     */
    down(&inst_sem);
    instances[inst->minor] = NULL;
    up(&inst_sem);

#ifdef BT_USB_DEBUG
    printk(KERN_ALERT "bt_usb%u: release called\n", inst->minor);
#endif

    down(&inst->access_sem);

    ptr = inst->first;
    while(ptr)
    {
        next = ptr->next;

        pfree(ptr->msg->buf);
        pfree(ptr->msg);
        pfree(ptr);

        ptr = next;
    }

#ifdef CONFIG_USB_SUSPEND
/*	UsbDev_PmEnable(inst->minor); */
#endif

    pfree(inst);
    up(&inst->access_sem);

    return 0;
}

static int bt_usb_ioctl(struct inode *inode, struct file *filp,
                        unsigned int cmd, unsigned long arg)

{
    int ret;
	bt_usb_instance_t *inst;
	inst = (bt_usb_instance_t *)filp->private_data;

#ifdef BT_USB_DEBUG
    printk(KERN_ALERT "BT_USB: IOCTL called\n");
#endif

    /* Check command */
    if((_IOC_TYPE(cmd) != BT_USB_IOC_MAGIC) ||
       (_IOC_NR(cmd) > BT_USB_IOC_MAX_NO))
    {
        return -ENOTTY;
    }

    /* Handle command */
    ret = -ENOTTY;
    switch(cmd)
    {
        case BT_USB_IOC_ENTER_DFU:
            {
                ret = UsbDrv_EnterDFU();
				break;
            }

        case BT_USB_IOC_RESET:
            {
                ret = 0;
                UsbDrv_Reset();
				break;
            }

#ifdef CONFIG_USB_SUSPEND
		case BT_USB_IOC_ENABLE_SUSPEND:
			{
				ret = 0;
				UsbDev_PmEnable(inst->minor);
				break;
			}

		case BT_USB_IOC_DISABLE_SUSPEND:
			{
				ret = 0;
				UsbDev_PmDisable(inst->minor);
				break;
			}
#endif

        default:
            {
                printk(KERN_ALERT "BT_USB: Unknown IOCTL (0x%x)\n", cmd);
                break;
            }
    }

    return ret;
}

static struct file_operations bt_usb_fops =
{
    owner     : THIS_MODULE,
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 35)
    ioctl     : bt_usb_ioctl,
#else
    unlocked_ioctl     : bt_usb_ioctl,
#endif		

    open      : bt_usb_open,
    release   : bt_usb_release,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,19)
    aio_read  : bt_usb_readv,
    aio_write : bt_usb_writev,
#else
    writev    : bt_usb_writev,
    readv     : bt_usb_readv,
#endif
};

/* Fake scheduler entry */
void csr_task_init(void *data)
{
    /* No tasks! */
}

void hcCom_ReceiveMsg(void *msg, uint8_t bcspChannel, uint8_t rel)
{
    bt_usb_instance_t *inst;
    read_q_elm_t *p;

    down(&inst_sem);
    inst = instances[rel];
    
    if (inst == NULL)
    {
        /* No takers, just toss it. */
        up(&inst_sem);

        pfree(((MessageStructure *)msg)->buf);
        pfree(msg);

        return;
    }

    p = pmalloc(sizeof(read_q_elm_t));
    p->channel = bcspChannel;
    p->msg = msg;

#ifdef BT_USB_DEBUG
    printk(KERN_ALERT "bt_usb%u: Received %i bytes for channel %i\n",
           rel, p->msg->buflen, p->channel);
    dumpData((uint8_t*)p->msg->buf, p->msg->buflen);
#endif

    p->next = NULL;

    if (down_interruptible(&inst->access_sem))
    {
        pfree(p->msg->buf);
        pfree(p->msg);
        pfree(p);
    }
    else
    {
        if (inst->first == NULL)
        {
            inst->count++;
            inst->first = p;
            inst->last = p;
        }
        else
        {
            inst->count++;
            inst->last->next = p;
            inst->last = p;
        }
        wake_up_interruptible(&inst->read_q);

        up(&inst->access_sem);
    }

    up(&inst_sem);
}

static int __init bt_usb_init(void)
{
    int result;

    csr_setup.running_with_scheduler = FALSE;

    printk("BT_USB: initializing\n");
	
#ifndef init_MUTEX_LOCKED
	sema_init(&inst_sem,0);
#else
	init_MUTEX_LOCKED(&inst_sem);
#endif

    memset(instances, 0, sizeof(*instances));

    result = UsbDrv_Start(NULL);
    if (result != TRUE)
    {
        printk(KERN_ERR "BT_USB: USB initialization failed, exiting.\n");
        return result;
    }

    result = register_chrdev(bt_usb_major, "bt_usb", &bt_usb_fops);
    if (result < 0)
    {
        printk(KERN_ERR "BT_USB: can't get major %d\n", bt_usb_major);
        return result;
    }

    if (bt_usb_major == 0)
    {
        bt_usb_major = result;
    }

    up(&inst_sem);

    return (0);
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0)
#else
#undef CONFIG_MODULE_UNLOAD
#define CONFIG_MODULE_UNLOAD
#endif

#ifdef CONFIG_MODULE_UNLOAD
static void __exit bt_usb_exit(void)
{
    unregister_chrdev(bt_usb_major,"bt_usb");

    UsbDrv_Stop();

    printk("BlueCore transport stopped.\n");
}
module_exit(bt_usb_exit);
#endif

module_init(bt_usb_init);
MODULE_LICENSE("GPL");
