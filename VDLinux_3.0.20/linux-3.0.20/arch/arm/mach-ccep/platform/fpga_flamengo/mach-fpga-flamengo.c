/*
 * mach-fpga-flamengo.c
 *
 * Copyright (C) 2011 Samsung Electronics.co
 * SeungJun Heo <seungjun.heo@samsung.com>
 */
/*
 * Jul,14,2010 : changeable kernel mem size from boot args
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

unsigned int sdp_sys_mem0_size = SYS_MEM0_SIZE;
unsigned int sdp_sys_mem1_size = SYS_MEM1_SIZE;
unsigned int sdp_sys_mem2_size = SYS_MEM2_SIZE;
unsigned int sdp_mach_mem0_size = MACH_MEM0_SIZE;
unsigned int sdp_mach_mem1_size = MACH_MEM1_SIZE;
unsigned int sdp_mach_mem2_size = MACH_MEM2_SIZE;
EXPORT_SYMBOL(sdp_sys_mem0_size);
EXPORT_SYMBOL(sdp_sys_mem1_size);

const unsigned int sdp_vmalloc_end = 0xFE000000;

EXPORT_SYMBOL(sdp_vmalloc_end);

extern void sdp_init_irq(void); 
extern struct sys_timer sdp_timer; 
extern void sdp1106fpga_init(void);
extern void sdp1106fpga_iomap_init(void);

unsigned int sdp_revision_id = 0;

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
		(struct sdp_mem_param_t*) (PHYS_OFFSET + 0xA00);
static char mem_info_magic[] __initdata = MEM_INFO_MAGIC;

#if 0
static struct map_desc fpga_flamengo_io_desc[] __initdata = {
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

static void __init fpga_flamengo_iomap_init(void)
{
//	iotable_init(fpga_flamengo_io_desc, ARRAY_SIZE(fpga_flamengo_io_desc));
}
#endif

void __init sdp_gic_init_irq(void)
{
//	gic_cpu_base_addr = VA_GIC_CPU_BASE;
	gic_dist_init(0, (void __iomem *) VA_GIC_DIST_BASE, IRQ_LOCALTIMER);
	gic_cpu_init(0, (void __iomem *) VA_GIC_CPU_BASE);
}

static void __init fpga_flamengo_map_io(void)
{
	//initialize iomap of special function register address
	sdp1106fpga_iomap_init();

//	flamengo_io_desc[0].pfn = 
//		__phys_to_pfn(MACH_MEM0_BASE + sdp_mach_mem0_size - MACH_DTVSUB0_SIZE);
//	flamengo_io_desc[1].pfn = 
//		__phys_to_pfn(MACH_MEM1_BASE + sdp_sys_mem1_size);

	//initialize iomap for peripheral device
//	flamengo_iomap_init();

//  interrupt 29 all clear[0:7] & mask[15:8]
	*(volatile u32*)(VA_IO_BASE0 + 0x00091400) = 0xFFFF;
}

static void __init fpga_flamengo_init(void)
{
	sdp1106fpga_init();
}

static void __init 
fpga_flamengo_fixup(struct machine_desc *mdesc, struct tag *tags, char ** cmdline, struct meminfo *meminfo)
{
	int nbank = 0; 
	
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
			sdp_revision_id = gp_mem_param->magic[15] - '0';
	}
	else{
		printk(KERN_INFO"[%s] Set Default memory configruation\n",__FUNCTION__);
	}

	if(sdp_sys_mem0_size != 0)
	{
		meminfo->bank[nbank].start = MACH_MEM0_BASE;
		meminfo->bank[nbank].size =  sdp_sys_mem0_size;
#ifdef CONFIG_SDP_DISCONTIGMEM_SUPPORT
		meminfo->bank[nbank].node =  nbank;
#else
		meminfo->bank[nbank].node =  0;
#endif
		nbank++;
	}

	if(sdp_sys_mem1_size != 0)
	{
		meminfo->bank[nbank].start = MACH_MEM1_BASE;
		meminfo->bank[nbank].size =  sdp_sys_mem1_size;
#ifdef CONFIG_SDP_DISCONTIGMEM_SUPPORT
		meminfo->bank[nbank].node =  nbank;
#else
		meminfo->bank[nbank].node =  0;
#endif
		nbank++;
	}

	if(sdp_sys_mem2_size != 0)
	{
		meminfo->bank[nbank].start = MACH_MEM2_BASE;
		meminfo->bank[nbank].size =  sdp_sys_mem2_size;
#ifdef CONFIG_SDP_DISCONTIGMEM_SUPPORT
		meminfo->bank[nbank].node =  nbank;
#else
		meminfo->bank[nbank].node =  0;
#endif
		nbank++;
	}
	meminfo->nr_banks = nbank;

	printk("Board Memory : %dMB(%dMB+%dMB+%dMB), Kernel Memory : %dMB(%dMB+%dMB+%dMB)\n"
		, (sdp_mach_mem0_size + sdp_mach_mem1_size + sdp_mach_mem2_size) >> 20
		, sdp_mach_mem0_size >> 20, sdp_mach_mem1_size >> 20, sdp_mach_mem2_size >> 20
		, (sdp_sys_mem0_size + sdp_sys_mem1_size + sdp_sys_mem2_size) >> 20
		, sdp_sys_mem0_size >> 20, sdp_sys_mem1_size >> 20, sdp_sys_mem2_size >> 20);
}

int sdp_get_mem_cfg(int nType)
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
	}
	return -1;
}

EXPORT_SYMBOL(sdp_get_mem_cfg);

MACHINE_START(SDP1106FPGA_FLAMENGO, "Samsung SDP1106 FPGA board")
	.phys_io  = PA_IO_BASE0,
	.io_pg_offst = VA_IO_BASE0,
	.boot_params  = (PHYS_OFFSET + 0x100),
	.map_io		= fpga_flamengo_map_io,
	.init_irq	= sdp_gic_init_irq,
	.timer		= &sdp_timer,
	.init_machine	= fpga_flamengo_init,
	.fixup		= fpga_flamengo_fixup,
MACHINE_END

