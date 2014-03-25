#ifndef NVTUSB_REGS
#define NVTUSB_REGS

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/writeback.h>
#define USB0_EHCI_BASE								0xbc140000
#define USB1_EHCI_BASE								0xbc148000
#define USB0_APB								0xbd170000
#define USB1_APB								0xbd180000

#define OTGC_INT_BSRPDN 						  1	
#define OTGC_INT_ASRPDET						  1<<4
#define OTGC_INT_AVBUSERR						  1<<5
#define OTGC_INT_RLCHG							  1<<8
#define OTGC_INT_IDCHG							  1<<9
#define OTGC_INT_OVC							  1<<10
#define OTGC_INT_BPLGRMV						  1<<11
#define OTGC_INT_APLGRMV						  1<<12

#define OTGC_INT_A_TYPE 						  (OTGC_INT_ASRPDET|OTGC_INT_AVBUSERR|OTGC_INT_OVC|OTGC_INT_RLCHG|OTGC_INT_IDCHG|OTGC_INT_BPLGRMV|OTGC_INT_APLGRMV)
#define OTGC_INT_B_TYPE 						  (OTGC_INT_BSRPDN|OTGC_INT_AVBUSERR|OTGC_INT_OVC|OTGC_INT_RLCHG|OTGC_INT_IDCHG)

#define writel(v,c)	*(volatile unsigned int *)(c) = ((unsigned int)v)
#define readl(c)    *(volatile unsigned int *)(c)

#define clear(add,wValue)    writel(~(wValue)&readl(add),add)
#define set(add,wValue)      writel(wValue|readl(add),add)
extern raw_spinlock_t clock_gen_lock;
extern void resetUSBClock(unsigned long addr0, unsigned long addr1, unsigned long addr2, unsigned long bit0, unsigned long bit1, unsigned long bit2);


#endif

