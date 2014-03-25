/* linux/arch/arm/mach-ccep/ccep_soc/sdp1202/sdp1202_cpufreq.c
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
#include <plat/cpufreq.h>
#include <plat/asv.h>

//#define CONFIG_CPUFREQ_EMA
#define FREQ_LOCK_TIME		50

#define CPUFREQ_LEVEL_END	L14
#define CPUFREQ_ASV_COUNT	12
#define CPUFREQ_EMA_COUNT	4	/* MAX EMA COUNT + 1 */

extern unsigned int sdp_revision_id;

static unsigned int max_support_idx;
static unsigned int min_support_idx = L12;
static int emergency_idx = L13;

//static spinlock_t cpufreq_lock;

static struct clk *cpu_clk;

static struct cpufreq_frequency_table sdp1202_freq_table[] = {
	{ L0, 1350*1000},
	{ L1, 1302*1000},
	{ L2, 1200*1000},
	{ L3, 1100*1000},
	{ L4, 1000*1000},
	{ L5, 900*1000},
	{ L6, 800*1000},
	{ L7, 700*1000},
	{ L8, 600*1000},
	{ L9, 500*1000},	
	{L10, 400*1000},	
	{L11, 300*1000},	
	{L12, 200*1000},	
	{L13, 100*1000},
	{0, CPUFREQ_TABLE_END},
};

/*
@===========================================================================
@PLL PMS Table
@===========================================================================
@CPU,AMS,GPU,DSP		@DDR
@---------------------------------------------------------------------------
@FIN FOUT Value			@FIN FOUT Value
@---------------------------------------------------------------------------
@24   50  0x30C805
@24  100  0x30C804		@24  100  0x30C804
@24  200  0x30C803		@24  200  0x30C803
@24  300  0x20C803		@24  300  0x206402
@24  400  0x30C802		@24  400  0x30C802
@24  500  0x30FA02		@24  500  0x20A702
@24  600  0x20C802		@24  600  0x206401
@24  700  0x30AF01		@24  700  0x207501
@24  800  0x30C801		@24  800  0x30C801
@24  900  0x209601		@24  900  0x209601
@24 1000  0x30FA01		@24 1000  0x20A701
@24 1100  0x311301		@24 1100  0x311301
@24 1200  0x20C801		@24 1200  0x206400
@24 1302  0x40D900		@24 1300  0x30A300
@24 1350  0x40E100
@24 1400  0x30AF00		@24 1400  0x207500
@24 1500  0x207D00		@24 1500  0x207D00
@24 1600  0x30C800		@24 1600  0x30C800
@24 1704  0x208E00		@24 1700  0x208E00
@24 1800  0x209600		@24 1800  0x209600
@24 1896  0x209E00		@24 1900  0x30EE00
@24 2000  0x30FA00		@24 2000  0x20A700
@===========================================================================
*/
static unsigned int clkdiv_cpu[CPUFREQ_LEVEL_END] = {
	/* PMS value */
	0x40E100, /* 1350 L0 */
	0x40D900, /* 1300 L1 */
	0x20C801, /* 1200 L2 */
	0x311301, /* 1100 L3 */
	0x30FA01, /* 1000 L4 */
	0x209601, /*  900 L5 */
	0x30C801, /*  800 L6 */
	0x30AF01, /*  700 L7 */
	0x20C802, /*  600 L8 */
	0x30FA02, /*  500 L9 */
	0x30C802, /*  400 L10 */
	0x20C803, /*  300 L11 */
	0x30C803, /*  200 L12 */
	0x30C804, /*  100 L13, only for emergency situation */
};

static struct cpufreq_timerdiv_table sdp1202_timerdiv_table[CPUFREQ_LEVEL_END] = {
	{256, 8}, /* L0 */
	{256, 8}, /* L1 */
	{256, 8}, /* L2 */
	{256, 8}, /* L3 */
	{256, 8}, /* L4 */
	{256, 8}, /* L5 */
	{256, 8}, /* L6 */
	{256, 8}, /* L7 */
	{256, 8}, /* L8 */
	{256, 8}, /* L9 */
	{256, 8}, /* L10 */
	{256, 8}, /* L11 */
	{256, 8}, /* L12 */
	{256, 8}, /* L13 */		
};

/* voltage table */
/* uV scale */
static unsigned int sdp1202_volt_table[CPUFREQ_LEVEL_END];

