
/* drivers/misc/lowmemorykiller.c
 *
 * The lowmemorykiller driver lets user-space specify a set of memory thresholds
 * where processes with a range of oom_adj values will get killed. Specify the
 * minimum oom_adj values in /proc/sys/vm/lmk/adj and the
 * number of free pages in /proc/sys/vm/lmk/minfree. Both
 * files take a comma separated list of numbers in ascending order.
 *
 * For example, write "0 8" to /proc/sys/vm/lmk/adj and
 * "1024 4096" to /proc/sys/vm/lmk/minfree to kill
 * processes with a oom_adj value of 8 or higher when the free memory drops
 * below 4096 pages and kill processes with a oom_adj value of 0 or higher
 * when the free memory drops below 1024 pages.
 *
 * If you write "2" to /proc/sys/vm/lmk/mode then LMK will automatically define
 * minfree parameter proportionally to min_free_kbytes value and
 * /proc/sys/vm/lmk/minfree_ratio values.
 *
 * The driver considers memory used for caches to be free, but if a large
 * percentage of the cached memory is locked this can be very inaccurate
 * and processes may not get killed until the normal oom killer is triggered.
 *
 * Copyright (C) 2007-2008 Google, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/oom.h>
#include <linux/sched.h>
#include <linux/mutex.h>
#include <linux/sysctl.h>
#include <linux/pid.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#define ARRAYSIZE 6

#include <linux/miscdevice.h>
#include <linux/uaccess.h>

#include "lowmemorykiller.h"


static DEFINE_MUTEX(m_lock);

/*
 * PID number of "task manager"  which should handle info about killed proceses
 */
static int taskManager;

static DEFINE_MUTEX(lowmem_lock); /* to protect from changing parameters during
run */

static uint32_t lowmem_debug_level = 1;
static uint32_t lowmem_enabled = 0; /* ability to turn on/off from user space:
				     * 0 - LMK is disabled
				     * 1 - LMK is enabled */
static uint32_t lowmem_mode = 2; /* change the way LMK deals with parameters:
				  * 0(default) - adjust minfree param only at
				  * init using min_free_kbytes value, if
				  * min_free_kbytes value changes, print dmesg
				  * message with appropriate params
				  * 1 - always adjust minfree param if
				  * min_free_kbytes value changes
				  * 2 - use user defined minfree_ratio and
				  * adj_ratio params to automatically change
				  * minfree if min_free_kbytes value changes
				  * Mode must be 2 at kernel start to
				  * automatically define new minfree and
				  * minfree_ratio values using watermarks*/

static int lowmem_adj[ARRAYSIZE] = {
	0,
	1,
	6,
	12,
};
static int lowmem_adj_size = 4;
static size_t lowmem_minfree[ARRAYSIZE] = {
	764, /* pages_min */
	802,
	955, /* pages_low */
	1146, /* pages_high */
};
static int lowmem_minfree_size = 4;
static long previous_min_pages;  /* pages low watermark, will be defined
at kernel start */
static long previous_high_pages; /* pages high watermark, will be defined
at kernel start */
static long previous_used_high_pages;
static int lowmem_adj_ratio[ARRAYSIZE] = {
	0,
	1,
	6,
	12,
};
static int lowmem_adj_ratio_size = 4;

static size_t lowmem_minfree_ratio[ARRAYSIZE] = { /* will be redefined using
watermarks at kernel start */
	0, /* pages_min */
	0,
	75, /* pages_low = (pages_min + pages_high)/2 */
	100, /* pages_high */
};
static int lowmem_minfree_ratio_size = 4;

#define lowmem_print(level, x...)			\
	do {						\
		if (lowmem_debug_level >= (level))	\
			printk(x);			\
	} while (0)

static int lowmem_shrink(struct shrinker *s, struct shrink_control *sc);

static struct shrinker lowmem_shrinker = {
	.shrink = lowmem_shrink,
	.seeks = DEFAULT_SEEKS * 16
};

