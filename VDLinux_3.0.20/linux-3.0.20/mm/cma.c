/*
 * Contiguous Memory Allocator framework
 * Copyright (c) 2010 by Samsung Electronics.
 * Written by Michal Nazarewicz (m.nazarewicz@samsung.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License or (at your optional) any later version of the license.
 */

/*
 * See include/linux/cma.h for details.
 */

#define pr_fmt(fmt) "cma: " fmt

#ifdef CONFIG_CMA_DEBUG
#  define DEBUG
#endif

#include <linux/cma.h>
#include <linux/bootmem.h>
#include <linux/err.h>
#include <linux/genalloc.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/page-isolation.h>
#include <linux/slab.h>
#include <linux/swap.h>

#include "internal.h"

#include <linux/memblock.h>

/********************** CMA chunks RB-Tree ************************/
static inline void cma_rb_insert(struct cma *cmactx,
						struct cm *cm)
{
	struct rb_node **p = &cmactx->rbroot.rb_node;
	struct rb_node *parent = NULL;
	struct cm *c;

	while (*p) {
		parent = *p;
		c = rb_entry(parent, struct cm, rbnode);

		BUG_ON(cm->phys == c->phys);

		if (cm->phys  < c->phys)
			p = &(*p)->rb_left;
		else if (cm->phys  > c->phys)
			p = &(*p)->rb_right;
	}
	rb_link_node(&cm->rbnode, parent, p);
	rb_insert_color(&cm->rbnode, &cmactx->rbroot);
}

static inline void cma_rb_remove(struct cma *cmactx,
						struct cm *cm)
{
	rb_erase(&cm->rbnode, &cmactx->rbroot);
}

static inline struct cm **cma_rb_get_list(struct cma *cmactx)
{
	int i = 0;
	struct cm **cm;
	struct rb_node *n;

	if (!cmactx)
		return NULL;

	if (!cmactx->chunks)
		return NULL;

	cm = kmalloc(sizeof(struct cm *) * cmactx->chunks, GFP_KERNEL);
	if (!cm)
		return NULL;
	n = rb_first(&cmactx->rbroot);
	while (n) {
		BUG_ON(i >= cmactx->chunks);
		cm[i] = rb_entry(n, struct cm, rbnode);
		n = rb_next(n);
		i++;
	}

	return cm;
}

struct cm **cma_get_chunks_list(struct cma *cmactx)
{
	return cma_rb_get_list(cmactx);
}
EXPORT_SYMBOL_GPL(cma_get_chunks_list);

unsigned long cma_get_largest(struct cma *cmactx)
{
	int i;
	struct cm **cm;
	unsigned long largest = 0;

	if (!cmactx)
		goto out;

	cm = cma_get_chunks_list(cmactx);

	if (!cm) {
		largest = cmactx->region->size;
		goto out;
	}

	largest = cm[0]->phys - cmactx->region->start;
	for (i = 1; i < cmactx->chunks; i++)
		largest = max(largest,
			cm[i]->phys - cm[i-1]->phys - cm[i-1]->size);
	largest = max(largest, cmactx->region->start +
		cmactx->region->size - cm[i-1]->phys - cm[i-1]->size);
	kfree(cm);
out:
	return largest;
}
EXPORT_SYMBOL_GPL(cma_get_largest);

/************************* Initialise CMA *************************/

#ifdef CONFIG_MIGRATE_CMA

static struct cma_grabbed {
	unsigned long start;
	unsigned long size;
} cma_grabbed[8] __initdata;
static unsigned cma_grabbed_count __initdata;

static int __cma_give_back(unsigned long start, unsigned long size)
{
	int i;
	unsigned long pfn;
	unsigned long _start = ALIGN(start, MAX_ORDER_NR_PAGES << PAGE_SHIFT);
	if (_start > start)
		size += start + (pageblock_nr_pages << PAGE_SHIFT) - _start;
	size = ALIGN(size, MAX_ORDER_NR_PAGES << PAGE_SHIFT);
	i = size >> (PAGE_SHIFT + pageblock_order);
	pfn = phys_to_pfn(start) & ~(pageblock_nr_pages - 1);

	pr_debug("%s(%p+%p)\n", __func__, (void *)start, (void *)size);

	do {
		__free_pageblock_cma(pfn_to_page(pfn));
		pfn += pageblock_nr_pages;
	} while (--i > 0);

	return 0;
}

static int __init __cma_queue_give_back(unsigned long start, unsigned long size)
{
	if (cma_grabbed_count == ARRAY_SIZE(cma_grabbed))
		return -ENOSPC;

	cma_grabbed[cma_grabbed_count].start = start;
	cma_grabbed[cma_grabbed_count].size  = size;
	++cma_grabbed_count;
	return 0;
}

static int (*cma_give_back)(unsigned long start, unsigned long size) =
	__cma_queue_give_back;

