/*
 * DMA Engine test module
 *
 * Copyright (C) 2007 Atmel Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

 /*
  * modify for CCEP by Dongseok Lee     <drain.lee@samsung.com>
  *
  * 20120718	drain.lee	add manual address param.
  * 20121210	drain.lee	remove DMA330 dependency.
  */

#include <linux/delay.h>
#include <linux/dma-mapping.h>
#include <linux/dmaengine.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/random.h>
#include <linux/slab.h>
#include <linux/wait.h>
#include <asm/cacheflush.h>

static char *test_buf_size_str = "0x4000";
module_param_named(test_buf_size, test_buf_size_str, charp, S_IRUGO);
MODULE_PARM_DESC(test_buf_size, "Size of the memcpy test buffer(hex)");

static char test_channel[20];
module_param_string(channel, test_channel, sizeof(test_channel), S_IRUGO);
MODULE_PARM_DESC(channel, "Bus ID of the channel to test (default: any)");

static char test_device[20];
module_param_string(device, test_device, sizeof(test_device), S_IRUGO);
MODULE_PARM_DESC(device, "Bus ID of the DMA Engine to test (default: any)");

static unsigned int threads_per_chan = 1;
module_param(threads_per_chan, uint, S_IRUGO);
MODULE_PARM_DESC(threads_per_chan,
		"Number of threads to start per channel (default: 1)");

static unsigned int max_channels;
module_param(max_channels, uint, S_IRUGO);
MODULE_PARM_DESC(max_channels,
		"Maximum number of channels to use (default: all)");

static unsigned int iterations;
module_param(iterations, uint, S_IRUGO);
MODULE_PARM_DESC(iterations,
		"Iterations before stopping test (default: infinite)");

static unsigned int xor_sources = 3;
module_param(xor_sources, uint, S_IRUGO);
MODULE_PARM_DESC(xor_sources,
		"Number of xor source buffers (default: 3)");

static unsigned int pq_sources = 3;
module_param(pq_sources, uint, S_IRUGO);
MODULE_PARM_DESC(pq_sources,
		"Number of p+q source buffers (default: 3)");

static int timeout = 3000;
module_param(timeout, uint, S_IRUGO);
MODULE_PARM_DESC(timeout, "Transfer Timeout in msec (default: 3000), "
		 "Pass -1 for infinite timeout");



//static unsigned int manual_src = 0;
static char * manual_src_str = "0x0";
module_param_named(manual_src, manual_src_str, charp, S_IRUGO);
MODULE_PARM_DESC(manual_src,
		"Manual Src physical address.hex");

static char * manual_dst_str = "0x0";
module_param_named(manual_dst, manual_dst_str, charp, S_IRUGO);
MODULE_PARM_DESC(manual_dst,
		"Manual Dst physical address. hex");

static int no_verify = 0;
module_param(no_verify, bool, S_IRUGO);
MODULE_PARM_DESC(no_verify,
		"if y, skip verifing.");

static int no_random = 0;
module_param(no_random, bool, S_IRUGO);
MODULE_PARM_DESC(no_random,
		"if y, no random size and offset.");

static int print_interval = 10;
module_param(print_interval, uint, S_IRUGO);
MODULE_PARM_DESC(print_interval,
		"print msg interval default 10sec.");


static u32 test_buf_size = 0;

/*
 * Initialization patterns. All bytes in the source buffer has bit 7
 * set, all bytes in the destination buffer has bit 7 cleared.
 *
 * Bit 6 is set for all bytes which are to be copied by the DMA
 * engine. Bit 5 is set for all bytes which are to be overwritten by
 * the DMA engine.
 *
 * The remaining bits are the inverse of a counter which increments by
 * one for each byte address.
 */
#define PATTERN_SRC		0x80
#define PATTERN_DST		0x00
#define PATTERN_COPY		0x40
#define PATTERN_OVERWRITE	0x20
#define PATTERN_COUNT_MASK	0x1f

struct sdp_dmatest_thread {
	struct list_head	node;
	struct task_struct	*task;
	struct dma_chan		*chan;
	u8			**srcs;
	u8			**dsts;
	enum dma_transaction_type type;
	int	id;
};

