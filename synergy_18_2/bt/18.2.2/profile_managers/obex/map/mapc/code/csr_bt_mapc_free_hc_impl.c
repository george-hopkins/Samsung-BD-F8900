/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

                All rights reserved and confidential information of CSR

REVISION:       $Revision: #2 $
***************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_MAPC_MODULE

#include "csr_pmem.h"
#include "csr_bt_mapc_prim.h"

void CsrBtMapcSelectMasInstanceIndPrimFree(CsrBtMapcPrim *prim)
{
    CsrBtMapcSelectMasInstanceInd *p;

    p = (CsrBtMapcSelectMasInstanceInd*)prim;

    CsrPmemFree(p->masInstanceList);
    p->masInstanceList = NULL;
}

#endif


