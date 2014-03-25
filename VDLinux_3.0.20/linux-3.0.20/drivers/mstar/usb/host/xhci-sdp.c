/*
 * xHCI HCD Bus glue.
 * 
 * Copyright (C) 2012 Samsung electronics Corp.
 *
 * Author: Seihee Chon
 * 
 * revision history
 * --------------------------------------------------
 *     date     | 	author   |  description
 * --------------------------------------------------
 *  15.Mar.2012		sh.chon		created
 */
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/dma-mapping.h>
#include "xhci.h"

#if !defined(CONFIG_ARCH_SDP) && !defined(CONFIG_ARCH_CCEP)
#error "This file is only for the SDPXXXX based platform. Configuration may wrong."
#endif

static const char hcd_name[] = "xhci_hcd";

/* called during sdp_xhci_hcd_probe() after chip reset completes */
static int sdp_xhci_setup(struct usb_hcd *hcd)
{
	struct xhci_hcd *xhci = hcd_to_xhci(hcd);
	int retval;
	u32	temp;

	hcd->self.sg_tablesize = TRBS_PER_SEGMENT - 2;

	if (usb_hcd_is_primary_hcd(hcd)) {
		xhci = kzalloc(sizeof(struct xhci_hcd), GFP_KERNEL);
		if (!xhci)
			return -ENOMEM;
		*((struct xhci_hcd **) hcd->hcd_priv) = xhci;
		xhci->main_hcd = hcd;
		/* Mark the first roothub as being USB 2.0.
		 * The xHCI driver will register the USB 3.0 roothub.
		 */
		hcd->speed = HCD_USB2;
		hcd->self.root_hub->speed = USB_SPEED_HIGH;
		/*
		 * USB 2.0 roothub under xHCI has an integrated TT,
		 * (rate matching hub) as opposed to having an OHCI/UHCI
		 * companion controller.
		 */
		hcd->has_tt = 1;
	} else {
		/* xHCI private pointer was set in xhci_pci_probe for the second
		 * registered roothub.
		 */
		xhci = hcd_to_xhci(hcd);
		temp = xhci_readl(xhci, &xhci->cap_regs->hcc_params);
		if (HCC_64BIT_ADDR(temp)) {
			xhci_dbg(xhci, "Enabling 64-bit DMA addresses.\n");
			dma_set_mask(hcd->self.controller, DMA_BIT_MASK(64));
		} else {
			dma_set_mask(hcd->self.controller, DMA_BIT_MASK(32));
		}
		return 0;
	}

	xhci->cap_regs = hcd->regs;
	xhci->op_regs = hcd->regs + 
		HC_LENGTH(xhci_readl(xhci, &xhci->cap_regs->hc_capbase));
	xhci->run_regs = hcd->regs + 
		(xhci_readl(xhci, &xhci->cap_regs->run_regs_off) & RTSOFF_MASK);

	/* Cache read-only capability registers */
	xhci->hcs_params1 = xhci_readl(xhci, &xhci->cap_regs->hcs_params1);
	xhci->hcs_params2 = xhci_readl(xhci, &xhci->cap_regs->hcs_params2);
	xhci->hcs_params3 = xhci_readl(xhci, &xhci->cap_regs->hcs_params3);
	xhci->hcc_params = xhci_readl(xhci, &xhci->cap_regs->hc_capbase);
	xhci->hci_version = HC_VERSION(xhci->hcc_params);
	xhci->hcc_params = xhci_readl(xhci, &xhci->cap_regs->hcc_params);
	xhci_print_registers(xhci);

	/* Look for vendor-specific quirks */
	// removed PCI settings
	
	/* Make sure the HC is halted. */
	retval = xhci_halt(xhci);
	if (retval)
		goto error;

	xhci_dbg(xhci, "Resetting HCD\n");
	/* Reset the internal HC memory state and registers. */
	retval = xhci_reset(xhci);
	if (retval)
		goto error;

	xhci_dbg(xhci, "Reset complete\n");

	temp = xhci_readl(xhci, &xhci->cap_regs->hcc_params);
	if (HCC_64BIT_ADDR(temp)) {
		xhci_dbg(xhci, "Enabling 64-bit DMA addresses.\n");
		dma_set_mask(hcd->self.controller, DMA_BIT_MASK(64));
	} else {
		dma_set_mask(hcd->self.controller, DMA_BIT_MASK(32));
	}

	xhci_dbg(xhci, "Calling HCD init\n");
	/* Initialize HCD and host controller data structures. */
	retval = xhci_init(hcd);
	if (retval)
		goto error;

	xhci_dbg(xhci, "Called HCD init\n");

	/* Find any debug ports */
	// removed PCI settings
	
	/* addtional settings */
#ifdef CONFIG_ARCH_SDP1105
#define XHCI_GSBUSCFG0_OFFSET	(0xc100)
#define XHCI_GSBUSCFG1_OFFSET	(0xc104)
	temp = readl(hcd->regs + XHCI_GSBUSCFG0_OFFSET);
	temp = (temp & 0xf) | 0xe;
	writel(temp, hcd->regs + XHCI_GSBUSCFG0_OFFSET);

	temp = readl(hcd->regs + XHCI_GSBUSCFG1_OFFSET);
	temp = (temp & 0xf00) | 0x700;
	writel(temp, hcd->regs + XHCI_GSBUSCFG1_OFFSET);
#endif

	return retval;
	
error:
	kfree(xhci);
	return retval;
}
EXPORT_SYMBOL(sdp_xhci_setup);

