/*
 * Hardware Dedicated Memory Allocator
 * Copyright (c) 2011 by Samsung Electronics.
 * Written by Vasily Leonenko (v.leonenko@samsung.com)
 */
#include <linux/err.h>
#include <linux/cma.h>
#include <linux/hdma.h>
#include <linux/mm.h>

struct cmainfo hdmainfo;
EXPORT_SYMBOL(hdmainfo);

static int __init hdma_add_region(unsigned long start, unsigned long size)
{
	int i;
	struct cmaregion *region = &hdmainfo.region[hdmainfo.nr_regions];

	if (hdmainfo.nr_regions >= MAX_CMA_REGIONS) {
		printk(KERN_CRIT "MAX_CMA_REGIONS too low, "
			"ignoring memory at %#lx\n", start);
		return -EINVAL;
	}

	for (i = 0; i < hdmainfo.nr_regions; i++) {
		struct cmaregion *r = &hdmainfo.region[i];
		if (cma_regions_overlap(r->start, r->size, start, size)) {
			printk(KERN_CRIT "hdma: Found regions overlap for"
					"[0x%lx@0x%lx]. Region skipped.\n",
					size, start);
			return -EINVAL;
		}
	}

	/*
	 * Ensure that start/size are aligned to a page boundary.
	 * Start is appropriately rounded down, size is rounded up.
	 */
	size += start & ~PAGE_MASK;
	region->start = start & PAGE_MASK;
	region->size  = PAGE_ALIGN(size);
	region->free = region->size;
	region->ctx   = NULL;

	/*
	 * Check whether this memory region has non-zero size or
	 * invalid node number.
	 */
	if (region->size == 0)
		return -EINVAL;

	hdmainfo.nr_regions++;
	return 0;
}

/*
 * Pick out the HDMA size and offset.  We look for hdma=size@start,
 * where start and size are "size[KkMm]"
 */
static int __init early_hdma(char *p)
{
	unsigned long size, start = 0;
	char *endp;

	size  = memparse(p, &endp);
	if (*endp != '@') {
		printk(KERN_ERR "hdma cmdline parameter [%s] skipped."
				"Check parameter form [size@offt]\n", p);
		return 0;
	}
	start = memparse(endp + 1, NULL);
	hdma_add_region(start, size);

	return 0;
}
early_param("hdma", early_hdma);

void hdma_regions_reserve(void)
{
	int i;
	unsigned long start;
	struct cmaregion *region;

	for (i = 0; i < hdmainfo.nr_regions; i++) {
		region = &hdmainfo.region[i];
		start = cma_reserve(region->start, region->size, 0, true);
		if (IS_ERR_VALUE(start)) {
			printk(KERN_WARNING "hdma: unable to reserve %lu "
			"for CMA: %d\n", region->size >> 20,
			(int)region->start);
			region->start = start;
		} else if (region->start == 0)
			region->start = start;
	}
}

static int __init hdma_regions_init(void)
{
	int i, ret = 0;
	struct cmaregion *region;
	struct cma *ctx;

	for (i = 0; i < hdmainfo.nr_regions; i++) {
		region = &hdmainfo.region[i];
		if (region->start && !IS_ERR_VALUE(region->start)) {
			ctx = cma_create(region->start, region->size, 0, false);
			if (IS_ERR(ctx)) {
				ret = PTR_ERR(ctx);
				printk(KERN_WARNING
					"hdma: cma_create(%p, %p) failed: %d\n",
					(void *)region->start,
					(void *)region->size, ret);
			} else {
				ctx->region = region;
				region->ctx = ctx;
			}
		}
	}
	return ret;
}
device_initcall(hdma_regions_init);
