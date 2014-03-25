/**
 * @file	fs/emmcfs/emmcfs.h
 * @brief	Internal constants and data structures for eMMCFS.
 * @date	01/17/2012
 *
 * eMMCFS -- Samsung eMMC chip oriented File System, Version 1.
 *
 * This file defines eMMCFS common internal data structures, constants and
 * functions
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


#ifndef _EMMCFS_EMMCFS_H_
#define _EMMCFS_EMMCFS_H_

#include <linux/fs.h>
#include <linux/spinlock.h>
#include <linux/emmcfs_fs.h>
#include <linux/pagemap.h>
#include "btree.h"
#include "fsm_btree.h"
#include "debug.h"

#ifdef CONFIG_EMMCFS_NOOPTIMIZE
#pragma GCC optimize("O0")
#endif


/* Enabels additional print at mount - how long mount is */
/* #define CONFIG_EMMCFS_PRINT_MOUNT_TIME */

#define EMMCFS_BUG() do { BUG(); } while(0)

#if 0
EMMCFS_DEBUG_TMP("BUG_ON"); \

#endif
#define EMMCFS_BUG_ON(cond) do { \
	BUG_ON((cond)); \
} while(0)

/*#define CONFIG_EMMCFS_CHECK_FRAGMENTATION*/
/*#define	CONFIG_EMMCFS_POPO_HELPER*/

/** Special inodes */
/** root inode */
#define EMMCFS_ROOT_INO				1
/** catalog tree inode */
#define EMMCFS_CAT_TREE_INO			2
/** LEBs bitmap inode */
#define EMMCFS_LEB_BITMAP_INO			3
/** Extents tree inode */
#define EMMCFS_EXTENTS_TREE_INO			4
/** Free space bitmap inode */
#define EMMCFS_FREE_INODE_BITMAP_INO		6
/** Hardlinks area inode */
#define EMMCFS_HARDLINKS_AREA_INO		7
/** Orphan inodes inode */
#define EMMCFS_ORPHAN_INODES_INO		8
/** First file inode */
#define EMMCFS_1ST_FILE_INO			10

extern unsigned int file_prealloc;
extern unsigned int cattree_prealloc;
/** Base super block location in an eMMCFS volume.
 *  First 1024 bytes is reserved area. */
#define EMMCFS_RESERVED_AREA_LENGTH		1024

#define SECTOR_SIZE		512
#define SECTOR_SIZE_SHIFT	9
#define SECTOR_PER_PAGE		(PAGE_CACHE_SIZE / SECTOR_SIZE)
#define SB_SIZE 		(sizeof(struct emmcfs_super_block))
#define SB_SIZE_IN_SECTOR	(SB_SIZE / SECTOR_SIZE)
#define ESB_SIZE_IN_SECTOR	5

/** Maximal supported file size. Equal maximal file size supported by VFS */
#define EMMCFS_MAX_FILE_SIZE_IN_BYTES MAX_LFS_FILESIZE

/* The eMMCFS flags */
#define EXSB_DIRTY		0
#define EXSB_DIRTY_SNAPSHOT	1
#define IS_MOUNT_FINISHED	2

#define EMMCFS_ROOTDIR_NAME "root"
#define EMMCFS_ROOTDIR_OBJ_ID ((__u64) 0)

/* Hard links file defines */
#define INVALID_HLINK_ID ((u64)-1)
typedef u64 hlink_id_t;

/* Macros for calculating catalog tree expand step size.
 * x - total blocks count on volume
 * right shift 7 - empirical value. For example, on 1G volume catalog tree will
 * expands by 8M one step*/
#define TREE_EXPAND_STEP(x)	(x >> 7)

/* Preallocation blocks amount */
#define FSM_PREALLOC_DELTA	0

/* Length of log buffer */
#define BUFFER_LENGTH 32768
#define MAX_FUNCTION_LENGTH 30
#define EMMCFS_PROC_DIR_NAME "fs/emmcfs"
#define EMMCFS_MAX_STAT_LENGTH			55

/** @brief	Maintains private super block information.
 */
struct emmcfs_sb_info {
	/** The VFS super block */
	struct super_block	*sb;
	/** The page that contains superblocks */
	struct page *superblocks;
	/** The page that contains superblocks copy */
	struct page *superblocks_copy;
	/** The eMMCFS mapped raw superblock data */
	void *raw_superblock;
	/** The eMMCFS mapped raw superblock copy data */
	void *raw_superblock_copy;
	/** The eMMCFS flags */
	unsigned long flags;
	/** Allocated block size in bytes */
	unsigned int		block_size;
	/** Allocated block size shift  */
	unsigned int		block_size_shift;
	/** Allocated block size mask for offset  */
	unsigned int		offset_msk_inblock;
	/** Size of LEB in blocks */
	unsigned int		log_blocks_in_leb;
	/** Log sector per block */
	unsigned int		log_sectors_per_block;
	/** The eMMCFS mount options */
	unsigned int		mount_options;
	/** Total block count in the volume */
	unsigned long long	total_leb_count;
	/** The current value of free block count on the whole eMMCFS volume */
	unsigned long long	free_blocks_count;
	/** The files count on the whole eMMCFS volume */
	unsigned long long	files_count;
	/** The folders count on the whole eMMCFS volume */
	unsigned long long	folders_count;
	/** 64-bit uuid for volume */
	u64			volume_uuid;
	/** How many blocks in the bnode */
	unsigned long		btree_node_size_blks;
	/** Catalog tree in memory */
	struct emmcfs_btree	*catalog_tree;
	/** Maximum value of catalog tree height */
	int			max_cattree_height;
	/** Extents overflow tree */
	struct emmcfs_btree	*extents_tree;
	/** Number of blocks in LEBs bitmap */
	__le64		lebs_bm_blocks_count;
	/** Log2 for blocks described in one block */
	__le32		lebs_bm_log_blocks_block;
	/** Number of blocks described in last block */
	__le32		lebs_bm_bits_in_last_block;

