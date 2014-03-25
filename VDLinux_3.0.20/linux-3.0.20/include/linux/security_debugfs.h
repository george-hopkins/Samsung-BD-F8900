#ifndef __SECURITY_DEBUGFS_H
#define __SECURITY_DEBUGFS_H
extern spinlock_t security_debug;
enum security_hook_id
{
  LSM_PATH_UNLINK,
  LSM_PATH_MKDIR,
  LSM_PATH_RMDIR,
  LSM_PATH_MKNOD,
  LSM_PATH_TRUNCATE,
  LSM_PATH_SYMLINK,
  LSM_PATH_LINK,
  LSM_PATH_RENAME,
  LSM_PATH_CHMOD,
  LSM_PATH_CHOWN,
  LSM_PATH_CHROOT,
  LSM_SB_MOUNT,
  LSM_SB_UMOUNT,
  LSM_SB_PIVOTROOT,
  LSM_BPRM_SET_CREDS,
  LSM_BPRM_CHECK_SECURITY,
  LSM_CRED_ALLOC_BLANK,
  LSM_CRED_FREE,
  LSM_PREPARE_CREDS,
  LSM_CRED_TRANSFER,
  LSM_DENTRY_OPEN,
  LSM_FILE_FCNTL,
  LSM_FILE_IOCTL,
  LSM_FILE_PERMISSION,
  LSM_MSG_MSG_ALLOC,
  LSM_TASK_PRCTL,
  LSM_VM_ENOUGH_MEMORY,
  LSM_INODE_GETATTR,
  LSM_INODE_EXEC_PERMISSION,
  LSM_VM_ENOUGH_MEMORY_MM,
  LSM_SEM_SEMCTL,
  LSM_IPC_PERMISSION,
  LSM_CAPABLE,
  LSM_INODE_PERMISSION,
  LSM_INODE_FOLLOW_LINK,
  MAX_SECURITY_HOOK_ID
};

struct hook_call_time
{
    unsigned long counter;
    unsigned long min_time;
    unsigned long max_time;
    unsigned long avg_time;	
    unsigned long total_time;
};
extern struct hook_call_time lsm_call_time[MAX_SECURITY_HOOK_ID];
void  security_hook_calltime (int num, struct timespec after, struct timespec before);
extern unsigned int check_performance;
#endif /* ! SECURITY_DEBUGFS */
