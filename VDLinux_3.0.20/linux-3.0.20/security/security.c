/*
 * Security plug functions
 *
 * Copyright (C) 2001 WireX Communications, Inc <chris@wirex.com>
 * Copyright (C) 2001-2002 Greg Kroah-Hartman <greg@kroah.com>
 * Copyright (C) 2001 Networks Associates Technology, Inc <ssmalley@nai.com>
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 */

#include <linux/capability.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/security.h>
#include <linux/ima.h>

#ifdef CONFIG_SECURITY_DEBUG
struct hook_call_time lsm_call_time[MAX_SECURITY_HOOK_ID];
unsigned int check_performance = 0;
#endif

/* Boot-time LSM user choice */
static __initdata char chosen_lsm[SECURITY_NAME_MAX + 1] =
	CONFIG_DEFAULT_SECURITY;

/* things that live in capability.c */
extern void __init security_fixup_ops(struct security_operations *ops);

static struct security_operations *security_ops;
static struct security_operations default_security_ops = {
	.name	= "default",
};

static inline int __init verify(struct security_operations *ops)
{
	/* verify the security_operations structure exists */
	if (!ops)
		return -EINVAL;
	return 0;
}

static void __init do_security_initcalls(void)
{
	initcall_t *call;
	call = __security_initcall_start;
	while (call < __security_initcall_end) {
		(*call) ();
		call++;
	}
}

/**
 * security_init - initializes the security framework
 *
 * This should be called early in the kernel initialization sequence.
 */
int __init security_init(void)
{
	printk(KERN_INFO "Security Framework initialized\n");
	security_ops = &default_security_ops;
	do_security_initcalls();

	return 0;
}

void reset_security_ops(void)
{
	security_ops = &default_security_ops;
}

/* Save user chosen LSM */
static int __init choose_lsm(char *str)
{
	strncpy(chosen_lsm, str, SECURITY_NAME_MAX);
	return 1;
}
__setup("security=", choose_lsm);

/**
 * security_module_enable - Load given security module on boot ?
 * @ops: a pointer to the struct security_operations that is to be checked.
 *
 * Each LSM must pass this method before registering its own operations
 * to avoid security registration races. This method may also be used
 * to check if your LSM is currently loaded during kernel initialization.
 *
 * Return true if:
 *	-The passed LSM is the one chosen by user at boot time,
 *	-or the passed LSM is configured as the default and the user did not
 *	 choose an alternate LSM at boot time.
 * Otherwise, return false.
 */
int __init security_module_enable(struct security_operations *ops)
{
	return !strcmp(ops->name, chosen_lsm);
}

/**
 * register_security - registers a security framework with the kernel
 * @ops: a pointer to the struct security_options that is to be registered
 *
 * This function allows a security module to register itself with the
 * kernel security subsystem.  Some rudimentary checking is done on the @ops
 * value passed to this function. You'll need to check first if your LSM
 * is allowed to register its @ops by calling security_module_enable(@ops).
 *
 * If there is already a security module registered with the kernel,
 * an error will be returned.  Otherwise %0 is returned on success.
 */
int __init register_security(struct security_operations *ops)
{
	if (verify(ops)) {
		printk(KERN_DEBUG "%s could not verify "
		       "security_operations structure.\n", __func__);
		return -EINVAL;
	}

	if (security_ops != &default_security_ops)
		return -EAGAIN;

	security_ops = ops;

	return 0;
}

#ifdef CONFIG_SECURITY_DEBUG

DEFINE_SPINLOCK (security_debug);

/* security_hook_calltime - This function will be called by 
 *				all LSM hooks to calculate time 
 *				taken by LSM hook. This function
 *				gets minimum, maximum time  taken
 *				by executing LSM hook. It also give
 *				total number of time the function is 
 *				called as wells average time and total 
 *				 time for LSM hook.
 *
 * @ num : enum ID of each LSM hook
 * @ time: time taken for each LSM hook call
 *
 * Returns nothing.
 *
 */

