#ifndef __LINUX_CMA_H
#define __LINUX_CMA_H

/*
 * Contiguous Memory Allocator
 * Copyright (c) 2010 by Samsung Electronics.
 * Written by Michal Nazarewicz (m.nazarewicz@samsung.com)
 */

/*
 * Contiguous Memory Allocator
 *
 *   The Contiguous Memory Allocator (CMA) makes it possible for
 *   device drivers to allocate big contiguous chunks of memory after
 *   the system has booted.
 *
 *   It requires some machine- and/or platform-specific initialisation
 *   code which prepares memory ranges to be used with CMA and later,
 *   device drivers can allocate memory from those ranges.
 *
 * Why is it needed?
 *
 *   Various devices on embedded systems have no scatter-getter and/or
 *   IO map support and require contiguous blocks of memory to
 *   operate.  They include devices such as cameras, hardware video
 *   coders, etc.
 *
 *   Such devices often require big memory buffers (a full HD frame
 *   is, for instance, more then 2 mega pixels large, i.e. more than 6
 *   MB of memory), which makes mechanisms such as kmalloc() or
 *   alloc_page() ineffective.
 *
 *   At the same time, a solution where a big memory region is
 *   reserved for a device is suboptimal since often more memory is
 *   reserved then strictly required and, moreover, the memory is
 *   inaccessible to page system even if device drivers don't use it.
 *
 *   CMA tries to solve this issue by operating on memory regions
 *   where only movable pages can be allocated from.  This way, kernel
 *   can use the memory for pagecache and when device driver requests
 *   it, allocated pages can be migrated.
 *
 * Driver usage
 *
 *   For device driver to use CMA it needs to have a pointer to a CMA
 *   context represented by a struct cma (which is an opaque data
 *   type).
 *
 *   Once such pointer is obtained, device driver may allocate
 *   contiguous memory chunk using the following function:
 *
 *     cm_alloc()
 *
 *   This function returns a pointer to struct cm (another opaque data
 *   type) which represent a contiguous memory chunk.  This pointer
 *   may be used with the following functions:
 *
 *     cm_free()    -- frees allocated contiguous memory
 *     cm_pin()     -- pins memory
 *     cm_unpin()   -- unpins memory
 *
 *   See the respective functions for more information.
 *
 * Platform/machine integration
 *
 *   For device drivers to be able to use CMA platform or machine
 *   initialisation code must create a CMA context and pass it to
 *   device drivers.  The latter may be done by a global variable or
 *   a platform/machine specific function.  For the former CMA
 *   provides the following functions:
 *
 *     cma_init_migratetype()
 *     cma_reserve()
 *     cma_create()
 *
 *   The first one initialises a portion of reserved memory so that it
 *   can be used with CMA.  The second first tries to reserve memory
 *   (using memblock) and then initialise it.
 *
 *   The cma_reserve() function must be called when memblock is still
 *   operational and reserving memory with it is still possible.  On
 *   ARM platform the "reserve" machine callback is a perfect place to
 *   call it.
 *
 *   The last function creates a CMA context on a range of previously
 *   initialised memory addresses.  Because it uses kmalloc() it needs
 *   to be called after SLAB is initialised.
 */

#include <linux/ioctl.h>
#include <linux/types.h>
#include <linux/device.h>
#include <linux/rbtree.h>

#define CMA_MAGIC (('c' << 24) | ('M' << 16) | ('a' << 8) | 0x42)

/**
 * An information about area exportable to user space.
 * @magic:	must always be CMA_MAGIC.
 * @_pad:	padding (ignored).
 * @size:	size of the chunk to allocate.
 * @alignment:	desired alignment of the chunk (must be power of two or zero).
 * @start:	when ioctl() finishes this stores physical address of the chunk.
 */
struct cma_alloc_request {
	__u32 magic;
	__u32 _pad;
	__u32 size;
	__u32 alignment;
	__u32 start;
};

