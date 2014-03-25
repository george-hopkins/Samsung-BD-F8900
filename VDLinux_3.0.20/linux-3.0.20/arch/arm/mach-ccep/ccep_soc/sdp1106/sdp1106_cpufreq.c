/* linux/arch/arm/mach-ccep/sdp1106_cpufreq.c
 *
 * Copyright (c) 2012 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com
 *
 * SDP1106 - CPU frequency scaling support
 *
 */
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/regulator/consumer.h>
#include <linux/cpufreq.h>
#include <linux/cpu.h>
#include <linux/delay.h>

#include <asm/cacheflush.h>

#include <mach/hardware.h>
#include <mach/platform.h>
#include <plat/cpufreq.h>

#define CPUFREQ_LEVEL_END	L9

static int max_support_idx;
static int min_support_idx = (CPUFREQ_LEVEL_END - 1);

/* PMIC control gpio */
#define VA_GPIO8_CONT	*((volatile unsigned int*)(VA_PADCTRL_BASE + 0x160)) /* use [12:0] */
#define VA_GPIO8_WRITE	*((volatile unsigned int*)(VA_PADCTRL_BASE + 0x164)) /* use [3:0] */

static spinlock_t cpufreq_lock;

static struct clk *cpu_clk;

static struct cpufreq_frequency_table sdp1106_freq_table[] = {
//	{O0, 1500*1000},
	{L0, 1000*1000},
	{L1, 900*1000},
	{L2, 800*1000},
	{L3, 700*1000},
	{L4, 600*1000},
	{L5, 500*1000},
	{L6, 400*1000},
	{L7, 300*1000},
	{L8, 250*1000},	
	{0, CPUFREQ_TABLE_END},
};

static unsigned int clkdiv_cpu[CPUFREQ_LEVEL_END] = {
	/* PMU_PMS value */
//	0x0040FA00, /* ARM O0 : 1500 MHz */
	0x00307D00, /* ARM L0 : 1000 MHz */
	0x00409600, /* ARM L1 : 900 MHz */
	0x00306400, /* ARM L2 : 800 MHz */
	0x0030AF01, /* ARM L3 : 700 MHz */
	0x0040C801, /* ARM L4 : 600 MHz */
	0x00307D01, /* ARM L5 : 500 MHz */
	0x00306401, /* ARM L6 : 400 MHz */
	0x0040C802, /* ARM L7 : 300 MHz */
	0x00307D02, /* ARM L8 : 250 MHz */
};

#if 0
struct cpufreq_voltage_table {
	unsigned int	index;		/* level */
	unsigned int	arm_volt;	/* mV */
	unsigned int	gpio_val;	/* gpio value for PMIC control */
};
#endif

struct gic_chip_data {
	u32 saved_spi_enable[DIV_ROUND_UP(1020, 32)];
	u32 saved_spi_conf[DIV_ROUND_UP(1020, 16)];
	u32 saved_spi_pri[DIV_ROUND_UP(1020, 4)];
	u32 saved_spi_target[DIV_ROUND_UP(1020, 4)];
	u32 saved_ppi_enable[DIV_ROUND_UP(32, 32)];
	u32 saved_ppi_conf[DIV_ROUND_UP(32, 16)];
//	u32 saved_ppi_pri;
};

static struct gic_chip_data gic_data;
static unsigned int twd_regs[9];

/* voltage table */
/* 0x0: 1.196v, 0x4: 1.132v, 0x5: 1.116v, 0x6: 1.100v, */
/* 0x7: 1.084v, 0x8: 1.068v, 0x9: 1.052v, 0xc: 1.004v, 0xf: 0.956v */
/* uV scale */
static unsigned int sdp1106_volt_table[CPUFREQ_LEVEL_END] = {
//1164000,
1132000, /* L1 */
1132000,
1132000,
1132000,
1132000,
1132000,
1132000,
1132000,
1084000, /* L8 */
};

