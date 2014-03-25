#ifndef _ARM_FCNTL_H
#define _ARM_FCNTL_H

#define O_DIRECTORY	 040000	/* must be a directory */
#define O_NOFOLLOW	0100000	/* don't follow links */
#define O_DIRECT	0200000	/* direct disk access hint - currently ignored */
#define O_LARGEFILE	0400000

#ifdef CONFIG_BD_CACHE_ENABLED
#define O_BDCACHE 040000000 
#endif

#include <asm-generic/fcntl.h>

#endif
