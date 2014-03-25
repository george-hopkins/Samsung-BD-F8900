/*
 * sdp1106_misc.c
 * sdp1106 miscellaneous (ext interrupt, pad mux, and pin attribute)
 *
 * Copyright (C) 2010 Samsung Electronics.co
 * tukho.kim <tukho.kim@samsung.com>
 *
 */

/*
 * 2011.06.28 sssol.lee@samsung.com
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
/*   Merger/External interrupt source 				 */
/*********************************************************/
#define A_SUBINT_STAT		(VA_IO_BASE0 + 0x00090F94)				// INT 31 sub	0~1

#define A_INTMER_STAT_CH1		(VA_IO_BASE0 + 0x00091414)				// INT 30 sub eint 0~7
#define A_INTMER_MASK_CH1		(VA_IO_BASE0 + 0x00091418)			
#define A_INTMER_POL_CH1		(VA_IO_BASE0 + 0x00091420)				
#define A_INTMER_LEV_CH1		(VA_IO_BASE0 + 0x00091424)	

#define A_INTMER_STAT_CH0		(VA_IO_BASE0 + 0x00091400)				// INT 29 sub eint 0~5
#define A_INTMER_MASK_CH0		(VA_IO_BASE0 + 0x00091404)				
#define A_INTMER_POL_CH0		(VA_IO_BASE0 + 0x0009140C)				
#define A_INTMER_LEV_CH0		(VA_IO_BASE0 + 0x00091410)	
			

/*********************************************************/
/*   interrupt merger irq 29 							 */
/*********************************************************/
#define INTC_MERG_NAME		"SDP INTR MERGER"

#define NR_INTR_MERGER	6		// HW spec
#define NR_ISR_SLOT		2		// MAX 16, virtual isr = SHARED INTERRUPT

typedef irqreturn_t (*intc_merg_isr)(int irq, void* dev_id);

struct sdp_intc_merger_t{
	void*				dev_id;
	intc_merg_isr		fp;
};

static DEFINE_SPINLOCK(intc_merg_lock);
static struct sdp_intc_merger_t intc_merger[NR_INTR_MERGER][NR_ISR_SLOT];
static void sdp_intc_merger_enable(int n_int)
{
	unsigned long flag;
	u32 regval;

	spin_lock_irqsave(&intc_merg_lock, flag);

	regval = readl(A_INTMER_STAT_CH0);
	regval |= (1 << n_int); 	// pending clear
	writel(regval, A_INTMER_STAT_CH0);

	regval = readl(A_INTMER_MASK_CH0);
	regval &= ~(1 << n_int ); 	// un-masking	
	writel(regval, A_INTMER_MASK_CH0);

	spin_unlock_irqrestore(&intc_merg_lock, flag);
}

static void sdp_intc_merger_disable(int n_int)
{
	unsigned long flag;
	u32 regval ;

	spin_lock_irqsave(&intc_merg_lock, flag);

	regval = readl(A_INTMER_STAT_CH0);
	regval |= (1 << n_int);  	// pending clear
	writel(regval, A_INTMER_STAT_CH0);

	regval = readl(A_INTMER_MASK_CH0);
	regval |= (1 << n_int );  	//masking	
	writel(regval, A_INTMER_MASK_CH0);

	spin_unlock_irqrestore(&intc_merg_lock, flag);
}

static void inline merger_isr_run(int irq, int n_int)
{
	unsigned short	run_flag = 0;
	void		   *m_dev_id;
	u8				slot;  

	for(slot = 0; slot < NR_ISR_SLOT; slot++){
		if(intc_merger[n_int][slot].fp){
			m_dev_id = intc_merger[n_int][slot].dev_id;
			intc_merger[n_int][slot].fp(irq, m_dev_id);
			run_flag |= (1 << slot);
		}
	}

	if(!run_flag) {
		printk(KERN_ERR"[%s] %d ext not exist ISR\n", INTC_MERG_NAME, n_int);
		printk(KERN_ERR"[%s] %d ext is disabled\n", INTC_MERG_NAME, n_int);
		sdp_intc_merger_disable(n_int);
	}
}


static irqreturn_t sdp_intc_merger(int irq, void* dev_id)
{
	int idx;
	volatile u32 * reg_status;
	u32 status;
	
	reg_status = (volatile u32*)A_INTMER_STAT_CH0;
	status = *reg_status;	

	if(!status) return IRQ_NONE;

	for(idx = 0; idx < NR_INTR_MERGER; idx++){
		if(status & (1 << idx)){
			merger_isr_run(irq, idx);	
		}	
	}
	*reg_status = status;

	return IRQ_HANDLED;
}

