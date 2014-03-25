/*
 * arch/arm/mach-ccep/include/mach/entry-macro.S
 *
 * Samsung DTV Soc interrupt Assembler
 * Copyright (C) 2012 seungjun.heo@samsung.com
 *
 */

#include <mach/hardware.h>
#include <plat/sdp_irq.h>

#ifdef CONFIG_ARM_GIC

#include <asm/hardware/gic.h>

#ifndef HAVE_GET_IRQNR_PREAMBLE
	.macro	get_irqnr_preamble, base, tmp
#ifdef	CONFIG_USE_EXT_GIC
	mov	\tmp, #0

	mrc	p15, 0, \base, c0, c0, 5
	and	\base, \base, #3
	cmp	\base, #0
	beq	1f

	ldr	\tmp, =gic_bank_offset
	ldr	\tmp, [\tmp]
	cmp	\base, #1
	beq	1f

	cmp	\base, #2
	addeq	\tmp, \tmp, \tmp
	addne	\tmp, \tmp, \tmp, LSL #1
#endif

1:	ldr	\base, =gic_cpu_base_addr
	ldr	\base, [\base]
#ifdef	CONFIG_USE_EXT_GIC
	add	\base, \base, \tmp
#endif
	.endm

/*
 * The interrupt numbering scheme is defined in the
 * interrupt controller spec.  To wit:
 *
 * Interrupts 0-15 are IPI
 * 16-28 are reserved
 * 29-31 are local.  We allow 30 to be used for the watchdog.
 * 32-1020 are global
 * 1021-1022 are reserved
 * 1023 is "spurious" (no interrupt)
 *
 * For now, we ignore all local interrupts so only return an interrupt if it's
 * between 30 and 1020.  The test_for_ipi routine below will pick up on IPIs.
 *
 * A simple read from the controller will tell us the number of the highest
 * priority enabled interrupt.  We then just need to check whether it is in the
 * valid range for an IRQ (30-1020 inclusive).
 */

	.macro  get_irqnr_and_base, irqnr, irqstat, base, tmp

	ldr     \irqstat, [\base, #GIC_CPU_INTACK]
	/* bits 12-10 = src CPU, 9-0 = int # */

	ldr	\tmp, =1021
	bic     \irqnr, \irqstat, #0x1c00
	cmp     \irqnr, #30
	cmpcc	\irqnr, \irqnr
	cmpne	\irqnr, \tmp
	cmpcs	\irqnr, \irqnr
	.endm


/* We assume that irqstat (the raw value of the IRQ acknowledge
 * register) is preserved from the macro above.
 * If there is an IPI, we immediately signal end of interrupt on the
 * controller, since this requires the original irqstat value which
 * we won't easily be able to recreate later.
 */

	.macro test_for_ipi, irqnr, irqstat, base, tmp
	bic	\irqnr, \irqstat, #0x1c00
	cmp	\irqnr, #16
	strcc	\irqstat, [\base, #GIC_CPU_EOI]
	cmpcs	\irqnr, \irqnr
	.endm

/* As above, this assumes that irqstat and base are preserved.. */

	.macro test_for_ltirq, irqnr, irqstat, base, tmp
	bic	\irqnr, \irqstat, #0x1c00
	mov 	\tmp, #0
	cmp	\irqnr, #29
	cmpne	\irqnr, #30
	moveq	\tmp, #1
	streq	\irqstat, [\base, #GIC_CPU_EOI]
	cmp	\tmp, #0
	.endm

#endif

#else
.macro get_irqnr_preamble, base, tmp
.endm

.macro get_irqnr_and_base, irqnr, irqstat, base, tmp
/* Try to semi vectored interrupt.
 * if using semi vectored interrupt, must set semi-vectored
 * interrupt in INTCON register
 * tukho.kim@samsung.com 2010/06  */

	mov     \base, #SDP_INTC_BASE0&0xff000000 @ IO Base add virtual
	add     \base, \base, #SDP_INTC_BASE0&0x00ff0000
	add     \base, \base, #SDP_INTC_BASE0&0x0000ff00

	@ read rI_ISPR register check, 
	@ if Pend regiser is 0, just out
	ldr	\irqstat, [\base, #I_ISPR]      @ get interrupts bits
	cmp	\irqstat, #0

#if defined(CONFIG_SDP_INTC64)
	beq	1001f				@ 2nd interrupt controller
#else
	moveq	\irqnr, #IRQ_TIMER
#endif
	ldrne	\irqstat, [\base, #0x78]
	movne	\irqnr, \irqstat, LSR #0x2

#if defined(CONFIG_SDP_INTC64)
	bne	1002f
1001:
#if (SDP_INTC_BASE1 > SDP_INTC_BASE0)
	add     \base, \base, #(SDP_INTC_BASE1 - SDP_INTC_BASE0)
#else
	add     \base, \base, #(SDP_INTC_BASE0 - SDP_INTC_BASE1)
#endif
	ldr	\irqstat, [\base, #I_ISPR]      @ get interrupts bits
	cmp	\irqstat, #0
	
	moveq	\irqnr, #IRQ_TIMER

	ldrne	\irqstat, [\base, #0x78]
	movne	\irqnr, \irqstat, LSR #0x2
	addne 	\irqnr, \irqnr, #32
1002:
#endif
.endm

#endif
.macro  disable_fiq
.endm

.macro arch_ret_to_user, tmp1, tmp2
.endm

