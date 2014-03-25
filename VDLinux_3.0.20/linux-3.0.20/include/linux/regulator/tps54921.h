
#ifndef __TPS54921_H
#define __TPS54921_H

#include <linux/regulator/driver.h>
#include <linux/regulator/machine.h>

#if defined(CONFIG_TPS54921_DEBUGGING)
#define TPS54921_DEBUG 1
#else
#define TPS54921_DEBUG 0
#endif

#define tps54921_dbg(fmt, args...) \
	do { if (TPS54921_DEBUG) printk(fmt , ## args); } while (0)

struct tps_info {
	const char *name;
	unsigned min_uV;
	unsigned max_uV;
	u8 table_len;
	const u16 *table;
	int enable;
	int cur_vsel;
};

struct tps54921_pmic {
	struct regulator_desc desc;
	struct regulator_dev *rdev;
	struct tps_info *info;
	struct mutex io_lock;
	int i2c_addr;
	int i2c_port;
	int def_volt;
};

struct tps54921_platform_data {
	int i2c_addr;
	int i2c_port;
	int def_volt;
	struct regulator_init_data *init_data;
};


#endif
