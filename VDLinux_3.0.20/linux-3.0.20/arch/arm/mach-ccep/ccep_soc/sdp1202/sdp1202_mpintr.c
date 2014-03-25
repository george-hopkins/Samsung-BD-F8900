/*************************************8********************************************************
 *
 *	sdp1202_mpintr.c (FoxMP Interrupt Controller Driver for DualMP)
 *
 *	author : seungjun.heo@samsung.com
 *	
 ********************************************************************************************/
/*********************************************************************************************
 * Description 
 * Date 	author		Description
 * ----------------------------------------------------------------------------------------
	Sep,10,2012 	seungjun.heo	created
 ********************************************************************************************/

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/syscalls.h>
#include <asm/uaccess.h>
#include <linux/miscdevice.h>

#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>

#define MPISR_BASE	0x1A091000
#define MPISR_REG_STAT_CLEAR	0x0
#define MPISR_REG_MASK		0x4
#define MPISR_REG_MASKED_STAT	0x8
#define MPISR_REG_POLARITY	0xC
#define MPISR_REG_TYPE		0x10
#define	MPISR_REG_STMMODE	0x14

#define MPISR_IRQ_TSD	0
#define MPISR_IRQ_AIO	1
#define MPISR_IRQ_AE	2
#define MPISR_IRQ_JPEG	3
#define MPISR_IRQ_DP	4
#define MPISR_IRQ_GA	5
#define MPISR_IRQ_MFD	6




#define SDP_MP_ISR_DEV_NAME		"sdp_mpisr"

static u32 mpisr_base = 0;

int sdp_mpint_enable(int n_mpirq);
int sdp_mpint_disable(int n_mpirq);
int sdp_mpint_request_irq(int n_mpirq, void (*fp)(void*), void* args);

//#define	IRQ_MPISR	31
#define	IRQ_MPISR	IRQ_EXT6

#define NR_MP_INTR	32

struct sdp_mpisr_t {
	void * 	args;
	void (*fp)(void*);
};

static DEFINE_SPINLOCK(mpisr_lock);
static struct sdp_mpisr_t sdp_mpisr[NR_MP_INTR];

static void __init sdp_mpisr_preinit(void)	//mask 설정 안하는 이유 : 기본 셋팅이 masking
{
	writel(0xFFFFFFFF, mpisr_base + MPISR_REG_STAT_CLEAR);	// all clear
}

int sdp_mpint_enable(int n_mpirq)
{
	unsigned long flags, val;

	if(n_mpirq >= NR_MP_INTR){
		return -1;
	}

	spin_lock_irqsave(&mpisr_lock, flags);
	
	val = readl(mpisr_base + MPISR_REG_MASK);
	val &= ~(1UL << n_mpirq);
	writel(val, mpisr_base + MPISR_REG_MASK);	//enable
	writel(1 << n_mpirq, mpisr_base + MPISR_REG_STAT_CLEAR);	//clear

	spin_unlock_irqrestore(&mpisr_lock, flags);

	return 0;
}
EXPORT_SYMBOL(sdp_mpint_enable);

int sdp_mpint_disable(int n_mpirq)
{
	unsigned long flags, val;

	if(n_mpirq >= NR_MP_INTR){
		return -1;
	}

	spin_lock_irqsave(&mpisr_lock, flags);
	
	val = readl(mpisr_base + MPISR_REG_MASK);
	val |= (1UL << n_mpirq);
	writel(val, mpisr_base + MPISR_REG_MASK);	//disable
	writel(1 << n_mpirq, mpisr_base + MPISR_REG_STAT_CLEAR);	//clear

	spin_unlock_irqrestore(&mpisr_lock, flags);

	return 0;
}
EXPORT_SYMBOL(sdp_mpint_disable);

int sdp_mpint_request_irq(int n_mpirq, void (*fp)(void*), void* args)
{
	int retval = 0;

	if(mpisr_base == 0)
	{
		printk(KERN_ERR"Cannot Register Interrupt!!! MP2 Interrupt is not initialized!!\n");
		return -1;
	}
	
	if(n_mpirq >= NR_MP_INTR) {
		return -1;
	}


	if(sdp_mpisr[n_mpirq].fp) {
		printk(KERN_ERR"[%s] %d sub ISR slot not empty\n", SDP_MP_ISR_DEV_NAME, n_mpirq);
		retval = -1;
		goto __out_register;		
	}
		
	sdp_mpisr[n_mpirq].fp = fp;
	sdp_mpisr[n_mpirq].args = args;

	printk(KERN_INFO"[%s] %d sub ISR is registered successfully\n", SDP_MP_ISR_DEV_NAME, n_mpirq);

	sdp_mpint_enable(n_mpirq);

__out_register:

	return retval;
}
EXPORT_SYMBOL(sdp_mpint_request_irq);

