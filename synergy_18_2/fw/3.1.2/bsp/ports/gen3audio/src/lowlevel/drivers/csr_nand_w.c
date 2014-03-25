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

#define NAND_LOCK()                 BIT_SET(rNFCONT, 2)
#define NAND_UNLOCK()               BIT_CLEAR(rNFCONT, 2)
#define NAND_LOCK_TIGHT()           BIT_SET(rNFCONT, 3)
#define NAND_IS_LOCKED_TIGHT()      BIT_CHECK(rNFCONT, 3)
#define NAND_IS_LOCKED()            BIT_CHECK(rNFCONT, 2)

/* Set the lock state and return the old lock state */
CsrUint8 NANDLockSet(CsrUint8 lockState)
{
    CsrUint8 oldState;

    if (NAND_IS_LOCKED_TIGHT())
    {
        oldState = NAND_LOCKED_TIGHT;
    }
    else if (NAND_IS_LOCKED())
    {
        oldState = NAND_LOCKED;
    }
    else
    {
        oldState = NAND_UNLOCKED;
    }

    switch (lockState)
    {
        case NAND_UNLOCKED:
            NAND_UNLOCK();
            break;
        case NAND_LOCKED:
            NAND_LOCK();
            break;
        case NAND_LOCKED_TIGHT:
            NAND_LOCK_TIGHT();
            break;
        default:
            break;
    }

    return oldState;
}

/* Write one byte to flash
    Return values:
    NAND_ERROR_BADBLOCK = The block is bad
    NAND_OK = Data sucessfully written */
static CsrInt32 randomWrite(CsrUint32 page, CsrUint32 column, CsrUint8 data, CsrUint8 ignoreBadBlocks)
{
    COMMAND(CMD_WRITE);

    FULL_ADDRESS(column, page);

    /* tADL > 100ns */
    DelayUs(1);

    DATA_WRITE(data);

    COMMAND(CMD_WRITE >> 8);

    NAND_READY_WAIT();
    NAND_READY_CLEAR();

    COMMAND(CMD_READ_STATUS);

    /* tWRH > 60ns */
    DelayUs(1);

    data = DATA_READ();

    /* Check if the write failed */
    if (!ignoreBadBlocks && (data & 0x1))
    {
        /* Mark the block as bad */
        NANDBlockMarkBad(page / NAND_PAGESPERBLOCK);
        return NAND_ERROR_BADBLOCK;
    }

    return NAND_OK;
}

/* Write one byte to flash
    Return values:
    NAND_ERROR_BADBLOCK = The block is bad
    NAND_OK = Data sucessfully written */
CsrInt32 NANDRandomWrite(CsrUint32 page, CsrUint32 column, CsrUint8 data)
{
    return randomWrite(page, column, data, FALSE);
}

/* Mark a block as bad
    Return values:
    NAND_ERROR_OUT_OF_RANGE = Out of range
    NAND_OK = Block marked as bad */
CsrInt32 NANDBlockMarkBad(CsrUint32 block)
{
    if (block >= NAND_BLOCKS)
    {
        return NAND_ERROR_OUT_OF_RANGE;
    }

    randomWrite(block * NAND_PAGESPERBLOCK, NAND_PAGESIZE, 0, TRUE);
    randomWrite(block * NAND_PAGESPERBLOCK + 1, NAND_PAGESIZE, 0, TRUE);

    return NAND_OK;
}

/* Erase a specific block
    Return values:
    NAND_ERROR_BADBLOCK = The block is bad
    NAND_ERROR_OUT_OF_RANGE = Out of range
    NAND_OK = Block successfully erased */
CsrInt32 NANDBlockErase(CsrUint32 block)
{
    CsrUint32 page;
    CsrUint8 data;

    /* Check if out of range */
    if (block >= NAND_BLOCKS)
    {
        return NAND_ERROR_OUT_OF_RANGE;
    }

    /* Check if block is marked as bad */
    if (NANDBlockIsBad(block))
    {
        return NAND_ERROR_BADBLOCK;
    }

    page = block * NAND_PAGESPERBLOCK;

    COMMAND(CMD_ERASE_BLOCK);

    ADDRESS(page);
    ADDRESS(page >> 8);

    COMMAND(CMD_ERASE_BLOCK >> 8);

    NAND_READY_WAIT();
    NAND_READY_CLEAR();

    COMMAND(CMD_READ_STATUS);

    /* tWHR > 60ns */
    DelayUs(1);

    data = DATA_READ();

    /* Check if erase failed */
    if (data & 0x1)
    {
        /* Mark the block as bad */
        NANDBlockMarkBad(block);
        return NAND_ERROR_BADBLOCK;
    }

    return NAND_OK;
}

