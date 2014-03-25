/* incluce/linux/sdp_dmadev.h
 *
 * Copyright (C) 2011 Samsung Electronics Co. Ltd.
 * Dongseok lee <drain.lee@samsung.com>
 */

 /*
 * 20110621	created by drain.lee
 * 20110918	change ioctl struct
 */

#include <linux/types.h>
#include <linux/ioctl.h>


#ifndef _LINUX_SDP_DMADEV_H
#define _LINUX_SDP_DMADEV_H

#define SDP_DMADEV_IOC_MAGIC		'D'

/* testflag */
enum sdp_dmadev_testflags {
	SDP_DMADEV_DISABLE_SG_OPTIMIZE		= (0x1 << 0),
	SDP_DMADEV_SKIP_SRC_CLEAN			= (0x1 << 1),
	SDP_DMADEV_SKIP_DST_INVALIDATE		= (0x1 << 2),
	SDP_DMADEV_SKIP_VIRT_TO_PHYS		= (0x1 << 3),/* use physical memory */
};

struct sdp_dmadev_ioctl_args {
	unsigned long src_addr;
	unsigned long dst_addr;
	size_t len;

	unsigned long testflag;
	int flush_all_size;
};

#ifdef CONFIG_SDP_DMA330_2DCOPY
struct sdp_dmadev_window {
	unsigned long start_addr;
	unsigned short x;
	unsigned short y;
	unsigned short width;
	unsigned short height;
};

struct sdp_dmadev_ioctl_2dcpy {
	struct sdp_dmadev_window src_window;/* info src window */
	struct sdp_dmadev_window dst_window;/* info dst window */

	unsigned short srcx;
	unsigned short srcy;
	unsigned short dstx;
	unsigned short dsty;
	unsigned short width;
	unsigned short height;

	unsigned char bpp;/* bit per pixel */

	enum pl330_srccachectrl src_cache;
	enum pl330_dstcachectrl dst_cache;
};

/*
	SDP_DMADEV_IOC_2DCPY
	Return : <0 Error, 0 OK(sync), >0 OK, return DMA handle(async)
*/
#define SDP_DMADEV_IOC_2DCPY		_IOW(SDP_DMADEV_IOC_MAGIC, 4, struct sdp_dmadev_ioctl_2dcpy)

#endif

struct sdp_dmadev_is_cmp_args {
	int dma_handle;
};

/* for dma memcpy */
/*
	SDP_DMADEV_IOC_MEMCPY
	Return : <0 Error, 0 OK(sync), >0 OK, return DMA handle(async)
*/
#define SDP_DMADEV_IOC_MEMCPY		_IOW(SDP_DMADEV_IOC_MAGIC, 1, struct sdp_dmadev_ioctl_args)

/* for dma memcpy use physical address */
/*
	SDP_DMADEV_IOC_MEMCPY_PHYS
	Return : <0 Error, 0 OK(sync), >0 OK, return DMA handle(async)
*/
#define SDP_DMADEV_IOC_MEMCPY_PHYS		_IOW(SDP_DMADEV_IOC_MAGIC, 2, struct sdp_dmadev_ioctl_args)


/*
	SDP_DMADEV_IOC_IS_COMPLETE
	Return : <0 Error, 0 Complete, >0 Incomplete
*/
#define SDP_DMADEV_IOC_IS_COMPLETE	_IOW(SDP_DMADEV_IOC_MAGIC, 5, struct sdp_dmadev_is_cmp_args)

#endif/*_LINUX_SDP_DMADEV_H*/
