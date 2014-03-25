/*
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 * Routines for generic manipulation of the interrupts found on the MIPS
 * Malta board.
 * The interrupt controller is located in the South Bridge a PIIX4 device
 * with two internal 82C95 interrupt controllers.
 */


#include <linux/init.h>
#include <linux/irq.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/kernel_stat.h>
#include <linux/kernel.h>
#include <linux/random.h>

#include <asm/traps.h>
#include <asm/irq_cpu.h>
#include <asm/irq_regs.h>
//#include <asm/mips-boards/malta.h>
#include <asm/mips-boards/maltaint.h>
#include <asm/mips-boards/piix4.h>
//#include <asm/gt64120.h>
#include <asm/mips-boards/generic.h>
#include <asm/gic.h>
#include <asm/gcmpregs.h>

#include <asm/wbflush.h>

#include <nvt-int.h>

asmlinkage void plat_irq_dispatch(void);

int gcmp_present = -1;
int gic_present;

//static DEFINE_SPINLOCK(mips_irq_lock);

static void nvt_irq_dispatch(void)
{
	unsigned long id;
	id = NVT_IRQ_ID;

	if(id & 0x100)
	{
		do_IRQ(id & 0x3f);
	}
	else
	{
		printk(KERN_ALERT "Invalid interrupt ID\n");
	}

	wbflush_ahb();
}


#if 0
static inline int mips_pcibios_iack(void)
{
	int irq;
	u32 dummy;

	/*
	 * Determine highest priority pending interrupt by performing
	 * a PCI Interrupt Acknowledge cycle.
	 */
	switch (mips_revision_sconid) {
	case MIPS_REVISION_SCON_SOCIT:
	case MIPS_REVISION_SCON_ROCIT:
	case MIPS_REVISION_SCON_SOCITSC:
	case MIPS_REVISION_SCON_SOCITSCP:
		MSC_READ(MSC01_PCI_IACK, irq);
		irq &= 0xff;
		break;
	case MIPS_REVISION_SCON_GT64120:
		irq = GT_READ(GT_PCI0_IACK_OFS);
		irq &= 0xff;
		break;
	case MIPS_REVISION_SCON_BONITO:
		/* The following will generate a PCI IACK cycle on the
		 * Bonito controller. It's a little bit kludgy, but it
		 * was the easiest way to implement it in hardware at
		 * the given time.
		 */
		BONITO_PCIMAP_CFG = 0x20000;

		/* Flush Bonito register block */
		dummy = BONITO_PCIMAP_CFG;
		iob();    /* sync */

		irq = readl((u32 *)_pcictrl_bonito_pcicfg);
		iob();    /* sync */
		irq &= 0xff;
		BONITO_PCIMAP_CFG = 0;
		break;
	default:
		printk(KERN_WARNING "Unknown system controller.\n");
		return -1;
	}
	return irq;
}

static inline int get_int(void)
{
	unsigned long flags;
	int irq;
	spin_lock_irqsave(&mips_irq_lock, flags);

	irq = mips_pcibios_iack();

	/*
	 * The only way we can decide if an interrupt is spurious
	 * is by checking the 8259 registers.  This needs a spinlock
	 * on an SMP system,  so leave it up to the generic code...
	 */

	spin_unlock_irqrestore(&mips_irq_lock, flags);

	return irq;
}


static inline int clz(unsigned long x)
{
	__asm__(
	"	.set	push					\n"
	"	.set	mips32					\n"
	"	clz	%0, %1					\n"
	"	.set	pop					\n"
	: "=r" (x)
	: "r" (x));

	return x;
}

/*
 * Version of ffs that only looks at bits 12..15.
 */
static inline unsigned int irq_ffs(unsigned int pending)
{
#if defined(CONFIG_CPU_MIPS32) || defined(CONFIG_CPU_MIPS64)
	return -clz(pending) + 31 - CAUSEB_IP;
#else
	unsigned int a0 = 7;
	unsigned int t0;

	t0 = pending & 0xf000;
	t0 = t0 < 1;
	t0 = t0 << 2;
	a0 = a0 - t0;
	pending = pending << t0;

	t0 = pending & 0xc000;
	t0 = t0 < 1;
	t0 = t0 << 1;
	a0 = a0 - t0;
	pending = pending << t0;

	t0 = pending & 0x8000;
	t0 = t0 < 1;
	/* t0 = t0 << 2; */
	a0 = a0 - t0;
	/* pending = pending << t0; */

	return a0;
#endif
}
#endif

/*
 * IRQs on the Malta board look basically (barring software IRQs which we
 * don't use at all and all external interrupt sources are combined together
 * on hardware interrupt 0 (MIPS IRQ 2)) like:
 *
 *	MIPS IRQ	Source
 *      --------        ------
 *             0	Software (ignored)
 *             1        Software (ignored)
 *             2        Combined hardware interrupt (hw0)
 *             3        Hardware (ignored)
 *             4        Hardware (ignored)
 *             5        Hardware (ignored)
 *             6        Hardware (ignored)
 *             7        R4k timer (what we use)
 *
 * We handle the IRQ according to _our_ priority which is:
 *
 * Highest ----     R4k Timer
 * Lowest  ----     Combined hardware interrupt
 *
 * then we just return, if multiple IRQs are pending then we will just take
 * another exception, big deal.
 */

asmlinkage void plat_irq_dispatch(void)
{
}

#ifdef CONFIG_MIPS_MT_SMP


#define GIC_MIPS_CPU_IPI_RESCHED_IRQ	3
#define GIC_MIPS_CPU_IPI_CALL_IRQ	4

#define MIPS_CPU_IPI_RESCHED_IRQ 0	/* SW int 0 for resched */
#define C_RESCHED C_SW0
#define MIPS_CPU_IPI_CALL_IRQ 1		/* SW int 1 for resched */
#define C_CALL C_SW1

#endif /* CONFIG_MIPS_MT_SMP */


void __init arch_init_irq(void)
{
	int i;

	mips_cpu_irq_init();
	for(i = 1; i < NR_IRQS; i++) {
		set_vi_handler (i, nvt_irq_dispatch);
	}
}

#if 0
void malta_be_init(void)
{
	if (gcmp_present) {
		/* Could change CM error mask register */
	}
}


int malta_be_handler(struct pt_regs *regs, int is_fixup)
{
	/* This duplicates the handling in do_be which seems wrong */
	return 0;
}
#endif

