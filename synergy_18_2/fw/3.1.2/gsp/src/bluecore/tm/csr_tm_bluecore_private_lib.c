/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/
#include "csr_tm_bluecore_private_prim.h"
#include "csr_tm_bluecore_private_lib.h"
#include "csr_pmem.h"

CsrTmBluecoreResetInd *CsrTmBluecoreResetInd_struct(void)
{
    CsrTmBluecoreResetInd *prim;
    prim = (CsrTmBluecoreResetInd *) CsrPmemAlloc(sizeof(CsrTmBluecoreResetInd));
    prim->type = CSR_TM_BLUECORE_RESET_IND;
    return prim;
}

CsrTmBluecorePanicInd *CsrTmBluecorePanicInd_struct(void)
{
    CsrTmBluecorePanicInd *prim;
    prim = (CsrTmBluecorePanicInd *) CsrPmemAlloc(sizeof(CsrTmBluecorePanicInd));
    prim->type = CSR_TM_BLUECORE_PANIC_IND;
    return prim;
}