void
security_hook_calltime (int num, struct timespec after, struct timespec before)
{
        struct timespec diff;
        unsigned long time;
	unsigned long flags;

	spin_lock_irqsave(&security_debug, flags);
        diff = timespec_sub(after, before);
        time = diff.tv_sec * NSEC_PER_SEC +  diff.tv_nsec;
	
	lsm_call_time[num].counter++;
	if(lsm_call_time[num].counter == 1)
	{
		lsm_call_time[num].total_time=0;
		lsm_call_time[num].avg_time=0;
		lsm_call_time[num].min_time = time;
		lsm_call_time[num].max_time = time;
	}
	if (time < lsm_call_time[num].min_time)
		lsm_call_time[num].min_time = time;
        if (time > lsm_call_time[num].max_time)
                lsm_call_time[num].max_time = time;
        lsm_call_time[num].total_time += time;
        lsm_call_time[num].avg_time = lsm_call_time[num].total_time / lsm_call_time[num].counter;
        spin_unlock_irqrestore(&security_debug, flags);
}

#endif /* CONFIG_SECURITY_DEBUG */


/* Security operations */

int security_bprm_set_creds(struct linux_binprm *bprm)
{
#ifdef CONFIG_SECURITY_DEBUG
	if (check_performance)
	{
		struct timespec before, after;
		int ret;

		getnstimeofday(&before);
		ret = security_ops->bprm_set_creds(bprm);
		getnstimeofday(&after);
		security_hook_calltime(LSM_BPRM_SET_CREDS, after, before);
		return ret;
	} else 
#endif
		return security_ops->bprm_set_creds(bprm);
}

int security_bprm_check(struct linux_binprm *bprm)
{
	int ret;

#ifdef CONFIG_SECURITY_DEBUG
	if (check_performance)
	{
		struct timespec before, after;

		getnstimeofday(&before);
		ret = security_ops->bprm_check_security(bprm);
		getnstimeofday(&after);
		security_hook_calltime(LSM_BPRM_CHECK_SECURITY, after, before);        
	} else
#endif
		ret = security_ops->bprm_check_security(bprm);
	if (ret)
		return ret;
	return ima_bprm_check(bprm);
}

int security_sb_mount(char *dev_name, struct path *path,
                       char *type, unsigned long flags, void *data)
{
#ifdef CONFIG_SECURITY_DEBUG
	if (check_performance)
	{
		struct timespec before, after;
		int ret;

		getnstimeofday(&before);
		ret = security_ops->sb_mount(dev_name, path, type, flags, data);
		getnstimeofday(&after);
		security_hook_calltime(LSM_SB_MOUNT, after, before);
		return ret;
	} else 
#endif
		return security_ops->sb_mount(dev_name, path, type, flags, data);
}

int security_sb_umount(struct vfsmount *mnt, int flags)
{
#ifdef CONFIG_SECURITY_DEBUG
	if (check_performance)
	{
		struct timespec before, after;
		int ret;

		getnstimeofday(&before);
		ret = security_ops->sb_umount(mnt, flags);
		getnstimeofday(&after);
		security_hook_calltime(LSM_SB_UMOUNT, after, before);
		return ret;
	} else
#endif
		return security_ops->sb_umount(mnt, flags);
}

int security_sb_pivotroot(struct path *old_path, struct path *new_path)
{
#ifdef CONFIG_SECURITY_DEBUG
	if (check_performance)
	{
		struct timespec before, after;
		int ret;

		getnstimeofday(&before);
		ret = security_ops->sb_pivotroot(old_path, new_path);
		getnstimeofday(&after);
		security_hook_calltime(LSM_SB_PIVOTROOT, after, before);  
		return ret;
	} else
#endif
		return security_ops->sb_pivotroot(old_path, new_path);
}

