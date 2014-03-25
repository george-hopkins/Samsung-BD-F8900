/****************************************************************************
 *   FileName    : Dma.h
 *   Description :
 ****************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips, Inc.
 *   ALL RIGHTS RESERVED
 *
 ****************************************************************************/

#ifndef __DMA_H__
#define __DMA_H__

#if 0
#include "IO_LIB/include/IO_TCC7XX.h"
#endif

//#ifdef CD_RIPPING_INCLUDE
//enum { IO_DMA_CH0, IO_DMA_CH1};	//CDDMA
//#endif

/*****************************************************************************/
//	TRANSFER TYPE IN CHCTRL REGISTER
/*****************************************************************************/
enum
{
	DMA_SINGLE_EDGE = 0,	// single transfer with edge-triggered detection
	DMA_HW_ARBIT,			// HW transfer
	DMA_SW_ARBIT,			// SW transfer
	DMA_SINGLE_LEVEL		// SINGLE transfer with level-sensitive detection
};

/*****************************************************************************/
//	BURST SIZE IN CHCTRL REGISTER
/*****************************************************************************/
enum
{
	DMA_BURST1,		// 1 pair of read/write per 1 hop
	DMA_BURST2, 	// 2 pair of read/write per 1 hop
	DMA_BURST4		// 4 pair of read/write per 1 hop
};

/*****************************************************************************/
//	WORD SIZE IN CHCTRL REGISTER
/*****************************************************************************/
enum
{
	DMA_BYTE, 		// 1 byte
	DMA_HALFWORD, 	// 2 byte
	DMA_WORD		// 4 byte
};

/*****************************************************************************/
//	BURST Transfer
/*****************************************************************************/
enum
{
	DMA_ARBIT_MODE = 0, // DMA transfer with arbitration
	DMA_BURST_MODE		// DMA transfer with no arbitration (burst operation)
};

/*****************************************************************************/
// field of the DMA register SPARAM, DPARAM
//    mask : address mask field in registers
//    inc : address increment in registers
/*****************************************************************************/
typedef struct
{
	unsigned int inc :8;
	unsigned int mask :24;
} DMAPARAM;

#if 0//def CD_CTRL_DSP	//CDDMA
typedef	volatile struct
{
	unsigned	ST_SADR;
	unsigned	SPARAM;
	unsigned	_SPARAM;
	unsigned	C_SADR;
	unsigned	ST_DADR;
	unsigned	DPARAM;
	unsigned	_DPARAM;
	unsigned	C_DADR;
	unsigned	HCOUNT;
	unsigned	CHCTRL;
	unsigned	RPTCTRL;
} sHwDMA;

#define	CDDIDMA_BUFFER_SIZE	256
#endif

/*****************************************************************************/
// field of the DMA register CHCTRL
/*****************************************************************************/
typedef struct
{
	unsigned int en:1;		// bit[0]
	unsigned int rep:1;		// bit[1]
	unsigned int ien:1;		// bit[2]
	unsigned int flag:1;	// bit[3]
	unsigned int wsize:2;	// bit[4 - 5]
	unsigned int bsize:2;	// bit[6 - 7]
	unsigned int type:2;	// bit[8 - 9]
	unsigned int bst:1;		// bit[10]
	unsigned int lock:1;	// bit[11]
	unsigned int hrd:1;		// bit[12]
	unsigned int sync:1;	// bit[13]
	unsigned int :1;		// bit[14]
	unsigned int cont:1;	// bit[15]
	unsigned int dmasel:13;	// bit[16 - 28]
	unsigned int :3;		// bit[29 - 31]
} CHCTRL;

/*****************************************************************************/
// DMA registers
/*****************************************************************************/
typedef struct
{
	unsigned long st_sadr;      // ST_SADR
	DMAPARAM sparam;  			// SPARAM
	unsigned long res1;         // reserved
	unsigned long c_sadr;		// C_SADR
	unsigned long st_dadr;		// ST_DADR
	DMAPARAM dparam;			// DPARAM
	unsigned long res2;			// reserved
	unsigned long c_dadr;		// C_DADR
	unsigned long hcount;		// HCOUNT
	CHCTRL chctrl;				// CHCTRL
} DMAC_REG;

