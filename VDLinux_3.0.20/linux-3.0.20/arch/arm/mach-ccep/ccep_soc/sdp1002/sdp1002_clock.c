/*
 * linux/arch/arm/mach-sdp/sdp1002_clock.c
 *
 * Copyright (C) 2010 Samsung Electronics.co
 * Author : tukho.kim@samsung.com 	07/30/2010
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
static DECLARE_MUTEX(clocks_sem);

static int sdp1002_init_clock(void);
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

static struct clk shdclk = {
	.source 	= NULL,
	.name	 	= "SHDCLK",
	.rate		= 0,
	.ctrlbit	= -1,
};

static struct clk ad1clk = {
	.source 	= NULL,
	.name	 	= "AD1CLK",
	.rate		= 0,
	.ctrlbit	= -1,
};

static struct clk busclk = {
	.source 	= NULL,
	.name	 	= "BUSCLK",
	.rate		= 0,
	.ctrlbit	= -1,
};

static struct clk hclk = {
	.source 	= NULL,
	.name		= "HCLK",
	.rate		= 0,
	.ctrlbit 	= -1,
};

static struct clk pclk = {
	.source 	= NULL,
	.name		= "PCLK",
	.rate		= 0,
	.ctrlbit 	= -1,
};

static struct clk *init_clocks[]={
	&fclk,
	&dclk,
	&shdclk,
	&ad1clk,
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

void 	clk_put(struct clk *clk)
{
	module_put(clk->owner); 
}

int 	clk_enable(struct clk *clk)
{

	return 0;
}

void 	clk_disable(struct clk *clk)
{

}

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


unsigned long sdp1002_get_clock (char mode)
{
// 20080524 by tukho.kim 
// apply HRTIMER 
	if(!fclk.rate) sdp1002_init_clock();

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

static unsigned int sdp1002_calc_cpu_pms(unsigned int pms)
{
	unsigned int ret;

	ret = (INPUT_FREQ >> (GET_PLL_S(pms)-1)) / GET_PLL_P(pms);
	ret *= GET_PLL_M(pms); 
	return ret;
}

static unsigned int sdp1002_calc_bus_pms(unsigned int pms)
{
	unsigned int ret;
	ret = (INPUT_FREQ >> (GET_PLL_S(pms)-1)) / GET_PLL_P(pms);
	ret *= GET_PLL_M(pms); 
	return ret;
}

static unsigned int sdp1002_calc_ddr_pmsk(unsigned int pms, unsigned int k)
{
	unsigned int ret;
	unsigned long long tmp;
	tmp = (INPUT_FREQ >> GET_PLL_S(pms)) / GET_PLL_P(pms);
	ret =  (tmp * GET_PLL_M(pms)) + ((tmp * k) >> 10);
	return ret;
}

static unsigned int sdp1002_calc_ad1_pmsk(unsigned int pms, unsigned int k)
{
	unsigned int r, p, m, s;
	p = (pms >> 24) & 0x3f;
	m = (pms >> 8) & 0x1ff;
	s = pms & 0x7;

	r = (k * 27000)/65536 + (m * 27000);
	r *= 1000;
	r /= (p * 1<<s);

	return r;
}

static int sdp1002_init_clock(void)
{
	unsigned int pms_core = R_PMU_CPU_PMS_CON;
	unsigned int pms_ddr = R_PMU_DDR_PMS_CON;
	unsigned int k_ddr = R_PMU_DDR_K_CON;
	unsigned int pms_shd = R_PMU_SHD_PMS_CON;
	unsigned int pms_ad1 = R_PMU_AD1_PMS_CON;
	unsigned int k_ad1 = (R_PMU_AD1_K_CON >> 16);
	unsigned int clksel = R_PMU_CLKSEL;
	unsigned int sysclksel = R_PMU_SYSCLKSEL;
	unsigned int mux;

// 20080524 by tukho.kim 
// apply HRTIMER 
	if(fclk.rate) return -1;

	/* base clock sources from PLL logic */
	fclk.rate = sdp1002_calc_cpu_pms(pms_core);
	dclk.rate = sdp1002_calc_ddr_pmsk(pms_ddr, (k_ddr >> 20) & 0xfff);
	ad1clk.rate = sdp1002_calc_ad1_pmsk(pms_ad1, k_ad1);
	shdclk.rate = sdp1002_calc_bus_pms(pms_shd);

	/* BUS clock
	 * 0=AD1PLL, 1=SHDPLL/2, 2=TENPLL/2, 3=CPUPLL/3, 4=SHDPLL/3, 5=TENPLL/3 */
	switch ((sysclksel >> 12) & 0x7) {
	case 0:
		busclk.rate = ad1clk.rate;
		busclk.source = &ad1clk;
		break;
	case 1:
		busclk.source = &shdclk;
		busclk.rate = shdclk.rate >> 1;
		break;
	default:
		printk (KERN_ERR "Unsupported bus clock source. Check your bootloader.\n");
		break;
	}

	/* AHB clock
	 * 0=AD1PLL/2, 1=SHDPLL/4, 2=TENPLL/4, 3=CPUPLL/6, 4=SHDPLL/6, 5=TENPLL/6 */
	switch ((sysclksel >> 16) & 0x7) {
	case 0:
		hclk.rate = ad1clk.rate >> 1;
		hclk.source = &ad1clk;
		break;
	case 1:
		hclk.rate = shdclk.rate >> 2;
		hclk.source = &ad1clk;
		break;
	default:
		printk (KERN_ERR "Unsupported AHB clock source. Check your bootloader.\n");
		break;
	}

	/* APB clock */
	if (clksel & 0x8) {
		pclk.rate = ad1clk.rate >> 3;
		pclk.source = &ad1clk;
	} else {
		pclk.rate = fclk.rate >> 4;
		pclk.source = &fclk;
	}
	
	return 0;
}

int __init clk_init(void)
{
	struct clk *clk;
	int i;

	printk(KERN_INFO "sdp1002 clock control (c) 2010 Samsung Electronics\n");
	sdp1002_init_clock();
	//	return 0;

	for(i= 0; i< ARRAY_SIZE(init_clocks);i++) {
		clk = init_clocks[i];
		clk_register(clk);
		printk (KERN_INFO "SDP1002 %s Clock: %d.%03dMhz\n",
				clk->name,
				(unsigned int)clk->rate / MHZ,
				(unsigned int)clk->rate % MHZ / 1000);
	}

	return 0;
}

arch_initcall(clk_init);

#if 0
EXPORT_SYMBOL(clk_init);
EXPORT_SYMBOL(clk_get_rate);
EXPORT_SYMBOL(clk_unuse);
EXPORT_SYMBOL(clk_use);
EXPORT_SYMBOL(clk_disable);
EXPORT_SYMBOL(clk_enable);
EXPORT_SYMBOL(clk_put);
EXPORT_SYMBOL(clk_get);
EXPORT_SYMBOL(clk_round_rate);
EXPORT_SYMBOL(clk_set_rate);
EXPORT_SYMBOL(clk_register);
EXPORT_SYMBOL(sdp1002_get_clock); 
#endif

