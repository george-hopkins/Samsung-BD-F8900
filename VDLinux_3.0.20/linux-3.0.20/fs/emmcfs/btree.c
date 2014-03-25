/**
 * @file	fs/emmcfs/btree.c
 * @brief	Basic B-tree opreations.
 * @date	03/05/2012
 *
 * eMMCFS -- Samsung eMMC chip oriented File System, Version 1.
 *
 * This file implements interface and functionality for eMMCFS B-tree
 *
 * Copyright 2011 by Samsung Electronics, Inc.,
 *
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

#ifdef USERSPACE_TESTING
#include "kernel_redef.h"
#else
#include "emmcfs.h"
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/pagevec.h>
#include <linux/vmalloc.h>
#endif

#include "btree.h"

#define BIN_SEARCH

/** Leaf level in any tree. */
#define EMMCFS_BTREE_LEAF_LVL 1

#define EMMCFS_BNODE_RESERVE(btree) (emmcfs_btree_get_height(btree)*4)

#if defined(CONFIG_EMMCFS_CRC_CHECK)
int emmcfs_sign_dirty_bnodes(struct address_space *mapping,
		struct emmcfs_btree *btree)
{
	struct pagevec pvec;
	void *bnode_data;
	pgoff_t index = 0;

	while(pagevec_lookup_tag(&pvec, mapping, &index, PAGECACHE_TAG_DIRTY,
			btree->pages_per_node))
	{
		bnode_data = vmap(pvec.pages, btree->pages_per_node,
				VM_MAP, PAGE_KERNEL);
		if (!bnode_data) {
			printk(KERN_ERR "can not allocate virtual memory");
			return -ENOMEM;
		}

		emmcfs_sign_bnode(bnode_data, btree);

		vunmap(bnode_data);
	}

	return 0;
}
#endif
/**
 * @brief		Increase B-tree height.
 * @param [in] 	btree	B-tree for which the height is increased
 * @return	void
 */
static void emmcfs_btree_inc_height(struct emmcfs_btree *btree)
{
	u16 btree_height = le16_to_cpu(EMMCFS_BTREE_HEAD(btree)->btree_height);

	EMMCFS_LOG_FUNCTION_START(btree->sbi);
	btree_height++;
	EMMCFS_BUG_ON(EMMCFS_ADD_CHUNK(btree->inode, btree->head_bnode->pages,
			btree->pages_per_node));
	EMMCFS_BTREE_HEAD(btree)->btree_height = cpu_to_le16(btree_height);
	emmcfs_mark_bnode_dirty(btree->head_bnode);
	EMMCFS_LOG_FUNCTION_END(btree->sbi, 0);
}

/**
 * @brief		Decrease B-tree height.
 * @param [in] 	btree	B-tree for which the height will be decreased
 * @return	void
 */
static void emmcfs_btree_dec_height(struct emmcfs_btree *btree)
{
	u16 btree_height = le16_to_cpu(EMMCFS_BTREE_HEAD(btree)->btree_height);
	EMMCFS_LOG_FUNCTION_START(btree->sbi);
	btree_height--;
	EMMCFS_BUG_ON(btree_height == 0);
	EMMCFS_BUG_ON(EMMCFS_ADD_CHUNK(btree->inode, btree->head_bnode->pages,
			btree->pages_per_node));
	EMMCFS_BTREE_HEAD(btree)->btree_height = cpu_to_le16(btree_height);
	emmcfs_mark_bnode_dirty(btree->head_bnode);

	EMMCFS_LOG_FUNCTION_END(btree->sbi, 0);
}

/**
 * @brief		Get B-tree height.
 * @param [in] 	btree	B-tree for which the height is requested
 * @return		Returns height of corresponding B-tree
 */
static u16 emmcfs_btree_get_height(struct emmcfs_btree *btree)
{
	u16 ret;
	EMMCFS_LOG_FUNCTION_START(btree->sbi);
	ret = le16_to_cpu(EMMCFS_BTREE_HEAD(btree)->btree_height);
	EMMCFS_LOG_FUNCTION_END(btree->sbi, ret);
	return ret;
}

/**
 * @brief		Get B-tree rood bnode id.
 * @param [in] 	btree	Tree for which the root bnode is requested
 * @return		Returns bnode id of the root bnode
 */
u32 emmcfs_btree_get_root_id(struct emmcfs_btree *btree)
{
	u32 ret;
	EMMCFS_LOG_FUNCTION_START(btree->sbi);
	ret = le32_to_cpu(EMMCFS_BTREE_HEAD(btree)->root_bnode_id);
	EMMCFS_LOG_FUNCTION_END(btree->sbi, ret);
	return ret;
}

/**
 * @brief 				Set B-tree rood bnode id.
 * @param [in] 	btree 			B-tree for which the root bnode is set
 * @param [in] 	new_root_bnode_id 	New id value for root bnode
 * @return	void
 */
static void emmcfs_btree_set_root_id(struct emmcfs_btree *btree,
		u32 new_root_bnode_id)
{
	EMMCFS_LOG_FUNCTION_START(btree->sbi);
	EMMCFS_BUG_ON(EMMCFS_ADD_CHUNK(btree->inode, btree->head_bnode->pages,
			btree->pages_per_node));
	EMMCFS_BTREE_HEAD(btree)->root_bnode_id = cpu_to_le32(
			new_root_bnode_id);
	emmcfs_mark_bnode_dirty(btree->head_bnode);

	EMMCFS_LOG_FUNCTION_END(btree->sbi, 0);
}

/**
 * @brief 	Structure contains the path in which the search was done to
 * 		avoid recursion. Path is contained as array of these records.
 */
struct path_container {
	/** The bnode passed while searching */
	struct emmcfs_bnode *bnode;
	/** Index of record in the bnode */
	int index;
};


/**
 * @brief 		Get address of cell containing offset of record with
 * 			specified index.
 * @param [in] 	bnode 	The bnode containing necessary record
 * @param [in]	index 	Index of offset to receive
 * @return 		Returns address in memory of offset place
 */
static void *get_offset_addr(struct emmcfs_bnode *bnode, unsigned int index)
{
	void *ret;
	EMMCFS_LOG_FUNCTION_START(bnode->host->sbi);
	ret = bnode->data + bnode->host->node_size_bytes -
		EMMCFS_BNODE_FIRST_OFFSET -
		sizeof(emmcfs_bt_off_t) * (index + 1);

	if (((unsigned)(ret - bnode->data)) >= bnode->host->node_size_bytes) {
		if (!is_sbi_flag_set(bnode->host->sbi, IS_MOUNT_FINISHED))
			return ERR_PTR(-EFAULT);
		else
			EMMCFS_BUG();
	}
	
	EMMCFS_LOG_FUNCTION_END(bnode->host->sbi, 0);
	return ret;
}

/**
 * @brief 		Get offset for record with specified index
 *			inside bnode.
 * @param [in] 	bnode 	The bnode containing necessary record
 * @param [in] 	index 	Index of offset to get
 * @return 		Returns offset starting from bnode start address
 */
static emmcfs_bt_off_t get_offset(struct emmcfs_bnode *bnode,
		unsigned int index)
{
	emmcfs_bt_off_t *p_ret;
	
	p_ret = get_offset_addr(bnode, index);
	if (IS_ERR(p_ret))
		return EMMCFS_BT_INVALID_OFFSET;

	return *p_ret;
}

static inline int is_offset_correct(struct emmcfs_bnode *bnode,
		emmcfs_bt_off_t offset)
{
	if (offset == 0)
		return 0;
	if (offset >= bnode->host->node_size_bytes)
		return 0;

	return 1;
}

