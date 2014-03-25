/*
 * tps54921.c
 * 
 * Regulator driver for TPS54921 PMIC
 *
 * Copyright (C) 2012 Samsung Electronics
 * Seihee Chon <sh.chon@samsung.com>
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/err.h>
#include <linux/platform_device.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/machine.h>
#include <linux/regulator/tps54921.h>
#include <linux/delay.h>
#include <linux/slab.h>

#include <asm/ioctl.h>

#if defined(CONFIG_ARCH_CCEP)
#include <plat/sdp_i2c_io.h>
#else
#error "Use this regulator driver for SDP"
#endif

#define TPS54921_CHNAGE_DELAY	200

static int is_setvolt = 0;

/* Supported voltage values for regulators (in milliVolts) */
static const u16 vout_table[] = {
	720, 730, 740, 750, 760,
	770, 780, 790, 800, 810,
	820, 830, 840, 850, 860,
	870, 880, 890, 900, 910,
	920, 930, 940, 950, 960,
	970, 980, 990, 1000, 1010,
	1020, 1030, 1040, 1050, 1060,
	1070, 1080, 1090, 1100, 1110,
	1120, 1130, 1140, 1150, 1160,
	1170, 1180, 1190, 1200, 1210,
	1220, 1230, 1240, 1250, 1260,
	1270, 1280, 1290, 1300, 1310,
	1320, 1330, 1340, 1350, 1360,
	1370, 1380, 1390, 1400, 1410,
	1420, 1430, 1440, 1450, 1460,
	1470, 1480,
};

static struct tps_info tps54921_pmic_regs = {
		.name = "TPS54921VOUT",
		.min_uV = 720000,
		.max_uV = 1480000,
		.table_len = ARRAY_SIZE(vout_table),
		.table = vout_table,
};
		
static int tps54921_pmic_write(struct regulator_dev *dev, u8 val)
{
	struct tps54921_pmic *tps = rdev_get_drvdata(dev);
	struct sdp_i2c_packet_t	packet; 
	unsigned char buff = val;
	unsigned char checksum = 0;
	int ret, i;

	/* add checksum bit */
	for (i = 0; i < 7; i++) {
		if (val & 0x1)
			checksum++;
		val = val >> 1;
	}
	if (checksum % 2)
		buff |= 0x80;		

	packet.slaveAddr	= tps->i2c_addr;
	packet.subAddrSize	= 0;
	packet.udelay		= 0;
	packet.speedKhz		= 400;	// 400 KHz clk
	packet.dataSize		= 1;
	packet.reserve[0]	= 0;
	packet.reserve[1]	= 0;
	packet.reserve[2]	= 0;
	packet.reserve[3]	= 0;

	packet.pSubAddr 	= 0;
	packet.pDataBuffer	= &buff;

	ret = sdp_i2c_request(tps->i2c_port, I2C_CMD_WRITE, &packet);
	if (ret < 0)
		printk(KERN_ERR "tps54921: i2s write error %d\n", ret);

	return ret;
}

static int tps54921_pmic_set_voltage(struct regulator_dev *dev,
									int min_uV, int max_uV, unsigned *selector)
{
	struct tps54921_pmic *tps = rdev_get_drvdata(dev);
	int rdev_id = rdev_get_id(dev);
	int vsel;
	int ret;

	tps54921_dbg("tps54921:%s, min=%d, max=%d\n", __func__, min_uV, max_uV);

	if (rdev_id != 0) {
		printk(KERN_ERR "rdev_id is %d\n", rdev_id);
		return -EINVAL;
	}

	if (min_uV < tps->info->min_uV
		|| min_uV > tps->info->max_uV) {
		printk(KERN_ERR "min_uV error. %d\n", min_uV);
		return -EINVAL;
	}
	if (max_uV < tps->info->min_uV
		|| max_uV > tps->info->max_uV) {
		printk(KERN_ERR "max_uV error. %d\n", max_uV);
		return -EINVAL;
	}

	mutex_lock(&tps->io_lock);

	for (vsel = 0; vsel < tps->info->table_len; vsel++) {
		int mV = tps->info->table[vsel];
		int uV = mV * 1000;

		/* Break at the first in-range value */
		if (min_uV <= uV && uV <= max_uV)
			break;
	}

	/* write to the pmic in case we found a match */
	if (vsel == tps->info->table_len) {
		printk(KERN_ERR "tps54921: voltage not found!\n");
		mutex_unlock(&tps->io_lock);
		return -EINVAL;
	}

	tps54921_dbg("tps54921: vsel = %d\n", vsel);
	ret = tps54921_pmic_write(dev, vsel);
	if (ret >= 0) {
		if (!is_setvolt)
			is_setvolt = 1;
		tps->info->cur_vsel = vsel; /* store for future use */
	} else {
		/* if the first write fail, store defualt voltage */		   
		if (!is_setvolt) {
			for (vsel = 0; vsel < tps->info->table_len; vsel++) {
				int mV = tps->info->table[vsel];
				int uV = mV * 1000;

				/* Break at the first in-range value */
				if (min_uV <= uV && uV <= max_uV)
					break;
			}
			tps->info->cur_vsel = vsel;
			printk(KERN_INFO "tps54921: first write fail. default voltage stored.(vsel=%d)\n", vsel);
		}
		ret = -EIO;
	}
	
	*selector = tps->info->cur_vsel;

	udelay(TPS54921_CHNAGE_DELAY);

	mutex_unlock(&tps->io_lock);

	return ret;
}

static int tps54921_pmic_is_enabled(struct regulator_dev *dev)
{
	int pmic_id = rdev_get_id(dev);

	tps54921_dbg("tps54921:%s\n", __func__);

	if (pmic_id != 0)
		return -EINVAL;

	return 1; /* TODO: temporary 1*/
}

