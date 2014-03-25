/*
 * sn1202033.c
 * 
 * Regulator driver for SN1202033 PMIC
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
#include <linux/regulator/sn1202033.h>
#include <linux/delay.h>
#include <linux/slab.h>

#include <asm/ioctl.h>

#if defined(CONFIG_ARCH_CCEP)
#include <plat/sdp_i2c_io.h>
#else
#error "Use this regulator driver for SDP"
#endif

/* Supported voltage values for regulators (in milliVolts) */
static const u16 vout_table[] = {
	680, 690, 700, 710, 720,
	730, 740, 750, 760, 770,
	780, 790, 800, 810, 820,
	830, 840, 850, 860, 870,
	880, 890, 900, 910, 920,
	930, 940, 950, 960, 970, 
	980, 990, 1000, 1010, 1020,
	1030, 1040, 1050, 1060, 1070, 
	1080, 1090, 1100, 1110, 1120,
	1130, 1140, 1150, 1160, 1170,
	1180, 1190, 1200, 1210, 1220,
	1230, 1240, 1250, 1260, 1270,
	1280, 1290, 1300, 1310, 1320,
	1330, 1340, 1350, 1360, 1370,
	1380, 1390, 1400, 1410, 1420,
	1430, 1440, 1450, 1460, 1470, 
	1480, 1490, 1500, 1510, 1520, 
	1530, 1540, 1550, 1560, 1570,
	1580, 1590, 1600, 1610, 1620,
	1630, 1640, 1650, 1660, 1670,
	1680, 1690, 1700, 1710, 1720,
	1730, 1740, 1750, 1760, 1770,
	1780, 1790, 1800, 1810, 1820,
	1830, 1840, 1850, 1860, 1870, 
	1880, 1890, 1900, 1910, 1920,
	1930, 1940, 1950, 
};

static char* vout_name[SN1202033_MAXOUT] = {
	"SN1202033VOUT1",
	"SN1202033VOUT2",
};

static int sn1202033_i2c_write(struct regulator_dev *dev,
								unsigned char subaddr,
								unsigned char data)
{
	struct sn1202033_pmic *sn1202033 = rdev_get_drvdata(dev);
	struct sdp_i2c_packet_t	packet; 
	int ret;
	
	packet.slaveAddr	= sn1202033->i2c_addr;
	packet.subAddrSize	= 1;
	packet.udelay		= 0;
	packet.speedKhz		= 400;	// 400 KHz clk
	packet.dataSize		= 1;
	packet.reserve[0]	= 0;
	packet.reserve[1]	= 0;
	packet.reserve[2]	= 0;
	packet.reserve[3]	= 0;

	packet.pSubAddr 	= &subaddr;
	packet.pDataBuffer	= &data;

	sn1202033_dbg("sn1202033: WRITE i2cport:%d, addr:0x%x, subaddr:0x%x, data:0x%x\n",
				sn1202033->i2c_port, packet.slaveAddr,
				subaddr, data);
	ret = sdp_i2c_request(sn1202033->i2c_port, I2C_CMD_WRITE, &packet);
	if (ret < 0)
		printk(KERN_ERR "%s error : %d\n", __func__, ret);

	return ret;
}

static int sn1202033_i2c_read(struct regulator_dev *dev,
								unsigned char subaddr,
								unsigned char *data)
{
	struct sn1202033_pmic *sn1202033 = rdev_get_drvdata(dev);
	struct sdp_i2c_packet_t	packet; 
	int ret;
	
	packet.slaveAddr	= sn1202033->i2c_addr;
	packet.subAddrSize	= 1;
	packet.udelay		= 0;
	packet.speedKhz		= 400;	// 400 KHz clk
	packet.dataSize		= 1;
	packet.reserve[0]	= 0;
	packet.reserve[1]	= 0;
	packet.reserve[2]	= 0;
	packet.reserve[3]	= 0;

	packet.pSubAddr 	= &subaddr;
	packet.pDataBuffer	= data;

	ret = sdp_i2c_request(sn1202033->i2c_port, I2C_CMD_COMBINED_READ, &packet);
	if (ret < 0)
		printk(KERN_ERR "error %s: ret = %d\n", __func__, ret);

	sn1202033_dbg("sn1202033: READ i2cport:%d, addr:0x%x, subaddr:0x%x, data:0x%x\n",
				sn1202033->i2c_port, packet.slaveAddr,
				subaddr, *data);
	
	return ret;
}

