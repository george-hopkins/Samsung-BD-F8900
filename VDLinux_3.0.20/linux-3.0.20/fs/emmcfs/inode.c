/**
 * @file	fs/emmcfs/inode.c
 * @brief	Basic inode operations.
 * @date	01/17/2012
 *
 * eMMCFS -- Samsung eMMC chip oriented File System, Version 1.
 *
 * This file implements inode operations and its related functions.
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

#include <linux/slab.h>
#include <linux/buffer_head.h>
#include <linux/writeback.h>
#include <linux/nls.h>
#include <linux/mpage.h>
#include <linux/version.h>
#include <linux/migrate.h>

#include "emmcfs.h"
#include "debug.h"

#define EMMCFS_CAT_MAX_NAME	255

#define EMMCFS_LOG_BITS_PER_PAGE (3 + PAGE_SHIFT)
#define EMMCFS_BITMAP_PAGE_MASK (((__u64) 1 << EMMCFS_LOG_BITS_PER_PAGE) - 1)

#define EMMCFS_LOG_HLINKS_PER_PAGE 2
#define EMMCFS_LOG_ONE_HLINK_DATA_SZ (PAGE_SHIFT - EMMCFS_LOG_HLINKS_PER_PAGE)
#define EMMCFS_HLINK_IN_PAGE_MASK (((hlink_id_t) \
			1 << EMMCFS_LOG_HLINKS_PER_PAGE) - 1)

/* For testing purpose use fake page cash size */
/*#define EMMCFS_PAGE_SHIFT 5*/
/*#define EMMCFS_BITMAP_PAGE_SHIFT (3 + EMMCFS_PAGE_SHIFT)*/
/*#define EMMCFS_BITMAP_PAGE_MASK (((__u64) 1 << EMMCFS_BITMAP_PAGE_SHIFT)\
 * - 1)*/

/**
 * @brief 		Create inode.
 * @param [out]	dir 	The inode to be created
 * @param [in] 	dentry 	Struct dentry with information
 * @param [in] 	mode 	Mode of creation
 * @param [in] 	nd 	Struct with name data
 * @return 		Returns 0 on success, errno on failure
 */
static int emmcfs_create(struct inode *dir, struct dentry *dentry, int mode,
		struct nameidata *nd);

/**
 * @brief 		Write inode to bnode.
 * @param [in] 	inode 	The inode to be written to bnode
 * @return 		Returns 0 on success, errno on failure
 */
static int emmcfs_write_inode_to_bnode(struct inode *inode);

/**
 * @brief 		Allocate new inode.
 * @param [in] 	dir 	Parent directory
 * @param [in] 	mode 	Mode of operation
 * @return 		Returns pointer to newly created inode on success,
 * 			errno on failure
 */
static struct inode *emmcfs_new_inode(struct inode *dir, int mode);

/**
 * @brief 		Get root folder.
 * @param [in] 	sbi 	Pointer to superblock information
 * @param [out] fd 	Buffer for finded data
 * @return 		Returns 0 on success, errno on failure
 */
static int get_root_folder(struct emmcfs_sb_info *sbi,
		struct emmcfs_find_data *fd)
{
	int ret;

	EMMCFS_LOG_FUNCTION_START(sbi);
	ret = emmcfs_cattree_find(sbi, EMMCFS_ROOTDIR_OBJ_ID,
		EMMCFS_ROOTDIR_NAME, sizeof(EMMCFS_ROOTDIR_NAME) - 1, fd,
		EMMCFS_BNODE_MODE_RO);
	EMMCFS_LOG_FUNCTION_END(sbi, ret);

	return ret;
}

/**
 * @brief		Checks and copy layout fork into run time fork.
 * @param [out]	inode	The inode for appropriate run time fork
 * @param [in] 	lfork	Layout fork
 * @return		Returns 0 on success, errno on failure
 */
int emmcfs_parse_fork(struct inode *inode, struct emmcfs_fork *lfork)
{
	struct emmcfs_fork_info *ifork = &EMMCFS_I(inode)->fork;
	struct emmcfs_sb_info *sbi = inode->i_sb->s_fs_info;
	u64 i_size_64_fork;
	EMMCFS_LOG_FUNCTION_START(sbi);
	if (!is_fork_valid(lfork)) {
		EMMCFS_LOG_FUNCTION_END(sbi, -EINVAL);
		return -EINVAL;
	}

	inode->i_size = le64_to_cpu(lfork->size_in_bytes);
	if (S_ISCHR(inode->i_mode) || S_ISBLK(inode->i_mode)) {
		inode->i_rdev = inode->i_size;
		inode->i_size = 0;
	}

	ifork->total_block_count = le32_to_cpu(lfork->total_blocks_count);

	ifork->prealloc_block_count = 0;
	ifork->prealloc_start_block = 0;

	/* VFS expects i_blocks value in sectors*/
	inode->i_blocks = ifork->total_block_count <<
			(sbi->block_size_shift - 9);

	/* TODO: remove this when we start supporting embedding file data into
	 * the fork */
	i_size_64_fork = ifork->total_block_count;
	i_size_64_fork <<= sbi->block_size_shift;
	if (i_size_64_fork < inode->i_size) {
		EMMCFS_ERR("bad fork (blocks = %u, bytes = %llu)",
				ifork->total_block_count, inode->i_size);
		EMMCFS_LOG_FUNCTION_END(sbi, -EINVAL);
		return -EINVAL;
	}

	emmcfs_lfork_to_rfork(lfork, ifork);
	EMMCFS_LOG_FUNCTION_END(sbi, 0);
	return 0;
}

/**
 * @brief		Form layout fork from run time fork.
 * @param [out]	inode	The inode for appropriate run time fork
 * @param [in]	lfork	Layout fork
 * @return	void
 */
void emmcfs_form_fork(struct emmcfs_fork *lfork, struct inode *inode)
{
	struct emmcfs_fork_info *ifork = &(EMMCFS_I(inode)->fork);
	struct emmcfs_sb_info *sbi = inode->i_sb->s_fs_info;
	unsigned i;

	EMMCFS_LOG_FUNCTION_START(sbi);
	memset(lfork, 0, sizeof(struct emmcfs_fork));

	if (S_ISCHR(inode->i_mode) || S_ISBLK(inode->i_mode))
		lfork->size_in_bytes = cpu_to_le64(inode->i_rdev);
	else {
		lfork->size_in_bytes = cpu_to_le64(inode->i_size);
		lfork->total_blocks_count = cpu_to_le32(
			(inode->i_size >> sbi->block_size_shift) +
			/* (inode->i_size % sbi->block_size) != 0
			 * - more readable */
			((inode->i_size &
				((1 << sbi->block_size_shift) - 1)) != 0));

		for (i = 0; i < ifork->used_extents; ++i) {
			struct emmcfs_extent *lextent;

			lextent = &lfork->extents[i];
			lextent->begin = cpu_to_le64(
				ifork->extents[i].first_block);
			lextent->length = cpu_to_le32(
				ifork->extents[i].block_count);
		}
	}
	lfork->magic = EMMCFS_FORK_MAGIC;
	EMMCFS_LOG_FUNCTION_END(sbi, 0);
}

/**
 * @brief 			Get entry type (file/folder).
 * @param [in] 	rec_type	Record type
 * @return 			Returns entry type
 */
static unsigned char get_entry_type(int rec_type)
{
	if (rec_type & EMMCFS_CATALOG_FILE_RECORD) {
		if (rec_type & EMMCFS_CATALOG_FILE_RECORD_SYMLINK)
			return DT_LNK;
		else
			return DT_REG;
	} else if (rec_type == EMMCFS_CATALOG_FOLDER_RECORD) {
		return DT_DIR;
	} else {
		EMMCFS_BUG();
		return DT_UNKNOWN;
	}
}

/**
 * @brief 			Read page, if absent create it.
 * @param [in] 	inode 		Pointer to inode information
 * @param [in] 	page_index 	Page index
 * @return 			Returns pointer to page
 */
static struct page *emmcfs_read_create_page(struct inode *inode, pgoff_t page_index)
{
	struct emmcfs_sb_info *sbi = EMMCFS_SB(inode->i_sb);
	u32 block_index = page_index << (PAGE_SHIFT - sbi->block_size_shift);
	struct emmcfs_inode_info *inode_i = EMMCFS_I(inode);
	struct page *rvalue = NULL;

	if (block_index >= inode_i->fork.total_block_count)
		rvalue = emmcfs_alloc_new_page(inode->i_mapping, page_index);
	else
		rvalue = read_mapping_page(inode->i_mapping, page_index, NULL);

	return rvalue;
}

/**
 * @brief 			Get hard link value.
 * @param [in] 	hlink_area_ino 	The inode with hardlink
 * @param [in] 	hlink_id 	Hardlink id
 * @param [in] 	page_offset 	Page offset
 * @return 			Returns pointer to page
 */
static inline struct page *get_hlink_value_page(struct inode *hlink_area_ino,
		hlink_id_t hlink_id, u16 *page_offset)
{
	/*  Binary representation of hlink_id:
	 *
	 *  bit_n    ...14 13 12|11 10  9  8  7  6  5  4  3  2| 1  0
	 *  val    chunk number |   page in chunk             |pos in page
	 */
	pgoff_t page_index;
	int chunk_n = hlink_id >> EMMCFS_LOG_BITS_PER_PAGE;

	BUILD_BUG_ON(PAGE_SHIFT < EMMCFS_LOG_ONE_HLINK_DATA_SZ);

	/* First page in the chunk */
	page_index = (chunk_n << (EMMCFS_LOG_BITS_PER_PAGE -
				EMMCFS_LOG_HLINKS_PER_PAGE)) + chunk_n;
	/* First data page in the chunk (next page after bitmap) */
	page_index++;

	/* Add offset to the goal data page in the chunk */
	page_index += (hlink_id & EMMCFS_BITMAP_PAGE_MASK) >>
		EMMCFS_LOG_HLINKS_PER_PAGE;

	/* Number of the hlink inside the page */
	*page_offset = hlink_id & EMMCFS_HLINK_IN_PAGE_MASK;
	/* Offset to the hlink in bytes */
	*page_offset <<= EMMCFS_LOG_ONE_HLINK_DATA_SZ;

	return emmcfs_read_create_page(hlink_area_ino, page_index);
}

/**
 * @brief 			Get parameters of hard link.
 * @param [out]	hlinks_inode  	The inodes array to be filled
 * @param [in] 	hlink_id  	Hardlink id
 * @param [out]	obj_id 		Pointer to object ID
 * @param [out]	rec_type 	Pointer to record type
 * @return 			Returns 0 on success, errno on failure
 */
static inline int get_params_on_hlink_id(struct inode *hlinks_inode,
		hlink_id_t hlink_id, u64 *obj_id, int *rec_type)
{
	u16 page_offset;
	struct page *page;
	struct emmcfs_catalog_folder_record *value_area;
	int ret = 0;

	EMMCFS_LOG_FUNCTION_START((EMMCFS_SB(hlinks_inode->i_sb)));
	page = get_hlink_value_page(hlinks_inode, hlink_id, &page_offset);
	if(IS_ERR(page)) {
		ret = PTR_ERR(page);
		EMMCFS_LOG_FUNCTION_END((EMMCFS_SB(hlinks_inode->i_sb)), ret);
		return ret;
	}
	value_area = kmap(page) + page_offset;

	*obj_id = le64_to_cpu(value_area->object_id);
	*rec_type = le16_to_cpu(value_area->record_type);
	kunmap(page);

	EMMCFS_LOG_FUNCTION_END((EMMCFS_SB(hlinks_inode->i_sb)), ret);
	return ret;
}

/**
 * @brief 			Free hardlink data.
 * @param [in] 	sbi 		Pointer to superblock information
 * @param [in] 	hlink_id_t 	Hardlink type
 * @param [in] 	hlink_id  	Hardlink id
 * @return 			Returns 0 on success, errno on failure
 */
static int free_hlink_data(struct emmcfs_sb_info *sbi, hlink_id_t hlink_id)
{
	int chunk_n = hlink_id >> EMMCFS_LOG_BITS_PER_PAGE;
	int page_index = (chunk_n <<
			(EMMCFS_LOG_BITS_PER_PAGE - EMMCFS_LOG_HLINKS_PER_PAGE))
			+ chunk_n;
	hlink_id_t id_offset;
	struct page *page;
	void *data;
	int err = 0;


	EMMCFS_LOG_FUNCTION_START(sbi);
	id_offset = ((hlink_id_t) chunk_n) << EMMCFS_LOG_BITS_PER_PAGE;
	page = emmcfs_read_create_page(sbi->hard_links.inode, page_index);

	if (IS_ERR(page)) {
		EMMCFS_LOG_FUNCTION_END(sbi, PTR_ERR(page));
		return PTR_ERR(page);
	}

	err = EMMCFS_ADD_CHUNK(sbi->hard_links.inode, &page, 1);
	if (err)
		goto exit;

	data = kmap(page);
	if (!test_and_clear_bit(hlink_id - id_offset, data)) {
		printk(KERN_ERR "hlink_id %llu\nid_offset %llu\n"
			"chunk_n %d\npage_index %d",
			hlink_id, id_offset, chunk_n, page_index);
		EMMCFS_BUG();
	}

	if (sbi->hard_links.last_used_id > hlink_id)
		sbi->hard_links.last_used_id = hlink_id;

	set_page_dirty_lock(page);

exit:
	kunmap(page);
	page_cache_release(page);
	EMMCFS_LOG_FUNCTION_END(sbi, 0);

	return err;
}

