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

#ifndef _LINUX_VDBINDER_DBG_H
#define _LINUX_VDBINDER_DBG_H

#define MAX_VDBINDER_HANG_STAT 50
#define MAX_VDBINDER_LOG_COUNT 100
#define MAX_FUNC_LEN  (TASK_COMM_LEN + 32)

struct vdbinder_dbg_write_read {
	signed long	write_size;     /* bytes to write */
	unsigned long	write_buffer;
	signed long	read_size;      /* bytes to read */
	unsigned long	read_buffer;
};

/*Structure to hold the debug info.*/
struct vdbinder_log_info {
	uint32_t module;
	uint32_t level;
	uint32_t verbosity;
};

struct vdbinder_hang_stat {
	char	sender_thread[MAX_FUNC_LEN];
	char	sender_proc[MAX_FUNC_LEN];
	char	target[MAX_FUNC_LEN];
	long	wait_ticks;
	pid_t	sender_pid;
	pid_t	target_pid;
	int	board_num;
	void	*obj;
	unsigned int	func_code;
};

struct vdbinder_obj_info {
	pid_t	target_pid;
	int	board_num;
	void	*obj;
};

struct vdbinder_transaction_fail {
	char proc_name[MAX_FUNC_LEN];
	char thread_name[MAX_FUNC_LEN];
	int32_t thread_id;
	char file_name[MAX_FUNC_LEN];
	int32_t line_num;
	char func_name[MAX_FUNC_LEN];
	int32_t error_code;
	int board_num;
};

struct vdbinder_debug_version {
	/* Vdbinder debugger version -- increment with incompatible change */
	uint32_t	debug_version;
};

struct dbg_transact {
	struct list_head entry;
	enum {
		TRANSACTION_START  = 0x04f4f4f4,
		TRANSACTION_FINISH = 0x0f4f4f4f,
	} type;
};

/* This is the current debugger version. */
#define VDBINDER_CURRENT_DEBUG_VERSION 2

#define VDBINDER_DEBUG_OPERATION	_IOWR('b', 18, \
		struct vdbinder_dbg_write_read)
#define VDBINDER_SET_RPC_AGENT_THREAD	_IOW('b', 19, int)
#define VDBINDER_SET_DEBUG_THREAD	_IOW('b', 20, int)

enum VDBinderDbgReturnProtocol {
	VD_BR_SET_LOG_LEVEL = _IOR('r', 20, struct vdbinder_log_info),
	VD_BR_GET_HANG_THREAD = _IOR('r', 21, struct vdbinder_hang_stat),
	VD_BR_DBG_WORK_DONE = _IO('r', 22),
	VD_BR_THREAD_FINISH = _IO('r', 23),
	VD_BR_GET_OBJ_NAME = _IOR('r', 24, struct vdbinder_obj_info),
	VD_BR_GET_REMOTE_TRANSACTION_FAIL_LOG =
					_IOR('r', 25, struct vdbinder_obj_info),
	VD_BR_SET_VDBINDER_FAIL_FLAG = _IOR('r', 26, uint32_t),
	/*VD_BR_GET_HANG_TASK = _IOR('r', 21, struct vdbinder_hang_stat),
	VD_BR_GET_TRANSACTION_FAIL = _IOR('r', 22, struct vdbinder_hang_stat),
	VD_BR_GET_TASK_FAIL = _IOR('r', 23, struct vdbinder_hang_stat),
	VD_BR_DBG_ERROR = _IO('r', 26),
	VD_BR_DBG_HANG_THREAD_DONE = _IOR('r', 28,
	struct vdbinder_hang_stat),*/
};

enum VDBinderDbgCommandProtocol {
	VD_BC_SET_LOG_LEVEL = _IOW('c', 20, struct vdbinder_log_info),
	VD_BC_GET_HANG_THREAD = _IO('c', 21),
	VD_BC_PUT_HANG_THREAD = _IOW('c', 22, struct vdbinder_hang_stat),
	VD_BC_DEBUG_THREAD_FINISH = _IO('c', 23),
	VD_BC_GET_OBJ_NAME = _IOW('c', 24, struct vdbinder_obj_info),
	VD_BC_PUT_OBJ_NAME = _IOW('c', 25, void *),
	VD_BC_SET_TRANSACT_FAIL_LOG =
				_IOW('c', 26, struct vdbinder_transaction_fail),
	VD_BC_GET_TRANSACT_FAIL_LOG =
				_IOR('c', 27, struct vdbinder_transaction_fail),
	VD_BC_PUT_REMOTE_TRANSACTION_FAIL_LOG =
				_IOR('c', 28, struct vdbinder_transaction_fail),
	VD_BC_SET_VDBINDER_FAIL_FLAG = _IOW('c', 29, uint32_t),
	/*VD_BC_GET_HANG_TASK = _IO('c', 21),
	VD_BC_GET_TRANSACTION_FAIL = _IO('c', 22),
	VD_BC_GET_TASK_FAIL = _IO('c', 23),
	VD_BC_GET_OBJ_NAME_REPLY = _IOW('c', 26, struct vdbinder_obj_info),*/
};
#endif