/* es0 voltage table */
static const unsigned int sdp1202_asv_voltage_es0[CPUFREQ_LEVEL_END][CPUFREQ_ASV_COUNT] = {
	/*   ASV0,    ASV1,    ASV2,    ASV3,	 ASV4,	  ASV5,	   ASV6,    ASV7,    ASV8,    ASV9,   ASV10,   ASV11 */
	{ 1170000, 1170000, 1170000, 1170000, 1170000, 1170000, 1170000, 1170000, 1170000, 1170000, 1170000, 1170000}, /* L0 */
	{ 1170000, 1170000, 1170000, 1170000, 1170000, 1170000, 1170000, 1170000, 1170000, 1170000, 1170000, 1170000}, /* L1 */
	{ 1170000, 1170000, 1170000, 1170000, 1170000, 1170000, 1170000, 1170000, 1170000, 1170000, 1170000, 1170000}, /* L2 */
	{ 1170000, 1170000, 1170000, 1170000, 1170000, 1170000, 1170000, 1170000, 1170000, 1170000, 1170000, 1170000}, /* L3 */
	{ 1170000, 1170000, 1170000, 1170000, 1170000, 1170000, 1170000, 1170000, 1170000, 1170000, 1170000, 1170000}, /* L4 */
	{ 1170000, 1170000, 1170000, 1170000, 1170000, 1170000, 1170000, 1170000, 1170000, 1170000, 1170000, 1170000}, /* L5 */
	{ 1170000, 1170000, 1150000, 1120000, 1110000, 1080000, 1070000, 1060000, 1060000, 1060000, 1060000, 1060000}, /* L6 */
	{ 1170000, 1130000, 1130000, 1080000, 1060000, 1040000, 1040000, 1020000, 1020000, 1020000, 1020000, 1020000}, /* L7 */
	{ 1170000, 1070000, 1060000, 1060000, 1010000, 1000000, 1000000,  980000,  980000,  980000,  980000,  980000}, /* L8 */
	{ 1170000, 1020000, 1010000, 1010000,  970000,  970000,  970000,  940000,  940000,  940000,  940000,  940000}, /* L9 */
	{ 1170000,  960000,  960000,  960000,  900000,  890000,  890000,  880000,  880000,  880000,  880000,  880000}, /* L10 */
	{ 1170000,  920000,  900000,  880000,  860000,  860000,  850000,  850000,  850000,  850000,  850000,  850000}, /* L11 */
	{ 1170000,  890000,  880000,  870000,  850000,  850000,  850000,  850000,  850000,  850000,  850000,  850000}, /* L12 */
	{ 1170000,  890000,  880000,  870000,  850000,  850000,  850000,  850000,  850000,  850000,  850000,  850000}, /* L13 */		
};
/* es1 voltage table */
static const unsigned int sdp1202_asv_voltage_es1[CPUFREQ_LEVEL_END][CPUFREQ_ASV_COUNT] = {
	/*   ASV0,    ASV1,    ASV2,    ASV3,	 ASV4,	  ASV5,	   ASV6,    ASV7,    ASV8,    ASV9,   ASV10,   ASV11 */
	{ 1130000, 1100000, 1090000, 1070000, 1070000, 1060000, 1060000, 1060000, 1060000, 1060000, 1060000, 1060000}, /* L0 */
	{ 1130000, 1090000, 1080000, 1060000, 1060000, 1060000, 1060000, 1060000, 1060000, 1060000, 1060000, 1060000}, /* L1 */
	{ 1130000, 1050000, 1040000, 1020000, 1010000, 1010000, 1010000, 1010000, 1010000, 1010000, 1010000, 1010000}, /* L2 */
	{ 1130000, 1020000, 1000000,  980000,  980000,  980000,  980000,  980000,  980000,  980000,  980000,  980000}, /* L3 */
	{ 1130000,  980000,  980000,  980000,  980000,  980000,  980000,  980000,  980000,  980000,  980000,  980000}, /* L4 */
	{ 1130000,  980000,  980000,  980000,  980000,  980000,  980000,  980000,  980000,  980000,  980000,  980000}, /* L5 */
	{ 1130000,  980000,  980000,  980000,  980000,  980000,  980000,  980000,  980000,  980000,  980000,  980000}, /* L6 */
	{ 1130000,  980000,  980000,  980000,  980000,  980000,  980000,  980000,  980000,  980000,  980000,  980000}, /* L7 */
	{ 1130000,  980000,  980000,  980000,  980000,  980000,  980000,  980000,  980000,  980000,  980000,  980000}, /* L8 */
	{ 1130000,  980000,  980000,  980000,  980000,  980000,  980000,  980000,  980000,  980000,  980000,  980000}, /* L9 */
	{ 1130000,  980000,  980000,  980000,  980000,  980000,  980000,  980000,  980000,  980000,  980000,  980000}, /* L10 */
	{ 1130000,  980000,  980000,  980000,  980000,  980000,  980000,  980000,  980000,  980000,  980000,  980000}, /* L11 */
	{ 1130000,  980000,  980000,  980000,  980000,  980000,  980000,  980000,  980000,  980000,  980000,  980000}, /* L12 */
	{ 1130000,  980000,  980000,  980000,  980000,  950000,  950000,  950000,  950000,  950000,  950000,  950000}, /* L13 */		
};

