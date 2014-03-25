/*
 * Header file for using the wbflush routine
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (c) 1998 Harald Koerfgen
 * Copyright (C) 2002 Maciej W. Rozycki
 */
#ifndef _ASM_WBFLUSH_H
#define _ASM_WBFLUSH_H


#ifdef CONFIG_CPU_HAS_WB

extern void (*__wbflush)(void);
extern void wbflush_setup(void);

#define wbflush()			\
	do {				\
		__sync();		\
		__wbflush();		\
	} while (0)

#else /* !CONFIG_CPU_HAS_WB */

#define wbflush_setup() do { } while (0)

#define wbflush() fast_iob()

#endif /* !CONFIG_CPU_HAS_WB */


#ifdef CONFIG_NVT_CHIP

#define __read_ahb_reg() \
	__asm__ __volatile__( \
			".set push\n\t" \
			".set noreorder\n\t" \
			"lw $0, %0\n\t" \
			"nop\n\t" \
			".set pop\n\t" \
			: /* no output */ \
			: "m" (*(int*)0xbd0e0090) \
			: "memory" )

#ifdef CONFIG_EXTERNAL_SYNC

#define wbflush_axi() __sync()
#define wbflush_ahb() __sync()

#else	/* !CONFIG_EXTERNAL_SYNC */

#define wbflush_axi() fast_iob()
#define wbflush_ahb() \
	do { 	\
		__sync(); \
		__read_ahb_reg(); \
	} while (0)

#endif /* !CONFIG_EXTERNAL_SYNC */

#define external_sync() \
	do{\
		extern externl_sync_lock;\
		unsigned long flags;\
		spin_lock_irqsave(&externl_sync_lock, flags);\
		set_c0_config7(0x100);\
		__asm__ __volatile__("ehb\n\t");\
		__sync();\
		clear_c0_config7(0x100);\
		__asm__ __volatile__("ehb\n\t");\
		spin_unlock_irqrestore(&externl_sync_lock, flags);\
	}while(0)

#endif /* CONFIG_NVT_CHIP */

#endif /* _ASM_WBFLUSH_H */