static inline void cpu_enter_lowpower(void)
{
	unsigned int v;

	flush_cache_all();
	asm volatile(
	"	mcr	p15, 0, %1, c7, c5, 0\n"
	"	mcr	p15, 0, %1, c7, c10, 4\n"
	/*
	 * Turn off coherency
	 */
	"	mrc	p15, 0, %0, c1, c0, 1\n"
	"	bic	%0, %0, #0x20\n"
	"	mcr	p15, 0, %0, c1, c0, 1\n"
	"	mrc	p15, 0, %0, c1, c0, 0\n"
	"	bic	%0, %0, #0x04\n"
	"	mcr	p15, 0, %0, c1, c0, 0\n"
	  : "=&r" (v)
	  : "r" (0)
	  : "cc");
}

static inline void cpu_leave_lowpower(void)
{
	unsigned int v;

	asm volatile(	"mrc	p15, 0, %0, c1, c0, 0\n"
	"	orr	%0, %0, #0x04\n"
	"	mcr	p15, 0, %0, c1, c0, 0\n"
	"	mrc	p15, 0, %0, c1, c0, 1\n"
	"	orr	%0, %0, #0x20\n"
	"	mcr	p15, 0, %0, c1, c0, 1\n"
	  : "=&r" (v)
	  :
	  : "cc");
}

static void gic_save_info(void)
{
#if 0	
	unsigned int gic_irqs;
	u32 cpu_base = VA_GIC_CPU_BASE, dist_base = VA_GIC_DIST_BASE;
	int i;
	u32* ptr;

	gic_irqs = readl(dist_base + GIC_DIST_CTR) & 0x1f;
	gic_irqs = (gic_irqs + 1) * 32;
	if (gic_irqs > 1020)
		gic_irqs = 1020;
	
	/* save dist */
	for (i = 0; i < DIV_ROUND_UP(gic_irqs, 16); i++)
		gic_data.saved_spi_conf[i] = 
		readl_relaxed(dist_base + GIC_DIST_CONFIG + i * 4);

	for (i = 0; i < DIV_ROUND_UP(gic_irqs, 4); i++) 
		gic_data.saved_spi_pri[i] = 
		readl_relaxed(dist_base + GIC_DIST_PRI + i * 4); 

	for (i = 0; i < DIV_ROUND_UP(gic_irqs, 4); i++) 
		gic_data.saved_spi_target[i] = 
		readl_relaxed(dist_base + GIC_DIST_TARGET + i * 4); 

	for (i = 0; i < DIV_ROUND_UP(gic_irqs, 32); i++) 
		gic_data.saved_spi_enable[i] = 
		readl_relaxed(dist_base + GIC_DIST_ENABLE_SET + i * 4); 

	writel_relaxed(0, dist_base + GIC_DIST_CTRL); 

	/* save cpu interface */
	ptr = gic_data.saved_ppi_enable;
	for (i = 0; i < DIV_ROUND_UP(32, 32); i++)
		ptr[i] = readl_relaxed(dist_base + GIC_DIST_ENABLE_SET + i * 4);

	ptr = gic_data.saved_ppi_conf;
	for (i = 0; i < DIV_ROUND_UP(32, 16); i++)
		ptr[i] = readl_relaxed(dist_base + GIC_DIST_CONFIG + i * 4);
	
	writel_relaxed(0, cpu_base + GIC_CPU_CTRL); 
#endif	
}

