/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/*!

\file   mblk_pmalloc.c

\brief  MBLKs with data stored in pmalloc blocks.
*/

#include "csr_mblk_private.h"

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_util.h"
#include "csr_panic.h"

static void mblk_pmalloc_destroy(CsrMblk *mblk);
static CsrMblkSize mblk_pmalloc_read_head(CsrMblk *mblk,
    void *data,
    CsrMblkSize *data_size);
static CsrMblkSize mblk_pmalloc_read_tail(CsrMblk *mblk,
    void *data,
    CsrMblkSize *data_size);
static CsrMblkSize mblk_pmalloc_copy_to_memory(const CsrMblk *mblk,
    CsrMblkSize mblk_offset,
    CsrMblkSize mblk_size,
    CsrUint8 *mem_ptr);
#ifndef EXCLUDE_MBLK_BUFFER_SUPPORT
static CsrMblkSize mblk_pmalloc_copy_to_buffer(const CsrMblk *mblk,
    CsrMblkSize mblk_offset,
    CsrMblkSize mblk_size,
    BUFFER *buffer,
    CsrMblkSize buffer_offset);
#endif
static void *mblk_pmalloc_map(const CsrMblk *mblk,
    CsrMblkSize mblk_offset,
    CsrMblkSize length);
static void mblk_pmalloc_unmap(const CsrMblk *mblk,
    void *map);
static CsrMblk *mblk_pmalloc_split(CsrMblk *mblk,
    CsrMblkSize split_offset);
static CsrMblkSize mblk_pmalloc_coalesce(CsrMblk *mblk1,
    CsrMblk **mblk2);

/*! \brief MsgFrag MBLK operator table

    This table contain pointers to functions for the various operations that
    can be performed on a MBLK.
*/
static const CsrMblkVtable vtable =
{
    mblk_pmalloc_destroy,
    mblk_pmalloc_read_head,
    mblk_pmalloc_read_tail,
    mblk_pmalloc_copy_to_memory,
#ifndef EXCLUDE_MBLK_BUFFER_SUPPORT
    mblk_pmalloc_copy_to_buffer,
#endif
    mblk_pmalloc_map,
    mblk_pmalloc_unmap,
    mblk_pmalloc_split,
    mblk_pmalloc_coalesce
};

/*! \brief Create and initialise MBLK with data block

    This function will create and initialise an MBLK for use with data blocks.

    \param free_block TRUE if data block should be freed when MBLK destroyed.
    \param added_size Length of metadata that will be appended.
    \param block_size Size of data block.
    \param block Pointer to data block, or NULL to pmalloc new block.
    \returns Pointer to created MBLK, or NULL on failure.
*/
CsrMblk *CsrMblkPmallocNew(CsrBool free_block,
    CsrUint16 added_size,
    CsrMblkSize block_size,
    void *block)
{
    CsrMblk *mblk;
    void *allocated_block = NULL;

    /* Check to see if we need to allocate for the data block. */
    if (block == NULL)
    {
        /* We must create block. */
        if ((block_size <= 4) && free_block && (added_size == 0))
        {
            /* Datablock can sit in the same pmalloc pool as MBLK. */
            added_size = block_size;
        }
        else
        {
            /* Allocate separate block for data. */
            if ((block = allocated_block = CsrPmemAlloc(block_size)) == NULL)
            {
                return NULL;
            }
        }
    }

    if ((mblk = CsrPmemAlloc(sizeof(CsrMblk) + added_size)) == NULL)
    {
        CsrPmemFree(allocated_block);
        return NULL;
    }

#ifdef MBLK_DEBUG
    mblk->guard = 0xDEAD;
#endif
    mblk->ref_count = 1;
    mblk->vtable = &vtable;
    mblk->next_ptr = NULL;
    mblk->destructor = NULL;

    mblk->u.pmalloc.block = NULL;
    if (free_block)
    {
        mblk->u.pmalloc.block = block;
    }

    mblk->data_size = block_size;

    mblk->u.pmalloc.data = block;
    if (block == NULL)
    {
        mblk->u.pmalloc.data = (CsrUint8 *) (mblk + 1);
    }

    return mblk;
}

/*! \brief Create MBLK from specified data block

    This function attempts to create a MBLK from the specified data block.

    \param block Pointer to data block.
    \param block_size Size of data block.
    \param free_block TRUE if data block should be freed when MBLK destroyed.
    \return Pointer to new MBLK.
*/
CsrMblk *CsrMblkDataCreate(void *block,
    CsrMblkSize block_size,
    CsrBool free_block)
{
    if (block == NULL)
    {
        return NULL;
    }

    return CsrMblkPmallocNew(free_block, 0, block_size, block);
}