static int zero;
static int __maybe_unused one = 1;
static int __maybe_unused two = 2;
static int __maybe_unused five = 5;
static int max = INT_MAX;
static int min_adj = OOM_ADJUST_MIN;
static int max_adj = OOM_ADJUST_MAX;

#define LMK_PR_PRFX "lowmem: "
void lowmem_print_params(void)
{
	unsigned int i;

	pr_info(LMK_PR_PRFX "enabled: %s\n", lowmem_enabled? "yes": "no");
	pr_info(LMK_PR_PRFX "mode: %u\n", lowmem_mode);
	pr_info(LMK_PR_PRFX "          adj:");
	for (i = 0; i < lowmem_adj_size; ++i)
		printk(KERN_INFO "%6u ", lowmem_adj[i]);
	pr_info(LMK_PR_PRFX "    adj ratio:");
	for (i = 0; i < lowmem_adj_ratio_size; ++i)
		printk(KERN_INFO "%6u ", lowmem_adj_ratio[i]);
	pr_info(LMK_PR_PRFX "      minfree:");
	for (i = 0; i < lowmem_minfree_size; ++i)
		printk(KERN_INFO "%6u ", lowmem_minfree[i]);
	pr_info(LMK_PR_PRFX "minfree ratio:");
	for (i = 0; i < lowmem_minfree_ratio_size; ++i)
		printk(KERN_INFO "%6u ", lowmem_minfree_ratio[i]);
}

int get_parameters_count(void __user *buffer, size_t *length)
{
	int count = 0;
	int i;
	char *str = buffer;
	for (i = 0; i < *length - 2; i++) {
		if ((str[i] == ' ') && (str[i+1] != ' ')) {
			count = count + 1;
		}
	}
	if (str[0] != ' ') {
		count = count + 1;
	}
	if (count > ARRAYSIZE)	{
		count = ARRAYSIZE;
	}
	return count;
}

/* lmk parameters (files) */

int lowmem_mode_sysctl_handler(ctl_table *table, int write,
	void __user *buffer, size_t *length, loff_t *ppos)
{
	int ret;
	if (write) {
		mutex_lock(&lowmem_lock);
		ret = proc_dointvec_minmax(table, write, buffer, length, ppos);
		mutex_unlock(&lowmem_lock);
		if (!ret) {
			calculate_lowmemkiller_params(previous_min_pages,
				previous_high_pages);
		}
	} else {
		ret = proc_dointvec_minmax(table, write, buffer, length, ppos);
	}
	return (struct task_struct*)ret;
}

int lowmem_minfree_sysctl_handler(ctl_table *table, int write,
	void __user *buffer, size_t *length, loff_t *ppos)
{
	int i, ret;
	if (write) {
		mutex_lock(&lowmem_lock);
		ret = proc_dointvec_minmax(table, write, buffer, length, ppos);
		if (!ret) {
			lowmem_minfree_size = get_parameters_count(buffer, length);
			for (i = lowmem_minfree_size; i < ARRAYSIZE; i++) {
				lowmem_minfree[i] = 0;
			}
		}
		mutex_unlock(&lowmem_lock);
	} else {
		ret = proc_dointvec_minmax(table, write, buffer, length, ppos);
	}
	return ret;
}

int lowmem_adj_sysctl_handler(ctl_table *table, int write,
	void __user *buffer, size_t *length, loff_t *ppos)
{
	int i, ret;
	if (write) {
		mutex_lock(&lowmem_lock);
		ret = proc_dointvec_minmax(table, write, buffer, length, ppos);
		if (!ret) {
			lowmem_adj_size = get_parameters_count(buffer, length);
			for (i = lowmem_adj_size; i < ARRAYSIZE; i++) {
				lowmem_adj[i] = 0;
			}
		}
		mutex_unlock(&lowmem_lock);
	} else {
		ret = proc_dointvec_minmax(table, write, buffer, length, ppos);
	}
	return ret;
}

