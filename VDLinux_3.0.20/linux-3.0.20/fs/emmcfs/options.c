/**
 * @file	fs/emmcfs/options.c
 * @brief	The eMMCFS mount options parsing routines.
 * @author	Dmitry Voytik, d.voytik@samsung.com
 * @date	01/19/2012
 *
 * eMMCFS -- Samsung eMMC chip oriented File System, Version 1.
 *
 * This file implements eMMCFS parsing options
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

#include <linux/string.h>
#include <linux/parser.h>
#include "emmcfs.h"
#include "debug.h"

/**
 * The eMMCFS mount options.
 */
enum {
	option_nojournal,
	option_novercheck,
	option_btreecheck,
	option_readonly,
	option_count,
	option_error
};

/**
 * The eMMCFS mount options match tokens.
 */
static const match_table_t tokens = {
	{option_nojournal, "nojournal"},
	{option_novercheck, "novercheck"},
	{option_btreecheck, "btreecheck"},
	{option_readonly, "ro"},
	{option_count, "count=%u"},
	{option_error, NULL},
};

/**
 * @brief		Parse eMMCFS options.
 * @param [in] 	sb	VFS super block
 * @param [in] 	input	Options string for parsing
 * @return		Returns 0 on success, errno on failure
 */
int emmcfs_parse_options(struct super_block *sb, char *input)
{
	int ret = 0;
	int token;
	substring_t args[MAX_OPT_ARGS];
	char *p;
	int option;

	set_option(EMMCFS_SB(sb), SNAPSHOT);
	set_option(EMMCFS_SB(sb), VERCHECK);

	if (!input)
		return 0;

	while ((p = strsep(&input, ",")) != NULL) {
		if (!*p)
			continue;

		token = match_token(p, tokens, args);
		switch (token) {
		case option_nojournal:
			clear_option(EMMCFS_SB(sb), SNAPSHOT);
			EMMCFS_DEBUG_SNAPSHOT("journal is disable, option:%d",
					EMMCFS_SB(sb)->mount_options);
			break;

		case option_novercheck:
			clear_option(EMMCFS_SB(sb), VERCHECK);
			printk(KERN_WARNING "[EMMCFS-warning] Checking versions"
					" of driver and mkfs is disabled\n");

			break;

		case option_btreecheck:
			set_option(EMMCFS_SB(sb), BTREE_CHECK);
			break;

		case option_readonly:
			EMMCFS_SET_READONLY(sb);

		case option_count:
			if (match_int(&args[0], &option)) {
				BUG();
			}
			EMMCFS_DEBUG_TMP("counter %d", option);
			EMMCFS_SB(sb)->bugon_count = option;
			break;

		default:
			return -EINVAL;
		}
	}

	EMMCFS_DEBUG_SB("finished (ret = %d)", ret);
	return ret;
}