#define IOCTL_CMA_ALLOC     _IOWR('p', 0, struct cma_alloc_request)
#define IOCTL_CMA_GET_FREE    _IOR('p', 1, unsigned long)
#define IOCTL_CMA_GET_LARGEST _IOR('p', 2, unsigned long)
#define IOCTL_CMA_GET_SIZE    _IOR('p', 3, unsigned long)

/***************************** Kernel level API *****************************/

#if defined __KERNEL__ && defined CONFIG_CMA

#include <asm/page.h>
#include <linux/io.h>
#include <linux/mm.h>

#ifdef phys_to_pfn
/* nothing to do */
#elif defined __phys_to_pfn
#  define phys_to_pfn __phys_to_pfn
#else
#  warning correct phys_to_pfn implementation needed
static unsigned long phys_to_pfn(phys_addr_t phys)
{
	return page_to_pfn(virt_to_page(phys_to_virt(phys)));
}
#endif

struct cmaregion;

/* CMA context */
struct cma {
	int migratetype;
	struct gen_pool *pool;
	struct cmaregion *region;
	struct rb_root rbroot;
	unsigned int chunks;
};

/* Contiguous Memory chunk */
struct cm {
	struct rb_node rbnode;
	struct cma *cma;
	unsigned long phys, size;
	atomic_t pinned, mapped;
};

#define MAX_CMA_REGIONS 8

#define cma_regions_overlap(st1, sz1, st2, sz2)	\
	((st1 >= st2 && st1 < st2 + sz2) || \
	(st1 + sz1 > st2 && st1 + sz1 < st2 + sz2) || \
	(st2 >= st1 && st2 < st1 + sz1))

/* CMA region */
struct cmaregion {
	unsigned long start;
	unsigned long size;
	unsigned long free;
	struct cma *ctx;
	int id;
	struct device *dev;
};

/* CMA regions information */
struct cmainfo {
	int nr_regions;
	struct cmaregion region[MAX_CMA_REGIONS];
};
extern struct cmainfo cmainfo;

#ifdef CONFIG_MIGRATE_CMA

/**
 * cma_init_migratetype() - initialises range of physical memory to be used
 *		with CMA context.
 * @start:	start address of the memory range in bytes.
 * @size:	size of the memory range in bytes.
 *
 * The range must be MAX_ORDER_NR_PAGES aligned and it must have been
 * already reserved (eg. with memblock).
 *
 * The actual initialisation is deferred until subsys initcalls are
 * evaluated (unless this has already happened).
 *
 * Returns zero on success or negative error.
 */
int cma_init_migratetype(unsigned long start, unsigned long end);

#else

static inline int cma_init_migratetype(unsigned long start, unsigned long end)
{
	(void)start; (void)end;
	return -EOPNOTSUPP;
}

#endif

/**
 * cma_get_chunks_list() - returns array of pointers to cm struct items.
 * @cmactx:	pointer to struct cma which contains allocated CMA chunks
 *
 * It will use rb-tree to find all allocated chunks and return it in
 * ordered form. Returned value should be freed after usage.
 *
 * Returns ordered by start addres array of pointers to struct cm items
 */
struct cm **cma_get_chunks_list(struct cma *cmactx);

/**
 * cma_get_largest() - returns size of largest free contiguous memory chunk
 * @cmactx:	pointer to struct cma which contains allocated CMA chunks
 *
 * It will use rb-tree to find all allocated chunks and return largest free
 * memory chunk of CMA memory.
 *
 * Returns size of largest free CMA memory chunk
 */
unsigned long cma_get_largest(struct cma *cmactx);

