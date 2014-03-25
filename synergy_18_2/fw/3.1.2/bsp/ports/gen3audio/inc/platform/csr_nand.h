#ifndef CSR_NAND_H__
#define CSR_NAND_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Geometry */
#define NAND_PAGESIZE       2048
#define NAND_PAGES          65536
#define NAND_BLOCKS         1024
#define NAND_BLOCKSIZE      (NAND_PAGESIZE * NAND_PAGES / NAND_BLOCKS)
#define NAND_PAGESPERBLOCK  (NAND_PAGES / NAND_BLOCKS)

/* Calculate the number of pages required to store a specified data size */
#define NAND_CALCULATEPAGES(size)   ((size) % NAND_PAGESIZE ? (size) / NAND_PAGESIZE + 1 : (size) / NAND_PAGESIZE)

/* Error Codes */
#define NAND_OK                      0
#define NAND_ERROR_OUT_OF_RANGE     -1
#define NAND_ERROR_DATACORRUPTION   -2
#define NAND_ERROR_BADBLOCK         -3

/* Locking. Returns the previous state. (note: flash access is locked by default) */
#define NAND_UNLOCKED           0
#define NAND_LOCKED             1
#define NAND_LOCKED_TIGHT       2
CsrUint8 NANDLockSet(CsrUint8 lockState);

/* I/O Access (note: flash access is locked by default) */
CsrInt32 NANDRandomWrite(CsrUint32 page, CsrUint32 column, CsrUint8 data);
CsrUint8 NANDRandomRead(CsrUint32 page, CsrUint32 column);
CsrInt32 NANDBlockIsBad(CsrUint32 block);
CsrInt32 NANDBlockErase(CsrUint32 block);
CsrInt32 NANDBlockMarkBad(CsrUint32 block);
CsrInt32 NANDPageRead(CsrUint32 page, CsrUint8 *buffer);
CsrInt32 NANDPageWrite(CsrUint32 page, CsrUint8 *buffer);
CsrInt32 NANDRead(CsrUint32 startBlock, CsrUint32 limitBlock, CsrUint32 pages, CsrUint8 *data);
CsrInt32 NANDWrite(CsrUint32 startBlock, CsrUint32 limitBlock, CsrUint32 pages, CsrUint8 *data);

/* Retrieve the Vendor Identifier of the Chip */
void NANDIdentifierRead(CsrUint8 id[5]);

/* Block Marking */
#define NAND_MARK_EMPTY         0xFF
#define NAND_MARK_WRITING       0x77
#define NAND_MARK_ACTIVE        0x00
CsrInt32 NANDBlockMarkerSet(CsrUint32 block, CsrUint8 mark);
CsrUint8 NANDBlockMarkerGet(CsrUint32 block);

#ifdef __cplusplus
}
#endif

#endif
