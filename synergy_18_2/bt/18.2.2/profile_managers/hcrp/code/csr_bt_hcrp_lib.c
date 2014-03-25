/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_msg_transport.h"
#include "csr_bt_profiles.h"
#include "csr_pmem.h"
#include "csr_bt_hcrp_lib.h"

void CsrBtHcrpMsgTransport(void* msg)
{
    CsrMsgTransport(CSR_BT_HCRP_IFACEQUEUE, CSR_BT_HCRP_PRIM, msg);
}