#define _dmac_reg0       ((volatile DMAC_REG *)&(HwST_SADR0))	// DMA CHANNEL 0
#define _dmac_reg1       ((volatile DMAC_REG *)&(HwST_SADR1))	// DMA CHANNEL 1
#if 1
volatile DMAC_REG *dmac_reg(int ch);
#else
#define	dmac_reg(ch)	( (volatile DMAC_REG *)( ((ch) == 0) ? (&(HwST_SADR0)) : (&(HwST_SADR1)) ))
#endif

/*****************************************************************************/
// CHANNEL CONFIGURATION.
/*****************************************************************************/
#define DMAC_IS1		Hw22
#define DMAC_IS0		Hw21
#define DMAC_MIS1		Hw17
#define DMAC_MIS0		Hw16

#define DMAC_SWP1		Hw9
#define DMAC_SWP0		Hw8
#define DMAC_PRI		Hw4
#define DMAC_FIX		Hw0

/*****************************************************************************/
// Macro Functions for DMA.
/*****************************************************************************/

// ch : channel number have to be a 0 or 1.
#define dmac_reset(ch)	do{										\
		memset((void *)dmac_reg(ch), 0, sizeof(DMAC_REG));	\
	}while(0)

#define dmac_disable(ch)  do{                                \
		dmac_reg(ch)->chctrl.en	= 0;		\
		dmac_reg(ch)->chctrl.flag	= 1;		\
	}while(0)

#define dmac_enable(ch)  do{                        \
		dmac_reg(ch)->chctrl.en	= 1;		\
	}while(0)

#define dmac_clear(ch)    do{                      \
		dmac_reg(ch)->chctrl.flag	= 1;		\
	}while(0)

#define dmac_set_cfg(ch, a, b, c, d, e, f, g, h, i, j, zz) do{  \
		(*(volatile unsigned *)(&(dmac_reg(ch)->chctrl)))	= ((a) << 8)  |    \
		        ((b) << 10) |     \
		        ((c) << 11) |     \
		        ((d) << 15) |     \
		        ((e) << 13) |     \
		        ((f) << 6)  |     \
		        ((g) << 4)  |     \
		        ((h) << 12) |     \
		        ((i) << 2)  |     \
		        ((j) << 1);       \
		dmac_reg(ch)->hcount = (zz);   \
	}while(0)


#define dmac_set_src_addr(ch, x, y, z) do{       \
		dmac_reg(ch)->st_sadr = (unsigned long)(x);    \
		dmac_reg(ch)->sparam.inc = (((unsigned int)(y)) & 0x000000ff);              \
		dmac_reg(ch)->sparam.mask = (((unsigned int)(z)) & 0x0ffffff0) >> 4; \
	}while(0)

#define dmac_set_dst_addr(ch, x, y, z) do{  \
		dmac_reg(ch)->st_dadr = (unsigned long)(x);    \
		dmac_reg(ch)->dparam.inc =  (((unsigned int)(y)) & 0x000000ff);      \
		dmac_reg(ch)->dparam.mask = (((unsigned int)(z))& 0x0ffffff0) >> 4;     \
	}while(0)

// if burst 4 pair & 4 bytes word size, then
#define HOP_PER_DMA    32
#define TOTAL_WORDS    (HOP_PER_DMA * 16)   // 32 * 16 bytes = 512 bytes (1 sectors)

extern int dmac_is_done(int ch);
extern int DMA_SwTransfer(unsigned char channel, const void *src, const void *dst,
                          unsigned long hcount, unsigned int word_size,
                          unsigned int src_inc, unsigned int des_inc);
extern void DMA_Config(unsigned char channel, unsigned char swap_flag);

#endif	// __DMA_H__

/* end of file */

