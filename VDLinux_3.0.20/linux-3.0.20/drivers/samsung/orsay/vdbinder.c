/* vdbinder.c
 * version : VDBinder 2.2
 * Android IPC Subsystem
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

#include <asm/cacheflush.h>
#include <linux/fdtable.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/list.h>
#include <linux/miscdevice.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/nsproxy.h>
#include <linux/poll.h>
#include <linux/debugfs.h>
#include <linux/rbtree.h>
#include <linux/sched.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/vmalloc.h>

#include "vdbinder.h"
#include "vdbinder_debug.h"
#include "vdbinder_internal.h"

DEFINE_MUTEX(binder_lock);
static DEFINE_MUTEX(binder_deferred_lock);

HLIST_HEAD(binder_procs);
static HLIST_HEAD(binder_deferred_list);
static HLIST_HEAD(binder_dead_nodes);
static LIST_HEAD(vdbinder_service_wait_list);

static struct dentry *binder_debugfs_dir_entry_root;
static struct dentry *binder_debugfs_dir_entry_proc;
static struct vdbinder_node *binder_context_mgr_node;
static uid_t binder_context_mgr_uid = -1;
static int binder_last_id;
static struct workqueue_struct *binder_deferred_workqueue;
int binder_this_board_no;

static unsigned long vdbinder_time_offset; /*in seconds*/
static int vdbinder_is_new_time_set; /*set if new time arrives*/
static DECLARE_WAIT_QUEUE_HEAD(vdbinder_rpcagent_wait);

#define VDBINDER_DEBUG_ENTRY(name) \
static int binder_##name##_open(struct inode *inode, struct file *file) \
{ \
	return single_open(file, binder_##name##_show, inode->i_private); \
} \
\
static const struct file_operations binder_##name##_fops = { \
	.owner = THIS_MODULE, \
	.open = binder_##name##_open, \
	.read = seq_read, \
	.llseek = seq_lseek, \
	.release = single_release, \
}

static int binder_proc_show(struct seq_file *m, void *unused);
VDBINDER_DEBUG_ENTRY(proc);

/* This is only defined in include/asm-arm/sizes.h */
#ifndef VD_SZ_1K
#define VD_SZ_1K                               0x400
#endif

#ifndef VD_SZ_4M
#define VD_SZ_4M                               0x400000
#endif

#define VD_FORBIDDEN_MMAP_FLAGS                (VM_WRITE)

#define VD_BINDER_SMALL_BUF_SIZE (PAGE_SIZE * 64)

#define IS_BINDER_USER_THREAD(looper) ((bool)!(looper & \
					(VDBINDER_LOOPER_STATE_REGISTERED | \
					 VDBINDER_LOOPER_STATE_ENTERED | \
					 VDBINDER_LOOPER_STATE_EXITED | \
					 VDBINDER_LOOPER_STATE_INVALID)))

enum {
	VDBINDER_DEBUG_USER_ERROR             = 1U << 0,
	VDBINDER_DEBUG_FAILED_TRANSACTION     = 1U << 1,
	VDBINDER_DEBUG_DEAD_TRANSACTION       = 1U << 2,
	VDBINDER_DEBUG_OPEN_CLOSE             = 1U << 3,
	VDBINDER_DEBUG_DEAD_BINDER            = 1U << 4,
	VDBINDER_DEBUG_DEATH_NOTIFICATION     = 1U << 5,
	VDBINDER_DEBUG_READ_WRITE             = 1U << 6,
	VDBINDER_DEBUG_USER_REFS              = 1U << 7,
	VDBINDER_DEBUG_THREADS                = 1U << 8,
	VDBINDER_DEBUG_TRANSACTION            = 1U << 9,
	VDBINDER_DEBUG_TRANSACTION_COMPLETE   = 1U << 10,
	VDBINDER_DEBUG_FREE_BUFFER            = 1U << 11,
	VDBINDER_DEBUG_INTERNAL_REFS          = 1U << 12,
	VDBINDER_DEBUG_BUFFER_ALLOC           = 1U << 13,
	VDBINDER_DEBUG_PRIORITY_CAP           = 1U << 14,
	VDBINDER_DEBUG_BUFFER_ALLOC_ASYNC     = 1U << 15,
};
static uint32_t binder_debug_mask = VDBINDER_DEBUG_USER_ERROR |
	VDBINDER_DEBUG_FAILED_TRANSACTION | VDBINDER_DEBUG_DEAD_TRANSACTION;
module_param_named(debug_mask, binder_debug_mask, uint, S_IWUSR | S_IRUGO);

static int binder_debug_no_lock;
module_param_named(proc_no_lock, binder_debug_no_lock, bool, S_IWUSR | S_IRUGO);

static DECLARE_WAIT_QUEUE_HEAD(binder_user_error_wait);
static DECLARE_WAIT_QUEUE_HEAD(binder_context_mgr_wait);
static int binder_stop_on_user_error;

static int binder_set_stop_on_user_error(const char *val,
					 struct kernel_param *kp)
{
	int ret;
	ret = param_set_int(val, kp);
	if (binder_stop_on_user_error < 2)
		wake_up(&binder_user_error_wait);
	return ret;
}
module_param_call(stop_on_user_error, binder_set_stop_on_user_error,
	param_get_int, &binder_stop_on_user_error, S_IWUSR | S_IRUGO);

#define vdbinder_debug(mask, x...) \
	do { \
		if (binder_debug_mask & mask) \
			printk(KERN_INFO x); \
	} while (0)

#define vdbinder_user_error(x...) \
	do { \
		if (binder_debug_mask & VDBINDER_DEBUG_USER_ERROR) \
			printk(KERN_INFO x); \
		if (binder_stop_on_user_error) \
			binder_stop_on_user_error = 2; \
	} while (0)

static struct vdbinder_stats vdbinder_stats;

static inline void binder_stats_deleted(enum vdbinder_stat_types type)
{
	vdbinder_stats.obj_deleted[type]++;
}

static inline void binder_stats_created(enum vdbinder_stat_types type)
{
	vdbinder_stats.obj_created[type]++;
}

struct vdbinder_transaction_log_entry {
	int debug_id;
	int call_type;
	int from_proc;
	int from_thread;
	int target_handle;
	int to_proc;
	int to_thread;
	int to_node;
	int data_size;
	int offsets_size;
};
struct vdbinder_transaction_log {
	int next;
	int full;
	struct vdbinder_transaction_log_entry entry[32];
};
static struct vdbinder_transaction_log vdbinder_transaction_log;
static struct vdbinder_transaction_log binder_transaction_log_failed;

static struct vdbinder_transaction_log_entry *binder_transaction_log_add(
	struct vdbinder_transaction_log *log)
{
	struct vdbinder_transaction_log_entry *e;
	e = &log->entry[log->next];
	memset(e, 0, sizeof(*e));
	log->next++;
	if (log->next == ARRAY_SIZE(log->entry)) {
		log->next = 0;
		log->full = 1;
	}
	return e;
}

struct vdbinder_service_wait_work {
	struct vdbinder_transaction *t;
	size_t *offset;
	unsigned int index;
	struct list_head wait_entry;
};

struct vdbinder_ref_death {
	struct vdbinder_work work;
	void __user *cookie;
};

struct vdbinder_ref {
	/* Lookups needed: */
	/*   node + proc => ref (transaction) */
	/*   desc + proc => ref (transaction, inc/dec ref) */
	/*   node => refs + procs (proc exit) */
	int debug_id;
	struct rb_node rb_node_desc;
	struct rb_node rb_node_node;
	struct hlist_node node_entry;
	struct vdbinder_proc *proc;
	struct vdbinder_node *node;
	uint32_t desc;
	int strong;
	int weak;
	struct vdbinder_ref_death *death;
};

enum vdbinder_deferred_state {
	VDBINDER_DEFERRED_PUT_FILES    = 0x01,
	VDBINDER_DEFERRED_FLUSH        = 0x02,
	VDBINDER_DEFERRED_RELEASE      = 0x04,
};

enum {
	VDBINDER_LOOPER_STATE_REGISTERED  = 0x01,
	VDBINDER_LOOPER_STATE_ENTERED     = 0x02,
	VDBINDER_LOOPER_STATE_EXITED      = 0x04,
	VDBINDER_LOOPER_STATE_INVALID     = 0x08,
	VDBINDER_LOOPER_STATE_WAITING     = 0x10,
	VDBINDER_LOOPER_STATE_NEED_RETURN = 0x20,
	VDBINDER_LOOPER_STATE_THREAD_WAIT = 0x40
};

static void
binder_defer_work(struct vdbinder_proc *proc, enum vdbinder_deferred_state defer);

/*
 * copied from get_unused_fd_flags
 */
static int task_get_unused_fd_flags(struct vdbinder_proc *proc, int flags)
{
	struct files_struct *files = proc->files;
	int fd, error;
	struct fdtable *fdt;
	unsigned long rlim_cur;
	unsigned long irqs;

	if (files == NULL)
		return -ESRCH;

	error = -EMFILE;
	spin_lock(&files->file_lock);

repeat:
	fdt = files_fdtable(files);
	fd = find_next_zero_bit(fdt->open_fds->fds_bits, fdt->max_fds,
				files->next_fd);

	/*
	 * N.B. For clone tasks sharing a files structure, this test
	 * will limit the total number of files that can be opened.
	 */
	rlim_cur = 0;
	if (lock_task_sighand(proc->tsk, &irqs)) {
		rlim_cur = proc->tsk->signal->rlim[RLIMIT_NOFILE].rlim_cur;
		unlock_task_sighand(proc->tsk, &irqs);
	}
	if (fd >= rlim_cur)
		goto out;

	/* Do we need to expand the fd array or fd set?  */
	error = expand_files(files, fd);
	if (error < 0)
		goto out;

	if (error) {
		/*
		 * If we needed to expand the fs array we
		 * might have blocked - try again.
		 */
		error = -EMFILE;
		goto repeat;
	}

	FD_SET(fd, fdt->open_fds);
	if (flags & O_CLOEXEC)
		FD_SET(fd, fdt->close_on_exec);
	else
		FD_CLR(fd, fdt->close_on_exec);
	files->next_fd = fd + 1;
#if 1
	/* Sanity check */
	if (fdt->fd[fd] != NULL) {
		printk(KERN_WARNING "get_unused_fd: slot %d not NULL!\n", fd);
		fdt->fd[fd] = NULL;
	}
#endif
	error = fd;

out:
	spin_unlock(&files->file_lock);
	return error;
}

/*
 * copied from fd_install
 */
static void task_fd_install(
	struct vdbinder_proc *proc, unsigned int fd, struct file *file)
{
	struct files_struct *files = proc->files;
	struct fdtable *fdt;

	if (files == NULL)
		return;

	spin_lock(&files->file_lock);
	fdt = files_fdtable(files);
	BUG_ON(fdt->fd[fd] != NULL);
	rcu_assign_pointer(fdt->fd[fd], file);
	spin_unlock(&files->file_lock);
}

/*
 * copied from __put_unused_fd in open.c
 */
static void __put_unused_fd(struct files_struct *files, unsigned int fd)
{
	struct fdtable *fdt = files_fdtable(files);
	__FD_CLR(fd, fdt->open_fds);
	if (fd < files->next_fd)
		files->next_fd = fd;
}

/*
 * copied from sys_close
 */
static long task_close_fd(struct vdbinder_proc *proc, unsigned int fd)
{
	struct file *filp;
	struct files_struct *files = proc->files;
	struct fdtable *fdt;
	int retval;

	if (files == NULL)
		return -ESRCH;

	spin_lock(&files->file_lock);
	fdt = files_fdtable(files);
	if (fd >= fdt->max_fds)
		goto out_unlock;
	filp = fdt->fd[fd];
	if (!filp)
		goto out_unlock;
	rcu_assign_pointer(fdt->fd[fd], NULL);
	FD_CLR(fd, fdt->close_on_exec);
	__put_unused_fd(files, fd);
	spin_unlock(&files->file_lock);
	retval = filp_close(filp, files);

	/* can't restart close syscall because file table entry was cleared */
	if (unlikely(retval == -ERESTARTSYS ||
		     retval == -ERESTARTNOINTR ||
		     retval == -ERESTARTNOHAND ||
		     retval == -ERESTART_RESTARTBLOCK))
		retval = -EINTR;

	return retval;

out_unlock:
	spin_unlock(&files->file_lock);
	return -EBADF;
}

static void binder_set_nice(long nice)
{
	long min_nice;
	if (can_nice(current, nice)) {
		set_user_nice(current, nice);
		return;
	}
	min_nice = 20 - current->signal->rlim[RLIMIT_NICE].rlim_cur;
	vdbinder_debug(VDBINDER_DEBUG_PRIORITY_CAP,
		     "binder: %d: nice value %ld not allowed use "
		     "%ld instead\n", current->pid, nice, min_nice);
	set_user_nice(current, min_nice);
	if (min_nice < 20)
		return;
	vdbinder_user_error("binder: %d RLIMIT_NICE not set\n", current->pid);
}

static size_t binder_buffer_size(struct vdbinder_proc *proc,
				 struct vdbinder_buffer *buffer)
{
	if (list_is_last(&buffer->entry, &proc->buffers))
		return proc->buffer + proc->buffer_size - (void *)buffer->data;
	else
		return (size_t)list_entry(buffer->entry.next,
			struct vdbinder_buffer, entry) - (size_t)buffer->data;
}

static void binder_insert_free_buffer(struct vdbinder_proc *proc,
				      struct vdbinder_buffer *new_buffer)
{
	struct rb_node **p = &proc->free_buffers.rb_node;
	struct rb_node *parent = NULL;
	struct vdbinder_buffer *buffer;
	size_t buffer_size;
	size_t new_buffer_size;

	BUG_ON(!new_buffer->free);

	new_buffer_size = binder_buffer_size(proc, new_buffer);

	vdbinder_debug(VDBINDER_DEBUG_BUFFER_ALLOC,
		     "binder: %d: add free buffer, size %zd, "
		     "at %p\n", proc->pid, new_buffer_size, new_buffer);

	while (*p) {
		parent = *p;
		buffer = rb_entry(parent, struct vdbinder_buffer, rb_node);
		BUG_ON(!buffer->free);

		buffer_size = binder_buffer_size(proc, buffer);

		if (new_buffer_size < buffer_size)
			p = &parent->rb_left;
		else
			p = &parent->rb_right;
	}
	rb_link_node(&new_buffer->rb_node, parent, p);
	rb_insert_color(&new_buffer->rb_node, &proc->free_buffers);
}

static void binder_insert_allocated_buffer(struct vdbinder_proc *proc,
					   struct vdbinder_buffer *new_buffer)
{
	struct rb_node **p = &proc->allocated_buffers.rb_node;
	struct rb_node *parent = NULL;
	struct vdbinder_buffer *buffer;

	BUG_ON(new_buffer->free);

	while (*p) {
		parent = *p;
		buffer = rb_entry(parent, struct vdbinder_buffer, rb_node);
		BUG_ON(buffer->free);

		if (new_buffer < buffer)
			p = &parent->rb_left;
		else if (new_buffer > buffer)
			p = &parent->rb_right;
		else
			BUG();
	}
	rb_link_node(&new_buffer->rb_node, parent, p);
	rb_insert_color(&new_buffer->rb_node, &proc->allocated_buffers);
}

static struct vdbinder_buffer *binder_buffer_lookup(struct vdbinder_proc *proc,
						  void __user *user_ptr)
{
	struct rb_node *n = proc->allocated_buffers.rb_node;
	struct vdbinder_buffer *buffer;
	struct vdbinder_buffer *kern_ptr;

	kern_ptr = user_ptr - proc->user_buffer_offset
		- offsetof(struct vdbinder_buffer, data);

	while (n) {
		buffer = rb_entry(n, struct vdbinder_buffer, rb_node);
		BUG_ON(buffer->free);

		if (kern_ptr < buffer)
			n = n->rb_left;
		else if (kern_ptr > buffer)
			n = n->rb_right;
		else
			return buffer;
	}
	return NULL;
}

static int binder_update_page_range(struct vdbinder_proc *proc, int allocate,
				    void *start, void *end,
				    struct vm_area_struct *vma)
{
	void *page_addr;
	unsigned long user_page_addr;
	struct vm_struct tmp_area;
	struct page **page;
	struct mm_struct *mm;

	vdbinder_debug(VDBINDER_DEBUG_BUFFER_ALLOC,
		     "binder: %d: %s pages %p-%p\n", proc->pid,
		     allocate ? "allocate" : "free", start, end);

	if (end <= start)
		return 0;

	if (vma)
		mm = NULL;
	else
		mm = get_task_mm(proc->tsk);

	if (mm) {
		down_write(&mm->mmap_sem);
		vma = proc->vma;
	}

	if (allocate == 0)
		goto free_range;

	if (vma == NULL) {
		printk(KERN_ERR "binder: %d: binder_alloc_buf failed to "
		       "map pages in userspace, no vma\n", proc->pid);
		goto err_no_vma;
	}

	for (page_addr = start; page_addr < end; page_addr += PAGE_SIZE) {
		int ret;
		struct page **page_array_ptr;
		page = &proc->pages[(page_addr - proc->buffer) / PAGE_SIZE];

		BUG_ON(*page);
		*page = alloc_page(GFP_KERNEL | __GFP_ZERO);
		if (*page == NULL) {
			printk(KERN_ERR "binder: %d: binder_alloc_buf failed "
			       "for page at %p\n", proc->pid, page_addr);
			goto err_alloc_page_failed;
		}
		tmp_area.addr = page_addr;
		tmp_area.size = PAGE_SIZE + PAGE_SIZE /* guard page? */;
		page_array_ptr = page;
		ret = map_vm_area(&tmp_area, PAGE_KERNEL, &page_array_ptr);
		if (ret) {
			printk(KERN_ERR "binder: %d: binder_alloc_buf failed "
			       "to map page at %p in kernel\n",
			       proc->pid, page_addr);
			goto err_map_kernel_failed;
		}
		user_page_addr =
			(uintptr_t)page_addr + proc->user_buffer_offset;
		ret = vm_insert_page(vma, user_page_addr, page[0]);
		if (ret) {
			printk(KERN_ERR "binder: %d: binder_alloc_buf failed "
			       "to map page at %lx in userspace\n",
			       proc->pid, user_page_addr);
			goto err_vm_insert_page_failed;
		}
		/* vm_insert_page does not seem to increment the refcount */
	}
	if (mm) {
		up_write(&mm->mmap_sem);
		mmput(mm);
	}
	return 0;

free_range:
	for (page_addr = end - PAGE_SIZE; page_addr >= start;
	     page_addr -= PAGE_SIZE) {
		page = &proc->pages[(page_addr - proc->buffer) / PAGE_SIZE];
		if (vma)
			zap_page_range(vma, (uintptr_t)page_addr +
				proc->user_buffer_offset, PAGE_SIZE, NULL);
err_vm_insert_page_failed:
		unmap_kernel_range((unsigned long)page_addr, PAGE_SIZE);
err_map_kernel_failed:
		__free_page(*page);
		*page = NULL;
err_alloc_page_failed:
		;
	}
err_no_vma:
	if (mm) {
		up_write(&mm->mmap_sem);
		mmput(mm);
	}
	return -ENOMEM;
}