int lowmem_minfree_ratio_sysctl_handler(ctl_table *table, int write,
	void __user *buffer, size_t *length, loff_t *ppos)
{
	int i, ret;
	if (write) {
		mutex_lock(&lowmem_lock);
		ret = proc_dointvec_minmax(table, write, buffer, length, ppos);
		if (!ret) {
			lowmem_minfree_ratio_size = get_parameters_count(buffer, length);
			for (i = lowmem_minfree_ratio_size; i < ARRAYSIZE; i++) {
				lowmem_minfree_ratio[i] = 0;
			}
		}
		mutex_unlock(&lowmem_lock);
		if ((lowmem_mode == 2) && !ret){
			calculate_lowmemkiller_params(previous_min_pages,
				previous_high_pages);
		}
	} else {
		ret = proc_dointvec_minmax(table, write, buffer, length, ppos);
	}
	return ret;
}

int lowmem_enabled_sysctl_handler(ctl_table *table, int write,
	void __user *buffer, size_t *length, loff_t *ppos)
{
	int ret;
	uint32_t old_enabled = lowmem_enabled;
	if (write) {
		mutex_lock(&lowmem_lock);
		ret = proc_dointvec_minmax(table, write, buffer, length, ppos);
		if (!ret)
			old_enabled -= lowmem_enabled;
		mutex_unlock(&lowmem_lock);
		if (!ret)
			switch (old_enabled) {
			case 1: /* lowmem_enabled changed from 0 to 1 */
				unregister_shrinker(&lowmem_shrinker);
				break;
			case -1: /* lowmem_enabled changed from 1 to 0 */
				register_shrinker(&lowmem_shrinker);
				break;
			}
	} else {
		ret = proc_dointvec_minmax(table, write, buffer, length, ppos);
	}
	return ret;
}

int lowmem_adj_ratio_sysctl_handler(ctl_table *table, int write,
	void __user *buffer, size_t *length, loff_t *ppos)
{
	int i, ret;
	if (write) {
		mutex_lock(&lowmem_lock);
		ret = proc_dointvec_minmax(table, write, buffer, length, ppos);
		if (!ret) {
			lowmem_adj_ratio_size = get_parameters_count(buffer, length);
			for (i = lowmem_adj_ratio_size; i < ARRAYSIZE; i++) {
				lowmem_adj_ratio[i] = 0;
			}
		}
		mutex_unlock(&lowmem_lock);
		if ((lowmem_mode == 2) && !ret) {
			calculate_lowmemkiller_params(previous_min_pages,
				previous_high_pages);
		}
	} else {
		ret = proc_dointvec_minmax(table, write, buffer, length, ppos);
	}
	return ret;
}

static unsigned long lowmem_deathpending_timeout;


/*
 * Change LowmemoryKiller Parameters when min_free_kbytes param is changed
 */
