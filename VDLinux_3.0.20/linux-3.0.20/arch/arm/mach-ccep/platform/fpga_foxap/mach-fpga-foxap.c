/*
 * mach-fpga-foxap.c
 *
 * Copyright (C) 2011 Samsung Electronics.co
 * SeungJun Heo <sh.chon@samsung.com>
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
extern void sdp1202fpga_init(void);
extern void sdp1202fpga_iomap_init(void);

unsigned int sdp_revision_id = 0;

#ifdef CONFIG_USE_EXT_GIC
unsigned int gic_bank_offset __read_mostly;

struct gic_chip_data {
	unsigned int irq_offset;
	void __iomem *dist_base;
	void __iomem *cpu_base;
};

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

void __init sdp_gic_init_irq(void)
{
#ifdef CONFIG_ARM_GIC
	gic_init(0, IRQ_LOCALTIMER, (void __iomem *) VA_GIC_DIST_BASE, (void __iomem *) VA_GIC_CPU_BASE);
#ifdef CONFIG_USE_EXT_GIC
	gic_bank_offset = 0x8000;
#endif
#endif	
}

static void __init fpga_foxap_map_io(void)
{
	//initialize iomap of special function register address
	sdp1202fpga_iomap_init();
}

static void __init fpga_foxap_init(void)
{
	sdp1202fpga_init();
}

static void __init 
fpga_foxap_fixup(struct machine_desc *desc, struct tag *tags, char ** cmdline, struct meminfo *meminfo)
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
#ifdef CONFIG_SDP_SINGLE_DDR_B
		sdp_sys_mem0_size = 0;
		sdp_sys_mem2_size = 0;
#endif
#ifdef CONFIG_SDP_SINGLE_DDR_C
		sdp_sys_mem0_size = 0;
		sdp_sys_mem1_size = 0;
#endif

	if(sdp_sys_mem0_size != 0)
	{
		meminfo->bank[nbank].start = MACH_MEM0_BASE;
		meminfo->bank[nbank].size =  sdp_sys_mem0_size;
		nbank++;
	}

	if(sdp_sys_mem1_size != 0)
	{
		meminfo->bank[nbank].start = MACH_MEM1_BASE;
		meminfo->bank[nbank].size =  sdp_sys_mem1_size;
		nbank++;
	}

	if(sdp_sys_mem2_size != 0)
	{
		meminfo->bank[nbank].start = MACH_MEM2_BASE;
		meminfo->bank[nbank].size =  sdp_sys_mem2_size;
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

MACHINE_START(SDP1202FPGA_FOXAP, "Samsung SDP1202 FPGA board")
	.boot_params  = PHYS_OFFSET + 0x100,
	.map_io		= fpga_foxap_map_io,
	.init_irq	= sdp_gic_init_irq,
	.timer		= &sdp_timer,
	.init_machine	= fpga_foxap_init,
	.fixup		= fpga_foxap_fixup,
MACHINE_END

