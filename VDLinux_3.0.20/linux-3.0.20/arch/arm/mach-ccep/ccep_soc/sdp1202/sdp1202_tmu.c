/* linux/arch/arm/mach-ccep/ccep_soc/sdp1202/sdp1202_tmu.c
*
* Copyright (c) 2012 Samsung Electronics Co., Ltd.
*      http://www.samsung.com
*
* SDP - Thermal Management support
*
*/

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/power_supply.h>
#include <linux/interrupt.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/irq.h>
#include <linux/gpio.h>
#include <linux/slab.h>
#include <linux/kobject.h>

#include <asm/irq.h>

#include <plat/cpufreq.h>
#include <plat/sdp_tmu.h>
#include <plat/asv.h>

/* Register define */
#define SDP1202_TSC_CONTROL0	0x0
#define SDP1202_TSC_CONTROL1	0x4
#define SDP1202_TSC_CONTROL2	0x8
#define SDP1202_TSC_CONTROL3	0xC
#define SDP1202_TSC_CONTROL4	0x10
#define TSC_TEM_T_EN		0
#define SDP1202_TSC_CONTROL5	0x14
#define TSC_TEM_T_TS_8BIT_TS	12

#define SAMPLING_RATE		(2 * 1000 * 1000) /* u sec */
#define MONITOR_PERIOD		(10 * 1000 * 1000) /* u sec */
#define PANIC_TIME_SEC		(10 * 60 * 1000 * 1000 / SAMPLING_RATE) /* 10 minutes */
#define PRINT_RATE			(6 * 1000 * 1000) /* 6 sec */

static enum {
	ENABLE_TEMP_MON	= 0x1,
	ENABLE_TEST_MODE = 0x2,
} enable_mask = ENABLE_TEMP_MON | ENABLE_TEST_MODE;
module_param_named(enable_mask, enable_mask, uint, 0644);
//#define ENABLE_DBGMASK (ENABLE_TEMP_MON | ENABLE_TEST_MODE)
#define ENABLE_DBGMASK (ENABLE_TEST_MODE)

/* for factory mode */
#define CONFIG_TMU_SYSFS
#define CONFIG_TMU_DEBUG

/* flags that throttling or trippint is treated */
#define THROTTLE_ZERO_FLAG (0x1 << 0)
#define THROTTLE_1ST_FLAG  (0x1 << 1)
#define THROTTLE_2ND_FLAG  (0x1 << 2)
#define THROTTLE_3RD_FLAG  (0x1 << 3)

#define TIMING_AREF_OFFSET	0x30

#define LONGNAME_SIZE		50
#define LOG_BUF_LEN			255
#ifdef CONFIG_ARCH_SDP1202_EVAL	
const char *TMU_LOG_FILE = "/rwarea/ThermalThrottle.txt";
#else
const char *TMU_LOG_FILE = "/mtd_rwarea/ThermalThrottle.txt";
#endif

static struct workqueue_struct  *tmu_monitor_wq;

static DEFINE_MUTEX(tmu_lock);

static bool throt_on = true;
static int tmu_print_temp_on_off = false;
static int tmu_print_temp_throt_on = false;

static int tmu_1st_throttle_count = 0;
static int tmu_2nd_throttle_count = 0;
static int tmu_3rd_throttle_count = 0;

static unsigned int get_curr_temp(struct sdp_tmu_info *info)
{
	int temp;
	static int print_delay = PRINT_RATE;

	/* get temperature from TSC register */
	temp = (__raw_readl((u32)info->tmu_base + SDP1202_TSC_CONTROL5) >> TSC_TEM_T_TS_8BIT_TS) & 0xFF;
	temp += info->fusing_diffval;
	temp -= 46 - 25; /* 25'C is 46 */
	if (temp < 25) {
		//pr_info("TMU: temperature is lower than 25'C. %d'C\n", temp);
		temp = 25;
	}

	/* Temperature is printed every PRINT_RATE. */ 
	if (tmu_print_temp_on_off || tmu_print_temp_throt_on) {
		print_delay -= SAMPLING_RATE;
		if (print_delay <= 0) {
			printk(KERN_INFO "\033[1;7;33mT^%d'C\033[0m\n", temp);
			print_delay = PRINT_RATE;
		}
	}
	
	return (unsigned int)temp;
}

static void write_log(unsigned int throttle)
{
	char tmu_longname[LONGNAME_SIZE];
	static char tmu_logbuf[LOG_BUF_LEN];
	int len;
	struct file *fp;
	
	if (throttle == TMU_1ST_THROTTLE)
		tmu_1st_throttle_count++;
	else if (throttle == TMU_2ND_THROTTLE)
		tmu_2nd_throttle_count++;
	else if (throttle == TMU_3RD_THROTTLE)
		tmu_3rd_throttle_count++;
	else
		printk(KERN_INFO "TMU: %s - throttle level is not valid. %u\n",
				__func__, throttle);

	printk(KERN_INFO "TMU: %s - 1st = %d, 2nd = %d, 3rd = %d\n", __func__,
			tmu_1st_throttle_count, tmu_2nd_throttle_count, tmu_3rd_throttle_count);

	snprintf(tmu_longname, LONGNAME_SIZE, TMU_LOG_FILE);
	printk("tmu_longname : %s\n", tmu_longname);

	fp = filp_open(tmu_longname, O_CREAT|O_WRONLY|O_TRUNC|O_LARGEFILE, 0644);
	if (IS_ERR(fp)) {
		printk(KERN_ERR "TMU: error in opening tmu log file.\n");
		return;
	}

	sprintf(tmu_logbuf, 
		"1st throttle count = %d\n"
		"2nd throttle count = %d\n"
		"3rd throttle count = %d\n",
		tmu_1st_throttle_count, tmu_2nd_throttle_count, tmu_3rd_throttle_count);
	len = strlen(tmu_logbuf);
	
	fp->f_op->write(fp, tmu_logbuf, len, &fp->f_pos);

	filp_close(fp, NULL);	
}

