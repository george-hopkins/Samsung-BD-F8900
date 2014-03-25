/*
 * xHCI host controller driver
 *
 * Copyright (C) 2012 MStar Inc.
 *
 * Date: May 2012
 */

#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/dma-mapping.h>
#include "xhci.h"
#include "xhci-mstar.h"

#include <linux/kthread.h>

static const char hcd_name[] = "mstar_xhci_hcd";


void  DEQ_init(unsigned int U3PHY_D_base, unsigned int U3PHY_A_base)
{

    writeb(0x00,   (void*)(U3PHY_A_base+0xAE*2));   
    writew(0x080C, (void*)(U3PHY_D_base+0x82*2));   
    writeb(0x10,   (void*)(U3PHY_D_base+0xA4*2)); //0x10  0x30  
    writew(0x4100, (void*)(U3PHY_D_base+0xA0*2));   
	
    writew(0x06,   (void*)(U3PHY_A_base+0x06*2));   

}


void Mstar_xhc_init(unsigned int UTMI_base, unsigned int U3PHY_D_base, unsigned int U3PHY_A_base,
	                   unsigned int XHCI_base, unsigned int U3TOP_base, unsigned int U3BC_base, unsigned int flag)
{
	printk("Mstar_xhc_init start\n");


    writew(0x8051, (void*) (UTMI_base+0x20*2)); 
    writew(0x2088, (void*) (UTMI_base+0x22*2)); 
    writew(0x0004, (void*) (UTMI_base+0x2*2)); 
    writew(0x6BC3, (void*) (UTMI_base)); 
    mdelay(1);
    writew(0x69C3, (void*) (UTMI_base)); 
    mdelay(1);
    writew(0x0001, (void*) (UTMI_base)); 
    mdelay(1);
    writew(0x0228, (void*) (UTMI_base+0x4*2)); 
    writew(0x0001, (void*) (UTMI_base+0x4*2)); 
	
#if defined(CONFIG_MSTAR_EDISON) 		
    writeb(0x07, (void*) (UTMI_base+0x8*2));   
#endif

    if (flag & EHCFLAG_TESTPKG)
    {
	    writew(0x2084, (void*) (UTMI_base+0x2*2));
	    writew(0x8051, (void*) (UTMI_base+0x20*2));
    }

	writeb(readb((void*)(UTMI_base+0x09*2-1)) & ~0x08, (void*) (UTMI_base+0x09*2-1)); // Disable force_pll_on
	writeb(readb((void*)(UTMI_base+0x08*2)) & ~0x80, (void*) (UTMI_base+0x08*2)); // Enable band-gap current
	writeb(0xC3, (void*)(UTMI_base)); // reg_pdn: bit<15>, bit <2> ref_pdn
	mdelay(1);	// delay 1ms

	writeb(0x69, (void*) (UTMI_base+0x01*2-1)); // Turn on UPLL, reg_pdn: bit<9>
	mdelay(2);	// delay 2ms

	writeb(0x01, (void*) (UTMI_base)); // Turn all (including hs_current) use override mode
	writeb(0, (void*) (UTMI_base+0x01*2-1)); // Turn on UPLL, reg_pdn: bit<9>

	writeb(readb((void*)(UTMI_base+0x3C*2)) | 0x01, (void*) (UTMI_base+0x3C*2)); // set CA_START as 1
	mdelay(10);

	writeb(readb((void*)(UTMI_base+0x3C*2)) & ~0x01, (void*) (UTMI_base+0x3C*2)); // release CA_START

	while ((readb((void*)(UTMI_base+0x3C*2)) & 0x02) == 0);	// polling bit <1> (CA_END)

    if (flag & EHCFLAG_DPDM_SWAP)
    	writeb(readb((void*)(UTMI_base+0x0b*2-1)) |0x20, (void*) (UTMI_base+0x0b*2-1)); // dp dm swap

	writeb((readb((void*)(UTMI_base+0x06*2)) & 0x9F) | 0x40, (void*) (UTMI_base+0x06*2)); //reg_tx_force_hs_current_enable

	writeb(readb((void*)(UTMI_base+0x03*2-1)) | 0x28, (void*) (UTMI_base+0x03*2-1)); //Disconnect window select
	writeb(readb((void*)(UTMI_base+0x03*2-1)) & 0xef, (void*) (UTMI_base+0x03*2-1)); //Disconnect window select

	writeb(readb((void*)(UTMI_base+0x07*2-1)) & 0xfd, (void*) (UTMI_base+0x07*2-1)); //Disable improved CDR
	writeb(readb((void*)(UTMI_base+0x09*2-1)) |0x81, (void*) (UTMI_base+0x09*2-1)); // UTMI RX anti-dead-loc, ISI effect improvement
	writeb(readb((void*)(UTMI_base+0x0b*2-1)) |0x80, (void*) (UTMI_base+0x0b*2-1)); // TX timing select latch path
	writeb(readb((void*)(UTMI_base+0x15*2-1)) |0x20, (void*) (UTMI_base+0x15*2-1)); // Chirp signal source select

	writeb(readb((void*)(UTMI_base+0x3F*2-1)) |0x80, (void*) (UTMI_base+0x3F*2-1)); // Enable XHCI preamble function

#if defined(CONFIG_MSTAR_EDISON) 	
	//writeb(readb((void*)(UTMI_base+0x08*2)) | 0x8, (void*) (UTMI_base+0x08*2)); // for 240's phase as 120's clock
	writeb(readb((void*)(UTMI_base+0x08*2)) | 0x18, (void*) (UTMI_base+0x08*2)); // for 240's phase as 120's clock
#endif

#if defined(CONFIG_MSTAR_EDISON) 	
	writeb(readb((void*)(UTMI_base+0x15*2-1)) |0x40, (void*) (UTMI_base+0x15*2-1)); // change to 55 timing
#endif	

#if defined(CONFIG_MSTAR_EDISON)	
	writeb(readb((void*)(UTMI_base+0x09*2-1)) |0x04, (void*) (UTMI_base+0x09*2-1)); // for CLK 120 override enable
#endif	

#if defined(CONFIG_MSTAR_EDISON) 
	writeb(0x98, (void*) (UTMI_base+0x2c*2));
	writeb(0x10, (void*) (UTMI_base+0x2e*2));
	writeb(0x01, (void*) (UTMI_base+0x2f*2-1));
	writeb(0x02, (void*) (UTMI_base+0x2d*2-1));
#endif    

#if defined(ENABLE_LS_CROSS_POINT_ECO)
	writeb(readb((void*)(UTMI_base+0x04*2)) | 0x40, (void*) (UTMI_base+0x04*2));  //enable deglitch SE0би(low-speed cross point)
#endif

#if defined(ENABLE_TX_RX_RESET_CLK_GATING_ECO)
	writeb(readb((void*)(UTMI_base+0x04*2)) | 0x20, (void*) (UTMI_base+0x04*2)); //enable hw auto deassert sw reset(tx/rx reset)
#endif

    if (flag & EHCFLAG_TESTPKG)
    {
	    writew(0x0600, (void*) (UTMI_base+0x14*2)); 
	    writew(0x0038, (void*) (UTMI_base+0x10*2)); 
	    writew(0x0BFE, (void*) (UTMI_base+0x32*2)); 
    }

    //U3phy initial sequence 
    writew(0x0,    (void*) (U3PHY_A_base));          // power on rx atop 
    writew(0x0,    (void*) (U3PHY_A_base+0x2*2));    // power on tx atop
    writew(0x0910, (void*) (U3PHY_D_base+0x4*2));  
    writew(0x0,    (void*) (U3PHY_A_base+0x3A*2)); 
    writew(0x0160, (void*) (U3PHY_D_base+0x18*2)); 
    writew(0x0,    (void*) (U3PHY_D_base+0x20*2));   // power on u3_phy clockgen 
    writew(0x0,    (void*) (U3PHY_D_base+0x22*2));   // power on u3_phy clockgen 
  
    writew(0x013F, (void*) (U3PHY_D_base+0x4A*2)); 
    writew(0x1010, (void*) (U3PHY_D_base+0x4C*2)); 

    writew(0x0,    (void*) (U3PHY_A_base+0x3A*2));   // override PD control
    writeb(0x02,   (void*) (U3PHY_A_base+0xCD*2-1)); // reg_test_usb3aeq_acc
    writeb(0x40,   (void*) (U3PHY_A_base+0xC9*2-1)); // reg_gcr_usb3aeq_threshold_abs
    writeb(0x10,   (void*) (U3PHY_A_base+0xE5*2-1)); // [4]: AEQ select PD no-delay and 2elay path, 0: delay, 1: no-
    writeb(0x11,   (void*) (U3PHY_A_base+0xC6*2));   // analog symbol lock and EQ converage step 
    writeb(0x02,   (void*) (U3PHY_D_base+0xA0*2));   // [1] aeq mode
    
    writeb(0x07,   (void*) (U3PHY_A_base+0xB0*2));   // reg_gcr_usb3rx_eq_str_ov_value
    writeb(0x1C,   (void*) (U3PHY_A_base+0xCD*2-1)); // long EQ converge  	

#if (ENABLE_XHCI_SSC)  
	writew(0x04A0,  (void*) (U3PHY_D_base+0xC6*2));  //reg_tx_synth_span
	writew(0x0003,  (void*) (U3PHY_D_base+0xC4*2));  //reg_tx_synth_step
	writew(0x9375,  (void*) (U3PHY_D_base+0xC0*2));  //reg_tx_synth_set
	writeb(0x18,    (void*) (U3PHY_D_base+0xC2*2));  //reg_tx_synth_set
#endif  

	// RX phase control
    writew(0x0100, (void*)(U3PHY_A_base+0x90*2));   
    writew(0x0302, (void*)(U3PHY_A_base+0x92*2));   
    writew(0x0504, (void*)(U3PHY_A_base+0x94*2));   
    writew(0x0706, (void*)(U3PHY_A_base+0x96*2));   
    writew(0x1708, (void*)(U3PHY_A_base+0x98*2));   
    writew(0x1516, (void*)(U3PHY_A_base+0x9A*2));   
    writew(0x1314, (void*)(U3PHY_A_base+0x9C*2));   
    writew(0x1112, (void*)(U3PHY_A_base+0x9E*2));   
    writew(0x3000, (void*)(U3PHY_D_base+0xA8*2)); 
	writew(0x7380, (void*)(U3PHY_A_base+0x40*2));   

	#if (XHCI_ENABLE_DEQ)   
    DEQ_init(U3PHY_D_base, U3PHY_A_base);
	#endif

	writeb(readb((void*)(XHCI_base+0x430F)) | 0xC0, (void*)(XHCI_base+0x430F));  //Inter packet delay (bit6~7 = "11")
	writeb(readb((void*)(XHCI_base+0x681A)) | 0x10, (void*)(XHCI_base+0x681A));  //LFPS patch  (bit4 = 1)
	
	writeb((readb((void*)(U3TOP_base+0x24*2)) & 0xF0) | 0x8, (void*)(U3TOP_base+0x24*2));    //Bus Reset setting => default 50ms  //[5] = reg_debug_mask to 1'b0	

	#if (XHCI_CHIRP_PATCH)
		#if defined(CONFIG_MSTAR_EDISON) 
		writeb(0x0, (void*) (UTMI_base+0x3E*2)); //override value 
		writeb((readb((void*)(U3TOP_base+0x24*2)) & 0xF0) | 0x0A, (void*)(U3TOP_base+0x24*2)); // set T1=50, T2=20
		writeb(readb((void*)(UTMI_base+0x3F*2-1)) | 0x1, (void*) (UTMI_base+0x3F*2-1)); //enable the patch 
		#endif
	#endif

	
	#if (XHCI_SS_CS_PATCH)
    writeb(readb((void*)(U3PHY_A_base+0x11*2-1)) | 0x4, (void*)(U3PHY_A_base+0x11*2-1)); 
    writeb(0x3F, (void*)(U3PHY_A_base+0xE*2));  
    writeb(0x0, (void*)(U3PHY_A_base+0x25*2-1));

    writeb(0x30, (void*)(U3PHY_A_base+0x60*2));

	writeb((readb((void*)(UTMI_base+0x2D*2-1))&0x87) | 0x28, (void*)(UTMI_base+0x2D*2-1));    
	writeb(readb((void*)(U3BC_base+0x62*2)) | 0x8, (void*)(U3BC_base+0x62*2));                 
    writeb((readb((void*)(UTMI_base+0x2B*2-1))&0xFC) | 0x2, (void*)(UTMI_base+0x2B*2-1));  
	#endif


}