/**
 * @brief		Method to read (list) directory.
 * @param [in] 	filp	File pointer
 * @param [in] 	dirent	Directory entry
 * @param [in] 	filldir	Callback filldir for kernel
 * @return		Returns count of files/dirs
 */
static int emmcfs_readdir(struct file *filp, void *dirent, filldir_t filldir)
{
	struct inode			*inode = filp->f_path.dentry->d_inode;
	struct dentry			*dentry = filp->f_dentry;
	int				ret = 0;
	unsigned int			str_len;
	char				str[2*EMMCFS_CAT_MAX_NAME+1];
	struct emmcfs_sb_info *sbi = inode->i_sb->s_fs_info;

	int pos = -1;
	struct emmcfs_bnode *bnode = NULL;
	struct emmcfs_cattree_key *key;


	EMMCFS_LOG_FUNCTION_START(sbi);
	switch (filp->f_pos) {
	case 0:
		if (filldir(dirent, ".", 1, filp->f_pos++, inode->i_ino,
					DT_DIR))
			goto exit_noput;
		/* fall through */
	case 1:
		if (filldir(dirent, "..", 2, filp->f_pos++,
			dentry->d_parent->d_inode->i_ino, DT_DIR))
			goto exit_noput;
		break;
	default:
		if (!filp->private_data) {
			EMMCFS_LOG_FUNCTION_END(sbi, 0);
			return 0;
		}
		break;
	}

	EMMCFS_DEBUG_MUTEX("cattree mutex r lock");
	mutex_r_lock(sbi->catalog_tree->rw_tree_lock);
	EMMCFS_DEBUG_MUTEX("cattree mutex r lock succ");
	if (!filp->private_data) {
		bnode = emmcfs_find_first_catalog_child(inode, &pos);
	} else {
		key = filp->private_data;
		bnode = emmcfs_btree_find(sbi->catalog_tree, &key->gen_key,
				&pos, EMMCFS_BNODE_MODE_RO);
	}

	if (IS_ERR(bnode)) {
		ret = PTR_ERR(bnode);
		goto fail;
	}

	key = emmcfs_get_btree_record(bnode, pos);
	if (IS_ERR(key)) {
		ret = PTR_ERR(key);
		goto fail;
	}

	while (1) {
		unsigned char d_type = DT_UNKNOWN;
		__u64 obj_id = 0;
		struct emmcfs_catalog_folder_record *cattree_val;
		int rec_type;

		if (key->parent_id != cpu_to_le64(inode->i_ino))
			goto exit;

		EMMCFS_BUG_ON(!key);
		cattree_val = get_value_pointer(key);
		rec_type = cattree_val->record_type;
		if (rec_type != EMMCFS_CATALOG_HLINK_RECORD) {
			obj_id = le64_to_cpu(
				((struct emmcfs_catalog_folder_record *)
						cattree_val)->object_id);
		} else {
			hlink_id_t hlink_id = le64_to_cpu(
			((struct emmcfs_catalog_hlink_record *)cattree_val)->
				hlink_id);

			ret = get_params_on_hlink_id(sbi->hard_links.inode, hlink_id,
					&obj_id, &rec_type);
			if (ret)
				goto fail;
		}

		str_len = min(key->name.length,
			(unsigned) 2 * EMMCFS_CAT_MAX_NAME);
		memcpy(str, key->name.unicode_str,
			min(key->name.length,
			(unsigned) 2 * EMMCFS_CAT_MAX_NAME));
		str[str_len] = 0;

		d_type = get_entry_type(rec_type);
		ret = filldir(dirent, str, str_len,
				filp->f_pos, obj_id, d_type);

		if (ret) {
			/* Buffer is over, there will be another call of
			 * the read_dir */
			struct emmcfs_cattree_key *prev_key =
				filp->private_data;

			/* Check buffer for key is too small, or there is
			 * no buffer at all */
			if (!prev_key || prev_key->gen_key.key_len <=
					key->gen_key.key_len) {
				kfree(filp->private_data);
				filp->private_data = kmalloc(
						key->gen_key.key_len,
						GFP_KERNEL);
				if (!filp->private_data) {
					ret = -ENOMEM;
					goto fail;
				}
			}

			/* Store next key for next run */
			memcpy(filp->private_data, key, key->gen_key.key_len);
			ret = 0;
			goto exit;
		}

		key = emmcfs_get_next_btree_record(&bnode, &pos);

		if ( PTR_ERR(key) == -ENOENT ||
				key->parent_id != cpu_to_le64(inode->i_ino)) {
			kfree(filp->private_data);
			filp->private_data = NULL;
			break;
		} else if (IS_ERR(key)) {
			ret = PTR_ERR(key);
			goto fail;
		}
	}

exit:
	emmcfs_put_bnode(bnode);
exit_noput:
	EMMCFS_DEBUG_MUTEX("cattree mutex r lock un");
	mutex_r_unlock(sbi->catalog_tree->rw_tree_lock);
	EMMCFS_LOG_FUNCTION_END(sbi, ret);
	return ret;
fail:
	EMMCFS_DEBUG_INO("finished with err (%d)", ret);
	if (!IS_ERR(bnode))
		emmcfs_put_bnode(bnode);
	EMMCFS_DEBUG_MUTEX("cattree mutex r lock un");
	mutex_r_unlock(sbi->catalog_tree->rw_tree_lock);
	EMMCFS_LOG_FUNCTION_END(sbi, ret);
	return ret;
}

/**
 * @brief		Method to look up an entry in a directory.
 * @param [in] 	dir	Parent directory
 * @param [in] 	dentry	Searching entry
 * @param [in] 	nd	Associated nameidata
 * @return		Returns pointer to found dentry, NULL if it is
 *	 		not found, ERR_PTR(errno) on failure
 */
static struct dentry *emmcfs_lookup(struct inode *dir, struct dentry *dentry,
					struct nameidata *nd)
{
	struct super_block		*sb = dir->i_sb;
	struct emmcfs_sb_info		*sbi = sb->s_fs_info;
	struct emmcfs_cattree_key	*key = NULL, *found_key;
	struct inode			*found_inode = NULL;
	struct emmcfs_bnode		*bnode;
	struct emmcfs_find_data		fdat = {NULL, 0, 0};
	struct emmcfs_catalog_folder_record *cattree_val;
	__u64 obj_id = 0;

	EMMCFS_LOG_FUNCTION_START(sbi);

	if (dentry->d_name.len > EMMCFS_CAT_MAX_NAME) {
		EMMCFS_LOG_FUNCTION_END(sbi, -ENAMETOOLONG);
		return ERR_PTR(-ENAMETOOLONG);
	}
	if (!S_ISDIR(dir->i_mode)) {
		EMMCFS_LOG_FUNCTION_END(sbi, -ENOTDIR);
		return ERR_PTR(-ENOTDIR);
	}

	key = kzalloc(sizeof(*key), GFP_KERNEL);
	if (!key) {
		EMMCFS_LOG_FUNCTION_END(sbi, -ENOMEM);
		return ERR_PTR(-ENOMEM);
	}

	key->parent_id = cpu_to_le64(dir->i_ino);
	key->name.length = min(dentry->d_name.len,
			(unsigned int)EMMCFS_UNICODE_STRING_MAX_LEN+1);
	memcpy(key->name.unicode_str, dentry->d_name.name,
			min(dentry->d_name.len,
			(unsigned int)EMMCFS_UNICODE_STRING_MAX_LEN+1));

	EMMCFS_DEBUG_MUTEX("cattree mutex r lock");
	mutex_r_lock(sbi->catalog_tree->rw_tree_lock);
	EMMCFS_DEBUG_MUTEX("cattree mutex r lock succ");
	bnode = emmcfs_btree_find(sbi->catalog_tree, &key->gen_key, &fdat.pos,
			EMMCFS_BNODE_MODE_RO);

	if (IS_ERR(bnode)) {
		EMMCFS_LOG_FUNCTION_END(sbi, PTR_ERR(bnode));
		return (struct dentry *)bnode;
	}

	found_key = emmcfs_get_btree_record(bnode, fdat.pos);
	if (IS_ERR(found_key)) {
		EMMCFS_LOG_FUNCTION_END(sbi, PTR_ERR(found_key));
		return (struct dentry *)found_key;
	}

	if (sbi->catalog_tree->comp_fn(&found_key->gen_key, &key->gen_key)) {
		emmcfs_put_bnode(bnode);
		kfree(key);
		goto splice;
	}

	cattree_val = get_value_pointer(found_key);
	if (cattree_val->record_type != EMMCFS_CATALOG_HLINK_RECORD) {
		obj_id = le64_to_cpu(cattree_val->object_id);
	} else {
		int rec_type;
		hlink_id_t hlink_id = le64_to_cpu(
			((struct emmcfs_catalog_hlink_record *)cattree_val)->
				hlink_id);
		int ret;
		ret = get_params_on_hlink_id(sbi->hard_links.inode,
			hlink_id, &obj_id, &rec_type);
		if (ret) {
			EMMCFS_LOG_FUNCTION_END(sbi, ret);
			return ERR_PTR(ret);
		}
	}

	fdat.bnode_id = le32_to_cpu(EMMCFS_BNODE_DSCR(bnode)->node_id);
	fdat.bnode = bnode;
	found_inode = emmcfs_iget(sb, obj_id, &fdat);
	emmcfs_put_bnode(bnode);

	kfree(key);
	key = NULL;
	if (IS_ERR(found_inode)) {
		EMMCFS_DEBUG_MUTEX("cattree mutex r lock un");
		mutex_r_unlock(sbi->catalog_tree->rw_tree_lock);
		EMMCFS_LOG_FUNCTION_END(sbi, 0);
		return NULL;
	}
splice:
	EMMCFS_DEBUG_MUTEX("cattree mutex r lock un");
	mutex_r_unlock(sbi->catalog_tree->rw_tree_lock);

	EMMCFS_LOG_FUNCTION_END(sbi, 0);
	return d_splice_alias(found_inode, dentry);
}

/**
 * @brief 		Get free inode.
 * @param [in]	sbi 	Pointer to superblock information
 * @param [out] i_ino	Resulting inode number
 * @return 		Returns 0 if success, err code if fault
 */
static int get_free_inode(struct emmcfs_sb_info *sbi, ino_t *i_ino)
{
	struct page *page;
	void *data;
	__u64 last_used = atomic64_read(&sbi->free_inode_bitmap.last_used);
	unsigned int page_index = last_used >> EMMCFS_LOG_BITS_PER_PAGE;
	unsigned int id_offset = last_used & ~EMMCFS_BITMAP_PAGE_MASK;
	__u64 find_from = last_used - id_offset;
	int err = 0;

	EMMCFS_LOG_FUNCTION_START(sbi);

	*i_ino = 0;
	while (*i_ino == 0) {
		page = emmcfs_read_create_page(sbi->free_inode_bitmap.inode,
				page_index);

		if (IS_ERR(page)) {
			EMMCFS_LOG_FUNCTION_END(sbi, PTR_ERR(page));
			return PTR_ERR(page);
		}

		/* TODO optimize later */

		err = EMMCFS_ADD_CHUNK(sbi->free_inode_bitmap.inode, &page, 1);
		if (err) {
			page_cache_release(page);
			return err;
		}

		lock_page(page);
		data = kmap(page);

		*i_ino = find_next_zero_bit(data, 1 << EMMCFS_LOG_BITS_PER_PAGE,
				find_from);

		if (*i_ino < (1 << EMMCFS_LOG_BITS_PER_PAGE)) {
			EMMCFS_BUG_ON(*i_ino + id_offset < EMMCFS_1ST_FILE_INO);
			EMMCFS_BUG_ON(test_and_set_bit(*i_ino, data));

			*i_ino += id_offset;
			/* Using atomic_t siplifies algorithm, but there is
			 * possibility that last_used had been already changed
			 * here. This can produce holes in inode bitmap, which
			 * is not greate problem: those object-ids will be
			 * used after remount or after end of bitmap */
			atomic64_set(&sbi->free_inode_bitmap.last_used, *i_ino);
			set_page_dirty(page);
		} else {
			*i_ino = 0;
		}

		kunmap(page);
		unlock_page(page);
		page_cache_release(page);

		page_index++;
		id_offset = page_index << EMMCFS_LOG_BITS_PER_PAGE;
		find_from = 0;
	}

	EMMCFS_LOG_FUNCTION_END(sbi, *i_ino);
	return err;
}

/**
 * @brief 		Free several inodes.
 * @param [in] 	sbi 	Superblock information
 * @param [in] 	inode_n	Number of inodes to be free
 * @return 		Returns error code
 */
