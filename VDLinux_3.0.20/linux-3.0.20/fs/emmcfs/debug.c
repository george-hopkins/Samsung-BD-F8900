/**
 * @file	fs/emmcfs/debug.c
 * @brief	eMMCFS kernel debug support
 * @author	TODO
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

#include "emmcfs.h"
#include "debug.h"

/**
 * @brief		Prints super block parameters
 * @param [in] sbi	Pointer to super block info data structure
 * @return		void
 */
#if defined(CONFIG_EMMCFS_DEBUG)
void emmcfs_debug_print_sb(struct emmcfs_sb_info *sbi)
{
	EMMCFS_LOG_FUNCTION_START(sbi);
	EMMCFS_DEBUG_SB("\nbytes in block = %u\n"\
			"blocks in leb = %u\ntotal lebs count = %llu\n"
			"free blocks count = %llu\n"
			"files count = %llu",
			sbi->block_size, EMMCFS_BLOCKS_IN_LEB(sbi),
			sbi->total_leb_count, sbi->free_blocks_count,
			sbi->files_count);
	EMMCFS_LOG_FUNCTION_END(sbi, 0);
}
#else
/* inline void emmcfs_debug_print_sb(struct emmcfs_sb_info *sbi) {} */
#endif