static int check_bnode_offset_area(struct emmcfs_bnode *bnode)
{
	int recs_count, i;
	emmcfs_bt_off_t offset;

	recs_count = EMMCFS_BNODE_DSCR(bnode)->recs_count;
	for (i = 0; i < recs_count; i++) {
		offset = get_offset(bnode, i);
		if (!is_offset_correct(bnode, offset))
			return -1;
	}

	return 0;
}

/**
 * @brief 		Get memory address of record with specified index.
 * @param [in] 	bnode 	The bnode containing necessary record
 * @param [in] 	index 	Index of offset to get
 * @return 		Returns memory pointer to record with corresponding
 * 			index
 */
static void *get_record(struct emmcfs_bnode *bnode, unsigned int index)
{
	void *ret;
	emmcfs_bt_off_t offset;

	if (!bnode || index > EMMCFS_BNODE_DSCR(bnode)->recs_count)
		return ERR_PTR(-EINVAL);

	offset = get_offset(bnode, index);
	if (!is_offset_correct(bnode, offset))
		return ERR_PTR(-EINVAL);

	ret = (void *)bnode->data + offset;
	return ret;
}

/**
 * @brief		Set offset for record with specified index
 * 			inside bnode.
 * @param [in] 	bnode 	The bnode containing necessary record
 * @param [in] 	index 	Index of offset to set
 * @param [in] 	new_val	New value of offset
 * @return		0 - success, error code - fail
 */
static int check_set_offset(struct emmcfs_bnode *bnode, unsigned int index,
		emmcfs_bt_off_t new_val)
{
	emmcfs_bt_off_t *offset = get_offset_addr(bnode, index);

	if (IS_ERR(offset) || !offset ) {
		if (!is_sbi_flag_set(bnode->host->sbi, IS_MOUNT_FINISHED))
			return -EFAULT;
		else
			EMMCFS_BUG();
	}

	if (!is_offset_correct(bnode, new_val)) {
		if (!is_sbi_flag_set(bnode->host->sbi, IS_MOUNT_FINISHED))
			return -EFAULT;
		else
			EMMCFS_BUG();
	}
	*offset = new_val;


	return 0;
}

/**
 * @brief 		Interface to function get_record.
 * @param [in] 	bnode	Descriptor of bnode
 * @param [in] 	index	Index of offset to get
 * @return 		Returns memory pointer to record with corresponding index
 */
void *emmcfs_get_btree_record(struct emmcfs_bnode *bnode, int index)
{
	void *ret;
	ret = get_record(bnode, index);
	return ret;
}

/**
 * @brief 		Get descriptor of next bnode.
 * @param [in] 	bnode	Current bnode
 * @return 		Returns next bnode
 */
struct emmcfs_bnode *emmcfs_get_next_bnode(struct emmcfs_bnode *bnode)
{
	struct emmcfs_bnode *ret;
	EMMCFS_LOG_FUNCTION_START(bnode->host->sbi);

	if (EMMCFS_BNODE_DSCR(bnode)->next_node_id == 0) {
		ret = ERR_PTR(-ENOENT);
		EMMCFS_LOG_FUNCTION_END(bnode->host->sbi, -ENOENT);
		return ret;
	}
	ret = emmcfs_get_bnode(bnode->host,
		le32_to_cpu(EMMCFS_BNODE_DSCR(bnode)->next_node_id),
		bnode->mode);
	EMMCFS_LOG_FUNCTION_END(bnode->host->sbi, 0);
	return ret;
}

/**
 * @brief 	   		Get next record transparently to user.
 * @param [in,out]	__bnode	Current bnode
 * @param [in,out] 	index	Index of offset to get
 * @return 	   		Returns address of the next record even
 * 				locating in the next bnode
 */
void *emmcfs_get_next_btree_record(struct emmcfs_bnode **__bnode, int *index)
{
	struct emmcfs_bnode *bnode = *__bnode;
	struct emmcfs_bnode *next_bnode;
	void *ret;
	EMMCFS_LOG_FUNCTION_START(bnode->host->sbi);

	EMMCFS_BUG_ON(!bnode);

	if (*index+1 < EMMCFS_BNODE_DSCR(bnode)->recs_count) {
		(*index)++;
		ret = get_record(bnode, *index);
		EMMCFS_LOG_FUNCTION_END(bnode->host->sbi, 0);
		return ret;
	}

	next_bnode = emmcfs_get_next_bnode(bnode);
	if (IS_ERR(next_bnode)) {
		ret = (void *) next_bnode;
		EMMCFS_LOG_FUNCTION_END(bnode->host->sbi, 0);
		return ret;
	}

	emmcfs_put_bnode(bnode);
	*__bnode = bnode = next_bnode;
	*index = 0;

	ret = get_record(bnode, *index);
	EMMCFS_LOG_FUNCTION_END(bnode->host->sbi, 0);
	return ret;
}

/**
 * @brief 		Get child bnode contained in specified index record.
 * @param [in] 	btree	B-tree which record belongs to
 * @param [in] 	record	Record containing info about child
 * @param [in] 	mode	If the requested for r/o or r/w
 * @return 		Returns child bnode descriptor
 */
static struct emmcfs_bnode *get_child_bnode(struct emmcfs_btree *btree,
		void *record, enum emmcfs_get_bnode_mode mode)
{
	struct emmcfs_generic_key *key;
	struct generic_index_value *value;
	struct emmcfs_bnode *ret;
	EMMCFS_LOG_FUNCTION_START(btree->sbi);

	if (IS_ERR(record)) {
		EMMCFS_LOG_FUNCTION_END(btree->sbi, PTR_ERR(record));
		return record;
	}
	key = (struct emmcfs_generic_key *)record;
	value = (void *)key + key->key_len;
	if (value->node_id == 0) {
		if (!is_sbi_flag_set(btree->sbi, IS_MOUNT_FINISHED))
			return ERR_PTR(-EFAULT);
		else
			EMMCFS_BUG();
	}
	ret = emmcfs_get_bnode(btree, value->node_id, mode);

	EMMCFS_LOG_FUNCTION_END(btree->sbi, 0);
	return ret;
}

/**
 * @brief 			Checks whether node has enough space to
 * 				add adding_record.
 * @param [in] 	node_descr		Node descriptor to add
 * @param [in] 	adding_record	Record to add
 * @return 			Returns 0 if the space is enough, 1 otherwise
 */
static int is_node_full(struct emmcfs_gen_node_descr *node_descr,
	 	void *adding_record)
{
	struct emmcfs_generic_key *gen_key = adding_record;
	return node_descr->free_space <
		gen_key->record_len + sizeof(emmcfs_bt_off_t);
}

/**
 * @brief 		Inits newly created bnode descriptor.
 * @param [in] 	bnode	Node descriptor to add
 * @param [in] 	type	TODO
 * @return 	void
 */
static void init_new_node_descr(struct emmcfs_bnode *bnode,
		enum emmcfs_node_type type)
{
	struct emmcfs_gen_node_descr *node_descr = EMMCFS_BNODE_DSCR(bnode);
	EMMCFS_LOG_FUNCTION_START(bnode->host->sbi);


	node_descr->free_space = cpu_to_le32(bnode->host->node_size_bytes -
		sizeof(struct emmcfs_gen_node_descr) -
		sizeof(emmcfs_bt_off_t) - EMMCFS_BNODE_FIRST_OFFSET);
	node_descr->recs_count = 0;
	node_descr->type = type;
	node_descr->node_id = cpu_to_le32(bnode->node_id);

