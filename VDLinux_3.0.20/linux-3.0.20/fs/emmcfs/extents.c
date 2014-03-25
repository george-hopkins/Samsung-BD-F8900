/**
 * @file	fs/emmcfs/extents.c
 * @brief	extents operations
 * @author
 *
 * eMMCFS -- Samsung eMMC chip oriented File System, Version 1.
 *
 * This file implements bnode operations and its related functions.
 *
 * Copyright 2011 by Samsung Electronics, Inc.,
 *
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

#include <linux/slab.h>
#include <linux/version.h>

#include "emmcfs.h"
#include "debug.h"

/** eMMCFS extents tree key cache.
 */
static struct kmem_cache *extents_tree_key_cachep;

/**
 * @brief		exttree key structure initializer.
 * @param [in,out] key	exttree key for initialization.
 * @return		void
 */
static void exttree_key_ctor(void *key)
{
	memset(key, 0, sizeof(struct emmcfs_exttree_key));
}

int emmcfs_exttree_cache_init(void)
{
	extents_tree_key_cachep = kmem_cache_create("emmcfs_exttree_key",
		sizeof(struct emmcfs_exttree_key), 0,
		SLAB_HWCACHE_ALIGN, exttree_key_ctor);
	if (!extents_tree_key_cachep) {
		EMMCFS_ERR("failed to initialize extents tree key cache\n");
		return -ENOMEM;
	}
	return 0;
}

void emmcfs_exttree_cache_destroy(void)
{
	kmem_cache_destroy(extents_tree_key_cachep);
}

struct emmcfs_exttree_key *emmcfs_get_exttree_key(void)
{
	struct emmcfs_exttree_key *key =
		kmem_cache_alloc(extents_tree_key_cachep, __GFP_WAIT);
	return key ? key : ERR_PTR(-ENOMEM);
}

void emmcfs_put_exttree_key(struct emmcfs_exttree_key *key)
{
	kmem_cache_free(extents_tree_key_cachep, key);
}

static inline int cmp_2_le64(__le64 a, __le64 b)
{
	if (le64_to_cpu(a) == le64_to_cpu(b))
		return 0;
	else
		return (le64_to_cpu(a) > le64_to_cpu(b)) ? 1 : -1;
}

/**
 * @brief		Extents tree key compare function.
 * @param [in]	__key1	first key to compare.
 * @param [in]	__key2	second key to compare.
 * @return		-1 if key1 less than key2, 0 if equal, 1 in all other
 *			situations.
 */
int emmcfs_exttree_cmpfn(struct emmcfs_generic_key *__key1,
		struct emmcfs_generic_key *__key2)
{
	struct emmcfs_exttree_key *key1, *key2;

	key1 = container_of(__key1, struct emmcfs_exttree_key, gen_key);
	key2 = container_of(__key2, struct emmcfs_exttree_key, gen_key);

	if ((key1->object_id != key2->object_id) ||
			(le64_to_cpu(key2->iblock) == IBLOCK_DOES_NOT_MATTER))
		return cmp_2_le64(key1->object_id, key2->object_id);

	return cmp_2_le64 (key1->iblock, key2->iblock);
}

/**
 * @brief	Add newly allocated blocks chunk to extents overflow area.
 * @param [in] inode_info	inode information structure.
 * @param [in] iblock		starting logical block number of block chunk.
 * @param [in] cnt		block count.
 * @return	0 if success, or error number.
 */
