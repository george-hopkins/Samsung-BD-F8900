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

#define R_INTC(nr,name) (gp_intr_base[nr]->name)

#define INTMSK_MASK(id, source) \
	do{ R_INTC(id, mask) |= ( 1 << source ); } while(0)

#define INTMSK_UNMASK(id, source) \
	do{ R_INTC(id, mask) &= ~( 1 << source ); } while(0)

#define INTPND_I_CLEAR(id, source) \
	do{ R_INTC(id, i_ispc) = ( 1 << source ); } while(0)
	
#define INTPND_F_CLEAR(id, source) \
	do{ R_INTC(id, f_ispc) = ( 1 << source ); } while(0)


static spinlock_t lockIrq;

/* interrupt source is defined in machine header file */
static SDP_INTR_REG_T * const gp_intr_base[2] = 
			{(SDP_INTR_REG_T*)SDP_INTC_BASE0 ,(SDP_INTR_REG_T*)SDP_INTC_BASE1};

const static SDP_INTR_ATTR_T g_intr_attr[] = SDP_INTERRUPT_RESOURCE;

/* priority resource */
static unsigned long intrDisableFlag[2] = {0xFFFFFFFF, 0xFFFFFFFF} ;
static  struct  { 
	u32  intc_mask[2];
}intrMaskSave[NR_IRQS];

static void sdp_enable_irq(unsigned int irq)
{
	unsigned long flags;

	int nid = irq >> 5;
	int n_bit = irq & 0x1F;

	spin_lock_irqsave(&lockIrq, flags);
	intrDisableFlag[nid] &= ~(1 << n_bit);
	INTMSK_UNMASK(nid, n_bit);
	spin_unlock_irqrestore(&lockIrq, flags);
}

static void sdp_disable_irq(unsigned int irq)
{
	unsigned long flags;

	int nid = irq >> 5;
	int n_bit = irq & 0x1F;

	spin_lock_irqsave(&lockIrq, flags);
	intrDisableFlag[nid] |= (1 << n_bit);
	intrMaskSave[irq].intc_mask[nid] |= (1 << n_bit);
	INTMSK_MASK(nid, n_bit);	
	INTPND_I_CLEAR(nid, n_bit);
	spin_unlock_irqrestore(&lockIrq, flags);
}

static void sdp_ack_irq(unsigned int irq)
{
	int nid = irq >> 5;
	int n_bit = irq & 0x1F;

	INTPND_I_CLEAR(nid, n_bit);	/* offset 0x40 */
}

static void sdp_mask_irq(unsigned int irq)
{
	struct irq_desc *desc = irq_to_desc(irq);
	if (unlikely(desc->status & IRQ_DISABLED)) {
		INTMSK_MASK(irq >>5, irq & 0x1f);	
		INTPND_I_CLEAR(irq >> 5, irq & 0x1f);
		printk (KERN_WARNING "[WARNING] mask_irq:%d called with IRQ_DISABLED.\n", irq);
	} else {
		// save previous interrupt mask
		intrMaskSave[irq].intc_mask[0] = R_INTC(0, mask);				  
		intrMaskSave[irq].intc_mask[1] = R_INTC(1, mask);				  
		// prioriry mask
		R_INTC(0, mask) = g_intr_attr[irq].prioMask | intrDisableFlag[0];   
		R_INTC(1, mask) = g_intr_attr[irq].subPrioMask | intrDisableFlag[1];   
		// clear interrupt pend register
	//	R_INTC(0,pending) &= ~(g_intr_attr[irq].prioMask);  	          
	}
}

static void sdp_unmask_irq(unsigned int irq)
{
	 R_INTC(0, mask) = intrMaskSave[irq].intc_mask[0] | intrDisableFlag[0];
	 R_INTC(1, mask) = intrMaskSave[irq].intc_mask[1] | intrDisableFlag[1];
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

static void __init sdp_intc_init(int nid)
{
	int i, n_irq;
	u32 polarity = 0, level = 0;


	/* initialize interrupt controller */
	R_INTC(nid, control) = INTCON_IRQ_FIQ_DIS;
	R_INTC(nid, mode) = 0x00000000; 	// all irq mode 
	R_INTC(nid, mask) = 0xFFFFFFFF; 	// all interrupt source mask
	R_INTC(nid, i_ispc) = 0xFFFFFFFF; 	// irq status clear
	R_INTC(nid, f_ispc) = 0xFFFFFFFF; 	// fiq status clear
	
	for (i = 0; i < 32; i++) {
		n_irq = i + (nid << 5); // 32 * n_intc
		polarity |= (g_intr_attr[n_irq].polarity & 0x1) << i;
		level    |= (g_intr_attr[n_irq].level & 0x1) << i;
	}

	R_INTC(nid, polarity) = polarity; // Polarity high
	R_INTC(nid, level) = level;  // Level configuration 

	/* register interrupt resource */
	for (i = 0; i < 32; i++) {

		n_irq = i + (nid << 5); // 32 * n_intc
		set_irq_chip(n_irq, &sdp_intctl);


		switch(g_intr_attr[n_irq].level) {
			case (LEVEL_EDGE):
			case (LEVEL_EDGE_EXT):
				set_irq_handler(n_irq, handle_edge_irq);
				break;
			case (LEVEL_LEVEL):
			case (LEVEL_LEVEL_EXT):
			default:
				set_irq_handler(n_irq, handle_level_irq);
				break;
		}
		set_irq_flags(n_irq, IRQF_VALID | IRQF_PROBE);
	}

	R_INTC(nid, control) = INTCON_FIQ_DIS | INTCON_VECTORED;
}

void __init sdp_init_irq(void)
{
	spin_lock_init(&lockIrq);

	sdp_intc_init(0);
	sdp_intc_init(1);
}

