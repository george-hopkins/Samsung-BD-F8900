/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_pmem.h"
#include "platform/csr_confdata.h"
#include "platform/csr_nand.h"
#include "csr_confdata_r.h"

/*  Write the Configuration Data to the NAND flash */
CsrInt32 ConfDataWrite(CsrUint8 *data)
{
    CsrInt32 block;
    CsrInt32 currentBlock;
    CsrInt32 dataSize;
    CsrInt32 pages;
    CsrInt32 lockState;

    dataSize = *((CsrInt32 *) data);

    /* Check if data fits within a single block */
    if (dataSize > NAND_BLOCKSIZE)
    {
        return CONFDATA_ERROR_OVERSIZE;
    }

    /* Calculate number of pages required */
    pages = NAND_CALCULATEPAGES(dataSize);

    /* Find the current active block (if any) */
    currentBlock = ConfDataFindActiveBlock();

    lockState = NANDLockSet(NAND_UNLOCKED);

    /* Erase the other non-empty blocks to make room for writing */
    for (block = CONFDATA_FIRSTBLOCK; block <= CONFDATA_LASTBLOCK; block++)
    {
        if ((block != currentBlock) || (currentBlock == CONFDATA_INVALIDBLOCK))
        {
            if (NANDBlockMarkerGet(block) != NAND_MARK_EMPTY)
            {
                NANDBlockErase(block);
            }
        }
    }

    if (NANDWrite(CONFDATA_FIRSTBLOCK, CONFDATA_LASTBLOCK, pages, data))
    {
        NANDLockSet(lockState);
        return CONFDATA_ERROR_WRITE;
    }

    /* Erase the previous active block if it exists (will be marked as bad if failed) */
    if (currentBlock != CONFDATA_INVALIDBLOCK)
    {
        NANDBlockErase(currentBlock);
    }

    NANDLockSet(lockState);
    return CONFDATA_OK;
}

CsrInt32 ConfDataSetValue(CsrUint8 **data, CsrUint16 tag, CsrUint16 length, CsrUint8 *value)
{
    CsrUint8 *dataptr = NULL;
    CsrUint32 dataSize;
    CsrUint32 newTag = 0;

    if ((dataSize = *((CsrUint32 *) *data)) >= 8)
    {
        /* Look for existing entry */
        dataptr = *data + 4;
        while ((dataptr[0] + (dataptr[1] << 8)) != tag)
        {
            /* Read length of current value and skip it */
            dataptr += dataptr[2] + (dataptr[3] << 8) + 2 + 2;

            if ((dataptr >= *data + dataSize))
            {
                newTag = 1;
                break;
            }
        }
    }

    if ((dataSize < 8) || newTag) /* Tag does not exist in set */
    {
        if (!length)
        {
            /* Trying to remove non-existing field */
            return CONFDATA_OK;
        }

        dataptr = CsrPmemAlloc(dataSize + length + 2 + 2); /* Make room for new tag */
        memcpy(dataptr, *data, dataSize); /* copy existing data */
        CsrPmemFree(*data); /* free existing data */
        *data = dataptr; /* assign data */
        *((CsrUint32 *) dataptr) = dataSize + length + 2 + 2; /* write new size */
        dataptr += dataSize; /* Skip to end of existing data */
    }
    else /* Tag exists in set - dataptr points to it */
    {
        CsrUint16 oldLength;
        oldLength = dataptr[2] + (dataptr[3] << 8);
        if (length == oldLength) /* Length is the same - direct replace possible */
        {
            memcpy(dataptr + 2 + 2, value, length);
            return CONFDATA_OK;
        }
        else /* Length differs - reallocate */
        {
            CsrUint8 *ptr;
            CsrUint8 *p1 = dataptr + oldLength + 2 + 2; /* right after old entry */
            CsrUint8 *p2 = *data + dataSize; /* end of data */
            CsrUint32 newSize = (dataptr - *data) + (p2 - p1); /* size of data to preserve */

            if (length)
            {
                /* Add size of new item, tag and length field */
                newSize += length + 2 + 2;
            }

            ptr = CsrPmemAlloc(newSize);
            *((CsrUint32 *) ptr) = newSize; /* write new size */

            /* copy existing data (before and after wiped entry) */
            if (dataptr - (*data + 4))
            {
                memcpy(ptr + 4, *data + 4, (dataptr - (*data + 4))); /* Part before tag */
            }
            if (p2 - p1)
            {
                memcpy(ptr + (dataptr - *data), p1, p2 - p1); /* Part after tag */
            }

            dataptr = ptr + (dataptr - *data) + (p2 - p1); /* Point to new entry*/
            CsrPmemFree(*data); /* free old data */
            *data = ptr; /* assign data */
        }
    }

    /* Write new entry to entry pointed at by dataptr */
    if (length)
    {
        dataptr[0] = tag & 0xFF; /* write new tag */
        dataptr[1] = (tag >> 8) & 0xFF;
        dataptr[2] = length & 0xFF;
        dataptr[3] = (length >> 8) & 0xFF;
        memcpy(dataptr + 4, value, length); /* write new value */
    }
    return CONFDATA_OK;
}