void calculate_lowmemkiller_params(long min_pages, long high_pages)
{
	int array_size, i;
	long minfree;
	BUG_ON(min_pages <= 0);
	BUG_ON(high_pages <= 0);
	lowmem_print(1, "[MULTITASKING] LMK - low_wmark is %ld, high_wmark is %ld \n",
		min_pages, high_pages);
	mutex_lock(&lowmem_lock);
	switch (lowmem_mode) {
	case 1:
			/* change params proportionally */
			array_size = ARRAYSIZE;
			if (lowmem_adj_size < array_size)
				array_size = lowmem_adj_size;
			if (lowmem_minfree_size < array_size)
				array_size = lowmem_minfree_size;
			lowmem_print(1, "[MULTITASKING] LMK - lowmem minfree param changed to: ");
			for (i = 0; i < array_size; i++) {
				lowmem_minfree[i] =
					lowmem_minfree[i] * high_pages /
					previous_used_high_pages;
				if (lowmem_minfree[i] < min_pages)
					lowmem_minfree[i] = min_pages+1;
				else
					if (lowmem_minfree[i] > high_pages)
						lowmem_minfree[i] = high_pages;
				lowmem_print(1, " %ld,", (long) (lowmem_minfree[i]));
			}
			lowmem_print(1, "\n");
			previous_used_high_pages = high_pages;
			break;
	case 2:
			/* change params using user-defined ratio values */
			array_size = ARRAYSIZE;
			if (lowmem_adj_ratio_size < array_size)
				array_size = lowmem_adj_ratio_size;
			if (lowmem_minfree_ratio_size < array_size)
				array_size = lowmem_minfree_ratio_size;
			lowmem_print(1, "[MULTITASKING] LMK - lowmem minfree param changed to: ");
			for (i = 0; i < array_size; i++) {
				lowmem_minfree[i] = min_pages +
					(lowmem_minfree_ratio[i] *
					(high_pages - min_pages)) / 100;
				lowmem_adj[i] = lowmem_adj_ratio[i];
				lowmem_print(1, " %ld,", (long) (lowmem_minfree[i]));
			}
			lowmem_print(1, "\n");
			lowmem_adj_size = array_size;
			lowmem_minfree_size = array_size;
			for (i = lowmem_minfree_size; i < ARRAYSIZE; i++) {
				lowmem_minfree[i] = 0;
			}
			for (i = lowmem_adj_size; i < ARRAYSIZE; i++) {
				lowmem_adj[i] = 0;
			}
			previous_used_high_pages = high_pages;
			break;
	case 0:
	default: /* '?' */
			/* print appropriate proportionally adjusted minfree values */
			array_size = ARRAYSIZE;
			if (lowmem_adj_size < array_size)
				array_size = lowmem_adj_size;
			if (lowmem_minfree_size < array_size)
				array_size = lowmem_minfree_size;
			lowmem_print(1, "[MULTITASKING] LMK - lowmem minfree param should be changed to: ");
			for (i = 0; i < array_size; i++) {
				minfree = (lowmem_minfree[i] * high_pages) /
					previous_used_high_pages;
				if (minfree < min_pages)
					minfree = min_pages+1;
				else
					if (minfree > high_pages)
						minfree = high_pages;

				lowmem_print(1, " %ld,", (long) minfree);
			}
			lowmem_print(1, "\n");
	}
	previous_min_pages = min_pages;
	previous_high_pages = high_pages;
	mutex_unlock(&lowmem_lock);
}
EXPORT_SYMBOL(calculate_lowmemkiller_params);

#define MAX_ELEMENT	256

int pid_list[MAX_ELEMENT];
char comm_list[MAX_ELEMENT][TASK_COMM_LEN];
unsigned int index_of_pid_comm=0;

static DEFINE_MUTEX(stt_lock);

static void get_pid_comm(int index, int *pid, char *comm) {
	*pid = pid_list[index];
	strncpy(comm, comm_list[index], strlen(comm_list[index])+1);
}

void reset_pid_comm_list(void) {
	index_of_pid_comm = 0;
}

static int killed_process_proc_show(struct seq_file *m, void *v)
{
	int pid;
	int i;
	char comm[TASK_COMM_LEN];
	mutex_lock(&stt_lock);
	for(i=0;i<index_of_pid_comm;i++) {
		get_pid_comm(i,&pid,comm);
		seq_printf(m, "%d %s\n",pid,comm);
	}
	reset_pid_comm_list();
	mutex_unlock(&stt_lock);
	return 0;
}

static int killed_process_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, killed_process_proc_show, NULL);
}

