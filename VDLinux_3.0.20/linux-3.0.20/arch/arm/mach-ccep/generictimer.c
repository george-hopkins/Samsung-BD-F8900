/*
 *  linux/arch/arm/mach-ccep/generictimer.c
 *
 * Copyright (C) 2012 Samsung Electronics.co
 * Author : seungjun.heo@samsung.com
 *
 */
 
#include <linux/init.h>
#include <linux/smp.h>
#include <linux/clockchips.h>
#ifdef CONFIG_SDP_TIMER_SCALING
#include <linux/cpufreq.h>
#include <plat/cpufreq.h>
#endif

#include <asm/irq.h>
#include <asm/smp_twd.h>
#include <asm/localtimer.h>
#include <asm/hardware/gic.h>


#define GT_CTRL_ENABLE	(1 << 0)
#define GT_CTRL_IT_MASK	(1 << 1)
#define GT_CTRL_IT_STAT	(1 << 2)

#define GT_REG_CTRL	0
#define GT_REG_FREQ	1
#define GT_REG_TVAL	2

static u32 gt_rate = 0;

extern unsigned int sdp_revision_id;

//static struct clock_event_device __percpu **gt_evt;

#ifdef CONFIG_SDP_TIMER_SCALING
static unsigned int cpufreq_freq_index;
static struct cpufreq_timerdiv_table *cpufreq_timerdiv_table;
#endif

static u32 gt_reg_read(int reg)
{
	u32 val = 0;
	
	switch(reg)
	{
		case GT_REG_CTRL:
			asm volatile("mrc	p15, 0, %0, c14, c2, 1" : "=r"(val));
			break;
		case GT_REG_FREQ:
			asm volatile("mrc	p15, 0, %0, c14, c0, 0" : "=r"(val));
			break;
	}
	return val;
}

static void gt_reg_write(int reg, u32 val)
{
	switch(reg)
	{
		case GT_REG_CTRL:
			asm volatile("mcr	p15, 0, %0, c14, c2, 1" : : "r"(val));
			break;
		case GT_REG_FREQ:
			asm volatile("mcr	p15, 0, %0, c14, c0, 0" : : "r"(val));
			break;
		case GT_REG_TVAL:
			asm volatile("mcr	p15, 0, %0, c14, c2, 0" : : "r"(val));
			break;
	}
	isb();
}

static void gt_get_rate(void)
{
	if(gt_rate == 0)
	{
		gt_rate = gt_reg_read(GT_REG_FREQ);
#ifdef CONFIG_ARCH_CCEP_FPGA
		*((volatile unsigned int *) (VA_CORE_POWER_BASE + 0xc0)) = 0;		//divider = /4
		*((volatile unsigned int *) (VA_CORE_POWER_BASE + 0xc4)) = 1;		//enable counter
		gt_reg_write(GT_REG_FREQ, 1200000);
		gt_rate = 1200000;
#endif
		if(gt_rate == 0)
		{
			printk("Generic timer : Cannot get frequency!!!\n");
			return;
		}
		printk("ARM Generic timer Frequency : %u.%02uMHz.\n", gt_rate / 1000000,
			(gt_rate / 10000) % 100);
	}
}

static void gt_disable(void)
{
	u32 ctrl;

	ctrl = gt_reg_read(GT_REG_CTRL);
	ctrl &= ~GT_CTRL_ENABLE;

	gt_reg_write(GT_REG_CTRL, ctrl);
}

static void gt_set_mode(enum clock_event_mode mode,
			struct clock_event_device *clk)
{
	switch (mode)
	{
		case CLOCK_EVT_MODE_UNUSED:
		case CLOCK_EVT_MODE_SHUTDOWN:
			gt_disable();
			break;		
		default:
			break;
	}
}

