/* linux/arch/arm/mach-ccep/sdp1202_cpufreq.c
 *
 * Copyright (c) 2012 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com
 *
 * SDP1202 - CPU frequency scaling support
 *
 */
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/regulator/consumer.h>
#include <linux/cpufreq.h>
#include <linux/cpu.h>
#include <linux/delay.h>

#include <asm/cacheflush.h>

#include <mach/hardware.h>
#include <mach/platform.h>

static spinlock_t cpufreq_lock;

static struct clk *cpu_clk;

//static struct regulator *arm_regulator;

static struct cpufreq_freqs freqs;

enum cpufreq_level_index {
	L0,
	L1,
	L2,
	L3,
	L4,
	L5,
	L6,
	L7,
	L8,
	CPUFREQ_LEVEL_END,
};

static struct cpufreq_frequency_table sdp1202_freq_table[] = {
	{L0, 1000*1000},
	{L1, 900*1000},
	{L2, 800*1000},
	{L3, 700*1000},
	{L4, 600*1000},
	{L5, 500*1000},
	{L6, 400*1000},
	{L7, 300*1000},
	{L8, 250*1000},	
	{0, CPUFREQ_TABLE_END},
};

static unsigned int clkdiv_cpu[CPUFREQ_LEVEL_END] = {
	/* PMU_PMS value */
	0x00307D00, /* ARM L0 : 1000 MHz */
	0x00409600, /* ARM L1 : 900 MHz */
	0x00306400, /* ARM L2 : 800 MHz */
	0x0030AF01, /* ARM L3 : 700 MHz */
	0x0040C801, /* ARM L4 : 600 MHz */
	0x00307D01, /* ARM L5 : 500 MHz */
	0x00306401, /* ARM L6 : 400 MHz */
	0x0040C802, /* ARM L7 : 300 MHz */
	0x00307D02, /* ARM L8 : 250 MHz */
};

struct cpufreq_voltage_table {
	unsigned int	index;		/* level */
	unsigned int	arm_volt;	/* mV */
	unsigned int	gpio_val;	/* gpio value for PMIC control */
};

struct gic_chip_data {
	u32 saved_spi_enable[DIV_ROUND_UP(1020, 32)];
	u32 saved_spi_conf[DIV_ROUND_UP(1020, 16)];
	u32 saved_spi_pri[DIV_ROUND_UP(1020, 4)];
	u32 saved_spi_target[DIV_ROUND_UP(1020, 4)];
	u32 saved_ppi_enable[DIV_ROUND_UP(32, 32)];
	u32 saved_ppi_conf[DIV_ROUND_UP(32, 16)];
//	u32 saved_ppi_pri;
};

/* voltage table */
static struct cpufreq_voltage_table sdp1202_volt_table[CPUFREQ_LEVEL_END] = {
	{ L0, 1132, 0x4 },
	{ L1, 1132, 0x4 },
	{ L2, 1132, 0x4 },
	{ L3, 1132, 0x4 },
	{ L4, 1132, 0x4 },
	{ L5, 1132, 0x4 },
	{ L6, 1132, 0x4 },
	{ L7, 1132, 0x4 },
	{ L8, 1084, 0x7 },
};

static void sdp1202_set_clock(unsigned int div_index)
{
	/* TODO: set PMS value here. */
	//R_PMU_CPU_PMS_CON = clkdiv_cpu[div_index]; // ARM sets pms value directily.
}

int sdp1202_verify_speed(struct cpufreq_policy *policy)
{
	return cpufreq_frequency_table_verify(policy, sdp1202_freq_table);
}

unsigned int sdp1202_getspeed(unsigned int cpu)
{
	unsigned int val = 0;
	
	/* TODO: calculate frequency here */
//	val = R_PMU_CPU_PMS_CON;
//	val = (INPUT_FREQ >> (GET_PLL_S(val))) / GET_PLL_P(val) * GET_PLL_M(val);
//	val /= 1000;
	
	return val;
//	return clk_get_rate(cpu_clk) / 1000;
}

void sdp1202_set_clkdiv(unsigned int div_index)
{
	unsigned long flags;
	
//	printk("\033[1;7;33mSet clock: %dMHz(0x%x), cur pms val = 0x%x\033[0m\n", 
//			sdp1202_freq_table[div_index].frequency/1000,
//			clkdiv_cpu[div_index], R_PMU_CPU_PMS_CON);

	spin_lock_irqsave(&cpufreq_lock, flags);

	sdp1202_set_clock(div_index);

	spin_unlock_irqrestore(&cpufreq_lock, flags);
	
//	printk("\033[1;7;33mcur pms val = 0x%x\033[0m\n", R_PMU_CPU_PMS_CON);
}

static void sdp1202_set_frequency(unsigned int old_index, unsigned int new_index)
{
	/* Change the system clock divider values */
	sdp1202_set_clkdiv(new_index);
}

