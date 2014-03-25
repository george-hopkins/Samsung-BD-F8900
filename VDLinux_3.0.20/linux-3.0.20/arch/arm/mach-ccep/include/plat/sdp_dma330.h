/* linux/include/linux/amba/pl330.h
 *
 * Copyright (C) 2010 Samsung Electronics Co. Ltd.
 *	Jaswinder Singh <jassi.brar@samsung.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

 /* arch/arm/mach-ccep/include/plat/sdp_dma330.h
  *
  * modified by dongseok lee <drain.lee@samsung.com>
  *
  * 20120703	drain.lee	Create file.
  * 20120703	drain.lee	add plat init field in sdp_dma330_platdata.
  * 20120703	drain.lee	add clock gating field in sdp_dma330_platdata.
  * 20121220	drain.lee	add Export function prototype
  */


#ifndef	__SDP_DMA_330_H_
#define	__SDP_DMA_330_H_

#include <asm/hardware/pl330.h>

struct dma_pl330_peri {
	/*
	 * Peri_Req i/f of the DMAC that is
	 * peripheral could be reached from.
	 */
	u8 peri_id; /* {0, 31} */
	enum pl330_reqtype rqtype;

	/* For M->D and D->M Channels */
	int burst_sz; /* in power of 2 */
	dma_addr_t fifo_addr;
};

struct sdp_dma330_platdata {
	/*
	 * Number of valid peripherals connected to DMAC.
	 * This may be different from the value read from
	 * CR0, as the PL330 implementation might have 'holes'
	 * in the peri list or the peri could also be reached
	 * from another DMAC which the platform prefers.
	 */
	u8 nr_valid_peri;
	/* Array of valid peripherals */
	struct dma_pl330_peri *peri;
	/* Bytes to allocate for MC buffer */
	unsigned mcbuf_sz;

	int (*plat_init)(void);

#ifdef CONFIG_SDP_CLOCK_GATING
	/* address of used count */
	int *plat_clk_used_cnt;
	/* clock setting 0Hz */
	int (*plat_clk_gate)(void);
	/* restore clock value */
	int (*plat_clk_ungate)(void);
#endif/* CONFIG_SDP_CLOCK_GATING */

};

extern bool sdp_dma330_is_cpu_dma(struct dma_chan * chan);

extern struct dma_async_tx_descriptor *
sdp_dma330_cache_ctrl(struct dma_chan *chan,
	struct dma_async_tx_descriptor *tx, u32 dst_cache, u32 src_cache);

#endif	/* __SDP_DMA_330_H_ */
