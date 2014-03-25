/**
 * @file	fs/emmcfs/cattree.c
 * @brief	Operations with catalog tree.
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
#include <linux/ctype.h>
/*#include <linux/buffer_head.h>*/
/*#include <linux/vmalloc.h>*/

#include "emmcfs.h"
#include "debug.h"

/**
 * @brief 		Key compare function for catalog tree
 *			for case-sensitive usecase.
 * @param [in] 	__key1	Pointer to the first key
 * @param [in] 	__key2	Pointer to the second key
 * @return 		Returns value 	< 0	if key1 < key2,
		 			== 0	if key1 = key2,
					> 0 	if key1 > key2 (like strcmp)
 */
int emmcfs_cattree_cmpfn(struct emmcfs_generic_key *__key1,
		struct emmcfs_generic_key *__key2)
{
	struct emmcfs_cattree_key *key1, *key2;
	__u8 *ch1, *ch2;
	__s8 diff;
	int i;


	key1 = container_of(__key1, struct emmcfs_cattree_key, gen_key);
	key2 = container_of(__key2, struct emmcfs_cattree_key, gen_key);

	if (key1->parent_id != key2->parent_id)
		return (__s64) le64_to_cpu(key1->parent_id) -
			(__s64) le64_to_cpu(key2->parent_id);

	ch1 = key1->name.unicode_str;
	ch2 = key2->name.unicode_str;
	for (i = min(le32_to_cpu(key1->name.length),
				le32_to_cpu(key2->name.length)); i > 0; i--) {
		diff = le16_to_cpu(*ch1) - le16_to_cpu(*ch2);
		if (diff)
			return diff;
		ch1++;
		ch2++;
	}

	return le32_to_cpu(key1->name.length) -
		le32_to_cpu(key2->name.length);
}

/**
 * @brief 		Key compare function for catalog tree
 *			for case-insensitive usecase.
 * @param [in] 	__key1	Pointer to the first key
 * @param [in] 	__key2	Pointer to the second key
 * @return 		Returns value	< 0 	if key1 < key2,
					== 0 	if key1 = key2,
					> 0 	if key1 > key2 (like strcmp)
 */
int emmcfs_cattree_cmpfn_ci(struct emmcfs_generic_key *__key1,
		struct emmcfs_generic_key *__key2)
{
	struct emmcfs_cattree_key *key1, *key2;
	__u8 *ch1, *ch2;
	__s8 diff;
	int i;


	key1 = container_of(__key1, struct emmcfs_cattree_key, gen_key);
	key2 = container_of(__key2, struct emmcfs_cattree_key, gen_key);

	if (key1->parent_id != key2->parent_id)
		return (__s64) le64_to_cpu(key1->parent_id) -
			(__s64) le64_to_cpu(key2->parent_id);

	ch1 = key1->name.unicode_str;
	ch2 = key2->name.unicode_str;
	for (i = min(le32_to_cpu(key1->name.length),
				le32_to_cpu(key2->name.length)); i > 0; i--) {
		diff = le16_to_cpu(tolower(*ch1)) - le16_to_cpu(tolower(*ch2));
		if (diff)
			return diff;
		ch1++;
		ch2++;
	}

	return le32_to_cpu(key1->name.length) -
		le32_to_cpu(key2->name.length);
}

/**
 * @brief 			Interface to search specific object(file)
 *				in the catalog tree.
 * @param [in] 	sbi		Superblock information
 * @param [in] 	parent_id	Parent id of file to search for
 * @param [in] 	name		Name of file to search with specified parent id
 * @param [in] 	len		Name length
 * @param [out]	fd		Searching cache info for quick result
 * 				fetching later
 * @param [in]	mode		Mode in which bnode should be got
 *				(used later in emmcfs_btree_find)
 * @return 			Returns 0 on success, error code otherwise
 */
int emmcfs_cattree_find(struct emmcfs_sb_info *sbi,
		__u64 parent_id, const char *name, int len,
		struct emmcfs_find_data *fd, enum emmcfs_get_bnode_mode mode)
{
	struct emmcfs_cattree_key *key, *found_key;
	int ret = 0;
	EMMCFS_LOG_FUNCTION_START(sbi);

	if (fd->bnode) {
		EMMCFS_ERR("bnode should be empty");
		if (!is_sbi_flag_set(sbi, IS_MOUNT_FINISHED))
			return -EINVAL;
		else
			EMMCFS_BUG();
	}


