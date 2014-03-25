/*
 * arch/arm/plat-ccep/sdp_irq.c
 *
 * Copyright (C) 2010 Samsung Electronics.co
 * Author : tukho.kim@samsung.com
 *
 */
/*
 * CONFIG_LOCKDEP : On lockdep we don't want to enable hardirqs in hardirq context
 *
 *
 */

#include <linux/init.h>
#include <linux/device.h>
#include <linux/sysdev.h>
#include <linux/interrupt.h>

#include <asm/system.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/mach/arch.h>
#include <asm/mach/irq.h>

#include <asm/delay.h>

#include <plat/sdp_irq.h>

#define SDP_INTC_NAME		"sdp interrupt controller"

#define R_INTC(nr,name) (gp_intr_base##nr->name)

#define INTMSK_MASK(source) \
	do{ R_INTC(0, mask) |= ( 1 << source ); } while(0)

#define INTMSK_UNMASK(source) \
	do{ R_INTC(0, mask) &= ~( 1 << source ); } while(0)

#define INTPND_I_CLEAR(source) \
	do{ R_INTC(0, i_ispc) = ( 1 << source ); } while(0)
	
#define INTPND_F_CLEAR(source) \
	do{ R_INTC(0, f_ispc) = ( 1 << source ); } while(0)


static spinlock_t lockIrq;

/* interrupt source is defined in machine header file */
static SDP_INTR_REG_T * const gp_intr_base0 = (SDP_INTR_REG_T*)SDP_INTC_BASE0;
const static SDP_INTR_ATTR_T g_intr_attr[] = SDP_INTERRUPT_RESOURCE;

/* priority resource */
static unsigned long intrDisableFlag = 0xFFFFFFFF;
static unsigned long intrMaskSave[NR_IRQS];

static void sdp_enable_irq(unsigned int irq)
{
	unsigned long flags;

	spin_lock_irqsave(&lockIrq, flags);
	intrDisableFlag &= ~(1 << irq);
	INTMSK_UNMASK(irq);
	spin_unlock_irqrestore(&lockIrq, flags);
}

static void sdp_disable_irq(unsigned int irq)
{
	unsigned long flags;

	spin_lock_irqsave(&lockIrq, flags);
	intrDisableFlag |= (1 << irq);
	intrMaskSave[irq] |= (1 << irq);
	INTMSK_MASK(irq);	
	INTPND_I_CLEAR(irq);
	spin_unlock_irqrestore(&lockIrq, flags);
}

static void sdp_ack_irq(unsigned int irq)
{
	INTPND_I_CLEAR(irq);	/* offset 0x40 */
}

static void sdp_mask_irq(unsigned int irq)
{
	struct irq_desc *desc = irq_to_desc(irq);
	if (unlikely(desc->status & IRQ_DISABLED)) {
		INTMSK_MASK(irq);	
		INTPND_I_CLEAR(irq);
		printk (KERN_WARNING "[WARNING] mask_irq:%d called with IRQ_DISABLED.\n", irq);
	} else {
		// save previous interrupt mask
		intrMaskSave[irq] = R_INTC(0, mask);				  
		// prioriry mask
//		R_INTC(0,mask) = g_intr_attr[irq].prioMask | intrDisableFlag;   // interrupt nesting 
		R_INTC(0,mask) = 0xFFFFFFFF;
		// clear interrupt pend register
//		R_INTC(0,pending) &= ~(g_intr_attr[irq].prioMask);				// interrupt nesting
	}
}

static void sdp_unmask_irq(unsigned int irq)
{
	R_INTC(0,mask) = intrMaskSave[irq] | intrDisableFlag;
}

static int sdp_set_type(unsigned int irq, unsigned int flow_type)
{
	if(!(g_intr_attr[irq].level & LEVEL_ATTR_EXT)){
		printk(KERN_INFO"[irq] %d fixed interrupt type\n", irq);
		return 0;
	}
/* TODO */


	return 0;
}


static struct irq_chip sdp_intctl = {
	.name   = SDP_INTC_NAME,		
	.enable = sdp_enable_irq,
	.disable = sdp_disable_irq,
	.ack	= sdp_ack_irq,
	.mask	= sdp_mask_irq,
	.unmask	= sdp_unmask_irq,
	.set_type = sdp_set_type,
};

void __init sdp_init_irq(void)
{
	int i;
	unsigned int polarity = 0, level = 0;

	spin_lock_init(&lockIrq);

	/* initialize interrupt controller */
	R_INTC(0, control) = INTCON_IRQ_FIQ_DIS;
	R_INTC(0, mode) = 0x00000000; 	// all irq mode 
	R_INTC(0, mask) = 0xFFFFFFFF; 	// all interrupt source mask
	R_INTC(0, i_ispc) = 0xFFFFFFFF; 	// irq status clear
	R_INTC(0, f_ispc) = 0xFFFFFFFF; 	// fiq status clear
	
	for (i = 0; i < NR_IRQS; i++) {
		polarity |= (g_intr_attr[i].polarity & 0x1) << i;
		level    |= (g_intr_attr[i].level & 0x1) << i;
	}
	R_INTC(0, polarity) = polarity; // Polarity high
	R_INTC(0, level) = level;  // Level configuration 

	/* register interrupt resource */
	for (i = 0; i < NR_IRQS; i++) {
		set_irq_chip(i, &sdp_intctl);
		switch(g_intr_attr[i].level) {
			case (LEVEL_EDGE):
			case (LEVEL_EDGE_EXT):
				set_irq_handler(i, handle_edge_irq);
				break;
			case (LEVEL_LEVEL):
			case (LEVEL_LEVEL_EXT):
			default:
				set_irq_handler(i, handle_level_irq);
				break;
		}
		set_irq_flags(i, IRQF_VALID | IRQF_PROBE);
	}

	R_INTC(0, control) = INTCON_FIQ_DIS | INTCON_VECTORED;
}