	node_descr->next_node_id = EMMCFS_INVALID_NODE_ID;
	node_descr->prev_node_id = EMMCFS_INVALID_NODE_ID;

	BUG_ON(check_set_offset(bnode, 0,
				sizeof(struct emmcfs_gen_node_descr)));

	EMMCFS_LOG_FUNCTION_END(bnode->host->sbi, 0);
}


/**
 * @brief 			Inserts new data into bnode.
 * @param [in] 	bnode		The bnode to add
 * @param [in] 	new_record	Record to add
 * @param [in] 	insert_pos	Position to insert
 * @return	void
 */
static int insert_into_node(struct emmcfs_bnode *bnode,
		void *new_record, int insert_pos)
{
	struct emmcfs_gen_node_descr *node_descr_ptr = bnode->data;
	struct emmcfs_generic_key *new_key =
		(struct emmcfs_generic_key *) new_record;
	void *free_space = NULL;
	emmcfs_bt_off_t *offset;
	int moving_offsets_num;
	int err = 0;
	EMMCFS_LOG_FUNCTION_START(bnode->host->sbi);

	EMMCFS_BUG_ON(node_descr_ptr->free_space < new_key->record_len);

	offset = get_offset_addr(bnode, node_descr_ptr->recs_count);
	if (IS_ERR(offset))
		return PTR_ERR(offset);
	if (!is_offset_correct(bnode, *offset))
		return -EFAULT;

	free_space = (void *) node_descr_ptr + *offset;

	memcpy(free_space, new_record, new_key->record_len);

	/* Prepere space for new offset */
	moving_offsets_num = node_descr_ptr->recs_count - insert_pos;
	if (moving_offsets_num > 0)
		memmove(offset, (void *) (offset + 1),
				moving_offsets_num * sizeof(emmcfs_bt_off_t));
	/* Put new offset */
	err = check_set_offset(bnode, insert_pos,
			free_space - (void *) node_descr_ptr);
	if (err)
		return err;

	/* Check if node have room for another one offset (to free space) */
	EMMCFS_BUG_ON(node_descr_ptr->free_space < sizeof(emmcfs_bt_off_t));

	node_descr_ptr->recs_count++;
	node_descr_ptr->free_space -= new_key->record_len +
			sizeof(emmcfs_bt_off_t);

	err = check_set_offset(bnode, node_descr_ptr->recs_count, 
			free_space + new_key->record_len -
			(void *) node_descr_ptr);
	EMMCFS_LOG_FUNCTION_END(bnode->host->sbi, 0);

	return err;
}

/**
 * @brief 		When splitting set correct next-prev links
 * 			for newly created bnodes.
 * @param [in] 	source	The bnode that is splitted
 * @param [in] 	new1	New left bnode
 * @param [in] 	new2	New right bnode
 * @return	void
 */
static void set_next_prev_links(struct emmcfs_bnode *source,
		struct emmcfs_bnode *new1, struct emmcfs_bnode *new2)
{
	struct emmcfs_gen_node_descr *src_descr = EMMCFS_BNODE_DSCR(source);
	struct emmcfs_gen_node_descr *new1_descr = EMMCFS_BNODE_DSCR(new1);
	struct emmcfs_gen_node_descr *new2_descr = EMMCFS_BNODE_DSCR(new2);
	EMMCFS_LOG_FUNCTION_START(source->host->sbi);

	new1_descr->next_node_id = new2_descr->node_id;
	new2_descr->prev_node_id = new1_descr->node_id;
	new2_descr->next_node_id = src_descr->next_node_id;
	new1_descr->prev_node_id = src_descr->prev_node_id;


	if (src_descr->prev_node_id != EMMCFS_INVALID_NODE_ID) {
		struct emmcfs_bnode *prev_node =
				emmcfs_get_bnode(source->host,
						src_descr->prev_node_id,
						EMMCFS_BNODE_MODE_RW);
		EMMCFS_BUG_ON(IS_ERR(prev_node));
		EMMCFS_BNODE_DSCR(prev_node)->next_node_id =
							new1_descr->node_id;
		emmcfs_mark_bnode_dirty(prev_node);
		emmcfs_put_bnode(prev_node);
	}

	if (src_descr->next_node_id != EMMCFS_INVALID_NODE_ID) {
		struct emmcfs_bnode *next_node =
				emmcfs_get_bnode(source->host,
						src_descr->next_node_id,
						EMMCFS_BNODE_MODE_RW);
		EMMCFS_BUG_ON(IS_ERR(next_node));
		EMMCFS_BNODE_DSCR(next_node)->prev_node_id =
							new2_descr->node_id;
		emmcfs_mark_bnode_dirty(next_node);
		emmcfs_put_bnode(next_node);
	}

	EMMCFS_LOG_FUNCTION_END(source->host->sbi, 0);
}

/**
 * @brief 			Splits bnode data to two new bnodes.
 * @param [in] 	bnode		Source bnode that is splitted
 * @param [in] 	adding_index	Index where new data should be inserted
 * 				in source bnode
 * @param [in] 	adding_record	Data to insert
 * @param [in] 	part1		New left bnode
 * @param [in] 	part2		New right bnode
 * @return	void
 */
static void split_node(struct emmcfs_bnode *bnode,
		int adding_index, void *adding_record,
		struct emmcfs_bnode *part1, struct emmcfs_bnode *part2)
{
	struct emmcfs_gen_node_descr *node_descr = EMMCFS_BNODE_DSCR(bnode);
	unsigned int used_space, i;
	int part_to_insert = 0;
	int inserted = 0;
	EMMCFS_LOG_FUNCTION_START(bnode->host->sbi);

	init_new_node_descr(part1, node_descr->type);
	init_new_node_descr(part2, node_descr->type);

	set_next_prev_links(bnode, part1, part2);
	used_space = bnode->host->node_size_bytes -
			EMMCFS_BNODE_DSCR(part1)->free_space;
	EMMCFS_BUG_ON(node_descr->recs_count == 0);

	i = 0;
	while (i < node_descr->recs_count || !inserted) {
		struct emmcfs_bnode *ins_bnode;
		int new_rec_select = (i == adding_index && !inserted);

		struct emmcfs_generic_key *key = (struct emmcfs_generic_key *) (
				(new_rec_select) ?
				adding_record : get_record(bnode, i));

		BUG_ON(IS_ERR(key) || !key);

		/* possible wrong key isn't checked*/
		used_space += key->record_len + sizeof(emmcfs_bt_off_t);


		/* TODO: we need any rounding here - select the neares to
		 * the node middle split bound */
		ins_bnode = ((part_to_insert) ? part2 : part1);

		EMMCFS_BUG_ON(insert_into_node(ins_bnode, key,
				EMMCFS_BNODE_DSCR(ins_bnode)->recs_count));

		if (used_space > bnode->host->node_size_bytes / 2)
			part_to_insert = 1;

		if (!new_rec_select)
			++i;
		else
			inserted = 1;
	}
	EMMCFS_LOG_FUNCTION_END(bnode->host->sbi, 0);
}

/**
 * @brief 				Inits new root when splitting
 *					root bnode.
 * @param [in] 	btree			B-tree which root bnode should be
 *					updated
 * @param [in] 	new_root_node		New root bnode
 * @param [in] 	first_record		First record to be added to new root
 * @param [in] 	second_record		Second record to be added to new root
 * @param [in] 	changing_node_id  	Value to set for first record as
 * 					bnode value
 * @return	void
 */
