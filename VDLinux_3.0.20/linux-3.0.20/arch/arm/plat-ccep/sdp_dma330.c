/* linux/drivers/dma/sdp_dma330.c
 *
 * Copyright (C) 2010 Samsung Electronics Co. Ltd.
 *	Jaswinder Singh <jassi.brar@samsung.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

 /* arch/arm/plat-ccep/sdp_dma330.c
  *
  * modified by dongseok lee <drain.lee@samsung.com>
  *
  * 20120702	drain.lee	porting for kernel 3.0.20
  * 20120703	drain.lee	platform init code move to sdpxxxx.c file.
  * 20120703	drain.lee	support clock gating.
  * 20120714	drain.lee	bugfix complete src unmap.
  * 20121009	drain.lee	fix, clock gating.
  * 20121204	drain.lee	fix prevent defect and compile warning
  * 20121220	drain.lee	fix compile warning.
  * 20130201	drain.lee	fix prevent defect(Uninitialized scalar variable)
  */

#include <linux/io.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/dmaengine.h>
#include <linux/interrupt.h>
#include <linux/amba/bus.h>

#include <plat/sdp_dma330.h>

#define VERSION_SRTING		"20130201"

#ifdef CONFIG_ARCH_SDP1103
typedef irqreturn_t (*intc_merg_isr)(int irq, void* dev_id);
int request_irq_intc_merg(int n_int, void* dev_id, intc_merg_isr fp);
int release_irq_intc_merg(int n_int, void* dev_id, intc_merg_isr fp);
#endif

#define NR_DEFAULT_DESC	16

enum desc_status {
	/* In the DMAC pool */
	FREE,
	/*
	 * Allocted to some channel during prep_xxx
	 * Also may be sitting on the work_list.
	 */
	PREP,
	/*
	 * Sitting on the work_list and already submitted
	 * to the PL330 core. Not more than two descriptors
	 * of a channel can be BUSY at any time.
	 */
	BUSY,
	/*
	 * Sitting on the channel work_list but xfer done
	 * by PL330 core
	 */
	DONE,
};

struct dma_pl330_chan {
	/* Schedule desc completion */
	struct tasklet_struct task;

	/* DMA-Engine Channel */
	struct dma_chan chan;

	/* Last completed cookie */
	dma_cookie_t completed;

	/* List of to be xfered descriptors */
	struct list_head work_list;

	/* Pointer to the DMAC that manages this channel,
	 * NULL if the channel is available to be acquired.
	 * As the parent, this DMAC also provides descriptors
	 * to the channel.
	 */
	struct dma_pl330_dmac *dmac;

	/* To protect channel manipulation */
	spinlock_t lock;

	/* Token of a hardware channel thread of PL330 DMAC
	 * NULL if the channel is available to be acquired.
	 */
	void *pl330_chid;
};

struct dma_pl330_dmac {
	struct pl330_info pif;

	/* DMA-Engine Device */
	struct dma_device ddma;

	/* Pool of descriptors available for the DMAC's channels */
	struct list_head desc_pool;
	/* To protect desc_pool manipulation */
	spinlock_t pool_lock;

	struct sdp_dma330_platdata *platdata;

	/* Peripheral channels connected to this DMAC */
	struct dma_pl330_chan peripherals[0]; /* keep at end */
};

struct dma_pl330_desc {
	/* To attach to a queue as child */
	struct list_head node;

	/* Descriptor for the DMA Engine API */
	struct dma_async_tx_descriptor txd;

	/* Xfer for PL330 core */
	struct pl330_xfer px;

	struct pl330_reqcfg rqcfg;
	struct pl330_req req;

	enum desc_status status;

	/* The channel which currently holds this desc */
	struct dma_pl330_chan *pchan;
};

static inline struct dma_pl330_chan *
to_pchan(struct dma_chan *ch)
{
	if (!ch)
		return NULL;

	return container_of(ch, struct dma_pl330_chan, chan);
}

static inline struct dma_pl330_desc *
to_desc(struct dma_async_tx_descriptor *tx)
{
	return container_of(tx, struct dma_pl330_desc, txd);
}

static inline void free_desc_list(struct list_head *list)
{
	struct dma_pl330_dmac *pdmac = NULL;
	struct dma_pl330_desc *desc = NULL;
	struct dma_pl330_chan *pch = NULL;
	unsigned long flags;

	if (list_empty(list))
		return;

	/* Finish off the work list */
	list_for_each_entry(desc, list, node) {
		dma_async_tx_callback callback;
		void *param;

		/* All desc in a list belong to same channel */
		pch = desc->pchan;
		callback = desc->txd.callback;
		param = desc->txd.callback_param;

		if (callback)
			callback(param);

		desc->pchan = NULL;
	}

	pdmac = pch->dmac;

	spin_lock_irqsave(&pdmac->pool_lock, flags);
	list_splice_tail_init(list, &pdmac->desc_pool);
	spin_unlock_irqrestore(&pdmac->pool_lock, flags);
}


#if 0
static inline struct pl330_xfer *
alloc_xferlist(struct dma_pl330_desc *desc, int len)
{
	return kmalloc(sizeof(struct pl330_xfer) * len, GFP_KERNEL);
}
static inline void free_xferlist(struct dma_pl330_desc *desc)
{
	struct pl330_xfer *px = desc->px.next;
	for(px = desc->px.next; px; px = px->next)
	{
		kfree(px);
	}
	desc->px.next = 0;
}
#else
#include <linux/vmalloc.h>
static inline struct pl330_xfer *
alloc_xferlist(struct dma_pl330_desc *desc, size_t len)
{
	return kmalloc(sizeof(struct pl330_xfer) * len, GFP_KERNEL);
}
static inline void
free_xferlist(struct dma_pl330_desc *desc)
{
	struct pl330_xfer *px = desc->px.next;
	kfree(px);
	desc->px.next = 0;
}