static int __init cma_give_back_queued(void)
{
	struct cma_grabbed *r = cma_grabbed;
	unsigned i = cma_grabbed_count;

	pr_debug("%s(): will give %u range(s)\n", __func__, i);

	cma_give_back = __cma_give_back;

	for (; i; --i, ++r)
		__cma_give_back(r->start, r->size);

	return 0;
}
subsys_initcall(cma_give_back_queued);

int __ref cma_init_migratetype(unsigned long start, unsigned long size)
{
	pr_debug("%s(%p+%p)\n", __func__, (void *)start, (void *)size);

	if (!size)
		return -EINVAL;
	if (start + size < start)
		return -EOVERFLOW;

	return cma_give_back(start, size);
}

#endif

extern phys_addr_t lowmem_limit;
unsigned long cma_reserve(unsigned long start, unsigned long size,
			  unsigned long alignment, bool init_migratetype)
{
	unsigned long pfn;
	unsigned long _start = start;
	unsigned long _size = size;
	unsigned long _max_low_pfn = max_low_pfn;

	pr_debug("%s(%p+%p/%p)\n", __func__, (void *)start, (void *)size,
		 (void *)alignment);

#ifndef CONFIG_MIGRATE_CMA
	if (init_migratetype)
		return -EOPNOTSUPP;
#endif

	/* Sanity checks */
	if (!size || (alignment & (alignment - 1)))
		return (unsigned long)-EINVAL;

	/* Sanitise input arguments */
	if (init_migratetype) {
		start = ALIGN(start, MAX_ORDER_NR_PAGES << PAGE_SHIFT);
		/* Align start and size by left boundary */
		if (start > _start) {
			start -= (MAX_ORDER_NR_PAGES << PAGE_SHIFT);
			size += _start - start;
		}
		size  = ALIGN(size , MAX_ORDER_NR_PAGES << PAGE_SHIFT);
		/* Only for case with assigned start address */
		if (start) {
			while (!pfn_valid((start + size) >> PAGE_SHIFT) &&
				size > 0)
				size--;
			if (size)
				size++;
		}
		if (alignment < (MAX_ORDER_NR_PAGES << PAGE_SHIFT))
			alignment = MAX_ORDER_NR_PAGES << PAGE_SHIFT;
	} else {
		start = PAGE_ALIGN(start);
		size  = PAGE_ALIGN(size);
		if (alignment < PAGE_SIZE)
			alignment = PAGE_SIZE;
	}

	if (size < _size) {
		pr_debug("%s region size decreased after start/size corrections"
		"(%p+%p/%p)\n", __func__, (void *)start, (void *)size,
		(void *)alignment);
		return (unsigned long)-EINVAL;
	}

	if (start >= lowmem_limit)
		goto init;

	/* Reserve memory */
	if (start) {
		if (memblock_reserve(start, size) < 0)
			return (unsigned long)-EBUSY;
	} else {
		unsigned long addr = 0;
		/*
		 * Use __alloc_bootmem_nopanic() since
		 * __alloc_bootmem() panic()s.
		 */
		addr = (unsigned long)
			memblock_alloc_base(size, alignment, 0);
		addr = (unsigned long)virt_to_phys((void *)addr);
		if (!addr) {
			return (unsigned long)-ENOMEM;
		} else if (addr + size > ~(unsigned long)0) {
			memblock_free(addr, size);
			return (unsigned long)-EOVERFLOW;
		} else {
			start = addr;
		}
		_start = start;
	}

init:
	/* All pageblocks should be valid */
	for (pfn = phys_to_pfn(start);
		pfn < phys_to_pfn(start + size);
		pfn += pageblock_nr_pages) {
		if (!pfn_valid(pfn)) {
			pr_debug("%s found !valid pfn %p in region(%p+%p)\n",
			__func__, (void *)pfn, (void *)start, (void *)size);
			return (unsigned long)-EINVAL;
		}
	}

	/* CMA Initialise */
	if (init_migratetype) {
		int ret = cma_init_migratetype(_start, _size);
		if (ret < 0 && phys_to_pfn(start) < _max_low_pfn) {
			memblock_free(start, size);
			return ret;
		}
	}

#ifdef CONFIG_MIGRATE_CMA
	/* Adjust totalram_pages for CMA case */
	if (init_migratetype)
		totalram_pages += size >> PAGE_SHIFT;
#endif
	return start;
}

struct cmainfo cmainfo;
EXPORT_SYMBOL(cmainfo);