static ssize_t show_temperature(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct sdp_tmu_info *info = dev_get_drvdata(dev);
	unsigned int temperature;

	if (!dev)
		return -ENODEV;

	mutex_lock(&tmu_lock);

	temperature = get_curr_temp(info);

	mutex_unlock(&tmu_lock);

	return sprintf(buf, "%u\n", temperature);
}

static ssize_t show_tmu_state(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct sdp_tmu_info *info = dev_get_drvdata(dev);

	if (!dev)
		return -ENODEV;

	return sprintf(buf, "%d\n", info->tmu_state);
}

static ssize_t show_lot_id(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	unsigned int base = VA_IO_BASE0 + 0x80000;
	int diff;

	/* read efuse */
	diff = __raw_readl(base + 0x10) & 0xFF;

	return sprintf(buf, "fusing val = %d\n", diff);
}

static ssize_t show_throttle_on(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", throt_on);
}

static ssize_t store_throttle_on(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	int ret = 0;
	unsigned int on;

	ret = sscanf(buf, "%u", &on);
	if (ret != 1) {
		printk(KERN_ERR "%s invalid arg\n", __func__);
		return -EINVAL;
	}

	if (on == 1) {
		if (throt_on) {
			printk(KERN_ERR "TMU: throttle already ON.\n");
			goto out;
		}
		throt_on = true;
		dev_info(dev, "throttle ON\n");
	} else if (on == 0) {
		if (!throt_on) {
			printk(KERN_ERR "TMU: throttle already OFF\n");
			goto out;
		}
		throt_on = false;
		dev_info(dev, "throttle OFF\n");
	} else {
		dev_err(dev, "Invalid value!! %d\n", on);
		return -EINVAL;
	}

out:
	return (ssize_t)count;
}

static DEVICE_ATTR(temperature, 0444, show_temperature, NULL);
static DEVICE_ATTR(tmu_state, 0444, show_tmu_state, NULL);
static DEVICE_ATTR(lot_id, 0444, show_lot_id, NULL);
static DEVICE_ATTR(throttle_on, 0644, show_throttle_on, store_throttle_on);

static void print_temperature_params(struct sdp_tmu_info *info)
{
	struct sdp_platform_tmu *pdata = info->dev->platform_data;

	pr_info("TMU - temperature set value\n");
	pr_info("zero throttling start_temp = %u, stop_temp      = %u\n",
		pdata->ts.start_zero_throttle, pdata->ts.stop_zero_throttle);
	pr_info("1st  throttling stop_temp  = %u, start_temp     = %u\n",
		pdata->ts.stop_1st_throttle, pdata->ts.start_1st_throttle);
	pr_info("2nd  throttling stop_temp  = %u, start_temp     = %u\n",
		pdata->ts.stop_2nd_throttle, pdata->ts.start_2nd_throttle);
	pr_info("3rd  throttling stop_temp  = %u, start_temp     = %u\n",
		pdata->ts.stop_3rd_throttle, pdata->ts.start_3rd_throttle);
}

#if 0
static unsigned int get_refresh_interval(unsigned int freq_ref,
					unsigned int refresh_nsec)
{
	unsigned int uRlk, refresh = 0;

	/*
	 * uRlk = FIN / 100000;
	 * refresh_usec =  (unsigned int)(fMicrosec * 10);
	 * uRegVal = ((unsigned int)(uRlk * uMicroSec / 100)) - 1;
	 * refresh =
	 * (unsigned int)(freq_ref * (unsigned int)(refresh_usec * 10) / 100) - 1;
	*/
	uRlk = freq_ref / 1000000;
	refresh = ((unsigned int)(uRlk * refresh_nsec / 1000));

	pr_info("tmu - get_refresh_interval = 0x%02x\n", refresh);
	return refresh;
}
#endif

struct tmu_early_param {
	int set_ts;
	struct temperature_params ts;
	int set_lock;
	unsigned cpufreq_level_1st_throttle;
	unsigned cpufreq_level_2nd_throttle;
	int set_rate;
	unsigned int sampling_rate;
	unsigned int monitor_rate;
};
static struct tmu_early_param tmu_in;

static int __init get_temperature_params(char *str)
{
	int ints[11];

	unsigned int mask = (enable_mask & ENABLE_DBGMASK);

	if (!(mask & ENABLE_TEST_MODE))
		return -EPERM;

	get_options(str, ARRAY_SIZE(ints), ints);

	/*  output the input value */
	//pr_info("tmu_test=%s\n", str);

	if (ints[0])
		tmu_in.set_ts = 1;
	if (ints[0] > 0)
		tmu_in.ts.stop_1st_throttle = (unsigned int)ints[1];
	if (ints[0] > 1)
		tmu_in.ts.start_1st_throttle = (unsigned int)ints[2];
	if (ints[0] > 2)
		tmu_in.ts.stop_2nd_throttle = (unsigned int)ints[3];
	if (ints[0] > 3)
		tmu_in.ts.start_2nd_throttle = (unsigned int)ints[4];
	if (ints[0] > 4)
		tmu_in.ts.stop_3rd_throttle = (unsigned int)ints[5];
	if (ints[0] > 5)
		tmu_in.ts.stop_3rd_throttle = (unsigned int)ints[6];

	/*  output the input value */
	pr_info("1st throttling temp: start[%u], stop[%u]\n"
		"2nd throttling temp: start[%u], stop[%u]\n"
		"3rd throttling temp: start[%u], stop[%u]\n",
		tmu_in.ts.start_1st_throttle, tmu_in.ts.stop_1st_throttle,
		tmu_in.ts.start_2nd_throttle, tmu_in.ts.stop_2nd_throttle,
		tmu_in.ts.start_3rd_throttle, tmu_in.ts.stop_3rd_throttle);

	return 0;
}
early_param("tmu_test", get_temperature_params);

