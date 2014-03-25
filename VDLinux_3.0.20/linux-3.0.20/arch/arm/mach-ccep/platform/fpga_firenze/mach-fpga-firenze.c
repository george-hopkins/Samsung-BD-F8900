/*
 * mach-firenze.c
 *
 * Copyright (C) 2010 Samsung Electronics.co
 * seungjun.heo <tukho.kim@samsung.com>
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
unsigned int sdp_sys_mem2_size = 0;
unsigned int sdp_mach_mem0_size = MACH_MEM0_SIZE;
unsigned int sdp_mach_mem1_size = MACH_MEM1_SIZE;
unsigned int sdp_mach_mem2_size = 0;
EXPORT_SYMBOL(sdp_sys_mem0_size);
EXPORT_SYMBOL(sdp_sys_mem1_size);

const unsigned int sdp_vmalloc_end = 0xE8000000;

EXPORT_SYMBOL(sdp_vmalloc_end);

extern void sdp_init_irq(void); 
extern struct sys_timer sdp_timer; 
extern void sdp1004fpga_init(void);
extern void sdp1004fpga_iomap_init(void);

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

#if 1
static struct map_desc fpga_firenze_io_desc[] __initdata = {
{ 
	.virtual = MACH_BDSUB0_BASE,
	.pfn     = __phys_to_pfn(MACH_MEM0_BASE + SYS_MEM0_SIZE),  
	.length  = MACH_MEM0_SIZE - SYS_MEM0_SIZE,
	.type    = MT_DEVICE 
},
{ 
	.virtual = MACH_BDSUB1_BASE,
	.pfn     = __phys_to_pfn(MACH_MEM1_BASE + SYS_MEM1_SIZE),  
	.length  = MACH_MEM1_SIZE - SYS_MEM1_SIZE,
	.type    = MT_DEVICE 
},
};

static void __init fpga_firenze_iomap_init(void)
{
	iotable_init(fpga_firenze_io_desc, ARRAY_SIZE(fpga_firenze_io_desc));
}
#endif

static void __init fpga_firenze_map_io(void)
{
	//initialize iomap of special function register address
	sdp1004fpga_iomap_init();

	fpga_firenze_io_desc[0].pfn = 
		__phys_to_pfn(MACH_MEM0_BASE + sdp_sys_mem0_size);
	fpga_firenze_io_desc[0].length = (sdp_mach_mem0_size - sdp_sys_mem0_size);

	fpga_firenze_io_desc[1].pfn = 
		__phys_to_pfn(MACH_MEM1_BASE + sdp_sys_mem1_size);
	fpga_firenze_io_desc[1].length = (sdp_mach_mem1_size - sdp_sys_mem1_size);

	if(fpga_firenze_io_desc[1].length > (VA_IO_BASE0 - MACH_BDSUB1_BASE)) {
		printk("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		printk("!! BD SUB1 Size is too big %dMB -> %dMB !!\n", 
		fpga_firenze_io_desc[1].length >> 20, (VA_IO_BASE0 - MACH_BDSUB1_BASE) >> 20);
		printk("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		fpga_firenze_io_desc[1].length = VA_IO_BASE0 - MACH_BDSUB1_BASE;
	}


	printk("Board H/w memory Map: \n");
	printk("CH. A: 0x%08x -> 0x%08x, size:0x%08x\n", 
			MACH_MEM0_BASE + sdp_sys_mem0_size, 
			MACH_BDSUB0_BASE, 
			fpga_firenze_io_desc[0].length);
	printk("CH. B: 0x%08x -> 0x%08x, size:0x%08x\n", 
			MACH_MEM1_BASE + sdp_sys_mem1_size, 
			MACH_BDSUB1_BASE, 
			fpga_firenze_io_desc[1].length);

	//initialize iomap for peripheral device
	fpga_firenze_iomap_init();
}

static void __init fpga_firenze_init(void)
{
	sdp1004fpga_init();
#if 0
/* INTC64 bit configuration */
#ifdef CONFIG_SDP_INTC64
	printk("SDP1004 Interrupt 64 mode\n");
	*(volatile u32*)(VA_IO_BASE0 + 0x000D0014) = 0xFFFFFFFF;
#else
	printk("SDP1004 Interrupt 32 mode\n");
	*(volatile u32*)(VA_IO_BASE0 + 0x000D0014) = 0x0;
