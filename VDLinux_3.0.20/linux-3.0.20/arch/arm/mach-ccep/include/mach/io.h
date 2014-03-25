/*
 *  arch/arm/mach-ccep/include/mach/io.h
 *
 *  Copyright (C) 2003 ARM Limited
 *  Copyright (C) 2010 Samsung elctronics co.
 *  Author : tukho.kim@samsung.com
 *
 * modification history
 * --------------------
 * 15,Sep,2006 ij.jang : Add __mem_pci macro
 * 9 ,Nov,2006 ij.jang : modify __io macro, Add some macro for PCI
 * 25,Aug,2007 tukho.kim : io operation define for SDP75 
 * 02,Jun,2010 tukho.kim : kernel version 2.6.30.9
 */

#ifndef __ASM_ARM_MACH_IO_H
#define __ASM_ARM_MACH_IO_H

/* XXX: see arch/arm/mach-ssdtv/mach-xxx.c */
#define IO_SPACE_LIMIT		0x0FFFFFFF
#define PCI_IO_VADDR		0xF4000000

#define __io(a) 		((void __iomem *)(PCI_IO_VADDR + (a)))


#define iomem_valid_addr(iomem,size)	(1)
#define iomem_to_phys(iomem)		(iomem)

#define __mem_pci(a)			((void __iomem *)(a))

#endif /* __ASM_ARM_MACH_IO_H */
