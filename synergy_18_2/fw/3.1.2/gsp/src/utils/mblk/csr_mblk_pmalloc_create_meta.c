/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/*!

\file   mblk_pmalloc_create_meta.c

\brief  Create MBLK with a newly allocated data block.
*/

#include "csr_mblk_private.h"

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_util.h"

#if 1
/*! \brief Create MBLK with new data block and meta-data

    This function attempts to create a MBLK with a newly allocated data block.

    \param block_ptr Pointer to data block pointer, set by this function.
    \param block_size Size of data block.
    \param metadata Pointer to metadata to copy to MBLK.
    \param metasize Size of metadata.
    \return Pointer to new MBLK.
*/
CsrMblk *CsrMblkMallocCreateMeta(void **block_ptr,
    CsrMblkSize block_size,
    const void *metadata,
    CsrUint16 metasize)
{
    CsrMblk *mblk;

    *block_ptr = NULL;
    if ((mblk = CsrMblkPmallocNew(TRUE, metasize, block_size, NULL)) != NULL)
    {
        *block_ptr = mblk->u.pmalloc.data;
        CsrMemCpy(CsrMblkGetMetadata(mblk), metadata, metasize);
    }

    return mblk;
}

#endif
