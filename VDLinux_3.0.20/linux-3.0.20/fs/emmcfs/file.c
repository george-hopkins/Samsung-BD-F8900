/**
 * @file	fs/emmcfs/file.c
 * @brief	The eMMCFS file operations.
 * @author	TODO
 * @date	TODO
 *
 * eMMCFS -- Samsung eMMC chip oriented File System, Version 1.
 * TODO: Detailed description
 * @see		TODO: documents
 *
 * Copyright 2011 by Samsung Electronics, Inc.,
 *
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

#include <linux/version.h>
#include "emmcfs.h"

/**
 * @brief		This function is called during inode unlink VFS call.
 *			1) It clears inode internal fork extents.
 *			2) If extents tree extents for this inode exist,
 *			populates orphan inode list.
 *			Exttree extents and orphan inode record for this inode
 *			will be deleted later, during delete/evict inode
 *			VFS call.
 * @param [in]	inode	VFS inode pointer
 * @return		Returns 0 on success, not null error code on failure.
 */
int emmcfs_fsm_free_runtime_fork(struct inode *inode)
{
	int err = 0;
	struct emmcfs_fork_info *fork = &EMMCFS_I(inode)->fork;

	while (fork->used_extents && fork->total_block_count)
		if (fork->extents[--fork->used_extents].block_count) {
			__u64 first_block =
				fork->extents[fork->used_extents].first_block;
			__u32 length_in_blocks = 
				fork->extents[fork->used_extents].block_count;

			err = emmcfs_fsm_put_free_block(EMMCFS_I(inode),
					first_block, length_in_blocks);
			if (err) {
				EMMCFS_ERR("can not free extent "
					"first_block = %llu "
					"block_count = %u err = %d",
					first_block, length_in_blocks, err);
				return err;
			}

			fork->total_block_count -=
				fork->extents[fork->used_extents].block_count;
			fork->extents[fork->used_extents].first_block = 0;
			fork->extents[fork->used_extents].block_count = 0;
		}

	if (fork->total_block_count != 0) /* Extents tree for this file !empty*/
		return emmcfs_add_orphan_inode(inode);
	else
		return emmcfs_free_inode_n(EMMCFS_SB(inode->i_sb),
			inode->i_ino);
}

/**
 * @brief		This function is called during inode delete/evict VFS
 *			call. Inode internal fork extents have already been
 *			cleared, it's time to clear exttree extents for this
 *			inode and, finally, remove exttree orphan inode list
 *			record for this inode.
 * @param [in]	sbi	Superblock information structure pointer.
 * @param [in]	ino	The inode number.
 * @return		Returns 0 on success, not null error code on failure.
 */
int emmcfs_fsm_free_exttree(struct emmcfs_sb_info *sbi, __u64 ino)
{
#if defined(CONFIG_EMMCFS_DEBUG)
	int extents = 0;
#endif
	int rc = 0;
	struct emmcfs_exttree_key *key = emmcfs_get_exttree_key();
	if (IS_ERR(key)) {
		EMMCFS_LOG_FUNCTION_END(sbi, -ENOMEM);
		return -ENOMEM;
	}
	EMMCFS_DEBUG_TMP("clear_orphan_inode %llu", ino);

	while (true) {
		struct emmcfs_bnode *bnode;
		struct emmcfs_exttree_record *record;
		__u64 iblock = 0;
		int pos = 0;

		key->object_id = cpu_to_le64(ino);
		key->iblock = cpu_to_le64(IBLOCK_DOES_NOT_MATTER);

		EMMCFS_START_TRANSACTION(sbi);

		EMMCFS_DEBUG_MUTEX("exttree mutex w lock");
		mutex_w_lock(sbi->extents_tree->rw_tree_lock);
		EMMCFS_DEBUG_MUTEX("exttree mutex w lock succ");

		bnode = emmcfs_btree_find(sbi->extents_tree, &key->gen_key,
			&pos, EMMCFS_BNODE_MODE_RO);
		if (IS_ERR(bnode)) {
			rc = PTR_ERR(bnode);
			goto pass;
		}

		record = emmcfs_get_btree_record(bnode, pos);
		if (le64_to_cpu(record->key.object_id) != ino) {
			emmcfs_put_bnode(bnode);
			goto pass;
		} else {
			__u64 block_offset = le64_to_cpu(record->lextent.begin);
			__u32 length_in_blocks =
				le32_to_cpu(record->lextent.length);
			iblock = le64_to_cpu(record->key.iblock);
			emmcfs_put_bnode(bnode);

			key->object_id = cpu_to_le64(ino);
			key->iblock = cpu_to_le64(iblock);
			rc = emmcfs_btree_remove(sbi->extents_tree,
				(struct emmcfs_generic_key *)key);

			if (rc) {
				EMMCFS_ERR("Can not remove orphan ino %llu",
						ino);
				mutex_w_unlock(sbi->extents_tree->rw_tree_lock);
				break;
			}
			
			rc = emmcfs_fsm_free_exttree_extent(sbi, block_offset,
					length_in_blocks);
			if (rc) {
				EMMCFS_ERR("Can not free extnent while removing"
						" orphan ino %llu", ino);
				mutex_w_unlock(sbi->extents_tree->rw_tree_lock);
				break;
			}

#if defined(CONFIG_EMMCFS_DEBUG)
			++extents;
#endif
		}
		EMMCFS_DEBUG_MUTEX("exttree mutex w lock un");
		mutex_w_unlock(sbi->extents_tree->rw_tree_lock);
		EMMCFS_STOP_TRANSACTION(sbi);
	}
pass:
	EMMCFS_DEBUG_TMP("cleared %d extents", extents);
	key->object_id = cpu_to_le64(EMMCFS_ORPHAN_INODES_INO);
	key->iblock = cpu_to_le64(ino);

	if (!rc)
		rc = emmcfs_btree_remove(sbi->extents_tree,
				(struct emmcfs_generic_key *)key);

	EMMCFS_DEBUG_MUTEX("exttree mutex w lock un");
	mutex_w_unlock(sbi->extents_tree->rw_tree_lock);

	if (!rc)
		rc = emmcfs_free_inode_n(sbi, ino);

	EMMCFS_STOP_TRANSACTION(sbi);

	emmcfs_put_exttree_key(key);
	return rc;
}

