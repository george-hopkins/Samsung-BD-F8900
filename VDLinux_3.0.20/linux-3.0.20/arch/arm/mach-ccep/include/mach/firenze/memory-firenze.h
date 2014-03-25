/*
 * memory-firenze.h : Define memory address space.
 * 
 * Copyright (C) 2010 Samsung Electronics co.
 * tukho.kim <tukho.kim@samsung.com>
 *
 */
#ifndef _MEMORY_FIRENZE_H_
#define _MEMORY_FIRENZE_H_

/* Address space for DTV memory map */
#define MACH_MEM0_BASE 		0x60000000
#define MACH_MEM1_BASE 		0x70000000
#define MACH_MEM2_BASE 		0x80000000

#define SYS_MEM0_SIZE		(64 << 20)
#define SYS_MEM1_SIZE		(64 << 20)

#define MACH_MEM0_SIZE 		SZ_256M
#define MACH_MEM1_SIZE 		SZ_256M

#define MACH_BDSUB0_BASE	0xE8000000
#define MACH_BDSUB1_BASE	0xF4000000 /* 256MB - 16MB */


/* see arch/arm/mach-sdp/Kconfig */
#if !defined(CONFIG_SDP_SINGLE_DDR_B)
#define PHYS_OFFSET		MACH_MEM0_BASE
#else
#define PHYS_OFFSET		MACH_MEM1_BASE
#endif

/* Firenze DDR Bank
 * node 0:  0x60000000-0x6FFFFFFF - 256MB
 * node 1:  0x70000000-0x7FFFFFFF - 256MB
 * node 2:  0x80000000-0x8FFFFFFF - 256MB
 */
 
#if defined(CONFIG_DISCONTIGMEM) || defined(CONFIG_SPARSEMEM)

#define RANGE(n, start, size)   \
	        ( (((unsigned long)(n)) >= (start)) && (((unsigned long)n) < ((start)+(size))) )

/* Kernel size limit 256 */
#define KVADDR_MASK	0x1FFFFFFF
/* Bank size limit 256MByte */
#define MACH_MEM_MASK	0x0FFFFFFF
#define MACH_MEM_SHIFT	28	

#ifndef __ASSEMBLY__
extern unsigned int sdp_sys_mem0_size;
extern unsigned int sdp_sys_mem1_size;
#endif

#define __phys_to_virt(x) \
	({ u32 val = PAGE_OFFSET; \
           switch((x >> MACH_MEM_SHIFT) - 6) { \
		case 2:			\
			val += sdp_sys_mem1_size; \
		case 1:			\
			val += sdp_sys_mem0_size; \
		default:		\
		case 0:			\
			break;		\
	   } \
	   val += (x & MACH_MEM_MASK); \
	   val;})

#define __virt_to_phys(x) \
	({ u32 val = x - PAGE_OFFSET; \
	   if(val < sdp_sys_mem0_size) val += MACH_MEM0_BASE; \
           else { \
		val -= sdp_sys_mem0_size; \
		if (val < sdp_sys_mem1_size) val += MACH_MEM1_BASE; \
		else { \
		    val -= sdp_sys_mem1_size; \
		    val += MACH_MEM2_BASE; \
		} \
	   } \
	 val;})

#ifdef CONFIG_DISCONTIGMEM
#define KVADDR_TO_NID(addr) \
	({ u32 val = (u32)addr - PAGE_OFFSET; \
	   if(val < sdp_sys_mem0_size) val = 0; \
           else { \
		val -= sdp_sys_mem0_size; \
		if (val < sdp_sys_mem1_size) val = 1; \
		else  val = 2; \
	   } \
	 val;})


#define PFN_TO_NID(pfn) \
	(((pfn) >> (MACH_MEM_SHIFT - PAGE_SHIFT)) - 6)

#define LOCAL_MAP_PFN_NR(addr)  ((((unsigned long)addr & MACH_MEM_MASK) >> PAGE_SHIFT))

#define LOCAL_MAP_KVADDR_NR(kaddr) \
	({ u32 val = (u32)kaddr - PAGE_OFFSET; \
	   if(val >= sdp_sys_mem0_size) { \
		val -= sdp_sys_mem0_size; \
		if (val >= sdp_sys_mem1_size) \
		    val -= sdp_sys_mem1_size; \
	   } \
	 val = val >> PAGE_SHIFT; \
	 val;})
#endif

#ifdef CONFIG_SPARSEMEM
/* 0x60000000~, 0x70000000~, 0x80000000~ */
#define MAX_PHYSMEM_BITS	32
#define SECTION_SIZE_BITS	23
#endif

#endif

#endif