#ifdef CONFIG_SECURITY_PATH
int security_path_mknod(struct path *dir, struct dentry *dentry, int mode,
			unsigned int dev)
{
	if (unlikely(IS_PRIVATE(dir->dentry->d_inode)))
		return 0;
#ifdef CONFIG_SECURITY_DEBUG
	if (check_performance)
	{
		struct timespec before, after;
		int ret;

		getnstimeofday(&before);
		ret = security_ops->path_mknod(dir, dentry, mode, dev);
		getnstimeofday(&after);
		security_hook_calltime(LSM_PATH_MKNOD, after, before);
		return ret;
	} else
#endif
		return security_ops->path_mknod(dir, dentry, mode, dev);
}
EXPORT_SYMBOL(security_path_mknod);

int security_path_mkdir(struct path *dir, struct dentry *dentry, int mode)
{
	if (unlikely(IS_PRIVATE(dir->dentry->d_inode)))
		return 0;
#ifdef CONFIG_SECURITY_DEBUG
	if (check_performance)
	{
		struct timespec before, after;
		int ret;

		getnstimeofday(&before);
		ret = security_ops->path_mkdir(dir, dentry, mode);
		getnstimeofday(&after);
		security_hook_calltime(LSM_PATH_MKDIR, after, before);
		return ret;
	} else
#endif
		return security_ops->path_mkdir(dir, dentry, mode);
}
EXPORT_SYMBOL(security_path_mkdir);

int security_path_rmdir(struct path *dir, struct dentry *dentry)
{
	if (unlikely(IS_PRIVATE(dir->dentry->d_inode)))
		return 0;
#ifdef CONFIG_SECURITY_DEBUG
	if (check_performance)
	{
		struct timespec before, after;
		int ret;

		getnstimeofday(&before);
		ret = security_ops->path_rmdir(dir, dentry);
		getnstimeofday(&after);
		security_hook_calltime(LSM_PATH_RMDIR, after, before);
		return ret;
	} else
#endif
		return security_ops->path_rmdir(dir, dentry);
}

int security_path_unlink(struct path *dir, struct dentry *dentry)
{
	if (unlikely(IS_PRIVATE(dir->dentry->d_inode)))
		return 0;
#ifdef CONFIG_SECURITY_DEBUG
	if (check_performance)
	{
		struct timespec before, after;
		int ret;

		getnstimeofday(&before);
		ret = security_ops->path_unlink(dir, dentry);
		getnstimeofday(&after);
		security_hook_calltime(LSM_PATH_UNLINK, after, before);
		return ret;
	} else
#endif
		return security_ops->path_unlink(dir, dentry);
}
EXPORT_SYMBOL(security_path_unlink);

int security_path_symlink(struct path *dir, struct dentry *dentry,
			  const char *old_name)
{
	if (unlikely(IS_PRIVATE(dir->dentry->d_inode)))
		return 0;
#ifdef CONFIG_SECURITY_DEBUG
	if (check_performance)
	{
		struct timespec before, after;
		int ret;

		getnstimeofday(&before);
		ret = security_ops->path_symlink(dir, dentry, old_name);
		getnstimeofday(&after);
		security_hook_calltime(LSM_PATH_SYMLINK, after, before);
		return ret;
	} else
#endif
		return security_ops->path_symlink(dir, dentry, old_name);
}

int security_path_link(struct dentry *old_dentry, struct path *new_dir,
		       struct dentry *new_dentry)
{
	if (unlikely(IS_PRIVATE(old_dentry->d_inode)))
		return 0;
#ifdef CONFIG_SECURITY_DEBUG
	if (check_performance)
	{
		struct timespec before, after;
		int ret;

		getnstimeofday(&before);
		ret = security_ops->path_link(old_dentry, new_dir, new_dentry);
		getnstimeofday(&after);
		security_hook_calltime(LSM_PATH_LINK, after, before);
		return ret;
	} else
#endif
		return security_ops->path_link(old_dentry, new_dir, new_dentry);
}