static void gic_restore_info(void)
{
#if 0
	unsigned int gic_irqs; 
	unsigned int i; 
	u32* ptr;
	u32 cpu_base = VA_GIC_CPU_BASE, dist_base = VA_GIC_DIST_BASE;

	gic_irqs = readl(dist_base + GIC_DIST_CTR) & 0x1f;
	gic_irqs = (gic_irqs + 1) * 32;
	if (gic_irqs > 1020)
		gic_irqs = 1020;

	/* restore dist */
	writel_relaxed(0, dist_base + GIC_DIST_CTRL); 

	for (i = 0; i < DIV_ROUND_UP(gic_irqs, 16); i++) 
	writel_relaxed(gic_data.saved_spi_conf[i], 
	dist_base + GIC_DIST_CONFIG + i * 4); 

	for (i = 0; i < DIV_ROUND_UP(gic_irqs, 4); i++) 
	writel_relaxed(gic_data.saved_spi_pri[i], 
	dist_base + GIC_DIST_PRI + i * 4); 

	for (i = 0; i < DIV_ROUND_UP(gic_irqs, 4); i++) 
	writel_relaxed(gic_data.saved_spi_target[i], 
	dist_base + GIC_DIST_TARGET + i * 4); 

	for (i = 0; i < DIV_ROUND_UP(gic_irqs, 32); i++) 
	writel_relaxed(gic_data.saved_spi_enable[i], 
	dist_base + GIC_DIST_ENABLE_SET + i * 4); 

	writel_relaxed(1, dist_base + GIC_DIST_CTRL); 	

	/* restore cpu interface */
	ptr = gic_data.saved_ppi_enable;
	for (i = 0; i < DIV_ROUND_UP(32, 32); i++)
		writel_relaxed(ptr[i], dist_base + GIC_DIST_ENABLE_SET + i * 4);

	ptr = gic_data.saved_ppi_conf;
	for (i = 0; i < DIV_ROUND_UP(32, 16); i++)
		writel_relaxed(ptr[i], dist_base + GIC_DIST_CONFIG + i * 4);

	for (i = 0; i < DIV_ROUND_UP(32, 4); i++)
		writel_relaxed(0xa0a0a0a0, dist_base + GIC_DIST_PRI + i * 4);

	writel_relaxed(0xf0, cpu_base + GIC_CPU_PRIMASK);
	writel_relaxed(1, cpu_base + GIC_CPU_CTRL);
#endif	
}

static void timer_save_regs(void)
{
	unsigned int base = VA_TWD_BASE;
	int i;
	
	for (i = 0; i < 4; i++)
		twd_regs[i] = readl_relaxed(base + i * 4);
	
	for (i = 0; i < 5; i++)
		twd_regs[4+i] = readl_relaxed(base + 0x20 + i * 4);	
}

static void timer_restore_regs(void)
{
	unsigned int base = VA_TWD_BASE;
	int i;
	unsigned int reg;

	for (i = 0; i < 4; i++)
		writel_relaxed(twd_regs[i], base + i * 4);
	
	for (i = 0; i < 5; i++)
		writel_relaxed(twd_regs[4+i], base + 0x20 + i * 4);

	/* check timer counter */
	reg = readl_relaxed(base + 0x04);
	if (reg == 0)
		writel_relaxed(10, base + 0x04);

	/* timer enable */
	reg = readl_relaxed(base + 0x08);
	reg |= 0x1;
	writel_relaxed(reg, base + 0x08);	
}

unsigned int tsd_int;
unsigned int aio_int;
unsigned int ae_int[2];
unsigned int disp_int;
unsigned int ga2d_int;
unsigned int ga3d_int[10];
unsigned int avd_int[2];
unsigned int usb_int[2];
unsigned int emac_int;
unsigned int uart_int[4];
unsigned int hdmirx_int[6];
unsigned int dsp_int;
unsigned int osdp_int;
unsigned int ext_int;
unsigned int mmc_int;
unsigned int cipher_int;

