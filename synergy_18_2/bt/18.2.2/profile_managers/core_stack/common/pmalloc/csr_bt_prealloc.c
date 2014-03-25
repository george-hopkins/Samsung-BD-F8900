/****************************************************************************
FILE
        xprealloc.c  -  resize a block of memory from pool

CONTAINS
        xprealloc  -  resize a block of memory from pool
*/

#include "csr_synergy.h"

#include "csr_bt_core_stack_pmalloc.h"

/****************************************************************************
NAME
    xprealloc  -  resize a block of memory from pool
*/

void *host_xprealloc( void *ptr, size_t old_size, size_t new_size)
{
    void *ret;
    size_t copy_size;

    /*
     * First check if existing block is just the right size - if it is, keep
     * it
     */

    if( ptr != ( void * )( NULL ) && (old_size == new_size ))
    return ptr;

    /* Otherwise, get a new block and copy */

    ret = xpmalloc( new_size );
    if( ret == ( void * )( NULL ))
    return(( void * )( NULL ));

    if( ptr == ( void * )( NULL ))
    return( ret );

    /* CSRMIN(old_size, new_size) */
    copy_size = (( old_size >= new_size ) ? new_size : old_size );

    ( void ) CsrMemCpy( ret, ptr, ( size_t )( copy_size ));

    CsrPmemFree( ptr );

    return( ret );
}
