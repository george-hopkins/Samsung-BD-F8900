/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"

#include "platform/csr_confdata.h"
#include "platform/csr_nand.h"

#include "csr_confdata_r.h"

CsrInt32 ConfDataLightweightGetValue(CsrUint16 tag, CsrUint16 length, CsrUint8 *value)
{
    CsrInt32 i;
    CsrUint32 size;
    CsrUint32 block, page;
    CsrUint32 pointer;
    CsrUint16 thisTag;
    CsrUint16 thisLength;

    /* Search for first good and active block */
    for (block = CONFDATA_FIRSTBLOCK; block <= CONFDATA_LASTBLOCK; block++)
    {
        if (!NANDBlockIsBad(block) && (NANDBlockMarkerGet(block) == NAND_MARK_ACTIVE))
        {
            break;
        }
    }
    if (block == CONFDATA_INVALIDBLOCK)
    {
        return CONFDATA_ERROR_READ;
    }

    /* Calculate first page */
    page = block * NAND_PAGESPERBLOCK;

    /* Check integrity of first page of data (CRC) */
    if (NANDPageRead(page, NULL) != NAND_OK)
    {
        return CONFDATA_ERROR_READ;
    }

    /* Determine size of Configuration Data */
    size = NANDRandomRead(page, 0);
    size |= NANDRandomRead(page, 1) << 8;
    size |= NANDRandomRead(page, 2) << 16;
    size |= NANDRandomRead(page, 3) << 24;

    /* Sanity Check size */
    if (size > NAND_BLOCKSIZE)
    {
        return CONFDATA_ERROR_READ;
    }

    /* Check for no content */
    if (size <= 4)
    {
        return CONFDATA_ERROR_NOTFOUND;
    }

    /* Start at first tag */
    pointer = 4;

    while (TRUE)
    {
        thisTag = NANDRandomRead(page, pointer);
        thisTag |= NANDRandomRead(page, pointer + 1) << 8;
        thisLength = NANDRandomRead(page, pointer + 2);
        thisLength |= NANDRandomRead(page, pointer + 3) << 8;

        if (thisTag == tag)
        {
            break;
        }

        /* Skip to next tag */
        pointer += thisLength + 2 + 2;

        /* Wrap at end of page */
        if (pointer >= NAND_PAGESIZE)
        {
            page++;
            pointer %= NAND_PAGESIZE;

            /* Integrity check the page (CRC) */
            if (NANDPageRead(page, NULL) != NAND_OK)
            {
                return CONFDATA_ERROR_READ;
            }
        }

        /* Check if exceeding size */
        if (((page - block * NAND_PAGESPERBLOCK) * NAND_PAGESIZE + pointer) >= size)
        {
            return CONFDATA_ERROR_NOTFOUND;
        }
    }

    /* Found tag */
    pointer += 2 + 2;
    for (i = 0; (i < length) && (i < thisLength); i++)
    {
        *value++ = NANDRandomRead(page, pointer++);

        /* Wrap at end of page */
        if (pointer >= NAND_PAGESIZE)
        {
            page++;
            pointer %= NAND_PAGESIZE;

            /* Integrity check the page (CRC) */
            if (NANDPageRead(page, NULL) != NAND_OK)
            {
                return CONFDATA_ERROR_READ;
            }
        }
    }

    return CONFDATA_OK;
}
