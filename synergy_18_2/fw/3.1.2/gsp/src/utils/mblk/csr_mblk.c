/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/*!

\file   mblk.c

\brief  This file implements the MBLK chain manipulation routines.

        MBLKs are data structures used to pass data up and down the
        protocol stack.  Each MBLK can be variable size and multiple
        MBLKs can be linked together to make a MBLK chain.

        The data within a MBLK can be stored in a number of ways, the
        module provides a consistent API so that the caller doesn't
        have to worry about the internal storage of the data.
*/

#include "csr_mblk_private.h"

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_panic.h"
#include "csr_util.h"

/*! \brief Call MBLK destructors and free structure if now unused.

    This function calls destructors and then frees the MBLK structure
    if its reference count is zero. The user destructor is called after
    the class destructor meaning that the data will be discarded just
    before the user destructor is called.

    \param mblk Pointer to MBLK structure.
*/
void CsrMblkDestruct(CsrMblk *mblk)
{
    if (mblk != NULL)
    {
        MBLK_GUARD_CHECK(mblk);

        /* Call class destructor */
        mblk->vtable->destroy(mblk);

        /* Call user specified destructor */
        if (mblk->destructor != NULL)
        {
            mblk->destructor(mblk);
        }

        if (mblk->ref_count == 0)
        {
            CsrPmemFree(mblk);
        }
    }
}

/*! \brief Destroy MBLK chain

    This function destroys the specified MBLK chain.  All memory allocated
    is freed. Any user specified destructor is called just before the MBLK
    structure is freed.

    \param mblk Pointer to MBLK chain to destroy.
*/
void CsrMblkDestroy(CsrMblk *mblk)
{
    while (mblk != NULL)
    {
        CsrMblk *mblk_next = mblk->next_ptr;

        MBLK_GUARD_CHECK(mblk);

        /* Call class destuctor then user destructor then free mblk. */
        CsrMblkDestruct(mblk);

        /* Move to next MBLK in the chain */
        mblk = mblk_next;
    }
}

/*! \brief Read from tail of MBLK chain

    This function reads data from the tail of the specified MBLK chain.  Any
    data read is effectively removed from the MBLK chain.  Any MBLKs in the
    chain which are emptied will be freed automatically.  NOTE: The current
    implementation will only read from the last MBLK in the chain, if the
    requested amount is greater than the size of the last MBLK then the read
    will be truncated.

    \param mblk_ref Pointer to MBLK chain pointer, the chain pointer may be
                    changed.
    \param data Pointer buffer to store read data.
    \param size_left Amount of data to read.
    \return Amount of data actually read.
*/
CsrMblkSize CsrMblkReadTail(CsrMblk **mblk_ref,
    void *data,
    CsrMblkSize size_left)
{
    const CsrMblkSize size_total = size_left;

    while (*mblk_ref != NULL)
    {
        CsrMblk *mblk_ptr;
        CsrMblk **mblk_ptr_ptr = mblk_ref;
        CsrMblkSize size = size_left;
        CsrMblkSize mblk_chain_size;
        CsrMblkSize data_offset;

        /* Find last MBLK in chain list */
        for (mblk_chain_size = 0;
             (mblk_ptr = *mblk_ptr_ptr)->next_ptr != NULL;
             mblk_chain_size += mblk_ptr->data_size, mblk_ptr_ptr = &mblk_ptr->next_ptr)
        {
            /* Nop */
        }

        MBLK_GUARD_SINGLE_REF_CHECK(mblk_ptr);

        /* Check if last MBLK in chain is large enough for read */
        if (size_left <= mblk_ptr->data_size)
        {
            data_offset = 0;
        }
        else
        {
            /* Check if amount to read is greater then total chain sise */
            if (size_left > (mblk_chain_size + mblk_ptr->data_size))
            {
                data_offset = mblk_chain_size;
            }
            else
            {
                data_offset = size_left - mblk_ptr->data_size;
            }
        }

        /* Call class read_tail function, check if no data left in MBLK */
        if (mblk_ptr->vtable->read_tail(mblk_ptr, (CsrUint8 *) data + data_offset, &size) == 0)
        {
            /* Call class destuctor then user destructor then free mblk. */
            CsrMblkDestruct(mblk_ptr);

            /* Clear next pointer in last MBLK */
            *mblk_ptr_ptr = NULL;
        }

        /* Update size left to discard */
        size_left -= size;

        /* Check if we have read all the data we want */
        if (size_left == 0)
        {
            break;
        }
    }

    return size_total - size_left;
}

