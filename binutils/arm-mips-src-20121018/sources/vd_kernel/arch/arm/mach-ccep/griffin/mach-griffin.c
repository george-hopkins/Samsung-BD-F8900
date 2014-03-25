/*
 * mach-griffin.c
 *
 * Copyright (C) 2010 Samsung Electronics.co
 * seungjun.heo <seungjun.heo@samsung.com>
 */
/*
 * Jul,14,2010 : changeable kernel mem size from boot args
 */

#include <linux/init.h>
#include <linux/device.h>
#include <linux/dma-mapping.h>

#include <asm/mach-types.h>
#include <asm/mach/map.h>

#include <asm/setup.h>
#include <asm/mach/arch.h>

#include <mach/hardware.h>

unsigned int sdp_sys_mem0_size = SYS_MEM0_SIZE;
unsigned int sdp_sys_mem1_size = SYS_MEM1_SIZE;
unsigned int sdp_mach_mem0_size = MACH_MEM0_SIZE;
unsigned int sdp_mach_mem1_size = MACH_MEM1_SIZE;
EXPORT_SYMBOL(sdp_sys_mem0_size);

const unsigned int sdp_vmalloc_end = 0xFE000000;

EXPORT_SYMBOL(sdp_vmalloc_end);

extern void sdp_init_irq(void); 
extern struct sys_timer sdp_timer; 
extern void sdp1001_init(void);
extern void sdp1001_iomap_init(void);

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
		(struct sdp_mem_param_t*) (PHYS_OFFSET + 0xA00);
static char mem_info_magic[] __initdata = MEM_INFO_MAGIC;

#if 0
static struct map_desc griffin_io_desc[] __initdata = {
{ 
	.virtual = MACH_DTVSUB0_BASE,
	.pfn     = __phys_to_pfn(MACH_MEM0_BASE + MACH_MEM0_SIZE - MACH_DTVSUB0_SIZE),  
	.length  = MACH_DTVSUB0_SIZE,
	.type    = MT_DEVICE 
},
{ 
	.virtual = MACH_DTVSUB1_BASE,
	.pfn     = __phys_to_pfn(MACH_MEM1_BASE + SYS_MEM1_SIZE),  
	.length  = MACH_DTVSUB1_SIZE,
	.type    = MT_DEVICE 
},
};

static void __init griffin_iomap_init(void)
{
//	iotable_init(griffin_io_desc, ARRAY_SIZE(griffin_io_desc));
}
#endif

static void __init griffin_map_io(void)
{
	//initialize iomap of special function register address
	sdp1001_iomap_init();

//	griffin_io_desc[0].pfn = 
//		__phys_to_pfn(MACH_MEM0_BASE + sdp_mach_mem0_size - MACH_DTVSUB0_SIZE);
//	griffin_io_desc[1].pfn = 
//		__phys_to_pfn(MACH_MEM1_BASE + sdp_sys_mem1_size);

	//initialize iomap for peripheral device
//	griffin_iomap_init();

//  interrupt 29 all clear[0:7] & mask[15:8]
	*(volatile u32*)(VA_IO_BASE0 + 0x00091400) = 0xFFFF;
}

static void __init griffin_init(void)
{
	sdp1001_init();
}

static void __init 
griffin_fixup(struct machine_desc *mdesc, struct tag *tags, char ** cmdline, struct meminfo *meminfo)
{
	if(memcmp(gp_mem_param->magic, mem_info_magic, 15) == 0){
		sdp_sys_mem0_size = gp_mem_param->sys_mem0_size << 20;
		sdp_mach_mem0_size = gp_mem_param->mach_mem0_size << 20;
		sdp_sys_mem1_size = gp_mem_param->sys_mem1_size << 20;
		sdp_mach_mem1_size = gp_mem_param->mach_mem1_size << 20;
		if(gp_mem_param->magic[15] == 'i')
			sdp_revision_id = 0;
		else
			sdp_revision_id = gp_mem_param->magic[15] - '0';
	}
	else{
		printk(KERN_INFO"[%s] Set Default memory configruation\n",__FUNCTION__);
	}

	meminfo->nr_banks = 1;

	meminfo->bank[0].start = MACH_MEM0_BASE;
	meminfo->bank[0].size =  sdp_sys_mem0_size;
	meminfo->bank[0].node =  0;

#if defined(CONFIG_SDP_DISCONTIGMEM_SUPPORT) || defined(CONFIG_SDP_SPARSEMEM_SUPPORT)
	meminfo->bank[1].start = MACH_MEM1_BASE;
	meminfo->bank[1].size =  sdp_sys_mem1_size;
#ifndef CONFIG_SDP_SPARSEMEM_SUPPORT
	meminfo->bank[1].node =  1;
#endif

	if(meminfo->bank[1].size == 0)
		meminfo->nr_banks = 1;
	else
		meminfo->nr_banks = 2;

	printk("Board Memory : %dMB(%dMB+%dMB), Kernel Memory : %dMB(%dMB+%dMB)\n"
		, (sdp_mach_mem0_size + sdp_mach_mem1_size) >> 20, sdp_mach_mem0_size >> 20, sdp_mach_mem1_size >> 20
		, (sdp_sys_mem0_size + sdp_sys_mem1_size) >> 20, sdp_sys_mem0_size >> 20, sdp_sys_mem1_size >> 20);
#elif defined(CONFIG_SDP_SINGLE_DDR_B)
	meminfo->bank[0].start = MACH_MEM1_BASE;
	meminfo->bank[0].size =  sdp_sys_mem1_size;
	meminfo->bank[0].node =  0;

	printk("Board Memory : DDR-B, 0x80000000\n");
#endif

}

int sdp_get_mem_cfg(int nType)
{
	switch(nType)
	{
		case 0:
			return sdp_sys_mem0_size;
		case 1:
			return sdp_sys_mem1_size;
		case 2:
			return sdp_mach_mem0_size;
		case 3:
			return sdp_mach_mem1_size;
	}
	return -1;
}

EXPORT_SYMBOL(sdp_get_mem_cfg);

MACHINE_START(SDP1001_GRIFFIN, "Samsung SDP1001 Evaluation board")
	.phys_io  = PA_IO_BASE0,
	.io_pg_offst = VA_IO_BASE0,
	.boot_params  = (PHYS_OFFSET + 0x100),
	.map_io		= griffin_map_io,
	.init_irq	= sdp_init_irq,
	.timer		= &sdp_timer,
	.init_machine	= griffin_init,
	.fixup		= griffin_fixup,
MACHINE_END