static void call_mpint_fp (int n_mpirq)
{
	struct sdp_mpisr_t* p_mpint;

	p_mpint = (sdp_mpisr + n_mpirq);

	if(p_mpint->fp) {
		p_mpint->fp(p_mpint->args);
	}
	else {
		printk(KERN_ERR"[%s] %d sub not exist ISR\n", SDP_MP_ISR_DEV_NAME, n_mpirq);
		printk(KERN_ERR"[%s] %d sub is disabled\n", SDP_MP_ISR_DEV_NAME, n_mpirq);
		sdp_mpint_disable(n_mpirq);
	}
}

static irqreturn_t sdp_mpisr_isr(int irq, void* devid)
{
	int idx;
	int n_mpirq = 0;	
	u32 status;
	
	switch(irq){
		case IRQ_MPISR:
			status = readl(mpisr_base + MPISR_REG_MASKED_STAT);
			break;
		default:
			printk(KERN_ERR"[%s] %d Not registered interrupt source \n", SDP_MP_ISR_DEV_NAME,irq);
			return IRQ_NONE;
			break;
	}

	if(!status) return IRQ_NONE;
	for(idx = 0; idx < NR_MP_INTR; idx++){
		if(status & (1UL << idx))
		{
			n_mpirq = idx;
			call_mpint_fp(n_mpirq);
		}
	}
	writel(status, mpisr_base + MPISR_REG_STAT_CLEAR);
	return IRQ_HANDLED;
}

static int __init sdp_mpisr_init(void)
{
	int res, retval;
	u32 val;

	val = readl(VA_EBUS_BASE);

	if(!(val & 0x80000000))		//Dual MP인지 check
	{
		printk("Cannot Support Dual MP.\n");
		return -1;
	}

	if(mpisr_base == 0)
		mpisr_base = (u32) ioremap(MPISR_BASE, 0x100);
	
	if(mpisr_base == 0)
	{
		printk("mpisr : can't allocate device mem!!!\n");
		return -1;
	}
	
	writel(0, mpisr_base + MPISR_REG_STMMODE);
//	writel(0xFE, 0xfeb700b0);		
	
	printk("Support DualMP : Switch MP2 Interrupt... cannot use STM!\n");
	
	sdp_mpisr_preinit();

	spin_lock_init(&mpisr_lock);
	memset(sdp_mpisr, 0, sizeof(sdp_mpisr));
	
	retval = request_irq(IRQ_MPISR, sdp_mpisr_isr, IRQF_DISABLED, "sdp-mpisr", (void*) &sdp_mpisr[0]);//SUB 31 0~1
	if(retval) {
		printk(KERN_ERR"[%s] %d request_irq is failed\n", SDP_MP_ISR_DEV_NAME, IRQ_MPISR);
		res = -1;
	} else {
		printk(KERN_ERR"[%s] %d interrupt is registered \n", SDP_MP_ISR_DEV_NAME, IRQ_MPISR);
	}
	
	return res;
}

#if 0
static u32 dp_regbase;

#define DP_INT_MASK_STAT	0x284
#define DISPOUT_VSYNC_MASK	0x1
#define DISPOUT_VSYNC_STAT	0x10000

static void dpisr(void * args)
{
	unsigned int stat;
	static int cnt = 0;

	stat = readl(dp_regbase + DP_INT_MASK_STAT);
//	mask = stat & 0xFFFF;
	
	if(stat & DISPOUT_VSYNC_STAT)
		cnt++;
	if(cnt % 30 == 0)
		printk("DP Vsync %d occured!!!!\n", cnt);

	writel(stat, dp_regbase + DP_INT_MASK_STAT);
}

void testdpirq(void)
{
	unsigned int stat, mask;

	dp_regbase = (u32) ioremap(0x1A910000, 0x10000);
	
	mask = DISPOUT_VSYNC_MASK;
	stat = DISPOUT_VSYNC_STAT;
	writel(mask | stat, dp_regbase + DP_INT_MASK_STAT);		//DP VSync clear and only enable
	
	sdp_mpint_request_irq(MPISR_IRQ_DP, dpisr, NULL);
}
#endif

MODULE_LICENSE("Properity");
arch_initcall(sdp_mpisr_init);