/* Write one full page to flash
    Return values:
    NAND_ERROR_BADBLOCK = The block is bad
    NAND_ERROR_OUT_OF_RANGE = Out of range
    NAND_OK = Data sucessfully written */
CsrInt32 NANDPageWrite(CsrUint32 page, CsrUint8 *buffer)
{
    CsrInt32 i;
    CsrUint8 data;

    /* Check if out of range */
    if (page >= NAND_PAGES)
    {
        return NAND_ERROR_OUT_OF_RANGE;
    }

    /* Check if block is marked as bad */
    if (NANDBlockIsBad(page / NAND_PAGESPERBLOCK))
    {
        return NAND_ERROR_BADBLOCK;
    }

    ECC_RESET();            /* Initialize ECC decoder/encoder */
    ECC_MAIN_UNLOCK();      /* Unlock Main data area ECC generation */

    COMMAND(CMD_WRITE);
    FULL_ADDRESS(0, page);

    /* tADL > 100ns */
    DelayUs(1);

    for (i = 0; i < NAND_PAGESIZE; i++)
    {
        DATA_WRITE(*buffer++);
    }

    ECC_MAIN_LOCK()         /* Lock main data area ECC generation */

    DATA_WRITE(0xFF); /* Good block marker (only applicable for first or second page in block) */
    DATA_WRITE(rNFMECC0 & 0xFF);            /* ECC0 */
    DATA_WRITE((rNFMECC0 >> 8) & 0xFF);     /* ECC1 */
    DATA_WRITE((rNFMECC0 >> 16) & 0xFF);    /* ECC2 */
    DATA_WRITE((rNFMECC0 >> 24) & 0xFF);    /* ECC3 */

    COMMAND(CMD_WRITE >> 8);

    NAND_READY_WAIT();
    NAND_READY_CLEAR();

    COMMAND(CMD_READ_STATUS);

    /* tWRH > 60ns */
    DelayUs(1);

    data = DATA_READ();

    /* Check if the write failed */
    if (data & 0x1)
    {
        /* Mark the block as bad */
        NANDBlockMarkBad(page / NAND_PAGESPERBLOCK);
        return NAND_ERROR_BADBLOCK;
    }

    return NAND_OK;
}

/* Set the marker of a block
    Return values:
    NAND_ERROR_BADBLOCK = Block is bad
    NAND_ERROR_OUT_OF_RANGE = Out of range
    NAND_OK = Block marked */
CsrInt32 NANDBlockMarkerSet(CsrUint32 block, CsrUint8 mark)
{
    CsrUint32 page;

    if (block >= NAND_BLOCKS)
    {
        return NAND_ERROR_OUT_OF_RANGE;
    }

    page = block * NAND_PAGESPERBLOCK + 2;

    return randomWrite(page, NAND_PAGESIZE, mark, FALSE);
}

/* Write a given number of pages to NAND Flash
    - Will use any number of blocks from startBlock up to and including limitBlock
    - Skips bad blocks and blocks that are not marked as empty
    - Assumes blocks are completely erased if marked as empty
    Return values:
    NAND_ERROR_OUT_OF_RANGE = Exceeded limitBlock or physical number of blocks
    NAND_OK = Data successfully read */
CsrInt32 NANDWrite(CsrUint32 startBlock, CsrUint32 limitBlock, CsrUint32 pages, CsrUint8 *data)
{
    CsrInt32 i;
    CsrUint32 block;
    CsrUint32 page;
    CsrUint8 *dataptr;
    CsrInt32 badBlock;
    CsrInt32 pagec;

    block = startBlock;

    while (pages)
    {
        /* Check if block is within limits */
        if ((block > NAND_BLOCKS) || (block > limitBlock))
        {
            return NAND_ERROR_OUT_OF_RANGE;
        }

        /* Calculate destination page */
        page = block * NAND_PAGESPERBLOCK;

        /* Check state (bad or in use) and mark for write */
        if (NANDBlockIsBad(block) ||
            (NANDBlockMarkerGet(block) != NAND_MARK_EMPTY) ||
            NANDBlockMarkerSet(block, NAND_MARK_WRITING))
        {
            /* Block is bad or in use - try next */
            block++;
            continue;
        }

        dataptr = data;
        badBlock = 0;
        pagec = 0;

        for (i = 0; (i < NAND_PAGESPERBLOCK) && (pages - pagec); i++)
        {
            if (NANDPageWrite(page++, dataptr))
            {
                /* Block is bad - try next */
                badBlock = 1;
                break;
            }
            else
            {
                dataptr += NAND_PAGESIZE;
                pagec++;
            }
        }

        if (!badBlock)
        {
            /* Mark the block as active */
            if (!NANDBlockMarkerSet(block, NAND_MARK_ACTIVE))
            {
                pages -= pagec;
                data = dataptr;
            }
        }

        /* Advance to next block */
        block++;
    }

    return NAND_OK;
}
