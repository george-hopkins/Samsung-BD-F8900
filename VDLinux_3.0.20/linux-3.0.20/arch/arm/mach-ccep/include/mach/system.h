/*
 * arch/arm/mach-ccep/include/mach/system.h
 *
 * Copyright 2010 Samsung Electronics co.
 * Author: tukho.kim@samsung.com
 */
#ifndef __ASM_ARCH_SYSTEM_H
#define __ASM_ARCH_SYSTEM_H

#include <asm/io.h>
#include <plat/sdp_irq.h>

static inline void arch_idle(void)
{
        /*
         * This should do all the clock switching
         * and wait for interrupt ticks, 
	 *  using linux library 
         */
#ifndef CONFIG_POLL_INTR_PEND
	cpu_do_idle();
#elif defined(CONFIG_ARM_GIC)
	volatile unsigned int * gic_pending_reg1 = (volatile unsigned int *) (VA_GIC_DIST_BASE + 0x200); 
	volatile unsigned int * gic_pending_reg2 = (volatile unsigned int *) (VA_GIC_DIST_BASE + 0x204); 
	volatile unsigned int * gic_pending_reg3 = (volatile unsigned int *) (VA_GIC_DIST_BASE + 0x208); 
	volatile unsigned int * gic_pending_reg4 = (volatile unsigned int *) (VA_GIC_DIST_BASE + 0x20C); 

#ifdef CONFIG_USE_EXT_GIC
	{
		extern unsigned int gic_bank_offset;
		int ncpu = smp_processor_id();
		gic_pending_reg1 += gic_bank_offset * ncpu;
		gic_pending_reg2 += gic_bank_offset * ncpu;
		gic_pending_reg3 += gic_bank_offset * ncpu;
		gic_pending_reg4 += gic_bank_offset * ncpu;
	}
#endif
	do {
		if(gic_pending_reg1 || gic_pending_reg2 || gic_pending_reg3 || gic_pending_reg4) break;
	} while(1);
#else
	SDP_INTR_REG_T * const p_sdp_intc_reg = (SDP_INTR_REG_T*)SDP_INTC_BASE0; 

	do {
		if(p_sdp_intc_reg->pending) break;
	} while(1);

#endif
}

static inline void arch_reset(char mode, const char *cmd)	 //????
//static inline void arch_reset(char mode)
{
        /* use the watchdog timer reset to reset the processor */

        /* (at this point, MMU is shut down, so we use physical addrs) */
        volatile unsigned long *prWTCON = (unsigned long*) (PA_WDT_BASE + 0x00);
        volatile unsigned long *prWTDAT = (unsigned long*) (PA_WDT_BASE + 0x04);
        volatile unsigned long *prWTCNT = (unsigned long*) (PA_WDT_BASE + 0x08);

        /* set the countdown timer to a small value before enableing WDT */
        *prWTDAT = 0x00000100;
        *prWTCNT = 0x00000100;

        /* enable the watchdog timer */
        *prWTCON = 0x00008021;

        /* machine should reboot..... */
        mdelay(5000);
        panic("Watchdog timer reset failed!\n");
        printk(" Jump to address 0 \n");
        cpu_reset(0);
}

/**
  * @fn sdp_get_mem_cfg
  * @brief Get Kernel and System Memory Size for each DDR bus
  * @remarks	
  * @param nType [in]	0:Kernel A Size, 1:System A Size, 2:Kernel B Size, 3:System B Size, 4:Kernel C Size, 5:System C Size
  * @return	memory size(MB). if ocuur error, return -1
  */
int sdp_get_mem_cfg(int nType);

#endif