static const struct file_operations killed_process_proc_fops = {
	.open		= killed_process_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static struct task_struct *get_task(int p)
{
	struct task_struct *task;
	struct pid *pid;
	int ret = 0;

	pid = find_get_pid(p);

	if (!pid) {
		lowmem_print(1, "[MULTITASKING] LMK - The pid (%d) is unknown!\n", p);
		ret = -ESRCH;
		goto out;
	}

	task = get_pid_task(pid, PIDTYPE_PID);
	if (!task) {
		lowmem_print(1, "[MULTITASKING] LMK - There is not task with pid = %d!\n", p);
		ret = -ESRCH;
		put_pid(pid);
		goto out;
	}

	put_pid(pid);

	lowmem_print(3, "[MULTITASKING] LMK - get_task (pid = %d)\n", p);
	return task;
out:
	return ret;
}

static void put_task(struct task_struct *task)
{
	lowmem_print(3, "[MULTITASKING] LMK - put_task (pid = %d)\n", task->pid);
	put_task_struct(task);
}

int map_user_val(int val)
{
	int map_val;
	/*
	 * (Temoraly) Map userdefined prio to LMK adj values:
	 * 	6 is mapped to oom_adj = 12 (passive)
	 * 	5 is mapped to oom_adj = 6 (active)
	 */
	switch (val){
	case (6): map_val = 12;
		break;
	case (5): map_val = 6;
		break;
	case (OOM_DISABLE):
		map_val = val;
		break;
	default:
		map_val = val;
		lowmem_print(1,"[MULTITASKING] LMK - no rule to map user defined value (%d) to oom_adj\n", val);
		break;
	}
	return map_val;
}

static long lmk_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	struct task_struct *task;
	struct lmk_ioctl new = {0};
	int  ret = 0;

	switch(cmd)
	{
		case (0x00):
		case (0x10):
		case (0x20):
			lowmem_print(1, "[MULTITASKING] LMK - The command (0x%x) is depricated!\n", cmd);
			ret = -EINVAL;
			break;
		case (SET_KILL_PRIO_IOCTL):
			if (copy_from_user(&new, (void __user *)arg, sizeof(new))) {
				lowmem_print(1,"[MULTITASKING] LMK - unable to get lmk_ioctl params\n");
				ret = -EFAULT;
				break;
			}

			lowmem_print(4,"[MULTITASKING] LMK - The oom_adj = %d, pid = %d\n", new.val, new.pid);

			task = get_task(new.pid);

			ret = PTR_RET(task);
			if (!ret) {
				new.val = map_user_val(new.val);
				if (new.val != task->signal->oom_adj) {
					if ((new.val < OOM_ADJUST_MIN || new.val > OOM_ADJUST_MAX) &&
					    new.val != OOM_DISABLE) {
						ret = -EINVAL;
						lowmem_print(1,"[MULTITASKING] LMK - The oom_adj (%d) is not valid!\n", new.val);
						put_task(task);
						break;
					}

					/*
					 * Protect form beeing killing by OOM.
					 * LMK implementation is based on OOM_DISABLE only
					 * It is sems that further versions drops oom_disable_count
					 * and mpves on checking OOM_SCORE_ADJ_MIN only
					 */
					if (new.val == OOM_DISABLE)
						atomic_inc(&task->mm->oom_disable_count);
					if (task->signal->oom_adj == OOM_DISABLE)
						atomic_dec(&task->mm->oom_disable_count);

					ret = 0;
					task->signal->oom_adj = new.val;

					lowmem_print(1,"[MULTITASKING] LMK - The oom_adj = %d ret = %d pid = %d\n", task->signal->oom_adj, ret, new.pid);

				} else
					lowmem_print(4,"[MULTITASKING] LMK - The oom_adj for pid (%d) is not changed!\n", new.pid);

				put_task(task);
			} else
				lowmem_print(1,"[MULTITASKING] LMK - The pid (%d) is not valid!\n", new.pid);
		
			break;
		case (REG_MANAGER_IOCTL):
			if (copy_from_user(&new, (void __user *)arg, sizeof(new))) {
				lowmem_print(1,"[MULTITASKING] LMK - unable to get lmk_ioctl params\n");
				ret = -EFAULT;
				break;
			}

			lowmem_print(4,"[MULTITASKING] LMK - The oom_adj = %d, pid = %d\n", new.val, new.pid);

			if (taskManager && new.pid != taskManager)
				lowmem_print(1,"[MULTITASKING] LMK - Attempt to redefine Task Manager (%d) to pid = %d\n", taskManager, new.pid);

			/*
			 * Check the pid by getting its task_struct
			 */
			task = get_task(new.pid);

			ret = PTR_RET(task);
			if (!ret) {
				ret = taskManager;
				taskManager = new.pid;
				lowmem_print(4,"[MULTITASKING] LMK - The taskManager = %d ret = %d\n", taskManager, ret);

				put_task(task);
			} else
				lowmem_print(4,"[MULTITASKING] LMK - The pid (%d) is not valid!\n", new.pid);
			
			break;
		default:			
			lowmem_print(1,"[MULTITASKING] LMK - The command (0x%x) is unknown!\n", cmd);
			ret = -EINVAL;
			break;
	}

	return ret;
}