static void update_root_descriptor(
		struct emmcfs_btree *btree,
		struct emmcfs_bnode *new_root_node,
		struct emmcfs_generic_key *first_record,
		struct emmcfs_generic_key *second_record,
		u32 changing_node_id)
{
	struct emmcfs_generic_key *key;
	struct generic_index_value *value;
	EMMCFS_LOG_FUNCTION_START(btree->sbi);

	init_new_node_descr(new_root_node, EMMCFS_NODE_INDEX);
	EMMCFS_BUG_ON(insert_into_node(new_root_node, first_record, 0));
	EMMCFS_BUG_ON(insert_into_node(new_root_node, second_record, 1));

	/* Possible wrong key isn't checked*/
	key = (struct emmcfs_generic_key *) get_record(new_root_node, 0);
	BUG_ON(!key || IS_ERR(key));

	value = (void *)key + key->key_len;
	value->node_id = changing_node_id;

	/* Update btree head */
	emmcfs_btree_inc_height(btree);
	emmcfs_btree_set_root_id(btree, new_root_node->node_id);

	emmcfs_mark_bnode_dirty(new_root_node);
	emmcfs_mark_bnode_dirty(btree->head_bnode);
	emmcfs_put_bnode(new_root_node);

	EMMCFS_LOG_FUNCTION_END(btree->sbi, 0);
}

/**
 * @brief 			Forms necessary info from first record
 * 				of node to update upper levels.
 * @param [in] 	new_node	New node from which new record is formed
 * @param [in] 	adding_record	Pointer memory are to form new record
 * @return	void
 */
static void form_new_adding_record(struct emmcfs_bnode *new_node,
		void *adding_record)
{
	struct emmcfs_generic_key *new_key = get_record(new_node, 0);
	struct emmcfs_generic_key *forming_key;
	struct generic_index_value *value;
	EMMCFS_LOG_FUNCTION_START(new_node->host->sbi);

	/* Possible wrong new_key isn't checked*/
	BUG_ON(!new_key || IS_ERR(new_key));
	forming_key = (struct emmcfs_generic_key *)adding_record;
	value = (struct generic_index_value *)(adding_record +
							new_key->key_len);

	EMMCFS_BUG_ON(new_key->record_len > new_node->host->max_record_len);
	memcpy(adding_record, new_key, new_key->key_len);
	forming_key->record_len = new_key->key_len +
			sizeof(struct generic_index_value);
	value->node_id = new_node->node_id;
	EMMCFS_LOG_FUNCTION_END(new_node->host->sbi, 0);
}


/**
 * @brief 			Removes data from bnode.
 * @param [in] 	bnode		The bnode from which data is removed
 * @param [in] 	del_index	Index of record to be removed
 * @return	void
 */
static int delete_from_node(struct emmcfs_bnode *bnode, int del_index)
{
	struct emmcfs_gen_node_descr *node = EMMCFS_BNODE_DSCR(bnode);
	struct emmcfs_generic_key *del_key =
		(struct emmcfs_generic_key *) get_record(bnode, del_index);
	int i;
	emmcfs_bt_off_t rec_len;
	emmcfs_bt_off_t free_space_offset;
	emmcfs_bt_off_t del_offset;
	int err = 0;

	/*possible wrong del_key isn't checked*/
	if (!del_key || IS_ERR(del_key)) {
		if (!is_sbi_flag_set(bnode->host->sbi, IS_MOUNT_FINISHED))
			return -EFAULT;
		else
			BUG();
	}

	rec_len = del_key->record_len;
	free_space_offset = get_offset(bnode, node->recs_count);
	del_offset = get_offset(bnode, del_index);

	if (!is_offset_correct(bnode, free_space_offset) ||
			!is_offset_correct(bnode, del_offset)) {
		if (!is_sbi_flag_set(bnode->host->sbi, IS_MOUNT_FINISHED))
			return -EFAULT;
		else
			BUG();
	}

	memmove((void *)del_key, (void *)del_key + rec_len,
			free_space_offset - del_offset - rec_len);

	/* TODO - while deleting we should do two separate things -
	 * decrease the offsets that are bigger than deleting offset and
	 * shift all record offsets more than deleting in offset table
	 * up.
	 * I've separated them in two cycles because previous version
	 * worked not so good. Think how they can be united in one cycle
	 * without loosing functionality and readability.
	 */
	for (i = 0; i < node->recs_count; i++) {
		emmcfs_bt_off_t cur_offs = get_offset(bnode, i);
		if (cur_offs > del_offset) {
			err = check_set_offset(bnode, i, cur_offs - rec_len);
			if (err)
				return err;
		}
	}

	for (i = del_index; i < node->recs_count; i++) {
		emmcfs_bt_off_t next_offset = get_offset(bnode, i + 1);
		err = check_set_offset(bnode, i, next_offset);
		if (err)
			return err;
	}

	node->recs_count--;
	node->free_space += rec_len + sizeof(emmcfs_bt_off_t);
	err = check_set_offset(bnode,
			node->recs_count, free_space_offset - rec_len);
	if (err)
		return err;

#if defined(CONFIG_EMMCFS_DEBUG)
{
	emmcfs_bt_off_t *offset;
	void *free_spc_pointer = NULL;

	offset = get_offset_addr(bnode, node->recs_count);

	if (IS_ERR(offset))
		return PTR_ERR(offset);
	if (!is_offset_correct(bnode, *offset))
		return -EFAULT;

	free_spc_pointer = (void *) node + *offset;
	EMMCFS_BUG_ON(node->free_space != (void *) offset - free_spc_pointer);

	memset(free_spc_pointer, 0xA5, node->free_space);
}
#endif

	emmcfs_mark_bnode_dirty(bnode);
	EMMCFS_LOG_FUNCTION_END(bnode->host->sbi, 0);

	return 0;
}

/**
 * @brief 		Finds data in the B-tree.
 * @param [in] 	tree	The B-tree to search in
 * @param [in] 	key	Key descriptor to search
 * @param [in] 	level	B-tree level (0 - root) to start the search
 * @param [out] pos	Saves position into bnode
 * @param [out] path	If specified saves the bnodes passed and index in them
 * @param [in] 	mode	Gets the bnode in specified mode
 * @return 		Returns bnode containing specified record or error
 * 			if occurred
 */
struct emmcfs_bnode *btree_find(struct emmcfs_btree *tree,
				struct emmcfs_generic_key *key,
				int level, int *pos,
				struct path_container *path,
				enum emmcfs_get_bnode_mode mode)
{
	struct emmcfs_bnode *bnode, *bnode_new;
	int i, height, prev_pos;
	void *record, *prev_record;
	u16 btree_height = emmcfs_btree_get_height(tree);
	int levels_num = btree_height - level;
	int err = 0;
	struct emmcfs_bnode *ret;
	EMMCFS_LOG_FUNCTION_START(tree->sbi);

	EMMCFS_BUG_ON(!tree->comp_fn);
	bnode = emmcfs_get_bnode(tree, emmcfs_btree_get_root_id(tree), mode);
	if (IS_ERR(bnode)) {
		EMMCFS_LOG_FUNCTION_END(tree->sbi, PTR_ERR(bnode));
		return bnode;
	}

	if (path)
		memset(path, 0, sizeof(*path) * levels_num);