int security_path_rename(struct path *old_dir, struct dentry *old_dentry,
			 struct path *new_dir, struct dentry *new_dentry)
{
	if (unlikely(IS_PRIVATE(old_dentry->d_inode) ||
		     (new_dentry->d_inode && IS_PRIVATE(new_dentry->d_inode))))
		return 0;
#ifdef CONFIG_SECURITY_DEBUG
	if (check_performance)
	{
		struct timespec before, after;
		int ret;

		getnstimeofday(&before);
		ret = security_ops->path_rename(old_dir, old_dentry, new_dir,
					 new_dentry);
		getnstimeofday(&after);
		security_hook_calltime(LSM_PATH_RENAME, after, before);
		return ret;
	} else
#endif
		return security_ops->path_rename(old_dir, old_dentry, new_dir,
		                         new_dentry);
}
EXPORT_SYMBOL(security_path_rename);

int security_path_truncate(struct path *path)
{
	if (unlikely(IS_PRIVATE(path->dentry->d_inode)))
		return 0;
#ifdef CONFIG_SECURITY_DEBUG
	if (check_performance)
	{
		struct timespec before, after;
		int ret;

		getnstimeofday(&before);
		ret = security_ops->path_truncate(path);
		getnstimeofday(&after);
		security_hook_calltime(LSM_PATH_TRUNCATE, after, before); 
		return ret;
	} else
#endif
		return security_ops->path_truncate(path);
}

int security_path_chmod(struct dentry *dentry, struct vfsmount *mnt,
			mode_t mode)
{
	if (unlikely(IS_PRIVATE(dentry->d_inode)))
		return 0;
#ifdef CONFIG_SECURITY_DEBUG
	if (check_performance)
	{
		struct timespec before, after;
		int ret;

		getnstimeofday(&before);
		ret = security_ops->path_chmod(dentry, mnt, mode);
		getnstimeofday(&after);
		security_hook_calltime(LSM_PATH_CHMOD, after, before);
		return ret;
	} else
#endif
		return security_ops->path_chmod(dentry, mnt, mode);
}

int security_path_chown(struct path *path, uid_t uid, gid_t gid)
{
	if (unlikely(IS_PRIVATE(path->dentry->d_inode)))
		return 0;
#ifdef CONFIG_SECURITY_DEBUG
	if (check_performance)
	{
		struct timespec before, after;
		int ret;

		getnstimeofday(&before);
		ret = security_ops->path_chown(path, uid, gid);
		getnstimeofday(&after);
		security_hook_calltime(LSM_PATH_CHOWN, after, before);
		return ret;
	} else
#endif
		return security_ops->path_chown(path, uid, gid);
}

int security_path_chroot(struct path *path)
{
#ifdef CONFIG_SECURITY_DEBUG

	if (check_performance)
	{
		struct timespec before, after;
		int ret;

		getnstimeofday(&before);
		ret = security_ops->path_chroot(path);
		getnstimeofday(&after);
		security_hook_calltime(LSM_PATH_CHROOT, after, before);
		return ret;
	} else
#endif
		return security_ops->path_chroot(path);
}
#endif

int security_inode_getattr(struct vfsmount *mnt, struct dentry *dentry)
{
	if (unlikely(IS_PRIVATE(dentry->d_inode)))
		return 0;
#ifdef CONFIG_SECURITY_DEBUG
	if (check_performance)
	{
		struct timespec before, after;
		int ret;

		getnstimeofday(&before);
		ret = security_ops->inode_getattr(mnt, dentry);
		getnstimeofday(&after);
		security_hook_calltime(LSM_INODE_GETATTR, after, before);
		return ret;
	} else
#endif
		return security_ops->inode_getattr(mnt, dentry);
}

int security_file_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
#ifdef CONFIG_SECURITY_DEBUG
	if (check_performance)
	{
		struct timespec before, after;
		int ret;

		getnstimeofday(&before);
		ret = security_ops->file_ioctl(file, cmd, arg);
		getnstimeofday(&after);
		security_hook_calltime(LSM_FILE_IOCTL, after, before);  
		return ret;
	} else
#endif
		return security_ops->file_ioctl(file, cmd, arg);
}


