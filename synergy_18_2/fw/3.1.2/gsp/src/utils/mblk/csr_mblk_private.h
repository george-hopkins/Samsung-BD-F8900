#ifndef CSR_MBLK_PRIVATE_H__
#define CSR_MBLK_PRIVATE_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/*!

\file   mblk_private.h

\brief  This file defines internal interface for the MBLK chain
        manipulation routines.
*/

#include "csr_mblk.h"
#include "csr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

CsrMblk *CsrMblkPmallocNew(CsrBool free_block,
    CsrUint16 added_size,
    CsrMblkSize block_size,
    void *block);
void CsrMblkDestruct(CsrMblk *mblk);

#ifndef EXCLUDE_MBLK_BUFFER_SUPPORT
void mblk_msgfrag_init(CsrMblk *mblk, MSGFRAG *msgfrag);
#endif

#ifdef MBLK_DEBUG

void mblk_guard_check(const CsrMblk *const mblk);
void mblk_guard_single_ref_check(const CsrMblk *const mblk);
void mblk_guard_max_ref_check(const CsrMblk *const mblk);

#ifndef MBLK_GUARD_CHECK
#define MBLK_GUARD_CHECK(mblk) mblk_guard_check((mblk))
#endif

#ifndef MBLK_GUARD_SINGLE_REF_CHECK
#define MBLK_GUARD_SINGLE_REF_CHECK(mblk) mblk_guard_single_ref_check((mblk))
#endif

#ifndef MBLK_GUARD_MAX_REF_CHECK
#define MBLK_GUARD_MAX_REF_CHECK(mblk) mblk_guard_max_ref_check((mblk))
#endif

#else /* MBLK_DEBUG */

#define MBLK_GUARD_CHECK(mblk)
#define MBLK_GUARD_SINGLE_REF_CHECK(mblk)
#define MBLK_GUARD_MAX_REF_CHECK(mblk)

#endif /* MBLK_DEBUG */

#ifdef __cplusplus
}
#endif

#endif
