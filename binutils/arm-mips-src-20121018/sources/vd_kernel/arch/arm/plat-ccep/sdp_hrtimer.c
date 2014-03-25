/*
 * arch/arm/plat-ccep/sdp_hrtime.c
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

#include "sdp_timer.h"

extern unsigned long SDP_GET_TIMERCLK(char);

static SDP_TIMER_REG_T * const gp_sdp_timer = 
			(SDP_TIMER_REG_T*) SDP_TIMER_BASE;


typedef struct {
	u8  	prescaler;		// 0 ~ 255
	u8	reserve[3];
	u32	div_mode;		// div4 or div16
}SDP_TIMER_SET_T;

static struct sdp_hrt_clksrc_t{
	SDP_TIMER_SET_T 	init_value;
	u32			overflow;
}g_sdp_clksrc = 
	{ .init_value.div_mode = TMCON_MUX16, 
	  .init_value.prescaler =  1,
	  .overflow = 0 };

static struct sdp_hrt_event_t{
	SDP_TIMER_SET_T 	init_value;
	enum clock_event_mode 	mode;
}g_sdp_clkevt = 
	{ .init_value.div_mode = TMCON_MUX04, 
	  .init_value.prescaler =  1,
	  .mode = CLOCK_EVT_MODE_ONESHOT };


// resource 
static spinlock_t sdp_hrtimer_lock;

static void sdp_clkevent_setmode(enum clock_event_mode mode,
				   struct clock_event_device *clk)
{
	R_SYSTMCON = 0;
	g_sdp_clkevt.mode = mode;

	switch(mode){
		case(CLOCK_EVT_MODE_PERIODIC):
			R_SYSTMCON = (g_sdp_clkevt.init_value.div_mode | TMCON_INT_DMA_EN | TMCON_RUN);
//			printk("[%s] periodic mode\n", __FUNCTION__);
			break;
		case(CLOCK_EVT_MODE_ONESHOT):
			R_SYSTMCON = (g_sdp_clkevt.init_value.div_mode | TMCON_INT_DMA_EN);
//			printk("[%s] oneshot mode\n", __FUNCTION__);
			break;
		default: 
			break;
	}
}

static int sdp_clkevent_nextevent(unsigned long evt,
				 struct clock_event_device *unused)
{
	BUG_ON(!evt);

	if(evt > 0xFFFF)
		printk(KERN_DEBUG "HRTIMER: evt overflow 0x%08x\n",(unsigned int)evt);

	R_SYSTMCON = 0;
	R_SYSTMDATA = (g_sdp_clkevt.init_value.prescaler << 16) | (evt & 0xFFFF);  
	R_SYSTMCON = (g_sdp_clkevt.init_value.div_mode | TMCON_INT_DMA_EN | TMCON_RUN);

//	printk("[%s] evt is %u\n", __FUNCTION__, evt);

	return 0;
}

struct clock_event_device sdp_clockevent = {
	.name		= "SDP Timer clock event",
	.shift		= 32,		// nanosecond shift 
	.features	= CLOCK_EVT_FEAT_PERIODIC | CLOCK_EVT_FEAT_ONESHOT,
	.set_mode	= sdp_clkevent_setmode,
	.set_next_event = sdp_clkevent_nextevent,
};

static irqreturn_t sdp_hrtimer_isr(int irq, void *dev_id)
{
	unsigned int regVal = R_TMSTATUS;
	struct clock_event_device *evt = &sdp_clockevent;
	unsigned long flags;

	if(!regVal) return IRQ_NONE;

	local_irq_save(flags);

	if(regVal & CLKSRC_TIMER_BIT){
		R_TMSTATUS = CLKSRC_TIMER_BIT;         // clock source timer intr pend clear
		g_sdp_clksrc.overflow++;	       // clock source timer overflow 
	}
	
	local_irq_restore(flags);

	if(regVal & SYS_TIMER_BIT){
		if (g_sdp_clkevt.mode == CLOCK_EVT_MODE_ONESHOT) R_SYSTMCON = 0;
		R_TMSTATUS = SYS_TIMER_BIT;  // clock event timer intr pend clear
		evt->event_handler(evt);
	}

	return IRQ_HANDLED;
}

static struct irqaction sdp_hrtimer_event = {
	.name = "SDP Hrtimer interrupt handler",
	.flags = IRQF_SHARED | IRQF_DISABLED | IRQF_TIMER,
	.handler = sdp_hrtimer_isr,
};

static cycle_t sdp_clksrc_read (struct clocksource *cs)
{
	unsigned long retVal;
	unsigned long flags;	
	static unsigned long oldVal = 0;	

	spin_lock_irqsave(&sdp_hrtimer_lock, flags);

__sdp_clksrc_read:
	if(unlikely(R_TMSTATUS & CLKSRC_TIMER_BIT))
		retVal = ((g_sdp_clksrc.overflow + 1) << 16) | (0xFFFF - (R_CLKSRC_TMCNT & 0xFFFF));
	else
		retVal = (g_sdp_clksrc.overflow  << 16) | (0xFFFF - (R_CLKSRC_TMCNT & 0xFFFF));

	retVal &= 0xFFFFFFFF;

// correction
	if (unlikely(oldVal > retVal)) {
		printk(KERN_DEBUG "clksrc read is overflow value old: 0x%x, current: 0x%x\n", 
				(unsigned int)oldVal, (unsigned int)retVal);
		if((oldVal - retVal) < 0x10000)  // check clock source overlap
			goto __sdp_clksrc_read; // re-read clock source
	}
// correction end
	oldVal = retVal;		// to correct clock source to avoid time warp

	spin_unlock_irqrestore(&sdp_hrtimer_lock, flags);

	return retVal;
}

struct clocksource sdp_clocksource = {
	.name 		= "SDP Timer clock source",
	.rating 	= 200,
	.read 		= sdp_clksrc_read,
	.mask 		= CLOCKSOURCE_MASK(32),
	.shift 		= 21,		
	.flags		= CLOCK_SOURCE_IS_CONTINUOUS,
};

/* Initialize Timer */
static void __init sdp_hrtimer_init(void)
{
	unsigned long timer_clock;
	unsigned long clksrc_clock;
	unsigned long clkevt_clock;
	unsigned long init_clkevt;

	printk(KERN_INFO "Samsung DTV Linux System HRTimer initialize\n");
	
	R_TMDMASEL = 0;

// Timer reset & stop
	R_TIMER(0, control) = 0;
	R_TIMER(1, control) = 0;
	R_TIMER(2, control) = 0;

// get Timer source clock 
	timer_clock = SDP_GET_TIMERCLK(TIMER_CLOCK);  //MUX04
	printk(KERN_INFO "HRTIMER: source clock is %u Hz, SYS tick: %d\n", 
			(unsigned int)timer_clock, SYS_TICK);

/* clock source 1 tick -> 5us */
#define CLKSRC_TICK_HZ	 200000
	clksrc_clock = timer_clock >> 4;	// MUX16
	g_sdp_clksrc.init_value.div_mode = TMCON_MUX16;
	g_sdp_clksrc.init_value.prescaler =
		(clksrc_clock / CLKSRC_TICK_HZ) - 1;
	clksrc_clock = clksrc_clock / (g_sdp_clksrc.init_value.prescaler + 1);

/* clock event 1 tick -> 0.25us */
#define CLKEVT_TICK_HZ	4000000
	clkevt_clock = timer_clock >> 2;		// MUX04
	g_sdp_clkevt.init_value.div_mode = TMCON_MUX04;
	g_sdp_clkevt.init_value.prescaler =
		(clkevt_clock / CLKEVT_TICK_HZ) - 1;
	clkevt_clock = (clkevt_clock) / (g_sdp_clkevt.init_value.prescaler + 1);

// init lock 
	spin_lock_init(&sdp_hrtimer_lock);

// register timer interrupt service routine
	setup_irq(SYS_TIMER_IRQ, &sdp_hrtimer_event);

	R_CLKSRC_TMCON = g_sdp_clksrc.init_value.div_mode;
	R_CLKSRC_TMDATA = (g_sdp_clksrc.init_value.prescaler << 16) | (0xFFFF);  


// clock source init -> clock source 
	sdp_clocksource.mult = clocksource_hz2mult(clksrc_clock >> 2, sdp_clocksource.shift);
	clocksource_register(&sdp_clocksource);

	R_CLKSRC_TMCON |= TMCON_INT_DMA_EN;
	R_CLKSRC_TMCON |= TMCON_RUN;

// timer event init
	sdp_clockevent.mult = 
			div_sc(clkevt_clock, NSEC_PER_SEC, sdp_clockevent.shift);
	sdp_clockevent.max_delta_ns =
			clockevent_delta2ns(0xffff, &sdp_clockevent);
	sdp_clockevent.min_delta_ns =
			clockevent_delta2ns(0xf, &sdp_clockevent);

#if 1
	init_clkevt = clkevt_clock / SYS_TICK;

	if(init_clkevt > 0xFFFF) {
		printk("[%s]Please check HRTIMER counter\n", __FILE__);
		R_SYSTMCON = 0;
		R_SYSTMDATA = 0;
		return;
	}

//	R_SYSTMCON = g_sdp_clkevt.init_value.div_mode;
	R_SYSTMDATA = (g_sdp_clkevt.init_value.prescaler << 16) | init_clkevt;  
#endif

	sdp_clockevent.cpumask = cpumask_of(0);
	clockevents_register_device(&sdp_clockevent);
#if 0
	printk(KERN_INFO "HRTIMER: clksrc is %u Hz, clkevt is %u Hz\n", 
			(unsigned int)clksrc_clock, (unsigned int)clkevt_clock);
	printk(KERN_INFO "HRTIMER: clksrc mult is %u, clkevt mult is %u \n", 
			(unsigned int)sdp_clocksource.mult, (unsigned int)sdp_clockevent.mult);
	printk(KERN_INFO "HRTIMER: clkevt min is %u, clkevt max is %u \n", 
			(unsigned int)sdp_clockevent.min_delta_ns, (unsigned int)sdp_clockevent.max_delta_ns);
	printk(KERN_INFO "HRTIMER: clksrc presca is %u, clkevt presca is %u \n", 
			(unsigned int)g_sdp_clksrc.init_value.prescaler, 
			(unsigned int)g_sdp_clkevt.init_value.prescaler );
#endif
}

struct sys_timer sdp_timer = {
	.init		= sdp_hrtimer_init,
};

