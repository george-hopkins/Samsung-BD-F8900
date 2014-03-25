/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_bt_dg_lib.h"
#include "csr_msg_transport.h"

void CsrBtDgMsgTransport(CsrSchedQid qid, void* msg)
{
    CsrMsgTransport(qid, CSR_BT_DG_PRIM, msg);
}
