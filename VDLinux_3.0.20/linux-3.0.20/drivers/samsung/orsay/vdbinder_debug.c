/* vdbinder_debug.c
 * version : VDBinder Debug Ver.2
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

#include <linux/list.h>
#include <linux/mm.h>
#include <linux/mutex.h>
#include <linux/rbtree.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/vmalloc.h>

#include "vdbinder.h"
#include "vdbinder_debug.h"
#include "vdbinder_internal.h"

static LIST_HEAD(dbg_transact_list);
static struct vdbinder_thread *binder_rpc_agent_thread;/*RPCAgent Thread*/
static struct vdbinder_transaction_fail
	dbg_transact_fail_mem[MAX_VDBINDER_LOG_COUNT];
static unsigned long transact_fail_curr_offset;

struct vdbinder_dbg_work {
	struct list_head entry;
	enum {
		VDBINDER_WORK_SET_LOG_LEVEL = 1,
		VDBINDER_WORK_GET_HANG_THREAD,
		VDBINDER_WORK_GET_HANG_TASK,
		VDBINDER_WORK_GET_OBJ_NAME,
		VDBINDER_WORK_DBG_COMPLETE,
		VDBINDER_WORK_THREAD_FINISH,
		VDBINDER_WORK_HANG_THREAD_COMPLETE,
		VDBINDER_WORK_OBJ_NAME_COMPLETE,
		VDBINDER_WORK_TRANSACTION_FAIL_LOG,
		VDBINDER_WORK_REMOTE_TRANSACTION_FAIL_LOG,
		VDBINDER_WORK_SET_VDBINDER_FAIL_FLAG,
	} type;
};

struct vdbinder_dbg_transaction {
	struct vdbinder_dbg_work work;
	struct vdbinder_thread *from;
	struct vdbinder_dbg_transaction *from_parent;
	struct vdbinder_proc *to_proc;
	struct vdbinder_thread *to_thread;
	struct vdbinder_dbg_transaction *to_parent;
	void *data;
	uint32_t data_size;
};

void binder_add_to_debug_db(struct vdbinder_thread *curr_thread)
{
	BUG_ON(list_empty(&curr_thread->dbg_entry.entry) == false);
	BUG_ON(curr_thread->dbg_entry.type == TRANSACTION_START);
	if (list_empty(&curr_thread->dbg_entry.entry) == true) {
		list_add_tail(&curr_thread->dbg_entry.entry,
					&dbg_transact_list);
		curr_thread->jiffy = jiffies;
		curr_thread->dbg_entry.type = TRANSACTION_START;
	}
}

void binder_remove_from_debug_db(struct vdbinder_thread *curr_thread)
{
	BUG_ON(list_empty(&curr_thread->dbg_entry.entry) == true);
	BUG_ON(curr_thread->dbg_entry.type == TRANSACTION_FINISH);
	if (list_empty(&curr_thread->dbg_entry.entry) == false) {
		list_del_init(&curr_thread->dbg_entry.entry);
		curr_thread->jiffy = 0;
		curr_thread->dbg_entry.type = TRANSACTION_FINISH;
	}
}