int emmcfs_exttree_add(struct emmcfs_inode_info *inode_info, sector_t iblock,
		sector_t start_block, sector_t block_count)
{
	struct emmcfs_sb_info *sbi = inode_info->vfs_inode.i_sb->s_fs_info;
	struct emmcfs_exttree_record *record;
	int ret = 0;

	EMMCFS_LOG_FUNCTION_START(sbi);
	record = kzalloc(sizeof(*record), GFP_KERNEL);
	if (!record) {
		EMMCFS_LOG_FUNCTION_END(sbi, -ENOMEM);
		return -ENOMEM;
	}

	/* Fill the key */
	memcpy(record->key.gen_key.magic, EMMCFS_EXTTREE_KEY_MAGIC,
			sizeof(EMMCFS_EXTTREE_KEY_MAGIC) - 1);
	record->key.gen_key.key_len = cpu_to_le32(sizeof(record->key));
	record->key.gen_key.record_len = cpu_to_le32(sizeof(*record));
	record->key.object_id = cpu_to_le64(inode_info->vfs_inode.i_ino);
	record->key.iblock = cpu_to_le64(iblock);

	/* Fill the extent */
	record->lextent.begin = cpu_to_le64(start_block);
	record->lextent.length = cpu_to_le32(block_count);

	ret = emmcfs_btree_insert(sbi->extents_tree, record);

	EMMCFS_DEBUG_INO("exit with __FUNCTION__ %d", ret);
	kfree(record);
	EMMCFS_LOG_FUNCTION_END(sbi, ret);
	return ret;
}

/**
 * @brief		Helper function for logical to physical block numbers
 *			translation - find bnode which contains exttree record
 *			corresponded to iblock in extents overflow tree.
 * @param [in] inode_info	inode information structure.
 * @param [in] iblock		Logical block number to find.
 * @param [out] pos		resulting position of the record in the bnode.
 * @return		resulting bnode.
 */
struct emmcfs_bnode *emmcfs_extent_find(struct emmcfs_inode_info *inode_info,
		sector_t iblock, int *pos, enum emmcfs_get_bnode_mode mode)
{
	struct emmcfs_sb_info *sbi = inode_info->vfs_inode.i_sb->s_fs_info;
	struct emmcfs_exttree_key *key;
	struct emmcfs_bnode *bnode;

	EMMCFS_LOG_FUNCTION_START(sbi);
	key = emmcfs_get_exttree_key();
	if (IS_ERR(key)) {
		EMMCFS_LOG_FUNCTION_END(sbi, -ENOMEM);
		return ERR_PTR(-ENOMEM);
	}

	key->object_id = cpu_to_le64(inode_info->vfs_inode.i_ino);
	key->iblock = cpu_to_le64(iblock);

	bnode = emmcfs_btree_find(sbi->extents_tree, &key->gen_key, pos, mode);
	emmcfs_put_exttree_key(key);
	EMMCFS_LOG_FUNCTION_END(sbi, PTR_ERR(bnode));
	return bnode;
}

/**
 * @brief		Logical to physical block numbers translation for blocks
 * 			which placed in extents overflow.
 * @param [in] inode_info	inode information structure.
 * @param [in] iblock		Logical block number to translate.
 * @param [in] max_blocks	Count of sequentially allocated blocks from
 * 				iblock to end of extent. (This feature is used
 * 				VFS for minimize get_block calls)
 * @return		Physical block number corresponded to logical iblock.
 */
sector_t emmcfs_exttree_get_block(struct emmcfs_inode_info *inode_info,
				  sector_t iblock, __u32 *max_blocks)
{
	sector_t rc = 0;
	struct emmcfs_bnode *bnode;
	struct emmcfs_exttree_record *record;
	struct emmcfs_sb_info *sbi = EMMCFS_SB(inode_info->vfs_inode.i_sb);
	int pos = 0;
	__u64 object_id  = inode_info->vfs_inode.i_ino;

	EMMCFS_LOG_FUNCTION_START(sbi);

	EMMCFS_DEBUG_MUTEX("exttree mutex r lock");
	mutex_r_lock(sbi->extents_tree->rw_tree_lock);
	EMMCFS_DEBUG_MUTEX("exttree mutex r lock succ");

	bnode = emmcfs_extent_find(inode_info, iblock, &pos,
				   EMMCFS_BNODE_MODE_RO);
	if (IS_ERR(bnode))
		goto out;

	record = emmcfs_get_btree_record(bnode, pos);
	if (IS_ERR(record)) {
		rc = 0;
		goto out;
	}

	if (record->key.object_id != object_id)
		goto out;

	rc = le64_to_cpu(record->lextent.begin) + (iblock - record->key.iblock);
	if (max_blocks)
		*max_blocks =
			le32_to_cpu(record->lextent.length) -
			(iblock - record->key.iblock);
out:
	if (!IS_ERR(bnode))
		emmcfs_put_bnode(bnode);

	EMMCFS_DEBUG_MUTEX("exttree mutex r lock un");
	mutex_r_unlock(sbi->extents_tree->rw_tree_lock);
	EMMCFS_LOG_FUNCTION_END(sbi, rc);
	return rc;
}

