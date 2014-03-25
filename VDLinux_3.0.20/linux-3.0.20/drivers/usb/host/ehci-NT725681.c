/*
 *  che-chun Kuo <c_c_kuo@novatek.com.tw>
 *
 * This file is licenced under the GPL.
 */

#include "ehci-NT72568.h"
#include <linux/spinlock.h>
#include <asm/ntclk.h>
#ifdef CONFIG_DMA_COHERENT
#define USBH_ENABLE_INIT  (USBH_ENABLE_CE \
                         | USB_MCFG_PFEN | USB_MCFG_RDCOMB \
                         | USB_MCFG_SSDEN | USB_MCFG_UCAM \
                         | USB_MCFG_EBMEN | USB_MCFG_EMEMEN)
#else
#define USBH_ENABLE_INIT  (USBH_ENABLE_CE \
                         | USB_MCFG_PFEN | USB_MCFG_RDCOMB \
                         | USB_MCFG_SSDEN \
                         | USB_MCFG_EBMEN | USB_MCFG_EMEMEN)
#endif


static void NT725681_stop(struct platform_device *dev)
{
	//unsigned long count = 0;
	unsigned long flags;
	unsigned long reg;
	writel(0x2, USB1_EHCI_BASE+0x10);
	udelay(1000);

#if 0
	while(count < 10)
	{
		if(!KER_CLK_DownFlowSem())  
		{
			KER_CLK_SetClockReset(EN_KER_CLK_RST_AHB_USB1,  true);
			udelay(20);
			KER_CLK_SetClockReset(EN_KER_CLK_RST_AXI_USB1,  true);
			udelay(20);
			KER_CLK_SetClockReset(EN_KER_CLK_RST_CORE_USB1, true);    
			udelay(40);
			KER_CLK_SetClockReset(EN_KER_CLK_RST_AXI_USB1,  false);
			udelay(20);
			KER_CLK_SetClockReset(EN_KER_CLK_RST_AHB_USB1,  false);
			udelay(20);

			clear(USB1_APB, 0x1 << 8);
			udelay(100);
			//# select SUSPN and RST mask as 0
			clear(USB1_APB, 0x3 << 14);
			mdelay(1); 
			KER_CLK_SetClockReset(EN_KER_CLK_RST_CORE_USB1, false);
			KER_CLK_UpFlowSem();
			break;
		}
		printk(KERN_WARNING "%s usb reset retry\n",__func__);
		count ++;
	}
#endif
	reg = *(volatile unsigned long *)(0xBD020100);
	reg &= 0xFFFF;
	if(reg == 0x2569) {
		raw_spin_lock_irqsave(&clock_gen_lock, flags);
		*(volatile unsigned long *)(0xbd0200ac) = 0x80; //p-reset high
		udelay(20);
		*(volatile unsigned long *)(0xbd0200a8) = 0x80; //p-reset low
		udelay(20);

		//# select SUSPN and RST mask as 0
		clear(USB1_APB, 0x1 << 8);
		udelay(100);
		clear(USB1_APB, 0x3 << 14);
		mdelay(1);
		if(!(readl(0xbd020048)&0x08000000)) {
			clear(0xbd020048, 0x08000000);
			mdelay(1);
			set(0xbd020048,0x08000000);
			mdelay(1);
    }

		//Hreset
		*(volatile unsigned long *)(0xbd0200ac) = 0x10;
		udelay(20);
		*(volatile unsigned long *)(0xbd02008c) = 0x40;
		udelay(20);
		*(volatile unsigned long *)(0xbd02006C) = 0x20;
		udelay(20);
		*(volatile unsigned long *)(0xbd020068) = 0x20;
		udelay(20);
		*(volatile unsigned long *)(0xbd020088) = 0x40;
		udelay(20);
		*(volatile unsigned long *)(0xbd0200a8) = 0x10;
		udelay(20);
		raw_spin_unlock_irqrestore(&clock_gen_lock, flags);
	}else{
		raw_spin_lock_irqsave(&clock_gen_lock, flags);

		*(volatile unsigned long *)(0xbd02006C) = 0x20;
		udelay(20);

		//# set hclk sw reset
		*(volatile unsigned long *)(0xbd0200ac) = 0x10;
		udelay(20);
		//# set aclk sw reset
		*(volatile unsigned long *)(0xbd02008c) = 0x40;
		udelay(40);

		*(volatile unsigned long *)(0xbd020088) = 0x40;
		udelay(20);
		*(volatile unsigned long *)(0xbd0200a8) = 0x10;
		udelay(20);

		//# select SUSPN and RST mask as 0
		clear(USB1_APB, 0x1 << 8);
		udelay(100);
		clear(USB1_APB, 0x3 << 14);
		mdelay(1); 
		*(volatile unsigned long *)(0xbd020068) = 0x20;

		raw_spin_unlock_irqrestore(&clock_gen_lock, flags);

		mdelay(1); 
		if(!(readl(0xbd020048)&0x08000000)) {
			clear(0xbd020048, 0x08000000);
			mdelay(1);
			set(0xbd020048,0x08000000);
			mdelay(1);
		}		
	}
}

