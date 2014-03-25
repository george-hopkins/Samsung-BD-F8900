/*
 *  linux/arch/arm/kernel/irq.c
 *
 *  Copyright (C) 1992 Linus Torvalds
 *  Modifications for ARM processor Copyright (C) 1995-2000 Russell King.
 *
 *  Support for Dynamic Tick Timer Copyright (C) 2004-2005 Nokia Corporation.
 *  Dynamic Tick Timer written by Tony Lindgren <tony@atomide.com> and
 *  Tuukka Tikkanen <tuukka.tikkanen@elektrobit.com>.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  This file contains the code used by various IRQ handling routines:
 *  asking for different IRQ's should be done through these routines
 *  instead of just grabbing them. Thus setups with different IRQ numbers
 *  shouldn't result in any weird surprises, and installing new handlers
 *  should be easier.
 *
 *  IRQ's are in fact implemented a bit like signal handlers for the kernel.
 *  Naturally it's not a 1:1 relation, but there are similarities.
 */
#include <linux/kernel_stat.h>
#include <linux/module.h>
#include <linux/signal.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/random.h>
#include <linux/smp.h>
#include <linux/init.h>
#include <linux/seq_file.h>
#include <linux/errno.h>
#include <linux/list.h>
#include <linux/kallsyms.h>
#include <linux/proc_fs.h>
#include <linux/ftrace.h>
#include <linux/delay.h>

#include <asm/system.h>
#include <asm/mach/arch.h>
#include <asm/mach/irq.h>
#include <asm/mach/time.h>

/*
 * No architecture-specific irq_finish function defined in arm/arch/irqs.h.
 */
#ifndef irq_finish
#define irq_finish(irq) do { } while (0)
#endif

unsigned long irq_err_count;
#ifdef CONFIG_ARCH_CCEP
extern int g_irq_print;

void show_irq(void)
{
	int i, cpu;
	struct irq_desc *desc;
	struct irqaction *action;
	unsigned long flags;
	char cpuname[12];
#ifdef CONFIG_IRQ_TIME
        unsigned int cu_cpu = smp_processor_id();
        struct timeval now;
#endif
	printk( KERN_ALERT "=============================================================\n");
	printk( KERN_ALERT "interrupt monitor\n");
	printk( KERN_ALERT "-------------------------------------------------------------\n");

	printk("    ");
	for_each_present_cpu(cpu) {
		sprintf(cpuname, "CPU%d", cpu);
		printk(" %10s", cpuname);
	}
	printk("\n");

	for(i = 0; i < nr_irqs; i++)
	{
		desc = irq_to_desc(i);
		raw_spin_lock_irqsave(&desc->lock, flags);
		action = desc->action;
		if (!action)
			goto unlock;

		printk("%3d: ", i);
		for_each_present_cpu(cpu)
			printk("%10u ", kstat_irqs_cpu(i, cpu));
		printk(" %10s", desc->name ? : "-");
		printk("  %s", action->name);

		for (action = action->next; action; action = action->next)
			printk(", %s", action->name);
#ifdef CONFIG_IRQ_TIME
		printk("  (max %d usec)",desc->runtime);
#endif

		printk("\n");
unlock:
		raw_spin_unlock_irqrestore(&desc->lock, flags);

	}
	printk("Err: %10lu\n", irq_err_count);

	printk( KERN_ALERT "=============================================================\n");
#ifdef CONFIG_IRQ_TIME
                do_gettimeofday(&now);
		printk(KERN_ERR"IRQ : %d CPU : %d\n",irq_desc_last[cu_cpu].irq,cu_cpu);
                if(irq_desc_last[cu_cpu].name)
                        printk(KERN_ERR"LAST IRQ NAME : %s",irq_desc_last[cu_cpu].name);
                printk(KERN_ERR"LAST IRQ TIME : %d usec ",irq_desc_last[cu_cpu].last_time);
                printk(KERN_ERR" NOW : %d usec ",(now.tv_sec*1000000) + now.tv_usec);
                printk(KERN_ERR" IRQ WATCHDOG : %d usec\n",((now.tv_sec*1000000) + now.tv_usec) - irq_desc_last[cu_cpu].last_time);
	printk( KERN_ALERT "=============================================================\n");
#endif

}
EXPORT_SYMBOL(show_irq);
#endif

int arch_show_interrupts(struct seq_file *p, int prec)
{
#ifdef CONFIG_FIQ
	show_fiq_list(p, prec);
#endif
#ifdef CONFIG_SMP
	show_ipi_list(p, prec);
#endif
#ifdef CONFIG_LOCAL_TIMERS
	show_local_irqs(p, prec);
#endif
	seq_printf(p, "%*s: %10lu\n", prec, "Err", irq_err_count);
	return 0;
}

