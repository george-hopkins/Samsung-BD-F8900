/*
 * linux/arch/arm/plat-ccep/sdp_timer64.h
 *
 * Copyright (C) 2010 Samsung Electronics.co
 * Author : tukho.kim@samsung.com
 *
 */


#ifndef __SDP_TIMER_H_
#define __SDP_TIMER_H_

#ifndef SYS_TIMER
#define SYS_TIMER  	0 
#endif 

#ifndef CLKSRC_TIMER
#define CLKSRC_TIMER	1
#endif

#if (SYS_TIMER == CLKSRC_TIMER)
# error "check timer resource, system tick and hrtimer clock source"
#endif


typedef volatile struct {
	volatile u32 control0;		/* 0x00 */
	volatile u32 data0;		/* 0x04 */
	volatile u32 count0;		/* 0x08 */
	volatile u32 dma_sel;		/* 0x0C */
	volatile u32 control1;		/* 0x10 */
	volatile u32 data1;		/* 0x14 */
	volatile u32 count1;		/* 0x18 */
	volatile u32 reserved1;		/* 0x1C */
	volatile u32 control2;		/* 0x20 */
	volatile u32 data2;		/* 0x24 */
	volatile u32 count2;		/* 0x28 */
	volatile u32 reserved2;		/* 0x2C */
	volatile u32 control3;		/* 0x30 */
	volatile u32 data3;		/* 0x34 */
	volatile u32 count3;		/* 0x38 */
	volatile u32 reserved3;		/* 0x3C */
	volatile u32 intr_status;	/* 0x40 */
	volatile u32 reserved4_gap[15]; /* 0x44 ~ 0x7C */
	volatile u32 data64l0;		/* 0x80 */
	volatile u32 data64h0;		/* 0x84 */
	volatile u32 count64l0;		/* 0x88 */
	volatile u32 count64h0;		/* 0x8C */
	volatile u32 data64l1;		/* 0x90 */
	volatile u32 data64h1;		/* 0x94 */
	volatile u32 count64l1;		/* 0x98 */
	volatile u32 count64h1;		/* 0x9C */
	volatile u32 data64l2;		/* 0xA0 */
	volatile u32 data64h2;		/* 0xA4 */
	volatile u32 count64l2;		/* 0xA8 */
	volatile u32 count64h2;		/* 0xAC */
	volatile u32 data64l3;		/* 0xB0 */
	volatile u32 data64h3;		/* 0xB4 */
	volatile u32 count64l3;		/* 0xB8 */
	volatile u32 count64h3;		/* 0xBC */
}SDP_TIMER_REG_T;


#define TMCON_64BIT_DOWN	(0x0 << 5)
#define TMCON_64BIT_UP		(0x1 << 5)

#define TMCON_64BIT		(0x0 << 4)
#define TMCON_16BIT		(0x1 << 4)

#define TMCON_MUX04		(0x0 << 2)
#define TMCON_MUX08		(0x1 << 2)
#define TMCON_MUX16		(0x2 << 2)
#define TMCON_MUX32		(0x3 << 2)

#define TMCON_INT_DMA_EN	(0x1 << 1)
#define TMCON_RUN		(0x1)

#define TMDATA_PRES(x)	((x > 0) ? ((x & 0xFF) << 16) : 1)

#ifdef VA_TIMER_BASE
#define SDP_TIMER_BASE 	VA_TIMER_BASE
#else
# ifndef SDP_TIMER_BASE
# 	error	"SDP Timer base is not defined, Please check sdp platform header file" 
# endif 
#endif

#define R_TIMER(nr,reg)		(gp_sdp_timer->reg##nr)
#define R_TMDMASEL		(gp_sdp_timer->dma_sel)
#define R_TMSTATUS              (gp_sdp_timer->intr_status)

#if (SYS_TIMER == 1)
#define R_SYSTMCON		R_TIMER(1, control)
#define R_SYSTMDATA 		R_TIMER(1, data)
#define R_SYSTMCNT		R_TIMER(1, count)
#define R_SYSTMDATA64L		R_TIMER(1, data64l)
#define R_SYSTMDATA64H		R_TIMER(1, data64h)
#define R_SYSTMCNT64L		R_TIMER(1, count64l)
#define R_SYSTMCNT64H		R_TIMER(1, count64h)
#elif (SYS_TIMER == 2)
#define R_SYSTMCON		R_TIMER(2, control)
#define R_SYSTMDATA 		R_TIMER(2, data)
#define R_SYSTMCNT		R_TIMER(2, count)
#define R_SYSTMDATA64L		R_TIMER(2, data64l)
#define R_SYSTMDATA64H		R_TIMER(2, data64h)
#define R_SYSTMCNT64L		R_TIMER(2, count64l)
#define R_SYSTMCNT64H		R_TIMER(2, count64h)
#else
#define R_SYSTMCON		R_TIMER(0, control)
#define R_SYSTMDATA 		R_TIMER(0, data)
#define R_SYSTMCNT		R_TIMER(0, count)
#define R_SYSTMDATA64L		R_TIMER(0, data64l)
#define R_SYSTMDATA64H		R_TIMER(0, data64h)
#define R_SYSTMCNT64L		R_TIMER(0, count64l)
#define R_SYSTMCNT64H		R_TIMER(0, count64h)
#endif

#define SYS_TIMER_BIT		(1 << SYS_TIMER)
#define SYS_TIMER_IRQ		(IRQ_TIMER)

#if (CLKSRC_TIMER == 0)
#define R_CLKSRC_TMCON		R_TIMER(0, control)
#define R_CLKSRC_TMDATA 	R_TIMER(0, data)
#define R_CLKSRC_TMCNT		R_TIMER(0, count)
#define R_CLKSRC_TMDATA64L	R_TIMER(0, data64l)
#define R_CLKSRC_TMDATA64H	R_TIMER(0, data64h)
#define R_CLKSRC_TMCNT64L	R_TIMER(0, count64l)
#define R_CLKSRC_TMCNT64H	R_TIMER(0, count64h)
#elif (CLKSRC_TIMER == 2)
#define R_CLKSRC_TMCON		R_TIMER(2, control)
#define R_CLKSRC_TMDATA 	R_TIMER(2, data)
#define R_CLKSRC_TMCNT		R_TIMER(2, count)
#define R_CLKSRC_TMDATA64L	R_TIMER(2, data64l)
#define R_CLKSRC_TMDATA64H	R_TIMER(2, data64h)
#define R_CLKSRC_TMCNT64L	R_TIMER(2, count64l)
#define R_CLKSRC_TMCNT64H	R_TIMER(2, count64h)
#else
#define R_CLKSRC_TMCON		R_TIMER(1, control)
#define R_CLKSRC_TMDATA 	R_TIMER(1, data)
#define R_CLKSRC_TMCNT		R_TIMER(1, count)
#define R_CLKSRC_TMDATA64L	R_TIMER(1, data64l)
#define R_CLKSRC_TMDATA64H	R_TIMER(1, data64h)
#define R_CLKSRC_TMCNT64L	R_TIMER(1, count64l)
#define R_CLKSRC_TMCNT64H	R_TIMER(1, count64h)
#endif

#define CLKSRC_TIMER_BIT	(1 << CLKSRC_TIMER)
#define CLKSRC_TIMER_IRQ	(IRQ_TIMER)


#endif /*  __SDP_TIMER_H_ */
