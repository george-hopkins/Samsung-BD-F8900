/**
 * @file	fs/emmcfs/orphan.c
 * @brief	The eMMCFS orphan inodes management.
 * @author	Igor Skalkin, i.skalkin@samsung.com
 * @date	24.07.2012
 *
 * eMMCFS -- Samsung eMMC chip oriented File System, Version 1.
 *
 * Orphan inodes management implementation.
 *
 * @see		TODO: documents
 *
 * Copyright 2012 by Samsung Electronics, Inc.,
 *e
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

#include "emmcfs.h"

/**
 * @brief		Add inode to orphan inodes store.
 * @param [in]	inode	The VFS inode pointer.
 * @return		Returns 0 on success, not null error code on failure.
 */
int emmcfs_add_orphan_inode(struct inode *inode)
{
	int rc;
	struct emmcfs_sb_info *sbi = inode->i_sb->s_fs_info;
	struct emmcfs_exttree_key *key = emmcfs_get_exttree_key();

	EMMCFS_LOG_FUNCTION_START(sbi);
	if (IS_ERR(key)) {
		EMMCFS_LOG_FUNCTION_END(sbi, -ENOMEM);
		return -ENOMEM;
	}
	/* Fill the key */
	memcpy(key->gen_key.magic, EMMCFS_EXTTREE_KEY_MAGIC,
		sizeof(EMMCFS_EXTTREE_KEY_MAGIC) - 1);
	key->gen_key.key_len = cpu_to_le32(sizeof(*key));
	key->gen_key.record_len = cpu_to_le32(sizeof(*key));
	key->object_id = cpu_to_le64(EMMCFS_ORPHAN_INODES_INO);
	key->iblock = cpu_to_le64(inode->i_ino);

	EMMCFS_DEBUG_TMP("ino: %lu total block count %u", inode->i_ino,
			EMMCFS_I(inode)->fork.total_block_count);

	EMMCFS_DEBUG_MUTEX("exttree mutex w lock");
	mutex_w_lock(sbi->extents_tree->rw_tree_lock);
	EMMCFS_DEBUG_MUTEX("exttree mutex w lock succ");
	rc = emmcfs_btree_insert(sbi->extents_tree, key);
	EMMCFS_DEBUG_MUTEX("exttree mutex w lock un");
	mutex_w_unlock(sbi->extents_tree->rw_tree_lock);
	EMMCFS_DEBUG_MUTEX("exttree mutex w unlock succ");

	emmcfs_put_exttree_key(key);
	EMMCFS_LOG_FUNCTION_END(sbi, rc);
	return rc;
}

/**
 * @brief		After mount orphan inodes processing.
 * @param [in]	sbi	Superblock information structure pointer.
 * @return		Returns 0 on success, not null error code on failure.
 */
int emmcfs_process_orphan_inodes(struct emmcfs_sb_info *sbi)
{
	struct emmcfs_bnode *bnode;
	struct emmcfs_exttree_record *record;
	struct emmcfs_exttree_key *key;
	__u64 orphan_ino_no;
	int pos = 0, rc = 0;

	EMMCFS_LOG_FUNCTION_START(sbi);
	key = emmcfs_get_exttree_key();
	if (IS_ERR(key)) {
		EMMCFS_LOG_FUNCTION_END(sbi, -ENOMEM);
		return -ENOMEM;
	}

	while (true) {
		key->object_id = cpu_to_le64(EMMCFS_ORPHAN_INODES_INO);
		key->iblock = cpu_to_le64(IBLOCK_DOES_NOT_MATTER);

		bnode = emmcfs_btree_find(sbi->extents_tree, &key->gen_key,
			&pos, EMMCFS_BNODE_MODE_RO);
		if (IS_ERR(bnode)) {
			rc = PTR_ERR(bnode);
			break;
		}

		record = emmcfs_get_btree_record(bnode, pos);
		if (le64_to_cpu(record->key.object_id) !=
			EMMCFS_ORPHAN_INODES_INO) {
			emmcfs_put_bnode(bnode);
			break;
		}

		orphan_ino_no = le64_to_cpu(record->key.iblock);
		emmcfs_put_bnode(bnode);

		rc = emmcfs_fsm_free_exttree(sbi, orphan_ino_no);
		if (rc)
			break;
	}
	emmcfs_put_exttree_key(key);
	EMMCFS_LOG_FUNCTION_END(sbi, rc);
	return rc;
}