/* called during probe() after chip reset completes */
static int xhci_pci_setup(struct usb_hcd *hcd)
{
	struct xhci_hcd		*xhci = hcd_to_xhci(hcd);
	int			retval;
	u32			temp;

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
	//xhci->hci_version = HC_VERSION(xhci->hcc_params);
	xhci->hci_version = 0x96;  //Jonas modified for real HW version. 
	xhci->hcc_params = xhci_readl(xhci, &xhci->cap_regs->hcc_params);
	xhci_print_registers(xhci);

	/* Look for vendor-specific quirks */
	//xhci->quirks |= XHCI_RESET_ON_RESUME;
	//xhci_dbg(xhci, "QUIRK: Resetting on resume\n");

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

	return retval;

error:
	kfree(xhci);
	return retval;
}

void usb_hcd_mstar_remove(struct usb_hcd *hcd, struct platform_device *pdev)
{

	/* Fake an interrupt request in order to give the driver a chance
	 * to test whether the controller hardware has been removed (e.g.,
	 * cardbus physical eject).
	 */
	//local_irq_disable();
	//usb_hcd_irq(0, hcd);
	//local_irq_enable();

	usb_remove_hcd(hcd);
	iounmap(hcd->regs);
	release_mem_region(hcd->rsrc_start, hcd->rsrc_len);
	usb_put_hcd(hcd);
}