#endif


#ifdef CONFIG_SDP_CLOCK_GATING

/* all DMAC shered value */
static DEFINE_SPINLOCK(g_clock_lock);

static int sdp_dma330_clk_get(struct dma_pl330_dmac *pdmac)
{
	struct sdp_dma330_platdata *plat = pdmac->platdata;
	struct pl330_info *pif = &pdmac->pif;
	int ret = 0;

	spin_lock(&g_clock_lock);

	BUG_ON(!plat->plat_clk_used_cnt);
	BUG_ON((*plat->plat_clk_used_cnt) < 0);

	if((*plat->plat_clk_used_cnt) == 0) {
		if(plat->plat_clk_ungate) {
			plat->plat_clk_ungate();
			/* Check DMA330 Manager Status Register (0x000). */
			while(((readl(pif->base)&0xF) != 0x0)
				&& ((readl(pif->base)&0xF) != 0x0));
		} else {
			ret = -EINVAL;
			dev_err(pdmac->pif.dev, "%s: plat_ungate_clock is NULL!!!\n", __FUNCTION__);
		}
	}

	(*plat->plat_clk_used_cnt)++;

	spin_unlock(&g_clock_lock);

	return 0;
}

static int sdp_dma330_clk_put(struct dma_pl330_dmac *pdmac)
{
	struct sdp_dma330_platdata *plat = pdmac->platdata;
	struct pl330_info *pif = &pdmac->pif;
	int ret = 0;

	spin_lock(&g_clock_lock);

	BUG_ON(!plat->plat_clk_used_cnt);
	BUG_ON((*plat->plat_clk_used_cnt) <= 0);

	(*plat->plat_clk_used_cnt)--;

	if((*plat->plat_clk_used_cnt) == 0) {
		if(plat->plat_clk_gate) {
			/* Check DMA330 Manager Status Register (0x000). */
			while(((readl(pif->base)&0xF) != 0x0)
				&& ((readl(pif->base)&0xF) != 0x0));
			plat->plat_clk_gate();
		} else {
			ret = -EINVAL;
			dev_err(pdmac->pif.dev, "%s: plat_gate_clock is NULL!!!\n", __FUNCTION__);
		}
	}
	spin_unlock(&g_clock_lock);

	return 0;
}

#else

static inline int sdp_dma330_clk_get(struct dma_pl330_dmac *pdmac)
{
	return 0;
}

static inline int sdp_dma330_clk_put(struct dma_pl330_dmac *pdmac)
{
	return 0;
}
#endif/* CONFIG_SDP_CLOCK_GATING */


static inline void fill_queue(struct dma_pl330_chan *pch)
{
	struct dma_pl330_desc *desc;
	int ret;

	list_for_each_entry(desc, &pch->work_list, node) {

		/* If already submitted */
		if (desc->status == BUSY)
			break;

		ret = pl330_submit_req(pch->pl330_chid,
						&desc->req);
		if (!ret) {
			desc->status = BUSY;
			break;
		} else if (ret == -EAGAIN) {
			/* QFull or DMAC Dying */
			dev_dbg(pch->dmac->pif.dev, "%s:%d QFull or DMAC Dying(%d)\n",
					__func__, __LINE__, desc->txd.cookie);
			break;
		} else {
			/* Unacceptable request */
			desc->status = DONE;
			dev_err(pch->dmac->pif.dev, "%s:%d Bad Desc(%d)\n",
					__func__, __LINE__, desc->txd.cookie);
			tasklet_schedule(&pch->task);
		}
	}
}

static void pl330_tasklet(unsigned long data)
{
	struct dma_pl330_chan *pch = (struct dma_pl330_chan *)data;
	struct dma_pl330_desc *desc, *_dt;
	unsigned long flags;
	LIST_HEAD(list);

	spin_lock_irqsave(&pch->lock, flags);

	/* Pick up ripe tomatoes */
	list_for_each_entry_safe(desc, _dt, &pch->work_list, node)
		if (desc->status == DONE) {
			/* add dmaengine cache op 20120406 by dongseok.lee */
			struct dma_async_tx_descriptor *txd = &desc->txd;
			struct pl330_xfer *px = NULL;
			if(!(txd->flags & DMA_COMPL_SKIP_DEST_UNMAP)) {
				if( txd->flags & DMA_COMPL_DEST_UNMAP_SINGLE ) {
					for(px = &desc->px; px != NULL; px = px->next)
						dma_unmap_single(txd->chan->device->dev,
							px->dst_addr, px->bytes, DMA_FROM_DEVICE);
				}
				else
				{
					for(px = &desc->px; px != NULL; px = px->next)
						dma_unmap_page(txd->chan->device->dev,
							px->dst_addr, px->bytes, DMA_FROM_DEVICE);
				}
			}

			if(!(txd->flags & DMA_COMPL_SKIP_SRC_UNMAP)) {
				if( txd->flags & DMA_COMPL_SRC_UNMAP_SINGLE ) {
					for(px = &desc->px; px != NULL; px = px->next)
						dma_unmap_single(txd->chan->device->dev,
							px->src_addr, px->bytes, DMA_TO_DEVICE);
				}
				else
				{
					for(px = &desc->px; px != NULL; px = px->next)
						dma_unmap_page(txd->chan->device->dev,
							px->src_addr, px->bytes, DMA_TO_DEVICE);
				}
			}

			pch->completed = desc->txd.cookie;
			/* free xfer list add drain.lee */
			free_xferlist(desc);
			list_move_tail(&desc->node, &list);
		}

	/* Try to submit a req imm. next to the last completed cookie */
	fill_queue(pch);

	/* Make sure the PL330 Channel thread is active */
	pl330_chan_ctrl(pch->pl330_chid, PL330_OP_START);

	spin_unlock_irqrestore(&pch->lock, flags);

	free_desc_list(&list);
}

