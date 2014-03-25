/**
 * @file	fs/emmcfs/bnode.c
 * @brief	Basic B-tree node operations.
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
#include <linux/buffer_head.h>
#include <linux/vmalloc.h>

#include "emmcfs.h"
#include "debug.h"
#include "btree.h"

#include <linux/mmzone.h>

#if defined(CONFIG_EMMCFS_CRC_CHECK)
inline __le32 __get_checksum_offset(struct emmcfs_bnode *node)
{
	return node->host->node_size_bytes - EMMCFS_BNODE_FIRST_OFFSET;
}

inline __le32 *__get_checksum_addr(struct emmcfs_bnode *node)
{
	return node->data + __get_checksum_offset(node);
}

inline void __update_checksum(struct emmcfs_bnode *node)
{
	*__get_checksum_addr(node) = crc32(0, node->data,
			__get_checksum_offset(node));
}

inline __le32 __get_checksum(struct emmcfs_bnode *node)
{
	return *__get_checksum_addr(node);
}

inline __le32 __get_checksum_offset_from_btree(struct emmcfs_btree
		*btree)
{
	return btree->node_size_bytes -	EMMCFS_BNODE_FIRST_OFFSET;
}

inline __le32 *__get_checksum_addr_from_data(void *bnode_data,
		struct emmcfs_btree *btree)
{
	return bnode_data + __get_checksum_offset_from_btree(btree);
}

inline __le32 __calc_crc_for_bnode(struct emmcfs_bnode *bnode)
{
	return crc32(0, bnode->data, __get_checksum_offset(bnode));
}
inline void emmcfs_sign_bnode(void *bnode_data,
		struct emmcfs_btree *btree)
{
	*__get_checksum_addr_from_data(bnode_data, btree) =
		crc32(0, bnode_data, __get_checksum_offset_from_btree(btree));
}
#endif

/**
 * @brief 			Get page from cache or create page and
 * 				buffers for it.
 * @param [in] 	mapping		Mapping which this page should belong to
 * @param [in] 	page_index	Page index within this mapping
 * @return 			Returns pointer to page in case of success,
 * 				error code otherwise
 */
struct page *emmcfs_alloc_new_page(struct address_space *mapping,
					     pgoff_t page_index)
{
	struct page *page;
	struct inode *inode = mapping->host;
	struct buffer_head *head, *bh;
	sector_t iblock = ((sector_t) page_index) << (PAGE_CACHE_SHIFT -
			inode->i_sb->s_blocksize_bits);
	gfp_t gfp_mask = ((mapping_gfp_mask(mapping) &
			~(__GFP_FS | __GFP_MOVABLE)) | __GFP_ZERO);

	page = find_or_create_page(mapping, page_index, gfp_mask);

	if (!page)
		return ERR_PTR(-ENOMEM);

	SetPageUptodate(page);
	if (!page_has_buffers(page))
		create_empty_buffers(page, inode->i_sb->s_blocksize, 0);

	head = bh = page_buffers(page);

	do {
		if (!buffer_mapped(bh)) {
			int ret = emmcfs_get_block(inode, iblock++, bh, 1);
			if (ret) {
				unlock_page(page);
				return ERR_PTR(ret);
			}
		}
	} while ((bh = bh->b_this_page) != head);

	unlock_page(page);

	return page;
}

/**
 * @brief 		Extract bnode into memory.
 * @param [in] 	btree	The B-tree from which bnode is to be extracted
 * @param [in] 	node_id	Id of extracted bnode
 * @param [in] 	create	Flag if to create (1) or get existing (0) bnode
 * @param [in] 	mode	Mode in which the extacted bnode will be used
 * @return 		Returns pointer to bnode in case of success,
 * 			error code otherwise
 */