static uint32_t binder_get_hang_stat(struct vdbinder_hang_stat **hang_stat)
{
	struct vdbinder_thread *thread;
	struct vdbinder_hang_stat *hang_stat_buff;
	struct vdbinder_transaction *t;
	int  pos = 0;
	uint32_t count = 0;
	struct list_head *list_pos, *list_q;

	list_for_each_safe(list_pos, list_q, &dbg_transact_list)
		count++;

	if (!count)
		return 0;
	hang_stat_buff = kzalloc(count*sizeof(struct vdbinder_hang_stat),
		GFP_ATOMIC);
	if (!hang_stat_buff) {
		printk(KERN_ERR "NO memory for local Hang Stat\n");
		return 0;
	}
	list_for_each_safe(list_pos, list_q, &dbg_transact_list) {
		thread = list_entry(list_pos, struct vdbinder_thread,
					dbg_entry.entry);
		t = thread->transaction_stack;
		strlcpy(hang_stat_buff[pos].sender_thread,
				thread->tsk->comm, MAX_FUNC_LEN);
		strlcpy(hang_stat_buff[pos].sender_proc,
				thread->proc->tsk->comm, MAX_FUNC_LEN);

		if (t) {
			if (t->to_thread) {
				strlcpy(hang_stat_buff[pos].target,
					t->to_thread->tsk->comm, MAX_FUNC_LEN);
				hang_stat_buff[pos].target_pid =
					t->to_thread->pid;
			} else {
				strlcpy(hang_stat_buff[pos].target,
					"#",
					MAX_FUNC_LEN);
				hang_stat_buff[pos].target_pid = 0;
			}
			hang_stat_buff[pos].obj = t->buffer->target_node->ptr;
			hang_stat_buff[pos].func_code = t->code;
		} else {
			strlcpy(hang_stat_buff[pos].target, "-", MAX_FUNC_LEN);
			hang_stat_buff[pos].target_pid = 0;
			hang_stat_buff[pos].obj = NULL;
			hang_stat_buff[pos].func_code = 0xffff;
		}
		hang_stat_buff[pos].wait_ticks = jiffies - thread->jiffy;
		hang_stat_buff[pos].sender_pid = thread->pid;
		hang_stat_buff[pos].board_num = binder_this_board_no;
		pos++;
	}
	*hang_stat = hang_stat_buff;
	return pos * sizeof(struct vdbinder_hang_stat);
}

static int binder_send_debug_work(uint32_t work_cmd,
				void __user *buffer,
				uint32_t size,
				struct vdbinder_thread *parent_thread,
				struct vdbinder_thread *target_thread)
{
	int ret = 0;
	struct vdbinder_dbg_transaction *t_rpc;
	if (work_cmd == VDBINDER_WORK_HANG_THREAD_COMPLETE ||
			work_cmd == VDBINDER_WORK_OBJ_NAME_COMPLETE)
		t_rpc = parent_thread->dbg_stack;
	else
		t_rpc = kzalloc(sizeof(struct vdbinder_dbg_transaction),
					GFP_KERNEL);
	if (!t_rpc) {
		printk(KERN_ERR "vdbinder_dbg_transaction:"
						" NOMEM");
		return -ENOMEM;
	}
	if (size) {
		t_rpc->data = kzalloc(size, GFP_KERNEL);
		if (!t_rpc->data) {
			printk(KERN_ERR "allocating t_rpc->data: NOMEM");
			kfree(t_rpc);
			return -ENOMEM;
		}
		memcpy(t_rpc->data, buffer, size);
	}
	t_rpc->data_size = size;
	t_rpc->work.type = work_cmd;
	t_rpc->from = parent_thread;
	t_rpc->from_parent = parent_thread->dbg_stack;
	parent_thread->dbg_stack = t_rpc;
	t_rpc->to_thread = target_thread;
	list_add_tail(&t_rpc->work.entry,
		&target_thread->todo);
	if (&target_thread->wait)
		wake_up_interruptible(
			&target_thread->wait);
	return ret;
}

static int binder_add_self_work(struct vdbinder_thread *thread,
				unsigned int type)
{
	int ret = 0;
	struct vdbinder_dbg_work *tcomplete;
	tcomplete = kzalloc(sizeof(struct vdbinder_dbg_work),
					GFP_KERNEL);
	if (!tcomplete) {
		printk(KERN_ERR "vdbinder_dbg_work: NOMEM");
		return -ENOMEM;
	}
	tcomplete->type = type;
	list_add_tail(&tcomplete->entry, &thread->todo);
	return ret;
}