static int __init get_cpufreq_limit_param(char *str)
{
	int ints[3];
	unsigned int mask = (enable_mask & ENABLE_DBGMASK);

	if (!(mask & ENABLE_TEST_MODE))
		return -EPERM;

	get_options(str, ARRAY_SIZE(ints), ints);
	/*  output the input value */
	pr_info("cpu_level=%s\n", str);

	if (ints[0])
		tmu_in.set_lock = 1;
	if (ints[0] > 0)
		tmu_in.cpufreq_level_1st_throttle = (unsigned int)ints[1];
	if (ints[0] > 1)
		tmu_in.cpufreq_level_2nd_throttle = (unsigned int)ints[2];

	pr_info("tmu - cpufreq_limit: 1st cpu_level = %u, 2nd cpu_level = %u\n",
		tmu_in.cpufreq_level_1st_throttle,
		tmu_in.cpufreq_level_2nd_throttle);

	return 0;
}
early_param("cpu_level", get_cpufreq_limit_param);

static int __init get_sampling_rate_param(char *str)
{
	int ints[3];
	unsigned int mask = (enable_mask & ENABLE_DBGMASK);

	if (!(mask & ENABLE_TEST_MODE))
		return -EPERM;

	get_options(str, ARRAY_SIZE(ints), ints);
	/*  output the input value */
	pr_info("tmu_sampling_rate=%s\n", str);

	if (ints[0])
		tmu_in.set_rate = 1;
	if (ints[0] > 0)
		tmu_in.sampling_rate = (unsigned int)ints[1];
	if (ints[0] > 1)
		tmu_in.monitor_rate = (unsigned int)ints[2];

	pr_info("tmu - sampling_rate = %u ms, monitor_rate = %u ms\n",
		tmu_in.sampling_rate, tmu_in.monitor_rate);

	return 0;
}
early_param("tmu_sampling_rate", get_sampling_rate_param);

static void sdp_poll_cur_temp(struct work_struct *work)
{
	unsigned int cur_temp;
	struct delayed_work *delayed_work = to_delayed_work(work);
	struct sdp_tmu_info *info =
	    container_of(delayed_work, struct sdp_tmu_info, monitor);
	unsigned int mask = (enable_mask & ENABLE_DBGMASK);

	mutex_lock(&tmu_lock);

	if (mask & ENABLE_TEMP_MON) {
		cur_temp = get_curr_temp(info);

#if 0
		if (tmu_print_temp_on_off)
			pr_info("curr temp in polling_interval = %u state = %d\n",
				cur_temp, info->tmu_state);
		else
#endif			
			pr_debug("curr temp in polling_interval = %u\n", cur_temp);
	}
	queue_delayed_work_on(0, tmu_monitor_wq, &info->monitor,
			      info->monitor_period);

	mutex_unlock(&tmu_lock);
}

#ifdef CONFIG_TMU_DEBUG
static ssize_t tmu_show_print_state(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	int ret;

	ret = sprintf(buf, "[TMU] tmu_print_temp_on_off=%d\n"
					, tmu_print_temp_on_off);

	return ret;
}

static ssize_t tmu_store_print_state(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	int ret = 0;
	unsigned int on;

	ret = sscanf(buf, "%u", &on);
	if (ret != 1) {
		printk(KERN_ERR "%s invalid arg\n", __func__);
		return -EINVAL;
	}

	if (on)
		tmu_print_temp_on_off = true;
	else
		tmu_print_temp_on_off = false;

	return (ssize_t)count;
}
static DEVICE_ATTR(print_state, S_IRUGO | S_IWUSR,\
	tmu_show_print_state, tmu_store_print_state);
#endif

#if 0
static void set_refresh_rate(unsigned int auto_refresh)
{
	/*
	 * uRlk = FIN / 100000;
	 * refresh_usec =  (unsigned int)(fMicrosec * 10);
	 * uRegVal = ((unsigned int)(uRlk * uMicroSec / 100)) - 1;
	*/
	pr_debug("set_auto_refresh = 0x%02x\n", auto_refresh);
}
#endif

static void read_fusing_diffvalue(struct sdp_tmu_info *info)
{
	unsigned int base = VA_IO_BASE0 + 0x80000;
	int diff = 0;
	int timeout = 500;
	
	/* prepare to read */
	__raw_writel(0x1F, base + 0x4);
	while (timeout--) {
		if (__raw_readl(base) == 0)
			break;
		msleep(1);
	}
	if (!timeout) {
		pr_warn("efuse read fail!\n");
		goto out_diff;
	}

	/* read efuse */
	diff = __raw_readl(base + 0x10) & 0xFF;

	if (diff < 37 || diff > 55)
		pr_warn("fusing value is abnormal. %d\n", diff);

	pr_info("TMU: fusing value : %d (diff=%d)\n", diff, 46 - diff);

	diff = 46 - diff;
	
out_diff:
	info->fusing_diffval = diff;
	
	return;
}