int emmcfs_free_inode_n(struct emmcfs_sb_info *sbi, __u64 inode_n)
{
	unsigned int page_index = inode_n >> EMMCFS_LOG_BITS_PER_PAGE;
	unsigned int id_offset = inode_n & ~EMMCFS_BITMAP_PAGE_MASK;
	void *data;
	struct page *page;
	int err = 0;


	EMMCFS_LOG_FUNCTION_START(sbi);
	page = emmcfs_read_create_page(sbi->free_inode_bitmap.inode,
				page_index);

	if (IS_ERR(page)) {
		EMMCFS_LOG_FUNCTION_END(sbi, PTR_ERR(page));
		return PTR_ERR(page);
	}

	err = EMMCFS_ADD_CHUNK(sbi->free_inode_bitmap.inode, &page, 1);
	if (err) {
		page_cache_release(page);
		return err;
	}

	lock_page(page);
	data = kmap(page);

	if (!test_and_clear_bit(inode_n - id_offset, data)) {
		printk(KERN_INFO " emmcfs_free_inode_n %llu", inode_n);
		EMMCFS_BUG();
	}


	if (atomic64_read(&sbi->free_inode_bitmap.last_used) > inode_n)
		atomic64_set(&sbi->free_inode_bitmap.last_used, inode_n);

	set_page_dirty(page);
	kunmap(page);
	unlock_page(page);
	page_cache_release(page);
	EMMCFS_LOG_FUNCTION_END(sbi, 0);
	return 0;
}

/**
 * @brief 		Unlink function.
 * @param [in] 	dir 	Pointer to inode
 * @param [in] 	dentry 	Pointer to directory entry
 * @return 		Returns error codes
 */
static int emmcfs_unlink(struct inode *dir, struct dentry *dentry)
{
	struct inode			*inode = dentry->d_inode;
	struct super_block		*sb = inode->i_sb;
	struct emmcfs_sb_info		*sbi = sb->s_fs_info;
	int				ret = 0;
	struct emmcfs_cattree_key *rm_key;

	EMMCFS_LOG_FUNCTION_START(sbi);

	rm_key = emmcfs_alloc_cattree_key(dentry->d_name.len, 0);
	if (IS_ERR(rm_key)) {
		EMMCFS_LOG_FUNCTION_END(sbi, PTR_ERR(rm_key));
		return PTR_ERR(rm_key);
	}

	EMMCFS_DEBUG_INO("remove '%s', ino = %lu", dentry->d_iname,
			inode->i_ino);

	EMMCFS_START_TRANSACTION(sbi);

	emmcfs_fill_cattree_key(rm_key, dir->i_ino, &dentry->d_name);
	EMMCFS_DEBUG_MUTEX("cattree mutex w lock");
	mutex_w_lock(sbi->catalog_tree->rw_tree_lock);
	EMMCFS_DEBUG_MUTEX("cattree mutex w lock succ");

	ret = emmcfs_btree_remove(sbi->catalog_tree, &rm_key->gen_key);

	kfree(rm_key);
	if (ret) {
		EMMCFS_DEBUG_MUTEX("cattree mutex w lock un");
		mutex_w_unlock(sbi->catalog_tree->rw_tree_lock);
		goto exit;
	}

	if (dir->i_size != 0)
		dir->i_size--;
	else
		EMMCFS_DEBUG_INO("Files count mismatch");

	dir->i_ctime = emmcfs_current_time(dir);
	dir->i_mtime = emmcfs_current_time(dir);
	inode_dec_link_count(inode);

	EMMCFS_DEBUG_MUTEX("cattree mutex w lock un");
	mutex_w_unlock(sbi->catalog_tree->rw_tree_lock);

	emmcfs_write_inode_to_bnode(dir);
	if (inode->i_nlink) {
		inode->i_ctime = emmcfs_current_time(dir);
		emmcfs_write_inode_to_bnode(inode);
		goto exit;
	}

	/* Now i_nlink == 0, completely delete inode */
	if (EMMCFS_I(inode)->hlink_id != INVALID_HLINK_ID) {
		/* Object is hardlink */
		ret = free_hlink_data(sbi, EMMCFS_I(inode)->hlink_id);
		if (ret)
			goto exit;
	}

	inode->i_size = 0;
	remove_inode_hash(dentry->d_inode);

	if (S_ISREG(inode->i_mode) || S_ISLNK(inode->i_mode)) {
		EMMCFS_DEBUG_MUTEX("truncate mutex lock");
		mutex_lock(&EMMCFS_I(inode)->truncate_mutex);
		EMMCFS_DEBUG_MUTEX("truncate mutex lock success");
		ret = emmcfs_fsm_free_runtime_fork(inode);
		EMMCFS_DEBUG_MUTEX("truncate mutex unlock");
		mutex_unlock(&EMMCFS_I(inode)->truncate_mutex);
		if (!EMMCFS_I(inode)->fork.total_block_count || ret)
			goto exit;
	} else
		EMMCFS_BUG_ON(emmcfs_free_inode_n(EMMCFS_SB(inode->i_sb),
					inode->i_ino));

	EMMCFS_STOP_TRANSACTION(sbi);

	if (S_ISREG(inode->i_mode) && EMMCFS_I(inode)->fork.total_block_count)
		EMMCFS_BUG_ON(emmcfs_fsm_free_exttree(EMMCFS_SB(inode->i_sb),
					inode->i_ino));
	EMMCFS_LOG_FUNCTION_END(sbi, ret);
	return ret;
exit:
	EMMCFS_STOP_TRANSACTION(sbi);
	EMMCFS_LOG_FUNCTION_END(sbi, ret);
	return ret;
}

/**
 * @brief			This function expands file fork.
 * @param [in] 	inode_info	Pointer to inode_info structure.
 * @param [in] 	bcnt		Expansion step in blocks.
 * @param [in] 	result		Physical block number of the first of the
 *				allocated blocks.
 * @return			Returns 0 on success, error code on failure.
 */
static int expand_fork(struct emmcfs_inode_info *inode_info, sector_t bcnt,
		sector_t *result)
{
	struct emmcfs_fork_info *fork = &inode_info->fork;
	struct emmcfs_fsm_info *fsm =
			EMMCFS_SB(inode_info->vfs_inode.i_sb)->fsm_info;

	if ((fork->total_block_count == 0) && (fork->used_extents == 0)) {
		if (((fsm->free_space_start + bcnt) >> (PAGE_SHIFT + 3)) >=
			fsm->page_count)
			return -ENOSPC;
		*result = emmcfs_fsm_get_free_block(inode_info,
			fsm->free_space_start, bcnt);
		if (*result == 0)
			return -ENOSPC;
		fork->used_extents = 1;
		fork->extents[0].first_block = *result;
		fork->extents[0].block_count = bcnt;
	} else {
		struct emmcfs_sb_info *sbi =
			EMMCFS_SB(inode_info->vfs_inode.i_sb);
		sector_t offset =
			fork->extents[fork->used_extents-1].first_block +
			fork->extents[fork->used_extents-1].block_count;

		if ((offset + bcnt) >= (sbi->total_leb_count
					<< sbi->log_blocks_in_leb))
			return -ENOSPC;

		if (((offset + bcnt) >> (PAGE_SHIFT + 3)) >= fsm->page_count)
			return -ENOSPC;

		*result = emmcfs_fsm_get_free_block(inode_info, offset, bcnt);
		if (*result == 0)
			return -ENOSPC;
		if (*result == offset)
			fork->extents[fork->used_extents-1].block_count += bcnt;
		else {
			if (fork->used_extents >=
				EMMCFS_EXTENTS_COUNT_IN_FORK) {
				EMMCFS_BUG_ON(emmcfs_fsm_put_free_block(
							inode_info,
							*result, bcnt));
				return -EPERM;
			}
			fork->used_extents++;
			fork->extents[fork->used_extents-1].first_block =
				*result;
			fork->extents[fork->used_extents-1].block_count = bcnt;
		}
	}
	fork->total_block_count += bcnt;
	return 0;
}

/**
 * @brief			Logical to physical block numbers translation
 * 				for non yet allocated blocks (FSM call).
 * @param [in] 	inode_info	Pointer to inode_info structure.
 * @param [in] 	fsm		Pointer to fsm_info structure.
 * @param [in] 	iblock		Requested logical block number.
 * @param [in] 	blk_cnt		Requested logical blocks count.
 * @return			Returns physical block number, 0 if ENOSPC
 */
static sector_t find_new_block(struct emmcfs_inode_info *inode_info,
	struct emmcfs_fsm_info *fsm, sector_t iblock, sector_t blk_cnt)
{
	struct emmcfs_fork_info *fork = &inode_info->fork;
	int err;
	sector_t rc = 0;
	sector_t offset = 0;
	sector_t delta = iblock - inode_info->fork.total_block_count;
	int i = 0;

	if (fork->used_extents < EMMCFS_EXTENTS_COUNT_IN_FORK)
		err = expand_fork(inode_info, blk_cnt, &rc);
	else {
		for (i = 0; i < EMMCFS_EXTENTS_COUNT_IN_FORK; i++)
			offset += fork->extents[i].block_count;
		if (offset == fork->total_block_count) {
			err = expand_fork(inode_info, blk_cnt, &rc);
			if (err != -EPERM)
				goto pass;
		}
		err = 0;
		if (inode_info->vfs_inode.i_ino == EMMCFS_EXTENTS_TREE_INO)
			EMMCFS_BUG();
		rc = emmcfs_exttree_add_block(inode_info,
			fork->total_block_count, blk_cnt);
		if (rc)
			fork->total_block_count += blk_cnt;
	}
pass:
	if (err || (rc == 0))
		return 0;
	return rc + delta;
}

/**
 * @brief				Logical to physical block number
 *					translation for already allocated
 *					blocks.
 * @param [in] 		inode_info	Pointer to inode_info structure.
 * @param [in] 		iblock		Requested logical block number
 * @param [in, out] 	max_blocks	Distance (in blocks) from returned
 * 					block to end of extent is returned
 *					through this pointer.
 * @return				Returns physical block number.
 */
sector_t emmcfs_find_old_block(struct emmcfs_inode_info *inode_info,
	sector_t iblock, __u32 *max_blocks)
{
	struct emmcfs_fork_info *fork = &inode_info->fork;
	unsigned int i;
	sector_t iblock_start = iblock;

	for (i = 0; i < fork->used_extents; i++)
		if (iblock < fork->extents[i].block_count) {
			if (max_blocks)
				*max_blocks = fork->extents[i].block_count -
					iblock;
			return fork->extents[i].first_block + iblock;
		}
		else
			iblock -= fork->extents[i].block_count;
	return emmcfs_exttree_get_block(inode_info, iblock_start, max_blocks);
}

/**
 * @brief 			Calculate aligned value.
 * @param [in]	value		Value to align.
 * @param [in]	gran_log2	Alignment granularity log2.
 * @return 			val, aligned up to granularity boundary.
 */
static inline sector_t align_up(sector_t val, int gran_log2)
{
	return (val + (1<<gran_log2) - 1) & ~((1<<gran_log2) - 1);
}

/**
 * @brief 			Expands file.
 * @param [in] 	inode 		Pointer to inode
 * @param [in] 	iblock 		Current sectors count
 * @param [out]	res_block 	Pointer to result block
 * @return 			Returns error codes
 */
static int expand_file(struct inode *inode, sector_t iblock,
	sector_t *res_block)
{
	struct emmcfs_inode_info *inode_info = EMMCFS_I(inode);
	struct emmcfs_sb_info *sbi = EMMCFS_SB(inode->i_sb);
	sector_t expand_min = 1;	/* Minimum allowed expand step*/
	sector_t expand_req = 1;	/* Initial expand request size*/
	sector_t expand_gran_log2 = 0;	/* expand alignment granularity */
	int ret = 0;

	EMMCFS_LOG_FUNCTION_START(sbi);
	expand_min = iblock - inode_info->fork.total_block_count + 1;

	/* Metadata expands with leb granularity */
	if (inode->i_ino < EMMCFS_1ST_FILE_INO)
		expand_gran_log2 = sbi->log_blocks_in_leb;

	/* We can not allow excessive extents tree fragmentation, because
	 * extents tree fragments limited by internal fork extents, so
	 * we must limit extents tree minimum expand step */
	if (inode->i_ino == EMMCFS_EXTENTS_TREE_INO)
		expand_min = TREE_EXPAND_STEP((sbi->total_leb_count) <<
			(sbi->log_blocks_in_leb));

	/* Catalog tree should expands by big chunks if possible */
	if (inode->i_ino == EMMCFS_CAT_TREE_INO)
		expand_req = TREE_EXPAND_STEP((sbi->total_leb_count) <<
			(sbi->log_blocks_in_leb));

	expand_min = align_up(expand_min, expand_gran_log2);
	expand_req = align_up(max(expand_req, expand_min), expand_gran_log2);

	if (expand_min > sbi->free_blocks_count) {
		ret = -ENOSPC;
		goto exit;
	}

	/* Try to allocate blocks by single chunk */
	while (expand_req >= expand_min) {
		*res_block = find_new_block(inode_info, sbi->fsm_info, iblock,
			expand_req);

		if (*res_block)
			break;

		/* If we can not alloc blc_cnt blocks, try alloc half of than
		 * amount */
		expand_req >>= 1;
		if (expand_req < (1<<expand_gran_log2)) {
			ret = -ENOSPC;
			goto exit;
		}
		expand_req = align_up(expand_req, expand_gran_log2);
	}

	if (!*res_block) {
		ret = -ENOSPC;
		goto exit;
	}

	inode_add_bytes(inode, sbi->sb->s_blocksize * expand_req);
	if (inode->i_ino < EMMCFS_1ST_FILE_INO) {
		/* Special file meta info is contained in the superblocks */
		i_size_write(inode, i_size_read(inode) +
			sbi->sb->s_blocksize * expand_req);

		if (test_option(sbi, SNAPSHOT))
			set_sbi_flag(sbi, EXSB_DIRTY_SNAPSHOT);
		else {
			set_sbi_flag(sbi, EXSB_DIRTY);
			sbi->sb->s_dirt = 1;
		}
	} else
		ret = emmcfs_write_inode_to_bnode(inode);
exit:
	EMMCFS_LOG_FUNCTION_END(sbi, ret);
	return ret;
}

