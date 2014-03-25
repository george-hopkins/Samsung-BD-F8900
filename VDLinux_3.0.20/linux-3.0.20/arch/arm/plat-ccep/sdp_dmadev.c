/* driver/dma/sdp_dmadev.c
 *
 * Copyright (C) 2011 Samsung Electronics Co. Ltd.
 * Dongseok lee <drain.lee@samsung.com>
 *
 * generic user mode dma driver.
 */


/*
 * 20110617	created by drain.lee
 * 20120315 chack length is 0, edit error msg
 * 20120726 cache operation fix.
 * 20120907 change create dev node per channel
 * 20120907 remove cache control
 * 20121113 bug fix cache operation
 * 20121113 remove duplicated cache operation
 * 20121114 handling dma pinning return NULL.(NULL is no need pinning)
 * 20121115 add SDP_DMADEV_IOC_MEMCPY_PHYS cmd
 * 20121204 fix prevent defect and compile warning
 * 20121210 remove DMA330 dependency
 * 20121220 fix compile warning.
 * 20130201 fix prevent defect(Read from pointer after free).
 */


#include <linux/init.h>
#include <linux/module.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/types.h>
#include <linux/err.h>
#include <linux/list.h>
#include <linux/errno.h>
#include <linux/mutex.h>
#include <asm/uaccess.h>
#include <asm/system.h>
#include <asm/cacheflush.h>
#include <linux/slab.h>
#include <linux/dma-mapping.h>
#include <linux/dmaengine.h>
#include <plat/sdp_dmadev.h>

#define VERSION_SRTING		"20130201"

#ifdef CONFIG_SDP_DMA330
#include <asm/hardware/pl330.h>

extern struct dma_async_tx_descriptor *
sdp_dma330_cache_ctrl(struct dma_chan *chan,
	struct dma_async_tx_descriptor *tx, u32 dst_cache, u32 src_cache);

extern bool sdp_dma330_is_cpu_dma(struct dma_chan * chan);
#endif/*CONFIG_SDP_DMA330*/

#define SDP_DMADEV_MAJOR	MAJOR(MKDEV(251, 0))
#define SDP_DMADEV_MAXNUM	256
#define GET_PAGE_NUM(addr, len)		((PAGE_ALIGN(addr + len) -(addr & PAGE_MASK)) >> PAGE_SHIFT)
#define SDP_DMADEV_NAME		"sdp-dmadev"
#define SDP_DMADEV_MAX_CHAN_NAME	20
#define SDP_DMADEV_MAX_TIMEOUT		5000/* ms */


enum sdp_dmadev_dmactype
{
	SDP_DMADEV_CPUDMA,/* not flush */
	SDP_DMADEV_AMSDMA,
	SDP_DMADEV_ENDDMA,/* keep end!!! */
};

/* create at init */
struct sdp_dmadev {
	struct list_head node;

	/* for device file */
	struct device *dev;
//	dev_t devt;

	/* allocated channel info */
	int dma_device_id;
	int dma_channel_id;

	enum sdp_dmadev_dmactype type;

	/* Char device structure */
	struct cdev cdev;
};



/* create at open */
struct sdp_dmadev_chan {
	/* for dmaengine */
	struct dma_chan *chan;
	struct mutex ioctl_lock;
	struct list_head list_descs;
	struct sdp_dmadev *dmadev;
};

struct sdp_dmadev_data {
	struct list_head list_dmadev_dmac;
};

struct sdp_callback_args {
	struct completion *completion;
};

struct sdp_dmadev_desc {
	struct list_head node;
	struct sdp_dmadev_chan *sdpchan;
	struct sdp_dmadev_ioctl_args args_save;
	struct dma_pinned_list *pinned_list_src;
	struct dma_pinned_list *pinned_list_dst;
	struct sg_table sgtsrc;
	struct sg_table sgtdst;
	dma_cookie_t dma_cookie;
};

static struct sdp_dmadev_data *gsdpdmadev;


/* util functions */
inline static struct sdp_dmadev_chan *
to_sdpdmadevchan(struct dma_chan **chan)
{
	return container_of(chan, struct sdp_dmadev_chan, chan);
}


/* local functions*/
#if 0
static void _page_dump(struct page *page)
{
	int i;
	char *start = (char*)page_address(page);
	pr_info("page dump 0x%08x", start);
	//print_hex_dump(KERN_INFO, "", DUMP_PREFIX_NONE, 16, 1, start, PAGE_SIZE, 0);
}
#endif

//#define SDP_DMADEV_PRINT_SPEED
/* speed test */
#ifdef SDP_DMADEV_PRINT_SPEED
static struct timeval start, finish;
static inline void _start_timing(void)
{
	do_gettimeofday(&start);
}

static inline void _stop_timing(void)
{
	do_gettimeofday(&finish);
}

static unsigned long _calc_speed(unsigned long bytes)
{
	unsigned long us;
	unsigned long speed;

	us = (finish.tv_sec - start.tv_sec) * 1000000 +
	     (finish.tv_usec - start.tv_usec);
	if(us == 0) return 0;
	speed = bytes / us;
	return speed;
}
static inline void _stop_and_print_speed(
	struct sdp_dmadev_chan *sdpchan, const char *msg, unsigned long bytes)
{
	unsigned long us;
	_stop_timing();
	us = (finish.tv_sec - start.tv_sec) * 1000000 +
					     (finish.tv_usec - start.tv_usec);
	dev_info(sdpchan->dmadev->dev, "%s speed is %ldMB/s(%ldus), 0x%lx(%lu)byte\n", msg,
		_calc_speed(bytes), us, bytes, bytes);
}
#else
static inline void _start_timing(void)
{
}
static inline void _stop_timing(void)
{
}
static unsigned long _calc_speed(unsigned long bytes)
{
	return 0;
}
static inline void _stop_and_print_speed
	(struct sdp_dmadev_chan *sdpchan, const char *msg, unsigned long bytes)
{
	_calc_speed(0);
}
#endif