	/** @brief	Free space bitmap information
 	*/
	struct {
		struct inode *inode;
		atomic64_t last_used;
	} free_inode_bitmap;

	/** Free space management */
	struct emmcfs_fsm_info *fsm_info;

	/** @brief	Hard links information
	*/
	struct {
		struct inode *inode;
		hlink_id_t last_used_id;
	} hard_links;

	/** Snapshot manager */
	struct emmcfs_snapshot_info *snapshot_info;

#ifdef CONFIG_EMMCFS_CHECK_FRAGMENTATION
	/** TODO */
	unsigned long in_fork[EMMCFS_EXTENTS_COUNT_IN_FORK];
	/** TODO */
	unsigned long in_extents_overflow;
#endif
	/** Circular buffer for logging */
	struct emmcfs_log_buffer *buffer;

	/** Number of sectors per volume */
	__le64	sectors_per_volume;
#ifdef CONFIG_EMMCFS_PROC_INFO
	/** TODO */
	__u64 magic;
	/** TODO */
	struct emmcfs_nesting_keeper *nk;
#endif

	int bugon_count;
};

#ifdef CONFIG_EMMCFS_PROC_INFO
#define EMMCFS_SB_DEBUG_MAGIC 0x12345678
#define SB_HAS_EMMCFS_MAGIC(sb) (sb && EMMCFS_SB(sb) && EMMCFS_SB(sb)->magic ==\
		EMMCFS_SB_DEBUG_MAGIC)
#endif

/** @brief	Mutexes names, using by mutex tracker
 */
enum mutexes {EMMCFS_NEST = 0, EXT_TREE_R_LOCK, CAT_TREE_R_LOCK,
	TRANSACTION_R_LOCK, TASK_LOG_R_LOCK, FS_R_LOCK, FSM_R_LOCK,
	EXT_TREE_W_LOCK, CAT_TREE_W_LOCK, TRANSACTION_W_LOCK, TASK_LOG_W_LOCK,
	FS_W_LOCK, FSM_W_LOCK
};

#define EMMCFS_MUTEX_COUNT 12

/** @brief 	Structure for nesting keeper RB-tree.
 */
struct emmcfs_nest_log {
	/** Task pid */
	int pid;
	/** Task nests n times */
	unsigned int n;
	/** Node definition for inclusion in a red-black tree */
	struct rb_node node;
};

/** @brief 	Store for pids that locks the mutexes.
 */
struct pid_list {
	/** Pid that contains current element */
	int pid;
	/** Next element of the pid list */
	struct pid_list *next;
};

/** @brief 	Structure, that stores nesting of current function call and
 *		info about locked mutexes.
 */
struct emmcfs_nesting_keeper {
	/** Mutex for working with nesting keeper */
	spinlock_t nest_log_lock;
	/** Array of pid list that contain what pids hold mutexes */
	struct pid_list locked_mutexes[EMMCFS_MUTEX_COUNT];
	/** Number of processes that hold mutex */
	int size[EMMCFS_MUTEX_COUNT];
	/** Root of RB-tree using by nesting keeper */
	struct rb_root log_root;
};

/** @brief 	Current extent information.
 */
struct emmcfs_extent_info {
	/** First block */
	u64 first_block;
	/** Block count */
	u32 block_count;
};

/** @brief 	Current fork information.
 */
struct emmcfs_fork_info {
	/**
	 * The total number of allocation blocks which are
	 * allocated for file system object described by this fork */
	u32 total_block_count;
	/**
	 * Number of blocks,which are preallocated by free space manager for
	 * file system object described by this fork */
	__u32 prealloc_block_count;
	/* Preallocated space first block physical number */
	sector_t prealloc_start_block;
	/**
	 * The set of extents which describe file system object's blocks
	 * placement */
	struct emmcfs_extent_info extents[EMMCFS_EXTENTS_COUNT_IN_FORK];
	/** Used extents */
	unsigned int used_extents;
};

/** @brief 	Maintains free space management info.
 */