static struct vdbinder_buffer *binder_alloc_buf(struct vdbinder_proc *proc,
					      size_t data_size,
					      size_t offsets_size, int is_async)
{
	struct rb_node *n = proc->free_buffers.rb_node;
	struct vdbinder_buffer *buffer;
	size_t buffer_size;
	struct rb_node *best_fit = NULL;
	void *has_page_addr;
	void *end_page_addr;
	size_t size;

	if (proc->vma == NULL) {
		printk(KERN_ERR "binder: %d: binder_alloc_buf, no vma\n",
		       proc->pid);
		return NULL;
	}

	size = ALIGN(data_size, sizeof(void *)) +
		ALIGN(offsets_size, sizeof(void *));

	if (size < data_size || size < offsets_size) {
		vdbinder_user_error("binder: %d: got transaction with invalid "
			"size %zd-%zd\n", proc->pid, data_size, offsets_size);
		return NULL;
	}

	if (is_async &&
	    proc->free_async_space < size + sizeof(struct vdbinder_buffer)) {
		vdbinder_debug(VDBINDER_DEBUG_BUFFER_ALLOC,
			     "binder: %d: binder_alloc_buf size %zd"
			     "failed, no async space left\n", proc->pid, size);
		return NULL;
	}

	while (n) {
		buffer = rb_entry(n, struct vdbinder_buffer, rb_node);
		BUG_ON(!buffer->free);
		buffer_size = binder_buffer_size(proc, buffer);

		if (size < buffer_size) {
			best_fit = n;
			n = n->rb_left;
		} else if (size > buffer_size)
			n = n->rb_right;
		else {
			best_fit = n;
			break;
		}
	}
	if (best_fit == NULL) {
		printk(KERN_ERR "binder: %d: binder_alloc_buf size %zd failed, "
		       "no address space\n", proc->pid, size);
		return NULL;
	}
	if (n == NULL) {
		buffer = rb_entry(best_fit, struct vdbinder_buffer, rb_node);
		buffer_size = binder_buffer_size(proc, buffer);
	}

	vdbinder_debug(VDBINDER_DEBUG_BUFFER_ALLOC,
		     "binder: %d: binder_alloc_buf size %zd got buff"
		     "er %p size %zd\n", proc->pid, size, buffer, buffer_size);

	has_page_addr =
		(void *)(((uintptr_t)buffer->data + buffer_size) & PAGE_MASK);
	if (n == NULL) {
		if (size + sizeof(struct vdbinder_buffer) + 4 >= buffer_size)
			buffer_size = size; /* no room for other buffers */
		else
			buffer_size = size + sizeof(struct vdbinder_buffer);
	}
	end_page_addr =
		(void *)PAGE_ALIGN((uintptr_t)buffer->data + buffer_size);
	if (end_page_addr > has_page_addr)
		end_page_addr = has_page_addr;
	if (binder_update_page_range(proc, 1,
	    (void *)PAGE_ALIGN((uintptr_t)buffer->data), end_page_addr, NULL))
		return NULL;

	rb_erase(best_fit, &proc->free_buffers);
	buffer->free = 0;
	binder_insert_allocated_buffer(proc, buffer);
	if (buffer_size != size) {
		struct vdbinder_buffer *new_buffer = (void *)buffer->data + size;
		list_add(&new_buffer->entry, &buffer->entry);
		new_buffer->free = 1;
		binder_insert_free_buffer(proc, new_buffer);
	}
	vdbinder_debug(VDBINDER_DEBUG_BUFFER_ALLOC,
		     "binder: %d: binder_alloc_buf size %zd got "
		     "%p\n", proc->pid, size, buffer);
	buffer->data_size = data_size;
	buffer->offsets_size = offsets_size;
	buffer->async_transaction = is_async;
	if (is_async) {
		proc->free_async_space -= size + sizeof(struct vdbinder_buffer);
		vdbinder_debug(VDBINDER_DEBUG_BUFFER_ALLOC_ASYNC,
			     "binder: %d: binder_alloc_buf size %zd "
			     "async free %zd\n", proc->pid, size,
			     proc->free_async_space);
	}

	return buffer;
}

static void *buffer_start_page(struct vdbinder_buffer *buffer)
{
	return (void *)((uintptr_t)buffer & PAGE_MASK);
}

static void *buffer_end_page(struct vdbinder_buffer *buffer)
{
	return (void *)(((uintptr_t)(buffer + 1) - 1) & PAGE_MASK);
}

static void binder_delete_free_buffer(struct vdbinder_proc *proc,
				      struct vdbinder_buffer *buffer)
{
	struct vdbinder_buffer *prev, *next = NULL;
	int free_page_end = 1;
	int free_page_start = 1;

	BUG_ON(proc->buffers.next == &buffer->entry);
	prev = list_entry(buffer->entry.prev, struct vdbinder_buffer, entry);
	BUG_ON(!prev->free);
	if (buffer_end_page(prev) == buffer_start_page(buffer)) {
		free_page_start = 0;
		if (buffer_end_page(prev) == buffer_end_page(buffer))
			free_page_end = 0;
		vdbinder_debug(VDBINDER_DEBUG_BUFFER_ALLOC,
			     "binder: %d: merge free, buffer %p "
			     "share page with %p\n", proc->pid, buffer, prev);
	}

	if (!list_is_last(&buffer->entry, &proc->buffers)) {
		next = list_entry(buffer->entry.next,
				  struct vdbinder_buffer, entry);
		if (buffer_start_page(next) == buffer_end_page(buffer)) {
			free_page_end = 0;
			if (buffer_start_page(next) ==
			    buffer_start_page(buffer))
				free_page_start = 0;
			vdbinder_debug(VDBINDER_DEBUG_BUFFER_ALLOC,
				     "binder: %d: merge free, buffer"
				     " %p share page with %p\n", proc->pid,
				     buffer, prev);
		}
	}
	list_del(&buffer->entry);
	if (free_page_start || free_page_end) {
		vdbinder_debug(VDBINDER_DEBUG_BUFFER_ALLOC,
			     "binder: %d: merge free, buffer %p do "
			     "not share page%s%s with with %p or %p\n",
			     proc->pid, buffer, free_page_start ? "" : " end",
			     free_page_end ? "" : " start", prev, next);
		binder_update_page_range(proc, 0, free_page_start ?
			buffer_start_page(buffer) : buffer_end_page(buffer),
			(free_page_end ? buffer_end_page(buffer) :
			buffer_start_page(buffer)) + PAGE_SIZE, NULL);
	}
}

static void binder_free_buf(struct vdbinder_proc *proc,
			    struct vdbinder_buffer *buffer)
{
	size_t size, buffer_size;

	buffer_size = binder_buffer_size(proc, buffer);

	size = ALIGN(buffer->data_size, sizeof(void *)) +
		ALIGN(buffer->offsets_size, sizeof(void *));

	vdbinder_debug(VDBINDER_DEBUG_BUFFER_ALLOC,
		     "binder: %d: binder_free_buf %p size %zd buffer"
		     "_size %zd\n", proc->pid, buffer, size, buffer_size);

	BUG_ON(buffer->free);
	BUG_ON(size > buffer_size);
	BUG_ON(buffer->transaction != NULL);
	BUG_ON((void *)buffer < proc->buffer);
	BUG_ON((void *)buffer > proc->buffer + proc->buffer_size);

	if (buffer->async_transaction) {
		proc->free_async_space += size + sizeof(struct vdbinder_buffer);

		vdbinder_debug(VDBINDER_DEBUG_BUFFER_ALLOC_ASYNC,
			     "binder: %d: binder_free_buf size %zd "
			     "async free %zd\n", proc->pid, size,
			     proc->free_async_space);
	}

	binder_update_page_range(proc, 0,
		(void *)PAGE_ALIGN((uintptr_t)buffer->data),
		(void *)(((uintptr_t)buffer->data + buffer_size) & PAGE_MASK),
		NULL);
	rb_erase(&buffer->rb_node, &proc->allocated_buffers);
	buffer->free = 1;
	if (!list_is_last(&buffer->entry, &proc->buffers)) {
		struct vdbinder_buffer *next = list_entry(buffer->entry.next,
						struct vdbinder_buffer, entry);
		if (next->free) {
			rb_erase(&next->rb_node, &proc->free_buffers);
			binder_delete_free_buffer(proc, next);
		}
	}
	if (proc->buffers.next != &buffer->entry) {
		struct vdbinder_buffer *prev = list_entry(buffer->entry.prev,
						struct vdbinder_buffer, entry);
		if (prev->free) {
			binder_delete_free_buffer(proc, buffer);
			rb_erase(&prev->rb_node, &proc->free_buffers);
			buffer = prev;
		}
	}
	binder_insert_free_buffer(proc, buffer);
}

static struct vdbinder_node *binder_get_node(struct vdbinder_proc *proc,
					   void __user *ptr)
{
	struct rb_node *n = proc->nodes.rb_node;
	struct vdbinder_node *node;

	while (n) {
		node = rb_entry(n, struct vdbinder_node, rb_node);

		if (ptr < node->ptr)
			n = n->rb_left;
		else if (ptr > node->ptr)
			n = n->rb_right;
		else
			return node;
	}
	return NULL;
}

static struct vdbinder_node *binder_new_node(struct vdbinder_proc *proc,
					   void __user *ptr,
					   void __user *cookie)
{
	struct rb_node **p = &proc->nodes.rb_node;
	struct rb_node *parent = NULL;
	struct vdbinder_node *node;

	while (*p) {
		parent = *p;
		node = rb_entry(parent, struct vdbinder_node, rb_node);

		if (ptr < node->ptr)
			p = &(*p)->rb_left;
		else if (ptr > node->ptr)
			p = &(*p)->rb_right;
		else
			return NULL;
	}

	node = kzalloc(sizeof(*node), GFP_KERNEL);
	if (node == NULL)
		return NULL;
	binder_stats_created(VDBINDER_STAT_NODE);
	rb_link_node(&node->rb_node, parent, p);
	rb_insert_color(&node->rb_node, &proc->nodes);
	node->debug_id = ++binder_last_id;
	node->proc = proc;
	node->ptr = ptr;
	node->cookie = cookie;
	node->work.type = VDBINDER_WORK_NODE;
	INIT_LIST_HEAD(&node->work.entry);
	INIT_LIST_HEAD(&node->async_todo);
	vdbinder_debug(VDBINDER_DEBUG_INTERNAL_REFS,
		     "binder: %d:%d node %d u%p c%p created\n",
		     proc->pid, current->pid, node->debug_id,
		     node->ptr, node->cookie);
	return node;
}

static int binder_inc_node(struct vdbinder_node *node, int strong, int internal,
			   struct list_head *target_list)
{
	if (strong) {
		if (internal) {
			if (target_list == NULL &&
			    node->internal_strong_refs == 0 &&
			    !(node == binder_context_mgr_node &&
			    node->has_strong_ref)) {
				printk(KERN_ERR "binder: invalid inc strong "
					"node for %d\n", node->debug_id);
				return -EINVAL;
			}
			node->internal_strong_refs++;
		} else
			node->local_strong_refs++;
		if (!node->has_strong_ref && target_list) {
			list_del_init(&node->work.entry);
			list_add_tail(&node->work.entry, target_list);
		}
	} else {
		if (!internal)
			node->local_weak_refs++;
		if (!node->has_weak_ref && list_empty(&node->work.entry)) {
			if (target_list == NULL) {
				printk(KERN_ERR "binder: invalid inc weak node "
					"for %d\n", node->debug_id);
				return -EINVAL;
			}
			list_add_tail(&node->work.entry, target_list);
		}
	}
	return 0;
}

static int binder_dec_node(struct vdbinder_node *node, int strong, int internal)
{
	if (strong) {
		if (internal)
			node->internal_strong_refs--;
		else
			node->local_strong_refs--;
		if (node->local_strong_refs || node->internal_strong_refs)
			return 0;
	} else {
		if (!internal)
			node->local_weak_refs--;
		if (node->local_weak_refs || !hlist_empty(&node->refs))
			return 0;
	}
	if (node->proc && (node->has_strong_ref || node->has_weak_ref)) {
		if (list_empty(&node->work.entry)) {
			list_add_tail(&node->work.entry, &node->proc->todo);
			wake_up_interruptible(&node->proc->wait);
		}
	} else {
		if (hlist_empty(&node->refs) && !node->local_strong_refs &&
		    !node->local_weak_refs) {
			list_del_init(&node->work.entry);
			if (node->proc) {
				rb_erase(&node->rb_node, &node->proc->nodes);
				vdbinder_debug(VDBINDER_DEBUG_INTERNAL_REFS,
					     "binder: refless node %d deleted\n",
					     node->debug_id);
			} else {
				hlist_del(&node->dead_node);
				vdbinder_debug(VDBINDER_DEBUG_INTERNAL_REFS,
					     "binder: dead node %d deleted\n",
					     node->debug_id);
			}
			kfree(node);
			binder_stats_deleted(VDBINDER_STAT_NODE);
		}
	}

	return 0;
}


static struct vdbinder_ref *binder_get_ref(struct vdbinder_proc *proc,
					 uint32_t desc)
{
	struct rb_node *n = proc->refs_by_desc.rb_node;
	struct vdbinder_ref *ref;

	while (n) {
		ref = rb_entry(n, struct vdbinder_ref, rb_node_desc);

		if (desc < ref->desc)
			n = n->rb_left;
		else if (desc > ref->desc)
			n = n->rb_right;
		else
			return ref;
	}
	return NULL;
}

static struct vdbinder_ref *binder_get_ref_for_node(struct vdbinder_proc *proc,
						  struct vdbinder_node *node)
{
	struct rb_node *n;
	struct rb_node **p = &proc->refs_by_node.rb_node;
	struct rb_node *parent = NULL;
	struct vdbinder_ref *ref, *new_ref;

	while (*p) {
		parent = *p;
		ref = rb_entry(parent, struct vdbinder_ref, rb_node_node);

		if (node < ref->node)
			p = &(*p)->rb_left;
		else if (node > ref->node)
			p = &(*p)->rb_right;
		else
			return ref;
	}
	new_ref = kzalloc(sizeof(*ref), GFP_KERNEL);
	if (new_ref == NULL)
		return NULL;
	binder_stats_created(VDBINDER_STAT_REF);
	new_ref->debug_id = ++binder_last_id;
	new_ref->proc = proc;
	new_ref->node = node;
	rb_link_node(&new_ref->rb_node_node, parent, p);
	rb_insert_color(&new_ref->rb_node_node, &proc->refs_by_node);

	new_ref->desc = (node == binder_context_mgr_node) ? 0 : 1;
	for (n = rb_first(&proc->refs_by_desc); n != NULL; n = rb_next(n)) {
		ref = rb_entry(n, struct vdbinder_ref, rb_node_desc);
		if (ref->desc > new_ref->desc)
			break;
		new_ref->desc = ref->desc + 1;
	}

	p = &proc->refs_by_desc.rb_node;
	while (*p) {
		parent = *p;
		ref = rb_entry(parent, struct vdbinder_ref, rb_node_desc);

		if (new_ref->desc < ref->desc)
			p = &(*p)->rb_left;
		else if (new_ref->desc > ref->desc)
			p = &(*p)->rb_right;
		else
			BUG();
	}
	rb_link_node(&new_ref->rb_node_desc, parent, p);
	rb_insert_color(&new_ref->rb_node_desc, &proc->refs_by_desc);
	if (node) {
		hlist_add_head(&new_ref->node_entry, &node->refs);

		vdbinder_debug(VDBINDER_DEBUG_INTERNAL_REFS,
			     "binder: %d new ref %d desc %d for "
			     "node %d\n", proc->pid, new_ref->debug_id,
			     new_ref->desc, node->debug_id);
	} else {
		vdbinder_debug(VDBINDER_DEBUG_INTERNAL_REFS,
			     "binder: %d new ref %d desc %d for "
			     "dead node\n", proc->pid, new_ref->debug_id,
			      new_ref->desc);
	}
	return new_ref;
}

static void binder_delete_ref(struct vdbinder_ref *ref)
{
	vdbinder_debug(VDBINDER_DEBUG_INTERNAL_REFS,
		     "binder: %d delete ref %d desc %d for "
		     "node %d\n", ref->proc->pid, ref->debug_id,
		     ref->desc, ref->node->debug_id);

	rb_erase(&ref->rb_node_desc, &ref->proc->refs_by_desc);
	rb_erase(&ref->rb_node_node, &ref->proc->refs_by_node);
	if (ref->strong)
		binder_dec_node(ref->node, 1, 1);
	hlist_del(&ref->node_entry);
	binder_dec_node(ref->node, 0, 1);
	if (ref->death) {
		vdbinder_debug(VDBINDER_DEBUG_DEAD_BINDER,
			     "binder: %d delete ref %d desc %d "
			     "has death notification\n", ref->proc->pid,
			     ref->debug_id, ref->desc);
		list_del(&ref->death->work.entry);
		kfree(ref->death);
		binder_stats_deleted(VDBINDER_STAT_DEATH);
	}
	kfree(ref);
	binder_stats_deleted(VDBINDER_STAT_REF);
}