/*
 * do_IRQ handles all hardware IRQ's.  Decoded IRQs should not
 * come via this function.  Instead, they should provide their
 * own 'handler'
 */
asmlinkage void __exception_irq_entry
asm_do_IRQ(unsigned int irq, struct pt_regs *regs)
{
	struct pt_regs *old_regs = set_irq_regs(regs);

#if defined(CONFIG_ARCH_CCEP) && defined(CONFIG_ARM_GIC) && !(defined(CONFIG_ARCH_SDP1202) || defined(CONFIG_ARCH_SDP1207))
	udelay(2);
#endif

#ifdef CONFIG_ARCH_CCEP
	if(g_irq_print)show_irq();
#endif

	irq_enter();

	/*
	 * Some hardware gives randomly wrong interrupts.  Rather
	 * than crashing, do something sensible.
	 */
	if (unlikely(irq >= nr_irqs)) {
		if (printk_ratelimit())
			printk(KERN_WARNING "Bad IRQ%u\n", irq);
		ack_bad_irq(irq);
	} else {
		generic_handle_irq(irq);

	       /* VDLinux 3.x , based VDLP.4.2.1.x default patch No.12,
        	  detect kernel stack overflow, SP Team 2010-02-08 */
#ifdef CONFIG_DEBUG_STACKOVERFLOW
#ifndef STACK_WARN
# define STACK_WARN (THREAD_SIZE/8)
#endif
	        /* Debugging check for stack overflow */
		{
	                register unsigned long sp asm ("sp");
	                unsigned long thread_info ;
	                extern void print_modules(void);
	                thread_info =(sp & ~(THREAD_SIZE - 1));

	               if (unlikely(sp < thread_info + sizeof(struct thread_info) + STACK_WARN)) {
        	               printk(KERN_ERR "stack overflow: 0x%lx\n", sp);
                	       print_modules();
	                       show_regs(get_irq_regs());
        	               dump_stack();
        	       }
		}
#endif

	}

	/* AT91 specific workaround */
	irq_finish(irq);

	irq_exit();
	set_irq_regs(old_regs);
}

void set_irq_flags(unsigned int irq, unsigned int iflags)
{
	unsigned long clr = 0, set = IRQ_NOREQUEST | IRQ_NOPROBE | IRQ_NOAUTOEN;

	if (irq >= nr_irqs) {
		printk(KERN_ERR "Trying to set irq flags for IRQ%d\n", irq);
		return;
	}

	if (iflags & IRQF_VALID)
		clr |= IRQ_NOREQUEST;
	if (iflags & IRQF_PROBE)
		clr |= IRQ_NOPROBE;
	if (!(iflags & IRQF_NOAUTOEN))
		clr |= IRQ_NOAUTOEN;
	/* Order is clear bits in "clr" then set bits in "set" */
	irq_modify_status(irq, clr, set & ~clr);
}

void __init init_IRQ(void)
{
	machine_desc->init_irq();
}

#ifdef CONFIG_SPARSE_IRQ
int __init arch_probe_nr_irqs(void)
{
	nr_irqs = machine_desc->nr_irqs ? machine_desc->nr_irqs : NR_IRQS;
	return nr_irqs;
}
#endif

#ifdef CONFIG_HOTPLUG_CPU

static bool migrate_one_irq(struct irq_data *d)
{
	unsigned int cpu = cpumask_any_and(d->affinity, cpu_online_mask);
	bool ret = false;

	if (cpu >= nr_cpu_ids) {
		cpu = cpumask_any(cpu_online_mask);
		ret = true;
	}

	pr_debug("IRQ%u: moving from cpu%u to cpu%u\n", d->irq, d->node, cpu);

	d->chip->irq_set_affinity(d, cpumask_of(cpu), true);

	return ret;
}

/*
 * The CPU has been marked offline.  Migrate IRQs off this CPU.  If
 * the affinity settings do not allow other CPUs, force them onto any
 * available CPU.
 */
void migrate_irqs(void)
{
	unsigned int i, cpu = smp_processor_id();
	struct irq_desc *desc;
	unsigned long flags;

	local_irq_save(flags);

	for_each_irq_desc(i, desc) {
		struct irq_data *d = &desc->irq_data;
		bool affinity_broken = false;

		raw_spin_lock(&desc->lock);
		do {
			if (desc->action == NULL)
				break;

			if (d->node != cpu)
				break;

			affinity_broken = migrate_one_irq(d);
		} while (0);
		raw_spin_unlock(&desc->lock);

		if (affinity_broken && printk_ratelimit())
			pr_warning("IRQ%u no longer affine to CPU%u\n", i, cpu);
	}

	local_irq_restore(flags);
}
#endif /* CONFIG_HOTPLUG_CPU */
