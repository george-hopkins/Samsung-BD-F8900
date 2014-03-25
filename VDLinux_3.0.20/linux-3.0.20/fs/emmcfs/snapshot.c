/**
 * @file	fs/emmcfs/snapshot.c
 * @brief	The eMMCFS snapshot logic.
 * @author	Ivan Arishchenko i.arishchenko@samsung.com
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

#include <linux/fs.h>
#include <linux/rbtree.h>
#include <linux/vfs.h>
#include <linux/spinlock_types.h>
#include <linux/vmalloc.h>
#include <linux/buffer_head.h>
#include <linux/bio.h>
#include <linux/blkdev.h>
#include <linux/pagemap.h>
#include <linux/crc32.h>
#include <linux/slab.h>
#include <linux/version.h>

#include "emmcfs.h"

/**
 * @brief			Insert transaction to rb-tree.
 * @param [in]	root		Root of the tree to insert.
 * @param [in]	data		The emmcfs_transaction structure to insert.
 * @return			Returns 0 on success, errno on failure.
 */
static inline int transaction_insert(struct emmcfs_sb_info *sbi,
		struct rb_root *root,
		struct	emmcfs_transaction_data *data)
{
	struct rb_node **new = &(root->rb_node), *parent = NULL;

	if (!data) {
		if (!is_sbi_flag_set(sbi, IS_MOUNT_FINISHED))
			return -EFAULT;
		else
			BUG();
	}

	/* Figure out where to put new node */
	while (*new) {
		struct	emmcfs_transaction_data *this = container_of(*new,
			struct	emmcfs_transaction_data, transaction_node);

		parent = *new;
		if (data->on_volume < this->on_volume)
			new = &((*new)->rb_left);
		else if (data->on_volume > this->on_volume)
			new = &((*new)->rb_right);
		else
			return -EEXIST;
		}

	/* Add new node and rebalance tree. */
	rb_link_node(&data->transaction_node, parent, new);
	rb_insert_color(&data->transaction_node, root);

	return 0;
}

/**
 * @brief		Search : task->pid already exist or not.
 * @param [in]	root	Root of the tree to insert.
 * @return		TODO Returns 0 on success, errno on failure.
 */
static inline struct emmcfs_task_log *search_for_task_log(struct rb_root *root)
{
	struct task_struct *current_task = current;
	struct rb_node **new = &(root->rb_node);
	struct emmcfs_task_log *this = NULL;


	/* Figure out where to put new node */
	while (*new) {
		this = container_of(*new, struct emmcfs_task_log, node);

		if (current_task->pid < this->pid)
			new = &((*new)->rb_left);
		else if (current_task->pid > this->pid)
			new = &((*new)->rb_right);
		else
			return this; /* no such pid */
	}

	/* pid already exist */
	return NULL;
}

/**
 * @brief		TODO Search : task->pid already exist or not
 * @param [in]	root	TODO Root of the tree to insert
 * @param [in]	data	TODO
 * @return		Returns 0 on success, errno on failure.
 */
static inline int add_task_log(struct rb_root *root,
		struct emmcfs_task_log *data)
{
	struct rb_node **new = &(root->rb_node), *parent = NULL;

	EMMCFS_BUG_ON(!data);

	/* Figure out where to put new node */
	while (*new) {
		struct emmcfs_task_log *this = container_of(*new,
				struct emmcfs_task_log, node);

		parent = *new;
		if (data->pid < this->pid)
			new = &((*new)->rb_left);
		else if (data->pid > this->pid)
			new = &((*new)->rb_right);
		else
			return -EEXIST;
	}

	/* Add new node and rebalance tree. */
	rb_link_node(&data->node, parent, new);
	rb_insert_color(&data->node, root);

	return 0;
}

/**
 * @brief		TODO 
 * @param [in]	root	TODO Transaction rb-tree root.
 * @return	void
 * */
static inline void destroy_task_log(struct rb_root *root)
{
	struct rb_node *current_node;

	if (!root)
		return;

	current_node = rb_first(root);
	while (current_node) {
		struct rb_node *next_node = rb_next(current_node);
		struct emmcfs_task_log *task_log;

		task_log = rb_entry(current_node,
				struct emmcfs_task_log, node);

		rb_erase(current_node, root);
		kfree(task_log);

		current_node = next_node;
	}
}

/**
 * @brief		Clear transaction rb-tree.
 * @param [in]	rb_root	Transaction rb-tree root.
 * @return	void
 * */
static void clear_transaction_tree(struct rb_root *rb_root)
{
	struct rb_node *current_node;

	if (!rb_root)
		return;

	current_node = rb_first(rb_root);

	while (current_node) {
		struct rb_node *next_node = rb_next(current_node);
		struct emmcfs_transaction_data *transaction;

		transaction = rb_entry(current_node,
			struct emmcfs_transaction_data, transaction_node);

		rb_erase(current_node, rb_root);
		kfree(transaction);

		current_node = next_node;
	}
}

