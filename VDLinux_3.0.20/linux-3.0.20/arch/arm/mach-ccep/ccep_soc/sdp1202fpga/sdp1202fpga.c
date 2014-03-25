/*
 * sdp1202fpga.c
 *
 * Copyright (C) 2011 Samsung Electronics.co
 * Seihee Chon <sh.chon@samsung.com>
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

#include <plat/sdp_mmc.h>

extern unsigned int sdp_revision_id;

static struct map_desc sdp1202fpga_io_desc[] __initdata = {
/* ------------------------------------------- */
/* ------ special function register ---------- */
/* ------------------------------------------- */
// 0x1800_0000 ~ 0x1900_0000, 16Mib
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
		.start 	= IRQ_UART0,
		.end	= IRQ_UART0,
		.flags	= IORESOURCE_IRQ,
	},
};

#if 0
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
#endif

static struct resource sdp_otg_resource[] = {
        [0] = {
                .start  = PA_OTG_BASE,
                .end    = PA_OTG_BASE + 0x10000 - 1,
                .flags  = IORESOURCE_MEM,
        },
        [1] = {
                .start  = IRQ_USB_OTG,
                .end    = IRQ_USB_OTG,
                .flags  = IORESOURCE_IRQ,
        },
};

static struct platform_device sdp_uart0 = {
	.name		= "sdp-uart",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(sdp_uart0_resource),
	.resource	= sdp_uart0_resource,
};

#if 0

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
#endif

/* USB device controller */
static u64 sdp_otg_dmamask = (u32)0xFFFFFFFFUL;
static struct platform_device sdp_otg = {
        .name           = "sdp-otg",
        .id             = 0,
        .dev = {
                .dma_mask               = &sdp_otg_dmamask,
                .coherent_dma_mask      = 0xFFFFFFFFUL,
        },
        .num_resources  = ARRAY_SIZE(sdp_otg_resource),
        .resource       = sdp_otg_resource,
};

#if 0
// add sdpGmac platform device by tukho.kim 20091205
#include <plat/sdp_gmac_reg.h>

// sdpGmac resource 
static struct resource sdpGmac_resource[] = {
        [0] = {
                .start  = PA_SDP_GMAC_BASE,
                .end    = PA_SDP_GMAC_BASE + sizeof(SDP_GMAC_T),
                .flags  = IORESOURCE_MEM,
        },

        [1] = {
                .start  = PA_SDP_GMAC_MMC_BASE,
                .end    = PA_SDP_GMAC_MMC_BASE + sizeof(SDP_GMAC_MMC_T),
                .flags  = IORESOURCE_MEM,
        },

        [2] = {
                .start  = PA_SDP_GMAC_TIME_STAMP_BASE,
                .end    = PA_SDP_GMAC_TIME_STAMP_BASE + sizeof(SDP_GMAC_TIME_STAMP_T),
                .flags  = IORESOURCE_MEM,
        },

        [3] = {
                .start  = PA_SDP_GMAC_MAC_2ND_BLOCK_BASE,
                .end    = PA_SDP_GMAC_MAC_2ND_BLOCK_BASE
                                + sizeof(SDP_GMAC_MAC_2ND_BLOCK_T),  // 128KByte
                .flags  = IORESOURCE_MEM,
        },

        [4] = {
                .start  = PA_SDP_GMAC_DMA_BASE,
                .end    = PA_SDP_GMAC_DMA_BASE + sizeof(SDP_GMAC_DMA_T),  // 128KByte
                .flags  = IORESOURCE_MEM,
        },

        [5] = {
                .start  = IRQ_SDP_GMAC,
                .end    = IRQ_SDP_GMAC,
                .flags  = IORESOURCE_IRQ,
        },
};

static struct platform_device sdpGmac_devs = {
        .name           = ETHER_NAME,
        .id             = 0,
        .num_resources  = ARRAY_SIZE(sdpGmac_resource),
        .resource       = sdpGmac_resource,
};
#endif
// add sdpGmac platform device by tukho.kim 20091205 end
//
// mmc
static int sdp1202fpga_mmch_init(SDP_MMCH_T *p_sdp_mmch)
{
	if(p_sdp_mmch->pm_is_valid_clk_delay == false)
	{
		/* save clk delay */
		//p_sdp_mmch->pm_clk_delay[0] = *(volatile u32*)(VA_IO_BASE0 + 0x00090934);
		p_sdp_mmch->pm_is_valid_clk_delay = true;
	}
	else
	{
		/* restore clk delay */
		//*(volatile u32*)(VA_IO_BASE0 + 0x00090934) = p_sdp_mmch->pm_clk_delay[0];
	}

	/* Fox-ap fpga MMCIF Master Enable */
	//0x104185d4[6] = 0x1
	/* TrustZone DMA Secure Mode */
	//0x11400804 = 0x3

	return 0;
}