struct emmcfs_fsm_info {
	/* Superblock info structure pointer */
	struct emmcfs_sb_info *sbi;	
	/** Free space start (in blocks)*/
	__u32		free_space_start;
	/** LEB state bitmap pages */
	struct page	**pages;
	/** TODO */
	__u32		page_count;
	/** Mapped pages (ptr to LEB bitmap)*/
	__u32		*data;
	/** Bitmap length in bytes */
	__u32		length_in_bytes;
	/** Lock for LEBs bitmap  */
	struct mutex	lock;
	/** Free space bitmap inode (VFS inode) */
	struct inode	*leb_bitmap_inode;
	/** Lock for FSM B-tree */
	struct  emmcfs_fsm_btree tree;
};

/** @brief 	Maintains private inode info.
 */
struct emmcfs_inode_info {
	/** VFS inode */
	struct inode	vfs_inode;
	/** The inode fork information */
	struct emmcfs_fork_info fork;
	/** Truncate_mutex is for serializing emmcfs_truncate_blocks() against
	 *  emmcfs_getblock()
	 */
	struct mutex truncate_mutex;
	/* TODO - put name-par_id and hlink_id under union */
	/** Name information */
	char *name;
	/** Parent ID */
	__u64 parent_id;
	/** Hard link information */
	hlink_id_t hlink_id;
	/** Hint for current inode.
	 *  If record related to current inode was not moved, we can easy get
	 *  this record 
	 */
	struct {
		/** The bnode ID */
		__u32 bnode_id;
		/** Position */
		__s32 pos;
	} bnode_hint;

	/** TODO */
	int is_new;
};

/** @brief 	Maintains snapshot info.
 */
struct emmcfs_snapshot_info {
	/** Current snapshot callback's */
	const  struct	snapshot_operations *snapshot_op;
	/** Pages per snapshot head */
	unsigned int pages_per_head;
	/** Transactions count */
	unsigned int transactions_count;
	/** Head size in bytes */
	unsigned int head_size_bytes;
	/** Mapped snapshot head pages */
	void	*mapped_data;
	/** Last page index */
	pgoff_t page_index;
	/** Transaction rb-tree root */
	struct rb_root snapshot_root;
	/** Task log rb-tree root */
	struct rb_root task_log_root;
	/** Lock transaction rb-tree root */
	spinlock_t transaction_rbtree_lock;
	/** Lock transaction rb-tree root */
	spinlock_t task_log_lock;
	/** Transaction lock */
	struct rw_semaphore *transaction_lock;
	/** Maximum pages in snapshot */
	unsigned int maximum_transaction_lenght;
	/** Snapshot length in pages */
	unsigned int max_pages_in_snapshot;
	/** Lock metadata update */
	struct mutex insert_lock;
	/** Semaphore which will garantee that snapshot head will be written
	 * AFTER all transaction pages */
	struct rw_semaphore head_write_sem;
	/*snapshot head version*/
	u32 head_version;
	/** Various flags */
	int flags;
};

/** @brief 	Structure for log buffer data.
 */
struct emmcfs_log_buffer_data {
	/** Functioncall name */
	char name[MAX_FUNCTION_LENGTH];
	/** Process id called this function */
	int pid;
	/** TODO */
	unsigned int nest;
	/** TODO */
	unsigned int mutex;
	/** TODO */
	int ret_code;
	/** Time */
	struct timeval time;
};

/** @brief 	Structure for log buffer.
 */
struct emmcfs_log_buffer {
	/** Index of oldest element */
	int	start;
	/** Index at which to write new element  */
	int	end;
	/** Vector of elements */
	struct emmcfs_log_buffer_data *elements;
	/** Vector for printing elements */
	char *element_to_string;

	struct emmcfs_sb_info *sbi;
};

/** @brief 	Structure transaction info.
 */
struct	emmcfs_transaction_data {
	/** Node definition for inclusion in a red-black tree */
	struct rb_node transaction_node;
	/** Read from sector (original data place) */
	sector_t on_volume;
	/** Write to sector (data) */
	sector_t on_snapshot;
	/** Block count in transaction */
	unsigned int pages_count;
};

/** @brief 	Task logging.
 */
struct emmcfs_task_log {
	/** Task pid */
	int pid;
	/** Task hold r-lock n times */
	unsigned int r_lock_count;
	/** Node definition for inclusion in a red-black tree */
	struct rb_node node;
};

/**
 * @brief 		Get pointer to value in key-value pair.
 * @param [in]	key_ptr	Pointer to key-value pair
 * @return 		Returns pointer to value in key-value pair
 */
static inline void *get_value_pointer(void *key_ptr)
{
	struct emmcfs_cattree_key *key = key_ptr;
	return key_ptr + key->gen_key.key_len;
};

/** @brief 	Find function information.
 */
struct emmcfs_find_data {
	/** Filled by find */
	struct emmcfs_bnode *bnode;
	/** The bnode ID */
	__u32 bnode_id;
	/** Position */
	__s32 pos;
};

/** @brief 	Extents overflow information.
 */
struct emmcfs_exttree_key {
	/** Key */
	struct emmcfs_generic_key gen_key;
	/** Object ID */
	__le64 object_id;
	/** Block number */
	__le64 iblock;
} __packed;