/**
 * @brief			Convert transaction rb-tree to massive
 * 				(on-disk layout).
 * @param [in]	rb_root		Transaction rb-tree root.
 * @param [in]	destination	Destination massive.
 * @return			Returns 0 on success, errno on failure.
 * */
static int transaction_tree_to_snapshot(struct rb_root *rb_root,
		struct emmcfs_snapshot_descriptor *snapshot,int mount_count,
		int version)
{
	struct rb_node *current_node;
	struct emmcfs_transaction *current_trunsaction;
	__le64 emmcfs_snapshot_magic = EMMCFS_SNAPSHOT_MAGIC;
	__le32 checksum;
	unsigned int chuck_count = 0;

	if (!rb_root)
		return -EINVAL;

	/* copy magic number */
	memcpy(snapshot->signature, (char *)&emmcfs_snapshot_magic,
				sizeof(snapshot->signature));

	current_trunsaction = &snapshot->first_transaction;

	current_node = rb_first(rb_root);

	while (current_node) {
		struct rb_node *next_node = rb_next(current_node);
		struct emmcfs_transaction_data *transaction;

		transaction = rb_entry(current_node,
			struct emmcfs_transaction_data, transaction_node);

		current_trunsaction->page_count =
				cpu_to_le16(transaction->pages_count);
		current_trunsaction->on_snapshot =
				cpu_to_le64(transaction->on_snapshot);
		current_trunsaction->on_volume =
				cpu_to_le64(transaction->on_volume);
		chuck_count++;

		current_trunsaction++;
		current_node = next_node;
	}

	if (!chuck_count)
		return -EINVAL;

	snapshot->transaction_count = cpu_to_le16(chuck_count);
	snapshot->checksum_offset = cpu_to_le16((void *)current_trunsaction -
			(void *)snapshot);

	snapshot->mount_count = mount_count;
	snapshot->version = cpu_to_le32(version);

	/* sign snapshot : crc32 */
	checksum = crc32(0, snapshot, snapshot->checksum_offset);
	*((__le32 *)current_trunsaction) = cpu_to_le32(checksum);

	return 0;
}

/**
 * @brief		Get page start block by they index.
 * @param [in] ino_no	The inode number.
 * @param [in] iblock	Logical block number.
 * @return		Returns start block for given iblock.
 */
static sector_t get_start_block(struct inode *inode, pgoff_t iblock)
{
	struct emmcfs_inode_info *inode_info = EMMCFS_I(inode);
	__u32 max_block;
	sector_t ret;

	EMMCFS_LOG_FUNCTION_START(EMMCFS_SB(inode->i_sb));
	if (iblock > inode_info->fork.total_block_count) {
		EMMCFS_LOG_FUNCTION_END(EMMCFS_SB(inode->i_sb), 0);
		return 0;
	}
	ret = emmcfs_find_old_block(inode_info, iblock,
			&max_block);
	EMMCFS_BUG_ON(!ret);
	EMMCFS_LOG_FUNCTION_END(EMMCFS_SB(inode->i_sb), ret);
	return ret;
}

/**
 * @brief		Stop snapshot.
 * @param [in] 	sbi	The eMMCFS super block info.
 * @return		Returns 0 on success, errno on failure.
 */
static int emmcfs_clear_snapshot(struct emmcfs_sb_info *sbi)
{
	struct page *page;
	int error = 0;
	EMMCFS_LOG_FUNCTION_START(sbi);
	page = alloc_page(GFP_KERNEL | __GFP_ZERO);

	if (!page)
		return -ENOMEM;

	lock_page(page);

	EMMCFS_DEBUG_SNAPSHOT("enter emmcfs_clear_snapshot");
	set_page_writeback(page);
	error = emmcfs_write_page(sbi->sb->s_bdev, page,
			EMMCFS_SNAPSHOT_START(sbi) <<
			(PAGE_CACHE_SHIFT - SECTOR_SIZE_SHIFT), 1, 0);

	unlock_page(page);
	__free_pages(page, 0);

	if (error)
		return -EFAULT;

	sbi->snapshot_info->page_index = sbi->snapshot_info->pages_per_head;


	if (is_sbi_flag_set(sbi, EXSB_DIRTY_SNAPSHOT)) {
		emmcfs_sync_second_super(sbi->sb);
		clear_sbi_flag(sbi, EXSB_DIRTY_SNAPSHOT);
	}

	EMMCFS_DEBUG_SNAPSHOT("exit emmcfs_clear_snapshot");
	EMMCFS_LOG_FUNCTION_END(sbi, error);
	return error;
}