/**
 * @brief		Expand file in extents overflow area.
 * @param [in] inode_info	inode information structure.
 * @param [in] iblock		starting logical block number of block chunk.
 * @param [in] cnt		block count.
 * @return		starting physical block number of the newly allocated
 * 			blocks chunk.
 */
sector_t emmcfs_exttree_add_block(struct emmcfs_inode_info *inode_info,
				  sector_t iblock, int cnt)
{
	struct emmcfs_bnode *bnode;
	struct emmcfs_exttree_record *record;
	int pos = 0;
	sector_t rc = 0;
	__u64 object_id  = inode_info->vfs_inode.i_ino;
	struct emmcfs_sb_info	*sbi = inode_info->vfs_inode.i_sb->s_fs_info;
	int err = 0;

	EMMCFS_DEBUG_MUTEX("exttree mutex w lock");
	mutex_w_lock(sbi->extents_tree->rw_tree_lock);
	EMMCFS_DEBUG_MUTEX("exttree mutex w lock succ");

	EMMCFS_LOG_FUNCTION_START(sbi);
	bnode = emmcfs_extent_find(inode_info, iblock, &pos,
			EMMCFS_BNODE_MODE_RW);

	if (IS_ERR(bnode)) {
		EMMCFS_DEBUG_MUTEX("exttree mutex w lock un");
		mutex_w_unlock(sbi->extents_tree->rw_tree_lock);
		EMMCFS_LOG_FUNCTION_END(sbi, 0);
		return 0;
	}

	record = emmcfs_get_btree_record(bnode, pos);

	if (IS_ERR(record)) {
		rc = 0;
		goto out_put_bnode;
	}

	if (record->key.object_id == object_id) {
		__u64 next_block_after = le64_to_cpu(record->lextent.begin) +
			le32_to_cpu(record->lextent.length);

		/* Try to allock near the previous extent */
		rc = emmcfs_fsm_get_free_block(inode_info,
			next_block_after, cnt);
		if (rc == 0)
			goto out_put_bnode;

		if (rc == next_block_after) {
			/* Put block into existing extent */
			record->lextent.length = cpu_to_le32(
				le32_to_cpu(record->lextent.length) + cnt);
			emmcfs_mark_bnode_dirty(bnode);
			goto out_put_bnode;
		} else {
			/* Unfortunately we can not allocate block near previous
			 * extent, so we have to create new one */
			emmcfs_put_bnode(bnode);
			err = emmcfs_exttree_add(inode_info, iblock, rc, cnt);
			if (err)
				rc = 0;
			goto out;
		}
	} else {
		/* Can not find nearest extent, create new one */
		emmcfs_put_bnode(bnode);
		rc = emmcfs_fsm_get_free_block(inode_info,
				sbi->fsm_info->free_space_start, cnt);
		if (rc == 0)
			goto out;
		err = emmcfs_exttree_add(inode_info, iblock, rc, cnt);
		if (err)
			rc = 0;
		goto out;
	}
out_put_bnode:
	emmcfs_put_bnode(bnode);
out:
	EMMCFS_DEBUG_MUTEX("exttree mutex w lock un");
	mutex_w_unlock(sbi->extents_tree->rw_tree_lock);
	EMMCFS_LOG_FUNCTION_END(sbi, rc);
	return rc;
}
#if LINUX_VERSION_CODE == KERNEL_VERSION(2,6,35)
long emmcfs_fallocate(struct inode *inode, int mode, loff_t offset, loff_t len)
{
#elif LINUX_VERSION_CODE == KERNEL_VERSION(3,0,20) || \
		LINUX_VERSION_CODE == KERNEL_VERSION(3,0,33)
long emmcfs_fallocate(struct file *file, int mode, loff_t offset, loff_t len)
{
	struct inode *inode = file->f_path.dentry->d_inode;
#endif
	if (!S_ISREG(inode->i_mode))
		return -ENODEV;
	return -EOPNOTSUPP; /*todo implementation*/
}

