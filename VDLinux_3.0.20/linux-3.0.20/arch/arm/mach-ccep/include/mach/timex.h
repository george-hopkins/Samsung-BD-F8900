/*
 *  arch/arm/mach-ccep/include/mach/timex.h
 *
 *  timex specifications of Samsung CCEP
 *
 *  Copyright (C) 2003 ARM Limited
 *  Copyright 2010 Samsung Electronics co
 *  Author: tukho.kim@samsung.com
 *
 */

#ifndef __ASM_ARCH_SDP_TIMEX_H
#define __ASM_ARCH_SDP_TIMEX_H

#include <mach/platform.h>
#define CLOCK_TICK_RATE		(5000000)

#ifndef CONFIG_HIGH_RES_TIMERS
extern unsigned long sdp_timer_read(void);
#define mach_read_cycles() sdp_timer_read()
#endif

#endif /* __ASM_ARCH_SDP_TIMEX_H */
