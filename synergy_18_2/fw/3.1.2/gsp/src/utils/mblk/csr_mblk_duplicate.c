/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/*!

\file   mblk_duplicate.c

\brief  MBLKs that are duplicate of regions of other MBLKS.

        It's possible to create MBLKs that are duplicates of other
        MBLKS, this functionality is required when performing L2CAP
        Segmentation and Reassembly.

        When segmenting an L2CAP packet, instead of creating new MBLKs
        that contain copies of the a portion of the original MBLK, a
        duplicate MBLK is created that references a data region in the
        original MBLK.

        Each MBLK contains a reference count, when a MBLK is
        duplicated the reference coun of the original MBLK will be
        incremented, preventing the MBLK from being destroy unless the
        duplicate MBLK has also been destroyed.

        One limitation of making a duplicate MBLK is that it is then
        no longer possible to modify the original MBLK, attempting to
        do so will cause a panic when MBLK_DEBUG is defined.
*/
#include "csr_mblk_private.h"

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_util.h"

static void mblk_duplicate_destroy(CsrMblk *mblk);
static CsrMblkSize mblk_duplicate_copy_to_memory(const CsrMblk *mblk,
    CsrMblkSize mblk_offset,
    CsrMblkSize mblk_size,
    CsrUint8 *mem_ptr);
static void *mblk_duplicate_map(const CsrMblk *mblk,
    CsrMblkSize mblk_offset,
    CsrMblkSize map_size);
static void mblk_duplicate_unmap(const CsrMblk *mblk,
    void *map);
#ifndef EXCLUDE_MBLK_BUFFER_SUPPORT
static CsrMblkSize mblk_duplicate_copy_to_buffer(const CsrMblk *mblk,
    CsrMblkSize mblk_offset,
    CsrMblkSize mblk_size,
    BUFFER *buffer,
    CsrMblkSize buffer_offset);
#endif

/*! \brief Duplicate MBLK operator table

    This table contain pointers to functions for the various operations that
    can be performed on a MBLK.
*/
static const CsrMblkVtable vtable =
{
    mblk_duplicate_destroy,
    NULL, /* read_head */
    NULL, /* read_tail */
    mblk_duplicate_copy_to_memory,
#ifndef EXCLUDE_MBLK_BUFFER_SUPPORT
    mblk_duplicate_copy_to_buffer,
#endif
    mblk_duplicate_map,
    mblk_duplicate_unmap,
    NULL, /* split */
    NULL  /* Coalesce */
};

/*! \brief Create duplicate MBLK from another MBLK

    This function attempts to create a duplicate MBLK from the specified
    original MBLK.  The duplicate can be just a portion of the original
    MBLK.

    \param dup_mblk Pointer to MBLK that is going to be duplicated.
    \param dup_offset Offset of duplicate region.
    \param dup_size Size of duplicate region.
    \return Pointer to new MBLK.
*/
CsrMblk *CsrMblkDuplicateCreate(CsrMblk *dup_mblk,
    CsrMblkSize dup_offset,
    CsrMblkSize dup_size)
{
    /* Create duplicate MBLK structure */
    CsrMblk *mblk = zpnew(CsrMblk);
    if (mblk != NULL)
    {
#ifdef MBLK_DEBUG
        mblk->guard = 0xDEAD;
#endif
        /* Set initial reference count */
        mblk->ref_count = 1;

        /* Set pointer to operations for duplicate MBLKs */
        mblk->vtable = &vtable;

        MBLK_GUARD_MAX_REF_CHECK(dup_mblk);

        /* Increment reference count on original MBLK */
        dup_mblk->ref_count++;

        /* Create duplicate */
        mblk->u.duplicate.mblk_ptr = dup_mblk;
        mblk->u.duplicate.offset = dup_offset;
        mblk->data_size = dup_size;
    }

    return mblk;
}

