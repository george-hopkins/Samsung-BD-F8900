/*
 * sdp1207_misc.c
 * sdp1207 miscellaneous (ext interrupt, pad mux, and pin attribute)
 *
 * Copyright (C) 2011 Samsung Electronics.co
 * tukho.kim <tukho.kim@samsung.com>
 * sh.chon <sh.chon@samsung.com>
 * Sola lee <sssol.lee@samsung.com>
 * 
 */

/*
 * 2011.07.04: created by tukho.kim
 */

#ifndef CONFIG_ARM_GIC
#include <linux/init.h> 
#include <linux/spinlock.h>
#include <linux/module.h>
#include <linux/interrupt.h>
//#include <linux/jiffies.h>
#include <asm/io.h>

#include <asm/cacheflush.h>
#include <mach/platform.h>

#define VERSION 	"0.5"

/*********************************************************/
/*   i2c pre init 					 */
/*********************************************************/
/* TODO */

/*********************************************************/
/*   mac pre init 					 */
/*********************************************************/
/* TODO */

/*********************************************************/
/*   uart pre init 					 */
/*********************************************************/
/* TODO */

/*********************************************************/
/*   External interrupt source 				 */
/*********************************************************/
#define A_SUBINT_STAT		(VA_IO_BASE0 + 0x00090F94)				// INT 31 sub	0~7
 
/**********************************************************************************/
/* exiternal interrupt irq  31		 										  */  
/**********************************************************************************/
#define EXT_INTR_NAME		"SDP EINTR"

#define R_EXT_INTR_CON		(*(volatile u32*)(VA_IO_BASE0 + 0x00090CB0))	// Ext int
#define R_EXT_INTR_SEL		(*(volatile u32*)(VA_IO_BASE0 + 0x00090CB4))	// Ext int
#define R_EXT_INTR_BYPASS	(*(volatile u32*)(VA_IO_BASE0 + 0x00090CB8))	// Ext int
#define R_SUBINT_STAT		(*(volatile u32*)(A_SUBINT_STAT))				// INT 31 sub 0~7
#define NR_EXT_INTR		8

struct sdp_eint_t {
//	int	n_bit;
//	volatile u32 * status;
	void * 	args;
	void (*fp)(void*);

};

static DEFINE_SPINLOCK(ext_intr_lock);
static struct sdp_eint_t sdp_eint[NR_EXT_INTR];

int sdp_extint_enable(int);
int sdp_extint_disable(int);

static void call_eint_fp (int n_extirq)
{
	struct sdp_eint_t* p_eint;

	p_eint = (sdp_eint + n_extirq);

	if(p_eint->fp) {
		p_eint->fp(p_eint->args);
	}
	else {
		printk(KERN_ERR"[%s] %d ext not exist ISR\n", EXT_INTR_NAME, n_extirq);
		printk(KERN_ERR"[%s] %d ext is disabled\n", EXT_INTR_NAME, n_extirq);
		sdp_extint_disable(n_extirq);
	}
}

static irqreturn_t sdp_eint_isr(int irq, void* devid)
{
	int idx;
	int n_extirq, nr_extirq;	
	volatile u32 * reg_status;
	u32 status;
	
	switch(irq){
		case(31):
			reg_status = (volatile u32*)A_SUBINT_STAT;
			status = *reg_status;	
			nr_extirq = 8;		// 0~7
			break;
		default:
			printk(KERN_ERR"[%s] %d Not registered interrupt source \n", EXT_INTR_NAME,irq);
			return IRQ_NONE;
			break;
	}

	if(!status) return IRQ_NONE;

	for(idx = 0; idx < nr_extirq; idx++){
		if(status & (1 << idx)){
			n_extirq = idx;
			call_eint_fp(n_extirq);
		}	
	}

	*reg_status = status;// pending clear

	return IRQ_HANDLED;
}