/* emmcfs_exttree_key.iblock special values */
#define IBLOCK_DOES_NOT_MATTER	0xFFFFFFFF /* any extent for specific inode */
#define IBLOCK_MAX_NUMBER	0xFFFFFFFE /* find last extent ... */

/** @brief 	Extents overflow tree information.
 */
struct emmcfs_exttree_record {
	/** Key */
	struct emmcfs_exttree_key key;
	/** Extent for key */
	struct emmcfs_extent lextent;
} __packed;

/** @brief 	Snapshot operations.
 */
struct snapshot_operations {
	/** Finish working with the snapshot */
	int (*clear_snapshot)(struct emmcfs_sb_info *);
	/** Add new pages to snapshot */
	int (*add_to_snapshot)(struct emmcfs_sb_info *sbi, struct inode *inode,
		struct page **original_pages, unsigned int page_count);
	/** Prepare snapshot head to flash on disk */
	int (*commit_snapshot)(struct emmcfs_sb_info *);
	/** Restore consistent volume state from snapshot */
	int (*restore_snapshot)(struct emmcfs_sb_info *);
	/** Start transaction */
	int (*start_transaction)(struct emmcfs_sb_info *);
	/** Stop transaction */
	int (*stop_transaction)(struct emmcfs_sb_info *);
};

/**
 * @brief		Get eMMCFS inode info structure from
 *			encapsulated inode.
 * @param [in]	inode	VFS inode
 * @return		Returns pointer to eMMCFS inode info data structure
 */
static inline struct emmcfs_inode_info *EMMCFS_I(struct inode *inode)
{
	return container_of(inode, struct emmcfs_inode_info, vfs_inode);
}

/**
 * @brief		Get eMMCFS superblock from VFS superblock.
 * @param [in]	sb	The VFS superblock
 * @return		Returns eMMCFS run-time superblock
 */
static inline struct emmcfs_sb_info *EMMCFS_SB(struct super_block *sb)
{
	return sb->s_fs_info;
}

/**
 * @brief		Calculates count of blocks,
 *			necessary for store inode->i_size bytes.
 * @param [in]	inode	VFS inode
 * @return		Returns pointer to eMMCFS inode info data structure
 */
static inline __u32 inode_size_to_blocks(struct inode *inode)
{
	return (i_size_read(inode) + EMMCFS_SB(inode->i_sb)->block_size - 1) >>
		EMMCFS_SB(inode->i_sb)->block_size_shift;
}

/**
 * @brief		Get eMMCFS snapshot run-time structure.
 * @param [in]	inode	The VFS inode
 * @return		Returns pointer to struct emmcfs_snapshot_info
 */
static inline
struct emmcfs_snapshot_info *EMMCFS_SNAPSHOT_INFO(struct inode *inode)
{
	return EMMCFS_SB(inode->i_sb)->snapshot_info;
}

static inline int emmcfs_produce_err(struct emmcfs_sb_info *sbi)
{
	if(sbi->bugon_count <= 0) {
		BUG_ON(sbi->bugon_count < 0);
		sbi->bugon_count--;
		return -ERANGE;
	}
	sbi->bugon_count--;

	return 0;
}

/**
 * @brief		Validate fork.
 * @param [in] fork	Pointer to the fork for validation
 * @return		Returns 1 if fork is valid, 0 in case of wrong fork
 * */
static inline int is_fork_valid(const struct emmcfs_fork *fork)
{
	if (!fork)
		goto ERR;
	if (fork->magic != EMMCFS_FORK_MAGIC)
		goto ERR;

	return 1;
ERR:
	EMMCFS_ERR("fork is invalid");
	return 0;
}

/**
 * @brief 		Set flag in superblock.
 * @param [in]	sbi	Superblock information
 * @param [in]	flag	Value to be set
 * @return	void
 */
static inline void set_sbi_flag(struct emmcfs_sb_info *sbi, int flag)
{
	set_bit(flag, &sbi->flags);
}

/**
 * @brief 		Check flag in superblock.
 * @param [in] 	sbi 	Superblock information
 * @param [in] 	flag 	Flag to be checked
 * @return 		Returns flag value
 */
static inline int is_sbi_flag_set(struct emmcfs_sb_info *sbi, int flag)
{
	return test_bit(flag, &sbi->flags);
}

/**
 * @brief 		Clear flag in superblock.
 * @param [in] 	sbi 	Superblock information
 * @param [in] 	flag 	Value to be clear
 * @return	void
 */
static inline void clear_sbi_flag(struct emmcfs_sb_info *sbi, int flag)
{
	clear_bit(flag, &sbi->flags);
}

/**
 * @brief		Copy layout fork into run time fork.
 * @param [out]	rfork	Run time fork
 * @param [in] 	lfork	Layout fork
 * @return	void
 */
static inline void emmcfs_lfork_to_rfork(struct emmcfs_fork *lfork,
		struct emmcfs_fork_info *rfork)
{
	unsigned i;
	/* Caller must check fork, call is_fork_valid(struct emmcfs_fork *) */
	/* First blanked extent means - no any more valid extents */

