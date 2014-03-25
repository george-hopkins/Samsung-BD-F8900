/*
 * sdp1004.c
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
#include <linux/delay.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/irq.h>
#include <asm/hardware/cache-l2x0.h>

#include <asm/io.h>
#include <asm/irq.h>

#include <mach/hardware.h>
#include <mach/ccep_soc.h>

#include <plat/sdp_spi.h> // use for struct sdp_spi_info and sdp_spi_chip_ops by drain.lee
#include <linux/spi/spi.h>

#include <plat/sdp_nand.h>

#ifndef IRQ_UART0
#define IRQ_UART0 	IRQ_UART
#define IRQ_UART1 	IRQ_UART
#define IRQ_UART2 	IRQ_UART
#endif

static unsigned int sdp_revision_id;

static struct map_desc sdp1004_io_desc[] __initdata = {
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
		.start 	= IRQ_UART0,
		.end	= IRQ_UART0,
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
		.start 	= IRQ_UART1,
		.end	= IRQ_UART1,
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
		.start 	= IRQ_UART2,
		.end	= IRQ_UART2,
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
// add sdpGmac platform device by tukho.kim 20091205 end
//
// mmc

static struct resource sdp_mmc_resource[] = {
        [0] = {
                .start  = PA_MMC_BASE,
                .end    = PA_MMC_BASE + 0x200,
                .flags  = IORESOURCE_MEM,
        },
        [1] = {
                .start  = IRQ_SDMMC,
                .end    = IRQ_SDMMC,
                .flags  = IORESOURCE_IRQ,
        },
};
static struct platform_device sdp_mmc = {
	.name		= "sdp-mmc",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(sdp_mmc_resource),
	.resource	= sdp_mmc_resource,
};

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

/* SATA0 controller */
#define PA_SATA0_BASE                   (0x30010000)
#define VA_SATA0_BASE                   (PA_SATA0_BASE + DIFF_IO_BASE0)

/* SATA1 controller */
#define PA_SATA1_BASE                   (0x30018000)
#define VA_SATA1_BASE                   (PA_SATA1_BASE + DIFF_IO_BASE0)

/* SATA0 */
static struct resource sdp_sata0_resource[] = {
        [0] = {
                .start  = PA_SATA0_BASE,
                .end    = PA_SATA0_BASE + 0x200,
                .flags  = IORESOURCE_MEM,
        },
        [1] = {
                .start  = IRQ_SATA0,
                .end    = IRQ_SATA0,
                .flags  = IORESOURCE_IRQ,
        },
};

/* SATA1 */
static struct resource sdp_sata1_resource[] = {
        [0] = {
                .start  = PA_SATA1_BASE,
                .end    = PA_SATA1_BASE + 0x200,
                .flags  = IORESOURCE_MEM,
        },
        [1] = {
                .start  = IRQ_SATA1_PCIE,
                .end    = IRQ_SATA1_PCIE,
                .flags  = IORESOURCE_IRQ,
        },
};


static u64 sdp_sata0_dmamask = 0xFFFFFFFFUL;
static u64 sdp_sata1_dmamask = 0xFFFFFFFFUL;

static struct platform_device sdp_sata0 = {
        .name   =       "ahci-sata0",
        .id             =       0,
        .dev = {
        	.dma_mask               = &sdp_sata0_dmamask,
        	.coherent_dma_mask      = 0xFFFFFFFFUL,
        },
        .num_resources  = ARRAY_SIZE(sdp_sata0_resource),
        .resource       =       sdp_sata0_resource,
};

static struct platform_device sdp_sata1 = {
        .name   =       "ahci-sata1",
        .id             =       0,
    	.dev = {
        	.dma_mask               = &sdp_sata1_dmamask,
            .coherent_dma_mask      = 0xFFFFFFFFUL,
    	},
        .num_resources  = ARRAY_SIZE(sdp_sata1_resource),
        .resource       =       sdp_sata1_resource,
};

/* add nand flash, drain.lee 20110602 */
struct sdp_nand_platform __initdata sdp_nand_palt_data = {
	.tacls = 2,
	.twrph0= 3,
	.twrph1= 1,
};
 
static struct resource sdp_nand_resource[] = {
	[0] = {
		.start = PA_NAND_BASE,
		.end   = PA_NAND_BASE + 0x0218,
		.flags = IORESOURCE_MEM,
	}
};
struct platform_device sdp_nand = {
	.name		  = "sdp-nand",
	.id		  = -1,
	.dev = {
		.platform_data = &sdp_nand_palt_data,
	},
	.num_resources	  = ARRAY_SIZE(sdp_nand_resource),
	.resource	  = sdp_nand_resource,
};


/* SPI master controller */
// add by drain.lee@samsung.com  20110119
static struct resource sdp_spi_resource[] = 
{
	[0] = {
			.start = PA_SPI_BASE,
			.end = PA_SPI_BASE + 0x18 - 1,
			.flags = IORESOURCE_MEM,
		},
	[1] = {
			.start = IRQ_SPI,
			.end = IRQ_SPI,
			.flags = IORESOURCE_IRQ,
		},
};

