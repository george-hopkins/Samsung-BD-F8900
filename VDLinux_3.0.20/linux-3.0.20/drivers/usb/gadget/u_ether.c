/*
 * u_ether.c -- Ethernet-over-USB link layer utilities for Gadget stack
 *
 * Copyright (C) 2003-2005,2008 David Brownell
 * Copyright (C) 2003-2004 Robert Schwebel, Benedikt Spranger
 * Copyright (C) 2008 Nokia Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* #define VERBOSE_DEBUG */

#include <linux/kernel.h>
#include <linux/gfp.h>
#include <linux/device.h>
#include <linux/ctype.h>
#include <linux/etherdevice.h>
#include <linux/ethtool.h>
#include <linux/ip.h>
#include <linux/icmp.h>

#include "u_ether.h"


//#ifdef CONFIG_SAMSUNG_PATCH_WITH_USB_GADGET_COMMON
//#define CONFIG_SAMSUNG_BUTTOM_HALF	1
//#endif	// CONFIG_SAMSUNG_PATCH_WITH_USB_GADGET_COMMON

/*
 * This component encapsulates the Ethernet link glue needed to provide
 * one (!) network link through the USB gadget stack, normally "usb0".
 *
 * The control and data models are handled by the function driver which
 * connects to this code; such as CDC Ethernet (ECM or EEM),
 * "CDC Subset", or RNDIS.  That includes all descriptor and endpoint
 * management.
 *
 * Link level addressing is handled by this component using module
 * parameters; if no such parameters are provided, random link level
 * addresses are used.  Each end of the link uses one address.  The
 * host end address is exported in various ways, and is often recorded
 * in configuration databases.
 *
 * The driver which assembles each configuration using such a link is
 * responsible for ensuring that each configuration includes at most one
 * instance of is network link.  (The network layer provides ways for
 * this single "physical" link to be used by multiple virtual links.)
 */

#define UETH__VERSION	"29-May-2008"

struct eth_dev {
	/* lock is held while accessing port_usb
	 * or updating its backlink port_usb->ioport
	 */
	spinlock_t		lock;
	struct gether		*port_usb;

	struct net_device	*net;
	struct usb_gadget	*gadget;
#ifdef H_D_THROUGHPUT_IMPROVEMENT
	spinlock_t		tx_req_lock;	/* guard {rx,tx}_reqs */
	spinlock_t		rx_req_lock;	/* guard {rx,tx}_reqs */
#else
	spinlock_t		req_lock;	/* guard {rx,tx}_reqs */
#endif
	struct list_head	tx_reqs, rx_reqs;
	atomic_t		tx_qlen;

	struct sk_buff_head	rx_frames;

#ifdef CONFIG_SAMSUNG_PATCH_WITH_USB_GADGET_COMMON
#ifdef CONFIG_SAMSUNG_BUTTOM_HALF
	struct sk_buff_head		rx_done;
	struct tasklet_struct	bh;
#endif
#ifdef CONFIG_SAMSUNG_CDC_EEM_DEBUG
	struct sk_buff_head	dbg_rx_queue;
	struct tasklet_struct	dbg_bh;
	unsigned int  		debug_packet_id;
	unsigned char		ack_received;
	wait_queue_head_t 	dbg_queue;
#endif
#endif	// CONFIG_SAMSUNG_PATCH_WITH_USB_GADGET_COMMON

	unsigned		header_len;
	struct sk_buff		*(*wrap)(struct gether *, struct sk_buff *skb);
	int			(*unwrap)(struct gether *,
						struct sk_buff *skb,
						struct sk_buff_head *list);

	struct work_struct	work;

	unsigned long		todo;
#define	WORK_RX_MEMORY		0

	bool			zlp;
	u8			host_mac[ETH_ALEN];
};

/*-------------------------------------------------------------------------*/

#define RX_EXTRA		30	/* bytes guarding against rx overflows */

#define DEFAULT_QLEN	2	/* double buffering by default */


#ifdef CONFIG_USB_GADGET_DUALSPEED

#ifdef CONFIG_SAMSUNG_PATCH_WITH_USB_GADGET_COMMON
static unsigned qmult = 10;
#else	// CONFIG_SAMSUNG_PATCH_WITH_USB_GADGET_COMMON
static unsigned qmult = 5;
#endif	// CONFIG_SAMSUNG_PATCH_WITH_USB_GADGET_COMMON

module_param(qmult, uint, S_IRUGO|S_IWUSR);
MODULE_PARM_DESC(qmult, "queue length multiplier at high speed");

#else	/* full speed (low speed doesn't do bulk) */
#define qmult		1
#endif

extern atomic_t			received_packets;
extern atomic_t			transfred_packets;
extern unsigned int		throughput_test;
extern unsigned int     ping_test;
extern unsigned long  	received_speed_packets;

/* for dual-speed hardware, use deeper queues at highspeed */
static inline int qlen(struct usb_gadget *gadget)
{
	if (gadget_is_dualspeed(gadget) && gadget->speed == USB_SPEED_HIGH)
		return qmult * DEFAULT_QLEN;
	else
		return DEFAULT_QLEN;
}

/*-------------------------------------------------------------------------*/

/* REVISIT there must be a better way than having two sets
 * of debug calls ...
 */

#undef DBG
#undef VDBG
#undef ERROR
#undef INFO

