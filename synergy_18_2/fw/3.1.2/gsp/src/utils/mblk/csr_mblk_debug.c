/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/*!

\file   mblk_debug.c

\brief  Debugging MBLKs.
*/

#include "csr_mblk_private.h"
#include "csr_panic.h"

#ifdef MBLK_DEBUG

/* Max reference count gets set to 1000 for Synergy and 15 when we're on
   chip. Override by setting CSR_BLUESTACK_MBLK_MAX_REF before this point. */
#ifndef CSR_BLUESTACK_MBLK_MAX_REF
#if 1
#define CSR_BLUESTACK_MBLK_MAX_REF 250
#else /* BUILD_FOR_HOST */
#define CSR_BLUESTACK_MBLK_MAX_REF 15
#endif /* BUILD_FOR_HOST */
#endif /* ndef CSR_BLUESTACK_MBLK_MAX_REF */

/*! \brief Panic if pointer is NULL or MBLK guard field is not valid.

    \param mblk Pointer to MBLK structure.
*/
void mblk_guard_check(const CsrMblk *const mblk)
{
    if ((mblk == NULL) || (mblk->guard != 0xDEAD))
    {
        CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_UNEXPECTED_VALUE, "Panic Mystery");
    }
}

/*! \brief Do guard check and also panic if ref_count is not 1.
           No modifications may be made to an MBLK with duplicates,
           so we do a check here.

    \param mblk Pointer to MBLK structure.
*/
void mblk_guard_single_ref_check(const CsrMblk *const mblk)
{
    MBLK_GUARD_CHECK(mblk);

    /* Make sure there's a single reference (no duplicates). */
    if (mblk->ref_count != 1)
    {
        CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_UNEXPECTED_VALUE, "Panic Mystery");
    }
}

/*! \brief Do guard check and also panic if ref_count is maxed.
    Before adding a new duplicate, make sure we're not at the limit.

    \param mblk Pointer to MBLK structure.
*/
void mblk_guard_max_ref_check(const CsrMblk *const mblk)
{
    MBLK_GUARD_CHECK(mblk);

    /* Check the reference count isn't about to wrap around. */
    if (mblk->ref_count == CSR_BLUESTACK_MBLK_MAX_REF)
    {
        CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_UNEXPECTED_VALUE, "Panic Mystery");
    }
}

#endif
