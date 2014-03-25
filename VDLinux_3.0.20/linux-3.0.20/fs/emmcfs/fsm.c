/**
 * @file	fs/emmcfs/fsm.c
 * @brief	The eMMCFS free space management (FSM) implementation.
 * @author	Igor Skalkin, i.skalkin@samsung.com
 * @author	Ivan Arishchenko, i.arishchenk@samsung.com
 * @date	30/03/2012
 *
 * eMMCFS -- Samsung eMMC chip oriented File System, Version 1.
 *
 * In this file free space management (FSM) is implemented.
 *
 * @see		TODO: documents
 *
 * Copyright 2012 by Samsung Electronics, Inc.
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

#include <linux/pagemap.h>
#include <linux/vmalloc.h>

#include "emmcfs.h"

/**
 * @brief				Get free space block chunk from tree
 *					and update free space manager bitmap.
 * @param [in] 	inode_info		The inode information structure.
 * @param [in] 	block_offset		Desired physical number of first block
 *					of block chunk.
 * @param [in] 	length_in_blocks	Blocks count.
 * @return				Returns physical number of first block
 *					of block chunk (may differs from
 *					block_offset parameter if desired block
 *					already used by file system), 0 if
 *					function fails (no free space).
 */
__u64 emmcfs_fsm_get_free_block(struct emmcfs_inode_info *inode_info,
	__u64 block_offset, __u32 length_in_blocks)
{
	struct emmcfs_sb_info *sbi = EMMCFS_SB(inode_info->vfs_inode.i_sb);
	struct emmcfs_fsm_info *fsm = sbi->fsm_info;

	EMMCFS_LOG_FUNCTION_START(fsm->sbi);

	if ((block_offset + length_in_blocks) >=
		(sbi->total_leb_count << sbi->log_blocks_in_leb)) {
		EMMCFS_LOG_FUNCTION_END(fsm->sbi, 0);
		return 0;
	}

	mutex_lock(&fsm->lock);
	if (!FSM_PREALLOC_DELTA || !S_ISREG(inode_info->vfs_inode.i_mode) ||
		(inode_info->vfs_inode.i_ino < EMMCFS_1ST_FILE_INO))
		block_offset = fsm_btree_lookup(&fsm->tree,
			block_offset, length_in_blocks);
	else if ((length_in_blocks > 1) || /* sparse file, drop preallocation */
		(inode_info->fork.prealloc_start_block &&
		(block_offset != inode_info->fork.prealloc_start_block))) {
		if (inode_info->fork.prealloc_start_block &&
		inode_info->fork.prealloc_block_count) {
			fsm_btree_free(&fsm->tree,
			inode_info->fork.prealloc_start_block,
			inode_info->fork.prealloc_block_count);
		}
		inode_info->fork.prealloc_block_count = 0;
		inode_info->fork.prealloc_start_block = 0;
		block_offset = fsm_btree_lookup(&fsm->tree,
			block_offset, length_in_blocks);
	} else if (!inode_info->fork.prealloc_start_block ||
		!inode_info->fork.prealloc_block_count) {
		block_offset = fsm_btree_lookup(&fsm->tree, block_offset,
			1 + FSM_PREALLOC_DELTA);
		if (block_offset) {
			inode_info->fork.prealloc_block_count =
				FSM_PREALLOC_DELTA;
			inode_info->fork.prealloc_start_block =
				block_offset + 1;
		} else {
			inode_info->fork.prealloc_block_count = 0;
			inode_info->fork.prealloc_start_block = 0;
			block_offset =
				fsm_btree_lookup(&fsm->tree, block_offset, 1);
		}
	} else {
		block_offset = inode_info->fork.prealloc_start_block++;
		inode_info->fork.prealloc_block_count--;
	}
	if (block_offset) {
		__u64	start_page = block_offset >> (PAGE_SHIFT + 3),
			end_page = (block_offset + length_in_blocks) >>
			(PAGE_SHIFT + 3), index;

		EMMCFS_BUG_ON((start_page >= fsm->page_count) ||
			(end_page >= fsm->page_count));

		for (index = start_page; index <= end_page; index++)
			EMMCFS_BUG_ON(EMMCFS_ADD_CHUNK(fsm->leb_bitmap_inode,
				&fsm->pages[index], 1));

		fsm->sbi->free_blocks_count -= length_in_blocks;
		for (index = 0; index < length_in_blocks; index++) {
			__u64 x = block_offset + index;
			EMMCFS_BUG_ON(x >= (fsm->length_in_bytes<<3));
			EMMCFS_BUG_ON(test_and_set_bit(x, (void *)fsm->data));
		}
		for (index = start_page; index <= end_page; index++)
			set_page_dirty_lock(fsm->pages[index]);
	}
	mutex_unlock(&fsm->lock);

	EMMCFS_LOG_FUNCTION_END(fsm->sbi, 0);
	return block_offset;
}

