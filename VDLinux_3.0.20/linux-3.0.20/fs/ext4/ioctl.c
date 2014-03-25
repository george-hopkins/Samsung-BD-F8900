/*
 * linux/fs/ext4/ioctl.c
 *
 * Copyright (C) 1993, 1994, 1995
 * Remy Card (card@masi.ibp.fr)
 * Laboratoire MASI - Institut Blaise Pascal
 * Universite Pierre et Marie Curie (Paris VI)
 */

#include <linux/fs.h>
#include <linux/jbd2.h>
#include <linux/capability.h>
#include <linux/time.h>
#include <linux/compat.h>
#include <linux/mount.h>
#include <linux/file.h>
#include <asm/uaccess.h>
#include <linux/sort.h>
#include <linux/vmalloc.h>
#include "ext4_jbd2.h"
#include "ext4.h"

long ext4_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	struct inode *inode = filp->f_dentry->d_inode;
	struct ext4_inode_info *ei = EXT4_I(inode);
	unsigned int flags;

	ext4_debug("cmd = %u, arg = %lu\n", cmd, arg);

	switch (cmd) {
	case EXT4_IOC_GETFLAGS:
		ext4_get_inode_flags(ei);
		flags = ei->i_flags & EXT4_FL_USER_VISIBLE;
		return put_user(flags, (int __user *) arg);
	case EXT4_IOC_SETFLAGS: {
		handle_t *handle = NULL;
		int err, migrate = 0;
		struct ext4_iloc iloc;
		unsigned int oldflags;
		unsigned int jflag;

		if (!inode_owner_or_capable(inode))
			return -EACCES;

		if (get_user(flags, (int __user *) arg))
			return -EFAULT;

		err = mnt_want_write(filp->f_path.mnt);
		if (err)
			return err;

		flags = ext4_mask_flags(inode->i_mode, flags);

		err = -EPERM;
		mutex_lock(&inode->i_mutex);
		/* Is it quota file? Do not allow user to mess with it */
		if (IS_NOQUOTA(inode))
			goto flags_out;

		oldflags = ei->i_flags;

		/* The JOURNAL_DATA flag is modifiable only by root */
		jflag = flags & EXT4_JOURNAL_DATA_FL;

		/*
		 * The IMMUTABLE and APPEND_ONLY flags can only be changed by
		 * the relevant capability.
		 *
		 * This test looks nicer. Thanks to Pauline Middelink
		 */
		if ((flags ^ oldflags) & (EXT4_APPEND_FL | EXT4_IMMUTABLE_FL)) {
			if (!capable(CAP_LINUX_IMMUTABLE))
				goto flags_out;
		}

		/*
		 * The JOURNAL_DATA flag can only be changed by
		 * the relevant capability.
		 */
		if ((jflag ^ oldflags) & (EXT4_JOURNAL_DATA_FL)) {
			if (!capable(CAP_SYS_RESOURCE))
				goto flags_out;
		}
		if (oldflags & EXT4_EXTENTS_FL) {
			/* We don't support clearning extent flags */
			if (!(flags & EXT4_EXTENTS_FL)) {
				err = -EOPNOTSUPP;
				goto flags_out;
			}
		} else if (flags & EXT4_EXTENTS_FL) {
			/* migrate the file */
			migrate = 1;
			flags &= ~EXT4_EXTENTS_FL;
		}

		if (flags & EXT4_EOFBLOCKS_FL) {
			/* we don't support adding EOFBLOCKS flag */
			if (!(oldflags & EXT4_EOFBLOCKS_FL)) {
				err = -EOPNOTSUPP;
				goto flags_out;
			}
		} else if (oldflags & EXT4_EOFBLOCKS_FL)
			ext4_truncate(inode);

		handle = ext4_journal_start(inode, 1);
		if (IS_ERR(handle)) {
			err = PTR_ERR(handle);
			goto flags_out;
		}
		if (IS_SYNC(inode))
			ext4_handle_sync(handle);
		err = ext4_reserve_inode_write(handle, inode, &iloc);
		if (err)
			goto flags_err;

		flags = flags & EXT4_FL_USER_MODIFIABLE;
		flags |= oldflags & ~EXT4_FL_USER_MODIFIABLE;
		ei->i_flags = flags;

		ext4_set_inode_flags(inode);
		inode->i_ctime = ext4_current_time(inode);

		err = ext4_mark_iloc_dirty(handle, inode, &iloc);
flags_err:
		ext4_journal_stop(handle);
		if (err)
			goto flags_out;

		if ((jflag ^ oldflags) & (EXT4_JOURNAL_DATA_FL))
			err = ext4_change_inode_journal_flag(inode, jflag);
		if (err)
			goto flags_out;
		if (migrate)
			err = ext4_ext_migrate(inode);
flags_out:
		mutex_unlock(&inode->i_mutex);
		mnt_drop_write(filp->f_path.mnt);
		return err;
	}
	case EXT4_IOC_GETVERSION:
	case EXT4_IOC_GETVERSION_OLD:
		return put_user(inode->i_generation, (int __user *) arg);
	case EXT4_IOC_SETVERSION:
	case EXT4_IOC_SETVERSION_OLD: {
		handle_t *handle;
		struct ext4_iloc iloc;
		__u32 generation;
		int err;

		if (!inode_owner_or_capable(inode))
			return -EPERM;

		err = mnt_want_write(filp->f_path.mnt);
		if (err)
			return err;
		if (get_user(generation, (int __user *) arg)) {
			err = -EFAULT;
			goto setversion_out;
		}

		handle = ext4_journal_start(inode, 1);
		if (IS_ERR(handle)) {
			err = PTR_ERR(handle);
			goto setversion_out;
		}
		err = ext4_reserve_inode_write(handle, inode, &iloc);
		if (err == 0) {
			inode->i_ctime = ext4_current_time(inode);
			inode->i_generation = generation;
			err = ext4_mark_iloc_dirty(handle, inode, &iloc);
		}
		ext4_journal_stop(handle);
setversion_out:
		mnt_drop_write(filp->f_path.mnt);
		return err;
	}
#ifdef CONFIG_JBD2_DEBUG
	case EXT4_IOC_WAIT_FOR_READONLY:
		/*
		 * This is racy - by the time we're woken up and running,
		 * the superblock could be released.  And the module could
		 * have been unloaded.  So sue me.
		 *
		 * Returns 1 if it slept, else zero.
		 */
		{
			struct super_block *sb = inode->i_sb;
			DECLARE_WAITQUEUE(wait, current);
			int ret = 0;

			set_current_state(TASK_INTERRUPTIBLE);
			add_wait_queue(&EXT4_SB(sb)->ro_wait_queue, &wait);
			if (timer_pending(&EXT4_SB(sb)->turn_ro_timer)) {
				schedule();
				ret = 1;
			}
			remove_wait_queue(&EXT4_SB(sb)->ro_wait_queue, &wait);
			return ret;
		}
#endif
	case EXT4_IOC_GROUP_EXTEND: {
		ext4_fsblk_t n_blocks_count;
		struct super_block *sb = inode->i_sb;
		int err, err2=0;

		if (!capable(CAP_SYS_RESOURCE))
			return -EPERM;

		if (get_user(n_blocks_count, (__u32 __user *)arg))
			return -EFAULT;

		err = mnt_want_write(filp->f_path.mnt);
		if (err)
			return err;

		err = ext4_group_extend(sb, EXT4_SB(sb)->s_es, n_blocks_count);
		if (EXT4_SB(sb)->s_journal) {
			jbd2_journal_lock_updates(EXT4_SB(sb)->s_journal);
			err2 = jbd2_journal_flush(EXT4_SB(sb)->s_journal);
			jbd2_journal_unlock_updates(EXT4_SB(sb)->s_journal);
		}
		if (err == 0)
			err = err2;
		mnt_drop_write(filp->f_path.mnt);

		return err;
	}

#ifdef CONFIG_EXT4_FS_SPLIT_FILE
	case EXT4_IOC_SPLIT_FILE: {
		PVR_INFO pe;
		int err;
		struct kstat stat;
		void __user *args = (void __user *)arg;
		mm_segment_t old_fs = get_fs();


		if (!EXT4_HAS_INCOMPAT_FEATURE(inode->i_sb,
					EXT4_FEATURE_INCOMPAT_EXTENTS))
			return -ENOTSUPP;

		if (!(filp->f_mode & FMODE_READ) ||
		    !(filp->f_mode & FMODE_WRITE))
			return -EBADF;

		if (copy_from_user(&pe,
			args, sizeof(PVR_INFO)))
			return -EFAULT;


		if (do_mod(pe.offset, inode->i_sb->s_blocksize)) {
			printk(KERN_ERR "unaligned offset %llu\n", pe.offset);
			return -EINVAL;
		}

		if (atomic_read(&(inode->i_mutex.count)) == 0) {
			printk(KERN_ERR "File is already being used. Try Again!!\n");
			return -EAGAIN;
		}

		mutex_lock(&inode->i_mutex);

		set_fs(get_ds());
		err = vfs_lstat(pe.filename, &stat);
		set_fs(old_fs);

		if (!err) {
			mutex_unlock(&inode->i_mutex);
			return -EEXIST;
		}

		err = mnt_want_write(filp->f_path.mnt);
		if (err)
			goto split_out;

		err = ext4_split_file(filp, pe.filename, pe.offset);
		mnt_drop_write(filp->f_path.mnt);

split_out:
		mutex_unlock(&inode->i_mutex);
		return err;
	}
#endif /* CONFIG_EXT4_FS_SPLIT_FILE */

#ifdef CONFIG_EXT4_FS_MERGE_FILE
	case EXT4_IOC_MERGE_FILE: {
		PVR_INFO pe;
		int err;
		struct kstat stat;
		void __user *args = (void __user *)arg;
		mm_segment_t old_fs = get_fs();


		if (!EXT4_HAS_INCOMPAT_FEATURE(inode->i_sb,
					EXT4_FEATURE_INCOMPAT_EXTENTS))
			return -ENOTSUPP;

		if (!(filp->f_mode & FMODE_READ) ||
		    !(filp->f_mode & FMODE_WRITE))
			return -EBADF;

		if (copy_from_user(&pe,
			args, sizeof(PVR_INFO)))
			return -EFAULT;

		if (do_mod(inode->i_size, inode->i_sb->s_blocksize)) {
			printk(KERN_ERR "block unaligned file for merge\n");
			return -EINVAL;
		}

		if (atomic_read(&(inode->i_mutex.count)) == 0) {
			printk(KERN_ERR "File is already being used. Try Again!!\n");
			return -EAGAIN;
		}

		mutex_lock(&inode->i_mutex);

		set_fs(get_ds());
		err = vfs_lstat(pe.filename, &stat);
		set_fs(old_fs);

		if (err) {
			mutex_unlock(&inode->i_mutex);
			return -ENOENT;
		}

		err = mnt_want_write(filp->f_path.mnt);
		if (err)
			goto merge_out;

		err = ext4_merge_file(filp, pe.filename);
		mnt_drop_write(filp->f_path.mnt);

merge_out:
		mutex_unlock(&inode->i_mutex);
		return err;
	}
#endif /* CONFIG_EXT4_FS_MERGE_FILE */
	case EXT4_IOC_MOVE_EXT: {
		struct move_extent me;
		struct file *donor_filp;
		int err;

		if (!(filp->f_mode & FMODE_READ) ||
		    !(filp->f_mode & FMODE_WRITE))
			return -EBADF;

		if (copy_from_user(&me,
			(struct move_extent __user *)arg, sizeof(me)))
			return -EFAULT;
		me.moved_len = 0;

		donor_filp = fget(me.donor_fd);
		if (!donor_filp)
			return -EBADF;

		if (!(donor_filp->f_mode & FMODE_WRITE)) {
			err = -EBADF;
			goto mext_out;
		}

		err = mnt_want_write(filp->f_path.mnt);
		if (err)
			goto mext_out;

		err = ext4_move_extents(filp, donor_filp, me.orig_start,
					me.donor_start, me.len, &me.moved_len);
		mnt_drop_write(filp->f_path.mnt);
		if (me.moved_len > 0)
			file_remove_suid(donor_filp);

		if (copy_to_user((struct move_extent __user *)arg,
				 &me, sizeof(me)))
			err = -EFAULT;
mext_out:
		fput(donor_filp);
		return err;
	}

	case EXT4_IOC_GROUP_ADD: {
		struct ext4_new_group_data input;
		struct super_block *sb = inode->i_sb;
		int err, err2=0;

		if (!capable(CAP_SYS_RESOURCE))
			return -EPERM;

		if (copy_from_user(&input, (struct ext4_new_group_input __user *)arg,
				sizeof(input)))
			return -EFAULT;

		err = mnt_want_write(filp->f_path.mnt);
		if (err)
			return err;

		err = ext4_group_add(sb, &input);
		if (EXT4_SB(sb)->s_journal) {
			jbd2_journal_lock_updates(EXT4_SB(sb)->s_journal);
			err2 = jbd2_journal_flush(EXT4_SB(sb)->s_journal);
			jbd2_journal_unlock_updates(EXT4_SB(sb)->s_journal);
		}
		if (err == 0)
			err = err2;
		mnt_drop_write(filp->f_path.mnt);

		return err;
	}

	case EXT4_IOC_MIGRATE:
	{
		int err;
		if (!inode_owner_or_capable(inode))
			return -EACCES;

		err = mnt_want_write(filp->f_path.mnt);
		if (err)
			return err;
		/*
		 * inode_mutex prevent write and truncate on the file.
		 * Read still goes through. We take i_data_sem in
		 * ext4_ext_swap_inode_data before we switch the
		 * inode format to prevent read.
		 */
		mutex_lock(&(inode->i_mutex));
		err = ext4_ext_migrate(inode);
		mutex_unlock(&(inode->i_mutex));
		mnt_drop_write(filp->f_path.mnt);
		return err;
	}

	case EXT4_IOC_ALLOC_DA_BLKS:
	{
		int err;
		if (!inode_owner_or_capable(inode))
			return -EACCES;

		err = mnt_want_write(filp->f_path.mnt);
		if (err)
			return err;
		err = ext4_alloc_da_blocks(inode);
		mnt_drop_write(filp->f_path.mnt);
		return err;
	}

	case FITRIM:
	{
		struct super_block *sb = inode->i_sb;
		struct request_queue *q = bdev_get_queue(sb->s_bdev);
		struct fstrim_range range;
		int ret = 0;

		if (!capable(CAP_SYS_ADMIN))
			return -EPERM;

		if (!blk_queue_discard(q))
			return -EOPNOTSUPP;

		if (copy_from_user(&range, (struct fstrim_range *)arg,
		    sizeof(range)))
			return -EFAULT;

		range.minlen = max((unsigned int)range.minlen,
				   q->limits.discard_granularity);
		ret = ext4_trim_fs(sb, &range);
		if (ret < 0)
			return ret;

		if (copy_to_user((struct fstrim_range *)arg, &range,
		    sizeof(range)))
			return -EFAULT;

		return 0;
	}
#ifdef CONFIG_EXT4_FS_TRUNCATE_RANGE
	case EXT4_IOC_TRUNCATE_RANGE:
	{
		PVR_INFO tr;
		struct mutex *lock = &inode->i_mutex;
		int error = 0;
		if (copy_from_user(&tr, (const void *) arg, sizeof(PVR_INFO)))
			return -EFAULT;

		if(tr.offset < 0 || tr.end < 0)
			return -EINVAL;

		if (do_mod(tr.offset, inode->i_sb->s_blocksize) ||
			do_mod(tr.end, inode->i_sb->s_blocksize))
			return -EINVAL;

		if ((tr.offset >= tr.end) || (tr.offset >= inode->i_size))
			return -EINVAL;

		if (!(filp->f_mode & FMODE_WRITE))
			return -EBADF;

		if (tr.end > inode->i_size)
			tr.end = inode->i_size;

		if (!ext4_test_inode_flag(inode, EXT4_INODE_EXTENTS))
			return -ENOTSUPP;

		if (atomic_read(&lock->count) == 0) {
			printk(KERN_ERR "Truncate Range is already being used. Try Again!!\n");
			return -EAGAIN;
		}
		mutex_lock(&inode->i_mutex);
		error = ext4_ext_truncate_range(filp, tr.offset, tr.end);
		mutex_unlock(&inode->i_mutex);
		return error;
	}
	case EXT4_IOC_TRUNCATE_ARRAY_RANGE:
	{
		int error, count = 0;
		trange_array_t tarray;
		trange_t *ptr;
		struct mutex *lock = &inode->i_mutex;

		if (copy_from_user(&tarray, (const void *) arg,
								sizeof(trange_array_t)))
			return -EFAULT;

		ptr = tarray.trange;

		tarray.trange = vmalloc(tarray.elements * sizeof(trange_t));
		if (!tarray.trange)
			return -ENOMEM;

		if (copy_from_user((tarray.trange), (const void *) (ptr),
							(tarray.elements) * sizeof(trange_t))) {
			error = -EFAULT;
			goto out;
		}

		/* Check for validity of truncate range offsets provided */
		for (count = 0; count < tarray.elements ; count++) {
			ptr = tarray.trange + count;
			if (do_mod(ptr->start_off, inode->i_sb->s_blocksize) ||
				do_mod(ptr->end_off,inode->i_sb->s_blocksize)) {
					error = -EINVAL;
					goto out;
				}
			if ((ptr->start_off >= ptr->end_off) ||
				(ptr->start_off >= inode->i_size)) {
					error = -EINVAL;
					goto out;
				}
			if (ptr->end_off > inode->i_size)
				ptr->end_off = inode->i_size;
		}
		
		/* Sort the truncate range offsets in descending orders */
		sort(tarray.trange, tarray.elements, sizeof(trange_t),
						ext4_cmp_offsets, NULL);

		for (count = 0; count < tarray.elements ; count++) {
			ptr = tarray.trange + count;
			if (atomic_read(&lock->count) == 0) {
				printk(KERN_ERR "Truncate Range is already"
								"being used. Try Again!!\n");
				error = -EAGAIN;
				goto out;
			}
			mutex_lock(&inode->i_mutex);
			error = ext4_ext_truncate_range(filp, ptr->start_off,
												  ptr->end_off);
			mutex_unlock(&inode->i_mutex);
			if (error)
				goto out;
		}
out:
	vfree(tarray.trange);
	return error;
}

#endif

	default:
		return -ENOTTY;
	}
}