static void mask_all_blocks(void)
{
	unsigned int base = VA_IO_BASE0;
	
	// tsd
	//0x30110004 = 0;
	tsd_int = readl(base + 0x110004);
	writel(0, base + 0x110004);
	
	// aio
	//0x30640780 = 0bit;
	aio_int = readl(base + 0x640780);
	writel(0, base + 0x640780);
	
	// ae
	//0x30630100 = 0bit;
	//0x30630104 = 0bit;
	ae_int[0] = readl(base + 0x630100);
	ae_int[1] = readl(base + 0x630104);
	writel(0, base + 0x630100);
	writel(0, base + 0x630104);
	
	// mpeg0, mpeg1

	// disp
	//0x30910284 = 0bit;
	disp_int = readl(base + 0x910284);
	writel(0, base + 0x910284);
	
	// ga2d
	//0x30220074 = 1; // 반대
	ga2d_int = readl(base + 0x220074);
	writel(1, base + 0x220074);
	
	// ga3d 0~9
	//0x30509028 = 0;
	//0x3050401c = 0;
	//0x3050b028 = 0;
	//0x3050501c = 0;
	//0x3050d028 = 0;
	//0x3050601c = 0;
	//0x3050f028 = 0;
	//0x3050701c = 0;
	//0x3050002c = 0;
	//0x3050301c = 0;
	ga3d_int[0] = readl(base + 0x509028);
	ga3d_int[1] = readl(base + 0x50401c);
	ga3d_int[2] = readl(base + 0x50b028);
	ga3d_int[3] = readl(base + 0x50501c);
	ga3d_int[4] = readl(base + 0x50d028);
	ga3d_int[5] = readl(base + 0x50601c);
	ga3d_int[6] = readl(base + 0x50f028);
	ga3d_int[7] = readl(base + 0x50701c);
	ga3d_int[8] = readl(base + 0x50002c);
	ga3d_int[9] = readl(base + 0x50301c);
	writel(0, base + 0x509028);
	writel(0, base + 0x50401c);
	writel(0, base + 0x50b028);
	writel(0, base + 0x50501c);
	writel(0, base + 0x50d028);
	writel(0, base + 0x50601c);
	writel(0, base + 0x50f028);
	writel(0, base + 0x50701c);
	writel(0, base + 0x50002c);
	writel(0, base + 0x50301c);

	// pcie

	// mfd // unavailable

	// avd
	//0x30330930 = 0;
	//0x30330940 = 0;
	avd_int[0] = readl(base + 0x330930);
	avd_int[1] = readl(base + 0x330940);
	writel(0, base + 0x330930);
	
	
	// froad

	// cssys

	// cpudma

	// usb
	//0x30070018 = 0;
	//0x30070014 = 0bit 1;
	usb_int[0] = readl(base + 0x70018);
	writel(0, base + 0x70018);
	writel(1, base + 0x70014);
	usb_int[1] = readl(base + 0x80018);
	writel(0, base + 0x80018);
	writel(1, base + 0x80014);

	// emac
	//0x3005101c = 0;
	emac_int = readl(base + 0x5101c);
	writel(0, base + 0x5101c);

	// se
	// 0x303f0014 = 0;
	
	// cipher
	// 0x303f0410 = 0;
	cipher_int = readl(base + 0x3f0410);
	writel(0, base + 0x3f0410);
	
	// tsmux

	// uart
	//0x30090ac4 = 0;
	uart_int[0] = readl(base + 0x90a04);
	uart_int[1] = readl(base + 0x90a44);
	uart_int[2] = readl(base + 0x90a84);
	uart_int[3] = readl(base + 0x90ac4);
	writel(0, base + 0x90a04);
	writel(0, base + 0x90a44);
	writel(0, base + 0x90a84);
	writel(0, base + 0x90ac4);

	// hdmirx
	//0x30092104 = 0;
	//0x30092108 = 0;
	//0x3009210c = 0;
	//0x30092110 = 0;
	//0x30092114 = 0;
	//0x30092118 = 0;
	
	
	// timer

	// i2c

	// sci

	// dsp
	//0x30cc0004 = 0;
	dsp_int = readl(base + 0xcc0004);
	writel(0, base + 0xcc0004);
	
	// mmc
	// 0x30000000 = 4bit 0;
	mmc_int = readl(base + 0x00);
	writel(mmc_int&0xffffffef, base + 0x00);

	// osdp
	//0x3030004c = 1; // 반대
	osdp_int = readl(base + 0x30004c);
	writel(1, base + 0x30004c);
	
	// irr

	// rtc

	// irb

	// smc dma

	// ams dma

	// ext 0 ~ 7
	// vbus valid
	//0x30090c74 = 0x3ff;
	ext_int = readl(base + 0x90c74);
	writel(0x3ff, base + 0x90c74);
}

