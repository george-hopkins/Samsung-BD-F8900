/* linux/arch/arm/mach-ccep/cpufreq.c
 *
 * Copyright (c) 2012 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com
 *
 * SDP - CPU frequency scaling support for SDP series
 *
*/

#include <linux/sysdev.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/regulator/consumer.h>
#include <linux/cpufreq.h>
#include <linux/delay.h>
//#include <linux/suspend.h>
//#include <linux/reboot.h>

#include <plat/cpufreq.h>
#include <plat/asv.h>
#include <mach/hardware.h>
#include <mach/platform.h>

struct sdp_dvfs_info *sdp_info;

static struct regulator *arm_regulator;
static struct cpufreq_freqs freqs;

static bool sdp_cpufreq_disable;
static bool sdp_cpufreq_on;
static bool sdp_user_cpufreq_on;
static bool sdp_tmu_control;
static bool sdp_asv_on;
static bool sdp_cpufreq_lock_disable;
static bool sdp_cpufreq_init_done;
static DEFINE_MUTEX(set_freq_lock);
static DEFINE_MUTEX(set_cpu_freq_lock);
static DEFINE_MUTEX(cpufreq_on_lock);

unsigned int g_cpufreq_limit_id;
unsigned int g_cpufreq_limit_val[DVFS_LOCK_ID_END];
unsigned int g_cpufreq_limit_level;

unsigned int g_cpufreq_lock_id;
unsigned int g_cpufreq_lock_val[DVFS_LOCK_ID_END];
unsigned int g_cpufreq_lock_level;

static int sdp_verify_speed(struct cpufreq_policy *policy)
{
	return cpufreq_frequency_table_verify(policy,
					      sdp_info->freq_table);
}

static unsigned int sdp_getspeed(unsigned int cpu)
{
	return clk_get_rate(sdp_info->cpu_clk) / 1000;
}

struct cpufreq_timerdiv_table *sdp_cpufreq_get_timerdiv_table(void)
{
	return sdp_info->div_table;
}

static unsigned int sdp_get_safe_armvolt(unsigned int old_index, unsigned int new_index)
{
	unsigned int safe_arm_volt = 0;
#if 0
	struct cpufreq_frequency_table *freq_table = sdp_info->freq_table;
	unsigned int *volt_table = sdp_info->volt_table;

	/*
	 * ARM clock source will be changed APLL to MPLL temporary
	 * To support this level, need to control regulator for
	 * reguired voltage level
	 */
	if (sdp_info->need_apll_change != NULL) {
		if (sdp_info->need_apll_change(old_index, new_index) &&
			(freq_table[new_index].frequency < sdp_info->mpll_freq_khz) &&
			(freq_table[old_index].frequency < sdp_info->mpll_freq_khz)) {
				safe_arm_volt = volt_table[sdp_info->pll_safe_idx];
		}
	}
#endif

	return safe_arm_volt;
}

static int sdp_target(struct cpufreq_policy *policy,
			  unsigned int target_freq,
			  unsigned int relation)
{
	unsigned int index, old_index = UINT_MAX;
	unsigned int arm_volt, safe_arm_volt = 0;
	int ret = 0, i;
	struct cpufreq_frequency_table *freq_table = sdp_info->freq_table;
	unsigned int *volt_table = sdp_info->volt_table;

	mutex_lock(&set_freq_lock);

	if (sdp_cpufreq_disable)
		goto out;

	if (!sdp_cpufreq_on) {
		ret = -EAGAIN;
		goto out;
	}

	freqs.old = policy->cur;

	/*
	 * cpufreq_frequency_table_target() cannot be used for freqs.old
	 * because policy->min/max may have been changed. If changed, the
	 * resulting old_index may be inconsistent with freqs.old, which
	 * will lead to inconsistent voltage/frequency configurations later.
	 */
	for (i = 0; freq_table[i].frequency != (u32)CPUFREQ_TABLE_END; i++) {
		if (freq_table[i].frequency == freqs.old)
			old_index = freq_table[i].index;
	}
	if (old_index == UINT_MAX) {
		ret = -EINVAL;
		goto out;
	}

	if (cpufreq_frequency_table_target(policy, freq_table,
					   target_freq, relation, &index)) {
		ret = -EINVAL;
		goto out;
	}

	/* Need to set performance limitation */
	if (!sdp_cpufreq_lock_disable && (index > g_cpufreq_lock_level))
		index = g_cpufreq_lock_level;

	if (!sdp_cpufreq_lock_disable && (index < g_cpufreq_limit_level))
		index = g_cpufreq_limit_level;

#if 0
	/* Do NOT step up max arm clock directly to reduce power consumption */
	if (index == sdp_info->max_support_idx && old_index > 3)
		index = 3;
#endif

	freqs.new = freq_table[index].frequency;
	freqs.cpu = policy->cpu;

	safe_arm_volt = sdp_get_safe_armvolt(old_index, index);

	arm_volt = volt_table[index];

	cpufreq_notify_transition(&freqs, CPUFREQ_PRECHANGE);

	/* When the new frequency is higher than current frequency */
	if ((freqs.new > freqs.old) && !safe_arm_volt) {
		/* Firstly, voltage up to increase frequency */
		ret = regulator_set_voltage(arm_regulator, (int)arm_volt,
					     (int)arm_volt + 10000);
		if (ret < 0) {
			ret = -EIO;
			goto out;
		}

		/* set EMA */
		sdp_info->set_ema(index);
	}

	if (safe_arm_volt)
		regulator_set_voltage(arm_regulator, (int)safe_arm_volt,
				     (int)safe_arm_volt + 10000);

	if (freqs.new != freqs.old)
		sdp_info->set_freq(old_index, index);

	cpufreq_notify_transition(&freqs, CPUFREQ_POSTCHANGE);

	/* When the new frequency is lower than current frequency */
	if ((freqs.new < freqs.old) ||
	   ((freqs.new > freqs.old) && safe_arm_volt)) {
		/* set EMA */
		sdp_info->set_ema(index);

		udelay(50);
	   
		/* down the voltage after frequency change */
		regulator_set_voltage(arm_regulator, (int)arm_volt,
				     (int)arm_volt + 10000);
	}

out:
	mutex_unlock(&set_freq_lock);

	return ret;
}

