/*
 * foxap-smcspi-devs.c:
 *  Simple registrations of SPI devices on external bus(SMC)
 *  for updating serial flash devices on serdes FPGAs ('13 one-connection board).
 *  Only use this if you really know what you are doing.
 *
 * Copyright (C) 2013 Samsung Electronics.co
 */

#include <linux/platform_device.h>
#include <linux/spi/spi.h>
#include <linux/spi/sdp_spidev.h>
#include <linux/io.h>

#include <mach/hardware.h>
#include <plat/sdp_spi.h>

#define SMCSPI_BUSNUM1	1
#define SMCSPI_BUSNUM2	2

#undef SMCSPI_DEBUG0
#undef SMCSPI_DEBUG

#if defined(SMCSPI_DEBUG)
#define SPITRACE(fmt, ...)	printk(KERN_CRIT fmt, ##__VA_ARGS__)
#else
#define SPITRACE(fmt, ...)
#endif

#if defined(SMCSPI_DEBUG0)
#define SPITRACE0(fmt, ...)	printk(KERN_CRIT fmt, ##__VA_ARGS__)
#else
#define SPITRACE0(fmt, ...)
#endif

struct sdp_smcspi_dev {
	struct platform_device pdev;
	struct spi_board_info spi_bi;
	void __iomem *regbase;
	int pdev_registered;
	int spi_registered;
};

/* bus1: TV side FPGA */
static struct resource smcspi_res0[] = {
	[0] = {
		.start 	= 0x08004000,
		.end	= 0x0800401f,
		.flags	= IORESOURCE_MEM,
	},
};

/* bus2: Jackpack side */
static struct resource smcspi_res1[] = {
	[0] = {
		.start 	= 0x080020e0,
		.end	= 0x080020ff,
		.flags	= IORESOURCE_MEM,
	},
};

/* chip ops */
static int smcspi_chip_init(void);
static void smcspi_chip_cs(struct spi_device *spi, int val);
static int smcspi_chip_setup(struct spi_device *spi);

struct sdp_spi_chip_ops smcspi_chip_ops = {
	//.setup = smcspi_chip_setup,
	.cleanup = NULL,
	.cs_control = smcspi_chip_cs,
};

static struct sdp_spi_info smcspi_master_data1 = {
	.num_chipselect = 1,
	.max_clk_limit = 0,
	.init = smcspi_chip_init,
	//.default_chip_ops = &smcspi_chip_ops,
	.ext_clkrate = 125000000,
};

static struct sdp_spi_info smcspi_master_data2 = {
	.num_chipselect = 1,
	.max_clk_limit = 0,
	.init = smcspi_chip_init,
	//.default_chip_ops = &smcspi_chip_ops,
	.ext_clkrate = 156250000,
};

static struct sdp_smcspi_dev sdp_smcspi_devs[] = {
	[0] = {
		.pdev = {
			.name		= "sdp-spi",
			.id		= SMCSPI_BUSNUM1,
			.dev		= {
				.platform_data = &smcspi_master_data1,
			},
			.num_resources	= ARRAY_SIZE(smcspi_res0),
			.resource	= smcspi_res0,
		},
		.spi_bi = {
			.modalias = "sdp-spidev",
			.controller_data = &smcspi_chip_ops,
			.max_speed_hz = 25 * 1000 * 1000,
			.bus_num = SMCSPI_BUSNUM1,
			.chip_select = 0,
			.mode = SPI_MODE_0,
		},
	},
	[1] = {
		.pdev = {
			.name		= "sdp-spi",
			.id		= SMCSPI_BUSNUM2,
			.dev		= {
				.platform_data = &smcspi_master_data2,
			},
			.num_resources	= ARRAY_SIZE(smcspi_res1),
			.resource	= smcspi_res1,
		},
		.spi_bi = {
			.modalias = "sdp-spidev",
			.controller_data = &smcspi_chip_ops,
			.max_speed_hz = 25 * 1000 * 1000,
			.bus_num = SMCSPI_BUSNUM2,
			.chip_select = 0,
			.mode = SPI_MODE_0,
		},
	},
};

static struct sdp_smcspi_dev* spidev_to_smcspi(struct spi_device *spi)
{
	struct sdp_smcspi_dev *sdev = NULL;

	if (spi->master->bus_num == SMCSPI_BUSNUM1)
		sdev = &sdp_smcspi_devs[0];
	else if (spi->master->bus_num == SMCSPI_BUSNUM2)
		sdev = &sdp_smcspi_devs[1];
	else
		printk (KERN_ERR "spi_device %p(busnum=%d) is not external spi device.\n",
				spi, spi->master->bus_num);
	return sdev;
}

