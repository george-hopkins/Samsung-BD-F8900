/**
 * @file	fs/emmcfs/super.c
 * @brief	The eMMCFS initialization and superblock operations.
 * @author	Dmitry Voytik, d.voytik@samsung.com
 * @author
 * @date	01/17/2012
 *
 * eMMCFS -- Samsung eMMC chip oriented File System, Version 1.
 *
 * In this file mount and super block operations are implemented.
 *
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

#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/vfs.h>
#include <linux/buffer_head.h>
#include <linux/emmcfs_fs.h>
#include <linux/crc32.h>
#include <linux/version.h>
#include <linux/genhd.h>
#include <linux/vmalloc.h>

#include "emmcfs.h"
#include "btree.h"
#include "debug.h"

#define EMMCFS_MOUNT_INFO(fmt, ...)\
do {\
	printk(KERN_INFO "[EMMCFS] " fmt, ##__VA_ARGS__);\
} while(0)

/* Prototypes */
static inline void get_dev_sectors_count(struct super_block *sb,
							sector_t *s_count);
/**
 * @brief			B-tree destructor.
 * @param [in,out]	btree	Pointer to btree that will be destroyed
 * @return		void
 */
static void emmcfs_put_btree(struct emmcfs_btree *btree)
{
	int i;

	for (i = 0; i < EMMCFS_BNODE_CACHE_ITEMS; i++)
		if (btree->stupid_cache[i])
			emmcfs_put_cache_bnode(btree->stupid_cache[i]);

	emmcfs_destroy_free_bnode_bitmap(btree->bitmap);
	EMMCFS_BUG_ON(!btree->head_bnode);
	/*emmcfs_put_bnode(btree->head_bnode);*/
	iput(btree->inode);
	kfree(btree->rw_tree_lock);
	kfree(btree);
}

/**
 * @brief			Inode bitmap destructor.
 * @param [in,out]	sbi	Pointer to sb info which free_inode_bitmap
 * 				will be destroyed
 * @return		void
 */
static void destroy_free_inode_bitmap(struct emmcfs_sb_info *sbi)
{
	EMMCFS_LOG_FUNCTION_START(sbi);

	iput(sbi->free_inode_bitmap.inode);
	sbi->free_inode_bitmap.inode = NULL;

	EMMCFS_LOG_FUNCTION_END(sbi, 0);
}

/**
 * @brief			Hard links area destructor.
 * @param [in,out] 	sbi	Pointer to sb info which hard links area
 *	 			will be destroyed
 * @return		void
 */
static void emmcfs_destroy_hlinks_area(struct emmcfs_sb_info *sbi)
{
	EMMCFS_LOG_FUNCTION_START(sbi);

	iput(sbi->hard_links.inode);
	sbi->hard_links.inode = NULL;

	EMMCFS_LOG_FUNCTION_END(sbi, 0);
}

/** Debug mask is a module parameter. This parameter enables particular debug
 *  type printing (see EMMCFS_DBG_* in fs/emmcfs/debug.h).
 */
unsigned int debug_mask = 0
		/*+ EMMCFS_DBG_INO*/
		/*+ EMMCFS_DBG_FSM*/
		/*+ EMMCFS_DBG_SNAPSHOT*/
		/*+ EMMCFS_DBG_TRANSACTION*/
		+ EMMCFS_DBG_TMP
		;

/** The eMMCFS inode cache
 */
static struct kmem_cache *emmcfs_inode_cachep;

/**
 * @brief			Method to allocate inode.
 * @param [in,out]	sb	Pointer to eMMCFS superblock
 * @return			Returns pointer to inode, NULL on failure
 */
static struct inode *emmcfs_alloc_inode(struct super_block *sb)
{
	struct emmcfs_inode_info *inode;

	EMMCFS_LOG_FUNCTION_START(EMMCFS_SB(sb));

	inode = kmem_cache_alloc(emmcfs_inode_cachep, GFP_KERNEL);
	if (!inode)
		return NULL;
	inode->name = NULL;
	inode->fork.total_block_count = 0;

	inode->fork.prealloc_block_count = 0;
	inode->fork.prealloc_start_block = 0;

	inode->bnode_hint.bnode_id = 0;
	inode->bnode_hint.pos = -1;

	EMMCFS_LOG_FUNCTION_END(EMMCFS_SB(sb), 0);
	return &inode->vfs_inode;
}

/**
 * @brief			Method to destroy inode.
 * @param [in,out] 	inode	Pointer to inode for destroy
 * @return		void
 */
static void emmcfs_destroy_inode(struct inode *inode)
{
	if (EMMCFS_I(inode)->name)
		kfree(EMMCFS_I(inode)->name);

	kmem_cache_free(emmcfs_inode_cachep, EMMCFS_I(inode));
}

/**
 * @brief			Update extended super blocks forks.
 * @param [in,out] 	sbi	Pointer to sb info, which forks will be updated
 * @return		void
 */
static void update_super_forks(struct emmcfs_sb_info *sbi)
{
	struct emmcfs_extended_super_block *exsb = EMMCFS_RAw_EXSB(sbi);
	__le32 checksum;

	EMMCFS_LOG_FUNCTION_START(sbi);

	/* Catalog tree fork */
	emmcfs_form_fork(&exsb->catalog_tree_fork,
			sbi->catalog_tree->inode);

	/* Extents overflow tree fork */
	emmcfs_form_fork(&exsb->extents_overflow_tree_fork,
			sbi->extents_tree->inode);

	/* The inode number allocation fork */
	emmcfs_form_fork(&exsb->inode_num_bitmap_fork,
			sbi->free_inode_bitmap.inode);

	/* Hard links file fork */
	emmcfs_form_fork(&exsb->hlinks_fork, sbi->hard_links.inode);

	/* Update cheksum */
	checksum = crc32(0, exsb, sizeof(*exsb)
			- sizeof(exsb->checksum));

	exsb->checksum = checksum;
	EMMCFS_LOG_FUNCTION_END(sbi, 0);
}

/**
 * @brief 		Sync starting superblock.
 * @param [in] 	sb 	Superblock information
 * @return 		Returns error code
 */
int emmcfs_sync_first_super(struct super_block *sb)
{
	struct emmcfs_sb_info *sbi = sb->s_fs_info;
	int ret = 0;
	EMMCFS_LOG_FUNCTION_START(sbi);

	update_super_forks(sbi);

	if (trylock_page(sbi->superblocks)) {
		set_page_writeback(sbi->superblocks);
		ret = emmcfs_write_page(sb->s_bdev, sbi->superblocks,
			EMMCFS_RESERVED_AREA_LENGTH /
			SECTOR_SIZE + SB_SIZE_IN_SECTOR ,
			ESB_SIZE_IN_SECTOR,
			SB_SIZE_IN_SECTOR * SECTOR_SIZE);
		unlock_page(sbi->superblocks);
	}
	EMMCFS_LOG_FUNCTION_END(sbi, ret);
	return ret;
}

/**
 * @brief 		Sync finalizing superblock.
 * @param [in] 	sb 	Superblock information
 * @return 		Returns error code
 */
int emmcfs_sync_second_super(struct super_block *sb)
{
	struct emmcfs_sb_info *sbi = sb->s_fs_info;
	sector_t s_count;
	int ret = 0;

	EMMCFS_LOG_FUNCTION_START(sbi);
	get_dev_sectors_count(sb, &s_count);
	update_super_forks(sbi);

	if (trylock_page(sbi->superblocks)) {
		set_page_writeback(sbi->superblocks);
		ret = emmcfs_write_page(sb->s_bdev, sbi->superblocks,
			s_count - EMMCFS_RESERVED_AREA_LENGTH /
			SECTOR_SIZE - ESB_SIZE_IN_SECTOR,
			ESB_SIZE_IN_SECTOR,
			SB_SIZE_IN_SECTOR * SECTOR_SIZE);
		unlock_page(sbi->superblocks);
	}
	EMMCFS_LOG_FUNCTION_END(sbi, ret);
	return ret;
}