/*! \brief Discard data from head of MBLK chain

    This function removes data from the head of the specified MBLK chain.  Any
    MBLKs in the chain which are emptied will be freed automatically.

    \param mblk_ref Pointer to MBLK chain pointer, the chain pointer may be
                    changed.
    \param size_left Amount of data to remove.
    \return Amount of data actually discarded.
*/
CsrMblkSize CsrMblkDiscardHead(CsrMblk **mblk_ref,
    CsrMblkSize size_left)
{
    CsrMblk *mblk_ptr = *mblk_ref;
    const CsrMblkSize size_total = size_left;

    while (mblk_ptr != NULL)
    {
        CsrMblkSize size = size_left;
        CsrMblk *mblk_next_ptr = mblk_ptr->next_ptr;

        MBLK_GUARD_SINGLE_REF_CHECK(mblk_ptr);

        /* Call class discard_head function, check if no data left in MBLK */
        if (mblk_ptr->vtable->read_head(mblk_ptr, NULL, &size) == 0)
        {
            /* Call class destuctor then user destructor then free mblk. */
            CsrMblkDestruct(mblk_ptr);

            /* Move to next MBLK in the chain */
            mblk_ptr = mblk_next_ptr;
        }

        /* Update size left to discard */
        size_left -= size;

        /* Check if we have discarded all the data we want */
        if (size_left == 0)
        {
            break;
        }
    }

    *mblk_ref = mblk_ptr;
    return size_total - size_left;
}

/*! \brief Discard data from tail of MBLK chain

    This function removes data from the tail of the specified MBLK chain.  Any
    MBLKs in the chain which are emptied will be freed automatically.

    \param mblk_ref Pointer to MBLK chain pointer, the chain pointer may be
                    changed.
    \param size_left Amount of data to remove.
    \return Amount of data actually discarded.
*/
CsrMblkSize CsrMblkDiscardTail(CsrMblk **mblk_ref,
    CsrMblkSize size_left)
{
    const CsrMblkSize size_total = size_left;

    while (*mblk_ref != NULL)
    {
        CsrMblk *mblk_ptr;
        CsrMblk **mblk_ptr_ptr = mblk_ref;
        CsrMblkSize size = size_left;

        /* Find last MBLK in chain list */
        for ( ; (mblk_ptr = *mblk_ptr_ptr)->next_ptr != NULL;
              mblk_ptr_ptr = &mblk_ptr->next_ptr)
        {
            /* Nop */
        }

        MBLK_GUARD_SINGLE_REF_CHECK(mblk_ptr);

        /* Call class discard_tail function, check if no data left in MBLK */
        if (mblk_ptr->vtable->read_tail(mblk_ptr, NULL, &size) == 0)
        {
            /* Call class destuctor then user destructor then free mblk. */
            CsrMblkDestruct(mblk_ptr);

            /* Clear next pointer in last MBLK */
            *mblk_ptr_ptr = NULL;
        }

        /* Update size left to discard */
        size_left -= size;

        /* Check if we have discarded all the data we want */
        if (size_left == 0)
        {
            break;
        }
    }

    return size_total - size_left;
}

