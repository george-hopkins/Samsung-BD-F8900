/**
 * @file	fs/emmcfs/fsm_btree.c
 * @brief	The eMMCFS FSM B-tree functionality implementation.
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

#include "emmcfs.h"
#include "fsm_btree.h"
#include "debug.h"

/** The eMMCFS FSM B-tree cache.
 */
struct kmem_cache *emmcfs_fsm_node_cachep;

enum position_t {
	/** TODO add comment */
	LEFT = -1,
	/** TODO add comment */
	RIGHT = 1
};

enum fsm_btree_node_type {
	/** TODO add comment */
	FSM_INDEX_NODE,
	/** TODO add comment */
	FSM_LEAF_NODE
};

/** 
 * @brief The eMMCFS FSM B-tree node.
 */
struct emmcfs_fsm_btree_node {
	/** Pointer used for linked list */
	struct emmcfs_fsm_btree_node *next;

	/** Number of active keys */
	unsigned int nr_active;

	/** Level in the B-Tree */
	unsigned int level;

	/** Node type index or leaf */
	enum fsm_btree_node_type type;

	/** Array of keys and values */
	struct fsm_btree_key key_vals[2 * EMMCFS_FSM_BTREE_ORDER - 1];

	/** Array of pointers to child nodes */
	struct emmcfs_fsm_btree_node *children[2 * EMMCFS_FSM_BTREE_ORDER];
};

/**
 * @brief			FSM B-tree node initializer.
 * @param [in,out]	node	The FSM B-tree node for initialization.
 * @return		void
 */
static void btree_node_ctor(void *node)
{
	memset(node, 0, sizeof(struct emmcfs_fsm_btree_node));
}

/**
 * @brief	Initializes the eMMCFS FSM B-tree cache.
 * @return	Returns 0 on success, errno on failure.
 */
int emmcfs_fsm_cache_init(void)
{
	emmcfs_fsm_node_cachep = kmem_cache_create("emmcfs_fsm_btree",
		sizeof(struct emmcfs_fsm_btree_node), 0,
		SLAB_HWCACHE_ALIGN, btree_node_ctor);

	if (!emmcfs_fsm_node_cachep) {
		EMMCFS_ERR("failed to initialize inode cache\n");
		return -ENOMEM;
	}
	return 0;
}

/**
 * @brief		Destroys the eMMCFS FSM B-tree cache
 * @return	void
 */
void emmcfs_fsm_cache_destroy(void)
{
	kmem_cache_destroy(emmcfs_fsm_node_cachep);
}

/**
 * @brief		TODO Initializes the eMMCFS FSM B-tree cache.
 * @return	void
 */
static void fsm_btree_print_fn(void *key)
{
	EMMCFS_DEBUG_FSM("PRINT off %llu\tlen %llu ",
			 ((struct fsm_btree_key *)key)->block_offset,
			 ((struct fsm_btree_key *)key)->length);
}

/**
 * @brief		TODO Initializes the eMMCFS FSM B-tree cache.
 * @return	void
 */
static void fsm_btree_cpy_fn(void *dst, void *src)
{
	*((struct fsm_btree_key *)dst) = *((struct fsm_btree_key *)src);
}

/**
 * @brief		Function used to compare keys for the FSM B-tree nodes.
 * @param [in]	key1 	First FSM B-tree key.
 * @param [in]	key2 	Second FSM B-tree key.
 * @return		Returns	-1 if key1 < key2,
				1 if key1 > key2,
				0 if key1 == key2
 */
static int fsm_btree_cmp_fn(void *key1, void *key2)
{
	if (((struct fsm_btree_key *)key1)->block_offset <
		((struct fsm_btree_key *)key2)->block_offset)
		return -1;
	if (((struct fsm_btree_key *)key1)->block_offset >
		((struct fsm_btree_key *)key2)->block_offset)
		return 1;

	if (((struct fsm_btree_key *)key1)->length <
		((struct fsm_btree_key *)key2)->length)
		return -1;
	if (((struct fsm_btree_key *)key1)->length >
		((struct fsm_btree_key *)key2)->length)
		return 1;
	return 0;
}

/**
 * @brief		Function used to initialize FSM B-tree.
 * @param [in]	btree	The FSM B-tree structure pointer.
 * @return	void
 */
