/*
 * sdp1001fpga.c
 *
 * Copyright (C) 2010 Samsung Electronics.co
 * Ikjoon Jang <ij.jang@samsung.com>
 *
 */

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/list.h>
#include <linux/timer.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/platform_device.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/irq.h>
#include <asm/hardware/cache-l2x0.h>

#include <asm/io.h>
#include <asm/irq.h>

#include <mach/hardware.h>
#include <mach/ccep_soc.h>

unsigned int sdp_revision_id;

static struct map_desc sdp1001fpga_io_desc[] __initdata = {
/* ------------------------------------------- */
/* ------ special function register ---------- */
/* ------------------------------------------- */
// 0x3000_0000 ~ 0x3100_0000, 16Mib
 { 
	.virtual = VA_IO_BASE0,
	.pfn     = __phys_to_pfn(PA_IO_BASE0),
	.length  = (16 << 20),
	.type    = MT_DEVICE 
 },
};

static struct resource sdp_uart0_resource[] = {
	[0] = {
		.start 	= PA_UART_BASE,
		.end	= PA_UART_BASE + 0x30,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start 	= IRQ_UART,
		.end	= IRQ_UART,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct resource sdp_uart1_resource[] = {
	[0] = {
		.start 	= PA_UART_BASE + 0x40,
		.end	= PA_UART_BASE + 0x40 + 0x30,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start 	= IRQ_UART,
		.end	= IRQ_UART,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct resource sdp_uart2_resource[] = {
	[0] = {
		.start 	= PA_UART_BASE + 0x80,
		.end	= PA_UART_BASE + 0x80 + 0x30,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start 	= IRQ_UART,
		.end	= IRQ_UART,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct resource sdp_uart3_resource[] = {
	[0] = {
		.start 	= PA_UART_BASE + 0xC0,
		.end	= PA_UART_BASE + 0xC0 + 0x30,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start 	= IRQ_UART,
		.end	= IRQ_UART,
		.flags	= IORESOURCE_IRQ,
	},
};



/* EHCI host controller */
static struct resource sdp_ehci0_resource[] = {
        [0] = {
                .start  = PA_EHCI0_BASE,
                .end    = PA_EHCI0_BASE + 0x100,
                .flags  = IORESOURCE_MEM,
        },
        [1] = {
                .start  = IRQ_USB_EHCI0,
                .end    = IRQ_USB_EHCI0,
                .flags  = IORESOURCE_IRQ,
        },
};

static struct resource sdp_ehci1_resource[] = {
        [0] = {
                .start  = PA_EHCI1_BASE,
                .end    = PA_EHCI1_BASE + 0x100,
                .flags  = IORESOURCE_MEM,
        },
        [1] = {
                .start  = IRQ_USB_EHCI1,
                .end    = IRQ_USB_EHCI1,
                .flags  = IORESOURCE_IRQ,
        },
};

/* USB 2.0 companion OHCI */
static struct resource sdp_ohci0_resource[] = {
        [0] = {
                .start  = PA_OHCI0_BASE,
                .end    = PA_OHCI0_BASE + 0x100,
                .flags  = IORESOURCE_MEM,
        },
        [1] = {
                .start  = IRQ_USB_OHCI0,
                .end    = IRQ_USB_OHCI0,
                .flags  = IORESOURCE_IRQ,
        },
};

static struct resource sdp_ohci1_resource[] = {
        [0] = {
                .start  = PA_OHCI1_BASE,
                .end    = PA_OHCI1_BASE + 0x100,
                .flags  = IORESOURCE_MEM,
        },
        [1] = {
                .start  = IRQ_USB_OHCI1,
                .end    = IRQ_USB_OHCI1,
                .flags  = IORESOURCE_IRQ,
        },
};

static struct platform_device sdp_uart0 = {
	.name		= "sdp-uart",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(sdp_uart0_resource),
	.resource	= sdp_uart0_resource,
};
/*
static struct platform_device sdp_uart1 = {
	.name		= "sdp-uart",
	.id		= 1,
	.num_resources	= ARRAY_SIZE(sdp_uart1_resource),
	.resource	= sdp_uart1_resource,
};

static struct platform_device sdp_uart2 = {
	.name		= "sdp-uart",
	.id		= 2,
	.num_resources	= ARRAY_SIZE(sdp_uart2_resource),
	.resource	= sdp_uart2_resource,
};

static struct platform_device sdp_uart3 = {
	.name		= "sdp-uart",
	.id		= 3,
	.num_resources	= ARRAY_SIZE(sdp_uart3_resource),
	.resource	= sdp_uart3_resource,
};
*/
/* USB Host controllers */
static u64 sdp_ehci0_dmamask = (u32)0xFFFFFFFFUL;
static u64 sdp_ehci1_dmamask = (u32)0xFFFFFFFFUL;
static u64 sdp_ohci0_dmamask = (u32)0xFFFFFFFFUL;
static u64 sdp_ohci1_dmamask = (u32)0xFFFFFFFFUL;

static struct platform_device sdp_ehci0 = {
        .name           = "ehci-sdp",
        .id             = 0,
        .dev = {
                .dma_mask               = &sdp_ehci0_dmamask,
                .coherent_dma_mask      = 0xFFFFFFFFUL,
        },
        .num_resources  = ARRAY_SIZE(sdp_ehci0_resource),
        .resource       = sdp_ehci0_resource,
};

static struct platform_device sdp_ehci1 = {
        .name           = "ehci-sdp",
        .id             = 1,
        .dev = {
                .dma_mask               = &sdp_ehci1_dmamask,
                .coherent_dma_mask      = 0xFFFFFFFFUL,
        },
        .num_resources  = ARRAY_SIZE(sdp_ehci1_resource),
        .resource       = sdp_ehci1_resource,
};

static struct platform_device sdp_ohci0 = {
        .name           = "ohci-sdp",
        .id             = 0,
        .dev = {
                .dma_mask               = &sdp_ohci0_dmamask,
                .coherent_dma_mask      = 0xFFFFFFFFUL,
        },
        .num_resources  = ARRAY_SIZE(sdp_ohci0_resource),
        .resource       = sdp_ohci0_resource,
};

static struct platform_device sdp_ohci1 = {
        .name           = "ohci-sdp",
        .id             = 1,
        .dev = {
                .dma_mask               = &sdp_ohci1_dmamask,
                .coherent_dma_mask      = 0xFFFFFFFFUL,
        },
        .num_resources  = ARRAY_SIZE(sdp_ohci1_resource),
        .resource       = sdp_ohci1_resource,
};

static struct platform_device* sdp1001fpga_init_devs[] __initdata = {
	&sdp_uart0,
/*	&sdp_uart1,
	&sdp_uart2,
	&sdp_uart3,
       	&sdp_ehci0,
        &sdp_ohci0,
        &sdp_ehci1,
        &sdp_ohci1,*/
};

void __init sdp1001fpga_iomap_init (void)
{
	iotable_init(sdp1001fpga_io_desc, ARRAY_SIZE(sdp1001fpga_io_desc));
}

int sdp_get_revision_id(void)
{
	return sdp_revision_id;
}

#if defined(CONFIG_CACHE_L2X0)
#define L2C310_CR	0x100	/* control register (L2X0_CTRL) */
#define L2C310_TRLCR	0x108	/* tag RAM latency control */
#define L2C310_DRLCR	0x10C	/* data RAM latency control */
#define L2C310_PCR	0Xf60	/* prefetch control */

#define L2C310_PCR_DLF	(1<<30)	/* double linefill enable */
#define L2C310_PCR_IPF	(1<<29)	/* instruction prefetch enable */
#define L2C310_PCR_DPF	(1<<28)	/* data prefetch enable */
#define L2C310_PCR_DROP	(1<<24)	/* prefetch drop enable */
#define L2C310_PCR_IDLF	(1<<23)	/* incr double linefill enable */

static void __init sdp1001fpga_l2c_init(void)
{
	void __iomem *base = (void __iomem*)VA_L2C_BASE;
	u32 v;

	/* prefetch control */
	v = readl (base + L2C310_PCR);
	v |= L2C310_PCR_DLF | L2C310_PCR_IPF | L2C310_PCR_DPF;
	v |= L2C310_PCR_DROP | L2C310_PCR_IDLF;
	v &= ~0x1f;
	printk (KERN_CRIT "prefetch=0x%08x\n", v);
	writel (v, base + L2C310_PCR);

	/* tag RAM latency */
	writel (0, base + L2C310_TRLCR);

	/* data RAM letency */	
	writel (0, base + L2C310_DRLCR);

	/* Data prefetch enable, random replacement */
	l2x0_init (base, 0x10000000, ~(0x02000000));

}
#else
static void __init sdp1001fpga_l2c_init(void) {}
#endif

void __init sdp1001fpga_init(void)
{
	sdp1001fpga_l2c_init();
	platform_add_devices(sdp1001fpga_init_devs, ARRAY_SIZE(sdp1001fpga_init_devs));
}