int xhci_hcd_mstar_probe(const struct hc_driver *driver, struct platform_device *dev)
{
	int retval=0;
	struct usb_hcd *hcd;
	//struct xhci_hcd *xhci;    
	unsigned int flag=0;

	if (usb_disabled())
		return -ENODEV;

	if (!driver)
		return -EINVAL;	

	printk("Mstar-xhci-1 H.W init\n");
	Mstar_xhc_init(_MSTAR_U3UTMI_BASE, _MSTAR_U3PHY_DTOP_BASE, _MSTAR_U3PHY_ATOP_BASE,
	               _MSTAR_XHCI_BASE, _MSTAR_U3TOP_BASE, _MSTAR_U3BC_BASE, flag);


	if (dev->resource[2].flags != IORESOURCE_IRQ) {
		pr_debug("resource[1] is not IORESOURCE_IRQ");
		retval = -ENOMEM;
	}
	hcd = usb_create_hcd(driver, &dev->dev, "mstar");
	if (!hcd)
		return -ENOMEM;
	hcd->rsrc_start = dev->resource[1].start;
	hcd->rsrc_len = dev->resource[1].end - dev->resource[1].start + 0;

	if (!request_mem_region(hcd->rsrc_start, hcd->rsrc_len, hcd_name)) {
		printk("request_mem_region failed");
		retval = -EBUSY;
		goto clear_companion;
	}

	hcd->regs = (void *)(u32)(hcd->rsrc_start);	        	
	if (!hcd->regs) {
		printk("ioremap failed");
		retval = -ENOMEM;
		goto release_mem_region;
	}

	retval = usb_add_hcd(hcd, dev->resource[2].start, IRQF_DISABLED /* | IRQF_SHARED */);
	if (retval != 0)
		goto release_mem_region;
	//set_hs_companion(dev, hcd);
  
	hcd->xhci_base = _MSTAR_XHCI_BASE;
	hcd->u3phy_d_base = _MSTAR_U3PHY_DTOP_BASE;
	hcd->u3phy_a_base = _MSTAR_U3PHY_ATOP_BASE;
	hcd->u3top_base = _MSTAR_U3TOP_BASE;
	hcd->u3indctl_base = _MSTAR_U3INDCTL_BASE;
    hcd->utmi_base = _MSTAR_U3UTMI_BASE;

	return retval;
    
release_mem_region:
	release_mem_region(hcd->rsrc_start, hcd->rsrc_len);
clear_companion:
	usb_put_hcd(hcd);    
	//clear_hs_companion(dev, hcd);
	return retval;
}