int sdp_extint_set_attr(int n_extirq, int attr)
{
	u32 regval;
	unsigned long flags; 

	if((n_extirq < NR_EXT_INTR) && (attr < 8)){

		spin_lock_irqsave(&ext_intr_lock, flags);

		regval = R_EXT_INTR_CON & ~(7 << (3 * n_extirq));	
		regval |= (attr << (3 * n_extirq));	
		R_EXT_INTR_CON = regval;
		
		spin_unlock_irqrestore(&ext_intr_lock, flags);
	}
	else {
		printk(KERN_ERR"[%s] NOT Support %d:%d \n", EXT_INTR_NAME,  n_extirq, attr);
		return -1;
	}

	return 0;
}

EXPORT_SYMBOL(sdp_extint_set_attr);

int sdp_extint_enable(int n_extirq)
{
	unsigned long flags;

	if(n_extirq >= NR_EXT_INTR){
		return -1;
	}

	spin_lock_irqsave(&ext_intr_lock, flags);
	
	R_EXT_INTR_SEL &=  ~(1 << n_extirq); 	// '0': enable

	spin_unlock_irqrestore(&ext_intr_lock, flags);


	return 0;
}
EXPORT_SYMBOL(sdp_extint_enable);

int sdp_extint_disable(int n_extirq)
{
	unsigned long flags;

	if(n_extirq >= NR_EXT_INTR){
		return -1;
	}

	spin_lock_irqsave(&ext_intr_lock, flags);

	R_EXT_INTR_SEL |=  (1 << n_extirq); 	// '1': disable

	spin_unlock_irqrestore(&ext_intr_lock, flags);

	return 0;
}
EXPORT_SYMBOL(sdp_extint_disable);

int sdp_extint_fp_register(int n_extirq, int attr, void (*fp)(void*), void* args)
{
	int retval = 0;
	
	if(n_extirq >= NR_EXT_INTR) {
		return -1;
	}
	
	if(sdp_extint_set_attr(n_extirq, attr) < 0) {
		return -1;
	}

	spin_lock(&ext_intr_lock);

	if(sdp_eint[n_extirq].fp) {
		printk(KERN_ERR"[%s] %d Ext ISR slot not empty\n", EXT_INTR_NAME, n_extirq);
		retval = -1;
		goto __out_register;		
	}
		
	sdp_eint[n_extirq].fp = fp;
	sdp_eint[n_extirq].args = args;

	printk(KERN_INFO"[%s] %d Ext ISR is registered successfully\n", EXT_INTR_NAME, n_extirq);

	sdp_extint_enable(n_extirq);

__out_register:
	spin_unlock(&ext_intr_lock);

	return retval;
}
EXPORT_SYMBOL(sdp_extint_fp_register);

static void __init sdp_extint_preinit(void)
{
	R_EXT_INTR_BYPASS = 0x5; 	// Use external OSC clk
	R_EXT_INTR_SEL = 0xFFFFFFFF; 	// all disable
	R_SUBINT_STAT = 0xFFFFFFFF;	// all clear
}

static int __init sdp_extint_init(void)
{
	int retval = 0;
	
	printk(KERN_INFO"sdp external interrupt init\n");

	sdp_extint_preinit();

	spin_lock_init(&ext_intr_lock);
	memset(sdp_eint, 0, sizeof(sdp_eint));

	retval = request_irq(IRQ_EXT, sdp_eint_isr, IRQF_DISABLED, "sdp-eint", (void*) &sdp_eint[0]);
	if(retval) {
 		printk(KERN_ERR"[%s] %d request_irq is failed\n", EXT_INTR_NAME, IRQ_EXT);
		goto __extint_exit;	
	} else {
		printk(KERN_ERR"[%s] %d External interrupt is registered \n", EXT_INTR_NAME, IRQ_EXT);
	}

__extint_exit:
	return retval;
}

 static void __exit sdp_extint_exit(void)
{
	 free_irq(31, (void *)&sdp_eint[0]);
}

static int __init sdp_misc_init(void)
{
	int retval;

	retval = sdp_extint_init();
	return retval;
}

static void __exit sdp_misc_exit(void)
{
	sdp_extint_exit();
}

module_init(sdp_misc_init);
module_exit(sdp_misc_exit);
#endif