static void set_temperature_params(struct sdp_tmu_info *info)
{
	struct sdp_platform_tmu *data = info->dev->platform_data;

	/* In the tmu_test mode, change temperature_params value
	 * input data.
	*/
	if (tmu_in.set_ts)
		data->ts = tmu_in.ts;
	if (tmu_in.set_lock) {
		info->cpufreq_level_1st_throttle =
				tmu_in.cpufreq_level_1st_throttle;
		info->cpufreq_level_2nd_throttle =
				tmu_in.cpufreq_level_2nd_throttle;
	}
	if (tmu_in.set_rate) {
		info->sampling_rate =
			usecs_to_jiffies(tmu_in.sampling_rate * 1000);
		info->monitor_period =
			usecs_to_jiffies(tmu_in.monitor_rate * 1000);
	}
	print_temperature_params(info);
}

#if 0
static int notify_change_of_tmu_state(struct sdp_tmu_info *info)
{
	char temp_buf[20];
	char *envp[2];
	int env_offset = 0;

	snprintf(temp_buf, sizeof(temp_buf), "TMUSTATE=%d", info->tmu_state);
	envp[env_offset++] = temp_buf;
	envp[env_offset] = NULL;

	pr_info("%s: uevent: %d, name = %s\n",
			__func__, info->tmu_state, temp_buf);

	return kobject_uevent_env(&info->dev->kobj, KOBJ_CHANGE, envp);
}
#endif

/* limit cpu and gpu frequency */
static void sdp_tmu_limit(struct sdp_tmu_info* info, unsigned int throttle)
{
	struct sdp_platform_tmu *data = info->dev->platform_data;

	if (!throt_on) {
		printk(KERN_INFO "TMU: thermal throttle is OFF now.");
		return;
	}
	
	/* 1st throttle */
	if (throttle == TMU_1ST_THROTTLE) {
		pr_info("TMU: 1st limit\n");
		tmu_print_temp_throt_on = true;
		
		/* cpu */
		if (info->cpufreq_level_1st_throttle == TMU_INV_FREQ_LEVEL) {
			sdp_cpufreq_get_level(data->cpufreq.limit_1st_throttle,
								&info->cpufreq_level_1st_throttle);
			pr_info("TMU: CPU 1st throttle level = %d\n", info->cpufreq_level_1st_throttle);
		}
		sdp_cpufreq_upper_limit(DVFS_LOCK_ID_TMU,
				info->cpufreq_level_1st_throttle);

		/* gpu */
		if (info->gpu_freq_limit) {
			info->gpu_freq_limit((int)info->gpufreq_level_1st_throttle);
			pr_info("TMU: GPU 1st throttle freq = %d\n",
					info->gpufreq_level_1st_throttle);
		}
	}
	/* 2nd throttle */
	else if (throttle == TMU_2ND_THROTTLE) {
		pr_info("TMU: 2nd limit\n");
		tmu_print_temp_throt_on = true;
		
		/* cpu */
		if (info->cpufreq_level_2nd_throttle == TMU_INV_FREQ_LEVEL) {
			sdp_cpufreq_get_level(data->cpufreq.limit_2nd_throttle,
								&info->cpufreq_level_2nd_throttle);
			pr_info("TMU: CPU 2nd throttle level = %d\n", info->cpufreq_level_2nd_throttle);
		}
		sdp_cpufreq_upper_limit(DVFS_LOCK_ID_TMU,
				info->cpufreq_level_2nd_throttle);
		
		/* gpu */
		if (info->gpu_freq_limit) {
			info->gpu_freq_limit((int)info->gpufreq_level_2nd_throttle);
			pr_info("TMU: GPU 2nd throttle freq = %d\n",
					info->gpufreq_level_2nd_throttle);
		}
	}
	/* 3rd throttle */
	else if (throttle == TMU_3RD_THROTTLE) {
		pr_info("TMU: 3rd limit. emergency call.\n");
		tmu_print_temp_throt_on = true;
		
		/* cpu */
		if (info->cpufreq_level_3rd_throttle == TMU_INV_FREQ_LEVEL) {
			sdp_cpufreq_get_level(data->cpufreq.limit_3rd_throttle,
								&info->cpufreq_level_3rd_throttle);
			pr_info("TMU: CPU 3rd throttle level = %d\n", info->cpufreq_level_3rd_throttle);
		}
		sdp_cpufreq_upper_limit(DVFS_LOCK_ID_TMU,
				info->cpufreq_level_3rd_throttle);
		
		/* gpu */
		if (info->gpu_freq_limit) {
			info->gpu_freq_limit((int)info->gpufreq_level_3rd_throttle);
			pr_info("TMU: GPU 3rd throttle freq = %d\n",
					info->gpufreq_level_3rd_throttle);
		}

		/* call emergency limit */
		/* cpu */
		sdp_cpufreq_emergency_limit();

		/* gpu */
		if (info->gpu_freq_emergency_limit) {
			info->gpu_freq_emergency_limit();
			pr_info("TMU: GPU emergency limit called\n");
		}
	}
	else {
		pr_err("TMU: %s - throttle level error. %d\n", __func__, throttle);
	}
}