	key = kzalloc(sizeof(*key), GFP_KERNEL);
	if (!key) {
		EMMCFS_LOG_FUNCTION_END(sbi, -ENOMEM);
		return -ENOMEM;
	}

	key->parent_id = cpu_to_le64(parent_id);
	/*key->name.length = utf8s_to_utf16s(name, len,
				 key->name.unicode_str);*/
	/*key->name.length = emmcfs_unicode_strlen(name);*/
	/*key2 = kzalloc(sizeof(*key2), GFP_KERNEL);*/
	key->name.length = len;
	memcpy(key->name.unicode_str, name, len);


	fd->bnode = emmcfs_btree_find(sbi->catalog_tree, &key->gen_key,
			&fd->pos, mode);

	if (IS_ERR(fd->bnode)) {
		ret = PTR_ERR(fd->bnode);
		goto exit;
	}

	if (fd->pos == -1) {
		EMMCFS_ERR("parent_id=%llu name=%s", parent_id, name);
		if (!is_sbi_flag_set(sbi, IS_MOUNT_FINISHED))
			ret = -EFAULT;
		else
			EMMCFS_BUG();
	}

	found_key = emmcfs_get_btree_record(fd->bnode, fd->pos);
	if (IS_ERR(found_key)) {
		ret = PTR_ERR(found_key);
		goto exit;
	}

	if (sbi->catalog_tree->comp_fn(&key->gen_key, &found_key->gen_key)) {
		emmcfs_put_bnode(fd->bnode);
		ret = -ENOENT;
	}

exit:
	kfree(key);
	EMMCFS_LOG_FUNCTION_END(sbi, ret);
	return ret;
}

/**
 * @brief 			Allocate key for catalog tree record.
 * @param [in] 	name_len	Length of a name for an allocating object
 * @param [in] 	record_type	Type of record which it should be
 * @return 			Returns pointer to a newly allocated key
 * 				or error code otherwise
 */
struct emmcfs_cattree_key *emmcfs_alloc_cattree_key(int name_len,
		int record_type)
{
	u32 record_len = 0;
	struct emmcfs_cattree_key *form_key;
	u32 key_len = sizeof(struct emmcfs_cattree_key) -
			sizeof(struct emmcfs_unicode_string) +
			sizeof(__le32) +
			/* TODO - if the terminating #0 symbol necessary and
			 * is used then '+ 1' is necessary
			 */
			(name_len + 1) * sizeof(*form_key->name.unicode_str);

	if (record_type & EMMCFS_CATALOG_FILE_RECORD)
		record_len = sizeof(struct emmcfs_catalog_file_record);
	else if (record_type == EMMCFS_CATALOG_FOLDER_RECORD)
		record_len = sizeof(struct emmcfs_catalog_folder_record);
	else if (record_type == EMMCFS_CATALOG_HLINK_RECORD)
		record_len = sizeof(struct emmcfs_catalog_folder_record);

	form_key = kzalloc(key_len + record_len, GFP_KERNEL);
	if (!form_key)
		return ERR_PTR(-ENOMEM);

	form_key->gen_key.key_len = cpu_to_le32(key_len);
	form_key->gen_key.record_len = cpu_to_le32(key_len + record_len);

	return form_key;
}

/**
 * @brief 			Fill already allocated key with data.
 * @param [out] fill_key	Key to fill
 * @param [in] 	parent_id	Parent id of object
 * @param [in] 	name		Object name
 * @return	void
 */
void emmcfs_fill_cattree_key(struct emmcfs_cattree_key *fill_key, u64 parent_id,
		struct qstr *name)
{
	fill_key->name.length = name->len;
	memcpy(fill_key->name.unicode_str, name->name, name->len);
	fill_key->parent_id = cpu_to_le64(parent_id);
}

/**
 * @brief 		Converts VFS mode to eMMCFS record type.
 * @param [in] 	mode	Mode of VFS object
 * @return 		Returns record type corresponding to mode
 */
