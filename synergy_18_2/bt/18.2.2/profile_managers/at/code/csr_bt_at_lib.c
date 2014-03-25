/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_AT_MODULE

#include "csr_bt_tasks.h"
#include "csr_bt_at_prim.h"
#include "csr_pmem.h"
#include "csr_msg_transport.h"

void CsrBtAtDgMsgTransport(void* msg)
{
    CsrMsgTransport(CSR_BT_AT_IFACEQUEUE, CSR_BT_AT_PRIM, msg);
}

#endif