static void dma_pl330_rqcb(void *token, enum pl330_op_err err)
{
	struct dma_pl330_desc *desc = token;
	struct dma_pl330_chan *pch = desc->pchan;
	unsigned long flags;

	/* If desc aborted */
	if (!pch)
		return;

	spin_lock_irqsave(&pch->lock, flags);

	desc->status = DONE;

	sdp_dma330_clk_put(pch->dmac);/* clk gateing */

	spin_unlock_irqrestore(&pch->lock, flags);

	tasklet_schedule(&pch->task);
}

static int pl330_alloc_chan_resources(struct dma_chan *chan)
{
	struct dma_pl330_chan *pch = to_pchan(chan);
	struct dma_pl330_dmac *pdmac = pch->dmac;
	unsigned long flags;

	spin_lock_irqsave(&pch->lock, flags);

	pch->completed = chan->cookie = 1;

	pch->pl330_chid = pl330_request_channel(&pdmac->pif);
	if (!pch->pl330_chid) {
		spin_unlock_irqrestore(&pch->lock, flags);
		return 0;
	}

	tasklet_init(&pch->task, pl330_tasklet, (unsigned long) pch);

	spin_unlock_irqrestore(&pch->lock, flags);

	return 1;
}

static int pl330_control(struct dma_chan *chan, enum dma_ctrl_cmd cmd, unsigned long arg)
{
	struct dma_pl330_chan *pch = to_pchan(chan);
	struct dma_pl330_desc *desc;
	unsigned long flags;

	/* Only supports DMA_TERMINATE_ALL */
	if (cmd != DMA_TERMINATE_ALL)
		return -ENXIO;

	spin_lock_irqsave(&pch->lock, flags);

	/* FLUSH the PL330 Channel thread */
	pl330_chan_ctrl(pch->pl330_chid, PL330_OP_FLUSH);

	/* Mark all desc done */
	list_for_each_entry(desc, &pch->work_list, node)
		desc->status = DONE;

	spin_unlock_irqrestore(&pch->lock, flags);

	pl330_tasklet((unsigned long) pch);

	return 0;
}

static void pl330_free_chan_resources(struct dma_chan *chan)
{
	struct dma_pl330_chan *pch = to_pchan(chan);
	unsigned long flags;

	spin_lock_irqsave(&pch->lock, flags);

	tasklet_kill(&pch->task);

	pl330_release_channel(pch->pl330_chid);
	pch->pl330_chid = NULL;

	spin_unlock_irqrestore(&pch->lock, flags);
}

static enum dma_status
pl330_tx_status(struct dma_chan *chan, dma_cookie_t cookie,
		 struct dma_tx_state *txstate)
{
	struct dma_pl330_chan *pch = to_pchan(chan);
	dma_cookie_t last_done, last_used;
	int ret;

	last_done = pch->completed;
	last_used = chan->cookie;

	ret = dma_async_is_complete(cookie, last_done, last_used);

	dma_set_tx_state(txstate, last_done, last_used, 0);

	return ret;
}

static void pl330_issue_pending(struct dma_chan *chan)
{
	pl330_tasklet((unsigned long)(void *)to_pchan(chan));
}

/*
 * We returned the last one of the circular list of descriptor(s)
 * from prep_xxx, so the argument to submit corresponds to the last
 * descriptor of the list.
 */
static dma_cookie_t pl330_tx_submit(struct dma_async_tx_descriptor *tx)
{
	struct dma_pl330_desc *desc, *last = to_desc(tx);
	struct dma_pl330_chan *pch = to_pchan(tx->chan);
	dma_cookie_t cookie;
	unsigned long flags;

	spin_lock_irqsave(&pch->lock, flags);

	sdp_dma330_clk_get(pch->dmac);/* clk gateing */

	/* Assign cookies to all nodes */
	cookie = tx->chan->cookie;

	while (!list_empty(&last->node)) {
		desc = list_entry(last->node.next, struct dma_pl330_desc, node);

		if (++cookie < 0)
			cookie = 1;
		desc->txd.cookie = cookie;

		list_move_tail(&desc->node, &pch->work_list);
	}

	if (++cookie < 0)
		cookie = 1;
	last->txd.cookie = cookie;

	list_add_tail(&last->node, &pch->work_list);

	tx->chan->cookie = cookie;

	spin_unlock_irqrestore(&pch->lock, flags);

	return cookie;
}

static inline void _init_desc(struct dma_pl330_desc *desc)
{
	desc->pchan = NULL;
	desc->req.x = &desc->px;
	desc->req.token = desc;
	desc->rqcfg.swap = SWAP_NO;
	desc->rqcfg.privileged = 0;
	desc->rqcfg.insnaccess = 0;
	desc->rqcfg.scctl = SCCTRL0;
	desc->rqcfg.dcctl = DCCTRL0;
	desc->req.cfg = &desc->rqcfg;
	desc->req.xfer_cb = dma_pl330_rqcb;
	desc->txd.tx_submit = pl330_tx_submit;

	INIT_LIST_HEAD(&desc->node);
}