static int sn1202033_pmic_status(struct regulator_dev *dev)
{
	unsigned char buff;
	int ret, i;

	for (i = 0; i < 5; i++) {
		ret = sn1202033_i2c_read(dev, i, &buff);
		sn1202033_dbg("addr %d = 0x%x\n", i, buff);
		if (ret < 0)
			break;
	}
	
	return ret;
}
		
static int sn1202033_pmic_write(struct regulator_dev *dev, u8 val)
{
	unsigned char subaddr;
	struct sn1202033_pmic *sn1202033 = rdev_get_drvdata(dev);
	struct sn1202033_info *info;
	unsigned char buff;
	int ret;

	if (sn1202033->vout_port < 1 || sn1202033->vout_port > 2)
		return -EINVAL;
	
	info = sn1202033->info;
	
	if (sn1202033->vout_port == 1)
		subaddr = 0; /* VOUT1 */
	else
		subaddr = 1; /* VOUT2 */

	ret = sn1202033_i2c_read(dev, subaddr, &buff);
	if (ret < 0)
		return -EIO;
	
	buff = (buff & ~(0x7F)) | (val & 0x7F);

	ret = sn1202033_i2c_write(dev, subaddr, buff);

	return ret;
}

static int sn1202033_pmic_read(struct regulator_dev *dev, u8 *vsel)
{
	unsigned char subaddr;
	struct sn1202033_pmic *sn1202033 = rdev_get_drvdata(dev);
	unsigned char buff;
	int ret;

	if (sn1202033->vout_port < 1 || sn1202033->vout_port > 2)
		return -EINVAL;
	
	if (sn1202033->vout_port == 1)
		subaddr = 0; /* VOUT1 */
	else
		subaddr = 1; /* VOUT2 */

	ret = sn1202033_i2c_read(dev, subaddr, &buff);

	*vsel = buff & 0x7F;

	return ret;
}

static int sn1202033_pmic_set_voltage(struct regulator_dev *dev,
									int min_uV, int max_uV, unsigned *selector)
{
	struct sn1202033_pmic *sn1202033 = rdev_get_drvdata(dev);
	int vsel;
	int ret;

	sn1202033_dbg("sn1202033: %s, min=%d, max=%d\n", __func__, min_uV, max_uV);

	if (sn1202033->vout_port < 1 || sn1202033->vout_port > 2) {
		printk(KERN_ERR "vout_port is %d\n", sn1202033->vout_port);
		return -EINVAL;
	}

	if (min_uV < sn1202033->info->min_uV
		|| min_uV > sn1202033->info->max_uV) {
		printk(KERN_ERR "min_uV error. %d\n", min_uV);
		return -EINVAL;
	}
	if (max_uV < sn1202033->info->min_uV
		|| max_uV > sn1202033->info->max_uV) {
		printk(KERN_ERR "max_uV error. %d\n", max_uV);
		return -EINVAL;
	}

	mutex_lock(&sn1202033->io_lock);

	for (vsel = 0; vsel < sn1202033->info->table_len; vsel++) {
		int mV = sn1202033->info->table[vsel];
		int uV = mV * 1000;

		/* Break at the first in-range value */
		if (min_uV <= uV && uV <= max_uV)
			break;
	}

	/* write to the pmic in case we found a match */
	if (vsel == sn1202033->info->table_len) {
		printk(KERN_ERR "voltage not found\n");
		mutex_unlock(&sn1202033->io_lock);
		return -EINVAL;
	}

	sn1202033_dbg("vsel=%d\n", vsel);
	ret = sn1202033_pmic_write(dev, vsel);
	if (ret >= 0)
		sn1202033->info->cur_vsel = vsel; /* store for future use */
	*selector = sn1202033->info->cur_vsel;

	mutex_unlock(&sn1202033->io_lock);

	return ret;
}

static int sn1202033_pmic_is_enabled(struct regulator_dev *dev)
{
	struct sn1202033_pmic *sn1202033 = rdev_get_drvdata(dev);

	sn1202033_dbg("sn1202033: %s\n", __func__);

	if (sn1202033->vout_port < 1 || sn1202033->vout_port > 2)
		return -EINVAL;

	return 1; /* TODO: temporary 1*/
}