/**
 * @brief				Free block chunk (put free space chunk
 *					to tree and update free space manager
 *					bitmap.
 * @param [in] 	fsm			FSM information structure.
 * @param [in] 	block_offset		Physical number of first block of
 *					block chunk.
 * @param [in] 	length_in_blocks	Blocks count.
 * @param [in] 	inode_info		The inode information structure.
 * @return	void
 */
static int fsm_free_block_chunk(struct emmcfs_fsm_info *fsm,
	__u64 block_offset, __u32 length_in_blocks,
	struct emmcfs_inode_info *inode_info)
{
	int err = 0;
	__u32 i;
	__u64 start_page = block_offset >> (PAGE_SHIFT + 3),
		end_page = (block_offset + length_in_blocks) >>
			(PAGE_SHIFT + 3), page_index;


	EMMCFS_BUG_ON((start_page >= fsm->page_count) ||
	       (end_page >= fsm->page_count));

	for (page_index = start_page; page_index <= end_page; page_index++) {
		err = EMMCFS_ADD_CHUNK(fsm->leb_bitmap_inode,
			&fsm->pages[page_index], 1);

		if (err) {
			if (!is_sbi_flag_set(fsm->sbi,
						IS_MOUNT_FINISHED))
				return err;
			else
				EMMCFS_BUG();
		}
	}

	mutex_lock(&fsm->lock);
	if (inode_info) {
		if (inode_info->fork.prealloc_start_block &&
			inode_info->fork.prealloc_block_count)
			fsm_btree_free(&fsm->tree,
				inode_info->fork.prealloc_start_block,
				inode_info->fork.prealloc_block_count);
		inode_info->fork.prealloc_block_count = 0;
		inode_info->fork.prealloc_start_block = 0;
	}
	fsm_btree_free(&fsm->tree, block_offset, length_in_blocks);
	fsm->sbi->free_blocks_count += length_in_blocks;
	for (i = 0; i < length_in_blocks; i++) {
		__u32 index = block_offset + i;
		if (index >= (fsm->length_in_bytes<<3)) {
			if (!is_sbi_flag_set(fsm->sbi, IS_MOUNT_FINISHED))
				return -EFAULT;
			else
				EMMCFS_BUG();
		}
		if (!test_and_clear_bit(index, (void *)fsm->data)) {
			if (!is_sbi_flag_set(fsm->sbi, IS_MOUNT_FINISHED))
				return -EFAULT;
			else
				EMMCFS_BUG();
		}
	}
	mutex_unlock(&fsm->lock);
	for (page_index = start_page; page_index <= end_page; page_index++)
		set_page_dirty_lock(fsm->pages[page_index]);

	return 0;
}

/**
 * @brief				The fsm_free_block_chunk wrapper. This
 *					function is called during truncate or
 *					unlink inode processes.
 * @param [in] 	sbi			Superblock information structure.
 * @param [in] 	block_offset		Physical number of first block of
 *					inserted chunk
 * @param [in] 	length_in_blocks	Inserted blocks count.
 * @return	void
 */
int emmcfs_fsm_put_free_block(struct emmcfs_inode_info *inode_info,
	__u64 block_offset, __u32 length_in_blocks)
{
	return fsm_free_block_chunk(EMMCFS_SB(inode_info->vfs_inode.i_sb)->fsm_info,
		block_offset, length_in_blocks, inode_info);
}

/**
 * @brief		The fsm_free_block_chunk wrapper. This function is
 *			called during evict inode or process orphan inodes
 *			processes (inode already doesn't exist, but exttree
 *			inode blocks still exist).
 * @param [in] 	sbi	Superblock information structure.
 * @param [in] 	offset	Physical number of first block of inserted chunk
 * @param [in] 	length	Inserted blocks count.
 * @return	void
 */
int emmcfs_fsm_free_exttree_extent(struct emmcfs_sb_info *sbi,
	__u64 offset, __u32 length)
{
	return fsm_free_block_chunk(sbi->fsm_info, offset, length, NULL);
}

/**
 * @brief			Puts back to tree preallocated blocks.
 * @param [in] 	inode_info	The inode information structure.
 * @return			Returns 0 on success, -1 on failure.
 */
