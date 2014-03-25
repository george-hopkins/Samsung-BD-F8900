/*
 * Copyright (C) 2008 Google, Inc.
 *
 * Based on, but no longer compatible with, the original
 * OpenBinder.org binder driver interface, which is:
 *
 * Copyright (c) 2005 Palmsource, Inc.
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

#ifndef _LINUX_VDBINDER_INTERNAL_H
#define _LINUX_VDBINDER_INTERNAL_H

extern struct hlist_head binder_procs;
extern struct mutex binder_lock;
extern int binder_this_board_no;

enum vdbinder_stat_types {
	VDBINDER_STAT_PROC,
	VDBINDER_STAT_THREAD,
	VDBINDER_STAT_NODE,
	VDBINDER_STAT_REF,
	VDBINDER_STAT_DEATH,
	VDBINDER_STAT_TRANSACTION,
	VDBINDER_STAT_TRANSACTION_COMPLETE,
	VDBINDER_STAT_COUNT
};

struct vdbinder_stats {
	int br[_IOC_NR(VD_BR_FAILED_REPLY) + 1];
	int bc[_IOC_NR(VD_BC_DEAD_BINDER_DONE) + 1];
	int obj_created[VDBINDER_STAT_COUNT];
	int obj_deleted[VDBINDER_STAT_COUNT];
};

struct vdbinder_work {
	struct list_head entry;
	enum {
		VDBINDER_WORK_TRANSACTION = 1,
		VDBINDER_WORK_TRANSACTION_COMPLETE,
		VDBINDER_WORK_NODE,
		VDBINDER_WORK_DEAD_BINDER,
		VDBINDER_WORK_DEAD_BINDER_AND_CLEAR,
		VDBINDER_WORK_CLEAR_DEATH_NOTIFICATION,
	} type;
#ifdef CONFIG_ORSAY_DEBUG
	int owner;
#endif
};

struct vdbinder_node {
	int debug_id;
	struct vdbinder_work work;
	union {
		struct rb_node rb_node;
		struct hlist_node dead_node;
	};
	struct vdbinder_proc *proc;
	struct hlist_head refs;
	int internal_strong_refs;
	int local_weak_refs;
	int local_strong_refs;
	void __user *ptr;
	void __user *cookie;
	unsigned has_strong_ref:1;
	unsigned pending_strong_ref:1;
	unsigned has_weak_ref:1;
	unsigned pending_weak_ref:1;
	unsigned has_async_transaction:1;
	unsigned accept_fds:1;
	unsigned min_priority:8;
	struct list_head async_todo;
};

struct vdbinder_buffer {
	struct list_head entry; /* free and allocated entries by addesss */
	struct rb_node rb_node; /* free entry by size or allocated entry */
				/* by address */
	unsigned free:1;
	unsigned allow_user_free:1;
	unsigned async_transaction:1;
	unsigned debug_id:29;

	struct vdbinder_transaction *transaction;

	struct vdbinder_node *target_node;
	size_t data_size;
	size_t offsets_size;
	uint8_t data[0];
};

struct vdbinder_proc {
	struct hlist_node proc_node;
	struct rb_root threads;
	struct rb_root nodes;
	struct rb_root refs_by_desc;
	struct rb_root refs_by_node;
	int pid;
	struct vm_area_struct *vma;
	struct task_struct *tsk;
	struct files_struct *files;
	struct hlist_node deferred_work_node;
	int deferred_work;
	void *buffer;
	ptrdiff_t user_buffer_offset;

	struct list_head buffers;
	struct rb_root free_buffers;
	struct rb_root allocated_buffers;
	size_t free_async_space;

	struct page **pages;
	size_t buffer_size;
	uint32_t buffer_free;
	struct list_head todo;
	wait_queue_head_t wait;
	struct vdbinder_stats stats;
	struct list_head delivered_death;
	int max_threads;
	int requested_threads;
	int requested_threads_started;
	int ready_threads;
	long default_priority;
	struct dentry *debugfs_entry;
	struct vdbinder_thread *dbg_thread;
};

struct vdbinder_thread {
	struct vdbinder_proc *proc;
	struct rb_node rb_node;
	int pid;
	int looper;
	struct vdbinder_transaction *transaction_stack;
	struct vdbinder_dbg_transaction *dbg_stack;
	struct list_head todo;
	uint32_t return_error; /* Write failed, return error code in read buf */
	uint32_t return_error2; /* Write failed, return error code in read */
		/* buffer. Used when sending a reply to a dead process that */
		/* we are also waiting on */
	wait_queue_head_t wait;
	struct vdbinder_stats stats;
	unsigned long jiffy;
	struct task_struct *tsk;
	struct dbg_transact dbg_entry;
};

struct vdbinder_transaction {
	int debug_id;
	struct vdbinder_work work;
	struct vdbinder_thread *from;
	struct vdbinder_transaction *from_parent;
	struct vdbinder_proc *to_proc;
	struct vdbinder_thread *to_thread;
	struct vdbinder_transaction *to_parent;
	unsigned need_reply:1;
	/* unsigned is_dead:1; */	/* not used at the moment */

	struct vdbinder_buffer *buffer;
	unsigned int	code;
	unsigned int	flags;
	unsigned int	isEnc;
	unsigned int	isShrd;
	long	priority;
	long	saved_priority;
	uid_t	sender_euid;
};

void binder_add_to_debug_db(struct vdbinder_thread *curr_thread);
void binder_remove_from_debug_db(struct vdbinder_thread *curr_thread);
int binder_set_debug_thread(struct vdbinder_proc *proc,
	struct vdbinder_thread *thread);
int binder_set_rpcagent_thread(struct vdbinder_thread *thread);
int binder_dbg_operation(unsigned int size, void __user *ubuf,
	struct vdbinder_proc *proc,
	struct vdbinder_thread *thread);
int binder_get_dbg_version(unsigned int size, void __user *ubuf);
void binder_dbg_free_thread(struct vdbinder_thread *thread);

#endif
