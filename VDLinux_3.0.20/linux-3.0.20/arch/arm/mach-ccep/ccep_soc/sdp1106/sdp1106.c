/*
 * sdp1106.c
 *
 * Copyright (C) 2011 Samsung Electronics.co
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

#include <plat/sdp_mmc.h>

extern unsigned int sdp_revision_id;

static struct map_desc sdp1106_io_desc[] __initdata = {
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
#ifdef CONFIG_ARM_GIC	
		.start 	= IRQ_UART0,
		.end	= IRQ_UART0,
#else
		.start 	= IRQ_UART,
		.end	= IRQ_UART,
#endif
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
#ifdef CONFIG_ARM_GIC	
		.start 	= IRQ_UART1,
		.end	= IRQ_UART1,
#else
		.start 	= IRQ_UART,
		.end	= IRQ_UART,
#endif
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
#ifdef CONFIG_ARM_GIC	
		.start 	= IRQ_UART2,
		.end	= IRQ_UART2,
#else
		.start 	= IRQ_UART,
		.end	= IRQ_UART,
#endif
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
#ifdef CONFIG_ARM_GIC	
		.start 	= IRQ_UART3,
		.end	= IRQ_UART3,
#else
		.start 	= IRQ_UART,
		.end	= IRQ_UART,
#endif
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


// add sdpGmac platform device by tukho.kim 20091205
#include <plat/sdp_gmac_reg.h>

static int sdp1106_gmac_init(void)
{
	/* SRAM select*/
	*(volatile u32 *)(VA_IO_BASE0+0x00090CDC) = 1;
	return  0;
}