void emmcfs_fsm_discard_preallocation(struct emmcfs_inode_info *inode_info)
{
	struct emmcfs_sb_info *sbi = inode_info->vfs_inode.i_sb->s_fs_info;
	struct emmcfs_fork_info *fork = &inode_info->fork;

	mutex_lock(&sbi->fsm_info->lock);
	if (fork->prealloc_start_block && fork->prealloc_block_count) {
		fsm_btree_free(&sbi->fsm_info->tree, fork->prealloc_start_block,
			fork->prealloc_block_count);
		fork->prealloc_block_count = 0;
		fork->prealloc_start_block = 0;
	}
	mutex_unlock(&sbi->fsm_info->lock);
}

/**
 * @brief			The fsm_btree_insert wrapper. It inserts
 *				leaf to FSM tree.
 * @param [in] 	fsm		Free space manager information structure.
 * @param [in] 	block_offset	Physical number of first block of inserted
 *				chunk.
 * @param [in] 	length		Inserted blocks count.
 * @return			Returns 0 on success, -1 on failure.
 */
static int fsm_insert(struct emmcfs_fsm_info *fsm,
			__u64 block_offset, __u64 length)
{
	struct  emmcfs_fsm_btree *btree = &fsm->tree;
	struct fsm_btree_key key;
	EMMCFS_LOG_FUNCTION_START(fsm->sbi);
	key.block_offset = block_offset;
	key.length = length;
	EMMCFS_DEBUG_FSM("INSERT off %llu\tlen %llu ",
			 ((struct fsm_btree_key *)&key)->block_offset,
			 ((struct fsm_btree_key *)&key)->length);
	if (fsm_btree_insert(btree, &key) != 0) {
		EMMCFS_LOG_FUNCTION_END(fsm->sbi, -1);
		return -1;
	}

	fsm->sbi->free_blocks_count += length;
	EMMCFS_LOG_FUNCTION_END(fsm->sbi, 0);
	return 0;
}

/**
 * @brief			Build free space management tree from
 *				on-disk bitmap.
 * @param [in,out] 	fsm	Free space manager information structure.
 * @return			Returns error code.
 */
static int fsm_get_tree(struct emmcfs_fsm_info *fsm)
{
	int ret = 0;
	__u32 off = 0;
	int mode = FIND_CLEAR_BIT;
	__u64 i = 0, free_start = 0, free_end = 0;
	__u32 size = fsm->length_in_bytes >> 2;
	__u32 rest_size = fsm->length_in_bytes - (size << 2);
	EMMCFS_LOG_FUNCTION_START(fsm->sbi);

	fsm->sbi->free_blocks_count = 0;

	for (; i < size; i++)
		for (off = 0; off < 32; off++) {
			if ((mode == FIND_CLEAR_BIT) && !off) {
				if (fsm->data[i] == 0xFFFFFFFF)
					break;
				off = ffz(fsm->data[i]);
				free_start = (i << 5) + off;
				free_end = free_start;
				mode = FIND_SET_BIT;
				continue;
			}
			if ((mode == FIND_CLEAR_BIT) && off) {
				if (!(fsm->data[i] & (1 << off))) {
					free_start = (i << 5) + off;
					free_end = free_start;
					mode = FIND_SET_BIT;
					continue;
				}
			}
			if ((mode == FIND_SET_BIT) && !off) {
				if (fsm->data[i] == 0)
					break;
				off = ffs(fsm->data[i]) - 1 ;
				free_end = (i << 5) + off ;
				mode = FIND_CLEAR_BIT;
				ret = fsm_insert(fsm, free_start,
					   free_end - free_start);
				if (ret) {
					EMMCFS_LOG_FUNCTION_END(fsm->sbi, ret);
					return ret;
				}
				continue;
			}
			if ((mode == FIND_SET_BIT) && off) {
				if ((fsm->data[i] & (1 << off))) {
					free_end = (i << 5) + off;
					mode = FIND_CLEAR_BIT;
					fsm_insert(fsm, free_start,
						   free_end - free_start);
					continue;
				}
			}
		}
	if (rest_size)
		for (off = 0; off < rest_size * 8; off++) {
			if(mode == FIND_SET_BIT) {
				if ((fsm->data[i] & (1 << off))) {
					free_end = (i << 5) + off;
					mode = FIND_CLEAR_BIT;
					fsm_insert(fsm, free_start,
							free_end - free_start);
					continue;
				}
			}
			if(mode == FIND_CLEAR_BIT) {
				if (!(fsm->data[i] & (1 << off))) {
					free_start = (i << 5) + off;
					free_end = free_start;
					mode = FIND_SET_BIT;
					continue;
				}
			}
		}

	if (mode == FIND_SET_BIT)
		fsm_insert(fsm, free_start, ((i << 5)+ rest_size * 8)
				- free_start);
	EMMCFS_LOG_FUNCTION_END(fsm->sbi, ret);
	return ret;
}

