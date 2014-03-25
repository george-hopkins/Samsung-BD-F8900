/*
 * drivers/usb/gadget/sdp_udc.h
 * Samsung SDP on-chip full/high speed USB device controllers
 * Copyright (C) 2012 for Samsung Electronics
 */

#ifndef __SDP_USB_GADGET
#define __SDP_USB_GADGET

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/ioport.h>
#include <linux/types.h>
#include <linux/version.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/list.h>
#include <linux/interrupt.h>
#include <linux/proc_fs.h>
#include <linux/mm.h>
#include <linux/device.h>
#include <linux/dma-mapping.h>
#include <linux/io.h>

#include <linux/usb/ch9.h>
#include <linux/usb/gadget.h>

#include <asm/byteorder.h>
#include <asm/dma.h>
#include <asm/irq.h>
#include <asm/system.h>
#include <asm/unaligned.h>
//#include <linux/wakelock.h>

/* Max packet size */
#if 0 /* defined(CONFIG_USB_GADGET_S3C_FS) */
#define EP0_FIFO_SIZE		8
#define EP_FIFO_SIZE		64
#define SDP_MAX_ENDPOINTS	5
#else
#define EP0_FIFO_SIZE		64
#define EP_FIFO_SIZE		512
#define EP_FIFO_SIZE2		1024
#define SDP_MAX_ENDPOINTS	9
#define SDP_MAX_FIFOS		4
#define DED_TX_FIFO			1	/* Dedicated NPTx fifo */
#endif

#define WAIT_FOR_SETUP          0
#define DATA_STATE_XMIT         1
#define DATA_STATE_NEED_ZLP     2
#define WAIT_FOR_OUT_STATUS     3
#define DATA_STATE_RECV         4
#define RegReadErr				5
#define FAIL_TO_SETUP			6

#define TEST_J_SEL				0x1
#define TEST_K_SEL				0x2
#define TEST_SE0_NAK_SEL		0x3
#define TEST_PACKET_SEL			0x4
#define TEST_FORCE_ENABLE_SEL	0x5

/* ************************************************************************* */
/* IO
 */

typedef enum ep_type {
	ep_control, 
	ep_bulk_in, 
	ep_bulk_out, 
	ep_int_in,
	ep_int_out,
	ep_iso_in,
	ep_iso_out
} ep_type_t;

struct sdp_ep {
	struct usb_ep ep;
	struct sdp_udc *dev;

	const struct usb_endpoint_descriptor *desc;
	struct list_head queue;
	unsigned long pio_irqs;

	u8 stopped;
	u8 bEndpointAddress;
	u8 bmAttributes;

	ep_type_t ep_type;
	u32 fifo;
#if 0//def CONFIG_USB_GADGET_SDP_FS
	u32 csr1;
	u32 csr2;
#endif
};

struct sdp_request {
	struct usb_request req;
	struct list_head queue;
	unsigned char mapped;
	unsigned written_bytes;
};

struct sdp_udc {
	struct usb_gadget gadget;
	struct usb_gadget_driver *driver;
	struct platform_device *dev;
	spinlock_t lock;

	int ep0state;
	struct sdp_ep ep[SDP_MAX_ENDPOINTS];

	unsigned char usb_address;
	struct usb_ctrlrequest *usb_ctrl;
	dma_addr_t usb_ctrl_dma;

	void __iomem *regs;
	struct resource *regs_res;
	unsigned int irq;
	unsigned req_pending:1, req_std:1, req_config:1;
//	struct wake_lock	usbd_wake_lock;
	int udc_enabled;
};

extern struct sdp_udc *the_controller;
extern void samsung_cable_check_status(int flag);

#define ep_is_in(EP)	(((EP)->bEndpointAddress&USB_DIR_IN) == USB_DIR_IN)

#define ep_index(EP)		((EP)->bEndpointAddress&0xF)
#define ep_maxpacket(EP)	((EP)->ep.maxpacket)

#endif