/**
 * @brief				Logical to physical block numbers
 * 					translation.
 * @param [in] 		inode		Pointer to inode structure.
 * @param [in] 		iblock		Requested logical block number.
 * @param [in, out]	bh_result	Pointer to buffer_head.
 * @param [in] 		create		"Expand file allowed" flag.
 * @return				Returns physical block number,
 * 					0 if ENOSPC
 */
int emmcfs_get_block(struct inode *inode, sector_t iblock,
	struct buffer_head *bh_result, int create)
{
	struct emmcfs_inode_info *inode_info = EMMCFS_I(inode);
	struct emmcfs_sb_info	*sbi = inode->i_sb->s_fs_info;
	struct super_block *sb = inode->i_sb;
	sector_t res_block = 0;
	int alloc = 0;
	__u32 max_blocks = 1;
	__u32 buffer_size = bh_result->b_size >> sbi->block_size_shift;
	int err = 0;

	EMMCFS_LOG_FUNCTION_START(sbi);
	EMMCFS_DEBUG_MUTEX("truncate mutex lock");
	mutex_lock(&inode_info->truncate_mutex);
	EMMCFS_DEBUG_MUTEX("truncate mutex lock success");

	if (iblock >= inode_info->fork.total_block_count) {
		if (!create)
			goto pass;
		EMMCFS_BUG_ON(inode->i_ino == EMMCFS_LEB_BITMAP_INO);

		err = expand_file(inode, iblock, &res_block);
		if (err)
			goto pass;
		alloc = 1;
	} else {
		res_block = emmcfs_find_old_block(inode_info, iblock,
						  &max_blocks);
		if (res_block == 0) {
			err = -ENOSPC; /*todo ?*/
			goto pass;
		}
	}


	if (res_block > 
		(sb->s_bdev->bd_inode->i_size >> sbi->block_size_shift)) {
		if (!is_sbi_flag_set(sbi, IS_MOUNT_FINISHED)) {
			EMMCFS_ERR("Block beyond block bound requested");
			err = -EFAULT;
			goto pass;
		} else {
			BUG();
		}
	}
	
	clear_buffer_new(bh_result);
	map_bh(bh_result, inode->i_sb, res_block);
	bh_result->b_size = sb->s_blocksize * min(max_blocks, buffer_size);

	if (alloc)
		set_buffer_new(bh_result);
pass:
	EMMCFS_DEBUG_MUTEX("truncate mutex unlock");
	mutex_unlock(&inode_info->truncate_mutex);
	EMMCFS_LOG_FUNCTION_END(sbi, err);
	return err;
}

/**
 * @brief 		Read page function.
 * @param [in] 	file 	Pointer to file structure
 * @param [out]	page 	Pointer to page structure
 * @return 		Returns error codes
 */
static int emmcfs_readpage(struct file *file, struct page *page)
{
	int err = 0;

	err = mpage_readpage(page, emmcfs_get_block);

	return err;
}

/**
 * @brief 			Read multiple pages function.
 * @param [in] 	file 		Pointer to file structure
 * @param [in] 	mapping		Address of pages mapping
 * @param [out]	pages 		Pointer to list with pages
 * param [in] 	nr_pages 	Number of pages
 * @return 			Returns error codes
 */
static int emmcfs_readpages(struct file *file, struct address_space *mapping,
		struct list_head *pages, unsigned nr_pages)
{
	int err = 0;

	err = mpage_readpages(mapping, pages, nr_pages, emmcfs_get_block);

	return err;
}
/**
 * @brief 			Write eMMCFS special file.
 * @param [in] 	sbi 		Pointer to superblock information
 * @param [in] 	file_type	Type of file
 * @return 			Returns error codes
 */
static int emmcfs_write_special_file(struct emmcfs_sb_info *sbi,
		unsigned int file_type)
{
	struct address_space *mapping = NULL;
	struct writeback_control wbc;
	int ret;

	EMMCFS_LOG_FUNCTION_START(sbi);
	switch (file_type) {
	case (EMMCFS_CAT_TREE_INO):
		if (sbi->catalog_tree && sbi->catalog_tree->inode) {
			mapping	= sbi->catalog_tree->inode->i_mapping;
#if defined(CONFIG_EMMCFS_CRC_CHECK)
			emmcfs_sign_dirty_bnodes(mapping, sbi->catalog_tree);
#endif
		} else
			return -EINVAL;
		break;
	case (EMMCFS_LEB_BITMAP_INO):
		if(sbi->fsm_info && sbi->fsm_info->leb_bitmap_inode)
			mapping = sbi->fsm_info->leb_bitmap_inode->i_mapping;
		else
			return -EINVAL;
		break;
	case (EMMCFS_EXTENTS_TREE_INO):
		if(sbi->extents_tree && sbi->extents_tree->inode) {
			mapping = sbi->extents_tree->inode->i_mapping;
#if defined(CONFIG_EMMCFS_CRC_CHECK)
			emmcfs_sign_dirty_bnodes(mapping, sbi->extents_tree);
#endif
		} else
			return -EINVAL;
		break;
	case (EMMCFS_FREE_INODE_BITMAP_INO):
		if(sbi->free_inode_bitmap.inode)
			mapping = sbi->free_inode_bitmap.inode->i_mapping;
		else
			return -EINVAL;
		break;
	case (EMMCFS_HARDLINKS_AREA_INO):
		if(sbi->hard_links.inode)
			mapping = sbi->hard_links.inode->i_mapping;
		else
			return -EINVAL;
		break;
	default:
		EMMCFS_LOG_FUNCTION_END(sbi, -EINVAL);
		return -EINVAL;
		break;
	}

	wbc.sync_mode = WB_SYNC_ALL;
	wbc.nr_to_write = LONG_MAX;
	wbc.range_start = 0;
	wbc.range_end = LLONG_MAX;

	EMMCFS_DEBUG_TRN("ino#%u", file_type);
	ret = emmcfs_mpage_writepages(mapping, &wbc);

	EMMCFS_LOG_FUNCTION_END(sbi, ret);
	return ret;
}

/**
 * @brief 		Sync all metadata.
 * @param [in] 	sbi 	Superblock information
 * @return 		Returns error code
 */
int emmcfs_sync_metadata(struct emmcfs_sb_info *sbi)
{
	int error = 0;
	/* Write ALL dirty meta pages to disk */
	EMMCFS_DEBUG_TRN("");
	EMMCFS_LOG_FUNCTION_START(sbi);
	error = emmcfs_write_special_file(sbi, EMMCFS_CAT_TREE_INO);
	if (error)
		goto err_exit;
	error = emmcfs_write_special_file(sbi, EMMCFS_LEB_BITMAP_INO);
	if (error)
		goto err_exit;
	error = emmcfs_write_special_file(sbi, EMMCFS_EXTENTS_TREE_INO);
	if (error)
		goto err_exit;
	error = emmcfs_write_special_file(sbi, EMMCFS_FREE_INODE_BITMAP_INO);
	if (error)
		goto err_exit;
	error = emmcfs_write_special_file(sbi, EMMCFS_HARDLINKS_AREA_INO);
	if (error)
		goto err_exit;

	EMMCFS_DEBUG_TRN("end");
	EMMCFS_LOG_FUNCTION_END(sbi, 0);
	return 0;
err_exit:

	EMMCFS_DEBUG_TRN("error=%d", error);
	EMMCFS_LOG_FUNCTION_END(sbi, error);
	return error;
}

/**
 * @brief 		Update all metadata.
 * @param [in] 	sbi 	Pointer to superblock information
 * @return 		Returns error codes
 */
static int update_metadata(struct emmcfs_sb_info *sbi)
{
	int error;
#ifdef CONFIG_EMMCFS_POPO_HELPER
	int count = 0;
#endif
	EMMCFS_LOG_FUNCTION_START(sbi);

	down_write(sbi->snapshot_info->transaction_lock);
	EMMCFS_DEBUG_MUTEX("transaction_lock w lock succ");
	sbi->snapshot_info->flags = 1;
	EMMCFS_DEBUG_SNAPSHOT("COMMIT SNAPSHOT");
	error = EMMCFS_COMMIT_SNAPSHOT(sbi);
	if (error) {
		sbi->snapshot_info->flags = 0;
		up_write(sbi->snapshot_info->transaction_lock);
		EMMCFS_LOG_FUNCTION_END(sbi, error);
		return error;
	}
#ifdef CONFIG_EMMCFS_POPO_HELPER
	printk(KERN_ERR "POPO test point 1: commit snapshot");
	for (count = 0; count < 5; count++) {
		printk(KERN_ERR "POPO sleep - 1: %d", count);
		msleep(1000);
	}
#endif

	error = emmcfs_sync_metadata(sbi);
	if (error) {
		sbi->snapshot_info->flags = 0;
		up_write(sbi->snapshot_info->transaction_lock);
		EMMCFS_LOG_FUNCTION_END(sbi, error);
		return error;
	}

#ifdef CONFIG_EMMCFS_POPO_HELPER
	printk(KERN_ERR "POPO test point 2: sync metadata");
	for (count = 0; count < 5; count++) {
		printk(KERN_ERR "POPO sleep - 2 : %d", count);
		msleep(1000);
	}
#endif

	/* Clear snapshot on disk */
	error = EMMCFS_CLEAR_SNAPSHOT(sbi);

	sbi->snapshot_info->flags = 0;
	EMMCFS_DEBUG_SNAPSHOT("SNAPSHOT CLEAR");
	up_write(sbi->snapshot_info->transaction_lock);
	EMMCFS_LOG_FUNCTION_END(sbi, 0);

	return 0;
}

/**
 * @brief 		Write pages.
 * @param [in] 	page 	List of pages
 * @param [in] 	wbc 	Write back control array
 * @return 		Returns error codes
 */
static int emmcfs_writepage(struct page *page, struct writeback_control *wbc)
{
	struct super_block *sb = page->mapping->host->i_sb;
	struct emmcfs_sb_info *sbi = sb->s_fs_info;
	int err = 0;

	EMMCFS_LOG_FUNCTION_START(sbi);
	EMMCFS_DEBUG_SNAPSHOT("emmcfs_writepage START");

	/* Common data, not special files */
	if (page->mapping->host->i_ino >= EMMCFS_1ST_FILE_INO ||
			!test_option(sbi, SNAPSHOT))
		goto write;

	set_page_dirty(page);
	EMMCFS_LOG_FUNCTION_END(sbi, AOP_WRITEPAGE_ACTIVATE);
	return AOP_WRITEPAGE_ACTIVATE;

write:
	err = block_write_full_page(page, emmcfs_get_block, wbc);
	EMMCFS_DEBUG_SNAPSHOT("emmcfs_writepage STOP - 2");
	EMMCFS_LOG_FUNCTION_END(sbi, err);

	return err;
}

/**
 * @brief		Write some dirty pages.
 * @param [in] 	mapping	Address space mapping (holds pages)
 * @param [in] 	wbc	Writeback control - how many pages to write
 *			and write mode
 * @return		Returns 0 on success, errno on failure
 */
static int emmcfs_writepages(struct address_space *mapping,
		struct writeback_control *wbc)
{
	struct super_block *sb = mapping->host->i_sb;
	struct emmcfs_sb_info *sbi = sb->s_fs_info;
	int err = 0;

	EMMCFS_DEBUG_SNAPSHOT("emmcfs_writepages START");

	EMMCFS_LOG_FUNCTION_START(sbi);


	/* Common data, not special files */
	if (mapping->host->i_ino >= EMMCFS_1ST_FILE_INO ||
			!test_option(sbi, SNAPSHOT))
		goto write;

	/* Catalog tree can be killed aready (when umounting) */
	if (!sbi->catalog_tree)
		goto write;

	/* If VFS want to write special file, all we should to do is
	 * update_metadata. On error try to write page in common way */
	if (update_metadata(sbi))
		goto write;

	EMMCFS_DEBUG_SNAPSHOT("emmcfs_writepages STOP - 1");
	wbc->nr_to_write = 0;
	EMMCFS_LOG_FUNCTION_END(sbi, 0);
	return 0;

write:
	err = mpage_writepages(mapping, wbc, emmcfs_get_block);
	EMMCFS_DEBUG_SNAPSHOT("emmcfs_writepages STOP - 2");
	EMMCFS_LOG_FUNCTION_END(sbi, err);
	return err;
}

/**
 * @brief 		Write begin with snapshots.
 * @param [in] 	file 	Pointer to file structure
 * @param [in] 	mapping Address of pages mapping
 * @param [in] 	pos 	Position
 * @param [in] 	len 	Length
 * @param [in] 	flags 	Flags
 * @param [in] 	pagep 	Pages array
 * @param [in] 	fs_data	Data array
 * @return 		Returns error codes
 */
static int emmcfs_write_begin(struct file *file, struct address_space *mapping,
			loff_t pos, unsigned len, unsigned flags,
			struct page **pagep, void **fsdata)
{
	int rc = 0;
	EMMCFS_START_TRANSACTION(EMMCFS_SB(mapping->host->i_sb));
#if LINUX_VERSION_CODE == KERNEL_VERSION(2, 6, 35)
	*pagep = NULL;
	rc = block_write_begin(file, mapping, pos, len, flags, pagep,
		NULL, emmcfs_get_block);
#elif LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 20) ||\
		LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 33)
	rc = block_write_begin(mapping, pos, len, flags, pagep,
		emmcfs_get_block);
