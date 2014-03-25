/*
 * Carsten Langgaard, carstenl@mips.com
 * Copyright (C) 2002 MIPS Technologies, Inc.  All rights reserved.
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
 * Defines for the NVT CHIP Interrupt Controller.
 */
#ifndef _NVT_INT_REGS_H
#define _NVT_INT_REGS_H

#define NVT_INT_BASE       0
#define NVT_INT_BASE2      32

/*
 * Interrupt Controller
 */
#define NVT_IRQ_ADDR_BASE (0xBD0E0000)

#define NVT_IRQ_MASK_ADDR_L		(NVT_IRQ_ADDR_BASE + 0x10)
#define NVT_IRQ_MASK_ADDR_H		(NVT_IRQ_ADDR_BASE + 0x14)
#define NVT_IRQ_TYPE_ADDR_L		(NVT_IRQ_ADDR_BASE + 0x18)
#define NVT_IRQ_TYPE_ADDR_H		(NVT_IRQ_ADDR_BASE + 0x1C)
#define NVT_IRQ_LEVEL_ADDR_L	(NVT_IRQ_ADDR_BASE + 0x20)
#define NVT_IRQ_LEVEL_ADDR_H	(NVT_IRQ_ADDR_BASE + 0x24)

#define NVT_IRQ_SRS_ADDR_BASE 	(NVT_IRQ_ADDR_BASE + 0x40)
#define NVT_IRQ_ID_ADDR 		(NVT_IRQ_ADDR_BASE + 0x80)

#define NVT_IRQ_MASK_L	*((volatile unsigned long *)(NVT_IRQ_MASK_ADDR_L))
#define NVT_IRQ_MASK_H	*((volatile unsigned long *)(NVT_IRQ_MASK_ADDR_H))
#define NVT_IRQ_TYPE_L	*((volatile unsigned long *)(NVT_IRQ_TYPE_ADDR_L))
#define NVT_IRQ_TYPE_H	*((volatile unsigned long *)(NVT_IRQ_TYPE_ADDR_H))
#define NVT_IRQ_LEVEL_L	*((volatile unsigned long *)(NVT_IRQ_LEVEL_ADDR_L))
#define NVT_IRQ_LEVEL_H	*((volatile unsigned long *)(NVT_IRQ_LEVEL_ADDR_H))

#define NVT_IRQ_ID		*((volatile unsigned long *)(NVT_IRQ_ID_ADDR))

#include_next <nvt-int.h> 

#endif /* !(_NVT_INT_REGS_H) */