#endif

/* USB initialize */
// Phy tune
	*(volatile u32*)(VA_IO_BASE0 + 0x00090CE0) = 0x3385DC92;
	*(volatile u32*)(VA_IO_BASE0 + 0x00090CE4) = 0x3385DC92;
// Bufffer threshold
	*(volatile u32*)(VA_IO_BASE0 + 0x00070094) = 0x00800020;
	*(volatile u32*)(VA_IO_BASE0 + 0x00080094) = 0x00800020;
// Burst Enable
	*(volatile u32*)(VA_IO_BASE0 + 0x000D0010) = 0x000000FF;
// USB port volatage enable	// GPIO PORT 6.[3:4] pin
	*(volatile u32*)(VA_IO_BASE0 + 0x00090C78) |= 0x00033000;
	*(volatile u32*)(VA_IO_BASE0 + 0x00090C7C) |= 0x00000018;

/* Ext interrupt initialize */
	*(volatile u32*)(VA_IO_BASE0 + 0x00090CB0) = 0x00FFFE00;
	*(volatile u32*)(VA_IO_BASE0 + 0x00090CB4) = 0x00000000;	
	*(volatile u32*)(VA_IO_BASE0 + 0x00090F94) = 0xFFFFFFFF;	// sub interrupt clear
#endif
}

static void __init 
fpga_firenze_fixup(struct machine_desc *mdesc, struct tag *tags, char ** cmdline, struct meminfo *meminfo)
{
	if(memcmp(gp_mem_param->magic, mem_info_magic, 15) == 0){
		sdp_sys_mem0_size = gp_mem_param->sys_mem0_size << 20;
		sdp_mach_mem0_size = gp_mem_param->mach_mem0_size << 20;
		sdp_sys_mem1_size = gp_mem_param->sys_mem1_size << 20;
		sdp_mach_mem1_size = gp_mem_param->mach_mem1_size << 20;
		sdp_sys_mem2_size = gp_mem_param->sys_mem2_size << 20;
		sdp_mach_mem2_size = gp_mem_param->mach_mem2_size << 20;
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

#define SIZE_256MB (256 << 20)
	if(sdp_mach_mem2_size <= SIZE_256MB) {
		if(sdp_sys_mem2_size && (sdp_sys_mem2_size <= sdp_mach_mem2_size)){
			meminfo->bank[2].start = MACH_MEM2_BASE;
			meminfo->bank[2].size =  sdp_sys_mem2_size;
			meminfo->bank[2].node =  2;

			meminfo->nr_banks = 3;
		}
	}

	if(meminfo->nr_banks == 2) {
		printk("Board Memory : %dMB(%dMB+%dMB), Kernel Memory : %dMB(%dMB+%dMB)\n"
			, (sdp_mach_mem0_size + sdp_mach_mem1_size) >> 20, sdp_mach_mem0_size >> 20, sdp_mach_mem1_size >> 20
			, (sdp_sys_mem0_size + sdp_sys_mem1_size) >> 20, sdp_sys_mem0_size >> 20, sdp_sys_mem1_size >> 20);
	}
	else {
		printk("Board Memory : %dMB(%dMB+%dMB+%dMB), Kernel Memory : %dMB(%dMB+%dMB+%dMB)\n"
			, (sdp_mach_mem0_size + sdp_mach_mem1_size + sdp_mach_mem2_size) >> 20
			, sdp_mach_mem0_size >> 20, sdp_mach_mem1_size >> 20, sdp_mach_mem2_size >> 20
			, (sdp_sys_mem0_size + sdp_sys_mem1_size + sdp_sys_mem2_size) >> 20
			, sdp_sys_mem0_size >> 20, sdp_sys_mem1_size >> 20, sdp_sys_mem2_size >> 20);
	}

	
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

MACHINE_START(SDP1004FPGA_FIRENZE, "Samsung SDP1004 FPGA board")
	.phys_io  = PA_IO_BASE0,
	.io_pg_offst = VA_IO_BASE0,
	.boot_params  = (PHYS_OFFSET + 0x100),
	.map_io		= fpga_firenze_map_io,
	.init_irq	= sdp_init_irq,
	.timer		= &sdp_timer,
	.init_machine	= fpga_firenze_init,
	.fixup		= fpga_firenze_fixup,
MACHINE_END

