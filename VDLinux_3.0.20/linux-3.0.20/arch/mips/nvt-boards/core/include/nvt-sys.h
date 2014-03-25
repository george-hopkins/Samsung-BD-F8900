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
 * Defines of REGs and Address of NVT CHIP System
 *
 */
#ifndef _NVT_SYS_H
#define _NVT_SYS_H

#include <asm/addrspace.h>

/* System pll offset */
#define _CPU_PLL_OFFSET 0x62
#define _AXI_PLL_OFFSET 0x6a
#define _DDR_PLL_OFFSET 0x82
#define _MPG_PLL_OFFSET 0x9a
#define _ETH_PLL_OFFSET 0xaa

/*
 * UART register base.
 */
#define NVT_UART0_REGS_BASE    (0xBD090000)
#define NVT_UART1_REGS_BASE    (0xBD091000)
#define NVT_UART2_REGS_BASE    (0xBD092000)
#define NVT_BASE_BAUD ( 57600 / 16 )

#define NVT_SYS_UART_BASE NVT_UART1_REGS_BASE

#include_next <nvt-sys.h>

#endif /* !(_NVT_SYS_H) */