int request_irq_intc_merg(int n_int, void* dev_id, intc_merg_isr fp)
{
	unsigned long flag;

	intc_merg_isr m_fp;
	void *m_dev_id;
	u16  		slot_flag = 0, check_slot = 0;
	u8 		   	slot;

	if(!fp){
		printk(KERN_ERR"[%s] %d interrupt merger register failed\n", __FUNCTION__, n_int);
		printk(KERN_ERR"[%s] %d interrupt merger dev_id null\n", __FUNCTION__, n_int);
		return -1;	
	}


	for(slot = 0; slot < NR_ISR_SLOT; slot++){
		m_fp = intc_merger[n_int][slot].fp;
		if(m_fp){
			check_slot |= (1 << slot);
		}
	}

	for(slot = 0; slot < NR_ISR_SLOT; slot++){
		m_fp = intc_merger[n_int][slot].fp;
		m_dev_id = intc_merger[n_int][slot].dev_id;

		if((m_fp == fp) && (m_dev_id == dev_id)){
			break;
		} else if(!m_fp){
			spin_lock_irqsave(&intc_merg_lock, flag);
			intc_merger[n_int][slot].dev_id = dev_id;
			intc_merger[n_int][slot].fp = fp;
			slot_flag = (1 << slot);
			spin_unlock_irqrestore(&intc_merg_lock, flag);
			break;
		}
	}

	if(!check_slot && slot_flag){					// interrupt enable
		sdp_intc_merger_enable(n_int);
	}

	if(!slot_flag){
		printk(KERN_ERR"[%s] %d interrupt merger register failed\n", __FUNCTION__, n_int);
		printk(KERN_ERR"[%s] %d interrupt merger slot is full\n", __FUNCTION__, n_int);
		return -1;	
	}

	printk(KERN_INFO"[%s] %d:%d interrupt merger registerd\n", __FUNCTION__, n_int, slot);

	return 0;
}

EXPORT_SYMBOL(request_irq_intc_merg);

int release_irq_intc_merg(int n_int, void* dev_id, intc_merg_isr fp)
{
	int retval = 0;
	unsigned long flag;

	intc_merg_isr m_fp;
	void * 		  m_dev_id;
	u16 		slot_flag = 0;
	u16			check_slot = 0;
	u8 			slot;

	for(slot = 0; slot < NR_ISR_SLOT; slot++){
		m_fp = intc_merger[n_int][slot].fp;
		m_dev_id = intc_merger[n_int][slot].dev_id;

		if((m_fp == fp) && (m_dev_id == dev_id)){
			spin_lock_irqsave(&intc_merg_lock, flag);
			intc_merger[n_int][slot].dev_id = NULL;
			intc_merger[n_int][slot].fp = NULL;
			spin_unlock_irqrestore(&intc_merg_lock, flag);
			slot_flag = (1 << slot);
		} else if(m_fp){
			check_slot |= (1 << slot);
		}
	}	

	if(!check_slot){
		sdp_intc_merger_disable(n_int);
	}

	if(!slot_flag){
		printk(KERN_INFO"[%s] %d interrupt merger release failed\n", __FUNCTION__, n_int);
		retval = -1;
	}

	printk(KERN_INFO"[%s] %d:%d interrupt merger released \n", __FUNCTION__, n_int, slot);
	

	return retval;
}

EXPORT_SYMBOL(release_irq_intc_merg);

/**********************************************************************************/
/* exiternal interrupt irq 30	 										  */  
/**********************************************************************************/
#define EXT_INTR_NAME		"SDP EINTR"

#define R_EXT_INTR_CON		(*(volatile u32*)(VA_IO_BASE0 + 0x00090C70))	
#define R_EXT_INTR_SEL		(*(volatile u32*)(VA_IO_BASE0 + 0x00090C74))	
#define R_EXT_INTR_BYPASS	(*(volatile u32*)(VA_IO_BASE0 + 0x00090C78))	

#define R_INTMER_STAT_CH1		(*(volatile u32*)(A_INTMER_STAT_CH1))				
#define R_INTMER_MASK_CH1		(*(volatile u32*)(A_INTMER_MASK_CH1))				
#define R_INTMER_POL_CH1		(*(volatile u32*)(A_INTMER_POL_CH1))				
#define R_INTMER_LEV_CH1		(*(volatile u32*)(A_INTMER_LEV_CH1))				 

