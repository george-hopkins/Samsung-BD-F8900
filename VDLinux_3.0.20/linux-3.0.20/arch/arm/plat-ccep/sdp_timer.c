/*
 * linux/arch/arm/plat-ccep/sdp_timer.c
 *
 * Copyright (C) 2010 Samsung Electronics.co
 * Author : tukho.kim@samsung.com
 *
 */
#include <linux/version.h>
#include <linux/init.h>
#include <linux/dma-mapping.h>
#include <linux/sysdev.h>
#include <linux/interrupt.h>
#include <linux/irq.h>

#include <asm/mach/time.h>
#include <mach/platform.h>

#include "sdp_timer.h"


extern void clk_init(void);
extern unsigned long SDP_GET_TIMERCLK(char mode);


struct cal_usec {
	unsigned long usec;
	unsigned long nsec;
};


static unsigned int init_val_sys_tmcnt = 0;
static struct cal_usec usec_per_clock = {0, 0};

// Don't rename this structure 
static SDP_TIMER_REG_T * const gp_sdp_timer = 
		(SDP_TIMER_REG_T*) SDP_TIMER_BASE;


unsigned long sdp_timer_read(void)
{
	return R_SYSTMCNT;
}

#ifndef CONFIG_GENERIC_TIME
static unsigned long sdp_gettimeoffset (void)
{
	unsigned long usec;
	unsigned long nsec;
	unsigned int n_past;
	unsigned int cur_count = R_SYSTMCNT;

// check timer interrupt 
	if (R_TMSTATUS & SYS_TIMER_BIT)
		return 1000000 / HZ;

// check timer reloading
	if (cur_count > init_val_sys_tmcnt)
		return 0;
 
// calcurate usec delay
//	pastCNT = init_val_sys_tmcnt - R_SYSTMCNT;  // this code is wrong
	n_past = init_val_sys_tmcnt - cur_count;

	usec = (unsigned long)usec_per_clock.usec * n_past;
	nsec = (unsigned long)usec_per_clock.nsec * n_past;
	nsec += 500;
	usec += nsec / 1000;

	return usec;
}
#else
#endif

// interrupt service routine
static irqreturn_t
sdp_timer_interrupt(int irq, void *dev_id)
{

	if (!(R_TMSTATUS & SYS_TIMER_BIT))
		return IRQ_NONE;

	R_TMSTATUS = SYS_TIMER_BIT;  // clear

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27))
	write_seqlock(&xtime_lock);
	timer_tick();
	write_sequnlock(&xtime_lock);
#else
	timer_tick();
#endif
	return IRQ_HANDLED;
}

// interrupt resource 
static struct irqaction sdp_timer_irq = {
	.name	= "SDP Timer tick",
	.flags	= IRQF_SHARED | IRQF_DISABLED,
	.handler = sdp_timer_interrupt
};

// Calcuration about usec per 1 tick for another parts
static void __init sdp_get_time_tick(unsigned long* max_value)
{
	unsigned long timer_source;
	unsigned long nsec = SYS_TICK;

	if(init_val_sys_tmcnt == 0){

	 	timer_source = SDP_GET_TIMERCLK(TIMER_CLOCK) >> 2;		
		if(!timer_source){
			clk_init();
	 		timer_source = SDP_GET_TIMERCLK(TIMER_CLOCK) >> 2;		
		}

		/*timer scale is 1ns -> 1000000us * 1000 */
		if ((nsec <= 0) && (nsec > 1000)) {
			nsec = 100;
		}

		init_val_sys_tmcnt = (timer_source/nsec/(SYS_TIMER_PRESCALER+1));
		nsec = 1000000000 / nsec / init_val_sys_tmcnt;

		init_val_sys_tmcnt--;
		usec_per_clock.nsec = nsec % 1000;
		usec_per_clock.usec = nsec / 1000;

	}

	printk("Samsung SDP Timer Clock: %d.%03d usec per tick\n"
			"\tLoad value=%d\n", 
			(int)usec_per_clock.usec, (int)usec_per_clock.nsec, init_val_sys_tmcnt);	

	*max_value = init_val_sys_tmcnt;

	return;
}

// Initailize the timer
static void __init sdp_timer_init(void)
{
	unsigned long val_data;

	printk(KERN_INFO"Samsung DTV Linux System timer initialize\n");

	R_TMDMASEL = 0;

	R_TIMER(0, control) = 0;
	R_TIMER(1, control) = 0;

	R_SYSTMCON = TMCON_MUX04;
	
	sdp_get_time_tick(&val_data);

	R_SYSTMDATA = val_data | (SYS_TIMER_PRESCALER << 16);

	setup_irq(SYS_TIMER_IRQ, &sdp_timer_irq);

	R_SYSTMCON |= TMCON_INT_DMA_EN;
	R_SYSTMCON |= TMCON_RUN;
}

struct sys_timer sdp_timer = {
	.init	= sdp_timer_init,
#ifndef CONFIG_GENERIC_TIME
	.offset	= sdp_gettimeoffset,
#endif
};