static int __init cma_add_region(unsigned long start, unsigned long size)
{
	int i;
	struct cmaregion *region = &cmainfo.region[cmainfo.nr_regions];

	if (cmainfo.nr_regions >= MAX_CMA_REGIONS) {
		printk(KERN_CRIT "MAX_CMA_REGIONS too low, "
			"ignoring memory at %#lx\n", start);
		return -EINVAL;
	}

	for (i = 0; i < cmainfo.nr_regions; i++) {
		struct cmaregion *r = &cmainfo.region[i];
		if (cma_regions_overlap(r->start, r->size, start, size)) {
			printk(KERN_CRIT "cma: Found regions overlap for "
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

	cmainfo.nr_regions++;
	return 0;
}

/*
 * Pick out the CMA size and offset.  We look for cma=size@start,
 * where start and size are "size[KkMm]"
 */
static int __init early_cma(char *p)
{
	unsigned long size, start = 0;
	char *endp;

	size  = memparse(p, &endp);
	if (*endp != '@') {
		printk(KERN_ERR "cma cmdline parameter [%s] skipped."
			"Check parameter form [size@offt]\n", p);
		return 0;
	}
	start = memparse(endp + 1, NULL);
	cma_add_region(start, size);

	return 0;
}
early_param("cma", early_cma);

/************************** CMA context ***************************/

static int __cma_check_range(unsigned long start, unsigned long size)
{
	int migratetype = MIGRATE_MOVABLE;
	unsigned long pfn;
	long count;
	struct page *page;
	struct zone *zone;

	start = phys_to_pfn(start);
	if (WARN_ON(!pfn_valid(start)))
		return -EINVAL;

#ifdef CONFIG_MIGRATE_CMA
	if (page_zonenum(pfn_to_page(start)) != ZONE_MOVABLE)
		migratetype = MIGRATE_CMA;
#else
	if (WARN_ON(page_zonenum(pfn_to_page(start)) != ZONE_MOVABLE))
		return -EINVAL;
#endif

	/* First check if all pages are valid and in the same zone */
	zone  = page_zone(pfn_to_page(start));
	count = size >> PAGE_SHIFT;
	pfn   = start;
	while (++pfn, --count) {
		if (WARN_ON(!pfn_valid(pfn)) ||
		    WARN_ON(page_zone(pfn_to_page(pfn)) != zone))
			return -EINVAL;
	}

	/* Now check migratetype of their pageblocks. */
	start = start & ~(pageblock_nr_pages - 1);
	pfn   = ALIGN(pfn, pageblock_nr_pages);
	page  = pfn_to_page(start);
	count = (pfn - start) >> pageblock_order;
	do {
		if (WARN_ON(get_pageblock_migratetype(page) != migratetype))
			return -EINVAL;
		page += pageblock_nr_pages;
		count--;
	} while (count > 0);

	return migratetype;
}

struct cma *cma_create(unsigned long start, unsigned long size,
		       unsigned long min_alignment, bool private)
{
	struct gen_pool *pool;
	int migratetype, ret;
	struct cma *cma;

	pr_debug("%s(%p+%p)\n", __func__, (void *)start, (void *)size);

	if (!size)
		return ERR_PTR(-EINVAL);
	if (min_alignment & (min_alignment - 1))
		return ERR_PTR(-EINVAL);
	if (min_alignment < PAGE_SIZE)
		min_alignment = PAGE_SIZE;
	if ((start | size) & (min_alignment - 1))
		return ERR_PTR(-EINVAL);
	if (start + size < start)
		return ERR_PTR(-EOVERFLOW);

	if (private) {
		migratetype = 0;
	} else {
		migratetype = __cma_check_range(start, size);
		if (migratetype < 0)
			return ERR_PTR(migratetype);
	}

	cma = kmalloc(sizeof *cma, GFP_KERNEL);
	if (!cma)
		return ERR_PTR(-ENOMEM);

	pool = gen_pool_create(ffs(min_alignment) - 1, -1);
	if (!pool) {
		ret = -ENOMEM;
		goto error1;
	}

	ret = gen_pool_add(pool, start, size, -1);
	if (unlikely(ret))
		goto error2;

	cma->migratetype = migratetype;
	cma->pool = pool;
	cma->rbroot = RB_ROOT;
	cma->chunks = 0;

	pr_debug("%s: returning <%p>\n", __func__, (void *)cma);
	return cma;

error2:
	gen_pool_destroy(pool);
error1:
	kfree(cma);
	return ERR_PTR(ret);
}

void cma_destroy(struct cma *cma)
{
	pr_debug("%s(<%p>)\n", __func__, (void *)cma->pool);
	gen_pool_destroy((void *)cma->pool);
}


/************************* Allocate and free *************************/

/* Protects cm_alloc(), cm_free() as well as gen_pools of each cm. */
static DEFINE_MUTEX(cma_mutex);

struct cm *cm_alloc(struct cma *cma, unsigned long size,
			unsigned long alignment, unsigned long *failed_pages)
{
	unsigned long start;
	int ret = -ENOMEM;
	struct cm *cm;

	pr_debug("%s(<%p>, %p/%p)\n", __func__, (void *)cma,
		 (void *)size, (void *)alignment);

	if (!size || (alignment & (alignment - 1)))
		return ERR_PTR(-EINVAL);
	size = PAGE_ALIGN(size);

	cm = kmalloc(sizeof *cm, GFP_KERNEL);
	if (!cm)
		return ERR_PTR(-ENOMEM);

	mutex_lock(&cma_mutex);

	start = gen_pool_alloc_aligned(cma->pool, size,
				       alignment ? ffs(alignment) - 1 : 0);
	if (!start)
		goto error1;

	if (cma->migratetype) {
		unsigned long pfn = phys_to_pfn(start);
#ifdef CONFIG_CMA_DEBUG
		printk("%s(pfn : %lu, size : %lu)\n", __func__, pfn, size);
#endif
		struct zone *zone = page_zone(pfn_to_page(pfn));
		ret = alloc_contig_range(pfn, pfn + (size >> PAGE_SHIFT),
					 0, cma->migratetype);
		if (ret) {
			if (failed_pages && ret > 0) {
				*failed_pages = ret;
				ret = -EAGAIN;
			}
			goto error2;
		}
		spin_lock(&zone->lock);
		__mod_zone_page_state(zone, NR_UNEVICTABLE,
			(size >> PAGE_SHIFT));
		spin_unlock(&zone->lock);
	}

	cm->cma         = cma;
	cm->phys        = start;
	cm->size        = size;

	cm->cma->region->free -= size;
	cma->chunks++;
	cma_rb_insert(cma, cm);

	mutex_unlock(&cma_mutex);

	atomic_set(&cm->pinned, 0);
	atomic_set(&cm->mapped, 0);

	pr_debug("%s(): returning [%p]\n", __func__, (void *)cm);
	return cm;

error2:
	gen_pool_free((void *)cma->pool, start, size);
error1:
	mutex_unlock(&cma_mutex);
	kfree(cm);
	return ERR_PTR(ret);
}
EXPORT_SYMBOL_GPL(cm_alloc);

void cm_free(struct cm *cm)
{
	struct page *page;
	pr_debug("%s([%p])\n", __func__, (void *)cm);

	if (WARN_ON(atomic_read(&cm->pinned) || atomic_read(&cm->mapped)))
		return;

	mutex_lock(&cma_mutex);

	cm->cma->region->free += cm->size;
	cm->cma->chunks--;
	cma_rb_remove(cm->cma, cm);

	gen_pool_free(cm->cma->pool, cm->phys, cm->size);
	if (cm->cma->migratetype) {
		struct zone *zone;
		page = phys_to_page(cm->phys);
		zone = page_zone(page);
		free_contig_pages(page, cm->size >> PAGE_SHIFT);
		spin_lock(&zone->lock);
		__mod_zone_page_state(zone, NR_UNEVICTABLE,
			-(cm->size >> PAGE_SHIFT));
		spin_unlock(&zone->lock);
	}

	mutex_unlock(&cma_mutex);

	kfree(cm);
}
EXPORT_SYMBOL_GPL(cm_free);


/************************* Mapping and addresses *************************/

/*
 * Currently no-operations but keep reference counters for error
 * checking.
 */

unsigned long cm_pin(struct cm *cm)
{
	pr_debug("%s([%p])\n", __func__, (void *)cm);
	atomic_inc(&cm->pinned);
	return cm->phys;
}
EXPORT_SYMBOL_GPL(cm_pin);

void cm_unpin(struct cm *cm)
{
	pr_debug("%s([%p])\n", __func__, (void *)cm);
	WARN_ON(!atomic_add_unless(&cm->pinned, -1, 0));
}
EXPORT_SYMBOL_GPL(cm_unpin);

void cma_regions_reserve(void)
{
	int i;
	unsigned long start;
	struct cmaregion *region;

	for (i = 0; i < cmainfo.nr_regions; i++) {
		region = &cmainfo.region[i];
		start = cma_reserve(region->start, region->size, 0, true);
		if (IS_ERR_VALUE(start)) {
			printk(KERN_WARNING "cma: unable to reserve %lu for CMA"
			": %d\n", region->size >> 20, (int)region->start);
			region->start = start;
		} else if (region->start == 0)
			region->start = start;
	}
}

static int __init cma_regions_init(void)
{
	int i, ret = 0;
	struct cmaregion *region;
	struct cma *ctx;

	for (i = 0; i < cmainfo.nr_regions; i++) {
		region = &cmainfo.region[i];
		if (region->start) {
			ctx = cma_create(region->start, region->size, 0, false);
			if (IS_ERR(ctx) && !IS_ERR_VALUE(region->start)) {
				ret = PTR_ERR(ctx);
				printk(KERN_WARNING
					"cma: cma_create(%p, %p) failed: %d\n",
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
device_initcall(cma_regions_init);