static int binder_inc_ref(struct vdbinder_ref *ref, int strong,
			  struct list_head *target_list)
{
	int ret;
	if (strong) {
		if (ref->strong == 0) {
			ret = binder_inc_node(ref->node, 1, 1, target_list);
			if (ret)
				return ret;
		}
		ref->strong++;
	} else {
		if (ref->weak == 0) {
			ret = binder_inc_node(ref->node, 0, 1, target_list);
			if (ret)
				return ret;
		}
		ref->weak++;
	}
	return 0;
}


static int binder_dec_ref(struct vdbinder_ref *ref, int strong)
{
	if (strong) {
		if (ref->strong == 0) {
			vdbinder_user_error("binder: %d invalid dec strong, "
					  "ref %d desc %d s %d w %d\n",
					  ref->proc->pid, ref->debug_id,
					  ref->desc, ref->strong, ref->weak);
			return -EINVAL;
		}
		ref->strong--;
		if (ref->strong == 0) {
			int ret;
			ret = binder_dec_node(ref->node, strong, 1);
			if (ret)
				return ret;
		}
	} else {
		if (ref->weak == 0) {
			vdbinder_user_error("binder: %d invalid dec weak, "
					  "ref %d desc %d s %d w %d\n",
					  ref->proc->pid, ref->debug_id,
					  ref->desc, ref->strong, ref->weak);
			return -EINVAL;
		}
		ref->weak--;
	}
	if (ref->strong == 0 && ref->weak == 0)
		binder_delete_ref(ref);
	return 0;
}

static void binder_pop_transaction(struct vdbinder_thread *target_thread,
				   struct vdbinder_transaction *t)
{
	if (target_thread) {
		BUG_ON(target_thread->transaction_stack != t);
		BUG_ON(target_thread->transaction_stack->from != target_thread);
		target_thread->transaction_stack =
			target_thread->transaction_stack->from_parent;
		t->from = NULL;
	}
	t->need_reply = 0;
	if (t->buffer)
		t->buffer->transaction = NULL;
	kfree(t);
	binder_stats_deleted(VDBINDER_STAT_TRANSACTION);
}

static void binder_send_failed_reply(struct vdbinder_transaction *t,
				     uint32_t error_code)
{
	struct vdbinder_thread *target_thread;
	BUG_ON(t->flags & VD_TF_ONE_WAY);
	while (1) {
		target_thread = t->from;
		if (target_thread) {
			if (target_thread->return_error != VD_BR_OK &&
			   target_thread->return_error2 == VD_BR_OK) {
				target_thread->return_error2 =
					target_thread->return_error;
				target_thread->return_error = VD_BR_OK;
			}
			if (target_thread->return_error == VD_BR_OK) {
				vdbinder_debug(VDBINDER_DEBUG_FAILED_TRANSACTION,
					     "binder: send failed reply for "
					     "transaction %d to %d:%d\n",
					      t->debug_id, target_thread->proc->pid,
					      target_thread->pid);

				binder_pop_transaction(target_thread, t);
				target_thread->return_error = error_code;
				wake_up_interruptible(&target_thread->wait);
			} else {
				printk(KERN_ERR "binder: reply failed, target "
					"thread, %d:%d, has error code %d "
					"already\n", target_thread->proc->pid,
					target_thread->pid,
					target_thread->return_error);
			}
			return;
		} else {
			struct vdbinder_transaction *next = t->from_parent;

			vdbinder_debug(VDBINDER_DEBUG_FAILED_TRANSACTION,
				     "binder: send failed reply "
				     "for transaction %d, target dead\n",
				     t->debug_id);

			binder_pop_transaction(target_thread, t);
			if (next == NULL) {
				vdbinder_debug(VDBINDER_DEBUG_DEAD_BINDER,
					     "binder: reply failed,"
					     " no target thread at root\n");
				return;
			}
			t = next;
			vdbinder_debug(VDBINDER_DEBUG_DEAD_BINDER,
				     "binder: reply failed, no target "
				     "thread -- retry %d\n", t->debug_id);
		}
	}
}

static void binder_transaction_buffer_release(struct vdbinder_proc *proc,
					      struct vdbinder_buffer *buffer,
					      size_t *failed_at)
{
	size_t *offp, *off_end;
	int debug_id = buffer->debug_id;

	vdbinder_debug(VDBINDER_DEBUG_TRANSACTION,
		     "binder: %d buffer release %d, size %zd-%zd, failed at %p\n",
		     proc->pid, buffer->debug_id,
		     buffer->data_size, buffer->offsets_size, failed_at);

	if (buffer->target_node)
		binder_dec_node(buffer->target_node, 1, 0);

	offp = (size_t *)(buffer->data + ALIGN(buffer->data_size, sizeof(void *)));
	if (failed_at)
		off_end = failed_at;
	else
		off_end = (void *)offp + buffer->offsets_size;
	for (; offp < off_end; offp++) {
		struct flat_vdbinder_object *fp;
		if (*offp > buffer->data_size - sizeof(*fp) ||
		    buffer->data_size < sizeof(*fp) ||
		    !IS_ALIGNED(*offp, sizeof(void *))) {
			printk(KERN_ERR "binder: transaction release %d bad"
					"offset %zd, size %zd\n", debug_id,
					*offp, buffer->data_size);
			continue;
		}
		fp = (struct flat_vdbinder_object *)(buffer->data + *offp);
		switch (fp->type) {
		case VDBINDER_TYPE_BINDER:
		case VDBINDER_TYPE_WEAK_BINDER: {
			struct vdbinder_node *node = binder_get_node(proc, fp->binder);
			if (node == NULL) {
				printk(KERN_ERR "binder: transaction release %d"
				       " bad node %p\n", debug_id, fp->binder);
				break;
			}
			vdbinder_debug(VDBINDER_DEBUG_TRANSACTION,
				     "        node %d u%p\n",
				     node->debug_id, node->ptr);
			binder_dec_node(node, fp->type == VDBINDER_TYPE_BINDER, 0);
		} break;
		case VDBINDER_TYPE_HANDLE:
		case VDBINDER_TYPE_WEAK_HANDLE: {
			struct vdbinder_ref *ref = binder_get_ref(proc, fp->handle);
			if (ref == NULL) {
				printk(KERN_ERR "binder: transaction release %d"
				       " bad handle %ld\n", debug_id,
				       fp->handle);
				break;
			}
			vdbinder_debug(VDBINDER_DEBUG_TRANSACTION,
				     "        ref %d desc %d (node %d)\n",
				     ref->debug_id, ref->desc, ref->node->debug_id);
			binder_dec_ref(ref, fp->type == VDBINDER_TYPE_HANDLE);
		} break;

		case VDBINDER_TYPE_FD:
			vdbinder_debug(VDBINDER_DEBUG_TRANSACTION,
				     "        fd %ld\n", fp->handle);
			if (failed_at)
				task_close_fd(proc, fp->handle);
			break;
		case VDBINDER_TYPE_WAIT_BINDER:
			break;
		default:
			printk(KERN_ERR "binder: transaction release %d bad "
			       "object type %lx\n", debug_id, fp->type);
			break;
		}
	}
}

static void vdbinder_transaction(struct vdbinder_proc *proc,
			       struct vdbinder_thread *thread,
			       struct vdbinder_transaction_data *tr,
				int reply, int is_wait)
{
	struct vdbinder_transaction *t;
	struct vdbinder_work *tcomplete;
	size_t *offp, *off_end;
	struct vdbinder_proc *target_proc;
	struct vdbinder_thread *target_thread = NULL;
	struct vdbinder_node *target_node = NULL;
	struct list_head *target_list;
	wait_queue_head_t *target_wait;
	struct vdbinder_transaction *in_reply_to = NULL;
	struct vdbinder_transaction_log_entry *e;
	uint32_t return_error;

	static unsigned long resume;
	static unsigned long nr_shown;
	static unsigned long nr_unshown;

	e = binder_transaction_log_add(&vdbinder_transaction_log);
	e->call_type = reply ? 2 : !!(tr->flags & VD_TF_ONE_WAY);
	e->from_proc = proc->pid;
	e->from_thread = thread->pid;
	e->target_handle = tr->target.handle;
	e->data_size = tr->data_size;
	e->offsets_size = tr->offsets_size;

	if (reply) {
		in_reply_to = thread->transaction_stack;
		if (in_reply_to == NULL) {
			vdbinder_user_error("binder: %d:%d got reply transaction "
					  "with no transaction stack\n",
					  proc->pid, thread->pid);
			return_error = VD_BR_FAILED_REPLY;
			goto err_empty_call_stack;
		}
		binder_set_nice(in_reply_to->saved_priority);
		if (in_reply_to->to_thread != thread) {
			vdbinder_user_error("binder: %d:%d got reply transaction "
				"with bad transaction stack,"
				" transaction %d has target %d:%d\n",
				proc->pid, thread->pid, in_reply_to->debug_id,
				in_reply_to->to_proc ?
				in_reply_to->to_proc->pid : 0,
				in_reply_to->to_thread ?
				in_reply_to->to_thread->pid : 0);
			return_error = VD_BR_FAILED_REPLY;
			in_reply_to = NULL;
			goto err_bad_call_stack;
		}
		thread->transaction_stack = in_reply_to->to_parent;
		target_thread = in_reply_to->from;
		if (target_thread == NULL) {
			return_error = VD_BR_DEAD_REPLY;
			goto err_dead_binder;
		}
		if (target_thread->transaction_stack != in_reply_to) {
			vdbinder_user_error("binder: %d:%d got reply transaction "
				"with bad target transaction stack %d, "
				"expected %d\n",
				proc->pid, thread->pid,
				target_thread->transaction_stack ?
				target_thread->transaction_stack->debug_id : 0,
				in_reply_to->debug_id);
			return_error = VD_BR_FAILED_REPLY;
			in_reply_to = NULL;
			target_thread = NULL;
			goto err_dead_binder;
		}
		target_proc = target_thread->proc;
	} else {
		if (tr->target.handle) {
			struct vdbinder_ref *ref;
			ref = binder_get_ref(proc, tr->target.handle);
			if (ref == NULL) {
				vdbinder_user_error("binder: %d:%d got "
					"transaction to invalid handle\n",
					proc->pid, thread->pid);
				return_error = VD_BR_FAILED_REPLY;
				goto err_invalid_target_handle;
			}
			target_node = ref->node;
		} else {
			target_node = binder_context_mgr_node;
			if (target_node == NULL) {
				if (is_wait) {
					int ret;
					mutex_unlock(&binder_lock);
					ret = wait_event_interruptible(
					     binder_context_mgr_wait,
					     (binder_context_mgr_node != NULL));
					mutex_lock(&binder_lock);
					if (ret != 0) {
						return_error = VD_BR_DEAD_REPLY;
						goto err_no_context_mgr_node;
					}
					target_node = binder_context_mgr_node;
				} else {
					return_error = VD_BR_DEAD_REPLY;
					goto err_no_context_mgr_node;
				}
			}
		}
		if (is_wait)
			thread->looper |= VDBINDER_LOOPER_STATE_THREAD_WAIT;
		e->to_node = target_node->debug_id;
		target_proc = target_node->proc;
		if (target_proc == NULL) {
			return_error = VD_BR_DEAD_REPLY;
			goto err_dead_binder;
		}
		if (!(tr->flags & VD_TF_ONE_WAY) && thread->transaction_stack) {
			struct vdbinder_transaction *tmp;
			tmp = thread->transaction_stack;
			if (tmp->to_thread != thread) {
				vdbinder_user_error("binder: %d:%d got new "
					"transaction with bad transaction stack"
					", transaction %d has target %d:%d\n",
					proc->pid, thread->pid, tmp->debug_id,
					tmp->to_proc ? tmp->to_proc->pid : 0,
					tmp->to_thread ?
					tmp->to_thread->pid : 0);
				return_error = VD_BR_FAILED_REPLY;
				goto err_bad_call_stack;
			}
			while (tmp) {
				if (tmp->from && tmp->from->proc == target_proc)
					target_thread = tmp->from;
				tmp = tmp->from_parent;
			}
		}
	}
	if (target_thread) {
		e->to_thread = target_thread->pid;
		target_list = &target_thread->todo;
		target_wait = &target_thread->wait;
	} else {
		target_list = &target_proc->todo;
		target_wait = &target_proc->wait;
	}
	e->to_proc = target_proc->pid;

	/* TODO: reuse incoming transaction for reply */
	t = kzalloc(sizeof(*t), GFP_KERNEL);
	if (t == NULL) {
		return_error = VD_BR_FAILED_REPLY;
		goto err_alloc_t_failed;
	}
	binder_stats_created(VDBINDER_STAT_TRANSACTION);

	tcomplete = kzalloc(sizeof(*tcomplete), GFP_KERNEL);
	if (tcomplete == NULL) {
		return_error = VD_BR_FAILED_REPLY;
		goto err_alloc_tcomplete_failed;
	}
	binder_stats_created(VDBINDER_STAT_TRANSACTION_COMPLETE);

	t->debug_id = ++binder_last_id;
	e->debug_id = t->debug_id;

	if (reply)
		vdbinder_debug(VDBINDER_DEBUG_TRANSACTION,
			     "binder: %d:%d VD_BC_REPLY %d -> %d:%d, "
			     "data %p-%p size %zd-%zd\n",
			     proc->pid, thread->pid, t->debug_id,
			     target_proc->pid, target_thread->pid,
			     tr->data.ptr.buffer, tr->data.ptr.offsets,
			     tr->data_size, tr->offsets_size);
	else
		vdbinder_debug(VDBINDER_DEBUG_TRANSACTION,
			     "binder: %d:%d VD_BC_TRANSACTION %d -> "
			     "%d - node %d, data %p-%p size %zd-%zd\n",
			     proc->pid, thread->pid, t->debug_id,
			     target_proc->pid, target_node->debug_id,
			     tr->data.ptr.buffer, tr->data.ptr.offsets,
			     tr->data_size, tr->offsets_size);

	if (!reply && !(tr->flags & VD_TF_ONE_WAY))
		t->from = thread;
	else
		t->from = NULL;
	t->sender_euid = proc->tsk->cred->euid;
	t->to_proc = target_proc;
	t->to_thread = target_thread;
	t->code = tr->code;
	t->flags = tr->flags;
	t->isEnc = tr->isEnc;
	t->isShrd = tr->isShrd;
	t->priority = task_nice(current);
	t->buffer = binder_alloc_buf(target_proc, tr->data_size,
		tr->offsets_size, !reply && (t->flags & VD_TF_ONE_WAY));
	if (t->buffer == NULL) {
		return_error = VD_BR_FAILED_REPLY_NO_MEM;
		goto err_binder_alloc_buf_failed;
	}
	t->buffer->allow_user_free = 0;
	t->buffer->debug_id = t->debug_id;
	t->buffer->transaction = t;
	t->buffer->target_node = target_node;
	if (target_node)
		binder_inc_node(target_node, 1, 0, NULL);

	offp = (size_t *)(t->buffer->data + ALIGN(tr->data_size, sizeof(void *)));

	if (copy_from_user(t->buffer->data, tr->data.ptr.buffer, tr->data_size)) {
		vdbinder_user_error("binder: %d:%d got transaction with invalid "
			"data ptr\n", proc->pid, thread->pid);
		return_error = VD_BR_FAILED_REPLY;
		goto err_copy_data_failed;
	}
	if (copy_from_user(offp, tr->data.ptr.offsets, tr->offsets_size)) {
		vdbinder_user_error("binder: %d:%d got transaction with invalid "
			"offsets ptr\n", proc->pid, thread->pid);
		return_error = VD_BR_FAILED_REPLY;
		goto err_copy_data_failed;
	}
	if (!IS_ALIGNED(tr->offsets_size, sizeof(size_t))) {
		vdbinder_user_error("binder: %d:%d got transaction with "
			"invalid offsets size, %zd\n",
			proc->pid, thread->pid, tr->offsets_size);
		return_error = VD_BR_FAILED_REPLY;
		goto err_bad_offset;
	}
	off_end = (void *)offp + tr->offsets_size;
	for (; offp < off_end; offp++) {
		struct flat_vdbinder_object *fp;
		if (*offp > t->buffer->data_size - sizeof(*fp) ||
		    t->buffer->data_size < sizeof(*fp) ||
		    !IS_ALIGNED(*offp, sizeof(void *))) {
			vdbinder_user_error("binder: %d:%d got transaction with "
				"invalid offset, %zd\n",
				proc->pid, thread->pid, *offp);
			return_error = VD_BR_FAILED_REPLY;
			goto err_bad_offset;
		}
		fp = (struct flat_vdbinder_object *)(t->buffer->data + *offp);
		switch (fp->type) {

		case VDBINDER_TYPE_WAIT_BINDER: {
			struct vdbinder_service_wait_work *sw;
			sw = kzalloc(sizeof(*sw), GFP_KERNEL);
			if (!sw) {
				return_error = VD_BR_FAILED_REPLY;
				goto err_copy_data_failed;
			}
			sw->t = t;
			sw->offset = offp;
			sw->index = fp->handle;
			list_add(&sw->wait_entry, &vdbinder_service_wait_list);
			binder_pop_transaction(target_thread, in_reply_to);
			tcomplete->type = VDBINDER_WORK_TRANSACTION_COMPLETE;
			list_add_tail(&tcomplete->entry, &thread->todo);
			return;
		}
		case VDBINDER_TYPE_BINDER:
		case VDBINDER_TYPE_WEAK_BINDER: {
			struct vdbinder_ref *ref;
			struct vdbinder_node *node = binder_get_node(proc, fp->binder);
			if (node == NULL) {
				node = binder_new_node(proc, fp->binder, fp->cookie);
				if (node == NULL) {
					return_error = VD_BR_FAILED_REPLY;
					goto err_binder_new_node_failed;
				}
				node->min_priority = fp->flags & VD_FLAT_BINDER_FLAG_PRIORITY_MASK;
				node->accept_fds = !!(fp->flags & VD_FLAT_BINDER_FLAG_ACCEPTS_FDS);
			}
			if (fp->cookie != node->cookie) {
				vdbinder_user_error("binder: %d:%d sending u%p "
					"node %d, cookie mismatch %p != %p\n",
					proc->pid, thread->pid,
					fp->binder, node->debug_id,
					fp->cookie, node->cookie);
				return_error = VD_BR_FAILED_REPLY;
				goto err_binder_get_ref_for_node_failed;
			}
			ref = binder_get_ref_for_node(target_proc, node);
			if (ref == NULL) {
				return_error = VD_BR_FAILED_REPLY;
				goto err_binder_get_ref_for_node_failed;
			}
			if (fp->type == VDBINDER_TYPE_BINDER)
				fp->type = VDBINDER_TYPE_HANDLE;
			else
				fp->type = VDBINDER_TYPE_WEAK_HANDLE;
			fp->handle = ref->desc;
			binder_inc_ref(ref, fp->type == VDBINDER_TYPE_HANDLE,
				       &thread->todo);

			vdbinder_debug(VDBINDER_DEBUG_TRANSACTION,
				     "        node %d u%p -> ref %d desc %d\n",
				     node->debug_id, node->ptr, ref->debug_id,
				     ref->desc);
		} break;
		case VDBINDER_TYPE_HANDLE:
		case VDBINDER_TYPE_WEAK_HANDLE: {
			struct vdbinder_ref *ref = binder_get_ref(proc, fp->handle);
			if (ref == NULL) {
				vdbinder_user_error("binder: %d:%d got "
					"transaction with invalid "
					"handle, %ld\n", proc->pid,
					thread->pid, fp->handle);
				return_error = VD_BR_FAILED_REPLY;
				goto err_binder_get_ref_failed;
			}
			if (ref->node->proc == target_proc) {
				if (fp->type == VDBINDER_TYPE_HANDLE)
					fp->type = VDBINDER_TYPE_BINDER;
				else
					fp->type = VDBINDER_TYPE_WEAK_BINDER;
				fp->binder = ref->node->ptr;
				fp->cookie = ref->node->cookie;
				binder_inc_node(ref->node, fp->type == VDBINDER_TYPE_BINDER, 0, NULL);
				vdbinder_debug(VDBINDER_DEBUG_TRANSACTION,
					     "        ref %d desc %d -> node %d u%p\n",
					     ref->debug_id, ref->desc, ref->node->debug_id,
					     ref->node->ptr);
			} else {
				struct vdbinder_ref *new_ref;
				new_ref = binder_get_ref_for_node(target_proc, ref->node);
				if (new_ref == NULL) {
					return_error = VD_BR_FAILED_REPLY;
					goto err_binder_get_ref_for_node_failed;
				}
				fp->handle = new_ref->desc;
				binder_inc_ref(new_ref, fp->type == VDBINDER_TYPE_HANDLE, NULL);
				vdbinder_debug(VDBINDER_DEBUG_TRANSACTION,
					     "        ref %d desc %d -> ref %d desc %d (node %d)\n",
					     ref->debug_id, ref->desc, new_ref->debug_id,
					     new_ref->desc, ref->node->debug_id);
			}
		} break;

		case VDBINDER_TYPE_FD: {
			int target_fd;
			struct file *file;

			if (reply) {
				if (!(in_reply_to->flags & VD_TF_ACCEPT_FDS)) {
					vdbinder_user_error("binder: %d:%d got reply with fd, %ld, but target does not allow fds\n",
						proc->pid, thread->pid, fp->handle);
					return_error = VD_BR_FAILED_REPLY;
					goto err_fd_not_allowed;
				}
			} else if (!target_node->accept_fds) {
				vdbinder_user_error("binder: %d:%d got transaction with fd, %ld, but target does not allow fds\n",
					proc->pid, thread->pid, fp->handle);
				return_error = VD_BR_FAILED_REPLY;
				goto err_fd_not_allowed;
			}

			file = fget(fp->handle);
			if (file == NULL) {
				vdbinder_user_error("binder: %d:%d got transaction with invalid fd, %ld\n",
					proc->pid, thread->pid, fp->handle);
				return_error = VD_BR_FAILED_REPLY;
				goto err_fget_failed;
			}
			target_fd = task_get_unused_fd_flags(target_proc, O_CLOEXEC);
			if (target_fd < 0) {
				fput(file);
				return_error = VD_BR_FAILED_REPLY;
				goto err_get_unused_fd_failed;
			}
			task_fd_install(target_proc, target_fd, file);
			vdbinder_debug(VDBINDER_DEBUG_TRANSACTION,
				     "        fd %ld -> %d\n", fp->handle, target_fd);
			/* TODO: fput? */
			fp->handle = target_fd;
		} break;

		default:
			vdbinder_user_error("binder: %d:%d got transactio"
				"n with invalid object type, %lx\n",
				proc->pid, thread->pid, fp->type);
			return_error = VD_BR_FAILED_REPLY;
			goto err_bad_object_type;
		}
	}
	if (reply) {
		BUG_ON(t->buffer->async_transaction != 0);
		binder_pop_transaction(target_thread, in_reply_to);
	} else if (!(t->flags & VD_TF_ONE_WAY)) {
		BUG_ON(t->buffer->async_transaction != 0);
		t->need_reply = 1;
		t->from_parent = thread->transaction_stack;
		thread->transaction_stack = t;
	} else {
		BUG_ON(target_node == NULL);
		BUG_ON(t->buffer->async_transaction != 1);
		if (target_node->has_async_transaction) {
			target_list = &target_node->async_todo;
			target_wait = NULL;
		} else
			target_node->has_async_transaction = 1;
	}
	t->work.type = VDBINDER_WORK_TRANSACTION;
#ifdef CONFIG_ORSAY_DEBUG
	t->work.owner = 0;
#endif
	list_add_tail(&t->work.entry, target_list);
	tcomplete->type = VDBINDER_WORK_TRANSACTION_COMPLETE;
	list_add_tail(&tcomplete->entry, &thread->todo);
	if (target_wait)
		wake_up_interruptible(target_wait);
	return;

err_get_unused_fd_failed:
err_fget_failed:
err_fd_not_allowed:
err_binder_get_ref_for_node_failed:
err_binder_get_ref_failed:
err_binder_new_node_failed:
err_bad_object_type:
err_bad_offset:
err_copy_data_failed:
	binder_transaction_buffer_release(target_proc, t->buffer, offp);
	t->buffer->transaction = NULL;
	binder_free_buf(target_proc, t->buffer);
err_binder_alloc_buf_failed:
	kfree(tcomplete);
	binder_stats_deleted(VDBINDER_STAT_TRANSACTION_COMPLETE);
err_alloc_tcomplete_failed:
	kfree(t);
	binder_stats_deleted(VDBINDER_STAT_TRANSACTION);
err_alloc_t_failed:
err_bad_call_stack:
err_empty_call_stack:
err_dead_binder:
err_invalid_target_handle:
err_no_context_mgr_node:

	/*
	* Allow a burst of 60 reports, then keep quiet for that minute;
	* or allow a steady drip of one report per second.
	*/
	do {
		if (nr_shown >= 60) {
			if (time_before(jiffies, resume)) {
				nr_unshown++;
				break;
			}
			if (nr_unshown) {
				printk("binder: transaction failed : %lu messages suppressed\n",
						nr_unshown);
				nr_unshown = 0;
			}
			nr_shown = 0;
		}
		if (nr_shown++ == 0)
			resume = jiffies + 60 * HZ;

		vdbinder_debug(VDBINDER_DEBUG_FAILED_TRANSACTION,
				"binder: %d:%d transaction failed %d, size %zd-%zd\n",
				proc->pid, thread->pid, return_error,
				tr->data_size, tr->offsets_size);

		{
			struct vdbinder_transaction_log_entry *fe;
			fe = binder_transaction_log_add(&binder_transaction_log_failed);
			*fe = *e;
		}
	} while (0);

	BUG_ON(thread->return_error != VD_BR_OK);
	if (in_reply_to) {
		thread->return_error = VD_BR_TRANSACTION_COMPLETE;
		binder_send_failed_reply(in_reply_to, return_error);
	} else
		thread->return_error = return_error;
}

