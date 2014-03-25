#ifndef __PMALLOC_H__
#define __PMALLOC_H__
/****************************************************************************

                (c) CSR plc 2007

                All rights reserved

FILE:           pmalloc.h

REVISION:        $Revision: #1 $

DESCRIPTION:    The pmalloc library provides a pool-based heap for dynamic allocation
                of storage. This is simply a familiar interface to a set of fixed
                sized memory pools.

                The library is not a real malloc()-style memory allocator - it does
                not obtain memory from any underlying operating system in the manner
                of sbrk, nor does it perform defragmentation of returned memory.   It
                uses a #defined set pools each holding memory blocks of a given
                #defined size.

                Memory blocks obtained by pmalloc(), and library functions based on
                it, are expected to be returned to the pools with pfree().

                In the style of the streams memory allocator, if a pool becomes
                depleted, memory is taken from the next pool - which will have larger
                memory blocks.

                Unlike a conventional unix malloc(), memory pfree()d is written to
                during the pfree() rather than left unsullied until the next memory
                allocation.

                If a user pmalloc()s a chunk of memory he may subsequently pfree() it
                by calling pfree() with *any* address within the memory block.   This
                might be useful, for example, when walking a pointer up a PDU in a
                pmalloc()ed memory block.

                The library is built as a C library to minimise memory consumption;
                the user may not wish to use some of the less common functions.

                If the number of pools becomes significantly large it may be worth
                adding a hashing function to pmalloc(), so that the correct pool may
                be found more quickly - pretty unlikely, actually.

                No attempt has been made to make this library thread-safe.   It is
                not expected that the library's functions will be called from an
                interrupt routine

                There are memory block addressing concerns.   Some machines insist
                that memory accesses of a given size are accessed at addresses that
                are multiples of that size.   For example, a sparc doesn't like
                addressing a 4 byte memory block on an odd memory address.  This is
                not a concern while all pools' memory blocks are multiples of 4 (?)
                bytes, however this library should be able to allocate 5 byte blocks
                on a machine that is less fussy about its addressing.

                Each pool's blocks must be big enough to hold a pointer.

****************************************************************************/

#include "csr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Allocate memory of a given type (t) from the pool. */
#define pnew(t) ((t *) (pmalloc(sizeof(t))))

/* Allocate memory of a given type (t) from the pool. */
#define xpnew(t)    ((t *)( pmalloc( sizeof(t) )))

/* Allocate zeroed memory of a given type (t) from the pool. */
#define zpnew(t) ((t *) (zpmalloc(sizeof(t))))

/* Allocate zeroed memory of a given type (t) from the pool. */
#define xzpnew(t)    ((t *)( zpmalloc( sizeof(t) )))

/* Return the pmalloc()ed memory block (d) to the pool. */
#define pdelete(d) (pfree((void *) (d)))

/* Allocate zeroed memory of a given type (t) from the pool. */
#define xzpmalloc(s)    (zpmalloc(s))

/*----------------------------------------------------------------------------*
 *  NAME
 *      init_pmalloc
 *
 *  DESCRIPTION
 *      Sets up the pool control blocks and establishes the pools' free
 *      lists.   Use only at the system's initialisation.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void init_pmalloc(void);

/*----------------------------------------------------------------------------*
 *  NAME
 *      pmalloc
 *
 *  DESCRIPTION
 *      Returns a pointer to a block of memory of length "size" bytes obtained
 *      from the pools.
 *
 *      Panic()s on failure.
 *
 *  RETURNS
 *      void * - pointer to allocated block
 *
 *----------------------------------------------------------------------------*/
#ifdef LINUX_KERNEL_MEM_LEAK_DEBUG
extern void *pmalloc_debug(size_t size,char *file, int line);
extern void *pmalloc_kernel_debug(size_t size,char *file, int line);
#define pmalloc(s) pmalloc_debug((s),__FILE__,__LINE__)
#define pmalloc_kernel(s) pmalloc_kernel_debug((s),__FILE__,__LINE__)
#else
extern void *pmalloc(size_t size);
extern void *pmalloc_kernel(size_t size);
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *      pfree
 *
 *  DESCRIPTION
 *      Return a memory block previously obtained via pmalloc to the pools.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
extern void pfree(void *ptr);

/*----------------------------------------------------------------------------*
 *  NAME
 *      psizeof
 *
 *  DESCRIPTION
 *      Obtain the size of a pmallocated memory block.
 *
 *  RETURNS
 *      uint32_t - size of memory block, or zero if block is not valid
 *
 *----------------------------------------------------------------------------*/
extern size_t psizeof(void *ptr);


/*----------------------------------------------------------------------------*
 *  NAME
 *      zpmalloc
 *
 *  DESCRIPTION
 *      Returns a pointer to a block of memory of length "size" bytes obtained
 *      from the pools, and sets it to all zeroes.
 *
 *      Panic()s on failure.
 *
 *  RETURNS
 *      void * - pointer to allocated block
 *
 *----------------------------------------------------------------------------*/
extern void *zpmalloc(size_t size);
extern void deinit_pmalloc(void);

#ifdef __cplusplus
}
#endif

#endif