static int smcspi_chip_setup(struct spi_device *spi)
{
	struct sdp_smcspi_dev *sdev;
	sdev = spidev_to_smcspi(spi);
	BUG_ON(!sdev || !sdev->regbase);

	/* SPI CS mux */	
	writel(1, sdev->regbase + 0x18);
	writel(1, sdev->regbase + 0x1c);
	
	SPITRACE("external spi%d: chip_setup, %p = 0x%x, %p = 0x%x\n", sdev->pdev.id,
			sdev->regbase + 0x18, readl(sdev->regbase + 0x18),
			sdev->regbase + 0x1c, readl(sdev->regbase + 0x1c));
	return 0;
}

static int smcspi_chip_init(void)
{
	static int inited = 0;
	if (inited)
		return 0;

	/* SMC timing */
	if (1) {
		writel(0x05, VA_SMC_BASE + 0x14);
		writel(0x08, VA_SMC_BASE + 0x0c);
		writel(0x17, VA_SMC_BASE + 0x20);
	}

	/* o_CS2 pin mux control */
	if (1) {
		u32 val;
		val = readl(VA_PADCTRL_BASE + 0x1fc);
		val &= (~0x300000);
		writel(val, VA_PADCTRL_BASE + 0x1fc);
	}

	inited = 1;
	return 0;
}

static void smcspi_chip_cs(struct spi_device *spi, int val)
{
	struct sdp_smcspi_dev *sdev = spidev_to_smcspi(spi);
	BUG_ON(!sdev || !sdev->regbase);
	if (!sdev->spi_registered)
		return;
	writel(1, sdev->regbase + 0x18);	/* paranoid way */
	writel((val ? 1 : 0), sdev->regbase + 0x1c);	
	SPITRACE0("external spi%d: chip select %p = %d, readback=%d\n",
			sdev->pdev.id,
			sdev->regbase + 0x1c, (val?1:0),
			readl(sdev->regbase + 0x1c));
}

static inline struct sdp_smcspi_dev* pdev_to_smcspi(struct platform_device *pdev)
{
	return (struct sdp_smcspi_dev*)pdev;
}

static void smcspi_unregister_pdevs(void)
{
	int i;
	for (i=0; i<ARRAY_SIZE(sdp_smcspi_devs); i++) {
		struct sdp_smcspi_dev *sdev = &sdp_smcspi_devs[i];
		if (sdev->pdev_registered) {
			if (sdev->regbase) {
				iounmap(sdev->regbase);
				sdev->regbase = NULL;
			}
			platform_device_unregister(&sdev->pdev);
			printk (KERN_INFO "external spi%d unregistered.\n", i);
		}
	}
}

static int foxap_smc_enable_flag = 0;

static int __init foxap_smcspi_enable(char *s)
{
	if (!strcmp(s, "yes"))
		foxap_smc_enable_flag = 1;
	return 0;
}

__setup("foxap_smcspi_enable=", foxap_smcspi_enable);

static int __init smcspi_devs_init(void)
{
	int i, ret;

	if (!foxap_smc_enable_flag) {
		printk (KERN_WARNING "To use FOX-P external spi devices, "
				"set 'foxap_smcspi_enable=yes' in cmdline.\n");
		return 0;
	}
	
	for (i=0; i<ARRAY_SIZE(sdp_smcspi_devs); i++) {
		struct resource *res;
		struct sdp_smcspi_dev *sdev = &sdp_smcspi_devs[i];
		
		ret = platform_device_register(&sdev->pdev);
		if (ret)
			goto smcspi_cleanup;
		sdev->pdev_registered = 1;
	
		/* dual ioremap on master register bank */	
		ret = -ENODEV;

		res = platform_get_resource(&sdev->pdev, IORESOURCE_MEM, 0);
		BUG_ON(!res);
		
		sdev->regbase = ioremap(res->start, resource_size(res));
		if (!sdev->regbase)
			goto smcspi_cleanup;
#if defined(MODULE)
		{
			struct spi_master *master;
			struct spi_device *slave;
			/* TODO */
			master = spi_busnum_to_master(sdev->pdev.id);
			if (!master)
				goto smcspi_cleanup;
			
			slave = spi_new_device(master, &sdev->spi_bi);
			if (!slave)
				goto smcspi_cleanup;
		}
#else
 		spi_register_board_info(&sdev->spi_bi, 1);
		sdev->spi_registered = 1;
#endif
		printk (KERN_INFO "external SPI master %d extra regbase=0x%p.\n",
				sdev->pdev.id, sdev->regbase);
	}
	
	printk (KERN_INFO "external SPI devices were successfully registered.\n");

	return 0;

smcspi_cleanup:
	printk (KERN_ERR "external SPI slave devices: registration failed.\n");
	smcspi_unregister_pdevs();

	return ret;
}

static void __exit smcspi_devs_exit(void)
{
	smcspi_unregister_pdevs();
}

//module_init(smcspi_devs_init);
late_initcall(smcspi_devs_init);
module_exit(smcspi_devs_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ikjoon Jang <ij.jang@samsung.com>");

