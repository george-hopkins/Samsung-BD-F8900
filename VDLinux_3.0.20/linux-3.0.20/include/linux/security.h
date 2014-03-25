/*
 * Linux Security plug
 *
 * Copyright (C) 2001 WireX Communications, Inc <chris@wirex.com>
 * Copyright (C) 2001 Greg Kroah-Hartman <greg@kroah.com>
 * Copyright (C) 2001 Networks Associates Technology, Inc <ssmalley@nai.com>
 * Copyright (C) 2001 James Morris <jmorris@intercode.com.au>
 * Copyright (C) 2001 Silicon Graphics, Inc. (Trust Technology Group)
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	Due to this file being licensed under the GPL there is controversy over
 *	whether this permits you to write a module that #includes this file
 *	without placing your module under the GPL.  Please consult a lawyer for
 *	advice before doing this.
 *
 */

#ifndef __LINUX_SECURITY_H
#define __LINUX_SECURITY_H

#include <linux/fs.h>
#include <linux/fsnotify.h>
#include <linux/binfmts.h>
#include <linux/dcache.h>
#include <linux/signal.h>
#include <linux/resource.h>
#include <linux/sem.h>
#include <linux/shm.h>
#include <linux/mm.h> /* PAGE_ALIGN */
#include <linux/msg.h>
#include <linux/sched.h>
#include <linux/key.h>
#include <linux/xfrm.h>
#include <linux/slab.h>
#include <net/flow.h>

#ifdef CONFIG_SECURITY_DEBUG
#include <linux/security_debugfs.h>
#endif

/* Maximum number of letters for an LSM name string */
#define SECURITY_NAME_MAX	10

/* If capable should audit the security request */
#define SECURITY_CAP_NOAUDIT 0
#define SECURITY_CAP_AUDIT 1

struct ctl_table;
struct audit_krule;
struct user_namespace;

#ifdef CONFIG_SECURITY_TOMOYO_DISABLE
extern unsigned int tomoyo_enable;
#endif

/*
 * These functions are in security/capability.c and are used
 * as the default capabilities functions
 */
extern int cap_capable(struct task_struct *tsk, const struct cred *cred,
		       struct user_namespace *ns, int cap, int audit);
extern int cap_settime(const struct timespec *ts, const struct timezone *tz);
extern int cap_ptrace_access_check(struct task_struct *child, unsigned int mode);
extern int cap_ptrace_traceme(struct task_struct *parent);
extern int cap_capget(struct task_struct *target, kernel_cap_t *effective, kernel_cap_t *inheritable, kernel_cap_t *permitted);
extern int cap_capset(struct cred *new, const struct cred *old,
		      const kernel_cap_t *effective,
		      const kernel_cap_t *inheritable,
		      const kernel_cap_t *permitted);
extern int cap_bprm_set_creds(struct linux_binprm *bprm);
extern int cap_bprm_secureexec(struct linux_binprm *bprm);
extern int cap_inode_setxattr(struct dentry *dentry, const char *name,
			      const void *value, size_t size, int flags);
extern int cap_inode_removexattr(struct dentry *dentry, const char *name);
extern int cap_inode_need_killpriv(struct dentry *dentry);
extern int cap_inode_killpriv(struct dentry *dentry);
extern int cap_file_mmap(struct file *file, unsigned long reqprot,
			 unsigned long prot, unsigned long flags,
			 unsigned long addr, unsigned long addr_only);
extern int cap_task_fix_setuid(struct cred *new, const struct cred *old, int flags);
extern int cap_task_prctl(int option, unsigned long arg2, unsigned long arg3,
			  unsigned long arg4, unsigned long arg5);
extern int cap_task_setscheduler(struct task_struct *p);
extern int cap_task_setioprio(struct task_struct *p, int ioprio);
extern int cap_task_setnice(struct task_struct *p, int nice);
extern int cap_vm_enough_memory(struct mm_struct *mm, long pages);

struct msghdr;
struct sk_buff;
struct sock;
struct sockaddr;
struct socket;
struct flowi;
struct dst_entry;
struct xfrm_selector;
struct xfrm_policy;
struct xfrm_state;
struct xfrm_user_sec_ctx;
struct seq_file;

extern int cap_netlink_send(struct sock *sk, struct sk_buff *skb);
extern int cap_netlink_recv(struct sk_buff *skb, int cap);

void reset_security_ops(void);

#ifdef CONFIG_MMU
extern unsigned long mmap_min_addr;
extern unsigned long dac_mmap_min_addr;
#else
#define dac_mmap_min_addr	0UL
#endif

/*
 * Values used in the task_security_ops calls
 */
/* setuid or setgid, id0 == uid or gid */
#define LSM_SETID_ID	1

/* setreuid or setregid, id0 == real, id1 == eff */
#define LSM_SETID_RE	2

/* setresuid or setresgid, id0 == real, id1 == eff, uid2 == saved */
#define LSM_SETID_RES	4

/* setfsuid or setfsgid, id0 == fsuid or fsgid */
#define LSM_SETID_FS	8

/* forward declares to avoid warnings */
struct sched_param;
struct request_sock;

/* bprm->unsafe reasons */
#define LSM_UNSAFE_SHARE	1
#define LSM_UNSAFE_PTRACE	2
#define LSM_UNSAFE_PTRACE_CAP	4

#ifdef CONFIG_MMU
/*
 * If a hint addr is less than mmap_min_addr change hint to be as
 * low as possible but still greater than mmap_min_addr
 */
static inline unsigned long round_hint_to_min(unsigned long hint)
{
	hint &= PAGE_MASK;
	if (((void *)hint != NULL) &&
	    (hint < mmap_min_addr))
		return PAGE_ALIGN(mmap_min_addr);
	return hint;
}
extern int mmap_min_addr_handler(struct ctl_table *table, int write,
				 void __user *buffer, size_t *lenp, loff_t *ppos);