static struct emmcfs_bnode *__get_bnode(struct emmcfs_btree *btree,
	__u32 node_id, int create, enum emmcfs_get_bnode_mode mode)
{
	struct inode *inode = btree->inode;
	struct emmcfs_bnode *node;
	struct page **pages = NULL;
	void *err_ret_code = NULL;
	u16 *magic;
	unsigned int i;
	pgoff_t page_index;

	EMMCFS_LOG_FUNCTION_START(btree->sbi);

	if (!inode) {
		if (!is_sbi_flag_set(btree->sbi, IS_MOUNT_FINISHED))
			return ERR_PTR(-EINVAL);
		else
			EMMCFS_BUG();
	}

	node = kmalloc(sizeof(struct emmcfs_bnode), GFP_KERNEL);
	if (!node) {
		EMMCFS_LOG_FUNCTION_END(btree->sbi, -ENOMEM);
		return ERR_PTR(-ENOMEM);
	}

	page_index = node_id * btree->pages_per_node;

	pages = kzalloc(sizeof(*pages) * btree->pages_per_node, GFP_KERNEL);
	if (!pages) {
		struct emmcfs_bnode *ret = ERR_PTR(-ENOMEM);
		kfree(node);
		EMMCFS_LOG_FUNCTION_END(btree->sbi, -ENOMEM);
		return ret;
	}

	for (i = 0; i < btree->pages_per_node ; i++) {
		if (create) {
			pages[i] = emmcfs_alloc_new_page(inode->i_mapping,
					page_index + i);
		} else {
			pages[i] = read_mapping_page(inode->i_mapping,
					page_index + i, NULL);
		}
		if (IS_ERR(pages[i])) {
			err_ret_code = pages[i];
			goto err_exit;
		}
	}

	node->data = vmap(pages, btree->pages_per_node, VM_MAP, PAGE_KERNEL);
	if (!node->data) {
		EMMCFS_ERR("unable to vmap %d pages", btree->pages_per_node);
		err_ret_code = ERR_PTR(-ENOMEM);
		goto err_exit;
	}

#if defined(CONFIG_EMMCFS_DEBUG)
	if (create) {	
		__u16 poison_val;
		__u16 max_poison_val = btree->pages_per_node << 
			(PAGE_CACHE_SHIFT - 1);
		__u16 *curr_pointer = node->data;

		for (poison_val = 0; poison_val < max_poison_val;
				poison_val++, curr_pointer++)
			*curr_pointer = poison_val;
	}
#endif

	magic = node->data;
	if (create) {
		*magic = EMMCFS_NODE_DESCR_MAGIC;
	} else if (node_id != 0 && *magic != EMMCFS_NODE_DESCR_MAGIC) {
		err_ret_code = ERR_PTR(-ENOENT);
		goto err_exit_vunmap;
	}

	node->pages = pages;
	node->host = btree;
	node->node_id = node_id;

	node->mode = mode;

#if defined(CONFIG_EMMCFS_CRC_CHECK)
	if (create)
		__update_checksum(node);
	if(!PageDirty(node->pages[0])) {
		__le32 from_disk = __get_checksum(node);
 		__le32 calculated = __calc_crc_for_bnode(node);
		if (from_disk != calculated) {
			EMMCFS_ERR("Btree Inode id: %lu Bnode id: %u",
					btree->inode->i_ino, node_id);
			EMMCFS_ERR("Expected %u, recieved: %u",
					calculated, from_disk);
			if (!is_sbi_flag_set(btree->sbi, IS_MOUNT_FINISHED))
				return ERR_PTR(-EINVAL);
			else
				EMMCFS_BUG();
		}
	}
#endif

	EMMCFS_LOG_FUNCTION_END(btree->sbi, 0);

	return node;

err_exit_vunmap:
	vunmap(node->data);
err_exit:
	for (i = 0; i < btree->pages_per_node; i++) {
		if (pages[i] && !IS_ERR(pages[i]))
			page_cache_release(pages[i]);
	}
	kfree(pages);
	kfree(node);
	EMMCFS_LOG_FUNCTION_END(btree->sbi, PTR_ERR(err_ret_code));
	return err_ret_code;
}

/**
 * @brief 		Extract bnode into memory from cache.
 * @param [in] 	btree	The B-tree from which bnode is to be extracted
 * @param [in] 	node_id	Id of extracted bnode
 * @param [in] 	mode	Mode in which the extacted bnode will be used
 * @return 		Returns pointer to bnode in case of success,
 * 			error code otherwise
 */
