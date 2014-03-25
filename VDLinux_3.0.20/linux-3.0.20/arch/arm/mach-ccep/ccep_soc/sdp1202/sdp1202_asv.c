/* linux/arch/arm/mach-ccep/ccep_soc/sdp1202/sdp1202_asv.c
 *
 * Copyright (c) 2012 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com/
 *
 * SDP1202 - ASV(Adaptive Supply Voltage) driver
 *
 */

#include <linux/init.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/regulator/consumer.h>

#include <plat/asv.h>

#define MP_COUNT	2

static struct sdp_asv_info *sdp1202_asv_info;
unsigned int sdp_result_of_asv;
unsigned int sdp_asv_stored_result;
unsigned int sdp_asv_gpu_ids;
unsigned int sdp_result_of_mp_asv[MP_COUNT];

extern bool sdp_is_dual_mp;
extern unsigned int sdp_revision_id;
unsigned int sdp_mp_rev_id;

/* ASV function for Fused Chip */
#define DEFAULT_ASV_GROUP	1
#define MAX_AP_ASV_GROUP	12
#define MAX_MP_ASV_GROUP	9

struct asv_judge_table sdp1202_limit[MAX_AP_ASV_GROUP];

/* fox-ap es0 ids table */
struct asv_judge_table sdp1202_limit_es0[MAX_AP_ASV_GROUP] = {
	/* IDS, TMCB */
	{    0,  0},	/* Reserved Group (typical fixed) */
	{   25,  0},	/* Reserved Group (typical default) */
	{   37,  0},
	{   49,  0},
	{   59,  0},
	{   69,  0},
	{   87,  0},
	{   88,  0},
	{ 1023, 63},
	{ 1023, 63},
	{ 1023, 63},
	{ 1023, 63},	/* Reserved Group (MAX) */
};
/* fox-ap es1 ids table */
struct asv_judge_table sdp1202_limit_es1[MAX_AP_ASV_GROUP] = {
	/* IDS, TMCB */
	{    0,  0},	/* Reserved Group (typical fixed) */
	{   69,  0},	/* Reserved Group (typical default) */
	{   87,  0},
	{  127,  0},
	{  143,  0},
	{  144,  0},
	{ 1023,  0},
	{ 1023,  0},
	{ 1023, 63},
	{ 1023, 63},
	{ 1023, 63},
	{ 1023, 63},	/* Reserved Group (MAX) */
};

/* MP asv table */
static struct asv_ids_volt_table sdp1202_mp_limit[MAX_MP_ASV_GROUP];

/* mp es0 ids table */
static struct asv_ids_volt_table sdp1202_mp_limit_es0[MAX_MP_ASV_GROUP] = {
	{   0, 1180000},
	{  13, 1180000},
	{  20, 1170000},
	{  34, 1100000},
	{  69, 1090000},
	{  83, 1070000},
	{  98, 1060000},
	{1023, 1000000},
	{1023, 1000000},
};
/* mp es1 ids table */
static struct asv_ids_volt_table sdp1202_mp_limit_es1[MAX_MP_ASV_GROUP] = {
	{   0, 1100000},
	{  30, 1150000},
	{  52, 1100000},
	{  60, 1090000},
	{  67, 1080000},
	{  85, 1070000},
	{ 110, 1050000},
	{ 141,  990000},
	{1023,  970000},
};

static struct regulator *mp_regulator[2];

struct sdp_asv_info * get_sdp_asv_info(void)
{
	return sdp1202_asv_info;
}

static int sdp1202_get_cpu_ids(struct sdp_asv_info *asv_info)
{
	asv_info->cpu_ids = (asv_info->ap_pkg_id >> 16) & 0x1F;
	asv_info->cpu_ids |= (u32)((asv_info->ap_pkg_id >> 41) & 0xF) << 5;
	asv_info->cpu_ids *= 2;
	printk(KERN_DEBUG "cpu_ids = %d\n", asv_info->cpu_ids);

	return 0;
}

static int sdp1202_get_cpu_tmcb(struct sdp_asv_info *asv_info)
{
	asv_info->cpu_tmcb = (asv_info->ap_pkg_id >> 26) & 0x3F;
	printk(KERN_DEBUG "cpu_tmcb = %d\n", asv_info->cpu_tmcb);

	return 0;
}

static int sdp1202_get_gpu_ids(struct sdp_asv_info *asv_info)
{
	asv_info->gpu_ids = (asv_info->ap_pkg_id >> 21) & 0x1F;
	asv_info->gpu_ids |= (u32)((asv_info->ap_pkg_id >> 45) & 0x7) << 5;
	asv_info->gpu_ids |= (u32)((asv_info->ap_pkg_id >> 40) & 0x1) << 8;
	asv_info->gpu_ids *= 2;
	sdp_asv_gpu_ids = asv_info->gpu_ids;
	printk(KERN_DEBUG "gpu_ids = %d\n", asv_info->gpu_ids);

	return 0;
}

