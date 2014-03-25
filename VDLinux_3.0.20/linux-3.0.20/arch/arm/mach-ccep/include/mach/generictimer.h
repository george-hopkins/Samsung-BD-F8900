#ifndef __ASM_GENERICTIMER_H
#define __ASM_GENERICTIMER_H

struct clock_event_device;

//extern void __iomem *twd_base;

int gt_timer_ack(void);

#endif