/* Returns the number of descriptors added to the DMAC pool */
static u8 add_desc(struct dma_pl330_dmac *pdmac, gfp_t flg, u8 count)
{
	struct dma_pl330_desc *desc;
	unsigned long flags;
	u8 i;

	if (!pdmac)
		return 0;

	desc = kmalloc(count * sizeof(*desc), flg);
	if (!desc)
		return 0;

	spin_lock_irqsave(&pdmac->pool_lock, flags);

	for (i = 0; i < count; i++) {
		_init_desc(&desc[i]);
		list_add_tail(&desc[i].node, &pdmac->desc_pool);
	}

	spin_unlock_irqrestore(&pdmac->pool_lock, flags);

	return count;
}

static struct dma_pl330_desc *
pluck_desc(struct dma_pl330_dmac *pdmac)
{
	struct dma_pl330_desc *desc = NULL;
	unsigned long flags;

	if (!pdmac)
		return NULL;

	spin_lock_irqsave(&pdmac->pool_lock, flags);

	if (!list_empty(&pdmac->desc_pool)) {
		desc = list_entry(pdmac->desc_pool.next,
				struct dma_pl330_desc, node);

		list_del_init(&desc->node);

		desc->status = PREP;
		desc->txd.callback = NULL;
	}

	spin_unlock_irqrestore(&pdmac->pool_lock, flags);

	return desc;
}

static struct dma_pl330_desc *pl330_get_desc(struct dma_pl330_chan *pch)
{
	struct dma_pl330_dmac *pdmac = pch->dmac;
	struct dma_pl330_peri *peri = pch->chan.private;
	struct dma_pl330_desc *desc;

	/* Pluck one desc from the pool of DMAC */
	desc = pluck_desc(pdmac);

	/* If the DMAC pool is empty, alloc new */
	if (!desc) {
		if (!add_desc(pdmac, GFP_ATOMIC, 1))
			return NULL;

		/* Try again */
		desc = pluck_desc(pdmac);
		if (!desc) {
			dev_err(pch->dmac->pif.dev,
				"%s:%d ALERT!\n", __func__, __LINE__);
			return NULL;
		}
	}

	/* Initialize the descriptor */
	desc->pchan = pch;
	desc->txd.cookie = 0;
	async_tx_ack(&desc->txd);

	desc->req.rqtype = peri->rqtype;
	desc->req.peri = peri->peri_id;

	dma_async_tx_descriptor_init(&desc->txd, &pch->chan);

	return desc;
}

static inline void fill_px(struct pl330_xfer *px,
		dma_addr_t dst, dma_addr_t src, size_t len)
{
	px->next = NULL;
	px->bytes = len;
	px->dst_addr = dst;
	px->src_addr = src;
}

static struct dma_pl330_desc *
__pl330_prep_dma_memcpy(struct dma_pl330_chan *pch, dma_addr_t dst,
		dma_addr_t src, size_t len)
{
	struct dma_pl330_desc *desc = pl330_get_desc(pch);

	if (!desc) {
		dev_err(pch->dmac->pif.dev, "%s:%d Unable to fetch desc\n",
			__func__, __LINE__);
		return NULL;
	}

	/*
	 * Ideally we should lookout for reqs bigger than
	 * those that can be programmed with 256 bytes of
	 * MC buffer, but considering a req size is seldom
	 * going to be word-unaligned and more than 200MB,
	 * we take it easy.
	 * Also, should the limit is reached we'd rather
	 * have the platform increase MC buffer size than
	 * complicating this API driver.
	 */
	fill_px(&desc->px, dst, src, len);

	return desc;
}

/* Call after fixing burst size */
static inline u32 get_burst_len(struct dma_pl330_desc *desc, size_t len)
{
	struct dma_pl330_chan *pch = desc->pchan;
	struct pl330_info *pi = &pch->dmac->pif;
	u32 burst_len;

	burst_len = pi->pcfg.data_bus_width / 8U;
	burst_len *= pi->pcfg.data_buf_dep;
	burst_len >>= desc->rqcfg.brst_size;

	/* src/dst_burst_len can't be more than 16 */
	if (burst_len > 16)
		burst_len = 16;

	while (burst_len > 1) {
//		if (!(len % (burst_len << desc->rqcfg.brst_size)))
		if (len >= (burst_len << desc->rqcfg.brst_size))
			break;
		burst_len--;// = burst_len >> 1;//burst len is 16, 8, 4, 2, 1
	}

	return burst_len;
}

static struct dma_async_tx_descriptor *
pl330_prep_dma_memcpy(struct dma_chan *chan, dma_addr_t dst,
		dma_addr_t src, size_t len, unsigned long flags)
{
	struct dma_pl330_desc *desc;
	struct dma_pl330_chan *pch = to_pchan(chan);
	struct dma_pl330_peri *peri = chan->private;
	struct pl330_info *pi;
	u32 burst;

	if (unlikely(!pch || !len || !peri))
		return NULL;

	if (peri->rqtype != MEMTOMEM)
		return NULL;

	pi = &pch->dmac->pif;

	desc = __pl330_prep_dma_memcpy(pch, dst, src, len);
	if (!desc)
		return NULL;

	desc->rqcfg.src_inc = 1;
	desc->rqcfg.dst_inc = 1;

	/* Select max possible burst size */
	burst = pi->pcfg.data_bus_width / 8U;

	while (burst > 1) {
//		if (!(len % burst))
		/* max burst is less eq then data len */
		if (len >= burst)
		{
			break;
		}
		burst /= 2;
	}

	desc->rqcfg.brst_size = 0;
	while (burst != (1U << desc->rqcfg.brst_size))
		desc->rqcfg.brst_size++;

	desc->rqcfg.brst_len = get_burst_len(desc, len);

	desc->txd.flags = flags;

	return &desc->txd;
}