	rfork->used_extents = 0;
	for (i = 0; i < EMMCFS_EXTENTS_COUNT_IN_FORK; ++i) {
		struct emmcfs_extent *lextent;

		lextent = &lfork->extents[i];

		rfork->extents[i].first_block = le64_to_cpu(lextent->begin);
		rfork->extents[i].block_count = le32_to_cpu(lextent->length);
		if (rfork->extents[i].first_block)
			rfork->used_extents++;
	}
}

/** 
 * @brief 		Get current time for inode.
 * @param [in] 	inode	The inode for which current time will be returned
 * @return 		Time value for current inode
 */
static inline struct timespec emmcfs_current_time(struct inode *inode)
{
	return (inode->i_sb->s_time_gran < NSEC_PER_SEC) ?
		current_fs_time(inode->i_sb) : CURRENT_TIME_SEC;
}

/* super.c */

/**
 * @brief 	Sync starting superblock.
 */
int emmcfs_sync_first_super(struct super_block *sb);

/**
 * @brief 	Sync finalizing superblock.
 */
int emmcfs_sync_second_super(struct super_block *sb);

/* inode.c */
/**
 * @brief	Sync all metadata.
 */
int emmcfs_sync_metadata(struct emmcfs_sb_info *sbi);

/**
 * @brief	Method to read inode to inode cache.
 */
struct inode *emmcfs_iget(struct super_block *sb, unsigned long ino,
		struct emmcfs_find_data *find_data);

/**
 * @brief	Checks and copy layout fork into run time fork.
 */
int emmcfs_parse_fork(struct inode *inode, struct emmcfs_fork *lfork);

/**
 * @brief	Form layout fork from run time fork.
 */
void emmcfs_form_fork(struct emmcfs_fork *lfork, struct inode *inode);

/**
 * @brief	Translation logical numbers to physical.
 */
int emmcfs_get_block(struct inode *inode, sector_t iblock,
			struct buffer_head *bh_result, int create);

/**
 * @brief 	TODO Check it!!! Find old block in snapshots.
 */
sector_t emmcfs_find_old_block(struct emmcfs_inode_info *inode_info,
				sector_t iblock,  __u32 *max_blocks);

/**
 * @brief 	Free several inodes.
 */
int emmcfs_free_inode_n(struct emmcfs_sb_info *sbi, __u64 inode_n);

/* options.c */

/**
 * @brief	Parse eMMCFS options.
 */
int emmcfs_parse_options(struct super_block *sb, char *input);

/* btree.c */

/**
 * @brief 	Get next B-tree record transparently to user.
 */
void *emmcfs_get_next_btree_record(struct emmcfs_bnode **__bnode, int *index);

/**
 * @brief 	B-tree verifying
 */
int emmcfs_verify_btree(struct emmcfs_btree *btree);

/* bnode.c */

/**
 * @brief 	Get from cache or create page and buffers for it.
 */
struct page *emmcfs_alloc_new_page(struct address_space *, pgoff_t);

/**
 * @brief 	Build free bnode bitmap runtime structure
 */
struct emmcfs_bnode_bitmap *build_free_bnode_bitmap(void *data,
		__u32 start_bnode_id, __u64 size_in_bytes,
		struct emmcfs_bnode *host_bnode);

/**
 * @brief 	Clear memory allocated for bnode bitmap.
 */
void emmcfs_destroy_free_bnode_bitmap(struct emmcfs_bnode_bitmap *bitmap);

/* Preallocate a reserve for splitting btree */
int emmcfs_prealloc_bnode_reserve(struct emmcfs_btree *btree,
		__u32 alloc_to_bnode_id);
/* cattree.c */

/**
 * @brief	Catalog tree key compare function for case-sensitive usecase.
 */
int emmcfs_cattree_cmpfn(struct emmcfs_generic_key *__key1,
		struct emmcfs_generic_key *__key2);

/**
 * @brief 	Catalog tree key compare function for case-insensitive usecase.
 */
int emmcfs_cattree_cmpfn_ci(struct emmcfs_generic_key *__key1,
		struct emmcfs_generic_key *__key2);

/**
 * @brief 	Interface to search specific object (file) in the catalog tree.
 */
int emmcfs_cattree_find(struct emmcfs_sb_info *sbi,
		__u64 parent_id, const char *name, int len,
		struct emmcfs_find_data *fd, enum emmcfs_get_bnode_mode mode);

/**
 * @brief 	Allocate key for catalog tree record.
 */
struct emmcfs_cattree_key *emmcfs_alloc_cattree_key(int name_len,
		int record_type);

/**
 * @brief 	Fill already allocated key with data.
 */
void emmcfs_fill_cattree_key(struct emmcfs_cattree_key *fill_key, u64 parent_id,
		struct qstr *name);

/**
 * @brief 	Fill already allocated value area (file or folder) with data
 *		from VFS inode.
 */
int emmcfs_fill_cattree_value(struct inode *inode, void *value_area);