struct sdp_dmatest_chan {
	struct list_head	node;
	struct dma_chan		*chan;
	struct list_head	threads;
};

#ifdef CONFIG_SDP_DMA330
struct dma_async_tx_descriptor *
sdp_dma330_cache_ctrl(struct dma_chan *chan, 
	struct dma_async_tx_descriptor *tx, u32 dst_cache, u32 src_cache);
#endif


/* 
 * This is Speed Test Util funcs.
 */
static unsigned long sdp_dmatest_calc_speed(unsigned long bytes, struct timeval * start, struct timeval * finish)
{
	unsigned long us, speed_kbs;

	us = ((finish->tv_sec - start->tv_sec) * 1000000u) +
	     ((finish->tv_usec - start->tv_usec) / 1u);

	if(us)
		speed_kbs = (bytes / us) << 10;
	else
		speed_kbs = 0;

	return speed_kbs;
}


/*
 * These are protected by dma_list_mutex since they're only used by
 * the DMA filter function callback
 */
static LIST_HEAD(sdp_dmatest_channels);
static unsigned int nr_channels;

static bool sdp_dmatest_match_channel(struct dma_chan *chan)
{
	if (test_channel[0] == '\0')
		return true;
	return strcmp(dma_chan_name(chan), test_channel) == 0;
}

static bool sdp_dmatest_match_device(struct dma_device *device)
{
	if (test_device[0] == '\0')
		return true;
	return strcmp(dev_name(device->dev), test_device) == 0;
}

static unsigned long sdp_dmatest_random(void)
{
	unsigned long buf;

	get_random_bytes(&buf, sizeof(buf));
	return buf;
}

static void sdp_dmatest_init_srcs(u8 **bufs, unsigned int start, unsigned int len)
{
	unsigned int i;
	u8 *buf;

	for (; (buf = *bufs); bufs++) {
		for (i = 0; i < start; i++)
			buf[i] = PATTERN_SRC | (~i & PATTERN_COUNT_MASK);
		for ( ; i < start + len; i++)
			buf[i] = PATTERN_SRC | PATTERN_COPY
				| (~i & PATTERN_COUNT_MASK);
		for ( ; i < test_buf_size; i++)
			buf[i] = PATTERN_SRC | (~i & PATTERN_COUNT_MASK);
		buf++;
	}
}

static void sdp_dmatest_init_dsts(u8 **bufs, unsigned int start, unsigned int len)
{
	unsigned int i;
	u8 *buf;

	for (; (buf = *bufs); bufs++) {
		for (i = 0; i < start; i++)
			buf[i] = PATTERN_DST | (~i & PATTERN_COUNT_MASK);
		for ( ; i < start + len; i++)
			buf[i] = PATTERN_DST | PATTERN_OVERWRITE
				| (~i & PATTERN_COUNT_MASK);
		for ( ; i < test_buf_size; i++)
			buf[i] = PATTERN_DST | (~i & PATTERN_COUNT_MASK);
	}
}

static void sdp_dmatest_mismatch(u8 actual, u8 pattern, unsigned int index,
		unsigned int counter, bool is_srcbuf)
{
	u8		diff = actual ^ pattern;
	u8		expected = pattern | (~counter & PATTERN_COUNT_MASK);
	const char	*thread_name = current->comm;

	if (is_srcbuf)
		pr_warning("%s: srcbuf[0x%x] overwritten!"
				" Expected %02x, got %02x\n",
				thread_name, index, expected, actual);
	else if ((pattern & PATTERN_COPY)
			&& (diff & (PATTERN_COPY | PATTERN_OVERWRITE)))
		pr_warning("%s: dstbuf[0x%x] not copied!"
				" Expected %02x, got %02x\n",
				thread_name, index, expected, actual);
	else if (diff & PATTERN_SRC)
		pr_warning("%s: dstbuf[0x%x] was copied!"
				" Expected %02x, got %02x\n",
				thread_name, index, expected, actual);
	else
		pr_warning("%s: dstbuf[0x%x] mismatch!"
				" Expected %02x, got %02x\n",
				thread_name, index, expected, actual);
}