/* free cpu and gpu frequency limitation */
static void sdp_tmu_limit_free(struct sdp_tmu_info* info, int check_handle)
{
	/* free emergency limit when 3RD throttle */
	if (check_handle & THROTTLE_3RD_FLAG) {
		/* cpu */
		sdp_cpufreq_emergency_limit_free();

		/* gpu */
		if (info->gpu_freq_emergency_limit_free) {
			info->gpu_freq_emergency_limit_free();
			pr_info("TMU: free GPU emergency limit\n");
		}
	}

	/* free limit */
	tmu_print_temp_throt_on = false;
	
	/* cpu */
	sdp_cpufreq_upper_limit_free(DVFS_LOCK_ID_TMU);
	pr_info("TMU: free cpu freq limit\n");
	
	/* gpu */
	if (info->gpu_freq_limit_free) {
		info->gpu_freq_limit_free();
		pr_info("TMU: free gpu freq limit\n");
	}
}

static int sdp_tmu_apply_avs(struct sdp_tmu_info *info)
{
	unsigned int temp = get_curr_temp(info);
	struct sdp_platform_tmu *pdata = info->dev->platform_data;
	int check_handle = 0;
	bool dvfs_save;

	/* AVS ON when temp is bigger than start_zero_throttle */
	if (temp > pdata->ts.start_zero_throttle) {
		pr_info("TMU: temp = %d'C, AVS ON\n", temp);
		/* avs on */
		sdp_cpufreq_set_asv_on(true);
		/* restore dvfs value */
		dvfs_save = sdp_cpufreq_is_cpufreq_on();
		sdp_cpufreq_set_cpufreq_on(dvfs_save);
		sdp_cpufreq_set_tmu_control(false);
		
	/* AVS OFF/ DVFS OFF when temp is less than or equal to start_zero_throttle */
	} else {
		pr_info("TMU: temp = %d'C, AVS OFF\n", temp);
		/* dvfs off */
		sdp_cpufreq_set_tmu_control(true);
		sdp_cpufreq_set_cpufreq_on(false);
		/* avs off. move to group 0 */
		sdp_cpufreq_set_asv_on(false);
		check_handle |= THROTTLE_ZERO_FLAG;
	}

	return check_handle;
}

