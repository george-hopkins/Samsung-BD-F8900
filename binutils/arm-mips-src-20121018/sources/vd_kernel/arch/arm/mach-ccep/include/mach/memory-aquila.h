/*
 * memory-aquila.h : Define memory address space.
 * 
 * Copyright (C) 2009 Samsung Electronics co.
 * Ikjoon Jang <ij.jang@samsung.com>
 *
 */
#ifndef _MEMORY_AQUILA_H_
#define _MEMORY_AQUILA_H_

/* Address space for DTV memory map */
#define MACH_MEM0_BASE 		0x60000000
#define MACH_MEM1_BASE 		0x80000000

#define SYS_MEM0_SIZE		(64 << 20)
#define SYS_MEM1_SIZE		(64 << 20)

#define MACH_MEM0_SIZE 		SZ_256M
#define MACH_MEM1_SIZE 		SZ_256M

#define MACH_DTVSUB0_BASE	0xF8000000
#define MACH_DTVSUB0_SIZE	SZ_4M

#define MACH_DTVSUB1_BASE	0xF8400000
#define MACH_DTVSUB1_SIZE	SZ_64M


/* see arch/arm/mach-sdp/Kconfig */
#if !defined(CONFIG_SDP_SINGLE_DDR_B)
#define PHYS_OFFSET		MACH_MEM0_BASE
#else
#define PHYS_OFFSET		MACH_MEM1_BASE
#endif

/* Valencia DDR Bank
 * node 0:  0x60000000-0x7FFFFFFF - 512MB
 * node 1:  0x80000000-0x9FFFFFFF - 512MB
 */
#ifdef CONFIG_DISCONTIGMEM

#define RANGE(n, start, size)   \
	        ((unsigned long)n >= start && (unsigned long)n < (start+size))

/* Kernel size limit 512Mbyte */
#define KVADDR_MASK	0x1FFFFFFF
/* Bank size limit 512MByte */
#define MACH_MEM_MASK	0x1FFFFFFF
#define MACH_MEM_SHIFT	31

#ifndef __ASSEMBLY__
extern unsigned int sdp_sys_mem0_size;
#endif

#define __phys_to_virt(x) \
	(((x >> MACH_MEM_SHIFT) & 1) ? \
		(PAGE_OFFSET + sdp_sys_mem0_size + (x & MACH_MEM_MASK) ) : \
		(PAGE_OFFSET + (x & MACH_MEM_MASK)) )

#define __virt_to_phys(x) \
	(RANGE(x, PAGE_OFFSET, sdp_sys_mem0_size) ? \
		(MACH_MEM0_BASE + (x & KVADDR_MASK)) : \
		(MACH_MEM1_BASE + ((x & KVADDR_MASK) - sdp_sys_mem0_size)) )

#define KVADDR_TO_NID(addr) \
	(RANGE(addr, PAGE_OFFSET, sdp_sys_mem0_size) ? 0 : 1)

#define PFN_TO_NID(pfn) \
	(((pfn) >> (MACH_MEM_SHIFT - PAGE_SHIFT)) & 1)

#define LOCAL_MAP_PFN_NR(addr)  ( (((unsigned long)addr & MACH_MEM_MASK) >> PAGE_SHIFT)  )

#define LOCAL_MAP_KVADDR_NR(kaddr) \
	((((unsigned long)kaddr & KVADDR_MASK) < sdp_sys_mem0_size) ? \
	((unsigned long)kaddr & KVADDR_MASK) >> PAGE_SHIFT : \
	(((unsigned long)kaddr & KVADDR_MASK) - sdp_sys_mem0_size) >> PAGE_SHIFT )

#endif	/* CONFIG_DISCONTIGMEM */

#endif