/**
 * @brief		Sync runtime and on-disk copies of superblocks.
 * @param [in] 	sb	Pointer to superblock, which sbi will update disk sb
 * @param [in] 	wait	Block on write completion
 * @return		Returns error code
 */
static int emmcfs_sync_super(struct super_block *sb, int wait)
{
	struct emmcfs_sb_info *sbi = sb->s_fs_info;
	int ret = 0;

	EMMCFS_LOG_FUNCTION_START(sbi);

	if (!test_option(sbi, SNAPSHOT)) {
		/* Update layout structures */
		ret = emmcfs_sync_first_super(sb);
		if (ret)
			return ret;

		ret = emmcfs_sync_second_super(sb);
		if (ret)
			return ret;

		clear_sbi_flag(sbi, EXSB_DIRTY);
	}
	sb->s_dirt = 0;
	EMMCFS_LOG_FUNCTION_END(sbi, ret);
	return ret;
}

/**
 * @brief       		Method to write out all dirty data associated
 * 				with the superblock.
 * @param [in,out] 	sb   	Pointer to the eMMCFS superblock
 * @param [in,out] 	wait 	Block on write completion
 * @return      		Returns 0 on success, errno on failure
 */
static int emmcfs_sync_fs(struct super_block *sb, int wait)
{
	EMMCFS_LOG_FUNCTION_START(EMMCFS_SB(sb));
	emmcfs_sync_super(sb, wait);
	EMMCFS_LOG_FUNCTION_END(EMMCFS_SB(sb), 0);
	return 0;
}

/**
 * @brief		Write superblock on disk.
 * @param [in] 	sb	Pointer to superblock which sbi will update disk sb
 * @return	void
 */
static void emmcfs_write_super(struct super_block *sb)
{
	EMMCFS_LOG_FUNCTION_START(EMMCFS_SB(sb));
	if (!(EMMCFS_IS_READONLY(sb)))
		emmcfs_sync_fs(sb, 1);
	else
		sb->s_dirt = 0;
	EMMCFS_LOG_FUNCTION_END(EMMCFS_SB(sb), 0);
}

/**
 * @brief			Method to free superblock (unmount).
 * @param [in,out] 	sbi	Pointer to the eMMCFS superblock
 * @return		void
 */
static void destroy_super(struct emmcfs_sb_info *sbi)
{
	sbi->raw_superblock_copy = NULL;
	sbi->raw_superblock = NULL;

	if (sbi->superblocks) {
		kunmap(sbi->superblocks);
		__free_pages(sbi->superblocks, 0);
	}

	if (sbi->superblocks_copy) {
		kunmap(sbi->superblocks_copy);
		__free_pages(sbi->superblocks_copy, 0);
	}
}

/**
 * @brief			Method to free sbi info.
 * @param [in,out] 	sb	Pointer to a superblock
 * @return		void
 */
static void emmcfs_put_super(struct super_block *sb)
{
	/* d.voytik-TODO-29-12-2011-17-22-00: [emmcfs_put_super]
	 * implement emmcfs_put_super() */
	struct emmcfs_sb_info *sbi = sb->s_fs_info;

	EMMCFS_LOG_FUNCTION_START(sbi);
	emmcfs_put_btree(sbi->catalog_tree);
	sbi->catalog_tree = NULL;
	emmcfs_put_btree(sbi->extents_tree);
	sbi->extents_tree = NULL;
	if (!(EMMCFS_IS_READONLY(sb))) {
		destroy_free_inode_bitmap(sbi);
		emmcfs_destroy_snapshot_manager(sbi);
		emmcfs_fsm_destroy_management(sb);
	}
	emmcfs_destroy_hlinks_area(sbi);

	if (sb->s_dirt || is_sbi_flag_set(sbi, EXSB_DIRTY))
		emmcfs_write_super(sb);

	destroy_super(sbi);

	EMMCFS_LOG_FUNCTION_END(sbi, 0);

#ifdef CONFIG_EMMCFS_PROC_INFO
	emmcfs_destroy_nesting_keeper(sbi);
	if(emmcfs_remove_from_list(sbi))
		EMMCFS_BUG();
	emmcfs_destroy_log_buffer(sbi);
#endif
	kfree(sbi);
	EMMCFS_DEBUG_SB("finished");
}

/**
 * @brief			Force FS into a consistency state and
 *				lock it (for LVM).
 * @param [in,out]	sb	Pointer to the eMMCFS superblock
 * @remark			TODO: detailed description
 * @return			Returns 0 on success, errno on failure
 */
static int emmcfs_freeze(struct super_block *sb)
{
	/* d.voytik-TODO-29-12-2011-17-24-00: [emmcfs_freeze]
	 * implement emmcfs_freeze() */
	int ret = 0;

	EMMCFS_LOG_FUNCTION_START(EMMCFS_SB(sb));
	EMMCFS_DEBUG_SB("finished (ret = %d)", ret);
	EMMCFS_LOG_FUNCTION_END(EMMCFS_SB(sb), ret);
	return ret;
}

/**
 * @brief			Calculates metadata size, using extended
 * 				superblock's forks
 * @param [in,out] 	sbi	Pointer to the eMMCFS superblock
 * @return		metadata size in 4K-blocks
 */
static u64 calc_special_files_size(struct emmcfs_sb_info *sbi)
{
	u64 res = 0;
	struct emmcfs_extended_super_block *exsb;

	exsb = EMMCFS_RAw_EXSB(sbi);
	res += exsb->catalog_tree_fork.total_blocks_count;
	res += exsb->extents_overflow_tree_fork.total_blocks_count;
	res += exsb->hlinks_fork.total_blocks_count;
	res += exsb->inode_num_bitmap_fork.total_blocks_count;
	res += exsb->lebs_bitmap_fork.total_blocks_count;
	res += exsb->snapshot_fork.total_blocks_count;
	res += (sbi->total_leb_count << sbi->log_blocks_in_leb)
			- exsb->volume_body.length;
	return res;
}

/**
 * @brief			Get FS statistics.
 * @param [in,out] 	dentry	Pointer to directory entry
 * @param [in,out] 	buf	Point to kstatfs buffer where information
 *				will be placed
 * @return			Returns 0 on success, errno on failure
 */
static int emmcfs_statfs(struct dentry *dentry, struct kstatfs *buf)
{
	struct super_block	*sb = dentry->d_sb;
	struct emmcfs_sb_info	*sbi = sb->s_fs_info;
#ifdef CONFIG_EMMCFS_CHECK_FRAGMENTATION
	int count;
#endif

	EMMCFS_LOG_FUNCTION_START(sbi);
	buf->f_type = (long)EMMCFS_SB_SIGNATURE;
	buf->f_bsize = sbi->block_size;
	buf->f_blocks = sbi->total_leb_count << sbi->log_blocks_in_leb;
	buf->f_bavail = buf->f_bfree =
		sbi->free_blocks_count;
	buf->f_files = sbi->files_count + sbi->folders_count;
	buf->f_fsid.val[0] = sbi->volume_uuid & 0xFFFFFFFFUL;
	buf->f_fsid.val[1] = (sbi->volume_uuid >> 32) & 0xFFFFFFFFUL;
	buf->f_namelen = EMMCFS_FILE_NAME_LEN;

#ifdef CONFIG_EMMCFS_CHECK_FRAGMENTATION
	for (count = 0; count < EMMCFS_EXTENTS_COUNT_IN_FORK; count++) {
		msleep(50);
		printk(KERN_ERR "in %d = %lu", count, sbi->in_fork[count]);
	}

	msleep(50);
	printk(KERN_ERR "in extents overflow = %lu", sbi->in_extents_overflow);
#endif

	{
		const char *device_name = sb->s_bdev->bd_part->__dev.kobj.name; 
		u64 meta_size = calc_special_files_size(sbi) <<
			(sbi->block_size_shift - 10);
		u64 data_size = (buf->f_blocks - sbi->free_blocks_count -
				calc_special_files_size(sbi)) <<
			(sbi->block_size_shift - 10);

		printk(KERN_INFO "%s: Meta: %lluKB     Data: %lluKB\n",
				device_name, meta_size, data_size);
	}
	EMMCFS_LOG_FUNCTION_END(sbi, 0);
	EMMCFS_DEBUG_SB("finished");
	return 0;
}

