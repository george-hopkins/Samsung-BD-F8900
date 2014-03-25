/****************************************************************8
 *
 * arch/arm/mach-ccep/include/mach/memory.h
 * 
 * Copyright (C) 2010 Samsung Electronics co.
 * Author : tukho.kim@samsung.com
 *
 */
#ifndef __ASM_ARCH_MEMORY_H
#define __ASM_ARCH_MEMORY_H

/*
 * Virtual view <-> DMA view memory address translations
 * virt_to_bus: Used to translate the virtual address to an
 *              address suitable to be passed to set_dma_addr
 * bus_to_virt: Used to convert an address for DMA operations
 *              to an address that the kernel can use.
 */

#if 0
#define __virt_to_bus(x)	 __virt_to_phys(x)
#define __bus_to_virt(x)	 __phys_to_virt(x)
#endif

#if defined(CONFIG_MACH_AQUILA)
#   include <mach/memory-aquila.h>
#elif defined(CONFIG_MACH_GRIFFIN)
#   include <mach/memory-griffin.h>
#elif defined(CONFIG_MACH_FPGA_GENOA)
#   include <mach/memory-fpga_genoa.h>
#elif defined(CONFIG_MACH_DALMA)
#   include <mach/memory-dalma.h>
#elif defined(CONFIG_MACH_FIRENZE)
#   include <mach/memory-firenze.h>
#elif defined(CONFIG_MACH_FPGA_FIRENZE)
#   include <mach/memory-fpga_firenze.h>
#elif defined(CONFIG_MACH_FLAMENGO)
#   include <mach/memory-flamengo.h>
#elif defined(CONFIG_MACH_FPGA_FLAMENGO)
#   include <mach/memory-fpga_flamengo.h>
#elif defined(CONFIG_MACH_VICTORIA) || defined(CONFIG_MACH_FPGA_VICTORIA)
#   include <mach/memory-victoria.h>
#elif defined(CONFIG_MACH_SANTOS) || defined(CONFIG_MACH_FPGA_SANTOS)
#   include <mach/memory-santos.h>
#elif defined(CONFIG_MACH_FOXAP)
#	include <mach/memory-foxap.h>
#elif defined(CONFIG_MACH_FPGA_FOXAP)
#	include <mach/memory-fpga_foxap.h>
#else
#   error
#endif 

#endif /*__ASM_ARCH_MEMORY_H*/