static void sdp_handler_tmu_state(struct work_struct *work)
{
	struct delayed_work *delayed_work = to_delayed_work(work);
	struct sdp_tmu_info *info =
		container_of(delayed_work, struct sdp_tmu_info, polling);
	struct sdp_platform_tmu *data = info->dev->platform_data;
	unsigned int cur_temp;
//	static int auto_refresh_changed;
	static int check_handle;
	int trend = 0;
	bool dvfs_save;
	static int count_down = PANIC_TIME_SEC;

	mutex_lock(&tmu_lock);

	cur_temp = get_curr_temp(info);
	trend = (int)cur_temp - (int)info->last_temperature;
	//pr_debug("curr_temp = %u, temp_diff = %d\n", cur_temp, trend);

	switch (info->tmu_state) {
	case TMU_STATUS_ZERO:
		/* 1. zero throttle. dvfs off */
		if (cur_temp <= data->ts.start_zero_throttle &&
			!(check_handle & THROTTLE_ZERO_FLAG)) {
			/* if limit is set, free */
			if (check_handle &
				(THROTTLE_1ST_FLAG | THROTTLE_2ND_FLAG | THROTTLE_3RD_FLAG)) {
				sdp_tmu_limit_free(info, check_handle);
				check_handle = 0;
				pr_info("TMU: zero throttle. all limit free.\n");
			}
			/* dvfs off */
			sdp_cpufreq_set_tmu_control(true);
			sdp_cpufreq_set_cpufreq_on(false);
			/* avs off. move to group 0 */
			sdp_cpufreq_set_asv_on(false);
			check_handle |= THROTTLE_ZERO_FLAG;

			/* store current state */
			info->tmu_prev_state = info->tmu_state;
		/* 2. change to NORMAL */
		} else if (cur_temp >= data->ts.stop_zero_throttle) {
			info->tmu_state = TMU_STATUS_NORMAL;
			pr_info("TMU: change state: zero -> normal. %d'C\n", cur_temp);
		/* 3. polling */
		} else {
			if (!(check_handle & THROTTLE_ZERO_FLAG))
				pr_info("TMU: polling. zero throttle is not activated yet. %d'C\n", cur_temp);
		}
		break;
		
	case TMU_STATUS_NORMAL:
		/* 1. change to ZERO THROTTLE */
		if (cur_temp <= data->ts.start_zero_throttle) {
			info->tmu_state = TMU_STATUS_ZERO;
			pr_info("TMU: change state: normal -> zero throttle. %d'C\n", cur_temp);
		/* 2. change to 1ST THROTTLE */
		} else if (cur_temp >= data->ts.start_1st_throttle) {
			info->tmu_state = TMU_STATUS_1ST;
			pr_info("TMU: change state: normal -> 1st throttle. %d'C\n", cur_temp);
		/* 3. all limit free or dvfs/avs on */
		} else if (cur_temp > data->ts.start_zero_throttle &&
					cur_temp <= data->ts.stop_1st_throttle &&
					check_handle) {
			if (check_handle & THROTTLE_ZERO_FLAG) {
				/* avs on */
				sdp_cpufreq_set_asv_on(true);
				/* restore dvfs value */
				dvfs_save = sdp_cpufreq_is_cpufreq_on();
				sdp_cpufreq_set_cpufreq_on(dvfs_save);
				sdp_cpufreq_set_tmu_control(false);
				check_handle &= ~THROTTLE_ZERO_FLAG;
			} else if (check_handle &
				(THROTTLE_1ST_FLAG | THROTTLE_2ND_FLAG | THROTTLE_3RD_FLAG)) {
				sdp_tmu_limit_free(info, check_handle);
				check_handle = 0;
			}

			/* store current state */
			info->tmu_prev_state = info->tmu_state;
			
			pr_info("TMU: current status is NORMAL. %d'C\n", cur_temp);
			pr_debug("TMU: check_handle = %d. %d'C\n", check_handle, cur_temp);
		} else {
			pr_debug("TMU: NORMAL polling. %d'C\n", cur_temp);
		}
		break;

	case TMU_STATUS_1ST:
		/* 1. change to NORMAL */
		if ((cur_temp <= data->ts.stop_1st_throttle) &&
			(trend < 0)) {
			info->tmu_state = TMU_STATUS_NORMAL;
			pr_info("TMU: change state: 1st throttle -> normal. %d'C\n", cur_temp);
		/* 2. change to 2ND THROTTLE */
		} else if (cur_temp >= data->ts.start_2nd_throttle) {
			info->tmu_state = TMU_STATUS_2ND;
			pr_info("TMU: change state: 1st throttle -> 2nd throttle. %d'C\n", cur_temp);
		/* 3. 1ST THROTTLE - cpu, gpu limitation */
		} else if ((cur_temp >= data->ts.start_1st_throttle) &&
				!(check_handle & THROTTLE_1ST_FLAG)) {
			if (check_handle & (THROTTLE_2ND_FLAG | THROTTLE_3RD_FLAG)) {
				sdp_tmu_limit_free(info, check_handle);
				check_handle = 0;
			}
			
			sdp_tmu_limit(info, TMU_1ST_THROTTLE);
			/* write tmu log */
			if (info->tmu_prev_state < info->tmu_state)
				write_log(TMU_1ST_THROTTLE);

			check_handle |= THROTTLE_1ST_FLAG;
			pr_debug("check_handle = %d\n", check_handle);
			
			/* store current state */
			info->tmu_prev_state = info->tmu_state;
		} else {
			pr_debug("TMU: 1ST THROTTLE polling. %d'C\n", cur_temp);
		}
		break;

	case TMU_STATUS_2ND:
		/* 1. change to 1ST THROTTLE */
		if ((cur_temp <= data->ts.stop_2nd_throttle) &&
			(trend < 0)) {
			info->tmu_state = TMU_STATUS_1ST;
			pr_info("TMU: change state: 2nd throttle -> 1st throttle. %d'C\n", cur_temp);
		/* 2. change to 3RD THROTTLE */
		} else if (cur_temp >= data->ts.start_3rd_throttle) {
			info->tmu_state = TMU_STATUS_3RD;
			pr_info("TMU: change state: 2nd throttle -> 3rd throttle. %d'C\n", cur_temp);
		/* 3. 2ND THROTTLE - cpu, gpu limitation */
		} else if ((cur_temp >= data->ts.start_2nd_throttle) &&
			!(check_handle & THROTTLE_2ND_FLAG)) {
			if (check_handle & (THROTTLE_1ST_FLAG | THROTTLE_3RD_FLAG)) {
				sdp_tmu_limit_free(info, check_handle);
				check_handle = 0;
			}

			sdp_tmu_limit(info, TMU_2ND_THROTTLE);
			/* write tmu log */
			if (info->tmu_prev_state < info->tmu_state)
				write_log(TMU_2ND_THROTTLE);

			check_handle |= THROTTLE_2ND_FLAG;
			pr_debug("check_handle = %d\n", check_handle);

			/* store current state */
			info->tmu_prev_state = info->tmu_state;
			
			pr_info("TMU: 2nd throttle: cpufreq is limited. level %d. %d'C\n",
					info->cpufreq_level_2nd_throttle, cur_temp);
		} else {
			pr_debug("TMU: 2ND THROTTLE polling. %d'C\n", cur_temp);
		}
		break;

	case TMU_STATUS_3RD:
		/* 1. change to 2ND THROTTLE */
		if ((cur_temp <= data->ts.stop_3rd_throttle) && (trend < 0)) {
			info->tmu_state = TMU_STATUS_2ND;
			pr_info("TMU: change state: 3rd throttle -> 2nd throttle. %d'C\n", cur_temp);
		/* 2. cpufreq limitation */
		} else if ((cur_temp >= data->ts.start_3rd_throttle) &&
			!(check_handle & THROTTLE_3RD_FLAG)){
			if (check_handle & ~(THROTTLE_3RD_FLAG)) {
				sdp_tmu_limit_free(info, check_handle);
				check_handle = 0;
			}

			/* call emergency limit */
			sdp_tmu_limit(info, TMU_3RD_THROTTLE);

			/* write tmu log */
			if (info->tmu_prev_state < info->tmu_state)
				write_log(TMU_3RD_THROTTLE);
			
			count_down = PANIC_TIME_SEC;
			
			check_handle |= THROTTLE_3RD_FLAG;
			pr_debug("check_handle = %d\n", check_handle);

			/* store current state */
			info->tmu_prev_state = info->tmu_state;
			
			pr_info("TMU: 3rd throttle: cpufreq is limited. level %d. %d'C\n",
					info->cpufreq_level_3rd_throttle, cur_temp);
		
		} else {
			pr_debug("TMU: 3RD THROTTLE polling. %d'C\n", cur_temp);
			
#if 0
			if (check_handle & THROTTLE_3RD_FLAG)
				printk("\033[1;7;41m>>>>> PANIC COUNT DOWN %d!!!\033[0m\n", count_down--);

			/* PANIC */
			if (count_down <= 0)
				panic("\033[1;7;41mTMU: CHIP is VERY HOT!\033[0m\n");
#endif			
		}
		break;

	case TMU_STATUS_INIT:
		/* send tmu initial status */
		if (cur_temp >= data->ts.start_3rd_throttle) {
			info->tmu_state = TMU_STATUS_3RD;
			info->tmu_prev_state = TMU_STATUS_3RD;
		} else if (cur_temp >= data->ts.start_2nd_throttle) {
			info->tmu_state = TMU_STATUS_2ND;
			info->tmu_prev_state = TMU_STATUS_2ND;
		} else if (cur_temp >= data->ts.start_1st_throttle) {
			info->tmu_state = TMU_STATUS_1ST;
			info->tmu_prev_state = TMU_STATUS_1ST;
		} else if (cur_temp > data->ts.start_zero_throttle) {
			info->tmu_state = TMU_STATUS_NORMAL;
			info->tmu_prev_state = TMU_STATUS_NORMAL;
		} else {
			info->tmu_state = TMU_STATUS_ZERO;
			info->tmu_prev_state = TMU_STATUS_ZERO;
		}

		/* apply AVS */
		check_handle = sdp_tmu_apply_avs(info);

		pr_info("TMU: init state is %d\n", info->tmu_state);
		break;

	default:
		pr_warn("Bug: checked tmu_state. %d\n", info->tmu_state);
		info->tmu_state = TMU_STATUS_INIT;
		
		break;
	} /* end */

	info->last_temperature = cur_temp;

	/* reschedule the next work */
	queue_delayed_work_on(0, tmu_monitor_wq, &info->polling,
			info->sampling_rate);

	mutex_unlock(&tmu_lock);

	return;
}

