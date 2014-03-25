/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_util.h"
#include "csr_bt_assert.h"
#include "csr_bt_common.h"

/* Reverse array byte-for-byte (12345 -> 54321) */
void CsrBtArrayReverse(void *array, CsrUint16 length)
{
    CsrUint8 *src = (CsrUint8*)array;
    CsrUint8 *dst = src + length;
    CsrUint8 tmp;

    while(length > 1)
    {
        dst--;
        tmp = *dst;
        *dst = *src;
        *src = tmp;
        src++;
        length -= 2;
    }
}

void CsrBtMemCpyPack(CsrUint16 *dest, const CsrUint8 *source, CsrUint16 length)
{
    while (length-- != 0)
    {
        *dest++ = read_uint16(&source);
    }
}

void CsrBtMemCpyUnpack(CsrUint8 *dest, const CsrUint16 *source, CsrUint16 length)
{
    while (length-- != 0)
    {
        write_uint16(&dest, *source++);
    }
}
