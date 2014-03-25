/**
 * @file	emmcfs_fs.h
 * @brief	Internal constants and data structures for eMMCFS
 * @date	01/11/2012
 *
 * eMMCFS -- Samsung eMMC chip oriented File System, Version 1.
 *
 * This file defines all important eMMCFS data structures and constants
 *
 * @see		SRC-SEP-11012-HLD001_-eMMCFS-File-System.doc
 *
 * Copyright 2011 by Samsung Electronics, Inc.,
 *
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

#ifndef _LINUX_EMMCFS_FS_H
#define _LINUX_EMMCFS_FS_H

/*
 * The eMMCFS filesystem constants/structures
 */

/** Maximum length of name string of a file/directory */
#define EMMCFS_FILE_NAME_LEN		255
/** Maximum length of full path */
#define EMMCFS_FULL_PATH_LEN		1023

/* Magic Numbers.
 * It is recommended to move magic numbers to include/linux/magic.h in release
 */
#define EMMCFS_SB_SIGNATURE	0x5346434D4D6531ULL	/* "1eMMCFS" */
#define EMMCFS_SB_VER_MAJOR	1
#define EMMCFS_SB_VER_MINOR	0

#define EMMCFS_CAT_FOLDER_MAGIC			"rDe"
#define EMMCFS_CAT_FILE_MAGIC			"eFr"
#define EMMCFS_SNAPSHOT_MAGIC			0x65534e50	/* "eSNP" */
#define EMMCFS_FORK_MAGIC			0x46
#define EMMCFS_NODE_DESCR_MAGIC			0x644E		/*"dN"*/

#define EMMCFS_BTREE_HEAD_NODE_MAGIC		"eHND"
#define EMMCFS_BTREE_LEAF_NODE_MAGIC		"NDlf"
#define EMMCFS_BTREE_INDEX_NODE_MAGIC		"NDin"

#define EMMCFS_EXTTREE_KEY_MAGIC		"ExtK"

/* Flags definition for emmcfs_unicode_string */

/* maximum length of unicode string */
#define EMMCFS_UNICODE_STRING_MAX_LEN 255

/** 
 * @brief 	The struct emmcfs_unicode_string is used to keep and represent
 *		any Unicode string on eMMCFS. It is used for file and folder
 *		names representation and for symlink case.
 */
struct emmcfs_unicode_string {
	/** The string's length */
	__le32 length;
	/** The chain of the Unicode symbols */
	u8 unicode_str[EMMCFS_UNICODE_STRING_MAX_LEN];
} __packed;

/** 
 * @brief 	The eMMCFS stores dates in unsigned 64-bit integer seconds and
 * 		unsigned 32-bit integer nanoseconds.
 */
struct emmcfs_date {
	/** The seconds part of the date */
	__le64 seconds;
	/** The nanoseconds part of the date */
	__le32 nanoseconds;
} __packed;


#define DATE_RESOLUTION_IN_NANOSECONDS_ENABLED 1

/** 
 * @brief 	For each file and folder, eMMCFS maintains a record containing
 *		access permissions.
 */
struct emmcfs_posix_permissions {
	/* File mode        16 |11 8|7  4|3  0| */
	/*                     |_rwx|_rwx|_rwx| */
	__le16 file_mode;			/** File mode */
	__le32 uid;				/** User ID */
	__le32 gid;				/** Group ID */
} __packed;

/** Permissions value for user read-write operations allow. */
#define EMMCFS_PERMISSIONS_DEFAULT_RW  0x666
/** Permissions value for user read-write-execute operations allow. */
#define EMMCFS_PERMISSIONS_DEFAULT_RWX 0x777

/* Flags definition for emmcfs_extent structure */
/** Extent describes information by means of continuous set of LEBs */
#define EMMCFS_LEB_EXTENT	0x80
/** Extent describes binary patch which should be apply inside of block or LEB
 * on the offset in bytes from begin */
#define EMMCFS_BYTE_PATCH	0x20
/** The information fragment is pre-allocated but not really written yet */
#define EMMCFS_PRE_ALLOCATED	0x10

