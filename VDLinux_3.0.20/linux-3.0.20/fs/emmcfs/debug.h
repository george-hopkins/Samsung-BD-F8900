/**
 * @file	fs/emmcfs/debug.h
 * @brief	The eMMCFS kernel debug support.
 * @date	01/17/2012
 *
 * eMMCFS -- Samsung eMMC chip oriented File System, Version 1.
 *
 * This files defines debug tools.
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

#ifndef _EMMCFS_DEBUG_H_
#define _EMMCFS_DEBUG_H_

#include <linux/crc32.h>

/**
 * @brief		Memory dump.
 * @param [in] 	type	Sets debug type (see EMMCFS_DBG_*).
 * @param [in] 	buf	Pointer to memory.
 * @param [in] 	len	Byte count in the dump.
 * @return	void
 */
#define EMMCFS_MDUMP(type, buf, len)\
	do {\
		if ((type) & debug_mask) {\
			EMMCFS_DEBUG(type, "");\
			print_hex_dump(KERN_INFO, "emmcfs ",\
					DUMP_PREFIX_ADDRESS, 16, 1, buf, len,\
					true);\
		} \
	} while (0)

/**
 * @brief		Print error message to kernel ring buffer.
 * @param [in]	fmt	Printf format string.
 * @return	void
 */
#define EMMCFS_ERR(fmt, ...)\
	do {\
		printk(KERN_ERR "emmcfs-ERROR:%d:%s: " fmt, __LINE__,\
			__FUNCTION__, ##__VA_ARGS__);\
	} while (0)

/** Enables EMMCFS_DEBUG_SB() in super.c */
#define EMMCFS_DBG_SB	(1 << 0)

/** Enables EMMCFS_DEBUG_INO() in inode.c */
#define EMMCFS_DBG_INO	(1 << 1)

/** Enables EMMCFS_DEBUG_INO() in fsm.c, fsm_btree.c */
#define EMMCFS_DBG_FSM	(1 << 2)

/** Enables EMMCFS_DEBUG_SNAPSHOT() in snapshot.c */
#define EMMCFS_DBG_SNAPSHOT	(1 << 3)

/** Enables EMMCFS_DEBUG_MUTEX() driver-wide */
#define EMMCFS_DBG_MUTEX	(1 << 4)

/** Enables EMMCFS_DEBUG_TRN() driver-wide. Auxilary, you can remove this, if
 * there is no more free debug_mask bits */
#define EMMCFS_DBG_TRANSACTION (1 << 5)

/* Non-permanent debug */
#define EMMCFS_DBG_TMP (1<<6)


#if defined(CONFIG_EMMCFS_DEBUG)
/**
 * @brief		Print debug information.
 * @param [in] 	type	Sets debug type (see EMMCFS_DBG_*).
 * @param [in] 	fmt	Printf format string.
 * @return	void
 */
#define EMMCFS_DEBUG(type, fmt, ...)\
	do {\
		if ((type) & debug_mask)\
			printk(KERN_INFO "%s:%d:%s: " fmt "\n", __FILE__,\
				__LINE__, __FUNCTION__, ##__VA_ARGS__);\
	} while (0)
#else
#define EMMCFS_DEBUG(type, fmt, ...) do {} while (0)
#endif

/**
 * @brief		Print debug information in super.c.
 * @param [in] 	fmt	Printf format string.
 * @return	void
 */
#define EMMCFS_DEBUG_SB(fmt, ...) EMMCFS_DEBUG(EMMCFS_DBG_SB, fmt,\
						##__VA_ARGS__)

/**
 * @brief		Print debug information in inode.c.
 * @param [in] 	fmt	Printf format string.
 * @return	void
 */
#define EMMCFS_DEBUG_INO(fmt, ...) EMMCFS_DEBUG(EMMCFS_DBG_INO, fmt,\
						##__VA_ARGS__)

/**
 * @brief		Print debug information in fsm.c, fsm_btree.c.
 * @param [in] 	fmt	Printf format string.
 * @return	void
 */
#define EMMCFS_DEBUG_FSM(fmt, ...) EMMCFS_DEBUG(EMMCFS_DBG_FSM, fmt,\
						##__VA_ARGS__)

/**
 * @brief		Print debug information in snapshot.c.
 * @param [in] 	fmt	Printf format string.
 * @return	void
 */
#define EMMCFS_DEBUG_SNAPSHOT(fmt, ...) EMMCFS_DEBUG(EMMCFS_DBG_SNAPSHOT, fmt,\
						##__VA_ARGS__)

/**
 * @brief		TODO Print debug information in ...
 * @param [in] 	fmt	Printf format string.
 * @return	void
 */
#define EMMCFS_DEBUG_MUTEX(fmt, ...) EMMCFS_DEBUG(EMMCFS_DBG_MUTEX, fmt,\
						##__VA_ARGS__)

/**
 * @brief		Print debug information with pid.
 * @param [in] 	fmt	Printf format string.
 * @return	void
 */
#define EMMCFS_DEBUG_TRN(fmt, ...) EMMCFS_DEBUG(EMMCFS_DBG_TRANSACTION,\
				"pid=%d " fmt,\
				((struct task_struct *) current)->pid,\
				##__VA_ARGS__)

/**
 * @brief		Print non-permanent debug information.
 * @param [in] 	fmt	Printf format string.
 * @return	void
 */
#define EMMCFS_DEBUG_TMP(fmt, ...) EMMCFS_DEBUG(EMMCFS_DBG_TMP,\
				"pid=%d " fmt,\
				((struct task_struct *) current)->pid,\
				##__VA_ARGS__)

extern unsigned int debug_mask;

#if defined(CONFIG_EMMCFS_DEBUG)
void emmcfs_debug_print_sb(struct emmcfs_sb_info *sbi);
#else
static inline void emmcfs_debug_print_sb(struct emmcfs_sb_info *sbi) {}
#endif

#ifdef CONFIG_EMMCFS_PROC_INFO

/**
 * @brief		Adds function start entry to the log buffer.
 * @param [in] 	sbi	Super block info where log buffer is.
 * @return	void
 */
#define EMMCFS_LOG_FUNCTION_START(sbi)\
	emmcfs_function_add(sbi->buffer, __func__, 1, 0)
/**
 * @brief		Adds function exit entry to the log buffer.
 * @param [in] 	sbi	Super block info where log buffer is.
 * @return	void
 */
#define EMMCFS_LOG_FUNCTION_END(sbi, ret)\
	emmcfs_function_add(sbi->buffer, __func__, -1, ret)
#else

#define EMMCFS_LOG_FUNCTION_START(sbi)
#define EMMCFS_LOG_FUNCTION_END(sbi, ret)

#endif

#endif /* _EMMCFS_DEBUG_H_ */