static void unmask_all_blocks(void)
{
	unsigned int base = VA_IO_BASE0;
	
	// tsd
	writel(tsd_int, base + 0x110004);
	
	// aio
	writel(aio_int, base + 0x640780);
	
	// ae
	writel(ae_int[0], base + 0x630100);
	writel(ae_int[1], base + 0x630104);
	
	// mpeg0, mpeg1

	// disp
	writel(disp_int, base + 0x910284);
	
	// ga2d
	writel(ga2d_int, base + 0x220074);
	
	// ga3d 0~9
	writel(ga3d_int[0], base + 0x509028);
	writel(ga3d_int[1], base + 0x50401c);
	writel(ga3d_int[2], base + 0x50b028);
	writel(ga3d_int[3], base + 0x50501c);
	writel(ga3d_int[4], base + 0x50d028);
	writel(ga3d_int[5], base + 0x50601c);
	writel(ga3d_int[6], base + 0x50f028);
	writel(ga3d_int[7], base + 0x50701c);
	writel(ga3d_int[8], base + 0x50002c);
	writel(ga3d_int[9], base + 0x50301c);

	// pcie

	// mfd // unavailable

	// avd
	//0x30330930 = 0;
	//0x30330940 = 0;

	// froad

	// cssys

	// cpudma

	// usb
	writel(usb_int[0], base + 0x70018);
	writel(usb_int[1], base + 0x80018);

	// emac
	writel(emac_int, base + 0x5101c);

	// se
	// 0x303f0014 = 0;

	// cipher
	writel(cipher_int, base + 0x3f0410);
	
	// tsmux

	// uart
	writel(uart_int[0], base + 0x90a04);
	writel(uart_int[1], base + 0x90a44);
	writel(uart_int[2], base + 0x90a84);
	writel(uart_int[3], base + 0x90ac4);

	// hdmirx
	//0x30092104 = 0;
	//0x30092108 = 0;
	//0x3009210c = 0;
	//0x30092110 = 0;
	//0x30092114 = 0;
	//0x30092118 = 0;
	
	// timer

	// i2c

	// sci

	// dsp
	writel(dsp_int, base + 0xcc0004);
	
	// mmc
	writel(mmc_int, base + 0x00);

	// osdp
	writel(osdp_int, base + 0x30004c);
	
	// irr

	// rtc

	// irb

	// smc dma

	// ams dma

	// ext 0 ~ 7
	// vbus valid
	writel(ext_int, base + 0x90c74);
}


extern void twd_set_mult(unsigned int target_freq);