#define PAGES_PER_HEAD_HISTORY	1

static inline unsigned int head_history_mask(void ) {

	return (((PAGES_PER_HEAD_HISTORY << (PAGE_CACHE_SHIFT -
			SECTOR_SIZE_SHIFT))) - 1);
}

/**
 * @brief		Commit snapshot.
 * @param [in] 	sbi	The eMMCFS super block info.
 * @return		Returns 0 on success, errno on failure.
 */
static int emmcfs_commit_snapshot(struct emmcfs_sb_info *sbi)
{
	struct emmcfs_snapshot_info *snapshot = sbi->snapshot_info;
	struct emmcfs_snapshot_descriptor *snapshot_head;
	spinlock_t *lock_transaction_tree =
			&sbi->snapshot_info->transaction_rbtree_lock;
	struct page **pages;
	unsigned int count;
	int error = 0;
	unsigned int pages_count;
	struct emmcfs_extended_super_block *exsb = EMMCFS_RAw_EXSB(sbi);
	EMMCFS_LOG_FUNCTION_START(sbi);

	pages_count = ((snapshot->transactions_count *
		sizeof(struct emmcfs_transaction) +
		sizeof(struct emmcfs_snapshot_descriptor) +
		(1 << PAGE_CACHE_SHIFT) - 1)) >> PAGE_CACHE_SHIFT;

	if(!snapshot->transactions_count) {
		EMMCFS_LOG_FUNCTION_END(sbi, error);
		return error;
	}

	if (is_sbi_flag_set(sbi, EXSB_DIRTY_SNAPSHOT))
		emmcfs_sync_first_super(sbi->sb);


	pages = kzalloc(sizeof(*pages) * pages_count, GFP_KERNEL);
	if (!pages) {
		EMMCFS_LOG_FUNCTION_END(sbi, -ENOMEM);
		return -ENOMEM;
	}

	for (count = 0; count < pages_count; count++) {
		pages[count] = alloc_page(GFP_KERNEL | __GFP_ZERO);
		lock_page(pages[count]);
	}


	snapshot_head = (struct emmcfs_snapshot_descriptor *)vmap(pages,
			pages_count, VM_MAP, PAGE_KERNEL);
	if (!snapshot_head) {
		EMMCFS_ERR("can not vmap snapshot head (%d pages)",
				pages_count);
		EMMCFS_BUG();
	}

	/* prepare snapshot head */
	spin_lock(lock_transaction_tree);
	error = transaction_tree_to_snapshot(&snapshot->snapshot_root,
			snapshot_head, exsb->mount_counter,
			snapshot->head_version);

	snapshot->head_version++;

	sbi->snapshot_info->transactions_count = 0;
	if (error) {
		spin_unlock(lock_transaction_tree);
		vunmap(snapshot_head);

		for (count = 0; count < pages_count; count++) {
			unlock_page(pages[count]);
			__free_page(pages[count]);
		}
		kfree(pages);
		EMMCFS_LOG_FUNCTION_END(sbi, error);
		return error;
	}

	clear_transaction_tree(&sbi->snapshot_info->snapshot_root);
	spin_unlock(lock_transaction_tree);
	vunmap(snapshot_head);

	for (count = 0; count < pages_count; count++)
		set_page_writeback(pages[count]);

	/* save snapshot head in last snapshot area block */
	error = emmcfs_write_page(sbi->sb->s_bdev, pages[0],
			((EMMCFS_SNAPSHOT_START(sbi) +
			EMMCFS_SNAPSHOT_LENGHT(sbi)) <<
			(PAGE_CACHE_SHIFT - SECTOR_SIZE_SHIFT)) -
			PAGES_PER_HEAD_HISTORY * SECTOR_PER_PAGE +
			(snapshot->head_version & head_history_mask()), 1, 0);

	set_page_writeback(pages[0]);

	error = emmcfs_write_snapshot_pages(sbi, pages,
			EMMCFS_SNAPSHOT_START(sbi) <<
			(PAGE_CACHE_SHIFT - SECTOR_SIZE_SHIFT),
			pages_count, 1);

	kfree(pages);
	EMMCFS_DEBUG_SNAPSHOT("emmcfs_commit_snapshot");
	EMMCFS_LOG_FUNCTION_END(sbi, error);
	return error;
}

/**
 * @brief			Restore single transaction.
 * @param [in] 	sbi		The eMMCFS super block info.
 * @param [in]	transaction	TODO
 * @param [in]	pade_index	TODO
 * @return			Returns 0 on success, errno on failure.
 */
static int restore_sigle_transaction(struct emmcfs_sb_info *sbi,
		struct emmcfs_transaction *transaction, pgoff_t page_index)
{
	struct page **pages;
	unsigned int count;
	int error;
	unsigned int page_count = le16_to_cpu(transaction->page_count);
	sector_t write_to = le64_to_cpu(transaction->on_volume);
	sector_t read_from = le64_to_cpu(transaction->on_snapshot);

