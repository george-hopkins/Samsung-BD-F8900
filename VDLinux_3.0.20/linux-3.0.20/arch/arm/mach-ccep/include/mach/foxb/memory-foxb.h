/*
 * memory-foxb.h : Define memory address space.
 * 
 * Copyright (C) 2011 Samsung Electronics co.
 * SeungJun Heo <seungjun.heo@samsung.com>
 * Seihee Chon <sh.chon@samsung.com>
 * Sola lee<sssol.lee@samsung.com>
 */
#ifndef _MEMORY_FOXB_H_
#define _MEMORY_FOXB_H_

#include <linux/version.h>

/* Address space for DTV memory map */
#define MACH_MEM0_BASE 		0x40000000
#define MACH_MEM1_BASE 		0x80000000

#define SYS_MEM0_SIZE		(64 << 20)
#define SYS_MEM1_SIZE		(64 << 20)

#define MACH_MEM0_SIZE 		SZ_256M
#define MACH_MEM1_SIZE 		SZ_256M

/*
#define MACH_DTVSUB0_BASE	0xF8000000
#define MACH_DTVSUB0_SIZE	SZ_4M

#define MACH_DTVSUB1_BASE	0xF8400000
#define MACH_DTVSUB1_SIZE	SZ_64M
*/

/* see arch/arm/mach-sdp/Kconfig */
#if !defined(CONFIG_SDP_SINGLE_DDR_B)
#	define PHYS_OFFSET		MACH_MEM0_BASE
#else
#	define PHYS_OFFSET		MACH_MEM1_BASE
#endif

/* Valencia DDR Bank
 * node 0:  0x40000000-0x7FFFFFFF - 1GB
 * node 1:  0x80000000-0xBFFFFFFF - 1GB
 */

/* Common macro for specific memory model */
#if defined(CONFIG_SPARSEMEM) || defined(CONFIG_DISCONTIGMEM)

#	define RANGE(n, start, size)   \
	        ((unsigned long)n >= start && (unsigned long)n < (start+size))

	/* Kernel size limit 1Gbyte */
#	define KVADDR_MASK	0x3FFFFFFF
	/* Bank size limit 1GByte */
#	define MACH_MEM_MASK	0x3FFFFFFF
#	define MACH_MEM_SHIFT	31

#	ifndef __ASSEMBLY__
	extern unsigned int sdp_sys_mem0_size;
#	endif

#	define __phys_to_virt(x) \
	(((x >> MACH_MEM_SHIFT) & 1) ? \
		(PAGE_OFFSET + sdp_sys_mem0_size + (x & MACH_MEM_MASK) ) : \
		(PAGE_OFFSET + (x & MACH_MEM_MASK)) )

#	define __virt_to_phys(x) \
	(RANGE(x, PAGE_OFFSET, sdp_sys_mem0_size) ? \
		(MACH_MEM0_BASE + (x & KVADDR_MASK)) : \
		(MACH_MEM1_BASE + ((x & KVADDR_MASK) - sdp_sys_mem0_size)) )

#endif  /* defined(CONFIG_SPARSEMEM) || defined(CONFIG_DISCONTIGMEM) */

#ifdef CONFIG_SPARSEMEM
/* 0x40000000~0xBFFFFFFF 0xC0000000~0xDFFFFFFF, 0xE0000000~0xFFFFFFFF */
#	define SECTION_SIZE_BITS 	23		/* ??? */
#	define MAX_PHYSMEM_BITS		32		/* ??? */
/* defined CONFIG_SPARSEMME  */

#elif defined(CONFIG_DISCONTIGMEM) && (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35)) 

#	define KVADDR_TO_NID(addr) \
	(RANGE(addr, PAGE_OFFSET, sdp_sys_mem0_size) ? 0 : 1)

#	define PFN_TO_NID(pfn) \
	(((pfn) >> (MACH_MEM_SHIFT - PAGE_SHIFT)) & 1)

#	define LOCAL_MAP_PFN_NR(addr)  ( (((unsigned long)addr & MACH_MEM_MASK) >> PAGE_SHIFT)  )

#	define LOCAL_MAP_KVADDR_NR(kaddr) \
	((((unsigned long)kaddr & KVADDR_MASK) < sdp_sys_mem0_size) ? \
	((unsigned long)kaddr & KVADDR_MASK) >> PAGE_SHIFT : \
	(((unsigned long)kaddr & KVADDR_MASK) - sdp_sys_mem0_size) >> PAGE_SHIFT )

#endif	/* defined(CONFIG_DISCONTIGMEM) && (LINUX_VERSION_CODE < KERNEL_VESION(2,6,35)) */

#endif /*  _MEMORY_FOXB_H_ */