#else
	BUILD_BUG();
#endif
	if (rc)
		EMMCFS_STOP_TRANSACTION(EMMCFS_SB(mapping->host->i_sb));
	return rc;
}

/**
 * @brief 		TODO Write begin with snapshots.
 * @param [in] 	file 	Pointer to file structure
 * @param [in] 	mapping	Address of pages mapping
 * @param [in] 	pos 	Position
 * @param [in] 	len 	Length
 * @param [in] 	copied 	Whould it be copied
 * @param [in] 	page 	Page pointer
 * @param [in] 	fs_data	Data
 * @return 		Returns error codes
 */
static int emmcfs_write_end(struct file *file, struct address_space *mapping,
			loff_t pos, unsigned len, unsigned copied,
			struct page *page, void *fsdata)
{
	struct inode *inode = mapping->host;
	int i_size_changed = 0;

	copied = block_write_end(file, mapping, pos, len, copied, page, fsdata);

	/*
	 * No need to use i_size_read() here, the i_size
	 * cannot change under us because we hold i_mutex.
	 *
	 * But it's important to update i_size while still holding page lock:
	 * page write out could otherwise come in and zero beyond i_size.
	 */
	if (pos+copied > inode->i_size) {
		i_size_write(inode, pos+copied);
		i_size_changed = 1;
	}

	unlock_page(page);
	page_cache_release(page);

	/*
	 * Don't mark the inode dirty under page lock. First, it unnecessarily
	 * makes the holding time of page lock longer. Second, it forces lock
	 * ordering of page lock and transaction start for journaling
	 * filesystems.
	 */
	if (i_size_changed)
		emmcfs_write_inode_to_bnode(inode);

	EMMCFS_STOP_TRANSACTION(EMMCFS_SB(inode->i_sb));
	return copied;
}

/**
 * @brief 		Release file.
 * @param [in] 	inode 	Pointer to inode information
 * @param [in] 	file 	Pointer to file structure
 * @return 		Returns error codes
 */
static int emmcfs_file_release(struct inode *inode, struct file *file)
{
	EMMCFS_DEBUG_INO("#%lu", inode->i_ino);
	EMMCFS_LOG_FUNCTION_START(EMMCFS_SB(inode->i_sb));

	if (file->f_mode & FMODE_WRITE)
		emmcfs_fsm_discard_preallocation(EMMCFS_I(inode));

	EMMCFS_LOG_FUNCTION_END(EMMCFS_SB(inode->i_sb), 0);
	return 0;
}

/**
 * @brief 		Function mkdir.
 * @param [in] 	dir 	Pointer to inode
 * @param [in] 	dentry 	Pointer to directory entry
 * @param [in] 	mode 	Mode of operation
 * @return 		Returns error codes
 */
static int emmcfs_mkdir(struct inode *dir, struct dentry *dentry, int mode)
{
	return emmcfs_create(dir, dentry, S_IFDIR | mode, NULL);
}

/**
 * @brief 		Function rmdir.
 * @param [in] 	dir 	Pointer to inode
 * @param [in] 	dentry 	Pointer to directory entry
 * @return 		Returns error codes
 */
static int emmcfs_rmdir(struct inode *dir, struct dentry *dentry)
{
	if (dentry->d_inode->i_size)
		return -ENOTEMPTY;

	return emmcfs_unlink(dir, dentry);
}

/**
 * @brief 		Direct IO.
 * @param [in] 	rw 	read/write
 * @param [in] 	iocb 	Pointer to io block
 * @param [in] 	iov 	Pointer to IO vector
 * @param [in] 	offset 	Offset
 * @param [in] 	nr_segs	Number of segments
 * @return 		Returns written size
 */
static ssize_t emmcfs_direct_IO(int rw, struct kiocb *iocb,
		const struct iovec *iov, loff_t offset, unsigned long nr_segs)
{
	ssize_t rc, inode_new_size = 0;
	struct file *file = iocb->ki_filp;
	struct inode *inode = file->f_path.dentry->d_inode->i_mapping->host;
	struct emmcfs_sb_info *sbi = EMMCFS_SB(inode->i_sb);

	EMMCFS_LOG_FUNCTION_START(sbi);
	EMMCFS_START_TRANSACTION(sbi);

	rc = blockdev_direct_IO(rw, iocb, inode, inode->i_sb->s_bdev, iov,
		offset, nr_segs, emmcfs_get_block, NULL);

	mutex_lock(&EMMCFS_I(inode)->truncate_mutex);
	EMMCFS_DEBUG_MUTEX("truncate mutex lock success");

	if (!IS_ERR_VALUE(rc)) { /* blockdev_direct_IO successfully finished */
		if ((offset + rc) > i_size_read(inode))
			/* last accessed byte behind old inode size */
			inode_new_size = offset + rc;
	}
	else if (EMMCFS_I(inode)->fork.total_block_count >
			inode_size_to_blocks(inode))
		/* blockdev_direct_IO finished with error, but some free space
		 * allocations for inode may have occured, inode internal fork
		 * changed, but inode i_size stay unchanged. */
		inode_new_size = EMMCFS_I(inode)->fork.total_block_count <<
			sbi->block_size_shift;

	if (inode_new_size) {
		i_size_write(inode, inode_new_size);
		emmcfs_write_inode_to_bnode(inode);
	}

	EMMCFS_DEBUG_MUTEX("truncate mutex unlock");
	mutex_unlock(&EMMCFS_I(inode)->truncate_mutex);

	EMMCFS_STOP_TRANSACTION(EMMCFS_SB(inode->i_sb));
	EMMCFS_LOG_FUNCTION_END(EMMCFS_SB(inode->i_sb), rc);
	return rc;
}

/**
 * @brief 		Set size for inode.
 * @param [in] 	inode 	Pointer to inode information
 * @param [in] 	newsize	New size value
 * @return 		Returns error codes
 */
static int setsize(struct inode *inode, loff_t newsize)
{
	loff_t oldsize;
	int error = 0;

	EMMCFS_LOG_FUNCTION_START(EMMCFS_SB(inode->i_sb));

	if (i_size_read(inode) <= newsize) {
		EMMCFS_LOG_FUNCTION_END(EMMCFS_SB(inode->i_sb), -EPERM);
		return -EPERM;
	}

	error = inode_newsize_ok(inode, newsize);
	if (error)
		goto exit;

	if (!(S_ISREG(inode->i_mode) || S_ISDIR(inode->i_mode) ||
	    S_ISLNK(inode->i_mode))) {
		error = -EINVAL;
		goto exit;
	}

	if (IS_APPEND(inode) || IS_IMMUTABLE(inode)) {
		error = -EPERM;
		goto exit;
	}

	error = block_truncate_page(inode->i_mapping,
				    newsize, emmcfs_get_block);
	if (error)
		goto exit;

	EMMCFS_DEBUG_MUTEX("truncate mutex lock");
	mutex_lock(&EMMCFS_I(inode)->truncate_mutex);
	EMMCFS_DEBUG_MUTEX("truncate mutex lock success");

	oldsize = inode->i_size;
	i_size_write(inode, newsize);
	truncate_pagecache(inode, oldsize, newsize);

	error = emmcfs_truncate_blocks(inode, newsize);

	EMMCFS_DEBUG_MUTEX("truncate mutex unlock");
	mutex_unlock(&EMMCFS_I(inode)->truncate_mutex);
	if (error)
		goto exit;

	inode->i_mtime = inode->i_ctime =
			emmcfs_current_time(inode);

exit:
	EMMCFS_LOG_FUNCTION_END(EMMCFS_SB(inode->i_sb), error);
	return error;
}

/**
 * @brief 		Set attributes.
 * @param [in] 	dentry 	Pointer to directory entry
 * @param [in] 	iattr 	Attributes to be set
 * @return 		Returns error codes
 */
static int emmcfs_setattr(struct dentry *dentry, struct iattr *iattr)
{
	struct inode *inode = dentry->d_inode;
	int error = 0;

	EMMCFS_LOG_FUNCTION_START(EMMCFS_SB(inode->i_sb));

	EMMCFS_START_TRANSACTION(EMMCFS_SB(inode->i_sb));
	error = inode_change_ok(inode, iattr);
	if (error)
		goto exit;

	if ((iattr->ia_valid & ATTR_SIZE) &&
			iattr->ia_size != i_size_read(inode)) {
		error = setsize(inode, iattr->ia_size);
		if (error)
			goto exit;
	}

#if LINUX_VERSION_CODE == KERNEL_VERSION(2, 6, 35)
	generic_setattr(inode, iattr);
#elif LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 20) ||\
		LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 33)
	setattr_copy(inode, iattr);
#else
	BUILD_BUG();
#endif

	error = emmcfs_write_inode_to_bnode(inode);

exit:
	EMMCFS_STOP_TRANSACTION(EMMCFS_SB(inode->i_sb));

	EMMCFS_LOG_FUNCTION_END(EMMCFS_SB(inode->i_sb), error);
	return error;
}

/**
 * @brief 		Make bmap.
 * @param [in] 	mapping	Address of pages mapping
 * @param [in] 	block 	Block number
 * @return		TODO Returns 0 on success, errno on failure
 */
static sector_t emmcfs_bmap(struct address_space *mapping, sector_t block)
{
	return generic_block_bmap(mapping, block, emmcfs_get_block);
}

/**
 * @brief 			Function rename.
 * @param [in] 	old_dir 	Pointer to old dir struct
 * @param [in] 	old_dentry 	Pointer to old dir entry struct
 * @param [in] 	new_dir 	Pointer to new dir struct
 * @param [in] 	new_dentry 	Pointer to new dir entry struct
 * @return 			Returns error codes
 */
static int emmcfs_rename(struct inode *old_dir, struct dentry *old_dentry,
		struct inode *new_dir, struct dentry *new_dentry)
{
	struct emmcfs_find_data fd;
	struct emmcfs_sb_info *sbi = old_dir->i_sb->s_fs_info;
	struct inode *mv_inode = old_dentry->d_inode;
	int ret = 0;
	struct emmcfs_cattree_key *rm_key = NULL, *key = NULL;

	EMMCFS_LOG_FUNCTION_START(sbi);
	if (new_dentry->d_name.len > EMMCFS_CAT_MAX_NAME) {
		EMMCFS_LOG_FUNCTION_END(sbi, -ENAMETOOLONG);
		return -ENAMETOOLONG;
	}

	if (new_dentry->d_inode) {
		struct inode *new_dentry_inode = new_dentry->d_inode;
		if (S_ISDIR(new_dentry_inode->i_mode) &&
				new_dentry_inode->i_size > 0) {
			EMMCFS_LOG_FUNCTION_END(sbi, -ENOTEMPTY);
			return -ENOTEMPTY;
		}
		ret = emmcfs_unlink(new_dir, new_dentry);
		if (ret) {
			EMMCFS_LOG_FUNCTION_END(sbi, ret);
			return ret;
		}
	}

	EMMCFS_START_TRANSACTION(sbi);
	/* Find old record */
	EMMCFS_DEBUG_MUTEX("cattree mutex w lock");
	mutex_w_lock(sbi->catalog_tree->rw_tree_lock);
	EMMCFS_DEBUG_MUTEX("cattree mutex w lock succ");

	fd.bnode = NULL;
	ret = emmcfs_cattree_find(sbi, old_dir->i_ino,
			(char *) old_dentry->d_name.name,
			old_dentry->d_name.len, &fd, EMMCFS_BNODE_MODE_RW);
	if (ret) {
		EMMCFS_DEBUG_MUTEX("cattree mutex w lock un");
		mutex_w_unlock(sbi->catalog_tree->rw_tree_lock);
		goto exit;
	}

	rm_key = emmcfs_get_btree_record(fd.bnode, fd.pos);
	if (IS_ERR(rm_key)) {
		ret = PTR_ERR(rm_key);
		goto exit;
	}

	/* found_key is a part of bnode, wich we are going to modify,
	 * so we have to copy save its current value */
	key = kmalloc(le32_to_cpu(rm_key->gen_key.key_len), GFP_KERNEL);
	if (!key) {
		emmcfs_put_bnode(fd.bnode);
		EMMCFS_DEBUG_MUTEX("cattree mutex w lock un");
		mutex_w_unlock(sbi->catalog_tree->rw_tree_lock);
		ret = -ENOMEM;
		goto exit;
	}
	memcpy(key, rm_key, le32_to_cpu(rm_key->gen_key.key_len));
	emmcfs_put_bnode(fd.bnode);
	

	ret = emmcfs_add_new_cattree_object(mv_inode, new_dir->i_ino,
			&new_dentry->d_name);
	if (ret) {
		/* TODO change to some erroneous action */
		EMMCFS_DEBUG_INO("can not rename #%d; old_dir_ino=%lu "
				"oldname=%s new_dir_ino=%lu newname=%s",
				ret,
				old_dir->i_ino, old_dentry->d_name.name,
				new_dir->i_ino, new_dentry->d_name.name);
		kfree(key);
		mutex_w_unlock(sbi->catalog_tree->rw_tree_lock);
		goto exit;
	}

	ret = emmcfs_btree_remove(sbi->catalog_tree, &key->gen_key);
	kfree(key);
	if (ret)
		/* TODO change to some erroneous action */
		EMMCFS_BUG();