#endif

#ifdef CONFIG_SECURITY

struct security_mnt_opts {
	char **mnt_opts;
	int *mnt_opts_flags;
	int num_mnt_opts;
};

static inline void security_init_mnt_opts(struct security_mnt_opts *opts)
{
	opts->mnt_opts = NULL;
	opts->mnt_opts_flags = NULL;
	opts->num_mnt_opts = 0;
}

static inline void security_free_mnt_opts(struct security_mnt_opts *opts)
{
	int i;
	if (opts->mnt_opts)
		for (i = 0; i < opts->num_mnt_opts; i++)
			kfree(opts->mnt_opts[i]);
	kfree(opts->mnt_opts);
	opts->mnt_opts = NULL;
	kfree(opts->mnt_opts_flags);
	opts->mnt_opts_flags = NULL;
	opts->num_mnt_opts = 0;
}

/**
 * stnuct security_operations - main security structure
 *
 * Security module identifier.
 *
 * @name:
 *	A string that acts as a unique identifeir for the LSM with max number
 *	of characters = SECURITY_NAME_MAX.
 *
 * Security hooks for program execution operations.
 *
 * @bprm_set_creds:
 *	Save security information in the bprm->security field, typically based
 *	on information about the bprm->file, for later use by the apply_creds
 *	hook.  This hook may also optionally check permissions (e.g. for
 *	transitions between security domains).
 *	This hook may be called multiple times during a single execve, e.g. for
 *	interpreters.  The hook can tell whether it has already been called by
 *	checking to see if @bprm->security is non-NULL.  If so, then the hook
 *	may decide either to retain the security information saved earlier or
 *	to replace it.
 *	@bprm contains the linux_binprm structure.
 *	Return 0 if the hook is successful and permission is granted.
 * @bprm_check_security:
 *	This hook mediates the point when a search for a binary handler will
 *	begin.  It allows a check the @bprm->security value which is set in the
 *	preceding set_creds call.  The primary difference from set_creds is
 *	that the argv list and envp list are reliably available in @bprm.  This
 *	hook may be called multiple times during a single execve; and in each
 *	pass set_creds is called first.
 *	@bprm contains the linux_binprm structure.
 *	Return 0 if the hook is successful and permission is granted.
 *
 * Security hooks for filesystem operations.
 *
 * @sb_mount:
 *	Check permission before an object specified by @dev_name is mounted on
 *	the mount point named by @nd.  For an ordinary mount, @dev_name
 *	identifies a device if the file system type requires a device.  For a
 *	remount (@flags & MS_REMOUNT), @dev_name is irrelevant.  For a
 *	loopback/bind mount (@flags & MS_BIND), @dev_name identifies the
 *	pathname of the object being mounted.
 *	@dev_name contains the name for object being mounted.
 *	@path contains the path for mount point object.
 *	@type contains the filesystem type.
 *	@flags contains the mount flags.
 *	@data contains the filesystem-specific data.
 *	Return 0 if permission is granted.
 * @sb_umount:
 *	Check permission before the @mnt file system is unmounted.
 *	@mnt contains the mounted file system.
 *	@flags contains the unmount flags, e.g. MNT_FORCE.
 *	Return 0 if permission is granted.
 * @sb_pivotroot:
 *	Check permission before pivoting the root filesystem.
 *	@old_path contains the path for the new location of the current root (put_old).
 *	@new_path contains the path for the new root (new_root).
 *	Return 0 if permission is granted.
 
 * Security hooks for inode operations.
 *
 * @inode_alloc_security:
 *	Allocate and attach a security structure to @inode->i_security.  The
 *	i_security field is initialized to NULL when the inode structure is
 *	allocated.
 *	@inode contains the inode structure.
 *	Return 0 if operation was successful.
 * @path_symlink:
 *	Check the permission to create a symbolic link to a file.
 *	@dir contains the path structure of parent directory of
 *	the symbolic link.
 *	@dentry contains the dentry structure of the symbolic link.
 *	@old_name contains the pathname of file.
 *	Return 0 if permission is granted.
 * @path_mkdir:
 *	Check permissions to create a new directory in the existing directory
 *	associated with path strcture @path.
 *	@dir containst the path structure of parent of the directory
 *	to be created.
 *	@dentry contains the dentry structure of new directory.
 *	@mode contains the mode of new directory.
 *	Return 0 if permission is granted.
 * @path_rmdir:
 *	Check the permission to remove a directory.
 *	@dir contains the path structure of parent of the directory to be
 *	removed.
 *	@dentry contains the dentry structure of directory to be removed.
 *	Return 0 if permission is granted.
* @path_mknod:
 *	Check permissions when creating a file. Note that this hook is called
 *	even if mknod operation is being done for a regular file.
 *	@dir contains the path structure of parent of the new file.
 *	@dentry contains the dentry structure of the new file.
 *	@mode contains the mode of the new file.
 *	@dev contains the undecoded device number. Use new_decode_dev() to get
 *	the decoded device number.
 *	Return 0 if permission is granted.
 * @path_rename:
 *	Check for permission to rename a file or directory.
 *	@old_dir contains the path structure for parent of the old link.
 *	@old_dentry contains the dentry structure of the old link.
 *	@new_dir contains the path structure for parent of the new link.
 *	@new_dentry contains the dentry structure of the new link.
 *	Return 0 if permission is granted.
 * @path_chmod:
 *	Check for permission to change DAC's permission of a file or directory.
 *	@dentry contains the dentry structure.
 *	@mnt contains the vfsmnt structure.
 *	@mode contains DAC's mode.
 *	Return 0 if permission is granted.
 * @path_chown:
 *	Check for permission to change owner/group of a file or directory.
 *	@path contains the path structure.
 *	@uid contains new owner's ID.
 *	@gid contains new group's ID.
 *	Return 0 if permission is granted.
 * @path_chroot:
 *	Check for permission to change root directory.
 *	@path contains the path structure.
 *	Return 0 if permission is granted.
 * @inode_getattr:
 *	Check permission before obtaining file attributes.
 *	@mnt is the vfsmount where the dentry was looked up
 *	@dentry contains the dentry structure for the file.
 *	Return 0 if permission is granted.
 
* Security hooks for file operations
 * @file_fcntl:
 *	Check permission before allowing the file operation specified by @cmd
 *	from being performed on the file @file.  Note that @arg can sometimes
 *	represents a user space pointer; in other cases, it may be a simple
 *	integer value.  When @arg represents a user space pointer, it should
 *	never be used by the security module.
 *	@file contains the file structure.
 *	@cmd contains the operation to be performed.
 *	@arg contains the operational arguments.
 *	Return 0 if permission is granted.
 * @file_ioctl:
 *      @file contains the file structure.
 *      @cmd contains the operation to perform.
 *      @arg contains the operational arguments.
 *      Check permission for an ioctl operation on @file.  Note that @arg can
 *      sometimes represents a user space pointer; in other cases, it may be a
 *      simple integer value.  When @arg represents a user space pointer, it
 *      should never be used by the security module.
 *      Return 0 if permission is granted.
 *
 * Security hook for dentry
 *
 * @dentry_open
 *	Save open-time permission checking state for later use upon
 *	file_permission, and recheck access if anything has changed
 *	since inode_permission.
 *
 * Security hooks for task operations.
 * @cred_alloc_blank:
 *	@cred points to the credentials.
 *	@gfp indicates the atomicity of any memory allocations.
 *	Only allocate sufficient memory and attach to @cred such that
 *	cred_transfer() will not get ENOMEM.
 * @cred_free:
 *	@cred points to the credentials.
 *	Deallocate and clear the cred->security field in a set of credentials.
 * @cred_prepare:
 *	@new points to the new credentials.
 *	@old points to the original credentials.
 *	@gfp indicates the atomicity of any memory allocations.
 *	Prepare a new set of credentials by copying the data from the old set.
 * @cred_transfer:
 *	@new points to the new credentials.
 *	@old points to the original credentials.
 *	Transfer data from original creds to new creds
 * @cred_restrict_privilege:
 * 	@parent points to the parent process credentials.
 *	@child  points to the current process credentials.
 *	Modify the credentials by reseting uid/gid. 
 *
 * Security hooks for socket operations.
 *
 * @socket_bind:
 *	Check permission before socket protocol layer bind operation is
 *	performed and the socket @sock is bound to the address specified in the
 *	@address parameter.
 *	@sock contains the socket structure.
 *	@address contains the address to bind to.
 *	@addrlen contains the length of address.
 *	Return 0 if permission is granted.
 * @socket_connect:
 *	Check permission before socket protocol layer connect operation
 *	attempts to connect socket @sock to a remote address, @address.
 *	@sock contains the socket structure.
 *	@address contains the address of remote endpoint.
 *	@addrlen contains the length of address.
 *	Return 0 if permission is granted.
 * @socket_listen:
 *	Check permission before socket protocol layer listen operation.
 *	@sock contains the socket structure.
 *	@backlog contains the maximum length for the pending connection queue.
 *	Return 0 if permission is granted.
 * @socket_sendmsg:
 *	Check permission before transmitting a message to another socket.
 *	@sock contains the socket structure.
 *	@msg contains the message to be transmitted.
 *	@size contains the size of message.
 *	Return 0 if permission is granted.
 * This is the main security structure.
 */
