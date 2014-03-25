/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_msg_transport.h"
#include "csr_bt_sapc_lib.h"

void CsrBtSapcMsgTransport(void* msg)
{
    CsrMsgTransport(CSR_BT_SAPC_IFACEQUEUE, CSR_BT_SAPC_PRIM, msg);
}