static unsigned int sdp_dmatest_verify(u8 **bufs, unsigned int start,
		unsigned int end, unsigned int counter, u8 pattern,
		bool is_srcbuf)
{
	unsigned int i;
	unsigned int error_count = 0;
	u8 actual;
	u8 expected;
	u8 *buf;
	unsigned int counter_orig = counter;

	for (; (buf = *bufs); bufs++) {
		counter = counter_orig;
		for (i = start; i < end; i++) {
			actual = buf[i];
			expected = pattern | (~counter & PATTERN_COUNT_MASK);
			if (actual != expected) {
				if(error_count == 0) {
					pr_warning("%s: %sbuf: 0x%x, start: 0x%x, end: 0x%x\n",
						current->comm, is_srcbuf?"src":"dst", *bufs, start, end);
				}
				if (error_count < 32)
					sdp_dmatest_mismatch(actual, pattern, i,
							 counter, is_srcbuf);
				error_count++;
			}
			counter++;
		}
	}

	if (error_count > 32)
		pr_warning("%s: %u errors suppressed\n",
			current->comm, error_count - 32);

	return error_count;
}

static void sdp_dmatest_callback(void *completion)
{
	complete(completion);
}

/*
 * This function repeatedly tests DMA transfers of various lengths and
 * offsets for a given operation type until it is told to exit by
 * kthread_stop(). There may be multiple threads running this function
 * in parallel for a single channel, and there may be multiple channels
 * being tested in parallel.
 *
 * Before each test, the source and destination buffer is initialized
 * with a known pattern. This pattern is different depending on
 * whether it's in an area which is supposed to be copied or
 * overwritten, and different in the source and destination buffers.
 * So if the DMA engine doesn't copy exactly what we tell it to copy,
 * we'll notice.
 */