/*! \brief Create MBLK with new data block

    This function attempts to create a MBLK with a newly allocated data block.

    \param block_ptr Pointer to data block pointer, set by this function.
    \param block_size Size of data block.
    \return Pointer to new MBLK.
*/
CsrMblk *CsrMblkMallocCreate(void **block_ptr, CsrMblkSize block_size)
{
    CsrMblk *mblk;

    *block_ptr = NULL;
    if ((mblk = CsrMblkPmallocNew(TRUE, 0, block_size, NULL)) != NULL)
    {
        *block_ptr = mblk->u.pmalloc.data;
    }

    return mblk;
}

/*! \brief Destroy MBLK

    This function will destroy the specified MBLK and the associated pmalloc
    block.

    \param mblk Pointer to MBLK.
*/
static void mblk_pmalloc_destroy(CsrMblk *mblk)
{
    /* Decrement reference counter */
    mblk->ref_count--;
    if (mblk->ref_count == 0)
    {
        MBLK_GUARD_CHECK(mblk);
#ifdef MBLK_DEBUG
        mblk->guard = 0xBEEF;
#endif
        CsrPmemFree(mblk->u.pmalloc.block);
        mblk->u.pmalloc.data = mblk->u.pmalloc.block = NULL;
        mblk->data_size = 0;
    }
}

/*! \brief Read from head of MBLK

    This function reads data from the specified MBLK.  Any data read is
    effectively removed from the MBLK.

    \param mblk Pointer to MBLK.
    \param data Pointer buffer to store read data, or NULL to just discard data.
    \param data_size_ref Pointer to amount of data to read, modified to amount actually read.
    \return Amount of data left in MBLK.
*/
static CsrMblkSize mblk_pmalloc_read_head(CsrMblk *mblk, void *data, CsrMblkSize *data_size_ref)
{
    CsrMblkSize data_size = *data_size_ref;

    MBLK_GUARD_SINGLE_REF_CHECK(mblk);

    if (data_size > mblk->data_size)
    {
        data_size = mblk->data_size;
    }

    if (data != NULL)
    {
        CsrMemCpy(data, mblk->u.pmalloc.data, data_size);
    }

    mblk->u.pmalloc.data += data_size;
    mblk->data_size -= data_size;

    *data_size_ref = data_size;
    return mblk->data_size;
}

/*! \brief Read from tail of MBLK

    This function reads data from the specified MBLK.  Any data read is
    effectively removed from the MBLK.

    \param mblk Pointer to MBLK.
    \param data Pointer buffer to store read data, or NULL to just discard data.
    \param data_size_ref Pointer to amount of data to read, modified to amount actually read.
    \return Amount of data left in MBLK.
*/
static CsrMblkSize mblk_pmalloc_read_tail(CsrMblk *mblk, void *data, CsrMblkSize *data_size_ref)
{
    CsrMblkSize data_size = *data_size_ref;

    MBLK_GUARD_SINGLE_REF_CHECK(mblk);

    if (data_size > mblk->data_size)
    {
        data_size = mblk->data_size;
    }

    mblk->data_size -= data_size;

    if (data != NULL)
    {
        CsrMemCpy(data, mblk->u.pmalloc.data + mblk->data_size, data_size);
    }

    *data_size_ref = data_size;
    return mblk->data_size;
}

/*! \brief Copy part of MBLK into memory region

    This function is called whenever an MBLK (or a part of) needs to be copied
    into a memory region.  The caller specifies the region of the MBLK that
    should be copied and the memory region where it should go.  It is assumed
    the caller doesn't specify out of bounds parameters.  For a duplicate MBLK
    we just adjust the offset and pass the request to the original MBLK.

    \param mblk Pointer to MBLK to copy from.
    \param mblk_offset Offset into MBLK.
    \param mblk_size Size of region to copy.
    \param mem_ptr Pointer to destination memory region.
    \return Number of bytes actually copied, in this case either 0 for failure or
            the size requested if successful.
*/
static CsrMblkSize mblk_pmalloc_copy_to_memory(const CsrMblk *mblk,
    CsrMblkSize mblk_offset,
    CsrMblkSize mblk_size,
    CsrUint8 *mem_ptr)
{
    if (mblk->u.pmalloc.data == NULL)
    {
        return 0;
    }

    /* Copy from MBLK into memory region */
    CsrMemCpy(mem_ptr, mblk->u.pmalloc.data + mblk_offset, mblk_size);

    /* Return size copied */
    return mblk_size;
}