/**
 * @brief			Evict inode.
 * @param [in,out] 	inode	Pointer to inode that will be evicted
 * @return		void
 */
static void emmcfs_evict_inode(struct inode *inode)
{
	EMMCFS_LOG_FUNCTION_START(EMMCFS_SB(inode->i_sb));
	EMMCFS_DEBUG_INO("evict inode %lu nlink\t%u",
			inode->i_ino, inode->i_nlink);

	truncate_inode_pages(&inode->i_data, 0);
	invalidate_inode_buffers(inode);

#if LINUX_VERSION_CODE == KERNEL_VERSION(2, 6, 35)
	clear_inode(inode);
#elif LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 20) || \
		LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 33)
	end_writeback(inode);
#endif
	EMMCFS_LOG_FUNCTION_END(EMMCFS_SB(inode->i_sb), 0);
}

/*
 * Structure of the eMMCFS super block operations
 */
static struct super_operations emmcfs_sops = {
	.alloc_inode	= emmcfs_alloc_inode,
	.destroy_inode	= emmcfs_destroy_inode,
	.put_super	= emmcfs_put_super,
	.write_super	= emmcfs_write_super,
	.sync_fs	= emmcfs_sync_fs,
	.freeze_fs	= emmcfs_freeze,
	.statfs		= emmcfs_statfs,
#if LINUX_VERSION_CODE == KERNEL_VERSION(2, 6, 35)
	.delete_inode	= emmcfs_evict_inode,
#elif LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 20) || \
		LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 33)
	.evict_inode	= emmcfs_evict_inode,
#else
	BUILD_EMMCFS_BUG();
#endif
};

/**
 * @brief		Determines volume size in sectors.
 * @param [in] 	sb	VFS super block
 * @param [out]	s_count	Stores here sectors count of volume
 * @return	void
 */
static inline void get_dev_sectors_count(struct super_block *sb,
							sector_t *s_count)
{
	EMMCFS_LOG_FUNCTION_START(EMMCFS_SB(sb));
	*s_count = sb->s_bdev->bd_inode->i_size >> SECTOR_SIZE_SHIFT;
	EMMCFS_LOG_FUNCTION_END(EMMCFS_SB(sb), 0);
}

/**
 * @brief		Determines volume size in blocks.
 * @param [in] 	sb	VFS super block
 * @param [out] b_count	Stores here blocks count of volume
 * @return	void
 */
static inline void get_dev_blocks_count(struct super_block *sb,
							sector_t *b_count)
{
	EMMCFS_LOG_FUNCTION_START(EMMCFS_SB(sb));
	*b_count = sb->s_bdev->bd_inode->i_size >> sb->s_blocksize_bits;
	EMMCFS_LOG_FUNCTION_END(EMMCFS_SB(sb), 0);
}

/**
 * @brief			Sanity check of eMMCFS super block.
 * @param [in] 	esb		The eMMCFS super block
 * @param [in]	str_sb_type	Determines which SB is verified on error
 *				printing
 * @param [in] 	silent		Doesn't print errors if silent is true
 * @return			Returns 0 on success, errno on failure
 */
static int emmcfs_verify_sb(struct emmcfs_super_block *esb, char *str_sb_type,
				int silent)
{
	__le64 emmcfs_check_magic = EMMCFS_SB_SIGNATURE;
	__le32 checksum;

	/* check magic number */
	if (memcmp(esb->signature, (char *)&emmcfs_check_magic,
						sizeof(esb->signature))) {
		if (!silent || debug_mask & EMMCFS_DBG_SB)
			EMMCFS_ERR("%s: bad signature - 0x%08llx, expected - "\
				"0x%08llx\n", str_sb_type,
			(long long unsigned int)*(__le64 *)(esb->signature),
			(long long unsigned int)(emmcfs_check_magic));
		return -EINVAL;
	}
	/* check version */
	if (esb->version.major != EMMCFS_SB_VER_MAJOR ||\
			esb->version.minor != EMMCFS_SB_VER_MINOR) {
		if (!silent || debug_mask & EMMCFS_DBG_SB)
			EMMCFS_ERR("%s: bad version (major = %d, minor = %d)\n",
					str_sb_type, (int)esb->version.major,
					(int)esb->version.minor);
		return -EINVAL;
	}
	/* check crc32 */
	checksum = crc32(0, esb, sizeof(*esb) - sizeof(esb->checksum));
	if (esb->checksum != checksum) {
		EMMCFS_ERR("%s: bad checksum - 0x%x, must be 0x%x\n",
				str_sb_type, esb->checksum, checksum);
		return -EINVAL;
	}
	return 0;
}

/**
 * @brief		Fill run-time superblock from on-disk superblock.
 * @param [in] 	esb	The eMMCFS super block
 * @param [in] 	sb	VFS superblock
 * @return		Returns 0 on success, errno on failure
 */
static int fill_runtime_superblock(struct emmcfs_super_block *esb,
		struct super_block *sb)
{
	int ret = 0;
	unsigned long block_size;
	unsigned int bytes_in_leb;
	unsigned long long total_block_count;
	sector_t dev_sectors_count;
	struct emmcfs_sb_info *sbi = sb->s_fs_info;

	EMMCFS_LOG_FUNCTION_START(sbi);
	/* check total block count in SB */

	if (*((long *) esb->mkfs_git_branch) &&
			*((long *) esb->mkfs_git_hash)) {
		EMMCFS_MOUNT_INFO("mkfs git branch is \"%s\"\n",
				esb->mkfs_git_branch);
		EMMCFS_MOUNT_INFO("mkfs git revhash \"%.40s\"\n",
				esb->mkfs_git_hash);
	}

	/* check if block size is supported and set it */
	block_size = 1 << esb->log_block_size;
	if (block_size & ~(512 | 1024 | 2048 | 4096)) {
		EMMCFS_ERR("unsupported block size (%ld)\n", block_size);
		ret = -EINVAL;
		goto err_exit;
	}
	sbi->block_size = block_size;
	if (!sb_set_blocksize(sb, sbi->block_size))
		EMMCFS_ERR("can't set block size\n");
	sbi->block_size_shift = esb->log_block_size;
	sbi->log_sectors_per_block = sbi->block_size_shift - SECTOR_SIZE_SHIFT;
	sbi->sectors_per_volume = esb->sectors_per_volume;

	sbi->offset_msk_inblock = 0xFFFFFFFF >> (32 - sbi->block_size_shift);

	/* check if LEB size is supported and set it */
	bytes_in_leb = 1 << esb->log_leb_size;
	if (bytes_in_leb == 0 || bytes_in_leb < block_size) {
		EMMCFS_ERR("unsupported LEB size (%u)\n", bytes_in_leb);
		ret = -EINVAL;
		goto err_exit;
	}
	sbi->log_blocks_in_leb = esb->log_leb_size - esb->log_block_size;

	sbi->btree_node_size_blks = 1 << sbi->log_blocks_in_leb;


	sbi->total_leb_count = le64_to_cpu(esb->total_leb_count);
	total_block_count = sbi->total_leb_count << sbi->log_blocks_in_leb;