/*! \brief Destroy MBLK

    This function will destroy the specified MBLK and the associated pmalloc
    block.

    \param mblk Pointer to MBLK.
*/
static void mblk_duplicate_destroy(CsrMblk *mblk)
{
    MBLK_GUARD_CHECK(mblk);

    /* Decrement reference counter, free MBLK if no more references */
    mblk->ref_count--;
    if (mblk->ref_count == 0)
    {
        /* Destroy original MBLK */
        CsrMblk *dup_mblk = mblk->u.duplicate.mblk_ptr;
        dup_mblk->vtable->destroy(dup_mblk);
        if (dup_mblk->ref_count == 0)
        {
            CsrPmemFree(dup_mblk);
            mblk->u.duplicate.mblk_ptr = NULL;
            mblk->data_size = 0;
        }

#ifdef MBLK_DEBUG
        mblk->guard = 0xBEEF;
#endif
    }
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
static CsrMblkSize mblk_duplicate_copy_to_memory(const CsrMblk *mblk,
    CsrMblkSize mblk_offset,
    CsrMblkSize mblk_size,
    CsrUint8 *mem_ptr)
{
    CsrMblk *mblk_orig;

    MBLK_GUARD_CHECK(mblk);

    if ((mblk_orig = mblk->u.duplicate.mblk_ptr) == NULL)
    {
        return 0;
    }

    mblk_offset += mblk->u.duplicate.offset;

    return mblk_orig->vtable->copy_to_memory(mblk_orig, mblk_offset, mblk_size, mem_ptr);
}

#ifndef EXCLUDE_MBLK_BUFFER_SUPPORT
/*! \brief Copy part of MBLK into Buffer

    This function is called whenever an MBLK (or a part of) needs to be copied
    into a Buffer.  The caller specifies the region of the MBLK that should be
    copied and the offset into the Buffer where it should go.  It is assumed
    the caller doesn't specify out of bounds parameters.  For a duplicate MBLK
    we just adjust the offset and pass the request to the original MBLK.

    \param mblk Pointer to MBLK to copy from.
    \param mblk_offset Offset into MBLK.
    \param mblk_size Size of region to copy.
    \param buffer Pointer to Buffer.
    \param buffer_offset Offset into Buffer.
    \return Number of bytes actually copied, in this case either 0 for failure or
            the size requested if successful.
*/
static CsrMblkSize mblk_duplicate_copy_to_buffer(const CsrMblk *mblk,
    CsrMblkSize mblk_offset,
    CsrMblkSize mblk_size,
    BUFFER *buffer,
    CsrMblkSize buffer_offset)
{
    CsrMblk *mblk_orig;

    MBLK_GUARD_CHECK(mblk);

    if ((mblk_orig = mblk->u.duplicate.mblk_ptr) == NULL)
    {
        return 0;
    }

    mblk_offset += mblk->u.duplicate.offset;

    return mblk_orig->vtable->copy_to_buffer(mblk_orig,
        mblk_offset,
        mblk_size,
        buffer,
        buffer_offset);
}

#endif

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
static void *mblk_duplicate_map(const CsrMblk *mblk,
    CsrMblkSize mblk_offset,
    CsrMblkSize map_size)
{
    CsrMblk *mblk_orig;

    MBLK_GUARD_CHECK(mblk);

    if ((mblk_orig = mblk->u.duplicate.mblk_ptr) == NULL)
    {
        return NULL;
    }

    return mblk_orig->vtable->map(mblk_orig,
        (CsrMblkSize) (mblk_offset +
                       mblk->u.duplicate.offset),
        map_size);
}

/*! \brief Unmap MBLK

    This function will unmap a MBLK previously mapped with mblk_msgfrag_map().

    \param mblk Pointer to mapped MBLK.
    \param map Pointer to linear buffer.
*/
static void mblk_duplicate_unmap(const CsrMblk *mblk,
    void *map)
{
    CsrMblk *mblk_orig;

    MBLK_GUARD_CHECK(mblk);

    /* Defer this to the original MBLK */
    if ((mblk_orig = mblk->u.duplicate.mblk_ptr) != NULL)
    {
        mblk_orig->vtable->unmap(mblk_orig, map);
    }
}