void emmcfs_init_fsm_btree(struct  emmcfs_fsm_btree *btree)
{
	btree->order = EMMCFS_FSM_BTREE_ORDER;
	btree->root = NULL;
	btree->cmp_key = fsm_btree_cmp_fn;
	btree->cpy_key = fsm_btree_cpy_fn;
	btree->print_key = fsm_btree_print_fn;
}

/**
 * @brief		Allocates memory for the FSM B-tree node.
 * @param [in]	btree 	The FSM B-tree.
 * @param [in]	order 	Order of the FSM B-tree.
 * @return		Returns allocated FSM B-tree node.
 */
static struct emmcfs_fsm_btree_node *allocate_btree_node(
		struct emmcfs_fsm_btree *tree)
{
	struct emmcfs_fsm_btree_node *node;

	/* Allocate memory for the node*/
	node = kmem_cache_alloc(emmcfs_fsm_node_cachep, __GFP_WAIT);

	if (!node) {
		EMMCFS_ERR("can't allocate memory for FSM b-tree node");
		return ERR_PTR(-ENOMEM);
	}

	node->nr_active = 0;		/* Active node count */
	node->type = FSM_LEAF_NODE;	/* New node is leaf node */
	node->level = 0;		/* Level in tree */
	node->next = NULL;		/* Initialize the linked list pointer*/

	return node;
}

/**
 * @brief		Frees memory allocated to the node.
 * @param [in]	tree	The FSM B-tree.
 * @param [in]	node	The node to be freed.
 * @return	void
 */
static void free_btree_node(struct emmcfs_fsm_btree *tree,
	struct emmcfs_fsm_btree_node *node)
{
	kmem_cache_free(emmcfs_fsm_node_cachep, node);
	node = NULL;
}

/**
 * @brief		Used to split the child node and adjust the parent
 *			so that it has two children.
 * @param [in]	parent 	The parent node.
 * @param [in]	index 	The index of the child node.
 * @param [in]	child  	Full child node.
 * @return		Returns 0 on success, ERRNO on fail.
 */
static int btree_split_child(struct emmcfs_fsm_btree *btree,
			struct emmcfs_fsm_btree_node *parent,
			unsigned int index,
			struct emmcfs_fsm_btree_node *child)
{
	unsigned int count = 0;
	unsigned int order = btree->order;

	struct emmcfs_fsm_btree_node *new_child = allocate_btree_node(btree);

	if (IS_ERR(new_child))
		return -ENOMEM;

	new_child->type = child->type;
	new_child->level = child->level;
	new_child->nr_active = btree->order - 1;

	/* Copy the higher order keys to the new child */
	for (count = 0; count < order - 1; count++) {
		new_child->key_vals[count] = child->key_vals[count + order];
		if (child->type != FSM_LEAF_NODE) {
			new_child->children[count] =
					child->children[count + order];
		}
	}

	/* Copy the last child pointer */
	if (child->type != FSM_LEAF_NODE)
		new_child->children[count] = child->children[count + order];

	child->nr_active = order - 1;

	for (count = parent->nr_active + 1; count > index + 1; count--)
		parent->children[count] = parent->children[count - 1];

	parent->children[index + 1] = new_child;

	for (count = parent->nr_active; count > index; count--)
		parent->key_vals[count] = parent->key_vals[count - 1];


	parent->key_vals[index] = child->key_vals[order - 1];
	parent->nr_active++;

	return 0;
}

/**
 * @brief			Used to insert a key in the non-full node.
 * @param [in]	btree		The eMMCFS FSM B-tree.
 * @param [in]	parent_node	The node to which the key will be added.
 * @param [in]	key_val		The key value pair.
 * @return			Returns 0 on success, ERRNO on fail.
 */
static int btree_insert_nonfull(struct emmcfs_fsm_btree *btree,
		struct emmcfs_fsm_btree_node *parent_node, void *key_val)
{

	int i, cmp_result;
	unsigned count2;
	int err = 0;
	struct emmcfs_fsm_btree_node *child;
	struct emmcfs_fsm_btree_node *node = parent_node;


insert:	i = node->nr_active - 1;