/* SPI Master Controller drain.lee */
static struct sdp_spi_info sdp_spi_master_data = {
	.num_chipselect = 1,
};

static struct platform_device sdp_spi = {
	.name		= "sdp-spi",
	.id		= 0,
	.dev		= {
		.platform_data = &sdp_spi_master_data,
	},
	.num_resources	= ARRAY_SIZE(sdp_spi_resource),
	.resource	= sdp_spi_resource,
};

//add board fixed spi devices info
static struct spi_board_info sdp_spi_spidevices[] = {
	[0] = {// sdp_spidev0.0
			.modalias = "sdp-spidev",
			.controller_data = NULL,
			.max_speed_hz = 10 * 1000000,
			.bus_num = 0,
			.chip_select = 0,
			.mode = SPI_MODE_0,
		},
};


/*     init dev array      */
static struct platform_device* sdp1004_init_devs[] __initdata = {
		&sdp_uart0,
		&sdp_uart1,
		&sdp_uart2,
       	&sdp_ehci0,
        &sdp_ohci0,
        &sdp_ehci1,
        &sdp_ohci1,
// add sdpGmac platform device by tukho.kim 20091205
        &sdpGmac_devs,	
#ifdef CONFIG_MTD_NAND_SDP
		/* current sdp_smc + sdp_nand not working,,, */
		&sdp_nand,
#else
		&sdp_mmc,
		&sdp_smc,
#endif
		&sdp_sata0, 
		&sdp_sata1, 
		&sdp_spi,
};

void __init sdp1004_iomap_init (void)
{
	iotable_init(sdp1004_io_desc, ARRAY_SIZE(sdp1004_io_desc));
}

static int sdp1004_get_revision_id(void)
{
	void __iomem *base = (void __iomem*) (0x30000000 + DIFF_IO_BASE0);
	unsigned int rev_data1, rev_data2;
	int rev_id,a;

	writel(0x7, base + 0x4);
	while(readl(base + 0x0));
	rev_data1 = readl(base + 0x8);
	rev_data2 = readl(base + 0xC);
	printk("SDP Get Revision ID : %08X%08X ", rev_data1, rev_data2);

	if(rev_data1 == 0 && rev_data2 == 0)
		sdp_revision_id = 0;
	else
		sdp_revision_id = 1;

	printk("version ES%d\n", sdp_revision_id);
	return sdp_revision_id;
}

int sdp_get_revision_id(void)
{
	return sdp_revision_id;
}

#if defined(CONFIG_CACHE_L2X0)
#define L2C310_CR		0x100	/* control register (L2X0_CTRL) */
#define L2C310_TRLCR	0x108	/* tag RAM latency control */
#define L2C310_DRLCR	0x10C	/* data RAM latency control */
#define L2C310_PCR		0Xf60	/* prefetch control */

#define L2C310_PCR_DLF	(1<<30)	/* double linefill enable */
#define L2C310_PCR_IPF	(1<<29)	/* instruction prefetch enable */
#define L2C310_PCR_DPF	(1<<28)	/* data prefetch enable */
#define L2C310_PCR_DROP	(1<<24)	/* prefetch drop enable */
#define L2C310_PCR_IDLF	(1<<23)	/* incr double linefill enable */

#define L2C310_AUX_IPF	(1<<29)	/* inst prefetch enable */
#define L2C310_AUX_DPF	(1<<28)	/* data prefetch enable */
#define L2C310_AUX_RR	(1<<25) /* cache replacement policy */

static void __init sdp1004_l2c_init(void)
{
	void __iomem *base = (void __iomem*)VA_L2C_BASE;
	u32 v;

	/* prefetch control */
	v = readl (base + L2C310_PCR);
	v |= L2C310_PCR_IPF | L2C310_PCR_DPF;
	v |= L2C310_PCR_DROP | L2C310_PCR_IDLF;
#ifndef CONFIG_SDP_ARM_PL310_ERRATA_752271
	if(sdp_get_revision_id() > 0)
		v |= L2C310_PCR_DLF;
	else
		v &= ~(L2C310_PCR_DLF);
#endif

	v &= ~0x1f;
	printk (KERN_CRIT "prefetch=0x%08x\n", v);
	writel (v, base + L2C310_PCR);

	/* tag RAM latency */
	writel (0, base + L2C310_TRLCR);

	/* data RAM letency */	
	writel (0, base + L2C310_DRLCR);

	/* Data/Inst prefetch enable, random replacement */
	l2x0_init (base, L2C310_AUX_IPF | L2C310_AUX_DPF, ~(L2C310_AUX_RR));
}
#else	/* CONFIG_SDP1001_L2C310 */
static void __init sdp1004_l2c_init(void) {}
#endif

void __init sdp1004_init(void)
{
	sdp1004_get_revision_id();
	sdp1004_l2c_init();
	platform_add_devices(sdp1004_init_devs, ARRAY_SIZE(sdp1004_init_devs));

	//add by drain.lee
	spi_register_board_info(sdp_spi_spidevices, ARRAY_SIZE(sdp_spi_spidevices));
}