static int tps54921_pmic_enable(struct regulator_dev *dev)
{
	int pmic_id = rdev_get_id(dev);
	struct tps_info *info = &tps54921_pmic_regs;

	tps54921_dbg("tps54921:%s\n", __func__);

	if (pmic_id != 0)
		return -EINVAL;

	info->enable = 1;

	return 0;
}

static int tps54921_pmic_disable(struct regulator_dev *dev)
{
	int pmic_id = rdev_get_id(dev);
	struct tps_info *info = &tps54921_pmic_regs;

	tps54921_dbg("tps54921:%s\n", __func__);

	if (pmic_id != 0)
		return -EINVAL;

	info->enable = 0;

	return 0;
}

/* must be called after set_voltage */
static int tps54921_pmic_get_voltage(struct regulator_dev *dev)
{
	int pmic_id = rdev_get_id(dev);
	struct tps54921_pmic *tps = rdev_get_drvdata(dev);
	struct tps_info *info = &tps54921_pmic_regs;
	int volt;

	tps54921_dbg("tps54921:%s\n", __func__);

	if (pmic_id != 0)
		return -EINVAL;

	if (!is_setvolt) {
		printk(KERN_INFO "tps54921: HW defalut %duV\n", tps->def_volt);
		return tps->def_volt; /* HW default */
	}

	volt = info->table[info->cur_vsel];

	return volt * 1000;	
}

static int tps54921_pmic_list_voltage(struct regulator_dev *dev,
									unsigned selector)
{
	int pmic_id = rdev_get_id(dev);
	struct tps_info *info = &tps54921_pmic_regs;

	if (pmic_id != 0)
		return -EINVAL;

	if (selector >= info->table_len)
		return -EINVAL;

	return info->table[selector] * 1000;
}

/* Operations */
static struct regulator_ops tps54921_pmic_ops = {
	.is_enabled = tps54921_pmic_is_enabled,
	.enable = tps54921_pmic_enable,
	.disable = tps54921_pmic_disable,
	.get_voltage = tps54921_pmic_get_voltage,
	.set_voltage = tps54921_pmic_set_voltage,
	.list_voltage = tps54921_pmic_list_voltage,
};

static __devinit int tps54921_pmic_probe(struct platform_device *pdev)
{
	struct tps_info *info;
	struct regulator_init_data *reg_data;
	struct regulator_dev *rdev;
	struct tps54921_pmic *pmic;
	struct tps54921_platform_data *pmic_plat_data;
	int error, ret, timeout = 3;
	unsigned sel;

	tps54921_dbg("tps54921: %s\n", __func__);

	pmic_plat_data = dev_get_platdata(&pdev->dev);
	if (!pmic_plat_data)
		return -EINVAL;

	reg_data = pmic_plat_data->init_data;
	if (!reg_data)
		return -EINVAL;

	pmic = kzalloc(sizeof(*pmic), GFP_KERNEL);
	if (!pmic)
		return -ENOMEM;

	pmic->i2c_addr = pmic_plat_data->i2c_addr;
	pmic->i2c_port = pmic_plat_data->i2c_port;
	pmic->def_volt = pmic_plat_data->def_volt;

	mutex_init(&pmic->io_lock);
	platform_set_drvdata(pdev, pmic);

	info = &tps54921_pmic_regs;

	pmic->info = info;
	pmic->desc.name = info->name;
	pmic->desc.id = 0;
	pmic->desc.n_voltages = info->table_len;
	pmic->desc.ops = &tps54921_pmic_ops;
	pmic->desc.type = REGULATOR_VOLTAGE;
	pmic->desc.owner = THIS_MODULE;

	rdev = regulator_register(&pmic->desc,
					&pdev->dev, reg_data, pmic);
	if (IS_ERR(rdev)) {
		dev_err(&pdev->dev,
			"failed to regiser %s regulator\n",
			pdev->name);
		error = PTR_ERR(rdev);
		goto err;
	}

	/* Save regulator for cleanup */
	pmic->rdev = rdev;

	/* Set default voltage */
	while (timeout > 0) {
		ret = tps54921_pmic_set_voltage(rdev, 
					pmic->def_volt, 
					pmic->def_volt + 10000, 
					&sel);
		if (ret >= 0) {
			dev_info(&pdev->dev, "pmic set voltage success\n");	
			break;
		} else {
			dev_info(&pdev->dev, "pmic set voltage fail\n");
		}
		timeout--;
	}
	if (timeout <= 0)
		dev_err(&pdev->dev, "fail to set default voltage\n");

	return 0;

err:
	kfree(pmic);
	return error;
}

static int __devexit tps54921_pmic_remove(struct platform_device *pdev)
{
	struct tps54921_pmic *tps = platform_get_drvdata(pdev);

	regulator_unregister(tps->rdev);

	kfree(tps);	

	return 0;
}

static struct platform_driver tps54921_pmic_driver = {
	.driver = {
		.name = "sdp_tps54921",
		.owner = THIS_MODULE,
	},
	.probe = tps54921_pmic_probe,
	.remove = __devexit_p(tps54921_pmic_remove),
};

/*
 * tps54921_pmic_init
 * Module init function
 */
static int __init tps54921_pmic_init(void)
{
	return platform_driver_register(&tps54921_pmic_driver);
}
//subsys_initcall(tps54921_pmic_init);
device_initcall(tps54921_pmic_init);

/*
 * tps54921_pmic_cleanup
 * Moduel exit function
 */
static void __exit tps54921_pmic_cleanup(void)
{
	platform_driver_unregister(&tps54921_pmic_driver);
}
module_exit(tps54921_pmic_cleanup);