static int binder_thread_write(struct vdbinder_proc *proc, struct vdbinder_thread *thread,
			void __user *buffer, int size, signed long *consumed)
{
	uint32_t cmd;
	void __user *ptr = buffer + *consumed;
	void __user *end = buffer + size;

	while (ptr < end && thread->return_error == VD_BR_OK) {
		if (get_user(cmd, (uint32_t __user *)ptr))
			return -EFAULT;
		ptr += sizeof(uint32_t);
		if (_IOC_NR(cmd) < ARRAY_SIZE(vdbinder_stats.bc)) {
			vdbinder_stats.bc[_IOC_NR(cmd)]++;
			proc->stats.bc[_IOC_NR(cmd)]++;
			thread->stats.bc[_IOC_NR(cmd)]++;
		}
		switch (cmd) {
		case VD_BC_BINDER_DEAD: {
			void __user *node_ptr;
			struct vdbinder_node *node;
			struct hlist_node *pos;
			int incoming_refs = 0;
			if (get_user(node_ptr, (void * __user *)ptr))
				return -EFAULT;
			ptr += sizeof(uint32_t);
			node = binder_get_node(proc, node_ptr);
			if (node == NULL)
				return -EFAULT;

			rb_erase(&node->rb_node, &proc->nodes);
			list_del_init(&node->work.entry);

			if (hlist_empty(&node->refs)) {
				kfree(node);
				binder_stats_deleted(VDBINDER_STAT_NODE);
			} else {
				struct vdbinder_ref *ref;
				int death = 0;

				node->proc = NULL;
				hlist_add_head(&node->dead_node,
						&binder_dead_nodes);

				hlist_for_each_entry(ref, pos,
					&node->refs, node_entry) {
					incoming_refs++;
					if (ref->death) {
						death++;
						if (list_empty(
						&ref->death->work.entry)) {
							ref->death->work.type =
							VDBINDER_WORK_DEAD_BINDER;

							list_add_tail(
							&ref->death->work.entry,
							&ref->proc->todo);

							wake_up_interruptible(
							 &ref->proc->wait);
						} else
							BUG();
					}
				}
				vdbinder_debug(VDBINDER_DEBUG_DEAD_BINDER,
						"binder: node %d now dead, "
						"refs %d, death %d\n",
						node->debug_id,
						incoming_refs, death);
			}
		}
		break;
		case VD_BC_INCREFS:
		case VD_BC_ACQUIRE:
		case VD_BC_RELEASE:
		case VD_BC_DECREFS: {
			uint32_t target;
			struct vdbinder_ref *ref;
			const char *debug_string;

			if (get_user(target, (uint32_t __user *)ptr))
				return -EFAULT;
			ptr += sizeof(uint32_t);
			if (target == 0 && binder_context_mgr_node &&
			    (cmd == VD_BC_INCREFS || cmd == VD_BC_ACQUIRE)) {
				ref = binder_get_ref_for_node(proc,
					       binder_context_mgr_node);
				if (ref->desc != target) {
					vdbinder_user_error("binder: %d:"
						"%d tried to acquire "
						"reference to desc 0, "
						"got %d instead\n",
						proc->pid, thread->pid,
						ref->desc);
				}
			} else
				ref = binder_get_ref(proc, target);
			if (ref == NULL) {
				vdbinder_user_error("binder: %d:%d refcou"
					"nt change on invalid ref %d\n",
					proc->pid, thread->pid, target);
				break;
			}
			switch (cmd) {
			case VD_BC_INCREFS:
				debug_string = "IncRefs";
				binder_inc_ref(ref, 0, NULL);
				break;
			case VD_BC_ACQUIRE:
				debug_string = "Acquire";
				binder_inc_ref(ref, 1, NULL);
				break;
			case VD_BC_RELEASE:
				debug_string = "Release";
				binder_dec_ref(ref, 1);
				break;
			case VD_BC_DECREFS:
			default:
				debug_string = "DecRefs";
				binder_dec_ref(ref, 0);
				break;
			}
			vdbinder_debug(VDBINDER_DEBUG_USER_REFS,
				     "binder: %d:%d %s ref %d desc %d s %d w %d for node %d\n",
				     proc->pid, thread->pid, debug_string, ref->debug_id,
				     ref->desc, ref->strong, ref->weak, ref->node->debug_id);
			break;
		}
		case VD_BC_INCREFS_DONE:
		case VD_BC_ACQUIRE_DONE: {
			void __user *node_ptr;
			void *cookie;
			struct vdbinder_node *node;

			if (get_user(node_ptr, (void * __user *)ptr))
				return -EFAULT;
			ptr += sizeof(void *);
			if (get_user(cookie, (void * __user *)ptr))
				return -EFAULT;
			ptr += sizeof(void *);
			node = binder_get_node(proc, node_ptr);
			if (node == NULL) {
				vdbinder_user_error("binder: %d:%d "
					"%s u%p no match\n",
					proc->pid, thread->pid,
					cmd == VD_BC_INCREFS_DONE ?
					"VD_BC_INCREFS_DONE" :
					"VD_BC_ACQUIRE_DONE",
					node_ptr);
				break;
			}
			if (cookie != node->cookie) {
				vdbinder_user_error("binder: %d:%d %s u%p node %d"
					" cookie mismatch %p != %p\n",
					proc->pid, thread->pid,
					cmd == VD_BC_INCREFS_DONE ?
					"VD_BC_INCREFS_DONE" : "VD_BC_ACQUIRE_DONE",
					node_ptr, node->debug_id,
					cookie, node->cookie);
				break;
			}
			if (cmd == VD_BC_ACQUIRE_DONE) {
				if (node->pending_strong_ref == 0) {
					vdbinder_user_error("binder: %d:%d "
						"VD_BC_ACQUIRE_DONE node %d has "
						"no pending acquire request\n",
						proc->pid, thread->pid,
						node->debug_id);
					break;
				}
				node->pending_strong_ref = 0;
			} else {
				if (node->pending_weak_ref == 0) {
					vdbinder_user_error("binder: %d:%d "
						"VD_BC_INCREFS_DONE node %d has "
						"no pending increfs request\n",
						proc->pid, thread->pid,
						node->debug_id);
					break;
				}
				node->pending_weak_ref = 0;
			}
			binder_dec_node(node, cmd == VD_BC_ACQUIRE_DONE, 0);
			vdbinder_debug(VDBINDER_DEBUG_USER_REFS,
				     "binder: %d:%d %s node %d ls %d lw %d\n",
				     proc->pid, thread->pid,
				     cmd == VD_BC_INCREFS_DONE ? "VD_BC_INCREFS_DONE" : "VD_BC_ACQUIRE_DONE",
				     node->debug_id, node->local_strong_refs, node->local_weak_refs);
			break;
		}
		case VD_BC_ATTEMPT_ACQUIRE:
			printk(KERN_ERR "binder: VD_BC_ATTEMPT_ACQUIRE not supported\n");
			return -EINVAL;
		case VD_BC_ACQUIRE_RESULT:
			printk(KERN_ERR "binder: VD_BC_ACQUIRE_RESULT not supported\n");
			return -EINVAL;

		case VD_BC_FREE_BUFFER: {
			void __user *data_ptr;
			struct vdbinder_buffer *buffer;

			if (get_user(data_ptr, (void * __user *)ptr))
				return -EFAULT;
			ptr += sizeof(void *);

			buffer = binder_buffer_lookup(proc, data_ptr);
			if (buffer == NULL) {
				vdbinder_user_error("binder: %d:%d "
					"VD_BC_FREE_BUFFER u%p no match\n",
					proc->pid, thread->pid, data_ptr);
				break;
			}
			if (!buffer->allow_user_free) {
				vdbinder_user_error("binder: %d:%d "
					"VD_BC_FREE_BUFFER u%p matched "
					"unreturned buffer\n",
					proc->pid, thread->pid, data_ptr);
				break;
			}
			vdbinder_debug(VDBINDER_DEBUG_FREE_BUFFER,
				     "binder: %d:%d VD_BC_FREE_BUFFER u%p found buffer %d for %s transaction\n",
				     proc->pid, thread->pid, data_ptr, buffer->debug_id,
				     buffer->transaction ? "active" : "finished");

			if (buffer->transaction) {
				buffer->transaction->buffer = NULL;
				buffer->transaction = NULL;
			}
			if (buffer->async_transaction && buffer->target_node) {
				BUG_ON(!buffer->target_node->has_async_transaction);
				if (list_empty(&buffer->target_node->async_todo))
					buffer->target_node->has_async_transaction = 0;
				else
					list_move_tail(buffer->target_node->async_todo.next, &thread->todo);
			}
			binder_transaction_buffer_release(proc, buffer, NULL);
			binder_free_buf(proc, buffer);
			break;
		}

		case VD_BC_TRANSACTION:
		case VD_BC_TRANSACTION_WAIT:
		case VD_BC_REPLY: {
			struct vdbinder_transaction_data tr;

			if (copy_from_user(&tr, ptr, sizeof(tr)))
				return -EFAULT;
			ptr += sizeof(tr);
			vdbinder_transaction(proc, thread, &tr,
					cmd == VD_BC_REPLY,
					cmd == VD_BC_TRANSACTION_WAIT);
			break;
		}

		case VD_BC_REGISTER_LOOPER:
			vdbinder_debug(VDBINDER_DEBUG_THREADS,
				     "binder: %d:%d VD_BC_REGISTER_LOOPER\n",
				     proc->pid, thread->pid);
			if (thread->looper & VDBINDER_LOOPER_STATE_ENTERED) {
				thread->looper |= VDBINDER_LOOPER_STATE_INVALID;
				vdbinder_user_error("binder: %d:%d ERROR:"
					" VD_BC_REGISTER_LOOPER called "
					"after VD_BC_ENTER_LOOPER\n",
					proc->pid, thread->pid);
			} else if (proc->requested_threads == 0) {
				thread->looper |= VDBINDER_LOOPER_STATE_INVALID;
				vdbinder_user_error("binder: %d:%d ERROR:"
					" VD_BC_REGISTER_LOOPER called "
					"without request\n",
					proc->pid, thread->pid);
			} else {
				proc->requested_threads--;
				proc->requested_threads_started++;
			}
			thread->looper |= VDBINDER_LOOPER_STATE_REGISTERED;
			break;
		case VD_BC_ENTER_LOOPER:
			vdbinder_debug(VDBINDER_DEBUG_THREADS,
				     "binder: %d:%d VD_BC_ENTER_LOOPER\n",
				     proc->pid, thread->pid);
			if (thread->looper & VDBINDER_LOOPER_STATE_REGISTERED) {
				thread->looper |= VDBINDER_LOOPER_STATE_INVALID;
				vdbinder_user_error("binder: %d:%d ERROR:"
					" VD_BC_ENTER_LOOPER called after "
					"VD_BC_REGISTER_LOOPER\n",
					proc->pid, thread->pid);
			}
			thread->looper |= VDBINDER_LOOPER_STATE_ENTERED;
			break;
		case VD_BC_EXIT_LOOPER:
			vdbinder_debug(VDBINDER_DEBUG_THREADS,
				     "binder: %d:%d VD_BC_EXIT_LOOPER\n",
				     proc->pid, thread->pid);
			thread->looper |= VDBINDER_LOOPER_STATE_EXITED;
			break;

		case VD_BC_REQUEST_DEATH_NOTIFICATION:
		case VD_BC_CLEAR_DEATH_NOTIFICATION: {
			uint32_t target;
			void __user *cookie;
			struct vdbinder_ref *ref;
			struct vdbinder_ref_death *death;

			if (get_user(target, (uint32_t __user *)ptr))
				return -EFAULT;
			ptr += sizeof(uint32_t);
			if (get_user(cookie, (void __user * __user *)ptr))
				return -EFAULT;
			ptr += sizeof(void *);
			ref = binder_get_ref(proc, target);
			if (ref == NULL) {
				vdbinder_user_error("binder: %d:%d %s "
					"invalid ref %d\n",
					proc->pid, thread->pid,
					cmd == VD_BC_REQUEST_DEATH_NOTIFICATION ?
					"VD_BC_REQUEST_DEATH_NOTIFICATION" :
					"VD_BC_CLEAR_DEATH_NOTIFICATION",
					target);
				break;
			}

			vdbinder_debug(VDBINDER_DEBUG_DEATH_NOTIFICATION,
				     "binder: %d:%d %s %p ref %d desc %d s %d w %d for node %d\n",
				     proc->pid, thread->pid,
				     cmd == VD_BC_REQUEST_DEATH_NOTIFICATION ?
				     "VD_BC_REQUEST_DEATH_NOTIFICATION" :
				     "VD_BC_CLEAR_DEATH_NOTIFICATION",
				     cookie, ref->debug_id, ref->desc,
				     ref->strong, ref->weak, ref->node->debug_id);

			if (cmd == VD_BC_REQUEST_DEATH_NOTIFICATION) {
				if (ref->death) {
					vdbinder_user_error("binder: %d:%"
						"d VD_BC_REQUEST_DEATH_NOTI"
						"FICATION death notific"
						"ation already set\n",
						proc->pid, thread->pid);
					break;
				}
				death = kzalloc(sizeof(*death), GFP_KERNEL);
				if (death == NULL) {
					thread->return_error = VD_BR_ERROR;
					vdbinder_debug(VDBINDER_DEBUG_FAILED_TRANSACTION,
						     "binder: %d:%d "
						     "VD_BC_REQUEST_DEATH_NOTIFICATION failed\n",
						     proc->pid, thread->pid);
					break;
				}
				binder_stats_created(VDBINDER_STAT_DEATH);
				INIT_LIST_HEAD(&death->work.entry);
				death->cookie = cookie;
				ref->death = death;
				if (ref->node->proc == NULL) {
					ref->death->work.type = VDBINDER_WORK_DEAD_BINDER;
					if (thread->looper & (VDBINDER_LOOPER_STATE_REGISTERED | VDBINDER_LOOPER_STATE_ENTERED)) {
						list_add_tail(&ref->death->work.entry, &thread->todo);
					} else {
						list_add_tail(&ref->death->work.entry, &proc->todo);
						wake_up_interruptible(&proc->wait);
					}
				}
			} else {
				if (ref->death == NULL) {
					vdbinder_user_error("binder: %d:%"
						"d VD_BC_CLEAR_DEATH_NOTIFI"
						"CATION death notificat"
						"ion not active\n",
						proc->pid, thread->pid);
					break;
				}
				death = ref->death;
				if (death->cookie != cookie) {
					vdbinder_user_error("binder: %d:%"
						"d VD_BC_CLEAR_DEATH_NOTIFI"
						"CATION death notificat"
						"ion cookie mismatch "
						"%p != %p\n",
						proc->pid, thread->pid,
						death->cookie, cookie);
					break;
				}
				ref->death = NULL;
				if (list_empty(&death->work.entry)) {
					death->work.type = VDBINDER_WORK_CLEAR_DEATH_NOTIFICATION;
					if (thread->looper & (VDBINDER_LOOPER_STATE_REGISTERED | VDBINDER_LOOPER_STATE_ENTERED)) {
						list_add_tail(&death->work.entry, &thread->todo);
					} else {
						list_add_tail(&death->work.entry, &proc->todo);
						wake_up_interruptible(&proc->wait);
					}
				} else {
					BUG_ON(death->work.type != VDBINDER_WORK_DEAD_BINDER);
					death->work.type = VDBINDER_WORK_DEAD_BINDER_AND_CLEAR;
				}
			}
		} break;
		case VD_BC_DEAD_BINDER_DONE: {
			struct vdbinder_work *w;
			void __user *cookie;
			struct vdbinder_ref_death *death = NULL;
			if (get_user(cookie, (void __user * __user *)ptr))
				return -EFAULT;

			ptr += sizeof(void *);
			list_for_each_entry(w, &proc->delivered_death, entry) {
				struct vdbinder_ref_death *tmp_death = container_of(w, struct vdbinder_ref_death, work);
				if (tmp_death->cookie == cookie) {
					death = tmp_death;
					break;
				}
			}
			vdbinder_debug(VDBINDER_DEBUG_DEAD_BINDER,
				     "binder: %d:%d VD_BC_DEAD_BINDER_DONE %p found %p\n",
				     proc->pid, thread->pid, cookie, death);
			if (death == NULL) {
				vdbinder_user_error("binder: %d:%d VD_BC_DEAD"
					"_BINDER_DONE %p not found\n",
					proc->pid, thread->pid, cookie);
				break;
			}

			list_del_init(&death->work.entry);
			if (death->work.type == VDBINDER_WORK_DEAD_BINDER_AND_CLEAR) {
				death->work.type = VDBINDER_WORK_CLEAR_DEATH_NOTIFICATION;
				if (thread->looper & (VDBINDER_LOOPER_STATE_REGISTERED | VDBINDER_LOOPER_STATE_ENTERED)) {
					list_add_tail(&death->work.entry, &thread->todo);
				} else {
					list_add_tail(&death->work.entry, &proc->todo);
					wake_up_interruptible(&proc->wait);
				}
			}
		} break;

		default:
			printk(KERN_ERR "binder: %d:%d unknown command %d\n",
			       proc->pid, thread->pid, cmd);
			return -EINVAL;
		}
		*consumed = ptr - buffer;
	}
	return 0;
}

