/*
 *  linux/fs/proc/fd_pid.c
 *
 *  Show thread-based file descriptor information (/proc/fd_pid)
 *  It's simillar with /proc/$pid/fd/. Thread id and name are added.
 *  by Choi Young-Ho (yh2005.choi@samsung.com)
 *
 *  Copyright (C) 2011  Samsung Electronics
 *
 *  2011-05-11  Created with initial draft.
 *
 */


#include <linux/types.h>
#include <linux/errno.h>
#include <linux/time.h>
#include <linux/kernel.h>
#include <linux/poll.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/fdtable.h>
#include <linux/namei.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include <linux/io.h>

/* #define FD_PID_DEBUG	1 */

#define MAX_PATHNAME	64

/* Function: fd_pid_get_pathname
 *
 * Found the full path of struct dentry.
 * struct dentry has d_parent entry. if it has parent path
 * (e.g. /dev/mem), it has the d_parent like following:
 *
 *         (mem)              (dev)         (/)
 *     sturct dentry    struct dentry    struct dentry
 *      +-----------+    +-----------+    +-----------+
 *      |  d_parent | -> |  d_parent | -> |  d_parent |
 *      +-----------+    +-----------+    +-----------+
 *      |d_name.name|    |d_name.name|    |d_name.name|
 *      +-----------+    +-----------+    +-----------+
 *      |     .     |    |     .     |    |     .     |
 *      |     .     |
 *      |     .     |
 *      +-----------+
 *
 * This function try to find pathname from end of entry.
 *
 * (1) find the root path until FD_PID_MAX_DEPTH and possible buffer size
 * (2) copy the each pathname to buffer
 *
 * See Also: struct dentry
 */

#define FD_PID_MAX_DEPTH 	5

static int fd_pid_get_pathname(struct dentry *dentry, char *pathname,
	int len, int depth)
{
	int expected_len = 0;

	/*
	 * At first, find the root path until FD_PID_MAX_DEPTH
	 * and possible buffer size
	 */

	if (dentry == NULL || depth >= FD_PID_MAX_DEPTH)
		return -1;

	if (dentry->d_parent != NULL && dentry->d_parent != dentry) {
		expected_len = len - (dentry->d_name.len + 1);

#ifdef FD_PID_DEBUG
		printk(KERN_CRIT"Before: len = %d, expected_len = %d, strlen = %d, depth = %d\n",
				len, expected_len, dentry->d_name.len, depth);
#endif

		if (expected_len <= 0)
			return -1;

		fd_pid_get_pathname(dentry->d_parent, pathname,
			expected_len, ++depth);
	}

	/*
	 * After found path, copy the each path name to buffer
	 */

	strncat(pathname, dentry->d_name.name, len);

	/*
	 * Insert '/' character to show human readable path name
	 */

	if (dentry->d_name.name[0] != '/' && depth != 1)
		strncat(pathname, "/", 1);

#ifdef FD_PID_DEBUG
	printk(KERN_CRIT"After: len = %d, expected_len = %d, strlen = %d, depth = %d\n",
			len, expected_len, dentry->d_name.len, depth);
#endif

	return len;
}

/* Function: fd_pid_write
 *
 * Proc(/proc/fd_pid) write function.
 * This function would be called when user write pid to proc.
 * (e.g. echo 64 > /proc/fd_pid)
 *
 * This function prints the thread-based file descriptor like following:
 *
 * 0    -> /dev/ttyS3       (38  :sh              )
 * 1    -> /dev/ttyS3       (38  :sh              )
 * 2    -> /dev/ttyS3       (38  :sh              )
 * 3    -> /dev/sdp_i2c0    (64  :exeDSP          )
 * 4    -> /dev/sdp_i2c1    (64  :exeDSP          )
 *
 * The task_struct has struct files_struct *file field, which include
 * opened file descriptor information by this process.
 *
 * This function found this file and shows file and thread information.
 *
 * See Also: fd_pid_get_pathname
 */

#define MAX_FD_PID_WRITE_BUF	8

static ssize_t fd_pid_write(struct file *file, const char __user *buf,
				size_t count, loff_t *ppos)
{
	char buffer[MAX_FD_PID_WRITE_BUF];
	long pid;

	struct task_struct *task;
	struct files_struct *files;
	struct fdtable *fdt;
	struct file *fds;
	int fd;
	char pathname[MAX_PATHNAME];

	/*
	 * Prologue of proc write function
	 */

	if (!count)
		return count;

	memset(buffer, 0, sizeof(buffer));

	if (count >= sizeof(buffer))
		count = sizeof(buffer) - 1;

	if (copy_from_user(buffer, buf, count))
		return -EFAULT;

	if (strict_strtol(buffer, 0, &pid) != 0)
		return -EINVAL;

	/*
	 * Found the task struct to read files by pid
	 */

	rcu_read_lock();
	task = find_task_by_pid_ns(pid, &init_pid_ns);
	rcu_read_unlock();

	if (task == NULL)
		return -EINVAL;

	/*
	 * Found struct files to read each opend file
	 */

	files = get_files_struct(task);
	if (!files)
		return -EINVAL;

	spin_lock(&files->file_lock);

	fdt = files_fdtable(files);

	/*
	 * Print the opend file and thread information
	 */

	for (fd = 0; fd < fdt->max_fds; fd++) {

		fds = fcheck_files(files, fd);

		if (fds == NULL)
			continue;

		rcu_read_lock();
		task = find_task_by_pid_ns(fds->pid, &init_pid_ns);
		rcu_read_unlock();

		memset(pathname, 0, MAX_PATHNAME);
		fd_pid_get_pathname(fds->f_path.dentry, pathname,
			MAX_PATHNAME-1 , 0);

		printk(KERN_CRIT"%-4d -> %-16s (%-4d:%-16s)\n",
			fd, pathname,
			fds->pid,
			(task != NULL) ? task->comm : "not found thread");
	}

	spin_unlock(&files->file_lock);
	put_files_struct(files);

	/*
	 * Epilogue of proc write function
	 */

	return count;
}

const struct file_operations proc_fd_pid_operations = {
	.write      = fd_pid_write,
};

/*
 * Create Proc Entry
 */
static int __init init_proc_fd_pid(void)
{
	struct proc_dir_entry *entry;

	entry = proc_create("fd_pid", S_IRUSR | S_IWUSR, NULL,
		&proc_fd_pid_operations);

	return 0;
}

module_init(init_proc_fd_pid);
