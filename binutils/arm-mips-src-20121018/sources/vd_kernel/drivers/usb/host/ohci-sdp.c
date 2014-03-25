/*
 * OHCI HCD (Host Controller Driver) for USB.
 *
 * (C) Copyright 1999 Roman Weissgaerber <weissg@vienna.at>
 * (C) Copyright 2000-2002 David Brownell <dbrownell@users.sourceforge.net>
 * (C) Copyright 2002 Hewlett-Packard Company
 * 
 * SA1111 Bus Glue
 *
 * Written by Christopher Hoover <ch@hpl.hp.com>
 * Based on fragments of previous driver by Rusell King et al.
 *
 * This file is licenced under the GPL.
 *
 * Modification history
 * --------------------
 * 29.July.2009 ij.jang : Port to kernel 2.6.24 rename platform name
 * 18.July.2007 ij.jang : Port to kernel 2.6.18
 * 08.Nov.2006 ij.jang : add inittial support for s5h2x
 * 10.Oct.2006 ij.jang : create for s5h2x
 */
 
#include <linux/platform_device.h>

#if !defined(CONFIG_ARCH_SDP) && !defined(CONFIG_ARCH_CCEP)
#error "This file is only for the SDP based platform. Configuration may wrong."
#endif

#ifdef DEBUG
#define sdp_dbg	printk
#else
#define sdp_dbg(...)
#endif

extern int usb_disabled(void);

/*-------------------------------------------------------------------------*/

void sdp_ohci_hcd_remove(struct usb_hcd *hcd, struct platform_device *pdev)
{
	usb_remove_hcd (hcd);
	iounmap (hcd->regs);
	release_mem_region (hcd->rsrc_start, hcd->rsrc_len);
	usb_put_hcd (hcd);

	dev_dbg (&pdev->dev, "SDP_OHCI hcd removed.\n");
}

int sdp_ohci_hcd_probe(const struct hc_driver *driver,
				struct platform_device *pdev)
{
	struct usb_hcd *hcd;
	struct resource *res;
	int irq, retval;
	void *reg_addr = NULL;

	hcd = usb_create_hcd (driver, &pdev->dev, "ohci-sdp");
	if (hcd == NULL) {
		return -ENOMEM;
	}

	/* find IRQ number */
	res = platform_get_resource (pdev, IORESOURCE_IRQ, 0);
	if (!res) {
		dev_err (&pdev->dev, "Found HCD with no IRQ, Check %s setup!\n",
				dev_name(&pdev->dev));
		retval = -ENODEV;
		goto err3;
	}
	irq = res->start;

	res = platform_get_resource (pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err (&pdev->dev, "Found HC with no register addr. Check %s setup!\n",
				dev_name(&pdev->dev));
		retval = -ENODEV;
		goto err3;
	}

	hcd->rsrc_start = res->start;
	hcd->rsrc_len = res->end - res->start;
	
	if (!request_mem_region (hcd->rsrc_start, hcd->rsrc_len, hcd_name)) {
		dev_err (&pdev->dev, "Failed to request memory region\n");
		retval = -ENODEV;
		goto err3;
	}

	reg_addr = ioremap_nocache (hcd->rsrc_start, hcd->rsrc_len);
	if (reg_addr == NULL) {
		dev_err (&pdev->dev, "error mapping memory\n");
		retval = -EFAULT;
		goto err2;
	}

	hcd->regs = reg_addr;
	dev_dbg (&pdev->dev, "SDP_OHCI ioremapped : %08X\n",
			(unsigned int)reg_addr);

	ohci_hcd_init (hcd_to_ohci(hcd));

	retval = usb_add_hcd (hcd, irq, IRQF_DISABLED);
	if (retval != 0) {
		dev_err (&pdev->dev, "Failed to add new hcd\n");
		goto err1;
	}

	dev_dbg (&pdev->dev, "ohci_hcd %p, irq %d mem %x~%x\n",
			hcd, irq, (u32)hcd->rsrc_start, (u32)hcd->rsrc_start + (u32)hcd->rsrc_len);
	dev_info (&pdev->dev, "ohci-sdp probing completed.\n");
	return 0;

err1:
	iounmap (hcd->regs);
err2:
	release_mem_region (hcd->rsrc_start, hcd->rsrc_len);
err3:
	usb_put_hcd (hcd);

	return retval;
}

static int __devinit sdp_ohci_hcd_start (struct usb_hcd *hcd)
{
	struct ohci_hcd	*ohci = hcd_to_ohci(hcd);
	int ret;

	if ((ret = ohci_init(ohci)) < 0)
	{
		err ("failed to ohci_init\n");
		return ret;
	}

	if ((ret = ohci_run (ohci)) < 0) {
		err ("can't start %s", hcd->self.bus_name);
		ohci_stop (hcd);
		return ret;
	}
	return 0;
}

static const struct hc_driver sdp_ohci_hc_driver = {
	.description =		hcd_name,
	.product_desc		= "SDP On-chip OHCI Host controller",
	.hcd_priv_size		= sizeof (struct ohci_hcd),

	/*
	 * generic hardware linkage
	 */
	.irq			= ohci_irq,
	.flags			= HCD_USB11 | HCD_MEMORY,

	/*
	 * basic lifecycle operations
	 */
	.start			= sdp_ohci_hcd_start,
	.stop			= ohci_stop,
	.shutdown		= ohci_shutdown,

	/*
	 * managing i/o requests and associated device resources
	 */
	.urb_enqueue		= ohci_urb_enqueue,
	.urb_dequeue		= ohci_urb_dequeue,
	.endpoint_disable	= ohci_endpoint_disable,

	/*
	 * scheduling support
	 */
	.get_frame_number	= ohci_get_frame,

	/*
	 * root hub support
	 */
	.hub_status_data	= ohci_hub_status_data,
	.hub_control		= ohci_hub_control,
#if defined(CONFIG_PM)
	.bus_suspend		= ohci_bus_suspend,
	.bus_resume		= ohci_bus_resume,
#endif
	.start_port_reset	= ohci_start_port_reset,
};

static int sdp_ohci_drv_probe(struct platform_device *pdev)
{
	if (usb_disabled())
		return -ENODEV;

	return sdp_ohci_hcd_probe(&sdp_ohci_hc_driver, pdev);
}

static int sdp_ohci_drv_remove(struct platform_device *pdev)
{
	struct usb_hcd *hcd = platform_get_drvdata (pdev);
	sdp_ohci_hcd_remove(hcd, pdev);
	return 0;
}

static struct platform_driver ohci_sdp_driver = {
	.probe	= sdp_ohci_drv_probe,
	.remove = sdp_ohci_drv_remove,
	.shutdown = usb_hcd_platform_shutdown,
	
	.driver	= {
		.owner	= THIS_MODULE,
		.name	= "ohci-sdp",
	},
};

