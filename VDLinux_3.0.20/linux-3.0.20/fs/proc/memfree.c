#include <linux/fs.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/mm.h>
#include <linux/vmstat.h>
#include <linux/blkdev.h>

#define K(x) ((x) << (PAGE_SHIFT - 10))
long buff_for_perf_kb = 30720;
static int memfree_proc_show(struct seq_file *m, void *v)
{
       seq_printf(m, "%lu\n",
                       K(global_page_state(NR_FREE_PAGES)
                       + global_page_state(NR_FILE_PAGES)
                       - global_page_state(NR_SHMEM))-buff_for_perf_kb);

       return 0;
}

static int memfree_proc_open(struct inode *inode, struct file *file)
{
       return single_open(file, memfree_proc_show, NULL);
}

static const struct file_operations memfree_proc_fops = {
       .open           = memfree_proc_open,
       .read           = seq_read,
       .llseek         = seq_lseek,
       .release        = single_release,
};

static int __init proc_memfree_init(void)
{
       proc_create("vd_memfree", 0, NULL, &memfree_proc_fops);
       return 0;
}
module_init(proc_memfree_init);