/**
 * @brief 	Add new object into catalog tree.
 */
int emmcfs_add_new_cattree_object(struct inode *inode, u64 parent_id,
		struct qstr *name);

/**
 * @brief	Find first child object for the specified catalog.
 */
struct emmcfs_bnode *emmcfs_find_first_catalog_child(struct inode *inode,
		int *pos);

/**
 * @brief 	Get the object from catalog tree basing on hint.
 */
int emmcfs_get_from_cattree(struct emmcfs_inode_info *inode_i,
		struct emmcfs_find_data *fd, enum emmcfs_get_bnode_mode mode);

/* extents.c */

/**
 * @brief 	TODO add comments.
 */
int emmcfs_exttree_cache_init(void);

/**
 * @brief 	TODO add comments.
 */
void emmcfs_exttree_cache_destroy(void);

/**
 * @brief 	TODO add comments.
 */
struct emmcfs_exttree_key *emmcfs_get_exttree_key(void);

/**
 * @brief 	TODO add comments.
 */
void emmcfs_put_exttree_key(struct emmcfs_exttree_key *key);

/**
 * @brief	Add newly allocated blocks chunk to extents overflow area.
 */
int emmcfs_exttree_add(struct emmcfs_inode_info *inode_info, sector_t iblock,
		sector_t start_block, sector_t block_count);

/**
 * @brief	Helper function for logical to physical block numbers
 *		translation - find bnode which contains exttree record
 *		corresponded to iblock in extents overflow tree.
 */
struct emmcfs_bnode *emmcfs_extent_find(struct emmcfs_inode_info *inode_info,
		sector_t iblock, int *pos, enum emmcfs_get_bnode_mode mode);

/**
 * @brief	Expand file in extents overflow area.
 */
sector_t emmcfs_exttree_add_block(struct emmcfs_inode_info *inode_info,
				  sector_t iblock, int cnt);
/**
 * @brief	Logical to physical block numbers translation for blocks
 * 		which placed in extents overflow.
 */
sector_t emmcfs_exttree_get_block(struct emmcfs_inode_info *inode_info,
				  sector_t iblock, __u32 *max_blocks);
/**
 * @brief	Extents tree key compare function.
 */
int emmcfs_exttree_cmpfn(struct emmcfs_generic_key *__key1,
		struct emmcfs_generic_key *__key2);

/* fsm.c */

/**
 * @brief	Build free space management.
 */
int emmcfs_fsm_build_management(struct super_block *);

/**
 * @brief	Destroy free space management.
 */
void emmcfs_fsm_destroy_management(struct super_block *);

/**
 * @brief	Get free space block chunk from tree and update free
 *		space manager bitmap.
 */
__u64 emmcfs_fsm_get_free_block(struct emmcfs_inode_info *inode_info,
		__u64 block_offset, __u32 length_in_blocks);

/**
 * @brief	Function is the fsm_free_block_chunk wrapper. Pust free space
 * 		chunk to tree and updates free space manager bitmap.
 *		This function is called during truncate or unlink inode
 * 		processes.
 */
int emmcfs_fsm_put_free_block(struct emmcfs_inode_info *inode_info,
	__u64 block_offset, __u32 length_in_blocks);

/**
 * @brief	Puts preallocated blocks back to the tree.
 */
void emmcfs_fsm_discard_preallocation(struct emmcfs_inode_info *inode_info);

/**
 * @brief	Function is the fsm_free_block_chunk wrapper.
 *		It is called during evict inode or process orphan inodes
 *		processes (inode already doesn't exist, but exttree inode blocks
 *		still where).
 */
int emmcfs_fsm_free_exttree_extent(struct emmcfs_sb_info *sbi,
	__u64 offset, __u32 length);

/* fsm_btree.c */

/**
* @brief	Function used to initialize FSM B-tree.
*/
void emmcfs_init_fsm_btree(struct  emmcfs_fsm_btree *btree);

/**
 * @brief	Initialize the eMMCFS FSM B-tree cache.
 */
int  emmcfs_fsm_cache_init(void);

/**
 * @brief	Destroy the eMMCFS FSM B-tree cache.
 */
void emmcfs_fsm_cache_destroy(void);

/* file.c */

/**
 * @brief	This function is called when a file is opened with O_TRUNC or
 *		truncated with truncate()/ftruncate() system calls.
 *		1) It truncates exttree extents according to new file size.
 *		2) If fork internal extents contains more extents than new file
 *		size in blocks, internal fork is also truncated.
 */
int emmcfs_truncate_blocks(struct inode *inode, loff_t offset);

/**
 * @brief	This function is called during inode unlink VFS call.
 *		1) It clears inode internal fork extents.
 *		2) If extents tree extents for this inode exist it populates
 *		orphan inode list.
 *		Exttree extents and orphan inode record for this inode will be
 *		deleted later, during delete/evict inode VFS call.
 */
int emmcfs_fsm_free_runtime_fork(struct inode *inode);

