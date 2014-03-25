/*
 *  linux/arch/arm/mach-sdp/pcie.c
 *
 *  Copyright (C) 2009 Samsung electronics.co
 *  created by tukho.kim@samsung.com
 */
#include <linux/kernel.h>
#include <linux/pci.h>
#include <linux/ptrace.h>
#include <linux/interrupt.h>
#include <linux/init.h>

#include <asm/irq.h>
#include <asm/system.h>

/* arm dependent */
#include <asm/mach/pci.h>
#include <asm/mach-types.h>

/* machine dependent */
#include <mach/platform.h>

extern int 
sdp_read_config (struct pci_bus *bus, unsigned int devfn, int where, int size, u32 *val);
int 
sdp_write_config (struct pci_bus *bus, unsigned int devfn, int where, int size, u32 val);

static struct pci_ops pci_sdp_ops = {
	.read	= sdp_read_config,
	.write	= sdp_write_config,
};

struct pci_bus *sdp_pcie_scan_bus(int nr, struct pci_sys_data *sys)
{
	return pci_scan_bus(sys->busnr, &pci_sdp_ops, sys);
}

int __init sdp_pcie_map_irq(struct pci_dev *dev, u8 slot, u8 pin)
{
	return IRQ_PCI;
}

extern int sdp_pcie_setup(int nr, struct pci_sys_data *sys);
extern void sdp_pcie_preinit(void);
extern void sdp_pcie_postinit(void);

static struct hw_pci sdp_pcie __initdata = {
	.nr_controllers		= 1,
	.map_irq		= sdp_pcie_map_irq,
	.setup			= sdp_pcie_setup,
	.scan			= sdp_pcie_scan_bus,
	.preinit		= sdp_pcie_preinit,
	.postinit               = sdp_pcie_postinit,
	.swizzle		= pci_std_swizzle,
};

extern void pci_common_init(struct hw_pci *hw);

static int __init sdp_pcie_init(void)
{
	pci_common_init(&sdp_pcie);
	return 0;
}

subsys_initcall(sdp_pcie_init);