#ifdef CONFIG_COMPAT
long ext4_compat_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	/* These are just misnamed, they actually get/put from/to user an int */
	switch (cmd) {
	case EXT4_IOC32_GETFLAGS:
		cmd = EXT4_IOC_GETFLAGS;
		break;
	case EXT4_IOC32_SETFLAGS:
		cmd = EXT4_IOC_SETFLAGS;
		break;
	case EXT4_IOC32_GETVERSION:
		cmd = EXT4_IOC_GETVERSION;
		break;
	case EXT4_IOC32_SETVERSION:
		cmd = EXT4_IOC_SETVERSION;
		break;
	case EXT4_IOC32_GROUP_EXTEND:
		cmd = EXT4_IOC_GROUP_EXTEND;
		break;
	case EXT4_IOC32_GETVERSION_OLD:
		cmd = EXT4_IOC_GETVERSION_OLD;
		break;
	case EXT4_IOC32_SETVERSION_OLD:
		cmd = EXT4_IOC_SETVERSION_OLD;
		break;
#ifdef CONFIG_JBD2_DEBUG
	case EXT4_IOC32_WAIT_FOR_READONLY:
		cmd = EXT4_IOC_WAIT_FOR_READONLY;
		break;
#endif
	case EXT4_IOC32_GETRSVSZ:
		cmd = EXT4_IOC_GETRSVSZ;
		break;
	case EXT4_IOC32_SETRSVSZ:
		cmd = EXT4_IOC_SETRSVSZ;
		break;
	case EXT4_IOC32_GROUP_ADD: {
		struct compat_ext4_new_group_input __user *uinput;
		struct ext4_new_group_input input;
		mm_segment_t old_fs;
		int err;

		uinput = compat_ptr(arg);
		err = get_user(input.group, &uinput->group);
		err |= get_user(input.block_bitmap, &uinput->block_bitmap);
		err |= get_user(input.inode_bitmap, &uinput->inode_bitmap);
		err |= get_user(input.inode_table, &uinput->inode_table);
		err |= get_user(input.blocks_count, &uinput->blocks_count);
		err |= get_user(input.reserved_blocks,
				&uinput->reserved_blocks);
		if (err)
			return -EFAULT;
		old_fs = get_fs();
		set_fs(KERNEL_DS);
		err = ext4_ioctl(file, EXT4_IOC_GROUP_ADD,
				 (unsigned long) &input);
		set_fs(old_fs);
		return err;
	}
	case EXT4_IOC_MOVE_EXT:
	case FITRIM:
		break;
	default:
		return -ENOIOCTLCMD;
	}
	return ext4_ioctl(file, cmd, (unsigned long) compat_ptr(arg));
}
#endif