static struct sdp_gmac_plat sdp_gmac_platdata =  {
	.init = sdp1106_gmac_init,/* board specipic init */
	.phy_mask = 0x0,/*bit filed 1 is masked */
	.napi_weight = 64,/* netif_napi_add() weight value. 1 ~ max */
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

static struct platform_device sdpGmac_devs = {
        .name           = ETHER_NAME,
        .id             = 0,
        .dev		= {
			.platform_data = &sdp_gmac_platdata,
		},
        .num_resources  = ARRAY_SIZE(sdpGmac_resource),
        .resource       = sdpGmac_resource,
};
// add sdpGmac platform device by tukho.kim 20091205 end
//
// mmc
static int sdp1106_mmch_init(SDP_MMCH_T *p_sdp_mmch)
{
	if(p_sdp_mmch->pm_is_valid_clk_delay == false)
	{
		/* save clk delay */
		p_sdp_mmch->pm_clk_delay[0] = *(volatile u32*)(VA_IO_BASE0 + 0x00090934);
		p_sdp_mmch->pm_is_valid_clk_delay = true;
	}
	else
	{
		/* restore clk delay */
		*(volatile u32*)(VA_IO_BASE0 + 0x00090934) = p_sdp_mmch->pm_clk_delay[0];
	}
	return 0;
}

static struct sdp_mmch_plat sdp_mmc_platdata = {
	.processor_clk = 100000000,
	.min_clk = 400000,
	.max_clk = 50000000,
	.caps =  (MMC_CAP_8_BIT_DATA | MMC_CAP_MMC_HIGHSPEED \
			| MMC_CAP_NONREMOVABLE/* | MMC_CAP_1_8V_DDR | MMC_CAP_UHS_DDR50*/),
	.init = sdp1106_mmch_init,
	.fifo_depth = 32,
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

/* thermal management */
static struct platform_device sdp_tmu = {
	.name		= "sdp-tmu",
	.id		= 0,
//	.num_resources	= ARRAY_SIZE(sdp_mmc_resource),
//	.resource	= sdp_mmc_resource,
};

/* SPI master controller */

#define REG_MSPI_TX_CTRL	0x30090268// set 0x1000 for sel SPI
#define MSPI_TX_CTRL_SELSPI (0x01 << 12)
static int sdp1106_spi_init(void)
{
	int tmp;
	//MSPI Reg TX_CTRL sel_spi
	tmp = readl((REG_MSPI_TX_CTRL + DIFF_IO_BASE0));
	tmp |= MSPI_TX_CTRL_SELSPI;
	writel(tmp, (REG_MSPI_TX_CTRL + DIFF_IO_BASE0));
	
	//Pad control Main/Sub Function Selection REGISTER  bit 20 set SPI use
	tmp = readl(VA_PADCTRL_BASE+0x50);
	tmp |= 0x01 << 20;
	writel(tmp, (VA_PADCTRL_BASE+0x50));
	return 0;
}

static struct resource sdp_spi_resource[] = 
{
	[0] = {
			.start = PA_SPI_BASE,
			.end = PA_SPI_BASE + 0x18 - 1,
			.flags = IORESOURCE_MEM,
		},
	/* EchoP is polling mode, not irq# */
};

/* SPI Master Controller drain.lee */
static struct sdp_spi_info sdp_spi_master_data = {
	.num_chipselect = CONFIG_SDP_SPI_NUM_OF_CS,
	.init = sdp1106_spi_init,
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


/* Regulator */
/* tps54921 */
static struct regulator_consumer_supply tps54921_supply = {
		.supply = "VDD_ARM",
		.dev_name = NULL,
};

static struct regulator_init_data tps54921_data = {
	.constraints	= {
		.name		= "VDD_ARM range",
		.min_uV		= 1080000,
		.max_uV		= 1200000,
		.always_on	= 1,
		.boot_on	= 1,
		.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE,
	},
	.num_consumer_supplies	= 1,
	.consumer_supplies	= &tps54921_supply,
};

static struct tps54921_platform_data tps54921_regulator = {
		.i2c_addr	= 0x6E,
		.i2c_port	= 0,
		.def_volt	= 1100000,
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
		.supply = "VDD_GPU",
		.dev_name = "vddgpu.0",
	},
	{
		.supply = "VDD_MP",
		.dev_name = "vddmp.0",
	},
};

static struct regulator_init_data sn1202033_data[] = {
	[0] = {
		.constraints	= {
			.name		= "VDD_GPU range",
			.min_uV		= 1000000,
			.max_uV		= 1200000,
			.always_on	= 1,
			.boot_on	= 1,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE,
		},
		.num_consumer_supplies = 1,
		.consumer_supplies = &sn1202033_supply[0],
	},
	[1] = {
		.constraints	= {
			.name		= "VDD_MP range",
			.min_uV		= 1100000,
			.max_uV		= 1200000,
			.always_on	= 1,
			.boot_on	= 1,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE,
		},
		.num_consumer_supplies = 1,
		.consumer_supplies = &sn1202033_supply[1],
	},
};

static struct sn1202033_platform_data sn1202033_regulator[] = {
	[0] = {
		.i2c_addr	= 0xC0,
		.i2c_port	= 0,
		.def_volt	= 1100000,
		.init_data	= &sn1202033_data[0],
	},
	[1] = {
		.i2c_addr	= 0xC0,
		.i2c_port	= 0,
		.def_volt	= 1100000,
		.init_data	= &sn1202033_data[1],
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
};

/* end Regulator */




static struct platform_device* sdp1106_init_devs[] __initdata = {
	&sdp_uart0,
	&sdp_uart1,
	&sdp_uart2,
	&sdp_uart3,
       	&sdp_ehci0,
        &sdp_ohci0,
        &sdp_ehci1,
        &sdp_ohci1,
// add sdpGmac platform device by tukho.kim 20091205
        &sdpGmac_devs,	
	&sdp_mmc,
	&sdp_smc,
	&sdp_spi,
	&sdp_tmu,
	&sdp_pmic_tps54921,
	&sdp_pmic_sn1202033[0],
	&sdp_pmic_sn1202033[1],
};

/* amba devices */
#include <linux/amba/bus.h>
/* for dma330 */
#include <plat/sdp_dma330.h>
static u64 sdp_dma330_dmamask = 0xffffffffUL;

static int sdp1106_cpu_dma330_init(void) {
		unsigned long val = 0;
#define __PA_SWRESET	(0x30B70034)
/*
	(*(volatile unsigned *)(CLKGENBASE+0x40)) = 0x07050301;
	(*(volatile unsigned *)(CLKGENBASE+0x44)) = 0x0f0d0b09;
	(*(volatile unsigned *)(CLKGENBASE+0x50)) = 0x07050301;
	(*(volatile unsigned *)(CLKGENBASE+0x58)) = 0x17151311;
*/
	/* CDMAARUSER0 */
	writel(0x07050301, (DIFF_IO_BASE0+0x30B70040));
	/* CDMAARUSER1 */
	writel(0x0f0d0b09, (DIFF_IO_BASE0+0x30B70044));

	/* CDMAAWUSER0 */
	writel(0x07050301, (DIFF_IO_BASE0+0x30B70050));
	/* CDMAAWUSER2 */
	writel(0x0f0d0b09, (DIFF_IO_BASE0+0x30B70058));

	/* DMAMODE */
	writel(0x00000101, (DIFF_IO_BASE0+0x30B7004c));

	//dma330 reset for dma menager thread Secure state
	val = readl((DIFF_IO_BASE0+__PA_SWRESET));
	writel(val&~(1<<13), (DIFF_IO_BASE0+__PA_SWRESET));
	writel(val|(1<<13), (DIFF_IO_BASE0+__PA_SWRESET));

	return 0;
}

static struct dma_pl330_peri sdp_cpu_dma330_peri[] = {
	[0] = {
		.peri_id = 0,
		.rqtype = MEMTOMEM,
	},
	[1] = {
		.peri_id = 1,
		.rqtype = MEMTOMEM,
	},
	[2] = {
		.peri_id = 2,
		.rqtype = MEMTOMEM,
	},
	[3] = {
		.peri_id = 3,
		.rqtype = MEMTOMEM,
	},
};

static struct sdp_dma330_platdata sdp_cpu_dma330_plat = {
	.nr_valid_peri = ARRAY_SIZE(sdp_cpu_dma330_peri),
	/* Array of valid peripherals */
	.peri = sdp_cpu_dma330_peri,
	/* Bytes to allocate for MC buffer */
	.mcbuf_sz = 1024*128,
	.plat_init = sdp1106_cpu_dma330_init,
};

#ifdef CONFIG_ARM_GIC
static struct amba_device amba_cpu_dma330 = {
	.dev		= {
		.init_name			= "sdp-cpudma330",
		.platform_data		= &sdp_cpu_dma330_plat,
		.dma_mask			= &sdp_dma330_dmamask,
		.coherent_dma_mask	= 0xffffffff,
	},
	.res		= {
		.start	= PA_CPU_DMA330_BASE,
		.end	= PA_CPU_DMA330_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	.irq		= { IRQ_CPUDMA, },
	.periphid	= 0x00241330,/*rev=02, designer=41, part=330*/
};
#endif



static int sdp1106_ams_dma330_init(void) {
	/* not to do */
	return 0;
}

static struct dma_pl330_peri sdp_ams_dma330_peri[] = {
	[0] = {
		.peri_id = 0,
		.rqtype = MEMTOMEM,
	},
	[1] = {
		.peri_id = 1,
		.rqtype = MEMTOMEM,
	},
};

static struct sdp_dma330_platdata sdp_ams_dma330_plat = {
	.nr_valid_peri = ARRAY_SIZE(sdp_ams_dma330_peri),
	/* Array of valid peripherals */
	.peri = sdp_ams_dma330_peri,
	/* Bytes to allocate for MC buffer */
	.mcbuf_sz = 1024*128,
	.plat_init = sdp1106_ams_dma330_init,
};

static struct amba_device amba_ams_dma330 = {
	.dev		= {
		.init_name			= "sdp-amsdma330",
		.platform_data		= &sdp_ams_dma330_plat,
		.dma_mask			= &sdp_dma330_dmamask,
		.coherent_dma_mask	= 0xffffffff,
	},
	.res		= {
		.start	= PA_AMS_DMA330_BASE,
		.end	= PA_AMS_DMA330_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
#ifdef CONFIG_ARM_GIC
	.irq		= { IRQ_AMSDMA, },
#else
	.irq		= { IRQ_MERGER0, },//IRQ_AMSDMA
#endif
	.periphid	= 0x00241330,/*rev=02, designer=41, part=330*/
};
/* end dma330 */

static __initdata struct amba_device *amba_devs[] = {
#ifdef CONFIG_ARM_GIC
	&amba_cpu_dma330,
#endif
	&amba_ams_dma330,
};

/* end amba devices */


void __init sdp1106_iomap_init (void)
{
	iotable_init(sdp1106_io_desc, ARRAY_SIZE(sdp1106_io_desc));
}

static int sdp1106_get_revision_id(void)
{
	void __iomem *base = (void __iomem*) (0x30030000 + DIFF_IO_BASE0);
	unsigned int rev_data;
	int rev_id;

	writel(0x1F, base + 0x4);	
	while(readl(base) != 0);
	while(readl(base) != 0);
	readl(base + 0x8);
	rev_data = readl(base + 0x8);
	rev_data = (rev_data & 0xC00) >> 10;
	printk("SDP Get Revision ID : %X ", rev_data);

	rev_id = rev_data;

	sdp_revision_id = rev_id;

	printk("version ES%d\n", sdp_revision_id);
	
	return sdp_revision_id;
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

#define L2C310_AUX_BRESP	(1<<30) /* early BRESP enable */
#define L2C310_AUX_IPF	(1<<29)	/* inst prefetch enable */
#define L2C310_AUX_DPF	(1<<28)	/* data prefetch enable */
#define L2C310_AUX_RR	(1<<25) /* cache replacement policy */

void __init sdp1106_l2c_init(void)
{
	void __iomem *base = (void __iomem*)VA_L2C_BASE;
	u32 v;

	/* prefetch control */
	v = readl (base + L2C310_PCR);
	v |= L2C310_PCR_IPF | L2C310_PCR_DPF;
	v |= L2C310_PCR_DROP | L2C310_PCR_IDLF;
#ifndef CONFIG_SDP_ARM_PL310_ERRATA_752271
	v |= L2C310_PCR_DLF;
#endif

	v &= ~0x1f;
	printk (KERN_CRIT "prefetch=0x%08x\n", v);
	writel (v, base + L2C310_PCR);

	/* tag RAM latency (Setup:1,Write:2,Read:2) */
	v = readl(base + L2C310_TRLCR);
	v &= ~(0x667);
	writel (v, base + L2C310_TRLCR);

	/* data RAM latency (Setup:1,Write:3,Read:2) */	
	v = readl(base + L2C310_DRLCR);
	v &= ~(0x657);
	writel (v, base + L2C310_DRLCR);

	/* Data/Inst prefetch enable, random replacement */
	l2x0_init (base, 0, ~(L2C310_AUX_RR));
}
EXPORT_SYMBOL(sdp1106_l2c_init);
#else	/* CONFIG_SDP1106_L2C310 */
static void __init sdp1106_l2c_init(void) {}
#endif

void sdp1106_patch(void)
{
	volatile unsigned int *v = (volatile unsigned int *) (VA_SCU_BASE + 0x30);

	*v |= 0x1;		//ARM_ERRATA_764369 SCU migratory bit set
}

void __init sdp1106_init(void)
{
	int i;

#ifdef CONFIG_SDP_ARM_A9_ERRATA_764369
	sdp1106_patch();
#endif
	
	sdp1106_get_revision_id();
//	sdp1106_l2c_init();
	platform_add_devices(sdp1106_init_devs, ARRAY_SIZE(sdp1106_init_devs));

	/* amba devices register */
	printk("AMBA : amba devices registers..");
	for (i = 0; i < ARRAY_SIZE(amba_devs); i++)
	{
		struct amba_device *d = amba_devs[i];
		amba_device_register(d, &iomem_resource);
	}
}