static int binder_dbg_thread_write(struct vdbinder_proc *proc,
			struct vdbinder_thread *thread,
			void __user *buffer, int size)
{
	int ret = 0;
	uint32_t cmd;
	void __user *ptr = buffer;
	if (get_user(cmd, (uint32_t __user *)ptr))
		return -EFAULT;
	ptr += sizeof(uint32_t);
	switch (cmd) {
	case VD_BC_SET_LOG_LEVEL: {
		struct vdbinder_proc *debug_proc;
		struct hlist_node *pos;
		ret = binder_add_self_work(thread, VDBINDER_WORK_DBG_COMPLETE);
		if (ret < 0)
			return ret;

		/*add work in RPCAgent list and wake up*/
		if (binder_rpc_agent_thread) {
			ret =
			binder_send_debug_work(VDBINDER_WORK_SET_LOG_LEVEL,
					ptr,
					sizeof(struct vdbinder_log_info),
					thread, binder_rpc_agent_thread);
			if (ret < 0)
				return ret;
		}

		/*add work in all dbg threads list and wake up*/
		hlist_for_each_entry(debug_proc, pos,
				&binder_procs, proc_node) {
			if (debug_proc->dbg_thread) {
				ret =
					binder_send_debug_work(
					VDBINDER_WORK_SET_LOG_LEVEL,
					ptr,
					sizeof(struct vdbinder_log_info),
					thread, debug_proc->dbg_thread);
				if (ret < 0)
					return ret;
			}
		}
	}
	break;
	case VD_BC_GET_HANG_THREAD: {
		if (binder_rpc_agent_thread) {
			ret =
				binder_send_debug_work(
					VDBINDER_WORK_GET_HANG_THREAD,
					NULL, 0, thread,
					binder_rpc_agent_thread);
			if (ret < 0)
				return ret;
		} else {
			binder_add_self_work(thread,
				VDBINDER_WORK_HANG_THREAD_COMPLETE);
		}
	}
	break;
	case VD_BC_PUT_HANG_THREAD: {
		struct vdbinder_thread *target_thread = thread->dbg_stack->from;
		uint32_t actual_data_size = size - sizeof(uint32_t);
		ret =
			binder_send_debug_work(
				VDBINDER_WORK_HANG_THREAD_COMPLETE,
				ptr, actual_data_size, thread,
				target_thread);
		if (ret < 0)
			return ret;
		ret = binder_add_self_work(thread, VDBINDER_WORK_DBG_COMPLETE);
		if (ret < 0)
			return ret;
	}
	break;
	case VD_BC_DEBUG_THREAD_FINISH: {
		ret = binder_add_self_work(thread, VDBINDER_WORK_DBG_COMPLETE);
		if (ret < 0)
			return ret;
		ret =
			binder_send_debug_work(
				VDBINDER_WORK_THREAD_FINISH,
				NULL, 0, thread,
				thread->proc->dbg_thread);
		if (ret < 0)
			return ret;
	}
	break;
	case VD_BC_GET_OBJ_NAME: {
		int is_target_thread_found = 0;
		struct vdbinder_obj_info *obj_info;
		struct vdbinder_proc *debug_proc;
		struct hlist_node *pos;
		obj_info = (struct vdbinder_obj_info *)(ptr);
		if (obj_info->board_num == binder_this_board_no) {
			hlist_for_each_entry(debug_proc, pos,
					&binder_procs, proc_node) {
				struct vdbinder_thread *target_thread = NULL;
				struct rb_node *parent = NULL;
				struct rb_node **p =
					&debug_proc->threads.rb_node;

				while (*p) {
					parent = *p;
					target_thread = rb_entry(parent,
							struct vdbinder_thread,
							rb_node);
					if (obj_info->target_pid <
						target_thread->pid)
						p = &(*p)->rb_left;
					else if (obj_info->target_pid >
							target_thread->pid)
						p = &(*p)->rb_right;
					else {
						is_target_thread_found = 1;
						ret =
						binder_send_debug_work(
					VDBINDER_WORK_GET_OBJ_NAME,
					ptr,
					sizeof(struct vdbinder_obj_info),
						thread, debug_proc->dbg_thread);
						return ret;
					}
				}
			}
			if (!is_target_thread_found) {
				ret = binder_add_self_work(thread,
					VDBINDER_WORK_DBG_COMPLETE);
				if (ret < 0)
					return ret;
			}
		} else {
		/*if the native is on other board*/
			if (binder_rpc_agent_thread) {
				ret =
				binder_send_debug_work(
					VDBINDER_WORK_GET_OBJ_NAME,
					ptr,
					sizeof(struct vdbinder_obj_info),
					thread, binder_rpc_agent_thread);
				if (ret < 0)
					return ret;
			}
		}
	}
	break;
	case VD_BC_PUT_OBJ_NAME: {
		struct vdbinder_thread *target_thread = thread->dbg_stack->from;
		uint32_t actual_data_size = size - sizeof(uint32_t);
		ret =
			binder_send_debug_work(
			VDBINDER_WORK_OBJ_NAME_COMPLETE,
			ptr, actual_data_size, thread,
			target_thread);
		if (ret < 0)
			return ret;
		ret = binder_add_self_work(thread, VDBINDER_WORK_DBG_COMPLETE);
		if (ret < 0)
			return ret;
	}
	break;
	case VD_BC_SET_TRANSACT_FAIL_LOG: {
		struct vdbinder_transaction_fail *dest_ptr = NULL;
		uint32_t write_offset = 0;
		uint32_t actual_data_size = size - sizeof(uint32_t);
		if (actual_data_size !=
			sizeof(struct vdbinder_transaction_fail)) {
			printk(KERN_ERR "VD_BC_SET_TRANSACT_FAIL_LOG:"
					"Failed to set Transact fail"
					" log: EINVAL\n");
			return -EINVAL;
		}
		write_offset = (transact_fail_curr_offset %
					MAX_VDBINDER_LOG_COUNT);
		dest_ptr = &dbg_transact_fail_mem[write_offset];
		if (copy_from_user(dest_ptr, ptr, actual_data_size)) {
			printk(KERN_ERR "VD_BC_SET_TRANSACT_FAIL_LOG:"
					"Failed to copy data: ENOMEM\n");
			return -EFAULT;
		}
		transact_fail_curr_offset++;
		ret = binder_add_self_work(thread, VDBINDER_WORK_DBG_COMPLETE);
		if (ret < 0)
			return ret;
	}
	break;
	case VD_BC_GET_TRANSACT_FAIL_LOG: {
		if (binder_rpc_agent_thread) {
			ret =
				binder_send_debug_work(
				VDBINDER_WORK_REMOTE_TRANSACTION_FAIL_LOG,
				NULL, 0, thread,
				binder_rpc_agent_thread);
			if (ret < 0)
				return ret;
		} else {
			binder_add_self_work(thread,
				VDBINDER_WORK_TRANSACTION_FAIL_LOG);
		}
	}
	break;
	case VD_BC_PUT_REMOTE_TRANSACTION_FAIL_LOG: {
		struct vdbinder_thread *target_thread = thread->dbg_stack->from;
		uint32_t actual_data_size = size - sizeof(uint32_t);
		ret =
			binder_send_debug_work(
				VDBINDER_WORK_TRANSACTION_FAIL_LOG,
				ptr, actual_data_size, thread,
				target_thread);
		if (ret < 0)
			return ret;
		ret = binder_add_self_work(thread, VDBINDER_WORK_DBG_COMPLETE);
		if (ret < 0)
			return ret;
	}
	break;
	case VD_BC_SET_VDBINDER_FAIL_FLAG: {
		struct vdbinder_proc *debug_proc;
		struct hlist_node *pos;
		ret = binder_add_self_work(thread, VDBINDER_WORK_DBG_COMPLETE);
		if (ret < 0)
			return ret;
		/*add work in RPCAgent list and wake up*/
		if (binder_rpc_agent_thread) {
			ret =
			binder_send_debug_work(
					VDBINDER_WORK_SET_VDBINDER_FAIL_FLAG,
					ptr,
					sizeof(uint32_t),
					thread, binder_rpc_agent_thread);
			if (ret < 0)
				return ret;
		}

		/*add work in all dbg threads list and wake up*/
		hlist_for_each_entry(debug_proc, pos,
				&binder_procs, proc_node) {
			if (debug_proc->dbg_thread) {
				ret =
					binder_send_debug_work(
					VDBINDER_WORK_SET_VDBINDER_FAIL_FLAG,
					ptr,
					sizeof(uint32_t),
					thread, debug_proc->dbg_thread);
				if (ret < 0)
					return ret;
			}
		}
	}
	break;
	default:
		printk(KERN_ERR "binder: %d:%d unknown command %d\n",
		       proc->pid, thread->pid, cmd);
		return -EINVAL;
	}
	return 0;
}

