#ifndef __LINUX_PAGEISOLATION_H
#define __LINUX_PAGEISOLATION_H

/*
 * Changes migrate type in [start_pfn, end_pfn) to be MIGRATE_ISOLATE.
 * If specified range includes migrate types other than MOVABLE or CMA,
 * this will fail with -EBUSY.
 *
 * For isolating all pages in the range finally, the caller have to
 * free all pages in the range. test_page_isolated() can be used for
 * test it.
 */
#ifdef CONFIG_CMA
int __start_isolate_page_range(unsigned long start_pfn, unsigned long end_pfn,
			       unsigned migratetype);

static inline int
start_isolate_page_range(unsigned long start_pfn, unsigned long end_pfn)
{
	return __start_isolate_page_range(start_pfn, end_pfn, MIGRATE_MOVABLE);
}
#else
extern int
start_isolate_page_range(unsigned long start_pfn, unsigned long end_pfn);
#endif

/*
 * Changes MIGRATE_ISOLATE to MIGRATE_MOVABLE.
 * target range is [start_pfn, end_pfn)
 */
#ifdef CONFIG_CMA
int __undo_isolate_page_range(unsigned long start_pfn, unsigned long end_pfn,
			      unsigned migratetype);
static inline int
undo_isolate_page_range(unsigned long start_pfn, unsigned long end_pfn)
{
	return __undo_isolate_page_range(start_pfn, end_pfn, MIGRATE_MOVABLE);
}
#else
extern int
undo_isolate_page_range(unsigned long start_pfn, unsigned long end_pfn);
#endif

/*
 * test all pages in [start_pfn, end_pfn)are isolated or not.
 */
#ifdef CONFIG_CMA
int test_pages_isolated(unsigned long start_pfn, unsigned long end_pfn);
#else
extern int
test_pages_isolated(unsigned long start_pfn, unsigned long end_pfn);
#endif

/*
 * For migration.
 */
#ifdef CONFIG_CMA
int set_migratetype_isolate(struct page *page);
void __unset_migratetype_isolate(struct page *page, unsigned migratetype);
static inline void unset_migratetype_isolate(struct page *page)
{
	__unset_migratetype_isolate(page, MIGRATE_MOVABLE);
}
extern unsigned long alloc_contig_freed_pages(unsigned long start,
					      unsigned long end, gfp_t flag);
extern int alloc_contig_range(unsigned long start, unsigned long end,
			      gfp_t flags, unsigned migratetype);
extern void free_contig_pages(struct page *page, int nr_pages);
int test_pages_in_a_zone(unsigned long start_pfn, unsigned long end_pfn);
unsigned long scan_lru_pages(unsigned long start, unsigned long end);
int do_migrate_range(unsigned long start_pfn, unsigned long end_pfn);
#else
/*
 * Internal functions. Changes pageblock's migrate type.
 */
extern int set_migratetype_isolate(struct page *page);
extern void unset_migratetype_isolate(struct page *page);
#endif

#endif