	for (height = btree_height; height >= level; height--) {
		record = NULL;
		prev_record = NULL;
		*pos = -1;
		prev_pos = -1;

		if (EMMCFS_BNODE_DSCR(bnode)->recs_count) {
			size_t first = 0;
			size_t last = EMMCFS_BNODE_DSCR(bnode)->recs_count;
			size_t mid;
			int cmp_res;

			record = get_record(bnode, first);
			if(IS_ERR(record)) {
				err = PTR_ERR(record);
				goto err_exit;
			}
			cmp_res = tree->comp_fn(record, key);
			if  (cmp_res > 0) {
				prev_record = record;
				prev_pos = first;
				first = last;
			}

			record = get_record(bnode, last - 1);
			if(IS_ERR(record)) {
				err = PTR_ERR(record);
				goto err_exit;
			}
			cmp_res = tree->comp_fn(record, key);
			if  (cmp_res < 0) {
				prev_record = record;
				*pos = last - 1;
				first = last;
			}

			while (first < last) {
				mid = first + ((last - first) >> 1);
				record = get_record(bnode, mid);
				if(IS_ERR(record)) {
					err = PTR_ERR(record);
					goto err_exit;
				}
				*pos = mid;
				cmp_res = tree->comp_fn(record, key);

				/*Exactly what we are searching*/
				if (cmp_res == 0)
					break;
				else if (cmp_res > 0)
					last = mid;
				else {
					prev_record = record;
					prev_pos = mid;
					first = mid + 1;
				}
			}
			/* Exact key was not found, current key is
			 * greater then searching, so we interested in
			 * previous record */
			if (cmp_res > 0) {
				WARN_ON(!prev_record);
				record = prev_record;
				*pos = prev_pos;
			}

		} else {
			err = -EINVAL;
			goto err_exit;
		}

		if (path) {
			path[btree_height - height].bnode = bnode;
			path[btree_height - height].index = *pos;
		}

		if (level < height) {
			bnode_new = get_child_bnode(tree, record, mode);
			if (IS_ERR(bnode_new)) {
				err = PTR_ERR(bnode_new);
				/* Undo adding to path (prevent double putting
				 * on err_exit */
				if (path)
					path[btree_height - height].bnode = 0;
				goto err_exit;
			}

			/* Put bnode only if we not interested in path to the
			 * searching key*/
			if (!path)
				emmcfs_put_bnode(bnode);
			bnode = bnode_new;
		}
	}
	EMMCFS_LOG_FUNCTION_END(tree->sbi, 0);
	return bnode;

err_exit:
	emmcfs_put_bnode(bnode);
	if (path) {
		for (i = 0; i < levels_num; i++)
			if (path[i].bnode)
				emmcfs_put_bnode(path[i].bnode);
	}
	ret = ERR_PTR(err);
	EMMCFS_LOG_FUNCTION_END(tree->sbi, err);
	return ret;
}


/**
 * @brief 		Interface for finding data in the whole tree.
 * @param [in] 	tree	B-tree to search in
 * @param [in] 	key	Key descriptor to search
 * @param [out] pos	Saves position into bnode
 * @param [in] 	mode	Gets the bnode in specified mode
 * @return 		Returns bnode containing specified record
 *			or error if occurred
 */
struct emmcfs_bnode  *emmcfs_btree_find(struct emmcfs_btree *tree,
				struct emmcfs_generic_key *key,
				int *pos, enum emmcfs_get_bnode_mode mode)
{
	struct emmcfs_bnode  *ret;

	EMMCFS_LOG_FUNCTION_START(tree->sbi);
	ret = btree_find(tree, key, EMMCFS_BTREE_LEAF_LVL, pos, NULL, mode);
	EMMCFS_LOG_FUNCTION_END(tree->sbi, 0);

	return ret;
}

/**
 * @brief		checks if btree has anough bnodes for safe splitting,
 * 			and tries to allocate resever if it has not.
 * @param [in] btree	which btree to check
 * @return		0 if success or error code
 * */
static int check_bnode_reserve(struct emmcfs_btree *btree)
{
	pgoff_t available_bnodes, free_bnodes, used_bnodes;
	long int bnodes_deficit;
	int ret = 0;

	EMMCFS_LOG_FUNCTION_START(btree->sbi);
	
	available_bnodes = EMMCFS_I(btree->inode)->fork.total_block_count;
	available_bnodes >>= btree->log_pages_per_node;

	used_bnodes = btree->bitmap->bits_num - btree->bitmap->free_num;

	free_bnodes = available_bnodes - used_bnodes;

	bnodes_deficit = (long int) EMMCFS_BNODE_RESERVE(btree) - free_bnodes;

	if (bnodes_deficit <= 0)
		goto exit;

	ret = emmcfs_prealloc_bnode_reserve(btree, available_bnodes
			+ bnodes_deficit - 1);

exit:
	EMMCFS_LOG_FUNCTION_END(btree->sbi, 0);
	return ret;
}

/**
 * @brief 			Inserts record in specified tree at specified
 * 				level.
 * @param [in] 	btree		B-tree to insert to
 * @param [in] 	new_data	Pointer to record to be inserted
 * @param [in] 	level		Level to start inserting
 * @return 			Returns 0 if success, error code if fail
 */
static int btree_insert_at_level(struct emmcfs_btree *btree,
		void *new_data, int level)
{
	void *adding_record, *record, *first_ins_root_rec;
	int stop_inserting = 0;
	/* TODO - think if it's necessary and remove */
	u32 changing_node_id = UINT_MAX;
	int err = 0;
	int i, height_indx = emmcfs_btree_get_height(btree) - level;
	struct path_container *path = NULL;
	struct emmcfs_bnode *curr_bnode;
	EMMCFS_LOG_FUNCTION_START(btree->sbi);

	path = kzalloc(sizeof(struct path_container) *
			emmcfs_btree_get_height(btree), GFP_KERNEL);

	if (!path) {
		EMMCFS_LOG_FUNCTION_END(btree->sbi, -ENOMEM);
		return -ENOMEM;
	}


	/* Find where to put new record */
	curr_bnode = btree_find(btree, new_data, level, &i, path,
			EMMCFS_BNODE_MODE_RW);
	if (IS_ERR(curr_bnode)) {
		err = PTR_ERR(curr_bnode);
		goto exit_no_path_clean;
	}
	/* Check if adding record already exists */
	record = get_record(curr_bnode, i);
	if(IS_ERR(record)) {
		err = PTR_ERR(record);
		goto exit_no_ar_clean;
	}
	if (btree->comp_fn(record, new_data) == 0) {
		err = -EEXIST;
		goto exit_no_ar_clean;
	}

	/* To have common mechanism of deleting unnecessary adding records */
	adding_record = kmalloc(btree->max_record_len, GFP_KERNEL);
	if (!adding_record) {
		err = -ENOMEM;
		goto exit_no_ar_clean;
	}
	first_ins_root_rec = kmalloc(btree->max_record_len, GFP_KERNEL);
	if (!first_ins_root_rec) {
		err = -ENOMEM;
		goto exit_no_fr_clean;
	}

	EMMCFS_BUG_ON(((struct emmcfs_generic_key *)new_data)->record_len >
				btree->max_record_len);
	memcpy(adding_record, new_data,
			((struct emmcfs_generic_key *)new_data)->record_len);

	while (!stop_inserting) {
		struct emmcfs_gen_node_descr *node_descr;
		int index;

		curr_bnode = path[height_indx].bnode;
		node_descr = curr_bnode->data;
		index = path[height_indx].index;

		if (changing_node_id != UINT_MAX) {
			/* TODO - leaf node doesn't need to update self
			 * value, this is
			 */
			struct emmcfs_generic_key *key =
				(struct emmcfs_generic_key *)
				get_record(curr_bnode, index);

			struct generic_index_value *value =
				(void *)key + key->key_len;

			value->node_id = changing_node_id;
		}

		if (is_node_full(node_descr, adding_record)) {
			struct emmcfs_bnode *bnode1, *bnode2;
			int head_bnode_changed = 0;

			/* Before spliting leaf bnode, we should be shure, that
			 * we have enough free bnodes for spliting index levels.
			 * Btree will be broken if we can not insert index 
			 * for the new leaf bnode */
			if (height_indx == emmcfs_btree_get_height(btree) - 1) {
				err = check_bnode_reserve(btree);
				if (err)
					goto exit;
			}

			bnode1 = emmcfs_alloc_new_bnode(btree);
			if (IS_ERR(bnode1)) {
				err = -ENOSPC;
				goto exit;
			}
			bnode2 = emmcfs_alloc_new_bnode(btree);
			if (IS_ERR(bnode2)) {
				err = -ENOSPC;
				EMMCFS_BUG_ON(emmcfs_destroy_bnode(bnode1));
				goto exit;
			}

			split_node(curr_bnode,
					index + 1,
					adding_record,
					bnode1,
					bnode2);

			changing_node_id = bnode1->node_id;

			form_new_adding_record(bnode2, adding_record);
			if (curr_bnode->node_id ==
					emmcfs_btree_get_root_id(btree)) {
				form_new_adding_record(curr_bnode,
						first_ins_root_rec);
				update_root_descriptor(btree, curr_bnode,
					first_ins_root_rec, adding_record,
					changing_node_id);
				stop_inserting = 1;
				head_bnode_changed = 1;
			}

			emmcfs_mark_bnode_dirty(bnode1);
			emmcfs_mark_bnode_dirty(bnode2);
			emmcfs_put_bnode(bnode1);
			emmcfs_put_bnode(bnode2);
			if (!head_bnode_changed)
				EMMCFS_BUG_ON(emmcfs_destroy_bnode(curr_bnode));
		} else {
			err = insert_into_node(curr_bnode, adding_record,
					index + 1);
			if (err)
				goto exit;

			stop_inserting = 1;
			emmcfs_mark_bnode_dirty(curr_bnode);
			emmcfs_put_bnode(curr_bnode);
		}
		--height_indx;
	}


exit:
	kfree(first_ins_root_rec);

exit_no_fr_clean:
	kfree(adding_record);

exit_no_ar_clean:
	for (i = height_indx; i >= 0; --i)
		emmcfs_put_bnode(path[i].bnode);

exit_no_path_clean:
	kfree(path);

	EMMCFS_LOG_FUNCTION_END(btree->sbi, err);
	return err;
}

