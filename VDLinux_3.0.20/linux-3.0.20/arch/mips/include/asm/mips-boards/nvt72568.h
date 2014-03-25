/*
 * 
 * ########################################################################
 *
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
 *
 * ########################################################################
 *
 * Defines of REGs and Address of NVT NT72558 SOC
 *
 */
#ifndef _MIPS_NVT72558_H
#define _MIPS_NVT72558_H

#include <asm/addrspace.h>

#define SYS_CLK_REG_BASE		0xBD020000

#define _SYS_MPLL_PAGE_B_ENABLE 0x00000001
#define _SYS_MPLL_PAGE_0_ENABLE 0x00000002

/* cpu pll register */
#define SYS_CLOCK_SELECT	*((volatile unsigned int *) (SYS_CLK_REG_BASE + 0x0030))
#define AXI_CLK_SEL 			((SYS_CLOCK_SELECT >> 8) & 0x7)
#define AHB_CLK_SEL 			((SYS_CLOCK_SELECT >> 5) & 0x7)
#define DSP_CLK_SEL 			((SYS_CLOCK_SELECT >> 2) & 0x7)
#define MIPS_CLK_SEL			((SYS_CLOCK_SELECT >> 0) & 0x3)
#define _CPU_PLL_OFFSET	0x62	
#define _AXI_PLL_OFFSET	0x6a
#define _DDR_PLL_OFFSET	0x82
#define _ETH_PLL_OFFSET	0xaa

#define SYS_MPLL_PBUS_OFFSET 			*((volatile unsigned int *)(SYS_CLK_REG_BASE + 0xB0))
#define SYS_MPLL_PBUS_WR_DATA 		*((volatile unsigned int *)(SYS_CLK_REG_BASE + 0xB4))
#define SYS_MPLL_PBUS_RD_DATA 		*((volatile unsigned int *)(SYS_CLK_REG_BASE + 0xB8))
#define SYS_MPLL_PBUS_PAGE_ENABLE *((volatile unsigned int *)(SYS_CLK_REG_BASE + 0xBC))

/*
 * UART register base.
 */

#if 1 // NOTE modify this, just temp solution
#define REG_GPIO_BASE                    (0xBC040400)
// -------------------------------------------------------------------------------------------------
#define REG_GPIO_LEVEL           (REG_GPIO_BASE+0x00)
#define REG_GPIO_DIR             (REG_GPIO_BASE+0x04)

#define REG_GPIO_PAD_BASE                (0xBC040800)
#define REG_GPIO4_PAD_SET                 (REG_GPIO_PAD_BASE+0x5e)
//index 10~0
//TODO add assert for index range
#define GPIO_DIR_INPUT(index)		(*(volatile unsigned long *)REG_GPIO_DIR &= ~(1<<(index)))
#define GPIO_DIR_OUTPUT(index)		(*(volatile unsigned long *)REG_GPIO_DIR |=  (1<<(index)))

#define GPIO_LV_VALUE(index)		(((*(volatile unsigned long *)REG_GPIO_LEVEL & 0x3ff)>>(index))&1)

//TODO modify this for better way
#define GPIO4_PAD_SET_PD()			(((*(volatile unsigned char*)REG_GPIO4_PAD_SET) = (*(volatile unsigned char*)REG_GPIO4_PAD_SET)& 0xf3 | 0x04))
#endif

#define NT72558_UART0_REGS_BASE    (0xBD090000)
#define NT72558_UART1_REGS_BASE    (0xBD091000)
#define NT72558_UART2_REGS_BASE    (0xBD092000)
#define NT72558_BASE_BAUD ( 115200 / 16 )

/*
 * INTERRUPT CONTROLLER
 */

#define REG_CPU0_IRQ_ID      *((volatile unsigned int *) (0xBD0E0080))
#define REG_CPU0_IRQ_MASK_L  *((volatile unsigned int *) (0xBD0E0010))
#define REG_CPU0_IRQ_MASK_H  *((volatile unsigned int *) (0xBD0E0014))

#define ADDR_CPU0_IRQ_MASK_L (0xBD0E0010)
#define ADDR_CPU0_IRQ_MASK_H (0xBD0E0014)

#endif /* !(_MIPS_NVT72558_H) */