struct security_operations {
	char name[SECURITY_NAME_MAX + 1];
	int (*bprm_set_creds) (struct linux_binprm *bprm);
	int (*bprm_check_security) (struct linux_binprm *bprm);
	int (*sb_mount) (char *dev_name, struct path *path,
			 char *type, unsigned long flags, void *data);
	int (*sb_umount) (struct vfsmount *mnt, int flags);
	int (*sb_pivotroot) (struct path *old_path,
			     struct path *new_path);
#ifdef CONFIG_SECURITY_PATH
	int (*path_unlink) (struct path *dir, struct dentry *dentry);
	int (*path_mkdir) (struct path *dir, struct dentry *dentry, int mode);
	int (*path_rmdir) (struct path *dir, struct dentry *dentry);
	int (*path_mknod) (struct path *dir, struct dentry *dentry, int mode,
			   unsigned int dev);
	int (*path_truncate) (struct path *path);
	int (*path_symlink) (struct path *dir, struct dentry *dentry,
			     const char *old_name);
	int (*path_link) (struct dentry *old_dentry, struct path *new_dir,
			  struct dentry *new_dentry);
	int (*path_rename) (struct path *old_dir, struct dentry *old_dentry,
			    struct path *new_dir, struct dentry *new_dentry);
	int (*path_chmod) (struct dentry *dentry, struct vfsmount *mnt,
			   mode_t mode);
	int (*path_chown) (struct path *path, uid_t uid, gid_t gid);
	int (*path_chroot) (struct path *path);
#endif
	int (*inode_getattr) (struct vfsmount *mnt, struct dentry *dentry);
	int (*file_ioctl) (struct file *file, unsigned int cmd,
			   unsigned long arg);
	int (*file_fcntl) (struct file *file, unsigned int cmd,
			   unsigned long arg);
	int (*dentry_open) (struct file *file, const struct cred *cred);
	int (*cred_alloc_blank) (struct cred *cred, gfp_t gfp);
	void (*cred_free) (struct cred *cred);
	int (*cred_prepare)(struct cred *new, const struct cred *old,
			    gfp_t gfp);
	void (*cred_transfer)(struct cred *new, const struct cred *old);
#ifdef CONFIG_SECURITY_TOMOYO_RESTRICT_PRIVILEGE
        void (*cred_restrict_privilege)(const struct cred *parent, struct cred *child);
#endif

#ifdef CONFIG_SECURITY_NETWORK
        int (*socket_bind) (struct socket *sock,
                            struct sockaddr *address, int addrlen);
        int (*socket_connect) (struct socket *sock,
                               struct sockaddr *address, int addrlen);
	int (*socket_sendmsg) (struct socket *sock,
			       struct msghdr *msg, int size);
         int (*socket_listen) (struct socket *sock, int backlog);
#endif	/* CONFIG_SECURITY_NETWORK */

};

