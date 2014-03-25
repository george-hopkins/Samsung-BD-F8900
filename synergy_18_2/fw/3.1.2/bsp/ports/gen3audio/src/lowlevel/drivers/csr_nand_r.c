/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "platform/csr_s3c24a0.h"
#include "platform/csr_delay.h"
#include "platform/csr_nand.h"
#include "csr_nand_r.h"

/* Read ID from flash */
void NANDIdentifierRead(CsrUint8 id[5])
{
    COMMAND(CMD_READ_ID);
    ADDRESS(0);

    /* tWHR > 60ns */
    DelayUs(1);

    id[0] = DATA_READ();
    id[1] = DATA_READ();
    id[2] = DATA_READ();
    id[3] = DATA_READ();
    id[4] = 0;
}

/* Read one byte from flash */
CsrUint8 NANDRandomRead(CsrUint32 page, CsrUint32 column)
{
    CsrUint8 data;

    COMMAND(CMD_READ);

    FULL_ADDRESS(column, page);

    COMMAND(CMD_READ >> 8);

    NAND_READY_WAIT();
    NAND_READY_CLEAR();

    /* tRR > 20ns */
    DelayUs(1);

    data = DATA_READ();

    return data;
}

/* Determine if a specific block is marked as bad
    Return values:
    NAND_ERROR_BADBLOCK = Block is bad
    NAND_ERROR_OUT_OF_RANGE = Out of range
    NAND_OK = Block is good */
CsrInt32 NANDBlockIsBad(CsrUint32 block)
{
    if (block >= NAND_BLOCKS)
    {
        return NAND_ERROR_OUT_OF_RANGE;
    }

    if ((NANDRandomRead(block * NAND_PAGESPERBLOCK, NAND_PAGESIZE) == 0xFF) &&
        (NANDRandomRead(block * NAND_PAGESPERBLOCK + 1, NAND_PAGESIZE) == 0xFF))
    {
        return NAND_OK;
    }
    else
    {
        return NAND_ERROR_BADBLOCK;
    }
}

/* Read one full page from flash
    Return values:
    NAND_ERROR_DATACORRUPTION = Unrecoverable error (data corruption)
    NAND_ERROR_OUT_OF_RANGE = Out of range
    NAND_OK = Data successfully read */
CsrInt32 NANDPageRead(CsrUint32 page, CsrUint8 *buffer)
{
    CsrInt32 i;
    CsrUint8 *ptr;
    CsrUint8 data;

    /* Check if out of range */
    if (page >= NAND_PAGES)
    {
        return NAND_ERROR_OUT_OF_RANGE;
    }

    ECC_RESET();        /* Initialise ECC decoder/encoder */
    ECC_MAIN_UNLOCK();  /* Unlock Main data area ECC generation */

    COMMAND(CMD_READ);

    FULL_ADDRESS(0, page);

    COMMAND(CMD_READ >> 8);

    NAND_READY_WAIT();
    NAND_READY_CLEAR();

    /* tRR > 20ns */
    DelayUs(1);

    /* Read out the entire page to the buffer */
    ptr = buffer;
    if (ptr != NULL)
    {
        for (i = 0; i < NAND_PAGESIZE; i++)
        {
            *ptr++ = DATA_READ();
        }
    }
    else
    {
        for (i = 0; i < NAND_PAGESIZE; i++)
        {
            data = DATA_READ();
        }
    }

    ECC_MAIN_LOCK();    /* Lock main data area ECC generation */

    data = DATA_READ();    /* Dummy read cycle to skip the bad block marker */

    rNFMECCDATA0 = DATA_READ();
    rNFMECCDATA1 = DATA_READ();
    rNFMECCDATA2 = DATA_READ();
    rNFMECCDATA3 = DATA_READ();

    data = rNFESTAT0 & 0x3;

    switch (data)
    {
        case 1:
        {
            /* 1-bit error */
            if (buffer != NULL)
            {
                BIT_TOGGLE(buffer[ECC_ERROR_BYTE()], ECC_ERROR_BIT());
            }
            else
            {
                return NAND_ERROR_DATACORRUPTION;
            }
        }
        case 0:
            /* No error */
            return NAND_OK;
        case 3:
        /* ECC area error */
        case 2:
        /* Unrecoverable multi-bit error */
        default:
            return NAND_ERROR_DATACORRUPTION;
    }
}

/* Return the marker of a block */
CsrUint8 NANDBlockMarkerGet(CsrUint32 block)
{
    CsrUint32 page;

    if (block >= NAND_BLOCKS)
    {
        return NAND_MARK_EMPTY;
    }

    page = block * NAND_PAGESPERBLOCK + 2;

    return NANDRandomRead(page, NAND_PAGESIZE);
}

/* Read a given number of pages starting from startBlock - skips non-active or bad blocks
    Return values:
    NAND_ERROR_OUT_OF_RANGE = Exceeded limitBlock or physical number of blocks
    NAND_ERROR_DATACORRUPTION = Unrecoverable error (data corruption)
    NAND_OK = Data successfully read */
CsrInt32 NANDRead(CsrUint32 startBlock, CsrUint32 limitBlock, CsrUint32 pages, CsrUint8 *data)
{
    CsrInt32 i;
    CsrUint32 block;
    CsrUint32 page;

    block = startBlock;

    while (pages)
    {
        /* Check if the block is within limits */
        if ((block > NAND_BLOCKS) || (block > limitBlock))
        {
            return NAND_ERROR_OUT_OF_RANGE;
        }

        /* Calculate source page */
        page = block * NAND_PAGESPERBLOCK;

        if (!NANDBlockIsBad(block) && (NANDBlockMarkerGet(block) == NAND_MARK_ACTIVE))
        {
            /* Read in pages of current block */
            for (i = 0; (i < NAND_PAGESPERBLOCK) && pages; i++)
            {
                if (NANDPageRead(page++, data))
                {
                    /* Read error */
                    return NAND_ERROR_DATACORRUPTION;
                }
                data += NAND_PAGESIZE;
                pages--;
            }
        }

        /* Advance to next block */
        block++;
    }

    return NAND_OK;
}
