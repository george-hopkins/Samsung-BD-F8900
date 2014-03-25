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
 * Defines for the NVT NT72568  interrupt controller.
 */
#ifndef _NT72568_INT_H
#define _NT72568_INT_H

#define NVT_INT_UART0		(25)
#define NVT_INT_UART1		(39)
#define NVT_INT_UART2		(29)

#define NVT_INT_CPUCTR		(32)
#define NVT_INT_PERFCTR		(33)
#define NVT_INT_SWINT0		(34)
#define NVT_INT_SWINT1		(35)

#define NVT_INT_LEVEL_DEF_L	(0x00820020)
#define NVT_INT_LEVEL_DEF_H	(0x00000020)

#endif /* !(_NT72568_INT_H) */