	EMMCFS_LOG_FUNCTION_START(sbi);
	EMMCFS_DEBUG_TMP("RESTORE :P_count %d From_sector %lu /"
			"Write_sector %lu",
			page_count,
			(long unsigned int)read_from,
			(long unsigned int)write_to);

	pages = kzalloc(sizeof(*pages) * page_count, GFP_KERNEL);
	if (!pages) {
		error = -ENOMEM;
		goto err_exit;
	}
	for (count = 0; count < page_count; count++) {
		pages[count] = alloc_page(GFP_KERNEL | __GFP_ZERO);
		if (!pages[count]) {
			error = -ENOMEM;
			goto err_free_mem;
		}
		lock_page(pages[count]);
	}
	error = emmcfs_read_pages(sbi->sb->s_bdev, pages, read_from,
			page_count);
	if (error)
		goto free_pages;

	for (count = 0; count < page_count; count++)
		set_page_writeback(pages[count]);

	error = emmcfs_write_snapshot_pages(sbi, pages,
			write_to, page_count, 1);

	kfree(pages);
	EMMCFS_LOG_FUNCTION_END(sbi, 0);
	return error;

free_pages:
	for (count = 0; count < page_count; count++) {
		unlock_page(pages[count]);
		__free_pages(pages[count], 0);
	}

err_free_mem:
	kfree(pages);
err_exit:
	EMMCFS_LOG_FUNCTION_END(sbi, error);
	return error;
};


/**
 * @brief		TODO
 * @param [in] 	sbi	The eMMCFS super block info.
 * @return		Returns 0 on success, errno on failure.
 */
static int emmcfs_is_snapshot_present(struct emmcfs_sb_info *sbi)
{
	struct page *snapshot_head;
	struct emmcfs_snapshot_descriptor *head;
	int error = 0;
	__le32 emmcfs_check_magic = EMMCFS_SNAPSHOT_MAGIC;

	snapshot_head = alloc_page(GFP_KERNEL | __GFP_ZERO);
	if (!snapshot_head) {
		error = -ENOMEM;
		goto error_exit;
	}
	lock_page(snapshot_head);

	/* check snapshot magic */
	error = emmcfs_read_page(sbi->sb->s_bdev, snapshot_head,
			EMMCFS_SNAPSHOT_START(sbi) << (PAGE_CACHE_SHIFT -
			SECTOR_SIZE_SHIFT), 8, 0);
	if (error)
		goto error_exit_free_page;

	head = kmap(snapshot_head);

	/* check magic number */
	if (memcmp(head->signature, (char *)&emmcfs_check_magic,
				sizeof(head->signature)))
		/* No magic number -> no snapshot */
		error = 0;
	 else
		/* snapshot is present */
		error = 1;

	kunmap(snapshot_head);

error_exit_free_page:
	unlock_page(snapshot_head);
	__free_pages(snapshot_head, 0);
error_exit:
	return error;
}

/**
 * @brief		Restore volume to consistent state.
 * @param [in] 	sbi	The eMMCFS super block info.
 * @return		Returns 0 on success, errno on failure.
 */
