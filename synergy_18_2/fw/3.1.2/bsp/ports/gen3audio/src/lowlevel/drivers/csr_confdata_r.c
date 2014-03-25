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

/* Get the size of the Configuration Data contained in the specified block */
static CsrInt32 getConfigurationDataSize(CsrUint32 block)
{
    CsrInt32 dataSize;
    CsrUint8 *pageData;

    pageData = (CsrUint8 *) CsrPmemAlloc(NAND_PAGESIZE);
    if (NANDPageRead(block * NAND_PAGESPERBLOCK, pageData))
    {
        /* This will happen if multiple bits are faulty (pages protected by ECC) */
        CsrPmemFree(pageData);
        return CONFDATA_ERROR_READ;
    }
    dataSize = *((CsrUint32 *) pageData) & 0x7FFFFFFF;
    CsrPmemFree(pageData);
    return dataSize;
}

/* Find the first good and active block in the Configuration Data Area */
CsrUint32 ConfDataFindActiveBlock(void)
{
    CsrUint32 block;

    /* Search for first good and active block */
    for (block = CONFDATA_FIRSTBLOCK; block <= CONFDATA_LASTBLOCK; block++)
    {
        if (!NANDBlockIsBad(block) && (NANDBlockMarkerGet(block) == NAND_MARK_ACTIVE))
        {
            break;
        }
    }

    return block;
}

/* Read the Configuration Data from the NAND flash */
CsrInt32 ConfDataRead(CsrUint8 **data)
{
    CsrUint32 block;
    CsrInt32 dataSize;

    /* Default value if read fails */
    *data = (CsrUint8 *) CsrPmemAlloc(4);
    *((CsrUint32 *) *data) = 4;

    /* Find the first good and active block in the Configuration Data Area */
    if ((block = ConfDataFindActiveBlock()) == CONFDATA_INVALIDBLOCK)
    {
        /* All blocks bad or no active blocks */
        return CONFDATA_ERROR_READ;
    }

    if ((dataSize = getConfigurationDataSize(block)) < 0)
    {
        /* dataSize contains standard error code if negative */
        return dataSize;
    }

    /* Check if all data is contained in a single block */
    if (dataSize <= NAND_BLOCKSIZE)
    {
        CsrInt32 pages;
        CsrUint8 *temp;

        /* Calculate number of pages */
        pages = NAND_CALCULATEPAGES(dataSize);

        temp = (CsrUint8 *) CsrPmemAlloc(pages * NAND_PAGESIZE);
        if (NANDRead(block, block, pages, temp))
        {
            CsrPmemFree(temp);
            return CONFDATA_ERROR_READ;
        }

        CsrPmemFree(*data);
        *data = temp;
        return CONFDATA_OK;
    }
    else
    {
        /* Configuration data is too large (must be caused by some sort of corruption) */
        return CONFDATA_ERROR_READ;
    }
}

/* Read the Configuration Data from the NAND flash */
CsrInt32 ConfDataDiscard(CsrUint8 *data)
{
    CsrPmemFree(data);
    return CONFDATA_OK;
}

CsrInt32 ConfDataGetValue(CsrUint8 *data, CsrUint16 tag, CsrUint16 *length, CsrUint8 *value)
{
    CsrUint8 *dataptr;
    CsrUint32 dataSize;

    if (length)
    {
        *length = 0;
    }

    if ((dataSize = *((CsrUint32 *) data)) <= 4)
    {
        return CONFDATA_ERROR_NOTFOUND;
    }

    dataptr = data + 4;

    while ((dataptr[0] + (dataptr[1] << 8)) != tag)
    {
        /* Read length of current value and skip it */
        dataptr += dataptr[2] + (dataptr[3] << 8) + 2 + 2;

        if ((dataptr >= data + dataSize))
        {
            return CONFDATA_ERROR_NOTFOUND;
        }
    }

    /* Found tag */
    if (length)
    {
        *length = dataptr[2] + (dataptr[3] << 8);
    }
    if (value)
    {
        memcpy(value, dataptr + 2 + 2, dataptr[2] + (dataptr[3] << 8));
    }

    return CONFDATA_OK;
}