/**
 * @brief 			Interface for simple insertion into tree.
 * @param [in] 	btree		B-tree to insert to
 * @param [in] 	new_data	Pointer to record to be inserted
 * @return 			Returns 0 if success, error code if fail
 */
int emmcfs_btree_insert(struct emmcfs_btree *btree, void *new_data)
{
	int ret;

	EMMCFS_LOG_FUNCTION_START(btree->sbi);
	ret = btree_insert_at_level(btree, new_data, EMMCFS_BTREE_LEAF_LVL);
	EMMCFS_LOG_FUNCTION_END(btree->sbi, ret);

	return ret;
}

/**
 * @brief 			Changes key on upper level if first key is
 *				removed.
 * @param [in] 	tree		B-tree update
 * @param [in] 	key		Key to be removed from specified level
 * @param [in] 	point_to	The bnode from which to take new key
 * @param [in] 	level		Level where to change key
 * @return 			Returns pointer to new key or error if fail
 */
static struct emmcfs_generic_key *btree_update_key(
		struct emmcfs_btree *tree,
		struct emmcfs_generic_key *key,
		struct emmcfs_bnode *point_to, int level)
{
	struct emmcfs_bnode *bnode;
	void *new_data;
	struct emmcfs_generic_key *ret;
	int err = 0;
	int pos;
	EMMCFS_LOG_FUNCTION_START(tree->sbi);

	new_data = kmalloc(tree->max_record_len, GFP_KERNEL);
	if (!new_data) {
		EMMCFS_LOG_FUNCTION_END(tree->sbi, -ENOMEM);
		return ERR_PTR(-ENOMEM);
	}

	form_new_adding_record(point_to, new_data);

	err = btree_insert_at_level(tree, new_data, level);
	if (err)
		goto error_exit;

	bnode = btree_find(tree, key, level, &pos, NULL, EMMCFS_BNODE_MODE_RW);
	if (IS_ERR(bnode)) {
		err = -EINVAL;
		goto error_exit;
	}

	err = delete_from_node(bnode, pos);
	if (err) {
		emmcfs_put_bnode(bnode);
		goto error_exit;
	}

	if (pos == 0 && level < emmcfs_btree_get_height(tree)) {
		ret = btree_update_key(tree, key, bnode, level + 1);
		if (IS_ERR(ret))
			err = PTR_ERR(ret);
		else
			kfree(ret);
	}

	emmcfs_put_bnode(bnode);

	if (err)
		goto error_exit;

	EMMCFS_LOG_FUNCTION_END(tree->sbi, 0);
	return new_data;

error_exit:
	kfree(new_data);
	new_data = NULL;
	EMMCFS_LOG_FUNCTION_END(tree->sbi, err);
	return ERR_PTR(err);
}

/**
 * @brief 			Merges data from right bnode to left bnode.
 * @param [in] 	tree		B-tree to be updated
 * @param [in] 	parent_bnode_id	Parent bnode id for both left and right bnode
 * @param [in] 	left_bnode_id	Id of left (receiving) bnode
 * @param [in] 	right_bnode_id	Id of right (giving back) bnode
 * @param [in] 	level		Level where bnodes are merged
 * @return			Returns the error code
 */