	/* leaf node */
	if (node->type == FSM_LEAF_NODE) {
		int imax = i, imin = 0;
		while (imax >= imin) {
			int imid = (imin + imax) / 2;
			cmp_result = btree->cmp_key(key_val,
						(void *)&node->key_vals[imid]);
			if (cmp_result > 0)
				imin = imid + 1;
			else if (cmp_result < 0)
				imax = imid - 1;
			else	/* key found at index imid */
				return -EEXIST;
		}
		imax++;
		for (imin = node->nr_active-1; imin >= imax; imin--)
			node->key_vals[imin+1] = node->key_vals[imin];
		btree->cpy_key(&node->key_vals[imax], key_val);
		node->nr_active++;
	} else { /* index node */
		for (; i >= 0; i--) {
			cmp_result = btree->cmp_key(key_val,
						    (void *)&node->key_vals[i]);

			/* Check for identical keys*/
			if (cmp_result == 0)
				return -EEXIST;

			if (cmp_result > 0)
				break;
		}

		i++;

		/* found children node */
		child = node->children[i];
		/* check : full node or no */
		if (child->nr_active == 2 * btree->order - 1) {
			/* Check for identical keys */
			for (count2 = 0; count2 < child->nr_active; count2++) {
				cmp_result = btree->cmp_key(key_val,
					(void *)&child->key_vals[count2]);
				/* Check for identical keys*/
				if (cmp_result == 0)
					return -EEXIST;

				/*all LEFT keys a smaller*/
				if (cmp_result < 0)
					break;
			}

			err = btree_split_child(btree, node, i, child);
			if (err)
				goto exit;
			if (btree->cmp_key(key_val,
					  (void *)&node->key_vals[i]) > 0)
				i++;
		}
		node = node->children[i];
		goto insert;
	}

exit:
	return err;
}


/**
 * @brief		Function used to insert node into a FSM B-tree.
 * @param [in]	root	Root of the FSM B-tree.
 * @param [in]	key_val	The node to be inserted.
 * @return		Returns 0 on success or -ERRNO on failure.
 */
int fsm_btree_insert(struct emmcfs_fsm_btree *btree, void *key_val)
{
	struct emmcfs_fsm_btree_node *rnode = NULL;
	int ret = -1;

	/* initialization check*/
	if (unlikely(btree->root == NULL))
		btree->root = allocate_btree_node(btree);

	if (unlikely(IS_ERR(btree->root)))
		return -ENOMEM;

	rnode = btree->root;

	if (rnode->nr_active == (2*btree->order - 1)) {
		struct emmcfs_fsm_btree_node *new_root;
		new_root = allocate_btree_node(btree);
		new_root->level = btree->root->level + 1;
		btree->root = new_root;
		new_root->type = FSM_INDEX_NODE;
		new_root->nr_active = 0;
		new_root->children[0]  = rnode;
		ret = btree_split_child(btree, new_root, 0, rnode);
		if (unlikely(ret))
			goto exit;
		ret = btree_insert_nonfull(btree, new_root, key_val);
	} else
		ret = btree_insert_nonfull(btree, rnode, key_val);

exit:
	return ret;
}

/**
 * @brief		Used to get the position of the MAX key within the
 *			subtree.
 * @param	btree	The FSM B-tree.
 * @param	nd	The subtree to be searched.
 * @return		Returns node containing the key and position of key.
 */
static struct emmcfs_fsm_btree_node *get_max_key_pos(
	struct emmcfs_fsm_btree *btree,
	struct emmcfs_fsm_btree_node *nd)
{

	struct emmcfs_fsm_btree_node *node = nd, *ret_node = NULL;

	while (true) {
		if (node == NULL)
			break;

		if (node->type == FSM_LEAF_NODE)
			return node;
		else {
			ret_node = node;
			node = node->children[node->nr_active];
		}
	}
	return ret_node;
}

/**
 * @brief		Used to get the position of the MIN key within
 *			the subtree.
 * @param [in]	btree 	The FSM B-tree.
 * @param [in]	nd 	Start point node for search.
 * @return		Returns node containing the key and position of key.
 */
static struct emmcfs_fsm_btree_node *get_min_key_pos(
	struct emmcfs_fsm_btree *btree,
	struct emmcfs_fsm_btree_node *nd)
{
	struct emmcfs_fsm_btree_node *node = nd, *ret_node = NULL;

