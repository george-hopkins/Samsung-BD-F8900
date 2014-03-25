/**
 * @file	fs/emmcfs/btree.h
 * @brief	Basic B-tree operations - interfaces and prototypes.
 * @date	0/05/2012
 *
 * eMMCFS -- Samsung eMMC chip oriented File System, Version 1.
 *
 * This file contains prototypes, constants and enumerations for emmcfs btree
 * functioning
 *
 * Copyright 2011 by Samsung Electronics, Inc.,
 *
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

#ifndef BTREE_H_
#define BTREE_H_

#define EMMCFS_BNODE_DSCR(bnode) ((struct emmcfs_gen_node_descr *) bnode->data)

#define EMMCFS_BNODE_RECS_NR(bnode) \
	le16_to_cpu(EMMCFS_BNODE_DSCR(bnode)->recs_count)

/** How many free space node should have on removing records, to merge
 * with neighborhood */
#define EMMCFS_BNODE_MERGE_LIMIT	(0.7)

#include "mutex_on_sem.h"

/** TODO */
enum emmcfs_get_bnode_mode {
	/** The bnode is operated in read-write mode */
	EMMCFS_BNODE_MODE_RW = 0,
	/** The bnode is operated in read-only mode */
	EMMCFS_BNODE_MODE_RO
};

/** TODO */
enum emmcfs_node_type {
	EMMCFS_FIRST_NODE_TYPE = 1,
	/** bnode type is index */
	EMMCFS_NODE_INDEX = EMMCFS_FIRST_NODE_TYPE,
	/** bnode type is leaf */
	EMMCFS_NODE_LEAF,
	EMMCFS_NODE_NR,
};

/** TODO */
enum emmcfs_btree_type {
	EMMCFS_BTREE_FIRST_TYPE = 0,
	/** btree is catalog tree */
	EMMCFS_BTREE_CATALOG = EMMCFS_BTREE_FIRST_TYPE,
	/** btree is extents overflow tree */
	EMMCFS_BTREE_EXTENTS,
	EMMCFS_BTREE_TYPES_NR
};

typedef int (emmcfs_btree_key_cmp)(struct emmcfs_generic_key*,
		struct emmcfs_generic_key*);

/**
 * @brief 	On-disk structure to hold essential information about B-tree.
 */
struct emmcfs_raw_btree_head {
	/** Magic */
	__u8 magic[4];
	/** The bnode id of root of the tree */
	__le32 root_bnode_id;
	/** Height of the tree */
	__le16 btree_height;
	/** Padding */
	__u8 padding[2];
	/** Starting byte of free bnode bitmap, bitmap follows this structure */
	__u8 bitmap;
} __packed;

/**
 * @brief 	Structure contains information about bnode in runtime.
 */
struct emmcfs_bnode {
	/** Pointer to memory area where contents of bnode is mapped to */
	void *data;

	/** The bnode's id */
	__u32 node_id;

	/** Array of pointers to \b struct \b page representing pages in
	 * memory containing data of this bnode
	 */
	struct page **pages;

	/** Pointer to tree containing this bnode */
	struct emmcfs_btree *host;

	/** Mode in which bnode is got */
	enum emmcfs_get_bnode_mode mode;
};

#define EMMCFS_BNODE_CACHE_ITEMS 1000
/**
 * @brief 	An eMMCFS B-tree held in memory.
 */
struct emmcfs_btree {
	/** Superblock info pointer containing this tree */
	struct emmcfs_sb_info *sbi;
	/** The inode of special file containing  this tree */
	struct inode *inode;
	/** Number of pages enough to contain whole bnode in memory */
	unsigned int pages_per_node;
	/** Number of pages enough to contain whole bnode in memory
	 * (power of 2)*/
	unsigned int log_pages_per_node;
	/** Size of bnode in bytes */
	unsigned int node_size_bytes;
	/** Maximal available record length for this tree */
	unsigned short max_record_len;

	/** Type of the tree */
	enum emmcfs_btree_type btree_type;
	/** Comparison function for this tree */
	emmcfs_btree_key_cmp *comp_fn;

	/** Pointer to head (containing essential info)  bnode */
	struct emmcfs_bnode *head_bnode;
	/** Info about free bnode list */
	struct emmcfs_bnode_bitmap *bitmap;

	/** Simple cache for bnode caching */
	struct emmcfs_bnode *stupid_cache[EMMCFS_BNODE_CACHE_ITEMS];

	/** Lock to protect tree operations */
	rw_mutex_t *rw_tree_lock;
};

/**
 * @brief 	Macro gets essential information about tree contained in head
 *		tree node.
 */
