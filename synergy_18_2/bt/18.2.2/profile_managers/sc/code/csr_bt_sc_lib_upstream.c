/****************************************************************************

    Copyright Cambridge Silicon Radio Limited and its licensors 2011.
            All rights reserved.

REVISION:      $Revision: #3 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_sched.h"
#include "csr_types.h"
#include "csr_bt_sc_lib_upstream.h"

void CsrBtScMessagePut(CsrSchedQid phandle, void* msg)
{
    CsrSchedMessagePut(phandle, CSR_BT_SC_PRIM, msg);
}