	while (true) {
		if (node == NULL)
			break;

		if (node->type == FSM_LEAF_NODE)
			return node;
		else {
			ret_node = node;
			node = node->children[0];
		}
	}
	return ret_node;
}

/**
 * @brief		Merge nodes n1 and n2 (case 3b from Cormen).
 * @param [in]	btree 	The FSM B-tree.
 * @param [in]	parent 	The parent node.
 * @param [in]	index 	The index of the child.
 * @param [in]	pos 	LEFT or RIGHT
 * @return		Returns merged node.
 */
static struct emmcfs_fsm_btree_node *merge_siblings(
		struct emmcfs_fsm_btree *btree,
		struct emmcfs_fsm_btree_node *parent,
		unsigned int index , enum position_t pos)
{
	unsigned int j;
	struct emmcfs_fsm_btree_node *new_node;
	struct emmcfs_fsm_btree_node *n1, *n2;

	if (index == (parent->nr_active)) {
		index--;
		n1 = parent->children[parent->nr_active - 1];
		n2 = parent->children[parent->nr_active];
	} else {
		n1 = parent->children[index];
		n2 = parent->children[index + 1];
	}

	/* Merge the current node with the LEFT node */

	new_node = allocate_btree_node(btree);

	if (IS_ERR(new_node))
		return new_node;

	new_node->level = n1->level;
	new_node->type = n1->type;

	for (j = 0; j < btree->order - 1; j++) {
		new_node->key_vals[j] =	n1->key_vals[j];
		new_node->children[j] =	n1->children[j];
	}

	new_node->key_vals[btree->order - 1] =	parent->key_vals[index];
	new_node->children[btree->order - 1] =	n1->children[btree->order - 1];

	for (j = 0; j < btree->order - 1; j++) {
		new_node->key_vals[j + btree->order] =	n2->key_vals[j];
		new_node->children[j + btree->order] =	n2->children[j];
	}
	new_node->children[2*btree->order - 1] = n2->children[btree->order - 1];

	parent->children[index] = new_node;

	for (j = index; j < parent->nr_active; j++) {
		parent->key_vals[j] = parent->key_vals[j + 1];
		parent->children[j + 1] = parent->children[j + 2];
	}

	new_node->nr_active = n1->nr_active + n2->nr_active + 1;
	parent->nr_active--;

	free_btree_node(btree, n1);
	free_btree_node(btree, n2);

	if (parent->nr_active == 0 && btree->root == parent) {
		free_btree_node(btree, parent);
		btree->root = new_node;
		if (new_node->level)
			new_node->type = FSM_INDEX_NODE;
		else
			new_node->type = FSM_LEAF_NODE;
	}

	return new_node;
}

/**
 * @brief		Move the key from node to another.
 * @param [in]	btree	The FSM B-tree
 * @param [in]	node	The parent node.
 * @param [in]	index	The index of the key to be moved done.
 * @param [in]	pos	The position of the child to receive the key.
 * @return	void
 */
static void move_key(struct emmcfs_fsm_btree *btree,
		struct emmcfs_fsm_btree_node *node,
		unsigned int index, enum position_t pos)
{
	struct emmcfs_fsm_btree_node *lchild;
	struct emmcfs_fsm_btree_node *rchild;
	unsigned int i;

	if (pos == RIGHT)
		index--;

	lchild = node->children[index];
	rchild = node->children[index + 1];

	/* Move the key from the parent to the LEFT child */
	if (pos == LEFT) {
		lchild->key_vals[lchild->nr_active] = node->key_vals[index];
		lchild->children[lchild->nr_active + 1] = rchild->children[0];
		rchild->children[0] = NULL;
		lchild->nr_active++;

		node->key_vals[index] = rchild->key_vals[0];

		for (i = 0; i < rchild->nr_active - 1; i++) {
			rchild->key_vals[i] = rchild->key_vals[i + 1];
			rchild->children[i] = rchild->children[i + 1];
		}
		rchild->children[rchild->nr_active - 1] =
				rchild->children[rchild->nr_active];
		rchild->nr_active--;
	} else {
		/* Move the key from the parent to the RIGHT child */
		for (i = rchild->nr_active; i > 0 ; i--) {
			rchild->key_vals[i] = rchild->key_vals[i - 1];
			rchild->children[i + 1] = rchild->children[i];
		}
		rchild->children[1] = rchild->children[0];
		rchild->children[0] = NULL;

		rchild->key_vals[0] = node->key_vals[index];

		rchild->children[0] = lchild->children[lchild->nr_active];
		lchild->children[lchild->nr_active] = NULL;

		node->key_vals[index] = lchild->key_vals[lchild->nr_active - 1];

		lchild->nr_active--;
		rchild->nr_active++;
	}
}