static int sdp_hcd_probe(struct hc_driver *driver, struct platform_device *pdev)
{
	struct usb_hcd *hcd;
	struct resource *res;
	unsigned long mem_start = 0, mem_len = 0;
	int irq;
	int retval;
	void *reg_addr = NULL;

	if (usb_disabled())
		return -ENODEV;

	if (!driver)
		return -EINVAL;

	/* get irq */
	res = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if (!res) {
		dev_err(&pdev->dev,	
				"Found HC with no IRQ. Check %s setup!\n", 
				dev_name(&pdev->dev));
		retval = -ENODEV;
		goto err0;
	}
	irq = res->start;

	dev_dbg(&pdev->dev, "Found IRQ resource=%d\n", irq);

	/* get regtister address */
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(&pdev->dev, 
				"Found HC with no register addr. Check %s setup!\n", 
				dev_name(&pdev->dev));
		retval = -ENODEV;
		goto err0;
	}

	if (!request_mem_region(res->start, res->end - res->start + 1,
							driver->description)) {
		dev_err(&pdev->dev, "Controller already in use\n");
		retval = -EBUSY;
		goto err0;
	}
	dev_dbg(&pdev->dev, "Found Memory resource: %08X--%08X\n",
			(unsigned int)res->start,
			(unsigned int)res->end);

	mem_start = res->start;
	mem_len = res->end - res->start + 1;
	
	reg_addr = ioremap_nocache(mem_start, mem_len);
	if (reg_addr == NULL) {
		dev_err(&pdev->dev, "error mapping memory\n");
		retval = -EFAULT;
		goto err1;
	}
	dev_dbg (&pdev->dev, "SDP_XHCI ioremapped : %08X\n",
			(unsigned int)reg_addr);

	hcd = usb_create_hcd (driver, &pdev->dev, dev_name(&pdev->dev));
	if (!hcd) {
		retval = -ENOMEM;
		goto err2;
	}

	hcd->regs = reg_addr;
	hcd->rsrc_start = mem_start;
	hcd->rsrc_len = mem_len;

	retval = usb_add_hcd(hcd, irq, IRQF_DISABLED);// | IRQF_SHARED);
	if (retval != 0) {
		dev_err(&pdev->dev, "usb_add_hcd error(0x%x)\n", retval);
		usb_put_hcd(hcd);
	} else {
		dev_info(&pdev->dev, "xhci-sdp probing completed.\n");
		return 0;
	}

err2:
	iounmap(reg_addr);
err1:
	release_mem_region(mem_start, mem_len);
err0:
	dev_err(&pdev->dev, "init %s fail, %d\n", dev_name(&pdev->dev), retval);
	return retval;	
}

static void sdp_hcd_remove(struct usb_hcd *hcd, struct platform_device *pdev)
{
	dev_dbg(&pdev->dev, "remove (bus=%s, state=%x)\n",
			hcd->self.bus_name, hcd->state);

	usb_remove_hcd(hcd);
	iounmap(hcd->regs);
	release_mem_region(hcd->rsrc_start, hcd->rsrc_len);
	usb_put_hcd(hcd);
}

static int sdp_xhci_hcd_probe(struct hc_driver *driver, struct platform_device *pdev)
{
	int retval;
	struct xhci_hcd *xhci;
	struct usb_hcd *hcd;
	struct resource *res;
	int irq;
	
	dev_info(&pdev->dev, "initializing SDP-SoC USB3 Controller\n");
	
	/* Register the USB 2.0 roothub.
	 * FIXME: USB core must know to register the USB 2.0 roothub first.
	 * This is sort of silly, because we could just set the HCD driver flags
	 * to say USB 2.0, but I'm not sure what the implications would be in
	 * the other parts of the HCD code.
	 */
	retval = sdp_hcd_probe(driver, pdev);
	dev_info(&pdev->dev, "after USB2.0 roothub registration\n");

	if (retval)
		return retval;

	/* USB 2.0 roothub is stored now. */
	hcd = dev_get_drvdata(&pdev->dev);
	xhci = hcd_to_xhci(hcd);
	xhci->shared_hcd = usb_create_shared_hcd(driver, &pdev->dev,
				dev_name(&pdev->dev), hcd);
	if (!xhci->shared_hcd) {
		retval = -ENOMEM;
		goto dealloc_usb2_hcd;
	}

	/* Set the xHCI pointer before xhci_pci_setup() (aka hcd_driver.reset)
	 * is called by usb_add_hcd().
	 */
	*((struct xhci_hcd **) xhci->shared_hcd->hcd_priv) = xhci;

	res = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if (!res) {
		dev_err(&pdev->dev,	
				"Found HC with no IRQ. Check %s setup!\n", 
				dev_name(&pdev->dev));
		retval = -ENODEV;
		goto dealloc_usb2_hcd;
	}
	irq = res->start;

	retval = usb_add_hcd(xhci->shared_hcd, irq, IRQF_DISABLED);// | IRQF_SHARED);
	if (retval)
		goto put_usb3_hcd;
	/* Roothub already marked as USB 3.0 speed */

	return 0;

put_usb3_hcd:
	usb_put_hcd(xhci->shared_hcd);
dealloc_usb2_hcd:
	sdp_hcd_remove(hcd, pdev);
	return retval;
}