static inline int get_record_type_on_mode(umode_t mode)
{
	if (S_ISDIR(mode))
		return EMMCFS_CATALOG_FOLDER_RECORD;
	else if (S_ISREG(mode))
		return EMMCFS_CATALOG_FILE_RECORD;
	else if (S_ISLNK(mode))
		return EMMCFS_CATALOG_FILE_RECORD |
				EMMCFS_CATALOG_FILE_RECORD_SYMLINK;
	else if (S_ISFIFO(mode))
		return EMMCFS_CATALOG_FILE_RECORD |
			EMMCFS_CATALOG_FILE_RECORD_FIFO;
	else if (S_ISSOCK(mode))
		return EMMCFS_CATALOG_FILE_RECORD |
			EMMCFS_CATALOG_FILE_RECORD_SOCKET;
	else if (S_ISCHR(mode))
		return EMMCFS_CATALOG_FILE_RECORD |
			EMMCFS_CATALOG_FILE_RECORD_CHR;
	else if (S_ISBLK(mode))
		return EMMCFS_CATALOG_FILE_RECORD |
			EMMCFS_CATALOG_FILE_RECORD_BLK;

	return -EINVAL;
}

/**
 * @brief 			Fill already allocated value area (file or
 *				folder) with data from VFS inode.
 * @param [in] 	inode		The inode to fill value area with
 * @param [out] value_area	Pointer to already allocated memory area
 *				representing the corresponding eMMCFS object
 * @return			Returns 0 on success or type of record
 *				if it's < 0
 */
int emmcfs_fill_cattree_value(struct inode *inode, void *value_area)
{
	int rec_type = get_record_type_on_mode(inode->i_mode);
	struct emmcfs_catalog_folder_record *comm_rec = value_area;
	EMMCFS_LOG_FUNCTION_START(EMMCFS_SB(inode->i_sb));

	if (rec_type < 0)
		return rec_type;

	((struct emmcfs_catalog_folder_record *)comm_rec)->record_type =
			cpu_to_le16(rec_type);


	/* TODO - set magic */
/*	memcpy(comm_rec->magic, get_magic(le16_to_cpu(comm_rec->record_type)),
			sizeof(EMMCFS_CAT_FOLDER_MAGIC) - 1);*/
	comm_rec->permissions.file_mode = cpu_to_le16(inode->i_mode);
	comm_rec->permissions.uid = cpu_to_le32(inode->i_uid);
	comm_rec->permissions.gid = cpu_to_le32(inode->i_gid);
	comm_rec->total_items_count = cpu_to_le64(inode->i_size);
	comm_rec->links_count = cpu_to_le64(inode->i_nlink);
	comm_rec->object_id = cpu_to_le64(inode->i_ino);

	comm_rec->creation_time.seconds = cpu_to_le64(inode->i_ctime.tv_sec);
	comm_rec->access_time.seconds = cpu_to_le64(inode->i_atime.tv_sec);
	comm_rec->modification_time.seconds =
			cpu_to_le64(inode->i_mtime.tv_sec);

	comm_rec->creation_time.nanoseconds =
			cpu_to_le64(inode->i_ctime.tv_nsec);
	comm_rec->access_time.nanoseconds =
			cpu_to_le64(inode->i_atime.tv_nsec);
	comm_rec->modification_time.nanoseconds =
			cpu_to_le64(inode->i_mtime.tv_nsec);

	if (rec_type & EMMCFS_CATALOG_FILE_RECORD) {
		struct emmcfs_catalog_file_record *file_rec = value_area;
		emmcfs_form_fork(&file_rec->data_fork, inode);
	}
	EMMCFS_LOG_FUNCTION_END(EMMCFS_SB(inode->i_sb), 0);
	return 0;
}

/**
 * @brief 			Fill already allocated value area (hardlink).
 * @param [in] 	inode		The inode to fill value area with
 * @param [out]	hl_record	Pointer to already allocated memory area
 *				representing the corresponding eMMCFS
 *				hardlink object
 * @return	void
 */
static void emmcfs_fill_hlink_value(struct inode *inode,
		struct emmcfs_catalog_hlink_record *hl_record)
{
	hl_record->hlink_id = cpu_to_le64(EMMCFS_I(inode)->hlink_id);
	hl_record->record_type = cpu_to_le16(EMMCFS_CATALOG_HLINK_RECORD);
}

/**
 * @brief 			Add new object into catalog tree.
 * @param [in] 	inode		The inode representing an object to be added
 * @param [in] 	parent_id	Parent id of the object to be added
 * @param [in] 	name		Name of the object to be added
 * @return 			Returns 0 in case of success,
 *				error code otherwise
 */