/*! \brief Copy MBLK chain into pmalloc block.

    This function copies a MBLK chain into the specified memory block.

    \param mblk Pointer to MBLK chain to copy.
    \param mblk_offset Offset into MBLK chain.
    \param mblk_size Size of data to copy from chain.
    \param mem_ptr Pointer to memory block to copy into.
    \return Length in bytes of data copied.
*/
CsrMblkSize CsrMblkCopyToMemory(const CsrMblk *mblk,
    CsrMblkSize mblk_offset,
    CsrMblkSize mblk_size,
    CsrUint8 *mem_ptr)
{
    CsrUint8 *mem_copy_ptr = mem_ptr;

    /* Loop along all MBLKS in the chain */
    while (mblk && mblk_size)
    {
        MBLK_GUARD_CHECK(mblk);

        /* Check this block has the data we're interested in */
        if (mblk_offset < mblk->data_size)
        {
            CsrMblkSize CsrMblkCopy_size;

            /* Clip amount if we want more than this block has */
            if (mblk_offset + mblk_size > mblk->data_size)
            {
                CsrMblkCopy_size = mblk->data_size - mblk_offset;
            }
            else
            {
                CsrMblkCopy_size = mblk_size;
            }

            /* Copy data into buffer */
            CsrMblkCopy_size = mblk->vtable->copy_to_memory(mblk,
                mblk_offset,
                CsrMblkCopy_size,
                mem_copy_ptr);

            /* Update memory pointer */
            mem_copy_ptr += CsrMblkCopy_size;

            /* Update size left to copy */
            mblk_size -= CsrMblkCopy_size;

            /* Update MBLK offset */
            mblk_offset = 0;
        }
        else
        {
            mblk_offset -= mblk->data_size;
        }

        /* Move to next MBLK in the chain */
        mblk = mblk->next_ptr;
    }

    return (CsrMblkSize) (mem_copy_ptr - mem_ptr);
}

#ifndef EXCLUDE_MBLK_BUFFER_SUPPORT
/*! \brief Copy MBLK chain into Buffer.

    This function walks down the MBLK chain, copying the data into the
    specified Buffer.

    \param mblk Pointer to MBLK chain to copy.
    \param mblk_offset Offset into MBLK chain.
    \param mblk_size Size of data to copy from chain.
    \param buffer Buffer to copy data into.
    \param buffer_offset Offset into buffer to start copy to.
    \return Length in bytes of data copied to Buffer.
*/
CsrMblkSize CsrMblkCopyToBuffer(const CsrMblk *mblk,
    CsrMblkSize mblk_offset,
    CsrMblkSize mblk_size,
    BUFFER *buffer,
    CsrMblkSize buffer_offset)
{
    CsrMblkSize buffer_copy_offset = buffer_offset;

    /* Loop along all MBLKS in the chain */
    while (mblk && mblk_size)
    {
        MBLK_GUARD_CHECK(mblk);

        /* Check this block has the data we're interested in */
        if (mblk_offset < mblk->data_size)
        {
            CsrMblkSize CsrMblkCopy_size;

            /* Clip amount if we want more than this block has */
            if (mblk_offset + mblk_size > mblk->data_size)
            {
                CsrMblkCopy_size = mblk->data_size - mblk_offset;
            }
            else
            {
                CsrMblkCopy_size = mblk_size;
            }

            /* Copy data into buffer */
            CsrMblkCopy_size = mblk->vtable->copy_to_buffer(mblk,
                mblk_offset,
                CsrMblkCopy_size,
                buffer,
                buffer_copy_offset);

            /* Update buffer offset */
            buffer_copy_offset += CsrMblkCopy_size;

            /* Update size left to copy */
            mblk_size -= CsrMblkCopy_size;

            /* Update MBLK offset */
            mblk_offset = 0;
        }
        else
        {
            mblk_offset -= mblk->data_size;
        }

        /* Move to next MBLK in the chain */
        mblk = mblk->next_ptr;
    }

    return buffer_copy_offset - buffer_offset;
}

#endif