static void sdp_xhci_hcd_remove(struct usb_hcd *hcd, struct platform_device *pdev)
{
	struct xhci_hcd *xhci;

	xhci = hcd_to_xhci(hcd);
	if (xhci->shared_hcd) {
		usb_remove_hcd(xhci->shared_hcd);
		usb_put_hcd(xhci->shared_hcd);
	}
	sdp_hcd_remove(hcd, pdev);
	kfree(xhci);
}

static struct hc_driver sdp_xhci_hc_driver = {
	.description		= hcd_name,
	.product_desc		= "SDP On-chip xHCI Host Controller",
	.hcd_priv_size		= sizeof(struct xhci_hcd *),
	
	/*
	 * generic hardware linkage
	 */
	.irq			= xhci_irq,
	.flags			= HCD_MEMORY | HCD_USB3,/* | HCD_SHARED,*/

	/*
	 * basic lifecycle operations
	 */
	.reset			= sdp_xhci_setup,
	.start			= xhci_run,
#ifdef CONFIG_PM
	//.pci_suspend	= xhci_pci_suspend,
	//.pci_resume	= xhci_pci_resume,
#endif
	.stop			= xhci_stop,
	.shutdown		= xhci_shutdown,

	/*
	 * managing i/o requests and associated device resources
	 */
	.urb_enqueue		= xhci_urb_enqueue,
	.urb_dequeue		= xhci_urb_dequeue,
	//.endpoint_disable	= xhci_endpoint_disble,
	//.start_port_reset	= ,
	//.relinquish_port	= ,
	//.port_handed_over	= ,
	//.clear_tt_buffer_complete = ,
	.alloc_dev			= xhci_alloc_dev,
	.free_dev			= xhci_free_dev,
	.alloc_streams		= xhci_alloc_streams,
	.free_streams		= xhci_free_streams,
	.add_endpoint		= xhci_add_endpoint,
	.drop_endpoint		= xhci_drop_endpoint,
	.endpoint_reset =	xhci_endpoint_reset,
	.check_bandwidth	= xhci_check_bandwidth,
	.reset_bandwidth	= xhci_reset_bandwidth,
	.address_device		= xhci_address_device,
	.update_hub_device	= xhci_update_hub_device,
	.reset_device		= xhci_discover_or_reset_device,

	/*
	 * scheduling support
	 */
	.get_frame_number	= xhci_get_frame,

	/* Root hub support */
	.hub_control		= xhci_hub_control,
	.hub_status_data	= xhci_hub_status_data,
	.bus_suspend =		xhci_bus_suspend,
	.bus_resume =		xhci_bus_resume,
};


static int sdp_xhci_drv_probe(struct platform_device *pdev)
{
	if (usb_disabled())
		return -ENODEV;

	return sdp_xhci_hcd_probe(&sdp_xhci_hc_driver, pdev);
}

static int sdp_xhci_drv_remove(struct platform_device *pdev)
{
	struct usb_hcd *hcd = platform_get_drvdata(pdev);

	dev_info(&pdev->dev, "sdp_xhci_drv_remove\n");
	sdp_xhci_hcd_remove(hcd, pdev);

	return 0;
}

MODULE_ALIAS("xhci-sdp");

/* platform driver module */
static struct platform_driver xhci_sdp_driver = {
	.probe		= sdp_xhci_drv_probe,
	.remove		= sdp_xhci_drv_remove,
	.shutdown	= usb_hcd_platform_shutdown,
	//.suspend	=,
	//.resume	=,
	.driver		= {
		.name = "xhci-sdp",
		.bus = &platform_bus_type,
	},
	//.id_table	= sdp_xhci_id,
};

int xhci_register(void)
{
	return platform_driver_register(&xhci_sdp_driver);
}

void xhci_unregister(void)
{
	platform_driver_unregister(&xhci_sdp_driver);
}