static int sdp1202_tmu_init(struct sdp_tmu_info *info)
{
	unsigned int tmp;
	
	/* Temperature sensor enable */
	tmp = __raw_readl((u32)info->tmu_base + SDP1202_TSC_CONTROL4);
	tmp |= (1 << TSC_TEM_T_EN);
	__raw_writel(tmp, (u32)info->tmu_base + SDP1202_TSC_CONTROL4);

	mdelay(1);

	return 0;
}

static int tmu_initialize(struct platform_device *pdev)
{
	struct sdp_tmu_info *info = platform_get_drvdata(pdev);
	int ret;

	ret = sdp1202_tmu_init(info);

	return ret;
}

#ifdef CONFIG_TMU_SYSFS
static ssize_t sdp_tmu_show_curr_temp(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct sdp_tmu_info *info = dev_get_drvdata(dev);
	unsigned int curr_temp;

	curr_temp = get_curr_temp(info);
	pr_info("curr temp = %d\n", curr_temp);

	return sprintf(buf, "%d\n", curr_temp);
}
static DEVICE_ATTR(curr_temp, S_IRUGO, sdp_tmu_show_curr_temp, NULL);
#endif

struct sdp_tmu_info* tmu_info;
struct sdp_tmu_info* sdp_tmu_get_info(void)
{
	return tmu_info;
}
EXPORT_SYMBOL(sdp_tmu_get_info);

static int __devinit sdp_tmu_probe(struct platform_device *pdev)
{
	struct sdp_tmu_info *info;
	struct sdp_platform_tmu *pdata;
	struct resource *res;
	unsigned int mask = (enable_mask & ENABLE_DBGMASK);
	int ret = 0;

	pr_debug("%s: probe=%p\n", __func__, pdev);

	info = kzalloc(sizeof(struct sdp_tmu_info), GFP_KERNEL);
	if (!info) {
		dev_err(&pdev->dev, "failed to alloc memory!\n");
		ret = -ENOMEM;
		goto err_nomem;
	}
	platform_set_drvdata(pdev, info);
	tmu_info = info;

	info->dev = &pdev->dev;
	info->tmu_state = TMU_STATUS_INIT;

	/* set cpufreq limit level at 1st_throttle & 2nd throttle */
	pdata = info->dev->platform_data;
	pr_info("TMU: CPU 1st throttle %dKHz, 2nd throttle %dKHz, 3rd throttle %dKHz\n",
			pdata->cpufreq.limit_1st_throttle,
			pdata->cpufreq.limit_2nd_throttle,
			pdata->cpufreq.limit_3rd_throttle);
	pr_info("TMU: GPU 1st throttle %dKHz, 2nd throttle %dKHz, 3rd throttle %dKHz\n",
			pdata->gpufreq.limit_1st_throttle,
			pdata->gpufreq.limit_2nd_throttle,
			pdata->gpufreq.limit_3rd_throttle);

	info->cpufreq_level_1st_throttle = TMU_INV_FREQ_LEVEL;
	info->cpufreq_level_2nd_throttle = TMU_INV_FREQ_LEVEL;
	info->cpufreq_level_3rd_throttle = TMU_INV_FREQ_LEVEL;
	info->gpufreq_level_1st_throttle = pdata->gpufreq.limit_1st_throttle;
	info->gpufreq_level_2nd_throttle = pdata->gpufreq.limit_2nd_throttle;
	info->gpufreq_level_3rd_throttle = pdata->gpufreq.limit_3rd_throttle;

	/* To poll current temp, set sampling rate to ONE second sampling */
	info->sampling_rate  = usecs_to_jiffies(SAMPLING_RATE);
	/* 10sec monitoring */
	info->monitor_period = usecs_to_jiffies(MONITOR_PERIOD);

	/* read fusing temperature */
	read_fusing_diffvalue(info);

	/* support test mode */
	if (mask & ENABLE_TEST_MODE)
		set_temperature_params(info);
	else
		print_temperature_params(info);

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(&pdev->dev, "failed to get memory region resource\n");
		ret = -ENODEV;
		goto err_nores;
	}

	info->ioarea = request_mem_region(res->start,
			res->end-res->start + 1, pdev->name);
	if (!(info->ioarea)) {
		dev_err(&pdev->dev, "failed to reserve memory region\n");
		ret = -EBUSY;
		goto err_nores;
	}

	info->tmu_base = ioremap(res->start, (res->end - res->start) + 1);
	if (!(info->tmu_base)) {
		dev_err(&pdev->dev, "failed ioremap()\n");
		ret = -ENOMEM;
		goto err_nomap;
	}
	
	tmu_monitor_wq = create_freezable_workqueue(dev_name(&pdev->dev));
	if (!tmu_monitor_wq) {
		pr_info("Creation of tmu_monitor_wq failed\n");
		ret = -ENOMEM;
		goto err_wq;
	}

	/* To support periodic temprature monitoring */
	if (mask & ENABLE_TEMP_MON) {
		INIT_DELAYED_WORK_DEFERRABLE(&info->monitor,
					sdp_poll_cur_temp);
		queue_delayed_work_on(0, tmu_monitor_wq, &info->monitor,
			info->monitor_period);
	}
	INIT_DELAYED_WORK_DEFERRABLE(&info->polling, sdp_handler_tmu_state);

	ret = device_create_file(&pdev->dev, &dev_attr_temperature);
	if (ret != 0) {
		pr_err("Failed to create temperatue file: %d\n", ret);
		goto err_sysfs_file1;
	}

	ret = device_create_file(&pdev->dev, &dev_attr_tmu_state);
	if (ret != 0) {
		pr_err("Failed to create tmu_state file: %d\n", ret);
		goto err_sysfs_file2;
	}
	ret = device_create_file(&pdev->dev, &dev_attr_lot_id);
	if (ret != 0) {
		pr_err("Failed to create lot id file: %d\n", ret);
		goto err_sysfs_file3;
	}

	ret = device_create_file(&pdev->dev, &dev_attr_throttle_on);
	if (ret != 0) {
		pr_err("Failed to create throttle on file: %d\n", ret);
		goto err_sysfs_file4;
	}

	ret = tmu_initialize(pdev);
	if (ret)
		goto err_init;