/**
 * sdp_find_cpufreq_level_by_volt - find cpufreq_level by requested
 * arm voltage.
 *
 * This function finds the cpufreq_level to set for voltage above req_volt
 * and return its value.
 */
int sdp_find_cpufreq_level_by_volt(unsigned int arm_volt,
					unsigned int *level)
{
	struct cpufreq_frequency_table *table;
	unsigned int *volt_table = sdp_info->volt_table;
	int i;

	if (!sdp_cpufreq_init_done)
		return -EINVAL;

	table = cpufreq_frequency_get_table(0);
	if (!table) {
		pr_err("%s: Failed to get the cpufreq table\n", __func__);
		return -EINVAL;
	}

	/* check if arm_volt has value or not */
	if (!arm_volt) {
		pr_err("%s: req_volt has no value.\n", __func__);
		return -EINVAL;
	}

	/* find cpufreq level in volt_table */
	for (i = (int)sdp_info->min_support_idx;
			i >= (int)sdp_info->max_support_idx; i--) {
		if (volt_table[i] >= arm_volt) {
			*level = (unsigned int)i;
			return 0;
		}
	}

	pr_err("%s: Failed to get level for %u uV\n", __func__, arm_volt);

	return -EINVAL;
}
EXPORT_SYMBOL_GPL(sdp_find_cpufreq_level_by_volt);

int sdp_cpufreq_get_level(unsigned int freq, unsigned int *level)
{
	struct cpufreq_frequency_table *table;
	unsigned int i;

	if (!sdp_cpufreq_init_done)
		return -EINVAL;

	table = cpufreq_frequency_get_table(0);
	if (!table) {
		pr_err("%s: Failed to get the cpufreq table\n", __func__);
		return -EINVAL;
	}

	for (i = sdp_info->max_support_idx;
		(table[i].frequency != (u32)CPUFREQ_TABLE_END); i++) {
		if (table[i].frequency == freq) {
			*level = i;
			return 0;
		}
	}

	pr_err("%s: %u KHz is an unsupported cpufreq\n", __func__, freq);

	return -EINVAL;
}
EXPORT_SYMBOL_GPL(sdp_cpufreq_get_level);

atomic_t sdp_cpufreq_lock_count;

int sdp_cpufreq_lock(unsigned int nId,
			 enum cpufreq_level_index cpufreq_level)
{
	int ret = 0, i, old_idx = -EINVAL;
	unsigned int freq_old, freq_new, arm_volt, safe_arm_volt;
	unsigned int *volt_table;
	struct cpufreq_policy *policy;
	struct cpufreq_frequency_table *freq_table;

	if (!sdp_cpufreq_init_done)
		return -EPERM;

	if (!sdp_info)
		return -EPERM;

	if (sdp_cpufreq_disable && (nId != DVFS_LOCK_ID_TMU)) {
		pr_info("CPUFreq is already fixed\n");
		return -EPERM;
	}

	if (cpufreq_level < sdp_info->max_support_idx
			|| cpufreq_level > sdp_info->min_support_idx) {
		pr_warn("%s: invalid cpufreq_level(%d:%d)\n", __func__, nId,
				cpufreq_level);
		return -EINVAL;
	}

	policy = cpufreq_cpu_get(0);
	if (!policy)
		return -EPERM;

	volt_table = sdp_info->volt_table;
	freq_table = sdp_info->freq_table;

	mutex_lock(&set_cpu_freq_lock);
	if (g_cpufreq_lock_id & (1U << nId)) {
		mutex_unlock(&set_cpu_freq_lock);
		return 0;
	}

	g_cpufreq_lock_id |= (1U << nId);
	g_cpufreq_lock_val[nId] = cpufreq_level;

	/* If the requested cpufreq is higher than current min frequency */
	if (cpufreq_level < g_cpufreq_lock_level)
		g_cpufreq_lock_level = cpufreq_level;

	mutex_unlock(&set_cpu_freq_lock);

	if ((g_cpufreq_lock_level < g_cpufreq_limit_level)
				&& (nId != DVFS_LOCK_ID_PM))
		return 0;

	/* Do not setting cpufreq lock frequency
	 * because current governor doesn't support dvfs level lock
	 * except DVFS_LOCK_ID_PM */
	if (sdp_cpufreq_lock_disable && (nId != DVFS_LOCK_ID_PM))
		return 0;

	/* If current frequency is lower than requested freq,
	 * it needs to update
	 */
	mutex_lock(&set_freq_lock);
	freq_old = policy->cur;
	freq_new = freq_table[cpufreq_level].frequency;
	if (freq_old < freq_new) {
		/* Find out current level index */
		for (i = 0; freq_table[i].frequency != (u32)CPUFREQ_TABLE_END; i++) {
			if (freq_old == freq_table[i].frequency) {
				old_idx = (int)freq_table[i].index;
				break;
			}
		}
		if (old_idx == -EINVAL) {
			printk(KERN_ERR "%s: Level not found\n", __func__);
			mutex_unlock(&set_freq_lock);
			return -EINVAL;
		}

		freqs.old = freq_old;
		freqs.new = freq_new;
		cpufreq_notify_transition(&freqs, CPUFREQ_PRECHANGE);

		/* get the voltage value */
		safe_arm_volt = sdp_get_safe_armvolt((u32)old_idx, (u32)cpufreq_level);
		if (safe_arm_volt)
			regulator_set_voltage(arm_regulator, (int)safe_arm_volt,
					     (int)safe_arm_volt + 10000);

		arm_volt = volt_table[cpufreq_level];
		ret = regulator_set_voltage(arm_regulator, (int)arm_volt,
				     (int)arm_volt + 10000);
		if (ret < 0) {
			ret = -EIO;
			goto out;
		}

		sdp_info->set_ema(cpufreq_level);

		sdp_info->set_freq((u32)old_idx, (u32)cpufreq_level);

		cpufreq_notify_transition(&freqs, CPUFREQ_POSTCHANGE);
	}

out:
	mutex_unlock(&set_freq_lock);

	return ret;
}
EXPORT_SYMBOL_GPL(sdp_cpufreq_lock);

