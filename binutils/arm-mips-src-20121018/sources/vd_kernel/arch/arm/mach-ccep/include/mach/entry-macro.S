/*
 * arch/arm/mach-ccep/include/mach/entry-macro.S
 *
 * Samsung DTV Soc interrupt Assembler
 * Copyright (C) 2010 tukho.kim@samsung.com
 *
 */

#include <mach/hardware.h>
#include <asm/hardware/gic.h>
#include <plat/sdp_irq.h>

#ifdef CONFIG_ARM_GIC

#include <asm/hardware/entry-macro-gic.S>

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