#define NR_EXT_INTR		8	// ch1 0~7 

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
	int n_extirq=0, nr_extirq=0;	
	volatile u32 * reg_status;
	u32 status;
	
	switch(irq){
		case(30):
			reg_status = (volatile u32*)A_INTMER_STAT_CH1;
			status = *reg_status & 0xFFFFFFFF;
			nr_extirq = 8;		// 0~7	수정OK
			break;
		default:
			printk(KERN_ERR"[%s] %d Not registered interrupt source \n", EXT_INTR_NAME,irq);
			return IRQ_NONE;
			break;
	}

	if(!status) return IRQ_NONE;

	for(idx = 0; idx < nr_extirq; idx++){
		if(status & (1<< idx)){
			n_extirq = idx;} 
			call_eint_fp(n_extirq);
		}	
	*reg_status = status;
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
/**********************************************************************************/
/* exiternal interrupt irq 31	 										  */  
/**********************************************************************************/
#define SUB_INTR_NAME		"SDP SUBINTR"

#define R_SUBINT_STAT		(*(volatile u32*)(A_SUBINT_STAT))				
#define R_SUB_MASK_POL(n)		(*(volatile u32*)(VA_IO_BASE0 + 0x00090FB8+0x100*n))	
#define R_SUB_MASK_MASK(n)		(*(volatile u32*)(VA_IO_BASE0 + 0x00090FBC+0x100*n))	

#define NR_SUB_INTR		2	// SUB 31 0~1

struct sdp_subint_t {
	void * 	args;
	void (*fp)(void*);
};

static DEFINE_SPINLOCK(sub_intr_lock);
static struct sdp_subint_t sdp_subint[NR_SUB_INTR];

int sdp_subint_enable(int n_extirq)
{
	unsigned long flags;

	if(n_extirq >= NR_SUB_INTR){
		return -1;
	}

	spin_lock_irqsave(&sub_intr_lock, flags);
	
	R_SUB_MASK_MASK(n_extirq)&=~(0x3F);	//enable 0
	R_SUBINT_STAT|=(1<<n_extirq);

	spin_unlock_irqrestore(&sub_intr_lock, flags);

	return 0;
}
EXPORT_SYMBOL(sdp_subint_enable);

int sdp_subint_disable(int n_extirq)
{
	unsigned long flags;

	if(n_extirq >= NR_SUB_INTR){
		return -1;
	}

	spin_lock_irqsave(&sub_intr_lock, flags);
	
	R_SUB_MASK_MASK(n_extirq)|=0x3F;//masking 1
	R_SUBINT_STAT|=(1<<n_extirq);

	spin_unlock_irqrestore(&sub_intr_lock, flags);

	return 0;
}
EXPORT_SYMBOL(sdp_subint_disable);
static void call_subint_fp (int n_extirq)
{
	struct sdp_subint_t* p_eint;

	p_eint = (sdp_subint + n_extirq);

	if(p_eint->fp) {
		p_eint->fp(p_eint->args);
	}
	else {
		printk(KERN_ERR"[%s] %d sub not exist ISR\n", SUB_INTR_NAME, n_extirq);
		printk(KERN_ERR"[%s] %d sub is disabled\n", SUB_INTR_NAME, n_extirq);
		sdp_subint_disable(n_extirq);
	}
}
static irqreturn_t sdp_subint_isr(int irq, void* devid)
{
	int idx;
	int n_extirq=0, nr_extirq=0;	
	volatile u32 * reg_status;
	u32 status;
	
	switch(irq){
		case(31):
			reg_status = (volatile u32*)A_SUBINT_STAT;
			status = *reg_status;	
			nr_extirq = 2;		// 0,1
			break;
		default:
			printk(KERN_ERR"[%s] %d Not registered interrupt source \n", SUB_INTR_NAME,irq);
			return IRQ_NONE;
			break;
	}

	if(!status) return IRQ_NONE;
	for(idx = 0; idx < nr_extirq; idx++){
		if(status & (1 << idx)){
			n_extirq = idx;
			} 
			call_subint_fp(n_extirq);
		}	
	*reg_status = status;
	return IRQ_HANDLED;
}