/* add Memory Copy use scatterlist */
static struct dma_async_tx_descriptor *
sdp_dma330_prep_dma_memcpy_sg(struct dma_chan *chan,
		struct scatterlist *dstsgl, struct scatterlist *srcsgl,
		size_t sg_len, unsigned long flags)
{
	struct dma_pl330_desc *desc;
	struct dma_pl330_chan *pch = to_pchan(chan);
	struct dma_pl330_peri *peri = chan->private;
	struct pl330_info *pi;
	int burst;
	struct pl330_xfer *prepx, *px = NULL;
	struct scatterlist *curdst, *cursrc;
	LIST_HEAD(list);

	if (unlikely(!pch || !sg_len || !peri))
		return NULL;

	if (unlikely(peri->rqtype != MEMTOMEM))
		return NULL;

	pi = &pch->dmac->pif;

	/* first address*/
	dev_dbg(pch->dmac->pif.dev, "scatterlist from 0x%08x to 0x%08x len 0x%x(%d)",
		sg_dma_address(srcsgl), sg_dma_address(dstsgl),
		sg_dma_len(dstsgl), sg_dma_len(dstsgl));

	desc = __pl330_prep_dma_memcpy(pch,
		sg_dma_address(dstsgl), sg_dma_address(srcsgl), sg_dma_len(dstsgl));

	if (unlikely(!desc))
		return NULL;

	/* second address ~ last */
	prepx = &desc->px;
#if 1
	px = alloc_xferlist(desc, sg_len-1);
	if(!px)
	{
		dev_err(pch->dmac->pif.dev, "error alloc_xferlist() return error %p\n", px);
		goto free_desc;
	}
	memset(px, 0, sizeof(*px) * (sg_len-1));
#endif
	for(curdst = sg_next(dstsgl), cursrc = sg_next(srcsgl);
		curdst && cursrc;
		curdst = sg_next(curdst), cursrc = sg_next(cursrc))
	{
		dev_dbg(pch->dmac->pif.dev,
			"scatterlist from 0x%08x to 0x%08x len 0x%x(%d)",
			sg_dma_address(cursrc), sg_dma_address(curdst),
			sg_dma_len(curdst), sg_dma_len(curdst));

		//px = kzalloc(sizeof(*px), GFP_KERNEL);
		if(IS_ERR_OR_NULL(px))
		{
			dev_err(pch->dmac->pif.dev, "px is NULL %p\n", px);
			goto free_desc;
		}
		fill_px(px, sg_dma_address(curdst), sg_dma_address(cursrc), sg_dma_len(curdst));

		prepx->next = px;
		prepx = px;

		px++;
	}

	/* chack both NULL*/
	if(unlikely(curdst || cursrc))
	{
		dev_err(pch->dmac->pif.dev, "is not the same as the length of the scatterlist.\n");
		/* TODO: free pl330_xfer list and desc */
		free_xferlist(desc);
		goto free_desc;
	}

	desc->rqcfg.src_inc = 1;
	desc->rqcfg.dst_inc = 1;

	/* Select max possible burst size */
	burst = pi->pcfg.data_bus_width / 8;
#if 0
	while (burst > 1) {
//		if (!(len % burst))
		/* max burst is less eq then data len */
		if (len >= burst)
		{
			break;
		}
		burst /= 2;
	}
#endif
	desc->rqcfg.brst_size = 0;
	while (burst != (1 << desc->rqcfg.brst_size))
		desc->rqcfg.brst_size++;

//	desc->rqcfg.brst_size--;///set 32bit

//	desc->rqcfg.brst_len = get_burst_len(desc, len);
	desc->rqcfg.brst_len = 16;

	desc->txd.flags = flags;

	return &desc->txd;

free_desc:
	list_move_tail(&desc->node, &list);
	free_desc_list(&list);
	return NULL;
}

static struct dma_async_tx_descriptor *
sdp_dma330_prep_dma_sg(
		struct dma_chan *chan,
		struct scatterlist *dst_sg, unsigned int dst_nents,
		struct scatterlist *src_sg, unsigned int src_nents,
		unsigned long flags)
{
	if(dst_nents == src_nents)
		return sdp_dma330_prep_dma_memcpy_sg(chan, dst_sg, src_sg, src_nents, flags);
	else
		return NULL;
}