#ifndef EXCLUDE_MBLK_BUFFER_SUPPORT
/*! \brief Copy part of MBLK into Buffer

    This function is called whenever an MBLK (or a part of) needs to be copied
    into a Buffer.  The caller specifies the region of the MBLK that should
    be copied and the offset into the Buffer where it should go.  It is assumed
    the caller doesn't specify out of bounds parameters.

    \param mblk Pointer to MBLK to copy from.
    \param mblk_offset Offset into MBLK.
    \param mblk_size Size of region to copy.
    \param buffer Pointer to Buffer.
    \param buffer_offset Offset into Buffer.
    \return Number of bytes actually copied, in this case either 0 for failure or
            the size requested if successful.
*/
static CsrMblkSize mblk_pmalloc_copy_to_buffer(const CsrMblk *mblk,
    CsrMblkSize mblk_offset,
    CsrMblkSize mblk_size,
    BUFFER *buffer,
    CsrMblkSize buffer_offset)
{
    /* Attempt to copy from MBLK into buffer */
    if ((mblk->u.pmalloc.data != NULL)
        && buf_write_msgfrag_at(buffer,
            buffer_offset,
            mblk->u.pmalloc.data + mblk_offset,
            mblk_size))
    {
        /* Return size copied */
        return mblk_size;
    }
    else
    {
        /* Return 0 indicating nothing copied */
        return 0;
    }
}

#endif

/*! \brief Map MBLK

    This function attempt to map the MBLK into a linear buffer, if the length
    requested is greater then the size of the MBLK then the request will fail.

    \param mblk Pointer to MBLK to map.
    \param map_size Length of data to map.
    \return Pointer to linear buffer, or NULL if operation failed.
*/
static void *mblk_pmalloc_map(const CsrMblk *mblk,
    CsrMblkSize mblk_offset,
    CsrMblkSize map_size)
{
    /* Check if we can handle map request */
    if ((mblk->u.pmalloc.data != NULL)
        && (mblk->data_size >= (map_size + mblk_offset)))
    {
        /* Return pointer into block (offset inherent due to pointer) */
        return mblk->u.pmalloc.data + mblk_offset;
    }
    else
    {
        /* This is illegal */
        CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_UNEXPECTED_VALUE, "MBLK map error");
        return NULL;
    }
}

/*! \brief Unmap MBLK

    This function will unmap a MBLK previously mapped with mblk_msgfrag_map().

    \param mblk Pointer to mapped MBLK.
    \param map Pointer to linear buffer.
*/
static void mblk_pmalloc_unmap(const CsrMblk *mblk, void *map)
{
    /* We don't care about this, as it's handled either by the
     * CsrMblkIterateRegion() or CsrMblkMap() */
}

/*! \brief Splits a single pmalloc based MBLK into two

    This function will attempt to split the supplied MBLK into two MBLKs,
    creating a new one in the process.

    \param mblk Pointer to MBLK to split.
    \param data_offset Offset to the start of new required MBLK.
    \return Pointer to new MBLK.
*/
static CsrMblk *mblk_pmalloc_split(CsrMblk *mblk, CsrMblkSize data_offset)
{
    CsrMblk *mblk_new = NULL;
    CsrMblkSize new_mblk_len;

    MBLK_GUARD_SINGLE_REF_CHECK(mblk);

    new_mblk_len = CsrMblkGetLength(mblk) - data_offset;

    if (new_mblk_len)
    {
        void *p_data = CsrPmemAlloc(new_mblk_len);
        (void) CsrMblkCopyToMemory(mblk, data_offset, new_mblk_len, p_data);
        mblk_new = CsrMblkDataCreate(p_data, new_mblk_len, TRUE);
    }

    /* Now reduce size of original mblk accordingly
    */
    if (CsrMblkDiscardTail(&mblk, new_mblk_len) != new_mblk_len)
    {
        /* Something has gone seriously wrong if this happens so panic!
        */
        /* This is illegal */
        CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_UNEXPECTED_VALUE, "MBLK discar tail error");
    }

    if (NULL == mblk_new)
    {
        /* Failed to create new mblk - panic
        */
        CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_UNEXPECTED_VALUE, "MBLK heap exhaustion");
    }

    return mblk_new;
}

/*! \brief Coalesce two pmalloc based MBLKs.

    This function will attempt to coalesce the supplied mblks, with the result
    returned in mblk1.

    NB This function is designed to work with 2 single MBLKs and not chains.

    \param mblk1 Pointer to first MBLK.
    \param mblk2 Pointer to second MBLK.
    \return Size of the coalesced MBLKs if successful, 0 otherwise
*/
static CsrMblkSize mblk_pmalloc_coalesce(CsrMblk *mblk1, CsrMblk **mblk2)
{
    MBLK_GUARD_SINGLE_REF_CHECK(mblk1);
    MBLK_GUARD_SINGLE_REF_CHECK(*mblk2);

    /* First chain the supplied MBLKs then call the general function to
       coalesce an MBLK chain into one pmalloc based MBLK.
    */
    mblk1 = CsrMblkAddTail(*mblk2, mblk1);

    if (CsrMblkCoalesceToPmalloc(&mblk1))
    {
        return CsrMblkGetLength(mblk1);
    }

    /* mblk2 will have been destroyed as part of the coalesce function thus
       just NULLify the callers pointer.
    */
    *mblk2 = NULL;

    return 0;
}