	get_dev_blocks_count(sb, &dev_sectors_count);
	if (total_block_count /** (block_size/SECTOR_SIZE)*/ >
			dev_sectors_count || total_block_count == 0) {
		EMMCFS_ERR("bad FS block count: %llu, device has %llu blocks\n",
				total_block_count, dev_sectors_count);
		ret = -EINVAL;
		goto err_exit;
	}

	sbi->lebs_bm_log_blocks_block =
		le32_to_cpu(esb->lebs_bm_log_blocks_block);
	sbi->lebs_bm_bits_in_last_block =
		le32_to_cpu(esb->lebs_bm_bits_in_last_block);
	sbi->lebs_bm_blocks_count =
		le32_to_cpu(esb->lebs_bm_blocks_count);


	/* squash 128 bit UUID into 64 bit by xoring */
	sbi->volume_uuid = le64_to_cpup((void *)esb->volume_uuid) ^
			le64_to_cpup((void *)esb->volume_uuid + sizeof(u64));

	if (esb->case_insensitive)
		set_option(sbi, CASE_INSENSITIVE);

err_exit:
	EMMCFS_LOG_FUNCTION_END(sbi, ret);
	return ret;
}

/**
 * @brief			Check volume for errors.
 * @param [in,out]	sb	Pointer to superblock which pages will be
 * 				checked
 * @return			Returns 0 on success, errno on failure
 */
static int emmcfs_volume_check(struct super_block *sb)
{
	int ret = 0;
	struct page *superblocks;
	void *raw_superblocks;
	struct emmcfs_super_block *esb;

	superblocks = alloc_page(GFP_KERNEL | __GFP_ZERO);
	if (!superblocks)
		return -ENOMEM;

	lock_page(superblocks);
	ret = emmcfs_read_page(sb->s_bdev, superblocks, 0, 2, 0);
	unlock_page(superblocks);

	if (ret)
		goto exit_free_page;

	raw_superblocks = kmap(superblocks);

	/* check superblocks */
	esb = (struct emmcfs_super_block *)raw_superblocks;
	ret = emmcfs_verify_sb(esb, "SB", 0);
	if (ret)
		goto exit;

	raw_superblocks++;
	ret = emmcfs_verify_sb(esb, "SB", 0);

exit:
	kunmap(superblocks);
exit_free_page:
	__free_pages(superblocks, 0);

	return ret;
}


/**
 * @brief				Restore superblock from snapshot.
 * @param [in] 	sb			Pointer to superblock
 * @param [in] 	superblocks		Pointer to superblock which will be
 * 					restored if snapshot magic is presented
 * @param [in]	superblocks_copy	Pointer to superblock_copy from where
 * 					superblock will be restored if snapshot
 * 					magic is present
 * @return				Returns 0 on success, errno on failure
 */
static int emmcfs_restore_exsb(struct super_block *sb, struct page *superblocks,
		struct page *superblocks_copy)
{
	struct emmcfs_snapshot_descriptor *snapshot_head;
	struct emmcfs_sb_info *sbi = sb->s_fs_info;
	__le32 emmcfs_check_magic = EMMCFS_SNAPSHOT_MAGIC;
	struct page *page;
	int error = 0;
	sector_t snapshot_start;

	void *raw_superblocks = NULL;
	void *raw_superblocks_copy = NULL;

	raw_superblocks = kmap(superblocks);
	raw_superblocks_copy = kmap(superblocks_copy);

	snapshot_start = le64_to_cpu(((struct emmcfs_extended_super_block *)
		(raw_superblocks + SB_SIZE))->snapshot_fork.extents[0].begin);

	page = alloc_page(GFP_KERNEL | __GFP_ZERO);
	if (!page) {
		error = -ENOMEM;
		goto unmap_sb;
	}
	lock_page(page);
	error = emmcfs_read_pages(sbi->sb->s_bdev, &page,
			snapshot_start << (PAGE_CACHE_SHIFT -
			SECTOR_SIZE_SHIFT), 1);

	unlock_page(page);

	if (error)
		goto release_head;

	snapshot_head = (struct emmcfs_snapshot_descriptor *)kmap(page);

	/* check magic number */
	if (memcmp(snapshot_head->signature, (char *)&emmcfs_check_magic,
				sizeof(snapshot_head->signature))) {
		/* No magic number -> no snapshot */
		error = 0;
		goto release_head;
	}

	/* snapshot magic is present, restore superblocks from copy */
	memcpy(raw_superblocks, raw_superblocks_copy, PAGE_CACHE_SIZE);

release_head:
	kunmap(page);
	__free_pages(page, 0);

unmap_sb:
	kunmap(superblocks);
	kunmap(superblocks_copy);
	return error;
}


/**
 * @brief		Reads and checks and recovers eMMCFS super blocks.
 * @param [in] 	sb	The VFS super block
 * @param [in] 	silent	Do not print errors if silent is true
 * @return		Returns 0 on success, errno on failure
 */
static int emmcfs_sb_read(struct super_block *sb, int silent)
{
	sector_t s_count;

	void *raw_superblocks;
	void *raw_superblocks_copy;
	struct emmcfs_super_block *esb;
	struct emmcfs_super_block *esb_copy;
	int ret = 0;
	int check_sb;
	int check_sb_copy;
	struct emmcfs_sb_info *sbi = sb->s_fs_info;


	struct page *superblocks;
	struct page *superblocks_copy;

	EMMCFS_LOG_FUNCTION_START(sbi);
	/* alloc pages for superblocks */
	superblocks = alloc_page(GFP_KERNEL | __GFP_ZERO);

	if (IS_ERR(superblocks)) {
		EMMCFS_ERR("Fail to alloc page for reading superblocks");
		ret = PTR_ERR(superblocks);
		EMMCFS_LOG_FUNCTION_END(sbi, ret);
		return ret;
	}

	superblocks_copy = alloc_page(GFP_KERNEL | __GFP_ZERO);
	if (IS_ERR(superblocks_copy)) {
		__free_pages(superblocks, 0);
		EMMCFS_ERR("Fail to alloc page for reading superblocks copy");
		ret = PTR_ERR(superblocks_copy);
		EMMCFS_LOG_FUNCTION_END(sbi, ret);
		return ret;
	}

	lock_page(superblocks);
	/* read super block and extended super block from volume */
	/*  0       1       2        3                8*/
	/*  | reserved area |   SB   |    ESB         |*/
	ret = emmcfs_read_page(sb->s_bdev, superblocks,
			EMMCFS_RESERVED_AREA_LENGTH / SECTOR_SIZE,
			ESB_SIZE_IN_SECTOR + SB_SIZE_IN_SECTOR, 0);
	unlock_page(superblocks);

#ifdef CONFIG_CMA_DEBUG 
       dump_page(superblocks);
       dump_page(superblocks_copy);
#endif 

	if (ret)
		goto err_superblock;

	/*    -8        -7              -2            volume end*/
	/*     |   SB    |    ESB        |reserved area |       */
	get_dev_sectors_count(sb, &s_count);
	lock_page(superblocks_copy);
	ret = emmcfs_read_page(sb->s_bdev, superblocks_copy,
			s_count - EMMCFS_RESERVED_AREA_LENGTH / SECTOR_SIZE -
			SB_SIZE_IN_SECTOR - ESB_SIZE_IN_SECTOR,
			ESB_SIZE_IN_SECTOR + SB_SIZE_IN_SECTOR, 0);
	unlock_page(superblocks_copy);

	if (ret)
		goto err_superblock_copy;

	raw_superblocks = kmap(superblocks);
	raw_superblocks_copy = kmap(superblocks_copy);

	/* check superblocks */
	esb = (struct emmcfs_super_block *)raw_superblocks;
	esb_copy =  (struct emmcfs_super_block *)raw_superblocks_copy;

	check_sb = emmcfs_verify_sb(esb, "SB", silent);
	check_sb_copy = emmcfs_verify_sb(esb_copy, "SB_COPY", 1);

	if (check_sb && check_sb_copy) {
		/*both superblocks are corrupted*/
		ret = check_sb;
		if (!silent || debug_mask & EMMCFS_DBG_SB)
			EMMCFS_ERR("can't find an emmcfs filesystem on dev %s",
					sb->s_id);
		goto err_superblock_copy;
	} else if ((!check_sb) && check_sb_copy) {
		/*first superblock is ok, copy is corrupted, recovery*/
		memcpy(esb_copy, esb, SECTOR_SIZE * SB_SIZE_IN_SECTOR);
		/* write superblock copy to disk */
		lock_page(superblocks_copy);
		set_page_writeback(superblocks_copy);
		ret = emmcfs_write_page(sb->s_bdev, superblocks_copy,
			s_count - EMMCFS_RESERVED_AREA_LENGTH / SECTOR_SIZE -
			SB_SIZE_IN_SECTOR - ESB_SIZE_IN_SECTOR,
			SB_SIZE_IN_SECTOR, 0);
		unlock_page(superblocks_copy);
	} else if (check_sb && (!check_sb_copy)) {
		/*first superblock is ok, copy is corrupted, recovery*/
		memcpy(esb, esb_copy, SECTOR_SIZE * SB_SIZE_IN_SECTOR);
		/* write superblock copy to disk */
		lock_page(superblocks);
		set_page_writeback(superblocks);
		ret = emmcfs_write_page(sb->s_bdev, superblocks,
			EMMCFS_RESERVED_AREA_LENGTH / SECTOR_SIZE,
			SB_SIZE_IN_SECTOR, 0);
		unlock_page(superblocks);
	}


	if (ret)
		goto err_superblock_copy;


	ret = fill_runtime_superblock(esb, sb);

	if (ret)
		goto err_superblock_copy;

	sbi->raw_superblock = raw_superblocks;
	sbi->raw_superblock_copy = raw_superblocks_copy;
	sbi->superblocks = superblocks;
	sbi->superblocks_copy = superblocks_copy;
	EMMCFS_LOG_FUNCTION_END(sbi, 0);

	return 0;

err_superblock_copy:
	__free_pages(superblocks_copy, 0);
err_superblock:
	__free_pages(superblocks, 0);
	EMMCFS_LOG_FUNCTION_END(sbi, ret);
	return ret;
}

