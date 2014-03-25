/*
 * linux/arch/arm/mach-ccep/sdp_irq.h
 *
 * Copyright (C) 2010 Samsung Electronics.co
 * Author : tukho.kim@samsung.com
 *
 */
#ifndef __SDP_INTC_H
#define __SDP_INTC_H

#include <mach/irqs.h>
#include <mach/platform.h>

#ifndef VA_INTC_BASE
#define SDP_INTC_BASE0		VA_INT_BASE
#else
#define SDP_INTC_BASE0		VA_INTC_BASE
#endif

#ifdef VA_INTC_BASE1
#define SDP_INTC_BASE1		VA_INTC_BASE1
#endif


#define INTCON		(0x00)
#define INTPND		(0x04)
#define INTMOD		(0x08)
#define INTMSK		(0x0C)
#define INT_LEVEL	(0x10)

#define I_PSLV0		(0x14)
#define I_PSLV1		(0x18)
#define I_PSLV2		(0x1C)
#define I_PSLV3		(0x20)
#define I_PMST		(0x24)
#define I_CSLV0		(0x28)
#define I_CSLV1		(0x2C)
#define I_CSLV2		(0x30)
#define I_CSLV3		(0x34)
#define I_CMST		(0x38)
#define I_ISPR		(0x3C)
#define I_ISPC		(0x40)

#define F_PSLV0		(0x44)
#define F_PSLV1		(0x48)
#define F_PSLV2		(0x4C)
#define F_PSLV3		(0x50)
#define F_PMST		(0x54)
#define F_CSLV0		(0x58)
#define F_CSLV1		(0x5C)
#define F_CSLV2		(0x60)
#define F_CSLV3		(0x64)
#define F_CMST		(0x68)
#define F_ISPR		(0x6C)
#define F_ISPC		(0x70)

#define INT_POLARITY	(0x74)
#define I_VECADDR	(0x78)
#define F_VECADDR	(0x7C)

#define INT_SRCPND	(0x90)

#ifdef SDP_USING_SUBINT
#define INT_SUBINT	(0x94)
#endif

#define INT_SRCSEL0	(0x98)
#define INT_SRCSEL1	(0x9C)
#define INT_SRCSEL2	(0xA0)
#define INT_SRCSEL3	(0xA4)
#define INT_SRCSEL4	(0xA8)
#define INT_SRCSEL5	(0xAC)
#define INT_SRCSEL6	(0xB0)
#define INT_SRCSEL7	(0xB4)


#define INTCON_FIQ_DIS		(0x1)
#define INTCON_IRQ_DIS		(0x1 << 1)
#define INTCON_VECTORED		(0x1 << 2)
#define INTCON_GMASK		(0x1 << 3)
#define INTCON_IRQ_FIQ_DIS	(0x3)

#define POLARITY_HIGH	(0)
#define POLARITY_LOW	(1)

/* level (edge or level) */
#define LEVEL_EDGE	(0x0)
#define LEVEL_LEVEL	(0x1)

#define LEVEL_ATTR_EXT 	(0x2)
#define LEVEL_EDGE_EXT	(LEVEL_ATTR_EXT | LEVEL_EDGE)
#define LEVEL_LEVEL_EXT	(LEVEL_ATTR_EXT | LEVEL_LEVEL)

#ifndef __ASSEMBLY__
typedef volatile struct {
	volatile u32 control;		/* 0x00 */
	volatile u32 pending;		/* 0x04 */
	volatile u32 mode;		/* 0x08 */
	volatile u32 mask;		/* 0x0C */
	volatile u32 level;		/* 0x10 */ /* Edge or level */

	volatile u32 i_pslv0;		/* 0x14 */
	volatile u32 i_pslv1;           /* 0x18 */
	volatile u32 i_pslv2;           /* 0x1C */
	volatile u32 i_pslv3;		/* 0x20 */
	volatile u32 i_pmst;            /* 0x24 */
	volatile u32 i_cslv0;           /* 0x28 */
	volatile u32 i_cslv1;           /* 0x2C */
	volatile u32 i_cslv2;		/* 0x30 */
	volatile u32 i_cslv3;           /* 0x34 */
	volatile u32 i_cmst;	        /* 0x38 */
	volatile u32 i_ispr;	        /* 0x3C */
	volatile u32 i_ispc;	        /* 0x40 */

	volatile u32 f_pslv0;		/* 0x44 */
	volatile u32 f_pslv1;           /* 0x48 */
	volatile u32 f_pslv2;           /* 0x4C */
	volatile u32 f_pslv3;		/* 0x50 */
	volatile u32 f_pmst;            /* 0x54 */
	volatile u32 f_cslv0;           /* 0x58 */
	volatile u32 f_cslv1;           /* 0x5C */
	volatile u32 f_cslv2;		/* 0x60 */
	volatile u32 f_cslv3;           /* 0x64 */
	volatile u32 f_cmst;	        /* 0x68 */
	volatile u32 f_ispr;	        /* 0x6C */
	volatile u32 f_ispc;	        /* 0x70 */

	volatile u32 polarity;	        /* 0x74 */
	volatile u32 i_vector_addr;     /* 0x78 */
	volatile u32 f_vector_addr;     /* 0x7C */

	volatile u32 tic_qsrc;		/* 0x80 */
	volatile u32 tic_f_irq;		/* 0x84 */
	volatile u32 reserved2;		/* 0x88 */
	volatile u32 tic_test_en;	/* 0x8C */
	
	volatile u32 source_pend;	/* 0x90 */
	volatile u32 sub_interrupt;	/* 0x94 */
	
	volatile u32 source_select0;	/* 0x98 */
	volatile u32 source_select1;	/* 0x9C */
	volatile u32 source_select2;	/* 0xA0 */
	volatile u32 source_select3;	/* 0xA4 */
	volatile u32 source_select4;	/* 0xA8 */
	volatile u32 source_select5;	/* 0xAC */
	volatile u32 source_select6;	/* 0xB0 */
	volatile u32 source_select7;	/* 0xB4 */
}SDP_INTR_REG_T;

typedef struct {
	unsigned irqSrc:8;
	unsigned qSrc:8;
	unsigned level:8;
	unsigned polarity:8;
	unsigned int prioMask;
	unsigned int subPrioMask;	
}SDP_INTR_ATTR_T;
#endif

#endif // __SDP_INTC_H
