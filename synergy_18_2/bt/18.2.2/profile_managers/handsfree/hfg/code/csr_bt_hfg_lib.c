/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_bt_hfg_lib.h"
#include "csr_msg_transport.h"
#include "csr_bt_hfg_prim.h"

void CsrBtHfgMsgTransport(void* __msg)
{
    CsrMsgTransport(CSR_BT_HFG_IFACEQUEUE, CSR_BT_HFG_PRIM, __msg);
}