/**
 * @brief	This function is called during inode delete/evict VFS call.
 *		Inode internal fork extents have been already cleared, it's
 *		time to clear exttree extents for this inode and, finally,
 *		remove exttree orphan inode list record for this inode.
 */
int emmcfs_fsm_free_exttree(struct emmcfs_sb_info *sbi, __u64 ino);

/* snapshot.c */

/**
 * @brief	Builds snapshot manager.
 */
int emmcfs_build_snapshot_manager(struct emmcfs_sb_info *sbi);

/**
 * @brief	Destroys snapshot manager.
 */
void emmcfs_destroy_snapshot_manager(struct emmcfs_sb_info *sbi);

/* procfs.c*/

/**
 * @brief	Builds log buffer.
 */
int emmcfs_build_log_buffer(struct emmcfs_sb_info *sbi);

/**
 * @brief	Destroys log buffer.
 */
void emmcfs_destroy_log_buffer(struct emmcfs_sb_info *sbi);

/**
 * @brief	Adds function call/exit to the logging buffer.
 */
int emmcfs_function_add(struct emmcfs_log_buffer *buffer, const char *name,
			int8_t flag, int ret);

/**
 * @brief	Creates proc entry for eMMCFS.
 */
int emmcfs_dir_init(void);

/**
 * @brief	Destroys proc entry for eMMCFS.
 */
void emmcfs_dir_exit(void);

/**
 * @brief	Gets bactrace for all mounted eMMCFS devices.
 */
int emmcfs_get_backtrace(void);

/**
 * @brief 	Initializing eMMCFS superblocks list.
 */
int emmcfs_init_sb_list(void);

/**
 * @brief 	Adds eMMCFS superblock to global superblock list.
 */
int emmcfs_add_sb_to_list(struct emmcfs_sb_info *sbi);

/**
 * @brief 	Removes eMMCFS superblock from global superblock list.
 */
int emmcfs_remove_from_list(struct emmcfs_sb_info *sbi);

/**
 * @brief	Nesting keeper constructor.
 */
int emmcfs_build_nesting_keeper(struct emmcfs_sb_info *sbi);

/**
 * @brief	Nesting keeper destructor.
 */
void emmcfs_destroy_nesting_keeper(struct emmcfs_sb_info *sbi);

/**
 * @brief	Adds mutex lock info, or increments nesting.
 */
int emmcfs_add_or_inc_data(struct emmcfs_sb_info *sbi, enum mutexes flag);

/**
 * @brief	Removes mutex lock info, or decrements nesting.
 */
int emmcfs_dec_data(struct emmcfs_sb_info *sbi, enum mutexes flag);

/**
 * @brief	Gets mutex lock info, or current nesting for pid.
 */
int *emmcfs_get_data(struct emmcfs_nesting_keeper *nk, enum mutexes flag);

/* data.c */

/**
 * @brief 	Read page from the given sector address.
 * 		Fill the locked page with data located in the sector address.
 * 		Read operation is synchronous, and caller must unlock the page.
 */
int emmcfs_read_page(struct block_device *, struct page *, sector_t ,
			unsigned int , unsigned int);

/**
 * @brief 	Write page to the given sector address.
 * 		Write the locked page to the sector address.
 * 		Write operation is synchronous and caller must unlock the page.
 */
int emmcfs_write_page(struct block_device *, struct page *, sector_t ,
			unsigned int , unsigned int);

/**
 * @brief 	Allocate new BIO.
 */
struct bio *allocate_new_bio(struct block_device *bdev,
		sector_t first_sector, int nr_vecs);

/**
 * @brief 	Read page from the given sector address.
 * 		Fill the locked page with data located in the sector address.
 * 		Read operation is synchronous, and caller must unlock the page.
 */
int emmcfs_read_pages(struct block_device *bdev, struct page **page,
			sector_t sector_addr, unsigned int page_count);

/**
 * @brief 	Write page to the given sector address.
 *        	Write the locked page to the sector address.
 *        	Write operation is synchronous, and caller must unlock the page.
 */
int emmcfs_write_snapshot_pages(struct emmcfs_sb_info *sbi, struct page **pages,
		sector_t start_sector, unsigned int page_count, int mode);

/**
 * @brief	TODO add comments.
 */
int emmcfs_mpage_writepages(struct address_space *mapping,
		struct writeback_control *wbc);

/* orphan.c */

/**
 * @brief	After mount orphan inodes processing.
 */
int emmcfs_process_orphan_inodes(struct emmcfs_sb_info *sbi);

/**
 * @brief	Adds inode to orphan inodes store.
 */
int emmcfs_add_orphan_inode(struct inode *inode);

/**
 * @brief			Add chunk to snapshot.
 * @param [in]	inode		The VFS inode
 * @param [in]	pages		Data pages
 * @param [in]	page_count 	Page count
 * @return			Returns 0 on success, errno on failure
 */
