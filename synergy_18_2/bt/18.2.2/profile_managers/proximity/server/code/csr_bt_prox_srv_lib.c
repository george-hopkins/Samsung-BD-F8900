/****************************************************************************

  (c) Cambridge Silicon Radio Limited 2010

  All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
 ****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_profiles.h"
#include "csr_msg_transport.h"
#include "csr_bt_prox_srv_lib.h"

void CsrBtProxSrvMsgTransport(void* msg)
{
    CsrMsgTransport(CSR_BT_PROX_SRV_IFACEQUEUE, CSR_BT_PROX_SRV_PRIM, msg);
}