/* for cache operations */
static inline void _wrapper__cpuc_flush_kern_all(void *info)
{
	__cpuc_flush_kern_all();
}
static inline void sdp_hwmem_flush_all(void)
{
	unsigned long flag;

	raw_local_irq_save (flag);

	flush_cache_all ();

#ifdef CONFIG_SMP
	raw_local_irq_restore (flag);

	smp_call_function(_wrapper__cpuc_flush_kern_all, NULL, 1);

	raw_local_irq_save (flag);
#endif

	outer_flush_all();

	raw_local_irq_restore (flag);
}

static inline void
sdp_dmadev_map(const void* vir_addr, const unsigned long phy_addr, const size_t size, enum dma_data_direction dir)
{
	if(dir == DMA_TO_DEVICE) {
		dmac_map_area (vir_addr, size, dir);
		outer_clean_range(phy_addr, (unsigned long)(phy_addr+size));
	} else if(dir == DMA_FROM_DEVICE) {
		dmac_map_area (vir_addr, size, dir);
		outer_inv_range(phy_addr, (unsigned long)(phy_addr+size));
	} else {
		WARN(true, "sdp_dmadev_map: dma_data_direction(0x%08x is not correct!\n", dir);
	}
}

static inline void
sdp_dmadev_unmap(const void* vir_addr, const unsigned long phy_addr, const size_t size, enum dma_data_direction dir)
{
	if(dir == DMA_TO_DEVICE) {
		//nothing
	} else if(dir == DMA_FROM_DEVICE) {
		outer_inv_range(phy_addr, (unsigned long)(phy_addr+size));
		dmac_unmap_area (vir_addr, size, dir);
	} else {
		WARN(true, "sdp_dmadev_unmap: dma_data_direction(0x%08x) is not correct!\n", dir);
	}
}

static void sdp_dmadev_callback(void *data)
{
	struct sdp_callback_args *callback_args = data;
	BUG_ON(!callback_args);
	complete(callback_args->completion);
}

static inline dma_addr_t _sdp_dmadev_vir_to_phys(u32 user_vaddr)
{
	unsigned long flags;

	u32 ttb;
	u32 user_paddr;
	u32 *ppage_1st, *ppage_2nd;

  /*
	 Translation Table Base 0 [31:14] | SBZ [13:5] | RGN[4:3] | IMP[2] | S[1] | C[0]
  */
	local_irq_save(flags);

	asm volatile ("mrc p15, 0, %0, c2, c0, 0 \n" : "=r"(ttb));
	ttb &= 0xFFFFC000;
	ppage_1st = phys_to_virt(ttb);

 /* address of 1st level descriptor */
	ppage_1st += (user_vaddr >> 20);

 /* 1st level descriptor */
 	/* 0x1 is page descriptor */
//	if( *ppage_1st&0x3 != 0x1 ) goto error;

 /* address of 2nd level descriptor */
	ppage_2nd = phys_to_virt(*ppage_1st);
	ppage_2nd = (u32 *)((u32)ppage_2nd & 0xFFFFFC00);
	ppage_2nd += (user_vaddr >> 12) & 0xFF;

 /* 2nd level descriptor */
 	/* is small page descriptor */
// 	if( *ppage_2nd&0x2 != 0x2 ) goto error;
	user_paddr = ((u32)*ppage_2nd & 0xFFFFF000) | (user_vaddr & 0xFFF);

	local_irq_restore(flags);
	return user_paddr;

//error:
//	local_irq_restore(flags);
//	return NULL;
}