static int emmcfs_restore_snapshot(struct emmcfs_sb_info *sbi)
{
	struct emmcfs_snapshot_info *snapshot = sbi->snapshot_info;
	struct emmcfs_snapshot_descriptor *snapshot_head;
	unsigned int transaction_count;
	unsigned int count = 0;
	struct emmcfs_transaction *current_transaction;

	__le32 checksum;
	__le32 on_disk_checksum;
	/*__le32 *p_on_disk_checksum;*/
	pgoff_t page_index;
	int error;
	struct page **pages;
	EMMCFS_LOG_FUNCTION_START(sbi);

	EMMCFS_DEBUG_SNAPSHOT("START recovery");
	error = emmcfs_is_snapshot_present(sbi);

	if (error != 1) {
		EMMCFS_LOG_FUNCTION_END(sbi, error);
		return error;
	}
	EMMCFS_DEBUG_SNAPSHOT("!!! Snapshot MAGIC is present ");

	pages = kzalloc(sizeof(*pages) * snapshot->pages_per_head, GFP_KERNEL);
	if (!pages) {
		EMMCFS_LOG_FUNCTION_END(sbi, -ENOMEM);
		return -ENOMEM;
	}

	for (count = 0; count < snapshot->pages_per_head; count++) {
		pages[count] = alloc_page(GFP_KERNEL | __GFP_ZERO);
		if (!pages[count]) {
			error = -ENOMEM;
			goto err_exit;
		}
		lock_page(pages[count]);
	}

	error = emmcfs_read_pages(sbi->sb->s_bdev, pages,
			EMMCFS_SNAPSHOT_START(sbi) << (PAGE_CACHE_SHIFT -
			SECTOR_SIZE_SHIFT), snapshot->pages_per_head);
	if (error)
		goto err_exit;

	snapshot->page_index = snapshot->pages_per_head;
	snapshot_head = (struct emmcfs_snapshot_descriptor *)vmap(pages,
			snapshot->pages_per_head, VM_MAP, PAGE_KERNEL);
	if (!snapshot_head) {
		EMMCFS_ERR("can not vmap snapshot head (%d pages)",
				snapshot->pages_per_head);
		EMMCFS_BUG();
	}

	/* check CRC32 checksum */
	checksum = crc32(0, snapshot_head,
			le16_to_cpu(snapshot_head->checksum_offset));

	on_disk_checksum = le32_to_cpu(*(__le32 *)((char *)snapshot_head +
			le16_to_cpu(snapshot_head->checksum_offset)));

	if (checksum != on_disk_checksum) {
		/* snapshot is broken */
		EMMCFS_ERR("Snapshot is broken shecksum - 0x%x, must be 0x%x\n",
				on_disk_checksum, checksum);
		error = -EINVAL;
		EMMCFS_BUG();
	}

	/* restore on- disk layout */
	EMMCFS_DEBUG_SNAPSHOT("Unclear unmount : recovery from snapshot");
	transaction_count = le16_to_cpu(snapshot_head->transaction_count);
	page_index = snapshot->pages_per_head;
	current_transaction = &snapshot_head->first_transaction;

	for (count = 0; count < transaction_count; count++) {
		error = restore_sigle_transaction(sbi,
				current_transaction, page_index);
		if (error)
			goto release_head;
		page_index += le16_to_cpu(
				current_transaction->page_count);
		current_transaction++;
	}

	error = emmcfs_clear_snapshot(sbi);
	if (error)
		goto err_exit;

	vunmap(snapshot_head);
	for (count = 0; count < snapshot->pages_per_head; count++) {
		unlock_page(pages[count]);
		__free_pages(pages[count], 0);
	}
	kfree(pages);

	EMMCFS_DEBUG_SNAPSHOT("STOP emmcfs_restore_snapshot");
	EMMCFS_LOG_FUNCTION_END(sbi, 0);
	return 0;

release_head:
	vunmap(snapshot_head);
err_exit:
	for (; count < snapshot->pages_per_head; count--) {
		unlock_page(pages[count]);
		__free_pages(pages[count], 0);
	}
	kfree(pages);
	EMMCFS_LOG_FUNCTION_END(sbi, error);
	return error;
}


/**
 * @brief			Add data chunk to snapshot.
 * @param [in] 	sbi		The eMMCFS super block info.
 * @param [in] 	inode		The VFS inode.
 * @param [in] 	original_pages	New snapshot data pages.
 * @param [in] 	pages_count	New data pages count.
 * @return			Returns 0 on success, errno on failure
 */