/*! \brief Iterator function for CsrMblkMap()

    In order to map chained MBLKs into a continuous buffer for
    off-chip uses - and to allow the coalesce function to combine
    chained blocks we utilise the CsrMblkIterateRegion() code to copy
    everything into a pmalloc-style memory block. For that to work we
    need an iterator function and a type for the user-data pointer.
*/
typedef struct
{
    CsrMblkSize length;
    CsrMblkSize offset;
    CsrUint8   *target;
} CsrMblkMap_iter_data_t;

static CsrBool CsrMblkMap_iter(const void *buffer,
    CsrMblkSize mblk_offset,
    CsrMblkSize length,
    void *userdata)
{
    CsrMblkMap_iter_data_t *user = (CsrMblkMap_iter_data_t *) userdata;
    CsrMblkSize tocopy;

    /* Limit copy by total and MBLK length */
    tocopy = (length < user->length
              ? length
              : user->length);

    /* Copy stuff into buffer and update counters */
    CsrMemCpy(user->target + user->offset,
        buffer,
        tocopy);
    user->offset += tocopy;
    user->length -= tocopy;

    /* Break iteration if length is reached */
    return user->length != 0;
}

/*! \brief Maps MBLK chain into linear buffer.

    This function returns a pointer to a linear buffer representation of the
    specified MBLK chain, if the MBLK chain constains only one MBLK then
    we can just return a pointer to the data within the MBLK, otherwise we
    have to allocate a tempory buffer.

    \param mblk MBLK chain to map.
    \param map_size Amount of data to map.
    \return Pointer to linear buffer or NULL if mapping failed.
*/
void *CsrMblkMap(const CsrMblk *mblk,
    CsrMblkSize mblk_offset,
    CsrMblkSize map_size)
{
    MBLK_GUARD_CHECK(mblk);

    /* Check if single MBLK in chain */
    if (mblk->next_ptr == NULL)
    {
        /* Return pointer to linear buffer */
        return mblk->vtable->map(mblk, mblk_offset, map_size);
    }
    else
    {
#if 1
        /* On-host, serialise the MBLKs into a continuous buffer */
        CsrMblkMap_iter_data_t user;

        /* Prepare and execute iteration */
        user.length = map_size;
        user.offset = 0;
        user.target = CsrPmemAlloc(map_size);

        CsrMblkIterateRegion(mblk,          /* mblk pointer */
            0,                             /* offset */
            map_size,                      /* total length */
            CsrMblkMap_iter,                 /* iteration function */
            &user);                        /* user data */
        return user.target;
#else
        /* This is illegal on-chip - use CsrMblkIterateRegion()
         * instead */
        CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_UNEXPECTED_VALUE, "MBLK map error");
        return NULL;
#endif
    }
}

/*! \brief Unmaps MBLK

    This function will unmap a linear buffer of the specified MBLK, it should be
    called to free up any resources used by CsrMblkMap().

    \param mblk Pointer to MBLK chain buffer belongs to.
    \param mapped_data Pointer to buffer.
*/
void CsrMblkUnmap(const CsrMblk *mblk, void *mapped_data)
{
    MBLK_GUARD_CHECK(mblk);

    /* Non-chained MBLKs don't use the extra continous buffer, so bail
     * out */
    if (mblk->next_ptr == NULL)
    {
        /* Do nothing */
    }
    else
    {
#if 0
        /* This is illegal on-chip - use CsrMblkIterateRegion()
         * instead */
        CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_UNEXPECTED_VALUE, "MBLK map error");
#else
        /* Unmap the continuous buffer. This is just a pmalloc, so
         * free it */
        CsrPmemFree(mapped_data);
#endif
    }
}

/*! \brief Calculates length of MBLK chain

    This function calculated the length of data in the MBLK chain.

    \param mblk Pointer to MBLK chain.
    \return Length of data in MBLK chain.
*/
CsrMblkSize CsrMblkGetLength(const CsrMblk *mblk)
{
    CsrMblkSize size_total = 0;
    while (mblk)
    {
        MBLK_GUARD_CHECK(mblk);

        size_total += mblk->data_size;
        mblk = mblk->next_ptr;
    }
    return size_total;
}

