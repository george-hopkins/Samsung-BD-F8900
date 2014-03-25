/*
 * Setup the right wbflush routine for nvt
 */

#include <linux/module.h>
#include <asm/system.h>
#include <asm/wbflush.h>
#include <asm/page.h>

void wbflush_axi(void)
{
	volatile unsigned long tmp;
	volatile unsigned long val;
	__sync();
	*(volatile unsigned long*)UNCAC_ADDR(&tmp) = 0;
	val = *(volatile unsigned long*)UNCAC_ADDR(&tmp);
}

EXPORT_SYMBOL(wbflush_axi);
