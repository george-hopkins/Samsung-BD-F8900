
#ifndef __SN1202033_H
#define __SN1202033_H

#include <linux/regulator/driver.h>
#include <linux/regulator/machine.h>


#define SN1202033_VOUT1		0
#define SN1202033_VOUT2		1
#define SN1202033_MAXOUT	2

#if defined(CONFIG_SN1202033_DEBUGGING)
#define SN1202033_DEBUG 1
#else
#define SN1202033_DEBUG 0
#endif

#define sn1202033_dbg(fmt, args...) \
	do { if (SN1202033_DEBUG) printk(fmt , ## args); } while (0)

struct sn1202033_info {
	const char *name;
	unsigned min_uV;
	unsigned max_uV;
	u8 table_len;
	const u16 *table;
	int enable;
	int cur_vsel;
};

struct sn1202033_pmic {
	struct regulator_desc desc;
	struct regulator_dev *rdev;
	struct sn1202033_info *info;
	struct mutex io_lock;
	int i2c_addr;
	int i2c_port;
	int def_volt;
	int vout_port;
};

struct sn1202033_platform_data {
	int i2c_addr;
	int i2c_port;
	int def_volt;
	int vout_port;
	struct regulator_init_data *init_data;
};


#endif

