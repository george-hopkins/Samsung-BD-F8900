#include <linux/fs.h>
#include <linux/hugetlb.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/mmzone.h>
#include <linux/proc_fs.h>
#include <linux/quicklist.h>
#include <linux/seq_file.h>
#include <linux/swap.h>
#include <linux/vmstat.h>
#include <asm/atomic.h>
#include <asm/page.h>
#include <asm/pgtable.h>
#include "internal.h"

void __attribute__((weak)) arch_report_meminfo(struct seq_file *m)
{
}

extern unsigned long CMA_free_pages(void);
extern unsigned long CMA_free_isolated_pages(void);

static int meminfo_proc_show(struct seq_file *m, void *v)
{
	struct sysinfo i;
	unsigned long committed;
	unsigned long allowed;
	struct vmalloc_info vmi;
	long cached;
	unsigned long pages[NR_LRU_LISTS];
	int lru;
#ifdef CONFIG_VMALLOCUSED_PLUS
       struct vmalloc_usedinfo sVmallocUsed;
       memset(&sVmallocUsed,0,sizeof(struct vmalloc_usedinfo));
       get_vmallocused(&sVmallocUsed);
#endif


/*
 * display in kilobytes.
 */
#define K(x) ((x) << (PAGE_SHIFT - 10))
	si_meminfo(&i);
	si_swapinfo(&i);
	committed = percpu_counter_read_positive(&vm_committed_as);
	allowed = ((totalram_pages - hugetlb_total_pages())
		* sysctl_overcommit_ratio / 100) + total_swap_pages;

	cached = global_page_state(NR_FILE_PAGES) -
			total_swapcache_pages - i.bufferram;
	if (cached < 0)
		cached = 0;

	get_vmalloc_info(&vmi);

	for (lru = LRU_BASE; lru < NR_LRU_LISTS; lru++)
		pages[lru] = global_page_state(NR_LRU_BASE + lru);

	/*
	 * Tagged format, for easy grepping and expansion.
	 */
	seq_printf(m,
		"MemTotal:       %8lu kB\n"
		"MemFree:        %8lu kB\n"
		"Buffers:        %8lu kB\n"
		"Cached:         %8lu kB\n"
		"SwapCached:     %8lu kB\n"
		"Active:         %8lu kB\n"
		"Inactive:       %8lu kB\n"
		"Active(anon):   %8lu kB\n"
		"Inactive(anon): %8lu kB\n"
		"Active(file):   %8lu kB\n"
		"Inactive(file): %8lu kB\n"
		"Unevictable:    %8lu kB\n"
		"Mlocked:        %8lu kB\n"
#ifdef CONFIG_HIGHMEM
		"HighTotal:      %8lu kB\n"
		"HighFree:       %8lu kB\n"
		"LowTotal:       %8lu kB\n"
		"LowFree:        %8lu kB\n"
#endif
#ifndef CONFIG_MMU
		"MmapCopy:       %8lu kB\n"
#endif
		"SwapTotal:      %8lu kB\n"
		"SwapFree:       %8lu kB\n"
		"Dirty:          %8lu kB\n"
		"Writeback:      %8lu kB\n"
		"AnonPages:      %8lu kB\n"
		"Mapped:         %8lu kB\n"
		"Shmem:          %8lu kB\n"
		"Slab:           %8lu kB\n"
		"SReclaimable:   %8lu kB\n"
		"SUnreclaim:     %8lu kB\n"
		"KernelStack:    %8lu kB\n"
		"PageTables:     %8lu kB\n"
#ifdef CONFIG_QUICKLIST
		"Quicklists:     %8lu kB\n"
#endif
		"NFS_Unstable:   %8lu kB\n"
		"Bounce:         %8lu kB\n"
		"WritebackTmp:   %8lu kB\n"
		"CommitLimit:    %8lu kB\n"
		"Committed_AS:   %8lu kB\n"
		"VmallocTotal:   %8lu kB\n"
#ifdef CONFIG_VMALLOCUSED_PLUS
               "VmallocUsed:    %8lu kB [%8lu kB ioremap, %8lu kB vmalloc, %8lu kB vmap, %8lu kB usermap, %8lu kB vpages]\n"
#else
		"VmallocUsed:    %8lu kB\n"
#endif
		"VmallocChunk:   %8lu kB\n"
#ifdef CONFIG_MEMORY_FAILURE
		"HardwareCorrupted: %5lu kB\n"
#endif
#ifdef CONFIG_TRANSPARENT_HUGEPAGE
		"AnonHugePages:  %8lu kB\n"
#endif
		,
		K(i.totalram),
		K(i.freeram),
		K(i.bufferram),
		K(cached),
		K(total_swapcache_pages),
		K(pages[LRU_ACTIVE_ANON]   + pages[LRU_ACTIVE_FILE]),
		K(pages[LRU_INACTIVE_ANON] + pages[LRU_INACTIVE_FILE]),
		K(pages[LRU_ACTIVE_ANON]),
		K(pages[LRU_INACTIVE_ANON]),
		K(pages[LRU_ACTIVE_FILE]),
		K(pages[LRU_INACTIVE_FILE]),
		K(pages[LRU_UNEVICTABLE]),
		K(global_page_state(NR_MLOCK)),
#ifdef CONFIG_HIGHMEM
		K(i.totalhigh),
		K(i.freehigh),
		K(i.totalram-i.totalhigh),
		K(i.freeram-i.freehigh),
#endif
#ifndef CONFIG_MMU
		K((unsigned long) atomic_long_read(&mmap_pages_allocated)),
#endif
		K(i.totalswap),
		K(i.freeswap),
		K(global_page_state(NR_FILE_DIRTY)),
		K(global_page_state(NR_WRITEBACK)),
#ifdef CONFIG_TRANSPARENT_HUGEPAGE
		K(global_page_state(NR_ANON_PAGES)
		  + global_page_state(NR_ANON_TRANSPARENT_HUGEPAGES) *
		  HPAGE_PMD_NR),
#else
		K(global_page_state(NR_ANON_PAGES)),
#endif
		K(global_page_state(NR_FILE_MAPPED)),
		K(global_page_state(NR_SHMEM)),
		K(global_page_state(NR_SLAB_RECLAIMABLE) +
				global_page_state(NR_SLAB_UNRECLAIMABLE)),
		K(global_page_state(NR_SLAB_RECLAIMABLE)),
		K(global_page_state(NR_SLAB_UNRECLAIMABLE)),
		global_page_state(NR_KERNEL_STACK) * THREAD_SIZE / 1024,
		K(global_page_state(NR_PAGETABLE)),
#ifdef CONFIG_QUICKLIST
		K(quicklist_total_size()),
#endif
		K(global_page_state(NR_UNSTABLE_NFS)),
		K(global_page_state(NR_BOUNCE)),
		K(global_page_state(NR_WRITEBACK_TEMP)),
		K(allowed),
		K(committed),
		(unsigned long)VMALLOC_TOTAL >> 10,
		vmi.used >> 10,
#ifdef CONFIG_VMALLOCUSED_PLUS
                sVmallocUsed.uIoremapSize >> 10,
                sVmallocUsed.uVmallocSize >> 10,
                sVmallocUsed.uVmapSize >> 10,
                sVmallocUsed.uUsermapSize >>10,
                sVmallocUsed.uVpagesSize >> 10,
#endif

		vmi.largest_chunk >> 10
#ifdef CONFIG_MEMORY_FAILURE
		,atomic_long_read(&mce_bad_pages) << (PAGE_SHIFT - 10)
#endif
#ifdef CONFIG_TRANSPARENT_HUGEPAGE
		,K(global_page_state(NR_ANON_TRANSPARENT_HUGEPAGES) *
		   HPAGE_PMD_NR)
#endif
		);

	hugetlb_report_meminfo(m);

	arch_report_meminfo(m);

#if defined(CONFIG_CMA) && defined(CONFIG_HDMA)
	seq_printf(m,
		"HDMAfree:       %8lu kB\n"
		"HDMAisolated:   %8lu kB\n",
		K(CMA_free_pages()),
		K(CMA_free_isolated_pages())
		);
#endif
	return 0;
#undef K
}