/*! \brief Set user destructor

    This function sets the user destructor for the specified MBLK, when the
    MBLK is destroyed the user callback function will be called.

    \param mblk Pointer to MBLK.
    \param destructor Callback function.
*/
void CsrMblkSetDestructor(CsrMblk *mblk, void (*destructor)(CsrMblk *))
{
    MBLK_GUARD_CHECK(mblk);

    mblk->destructor = destructor;
}

/*! \brief Modify MBLK reference count

    Modify (increase) the refence count.
*/
void CsrMblkIncRefcount(CsrMblk *mblk, CsrUint8 adjust)
{
    MBLK_GUARD_CHECK(mblk);

    mblk->ref_count += adjust;

    if (mblk->next_ptr != NULL)
    {
        CsrMblkIncRefcount(mblk->next_ptr, adjust);
    }
}

/*! \brief Duplicate MBLK chain

    This function will attempt to duplicate a MBLK chain.

    \param mblk Pointer to MBLK chain to duplicate.
    \param dup_offset Offset of duplicate region.
    \param dup_size Size of duplicate region.
    \return Pointer to duplicate, or NULL if operation failed.
*/
CsrMblk *CsrMblkDuplicateRegion(CsrMblk *mblk, CsrMblkSize dup_offset, CsrMblkSize dup_size)
{
    CsrMblk *mblk_dup = NULL;
    CsrMblk **mblk_dup_ptr = &mblk_dup;
    CsrMblkSize mblk_offset = 0;

    /* Loop along all MBLKS in the chain */
    while (mblk && dup_size)
    {
        MBLK_GUARD_CHECK(mblk);

        /* Check if this MBLK is to be duplicated */
        if ((dup_offset >= mblk_offset) && (dup_offset < mblk_offset + mblk->data_size))
        {
            CsrMblk *mblk_new;
            CsrMblkSize new_offset = dup_offset - mblk_offset;
            CsrMblkSize new_size = (dup_size + new_offset < mblk->data_size
                                    ? dup_size
                                    : mblk->data_size - new_offset);

            mblk_new = CsrMblkDuplicateCreate(mblk,
                (CsrMblkSize) (dup_offset - mblk_offset),
                new_size);
            if (mblk_new != NULL)
            {
                /* Link MBLK to chain */
                *mblk_dup_ptr = mblk_new;
                mblk_dup_ptr = &mblk_new->next_ptr;
            }
            else
            {
                /* Fails to duplicate MBLK, so destroy whole chain */
                CsrMblkDestroy(mblk_dup);
                return NULL;
            }

            dup_size -= new_size;
            dup_offset += new_size;
        }

        /* Move to next MBLK in the chain */
        mblk_offset += mblk->data_size;
        mblk = mblk->next_ptr;
    }

    return mblk_dup;
}

/*! \brief Splits a single MBLK / MBLK chain into two

    This function will attempt to split the supplied MBLK/chain into two,
    creating a new MBLK/chain in the process. It uses the vtable to determine
    which lower level split function to use as the process will be different for
    pmalloc MBLKS compared to msgfrag MBLKS.

    \param mblk - Pointer to MBLK/chain to split.
    \param split_offset - Offset to the start of new required MBLK.
    \return Pointer to new MBLK, or NULL if operation failed.
*/
CsrMblk *CsrMblkSplit(CsrMblk *mblk, CsrMblkSize split_offset)
{
    CsrMblk *mblk_new = NULL;
    CsrUint16 data_offset;
    CsrUint16 mblk_offset = 0;
    CsrUint16 next_blk;

    /* Do some initial sanity checking.
    */
    if ((split_offset == 0) || (split_offset >= CsrMblkGetLength(mblk)))
    {
        return NULL;
    }

    /* Loop along all MBLKS in the chain */
    while (mblk)
    {
        MBLK_GUARD_SINGLE_REF_CHECK(mblk);
        MBLK_GUARD_CHECK(mblk);

        next_blk = mblk_offset + mblk->data_size;

        if (split_offset == next_blk)
        {
            /* Split is on a chain boundary so just split the chain.
            */
            mblk_new = mblk->next_ptr;
            mblk->next_ptr = NULL;
            break;
        }
        /* Check if this MBLK is to be split
        */
        else if ((split_offset > mblk_offset) && (split_offset < next_blk))
        {
            /* Found the MBLK to split, thus need to split the underlying
               message frag/malloc block.
            */
            data_offset = split_offset - mblk_offset;

            mblk_new = mblk->vtable->split(mblk, data_offset);
            break;
        }

        /* Move to next MBLK in the chain */
        mblk_offset += mblk->data_size;
        mblk = mblk->next_ptr;
    }

    return mblk_new;
}