void sdp_cpufreq_lock_free(unsigned int nId)
{
	unsigned int i;

	if (!sdp_cpufreq_init_done)
		return;

	mutex_lock(&set_cpu_freq_lock);
	g_cpufreq_lock_id &= ~(1U << nId);
	g_cpufreq_lock_val[nId] = sdp_info->min_support_idx;
	g_cpufreq_lock_level = sdp_info->min_support_idx;
	if (g_cpufreq_lock_id) {
		for (i = 0; i < DVFS_LOCK_ID_END; i++) {
			if (g_cpufreq_lock_val[i] < g_cpufreq_lock_level)
				g_cpufreq_lock_level = g_cpufreq_lock_val[i];
		}
	}
	mutex_unlock(&set_cpu_freq_lock);
}
EXPORT_SYMBOL_GPL(sdp_cpufreq_lock_free);

int sdp_cpufreq_upper_limit(unsigned int nId,
				enum cpufreq_level_index cpufreq_level)
{
	int ret = 0, old_idx = 0, i;
	unsigned int freq_old, freq_new, arm_volt, safe_arm_volt;
	unsigned int *volt_table;
	struct cpufreq_policy *policy;
	struct cpufreq_frequency_table *freq_table;

	if (!sdp_cpufreq_init_done)
		return -EPERM;

	if (!sdp_info)
		return -EPERM;

	if (sdp_cpufreq_disable) {
		pr_info("CPUFreq is already fixed\n");
		return -EPERM;
	}

	if (cpufreq_level < sdp_info->max_support_idx
			|| cpufreq_level > sdp_info->min_support_idx) {
		pr_warn("%s: invalid cpufreq_level(%d:%d)\n", __func__, nId,
				cpufreq_level);
		return -EINVAL;
	}

	policy = cpufreq_cpu_get(0);
	if (!policy)
		return -EPERM;

	volt_table = sdp_info->volt_table;
	freq_table = sdp_info->freq_table;

	mutex_lock(&set_cpu_freq_lock);
	if (g_cpufreq_limit_id & (1U << nId)) {
		pr_err("[CPUFREQ]This device [%d] already limited cpufreq\n", nId);
		mutex_unlock(&set_cpu_freq_lock);
		return 0;
	}

	g_cpufreq_limit_id |= (1U << nId);
	g_cpufreq_limit_val[nId] = cpufreq_level;

	/* If the requested limit level is lower than current value */
	if (cpufreq_level > g_cpufreq_limit_level)
		g_cpufreq_limit_level = cpufreq_level;

	mutex_unlock(&set_cpu_freq_lock);

	mutex_lock(&set_freq_lock);
	/* If cur frequency is higher than limit freq, it needs to update */
	freq_old = policy->cur;
	freq_new = freq_table[cpufreq_level].frequency;
	if (freq_old > freq_new) {
		/* Find out current level index */
		for (i = 0; i <= (int)sdp_info->min_support_idx; i++) {
			if (freq_old == freq_table[i].frequency) {
				old_idx = (int)freq_table[i].index;
				break;
			} else if (i == (int)sdp_info->min_support_idx) {
				printk(KERN_ERR "%s: Level is not found\n", __func__);
				mutex_unlock(&set_freq_lock);

				return -EINVAL;
			} else {
				continue;
			}
		}
		freqs.old = freq_old;
		freqs.new = freq_new;

		cpufreq_notify_transition(&freqs, CPUFREQ_PRECHANGE);

		sdp_info->set_freq((u32)old_idx, (u32)cpufreq_level);

		safe_arm_volt = sdp_get_safe_armvolt((u32)old_idx, (u32)cpufreq_level);
		if (safe_arm_volt)
			regulator_set_voltage(arm_regulator, (int)safe_arm_volt,
					     (int)safe_arm_volt + 10000);

		/* set EMA */
		sdp_info->set_ema(cpufreq_level);

		arm_volt = volt_table[cpufreq_level];
		regulator_set_voltage(arm_regulator, (int)arm_volt, (int)arm_volt + 10000);

		cpufreq_notify_transition(&freqs, CPUFREQ_POSTCHANGE);
	}

	mutex_unlock(&set_freq_lock);

	return ret;
}

void sdp_cpufreq_emergency_limit(void)
{
	int i, old_idx, em_idx;
	unsigned int freq_old;
	unsigned int arm_volt;
	struct cpufreq_policy *policy;
	unsigned int *volt_table = sdp_info->volt_table;
	struct cpufreq_frequency_table *freq_table = sdp_info->freq_table;

	printk(KERN_INFO "%s: called\n", __func__);

	policy = cpufreq_cpu_get(0);
	if (!policy) {
		printk(KERN_ERR "%s: policy is NULL\n", __func__);
		return;
	}

	/* old freq index */
	freq_old = policy->cur;
	old_idx = (int)sdp_info->min_support_idx; /* initial idx is min idx */
	for (i = 0; i <= (int)sdp_info->min_support_idx; i++) {
		if (freq_old == freq_table[i].frequency) {
			old_idx = (int)freq_table[i].index;
			printk(KERN_INFO "%s: freq_old = %d, old_idx = %d\n", __func__, freq_old, old_idx);
			break;
		} else {
			continue;
		}
	}

	em_idx = sdp_info->get_emergency_freq_index();  /* emergency frequency index */
	arm_volt = volt_table[em_idx];                  /* emergency voltage */

	mutex_lock(&set_cpu_freq_lock);

	g_cpufreq_limit_id |= (1U << DVFS_LOCK_ID_TMU);
	g_cpufreq_limit_val[DVFS_LOCK_ID_TMU] = (u32)em_idx;
	g_cpufreq_limit_level = (u32)em_idx;

	mutex_unlock(&set_cpu_freq_lock);

	mutex_lock(&set_freq_lock);

	cpufreq_notify_transition(&freqs, CPUFREQ_PRECHANGE);

	/* set frequency */
	sdp_info->set_freq((u32)old_idx, (u32)em_idx);

	cpufreq_notify_transition(&freqs, CPUFREQ_POSTCHANGE);

	/* The emergency frequency is always lower than current frequency */
	/* set EMA */
	sdp_info->set_ema((u32)em_idx);
	  
	/* down the voltage after frequency change */
	regulator_set_voltage(arm_regulator, (int)arm_volt,
	                (int)arm_volt + 10000);

	mutex_unlock(&set_freq_lock);
}