static const struct file_operations lmk_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = lmk_ioctl,
	.compat_ioctl = lmk_ioctl,
};

static struct miscdevice lmk_misc = {
	.minor = LMK_MINOR,
	.name = "lmk",
	.fops = &lmk_fops,
};

#define K(x) ((x) << (PAGE_SHIFT - 10))
extern long buff_for_perf_kb;
static int lowmem_shrink(struct shrinker *s, struct shrink_control *sc)
{
	struct task_struct *p;
	struct task_struct *selected = NULL;
	int rem = 0;
	int tasksize;
	int i;
	int min_adj = OOM_ADJUST_MAX + 1;
	int selected_tasksize = 0;
	int selected_oom_adj;
	int array_size = ARRAYSIZE;
	int other_free = global_page_state(NR_FREE_PAGES) + 
			global_page_state(NR_FILE_PAGES) - 
			global_page_state(NR_SHMEM)-(buff_for_perf_kb/4);
	int other_file = global_page_state(NR_FILE_PAGES) -
					global_page_state(NR_SHMEM);
	struct task_struct *task;

	mutex_lock(&lowmem_lock);

	if (lowmem_adj_size < array_size)
		array_size = lowmem_adj_size;
	if (lowmem_minfree_size < array_size)
		array_size = lowmem_minfree_size;
	for (i = 0; i < array_size; i++) {
		if (other_free < lowmem_minfree[i] &&
			other_file < lowmem_minfree[i]) {
			min_adj = lowmem_adj[i];
			break;
		}
	}
	if (sc->nr_to_scan > 0)
		lowmem_print(3, "[MULTITASKING] LMK - lowmem_shrink %lu, %x, ofree %d %d, ma %d\n",
			sc->nr_to_scan, sc->gfp_mask, other_free,
			other_file, min_adj);
	rem = global_page_state(NR_ACTIVE_ANON) +
		global_page_state(NR_ACTIVE_FILE) +
		global_page_state(NR_INACTIVE_ANON) +
		global_page_state(NR_INACTIVE_FILE);
	if (sc->nr_to_scan <= 0 || min_adj == OOM_ADJUST_MAX + 1) {
		lowmem_print(5, "[MULTITASKING] LMK - lowmem_shrink %lu, %x, return %d\n",
				 sc->nr_to_scan, sc->gfp_mask, rem);
		goto out;
	}
	selected_oom_adj = min_adj;
	read_lock(&tasklist_lock);
	for_each_process(p) {
		struct mm_struct *mm;
		struct signal_struct *sig;
		int oom_adj;

		if (test_tsk_thread_flag(p, TIF_MEMDIE) &&
			time_before_eq(jiffies, lowmem_deathpending_timeout)) {
			read_unlock(&tasklist_lock);
			rem = 0;
			goto out;
		}
		task_lock(p);
		mm = p->mm;
		sig = p->signal;
		if (!mm || !sig) {
			task_unlock(p);
			continue;
		}
		oom_adj = sig->oom_adj;
		if ((oom_adj < min_adj) || (oom_adj == OOM_DISABLE)) {
			lowmem_print(3, "[MULTITASKING] LMK - task %d (%s), adj %d has been skipped\n",
				 p->pid, p->comm, oom_adj);

			task_unlock(p);
			continue;
		}
		tasksize = get_mm_rss(mm);
		task_unlock(p);
		if (tasksize <= 0)
			continue;
		if (selected) {
			if (oom_adj < selected_oom_adj)
				continue;
			if (oom_adj == selected_oom_adj &&
				tasksize <= selected_tasksize)
				continue;
		}
		selected = p;
		selected_tasksize = tasksize;
		selected_oom_adj = oom_adj;
		lowmem_print(2, "[MULTITASKING] LMK - select %d (%s), adj %d, size %d, to kill\n",
				 p->pid, p->comm, oom_adj, tasksize);
	}
	if (selected) {
#ifndef CONFIG_VD_RELEASE 
		struct task_struct *p;
		unsigned int oom_adj_6_cnt=0, oom_adj_12_cnt=0;

		for_each_process(p) {
	                task = find_lock_task_mm(p);
	                if (!task) {
	       	                 /*
	                         * This is a kthread or all of p's threads have already
        	                 * detached their mm's.  There's no need to report
	                         * them; they can't be oom killed anyway.
	     	                 */
	                        continue;
	                }
			if(p->signal->oom_adj == 6)
				oom_adj_6_cnt++;
			else if(p->signal->oom_adj == 12)
				oom_adj_12_cnt++;
			task_unlock(task);
			
		}

		lowmem_print(1, "[MULTITASKING] LMK - send sigkill to %d (%s), adj %d, size %d, vd_memfree %lu, ofree %d, of %d, 6 adj %d, 12 adj %d \n",
				 selected->pid, selected->comm,
				 selected_oom_adj, selected_tasksize,
				K(global_page_state(NR_FREE_PAGES)+global_page_state(NR_FILE_PAGES)-
				global_page_state(NR_SHMEM))-buff_for_perf_kb,
				K(other_free),K(other_file), oom_adj_6_cnt, oom_adj_12_cnt);

#else
		lowmem_print(1, "[MULTITASKING] LMK - send sigkill to %d (%s), adj %d, size %d, vd_memfree %lu, ofree %d, of %d\n",
				 selected->pid, selected->comm,
				 selected_oom_adj, selected_tasksize,
				K(global_page_state(NR_FREE_PAGES)+global_page_state(NR_FILE_PAGES)-
				global_page_state(NR_SHMEM))-buff_for_perf_kb,
				K(other_free),K(other_file));
#endif

		lowmem_deathpending_timeout = jiffies + HZ;

		force_sig(SIGKILL, selected);

		set_tsk_thread_flag(selected, TIF_MEMDIE);
		rem -= selected_tasksize;

	}
	read_unlock(&tasklist_lock);

	lowmem_print(4, "[MULTITASKING] LMK - lowmem_shrink %lu, %x, return %d\n",
			 sc->nr_to_scan, sc->gfp_mask, rem);
out:
	mutex_unlock(&lowmem_lock);
	return rem;
}