#ifdef CONFIG_SDP_DMA330_2DCOPY
/* add 2D Copy*/
#define MIN_BURST_LEN	(8)
#define MAX_LINE_WIDTH (MIN_BURST_LEN*256)
struct dma_async_tx_descriptor *
sdp_dma330_prep_dma_2dcopy(struct dma_chan *chan,
		dma_addr_t dst, int dst_linespan_px,
		dma_addr_t src, int src_linespan_px,
		int width, int height, int bpp, unsigned long flags)
{
	struct dma_pl330_desc *desc;
	struct dma_pl330_chan *pch = to_pchan(chan);
	struct dma_pl330_peri *peri = chan->private;
	struct pl330_info *pi;
	int burst;
	struct pl330_xfer *px = NULL;

	if (unlikely(!pch || !peri))
		return NULL;

	if (unlikely(peri->rqtype != MEMTOMEM))
		return NULL;

	if (unlikely(width > MAX_LINE_WIDTH))
		return NULL;

	/* chack bpp aligned 8bit.*/
	if (unlikely((bpp&0x7) != 0))
		return NULL;

	pi = &pch->dmac->pif;

	desc = __pl330_prep_dma_memcpy(pch, dst, src, width*height*bpp);

	if (unlikely(!desc))
		return NULL;

	px = &desc->px;

	desc->req.rqtype = MEMTOMEM_2D;
	px->src_linespan_bytes = src_linespan_px*bpp/8;
	px->dst_linespan_bytes = dst_linespan_px*bpp/8;
	px->line_bytes = width*bpp/8;
	px->height = height;

	desc->rqcfg.src_inc = 1;
	desc->rqcfg.dst_inc = 1;

	/* Select max possible burst size */
	burst = pi->pcfg.data_bus_width / 8;

	while (burst > 1) {
		if(!(
			(px->line_bytes % burst)
			|| (px->src_addr % burst)
			|| (px->dst_addr % burst)
			|| (px->src_linespan_bytes % burst)
			|| (px->dst_linespan_bytes % burst) ))
		{
			break;
		}
		burst /= 2;
	}

	BUG_ON(burst == 0);

	desc->rqcfg.brst_size = 0;
	while (burst != (1 << desc->rqcfg.brst_size))
		desc->rqcfg.brst_size++;

	/* finding aligned burst size (MIN_BURST_LEN~16) */
	desc->rqcfg.brst_len = 16;
	while(desc->rqcfg.brst_len > MIN_BURST_LEN)
	{
		if(!(px->line_bytes%(burst*desc->rqcfg.brst_len)))
		{
			break;
		}
		desc->rqcfg.brst_len--;
	}



	desc->txd.flags = flags;

	return &desc->txd;
}
EXPORT_SYMBOL(sdp_dma330_prep_dma_2dcopy);
#endif /* CONFIG_SDP_DMA330_2DCOPY */


/* if DMAC is secure mode, return true */
bool sdp_dma330_is_cpu_dma(struct dma_chan * chan)
{
	struct dma_pl330_chan *pchan = to_pchan(chan);

	return !(pchan->dmac->pif.pcfg.mode&DMAC_MODE_NS);
}
EXPORT_SYMBOL(sdp_dma330_is_cpu_dma);

/* CPU DMA330 Cache Control */
struct dma_async_tx_descriptor *
sdp_dma330_cache_ctrl(struct dma_chan *chan,
	struct dma_async_tx_descriptor *tx, u32 dst_cache, u32 src_cache)
{
	struct dma_pl330_desc *desc = to_desc(tx);

	/* TODO: cache valid chack */
	desc->rqcfg.scctl = src_cache;
	desc->rqcfg.dcctl = dst_cache;

	return tx;
}
EXPORT_SYMBOL(sdp_dma330_cache_ctrl);


#if 0
static struct pl330_xfer *
_add_xfer(struct dma_pl330_dmac *pdmac, gfp_t flg, int count)
{
	return NULL;
}
static struct pl330_xfer *
_pluck_xfer(struct dma_pl330_dmac *pdmac, gfp_t flg, int count)
{
	return NULL;
}
static struct pl330_xfer *
pl330_get_xfer()
{
	pl330_xfer *px = kzalloc(sizeof(*px));
	return px;
}

/* add Memory Copy use scatterlist */
static struct dma_async_tx_descriptor *
sdp_dma330_prep_dma_memcpy_sg(struct dma_chan *chan,
		struct scatterlist *dstsgl, struct scatterlist *srcsgl,
		size_t sg_len, unsigned long flags)
{
	struct dma_pl330_desc *first = NULL, *desc;
	struct dma_pl330_chan *pch = to_pchan(chan);
	struct dma_pl330_peri *peri = chan->private;
	struct pl330_info *pi;
	struct pl330_xfer *prepx, *px;
	int burst;
	int i = 0;

	if (unlikely(!pch || !len || !peri))
		return NULL;

	if (peri->rqtype != MEMTOMEM)
		return NULL;

	pi = &pch->dmac->pif;

	desc = pl330_get_desc(pch);
	if (!desc) {
		struct dma_pl330_dmac *pdmac = pch->dmac;

		dev_err(pch->dmac->pif.dev,
			"%s:%d Unable to fetch desc\n",
			__func__, __LINE__);
		if (!first)
			return NULL;

		spin_lock_irqsave(&pdmac->pool_lock, flags);

		while (!list_empty(&first->node)) {
			desc = list_entry(first->node.next,
					struct dma_pl330_desc, node);
			list_move_tail(&desc->node, &pdmac->desc_pool);
		}

		list_move_tail(&first->node, &pdmac->desc_pool);

		spin_unlock_irqrestore(&pdmac->pool_lock, flags);

		return NULL;
	}

	if (!first)
		first = desc;
	else
		list_add_tail(&desc->node, &first->node);

	if( sg_dma_len(srcsgl) != sg_dma_len(dstsgl) ) return NULL;
	fill_px(&desc->px,  sg_phys(dstsgl), sg_phys(srcsgl), sg_dma_len(srcsgl));
	prepx = &desc->px;
	//pl330_xfer *pxlist = kmalloc(sizeof(*pxlist) * (sg_len-1));
	for(i = 1; i < sg_len; i++)
	{
		px = pl330_get_xfer();
		if(!px)
		{
			dev_err(pch->dmac->pif.dev,
				"%s:%d Unable to fetch xfer\n",
				__func__, __LINE__);
			//all delete
			return NULL;
		}
		fill_px(px,  sg_phys(dstsgl + i), sg_phys(srcsgl + i), sg_dma_len(srcsgl + i));
		prepx->next = px;
		prepx = px;
	}

	desc->rqcfg.src_inc = 1;
	desc->rqcfg.dst_inc = 1;

	/* Select max possible burst size */
	burst = pi->pcfg.data_bus_width / 8;

	while (burst > 1) {
		if (!(len % burst))
			break;
		burst /= 2;
	}

	desc->rqcfg.brst_size = 0;
	while (burst != (1 << desc->rqcfg.brst_size))
		desc->rqcfg.brst_size++;

	desc->rqcfg.brst_len = get_burst_len(desc, len);

	desc->txd.flags = flags;

	return &desc->txd;
}
EXPORT_SYMBOL(sdp_dma330_prep_dma_memcpy_sg);
#endif

