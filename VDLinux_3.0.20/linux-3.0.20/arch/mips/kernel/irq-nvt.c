/*
 * Copyright 2001 MontaVista Software Inc.
 * Author: Jun Sun, jsun@mvista.com or jsun@junsun.net
 *
 * Copyright (C) 2001 Ralf Baechle
 * Copyright (C) 2005  MIPS Technologies, Inc.  All rights reserved.
 *      Author: Maciej W. Rozycki <macro@mips.com>
 *
 * This file define the irq handler for MIPS CPU interrupts.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

/*
 * Almost all MIPS CPUs define 8 interrupt sources.  They are typically
 * level triggered (i.e., cannot be cleared from CPU; must be cleared from
 * device).  The first two are software interrupts which we don't really
 * use or support.  The last one is usually the CPU timer interrupt if
 * counter register is present or, for CPUs with an external FPU, by
 * convention it's the FPU exception interrupt.
 *
 * Don't even think about using this on SMP.  You have been warned.
 *
 * This file exports one global function:
 *	void mips_cpu_irq_init(void);
 */
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>

#include <asm/irq_cpu.h>
#include <asm/mipsregs.h>
#include <asm/mipsmtregs.h>
#include <asm/system.h>

#include <asm/wbflush.h>

#ifdef NOVATEK_PATCH_WITH_USB
#include <linux/module.h>
#endif 

#include <nvt-int.h>

static inline void unmask_mips_irq(struct irq_data *d)
{
	clear_bit(d->irq - MIPS_CPU_IRQ_BASE, (void *)NVT_IRQ_MASK_ADDR_L);

	wbflush_ahb();
	
	irq_enable_hazard();
}
#ifdef NOVATEK_PATCH_WITH_USB 
EXPORT_SYMBOL(unmask_mips_irq);
#endif

static inline void mask_mips_irq(struct irq_data *d)
{
	set_bit(d->irq - MIPS_CPU_IRQ_BASE, (void *)NVT_IRQ_MASK_ADDR_L);

	wbflush_ahb();

	irq_disable_hazard();
}
#ifdef NOVATEK_PATCH_WITH_USB 
EXPORT_SYMBOL(mask_mips_irq);
#endif

static struct irq_chip mips_cpu_irq_controller = {
	.name			= "MIPS",
	.irq_ack		= mask_mips_irq,
	.irq_mask		= mask_mips_irq,
	.irq_mask_ack	= mask_mips_irq,
	.irq_unmask		= unmask_mips_irq,
	.irq_eoi		= unmask_mips_irq,
};

void __init mips_cpu_irq_init(void)
{
	unsigned int i;

	/* Mask interrupts. */
	clear_c0_status(ST0_IM);
	clear_c0_cause(CAUSEF_IP);

	/* Mask all interrupt */
	NVT_IRQ_MASK_L = 0xFFFFFFFF;
	NVT_IRQ_MASK_H = 0xFFFFFFFF;
	NVT_IRQ_LEVEL_L = NVT_INT_LEVEL_DEF_L;
	NVT_IRQ_LEVEL_H = NVT_INT_LEVEL_DEF_H;

	wbflush_ahb();

	/* set up irq chip & default handler */
	for (i = MIPS_CPU_IRQ_BASE; i < MIPS_CPU_IRQ_BASE + NR_IRQS; i++)
		irq_set_chip_and_handler(i, &mips_cpu_irq_controller, handle_percpu_irq);
}