static void binder_stat_br(struct vdbinder_proc *proc, struct vdbinder_thread *thread,
		    uint32_t cmd)
{
	if (_IOC_NR(cmd) < ARRAY_SIZE(vdbinder_stats.br)) {
		vdbinder_stats.br[_IOC_NR(cmd)]++;
		proc->stats.br[_IOC_NR(cmd)]++;
		thread->stats.br[_IOC_NR(cmd)]++;
	}
}
static int binder_is_thread_wait(struct vdbinder_thread *thread)
{
	return (thread->looper & (VDBINDER_LOOPER_STATE_THREAD_WAIT)) &&
				!(thread->looper &
					(VDBINDER_LOOPER_STATE_REGISTERED |
					VDBINDER_LOOPER_STATE_ENTERED |
					VDBINDER_LOOPER_STATE_EXITED |
					VDBINDER_LOOPER_STATE_INVALID |
					VDBINDER_LOOPER_STATE_NEED_RETURN));
}

static bool is_transaction(struct vdbinder_thread *thread , unsigned int flags)
{
	return (!(thread->transaction_stack == NULL &&
			list_empty(&thread->todo))) &&
				IS_BINDER_USER_THREAD(thread->looper) &&
				!(flags & O_NONBLOCK);
}

static int binder_has_proc_work(struct vdbinder_proc *proc,
				struct vdbinder_thread *thread)
{
	return !list_empty(&proc->todo) ||
		(thread->looper & VDBINDER_LOOPER_STATE_NEED_RETURN);
}

static int binder_has_thread_work(struct vdbinder_thread *thread)
{
	return !list_empty(&thread->todo) || thread->return_error != VD_BR_OK ||
		(thread->looper & VDBINDER_LOOPER_STATE_NEED_RETURN);
}

static int binder_thread_read(struct vdbinder_proc *proc,
			      struct vdbinder_thread *thread,
			      void  __user *buffer, int size,
			      signed long *consumed, int non_block)
{
	void __user *ptr = buffer + *consumed;
	void __user *end = buffer + size;

	int ret = 0;
	int wait_for_proc_work;

	if (*consumed == 0) {
		if (put_user(VD_BR_NOOP, (uint32_t __user *)ptr))
			return -EFAULT;
		ptr += sizeof(uint32_t);
	}

retry:
	wait_for_proc_work = thread->transaction_stack == NULL &&
				list_empty(&thread->todo);

	if (thread->return_error != VD_BR_OK && ptr < end) {
		if (thread->return_error2 != VD_BR_OK) {
			if (put_user(thread->return_error2, (uint32_t __user *)ptr))
				return -EFAULT;
			ptr += sizeof(uint32_t);
			if (ptr == end)
				goto done;
			thread->return_error2 = VD_BR_OK;
		}
		if (put_user(thread->return_error, (uint32_t __user *)ptr))
			return -EFAULT;
		ptr += sizeof(uint32_t);
		thread->return_error = VD_BR_OK;
		goto done;
	}


	thread->looper |= VDBINDER_LOOPER_STATE_WAITING;
	if (wait_for_proc_work)
		proc->ready_threads++;
	mutex_unlock(&binder_lock);
	if (wait_for_proc_work && binder_is_thread_wait(thread))
		ret = wait_event_interruptible(thread->wait,
				binder_has_thread_work(thread));
	else if (wait_for_proc_work) {
		if (!(thread->looper & (VDBINDER_LOOPER_STATE_REGISTERED |
					VDBINDER_LOOPER_STATE_ENTERED))) {
			vdbinder_user_error("binder: %d:%d ERROR: Thread waiting "
				"for process work before calling VD_BC_REGISTER_"
				"LOOPER or VD_BC_ENTER_LOOPER (state %x)\n",
				proc->pid, thread->pid, thread->looper);
			wait_event_interruptible(binder_user_error_wait,
						 binder_stop_on_user_error < 2);
		}
		binder_set_nice(proc->default_priority);
		if (non_block) {
			if (!binder_has_proc_work(proc, thread))
				ret = -EAGAIN;
		} else
			ret = wait_event_interruptible_exclusive(proc->wait, binder_has_proc_work(proc, thread));
	} else {
		if (non_block) {
			if (!binder_has_thread_work(thread))
				ret = -EAGAIN;
		} else
			ret = wait_event_interruptible(thread->wait, binder_has_thread_work(thread));
	}
	mutex_lock(&binder_lock);
	if (wait_for_proc_work)
		proc->ready_threads--;
	thread->looper &= ~VDBINDER_LOOPER_STATE_WAITING;

	if (ret)
		return ret;

	while (1) {
		uint32_t cmd;
		struct vdbinder_transaction_data tr;
		struct vdbinder_work *w;
		struct vdbinder_transaction *t = NULL;

		if (!list_empty(&thread->todo))
			w = list_first_entry(&thread->todo, struct vdbinder_work, entry);
		else if (!list_empty(&proc->todo) && wait_for_proc_work)
			w = list_first_entry(&proc->todo, struct vdbinder_work, entry);
		else {
			if (ptr - buffer == 4 && !(thread->looper & VDBINDER_LOOPER_STATE_NEED_RETURN)) /* no data added */
				goto retry;
			break;
		}

		if (end - ptr < sizeof(tr) + 4)
			break;

		switch (w->type) {
		case VDBINDER_WORK_TRANSACTION: {
#ifdef CONFIG_ORSAY_DEBUG
			BUG_ON(w->owner != 0);
			w->owner = thread->pid;
#endif
			t = container_of(w, struct vdbinder_transaction, work);
		} break;
		case VDBINDER_WORK_TRANSACTION_COMPLETE: {
			cmd = VD_BR_TRANSACTION_COMPLETE;
			if (put_user(cmd, (uint32_t __user *)ptr))
				return -EFAULT;
			ptr += sizeof(uint32_t);

			binder_stat_br(proc, thread, cmd);
			vdbinder_debug(VDBINDER_DEBUG_TRANSACTION_COMPLETE,
				     "binder: %d:%d VD_BR_TRANSACTION_COMPLETE\n",
				     proc->pid, thread->pid);

			list_del(&w->entry);
			kfree(w);
			binder_stats_deleted(VDBINDER_STAT_TRANSACTION_COMPLETE);
		} break;
		case VDBINDER_WORK_NODE: {
			struct vdbinder_node *node = container_of(w, struct vdbinder_node, work);
			uint32_t cmd = VD_BR_NOOP;
			const char *cmd_name;
			int strong = node->internal_strong_refs || node->local_strong_refs;
			int weak = !hlist_empty(&node->refs) || node->local_weak_refs || strong;
			if (weak && !node->has_weak_ref) {
				cmd = VD_BR_INCREFS;
				cmd_name = "VD_BR_INCREFS";
				node->has_weak_ref = 1;
				node->pending_weak_ref = 1;
				node->local_weak_refs++;
			} else if (strong && !node->has_strong_ref) {
				cmd = VD_BR_ACQUIRE;
				cmd_name = "VD_BR_ACQUIRE";
				node->has_strong_ref = 1;
				node->pending_strong_ref = 1;
				node->local_strong_refs++;
			} else if (!strong && node->has_strong_ref) {
				cmd = VD_BR_RELEASE;
				cmd_name = "VD_BR_RELEASE";
				node->has_strong_ref = 0;
			} else if (!weak && node->has_weak_ref) {
				cmd = VD_BR_DECREFS;
				cmd_name = "VD_BR_DECREFS";
				node->has_weak_ref = 0;
			}
			if (cmd != VD_BR_NOOP) {
				if (put_user(cmd, (uint32_t __user *)ptr))
					return -EFAULT;
				ptr += sizeof(uint32_t);
				if (put_user(node->ptr, (void * __user *)ptr))
					return -EFAULT;
				ptr += sizeof(void *);
				if (put_user(node->cookie, (void * __user *)ptr))
					return -EFAULT;
				ptr += sizeof(void *);

				binder_stat_br(proc, thread, cmd);
				vdbinder_debug(VDBINDER_DEBUG_USER_REFS,
					     "binder: %d:%d %s %d u%p c%p\n",
					     proc->pid, thread->pid, cmd_name, node->debug_id, node->ptr, node->cookie);
			} else {
				list_del_init(&w->entry);
				if (!weak && !strong) {
					vdbinder_debug(VDBINDER_DEBUG_INTERNAL_REFS,
						     "binder: %d:%d node %d u%p c%p deleted\n",
						     proc->pid, thread->pid, node->debug_id,
						     node->ptr, node->cookie);
					rb_erase(&node->rb_node, &proc->nodes);
					kfree(node);
					binder_stats_deleted(VDBINDER_STAT_NODE);
				} else {
					vdbinder_debug(VDBINDER_DEBUG_INTERNAL_REFS,
						     "binder: %d:%d node %d u%p c%p state unchanged\n",
						     proc->pid, thread->pid, node->debug_id, node->ptr,
						     node->cookie);
				}
			}
		} break;
		case VDBINDER_WORK_DEAD_BINDER:
		case VDBINDER_WORK_DEAD_BINDER_AND_CLEAR:
		case VDBINDER_WORK_CLEAR_DEATH_NOTIFICATION: {
			struct vdbinder_ref_death *death;
			uint32_t cmd;

			death = container_of(w, struct vdbinder_ref_death, work);
			if (w->type == VDBINDER_WORK_CLEAR_DEATH_NOTIFICATION)
				cmd = VD_BR_CLEAR_DEATH_NOTIFICATION_DONE;
			else
				cmd = VD_BR_DEAD_BINDER;
			if (put_user(cmd, (uint32_t __user *)ptr))
				return -EFAULT;
			ptr += sizeof(uint32_t);
			if (put_user(death->cookie, (void * __user *)ptr))
				return -EFAULT;
			ptr += sizeof(void *);
			vdbinder_debug(VDBINDER_DEBUG_DEATH_NOTIFICATION,
				     "binder: %d:%d %s %p\n",
				      proc->pid, thread->pid,
				      cmd == VD_BR_DEAD_BINDER ?
				      "VD_BR_DEAD_BINDER" :
				      "VD_BR_CLEAR_DEATH_NOTIFICATION_DONE",
				      death->cookie);

			if (w->type == VDBINDER_WORK_CLEAR_DEATH_NOTIFICATION) {
				list_del(&w->entry);
				kfree(death);
				binder_stats_deleted(VDBINDER_STAT_DEATH);
			} else
				list_move(&w->entry, &proc->delivered_death);
			if (cmd == VD_BR_DEAD_BINDER)
				goto done; /* DEAD_BINDER notifications can cause transactions */
		} break;
		}

		if (!t)
			continue;

		BUG_ON(t->buffer == NULL);
		if (t->buffer->target_node) {
			struct vdbinder_node *target_node = t->buffer->target_node;
			tr.target.ptr = target_node->ptr;
			tr.cookie =  target_node->cookie;
			t->saved_priority = task_nice(current);
			if (t->priority < target_node->min_priority &&
			    !(t->flags & VD_TF_ONE_WAY))
				binder_set_nice(t->priority);
			else if (!(t->flags & VD_TF_ONE_WAY) ||
				 t->saved_priority > target_node->min_priority)
				binder_set_nice(target_node->min_priority);
			cmd = VD_BR_TRANSACTION;
		} else {
			tr.target.ptr = NULL;
			tr.cookie = NULL;
			cmd = VD_BR_REPLY;
		}
		tr.code = t->code;
		tr.flags = t->flags;
		tr.isEnc = t->isEnc;
		tr.isShrd = t->isShrd;
		tr.sender_euid = t->sender_euid;

		if (t->from) {
			struct task_struct *sender = t->from->proc->tsk;
			tr.sender_pid = task_tgid_nr_ns(sender,
							current->nsproxy->pid_ns);
		} else {
			tr.sender_pid = 0;
		}

		tr.data_size = t->buffer->data_size;
		tr.offsets_size = t->buffer->offsets_size;
		tr.data.ptr.buffer = (void *)t->buffer->data +
					proc->user_buffer_offset;
		tr.data.ptr.offsets = tr.data.ptr.buffer +
					ALIGN(t->buffer->data_size,
					    sizeof(void *));

		if (put_user(cmd, (uint32_t __user *)ptr))
			return -EFAULT;
		ptr += sizeof(uint32_t);
		if (copy_to_user(ptr, &tr, sizeof(tr)))
			return -EFAULT;
		ptr += sizeof(tr);

		binder_stat_br(proc, thread, cmd);
		vdbinder_debug(VDBINDER_DEBUG_TRANSACTION,
			     "binder: %d:%d %s %d %d:%d, cmd %d"
			     "size %zd-%zd ptr %p-%p\n",
			     proc->pid, thread->pid,
			     (cmd == VD_BR_TRANSACTION) ? "VD_BR_TRANSACTION" :
			     "VD_BR_REPLY",
			     t->debug_id, t->from ? t->from->proc->pid : 0,
			     t->from ? t->from->pid : 0, cmd,
			     t->buffer->data_size, t->buffer->offsets_size,
			     tr.data.ptr.buffer, tr.data.ptr.offsets);
#ifdef CONFIG_ORSAY_DEBUG
		BUG_ON(w->owner != thread->pid);
		w->owner = 0;
#endif

#ifdef CONFIG_ORSAY_CHECK_TODO_WORKENTRY
		if ((w->entry.next == LIST_POISON1) ||
			 (w->entry.prev == LIST_POISON2)) {
			printk(KERN_INFO"\nbinder : work entry already deleted!\n");
			ptr -= sizeof(tr);
			ptr -= sizeof(uint32_t);
		} else {
#endif
			list_del(&t->work.entry);
			t->buffer->allow_user_free = 1;
			if (cmd == VD_BR_TRANSACTION &&
				 !(t->flags & VD_TF_ONE_WAY)) {
				t->to_parent = thread->transaction_stack;
				t->to_thread = thread;
				thread->transaction_stack = t;
			} else {
				t->buffer->transaction = NULL;
				kfree(t);
				binder_stats_deleted(VDBINDER_STAT_TRANSACTION);
			}
#ifdef CONFIG_ORSAY_CHECK_TODO_WORKENTRY
		}
#endif
		break;
	}

done:

	*consumed = ptr - buffer;
	if (proc->requested_threads + proc->ready_threads == 0 &&
	    proc->requested_threads_started < proc->max_threads &&
	    (thread->looper & (VDBINDER_LOOPER_STATE_REGISTERED |
	     VDBINDER_LOOPER_STATE_ENTERED)) /* the user-space code fails to */
	     /*spawn a new thread if we leave this out */) {
		proc->requested_threads++;
		vdbinder_debug(VDBINDER_DEBUG_THREADS,
			     "binder: %d:%d VD_BR_SPAWN_LOOPER\n",
			     proc->pid, thread->pid);
		if (put_user(VD_BR_SPAWN_LOOPER, (uint32_t __user *)buffer))
			return -EFAULT;
	}
	return 0;
}