static int _sdp_dmadev_make_sgtable(const u32 src, const u32 dst,
	const size_t len, struct sg_table *src_sgtable, struct sg_table *dst_sgtable, u32 flag)
{
	const u32 src_offset = src & ~PAGE_MASK;
	const u32 dst_offset = dst & ~PAGE_MASK;
	const u32 src_nr_pages = GET_PAGE_NUM(src, len);
	const u32 sg_nents = (src_offset == dst_offset ? src_nr_pages : (src_nr_pages*2)+1 );

	int i, ret;
	u32 curpagebyte = 0, copy = 0, totalcopybytes = 0;
	struct scatterlist *cursrc, *curdst;
	dma_addr_t src_next = 0;
	dma_addr_t dst_next = 0;
	dma_addr_t srcpa_cur = 0;
	dma_addr_t dstpa_cur = 0;


#ifndef ARCH_HAS_SG_CHAIN
	if(sg_nents > SG_MAX_SINGLE_ALLOC) {
		pr_err("%s: sg entries(%d) over Max entries(%d)!!!\n", __FUNCTION__, sg_nents, SG_MAX_SINGLE_ALLOC);
		return -ENOMEM;
	}
#endif

	ret = sg_alloc_table(src_sgtable, sg_nents, GFP_KERNEL);
	if(unlikely(ret < 0))
	{
		return ret;
	}
	ret = sg_alloc_table(dst_sgtable, sg_nents, GFP_KERNEL);
	if(unlikely(ret < 0))
	{
		sg_free_table(src_sgtable);
		return ret;
	}

	cursrc = src_sgtable->sgl;
	curdst = dst_sgtable->sgl;

	/* step1 : src first page copy */
	curpagebyte = min_t(u32, PAGE_SIZE-src_offset, len);
	copy = min_t(u32, curpagebyte, PAGE_SIZE-dst_offset);

	sg_dma_address(cursrc) = _sdp_dmadev_vir_to_phys(src);
	sg_dma_address(curdst) = _sdp_dmadev_vir_to_phys(dst);
	sg_dma_len(cursrc) = copy;
	sg_dma_len(curdst) = copy;

	totalcopybytes += copy;
//	dev_info(sdpchan->dmac->dev, "First Page SrcPA 0x%x DstPA 0x%x\n", sg_dma_address(cursrc), sg_dma_address(curdst));

	if(copy < curpagebyte)
	{
		cursrc = sg_next(cursrc);
		curdst = sg_next(curdst);

		sg_dma_address(cursrc) = _sdp_dmadev_vir_to_phys(src + totalcopybytes);
		sg_dma_address(curdst) = _sdp_dmadev_vir_to_phys(dst + totalcopybytes);
		sg_dma_len(cursrc) = curpagebyte-copy;
		sg_dma_len(curdst) = curpagebyte-copy;

		totalcopybytes += curpagebyte-copy;
	}


	/* step2 : 2 ~ N-1st page copy */
	for(i = 1; i < (int)(src_nr_pages-1); i++)
	{
		src_next = sg_dma_address(cursrc) + sg_dma_len(cursrc);
		dst_next = sg_dma_address(curdst) + sg_dma_len(curdst);
		srcpa_cur = _sdp_dmadev_vir_to_phys(src + totalcopybytes);
		if(src_offset == dst_offset)
			dstpa_cur = _sdp_dmadev_vir_to_phys(dst + totalcopybytes);
		else
			dstpa_cur = dst_next;

		curpagebyte = PAGE_SIZE;
		copy = min_t(u32, curpagebyte, PAGE_SIZE-((totalcopybytes+dst_offset)&~PAGE_MASK));

		if(!(flag & SDP_DMADEV_DISABLE_SG_OPTIMIZE) &&
			src_next == srcpa_cur &&
			dst_next == dstpa_cur)
		{
			sg_dma_len(cursrc) += copy;
			sg_dma_len(curdst) += copy;
		}
		else
		{
			cursrc = sg_next(cursrc);
			curdst = sg_next(curdst);

			sg_dma_address(cursrc) = srcpa_cur;
			sg_dma_address(curdst) = dstpa_cur;
			sg_dma_len(cursrc) = copy;
			sg_dma_len(curdst) = copy;
		}

		totalcopybytes += copy;

		if(copy < curpagebyte)
		{
			src_next = sg_dma_address(cursrc) + sg_dma_len(cursrc);
			dst_next = sg_dma_address(curdst) + sg_dma_len(curdst);
			srcpa_cur = src_next;
			dstpa_cur = _sdp_dmadev_vir_to_phys(dst + totalcopybytes);

			if(!(flag&SDP_DMADEV_DISABLE_SG_OPTIMIZE) &&
				src_next == srcpa_cur &&
				dst_next == dstpa_cur)
			{
				sg_dma_len(cursrc) += curpagebyte-copy;
				sg_dma_len(curdst) += curpagebyte-copy;
			}
			else
			{
				cursrc = sg_next(cursrc);
				curdst = sg_next(curdst);

				sg_dma_address(cursrc) = srcpa_cur;
				sg_dma_address(curdst) = dstpa_cur;
				sg_dma_len(cursrc) = curpagebyte-copy;
				sg_dma_len(curdst) = curpagebyte-copy;
			}

			totalcopybytes += curpagebyte-copy;
		}

	}


	/* step3 : Nst page copy */
	if(src_nr_pages >= 2)
	{
		curpagebyte = len-totalcopybytes;
		copy = min_t(u32, curpagebyte, PAGE_SIZE-((totalcopybytes+dst_offset)&~PAGE_MASK));
		cursrc = sg_next(cursrc);
		curdst = sg_next(curdst);

		sg_dma_address(cursrc) = _sdp_dmadev_vir_to_phys(src + totalcopybytes);
		sg_dma_address(curdst) = _sdp_dmadev_vir_to_phys(dst + totalcopybytes);
		sg_dma_len(cursrc) = copy;
		sg_dma_len(curdst) = copy;

		totalcopybytes += copy;

		if(copy < curpagebyte)
		{
			cursrc = sg_next(cursrc);
			curdst = sg_next(curdst);

			sg_dma_address(cursrc) = _sdp_dmadev_vir_to_phys(src + totalcopybytes);
			sg_dma_address(curdst) = _sdp_dmadev_vir_to_phys(dst + totalcopybytes);
			sg_dma_len(cursrc) = (curpagebyte-copy);
			sg_dma_len(curdst) = (curpagebyte-copy);

			totalcopybytes += curpagebyte-copy;
		}
	}

	sg_mark_end(cursrc);
	sg_mark_end(curdst);

	/* count sglist */
	for(i = 0, cursrc = src_sgtable->sgl; cursrc; i++, cursrc = sg_next(cursrc));
	return i;
}

static struct sdp_dmadev_desc *
sdp_dmadev_alloc_desc(
	struct sdp_dmadev_chan *sdpchan, struct sdp_dmadev_ioctl_args *args)
{
	struct sdp_dmadev_desc *dma_desc = NULL;
	struct iovec iov;

	BUG_ON(!sdpchan || !args);

	dma_desc = kzalloc(sizeof(struct sdp_dmadev_desc), GFP_KERNEL);
	if(!dma_desc) {
		return NULL;
	}

	dma_desc->sdpchan = sdpchan;
	dma_desc->args_save = *args;

	/* try memory pinning for heep area */
	iov.iov_base = (void *)args->src_addr;
	iov.iov_len = args->len;
	dma_desc->pinned_list_src = dma_pin_iovec_pages(&iov, args->len);

	iov.iov_base = (void *)args->dst_addr;
	iov.iov_len = args->len;
	dma_desc->pinned_list_dst = dma_pin_iovec_pages(&iov, args->len);


	/* add new desc into lists */
	list_add_tail(&dma_desc->node, &sdpchan->list_descs);

