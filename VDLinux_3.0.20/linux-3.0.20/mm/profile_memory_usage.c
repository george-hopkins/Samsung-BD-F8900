#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/sysctl.h>
#include <linux/memcontrol.h>
#include <linux/swap.h>
#include <linux/proc_fs.h>
#include "../fs/proc/internal.h"

struct mem_size_stats {
	struct vm_area_struct *vma;
	unsigned long resident;
	unsigned long shared_clean;
	unsigned long shared_dirty;
	unsigned long private_clean;
	unsigned long private_dirty;
	unsigned long referenced;
	unsigned long anonymous;
	unsigned long anonymous_thp;
	unsigned long swap;
	u64 pss;
};

struct smap_mem
{
	unsigned long total;
	unsigned long vmag[VMAG_CNT];
};

extern int smaps_pte_range(pmd_t *pmd, unsigned long addr, unsigned long end, struct mm_walk *walk);
#define PSS_SHIFT 12
#define K(x) ((x) << (PAGE_SHIFT-10))

/* A global variable is a bit ugly, but it keeps the code simple */
int sysctl_profile_memory_usage;
static int g_start_flag;
static struct timer_list my_mod_timer; 

static unsigned long g_start_pss;
static unsigned long g_start_kernel_mem;

static unsigned long g_stop_pss;
static unsigned long g_stop_kernel_mem;

static unsigned long g_max_pss;
static unsigned long g_max_kernel_mem;

static unsigned long g_min_pss;
static unsigned long g_min_kernel_mem;

static unsigned long get_kernel_mem(bool print)
{
	struct vmalloc_usedinfo sVmallocUsed;
	struct vmalloc_info vmi;
	unsigned long total;

	memset(&sVmallocUsed,0,sizeof(struct vmalloc_usedinfo));
	memset(&vmi,0,sizeof(struct vmalloc_info));

	get_vmallocused(&sVmallocUsed);
	get_vmalloc_info(&vmi);

	//Caculate kernel memory usage.
	//Unreclaimable Slab + PageTable + Vmalloc(except ioremap) + Kernel Stack
	total = K(global_page_state(NR_SLAB_UNRECLAIMABLE))
	+ K(global_page_state(NR_PAGETABLE)) + ((vmi.used >>10) 
	- (sVmallocUsed.uIoremapSize >> 10))
	+ global_page_state(NR_KERNEL_STACK)*(THREAD_SIZE/1024);

	if (print) {
		printk(" [Kernel]\n");
		printk(" Unreclaimable slab	:  %5lu  kbyte\n", 
				K(global_page_state(NR_SLAB_UNRECLAIMABLE)));
		printk(" PageTable		:  %5lu  kbyte\n", K(global_page_state(NR_PAGETABLE)));
		printk(" Vmalloc(except ioremap):  %5lu  kbyte\n", 
				((vmi.used >>10) - (sVmallocUsed.uIoremapSize >> 10)));
		printk(" PageTable		:  %5lu  kbyte\n", K(global_page_state(NR_PAGETABLE)));
		printk(" Kernel Stack		:  %5lu  kbyte\n", 
				global_page_state(NR_KERNEL_STACK)*(THREAD_SIZE/1024));
	}

	return total;
}

static int get_user_pss_rss(struct task_struct* p, struct smap_mem* pss, struct smap_mem* rss)
{
	int idx = 0;
	struct mm_struct *mm = NULL;
	struct vm_area_struct *vma = NULL;
	struct mem_size_stats mss;
	struct mm_walk smaps_walk = {
		.pmd_entry = smaps_pte_range,
		.private = &mss,
	};

	if (!thread_group_leader(p))
		return 0;

	task_lock(p);
	mm = p->mm;
	if (!mm) {
		task_unlock(p);
		return 0;
	}
	
	smaps_walk.mm = mm;
	vma = mm->mmap;
	if (!vma) {
		task_unlock(p);
		return 0;
	}

	/* Walk all VMAs to update various counters. */
	while (vma) {
		/* Ignore the huge TLB pages. */
		if (vma->vm_mm && !(vma->vm_flags & VM_HUGETLB)) {
			memset(&mss, 0, sizeof(struct mem_size_stats));
			mss.vma = vma;

			/* We need to release this spin_lock otherwise this leads
			 * to scheduling while atomic bugs by virtue of 
			 * walk_page_range() -> smaps_pte_range() -> _cond_resched() */
			task_unlock(p);
			walk_page_range(vma->vm_start, vma->vm_end, &smaps_walk);
			task_lock(p);

			pss->total += (unsigned long)(mss.pss >> (10 + PSS_SHIFT));
			rss->total += (unsigned long)(mss.resident >> 10);

			idx = get_group_idx(vma);

			pss->vmag[idx] += (unsigned long)(mss.pss >> (10 + PSS_SHIFT));
			rss->vmag[idx] += (unsigned long)(mss.resident >> 10);
		}
		vma = vma->vm_next;
	} 

	task_unlock(p);

	return 1;
}