/**
 * @brief		Merge nodes n1 and n2.
 * @param [in]	btree	The FSM B-tree.
 * @param [in]	n1	First node.
 * @param [in]	kv	The FSM B-tree key of the parent node.
 * @param [in]	n2	Second node.
 * @return		Returns combined node.
 */
static struct emmcfs_fsm_btree_node *merge_nodes(
		struct emmcfs_fsm_btree *btree,
		struct emmcfs_fsm_btree_node *n1,
		struct fsm_btree_key kv,
		struct emmcfs_fsm_btree_node *n2)
{
	struct emmcfs_fsm_btree_node *new_node;
	unsigned int i;

	new_node = allocate_btree_node(btree);

	if (IS_ERR(new_node))
		return new_node;

	new_node->type = FSM_LEAF_NODE;

	for (i = 0; i < n1->nr_active; i++) {
		new_node->key_vals[i] = n1->key_vals[i];
		new_node->children[i] = n1->children[i];
	}

	new_node->children[n1->nr_active] = n1->children[n1->nr_active];
	new_node->key_vals[n1->nr_active] = kv;

	for (i = 0; i < n2->nr_active; i++) {
		new_node->key_vals[i + n1->nr_active + 1] = n2->key_vals[i];
		new_node->children[i + n1->nr_active + 1] = n2->children[i];
	}
	new_node->children[2*btree->order - 1] = n2->children[n2->nr_active];

	new_node->nr_active = n1->nr_active + n2->nr_active + 1;
	new_node->type = n1->type;
	new_node->level = n1->level;

	free_btree_node(btree, n1);
	free_btree_node(btree, n2);

	return new_node;
}

/**
 * @brief		Used to remove a key from the FSM B-tree node.
 * @param [in]	btree	The FSM B-tree.
 * @param [in]	node	The node from which the key is to be removed.
 * @param [in]	pos	Record position in the node.
 * @return		Returns 0 on success, -1 on error.
 */
static int remove_key_from_leaf(struct emmcfs_fsm_btree *btree,
			 struct emmcfs_fsm_btree_node *node, unsigned int pos)
{
	if (node->type == FSM_INDEX_NODE)
		return -1;

	node->nr_active--;

	if (pos < node->nr_active)
		memmove(&node->key_vals[pos], &node->key_vals[pos+1],
			sizeof(struct fsm_btree_key) * (node->nr_active - pos));

	if (node->nr_active == 0) {
		if (btree->root == node)
			btree->root = NULL;

		free_btree_node(btree, node);
	}
	return 0;
}

/**
 * @brief 		Function is used to remove a node from a FSM B-tree.
 * @param [in]	btree	The FSM B-tree.
 * @param [in]	key	Key of the node to be removed.
 * @return		Returns 0 on success, -1 on error.
 */
