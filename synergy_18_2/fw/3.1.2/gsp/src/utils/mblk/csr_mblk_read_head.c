/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/*!

\file   CsrMblkReadHead.c

\brief  Read and remove data from head of MBLK.

*/

#include "csr_mblk_private.h"

#include "csr_types.h"

/*! \brief Read from head of MBLK chain

    This function reads data from the head of the specified MBLK chain.  Any
    data read is effectively removed from the MBLK chain.  Any MBLKs in the
    chain which are emptied will be freed automatically.

    \param mblk_ref Pointer to MBLK chain pointer, the chain pointer may be
                    changed.
    \param data Pointer buffer to store read data.
    \param size_left Amount of data to read.
    \return Amount of data actually read.
*/
CsrMblkSize CsrMblkReadHead(CsrMblk **mblk_ref,
    void *data,
    CsrMblkSize size_left)
{
    CsrMblk *mblk_ptr = *mblk_ref;
    const CsrMblkSize size_total = size_left;
    CsrUint8 *data_ptr = (CsrUint8 *) data;

    while (mblk_ptr != NULL)
    {
        CsrMblkSize size = size_left;
        CsrMblk *mblk_next_ptr = mblk_ptr->next_ptr;

        MBLK_GUARD_SINGLE_REF_CHECK(mblk_ptr);

        /* Call class read function */
        if (mblk_ptr->vtable->read_head(mblk_ptr, data_ptr, &size) == 0)
        {
            /* Call class destuctor then user destructor then free mblk. */
            CsrMblkDestruct(mblk_ptr);

            /* Move to next MBLK in the chain */
            mblk_ptr = mblk_next_ptr;
        }

        /* Update total size read */
        data_ptr += size;
        size_left -= size;

        /* Check if we read all the data we want */
        if (size_left == 0)
        {
            break;
        }
    }

    *mblk_ref = mblk_ptr;
    return size_total - size_left;
}