/**
 * @brief			Build free space management.
 * @param [in,out] 	sb	The VFS superblock
 * @return			Returns 0 on success, -errno on failure.
 */
int emmcfs_fsm_build_management(struct super_block *sb)
{
	struct emmcfs_fsm_info *fsm;
	struct emmcfs_sb_info *sbi = sb->s_fs_info;
	int err = 0;
	int page_index = 0;

	EMMCFS_LOG_FUNCTION_START(sbi);
	fsm = kzalloc(sizeof(struct emmcfs_fsm_info), GFP_KERNEL);
	if (!fsm) {
		EMMCFS_LOG_FUNCTION_END(sbi, -ENOMEM);
		return -ENOMEM;
	}
	fsm->page_count = ((sbi->lebs_bm_blocks_count<<sbi->block_size_shift) +
		(1 << PAGE_SHIFT) - 1) >> PAGE_SHIFT;

	sbi->fsm_info = fsm;
	fsm->sbi = sbi;

	fsm->free_space_start = le64_to_cpu(EMMCFS_RAw_EXSB(sbi)->
			volume_body.begin);
	mutex_init(&fsm->lock);

	fsm->pages = kzalloc(sizeof(*fsm->pages) * fsm->page_count, GFP_KERNEL);

	if (!fsm->pages) {
		err = -ENOMEM;
		goto fail;
	}
	for (page_index = 0; page_index < fsm->page_count ; page_index++)
		fsm->pages[page_index] = ERR_PTR(-EIO);

	fsm->leb_bitmap_inode =
		emmcfs_iget(sb, EMMCFS_LEB_BITMAP_INO, NULL);

	if (IS_ERR(fsm->leb_bitmap_inode)) {
		err = PTR_ERR(fsm->leb_bitmap_inode);
		goto fail;
	}

	for (page_index = 0; page_index < fsm->page_count ; page_index++) {
		fsm->pages[page_index] =
			read_mapping_page(fsm->leb_bitmap_inode->i_mapping,
				page_index, NULL);
		if (IS_ERR(fsm->pages[page_index])) {
			err = (int)fsm->pages[page_index];
			goto fail;
		}
	}

	fsm->data = vmap(fsm->pages, fsm->page_count, VM_MAP, PAGE_KERNEL);

	fsm->length_in_bytes = (sbi->lebs_bm_blocks_count-1) * sbi->block_size +
		((sbi->lebs_bm_bits_in_last_block + 7) >> 3);
	emmcfs_init_fsm_btree(&fsm->tree);

	fsm_get_tree(fsm);

	EMMCFS_LOG_FUNCTION_END(sbi, 0);
	return 0;

fail:
	if (fsm->pages) {
		for (page_index = 0; page_index < fsm->page_count; page_index++)
			if (!IS_ERR(fsm->pages[page_index]))
				page_cache_release(fsm->pages[page_index]);
		kfree(fsm->pages);
	}
	if (fsm->leb_bitmap_inode)
		iput(fsm->leb_bitmap_inode);
	kfree(fsm);
	sbi->fsm_info = NULL;
	EMMCFS_LOG_FUNCTION_END(sbi, err);
	return err;
}

/**
 * @brief			Destroy free space management.
 * @param [in,out]	sb	The VFS superblock.
 * @return		void
 */
void emmcfs_fsm_destroy_management(struct super_block *sb)
{
	struct emmcfs_sb_info *sbi = sb->s_fs_info;
	struct emmcfs_fsm_info *fsm = sbi->fsm_info;
	int page_index = 0;

	EMMCFS_LOG_FUNCTION_START(sbi);
	EMMCFS_BUG_ON(!fsm->data || !fsm->pages);

	fsm_print_tree(&fsm->tree);

	vunmap((void *)fsm->data);
	for (page_index = 0; page_index < fsm->page_count; page_index++) {
		set_page_dirty_lock(fsm->pages[page_index]);
		page_cache_release(fsm->pages[page_index]);
	}

	iput(fsm->leb_bitmap_inode);

	fsm_btree_destroy(&fsm->tree);

	kfree(sbi->fsm_info->pages);
	kfree(sbi->fsm_info);
	sbi->fsm_info = NULL;
	EMMCFS_LOG_FUNCTION_END(sbi, 0);
}
