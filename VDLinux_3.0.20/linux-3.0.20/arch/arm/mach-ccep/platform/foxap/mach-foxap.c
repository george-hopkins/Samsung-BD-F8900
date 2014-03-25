/*
 * mach-foxap.c
 *
 * Copyright (C) 2012 Samsung Electronics.co
 * SeungJun Heo <seungjun.heo@samsung.com>
 */

#include <linux/init.h>
#include <linux/device.h>
#include <linux/dma-mapping.h>
#include <linux/smp.h>
#include <linux/io.h>

#include <asm/mach-types.h>
#include <asm/mach/map.h>
#include <asm/smp_scu.h>
#include <asm/setup.h>
#include <asm/mach/arch.h>
#include <asm/hardware/gic.h>
#include <linux/irq.h>

#include <mach/hardware.h>

unsigned int sdp_sys_mem0_size = SYS_MEM0_SIZE;
unsigned int sdp_sys_mem1_size = SYS_MEM1_SIZE;
unsigned int sdp_sys_mem2_size = SYS_MEM2_SIZE;
unsigned int sdp_mach_mem0_size = MACH_MEM0_SIZE;
unsigned int sdp_mach_mem1_size = MACH_MEM1_SIZE;
unsigned int sdp_mach_mem2_size = MACH_MEM2_SIZE;
EXPORT_SYMBOL(sdp_sys_mem0_size);
EXPORT_SYMBOL(sdp_sys_mem1_size);
EXPORT_SYMBOL(sdp_sys_mem2_size);

extern void sdp_init_irq(void); 
extern struct sys_timer sdp_timer; 
extern void sdp1202_init(void);
extern void sdp1202_iomap_init(void);

unsigned int sdp_revision_id = 0;
EXPORT_SYMBOL(sdp_revision_id);

bool sdp_is_dual_mp;

#ifdef CONFIG_USE_EXT_GIC
unsigned int gic_bank_offset __read_mostly;
#endif

#define MEM_INFO_MAGIC	"SamsungCCEP_memi"

struct sdp_mem_param_t {
	char magic[16];
	u32  sys_mem0_size;
	u32  mach_mem0_size;
	u32  sys_mem1_size; 
	u32  mach_mem1_size;
	u32  sys_mem2_size; 
	u32  mach_mem2_size;
};

static struct sdp_mem_param_t * gp_mem_param __initdata =
		(struct sdp_mem_param_t*) (0xC0000000 + 0xA00);
static char mem_info_magic[] __initdata = MEM_INFO_MAGIC;

static void __init sdp_gic_init_irq(void)
{ 
#ifdef CONFIG_ARM_GIC
	gic_init(0, 29, (void __iomem *) VA_GIC_DIST_BASE, (void __iomem *) VA_GIC_CPU_BASE);
#ifdef CONFIG_USE_EXT_GIC
	gic_bank_offset = 0x8000;
#endif
#endif	
}

static void __init foxap_map_io(void)
{
	//initialize iomap of special function register address
	sdp1202_iomap_init();
}

static void __init foxap_init(void)
{
	sdp1202_init();
}