/*! \brief Coalesce 2 MBLKs into 1

    This function will attempt to coalesce the two supplied MBLKs into one, with
    the result stored in mblk1. It uses the vtable to determine which lower
    level coalesce function to use as the process will be different for pmalloc
    MBLKS compared to msgfrag MBLKS.
    If successful the second Mblk will be destroyed during the coalesce.

    NB This function is designed to work with 2 single MBLKs and not chains.

    \param mblk1 - Pointer to MBLK to coalesce to.
    \param mblk2 - Pointer to MBLK to coalesce from.
    \return size - Size of coalesced MBLK if successful, or 0 if failed
*/
CsrMblkSize CsrMblkCoalesce(CsrMblk *mblk1, CsrMblk **mblk2)
{
    if ((NULL != mblk1->next_ptr) || (NULL != (*mblk2)->next_ptr))
    {
        CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_UNEXPECTED_VALUE, CSR_PANIC_FW_UNEXPECTED_VALUE);
    }

    return mblk1->vtable->coalesce(mblk1, mblk2);
}

/*! \brief Join two MBLK chains

    This function appends the 'chain' MBLK to the
    'item' MBLK.

    \param item MBLK pointer of target
    \param chain MBLK pointer to chain to append
    \return Pointer to new MBLK chain
*/
CsrMblk *CsrMblkJoinChain(CsrMblk *item,
    CsrMblk *chain)
{
    CsrMblk *end;

    if (chain == NULL)
    {
        return item;
    }

    MBLK_GUARD_CHECK(chain);

    if (item == NULL)
    {
        return chain;
    }

    MBLK_GUARD_CHECK(item);

    /* Find last item */
    for (end = item; end->next_ptr != NULL; end = end->next_ptr)
    {
        MBLK_GUARD_CHECK(end);
    }

    /* Join chain into item and return it */
    end->next_ptr = chain;
    return item;
}

/*! \brief Iterate through MBLK chain

    This function will iterate though an MBLK chain, mapping in each
    MBLK and passing the mapped data to the specified callback
    function.

    \param mblk Pointer to the MBLK chain
    \param itr_offset Offset into MBLK to iterate through.
    \param itr_size Size of region to iterate through.
    \param itr_func Pointer to iterator function
    \param itr_data Pointer to iterator user data.

    Beware:
    To access the actual data, CsrMblkIterateRegion() will need to map
    in the data. Because the real CsrMblkMap() function may in turn use
    CsrMblkIterateRegion() to deal with chained MBLKs, it's important
    that we use the vtable-map function and calculate the lengths and
    offsets correctly to avoid death by recursion.

    The parameters passed to the itr_func are the data pointer,
    current data size and finally the user data.
*/
void CsrMblkIterateRegion(const CsrMblk *mblk,
    CsrMblkSize itr_offset,
    CsrMblkSize itr_size,
    CsrBool (*itr_func)(const void *ifct_buffer,
        CsrMblkSize ifct_offset,
        CsrMblkSize ifct_length,
        void *ifct_data),
    void *itr_data)
{
    CsrMblkSize mblk_offset = 0;

    /* Loop along all MBLKS in the chain */
    while (mblk && itr_size)
    {
        MBLK_GUARD_CHECK(mblk);

        /* Check if this MBLK is to be iterated through */
        if ((itr_offset >= mblk_offset) &&
            (itr_offset < mblk_offset + mblk->data_size))
        {
            /* Calulate offset and size into MBLK */
            CsrMblkSize new_offset = itr_offset - mblk_offset;
            CsrMblkSize new_size = ((itr_size + new_offset < mblk->data_size)
                                    ? itr_size
                                    : mblk->data_size - new_offset);

            /* Call iterator function with mapped data */
            if (!itr_func((const CsrUint8 *) (mblk->vtable->map(mblk,
                                                  0,
                                                  mblk->data_size)) + new_offset,
                    0,       /* offset not used here */
                    new_size,
                    itr_data))
            {
                break;
            }

            /* Update size and offset to iterate through */
            itr_size -= new_size;
            itr_offset += new_size;
        }

        /* Move to next MBLK in the chain */
        mblk_offset += mblk->data_size;
        mblk = mblk->next_ptr;
    }
}

