/*
 * mach-fpga_genoa.c
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

const unsigned int sdp_vmalloc_end = 0xF8000000;

EXPORT_SYMBOL(sdp_vmalloc_end);

extern void sdp_init_irq(void); 
extern struct sys_timer sdp_timer; 
extern void sdp1001fpga_init(void);
extern void sdp1001fpga_iomap_init(void);

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

static struct map_desc fpga_genoa_io_desc[] __initdata = {
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

static void __init fpga_genoa_iomap_init(void)
{
	iotable_init(fpga_genoa_io_desc, ARRAY_SIZE(fpga_genoa_io_desc));
}

static void __init fpga_genoa_map_io(void)
{
	//initialize iomap of special function register address
	sdp1001fpga_iomap_init();

	fpga_genoa_io_desc[0].pfn = 
		__phys_to_pfn(MACH_MEM0_BASE + sdp_mach_mem0_size - MACH_DTVSUB0_SIZE);
	fpga_genoa_io_desc[1].pfn = 
		__phys_to_pfn(MACH_MEM1_BASE + sdp_sys_mem1_size);

	//initialize iomap for peripheral device
	fpga_genoa_iomap_init();
}

static void __init fpga_genoa_init(void)
{
	sdp1001fpga_init();
}

static void __init 
fpga_genoa_fixup(struct machine_desc *mdesc, struct tag *tags, char ** cmdline, struct meminfo *meminfo)
{
	if(memcmp(gp_mem_param->magic, mem_info_magic, 16) == 0){
		sdp_sys_mem0_size = gp_mem_param->sys_mem0_size << 20;
		sdp_mach_mem0_size = gp_mem_param->mach_mem0_size << 20;
		sdp_sys_mem1_size = gp_mem_param->sys_mem1_size << 20;
		sdp_mach_mem1_size = gp_mem_param->mach_mem1_size << 20;
	}
	else{
		printk(KERN_INFO"[%s] Set Default memory configruation\n",__FUNCTION__);
	}

	meminfo->nr_banks = 1;

	meminfo->bank[0].start = MACH_MEM0_BASE;
	meminfo->bank[0].size =  sdp_sys_mem0_size;
	meminfo->bank[0].node =  0;

#if defined(CONFIG_SDP_DISCONTIGMEM_SUPPORT)
	meminfo->bank[1].start = MACH_MEM1_BASE;
	meminfo->bank[1].size =  sdp_sys_mem1_size;
	meminfo->bank[1].node =  1;

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

MACHINE_START(SDP1001FPGA_GENOA, "Samsung SDP1001 FPGA board")
	.phys_io  = PA_IO_BASE0,
	.io_pg_offst = VA_IO_BASE0,
	.boot_params  = (PHYS_OFFSET + 0x100),
	.map_io		= fpga_genoa_map_io,
	.init_irq	= sdp_init_irq,
	.timer		= &sdp_timer,
	.init_machine	= fpga_genoa_init,
	.fixup		= fpga_genoa_fixup,
MACHINE_END