int fsm_btree_delete(struct emmcfs_fsm_btree *btree, void *key)
{
	unsigned int i, index;
	struct emmcfs_fsm_btree_node *node = NULL, *rsibling, *lsibling;
	struct emmcfs_fsm_btree_node *comb_node, *parent;

	struct emmcfs_fsm_btree_node *sub_np_node;
	int sub_np_pos;

	struct fsm_btree_key to_remove_kv;

	node = btree->root;
	parent = NULL;

	/* Empty subtree */
	if (node == NULL)
		return 0;

del_loop:for (i = 0; ; i = 0) {

		if (IS_ERR(node))
			return PTR_ERR(node);

		/* Empty subtree */
		if (node == NULL)
			return 0;

		/* If there are no keys simply return */
		if (!node->nr_active)
			return 0;

		/* Fix the index of the key greater than or equal*/
		/* to the key that we would like to search*/
		while (i < node->nr_active &&
			btree->cmp_key(key, (void *)&node->key_vals[i]) > 0)
			i++;
		index = i;

		/*If we find such key break*/
		if (i < node->nr_active &&
			btree->cmp_key(key, (void *)&node->key_vals[i]) == 0)
			break;

		if (node->type == FSM_LEAF_NODE)
			return 0;

		/* Store the parent node */
		parent = node;

		/*To get a child node*/
		node = node->children[i];

		/* If NULL not found */
		if (node == NULL)
			return 0;

		if (index == (parent->nr_active)) {
			lsibling =  parent->children[parent->nr_active - 1];
			rsibling = NULL;
		} else if (index == 0) {
			lsibling = NULL;
			rsibling = parent->children[1];
		} else {
			lsibling = parent->children[i - 1];
			rsibling = parent->children[i + 1];
		}

		if (node->nr_active == btree->order - 1 && parent) {
			/* The current node has (t - 1) keys but
			 *  the RIGHT sibling has > (t - 1) keys*/
			if (rsibling &&
				(rsibling->nr_active > btree->order - 1)) {
				move_key(btree, parent, i, LEFT);
			/* The current node has (t - 1) keys but
			 * the LEFT sibling has (t - 1) keys */

			} else if (lsibling &&
				(lsibling->nr_active > btree->order - 1)) {
					move_key(btree, parent, i, RIGHT);
			/* Left sibling has (t - 1) keys */
			} else if (lsibling &&
				(lsibling->nr_active == btree->order - 1)) {
					node = merge_siblings(btree,
							parent, i, LEFT);
			/* Right sibling has (t - 1) keys */
			} else if (rsibling &&
				(rsibling->nr_active == btree->order - 1)) {
					node = merge_siblings(btree,
							parent, i, RIGHT);
			}
		}
	}
	/* Case 1 : The node containing the key is found and is the leaf node.
	   Also the leaf node has keys greater than the minimum required.
	   Simply remove the key*/
	if (IS_ERR(node))
		return PTR_ERR(node);

	if ((node->type == FSM_LEAF_NODE) &&
			(node->nr_active > btree->order - 1))
		return remove_key_from_leaf(btree, node, index);

	/* If the leaf node is the root permit deletion even if the number of
	   keys is less than (t - 1)*/
	if ((node->type == FSM_LEAF_NODE) && (node == btree->root))
		return remove_key_from_leaf(btree, node, index);

	/* Case 2: The node containing the key is found
	   and is an internal node */
	if (node->type == FSM_INDEX_NODE) {
		if (node->children[index]->nr_active > btree->order - 1) {
			btree->cpy_key(&to_remove_kv, &node->key_vals[index]);

			sub_np_node = get_max_key_pos(btree,
				node->children[index]);
			EMMCFS_BUG_ON(sub_np_node == NULL);
			sub_np_pos = sub_np_node->nr_active - 1;
			btree->cpy_key(&node->key_vals[index],
				&sub_np_node->key_vals[sub_np_pos]);

			btree->cpy_key(&sub_np_node->key_vals[sub_np_pos],
				&to_remove_kv);
			node = node->children[index];
			goto del_loop;

		} else if ((node->children[index + 1]->nr_active >
							btree->order - 1)) {
			btree->cpy_key(&to_remove_kv, &node->key_vals[index]);

			sub_np_node = get_min_key_pos(btree,
				node->children[index + 1]);
			EMMCFS_BUG_ON(sub_np_node == NULL);
			sub_np_pos = 0;
			btree->cpy_key(&node->key_vals[index],
				&sub_np_node->key_vals[sub_np_pos]);

			btree->cpy_key(&sub_np_node->key_vals[sub_np_pos],
				&to_remove_kv);

			node = node->children[index + 1];
			goto del_loop;

		} else if (
			node->children[index]->nr_active == btree->order - 1 &&
			node->children[index + 1]->nr_active ==
						btree->order - 1) {

			comb_node = merge_nodes(btree, node->children[index],
				node->key_vals[index],
				node->children[index + 1]);

			if (IS_ERR(comb_node))
				return PTR_ERR(comb_node);

			node->children[index] = comb_node;

			for (i = index + 1; i < node->nr_active; i++) {
				node->children[i] = node->children[i + 1];
				node->key_vals[i - 1] = node->key_vals[i];
			}
			node->nr_active--;
			if (node->nr_active == 0 && btree->root == node) {
				free_btree_node(btree, node);
				btree->root = comb_node;
			}
			node = comb_node;
			goto del_loop;
		}
	}

	/* Case 3:
		In this case start from the top of the tree and continue
		moving to the leaf node making sure that each node that
		we encounter on the way has at least 't' (order of the tree)
		keys */
	if ((node->type == FSM_LEAF_NODE) &&
			(node->nr_active > btree->order - 1))
		return remove_key_from_leaf(btree, node, index);

	return 0;
}