void sdp_cpufreq_emergency_limit_free(void)
{
	int em_idx, ret;
	unsigned int arm_volt;
	unsigned int *volt_table = sdp_info->volt_table;

	printk(KERN_INFO "%s: called\n", __func__);

	mutex_lock(&set_cpu_freq_lock);

	/* old emergency freq index */
	em_idx = sdp_info->get_emergency_freq_index();

	/* new stored min freq index and voltage */
	g_cpufreq_limit_id &= ~(1U << DVFS_LOCK_ID_TMU);
	g_cpufreq_limit_val[DVFS_LOCK_ID_TMU] = sdp_info->min_support_idx;

	g_cpufreq_limit_level = sdp_info->min_support_idx;
	arm_volt = volt_table[g_cpufreq_limit_level];

	mutex_unlock(&set_cpu_freq_lock);

	mutex_lock(&set_freq_lock);

	cpufreq_notify_transition(&freqs, CPUFREQ_PRECHANGE);

	/* The new frequency is always higher than emergency frequency */
	/* Firstly, voltage up to increase frequency */
	ret = regulator_set_voltage(arm_regulator, (int)arm_volt,
							(int)arm_volt + 10000);
	if (ret < 0)
		goto out;

	/* set EMA */
	sdp_info->set_ema(g_cpufreq_limit_level);

	/* set frequency */
	sdp_info->set_freq((u32)em_idx, g_cpufreq_limit_level);

	cpufreq_notify_transition(&freqs, CPUFREQ_POSTCHANGE);

	out:
	mutex_unlock(&set_freq_lock);
}

void sdp_cpufreq_upper_limit_free(unsigned int nId)
{
	unsigned int i;
	struct cpufreq_frequency_table *freq_table = sdp_info->freq_table;
	unsigned int *volt_table = sdp_info->volt_table;
	struct cpufreq_policy *policy;
	unsigned int old_index = UINT_MAX;
	unsigned int arm_volt;
	int ret = 0;

	if (!sdp_cpufreq_init_done)
		return;

	mutex_lock(&set_cpu_freq_lock);
	
	g_cpufreq_limit_id &= ~(1U << nId);
	g_cpufreq_limit_val[nId] = sdp_info->max_support_idx;
	g_cpufreq_limit_level = sdp_info->max_support_idx;

	/* find lowest frequency */
	if (g_cpufreq_limit_id) {
		for (i = 0; i < DVFS_LOCK_ID_END; i++) {
			if (g_cpufreq_limit_val[i] > g_cpufreq_limit_level)
				g_cpufreq_limit_level = g_cpufreq_limit_val[i];
		}
	}

	mutex_unlock(&set_cpu_freq_lock);

	mutex_lock(&set_freq_lock);

	/* set CPU frequency to lowest one */
	policy = cpufreq_cpu_get(0);
	freqs.old = policy->cur;
	freqs.new = freq_table[g_cpufreq_limit_level].frequency;
	freqs.cpu = policy->cpu;

	arm_volt = volt_table[g_cpufreq_limit_level];

	cpufreq_notify_transition(&freqs, CPUFREQ_PRECHANGE);

	/* When the new frequency is higher than current frequency */
	if (freqs.new > freqs.old) {
		/* Firstly, voltage up to increase frequency */
		ret = regulator_set_voltage(arm_regulator, (int)arm_volt,
					     (int)arm_volt + 10000);
		if (ret < 0)
			goto out;

		/* set EMA */
		sdp_info->set_ema(g_cpufreq_limit_level);
	}

	if (freqs.new != freqs.old)
		sdp_info->set_freq(old_index, g_cpufreq_limit_level);

	cpufreq_notify_transition(&freqs, CPUFREQ_POSTCHANGE);

	/* When the new frequency is lower than current frequency */
	if (freqs.new < freqs.old) {
		/* set EMA */
		sdp_info->set_ema(g_cpufreq_limit_level);
	   
		/* down the voltage after frequency change */
		regulator_set_voltage(arm_regulator, (int)arm_volt,
				     (int)arm_volt + 10000);
	}
out:	
	mutex_unlock(&set_freq_lock);
}

/* This API serve highest priority level locking */
int sdp_cpufreq_level_fix(unsigned int freq)
{
	struct cpufreq_policy *policy;
	int ret = 0;

	if (!sdp_cpufreq_init_done)
		return -EPERM;

	policy = cpufreq_cpu_get(0);
	if (!policy)
		return -EPERM;

	if (sdp_cpufreq_disable) {
		pr_info("CPUFreq is already fixed\n");
		return -EPERM;
	}
	ret = sdp_target(policy, freq, CPUFREQ_RELATION_L);

	sdp_cpufreq_disable = true;
	return ret;

}
EXPORT_SYMBOL_GPL(sdp_cpufreq_level_fix);

void sdp_cpufreq_level_unfix(void)
{
	if (!sdp_cpufreq_init_done)
		return;

	sdp_cpufreq_disable = false;
}
EXPORT_SYMBOL_GPL(sdp_cpufreq_level_unfix);

int sdp_cpufreq_is_fixed(void)
{
	return sdp_cpufreq_disable;
}
EXPORT_SYMBOL_GPL(sdp_cpufreq_is_fixed);

bool sdp_cpufreq_is_cpufreq_on(void)
{
	return sdp_user_cpufreq_on;
}

void sdp_cpufreq_set_tmu_control(bool on)
{
	mutex_lock(&cpufreq_on_lock);

	sdp_tmu_control = on;

	mutex_unlock(&cpufreq_on_lock);
}