static void binder_release_work(struct list_head *list)
{
	struct vdbinder_work *w;
	while (!list_empty(list)) {
		w = list_first_entry(list, struct vdbinder_work, entry);
		list_del_init(&w->entry);
		switch (w->type) {
		case VDBINDER_WORK_TRANSACTION: {
			struct vdbinder_transaction *t;

			t = container_of(w, struct vdbinder_transaction, work);
			if (t->buffer->target_node && !(t->flags & VD_TF_ONE_WAY))
				binder_send_failed_reply(t, VD_BR_DEAD_REPLY);
		} break;
		case VDBINDER_WORK_TRANSACTION_COMPLETE: {
			kfree(w);
			binder_stats_deleted(VDBINDER_STAT_TRANSACTION_COMPLETE);
		} break;
		default:
			break;
		}
	}

}

static struct vdbinder_thread *binder_get_thread(struct vdbinder_proc *proc)
{
	struct vdbinder_thread *thread = NULL;
	struct rb_node *parent = NULL;
	struct rb_node **p = &proc->threads.rb_node;

	while (*p) {
		parent = *p;
		thread = rb_entry(parent, struct vdbinder_thread, rb_node);

		if (current->pid < thread->pid)
			p = &(*p)->rb_left;
		else if (current->pid > thread->pid)
			p = &(*p)->rb_right;
		else
			break;
	}
	if (*p == NULL) {
		thread = kzalloc(sizeof(*thread), GFP_KERNEL);
		if (thread == NULL)
			return NULL;
		binder_stats_created(VDBINDER_STAT_THREAD);
		thread->proc = proc;
		thread->pid = current->pid;
		init_waitqueue_head(&thread->wait);
		INIT_LIST_HEAD(&thread->todo);
		INIT_LIST_HEAD(&thread->dbg_entry.entry);
		thread->dbg_entry.type = TRANSACTION_FINISH;
		rb_link_node(&thread->rb_node, parent, p);
		rb_insert_color(&thread->rb_node, &proc->threads);
		thread->looper |= VDBINDER_LOOPER_STATE_NEED_RETURN;
		thread->return_error = VD_BR_OK;
		thread->return_error2 = VD_BR_OK;
		thread->tsk = current;
		thread->jiffy = 0;
	}
	return thread;
}

static int binder_free_thread(struct vdbinder_proc *proc,
			      struct vdbinder_thread *thread)
{
	struct vdbinder_transaction *t;
	struct vdbinder_transaction *send_reply = NULL;
	int active_transactions = 0;

	rb_erase(&thread->rb_node, &proc->threads);
	t = thread->transaction_stack;
	if (t && t->to_thread == thread)
		send_reply = t;
	while (t) {
		active_transactions++;
		vdbinder_debug(VDBINDER_DEBUG_DEAD_TRANSACTION,
			     "binder: release %d:%d transaction %d "
			     "%s, still active\n", proc->pid, thread->pid,
			     t->debug_id,
			     (t->to_thread == thread) ? "in" : "out");

		if (t->to_thread == thread) {
			t->to_proc = NULL;
			t->to_thread = NULL;
			if (t->buffer) {
				t->buffer->transaction = NULL;
				t->buffer = NULL;
			}
			t = t->to_parent;
		} else if (t->from == thread) {
			t->from = NULL;
			t = t->from_parent;
		} else
			BUG();
	}
	if (send_reply)
		binder_send_failed_reply(send_reply, VD_BR_DEAD_REPLY);
	binder_release_work(&thread->todo);
	binder_dbg_free_thread(thread);
	kfree(thread);
	binder_stats_deleted(VDBINDER_STAT_THREAD);
	return active_transactions;
}

static unsigned int binder_poll(struct file *filp,
				struct poll_table_struct *wait)
{
	struct vdbinder_proc *proc = filp->private_data;
	struct vdbinder_thread *thread = NULL;
	int wait_for_proc_work;

	mutex_lock(&binder_lock);
	thread = binder_get_thread(proc);

	wait_for_proc_work = thread->transaction_stack == NULL &&
		list_empty(&thread->todo) && thread->return_error == VD_BR_OK;
	mutex_unlock(&binder_lock);

	if (wait_for_proc_work) {
		if (binder_has_proc_work(proc, thread))
			return POLLIN;
		poll_wait(filp, &proc->wait, wait);
		if (binder_has_proc_work(proc, thread))
			return POLLIN;
	} else {
		if (binder_has_thread_work(thread))
			return POLLIN;
		poll_wait(filp, &thread->wait, wait);
		if (binder_has_thread_work(thread))
			return POLLIN;
	}
	return 0;
}

static long vdbinder_clear_context_mgr_wait(struct vdbinder_proc *proc,
						struct vdbinder_thread *thread,
						unsigned int size,
						void __user *ubuf) {
	int ret = 0;
	struct vdbinder_wake_info wake_info;
	struct vdbinder_service_wait_work *sp_pos;
	struct list_head *pos, *q;
	if (size != sizeof(struct vdbinder_wake_info)) {
		vdbinder_user_error("Failed to clear"
				" the wait list\n");
		vdbinder_user_error("There are processes"
				" still waiting\n");
		ret = -EINVAL;
		return ret;
	}
	if (copy_from_user(&wake_info, ubuf, sizeof(wake_info))) {
		ret = -EFAULT;
		vdbinder_user_error("Failed to clear"
				" the wait list\n");
		vdbinder_user_error("There are processes"
				" still waiting\n");
		return ret;
	}
	/*Now, process the data from the service manager
	**Remove the first element from the list*/
	list_for_each_safe(pos, q, &vdbinder_service_wait_list)
	{
		sp_pos = list_entry(pos,
				struct vdbinder_service_wait_work,
				wait_entry);

		if (sp_pos->t->buffer == NULL) {
			printk(KERN_ERR "binder: service wait transact"
				"(id:%ld) buffer is null\n",
				wake_info.id);
			list_del(pos);
			kfree(sp_pos->t);
			kfree(sp_pos);
			continue;
		}

		if (sp_pos->index == wake_info.id) {
			/*Process the first entry of the list*/
			struct vdbinder_proc *target_proc;
			struct vdbinder_thread *target_thread;
			struct flat_vdbinder_object *fp;
			struct list_head *target_list;
			struct vdbinder_ref *ref;
			wait_queue_head_t *target_wait;
			fp = (struct flat_vdbinder_object *)
						(sp_pos->t->buffer->data +
							*sp_pos->offset);
			switch (fp->type) {

			case VDBINDER_TYPE_WAIT_BINDER: {
				target_proc = sp_pos->t->to_proc;
				target_thread = sp_pos->t->to_thread;
				if (target_thread) {
					target_list = &target_thread->todo;
					target_wait = &target_thread->wait;
				} else {
					target_list = &target_proc->todo;
					target_wait = &target_proc->wait;
				}
				fp->handle = wake_info.handle;
				fp->type = VDBINDER_TYPE_HANDLE;
				ref = binder_get_ref(proc, fp->handle);
				if (ref == NULL) {
					vdbinder_user_error("binder: %d:%d got "
						"transaction with invalid "
						"handle, %ld\n", proc->pid,
						thread->pid, fp->handle);
					vdbinder_user_error("Failed to clear"
							" the wait list\n");
					vdbinder_user_error("There are "
						"processes still waiting\n");
					ret = -EINVAL;
					return ret;
				}
				if (ref->node->proc == target_proc) {
					if (fp->type == VDBINDER_TYPE_HANDLE)
						fp->type = VDBINDER_TYPE_BINDER;
					else
						fp->type = VDBINDER_TYPE_WEAK_BINDER;
					fp->binder = ref->node->ptr;
					fp->cookie = ref->node->cookie;
					binder_inc_node(ref->node,
					fp->type == VDBINDER_TYPE_BINDER,
						0, NULL);
					vdbinder_debug(VDBINDER_DEBUG_TRANSACTION,
						"        ref %d desc %d ->"
						" node %d u%p\n",
						ref->debug_id, ref->desc,
						ref->node->debug_id,
						ref->node->ptr);
				} else {
					struct vdbinder_ref *new_ref;
					new_ref = binder_get_ref_for_node(target_proc, ref->node);
					if (new_ref == NULL) {
						vdbinder_user_error("binder:"
							" %d:%d failed to "
							"create ref between "
							"proc: %d and node: "
							"%p\n",	proc->pid,
							thread->pid,
							target_proc->pid,
							ref->node);
						vdbinder_user_error("Failed"
							"to clear the"
							" wait list\n");
						vdbinder_user_error("There are"
							" processes"
							" still waiting\n");
						ret = -EINVAL;
						return ret;
					}
					fp->handle = new_ref->desc;
					binder_inc_ref(new_ref,
					fp->type == VDBINDER_TYPE_HANDLE,
						NULL);
					vdbinder_debug(VDBINDER_DEBUG_TRANSACTION,
						"        ref %d desc %d -> "
						"ref %d desc %d (node %d)\n",
						ref->debug_id, ref->desc,
						new_ref->debug_id,
						new_ref->desc,
						ref->node->debug_id);
				}
			} break;
			default:
				ret = -EINVAL;
				return ret;
				break;
			}
			sp_pos->t->work.type = VDBINDER_WORK_TRANSACTION;
#ifdef CONFIG_ORSAY_DEBUG
			sp_pos->t->work.owner = 0;
#endif
			list_add_tail(&sp_pos->t->work.entry, target_list);
			if (target_wait)
				wake_up_interruptible(target_wait);
			list_del(pos);
			kfree(sp_pos);
		}
	}
	return ret;
}

static void delete_service_wait_list_by_thread(struct vdbinder_thread *thread)
{
	struct vdbinder_service_wait_work *sp_pos;
	struct list_head *pos, *q;

	list_for_each_safe(pos, q, &vdbinder_service_wait_list)
	{
		sp_pos = list_entry(pos, struct vdbinder_service_wait_work,
			wait_entry);
		if (sp_pos->t->to_thread == thread) {
			BUG_ON(sp_pos->t == NULL);
			if (sp_pos->t->buffer != NULL) {
				binder_transaction_buffer_release(thread->proc,
						sp_pos->t->buffer, NULL);
				sp_pos->t->buffer->transaction = NULL;
				binder_free_buf(thread->proc,
					sp_pos->t->buffer);
			}
			list_del(pos);
			kfree(sp_pos->t);
			kfree(sp_pos);
			break;
		}
	}
}

static void delete_service_wait_list_by_proc(struct vdbinder_proc *proc)
{
	struct rb_node *n;

	if (proc == NULL)
		return;

	for (n = rb_first(&proc->threads) ; n != NULL; n = rb_next(n)) {
		struct vdbinder_thread *thread =
			rb_entry(n, struct vdbinder_thread, rb_node);
		delete_service_wait_list_by_thread(thread);
	}
}