	if(0) {//for debug
		int i;
		struct dma_pinned_list *pinned_list = dma_desc->pinned_list_src;
		dev_dbg(sdpchan->dmadev->dev, "pinned_list dump\n");
		if(pinned_list) {
			for(i = 0; i < pinned_list->nr_iovecs; i++) {
				dev_dbg(sdpchan->dmadev->dev, "iovec_src[%3d]-> va_base 0x%p, nr_pages %3d\n", i,
					pinned_list->page_list[i].base_address, pinned_list->page_list[i].nr_pages);
			}
		} else {
			dev_dbg(sdpchan->dmadev->dev, "iovec_src address is not heep area\n");
		}

		if(pinned_list) {
			pinned_list = dma_desc->pinned_list_dst;
			for(i = 0; i < pinned_list->nr_iovecs; i++) {
				dev_dbg(sdpchan->dmadev->dev, "iovec_dst[%3d]-> va_base 0x%p, nr_pages %3d\n", i,
					pinned_list->page_list[i].base_address, pinned_list->page_list[i].nr_pages);
			}
		} else {
			dev_dbg(sdpchan->dmadev->dev, "iovec_src address is not heep area\n");
		}
	}

	return dma_desc;
}

static int
sdp_dmadev_free_desc(
	struct sdp_dmadev_chan *sdpchan, struct sdp_dmadev_desc *dma_desc)
{
	struct sdp_dmadev_ioctl_args *args;

	BUG_ON(!sdpchan || !dma_desc);

	list_del(&dma_desc->node);

	args = &dma_desc->args_save;

	if(!(args->testflag & SDP_DMADEV_SKIP_DST_INVALIDATE)) {
		u32 curuaddr;
		struct scatterlist *curpos;
		curuaddr = args->dst_addr;

		_start_timing();
		for(curpos = dma_desc->sgtdst.sgl; curpos; curpos = sg_next(curpos))
		{
			dev_dbg(sdpchan->dmadev->dev, "VA:%#x PA:%#x len %#x\n",
				curuaddr, sg_dma_address(curpos),	sg_dma_len(curpos));
			sdp_dmadev_unmap((const void *)curuaddr,
				sg_dma_address(curpos), sg_dma_len(curpos), DMA_FROM_DEVICE);
			curuaddr += sg_dma_len(curpos);
		}
		_stop_and_print_speed(sdpchan, "DMA Unmap from Dev", args->len);
	}

	sg_free_table(&dma_desc->sgtsrc);
	sg_free_table(&dma_desc->sgtdst);

	if(dma_desc->pinned_list_src)
		dma_unpin_iovec_pages(dma_desc->pinned_list_src);
	if(dma_desc->pinned_list_dst)
		dma_unpin_iovec_pages(dma_desc->pinned_list_dst);

	kzfree(dma_desc);
	return 0;
}

static dma_cookie_t
_sdp_dmadev_request_memcpy(
	struct sdp_dmadev_chan *sdpchan, struct sdp_dmadev_desc *dma_desc,
	dma_async_tx_callback callback, struct sdp_callback_args *callback_param)
{
	struct sdp_dmadev_ioctl_args *args = &dma_desc->args_save;
	const unsigned long src_uaddr = args->src_addr;
	const unsigned long dst_uaddr = args->dst_addr;
	struct dma_chan *chan = sdpchan->chan;
	size_t len = args->len;

	int ret = 0;
	const u32 src_nr_pages = GET_PAGE_NUM(src_uaddr, len);
	const u32 dst_nr_pages = GET_PAGE_NUM(dst_uaddr, len);

	dma_cookie_t dma_cookie = 0;
	struct dma_async_tx_descriptor *tx = NULL;

	struct sg_table *sgtsrc = &dma_desc->sgtsrc;
	struct sg_table *sgtdst = &dma_desc->sgtdst;
	u32 totalsg;

	dev_dbg(sdpchan->dmadev->dev, "src_nr_pages(%u)", src_nr_pages);
	dev_dbg(sdpchan->dmadev->dev, "dst_nr_pages(%u)", dst_nr_pages);

	_start_timing();

	if(!(args->testflag & SDP_DMADEV_SKIP_VIRT_TO_PHYS)) {
		ret = _sdp_dmadev_make_sgtable(src_uaddr, dst_uaddr, len,
			sgtsrc, sgtdst, args->testflag);

		if(ret < 0) {
			totalsg = 0;
		} else {
			totalsg = (u32)ret;
		}

	} else {
		ret = sg_alloc_table(sgtsrc, 1, GFP_KERNEL);
		if(unlikely(ret < 0))
		{
			totalsg = 0;
		}

		ret = sg_alloc_table(sgtdst, 1, GFP_KERNEL);
		if(unlikely(ret < 0))
		{
			sg_free_table(sgtsrc);
			totalsg = 0;
		}

		sg_dma_address(sgtsrc->sgl) = src_uaddr;
		sg_dma_address(sgtdst->sgl) = dst_uaddr;
		sg_dma_len(sgtsrc->sgl) = len;
		sg_dma_len(sgtdst->sgl) = len;

		sg_mark_end(sgtsrc->sgl);
		sg_mark_end(sgtdst->sgl);

		/* count sglist */
		totalsg = 1;
	}

	if(unlikely(totalsg == 0))
	{
		dev_err(sdpchan->dmadev->dev, "can not make sgtable.(%d)", totalsg);
		return ret;
	}

	_stop_and_print_speed(sdpchan, "setup scatter list", len);




	_start_timing();

	tx = chan->device->device_prep_dma_sg(chan, sgtdst->sgl, totalsg, sgtsrc->sgl, totalsg,
		DMA_CTRL_ACK|DMA_PREP_INTERRUPT|DMA_COMPL_SKIP_SRC_UNMAP|DMA_COMPL_SKIP_DEST_UNMAP);
	if(!tx)
	{
		dev_err(sdpchan->dmadev->dev, "prep error with src_addr=0x%lx "
					"dst_addr=0x%lx len=0x%x\n",
					src_uaddr, dst_uaddr, len);
		ret = -EIO;
		goto free;
	}

#ifdef CONFIG_SDP_DMA330
	tx = sdp_dma330_cache_ctrl(chan, tx, DCCTRL7, SCCTRL7);
#endif

	_stop_and_print_speed(sdpchan, "device_prep_dma_sg", len);

	dev_dbg(sdpchan->dmadev->dev, "total sglist %d\n",	totalsg);



	if(sdpchan->dmadev->type == SDP_DMADEV_AMSDMA)
	{
		if(!(args->testflag & SDP_DMADEV_SKIP_SRC_CLEAN))
		{
			u32 curuaddr;
			struct scatterlist *curpos;
			curuaddr = src_uaddr;

			_start_timing();
			for(curpos = sgtsrc->sgl; curpos; curpos = sg_next(curpos))
			{
				dev_dbg(sdpchan->dmadev->dev, "VA:%#x PA:%#x len %#x\n", curuaddr, sg_dma_address(curpos),	sg_dma_len(curpos));
				sdp_dmadev_map((const void *)curuaddr, sg_dma_address(curpos),
					sg_dma_len(curpos), DMA_TO_DEVICE);
				curuaddr += sg_dma_len(curpos);
			}
			_stop_and_print_speed(sdpchan, "DMA Map to Dev", len);

		}


		if(!(args->testflag & SDP_DMADEV_SKIP_DST_INVALIDATE)) {
			u32 curuaddr;
			struct scatterlist *curpos;
			curuaddr = dst_uaddr;

			_start_timing();
			for(curpos = sgtdst->sgl; curpos; curpos = sg_next(curpos))
			{
				dev_dbg(sdpchan->dmadev->dev, "VA:%#x PA:%#x len %#x\n", curuaddr, sg_dma_address(curpos),	sg_dma_len(curpos));
				sdp_dmadev_map((const void *)curuaddr, sg_dma_address(curpos),
					sg_dma_len(curpos), DMA_FROM_DEVICE);
				curuaddr += sg_dma_len(curpos);
			}
			_stop_and_print_speed(sdpchan, "DMA Map from Dev", len);
		}
	}

	_start_timing();

	tx->callback = callback;
	tx->callback_param = callback_param;

	dma_cookie = tx->tx_submit(tx);
	dma_async_issue_pending(chan);

	dma_desc->dma_cookie = dma_cookie;

	_stop_and_print_speed(sdpchan, "dma_async_issue_pending", len);


free:


	return dma_cookie;
}