static int btree_merge(struct emmcfs_btree *tree, int parent_bnode_id,
		int left_bnode_id, int right_bnode_id, int level)
{
	struct emmcfs_bnode *left_bnode = NULL, *right_bnode = NULL;
	struct emmcfs_gen_node_descr *left = NULL, *right = NULL;
	void *data_start;
	int i;
	emmcfs_bt_off_t start_offset;
	emmcfs_bt_off_t *l_free_space_addr;
	void *left_free_space;
	int err = 0;
	EMMCFS_LOG_FUNCTION_START(tree->sbi);


	left_bnode   = emmcfs_get_bnode(tree, left_bnode_id,
			EMMCFS_BNODE_MODE_RW);
	if (IS_ERR(left_bnode))
		return PTR_ERR(left_bnode);

	right_bnode  = emmcfs_get_bnode(tree, right_bnode_id,
			EMMCFS_BNODE_MODE_RW);
	if (IS_ERR(right_bnode)) {
		emmcfs_put_bnode(left_bnode);
		return PTR_ERR(right_bnode);
	}

	if (check_bnode_offset_area(left_bnode) ||
			check_bnode_offset_area(right_bnode)) {
		if (!is_sbi_flag_set(tree->sbi, IS_MOUNT_FINISHED)) {
			emmcfs_put_bnode(right_bnode);
			emmcfs_put_bnode(left_bnode);
			return -EFAULT;
		}
		else
			BUG();
	}

	left   = EMMCFS_BNODE_DSCR(left_bnode);
	right  = EMMCFS_BNODE_DSCR(right_bnode);

	data_start = (void *)right + sizeof(struct emmcfs_gen_node_descr);
	l_free_space_addr = get_offset_addr(left_bnode, left->recs_count);
	if (IS_ERR(l_free_space_addr)) {
		emmcfs_put_bnode(left_bnode);
		emmcfs_put_bnode(right_bnode);
		return (PTR_ERR(l_free_space_addr));
	}

	left_free_space = (void *)left + *l_free_space_addr;


	memmove(left_free_space, data_start,
				get_offset(right_bnode, right->recs_count) -
				sizeof(struct emmcfs_gen_node_descr));

	start_offset = get_offset(left_bnode, left->recs_count);
	for (i = 0; i <= right->recs_count; i++) {
		unsigned int record_index = i + left->recs_count;
		emmcfs_bt_off_t record_offset = start_offset +
			get_offset(right_bnode, i) -
			sizeof(struct emmcfs_gen_node_descr);

		BUG_ON(check_set_offset(left_bnode, record_index,
					record_offset));
	}

	left->free_space -= (get_offset(right_bnode, right->recs_count) -
					sizeof(struct emmcfs_gen_node_descr)) +
					right->recs_count *
					sizeof(emmcfs_bt_off_t);

	left->recs_count = left->recs_count + right->recs_count;
	left->next_node_id = right->next_node_id;

	if (right->next_node_id != EMMCFS_INVALID_NODE_ID) {
		struct emmcfs_bnode *next_bnode =
				emmcfs_get_bnode(tree, right->next_node_id,
						EMMCFS_BNODE_MODE_RW);
		if (IS_ERR(next_bnode)) {
			emmcfs_put_bnode(left_bnode);
			emmcfs_put_bnode(right_bnode);
			return PTR_ERR(right_bnode);
		}
		EMMCFS_BNODE_DSCR(next_bnode)->prev_node_id = left->node_id;
		left->next_node_id = next_bnode->node_id;

		emmcfs_mark_bnode_dirty(next_bnode);
		emmcfs_put_bnode(next_bnode);
	}

	emmcfs_mark_bnode_dirty(left_bnode);
	emmcfs_put_bnode(left_bnode);

	{
		/* possible bad offset */
		void *record = get_record(right_bnode, 0);
		struct emmcfs_generic_key *key =
		 kmalloc(((struct emmcfs_generic_key *)record)->key_len,
		 GFP_KERNEL);

		memcpy(key, record,
		 ((struct emmcfs_generic_key *)record)->key_len);

		key->record_len = key->key_len +
		 sizeof(struct generic_index_value);
		err = emmcfs_destroy_bnode(right_bnode);
		if (!err)
			err = btree_remove(tree, key, level+1);

		kfree(key);
		key = NULL;
	}
	EMMCFS_LOG_FUNCTION_END(tree->sbi, err);
	return err;
}

/**
 * @brief 				Determines if the tree needs to be
 * 					balanced and "victims" for this.
 * @param [in] 	tree			B-tree to be updated
 * @param [in] 	key			Key descriptor which was removed
 * @param [in] 	level			Level at which balancing should be done
 * @param [in] 	bnode_id		The bnode id from which key was removed
 * @param [in] 	bnode_free_space	Free space in bnode after deletion
 * @return	err val
 */
static int rebalance(struct emmcfs_btree *tree, struct emmcfs_generic_key *key,
		int level, int bnode_id, unsigned int bnode_free_space)
{
	unsigned int pos, neigh_free_space, parent_id, neigh_id;
	unsigned int left_id, right_id;
	struct emmcfs_bnode *neigh = NULL;
	struct emmcfs_bnode *parent = NULL;
	struct emmcfs_bnode *cur = NULL;
	int err = 0;

	EMMCFS_LOG_FUNCTION_START(tree->sbi);

	parent = btree_find(tree, key, level + 1, &pos, NULL,
			EMMCFS_BNODE_MODE_RW);
	if (IS_ERR(parent)) {
		EMMCFS_LOG_FUNCTION_END(tree->sbi, 0);
		return PTR_ERR(parent);
	}

	cur = emmcfs_get_bnode(tree, bnode_id, EMMCFS_BNODE_MODE_RW);
	if (IS_ERR(cur)) {
		err = PTR_ERR(cur);
		goto exit_put_parent;
	}

	parent_id = parent->node_id;


	if (pos > 0) {
		neigh_id = EMMCFS_BNODE_DSCR(cur)->prev_node_id;
		left_id = neigh_id;
		right_id = EMMCFS_BNODE_DSCR(cur)->node_id;
	} else if (pos < EMMCFS_BNODE_DSCR(parent)->recs_count-1) {
		neigh_id = EMMCFS_BNODE_DSCR(cur)->next_node_id;
		left_id = EMMCFS_BNODE_DSCR(cur)->node_id;
		right_id = neigh_id;
	} else {
		BUG();
	}

	neigh = emmcfs_get_bnode(tree, neigh_id, EMMCFS_BNODE_MODE_RW);
	if (IS_ERR(neigh)) {
		err = PTR_ERR(neigh);
		goto exit;
	}

	neigh_free_space = ((struct emmcfs_gen_node_descr *)
			neigh->data)->free_space;
	emmcfs_put_bnode(neigh);

	if (bnode_free_space + neigh_free_space >=
			tree->node_size_bytes) {
		emmcfs_put_bnode(parent);
		emmcfs_put_bnode(cur);
		return btree_merge(tree, parent_id, left_id, right_id, level);
	}

exit:
	emmcfs_put_bnode(cur);
exit_put_parent:
	emmcfs_put_bnode(parent);
	EMMCFS_LOG_FUNCTION_END(tree->sbi, 0);
	return err;
}

/**
 * @brief 		Removes the specified key at level.
 * @param [in] 	tree	B-tree to update
 * @param [in] 	key	Key descriptor to remvoe
 * @param [in] 	level	Level at which key should be searched
 * @return 		Returns 0 in case of success, error code otherwise
 */
