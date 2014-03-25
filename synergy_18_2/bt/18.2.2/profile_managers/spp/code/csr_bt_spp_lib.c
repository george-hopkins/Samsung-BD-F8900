/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_bt_spp_lib.h"
#include "csr_msg_transport.h"

void CsrBtSppMsgTransport(CsrSchedQid appHandle, void* msg)
{
    CsrMsgTransport(appHandle, CSR_BT_SPP_PRIM, msg);
}

