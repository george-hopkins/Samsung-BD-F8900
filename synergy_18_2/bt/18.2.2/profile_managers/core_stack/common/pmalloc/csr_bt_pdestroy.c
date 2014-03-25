/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "csr_pmem.h"

void pdestroy_array(void **ptr, CsrUint16 num)
{
    while (num--)
    {
        CsrPmemFree(*ptr);
        *ptr = NULL;
        ptr++;
    }
}