void sdp_cpufreq_set_cpufreq_on(bool on)
{
	struct cpufreq_policy *policy;
	int i, timeout = 100;
	
	printk(KERN_DEBUG "cpufreq on = %u\n", on);

	mutex_lock(&cpufreq_on_lock);	

	if (on == true) { /* ON */
		if (sdp_cpufreq_on) {
			printk(KERN_INFO "%s - cpufreq already ON\n", __FUNCTION__);
			goto out;
		}

		sdp_cpufreq_on = true;
		printk(KERN_INFO "DVFS ON\n");
	} else { /* OFF */
		if (!sdp_cpufreq_on) {
			printk(KERN_INFO "%s - cpufreq already OFF\n", __FUNCTION__);
			goto out;
		}
		
		policy = cpufreq_cpu_get(0);
		
		/* frequency to max */
		for (i = 0; i < timeout; i++) {
			if (!sdp_target(policy, policy->max, CPUFREQ_RELATION_H))
				break;
			printk(KERN_WARNING "retry frequnecy setting.\n");
			msleep(10);
		}
		if (i == timeout)
			printk(KERN_WARNING "%s - frequnecy set time out!!\n", __FUNCTION__);
		
		sdp_cpufreq_on = false;
		printk(KERN_INFO "DVFS OFF\n");
	}

out:
	mutex_unlock(&cpufreq_on_lock);
	return;
}

void sdp_cpufreq_set_asv_on(bool on)
{
	struct cpufreq_policy *policy;
	int i, timeout = 100, ret;
	int arm_volt, index;
	struct cpufreq_frequency_table *freq_table = sdp_info->freq_table;
	unsigned int *volt_table = sdp_info->volt_table;
	
	printk(KERN_DEBUG "avs on = %d\n", on);

	policy = cpufreq_cpu_get(0);

	/* check input error */
	if (on == true) { /* ON */
		if (sdp_asv_on) {
			printk(KERN_INFO "AVS already ON\n");
			goto out;
		}
	} else { /* OFF */
		if (!sdp_asv_on) {
			printk(KERN_INFO "AVS already OFF\n");
			goto out;
		}
	}
			
	mutex_lock(&set_freq_lock);

	if (on == true) { /* ON */
		/* restore ASV index */
		printk(KERN_INFO "restore ASV grp%d\n", sdp_asv_stored_result);
		sdp_result_of_asv = sdp_asv_stored_result;
	} else { /* OFF */
		/* select ASV1(typical voltage table) */
		printk(KERN_INFO "select ASV0(typical fix)\n");
		sdp_result_of_asv = 0;
	}

	/* change voltage table */
	if (sdp_info->update_volt_table)
		sdp_info->update_volt_table();
	else
		printk(KERN_ERR "ERR: cpufreq update_volt_table is NULL\n");

	/* change voltage */
	/* get current freq index */
	if (cpufreq_frequency_table_target(policy, freq_table,
				   policy->cur, CPUFREQ_RELATION_L, &index)) {
		printk(KERN_ERR "AVS : get cpufreq table index err.\n");
		mutex_unlock(&set_freq_lock);
		goto out;
	}
	printk(KERN_INFO "cur freq=%d, idx=%d, volt=%duV", policy->cur, index, volt_table[index]);
	
	arm_volt = (int)volt_table[index];
	for (i = 0; i < timeout; i++) {
		ret = regulator_set_voltage(arm_regulator, arm_volt,
				     arm_volt + 10000);
		if (ret == 0)
			break;
		printk(KERN_WARNING "retry volt set");
		msleep(10);
	}
	if (i == timeout)
		printk(KERN_WARNING "voltage change fail.\n");

	/* store result */
	if (on == 1) { /* ON*/
		sdp_asv_on = true;
		printk(KERN_INFO "AVS ON\n");
	} else { /* OFF */
		sdp_asv_on = false;
		printk(KERN_INFO "AVS OFF\n");
	}

	mutex_unlock(&set_freq_lock);

out:
	return;
}


#ifdef CONFIG_PM
static int sdp_cpufreq_suspend(struct cpufreq_policy *policy)
{
	return 0;
}

static int sdp_cpufreq_resume(struct cpufreq_policy *policy)
{
	return 0;
}
#endif

#if 0 /* for PM notifier */
static void sdp_save_gov_freq(void)
{
	unsigned int cpu = 0;

	sdp_info->gov_support_freq = sdp_getspeed(cpu);
	pr_debug("cur_freq[%d] saved to freq[%d]\n", sdp_getspeed(0),
			sdp_info->gov_support_freq);
}

static void sdp_restore_gov_freq(struct cpufreq_policy *policy)
{
	unsigned int cpu = 0;

	if (sdp_getspeed(cpu) != sdp_info->gov_support_freq)
		sdp_target(policy, sdp_info->gov_support_freq,
				CPUFREQ_RELATION_H);

	pr_debug("freq[%d] restored to cur_freq[%d]\n",
			sdp_info->gov_support_freq, sdp_getspeed(cpu));
}

static int sdp_cpufreq_notifier_event(struct notifier_block *this,
		unsigned long event, void *ptr)
{
	int ret = 0;
	unsigned int cpu = 0;
	struct cpufreq_policy *policy = cpufreq_cpu_get(cpu);

	switch (event) {
	case PM_SUSPEND_PREPARE:
	case PM_HIBERNATION_PREPARE:
	case PM_RESTORE_PREPARE:
		/* If current governor is userspace or performance or powersave,
		 * save the current cpufreq before sleep.
		 */
		if (sdp_cpufreq_lock_disable)
			sdp_save_gov_freq();

		ret = sdp_cpufreq_lock(DVFS_LOCK_ID_PM,
					   sdp_info->pm_lock_idx);
		if (ret < 0)
			return NOTIFY_BAD;
#if 0
		ret = sdp_cpufreq_upper_limit(DVFS_LOCK_ID_PM,
						sdp_info->pm_lock_idx);
		if (ret < 0)
			return NOTIFY_BAD;
#endif
		sdp_cpufreq_disable = true;

#if 0 //def CONFIG_SLP
		/*
		 * Safe Voltage for Suspend/Wakeup: Falling back to the
		 * default value of bootloaders.
		 * Note that at suspended state, this 'high' voltage does
		 * not incur higher power consumption because it is OFF.
		 * This is for the stability during suspend/wakeup process.
		 */
		regulator_set_voltage(arm_regulator, 120000, 120000 + 10000);
#endif

		pr_debug("PM_SUSPEND_PREPARE for CPUFREQ\n");
		return NOTIFY_OK;
	case PM_POST_RESTORE:
	case PM_POST_HIBERNATION:
	case PM_POST_SUSPEND:
		pr_debug("PM_POST_SUSPEND for CPUFREQ: %d\n", ret);
		sdp_cpufreq_lock_free(DVFS_LOCK_ID_PM);
#if 0
		sdp_cpufreq_upper_limit_free(DVFS_LOCK_ID_PM);
#endif
		sdp_cpufreq_disable = false;
		/* If current governor is userspace or performance or powersave,
		 * restore the saved cpufreq after waekup.
		 */
		if (sdp_cpufreq_lock_disable)
			sdp_restore_gov_freq(policy);

		return NOTIFY_OK;
	}
	return NOTIFY_DONE;
}