static int sn1202033_pmic_enable(struct regulator_dev *dev)
{
	struct sn1202033_pmic *sn1202033 = rdev_get_drvdata(dev);
	struct sn1202033_info *info;
	unsigned char subaddr, data;
	int ret;
	unsigned sel;

	sn1202033_dbg("sn1202033: %s\n", __func__);

	if (sn1202033->vout_port < 1 || sn1202033->vout_port > 2)
		return -EINVAL;
	
	info = sn1202033->info;

	/* must set default voltage at first 
	 * before output enable
	 */
	if (!info->enable)
		/* set default voltage */
		sn1202033_pmic_set_voltage(dev, 
						sn1202033->def_volt, 
						sn1202033->def_volt + 10000,
						&sel);

	/* output enable */
	if (sn1202033->vout_port == 1)
		subaddr = 0; /* VOUT1 */
	else
		subaddr = 1; /* VOUT2 */

	sn1202033_i2c_read(dev, subaddr, &data);
	data |= 0x80;

	ret = sn1202033_i2c_write(dev, subaddr, data);
	if (ret >= 0)
		info->enable = 1;

	return ret;
}

static int sn1202033_pmic_disable(struct regulator_dev *dev)
{
	struct sn1202033_pmic *sn1202033 = rdev_get_drvdata(dev);
	struct sn1202033_info *info;
	unsigned char subaddr, data;
	int ret;

	sn1202033_dbg("sn1202033: %s\n", __func__);

	if (sn1202033->vout_port < 1 || sn1202033->vout_port > 2)
		return -EINVAL;

	info = sn1202033->info;
	
	/* output enable */
	if (sn1202033->vout_port == 1)
		subaddr = 0; /* VOUT1 */
	else
		subaddr = 1; /* VOUT2 */

	sn1202033_i2c_read(dev, subaddr, &data);
	data &= ~0x80;

	ret = sn1202033_i2c_write(dev, subaddr, data);
	if (ret >= 0)
		info->enable = 0;
	
	return ret;
}

/* must be called after set_voltage */
static int sn1202033_pmic_get_voltage(struct regulator_dev *dev)
{
	struct sn1202033_pmic *sn1202033 = rdev_get_drvdata(dev);
	struct sn1202033_info *info;
	int volt, ret;
	u8 vsel = 0x2a;

	sn1202033_dbg("sn1202033: %s\n", __func__);

	if (sn1202033->vout_port < 1 || sn1202033->vout_port > 2)
		return -EINVAL;

	info = sn1202033->info;

	if (!info->enable) {
		printk(KERN_INFO "sn1202033: not enabled. return default voltage.\n");
		return sn1202033->def_volt;
	}

	ret = sn1202033_pmic_read(dev, &vsel);
	if (ret >= 0)
		volt = info->table[vsel];
	else
		volt = sn1202033->def_volt / 1000;

	/* micro volt */
	return volt * 1000;
}

static int sn1202033_pmic_list_voltage(struct regulator_dev *dev,
					unsigned selector)
{
	struct sn1202033_pmic *sn1202033 = rdev_get_drvdata(dev);
	struct sn1202033_info *info;

	sn1202033_dbg("sn1202033: %s, %d\n", __func__, selector);

	if (sn1202033->vout_port < 1 || sn1202033->vout_port > 2)
		return -EINVAL;

	info = sn1202033->info;

	if (selector >= info->table_len)
		return -EINVAL;

	return info->table[selector] * 1000;
}

/* Operations */
static struct regulator_ops sn1202033_pmic_ops = {
	.is_enabled = sn1202033_pmic_is_enabled,
	.enable = sn1202033_pmic_enable,
	.disable = sn1202033_pmic_disable,
	.get_voltage = sn1202033_pmic_get_voltage,
	.set_voltage = sn1202033_pmic_set_voltage,
	.list_voltage = sn1202033_pmic_list_voltage,
};