/*! \brief Coalesce MBLK

    Coalesce a chained MBLK into a single pmalloc style MBLK, freeing
    the old chain in the process. On-chip this function will also
    transform a msgfrag to a pmalloc MBLK. We use the MBLK iterator
    framework to traverse the chained MBLK. Also, because we can't
    really tell whether an MBLK is a duplicate, a msgfrag or a pmalloc
    block we *must* perform the copy operation even for non-chained
    pmalloc blocks.

    The return result will be TRUE on success, in which case the *mblk
    pointer will be changed to point to the new coalesced MBLK.  If
    something goes wrong the result will be FALSE, and the input will
    be left untouched.
*/
CsrBool CsrMblkCoalesceToPmalloc(CsrMblk **mblk)
{
    CsrMblkMap_iter_data_t user;
    CsrMblk *result;
    CsrMblkSize size;
    MBLK_GUARD_CHECK(*mblk);

    /* Prepare and execute iteration */
    size = CsrMblkGetLength(*mblk);
    user.length = size;
    user.offset = 0;
    user.target = CsrPmemAlloc(user.length);

    if (user.target != NULL)
    {
        /* Iterate chains using user-function that copies stuff into
         * the pmalloc blob */
        CsrMblkIterateRegion(*mblk,         /* mblk pointer */
            0,                             /* offset */
            user.length,                   /* total length */
            CsrMblkMap_iter,                 /* iteration function */
            &user);                        /* user data */

        /* Encapsulate pmalloc in an MBLK */
        result = CsrMblkDataCreate(user.target,
            size,
            TRUE);
        if (result != NULL)
        {
            /* New pmalloc block wrapped and ready. Destroy the old
             * one and return the new one */
            CsrMblkDestroy(*mblk);
            *mblk = result;
            return TRUE;
        }

        /* Couldn't create new MBLK - free the pmalloc buffer */
        CsrPmemFree(user.target);
    }

    /* Something went wrong */
    return FALSE;
}

/*! \brief Make a deep copy of a MBLK

    A few legacy applications need to perform deep copys
    of MBLKs. This function does exactly that. However, you
    really should use the mblk_duplicate() instead!

    \param mblk MBLK to copy
    \returns pointer to copy of the MBLK
*/
#if 1
CsrMblk *CsrMblkCopy(CsrMblk *mblk)
{
    CsrMblk *mblkCopy = NULL;

    if (mblk)
    {
        CsrMblkSize len;

        MBLK_GUARD_CHECK(mblk);
        len = CsrMblkGetLength(mblk);

        if (len)
        {
            void *p_data = CsrPmemAlloc(len);
            (void) CsrMblkCopyToMemory(mblk, 0, len, p_data);
            mblkCopy = CsrMblkDataCreate(p_data, len, TRUE);
        }
    }

    return mblkCopy;
}

#endif