static struct emmcfs_bnode *get_bnode_cache(struct emmcfs_btree *btree,
		__u32 node_id, enum emmcfs_get_bnode_mode mode)
{
	struct emmcfs_bnode *ret = NULL;

	if (node_id >= EMMCFS_BNODE_CACHE_ITEMS)
		return NULL;

	if (btree->stupid_cache[node_id]) {
		ret = btree->stupid_cache[node_id];
		ret->mode = mode;
		/*printk(KERN_ERR "bnode #%lu from cache", node_id);*/
	}

	return ret;
}

/**
 * @brief 		Place bnode into bnode cache.
 * @param [in]	bnode	Node to be added in cashe
 * @return	void
 */
static void add_bnode_cache(struct emmcfs_bnode *bnode)
{
	if (bnode->node_id < EMMCFS_BNODE_CACHE_ITEMS)
		bnode->host->stupid_cache[bnode->node_id] = bnode;
}

/**
 * @brief 		Interface for extracting bnode into memory.
 * @param [in] 	btree	B-tree from which bnode is to be extracted
 * @param [in] 	node_id	Id of extracted bnode
 * @param [in] 	mode	Mode in which the extacted bnode will be used
 * @return 		Returns pointer to bnode in case of success,
 *			error code otherwise
 */
struct emmcfs_bnode *emmcfs_get_bnode(struct emmcfs_btree *btree,
		__u32 node_id, enum emmcfs_get_bnode_mode mode)
{
	struct emmcfs_bnode *ret;

	if (node_id && !test_bit(node_id, btree->bitmap->data))
		return ERR_PTR(-ENOENT);

	EMMCFS_LOG_FUNCTION_START(btree->sbi);
	ret = get_bnode_cache(btree, node_id, mode);
	if (!ret) {
		ret = __get_bnode(btree, node_id, 0, mode);
		/*printk(KERN_ERR "bnode #%lu from disk", node_id);*/
		/*atomic_set(&ret->refs_count, 1);*/
		if (IS_ERR(ret)) {
			EMMCFS_LOG_FUNCTION_END(btree->sbi, 0);
			return ret;
		}
		add_bnode_cache(ret);
	} else {
		/* !!!
		 * Algorithms expects that nobody will take any bnode
		 * recursevely, so, its wrong behaviour if refs_count > 1 */

		/*atomic_inc(&ret->refs_count);*/
		/*EMMCFS_BUG_ON(mode == EMMCFS_BNODE_MODE_RW &&*/
				/*atomic_read(&ret->refs_count) > 1);*/
	}

	if ((mode == EMMCFS_BNODE_MODE_RW) && (!PageDirty(ret->pages[0]))
			&& node_id != 0) {
		int err = 0;
		EMMCFS_DEBUG_SNAPSHOT("Try to add chunk %d", node_id);
		err = EMMCFS_ADD_CHUNK(btree->inode, ret->pages,
				btree->pages_per_node);

		if (err) {
			emmcfs_put_bnode(ret);
			ret = ERR_PTR(err);
		}
	}

	EMMCFS_LOG_FUNCTION_END(btree->sbi, 0);
	return ret;
}

/**
 * @brief 		Create, reserve and prepare new bnode.
 * @param [in] 	btree	The B-tree from which bnode is to be prepared
 * @return 		Returns pointer to bnode in case of success,
 *			error code otherwise
 */
struct emmcfs_bnode *emmcfs_alloc_new_bnode(struct emmcfs_btree *btree)
{
	struct emmcfs_bnode_bitmap *bitmap = btree->bitmap;
	struct emmcfs_bnode *bnode;
	int err;
	EMMCFS_LOG_FUNCTION_START(btree->sbi);

	if (bitmap->free_num == 0) {
		EMMCFS_LOG_FUNCTION_END(btree->sbi, -ENOSPC);
		return ERR_PTR(-ENOSPC);
	}

	err = EMMCFS_ADD_CHUNK(btree->inode, btree->head_bnode->pages,
			btree->pages_per_node);
	if (err)
		return ERR_PTR(err);

