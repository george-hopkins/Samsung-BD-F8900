/*
 * arch/arm/plat-ccep/sdp_hrtimer64.c
 *
 * Copyright (C) 2010 Samsung Electronics.co
 * Author : tukho.kim@samsung.com
 *
 */
#include <linux/init.h>
#include <linux/dma-mapping.h>
#include <linux/sysdev.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/spinlock.h>

/* hrtimer headr file */
#include <linux/clocksource.h>
#include <linux/clockchips.h>
/* hrtimer headr file end */

#include <asm/mach/time.h>
#include <mach/platform.h>

#include "sdp_timer64.h"

extern unsigned long SDP_GET_TIMERCLK(char);
unsigned long long sched_clock(void);

static SDP_TIMER_REG_T * const gp_sdp_timer = 
			(SDP_TIMER_REG_T*) SDP_TIMER_BASE;

#ifdef CONFIG_HAVE_ARM_TWD
extern void __iomem *twd_base;
#endif

enum clock_event_mode g_clkevt_mode = CLOCK_EVT_MODE_PERIODIC;

// resource 
static spinlock_t sdp_hrtimer_lock;

static void sdp_clkevent_setmode(enum clock_event_mode mode,
				   struct clock_event_device *clk)
{
	g_clkevt_mode = mode;

	switch(mode){
		case(CLOCK_EVT_MODE_PERIODIC):
			R_SYSTMCON = (TMCON_MUX04 | TMCON_INT_DMA_EN | TMCON_RUN);
//			printk("[%s] periodic mode\n", __FUNCTION__);
			break;
		case(CLOCK_EVT_MODE_ONESHOT):
			R_SYSTMCON = (TMCON_MUX04 | TMCON_INT_DMA_EN);
//			printk("[%s] oneshot mode\n", __FUNCTION__);
			break;
        	case (CLOCK_EVT_MODE_UNUSED):
        	case (CLOCK_EVT_MODE_SHUTDOWN):
		default: 
			R_SYSTMCON = 0;
			break;
	}
}

static int sdp_clkevent_nextevent(unsigned long evt,
				 struct clock_event_device *unused)
{
	BUG_ON(!evt);

	R_SYSTMCON = 0;
	R_SYSTMDATA64L = evt;  
	R_SYSTMCON = (TMCON_MUX04 | TMCON_INT_DMA_EN | TMCON_RUN);

//	printk("[%s] evt is %d\n", __FUNCTION__,(u32)evt);

	return 0;
}

struct clock_event_device sdp_clockevent = {
	.name		= "SDP Timer clock event",
	.rating		= 200,
	.shift		= 32,		// nanosecond shift 
	.features	= CLOCK_EVT_FEAT_PERIODIC | CLOCK_EVT_FEAT_ONESHOT,
	.set_mode	= sdp_clkevent_setmode,
	.set_next_event = sdp_clkevent_nextevent,
};

static irqreturn_t sdp_hrtimer_isr(int irq, void *dev_id)
{
	unsigned int regVal = R_TMSTATUS;
	struct clock_event_device *evt = &sdp_clockevent;

	if(!regVal) return IRQ_NONE;

	if(regVal & SYS_TIMER_BIT){
		if (g_clkevt_mode == CLOCK_EVT_MODE_ONESHOT) 
				R_SYSTMCON = TMCON_MUX04;
		R_TMSTATUS = SYS_TIMER_BIT;  // clock event timer intr pend clear
		evt->event_handler(evt);
	}

	return IRQ_HANDLED;
}

static struct irqaction sdp_hrtimer_event = {
	.name = "SDP Hrtimer interrupt handler",
#ifdef CONFIG_ARM_GIC
	.flags = IRQF_SHARED | IRQF_DISABLED | IRQF_TIMER | IRQF_TRIGGER_RISING,
#else
	.flags = IRQF_SHARED | IRQF_DISABLED | IRQF_TIMER,
#endif
	.handler = sdp_hrtimer_isr,
};

static inline u64 sdp_clksrc_read_raw(void)
{
	unsigned long long val;
	u32 t_low, t_high1, t_high2;
	
	t_high1 = R_CLKSRC_TMCNT64H;
	t_low = R_CLKSRC_TMCNT64L;
	t_high2 = R_CLKSRC_TMCNT64H;
	if (unlikely(t_high1!= t_high2))
		t_low = R_CLKSRC_TMCNT64L;
	
	val = (unsigned long long)(((u64)t_high2<< 32) | t_low);

	return val;
}