/* prototypes */
extern int security_init(void);
extern int security_module_enable(struct security_operations *ops);
extern int register_security(struct security_operations *ops);

/* Security operations */
int security_bprm_set_creds(struct linux_binprm *bprm);
int security_bprm_check(struct linux_binprm *bprm);
int security_sb_mount(char *dev_name, struct path *path,
		      char *type, unsigned long flags, void *data);
int security_sb_umount(struct vfsmount *mnt, int flags);
int security_sb_pivotroot(struct path *old_path, struct path *new_path);
int security_inode_getattr(struct vfsmount *mnt, struct dentry *dentry);
int security_file_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
int security_file_fcntl(struct file *file, unsigned int cmd, unsigned long arg);
int security_dentry_open(struct file *file, const struct cred *cred);
int security_cred_alloc_blank(struct cred *cred, gfp_t gfp);
void security_cred_free(struct cred *cred);
int security_prepare_creds(struct cred *new, const struct cred *old, gfp_t gfp);
void security_transfer_creds(struct cred *new, const struct cred *old);

#ifdef CONFIG_SECURITY_TOMOYO_RESTRICT_PRIVILEGE
void security_restrict_creds(const struct cred *parent, struct cred *child);
#endif
#else /*CONFIG_SECURITY*/
struct security_mnt_opts {
};

static inline void security_init_mnt_opts(struct security_mnt_opts *opts)
{
}

static inline void security_free_mnt_opts(struct security_mnt_opts *opts)
{
}


static inline int security_init(void)
{
	return 0;
}

static inline int security_bprm_set_creds(struct linux_binprm *bprm)
{
        return cap_bprm_set_creds(bprm);
}

static inline int security_bprm_check(struct linux_binprm *bprm)
{
        return 0;
}

static inline int security_sb_mount(char *dev_name, struct path *path,
                                    char *type, unsigned long flags,
                                    void *data)
{
        return 0;
}

static inline int security_sb_umount(struct vfsmount *mnt, int flags)
{
        return 0;
}

static inline int security_sb_pivotroot(struct path *old_path,
                                        struct path *new_path)
{
        return 0;
}

static inline int security_inode_getattr(struct vfsmount *mnt,
                                          struct dentry *dentry)
{
        return 0;
}
static inline int security_file_ioctl(struct file *file, unsigned int cmd,
                                      unsigned long arg)
{
        return 0;
}
static inline int security_file_fcntl(struct file *file, unsigned int cmd,
                                      unsigned long arg)
{
        return 0;
}

static inline int security_dentry_open(struct file *file,
                                       const struct cred *cred)
{
        return 0;
}

static inline int security_cred_alloc_blank(struct cred *cred, gfp_t gfp)
{
        return 0;
}

static inline void security_cred_free(struct cred *cred)
{ }

static inline int security_prepare_creds(struct cred *new,
                                         const struct cred *old,
                                         gfp_t gfp)
{
        return 0;
}

static inline void security_transfer_creds(struct cred *new,
                                           const struct cred *old)
{
}

#ifdef CONFIG_SECURITY_TOMOYO_RESTRICT_PRIVILEGE
static inline void security_restrict_creds(const struct cred *parent, struct cred *child)
{
}
#endif

#endif  /*CONFIG_SECURITY*/

static inline int security_syslog(int type)
{
	return 0;
}

static inline int security_quotactl(int cmds, int type, int id,
				     struct super_block *sb)
{
	return 0;
}

static inline int security_quota_on(struct dentry *dentry)
{
	return 0;
}

static inline int security_shm_alloc(struct shmid_kernel *shp)
{
	return 0;
}

static inline void security_shm_free(struct shmid_kernel *shp)
{ }

static inline int security_shm_associate(struct shmid_kernel *shp,
					 int shmflg)
{
	return 0;
}

static inline int security_shm_shmctl(struct shmid_kernel *shp, int cmd)
{
	return 0;
}

static inline int security_shm_shmat(struct shmid_kernel *shp,
				     char __user *shmaddr, int shmflg)
{
	return 0;
}
static inline int security_task_create(unsigned long clone_flags)
{
	return 0;
}

static inline int security_task_fix_setuid(struct cred *new,
					   const struct cred *old,
					   int flags)
{
	return cap_task_fix_setuid(new, old, flags);
}

static inline int security_task_setpgid(struct task_struct *p, pid_t pgid)
{
	return 0;
}

static inline int security_task_getpgid(struct task_struct *p)
{
	return 0;
}

static inline int security_task_getsid(struct task_struct *p)
{
	return 0;
}

static inline void security_task_getsecid(struct task_struct *p, u32 *secid)
{
	*secid = 0;
}

static inline int security_task_setnice(struct task_struct *p, int nice)
{
	return cap_task_setnice(p, nice);
}

static inline int security_task_setioprio(struct task_struct *p, int ioprio)
{
	return cap_task_setioprio(p, ioprio);
}