static int sdp_dmatest_func(void *data)
{
	struct sdp_dmatest_thread	*thread = data;
	struct dma_chan		*chan;
	const char		*thread_name;
	unsigned int		src_off, dst_off, len;
	unsigned int		error_count;
	unsigned int		failed_tests = 0;
	unsigned int		total_tests = 0;
	dma_cookie_t		cookie;
	enum dma_status		status;
	enum dma_ctrl_flags 	flags;
	u8			pq_coefs[pq_sources + 1];
	int			ret;
	int			src_cnt;
	int			dst_cnt;
	int			i;

	static struct timeval start, finish;
	struct timeval print_time = {0};

	u32 manual_src = simple_strtoul(manual_src_str, NULL, 16);
	u32 manual_dst = simple_strtoul(manual_dst_str, NULL, 16);
	test_buf_size = simple_strtoul(test_buf_size_str, NULL, 16);

	thread_name = current->comm;

	ret = -ENOMEM;

	smp_rmb();
	chan = thread->chan;
	if (thread->type == DMA_MEMCPY)
		src_cnt = dst_cnt = 1;
	else if (thread->type == DMA_XOR) {
		src_cnt = xor_sources | 1; /* force odd to ensure dst = src */
		dst_cnt = 1;
	} else if (thread->type == DMA_PQ) {
		src_cnt = pq_sources | 1; /* force odd to ensure dst = src */
		dst_cnt = 2;
		for (i = 0; i < src_cnt; i++)
			pq_coefs[i] = 1;
	} else
		goto err_srcs;

	/* alloc Source buffer */
	thread->srcs = kcalloc(src_cnt+1, sizeof(u8 *), GFP_KERNEL);
	if (!thread->srcs)
			goto err_srcs;
	for (i = 0; i < src_cnt; i++) {
		if(manual_src)
		{
			if(i > 0)
			{
				pr_err("%s: not surppot multi buff at manual src.", thread_name);
				break;
			}
			thread->srcs[i] = ioremap_nocache(manual_src + (test_buf_size * thread->id), test_buf_size);
			pr_info("%s: manual src: %#x remaped at 0x%p\n", thread_name, manual_src + (test_buf_size * thread->id), thread->srcs[i]);
		}
		else
		{
			thread->srcs[i] = kmalloc(test_buf_size, GFP_KERNEL);
		}
		if (!thread->srcs[i])
			goto err_srcbuf;
	}
	thread->srcs[i] = NULL;



	/* alloc Dest buffer */
	thread->dsts = kcalloc(dst_cnt+1, sizeof(u8 *), GFP_KERNEL);
	if (!thread->dsts)
		goto err_dsts;
	for (i = 0; i < dst_cnt; i++) {
		if(manual_dst)
		{
			if(i > 0)
			{
				pr_err("%s: not surppot multi buff at manual dst.", thread_name);
				break;
			}
			thread->dsts[i] = ioremap_nocache(manual_dst + (test_buf_size * thread->id), test_buf_size);
			pr_info("%s: manual dst: %#x remaped at 0x%p\n", thread_name, manual_dst + (test_buf_size * thread->id), thread->dsts[i]);
		}
		else
		{
			thread->dsts[i] = kmalloc(test_buf_size, GFP_KERNEL);
		}
		if (!thread->dsts[i])
			goto err_dstbuf;
	}
	thread->dsts[i] = NULL;

	set_user_nice(current, 10);

	/*
	 * src buffers are freed by the DMAEngine code with dma_unmap_single()
	 * dst buffers are freed by ourselves below
	 */
	flags = DMA_CTRL_ACK | DMA_PREP_INTERRUPT
	      | DMA_COMPL_SKIP_DEST_UNMAP | DMA_COMPL_SRC_UNMAP_SINGLE;

	if(manual_src)
		flags |= DMA_COMPL_SKIP_SRC_UNMAP;
	if(manual_dst)
		flags |= DMA_COMPL_SKIP_DEST_UNMAP;

	while (!kthread_should_stop()
	       && !(iterations && total_tests >= iterations)) {
		struct dma_device *dev = chan->device;
		struct dma_async_tx_descriptor *tx = NULL;
		dma_addr_t dma_srcs[src_cnt];
		dma_addr_t dma_dsts[dst_cnt];
		struct completion cmp;
		unsigned long tmo = msecs_to_jiffies(timeout);
		u8 align = 0;

		total_tests++;

		/* honor alignment restrictions */
		if (thread->type == DMA_MEMCPY)
			align = dev->copy_align;
		else if (thread->type == DMA_XOR)
			align = dev->xor_align;
		else if (thread->type == DMA_PQ)
			align = dev->pq_align;

		if (1 << align > test_buf_size) {
			pr_err("%u-byte buffer too small for %d-byte alignment\n",
			       test_buf_size, 1 << align);
			break;
		}

		if(no_random) {
			len = test_buf_size;
			len = (len >> align) << align;
			src_off = 0;
			dst_off = 0;
		} else {
			len = sdp_dmatest_random() % test_buf_size + 1;
			len = (len >> align) << align;
			if (!len)
				len = 1 << align;
			src_off = sdp_dmatest_random() % (test_buf_size - len + 1);
			dst_off = sdp_dmatest_random() % (test_buf_size - len + 1);
		}

		src_off = (src_off >> align) << align;
		dst_off = (dst_off >> align) << align;

		if(!no_verify) {
			sdp_dmatest_init_srcs(thread->srcs, src_off, len);
			sdp_dmatest_init_dsts(thread->dsts, dst_off, len);
		}

		do_gettimeofday(&start);

		for (i = 0; i < src_cnt; i++) {
			if(manual_src)
			{
				dma_srcs[i] = (manual_src + (test_buf_size* thread->id)) + src_off;
			}
			else
			{
				u8 *buf = thread->srcs[i] + src_off;

				dma_srcs[i] = dma_map_single(dev->dev, buf, len,
							     DMA_TO_DEVICE);
			}
		}
		/* map with DMA_BIDIRECTIONAL to force writeback/invalidate */
		for (i = 0; i < dst_cnt; i++) {
			if(manual_dst)
			{
				dma_dsts[i] = manual_dst + (test_buf_size* thread->id);
			}
			else
			{
				dma_dsts[i] = dma_map_single(dev->dev, thread->dsts[i],
							     test_buf_size,
							     DMA_BIDIRECTIONAL);
			}
		}

		pr_debug("%s: [Src Addr] CPU 0x%p, DMA %#x [Dst Addr] CPU 0x%p DMA %#x dst_off %#x\n", thread_name, thread->srcs[0], dma_srcs[0], thread->dsts[0], dma_dsts[0], dst_off);


		if (thread->type == DMA_MEMCPY)
			tx = dev->device_prep_dma_memcpy(chan,
							 dma_dsts[0] + dst_off,
							 dma_srcs[0], len,
							 flags);
		else if (thread->type == DMA_XOR)
			tx = dev->device_prep_dma_xor(chan,
						      dma_dsts[0] + dst_off,
						      dma_srcs, src_cnt,
						      len, flags);
		else if (thread->type == DMA_PQ) {
			dma_addr_t dma_pq[dst_cnt];

			for (i = 0; i < dst_cnt; i++)
				dma_pq[i] = dma_dsts[i] + dst_off;
			tx = dev->device_prep_dma_pq(chan, dma_pq, dma_srcs,
						     src_cnt, pq_coefs,
						     len, flags);
		}

		if (!tx) {
			for (i = 0; i < src_cnt; i++)
			{
				if(manual_src)
				{
				}
				else
				{
					dma_unmap_single(dev->dev, dma_srcs[i], len,
							 DMA_TO_DEVICE);
				}
			}

			for (i = 0; i < dst_cnt; i++)
			{
				if(manual_dst)
				{
				}
				else
				{
					dma_unmap_single(dev->dev, dma_dsts[i],
							 test_buf_size,
							 DMA_BIDIRECTIONAL);
				}
			}
			pr_warning("%s: #%u: prep error with src_off=0x%x "
					"dst_off=0x%x len=0x%x\n",
					thread_name, total_tests - 1,
					src_off, dst_off, len);
			msleep(100);
			failed_tests++;
			continue;
		}

#ifdef CONFIG_SDP_DMA330
		tx = sdp_dma330_cache_ctrl(chan, tx, 7, 7);
		if(!tx) {
			pr_err("%s: DMA330 Cache control error!\n", thread_name);
		}
#endif

		init_completion(&cmp);
		tx->callback = sdp_dmatest_callback;
		tx->callback_param = &cmp;
		cookie = tx->tx_submit(tx);

		if (dma_submit_error(cookie)) {
			pr_warning("%s: #%u: submit error %d with src_off=0x%x "
					"dst_off=0x%x len=0x%x\n",
					thread_name, total_tests - 1, cookie,
					src_off, dst_off, len);
			msleep(100);
			failed_tests++;
			continue;
		}
		dma_async_issue_pending(chan);

		tmo = wait_for_completion_timeout(&cmp, tmo);
		status = dma_async_is_tx_complete(chan, cookie, NULL, NULL);

		if (tmo == 0) {
			pr_warning("%s: #%u: test timed out\n",
				   thread_name, total_tests - 1);
			failed_tests++;
			continue;
		} else if (status != DMA_SUCCESS) {
			pr_warning("%s: #%u: got completion callback,"
				   " but status is \'%s\'\n",
				   thread_name, total_tests - 1,
				   status == DMA_ERROR ? "error" : "in progress");
			chan->device->device_control(chan, DMA_TERMINATE_ALL, 0);
			failed_tests++;
			continue;
		}

		/* Unmap by myself (see DMA_COMPL_SKIP_DEST_UNMAP above) */
		for (i = 0; i < dst_cnt; i++)
		{
			if(manual_dst)
			{
			}
			else
			{
				dma_unmap_single(dev->dev, dma_dsts[i], test_buf_size,
						 DMA_BIDIRECTIONAL);
			}
		}
		error_count = 0;

		do_gettimeofday(&finish);

		if(print_time.tv_sec+print_interval < finish.tv_sec)
		{
			unsigned long us = ((finish.tv_sec - start.tv_sec) * 1000000u) +
							     ((finish.tv_usec - start.tv_usec) / 1u);
			pr_info("%s: now %5d tests, speed %4luMB/s, len %10dbyte, %luus\n",
				thread_name, total_tests, sdp_dmatest_calc_speed(len, &start, &finish)>>10, len, us);
			print_time = finish;
		}	

		if(!no_verify) {
			pr_debug("%s: verifying source buffer...\n", thread_name);
			error_count += sdp_dmatest_verify(thread->srcs, 0, src_off,
					0, PATTERN_SRC, true);
			error_count += sdp_dmatest_verify(thread->srcs, src_off,
					src_off + len, src_off,
					PATTERN_SRC | PATTERN_COPY, true);
			error_count += sdp_dmatest_verify(thread->srcs, src_off + len,
					test_buf_size, src_off + len,
					PATTERN_SRC, true);

			pr_debug("%s: verifying dest buffer...\n",
					thread->task->comm);
			error_count += sdp_dmatest_verify(thread->dsts, 0, dst_off,
					0, PATTERN_DST, false);
			error_count += sdp_dmatest_verify(thread->dsts, dst_off,
					dst_off + len, src_off,
					PATTERN_SRC | PATTERN_COPY, false);
			error_count += sdp_dmatest_verify(thread->dsts, dst_off + len,
					test_buf_size, dst_off + len,
					PATTERN_DST, false);

			if (error_count) {
				pr_warning("%s: #%u: %u errors with "
					"src_off=0x%x dst_off=0x%x len=0x%x\n",
					thread_name, total_tests - 1, error_count,
					src_off, dst_off, len);
				failed_tests++;
			} else {
				pr_debug("%s: #%u: No errors with "
					"src_off=0x%x dst_off=0x%x len=0x%x\n",
					thread_name, total_tests - 1,
					src_off, dst_off, len);
			}
		}
	}

	ret = 0;
	for (i = 0; thread->dsts[i]; i++)
	{
		if(manual_dst)
			iounmap(thread->dsts[i]);
		else
			kfree(thread->dsts[i]);
	}
err_dstbuf:
	kfree(thread->dsts);
err_dsts:
	for (i = 0; thread->srcs[i]; i++)
	{
		if(manual_src)
			iounmap(thread->srcs[i]);
		else
			kfree(thread->srcs[i]);
	}
err_srcbuf:
	kfree(thread->srcs);
err_srcs:
	pr_notice("%s: terminating after %u tests, %u failures (status %d)\n",
			thread_name, total_tests, failed_tests, ret);

	if (iterations > 0)
		while (!kthread_should_stop()) {
			DECLARE_WAIT_QUEUE_HEAD_ONSTACK(wait_sdp_dmatest_exit);
			interruptible_sleep_on(&wait_sdp_dmatest_exit);
		}

	return ret;
}