static struct notifier_block sdp_cpufreq_notifier = {
	.notifier_call = sdp_cpufreq_notifier_event,
};
#endif

static int sdp_cpufreq_policy_notifier_call(struct notifier_block *this,
				unsigned long code, void *data)
{
	struct cpufreq_policy *policy = data;

	switch (code) {
	case CPUFREQ_ADJUST:
		if ((!strnicmp(policy->governor->name, "powersave", CPUFREQ_NAME_LEN))
		|| (!strnicmp(policy->governor->name, "performance", CPUFREQ_NAME_LEN))
		|| (!strnicmp(policy->governor->name, "userspace", CPUFREQ_NAME_LEN))) {
			printk(KERN_DEBUG "cpufreq governor is changed to %s\n",
							policy->governor->name);
			sdp_cpufreq_lock_disable = true;
		} else
			sdp_cpufreq_lock_disable = false;

	case CPUFREQ_INCOMPATIBLE:
	case CPUFREQ_NOTIFY:
	default:
		break;
	}

	return NOTIFY_DONE;
}

static struct notifier_block sdp_cpufreq_policy_notifier = {
	.notifier_call = sdp_cpufreq_policy_notifier_call,
};


static int sdp_cpufreq_cpu_init(struct cpufreq_policy *policy)
{
	policy->cur = policy->min = policy->max = sdp_getspeed(policy->cpu);

	cpufreq_frequency_table_get_attr(sdp_info->freq_table, policy->cpu);

	/* set the transition latency value */
	policy->cpuinfo.transition_latency = 100000;

	/*
	 * SDP multi-core processors has 2 or 4 cores
	 * that the frequency cannot be set independently.
	 * Each cpu is bound to the same speed.
	 * So the affected cpu is all of the cpus.
	 */
	if (num_online_cpus() == 1) {
		cpumask_copy(policy->related_cpus, cpu_possible_mask);
		cpumask_copy(policy->cpus, cpu_online_mask);
	} else {
		cpumask_setall(policy->cpus);
	}

	return cpufreq_frequency_table_cpuinfo(policy, sdp_info->freq_table);
}

#if 0 /* for reboot notifier */
static int sdp_cpufreq_reboot_notifier_call(struct notifier_block *this,
				   unsigned long code, void *_cmd)
{
	int ret = 0;

	ret = sdp_cpufreq_lock(DVFS_LOCK_ID_PM, sdp_info->pm_lock_idx);
	if (ret < 0)
		return NOTIFY_BAD;

	printk(KERN_INFO "REBOOT Notifier for CPUFREQ\n");
	return NOTIFY_DONE;
}

static struct notifier_block sdp_cpufreq_reboot_notifier = {
	.notifier_call = sdp_cpufreq_reboot_notifier_call,
};
#endif

/*******************/
/* SYSFS interface */
// cpufreq fix
static ssize_t sdp_cpufreq_freqfix_show(struct kobject *kobj,
								struct attribute *attr, char *buf)
{
	struct cpufreq_policy *policy;
	ssize_t ret;

	policy = cpufreq_cpu_get(0);
	if (!policy)
		return -EPERM;

	if (sdp_cpufreq_is_fixed())
		ret = sprintf(buf, "%d\n", policy->cur);
	else
		ret = sprintf(buf, "0\n");

	return ret;
}

static ssize_t sdp_cpufreq_freqfix_store(struct kobject *a, struct attribute *b,
			 							const char *buf, size_t count)
{
	unsigned int freq;
	int ret;

	if (!sdp_cpufreq_on) {
		printk(KERN_ERR "%s : cpufreq_on must be turned on.\n", __func__);
		return -EPERM;
	}

	ret = sscanf(buf, "%u", &freq);
	if (ret != 1) {
		printk(KERN_ERR "%s invalid arg\n", __func__);
		return -EINVAL;
	}

	/* must unfix before frequency fix */
	sdp_cpufreq_level_unfix();

	if (freq > 0) {
		printk(KERN_DEBUG "freq=%u, cpufreq_fix\n", freq);
		sdp_cpufreq_level_fix(freq);
	} else {
		printk(KERN_DEBUG "freq=%u, cpufreq unfix\n", freq);
	}
	
	return (ssize_t)count;
}
static struct global_attr frequency = __ATTR(frequency, 0644, sdp_cpufreq_freqfix_show, sdp_cpufreq_freqfix_store);

// cpufreq on/off
static ssize_t sdp_cpufreq_on_show(struct kobject *kobj,
								struct attribute *attr, char *buf)
{
	ssize_t ret;

	ret = sprintf(buf, "%d\n", sdp_cpufreq_on);

	return ret;
}