int emmcfs_add_new_cattree_object(struct inode *inode, u64 parent_id,
		struct qstr *name)
{
	struct emmcfs_sb_info	*sbi = inode->i_sb->s_fs_info;
	void *record;
	int record_type;
	int ret = 0;

	EMMCFS_LOG_FUNCTION_START(sbi);
	EMMCFS_BUG_ON(inode->i_ino < EMMCFS_1ST_FILE_INO);

	if (EMMCFS_I(inode)->hlink_id == INVALID_HLINK_ID)
		record_type = get_record_type_on_mode(inode->i_mode);
	else
		record_type = EMMCFS_CATALOG_HLINK_RECORD;

	if(record_type < 0) {
		EMMCFS_LOG_FUNCTION_END(sbi, 0);
		return record_type;
	}

	record = emmcfs_alloc_cattree_key(name->len, record_type);
	if (IS_ERR(record)) {
		EMMCFS_LOG_FUNCTION_END(sbi, PTR_ERR(record));
		return PTR_ERR(record);
	}
	emmcfs_fill_cattree_key(record, parent_id, name);

	if (EMMCFS_I(inode)->hlink_id == INVALID_HLINK_ID)
		ret = emmcfs_fill_cattree_value(inode, get_value_pointer(record));
	else
		emmcfs_fill_hlink_value(inode, get_value_pointer(record));

	if (ret)
		goto exit;

	ret = emmcfs_btree_insert(sbi->catalog_tree, record);
	if (ret)
		goto exit;

	EMMCFS_I(inode)->is_new = 0;
exit:
	kfree(record);
	EMMCFS_LOG_FUNCTION_END(sbi, ret);
	return ret;
}

/**
 * @brief		Find first child object for the specified catalog.
 * @param [in]	inode	The inode representing a parent object
 * @param [out]	pos	Position in returned bnode
 * @return 		Returns pointer to bnode containing first child object
 *			in case of success, error code otherwise
 */
struct emmcfs_bnode *emmcfs_find_first_catalog_child(struct inode *inode,
		int *pos)
{
	struct emmcfs_cattree_key *search_key =
		kzalloc(sizeof(*search_key), GFP_KERNEL);
	struct emmcfs_bnode *bnode = NULL;
	struct emmcfs_cattree_key *found_key;
	struct emmcfs_sb_info *sbi = inode->i_sb->s_fs_info;

	EMMCFS_LOG_FUNCTION_START(sbi);
	if (!search_key) {
		struct emmcfs_bnode *ret;

		ret = ERR_PTR(-ENOMEM);
		EMMCFS_LOG_FUNCTION_END(sbi, -ENOMEM);
		return ret;
	}

	search_key->parent_id = cpu_to_le64(inode->i_ino);
	search_key->name.length = 0;
	bnode = emmcfs_btree_find(sbi->catalog_tree, &search_key->gen_key, pos,
			EMMCFS_BNODE_MODE_RO);

	if (IS_ERR(bnode)) {
		*pos = -1;
		goto exit;
	}

	EMMCFS_BUG_ON(*pos >= EMMCFS_BNODE_DSCR(bnode)->recs_count);
	found_key = emmcfs_get_btree_record(bnode, *pos);
	if (IS_ERR(found_key)) {
		EMMCFS_LOG_FUNCTION_END(sbi, PTR_ERR(found_key));
		return (struct emmcfs_bnode *)found_key;
	}

	if (found_key->parent_id == search_key->parent_id)
		goto exit;

	found_key = emmcfs_get_next_btree_record(&bnode, pos);
	if (IS_ERR(found_key)) {
		*pos = -1;
		emmcfs_put_bnode(bnode);
		/* If we are here, found_key contains err __FUNCTION__ */
		bnode = (void *) found_key;
		goto exit;
	}

	found_key = emmcfs_get_btree_record(bnode, *pos);
	if (found_key->parent_id != search_key->parent_id) {
		emmcfs_put_bnode(bnode);
		bnode = ERR_PTR(-ENOENT);
		*pos = -1;
	}

exit:
	kfree(search_key);
	EMMCFS_LOG_FUNCTION_END(sbi, 0);
	return bnode;
}