/*-------------------------------------------------------------------------*/

static void NT725681_init(void)
{
//	unsigned long count = 0;
	unsigned long flags;
	unsigned long reg;
#if 0
	while(count < 10)
	{
		if(!KER_CLK_DownFlowSem())  
		{
			KER_CLK_SetClockReset(EN_KER_CLK_RST_AHB_USB1,  true);
			udelay(20);
			KER_CLK_SetClockReset(EN_KER_CLK_RST_AXI_USB1,  true);
			udelay(20);
			KER_CLK_SetClockReset(EN_KER_CLK_RST_CORE_USB1, true);    
			udelay(40);
			KER_CLK_SetClockReset(EN_KER_CLK_RST_AXI_USB1,  false);
			udelay(20);
			KER_CLK_SetClockReset(EN_KER_CLK_RST_AHB_USB1,  false);
			udelay(20);

			clear(USB1_APB, 0x1 << 8);
			udelay(100);
			//# select SUSPN and RST mask as 0
			clear(USB1_APB, 0x3 << 14);
			mdelay(1); 
			KER_CLK_SetClockReset(EN_KER_CLK_RST_CORE_USB1, false);
			KER_CLK_UpFlowSem();
			break;
		}
		printk(KERN_WARNING "%s usb reset retry\n",__func__);
		count ++;
	}
#endif
	reg = *(volatile unsigned long *)(0xBD020100);
	reg &= 0xFFFF;
	if(reg == 0x2569) {
		raw_spin_lock_irqsave(&clock_gen_lock, flags);
		*(volatile unsigned long *)(0xbd0200ac) = 0x80; //p-reset high
		udelay(20);
		*(volatile unsigned long *)(0xbd0200a8) = 0x80; //p-reset low
		udelay(20);

		//# select SUSPN and RST mask as 0
		clear(USB1_APB, 0x1 << 8);
		udelay(100);
		clear(USB1_APB, 0x3 << 14);
		mdelay(1);
		if(!(readl(0xbd020048)&0x08000000)) {
			clear(0xbd020048, 0x08000000);
			mdelay(1);
			set(0xbd020048,0x08000000);
			mdelay(1);
    }

		//Hreset
		*(volatile unsigned long *)(0xbd0200ac) = 0x10;
		udelay(20);
		*(volatile unsigned long *)(0xbd02008c) = 0x40;
		udelay(20);
		*(volatile unsigned long *)(0xbd02006C) = 0x20;
		udelay(20);
		*(volatile unsigned long *)(0xbd020068) = 0x20;
		udelay(20);
		*(volatile unsigned long *)(0xbd020088) = 0x40;
		udelay(20);
		*(volatile unsigned long *)(0xbd0200a8) = 0x10;
		udelay(20);
		raw_spin_unlock_irqrestore(&clock_gen_lock, flags);
	}else{
		raw_spin_lock_irqsave(&clock_gen_lock, flags);

		*(volatile unsigned long *)(0xbd02006C) = 0x20;
		udelay(20);

		//# set hclk sw reset
		*(volatile unsigned long *)(0xbd0200ac) = 0x10;
		udelay(20);
		//# set aclk sw reset
		*(volatile unsigned long *)(0xbd02008c) = 0x40;
		udelay(40);

		*(volatile unsigned long *)(0xbd020088) = 0x40;
		udelay(20);
		*(volatile unsigned long *)(0xbd0200a8) = 0x10;
		udelay(20);

		//# select SUSPN and RST mask as 0
		clear(USB1_APB, 0x1 << 8);
		udelay(100);
		clear(USB1_APB, 0x3 << 14);
		mdelay(1); 
		*(volatile unsigned long *)(0xbd020068) = 0x20;

		raw_spin_unlock_irqrestore(&clock_gen_lock, flags);

		mdelay(1); 
		if(!(readl(0xbd020048)&0x08000000)) {
			clear(0xbd020048, 0x08000000);
			mdelay(1);
			set(0xbd020048,0x08000000);
			mdelay(1);
		}		
	}
	writel(0x20,(USB1_EHCI_BASE+0x100));
	writel(0x1300000e,(USB1_EHCI_BASE+0xe0));
	writel(readl(USB1_EHCI_BASE+0x84),USB1_EHCI_BASE+0x84);

	set(USB1_EHCI_BASE+0xc4,0x3);				 
	clear(USB1_EHCI_BASE+0x88,OTGC_INT_B_TYPE); 
	set(USB1_EHCI_BASE+0x88,OTGC_INT_A_TYPE);				 

	clear(USB1_EHCI_BASE+0x80,0x20);
	set(USB1_EHCI_BASE+0x80,0x10);
	//for phy power
	writel(0, USB1_APB+0x438);
	//for full speed device
	set(USB1_EHCI_BASE+0x80,1<<28);				 
}