/**
 * @brief		Function is used to destroy FSM B-tree.
 * @param [in]	btree	The FSM B-tree to be destroyed.
 * @return	void
 */
void fsm_btree_destroy(struct emmcfs_fsm_btree *btree)
{
	unsigned i = 0;
	unsigned int current_level;

	struct emmcfs_fsm_btree_node *head, *tail, *node;
	struct emmcfs_fsm_btree_node *child, *del_node;

	if (btree->root == NULL)
		return;

	node = btree->root;
	current_level = node->level;
	head = node;
	tail = node;

	while (true) {
		if (head == NULL)
			break;
		if (head->level < current_level)
			current_level = head->level;


		if (head->type == FSM_INDEX_NODE) {
			for (i = 0 ; i < head->nr_active + 1; i++) {
				child = head->children[i];
				tail->next = child;
				tail = child;
				child->next = NULL;
			}
		}
		del_node = head;
		head = head->next;
		free_btree_node(btree, del_node);
	}
}

/**
 * @brief			Function is used to put a node to FSM B-tree.
 * @param [in]	btree		FSM B-tree to be updated.
 * @param [in]	block_offset	block_offset of inserted free space chunk
 * 				(physical number of the block to find).
 * @param [in]	length		Length key value (size of continuous free block
 * 				sequence to insert into btree).
 * @return			Returns the error code.
 */
int fsm_btree_free(struct emmcfs_fsm_btree *btree, __u64 block_offset,
	__u64 length)
{
	unsigned i;
	int ret = 0;
	struct emmcfs_fsm_btree_node *node = btree->root;
	struct fsm_btree_key key = {block_offset, length};

	if (node == NULL)
		return fsm_btree_insert(btree, &key);

	for (i = 0; ; i = 0) {
		while (i < node->nr_active) {
			if (block_offset == node->key_vals[i].block_offset +
				node->key_vals[i].length) {
				node->key_vals[i].length += length;
				if (((i+1) <  node->nr_active) &&
					(node->key_vals[i+1].block_offset ==
					node->key_vals[i].block_offset +
					node->key_vals[i].length)) {
					node->key_vals[i].length +=
					node->key_vals[i+1].length;
					key.block_offset = node->
						key_vals[i+1].block_offset;
					key.length = node->key_vals[i+1].length;
					ret = fsm_btree_delete(btree, &key);
				}
				return ret;
			}
			if ((block_offset + length) ==
				node->key_vals[i].block_offset) {

				length += node->key_vals[i].length;

				key.block_offset = node->
					key_vals[i].block_offset;
				key.length = node->key_vals[i].length;
				fsm_btree_delete(btree, &key);

				key.block_offset = block_offset;
				key.length = length;
				fsm_btree_insert(btree, &key);

				return ret;
			}
			i++;
		}

		if (node->type == FSM_LEAF_NODE)
			break;
		node = node->children[i];
	}

	key.block_offset = block_offset;
	key.length = length;
	fsm_btree_insert(btree, &key);

	return ret;
}

/**
 * @brief			Function is used to search a node in FSM B-tree and
 *				(if search succeed) remove node from FSM B-tree.
 * @param [in]	btree		The FSM B-tree to be searched.
 * @param [in]	block_offset	The block_offset key value (physical number
 * 				of the block to find).
 * @param [in]	length		Length key value (size of continuous
 * 				free block sequence to find).
 * @return			Returns physical block number of the required
 * 				continuous sequence of free blocks.
 */