#ifdef CONFIG_SDP_DMA330_2DCOPY
extern struct dma_async_tx_descriptor *
sdp_dma330_prep_dma_2dcopy(struct dma_chan *chan,
		dma_addr_t dst, int dst_linespan_px,
		dma_addr_t src, int src_linespan_px,
		int width, int height, int bpp, unsigned long flags);

static dma_cookie_t
_sdp_dmadev_2dcpy(
	struct sdp_dmadev_chan *sdpchan, struct sdp_dmadev_ioctl_2dcpy *args,
	dma_async_tx_callback callback, void *callback_param)
{
	struct dma_chan *chan = sdpchan->chan;

	dma_cookie_t dma_cookie = 0;
	struct dma_async_tx_descriptor *tx = NULL;

	const int pixelbyte = args->bpp >> 3;
	const size_t len = args->width * args->height * pixelbyte;

	const unsigned long src_uaddr = args->src_window.start_addr +
		((args->srcx +(args->srcy*args->src_window.width)) * pixelbyte);
	const unsigned long dst_uaddr = args->dst_window.start_addr +
		((args->dstx +(args->dsty*args->dst_window.width)) * pixelbyte);

	dma_addr_t src = _sdp_dmadev_vir_to_phys(src_uaddr);
	dma_addr_t dst = _sdp_dmadev_vir_to_phys(dst_uaddr);
	int src_linespan = args->src_window.width - args->width;
	int dst_linespan = args->dst_window.width - args->width;

	if( (args->bpp&0x7) )
	{
		dev_err(sdpchan->dmadev->dev, "not support %dbpp\n", args->bpp);
		return -EINVAL;
	}


	_start_timing();

	tx = sdp_dma330_prep_dma_2dcopy(chan, dst, dst_linespan, src, src_linespan,
		args->width, args->height, args->bpp,
		DMA_CTRL_ACK|DMA_PREP_INTERRUPT|DMA_COMPL_SKIP_SRC_UNMAP|DMA_COMPL_SKIP_DEST_UNMAP);
	if(!tx)
	{
		dev_err(sdpchan->dmadev->dev, "prep error with src_addr=0x%lx "
					"dst_addr=0x%lx len=0x%x\n",
					src_uaddr, dst_uaddr, len);
		return -EIO;
	}
	tx = sdp_dma330_cache_ctrl(chan, tx, args->dst_cache, args->src_cache);

	_stop_and_print_speed(sdpchan, "sdp_dmadev_2dcpy", len);


	_start_timing();

	tx->callback = callback;
	tx->callback_param = callback_param;

	dma_cookie = tx->tx_submit(tx);
	dma_async_issue_pending(chan);

	_stop_and_print_speed(sdpchan, "dma_async_issue_pending", len);


	return dma_cookie;
}
#endif/* CONFIG_SDP_DMA330_2DCOPY */