/** 
 * @brief 	On eMMCFS volume all existed data are described by means of
 *		extents.
 */
struct emmcfs_extent {
	/** The start block or LEB index on the eMMCFS volume. */
	__le64	begin;
	__le32	length; /** Length in LEBs or blocks count */
} __packed;

/* Flags definition for emmcfs_fork structure */

/** The fork contains extents tree */
#define EMMCFS_EXTENTS_FORK		0x80
/** The fork contains inline symlink */
#define EMMCFS_INLINE_SYMLINK		0x40


/** eMMCFS maintains information about the contents of a file using the
 * emmcfs_fork structure.
 */
#define EMMCFS_EXTENTS_COUNT_IN_FORK		9
/**
 * @brief 	The eMMCFS fork structure.
 */
struct emmcfs_fork {
	/** magic */
	__u8	magic;			/* 0x46 – F */
	/** The size in bytes of the valid data in the fork */
	__le64			size_in_bytes;
	/** The total number of allocation blocks which is
	 * allocated for file system object under last actual
	 * snapshot in this fork */
	__le32			total_blocks_count;
	/** The set of extents which describe file system
	 * object's blocks placement */
	struct emmcfs_extent	extents[EMMCFS_EXTENTS_COUNT_IN_FORK];
} __packed;


/* Snapshots Management */

/**
 * @brief 	The eMMCFS transaction.
 */
struct emmcfs_transaction {
	/** Original sectors location  */
	__le64 on_volume;
	/** Location in snapshot */
	__le64 on_snapshot;
	/** Block count in transaction */
	__le16 page_count;
} __packed;

/**
 * @brief 	The eMMCFS snapshot descriptor.
 */
struct emmcfs_snapshot_descriptor {
	/** Signature magic */
	__u8 signature[4];				/* 0x65534e50 eSNP */
	/* mount version */
	__le32 mount_count;
	/* snapshot version count */
	__le32 version;
	/** Count of valid records in Root Snapshots Table */
	__le16 transaction_count;
	/** The offset from the end of this structure to place of CRC32
	 * checksum */
	__le16 checksum_offset;
	/** First transaction in snapshot */
	struct emmcfs_transaction first_transaction;

} __packed;

/** 
 * @brief	The version of structures and file system at whole
 *		is tracked by.
 */
struct emmcfs_version {
	/** Major version number */
	__u8 major:4;
	/** Minor version number */
	__u8 minor:4;
} __packed;

/** 
 * @brief 	The eMMCFS superblock.
 */
struct emmcfs_super_block {
	/** magic */
	__u8	signature[7];		/* 0x5346434D4D6531 – SFCMMe1 */
	/** The version of eMMCFS filesystem */
	struct emmcfs_version	version;
	/** log2 (Block size in bytes) */
	__u8	log_block_size;
	/** log2 (LEB size in bytes) */
	__u8	log_leb_size;

	/** Lotal lebs count */
	__le64	total_leb_count;

	/** Total volume encodings */
	__le64	volume_encodings;

	/** Creation timestamp */
	struct emmcfs_date	creation_timestamp;

	/** 128-bit uuid for volume */
	__u8	volume_uuid[16];
	/** Volume name */
	char	volume_name[16];

	/** --- leb bitmap parameters --- */

	/** TODO */
	__u8	lebs_bm_padding[2];
	/** Log2 for blocks described in one block */
	__le32	lebs_bm_log_blocks_block;
	/** Number of blocks in leb bitmap */
	__le32	lebs_bm_blocks_count;
	/** Number of blocks described in last block */
	__le32	lebs_bm_bits_in_last_block;

	/** File driver git repo branch name */
	char mkfs_git_branch[64];
	/** File driver git repo revision hash */
	char mkfs_git_hash[40];

	/** Total volume sectors count */
	__le64	sectors_per_volume;

	/** Case insensitive mode */
	__u8 case_insensitive;
	/** Padding */
	__u8	reserved[311];
	/** Checksum */
	__le32	checksum;
} __packed;

/** 
 * @brief 	The eMMCFS extended superblock.
 */