static int emmcfs_add_to_snapshot(struct emmcfs_sb_info *sbi,
		struct inode *inode, struct page **original_pages,
		unsigned int pages_count)
{
	struct page **pages = NULL;
	spinlock_t *lock_transaction_tree =
			&sbi->snapshot_info->transaction_rbtree_lock;
	struct rb_root *transaction_root = &sbi->snapshot_info->snapshot_root;
	struct	emmcfs_transaction_data *new_transation;
	sector_t start_block;
	void *destination_base;
	void *destination;
	void *source_base;
	void *source;
	int error = 0;
	unsigned int count = 0;

	EMMCFS_LOG_FUNCTION_START(sbi);
	start_block = get_start_block(inode, original_pages[0]->index);
	if (!start_block) {
		EMMCFS_LOG_FUNCTION_END(sbi, -EINVAL);
		return -EINVAL;
	}


	pages = kzalloc(sizeof(struct page *) * pages_count, GFP_KERNEL);
	if (!pages) {
		EMMCFS_LOG_FUNCTION_END(sbi, -ENOMEM);
		return -ENOMEM;
	}

	EMMCFS_DEBUG_SNAPSHOT("New transaction I: %lu",
				sbi->snapshot_info->page_index);

	new_transation = kzalloc(sizeof(struct	emmcfs_transaction_data),
			 GFP_KERNEL);

	if (!new_transation) {
		kfree(pages);
		EMMCFS_LOG_FUNCTION_END(sbi, -ENOMEM);
		return -ENOMEM;
	}

	/*set up transaction */
	new_transation->pages_count = pages_count;
	new_transation->on_volume = start_block << sbi->log_sectors_per_block;

	mutex_lock(&sbi->snapshot_info->insert_lock);

	spin_lock(lock_transaction_tree);
	/* insert new transaction to rb-tree,
	 * check : data already in snapshot or not */
	error = transaction_insert(sbi, transaction_root, new_transation);
	spin_unlock(lock_transaction_tree);

	if (error) {
		if (error == -EEXIST)
			error = 0;
		kfree(pages);
		kfree(new_transation);
		mutex_unlock(&sbi->snapshot_info->insert_lock);
		goto exit;
	}
	sbi->snapshot_info->transactions_count++;

	for (count = 0; count < pages_count; count++) {
		pages[count] = alloc_page(GFP_KERNEL | __GFP_ZERO);
		if (IS_ERR(pages[count])) {
			error = -ENOMEM;
			/* remove canceled transaction*/
			rb_erase(&new_transation->transaction_node,
					transaction_root);
			kfree(new_transation);
			count--;
			for (; count > 0; count--) {
				unlock_page(pages[count]);
				__free_pages(pages[count], 0);
			}
			mutex_unlock(&sbi->snapshot_info->insert_lock);
			goto err_exit;
			}
		lock_page(pages[count]);
	}

	destination_base = vmap(pages, pages_count, VM_MAP, PAGE_KERNEL);
	if (!destination_base) {
		EMMCFS_ERR("can not vmap destination base (%d pages)",
				pages_count);
		EMMCFS_BUG();
	}

	destination = destination_base;
	source_base = vmap(original_pages, pages_count, VM_MAP, PAGE_KERNEL);
	if (!source_base) {
		EMMCFS_ERR("can not vmap source base (%d pages)", pages_count);
		EMMCFS_BUG();
	}
	source = source_base;
	/* copy pages contents */
	for (count = 0; count < pages_count; count++) {
		EMMCFS_BUG_ON(PageDirty(original_pages[count]));
		copy_page(destination, source);

		destination = (void *)((char *)destination + PAGE_SIZE);
		source += PAGE_SIZE;
	}
	vunmap(destination_base);
	vunmap(source_base);

	for (count = 0; count < pages_count; count++)
		set_page_writeback(pages[count]);

	if (sbi->snapshot_info->flags) {
		EMMCFS_DEBUG_SNAPSHOT("inconsistent lock state");
		EMMCFS_BUG();
	}

	spin_lock(lock_transaction_tree);
	if ((sbi->snapshot_info->page_index + pages_count) >
		sbi->snapshot_info->max_pages_in_snapshot) {
		EMMCFS_BUG();
	}
	new_transation->on_snapshot = (EMMCFS_SNAPSHOT_START(sbi) +
			sbi->snapshot_info->page_index) <<
			sbi->log_sectors_per_block;
	sbi->snapshot_info->page_index += pages_count;
	spin_unlock(lock_transaction_tree);

	mutex_unlock(&sbi->snapshot_info->insert_lock);

	emmcfs_write_snapshot_pages(sbi, pages,
			new_transation->on_snapshot, pages_count, 0);

	EMMCFS_DEBUG_SNAPSHOT("P_count %d From_sector %lu Write_to %lu",
			pages_count,
			(long unsigned int)new_transation->on_volume,
			(long unsigned int)new_transation->on_snapshot);

	EMMCFS_DEBUG_SNAPSHOT("Added transaction I: %lu",
			sbi->snapshot_info->page_index);
err_exit:
	kfree(pages);
exit:
	EMMCFS_LOG_FUNCTION_END(sbi, error);
	return error;
}

/**
 * @brief		Flash snapshot.
 * @param [in] 	sbi	The eMMCFS super block info.
 * @return		Returns 0 on success, errno on failure.
 */
static int flash_snapshot(struct emmcfs_sb_info *sbi)
{
	int ret = 0;
	int max_lenght = sbi->snapshot_info->maximum_transaction_lenght;
	/*int max_lenght = (EMMCFS_SNAPSHOT_LENGHT(sbi) >> 1);*/

	EMMCFS_LOG_FUNCTION_START(sbi);
	if (sbi->snapshot_info->page_index > max_lenght) {
		down_write(sbi->snapshot_info->transaction_lock);
		sbi->snapshot_info->flags = 1;
		if (sbi->snapshot_info->page_index <= max_lenght)
			goto exit; /* some one already did sync */

		ret = emmcfs_commit_snapshot(sbi);
		if (ret)
			goto exit;
		ret = emmcfs_sync_metadata(sbi);

		if (ret)
			goto exit;
		ret = emmcfs_clear_snapshot(sbi);

exit:
		sbi->snapshot_info->flags = 0;
		up_write(sbi->snapshot_info->transaction_lock);
	}
	EMMCFS_LOG_FUNCTION_END(sbi, ret);
	return ret;
}

/**
 * @brief		Get task log start.
 * @param [in] 	sbi	The eMMCFS super block info.
 * @return		TODO Returns 0 on success, errno on failure.
 */