static __devinit int sn1202033_pmic_probe(struct platform_device *pdev)
{
	struct sn1202033_info *info;
	struct regulator_init_data *init_data;
	struct regulator_dev *rdev;
	struct regulator_dev temp_rdev;
	struct sn1202033_pmic *sn1202033;
	struct sn1202033_platform_data *pmic_plat_data;
	int error;
	unsigned sel;
	u8 vsel;

	sn1202033_dbg("sn1202033: %s\n", __func__);

	pmic_plat_data = dev_get_platdata(&pdev->dev);
	if (!pmic_plat_data)
		return -EINVAL;

	init_data = pmic_plat_data->init_data;
	if (!init_data)
		return -EINVAL;

	sn1202033 = kzalloc(sizeof(*sn1202033), GFP_KERNEL);
	if (!sn1202033)
		return -ENOMEM;

	info = kzalloc(sizeof(*info), GFP_KERNEL);
	if (!info) {
		error = -ENOMEM;
		goto err;
	}

	sn1202033->i2c_addr = pmic_plat_data->i2c_addr;
	sn1202033->i2c_port = pmic_plat_data->i2c_port;
	sn1202033->def_volt = pmic_plat_data->def_volt;
	sn1202033->vout_port = pmic_plat_data->vout_port;

	mutex_init(&sn1202033->io_lock);
	platform_set_drvdata(pdev, sn1202033);

	info->min_uV = 680000;
	info->max_uV = 1950000;
	info->table_len = ARRAY_SIZE(vout_table);
	info->table = vout_table;
	info->enable = 0;
	info->cur_vsel = 0;
	
	if (sn1202033->vout_port == 1) {
		info->name = vout_name[SN1202033_VOUT1];
	} else if (sn1202033->vout_port == 2) {
		info->name = vout_name[SN1202033_VOUT2];
	} else {
		printk(KERN_ERR "%s - VOUT %d is not available.", __func__, sn1202033->vout_port);
		error = -EINVAL;
		goto err2;
	}

	sn1202033->info = info;
	sn1202033->desc.name = info->name;
	sn1202033->desc.id = pdev->id;
	sn1202033->desc.n_voltages = info->table_len;
	sn1202033->desc.ops = &sn1202033_pmic_ops;
	sn1202033->desc.type = REGULATOR_VOLTAGE;
	sn1202033->desc.owner = THIS_MODULE;

	/* pmic alive check */
	temp_rdev.reg_data = sn1202033;
	if (0 > sn1202033_pmic_read(&temp_rdev, &vsel)) {
		printk(KERN_ERR "sn1202033 ERROR: I2C Port %d, Addr 0x%X, pmic is not alive\n",
					sn1202033->i2c_port, sn1202033->i2c_addr);
		error = -EIO;
		goto err2;
	}

	rdev = regulator_register(&sn1202033->desc,
					&pdev->dev, init_data, sn1202033);
	if (IS_ERR(rdev)) {
		dev_err(&pdev->dev,
			"failed to regiser %s regulator\n",
			pdev->name);
		error = PTR_ERR(rdev);
		goto err2;
	}

	/* Save regulator for cleanup */
	sn1202033->rdev = rdev;

	/* set default voltage */
	sn1202033_pmic_set_voltage(rdev, 
					sn1202033->def_volt, 
					sn1202033->def_volt + 10000,
					&sel);

	/* print all registers */
	sn1202033_pmic_status(rdev);

	return 0;

err2:
	kfree(info);
	
err:
	kfree(sn1202033);
	return error;
}

static int __devexit sn1202033_pmic_remove(struct platform_device *pdev)
{
	struct sn1202033_pmic *sn1202033 = platform_get_drvdata(pdev);

	regulator_unregister(sn1202033->rdev);

	kfree(sn1202033->info);
	kfree(sn1202033);	

	return 0;
}

static struct platform_driver sn1202033_pmic_driver = {
	.driver = {
		.name = "sdp_sn1202033",
		.owner = THIS_MODULE,
	},
	.probe = sn1202033_pmic_probe,
	.remove = __devexit_p(sn1202033_pmic_remove),
};

/*
 * sn1202033_pmic_init
 * Module init function
 */
static int __init sn1202033_pmic_init(void)
{
	return platform_driver_register(&sn1202033_pmic_driver);
}
//subsys_initcall(sn1202033_pmic_init);
device_initcall(sn1202033_pmic_init);

/*
 * sn1202033_pmic_cleanup
 * Moduel exit function
 */
static void __exit sn1202033_pmic_cleanup(void)
{
	platform_driver_unregister(&sn1202033_pmic_driver);
}
module_exit(sn1202033_pmic_cleanup);

