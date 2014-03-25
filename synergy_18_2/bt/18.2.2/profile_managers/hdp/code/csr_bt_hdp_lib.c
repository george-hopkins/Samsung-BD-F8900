
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_HDP_MODULE

#include "csr_msg_transport.h"
#include "csr_bt_hdp_lib.h"

void CsrBtHdpMsgTransport(void *msg)
{
    CsrMsgTransport(CSR_BT_HDP_IFACEQUEUE, CSR_BT_HDP_PRIM, msg);
}

#endif /* EXCLUDE_CSR_BT_HDP_MODULE */