static struct emmcfs_task_log *get_task_log_start(struct emmcfs_sb_info *sbi)
{
	struct emmcfs_task_log *current_task_log;
	int ret;

	spin_lock(&sbi->snapshot_info->task_log_lock);
	current_task_log =
		search_for_task_log(&sbi->snapshot_info->task_log_root);
	spin_unlock(&sbi->snapshot_info->task_log_lock);

	if (!current_task_log) {
again:
		current_task_log = kzalloc(sizeof(struct emmcfs_task_log)
				, GFP_KERNEL);
		if (!current_task_log)
			goto again;

		current_task_log->pid = current->pid;
		spin_lock(&sbi->snapshot_info->task_log_lock);
		ret = add_task_log(&sbi->snapshot_info->task_log_root,
				current_task_log);
		spin_unlock(&sbi->snapshot_info->task_log_lock);
		if (ret)
			EMMCFS_BUG();
	}

	return current_task_log;
}

/**
 * @brief		Get task log stop.
 * @param [in] 	sbi	The eMMCFS super block info.
 * @return		TODO Returns 0 on success, errno on failure.
 */
static struct emmcfs_task_log *get_task_log_stop(struct emmcfs_sb_info *sbi)
{
	struct emmcfs_task_log *current_task_log;

	spin_lock(&sbi->snapshot_info->task_log_lock);
	current_task_log =
		search_for_task_log(&sbi->snapshot_info->task_log_root);
	spin_unlock(&sbi->snapshot_info->task_log_lock);

	if (!current_task_log)
		EMMCFS_BUG();

	return current_task_log;
}

/**
 * @brief		Start transaction.
 * @param [in]	sbi	The eMMCFS super block info.
 * @return		Returns 0 on success, errno on failure.
 */
static int emmcfs_start_transaction(struct emmcfs_sb_info *sbi)
{
	int ret = 0;
	struct emmcfs_task_log *current_task_log;

	current_task_log = get_task_log_start(sbi);

	if (current_task_log->r_lock_count == 0) {
		ret = flash_snapshot(sbi);
		if (ret)
			return ret;
	}

	if (current_task_log->r_lock_count == 0) {
		EMMCFS_DEBUG_MUTEX("transaction_lock down_read");
		down_read(sbi->snapshot_info->transaction_lock);
		EMMCFS_DEBUG_MUTEX("transaction_lock down_read success");
	}
	current_task_log->r_lock_count++;
	return ret;
}

/**
 * @brief		Stop transaction.
 * @param [in] 	sbi	The eMMCFS super block info.
 * @return		Returns 0 on success, errno on failure.
 */
static int emmcfs_stop_transaction(struct emmcfs_sb_info *sbi)
{
	int ret = 0;
	struct emmcfs_task_log *current_task_log;

	current_task_log = get_task_log_stop(sbi);
	EMMCFS_BUG_ON(!current_task_log->r_lock_count);

	if (current_task_log->r_lock_count == 1) {
		EMMCFS_DEBUG_MUTEX("transaction_lock up_read");
		up_read(sbi->snapshot_info->transaction_lock);
	}
	current_task_log->r_lock_count--;

	if (current_task_log->r_lock_count == 0)
		ret = flash_snapshot(sbi);
	return ret;
}

/* dummy snapshot functions, it used if "journal" option is not set */
/* TODO add brief description for these functions*/
static inline int clear_snapshoting_no_snapshot(struct emmcfs_sb_info *sbi)
	{while (0); return 0; }
static inline int add_to_snapshot_no_snapshot(struct emmcfs_sb_info *sbi,
		struct inode *inode, struct page **original_pages,
		unsigned int offset)
	{while (0); return 0; }
static inline int commit_snapshot_no_snapshot(struct emmcfs_sb_info *sbi)
	{while (0); return 0; }
static inline int restore_snapshot_no_snapshot(struct emmcfs_sb_info *sbi)
	{while (0); return 0; }
static inline int start_transaction_no_snapshot(struct emmcfs_sb_info *sbi)
	{while (0); return 0;}
static inline int stop_transaction_no_snapshot(struct emmcfs_sb_info *sbi)
	{while (0); return 0;}


/**
 * TODO Empty snapshot operations
 */
static const struct snapshot_operations no_snapshot = {
	.clear_snapshot		= clear_snapshoting_no_snapshot,
	.add_to_snapshot	= add_to_snapshot_no_snapshot,
	.commit_snapshot	= commit_snapshot_no_snapshot,
	.restore_snapshot	= restore_snapshot_no_snapshot,
	.start_transaction	= start_transaction_no_snapshot,
	.stop_transaction	= stop_transaction_no_snapshot
};

/**
 * TODO
 */