#define EMMCFS_BTREE_HEAD(btree) ((struct emmcfs_raw_btree_head *) \
	(btree->head_bnode->data))

/**
 * @brief 	On-disk structure representing information about bnode common
 *		to all the trees.
 */
struct emmcfs_gen_node_descr {
	/** Magic */
	__u8 magic[2];
	/** Free space left in bnode */
	__le16 free_space;
	/** Amount of records that this bnode contains */
	__le16 recs_count;
	/** Node id */
	__le32 node_id;
	/** Node id of left sibling */
	__le32 prev_node_id;
	/** Node id of right sibling */
	__le32 next_node_id;
	/** Type of bnode node or index (value of enum emmcfs_node_type) */
	__u8 type;
} __packed;

/**
 * @brief 	Generic value for index nodes - node id of child.
 */
struct generic_index_value {
	/** Node id of child */
	__le32 node_id;
} __packed;

/**
 * @brief 	Runtime structure representing free bnode id bitmap.
 */
struct emmcfs_bnode_bitmap {
	/** Memory area containing bitmap itself */
	void *data;
	/** Size of bitmap */
	__u64 size;
	/** Starting bnode id */
	__u32 start_id;
	/** Maximal bnode id */
	__u32 end_id;
	/** First free id for quick free bnode id search */
	__u32 first_free_id;
	/** Amount of free bnode ids */
	__u32 free_num;
	/** Amount of bits in bitmap */
	__u32 bits_num;
	/** Pointer to bnode containing this bitmap */
	struct emmcfs_bnode *host;

	/** locking to modify data atomically */
	spinlock_t spinlock;
};
/* Set this to sizeof(crc_type) to handle CRC */
#define EMMCFS_BNODE_FIRST_OFFSET 4
#define EMMCFS_INVALID_NODE_ID 0

typedef __u32 emmcfs_bt_off_t;
#define EMMCFS_BT_INVALID_OFFSET ((__u32) (((__u64) 1 << 32) - 1))

/**
 * @brief 	Interface for finding data in the whole tree.
 */
struct emmcfs_bnode  *emmcfs_btree_find(struct emmcfs_btree *tree,
				struct emmcfs_generic_key *key,
				int *pos, enum emmcfs_get_bnode_mode mode);

/**
 * @brief 	Interface to function get_record.
 */
void *emmcfs_get_btree_record(struct emmcfs_bnode *bnode, int index);

/**
 * @brief 	Interface for simple insertion into tree.
 */
int emmcfs_btree_insert(struct emmcfs_btree *btree, void *new_data);

/**
 * @brief 	Interface for key removal.
 */
int emmcfs_btree_remove(struct emmcfs_btree *tree,
			struct emmcfs_generic_key *key);


/* Essential for B-tree algorithm functions */

/**
 * @brief 	Removes the specified key at level.
 */
int btree_remove(struct emmcfs_btree *tree,
		struct emmcfs_generic_key *key, int level);

/**
 * @brief 	Interface for extracting bnode into memory.
 */
struct emmcfs_bnode *emmcfs_get_bnode(struct emmcfs_btree *btree,
		__u32 node_id, enum emmcfs_get_bnode_mode mode);

/**
 * @brief 	Interface for freeing bnode structure.
 */
void emmcfs_put_bnode(struct emmcfs_bnode *bnode);

/**
 * @brief 	Mark bnode as free.
 */
int emmcfs_destroy_bnode(struct emmcfs_bnode *bnode);

/**
 * @brief 	Create, reserve and prepare new bnode.
 */
struct emmcfs_bnode *emmcfs_alloc_new_bnode(struct emmcfs_btree *btree);

/**
 * @brief 	Get descriptor of next bnode.
 */
struct emmcfs_bnode *emmcfs_get_next_bnode(struct emmcfs_bnode *bnode);

/**
 * @brief 	Mark bnode as dirty (data on disk and in memory differs).
 */
void emmcfs_mark_bnode_dirty(struct emmcfs_bnode *node);

/**
 * @brief	Get B-tree rood bnode id.
 */
u32 emmcfs_btree_get_root_id(struct emmcfs_btree *btree);

/**
 * @brief 	Interface for freeing bnode structure without
 *		saving it into cache.
 */
void emmcfs_put_cache_bnode(struct emmcfs_bnode *bnode);

inline void emmcfs_sign_bnode(void *bnode_data,
		struct emmcfs_btree *btree);

int emmcfs_sign_dirty_bnodes(struct address_space *mapping,
		struct emmcfs_btree *btree);

#endif /* BTREE_H_ */