static inline int security_task_getioprio(struct task_struct *p)
{
	return 0;
}

static inline int security_task_setrlimit(struct task_struct *p,
					  unsigned int resource,
					  struct rlimit *new_rlim)
{
	return 0;
}

static inline int security_task_setscheduler(struct task_struct *p)
{
	return cap_task_setscheduler(p);
}

static inline int security_task_getscheduler(struct task_struct *p)
{
	return 0;
}

static inline int security_task_movememory(struct task_struct *p)
{
	return 0;
}

static inline int security_task_kill(struct task_struct *p,
				     struct siginfo *info, int sig,
				     u32 secid)
{
	return 0;
}

static inline int security_task_wait(struct task_struct *p)
{
	return 0;
}

static inline int security_task_prctl(int option, unsigned long arg2,
				      unsigned long arg3,
				      unsigned long arg4,
				      unsigned long arg5)
{
	return cap_task_prctl(option, arg2, arg3, arg3, arg5);
}

static inline void security_task_to_inode(struct task_struct *p, struct inode *inode)
{ }

static inline int security_inode_notifysecctx(struct inode *inode, void *ctx, u32 ctxlen)
{
	return -EOPNOTSUPP;
}
static inline int security_inode_setsecctx(struct dentry *dentry, void *ctx, u32 ctxlen)
{
	return -EOPNOTSUPP;
}
static inline int security_inode_getsecctx(struct inode *inode, void **ctx, u32 *ctxlen)
{
	return -EOPNOTSUPP;
}

static inline int security_inode_alloc(struct inode *inode)
{
	return 0;
}

static inline void security_inode_free(struct inode *inode)
{ }

static inline int security_inode_init_security(struct inode *inode,
						struct inode *dir,
						const struct qstr *qstr,
						char **name,
						void **value,
						size_t *len)
{
	return -EOPNOTSUPP;
}

static inline int security_inode_create(struct inode *dir,
					 struct dentry *dentry,
					 int mode)
{
	return 0;
}

static inline int security_inode_link(struct dentry *old_dentry,
				       struct inode *dir,
				       struct dentry *new_dentry)
{
	return 0;
}

static inline int security_inode_unlink(struct inode *dir,
					 struct dentry *dentry)
{
	return 0;
}

static inline int security_inode_symlink(struct inode *dir,
					  struct dentry *dentry,
					  const char *old_name)
{
	return 0;
}

static inline int security_inode_mkdir(struct inode *dir,
					struct dentry *dentry,
					int mode)
{
	return 0;
}

static inline int security_inode_rmdir(struct inode *dir,
					struct dentry *dentry)
{
	return 0;
}

static inline int security_inode_mknod(struct inode *dir,
					struct dentry *dentry,
					int mode, dev_t dev)
{
	return 0;
}

static inline int security_inode_rename(struct inode *old_dir,
					 struct dentry *old_dentry,
					 struct inode *new_dir,
					 struct dentry *new_dentry)
{
	return 0;
}

static inline int security_inode_readlink(struct dentry *dentry)
{
	return 0;
}

static inline int security_inode_follow_link(struct dentry *dentry,
					      struct nameidata *nd)
{
	return 0;
}

static inline int security_inode_permission(struct inode *inode, int mask)
{
	return 0;
}

static inline int security_inode_exec_permission(struct inode *inode,
						  unsigned int flags)
{
	return 0;
}

static inline int security_inode_setattr(struct dentry *dentry,
					  struct iattr *attr)
{
	return 0;
}

static inline int security_inode_setxattr(struct dentry *dentry,
		const char *name, const void *value, size_t size, int flags)
{
	return cap_inode_setxattr(dentry, name, value, size, flags);
}

static inline void security_inode_post_setxattr(struct dentry *dentry,
		const char *name, const void *value, size_t size, int flags)
{ }

static inline int security_inode_getxattr(struct dentry *dentry,
			const char *name)
{
	return 0;
}

static inline int security_inode_listxattr(struct dentry *dentry)
{
	return 0;
}

static inline int security_inode_removexattr(struct dentry *dentry,
			const char *name)
{
	return cap_inode_removexattr(dentry, name);
}

static inline int security_inode_need_killpriv(struct dentry *dentry)
{
	return cap_inode_need_killpriv(dentry);
}

static inline int security_inode_killpriv(struct dentry *dentry)
{
	return cap_inode_killpriv(dentry);
}

static inline int security_inode_getsecurity(const struct inode *inode, const char *name, void **buffer, bool alloc)
{
	return -EOPNOTSUPP;
}

static inline int security_inode_setsecurity(struct inode *inode, const char *name, const void *value, size_t size, int flags)
{
	return -EOPNOTSUPP;
}

static inline int security_inode_listsecurity(struct inode *inode, char *buffer, size_t buffer_size)
{
	return 0;
}

static inline void security_inode_getsecid(const struct inode *inode, u32 *secid)
{
	*secid = 0;
}

static inline int security_file_permission(struct file *file, int mask)
{
	return 0;
}

static inline int security_file_alloc(struct file *file)
{
	return 0;
}

static inline void security_file_free(struct file *file)
{ }

static inline int security_file_mmap(struct file *file, unsigned long reqprot,
				     unsigned long prot,
				     unsigned long flags,
				     unsigned long addr,
				     unsigned long addr_only)
{
	return cap_file_mmap(file, reqprot, prot, flags, addr, addr_only);
}

static inline int security_file_mprotect(struct vm_area_struct *vma,
					 unsigned long reqprot,
					 unsigned long prot)
{
	return 0;
}

static inline int security_file_lock(struct file *file, unsigned int cmd)
{
	return 0;
}

static inline int security_file_set_fowner(struct file *file)
{
	return 0;
}