static struct dma_async_tx_descriptor *
pl330_prep_slave_sg(struct dma_chan *chan, struct scatterlist *sgl,
		unsigned int sg_len, enum dma_data_direction direction,
		unsigned long flg)
{
	struct dma_pl330_desc *first, *desc = NULL;
	struct dma_pl330_chan *pch = to_pchan(chan);
	struct dma_pl330_peri *peri = chan->private;
	struct scatterlist *sg;
	unsigned long flags;
	int burst_size;
	u32 sg_i;
	dma_addr_t addr;

	if (unlikely(!pch || !sgl || !sg_len))
		return NULL;

	/* Make sure the direction is consistent */
	if ((direction == DMA_TO_DEVICE &&
				peri->rqtype != MEMTODEV) ||
			(direction == DMA_FROM_DEVICE &&
				peri->rqtype != DEVTOMEM)) {
		dev_err(pch->dmac->pif.dev, "%s:%d Invalid Direction\n",
				__func__, __LINE__);
		return NULL;
	}

	addr = peri->fifo_addr;
	burst_size = peri->burst_sz;

	first = NULL;

	for_each_sg(sgl, sg, sg_len, sg_i) {

		desc = pl330_get_desc(pch);
		if (!desc) {
			struct dma_pl330_dmac *pdmac = pch->dmac;

			dev_err(pch->dmac->pif.dev,
				"%s:%d Unable to fetch desc\n",
				__func__, __LINE__);
			if (!first)
				return NULL;

			spin_lock_irqsave(&pdmac->pool_lock, flags);

			while (!list_empty(&first->node)) {
				desc = list_entry(first->node.next,
						struct dma_pl330_desc, node);
				list_move_tail(&desc->node, &pdmac->desc_pool);
			}

			list_move_tail(&first->node, &pdmac->desc_pool);

			spin_unlock_irqrestore(&pdmac->pool_lock, flags);

			return NULL;
		}

		if (!first)
			first = desc;
		else
			list_add_tail(&desc->node, &first->node);

		if (direction == DMA_TO_DEVICE) {
			desc->rqcfg.src_inc = 1;
			desc->rqcfg.dst_inc = 0;
			fill_px(&desc->px,
				addr, sg_dma_address(sg), sg_dma_len(sg));
		} else {
			desc->rqcfg.src_inc = 0;
			desc->rqcfg.dst_inc = 1;
			fill_px(&desc->px,
				sg_dma_address(sg), addr, sg_dma_len(sg));
		}

		desc->rqcfg.brst_size = (u32)burst_size;
		desc->rqcfg.brst_len = 1;
	}

	/* Return the last desc in the chain */
	desc->txd.flags = flg;
	return &desc->txd;
}

static irqreturn_t pl330_irq_handler(int irq, void *data)
{
	if (pl330_update(data))
		return IRQ_HANDLED;
	else
		return IRQ_NONE;
}

