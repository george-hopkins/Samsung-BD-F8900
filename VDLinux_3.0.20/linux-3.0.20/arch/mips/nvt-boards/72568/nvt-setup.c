/*
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 */


#include <linux/cpu.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/ioport.h>
#include <linux/irq.h>
#include <linux/time.h>

#include <asm/bootinfo.h>
#include <asm/mips-boards/generic.h>
#include <asm/mips-boards/prom.h>
#include <linux/serial.h>
#include <linux/serial_core.h>
#include <linux/serial_8250.h>
#include <linux/spinlock.h>
#include <asm/dma.h>
#include <asm/traps.h>

#include <nvt-clk.h>
#include <nvt-sys.h>

const char *get_system_type(void)
{
	return "NVT NT72568";
}

const char display_string[] = "        LINUX ON NVT NT72568       ";
static void __init clk_init(void);
unsigned long hclk = 0;

extern unsigned int PCI_DMA_BUS_IS_PHYS;
DEFINE_RAW_SPINLOCK(externl_sync_lock);

void __init plat_mem_setup(void)
{
	clk_init ();

	/*
	 * make USB work under HIGHMEM
	 */ 
	PCI_DMA_BUS_IS_PHYS = 1;

}

static void __init clk_init(void)
{
#ifndef CONFIG_AHB_CLOCK
	unsigned int ratio;
	unsigned int temp;

	//ratio = SYS_CLK_GetClockSource(EN_SYS_CLK_SRC_AHB);
	ratio = AHB_PLL_SELECT;

	/* source is CPUPLL */
	if ((ratio == EN_SYS_AHB_CLK_SRC_MIPS_D6CK) || (ratio == EN_SYS_AHB_CLK_SRC_MIPS_D4CK)) 
	{      
		temp = SYS_CLK_GetMpll(EN_SYS_CLK_MPLL_MIPS);
		if (ratio == EN_SYS_AHB_CLK_SRC_MIPS_D6CK)
		{
			hclk = temp/6;
		}
		else //if (ratio == EN_SYS_AHB_CLK_SRC_MIPS_D4CK) 
		{
			hclk = temp/4;
		}
	}
	else if (ratio == EN_SYS_AHB_CLK_SRC_BODA_CK){
		/* source is BODAPLL */
		temp = SYS_CLK_GetMpll(EN_SYS_CLK_MPLL_MPG);

		hclk = temp/2;
	}
	else if (ratio == EN_SYS_AHB_CLK_SRC_AXI_CK){
		/* source is AXIPLL */
		temp = SYS_CLK_GetMpll(EN_SYS_CLK_MPLL_AXI);

		hclk = temp/4;
	}
	else // if (ratio >= EN_SYS_AHB_CLK_SRC_REF_96M)
	{
		/* source is reference clock 12M*16 */
		hclk = 12*16/2;
		hclk *= 1000000; // Hz
	}

#if 0
	///< Set VD PLL default value
	unSetting.stVDPLL.u32Mul = 0x048000;
	unSetting.stVDPLL.u32Div = 0x020000;
	unSetting.stVDPLL.u32RefckMux = 0x40; ///< bit[6] = 1 => Reference clock
	KER_CLK_Update(EN_KER_CLK_MPLL_VD, unSetting);

	///< Set AUD PLL default value
	unSetting.stAUPLL.u32Mul = 0x0f0000;
	unSetting.stAUPLL.u32Div = 0x01d4c0;
	unSetting.stAUPLL.u32CtrlReg = 0xbe1800e8;
	unSetting.stAUPLL.u32RefckMux = 0x80; 	
	unSetting.stAUPLL.enClkSrc = 4; ///< EN_KER_AUD_PLL_CLK_SRC_12M
	KER_CLK_Update(EN_KER_CLK_MPLL_AU, unSetting);

	///< Set CRC PLL default value
	unSetting.stAUPLL.u32Mul = 0x0f0000;
	unSetting.stAUPLL.u32Div = 0x01d4c0;
	unSetting.stAUPLL.u32CtrlReg = 0xbe1800e0;
	unSetting.stAUPLL.u32RefckMux = 0x80; 	
	unSetting.stAUPLL.enClkSrc = 4; ///< EN_KER_AUD_PLL_CLK_SRC_12M
	KER_CLK_Update(EN_KER_CLK_MPLL_CRC, unSetting);
#endif
#else
	hclk = CONFIG_AHB_CLOCK*1000;
#endif
	printk("hclk = %ld (Hz)\n", hclk);
}

EXPORT_SYMBOL(hclk);

