/*
 * sdp1202.c
 *
 * Copyright (C) 2012 Samsung Electronics.co
 * SeungJun Heo <seungjun.heo@samsung.com>
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
#include <linux/regulator/machine.h>
#include <linux/regulator/tps54921.h>
#include <linux/regulator/sn1202033.h>
#include <linux/platform_data/sdp-hsotg.h>
#include <linux/proc_fs.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/irq.h>
#include <asm/hardware/cache-l2x0.h>

#include <asm/io.h>
#include <asm/irq.h>
#include <asm/delay.h>

#include <mach/hardware.h>
#include <mach/ccep_soc.h>

#include <plat/sdp_spi.h> // use for struct sdp_spi_info and sdp_spi_chip_ops by drain.lee
#include <linux/spi/spi.h>
#include <asm/pmu.h>
#include <plat/sdp_mmc.h>
#include <plat/sdp_tmu.h>

int sdp_get_revision_id(void);

extern unsigned int sdp_revision_id;
extern bool sdp_is_dual_mp;

static struct map_desc sdp1202_io_desc[] __initdata = {
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

static struct resource sdp_uart3_resource[] = {
	[0] = {
		.start 	= PA_UART_BASE + 0xC0,
		.end	= PA_UART_BASE + 0xC0 + 0x30,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start 	= IRQ_UART3,
		.end	= IRQ_UART3,
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

/* xHCI host controller */
static struct resource sdp_xhci0_resource[] = {
        [0] = {
                .start  = PA_XHCI0_BASE,
                .end    = PA_XHCI0_BASE + 0xC700,
                .flags  = IORESOURCE_MEM,
        },
        [1] = {
                .start  = IRQ_USB3,
                .end    = IRQ_USB3,
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
static u64 sdp_xhci0_dmamask = (u32)0xFFFFFFFFUL;

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

static struct platform_device sdp_xhci0 = {
        .name           = "xhci-sdp",
        .id             = 0,
        .dev = {
                .dma_mask               = &sdp_xhci0_dmamask,
                .coherent_dma_mask      = 0xFFFFFFFFUL,
        },
        .num_resources  = ARRAY_SIZE(sdp_xhci0_resource),
        .resource       = sdp_xhci0_resource,
};


/* USB device controller */
static int sdp1202_otg_phy_init(void)
{
	unsigned int reg = VA_IO_BASE0 + 0x90958;
	unsigned int val;
	val = readl(reg);
	val |= 0x1;
	writel(val, reg);
#if 0	
	u32 val;
	int timeout = 1000;
	
	/* rst_usb2_otg_por */
	val = readl(VA_IO_BASE0 + 0x90958);
	val &= ~(1<<0);
	writel(val, VA_IO_BASE0 + 0x90958);

	/* rstn_usb2_otg, rstn_usb2_otg_prst */
	/* disable ehci0 */
	val = readl(VA_IO_BASE0 + 0x90954);
	val &= ~(1<<0 | 1<<1 | 1<<2);
	writel(val, VA_IO_BASE0 + 0x90954);

	/* host/otg selection */
	val = readl(VA_IO_BASE0 + 0x90c74);
	val &= ~(1<<29);
	writel(val, VA_IO_BASE0 + 0x90c74);

	/* release sw reset */
	/* rst_usb2_otg_por */
	val = readl(VA_IO_BASE0 + 0x90958);
	val |= 1<<0;
	writel(val, VA_IO_BASE0 + 0x90958);
	/* rstn_usb2_otg_prst */
	val = readl(VA_IO_BASE0 + 0x90954);
	val |= 1<<2;
	writel(val, VA_IO_BASE0 + 0x90954);

	/* wait otg_prst */
	while (timeout--) {
		val = readl(VA_IO_BASE0 + 0x90c84);
		if ((val & 0x1) == 1)
			break;
		udelay(1);
	}

	/* release sw reset */
	/* rstn_usb2_otg */
	val = readl(VA_IO_BASE0 + 0x90954);
	val |= 1<<1;
	writel(val, VA_IO_BASE0 + 0x90954);

	/* usb1 phy tune */
	writel(0x6330dc95, VA_IO_BASE0 + 0x90c6c);

	/* usb20_reg_1 [12] COMMONONN */
	val = readl(VA_IO_BASE0 + 0x90c74);
	val |= 0x1000;
	writel(val, VA_IO_BASE0 + 0x90c74);

	/* usb20_reg_2 [3:2] TXPREEMPAMPTUNE0 */
	val = readl(VA_IO_BASE0 + 0x90c78);
	val |= 0x5;
	writel(val, VA_IO_BASE0 + 0x90c78);
#endif

	return 0;
}

static int sdp1202_otg_phy_exit(void)
{
	unsigned int reg = VA_IO_BASE0 + 0x90958;
	unsigned int val;
	val = readl(reg);
	val &= ~0x1U;
	writel(val, reg);
	
	return 0;
}

static struct resource sdp_otg_resource[] = {
        [0] = {
                .start  = PA_OTG_BASE,
                .end    = PA_OTG_BASE + 0x10000,//SZ_4K - 1,
                .flags  = IORESOURCE_MEM,
        },
        [1] = {
                .start  = IRQ_USB_OTG,
                .end    = IRQ_USB_OTG,
                .flags  = IORESOURCE_IRQ,
        },
};

static struct s3c_hsotg_plat sdp_otg_platdata = {
	.dma = S3C_HSOTG_DMA_DRV,
	.is_osc = 0,
	.phy_init = sdp1202_otg_phy_init,
	.phy_exit = sdp1202_otg_phy_exit,
};

static u64 sdp_otg_dmamask = (u32)0xFFFFFFFFUL;
static struct platform_device sdp_otg = {
        .name           = "sdp-otg",
        .id             = 0,
        .dev = {
                .dma_mask               = &sdp_otg_dmamask,
                .coherent_dma_mask      = 0xFFFFFFFFUL,
                .platform_data = &sdp_otg_platdata,
        },
        .num_resources  = ARRAY_SIZE(sdp_otg_resource),
        .resource       = sdp_otg_resource,
};

// add sdpGmac platform device by tukho.kim 20091205
#include <plat/sdp_gmac_reg.h>

/*
	fox ap pad ctrl
	0x10090CC4[0] : RGMII Sel
	0x10090CC8[0] : SRAM Sel
*/
static int sdp1202_gmac_init(void)
{
	/* SRAM select*/
	writel(readl(VA_IO_BASE0+0x00090CD4) | 0x1, VA_IO_BASE0+0x00090CD4);
	return  0;
}

static struct sdp_gmac_plat sdp_gmac_platdata =  {
	.init = sdp1202_gmac_init,/* board specipic init */
	.phy_mask = 0x0,/*bit filed 1 is masked */
	.napi_weight = 128,/* netif_napi_add() weight value. 1 ~ max */
};

// sdpGmac resource 
static struct resource sdpGmac_resource[] = {
        [0] = {
                .start  = PA_GMAC_BASE + SDP_GMAC_BASE,
                .end    = PA_GMAC_BASE + SDP_GMAC_BASE + sizeof(SDP_GMAC_T),
                .flags  = IORESOURCE_MEM,
        },

        [1] = {
                .start  = PA_GMAC_BASE + SDP_GMAC_MMC_BASE,
                .end    = PA_GMAC_BASE + SDP_GMAC_MMC_BASE + sizeof(SDP_GMAC_MMC_T),
                .flags  = IORESOURCE_MEM,
        },

        [2] = {
                .start  = PA_GMAC_BASE + SDP_GMAC_TIME_STAMP_BASE,
                .end    = PA_GMAC_BASE + SDP_GMAC_TIME_STAMP_BASE + sizeof(SDP_GMAC_TIME_STAMP_T),
                .flags  = IORESOURCE_MEM,
        },

        [3] = {
                .start  = PA_GMAC_BASE + SDP_GMAC_MAC_2ND_BLOCK_BASE,
                .end    = PA_GMAC_BASE + SDP_GMAC_MAC_2ND_BLOCK_BASE
                                + sizeof(SDP_GMAC_MAC_2ND_BLOCK_T),  // 128KByte
                .flags  = IORESOURCE_MEM,
        },

        [4] = {
                .start  = PA_GMAC_BASE + SDP_GMAC_DMA_BASE,
                .end    = PA_GMAC_BASE + SDP_GMAC_DMA_BASE + sizeof(SDP_GMAC_DMA_T),  // 128KByte
                .flags  = IORESOURCE_MEM,
        },

        [5] = {
                .start  = IRQ_EMAC,
                .end    = IRQ_EMAC,
                .flags  = IORESOURCE_IRQ,
        },
};

static u64 sdp_mac_dmamask = DMA_BIT_MASK(32);

static struct platform_device sdpGmac_devs = {
        .name           = ETHER_NAME,
        .id             = 0,
        .dev		= {
			.dma_mask = &sdp_mac_dmamask,
			.coherent_dma_mask = DMA_BIT_MASK(32),
			.platform_data = &sdp_gmac_platdata,
		},
        .num_resources  = ARRAY_SIZE(sdpGmac_resource),
        .resource       = sdpGmac_resource,
};
// add sdpGmac platform device by tukho.kim 20091205 end
//
// mmc

/*
	driving: 0x1009_092C[31:16]
	sample:  0x1009_092C[15:00]
*/
static int sdp1202_mmch_init(SDP_MMCH_T *p_sdp_mmch)
{
	int revision;
	if(p_sdp_mmch->pm_is_valid_clk_delay == false)
	{
		/* save clk delay */
		p_sdp_mmch->pm_clk_delay[0] = readl(VA_IO_BASE0+0x0009092C);
		p_sdp_mmch->pm_is_valid_clk_delay = true;
	}
	else
	{
		/* restore clk delay */
		writel(p_sdp_mmch->pm_clk_delay[0], VA_IO_BASE0+0x0009092C);
	}

	/* sdp1202 evt0 fixup. using pio mode! */
	revision = sdp_get_revision_id();
	if(revision == 0) {
		struct sdp_mmch_plat *platdata;
		platdata = dev_get_platdata(&p_sdp_mmch->pdev->dev);
		platdata->force_pio_mode = true;
	}
	return 0;
}

static struct sdp_mmch_plat sdp_mmc_platdata = {
	.processor_clk = 100000000,
	.min_clk = 400000,
	.max_clk = 50000000,
	.caps =  (MMC_CAP_8_BIT_DATA | MMC_CAP_MMC_HIGHSPEED \
			| MMC_CAP_NONREMOVABLE | MMC_CAP_1_8V_DDR | MMC_CAP_UHS_DDR50),
	.init = sdp1202_mmch_init,
	.fifo_depth = 128,
//	.get_ro,
//	.get_cd,
//	.get_ocr,
};

static struct resource sdp_mmc_resource[] = {
        [0] = {
                .start  = PA_MMC_BASE,
		.end    = PA_MMC_BASE + 0x800 -1,
                .flags  = IORESOURCE_MEM,
        },
        [1] = {
                .start  = IRQ_MMCIF,
                .end    = IRQ_MMCIF,
                .flags  = IORESOURCE_IRQ,
        },
};

static u64 sdp_mmc_dmamask = DMA_BIT_MASK(32);

static struct platform_device sdp_mmc = {
	.name		= "sdp-mmc",
	.id		= 0,
	.dev		= {
		.dma_mask = &sdp_mmc_dmamask,
		.coherent_dma_mask = DMA_BIT_MASK(32),
		.platform_data = &sdp_mmc_platdata,
	},
	.num_resources	= ARRAY_SIZE(sdp_mmc_resource),
	.resource	= sdp_mmc_resource,
};


/* SPI master controller */

#define REG_MSPI_TX_CTRL		0x10090268// set 0x1000 for sel SPI
#define MSPI_TX_CTRL_SELSPI		(0x01 << 12)
#define REG_PAD_CTRL_17			VA_PADCTRL_BASE+0x44/* Pad select for SPI */
#define PAD_CTRL_17_SPI_SEL		(0x1<<0)
static int sdp1202_spi_init(void)
{
	int tmp;
	//MSPI Reg TX_CTRL sel_spi
	tmp = readl((REG_MSPI_TX_CTRL + DIFF_IO_BASE0));
	tmp |= MSPI_TX_CTRL_SELSPI;
	writel(tmp, (REG_MSPI_TX_CTRL + DIFF_IO_BASE0));

#if 0/* comment for UD board */
	//Pad control Main/Sub Function Selection REGISTER  bit 0 set SPI use SPI_SEL
	tmp = readl(REG_PAD_CTRL_17);
	tmp |= PAD_CTRL_17_SPI_SEL;
	writel(tmp, REG_PAD_CTRL_17);
#endif
	return 0;
}

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
	.num_chipselect = CONFIG_SDP_SPI_NUM_OF_CS,
	.init = sdp1202_spi_init,
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

/* thermal management */
static struct resource sdp_tmu_resource[] = {
	[0] = {		/* tsc register */
		.start	= PA_TSC_BASE,
		.end	= PA_TSC_BASE + 0x20,
		.flags	= IORESOURCE_MEM,
	},
};

static struct sdp_platform_tmu sdp_tmu_data = {
	.ts = {
		.start_zero_throttle = 40,
		.stop_zero_throttle  = 45,
		.stop_1st_throttle   = 102,
		.start_1st_throttle  = 110,
		.stop_2nd_throttle   = 112,
		.start_2nd_throttle  = 118,
		.stop_3rd_throttle   = 120,
		.start_3rd_throttle  = 125,
	},
	.cpufreq = {
		.limit_1st_throttle = 1000000,
		.limit_2nd_throttle = 600000,
		.limit_3rd_throttle	= 200000,
	},
	.gpufreq = {
		.limit_1st_throttle = 200000,
		.limit_2nd_throttle = 200000,
		.limit_3rd_throttle = 200000,
	},
};

static u64 sdp_tmu_dmamask = 0xffffffffUL;
static struct platform_device sdp_tmu = {
	.name	= "sdp-tmu",
	.id		= 0,
	.dev = {
		.dma_mask = &sdp_tmu_dmamask,
		.coherent_dma_mask = 0xffffffff,
		.platform_data = &sdp_tmu_data,
	},
	.num_resources	= ARRAY_SIZE (sdp_tmu_resource),
	.resource	= sdp_tmu_resource,
};

/* Regulator */
/* tps54921 */
static struct regulator_consumer_supply tps54921_supply = {
		.supply = "VDD_ARM",
		.dev_name = NULL,
};

static struct regulator_init_data tps54921_data = {
	.constraints	= {
		.name		= "VDD_ARM range",
		.min_uV		= 720000,
		.max_uV		= 1480000,
		.always_on	= 1,
		.boot_on	= 1,
		.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE,
	},
	.num_consumer_supplies	= 1,
	.consumer_supplies	= &tps54921_supply,
};

static struct tps54921_platform_data tps54921_regulator = {
		.i2c_addr	= 0x6E,
#ifdef CONFIG_ARCH_SDP1202_EVAL		
		.i2c_port	= 3,
#else
		.i2c_port	= 2,
#endif
		.init_data	= &tps54921_data,
};

static struct platform_device sdp_pmic_tps54921 = {
	.name	= "sdp_tps54921",
	.id		= 0,
	.dev	= {
		.platform_data = &tps54921_regulator,
	},
};

/* sn1202033 */
static struct regulator_consumer_supply sn1202033_supply[] = {
	{
#ifdef CONFIG_ARCH_SDP1202_EVAL		
		.supply = "VDD_GPU",
#else
		.supply = "VDD_MP0",
#endif
		.dev_name = NULL,
	},
	{
#ifdef CONFIG_ARCH_SDP1202_EVAL		
		.supply = "VDD_MP0",
#else
		.supply = "VDD_GPU",
#endif
		.dev_name = NULL,
	},
	/* for dual MP board */
	{
		.supply = "VDD_MP1",
		.dev_name = NULL,
	},

};

static struct regulator_init_data sn1202033_data[] = {
	[0] = {
		.constraints	= {
#ifdef CONFIG_ARCH_SDP1202_EVAL			
			.name		= "VDD_GPU range",
#else
			.name		= "VDD_MP0 range",
#endif
			.min_uV		= 680000,
			.max_uV		= 1950000,
			.always_on	= 1,
			.boot_on	= 1,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE,
		},
		.num_consumer_supplies = 1,
		.consumer_supplies = &sn1202033_supply[0],
	},
	[1] = {
		.constraints	= {
#ifdef CONFIG_ARCH_SDP1202_EVAL			
			.name		= "VDD_MP0 range",
#else
			.name		= "VDD_GPU range",
#endif
			.min_uV		= 680000,
			.max_uV		= 1950000,
			.always_on	= 1,
			.boot_on	= 1,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE,
		},
		.num_consumer_supplies = 1,
		.consumer_supplies = &sn1202033_supply[1],
	},
	/* for dual MP board */
	[2] = {
		.constraints	= {
			.name		= "VDD_MP1 range",
			.min_uV		= 680000,
			.max_uV		= 1950000,
			.always_on	= 1,
			.boot_on	= 1,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE,
		},
		.num_consumer_supplies = 1,
		.consumer_supplies = &sn1202033_supply[2],
	},
};

static struct sn1202033_platform_data sn1202033_regulator[] = {
	[0] = {
		.i2c_addr	= 0xC0,
#ifdef CONFIG_ARCH_SDP1202_EVAL
		.i2c_port	= 4,
#else
		.i2c_port	= 3,
#endif
		.def_volt	= 1100000,
		.vout_port	= 1,		
		.init_data	= &sn1202033_data[0],
	},
	[1] = {
		.i2c_addr	= 0xC0,
#ifdef CONFIG_ARCH_SDP1202_EVAL
		.i2c_port	= 4,
#else
		.i2c_port	= 3,
#endif
		.def_volt	= 1100000,
		.vout_port	= 2,		
		.init_data	= &sn1202033_data[1],
	},
	/* for dual MP board */
	[2] = {
		.i2c_addr	= 0xC4,
		.i2c_port	= 3,
		.def_volt	= 1100000,
		.vout_port	= 1,		
		.init_data	= &sn1202033_data[2],
	},
};

static struct platform_device sdp_pmic_sn1202033[] = {
	[0] = {
		.name	= "sdp_sn1202033",
		.id		= 0,
		.dev	= {
			.platform_data = &sn1202033_regulator[0],
		},
	},
	[1] = {
		.name	= "sdp_sn1202033",
		.id		= 1,
		.dev	= {
			.platform_data = &sn1202033_regulator[1],
		},
	},
	/* for dual MP borad */
	[2] = {
		.name	= "sdp_sn1202033",
		.id		= 2,
		.dev	= {
			.platform_data = &sn1202033_regulator[2],
		},
	},
};
/* end Regulator */
static struct resource pmu_resources[] =
{
        [0] = {
                        .start = IRQ_PMU_CPU0,
                        .end = IRQ_PMU_CPU0,
                        .flags = IORESOURCE_IRQ,
                },
        [1] = {
                        .start = IRQ_PMU_CPU1,
                        .end = IRQ_PMU_CPU1,
                        .flags = IORESOURCE_IRQ,
                },
        [2] = {
                        .start = IRQ_PMU_CPU2,
                        .end = IRQ_PMU_CPU2,
                        .flags = IORESOURCE_IRQ,
                },
        [3] = {
                        .start = IRQ_PMU_CPU3,
                        .end = IRQ_PMU_CPU3,
                        .flags = IORESOURCE_IRQ,
                },
};

static struct platform_device pmu_device = {
        .name   = "arm-pmu",
        .id             = ARM_PMU_DEVICE_CPU,
        .num_resources  = ARRAY_SIZE(pmu_resources),
        .resource = pmu_resources,
};

static struct platform_device* sdp1202_init_devs[] __initdata = {
	&sdp_uart0,
	&sdp_uart1,
	&sdp_uart2,
	&sdp_uart3,
	&sdp_otg,	
	&sdp_ehci0,
	&sdp_ohci0,
	&sdp_ehci1,
	&sdp_ohci1,
	&sdp_xhci0,
// add sdpGmac platform device by tukho.kim 20091205
	&sdpGmac_devs,	
	&sdp_mmc,
	&sdp_spi,
	&sdp_tmu,
	&sdp_pmic_tps54921,
	&sdp_pmic_sn1202033[0],
	&sdp_pmic_sn1202033[1],
	&pmu_device,
};



/* amba devices */
#include <linux/amba/bus.h>
/* for dma330 */
#include <plat/sdp_dma330.h>
static u64 sdp_dma330_dmamask = 0xffffffffUL;


#ifdef CONFIG_SDP_CLOCK_GATING

extern int sdp_set_clockgating(u32 phy_addr, u32 mask, u32 value);

static int sdp1202_dma330_clk_used_cnt = 0;

/*SDP1202, DMA330_0 and DMA330_1 use same clk source. */
static int sdp1202_dma330_gate_clock(void) {
	/*
		0x10090954[10]		//S/W Reset DMA330
		0x10090944[7]=0		//DMA330 clk Disable of dma module
		0x10090944[6]=0		//DMA330 p-clk Disable of dma module
	*/
	sdp_set_clockgating(0x10090954, 1<<10, 0);
	sdp_set_clockgating(0x10090944, 1<<7, 0);
	sdp_set_clockgating(0x10090944, 1<<6, 0);
	return 0;
}

static int sdp1202_dma330_ungate_clock(void) {
	/*
		0x10090944[6]=0		//DMA330 p-clk Enable of dma module
		0x10090944[7]=0		//DMA330 clk Enable of dma module
		0x10090954[10]		//S/W Reset DMA330
	*/
	sdp_set_clockgating(0x10090944, 1<<6, 1<<6);
	sdp_set_clockgating(0x10090944, 1<<7, 1<<7);
	sdp_set_clockgating(0x10090954, 1<<10, 1<<10);
	return 0;
}
#endif/* CONFIG_SDP_CLOCK_GATING */


static int sdp1202_dma330_0_init(void) {
	/* not to do */
	return 0;
}

static struct dma_pl330_peri sdp_ams_dma330_peri_0[] = {
	[0] = {
		.peri_id = 0,
		.rqtype = MEMTOMEM,
	},
	[1] = {
		.peri_id = 1,
		.rqtype = MEMTOMEM,
	},
};

static struct sdp_dma330_platdata sdp_ams_dma330_0_plat = {
	.nr_valid_peri = ARRAY_SIZE(sdp_ams_dma330_peri_0),
	/* Array of valid peripherals */
	.peri = sdp_ams_dma330_peri_0,
	/* Bytes to allocate for MC buffer */
	.mcbuf_sz = 1024*128,
	.plat_init = sdp1202_dma330_0_init,

#ifdef CONFIG_SDP_CLOCK_GATING
	.plat_clk_gate = sdp1202_dma330_gate_clock,
	.plat_clk_ungate = sdp1202_dma330_ungate_clock,
	.plat_clk_used_cnt = &sdp1202_dma330_clk_used_cnt,
#endif
};

static struct amba_device amba_ams_dma330_0 = {
	.dev		= {
		.init_name			= "sdp-amsdma330_0",
		.platform_data		= &sdp_ams_dma330_0_plat,
		.dma_mask			= &sdp_dma330_dmamask,
		.coherent_dma_mask	= 0xffffffff,
	},
	.res		= {
		.start	= PA_AMS_DMA330_0_BASE,
		.end	= PA_AMS_DMA330_0_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	.irq		= { IRQ_DMA330, },
	.periphid	= 0x00241330,/*rev=02, designer=41, part=330*/
};




static int sdp1202_dma330_1_init(void) {
	/* not to do */
	return 0;
}

static struct dma_pl330_peri sdp_ams_dma330_peri_1[] = {
	[0] = {
		.peri_id = 0,
		.rqtype = MEMTOMEM,
	},
	[1] = {
		.peri_id = 1,
		.rqtype = MEMTOMEM,
	},
};

static struct sdp_dma330_platdata sdp_ams_dma330_1_plat = {
	.nr_valid_peri = ARRAY_SIZE(sdp_ams_dma330_peri_1),
	/* Array of valid peripherals */
	.peri = sdp_ams_dma330_peri_1,
	/* Bytes to allocate for MC buffer */
	.mcbuf_sz = 1024*128,
	.plat_init = sdp1202_dma330_1_init,

#ifdef CONFIG_SDP_CLOCK_GATING
	.plat_clk_gate = sdp1202_dma330_gate_clock,
	.plat_clk_ungate = sdp1202_dma330_ungate_clock,
	.plat_clk_used_cnt = &sdp1202_dma330_clk_used_cnt,
#endif
};

static struct amba_device amba_ams_dma330_1 = {
	.dev		= {
		.init_name			= "sdp-amsdma330_1",
		.platform_data		= &sdp_ams_dma330_1_plat,
		.dma_mask			= &sdp_dma330_dmamask,
		.coherent_dma_mask	= 0xffffffff,
	},
	.res		= {
		.start	= PA_AMS_DMA330_1_BASE,
		.end	= PA_AMS_DMA330_1_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	.irq		= { IRQ_DMA330, },
	.periphid	= 0x00241330,/*rev=02, designer=41, part=330*/
};
/* end dma330 */

static __initdata struct amba_device *amba_devs[] = {
	&amba_ams_dma330_0,
	&amba_ams_dma330_1,
};

/* end amba devices */

static struct proc_dir_entry *sdp_kernel;

void __init sdp1202_iomap_init (void)
{
	iotable_init(sdp1202_io_desc, ARRAY_SIZE(sdp1202_io_desc));
}

int sdp1202_get_revision_id(void)
{
	static int bInit = 0;
	void __iomem *base = (void __iomem*) (0x10080000 + DIFF_IO_BASE0);
	unsigned int rev_data;
	int rev_id;

	if(bInit)
		return (int) sdp_revision_id;

	writel(0x1F, (u32) base + 0x4);	
	while(readl(base) != 0);
	while(readl(base) != 0);
	readl((u32) base + 0x8);
	rev_data = readl((u32) base + 0x8);
	rev_data = (rev_data & 0xC00) >> 10;
	printk("SDP Get Revision ID : %X ", rev_data);

	rev_id = (int) rev_data;

	sdp_revision_id = (u32) rev_id;

	printk("version ES%d\n", sdp_revision_id);
	
	return (int) sdp_revision_id;
}

int sdp_get_revision_id(void)
{
	return (int) sdp_revision_id;
}

static bool sdp1202_is_dual_mp(void)
{
	unsigned int val = readl(VA_EBUS_BASE);

	if (val & ( 1UL << 31)) {
		printk("dual MP board\n");
		sdp_is_dual_mp = true;
	}
	else {
		printk("single MP board\n");
		sdp_is_dual_mp = false;
	}

	return sdp_is_dual_mp;
}

extern unsigned int sdp_sys_mem0_size;
extern unsigned int sdp_sys_mem1_size;
extern unsigned int sdp_sys_mem2_size;

static int proc_read_sdpkernel(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	int len;
	len = sprintf(page, "%d %d %d\n", sdp_sys_mem0_size >> 20, sdp_sys_mem1_size >> 20, sdp_sys_mem2_size >> 20);
	return len;
}


void __init sdp1202_init(void)
{
	int i;
	u32 val;

	sdp1202_get_revision_id();
	val = readl(VA_PMU_BASE + 0x154) & 0x10;	//get usb 3.0 reset
	if(!val)	//if reset on
	{
		sdp_xhci0_resource[0].flags = 0;	//remove IO_RESOURCE_MEM flag
	}

	/* pmic default voltage */
	if (sdp_revision_id == 0) { /* ES0 */
		tps54921_regulator.def_volt = 1170000;
	} else { /* ES1 */
		tps54921_regulator.def_volt = 1130000;
	}
	
	platform_add_devices(sdp1202_init_devs, ARRAY_SIZE(sdp1202_init_devs));

	/* for dual MP board */
	if (sdp1202_is_dual_mp())
		platform_device_register(&sdp_pmic_sn1202033[2]);

	/* amba devices register */
	printk("AMBA : amba devices registers..");
	for (i = 0; i < (int) ARRAY_SIZE(amba_devs); i++)
	{
		struct amba_device *d = amba_devs[i];
		amba_device_register(d, &iomem_resource);
	}
	
	sdp_kernel = create_proc_entry("sdp_kernel", 0644, NULL);
	if(sdp_kernel == NULL)
	{
		printk(KERN_ERR "[sdp_kernel] fail to create proc sdp_kernel info\n");
	}
	else
	{
		sdp_kernel->read_proc = proc_read_sdpkernel;
		printk("/proc/sdp_version is registered!\n");
	}
}