static int gt_set_next_event(unsigned long evt,
			struct clock_event_device *unused)
{
	u32 ctrl;

//	ctrl = gt_reg_read(GT_REG_CTRL);
	ctrl = GT_CTRL_ENABLE;
	ctrl &= ~GT_CTRL_IT_MASK;

#ifdef CONFIG_SDP_TIMER_SCALING
	if (cpufreq_timerdiv_table && sdp_revision_id == 0)
		evt = (evt * cpufreq_timerdiv_table[cpufreq_freq_index].mult) 
				>> cpufreq_timerdiv_table[cpufreq_freq_index].shift;
#endif
	gt_reg_write(GT_REG_TVAL, evt);
	gt_reg_write(GT_REG_CTRL, ctrl);
	
	return 0;
}

/*
 * local_timer_ack: checks for a local timer interrupt.
 *
 * If a local timer interrupt has occurred, acknowledge and return 1.
 * Otherwise, return 0.
 */
int gt_timer_ack(void)
{
	u32 ctrl;

	ctrl = gt_reg_read(GT_REG_CTRL);
	if(ctrl & GT_CTRL_IT_STAT)
	{
		gt_reg_write(GT_REG_CTRL, GT_CTRL_IT_MASK);
		return 1;
	}

	return 0;
}

/*
 * Setup the local clock events for a CPU.
 */
void __cpuinit generic_timer_setup(struct clock_event_device *clk)
{

	gt_get_rate();

	clk->name = "generic_timer";
	clk->features = CLOCK_EVT_FEAT_ONESHOT;
	clk->rating = 350;
	clk->set_mode = gt_set_mode;
	clk->set_next_event = gt_set_next_event;

	/* Make sure our local interrupt controller has this enabled */
	gic_enable_ppi(clk->irq);

	clockevents_config_and_register(clk, gt_rate, 0xf, 0x7fffffff);

}

/*
 * Setup the local clock events for a CPU.
 */
int __cpuinit local_timer_setup(struct clock_event_device *evt)
{
#ifdef CONFIG_ARCH_SDP1202
	extern int sdp1202_get_revision_id(void);
	if(sdp1202_get_revision_id())
	{
#ifdef CONFIG_ARM_TRUSTZONE
		evt->irq = 30;
#else
		evt->irq = 29;
#endif
	}
	else
		evt->irq = IRQ_LOCALTIMER;		
#else
	evt->irq = IRQ_LOCALTIMER;
#endif
	
	generic_timer_setup(evt);
	return 0;
}

#ifdef CONFIG_SDP_TIMER_SCALING
/*
 * must be called with interrupts disabled and on the cpu that is being changed
 */
static void twd_update_cpu_frequency_on_cpu(void *data)
{
	struct cpufreq_freqs *freq = data;
	struct cpufreq_policy policy;
	struct cpufreq_frequency_table *table;

	/* get timer div table */
	if (!cpufreq_timerdiv_table)
		cpufreq_timerdiv_table = sdp_cpufreq_get_timerdiv_table();

	/* find cpufreq_freqindex */
	cpufreq_get_policy(&policy, 0);
	table = cpufreq_frequency_get_table(0);
	cpufreq_frequency_table_target(&policy, table,
					   freq->new, CPUFREQ_RELATION_L, &cpufreq_freq_index);
}

static int twd_cpufreq_notifier(struct notifier_block *nb,
	unsigned long event, void *data)
{
	struct cpufreq_freqs *freq = data;

	/* ES1 is no need to scaling */
	if (sdp_revision_id)
		return 0;

	if (event == CPUFREQ_RESUMECHANGE ||
		(event == CPUFREQ_PRECHANGE && freq->new > freq->old) ||
		(event == CPUFREQ_POSTCHANGE && freq->new < freq->old)) {
		twd_update_cpu_frequency_on_cpu(freq);
	}
	
	return 0;
}

static struct notifier_block twd_cpufreq_notifier_block = {
	.notifier_call = twd_cpufreq_notifier,
};

static int twd_timer_setup_cpufreq(void)
{
	cpufreq_register_notifier(&twd_cpufreq_notifier_block,
			CPUFREQ_TRANSITION_NOTIFIER);

	return 0;
}
arch_initcall(twd_timer_setup_cpufreq);

#endif /* CONFIG_SDP_TIMER_SCALING */