	/*spin_lock(&bitmap->spinlock);*/
	EMMCFS_BUG_ON(test_and_set_bit(bitmap->first_free_id, bitmap->data));
	/*spin_unlock(&bitmap->spinlock);*/
	/* TODO bitmap->data  directly points to head bnode memarea so head
	 * bnode is always kept in memory. Maybe better to split totally
	 * on-disk and runtime structures and sync them only in sync-points
	 * (sync callback, write_inode) ?
	 */
	emmcfs_mark_bnode_dirty(btree->head_bnode);

	bnode = __get_bnode(btree, bitmap->first_free_id, 1,
			EMMCFS_BNODE_MODE_RW);
	if (IS_ERR(bnode)) {
		clear_bit(bitmap->first_free_id, bitmap->data);
		EMMCFS_LOG_FUNCTION_END(btree->sbi, PTR_ERR(bnode));
		return bnode;
	}

	/* Updata bitmap information */
	bitmap->first_free_id = find_next_zero_bit(bitmap->data,
			bitmap->bits_num, bitmap->first_free_id);
	bitmap->free_num--;
	emmcfs_mark_bnode_dirty(btree->head_bnode);

	/*atomic_set(&bnode->refs_count, 1);*/
	add_bnode_cache(bnode);
	EMMCFS_LOG_FUNCTION_END(btree->sbi, 0);
	return bnode;
}

/**
 * @brief 			Free bnode structure.
 * @param [in] 	bnode		Node to be freed
 * @param [in] 	keep_in_cache	Whether to save freeing bnode in bnode cache
 * @return	void
 */
static void __put_bnode(struct emmcfs_bnode *bnode, int keep_in_cache)
{
	unsigned int i;
	EMMCFS_LOG_FUNCTION_START(bnode->host->sbi);

	EMMCFS_BUG_ON(!bnode->data || !bnode->pages);

	/*atomic_dec(&bnode->refs_count);*/
	if (bnode->node_id < EMMCFS_BNODE_CACHE_ITEMS) {
		if (keep_in_cache) {
			EMMCFS_LOG_FUNCTION_END(bnode->host->sbi, 0);
			return;
		} else {
			bnode->host->stupid_cache[bnode->node_id] = 0;
		}
	}

	vunmap(bnode->data);
	for (i = 0; i < bnode->host->pages_per_node; i++)
		page_cache_release(bnode->pages[i]);

	bnode->data = NULL;

	kfree(bnode->pages);
	EMMCFS_LOG_FUNCTION_END(bnode->host->sbi, 0);
	kfree(bnode);
}

/**
 * @brief 		Interface for freeing bnode structure.
 * @param [in] 	bnode	Node to be freed
 * @return	void
 */
void emmcfs_put_bnode(struct emmcfs_bnode *bnode)
{
	__put_bnode(bnode, 1);
}

/**
 * @brief 		Interface for freeing bnode structure without
 *			saving it into cache.
 * @param [in] 	bnode	Node to be freed
 * @return	void
 */
void emmcfs_put_cache_bnode(struct emmcfs_bnode *bnode)
{
	__put_bnode(bnode, 0);
}

/**
 * @brief 			preallocate a reseve for splitting btree
 * @param btee			which btree needs bnodes reserve
 * @param alloc_to_bnode_id	at least this bnode will be available after
 * 				expanding
 * @return			0 on success or err code	
 * */
int emmcfs_prealloc_bnode_reserve(struct emmcfs_btree *btree,
		__u32 alloc_to_bnode_id)
{
	struct emmcfs_bnode *bnode;

	bnode = __get_bnode(btree, alloc_to_bnode_id, 1, EMMCFS_BNODE_MODE_RW);

	if (IS_ERR(bnode))
		return PTR_ERR(bnode);

	emmcfs_put_cache_bnode(bnode);

	return 0;
}
/**
 * @brief 		Mark bnode as free.
 * @param [in] 	bnode	Node to be freed
 * @return	void
 */