static long
sdp_dmadev_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	struct sdp_dmadev_chan *sdpchan = NULL;
	struct dma_chan *chan = NULL;
	struct sdp_dmadev_desc *dma_desc = NULL;

	int err = 0, ret;
	dma_cookie_t dma_cookie = 0;

	if(_IOC_TYPE(cmd) != SDP_DMADEV_IOC_MAGIC)
		return -ENOTTY;

	/* Check access direction once here; don't repeat below.
	 * IOC_DIR is from the user perspective, while access_ok is
	 * from the kernel perspective; so they look reversed.
	 */
	if(_IOC_SIZE(cmd) > 0)
	{
		if (_IOC_DIR(cmd) & _IOC_READ)
			err = !access_ok(VERIFY_WRITE,	(void __user *)arg, _IOC_SIZE(cmd));
		if (err == 0 && _IOC_DIR(cmd) & _IOC_WRITE)
			err = !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
		if (err)
			return -EFAULT;
	}
	/* guard against device removal before, or while,
	 * we issue this ioctl.
	 */
	sdpchan = filp->private_data;/*this is driver data in device */
	chan = sdpchan->chan;

	if (unlikely(chan == NULL))
	{
		return -ESHUTDOWN;
	}

	switch (cmd) {

	case SDP_DMADEV_IOC_MEMCPY:
		if(!dma_has_cap(DMA_SG, chan->device->cap_mask)) {
			/* not support SG */
			err = ENOTSUPP;
			break;
		}

	case SDP_DMADEV_IOC_MEMCPY_PHYS:
	{
		struct sdp_dmadev_ioctl_args argcpy;

		if(_IOC_SIZE(cmd) != sizeof(argcpy)) {
			err = -EINVAL;
			break;
		}

		ret = (int)copy_from_user(&argcpy, (const void *)arg, sizeof(argcpy));
		if(ret != 0) {
			err = -EINVAL;
			break;
		}

		dev_dbg(sdpchan->dmadev->dev,
			"SDP_DMADEV_IOC_MEMCPY srcaddr= 0x%lx, dstaddr= 0x%lx, len=0x%x%d\n",
			argcpy.src_addr, argcpy.dst_addr, argcpy.len, argcpy.len);

		if(!argcpy.len) {
			dev_err(sdpchan->dmadev->dev, "copy length is 0!!!\n");
			err = -EINVAL;
			break;
		}

		err = is_dma_copy_aligned(chan->device, argcpy.src_addr,
			argcpy.dst_addr, argcpy.len);
		if(!err)
		{
			dev_err(sdpchan->dmadev->dev, "address is not aligned.\n");
			err = -EINVAL;
			break;
		}


		dma_desc = sdp_dmadev_alloc_desc(sdpchan, &argcpy);
		if(!dma_desc) {
			dev_err(sdpchan->dmadev->dev, "can not allocated desc.\n");
			err = -ENOMEM;
			break;
		}

		if(cmd == SDP_DMADEV_IOC_MEMCPY_PHYS) {
			dma_desc->args_save.testflag |= SDP_DMADEV_SKIP_SRC_CLEAN;
			dma_desc->args_save.testflag |= SDP_DMADEV_SKIP_DST_INVALIDATE;
			dma_desc->args_save.testflag |= SDP_DMADEV_SKIP_VIRT_TO_PHYS;
		}

		//if async mode?
		if(filp->f_flags & FASYNC/*O_ASYNC*/)
		{

			/* no need completion */
			dma_cookie = _sdp_dmadev_request_memcpy(sdpchan,
				dma_desc, NULL, NULL);

			/* ASync mode, return dma_cookie */
			err = dma_cookie;
		}
		else
		{
			struct completion cmp;
			struct sdp_callback_args callback_args = {0};
			unsigned long tmo = msecs_to_jiffies(SDP_DMADEV_MAX_TIMEOUT);
			unsigned long remaining_time;

			init_completion(&cmp);
			callback_args.completion = &cmp;

			dma_cookie = _sdp_dmadev_request_memcpy(sdpchan,
				dma_desc, sdp_dmadev_callback, &callback_args);

			remaining_time = wait_for_completion_timeout(&cmp, tmo);
			if (remaining_time == 0)
			{
				dev_err(sdpchan->dmadev->dev, "%s->DMAEngine Timeout(over %dmsec)!!!\n",
					dma_chan_name(chan), jiffies_to_msecs(tmo));
				chan->device->device_control(sdpchan->chan, DMA_TERMINATE_ALL, 0);
				err = -ETIMEDOUT;
			}
			else if((err = dma_async_is_tx_complete(chan, dma_cookie, NULL, NULL)) != DMA_SUCCESS)
			{
				dev_err(sdpchan->dmadev->dev, "%s->DMAEngine retun not success!!! (%s)\n",
					dma_chan_name(chan),
					err==DMA_ERROR?"DMA_ERROR":(err==DMA_PAUSED?"DMA_PAUSED":"DMA_IN_PROGRESS"));
				chan->device->device_control(sdpchan->chan, DMA_TERMINATE_ALL, 0);
				err = -EIO;
			}

			sdp_dmadev_free_desc(sdpchan, dma_desc);
		}

		break;//end of case
	}

#ifdef CONFIG_SDP_DMA330_2DCOPY
	case SDP_DMADEV_IOC_2DCPY:
	{
		struct sdp_dmadev_ioctl_2dcpy arg2dcpy;

		err = copy_from_user(&arg2dcpy, (const void *)arg, _IOC_SIZE(cmd));

		init_completion(&cmp);
		callback_args.completion = &cmp;

		dma_cookie = _sdp_dmadev_2dcpy(sdpchan, &arg2dcpy, sdp_dmadev_callback, &callback_args);
		tmo = wait_for_completion_timeout(&cmp, tmo);
		if (tmo == 0)
		{
			dev_err(sdpchan->dmadev->dev, "%s->DMAEngine Timeout(%dmsec)!!!\n",
					dma_chan_name(chan), jiffies_to_msecs(tmo));
			err = -ETIMEDOUT;
			sdpchan->chan->device->device_control(sdpchan->chan, DMA_TERMINATE_ALL, 0);
		}
		else if(dma_async_is_tx_complete(chan, dma_cookie, NULL, NULL) != DMA_SUCCESS)
		{
			dev_err(sdpchan->dmadev->dev, "%s->DMAEngine retun not success!!! (%s)\n",
				dma_chan_name(chan),
				err==DMA_ERROR?"DMA_ERROR":(err==DMA_PAUSED?"DMA_PAUSED":"DMA_IN_PROGRESS"));
			err = -EIO;
			sdpchan->chan->device->device_control(sdpchan->chan, DMA_TERMINATE_ALL, 0);
		}

		break;//end of case
	}
#endif/* CONFIG_SDP_DMA330_2DCOPY */
	case SDP_DMADEV_IOC_IS_COMPLETE:
	{
		enum dma_status status;
		struct sdp_dmadev_is_cmp_args argcmp;

		ret = (int)copy_from_user(&argcmp, (const void *)arg, _IOC_SIZE(cmd));
		if(ret != 0) {
			err = -EINVAL;
			break;
		}

		if(dma_submit_error(argcmp.dma_handle)) {
			err = -EINVAL;
			break;
		}

		status = dma_async_is_tx_complete(chan, argcmp.dma_handle, NULL, NULL);
		if( status == DMA_IN_PROGRESS || status == DMA_PAUSED)
		{
			return 1;/* return busy */
		}

		if( status == DMA_SUCCESS )
		{
			err = 0;
		}
		else
		{
			dev_err(sdpchan->dmadev->dev, "%s->DMAEngine retun not success!!! (%s)\n",
				dma_chan_name(chan),
				err==DMA_ERROR?"DMA_ERROR":(err==DMA_PAUSED?"DMA_PAUSED":"DMA_IN_PROGRESS"));
			chan->device->device_control(sdpchan->chan, DMA_TERMINATE_ALL, 0);
			err = -EIO;
		}

		/* find cookie and free desc */
		{
			struct sdp_dmadev_desc *cur_dma_desc = NULL, *temp = NULL;

			list_for_each_entry_safe(cur_dma_desc, temp, &sdpchan->list_descs, node) {
				if(cur_dma_desc->dma_cookie == argcmp.dma_handle) {
					sdp_dmadev_free_desc(sdpchan, cur_dma_desc);
				}
			}
		}


		break;//end of case
	}
	default:
		err = -ENOTSUPP;
		break;
	}


	return err;

}