static int sdp1202_asv_store_result(struct sdp_asv_info *asv_info)
{
	unsigned int i, k;
	unsigned int mp_count;

	/* find AP group */
	for (i = 0; i < MAX_AP_ASV_GROUP; i++) {
		if (asv_info->cpu_ids <= sdp1202_limit[i].ids_limit) {
			sdp_asv_stored_result = i;
			printk(KERN_INFO "ASV Group %d selected\n", i);
			break;
		}
	}

	/*
	 * If ASV result value is lower than default value
	 * Fix with default value.
	 */
	if ((sdp_asv_stored_result < DEFAULT_ASV_GROUP) ||
		(sdp_asv_stored_result >= MAX_AP_ASV_GROUP))
		sdp_asv_stored_result = DEFAULT_ASV_GROUP;

	printk(KERN_INFO "SDP1202 ASV : CPU IDS : %d, TMCB : %d, GPU IDS : %d, RESULT : group %d\n",
		asv_info->cpu_ids, asv_info->cpu_tmcb, asv_info->gpu_ids,
		sdp_asv_stored_result);

	sdp_result_of_asv = sdp_asv_stored_result;

	/* find MP0, 1 group */
	if (sdp_is_dual_mp)
		mp_count = 2;
	else
		mp_count = 1;
	
	for (k = 0; k < mp_count; k++) {
		for (i = 0; i < MAX_MP_ASV_GROUP; i++) {
			if (asv_info->mp_ids[k] <= sdp1202_mp_limit[i].ids) {
				sdp_result_of_mp_asv[k] = i;
				break;
			}
		}

		printk(KERN_INFO "SDP1202 ASV : select MP%d group %d\n", k, sdp_result_of_mp_asv[k]);

		/* set mp voltage */
		printk(KERN_INFO "SDP1202 ASV : set MP%d voltage %duV\n", k, sdp1202_mp_limit[sdp_result_of_mp_asv[k]].volt);
		if (!IS_ERR(mp_regulator[k]))
			regulator_set_voltage(mp_regulator[k], sdp1202_mp_limit[sdp_result_of_mp_asv[k]].volt,
					    	 sdp1202_mp_limit[sdp_result_of_mp_asv[k]].volt + 10000);
		else
			printk(KERN_ERR "ASV : ERROR - can't set voltage mp_regulator%d. init failed.\n", k);
	}

	udelay(200); /* wait for stable mp voltage */

	/* mp ddr training */
	for (k = 0; k < mp_count; k++) {
		if (sdp_asv_mp_training(k) < 0)
			printk(KERN_ERR "SDP1202 ASV : MP%d training failed\n", k);
	}

	return 0;
}

static void sdp1202_asv_copy_table(void)
{
	int i;
	
	/* ap table */
	if (sdp_revision_id == 0) {
		for (i = 0; i < MAX_AP_ASV_GROUP; i++)
			sdp1202_limit[i] = sdp1202_limit_es0[i];
	} else {
		for (i = 0; i < MAX_AP_ASV_GROUP; i++)
			sdp1202_limit[i] = sdp1202_limit_es1[i];
	}
	
	/* mp table */
	if (sdp_mp_rev_id == 0) {
		for (i = 0; i < MAX_MP_ASV_GROUP; i++)
			sdp1202_mp_limit[i] = sdp1202_mp_limit_es0[i];
	} else {
		for (i = 0; i < MAX_MP_ASV_GROUP; i++)
			sdp1202_mp_limit[i] = sdp1202_mp_limit_es1[i];
	}
}