/*
 * We need to register our own probe function (instead of the USB core's
 * function) in order to create a second roothub under xHCI.
 */
static int xhci_mstar_probe(struct hc_driver *driver, struct platform_device *dev)
{
	int retval;
	struct xhci_hcd *xhci;
	struct usb_hcd *hcd;

	/* Register the USB 2.0 roothub.
	 * FIXME: USB core must know to register the USB 2.0 roothub first.
	 * This is sort of silly, because we could just set the HCD driver flags
	 * to say USB 2.0, but I'm not sure what the implications would be in
	 * the other parts of the HCD code.
	 */
	retval = xhci_hcd_mstar_probe(driver, dev);

	if (retval)
		return retval;

	/* USB 2.0 roothub is stored in the PCI device now. */
	hcd = dev_get_drvdata(&dev->dev);
	xhci = hcd_to_xhci(hcd);
	xhci->shared_hcd = usb_create_shared_hcd(driver, &dev->dev,
				"mstar", hcd);
	if (!xhci->shared_hcd) {
		retval = -ENOMEM;
		goto dealloc_usb2_hcd;
	}

	/* Set the xHCI pointer before xhci_pci_setup() (aka hcd_driver.reset)
	 * is called by usb_add_hcd().
	 */
	*((struct xhci_hcd **) xhci->shared_hcd->hcd_priv) = xhci;

	retval = usb_add_hcd(xhci->shared_hcd, dev->resource[2].start,
			IRQF_DISABLED | IRQF_SHARED);
	if (retval)
		goto put_usb3_hcd;
	/* Roothub already marked as USB 3.0 speed */

	hcd->xhci_base = _MSTAR_XHCI_BASE;
	hcd->u3phy_d_base = _MSTAR_U3PHY_DTOP_BASE;
	hcd->u3phy_a_base = _MSTAR_U3PHY_ATOP_BASE;
	hcd->u3top_base = _MSTAR_U3TOP_BASE;
	hcd->u3indctl_base = _MSTAR_U3INDCTL_BASE;
	hcd->utmi_base = _MSTAR_U3UTMI_BASE;
    
	return 0;

put_usb3_hcd:
	usb_put_hcd(xhci->shared_hcd);
dealloc_usb2_hcd:
	usb_hcd_mstar_remove(hcd, dev);
	return retval;
}

