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

#ifndef _LINUX_VDBINDER_H
#define _LINUX_VDBINDER_H

#include <linux/ioctl.h>

#define VD_B_PACK_CHARS(c1, c2, c3, c4) \
	((((c1)<<24)) | (((c2)<<16)) | (((c3)<<8)) | (c4))
#define VD_B_TYPE_LARGE 0x85

enum {
	VDBINDER_TYPE_BINDER	= VD_B_PACK_CHARS('s', 'b', '*', VD_B_TYPE_LARGE),
	VDBINDER_TYPE_WEAK_BINDER	= VD_B_PACK_CHARS('w', 'b', '*', VD_B_TYPE_LARGE),
	VDBINDER_TYPE_HANDLE	= VD_B_PACK_CHARS('s', 'h', '*', VD_B_TYPE_LARGE),
	VDBINDER_TYPE_WEAK_HANDLE	= VD_B_PACK_CHARS('w', 'h', '*', VD_B_TYPE_LARGE),
	VDBINDER_TYPE_FD		= VD_B_PACK_CHARS('f', 'd', '*', VD_B_TYPE_LARGE),
	VDBINDER_TYPE_WAIT_BINDER	= VD_B_PACK_CHARS('w', 't',
							'*', VD_B_TYPE_LARGE),
};

enum {
	VD_FLAT_BINDER_FLAG_PRIORITY_MASK = 0xff,
	VD_FLAT_BINDER_FLAG_ACCEPTS_FDS = 0x100,
};

/*
 * This is the flattened representation of a Binder object for transfer
 * between processes.  The 'offsets' supplied as part of a binder transaction
 * contains offsets into the data where these structures occur.  The Binder
 * driver takes care of re-writing the structure type and data as it moves
 * between processes.
 */
struct flat_vdbinder_object {
	/* 8 bytes for large_flat_header. */
	unsigned long		type;
	unsigned long		flags;

	/* 8 bytes of data. */
	union {
		void		*binder;	/* local object */
		signed long	handle;		/* remote object */
	};

	/* extra data associated with local object */
	void			*cookie;
};

/*
 * On 64-bit platforms where user code may run in 32-bits the driver must
 * translate the buffer (and local binder) addresses apropriately.
 */

struct vdbinder_write_read {
	signed long	write_size;	/* bytes to write */
	signed long	write_consumed;	/* bytes consumed by driver */
	unsigned long	write_buffer;
	signed long	read_size;	/* bytes to read */
	signed long	read_consumed;	/* bytes consumed by driver */
	unsigned long	read_buffer;
};

/* Use with BINDER_VERSION, driver fills in fields. */
struct vdbinder_version {
	/* driver protocol version -- increment with incompatible change */
	signed long	protocol_version;
};

struct vdbinder_wake_info {
	signed long	handle;
	signed long	id;
};

/* This is the current protocol version. */
#define VDBINDER_CURRENT_PROTOCOL_VERSION 11

#define VDBINDER_WRITE_READ	_IOWR('b', 1, struct vdbinder_write_read)
#define	VDBINDER_SET_IDLE_TIMEOUT		_IOW('b', 3, int64_t)
#define	VDBINDER_SET_MAX_THREADS		_IOW('b', 5, size_t)
#define	VDBINDER_SET_IDLE_PRIORITY	_IOW('b', 6, int)
#define	VDBINDER_SET_CONTEXT_MGR		_IOW('b', 7, int)
#define	VDBINDER_THREAD_EXIT		_IOW('b', 8, int)
#define VDBINDER_VERSION			_IOWR('b', 9, struct vdbinder_version)
#define VDBINDER_SET_BOARD_INFO		_IOW('b', 10, int)
#define VDBINDER_GET_BOARD_INFO		_IOR('b', 11, int)
#define	VDBINDER_CONTEXT_MGR_READY		_IOW('b', 12, int)
#define	VDBINDER_CONTEXT_MGR_CLEAR_WAIT_LIST	_IOW('b', 13,\
						struct vdbinder_wake_info)
#define VDBINDER_SET_TIME  _IOW('b', 14, unsigned long)
#define VDBINDER_GET_TIME  _IOR('b', 15, unsigned long)
#define VDBINDER_SET_TIME_NOWAIT  _IOW('b', 16, unsigned long)
#define VDBINDER_GET_TIME_WAIT  _IOR('b', 17, unsigned long)
#define VDBINDER_DEBUG_VERSION	  _IOWR('b', 23, struct vdbinder_debug_version)