static ctl_table lmk_table[] = {

	{
		.procname		= "cost",
		.data			= &lowmem_shrinker.seeks,
		.maxlen		 = sizeof(int),
		.mode			= 0444,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		 = &one,
		.extra2		 = &max
	},
	{
		.procname		= "debug_level",
		.data			= &lowmem_debug_level,
		.maxlen		 = sizeof(uint),
		.mode			= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		 = &zero,
		.extra2		 = &five
	},
	{
		.procname		= "enabled",
		.data			= &lowmem_enabled,
		.maxlen		 = sizeof(uint),
		.mode			= 0644,
		.proc_handler	= lowmem_enabled_sysctl_handler,
		.extra1		 = &zero,
		.extra2		 = &one
	},
	{
		.procname		= "mode",
		.data			= &lowmem_mode,
		.maxlen		 = sizeof(uint),
		.mode			= 0644,
		.proc_handler	= lowmem_mode_sysctl_handler,
		.extra1		 = &zero,
		.extra2		 = &two
	},
	{
		.procname		= "minfree",
		.data			= &lowmem_minfree,
		.maxlen		 = sizeof(lowmem_minfree),
		.mode			= 0644,
		.proc_handler	= lowmem_minfree_sysctl_handler,
		.extra1		 = &one,
		.extra2		 = &max
	},
	{
		.procname		= "adj",
		.data			= &lowmem_adj,
		.maxlen		 = sizeof(lowmem_adj),
		.mode			= 0644,
		.proc_handler	= lowmem_adj_sysctl_handler,
		.extra1		 = &min_adj,
		.extra2		 = &max_adj
	},
		{
		.procname		= "minfree_ratio",
		.data			= &lowmem_minfree_ratio,
		.maxlen		 = sizeof(lowmem_minfree_ratio),
		.mode			= 0644,
		.proc_handler	= lowmem_minfree_ratio_sysctl_handler,
		.extra1		 = &zero,
		.extra2		 = &max
	},
	{
		.procname		= "adj_ratio",
		.data			= &lowmem_adj_ratio,
		.maxlen		 = sizeof(lowmem_adj_ratio),
		.mode			= 0644,
		.proc_handler	= lowmem_adj_ratio_sysctl_handler,
		.extra1		 = &min_adj,
		.extra2		 = &max_adj
	},
	{0}
	};