static inline int security_file_send_sigiotask(struct task_struct *tsk,
					       struct fown_struct *fown,
					       int sig)
{
	return 0;
}

static inline int security_file_receive(struct file *file)
{
	return 0;
}

static inline int security_ipc_permission(struct kern_ipc_perm *ipcp,
					  short flag)
{
	return 0;
}

static inline void security_ipc_getsecid(struct kern_ipc_perm *ipcp, u32 *secid)
{
	*secid = 0;
}

static inline int security_msg_msg_alloc(struct msg_msg *msg)
{
	return 0;
}

static inline void security_msg_msg_free(struct msg_msg *msg)
{ }

static inline int security_msg_queue_alloc(struct msg_queue *msq)
{
	return 0;
}

static inline void security_msg_queue_free(struct msg_queue *msq)
{ }

static inline int security_msg_queue_associate(struct msg_queue *msq,
					       int msqflg)
{
	return 0;
}

static inline int security_msg_queue_msgctl(struct msg_queue *msq, int cmd)
{
	return 0;
}

static inline int security_msg_queue_msgsnd(struct msg_queue *msq,
					    struct msg_msg *msg, int msqflg)
{
	return 0;
}

static inline int security_msg_queue_msgrcv(struct msg_queue *msq,
					    struct msg_msg *msg,
					    struct task_struct *target,
					    long type, int mode)
{
	return 0;
}

static inline int security_sem_alloc(struct sem_array *sma)
{
	return 0;
}

static inline void security_sem_free(struct sem_array *sma)
{ }

static inline int security_sem_associate(struct sem_array *sma, int semflg)
{
	return 0;
}

static inline int security_sem_semctl(struct sem_array *sma, int cmd)
{
	return 0;
}

static inline int security_sem_semop(struct sem_array *sma,
				     struct sembuf *sops, unsigned nsops,
				     int alter)
{
	return 0;
}

static inline int security_kernel_act_as(struct cred *cred, u32 secid)
{
	return 0;
}

static inline int security_kernel_create_files_as(struct cred *cred,
						  struct inode *inode)
{
	return 0;
}

static inline int security_kernel_module_request(char *kmod_name)
{
	return 0;
}

static inline void security_d_instantiate(struct dentry *dentry, struct inode *inode)
{ }

static inline int security_getprocattr(struct task_struct *p, char *name, char **value)
{
	return -EINVAL;
}

static inline int security_setprocattr(struct task_struct *p, char *name, void *value, size_t size)
{
	return -EINVAL;
}

static inline int security_secid_to_secctx(u32 secid, char **secdata, u32 *seclen)
{
	return -EOPNOTSUPP;
}

static inline int security_secctx_to_secid(const char *secdata,
					   u32 seclen,
					   u32 *secid)
{
	return -EOPNOTSUPP;
}

static inline void security_release_secctx(char *secdata, u32 seclen)
{
}

static inline int security_sb_alloc(struct super_block *sb)
{
	return 0;
}

static inline void security_sb_free(struct super_block *sb)
{ }

static inline int security_sb_copy_data(char *orig, char *copy)
{
	return 0;
}

static inline int security_sb_remount(struct super_block *sb, void *data)
{
	return 0;
}

static inline int security_sb_kern_mount(struct super_block *sb, int flags, void *data)
{
	return 0;
}

static inline int security_sb_show_options(struct seq_file *m,
					   struct super_block *sb)
{
	return 0;
}

static inline int security_sb_statfs(struct dentry *dentry)
{
	return 0;
}


static inline int security_sb_set_mnt_opts(struct super_block *sb,
					   struct security_mnt_opts *opts)
{
	return 0;
}

static inline void security_sb_clone_mnt_opts(const struct super_block *oldsb,
					      struct super_block *newsb)
{ }

static inline int security_sb_parse_opts_str(char *options, struct security_mnt_opts *opts)
{
	return 0;
}

static inline void security_bprm_committing_creds(struct linux_binprm *bprm)
{
}

static inline void security_bprm_committed_creds(struct linux_binprm *bprm)
{
}

static inline int security_ptrace_traceme(struct task_struct *parent)
{
	return cap_ptrace_traceme(parent);
}

static inline int security_ptrace_access_check(struct task_struct *child,
					     unsigned int mode)
{
	return cap_ptrace_access_check(child, mode);
}


static inline int security_capget(struct task_struct *target,
				   kernel_cap_t *effective,
				   kernel_cap_t *inheritable,
				   kernel_cap_t *permitted)
{
	return cap_capget(target, effective, inheritable, permitted);
}

static inline int security_capset(struct cred *new,
				   const struct cred *old,
				   const kernel_cap_t *effective,
				   const kernel_cap_t *inheritable,
				   const kernel_cap_t *permitted)
{
	return cap_capset(new, old, effective, inheritable, permitted);
}

static inline int security_bprm_secureexec(struct linux_binprm *bprm)
{
	return cap_bprm_secureexec(bprm);
}

static inline int security_settime(const struct timespec *ts,
const struct timezone *tz)
{
return cap_settime(ts, tz);
}

static inline int security_netlink_send(struct sock *sk, struct sk_buff *skb)
{
	return cap_netlink_send(sk, skb);
}

static inline int security_netlink_recv(struct sk_buff *skb, int cap)
{
	return cap_netlink_recv(skb, cap);
}


#ifdef CONFIG_SECURITY_NETWORK
int security_socket_bind(struct socket *sock, struct sockaddr *address, int addrlen);
int security_socket_connect(struct socket *sock, struct sockaddr *address, int addrlen);
int security_socket_listen(struct socket *sock, int backlog);
int security_socket_sendmsg(struct socket *sock, struct msghdr *msg, int size);
#else /*CONFIG_SECURITY_NETWORK*/
static inline int security_socket_bind(struct socket *sock,
                                       struct sockaddr *address,
                                       int addrlen)
{
        return 0;
}