/*
 * NOTE: Two special error codes you should check for when calling
 * in to the driver are:
 *
 * EINTR -- The operation has been interupted.  This should be
 * handled by retrying the ioctl() until a different error code
 * is returned.
 *
 * ECONNREFUSED -- The driver is no longer accepting operations
 * from your process.  That is, the process is being destroyed.
 * You should handle this by exiting from your process.  Note
 * that once this error code is returned, all further calls to
 * the driver from any thread will return this same code.
 */

enum vd_transaction_flags {
	VD_TF_ONE_WAY	= 0x01,	/* this is a one-way call: async, no return */
	VD_TF_ROOT_OBJECT	= 0x04,	/* contents are the component's root object */
	VD_TF_STATUS_CODE	= 0x08,	/* contents are a 32-bit status code */
	VD_TF_ACCEPT_FDS	= 0x10,	/* allow replies with file descriptors */
};

struct vdbinder_transaction_data {
	/* The first two are only used for bcTRANSACTION and brTRANSACTION,
	 * identifying the target and contents of the transaction.
	 */
	union {
		size_t	handle;	/* target descriptor of command transaction */
		void	*ptr;	/* target descriptor of return transaction */
	} target;
	void		*cookie;	/* target object cookie */
	unsigned int	code;		/* transaction command */

	/* General information about the transaction. */
	unsigned int	flags;
	pid_t		sender_pid;
	uid_t		sender_euid;
	size_t		data_size;	/* number of bytes of data */
	size_t		offsets_size;	/* number of bytes of offsets */

	unsigned int	isEnc;	/* encryption flag */
	unsigned int	isShrd;	/* shared memory flag */

	/* If this transaction is inline, the data immediately
	 * follows here; otherwise, it ends with a pointer to
	 * the data buffer.
	 */
	union {
		struct {
			/* transaction data */
			const void	*buffer;
			/* offsets from buffer to flat_vdbinder_object structs */
			const void	*offsets;
		} ptr;
		uint8_t	buf[8];
	} data;
};

struct vdbinder_ptr_cookie {
	void *ptr;
	void *cookie;
};

struct vdbinder_pri_desc {
	int priority;
	int desc;
};

struct vdbinder_pri_ptr_cookie {
	int priority;
	void *ptr;
	void *cookie;
};

enum VDBinderDriverReturnProtocol {
	VD_BR_ERROR = _IOR('r', 0, int),
	/*
	 * int: error code
	 */

	VD_BR_OK = _IO('r', 1),
	/* No parameters! */

	VD_BR_TRANSACTION = _IOR('r', 2, struct vdbinder_transaction_data),
	VD_BR_REPLY = _IOR('r', 3, struct vdbinder_transaction_data),
	/*
	 * vdbinder_transaction_data: the received command.
	 */

	VD_BR_ACQUIRE_RESULT = _IOR('r', 4, int),
	/*
	 * not currently supported
	 * int: 0 if the last bcATTEMPT_ACQUIRE was not successful.
	 * Else the remote object has acquired a primary reference.
	 */

	VD_BR_DEAD_REPLY = _IO('r', 5),
	/*
	 * The target of the last transaction (either a bcTRANSACTION or
	 * a bcATTEMPT_ACQUIRE) is no longer with us.  No parameters.
	 */

	VD_BR_TRANSACTION_COMPLETE = _IO('r', 6),
	/*
	 * No parameters... always refers to the last transaction requested
	 * (including replies).  Note that this will be sent even for
	 * asynchronous transactions.
	 */

	VD_BR_INCREFS = _IOR('r', 7, struct vdbinder_ptr_cookie),
	VD_BR_ACQUIRE = _IOR('r', 8, struct vdbinder_ptr_cookie),
	VD_BR_RELEASE = _IOR('r', 9, struct vdbinder_ptr_cookie),
	VD_BR_DECREFS = _IOR('r', 10, struct vdbinder_ptr_cookie),
	/*
	 * void *:	ptr to binder
	 * void *: cookie for binder
	 */

	VD_BR_ATTEMPT_ACQUIRE = _IOR('r', 11, struct vdbinder_pri_ptr_cookie),
	/*
	 * not currently supported
	 * int:	priority
	 * void *: ptr to binder
	 * void *: cookie for binder
	 */