int security_file_fcntl(struct file *file, unsigned int cmd, unsigned long arg)
{
#ifdef CONFIG_SECURITY_DEBUG
	if (check_performance)
	{
		struct timespec before, after;
		int ret;

		getnstimeofday(&before);
		ret = security_ops->file_fcntl(file, cmd, arg);
		getnstimeofday(&after);
		security_hook_calltime(LSM_FILE_FCNTL, after, before);
		return ret;
	} else
#endif
		return security_ops->file_fcntl(file, cmd, arg);
}

int security_dentry_open(struct file *file, const struct cred *cred)
{
	int ret;
#ifdef CONFIG_SECURITY_DEBUG
	if (check_performance)
	{
		struct timespec before, after;

		getnstimeofday(&before);
		ret = security_ops->dentry_open(file, cred);
		getnstimeofday(&after);
		security_hook_calltime(LSM_DENTRY_OPEN, after, before);        
	} else
#endif
		ret = security_ops->dentry_open(file, cred);
	if (ret)
		return ret;

	return fsnotify_perm(file, MAY_OPEN);
}

int security_cred_alloc_blank(struct cred *cred, gfp_t gfp)
{
#ifdef CONFIG_SECURITY_DEBUG
	if (check_performance)
	{
		struct timespec before, after;
		int ret;

		getnstimeofday(&before);
		ret = security_ops->cred_alloc_blank(cred, gfp);
		getnstimeofday(&after);
		security_hook_calltime(LSM_CRED_ALLOC_BLANK, after, before);
		return ret;
	} else
#endif
		return security_ops->cred_alloc_blank(cred, gfp);
}

void security_cred_free(struct cred *cred)
{
#ifdef CONFIG_SECURITY_DEBUG
	if (check_performance)
	{
		struct timespec before, after;

		getnstimeofday(&before);
		security_ops->cred_free(cred);
		getnstimeofday(&after);
		security_hook_calltime(LSM_CRED_FREE, after, before);        
	} else
#endif
		security_ops->cred_free(cred);
}

int security_prepare_creds(struct cred *new, const struct cred *old, gfp_t gfp)
{
#ifdef CONFIG_SECURITY_DEBUG
	if (check_performance)
	{
		struct timespec before, after;
		int ret;

		getnstimeofday(&before);
		ret = security_ops->cred_prepare(new, old, gfp);
		getnstimeofday(&after);
		security_hook_calltime(LSM_PREPARE_CREDS, after, before);     
		return ret;
	} else
#endif
		return security_ops->cred_prepare(new, old, gfp);
}

void security_transfer_creds(struct cred *new, const struct cred *old)
{
#ifdef CONFIG_SECURITY_DEBUG
	if (check_performance)
	{
		struct timespec before, after;

		getnstimeofday(&before);
		security_ops->cred_transfer(new, old);
		getnstimeofday(&after);
		security_hook_calltime(LSM_CRED_TRANSFER, after, before);        
	} else
#endif
		security_ops->cred_transfer(new, old);
}

#ifdef CONFIG_SECURITY_TOMOYO_RESTRICT_PRIVILEGE

void security_restrict_creds(const struct cred *parent, struct cred *child)
{
        security_ops->cred_restrict_privilege(parent, child);
}

#endif

#ifdef CONFIG_SECURITY_NETWORK
int security_socket_bind(struct socket *sock, struct sockaddr *address, int addrlen)
{
	return security_ops->socket_bind(sock, address, addrlen);
}

int security_socket_connect(struct socket *sock, struct sockaddr *address, int addrlen)
{
	return security_ops->socket_connect(sock, address, addrlen);
}

int security_socket_listen(struct socket *sock, int backlog)
{
	return security_ops->socket_listen(sock, backlog);
}

int security_socket_sendmsg(struct socket *sock, struct msghdr *msg, int size)
{
	return security_ops->socket_sendmsg(sock, msg, size);
}
#endif	/* CONFIG_SECURITY_NETWORK */

