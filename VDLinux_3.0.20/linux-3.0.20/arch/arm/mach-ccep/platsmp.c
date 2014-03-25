/*
 *  linux/arch/arm/mach-ccep/platsmp.c
 *
 * Copyright (c) 2011 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com/
 *
 * Cloned from linux/arch/arm/mach-vexpress/platsmp.c
 *
 *  Copyright (C) 2002 ARM Ltd.
 *  All Rights Reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/jiffies.h>
#include <linux/smp.h>
#include <linux/io.h>

#include <asm/cacheflush.h>
#include <asm/mach-types.h>
#include <asm/localtimer.h>
#include <asm/unified.h>
#include <asm/hardware/gic.h>

#include <asm/smp_scu.h>

#include <mach/hardware.h>
#include <mach/platform.h>

#ifdef CONFIG_USE_EXT_GIC
extern unsigned int gic_bank_offset;
#endif
extern void sdp_secondary_startup(void);

/*
 * control for which core is the next to come out of the secondary
 * boot "holding pen"
 */
volatile int pen_release = -1;

#ifdef CONFIG_HAVE_ARM_SCU
static void __iomem *scu_base_addr(void)
{
	return (void __iomem *) VA_SCU_BASE;
}

static inline unsigned int get_core_count(void)
{
	void __iomem *scu_base = scu_base_addr();
	if (scu_base)
		return scu_get_core_count(scu_base);
	return 1;
}
#endif

static DEFINE_SPINLOCK(boot_lock);

#ifdef CONFIG_USE_EXT_GIC
static void __cpuinit sdp_gic_ext_secondary_init(void)
{
	void __iomem *dist_base = (void __iomem *) 
		(VA_GIC_DIST_BASE + gic_bank_offset * smp_processor_id());
	void __iomem *base = (void __iomem *) 
		(VA_GIC_CPU_BASE + gic_bank_offset * smp_processor_id());
	int i;

	/*
	 * Deal with the banked PPI and SGI interrupts - disable all
	 * PPI interrupts, ensure all SGI interrupts are enabled.
	 */
	writel_relaxed(0xffff0000, dist_base + GIC_DIST_ENABLE_CLEAR);
	writel_relaxed(0x0000ffff, dist_base + GIC_DIST_ENABLE_SET);

	/*
	 * Set priority on PPI and SGI interrupts
	 */

#if defined(CONFIG_ARM_TRUSTZONE) && defined(CONFIG_ARCH_SDP1202)
	for (i = 0; i < 32; i += 4)
		writel_relaxed(0xd0d0d0d0, dist_base + GIC_DIST_PRI + i * 4 / 4);
	
	writel_relaxed(0xff, base + GIC_CPU_PRIMASK);
	writel_relaxed(0x1f, base + GIC_CPU_CTRL);
#else
	for (i = 0; i < 32; i += 4)
		writel_relaxed(0xa0a0a0a0, dist_base + GIC_DIST_PRI + i * 4 / 4);
	
	writel_relaxed(0xf0, base + GIC_CPU_PRIMASK);
	writel_relaxed(1, base + GIC_CPU_CTRL);
#endif
}
#endif

void __cpuinit platform_secondary_init(unsigned int cpu)
{
	/*
	 * if any interrupts are already enabled for the primary
	 * core (e.g. timer irq), then they will not have been enabled
	 * for us: do so
	 */
#ifdef CONFIG_USE_EXT_GIC
	sdp_gic_ext_secondary_init();
#else
	gic_secondary_init(0);
#endif

	/*
	 * let the primary processor know we're out of the
	 * pen, then head off into the C entry point
	 */
	pen_release = -1;
	smp_wmb();

	/*
	 * Synchronise with the boot thread.
	 */
	spin_lock(&boot_lock);
	spin_unlock(&boot_lock);

	set_cpu_online(cpu, true);
	
}

#define COREPMU_CLKEN	0x34

