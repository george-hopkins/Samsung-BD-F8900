/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

                All rights reserved and confidential information of CSR

REVISION:       $Revision: #2 $
***************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_MAPS_MODULE

#include "csr_pmem.h"
#include "csr_bt_maps_prim.h"

void CsrBtMapsSelectMasInstanceIndPrimFree(CsrBtMapsPrim *prim)
{
    CsrBtMapsSelectMasInstanceInd *p;

    p = (CsrBtMapsSelectMasInstanceInd *)prim;

    CsrPmemFree(p->masInstanceList);
    p->masInstanceList = NULL;
}

#endif
