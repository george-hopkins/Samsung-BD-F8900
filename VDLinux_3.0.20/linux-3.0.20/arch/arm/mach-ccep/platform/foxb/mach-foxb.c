/*
 * mach-foxb.c
 *
 * Copyright (C) 2011 Samsung Electronics.co
 * SeungJun Heo <seungjun.heo@samsung.com>
 * Seihee Chon <sh.chon@samsung.com>
 * Sola Lee <sssol.lee@samsung.com>
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

#include <mach/hardware.h>

#ifdef CONFIG_CMA
#include <linux/cma.h>
#include <linux/hdma.h>
#endif

unsigned int sdp_sys_mem0_size = SYS_MEM0_SIZE;
unsigned int sdp_sys_mem1_size = SYS_MEM1_SIZE;
unsigned int sdp_mach_mem0_size = MACH_MEM0_SIZE;
unsigned int sdp_mach_mem1_size = MACH_MEM1_SIZE;
EXPORT_SYMBOL(sdp_sys_mem0_size);
EXPORT_SYMBOL(sdp_sys_mem1_size);


const unsigned int sdp_vmalloc_end = 0xFE000000;

EXPORT_SYMBOL(sdp_vmalloc_end);

extern void sdp_init_irq(void); 
extern struct sys_timer sdp_timer; 
extern void sdp1207_init(void);
extern void sdp1207_iomap_init(void);

unsigned int sdp_revision_id = 0;

#define MEM_INFO_MAGIC	"SamsungCCEP_memi"

struct sdp_mem_param_t {
	char magic[16];
	u32  sys_mem0_size;
	u32  mach_mem0_size;
	u32  sys_mem1_size; 
	u32  mach_mem1_size;
};

static struct sdp_mem_param_t * gp_mem_param __initdata =
		(struct sdp_mem_param_t*) (0xC0000000 + 0xA00);
static char mem_info_magic[] __initdata = MEM_INFO_MAGIC;

static void __init sdp_gic_init_irq(void)
{ 
#ifdef CONFIG_ARM_GIC
	gic_init(0, IRQ_LOCALTIMER, (void __iomem *) VA_GIC_DIST_BASE, (void __iomem *) VA_GIC_CPU_BASE);
#endif	
}

static void __init foxb_map_io(void)
{
	//initialize iomap of special function register address
	sdp1207_iomap_init();
	}

static void __init foxb_init(void)
{
	sdp1207_init();
}

static void __init 
foxb_fixup(struct machine_desc *mdesc, struct tag *tags, char ** cmdline, struct meminfo *minfo)
{
	int nbank = 0;
	
	if(memcmp(gp_mem_param->magic, mem_info_magic, 15) == 0){
		sdp_sys_mem0_size = gp_mem_param->sys_mem0_size << 20;
		sdp_mach_mem0_size = gp_mem_param->mach_mem0_size << 20;
		sdp_sys_mem1_size = gp_mem_param->sys_mem1_size << 20;
		sdp_mach_mem1_size = gp_mem_param->mach_mem1_size << 20;
		if(gp_mem_param->magic[15] == 'i')
			sdp_revision_id = 0;
		else
			sdp_revision_id = (u32) (gp_mem_param->magic[15] - '0');
	}
	else{
		printk(KERN_INFO"[%s] Set Default memory configruation\n",__FUNCTION__);
	}

	if (sdp_sys_mem0_size != 0)
	{
		minfo->bank[nbank].start = MACH_MEM0_BASE;
		minfo->bank[nbank].size =  sdp_sys_mem0_size;
		nbank++;
	}

	if(sdp_sys_mem1_size != 0)
	{
		minfo->bank[nbank].start = MACH_MEM1_BASE;
		minfo->bank[nbank].size =  sdp_sys_mem1_size;
		nbank++;
	}

	minfo->nr_banks = nbank;

	printk("Board Memory : %dMB(%dMB+%dMB), Kernel Memory : %dMB(%dMB+%dMB)\n"
		, (sdp_mach_mem0_size + sdp_mach_mem1_size) >> 20
		, sdp_mach_mem0_size >> 20, sdp_mach_mem1_size >> 20 
		, (sdp_sys_mem0_size + sdp_sys_mem1_size ) >> 20
		, sdp_sys_mem0_size >> 20, sdp_sys_mem1_size >> 20);

}

#ifdef CONFIG_HDMA
extern struct cmainfo hdmainfo;
extern struct meminfo meminfo;
#define MAX_OF_BANK 5
unsigned long hdma_size_of_bank[MAX_OF_BANK];
#endif

unsigned int sdp_get_mem_cfg(int nType)
{
	switch(nType)
	{
		case 0:
#ifdef CONFIG_HDMA
			return sdp_sys_mem0_size - hdma_size_of_bank[0];
#else
			return sdp_sys_mem0_size;
#endif
		case 1:
			return sdp_mach_mem0_size;
		case 2:
#ifdef CONFIG_HDMA
			return sdp_sys_mem1_size - hdma_size_of_bank[1];
#else
			return sdp_sys_mem1_size;
#endif
		case 3:
			return sdp_mach_mem1_size;
		default:
			return (u32) -1;
	}
	return (u32) -1;
}

EXPORT_SYMBOL(sdp_get_mem_cfg);

#ifdef CONFIG_CMA
static void __init foxb_reserve(void)
{
#ifdef CONFIG_HDMA
        int i,j;
#endif
        cma_regions_reserve();
#ifdef CONFIG_HDMA
        hdma_regions_reserve();

        for(i=0; i<meminfo.nr_banks; i++) {
        for(j=0; j<hdmainfo.nr_regions; j++) {
                if((meminfo.bank[i].start <= hdmainfo.region[j].start) &&
                        ((meminfo.bank[i].start + meminfo.bank[i].size) >=
                         (hdmainfo.region[j].start + hdmainfo.region[j].size)))
                        hdma_size_of_bank[i] += hdmainfo.region[j].size;
                }
        }
#endif
}
#endif

MACHINE_START(SDP1207_FOXB, "Samsung SDP1207 evaluation board")
	.boot_params  = (PHYS_OFFSET + 0x100),
	.map_io		= foxb_map_io,
#ifdef CONFIG_ARM_GIC
	.init_irq	= sdp_gic_init_irq,
#else
	.init_irq	= sdp_init_irq,
#endif
	.timer		= &sdp_timer,
	.init_machine	= foxb_init,
	.fixup		= foxb_fixup,
#ifdef CONFIG_CMA
        .reserve        = foxb_reserve,
#endif
MACHINE_END