static struct sdp_mmch_plat sdp_mmc_platdata = {
	.processor_clk = 34375000/8,/* input clock is PLL9 / 8 */
	.min_clk = 4212,
	.max_clk = 34375000/8,
	.caps =  (MMC_CAP_8_BIT_DATA | MMC_CAP_MMC_HIGHSPEED \
			| MMC_CAP_NONREMOVABLE/* | MMC_CAP_1_8V_DDR | MMC_CAP_UHS_DDR50*/),
	.init = sdp1202fpga_mmch_init,
//	.get_ro,
//	.get_cd,
//	.get_ocr,
};

static struct resource sdp_mmc_resource[] = {
        [0] = {
                .start  = PA_MMC_BASE,
                .end    = PA_MMC_BASE + 0x200,
                .flags  = IORESOURCE_MEM,
        },
        [1] = {
                .start  = IRQ_MMCIF,
                .end    = IRQ_MMCIF,
                .flags  = IORESOURCE_IRQ,
        },
};
static struct platform_device sdp_mmc = {
	.name		= "sdp-mmc",
	.id		= 0,
	.dev		= {
		.platform_data = &sdp_mmc_platdata,
	},
	.num_resources	= ARRAY_SIZE(sdp_mmc_resource),
	.resource	= sdp_mmc_resource,
};
#if 0
/* for sdp-smc device */
static struct resource sdp_smc_resource[] = {
	[0] = {		/* smcdma register */
		.start	= PA_SMCDMA_BASE,
		.end	= PA_SMCDMA_BASE + 0x1f,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {		/* smc bank 0 */
		.start	= 0,
		.end	= 0x1ffff,
		.flags	= IORESOURCE_MEM,
	},
};

static u64 sdp_smc_dmamask = 0xffffffffUL;
static struct platform_device sdp_smc = {
	.name		= "sdp-smc",
	.id		= 0,
	.dev = {
		.dma_mask = &sdp_smc_dmamask,
		.coherent_dma_mask = 0xffffffff,
	},
	.num_resources	= ARRAY_SIZE (sdp_smc_resource),
	.resource	= sdp_smc_resource,
};
#endif

static struct platform_device* sdp1202fpga_init_devs[] __initdata = {
	&sdp_uart0,
/*	&sdp_uart1,
	&sdp_uart2,
	&sdp_uart3,

	&sdp_ehci0,
	&sdp_ohci0,
	&sdp_ehci1,
	&sdp_ohci1,
// add sdpGmac platform device by tukho.kim 20091205
	&sdpGmac_devs,	
	&sdp_smc,
*/
	&sdp_mmc,
	&sdp_otg,
};

void __init sdp1202fpga_iomap_init (void)
{
	iotable_init(sdp1202fpga_io_desc, ARRAY_SIZE(sdp1202fpga_io_desc));
}

static int sdp1202fpga_get_revision_id(void)
{
#if 0	
	void __iomem *base = (void __iomem*) (0x30091000 + DIFF_IO_BASE0);
	unsigned int rev_data;
	int rev_id;

	writel(0x1F, base + 0x704);	
	while(readl(base + 0x700) != 0);
	while(readl(base + 0x700) != 0);
	readl(base + 0x708);
	rev_data = readl(base + 0x708);
	rev_data = (rev_data & 0xFC00) >> 10;
	printk("SDP Get Revision ID : %X ", rev_data);
	switch(rev_data)
	{
		case 0x31:		//ES 1
			rev_id = 1;
			break;			
		case 0x22:
			rev_id = 2;
			break;
		case 0:			//ES 0
		default:			
			rev_id = 0;
	}
	if((sdp_revision_id == 1) && (rev_id == 0))
		sdp_revision_id = 1;
	else
		sdp_revision_id = rev_id;

	printk("version ES%d\n", sdp_revision_id);
#endif
	return sdp_revision_id;
}

int sdp_get_revision_id(void)
{
	return sdp_revision_id;
}

void __init sdp1202fpga_init(void)
{
	sdp1202fpga_get_revision_id();
	platform_add_devices(sdp1202fpga_init_devs, ARRAY_SIZE(sdp1202fpga_init_devs));
}