static int sdp1202_asv_init(struct sdp_asv_info *asv_info)
{
	unsigned int ap_efuse_base = VA_IO_BASE0 + 0x80000;
	void __iomem * mp_efuse_base[MP_COUNT];
	unsigned int mp_base[MP_COUNT] = {0x18080000, 0x1a080000};
	int i, mp_count;
	int timeout = 500;
	char vddmp_name[10];

	sdp_result_of_asv = 0;
	sdp_asv_stored_result = 0;

	printk(KERN_INFO "SDP1202 ASV: Adaptive Support Voltage init\n");

	/* prepare to read ap efuse*/
	writel(0x1F, ap_efuse_base + 0x4);
	while (timeout--) {
		if (__raw_readl(ap_efuse_base) == 0)
			break;
		msleep(1);
	}
	if (!timeout) {
		pr_warn("ap efuse read fail!\n");
		return -1;
	}

	/* get ap package id [32:79] */
	asv_info->ap_pkg_id = __raw_readl(ap_efuse_base + 0x8);
	asv_info->ap_pkg_id |= ((unsigned long long)__raw_readl(ap_efuse_base + 0x10)) << 32;
	printk(KERN_DEBUG "ap_pkg_id = 0x%08X_%08X\n",
			(unsigned int)(asv_info->ap_pkg_id >> 32),
			(unsigned int)(asv_info->ap_pkg_id & 0xffffffff));

	/* dual mp check */
	if (sdp_is_dual_mp)
		mp_count = 2;
	else
		mp_count = 1;

	/* read mp ids, tmcb */
	for (i = 0; i < mp_count; i++) {
		/* mp regulator */
		sprintf(vddmp_name, "VDD_MP%d", i);
		mp_regulator[i] = regulator_get(NULL, vddmp_name);
		if (IS_ERR(mp_regulator[i])) {
			printk(KERN_ERR "ASV : ERROR - failed to get regulator resource %s\n", vddmp_name);
		}

		/* ioremap mp */
		mp_efuse_base[i] = ioremap_nocache(mp_base[i], 0x14);
		if (mp_efuse_base[i] == NULL) {
			printk(KERN_ERR "error mapping mp%d memory\n", i);
			return -1;
		}
		/* prepare to read mp efuse*/
		timeout = 500;
		writel(0x1F, (u32)mp_efuse_base[i] + 0x4);
		while (timeout--) {
			if (__raw_readl(mp_efuse_base[i]) == 0)
				break;
			msleep(1);
		}
		if (!timeout) {
			pr_warn("mp%d efuse read fail!\n", i);
			iounmap(mp_efuse_base[i]);
			return -1;
		}

		/* get mp package id */
		asv_info->mp_pkg_id[i] = __raw_readl((u32)mp_efuse_base[i] + 0xC);
		asv_info->mp_pkg_id[i] = ((unsigned long long)__raw_readl((u32)mp_efuse_base[i] + 0x8)) << 32;
		asv_info->mp_ids[i] = (unsigned int)(asv_info->mp_pkg_id[i] >> 48)&0x3FF;
		asv_info->mp_tmcb[i] = (unsigned int)(asv_info->mp_pkg_id[i] >> 58)&0x3F;
		printk(KERN_DEBUG "mp%d_pkg_id = 0x%08X_%08X\n",
				i, (unsigned int)(asv_info->mp_pkg_id[i] >> 32),
				(unsigned int)(asv_info->mp_pkg_id[i] & 0xffffffff));
		printk(KERN_INFO "SDP1202 ASV : MP%d IDS : %d, TMCB : %d\n",
				i, asv_info->mp_ids[i], asv_info->mp_tmcb[i]);

		iounmap(mp_efuse_base[i]);
	}
	sdp_mp_rev_id = (unsigned int)(asv_info->mp_pkg_id[0] >> 42)&0x3;
	printk(KERN_INFO "SDP1202 ASV : MP ES%d\n", sdp_mp_rev_id);

	/* copy AP and MP ids table */
	sdp1202_asv_copy_table();

	asv_info->get_cpu_ids = sdp1202_get_cpu_ids;
	asv_info->get_cpu_tmcb = sdp1202_get_cpu_tmcb;
	asv_info->get_gpu_ids = sdp1202_get_gpu_ids;
	asv_info->store_result = sdp1202_asv_store_result;

	return 0;
}


static int __init sdp_asv_init(void)
{
	int ret = -EINVAL;

	sdp1202_asv_info = kzalloc(sizeof(struct sdp_asv_info), GFP_KERNEL);
	if (!sdp1202_asv_info)
		goto out1;

	ret = sdp1202_asv_init(sdp1202_asv_info);
	/*
	 * If return value is not zero,
	 * There is already value for asv group.
	 * So, It is not necessary to execute for getting asv group.
	 */
	if (ret) {
		kfree(sdp1202_asv_info);
		return 0;
	}

	if (sdp1202_asv_info->check_vdd_arm) {
		if (sdp1202_asv_info->check_vdd_arm()) {
			printk(KERN_INFO "ASV: It is wrong vdd_arm\n");
			goto out2;
		}
	}

	/* Get CPU IDS Value */
	if (sdp1202_asv_info->get_cpu_ids) {
		if (sdp1202_asv_info->get_cpu_ids(sdp1202_asv_info)) {
			printk(KERN_INFO "ASV: Fail to get CPU IDS Value\n");
			goto out2;
		}
	} else {
		printk(KERN_INFO "ASV: Fail to get CPU IDS Value\n");
		goto out2;
	}

	/* Get CPU TMCB Value */
	if (sdp1202_asv_info->get_cpu_tmcb) {
		if (sdp1202_asv_info->get_cpu_tmcb(sdp1202_asv_info)) {
			printk(KERN_INFO "ASV: Fail to get CPU TMCB Value\n");
			goto out2;
		}
	} else {
		printk(KERN_INFO "ASV: Fail to get CPU TMCB Value\n");
		goto out2;
	}

	/* Get GPU IDS Value */
	if (sdp1202_asv_info->get_gpu_ids) {
		if (sdp1202_asv_info->get_gpu_ids(sdp1202_asv_info)) {
			printk(KERN_INFO "ASV: Fail to get GPU IDS Value\n");
			goto out2;
		}
	} else {
		printk(KERN_INFO "ASV: Fail to get GPU IDS Value\n");
		goto out2;
	}

	if (sdp1202_asv_info->store_result) {
		if (sdp1202_asv_info->store_result(sdp1202_asv_info)) {
			printk(KERN_INFO "ASV: Can not success to store result\n");
			goto out2;
		}
	} else {
		printk(KERN_INFO "ASV: No store_result function\n");
		goto out2;
	}

	return 0;
out2:
	kfree(sdp1202_asv_info);
out1:
	return -EINVAL;
}
device_initcall_sync(sdp_asv_init);