int __cpuinit boot_secondary(unsigned int cpu, struct task_struct *idle)
{
	unsigned long timeout;
	void __iomem * base, *base2;
#if defined(CONFIG_ARCH_SDP1202)
	unsigned int ldr_asm = 0xE59FF034;
#endif
	//u32 reg;
	printk("start boot_secondary....\r\n");

	base = ioremap(0x0, 512);						//if FPGA, size is small
#if defined(CONFIG_ARCH_SDP1202)
	ldr_asm +=  (( 0x3 - cpu ) * 4);
	__raw_writel(ldr_asm, base);						//jump to [0x3C] -> sdp_secondary_startup
	__raw_writel(BSYM(virt_to_phys(sdp_secondary_startup)), base + 0x3C + (( 0x3 - cpu ) * 4) );
#else
	__raw_writel(0xE59FF034, base);						//jump to [0x3C] -> sdp_secondary_startup
	__raw_writel(BSYM(virt_to_phys(sdp_secondary_startup)), base + 0x3C);
#endif
#if defined(CONFIG_ARCH_SDP1106FPGA) || defined(CONFIG_ARCH_SDP1105FPGA)		
// null
#elif defined(CONFIG_ARCH_SDP1207FPGA)
        __raw_writel(0xE51FF004, base);         //jump to [0x80] -> sdp_secondary_startup
        __raw_writel(BSYM(virt_to_phys(sdp_secondary_startup)), base + 0x04);
        wmb();
        __raw_writel(V_CORE_POWER_VALUE, VA_CORE_POWER_BASE + 0x34);
#else
	wmb();
//	base2 = ioremap(PA_CORE_POWER_BASE, 0x40);
	__raw_writel(V_CORE_POWER_VALUE, VA_CORE_POWER_BASE + 0x34);
//	iounmap(base2);
#endif
	iounmap(base);
	mdelay(10);

#if 0
#if defined(CONFIG_ARCH_SDP1105) || defined(CONFIG_ARCH_SDP1106)
 	writel(0, 0xFE090CDC);		//Set Stepping Stone
	base = ioremap(0x0, 512);	//if ES, size is small
	base2 = ioremap(PA_CORE_POWER_BASE, 0x40);
//	__raw_writel(0x2bfff, base2 + 0x34);
	__raw_writel(0xE59FF034, base);		 //jump to [0x80]
	__raw_writel(0xEAFFFFFE, base + 0x4);	 //jump to 0
	__raw_writel(0xEAFFFFFE, base + 0x8);	 //jump to 0
	__raw_writel(0xEAFFFFFE, base + 0xC);	 //jump to 0
	__raw_writel(0xEAFFFFFE, base + 0x10);	 //jump to 0
	__raw_writel(BSYM(virt_to_phys(sdp_secondary_startup)), base + 0x3C);	//[0x80] -> sdp_secondary_startup
	wmb();
	mdelay(10);

//	writel(0, 0xFEB70404);		//Set CPU1 PowerDown Disable
	writel(1, 0xFEB70408);		//Set CPU1 PowerUp Enable
	wmb();
	mdelay(10);
	__raw_writel(0x3feb, base2 + 0x34);	//Core 1 reset
	wmb();
	mdelay(10);
	__raw_writel(0x3fff, base2 + 0x34);
	wmb();
	mdelay(10);
#endif
#endif

	/*
	 * set synchronisation state between this boot processor
	 * and the secondary one
	 */
	spin_lock(&boot_lock);

	/* enable cpu clock on cpu1 */

	/*
	 * The secondary processor is waiting to be released from
	 * the holding pen - release it, then wait for it to flag
	 * that it has been released by resetting pen_release.
	 *
	 * Note that "pen_release" is the hardware CPU ID, whereas
	 * "cpu" is Linux's internal ID.
	 */
	pen_release = cpu;
	__cpuc_flush_dcache_area((void *)&pen_release, sizeof(pen_release));
	outer_clean_range(__pa(&pen_release), __pa(&pen_release + 1));

	/*
	 * XXX
	 *
	 * This is a later addition to the booting protocol: the
	 * bootMonitor now puts secondary cores into WFI, so
	 * poke_milo() no longer gets the cores moving; we need
	 * to send a soft interrupt to wake the secondary core.
	 * Use smp_cross_call() for this, since there's little
	 * point duplicating the code here
	 */

	timeout = jiffies + (1 * HZ);
	while (time_before(jiffies, timeout)) {
		smp_rmb();
		if (pen_release == -1)
		{
			printk("pen release ok!!!!!\n");
			break;
		}

		udelay(10);
	}


	/*
	 * now the secondary core is starting up let it run its
	 * calibrations, then wait for it to finish
	 */
	spin_unlock(&boot_lock);

	return pen_release != -1 ? -ENOSYS : 0;
}

/*
 * Initialise the CPU possible map early - this describes the CPUs
 * which may be present or become present in the system.
 */
void __init smp_init_cpus(void)
{
	unsigned int i, ncores;

#ifdef CONFIG_HAVE_ARM_SCU
	ncores = get_core_count();
#else
	ncores = nr_cpu_ids;
#endif

	/* sanity check */
	if (ncores > nr_cpu_ids) {
		pr_warn("SMP: %u cores greater than maximum (%u), clipping\n",
			ncores, nr_cpu_ids);
		ncores = nr_cpu_ids;
	}

	for (i = 0; i < ncores; i++)
		set_cpu_possible(i, true);

	set_smp_cross_call(gic_raise_softirq);
}

void __init platform_smp_prepare_cpus(unsigned int max_cpus)
{
	int i;
	void __iomem * base;

	for(i = 0; i < max_cpus; i++)
		set_cpu_present(i, true);
	
#ifdef CONFIG_HAVE_ARM_SCU
	scu_enable(scu_base_addr());
#endif
}

