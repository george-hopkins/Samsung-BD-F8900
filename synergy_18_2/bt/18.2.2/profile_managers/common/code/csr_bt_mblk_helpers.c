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

/* Convert an MBLK to normal pmem, destroying it in the process */
CsrUint8 *CsrBtMblkConsumeToMemory (CsrMblk **pp_mblk)
{
    CsrUint16 len;
    CsrUint8  *data = NULL;

    CsrMblk *p_mblk = *pp_mblk;

    /* Get length of MBLK chain */
    len = CsrMblkGetLength(p_mblk);

    if (len != 0)
    {
        data = (CsrUint8 *)CsrPmemAlloc(len);
        CsrMblkCopyToMemory(p_mblk, 0, len, data);
    }

    CsrMblkDestroy(p_mblk);
    *pp_mblk = NULL;

    return data;
}

/* Read and remove uint8_t from head of mblk */
CsrBool CsrBtMblkReadHead8(CsrMblk **mblk, CsrUint8 *value)
{
    return CsrMblkReadHead(mblk, value, 1) == 1;
}

/* Read and remove uint16_t from head of mblk */
CsrBool CsrBtMblkReadHead16(CsrMblk **mblk, CsrUint16 *value)
{
    CsrUint8 b[2];

    if(CsrMblkReadHead(mblk, b, 2) == 2)
    {
        *value = (CsrUint16)(b[0] | (b[1]<<8));
        return TRUE;
    }
    return FALSE;
}

/* Read and remove multiple uint8_ts from head of mblk */
CsrBool CsrBtMblkReadHead8s(CsrMblk **mblk, unsigned int num, ...)
{
    va_list ap;
    CsrUint8 *buffer;
    CsrBool success = FALSE;

    va_start(ap, num);
    buffer = CsrPmemAlloc(num);

    if (CsrMblkReadHead(mblk, buffer, (CsrUint16)num) == num)
    {
        CsrUint8 *ptr;

        for (ptr = buffer; ptr != buffer + num; ++ptr)
            *(va_arg(ap, CsrUint8*)) = *ptr;

        success = TRUE;
    }

    CsrPmemFree(buffer);
    va_end(ap);

    return success;   
}