static ssize_t sdp_cpufreq_on_store(struct kobject *a, struct attribute *b,
			 							const char *buf, size_t count)
{
	unsigned int on;
	int ret;
	struct cpufreq_policy *policy;
	int i, timeout = 100;
	
	ret = sscanf(buf, "%u", &on);
	if (ret != 1) {
		printk(KERN_ERR "%s invalid arg\n", __func__);
		return -EINVAL;
	}

	printk(KERN_DEBUG "cpufreq on = %u\n", on);

	mutex_lock(&cpufreq_on_lock);

	if (on == 1) { /* ON */
		if (sdp_cpufreq_on) {
			printk(KERN_INFO "cpufreq already ON\n");
			goto out;
		}

		/* store user setting */
		sdp_user_cpufreq_on = true;
		
		/* tmu zero throttle check */
		/* user can turn on when tmu control is false */
		if (sdp_tmu_control == false)
			sdp_cpufreq_on = true;
		else
			printk(KERN_INFO "cpufreq: now TMU controls dvfs on/off.\n"
					"dvfs ON will be automatically applied when temperature is over 40'C.\n");
		
	} else if (on == 0) { /* OFF */
		if (sdp_tmu_control) {
			printk(KERN_INFO "cpufreq: now TMU controls dvfs on/off.\n"
					"dvfs OFF is already applied.\n");
			sdp_user_cpufreq_on = false;
		}
				
		if (!sdp_cpufreq_on) {
			printk(KERN_INFO "cpufreq already OFF\n");
			goto out;
		}

		policy = cpufreq_cpu_get(0);
		
		/* frequency to max */
		for (i = 0; i < timeout; i++) {
			if (!sdp_target(policy, policy->max, CPUFREQ_RELATION_H))
				break;
			printk(KERN_WARNING "retry frequnecy setting.\n");
			msleep(10);
		}
		if (i == timeout)
			printk(KERN_WARNING "frequnecy set time out!!\n");
		
		sdp_cpufreq_on = false;
		sdp_user_cpufreq_on = false;
	} else {
		printk(KERN_ERR "%s: ERROR - input 0 or 1\n", __func__);
	}

out:
	mutex_unlock(&cpufreq_on_lock);
	return (ssize_t)count;
}
static struct global_attr cpufreq_on = __ATTR(cpufreq_on, 0644, sdp_cpufreq_on_show, sdp_cpufreq_on_store);

// AVS on/off
static ssize_t sdp_asv_on_show(struct kobject *kobj,
								struct attribute *attr, char *buf)
{
	ssize_t ret;

	ret = sprintf(buf, "%d\n", sdp_asv_on);

	return ret;
}

static ssize_t sdp_asv_on_store(struct kobject *a, struct attribute *b,
			 							const char *buf, size_t count)
{
#if 1
	printk(KERN_WARNING "AVS control is not permitted!\n");

	return (ssize_t)count;
#else
	unsigned int on;
	unsigned int ret;
	struct cpufreq_policy *policy;
	int i, timeout = 100;
	int arm_volt, index;
	struct cpufreq_frequency_table *freq_table = sdp_info->freq_table;
	unsigned int *volt_table = sdp_info->volt_table;
	
	ret = sscanf(buf, "%u", &on);
	if (ret != 1) {
		printk(KERN_ERR "%s invalid arg\n", __func__);
		return -EINVAL;
	}

	printk(KERN_DEBUG "avs on = %u\n", on);

	policy = cpufreq_cpu_get(0);

	/* check input error */
	if (on == 1) { /* ON */
		if (sdp_asv_on) {
			printk(KERN_INFO "AVS already ON\n");
			goto out;
		}
	} else if (on == 0) { /* OFF */
		if (!sdp_asv_on) {
			printk(KERN_INFO "AVS already OFF\n");
			goto out;
		}
	} else {
		printk(KERN_ERR "%s: ERROR - input 0 or 1\n", __func__);
		goto out;
	}
			
	mutex_lock(&set_freq_lock);

	if (on == 1) { /* ON */
		/* restore ASV index */
		printk(KERN_INFO "restore ASV grp%d\n", sdp_asv_stored_result);
		sdp_result_of_asv = sdp_asv_stored_result;
	} else { /* OFF */
		/* select ASV1(typical voltage table) */
		printk(KERN_INFO "select ASV1(typical)\n");
		sdp_result_of_asv = 1;
	}

	/* change voltage table */
	if (sdp_info->update_volt_table)
		sdp_info->update_volt_table();
	else
		printk(KERN_ERR "ERR: cpufreq update_volt_table is NULL\n");

	/* change voltage */
	/* get current freq index */
	if (cpufreq_frequency_table_target(policy, freq_table,
				   policy->cur, CPUFREQ_RELATION_L, &index)) {
		printk(KERN_ERR "AVS : get cpufreq table index err.\n");
		mutex_unlock(&set_freq_lock);
		goto out;
	}
	printk(KERN_INFO "cur freq=%d, idx=%d, volt=%duV", policy->cur, index, volt_table[index]);
	
	arm_volt = volt_table[index];
	for (i = 0; i < timeout; i++) {
		ret = regulator_set_voltage(arm_regulator, arm_volt,
				     arm_volt + 10000);
		if (ret == 0)
			break;
		printk(KERN_WARNING "retry volt set");
		msleep(10);
	}
	if (i == timeout)
		printk(KERN_WARNING "voltage change fail.\n");

	/* store result */
	if (on == 1) { /* ON*/
		sdp_asv_on = true;
	} else { /* OFF */
		sdp_asv_on = false;
	}

	mutex_unlock(&set_freq_lock);

out:
	return count;
#endif
}
static struct global_attr avs_on = __ATTR(avs_on, 0644, sdp_asv_on_show, sdp_asv_on_store);

// show AVS volt table
static ssize_t sdp_asv_volt_show(struct kobject *kobj,
								struct attribute *attr, char *buf)
{
	ssize_t ret = 0;
	int i;
	unsigned int * volt_table = sdp_info->volt_table;

	for (i = (int)sdp_info->max_support_idx; i <= (int)sdp_info->min_support_idx; i++)
		printk(KERN_INFO "[%d] %duV\n", i, volt_table[i]);		

	return ret;
}
static struct global_attr avs_table = __ATTR(avs_table, 0444, sdp_asv_volt_show, NULL);

// frequency limitation
static ssize_t sdp_cpufreq_freqlimit_show(struct kobject *kobj,
								struct attribute *attr, char *buf)
{
	ssize_t ret;
	unsigned int freq;

	if (g_cpufreq_limit_id & (1<<DVFS_LOCK_ID_USER)) {
		freq = sdp_info->freq_table[g_cpufreq_limit_val[DVFS_LOCK_ID_USER]].frequency;
		ret = sprintf(buf, "%d\n", freq);
	} else {
		ret = sprintf(buf, "0\n");
	}

	return ret;
}

