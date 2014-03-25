/*
 * arch/arm/plat-ccep/sdp_phy.c
 *
 * Driver for Realtek PHYs
 *
 * Author: Johnson Leung <r58129@freescale.com>
 *
 * Copyright (c) 2004 Freescale Semiconductor, Inc.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

/*
 * modified by dongseok.lee <drain.lee@samsung.com>
 * 
 * 20210420 drain.lee Create file.
 */
 
#include <linux/phy.h>
#include <linux/module.h>

#define RTL821x_PHYSR		0x11
#define RTL821x_PHYSR_DUPLEX	0x2000
#define RTL821x_PHYSR_SPEED	0xc000
#define RTL821x_INER		0x12
#define RTL821x_INER_INIT	0x6400
#define RTL821x_INSR		0x13

MODULE_DESCRIPTION("Realtek PHY driver");
MODULE_AUTHOR("Johnson Leung");
MODULE_LICENSE("GPL");

MODULE_AUTHOR("modified by Dongseok Lee");

static int rtl821x_ack_interrupt(struct phy_device *phydev)
{
	int err;

	err = phy_read(phydev, RTL821x_INSR);

	return (err < 0) ? err : 0;
}

static int rtl821x_config_intr(struct phy_device *phydev)
{
	int err;

	if (phydev->interrupts == PHY_INTERRUPT_ENABLED)
		err = phy_write(phydev, RTL821x_INER,
				RTL821x_INER_INIT);
	else
		err = phy_write(phydev, RTL821x_INER, 0);

	return err;
}


static void rtl8201f_EEE_Disable(struct phy_device *phydev)
{
	int phyVal;

	dev_info(&phydev->dev, "RTL8201F EEE Disable!\n");

	/* WOL Disable */
	phy_write(phydev, 31, 7);
	phyVal = phy_read(phydev,  19);
	phyVal = phyVal & ~(0x1<<10);
	phy_write(phydev,  19, phyVal);
	phy_write(phydev,  31, 0);

	/* LED Disable */
	phy_write(phydev,  31, 7);
	phyVal = phy_read(phydev,  19);
	phyVal = phyVal | (0x1<<3);
	phy_write(phydev,  19, phyVal);
	phy_write(phydev,  17, 0);
	phy_write(phydev,  31, 0);

	/* EEE Disable */
	phy_write(phydev,  31, 0x4);
	phy_write(phydev,  16, 0x4077);
	phy_write(phydev,  31, 0x0);
	phy_write(phydev,  13, 0x7);
	phy_write(phydev,  14, 0x3c);
	phy_write(phydev,  13, 0x4007);
	phy_write(phydev,  14, 0x0);
	phy_write(phydev,  0, 0x1200);
}

static int rtl8021f_config_init(struct phy_device *phydev)
{
	rtl8201f_EEE_Disable(phydev);
	return 0;
}

static int realtek_probe(struct phy_device *phydev)
{
	if((phydev->drv->phy_id_mask & 0xF) == 0) {
		dev_info(&phydev->dev, "%s%c probed.\n", phydev->drv->name, (phydev->phy_id&0xF)+'A'-1);
	} else {
		dev_info(&phydev->dev, "%s probed.\n", phydev->drv->name);
	}
	return 0;
}

/* supported phy list */
static struct phy_driver sdp_phy_drivers[] = {
	{/* RTL8201E */
		.phy_id		= 0x001CC815,
		.name		= "RTL8201E",
		.phy_id_mask	= 0x001fffff,
		.features	= PHY_BASIC_FEATURES,
		.flags		= PHY_HAS_INTERRUPT,
		.probe			= realtek_probe,
		.config_init	= NULL,
		.config_aneg	= genphy_config_aneg,
		.read_status	= genphy_read_status,
		.driver		= { .owner = THIS_MODULE,},
	},
	{/* RTL8201F */
		.phy_id		= 0x001CC816,
		.name		= "RTL8201F",
		.phy_id_mask	= 0x001fffff,
		.features	= PHY_BASIC_FEATURES,
		.flags		= PHY_HAS_INTERRUPT,
		.probe			= realtek_probe,
		.config_init	= rtl8021f_config_init,
		.config_aneg	= genphy_config_aneg,
		.read_status	= genphy_read_status,
		.driver		= { .owner = THIS_MODULE,},
	},
	{/* RTL8201x this is RTL8201x common phy driver. */
		.phy_id		= 0x001CC810,
		.name		= "RTL8201",
		.phy_id_mask	= 0x001ffff0,
		.features	= PHY_BASIC_FEATURES,
		.flags		= PHY_HAS_INTERRUPT,
		.probe			= realtek_probe,
		.config_init	= NULL,
		.config_aneg	= genphy_config_aneg,
		.read_status	= genphy_read_status,
		.driver		= { .owner = THIS_MODULE,},
	},


	/* for RGMII Phy */
	{/* RTL8211E */
		.phy_id		= 0x001cc915,
		.name		= "RTL8211E",
		.phy_id_mask	= 0x001fffff,
		.features	= PHY_GBIT_FEATURES,
		.flags		= PHY_HAS_INTERRUPT,
		.probe			= realtek_probe,
		.config_init	= NULL,
		.config_aneg	= genphy_config_aneg,
		.read_status	= genphy_read_status,
		.ack_interrupt	= rtl821x_ack_interrupt,
		.config_intr	= rtl821x_config_intr,
		.driver		= { .owner = THIS_MODULE,},
	},
	{/* RTL8211x this is RTL8211x common phy driver. */
		.phy_id		= 0x001cc910,
		.name		= "RTL8211",
		.phy_id_mask	= 0x001ffff0,
		.features	= PHY_GBIT_FEATURES,
		.flags		= PHY_HAS_INTERRUPT,
		.probe			= realtek_probe,
		.config_init	= NULL,
		.config_aneg	= genphy_config_aneg,
		.read_status	= genphy_read_status,
		.ack_interrupt	= rtl821x_ack_interrupt,
		.config_intr	= rtl821x_config_intr,
		.driver		= { .owner = THIS_MODULE,},
	},
};

static int __init sdp_phy_init(void)
{
	int i;
	int ret = 0;

	for(i = 0; i < ARRAY_SIZE(sdp_phy_drivers); i++)
	{
		ret = phy_driver_register(&sdp_phy_drivers[i]);
		if(ret < 0) {
			pr_err("phy driver register failed(index:%d name:%s).\n", i, sdp_phy_drivers[i].name);
			for(i = i-1; i >= 0; i--) {
				phy_driver_unregister(&sdp_phy_drivers[i]);
			}
			return ret;
		}
	}

	pr_info("Registered sdp-phy drivers.\n");

	return ret;
}

static void __exit sdp_phy_exit(void)
{
	int i;
	for(i = 0; i < ARRAY_SIZE(sdp_phy_drivers); i++) {
		phy_driver_unregister(&sdp_phy_drivers[i]);
	}
}

module_init(sdp_phy_init);
module_exit(sdp_phy_exit);

static struct mdio_device_id __maybe_unused sdp_phy_tbl[] = {
	{ 0x001CC815, 0x001fffff },
	{ 0x001CC816, 0x001fffff },
	{ 0x001CC810, 0x001ffff0 },
	{ 0x001cc915, 0x001fffff },
	{ 0x001cc910, 0x001ffff0 },
	{ }
};

MODULE_DEVICE_TABLE(mdio, sdp_phy_tbl);