/* CPU and L2 cache EMA value table */
#if 0 /* old value */
static const struct cpufreq_ema_table sdp1202_ema_table[CPUFREQ_EMA_COUNT] = {
	{ 950000, 7, 4}, /* less than equal 0.95V */
	{1000000, 4, 3}, /* 0.95 < x <= 1.00V */
	{1100000, 2, 1}, /* 1.00 < x <= 1.10V */
	{9999999, 2, 1}, /* 1.10 < x */
};
#else /* new value */
static const struct cpufreq_ema_table sdp1202_ema_table[CPUFREQ_EMA_COUNT] = {
	{ 900000, 7, 4}, /* less than equal 0.90V */
	{ 950000, 7, 4}, /* 0.90 < x <= 0.95V */
	{1050000, 4, 3}, /* 0.95 < x <= 1.05V */
	{9999999, 2, 1}, /* 1.05 < x */
};
#endif

extern int sdp_set_clockgating(u32 phy_addr, u32 mask, u32 value);

#define SDP1202_PWM_CLK_CON		0x30
#define SEL_ARM_VS_AMS			8
#define SEL_ARM_VS_AMSHALF		12
#define USE_24MHZ_VS_AMSCLOCKS	16
static DEFINE_SPINLOCK(freq_lock);
static void sdp1202_set_clkdiv(unsigned int old_index, unsigned int new_index)
{
	unsigned int val;
	unsigned long flags;
	struct cpufreq_freqs ci;

	spin_lock_irqsave(&freq_lock, flags);

	/* select temp clock source (AMS 500MHz) */
	val = __raw_readl(VA_CORE_MISC_BASE + SDP1202_PWM_CLK_CON);
	//val &= ~(1 << SEL_ARM_VS_AMSHALF | 1 << USE_24MHZ_VS_AMSCLOCKS); // 24MHz
	val = (1 << SEL_ARM_VS_AMSHALF) | (1 << USE_24MHZ_VS_AMSCLOCKS); // AMS 500MHz
	//val = (val & ~(1 << SEL_ARM_VS_AMSHALF)) | (1 << USE_24MHZ_VS_AMSCLOCKS); // AMS 1GHz
	__raw_writel(val, VA_CORE_MISC_BASE + SDP1202_PWM_CLK_CON);
	//printk("select temp clock : 0x%X = 0x%08X\n", VA_CORE_MISC_BASE + SDP1202_PWM_CLK_CON, val);

	ci.flags = 0;
	ci.old = sdp1202_freq_table[old_index].frequency;
	ci.new = 500000;
	adjust_jiffies(CPUFREQ_PRECHANGE, &ci);
	
	/* change CPU clock source to Temp clock (AMS 500MHz) */
	val = __raw_readl(VA_CORE_MISC_BASE + SDP1202_PWM_CLK_CON);
	val &= ~(1U << SEL_ARM_VS_AMS);
	__raw_writel(val, VA_CORE_MISC_BASE + SDP1202_PWM_CLK_CON);
	//printk("change CPU clock source to Temp clock : 0x%X=0x%08X\n",
	//			VA_CORE_MISC_BASE + SDP1202_PWM_CLK_CON, val);

	/* PWD off */
	sdp_set_clockgating(PA_PMU_BASE + 0x90, 0x1, 0);

	/* change CPU pll value */
	__raw_writel(clkdiv_cpu[new_index], VA_PMU_BASE);

	/* adjust jiffies to AMS 500MHz */
	adjust_jiffies(CPUFREQ_POSTCHANGE, &ci);

	/* PWD on */
	sdp_set_clockgating(PA_PMU_BASE + 0x90, 0x1, 1);

	/* wait PLL lock (over 25us) */
	udelay(FREQ_LOCK_TIME);

	/* change CPU clock source to PLL clock */
	val = __raw_readl(VA_CORE_MISC_BASE + SDP1202_PWM_CLK_CON);
	val |= 1 << SEL_ARM_VS_AMS;
	__raw_writel(val, VA_CORE_MISC_BASE + SDP1202_PWM_CLK_CON);
	//printk("change CPU clk source to PLL clk : 0x%X=0x%08X\n",
	//			VA_CORE_MISC_BASE + SDP1202_PWM_CLK_CON, val);

	/* adjust jiffies to new frequency */
	ci.old = 500000;
	ci.new = sdp1202_freq_table[new_index].frequency;
	adjust_jiffies(CPUFREQ_PRECHANGE, &ci);

	spin_unlock_irqrestore(&freq_lock, flags);
}

