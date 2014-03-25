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
 * Defines of REGs and Address of NVT NT72568 SOC
 *
 */
#ifndef _NT72568_SYS_H
#define _NT72568_SYS_H

#include <asm/addrspace.h>

#define SYS_PLL_REG_BASE			0xBD020000

#define SYS_PLL_SELECT_0	*((volatile unsigned long *)(SYS_PLL_REG_BASE + 0x30))

#define AXI_PLL_SELECT		((SYS_PLL_SELECT_0 >> 8) & 0x7)
#define AHB_PLL_SELECT		((SYS_PLL_SELECT_0 >> 5) & 0x7)
#define DSP_PLL_SELECT		((SYS_PLL_SELECT_0 >> 2) & 0x7)
#define CPU_PLL_SELECT		((SYS_PLL_SELECT_0 >> 0) & 0x3)

#define SYS_PLL_SELECT_1	*((volatile unsigned long *)(SYS_PLL_REG_BASE + 0x34))

#define SYS_MPLL_PBUS_OFFSET 	*((volatile unsigned int *)(SYS_PLL_REG_BASE + 0xB0))

#define SYS_MPLL_PBUS_WR_DATA	*((volatile unsigned int *)(SYS_PLL_REG_BASE + 0xB4))
#define SYS_MPLL_PBUS_RD_DATA	*((volatile unsigned int *)(SYS_PLL_REG_BASE + 0xB8))
#define SYS_MPLL_PBUS_PAGE_EN *((volatile unsigned int *)(SYS_PLL_REG_BASE + 0xBC))

#define _SYS_MPLL_PAGE_B_EN	0x00000001
#define _SYS_MPLL_PAGE_0_EN	0x00000002

#ifdef CONFIG_NVT_MICREL_PHY_SUPPORT
#define NVT_PHY_RESET_GPIO_ADDR 0xbd0f0400
#define NVT_PHY_RESET_GPIO_BIT	3
#elif defined CONFIG_NVT_RTL_PHY_SUPPORT
#define NVT_PHY_RESET_GPIO_ADDR 0xbd0f0400
#define NVT_PHY_RESET_GPIO_BIT	3
#endif

#endif /* (_NT72568_SYS_H) */