#define xprintk(d, level, fmt, args...) \
	printk(level "%s: " fmt , (d)->net->name , ## args)

#ifdef DEBUG
#undef DEBUG
#define DBG(dev, fmt, args...) \
	xprintk(dev , KERN_DEBUG , fmt , ## args)
#else
#define DBG(dev, fmt, args...) \
	do { } while (0)
#endif /* DEBUG */

#ifdef VERBOSE_DEBUG
#define VDBG	DBG
#else
#define VDBG(dev, fmt, args...) \
	do { } while (0)
#endif /* DEBUG */

#define ERROR(dev, fmt, args...) \
	xprintk(dev , KERN_ERR , fmt , ## args)
#define INFO(dev, fmt, args...) \
	xprintk(dev , KERN_INFO , fmt , ## args)

/*-------------------------------------------------------------------------*/

/* NETWORK DRIVER HOOKUP (to the layer above this driver) */

static int ueth_change_mtu(struct net_device *net, int new_mtu)
{
	struct eth_dev	*dev = netdev_priv(net);
	unsigned long	flags;
	int		status = 0;

	/* don't change MTU on "live" link (peer won't know) */
	spin_lock_irqsave(&dev->lock, flags);
	if (dev->port_usb)
		status = -EBUSY;
	else if (new_mtu <= ETH_HLEN || new_mtu > ETH_FRAME_LEN)
		status = -ERANGE;
	else
		net->mtu = new_mtu;
	spin_unlock_irqrestore(&dev->lock, flags);

	return status;
}

static void eth_get_drvinfo(struct net_device *net, struct ethtool_drvinfo *p)
{
	struct eth_dev	*dev = netdev_priv(net);

	strlcpy(p->driver, "g_ether", sizeof p->driver);
	strlcpy(p->version, UETH__VERSION, sizeof p->version);
	strlcpy(p->fw_version, dev->gadget->name, sizeof p->fw_version);
	strlcpy(p->bus_info, dev_name(&dev->gadget->dev), sizeof p->bus_info);
}

/* REVISIT can also support:
 *   - WOL (by tracking suspends and issuing remote wakeup)
 *   - msglevel (implies updated messaging)
 *   - ... probably more ethtool ops
 */

static const struct ethtool_ops ops = {
	.get_drvinfo = eth_get_drvinfo,
	.get_link = ethtool_op_get_link,
};

static void defer_kevent(struct eth_dev *dev, int flag)
{
	if (test_and_set_bit(flag, &dev->todo))
		return;
	if (!schedule_work(&dev->work))
		ERROR(dev, "kevent %d may have been dropped\n", flag);
	else
		DBG(dev, "kevent %d scheduled\n", flag);
}

static void rx_complete(struct usb_ep *ep, struct usb_request *req);

static int
rx_submit(struct eth_dev *dev, struct usb_request *req, gfp_t gfp_flags)
{
	struct sk_buff	*skb;
	int		retval = -ENOMEM;
	size_t		size = 0;
	struct usb_ep	*out;
	unsigned long	flags;

	spin_lock_irqsave(&dev->lock, flags);
	if (dev->port_usb)
		out = dev->port_usb->out_ep;
	else
		out = NULL;
	spin_unlock_irqrestore(&dev->lock, flags);

	if (!out)
		return -ENOTCONN;


	/* Padding up to RX_EXTRA handles minor disagreements with host.
	 * Normally we use the USB "terminate on short read" convention;
	 * so allow up to (N*maxpacket), since that memory is normally
	 * already allocated.  Some hardware doesn't deal well with short
	 * reads (e.g. DMA must be N*maxpacket), so for now don't trim a
	 * byte off the end (to force hardware errors on overflow).
	 *
	 * RNDIS uses internal framing, and explicitly allows senders to
	 * pad to end-of-packet.  That's potentially nice for speed, but
	 * means receivers can't recover lost synch on their own (because
	 * new packets don't only start after a short RX).
	 */
	size += sizeof(struct ethhdr) + dev->net->mtu + RX_EXTRA;
	size += dev->port_usb->header_len;
	size += out->maxpacket - 1;
	size -= size % out->maxpacket;

	if (dev->port_usb->is_fixed)
		size = max_t(size_t, size, dev->port_usb->fixed_out_len);

	skb = alloc_skb(size + NET_IP_ALIGN, gfp_flags);
	if (skb == NULL) {
		DBG(dev, "no rx skb\n");
		goto enomem;
	}

	/* Some platforms perform better when IP packets are aligned,
	 * but on at least one, checksumming fails otherwise.  Note:
	 * RNDIS headers involve variable numbers of LE32 values.
	 */
#ifdef CONFIG_SAMSUNG_PATCH_WITH_USB_GADGET_COMMON
	/*
	 * RX: Do not move data by IP_ALIGN:
	 * if your DMA controller cannot handle it
	 */
	if(!gadget_dma32(dev->gadget))
//		skb_reserve(skb, NET_IP_ALIGN);
#else	// CONFIG_SAMSUNG_PATCH_WITH_USB_GADGET_COMMON
	skb_reserve(skb, NET_IP_ALIGN);
#endif	// CONFIG_SAMSUNG_PATCH_WITH_USB_GADGET_COMMON

	req->buf = skb->data;
	req->length = size;
	req->complete = rx_complete;
	req->context = skb;

	retval = usb_ep_queue(out, req, gfp_flags);
	if (retval == -ENOMEM)
enomem:
		defer_kevent(dev, WORK_RX_MEMORY);
	if (retval) {
		DBG(dev, "rx submit --> %d\n", retval);
		if (skb)
			dev_kfree_skb_any(skb);
#ifdef  H_D_THROUGHPUT_IMPROVEMENT
		spin_lock_irqsave(&dev->rx_req_lock, flags);
		list_add(&req->list, &dev->rx_reqs);
		spin_unlock_irqrestore(&dev->rx_req_lock, flags);
#else
		spin_lock_irqsave(&dev->req_lock, flags);
		list_add(&req->list, &dev->rx_reqs);
		spin_unlock_irqrestore(&dev->req_lock, flags);
#endif
	}
	return retval;
}

#ifdef CONFIG_SAMSUNG_CDC_EEM_DEBUG
#define SAM_DEBUG_MAGIC_NUMBER_LEN 13
unsigned char 	*sam_magic_num  =  "#SISC#DEBUG#";

struct sam_debug_data {
	struct	sockaddr 	srcaddr;
	struct	sockaddr 	dstaddr;
	void*		 	data;
	int			data_size;
	unsigned int	 	time_out;
};

struct sam_debug_hdr {
	unsigned int  		packet_id;
	unsigned int  		packet_ack;
	unsigned long		start_sec;
	unsigned long		start_nsec;
	unsigned char  		magic_num[SAM_DEBUG_MAGIC_NUMBER_LEN+2];
};


static netdev_tx_t eth_start_xmit(struct sk_buff *skb,struct net_device *net);


static void dgb_defer_bh(unsigned long param)
{
   struct eth_dev 		*dev;
   struct iphdr			*ip4;
   struct sk_buff		*skb;
   struct sam_debug_hdr		dbg_hdr;
   __be32			tmp_addr;
   unsigned char		*data;


   dev = (struct eth_dev *)param;
   while((skb = skb_dequeue(&dev->dbg_rx_queue)))
   {
	unsigned char*	dpointer;

	// Get us the Packet ID...
	data = skb->data;
	memcpy(&dbg_hdr,data,sizeof(dbg_hdr));
	dbg_hdr.packet_ack =  1;

	// Change IP Address...
	skb_pull(skb,sizeof(dbg_hdr));
	ip4 = (struct iphdr*)skb->data;
   	tmp_addr   = ip4->saddr;
	ip4->saddr = ip4->daddr;
	ip4->daddr = tmp_addr;

	// Push Debug Header back..
	memcpy(dbg_hdr.magic_num,sam_magic_num,SAM_DEBUG_MAGIC_NUMBER_LEN);
	dpointer = skb_push(skb,sizeof(dbg_hdr));
	memset(dpointer,0,sizeof(dbg_hdr));
	memcpy(dpointer,&dbg_hdr,sizeof(dbg_hdr));

	if(eth_start_xmit(skb,dev->net) != NETDEV_TX_OK)
	   printk(KERN_DEBUG"Error Re-Transmitting");
   }
   return;
}

static void dgb_send_ack(struct eth_dev  *dev, struct sk_buff	*skb)
{
	struct sam_debug_hdr  		dbg_hdr;
    	unsigned int			result;
    	unsigned int			temp;
	unsigned char*			dpointer;
	unsigned char		  	*data;
	
	// Get us the Packet ID...
	data = skb->data;
	memcpy(&dbg_hdr,data,sizeof(dbg_hdr));
	memcpy(dbg_hdr.magic_num,sam_magic_num,SAM_DEBUG_MAGIC_NUMBER_LEN);
	dbg_hdr.packet_ack =  1;
	kfree_skb(skb);

	//Allocating New SKB
        skb = dev_alloc_skb(66 + 64);
        if(skb == NULL)
	{
	  printk(KERN_DEBUG"Error Allocating SKB");
	  return;
	}
        skb->dev = dev;

        // DMA Aligning
        result = sizeof(dbg_hdr) % 16;
        if(result)
      	  skb_reserve(skb,result);
        skb_reserve(skb, NET_IP_ALIGN);

        skb->data = skb_put(skb,(66-sizeof(dbg_hdr))); // increments all pointer or adds data
        if(skb->data == NULL)
        {
	    kfree_skb(skb);
	    return;
        }
        dpointer = skb_push(skb,sizeof(dbg_hdr));
        memset(dpointer,0,sizeof(dbg_hdr));
        memcpy(dpointer,&dbg_hdr,sizeof(dbg_hdr));

        result = eth_start_xmit(skb,dev->net);
        if(result != NETDEV_TX_OK)
        {
           kfree_skb(skb);
           printk(KERN_DEBUG"Error Re-Transmitting");
        }
	return;
}

static void defer_debug_bh(struct eth_dev *dev, struct sk_buff *skb)
{
	unsigned long flags;

	spin_lock_irqsave(&dev->dbg_rx_queue.lock, flags);
	__skb_queue_tail(&dev->dbg_rx_queue, skb);
	if(dev->dbg_rx_queue.qlen == 1)
		tasklet_schedule(&dev->dbg_bh);
	spin_unlock_irqrestore(&dev->dbg_rx_queue.lock, flags);
}
#endif

#ifdef CONFIG_SAMSUNG_PATCH_WITH_USB_GADGET_COMMON
#ifdef CONFIG_SAMSUNG_BUTTOM_HALF
static void uether_bh(unsigned long param)
{
	struct eth_dev	*dev = (struct eth_dev *) param;
	struct sk_buff	*skb, *skb2;
	int				status = 0;
	struct timespec	scan_time;
#ifdef H_D_THROUGHPUT_IMPROVEMENT 
	while((skb2 = skb_dequeue(&dev->rx_done)))
	{
		if(skb2)
#else
	while((skb = skb_dequeue(&dev->rx_done)))
	{
		if(dev->unwrap)
		{
			unsigned long flags;

			spin_lock_irqsave(&dev->lock, flags);
			if(dev->port_usb)
				status = dev->unwrap(dev->port_usb,skb,&dev->rx_frames);
			else
			{
				dev_kfree_skb_any(skb);
				status = -ENOTCONN;
			}
			spin_unlock_irqrestore(&dev->lock, flags);
		}
		else
			skb_queue_tail(&dev->rx_frames, skb);

		skb = NULL;
		skb2 = skb_dequeue(&dev->rx_frames);
		while(skb2)
#endif
		{
#ifdef CONFIG_SAMSUNG_CDC_EEM_DEBUG
			// Debug Header processing here...
			struct sam_debug_hdr	*ptr_dbg_hdr;

			ptr_dbg_hdr = (struct sam_debug_hdr *)skb2->data;
			if(strcmp(ptr_dbg_hdr->magic_num,sam_magic_num) == 0)
			{
			    getnstimeofday(&scan_time);
				if(ptr_dbg_hdr->packet_ack == 1)
				{
					// Set Packet Acknowledgement...
					// Write some code here.........
					dev->ack_received = 1;
					dev_kfree_skb_any(skb2);
					if(throughput_test == 1)
					{
						atomic_inc(&received_packets);
						received_speed_packets++;
					}
					else if(ping_test == 1)
					{
						wake_up_interruptible(&dev->dbg_queue);
						printk(KERN_ALERT"CDC_EEM Ping ACK Received (RTT : %lusec-%lunsec)\n",
                                                                 (scan_time.tv_sec - ptr_dbg_hdr->start_sec),
                                                                 (scan_time.tv_nsec-ptr_dbg_hdr->start_nsec));
					}
				}
				else
					dgb_send_ack(dev,skb2);
			}
			else
			{
				if(status < 0 || ETH_HLEN > skb2->len || skb2->len > ETH_FRAME_LEN)
				{
					dev->net->stats.rx_errors++;
					dev->net->stats.rx_length_errors++;
					DBG(dev, "rx length %d\n", skb2->len);
					dev_kfree_skb_any(skb2);
#ifdef H_D_THROUGHPUT_IMPROVEMENT 
					continue;
#else
					goto next_frame;
#endif
				}
				skb2->protocol = eth_type_trans(skb2, dev->net);
				dev->net->stats.rx_packets++;
				dev->net->stats.rx_bytes += skb2->len;

				/* no buffer copies needed, unless hardware can't
				 * use skb buffers.
				 */
				status = netif_rx(skb2);
			}
#else
			if(status < 0 || ETH_HLEN > skb2->len || skb2->len > ETH_FRAME_LEN)
			{
				dev->net->stats.rx_errors++;
				dev->net->stats.rx_length_errors++;
				DBG(dev, "rx length %d\n", skb2->len);
				dev_kfree_skb_any(skb2);
#ifdef H_D_THROUGHPUT_IMPROVEMENT 
				continue;
#else
					goto next_frame;
#endif
			}
			skb2->protocol = eth_type_trans(skb2, dev->net);
			dev->net->stats.rx_packets++;
			dev->net->stats.rx_bytes += skb2->len;

			/* no buffer copies needed, unless hardware can't
			 * use skb buffers.
			 */
			status = netif_rx(skb2);
#endif
#ifndef H_D_THROUGHPUT_IMPROVEMENT 
next_frame:
			skb2 = skb_dequeue(&dev->rx_frames);
#endif
		}
	}

	return;
}

static void defer_bh(struct eth_dev *dev, struct sk_buff *skb)
{
	unsigned long flags;

	spin_lock_irqsave(&dev->rx_done.lock, flags);
	__skb_queue_tail(&dev->rx_done, skb);
#ifdef H_D_THROUGHPUT_IMPROVEMENT
	if(dev->rx_done.qlen > 0)
#else
	if(dev->rx_done.qlen == 1)
#endif
		tasklet_schedule(&dev->bh);
	spin_unlock_irqrestore(&dev->rx_done.lock, flags);
}
#endif
#endif	// CONFIG_SAMSUNG_PATCH_WITH_USB_GADGET_COMMON

static void rx_complete(struct usb_ep *ep, struct usb_request *req)
{
	struct sk_buff	*skb = req->context, *skb2;
	struct eth_dev	*dev = ep->driver_data;
	int		status = req->status;
	struct timespec	scan_time;

	switch (status) {

	/* normal completion */
	case 0:
#ifdef CONFIG_SAMSUNG_PATCH_WITH_USB_GADGET_COMMON
#if CONFIG_SAMSUNG_BUTTOM_HALF
		if(!dev->port_usb->devnum)
		{
			skb_put(skb, req->actual);
			break;
		}
#endif
#endif	// CONFIG_SAMSUNG_PATCH_WITH_USB_GADGET_COMMON

		skb_put(skb, req->actual);
//		printk(KERN_DEBUG"SBK Len %d",skb->len);
		if (dev->unwrap) {
			unsigned long	flags;

			spin_lock_irqsave(&dev->lock, flags);
			if (dev->port_usb) {
				status = dev->unwrap(dev->port_usb,skb,&dev->rx_frames);
			} else {
				dev_kfree_skb_any(skb);
				status = -ENOTCONN;
			}
			spin_unlock_irqrestore(&dev->lock, flags);
		} else {
			skb_queue_tail(&dev->rx_frames, skb);
		}
		skb = NULL;


		skb2 = skb_dequeue(&dev->rx_frames);
		while (skb2) 
		{
#ifdef CONFIG_SAMSUNG_CDC_EEM_DEBUG
			// Debug Header processing here...
			struct sam_debug_hdr	*ptr_dbg_hdr;

			ptr_dbg_hdr = (struct sam_debug_hdr *)skb2->data;
			if(strcmp(ptr_dbg_hdr->magic_num,sam_magic_num) == 0)
			{
				if(ptr_dbg_hdr->packet_ack == 1)
				{
					// Set Packet Acknowledgement...
					// Write some code here.........
					dev->ack_received = 1;
					dev_kfree_skb_any(skb2);
					if(throughput_test == 1)
					{
						atomic_inc(&received_packets);
						received_speed_packets++;
					}
					else if(ping_test == 1)
					{
						getnstimeofday(&scan_time);
						wake_up_interruptible(&dev->dbg_queue);
						printk(KERN_ALERT"CDC_EEM Ping ACK Received (RTT : %lusec-%lunsec)\n",
							         (scan_time.tv_sec - ptr_dbg_hdr->start_sec),
								 (scan_time.tv_nsec-ptr_dbg_hdr->start_nsec));
					}
				}
				else
				 dgb_send_ack(dev,skb2);
			}
			else
			{
				if (status < 0	|| ETH_HLEN > skb2->len	|| skb2->len > ETH_FRAME_LEN)
				{
					dev->net->stats.rx_errors++;
					dev->net->stats.rx_length_errors++;
					DBG(dev, "rx length %d\n", skb2->len);
					dev_kfree_skb_any(skb2);
					goto next_frame;
				}
				skb2->protocol = eth_type_trans(skb2, dev->net);
				dev->net->stats.rx_packets++;
				dev->net->stats.rx_bytes += skb2->len;

				/* no buffer copies needed, unless hardware can't
				 * use skb buffers.
				 */
				status = netif_rx(skb2);
			}
#else
			if (status < 0	|| ETH_HLEN > skb2->len	|| skb2->len > ETH_FRAME_LEN)
			{
				dev->net->stats.rx_errors++;
				dev->net->stats.rx_length_errors++;
				DBG(dev, "rx length %d\n", skb2->len);
				dev_kfree_skb_any(skb2);
				goto next_frame;
			}
			skb2->protocol = eth_type_trans(skb2, dev->net);
			dev->net->stats.rx_packets++;
			dev->net->stats.rx_bytes += skb2->len;

			/* no buffer copies needed, unless hardware can't
			 * use skb buffers.
			 */
			status = netif_rx(skb2);
#endif

next_frame:
			skb2 = skb_dequeue(&dev->rx_frames);
		}
		break;

	/* software-driven interface shutdown */
	case -ECONNRESET:		/* unlink */
	case -ESHUTDOWN:		/* disconnect etc */
		VDBG(dev, "rx shutdown, code %d\n", status);
		goto quiesce;

	/* for hardware automagic (such as pxa) */
	case -ECONNABORTED:		/* endpoint reset */
		DBG(dev, "rx %s reset\n", ep->name);
		defer_kevent(dev, WORK_RX_MEMORY);
quiesce:
		dev_kfree_skb_any(skb);
		goto clean;

	/* data overrun */
	case -EOVERFLOW:
		dev->net->stats.rx_over_errors++;
		/* FALLTHROUGH */

	default:
		dev->net->stats.rx_errors++;
		DBG(dev, "rx status %d\n", status);
		break;
	}

#ifdef CONFIG_SAMSUNG_PATCH_WITH_USB_GADGET_COMMON
#if CONFIG_SAMSUNG_BUTTOM_HALF
	if(!dev->port_usb->devnum)
	{  
		defer_bh(dev,skb);
	}
	else
#endif
#endif	// CONFIG_SAMSUNG_PATCH_WITH_USB_GADGET_COMMON
	    if (skb)
		    dev_kfree_skb_any(skb);
    
	if (!netif_running(dev->net)) {
clean:
#ifdef H_D_THROUGHPUT_IMPROVEMENT
		spin_lock(&dev->rx_req_lock);
		list_add(&req->list, &dev->rx_reqs);
		spin_unlock(&dev->rx_req_lock);
#else
		spin_lock(&dev->req_lock);
		list_add(&req->list, &dev->rx_reqs);
		spin_unlock(&dev->req_lock);
#endif
		req = NULL;
	}
	if (req)
		rx_submit(dev, req, GFP_ATOMIC);
}

static int prealloc(struct list_head *list, struct usb_ep *ep, unsigned n)
{
	unsigned		i;
	struct usb_request	*req;

	if (!n)
		return -ENOMEM;

	/* queue/recycle up to N requests */
	i = n;
	list_for_each_entry(req, list, list) {
		if (i-- == 0)
			goto extra;
	}
	while (i--) {
		req = usb_ep_alloc_request(ep, GFP_ATOMIC);
		if (!req)
			return list_empty(list) ? -ENOMEM : 0;
		list_add(&req->list, list);
	}
	return 0;

extra:
	/* free extras */
	for (;;) {
		struct list_head	*next;

		next = req->list.next;
		list_del(&req->list);
		usb_ep_free_request(ep, req);

		if (next == list)
			break;

		req = container_of(next, struct usb_request, list);
	}
	return 0;
}

static int alloc_requests(struct eth_dev *dev, struct gether *link, unsigned n)
{
	int	status;

#ifdef H_D_THROUGHPUT_IMPROVEMENT
	spin_lock(&dev->tx_req_lock);
	status = prealloc(&dev->tx_reqs, link->in_ep, n);
	if (status < 0) {
		spin_unlock(&dev->tx_req_lock);
		DBG(dev, "can't alloc requests\n");
		goto fail;
	}
	spin_unlock(&dev->tx_req_lock);
	
	spin_lock(&dev->rx_req_lock);
	status = prealloc(&dev->rx_reqs, link->out_ep, n);
	if (status < 0) {
		spin_unlock(&dev->rx_req_lock);
		DBG(dev, "can't alloc requests\n");
		goto fail;
	}
	spin_unlock(&dev->rx_req_lock);

fail:
#else
	spin_lock(&dev->req_lock);
	status = prealloc(&dev->tx_reqs, link->in_ep, n);
	if (status < 0)
		goto fail;
	status = prealloc(&dev->rx_reqs, link->out_ep, n);
	if (status < 0)
		goto fail;
	goto done;
fail:
	DBG(dev, "can't alloc requests\n");
done:
	spin_unlock(&dev->req_lock);
#endif
	return status;
}

static void rx_fill(struct eth_dev *dev, gfp_t gfp_flags)
{
	struct usb_request	*req;
	unsigned long		flags;

	/* fill unused rxq slots with some skb */
#ifdef H_D_THROUGHPUT_IMPROVEMENT
	spin_lock_irqsave(&dev->rx_req_lock, flags);
	while (!list_empty(&dev->rx_reqs)) {
		req = container_of(dev->rx_reqs.next,
				struct usb_request, list);
		list_del_init(&req->list);
		spin_unlock_irqrestore(&dev->rx_req_lock, flags);

		if (rx_submit(dev, req, gfp_flags) < 0) {
			defer_kevent(dev, WORK_RX_MEMORY);
			return;
		}

		spin_lock_irqsave(&dev->rx_req_lock, flags);
	}
	spin_unlock_irqrestore(&dev->rx_req_lock, flags);
#else
	spin_lock_irqsave(&dev->req_lock, flags);
	while (!list_empty(&dev->rx_reqs)) {
		req = container_of(dev->rx_reqs.next,
				struct usb_request, list);
		list_del_init(&req->list);
		spin_unlock_irqrestore(&dev->req_lock, flags);

		if (rx_submit(dev, req, gfp_flags) < 0) {
			defer_kevent(dev, WORK_RX_MEMORY);
			return;
		}

		spin_lock_irqsave(&dev->req_lock, flags);
	}
	spin_unlock_irqrestore(&dev->req_lock, flags);
#endif
}

static void eth_work(struct work_struct *work)
{
	struct eth_dev	*dev = container_of(work, struct eth_dev, work);

	if (test_and_clear_bit(WORK_RX_MEMORY, &dev->todo)) {
		if (netif_running(dev->net))
			rx_fill(dev, GFP_KERNEL);
	}

	if (dev->todo)
		DBG(dev, "work done, flags = 0x%lx\n", dev->todo);
}

static void tx_complete(struct usb_ep *ep, struct usb_request *req)
{
	struct sk_buff	*skb = req->context;
	struct eth_dev	*dev = ep->driver_data;

	switch (req->status) {
	default:
		dev->net->stats.tx_errors++;
		VDBG(dev, "tx err %d\n", req->status);
		/* FALLTHROUGH */
	case -ECONNRESET:		/* unlink */
	case -ESHUTDOWN:		/* disconnect etc */
		break;
	case 0:
		dev->net->stats.tx_bytes += skb->len;
	}
	dev->net->stats.tx_packets++;
	if(throughput_test ==1)
	 atomic_inc(&transfred_packets);

#ifdef H_D_THROUGHPUT_IMPROVEMENT
	spin_lock(&dev->tx_req_lock);
	list_add(&req->list, &dev->tx_reqs);
	spin_unlock(&dev->tx_req_lock);
#else
	spin_lock(&dev->req_lock);
	list_add(&req->list, &dev->tx_reqs);
	spin_unlock(&dev->req_lock);
#endif
	dev_kfree_skb_any(skb);

	atomic_dec(&dev->tx_qlen);
	if (netif_carrier_ok(dev->net))
		netif_wake_queue(dev->net);
}

static inline int is_promisc(u16 cdc_filter)
{
	return cdc_filter & USB_CDC_PACKET_TYPE_PROMISCUOUS;
}

static netdev_tx_t eth_start_xmit(struct sk_buff *skb,
					struct net_device *net)
{
	struct eth_dev		*dev = netdev_priv(net);
	int			length = skb->len;
	int			retval;
	struct usb_request	*req = NULL;
	unsigned long		flags;
	struct usb_ep		*in;
	u16			cdc_filter;
#if 1
	if(!dev->port_usb->devnum && !is_enable_usb0) {
		netif_stop_queue(net);
		return NETDEV_TX_OK;
	}
	
	if(dev->port_usb->devnum && !is_enable_usb1) {
		netif_stop_queue(net);
		return NETDEV_TX_OK;
	}
#endif

	spin_lock_irqsave(&dev->lock, flags);
	if (dev->port_usb) {
		in = dev->port_usb->in_ep;
		cdc_filter = dev->port_usb->cdc_filter;
	} else {
		in = NULL;
		cdc_filter = 0;
	}
	spin_unlock_irqrestore(&dev->lock, flags);

	if (!in) {
		dev_kfree_skb_any(skb);
		return NETDEV_TX_OK;
	}

	/* apply outgoing CDC or RNDIS filters */
	if (!is_promisc(cdc_filter)) {
		u8		*dest = skb->data;

		if (is_multicast_ether_addr(dest)) {
			u16	type;

			/* ignores USB_CDC_PACKET_TYPE_MULTICAST and host
			 * SET_ETHERNET_MULTICAST_FILTERS requests
			 */
			if (is_broadcast_ether_addr(dest))
				type = USB_CDC_PACKET_TYPE_BROADCAST;
			else
				type = USB_CDC_PACKET_TYPE_ALL_MULTICAST;
			if (!(cdc_filter & type)) {
				dev_kfree_skb_any(skb);
				return NETDEV_TX_OK;
			}
		}
		/* ignores USB_CDC_PACKET_TYPE_DIRECTED */
	}

#ifdef H_D_THROUGHPUT_IMPROVEMENT
	spin_lock_irqsave(&dev->tx_req_lock, flags);
	/*
	 * this freelist can be empty if an interrupt triggered disconnect()
	 * and reconfigured the gadget (shutting down this queue) after the
	 * network stack decided to xmit but before we got the spinlock.
	 */
	if (list_empty(&dev->tx_reqs)) {
		spin_unlock_irqrestore(&dev->tx_req_lock, flags);
		return NETDEV_TX_BUSY;
	}

	req = container_of(dev->tx_reqs.next, struct usb_request, list);
	list_del(&req->list);

	/* temporarily stop TX queue when the freelist empties */
	if (list_empty(&dev->tx_reqs))
		netif_stop_queue(net);
	spin_unlock_irqrestore(&dev->tx_req_lock, flags);
#else
	spin_lock_irqsave(&dev->req_lock, flags);
	/*
	 * this freelist can be empty if an interrupt triggered disconnect()
	 * and reconfigured the gadget (shutting down this queue) after the
	 * network stack decided to xmit but before we got the spinlock.
	 */
	if (list_empty(&dev->tx_reqs)) {
		spin_unlock_irqrestore(&dev->req_lock, flags);
		return NETDEV_TX_BUSY;
	}

	req = container_of(dev->tx_reqs.next, struct usb_request, list);
	list_del(&req->list);

	/* temporarily stop TX queue when the freelist empties */
	if (list_empty(&dev->tx_reqs))
		netif_stop_queue(net);
	spin_unlock_irqrestore(&dev->req_lock, flags);
#endif

	/* no buffer copies needed, unless the network stack did it
	 * or the hardware can't use skb buffers.
	 * or there's not enough space for extra headers we need
	 */
	if (dev->wrap) {
		unsigned long	flags;

		spin_lock_irqsave(&dev->lock, flags);
		if (dev->port_usb)
			skb = dev->wrap(dev->port_usb, skb);
		spin_unlock_irqrestore(&dev->lock, flags);
		if (!skb)
			goto drop;

		length = skb->len;
	}
	req->buf = skb->data;
	req->context = skb;
	req->complete = tx_complete;
//	printk(KERN_DEBUG"<XMIT>SIZE OF SKB %d",skb->len);

	/* NCM requires no zlp if transfer is dwNtbInMaxSize */
	if (dev->port_usb->is_fixed &&
	    length == dev->port_usb->fixed_in_len &&
	    (length % in->maxpacket) == 0)
		req->zero = 0;
	else
		req->zero = 1;

	/* use zlp framing on tx for strict CDC-Ether conformance,
	 * though any robust network rx path ignores extra padding.
	 * and some hardware doesn't like to write zlps.
	 */
	if (req->zero && !dev->zlp && (length % in->maxpacket) == 0)
		length++;

	req->length = length;

	/* throttle highspeed IRQ rate back slightly */
	if (gadget_is_dualspeed(dev->gadget))
		req->no_interrupt = (dev->gadget->speed == USB_SPEED_HIGH)
			? ((atomic_read(&dev->tx_qlen) % qmult) != 0)
			: 0;

	retval = usb_ep_queue(in, req, GFP_ATOMIC);
	switch (retval) {
	default:
		DBG(dev, "tx queue err %d\n", retval);
		break;
	case 0:
		net->trans_start = jiffies;
		atomic_inc(&dev->tx_qlen);
	}

	if (retval) {
		dev_kfree_skb_any(skb);
drop:
		dev->net->stats.tx_dropped++;
#ifdef H_D_THROUGHPUT_IMPROVEMENT
		spin_lock_irqsave(&dev->tx_req_lock, flags);
		if (list_empty(&dev->tx_reqs))
			netif_start_queue(net);
		list_add(&req->list, &dev->tx_reqs);
		spin_unlock_irqrestore(&dev->tx_req_lock, flags);
#else
		spin_lock_irqsave(&dev->req_lock, flags);
		if (list_empty(&dev->tx_reqs))
			netif_start_queue(net);
		list_add(&req->list, &dev->tx_reqs);
		spin_unlock_irqrestore(&dev->req_lock, flags);
#endif
	}
	return NETDEV_TX_OK;
}

/*-------------------------------------------------------------------------*/

static void eth_start(struct eth_dev *dev, gfp_t gfp_flags)
{
	DBG(dev, "%s\n", __func__);

#ifdef CONFIG_SAMSUNG_PATCH_WITH_USB_GADGET_COMMON
#ifdef CONFIG_SAMSUNG_BUTTOM_HALF
	dev->bh.func = uether_bh;
	dev->bh.data = (unsigned long) dev;
#endif
#ifdef CONFIG_SAMSUNG_CDC_EEM_DEBUG
	dev->dbg_bh.func 	= dgb_defer_bh;
	dev->dbg_bh.data 	= (unsigned long)dev;
	dev->debug_packet_id 	= 0;
	init_waitqueue_head(&dev->dbg_queue);
#endif
#endif	// CONFIG_SAMSUNG_PATCH_WITH_USB_GADGET_COMMON

	/* fill the rx queue */
	rx_fill(dev, gfp_flags);

	/* and open the tx floodgates */
	atomic_set(&dev->tx_qlen, 0);
	netif_wake_queue(dev->net);
}

static int eth_open(struct net_device *net)
{
	struct eth_dev	*dev = netdev_priv(net);
	struct gether	*link;

	DBG(dev, "%s\n", __func__);
	if (netif_carrier_ok(dev->net))
		eth_start(dev, GFP_KERNEL);

	spin_lock_irq(&dev->lock);
	link = dev->port_usb;
	if (link && link->open)
		link->open(link);
	spin_unlock_irq(&dev->lock);

	return 0;
}

static int eth_stop(struct net_device *net)
{
	struct eth_dev	*dev = netdev_priv(net);
	unsigned long	flags;

	VDBG(dev, "%s\n", __func__);
	netif_stop_queue(net);

	DBG(dev, "stop stats: rx/tx %ld/%ld, errs %ld/%ld\n",
		dev->net->stats.rx_packets, dev->net->stats.tx_packets,
		dev->net->stats.rx_errors, dev->net->stats.tx_errors
		);

	/* ensure there are no more active requests */
	spin_lock_irqsave(&dev->lock, flags);
	if (dev->port_usb) {
		struct gether	*link = dev->port_usb;

		if (link->close)
			link->close(link);

		/* NOTE:  we have no abort-queue primitive we could use
		 * to cancel all pending I/O.  Instead, we disable then
		 * reenable the endpoints ... this idiom may leave toggle
		 * wrong, but that's a self-correcting error.
		 *
		 * REVISIT:  we *COULD* just let the transfers complete at
		 * their own pace; the network stack can handle old packets.
		 * For the moment we leave this here, since it works.
		 */
		usb_ep_disable(link->in_ep);
		usb_ep_disable(link->out_ep);
		if (netif_carrier_ok(net)) {
			DBG(dev, "host still using in/out endpoints\n");
			usb_ep_enable(link->in_ep, link->in);
			usb_ep_enable(link->out_ep, link->out);
		}
	}

#ifdef CONFIG_SAMSUNG_PATCH_WITH_USB_GADGET_COMMON
#ifdef CONFIG_SAMSUNG_BUTTOM_HALF
	tasklet_kill(&dev->bh);
#endif
#ifdef CONFIG_SAMSUNG_CDC_EEM_DEBUG
	tasklet_kill(&dev->dbg_bh);
#endif
#endif	// CONFIG_SAMSUNG_PATCH_WITH_USB_GADGET_COMMON

	spin_unlock_irqrestore(&dev->lock, flags);

	return 0;
}

#ifdef  CONFIG_SAMSUNG_CDC_EEM_DEBUG
#define DBG_DEFAULT_PACKET_DATA_SIZE	64

unsigned short in_cksum(unsigned short *addr, int len)
{
    register int sum = 0;
        u_short answer = 0;
        register u_short *w = addr;
        register int nleft = len;
        /*
        * Our algorithm is simple, using a 32 bit accumulator (sum), we add
        * sequential 16 bit words to it, and at the end, fold back all the
        * carry bits from the top 16 bits into the lower 16 bits.
        */
        while (nleft > 1)
        {
          sum += *w++;
          nleft -= 2;
        }
        /* mop up an odd byte, if necessary */
        if (nleft == 1)
        {
          *(u_char *) (&answer) = *(u_char *) w;
          sum += answer;
        }
        /* add back carry outs from top 16 bits to low 16 bits */
        sum = (sum >> 16) + (sum & 0xffff);     /* add hi 16 to low 16 */
        sum += (sum >> 16);             /* add carry */
        answer = ~sum;              /* truncate to 16 bits */
        return (answer);
}

int send_debug_packet(struct net_device *dev,struct sam_debug_data *dbg_data)
{

   struct iphdr			ip4;
   struct icmphdr		icmp;
   struct sk_buff*		skb;
   struct sam_debug_hdr dbg_hrd;
   struct eth_dev		*eem_dev = netdev_priv(dev);
   unsigned char		*dbg_pointer;
   unsigned int			len;
   int					result;
   struct timespec		scan_time;

      // Writing Magic number in Debug Header.
      memcpy(dbg_hrd.magic_num,sam_magic_num,SAM_DEBUG_MAGIC_NUMBER_LEN);
      
      //Creating New SKB...
      len = dbg_data->data_size;
      if(len < DBG_DEFAULT_PACKET_DATA_SIZE)
         len = DBG_DEFAULT_PACKET_DATA_SIZE;
      if(len > 1535)
      {
    	  printk(KERN_DEBUG"Data Size Exceeded, Setting Default Size %d",DBG_DEFAULT_PACKET_DATA_SIZE);
    	  len = DBG_DEFAULT_PACKET_DATA_SIZE;
      }
//    printk(KERN_DEBUG"SIZE OF HDR %d",sizeof(struct sam_debug_hdr));
      skb = dev_alloc_skb(len + sizeof(struct sam_debug_hdr) + 64);	//64 Byte for padding
      if(skb == NULL)
	printk(KERN_DEBUG"Error Allocating SKB");
      skb->dev 	= dev;

      // DMA Aligning
      result = (len + sizeof(struct sam_debug_hdr)) % 16;
      if(result)
      	 skb_reserve(skb, result);
      skb_reserve(skb, NET_IP_ALIGN);

      //Copying User Data in SKB...
      skb->data = skb_put(skb,len); // increments all pointer or adds data
      if(skb->data == NULL)
      {
	    printk(KERN_DEBUG"DATA:Packet size exceeds maximum limit\n");
	    kfree_skb(skb);
	    return -1;
      }
      if(dbg_data->data != NULL)
         copy_from_user(skb->data,dbg_data->data,dbg_data->data_size);

      //SISC Debuglayer Header...
      dbg_hrd.packet_id  = eem_dev->debug_packet_id++;
      dbg_hrd.packet_ack = 0;
      getnstimeofday(&scan_time);
      dbg_hrd.start_sec  = scan_time.tv_sec;
      dbg_hrd.start_nsec = scan_time.tv_nsec;
      dbg_pointer = skb_push(skb,sizeof(struct sam_debug_hdr));
      if(dbg_pointer == NULL)
      {
	    printk(KERN_DEBUG"DEBUG:Packet size exceeds maximum limit\n");
	    kfree_skb(skb);
	    return -1;
      }
      memset(dbg_pointer,0,sizeof(dbg_hrd));
      memcpy(dbg_pointer,&dbg_hrd,sizeof(dbg_hrd));

      //Transmitting Packet...
//    printk(KERN_DEBUG"SIZE OF SKB %d",skb->len);
send_retry:
      result = eth_start_xmit(skb,dev);
      if(result !=NETDEV_TX_OK)
      {
    	if(result == NETDEV_TX_BUSY)
    	   goto send_retry;
    	return -1;
      }
      
      return 0; 
}

static int eth_ioctl(struct net_device *dev, struct ifreq *ifr, int cmd)
{
	struct sam_debug_data 	dbg_data;
	struct eth_dev		*eem_dev = netdev_priv(dev);

	if(!netif_running(dev))
		return -EINVAL;

	switch(cmd)
	{
	case SIOCDEVPRIVATE:
	  {
	      int  result;
		
	      //Getting user data...
	      if(ifr->ifr_ifru.ifru_data == NULL)
		     return -1;
	      copy_from_user(&dbg_data,ifr->ifr_ifru.ifru_data,sizeof(dbg_data));

	      //Sending Debug Packet...
	      result = send_debug_packet(dev,&dbg_data);
	      if(result < 0)
             return result;

	      //Wating for Acknoledgement packet...
	      if(dbg_data.time_out > 0)
	      {
	    	  if(wait_event_interruptible_timeout(eem_dev->dbg_queue,eem_dev->ack_received == 1,HZ*dbg_data.time_out) < 0)
	    	  {
				 eem_dev->ack_received = 0;
				 return 0;
	    	  }
	    	  else
	    	  {
				 eem_dev->ack_received = 0;
				 return -1;
	    	  }
	      }
	      else
	      {
		  if( wait_event_interruptible(eem_dev->dbg_queue,eem_dev->ack_received == 1) == 0 )
		  {
		     eem_dev->ack_received = 0;
		     return 0;
		  }
		  else
		  {
		    eem_dev->ack_received = 0;
		    return -1;
		  }
	      }
	  }
	  default :
		return 0;
	}

	return 0;
}
#endif

/*-------------------------------------------------------------------------*/

/* initial value, changed by "ifconfig usb0 hw ether xx:xx:xx:xx:xx:xx" */
static char *dev_addr;
module_param(dev_addr, charp, S_IRUGO);
MODULE_PARM_DESC(dev_addr, "Device Ethernet Address");

/* this address is invisible to ifconfig */
static char *host_addr;
module_param(host_addr, charp, S_IRUGO);
MODULE_PARM_DESC(host_addr, "Host Ethernet Address");

static int get_ether_addr(const char *str, u8 *dev_addr)
{
	if (str) {
		unsigned	i;

		for (i = 0; i < 6; i++) {
			unsigned char num;

			if ((*str == '.') || (*str == ':'))
				str++;
			num = hex_to_bin(*str++) << 4;
			num |= hex_to_bin(*str++);
			dev_addr [i] = num;
		}
		if (is_valid_ether_addr(dev_addr))
			return 0;
	}
	random_ether_addr(dev_addr);
	return 1;
}

static struct eth_dev *the_dev;

static const struct net_device_ops eth_netdev_ops = {
	.ndo_open				= eth_open,
	.ndo_stop				= eth_stop,
	.ndo_start_xmit			= eth_start_xmit,
	.ndo_change_mtu			= ueth_change_mtu,
	.ndo_set_mac_address 	= eth_mac_addr,
	.ndo_validate_addr		= eth_validate_addr,
#ifdef CONFIG_SAMSUNG_CDC_EEM_DEBUG
	.ndo_do_ioctl			= eth_ioctl,
#endif
};

static struct device_type gadget_type = {
	.name	= "gadget",
};

/**
 * gether_setup - initialize one ethernet-over-usb link
 * @g: gadget to associated with these links
 * @ethaddr: NULL, or a buffer in which the ethernet address of the
 *	host side of the link is recorded
 * Context: may sleep
 *
 * This sets up the single network link that may be exported by a
 * gadget driver using this framework.  The link layer addresses are
 * set up using module parameters.
 *
 * Returns negative errno, or zero on success
 */
int gether_setup(struct usb_gadget *g, u8 ethaddr[ETH_ALEN])
{
	struct eth_dev		*dev;
	struct net_device	*net;
	int			status;

	if (the_dev)
		return -EBUSY;

	net = alloc_etherdev(sizeof *dev);
	if (!net)
		return -ENOMEM;

	dev = netdev_priv(net);
	spin_lock_init(&dev->lock);
#ifdef H_D_THROUGHPUT_IMPROVEMENT
	spin_lock_init(&dev->tx_req_lock);
	spin_lock_init(&dev->rx_req_lock);
#else
	spin_lock_init(&dev->req_lock);
#endif
	INIT_WORK(&dev->work, eth_work);
	INIT_LIST_HEAD(&dev->tx_reqs);
	INIT_LIST_HEAD(&dev->rx_reqs);

	skb_queue_head_init(&dev->rx_frames);

#ifdef CONFIG_SAMSUNG_PATCH_WITH_USB_GADGET_COMMON
#ifdef CONFIG_SAMSUNG_BUTTOM_HALF
	skb_queue_head_init(&dev->rx_done);
#endif
#ifdef CONFIG_SAMSUNG_CDC_EEM_DEBUG
	skb_queue_head_init(&dev->dbg_rx_queue);
#endif
#endif	// CONFIG_SAMSUNG_PATCH_WITH_USB_GADGET_COMMON

	/* network device setup */
	dev->net = net;
	strcpy(net->name, "usb%d");

	if (get_ether_addr(dev_addr, net->dev_addr))
		dev_warn(&g->dev,
			"using random %s ethernet address\n", "self");
	if (get_ether_addr(host_addr, dev->host_mac))
		dev_warn(&g->dev,
			"using random %s ethernet address\n", "host");

#ifndef CONFIG_SAMSUNG_PATCH_WITH_USB_GADGET_COMMON
	if(ethaddr)
		memcpy(ethaddr, dev->host_mac, ETH_ALEN);
#endif	// CONFIG_SAMSUNG_PATCH_WITH_USB_GADGET_COMMON

	net->netdev_ops = &eth_netdev_ops;

	SET_ETHTOOL_OPS(net, &ops);

	/* two kinds of host-initiated state changes:
	 *  - iff DATA transfer is active, carrier is "on"
	 *  - tx queueing enabled if open *and* carrier is "on"
	 */
	netif_carrier_off(net);

	dev->gadget = g;
	SET_NETDEV_DEV(net, &g->dev);
	SET_NETDEV_DEVTYPE(net, &gadget_type);

	status = register_netdev(net);
	if (status < 0) {
		dev_dbg(&g->dev, "register_netdev failed, %d\n", status);
		free_netdev(net);
	} else {
		INFO(dev, "MAC %pM\n", net->dev_addr);
		INFO(dev, "HOST MAC %pM\n", dev->host_mac);

		the_dev = dev;
	}

	return status;
}

/**
 * gether_cleanup - remove Ethernet-over-USB device
 * Context: may sleep
 *
 * This is called to free all resources allocated by @gether_setup().
 */
void gether_cleanup(void)
{
	if (!the_dev)
		return;

	unregister_netdev(the_dev->net);
	flush_work_sync(&the_dev->work);
	free_netdev(the_dev->net);

	the_dev = NULL;
}


/**
 * gether_connect - notify network layer that USB link is active
 * @link: the USB link, set up with endpoints, descriptors matching
 *	current device speed, and any framing wrapper(s) set up.
 * Context: irqs blocked
 *
 * This is called to activate endpoints and let the network layer know
 * the connection is active ("carrier detect").  It may cause the I/O
 * queues to open and start letting network packets flow, but will in
 * any case activate the endpoints so that they respond properly to the
 * USB host.
 *
 * Verify net_device pointer returned using IS_ERR().  If it doesn't
 * indicate some error code (negative errno), ep->driver_data values
 * have been overwritten.
 */
struct net_device *gether_connect(struct gether *link)
{
#ifdef CONFIG_SAMSUNG_PATCH_WITH_USB_GADGET_COMMON
	struct dual_eem_gadget *dual_eem = (struct dual_eem_gadget *) link->dev_info;
#endif	// CONFIG_SAMSUNG_PATCH_WITH_USB_GADGET_COMMON
    
	struct eth_dev		*dev = the_dev;
	int			result = 0;

	if (!dev)
		return ERR_PTR(-EINVAL);

#ifdef CONFIG_SAMSUNG_PATCH_WITH_USB_GADGET_COMMON
	if(dual_eem)
		dev = dual_eem->dev[link->devnum];
#endif	// CONFIG_SAMSUNG_PATCH_WITH_USB_GADGET_COMMON

	link->in_ep->driver_data = dev;
	result = usb_ep_enable(link->in_ep, link->in);
	if (result != 0) {
		DBG(dev, "enable %s --> %d\n",
			link->in_ep->name, result);
		goto fail0;
	}

	link->out_ep->driver_data = dev;
	result = usb_ep_enable(link->out_ep, link->out);
	if (result != 0) {
		DBG(dev, "enable %s --> %d\n",
			link->out_ep->name, result);
		goto fail1;
	}

	if (result == 0)
		result = alloc_requests(dev, link, qlen(dev->gadget));

	if (result == 0) {
		dev->zlp = link->is_zlp_ok;
		DBG(dev, "qlen %d\n", qlen(dev->gadget));

		dev->header_len = link->header_len;
		dev->unwrap = link->unwrap;
		dev->wrap = link->wrap;

		spin_lock(&dev->lock);
		dev->port_usb = link;
		link->ioport = dev;
		if (netif_running(dev->net)) {
			if (link->open)
				link->open(link);
		} else {
			if (link->close)
				link->close(link);
		}
		spin_unlock(&dev->lock);

		netif_carrier_on(dev->net);
		if (netif_running(dev->net))
			eth_start(dev, GFP_ATOMIC);

	/* on error, disable any endpoints  */
	} else {
		(void) usb_ep_disable(link->out_ep);
fail1:
		(void) usb_ep_disable(link->in_ep);
	}
fail0:
	/* caller is responsible for cleanup on error */
	if (result < 0)
		return ERR_PTR(result);
	return dev->net;
}

/**
 * gether_disconnect - notify network layer that USB link is inactive
 * @link: the USB link, on which gether_connect() was called
 * Context: irqs blocked
 *
 * This is called to deactivate endpoints and let the network layer know
 * the connection went inactive ("no carrier").
 *
 * On return, the state is as if gether_connect() had never been called.
 * The endpoints are inactive, and accordingly without active USB I/O.
 * Pointers to endpoint descriptors and endpoint private data are nulled.
 */
void gether_disconnect(struct gether *link)
{
	struct eth_dev		*dev = link->ioport;
	struct usb_request	*req;

	WARN_ON(!dev);
	if (!dev)
		return;

	DBG(dev, "%s\n", __func__);

	netif_stop_queue(dev->net);
	netif_carrier_off(dev->net);

	/* disable endpoints, forcing (synchronous) completion
	 * of all pending i/o.  then free the request objects
	 * and forget about the endpoints.
	 */
	usb_ep_disable(link->in_ep);
#ifdef H_D_THROUGHPUT_IMPROVEMENT
	spin_lock(&dev->tx_req_lock);
	while (!list_empty(&dev->tx_reqs)) {
		req = container_of(dev->tx_reqs.next,
					struct usb_request, list);
		list_del(&req->list);

		spin_unlock(&dev->tx_req_lock);
		usb_ep_free_request(link->in_ep, req);
		spin_lock(&dev->tx_req_lock);
	}
	spin_unlock(&dev->tx_req_lock);
	link->in_ep->driver_data = NULL;
	link->in = NULL;

	usb_ep_disable(link->out_ep);
	spin_lock(&dev->rx_req_lock);
	while (!list_empty(&dev->rx_reqs)) {
		req = container_of(dev->rx_reqs.next,
					struct usb_request, list);
		list_del(&req->list);

		spin_unlock(&dev->rx_req_lock);
		usb_ep_free_request(link->out_ep, req);
		spin_lock(&dev->rx_req_lock);
	}
	spin_unlock(&dev->rx_req_lock);
#else
	spin_lock(&dev->req_lock);
	while (!list_empty(&dev->tx_reqs)) {
		req = container_of(dev->tx_reqs.next,
					struct usb_request, list);
		list_del(&req->list);

		spin_unlock(&dev->req_lock);
		usb_ep_free_request(link->in_ep, req);
		spin_lock(&dev->req_lock);
	}
	spin_unlock(&dev->req_lock);
	link->in_ep->driver_data = NULL;
	link->in = NULL;

	usb_ep_disable(link->out_ep);
	spin_lock(&dev->req_lock);
	while (!list_empty(&dev->rx_reqs)) {
		req = container_of(dev->rx_reqs.next,
					struct usb_request, list);
		list_del(&req->list);

		spin_unlock(&dev->req_lock);
		usb_ep_free_request(link->out_ep, req);
		spin_lock(&dev->req_lock);
	}
	spin_unlock(&dev->req_lock);
#endif
	link->out_ep->driver_data = NULL;
	link->out = NULL;

	/* finish forgetting about this USB link episode */
	dev->header_len = 0;
	dev->unwrap = NULL;
	dev->wrap = NULL;

	spin_lock(&dev->lock);
	dev->port_usb = NULL;
	link->ioport = NULL;
	spin_unlock(&dev->lock);
}