static void __init 
foxap_fixup(struct machine_desc *desc, struct tag *tags, char ** cmdline, struct meminfo *minfo)
{
	int nbank = 0;
	int chan_num = 0;
	
	if(memcmp(gp_mem_param->magic, mem_info_magic, 15) == 0){
		sdp_sys_mem0_size = gp_mem_param->sys_mem0_size << 20;
		sdp_mach_mem0_size = gp_mem_param->mach_mem0_size << 20;
		sdp_sys_mem1_size = gp_mem_param->sys_mem1_size << 20;
		sdp_mach_mem1_size = gp_mem_param->mach_mem1_size << 20;
		sdp_sys_mem2_size = gp_mem_param->sys_mem2_size << 20;
		sdp_mach_mem2_size = gp_mem_param->mach_mem2_size << 20;
		if(gp_mem_param->magic[15] == 'i')
			sdp_revision_id = 0;
		else
			sdp_revision_id = (u32) (gp_mem_param->magic[15] - '0');
	}
	else{
		printk(KERN_INFO"[%s] Set Default memory configruation\n",__FUNCTION__);
	}

	/* set bank0 */
	if((MACH_MEM0_BASE <= PHYS_OFFSET) && ((MACH_MEM0_BASE+sdp_mach_mem0_size) > PHYS_OFFSET)) {
		if((PHYS_OFFSET+sdp_sys_mem0_size) <= (MACH_MEM0_BASE+sdp_mach_mem0_size)) {
			chan_num = 0;
			minfo->bank[nbank].start = PHYS_OFFSET;
			minfo->bank[nbank].size =  sdp_sys_mem0_size;
			nbank++;
		} else {
			printk(KERN_ERR"[%s] system memory size(0x%08x) is too big!!\n",__FUNCTION__, (u32) PHYS_OFFSET);
			BUG();
		}
	} else if(MACH_MEM1_BASE <= PHYS_OFFSET && (MACH_MEM1_BASE+sdp_mach_mem1_size) > PHYS_OFFSET) {
		if((PHYS_OFFSET+sdp_sys_mem1_size) <= (MACH_MEM1_BASE+sdp_mach_mem1_size)) {
			chan_num = 1;
			minfo->bank[nbank].start = PHYS_OFFSET;
			minfo->bank[nbank].size =  sdp_sys_mem1_size;
			nbank++;
		} else {
			printk(KERN_ERR"[%s] system memory size(0x%08x) is too big!!\n",__FUNCTION__, (u32) PHYS_OFFSET);
			BUG();
		}
	} else if(MACH_MEM2_BASE <= PHYS_OFFSET && (MACH_MEM2_BASE+sdp_mach_mem2_size) > PHYS_OFFSET) {
		if((PHYS_OFFSET+sdp_sys_mem2_size) <= (MACH_MEM2_BASE+sdp_mach_mem2_size)) {
			chan_num = 2;
			minfo->bank[nbank].start = PHYS_OFFSET;
			minfo->bank[nbank].size =  sdp_sys_mem2_size;
			nbank++;
		} else {
			printk(KERN_ERR"[%s] system memory size(0x%08x) is too big!!\n",__FUNCTION__, (u32) PHYS_OFFSET);
			BUG();
		}
	} else {
		printk(KERN_ERR"[%s] PHYS_OFFSET(0x%08x) is not on memory!\n",__FUNCTION__, (u32) PHYS_OFFSET);
		BUG();
	}

//	printk(KERN_INFO"[%s] bank0: 0x%08x++0x%08x\n",__FUNCTION__, minfo->bank[0].start, minfo->bank[0].size);


	/* set bank1~2 */
#ifdef CONFIG_FLATMEM
	if(chan_num != 0)
		sdp_sys_mem0_size = 0;

	if(chan_num != 1)
		sdp_sys_mem1_size = 0;

	if(chan_num != 2)
		sdp_sys_mem2_size = 0;
#else
	if ((sdp_sys_mem0_size != 0) && (chan_num != 0))
	{
		minfo->bank[nbank].start = MACH_MEM0_BASE;
		minfo->bank[nbank].size =  sdp_sys_mem0_size;
		nbank++;
	}

	if((sdp_sys_mem1_size != 0) && (chan_num != 1))
	{
		minfo->bank[nbank].start = MACH_MEM1_BASE;
		minfo->bank[nbank].size =  sdp_sys_mem1_size;
		nbank++;
	}

	if((sdp_sys_mem2_size != 0) && (chan_num != 2))
	{
		minfo->bank[nbank].start = MACH_MEM2_BASE;
		minfo->bank[nbank].size =  sdp_sys_mem2_size;
		nbank++;
	}
#endif
	minfo->nr_banks = nbank;

	for(chan_num = 0; chan_num < minfo->nr_banks; chan_num++) {
		printk(KERN_DEBUG"[%s] bank%d: 0x%08x++0x%08x\n",__FUNCTION__, chan_num, minfo->bank[chan_num].start, (u32) minfo->bank[chan_num].size);
	}

	printk("Board Memory : %dMB(%dMB+%dMB+%dMB), Kernel Memory : %dMB(%dMB+%dMB+%dMB)\n"
			, (sdp_mach_mem0_size + sdp_mach_mem1_size + sdp_mach_mem2_size) >> 20
			, sdp_mach_mem0_size >> 20, sdp_mach_mem1_size >> 20, sdp_mach_mem2_size >> 20
			, (sdp_sys_mem0_size + sdp_sys_mem1_size + sdp_sys_mem2_size) >> 20
			, sdp_sys_mem0_size >> 20, sdp_sys_mem1_size >> 20, sdp_sys_mem2_size >> 20);

}

unsigned int sdp_get_mem_cfg(int nType)
{
	switch(nType)
	{
		case 0:
			return sdp_sys_mem0_size;
		case 1:
			return sdp_mach_mem0_size;
		case 2:
			return sdp_sys_mem1_size;
		case 3:
			return sdp_mach_mem1_size;
		case 4:
			return sdp_sys_mem2_size;
		case 5:
			return sdp_mach_mem2_size;
		default:
			return (u32) -1;
	}
	return (u32) -1;
}

EXPORT_SYMBOL(sdp_get_mem_cfg);

MACHINE_START(SDP1202_FOXAP, "Samsung SDP1202 evaluation board")
#ifndef CONFIG_ARM_PATCH_PHYS_VIRT
	.boot_params  = PHYS_OFFSET + 0x100,
#else
	.boot_params  = 0x100,
#endif
	.map_io		= foxap_map_io,
	.init_irq	= sdp_gic_init_irq,
	.timer		= &sdp_timer,
	.init_machine	= foxap_init,
	.fixup		= foxap_fixup,
MACHINE_END