static int __devinit
pl330_probe(struct amba_device *adev, const struct amba_id *id)
{
	struct sdp_dma330_platdata *pdat;
	struct dma_pl330_dmac *pdmac;
	struct dma_pl330_chan *pch;
	struct pl330_info *pi;
	struct dma_device *pd;
	struct resource *res;
	int i, ret;
	u32 irq;

	pdat = adev->dev.platform_data;

	if (!pdat || !pdat->nr_valid_peri) {
		dev_err(&adev->dev, "platform data missing\n");
		return -ENODEV;
	}

	/* paltform init! */
	if(pdat->plat_init) {
		ret = pdat->plat_init();
		if(ret) {
			dev_err(&adev->dev, "failed to platform init.(ret:%d)\n", ret);
			return ret;
		}
	}
	else {
		dev_warn(&adev->dev, "platform init is not available!\n");
	}

	/* Allocate a new DMAC and its Channels */
	pdmac = kzalloc(pdat->nr_valid_peri * sizeof(*pch)
				+ sizeof(*pdmac), GFP_KERNEL);
	if (!pdmac) {
		dev_err(&adev->dev, "unable to allocate mem\n");
		return -ENOMEM;
	}

	pdmac->platdata = pdat;/*  */

	pi = &pdmac->pif;
	pi->dev = &adev->dev;
	pi->pl330_data = NULL;
	pi->mcbufsz = pdat->mcbuf_sz;

	res = &adev->res;
	request_mem_region(res->start, resource_size(res), dev_name(&adev->dev));

	pi->base = ioremap(res->start, resource_size(res));
	if (!pi->base) {
		ret = -ENXIO;
		goto probe_err1;
	}

	irq = adev->irq[0];
#if defined(CONFIG_ARCH_SDP1103) || (defined(CONFIG_ARCH_SDP1106) && !defined(CONFIG_ARM_GIC))
#define IRQ_AMSDMA 5
	ret = request_irq_intc_merg(IRQ_AMSDMA, pi, pl330_irq_handler);
#else
	ret = request_irq(irq, pl330_irq_handler, IRQF_SHARED,
			dev_name(&adev->dev), pi);
#endif
	if (ret)
		goto probe_err2;

	sdp_dma330_clk_get(pdmac);

	ret = pl330_add(pi);
	if (ret)
		goto probe_err3;

	INIT_LIST_HEAD(&pdmac->desc_pool);
	spin_lock_init(&pdmac->pool_lock);

	/* Create a descriptor pool of default size */
	if (!add_desc(pdmac, GFP_KERNEL, NR_DEFAULT_DESC))
		dev_warn(&adev->dev, "unable to allocate desc\n");

	pd = &pdmac->ddma;
	INIT_LIST_HEAD(&pd->channels);

	/* Initialize channel parameters */
	for (i = 0; i < pdat->nr_valid_peri; i++) {
		struct dma_pl330_peri *peri = &pdat->peri[i];
		pch = &pdmac->peripherals[i];

		switch (peri->rqtype) {
		case MEMTOMEM:
			dma_cap_set(DMA_MEMCPY, pd->cap_mask);
			dma_cap_set(DMA_SG, pd->cap_mask);
			pd->copy_align = 0;
			break;
		case MEMTODEV:
		case DEVTOMEM:
			dma_cap_set(DMA_SLAVE, pd->cap_mask);
			break;
		default:
			dev_err(&adev->dev, "DEVTODEV Not Supported\n");
			continue;
		}

		INIT_LIST_HEAD(&pch->work_list);
		spin_lock_init(&pch->lock);
		pch->pl330_chid = NULL;
		pch->chan.private = peri;
		pch->chan.device = pd;
		pch->chan.chan_id = i;
		pch->dmac = pdmac;

		/* Add the channel to the DMAC list */
		pd->chancnt++;
		list_add_tail(&pch->chan.device_node, &pd->channels);
	}

	pd->dev = &adev->dev;

	pd->device_alloc_chan_resources = pl330_alloc_chan_resources;
	pd->device_free_chan_resources = pl330_free_chan_resources;
	pd->device_prep_dma_memcpy = pl330_prep_dma_memcpy;
	pd->device_prep_dma_sg = sdp_dma330_prep_dma_sg;
	pd->device_tx_status = pl330_tx_status;
	pd->device_prep_slave_sg = pl330_prep_slave_sg;
	pd->device_control = pl330_control;
	pd->device_issue_pending = pl330_issue_pending;

	ret = dma_async_device_register(pd);
	if (ret) {
		dev_err(&adev->dev, "unable to register DMAC\n");
		goto probe_err4;
	}

	amba_set_drvdata(adev, pdmac);

	dev_info(&adev->dev,
		"Loaded driver for PL330 DMAC-0x%x, driver version: %s\n", adev->periphid, VERSION_SRTING);
	dev_info(&adev->dev,
		"\tDBUFF-%ux%ubytes Num_Chans-%u Num_Peri-%u Num_Events-%u\n",
		pi->pcfg.data_buf_dep,
		pi->pcfg.data_bus_width / 8, pi->pcfg.num_chan,
		pi->pcfg.num_peri, pi->pcfg.num_events);
	dev_info(&adev->dev,
		"\tMCodeBuf Size-%dbyte/chan, %d valid peri.\n", pdat->mcbuf_sz, pdat->nr_valid_peri);
	if(dma_has_cap(DMA_MEMCPY, pd->cap_mask)) {
		dev_info(&adev->dev,
			"\tSupport %dbyte aligned mem to mem transfer.\n", 0x1<<pd->copy_align);
	}

	sdp_dma330_clk_put(pdmac);

	return 0;

probe_err4:
	pl330_del(pi);
probe_err3:
	sdp_dma330_clk_put(pdmac);
	free_irq(irq, pi);
probe_err2:
	iounmap(pi->base);
probe_err1:
	release_mem_region(res->start, resource_size(res));
	kfree(pdmac);

	return ret;
}

static int __devexit pl330_remove(struct amba_device *adev)
{
	struct dma_pl330_dmac *pdmac = amba_get_drvdata(adev);
	struct dma_pl330_chan *pch, *_p;
	struct pl330_info *pi;
	struct resource *res;
	u32 irq;

	if (!pdmac)
		return 0;

	amba_set_drvdata(adev, NULL);

	/* Idle the DMAC */
	list_for_each_entry_safe(pch, _p, &pdmac->ddma.channels,
			chan.device_node) {

		/* Remove the channel */
		list_del(&pch->chan.device_node);

		/* Flush the channel */
		pl330_control(&pch->chan, DMA_TERMINATE_ALL, 0);
		pl330_free_chan_resources(&pch->chan);
	}

	pi = &pdmac->pif;

	pl330_del(pi);

	irq = adev->irq[0];
#ifdef CONFIG_ARCH_SDP1103
	release_irq_intc_merg(IRQ_AMSDMA, pi, pl330_irq_handler);
#else
	free_irq(irq, pi);
#endif

	iounmap(pi->base);

	res = &adev->res;
	release_mem_region(res->start, resource_size(res));

	kfree(pdmac);

	return 0;
}

static struct amba_id pl330_ids[] = {
	{
		.id	= 0x00041330,
		.mask	= 0x000fffff,
	},
	{ 0, 0, 0 },
};

static struct amba_driver pl330_driver = {
	.drv = {
		.owner = THIS_MODULE,
		.name = "sdp-dma330",
	},
	.id_table = pl330_ids,
	.probe = pl330_probe,
	.remove = pl330_remove,
};

static int __init pl330_init(void)
{
	return amba_driver_register(&pl330_driver);
}
module_init(pl330_init);

static void __exit pl330_exit(void)
{
	amba_driver_unregister(&pl330_driver);
	return;
}
module_exit(pl330_exit);

MODULE_AUTHOR("Jaswinder Singh <jassi.brar@samsung.com>");
MODULE_DESCRIPTION("API Driver for PL330 DMAC");
MODULE_LICENSE("GPL");