static void xhci_mstar_remove(struct usb_hcd *hcd, struct platform_device *pdev)
{
	struct xhci_hcd *xhci;

	xhci = hcd_to_xhci(hcd);
	if (xhci->shared_hcd) {
		usb_remove_hcd(xhci->shared_hcd);
		usb_put_hcd(xhci->shared_hcd);
	}
	usb_hcd_mstar_remove(hcd, pdev);
	kfree(xhci);
}

#ifdef CONFIG_PM
static int xhci_pci_suspend(struct usb_hcd *hcd, bool do_wakeup)
{
	struct xhci_hcd	*xhci = hcd_to_xhci(hcd);
	int	retval = 0;

	if (hcd->state != HC_STATE_SUSPENDED ||
			xhci->shared_hcd->state != HC_STATE_SUSPENDED)
		return -EINVAL;

	retval = xhci_suspend(xhci);

	return retval;
}

static int xhci_pci_resume(struct usb_hcd *hcd, bool hibernated)
{
	struct xhci_hcd		*xhci = hcd_to_xhci(hcd);
	int			retval = 0;

	retval = xhci_resume(xhci, hibernated);
	return retval;
}
#endif /* CONFIG_PM */

static struct hc_driver xhci_mstar_hc_driver = {
	.description =		hcd_name,
	.product_desc =		"Mstar xHCI Host Controller",
	.hcd_priv_size =	sizeof(struct xhci_hcd *),