static bool filter_chan_by_name(struct dma_chan *chan, void *param)
{
	const char *find_chan_name = param;
	const size_t chan_strlen = strnlen(find_chan_name, SDP_DMADEV_MAX_CHAN_NAME);
	if( strncmp(dma_chan_name(chan), find_chan_name, chan_strlen) == 0)
		return true;
	else
		return false;
}

static bool filter_all(struct dma_chan *chan, void *param)
{
//	dev_info(&chan->dev->device, "chanid %d, deviceid %d\n", chan->chan_id, chan->device->dev_id);
	return true;
}

static int sdp_dmadev_open(struct inode *inode, struct file *filp)
{
	struct sdp_dmadev_chan *sdpchan;
	struct dma_chan *chan;
	dma_cap_mask_t dma_cap;
	struct sdp_dmadev *dmadev = NULL;
	char channame[SDP_DMADEV_MAX_CHAN_NAME] = { 0, };

	dmadev = container_of(inode->i_cdev, struct sdp_dmadev, cdev);

	if (MINOR(dmadev->dev->devt) != iminor(inode)) {
		pr_err(SDP_DMADEV_NAME ": nothing for minor %d\n", iminor(inode));
		return -ENXIO;
	}

	sdpchan = kzalloc(sizeof(*sdpchan), GFP_KERNEL);
	if( IS_ERR_OR_NULL(sdpchan) )
	{
		pr_debug(SDP_DMADEV_NAME ": do not allocate memory(%d).\n", ENOMEM);
		return -ENOMEM;
	}


	dma_cap_zero(dma_cap);
	dma_cap_set(DMA_SG, dma_cap);
	snprintf(channame, SDP_DMADEV_MAX_CHAN_NAME, "dma%dchan%d", dmadev->dma_device_id, dmadev->dma_channel_id);
	chan = dma_request_channel(dma_cap, filter_chan_by_name, channame);
	if( IS_ERR_OR_NULL(chan) )
	{
		pr_debug(SDP_DMADEV_NAME ": do not allocated %s.(%ld).\n", channame, PTR_ERR(chan));
		kfree(sdpchan);
		return -EBUSY;
	}
	pr_debug(SDP_DMADEV_NAME ": use %s.\n", dma_chan_name(chan));

	sdpchan->dmadev = dmadev;
	sdpchan->chan = chan;
	mutex_init(&sdpchan->ioctl_lock);
	filp->private_data = sdpchan;
	INIT_LIST_HEAD(&sdpchan->list_descs);
	return nonseekable_open(inode, filp);

}
static int sdp_dmadev_release(struct inode *inode, struct file *filp)
{
	struct sdp_dmadev_chan *sdpchan;
	struct sdp_dmadev_desc *dma_desc = NULL, *temp = NULL;

	sdpchan = (struct sdp_dmadev_chan*)filp->private_data;
	filp->private_data = NULL;

	if(sdpchan)
	{
		list_for_each_entry_safe(dma_desc, temp, &sdpchan->list_descs, node) {
			if(dma_async_is_tx_complete(sdpchan->chan, dma_desc->dma_cookie, NULL, NULL) != DMA_SUCCESS) {
				dev_err(sdpchan->dmadev->dev, "dma cookie %d is not successed!\n",
					dma_desc->dma_cookie);
			}
			sdp_dmadev_free_desc(sdpchan, dma_desc);
		}

		if(sdpchan->chan) {
			if( sdpchan->chan->device->device_control(sdpchan->chan, DMA_TERMINATE_ALL, 0) < 0 ) {
				dev_err(sdpchan->dmadev->dev, "DMA Terminate all return failed!\n");
			}
			dma_release_channel(sdpchan->chan);
		}

		mutex_destroy(&sdpchan->ioctl_lock);
		kzfree(sdpchan);
	}

	return 0;
}

static const struct file_operations sdp_dmadev_fops = {
	.owner =	THIS_MODULE,
	.unlocked_ioctl = sdp_dmadev_ioctl,
	.open =		sdp_dmadev_open,
	.release =	sdp_dmadev_release,
};


static struct class *sdp_dmadev_class;