static void sdp_dmatest_cleanup_channel(struct sdp_dmatest_chan *dtc)
{
	struct sdp_dmatest_thread	*thread;
	struct sdp_dmatest_thread	*_thread;
	int			ret;

	list_for_each_entry_safe(thread, _thread, &dtc->threads, node) {
		ret = kthread_stop(thread->task);
		pr_debug("sdp_dmatest: thread %s exited with status %d\n",
				thread->task->comm, ret);
		list_del(&thread->node);
		kfree(thread);
	}
	kfree(dtc);
}

static int sdp_dmatest_add_threads(struct sdp_dmatest_chan *dtc, enum dma_transaction_type type)
{
	struct sdp_dmatest_thread *thread;
	struct dma_chan *chan = dtc->chan;
	char *op;
	unsigned int i;
	static int thread_cnt = 0;

	if (type == DMA_MEMCPY)
		op = "copy";
	else if (type == DMA_XOR)
		op = "xor";
	else if (type == DMA_PQ)
		op = "pq";
	else
		return -EINVAL;

	for (i = 0; i < threads_per_chan; i++) {
		thread = kzalloc(sizeof(struct sdp_dmatest_thread), GFP_KERNEL);
		if (!thread) {
			pr_warning("sdp_dmatest: No memory for %s-%s%u\n",
				   dma_chan_name(chan), op, i);

			break;
		}
		thread->chan = dtc->chan;
		thread->type = type;
		thread->id = thread_cnt++;
		smp_wmb();
		thread->task = kthread_run(sdp_dmatest_func, thread, "%s-%s%u(id:%d)",
				dma_chan_name(chan), op, i, thread->id);
		if (IS_ERR(thread->task)) {
			pr_warning("sdp_dmatest: Failed to run thread %s-%s%u\n",
					dma_chan_name(chan), op, i);
			kfree(thread);
			break;
		}

		/* srcbuf and dstbuf are allocated by the thread itself */

		list_add_tail(&thread->node, &dtc->threads);
	}

	return i;
}