/**
 * cma_reserve() - reserves memory.
 * @start:	start address of the memory range in bytes hint; if unsure
 *		pass zero.
 * @size:	size of the memory to reserve in bytes.
 * @alignment:	desired alignment in bytes (must be power of two or zero).
 * @init_migratetype:	whether to initialise pageblocks.
 *
 * It will use memblock to allocate memory.  If @init_migratetype is
 * true, the function will also call cma_init_migratetype() on
 * reserved region so that a non-private CMA context can be created on
 * given range.
 *
 * @start and @size will be aligned to PAGE_SIZE if @init_migratetype
 * is false or to (MAX_ORDER_NR_PAGES << PAGE_SHIFT) if
 * @init_migratetype is true.
 *
 * Returns reserved's area physical address or value that yields true
 * when checked with IS_ERR_VALUE().
 */
unsigned long cma_reserve(unsigned long start, unsigned long size,
			  unsigned long alignment, _Bool init_migratetype);

/**
 * cma_create() - creates a CMA context.
 * @start:	start address of the context in bytes.
 * @size:	size of the context in bytes.
 * @min_alignment:	minimal desired alignment or zero.
 * @private:	whether to create private context.
 *
 * The range must be page aligned.  Different contexts cannot overlap.
 *
 * Unless @private is true the memory range must either lay in
 * ZONE_MOVABLE or must have been initialised with
 * cma_init_migratetype() function.  If @private is true no
 * underlaying memory checking is done and during allocation no pages
 * migration will be performed - it is assumed that the memory is
 * reserved and only CMA manages it.
 *
 * @start and @size must be page and @min_alignment aligned.
 * @min_alignment specifies the minimal alignment that user will be
 * able to request through cm_alloc() function.  In most cases one
 * will probably pass zero as @min_alignment but if the CMA context
 * will be used only for, say, 1 MiB blocks passing 1 << 20 as
 * @min_alignment may increase performance and reduce memory usage
 * slightly.
 *
 * Because this function uses kmalloc() it must be called after SLAB
 * is initialised.  This in particular means that it cannot be called
 * just after cma_reserve() since the former needs to be run way
 * earlier.
 *
 * Returns pointer to CMA context or a pointer-error on error.
 */
struct cma *cma_create(unsigned long start, unsigned long size,
		       unsigned long min_alignment, _Bool private);

/**
 * cma_destroy() - destroys CMA context.
 * @cma:	context to destroy.
 */
void cma_destroy(struct cma *cma);

/**
 * cm_alloc() - allocates contiguous memory.
 * @cma:	CMA context to use.
 * @size:	desired chunk size in bytes (must be non-zero).
 * @alignent:	desired minimal alignment in bytes (must be power of two
 *		or zero).
 * @failed_pages:	number of additional pages necessary for successful
 *		migration.
 *
 * Returns pointer to structure representing contiguous memory or
 * a pointer-error on error.
 */
struct cm *cm_alloc(struct cma *cma, unsigned long size,
			unsigned long alignment, unsigned long *failed_pages);

/**
 * cm_free() - frees contiguous memory.
 * @cm:	contiguous memory to free.
 *
 * The contiguous memory must be not be pinned (see cma_pin()) and
 * must not be mapped to kernel space (cma_vmap()).
 */
void cm_free(struct cm *cm);

/**
 * cm_pin() - pins contiguous memory.
 * @cm: contiguous memory to pin.
 *
 * Pinning is required to obtain contiguous memory's physical address.
 * While memory is pinned the memory will remain valid it may change
 * if memory is unpinned and then pinned again.  This facility is
 * provided so that memory defragmentation can be implemented inside
 * CMA.
 *
 * Each call to cm_pin() must be accompanied by call to cm_unpin() and
 * the calls may be nested.
 *
 * Returns chunk's physical address or a value that yields true when
 * tested with IS_ERR_VALUE().
 */
unsigned long cm_pin(struct cm *cm);

/**
 * cm_unpin() - unpins contiguous memory.
 * @cm: contiguous memory to unpin.
 *
 * See cm_pin().
 */
void cm_unpin(struct cm *cm);

void cma_regions_reserve(void);
#endif

#endif