static inline int security_socket_connect(struct socket *sock,
                                          struct sockaddr *address,
                                          int addrlen)
{
        return 0;
}

static inline int security_socket_listen(struct socket *sock, int backlog)
{
        return 0;
}
static inline int security_socket_sendmsg(struct socket *sock,
                                          struct msghdr *msg, int size)
{
        return 0;
}
#endif  /* CONFIG_SECURITY_NETWORK  */
static inline int security_unix_stream_connect(struct sock *sock,
					       struct sock *other,
					       struct sock *newsk)
{
	return 0;
}

static inline int security_unix_may_send(struct socket *sock,
					 struct socket *other)
{
	return 0;
}

static inline int security_socket_create(int family, int type,
					 int protocol, int kern)
{
	return 0;
}

static inline int security_socket_post_create(struct socket *sock,
					      int family,
					      int type,
					      int protocol, int kern)
{
	return 0;
}


static inline int security_socket_accept(struct socket *sock,
					 struct socket *newsock)
{
	return 0;
}

static inline int security_socket_recvmsg(struct socket *sock,
					  struct msghdr *msg, int size,
					  int flags)
{
	return 0;
}

static inline int security_socket_getsockname(struct socket *sock)
{
	return 0;
}

static inline int security_socket_getpeername(struct socket *sock)
{
	return 0;
}

static inline int security_socket_getsockopt(struct socket *sock,
					     int level, int optname)
{
	return 0;
}

static inline int security_socket_setsockopt(struct socket *sock,
					     int level, int optname)
{
	return 0;
}

static inline int security_socket_shutdown(struct socket *sock, int how)
{
	return 0;
}
static inline int security_sock_rcv_skb(struct sock *sk,
					struct sk_buff *skb)
{
	return 0;
}

static inline int security_socket_getpeersec_stream(struct socket *sock, char __user *optval,
						    int __user *optlen, unsigned len)
{
	return -ENOPROTOOPT;
}

static inline int security_socket_getpeersec_dgram(struct socket *sock, struct sk_buff *skb, u32 *secid)
{
	return -ENOPROTOOPT;
}

static inline int security_sk_alloc(struct sock *sk, int family, gfp_t priority)
{
	return 0;
}

static inline void security_sk_free(struct sock *sk)
{
}

static inline void security_sk_clone(const struct sock *sk, struct sock *newsk)
{
}

static inline void security_sk_classify_flow(struct sock *sk, struct flowi *fl)
{
}

static inline void security_req_classify_flow(const struct request_sock *req, struct flowi *fl)
{
}

static inline void security_sock_graft(struct sock *sk, struct socket *parent)
{
}

static inline int security_inet_conn_request(struct sock *sk,
			struct sk_buff *skb, struct request_sock *req)
{
	return 0;
}

static inline void security_inet_csk_clone(struct sock *newsk,
			const struct request_sock *req)
{
}

static inline void security_inet_conn_established(struct sock *sk,
			struct sk_buff *skb)
{
}

static inline int security_secmark_relabel_packet(u32 secid)
{
	return 0;
}

static inline void security_secmark_refcount_inc(void)
{
}

static inline void security_secmark_refcount_dec(void)
{
}

static inline int security_tun_dev_create(void)
{
	return 0;
}

static inline void security_tun_dev_post_create(struct sock *sk)
{
}

static inline int security_tun_dev_attach(struct sock *sk)
{
	return 0;
}

static inline int security_xfrm_policy_alloc(struct xfrm_sec_ctx **ctxp, struct xfrm_user_sec_ctx *sec_ctx)
{
	return 0;
}

static inline int security_xfrm_policy_clone(struct xfrm_sec_ctx *old, struct xfrm_sec_ctx **new_ctxp)
{
	return 0;
}

static inline void security_xfrm_policy_free(struct xfrm_sec_ctx *ctx)
{
}

static inline int security_xfrm_policy_delete(struct xfrm_sec_ctx *ctx)
{
	return 0;
}

static inline int security_xfrm_state_alloc(struct xfrm_state *x,
					struct xfrm_user_sec_ctx *sec_ctx)
{
	return 0;
}

static inline int security_xfrm_state_alloc_acquire(struct xfrm_state *x,
					struct xfrm_sec_ctx *polsec, u32 secid)
{
	return 0;
}

static inline void security_xfrm_state_free(struct xfrm_state *x)
{
}

static inline int security_xfrm_state_delete(struct xfrm_state *x)
{
	return 0;
}

static inline int security_xfrm_policy_lookup(struct xfrm_sec_ctx *ctx, u32 fl_secid, u8 dir)
{
	return 0;
}

static inline int security_xfrm_state_pol_flow_match(struct xfrm_state *x,
			struct xfrm_policy *xp, const struct flowi *fl)
{
	return 1;
}

static inline int security_xfrm_decode_session(struct sk_buff *skb, u32 *secid)
{
	return 0;
}
static inline void security_skb_classify_flow(struct sk_buff *skb, struct flowi *fl)
{
}

static inline int security_capable(struct user_namespace *ns,
				   const struct cred *cred, int cap)
{
	return cap_capable(current, cred, ns, cap, SECURITY_CAP_AUDIT);
}

static inline int security_real_capable(struct task_struct *tsk, struct user_namespace *ns, int cap)
{
	int ret;

	rcu_read_lock();
	ret = cap_capable(tsk, __task_cred(tsk), ns, cap, SECURITY_CAP_AUDIT);
	rcu_read_unlock();
	return ret;
}