/**
 * @brief		Verifies eMMCFS extended super block checksum.
 * @param [in] 	exsb	The eMMCFS super block
 * @return		Returns 0 on success, errno on failure
 */
static int emmcfs_verify_exsb(struct emmcfs_extended_super_block *exsb)
{
	__le32 checksum;

	/* check crc32 */
	checksum = crc32(0, exsb, sizeof(*exsb) - sizeof(exsb->checksum));

	if (exsb->checksum != checksum) {
		EMMCFS_ERR("bad checksum of extended super block - 0x%x, "\
				"must be 0x%x\n", exsb->checksum, checksum);
		return -EINVAL;
	} else
		return 0;
}

/**
 * @brief		Reads and checks eMMCFS extended super block.
 * @param [in] 	sb	The VFS super block
 * @return		Returns 0 on success, errno on failure
 */
static int emmcfs_extended_sb_read(struct super_block *sb)
{
	struct emmcfs_extended_super_block *exsb;
	struct emmcfs_extended_super_block *exsb_copy;
	int ret = 0;
	struct emmcfs_sb_info *sbi = sb->s_fs_info;
	int check_exsb;
	int check_exsb_copy;
	sector_t s_count;

	EMMCFS_LOG_FUNCTION_START(sbi);
	exsb =  EMMCFS_RAw_EXSB(sbi);
	exsb_copy = EMMCFS_RAw_EXSB_COPY(sbi);

	check_exsb = emmcfs_verify_exsb(exsb);
	check_exsb_copy = emmcfs_verify_exsb(exsb_copy);

	get_dev_sectors_count(sb, &s_count);

	if (check_exsb && check_exsb_copy) {
		EMMCFS_ERR("Extended superblocks are corrupted");
		ret = check_exsb;
		EMMCFS_LOG_FUNCTION_END(sbi, ret);
		return ret;
	} else if ((!check_exsb) && check_exsb_copy) {
		/* extended superblock copy are corrupted, recovery */
		lock_page(sbi->superblocks);
		memcpy(exsb_copy, exsb,
				sizeof(struct emmcfs_extended_super_block));
		set_page_writeback(sbi->superblocks);
		ret = emmcfs_write_page(sb->s_bdev, sbi->superblocks,
			s_count - EMMCFS_RESERVED_AREA_LENGTH / SECTOR_SIZE -
			ESB_SIZE_IN_SECTOR, ESB_SIZE_IN_SECTOR,
			SB_SIZE_IN_SECTOR * SECTOR_SIZE);
		unlock_page(sbi->superblocks);
	} else if (check_exsb && (!check_exsb_copy)) {
		/* main extended superblock are corrupted, recovery */
		lock_page(sbi->superblocks_copy);
		memcpy(exsb, exsb_copy,
				sizeof(struct emmcfs_extended_super_block));
		set_page_writeback(sbi->superblocks_copy);
		ret = emmcfs_write_page(sb->s_bdev, sbi->superblocks_copy,
				EMMCFS_RESERVED_AREA_LENGTH / SECTOR_SIZE
				+ SB_SIZE_IN_SECTOR , ESB_SIZE_IN_SECTOR,
				SB_SIZE_IN_SECTOR * SECTOR_SIZE);
		unlock_page(sbi->superblocks_copy);
	}
	if(ret) {
		EMMCFS_LOG_FUNCTION_END(sbi, ret);
		return ret;
	}

	if (test_option(sbi, SNAPSHOT))
		ret = emmcfs_restore_exsb(sb, sbi->superblocks,
				sbi->superblocks_copy);


	EMMCFS_LOG_FUNCTION_END(sbi, ret);
	return ret;
}

/**
 * @brief		The eMMCFS B-tree common constructor.
 * @param [in] 	sbi	The eMMCFS superblock info
 * @param [in] 	btree	The eMMCFS B-tree
 * @param [in] 	inode	The inode
 * @return		Returns 0 on success, errno on failure
 */
static int fill_btree(struct emmcfs_sb_info *sbi,
		struct emmcfs_btree *btree, struct inode *inode)
{
	struct emmcfs_bnode *head_bnode, *bnode;
	struct emmcfs_raw_btree_head *raw_btree_head;
	__u64 bitmap_size;
	int err = 0;
	enum emmcfs_get_bnode_mode mode;

	EMMCFS_LOG_FUNCTION_START(sbi);
	btree->sbi = sbi;
	btree->inode = inode;
	btree->pages_per_node = 1 << (sbi->log_blocks_in_leb +
			sbi->block_size_shift - PAGE_SHIFT);
	btree->log_pages_per_node = sbi->log_blocks_in_leb +
			sbi->block_size_shift - PAGE_SHIFT;
	btree->node_size_bytes = btree->pages_per_node << PAGE_SHIFT;

	btree->rw_tree_lock = kmalloc(sizeof(rw_mutex_t), GFP_KERNEL);

	init_mutex(btree->rw_tree_lock);
	if (EMMCFS_IS_READONLY(sbi->sb))
		mode = EMMCFS_BNODE_MODE_RO;
	else
		mode = EMMCFS_BNODE_MODE_RW;

	head_bnode =  emmcfs_get_bnode(btree, 0, mode);
	if (IS_ERR(head_bnode)) {
		EMMCFS_LOG_FUNCTION_END(sbi, PTR_ERR(head_bnode));
		return PTR_ERR(head_bnode);
	}