	VD_BR_NOOP = _IO('r', 12),
	/*
	 * No parameters.  Do nothing and examine the next command.  It exists
	 * primarily so that we can replace it with a VD_BR_SPAWN_LOOPER command.
	 */

	VD_BR_SPAWN_LOOPER = _IO('r', 13),
	/*
	 * No parameters.  The driver has determined that a process has no
	 * threads waiting to service incomming transactions.  When a process
	 * receives this command, it must spawn a new service thread and
	 * register it via bcENTER_LOOPER.
	 */

	VD_BR_FINISHED = _IO('r', 14),
	/*
	 * not currently supported
	 * stop threadpool thread
	 */

	VD_BR_DEAD_BINDER = _IOR('r', 15, void *),
	/*
	 * void *: cookie
	 */
	VD_BR_CLEAR_DEATH_NOTIFICATION_DONE = _IOR('r', 16, void *),
	/*
	 * void *: cookie
	 */

	VD_BR_FAILED_REPLY = _IO('r', 17),
	/*
	 * The the last transaction (either a bcTRANSACTION or
	 * a bcATTEMPT_ACQUIRE) failed (e.g. out of memory).  No parameters.
	 */

	VD_BR_FAILED_REPLY_NO_MEM = _IO('r', 18),
	/*
	 * The the last transaction (bcTRANSACTION)
	 * failed due to out of memory. No parameters.
	 */
};

enum VDBinderDriverCommandProtocol {
	VD_BC_TRANSACTION = _IOW('c', 0, struct vdbinder_transaction_data),
	VD_BC_REPLY = _IOW('c', 1, struct vdbinder_transaction_data),
	/*
	 * vdbinder_transaction_data: the sent command.
	 */

	VD_BC_ACQUIRE_RESULT = _IOW('c', 2, int),
	/*
	 * not currently supported
	 * int:  0 if the last VD_BR_ATTEMPT_ACQUIRE was not successful.
	 * Else you have acquired a primary reference on the object.
	 */

	VD_BC_FREE_BUFFER = _IOW('c', 3, int),
	/*
	 * void *: ptr to transaction data received on a read
	 */

	VD_BC_INCREFS = _IOW('c', 4, int),
	VD_BC_ACQUIRE = _IOW('c', 5, int),
	VD_BC_RELEASE = _IOW('c', 6, int),
	VD_BC_DECREFS = _IOW('c', 7, int),
	/*
	 * int:	descriptor
	 */

	VD_BC_INCREFS_DONE = _IOW('c', 8, struct vdbinder_ptr_cookie),
	VD_BC_ACQUIRE_DONE = _IOW('c', 9, struct vdbinder_ptr_cookie),
	/*
	 * void *: ptr to binder
	 * void *: cookie for binder
	 */

	VD_BC_ATTEMPT_ACQUIRE = _IOW('c', 10, struct vdbinder_pri_desc),
	/*
	 * not currently supported
	 * int: priority
	 * int: descriptor
	 */

	VD_BC_REGISTER_LOOPER = _IO('c', 11),
	/*
	 * No parameters.
	 * Register a spawned looper thread with the device.
	 */

	VD_BC_ENTER_LOOPER = _IO('c', 12),
	VD_BC_EXIT_LOOPER = _IO('c', 13),
	/*
	 * No parameters.
	 * These two commands are sent as an application-level thread
	 * enters and exits the binder loop, respectively.  They are
	 * used so the binder can have an accurate count of the number
	 * of looping threads it has available.
	 */

	VD_BC_REQUEST_DEATH_NOTIFICATION = _IOW('c', 14, struct vdbinder_ptr_cookie),
	/*
	 * void *: ptr to binder
	 * void *: cookie
	 */

	VD_BC_CLEAR_DEATH_NOTIFICATION = _IOW('c', 15, struct vdbinder_ptr_cookie),
	/*
	 * void *: ptr to binder
	 * void *: cookie
	 */

	VD_BC_DEAD_BINDER_DONE = _IOW('c', 16, void *),
	/*
	 * void *: cookie
	 */
	VD_BC_BINDER_DEAD = _IOW('c', 17, void *),
	VD_BC_TRANSACTION_WAIT = _IOW('c', 18,
				struct vdbinder_transaction_data),
};

#endif /* _LINUX_VDBINDER_H */