/*-------------------------------------------------------------------------*/

/* configure so an HC device and id are always provided */
/* always called with process context; sleeping is OK */

/**
 * usb_ehci_NT725681_probe - initialize NT725681-based HCDs
 * Context: !in_interrupt()
 *
 * Allocates basic resources for this USB host controller, and
 * then invokes the start() method for the HCD associated with it
 * through the hotplug entry's driver_data.
 *
 */
static int usb_ehci_NT725681_probe(const struct hc_driver *driver,
			  struct usb_hcd **hcd_out, struct platform_device *dev)
{
	int retval;
	struct usb_hcd *hcd;
	struct ehci_hcd *ehci;

	NT725681_init();
	if (dev->resource[1].flags != IORESOURCE_IRQ) {
		pr_debug("resource[1] is not IORESOURCE_IRQ");
		retval = -ENOMEM;
	}
	hcd = usb_create_hcd(driver, &dev->dev, "NT725681");
	if (!hcd)
		return -ENOMEM;
	hcd->rsrc_start = dev->resource[0].start;
	hcd->rsrc_len = dev->resource[0].end - dev->resource[0].start + 1;

	if (!request_mem_region(hcd->rsrc_start, hcd->rsrc_len, "ehci_hcd_NT725681")) {
		pr_debug("request_mem_region failed");
		retval = -EBUSY;
		goto err1;
	}

	hcd->regs =(void __iomem*) hcd->rsrc_start;

	if (!hcd->regs) {
		pr_debug("ioremap failed");
		retval = -ENOMEM;
		goto err2;
	}

	ehci = hcd_to_ehci(hcd);
	ehci->caps = hcd->regs;
	ehci->regs = hcd->regs +
		HC_LENGTH(ehci, readl(&ehci->caps->hc_capbase));

	/* cache this readonly data; minimize chip reads */
	ehci->hcs_params = readl(&ehci->caps->hcs_params);
	/* ehci_hcd_init(hcd_to_ehci(hcd)); */
	ehci->apbs = (void __iomem*)USB1_APB;
	retval =
	    usb_add_hcd(hcd, dev->resource[1].start, IRQF_DISABLED | IRQF_SHARED);
	if (retval == 0)
		return retval;

	NT725681_stop(dev);
err2:
	release_mem_region(hcd->rsrc_start, hcd->rsrc_len);
err1:
	usb_put_hcd(hcd);
	return retval;
}