	raw_btree_head = head_bnode->data;

	/* Check the magic */
	if (memcmp(raw_btree_head->magic, EMMCFS_BTREE_HEAD_NODE_MAGIC,
				sizeof(EMMCFS_BTREE_HEAD_NODE_MAGIC) - 1)) {
		err = -EINVAL;
		goto err_put_bnode;
	}

	btree->head_bnode = head_bnode;

	/* Fill free bnode bitmpap */
	bitmap_size = btree->node_size_bytes -
		((void *) &raw_btree_head->bitmap - head_bnode->data) -
		EMMCFS_BNODE_FIRST_OFFSET;
	btree->bitmap = build_free_bnode_bitmap(&raw_btree_head->bitmap, 0,
			bitmap_size, head_bnode);
	if (IS_ERR(btree->bitmap)) {
		err = PTR_ERR(btree->bitmap);
		btree->bitmap = NULL;
		goto err_put_bnode;
	}

	/* Check if root bnode non-empty */
	bnode = emmcfs_get_bnode(btree, emmcfs_btree_get_root_id(btree),
			EMMCFS_BNODE_MODE_RO);

	if (IS_ERR(bnode)) {
		err = PTR_ERR(bnode);
		goto err_put_bnode;
	}

	if (EMMCFS_BNODE_DSCR(bnode)->recs_count == 0) {
		emmcfs_put_bnode(bnode);
		err = -EINVAL;
		goto err_put_bnode;
	}
	emmcfs_put_bnode(bnode);

	EMMCFS_LOG_FUNCTION_END(sbi, 0);
	return 0;

err_put_bnode:
	emmcfs_put_bnode(head_bnode);
	EMMCFS_LOG_FUNCTION_END(sbi, err);
	return err;
}

/**
 * @brief		Count max catalog tree height.
 * @param [in] 	sbi	The eMMCFS super block info
 * @return		Returns max catalog tree height
 */
static int count_max_cattree_height(struct emmcfs_sb_info *sbi)
{
	struct emmcfs_btree *cat_tree = sbi->catalog_tree;

	int bnode_space_for_records_size = cat_tree->node_size_bytes -
		sizeof(struct emmcfs_gen_node_descr) -
		/* Check sum */
		sizeof(__le32) -
		/* Bnode always has pointer to its free space */
		sizeof(emmcfs_bt_off_t);

	int max_bnodes_on_volume = sbi->sb->s_bdev->bd_inode->i_size >>
		(PAGE_SHIFT + cat_tree->pages_per_node);

	int max_index_rec_len = sizeof(struct emmcfs_cattree_key) +
		sizeof(struct generic_index_value);

	int min_rec_num_in_index_bnode = bnode_space_for_records_size /
		(max_index_rec_len + sizeof(emmcfs_bt_off_t));

	int max_cattree_height = 1;
	int nodes = max_bnodes_on_volume;
	EMMCFS_LOG_FUNCTION_START(sbi);

	while (nodes > 1) {
		nodes /= min_rec_num_in_index_bnode;
		max_cattree_height++;
	}
#if 0
	/* Count max possible objects on volume. Not well tested yet, if you
	 * need this __FUNCTION__, please recheck at first */
	int min_record_size = sizeof(struct emmcfs_cattree_key) -
		sizeof(struct emmcfs_unicode_string) +
		sizeof(struct emmcfs_catalog_folder_record);

	int max_records_per_bnode = bnode_space_for_records_size /
		(min_record_size + sizeof(emmcfs_bt_off_t));

	int max_objects_on_volume = max_records_per_bnode *
		max_bnodes_on_volume;
#endif

	EMMCFS_LOG_FUNCTION_END(sbi, max_cattree_height);
	return max_cattree_height;
}

/**
 * @brief			Set up max snapshot transaction length.
 * @param [in,out]	sbi	The eMMCFS super block info
 * @return		void
 */
static void set_up_max_snapshot_height(struct emmcfs_sb_info *sbi)
{
	int btree_height = count_max_cattree_height(sbi) + 1; /* one for head */
	struct emmcfs_snapshot_info *snapshot = sbi->snapshot_info;

	EMMCFS_LOG_FUNCTION_START(sbi);
	snapshot->maximum_transaction_lenght =
			snapshot->max_pages_in_snapshot -
			snapshot->pages_per_head -
			8 * btree_height - 6;
	EMMCFS_LOG_FUNCTION_END(sbi, 0);
}

/**
 * @brief			Catalog tree constructor.
 * @param [in,out]	sbi	The eMMCFS super block info
 * @return			Returns 0 on success, errno on failure
 */
static int emmcfs_fill_cat_tree(struct emmcfs_sb_info *sbi)
{
	struct inode *inode = NULL;
	struct emmcfs_btree *cat_tree;

	int err = 0;

	EMMCFS_LOG_FUNCTION_START(sbi);
	cat_tree = kzalloc(sizeof(*cat_tree), GFP_KERNEL);
	if (!cat_tree) {
		EMMCFS_LOG_FUNCTION_END(sbi, -ENOMEM);
		return -ENOMEM;
	}

	cat_tree->btree_type = EMMCFS_BTREE_CATALOG;
	cat_tree->max_record_len = sizeof(struct emmcfs_cattree_key) +
			sizeof(struct emmcfs_catalog_file_record) + 1;
	inode = emmcfs_iget(sbi->sb, EMMCFS_CAT_TREE_INO, NULL);
	if (IS_ERR(inode)) {
		int ret = PTR_ERR(inode);
		kfree(cat_tree);
		EMMCFS_LOG_FUNCTION_END(sbi, ret);
		return ret;
	}

	err = fill_btree(sbi, cat_tree, inode);
	if (err)
		goto err_put_inode;

	cat_tree->comp_fn = test_option(sbi, CASE_INSENSITIVE) ?
			emmcfs_cattree_cmpfn_ci : emmcfs_cattree_cmpfn;
	sbi->catalog_tree = cat_tree;

	/*sbi->max_cattree_height = count_max_cattree_height(sbi);*/
	EMMCFS_LOG_FUNCTION_END(sbi, 0);
	return 0;

err_put_inode:
	iput(inode);
	EMMCFS_ERR("can not read catalog tree");
	EMMCFS_LOG_FUNCTION_END(sbi, err);
	return err;
}

/**
 * @brief			Extents tree constructor.
 * @param [in,out] 	sbi	The eMMCFS super block info
 * @return			Returns 0 on success, errno on failure
 */
static int emmcfs_fill_ext_tree(struct emmcfs_sb_info *sbi)
{
	struct inode *inode = NULL;
	struct emmcfs_btree *ext_tree;
	int err = 0;

	EMMCFS_LOG_FUNCTION_START(sbi);
	ext_tree = kzalloc(sizeof(*ext_tree), GFP_KERNEL);
	if (!ext_tree) {
		EMMCFS_LOG_FUNCTION_END(sbi, -ENOMEM);
		return -ENOMEM;
	}

	ext_tree->btree_type = EMMCFS_BTREE_EXTENTS;
	ext_tree->max_record_len = sizeof(struct emmcfs_exttree_key) +
			sizeof(struct emmcfs_exttree_record);
	inode = emmcfs_iget(sbi->sb, EMMCFS_EXTENTS_TREE_INO, NULL);
	if (IS_ERR(inode)) {
		int ret;
		kfree(ext_tree);
		ret = PTR_ERR(inode);
		EMMCFS_LOG_FUNCTION_END(sbi, ret);
		return ret;
	}

	err = fill_btree(sbi, ext_tree, inode);
	if (err)
		goto err_put_inode;

	ext_tree->comp_fn = emmcfs_exttree_cmpfn;
	sbi->extents_tree = ext_tree;
	EMMCFS_LOG_FUNCTION_END(sbi, 0);
	return 0;

err_put_inode:
	iput(inode);
	EMMCFS_ERR("can not read extents overflow tree");
	EMMCFS_LOG_FUNCTION_END(sbi, err);
	return err;
}

