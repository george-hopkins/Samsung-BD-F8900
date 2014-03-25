#ifndef CSR_ABCSP_CRC_H__
#define CSR_ABCSP_CRC_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_abcsp.h"

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
NAME
        CsrAbcspCrcInit  -  initialise the crc calculator

FUNCTION
        Write an initial value (0xffff) into *crc.
*/

void CsrAbcspCrcInit(CsrUint16 *crc);


/****************************************************************************
NAME
        CsrAbcspCrcUpdate  -  update crc with next data byte

FUNCTION
        Updates the cyclic redundancy check value held in *crc with the
        next "len" data byte in the current sequence pointed to by "block"
*/

void CsrAbcspCrcUpdate(CsrUint16 *crc, const CsrUint8 *block, CsrUint32 len);


/****************************************************************************
NAME
        CsrAbcspCrcReverse  -  translate crc into big-endian number

RETURNS
        A bit reversed version of crc.
*/

CsrUint16 CsrAbcspCrcReverse(CsrUint16 crc);


/****************************************************************************
NAME
        CsrAbcspCrcBlock  -  calculate a CRC over a block of data

RETURNS
    A crc the correct way round (no need to call CsrAbcspCrcReverse()).
*/

CsrUint16 CsrAbcspCrcBlock(const CsrUint8 *block, CsrUint32 len);

#ifdef __cplusplus
}
#endif

#endif
