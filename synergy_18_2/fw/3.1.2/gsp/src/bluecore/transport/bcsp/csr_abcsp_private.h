#ifndef CSR_ABCSP_PRIVATE_H__
#define CSR_ABCSP_PRIVATE_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_abcsp.h"
#include "csr_abcsp_txrx.h"
#include "csr_abcsp_config_event.h"
#include "csr_abcsp_config_panic.h"
#include "csr_abcsp_config_timer.h"
#include "csr_abcsp_txmsg.h"
#include "csr_abcsp_txslip.h"
#include "csr_abcsp_le.h"
#include "csr_abcsp_crc.h"
#include "csr_abcsp_panics.h"
#include "csr_abcsp_events.h"

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
NAME
        MALLOC  -  allocate a block of memory

SYNOPSIS
        void *MALLOC(CsrUint16 n)

FUNCTION
        As the standard C library function, malloc().

RETURNS
        A block of memory of size "n" bytes, or NULL if it could not be
        obtained.
*/

#define MALLOC(n)       abcsp_malloc(n)


/****************************************************************************
NAME
        NEW  -  allocate a typed block of memory

SYNOPSIS
        void *NEW(t)

FUNCTION
    Macro to allocate a block of memory of type "t" and cast the memory's
    base address to be of type "*t".

RETURNS
        A block of memory of large enough to contain a data element of type
        "t", or NULL if it could not be obtained.
*/

#define NEW(t)          ((t *) (MALLOC(sizeof(t)))


/****************************************************************************
NAME
        DELETE  -  carelessly liberate a block of memory

SYNOPSIS
        void ABCSP_FREE(void *p);

FUNCTION
        As the standard C library function, free().

        The block of memory "p", previously obtained via one of the macros
        defined in this file, is returned to its source.

        This macro wrapping of FREE() allows the caller to be careless
        of the type of the memory returned.
*/

#define ABCSP_FREE(n)            CsrPmemFree(n)


/****************************************************************************
NAME
        DELETE  -  carelessly liberate a block of memory

SYNOPSIS
        void DELETE(void *p);

FUNCTION
        As the standard C library function, free().

        The block of memory "p", previously obtained via one of the macros
        defined in this file, is returned to its source.

        This macro wrapping of FREE() allows the caller to be careless
        of the type of the memory returned.
*/

#define DELETE(p)       (ABCSP_FREE((void *) (p)))


/****************************************************************************
NAME
        abcsp_malloc  -  allocate a block of memory

FUNCTION
        As the standard C library function, malloc().

RETURNS
        A block of memory of size "n" bytes, or NULL if it could not be
        obtained.
*/

void *abcsp_malloc(CsrUint16 n);

#ifdef __cplusplus
}
#endif

#endif
