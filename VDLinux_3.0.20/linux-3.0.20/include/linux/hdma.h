#ifndef __LINUX_HDMA_H
#define __LINUX_HDMA_H

/*
 * Hardware Dedicated Memory Allocator
 * Copyright (c) 2011 by Samsung Electronics.
 * Written by Vasily Leonenko (v.leonenko@samsung.com)
 */

#include <linux/ioctl.h>
#include <linux/cma.h>

#define IOCTL_HDMA_NOTIFY_ALL   _IOWR('p', 0, unsigned long)
#define IOCTL_HDMA_NOTIFY       _IOWR('p', 1, unsigned long)

extern struct cmainfo hdmainfo;

void hdma_regions_reserve(void);

#endif