//#define USE_SE_INT
//#define USE_ARMPERI_RESET
//#define USE_MASK_ALLBLOCK
static void sdp1106_set_clock(unsigned int div_index)
{
	unsigned int reg;

	/* disable PPI */
//	writel(0xffff0000, VA_GIC_DIST_BASE + GIC_DIST_ENABLE_CLEAR);
//	writel(0x0000ffff, VA_GIC_DIST_BASE + GIC_DIST_ENABLE_SET);
	/* mask all interrupt */
	/* disable all interrupts */
//	for (i = 1; i < 3; i++) 
//		writel(0xffffffff, VA_GIC_DIST_BASE + GIC_DIST_ENABLE_CLEAR + (4*i));

	/* save timer */
	timer_save_regs();

	/* save gic */
	gic_save_info();

#ifdef USE_MASK_ALLBLOCK
	/* mask all blocks */
	mask_all_blocks();
#endif

#ifdef USE_ARMPERI_RESET
	/* reset enable ARM peri */
	reg = readl(VA_IO_BASE0 + (0x30b70434 - PA_IO_BASE0));
	reg = reg & ~(1<<9);
	writel(reg, VA_IO_BASE0 + (0x30b70434 - PA_IO_BASE0));
#endif	
	
#ifdef USE_SE_INT	
	/* enable legacy interrupt controller's SE */
	reg = readl(VA_INTC_BASE + 0x0C);
	reg &= ~(1<<17); // only SE(17)
	writel(reg, VA_INTC_BASE + 0x0C);

	/* request freq change to SE */
	// check SE stop ('0' is stop)
	while(readl(VA_IO_BASE0 + 0x003E840C)) {
		printk(">>SE is not ready\n");
	}
	// send pll value to SE
	reg = clkdiv_cpu[div_index];
	writel(reg, VA_IO_BASE0 + 0x003E8410);
	// SE start
	writel(0x1, VA_IO_BASE0 + 0x003E840C);
#endif	
//	R_PMU_CPU_PMS_CON = clkdiv_cpu[div_index]; // ARM sets pms value directily.

	/*
	 * here's the WFI
	 */
	asm(".word	0xe320f003\n"
	    :
	    :
	    : "memory", "cc");

//	printk(">out WFI\n");

#ifdef USE_SE_INT
	/* disable legacy SE interrupt */
	writel(0xffffffff, VA_INTC_BASE + 0x0C);

	/* SE pending clear */
	writel(1<<17, VA_INTC_BASE + 0x04);
	writel(1<<17, VA_INTC_BASE + 0x40);
#endif

#ifdef USE_ARMPERI_RESET
	/* reset disable ARM peri */
//	reg = readl(VA_IO_BASE0 + (0x30b70434 - PA_IO_BASE0));
//	reg = reg | (1<<9);
//	writel(reg, VA_IO_BASE0 + (0x30b70434 - PA_IO_BASE0));

//	mdelay(1);
	
	/* reset enable ARM peri */
//	reg = readl(VA_IO_BASE0 + (0x30b70434 - PA_IO_BASE0));
//	reg = reg & ~(1<<9);
//	writel(reg, VA_IO_BASE0 + (0x30b70434 - PA_IO_BASE0));
	
//	mdelay(1);
	
	/* reset disable ARM peri */
	reg = readl(VA_IO_BASE0 + (0x30b70434 - PA_IO_BASE0));
	reg = reg | (1<<9);
	writel(reg, VA_IO_BASE0 + (0x30b70434 - PA_IO_BASE0));
#endif	

	/* restore masked interrupt (enable) */
//	for (i = 0; i < 3; i++) {
//		writel(dist_int[i], VA_GIC_DIST_BASE + GIC_DIST_ENABLE_SET + (4*i));
//	}

	/* restore gic */
	gic_restore_info();

	/* restore timer */
	timer_restore_regs();

#ifdef USE_MASK_ALLBLOCK
	/* unmask all blocks */
	unmask_all_blocks();
#endif
}

unsigned int sdp1106_getspeed(unsigned int cpu)
{
	unsigned int val;
	
	val = R_PMU_CPU_PMS_CON;
	val = (INPUT_FREQ >> (GET_PLL_S(val))) / GET_PLL_P(val) * GET_PLL_M(val);
	val /= 1000;
	
	return val;
//	return clk_get_rate(cpu_clk) / 1000;
}

void sdp1106_set_clkdiv(unsigned int div_index)
{
	unsigned long flags;
//	unsigned int val;
	
	printk("\033[1;7;33mSet clock: %dMHz(0x%x), cur pms val = 0x%x\033[0m\n", 
			sdp1106_freq_table[div_index].frequency/1000,
			clkdiv_cpu[div_index], R_PMU_CPU_PMS_CON);

#ifdef USE_SE_INT
	/* check SE ready */
	if (readl(VA_IO_BASE0 + 0x003E840C))
	{
		printk("\033[1;7;33mSE is not ready\033[0m\n");	
		return;
	}
#endif

	/* set local timer mult */
//	twd_set_mult(sdp1106_freq_table[div_index].frequency);

	/* Change Divider - CPU */
	/* CPU 1 down */
//	if (cpu_online(1))
//		cpu_down(1);

	spin_lock_irqsave(&cpufreq_lock, flags);

//	sdp1106_set_clock(div_index); // CPU0 WFI, request to SE
//	R_PMU_CPU_PMS_CON = clkdiv_cpu[div_index]; // ARM sets pms value directily.

	spin_unlock_irqrestore(&cpufreq_lock, flags);
	
	/* CPU 1 up */
//	if (cpu_is_offline(1))
//		cpu_up(1);
	
	printk("\033[1;7;33mcur pms val = 0x%x\033[0m\n", R_PMU_CPU_PMS_CON);

}