struct emmcfs_extended_super_block {
	/** Block bitmap fork */
	struct emmcfs_fork	lebs_bitmap_fork;
	/** Snapshot fork */
	struct emmcfs_fork	snapshot_fork;
	/** Catalog tree fork */
	struct emmcfs_fork	catalog_tree_fork;
	/** Extents overflow tree fork */
	struct emmcfs_fork	extents_overflow_tree_fork;
	/** File system files count */
	__le64			files_count;
	/** File system folder count */
	__le64			folders_count;
	/** Extent describing the volume */
	struct emmcfs_extent 	volume_body;
	/** The inode number allocation fork */
	struct emmcfs_fork	inode_num_bitmap_fork;
	/** Hard links file fork */
	struct emmcfs_fork	hlinks_fork;
	/** Number of mount operations */
	__le32			mount_counter;
	/** Number of umount operations */
	__le32			umount_counter;
	
	struct emmcfs_extent 	debug_area;
	/** Reserved */
	__u8			reserved[1270];
	/** Extended superblock checksum */
	__le32			checksum;
} __packed;


#define EMMCFS_CATALOG_FOLDER_RECORD		0x0100
#define EMMCFS_CATALOG_FILE_RECORD		0x0200
#define EMMCFS_CATALOG_HLINK_RECORD		0x0400
#define EMMCFS_CATALOG_FILE_RECORD_SYMLINK	0x0001
#define EMMCFS_CATALOG_FILE_RECORD_FIFO		0x0002
#define EMMCFS_CATALOG_FILE_RECORD_SOCKET	0x0004
#define EMMCFS_CATALOG_FILE_RECORD_CHR		0x0008
#define EMMCFS_CATALOG_FILE_RECORD_BLK		0x0010

/**
 * @brief 	On-disk structure to hold generic for all the trees.
 */
struct emmcfs_generic_key {
	/** Unique number that identifies structure */
	__u8 magic[4];
	/** Length of tree-specific key */
	__le32 key_len;
	/** Full length of record containing the key */
	__le32 record_len;
} __packed;

/**
 * @brief 	On-disk structure to catalog tree keys.
 */
struct emmcfs_cattree_key {
	/** Generic key part */
	struct emmcfs_generic_key gen_key;
	/** File type record describes */
	__le16 record_type;
	/** Object id of parent object (directory) */
	__le64 parent_id;
	/** Object's name */
	struct emmcfs_unicode_string name;
} __packed;

/**
 * @brief 	On-disk structure to hold hardlink records.
 */
struct emmcfs_catalog_hlink_record {
	/** Record type hardlink - EMMCFS_CATALOG_HLINK_RECORD */
	__le16	record_type;
	/** Id of hardlink within hardlinks area */
	__le64 hlink_id;
} __packed;

/**
 * @brief 	On-disk structure to hold file and folder records.
 */
struct emmcfs_catalog_folder_record {
	/* TODO - to rename in some way */
	/**
	 * Type of record - file/folder
	 * EMMCFS_CATALOG_FILE_RECORD/EMMCFS_CATALOG_FOLDER_RECORD
	 * */
	__le16	record_type;
	/** Some flags */
	__le32	flags;
	/* TODO - for file total_items_count has no sense, for folder
	 * links_count has no sense too. Can these fields be merged and
	 * interpreted according to record_type ?
	 */
	/** Amount of files in the directory */
	__le64	total_items_count;
	/** Link's count for file */
	__le64	links_count;
	/** Object id - unique id within filesystem */
	__le64  object_id;
	/** Permissions of record */
	struct emmcfs_posix_permissions	permissions;
	/** Record creation time */
	struct emmcfs_date	creation_time;
	/** Record modification time */
	struct emmcfs_date	modification_time;
	/** Record last access time */
	struct emmcfs_date	access_time;
} __packed;

/**
 * @brief 	On-disk structure to hold file records.
 */
struct emmcfs_catalog_file_record {
	/** Common part of record (file or folder) */
	struct emmcfs_catalog_folder_record common;
	/** Fork containing info about area occupied by file */
	struct emmcfs_fork	data_fork;
} __packed;

#endif	/* _LINUX_EMMCFS_FS_H */