static void usb_ehci_NT725681_remove(struct usb_hcd *hcd, struct platform_device *dev)
{
	usb_remove_hcd(hcd);
	release_mem_region(hcd->rsrc_start, hcd->rsrc_len);
	usb_put_hcd(hcd);
	NT725681_stop(dev);

}

static void NT725681_patch(struct usb_hcd *hcd){
	struct ehci_hcd		*ehci = hcd_to_ehci(hcd);
	unsigned int	command = ehci_readl(ehci, &ehci->regs->command);
	int retval,retry=0;

	command |= CMD_RESET;
	ehci_writel(ehci, command, &ehci->regs->command);
	do{
		retval = handshake (ehci, &ehci->regs->command,
			    CMD_RESET, 0, 250 * 1000);
		retry ++;
	}while(retval &&retry <3);

	if (unlikely(retval !=0 && retry >= 3))
        	ehci_err(ehci, "reset fail!\n");

	command = ehci->command;

	ehci_writel(ehci, (command &~((unsigned int)(CMD_RUN|CMD_PSE|CMD_ASE))), &ehci->regs->command);
	ehci_writel(ehci, ehci->periodic_dma, &ehci->regs->frame_list);
	ehci_writel(ehci, (u32)ehci->async->qh_dma, &ehci->regs->async_next);
	retry=0;
	do{
		ehci_writel(ehci, INTR_MASK,
		    &ehci->regs->intr_enable); 
		retval = handshake (ehci, &ehci->regs->intr_enable,
			    INTR_MASK, INTR_MASK, 250 );
		retry ++;
	}while(retval != 0);

	if (unlikely(retval != 0))
        	ehci_err(ehci, "write fail!\n");
		
	set_bit(1, &hcd->porcd);
}



static int NT725681_ehci_init(struct usb_hcd *hcd)
{
	struct ehci_hcd		*ehci = hcd_to_ehci(hcd);
	u32			temp;
	int			retval;
	u32			hcc_params;
	struct ehci_qh_hw	*hw;

	spin_lock_init(&ehci->lock);

	ehci->need_io_watchdog = 1;
	init_timer(&ehci->watchdog);
	ehci->watchdog.function = ehci_watchdog;
	ehci->watchdog.data = (unsigned long) ehci;

	init_timer(&ehci->iaa_watchdog);
	ehci->iaa_watchdog.function = ehci_iaa_watchdog;
	ehci->iaa_watchdog.data = (unsigned long) ehci;

	/*
	 * hw default: 1K periodic list heads, one per frame.
	 * periodic_size can shrink by USBCMD update if hcc_params allows.
	 */
	ehci->periodic_size = DEFAULT_I_TDPS;
	INIT_LIST_HEAD(&ehci->cached_itd_list);
	INIT_LIST_HEAD(&ehci->cached_sitd_list);
	if ((retval = ehci_mem_init(ehci, GFP_KERNEL)) < 0)
		return retval;

	/* controllers may cache some of the periodic schedule ... */
	hcc_params = ehci_readl(ehci, &ehci->caps->hcc_params);
	if (HCC_ISOC_CACHE(hcc_params))		// full frame cache
		ehci->i_thresh = 8;
	else					// N microframes cached
		ehci->i_thresh = 2 + HCC_ISOC_THRES(hcc_params);

	ehci->reclaim = NULL;
	ehci->next_uframe = -1;
	ehci->clock_frame = -1;

	/*
	 * dedicate a qh for the async ring head, since we couldn't unlink
	 * a 'real' qh without stopping the async schedule [4.8].  use it
	 * as the 'reclamation list head' too.
	 * its dummy is used in hw_alt_next of many tds, to prevent the qh
	 * from automatically advancing to the next td after short reads.
	 */
	ehci->async->qh_next.qh = NULL;
	hw = ehci->async->hw;
	hw->hw_next = QH_NEXT(ehci, ehci->async->qh_dma);
	hw->hw_info1 = cpu_to_hc32(ehci, QH_HEAD);
	hw->hw_token = cpu_to_hc32(ehci, QTD_STS_HALT);
	hw->hw_qtd_next = EHCI_LIST_END(ehci);
	ehci->async->qh_state = QH_STATE_LINKED;
	hw->hw_alt_next = QTD_NEXT(ehci, ehci->async->dummy->qtd_dma);

	/* clear interrupt enables, set irq latency */
	if (log2_irq_thresh < 0 || log2_irq_thresh > 6)
		log2_irq_thresh = 0;
	temp = 1 << (16 + log2_irq_thresh);
	ehci->has_ppcd = 0;
	if (HCC_CANPARK(hcc_params)) {
		/* HW default park == 3, on hardware that supports it (like
		 * NVidia and ALI silicon), maximizes throughput on the async
		 * schedule by avoiding QH fetches between transfers.
		 *
		 * With fast usb storage devices and NForce2, "park" seems to
		 * make problems:  throughput reduction (!), data errors...
		 */
		if (park) {
			park = min(park, (unsigned) 3);
			temp |= CMD_PARK;
			temp |= park << 8;
		}
	}
	if (HCC_PGM_FRAMELISTLEN(hcc_params)) {
		/* periodic schedule size can be smaller than default */
		temp &= ~(3 << 2);
		temp |= (EHCI_TUNE_FLS << 2);
		switch (EHCI_TUNE_FLS) {
		case 0: ehci->periodic_size = 1024; break;
		case 1: ehci->periodic_size = 512; break;
		case 2: ehci->periodic_size = 256; break;
		default:	BUG();
		}
	}
	ehci->has_lpm = 0;
	ehci->command = temp;
	hcd->has_tt = 1;
	hcd->self.sg_tablesize = 0;
	return 0;
}