/**
 * @brief			Hardlinks area constructor.
 * @param [in,out] 	sbi	The eMMCFS super block info
 * @return			Returns 0 on success, errno on failure
 */
static int emmcfs_init_hlinks_area(struct emmcfs_sb_info *sbi)
{
	EMMCFS_LOG_FUNCTION_START(sbi);
	sbi->hard_links.inode = emmcfs_iget(sbi->sb, EMMCFS_HARDLINKS_AREA_INO,
			NULL);
	if (IS_ERR(sbi->hard_links.inode)) {
		EMMCFS_LOG_FUNCTION_END(sbi, PTR_ERR(sbi->hard_links.inode));
		return PTR_ERR(sbi->hard_links.inode);
	}

	EMMCFS_LOG_FUNCTION_END(sbi, 0);
	return 0;
}

/**
 * @brief			Free inode bitmap constructor.
 * @param [in,out] 	sbi	The eMMCFS super block info
 * @return			Returns 0 on success, errno on failure
 */
static int build_free_inode_bitmap(struct emmcfs_sb_info *sbi)
{
	int ret = 0;
	struct inode *inode;

	EMMCFS_LOG_FUNCTION_START(sbi);
	inode = emmcfs_iget(sbi->sb, EMMCFS_FREE_INODE_BITMAP_INO, NULL);
	if (IS_ERR(inode)) {
		ret = PTR_ERR(inode);
		EMMCFS_LOG_FUNCTION_END(sbi, ret);
		return ret;
	}

	atomic64_set(&sbi->free_inode_bitmap.last_used, EMMCFS_1ST_FILE_INO);
	sbi->free_inode_bitmap.inode = inode;
	EMMCFS_LOG_FUNCTION_END(sbi, ret);
	return ret;
}

#include <linux/genhd.h>
/**
 * @brief			Initialize the eMMCFS filesystem.
 * @param [in,out] 	sb	The VFS superblock
 * @param [in,out] 	data	FS private information
 * @param [in] 		silent	Flag whether to print error message
 * @remark			Reads super block and FS internal data
 * 				structures for futher use.
 * @return			Return 0 on success, errno on failure
 */
static int emmcfs_fill_super(struct super_block *sb, void *data, int silent)
{
	int			ret	= 0;
	struct emmcfs_sb_info	*sbi;
	struct inode		*root;
#ifdef CONFIG_EMMCFS_CHECK_FRAGMENTATION
	int count;
#endif

#ifdef CONFIG_EMMCFS_PRINT_MOUNT_TIME
	unsigned long mount_start = jiffies;
#endif



#if defined(EMMCFS_GIT_BRANCH) && defined(EMMCFS_GIT_REV_HASH) && \
		defined(EMMCFS_VERSION)
	printk(KERN_ERR "%.5s", EMMCFS_VERSION);
	EMMCFS_MOUNT_INFO("version is \"%s\"", EMMCFS_VERSION);
	EMMCFS_MOUNT_INFO("git branch is \"%s\"", EMMCFS_GIT_BRANCH);
	EMMCFS_MOUNT_INFO("git revhash \"%.40s\"", EMMCFS_GIT_REV_HASH);
#endif
#ifdef CONFIG_EMMCFS_NOOPTIMIZE
	printk(KERN_WARNING "[EMMCFS-warning]: Build optimization "
			"is switched off");
#endif

	if (!sb)
		return -ENXIO;
	if (!sb->s_bdev)
		return -ENXIO;
	if (!sb->s_bdev->bd_part)
		return -ENXIO;


	EMMCFS_MOUNT_INFO("mounting %s",
			sb->s_bdev->bd_part->__dev.kobj.name);

	sbi = kzalloc(sizeof(*sbi), GFP_KERNEL);
	if (!sbi)
		return -ENOMEM;
	sb->s_fs_info = sbi;
	sbi->sb = sb;

	sb->s_maxbytes = EMMCFS_MAX_FILE_SIZE_IN_BYTES;

	ret = emmcfs_parse_options(sb, data);
	if (ret) {
		EMMCFS_ERR("unable to parse mount options\n");
		ret = -EINVAL;
		goto emmcfs_parse_options_error;
	}

	ret = emmcfs_volume_check(sb);
	if (ret)
		goto not_emmcfs_volume;

#ifdef CONFIG_EMMCFS_PROC_INFO
	sbi->magic = EMMCFS_SB_DEBUG_MAGIC;
	ret = emmcfs_build_log_buffer(sbi);
	if (ret)
		goto emmcfs_build_log_buffer_error;
	ret = emmcfs_add_sb_to_list(sbi);
	if (ret)
		goto add_sb_to_list_error;
	ret = emmcfs_build_nesting_keeper(sbi);
	if (ret)
		goto build_nesting_keeper_error;
#endif

	ret = emmcfs_sb_read(sb, silent);
	if (ret)
		goto emmcfs_sb_read_error;

#ifdef EMMCFS_VERSION
	if (test_option(sbi, VERCHECK)) {
		int version = 0;
		char *parsing_string = EMMCFS_RAw_SB(sbi)->mkfs_git_branch;
		if (!sscanf(parsing_string, "v.%d", &version)) {
			EMMCFS_ERR("Not a release version!\n");
			ret = -EINVAL;
			goto emmcfs_extended_sb_read_error;
		} else if (version < CONFIG_EMMCFS_OLDEST_LAYOUT_VERSION) {
			EMMCFS_ERR("Non-supported ondisk layout. "
				"Please use version of mkfs.emmcfs utility not "
				"older than %d\n",
				CONFIG_EMMCFS_OLDEST_LAYOUT_VERSION);
			ret = -EINVAL;
			goto emmcfs_extended_sb_read_error;
		}
	}
#endif


	ret = emmcfs_extended_sb_read(sb);
	if (ret)
		goto emmcfs_extended_sb_read_error;

	printk(KERN_ERR "mount %d", EMMCFS_RAw_EXSB(sbi)->mount_counter);
	EMMCFS_MOUNT_INFO("mounted %d times\n",
			EMMCFS_RAw_EXSB(sbi)->mount_counter);

	emmcfs_debug_print_sb(sbi);

	sb->s_op = &emmcfs_sops;
	/* s_magic is 4 bytes on 32-bit; system */
	sb->s_magic = (unsigned long)EMMCFS_SB_SIGNATURE;

	sbi->max_cattree_height = 5;
	ret = emmcfs_build_snapshot_manager(sbi);
	if (ret)
		goto emmcfs_build_snapshot_manager_error;

	if (!(EMMCFS_IS_READONLY(sb))) {
		ret = emmcfs_fsm_build_management(sb);
		if (ret)
			goto emmcfs_fsm_create_error;
	}
	ret = emmcfs_fill_ext_tree(sbi);
	if (ret)
		goto emmcfs_fill_ext_tree_error;

	ret = emmcfs_fill_cat_tree(sbi);
	if (ret)
		goto emmcfs_fill_cat_tree_error;

	if (!(EMMCFS_IS_READONLY(sb))) {
		ret = build_free_inode_bitmap(sbi);
		if (ret)
			goto build_free_inode_bitmap_error;

		ret = emmcfs_process_orphan_inodes(sbi);
		if (ret)
			goto process_orphan_inodes_error;
	}

	/* allocate root directory */
	root = emmcfs_iget(sb, EMMCFS_ROOT_INO, NULL);
	if (IS_ERR(root)) {
		EMMCFS_ERR("failed to load root directory\n");
		ret = PTR_ERR(root);
		goto emmcfs_iget_err;
	}
	sb->s_root = d_alloc_root(root);
	if (!sb->s_root) {
		EMMCFS_ERR("unable to get root inode\n");
		ret = -EINVAL;
		goto d_alloc_root_err;
	}

	if (DATE_RESOLUTION_IN_NANOSECONDS_ENABLED)
		sb->s_time_gran = 1;
	else
		printk(KERN_WARNING
			"Date resolution in nanoseconds is disabled\n");

	ret = emmcfs_init_hlinks_area(sbi);
	if (ret)
		goto emmcfs_hlinks_area_err;

	set_up_max_snapshot_height(sbi);

#ifdef CONFIG_EMMCFS_CHECK_FRAGMENTATION
	for (count = 0; count < EMMCFS_EXTENTS_COUNT_IN_FORK; count++)
		sbi->in_fork[count] = 0;
	sbi->in_extents_overflow = 0;
#endif

	if (test_option(sbi, BTREE_CHECK)) {
		ret = emmcfs_verify_btree(sbi->catalog_tree);
		if (ret) {
			EMMCFS_ERR("Bad btree!!!\n");
			goto btree_not_verified;
		}
	}

	if (!(EMMCFS_IS_READONLY(sb))) {
		/* If somebody will switch power off right atre mounting,
		 * mount count will not inclreased
		 * But this operation takes a lot of time, and dramatically
		 * increase mount time */
		le32_add_cpu(&(EMMCFS_RAw_EXSB(sbi)->mount_counter), 1);
		if (test_option(sbi, SNAPSHOT))
			set_sbi_flag(sbi, EXSB_DIRTY_SNAPSHOT);
		else {
			set_sbi_flag(sbi, EXSB_DIRTY);
			sbi->sb->s_dirt = 1;
		}
	}

	set_sbi_flag(sbi, IS_MOUNT_FINISHED);
	EMMCFS_DEBUG_SB("finished ok");

#ifdef CONFIG_EMMCFS_PRINT_MOUNT_TIME
	{
		unsigned long result = jiffies - mount_start;
		printk(KERN_ERR "Mount time %lu ms\n", result * 1000 / HZ);
	}
#endif

	return 0;

btree_not_verified:
emmcfs_hlinks_area_err:
	dput(sb->s_root);
	sb->s_root = NULL;
	root = NULL;

d_alloc_root_err:
	iput(root);

emmcfs_iget_err:
process_orphan_inodes_error:
	destroy_free_inode_bitmap(sbi);

build_free_inode_bitmap_error:
	emmcfs_put_btree(sbi->catalog_tree);
	sbi->catalog_tree = NULL;

emmcfs_fill_cat_tree_error:
	emmcfs_put_btree(sbi->extents_tree);
	sbi->extents_tree = NULL;

emmcfs_fill_ext_tree_error:
	emmcfs_fsm_destroy_management(sb);

emmcfs_fsm_create_error:
	emmcfs_destroy_snapshot_manager(sbi);

emmcfs_build_snapshot_manager_error:
emmcfs_extended_sb_read_error:
emmcfs_sb_read_error:


#ifdef CONFIG_EMMCFS_PROC_INFO
	emmcfs_destroy_nesting_keeper(sbi);
build_nesting_keeper_error:
	emmcfs_remove_from_list(sbi);
add_sb_to_list_error:
	emmcfs_destroy_log_buffer(sbi);
emmcfs_build_log_buffer_error:
#endif

emmcfs_parse_options_error:
	destroy_super(sbi);

not_emmcfs_volume:

	kfree(sbi);
	EMMCFS_DEBUG_SB("finished with error = %d", ret);

	return ret;
}

