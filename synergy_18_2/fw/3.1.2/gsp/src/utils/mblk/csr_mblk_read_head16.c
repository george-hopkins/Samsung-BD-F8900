/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/*!

\file   CsrMblkReadHead16.c

\brief  Read and remove a CsrUint16 little-endian style from head of MBLK.

*/

#include "csr_mblk_private.h"

#include "csr_types.h"
#include "csr_macro.h"


/*! \brief Read and remove CsrUint16 from head of mblk.

    This is just a convenient wrapper for CsrMblkReadHead()
    and read_uint16(). It will read a CsrUint16 in little-endian
    format from an MBLK, discarding the read data from the MBLK.

    \param mblk MBLK from which to read.
    \param value Pointer to return value that was read.
    \returns TRUE if successful, otherwise FALSE.
*/
CsrBool CsrMblkReadHead16(CsrMblk **mblk, CsrUint16 *value)
{
    CsrUint8 b[2];

    if (CsrMblkReadHead(mblk, b, 2) == 2)
    {
        const CsrUint8 *bp = b;

        *value = CSR_GET_UINT16_FROM_LITTLE_ENDIAN(&bp);

        return TRUE;
    }

    return FALSE;
}