static const struct snapshot_operations snapshoting = {
	.clear_snapshot		= emmcfs_clear_snapshot,
	.add_to_snapshot	= emmcfs_add_to_snapshot,
	.commit_snapshot	= emmcfs_commit_snapshot,
	.restore_snapshot	= emmcfs_restore_snapshot,
	.start_transaction	= emmcfs_start_transaction,
	.stop_transaction	= emmcfs_stop_transaction
};

/**
 * @brief		Build snapshot manager.
 * @param [in] 	sbi	The eMMCFS super block info.
 * @return		Returns 0 on success, errno on failure.
 */
int emmcfs_build_snapshot_manager(struct emmcfs_sb_info *sbi)
{
	struct emmcfs_snapshot_info *snapshot;
	int error = 0;
	EMMCFS_DEBUG_SNAPSHOT("build snapshot manager");

	EMMCFS_LOG_FUNCTION_START(sbi);

	if (EMMCFS_IS_READONLY(sbi->sb)) {
		error = emmcfs_is_snapshot_present(sbi);
		if (error) {
			EMMCFS_ERR("Snapshot is present, volume cann't be mounted"
					"in read-only mode");
			error = -EINVAL;
			goto err_exit;
		}
	}


	snapshot = kzalloc(sizeof(struct emmcfs_snapshot_info), GFP_KERNEL);

	if (!snapshot) {
		error = -ENOMEM;
		goto err_exit;
	}

	snapshot->transaction_lock = kmalloc(sizeof(struct rw_semaphore)
			, GFP_KERNEL);

	if (!snapshot->transaction_lock) {
		kfree(snapshot);
		error = -ENOMEM;
		goto err_exit;
	}
	init_rwsem(snapshot->transaction_lock);

	init_rwsem(&snapshot->head_write_sem);

	/* Pages count in snapshot area */
	snapshot->pages_per_head = EMMCFS_SNAPSHOT_LENGHT(sbi) >>
		(PAGE_CACHE_SHIFT - sbi->block_size_shift);
	/* How many bytes we need for structures to handle all
	 * that pages */
	snapshot->pages_per_head *= sizeof(struct emmcfs_transaction);
	/* Count pages num */
		snapshot->pages_per_head +=
				sizeof(struct emmcfs_snapshot_descriptor);
	snapshot->pages_per_head += (1 << PAGE_CACHE_SHIFT) - 1;
	snapshot->pages_per_head >>= PAGE_CACHE_SHIFT;

	snapshot->max_pages_in_snapshot = ((EMMCFS_SNAPSHOT_LENGHT(sbi)
			<< sbi->block_size_shift) >> PAGE_CACHE_SHIFT) -
			snapshot->pages_per_head - PAGES_PER_HEAD_HISTORY;

	/* 2 heights of each btree (worth case - full path splited)
	 * 2 pages for hlinks bitmap
	 * 2 pages for free inode bitmap */
	snapshot->maximum_transaction_lenght =
		snapshot->max_pages_in_snapshot -
		snapshot->pages_per_head -
		4 * sbi->max_cattree_height - 6;

	snapshot->head_size_bytes =
			snapshot->pages_per_head << PAGE_CACHE_SHIFT;
	snapshot->page_index = (pgoff_t)snapshot->pages_per_head;
	snapshot->snapshot_root = RB_ROOT;
	snapshot->task_log_root = RB_ROOT;
	spin_lock_init(&snapshot->transaction_rbtree_lock);
	spin_lock_init(&snapshot->task_log_lock);
	snapshot->flags = 0;
	if (test_option(sbi, SNAPSHOT) && !(EMMCFS_IS_READONLY(sbi->sb))) {
		/* snapshot is enable */
		snapshot->snapshot_op = &snapshoting;
	} else {
		/* no snapshot */
		snapshot->snapshot_op = &no_snapshot;
	}

	snapshot->head_version = 0;

	mutex_init(&snapshot->insert_lock);

	sbi->snapshot_info = snapshot;

	error = EMMCFS_RESTORE_FROM_SNAPSHOT(sbi);
err_exit:
	EMMCFS_LOG_FUNCTION_END(sbi, error);
	return error;
}

/**
 * @brief		Destroys snapshot manager.
 * @param [in] 	sbi	The eMMCFS super block info.
 * @return	void
 */
void emmcfs_destroy_snapshot_manager(struct emmcfs_sb_info *sbi)
{
	struct emmcfs_snapshot_info *snapshot_info = sbi->snapshot_info;
	EMMCFS_LOG_FUNCTION_START(sbi);
	sbi->snapshot_info = NULL;
	destroy_task_log(&snapshot_info->task_log_root);
	kfree(snapshot_info->transaction_lock);
	kfree(snapshot_info);
	EMMCFS_LOG_FUNCTION_END(sbi, 0);
}