static void sdp1202_set_frequency(unsigned int old_index, unsigned int new_index)
{
	/* Change the system clock divider values */
#if defined(CONFIG_CPU_FREQ_SDP1202_DEBUG)
	printk("@$%u\n", sdp1202_freq_table[new_index].frequency/10000);
#endif

	/* change cpu frequnecy */
	sdp1202_set_clkdiv(old_index, new_index);
}

#define CPU_EMA_CTRL_REG	(0x60)
#define CPUxEMA_MASK		(0xFFFF)
#define CPU0EMA_SHIFT		(0)
#define CPU1EMA_SHIFT		(4)
#define CPU2EMA_SHIFT		(8)
#define CPU3EMA_SHIFT		(12)
#define L2_EMA_CTRL_REG		(0x64)
#define L2EMA_MASK			(0xF)
#define L2EMA_SHIFT			(0)
static void sdp1202_set_ema(unsigned int index)
{
#if defined(CONFIG_CPUFREQ_EMA)	
	int i;
	unsigned int val;
	unsigned int reg_base = VA_CORE_POWER_BASE; /* 0x10b70000 */

	/* revision check, ES0 is not needed */
	if (!sdp_revision_id)
		return;
	
	/* find ema value */
	for (i = 0; i < CPUFREQ_EMA_COUNT; i++) {
		if (sdp1202_volt_table[index] <= sdp1202_ema_table[i].volt) {
			//printk("%s - input volt = %duV, ema table %d selected\n", 
			//		__func__, sdp1202_volt_table[index], i);
			break;
		}
	}
	if (i == CPUFREQ_EMA_COUNT) {
		printk(KERN_WARNING "WARN: %s - %d uV is not in EMA table.\n",
							__func__, sdp1202_volt_table[index]);
		return;
	}

	/* CPU EMA */
	val = __raw_readl(reg_base + CPU_EMA_CTRL_REG);

	val &= ~CPUxEMA_MASK;
	
	/* cpu0~3ema */
	val |= sdp1202_ema_table[i].cpuema << CPU0EMA_SHIFT;
	val |= sdp1202_ema_table[i].cpuema << CPU1EMA_SHIFT;
	val |= sdp1202_ema_table[i].cpuema << CPU2EMA_SHIFT;
	val |= sdp1202_ema_table[i].cpuema << CPU3EMA_SHIFT;

	__raw_writel(val, reg_base + CPU_EMA_CTRL_REG);

	/* L2 EMA */
	val = __raw_readl(reg_base + L2_EMA_CTRL_REG);
	val &= ~(L2EMA_MASK << L2EMA_SHIFT);
	val |= sdp1202_ema_table[i].l2ema << L2EMA_SHIFT;
	__raw_writel(val, reg_base + L2_EMA_CTRL_REG);	
#else
	return;
#endif
}