static void sdp_dmadev_setup_cdev(struct sdp_dmadev *dmadev, int index)
{
        int err;
        dev_t devno = MKDEV(SDP_DMADEV_MAJOR, (u32)index);

        cdev_init(&dmadev->cdev, &sdp_dmadev_fops);
        dmadev->cdev.owner = THIS_MODULE;


        dmadev->cdev.ops = &sdp_dmadev_fops;
        err = cdev_add (&dmadev->cdev, devno, 1);
        /* Fail gracefully if need be */
        if (err)
                printk(KERN_NOTICE "Error %d adding " SDP_DMADEV_NAME "%d", err, index);
}


/* recurcive!! */
static int
sdp_dmadev_prove_channel(struct sdp_dmadev_data *sdpdmadev, dma_cap_mask_t dma_cap, int miner)
{
	int ret = 0;
	struct sdp_dmadev *dmadev = NULL;
	struct dma_chan *chan = dma_request_channel(dma_cap, filter_all, NULL);
	struct device *dev = NULL;

	if (chan) {
		/* create driver data */
		dmadev = kzalloc(sizeof(*dmadev), GFP_KERNEL);
		if( IS_ERR_OR_NULL(dmadev) )
		{
			ret = -ENOMEM;
			goto error_free;
		}
		INIT_LIST_HEAD(&dmadev->node);
		list_add_tail(&dmadev->node, &sdpdmadev->list_dmadev_dmac);

		sdp_dmadev_setup_cdev(dmadev, miner);

		dmadev->type = SDP_DMADEV_AMSDMA;
		dev = device_create(sdp_dmadev_class, NULL, dmadev->cdev.dev, NULL/*drv data*/,
		"sdp_dmadev%d.%d", chan->device->dev_id, chan->chan_id);



		if( IS_ERR_OR_NULL(dev))
		{
			pr_err(SDP_DMADEV_NAME ": device_create error!!(%ld)\n", PTR_ERR(dev));
			ret = (int)PTR_ERR(dev);
			goto error_free;
		}

		dmadev->dev = dev;
		dmadev->dma_device_id = chan->device->dev_id;
		dmadev->dma_channel_id = chan->chan_id;

		pr_info(SDP_DMADEV_NAME ": \tcreate dev node /dev/%s\n", dev_name(dev));

		ret = sdp_dmadev_prove_channel(sdpdmadev, dma_cap, miner + 1);
		if(ret < 0)
			goto error_free;

		dma_release_channel(chan);

	}

	return 0;

error_free:

	dma_release_channel(chan);

	if( dmadev ) {
		if( dmadev->dev ) {
			pr_info(SDP_DMADEV_NAME": \tdestroy dev node /dev/%s\n", dev_name(dev));
			device_destroy(sdp_dmadev_class, dmadev->dev->devt);
		}

		kfree(dmadev);
	}

	return ret;
}

static int __init sdp_dmadev_init(void)
{
	int ret = 0;
	dma_cap_mask_t dma_cap;

	pr_info(SDP_DMADEV_NAME ": Version %s", VERSION_SRTING);

	pr_info(SDP_DMADEV_NAME ": Probing DMAC Channels...\n");

	ret = register_chrdev_region(MKDEV(SDP_DMADEV_MAJOR, 0), SDP_DMADEV_MAXNUM, SDP_DMADEV_NAME);
	if(ret < 0)
	{
		pr_err(SDP_DMADEV_NAME ": register_chrdev_region error!!(%d)\n", ret);
		return ret;
	}

	sdp_dmadev_class = class_create(THIS_MODULE, SDP_DMADEV_NAME);
	if (IS_ERR_OR_NULL(sdp_dmadev_class)) {
		pr_err(SDP_DMADEV_NAME ": class_create error!!(%ld)\n", PTR_ERR(sdp_dmadev_class));
		ret = (int)PTR_ERR(sdp_dmadev_class);
		goto err0;
	}

	/*alloc sdp_dmadev_data */
	gsdpdmadev = kzalloc(sizeof(*gsdpdmadev), GFP_KERNEL);
	if( IS_ERR_OR_NULL(gsdpdmadev) )
	{
		gsdpdmadev = NULL;
		ret = -ENOMEM;
		goto err1;
	}

	INIT_LIST_HEAD(&gsdpdmadev->list_dmadev_dmac);

	dma_cap_zero(dma_cap);
	dma_cap_set(DMA_MEMCPY, dma_cap);

	ret = sdp_dmadev_prove_channel(gsdpdmadev, dma_cap, 0);

	if(ret < 0)
		goto err2;

	pr_info(SDP_DMADEV_NAME ": done.\n");
	return 0;

err2:
	kfree(gsdpdmadev);

err1:
	class_destroy(sdp_dmadev_class);

err0:/* chrdev unregister */
	unregister_chrdev_region(MKDEV(SDP_DMADEV_MAJOR, 0), SDP_DMADEV_MAXNUM);
	return ret;

}
module_init(sdp_dmadev_init);


static void __exit sdp_dmadev_exit(void)
{
	struct sdp_dmadev *dmadev, *t;


	list_for_each_entry_safe(dmadev, t, &gsdpdmadev->list_dmadev_dmac, node)
	{
		if( dmadev ) {
			if( dmadev->dev )
			{
				device_destroy(sdp_dmadev_class, dmadev->dev->devt);
			}

			cdev_del(&dmadev->cdev);
			kfree(dmadev);
		}
	}
	kfree(gsdpdmadev);

	class_destroy(sdp_dmadev_class);

	unregister_chrdev_region(MKDEV(SDP_DMADEV_MAJOR, 0), SDP_DMADEV_MAXNUM);
}
module_exit(sdp_dmadev_exit);

MODULE_AUTHOR("Dongseok lee <drain.lee@samsung.com>");
MODULE_DESCRIPTION("User level device for DMAEngine");
MODULE_LICENSE("Proprietary");
MODULE_ALIAS("dma:sdp-dmadev");