	/*
	 * generic hardware linkage
	 */
	.irq =			xhci_irq,
	.flags =		HCD_MEMORY | HCD_USB3 | HCD_SHARED,

	/*
	 * basic lifecycle operations
	 */
	.reset =		xhci_pci_setup,
	.start =		xhci_run,
#ifdef CONFIG_PM
	.pci_suspend =          xhci_pci_suspend,
	.pci_resume =           xhci_pci_resume,
#endif
	.stop =			xhci_stop,
	.shutdown =		xhci_shutdown,

	/*
	 * managing i/o requests and associated device resources
	 */
	.urb_enqueue =		xhci_urb_enqueue,
	.urb_dequeue =		xhci_urb_dequeue,
	.alloc_dev =		xhci_alloc_dev,
	.free_dev =		xhci_free_dev,
	.alloc_streams =	xhci_alloc_streams,
	.free_streams =		xhci_free_streams,
	.add_endpoint =		xhci_add_endpoint,
	.drop_endpoint =	xhci_drop_endpoint,
	.endpoint_reset =	xhci_endpoint_reset,
	.check_bandwidth =	xhci_check_bandwidth,
	.reset_bandwidth =	xhci_reset_bandwidth,
	.address_device =	xhci_address_device,
	.update_hub_device =	xhci_update_hub_device,
	.reset_device =		xhci_discover_or_reset_device,

	/*
	 * scheduling support
	 */
	.get_frame_number =	xhci_get_frame,

	/* Root hub support */
	.hub_control =		xhci_hub_control,
	.hub_status_data =	xhci_hub_status_data,
	.bus_suspend =		xhci_bus_suspend,
	.bus_resume =		xhci_bus_resume,
};

static int xhci_hcd_mstar_drv_probe(struct platform_device *pdev)
{
	int ret;

	pr_debug("In xhci_hcd_mstar_drv_probe\n");

	if (usb_disabled())
		return -ENODEV;

	ret = xhci_mstar_probe(&xhci_mstar_hc_driver, pdev);
	return ret;
}

static int xhci_hcd_mstar_drv_remove(struct platform_device *pdev)
{
	struct usb_hcd *hcd = platform_get_drvdata(pdev);

	xhci_mstar_remove(hcd, pdev);
	return 0;
}

/*-------------------------------------------------------------------------*/

static struct platform_driver xhci_mstar_driver = {

	.probe =	xhci_hcd_mstar_drv_probe,
	.remove =	xhci_hcd_mstar_drv_remove,
	// .shutdown	= usb_hcd_platform_shutdown,

	/* suspend and resume implemented later */

    .driver = {
        .name   = "Mstar-xhci-1",
        // .bus    = &platform_bus_type,
	},
};

int xhci_register_mstar(void)
{
	return platform_driver_register(&xhci_mstar_driver);
}

void xhci_unregister_mstar(void)
{
	platform_driver_unregister(&xhci_mstar_driver);
}