static int binder_dbg_thread_read(struct vdbinder_proc *proc,
				struct vdbinder_thread *thread,
				void __user *buffer, int *size)
{
	void __user *ptr = buffer;
	uint32_t cmd;
	int ret = 0;
	struct vdbinder_dbg_work *w;
	struct vdbinder_dbg_transaction *t = NULL;

retry:

	mutex_unlock(&binder_lock);
	ret = wait_event_interruptible(thread->wait,
				!list_empty(&thread->todo));
	mutex_lock(&binder_lock);

	if (ret)
		return ret;

	if (!list_empty(&thread->todo))
		w = list_first_entry(&thread->todo,
				struct vdbinder_dbg_work, entry);
	else {
		printk(KERN_ERR "Empty thread todo list\n");
		goto retry;
	}
	switch (w->type) {
	case VDBINDER_WORK_SET_LOG_LEVEL: {
		t = container_of(w, struct vdbinder_dbg_transaction, work);
		if (t->to_thread != thread) {
			printk(KERN_ERR "Incorrect dbg transaction\n");
			goto retry;
		}
		cmd = VD_BR_SET_LOG_LEVEL;
		if (put_user(cmd, (uint32_t __user *)ptr)) {
			ret = -EFAULT;
			goto out;
		}
		ptr += sizeof(uint32_t);
		*size += sizeof(uint32_t);
		if (copy_to_user(ptr, t->data,
			sizeof(struct vdbinder_log_info))) {
			ret = -EFAULT;
			goto out;
		}
		*size += sizeof(struct vdbinder_log_info);
		kfree(t->data);
		list_del(&t->work.entry);
		kfree(t);
	} break;
	case VDBINDER_WORK_THREAD_FINISH: {
		t = container_of(w, struct vdbinder_dbg_transaction, work);
		if (t->to_thread != thread) {
			printk(KERN_ERR "Incorrect dbg transaction\n");
			goto retry;
		}
		cmd = VD_BR_THREAD_FINISH;
		if (put_user(cmd, (uint32_t __user *)ptr))
			return -EFAULT;
		ptr += sizeof(uint32_t);
		*size += sizeof(uint32_t);
		list_del(&t->work.entry);
		kfree(t);
		thread->proc->dbg_thread = NULL;
	} break;

	case VDBINDER_WORK_DBG_COMPLETE: {
		cmd = VD_BR_DBG_WORK_DONE;
		if (put_user(cmd, (uint32_t __user *)ptr))
			return -EFAULT;
		ptr += sizeof(uint32_t);
		*size += sizeof(uint32_t);
		list_del(&w->entry);
		kfree(w);
	} break;
	case VDBINDER_WORK_GET_HANG_THREAD: {
		t = container_of(w, struct vdbinder_dbg_transaction, work);
		if (t->to_thread != thread) {
			printk(KERN_ERR "Incorrect dbg transaction\n");
			goto retry;
		}
		thread->dbg_stack = t;
		cmd = VD_BR_GET_HANG_THREAD;
		if (put_user(cmd, (uint32_t __user *)ptr))
			return -EFAULT;
		ptr += sizeof(uint32_t);
		*size += sizeof(uint32_t);
		list_del(&t->work.entry);
	} break;
	case VDBINDER_WORK_HANG_THREAD_COMPLETE: {
		struct vdbinder_hang_stat *hang_stat = NULL;
		uint32_t data_size = 0;
		/*cmd = VD_BR_DBG_HANG_THREAD_DONE;
		if (put_user(cmd, (uint32_t __user *)ptr))
			return -EFAULT;
		ptr += sizeof(uint32_t);
		*size += sizeof(uint32_t);*/
		if (binder_rpc_agent_thread) {
			t = container_of(w,
				struct vdbinder_dbg_transaction, work);
			if (t->to_thread != thread) {
				printk(KERN_ERR "VDBINDER_WORK_"
					"HANG_THREAD_COMPLETE: EINVAL\n");
				ret = -EINVAL;
				goto out;
			}
			if (copy_to_user(ptr, t->data, t->data_size)) {
				printk(KERN_ERR "VDBINDER_WORK_HANG"
					"_THREAD_COMPLETE: EFAULT\n");
				ret = -EFAULT;
				goto out;
			}
			ptr += t->data_size;
			*size += t->data_size;
			list_del(&t->work.entry);
			kfree(t->data);
			kfree(t);
		} else {
			list_del(&w->entry);
			kfree(w);
		}
		data_size = binder_get_hang_stat(&hang_stat);
		if (data_size >
			MAX_VDBINDER_HANG_STAT *
			sizeof(struct vdbinder_hang_stat))
			data_size =
				MAX_VDBINDER_HANG_STAT *
				sizeof(struct vdbinder_hang_stat);
		if (data_size) {
			if (copy_to_user(ptr, hang_stat, data_size)) {
				printk(KERN_ERR "Incorrect dbg transaction\n");
				kfree(hang_stat);
				return -EFAULT;
			}
		}
		kfree(hang_stat);
		*size += data_size;
	} break;
	case VDBINDER_WORK_GET_OBJ_NAME: {
		t = container_of(w,
		struct vdbinder_dbg_transaction, work);
		if (t->to_thread != thread) {
			printk(KERN_ERR "VDBINDER_WORK_GET_"
				"OBJ_NAME: invalid input EINVAL\n");
			ret = -EINVAL;
			goto out;
		}
		thread->dbg_stack = t;
		cmd = VD_BR_GET_OBJ_NAME;
		if (put_user(cmd, (uint32_t __user *)ptr)) {
			printk(KERN_ERR "Failed to send command:"
				"VD_BR_GET_OBJ_NAME: EFAULT\n");
			ret = -EFAULT;
			goto out;
		}
		ptr += sizeof(uint32_t);
		*size += sizeof(uint32_t);
		if (copy_to_user(ptr, t->data, t->data_size)) {
			printk(KERN_ERR "Failed to send data:"
					"VDBINDER_WORK_GET_OBJ_NAME:EFAULT\n");
			ret = -EFAULT;
			goto out;
		}
		ptr += t->data_size;
		*size += t->data_size;
		list_del(&t->work.entry);
		kfree(t->data);
	}
	break;
	case VDBINDER_WORK_OBJ_NAME_COMPLETE: {
		t = container_of(w,
			struct vdbinder_dbg_transaction, work);
		if (t->to_thread != thread) {
			printk(KERN_ERR "VDBINDER_WORK_OBJ_NAME"
					"_COMPLETE: EINVAL\n");
			ret = -EINVAL;
			goto out;
		}
		if (copy_to_user(ptr, t->data, t->data_size)) {
			printk(KERN_ERR "Failed to send data: VDBINDER_WORK"
					"_OBJ_NAME_COMPLETE: EFAULT\n");
			ret = -EFAULT;
			goto out;
		}
		ptr += t->data_size;
		*size += t->data_size;
		list_del(&t->work.entry);
		kfree(t->data);
		kfree(t);
	}
	break;
	case VDBINDER_WORK_TRANSACTION_FAIL_LOG: {
		uint32_t data_size = 0;
		if (binder_rpc_agent_thread) {
			t = container_of(w,
				struct vdbinder_dbg_transaction, work);
			if (t->to_thread != thread) {
				printk(KERN_ERR "VDBINDER_WORK_"
					"TRANSACTION_FAIL_LOG: EINVAL\n");
				ret = -EINVAL;
				goto out;
			}
			if (copy_to_user(ptr, t->data, t->data_size)) {
				printk(KERN_ERR "VDBINDER_WORK_"
					"TRANSACTION_FAIL_LOG: EFAULT\n");
				ret = -EFAULT;
				goto out;
			}
			ptr += t->data_size;
			*size += t->data_size;
			list_del(&t->work.entry);
			kfree(t->data);
			kfree(t);
		} else {
			list_del(&w->entry);
			kfree(w);
		}

		if (transact_fail_curr_offset >= MAX_VDBINDER_LOG_COUNT)
			data_size = MAX_VDBINDER_LOG_COUNT *
				sizeof(struct vdbinder_transaction_fail);
		else
			data_size = transact_fail_curr_offset *
				sizeof(struct vdbinder_transaction_fail);
		if (copy_to_user(ptr, dbg_transact_fail_mem, data_size)) {
			printk(KERN_ERR "VDBINDER_WORK_TRANSACTION_FAIL_LOG:"
					"Failed to send datan");
			return -EFAULT;
		}
		*size += data_size;
	} break;
	case VDBINDER_WORK_REMOTE_TRANSACTION_FAIL_LOG: {
		t = container_of(w, struct vdbinder_dbg_transaction, work);
		if (t->to_thread != thread) {
			printk(KERN_ERR "Incorrect dbg transaction\n");
			goto retry;
		}
		thread->dbg_stack = t;
		cmd = VD_BR_GET_REMOTE_TRANSACTION_FAIL_LOG;
		if (put_user(cmd, (uint32_t __user *)ptr))
			return -EFAULT;
		ptr += sizeof(uint32_t);
		*size += sizeof(uint32_t);
		list_del(&t->work.entry);
	} break;
	case VDBINDER_WORK_SET_VDBINDER_FAIL_FLAG: {
		t = container_of(w, struct vdbinder_dbg_transaction, work);
		if (t->to_thread != thread) {
			printk(KERN_ERR "Incorrect dbg transaction\n");
			goto retry;
		}
		cmd = VD_BR_SET_VDBINDER_FAIL_FLAG;
		if (put_user(cmd, (uint32_t __user *)ptr)) {
			ret = -EFAULT;
			goto out;
		}
		ptr += sizeof(uint32_t);
		*size += sizeof(uint32_t);
		if (copy_to_user(ptr, t->data,
			sizeof(uint32_t))) {
			ret = -EFAULT;
			goto out;
		}
		*size += sizeof(uint32_t);
		kfree(t->data);
		list_del(&t->work.entry);
		kfree(t);
	} break;
	default:
		printk(KERN_ERR "Invalid work type\n");
		break;
	}
out:
	if (ret != 0) {
		list_del(&t->work.entry);
		kfree(t->data);
		kfree(t);
	}
	return ret;
}