static int sdp_dmatest_add_channel(struct dma_chan *chan)
{
	struct sdp_dmatest_chan	*dtc;
	struct dma_device	*dma_dev = chan->device;
	unsigned int		thread_count = 0;
	int cnt;

	dtc = kmalloc(sizeof(struct sdp_dmatest_chan), GFP_KERNEL);
	if (!dtc) {
		pr_warning("sdp_dmatest: No memory for %s\n", dma_chan_name(chan));
		return -ENOMEM;
	}

	dtc->chan = chan;
	INIT_LIST_HEAD(&dtc->threads);

	if (dma_has_cap(DMA_MEMCPY, dma_dev->cap_mask)) {
		cnt = sdp_dmatest_add_threads(dtc, DMA_MEMCPY);
		thread_count += cnt > 0 ? cnt : 0;
	}
	if (dma_has_cap(DMA_XOR, dma_dev->cap_mask)) {
		cnt = sdp_dmatest_add_threads(dtc, DMA_XOR);
		thread_count += cnt > 0 ? cnt : 0;
	}
	if (dma_has_cap(DMA_PQ, dma_dev->cap_mask)) {
		cnt = sdp_dmatest_add_threads(dtc, DMA_PQ);
		thread_count += cnt > 0 ? cnt: 0;
	}

	pr_info("sdp_dmatest: Started %u threads using %s\n",
		thread_count, dma_chan_name(chan));

	list_add_tail(&dtc->node, &sdp_dmatest_channels);
	nr_channels++;

	return 0;
}