/**
 * @brief			This function called when a file is opened with
 *				O_TRUNC or truncated with truncate()/
 *				ftruncate() system calls and truncate file
 *				exttree extents according new file size in
 *				blocks.
 * @param [in]	inode_info	inode_info pointer.
 * @param [in]	new_size	new file size in blocks.
 * @return			Returns TODO
 */
static int truncate_exttree(struct emmcfs_inode_info *inode_info,
	u64 new_size)
{
	int rc = 0;
	struct emmcfs_fork_info *fork = &inode_info->fork;
	struct emmcfs_sb_info	*sbi = inode_info->vfs_inode.i_sb->s_fs_info;
	struct emmcfs_exttree_key *key = emmcfs_get_exttree_key();
	if (IS_ERR(key))
		return -ENOMEM;

	while (true) {
		__u64 object_id = inode_info->vfs_inode.i_ino;
		__u64 iblock = 0;
		__u64 ext_off = 0;
		__u32 ext_len = 0;
		struct emmcfs_bnode *bnode;
		struct emmcfs_exttree_record *record;
		int pos = 0;

		bnode = emmcfs_extent_find(inode_info, IBLOCK_MAX_NUMBER, &pos,
			EMMCFS_BNODE_MODE_RW);
		if (IS_ERR(bnode))
			goto pass;

		record = emmcfs_get_btree_record(bnode, pos);
		if (le64_to_cpu(record->key.object_id) != object_id) {
			emmcfs_put_bnode(bnode);
			goto pass;
		}

		iblock = le64_to_cpu(record->key.iblock);
		ext_off = le64_to_cpu(record->lextent.begin);
		ext_len = le32_to_cpu(record->lextent.length);

		if (iblock >= new_size) {
			rc = emmcfs_fsm_put_free_block(inode_info,
					ext_off, ext_len);
			if (rc) {
				EMMCFS_ERR("can not free extent while "
						"truncating exttree "
						"ext_off = %llu "
						"ext_len = %u, err = %d",
						ext_off, ext_len, rc);
				emmcfs_put_bnode(bnode);
				goto pass;
			}

			fork->total_block_count -= ext_len;
			emmcfs_put_bnode(bnode);
			key->object_id = cpu_to_le64(object_id);
			key->iblock = cpu_to_le64(iblock);
			emmcfs_btree_remove(sbi->extents_tree,
				(struct emmcfs_generic_key *)key);
			continue;
		} else if ((iblock + ext_len) > new_size) {
			__u64 delta = iblock + ext_len - new_size;
			rc = emmcfs_fsm_put_free_block(inode_info,
				ext_off + ext_len - delta, delta);
			if (rc) {
				EMMCFS_ERR("can not free extent while "
						"truncating exttree "
						"ext_off = %llu "
						"ext_len = %u, err = %d",
						ext_off, ext_len, rc);
				emmcfs_put_bnode(bnode);
				goto pass;
			}

			record->lextent.length = cpu_to_le32(ext_len - delta);
			fork->total_block_count -= delta;
			emmcfs_mark_bnode_dirty(bnode);
			emmcfs_put_bnode(bnode);
			goto pass;
		} else {
			emmcfs_put_bnode(bnode);
			goto pass;
		}
	}
pass:
	emmcfs_put_exttree_key(key);
	return rc;
}

