#ifndef __PVR_EDIT_H_
#define __PVR_EDIT_H_


#if (defined(CONFIG_XFS_FS_TRUNCATE_RANGE) || defined (CONFIG_XFS_FS_SPLIT) \
	|| defined(CONFIG_EXT3_FS_FILE_FUNC) \
	|| defined(CONFIG_EXT3_FS_TRUNCATE_RANGE))\
	|| defined(CONFIG_EXT4_FS_TRUNCATE_RANGE) \
	|| defined(CONFIG_EXT4_FS_SPLIT_FILE) \
	|| defined(CONFIG_EXT4_FS_MERGE_FILE)

typedef struct {
	loff_t offset;
	loff_t end;
        char filename[64];
}PVR_INFO;

typedef struct trange {
	loff_t start_off;
	loff_t end_off;
} trange_t;

typedef struct trange_array {
	int elements;
	trange_t *trange;
} trange_array_t;

#define FTRUNCATERANGE	0x9001
#define FSPLIT		0x9002
#define FMERGE		0x9003
#define FTRUNCATE_ARRAY_RANGE	0x9004
#endif

#endif
