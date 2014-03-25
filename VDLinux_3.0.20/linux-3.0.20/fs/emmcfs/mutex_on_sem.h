/**
 * @file	fs/emmcfs/mutex_on_sem.h
 * @brief	Basic mutex operations.
 * @date	04/17/2012
 *
 * eMMCFS -- Samsung eMMC File System, Version 1.
 *
 * This file implements inode operations and its related functions.
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

#ifndef MUTEX_ON_SEM_H_
#define MUTEX_ON_SEM_H_

#define RW_MUTEX
#ifdef RW_MUTEX
#include <linux/rwsem.h>

#define rw_mutex_t struct rw_semaphore

/**
 * @brief		Locks mutex for reading.
 * @param [in] 	mutex	Mutex that will be locked.
 * @return	void
 */
#define mutex_r_lock(mutex) down_read(mutex)

/**
 * @brief		Locks mutex for writing.
 * @param [in] 	mutex	Mutex that will be locked.
 * @return	void
 */
#define mutex_w_lock(mutex) down_write(mutex)

/**
 * @brief		Unlocks mutex for reading.
 * @param [in] 	mutex	Mutex that will be unlocked.
 * @return	void
 */
#define mutex_r_unlock(mutex) up_read(mutex)

/**
 * @brief		Unlocks mutex for writing.
 * @param [in] 	mutex	Mutex that will be unlocked.
 * @return	void
 */
#define mutex_w_unlock(mutex) up_write(mutex)

/**
 * @brief		Inits mutex.
 * @param [in] 	mutex	Mutex that will be inited.
 * @return	void
 */
#define init_mutex(mutex) init_rwsem(mutex)

#else

#include <linux/mutex.h>
#define rw_mutex_t struct mutex

/**
 * @brief		Locks mutex for reading.
 * @param [in] 	mutex	Mutex that will be locked.
 * @return	void
 */
#define mutex_r_lock(mutex) mutex_lock(mutex)

/**
 * @brief		Locks mutex for writing.
 * @param [in] 	mutex	Mutex that will be locked.
 * @return	void
 */
#define mutex_w_lock(mutex) mutex_lock(mutex)

/**
 * @brief		Unlocks mutex for reading.
 * @param [in] 	mutex	Mutex that will be unlocked.
 * @return	void
 */
#define mutex_r_unlock(mutex) mutex_unlock(mutex)

/**
 * @brief		Unlocks mutex for writing.
 * @param [in] 	mutex	Mutex that will be unlocked.
 * @return	void
 */
#define mutex_w_unlock(mutex) mutex_unlock(mutex)

/**
 * @brief		Inits mutex.
 * @param [in] 	mutex	Mutex that will be inited.
 * @return	void
 */
#define init_mutex(mutex) mutex_init(mutex)
#endif

#endif /* MUTEX_ON_SEM_H_ */