static void sdp1106_set_frequency(unsigned int old_index, unsigned int new_index)
{
	/* Change the system clock divider values */
	sdp1106_set_clkdiv(new_index);
}

/* change the arm core voltage */
static void sdp1106_set_voltage(unsigned int index)
{
	unsigned long flags;
	unsigned int val;
	static unsigned int old_val = 0;

#ifdef USE_SE_INT
	/* check SE ready */
	if (readl(VA_IO_BASE0 + 0x003E840C))
	{
		printk("\033[1;7;33mSE is not ready\033[0m\n");	
		return;
	}
#endif	

#if 1	
	/* set gpio */
	//printk(">> set voltage : %dmV\n", sdp1106_volt_table[index].arm_volt);
	spin_lock_irqsave(&cpufreq_lock, flags);

	val = VA_GPIO8_WRITE;
	if (index <= L7) 
		val = (val & 0xF) | 0x4;
	else
		val = (val & 0xF) | 0x7;
	
	VA_GPIO8_WRITE = val;
	printk("\033[1;7;32mVoltage set to %duV\033[0m\n", sdp1106_volt_table[index]);

	old_val = sdp1106_volt_table[index];

	spin_unlock_irqrestore(&cpufreq_lock, flags);
#endif	
}


static void __init set_volt_table(void)
{
	/* TODO: apply ASV table settings */
}

bool sdp1106_pms_change(unsigned int old_index, unsigned int new_index)
{
	unsigned int old_pm = clkdiv_cpu[old_index];
	unsigned int new_pm = clkdiv_cpu[new_index];

	return (old_pm == new_pm) ? 0 : 1;
}

int sdp_cpufreq_mach_init(struct sdp_dvfs_info *info)
{
	unsigned long flags;
	unsigned int val;

	cpu_clk = clk_get(NULL, "FCLK");
	if (IS_ERR(cpu_clk)) {
		/* error */
		printk(KERN_ERR "%s - clock get fail", __func__);
		return PTR_ERR(cpu_clk);
	}
	
	/* init gpio control for PMIC */
	spin_lock_irqsave(&cpufreq_lock, flags);
	val = VA_GPIO8_CONT;
	val = (val & 0xFFFF) | 0x3333; /* 8.0 ~ 8.3 out */
	VA_GPIO8_CONT = val;
	spin_unlock_irqrestore(&cpufreq_lock, flags);

	/* init legacy interrupt controller */
	writel(0, VA_INTC_BASE + 0x08); // INTMOD
	writel(0xffffffff, VA_INTC_BASE + 0x0C); // INTMASK
	writel(1, VA_INTC_BASE); // INTCON

	info->pll_safe_idx = L0;
	info->pm_lock_idx = L2;
	info->max_support_idx = max_support_idx;
	info->min_support_idx = min_support_idx;
	info->gov_support_freq = NULL;
	info->cpu_clk = cpu_clk;
	info->volt_table = sdp1106_volt_table;
	info->freq_table = sdp1106_freq_table;
	info->set_freq = sdp1106_set_frequency;
	info->need_apll_change = sdp1106_pms_change;
//	info->get_speed = sdp1106_getspeed;

	return 0;	
}
EXPORT_SYMBOL(sdp_cpufreq_mach_init);