static const struct hc_driver ehci_NT725681_hc_driver = {
	.description =		hcd_name,
	.product_desc =		"Novatek NT725681",
	.hcd_priv_size =	sizeof(struct ehci_hcd),

	/*
	 * generic hardware linkage
	 */
	.irq =			ehci_irq,
	.flags =		HCD_MEMORY | HCD_USB2,

	/*
	 * basic lifecycle operations
	 */
	.reset =		NT725681_ehci_init,
	.start =		ehci_run,
	.stop =			ehci_stop,
	.shutdown =		ehci_shutdown,

	/*
	 * managing i/o requests and associated device resources
	 */
	.urb_enqueue =		ehci_urb_enqueue,
	.urb_dequeue =		ehci_urb_dequeue,
	.endpoint_disable =	ehci_endpoint_disable,
	.endpoint_reset =	ehci_endpoint_reset,

	/*
	 * scheduling support
	 */
	.get_frame_number =	ehci_get_frame,

	/*
	 * root hub support
	 */
	.hub_status_data =	ehci_hub_status_data,
	.hub_control =		ehci_hub_control,
	.bus_suspend =		ehci_bus_suspend,
	.bus_resume =		ehci_bus_resume,
	.relinquish_port =	ehci_relinquish_port,
	.port_handed_over =	ehci_port_handed_over,

	.clear_tt_buffer_complete	= ehci_clear_tt_buffer_complete,
	.port_nc = NT725681_patch,
};


/*-------------------------------------------------------------------------*/

static int ehci_hcd_NT725681_drv_probe(struct platform_device *pdev)
{
	struct usb_hcd *hcd = NULL;
	int ret;

	if (usb_disabled())
		return -ENODEV;

	ret = usb_ehci_NT725681_probe(&ehci_NT725681_hc_driver, &hcd, pdev);
	return ret;
}

static int ehci_hcd_NT725681_drv_remove(struct platform_device *pdev)
{
	struct usb_hcd *hcd = platform_get_drvdata(pdev);
	pr_debug("In ehci_hcd_NT725681_drv_remove\n");
	usb_ehci_NT725681_remove(hcd, pdev);
	platform_set_drvdata(pdev, NULL);

	return 0;
}

MODULE_ALIAS("NT725681-ehci");
static struct platform_driver ehci_hcd_NT725681_driver = {
	.probe = ehci_hcd_NT725681_drv_probe,
	.remove = ehci_hcd_NT725681_drv_remove,
	.driver = {
		.name = "NT725681-ehci",
		.bus = &platform_bus_type
	}
};