int emmcfs_destroy_bnode(struct emmcfs_bnode *bnode)
{
	__u32 bnode_id = bnode->node_id;
	struct emmcfs_btree *btree = bnode->host;
	struct emmcfs_bnode_bitmap *bitmap = btree->bitmap;
	int err = 0;

	EMMCFS_LOG_FUNCTION_START(btree->sbi);
	EMMCFS_BUG_ON(bnode_id == EMMCFS_INVALID_NODE_ID);

	/* TODO: use remove_from_page_cache here, when we have optimised
	 * algorithm for getting bnodes */
	__put_bnode(bnode, 0);

	/*spin_lock(&bitmap->spinlock);*/
	err = EMMCFS_ADD_CHUNK(btree->inode, btree->head_bnode->pages,
				btree->pages_per_node);

	if (err)
		return err;

	if (!test_and_clear_bit(bnode_id, bitmap->data)) {
		if (!is_sbi_flag_set(bnode->host->sbi, IS_MOUNT_FINISHED))
			return -EFAULT;
		else
			EMMCFS_BUG();
	}

	/* TODO - the comment same as at emmcfs_alloc_new_bnode */
	emmcfs_mark_bnode_dirty(btree->head_bnode);
	if (bitmap->first_free_id > bnode_id)
		bitmap->first_free_id = bnode_id;
	bitmap->free_num++;
	EMMCFS_LOG_FUNCTION_END(btree->sbi, 0);
	/*spin_unlock(&bitmap->spinlock);*/

	/* TODO - seems that it's non-necessary, but let it stay under TODO */
	/*mutex_unlock(btree->node_locks + node_id);*/

	return err;
}

/**
 * @brief 			Build free bnode bitmap runtime structure.
 * @param [in] 	data		Pointer to reserved memory area satisfying
 *				to keep bitmap
 * @param [in] 	start_bnode_id	Starting bnode id to fix into bitmap
 * @param [in] 	size_in_bytes	Size of reserved memory area in bytes
 * @param [in] 	host_bnode	Pointer to bnode containing essential info
 * @return 			Returns pointer to bitmap on success,
 *				error code on failure
 */
struct emmcfs_bnode_bitmap *build_free_bnode_bitmap(void *data,
		__u32 start_bnode_id, __u64 size_in_bytes,
		struct emmcfs_bnode *host_bnode)
{
	struct emmcfs_bnode_bitmap *bitmap;

	EMMCFS_LOG_FUNCTION_START(host_bnode->host->sbi);

	if (!host_bnode)
		return ERR_PTR(-EINVAL);

	bitmap = kzalloc(sizeof(*bitmap), GFP_KERNEL);
	if (!bitmap) {
		EMMCFS_LOG_FUNCTION_END(host_bnode->host->sbi, -ENOMEM);
		return ERR_PTR(-ENOMEM);
	}

	bitmap->data = data;
	bitmap->size = size_in_bytes;
	bitmap->bits_num = bitmap->size * 8;

	bitmap->start_id = start_bnode_id;
	bitmap->end_id = start_bnode_id + bitmap->bits_num;

	bitmap->free_num = bitmap->bits_num - __bitmap_weight(bitmap->data,
			bitmap->bits_num);
	bitmap->first_free_id = find_first_zero_bit(bitmap->data,
			bitmap->bits_num) + start_bnode_id;

	bitmap->host = host_bnode;
	spin_lock_init(&bitmap->spinlock);

	EMMCFS_LOG_FUNCTION_END(host_bnode->host->sbi, 0);
	return bitmap;
}

/**
 * @brief 		Clear memory allocated for bnode bitmap.
 * @param [in] 	bitmap	Bitmap to be cleared
 * @return	void
 */
void emmcfs_destroy_free_bnode_bitmap(struct emmcfs_bnode_bitmap *bitmap)
{
	kfree(bitmap);
}

/**
 * @brief 		Mark bnode as dirty (data on disk and in memory
 *			differ).
 * @param [in] 	bnode	Node to be marked as dirty
 * @return	void
 */
void emmcfs_mark_bnode_dirty(struct emmcfs_bnode *node)
{
	unsigned int i;

	EMMCFS_BUG_ON(!node);
	EMMCFS_LOG_FUNCTION_START(node->host->sbi);

	EMMCFS_BUG_ON(node->mode != EMMCFS_BNODE_MODE_RW);

	for (i = 0; i < node->host->pages_per_node; i++)
		set_page_dirty_lock(node->pages[i]);

	EMMCFS_LOG_FUNCTION_END(node->host->sbi, 0);
}