static int meminfo_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, meminfo_proc_show, NULL);
}

static const struct file_operations meminfo_proc_fops = {
	.open		= meminfo_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static int __init proc_meminfo_init(void)
{
	proc_create("meminfo", 0, NULL, &meminfo_proc_fops);
	return 0;
}
module_init(proc_meminfo_init);

#ifdef CONFIG_VD_MEMINFO
extern void dump_tasks_plus(const struct mem_cgroup *mem, struct seq_file *s);
static void show_kernel_memory(struct seq_file *m)
{
       struct sysinfo i;
       struct vmalloc_usedinfo sVmallocUsed;
       struct vmalloc_info vmi;
       long cached;

       si_meminfo(&i);

       cached = global_page_state(NR_FILE_PAGES) -
                       total_swapcache_pages - i.bufferram;
       if (cached < 0)
               cached = 0;

       memset(&sVmallocUsed,0,sizeof(struct vmalloc_usedinfo));
       get_vmallocused(&sVmallocUsed);

       get_vmalloc_info(&vmi);

#define K(x) ((x) << (PAGE_SHIFT - 10))
       seq_printf(m,"\nKERNEL MEMORY INFO\n");
       seq_printf(m,"=================\n");
       seq_printf(m,"Buffers\t\t\t: %7luK\n", K(i.bufferram));
       seq_printf(m,"Cached\t\t\t: %7luK\n", K(cached));
       seq_printf(m,"SwapCached\t\t: %7luK\n", K(total_swapcache_pages));
       seq_printf(m,"Anonymous Memory\t: %7luK\n", K(global_page_state(NR_ANON_PAGES)));
       seq_printf(m,"Slab\t\t\t: %7luK (Unreclaimable : %luK)\n",
       K(global_page_state(NR_SLAB_RECLAIMABLE) + global_page_state(NR_SLAB_UNRECLAIMABLE)),
       K(global_page_state(NR_SLAB_UNRECLAIMABLE)));
       seq_printf(m,"PageTable\t\t: %7luK\n", K(global_page_state(NR_PAGETABLE)));
       seq_printf(m,"VmallocUsed\t\t: %7luK (ioremap : %luK)\n", vmi.used >> 10, sVmallocUsed.uIoremapSize >> 10);
       seq_printf(m,"Kernel Stack\t\t: %7luK\n", global_page_state(NR_KERNEL_STACK)*(THREAD_SIZE/1024));
       seq_printf(m,"kernel Total\t\t: %7luK\n", K(i.bufferram) + K(cached)
       + K(total_swapcache_pages) + K(global_page_state(NR_ANON_PAGES))
       + K(global_page_state(NR_SLAB_RECLAIMABLE) + global_page_state(NR_SLAB_UNRECLAIMABLE))
       + K(global_page_state(NR_PAGETABLE)) + ((vmi.used >>10) - (sVmallocUsed.uIoremapSize >> 10))
       + global_page_state(NR_KERNEL_STACK)*(THREAD_SIZE/1024));

       seq_printf(m,"MemTotal\t\t: %7luK\n", K(i.totalram));
       seq_printf(m,"MemTotal-MemFree\t: %7luK\n", K(i.totalram-i.freeram));
       seq_printf(m,"Unevictable\t\t: %7luK\n", K(global_page_state(NR_UNEVICTABLE)));
       seq_printf(m,"Unmapped PageCache\t: %7luK\n", K(cached-global_page_state(NR_FILE_MAPPED)));
#undef K
}

static int vd_meminfo_proc_show(struct seq_file *m, void *v)
{
       dump_tasks_plus(NULL, m);
       show_kernel_memory(m);

       return 0;
}

static int vd_meminfo_proc_open(struct inode *inode, struct file *file)
{
       return single_open(file, vd_meminfo_proc_show, NULL);
}

static const struct file_operations vd_meminfo_proc_fops = {
       .open           = vd_meminfo_proc_open,
       .read           = seq_read,
       .llseek         = seq_lseek,
       .release        = single_release,
};

static int __init proc_vd_meminfo_init(void)
{
       proc_create("vd_meminfo", 0, NULL, &vd_meminfo_proc_fops);
       return 0;
}
module_init(proc_vd_meminfo_init);
#endif