static long binder_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int ret;
	struct vdbinder_proc *proc = filp->private_data;
	struct vdbinder_thread *thread;
	unsigned int size = _IOC_SIZE(cmd);
	void __user *ubuf = (void __user *)arg;

	/*printk(KERN_INFO "binder_ioctl: %d:%d %x %lx\n", proc->pid, current->pid, cmd, arg);*/

	ret = wait_event_interruptible(binder_user_error_wait, binder_stop_on_user_error < 2);
	if (ret)
		return ret;

	mutex_lock(&binder_lock);
	thread = binder_get_thread(proc);
	if (thread == NULL) {
		ret = -ENOMEM;
		goto err;
	}

	switch (cmd) {
	case VDBINDER_WRITE_READ: {
		struct vdbinder_write_read bwr;
		if (size != sizeof(struct vdbinder_write_read)) {
			ret = -EINVAL;
			goto err;
		}
		if (copy_from_user(&bwr, ubuf, sizeof(bwr))) {
			ret = -EFAULT;
			goto err;
		}
		vdbinder_debug(VDBINDER_DEBUG_READ_WRITE,
			     "binder: %d:%d write %ld at %08lx, read %ld at %08lx\n",
			     proc->pid, thread->pid, bwr.write_size, bwr.write_buffer,
			     bwr.read_size, bwr.read_buffer);

		if (bwr.write_size > 0) {
			ret = binder_thread_write(proc, thread, (void __user *)bwr.write_buffer, bwr.write_size, &bwr.write_consumed);
			if (ret < 0) {
				bwr.read_consumed = 0;
				if (copy_to_user(ubuf, &bwr, sizeof(bwr)))
					ret = -EFAULT;
				goto err;
			}
		}
		if (bwr.read_size > 0) {
#ifdef CONFIG_ORSAY_DEBUG
			bool transaction_flag = is_transaction(thread ,
						filp->f_flags);
			if (transaction_flag)
				binder_add_to_debug_db(thread);
#endif
			ret = binder_thread_read(proc, thread, (void __user *)bwr.read_buffer, bwr.read_size, &bwr.read_consumed, filp->f_flags & O_NONBLOCK);
#ifdef CONFIG_ORSAY_DEBUG
			if (transaction_flag && (thread->dbg_entry.type ==
					TRANSACTION_START))
				binder_remove_from_debug_db(thread);
#endif
			if (!list_empty(&proc->todo))
				wake_up_interruptible(&proc->wait);
			if (ret < 0) {
				if (copy_to_user(ubuf, &bwr, sizeof(bwr)))
					ret = -EFAULT;
				goto err;
			}
		}
		vdbinder_debug(VDBINDER_DEBUG_READ_WRITE,
			     "binder: %d:%d wrote %ld of %ld, read return %ld of %ld\n",
			     proc->pid, thread->pid, bwr.write_consumed, bwr.write_size,
			     bwr.read_consumed, bwr.read_size);
		if (copy_to_user(ubuf, &bwr, sizeof(bwr))) {
			ret = -EFAULT;
			goto err;
		}
		break;
	}
	case VDBINDER_SET_MAX_THREADS:
		if (copy_from_user(&proc->max_threads, ubuf, sizeof(proc->max_threads))) {
			ret = -EINVAL;
			goto err;
		}
		break;
	case VDBINDER_CONTEXT_MGR_READY:
		wake_up_interruptible_all(&binder_context_mgr_wait);
		break;
	case VDBINDER_SET_CONTEXT_MGR:
		if (binder_context_mgr_node != NULL) {
			printk(KERN_ERR "binder: VDBINDER_SET_CONTEXT_MGR already set\n");
			ret = -EBUSY;
			goto err;
		}
		if (binder_context_mgr_uid != -1) {
			if (binder_context_mgr_uid != current->cred->euid) {
				printk(KERN_ERR "binder: BINDER_SET_"
				       "CONTEXT_MGR bad uid %d != %d\n",
				       current->cred->euid,
				       binder_context_mgr_uid);
				ret = -EPERM;
				goto err;
			}
		} else
			binder_context_mgr_uid = current->cred->euid;
		binder_context_mgr_node = binder_new_node(proc, NULL, NULL);
		if (binder_context_mgr_node == NULL) {
			ret = -ENOMEM;
			goto err;
		}
		binder_context_mgr_node->local_weak_refs++;
		binder_context_mgr_node->local_strong_refs++;
		binder_context_mgr_node->has_strong_ref = 1;
		binder_context_mgr_node->has_weak_ref = 1;
		break;
	case VDBINDER_THREAD_EXIT:
		vdbinder_debug(VDBINDER_DEBUG_THREADS, "binder: %d:%d exit\n",
			     proc->pid, thread->pid);
		binder_free_thread(proc, thread);
		thread = NULL;
		break;
	case VDBINDER_VERSION:
		if (size != sizeof(struct vdbinder_version)) {
			ret = -EINVAL;
			goto err;
		}
		if (put_user(VDBINDER_CURRENT_PROTOCOL_VERSION, &((struct vdbinder_version *)ubuf)->protocol_version)) {
			ret = -EINVAL;
			goto err;
		}
		break;
	case VDBINDER_DEBUG_VERSION:
		ret = binder_get_dbg_version(size, ubuf);
		if (ret < 0)
			goto err;
		break;
	case VDBINDER_SET_BOARD_INFO: {
		int board_no = -1;
		if (size != sizeof(int)) {
			ret = -EINVAL;
			goto err;
		}
		if (get_user(board_no, (int *)ubuf)) {
			ret = -EINVAL;
			goto err;
		}

		WARN_ON(board_no < 0 || board_no > 1);
		if (board_no == 0 || board_no == 1) {
			binder_this_board_no = board_no;
		} else {
			ret = -EINVAL;
			goto err;
		}
		break;
	}
	case VDBINDER_GET_BOARD_INFO:
		if (size != sizeof(int)) {
			ret = -EINVAL;
			goto err;
		}
		if (put_user(binder_this_board_no, (int *)ubuf)) {
			ret = -EINVAL;
			goto err;
		}
		break;
	case VDBINDER_CONTEXT_MGR_CLEAR_WAIT_LIST:
		ret = vdbinder_clear_context_mgr_wait(proc, thread, size, ubuf);
		if (ret < 0)
			goto err;
		break;
	case VDBINDER_SET_TIME: {
		unsigned long sec = 0;
		if (size != sizeof(unsigned long)) {
			ret = -EINVAL;
			goto err;
		}
		if (get_user(sec, (unsigned long *)ubuf)) {
			ret = -EINVAL;
			goto err;
		}
		vdbinder_time_offset = sec;
		vdbinder_is_new_time_set = 1;
		wake_up_interruptible(&vdbinder_rpcagent_wait);
		break;
	}
        case VDBINDER_GET_TIME:
                if (size != sizeof(unsigned long)) {
                        ret = -EINVAL;
                        goto err;
                }
                if (put_user(vdbinder_time_offset, (unsigned long *)ubuf)) {
                        ret = -EINVAL;
                        goto err;
                }
                break;
        case VDBINDER_SET_TIME_NOWAIT: {
                unsigned long sec = 0;
                if (size != sizeof(unsigned long)) {
                        ret = -EINVAL;
                        goto err;
                }
                if (get_user(sec, (unsigned long *)ubuf)) {
                        ret = -EINVAL;
                        goto err;
                }
                vdbinder_time_offset = sec;
                break;
        }
        case VDBINDER_GET_TIME_WAIT:
                if (size != sizeof(unsigned long)) {
                        ret = -EINVAL;
                        goto err;
                }
                mutex_unlock(&binder_lock);
                ret = wait_event_interruptible(
                     vdbinder_rpcagent_wait,
                     (vdbinder_is_new_time_set == 1));
                if (ret)
                        return ret;
                mutex_lock(&binder_lock);

                if (put_user(vdbinder_time_offset, (unsigned long *)ubuf)) {
                        ret = -EINVAL;
                        goto err;
                }
                vdbinder_is_new_time_set = 0;
                break;
	case VDBINDER_SET_RPC_AGENT_THREAD:
		ret = binder_set_rpcagent_thread(thread);
		if (ret < 0)
			goto err;
		break;
	case VDBINDER_SET_DEBUG_THREAD:
		ret = binder_set_debug_thread(proc, thread);
		if (ret < 0)
			goto err;
		break;
	case VDBINDER_DEBUG_OPERATION: {
		ret = binder_dbg_operation(size, ubuf, proc, thread);
		if (ret < 0)
			goto err;
		break;
	}
	default:
		ret = -EINVAL;
		goto err;
	}
	ret = 0;
err:
	if (thread)
		thread->looper &= ~VDBINDER_LOOPER_STATE_NEED_RETURN;
	mutex_unlock(&binder_lock);
	wait_event_interruptible(binder_user_error_wait, binder_stop_on_user_error < 2);
	if (ret && ret != -ERESTARTSYS)
		printk(KERN_INFO "binder: %d:%d ioctl %x %lx returned %d\n", proc->pid, current->pid, cmd, arg, ret);
	return ret;
}

static void binder_vma_open(struct vm_area_struct *vma)
{
	struct vdbinder_proc *proc = vma->vm_private_data;
	vdbinder_debug(VDBINDER_DEBUG_OPEN_CLOSE,
		     "binder: %d open vm area %lx-%lx (%ld K) vma %lx pagep %lx\n",
		     proc->pid, vma->vm_start, vma->vm_end,
		     (vma->vm_end - vma->vm_start) / VD_SZ_1K, vma->vm_flags,
		     (unsigned long)pgprot_val(vma->vm_page_prot));
	dump_stack();
}

static void binder_vma_close(struct vm_area_struct *vma)
{
	struct vdbinder_proc *proc = vma->vm_private_data;
	vdbinder_debug(VDBINDER_DEBUG_OPEN_CLOSE,
		     "binder: %d close vm area %lx-%lx (%ld K) vma %lx pagep %lx\n",
		     proc->pid, vma->vm_start, vma->vm_end,
		     (vma->vm_end - vma->vm_start) / VD_SZ_1K, vma->vm_flags,
		     (unsigned long)pgprot_val(vma->vm_page_prot));
	proc->vma = NULL;
	binder_defer_work(proc, VDBINDER_DEFERRED_PUT_FILES);
}

static struct vm_operations_struct binder_vm_ops = {
	.open = binder_vma_open,
	.close = binder_vma_close,
};

static int binder_mmap(struct file *filp, struct vm_area_struct *vma)
{
	int ret;
	struct vm_struct *area;
	struct vdbinder_proc *proc = filp->private_data;
	const char *failure_string;
	struct vdbinder_buffer *buffer;

	if ((vma->vm_end - vma->vm_start) > VD_SZ_4M)
		vma->vm_end = vma->vm_start + VD_SZ_4M;

	vdbinder_debug(VDBINDER_DEBUG_OPEN_CLOSE,
		     "binder_mmap: %d %lx-%lx (%ld K) vma %lx pagep %lx\n",
		     proc->pid, vma->vm_start, vma->vm_end,
		     (vma->vm_end - vma->vm_start) / VD_SZ_1K, vma->vm_flags,
		     (unsigned long)pgprot_val(vma->vm_page_prot));

	if (vma->vm_flags & VD_FORBIDDEN_MMAP_FLAGS) {
		ret = -EPERM;
		failure_string = "bad vm_flags";
		goto err_bad_arg;
	}
	vma->vm_flags = (vma->vm_flags | VM_DONTCOPY) & ~VM_MAYWRITE;

	if (proc->buffer) {
		ret = -EBUSY;
		failure_string = "already mapped";
		goto err_already_mapped;
	}

	area = get_vm_area(vma->vm_end - vma->vm_start, VM_IOREMAP);
	if (area == NULL) {
		ret = -ENOMEM;
		failure_string = "get_vm_area";
		goto err_get_vm_area_failed;
	}
	proc->buffer = area->addr;
	proc->user_buffer_offset = vma->vm_start - (uintptr_t)proc->buffer;

#ifdef CONFIG_CPU_CACHE_VIPT
	if (cache_is_vipt_aliasing()) {
		while (CACHE_COLOUR((vma->vm_start ^ (uint32_t)proc->buffer))) {
			printk(KERN_INFO "binder_mmap: %d %lx-%lx maps %p bad alignment\n", proc->pid, vma->vm_start, vma->vm_end, proc->buffer);
			vma->vm_start += PAGE_SIZE;
		}
	}
#endif
	proc->pages = kzalloc(sizeof(proc->pages[0]) * ((vma->vm_end - vma->vm_start) / PAGE_SIZE), GFP_KERNEL);
	if (proc->pages == NULL) {
		ret = -ENOMEM;
		failure_string = "alloc page array";
		goto err_alloc_pages_failed;
	}
	proc->buffer_size = vma->vm_end - vma->vm_start;

	vma->vm_ops = &binder_vm_ops;
	vma->vm_private_data = proc;

	if (binder_update_page_range(proc, 1, proc->buffer, proc->buffer + PAGE_SIZE, vma)) {
		ret = -ENOMEM;
		failure_string = "alloc small buf";
		goto err_alloc_small_buf_failed;
	}
	buffer = proc->buffer;
	INIT_LIST_HEAD(&proc->buffers);
	list_add(&buffer->entry, &proc->buffers);
	buffer->free = 1;
	binder_insert_free_buffer(proc, buffer);
	proc->free_async_space = proc->buffer_size / 2;
	barrier();
	proc->files = get_files_struct(current);
	proc->vma = vma;

	/*printk(KERN_INFO "binder_mmap: %d %lx-%lx maps %p\n",
		 proc->pid, vma->vm_start, vma->vm_end, proc->buffer);*/
	return 0;

err_alloc_small_buf_failed:
	kfree(proc->pages);
	proc->pages = NULL;
err_alloc_pages_failed:
	vfree(proc->buffer);
	proc->buffer = NULL;
err_get_vm_area_failed:
err_already_mapped:
err_bad_arg:
	printk(KERN_ERR "binder_mmap: %d %lx-%lx %s failed %d\n",
	       proc->pid, vma->vm_start, vma->vm_end, failure_string, ret);
	return ret;
}

static int binder_open(struct inode *nodp, struct file *filp)
{
	struct vdbinder_proc *proc;

	vdbinder_debug(VDBINDER_DEBUG_OPEN_CLOSE, "binder_open: %d:%d\n",
		     current->group_leader->pid, current->pid);

	proc = kzalloc(sizeof(*proc), GFP_KERNEL);
	if (proc == NULL)
		return -ENOMEM;
	get_task_struct(current);
	proc->tsk = current;
	INIT_LIST_HEAD(&proc->todo);
	init_waitqueue_head(&proc->wait);
	proc->default_priority = task_nice(current);
	mutex_lock(&binder_lock);
	binder_stats_created(VDBINDER_STAT_PROC);
	hlist_add_head(&proc->proc_node, &binder_procs);
	proc->pid = current->group_leader->pid;
	INIT_LIST_HEAD(&proc->delivered_death);
	filp->private_data = proc;
	mutex_unlock(&binder_lock);

	if (binder_debugfs_dir_entry_proc) {
		char strbuf[11];
		snprintf(strbuf, sizeof(strbuf), "%u", proc->pid);
		proc->debugfs_entry = debugfs_create_file(strbuf, S_IRUGO,
			binder_debugfs_dir_entry_proc, proc, &binder_proc_fops);
	}

	return 0;
}

static int binder_flush(struct file *filp, fl_owner_t id)
{
	struct vdbinder_proc *proc = filp->private_data;

	binder_defer_work(proc, VDBINDER_DEFERRED_FLUSH);

	return 0;
}

static void binder_deferred_flush(struct vdbinder_proc *proc)
{
	struct rb_node *n;
	int wake_count = 0;
	for (n = rb_first(&proc->threads); n != NULL; n = rb_next(n)) {
		struct vdbinder_thread *thread = rb_entry(n, struct vdbinder_thread, rb_node);
		thread->looper |= VDBINDER_LOOPER_STATE_NEED_RETURN;
		if (thread->looper & VDBINDER_LOOPER_STATE_WAITING) {
			wake_up_interruptible(&thread->wait);
			wake_count++;
		}
	}
	wake_up_interruptible_all(&proc->wait);

	vdbinder_debug(VDBINDER_DEBUG_OPEN_CLOSE,
		     "binder_flush: %d woke %d threads\n", proc->pid,
		     wake_count);
}

static int binder_release(struct inode *nodp, struct file *filp)
{
	struct vdbinder_proc *proc = filp->private_data;
	debugfs_remove(proc->debugfs_entry);
	binder_defer_work(proc, VDBINDER_DEFERRED_RELEASE);

	return 0;
}

static void binder_deferred_release(struct vdbinder_proc *proc)
{
	struct hlist_node *pos;
	struct vdbinder_transaction *t;
	struct rb_node *n;
	int threads, nodes, incoming_refs, outgoing_refs, buffers, active_transactions, page_count;

	BUG_ON(proc->vma);
	BUG_ON(proc->files);

	hlist_del(&proc->proc_node);
	if (binder_context_mgr_node && binder_context_mgr_node->proc == proc) {
		vdbinder_debug(VDBINDER_DEBUG_DEAD_BINDER,
			     "binder_release: %d context_mgr_node gone\n",
			     proc->pid);
		binder_context_mgr_node = NULL;
	}

	threads = 0;
	active_transactions = 0;
	while ((n = rb_first(&proc->threads))) {
		struct vdbinder_thread *thread = rb_entry(n, struct vdbinder_thread, rb_node);
		threads++;
		active_transactions += binder_free_thread(proc, thread);
	}
	nodes = 0;
	incoming_refs = 0;
	while ((n = rb_first(&proc->nodes))) {
		struct vdbinder_node *node = rb_entry(n, struct vdbinder_node, rb_node);

		nodes++;
		rb_erase(&node->rb_node, &proc->nodes);
		list_del_init(&node->work.entry);
		if (hlist_empty(&node->refs)) {
			kfree(node);
			binder_stats_deleted(VDBINDER_STAT_NODE);
		} else {
			struct vdbinder_ref *ref;
			int death = 0;

			node->proc = NULL;
			node->local_strong_refs = 0;
			node->local_weak_refs = 0;
			hlist_add_head(&node->dead_node, &binder_dead_nodes);

			hlist_for_each_entry(ref, pos, &node->refs, node_entry) {
				incoming_refs++;
				if (ref->death) {
					death++;
					if (list_empty(&ref->death->work.entry)) {
						ref->death->work.type = VDBINDER_WORK_DEAD_BINDER;
						list_add_tail(&ref->death->work.entry, &ref->proc->todo);
						wake_up_interruptible(&ref->proc->wait);
					} else
						BUG();
				}
			}
			vdbinder_debug(VDBINDER_DEBUG_DEAD_BINDER,
				     "binder: node %d now dead, "
				     "refs %d, death %d\n", node->debug_id,
				     incoming_refs, death);
		}
	}
	outgoing_refs = 0;
	while ((n = rb_first(&proc->refs_by_desc))) {
		struct vdbinder_ref *ref = rb_entry(n, struct vdbinder_ref,
						  rb_node_desc);
		outgoing_refs++;
		binder_delete_ref(ref);
	}
	binder_release_work(&proc->todo);
	buffers = 0;

	while ((n = rb_first(&proc->allocated_buffers))) {
		struct vdbinder_buffer *buffer = rb_entry(n, struct vdbinder_buffer,
							rb_node);
		t = buffer->transaction;
		if (t) {
			t->buffer = NULL;
			buffer->transaction = NULL;
			printk(KERN_ERR "binder: release proc %d, "
			       "transaction %d, not freed\n",
			       proc->pid, t->debug_id);
			/*BUG();*/
		}
		binder_free_buf(proc, buffer);
		buffers++;
	}

	binder_stats_deleted(VDBINDER_STAT_PROC);

	page_count = 0;
	if (proc->pages) {
		int i;
		for (i = 0; i < proc->buffer_size / PAGE_SIZE; i++) {
			if (proc->pages[i]) {
				void *page_addr = proc->buffer + i * PAGE_SIZE;
				vdbinder_debug(VDBINDER_DEBUG_BUFFER_ALLOC,
					     "binder_release: %d: "
					     "page %d at %p not freed\n",
					     proc->pid, i,
					     page_addr);
				unmap_kernel_range((unsigned long)page_addr,
					PAGE_SIZE);
				__free_page(proc->pages[i]);
				page_count++;
			}
		}
		kfree(proc->pages);
		vfree(proc->buffer);
	}

	put_task_struct(proc->tsk);

	vdbinder_debug(VDBINDER_DEBUG_OPEN_CLOSE,
		     "binder_release: %d threads %d, nodes %d (ref %d), "
		     "refs %d, active transactions %d, buffers %d, "
		     "pages %d\n",
		     proc->pid, threads, nodes, incoming_refs, outgoing_refs,
		     active_transactions, buffers, page_count);

	kfree(proc);
}

static void binder_deferred_func(struct work_struct *work)
{
	struct vdbinder_proc *proc;
	struct files_struct *files;

	int defer;
	do {
		mutex_lock(&binder_lock);
		mutex_lock(&binder_deferred_lock);
		if (!hlist_empty(&binder_deferred_list)) {
			proc = hlist_entry(binder_deferred_list.first,
					struct vdbinder_proc, deferred_work_node);
			hlist_del_init(&proc->deferred_work_node);
			defer = proc->deferred_work;
			proc->deferred_work = 0;
		} else {
			proc = NULL;
			defer = 0;
		}
		mutex_unlock(&binder_deferred_lock);

		files = NULL;
		if (defer & VDBINDER_DEFERRED_PUT_FILES) {
			files = proc->files;
			if (files)
				proc->files = NULL;
		}


		if (defer & VDBINDER_DEFERRED_FLUSH)
			binder_deferred_flush(proc);

		if (defer & VDBINDER_DEFERRED_RELEASE) {
			delete_service_wait_list_by_proc(proc);
			binder_deferred_release(proc); /* frees proc */
		}

		mutex_unlock(&binder_lock);
		if (files)
			put_files_struct(files);
	} while (proc);
}
static DECLARE_WORK(binder_deferred_work, binder_deferred_func);