static inline
int security_real_capable_noaudit(struct task_struct *tsk, struct user_namespace *ns, int cap)
{
	int ret;

	rcu_read_lock();
	ret = cap_capable(tsk, __task_cred(tsk), ns, cap,
			       SECURITY_CAP_NOAUDIT);
	rcu_read_unlock();
	return ret;
}


static inline int security_vm_enough_memory(long pages)
{
	WARN_ON(current->mm == NULL);
	return cap_vm_enough_memory(current->mm, pages);
}

static inline int security_vm_enough_memory_mm(struct mm_struct *mm, long pages)
{
	WARN_ON(mm == NULL);
	return cap_vm_enough_memory(mm, pages);
}

static inline int security_vm_enough_memory_kern(long pages)
{
	/* If current->mm is a kernel thread then we will pass NULL,
	   for this specific case that is fine */
	return cap_vm_enough_memory(current->mm, pages);
}

/*
 * This is the default capabilities functionality.  Most of these functions
 * are just stubbed out, but a few must call the proper capable code.
 */


#ifdef CONFIG_SECURITY_PATH
int security_path_unlink(struct path *dir, struct dentry *dentry);
int security_path_mkdir(struct path *dir, struct dentry *dentry, int mode);
int security_path_rmdir(struct path *dir, struct dentry *dentry);
int security_path_mknod(struct path *dir, struct dentry *dentry, int mode,
			unsigned int dev);
int security_path_truncate(struct path *path);
int security_path_symlink(struct path *dir, struct dentry *dentry,
			  const char *old_name);
int security_path_link(struct dentry *old_dentry, struct path *new_dir,
		       struct dentry *new_dentry);
int security_path_rename(struct path *old_dir, struct dentry *old_dentry,
			 struct path *new_dir, struct dentry *new_dentry);
int security_path_chmod(struct dentry *dentry, struct vfsmount *mnt,
			mode_t mode);
int security_path_chown(struct path *path, uid_t uid, gid_t gid);
int security_path_chroot(struct path *path);

#else	/* CONFIG_SECURITY_PATH */
static inline int security_path_unlink(struct path *dir, struct dentry *dentry)
{
        return 0;
}

static inline int security_path_mkdir(struct path *dir, struct dentry *dentry,
                                      int mode)
{
        return 0;
}

static inline int security_path_rmdir(struct path *dir, struct dentry *dentry)
{
        return 0;
}

static inline int security_path_mknod(struct path *dir, struct dentry *dentry,
                                      int mode, unsigned int dev)
{
        return 0;
}

static inline int security_path_truncate(struct path *path)
{
        return 0;
}
static inline int security_path_symlink(struct path *dir, struct dentry *dentry,
                                        const char *old_name)
{
        return 0;
}

static inline int security_path_link(struct dentry *old_dentry,
                                     struct path *new_dir,
                                     struct dentry *new_dentry)
{
        return 0;
}

static inline int security_path_rename(struct path *old_dir,
                                       struct dentry *old_dentry,
                                       struct path *new_dir,
                                       struct dentry *new_dentry)
{
        return 0;
}

static inline int security_path_chmod(struct dentry *dentry,
                                      struct vfsmount *mnt,
                                      mode_t mode)
{
        return 0;
}
static inline int security_path_chown(struct path *path, uid_t uid, gid_t gid)
{
        return 0;
}

static inline int security_path_chroot(struct path *path)
{
        return 0;
}
#endif /*CONFIG_SECURITY_PATH*/

#ifdef CONFIG_KEYS
static inline int security_key_alloc(struct key *key,
				     const struct cred *cred,
				     unsigned long flags)
{
	return 0;
}

static inline void security_key_free(struct key *key)
{
}

static inline int security_key_permission(key_ref_t key_ref,
					  const struct cred *cred,
					  key_perm_t perm)
{
	return 0;
}

static inline int security_key_getsecurity(struct key *key, char **_buffer)
{
	*_buffer = NULL;
	return 0;
}

#endif /* CONFIG_KEYS */

#ifdef CONFIG_AUDIT

static inline int security_audit_rule_init(u32 field, u32 op, char *rulestr,
					   void **lsmrule)
{
	return 0;
}

static inline int security_audit_rule_known(struct audit_krule *krule)
{
	return 0;
}

static inline int security_audit_rule_match(u32 secid, u32 field, u32 op,
				   void *lsmrule, struct audit_context *actx)
{
	return 0;
}

static inline void security_audit_rule_free(void *lsmrule)
{ }

#endif /* CONFIG_AUDIT */

#ifdef CONFIG_SECURITYFS

extern struct dentry *securityfs_create_file(const char *name, mode_t mode,
					     struct dentry *parent, void *data,
					     const struct file_operations *fops);
extern struct dentry *securityfs_create_dir(const char *name, struct dentry *parent);
extern void securityfs_remove(struct dentry *dentry);

#else /* CONFIG_SECURITYFS */

static inline struct dentry *securityfs_create_dir(const char *name,
						   struct dentry *parent)
{
	return ERR_PTR(-ENODEV);
}

static inline struct dentry *securityfs_create_file(const char *name,
						    mode_t mode,
						    struct dentry *parent,
						    void *data,
						    const struct file_operations *fops)
{
	return ERR_PTR(-ENODEV);
}

static inline void securityfs_remove(struct dentry *dentry)
{}

#endif

#ifdef CONFIG_SECURITY
static inline char *alloc_secdata(void)
{
	return (char *)get_zeroed_page(GFP_KERNEL);
}

static inline void free_secdata(void *secdata)
{
	free_page((unsigned long)secdata);
}

#else
static inline char *alloc_secdata(void)
{
        return (char *)1;
}

static inline void free_secdata(void *secdata)
{ }
#endif /*CONFIG_SECURITY*/
#endif /* ! __LINUX_SECURITY_H */