/* change the arm core voltage */
static void sdp1202_set_voltage(unsigned int index)
{
//	unsigned long flags;
//	unsigned int val;
//	static unsigned int old_val = 0;

	/* TODO: implement here */

#if 0
	/* set gpio */
	//printk(">> set voltage : %dmV\n", sdp1202_volt_table[index].arm_volt);
	spin_lock_irqsave(&cpufreq_lock, flags);

	if (old_val != sdp1202_volt_table[index].gpio_val) {
		val = VA_GPIO8_WRITE;
		val = (val & 0xF) | sdp1202_volt_table[index].gpio_val;
		VA_GPIO8_WRITE = val;
		printk("\033[1;7;32mVoltage set to %dmV\033[0m\n", sdp1202_volt_table[index].arm_volt);
	}

	old_val = sdp1202_volt_table[index].gpio_val;

	spin_unlock_irqrestore(&cpufreq_lock, flags);
#endif	
}

static int sdp1202_target(struct cpufreq_policy *policy,
			  unsigned int target_freq,
			  unsigned int relation)
{
	unsigned int target_index, old_index;
	//unsigned int arm_volt;

	/* get current cpu frequency */
	//freqs.old = policy->cur;
	freqs.old = sdp1202_getspeed(policy->cpu);

	/* check the current cpu frequency and get freq_table index */
	if (cpufreq_frequency_table_target(policy, sdp1202_freq_table,
					   freqs.old, relation, &old_index))
		return -EINVAL;

	/* check the target cpu frequency and get target index */
	if (cpufreq_frequency_table_target(policy, sdp1202_freq_table,
					   target_freq, relation, &target_index))
		return -EINVAL;

	freqs.new = sdp1202_freq_table[target_index].frequency;
	freqs.cpu = policy->cpu;
	//printk(">> old=%d, new=%d\n", freqs.old, freqs.new);

	if (freqs.new == freqs.old) {
		//printk(">> cpufreq : new == old\n");
		return 0;
	}

	/* get the voltage value */
	//arm_volt = sdp1202_volt_table[target_index].arm_volt;

	cpufreq_notify_transition(&freqs, CPUFREQ_PRECHANGE);

	/* If the new frequency is higher than old one,
	 * change the voltage first. 
	 * Otherwise, if new freq is lower than old one,
	 * change the frequency first.
	 */
	/* control regulator */
	if (freqs.new > freqs.old) {
		/* Voltage up */
		sdp1202_set_voltage(target_index);
	}

	/* Clock Configuration Procedure */
	sdp1202_set_frequency(old_index, target_index);

	/* control regulator */
	if (freqs.new < freqs.old) {
		/* Voltage down */
		sdp1202_set_voltage(target_index);
	}

	cpufreq_notify_transition(&freqs, CPUFREQ_POSTCHANGE);

	return 0;
}

#ifdef CONFIG_PM
static int sdp1202_cpufreq_suspend(struct cpufreq_policy *policy)
{
	return 0;
}

static int sdp1202_cpufreq_resume(struct cpufreq_policy *policy)
{
	return 0;
}
#endif

static int sdp1202_cpufreq_cpu_init(struct cpufreq_policy *policy)
{
	policy->cur = policy->min = policy->max = sdp1202_getspeed(policy->cpu);

	cpufreq_frequency_table_get_attr(sdp1202_freq_table, policy->cpu);

	/* set the transition latency value */
	policy->cpuinfo.transition_latency = 100000;

	/*
	 * SDP1202 processors has 2 cores
	 * that the frequency cannot be set independently.
	 * Each cpu is bound to the same speed.
	 * So the affected cpu is all of the cpus.
	 */
	cpumask_setall(policy->cpus);

	return cpufreq_frequency_table_cpuinfo(policy, sdp1202_freq_table);
}

static struct cpufreq_driver sdp1202_driver = {
	.flags		= CPUFREQ_STICKY,
	.verify		= sdp1202_verify_speed,
	.target		= sdp1202_target,
	.get		= sdp1202_getspeed,
	.init		= sdp1202_cpufreq_cpu_init,
	.name		= "sdp1202_cpufreq",
#ifdef CONFIG_PM
	.suspend	= sdp1202_cpufreq_suspend,
	.resume		= sdp1202_cpufreq_resume,
#endif
};

static int __init sdp1202_cpufreq_init(void)
{
	cpu_clk = clk_get(NULL, "FCLK");
	if (IS_ERR(cpu_clk)) {
		/* error */
		printk(KERN_ERR "%s - clock get fail", __func__);
		return PTR_ERR(cpu_clk);
	}
		
	return cpufreq_register_driver(&sdp1202_driver);
}
late_initcall(sdp1202_cpufreq_init);