static void
binder_defer_work(struct vdbinder_proc *proc, enum vdbinder_deferred_state defer)
{
	mutex_lock(&binder_deferred_lock);
	proc->deferred_work |= defer;
	if (hlist_unhashed(&proc->deferred_work_node)) {
		hlist_add_head(&proc->deferred_work_node,
				&binder_deferred_list);
		queue_work(binder_deferred_workqueue, &binder_deferred_work);
	}
	mutex_unlock(&binder_deferred_lock);
}

static void print_binder_transaction(struct seq_file *m, const char *prefix,
				     struct vdbinder_transaction *t)
{
	seq_printf(m,
		   "%s %d: %p from %d:%d to %d:%d code %x flags %x pri %ld r%d",
		   prefix, t->debug_id, t,
		   t->from ? t->from->proc->pid : 0,
		   t->from ? t->from->pid : 0,
		   t->to_proc ? t->to_proc->pid : 0,
		   t->to_thread ? t->to_thread->pid : 0,
		   t->code, t->flags, t->priority, t->need_reply);
	if (t->buffer == NULL) {
		seq_puts(m, " buffer free\n");
		return;
	}
	if (t->buffer->target_node)
		seq_printf(m, " node %d",
			   t->buffer->target_node->debug_id);
	seq_printf(m, " size %zd:%zd data %p\n",
		   t->buffer->data_size, t->buffer->offsets_size,
		   t->buffer->data);
}

static void print_binder_buffer(struct seq_file *m, const char *prefix,
				struct vdbinder_buffer *buffer)
{
	seq_printf(m, "%s %d: %p size %zd:%zd %s\n",
		   prefix, buffer->debug_id, buffer->data,
		   buffer->data_size, buffer->offsets_size,
		   buffer->transaction ? "active" : "delivered");
}

static void print_binder_work(struct seq_file *m, const char *prefix,
			      const char *transaction_prefix,
			      struct vdbinder_work *w)
{
	struct vdbinder_node *node;
	struct vdbinder_transaction *t;

	switch (w->type) {
	case VDBINDER_WORK_TRANSACTION:
		t = container_of(w, struct vdbinder_transaction, work);
		print_binder_transaction(m, transaction_prefix, t);
		break;
	case VDBINDER_WORK_TRANSACTION_COMPLETE:
		seq_printf(m, "%stransaction complete\n", prefix);
		break;
	case VDBINDER_WORK_NODE:
		node = container_of(w, struct vdbinder_node, work);
		seq_printf(m, "%snode work %d: u%p c%p\n",
			   prefix, node->debug_id, node->ptr, node->cookie);
		break;
	case VDBINDER_WORK_DEAD_BINDER:
		seq_printf(m, "%shas dead binder\n", prefix);
		break;
	case VDBINDER_WORK_DEAD_BINDER_AND_CLEAR:
		seq_printf(m, "%shas cleared dead binder\n", prefix);
		break;
	case VDBINDER_WORK_CLEAR_DEATH_NOTIFICATION:
		seq_printf(m, "%shas cleared death notification\n", prefix);
		break;
	default:
		seq_printf(m, "%sunknown work: type %d\n", prefix, w->type);
		break;
	}
}

static void print_binder_thread(struct seq_file *m,
				struct vdbinder_thread *thread,
				int print_always)
{
	struct vdbinder_transaction *t;
	struct vdbinder_work *w;
	size_t start_pos = m->count;
	size_t header_pos;

	seq_printf(m, "  thread %d: l %02x\n", thread->pid, thread->looper);
	header_pos = m->count;
	t = thread->transaction_stack;
	while (t) {
		if (t->from == thread) {
			print_binder_transaction(m,
						 "    outgoing transaction", t);
			t = t->from_parent;
		} else if (t->to_thread == thread) {
			print_binder_transaction(m,
						 "    incoming transaction", t);
			t = t->to_parent;
		} else {
			print_binder_transaction(m, "    bad transaction", t);
			t = NULL;
		}
	}
	list_for_each_entry(w, &thread->todo, entry) {
		print_binder_work(m, "    ", "    pending transaction", w);
	}
	if (!print_always && m->count == header_pos)
		m->count = start_pos;
}

static void print_binder_node(struct seq_file *m, struct vdbinder_node *node)
{
	struct vdbinder_ref *ref;
	struct hlist_node *pos;
	struct vdbinder_work *w;
	int count;

	count = 0;
	hlist_for_each_entry(ref, pos, &node->refs, node_entry)
		count++;

	seq_printf(m, "  node %d: u%p c%p hs %d hw %d ls %d lw %d is %d iw %d",
		   node->debug_id, node->ptr, node->cookie,
		   node->has_strong_ref, node->has_weak_ref,
		   node->local_strong_refs, node->local_weak_refs,
		   node->internal_strong_refs, count);
	if (count) {
		seq_puts(m, " proc");
		hlist_for_each_entry(ref, pos, &node->refs, node_entry)
			seq_printf(m, " %d", ref->proc->pid);
	}
	seq_puts(m, "\n");
	list_for_each_entry(w, &node->async_todo, entry)
		print_binder_work(m, "    ",
				  "    pending async transaction", w);
}

static void print_binder_ref(struct seq_file *m, struct vdbinder_ref *ref)
{
	seq_printf(m, "  ref %d: desc %d %snode %d s %d w %d d %p\n",
		   ref->debug_id, ref->desc, ref->node->proc ? "" : "dead ",
		   ref->node->debug_id, ref->strong, ref->weak, ref->death);
}

static void print_binder_proc(struct seq_file *m,
			      struct vdbinder_proc *proc, int print_all)
{
	struct vdbinder_work *w;
	struct rb_node *n;
	size_t start_pos = m->count;
	size_t header_pos;

	seq_printf(m, "proc %d\n", proc->pid);
	header_pos = m->count;

	for (n = rb_first(&proc->threads); n != NULL; n = rb_next(n))
		print_binder_thread(m, rb_entry(n, struct vdbinder_thread,
						rb_node), print_all);
	for (n = rb_first(&proc->nodes); n != NULL; n = rb_next(n)) {
		struct vdbinder_node *node = rb_entry(n, struct vdbinder_node,
						    rb_node);
		if (print_all || node->has_async_transaction)
			print_binder_node(m, node);
	}
	if (print_all) {
		for (n = rb_first(&proc->refs_by_desc);
		     n != NULL;
		     n = rb_next(n))
			print_binder_ref(m, rb_entry(n, struct vdbinder_ref,
						     rb_node_desc));
	}
	for (n = rb_first(&proc->allocated_buffers); n != NULL; n = rb_next(n))
		print_binder_buffer(m, "  buffer",
				    rb_entry(n, struct vdbinder_buffer, rb_node));
	list_for_each_entry(w, &proc->todo, entry)
		print_binder_work(m, "  ", "  pending transaction", w);
	list_for_each_entry(w, &proc->delivered_death, entry) {
		seq_puts(m, "  has delivered dead binder\n");
		break;
	}
	if (!print_all && m->count == header_pos)
		m->count = start_pos;
}

static const char * const binder_return_strings[] = {
	"VD_BR_ERROR",
	"VD_BR_OK",
	"VD_BR_TRANSACTION",
	"VD_BR_REPLY",
	"VD_BR_ACQUIRE_RESULT",
	"VD_BR_DEAD_REPLY",
	"VD_BR_TRANSACTION_COMPLETE",
	"VD_BR_INCREFS",
	"VD_BR_ACQUIRE",
	"VD_BR_RELEASE",
	"VD_BR_DECREFS",
	"VD_BR_ATTEMPT_ACQUIRE",
	"VD_BR_NOOP",
	"VD_BR_SPAWN_LOOPER",
	"VD_BR_FINISHED",
	"VD_BR_DEAD_BINDER",
	"VD_BR_CLEAR_DEATH_NOTIFICATION_DONE",
	"VD_BR_FAILED_REPLY"
};

static const char * const binder_command_strings[] = {
	"VD_BC_TRANSACTION",
	"VD_BC_REPLY",
	"VD_BC_ACQUIRE_RESULT",
	"VD_BC_FREE_BUFFER",
	"VD_BC_INCREFS",
	"VD_BC_ACQUIRE",
	"VD_BC_RELEASE",
	"VD_BC_DECREFS",
	"VD_BC_INCREFS_DONE",
	"VD_BC_ACQUIRE_DONE",
	"VD_BC_ATTEMPT_ACQUIRE",
	"VD_BC_REGISTER_LOOPER",
	"VD_BC_ENTER_LOOPER",
	"VD_BC_EXIT_LOOPER",
	"VD_BC_REQUEST_DEATH_NOTIFICATION",
	"VD_BC_CLEAR_DEATH_NOTIFICATION",
	"VD_BC_DEAD_BINDER_DONE"
};

static const char * const binder_objstat_strings[] = {
	"proc",
	"thread",
	"node",
	"ref",
	"death",
	"transaction",
	"transaction_complete"
};

static void print_binder_stats(struct seq_file *m, const char *prefix,
			       struct vdbinder_stats *stats)
{
	int i;

	BUILD_BUG_ON(ARRAY_SIZE(stats->bc) !=
		     ARRAY_SIZE(binder_command_strings));
	for (i = 0; i < ARRAY_SIZE(stats->bc); i++) {
		if (stats->bc[i])
			seq_printf(m, "%s%s: %d\n", prefix,
				   binder_command_strings[i], stats->bc[i]);
	}

	BUILD_BUG_ON(ARRAY_SIZE(stats->br) !=
		     ARRAY_SIZE(binder_return_strings));
	for (i = 0; i < ARRAY_SIZE(stats->br); i++) {
		if (stats->br[i])
			seq_printf(m, "%s%s: %d\n", prefix,
				   binder_return_strings[i], stats->br[i]);
	}

	BUILD_BUG_ON(ARRAY_SIZE(stats->obj_created) !=
		     ARRAY_SIZE(binder_objstat_strings));
	BUILD_BUG_ON(ARRAY_SIZE(stats->obj_created) !=
		     ARRAY_SIZE(stats->obj_deleted));
	for (i = 0; i < ARRAY_SIZE(stats->obj_created); i++) {
		if (stats->obj_created[i] || stats->obj_deleted[i])
			seq_printf(m, "%s%s: active %d total %d\n", prefix,
				binder_objstat_strings[i],
				stats->obj_created[i] - stats->obj_deleted[i],
				stats->obj_created[i]);
	}
}

static void print_binder_proc_stats(struct seq_file *m,
				    struct vdbinder_proc *proc)
{
	struct vdbinder_work *w;
	struct rb_node *n;
	int count, strong, weak;

	seq_printf(m, "proc %d\n", proc->pid);
	count = 0;
	for (n = rb_first(&proc->threads); n != NULL; n = rb_next(n))
		count++;
	seq_printf(m, "  threads: %d\n", count);
	seq_printf(m, "  requested threads: %d+%d/%d\n"
			"  ready threads %d\n"
			"  free async space %zd\n", proc->requested_threads,
			proc->requested_threads_started, proc->max_threads,
			proc->ready_threads, proc->free_async_space);
	count = 0;
	for (n = rb_first(&proc->nodes); n != NULL; n = rb_next(n))
		count++;
	seq_printf(m, "  nodes: %d\n", count);
	count = 0;
	strong = 0;
	weak = 0;
	for (n = rb_first(&proc->refs_by_desc); n != NULL; n = rb_next(n)) {
		struct vdbinder_ref *ref = rb_entry(n, struct vdbinder_ref,
						  rb_node_desc);
		count++;
		strong += ref->strong;
		weak += ref->weak;
	}
	seq_printf(m, "  refs: %d s %d w %d\n", count, strong, weak);

	count = 0;
	for (n = rb_first(&proc->allocated_buffers); n != NULL; n = rb_next(n))
		count++;
	seq_printf(m, "  buffers: %d\n", count);

	count = 0;
	list_for_each_entry(w, &proc->todo, entry) {
		switch (w->type) {
		case VDBINDER_WORK_TRANSACTION:
			count++;
			break;
		default:
			break;
		}
	}
	seq_printf(m, "  pending transactions: %d\n", count);

	print_binder_stats(m, "  ", &proc->stats);
}


static int binder_state_show(struct seq_file *m, void *unused)
{
	struct vdbinder_proc *proc;
	struct hlist_node *pos;
	struct vdbinder_node *node;
	int do_lock = !binder_debug_no_lock;

	if (do_lock)
		mutex_lock(&binder_lock);

	seq_puts(m, "binder state:\n");

	if (!hlist_empty(&binder_dead_nodes))
		seq_puts(m, "dead nodes:\n");
	hlist_for_each_entry(node, pos, &binder_dead_nodes, dead_node)
		print_binder_node(m, node);

	hlist_for_each_entry(proc, pos, &binder_procs, proc_node)
		print_binder_proc(m, proc, 1);
	if (do_lock)
		mutex_unlock(&binder_lock);
	return 0;
}

static int binder_stats_show(struct seq_file *m, void *unused)
{
	struct vdbinder_proc *proc;
	struct hlist_node *pos;
	int do_lock = !binder_debug_no_lock;

	if (do_lock)
		mutex_lock(&binder_lock);

	seq_puts(m, "binder stats:\n");

	print_binder_stats(m, "", &vdbinder_stats);

	hlist_for_each_entry(proc, pos, &binder_procs, proc_node)
		print_binder_proc_stats(m, proc);
	if (do_lock)
		mutex_unlock(&binder_lock);
	return 0;
}

static int binder_transactions_show(struct seq_file *m, void *unused)
{
	struct vdbinder_proc *proc;
	struct hlist_node *pos;
	int do_lock = !binder_debug_no_lock;

	if (do_lock)
		mutex_lock(&binder_lock);

	seq_puts(m, "binder transactions:\n");
	hlist_for_each_entry(proc, pos, &binder_procs, proc_node)
		print_binder_proc(m, proc, 0);
	if (do_lock)
		mutex_unlock(&binder_lock);
	return 0;
}

static int binder_proc_show(struct seq_file *m, void *unused)
{
	struct vdbinder_proc *proc = m->private;
	int do_lock = !binder_debug_no_lock;

	if (do_lock)
		mutex_lock(&binder_lock);
	seq_puts(m, "binder proc state:\n");
	print_binder_proc(m, proc, 1);
	if (do_lock)
		mutex_unlock(&binder_lock);
	return 0;
}

static void print_binder_transaction_log_entry(struct seq_file *m,
					struct vdbinder_transaction_log_entry *e)
{
	seq_printf(m,
		   "%d: %s from %d:%d to %d:%d node %d handle %d size %d:%d\n",
		   e->debug_id, (e->call_type == 2) ? "reply" :
		   ((e->call_type == 1) ? "async" : "call "), e->from_proc,
		   e->from_thread, e->to_proc, e->to_thread, e->to_node,
		   e->target_handle, e->data_size, e->offsets_size);
}

static int binder_transaction_log_show(struct seq_file *m, void *unused)
{
	struct vdbinder_transaction_log *log = m->private;
	int i;

	if (log->full) {
		for (i = log->next; i < ARRAY_SIZE(log->entry); i++)
			print_binder_transaction_log_entry(m, &log->entry[i]);
	}
	for (i = 0; i < log->next; i++)
		print_binder_transaction_log_entry(m, &log->entry[i]);
	return 0;
}

static const struct file_operations binder_fops = {
	.owner = THIS_MODULE,
	.poll = binder_poll,
	.unlocked_ioctl = binder_ioctl,
	.mmap = binder_mmap,
	.open = binder_open,
	.flush = binder_flush,
	.release = binder_release,
};

static struct miscdevice binder_miscdev = {
	.minor = VDBINDER_MINOR,
	.name = "vdbinder",
	.fops = &binder_fops
};

VDBINDER_DEBUG_ENTRY(state);
VDBINDER_DEBUG_ENTRY(stats);
VDBINDER_DEBUG_ENTRY(transactions);
VDBINDER_DEBUG_ENTRY(transaction_log);

static int __init binder_init(void)
{
	int ret;
	binder_deferred_workqueue = create_singlethread_workqueue("vdbinder");
	if (!binder_deferred_workqueue)
		return -ENOMEM;

	binder_debugfs_dir_entry_root = debugfs_create_dir("vdbinder", NULL);
	if (binder_debugfs_dir_entry_root)
		binder_debugfs_dir_entry_proc = debugfs_create_dir("proc",
						 binder_debugfs_dir_entry_root);
	ret = misc_register(&binder_miscdev);
	if (binder_debugfs_dir_entry_root) {
		debugfs_create_file("state",
				    S_IRUGO,
				    binder_debugfs_dir_entry_root,
				    NULL,
				    &binder_state_fops);
		debugfs_create_file("stats",
				    S_IRUGO,
				    binder_debugfs_dir_entry_root,
				    NULL,
				    &binder_stats_fops);
		debugfs_create_file("transactions",
				    S_IRUGO,
				    binder_debugfs_dir_entry_root,
				    NULL,
				    &binder_transactions_fops);
		debugfs_create_file("transaction_log",
				    S_IRUGO,
				    binder_debugfs_dir_entry_root,
				    &vdbinder_transaction_log,
				    &binder_transaction_log_fops);
		debugfs_create_file("failed_transaction_log",
				    S_IRUGO,
				    binder_debugfs_dir_entry_root,
				    &binder_transaction_log_failed,
				    &binder_transaction_log_fops);
	}
	return ret;
}

static int __init vdboard_maintv(char *str)
{
	binder_this_board_no = 0; /* maintv */
	return 0;
}

static int __init vdboard_sbb(char *str)
{
	binder_this_board_no = 1; /* sbb */
	return 0;
}

early_param("maintv", vdboard_maintv);
early_param("sbb", vdboard_sbb);

device_initcall(binder_init);

MODULE_LICENSE("GPL v2");
