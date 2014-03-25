/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

                All rights reserved and confidential information of CSR

REVISION:       $Revision: #2 $
***************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_SPP_MODULE

#include "csr_pmem.h"
#include "csr_bt_spp_prim.h"

void CsrBtSppServiceNameIndPrimFree(CsrBtSppPrim *prim)
{
    CsrBtSppServiceNameInd *p;

    p = (CsrBtSppServiceNameInd *)prim;

    CsrPmemFree(p->serviceNameList);
    p->serviceNameList = NULL;
}


#endif

