/*
 * arch/arm/mach-ccep/include/mach/preempt.h
 *
 * Copyright (C) 2004 ARM Limited
 * Copyright (C) 2010 Samsung Electronics co. 
 * Author : tukho.kim@samsung.com
 *
 */

#ifndef _ASM_ARCH_PREEMT_H
#define _ASM_ARCH_PREEMT_H

static inline unsigned long clock_diff(unsigned long start, unsigned long stop)
{
	return (start - stop);
}

extern unsigned long sdp_timer_read(void);

/*
 * timer 1 runs @ 1Mhz, 1 tick = 1 microsecond
 * and is configured as a count down timer.
 */


#define TICKS_PER_USEC			2
#define ARCH_PREDEFINES_TICKS_PER_USEC
#define readclock()		(sdp_timer_read())
#define clock_to_usecs(x)	((x) * TICKS_PER_USEC)
#define INTERRUPTS_ENABLED(x)	(!(x & PSR_I_BIT))

#endif