__u64 fsm_btree_lookup(struct emmcfs_fsm_btree *btree, __u64 block_offset,
	__u32 length)
{
	unsigned int i;
	struct emmcfs_fsm_btree_node *node = btree->root;
	struct emmcfs_fsm_btree_node *tail;
	int ret = 0;

	if (node == NULL)
		return 0L;

	for (i = 0; ; i = 0) {
		while (i < node->nr_active &&
			(block_offset > node->key_vals[i].block_offset))
				i++;

		if (i < node->nr_active &&
			(block_offset == node->key_vals[i].block_offset) &&
			(length <= node->key_vals[i].length)) {
			struct fsm_btree_key key;
			if (length < node->key_vals[i].length) {
				key.block_offset =
					node->key_vals[i].block_offset;
				key.length = node->key_vals[i].length;
				ret = fsm_btree_delete(btree, &key);
				if (ret)
					return 0L;
				key.block_offset += length;
				key.length -= length;
				ret = fsm_btree_insert(btree, &key);
				if (ret)
					return 0L;
			} else {
				key.block_offset = block_offset;
				key.length = length;
				ret = fsm_btree_delete(btree, &key);
				if (ret)
					return 0L;
			}
			return block_offset;
		}
		if (node->type == FSM_LEAF_NODE)
			break;
		node = node->children[i];
	}

	node = btree->root;
	tail = node;

	while (true) {
		if (node == NULL)
			break;
		for (i = 0; i < node->nr_active; i++) {
			if (length == node->key_vals[i].length) {
				struct fsm_btree_key key;
				block_offset = node->key_vals[i].block_offset;
				key.block_offset = block_offset;
				key.length = length;
				fsm_btree_delete(btree, &key);
				return block_offset;
			}
			if (length < node->key_vals[i].length) {
				struct fsm_btree_key key;
				key.block_offset = block_offset =
					node->key_vals[i].block_offset;
				key.length = node->key_vals[i].length;
				fsm_btree_delete(btree, &key);
				key.block_offset += length;
				key.length -= length;
				fsm_btree_insert(btree, &key);
				return block_offset;
			}
		}
		if (node->type == FSM_INDEX_NODE) {
			for (i = 0 ; i < node->nr_active + 1; i++) {
				tail->next = node->children[i];
				tail = node->children[i];
				node->children[i]->next = NULL;
			}
		}
		node = node->next;
	}
	return 0L;
}

/**
 * @brief		Function is used to print the FSM B-tree node.
 * @param [in]	btree	The FSM B-tree to print.
 * @param [in]	node	The node to print.
 * @return	void
 */
static void print_single_node(struct emmcfs_fsm_btree *btree,
	struct emmcfs_fsm_btree_node *node)
{
	unsigned int i = 0;

	EMMCFS_DEBUG_FSM(" { ");
	while (i < node->nr_active)
		btree->print_key(&node->key_vals[i++]);

	EMMCFS_DEBUG_FSM("} (0x%p,cnt %d, level %d type %s) ",
		node, node->nr_active, node->level,
		(node->type == FSM_LEAF_NODE) ? "LEAF" : "INDEX");
}

/**
 * @brief		Function is used to print the FSM B-tree.
 * @param [in]	btree	The FSM B-tree to print.
 * @return	void
 */
void fsm_print_tree(struct emmcfs_fsm_btree *btree)
{
	unsigned int i = 0;
	unsigned int current_level;

	struct emmcfs_fsm_btree_node *head, *tail, *node = btree->root;

	if (!node) {
		EMMCFS_DEBUG_FSM("DEBUG: Subtree is empty\n");
		return;
	}
	current_level = node->level;
	head = node;
	tail = node;

	EMMCFS_DEBUG_FSM("DEBUG: Printing subtree\n");
	while (true) {
		if (head == NULL)
			break;
		if (head->level < current_level) {
			current_level = head->level;
			EMMCFS_DEBUG_FSM("\n");
		}
		print_single_node(btree, head);

		if (head->type == FSM_INDEX_NODE) {
			for (i = 0 ; i < head->nr_active + 1; i++) {
				tail->next = head->children[i];
				tail = head->children[i];
				head->children[i]->next = NULL;
			}
		}
		head = head->next;
	}
	EMMCFS_DEBUG_FSM("\n");
}