static cycle_t sdp_clksrc_read (struct clocksource *cs)
{
#if 0
	cycle_t retval;
	unsigned long flags;	

	u32 timer64_h, timer64_l;	


	spin_lock_irqsave(&sdp_hrtimer_lock, flags);

	timer64_h = R_CLKSRC_TMCNT64H;
	timer64_l = R_CLKSRC_TMCNT64L;

	if(timer64_h != R_CLKSRC_TMCNT64H)
		timer64_l = R_CLKSRC_TMCNT64L;

	retval = timer64_h;
	retval = (retval << 32) + timer64_l;
	retval = timer64_l;

	spin_unlock_irqrestore(&sdp_hrtimer_lock, flags);

	return retval;
#else 
	return (cycle_t)sdp_clksrc_read_raw();
#endif
}

struct clocksource sdp_clocksource = {
	.name 		= "SDP Timer clock source",
	.rating 	= 200,
	.read 		= sdp_clksrc_read,
	.mask 		= CLOCKSOURCE_MASK(64),
	.mult		= 0,
	.shift 		= 9,	
	.flags		= CLOCK_SOURCE_IS_CONTINUOUS,
};

static void __init sdp_hrtimer_clksrc_init(unsigned long timer_clock)
{
	unsigned long clksrc_clock;

	clksrc_clock = timer_clock >> 2;	// MUX4
	R_CLKSRC_TMDATA = 9 << 16;		// divide 10
	clksrc_clock = clksrc_clock / 10;

// clock source init -> clock source 
	sdp_clocksource.mult = clocksource_khz2mult(clksrc_clock / 1000, sdp_clocksource.shift);
	clocksource_register(&sdp_clocksource);

	R_CLKSRC_TMDATA64L = 0x0;
	R_CLKSRC_TMDATA64H = 0x0;
	R_CLKSRC_TMCON = TMCON_64BIT_UP | TMCON_MUX04 | TMCON_RUN;
//	R_CLKSRC_TMCON = TMCON_MUX04 | TMCON_RUN;

	pr_info ("sdp_hrtimer64: clock source freq=%ldHZ, (mult/shift=%d/%d)\n",
			clksrc_clock,
			sdp_clocksource.mult, sdp_clocksource.shift);

}

unsigned long long sched_clock(void)
{
	unsigned long long val;
	
	if(unlikely(!sdp_clocksource.mult))
		return 0;
	
	val = (unsigned long long)sdp_clksrc_read_raw();
	val = (val * sdp_clocksource.mult) >> sdp_clocksource.shift;

	return val;
}

/* Initialize Timer */
static void __init sdp_hrtimer_init(void)
{
	unsigned long timer_clock;
	unsigned long clkevt_clock;
//	unsigned long init_clkevt;

#ifdef CONFIG_HAVE_ARM_TWD
	twd_base = VA_TWD_BASE;
#endif

	printk(KERN_INFO "Samsung DTV Linux System HRTimer initialize\n");
	
	R_TMDMASEL = 0;

// Timer reset & stop
	R_TIMER(0, control) = 0;
	R_TIMER(1, control) = 0;

// get Timer source clock 
	timer_clock = SDP_GET_TIMERCLK(TIMER_CLOCK);  //MUX04
	printk(KERN_INFO "HRTIMER: source clock is %u Hz, SYS tick: %d\n", 
			(unsigned int)timer_clock, SYS_TICK);

// init lock 
	spin_lock_init(&sdp_hrtimer_lock);

	sdp_hrtimer_clksrc_init(timer_clock);

	clkevt_clock = timer_clock >> 2;		// MUX04
	R_SYSTMDATA = 1 << 16;
	clkevt_clock = (clkevt_clock) >> 1; 	// divide 2
	R_SYSTMDATA64L = clkevt_clock / SYS_TICK;

// register timer interrupt service routine
	setup_irq(SYS_TIMER_IRQ, &sdp_hrtimer_event);

// timer event init
	sdp_clockevent.mult = 
			div_sc(clkevt_clock, NSEC_PER_SEC, (int) sdp_clockevent.shift);
	sdp_clockevent.max_delta_ns =
			clockevent_delta2ns(0xFFFFFFFF, &sdp_clockevent);
	sdp_clockevent.min_delta_ns =
			clockevent_delta2ns(0xf, &sdp_clockevent);

	sdp_clockevent.cpumask = cpumask_of(0);
	clockevents_register_device(&sdp_clockevent);
}

struct sys_timer sdp_timer = {
	.init		= sdp_hrtimer_init,
};