static unsigned long get_pss_from_tasks(bool print)
{
	struct task_struct *g = NULL, *p = NULL;
	struct smap_mem pss;
	struct smap_mem rss;
	unsigned long sum_of_pss = 0;
	unsigned long kernel_mem;
	static int toggle = 0;

	toggle ^= 1;

	if(print && toggle) {
		printk("===============================\n");
		printk("      Current Memory Usage\n");
		printk("===============================\n");
	}

	do_each_thread(g, p) {
		memset(&pss, 0, sizeof(struct smap_mem));
		memset(&rss, 0, sizeof(struct smap_mem));

		if(get_user_pss_rss(p, &pss, &rss) == 0)
			continue;

		sum_of_pss += pss.total;

		if(print && toggle) {
			printk(" [Process:%s]\n",p->comm);
			printk("              Pss    Rss\n");
			printk(" Total   :  %5lu  %5lu  kbyte\n",pss.total, rss.total);
			printk(" Code    :  %5lu  %5lu  kbyte\n",pss.vmag[0], rss.vmag[0]);
			printk(" Data    :  %5lu  %5lu  kbyte\n",pss.vmag[1], rss.vmag[1]);
			printk(" LibCode :  %5lu  %5lu  kbyte\n",pss.vmag[2], rss.vmag[2]);
			printk(" LibData :  %5lu  %5lu  kbyte\n",pss.vmag[3], rss.vmag[3]);
			printk(" Stack   :  %5lu  %5lu  kbyte\n",pss.vmag[4], rss.vmag[4]);
			printk(" Other   :  %5lu  %5lu  kbyte\n\n",pss.vmag[5], rss.vmag[5]);
		}
	} while_each_thread(g, p);

	if(print && toggle) {
		printk(" * User(pss) :  %5lu kbyte\n", sum_of_pss);
		kernel_mem = get_kernel_mem(true);
		printk(" * Kernel    :  %5lu kbyte\n", kernel_mem);
		printk("===============================\n");
	}

	return sum_of_pss;
}

static void profile(unsigned long key)
{
	unsigned long current_pss = 0;
	unsigned long current_kernel_mem = 0;

	if(sysctl_profile_memory_usage == 2)
		current_pss = get_pss_from_tasks(true);
	else if(sysctl_profile_memory_usage == 1)
		current_pss = get_pss_from_tasks(false);

	current_kernel_mem = get_kernel_mem(false);

	//Set high,low peak values
	if(g_max_pss < current_pss)
		g_max_pss = current_pss;
	if(g_min_pss > current_pss)
		g_min_pss = current_pss;

	if(g_max_kernel_mem < current_kernel_mem)
		g_max_kernel_mem = current_kernel_mem;
	if(g_min_kernel_mem > current_kernel_mem)
		g_min_kernel_mem = current_kernel_mem;

	//Reinitialize kernel timer
	if(key == 0x6789) {
		init_timer(&my_mod_timer);
		my_mod_timer.expires = (jiffies + HZ/2);
		my_mod_timer.data = 0x6789;
		my_mod_timer.function = profile; 

		add_timer(&my_mod_timer);
	}
}

static void profiling_start(void)
{
	if(g_start_flag == 1) {
		printk("Profiling already started.\n");
		return;
	}
	printk("Profiling Started...\n\n");
	g_start_flag = 1;

	//Initialize global variables
	g_start_pss = g_max_pss = g_min_pss = get_pss_from_tasks(true);
	g_start_kernel_mem = g_max_kernel_mem = g_min_kernel_mem = get_kernel_mem(false);

	//Initialize kernel timer
	init_timer(&my_mod_timer);
	my_mod_timer.expires = (jiffies + HZ/2);
	my_mod_timer.data = 0x6789;
	my_mod_timer.function = profile;
	add_timer(&my_mod_timer);
}

static void profiling_stop(void)
{
	if(g_start_flag == 0) {
		printk("Profiling already stopped.\n");
		printk("Please start profiling first.\n");
		return;
	}
	printk("Profiling Stopped.\n\n");
	g_start_flag = 0;

	g_stop_pss = get_pss_from_tasks(true);
	g_stop_kernel_mem = get_kernel_mem(false);

	//Set high,low peak values
	if(g_max_pss < g_stop_pss)
		g_max_pss = g_stop_pss;
	if(g_min_pss > g_stop_pss)
		g_min_pss = g_stop_pss;

	if(g_max_kernel_mem < g_stop_kernel_mem)
		g_max_kernel_mem = g_stop_kernel_mem;
	if(g_min_kernel_mem > g_stop_kernel_mem)
		g_min_kernel_mem = g_stop_kernel_mem;

	printk("\n\n==========================\n");
	printk("       Final Report\n");
	printk("==========================\n");
	printk(" * START *\n");
	printk(" USER(PSS) : %lu kbyte\n", g_start_pss);
	printk(" KERNEL    : %lu kbyte\n\n", g_start_kernel_mem);
	printk(" * CURRENT *\n");
	printk(" USER(PSS) : %lu kbyte\n", g_stop_pss);
	printk(" KERNEL    : %lu kbyte\n\n", g_stop_kernel_mem);
	printk(" * MAX *\n");
	printk(" USER(PSS) : %lu kbyte\n", g_max_pss);
	printk(" KERNEL    : %lu kbyte\n\n", g_max_kernel_mem);
	printk(" * MIN *\n");
	printk(" USER(PSS) : %lu kbyte\n", g_min_pss);
	printk(" KERNEL    : %lu kbyte\n", g_min_kernel_mem);
	printk("==========================\n");

	del_timer_sync(&my_mod_timer);
}

int profile_memory_usage_sysctl_handler(ctl_table *table, int write,
	void __user *buffer, size_t *length, loff_t *ppos)
{
	proc_dointvec_minmax(table, write, buffer, length, ppos);
	if (write) {
		if (sysctl_profile_memory_usage == 0)
			profiling_stop();
		else if (sysctl_profile_memory_usage == 1)
			profiling_start();
		else if (sysctl_profile_memory_usage == 2)
			profiling_start();
		else {
			printk("\nWrong! Input value should be 0 or 1 or 2.\n");
			printk(" 0 : stop to profile\n");
			printk(" 1 : start to profile\n");
			printk(" 2 : start to profile with printing info\n");
		}
	}
	return 0;
}