#ifdef CONFIG_TMU_SYSFS
	ret = device_create_file(&pdev->dev, &dev_attr_curr_temp);
	if (ret < 0) {
		dev_err(&pdev->dev, "Failed to create sysfs group\n");
		goto err_init;
	}
#endif

#ifdef CONFIG_TMU_DEBUG
	ret = device_create_file(&pdev->dev, &dev_attr_print_state);
	if (ret) {
		dev_err(&pdev->dev, "Failed to create tmu sysfs group\n\n");
		return ret;
	}
#endif

	/* initialize tmu_state */
	queue_delayed_work_on(0, tmu_monitor_wq, &info->polling,
		info->sampling_rate);

	return ret;

err_init:
	device_remove_file(&pdev->dev, &dev_attr_throttle_on);
	
err_sysfs_file4:
	device_remove_file(&pdev->dev, &dev_attr_lot_id);

err_sysfs_file3:
	device_remove_file(&pdev->dev, &dev_attr_tmu_state);

err_sysfs_file2:
	device_remove_file(&pdev->dev, &dev_attr_temperature);

err_sysfs_file1:
	destroy_workqueue(tmu_monitor_wq);

err_wq:
	iounmap(info->tmu_base);

err_nomap:
	release_resource(info->ioarea);
	kfree(info->ioarea);

err_nores:
	kfree(info);
	info = NULL;

err_nomem:
	dev_err(&pdev->dev, "initialization failed.\n");

	return ret;
}

static int __devinit sdp_tmu_remove(struct platform_device *pdev)
{
	struct sdp_tmu_info *info = platform_get_drvdata(pdev);

	cancel_delayed_work(&info->polling);
	destroy_workqueue(tmu_monitor_wq);

	device_remove_file(&pdev->dev, &dev_attr_temperature);
	device_remove_file(&pdev->dev, &dev_attr_tmu_state);

	kfree(info);
	info = NULL;

	pr_info("%s is removed\n", dev_name(&pdev->dev));
	return 0;
}

#ifdef CONFIG_PM
static int sdp_tmu_suspend(struct platform_device *pdev, pm_message_t state)
{
	struct sdp_tmu_info *info = platform_get_drvdata(pdev);

	if (!info)
		return -EAGAIN;

	/* save register value */

	return 0;
}

static int sdp_tmu_resume(struct platform_device *pdev)
{
	struct sdp_tmu_info *info = platform_get_drvdata(pdev);
	//struct sdp_platform_tmu *data = info->dev->platform_data;

	if (!info)
		return -EAGAIN;

	/* restore tmu register value */

	/* Find out tmu_state after wakeup */
	queue_delayed_work_on(0, tmu_monitor_wq, &info->polling, 0);

	return 0;
}
#else
#define sdp_tmu_suspend	NULL
#define sdp_tmu_resume	NULL
#endif

static struct platform_driver sdp_tmu_driver = {
	.probe		= sdp_tmu_probe,
	.remove		= sdp_tmu_remove,
	.suspend	= sdp_tmu_suspend,
	.resume		= sdp_tmu_resume,
	.driver		= {
		.name   = "sdp-tmu",
		.owner  = THIS_MODULE,
	},
};

static int __init sdp_tmu_driver_init(void)
{
	return platform_driver_register(&sdp_tmu_driver);
}

static void __exit sdp_tmu_driver_exit(void)
{
	platform_driver_unregister(&sdp_tmu_driver);
}
late_initcall_sync(sdp_tmu_driver_init);
module_exit(sdp_tmu_driver_exit);