/**
 * @brief				Method to get the eMMCFS super block.
 * @param [in,out]	fs_type		Describes the eMMCFS file system
 * @param [in] 		flags		Input flags
 * @param [in] 		dev_name	Block device name
 * @param [in,out] 	data		Private information
 * @param [in,out] 	mnt		Mounted eMMCFS filesystem information
 * @return				Returns 0 on success, errno on failure
 */
#if LINUX_VERSION_CODE == KERNEL_VERSION(2, 6, 35)
static int emmcfs_get_sb(struct file_system_type *fs_type, int flags,
				const char *dev_name, void *data,struct vfsmount *mnt)
{
	return get_sb_bdev(fs_type, flags, dev_name, data, emmcfs_fill_super, mnt);

}
#elif LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 20) || \
		LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 33)
static struct dentry *emmcfs_mount(struct file_system_type *fs_type, int flags,
				const char *dev_name, void *data)
{
	return mount_bdev(fs_type, flags, dev_name, data, emmcfs_fill_super);
}
#endif

/*
 * Structure of the eMMCFS filesystem type
 */
static struct file_system_type emmcfs_fs_type = {
	.owner		= THIS_MODULE,
	.name		= "emmcfs",
#if LINUX_VERSION_CODE == KERNEL_VERSION(2, 6, 35)
	.get_sb		= emmcfs_get_sb,
#elif LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 20) || \
		LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 33)
	.mount		= emmcfs_mount,
#else
	BUILD_EMMCFS_BUG();
#endif
	.kill_sb	= kill_block_super,
	.fs_flags	= FS_REQUIRES_DEV,
};

/**
 * @brief				Inode storage initializer.
 * @param [in,out] 	generic_inode	Inode for init
 * @return		void
 */
static void emmcfs_init_inode_once(void *generic_inode)
{
	struct emmcfs_inode_info *inode = generic_inode;

	mutex_init(&inode->truncate_mutex);
	inode_init_once(&inode->vfs_inode);
}

/**
 * @brief	Initialization of the eMMCFS module.
 * @return	Returns 0 on success, errno on failure
 */
static int __init init_emmcfs_fs(void)
{
	int ret;

	emmcfs_inode_cachep = kmem_cache_create("emmcfs_icache",
				sizeof(struct emmcfs_inode_info), 0,
				SLAB_HWCACHE_ALIGN, emmcfs_init_inode_once);
	if (!emmcfs_inode_cachep) {
		EMMCFS_ERR("failed to initialise inode cache\n");
		return -ENOMEM;
	}

	ret = emmcfs_exttree_cache_init();
	if (ret)
		goto fail_create_exttree_cache;

	ret = emmcfs_fsm_cache_init();

	if (ret)
		goto fail_create_fsm_cache;

	ret = register_filesystem(&emmcfs_fs_type);
	if (ret)
		goto failed_register_fs;

	ret = emmcfs_dir_init();
	if (ret)
		goto failed_emmcfs_proc_init;

	ret = emmcfs_init_sb_list();
	if(ret)
		goto failed_init_sb_list;

	return 0;

failed_init_sb_list:
	emmcfs_fsm_cache_destroy();
fail_create_fsm_cache:
fail_create_exttree_cache:
	emmcfs_exttree_cache_destroy();
failed_emmcfs_proc_init:

failed_register_fs:

	return ret;
}

/**
 * @brief		Module unload callback.
 * @return	void
 */
static void __exit exit_emmcfs_fs(void)
{
	emmcfs_dir_exit();
	emmcfs_fsm_cache_destroy();
	unregister_filesystem(&emmcfs_fs_type);
	kmem_cache_destroy(emmcfs_inode_cachep);
	emmcfs_exttree_cache_destroy();
}

module_init(init_emmcfs_fs)
module_exit(exit_emmcfs_fs)

module_param(debug_mask, uint, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(debug_mask, "Debug mask (1 - print debug for superblock ops,"\
				" 2 - print debug for inode ops)");
MODULE_AUTHOR("Samsung Electronics SRC CSG File System Part");
MODULE_VERSION(__stringify(EMMCFS_VERSION));
MODULE_DESCRIPTION("Samsung eMMC chip oriented File System");
MODULE_LICENSE("Samsung, Proprietary");