int binder_set_rpcagent_thread(struct vdbinder_thread *thread)
{
	int ret = 0;
	if (!binder_rpc_agent_thread)
		binder_rpc_agent_thread = thread;
	else {
		printk(KERN_ERR "RPCAgent Thread already set\n");
		ret = -EBUSY;
	}
	return ret;
}

int binder_set_debug_thread(struct vdbinder_proc *proc,
	struct vdbinder_thread *thread)
{
	int ret = 0;
	if (!proc->dbg_thread)
		proc->dbg_thread = thread;
	else {
		printk(KERN_ERR "Debug Thread already set\n");
		ret = -EBUSY;
	}
	return ret;
}

int binder_dbg_operation(unsigned int size, void __user *ubuf,
	struct vdbinder_proc *proc,
	struct vdbinder_thread *thread)
{
	int ret = 0;
	struct vdbinder_dbg_write_read dwr;
	int read_size = 0;
	if (size != sizeof(struct vdbinder_dbg_write_read))
		return -EINVAL;
	if (copy_from_user(&dwr, ubuf, sizeof(dwr)))
		return -EFAULT;
	if (dwr.write_size > 0) {
		ret = binder_dbg_thread_write(proc, thread,
		(void __user *)dwr.write_buffer,
		dwr.write_size);
		if (ret < 0) {
			if (copy_to_user(ubuf, &dwr, sizeof(dwr)))
				return -EFAULT;
			return ret;
		}
	}
	ret = binder_dbg_thread_read(proc, thread,
		(void __user *)dwr.read_buffer,
		&read_size);
	if (ret < 0)
		return ret;
	dwr.read_size = read_size;
	if (copy_to_user(ubuf, &dwr, sizeof(dwr)))
		return -EFAULT;
	return ret;
}

int binder_get_dbg_version(unsigned int size, void __user *ubuf)
{
	int ret = 0;
	if (size != sizeof(struct vdbinder_debug_version))
		return -EINVAL;
	if (put_user(VDBINDER_CURRENT_DEBUG_VERSION,
			&((struct vdbinder_debug_version *)
					ubuf)->debug_version))
		return -EINVAL;
	return ret;
}

void binder_dbg_free_thread(struct vdbinder_thread *thread)
{
	if (thread == binder_rpc_agent_thread)
		binder_rpc_agent_thread = NULL;
#ifdef CONFIG_ORSAY_DEBUG
	if (thread->dbg_entry.type == TRANSACTION_START) {
		printk(KERN_INFO "binder %d : %d (%p:%p) binder_dbg_free_thread()\n",
			thread->proc->pid,
			thread->pid, thread,
			&thread->dbg_entry.entry);
		binder_remove_from_debug_db(thread);
	}
#endif
}

