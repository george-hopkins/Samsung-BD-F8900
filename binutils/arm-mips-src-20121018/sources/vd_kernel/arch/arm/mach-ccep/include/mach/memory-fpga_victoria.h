/*
 * memory-fpga_victoria.h : Define memory address space.
 * 
 * Copyright (C) 2011 Samsung Electronics co.
 * SeungJun Heo <seungjun.heo@samsung.com>
 *
 */
#ifndef _MEMORY_FPGA_VICTORIA_H_
#define _MEMORY_FPGA_VICTORIA_H_

/* Address space for DTV memory map */
#define MACH_MEM0_BASE 		0x60000000
#define MACH_MEM1_BASE 		0x70000000
#define MACH_MEM2_BASE 		0x80000000
#define MACH_MEM3_BASE 		0x90000000

#define SYS_MEM0_SIZE		(64 << 20)
#define SYS_MEM1_SIZE		(64 << 20)
#define SYS_MEM2_SIZE		(64 << 20)
#define SYS_MEM3_SIZE		(64 << 20)

#define MACH_MEM0_SIZE 		SZ_256M
#define MACH_MEM1_SIZE 		SZ_256M
#define MACH_MEM2_SIZE 		SZ_256M
#define MACH_MEM3_SIZE 		SZ_256M

/* see arch/arm/mach-sdp/Kconfig */
#if !defined(CONFIG_SDP_SINGLE_DDR_B)
#define PHYS_OFFSET		MACH_MEM0_BASE
#else
#define PHYS_OFFSET		MACH_MEM1_BASE
#endif

/* Victoria DDR Bank
 * bank 0:  0x60000000-0x6FFFFFFF - 256MB
 * bank 1:  0x70000000-0x7FFFFFFF - 256MB
 * bank 2:  0x80000000-0x8FFFFFFF - 256MB
 * bank 3:  0x90000000-0x9FFFFFFF - 256MB
 */
 
#if defined(CONFIG_DISCONTIGMEM) || defined(CONFIG_SPARSEMEM)

#define RANGE(n, start, size)   \
	        ( (((unsigned long)(n)) >= (start)) && (((unsigned long)n) < ((start)+(size))) )

/* Bank size limit 512MByte */
#define MACH_MEM_MASK	(0x0FFFFFFF)
#define MACH_MEM_SHIFT	(28)

/* Kernel size limit 1024Mbyte */
#define KVADDR_MASK	(0x1FFFFFFF)

#ifndef __ASSEMBLY__
extern unsigned int sdp_sys_mem0_size;
extern unsigned int sdp_sys_mem1_size;
extern unsigned int sdp_sys_mem2_size;
#endif

#define __phys_to_virt(x) \
	({ u32 val = PAGE_OFFSET; \
           switch((x >> MACH_MEM_SHIFT) - 6) { \
		case 3:			\
			val += sdp_sys_mem2_size; \
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
		    if(val < sdp_sys_mem2_size) val += MACH_MEM2_BASE; \
			else { \
				val -= sdp_sys_mem2_size; \
				val += MACH_MEM3_BASE; \
			} \
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
/* 0x60000000~0x70000000 0x70000000~0x80000000, 0x80000000~0x90000000 0x90000000~0xA0000000*/
#define SECTION_SIZE_BITS	23
#define MAX_PHYSMEM_BITS	32
#endif

#endif

#endif