static ctl_table lmk_vm_table[] = {
	{
			.procname	  = "lmk",
			.mode		  = 0555,
			.child		= lmk_table
	},
	{ 0 }
};

static ctl_table lmk_root_table[] = {
	{
			.procname		= "vm",
			.mode			= 0555,
			.child		  = lmk_vm_table
	},
	{  0 }
};

static struct ctl_table_header *lmk_table_header;

static int __init lowmem_init(void)
{
	int ret;
	struct proc_dir_entry *entry;

	lowmem_mode = 0;
	lmk_table_header = register_sysctl_table(lmk_root_table);
	if (!lmk_table_header)
		return -ENOMEM;
	mutex_lock(&lowmem_lock);
	if (lowmem_enabled)
		register_shrinker(&lowmem_shrinker);
	mutex_unlock(&lowmem_lock);

	entry = proc_create("killed_process_by_lmk", 0, NULL, &killed_process_proc_fops);
	if (!entry) {
		printk(KERN_ERR "failed to create proc entry!\n");
		return -ENOMEM;
	}

	ret = misc_register(&lmk_misc);
	if (unlikely(ret)) {
		printk(KERN_ERR "failed to register misc device!\n");
		return ret;
	}

	return 0;
}

static void __exit lowmem_exit(void)
{
	int ret;
	mutex_lock(&lowmem_lock);
	if (lowmem_enabled)
		unregister_shrinker(&lowmem_shrinker);
	mutex_unlock(&lowmem_lock);
	unregister_sysctl_table(lmk_table_header);
	remove_proc_entry("killed_process_by_lmk", NULL);
	ret = misc_deregister(&lmk_misc);
	if (unlikely(ret))
		printk(KERN_ERR "failed to unregister misc device!\n");
}

module_param_named(cost, lowmem_shrinker.seeks, int, S_IRUGO | S_IWUSR);
module_param_array_named(adj, lowmem_adj, int, &lowmem_adj_size,
			 S_IRUGO | S_IWUSR);
module_param_array_named(minfree, lowmem_minfree, uint, &lowmem_minfree_size,
			 S_IRUGO | S_IWUSR);
module_param_named(debug_level, lowmem_debug_level, uint, S_IRUGO | S_IWUSR);

module_init(lowmem_init);
module_exit(lowmem_exit);

MODULE_LICENSE("GPL");

