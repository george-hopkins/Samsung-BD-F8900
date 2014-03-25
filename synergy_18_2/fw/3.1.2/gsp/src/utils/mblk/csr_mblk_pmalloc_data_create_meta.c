/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/*!

\file   mblk_pmalloc_data_create_meta.c

\brief  Create MBLK from the specified data block.
*/

#include "csr_mblk_private.h"

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_util.h"

#if 1
/*! \brief Create MBLK from specified data block with meta data.

    This function attempts to create a MBLK from the specified data block.

    \param block Pointer to data block.
    \param block_size Size of data block.
    \param free_block TRUE if data block should be freed when MBLK destroyed.
    \param metadata Pointer to metadata to copy to MBLK.
    \param metasize Size of metadata.
    \return Pointer to new MBLK.
*/
CsrMblk *CsrMblkDataCreateMeta(void *block,
    CsrMblkSize block_size,
    CsrBool free_block,
    const void *metadata,
    CsrUint16 metasize)
{
    CsrMblk *mblk;

    if ((block == NULL) ||
        ((mblk = CsrMblkPmallocNew(free_block, metasize, block_size, block)) == NULL))
    {
        return NULL;
    }

    CsrMemCpy(CsrMblkGetMetadata(mblk), metadata, metasize);
    return mblk;
}

#endif