static void __init set_volt_table(void)
{
	unsigned int i;
	unsigned int freq;

	/* get current cpu's clock */
	freq = clk_get_rate(cpu_clk);
	for (i = L0; i < CPUFREQ_LEVEL_END; i++) {
		if ((sdp1202_freq_table[i].frequency*1000) == freq)
			break;
	}

	if (i < CPUFREQ_LEVEL_END)
		max_support_idx = i;
	else
		max_support_idx = L7;

	pr_info("DVFS: current CPU clk = %dMHz, max support freq is %dMHz",
				freq/1000000, sdp1202_freq_table[i].frequency/1000);
	
	for (i = L0; i < max_support_idx; i++)
		sdp1202_freq_table[i].frequency = CPUFREQ_ENTRY_INVALID;

	pr_info("DVFS: VDD_ARM Voltage table is setted with asv group %d\n", sdp_result_of_asv);

	if (sdp_result_of_asv < CPUFREQ_ASV_COUNT) { 
		for (i = 0; i < CPUFREQ_LEVEL_END; i++) {
			/* ES1 */
			if (sdp_revision_id)
				sdp1202_volt_table[i] =
					sdp1202_asv_voltage_es1[i][sdp_result_of_asv];
			/* ES0 */
			else
				sdp1202_volt_table[i] = 
					sdp1202_asv_voltage_es0[i][sdp_result_of_asv];
		}
	} else {
		pr_err("%s: asv table index error. %d\n", __func__, sdp_result_of_asv);
	}
}

static void __init set_timerdiv_table(void)
{
	int i;

	for (i = 0; i < CPUFREQ_LEVEL_END; i++) {
		if (sdp1202_freq_table[i].frequency == (unsigned int)CPUFREQ_ENTRY_INVALID)
			sdp1202_timerdiv_table[i].mult = 1 << 8;
		else
			sdp1202_timerdiv_table[i].mult = ((sdp1202_freq_table[i].frequency/1000)<<8) /
											(sdp1202_freq_table[max_support_idx].frequency/1000);
		sdp1202_timerdiv_table[i].shift = 8;
	}
}

static void update_volt_table(void)
{
	int i;

	pr_info("DVFS: VDD_ARM Voltage table is setted with asv group %d\n", sdp_result_of_asv);

	if (sdp_result_of_asv < CPUFREQ_ASV_COUNT) { 
		for (i = 0; i < CPUFREQ_LEVEL_END; i++) {
			/* ES1 */
			if (sdp_revision_id)
				sdp1202_volt_table[i] = 
					sdp1202_asv_voltage_es1[i][sdp_result_of_asv];
			/* ES0 */
			else
				sdp1202_volt_table[i] = 
					sdp1202_asv_voltage_es0[i][sdp_result_of_asv];
		}
	} else {
		pr_err("%s: asv table index error. %d\n", __func__, sdp_result_of_asv);
	}
}

static int sdp1202_get_emergency_freq_index(void)
{
	return emergency_idx;
}

static bool sdp1202_pms_change(unsigned int old_index, unsigned int new_index)
{
	unsigned int old_pm = clkdiv_cpu[old_index];
	unsigned int new_pm = clkdiv_cpu[new_index];

	return (old_pm == new_pm) ? 0 : 1;
}

int sdp_cpufreq_mach_init(struct sdp_dvfs_info *info)
{
	cpu_clk = clk_get(NULL, "FCLK");
	if (IS_ERR(cpu_clk)) {
		/* error */
		printk(KERN_ERR "%s - clock get fail", __func__);
		return PTR_ERR(cpu_clk);
	}

	set_volt_table();
	set_timerdiv_table();
	
	info->pll_safe_idx = L5;
	info->pm_lock_idx = L5;
	info->max_support_idx = max_support_idx;
	info->min_support_idx = min_support_idx;
//	info->gov_support_freq = NULL;
	info->cpu_clk = cpu_clk;
	info->volt_table = sdp1202_volt_table;
	info->freq_table = sdp1202_freq_table;
	info->div_table = sdp1202_timerdiv_table;
	info->set_freq = sdp1202_set_frequency;
	info->need_apll_change = sdp1202_pms_change;
//	info->get_speed = sdp1202_getspeed;
	info->update_volt_table = update_volt_table;
	info->set_ema = sdp1202_set_ema;
	info->get_emergency_freq_index = sdp1202_get_emergency_freq_index;

	printk(KERN_INFO "DVFS: freq lock delay = %dus\n", FREQ_LOCK_TIME);
#ifdef CONFIG_CPUFREQ_EMA
	printk(KERN_INFO "DVFS: use EMA settings\n");
#else
	printk(KERN_INFO "DVFS: no use EMA settings\n");
#endif

	return 0;	
}
EXPORT_SYMBOL(sdp_cpufreq_mach_init);