/**
 * @brief 		Get the object from catalog tree basing on hint.
 * @param [in] 	inode_i The eMMCFS inode runtime structure representing
 *			object to get
 * @param [out]	fd	Find data storing info about the place where object
 *			was found
 * @param [in] 	mode	Mode in which bnode is got
 * @return 		Returns 0 in case of success, error code otherwise
 */
int emmcfs_get_from_cattree(struct emmcfs_inode_info *inode_i,
		struct emmcfs_find_data *fd, enum emmcfs_get_bnode_mode mode)
{
	struct emmcfs_sb_info *sbi = EMMCFS_SB(inode_i->vfs_inode.i_sb);
	struct emmcfs_cattree_key *key, *found_key;
	int ret = 0;


	EMMCFS_BUG_ON(fd->bnode);

	/* Form search key */
	key = kzalloc(sizeof(*key), GFP_KERNEL);
	if (!key)
		return -ENOMEM;
	key->parent_id = cpu_to_le64(inode_i->parent_id);
	/*key->name.length = utf8s_to_utf16s(inode_i->name,
			 strlen(inode_i->name),	key->name.unicode_str);*/
	/*key->name.length = emmcfs_unicode_strlen(inode_i->name);*/

	/*key2 = kzalloc(sizeof(*key), GFP_KERNEL);*/
	key->name.length = strlen(inode_i->name);
	memcpy(key->name.unicode_str, inode_i->name, strlen(inode_i->name));

	/* Check correctoness of bnode_hint */
	if (inode_i->bnode_hint.bnode_id == 0 || inode_i->bnode_hint.pos < 0)
		goto find;

	fd->bnode = emmcfs_get_bnode(sbi->catalog_tree,
			inode_i->bnode_hint.bnode_id, mode);
	if (IS_ERR(fd->bnode))
		/* Something wrong whith bnode, possibly it was destroyed */
		goto find;

	if (EMMCFS_BNODE_DSCR(fd->bnode)->type != EMMCFS_NODE_LEAF) {
		emmcfs_put_bnode(fd->bnode);
		goto find;
	}

	if (inode_i->bnode_hint.pos >=
			EMMCFS_BNODE_DSCR(fd->bnode)->recs_count) {
		emmcfs_put_bnode(fd->bnode);
		goto find;
	}

	/* Get record described by bnode_hint */
	found_key = emmcfs_get_btree_record(fd->bnode,
			inode_i->bnode_hint.pos);
	if (IS_ERR(found_key)) {
		ret = PTR_ERR(found_key);
		goto exit;
	}

	if (!sbi->catalog_tree->comp_fn(&key->gen_key, &found_key->gen_key)) {
		/* Hinted record is exactly what we are searching */
		fd->pos = inode_i->bnode_hint.pos;
		fd->bnode_id = inode_i->bnode_hint.bnode_id;
		EMMCFS_DEBUG_INO("ino#%lu hint match bnode_id=%u pos=%d",
				inode_i->vfs_inode.i_ino,
				inode_i->bnode_hint.bnode_id,
				inode_i->bnode_hint.pos);
		goto exit;
	}

	/* Unfortunately record had been moved since last time hint
	 * was updated */
	emmcfs_put_bnode(fd->bnode);
find:
	EMMCFS_DEBUG_INO("ino#%lu moved from bnode_id=%lu pos=%d",
			inode_i->vfs_inode.i_ino,
			(long unsigned int) inode_i->bnode_hint.bnode_id,
			inode_i->bnode_hint.pos);

	fd->bnode = emmcfs_btree_find(sbi->catalog_tree, &key->gen_key,
			&fd->pos, mode);

	if (fd->pos == -1) {
		ret = -ENOENT;
		goto exit;
	}

	found_key = emmcfs_get_btree_record(fd->bnode, fd->pos);
	if (IS_ERR(found_key)) {
		ret = PTR_ERR(found_key);
		goto exit;
	}
	if (sbi->catalog_tree->comp_fn(&key->gen_key, &found_key->gen_key)) {
		emmcfs_put_bnode(fd->bnode);
		ret = -ENOENT;
		/* Current hint is wrong */
		inode_i->bnode_hint.bnode_id = 0;
		inode_i->bnode_hint.pos = -1;
		goto exit;
	}

	/* Update hint with new record position */
	inode_i->bnode_hint.bnode_id = fd->bnode->node_id;
	inode_i->bnode_hint.pos = fd->pos;
exit:
	kfree(key);
	return ret;
}