	if (old_dir->i_size != 0)
		old_dir->i_size--;
	else
		EMMCFS_DEBUG_INO("Files count mismatch");

	new_dir->i_size++;

	if (EMMCFS_I(mv_inode)->hlink_id == INVALID_HLINK_ID) {
		char *saved_name;
		EMMCFS_I(mv_inode)->parent_id = new_dir->i_ino;
		kfree(EMMCFS_I(mv_inode)->name);
		saved_name = kmalloc(new_dentry->d_name.len + 1, GFP_KERNEL);
		if (!saved_name) {
			iput(mv_inode);
			ret = -ENOMEM;
			goto exit;
		}

		strncpy(saved_name, new_dentry->d_name.name,
				new_dentry->d_name.len + 1);
		EMMCFS_I(mv_inode)->name = saved_name;
	}

	mv_inode->i_ctime = emmcfs_current_time(mv_inode);

	EMMCFS_DEBUG_MUTEX("cattree mutex w lock un");
	mutex_w_unlock(sbi->catalog_tree->rw_tree_lock);

	ret = emmcfs_write_inode_to_bnode(old_dir);
	if (ret)
		goto exit;
	ret = emmcfs_write_inode_to_bnode(new_dir);
exit:
	EMMCFS_STOP_TRANSACTION(sbi);

	EMMCFS_LOG_FUNCTION_END(sbi, ret);
	return ret;
}

/**
 * @brief 			Create hardlink record .
 * @param [in] 	hlink_id  	Hardlink id
 * @param [in] 	par_ino_n 	Parent inode number
 * @param [in] 	name 		Name
 * @return 			Returns pointer to buffer with hardlink
 */
static void *form_hlink_record(hlink_id_t hlink_id,
		ino_t par_ino_n, struct qstr *name)
{
	void *record = emmcfs_alloc_cattree_key(name->len,
			EMMCFS_CATALOG_HLINK_RECORD);
	struct emmcfs_catalog_hlink_record *hlink_val;
	if (!IS_ERR(record)) {
		emmcfs_fill_cattree_key(record, par_ino_n, name);
		hlink_val = get_value_pointer(record);
		hlink_val->hlink_id = cpu_to_le64(hlink_id);
		hlink_val->record_type = EMMCFS_CATALOG_HLINK_RECORD;
	}
	return record;
}

/**
 * @brief 			Add hardlink record .
 * @param [in] 	cat_tree 	Pointer to catalog tree
 * @param [in] 	hlink_id  	Hardlink id
 * @param [in] 	par_ino_n	Parent inode number
 * @param [in] 	name 		Name
 * @return 			Returns error codes
 */
static int add_hlink_record(struct emmcfs_btree *cat_tree, hlink_id_t hlink_id,
		ino_t par_ino_n, struct qstr *name)
{
	void *ins_rec = form_hlink_record(hlink_id, par_ino_n, name);
	int ret;

	EMMCFS_LOG_FUNCTION_START(cat_tree->sbi);
	if (IS_ERR(ins_rec)) {
		EMMCFS_LOG_FUNCTION_END(cat_tree->sbi, PTR_ERR(ins_rec));
		return PTR_ERR(ins_rec);
	}
	ret = emmcfs_btree_insert(cat_tree, ins_rec);
	kfree(ins_rec);
	EMMCFS_LOG_FUNCTION_END(cat_tree->sbi, 0);
	return ret;
}

/**
 * @brief 		Get hlink ID.
 * @param [in] 	sbi 	Pointer to superblock information
 * @return 		Returns hardlink id
 */
static hlink_id_t obtain_hlink_id(struct emmcfs_sb_info *sbi)
{
	struct page *page;
	pgoff_t page_index = 0;
	void *data = NULL;
	hlink_id_t res = INVALID_HLINK_ID, find_from, id_offset;
	hlink_id_t chunk_n = 0;

	EMMCFS_LOG_FUNCTION_START(sbi);

	find_from = sbi->hard_links.last_used_id & EMMCFS_BITMAP_PAGE_MASK;

	chunk_n = sbi->hard_links.last_used_id >> EMMCFS_LOG_BITS_PER_PAGE;
	while (res == INVALID_HLINK_ID) {
		page_index = (chunk_n <<
			(EMMCFS_LOG_BITS_PER_PAGE - EMMCFS_LOG_HLINKS_PER_PAGE))
			+ chunk_n;
		id_offset = chunk_n << EMMCFS_LOG_BITS_PER_PAGE;
		page = emmcfs_read_create_page(sbi->hard_links.inode,
				page_index);

		if (IS_ERR(page)) {
			EMMCFS_DEBUG_INO("error %ld", PTR_ERR(page));
			EMMCFS_LOG_FUNCTION_END(sbi, PTR_ERR(page));
			return INVALID_HLINK_ID;
		}

		data = kmap(page);

		res = find_next_zero_bit(data, (1 << EMMCFS_LOG_BITS_PER_PAGE),
				find_from);

		if (res < (1 << EMMCFS_LOG_BITS_PER_PAGE)) {
			/*EMMCFS_BUG_ON(res + id_offset < EMMCFS_1ST_FILE_INO);*/
			EMMCFS_BUG_ON(EMMCFS_ADD_CHUNK(sbi->hard_links.inode,
						&page, 1));

			EMMCFS_BUG_ON(test_and_set_bit(res, data));

			res += id_offset;
			sbi->hard_links.last_used_id = res;
			set_page_dirty_lock(page);
		} else {
			res = INVALID_HLINK_ID;
		}

		kunmap(page);
		page_cache_release(page);

		chunk_n++;
		find_from = 0;
	}

	EMMCFS_LOG_FUNCTION_END(sbi, res);
	return res;
}

/**
 * @brief 		Clone hardlink data.
 * @param [in] 	sbi 	Pointer to superblock information
 * @param [in] 	data 	Data
 * @param [in] 	size 	Data size
 * @return 		Returns hardlink id
 */
static hlink_id_t clone_data_into_hlink_area(struct emmcfs_sb_info *sbi, void *data,
		u32 size)
{
	struct page *page;
	hlink_id_t result;
	u16 page_offset;

	EMMCFS_LOG_FUNCTION_START(sbi);
	result = obtain_hlink_id(sbi);
	if (result == INVALID_HLINK_ID) {
		EMMCFS_LOG_FUNCTION_END(sbi, result);
		return result;
	}

	page = get_hlink_value_page(sbi->hard_links.inode,
			result, &page_offset);
	if (IS_ERR(page)) {
		EMMCFS_LOG_FUNCTION_END(sbi, PTR_ERR(page));
		return INVALID_HLINK_ID;
	}

	EMMCFS_BUG_ON(EMMCFS_ADD_CHUNK(sbi->hard_links.inode, &page, 1));

	EMMCFS_BUG_ON(size > (1 << EMMCFS_LOG_ONE_HLINK_DATA_SZ));
	memcpy(kmap(page) + page_offset, data, size);
	kunmap(page);
	set_page_dirty_lock(page);
	EMMCFS_LOG_FUNCTION_END(sbi, result);
	return result;
}

/* TODO ext2 defines this as 32000, but let it stay so for a while */
#define EMMCFS_LINK_MAX 32
/**
 * @brief 			Create link.
 * @param [in] 	old_dentry	Old dentry
 * @param [in] 	dir 		The inode dir pointer
 * @param [out]	dentry 		Pointer to result dentry
 * @return 			Returns error codes
 */
static int emmcfs_link(struct dentry *old_dentry, struct inode *dir,
	struct dentry *dentry)
{
	struct inode *inode = old_dentry->d_inode;
	struct inode *par_inode = old_dentry->d_parent->d_inode;
	struct emmcfs_sb_info *sbi = old_dentry->d_inode->i_sb->s_fs_info;
	struct emmcfs_cattree_key *found_key;
	hlink_id_t hlink_id;
	struct emmcfs_catalog_folder_record *data;
	struct emmcfs_find_data fd;
	int ret;

	EMMCFS_LOG_FUNCTION_START(sbi);
	if (dentry->d_name.len > EMMCFS_CAT_MAX_NAME) {
		EMMCFS_LOG_FUNCTION_END(sbi, -ENAMETOOLONG);
		return -ENAMETOOLONG;
	}

	EMMCFS_BUG_ON(S_ISDIR(inode->i_mode));
	if (inode->i_nlink >= EMMCFS_LINK_MAX) {
		EMMCFS_LOG_FUNCTION_END(sbi, -EMLINK);
		return -EMLINK;
	}
	/* TODO - rlock - runlock - wlock - think about more elegant locking
	 * scheme
	 */
	EMMCFS_DEBUG_MUTEX("cattree mutex r lock");
	mutex_r_lock(sbi->catalog_tree->rw_tree_lock);
	EMMCFS_DEBUG_MUTEX("cattree mutex w lock succ");
	fd.bnode = NULL;
	ret = emmcfs_cattree_find(sbi, dir->i_ino, dentry->d_name.name,
			dentry->d_name.len, &fd, EMMCFS_BNODE_MODE_RO);
	if (!ret || ret != -ENOENT) {
		if (!ret)
			emmcfs_put_bnode(fd.bnode);
		EMMCFS_DEBUG_MUTEX("cattree mutex r lock un");
		mutex_r_unlock(sbi->catalog_tree->rw_tree_lock);
		EMMCFS_LOG_FUNCTION_END(sbi, -EEXIST);
		return -EEXIST;
	}
	EMMCFS_DEBUG_MUTEX("cattree mutex r lock un");
	mutex_r_unlock(sbi->catalog_tree->rw_tree_lock);

	/* TODO - to think wether it can be - source inode is actual,
	 * but source object is absent in tree. Change to BUG if it
	 * can never be
	 */
	EMMCFS_START_TRANSACTION(sbi);
	EMMCFS_DEBUG_MUTEX("cattree mutex w lock");
	mutex_w_lock(sbi->catalog_tree->rw_tree_lock);
	EMMCFS_DEBUG_MUTEX("cattree mutex w lock succ");
	fd.bnode = NULL;
	ret = emmcfs_cattree_find(sbi, par_inode->i_ino,
			old_dentry->d_name.name, old_dentry->d_name.len, &fd,
			EMMCFS_BNODE_MODE_RW);
	if (ret)
		goto exit;

	found_key = emmcfs_get_btree_record(fd.bnode, fd.pos);
	if (IS_ERR(found_key)) {
		ret = PTR_ERR(found_key);
		goto exit;
	}
	data = get_value_pointer(found_key);
	if (le16_to_cpu(data->record_type) != EMMCFS_CATALOG_HLINK_RECORD) {
		struct emmcfs_cattree_key *key;
		hlink_id = clone_data_into_hlink_area(sbi, data,
				le32_to_cpu(found_key->gen_key.record_len) -
				le32_to_cpu(found_key->gen_key.key_len));

		if (hlink_id == INVALID_HLINK_ID) {
			ret = -EIO;
			goto exit;
		}

		/* found_key is a part of bnode, wich we are going to modify,
		 * so we have to copy save its current value */
		key = kmalloc(le32_to_cpu(found_key->gen_key.key_len),
				GFP_KERNEL);
		if (!key) {
			ret = -ENOMEM;
			goto exit;
		}
		memcpy(key, found_key, le32_to_cpu(found_key->gen_key.key_len));

		emmcfs_put_bnode(fd.bnode);
		fd.bnode = NULL;
		ret = emmcfs_btree_remove(sbi->catalog_tree,
				&key->gen_key);
		kfree(key);
		if (ret)
			goto exit;

		ret = add_hlink_record(sbi->catalog_tree, hlink_id,
				par_inode->i_ino, &old_dentry->d_name);
		if (ret) {
			/* TODO - potential bad thing - old record is killed,
			 * and new can't be inserted, the operation fails and
			 * we loose the target file. Think how it can be avoided
			 */
			EMMCFS_BUG();
			goto exit;
		}
		kfree(EMMCFS_I(inode)->name);
		EMMCFS_I(inode)->name = NULL;
		EMMCFS_I(inode)->hlink_id = hlink_id;
	} else {
		hlink_id = le64_to_cpu(
				((struct emmcfs_catalog_hlink_record *)data)->
				hlink_id);
	}

	if (fd.bnode && !IS_ERR(fd.bnode)) {
		emmcfs_put_bnode(fd.bnode);
		fd.bnode = NULL;
	}

	ret = add_hlink_record(sbi->catalog_tree, hlink_id, dir->i_ino,
			&dentry->d_name);
	if (ret)
		goto exit;

	inode->i_ctime = emmcfs_current_time(inode);
	dir->i_ctime = emmcfs_current_time(dir);
	dir->i_mtime = emmcfs_current_time(dir);
	inode_inc_link_count(inode);

#if LINUX_VERSION_CODE == KERNEL_VERSION(2, 6, 35)
	atomic_inc(&inode->i_count);
#elif LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 20) ||\
		LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 33)
	ihold(inode);
#endif

	dir->i_size++;
	sbi->files_count++;
	d_instantiate(dentry, inode);

exit:
	if (fd.bnode && !IS_ERR(fd.bnode))
		emmcfs_put_bnode(fd.bnode);
	EMMCFS_DEBUG_MUTEX("cattree mutex w lock un");
	mutex_w_unlock(sbi->catalog_tree->rw_tree_lock);
	if (!ret) {
		/* TODO all this writes can fail.
		 * Think what to do if they will*/
		emmcfs_write_inode_to_bnode(dir);
		emmcfs_write_inode_to_bnode(inode);
	}

	EMMCFS_STOP_TRANSACTION(sbi);
	EMMCFS_LOG_FUNCTION_END(sbi, ret);
	return ret;
}