int btree_remove(struct emmcfs_btree *tree, struct emmcfs_generic_key *key,
		int level)
{
	struct emmcfs_bnode *bnode;
	struct emmcfs_gen_node_descr *node;
	struct emmcfs_generic_key *new_key = NULL;

	void *record;
	int pos;
	int err = 0;
	EMMCFS_LOG_FUNCTION_START(tree->sbi);

	EMMCFS_BUG_ON(!tree->comp_fn);

	/*
	 * TODO: something strange is happening here
	 * this __FUNCTION__ replaced with BUG_ON
	if (level > tree->btree_height) {
		tree->btree_height = 0;
		tree->root_bnode_id = -1;
		return 0;
	}
	*/
	EMMCFS_BUG_ON(level > emmcfs_btree_get_height(tree));

	bnode = btree_find(tree, key, level, &pos, NULL, EMMCFS_BNODE_MODE_RW);
	if (IS_ERR(bnode)) {
		EMMCFS_LOG_FUNCTION_END(tree->sbi, PTR_ERR(bnode));
		return PTR_ERR(bnode);
	}

	node = bnode->data;
	if (!node) {
		err = -EINVAL;
		goto exit_put_bnode;
	}

	record = get_record(bnode, pos);
	if(IS_ERR(record)) {
		err = PTR_ERR(record);
		goto exit_put_bnode;
	}

	/* 1 is a leaf level here. Nobody knows why record checked only
	 * at leaf level */
	if ((level == 1) && (tree->comp_fn(record, key) != 0)) {
		err = -ENOENT;
		goto exit_put_bnode;
	}

	err = delete_from_node(bnode, pos);
	if (err)
		goto exit_put_bnode;

	if (node->recs_count == 0) {
		/* Bnode is empty, before removing it update prev and next
		 * pointers of neighborhoods */
		if (node->prev_node_id != EMMCFS_INVALID_NODE_ID) {
			struct emmcfs_bnode *prev_bnode =
				emmcfs_get_bnode(tree, node->prev_node_id,
						EMMCFS_BNODE_MODE_RW);

			/* TODO: error path */
			EMMCFS_BUG_ON(IS_ERR(prev_bnode));

			EMMCFS_BNODE_DSCR(prev_bnode)->next_node_id =
				node->next_node_id;

			emmcfs_mark_bnode_dirty(prev_bnode);
			emmcfs_put_bnode(prev_bnode);
		}

		if (node->next_node_id != EMMCFS_INVALID_NODE_ID) {
			struct emmcfs_bnode *next_bnode =
				emmcfs_get_bnode(tree, node->next_node_id,
						EMMCFS_BNODE_MODE_RW);

			/* TODO: error path */
			EMMCFS_BUG_ON(IS_ERR(next_bnode));

			EMMCFS_BNODE_DSCR(next_bnode)->prev_node_id =
				node->prev_node_id;

			emmcfs_mark_bnode_dirty(next_bnode);
			emmcfs_put_bnode(next_bnode);
		}

		err = emmcfs_destroy_bnode(bnode);
		if (err) {
			if (!is_sbi_flag_set(bnode->host->sbi,
						IS_MOUNT_FINISHED))
				return err;
			else
				EMMCFS_BUG();
		}
		EMMCFS_LOG_FUNCTION_END(tree->sbi, 0);
		return btree_remove(tree, key, level + 1);
	} else if ((pos == 0) && ((level < emmcfs_btree_get_height(tree)))) {
		/* Left position of bnode have been changed, so index record,
		 * pointing to current bnode, is outdated. Update it */
		new_key = btree_update_key(tree, key, bnode, level + 1);

		if (IS_ERR(new_key)) {
			err = PTR_ERR(new_key);
			goto exit_put_bnode;
		}

		key = new_key;
	}


	if (node->free_space > tree->node_size_bytes / 3) {
		if (level < emmcfs_btree_get_height(tree)) {
			/* It is NOT a root level */
			int node_id = bnode->node_id,
			    node_fs = ((struct emmcfs_gen_node_descr *)
				bnode->data)->free_space;
			emmcfs_put_bnode(bnode);
			err = rebalance(tree, key, level, node_id, node_fs);
			if (err && is_sbi_flag_set(tree->sbi,
						IS_MOUNT_FINISHED))
				BUG();
			goto exit;
		} else if (node->recs_count == 1) {
			/* It is a root bnode and now it contains the only
			 * pointer. If we are here we have to decrease btree
			 * height */
			struct emmcfs_bnode *bnode_n;
			if (emmcfs_btree_get_height(tree) <= 1)
				goto exit_put_bnode;

			/* The only child of current root, will be the
			 * new root*/
			bnode_n = get_child_bnode(tree, get_record(bnode, 0),
					EMMCFS_BNODE_MODE_RW);
			if (IS_ERR(bnode_n)) {
				err = PTR_ERR(bnode_n);
				goto exit_put_bnode;
			}

			emmcfs_btree_set_root_id(tree, bnode_n->node_id);
			emmcfs_btree_dec_height(tree);
			emmcfs_put_bnode(bnode_n);

			/* Old root is free now */
			err = emmcfs_destroy_bnode(bnode);
			goto exit;
		}
	}

exit_put_bnode:
	emmcfs_put_bnode(bnode);

exit:
	if (!IS_ERR(new_key))
		kfree(new_key);
	new_key = NULL;
	EMMCFS_LOG_FUNCTION_END(tree->sbi, 0);
	return err;
}

/**
 * @brief 		Interface for key removal.
 * @param [in] 	tree	B-tree to be update
 * @param [in] 	key	Key descriptor to be removed
 * @return 		Returns 0 in case of success, error code otherwise.
 */
int emmcfs_btree_remove(struct emmcfs_btree *tree,
			struct emmcfs_generic_key *key)
{
	int ret;
	EMMCFS_LOG_FUNCTION_START(tree->sbi);
	ret = btree_remove(tree, key, 1);
	EMMCFS_LOG_FUNCTION_END(tree->sbi, ret);

	return ret;
}

/** @brief 	Query of bnodes. Used by emmcfs_verify_btree.
 */
struct query {
	int start;
	int end;
	struct emmcfs_bnode **data;
};

/**
 * @brief	Initialization of bnodes query
 * @param	none
 * @return	Returns initialized query
 */
static struct query emmcfs_init_query(void)
{
	 struct query q;
	 q.start = 0;
	 q.end = 0;
	 q.data = kmalloc(sizeof(struct emmcfs_bnode *) *
			 EMMCFS_BNODE_CACHE_ITEMS, GFP_KERNEL);
	 return q;
}

/**
 * @brief			Push bnode to query.
 * @param [in,out] 	q	Query in which element will be pushed
 * @param [in] 		element	Element to be pushed
 * @return 			Returns 0 on success, error code on failure
 */
static int emmcfs_push_query(struct query *q, struct emmcfs_bnode *element)
{
	if ((q->end + 1) % EMMCFS_BNODE_CACHE_ITEMS == q->start)
		return -ENOSPC; /* query is overfull */
	q->data[q->end] = element;
	q->end = (q->end + 1) % EMMCFS_BNODE_CACHE_ITEMS;
	return 0;
}

/**
 * @brief 			Pop element from query.
 * @param [in,out] 	q	Query from which element will be deleted
 * @param [out] 	res	Where poped element will be placed
 * @return 		void
 */
static void emmcfs_pop_query(struct query *q, struct emmcfs_bnode **res)
{
	if (q->start == q->end) {
		*res = NULL;
		return;
	}

	(*res) = q->data[q->start];
	q->start = (q->start + 1) % EMMCFS_BNODE_CACHE_ITEMS;
}

/**
 * @brief 		B-tree verifying.
 * @param [in] 	btree	B-tree that will be verified
 * @return 		Returns 0 in case of success, error code otherwise
 */
int emmcfs_verify_btree(struct emmcfs_btree *btree)
{
	int i;
	int err = 0;
	struct emmcfs_bnode *bnode = NULL;
	struct query q = emmcfs_init_query();

	EMMCFS_LOG_FUNCTION_START(btree->sbi);

	emmcfs_push_query(&q, emmcfs_get_bnode(btree,
		emmcfs_btree_get_root_id(btree), EMMCFS_BNODE_MODE_RO));

	while (1) {
		struct emmcfs_bnode *temp_bnode;

		emmcfs_pop_query(&q, &bnode);
		if (bnode == NULL)
			break;

		if (IS_ERR(bnode)) {
			err = PTR_ERR(bnode);
			goto err_exit;
		}


		if(!EMMCFS_BNODE_DSCR(bnode)->next_node_id)
			break;

		temp_bnode = emmcfs_get_next_bnode(bnode);

		if (IS_ERR(temp_bnode)) {
			err = PTR_ERR(temp_bnode);
			goto err_exit;
		}

		if(EMMCFS_BNODE_DSCR(bnode)->next_node_id !=
					temp_bnode->node_id ||
			EMMCFS_BNODE_DSCR(temp_bnode)->prev_node_id !=
					bnode->node_id) {
			err = -EINVAL;
			goto err_exit;
		}


		if (EMMCFS_BNODE_DSCR(bnode)->type == EMMCFS_NODE_LEAF)
			continue;

		for (i = 0; i < EMMCFS_BNODE_DSCR(bnode)->recs_count; i++) {
			err = emmcfs_push_query(&q, get_child_bnode(btree,
				get_record(bnode, i), EMMCFS_BNODE_MODE_RO));
			if (err)
				break;
		}
	}

err_exit:
	kfree(q.data);
	EMMCFS_LOG_FUNCTION_END(btree->sbi, err);
	return err;
}
