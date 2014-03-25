/**
 * @file	fs/emmcfs/fsm_btree.h
 * @brief	The eMMCFS 2-3-4 B-tree functionality implementation.
 * @author	Ivan Arishchenko, i.arishchenk@samsung.com
 * @date	03/16/2012
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
#ifndef _FSM_B_TREE_H_
#define _FSM_B_TREE_H_

#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/preempt.h>
#include <linux/types.h>

#define EMMCFS_FSM_BTREE_ORDER	32

#define FIND_CLEAR_BIT		0
#define FIND_SET_BIT		1


/**
 * @brief 	The FSM B-tree key.
 */
struct fsm_btree_key {
	/** Offset in blocks */
	__u64	block_offset;
	/** Length */
	__u64	length;
};

/**
 * @brief 	The eMMCFS B-tree.
 */
struct emmcfs_fsm_btree {
	/* Order */
	unsigned int order;
	/* Root pointer */
	struct emmcfs_fsm_btree_node *root;
	/* Function to compare keys */
	int (*cmp_key)(void*, void*);
	/* Function to copy keys */
	void (*cpy_key)(void*, void*);
	/* Function to print keys */
	void (*print_key)(void*);
};

/**
 * @brief	Function is used to search a node in B-tree and
 *		(if search succeed) remove node from B-tree.
 */
__u64 fsm_btree_lookup(struct emmcfs_fsm_btree *btree, __u64 off, __u32 len);

/**
 * @brief	Function is used to put a node to B-tree.
 */
int fsm_btree_free(struct emmcfs_fsm_btree *btree, __u64 off, __u64 len);

/**
 * @brief	Function used to insert node into a B-tree.
 */
int fsm_btree_insert(struct  emmcfs_fsm_btree *btree, void *key_value);

/**
 * @brief 	Function is used to remove a node from a  B-tree.
 */
int fsm_btree_delete(struct  emmcfs_fsm_btree *btree, void *key_value);

/**
 * @brief	Function is used to destroy B-tree.
 */
void fsm_btree_destroy(struct emmcfs_fsm_btree *btree);

/**
 * @brief		Function is used to print the B-tree.
 */
void fsm_print_tree(struct emmcfs_fsm_btree *btree);

#endif /* _FSM_B_TREE_H_ */
