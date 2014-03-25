/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_bt_bsl_lib.h"
#include "csr_bt_tasks.h"
#include "csr_msg_transport.h"

void CsrBtBslMsgTransport(void* msg)
{
    CsrMsgTransport(CSR_BT_BSL_IFACEQUEUE, CSR_BT_BSL_PRIM, msg);
}