/**
 * @brief 			Make node.
 * @param [in,out]	dir 	Directory where node will be created
 * @param [in] 		dentry 	Created dentry
 * @param [in] 		mode 	Mode for file
 * @param [in] 		rdev 	Device
 * @return			Returns 0 on success, errno on failure
 */
static int emmcfs_mknod(struct inode *dir, struct dentry *dentry,
			int mode, dev_t rdev)
{
	struct inode *created_ino;
	int ret;

	EMMCFS_LOG_FUNCTION_START(EMMCFS_SB(dir->i_sb));
	EMMCFS_START_TRANSACTION(EMMCFS_SB(dir->i_sb));

	if (!new_valid_dev(rdev)) {
		ret = -EINVAL;
		goto exit;
	}

	ret = emmcfs_create(dir, dentry, mode, NULL);
	if (ret)
		goto exit;

	created_ino = dentry->d_inode;
	init_special_inode(created_ino, created_ino->i_mode, rdev);
	ret = emmcfs_write_inode_to_bnode(created_ino);
exit:
	EMMCFS_STOP_TRANSACTION(EMMCFS_SB(dir->i_sb));
	EMMCFS_LOG_FUNCTION_END(EMMCFS_SB(dir->i_sb), ret);
	return ret;
}

/**
 * @brief 			Make symlink.
 * @param [in,out] 	dir 	Directory where node will be created
 * @param [in] 		dentry 	Created dentry
 * @param [in] 		symname Symbolic link name
 * @return			Returns 0 on success, errno on failure
 */
static int emmcfs_symlink(struct inode *dir, struct dentry *dentry,
	const char *symname)
{
	int ret;
	struct inode *created_ino;
	unsigned int len = strlen(symname);

	if ((len > EMMCFS_FULL_PATH_LEN) ||
	    (dentry->d_name.len > EMMCFS_CAT_MAX_NAME))
		return -ENAMETOOLONG;

	EMMCFS_START_TRANSACTION(EMMCFS_SB(dir->i_sb));

	ret = emmcfs_create(dir, dentry, S_IFLNK | S_IRWXUGO, NULL);

	if (ret)
		goto exit;

	created_ino = dentry->d_inode;
	ret = page_symlink(created_ino, symname, ++len);
exit:
	EMMCFS_STOP_TRANSACTION(EMMCFS_SB(dir->i_sb));
	return ret;
}

/**
 * The eMMCFS address space operations.
 */
static const struct address_space_operations emmcfs_aops = {
	.readpage	= emmcfs_readpage,
	.readpages	= emmcfs_readpages,
	.writepage	= emmcfs_writepage,
	.writepages	= emmcfs_writepages,
#if LINUX_VERSION_CODE == KERNEL_VERSION(2,6,35)
	.sync_page	= block_sync_page,
#endif
	.write_begin	= emmcfs_write_begin,
	.write_end	= emmcfs_write_end,
	.bmap		= emmcfs_bmap,
	.direct_IO	= emmcfs_direct_IO,
	.migratepage	= buffer_migrate_page,
/*	.set_page_dirty = __set_page_dirty_buffers,*/

};

static const struct address_space_operations emmcfs_aops_special = {
	.readpage	= emmcfs_readpage,
	.readpages	= emmcfs_readpages,
	.writepage	= emmcfs_writepage,
	.writepages	= emmcfs_writepages,
#if LINUX_VERSION_CODE == KERNEL_VERSION(2,6,35)
	.sync_page	= block_sync_page,
#endif
	.write_begin	= emmcfs_write_begin,
	.write_end	= emmcfs_write_end,
	.bmap		= emmcfs_bmap,
	.direct_IO	= emmcfs_direct_IO,
	.migratepage	= fail_migrate_page,
/*	.set_page_dirty = __set_page_dirty_buffers,*/

};

/**
 * The eMMCFS directory inode operations.
 */
static const struct inode_operations emmcfs_dir_inode_operations = {
	/* d.voytik-TODO-19-01-2012-11-15-00:
	 * [emmcfs_dir_inode_ops] add to emmcfs_dir_inode_operations
	 * necessary methods */
	.create		= emmcfs_create,
	.symlink	= emmcfs_symlink,
	.lookup		= emmcfs_lookup,
	.link		= emmcfs_link,
	.unlink		= emmcfs_unlink,
	.mkdir		= emmcfs_mkdir,
	.rmdir		= emmcfs_rmdir,
	.mknod		= emmcfs_mknod,
	.rename		= emmcfs_rename,
};

/**
 * The eMMCFS symlink inode operations.
 */
const struct inode_operations emmcfs_symlink_inode_operations = {
	.readlink	= generic_readlink,
	.follow_link	= page_follow_link_light,
	.put_link	= page_put_link,
	.setattr	= emmcfs_setattr,
};


/**
 * The eMMCFS directory operations.
 */
static const struct file_operations emmcfs_dir_operations = {
	/* d.voytik-TODO-19-01-2012-11-16-00:
	 * [emmcfs_dir_ops] add to emmcfs_dir_operations necessary methods */
	.read		= generic_read_dir,
	.readdir	= emmcfs_readdir,
};

/**
 * TODO
 */
static int emmcfs_file_fsync(struct file *file, int datasync)
{
	struct emmcfs_sb_info *sbi = file->f_mapping->host->i_sb->s_fs_info;
	int ret = 0;
	
	ret = generic_file_fsync(file, datasync);
	if (ret)
		return ret;

	ret = update_metadata(sbi);
	return (!ret || ret == -EINVAL) ? 0 : ret;
}

#if LINUX_VERSION_CODE == KERNEL_VERSION(2, 6, 35)
long emmcfs_fallocate(struct inode *inode, int mode, loff_t offset, loff_t len);
#elif LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 20) || \
		LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 33)
long emmcfs_fallocate(struct file *file, int mode, loff_t offset, loff_t len);
#endif

/**
 * The eMMCFS file operations.
 */
static const struct file_operations emmcfs_file_operations = {
	.llseek		= generic_file_llseek,
	.read		= do_sync_read,
	.aio_read	= generic_file_aio_read,
	.write		= do_sync_write,
	.aio_write	= generic_file_aio_write,
	.mmap		= generic_file_mmap,
	.open		= generic_file_open,
	.release	= emmcfs_file_release,
	.fsync		= emmcfs_file_fsync,
	.splice_read	= generic_file_splice_read,
	.splice_write	= generic_file_splice_write,
#if LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 20) || \
	LINUX_VERSION_CODE == KERNEL_VERSION(3, 0, 33)
	.fallocate	= emmcfs_fallocate,
#endif
};

/**
 * The eMMCFS files inode operations.
 */
static const struct inode_operations emmcfs_file_inode_operations = {
		/* FIXME & TODO is this correct : use same function as in
		emmcfs_dir_inode_operations? */
		.lookup		= emmcfs_lookup,
		/*.truncate	= emmcfs_file_truncate, depricated*/
		.setattr	= emmcfs_setattr,
#if LINUX_VERSION_CODE == KERNEL_VERSION(2, 6, 35)
		.fallocate	= emmcfs_fallocate,
#endif
};

/**
 * @brief 		The eMMCFS inode constructor.
 * @param [in] 	dir 	Directory, where inode will be created
 * @param [in] 	mode 	Mode for created inode
 * @return		Returns pointer to inode on success, errno on failure
 */
static struct inode *emmcfs_new_inode(struct inode *dir, int mode)
{
	struct super_block *sb = dir->i_sb;
	ino_t ino = 0;
	struct inode *inode;
	int err, i;
	struct emmcfs_fork_info *ifork;
	struct inode *ret;

	EMMCFS_LOG_FUNCTION_START(EMMCFS_SB(sb));

	err = get_free_inode(sb->s_fs_info, &ino);

	if (err) {
		EMMCFS_LOG_FUNCTION_END(EMMCFS_SB(sb), err);
		return ERR_PTR(err);
	}

	/*EMMCFS_DEBUG_INO("#%lu", ino);*/
	inode = new_inode(sb);
	if (!inode) {
		ret = ERR_PTR(-ENOMEM);
		EMMCFS_LOG_FUNCTION_END(EMMCFS_SB(sb), -ENOMEM);
		return ret;
	}

	inode->i_ino = ino;
	inode_init_owner(inode, dir, mode);
	inode->i_nlink = 1;
	inode->i_size = 0;
	sb->s_dirt = 1;
	inode->i_blocks = 0;
	inode->i_mtime = inode->i_atime = inode->i_ctime =
			emmcfs_current_time(inode);
	inode->i_flags = dir->i_flags;
	if (S_ISDIR(mode))
		inode->i_op =  &emmcfs_dir_inode_operations;
	else if (S_ISLNK(mode))
		inode->i_op = &emmcfs_symlink_inode_operations;
	else
		inode->i_op = &emmcfs_file_inode_operations;

	inode->i_mapping->a_ops = &emmcfs_aops;
	inode->i_fop = (S_ISDIR(mode)) ?
			&emmcfs_dir_operations : &emmcfs_file_operations;

	/* Init extents with zeros - file has no any space yet */
	ifork = &(EMMCFS_I(inode)->fork);
	ifork->used_extents = 0;
	for (i = EMMCFS_EXTENTS_COUNT_IN_FORK - 1; i >= 0; i--) {
		ifork->extents[i].first_block = 0;
		ifork->extents[i].block_count = 0;
	}
	ifork->total_block_count = 0;

	ifork->prealloc_start_block = 0;
	ifork->prealloc_block_count = 0;

	EMMCFS_I(inode)->parent_id = 0;

	EMMCFS_I(inode)->is_new = 1;
	if (insert_inode_locked(inode) < 0) {
		err = -EINVAL;
		goto err_exit;
	}

	EMMCFS_LOG_FUNCTION_END(EMMCFS_SB(sb), 0);
	return inode;

err_exit:
	if (emmcfs_free_inode_n(sb->s_fs_info, inode->i_ino))
		EMMCFS_ERR("can not free inode while handling error");

	iput(inode);
	EMMCFS_LOG_FUNCTION_END(EMMCFS_SB(sb), err);
	return ERR_PTR(err);
}

/**
 * @brief 			Standard callback to create file.
 * @param [in,out]	dir 	Directory where node will be created
 * @param [in] 		dentry 	Created dentry
 * @param [in] 		mode 	Mode for file
 * @param [in] 		nd 	Namedata for file
 * @return			Returns 0 on success, errno on failure
 */
static int emmcfs_create(struct inode *dir, struct dentry *dentry, int mode,
		struct nameidata *nd)
{
	struct super_block		*sb = dir->i_sb;
	struct emmcfs_sb_info		*sbi = sb->s_fs_info;
	struct inode			*inode;
	char *saved_name;
	int ret = 0;
	/* TODO - ext2 does it - detrmine if it's necessary here */
	/* dquot_initialize(dir); */

	EMMCFS_LOG_FUNCTION_START(sbi);
	EMMCFS_DEBUG_INO("'%s' dir = %ld", dentry->d_name.name, dir->i_ino);

	/*if (emmcfs_unicode_strlen(dentry->d_name.name)*/
	if (strlen(dentry->d_name.name)	> EMMCFS_UNICODE_STRING_MAX_LEN) {
		EMMCFS_LOG_FUNCTION_END(sbi, -ENAMETOOLONG);
		return -ENAMETOOLONG;
	}

	EMMCFS_START_TRANSACTION(sbi);
	inode = emmcfs_new_inode(dir, mode);

	if (IS_ERR(inode)) {
		ret = PTR_ERR(inode);
		goto exit;
	}

	saved_name = kmalloc(dentry->d_name.len + 1, GFP_KERNEL);
	if (!saved_name) {
		iput(inode);
		ret = -ENOMEM;
		goto exit;
	}

	strncpy(saved_name, dentry->d_name.name, dentry->d_name.len + 1);

	EMMCFS_DEBUG_MUTEX("cattree mutex w lock");
	mutex_w_lock(sbi->catalog_tree->rw_tree_lock);
	EMMCFS_DEBUG_MUTEX("cattree mutex w lock succ");

	EMMCFS_I(inode)->name = saved_name;
	EMMCFS_I(inode)->parent_id = dir->i_ino;
	EMMCFS_I(inode)->hlink_id = INVALID_HLINK_ID;
	unlock_new_inode(inode);

	ret = emmcfs_add_new_cattree_object(inode, dir->i_ino, &dentry->d_name);

	if (ret) {
		iput(inode);
		emmcfs_free_inode_n(sbi, inode->i_ino);
		EMMCFS_DEBUG_MUTEX("cattree mutex w lock un");
		mutex_w_unlock(sbi->catalog_tree->rw_tree_lock);
		goto exit;
	}

	EMMCFS_DEBUG_MUTEX("cattree mutex w lock un");
	mutex_w_unlock(sbi->catalog_tree->rw_tree_lock);

	d_instantiate(dentry, inode);
	dir->i_size++;
	sbi->files_count++;
	dir->i_ctime = emmcfs_current_time(dir);
	dir->i_mtime = emmcfs_current_time(dir);
	ret = emmcfs_write_inode_to_bnode(dir);

exit:
	EMMCFS_STOP_TRANSACTION(sbi);
	EMMCFS_LOG_FUNCTION_END(sbi, ret);
	return ret;
}

