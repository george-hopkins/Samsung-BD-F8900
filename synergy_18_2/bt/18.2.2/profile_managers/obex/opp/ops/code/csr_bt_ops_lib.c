/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_profiles.h"
#include "csr_bt_ops_lib.h"
#include "csr_msg_transport.h"

void CsrBtOpsMsgTransport(void* msg)
{
    CsrMsgTransport(CSR_BT_OPS_IFACEQUEUE, CSR_BT_OPS_PRIM, msg);
}

