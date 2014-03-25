/*
 * EHCI HCD Bus glue logic for SDP USB Controller
 *
 * Modification history
 * --------------------
 * 29.July.2009 ij.jang : Port to kernel 2.6.24, rename platform name to sdp
 * 18.July.2007 ij.jang : Port to kernel 2.6.18
 * 08.Nov.2005 ij.jang : Add initial support for s5h2150
 * 20.Oct.2006 ij.jang : Created for s5h2110
 */

#include <linux/platform_device.h>

#if !defined(CONFIG_ARCH_SDP) && !defined(CONFIG_ARCH_CCEP)
#error "This file is only for the SDPxxxx based platform. Configuration may wrong."
#endif

/* called during sdp_ehci_hcd_probe() after chip reset completes */
static int sdp_ehci_setup (struct usb_hcd *hcd)
{
	struct ehci_hcd *ehci = hcd_to_ehci(hcd);
	int retval;

	ehci_dbg (ehci, "SDP ehci setup()\n");

	/* EHCI register address */
	ehci = hcd_to_ehci (hcd);
	ehci->caps = hcd->regs;
	ehci->regs = hcd->regs +
		HC_LENGTH (ehci,readl(&ehci->caps->hc_capbase));

	dbg_hcs_params (ehci, "reset");
	dbg_hcc_params (ehci, "reset");
	
	/* cache this readonly data (capability) */
	ehci->hcs_params = readl (&ehci->caps->hcs_params);

	retval = ehci_halt (ehci);
	if (retval != 0) {
		ehci_err (ehci, "ehci_halt error\n");
		return retval;
	}

	retval = ehci_init (hcd);
	if (retval != 0) {
		ehci_err (ehci, "ehci_init error\n");
		return retval;
	}

	ehci->sbrn = 0x20;
	ehci_info (ehci, "sdp_ehci_setup success\n");

	return 0;
}

static int sdp_ehci_hcd_probe(struct hc_driver *driver,
			struct platform_device *pdev)
{
	struct usb_hcd *hcd;
	struct resource *res;
	unsigned long mem_start=0, mem_len=0;
	int irq;
	int retval;
	void *reg_addr = NULL;

	dev_info (&pdev->dev, "initializing SDP-SOC USB Controller\n");

	res = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if (!res) {
		dev_err(&pdev->dev,
			"Found HC with no IRQ. Check %s setup!\n",
			dev_name(&pdev->dev));
		retval = -ENODEV;
		goto err0;
	}
	irq = res->start;
	dev_dbg (&pdev->dev, "Found IRQ resource=%d\n", irq);

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
		dev_err(&pdev->dev, "controller already in use\n");
		retval = -EBUSY;
		goto err0;
	}

	dev_dbg (&pdev->dev, "Found Memory resource: %08X--%08X\n",
			(unsigned int)res->start,
			(unsigned int)res->end);

	mem_start = res->start;
	mem_len = res->end - res->start + 1;
	reg_addr = ioremap_nocache (mem_start, mem_len);
	
	if (reg_addr == NULL) {
		dev_err(&pdev->dev, "error mapping memory\n");
		retval = -EFAULT;
		goto err1;
	}

	dev_dbg (&pdev->dev, "SDP_EHCI ioremapped : %08X\n",
			(unsigned int)reg_addr);

	hcd = usb_create_hcd (driver, &pdev->dev, dev_name(&pdev->dev));
	if (!hcd) {
		retval = -ENOMEM;
		goto err2;
	}

	hcd->regs = reg_addr;
	hcd->rsrc_start = mem_start;
	hcd->rsrc_len = mem_len;

	retval = usb_add_hcd (hcd, irq, IRQF_DISABLED);
	
	if (retval !=0) {
		usb_put_hcd (hcd);
	} else {
		dev_info (&pdev->dev, "ehci-sdp probing completed.\n");
		return 0;
	}

err2:
	iounmap (reg_addr);
err1:
	release_mem_region(mem_start, mem_len);
err0:
	dev_err(&pdev->dev, "init %s fail, %d\n", dev_name(&pdev->dev), retval);
	return retval;
}

static void sdp_ehci_hcd_remove(struct usb_hcd *hcd, struct platform_device *pdev)
{
	dev_dbg (&pdev->dev, "remove (bus=%s, state=%x)\n",
			hcd->self.bus_name, hcd->state);

	usb_remove_hcd (hcd);
	iounmap (hcd->regs);
	release_mem_region (hcd->rsrc_start, hcd->rsrc_len);
	usb_put_hcd (hcd);
}

static struct hc_driver sdp_ehci_hc_driver = {
	.description		= hcd_name,
	.product_desc		= "SDP On-chip EHCI Host controller",
	.hcd_priv_size		= sizeof(struct ehci_hcd),
	
	/*
	 * generic hardware linkage
	 */
	.irq			= ehci_irq,
	.flags			= HCD_MEMORY | HCD_USB2,
	
	/*
	 * basic lifecycle operations
	 */
	.reset			= sdp_ehci_setup,
	.start			= ehci_run,
	.stop			= ehci_stop,
	.shutdown		= ehci_shutdown,

	.bus_suspend		= ehci_bus_suspend,
	.bus_resume	        = ehci_bus_resume,

	/*
	 * managing i/o requests and associated device resources
	 */
	.urb_enqueue		= ehci_urb_enqueue,
	.urb_dequeue		= ehci_urb_dequeue,
	.endpoint_disable	= ehci_endpoint_disable,
	
	/*
	 * scheduling support
	 */
	.get_frame_number	= ehci_get_frame,
	
	/*
	 * root hub support
	 */
	.hub_status_data	= ehci_hub_status_data,
	.hub_control		= ehci_hub_control,
#if defined(CONFIG_PM)
	.pci_suspend		= ehci_bus_suspend,
	.pci_resume		= ehci_bus_resume,
#endif

	.get_frame_number = ehci_get_frame,
	.endpoint_reset = ehci_endpoint_reset,
	//.start_port_reset = ,
	
	.relinquish_port = ehci_relinquish_port,
	.port_handed_over = ehci_port_handed_over,
	.clear_tt_buffer_complete = ehci_clear_tt_buffer_complete,
};

static int sdp_ehci_drv_probe(struct platform_device *pdev)
{
	if (usb_disabled())
		return -ENODEV;

	return sdp_ehci_hcd_probe(&sdp_ehci_hc_driver, pdev);
}

static int sdp_ehci_drv_remove(struct platform_device *pdev)
{
	struct usb_hcd *hcd = platform_get_drvdata(pdev);

	dev_info(&pdev->dev, "sdp_ehci_drv_remove\n");
	sdp_ehci_hcd_remove(hcd, pdev);

	return 0;
}

MODULE_ALIAS("ehci-sdp");

static struct platform_driver ehci_sdp_driver = {
	.probe		= sdp_ehci_drv_probe,
	.remove		= sdp_ehci_drv_remove,
	.shutdown	= usb_hcd_platform_shutdown,
	.driver 	= {
		.name = "ehci-sdp",
		.bus = &platform_bus_type,
	}
};