/**
 * @brief			This function is called when a file is opened
 *				with O_TRUNC or truncated with truncate()/
 *				ftruncate() system calls and truncate internal
 *				fork according new file size in blocks.
 * @param [in]	inode_info	The inode_info pointer.
 * @param [in]	new_size	New file size in blocks.
 * @return	void
 */
static int truncate_fork(struct emmcfs_inode_info *inode_info, loff_t new_size)
{
	struct emmcfs_fork_info *fork = &inode_info->fork;
	int i = fork->used_extents - 1;
	int err = 0;

	if (!fork->used_extents)
		return 0;

	for (; i >= 0; i--) {
		__u64 first_block =
			fork->extents[fork->used_extents - 1].first_block;
		__u32 length_in_blocks = 
			fork->extents[fork->used_extents - 1].block_count;
		if ((fork->total_block_count - new_size) >=
				fork->extents[i].block_count) {
			err = emmcfs_fsm_put_free_block(inode_info,
				first_block, length_in_blocks);
			if (err) {
				EMMCFS_ERR("can not free extent while "
					"truncating fork "
					"start = %llu "
					"ext_len = %u, err = %d",
					first_block, length_in_blocks, err);
				return err;
			}
			fork->total_block_count -= fork->extents[i].block_count;
			fork->extents[i].first_block = 0;
			fork->extents[i].block_count = 0;
			fork->used_extents--;
			if (fork->total_block_count <= new_size)
				break;
		} else {
			__u64 delta = fork->total_block_count - new_size;

			err = emmcfs_fsm_put_free_block(inode_info,
				first_block + length_in_blocks - delta, delta);
			if (err) {
				EMMCFS_ERR("can not free extent while "
					"truncating fork "
					"start = %llu "
					"ext_len = %u, err = %d",
					first_block, length_in_blocks, err);
				return err;
			}

			fork->total_block_count -= delta;
			fork->extents[i].block_count -= delta;
			break;
		}
	}

	return 0;
}

/**
 * @brief			This function is called when a file is opened
 *				with O_TRUNC or	truncated with truncate()/
 *				ftruncate() system calls.
 *				1) truncates exttree extents according to new
 *				file size.
 *				2) if fork intetnal extents contains more
 *				extents than new file size in blocks, internal
 *				fork also truncated.
 * @param [in]	inode		VFS inode pointer
 * @param [in]	new_size	New file size in bytes.
 * @return			Returns 0 on success, not null error code on
 *				failure.
 */
int emmcfs_truncate_blocks(struct inode *inode, loff_t new_size)
{
	unsigned int i;
	int rc = 0;
	__u64 internal_extents_bcnt;
	struct emmcfs_inode_info *inode_info = EMMCFS_I(inode);
	struct emmcfs_fork_info *fork = &inode_info->fork;
	struct emmcfs_sb_info	*sbi = inode->i_sb->s_fs_info;

	if (inode->i_ino < EMMCFS_1ST_FILE_INO)
		EMMCFS_BUG();
	if (!(S_ISREG(inode->i_mode) || S_ISLNK(inode->i_mode)))
		return -EPERM;

	new_size = (new_size + sbi->block_size - 1) >> (sbi->block_size_shift);
	EMMCFS_DEBUG_FSM("truncate ino %lu\told_size %d\tnew_size %llu",
		inode->i_ino, fork->total_block_count, new_size);

	if (fork->total_block_count <= new_size)
		goto pass;
	for (i = 0, internal_extents_bcnt = 0; i < fork->used_extents; i++)
		internal_extents_bcnt += fork->extents[i].block_count;
	if (internal_extents_bcnt < fork->total_block_count) {
		EMMCFS_DEBUG_MUTEX("exttree mutex w lock");
		mutex_w_lock(sbi->extents_tree->rw_tree_lock);
		EMMCFS_DEBUG_MUTEX("exttree mutex w lock succ");
		rc = truncate_exttree(inode_info, new_size);
		EMMCFS_DEBUG_MUTEX("exttree mutex w lock un");
		mutex_w_unlock(sbi->extents_tree->rw_tree_lock);
		if (rc)
			goto pass;
	}
	if (fork->total_block_count <= new_size)
		goto pass;

	if (!rc)
		rc = truncate_fork(inode_info, new_size);
pass:
	return rc;
}