/**
 * @brief 			Write inode to bnode.
 * @param [in,out] 	inode 	The inode, that will be written to bnode
 * @return			Returns 0 on success, errno on failure
 */
static int emmcfs_write_inode_to_bnode(struct inode *inode)
{
	struct super_block *sb = inode->i_sb;
	struct emmcfs_sb_info *sbi = sb->s_fs_info;
	struct emmcfs_bnode *bnode = NULL;
	struct emmcfs_find_data fd = {NULL, 0, 0};
	struct page *page = NULL;
	int ret = 0;
	void *value_area;

	if (inode->i_ino < EMMCFS_1ST_FILE_INO)
		return 0;

	EMMCFS_LOG_FUNCTION_START(sbi);
	EMMCFS_DEBUG_MUTEX("cattree mutex w lock");
	mutex_w_lock(sbi->catalog_tree->rw_tree_lock);
	EMMCFS_DEBUG_MUTEX("cattree mutex w lock succ");
	if (EMMCFS_I(inode)->hlink_id != INVALID_HLINK_ID) {
		u16 page_offset;
		page = get_hlink_value_page(sbi->hard_links.inode,
			EMMCFS_I(inode)->hlink_id, &page_offset);
		if (IS_ERR(page)) {
			ret = PTR_ERR(page);
			EMMCFS_LOG_FUNCTION_END(sbi, ret);
			goto exit_unlock;
		}
		EMMCFS_BUG_ON(EMMCFS_ADD_CHUNK(sbi->hard_links.inode,
					&page, 1));
		value_area = kmap(page) + page_offset;
	} else {
		void *record;
		/* Get record for current inode from the catalog tree */
		ret = emmcfs_get_from_cattree(EMMCFS_I(inode), &fd,
				EMMCFS_BNODE_MODE_RW);
		if (ret)
			goto exit_unlock;

		bnode = fd.bnode;
		record = emmcfs_get_btree_record(bnode, fd.pos);
		if (IS_ERR(record)) {
			ret = PTR_ERR(record);
			goto exit_unlock;
		}
		value_area = get_value_pointer(record);
	}
	ret = emmcfs_fill_cattree_value(inode, value_area);
	if (ret)
		goto exit_unlock;

	if (EMMCFS_I(inode)->hlink_id == INVALID_HLINK_ID) {
		emmcfs_mark_bnode_dirty(bnode);
		emmcfs_put_bnode(bnode);
	} else {
		EMMCFS_BUG_ON(!page);
		kunmap(page);
		set_page_dirty_lock(page);
	}

exit_unlock:
	EMMCFS_DEBUG_MUTEX("cattree mutex w lock un");
	mutex_w_unlock(sbi->catalog_tree->rw_tree_lock);
	EMMCFS_LOG_FUNCTION_END(sbi, ret);
	return ret;
}

/**
 * @brief			Method to read inode.
 * @param [in, out]	inode	Pointer to inode to be read
 * @param [in, out] 	fd	Find data
 * @return			Returns 0 on success, errno on failure
 */
static int emmcfs_read_inode(struct inode *inode, struct emmcfs_find_data *fd)
{
	struct super_block		*sb = inode->i_sb;
	struct emmcfs_sb_info *sbi = sb->s_fs_info;
	int				ret = 0;
	char				*new_name;
	int				str_len = 0;
	struct emmcfs_cattree_key *key;
	struct emmcfs_catalog_folder_record *comm_val;
	struct emmcfs_catalog_folder_record *folder_val;
	struct page *page = NULL;
	int rec_type;
	hlink_id_t hlink_id = INVALID_HLINK_ID;
	EMMCFS_LOG_FUNCTION_START(sbi);

	if (inode->i_ino == EMMCFS_ROOT_INO) {
		ret = get_root_folder(sb->s_fs_info, fd);
		if (ret) {
			EMMCFS_LOG_FUNCTION_END(sbi, ret);
			return ret;
		}
	}
	EMMCFS_BUG_ON(!fd->bnode);

	/* Fill inode's search hint */
	EMMCFS_I(inode)->bnode_hint.bnode_id = fd->bnode->node_id;
	EMMCFS_I(inode)->bnode_hint.pos = fd->pos;

	key = emmcfs_get_btree_record(fd->bnode, fd->pos);
	if (IS_ERR(key)) {
		ret = PTR_ERR(key);
		goto err_exit;
	}
	comm_val = get_value_pointer(key);
	rec_type = le16_to_cpu(comm_val->record_type);

	if (rec_type == EMMCFS_CATALOG_HLINK_RECORD) {
		u16 page_offset;
		hlink_id = le64_to_cpu(
			((struct emmcfs_catalog_hlink_record *)comm_val)->
					hlink_id);
		page = get_hlink_value_page(sbi->hard_links.inode, hlink_id,
				&page_offset);
		if (IS_ERR(page)) {
			EMMCFS_LOG_FUNCTION_END(sbi, PTR_ERR(page));
			return PTR_ERR(page);
		}
		comm_val = kmap(page) + page_offset;
		rec_type = comm_val->record_type;
	}

	folder_val = (struct emmcfs_catalog_folder_record *)comm_val;
	if (hlink_id == INVALID_HLINK_ID) {
		new_name = kmalloc(key->name.length + 1, GFP_KERNEL);
		if (!new_name)
			goto err_exit;

		/*str_len = utf16s_to_utf8s((const u16 *)key->name.unicode_str,
				key->name.length, UTF16_LITTLE_ENDIAN, new_name,
				EMMCFS_CAT_MAX_NAME);*/
		/*str_len = emmcfs_unicode_strlen(key->name.unicode_str);*/
		str_len = min(key->name.length, (unsigned) EMMCFS_CAT_MAX_NAME);
		memcpy(new_name, key->name.unicode_str,
			min(key->name.length, (unsigned) EMMCFS_CAT_MAX_NAME));
		new_name[str_len] = 0;
		EMMCFS_BUG_ON(EMMCFS_I(inode)->name);
		EMMCFS_I(inode)->name = new_name;
		EMMCFS_I(inode)->parent_id = le64_to_cpu(key->parent_id);
	}
	EMMCFS_I(inode)->hlink_id = hlink_id;

	inode->i_mode = le16_to_cpu(folder_val->permissions.file_mode);
	inode->i_uid = (uid_t)le32_to_cpu(folder_val->permissions.uid);
	inode->i_gid = (uid_t)le32_to_cpu(folder_val->permissions.gid);
	inode->i_nlink = le64_to_cpu(folder_val->links_count);
	/* TODO for Klepikov set root inode nlinlk*/
	if (inode->i_ino == EMMCFS_ROOT_INO)
		inode->i_nlink = 1;
	/* TODO - set correct time */
	inode->i_mtime.tv_sec =
			le64_to_cpu(folder_val->modification_time.seconds);
	inode->i_atime.tv_sec =
			le64_to_cpu(folder_val->access_time.seconds);
	inode->i_ctime.tv_sec =
			le64_to_cpu(folder_val->creation_time.seconds);

	inode->i_mtime.tv_nsec =
			le64_to_cpu(folder_val->modification_time.nanoseconds);
	inode->i_atime.tv_nsec =
			le64_to_cpu(folder_val->access_time.nanoseconds);
	inode->i_ctime.tv_nsec =
			le64_to_cpu(folder_val->creation_time.nanoseconds);

	if (rec_type & EMMCFS_CATALOG_FILE_RECORD) {
		struct emmcfs_catalog_file_record *file_val = (void *)comm_val;
#ifdef CONFIG_EMMCFS_CHECK_FRAGMENTATION
		struct emmcfs_inode_info *inode_info;
		long block_count_in_fork = 0;
		int count;
#endif
		inode->i_op = (rec_type & EMMCFS_CATALOG_FILE_RECORD_SYMLINK) ?
				&emmcfs_symlink_inode_operations :
				&emmcfs_file_inode_operations;
		inode->i_mapping->a_ops = &emmcfs_aops;
		ret = emmcfs_parse_fork(inode, &file_val->data_fork);
		if (S_ISREG(inode->i_mode) || S_ISLNK(inode->i_mode))
			inode->i_fop = &emmcfs_file_operations;
		else
			init_special_inode(inode, inode->i_mode, inode->i_rdev);
#ifdef CONFIG_EMMCFS_CHECK_FRAGMENTATION
		inode_info = EMMCFS_I(inode);

		for (count = 0; count < inode_info->fork.used_extents; count++)
			block_count_in_fork +=
				inode_info->fork.extents[count].block_count;

		if (block_count_in_fork > inode_info->fork.total_block_count)
			sbi->in_extents_overflow++;
		else
			sbi->in_fork[inode_info->fork.used_extents - 1]++;
#endif
	} else {
		EMMCFS_BUG_ON(rec_type != EMMCFS_CATALOG_FOLDER_RECORD);

		inode->i_size = (loff_t)le64_to_cpu(
				folder_val->total_items_count);
		inode->i_op = &emmcfs_dir_inode_operations;
		inode->i_fop = &emmcfs_dir_operations;
	}

	if (page != NULL)
		/* it was hardlink */
		kunmap(page);
err_exit:
	if (inode->i_ino == EMMCFS_ROOT_INO)
		emmcfs_put_bnode(fd->bnode);
	EMMCFS_LOG_FUNCTION_END(sbi, ret);
	return ret;
}

/**
 * @brief		Method to read inode to inode cache.
 * @param [in] 	sb	Pointer to superblock
 * @param [in] 	ino	The inode number
 * @return		Returns pointer to inode on success,
 * 			ERR_PTR(errno) on failure
 */
struct inode *emmcfs_iget(struct super_block *sb, unsigned long ino,
		struct emmcfs_find_data *find_data)
{
	struct inode		*inode;
	int			ret = 0;
	struct emmcfs_find_data *fdat;
	struct emmcfs_sb_info			*sbi = sb->s_fs_info;
	gfp_t gfp_mask;

	EMMCFS_LOG_FUNCTION_START(sbi);
	if (!find_data) {
		fdat = kzalloc(sizeof(*fdat), GFP_KERNEL);
		if (!fdat) {
			EMMCFS_LOG_FUNCTION_END(sbi, -ENOMEM);
			return ERR_PTR(-ENOMEM);
		}
	} else {
		fdat = find_data;
	}

	EMMCFS_DEBUG_INO("inode #%lu", ino);
	inode = iget_locked(sb, ino);

	if (!inode) {
		ret = -ENOMEM;
		goto err_exit_no_fail;
	}

	if (!(inode->i_state & I_NEW))
		goto exit;


	inode->i_mode = 0;
	/* Auxilary inodes */

	if (ino < EMMCFS_1ST_FILE_INO){
		/* Metadata pages can not be migrated */
		gfp_mask = (mapping_gfp_mask(inode->i_mapping)
				& ~GFP_MOVABLE_MASK);
		mapping_set_gfp_mask(inode->i_mapping, gfp_mask);
	}

	switch (ino) {
	case EMMCFS_CAT_TREE_INO:
		inode->i_mapping->a_ops = &emmcfs_aops_special;
		ret = emmcfs_parse_fork(inode, &EMMCFS_RAw_EXSB(sbi)->
				catalog_tree_fork);
		if (ret)
			goto err_exit;
		/* emmcfs_parse_fork already copied size in blocks
		inode->i_size *= sbi->block_size; */

		break;
	case EMMCFS_LEB_BITMAP_INO:
		inode->i_mapping->a_ops = &emmcfs_aops_special;
		inode->i_size = sbi->lebs_bm_blocks_count * sbi->block_size;
		ret = emmcfs_parse_fork(inode, &EMMCFS_RAw_EXSB(sbi)->
				lebs_bitmap_fork);
		if (ret)
			goto err_exit;
		break;
	case EMMCFS_EXTENTS_TREE_INO:
		inode->i_mapping->a_ops = &emmcfs_aops_special;

		ret = emmcfs_parse_fork(inode, &EMMCFS_RAw_EXSB(sbi)->
				extents_overflow_tree_fork);
		if (ret)
			goto err_exit;
		break;
	case EMMCFS_FREE_INODE_BITMAP_INO:
		inode->i_mapping->a_ops = &emmcfs_aops_special;

		ret = emmcfs_parse_fork(inode, &EMMCFS_RAw_EXSB(sbi)->
				inode_num_bitmap_fork);
		if (ret)
			goto err_exit;
		break;

	case EMMCFS_HARDLINKS_AREA_INO:
		inode->i_mapping->a_ops = &emmcfs_aops_special;

		ret = emmcfs_parse_fork(inode, &EMMCFS_RAw_EXSB(sbi)->
				hlinks_fork);
		if (ret)
			goto err_exit;

		break;
	default:
		ret = emmcfs_read_inode(inode, fdat);
		if (ret)
			goto err_exit;
		break;
	}

	if (inode->i_ino < EMMCFS_1ST_FILE_INO)
		EMMCFS_I(inode)->hlink_id = INVALID_HLINK_ID;

	EMMCFS_I(inode)->is_new = 0;
	unlock_new_inode(inode);

exit:
	if (!find_data)
		kfree(fdat);
	EMMCFS_LOG_FUNCTION_END(sbi, 0);
	return inode;

err_exit:
	iget_failed(inode);
err_exit_no_fail:
	EMMCFS_DEBUG_INO("inode #%lu read FAILED", ino);
	if (!find_data)
		kfree(fdat);
	EMMCFS_LOG_FUNCTION_END(sbi, ret);
	return ERR_PTR(ret);
}