static bool filter(struct dma_chan *chan, void *param)
{
	if (!sdp_dmatest_match_channel(chan) || !sdp_dmatest_match_device(chan->device))
		return false;
	else
		return true;
}

static int __init sdp_dmatest_init(void)
{
	dma_cap_mask_t mask;
	struct dma_chan *chan;
	int err = 0;

	dma_cap_zero(mask);
	dma_cap_set(DMA_MEMCPY, mask);
	for (;;) {
		chan = dma_request_channel(mask, filter, NULL);
		if (chan) {
			err = sdp_dmatest_add_channel(chan);
			if (err) {
				dma_release_channel(chan);
				break; /* add_channel failed, punt */
			}
		} else
			break; /* no more channels available */
		if (max_channels && nr_channels >= max_channels)
			break; /* we have all we need */
	}

	return err;
}
/* when compiled-in wait for drivers to load first */
late_initcall(sdp_dmatest_init);

static void __exit sdp_dmatest_exit(void)
{
	struct sdp_dmatest_chan *dtc, *_dtc;
	struct dma_chan *chan;

	list_for_each_entry_safe(dtc, _dtc, &sdp_dmatest_channels, node) {
		list_del(&dtc->node);
		chan = dtc->chan;
		sdp_dmatest_cleanup_channel(dtc);
		pr_debug("sdp_dmatest: dropped channel %s\n",
			 dma_chan_name(chan));
		dma_release_channel(chan);
	}
}
module_exit(sdp_dmatest_exit);

MODULE_AUTHOR("Haavard Skinnemoen (Atmel)");
MODULE_LICENSE("GPL v2");
