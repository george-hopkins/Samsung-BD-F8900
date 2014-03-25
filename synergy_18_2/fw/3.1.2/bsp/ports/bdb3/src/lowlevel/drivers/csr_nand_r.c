/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"

#include "platform/csr_s3c6410.h"
#include "platform/csr_delay.h"
#include "platform/csr_nand.h"

#include "csr_nand_r.h"

/* Read ID from flash */
void NANDIdentifierRead(CsrUint8 id[5])
{
    CHIP_ENABLE_ATOMIC();

    COMMAND(CMD_READ_ID);
    ADDRESS(0);

    /* tWHR > 60ns */
    DelayUs(1);

    id[0] = DATA_READ8();
    id[1] = DATA_READ8();
    id[2] = DATA_READ8();
    id[3] = DATA_READ8();
    id[4] = DATA_READ8();

    CHIP_DISABLE_ATOMIC();
}

/* Read one byte from flash */
CsrUint8 NANDRandomRead(CsrUint32 page, CsrUint32 column)
{
    CsrUint8 data;

    RNB_DETECT_CLEAR();

    CHIP_ENABLE_ATOMIC();

    COMMAND(CMD_READ);

    FULL_ADDRESS(column, page);

    COMMAND(CMD_READ >> 8);

    CHIP_DISABLE_ATOMIC();

    while (!RNB_DETECT())
    {
    }

    /* tRR > 20ns */
    DelayUs(1);

    CHIP_ENABLE_ATOMIC();

    data = DATA_READ8();

    CHIP_DISABLE_ATOMIC();

    return data;
}

/* Determine if a specific block is marked as bad
    Return values:
    NAND_ERROR_BADBLOCK = Block is bad
    NAND_ERROR_OUT_OF_RANGE = Out of range
    NAND_OK = Block is good */
CsrInt32 NANDBlockIsBad(CsrUint32 block)
{
    CsrUint32 page;
    CsrUint8 data[2];

    if (block == 0)
    {
        return NAND_OK;
    }

    if (block >= NAND_BLOCKS)
    {
        return NAND_ERROR_OUT_OF_RANGE;
    }

    page = block * NAND_PAGESPERBLOCK;

    data[0] = NANDRandomRead(page, NAND_PAGESIZE);

    page++;

    data[1] = NANDRandomRead(page, NAND_PAGESIZE);

    if ((data[0] == 0xFF) && (data[1] == 0xFF))
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
    CsrUint8 buf[4];

    /* Check if out of range */
    if ((page >= NAND_PAGES) || ((page / NAND_PAGESPERBLOCK) == 0))
    {
        return NAND_ERROR_OUT_OF_RANGE;
    }

    ECC_TYPE_1BIT();

    ECC_MAIN_RESET();   /* Initialise ECC decoder/encoder */
    ECC_MAIN_UNLOCK();  /* Unlock Main data area ECC generation */

    RNB_DETECT_CLEAR();

    CHIP_ENABLE_ATOMIC();

    COMMAND(CMD_READ);

    FULL_ADDRESS(0, page);

    COMMAND(CMD_READ >> 8);

    CHIP_DISABLE_ATOMIC();

    while (!RNB_DETECT())
    {
    }

    /* tRR > 20ns */
    DelayUs(1);

    /* Read out the entire page to the buffer */
    ptr = buffer;
    if (ptr != NULL)
    {
        for (i = 0; i < NAND_PAGESIZE; i++)
        {
            CHIP_ENABLE_ATOMIC();
            *ptr++ = DATA_READ8();
            CHIP_DISABLE_ATOMIC();
        }
    }
    else
    {
        for (i = 0; i < NAND_PAGESIZE; i++)
        {
            CHIP_ENABLE_ATOMIC();
            buf[0] = DATA_READ8();
            CHIP_DISABLE_ATOMIC();
        }
    }

    ECC_MAIN_LOCK();    /* Lock main data area ECC generation */

    CHIP_ENABLE_ATOMIC();

    buf[0] = DATA_READ8();    /* Dummy read cycle to skip the bad block marker */
    buf[0] = DATA_READ8();
    buf[1] = DATA_READ8();
    buf[2] = DATA_READ8();
    buf[3] = DATA_READ8();

    CHIP_DISABLE_ATOMIC();

    rNFMECCD0 = (buf[1] << 16) | buf[0];
    rNFMECCD1 = (buf[3] << 16) | buf[2];

    switch (rNFECCERR0 & 0x3)
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

    if ((block >= NAND_BLOCKS) || (block == 0))
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