static inline int EMMCFS_ADD_CHUNK(struct inode *inode, struct page **pages,
		unsigned int page_count)
{
	int error = 0;

	error = EMMCFS_SNAPSHOT_INFO(inode)->snapshot_op->
		add_to_snapshot(EMMCFS_SB(inode->i_sb),
		inode, pages, page_count);

	if (error) {
		if (!is_sbi_flag_set(EMMCFS_SB(inode->i_sb), IS_MOUNT_FINISHED))
			return error;
		else
			EMMCFS_BUG();
	}
		

	return 0;
}

/**
 * @brief		Flash changed metadata to disk.
 * @param [in]	sbi	The eMMCFS run-time superblock
 * @return		Returns 0 on success, errno on failure
 */
static inline int EMMCFS_COMMIT_SNAPSHOT(struct emmcfs_sb_info *sbi)
{
	return sbi->snapshot_info->snapshot_op->commit_snapshot(sbi);
}

/**
 * @brief		Clear on-disk snapshot.
 * @param [in]	sbi	The eMMCFS run-time superblock
 * @return		Returns 0 on success, errno on failure
 */
static inline int EMMCFS_CLEAR_SNAPSHOT(struct emmcfs_sb_info *sbi)
{
	return sbi->snapshot_info->snapshot_op->clear_snapshot(sbi);

}
/**
 * @brief		Restore metadata from snapshot if it is possible.
 * @param [in]	sbi	The eMMCFS run-time superblock
 * @return		Returns 0 on success, errno on failure
 */
static inline int EMMCFS_RESTORE_FROM_SNAPSHOT(struct emmcfs_sb_info *sbi)
{
	return sbi->snapshot_info->snapshot_op->restore_snapshot(sbi);
}

/**
 * @brief		Start the new transaction.
 * @param [in]	sbi	The eMMCFS run-time superblock
 * @return		Returns 0 on success, errno on failure
 */
#define EMMCFS_START_TRANSACTION(sbi)\
do {\
	EMMCFS_DEBUG_TRN("START TRNS");\
	sbi->snapshot_info->snapshot_op->start_transaction(sbi);\
} while (0)

/**
 * @brief		Finish transaction.
 * @param [in]	sbi	The eMMCFS run-time superblock
 * @return		Returns 0 on success, errno on failure
 */
#define EMMCFS_STOP_TRANSACTION(sbi)\
do {\
	EMMCFS_DEBUG_TRN("STOP TRNS");\
	sbi->snapshot_info->snapshot_op->stop_transaction(sbi);\
} while (0)

/* macros */

#define EMMCFS_IS_READONLY(sb) (sb->s_flags & MS_RDONLY)
#define EMMCFS_SET_READONLY(sb) (sb->s_flags |= MS_RDONLY)

#define EMMCFS_IS_FOLDER(record_type)\
	((record_type) == EMMCFS_CATALOG_FOLDER_RECORD)

#define EMMCFS_IS_FILE(record_type)\
	((record_type) & EMMCFS_CATALOG_FILE_RECORD)

#define EMMCFS_IS_REGFILE(record_type)\
	((record_type) == EMMCFS_CATALOG_FILE_RECORD)

#define EMMCFS_BLOCKS_IN_LEB(sbi) (1 << sbi->log_blocks_in_leb)

#define EMMCFS_LEB_FROM_BLK(sbi, blk) (blk >> sbi->log_blocks_in_leb)
#define EMMCFS_LEB_START_BLK(sbi, leb_n) (leb_n << sbi->log_blocks_in_leb)
#define EMMCFS_BLK_INDEX_IN_LEB(sbi, blk) \
		(blk & ((1 << sbi->log_blocks_in_leb) - 1))

#define EMMCFS_RAw_SB(sbi)	((struct emmcfs_super_block *) \
				sbi->raw_superblock)

#define EMMCFS_RAw_EXSB(sbi)	((struct emmcfs_extended_super_block *) \
				(sbi->raw_superblock + SB_SIZE))


#define EMMCFS_RAw_EXSB_COPY(sbi) ((struct emmcfs_extended_super_block *) \
				(sbi->raw_superblock_copy + SB_SIZE))

#define EMMCFS_SNAPSHOT_START(sbi) le64_to_cpu(EMMCFS_RAw_EXSB(sbi)-> \
		snapshot_fork.extents[0].begin)

#define EMMCFS_SNAPSHOT_LENGHT(sbi) le32_to_cpu(EMMCFS_RAw_EXSB(sbi)-> \
			snapshot_fork.extents[0].length)

/* mount options */

/* Enable\disable snapshoting*/
#define EMMCFS_MOUNT_SNAPSHOT		0x00000001
#define EMMCFS_MOUNT_CASE_INSENSITIVE	0x00000010
#define EMMCFS_MOUNT_VERCHECK		0x00000100
#define EMMCFS_MOUNT_BTREE_CHECK	0x00001000

#define clear_option(sbi, option)	(sbi->mount_options &= \
						~EMMCFS_MOUNT_##option)
#define set_option(sbi, option)		(sbi->mount_options |= \
						EMMCFS_MOUNT_##option)
#define test_option(sbi, option)	(sbi->mount_options & \
						EMMCFS_MOUNT_##option)



#endif /* _EMMCFS_EMMCFS_H_ */