int sdp_subint_fp_register(int n_extirq, void (*fp)(void*), void* args)
{
	int retval = 0;
	
	if(n_extirq >= NR_SUB_INTR) {
		return -1;
	}

	spin_lock(&sub_intr_lock);

	if(sdp_subint[n_extirq].fp) {
		printk(KERN_ERR"[%s] %d sub ISR slot not empty\n", SUB_INTR_NAME, n_extirq);
		retval = -1;
		goto __out_register;		
	}
		
	sdp_subint[n_extirq].fp = fp;
	sdp_subint[n_extirq].args = args;

	printk(KERN_INFO"[%s] %d sub ISR is registered successfully\n", SUB_INTR_NAME, n_extirq);

	sdp_subint_enable(n_extirq);

__out_register:
	spin_unlock(&sub_intr_lock);

	return retval;
}
EXPORT_SYMBOL(sdp_subint_fp_register);

//-------------------------------------------
static void __init sdp_extint_preinit(void)
{
	R_EXT_INTR_SEL = 0x0FF; 	// all disable 0~7
	R_EXT_INTR_BYPASS = 0x2; 	//	24Mhz OSC
	R_INTMER_MASK_CH1 |= 0xFFFFFFFF;	// all clear
	R_INTMER_STAT_CH1 |= 0xFFFFFFFF;	// all un-mask 
	
}
static void __init sdp_subint_preinit(void)//sub_mask 설정 안하는 이유 : 기본 셋팅이 masking
{
	R_SUBINT_STAT = 0xFFFFFFFF;	// all clear
}
static int __init sdp_extint_init(void)
{
	int retval = 0;
	
	printk(KERN_INFO"sdp external interrupt init\n");

	sdp_extint_preinit();

	spin_lock_init(&ext_intr_lock);
	memset(sdp_eint, 0, sizeof(sdp_eint));

	retval = request_irq(IRQ_MERGER1, sdp_eint_isr, IRQF_DISABLED, "sdp-eint", (void*) &sdp_eint[0]);
	if(retval) {
		printk(KERN_ERR"[%s] %d request_irq is failed\n", EXT_INTR_NAME, 30);
		goto __extint_exit;	
	} else {
		printk(KERN_ERR"[%s] %d External interrupt is registered \n", EXT_INTR_NAME, 30);
	}

__extint_exit:
	return retval;
}

static int __init sdp_intcmerg_init(void)
{
	int retval = 0;
	
	printk(KERN_INFO"sdp  merger interruptinit\n");
	
	writel(0xFFFFFFFF, A_INTMER_MASK_CH0);	// all pending clear
	writel(0xFFFFFFFF, A_INTMER_STAT_CH0);	// all disable
	
	spin_lock_init(&intc_merg_lock); 
	memset(intc_merger, 0, sizeof(intc_merger));

	retval = request_irq(IRQ_MERGER0, sdp_intc_merger, IRQF_DISABLED, "sdp-intc-merger", (void*) &intc_merger);
	if(retval) {
		printk(KERN_ERR"[%s] %d request_irq is failed\n", INTC_MERG_NAME, 29);
	} else {
		printk(KERN_ERR"[%s] %d interrupt merger is registered \n", INTC_MERG_NAME, 29);
	}

	return retval;
}
static int __init sdp_subint_init(void)
{
	int retval = 0;
	
	printk(KERN_INFO"sdp sub interrupt init\n");

	sdp_subint_preinit();

	spin_lock_init(&sub_intr_lock);
	memset(sdp_subint, 0, sizeof(sdp_subint));

	retval = request_irq(IRQ_SUB, sdp_subint_isr, IRQF_DISABLED, "sdp-subint", (void*) &sdp_subint[0]);//SUB 31 0~1
	if(retval) {
		printk(KERN_ERR"[%s] %d request_irq is failed\n", SUB_INTR_NAME, 31);
		goto __extint_exit;	
	} else {
		printk(KERN_ERR"[%s] %d External interrupt is registered \n", SUB_INTR_NAME, 31);
	}

__extint_exit:
	return retval;
}


static void __exit sdp_extint_exit(void)
{
	  free_irq(IRQ_MERGER1, (void *)&sdp_eint[0]);
}
static void __exit sdp_subint_exit(void)
{
	  free_irq(IRQ_SUB, (void *)&sdp_subint[0]);
}
static void __exit sdp_intcmerg_exit(void)
{
	 free_irq(IRQ_MERGER0, (void *)intc_merger);
}

static int __init sdp_misc_init(void)
{
	int retval;

	retval = sdp_intcmerg_init();
	if(retval) return retval;
	retval = sdp_subint_init();
	if(retval) return retval;
	return sdp_extint_init();
}


static void __exit sdp_misc_exit(void)
{
	sdp_extint_exit();
	sdp_intcmerg_exit();
	sdp_subint_exit();
}

module_init(sdp_misc_init);
module_exit(sdp_misc_exit);

#endif 

