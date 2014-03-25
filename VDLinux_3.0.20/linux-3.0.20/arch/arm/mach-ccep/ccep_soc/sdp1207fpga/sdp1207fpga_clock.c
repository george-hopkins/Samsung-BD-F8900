/*
 * linux/arch/arm/mach-sdp/ccep_soc/sdp1207fpga/clock.c
 *
 * Copyright (C) 2012 Samsung Electronics.co
 * Author : tukho.kim@samsung.com 	
 *
 */
/*
 * 2012/04/09, tukho.kim, created
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/device.h>
#include <linux/semaphore.h>

#include <mach/hardware.h>
#include <mach/platform.h>

#if !defined(MHZ)
#define MHZ	1000000
#endif

struct module;

struct clk {
	struct list_head	list;
	struct module*		owner;
	struct clk*		source;
	const char*		name;

	atomic_t		used;
	unsigned long		rate;
	int			ctrlbit;
};


static LIST_HEAD(clocks);
static DEFINE_SEMAPHORE(clocks_sem);

static int sdp1207fpga_init_clock(void);
/* Linux kernel -2.6 clock API */

static struct clk fclk = {
	.source 	= NULL,
	.name		= "FCLK",
	.rate		= 0,
	.ctrlbit 	= -1,
};

static struct clk dclk = {
	.source 	= NULL,
	.name		= "DCLK",
	.rate		= 0,
	.ctrlbit 	= -1,
};

static struct clk busclk = {
	.source 	= NULL,
	.name	 	= "BUSCLK",
	.rate		= 0,
	.ctrlbit	= -1,
};

static struct clk hclk = {
	.source 	= &busclk,
	.name		= "HCLK",
	.rate		= 0,
	.ctrlbit 	= -1,
};

static struct clk pclk = {
	.source 	= &fclk,
	.name		= "PCLK",
	.rate		= 0,
	.ctrlbit 	= -1,
};

static struct clk *init_clocks[]={
	&fclk,
	&dclk,
	&busclk,
	&hclk,
	&pclk,
};

struct clk*
clk_get(struct device *dev, const char *id)
{
        struct clk *p, *pclk = ERR_PTR(-ENOENT);

        down(&clocks_sem);
        list_for_each_entry(p, &clocks, list) {
                if (strcmp(id, p->name) == 0 && try_module_get(p->owner)) {
                        pclk = p;
                        break;
                }
        }
        up(&clocks_sem);

        return pclk;
}
EXPORT_SYMBOL(clk_get);

void 	clk_put(struct clk *clk)
{
	module_put(clk->owner); 
}
EXPORT_SYMBOL(clk_put);

int 	clk_enable(struct clk *clk)
{
	return 0;
}
EXPORT_SYMBOL(clk_enable);

void 	clk_disable(struct clk *clk)
{

}
EXPORT_SYMBOL(clk_disable);

int 	clk_use(struct clk *clk)
{
	atomic_inc(&clk->used);
	return 0;
}

void 	clk_unuse(struct clk *clk)
{
	if(clk->used.counter > 0) atomic_dec(&clk->used); 
}

unsigned long clk_get_rate(struct clk *clk)
{
	return (clk->rate) ? clk->rate : clk->source->rate;
}
EXPORT_SYMBOL(clk_get_rate);

long 	clk_round_rate(struct clk *clk, unsigned long rate)
{
	return rate;
}

int 	clk_set_rate(struct clk *clk, unsigned long rate)
{
	printk(KERN_WARNING "Can't support to chagne rate\n");
	return 0;
}

int clk_register(struct clk* clk)
{
	clk->owner = THIS_MODULE;
	atomic_set(&clk->used, 0);

	down(&clocks_sem);
	list_add(&clk->list, &clocks);
	up(&clocks_sem);

	return 0;
}


unsigned long sdp1207fpga_get_clock (char mode)
{
// 20080524 by tukho.kim 
// apply HRTIMER 
	if(!fclk.rate) sdp1207fpga_init_clock();

	switch(mode){
		case (REQ_DCLK):
			return dclk.rate;
			break;

		case (REQ_BUSCLK):
			return busclk.rate;
			break;

		case (REQ_HCLK):
			return hclk.rate;
			break;

		case (REQ_PCLK):
			return pclk.rate;
			break;

		case (REQ_FCLK):
		default:
			return fclk.rate;
			break;
	}
}

static int __init sdp1207fpga_init_clock(void)
{
	if(fclk.rate) return -1;

	fclk.rate = 0;
	dclk.rate = 0;
//	busclk.rate = CONFIG_FPGA_BUSCLKSRC / 4;
	busclk.rate = 96875000 / 8;
	hclk.rate = busclk.rate;
	pclk.rate = busclk.rate;

	return 0;
}

int __init clk_init(void)
{
	struct clk *clk;
	int i;

	printk(KERN_INFO "sdp1207fpga clock control (c) 2011 Samsung Electronics\n");
	sdp1207fpga_init_clock();

	for(i= 0; i< ARRAY_SIZE(init_clocks);i++) {
		clk = init_clocks[i];
		clk_register(clk);
		printk (KERN_INFO "sdp1207fpga %s Clock: %d.%03dMhz\n",
				clk->name,
				(unsigned int)clk->rate / MHZ,
				(unsigned int)clk->rate % MHZ / 1000);
	}

	return 0;
}

arch_initcall(clk_init);