static ssize_t sdp_cpufreq_freqlimit_store(struct kobject *a, struct attribute *b,
			 							const char *buf, size_t count)
{
	unsigned int freq;
	int ret;
	unsigned int level;

	ret = sscanf(buf, "%u", &freq);
	if (ret != 1) {
		printk(KERN_ERR "%s invalid arg\n", __func__);
		return -EINVAL;
	}

	if (freq != 0)
		sdp_cpufreq_get_level(freq, &level);
	//printk("freq=%u, level=%u\n", freq, level);

	if (g_cpufreq_limit_id & (1<<DVFS_LOCK_ID_USER)) {
		printk(KERN_DEBUG "freq=%u, freq unlimit\n", freq);
		sdp_cpufreq_upper_limit_free(DVFS_LOCK_ID_USER);
	}

	if (freq > 0) {
		printk(KERN_DEBUG "freq=%u, freq limit\n", freq);
		sdp_cpufreq_upper_limit(DVFS_LOCK_ID_USER, level);
	} else {
		printk(KERN_DEBUG "freq=%u, freq unlimit\n", freq);
		sdp_cpufreq_upper_limit_free(DVFS_LOCK_ID_USER);
	}
	
	return (ssize_t)count;
}
static struct global_attr freq_limit = __ATTR(freq_limit, 0644, sdp_cpufreq_freqlimit_show, sdp_cpufreq_freqlimit_store);


static struct attribute *dbs_attributes[] = {
	&frequency.attr,
	NULL
};

static struct attribute_group dbs_attr_group = {
	.attrs = dbs_attributes,
	.name = "freqfix",
};

static int sdp_cpufreq_dev_register(void)
{
	int err;

	err = sysfs_create_group(cpufreq_global_kobject,
						&dbs_attr_group);
	if (err < 0)
		return err;

	err = sysfs_create_file(cpufreq_global_kobject, 
						&cpufreq_on.attr);
	if (err < 0)
		return err;

	err = sysfs_create_file(cpufreq_global_kobject, 
						&avs_on.attr);
	if (err < 0)
		return err;

	err = sysfs_create_file(cpufreq_global_kobject, 
						&avs_table.attr);
	if (err < 0)
		return err;

	err = sysfs_create_file(cpufreq_global_kobject, 
						&freq_limit.attr);
		
	return err;
}
/* SYSFS interface end */
/***********************/

static struct cpufreq_driver sdp_driver = {
	.flags		= CPUFREQ_STICKY,
	.verify		= sdp_verify_speed,
	.target		= sdp_target,
	.get		= sdp_getspeed,
	.init		= sdp_cpufreq_cpu_init,
	.name		= "sdp_cpufreq",
#ifdef CONFIG_PM
	.suspend	= sdp_cpufreq_suspend,
	.resume		= sdp_cpufreq_resume,
#endif
};

static int __init sdp_cpufreq_init(void)
{
	int ret = -EINVAL;
	int i;

	sdp_info = kzalloc(sizeof(struct sdp_dvfs_info), GFP_KERNEL);
	if (!sdp_info)
		return -ENOMEM;

	ret = sdp_cpufreq_mach_init(sdp_info);
	if (ret)
		goto err_vdd_arm;

	if (sdp_info->set_freq == NULL) {
		printk(KERN_ERR "%s: No set_freq function (ERR)\n",	__func__);
		goto err_vdd_arm;
	}

	if (sdp_info->update_volt_table == NULL) {
		printk(KERN_ERR "%s: No update_volt_table function (ERR)\n", __func__);
		goto err_vdd_arm;
	}

	if (sdp_info->set_ema == NULL) {
		printk(KERN_ERR "%s: No set_ema function (ERR)\n", __func__);
		goto err_vdd_arm;
	}

	if (sdp_info->get_emergency_freq_index == NULL) {
		printk(KERN_ERR "%s: No get_emergency_freq_index function (ERR)\n", __func__);
		goto err_vdd_arm;
	}

	arm_regulator = regulator_get(NULL, "VDD_ARM");
	if (IS_ERR(arm_regulator)) {
		printk(KERN_ERR "failed to get resource %s\n", "VDD_ARM");
		goto err_vdd_arm;
	}

	sdp_cpufreq_disable = false;
	sdp_cpufreq_on = false;
	sdp_asv_on = false; /* AVS will be ON in TMU initial status */

//	register_pm_notifier(&sdp_cpufreq_notifier);
//	register_reboot_notifier(&sdp_cpufreq_reboot_notifier);
	cpufreq_register_notifier(&sdp_cpufreq_policy_notifier,
						CPUFREQ_POLICY_NOTIFIER);

	sdp_cpufreq_init_done = true;

	for (i = 0; i < DVFS_LOCK_ID_END; i++) {
		g_cpufreq_lock_val[i] = sdp_info->min_support_idx;
		g_cpufreq_limit_val[i] = sdp_info->max_support_idx;
	}

	g_cpufreq_lock_level = sdp_info->min_support_idx;
	g_cpufreq_limit_level = sdp_info->max_support_idx;

	if (cpufreq_register_driver(&sdp_driver)) {
		pr_err("failed to register cpufreq driver\n");
		goto err_cpufreq;
	}

	/* sysfs register*/
	ret = sdp_cpufreq_dev_register();
	if (ret < 0)
		pr_err("failed to register sysfs device\n");

#if 0 /* moved to TMU code */
	/* apply AVS */
	while (timeout--) {
		ret = regulator_set_voltage(arm_regulator, sdp_info->volt_table[sdp_info->max_support_idx],
					     sdp_info->volt_table[sdp_info->max_support_idx] + 10000);

		if (ret >= 0) {
			break;
		}
	}
	if (timeout == 0) {
		printk(KERN_INFO "DVFS: ERROR - apply AVS - CPU voltage setting error.\n");
	} else {
		printk(KERN_INFO "DVFS: apply AVS - group%d %duV\n", sdp_result_of_asv,
						sdp_info->volt_table[sdp_info->max_support_idx]);
	}
#endif
	return 0;
err_cpufreq:
//	unregister_reboot_notifier(&sdp_cpufreq_reboot_notifier);
//	unregister_pm_notifier(&sdp_cpufreq_notifier);

	if (!IS_ERR(arm_regulator))
		regulator_put(arm_regulator);
err_vdd_arm:
	kfree(sdp_info);
	pr_debug("%s: failed initialization\n", __func__);
	return -EINVAL;
}
late_initcall(sdp_cpufreq_init);

