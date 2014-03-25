/*
 * vdashmem.h
 *
 * Copyright 2008 Google Inc.
 * Author: Robert Love
 *
 * This file is dual licensed.  It may be redistributed and/or modified
 * under the terms of the Apache 2.0 License OR version 2 of the GNU
 * General Public License.
 */

#ifndef _LINUX_VDASHMEM_H
#define _LINUX_VDASHMEM_H

#include <linux/limits.h>
#include <linux/ioctl.h>

#define VDASHMEM_NAME_LEN		256

#define VDASHMEM_NAME_DEF		"dev/vdashmem"

/* Return values from VDASHMEM_PIN: Was the mapping purged while unpinned? */
#define VDASHMEM_NOT_PURGED	0
#define VDASHMEM_WAS_PURGED	1

/* Return values from VDASHMEM_GET_PIN_STATUS: Is the mapping pinned? */
#define VDASHMEM_IS_UNPINNED	0
#define VDASHMEM_IS_PINNED	1

struct vdashmem_pin {
	__u32 offset;	/* offset into region, in bytes, page-aligned */
	__u32 len;	/* length forward from offset, in bytes, page-aligned */
};

#define __VDASHMEMIOC		0x77

#define VDASHMEM_SET_NAME		_IOW(__VDASHMEMIOC, 1, char[VDASHMEM_NAME_LEN])
#define VDASHMEM_GET_NAME		_IOR(__VDASHMEMIOC, 2, char[VDASHMEM_NAME_LEN])
#define VDASHMEM_SET_SIZE		_IOW(__VDASHMEMIOC, 3, size_t)
#define VDASHMEM_GET_SIZE		_IO(__VDASHMEMIOC, 4)
#define VDASHMEM_SET_PROT_MASK	_IOW(__VDASHMEMIOC, 5, unsigned long)
#define VDASHMEM_GET_PROT_MASK	_IO(__VDASHMEMIOC, 6)
#define VDASHMEM_PIN		_IOW(__VDASHMEMIOC, 7, struct vdashmem_pin)
#define VDASHMEM_UNPIN		_IOW(__VDASHMEMIOC, 8, struct vdashmem_pin)
#define VDASHMEM_GET_PIN_STATUS	_IO(__VDASHMEMIOC, 9)
#define VDASHMEM_PURGE_ALL_CACHES	_IO(__VDASHMEMIOC, 10)

#endif	/* _LINUX_VDASHMEM_H */
